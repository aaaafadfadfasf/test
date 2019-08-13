
#include "cms/ospext/ospinst.h"
#include "flowctrltask.h"

extern bool g_bDebugOn;

CFlowCtrlMngTask::CFlowCtrlMngTask(CInstExt *pcInst) : CTask(pcInst)
{
	m_dwTop   = 0;
}

void CFlowCtrlMngTask::InitStateMachine()
{
	CStateProc cProc;
	cProc.ProcMsg = (CTask::PFProcMsg)&CFlowCtrlMngTask::OnWorking;
	cProc.ProcErrMsg = (CTask::PFProcMsg)&CFlowCtrlMngTask::OnWorking;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CFlowCtrlMngTask::OnWorkingTimer;
	AddRuleProc(Working, cProc);

	NextState(Working);
}

const char *CFlowCtrlMngTask::GetStateName(u32 dwState) const
{
	switch (dwState)
	{
	case Working:
		return "Working";
	default:
		break;
	}
	return "Unknown State";
}

void CFlowCtrlMngTask::PrintData() const
{
	CTask::PrintData();

	OspPrintf(TRUE, FALSE, "\n  Top[%u], Parallel[%u], Rest[%u], Drop[%u]\n",
		m_dwTop, m_dwMaxParallelNum, m_cTaskQueue.size(), m_dwDrop);

	if (g_bDebugOn)
	{
		for (map<u32, u16>::const_iterator it = m_cProcessing.begin(); m_cProcessing.end() != it; ++it)
		{
			OspPrintf(TRUE, FALSE, "  TaskNO[%u], TimerPoll[%d]\n", it->first, it->second);
		}
	}
}

void CFlowCtrlMngTask::RegistTask(u32 dwTaskNO)
{
	m_cTaskQueue.push(dwTaskNO);

	// ͳ��������Ϣ
	m_dwTop = m_dwTop < m_cTaskQueue.size() ? m_cTaskQueue.size() : m_dwTop;

	// ��ʼ����
	Begin();
}

void CFlowCtrlMngTask::EndTask(u32 dwTaskNO)
{
	map<u32, u16>::iterator it = m_cProcessing.find(dwTaskNO);
	if (m_cProcessing.end() != it)
	{
		m_cProcessing.erase(it);

		// ����һ������󣬼���������������
		Begin();
	}
}

void CFlowCtrlMngTask::Begin()
{
	u32 dwProcessing = m_cProcessing.size();			// ��ǰ���ڴ������������
	while (!m_cTaskQueue.empty() && dwProcessing < m_dwMaxParallelNum)
	{
		u32 dwFrontTask = m_cTaskQueue.front();
		CFlowCtrlTask *pcTask = dynamic_cast<CFlowCtrlTask *>(GetInstance()->FindTask(dwFrontTask));
		if (NULL != pcTask)
		{
			if (pcTask->StartWork())
			{
				++dwProcessing;
				m_cProcessing.insert(map<u32, u16>::value_type(dwFrontTask, 0));
			}
		}
		else			// pcTaskΪNULL�����ܸ�Task�Ѿ������ȵ����������
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "������ڻ���ע�������CFlowCtrlTask������֮��TaskNO[%u]����ǰ���ڴ���������[%u]\n", 
				dwFrontTask, m_cProcessing.size());
		}

		m_cTaskQueue.pop();
	}
}

u32 CFlowCtrlMngTask::OnWorkingTimer()
{
	for (map<u32, u16>::iterator it = m_cProcessing.begin(); m_cProcessing.end() != it;)
	{
		if (++(it->second) >= CMU_REQ_RSP_TASK_TIMEOUT)			// ������ʱ
		{
			m_cProcessing.erase(it++);

            ++m_dwDrop;
		}
		else
		{
			++it;
		}
	}

	// ɾ���˳�ʱ������󣬼��������������񣬱������Ĳ�����
	Begin();

	return TIMERPOLL_DONE;
}


