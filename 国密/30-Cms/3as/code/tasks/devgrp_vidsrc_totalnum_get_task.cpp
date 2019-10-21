#include "devgrp_vidsrc_totalnum_get_task.h"

#include "user_devgrp_vidsrc_query_task.h"
#include "common/ssn_taskbase.h"

CDevGrpVidsrcTotalnumGetTask::CDevGrpVidsrcTotalnumGetTask( CSsnTaskbase* pcSsnTask )
	:CSipTask(pcSsnTask->GetInstance()), m_pcSsnTask(pcSsnTask)
{
	m_strObjName = "CDevGrpVidsrcTotalnumGetTask";
}

CDevGrpVidsrcTotalnumGetTask::~CDevGrpVidsrcTotalnumGetTask()
{
	m_pcSsnTask->ReverseDelGetDevGrpVidSrcNumRecord(this);
}

const char* CDevGrpVidsrcTotalnumGetTask::GetStateName( u32 dwState ) const
{
	if (dwState == enStateIdle) return "enStateIdle";
	if (dwState == enStateGet) return "enStateGet";
	return "unknown";
}

void CDevGrpVidsrcTotalnumGetTask::InitStateMachine()
{
	DoNextState(enStateIdle);

	CStateProc cProc;

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpVidsrcTotalnumGetTask::OnIdle;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(enStateIdle, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpVidsrcTotalnumGetTask::OnGet;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrpVidsrcTotalnumGetTask::OnGetTimerPoll;
	AddRuleProc(enStateGet, cProc);
}

u32 CDevGrpVidsrcTotalnumGetTask::OnIdle( CMessage *const pcMsg )
{
	if (TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ == pcMsg->event)
	{
		COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
		CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
		pcSipMsg->GetMsgBody(cTrans.GetMsgObj());

		if (PostTaskMsgTransfer2DbTaskPool(cTrans))
		{
			m_strObjName.append("[");
			m_strObjName.append(OspExtEventDesc(pcMsg->event));
			m_strObjName.append("]");
			DoNextState(enStateGet);
			return PROCMSG_OK;
		}
		else
		{
			Log(WARNING_LEV, "CDevGrpVidsrcTotalnumGetTask::OnIdle delete because PostTaskMsgTransfer2DbTaskPool fail !!!\n");
			CDevGrpVidsrcTotalnumGetRsp cRsp;
			cRsp.SetHeadFromReq(cTrans.GetMsgObj());
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp.ToXml());
			return PROCMSG_DEL;
		}
	}
	else
	{
		Log(WARNING_LEV, "CDevGrpVidsrcTotalnumGetTask::OnIdle delete with wrong-msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		return PROCMSG_DEL;
	}
}

u32 CDevGrpVidsrcTotalnumGetTask::OnGet( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db != pcMsg->event)
	{
		CMS_ASSERT(false);
		Log(WARNING_LEV, "CDevGrpVidsrcTotalnumGetTask::OnQuery delete(invalid message)\n");
		return PROCMSG_DEL;
	}

	DoNextState(enStateGet); // 收到了应答，重新进入状态更新当前状态Holdtime

	ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
	CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

	if (TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP == pcTrans->GetEventId())
	{
		CDevGrpVidsrcTotalnumGetRsp *pcRsp = reinterpret_cast<CDevGrpVidsrcTotalnumGetRsp*>(pcTrans->GetMsgObjAddr());
		*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // 自己保存之后置空，这样CCtrl才不会Delete掉它

		Log(PROGRESS_LEV, "CDevGrpVidsrcTotalnumGetTask::OnGet rsp's Records:%d, \n", pcRsp->GetDevGrpVidsrcNum().size());

		DoNextState(enStateGet);

		string strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
		if(PROCMSG_OK == PostMsgRsp(pcTrans->GetSipTransId(), strMsg))
		{
			Log(PROGRESS_LEV, "CDevGrpVidsrcTotalnumGetTask PostMsgRsp success \n");
		}
		else
		{
			Log(ERROR_LEV, "CDevGrpVidsrcTotalnumGetTask PostMsgRsp fail \n");
		}

		return PROCMSG_DEL;
	}
	else
	{
		Log(WARNING_LEV, "CDevGrpVidsrcTotalnumGetTask::OnGet recv err-msg[%d-%s] !\n",
			pcTrans->GetEventId(), OspExtEventDesc(pcTrans->GetEventId()).c_str());
		return PROCMSG_DEL;
	}
}

u32 CDevGrpVidsrcTotalnumGetTask::OnGetTimerPoll( )
{
	return (GetCurStateHoldTime() > 60*5)? TIMERPOLL_DEL: TIMERPOLL_DONE; // 累计300秒数据库查询不返回就删除任务
}

void CDevGrpVidsrcTotalnumGetTask::DoNextState( u32 enState )
{
	NextState(enState);
}
