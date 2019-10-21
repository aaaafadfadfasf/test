/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : clientinst.cpp
   相关文件    : 
   文件实现功能: ClientInst的实现
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
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
        OspPrintf(TRUE, FALSE, "Proxy断链, 销毁所有的事务\n");
        EmptyTask();
        NextState(Idle);
        return TRUE;
    }
    else if ( OSP_SIP_DISCONNECT == pcMsg->event )
    {
        OspPrintf(TRUE, FALSE, "与Server断链, 销毁所有的事务\n");
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

//Idle状态下实例处理函数
void CClientInst::OnIdle(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        {
            //连接上Proxy
            NextState(Service);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            // SIP协议栈处理失败，多半因为返回404，对端UA不在线，根据TaskID处理
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

//Service状态下实例处理函数
void CClientInst::OnService(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case EV_START_REG:
        {
            char *pClientName = (char *)pcMsg->content;
            if ( 0 == strlen(pClientName) )
            {
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
                return;
            }
            CClientTask *pcTask = FindClient(pClientName);
            if ( NULL != pcTask )
            {
                pcTask->StartUnReg();
            }
			else
			{
				OspPrintf(TRUE,FALSE,"没有找到用户！");
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
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
				OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
				OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
                OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
				OspPrintf(TRUE, FALSE, "请求注册的用户名为空\n");
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
            // SIP协议栈处理失败，多半因为返回404，对端UA不在线，根据TaskID处理
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
                OspPrintf(TRUE, FALSE, "[SERVICE]没找到应答消息[%s--%d]含带的会话[%s]对应的客户端\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, strSession.c_str());
            }
            //OspPrintf(TRUE, FALSE, "[SERVICE]unknow msg %s--%d\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}
