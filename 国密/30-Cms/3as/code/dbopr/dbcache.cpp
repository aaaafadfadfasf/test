#include "dbcache.h"
#include "dbopr/dbopr.h"

CDbCache g_cDbCache;

bool CDbCache::Init()
{
    if (!OspSemBCreate(&m_hDeviceModelLock))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create CDbCache::m_hDeviceModelLock failed\n");
        return false;
    }

    if (!OspSemBCreate(&m_hDbOprLock))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "create CDbCache::m_hDbOprLock failed\n");
        return false;
    }
    m_pcDbOpr = new CDbOpr();

    return true;
}

void CDbCache::Quit()
{
    delete m_pcDbOpr;
    OspSemDelete(m_hDbOprLock);
    OspSemDelete(m_hDeviceModelLock);
}

CDeviceModelInfo CDbCache::GetDeviceModelInfo( const UUIDString &strModelId, CDbOpr *pcDbOpr )
{
    CDeviceModelInfo cVal;

    OspSemTake(m_hDeviceModelLock);
    DevModelMap::iterator it = m_cDeviceModels.find(strModelId);
    if (m_cDeviceModels.end() != it)
    {
        cVal = it->second;
    }
    OspSemGive(m_hDeviceModelLock);

    if (cVal.GetDeviceModelId().empty() && NULL != pcDbOpr)
    {
        CDeviceModelInfo cModel;
        if (CMS_SUCCESS == pcDbOpr->DeviceModelItemQry(strModelId, cModel))
        {
            OspSemTake(m_hDeviceModelLock);
            
            pair<DevModelMap::iterator, bool> pairV = m_cDeviceModels.insert(DevModelMap::value_type(strModelId, cModel));
            cVal = pairV.first->second;

            OspSemGive(m_hDeviceModelLock);
        }
    }
    if (cVal.GetDeviceModelId().empty() && NULL == pcDbOpr)
    {
        CDeviceModelInfo cModel;
        
        OspSemTake(m_hDbOprLock);
        m_pcDbOpr->CheckDbConnection();
        ENTasErrno enQryRet = m_pcDbOpr->DeviceModelItemQry(strModelId, cModel);
        OspSemGive(m_hDbOprLock);
        if (CMS_SUCCESS == enQryRet)
        {
            OspSemTake(m_hDeviceModelLock);
            pair<DevModelMap::iterator, bool> pairV = m_cDeviceModels.insert(DevModelMap::value_type(strModelId, cModel));
            cVal = pairV.first->second;
            OspSemGive(m_hDeviceModelLock);
        }
    }

    if (cVal.GetDeviceModelId().empty())
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "Not found DeviceModel, the DeviceModelId=%s\n", strModelId.c_str());
    }
    return cVal;
}

void CDbCache::DeleteDeviceModelInfoByModelId( const UUIDString &strModelId )
{
    OspSemTake(m_hDeviceModelLock);
    m_cDeviceModels.erase(strModelId);
    OspSemGive(m_hDeviceModelLock);
}

bool CDbCache::IsConnectedDb()
{
    return m_pcDbOpr->IsConnectedDb();
}
