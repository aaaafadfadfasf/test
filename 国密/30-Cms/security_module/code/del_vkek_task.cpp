#include "del_vkek_task.h"
#include "dbixface.h"

CDelVkekTask *g_pcDelVkekTask;


CDelVkekTask::CDelVkekTask(CInstExt *pcInst) : CSipTask(pcInst)
{
	
}

CDelVkekTask::~CDelVkekTask()
{

}

void CDelVkekTask::InitStateMachine()
{
	CStateProc cState;
	cState.ProcMsg = (CSipTask::PFProcMsg)&CDelVkekTask::ProcServerMsg;
	cState.ProcErrMsg = (CSipTask::PFProcMsg)&CDelVkekTask::ProcServerMsg;
	cState.TimerPoll = (CSipTask::PFTimerPoll)&CDelVkekTask::ServerTimerPoll;
	AddRuleProc(SERVER, cState);

	NextState(SERVER);
}

u32 CDelVkekTask::ProcServerMsg(CMessage *const pcMsg)
{
	NextState(SERVER);
	if (DBIX_RESULT_CB == pcMsg->event)
	{
		CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;

		if (pDbMsg->GetDBResult() != enDBResultSuccess)
		{
			TASKLOG(ERROR_LEV, "É¾³ý±í[%s]Ê§°Ü!\n", VKEK_INFO_TABLE);
		}	
	}
	return PROCMSG_OK;
}

u32 CDelVkekTask::ServerTimerPoll()
{
	if (GetCurStateHoldTime() >= 10)
	{
		//Ë¢ÐÂ×´Ì¬
		NextState(SERVER);

		int year, month, day, hour, minute, sec;
		long timeZone;
		CCMSTime cNowTime;
		cNowTime.GetLocalTime(year, month, day, hour, minute, sec, timeZone);
		if (hour ==0 && minute >= 0 && minute < 59)
		{
			string strTmp;
			CDBEntity cEntity(g_cVkekInfoTable);

			u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
			cEntity.SetCBParm(lwDbCb);

			CDBCriteria Condition;
			Condition = lt(VKIT_TMFROM, CCMSTime(cNowTime - g_cConf.GetDelVkekInterval() * 24 * 60 * 60).ToString());

			cEntity.SetDelFlag(&Condition);
			OperateDBEntity(cEntity);

		}
			
	}

	return TIMERPOLL_DONE;
}




