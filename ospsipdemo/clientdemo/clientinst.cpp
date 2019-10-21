/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : clientinst.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ClientInst��ʵ��
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#include "clientinst.h"
#include "clienttask.h"
#include "cms/cmsassert.h"

CClientInst::CClientInst()
{

}

CClientInst::~CClientInst()
{

}

LPCSTR CClientInst::GetStrState()
{
    switch (CurState())
    {
    case Idle:
        return "Idle";
    	break;
    case Service:
        return "Service";
        break;
    default:
        break;
    }
    return "Unknown State";
}

BOOL32 CClientInst::OnUserInnerCommonMsg(CMessage* const pcMsg)
{
    if ( OSP_PROXY_DISCONNECT == pcMsg->event )
    {
        OspPrintf(TRUE, FALSE, "Proxy����, �������е�����\n");
        EmptyTask();
        NextState(Idle);
        return TRUE;
    }
    else if ( OSP_SIP_DISCONNECT == pcMsg->event )
    {
        OspPrintf(TRUE, FALSE, "��Server����, �������е�����\n");
        EmptyTask();
        return TRUE;
    }
    return FALSE;
}

void CClientInst::DaemonEntry(CMessage* const pcMsg)
{
    switch (CurState())
    {
    case Idle:
        {
            OnIdle(pcMsg);
        }
        break;
    case Service:
        {
            OnService(pcMsg);
        }
        break;
    default:
        break;
    }
}

//Idle״̬��ʵ��������
void CClientInst::OnIdle(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        {
            //������Proxy
            NextState(Service);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            // SIPЭ��ջ����ʧ�ܣ������Ϊ����404���Զ�UA�����ߣ�����TaskID����
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CTask *pcTask = FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( pcTask != NULL )
            {
                pcTask->ProcMsg(pcMsg);
            }
        }
        break;
    default:
        {
            OspPrintf(TRUE, FALSE, "[IDLE]unknow msg %s--%d\n", 
                OspExtEventDesc(pcMsg->event).c_str(),pcMsg->event);
        }
        break;
    }
}

CClientTask * CClientInst::FindClient(const char *pClientName)
{
    if ( NULL == pClientName )
    {
        return NULL;
    }

    if ( 0 == GetTaskNum() )
    {
        return NULL;
    }

    vector<CTask*> cTaskArry = GetTasksArray();
    vector<CTask*>::iterator pItem = cTaskArry.begin();
    while (pItem != cTaskArry.end())
    {
        
        CClientTask *pcTask = (CClientTask *)((*pItem) ->GetThisTask());
        if ( pcTask->m_strUserName == pClientName )
        {
            return pcTask;
        }
        ++pItem;
    }
    return NULL;
}

CClientTask * CClientInst::FindClientBySession(const char *pClientSession)
{
    if ( NULL == pClientSession )
    {
        return NULL;
    }

    if ( 0 == GetTaskNum() )
    {
        return NULL;
    }

    vector<CTask*> cTaskArry = GetTasksArray();
    vector<CTask*>::iterator pItem = cTaskArry.begin();
    while (pItem != cTaskArry.end())
    {

        CClientTask *pcTask = (CClientTask *)((*pItem) ->GetThisTask());
        if ( pcTask->m_strSession == pClientSession )
        {
            return pcTask;
        }
        ++pItem;
    }
    return NULL;
}

//Service״̬��ʵ��������
void CClientInst::OnService(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case EV_START_REG:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcRegTask = new CClientTask(this);
            pcRegTask->m_strUserName = pClientName;
            pcRegTask->m_tServerUri.SetNOString(SERVER_URI);
            pcRegTask->StartReg();
        }
        break;
    case EV_SERVER_CLIENT_REG_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CTask *pcTask = FindTask(pcOspSipMsg->GetOspTaskNO());
            CMS_ASSERT(NULL != pcTask);
            pcTask->ProcMsg(pcMsg);
        }
        break;
    case EV_STOP_REG:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->StartUnReg();
            }
			else
			{
				OspPrintf(TRUE,FALSE,"û���ҵ��û���");
			}
        }
        break;
    case EV_SEND_MSG:
        {
            char szBuf[1024] = {0};
            memcpy((u8*)szBuf, pcMsg->content, 1024);
            char *pClientName = szBuf;
            char *pMsg = pClientName + 512;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->SendMsg(pMsg);
            }
        }
        break;
    case EV_START_SS:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->StartSubscribe();
            }
        }
        break;
    case EV_STOP_SS:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->StopSubscribe();
            }
        }
        break;
	case EV_START_IN:
		{
			char *pClientName = (char *)pcMsg->content;
			if ( 0 == strlen(pClientName) )
			{
				OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
				return;
			}
			CClientTask *pcTask = FindClient(pClientName);
			if ( NULL != pcTask )
			{
				pcTask->StartInvite();
			}
		}
		break;
	case EV_STOP_IN:
		{
			char *pClientName = (char *)pcMsg->content;
			if ( 0 == strlen(pClientName) )
			{
				OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
				return;
			}
			CClientTask *pcTask = FindClient(pClientName);
			if ( NULL != pcTask )
			{
				pcTask->StopInvite();
			}
		}
		break;
	case EV_SEND_INFO:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->SendInfoMsg();
            }
        }
        break;
	case EN_SEND_DLG_MSG:
		{
			char *pClientName = (char *)pcMsg->content;
			if ( 0 == strlen(pClientName) )
			{
				OspPrintf(TRUE, FALSE, "����ע����û���Ϊ��\n");
				return;
			}
			CClientTask *pcTask = FindClient(pClientName);
			if ( NULL != pcTask )
			{
				pcTask->SendDlgMsg();
			}
		}
		break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            // SIPЭ��ջ����ʧ�ܣ������Ϊ����404���Զ�UA�����ߣ�����TaskID����
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CTask *pcTask = FindTask(pcOspSipMsg->GetOspTaskNO());
            if ( pcTask != NULL )
            {
                pcTask->ProcMsg(pcMsg);
            }
        }
        break;
    default:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            string strSession = GetXmlVal(pcOspSipMsg->GetMsgBody(), "session");
            CTask *pcTask = FindClientBySession(strSession.c_str());
            if ( NULL != pcTask )
            {
                pcTask->ProcMsg(pcMsg);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "[SERVICE]û�ҵ�Ӧ����Ϣ[%s--%d]�����ĻỰ[%s]��Ӧ�Ŀͻ���\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, strSession.c_str());
            }
            //OspPrintf(TRUE, FALSE, "[SERVICE]unknow msg %s--%d\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}
