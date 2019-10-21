/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : clienttask.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: client taskʵ��
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#include "clienttask.h"
#include "clientinst.h"


CClientTask::CClientTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSubscribeID = INVALID_DIALOG_ID;
	m_tInviteID    = INVALID_DIALOG_ID;
}

CClientTask::~CClientTask()
{
}

void CClientTask::TerminateTask()
{
    if ( m_tSubscribeID != INVALID_DIALOG_ID )
    {
		OspPrintf(TRUE,FALSE,"\n����OspSipReleaseDlg������\n");
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;
    }

	if ( m_tInviteID != INVALID_DIALOG_ID )
	{
		OspSipReleaseDlg(m_tInviteID);
		m_tInviteID = INVALID_DIALOG_ID;
		OspPrintf(TRUE,FALSE,"\n����OspSipReleaseDlg������\n");
	}
	delete this;
}

const char* CClientTask::GetStateName(u32 dwState)
{
    switch (dwState)
    {
    case IDLE_STATE:
        return "Idle State";
    case WAIT_REG_RSP_STATE:
        return "Wait Reg Rsp State";
    case SERVICE_STATE:
        return "Service State";
    default:
        break;
    }
    return "Unknown State";
}
void CClientTask::InitStateMachine()
{
    CStateProc cRegProc;
    cRegProc.ProcMsg = (CTask::PFProcMsg)&CClientTask::WaitRegRspProcMsg;
    cRegProc.TimerPoll = (CTask::PFTimerPoll)&CClientTask::WaitRegRspTimerPoll;
    AddRuleProc(WAIT_REG_RSP_STATE, cRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CClientTask::ServiceProcMsg;
    cServiceProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(SERVICE_STATE, cServiceProc);
}

u32 CClientTask::StartReg()
{
    CRegReq cRegReq;
    cRegReq.SetUserName(m_strUserName);
    cRegReq.SetPassWord(m_strUserName);
    cRegReq.SetClientURI(OspSipGetLocalURI().GetURIString());

    if (PROCMSG_OK != PostMsgReq(cRegReq, m_tServerUri))

    {
        OspPrintf(TRUE, FALSE, "����SIP_EV_MESSAGE_REQʧ��\n");
        return PROCMSG_FAIL;
    }

    OspPrintf(TRUE, FALSE, "����EV_CLIENT_SERVER_REG_REQ�ɹ�\n");
    NextState(WAIT_REG_RSP_STATE);
    return PROCMSG_OK;
}

u32 CClientTask::WaitRegRspTimerPoll()
{
    if ( GetPollCounter() > 5 )
    {
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CClientTask::WaitRegRspProcMsg(CMessage *const pcMsg)
{
    switch (pcMsg->event)
    {
    case EV_SERVER_CLIENT_REG_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CRegRsp cRegRsp;
            cRegRsp.FromXml(pcOspSipMsg->GetMsgBody());

            if ( cRegRsp.GetErrorCode() == EV_SUCCESS )
            {
                OspPrintf(TRUE, FALSE, "ע��ɹ�\n");

                m_strSession = cRegRsp.GetSession();
                NextState(SERVICE_STATE);

                //��������
                TSipURI tCltURI;
                tCltURI.SetURIString(SERVER_URI);
                SetHBParam(tCltURI);

                return PROCMSG_OK;
            }
            else
            {
                OspPrintf(TRUE, FALSE, "ע��ʧ��\n");
                //ע��ʧ��
                TerminateTask();
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            OspPrintf(TRUE, FALSE, "SIPЭ��ջ����, errorcode[%d], ����ע������ʧ��\n", 
                pcOspSipMsg->GetSipErrorCode());
        }
        break;
    default:
        {
            OspPrintf(TRUE, FALSE, "[IDLE]unknow msg %s--%d\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_DEL;
}

u32 CClientTask::StartUnReg()
{
    if ( GetState() != SERVICE_STATE )
    {
        OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷�ע���ͻ�\n");
        return PROCMSG_OK;
    }

    CUnRegReq cUnRegReq;
    cUnRegReq.SetSession(m_strSession);

    if ( PROCMSG_OK != PostMsgReq(cUnRegReq, m_tServerUri) )
    {
        OspPrintf(TRUE, FALSE, "����SIP_EV_MESSAGE_REQʧ��\n");
        return PROCMSG_FAIL;
    }

    OspPrintf(TRUE, FALSE, "����EV_CLIENT_SERVER_UNREG_REQ�ɹ�\n");
    return PROCMSG_OK;
}

u32 CClientTask::SendMsg(const string strMsg)
{
    if ( GetState() != SERVICE_STATE )
    {
        OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷�������Ϣ\n");
        return PROCMSG_OK;
    }

    CSendMsgReq cSendMsgReq;
    cSendMsgReq.SetMsg(strMsg);
    cSendMsgReq.SetSession(m_strSession);

    if ( PROCMSG_OK != PostMsgReq(cSendMsgReq, m_tServerUri) )
    {
        OspPrintf(TRUE, FALSE, "����SIP_EV_MESSAGE_REQʧ��\n");
        return PROCMSG_FAIL;
    }

    OspPrintf(TRUE, FALSE, "����EV_CLIENT_SERVER_UNREG_REQ�ɹ�\n");
    return PROCMSG_OK;
}

u32 CClientTask::StartSubscribe()
{
    if ( GetState() != SERVICE_STATE )
    {
        OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷�������Ϣ\n");
        return PROCMSG_OK;
    }

    if ( m_tSubscribeID != INVALID_DIALOG_ID )
    {
        OspPrintf(TRUE, FALSE, "�ͻ��Ѿ���������Ϣ\n");
        return PROCMSG_OK;
    }

    CSubscribeReq cSubscribeReq;
    cSubscribeReq.SetSession(m_strSession);

    PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, cSubscribeReq, m_tServerUri);
    return PROCMSG_OK;
}

u32 CClientTask::StopSubscribe()
{
    if ( GetState() != SERVICE_STATE )
    {
        OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷�ȡ������\n");
        return PROCMSG_OK;
    }

    if ( m_tSubscribeID == INVALID_DIALOG_ID )
    {
        OspPrintf(TRUE, FALSE, "�ͻ���δ������Ϣ\n");
        return PROCMSG_OK;
    }

    CUnSubscribeReq cUnSubscribeReq;
    cUnSubscribeReq.SetSession(m_strSession);

    PostInDlgReq(KDSIP_EVENT_UNSUBSCRIBE_REQ, cUnSubscribeReq, m_tSubscribeID);
    return PROCMSG_OK;
}


/*//////////////////////////////////////////////////////////////////////////////
    ��Ƶ���ʵ��
//////////////////////////////////////////////////////////////////////////////*/
u32 CClientTask::StartInvite()
{
	if ( GetState() != SERVICE_STATE )
	{
		OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷������Ƶ��\n");
		return PROCMSG_FAIL;
	}

	if ( m_tInviteID != INVALID_DIALOG_ID )
	{
		OspPrintf(TRUE, FALSE, "�ͻ��Ѿ��������\n");
		return PROCMSG_OK;
	}

	CEventReq cInviteReq;
	cInviteReq.SetSession(m_strSession);
	cInviteReq.SetEvent("EV_C_S_INVITE_REQ");

	if(!PostReq(KDSIP_EVENT_INVITE_REQ, cInviteReq, m_tServerUri))
	{
		return PROCMSG_FAIL;
	}
	return PROCMSG_OK;
}


u32 CClientTask::StopInvite()
{
	if ( GetState() != SERVICE_STATE )
	{
		OspPrintf(TRUE, FALSE, "�ͻ���ǰ���Ƿ���̬, �޷�ֹͣ�����\n");
		return PROCMSG_FAIL;
	}

	if ( m_tInviteID == INVALID_DIALOG_ID )
	{
		OspPrintf(TRUE, FALSE, "�ͻ���δ������Ƶ�����\n");
		return PROCMSG_OK;
	}

	CEventReq cUnInviteReq;
	cUnInviteReq.SetSession(m_strSession);
	cUnInviteReq.SetEvent("EV_C_S_INVITE_BYE_REQ");

	if(PROCMSG_OK!=PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cUnInviteReq, m_tInviteID))
	{
		OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INVITE_BYE_REQʧ�ܣ�");
		return PROCMSG_FAIL;
	}
	OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INVITE_BYE_REQ�ɹ���");

	//////////////////////////////////////////////////////////////////////
	m_tInviteID = INVALID_DIALOG_ID;
	//////////////////////////////////////////////////////////////////////
	return PROCMSG_OK;
}

u32 CClientTask::SendInfoMsg()
{
    if(m_tInviteID == INVALID_DIALOG_ID)
    {
        OspPrintf(TRUE,FALSE,"�û���δ���������");
        return PROCMSG_FAIL;
    }

    CSendMsgReq myMessage;
    myMessage.SetEvent("EV_C_S_INFO_REQ");
    std::string bigstr;
    std::string str1 = "abcdefghijklmnopqrstuvwxyz\n";
    for(int ii=0;ii<1;ii++)
    {
        bigstr+=str1;
    }
    myMessage.SetMsg(bigstr);
    myMessage.SetSession(m_strSession);

    if(PROCMSG_OK != PostInDlgReq(KDSIP_EVENT_INFO_REQ,myMessage,m_tInviteID))
    {
        OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INFO_REQʧ�ܣ�");
        return PROCMSG_FAIL;
    }
    return PROCMSG_OK;
}

u32 CClientTask::SendDlgMsg()
{
    if(m_tInviteID == INVALID_DIALOG_ID)
    {
        OspPrintf(TRUE,FALSE,"�û���δ���������");
        return PROCMSG_FAIL;
    }

    CSendMsgReq myMessage;
    myMessage.SetEvent("EV_C_S_MESSAGE_DIALOG_REQ");
    myMessage.SetMsg("This is an dialog message test!");
    myMessage.SetSession(m_strSession);

    if(PROCMSG_OK != PostInDlgReq(KDSIP_EVENT_MESSAGE_DIALOG_REQ,myMessage,m_tInviteID))
    {
        OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_MESSAGE_DIALOG_REQʧ�ܣ�");
        return PROCMSG_FAIL;
    }
    return PROCMSG_OK;
}
//////////////////////////////////////////////////////////////////////////////


u32 CClientTask::ServiceProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    switch (pcMsg->event)
    {
    case EV_SERVER_CLIENT_SEND_MSG_RSP:
        {
            OspPrintf(TRUE,FALSE,"������Ѿ��յ���Ϣ��");
        }
        break;
    case EV_SERVER_CLIENT_UNREG_RSP:
        {
            CUnRegRsp cUnRegRsp;
            pcOspSipMsg->GetMsgBody(cUnRegRsp);

            //���۽����Σ�����������
            TerminateTask();
            return PROCMSG_DEL;
        }
        break;
    case EV_S_C_SUBSCRIBE_MSG_RSP:
        {
			CSubscribeRsp cSubRsp;
			pcOspSipMsg->GetMsgBody(cSubRsp);
			if(EV_SUCCESS == cSubRsp.GetErrorCode())
			{
				m_tSubscribeID = pcOspSipMsg->GetSipDlgID();
				OspPrintf(TRUE,FALSE,"���ĳɹ���"); 
				return PROCMSG_OK;
			}
			else
			{
				//ҵ��㶩��ʧ�ܣ������˶�
                CUnSubscribeReq cUnSubscribeReq;
                cUnSubscribeReq.SetSession(m_strSession);

                PostInDlgReq(KDSIP_EVENT_UNSUBSCRIBE_REQ, cUnSubscribeReq, m_tSubscribeID);

				OspPrintf(TRUE,FALSE,"����ʧ�ܣ�");
				return PROCMSG_FAIL;
			}
        }
        break;
    case EV_S_C_UNSUBSCRIBE_MSG_RSP:
        {
            CUnSubscribeRsp cUnSubscribeRsp;
            pcOspSipMsg->GetMsgBody(cUnSubscribeRsp);
            if ( EV_SUCCESS == cUnSubscribeRsp.GetErrorCode() )
            {
				OspPrintf(TRUE,FALSE,"ȡ�����ĳɹ���");
                m_tSubscribeID = INVALID_DIALOG_ID;
            }
        }
        break;
    case EV_S_C_BROADCAST_MSG_NTF_REQ:
        {
            CBroadcastMsgNtfReq cBroadcastMsgNtfReq;
            pcOspSipMsg->GetMsgBody(cBroadcastMsgNtfReq);

            OspPrintf(TRUE, FALSE, "�ͻ�%s�յ��������Ĺ㲥��Ϣ: %s\n", 
                    m_strUserName.c_str(), cBroadcastMsgNtfReq.GetMsg().c_str());

            CEventRsp cNtfRsp;
            cNtfRsp.SetEvent(EV_S_C_BROADCAST_MSG_NTF_RSP);
            cNtfRsp.SetSession(m_strSession);
            cNtfRsp.SetSeqNum(cBroadcastMsgNtfReq.GetSeqNum());
            cNtfRsp.SetErrorCode(EV_SUCCESS);

			if(PROCMSG_OK!=PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp))
			{
				OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_NOTIFY_RSPʧ�ܣ�");
			}
        }
        break;
	case EV_S_C_INVITE_RSP:
		{
			CEventRsp cInviteRsp;
			pcOspSipMsg->GetMsgBody(cInviteRsp);
			if(cInviteRsp.GetErrorCode()!=EV_SUCCESS)
			{
//				OspSipReleaseInvite(pcOspSipMsg->GetSipDlgID());
				OspPrintf(TRUE,FALSE,"�������ʧ�ܣ�");
				return PROCMSG_FAIL;
			}

			m_tInviteID = pcOspSipMsg->GetSipDlgID();

			CEventReq cInviteAck;
			cInviteAck.SetEvent("EV_C_S_INVITE_ACK");
			cInviteAck.SetSession(m_strSession);
			cInviteAck.SetSeqNum(cInviteRsp.GetSeqNum());

			if(PROCMSG_OK!=PostInDlgReq(KDSIP_EVENT_INVITE_ACK,cInviteAck,m_tInviteID))
			{
				OspPrintf(TRUE,FALSE,"\n\n����KDSIP_EVENT_INVITE_ACKʧ�ܣ�\n\n");
				return PROCMSG_FAIL;
			}
			OspPrintf(TRUE,FALSE,"\n��Ƶ����ɹ���\n");
			return PROCMSG_OK;
		}
		break;
	case EV_S_C_INFO_RSP:
		{
			OspPrintf(TRUE,FALSE,"������յ�INFO��Ϣ��");
		}
		break;
	case EV_S_C_MESSAGE_DIALOG_RSP:
		{
			OspPrintf(TRUE,FALSE,"������յ�MESSAGE_DIALOG��Ϣ��");
		}
		break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            OspPrintf(TRUE, FALSE, "SIPЭ��ջ����, errorcode[%d], ����ע������ʧ��\n", 
                pcOspSipMsg->GetSipErrorCode());
        }
        break;
    default:
        {
            OspPrintf(TRUE, FALSE, "[SERVICE]unknow msg %s--%d\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

