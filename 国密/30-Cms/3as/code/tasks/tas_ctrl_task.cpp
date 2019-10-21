#include "tas_ctrl_task.h"

CTasCtrlTask::CTasCtrlTask( CInstExt *pcInst ): CSipTask(pcInst)
{

}

CTasCtrlTask::~CTasCtrlTask()
{

}

void CTasCtrlTask::InitStateMachine()
{
    NextState(enStateServer);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasCtrlTask::OnServer;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateServer, cProc);
}

u32 CTasCtrlTask::OnServer( CMessage *const pcMsg )
{
    // hzytodo2 暂时回复个假的
    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
    CCtrlTasReq cReq;
    pcSipMsg->GetMsgBody(cReq);
    CCtrlTasRsp cRsp;
    cRsp.SetSession(cReq.GetSession());
    cRsp.SetSeqNum(cReq.GetSeqNum());

    PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);
    return PROCMSG_DEL;
}
