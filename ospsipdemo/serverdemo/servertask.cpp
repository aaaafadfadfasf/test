/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : servertask.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ServerTask��ʵ��
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#include "servertask.h"
#include "cms/utility/uuid.h"
#include "serverinst.h"



#include <fstream>

CRegTask::CRegTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    //��ʼ����CRegState״̬
    SetCurState(REG_STATE);
}

CRegTask::~CRegTask()
{
    
}

const char* CRegTask::GetStateName(u32 dwState)
{
    switch (dwState)
    {
    case REG_STATE:
        {
            return "Reg State";
        }
        break;
    case UNREG_STATE:
        {
            return "UnReg State";
        }
        break;
    default:
        break;
    }
    return "Unknown State";
}

void CRegTask::InitStateMachine()
{
    CStateProc cRegProc;
    cRegProc.ProcMsg = (CTask::PFProcMsg)&CRegTask::RegStateProcMsg;
    cRegProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(REG_STATE, cRegProc);

    CStateProc cUnregState;
    cUnregState.ProcMsg = (CTask::PFProcMsg)&CRegTask::UnregStateProcMsg;
    cUnregState.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(UNREG_STATE, cUnregState);
}

void CRegTask::TerminateTask()
{
    //���ٸ��û�����������
    vector<CTask *>::iterator pItem = m_tTaskArray.begin();
    while (pItem != m_tTaskArray.end())
    {
        (*pItem) ->TerminateTask();
        ++pItem;
    }
    m_tTaskArray.clear();

	vector<CTask *>::iterator pItem1 = m_InviteArray.begin();
	while (pItem1 != m_InviteArray.end())
	{
		(*pItem1) ->TerminateTask();
		++pItem1;
	}
	m_InviteArray.clear();

    //��ȫ���û��б���ɾ�����û�
    g_cServerApp.GetAppData().DelUser(this);
}

void CRegTask::AddTask(CTask *pcTask)
{
    m_tTaskArray.push_back(pcTask);
}

void CRegTask::DelTask(CTask *pcTask)
{
    vector<CTask *>::iterator pItem = m_tTaskArray.begin();
    while (pItem != m_tTaskArray.end())
    {
        if ( (*pItem)->GetThisTask() == pcTask )
        {
            m_tTaskArray.erase(pItem);
            break;
        }
        ++pItem;
    }
}

void CRegTask::AddInviteTask(CTask *pcTask)
{
	m_InviteArray.push_back(pcTask);
}
void CRegTask::DelInviteTask(CTask *pcTask)
{
	vector<CTask *>::iterator pItem = m_InviteArray.begin();
	while (pItem != m_InviteArray.end())
	{
		if ( (*pItem)->GetThisTask() == pcTask )
		{
			m_InviteArray.erase(pItem);
			break;
		}
		++pItem;
	}
}

//ON REG_STATE
u32 CRegTask::RegStateProcMsg(CMessage *const pcMsg)
{
    switch (pcMsg->event)
    {
    case EV_CLIENT_SERVER_REG_REQ:
        {
            COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;

            CRegReq cRegReq;
            pcMonMsg->GetMsgBody(cRegReq);

            OspPrintf(TRUE, FALSE, "�յ��ͻ��� %s, ��ע����Ϣ\n", cRegReq.GetUserName().c_str());

            u32 dwErrorCode = EV_SUCCESS;
            CUserData *pcUserData = NULL;
            if (NULL != g_cServerApp.GetAppData().FindUserByUserName(cRegReq.GetUserName()))
            {
                dwErrorCode = EV_USER_ALREADY_EXIST;
                OspPrintf(TRUE, FALSE, "�Ѿ���ͬ���û� %s ������, ���󽫱��ܾ�\n", cRegReq.GetUserName().c_str());
            }
            else
            {
                pcUserData = this;
                pcUserData->SetUserName(cRegReq.GetUserName());
                pcUserData->SetPassWord(cRegReq.GetPassWord());
                CUUID cNewSessId;
                pcUserData->SetSessionID(cNewSessId);
                TSipURI tTmpURI;
                tTmpURI.SetURIString(cRegReq.GetClientURI());
                pcUserData->SetClientURI(tTmpURI);
                g_cServerApp.GetAppData().AddUser(pcUserData);

                //�л�����һ״̬
                GetStateName(GetState());
                NextState(UNREG_STATE);
            }

            CRegRsp cRegRsp;
            cRegRsp.SetErrorCode(dwErrorCode);
            cRegRsp.SetSeqNum(pcMonMsg->GetOspTransID().dwSeqNO);
            cRegRsp.SetSession(m_strSessId);
            PostMsgRsp(pcMonMsg->GetSipTransID(), cRegRsp);            

            if ( EV_SUCCESS == dwErrorCode )
            {
                //��������
                TSipURI tCltURI;
                tCltURI.SetURIString(cRegReq.GetClientURI());
                SetHBParam(tCltURI);
            }
            else
            {
                TerminateTask();
            }
            return PROCMSG_OK;
        }
        break;
    case OSP_SIP_DISCONNECT:
        {
            COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;
            OspPrintf(TRUE, FALSE, "�û� %s ����, ���ٴ�����\n",
                pcMonMsg->GetMsgBody());
            TerminateTask();
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

//ON UNREG_STATE
u32 CRegTask::UnregStateProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;
    switch (pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            OspPrintf(TRUE, FALSE, "�û� %s ����, ���ٴ�����\n",
                pcMonMsg->GetMsgBody());
            TerminateTask();
        }
    	break;
    case EV_CLIENT_SERVER_SEND_MSG_REQ:
        {
            CSendMsgReq cSendMsgReq;
            pcMonMsg->GetMsgBody(cSendMsgReq);

            OspPrintf(TRUE, FALSE, "�յ��ͻ� %s ���͵���Ϣ %s\n", 
                m_strUserName.c_str(), cSendMsgReq.GetMsg().c_str());

            CSendMsgRsp cSendMsgRsp;
            cSendMsgRsp.SetErrorCode(EV_SUCCESS);
            cSendMsgRsp.SetSeqNum(cSendMsgReq.GetSeqNum());
            cSendMsgRsp.SetSession(m_strSessId);
            PostMsgRsp(pcMonMsg->GetSipTransID(), cSendMsgRsp);
        }
        break;
    case EV_CLIENT_SERVER_UNREG_REQ:
        {
            CUnRegReq cUnRegReq;
            pcMonMsg->GetMsgBody(cUnRegReq);
            OspPrintf(TRUE, FALSE, "�յ��ͻ� %s ��ע������\n", 
                m_strUserName.c_str());

            CUnRegRsp cUnRegRsp;
            cUnRegRsp.SetSeqNum(cUnRegReq.GetSeqNum());
            cUnRegRsp.SetSession(m_strSessId);
            PostMsgRsp(pcMonMsg->GetSipTransID(), cUnRegRsp);
            TerminateTask();
        }
        break;
    case EV_C_S_SUBSCRIBE_MSG_REQ:
        {
            OspPrintf(TRUE, FALSE, "�յ��ͻ� %s ��������\n", 
                m_strUserName.c_str());

            //���ж��Ƿ��Ѿ�����
            if ( !m_tTaskArray.empty() )
            { //�Ѿ����Ĺ�
                OspPrintf(TRUE, FALSE, "�ͻ� %s �Ѿ����Ĺ������Ա��β���\n", 
                    m_strUserName.c_str());
                CTask *pcSubscribeTask = m_tTaskArray.at(0);
                pcSubscribeTask->ProcMsg(pcMsg);
            }
            else
            {
                //CClientObsvr *pcObsvr = new CClientObsvr(g_cServerApp.GetAppData().GetSubject())
                //����������
                CClientSubscribeTask *pcSubscribeTask = new CClientSubscribeTask(GetInstance());
                pcSubscribeTask->SetSessionID(m_strSessId);
                pcSubscribeTask->SetClientURI(m_tClientURI);

				pcSubscribeTask->SetSubscribeID(pcMonMsg->GetSipDlgID());

                //������������Ϣ
                u32 dwRet = pcSubscribeTask->ProcMsg(pcMsg);
                if ( PROCMSG_OK == dwRet )
                {
                    //����������뵽�����񼯺���
                    AddTask(pcSubscribeTask);
                }
            }
        }
        break;
    case EV_C_S_UNSUBSCRIBE_MSG_REQ:
        {
            OspPrintf(TRUE, FALSE, "�յ��ͻ� %s ȡ����������\n", 
                m_strUserName.c_str());

            //�ҵ�������
            if ( !m_tTaskArray.empty() )
            {
                vector<CTask *>::iterator pItem = m_tTaskArray.begin();
                while ( pItem != m_tTaskArray.end() )
                {
                    CClientSubscribeTask *pcTask = (CClientSubscribeTask*)(*pItem);
                    if ( pcTask->GetSubscribeID() == pcMonMsg->GetSipDlgID() )
                    {
                        u32 dwRet = pcTask->ProcMsg(pcMsg);
                        if ( dwRet == PROCMSG_OK )
                        {
                            m_tTaskArray.erase(pItem);
                        }
                        break;
                    }
                    ++pItem;
                }
            }
            else
            {
                CUnSubscribeReq cUnSubscribeReq;
                pcMonMsg->GetMsgBody(cUnSubscribeReq);

                CUnSubscribeRsp cUnSubscribeRsp;
                cUnSubscribeRsp.SetErrorCode(EV_UNSUBSCRIBE);
                cUnSubscribeRsp.SetSeqNum(cUnSubscribeReq.GetSeqNum());
                cUnSubscribeRsp.SetSession(m_strSessId);

                PostRsp(pcMonMsg->GetSipRsp(), pcMonMsg->GetSipTransID(), cUnSubscribeRsp);
            }
        }
        break;
    case EV_S_C_BROADCAST_MSG_NTF_RSP:
        {
            if ( !m_tTaskArray.empty() )
            {
                vector<CTask *>::iterator pItem = m_tTaskArray.begin();
                while ( pItem != m_tTaskArray.end() )
                {
                    CClientSubscribeTask *pcTask = (CClientSubscribeTask*)(*pItem);
                    if ( pcTask->GetSubscribeID() == pcMonMsg->GetSipDlgID() )
                    {
                        pcTask->ProcMsg(pcMsg);
                        break;
                    }
                    ++pItem;
                }
            }
        }
        break;
	case EV_C_S_INVITE_REQ:
		{
			OspPrintf(TRUE, FALSE, "�յ��ͻ� %s �������\n", 
				m_strUserName.c_str());

			if ( !m_InviteArray.empty() )
			{ //�Ѿ��������
				vector<CTask *>::iterator pItem = m_InviteArray.begin();
				while ( pItem != m_InviteArray.end() )
				{
					CClientInviteTask *pcTask = (CClientInviteTask*)(*pItem);
					if ( pcTask->GetDialogId() == pcMonMsg->GetSipDlgID() )
					{
						OspPrintf(TRUE,FALSE,"�������Ѿ����ڣ�");
						return PROCMSG_OK;
					}
					++pItem;
				}
			}

				CClientInviteTask *pcInviteTask = new CClientInviteTask(GetInstance());
				pcInviteTask->SetSessId(m_strSessId);
				pcInviteTask->SetClientURI(m_tClientURI);
				pcInviteTask->SetDialogId(pcMonMsg->GetSipDlgID());

				u32 dwRet = pcInviteTask->ProcMsg(pcMsg);
				if ( PROCMSG_OK == dwRet )
				{
					AddInviteTask(pcInviteTask);
				}
		}
		break;
	case EV_C_S_INVITE_ACK:
	case EV_C_S_INFO_REQ:
	case EV_C_S_MESSAGE_DIALOG_REQ:
		{
			if(m_InviteArray.empty())
			{
				OspPrintf(TRUE,FALSE,"û���ҵ�������");
				return PROCMSG_FAIL;
			}

			vector<CTask *>::iterator pItem = m_InviteArray.begin();
			while ( pItem != m_InviteArray.end() )
			{
				CClientInviteTask *pcTask = (CClientInviteTask*)(*pItem);
				if ( pcTask->GetDialogId() == pcMonMsg->GetSipDlgID() )
				{
					pcTask->ProcMsg(pcMsg);
					return PROCMSG_OK;
				}
				++pItem;
			}
			OspPrintf(TRUE,FALSE,"û���ҵ�������");
			return PROCMSG_FAIL;
		}
		break;
	case EV_C_S_INVITE_BYE_REQ:
		{
			if(m_InviteArray.empty())
			{
				OspPrintf(TRUE,FALSE,"û���ҵ�������");
				return PROCMSG_FAIL;
			}

			vector<CTask *>::iterator pItem = m_InviteArray.begin();
			while ( pItem != m_InviteArray.end() )
			{
				CClientInviteTask *pcTask = (CClientInviteTask*)(*pItem);
				if ( pcTask->GetDialogId() == pcMonMsg->GetSipDlgID() )
				{
					if(PROCMSG_OK == pcTask->ProcMsg(pcMsg))
					{
						DelInviteTask(pcTask);
						return PROCMSG_OK;
					}
					return PROCMSG_FAIL;
				}
				++pItem;
			}
			OspPrintf(TRUE,FALSE,"û���ҵ�������");
			return PROCMSG_FAIL;
		}
		break;
    default:
        {
            OspPrintf(TRUE,FALSE,"\n\n�յ���Ϣ\n\n");
        }
        break;
    }
    return PROCMSG_OK;
}

CClientObsvr::CClientObsvr(CClientSubscribeTask *pcTask, CSubject *pSub) : CObserver(pSub)
{
    m_pcTask = pcTask;
}

void CClientObsvr::Update(CObsMessage &cMsg)
{
    m_pcTask->BroadcastMsg(cMsg);
}


CClientSubscribeTask::CClientSubscribeTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_pcObsvr = NULL;
    m_tSubscribeID = INVALID_DIALOG_ID;
}

CClientSubscribeTask::~CClientSubscribeTask()
{
    if ( NULL != m_pcObsvr )
    {
        delete m_pcObsvr;
        m_pcObsvr = NULL;
    }

    if ( INVALID_DIALOG_ID != m_tSubscribeID )
    {
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;
    }
}

void CClientSubscribeTask::InitStateMachine()
{
    CStateProc cUnsubProc;
    cUnsubProc.ProcMsg = (CTask::PFProcMsg)&CClientSubscribeTask::UnSsStateProcMsg;
    cUnsubProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(UNSUBSCRIBE_STATE, cUnsubProc);

    CStateProc cSubProc;
    cSubProc.ProcMsg = (CTask::PFProcMsg)&CClientSubscribeTask::SubscribeStateProcMsg;
    cSubProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(SUBSCRIBLE_STATE, cSubProc);

    SetCurState(UNSUBSCRIBE_STATE);
}

void CClientSubscribeTask::TerminateTask()
{
    if ( m_pcObsvr )
    {
        m_pcObsvr->UnsubscribeReq();
        m_pcObsvr = NULL;
    }

    CTask::TerminateTask();
}

void CClientSubscribeTask::BroadcastMsg(const CObsMessage &cMsg)
{
    CBroadcastMsgNtfReq cBroadcastMsgNtfReq;
    cBroadcastMsgNtfReq.SetMsg(cMsg);
    cBroadcastMsgNtfReq.SetSession(GetSessionID());

    PostInDlgReq(KDSIP_EVENT_NOTIFY_REQ, cBroadcastMsgNtfReq, m_tSubscribeID);
}

u32 CClientSubscribeTask::UnSsStateProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    switch (pcMsg->event)
    {
    case EV_C_S_SUBSCRIBE_MSG_REQ:
        {
            CSubscribeReq cSubscribeReq;
            pcOspSipMsg->GetMsgBody(cSubscribeReq);

            //���Զ���
            if ( NULL != m_pcObsvr )
            {
                m_pcObsvr->UnsubscribeReq();
                m_pcObsvr = NULL;
            }
            m_pcObsvr = new CClientObsvr(this, g_cServerApp.GetAppData().GetSubject());
            m_pcObsvr->Subscribe();

            CSubscribeRsp cSubscribeRsp;
            cSubscribeRsp.SetSeqNum(cSubscribeReq.GetSeqNum());
            cSubscribeRsp.SetSession(m_strSessId);

            PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cSubscribeRsp);

            NextState(SUBSCRIBLE_STATE);
        }
    	break;
    case EV_C_S_UNSUBSCRIBE_MSG_REQ:
        {
            CUnSubscribeReq cUnSubscribeReq;
            pcOspSipMsg->GetMsgBody(cUnSubscribeReq);

            CUnSubscribeRsp cUnSubscribeRsp;
            cUnSubscribeRsp.SetErrorCode(EV_UNSUBSCRIBE);
            cUnSubscribeRsp.SetSeqNum(cUnSubscribeReq.GetSeqNum());
            cUnSubscribeRsp.SetSession(m_strSessId);

            PostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cUnSubscribeRsp);
        }
        break;
    default:
        break;
    }
    return PROCMSG_OK;
}

u32 CClientSubscribeTask::SubscribeStateProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    switch (pcMsg->event)
    {
    case EV_C_S_SUBSCRIBE_MSG_REQ:
        {
            CSubscribeReq cSubscribeReq;
            pcOspSipMsg->GetMsgBody(cSubscribeReq);

            CSubscribeRsp cSubscribeRsp;
            cSubscribeRsp.SetSeqNum(cSubscribeReq.GetSeqNum());
            cSubscribeRsp.SetSession(m_strSessId);
            PostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cSubscribeRsp);
        }
        break;
    case EV_C_S_UNSUBSCRIBE_MSG_REQ:
        {
            CUnSubscribeReq cUnSubscribeReq;
            pcOspSipMsg->GetMsgBody(cUnSubscribeReq);

			m_tSubscribeID = INVALID_DIALOG_ID;

            //����ȡ������
            if ( NULL != m_pcObsvr )
            {
                m_pcObsvr->UnsubscribeReq();
                m_pcObsvr = NULL;
            }

            CUnSubscribeRsp cUnSubscribeRsp;
            cUnSubscribeRsp.SetSeqNum(cUnSubscribeReq.GetSeqNum());
            cUnSubscribeRsp.SetSession(m_strSessId);
            PostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cUnSubscribeRsp);
            NextState(UNSUBSCRIBE_STATE);

            TerminateTask();
        }
    	break;
	case EV_S_C_BROADCAST_MSG_NTF_RSP:
		{
			OspPrintf(TRUE,FALSE,"�ͻ����յ��㲥��Ϣ��");
		}
		break;
    default:
        break;
    }
    return PROCMSG_OK;
}


/////////////////////////////////////////////////////////////////////////////////

CClientInviteTask::CClientInviteTask(CInstExt *pcInst):CSipTask(pcInst)
{
	m_tInviteID = INVALID_DIALOG_ID;
}

CClientInviteTask::~CClientInviteTask()
{
	if ( INVALID_DIALOG_ID != m_tInviteID )
	{
		OspSipReleaseDlg(m_tInviteID);
		m_tInviteID = INVALID_DIALOG_ID;
	}
}

const char* CClientInviteTask::GetObjName() const
{
	return "CClientInviteTask";
}

const char* CClientInviteTask::GetStateName(u32 dwState)
{
	switch(dwState)
	{
	case UNINVITE_STATE:
		{
			return "UnInvite State!";
		}
		break;
	case INVITE_STATE:
		{
			return "Invited State!";
		}
		break;
	default:
		break;
	}
	return "UnKnow State!";
}

void CClientInviteTask::TerminateTask()
{
	delete this;
}

void CClientInviteTask::InitStateMachine()
{
	CStateProc cUnInvProc;
	cUnInvProc.ProcMsg = (CTask::PFProcMsg)&CClientInviteTask::UnInviteProcMsg;
	cUnInvProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(UNINVITE_STATE, cUnInvProc);

	CStateProc cInvProc;
	cInvProc.ProcMsg = (CTask::PFProcMsg)&CClientInviteTask::InviteProcMsg;
	cInvProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(INVITE_STATE, cInvProc);

	SetCurState(UNINVITE_STATE);
}

u32 CClientInviteTask::UnInviteProcMsg(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	
	switch(pcMsg->event)
	{
	case EV_C_S_INVITE_REQ:
		{
			m_tInviteID = pcOspSipMsg->GetSipDlgID();
			CEventReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			CEventRsp cRsp;
			cRsp.SetErrorCode(EV_SUCCESS);
			cRsp.SetEvent("EV_S_C_INVITE_RSP");
			cRsp.SetSeqNum(cReq.GetSeqNum());
			cRsp.SetSession(cReq.GetSession());

			PostRsp(KDSIP_EVENT_INVITE_RSP,pcOspSipMsg->GetSipTransID(),cRsp);

			NextState(INVITE_STATE);
		}
		break;
	case EV_C_S_INVITE_BYE_REQ:
		{
			CEventReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			CEventRsp cRsp;
			cRsp.SetErrorCode(EV_UNINVITE);
			cRsp.SetEvent("EV_S_C_INVITE_BYE_RSP");
			cRsp.SetSeqNum(cReq.GetSeqNum());
			cRsp.SetSession(cReq.GetSession());

			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP,pcOspSipMsg->GetSipTransID(),cRsp);
		}
		break;
	default:
		{
			OspPrintf(TRUE,FALSE,"Recive UnKnow Message [%s]",pcMsg->event);
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CClientInviteTask::InviteProcMsg(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

	switch(pcMsg->event)
	{
	case EV_C_S_INVITE_ACK:
		{
			OspPrintf(TRUE,FALSE,"�ͻ����Ѿ�׼���ã����Է���������");
			return PROCMSG_OK;
		}
	case EV_C_S_INVITE_BYE_REQ:
		{
			CEventReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tInviteID = INVALID_DIALOG_ID;

			CEventRsp cRsp;
			cRsp.SetErrorCode(EV_SUCCESS);
			cRsp.SetEvent("EV_S_C_INVITE_BYE_RSP");
			cRsp.SetSeqNum(cReq.GetSeqNum());
			cRsp.SetSession(cReq.GetSession());

			if(PROCMSG_OK != PostRsp(KDSIP_EVENT_INVITE_BYE_RSP,pcOspSipMsg->GetSipTransID(),cRsp))
			{
				OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INVITE_BYE_RSPʧ�ܣ�");
				return PROCMSG_FAIL;
			}
			OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INVITE_BYE_RSP�ɹ���");
			NextState(UNINVITE_STATE);
			TerminateTask();
		}
		break;
	case EV_C_S_INFO_REQ:
		{
			CSendMsgReq myReq;
			pcOspSipMsg->GetMsgBody(myReq);

			OspPrintf(TRUE,FALSE,"\n�յ��ͻ��˷�����INFO��Ϣ��\n%s\n��\n",myReq.GetMsg().c_str());


			std::ofstream output("bigstr.txt");
			output<<myReq.GetMsg();

			CSendMsgRsp myRsp;
			myRsp.SetErrorCode(EV_SUCCESS);
			myRsp.SetEvent("EV_S_C_INFO_RSP");
			myRsp.SetSeqNum(myReq.GetSeqNum());
			myRsp.SetSession(m_strSessId);
			if(PROCMSG_OK != PostRsp(KDSIP_EVENT_INFO_RSP,pcOspSipMsg->GetSipTransID(),myRsp))
			{
				OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INFO_RSPʧ�ܣ�");
				return PROCMSG_FAIL;
			}
		}
		break;
	case EV_C_S_MESSAGE_DIALOG_REQ:
		{
			CSendMsgReq myReq;
			pcOspSipMsg->GetMsgBody(myReq);

			OspPrintf(TRUE,FALSE,"\n�յ��ͻ��˷�����MESSAGE_DIALOG��Ϣ��%s��\n",myReq.GetMsg().c_str());
			CSendMsgRsp myRsp;
			myRsp.SetErrorCode(EV_SUCCESS);
			myRsp.SetEvent("EV_S_C_MESSAGE_DIALOG_RSP");
			myRsp.SetSeqNum(myReq.GetSeqNum());
			myRsp.SetSession(m_strSessId);
			if(PROCMSG_OK != PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP,pcOspSipMsg->GetSipTransID(),myRsp))
			{
				OspPrintf(TRUE,FALSE,"����KDSIP_EVENT_INFO_RSPʧ�ܣ�");
				return PROCMSG_FAIL;
			}
		}
		break;
	default:
		{
			OspPrintf(TRUE,FALSE,"Recive UnKonw Message :[%s]",pcMsg->event);
		}
	}
	return PROCMSG_OK;
}


void CClientInviteTask::SetSessId(const string strSessId)
{
	m_strSessId = strSessId;
}

void CClientInviteTask::SetClientURI(const TSipURI client)
{
	m_tClientURI = client;
}

void CClientInviteTask::SetDialogId(TKDSIP_DIALOG_ID id)
{
	m_tInviteID = id;
}

string CClientInviteTask::GetSessId()
{
	return m_strSessId;
}

TSipURI CClientInviteTask::GetClientURE()
{
	return m_tClientURI;
}

TKDSIP_DIALOG_ID CClientInviteTask::GetDialogId()
{
	return m_tInviteID;
}

