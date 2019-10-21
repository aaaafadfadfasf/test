#include "tas_conn_task_poweron.h"
#include "cctrl.h"
#include "tasks/parent_tas_conn_task.h"
#include "tasks/p2p_tas_conn_task.h"

CTasConnTaskPowerOn::CTasConnTaskPowerOn( CInstExt *pcInst ): CTask(pcInst)
{

}

const char* CTasConnTaskPowerOn::GetStateName( u32 dwState ) const
{
    if (enStateQueryDstDomainListing == dwState)
    {
        return "QueryDstDomainListingState";
    }
    else if (enStateQueryDstDomainListTry == dwState)
    {
        return "QueryDstDomainListTryState";
    }
    else
    {
        return "unknownState";
    }
}

void CTasConnTaskPowerOn::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConnTaskPowerOn::QueryDstDomainListingStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(enStateQueryDstDomainListing, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConnTaskPowerOn::QueryDstDomainListTryStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CTasConnTaskPowerOn::QueryDstDomainListTryStateTimerPoll;
    AddRuleProc(enStateQueryDstDomainListTry, cProc);

    EnterQueryDstDomainListingState();
}

u32 CTasConnTaskPowerOn::QueryDstDomainListingStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event != msgDbMsgTransfer) // 只处理此消息
    {
        return PROCMSG_FAIL;
    }

    CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
    if (pcTransBase->GetEventId() != msgSyncDstDomainListGetRsp)
    {
        return PROCMSG_FAIL;
    }

    CDbMsgTransfer<TSyncDstDomainListGetRsp> *pcSyncDstDomainList = static_cast<CDbMsgTransfer<TSyncDstDomainListGetRsp> *>(pcTransBase);
    if (pcSyncDstDomainList->GetMsgObj().GetErrno() == 0)
    {
        vector<TSyncDstDomainInfo> cSyncDstDomains = pcSyncDstDomainList->GetMsgObj().dstDomainList;

        for (u32 i = 0; i < cSyncDstDomains.size(); ++i)
        {
            if (cSyncDstDomains[i].syncType == enSyncTypeParent)
            {
                CParentTasConnTask *pcTask = new CParentTasConnTask(GetInstance(), 
                                                 cSyncDstDomains[i].domainId, cSyncDstDomains[i].domainName);
                pcTask->InitStateMachine();
            }
            else
            {
                CP2PTasConnTask *pcTask = new CP2PTasConnTask(GetInstance(), cSyncDstDomains[i].domainId,
                                                  cSyncDstDomains[i].domainName, cSyncDstDomains[i].syncType);
                pcTask->InitStateMachine();
            }
        }

        return PROCMSG_DEL;
    }
    else
    {
        NextState(enStateQueryDstDomainListTry);
        return PROCMSG_OK;
    }
}

u32 CTasConnTaskPowerOn::QueryDstDomainListTryStateProcMsg( CMessage *const pcMsg )
{
    // 什么都不做
    return PROCMSG_OK;
}

u32 CTasConnTaskPowerOn::QueryDstDomainListTryStateTimerPoll()
{
    if (GetCurStateHoldTime() > 1)
    {
        EnterQueryDstDomainListingState();
    }

    return TIMERPOLL_DONE;
}

void CTasConnTaskPowerOn::EnterQueryDstDomainListingState()
{
    NextState(enStateQueryDstDomainListing);

    CDbMsgTransfer<void> cSyncDstDomainListGet(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDstDomainListGetReq);
    PostDbMsgTransfer2DbTaskPool(cSyncDstDomainListGet);
}
