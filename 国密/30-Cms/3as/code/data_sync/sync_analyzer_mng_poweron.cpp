#include "sync_analyzer_mng_poweron.h"
#include "../sync_analyzer_mng.h"
#include "sync_analyzer_parent.h"
#include "sync_analyzer_p2plimit.h"

CSyncAnalyzerMngPoweron::CSyncAnalyzerMngPoweron(CInstExt *pcInst): CTask(pcInst)
{
    m_enPrivateDevIdListState = enDataStateInit;
    m_enSubdomainIdListState = enDataStateInit;
    m_enSyncDstDomainListState = enDataStateInit;
}

CSyncAnalyzerMngPoweron::~CSyncAnalyzerMngPoweron()
{

}

void CSyncAnalyzerMngPoweron::InitStateMachine()
{
    NextState(enStateServer);

    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerMngPoweron::ServerStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CSyncAnalyzerMngPoweron::ServerTimerPoll;
    AddRuleProc(enStateServer, cProc);

    QueryPrivateDevIdList();
    QuerySubdomainIdList();
    QuerySyncDstDomainList();
}

u32 CSyncAnalyzerMngPoweron::ServerStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event != msgDbMsgTransfer) // 只处理此消息
    {
        return PROCMSG_FAIL;
    }

    CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
    CSyncAnalyzerAppData& cAppData = static_cast<CSyncAnalyzerInst*>(GetInstance())->GetAppData();

    if (pcTransBase->GetEventId() == msgPrivateDevIdListGetRsp)
    {
        CDbMsgTransfer<TPrivateDevIdListGetRsp> *pcPrivateDevIdList = static_cast<CDbMsgTransfer<TPrivateDevIdListGetRsp> *>(pcTransBase);
        if (pcPrivateDevIdList->GetMsgObj().GetErrno() == 0)
        {
            cAppData.PrivateDevIds() = pcPrivateDevIdList->GetMsgObj().devIds;
            m_enPrivateDevIdListState = enDataStateReady;
        }
        else
        {
            m_enPrivateDevIdListState = enDataStateFailed;
        }
    }
    else if (pcTransBase->GetEventId() == msgDomainRelationListGetRsp)
    {
        CDbMsgTransfer<TDomainRelationListGetRsp> *pcSubdomainIdList = static_cast<CDbMsgTransfer<TDomainRelationListGetRsp> *>(pcTransBase);
        if (pcSubdomainIdList->GetMsgObj().GetErrno() == 0)
        {
            vector<CDomainRelationInfo> cSubDomainIds;
            vector<CDomainRelationInfo> &cDomainRelationList = pcSubdomainIdList->GetMsgObj().domainRelationList;
            FindSubdomainList(cDomainRelationList, CurrentDomainId(), cSubDomainIds);
            cAppData.SubDomainIdInit(cSubDomainIds);

            m_enSubdomainIdListState = enDataStateReady;
        }
        else
        {
            m_enSubdomainIdListState = enDataStateFailed;
        }
    }
    else if (pcTransBase->GetEventId() == msgSyncDstDomainListGetRsp)
    {
        CDbMsgTransfer<TSyncDstDomainListGetRsp> *pcSyncDstDomainList = static_cast<CDbMsgTransfer<TSyncDstDomainListGetRsp> *>(pcTransBase);
        if (pcSyncDstDomainList->GetMsgObj().GetErrno() == 0)
        {
            m_cSyncDstDomainList = pcSyncDstDomainList->GetMsgObj().dstDomainList;
            m_enSyncDstDomainListState = enDataStateReady;
        }
        else
        {
            m_enSyncDstDomainListState = enDataStateFailed;
        }
    }
    else
    {
        CMS_ASSERT(false && "unhandled msg");
    }

    if (IsAllReady())
    {
        // 全部就绪时创建同步分析器再删除任务
        CreateSyncAnalyzers();
        return PROCMSG_DEL;
    }
    else
    {
        return PROCMSG_OK;
    }
}

u32 CSyncAnalyzerMngPoweron::ServerTimerPoll()
{
    QueryPrivateDevIdList();
    QuerySubdomainIdList();
    QuerySyncDstDomainList();

    return TIMERPOLL_DONE;
}

void CSyncAnalyzerMngPoweron::QueryPrivateDevIdList()
{
    if (m_enPrivateDevIdListState == enDataStateInit || m_enPrivateDevIdListState == enDataStateFailed)
    {
        CDbMsgTransfer<void> cPrivateDevListGet(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgPrivateDevIdListGetReq);
        PostDbMsgTransfer2DbTaskPool(cPrivateDevListGet);

        m_enPrivateDevIdListState = enDataStateWaiting;
    }
}

void CSyncAnalyzerMngPoweron::QuerySubdomainIdList()
{
    if (m_enSubdomainIdListState == enDataStateInit || m_enSubdomainIdListState == enDataStateFailed)
    {
        CDbMsgTransfer<void> cSubDomainListGet(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDomainRelationListGetReq);
        PostDbMsgTransfer2DbTaskPool(cSubDomainListGet);

        m_enSubdomainIdListState = enDataStateWaiting;
    }
}

void CSyncAnalyzerMngPoweron::QuerySyncDstDomainList()
{
    if (m_enSyncDstDomainListState == enDataStateInit || m_enSyncDstDomainListState == enDataStateFailed)
    {
        CDbMsgTransfer<void> cSyncDstDomainListGet(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDstDomainListGetReq);
        PostDbMsgTransfer2DbTaskPool(cSyncDstDomainListGet);

        m_enSyncDstDomainListState = enDataStateWaiting;
    }
}

bool CSyncAnalyzerMngPoweron::IsAllReady()
{
    bool bRet = (m_enPrivateDevIdListState == enDataStateReady) && (m_enSubdomainIdListState == enDataStateReady)
        && (m_enSyncDstDomainListState == enDataStateReady);

    return bRet;
}

void CSyncAnalyzerMngPoweron::CreateSyncAnalyzers()
{
    CSyncAnalyzerAppData& cAppData = static_cast<CSyncAnalyzerInst*>(GetInstance())->GetAppData();

    for (u32 i = 0; i < m_cSyncDstDomainList.size(); ++i)
    {
        if (m_cSyncDstDomainList[i].syncType == enSyncTypeParent)
        {
            CSyncAnalyzerParent *pcAnalyzer = new CSyncAnalyzerParent(GetInstance(), m_cSyncDstDomainList[i].domainId);
            pcAnalyzer->InitStateMachine();

            cAppData.SyncAnalyzerParent() = pcAnalyzer;
        }
        else if (m_cSyncDstDomainList[i].syncType == enSyncTypeP2PLimit)
        {
            CSyncAnalyzerP2PLimit *pcAnalyzer = new CSyncAnalyzerP2PLimit(GetInstance(), m_cSyncDstDomainList[i].domainId);
            pcAnalyzer->InitStateMachine();

            cAppData.SyncAnalyzerP2PLimit().push_back(pcAnalyzer);
        }
        else if (m_cSyncDstDomainList[i].syncType == enSyncTypeP2PNoLimit)
        {
            // hzytodo2 创建全联通分析器
        }
        else
        {
            CMS_ASSERT(false);
        }
    }
}

