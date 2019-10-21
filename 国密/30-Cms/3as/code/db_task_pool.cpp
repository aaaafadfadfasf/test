#include "db_task_pool.h"
#include "config_util/tas_config.h"
#include "dbopr/dbengine.h"
#include "dbopr/dbcache.h"
#include "tas_redis.h"
#include "osp/osp.h"

CDbTaskPool g_cDbTaskPool;

CDbTaskPool::~CDbTaskPool()
{
    while(!m_tDbTaskInfoList.empty())
    {
        const TDbTaskItem &tItem = m_tDbTaskInfoList.front();
        delete tItem.m_pMsg;

        m_tDbTaskInfoList.pop_front();
    }
}

bool CDbTaskPool::Init()
{
    if (!OspSemBCreate(&m_hDbTaskInfoListLock))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create m_hDbTaskInfoListLock failed\n");
        return false;
    }

    if (!OspSemCCreate(&m_hDbTaskInfoSemCount, 0, 0xfffffff))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create m_hDbTaskInfoSemCount failed\n");
        return false;
    }

    if (!InitalDBEngine((ENDBType)CTasConfig::GetDbType()))
    {
        return false;
    }
    if (!CDbOpr::Init())
    {
        return false;
    }

    m_bQuit = false;
    m_dwDbTaskInfoMaxNum = CTasConfig::GetDbTaskItemMaxNum();
    u32 dwDbTaskThreadNum = CTasConfig::GetDbTaskThreadNum();

    if (!g_cDbCache.Init())
    {
        return false;
    }

    m_pcDbOpr = new CDbOpr[dwDbTaskThreadNum];
    m_phTaskHandle = new TASKHANDLE[dwDbTaskThreadNum];
    for (u32 i = 0; i < dwDbTaskThreadNum; ++i)
    {
		u32 *pdwDbOprIndex = new u32(i);
        m_phTaskHandle[i] = OspTaskCreate(CDbTaskPool::ProcessTaskEntry, "DbOprTask", 90, 1024*1024, (KD_PTR)pdwDbOprIndex, 0);
        if (0 == m_phTaskHandle[i])
        {
            CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create DbOprTask failed\n");
            return false;
        }
    }

	if (!OspSemBCreate(&m_hTransProgressLock))
	{
		CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create m_hTransProgressLock failed\n");
		return false;
	}

    m_cTransTimeoutTicks=OspClkRateGet()*60*2;

    return true;
}

void CDbTaskPool::Quit()
{
    m_bQuit = true;
    // 释放所有信号，以释放Db线程
    for (u32 i = 0; i < CTasConfig::GetDbTaskThreadNum(); ++i)
    {
        OspSemGive(m_hDbTaskInfoSemCount);
    }
    OspTaskDelay(500);

    CDbOpr::Quit();
}

bool CDbTaskPool::PostTask( u16 wEventId, const COspObj *pcMsg )
{
    OspSemTake(m_hDbTaskInfoListLock);
    if (m_tDbTaskInfoList.size() > m_dwDbTaskInfoMaxNum)
    {
        OspSemGive(m_hDbTaskInfoListLock);
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "DbTaskPool is full\n");
        return false;
    }
    OspSemGive(m_hDbTaskInfoListLock);

    TDbTaskItem tDbTaskItem(wEventId, pcMsg);
    Enqueue(tDbTaskItem);

    return true;
}

void* CDbTaskPool::ProcessTaskEntry( void* parm )
{
	u32 *pdwDbOprIndex=(u32*)parm;
	if (!pdwDbOprIndex)
	{
		GLOBELOG(ERROR_LEV, "ResetDate pArg is NULL");
		return NULL;
	}
	u32 dwDbOprIndex = *pdwDbOprIndex;
	delete pdwDbOprIndex;
	pdwDbOprIndex=NULL;
    // 建立连接
    g_cDbTaskPool.CheckDbConnection(dwDbOprIndex);

    u32 dwDequeueTimeoutTimes = 0;
    TDbTaskItem tDbTaskItem;
    while (!g_cDbTaskPool.m_bQuit)
    {
        if (g_cDbTaskPool.Dequeue(tDbTaskItem))
        {
            g_cDbTaskPool.ProcessTask(dwDbOprIndex, tDbTaskItem);
            GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "CDbTaskPool delete TDbTaskItem, msgobj pointer=%p\n", tDbTaskItem.m_pMsg);
            delete tDbTaskItem.m_pMsg;
        }
        else
        {
            // 长时间未处理任务，执行一下测试语句再检查连接情况
            if (++dwDequeueTimeoutTimes > 20)
            {
                g_cDbTaskPool.CheckDbConnection(dwDbOprIndex, true);
                dwDequeueTimeoutTimes = 0;
            }
        }
    }
    return 0;
}

void CDbTaskPool::Enqueue( const TDbTaskItem &tDbTaskItem )
{
    OspSemTake(m_hDbTaskInfoListLock);
    m_tDbTaskInfoList.push_back(tDbTaskItem);
    OspSemGive(m_hDbTaskInfoListLock);

    OspSemGive(m_hDbTaskInfoSemCount);
}

bool CDbTaskPool::Dequeue( TDbTaskItem &tDbTaskItem )
{
    if (FALSE == OspSemTakeByTime(m_hDbTaskInfoSemCount, 1000*3))
        return false;
    if (m_bQuit)
    {
        return false;
    }
    OspSemTake(m_hDbTaskInfoListLock);
    tDbTaskItem = *m_tDbTaskInfoList.begin();
    m_tDbTaskInfoList.pop_front();
    OspSemGive(m_hDbTaskInfoListLock);

    return true;
}

void CDbTaskPool::ProcessTask( u32 dwDbOprIndex, const TDbTaskItem &tDbTaskItem )
{
    CheckDbConnection(dwDbOprIndex);
	CheckFastdbConnection(dwDbOprIndex);

    u32 dwEventId = tDbTaskItem.m_wMsgId;

    if (msgTaskMsgTransfer4Db == dwEventId)
    {
        OnTaskSipMsgTransfer(&m_pcDbOpr[dwDbOprIndex], reinterpret_cast<const CTaskMsgTransferBase*>(tDbTaskItem.m_pMsg));
    }
    else if (msgDbMsgTransfer == dwEventId)
    {
        OnDbMsgTransfer(&m_pcDbOpr[dwDbOprIndex], reinterpret_cast<const CDbMsgTransferBase*>(tDbTaskItem.m_pMsg));
    }
    else if (msgLicenseUsageGetReq == dwEventId)
    {
        m_pcDbOpr[dwDbOprIndex].QueryLicenseUsage();
    }
    else if(msgDump == dwEventId)
    {
        m_pcDbOpr[dwDbOprIndex].Dump();
    }
    else
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "CDbTaskPool::ProcessTask recved unknown message, eventid=%u\n", dwEventId);
    }
}

void CDbTaskPool::CheckDbConnection( u32 dwDbOprIndex, bool bTryExecTestSql )
{
    CDbOpr *pDbOpr = &m_pcDbOpr[dwDbOprIndex];
    pDbOpr->CheckDbConnection(bTryExecTestSql);
}


void CDbTaskPool::CheckFastdbConnection(u32 dwDbOprIndex)
{
	CDbOpr *pDbOpr = &m_pcDbOpr[dwDbOprIndex];
	pDbOpr->CheckFastdbConnect();
}


void CDbTaskPool::OnTaskSipMsgTransfer( CDbOpr *pcDbOpr, const CTaskMsgTransferBase *pcMsg )
{
    u16 wEventId = pcMsg->GetEventId();

    switch(wEventId)
    {
    case TAS_LOGIN_REQ:
        pcDbOpr->Login(reinterpret_cast<const CTaskMsgTransfer<CLoginTasReq>*>(pcMsg));
        break;
    case msgCuserOnlineReq:
        pcDbOpr->CuOnline(reinterpret_cast<const CTaskMsgTransfer<CCuOnlineReq>*>(pcMsg));
        break;
    case TAS_DEVICE_ADD_REQ:
        pcDbOpr->DeviceAdd(reinterpret_cast<const CTaskMsgTransfer<CDeviceAddReq>*>(pcMsg));
        break;
    case TAS_DEVICE_DEL_REQ:
        pcDbOpr->DeviceDel(reinterpret_cast<const CTaskMsgTransfer<CDeviceDelReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MOD_REQ:
        pcDbOpr->DeviceMod(reinterpret_cast<const CTaskMsgTransfer<CDeviceModReq>*>(pcMsg));
        break;
	case TAS_DEVICE_BATCH_CHANGE_REQ:
		pcDbOpr->DeviceBatchChange(reinterpret_cast<const CTaskMsgTransfer<CTasDeviceBatchChangeReq>*>(pcMsg));
		break;
    case TAS_DEVICE_QRY_REQ:
        pcDbOpr->DeviceQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceQryReq>*>(pcMsg));
        break;
    case msgQryDevidReq:
        pcDbOpr->DeviceIdQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceQryReq>*>(pcMsg));
        break;
    case TAS_TVWALL_ADD_REQ:
        pcDbOpr->TvwallAdd(reinterpret_cast<const CTaskMsgTransfer<CTvwallAddReq>*>(pcMsg));
        break;
    case TAS_TVWALL_DEL_REQ:
        pcDbOpr->TvwallDel(reinterpret_cast<const CTaskMsgTransfer<CTvwallDelReq>*>(pcMsg));
        break;
    case TAS_TVWALL_MOD_REQ:
        pcDbOpr->TvwallMod(reinterpret_cast<const CTaskMsgTransfer<CTvwallModReq>*>(pcMsg));
        break;
    case TAS_TVWALL_QRY_REQ:
        pcDbOpr->TvwallQry(reinterpret_cast<const CTaskMsgTransfer<CTvwallQryReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MODEL_ADD_REQ:
        pcDbOpr->DeviceModelAdd(reinterpret_cast<const CTaskMsgTransfer<CDeviceModelAddReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MODEL_DEL_REQ:
        pcDbOpr->DeviceModelDel(reinterpret_cast<const CTaskMsgTransfer<CDeviceModelDelReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MODEL_QRY_REQ:
        pcDbOpr->DeviceModelQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceModelQryReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MANUFACTURER_ADD_REQ:
        pcDbOpr->DeviceManufacturerAdd(reinterpret_cast<const CTaskMsgTransfer<CDeviceManufacturerAddReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MANUFACTURER_DEL_REQ:
        pcDbOpr->DeviceManufacturerDel(reinterpret_cast<const CTaskMsgTransfer<CDeviceManufacturerDelReq>*>(pcMsg));
        break;
    case TAS_DEVICE_MANUFACTURER_QRY_REQ:
        pcDbOpr->DeviceManufacturerQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceManufacturerQryReq>*>(pcMsg));
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_GET_REQ:
        pcDbOpr->DeviceCapIndexNameGet(reinterpret_cast<const CTaskMsgTransfer<CDeviceCapIndexNameGetReq>*>(pcMsg));
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        pcDbOpr->DeviceCapIndexNameSet(reinterpret_cast<const CTaskMsgTransfer<CDeviceCapIndexNameSetReq>*>(pcMsg));
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ:
        pcDbOpr->DeviceCapIndexNameQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceCapIndexNameQryReq>*>(pcMsg));
        break;
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ:
        pcDbOpr->CuserDevicefullinfoAndPrivilegeQry(reinterpret_cast<const CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryReq>*>(pcMsg));
        break;
	case TAS_VIDSRC_INFO_QRY_REQ:
		pcDbOpr->VidsrcInfoQry(reinterpret_cast<const CTaskMsgTransfer<CVidsrcInfoQryReq>*>(pcMsg));
		break;
    case TAS_CUSER_ADD_REQ:
        pcDbOpr->CuserAdd(reinterpret_cast<const CTaskMsgTransfer<CUserAddReq>*>(pcMsg));
        break;
	case TAS_CUSER_DEL_REQ:
		pcDbOpr->CuserDel(reinterpret_cast<const CTaskMsgTransfer<CUserDelReq>*>(pcMsg));
		break;
	case TAS_CUSER_MOD_REQ:
		pcDbOpr->CuserMod(reinterpret_cast<const CTaskMsgTransfer<CUserModReq>*>(pcMsg));
		break;
	case TAS_CUSER_QRY_REQ:
		pcDbOpr->CuserQry(reinterpret_cast<const CTaskMsgTransfer<CUserQryReq>*>(pcMsg));
		break;
    case TAS_CUSER_DEVICE_ADD_REQ:
        pcDbOpr->CuserDevAdd(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceAddReq>*>(pcMsg));
        break;
    case TAS_CUSER_DEVICE_DEL_REQ:
        pcDbOpr->CuserDevDel(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceDelReq>*>(pcMsg));
        break;
    case TAS_CUSER_DEVICE_MOD_REQ:
        pcDbOpr->CuserDevMod(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceModReq>*>(pcMsg));
        break;
    case TAS_CUSER_DEVICE_QRY_REQ:
        pcDbOpr->CuserDevQry(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceQryReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_ADD_REQ:
        pcDbOpr->DevGrpAdd(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupAddReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_DEL_REQ:
        pcDbOpr->DevGrpDel(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDelReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_MOD_REQ:
        pcDbOpr->DevGrpMod(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupModReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_QRY_REQ:
        pcDbOpr->DevGrpQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupQryReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_DEVICE_ADD_REQ:
        pcDbOpr->DevGrpDevAdd(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDeviceAddReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_DEVICE_DEL_REQ:
        pcDbOpr->DevGrpDevDel(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDeviceDelReq>*>(pcMsg));
        break;
    case TAS_DEVICE_GROUP_DEVICE_QRY_REQ:
        pcDbOpr->DevGrpDevQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDeviceQryReq>*>(pcMsg));
        break;
	case TAS_DEVICE_GROUP_CHILDREN_QRY_REQ:
		pcDbOpr->DevGrpChildrenQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupChildrenQryReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ:
		pcDbOpr->DevGrpDevQry4Gb(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDeviceQry4GbReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ:
		pcDbOpr->DevGrpDevSumNumQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGrpDevSumNumReq>*>(pcMsg));
		break;
	case TAS_DEVICE_MATCH_GBID_REQ:
		pcDbOpr->DevGrpMatch(reinterpret_cast<const CTaskMsgTransfer<CDeviceMatchGbidReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GBINFO_QRY_REQ:
		pcDbOpr->DevGbInfoQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGbinfoQryReq>*>(pcMsg));
		break;
	case TAS_DEVGRP_GBINFO_QRY_REQ:
		pcDbOpr->DevGrpGbInfoQry(reinterpret_cast<const CTaskMsgTransfer<CDevgrpGbinfoQryReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GBID_ERROR_GET_REQ:
		pcDbOpr->DevGbidNextErrQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGbidErrGetReq>*>(pcMsg));
		break;  

    case TAS_CUSER_GROUP_ADD_REQ:
        pcDbOpr->CuserGrpAdd(reinterpret_cast<const CTaskMsgTransfer<CUserGroupAddReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_DEL_REQ:
        pcDbOpr->CuserGrpDel(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDelReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_MOD_REQ:
        pcDbOpr->CuserGrpMod(reinterpret_cast<const CTaskMsgTransfer<CUserGroupModReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_QRY_REQ:
        pcDbOpr->CuserGrpQry(reinterpret_cast<const CTaskMsgTransfer<CUserGroupQryReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_DEVICE_ADD_REQ:
        pcDbOpr->CuserGrpDevAdd(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceAddReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_DEVICE_DEL_REQ:
        pcDbOpr->CuserGrpDevDel(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceDelReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_DEVICE_MOD_REQ:
        pcDbOpr->CuserGrpDevMod(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceModReq>*>(pcMsg));
        break;
    case TAS_CUSER_GROUP_DEVICE_QRY_REQ:
        pcDbOpr->CuserGrpDevQry(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceQryReq>*>(pcMsg));
        break;
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ:
		pcDbOpr->CuserDevGrpVidSrcQry(reinterpret_cast<const CTaskMsgTransfer<CUserDevGrpVidsrcQryReq>*>(pcMsg));
		break;
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ:
		{
			pcDbOpr->CDevGrpVidsrcNumGet(reinterpret_cast<const CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetReq>*>(pcMsg));
		}
		break;
    case TAS_DOMAIN_RELATION_QRY_REQ:
        pcDbOpr->DomainRelationQry(reinterpret_cast<const CTaskMsgTransfer<CDomainRelationQryReq>*>(pcMsg));
        break;
    case TAS_DOMAIN_NAME_GET_REQ:
        pcDbOpr->GetDomainName(reinterpret_cast<const CTaskMsgTransfer<CDomainNameGetReq>*>(pcMsg));
        break;
    case TAS_CONFIG_TAS_GET_REQ:
        pcDbOpr->TasConfigGet(reinterpret_cast<const CTaskMsgTransfer<CConfigTasGetReq>*>(pcMsg));
        break;
    case TAS_CONFIG_TAS_SET_REQ:
        pcDbOpr->TasConfigSet(reinterpret_cast<const CTaskMsgTransfer<CConfigTasSetReq>*>(pcMsg));
        break;
    case TAS_CHILDTAS_ADD_REQ:
        pcDbOpr->ChildTasAdd(reinterpret_cast<const CTaskMsgTransfer<CChildtasAddReq>*>(pcMsg));
        break;
    case TAS_CHILDTAS_DEL_REQ:
        pcDbOpr->ChildTasDel(reinterpret_cast<const CTaskMsgTransfer<CChildtasDelReq>*>(pcMsg));
        break;
    case FILE_EXPORT_REQ:
        pcDbOpr->CuserExportDevData(reinterpret_cast<const CTaskMsgTransfer<CFileExportReq>*>(pcMsg));
        break;
	case TAS_CUSER_DEVICE_GROUP_ADD_REQ:
		pcDbOpr->CuserDeviceGroupAdd(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceGroupAddReq>*>(pcMsg));
		break;
	case TAS_CUSER_DEVICE_GROUP_DEL_REQ:
		pcDbOpr->CuserDeviceGroupDel(reinterpret_cast<const CTaskMsgTransfer<CUserDeviceGroupDelReq>*>(pcMsg));
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ:
		pcDbOpr->CuserGroupDeviceGroupAdd(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceGroupAddReq>*>(pcMsg));
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ:
		pcDbOpr->CuserGroupDeviceGroupDel(reinterpret_cast<const CTaskMsgTransfer<CUserGroupDeviceGroupDelReq>*>(pcMsg));
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ:
		pcDbOpr->CDeviceAutoAssignToUserSet(reinterpret_cast<const CTaskMsgTransfer<CDeviceAutoAssignToUserSetReq>*>(pcMsg));
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ:
		pcDbOpr->CDeviceAutoAssignToUserGroupSet(reinterpret_cast<const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupSetReq>*>(pcMsg));
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ:
		pcDbOpr->CDeviceAutoAssignToUserQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceAutoAssignToUserQryReq>*>(pcMsg));
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ:
		pcDbOpr->CDeviceAutoAssignToUserGroupQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupQryReq>*>(pcMsg));
		break;
    case TAS_CUSER_DATA_VERSION_GET_REQ:
        pcDbOpr->CuDataVersionGet(reinterpret_cast<const CTaskMsgTransfer<CCuserDataVersionGetReq>*>(pcMsg));
        break;
	case TAS_DEVTREE_ADD_REQ:
		pcDbOpr->DevTreeAdd(reinterpret_cast<const CTaskMsgTransfer<CDevTreeAddReq>*>(pcMsg));
		break;														 
	case TAS_DEVTREE_DEL_REQ:										 
		pcDbOpr->DevTreeDel(reinterpret_cast<const CTaskMsgTransfer<CDevTreeDelReq>*>(pcMsg));
		break;														 
	case TAS_DEVTREE_MOD_REQ:										 
		pcDbOpr->DevTreeMod(reinterpret_cast<const CTaskMsgTransfer<CDevTreeModReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GROUP_DEVICE_ORDER_REQ:
		pcDbOpr->DevGrpDevOrder(reinterpret_cast<const CTaskMsgTransfer<CDeviceGroupDeviceOrderReq>*>(pcMsg));
		break;
	case TAS_DEVTREE_QRY_REQ:
		pcDbOpr->DevTreeQry(reinterpret_cast<const CTaskMsgTransfer<CDevTreeQryReq>*>(pcMsg));
		break;
	case TAS_DEVTREE_DEVGROUP_ASSIGN_REQ:
		pcDbOpr->DevTreeDevGrpAssign(reinterpret_cast<const CTaskMsgTransfer<CDevTreeDevGrpAssignReq>*>(pcMsg));
		break;
	case TAS_DEVICE_GROUPPATH_QRY_REQ:
		pcDbOpr->DeviceGrpPathQry(reinterpret_cast<const CTaskMsgTransfer<CDeviceGrpPathQryReq>*>(pcMsg));
		break;
	case TAS_DEV_CAPINFO_MOD_REQ:										 
		pcDbOpr->DevCapInfoMod(reinterpret_cast<const CTaskMsgTransfer<CDevCapInfoModReq>*>(pcMsg));
		break;
	case TAS_PTZLOCK_ADD_REQ:
		pcDbOpr->PtzLockAdd(reinterpret_cast<const CTaskMsgTransfer<CPtzLockAddReq>*>(pcMsg));
		break;														 
	case TAS_PTZLOCK_DEL_REQ:										 
		pcDbOpr->PtzLockDel(reinterpret_cast<const CTaskMsgTransfer<CPtzLockDelReq>*>(pcMsg));
		break;														 
	case TAS_PTZLOCK_QRY_REQ:
		pcDbOpr->PtzLockQry(reinterpret_cast<const CTaskMsgTransfer<CPtzLockQryReq>*>(pcMsg));
		break;
	case TAS_GATEWAY_DEL_REQ:
		pcDbOpr->DelGateWayData(reinterpret_cast<const CTaskMsgTransfer<CGatewayDelReq>*>(pcMsg));
		break;
    default:
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "OnTaskSipMsgTransfer recved unknown message, eventid=%u\n", (u32)wEventId);
        break;
    }
}

void CDbTaskPool::OnDbMsgTransfer( CDbOpr *pcDbOpr, const CDbMsgTransferBase *pcMsg )
{
    u16 wEventId = pcMsg->GetEventId();

    switch(wEventId)
    {
    case msgSyncDataRecordStoreReq:
        pcDbOpr->SyncDataRecordAdd(reinterpret_cast<const CDbMsgTransfer<TSyncDataRecordStoreReq>*>(pcMsg));
        break;
    case msgSyncDataRecordDelReq:
        pcDbOpr->SyncDataRecordDel(reinterpret_cast<const CDbMsgTransfer<TSyncDataRecordDelReq>*>(pcMsg));
        break;
    case msgSyncDataConstructReq:
        pcDbOpr->SyncDataConstruct(reinterpret_cast<const CDbMsgTransfer<TSyncDataConstructReq>*>(pcMsg));
        break;
    case msgSyncDataStoreReq:
        pcDbOpr->SyncDataStore(reinterpret_cast<const CDbMsgTransfer<TSyncDataStoreReq>*>(pcMsg));
        break;
    case msgSyncDomainAuthorizationDependenceGetReq:
        pcDbOpr->SyncDomainAuthorizationDependenceQry(reinterpret_cast<const CDbMsgTransfer<string>*>(pcMsg));
        break;
    case msgSyncDevInfoRelateDataGetReq:
        pcDbOpr->SyncDevInfoRelateDataQry(reinterpret_cast<const CDbMsgTransfer<string>*>(pcMsg));
        break;

    case msgPrivateDevIdListGetReq:
        pcDbOpr->PrivateDeviceIdListQry(reinterpret_cast<const CDbMsgTransfer<void>*>(pcMsg));
        break;
    case msgDomainRelationListGetReq:
        pcDbOpr->DomainRelationListQry(reinterpret_cast<const CDbMsgTransfer<void>*>(pcMsg));
        break;
    case msgSyncDstDomainListGetReq:
        pcDbOpr->SyncDstDomainListQry(reinterpret_cast<const CDbMsgTransfer<void>*>(pcMsg));
        break;
    case msgDomainAuthorizationDevIdListGetReq:
        pcDbOpr->DomainAuthorizationDeviceIdListQry(reinterpret_cast<const CDbMsgTransfer<string>*>(pcMsg));
        break;
	case msgSetDevGrpGbidReq:
		pcDbOpr->DevGrpGbidSet(reinterpret_cast<const CDbMsgTransfer<TSetDevGrpGbidReq>*>(pcMsg));
		break;
	case msgSetDevGbidReq:
		pcDbOpr->DevGbidSet(reinterpret_cast<const CDbMsgTransfer<TSetDevGbidReq>*>(pcMsg));
		break;
	case msgMatchChildrenGrpGbidReq:
		pcDbOpr->MatchChildrenGrpGbid(reinterpret_cast<const CDbMsgTransfer<TMatchChildrenGrpGbidReq>*>(pcMsg));
		break;
	case msgQryGbidEmptyDevsReq:
		pcDbOpr->EmptyGbidDevsQry(reinterpret_cast<const CDbMsgTransfer<TQryGbidEmptyDevsReq>*>(pcMsg));
		break;
	case msgDevGrp2DevGbidOperateReq:
		pcDbOpr->DevGrp2DevGbidOperate(reinterpret_cast<const CDbMsgTransfer<CDevGrp2DevGbidOperateReq>*>(pcMsg));
		break;
	case msgDevGrpGbidOperateReq:
		pcDbOpr->DevGrpGbidOperate(reinterpret_cast<const CDbMsgTransfer<CDevGrpGbidOperateReq>*>(pcMsg));
		break;
	case msgDevGrpAllChildDevQryReq:
		pcDbOpr->DevGrpAllChildDevQry(reinterpret_cast<const CDbMsgTransfer<CDevGrpAllChildDevQryReq>*>(pcMsg));
		break;
	case msgTableDataToPushXmlReq:
		pcDbOpr->TableDataToPushXmlFile(reinterpret_cast<const CDbMsgTransfer<string>*>(pcMsg));
		break;
    default:
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "OnDbMsgTransfer recved unknown message, eventid=%u\n", (u32)wEventId);
    }
}


void CDbTaskPool::Dump(u32 dwCode)
{
    TelePrint(" 数据库任务池状态数据=============================================================");
    OspSemTake(m_hDbTaskInfoListLock);
    TelePrint(" 当前任务数目 : %d", m_tDbTaskInfoList.size());
    OspSemGive(m_hDbTaskInfoListLock);

    TelePrint(" 任务线程信息---------------------------------------------------------------------");
    for (u32 i = 0; i < CTasConfig::GetDbTaskThreadNum(); ++i)
    {
		if (!m_pcDbOpr)
		{
			break;
		}
        TelePrint(" 线程[%u] 数据库连接状态 : %s", i, m_pcDbOpr[i].IsConnectedDb()? "连接":"断开");
    }

    TelePrint(" 数据库Cache对象的连接状态 : %s", g_cDbCache.IsConnectedDb()? "连接":"断开");
    if (dwCode > 0) PostTask(msgDump, NULL);

	TelePrint(" 事务的完成进度=============================================================");
	CCMSTime cStartTime;
	s32 nElapse = 0;
	OspSemTake(m_hTransProgressLock);
	for (map<string, TTransProgress>::const_iterator it = m_cTransProgress.begin(); m_cTransProgress.end() != it; ++it)
	{
		cStartTime.SetTime((time_t)it->second.startTime);
		nElapse = it->second.endTime - it->second.startTime;
		nElapse = nElapse > 0 ? nElapse : 0;
		

		TelePrint(" 事务[%s]的进度信息：StartTime[%s], Elapse[%d]秒, TotalNum[%d], FinishNum[%d]", 
			it->first.c_str(), cStartTime.ToString().c_str(), nElapse,  
			it->second.totalNum, it->second.finishNum);
	}
	OspSemGive(m_hTransProgressLock);

	TelePrint("\n");
}

void CDbTaskPool::AddTransProgress(const string &strTransTag, int nTotalNum, int nFinishNum)
{
	OspSemTake(m_hTransProgressLock);

	time_t tTime;
	time(&tTime);

	TTransProgress tProgress;
	tProgress.startTime = (s32)tTime;
	tProgress.totalNum  = nTotalNum;
	tProgress.finishNum = nFinishNum;

	if (nTotalNum <= nFinishNum)		// 登记时已经完成事务
	{
		tProgress.endTime = (s32)tTime;
	}

	m_cTransProgress[strTransTag] = tProgress;
    m_cTransAliveData[strTransTag]= OspTickGet64();

	// 事务的数量达到20的倍数时开始清理过期的进度信息
	if (0 == m_cTransProgress.size() % 20)
	{
		for (map<string, TTransProgress>::iterator it = m_cTransProgress.begin(); m_cTransProgress.end() != it;)
		{
			// 当事务处理时间超过上限（2小时）或者处理结束超过半小时时，这些进度信息都可以清除掉了
			if (it->second.startTime + 7200 < (s32)tTime || (it->second.endTime != 0 && it->second.endTime + 1800 < (s32)tTime))
			{
				m_cTransProcErr.erase(it->first);
				m_cTransProgress.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	OspSemGive(m_hTransProgressLock);
}

bool CDbTaskPool::AddTransProgress(const string &strTransTag, int nFinishNum, ECMS_ERRORCODE errCode)
{
    bool bSuccess=true;
	OspSemTake(m_hTransProgressLock);

	map<string, TTransProgress>::iterator it = m_cTransProgress.find(strTransTag);
	if (m_cTransProgress.end() != it)
	{
		it->second.finishNum = nFinishNum;

		if (errCode != CMS_SUCCESS)
		{
			if (m_cTransProcErr.end() == m_cTransProcErr.find(strTransTag))
			{
				m_cTransProcErr.insert(std::pair<string, ECMS_ERRORCODE>(strTransTag, errCode));
			}
		}

		if (it->second.totalNum <= it->second.finishNum)
		{
			time_t tTime;
			time(&tTime);
			it->second.endTime = (s32)tTime;
		}

        if (OspTickGet64()-m_cTransAliveData[strTransTag]>m_cTransTimeoutTicks)
        {
            bSuccess=false;
        }
	}else bSuccess=false;

	OspSemGive(m_hTransProgressLock);

    return bSuccess;
}

TTransProgress CDbTaskPool::GetTransProgress(const string &strTransTag)
{
	OspSemTake(m_hTransProgressLock);
	TTransProgress tProgress;

	map<string, TTransProgress>::const_iterator it = m_cTransProgress.find(strTransTag);
	if (m_cTransProgress.end() != it)
	{
        m_cTransAliveData[strTransTag]=OspTickGet64();
		tProgress = it->second;
	}
	else
	{
		tProgress.startTime = 0;
		tProgress.endTime   = 0;
		tProgress.totalNum  = -1;
		tProgress.finishNum = -1;
	}

	OspSemGive(m_hTransProgressLock);
	return tProgress;
}

ECMS_ERRORCODE CDbTaskPool::GetTransProErr( const string &strTransTag )
{
	map<string, ECMS_ERRORCODE> ::iterator it = m_cTransProcErr.find(strTransTag);
	if (it != m_cTransProcErr.end())
	{
		return it->second;
	}
	return CMS_SUCCESS;
}

TDbTaskItem::TDbTaskItem( u16 wEventId, const COspObj *pcMsg )
{
	m_wMsgId = wEventId;
	m_pMsg = pcMsg;
}