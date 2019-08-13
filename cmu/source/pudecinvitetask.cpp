#include "pudecinvitetask.h"
#include "devlogintask.h"
#include "puiregtask.h"
#include "cmudata.h"
#include "cmuconst.h"

CPuDecInviteTask::CPuDecInviteTask(CInstExt *pcInst) : CSipTask(pcInst)
{
	m_tSipTransId = INVALID_TRANS_ID;
	m_tSenderDlgId = INVALID_DIALOG_ID;
	m_tReceiverDlgId = INVALID_DIALOG_ID;
}

CPuDecInviteTask::~CPuDecInviteTask()
{
	ReleaseResource();
}

void CPuDecInviteTask::ReleaseResource()
{
	if (m_tSenderDlgId != INVALID_DIALOG_ID)
	{
		CByeReq cReq;
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tSenderDlgId);
		m_tSenderDlgId = INVALID_DIALOG_ID;
	}
	if (m_tReceiverDlgId != INVALID_DIALOG_ID)
	{
		CByeReq cReq;
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tReceiverDlgId);
		m_tReceiverDlgId = INVALID_DIALOG_ID;
	}
	CLoginSession* ptUserLoginTask = GetLoginSession(m_tReq.GetSession());
	if (ptUserLoginTask != NULL)
	{
		ptUserLoginTask->DelTask(this);
	}
	TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDstChn().GetDevUri());
	CLoginSession* ptPuiLonginTask = GetPuiRegTask(tPuRouterUri);
	if (ptPuiLonginTask != NULL)
	{
		ptPuiLonginTask->DelTask(this);
	}
}

void CPuDecInviteTask::InitStateMachine()
{
	CStateProc cIdlState;
	cIdlState.ProcMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnIdle;
	cIdlState.ProcErrMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnIdle;
	cIdlState.TimerPoll = (CTask::PFTimerPoll)&CPuDecInviteTask::OnIdleTimer;
	AddRuleProc(Idle,cIdlState);

	CStateProc cRspState;
	cRspState.ProcMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnWaitRsp;
	cRspState.ProcErrMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnWaitRsp;
	cRspState.TimerPoll = (CTask::PFTimerPoll)&CPuDecInviteTask::OnWaitRspTimer;
	AddRuleProc(WaitRsp,cRspState);

	CStateProc cAckState;
	cAckState.ProcMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnWaitAck;
	cAckState.ProcErrMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnWaitAck;
	cAckState.TimerPoll = (CTask::PFTimerPoll)&CPuDecInviteTask::OnWaitAckTimer;
	AddRuleProc(WaitAck,cAckState);

	CStateProc cSrvState;
	cSrvState.ProcMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnService;
	cSrvState.ProcErrMsg = (CTask::PFProcMsg)&CPuDecInviteTask::OnService;
	cSrvState.TimerPoll = (CTask::PFTimerPoll)&CPuDecInviteTask::OnServiceTimer;
	AddRuleProc(Service,cSrvState);

	NextState(Idle);
}

u32 CPuDecInviteTask::OnIdle(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
		return PROCMSG_DEL;
	}

	switch(pcMsg->event)
	{
	case INVITE_DECORDER_CALL_REQ:
		{
			pcOspSipMsg->GetMsgBody(m_tReq);
			m_tSipTransId = pcOspSipMsg->GetSipTransID();
			m_tSenderDlgId = pcOspSipMsg->GetSipDlgID();

			//确定pu路由
			TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDstChn().GetDevUri());
			if (tPuRouterUri.IsNull())
			{
				TASKLOG(ERROR_LEV, "Dev[%s]无法路由, Msg[%s-%d]请求失败\n", 
					m_tReq.GetDstChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

				SendRsp(ERR_CMU_DEV_NO_ROUTER);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CLoginSession* ptPuiLonginTask = GetPuiRegTask(tPuRouterUri);
			if (ptPuiLonginTask == NULL)
			{
				TASKLOG(CRITICAL_LEV, "pui登录session[%s]非法\n", tPuRouterUri.GetURIString().c_str());

				SendRsp(ERR_CMU_PUI_NOT_REG);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}
			ptPuiLonginTask->AddTask(this);

			//用户是否登录
			CLoginSession* ptTvsLoginTask = GetLoginSession(m_tReq.GetSession());
			if (ptTvsLoginTask == NULL)
			{
				TASKLOG(CRITICAL_LEV, "tvs登录session[%s]非法\n", m_tReq.GetSession().c_str());

				SendRsp(ERR_TVS_NOT_LOGIN_CMU);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CInviteDecoderCallReq cReq(m_tReq);
			cReq.SetSession(m_tReq.GetDstChn().GetDevUri());

			PostReq(KDSIP_EVENT_INVITE_REQ,cReq, tPuRouterUri);
			ptTvsLoginTask->AddTask(this);

			NextState(WaitRsp);
		}
		break;
	default:
		{
			TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_DEL;
		}
		break;
	}
	return dwRet;
}
u32 CPuDecInviteTask::OnIdleTimer()
{
	return TIMERPOLL_DONE;
}

u32 CPuDecInviteTask::OnWaitRsp(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
		return PROCMSG_DEL;
	}

	switch(pcMsg->event)
	{
	case INVITE_DECORDER_CALL_RSP:
		{
			CInviteDecoderCallRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_tReceiverDlgId = pcOspSipMsg->GetSipDlgID();

			if (PROCMSG_OK != cRsp.GetErrorCode())
			{
				dwRet = PROCMSG_DEL;
			}
			PostRsp(KDSIP_EVENT_INVITE_RSP,m_tSipTransId, cRsp);
			NextState(WaitAck);
		}
		break;
	default:
		{
			TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_DEL;
		}
		break;
	}
	return dwRet;
}
u32 CPuDecInviteTask::OnWaitRspTimer()
{
	return TIMERPOLL_DONE;
}

u32 CPuDecInviteTask::OnWaitAck(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
		return PROCMSG_DEL;
	}
	switch(pcMsg->event)
	{
	case INVITE_DECORDER_CALL_ACK:
		{
			CInviteDecoderCallAck cAck;
			pcOspSipMsg->GetMsgBody(cAck);

			PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cAck, m_tReceiverDlgId);

			NextState(Service);
		}
		break;
	case BYE_REQ:
		{
			if (IsFromSender(pcOspSipMsg->GetSipFromUri().GetURIString()))
			{
				m_tSenderDlgId = INVALID_DIALOG_ID;

				CByeReq cReq;
				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tReceiverDlgId);
				m_tReceiverDlgId = INVALID_DIALOG_ID;
			}
			else
			{
				m_tReceiverDlgId = INVALID_DIALOG_ID;

				CByeReq cReq;
				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tSenderDlgId);
				m_tSenderDlgId = INVALID_DIALOG_ID;
			}

			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_DISCONNECT:
		{
			//释放sip协议栈资源
			if (m_tReceiverDlgId != INVALID_DIALOG_ID)
			{  
				OspSipReleaseDlg(m_tReceiverDlgId);
				m_tReceiverDlgId = INVALID_DIALOG_ID;
			}
			if (m_tSenderDlgId != INVALID_DIALOG_ID)
			{
				OspSipReleaseDlg(m_tSenderDlgId);
				m_tSenderDlgId = INVALID_DIALOG_ID;
			}

			dwRet = PROCMSG_DEL;
		}
		break;

	case OSP_SIP_DIALOG_TERMINATE:
		{
			//SIP层Dialog异常终止通知
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(ERROR_LEV, "SIP层Dialog异常终止, 事务终止，sip status code[%d]\n",
				pcOspSipMsg->GetSipErrorCode());

			m_tSenderDlgId = INVALID_DIALOG_ID;
			m_tReceiverDlgId = INVALID_DIALOG_ID;

			dwRet = PROCMSG_DEL;    
		}
		break;
	default:
		{
			TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_DEL;
		}
		break;
	}
	return dwRet;
}
u32 CPuDecInviteTask::OnWaitAckTimer()
{
	return TIMERPOLL_DONE;
}

u32 CPuDecInviteTask::OnService(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
		return PROCMSG_DEL;
	}
	switch(pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			if (IsFromSender(pcOspSipMsg->GetSipFromUri().GetURIString()))
			{
				m_tSenderDlgId = INVALID_DIALOG_ID;

				CByeReq cReq;
				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tReceiverDlgId);
				m_tReceiverDlgId = INVALID_DIALOG_ID;
			}
			else
			{
				m_tReceiverDlgId = INVALID_DIALOG_ID;

				CByeReq cReq;
				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tSenderDlgId);
				m_tSenderDlgId = INVALID_DIALOG_ID;
			}

			dwRet = PROCMSG_DEL;
		}
		break;
	case CMU_DEV_OFFLINE_NTF:
		{
			//释放sip协议栈资源
			TDevOnOffCBMsg* pDevOnOffCBMsg = (TDevOnOffCBMsg*)pcMsg->content;
			//断链消息来自tvs
			if (pDevOnOffCBMsg->GetDevType()==DEV_TYPE_TVS)
			{
				if (m_tReceiverDlgId != INVALID_DIALOG_ID)
				{  
					CByeReq cReq;
					PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tReceiverDlgId);
					m_tReceiverDlgId = INVALID_DIALOG_ID;
				}
				if (m_tSenderDlgId != INVALID_DIALOG_ID)
				{
					OspSipReleaseDlg(m_tSenderDlgId);
					m_tSenderDlgId = INVALID_DIALOG_ID;
				}
			}
			else //断链消息来自pui
			{
				if (m_tSenderDlgId != INVALID_DIALOG_ID)
				{  
					CByeReq cReq;
					PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,cReq,m_tSenderDlgId);
					m_tSenderDlgId = INVALID_DIALOG_ID;
				}
				if (m_tReceiverDlgId != INVALID_DIALOG_ID)
				{
					OspSipReleaseDlg(m_tReceiverDlgId);
					m_tReceiverDlgId = INVALID_DIALOG_ID;
				}
			}

			dwRet = PROCMSG_DEL;
		}
		break;

	case OSP_SIP_DIALOG_TERMINATE:
		{
			//SIP层Dialog异常终止通知
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(ERROR_LEV, "SIP层Dialog异常终止, 事务终止，sip status code[%d]\n",
				pcOspSipMsg->GetSipErrorCode());

			m_tSenderDlgId = INVALID_DIALOG_ID;
			m_tReceiverDlgId = INVALID_DIALOG_ID;

			dwRet = PROCMSG_DEL;    
		}
		break;
	}
	return dwRet;
}
u32 CPuDecInviteTask::OnServiceTimer()
{
	return TIMERPOLL_DONE;
}

bool CPuDecInviteTask::SendRsp(u32 dwError)
{
	CInviteDecoderCallRsp cRsp;
	cRsp.SetSeqNum(m_tReq.GetSeqNum());
	cRsp.SetSession(m_tReq.GetSession());
	cRsp.SetErrorCode((int)dwError);

	return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}

CPuiRegTask* CPuDecInviteTask::GetPuiRegTask(TSipURI tPuiUri)
{
	CLoginSession* ptPuiRegTask = NULL;
	g_cPuiList.Find(tPuiUri.GetURIString(), ptPuiRegTask);

	return (CPuiRegTask*)ptPuiRegTask;
}

//消息来源是否来自请求方，目前此task仅用于录像上墙，请求方为tvs模块
bool CPuDecInviteTask::IsFromSender(const string& strUri)
{
#define MODULE_FLAG_TVS "tvs"
	return (string::npos != strUri.find(MODULE_FLAG_TVS));
}