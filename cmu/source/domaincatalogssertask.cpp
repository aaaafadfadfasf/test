
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "domaincatalogssertask.h"
#include "cmuconnecttask.h"
#include "pulistsstask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"


CDomainCatalogSserTask::CDomainCatalogSserTask(CInstExt *pcInst, CCmuConnecterTask * const pcConnectTask) : CDomainSsBaseTaskTmpl<TSsType>(pcInst, pcConnectTask)
{
    m_dwDrop  = 0;
    m_dwSize  = 0;
    m_dwTotal = 0;
    m_dwTop   = 0;
}

CDomainCatalogSserTask::~CDomainCatalogSserTask()
{
	ReleaseResource();

    CCmuConnecterTask *pcConnectTask = dynamic_cast<CCmuConnecterTask *>(GetConnectTask());
    if (NULL != pcConnectTask)
    {
        pcConnectTask->ExitDomainCatalogSserTask(this);
    }
}


void CDomainCatalogSserTask::ReleaseResource()
{

}

void CDomainCatalogSserTask::PrintData() const
{
	CDomainSsBaseTaskTmpl<TSsType>::PrintData();

    OspPrintf(TRUE, FALSE, "---------------------------------DevDataCache----------------------------------\n");
    OspPrintf(TRUE, FALSE, "  Size[%u], Drop[%u], Top[%u], Total[%u], MaxNum[%u], MaxTime[%d]\n", 
        m_dwSize, m_dwDrop, m_dwTop, m_dwTotal, m_sMaxCacheNum, m_sMaxCacheTime);
    OspPrintf(TRUE, FALSE, "\n");
}

void CDomainCatalogSserTask::TUnitList2TSsList(const set<CSsUnit<string> > &tUnitList, TSsTypes &tSsTypes)
{
    for (set<CSsUnit<string> >::const_iterator it = tUnitList.begin(); tUnitList.end() != it; ++it)
    {
        tSsTypes.insert(it->GetSsType());
    }
}

string CDomainCatalogSserTask::GetSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo)
{
	const CCmuConnecterTask *pcConnectTask = (CCmuConnecterTask *)GetConnectTask();
	CMS_ASSERT(NULL != pcConnectTask);

	CCatalogStatusSsReq cReq;
	cReq.SetSession(GetLocalUri().GetURIString());
	cReq.SetUserUri(GetLocalUri().GetURIString());
	cReq.SetBatchNotify(true);
    cReq.SetDomain(GetDomainName());
    TUnitList2TSsList(tOpInfo.cSsList, cReq.GetSsTypes());

	return cReq.ToXml();
}

string CDomainCatalogSserTask::GetRefreshSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo)
{
	const CCmuConnecterTask *pcConnectTask = (CCmuConnecterTask *)GetConnectTask();
	CMS_ASSERT(NULL != pcConnectTask);

	CCatalogStatusRefreshSsReq cReq;
	cReq.SetSession(GetLocalUri().GetURIString());
	cReq.SetSsOpType(tOpInfo.tSsOpType);
    TUnitList2TSsList(tOpInfo.cSsList, cReq.GetSsTypes());

	return cReq.ToXml();
}

void CDomainCatalogSserTask::ProcSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList)
{
	DealSsOpRsp(tOpInfo, cErrorList);
}

void CDomainCatalogSserTask::ProcRefreshSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList)
{
	DealSsOpRsp(tOpInfo, cErrorList);
}

void CDomainCatalogSserTask::ProcNotifyDevNotExist(const TSsType &tSsType, const CEventReq *pcNtfReq)
{
    TASKLOG(CMU_MOD_SS, WARNING_LEV, "receive ss[%s] notify, but pudata is not exist, add into cache!\n", tSsType.c_str());

    if (PU_GPS_STATUS == tSsType || PU_TRANSDATA_STATUS == tSsType)
    {
        /*
         * gps、透明数据有很强的即时性，没有缓存的必要
         */
        return;
    }

    if (PU_ON_OFF_STATUS == tSsType)
    {
        const CPuStatusNtyReq *pcReq = dynamic_cast<const CPuStatusNtyReq *>(pcNtfReq);
        if (NULL != pcReq)
        {
            TSipURI tDevUri(pcReq->GetDevURI());

            map<string, TDevCacheData>::iterator it = m_cDevCacheData.find(tDevUri.GetUser());
            if (m_cDevCacheData.end() != it)
            {
                it->second.data.SetPuStatus(pcReq->GetOnOffStatus());
            }
            else
            {
                ++m_dwTotal;

                TDevCacheData &tCacheData = m_cDevCacheData[tDevUri.GetUser()];
                tCacheData.data.SetPuStatus(pcReq->GetOnOffStatus());
                tCacheData.time = time(NULL);
            }
        }
    }
    else if (PU_CONFIG_STATUS == tSsType)
    {
        const CPuConfigNtyReq *pcReq = dynamic_cast<const CPuConfigNtyReq *>(pcNtfReq);
        if (NULL != pcReq)
        {
            TSipURI tDevUri(pcReq->GetDevURI());

            map<string, TDevCacheData>::iterator it = m_cDevCacheData.find(tDevUri.GetUser());
            if (m_cDevCacheData.end() != it)
            {
                it->second.data.MergePuConfig(pcReq->GetConfig());
                it->second.puconfig = true;
            }
            else
            {
                TASKLOG(CMU_MOD_SS, WARNING_LEV, "invalid ss notify, type[%s]!\n", tSsType.c_str());
            }
        }
    }
    else if (PU_SERVICE_STATUS == tSsType)
    {
        const CPuServiceStatusNtyReq *pcReq = dynamic_cast<const CPuServiceStatusNtyReq *>(pcNtfReq);
        if (NULL != pcReq)
        {
            TSipURI tDevUri(pcReq->GetDevURI());

            map<string, TDevCacheData>::iterator it = m_cDevCacheData.find(tDevUri.GetUser());
            if (m_cDevCacheData.end() != it)
            {
                it->second.data.SetPuServiceStatus(pcReq->GetServiceStatus());
                it->second.puservice = true;
            }
            else
            {
                TASKLOG(CMU_MOD_SS, WARNING_LEV, "invalid ss notify, type[%s]!\n", tSsType.c_str());
            }
        }
    }
    else if (PU_ALARM_STATUS == tSsType)
    {
        const CPuAlarmStatusNtyReq *pcReq = dynamic_cast<const CPuAlarmStatusNtyReq *>(pcNtfReq);
        if (NULL != pcReq)
        {
            TSipURI tDevUri(pcReq->GetDevURI());

            map<string, TDevCacheData>::iterator it = m_cDevCacheData.find(tDevUri.GetUser());
            if (m_cDevCacheData.end() != it)
            {
                it->second.data.UpdateAlarmStatus(pcReq->GetAlarmStatus());
                it->second.pualarm = true;
            }
            else
            {
                TASKLOG(CMU_MOD_SS, WARNING_LEV, "invalid ss notify, type[%s]!\n", tSsType.c_str());
            }
        }
    }

    u32 dwSize = m_cDevCacheData.size();
    if (dwSize > m_dwTop)
    {
        m_dwTop = dwSize;
    }

    if (dwSize > m_sMaxCacheNum)
    {
        ++m_dwDrop;

        u32 dwNow = time(NULL);
        
        string strPopDevId;
        string strLastCacheTimeDevId;
        u32    dwLastCacheTime = 0;
        for (map<string, TDevCacheData>::const_iterator it = m_cDevCacheData.begin(); m_cDevCacheData.end() != it; ++it)
        {
            if (it->second.time + m_sMaxCacheTime < dwNow)
            {
                strPopDevId = it->first;
                break;
            }

            if (it->second.time < dwLastCacheTime)
            {
                dwLastCacheTime = it->second.time;
                strLastCacheTimeDevId = it->first;
            }
        }

        // 如果没有找到超过最大缓存时间的记录，就取最初缓存的一条吧
        strPopDevId = strPopDevId.empty() ? strLastCacheTimeDevId : strPopDevId;

        if (!strPopDevId.empty())
        {
            TASKLOG(CMU_MOD_SS, WARNING_LEV, "缓存数据太多[%u]，超过最大值[%u]，DevId[%s]缓存信息将被删除，缓存时间[%u]s!\n", 
                dwSize, m_sMaxCacheNum, strPopDevId.c_str(), dwNow - dwLastCacheTime);

            m_cDevCacheData.erase(strPopDevId);
        }
    }
}

void CDomainCatalogSserTask::PopCache(const string &strDevId)
{
    map<string, TDevCacheData>::iterator it = m_cDevCacheData.find(strDevId);
    if (m_cDevCacheData.end() != it)
    {
        TSipURI tDevUri;
        tDevUri.SetUser(strDevId);
        tDevUri.SetDomain(GetDomainName());

        CPuData *pcPuData = GetPuDataByUri(tDevUri);
        CMS_ASSERT(NULL != pcPuData && "pcPuData = NULL!!!");

        CRedisDevData &cDevData = it->second.data;
        pcPuData->SetPuStatus(cDevData.GetPuStatus());
        pcPuData->PublishOnOff(cDevData.GetPuStatus());

        if (it->second.puconfig)
        {
            pcPuData->SetPuConfig(cDevData.GetPuConfig());

            pcPuData->PublishConfig(cDevData.GetPuConfig().GetDevConfig());
        }

        if (it->second.puservice)
        {
            pcPuData->SetPuServiceStatus(cDevData.GetPuServiceStatus());

            pcPuData->PublishService(cDevData.GetPuServiceStatus());
        }

        if (it->second.pualarm)
        {
            pcPuData->UpdateAlarmStatus(cDevData.GetPuAlarmStatus());

            pcPuData->PublishAlarm(cDevData.GetPuAlarmStatus());
        }

        u32 dwNow = time(NULL);
        TASKLOG(CMU_MOD_SS, EVENT_LEV, "DevId[%s]缓存信息将移出缓存，缓存时间[%u]s!\n", 
            it->first.c_str(), dwNow - it->second.time);

        m_cDevCacheData.erase(it);
    }
}

void CDomainCatalogSserTask::DealSsOpRsp(const TSsOpInfo<string> &tOpInfo, set<CSsUnit<string> > &cErrorList)
{
    // 域订阅当时的接口没有设置失败列表，啥都不用处理了
}



