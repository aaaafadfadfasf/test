#include "sync_analyzer_p2plimit.h"

void ConstructSyncDataAnalyzeInfo(TSyncDataAnalyzeInfo &tData, ENSyncDataType enDataType, ENSyncOperateType enOperateType, 
    const string &strDomainId, ...);


const char* CSyncAnalyzerP2PLimit::GetStateName( u32 dwState ) const
{
    if (enStateInit == dwState)
    {
        return "enStateInit";
    }
    else if (enStateServerNonblock == dwState)
    {
        return "enStateServerNonblock";
    }
    else if (enStateServerBlock == dwState)
    {
        return "enStateServerBlock";
    }
    else
    {
        return "enStateUnknown";
    }
}

void CSyncAnalyzerP2PLimit::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerP2PLimit::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncAnalyzerP2PLimit::InitStateTimerPoll);
    AddRuleProc(enStateInit, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerP2PLimit::ServerNonblockStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncAnalyzerBase::PollSyncDataLst);
    AddRuleProc(enStateServerNonblock, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CSyncAnalyzerP2PLimit::ServerBlockStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSyncAnalyzerP2PLimit::ServerBlockStateTimerPoll);
    AddRuleProc(enStateServerBlock, cProc);

    QueryDomainAuthorizationDevIdList();
}

void CSyncAnalyzerP2PLimit::Analyze( const TSyncDataAnalyzeInfo &tData )
{
    do 
    {
        /* ͨ������Ϣ����ͬ������ */
        if (tData.domainId != CurrentDomainId())
        {
            break;
        }
        if (tData.dataType == enSyncDataUserDevice)
        {
            s8 szUserDomainId[UUID_STRING_LEN+1];
            sscanf(tData.dataId.c_str(), "%*s%*s%s", szUserDomainId);

            if (szUserDomainId != m_strDstDomainId)
            {
                break;
            }
        }

        /* ͨ���������͹���ͬ������ */
        if (tData.dataType == enSyncDataDeviceGroup || tData.dataType == enSyncDataDeviceGroup2Dev )
        {
            break;
        }

        /* ͨ���豸��Ź���ͬ������ */
        if (tData.dataType == enSyncDataDeviceCapIndexName || tData.dataType == enSyncDataDeviceInfo
            || tData.dataType == enSyncDataUserDevice)
        {
            s8 szDevId[DEVID_LEN+1];
            if (tData.dataType == enSyncDataDeviceInfo)
            {
                strcpy(szDevId, tData.dataId.c_str());
            }
            else
            {
                sscanf(tData.dataId.c_str(), "%s", szDevId);
            }

            if (m_cDomainAuthorizationDevIdSet.end() == m_cDomainAuthorizationDevIdSet.find(szDevId))
            {
                break;
            }
        }

        /* ����Ϣ���豸�ͺ���Ϣ������ͬ��,������ */

        if (enStateServerNonblock == GetState())
        {
            if (tData.dataType == enSyncDataUserDevice)
            {
                NextState(enStateServerBlock);
                QueryDomainAuthorizationDependence(&tData);
            }
            else
            {
                CSyncAnalyzerBase::Analyze(tData);
            }
        }
        else
        {
            // ������ʱ��������
            m_tTempSyncDataLst.push_back(tData);
        }
    } while (false);
}

u32 CSyncAnalyzerP2PLimit::InitStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcTransBase->GetEventId() == msgPrivateDevIdListGetRsp)
        {
            CDbMsgTransfer<TPrivateDevIdListGetRsp> *pcRsp = static_cast<CDbMsgTransfer<TPrivateDevIdListGetRsp> *>(pcTransBase);
            if (pcRsp->GetMsgObj().GetErrno() == 0)
            {
                m_cDomainAuthorizationDevIdSet = pcRsp->GetMsgObj().devIds;

                EnterServerNonblockState();
            }
            else
            {
                // ����ȴ���������
            }
        }
    }

    return PROCMSG_OK;
}

u32 CSyncAnalyzerP2PLimit::ServerNonblockStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcTransBase->GetEventId() == msgSyncDataRecordStoreRsp)
        {
            CSyncAnalyzerBase::DefaultProc(pcMsg);
        }
    }
    return PROCMSG_OK;
}

u32 CSyncAnalyzerP2PLimit::ServerBlockStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == msgDbMsgTransfer)
    {
        CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
        if (pcTransBase->GetEventId() == msgSyncDataRecordStoreRsp)
        {
            CSyncAnalyzerBase::DefaultProc(pcMsg);
        }
        else if (pcTransBase->GetEventId() == msgSyncDomainAuthorizationDependenceGetRsp)
        {
            // ͬ��������(��������Ϣ���豸�ͺţ��豸��Ϣ���豸������������)�Ϳ�����Ȩ��������
            CDbMsgTransfer<TSyncDomainAuthorizationDependenceGetRsp> *pcRsp = static_cast<CDbMsgTransfer<TSyncDomainAuthorizationDependenceGetRsp> *>(pcTransBase);
            if (pcRsp->GetMsgObj().GetErrno() == 0)
            {
                TSyncDomainAuthorizationDependenceGetRsp &cRsp = pcRsp->GetMsgObj();
                TSyncDataAnalyzeInfo tData;
                ConstructSyncDataAnalyzeInfo(tData, enSyncDataDomainInfo, enSyncOperateAdd, m_tQueringDomainAuthorizationData.domainId);
                CSyncAnalyzerBase::Analyze(tData);
                ConstructSyncDataAnalyzeInfo(tData, enSyncDataDeviceModel, enSyncOperateAdd, m_tQueringDomainAuthorizationData.domainId, &cRsp.devModelId);
                CSyncAnalyzerBase::Analyze(tData);
                ConstructSyncDataAnalyzeInfo(tData, enSyncDataDeviceInfo, enSyncOperateAdd, m_tQueringDomainAuthorizationData.domainId, &cRsp.devId);
                CSyncAnalyzerBase::Analyze(tData);
                for (u32 i = 0; i < cRsp.devCapIndexs.size(); ++i)
                {
                    ConstructSyncDataAnalyzeInfo(tData, enSyncDataDeviceCapIndexName, enSyncOperateMod, m_tQueringDomainAuthorizationData.domainId, 
                        &cRsp.devId, cRsp.devCapIndexs[i].id, cRsp.devCapIndexs[i].indexs[0]);
                    CSyncAnalyzerBase::Analyze(tData);
                }

                CSyncAnalyzerBase::Analyze(m_tQueringDomainAuthorizationData);

                EnterServerNonblockState();
            }
            else if (pcRsp->GetMsgObj().GetErrno() == ERR_TAS_INVALID_SYNCDATA)
            {
                // ��Чͬ������ʱֱ�Ӷ���
                EnterServerNonblockState();
            }
            else
            {
                // �ſ����أ������Զ�����
                m_bForbidQueryDomainAuthorizationDependence = false;
            }
        }
    }
    return PROCMSG_OK;
}

u32 CSyncAnalyzerP2PLimit::ServerBlockStateTimerPoll()
{
    QueryDomainAuthorizationDependence();
    return TIMERPOLL_DONE;
}

u32 CSyncAnalyzerP2PLimit::InitStateTimerPoll()
{
    // ���Բ�ѯ������Ȩ�豸����б�
    QueryDomainAuthorizationDevIdList();

    return TIMERPOLL_DONE;
}

void CSyncAnalyzerP2PLimit::EnterServerNonblockState()
{
    NextState(enStateServerNonblock);
    HandleTempSyncDataList();
}

void CSyncAnalyzerP2PLimit::QueryDomainAuthorizationDevIdList()
{
    if (!m_bForbidQueryDomainAuthorizationDevIdList)
    {
        CDbMsgTransfer<string> cReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDomainAuthorizationDevIdListGetReq);
        cReq.GetMsgObj() = m_strDstDomainId;
        m_bForbidQueryDomainAuthorizationDevIdList = PostDbMsgTransfer2DbTaskPool(cReq);
    }
}

void CSyncAnalyzerP2PLimit::QueryDomainAuthorizationDependence( const TSyncDataAnalyzeInfo *ptNewSyncData /*= NULL*/ )
{
    // �µķ���
    if (NULL != ptNewSyncData)
    {
        m_tQueringDomainAuthorizationData = *ptNewSyncData;
        m_bForbidQueryDomainAuthorizationDependence = false;
    }
    
    if (m_bForbidQueryDomainAuthorizationDependence)
    {
        return;
    }

    CDbMsgTransfer<string> cReq(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSyncDomainAuthorizationDependenceGetReq);
    s8 szDevId[DEVID_LEN+1] = {'\0'};
    sscanf(m_tQueringDomainAuthorizationData.dataId.c_str(), "%s", szDevId);
    cReq.GetMsgObj() = szDevId;
    m_bForbidQueryDomainAuthorizationDependence = PostDbMsgTransfer2DbTaskPool(cReq);
}

void CSyncAnalyzerP2PLimit::HandleTempSyncDataList()
{
    CMS_ASSERT(GetState() == enStateServerNonblock);

    while(!m_tTempSyncDataLst.empty())
    {
        TSyncDataAnalyzeInfo tData = m_tTempSyncDataLst.front();
        m_tTempSyncDataLst.pop_front();

        if (tData.dataType == enSyncDataUserDevice)
        {
            NextState(enStateServerBlock);
            QueryDomainAuthorizationDependence(&tData);
            break;
        }
        else
        {
            CSyncAnalyzerBase::Analyze(tData);
        }
    }
}
