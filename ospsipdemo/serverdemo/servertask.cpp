/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : servertask.cpp
   相关文件    : 
   文件实现功能: ServerTask的实现
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#include "servertask.h"
#include "cms/utility/uuid.h"
#include "serverinst.h"



#include <fstream>

CRegTask::CRegTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    //初始就是CRegState状态
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
    //销毁该用户的所有事务
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

    //在全局用户列表中删除此用户
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

            OspPrintf(TRUE, FALSE, "收到客户端 %s, 的注册消息\n", cRegReq.GetUserName().c_str());

            u32 dwErrorCode = EV_SUCCESS;
            CUserData *pcUserData = NULL;
            if (NULL != g_cServerApp.GetAppData().FindUserByUserName(cRegReq.GetUserName()))
            {
                dwErrorCode = EV_USER_ALREADY_EXIST;
                OspPrintf(TRUE, FALSE, "已经有同名用户 %s 在线了, 请求将被拒绝\n", cRegReq.GetUserName().c_str());
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

                //切换到下一状态
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
                //设置心跳
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
            OspPrintf(TRUE, FALSE, "用户 %s 断链, 销毁此任务\n",
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
            OspPrintf(TRUE, FALSE, "用户 %s 断链, 销毁此任务\n",
                pcMonMsg->GetMsgBody());
            TerminateTask();
        }
    	break;
    case EV_CLIENT_SERVER_SEND_MSG_REQ:
        {
            CSendMsgReq cSendMsgReq;
            pcMonMsg->GetMsgBody(cSendMsgReq);

            OspPrintf(TRUE, FALSE, "收到客户 %s 发送的消息 %s\n", 
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
            OspPrintf(TRUE, FALSE, "收到客户 %s 的注销请求\n", 
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
            OspPrintf(TRUE, FALSE, "收到客户 %s 订阅请求\n", 
                m_strUserName.c_str());

            //先判断是否已经订阅
            if ( !m_tTaskArray.empty() )
            { //已经订阅过
                OspPrintf(TRUE, FALSE, "客户 %s 已经订阅过，忽略本次操作\n", 
                    m_strUserName.c_str());
                CTask *pcSubscribeTask = m_tTaskArray.at(0);
                pcSubscribeTask->ProcMsg(pcMsg);
            }
            else
            {
                //CClientObsvr *pcObsvr = new CClientObsvr(g_cServerApp.GetAppData().GetSubject())
                //创建子任务
                CClientSubscribeTask *pcSubscribeTask = new CClientSubscribeTask(GetInstance());
                pcSubscribeTask->SetSessionID(m_strSessId);
                pcSubscribeTask->SetClientURI(m_tClientURI);

				pcSubscribeTask->SetSubscribeID(pcMonMsg->GetSipDlgID());

                //让子任务处理消息
                u32 dwRet = pcSubscribeTask->ProcMsg(pcMsg);
                if ( PROCMSG_OK == dwRet )
                {
                    //将子任务加入到子任务集合中
                    AddTask(pcSubscribeTask);
                }
            }
        }
        break;
    case EV_C_S_UNSUBSCRIBE_MSG_REQ:
        {
            OspPrintf(TRUE, FALSE, "收到客户 %s 取消订阅请求\n", 
                m_strUserName.c_str());

            //找到子任务
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
			OspPrintf(TRUE, FALSE, "收到客户 %s 浏览请求\n", 
				m_strUserName.c_str());

			if ( !m_InviteArray.empty() )
			{ //已经建立浏览
				vector<CTask *>::iterator pItem = m_InviteArray.begin();
				while ( pItem != m_InviteArray.end() )
				{
					CClientInviteTask *pcTask = (CClientInviteTask*)(*pItem);
					if ( pcTask->GetDialogId() == pcMonMsg->GetSipDlgID() )
					{
						OspPrintf(TRUE,FALSE,"该任务已经存在！");
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
				OspPrintf(TRUE,FALSE,"没有找到该任务！");
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
			OspPrintf(TRUE,FALSE,"没有找到该任务！");
			return PROCMSG_FAIL;
		}
		break;
	case EV_C_S_INVITE_BYE_REQ:
		{
			if(m_InviteArray.empty())
			{
				OspPrintf(TRUE,FALSE,"没有找到该任务！");
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
			OspPrintf(TRUE,FALSE,"没有找到该任务！");
			return PROCMSG_FAIL;
		}
		break;
    default:
        {
            OspPrintf(TRUE,FALSE,"\n\n收到消息\n\n");
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

            //尝试订阅
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

            //尝试取消订阅
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
			OspPrintf(TRUE,FALSE,"客户端收到广播消息！");
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
			OspPrintf(TRUE,FALSE,"客户端已经准备好，可以发送码流！");
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
				OspPrintf(TRUE,FALSE,"发送KDSIP_EVENT_INVITE_BYE_RSP失败！");
				return PROCMSG_FAIL;
			}
			OspPrintf(TRUE,FALSE,"发送KDSIP_EVENT_INVITE_BYE_RSP成功！");
			NextState(UNINVITE_STATE);
			TerminateTask();
		}
		break;
	case EV_C_S_INFO_REQ:
		{
			CSendMsgReq myReq;
			pcOspSipMsg->GetMsgBody(myReq);

			OspPrintf(TRUE,FALSE,"\n收到客户端发来的INFO消息【\n%s\n】\n",myReq.GetMsg().c_str());


			std::ofstream output("bigstr.txt");
			output<<myReq.GetMsg();

			CSendMsgRsp myRsp;
			myRsp.SetErrorCode(EV_SUCCESS);
			myRsp.SetEvent("EV_S_C_INFO_RSP");
			myRsp.SetSeqNum(myReq.GetSeqNum());
			myRsp.SetSession(m_strSessId);
			if(PROCMSG_OK != PostRsp(KDSIP_EVENT_INFO_RSP,pcOspSipMsg->GetSipTransID(),myRsp))
			{
				OspPrintf(TRUE,FALSE,"发送KDSIP_EVENT_INFO_RSP失败！");
				return PROCMSG_FAIL;
			}
		}
		break;
	case EV_C_S_MESSAGE_DIALOG_REQ:
		{
			CSendMsgReq myReq;
			pcOspSipMsg->GetMsgBody(myReq);

			OspPrintf(TRUE,FALSE,"\n收到客户端发来的MESSAGE_DIALOG消息【%s】\n",myReq.GetMsg().c_str());
			CSendMsgRsp myRsp;
			myRsp.SetErrorCode(EV_SUCCESS);
			myRsp.SetEvent("EV_S_C_MESSAGE_DIALOG_RSP");
			myRsp.SetSeqNum(myReq.GetSeqNum());
			myRsp.SetSession(m_strSessId);
			if(PROCMSG_OK != PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP,pcOspSipMsg->GetSipTransID(),myRsp))
			{
				OspPrintf(TRUE,FALSE,"发送KDSIP_EVENT_INFO_RSP失败！");
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

