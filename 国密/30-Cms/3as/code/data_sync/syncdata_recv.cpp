#include "syncdata_recv.h"

const char* CSyncDataRecv::GetStateName( u32 dwState ) const
{
    if (enStateRecvData == dwState)
    {
        return "enStateRecvData";
    }
    else if (enStateStoreData == dwState)
    {
        return "enStateStoreData";
    }
    else
    {
        return "enStateUnknown";
    }
}

void CSyncDataRecv::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncDataRecv::RecvDataStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncDataRecv::RecvDataStateTimerPoll);
    AddRuleProc(enStateRecvData, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncDataRecv::StoreDataStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncDataRecv::StoreDataStateTimerPoll);
    AddRuleProc(enStateStoreData, cProc);

    NextState(enStateRecvData);
}

u32 CSyncDataRecv::RecvDataStateProcMsg( CMessage *const pcMsg )
{
    if (TAS_SYNC_TAS_DATA_REQ == pcMsg->event)
    {
        COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
        CSyncTasDataReq cReq;
        pcSipMsg->GetMsgBody(cReq);

        m_dwReqSeq = cReq.GetSeqNum();
        m_strReqSessId = cReq.GetSession();
        m_tReqTransId = pcSipMsg->GetSipTransID();

        CDbMsgTransfer<TSyncDataStoreReq> cStoreReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDataStoreReq);
        cStoreReq.GetMsgObj().data = cReq.GetSyncDataInfo();
        cStoreReq.GetMsgObj().enSyncType = m_enSyncType;

        if (PostDbMsgTransfer2DbTaskPool(cStoreReq))
        {
            NextState(enStateStoreData);
        }
    }

    return PROCMSG_OK;
}

u32 CSyncDataRecv::RecvDataStateTimerPoll()
{
    return TIMERPOLL_DONE;
}

u32 CSyncDataRecv::StoreDataStateProcMsg( CMessage *const pcMsg )
{
    if (msgDbMsgTransfer == pcMsg->event)
    {
        CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcBase->GetEventId() == msgSyncDataStoreRsp)
        {
            CDbMsgTransfer<TSyncDataStoreRsp> *pcStoreRsp = static_cast<CDbMsgTransfer<TSyncDataStoreRsp>* >(pcBase);
            
            CSyncTasDataRsp cRsp;
            cRsp.SetSeqNum(m_dwReqSeq);
            cRsp.SetSession(m_strReqSessId);
            cRsp.SetErrorCode(pcStoreRsp->GetMsgObj().GetErrno());
            PostMsgRsp(m_tReqTransId, cRsp);

            NextState(enStateRecvData);
        }
    }

    return PROCMSG_OK;
}

u32 CSyncDataRecv::StoreDataStateTimerPoll()
{
    return TIMERPOLL_DONE;
}
