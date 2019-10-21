#include "syncdata_send.h"


const char* CSyncDataSend::GetStateName( u32 dwState ) const
{
    if (enStateQueryData == dwState)
    {
        return "enStateQueryData";
    }
    else if (enStateTransferData == dwState)
    {
        return "enStateTransferData";
    }
    else if (enStateDeleteRecord == dwState)
    {
        return "enStateDeleteRecord";
    }
    else
    {
        return "enStateUnknown";
    }
}

void CSyncDataSend::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncDataSend::QueryDataStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncDataSend::QueryDataStateTimerPoll);
    AddRuleProc(enStateQueryData, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncDataSend::TransferDataStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncDataSend::TransferDataStateTimerPoll);
    AddRuleProc(enStateTransferData, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncDataSend::DeleteRecordStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncDataSend::DeleteRecordStateTimerPoll);
    AddRuleProc(enStateDeleteRecord, cProc);

    EnterQueryDataState();
}

u32 CSyncDataSend::QueryDataStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcBase->GetEventId() == msgSyncDataConstructRsp)
        {
            m_bForbidQueryData = false;

            CDbMsgTransfer<TSyncDataConstructRsp> *pcRsp = static_cast<CDbMsgTransfer<TSyncDataConstructRsp>* >(pcBase);

            if (pcRsp->GetMsgObj().GetErrno() == CMS_SUCCESS)
            {
                m_tSyncDataInfos = pcRsp->GetMsgObj().results;

                if (m_tSyncDataInfos.size() > 0)
                {
                    EnterTransferDataState();
                }
            }
            else
            {
                // 失败等待后续自动重试
            }
        }
    }

    // hzytodo 处理应答超时问题, 考虑对端的处理能力 KDSIP_RESPONSE_408_REQ_TIMEOUT

    return PROCMSG_OK;
}

u32 CSyncDataSend::QueryDataStateTimerPoll()
{
    QueryData();

    return TIMERPOLL_DONE;
}

u32 CSyncDataSend::TransferDataStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == TAS_SYNC_TAS_DATA_RSP)
    {
        m_bForbidSendSyncData = false;

        COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
        CSyncTasDataRsp cRsp;
        pcSipMsg->GetMsgBody(cRsp);

        if (cRsp.GetErrorCode() == CMS_SUCCESS)
        {
            EnterDeleteRecordState();
        }
        else
        {
            // 失败等待后续自动重试
        }
    }

    if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
    {
        // 出错允许重发
        m_bForbidSendSyncData = false;
    }

    return PROCMSG_OK;
}

u32 CSyncDataSend::TransferDataStateTimerPoll()
{
    if (m_bForbidSendSyncData && GetCurStateHoldTime() > 60*5)
    {
        m_bForbidSendSyncData = false;
    }

    TransferData();
    return TIMERPOLL_DONE;
}


u32 CSyncDataSend::DeleteRecordStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcBase->GetEventId() == msgSyncDataRecordDelRsp)
        {
            m_bForbidDeleteRecord = false;

            CDbMsgTransfer<TSyncDataRecordDelRsp> *pcRsp = static_cast<CDbMsgTransfer<TSyncDataRecordDelRsp>* >(pcBase);
            if (pcRsp->GetMsgObj().GetErrno() == CMS_SUCCESS)
            {
                EnterQueryDataState();
            }
            else
            {
                // 等待自动重试
            }
        }
    }

    return PROCMSG_OK;
}

u32 CSyncDataSend::DeleteRecordStateTimerPoll()
{
    DeleteRecord();
    return TIMERPOLL_DONE;
}

void CSyncDataSend::QueryData()
{
    if (!m_bForbidQueryData)
    {
        CDbMsgTransfer<TSyncDataConstructReq> cReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDataConstructReq);
        cReq.GetMsgObj().dstDomainId = m_strDstDomainId;
        cReq.GetMsgObj().dwExpectedDataNum = 10;
        cReq.GetMsgObj().enSyncType = m_enSyncType;

        m_bForbidQueryData = PostDbMsgTransfer2DbTaskPool(cReq);
    }
}

void CSyncDataSend::TransferData()
{
    if (!m_bForbidSendSyncData)
    {
        CSyncTasDataReq cReq;
        cReq.SetSession(m_strSessionId);
        cReq.SetSyncDataInfo(m_tSyncDataInfos);

        m_bForbidSendSyncData = (0 == PostMsgReq(cReq, m_tPeerUri));
    }
}

void CSyncDataSend::DeleteRecord()
{
    if (!m_bForbidDeleteRecord)
    {
        CDbMsgTransfer<TSyncDataRecordDelReq> cReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDataRecordDelReq);

        cReq.GetMsgObj().dstDomainId = m_strDstDomainId;
        cReq.GetMsgObj().enSyncType = m_enSyncType;
        cReq.GetMsgObj().sns.clear();
        for (u32 i = 0; i < m_tSyncDataInfos.size(); ++i)
        {
            cReq.GetMsgObj().sns.push_back(m_tSyncDataInfos[i].sn);
        }

        m_bForbidDeleteRecord = PostDbMsgTransfer2DbTaskPool(cReq);
    }
}

void CSyncDataSend::EnterQueryDataState()
{
    NextState(enStateQueryData);
    m_bForbidQueryData = false;
    QueryData();
}

void CSyncDataSend::EnterTransferDataState()
{
    NextState(enStateTransferData);
    m_bForbidSendSyncData = false;
    TransferData();
}

void CSyncDataSend::EnterDeleteRecordState()
{
    NextState(enStateDeleteRecord);
    m_bForbidDeleteRecord = false;
    DeleteRecord();
}
