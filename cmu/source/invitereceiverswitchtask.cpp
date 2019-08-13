/*****************************************************************************
ģ����      : cmu 
�ļ���      : invitereceiverswitchtask.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: CInviteReceiverSwitchTaskʵ��
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2011/11/04     1.0        ������        ����
2012/03/30     1.0        ������       �ع������߼�����֮ǰ��һ����������ֳ�Src��Dst������������
******************************************************************************/

#include "invitereceiverswitchtask.h"
#include "vtduregtask.h"

CInviteReceiverSwitchTask::CInviteReceiverSwitchTask(CInstExt *pcInst) : CSwitchTask(pcInst)
{
    
}

CInviteReceiverSwitchTask::~CInviteReceiverSwitchTask()
{
    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "����CInviteReceiverSwitchTask, this[0x%X],taskNo[%d] \n", this, this->GetTaskNO() );
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

//״̬����
u32 CInviteReceiverSwitchTask::OnState(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");
		//TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip ��ϢΪ��\n");
		//nErrorCode = ERR_CMU_SIP_BODY_NULL;
		return PROCMSG_FAIL;
	}

	//////////////////////////////////////////////////
	//{{ͨ����Ϣ����
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
	//}}ͨ����Ϣ����


	//////////////////////////////////////////////////
	//״̬��Ϣ����
	dwRet = PROCMSG_OK;

	switch(GetState())
	{
	case Init:
		{
			switch(pcMsg->event)
			{
			case INVITE_REQ:
				{
                    //��ȡ��Ϣ��
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
						//NextState(Ack);  //��ProcInviteRsp�����л�״̬
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

					//�Ȼظ�CUI�յ�
					CUpdateSwitchAddressRsp cRsp;
					if (PROCMSG_OK != PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp))
					{
						dwRet = PROCMSG_FAIL;
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���½�����ַ��Ӧ��CUI��Ϣʧ��\n");
					}

					if (m_pcInput != NULL)  //Ŀ�Ľ���
					{
						//��Ͷ�ݸ�VTDU
						if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cSender.GetDlgID()))
						{
							dwRet = PROCMSG_FAIL;
							TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���½�����ַ����ϢͶ�ݸ�VTDUʧ��\n");
						}
					}
					else
					{
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���½�����ַ�������ڵĽ�������\n");
					}
					

				}
				break;

			case UPDATE_SWITCH_ADDRESS_RSP:
				{
					CUpdateSwitchAddressRsp cRsp;
					pcOspSipMsg->GetMsgBody(cRsp);
					if (cRsp.GetErrorCode() == CMS_SUCCESS)
					{
						TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "���½�����ַ�ɹ�\n");
					}
					else
					{
						TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "���½�����ַʧ�ܣ�������[%d]\n", 
							cRsp.GetErrorCode());
					}
				}
				break;

            case END_OF_FILE_NTF_REQ:
                {
                    //SendSwitchTaskת������Ϣ
                    CEndOfFileNtfReq cReq;
                    pcOspSipMsg->GetMsgBody(cReq);
                    if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cReceiver.GetDlgID()))
                    {
                        dwRet = PROCMSG_FAIL;
                        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��ϢͶ��ʧ�� Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    }
                }
                break;

            case END_OF_FILE_NTF_RSP:
                {
                    CEndOfFileNtfRsp cRsp;
                    pcOspSipMsg->GetMsgBody(cRsp);
                    if (cRsp.GetErrorCode() == CMS_SUCCESS)
                    {
                        TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "�������֪ͨ�ɹ�\n");
                    }
                    else
                    {
                        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "�������֪ͨʧ�ܣ�������[%d]\n", 
                            cRsp.GetErrorCode());
                    }
                }
                break;

            case PLAYBACK_PROGRESS_NTF_REQ:
                {
                    //SendSwitchTaskת������Ϣ
                    CPlaybackProgressNtfReq cReq;
                    pcOspSipMsg->GetMsgBody(cReq);

                    if(PROCMSG_OK!= PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_cReceiver.GetDlgID()))
                    {
                        dwRet = PROCMSG_FAIL;
                        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��ϢͶ��ʧ�� Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                    }
                }
                break;

            case PLAYBACK_PROGRESS_NTF_RSP:
                {
                    CPlaybackProgressNtfRsp cRsp;
                    pcOspSipMsg->GetMsgBody(cRsp);
                    if (cRsp.GetErrorCode() == CMS_SUCCESS)
                    {
                        TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "�������֪ͨ�ɹ�\n");
                    }
                    else
                    {
                        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "�������֪ͨʧ�ܣ�������[%d]\n", 
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
                        //ֻҪ����ʧ�ܾ�ɾ��
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
//                         TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��ϢͶ��ʧ�� Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "������ʱ��ɾ������\n");
		//PrintData();

        this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);

       return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//��ʼ����ҵ���߼�
////////////////////////////////////////////////////////////////////////////////////////////////
CMS_RESULT CInviteReceiverSwitchTask::Start()
{
    //Receiver.Sdpû��׼����������Invite Receiver
    if(!m_cReceiver.GetSdp().HasMediaDes())
    {
        //���棺CInviteReceiverSwitchTask��Ӧ���ߵ��������
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver.Sdp is not ready��firstly Invite Receiver[%s].\n", m_cReceiver.GetChannel().ToString().c_str());

        InviteTo(m_cReceiver, m_cReceiver.GetSdp());
        NextState(Starting);
    }
    else
    {
        //SWITCH_MODE_SNG���������������
        if(m_pcInput)
        {
            if(m_pcInput->GetSwitchState() == SWITCH_STATE_SWITCHED)
            {
                TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InputNode has switch complete��StartSwitchToSrc[%s].\n", m_pcInput->GetChannel().ToString().c_str());

                StartSwitchToSrc(m_pcInput);
                NextState(Starting);
            }
            else
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "InputNode in a middle state: [%s], IGNORE!\n", GetSwitchStateStr(m_pcInput->GetSwitchState()));
            }
        }
        //SWITCH_MODE_DRTֱ���������������
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
            //�ݴ������û����Sdp.Owner,����
            if(tSdp.GetOwner().IsNull())
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��Ԥ�ڵ�Sdp.Owner[%s]����ΪSession+Chn[%s_0]��\n", 
                    tSdp.GetOwner().ToString().c_str(), cInviteReq.GetSession().c_str());

                //�����ͨ��Ĭ��ʼ����0�����Բ��Ǻܾ�ȷ�����Կ��Ǻ���Owner��DstChn/SrcChn���ж�ֻ�ж�Uri.
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
//	//����Ŀ�Ľ������񡣴��ڣ���ʧ�ܣ����򣬴���
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
//		TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "The dst has exist a media swith to src[%s]��\n", tSrcChn.ToString().c_str());
//
//		nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
//		return nErrorCode;
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//����ʵʱ����ҵ���������Ƶ���У�ʱ����Ҫ����Դ�������񡣴��ڣ���ֱ�ӷַ������򣬴���
//	CSwitchTask* pcSrcSwitchTask = NULL;
//    if (!tSdp.GetDirectTransfer() 
//        && (tSdp.GetSessionName() == SDP_SESSION_PLAY || tSdp.GetSessionName() == SDP_SESSION_AUDIOCALL)
//        )
//	{
//		pcSrcSwitchTask = g_cSwitchList.FindSwitchNode(tSrcChn, CHANNEL_TYPE_SENDONLY, tSdp.GetSessionName());
//	}
//
//    /////////////////////////////////////////////////////////////////////////////////
//    //�����Ҫ���õĽ���ģʽ
//    TSwitchMode tSwitchMode = CheckSwitchMode(tSrcChn, tSdp);
//    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "CheckSwitchMode:[%s]��\n", tSwitchMode.c_str());
//	/////////////////////////////////////////////////////////////////////////////////
//	//�ж�InviteReq��Դ
//	if(tSdp.GetOwner() == tDstChn)
//	{
//		//InviteReq����Receiver��
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from DstChn��\n");
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
//		//ֱ��
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
//		//InviteReq����Sender��
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from SrcChn��\n");
//
//        //������ǻỰ�ڵĶ��ͬԴInviteReq
//        if(SWITCH_MODE_SNG == tSwitchMode && NULL != pcSrcSwitchTask && pcSrcSwitchTask->GetSender().GetDlgID() != tDlgID)
//        {
//            TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ReInvite from SrcChn but not in Dialog��\n");
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
//		//ֱ��
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
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "InviteReq come from neither DstChn nor SrcChn��\n");
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////
//	//������������
//	if(NULL == pcDstSwitchTask)
//	{
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "DstChn SwitchNode isn't exist, create one��\n");
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
//		//����Դ�������
//		TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn SwitchNode isn't exist, create one��\n");
//
//		pcSrcSwitchTask = g_cSwitchList.CreateInviteSenderSwitchTask(GetInstance());
//		nErrorCode = pcSrcSwitchTask ->InitSender(tSrcChn);
//		if(CMS_SUCCESS != nErrorCode)
//		{
//			SAFE_DEL_PTR(pcDstSwitchTask);    //ʧ�ܺ�����ɾ����Task
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
//	//��Դ������ѡ��ַ��ڵ� �� ����vtdu
//	CSwitchTask* pcRtnDistribNode = NULL;
//	nErrorCode = pcSrcSwitchTask->ChooseDistribNode(pcDstSwitchTask, pcRtnDistribNode);
//
//	if(NULL != pcRtnDistribNode)
//	{
//        //ֱ�Ӹ�pcSrcSwitchTask��ֵ��֮ǰ���������SwitchTask��δ���??? fanxg
//		pcSrcSwitchTask = (CSwitchTask*)pcRtnDistribNode;
//	}
//	pcSrcSwitchTask->AddOutput(pcDstSwitchTask);                   //�Ȱѽ���������������ʧ��ʱptSrcSwitchNode���ܱ���ȷɾ��
//
//	if(CMS_SUCCESS != nErrorCode)
//	{
//		//SAFE_DEL_PTR(pcDstSwitchTask);    //ʧ�ܺ�����ɾ����Task
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
		TASKLOG(CRITICAL_LEV, "������[%s]\n", it->ToXml().c_str());
		if (MEDIA_TYPE_VIDEO == it->GetMediaType())
		{
			string strTmp;
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_PARAM_RESOLUTION, strTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ��ֱ���[%s]\n", strTmp.c_str());
			}
			s32 nTmp = 0;
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_FRAME_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�֡��[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_BIT_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�����[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ��ؼ�֡���[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_ENCODE_FORMAT, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ������ʽ[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_RATE_TYPE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ���������[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(VIDEO_ENC_CLOCK_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ���Ƶ������[%d]\n", nTmp);
			}
		}
		else if (MEDIA_TYPE_AUDIO == it->GetMediaType())
		{
			s32 nTmp = 0;
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_SAMPLE_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�������[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_CHANNEL_NUM, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�������[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_ENCODE_FORMAT, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ������ʽ[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_BIT_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�����[%d]\n", nTmp);
			}
			if (it->GetMediaFormat().GetFormatParam(AUDIO_ENC_CLOCK_RATE, nTmp))
			{
				TASKLOG(CRITICAL_LEV, "�����ԣ�ʱ���������[%d]\n", nTmp);
			}
		}
	}

    CMS_RESULT nErrorCode = cRsp.GetErrorCode();

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�յ�����[%s] DlgId[%lu]��InviteRsp��ErrorCode[%ld]��\n",
        m_pcCurInviting->GetDevDesc().c_str(), pcOspSipMsg->GetSipDlgID(), nErrorCode);

    if(CMS_SUCCESS != nErrorCode)
    {
        this->SetErrorCode(nErrorCode);
        return this->GetErrorCode();
    }


    //{{����У��
    if (!tRspSdp.HasMediaDes())
    {
        CMS_ASSERT(tRspSdp.HasMediaDes());

        this->SetErrorCode(ERR_INVALIAD_SDP);
        return this->GetErrorCode();
    }

    if(tRspSdp.GetOwner() != m_pcCurInviting->GetChannel())
    {
        //[HZCTO:�Ƿ�Ҫ�Ƶ�CSwitchPin::SetSdp��]
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��Ԥ�ڵ�Sdp.Owner[%s]����ΪCurInviting(SwitchPin.Chn)[%s]��\n", 
            tRspSdp.GetOwner().ToString().c_str(),
            m_pcCurInviting->GetChannel().ToString().c_str());

        tRspSdp.SetOwner(m_pcCurInviting->GetChannel());
    }

    if(tRspSdp.GetSessionName() != this->GetSessionName())
    {
        //[HZCTO:�Ƿ�Ҫ�Ƶ�CSwitchPin::SetSdp��]
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��Ԥ�ڵ�Sdp.SessionName[%s]����Ϊthis->GetSessionName[%s]��\n", 
            tRspSdp.GetSessionName().c_str(),
            this->GetSessionName().c_str());

        tRspSdp.SetSessionName(this->GetSessionName());
    }
    //}}����У��


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
        //��������Դ��Ǩ�Ƶ�����vtdu�Ϸַ�ʱ��������߼�
		//����Դ�Ȳ�һ���������ٽ����������������ǵڶ���������CU����֪��m_tReqTransId�Ѿ�ʹ�ù�
		//�����ⲿ���߼���Ҫɾ������Ϊ֪ͨĿ�����·��𽻻�����
        CInviteACK cInviteACK;
        cInviteACK.SetSession(m_pcCurInviting->GetDevSsnTask()->GetSession());

        if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteACK, m_pcCurInviting->GetDlgID()))
        {
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����KDSIP_EVENT_INVITE_ACKʧ�ܣ�\n");

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
	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "�յ�����[%s]��InviteACK��\n", cSwitchPin.GetDevDesc().c_str());

    CInviteACK cInviteACK;
    pcOspSipMsg->GetMsgBody(cInviteACK);

    cInviteACK.SetSession(m_pcCurInviting->GetDevSsnTask()->GetSession());

    if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteACK, m_pcCurInviting->GetDlgID()))
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����KDSIP_EVENT_INVITE_ACKʧ�ܣ�\n");

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
//		//����Ӧ��
//        //PostInviteRsp(, ERR_CMU_SWITCH_EXCEPTION);
//	}
//}

////////////////////////////////////////////////////////////////////////////////////////////////
//����
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

	if (SDP_SESSION_PLAY == pcReq->GetSdp().GetSessionName())//ʵʱ�����Ƶ������Ҫ�滻����ʱý��������
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

						TASKLOG(CRITICAL_LEV, "�����ԣ���ǰ�ֱ���[%s]����ʱ�ֱ���[%s]\n", strTmp.c_str(),buf);
						tMediaFormat.SetFormatParam(VIDEO_ENC_PARAM_RESOLUTION,buf);
					}
					s32 nTmp = 0;
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_FRAME_RATE, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "�����ԣ���ǰ֡��[%d]����ʱ֡��[%d]\n", nTmp,tVidPam.GetFrameRate());
						tMediaFormat.SetFormatParam(VIDEO_ENC_FRAME_RATE,tVidPam.GetFrameRate());
					}
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_BIT_RATE, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "�����ԣ���ǰ����[%d]����ʱ����[%d]\n", nTmp,tVidPam.GetBitRate());
						tMediaFormat.SetFormatParam(VIDEO_ENC_BIT_RATE,tVidPam.GetBitRate());
					}
					if (tMediaFormat.GetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL, nTmp))
					{
						TASKLOG(CRITICAL_LEV, "�����ԣ���ǰ�ؼ�֡���[%d]��ʵʱ�ؼ�֡���[%d]\n", nTmp,tVidPam.GetInterval());
						tMediaFormat.SetFormatParam(VIDEO_ENC_KEY_FRAME_INTVL,tVidPam.GetInterval());
					}
					return;
				}
 			}
		}
	}
}
