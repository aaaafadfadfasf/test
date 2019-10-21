#include "sync_analyzer_mng.h"
#include "data_sync/sync_analyzer_mng_poweron.h"
#include "data_sync/sync_analyzer_parent.h"

CSyncAnalyzerApp g_cSyncAnalyzerApp;

void SyncAnalyse( ENSyncDataType enDataType, ENSyncOperateType enOperateType, const string &strDataDomainId, ... )
{
    // 要求提供域号信息是因为删除时信息已经删除如果不及时提供域号信息后续将无法获取

    va_list parmList;
    
    /* 通过设备类型过滤 */
    if (strDataDomainId == CurrentDomainId())
    {
        u32 dwDevType = DEVICE_TYPE_ENCODER;

        va_start(parmList, strDataDomainId);
        switch(enDataType)
        {
        case enSyncDataDeviceModel:
            {
                va_arg(parmList, const string*);
                dwDevType = va_arg(parmList, u32);
            }
            break;
        case enSyncDataDeviceInfo:
            {
                va_arg(parmList, const string*);
                dwDevType = va_arg(parmList, u32);
            }
            break;
        case enSyncDataDeviceCapIndexName:
            {
                va_arg(parmList, const string*);
                va_arg(parmList, u32);
                va_arg(parmList, u32);
                dwDevType = va_arg(parmList, u32);
            }
            break;
        case enSyncDataDeviceGroup2Dev:
            {
                va_arg(parmList, const string*);
                va_arg(parmList, u32);
                va_arg(parmList, u32);
                va_arg(parmList, u32);
                dwDevType = va_arg(parmList, u32);
            }
            break;
		case enSyncDataDevGrpDevSeq:
			{
			    va_arg(parmList, const string*);
				va_arg(parmList, u32);
				va_arg(parmList, u32);
				va_arg(parmList, double);
				dwDevType = va_arg(parmList, u32);
			}
			break;
        default:
            break;
        }
        va_end(parmList);

        if (DEVICE_TYPE_ENCODER != dwDevType)
        {
            return;
        }
    }

    TSyncDataAnalyzeInfo *ptData = new TSyncDataAnalyzeInfo();
    va_start(parmList, strDataDomainId);
    ConstructSyncDataAnalyzeInfo(*ptData, enDataType, enOperateType, strDataDomainId, parmList);
    va_end(parmList);

    ptrint ptr = reinterpret_cast<ptrint>(ptData);
    if (0 != OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncAnalyzeCmd, &ptr, sizeof(ptr)))
    {
        delete ptData;
        GLOBELOG(enLogSrcSyncAnalyzer, ERROR_LEV, "SyncAnalyse post msg failed\n");
    }
}

void ConstructSyncDataAnalyzeInfo( TSyncDataAnalyzeInfo &tData, ENSyncDataType enDataType, ENSyncOperateType enOperateType, const string &strDomainId, va_list parmList )
{
    s8 szDataId[4000];
    switch(enDataType)
    {
    case enSyncDataDomainInfo:
        {
            sprintf(szDataId, "%s", strDomainId.c_str());
        }
        break;
    case enSyncDataDeviceModel:
        {
            const string *pstrModelId = va_arg(parmList, const string*);
            sprintf(szDataId, "%s", pstrModelId->c_str());
        }
        break;
    case enSyncDataDeviceInfo:
        {
            const string *pstrDevId = va_arg(parmList, const string*);
            u32 dwDevTypeId = va_arg(parmList, u32);
            u32 dwPrivatePropertyNew = va_arg(parmList, u32);
            u32 dwPrivatePropertyOld = va_arg(parmList, u32);
            sprintf(szDataId, "%s %u %u %u", pstrDevId->c_str(), dwDevTypeId, dwPrivatePropertyNew, dwPrivatePropertyOld);
        }
        break;
    case enSyncDataDeviceCapIndexName:
        {
            const string *pstrDevId = va_arg(parmList, const string*);
            u32 dwCapId = va_arg(parmList, u32);
            u32 dwCapIndex = va_arg(parmList, u32);
            sprintf(szDataId, "%s %u %u", pstrDevId->c_str(), dwCapId, dwCapIndex);
        }
        break;
    case enSyncDataDeviceGroup:
        {
            const string *pstrDevgrpId = va_arg(parmList, const string*);
            sprintf(szDataId, "%s", pstrDevgrpId->c_str());
        }
        break;
    case enSyncDataDeviceGroup2Dev:
        {
            const string *pstrDevgrpId = va_arg(parmList, const string*);
            const string *pstrDevId = va_arg(parmList, const string*);
            u32 dwCapId = va_arg(parmList, u32);
            u32 dwCapIndex = va_arg(parmList, u32);
            sprintf(szDataId, "%s %s %u %u", pstrDevgrpId->c_str(), pstrDevId->c_str(), dwCapId, dwCapIndex);
        }
        break;
	case enSyncDataDevGrpDevSeq:
		{
			const string *pstrDevGrpId = va_arg(parmList, const string*);
			const string *pstrDevId = va_arg(parmList, const string*);
			u32   dwCapIndex=va_arg(parmList, u32);
			double dDevSeql=va_arg(parmList,double);
			sprintf(szDataId, "%s %s %u %.14lf", pstrDevGrpId->c_str(), pstrDevId->c_str(), 
				dwCapIndex, dDevSeql);
		}
		break;
    case enSyncDataUserDevice:
        {
            const string *pstrDevId = va_arg(parmList, const string*);
            const string *pstrUserName = va_arg(parmList, const string*);
            const string *pstrDomainId = va_arg(parmList, const string*);
            sprintf(szDataId, "%s %s %s", pstrDevId->c_str(), pstrUserName->c_str(), pstrDomainId->c_str());
        }
        break;
    default:
        {
            GLOBELOG(enLogSrcSyncAnalyzer, ERROR_LEV, "ConstructSyncDataAnalyzeInfo detected unknow SyncDataType:%d\n", enDataType);
            return;
        }
        break;
    }

    tData.dataType = enDataType;
    tData.operateType = enOperateType;
    tData.domainId = strDomainId;
    tData.dataId = szDataId;
}

void ConstructSyncDataAnalyzeInfo( TSyncDataAnalyzeInfo &tData, ENSyncDataType enDataType, ENSyncOperateType enOperateType, const string &strDataDomainId, ... )
{
    va_list parmList;
    va_start(parmList, strDataDomainId);
    ConstructSyncDataAnalyzeInfo(tData, enDataType, enOperateType, strDataDomainId, parmList);
    va_end(parmList);
}

void CSyncAnalyzerInst::DaemonEntry( CMessage* const pcMsg )
{
    Log(enLogSrcSyncAnalyzer, EVENT_LEV, "CSyncAnalyzerInst::DaemonEntry recved message[%u-%s], curstate:%u\n", 
        pcMsg->event, OspExtEventDesc(pcMsg->event).c_str(), CurState());

    if (pcMsg->event == msgTasExitCmd)
    {
        OnPowerOff();
        return;
    }
    if (pcMsg->event == msgDump)
    {
        u32 *pdwCode = reinterpret_cast<u32*>(pcMsg->content);
        DaemonInstanceDump(*pdwCode);
        return;
    }

    if (CurState() == enStateServer)
    {
        ServerStateHandle(pcMsg);
    }
    else if (CurState() == enStateExit)
    {
        ExitStateHandle(pcMsg);
    }
    else if (CurState() == enStateInit)
    {
        InitStateHandle(pcMsg);
    }
    else // if (CurState() == enStateIdle)
    {
        IdleStateHandle(pcMsg);
    }
}

void CSyncAnalyzerInst::Analyze( const TSyncDataAnalyzeInfo &tData )
{
    CSyncAnalyzerAppData& cAppData = GetAppData();

    CSyncAnalyzerBase* pcAnalyzer = cAppData.SyncAnalyzerParent();
    if (pcAnalyzer != NULL)
    {
        pcAnalyzer->Analyze(tData);
    }

    vector<CSyncAnalyzerBase*> cAnalyzers = cAppData.SyncAnalyzerP2PLimit();
    for (vector<CSyncAnalyzerBase*>::iterator it = cAnalyzers.begin(); it != cAnalyzers.end(); ++it)
    {
        (*it)->Analyze(tData);
    }
}

CSyncAnalyzerAppData& CSyncAnalyzerInst::GetAppData()
{
    return g_cSyncAnalyzerApp.GetAppData();
}

LPCSTR CSyncAnalyzerInst::GetStrState() const
{
    if (enStateIdle == CurState()) return "enStateIdle";
    if (enStateInit == CurState()) return "enStateInit";
    if (enStateServer == CurState()) return "enStateServer";
    if (enStateExit == CurState()) return "enStateExit";
    CMS_ASSERT(false && "CSyncAnalyzerInst::GetStrState");
    return "enStateUnknown";
}

void CSyncAnalyzerInst::DaemonInstanceDump( u32 dwCode )
{
    TelePrint("\n 同步分析管理器App状态：%s\n", GetStrState());

    GetAppData().Dump(dwCode);
}

void CSyncAnalyzerInst::OnPowerOn()
{
    NextState(enStateInit);

    CSyncAnalyzerMngPoweron *pcSyncAnalyzerMngPoweron = new CSyncAnalyzerMngPoweron(this);
    pcSyncAnalyzerMngPoweron->InitStateMachine();
}

void CSyncAnalyzerInst::IdleStateHandle( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case OSP_POWERON:
        {
            OnPowerOn();
        }
        break;
    default:
        break;
    }
}


void CSyncAnalyzerInst::InitStateHandle( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case msgDbMsgTransfer:
        {
            // 只有一个上电任务
            if (PROCMSG_DEL == ProcDbMsgTransfer(pcMsg))
            {
                NextState(enStateServer);

                u32 dwModuleId = GetAppID();
                post(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgPoweronReadyNtf, &dwModuleId, sizeof(dwModuleId));
            }
        }
        break;
    default:
        break;
    }
}


void CSyncAnalyzerInst::ServerStateHandle( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case msgSyncAnalyzeCmd:
        {
            ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
            TSyncDataAnalyzeInfo *ptData = reinterpret_cast<TSyncDataAnalyzeInfo*>(ptr);

            Analyze(*ptData);
            delete ptData;
        }
        break;
    case msgDbMsgTransfer:
        {
            ProcDbMsgTransfer(pcMsg);
        }
        break;
    case msgSyncAnalyzerParentDel:
        {
            if (GetAppData().SyncAnalyzerParent() != NULL)
            {
                delete GetAppData().SyncAnalyzerParent();
                GetAppData().SyncAnalyzerParent() = NULL;
            }
        }
        break;
    case msgSyncAnalyzerParentAdd:
        {
            CMS_ASSERT(NULL == GetAppData().SyncAnalyzerParent());

            ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
            const UUIDString* ptParentDominId = reinterpret_cast<const UUIDString*>(ptr);
            CSyncAnalyzerParent *pcAnalyzer = new CSyncAnalyzerParent(this, *ptParentDominId);
            pcAnalyzer->InitStateMachine();
            delete ptParentDominId;

            GetAppData().SyncAnalyzerParent() = pcAnalyzer;
        }
        break;
    case msgSyncMngSubdomainAddNtf:
        {
            UUIDString tSubdomainId;
            tSubdomainId = reinterpret_cast<const s8*>(pcMsg->content);

            CDomainRelationInfo tInfo;
            tInfo.SetDomainID(tSubdomainId);
            tInfo.SetParentDomainID(CurrentDomainId());
            GetAppData().SubDomainIdAdd(tInfo);
        }
        break;
    case msgSyncMngSubdomainDelNtf:
        {
            UUIDString tSubdomainId;
            tSubdomainId = reinterpret_cast<const s8*>(pcMsg->content);
            GetAppData().SubDomainIdDel(tSubdomainId);
        }
        break;
    default:
        break;
    }
}

void CSyncAnalyzerInst::ExitStateHandle( CMessage* const pcMsg )
{
    if (msgSyncAnalyzerReadyExitDetectTimer == pcMsg->event)
    {
        if (GetAppData().WaitingSyncRecordNumber() == 0)
        {
            u32 dwModuleId = GetAppID();
            post(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgPoweroffReadyNtf, &dwModuleId, sizeof(dwModuleId));
        }
        else
        {
            StartReadyExitDetectTimer();
        }
    }
    else if (msgDbMsgTransfer == pcMsg->event)
    {
        ProcDbMsgTransfer(pcMsg);
    }
}

void CSyncAnalyzerInst::OnPowerOff()
{
    NextState(enStateExit);
    
    StartReadyExitDetectTimer();
}

void CSyncAnalyzerInst::StartReadyExitDetectTimer()
{
    SetTimer(msgSyncAnalyzerReadyExitDetectTimer, 300);
}

u32 CSyncAnalyzerInst::ProcDbMsgTransfer( CMessage* const pcMsg )
{
    u32 dwRet;

    CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));

    CTask *pcTask = FindTask(pcTransBase->GetOspTaskNo());
    if (NULL != pcTask)
    {
        dwRet = pcTask->ProcMsg(pcMsg);
        if (PROCMSG_DEL == dwRet)
        {
            delete pcTask;
        }
    }
    else
    {
        dwRet = PROCMSG_FAIL;
        Log(WARNING_LEV, "Not found the task for DbMsgTransfer, eventId=%u\n", (u32)pcTransBase->GetEventId());
    }

    delete pcTransBase;
    return dwRet;
}

void CSyncAnalyzerAppData::Dump( u32 dwCode )
{
    OspPrintf(TRUE, FALSE, "私有设备数目：%d\n", m_cPrivateDevIds.size());

    OspPrintf(TRUE, FALSE, "下级域列表:\n");
    for (u32 i = 0; i < m_cSubDomainIds.size(); ++i)
    {
        OspPrintf(TRUE, FALSE, "[%d]Id(%s)--parentId(%s)\n", i, 
            m_cSubDomainIds[i].GetDomainID().c_str(), m_cSubDomainIds[i].GetParentDomainID().c_str());
    }
}

u32 CSyncAnalyzerAppData::WaitingSyncRecordNumber()
{
    u32 dwNumber = 0;
    if (NULL != m_pcSyncAnalyzerParent)
    {
        dwNumber += m_pcSyncAnalyzerParent->WaitingSyncDataRecordNumber();
    }
    for (u32 i = 0; i < m_cSyncAnalyzerP2PLimit.size(); ++i)
    {
        dwNumber += m_cSyncAnalyzerP2PLimit[i]->WaitingSyncDataRecordNumber();
    }

    return dwNumber;
}

void CSyncAnalyzerAppData::SubDomainIdAdd( const CDomainRelationInfo &cInfo )
{
    m_cLock4SubDomainIds.Lock();
    bool bFound=false;
    for (vector<CDomainRelationInfo>::iterator it = m_cSubDomainIds.begin(); it != m_cSubDomainIds.end(); ++it)
    {
        if (it->GetDomainID() == cInfo.GetDomainID())
        {
            it->SetParentDomainID(cInfo.GetParentDomainID());
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        m_cSubDomainIds.push_back(cInfo);
    }

    m_cLock4SubDomainIds.UnLock();
}

void CSyncAnalyzerAppData::SubDomainIdDel( const UUIDString &tDomainId )
{
    vector<CDomainRelationInfo> tSubDomains;
    m_cLock4SubDomainIds.Lock();

    FindSubdomainList(m_cSubDomainIds, tDomainId, tSubDomains);

    for (vector<CDomainRelationInfo>::iterator it = m_cSubDomainIds.begin(); it != m_cSubDomainIds.end(); ++it)
    {
        if (it->GetDomainID() == tDomainId)
        {
            m_cSubDomainIds.erase(it);
            break;
        }
    }
    for (u32 i = 0; i < tSubDomains.size(); ++i)
    {
        for (vector<CDomainRelationInfo>::iterator it = m_cSubDomainIds.begin(); it != m_cSubDomainIds.end(); ++it)
        {
            if (it->GetDomainID() == tSubDomains[i].GetDomainID())
            {
                m_cSubDomainIds.erase(it);
                break;
            }
        }
    }


    m_cLock4SubDomainIds.UnLock();
}

void DelParentAnalyzer()
{
    OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncAnalyzerParentDel);
}

void AddParentAnalyzer(const UUIDString &tParentDomainId)
{
    UUIDString *ptId = new UUIDString(tParentDomainId);
    if (0 != OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncAnalyzerParentAdd, &ptId, sizeof(UUIDString*)))
    {
        delete ptId;
    }
}
