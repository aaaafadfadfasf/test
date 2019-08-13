/*****************************************************************************
模块名      : cmu 
文件名      : invitesenderswitchtask.cpp
相关文件    : 
文件实现功能: CInviteSenderSwitchTask实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
2012/03/30     1.0        黄至春       重构交换逻辑：把之前的一个交换任务分成Src和Dst两个交换任务
******************************************************************************/

#include "invitesenderswitchtask.h"
#include "vtduregtask.h"

CInviteSenderSwitchTask::CInviteSenderSwitchTask(CInstExt *pcInst) : CSwitchTask(pcInst)
{
	m_bDelayPuAck = false;
}

CInviteSenderSwitchTask::~CInviteSenderSwitchTask()
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "析构CInviteSenderSwitchTask, this[0x%X],taskNo[%d] \n", this, this->GetTaskNO() );
}


void CInviteSenderSwitchTask::InitStateMachine()
{
    CStateProc cStateProc;
    cStateProc.ProcMsg = (CTask::PFProcMsg)&CInviteSenderSwitchTask::OnState;
    cStateProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteSenderSwitchTask::OnState;
    cStateProc.TimerPoll = (CTask::PFTimerPoll)&CInviteSenderSwitchTask::OnStateTimer;

    AddRuleProc(Init, cStateProc);
    AddRuleProc(Starting, cStateProc);
    AddRuleProc(InviteSender, cStateProc);
    AddRuleProc(Service, cStateProc);

    NextState(Init);
}

//状态处理
u32 CInviteSenderSwitchTask::OnState(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
		//TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
		//nErrorCode = ERR_CMU_SIP_BODY_NULL;
		return PROCMSG_FAIL;
	}

	//////////////////////////////////////////////////
	//{{通用消息处理
	switch(pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			ProcOspSipException(pcMsg);
			if (Service == GetState())
			{
				dwRet = PROCMSG_OK;
			}
			else
			{
				dwRet = PROCMSG_DEL;		
			}
			return dwRet;
		}
		break;

    case OSP_SIP_DIALOG_TERMINATE:
        {
            ProcOspSipException(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;

	case OSP_SIP_BYE_REQ:
	case BYE_REQ:
		{
			ProcByeReq(pcMsg);

			dwRet = PROCMSG_DEL;
			return dwRet;
		}
		break;

    case CMU_CMU_DISCONNECT:
    case CMU_CMU_DELETE:
        //case OSP_SIP_DISCONNECT:
        {
            TDevOnOffCBMsg tDevOnOffCBMsg;
            tDevOnOffCBMsg.SetDevType(DEV_TYPE_CMU);
            tDevOnOffCBMsg.SetDevUri(pcOspSipMsg->GetMsgBody());

            ProcDisconnect(&tDevOnOffCBMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case CMU_DEV_OFFLINE_NTF:
        {
            ProcDisconnect((const TDevOnOffCBMsg*)pcMsg->content);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;

	case OSP_SIP_BYE_RSP:
	case BYE_RSP:
		{
			if (ProcByeRsp(pcOspSipMsg) != CMS_SUCCESS)
			{
				dwRet = PROCMSG_FAIL;
			}

			return dwRet;
		}
		break;

    default:
        {
        }
        break;
	}
	//}}通用消息处理


	//////////////////////////////////////////////////
	//状态消息处理
	dwRet = PROCMSG_OK;

	switch(GetState())
	{
	case Init:
		{
			//switch(pcMsg->event)
			//{
			//case INVITE_REQ:
			//	{
			//		if (ProcInviteReq(pcOspSipMsg) != CMS_SUCCESS)
			//		{
			//			dwRet = PROCMSG_DEL;
			//		}
			//		else
			//		{
			//			NextState(Starting);
			//		}
			//	}
			//	break;

			//default:
			//	{
			//		dwRet = PROCMSG_FAIL;
			//		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			//	}
			//	break;
			//}
		}
		break;

	case Starting:
		{
			switch(pcMsg->event)
			{
			case INVITE_RSP:
				{
					if (ProcInviteRsp(pcOspSipMsg) != CMS_SUCCESS)
					{
						dwRet = PROCMSG_DEL;
					}
					else
					{
						NextState(InviteSender);
					}
				}
				break;

			default:
				{
					dwRet = PROCMSG_FAIL;
					TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				break;
			}
		}
		break;

	case InviteSender:
		{
			switch(pcMsg->event)
			{
			case INVITE_RSP:
				{
					if (ProcInviteRsp(pcOspSipMsg) != CMS_SUCCESS)
					{
						dwRet = PROCMSG_DEL;
					}
					else
					{
						NextState(Service);
					}
				}
				break;

			default:
				{
					dwRet = PROCMSG_FAIL;
					TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				break;
			}
		}
		break;

    case Service:
        {
            switch(pcMsg->event)
            {
			case UPDATE_SWITCH_ADDRESS_REQ:
				{
					CUpdateSwitchAddressReq cReq;
					pcOspSipMsg->GetMsgBody(cReq);

					//先回复PUI（CUI）收到
					CUpdateSwitchAddressRsp cRsp;
					u32 dwRet = PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
					{
						dwRet = PROCMSG_FAIL;
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，应答PUI消息失败\n");
					}

					if (m_pcInput == NULL)  //源交换
					{
						//再投递给VTDU
						if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cReceiver.GetDlgID()))
						{
							dwRet = PROCMSG_FAIL;
							TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，消息投递给VTDU失败\n");
						}
					}
					else // 音频呼叫的目的交换（pui侧发送过来的，vtdu是发送者）
					{
						//再投递给VTDU
						if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cSender.GetDlgID()))
						{
							dwRet = PROCMSG_FAIL;
							TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，消息投递给VTDU失败\n");
						}
					}
				}
				break;

			case UPDATE_SWITCH_ADDRESS_RSP:
				{
					CUpdateSwitchAddressRsp cRsp;
					pcOspSipMsg->GetMsgBody(cRsp);
					if (cRsp.GetErrorCode() == CMS_SUCCESS)
					{
						TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "更新交换地址成功\n");
					}
					else
					{
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址失败，错误码[%d]\n", 
							cRsp.GetErrorCode());
					}
				}
				break;

            case END_OF_FILE_NTF_REQ:
                {
                    //pui的通知请求
                    dwRet = ProcInDlgNtfReq<CEndOfFileNtfReq, CEndOfFileNtfRsp>(pcMsg);                   
                }
                break;

            case PLAYBACK_PROGRESS_NTF_REQ:
                {
                    //pui的通知请求
                    dwRet = ProcInDlgNtfReq<CPlaybackProgressNtfReq, CPlaybackProgressNtfRsp>(pcMsg);                    
                }
                break;

            case VCR_CTRL_REQ:
                {
                    if (NULL == m_pcInput)          // 作为源交换task
                    {
                        //SendSwitchTask转来的消息
                        CVcrCtrlReq cReq;
                        pcOspSipMsg->GetMsgBody(cReq);
                        if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cSender.GetDlgID()))
                        {
                            dwRet = PROCMSG_FAIL;
                            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "消息投递失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                        }
                    }
                    else                            // 作为目的交换task
                    {
                        CPuVcrCtrlTask* pcVcrTask = new CPuVcrCtrlTask(GetInstance(), this);
                        pcVcrTask->InitStateMachine();
                        if (pcVcrTask->ProcMsg(pcMsg) != PROCMSG_OK)
                        {
                            //只要处理失败就删除
                            delete pcVcrTask;
                            pcVcrTask = NULL;
                        }
                    }
                }
                break;

            case VCR_CTRL_RSP:
                {
                    if (NULL == m_pcInput)          // 作为源交换task
                    {
                        //pui的操作应答
                        CSwitchTaskList& cTaskList = GetOutputList();
                        CSwitchTask* pcTask = (CSwitchTask*)cTaskList.front();
                        if (pcTask != NULL)
                        {
                            pcTask->ProcMsg(pcMsg);
                        }
                        else
                        {
                            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "获取目的交换事务失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                        }
                    }
                    else                            // 作为目的交换task
                    {
                        CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
                        if (pcTask != NULL)
                        {
                            if (pcTask->ProcMsg(pcMsg) == PROCMSG_DEL)
                            {
                                delete pcTask;
                                pcTask = NULL;
                            }
                        }
                    }
                }
                break;

            //case KEY_FRAME_REQ:
            //    {
            //        if (ProcKeyFrameReq(pcOspSipMsg) != CMS_SUCCESS)
            //        {
            //            dwRet = PROCMSG_FAIL;
            //        }
            //    }
            //    break;
            //case KEY_FRAME_RSP:
            //    {
            //        if (ProcKeyFrameRsp(pcOspSipMsg) != CMS_SUCCESS)
            //        {
            //            dwRet = PROCMSG_FAIL;
            //        }
            //    }
            //    break;

            default:
                {
                    dwRet = PROCMSG_FAIL;
                    TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                }
                break;
            }
        }
        break;

	default:
		{
		}
		break;
	}

	return dwRet;
}

u32 CInviteSenderSwitchTask::OnStateTimer()
{
    if (GetState()!= Service && GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "交换超时，删除交换\n");
		//PrintData();

        this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);
       return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//开始交换业务逻辑
////////////////////////////////////////////////////////////////////////////////////////////////
CMS_RESULT CInviteSenderSwitchTask::Start()
{
    //Receiver.Sdp没有准备就绪，先Invite Receiver
    if(!m_cReceiver.GetSdp().HasMediaDes())
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver.Sdp is not ready，firstly Invite Receiver[%s].\n", m_cReceiver.GetChannel().ToString().c_str());

		string strStreamTag;
		if (NULL != m_pcInput && m_pcInput->GetSender().GetSdp().GetExtInfo(SDP_EXT_INFO_STREAM_TAG, strStreamTag))
		{
			m_cReceiver.GetSdp().SetExtInfo(SDP_EXT_INFO_STREAM_TAG, strStreamTag);
		}

        // 设置资源id
        m_cReceiver.GetSdp().SetResourceId(m_tVtduResourceId);
        m_cReceiver.GetSdp().SetSwitchFlow(SWITCH_FLOW_CMU);

        InviteTo(m_cReceiver, m_cReceiver.GetSdp());
        NextState(Starting);
    }
    else
    {
        //m_cReceiver.GetSdp().PrintData(false);

        if(m_pcInput)
        {
            if(m_pcInput->GetSwitchState() == SWITCH_STATE_SWITCHED)
            {
                TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InputNode has switch complete，StartSwitchToSrc[%s].\n", m_pcInput->GetChannel().ToString().c_str());

                StartSwitchToSrc(m_pcInput);
                NextState(InviteSender);
            }
            else
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "InputNode in a middle state: [%s], IGNORE!\n", GetSwitchStateStr(m_pcInput->GetSwitchState()));
            }
        }
        else if(!m_cSender.GetSdp().HasMediaDes())
        {
            //目前不会走到这个流程，因为InviteSender在ProcInviteRsp中处理了。
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Without InputNode. Sender.Sdp is not ready, Invite Sender[%s].\n", m_cSender.GetChannel().ToString().c_str());
            InviteTo(m_cSender, m_cReceiver.GetSdp());
            NextState(InviteSender);
        }
        else
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Without InputNode. Receiver.Sdp[%s] and Sender.Sdp[%s] are all ready!.\n",
                m_cReceiver.GetChannel().ToString().c_str(), m_cSender.GetChannel().ToString().c_str());
        }
    }

    return this->GetErrorCode();
}

CMS_RESULT CInviteSenderSwitchTask::ProcInviteRsp(const COspSipMsg*const pcOspSipMsg)
{
    CInviteRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);
    TSdp& tRspSdp = cRsp.GetSdp();

	m_pcCurInviting->SetDlgID(pcOspSipMsg->GetSipDlgID());

    CMS_RESULT nErrorCode = cRsp.GetErrorCode();

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "收到来自[%s] DlgId[%lu]的InviteRsp，ErrorCode[%ld]！\n",
        m_pcCurInviting->GetDevDesc().c_str(), pcOspSipMsg->GetSipDlgID(), nErrorCode);

    if(CMS_SUCCESS != nErrorCode)
    {
        this->SetErrorCode(nErrorCode);
        return this->GetErrorCode();
    }


    //{{参数校验
    if (!tRspSdp.HasMediaDes())
    {
        CMS_ASSERT(tRspSdp.HasMediaDes());

        this->SetErrorCode(ERR_INVALIAD_SDP);
        return this->GetErrorCode();
    }

    if(tRspSdp.GetOwner() != m_pcCurInviting->GetChannel())
    {
        //[HZCTO:是否要移到CSwitchPin::SetSdp中]
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "非预期的Sdp.Owner[%s]纠正为CurInviting(SwitchPin.Chn)[%s]！\n", 
            tRspSdp.GetOwner().ToString().c_str(),
            m_pcCurInviting->GetChannel().ToString().c_str());

        tRspSdp.SetOwner(m_pcCurInviting->GetChannel());
    }

    if(tRspSdp.GetSessionName() != this->GetSessionName())
    {
        //[HZCTO:是否要移到CSwitchPin::SetSdp中]
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "非预期的Sdp.SessionName[%s]纠正为this->GetSessionName[%s]！\n", 
            tRspSdp.GetSessionName().c_str(),
            this->GetSessionName().c_str());

        tRspSdp.SetSessionName(this->GetSessionName());
    }
    //}}参数校验

	//{{{{{{{{{{{{{{{ add by Royan 20140903 SSRC
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "ProcInviteRsp, SSRC[%d], SessionName[%s], DevType[%s]\n",
		this->GetSender().GetSdp().GetSSRC(), m_cSender.GetSdp().GetSessionName().c_str(), m_cSender.GetDevType().c_str());

    if (SDP_SESSION_AUDIOCALL != tRspSdp.GetSessionName())
    {
        tRspSdp.SetSSRC(this->GetSender().GetSdp().GetSSRC());
    }
	
	//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC
    m_pcCurInviting->SetSdp(tRspSdp);
	//{{{{{{{{{{{{{{{ modify by Royan 20140708 国标流保活支持上下行保活
	if (!tRspSdp.GetGBStreamKAInfo().empty())
	{
		m_pcCurInviting->GetSdp().SetGBStreamKAInfo(tRspSdp.GetGBStreamKAInfo());
	}
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "ProcInviteRsp, RspSdp KAInfo[%s]\n",
		tRspSdp.GetGBStreamKAInfo().c_str());
	//}}}}}}}}}}}}}}} modify by Royan 20140708 国标流保活支持上下行保活
	
	//判断另一端是否获取完MediaDesc
	CSwitchPin& cSwitchPin = GetSwitchPin(m_pcCurInviting->GetDlgID(), false);
	if(!cSwitchPin.GetSdp().HasMediaDes())
	{
        if(this->GetInput())
        {
            StartSwitchToSrc(this->GetInput());
        }
        else
        {
            InviteTo(cSwitchPin, m_pcCurInviting->GetSdp());    //HZC:相当于InviteTo(cSwitchPin, m_cReceiver.GetSdp());
        }

		return this->GetErrorCode();
	}
    //双方都回了InviteRsp
	else
	{
        TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "媒体交换完成，给双方发ACK.\n");

		PostInviteAck(m_cReceiver, m_cSender.GetSdp());
		//级联音频呼叫需要ACK修改8002端口
		//PostInviteAck(m_cSender, TSdp());
		if (SDP_SESSION_AUDIOCALL == m_cSender.GetSdp().GetSessionName() && DEV_TYPE_CU == m_cSender.GetDevType())
		{
			TSdp tSdp = m_cReceiver.GetSdp();
			tSdp.SetIsNatPacketMode(m_cSender.GetSdp().GetIsNatPacketMode());
			if (tSdp.GetIsNatPacketMode())
			{
				u16 wVtduPort = (u16)VTDU_MEDIA_UDP_M2P_PORT;
				if (DEV_TYPE_VTDU == m_cReceiver.GetDevType())
				{
					CLoginSession* ptLoginSession = NULL;
                    if (IsLocalDomainByDevUri(m_cSender.GetChannel().GetDevUri()))
					{
						if (g_cVtduList.Find(m_cSender.GetChannel().GetDevUri(), ptLoginSession))
						{
							wVtduPort = ((CVtduRegTask*)ptLoginSession)->GetMediaM2PPort();
							TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "vtdu(%s) MediaM2PPort(%u).\n", 
								m_cReceiver.GetChannel().GetDevUri().c_str(),
								wVtduPort);
						}
					}
				}

				TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
				for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
					itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
				{
					TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
					for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
						itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
					{
						if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
						{
							TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
							for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
								itrNatAddr != tNatAddrList.end();itrNatAddr++)
							{
								itrNatAddr->SetNetPort(wVtduPort);//todo
							}
						}
					}
				}
			}

			PostInviteAck(m_cSender, tSdp);
		}
		else if (SDP_SESSION_AUDIOCALL != m_cSender.GetSdp().GetSessionName()
			&& DEV_TYPE_PU == m_cSender.GetDevType()
			&& NULL == m_pcInput)	// 第三方呼叫和音频呼叫有两个CInviteSenderSwitchTask，这三个条件确定了该CInviteSenderSwitchTask是源交换
		{
			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "源交换建立完成，延迟发送Ack给PU，等到目的交换建立完成后发送Ack！\n");

			// cu进行实时浏览的话，延迟发送源交换的ack，等到目的交换完成后再发
			m_bDelayPuAck = true;
		}
		else
		{
			PostInviteAck(m_cSender, TSdp());
		}
	}

    return this->GetErrorCode();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//其他
////////////////////////////////////////////////////////////////////////////////////////////////
void CInviteSenderSwitchTask::PrintData(ESwitchTrace eTrace)const
{
    CSwitchTask::PrintData(eTrace);
}
