#include "cmemorymngtask.h"
#include "cmudbg.h"

CMemoryMngTask::CMemoryMngTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}

CMemoryMngTask::~CMemoryMngTask()
{

}

void CMemoryMngTask::InitStateMachine()
{
	CStateProc cWaitRegProc;

	CStateProc cServiceProc;
	cServiceProc.ProcMsg = (CTask::PFProcMsg)&CMemoryMngTask::OnService;
	cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CMemoryMngTask::OnService;
	cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CMemoryMngTask::OnServiceTimer;
	AddRuleProc(Service, cServiceProc);

	NextState(Service);
}

u32 CMemoryMngTask::OnService(CMessage *const pcMsg)
{
	return PROCMSG_OK;
}
u32 CMemoryMngTask::OnServiceTimer()
{	
	int nElapse = GetCurStateHoldTime();
	int nInterval = g_cCmuConf.GetTcmallocReleaseInterval();
	if (nInterval > 0 && (nElapse%nInterval) == 0)		
	{
		TASKLOG(EVENT_LEV, "elapse=%u, call ReleaseFreeMemory\n", nElapse);
		ReleaseFreeMemory();
	}
	return TIMERPOLL_DONE;
}
