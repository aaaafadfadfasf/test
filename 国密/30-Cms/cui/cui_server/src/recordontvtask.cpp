#include "recordontvtask.h"
#include "culogintask.h"

CRecordOnTvTask::CRecordOnTvTask(CInstExt* pInst,CCuAgent *pcCuAgent,PTRecvNode ptRecvNode) : CSipTask(pInst)
{
	m_pcCuAgent = pcCuAgent;
	m_tTvsUri.SetURIString(string( DEV_TYPE_TVS ) + "@" + GetDomainName());
	m_ptRecvNode = ptRecvNode;
}

CRecordOnTvTask::~CRecordOnTvTask()
{
	m_pcCuAgent->DelRecTvTask(m_tTvChn);
	if (Service == GetState())      //此种状态下，析构task说明cu与cui断链
	{
		CRecordOffTvReq cReq;
		cReq.SetTvChn(m_tTvChn);
		cReq.SetSession(GetTvsSession());
		PostMsgReq(cReq,m_tTvsUri);
	}
}

void CRecordOnTvTask::InitStateMachine()
{
	CStateProc cWaitProc;
	cWaitProc.ProcMsg = (CTask::PFProcMsg)&CRecordOnTvTask::OnWait;
	cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CRecordOnTvTask::OnWait;
	cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CRecordOnTvTask::OnWaitTimer;
	AddRuleProc(Wait, cWaitProc);

	CStateProc cServiceProc;
	cServiceProc.ProcMsg = (CTask::PFProcMsg)&CRecordOnTvTask::OnService;
	cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CRecordOnTvTask::OnService;
	cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CRecordOnTvTask::OnServiceTimer;
	AddRuleProc(Service, cServiceProc);

	NextState(Wait);
}

u32 CRecordOnTvTask::OnWait(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

	switch (pcMsg->event)
	{
	case RECORD_ON_TV_REQ:
		{
			CRecordOnTvReq* pcReq =(CRecordOnTvReq*)pcMsg->content;
			pcReq->SetSession(GetTvsSession());

			m_tTvChn = pcReq->GetTvChn();
			PostMsgReq(*pcReq,m_tTvsUri);

			m_pcCuAgent->AddRecTvTask(m_tTvChn,this);
			NextState(Service);
		}
		break;
	case RECORD_OFF_TV_RSP:
		{
			CRecordOffTvRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			TASKLOG(NORMAL_TASK, ERROR_LEV, "录像下墙，错误码[%d]\n",cRsp.GetErrorCode());

			m_pcCuAgent->DelRecTvTask(m_tTvChn);
			SendRspToCu<CRecordOffTvRsp>(cRsp,m_ptRecvNode);
			delete this;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_DEL;
		}
		break;
	}

	return dwRet;
}
u32 CRecordOnTvTask::OnWaitTimer()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待录像上墙应答超时，结束会话\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CRecordOnTvTask::OnService(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	switch (pcMsg->event)
	{
	case RECORD_ON_TV_RSP:
		{
			CRecordOnTvRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			SendRspToCu<CRecordOnTvRsp>(cRsp,m_ptRecvNode);
			if (PROCMSG_OK != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "录像上墙失败，错误码[%d]\n",cRsp.GetErrorCode());
				m_pcCuAgent->DelRecTvTask(m_tTvChn);
				delete this;
			}
		}
		break;
	case RECORD_OFF_TV_REQ:
		{
			CRecordOffTvReq* pcReq = (CRecordOffTvReq*)pcMsg->content;;
			pcReq->SetSession(GetTvsSession());

			PostMsgReq(*pcReq,m_tTvsUri);
			NextState(Wait);
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_DEL;
		}
		break;
	}
	return dwRet;
}

u32 CRecordOnTvTask::OnServiceTimer()
{
	return TIMERPOLL_DONE;
}

string CRecordOnTvTask::GetTvsSession()
{
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	return pcInst->IsProvideServer(m_tTvsUri, GetTaskNO());
}