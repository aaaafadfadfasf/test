#include "batoptchecktask.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "batopttask.h"


CCuBatOptCheckTask::CCuBatOptCheckTask( CInstExt *pcInst ) : CSipTask(pcInst)
{
	m_enNextCheckList = EBAT_OPT_PLAT_LIST;
}

CCuBatOptCheckTask::~CCuBatOptCheckTask()
{
	TerminateTask();
}

void CCuBatOptCheckTask::TerminateTask()
{
	u32 nTaskNo = this->GetTaskNO();
	if ( nTaskNo == g_cCuiDataMgr.m_dwBatOptCheckTaskNo)
	{
		g_cCuiDataMgr.m_dwBatOptCheckTaskNo = INVALID_TASKNO;
		g_cCuiDataMgr.m_cPlatBatOptRequestList.Clear();
		g_cCuiDataMgr.m_cPuBatOptRequestList.Clear();
	}
}

void CCuBatOptCheckTask::InitStateMachine()
{
    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CSipTask::PFProcMsg)&CCuBatOptCheckTask::ServiceProcMsg;
    cServiceProc.TimerPoll = (CSipTask::PFTimerPoll)&CCuBatOptCheckTask::ServiceTimerPoll;
    cServiceProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuBatOptCheckTask::ServiceProcMsg;
    AddRuleProc(SERVICE_STATE, cServiceProc);

    NextState(SERVICE_STATE);
}

u32 CCuBatOptCheckTask::ServiceProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "正常不会收到[OSP_SIP_MSG_PROC_FAIL]消息，不做处理\n");
			return PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
		}
		break;
	}
	// 不做业务处理
    return PROCMSG_OK;
}

u32 CCuBatOptCheckTask::ServiceTimerPoll()
{
	if (CheckBatOpt() == PROCMSG_DEL)
	{
		return TIMERPOLL_DEL;
	}

	NextState(SERVICE_STATE);
	return TIMERPOLL_DONE;
}

CuBatOptList &CCuBatOptCheckTask::GetCheckList()
{
	switch (m_enNextCheckList)
	{
	case EBAT_OPT_PLAT_LIST:
		{
			m_enNextCheckList = EBAT_OPT_PU_LIST;
			return g_cCuiDataMgr.m_cPlatBatOptRequestList;
		}
		break;
	case EBAT_OPT_PU_LIST:
	default:
		{
			m_enNextCheckList = EBAT_OPT_PLAT_LIST;
			return g_cCuiDataMgr.m_cPuBatOptRequestList;
		}
		break;
	}
}

u32 CCuBatOptCheckTask::CheckBatOpt()
{
	if ( g_cCuiDataMgr.m_cPlatBatOptRequestList.IsEmpty()
		&& g_cCuiDataMgr.m_cPuBatOptRequestList.IsEmpty() )
	{
		TASKLOG(NORMAL_TASK, PROGRESS_LEV, "已经没有批量任务需要执行，task[%u]退出!\n", this->GetTaskNO());
		return PROCMSG_DEL;
	}

	CuBatOptList &pcCuBatOptList = GetCheckList();
	if (pcCuBatOptList.IsEmpty())
	{
		return PROCMSG_OK;
	}

	TBatOptInfo batOptInfo;
	if (pcCuBatOptList.GetFrontBatOptInfo(batOptInfo))
	{
		if (batOptInfo.m_nTaskNo != INVALID_TASKNO)
		{
			if (this->GetInstance()->FindTask(batOptInfo.m_nTaskNo) == NULL)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "task[%u] 已经不存在了，批量操作[%s]任务没有被清除!", batOptInfo.m_nTaskNo, batOptInfo.m_strBatOptReq.c_str());
				pcCuBatOptList.PopFront();
			}
		}
		else
		{
			CBatOptTask *pTask = new CBatOptTask(this->GetInstance(), batOptInfo.m_strBatOptReq);
			if (pTask != NULL)
			{
                const u32 dwTaskNo = pTask->GetTaskNO();
				pcCuBatOptList.SetFrontBatOptTaskNo(batOptInfo.m_strBatOptReq, dwTaskNo );
				pTask->InitStateMachine();

				CCuBatOptReq cReq;
				cReq.FromXml(batOptInfo.m_strBatOptReq);
				u32 dwResult = SetCuReqMsg(cReq, pTask);

                //如果此时剔除第一个项目，可以短时间内创建多个batoptTask
                //否则就等待一个task处理完后才创建下一个
                //pcCuBatOptList.PopFront();
                TASKLOG(NORMAL_TASK, ERROR_LEV, "准备处理进行批量处理[%s]创建CBatOptTask， taskno[%u], 处理消息返回[%u]!\n",batOptInfo.m_strBatOptReq.c_str(), dwTaskNo, dwResult  );
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "准备处理进行批量处理[%s]创建CBatOptTask 失败，直接删除这个批量操作，避免业务死循环!\n",batOptInfo.m_strBatOptReq.c_str());
				pcCuBatOptList.PopFront();
			}
		}
	}

	return PROCMSG_OK;
}
