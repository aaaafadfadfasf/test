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
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]�յ�������[%s]��Ӧ����Ϣ[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "���������յ�[OSP_SIP_MSG_PROC_FAIL]��Ϣ����������\n");
			return PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
		}
		break;
	}
	// ����ҵ����
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
		TASKLOG(NORMAL_TASK, PROGRESS_LEV, "�Ѿ�û������������Ҫִ�У�task[%u]�˳�!\n", this->GetTaskNO());
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
				TASKLOG(NORMAL_TASK, EVENT_LEV, "task[%u] �Ѿ��������ˣ���������[%s]����û�б����!", batOptInfo.m_nTaskNo, batOptInfo.m_strBatOptReq.c_str());
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

                //�����ʱ�޳���һ����Ŀ�����Զ�ʱ���ڴ������batoptTask
                //����͵ȴ�һ��task�������Ŵ�����һ��
                //pcCuBatOptList.PopFront();
                TASKLOG(NORMAL_TASK, ERROR_LEV, "׼�����������������[%s]����CBatOptTask�� taskno[%u], ������Ϣ����[%u]!\n",batOptInfo.m_strBatOptReq.c_str(), dwTaskNo, dwResult  );
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "׼�����������������[%s]����CBatOptTask ʧ�ܣ�ֱ��ɾ�������������������ҵ����ѭ��!\n",batOptInfo.m_strBatOptReq.c_str());
				pcCuBatOptList.PopFront();
			}
		}
	}

	return PROCMSG_OK;
}
