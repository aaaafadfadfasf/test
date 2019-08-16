#include "userregtask.h"

CUserRegTask::CUserRegTask(CInstExt *pcInst) :CLoginSession(pcInst)
{

}
CUserRegTask::~CUserRegTask()
{

}

void CUserRegTask::InitStateMachine()
{
	CStateProc cWaitRegProc;
	cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CUserRegTask::OnWaitReg;
	cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CUserRegTask::OnWaitReg;
	cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CUserRegTask::OnWaitRegTimer;
	AddRuleProc(WaitReg, cWaitRegProc);

	CStateProc cServiceProc;
	cServiceProc.ProcMsg = (CTask::PFProcMsg)&CUserRegTask::OnService;
	cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CUserRegTask::OnService;
	cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CUserRegTask::OnServiceTimer;
	AddRuleProc(Service, cServiceProc);

	NextState(WaitReg);
}

u32 CUserRegTask::OnWaitReg(CMessage *const pcMsg)
{
	u32 dProcResult = PROCMSG_FAIL;
	switch (pcMsg->event)
	{
	case IM_REG_REQ:
		{
			int dwErrorCode = DEMO_SUCCESS;

			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == nullptr)
			{
				TASKLOG(DEMO_SERVER, ERROR_LEV, "OSP SIP ÏûÏ¢Îª¿Õ!\n");
				dwErrorCode = ERR_SIP_BODY_EMPTY;
				dProcResult = PROCMSG_DEL;
				return dProcResult;
			}

			CDemoMsgReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			CDemoMsgRsp cRsp;
			cRsp.SetErrorCode(dwErrorCode);
			cRsp.SetSeqNum(cReq.GetSeqNum);
			cRsp.SetSession(GetDevUri());

			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			if (dwErrorCode == DEMO_SUCCESS)
			{
				TASKLOG(DEMO_SERVER, CRITICAL_LEV, "%s×¢²á³É¹¦!\n", GetDevUri().c_str());
				dProcResult = PROCMSG_OK;
			}
			else
			{
				TASKLOG(DEMO_SERVER, ERROR_LEV, "%s×¢²áÊ§°Ü!\n", GetDevUri().c_str());
				dProcResult = PROCMSG_DEL;
			}
		}
		break;

	default:
		TASKLOG(DEMO_SERVER, WARNING_LEV, "Recv unkown msg [%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		break;
	}
}