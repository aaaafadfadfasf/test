/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : serverinst.cpp
   相关文件    : 
   文件实现功能: serverinst的实现
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#include "serverinst.h"

CServerInst::CServerInst()
{

}

CServerInst::~CServerInst()
{

}

LPCSTR CServerInst::GetStrState()
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

void CServerInst::DaemonEntry(CMessage* const pcMsg)
{
    switch (pcMsg->event)
    {
    case EV_CLIENT_SERVER_REG_REQ:
        {
            COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;

            //CRegTask *pcRegTask = new CRegTask(this);
            CTask *pcRegTask = new CRegTask(this);
            u32 dwRet = pcRegTask->ProcMsg(pcMsg);
        }
        break;
    case OSP_PROXY_CONNECT:
        {
            //开始运行
        }
        break;
    case OSP_SIP_DISCONNECT:
        {
            COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;
            CTask *pcRegTask = FindTask(pcMonMsg->GetOspTaskNO());
            if ( NULL == pcRegTask )
            {
                OspPrintf(TRUE, FALSE, "找不到断链节点的事务\n");
            }
            else
            {
                pcRegTask->ProcMsg(pcMsg);
            }
        }
        break;
    case EV_CLIENT_SERVER_SEND_MSG_REQ:
    case EV_CLIENT_SERVER_UNREG_REQ:
    case EV_C_S_SUBSCRIBE_MSG_REQ:
    case EV_C_S_UNSUBSCRIBE_MSG_REQ:
    case EV_S_C_BROADCAST_MSG_NTF_RSP:
	case EV_C_S_INVITE_REQ:
	case EV_C_S_INVITE_ACK:
	case EV_C_S_INVITE_BYE_REQ:
	case EV_C_S_INFO_REQ:
	case EV_C_S_MESSAGE_DIALOG_REQ:
        {
            COspSipMsg *pcMonMsg = (COspSipMsg *)pcMsg->content;
            string strSession = GetXmlVal(pcMonMsg->GetMsgBody(), "session");
            PCUserData pcUserData = g_cServerApp.GetAppData().FindUserBySessId(strSession);
            if ( NULL != pcUserData )
            {
                pcUserData->ProcMsg(pcMsg);
            }
            else
            {
                OspPrintf(TRUE, FALSE, "没找到会话 %s 对应的任务\n",
                    strSession.c_str());
            }
        }
        break;
    default:
        {
            OspPrintf(TRUE, FALSE, "\n\n[CServerInst::DaemonEntry]unknow msg %s--%d\n\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

