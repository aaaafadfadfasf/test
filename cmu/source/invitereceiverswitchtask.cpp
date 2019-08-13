/*****************************************************************************
模块名      : cmu 
文件名      : invitereceiverswitchtask.cpp
相关文件    : 
文件实现功能: CInviteReceiverSwitchTask实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
2012/03/30     1.0        黄至春       重构交换逻辑：把之前的一个交换任务分成Src和Dst两个交换任务
******************************************************************************/

#include "invitereceiverswitchtask.h"
#include "vtduregtask.h"

CInviteReceiverSwitchTask::CInviteReceiverSwitchTask(CInstExt *pcInst) : CSwitchTask(pcInst)
{
    
}

CInviteReceiverSwitchTask::~CInviteReceiverSwitchTask()
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "析构CInviteReceiverSwitchTask, this[0x%X],taskNo[%d] \n", this, this->GetTaskNO() );
}


void CInviteReceiverSwitchTask::InitStateMachine()
{
    CStateProc cStateProc;
    cStateProc.ProcMsg = (CTask::PFProcMsg)&CInviteReceiverSwitchTask::OnState;
    cStateProc.ProcErrMsg = (CTask::PFProcMsg)&CInviteReceiverSwitchTask::OnState;
    cStateProc.TimerPoll = (CTask::PFTimerPoll)&CInviteReceiverSwitchTask::OnStateTimer;

    AddRuleProc(Init, cStateProc);
    AddRuleProc(Starting, cStateProc);
    AddRuleProc(Ack, cStateProc);
    AddRuleProc(Service, cStateProc);

    NextState(Init);
}

//状态处理
u32 CInviteReceiverSwitchTask::OnState(CMessage *const pcMsg)
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
			switch(pcMsg->event)
			{
			case INVITE_REQ:
				{
                    //获取消息体
                    ProcSipReq2<CInviteReq, CInviteRsp>(pcMsg);

                    CMS_RESULT nErrorCode = ProcInviteReq();
                    if (ERR_CMU_CMU_NOT_CONN == nErrorCode)
                    {
                        break;
                    }
                    else if (CMS_SUCCESS != nErrorCode)
                    {
                        dwRet = PROCMSG_DEL;
                    }
                    else
                    {
                        NextState(Starting);
                    }
				}
				break;

            case CMU_CMU_CONNECT_SUCCESS:
                {
                    if (ProcInviteReq() != CMS_SUCCESS)
                    {
                        dwRet = PROCMSG_DEL;
                    }
                    else
                    {
                        NextState(Starting);
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
						//NextState(Ack);  //在ProcInviteRsp里面切换状态
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

	case Ack:
		{
			switch(pcMsg->event)
			{
			case INVITE_ACK:
				{
					if (ProcInviteAck(pcOspSipMsg) != CMS_SUCCESS)
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

					//先回复CUI收到
					CUpdateSwitchAddressRsp cRsp;
					if (PROCMSG_OK != PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp))
					{
						dwRet = PROCMSG_FAIL;
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，应答CUI消息失败\n");
					}

					if (m_pcInput != NULL)  //目的交换
					{
						//再投递给VTDU
						if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cSender.GetDlgID()))
						{
							dwRet = PROCMSG_FAIL;
							TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，消息投递给VTDU失败\n");
						}
					}
					else
					{
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "更新交换地址，不存在的交换场景\n");
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
                    //SendSwitchTask转来的消息
                    CEndOfFileNtfReq cReq;
                    pcOspSipMsg->GetMsgBody(cReq);
                    if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cReceiver.GetDlgID()))
                    {
                        dwRet = PROCMSG_FAIL;
                        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "消息投递失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    }
                }
                break;

            case END_OF_FILE_NTF_RSP:
                {
                    CEndOfFileNtfRsp cRsp;
                    pcOspSipMsg->GetMsgBody(cRsp);
                    if (cRsp.GetErrorCode() == CMS_SUCCESS)
                    {
                        TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "放像结束通知成功\n");
                    }
                    else
                    {
                        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "放像结束通知失败，错误码[%d]\n", 
                            cRsp.GetErrorCode());
                    }
                }
                break;

            case PLAYBACK_PROGRESS_NTF_REQ:
                {
                    //SendSwitchTask转来的消息
                    CPlaybackProgressNtfReq cReq;
                    pcOspSipMsg->GetMsgBody(cReq);

                    if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cReceiver.GetDlgID()))
                    {
                        dwRet = PROCMSG_FAIL;
                        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "消息投递失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    }
                }
                break;

            case PLAYBACK_PROGRESS_NTF_RSP:
                {
                    CPlaybackProgressNtfRsp cRsp;
                    pcOspSipMsg->GetMsgBody(cRsp);
                    if (cRsp.GetErrorCode() == CMS_SUCCESS)
                    {
                        TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "放像进度通知成功\n");
                    }
                    else
                    {
                        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "放像进度通知失败，错误码[%d]\n", 
                            cRsp.GetErrorCode());
                    }
                }
                break;

            case VCR_CTRL_REQ:
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
                break;

            case VCR_CTRL_RSP:
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
                break;

//             case PLAYBACK_PROGRESS_QUERY_RSP:
//                 {
//                     CPlaybackProgressQueryRsp cRsp;
//                     pcOspSipMsg->GetMsgBody(cRsp);
//                     if (PostInfoReq(m_cReceiver, cRsp.ToXml()) != CMS_SUCCESS)
//                     {
//                         TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "消息投递失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
//                     }
//                 }
//                 break;         

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

u32 CInviteReceiverSwitchTask::OnStateTimer()
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
CMS_RESULT CInviteReceiverSwitchTask::Start()
{
    //Receiver.Sdp没有准备就绪，先Invite Receiver
    if(!m_cReceiver.GetSdp().HasMediaDes())
    {
        //警告：CInviteReceiverSwitchTask不应该走到这个流程
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver.Sdp is not ready，firstly Invite Receiver[%s].\n", m_cReceiver.GetChannel().ToString().c_str());

        InviteTo(m_cReceiver, m_cReceiver.GetSdp());
        NextState(Starting);
    }
    else
    {
        //SWITCH_MODE_SNG交换回走这个流程
        if(m_pcInput)
        {
            if(m_pcInput->GetSwitchState() == SWITCH_STATE_SWITCHED)
            {
                TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InputNode has switch complete，StartSwitchToSrc[%s].\n", m_pcInput->GetChannel().ToString().c_str());

                StartSwitchToSrc(m_pcInput);
                NextState(Starting);
            }
            else
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "InputNode in a middle state: [%s], IGNORE!\n", GetSwitchStateStr(m_pcInput->GetSwitchState()));
            }
        }
        //SWITCH_MODE_DRT直传交换走这个流程
        else if(!m_cSender.GetSdp().HasMediaDes())
        {
            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Without InputNode. Sender.Sdp is not ready, Invite Sender[%s].\n", m_cSender.GetChannel().ToString().c_str());
            InviteTo(m_cSender, m_cReceiver.GetSdp());
            NextState(Starting);
        }
        else
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Without InputNode. Receiver.Sdp[%s] and Sender.Sdp[%s] are all ready!.\n",
                m_cReceiver.GetChannel().ToString().c_str(), m_cSender.GetChannel().ToString().c_str());
        }
    }

    return this->GetErrorCode();
}

CMS_RESULT CInviteReceiverSwitchTask::ProcInviteReq()
{
    this->SetErrorCode(CMS_SUCCESS);

    CInviteReq& cInviteReq = *this->GetRealReq<CInviteReq>();

    const TChannel& tSrcChn = cInviteReq.GetSrcChn();
    const TChannel& tDstChn = cInviteReq.GetDstChn();
    TSdp& tSdp = cInviteReq.GetSdp();

    TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "InviteReq from Session[%s]: Dst[%s] Src[%s]!\n", 
        cInviteReq.GetSession().c_str(), tDstChn.ToString().c_str(), tSrcChn.ToString().c_str());

	if (tSdp.GetIsNatPacketMode())
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "InviteReq sdp is NATPacketMode, set IsNeedPunch false!\n");
		tSdp.SetIsNeedPunch(false);
	}

    do 
    {
        if(!cInviteReq.GetSession().empty())
        {
            //容错处理，如果没有填Sdp.Owner,则补上
            if(tSdp.GetOwner().IsNull())
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "非预期的Sdp.Owner[%s]纠正为Session+Chn[%s_0]！\n", 
                    tSdp.GetOwner().ToString().c_str(), cInviteReq.GetSession().c_str());

                //这里的通道默认始终是0，所以不是很精确，可以考虑后续Owner与DstChn/SrcChn的判断只判断Uri.
                tSdp.SetOwner( TChannel(cInviteReq.GetSession(), 0) );
            }
        }

        CMS_RESULT nErrorCode = CSwitchTask::StartSwitch(tDstChn, tSrcChn, tSdp, this);
        this->SetErrorCode(nErrorCode);
        break;

    } while (false);

    return this->GetErrorCode();
}


//CMS_RESULT CInviteReceiverSwitchTask::StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, const TSdp& tSdp, TKDSIP_DIALOG_ID tDlgID)
//{
//	CMS_RESULT nErrorCode = CMS_SUCCESS;
//	TInviteSubject tSubject(tDstChn, tSrcChn);
//
//	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s], tSdp.Owner[%s], tSdp.OfflineSwitch[%s], tSdp.DirectTransfer[%s].\n", 
//		tDstChn.ToString().c_str(), 
//		tSrcChn.ToString().c_str(), 
//		tSdp.GetOwner().ToString().c_str(),
//		BoolStr(tSdp.GetOfflineSwitch()),
//		BoolStr(tSdp.GetDirectTransfer()));
//
//
//	////////////////////////////////////////////////////////////////////////
//	//查找目的交换任务。存在，则失败；否则，创建
//	CSwitchTask* pcDstSwitchTask = g_cSwitchList.FindSwitchNode(tDstChn, CHANNEL_TYPE_RECVONLY, tSdp.GetSessionName());
//	if(NULL != pcDstSwitchTask )
//	{
//		CSwitchTask* pcTopSrcSwitchTask = pcDstSwitchTask->GetTopSrc();
//		TChannel tSrcChn;
//		if(pcTopSrcSwitchTask)
//		{
//			tSrcChn = pcTopSrcSwitchTask->GetSender().GetChannel();
//		}
//
//		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]！\n", tSrcChn.ToString().c_str());
//
//		nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
//		return nErrorCode;
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//开启实时码流业务（浏览、音频呼叫）时，需要查找源交换任务。存在，则直接分发；否则，创建
//	CSwitchTask* pcSrcSwitchTask = NULL;
//    if (!tSdp.GetDirectTransfer() 
//        && (tSdp.GetSessionName() == SDP_SESSION_PLAY || tSdp.GetSessionName() == SDP_SESSION_AUDIOCALL)
//        )
//	{
//		pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
//	}
//
//    /////////////////////////////////////////////////////////////////////////////////
//    //检查需要采用的交换模式
//    TSwitchMode tSwitchMode = CheckSwitchMode(tSrcChn, tSdp);
//    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "CheckSwitchMode:[%s]！\n", tSwitchMode.c_str());
//	/////////////////////////////////////////////////////////////////////////////////
//	//判断InviteReq来源
//	if(tSdp.GetOwner() == tDstChn)
//	{
//		//InviteReq来自Receiver。
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from DstChn！\n");
//
//		pcDstSwitchTask = this;
//		nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn, NULL, tDlgID);
//		this->SetErrorCode(nErrorCode);
//		if(CMS_SUCCESS != this->GetErrorCode())
//		{
//			return this->GetErrorCode();
//		}
//
//		pcDstSwitchTask->SetSubject(tSubject);
//		pcDstSwitchTask->SetSessionName(tSdp.GetSessionName());
//        pcDstSwitchTask->SetSwitchMode(tSwitchMode);
//        pcDstSwitchTask->SetSdp(tSdp);
//
//		//直传
//        if ( SWITCH_MODE_DRT == tSwitchMode )
//		{
//			nErrorCode = pcDstSwitchTask ->InitSender(tSrcChn);
//			this->SetErrorCode(nErrorCode);
//			if(CMS_SUCCESS != this->GetErrorCode())
//			{
//				return this->GetErrorCode();
//			}
//
//            pcDstSwitchTask->GetSender().GetDevSsnTask()->AddCurDirectTransNum();
//
//			nErrorCode = pcDstSwitchTask->Start();
//			this->SetErrorCode(nErrorCode);
//
//			return this->GetErrorCode();
//		}
//	}
//	else if(tSdp.GetOwner() == tSrcChn)
//	{
//		//InviteReq来自Sender。
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from SrcChn！\n");
//
//        //不允许非会话内的多次同源InviteReq
//        if(SWITCH_MODE_SNG == tSwitchMode && NULL != pcSrcSwitchTask && pcSrcSwitchTask->GetSender().GetDlgID() != tDlgID)
//        {
//            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ReInvite from SrcChn but not in Dialog！\n");
//            nErrorCode = ERR_CMU_SWITCH_SRC_EXIST_SWITCH;
//            return nErrorCode;
//        }
//
//		pcSrcSwitchTask = this;
//		nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn, NULL, tDlgID);
//		this->SetErrorCode(nErrorCode);
//		if(CMS_SUCCESS != this->GetErrorCode())
//		{
//			return this->GetErrorCode();
//		}
//
//        pcSrcSwitchTask->SetSubject(tSubject);
//        pcSrcSwitchTask->SetSessionName(tSdp.GetSessionName());
//        pcSrcSwitchTask->SetSwitchMode(tSwitchMode);
//        pcSrcSwitchTask->SetSdp(tSdp);
//
//		//直传
//		if ( SWITCH_MODE_DRT == tSwitchMode)
//		{
//			pcDstSwitchTask->GetSender().GetDevSsnTask()->AddCurDirectTransNum();
//
//			nErrorCode = pcSrcSwitchTask ->InitReceiver(tDstChn);
//			this->SetErrorCode(nErrorCode);
//			if(CMS_SUCCESS != this->GetErrorCode())
//			{
//				return this->GetErrorCode();
//			}
//
//			nErrorCode = pcSrcSwitchTask->Start();
//			this->SetErrorCode(nErrorCode);
//
//			return this->GetErrorCode();
//		}
//	}
//	else
//	{
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from neither DstChn nor SrcChn！\n");
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//创建交换任务
//	if(NULL == pcDstSwitchTask)
//	{
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one！\n");
//
//		pcDstSwitchTask =  g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
//		nErrorCode = pcDstSwitchTask ->InitReceiver(tDstChn);
//		if(CMS_SUCCESS != nErrorCode)
//		{
//			SAFE_DEL_PTR(pcDstSwitchTask);
//			return nErrorCode;
//		}
//
//        pcDstSwitchTask->SetSubject(tSubject);
//        pcDstSwitchTask->SetSessionName(tSdp.GetSessionName());
//        pcDstSwitchTask->SetSwitchMode(tSwitchMode);
//		pcDstSwitchTask->SetSdp(tSdp);
//	}
//
//	if(NULL == pcSrcSwitchTask)
//	{
//		//创建源交换结点
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one！\n");
//
//		pcSrcSwitchTask = g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
//		nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn);
//		if(CMS_SUCCESS != nErrorCode)
//		{
//			SAFE_DEL_PTR(pcDstSwitchTask);    //失败后外层会删除本Task
//            if (ERR_CMU_CMU_NOT_CONN != nErrorCode)
//            {
//                SAFE_DEL_PTR(pcSrcSwitchTask);
//            }
//			return nErrorCode;
//		}
//
//        pcSrcSwitchTask->SetSubject(tSubject);
//        pcSrcSwitchTask->SetSessionName(tSdp.GetSessionName());
//        pcSrcSwitchTask->SetSwitchMode(tSwitchMode);
//        pcSrcSwitchTask->SetSdp(tSdp);
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//从源交换中选择分发节点 或 调度vtdu
//	CSwitchTask* pcRtnDistribNode = NULL;
//	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);
//
//	if(NULL != pcRtnDistribNode)
//	{
//        //直接给pcSrcSwitchTask赋值，之前分配给他的SwitchTask如何处理??? fanxg
//		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
//	}
//	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //先把交换结点关联，否则失败时ptSrcSwitchNode不能被正确删除
//
//	if(CMS_SUCCESS != nErrorCode)
//	{
//		//SAFE_DEL_PTR(pcDstSwitchTask);    //失败后外层会删除本Task
//		return nErrorCode;
//	}
//
//	nErrorCode = MakeLink(pcSrcSwitchTask, pcDstSwitchTask);
//    return nErrorCode;
//}

CMS_RESULT CInviteReceiverSwitchTask::ProcInviteRsp(const COspSipMsg*const pcOspSipMsg)
{
    CInviteRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);
    TSdp& tRspSdp = cRsp.GetSdp();

	m_pcCurInviting->SetDlgID(pcOspSipMsg->GetSipDlgID());

	const TMediaDescList& tMediaDescList = tRspSdp.GetMediaDescList();

	for (TMediaDescList::const_iterator it = tMediaDescList.begin(); tMediaDescList.end() != it; ++it)
	{
		TASKLOG(CRITICAL_LEV, "流属性[%s]\n", it->ToXml().c_str());
		if (MEDIA_TYPE_VIDEO == it->GetMediaType())
		{
			string strTmp;
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_PARAM_RESOLUTION, strTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：分辨率[%s]\n", strTmp.c_str());
			}
			s32 nTmp = 0;
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_FRAME_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：帧率[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_BIT_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：码率[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：关键帧间隔[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_ENCODE_FORMAT, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：编码格式[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_RATE_TYPE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：码率类型[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_CLOCK_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：视频采样率[%d]\n", nTmp);
			}
		}
		else if (MEDIA_TYPE_AUDIO == it->GetMediaType())
		{
			s32 nTmp = 0;
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_SAMPLE_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：采样率[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_CHANNEL_NUM, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：声道数[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_ENCODE_FORMAT, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：编码格式[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_BIT_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：码率[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_CLOCK_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "流属性：时间戳采样率[%d]\n", nTmp);
			}
		}
	}

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


    m_pcCurInviting->SetSdp(cRsp.GetSdp());

    if (INVALID_TRANS_ID != this->GetSipTransID())
    {
        //CSwitchPin& cSwitchPin = GetSwitchPin(m_pcCurInviting->GetDlgID(), false);

        //CInviteRsp cRsp;
        cRsp.SetSdp(m_pcCurInviting->GetSdp());
		if ((DEV_TYPE_VTDU == m_cSender.GetDevType() && DEV_TYPE_CMU == m_cReceiver.GetDevType())
			|| (DEV_TYPE_CMU == m_cSender.GetDevType() && DEV_TYPE_VTDU == m_cReceiver.GetDevType()))
		{
			cRsp.GetSdp().SetIsNatPacketMode(false);
		}

        if (NULL != m_pcInput)
        {
            const TSdp &tSrcSenderSdp = m_pcInput->GetSender().GetSdp();
            if (tSrcSenderSdp.GetExtInfo().end() != tSrcSenderSdp.GetExtInfo().find(SDP_EXT_INFO_FILE_SIZE))
            {
                cRsp.GetSdp().SetFileSize(tSrcSenderSdp.GetFileSize());
            }
        }

		ReSetSdpMediaInfo(cRsp.GetSdp());
        DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);

        NextState(Ack);
    }
    else
    {
        //创建备份源后，迁移到其他vtdu上分发时，走这个逻辑
		//备份源先拆一个交换，再建两个交换；这里是第二个交换，CU不感知；m_tReqTransId已经使用过
		//后续这部分逻辑需要删除，改为通知目的重新发起交换请求
        CInviteACK cInviteACK;
        cInviteACK.SetSession(m_pcCurInviting->GetDevSsnTask()->GetSession());

        if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteACK, m_pcCurInviting->GetDlgID()))
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发送KDSIP_EVENT_INVITE_ACK失败！\n");

            this->SetErrorCode(ERR_POST_MSG_FAILED);
            return this->GetErrorCode();
        }
        NextState(Service);
    }

    return this->GetErrorCode();
}


CMS_RESULT CInviteReceiverSwitchTask::ProcInviteAck(const COspSipMsg*const pcOspSipMsg)
{
	CSwitchPin& cSwitchPin = GetSwitchPin(pcOspSipMsg->GetSipDlgID());
	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "收到来自[%s]的InviteACK。\n", cSwitchPin.GetDevDesc().c_str());

    CInviteACK cInviteACK;
    pcOspSipMsg->GetMsgBody(cInviteACK);

    cInviteACK.SetSession(m_pcCurInviting->GetDevSsnTask()->GetSession());

    if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteACK, m_pcCurInviting->GetDlgID()))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发送KDSIP_EVENT_INVITE_ACK失败！\n");

        this->SetErrorCode(ERR_POST_MSG_FAILED) ;
        return this->GetErrorCode();
    }

	return this->GetErrorCode();
}

//void CInviteReceiverSwitchTask::ReleaseResource()
//{
//    CSwitchTask::ReleaseResource();
//
//	if(INVALID_TRANS_ID != m_tReqTransId)
//	{
//		//发送应答
//        //PostInviteRsp(, ERR_CMU_SWITCH_EXCEPTION);
//	}
//}

////////////////////////////////////////////////////////////////////////////////////////////////
//其他
////////////////////////////////////////////////////////////////////////////////////////////////
void CInviteReceiverSwitchTask::PrintData(ESwitchTrace eTrace)const
{
    CSwitchTask::PrintData(eTrace);
}

#define   STR_VID_RES_FLAG   "*"
void CInviteReceiverSwitchTask::ReSetSdpMediaInfo(TSdp& tSdp)
{
	CInviteReq* pcReq = (CInviteReq*)GetReq();
	TChannel& tPuChn = pcReq->GetSrcChn();

	if (SDP_SESSION_PLAY == pcReq->GetSdp().GetSessionName())//实时浏览视频属性需要替换成适时媒体流属性
	{
		TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
		for (TMediaDescList::iterator it = tMediaDescList.begin(); tMediaDescList.end() != it; ++it)
		{
			if (MEDIA_TYPE_VIDEO == it->GetMediaType())
			{
				TMediaFormat& tMediaFormat = it->GetMediaFormat();
                CPuData *ptPuData = CCmuData::GetInstance()->GetPuDataByUri(tPuChn.GetDevUri());
				if (ptPuData != NULL)
				{
					TVideoEncodeParam tVidPam = ptPuData->GetVideoEncodeParam(tPuChn.GetChnNO());
					string strTmp;
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_PARAM_RESOLUTION, strTmp))
					{
						string strWidth,strHeith;
						string::size_type nPos = tVidPam.GetVideoRes().find(STR_VID_RES_FLAG);
						if (string::npos != nPos)
						{
							strWidth = tVidPam.GetVideoRes().substr(0,nPos);
							strHeith = tVidPam.GetVideoRes().substr(nPos+1);
						}
						char buf[128]={0};
						sprintf( buf, "W:%s,H:%s",strWidth.c_str(),strHeith.c_str());

						TASKLOG(CRITICAL_LEV, "流属性：当前分辨率[%s]，适时分辨率[%s]\n", strTmp.c_str(),buf);
						tMediaFormat.SetFormatParam(VIDEO_ENC_PARAM_RESOLUTION,buf);
					}
					s32 nTmp = 0;
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_FRAME_RATE, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "流属性：当前帧率[%d]，适时帧率[%d]\n", nTmp,tVidPam.GetFrameRate());
						tMediaFormat.SetFormatParam(VIDEO_ENC_FRAME_RATE,tVidPam.GetFrameRate());
					}
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_BIT_RATE, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "流属性：当前码率[%d]，适时码率[%d]\n", nTmp,tVidPam.GetBitRate());
						tMediaFormat.SetFormatParam(VIDEO_ENC_BIT_RATE,tVidPam.GetBitRate());
					}
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "流属性：当前关键帧间隔[%d]，实时关键帧间隔[%d]\n", nTmp,tVidPam.GetInterval());
						tMediaFormat.SetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL,tVidPam.GetInterval());
					}
					return;
				}
 			}
		}
	}
}
