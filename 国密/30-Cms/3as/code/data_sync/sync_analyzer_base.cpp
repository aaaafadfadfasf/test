#include "sync_analyzer_base.h"

u32 CSyncAnalyzerBase::WaitingSyncDataRecordNumber()
{
    return static_cast<u32>(m_cSyncData.size());
}

void CSyncAnalyzerBase::Analyze( const TSyncDataAnalyzeInfo &tData )
{
    m_cSyncData.push_back(TSyncDataRecordStoreReq(m_strDstDomainId, m_enSyncType, tData.dataType, tData.dataId, tData.operateType, tData.domainId));

    SendStoreReq();
}

u32 CSyncAnalyzerBase::DefaultProc( CMessage *const pcMsg )
{
    if (msgDbMsgTransfer == pcMsg->event)
    {
        CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));

        if (msgSyncDataRecordStoreRsp == pcTransBase->GetEventId())
        {
            m_bForbidSendStoreReq = false;

            TSyncDataRecordStoreRsp* pcRsp = static_cast<TSyncDataRecordStoreRsp*>(pcTransBase->GetMsgObjAddr());

            if (!m_cSyncData.empty())
            {
                if (pcRsp->GetErrno() == 0)
                {
                    m_cSyncData.pop_front();
                    SendStoreReq();
                }
                else
                {
                    // 出错时等待定期重发
                }
            }
        }
    }

    return PROCMSG_OK;
}

void CSyncAnalyzerBase::SendStoreReq()
{
    if (!m_bForbidSendStoreReq && !m_cSyncData.empty())
    {
        CDbMsgTransfer<TSyncDataRecordStoreReq> tSyncDataRecordStoreReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDataRecordStoreReq);
        tSyncDataRecordStoreReq.GetMsgObj() = m_cSyncData.front();

        m_bForbidSendStoreReq = PostDbMsgTransfer2DbTaskPool(tSyncDataRecordStoreReq);
    }
}

u32 CSyncAnalyzerBase::PollSyncDataLst()
{
    SendStoreReq();

    return TIMERPOLL_DONE;
}

