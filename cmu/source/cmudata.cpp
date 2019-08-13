
#include "cms/ospext/osplog.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/cmstime.h"
#include "cms/tas/tas_event.h"
#include "cms/uas/uas_event.h"

#include "system/converthelper/toxml.h"
#include "system/converthelper/tostruct.h"

#include "cmudata.h"
#include "cmuconst.h"

#include "cmucoreapp.h"

#include "puregtask.h"
#include "vtduregtask.h"
#include "puiregtask.h"
#include "cuiregtask.h"
#include "culogintask.h"
#include "devlogintask.h"
#include "catalogsstask.h"
#include "domaincatalogssertask.h"
#include "domainpulistssertask.h"
#include "pureqrsptask.h"
#include "cms/cmu/cmu_redis.h"
#include "pubatchregtask.h"
#include "redisloginsession.h"

TPuIdUriMapList g_cPuIdUriMapList; //1.0puid和2.0puUri映射表

//===============================================================================================================================

TRedisModuleType DevType2RedisModuleType(const string &tDevType)
{
    if (DEV_TYPE_CUI == tDevType)
    {
        return RedisModuleType_Cui;
    }
    else if (DEV_TYPE_CMU == tDevType)
    {
        return RedisModuleType_Cmu;
    }
    else if (DEV_TYPE_PUI == tDevType)
    {
        return RedisModuleType_Pui;
    }
    else if (DEV_TYPE_VTDU == tDevType)
    {
        return RedisModuleType_Vtdu;
    }
    else if (DEV_TYPE_NRU == tDevType)
    {
        return RedisModuleType_Nru;
    }
    else if (DEV_TYPE_RCS == tDevType)
    {
        return RedisModuleType_Rcs;
    }
    else
    {
        return RedisModuleType_Invalid;
    }
}

string RedisModuleType2DevType(const TRedisModuleType &tModuleType)
{
    if (RedisModuleType_Cui == tModuleType)
    {
        return DEV_TYPE_CUI;
    }
    else if (RedisModuleType_Cmu == tModuleType)
    {
        return DEV_TYPE_CMU;
    }
    else if (RedisModuleType_Pui == tModuleType)
    {
        return DEV_TYPE_PUI;
    }
    else if (RedisModuleType_Vtdu == tModuleType)
    {
        return DEV_TYPE_VTDU;
    }
    else if (RedisModuleType_Nru == tModuleType)
    {
        return DEV_TYPE_NRU;
    }
    else if (RedisModuleType_Rcs == tModuleType)
    {
        return DEV_TYPE_RCS;
    }
    else
    {
        return DEV_TYPE_UNKNOWN;
    }
}

CCmuCoreInst* GetWorkInst()
{
    CApp* pcApp = (CApp*)(&g_cCmuCoreApp);
    CInstExt* pcInst = (CInstExt*)pcApp->GetInstance(CInstance::DAEMON);
    CMS_ASSERT(pcInst != NULL);

    return (CCmuCoreInst*)pcInst;
}

vector<CTask *> GetDomainSserTasksByDomainName(const string &strDomainName)
{
#define CATALOG_SS_TASK_NAME "CCatalogSubscribeTask"

    vector<CTask *> cTasks = GetWorkInst()->GetTasksArray(CATALOG_SS_TASK_NAME);
    vector<CTask *> cResult;
    for (vector<CTask *>::iterator it = cTasks.begin(); cTasks.end() != it; ++it)
    {
        CCatalogSubscribeTask *pcTask = dynamic_cast<CCatalogSubscribeTask *>(*it);
        if (NULL != pcTask && strDomainName == pcTask->GetDomain())
        {
            cResult.push_back(*it);
        }
    }

    return cResult;
}

CRedisPipe &GetLocalRedisPipe()
{
    return CCmuData::GetInstance()->GetLocalRedisPipe();
}

TSipURI GetLocalUri()
{
    return g_cCmuConf.m_tLocalUri;
}

string GetDevDomainId(const string &strDevId)
{
    string strDomainId;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevDomainId(strDevId, strDomainId);

    return strDomainId;
}

TSipURI GetDevUriByDevId(const string &strDevId)
{
    TSipURI tDevUri;
    tDevUri.SetUser(strDevId);
    tDevUri.SetDomain(DomainId2DomainName(GetDevDomainId(strDevId)));

    return tDevUri;    
}

bool IsLocalDomainByDomainId(const string &strDomainId)
{    
    return (strDomainId == CCmuData::GetInstance()->GetLocalDomainId());
}    

bool IsLocalDomainByDomainName(const string &strDomainName)
{    
    return (strDomainName == CCmuData::GetInstance()->GetLocalDomainName());
}

bool IsLocalDomainByRedisPipe(const CRedisPipe *pcRedisPipe)
{
    return (pcRedisPipe == &(CCmuData::GetInstance()->GetLocalRedisPipe()));
}

bool IsLocalDomainByDevUri(const string &strDevUri)
{    
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    return (tDevUri.GetDomain() == CCmuData::GetInstance()->GetLocalDomainName());
}    

bool IsLocalDomainByDevUri(const TSipURI &tDevUri)
{
    return (tDevUri.GetDomain() == CCmuData::GetInstance()->GetLocalDomainName());
}

string DomainId2DomainName(const string &strDomainId)
{
    return CCmuData::GetInstance()->DomainId2DomainName(strDomainId);
}

string DomainName2DomainId(const string &strDomainName)
{
    return CCmuData::GetInstance()->DomainName2DomainId(strDomainName);
}

string DomainNO2DomainName(const TKDMNO &tDomainNO)
{
    return CCmuData::GetInstance()->DomainNO2DomainName(tDomainNO);
}

CPuData *GetPuData(const string &strDevId)
{
    return CCmuData::GetInstance()->GetPuData(strDevId);
}

CPuData *GetPuData(const string &strDomainId, const string &strDevId)
{
    return CCmuData::GetInstance()->GetPuData(strDomainId, strDevId);
}

CPuData *GetPuDataByUri(const string &strDevUri)
{
    return CCmuData::GetInstance()->GetPuDataByUri(strDevUri);
}

CPuData *GetPuDataByUri(const TSipURI &tDevUri)
{
    return CCmuData::GetInstance()->GetPuDataByUri(tDevUri);
}

CDomainData *GetDomainDataByDomainId(const string &strDomainId)
{
    return CCmuData::GetInstance()->GetDomainDataByDomainId(strDomainId);
}

CDomainData *GetDomainDataByDomainName(const string &strDomainName)
{
    return CCmuData::GetInstance()->GetDomainDataByDomainName(strDomainName);
}

CDomainData *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe)
{
    return CCmuData::GetInstance()->GetDomainDataByRedisPipe(pcRedisPipe);
}

CDomainData *GetLocalDomainData()
{
    return CCmuData::GetInstance()->GetLocalDomainData();
}

bool IsDomainInTopByDomainId(const string &strDomainId)
{
    return (NULL != CCmuData::GetInstance()->GetDomainDataByDomainId(strDomainId));
}

bool IsDomainInTopByDomainName(const string &strDomainName)
{
    return (NULL != CCmuData::GetInstance()->GetDomainDataByDomainName(strDomainName));
}

bool IsDomainInTopByDevUri(const string &strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    return (NULL != CCmuData::GetInstance()->GetDomainDataByDomainName(tDevUri.GetDomain()));
}

bool IsDomainInTopByDevUri(const TSipURI &tDevUri)
{
    return (NULL != CCmuData::GetInstance()->GetDomainDataByDomainName(tDevUri.GetDomain()));
}

TSipURI GetCmuUriFromDomainName(const string &strDomainName)
{
    TSipURI tCmuUri;
    tCmuUri.SetDomain(strDomainName);

    return tCmuUri.GetCmuURI();
}

TSipURI GetCmuUriFromDevUri(const string &strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    return tDevUri.GetCmuURI();
}

TSipURI GetCmuUriFromDevUri(const TSipURI &tDevUri)
{
    return tDevUri.GetCmuURI();
}

CTask *GetQueryTask(const string& strQueryTag)
{
    CTask* pcQryTask = NULL;
    g_cPuQueryTaskList.Find(strQueryTag, pcQryTask);

    return pcQryTask;
}

CCmuConnecterTask *GetConnectTaskByDomainId(const string &strDomainId)
{
    string strDomainName = DomainId2DomainName(strDomainId);

    return GetConnectTaskByDomainName(strDomainName);
}

CCmuConnecterTask *GetConnectTaskByDomainName(const string &strDomainName)
{
    TSipURI tCmuUri = GetCmuUriFromDomainName(strDomainName);

    CLoginSession *pcCmuTask = NULL;
    g_cCmuList.Find(tCmuUri.GetURIString(), pcCmuTask);

    return (CCmuConnecterTask *)pcCmuTask;
}

CLoginSession *GetLoginSession(const string &strSession)
{
    CLoginSession* ptLoginSession = NULL;
    if (IsLocalDomainByDevUri(strSession))
    {
        if (g_cCuList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

        if (g_cCuiList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

        CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(strSession);
        if (NULL != pcPuData)
        {
            TSipURI tDevUri(strSession);

            CPuRegBaseTask *pcRegTask = pcPuData->GetPuRegTask();
            return (NULL != pcRegTask ? pcRegTask->GetLoginSession(tDevUri.GetUser()) : NULL);
        }

        if (g_cPuiList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

        if (g_cVtduList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

        if (g_cGeneralLoginList.Find(strSession, ptLoginSession))
        {
            return ptLoginSession;
        }

		// 在redis中检查是否有该模块的信息
		return CRedisLoginSessionMng::GetInstance()->GetLoginSession(strSession);
    }
    else
    {
        TSipURI tCmuUri = GetCmuUriFromDevUri(strSession);
        g_cCmuList.Find(tCmuUri.GetURIString(), ptLoginSession, false);
        return ptLoginSession;
    }

    return ptLoginSession;
}

TSipURI GetPuRouter(const string &strPuUri)
{
    if (IsLocalDomainByDevUri(strPuUri))
    {
        TSipURI tPuiUri;
        CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(strPuUri);
        if (NULL != pcPuData)
        {
            CPuRegBaseTask *pcPuRegTask = pcPuData->GetPuRegTask();
            if (NULL != pcPuRegTask)
            {
                tPuiUri = pcPuRegTask->GetPuiUri();
            }
        }

        return tPuiUri; 
    }
    else        //异域设备
    {
        TSipURI tCmuUri = GetCmuUriFromDevUri(strPuUri);

        return tCmuUri;
    }
}

string GetDevRouter(const string &strSession, bool bRouteFollowTopo)
{
    TSipURI tSipDevUri;
    CLoginSession* ptLoginSession = NULL;

    if (IsLocalDomainByDevUri(strSession))
    {
        if (g_cCuList.Find(strSession, ptLoginSession))
        {
            return ((CCuLoginTask*)ptLoginSession)->GetCuiRegTask()->GetDevUri();
        }

        CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(strSession);
        if (NULL != pcPuData)
        {
            CPuRegBaseTask *pcTask = pcPuData->GetPuRegTask();
            if (NULL != pcTask)
            {
                return pcTask->GetPuiUri().GetURIString();
            }
        }

        if (g_cGeneralLoginList.Find(strSession, ptLoginSession))
        {
            if(!ptLoginSession->GetServerUri().empty() &&  g_cCmuConf.m_tLocalUri != ptLoginSession->GetServerUri())
            {
                return ptLoginSession->GetServerUri();
            }
        }

        return strSession;
    }
    else            // 异域设备
    {
        if (!bRouteFollowTopo)
        {
            TSipURI tCmuUri = GetCmuUriFromDevUri(strSession);

            return tCmuUri.GetURIString();
        }
        else
        {
            string strDomainName = CCmuData::GetInstance()->GetNextDomainNameToFromDevUri(strSession);
            TSipURI tCmuUri;
            tCmuUri.SetDomain(strDomainName);

            return tCmuUri.GetCmuURI().GetURIString();
        }
    }
}

string GetSsTypesStr(const TSsTypes &tSsTypes)
{
    return GetStrFromContainer(tSsTypes);
}

string GetSsersStr(const set<u32> &cSsers)
{
    return GetStrFromContainer(cSsers);
}

string IpListToStr(const CIpList& cIpList)
{
    return GetStrFromContainer(cIpList);
}

bool IsCmuSurpportEvent(u16 wEvent)
{
    //cmu内部消息
    if (wEvent >=CMU_INNER_EVENT_BEGIN && wEvent<= CMU_INNER_EVENT_END)
    {
        return true;
    }

    //ospsip消息
    if (wEvent >=OSP_SIP_EV_BEGIN && wEvent <= OSP_SIP_EV_END)
    {
        return true;
    }

    //cui、pui消息
    if (wEvent >=CMS_EVENT_BEGIN && wEvent <= CMS_EVENT_END)
    {
        return true;
    }

    //vtdu消息
    if (wEvent >=VTDU_EVENT_BEGIN && wEvent <= VTDU_EVENT_END)
    {
        return true;
    }

    //tas消息
    if (wEvent >=TAC_TAS_EVENT_BEGIN && wEvent <= TAC_TAS_EVENT_END)
    {
        return true;
    }

    //uas消息
    if (wEvent >=UAS_EVENT_BEGIN && wEvent <= UAS_EVENT_END)
    {
        return true;
    }

    return false;
}

//===============================================================================================================================

CPuData::CPuData(CPuDataList *pcPuList, const string &strDevId) : m_strDevId(strDevId), m_pcPuList(pcPuList), m_pcRegTask(NULL)
{
    CMS_ASSERT(NULL != pcPuList && "pcPuList = NULL!");

    m_bOnline = false;
}

CPuData::~CPuData()
{
    if (NULL != m_pcRegTask)
    {
        m_pcRegTask->ForceOffline(m_strDevId);
        m_pcRegTask = NULL;
    }
}

TSipURI CPuData::GetDevUri() const
{
    TSipURI tDevUri;
    tDevUri.SetUser(m_strDevId);
    tDevUri.SetDomain(m_pcPuList->GetDomainName());

    return tDevUri;
}

bool CPuData::IsPuInUse() const
{
    bool bInUse = false;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuInUse(m_strDevId, bInUse);

    return bInUse;
}

bool CPuData::IsPuExpired() const
{
    string strExpireDate;
    if (CCmuData::GetInstance()->GetLocalRedisPipe().GetExpireDate(m_strDevId, strExpireDate))
    {
        CCMSTime cTime;
        cTime.FromString(strExpireDate);

        return (time(NULL) > cTime.GetTime());        
    }

    return true;
}

CDeviceInfo CPuData::GetDevInfo() const
{
    CDeviceInfo cInfo;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevInfo(m_strDevId, cInfo);

    return cInfo;
}

s32 CPuData::GetDevType() const
{
    s32 nDevType = DEVICE_TYPE_INVALID;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevType(m_strDevId, nDevType);

    return nDevType;
}

string CPuData::GetDevTypeStr() const
{
    s32 nDevType = GetDevType();
    if (DEVICE_TYPE_ENCODER == nDevType)
    {
        return PU_ENCODER;
    }
    else if (DEVICE_TYPE_DECODER == nDevType)
    {
        return PU_DECODER;
    }
    else if (DEVICE_TYPE_CODECER == nDevType)
    {
        return PU_CODEC;
    }
    else
    {
        return PU_UNKNOWN;
    }
}

bool CPuData::GetDevGBFlag() const
{
    s32 nGBFlag = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevGBFlag(m_strDevId, nGBFlag);

    return (0 != nGBFlag);
}

string CPuData::GetDevName() const
{
    string strDevName;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevName(m_strDevId, strDevName);

    return strDevName;
}

string CPuData::GetDevAlias() const
{
    string strAlias;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevAlias(m_strDevId, strAlias);

    return strAlias;
}

string CPuData::GetDevNetAddr() const
{
    string strAddr;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevNetAddr(m_strDevId, strAddr);

    return strAddr;
}

string CPuData::GetPuConnectAddr() const
{
    string strAddr;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuConnectAddr(m_strDevId, strAddr);

    return strAddr;
}

string CPuData::GetDevModelId() const
{
    string strModelId;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevModelId(m_strDevId, strModelId);

    return strModelId;
}

string CPuData::GetDevManufacturer() const
{
    string strManufacturer;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDevManufacturer(m_strDevId, strManufacturer);

    return strManufacturer;
}

s32 CPuData::GetNetType() const
{
    s32 nNetType;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetNetType(m_strDevId, nNetType);

    return nNetType;
}

s32 CPuData::GetCreater() const
{
    s32 nCreater;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetCreater(m_strDevId, nCreater);

    return nCreater;
}

string CPuData::GetOwner() const
{
    string strOwner;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetOwner(m_strDevId, strOwner);

    return strOwner;
}

string CPuData::GetExpireDate() const
{
    string strExpireDate;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetExpireDate(m_strDevId, strExpireDate);

    return strExpireDate;
}

s32 CPuData::GetPuVidSrcNum() const
{
    s32 nVidSrcNum = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuVidSrcNum(m_strDevId, nVidSrcNum);

    return nVidSrcNum;
}

s32 CPuData::GetPuVidEncNum()
{
    s32 nVidEncNum = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuVidEncNum(m_strDevId, nVidEncNum);

    return nVidEncNum;
}

s32 CPuData::GetPuHdmiOutputNum() const
{
    s32 nHdmiOutputNum = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuHdmiOutputNum(m_strDevId, nHdmiOutputNum);

    return nHdmiOutputNum;
}

u8 CPuData::GetDevState() const
{
    if (NULL != m_pcRegTask)
    {
        if (m_pcRegTask->IsService() && GetPuIsOnline())
        {
            return 1;                   // 在线
        }
        else
        {
            return 2;                   // 虚拟注册
        }
    }
    else
    {
        return 0;                       // 离线
    }
}

string CPuData::GetKdmNO() const
{
    string strKdmNO;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetKdmNO(m_strDevId, strKdmNO);

    return strKdmNO;
}

bool CPuData::IsAllChnNotOnline(CPuChnStateList const *pcChnState) const
{
    map<TChannelID, TPuChnStateConfig> tPuChnState;

    if (NULL != pcChnState)
    {
        tPuChnState = *pcChnState;
    }
    else
    {
        tPuChnState = GetChnStateConfigSet();
    }
    
    bool bIsAllChnNotOnline = true;
    for (map<TChannelID, TPuChnStateConfig>::const_iterator it = tPuChnState.begin(); tPuChnState.end() != it; ++it)
    {
        if(it->second.GetChnState() == CHNSTATUS_ONLINE)
        {
            bIsAllChnNotOnline = false;
            break;
        }
    }

    return bIsAllChnNotOnline;  
}

TVideoEncodeParam CPuData::GetVideoEncodeParam(s32 nChnNO) const
{
    TVideoEncodeParam tParam;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetVideoEncodeParam(m_strDevId, nChnNO, tParam);

    return tParam;
}

TAudioEncodeParam CPuData::GetAudioEncodeParam(s32 nChnNO) const
{
    TAudioEncodeParam tParam;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetAudioEncodeParam(m_strDevId, nChnNO, tParam);

    return tParam;
}

TPuChnStateConfig CPuData::GetChnStateConfig(s32 nChnNO) const
{
    TPuChnStateConfig tChnState;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuChnState(m_strDevId, nChnNO, tChnState);

    return tChnState;
}

map<int, TPuChnStateConfig> CPuData::GetChnStateConfigSet() const
{
    map<int, TPuChnStateConfig> cChnStateSet;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuChnStateSet(m_strDevId, cChnStateSet);

    return cChnStateSet;
}

CVidSrcSet CPuData::GetChnVidSrc(s32 nChnNO) const
{
    CVidSrcSet cVidSrc;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetChnVidSrc(m_strDevId, nChnNO, cVidSrc);

    return cVidSrc;
}

map<int, CVidSrcSet> CPuData::GetChnVidSrcSet() const
{
    map<int, CVidSrcSet> cVidSrcSet;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetChnVidSrcSet(m_strDevId, cVidSrcSet);

    return cVidSrcSet;
}

CPuChnSet CPuData::GetVidSrcChn(s32 nVidSrcNO) const
{
    CPuChnSet cChn;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetVidSrcChn(m_strDevId, nVidSrcNO, cChn);

    return cChn;
}

map<int, CPuChnSet> CPuData::GetVidSrcChnSet() const
{
    map<int, CPuChnSet> cChnSet;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetVidSrcChnSet(m_strDevId, cChnSet);

    return cChnSet;
}

s32 CPuData::GetDirectTransNum() const
{
    s32 nTransNum = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDirectTransNum(m_strDevId, nTransNum);

    return nTransNum;
}

s32 CPuData::GetPuFreeLicenseNum()
{
    s32 nFreeNum = 0;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuFreeLicenseNum(m_strDevId, nFreeNum);

    return nFreeNum;
}

TPuBaseCapSet CPuData::GetBaseCapSet()
{
    TPuBaseCapSet tCapSet;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetBaseCapSet(m_strDevId, tCapSet);

    return tCapSet;
}

string CPuData::GetPuiIp()
{
    string strPuiIp;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuiIp(m_strDevId, strPuiIp);

    return strPuiIp;
}

TPuStatus CPuData::GetPuStatus() const
{
    TPuStatus tPuStatus;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuStatus(m_strDevId, tPuStatus);

    return tPuStatus;
}

bool CPuData::SetPuStatus(const TPuStatus &tPuStatus)
{
    if (m_bOnline && !tPuStatus.GetIsOnline())          // 由上线状态转为下线状态
    {
        SetOfflineTime();
    }

    if (tPuStatus.GetIsOnline())
    {
        CCmuData::GetInstance()->AddOnlineVidSrcNum(GetPuVidSrcNum());
    }
    else
    {
        CCmuData::GetInstance()->DelOnlineVidSrcNum(GetPuVidSrcNum());
    }

    m_bOnline = tPuStatus.GetIsOnline();
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuStatus(m_strDevId, tPuStatus);
}

bool CPuData::GetPuIsOnline() const
{
    return m_bOnline;

//     // 设备在线状态使用非常频繁，直接在内存中缓存以加快速度
//     return CCmuData::GetInstance()->GetLocalRedisPipe().GetPuIsOnline(m_strDevId);
}

bool CPuData::SetPuIsOnline(bool bOnline, bool bUpdateRedis)
{
    if (m_bOnline && !bOnline)                          // 由上线状态转为下线状态
    {
        SetOfflineTime();
    }

    if (bOnline)
    {
        CCmuData::GetInstance()->AddOnlineVidSrcNum(GetPuVidSrcNum());
    }
    else
    {
        CCmuData::GetInstance()->DelOnlineVidSrcNum(GetPuVidSrcNum());
    }

    m_bOnline = bOnline;

    if (bUpdateRedis)
    {
        return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuIsOnline(m_strDevId, bOnline);
    }
    
    return true;
}

bool CPuData::CleanupDevChnState()
{
    if (DEVICE_TYPE_ENCODER == GetDevType())
    {
        return CCmuData::GetInstance()->GetLocalRedisPipe().CleanupDevChnState(m_strDevId);
    }
    
    return true;
}

TPuConfig CPuData::GetPuConfig() const
{
    TPuConfig tPuConfig;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuConfig(m_strDevId, tPuConfig);

    return tPuConfig;
}

bool CPuData::MergePuConfig(TPuDevConfig &tDevConfig) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().MergePuConfig(m_strDevId, tDevConfig);
}

bool CPuData::SetPuConfig(TPuConfig &tPuConfig) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuConfig(m_strDevId, tPuConfig);
}

TPuDevConfig CPuData::GetPuDevConfig() const
{
    TPuDevConfig tPuDevConfig;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuDevConfig(m_strDevId, tPuDevConfig);

    return tPuDevConfig;
}

map<int,int> CPuData::GetExtCapSet() const
{
    map<int,int> tExtCapSet;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetExtCapSet(m_strDevId, tExtCapSet);

    return tExtCapSet;
}

bool CPuData::SetExtCapSet(const map<int,int> &cExtCapSet) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetExtCapSet(m_strDevId, cExtCapSet);
}

TPuServiceStatus CPuData::GetPuServiceStatus() const
{
    TPuServiceStatus tPuServiceStatus;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuServiceStatus(m_strDevId, tPuServiceStatus);

    return tPuServiceStatus;
}

bool CPuData::SetPuServiceStatus(const TPuServiceStatus &tPuServiceStatus) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuServiceStatus(m_strDevId, tPuServiceStatus);
}

bool CPuData::ClearPuServiceStatus() const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().ClearPuServiceStatus(m_strDevId);
}

bool CPuData::GetIsPuRec(s32 nChnNO) const
{
    bool bIsRec = false;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetIsPuRec(m_strDevId, nChnNO, bIsRec);

    return bIsRec;
}

bool CPuData::SetIsPuRec(s32 nChnNO, bool bIsPuRec) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetIsPuRec(m_strDevId, nChnNO, bIsPuRec);
}

bool CPuData::GetIsPuAudCall(s32 nChnNO, TChannel &tChn) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().GetPuAudioCallSrc(m_strDevId, nChnNO, tChn);
}

bool CPuData::SetIsPuAudCall(s32 nChnNO, const TChannel &tChn) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuAudioCallSrc(m_strDevId, nChnNO, tChn);
}

TPuAlarmStatus CPuData::GetPuAlarmStatus() const
{
    TPuAlarmStatus tPuAlarmStatus;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuAlarmStatus(m_strDevId, tPuAlarmStatus);

    return tPuAlarmStatus;
}

bool CPuData::SetAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetAlarmStatus(m_strDevId, tPuAlarmStatus);
}

bool CPuData::UpdateAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().UpdateAlarmStatus(m_strDevId, tPuAlarmStatus);
}

bool CPuData::UpdateAlarmInput(const TAlarmInput &tAlarmInput) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().UpdateAlarmInput(m_strDevId, tAlarmInput);
}

bool CPuData::ClearAlarmStatus() const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().ClearAlarmStatus(m_strDevId);
}

TTransparentData CPuData::GetPuTransparentData() const
{
    TTransparentData tTransData;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuTransparentData(m_strDevId, tTransData);

    return tTransData;
}

bool CPuData::SetPuTransparentData(const TTransparentData &tTransData) const
{
    return CCmuData::GetInstance()->GetLocalRedisPipe().SetPuTransparentData(m_strDevId, tTransData);
}

map<int, string> CPuData::GetVidSrcAliasMap() const
{
    map<int, string> cAliasMap;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetVidSrcAliasMap(m_strDevId, cAliasMap);

    return cAliasMap;
}

string CPuData::GetVidSrcAlias(s32 nVidSrcIdx) const
{
    string strAlias;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetVidSrcAlias(m_strDevId, nVidSrcIdx, strAlias);

    return strAlias;
}

map<int, string> CPuData::GetPuVidSrcAliasMap() const
{
    map<int, string> cAliasMap;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuVidSrcAliasMap(m_strDevId, cAliasMap);

    return cAliasMap;
}

string CPuData::GetPuVidSrcAlias(s32 nVidSrcIdx) const
{
    string strAlias;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetPuVidSrcAlias(m_strDevId, nVidSrcIdx, strAlias);

    return strAlias;
}

void CPuData::SetOfflineTime()
{
    if (GetDevType() == DEVICE_TYPE_ENCODER && g_cCmuConf.IsEnableCheckPuOfflineTime())
    {
        time_t tTime = time(NULL);
        m_cOfflineTimeList.push_back(tTime);

        ClearOutOfTimeRange();
    }
}

// 删除统计时间范围外的记录
void CPuData::ClearOutOfTimeRange()
{
    time_t tFirstChangeTime;
    time_t tCurTime = time(NULL);
    do 
    {
        if ( m_cOfflineTimeList.empty() )
        {
            break;
        }

        tFirstChangeTime = m_cOfflineTimeList[0];

        // 状态更改的时间
        if (m_cOfflineTimeList.size() > (u32)g_cCmuConf.m_nPuMaxOfflineTimes 
            || tFirstChangeTime < tCurTime - g_cCmuConf.m_nPuOfflineTimeRange)
        {
            m_cOfflineTimeList.erase(m_cOfflineTimeList.begin());
        }
        else
        {
            break;
        }

    } while (1);
}

// 获取是否在时间范围内有频繁的下线
bool CPuData::IsOfflineTooBrief(string &strContent)
{
    if (DEVICE_TYPE_ENCODER == GetDevType() && g_cCmuConf.IsEnableCheckPuOfflineTime())
    {
        ClearOutOfTimeRange();

        if (m_cOfflineTimeList.size() >= (u32)g_cCmuConf.m_nPuMaxOfflineTimes)
        {
            strContent = GetLanDesc(LAN_DESC_DEV_NAME) + "[" + GetDevName() + "]," 
                + GetLanDesc(LAN_DESC_DEVICE_ID) + "[" + GetDevUri().GetURIString() + "]";
            return true;
        }
    }

    return false;
}

TDeviceLogInfo CPuData::GetDevLog() 
{
    TDeviceLogInfo tLogInfo;

    tLogInfo.happenedTime = CCMSTime().ToString();

    tLogInfo.devUri = GetDevUri().GetURIString();
    CDeviceInfo cDeviceInfo = GetDevInfo();
    //modify by ljn 20170720
    tLogInfo.devName  = GetUTF8String(cDeviceInfo.GetDeviceName());
    tLogInfo.devAlias = GetUTF8String(cDeviceInfo.GetDeviceAlias());
    tLogInfo.netAddr = GetPuConnectAddr();
    tLogInfo.logType = (GetPuIsOnline() ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);
    tLogInfo.logDesc = (GetPuIsOnline() ? GetLanDesc(LAN_DESC_ONLINE) : GetLanDesc(LAN_DESC_OFFLINE));

    return tLogInfo;
}

TDeviceLogInfo CPuData::GetDevLog(const string &strDevName, const string &strDevAlias, const string &strPuConnectAddr)
{
    TDeviceLogInfo tLogInfo;

    tLogInfo.happenedTime = CCMSTime().ToString();

    tLogInfo.devUri = GetDevUri().GetURIString();
    tLogInfo.devName  = strDevName;
    tLogInfo.devAlias = strDevAlias;
    tLogInfo.netAddr = strPuConnectAddr;
    tLogInfo.logType = (GetPuIsOnline() ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);
    tLogInfo.logDesc = (GetPuIsOnline() ? GetLanDesc(LAN_DESC_ONLINE) : GetLanDesc(LAN_DESC_OFFLINE));

    return tLogInfo;
}

bool CPuData::IsNoneSipSserBySsType(const TSsType &tSsType)
{
    map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        if (!it->second.empty())
        {
            return false;
        }
    }

    return m_pcPuList->GetDomainData()->IsNoneCatalogSserBySsType(tSsType);
}

void CPuData::PublishOnOff(TSsMode tSsMode)
{
    PublishOnOff(GetPuStatus(), tSsMode);
}

void CPuData::PublishOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode)
{
    PublishRedisOnOff(tPuStatus, tSsMode);

    PublishSipOnOff(tPuStatus, tSsMode);
}

void CPuData::PublishConfig(TSsMode tSsMode)
{
    PublishConfig(GetPuDevConfig(), tSsMode);
}

void CPuData::PublishConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode)
{
    PublishRedisConfig(tPuDevConfig, tSsMode);

    PublishSipConfig(tPuDevConfig, tSsMode);
}

void CPuData::PublishService(TSsMode tSsMode)
{
    PublishService(GetPuServiceStatus(), tSsMode);
}

void CPuData::PublishService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode)
{
    PublishRedisService(tPuServiceStatus, tSsMode);

    PublishSipService(tPuServiceStatus, tSsMode);
}

void CPuData::PublishAlarm(TSsMode tSsMode)
{
    PublishAlarm(GetPuAlarmStatus(), tSsMode);
}

void CPuData::PublishAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode)
{
    PublishRedisAlarm(tPuAlarmStatus, tSsMode);

    PublishSipAlarm(tPuAlarmStatus, tSsMode);
}

void CPuData::PublishAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode)
{
    TPuAlarmStatus tPuAlarmStatus;
    tPuAlarmStatus.AddAlarmItem(tAlarmInput);

    PublishAlarm(tPuAlarmStatus, tSsMode);
}

void CPuData::PublishGpsData(TSsMode tSsMode)
{
    PublishGpsData(GetPuGpsData(), tSsMode);
}

void CPuData::PublishGpsData(const TGpsData &tGpsData, TSsMode tSsMode)
{
    PublishRedisGpsData(tGpsData, tSsMode);

    PublishSipGpsData(tGpsData, tSsMode);
}

void CPuData::PublishTransData(TSsMode tSsMode)
{
    PublishTransData(GetPuTransparentData(), tSsMode);
}

void CPuData::PublishTransData(const TTransparentData &tTransData, TSsMode tSsMode)
{
    PublishRedisTransData(tTransData, tSsMode);

    PublishSipTransData(tTransData, tSsMode);
}

void CPuData::PublishRedisOnOff(TSsMode tSsMode)
{
    PublishRedisOnOff(GetPuStatus(), tSsMode);
}

void CPuData::PublishRedisOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode)
{
    CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuStatus(
        m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tPuStatus, tSsMode);
}

void CPuData::PublishRedisConfig(TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuConfig(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, GetPuDevConfig(), tSsMode);
    }
}

void CPuData::PublishRedisConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        const CPuBaseCfgList &cBaseCfgList = tPuDevConfig.GetBaseCfgList();
        if (!cBaseCfgList.empty() && !cBaseCfgList[0].GetDevUri().empty())
        {
            CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuConfig(
                m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tPuDevConfig, tSsMode);
        }
        else
        {
            PublishRedisConfig(tSsMode);
        }
    }
}

void CPuData::PublishRedisService(TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        PublishRedisService(GetPuServiceStatus(), tSsMode);
    }
}

void CPuData::PublishRedisService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuServiceStatus(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tPuServiceStatus, tSsMode);
    }
}

void CPuData::PublishRedisAlarm(TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        PublishRedisAlarm(GetPuAlarmStatus(), tSsMode);
    }
}

void CPuData::PublishRedisAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuAlarmStatus(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tPuAlarmStatus, tSsMode);
    }
}

void CPuData::PublishRedisAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        TPuAlarmStatus tPuAlarmStatus;
        tPuAlarmStatus.GetAlarmArray().push_back(tAlarmInput);

        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuAlarmStatus(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tPuAlarmStatus, tSsMode);
    }
}

void CPuData::PublishRedisGpsData(TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        PublishRedisGpsData(GetPuGpsData(), tSsMode);
    }
}

void CPuData::PublishRedisGpsData(const TGpsData &tGpsData, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuGpsData(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tGpsData, tSsMode);
    }
}

void CPuData::PublishRedisTransData(TSsMode tSsMode)
{
    PublishRedisTransData(GetPuTransparentData(), tSsMode);
}

void CPuData::PublishRedisTransData(const TTransparentData &tTransData, TSsMode tSsMode)
{
    if (GetPuIsOnline())
    {
        CCmuData::GetInstance()->GetLocalRedisPipe().PublishPuTransparentData(
            m_pcPuList->GetDomainId(), m_pcPuList->GetDomainName(), m_strDevId, tTransData, tSsMode);
    }
}

void CPuData::PublishSipOnOff(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (!IsNoneSipSserBySsType(PU_ON_OFF_STATUS))
    {
        if (bGenContent)
        {
            PublishSipOnOff(GetPuStatus(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_ON_OFF_STATUS), cReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode, u32 dwTaskNO)
{
    if (!IsNoneSipSserBySsType(PU_ON_OFF_STATUS))
    {
        CPuStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        cNtfReq.SetOnOffStatus(tPuStatus);

        PublishSipNotify(PU_ON_OFF_STATUS, cNtfReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipConfig(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_CONFIG_STATUS))
    {
        if (bGenContent)
        {
            PublishSipConfig(GetPuDevConfig(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_CONFIG_STATUS), cReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_CONFIG_STATUS))
    {
        vector<TPuDevConfig> vctConfig;
        SplitPuConfig2ConfigNtyReq(tPuDevConfig, vctConfig);

        CPuConfigNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        for (vector<TPuDevConfig>::const_iterator it = vctConfig.begin(); vctConfig.end() != it; ++it)
        {
            cNtfReq.SetConfig(*it);
            PublishSipNotify(PU_CONFIG_STATUS, cNtfReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipService(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_SERVICE_STATUS))
    {
        if (bGenContent)
        {
            PublishSipService(GetPuServiceStatus(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_SERVICE_STATUS), cReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_SERVICE_STATUS))
    {
        CPuServiceStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        cNtfReq.SetServiceStatus(tPuServiceStatus);

        PublishSipNotify(PU_SERVICE_STATUS, cNtfReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipAlarm(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_ALARM_STATUS))
    {
        if (bGenContent)
        {
            PublishSipAlarm(GetPuAlarmStatus(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_ALARM_STATUS), cReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_ALARM_STATUS))
    {
        CPuAlarmStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        cNtfReq.SetAlarmStatus(tPuAlarmStatus);

        PublishSipNotify(PU_ALARM_STATUS, cNtfReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_ALARM_STATUS))
    {
        TPuAlarmStatus tPuAlarmStatus;
        tPuAlarmStatus.AddAlarmItem(tAlarmInput);

        PublishSipAlarm(tPuAlarmStatus, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipGpsData(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_GPS_STATUS))
    {
        /*
         * gpsdata是即时数据，不用关心每一次详细变化，其变更均采用发送通知时获取数据的方式，而不用在发布时就获取数据
         *
        if (bGenContent)
        {
            PublishSipGpsData(GetPuGpsData(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_GPS_STATUS), cReq, tSsMode, dwTaskNO);
        }
        */

        CEventReq cReq;
        PublishSipNotify(TSsType(PU_GPS_STATUS), cReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipGpsData(const TGpsData &tGpsData, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_GPS_STATUS))
    {
        /*
         * gpsdata是即时数据，不用关心每一次详细变化，其变更均采用发送通知时获取数据的方式，而不用在发布时就获取数据
         *
        CPuGpsStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        cNtfReq.SetGpsData(tGpsData);

        PublishSipNotify(PU_GPS_STATUS, cNtfReq, tSsMode, dwTaskNO);
        */

        CEventReq cReq;
        PublishSipNotify(TSsType(PU_GPS_STATUS), cReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishSipTransData(TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_TRANSDATA_STATUS))
    {
       if (bGenContent)
        {
            PublishSipTransData(GetPuTransparentData(), tSsMode, dwTaskNO);
        }
        else
        {
            CEventReq cReq;
            PublishSipNotify(TSsType(PU_TRANSDATA_STATUS), cReq, tSsMode, dwTaskNO);
        }
    }
}

void CPuData::PublishSipTransData(const TTransparentData &tTransData, TSsMode tSsMode, u32 dwTaskNO)
{
    if (GetPuIsOnline() && !IsNoneSipSserBySsType(PU_TRANSDATA_STATUS))
    {
        CPuTransDataStatusNtyReq cNtfReq;
        cNtfReq.SetDevURI(GetDevUri().GetURIString());
        cNtfReq.SetData(tTransData);

        PublishSipNotify(PU_TRANSDATA_STATUS, cNtfReq, tSsMode, dwTaskNO);
    }
}

void CPuData::PublishCertainSipSsTypes(const TSsTypes &tSsTypes, TSsMode tSsMode, u32 dwTaskNO, bool bGenContent)
{
    // 当需要发布多种通知类型时，保证上下线通知优先发布
    if (tSsTypes.end() != tSsTypes.find(PU_ON_OFF_STATUS))
    {
        PublishSipOnOff(tSsMode, dwTaskNO, bGenContent);
    }

    if (GetPuIsOnline())
    {
        for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
        {
            if (PU_ON_OFF_STATUS != *it)
            {
                if (PU_CONFIG_STATUS == *it)
                {
                    PublishSipConfig(tSsMode, dwTaskNO, bGenContent);
                }
                else if (PU_SERVICE_STATUS == *it)
                {
                    PublishSipService(tSsMode, dwTaskNO, bGenContent);
                }
                else if (PU_ALARM_STATUS == *it)
                {
                    PublishSipAlarm(tSsMode, dwTaskNO, bGenContent);
                }
                else if (PU_GPS_STATUS == *it)
                {
                    PublishSipGpsData(tSsMode, dwTaskNO, bGenContent);
                }
                else if (PU_TRANSDATA_STATUS == *it)
                {
                    PublishSipTransData(tSsMode, dwTaskNO, bGenContent);
                }
                else
                {
                    CMS_ASSERT(false && "unreachable...");
                }
            }
        }
    }
}

void CPuData::EnableGps()
{
	bool bIsGBFlag = GetDevGBFlag();
	s32  nCreater  = GetCreater();
	bool bIsGBDev  = bIsGBFlag || nCreater==CREATE_BY_GBS || nCreater==CREATE_BY_GBS_FOREIGN_DOMAIN;

	bool bGpsEnable = false;
	if (!bIsGBDev)
	{
		bGpsEnable = true;
	}
	if (bIsGBDev && g_cCmuConf.m_bGbDevGpsEnable)
	{
		bGpsEnable = true;
	}

	const map<int,int> cExtCapSet = GetExtCapSet();
    if (bGpsEnable && cExtCapSet.end() != cExtCapSet.find(VSIP_ENTITY_GPS))
    {
        //开启GPS
        SetPuGpsSs(true);
    }
}

void CPuData::DisableGps()
{
	bool bIsGBFlag = GetDevGBFlag();
	s32  nCreater  = GetCreater();
	bool bIsGBDev  = bIsGBFlag || nCreater==CREATE_BY_GBS || nCreater==CREATE_BY_GBS_FOREIGN_DOMAIN;

	bool bGpsEnable = false;
	if (!bIsGBDev)
	{
		bGpsEnable = true;
	}
	if (bIsGBDev && g_cCmuConf.m_bGbDevGpsEnable)
	{
		bGpsEnable = true;
	}

	const map<int,int> cExtCapSet = GetExtCapSet();
    if (bGpsEnable && cExtCapSet.end() != cExtCapSet.find(VSIP_ENTITY_GPS))
    {
        //关闭GPS
        SetPuGpsSs(false);
    }
}

void CPuData::SplitPuConfig2ConfigNtyReq(const TPuConfig &tPuConfig, vector<TPuDevConfig> &vctConfig)
{
    SplitPuConfig2ConfigNtyReq(tPuConfig.GetDevConfig(), vctConfig);
}

void CPuData::SplitPuConfig2ConfigNtyReq(const TPuDevConfig &tPuDevConfig, vector<TPuDevConfig> &vctConfig)
{
    TPuDevConfig tItem;

    //基本配置
    CPuBaseCfgList &tBaseCfgList = tItem.GetBaseCfgList();
    if (!tPuDevConfig.GetBaseCfgList().empty())
    {
        tBaseCfgList = tPuDevConfig.GetBaseCfgList();

        if (tItem.ToXml().size() >= NTF_BUF_SIZE_LIMIT)
        {
            vctConfig.push_back(tItem);
            tItem.Clear();
        }
    }

    if (!tBaseCfgList.empty() && tPuDevConfig.GetChnCfgList().empty() && tPuDevConfig.GetVidSrcCfgList().empty() && tPuDevConfig.GetChnStateCfgList().empty())
    {
        vctConfig.push_back(tItem);
        tItem.Clear();
        return;
    }

    //通道配置
    CPuChnCfgList& tChnCfgList = tItem.GetChnCfgList();
    const CPuChnCfgList& tWaitNtfChnCfgList = tPuDevConfig.GetChnCfgList();
    for (CPuChnCfgList::const_iterator iChn = tWaitNtfChnCfgList.begin(); 
        iChn != tWaitNtfChnCfgList.end(); ++iChn)
    {
        tChnCfgList[iChn->first] = iChn->second;

        if (tItem.ToXml().size() >= NTF_BUF_SIZE_LIMIT)
        {
            vctConfig.push_back(tItem);
            tItem.Clear();
        }
    }

    if (!tChnCfgList.empty() && tPuDevConfig.GetVidSrcCfgList().empty() && tPuDevConfig.GetChnStateCfgList().empty())
    {
        vctConfig.push_back(tItem);
        tItem.Clear();
        return;
    }

    //视频源配置
    CPuVidSrcCfgList& tVidSrcCfgList = tItem.GetVidSrcCfgList();
    const CPuVidSrcCfgList& tWaitNtfVidSrcCfgList = tPuDevConfig.GetVidSrcCfgList();
    for (CPuVidSrcCfgList::const_iterator iVidSrc = tWaitNtfVidSrcCfgList.begin(); 
        iVidSrc != tWaitNtfVidSrcCfgList.end(); ++iVidSrc)
    {
        tVidSrcCfgList[iVidSrc->first] = iVidSrc->second;

        if (tItem.ToXml().size() >= NTF_BUF_SIZE_LIMIT)
        {
            vctConfig.push_back(tItem);
            tItem.Clear();
        }
    }

    if (!tVidSrcCfgList.empty() && tPuDevConfig.GetChnStateCfgList().empty())
    {
        vctConfig.push_back(tItem);
        tItem.Clear();
        return;
    }

    //通道状态配置
    CPuChnStateList& tPuChnStateCfgList = tItem.GetChnStateCfgList();
    const CPuChnStateList& tWaitNtfCPuChnStateList = tPuDevConfig.GetChnStateCfgList();
    for (CPuChnStateList::const_iterator chnStateCfgItor = tWaitNtfCPuChnStateList.begin(); 
        chnStateCfgItor != tWaitNtfCPuChnStateList.end(); ++chnStateCfgItor)
    {
        tPuChnStateCfgList[chnStateCfgItor->first] = chnStateCfgItor->second;

        if (tItem.ToXml().size() >= NTF_BUF_SIZE_LIMIT)
        {
            vctConfig.push_back(tItem);
            tItem.Clear();
        }
    }

    if (!tPuChnStateCfgList.empty())
    {
        vctConfig.push_back(tItem);
        tItem.Clear();
    }
}

void CPuData::PrintData()
{
    OspPrintf(TRUE, FALSE, "  -----------------------------------入网信息-----------------------------------\n");
    string strOnOff = (GetPuIsOnline()) ? "online" : "offline";

    TPuStatus tPuStatus = GetPuStatus();

    string strSession;
    string strUsedPuiIp;
    u32 dwRegTask = INVALID_TASKNO;
    if (NULL != m_pcRegTask)
    {
        strSession   = m_pcRegTask->GetSession();
        strUsedPuiIp = m_pcRegTask->GetPuiIp();
        dwRegTask    = m_pcRegTask->GetTaskNO();
    }

    OspPrintf(TRUE, FALSE, "pu[%s]-taskNO[%u]-session[%s]-state[%s]-pustatus(InTop-Online-InUse)[%d-%d-%d]-puiIp[%s]\n", 
        m_strDevId.c_str(), dwRegTask, strSession.c_str(), 
        strOnOff.c_str(), tPuStatus.GetIsInTop(), tPuStatus.GetIsOnline(), 
        tPuStatus.GetIsUsed(), strUsedPuiIp.c_str());

    OspPrintf(TRUE, FALSE, "devName[%s] devType[%d] devIp[%s] dev model[%s] manufacturer[%s]\n",
        UTF8ToGBK(GetDevName()).c_str(), GetDevType(), GetDevNetAddr().c_str(),
        GetDevModelId().c_str(), GetDevManufacturer().c_str());

    OspPrintf(TRUE, FALSE, "netType[%d] expire Date[%s] creater[%d]\n", 
        GetNetType(), GetExpireDate().c_str(), GetCreater());

    if (g_bDebugOn)
    {
        OspPrintf(TRUE, FALSE, "gpsdata:[%s]\n", m_tGpsData.ToXml().c_str());
    }

    if (IsPuRegistered())            // 设备已注册则打印更详细的信息
    {
        GetPuConfig().PrintData();

        OspPrintf(TRUE, FALSE, "\n--------------------------------PU下线时间记录----------------------------------\n\n");

        OspPrintf(TRUE,FALSE, "统计设备频繁下线的时间范围[%d s], 最多下线[%d]次以后通知CU\n", 
            g_cCmuConf.m_nPuOfflineTimeRange, g_cCmuConf.m_nPuMaxOfflineTimes);

        CCMSTime cTime;
        for ( size_t i=0; i < m_cOfflineTimeList.size(); i++)
        {
            if (0 < m_cOfflineTimeList[i])
            {
                cTime.SetTime(m_cOfflineTimeList[i]);
                OspPrintf(TRUE,FALSE, "下线时间[%d] = [%s]\n", i, cTime.ToString().c_str());
            }
        }

        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU服务状态-----------------------------------\n\n");

        TPuServiceStatus tServiceStatus = GetPuServiceStatus();
        const map<int, TChannel>& tAudioCallSrc = tServiceStatus.GetAudioCallSrc();
        typedef map<int, TChannel>::const_iterator MI;
        for (MI it = tAudioCallSrc.begin(); it != tAudioCallSrc.end(); ++it)
        {
            OspPrintf(TRUE, FALSE, "chn[%d] AudioCallSrc[%s-%d]\n",
                it->first, it->second.GetDevUri().c_str(), it->second.GetChnNO());
        }

        const map<int, bool>& tPuRecStat = tServiceStatus.GetIsPuRec();
        typedef map<int, bool>::const_iterator MIT;
        for (MIT mIt = tPuRecStat.begin(); mIt != tPuRecStat.end(); ++mIt)
        {
            OspPrintf(TRUE, FALSE, "chn[%d] IsPuRecording[%d]\n",
                mIt->first, mIt->second);
        }

        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU服务状态-----------------------------------\n\n");

        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU告警信息-----------------------------------\n\n");

        TPuAlarmStatus tPuAlarmStatus = GetPuAlarmStatus();
        const CAlarmInputArray& tPuAlarmList = tPuAlarmStatus.GetAlarmArray();
        typedef CAlarmInputArray::const_iterator VI;
        for (VI it = tPuAlarmList.begin(); it != tPuAlarmList.end(); ++it)
        {
            OspPrintf(TRUE, FALSE, "dev[%s] [alarmType-%s] [inputId-%d] [flag-%s] [data-%s] [alarmTime-%s]\n",
                m_strDevId.c_str(), it->GetAlarmType().c_str(),
                it->GetInputId(), it->GetAlarmFlag().c_str(),
                it->GetAlarmData().c_str(), it->GetAlarmTime().c_str());
        }

        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU告警信息-----------------------------------\n\n");

        OspPrintf(TRUE, FALSE, "\n-----------------------------------视频源别名-----------------------------------\n\n");
        {
            map<int, string> cAliasMap = GetVidSrcAliasMap();
            s32 nIdx = 0;
            for (map<int, string>::const_iterator it = cAliasMap.begin(); cAliasMap.end() != it; ++it)
            {
                OspPrintf(TRUE, FALSE, "%4d. VidSrcId[%d], alias[%s]\n", 
                    ++nIdx, it->first, it->second.c_str());
            }     
        }   
        OspPrintf(TRUE, FALSE, "\n-----------------------------------视频源别名-----------------------------------\n\n");
    }
}

void CPuData::PrintSsList() const
{
    if (!m_cSsTaskList.empty())
    {
        OspPrintf(TRUE, FALSE, "\n设备[%s]的订阅者信息如下：\n", m_strDevId.c_str());
        for (map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
        {
            set<u32> cCatalogSser = m_pcPuList->GetDomainData()->GetCatalogSserBySsType(it->first);
            OspPrintf(TRUE, FALSE, "  Type[%-12s] ListSsers[%s] CatalogSsers[%s]\n", 
                it->first.c_str(), GetSsersStr(it->second).c_str(), GetSsersStr(cCatalogSser).c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "\n");
}

void CPuData::AddSipSsTask(u32 dwTaskNO, const TSsType &tSsType)
{
    CMS_ASSERT(INVALID_TASKNO != dwTaskNO && "invalid taskNO!!!");

    m_cSsTaskList[tSsType].insert(dwTaskNO);
}

void CPuData::AddSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes)
{
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        AddSipSsTask(dwTaskNO, *it);
    }
}

void CPuData::DelSipSsTask(u32 dwTaskNO)
{
    for (map<TSsType, set<u32> >::iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
    {
        it->second.erase(dwTaskNO);
    }
}

void CPuData::DelSipSsTask(u32 dwTaskNO, const TSsType &tSsType)
{
    map<TSsType, set<u32> >::iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        it->second.erase(dwTaskNO);
    }
}

void CPuData::DelSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes)
{
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        DelSipSsTask(dwTaskNO, *it);
    }
}

void CPuData::GetSipSserTasksBySsType(const TSsType &tSsType, set<u32> &cTasks) const
{
    map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        const set<u32> &cListSsTasks = it->second;

        set<u32> cDomainSsTasks;
        m_pcPuList->GetDomainData()->GetCatalogSserBySsType(tSsType, cDomainSsTasks);

        set_union(cListSsTasks.begin(), cListSsTasks.end(), cDomainSsTasks.begin(), cDomainSsTasks.end(), 
            inserter(cTasks, cTasks.begin()));
    }
    else
    {
        m_pcPuList->GetDomainData()->GetCatalogSserBySsType(tSsType, cTasks);
    }
}

void CPuData::PublishSipNotify(const TSsType &tSsType, CEventReq &cNtfReq, TSsMode tSsMode, u32 dwTaskNO)
{
    if (INVALID_TASKNO == dwTaskNO)
    {
        set<u32> cSsers;
        GetSipSserTasksBySsType(tSsType, cSsers);
        for (set<u32>::const_iterator it4Task = cSsers.begin(); cSsers.end() != it4Task; ++it4Task)
        {
            CTask *pcTask = GetWorkInst()->FindTask(*it4Task);
            if (NULL != pcTask)
            {
                CSsBaseTask *pcSsTask = (CSsBaseTask *)(pcTask);
                if (NULL != pcSsTask && (ALL_SS == tSsMode || tSsMode == pcSsTask->GetSsMode()))
                {
                    cNtfReq.SetSession(pcSsTask->GetSession());
                    pcSsTask->PostNotify(GetDevUri().GetURIString(), tSsType, cNtfReq);
                }
            }
        }
    }
    else
    {
        CTask *pcTask = GetWorkInst()->FindTask(dwTaskNO);
        if (NULL != pcTask)
        {
            CSsBaseTask *pcSsTask = (CSsBaseTask *)(pcTask);
            if (NULL != pcSsTask && (ALL_SS == tSsMode || tSsMode == pcSsTask->GetSsMode()))
            {
                cNtfReq.SetSession(pcSsTask->GetSession());
                pcSsTask->PostNotify(GetDevUri().GetURIString(), tSsType, cNtfReq);
            }
        }
    }
}

void CPuData::ProcRemovePu()
{
    set<u32> cSsers;
    for (map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
    {
        cSsers.insert(it->second.begin(), it->second.end());
    }

    for (set<u32>::const_iterator it = cSsers.begin(); cSsers.end() != it; ++it)
    {
        CSsBaseTask *pcTask = dynamic_cast<CSsBaseTask *>(GetWorkInst()->FindTask(*it));
        if (NULL != pcTask)
        {
            pcTask->ProcRemovePu(GetDevUri().GetURIString());
        }
    }
}

void CPuData::SetPuGpsSs(bool bIsEnable)
{
    CPuGpsSsSetTask* pcTask = new CPuGpsSsSetTask(GetWorkInst());
    CMS_ASSERT(NULL != pcTask && "new failed...");
    pcTask->InitStateMachine();

    TSipURI tDevUri = GetDevUri();
    CPuGpsSsSetReq cReq;
    cReq.SetDevUri(tDevUri.GetURIString());
    cReq.SetSession(cReq.GetDevUri());
    cReq.GetParam().SetEnable(bIsEnable);
    pcTask->SetReq(cReq);

    if (GetPuIsOnline())
    {
        if (NULL != m_pcRegTask)
        {
            pcTask->SendReq(cReq, m_pcRegTask->GetPuiUri());
        }
        else
        {
            pcTask->SendReq(cReq, GetCmuUriFromDevUri(tDevUri));
        }
    }
}

//===============================================================================================================================

CPuDataList::CPuDataList()
{
    
}

CPuDataList::~CPuDataList()
{
    vector<string> cPuIdList;
    GetPuIdList(cPuIdList);

    for (vector<string>::const_iterator it = cPuIdList.begin(); cPuIdList.end() != it; ++it)
    {
        DelPuData(*it);
    }
}

bool CPuDataList::Init(CDomainData *pcDomainData)
{
    CMS_ASSERT(NULL != pcDomainData && "pcDomainData = NULL!!!");
    m_pcDomainData = pcDomainData;

    return true;
}

CPuData *CPuDataList::AddPuData(const string &strDevId)
{
    CPuDataMap::iterator it = m_cPuList.find(strDevId);
    if (m_cPuList.end() == it)
    {
        pair<CPuDataMap::iterator, bool> cRet = 
            m_cPuList.insert(CPuDataMap::value_type(strDevId, CPuData(this, strDevId)));
        if (cRet.second)
        {
            CPuData *pcPuData = &(cRet.first->second);

            string strKdmNO = pcPuData->GetKdmNO();
            if (!strKdmNO.empty())
            {
                g_cPuIdUriMapList[strKdmNO] = pcPuData->GetDevUri().GetURIString();
            }

            return pcPuData;
        }
    }

    return NULL;
}

void CPuDataList::DelPuData(const string &strDevId)
{
    CPuDataMap::iterator it = m_cPuList.find(strDevId);
    if (m_cPuList.end() != it)
    {
        string strKdmNO = it->second.GetKdmNO();
        if (!strKdmNO.empty())
        {
            g_cPuIdUriMapList.erase(strKdmNO);
        }

        // 前端退网处理
        it->second.ProcRemovePu();

        m_cPuList.erase(it);
    }
}

void CPuDataList::ModPuData(const string &strDevId)
{
    // 现在设备信息都保存在redis中，对于修改通知，没有什么需要处理的，但是下面的情况需要处理下：
    // 本域的设备，从启用变更为停用，如果当前设备在线的话，需要踢下该设备
    CPuDataMap::iterator it = m_cPuList.find(strDevId);
    if (m_cPuList.end() != it)
    {
        CPuRegBaseTask *pcRegTask = it->second.GetPuRegTask();
        if (NULL != pcRegTask)
        {
            // 强制设备下线，注意，pcRegTask可能因此失效成为野指针
            // 本来修改设备的时候，只有修改特定参数需要强制设备下线的，但是现在3as发出来的设备修改通知中不包括旧数据，
            // 所以没有办法判断是哪些数据发生变更，只能设备修改了就一律强制下线
            pcRegTask->ForceOffline(strDevId);
            pcRegTask = NULL;
        }
    }
}

CPuData *CPuDataList::GetPuData(const string &strDevId)
{
    CPuDataMap::iterator it = m_cPuList.find(strDevId);
    if (m_cPuList.end() != it)
    {
        return &it->second;
    }

    return NULL;
}

u32 CPuDataList::GetOnlineVidSrcNum()
{
    u32 dwNum = 0;

    for (CPuDataMap::iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        CPuRegBaseTask *pcPuRegTask = it->second.GetPuRegTask();
        if (NULL != pcPuRegTask && pcPuRegTask->IsService())
        {
            dwNum += it->second.GetPuVidSrcNum();
        }
    }

    return dwNum;
}

u32 CPuDataList::GetOnlineVidSrcNum2()
{
    u32 dwNum = 0;

    for (CPuDataMap::iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        if (DEVICE_TYPE_ENCODER == it->second.GetDevType() && it->second.GetPuIsOnline())
        {
            CPuRegBaseTask *pcRegTask = it->second.GetPuRegTask();
            if (NULL != pcRegTask && pcRegTask->IsService())            //设备注册完成才计算在线视频源
            {
                map<int, CPuChnSet>  vidSrcChnSet = it->second.GetVidSrcChnSet();                     //视频源当前正在使用的通道列表
                map<TChannelID, TPuChnStateConfig> chnStatusSet = it->second.GetChnStateConfigSet();  //通道状态集

                map<int, CPuChnSet>::const_iterator itrVidSrc = vidSrcChnSet.begin();
                for (/*itrVidSrc*/; itrVidSrc != vidSrcChnSet.end(); itrVidSrc++)
                {
                    const CPuChnSet &cPuChnSet = itrVidSrc->second;
                    CPuChnSet::const_iterator itrCh = cPuChnSet.begin();
                    for (; itrCh != cPuChnSet.end(); itrCh++)
                    {
                        map<TChannelID, TPuChnStateConfig>::const_iterator itrChStat;
                        itrChStat = chnStatusSet.find(*itrCh);
                        if (itrChStat != chnStatusSet.end())
                        {
                            // 如果设备在线的话，视频源对应的通道只要有一个在线状态，视频源处于在线状态 [2/9/2015 pangubing]
                            if (itrChStat->second.GetChnState() == CHNSTATUS_ONLINE)
                            {
                                ++dwNum;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return dwNum;
}

TDeviceVideoSrcIdList CPuDataList::GetOfflineDevList(int& nOffNum, int& nIdleNum)
{
    TDeviceVideoSrcIdList tDevOffLinelist;

    for (CPuDataMap::iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        CPuData &cPuData = it->second;
        s32 nDevType = cPuData.GetDevType();
        if (DEVICE_TYPE_ENCODER == nDevType)
        {
            TDeviceVideoSrcId tTmpDevVidSrc;
            tTmpDevVidSrc.SetDeviceId(cPuData.GetDevUri().GetURIString());
            tTmpDevVidSrc.SetDeviceOnline(cPuData.GetPuIsOnline());

            CPuRegBaseTask *pcRegTask = cPuData.GetPuRegTask();
            if (NULL != pcRegTask && pcRegTask->IsService())        // 数据已上报完成
            {
                map<int,CPuChnSet> cVidSrcChnSet = cPuData.GetVidSrcChnSet();
                map<TChannelID, TPuChnStateConfig> cChnStatusSet = cPuData.GetChnStateConfigSet();

                map<int,CPuChnSet>::const_iterator itrVidSrc=cVidSrcChnSet.begin();
                for (;itrVidSrc!=cVidSrcChnSet.end();itrVidSrc++)
                {
                    tTmpDevVidSrc.SetVideoSrcId(itrVidSrc->first);
                    const CPuChnSet &cPuChnSet  = itrVidSrc->second;
                    CPuChnSet::const_iterator itrCh = cPuChnSet.begin();
                    if (cPuChnSet.end() != itrCh)//只需要判断视频源通道列表的第一通道（即主流）状态
                    {
                        map<TChannelID, TPuChnStateConfig>::const_iterator itrChStat;
                        itrChStat=cChnStatusSet.find(*itrCh);
                        if (itrChStat!=cChnStatusSet.end())
                        {

                            if (cPuData.GetPuIsOnline())
                            {
                                // 如果设备在线的话，视频源对应的第一通道（即主流）是下线状态，视频源处于下线状态
                                if (itrChStat->second.GetChnState()==CHNSTATUS_OFFLINE)
                                {
                                    nOffNum++;
                                    tTmpDevVidSrc.SetChnStat(CHNSTATUS_OFFLINE);
                                    tDevOffLinelist.push_back(tTmpDevVidSrc);
                                }// 如果设备在线的话，视频源对应的第一通道（即主流）是idle状态，视频源处于idle状态
                                else if(itrChStat->second.GetChnState()==CHNSTATUS_IDEL)
                                {
                                    nIdleNum++;
                                    tTmpDevVidSrc.SetChnStat(CHNSTATUS_IDEL);
                                    tDevOffLinelist.push_back(tTmpDevVidSrc);
                                }
                            }
                            else
                            {
                                // 如果设备不在线，则视频源处于下线状态
                                nOffNum++;
                                tTmpDevVidSrc.SetChnStat(CHNSTATUS_OFFLINE);
                                tDevOffLinelist.push_back(tTmpDevVidSrc);
                            }
                        }
                        else // 这里没有找到对应视频源，通道对应的状态，认为视频源不在线 [2/9/2015 pangubing]
                        {
                            nOffNum++;
                            tTmpDevVidSrc.SetChnStat(CHNSTATUS_OFFLINE);
                            tDevOffLinelist.push_back(tTmpDevVidSrc);
                        }
                    }
                }

                //如果上报视频源数量小于入网视频源数量，则剩余的视频源数量认定为IDEL。
                s32 nVidSrcNum = cPuData.GetPuVidSrcNum();
                for (s32 i = cVidSrcChnSet.size(); i < nVidSrcNum; ++i)
                {
                    ++nIdleNum;
                    tTmpDevVidSrc.SetVideoSrcId(i);
                    tTmpDevVidSrc.SetChnStat(CHNSTATUS_IDEL);
                    tDevOffLinelist.push_back(tTmpDevVidSrc);
                }
            }
            else            // 数据未上报则使用入网数据
            {
                s32 nVidSrcNum = cPuData.GetPuVidSrcNum();
                for (s32 i = 0; i < nVidSrcNum; ++i)
                {
                    ++nOffNum;
                    tTmpDevVidSrc.SetVideoSrcId(i);
                    tTmpDevVidSrc.SetChnStat(CHNSTATUS_OFFLINE);
                    tDevOffLinelist.push_back(tTmpDevVidSrc);
                }
            }
        }
    }

    return tDevOffLinelist;
}

string CPuDataList::GetDomainId()
{
    CMS_ASSERT(NULL != m_pcDomainData && "m_pcDomainData = NULL");

    return m_pcDomainData->GetDomainId();
}

string CPuDataList::GetDomainName()
{
    CMS_ASSERT(NULL != m_pcDomainData && "m_pcDomainData = NULL");

    return m_pcDomainData->GetDomainName();
}

CRedisPipe &CPuDataList::GetRedisPipe()
{
    CMS_ASSERT(NULL != m_pcDomainData && "m_pcDomainData = NULL");

    return m_pcDomainData->GetRedisPipe();
}

vector<string> CPuDataList::GetPuIdList() const
{
    vector<string> cPuIdList;
    cPuIdList.reserve(m_cPuList.size());
    for (CPuDataMap::const_iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        cPuIdList.push_back(it->first);
    }
	
	return cPuIdList;
}

void CPuDataList::GetPuIdList(vector<string> &cPuIdList) const
{
    cPuIdList.reserve(m_cPuList.size());
    for (CPuDataMap::const_iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        cPuIdList.push_back(it->first);
    }
}

void CPuDataList::GetOnlinePuIdList(set<string> &cPuIdList) const
{
    for (CPuDataMap::const_iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        if (it->second.GetPuIsOnline())
        {
            cPuIdList.insert(it->first);
        }
    }
}


void CPuDataList::PrintData(bool bSummaryInfo, u8 byState)
{
    OspPrintf(TRUE, FALSE, " \n");

    u32 dwOnlineNum = 0, dwSimuRegedNum = 0, dwTotalNum = 0, dwOthers = 0;
    map<string, u32> cOnlineNum;
    map<string, u32> cSimuRegedNum;

    GetDevNum(dwTotalNum, dwOnlineNum, dwSimuRegedNum, dwOthers, cOnlineNum, cSimuRegedNum);

    if (bSummaryInfo)
    {
        OspPrintf(TRUE, FALSE, "当前在线的pu：[%u]个\n", dwOnlineNum);
        OspPrintf(TRUE, FALSE, "当前虚拟注册的pu：[%u]个\n", dwSimuRegedNum);
        OspPrintf(TRUE, FALSE, "当前离线的pu：[%u]个\n", dwOthers);
        OspPrintf(TRUE, FALSE, "当前所有的pu：[%u]个\n", dwTotalNum); 

        if (g_bDebugOn)
        {
            OspPrintf(TRUE, FALSE, "\n\n\t各个pui接入设备信息统计如下：\n");

            Iterator pPos;
            string strPuiUri;
            CLoginSession* pTask = NULL;

            while (!pPos.End())
            {
                if (g_cPuiList.Iterate(pPos, strPuiUri, pTask))
                {
                    string strUri = strPuiUri;
                    if (DEV_TYPE_GBU == pTask->GetDevType())
                    {
                        CPuBatchRegTask *pcRegTask = ((CPuiRegTask *)pTask)->GetBatchRegTask();
                        if (NULL != pcRegTask)
                        {
                            strUri = pcRegTask->GetPuiUri().GetURIString();
                        }
                    }

                    u32 dwPuiOnlineNum = 0, dwPuiSimuRegedNum = 0;
                    map<string, u32>::const_iterator it4Online = cOnlineNum.find(strUri);
                    if (cOnlineNum.end() != it4Online)
                    {
                        dwPuiOnlineNum = it4Online->second;
                    }

                    map<string, u32>::const_iterator it4Simu = cSimuRegedNum.find(strUri);
                    if (cSimuRegedNum.end() != it4Simu)
                    {
                        dwPuiSimuRegedNum = it4Simu->second;
                    }

                    OspPrintf(TRUE, FALSE, "\tpui[%s]-type[%s]-task[%u]-online[%u]-simu[%u]\n", 
                        strUri.c_str(), pTask->GetDevType().c_str(), pTask->GetTaskNO(), dwPuiOnlineNum, dwPuiSimuRegedNum);
                }
            }

            OspPrintf(TRUE, FALSE, "\n");
        }

        return;
    }
    else
    {
        if (dwTotalNum > s_dwMaxPrintPuNum && !g_bDebugOn)
        {
            OspPrintf(TRUE, FALSE, "当前设备量太多，打印设备列表可能花费较多时间!请使用debug 1 命令确认后再打印...\n");
            return;
        }

        if (byState == 1)
        {
            OspPrintf(TRUE, FALSE, "当前在线的pu：[%u]个\n", dwOnlineNum);
        }
        else if (byState == 2)
        {
            OspPrintf(TRUE, FALSE, "当前虚拟注册的pu：[%u]个\n", dwSimuRegedNum);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "当前所有的pu：[%u]个\n", dwTotalNum); 
        }

        u32 dwCnt = 0;
        for (CPuDataMap::iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
        {
            CPuData *pcPuData = &it->second;

            CPuRegBaseTask* pcPuRegTask = pcPuData->GetPuRegTask();

            if (byState == 1)                // 打印在线的PU
            {
                if (pcPuData->GetPuIsOnline())
                {
                    OspPrintf(TRUE, FALSE, "  %4u. devId[%s]\n", ++dwCnt, it->first.c_str());
                }
            }
            else if (byState == 2)           // 打印虚拟注册的PU
            {
                if (pcPuRegTask != NULL && pcPuRegTask->IsService() && !pcPuData->GetPuIsOnline())
                {
                    OspPrintf(TRUE, FALSE, "  %4u. devId[%s]\n", ++dwCnt, it->first.c_str());
                }             
            }
            else                            // 打印所有状态的PU
            {
                OspPrintf(TRUE, FALSE, "  %4u. devId[%s]\n", ++dwCnt, it->first.c_str());
            }         
        }

        if (byState == 1)
        {
            OspPrintf(TRUE, FALSE, "当前在线的pu：[%u]个\n", dwOnlineNum);
        }
        else if (byState == 2)
        {
            OspPrintf(TRUE, FALSE, "当前虚拟注册的pu：[%u]个\n", dwSimuRegedNum);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "当前所有的pu：[%u]个\n", dwTotalNum); 
        }
    }
}

void CPuDataList::GetDevNum(u32 &dwTotalNum, u32 &dwOnlineNum, u32 &dwSimuRegedNum, u32 &dwOthers, 
    map<string, u32> &cOnlineNum, map<string, u32> &cSimuRegedNum)
{
    u32 dwCnt = 0;
    string strLocalDomainId = CCmuData::GetInstance()->GetLocalDomainId();
    for (CPuDataMap::iterator it = m_cPuList.begin(); m_cPuList.end() != it; ++it)
    {
        ++dwCnt;

        if (strLocalDomainId == m_pcDomainData->GetDomainId())
        {
            CPuRegBaseTask *pcRegTask = it->second.GetPuRegTask();
            if (NULL != pcRegTask && pcRegTask->IsService())
            {
                if (it->second.GetPuIsOnline())
                {
                    ++dwOnlineNum;

                    ++(cOnlineNum[pcRegTask->GetPuiUri().GetURIString()]);
                }
                else
                {
                    ++dwSimuRegedNum;

                    ++(cSimuRegedNum[pcRegTask->GetPuiUri().GetURIString()]);
                }
            } 
        }
        else
        {
            if (it->second.GetPuIsOnline())
            {
                ++dwOnlineNum;

                if (NULL != it->second.GetPuRegTask())
                {
                    ++(cOnlineNum[it->second.GetPuRegTask()->GetPuiUri().GetURIString()]);
                }
            }
        }
    }

    dwTotalNum = dwCnt;
    dwOthers   = dwTotalNum - dwOnlineNum - dwSimuRegedNum;
}



//===============================================================================================================================

CDomainData::CDomainData(const string &strDomainId, const string &strDomainName, DomainType emDomainType) 
    : m_strDomainId(strDomainId), m_emDomainType(emDomainType), m_strDomainName(strDomainName)
{
    m_pcConnectTask = NULL;
}

CDomainData::~CDomainData()
{
    if (NULL != m_pcConnectTask)
    {
        delete m_pcConnectTask;
    }
}

bool CDomainData::Connect()
{
    if (NULL == m_pcConnectTask)
    {
        m_pcConnectTask = new CCmuConnecterTask(GetWorkInst(), this);
        CMS_ASSERT(NULL != m_pcConnectTask && "m_pcConnectTask = NULL!!!");
        m_pcConnectTask->InitStateMachine();
    }

    m_pcConnectTask->SetDstUri(GetCmuUriFromDomainName(m_strDomainName));
    m_pcConnectTask->Connect();

    return true;
}

CDomainRelationInfo CDomainData::GetDomainRelationInfo()
{
    CDomainRelationInfo cInfo;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDomainRelationInfo(m_strDomainId, cInfo);

    return cInfo;
}

string CDomainData::GetDomainKdmNO() const
{
    string strKdmNO;
    CCmuData::GetInstance()->GetLocalRedisPipe().GetDomainKdmNO(m_strDomainId, strKdmNO);

    return strKdmNO;
}

bool CDomainData::IsNoneCatalogSserBySsType(const TSsType  &tSsType) const
{
    map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        return it->second.empty();
    }

    return true;
}

set<u32> CDomainData::GetCatalogSserBySsType(const TSsType  &tSsType) const
{
    set<u32> cTasks;
    map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        cTasks = it->second;
    }

    return cTasks;
}

void CDomainData::GetCatalogSserBySsType(const TSsType  &tSsType, set<u32> &cTasks) const
{
    map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        cTasks = it->second;
    }
}

void CDomainData::ExitCmuConnectTask(const CCmuConnecterTask *pcTask)
{
    if (m_pcConnectTask == pcTask)
    {
        m_pcConnectTask = NULL;
    }
}

void CDomainData::PrintData()
{
    CDomainRelationInfo cDomainInfo = GetDomainRelationInfo();

    OspPrintf(TRUE, FALSE, "DomainId[%s], DomainName[%s], ParentDomainId[%s], DomainType[%d], RedisConnect[%d]\n", 
        m_strDomainId.c_str(), cDomainInfo.GetDomainName().c_str(), 
        cDomainInfo.GetParentDomainID().c_str(), m_emDomainType, m_cRedisPipe.IsConnected());

    if (NULL != m_pcConnectTask)
    {
        m_pcConnectTask->PrintData();

        if (g_bDebugOn && m_pcConnectTask->GetIsSupportRedis())
        {
            OspPrintf(TRUE, FALSE, "RedisConnInfo[%s - %d - %s]\n", 
                m_cRedisPipe.GetRedisIp().c_str(), m_cRedisPipe.GetRedisPort(), m_cRedisPipe.GetRedisPassword().c_str());
        }
    }

    if (!m_cSsTaskList.empty())
    {
        OspPrintf(TRUE, FALSE, "\n域[%s - %s]的订阅者信息如下：\n", m_strDomainName.c_str(), m_strDomainId.c_str());
        for (map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
        {
            if (!it->second.empty())                // 过滤掉没有订阅者的类型
            {
                OspPrintf(TRUE, FALSE, "  Type[%-12s] CatalogSsers[%s]\n", it->first.c_str(), GetSsersStr(it->second).c_str());
            }
        }
    }

    if (g_bDebugOn)
    {
        OspPrintf(TRUE, FALSE, "\n\n-------------------------------RedisSdk------------------------------------------\n");

        m_cRedisPipe.GetRedisSdkPtr()->PrintData();
    }

    OspPrintf(TRUE, FALSE, "\n\n");
}

void CDomainData::ProcRemoveDomain()
{
    set<u32> cSsers;
    for (map<TSsType, set<u32> >::const_iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
    {
        cSsers.insert(it->second.begin(), it->second.end());
    }

    for (set<u32>::const_iterator it = cSsers.begin(); cSsers.end() != it; ++it)
    {
        CSsBaseTask *pcTask = dynamic_cast<CSsBaseTask *>(GetWorkInst()->FindTask(*it));
        if (NULL != pcTask)
        {
            pcTask->ProcRemoveDomain(m_strDomainName);
        }
    }
}

void CDomainData::AddCatalogSsTask(u32 dwTaskNO, const TSsType &tSsType)
{
    CMS_ASSERT(INVALID_TASKNO != dwTaskNO && "invalid taskNO!!!");

    m_cSsTaskList[tSsType].insert(dwTaskNO);
}

void CDomainData::AddCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes)
{
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        AddCatalogSsTask(dwTaskNO, *it);
    }
}

void CDomainData::DelCatalogSsTask(u32 dwTaskNO)
{
    for (map<TSsType, set<u32> >::iterator it = m_cSsTaskList.begin(); m_cSsTaskList.end() != it; ++it)
    {
        it->second.erase(dwTaskNO);
    }
}

void CDomainData::DelCatalogSsTask(u32 dwTaskNO, const TSsType &tSsType)
{
    map<TSsType, set<u32> >::iterator it = m_cSsTaskList.find(tSsType);
    if (m_cSsTaskList.end() != it)
    {
        it->second.erase(dwTaskNO);
    }
}

void CDomainData::DelCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes)
{
    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
    {
        DelCatalogSsTask(dwTaskNO, *it);
    }
}

//===============================================================================================================================

CDomainDataList::CDomainDataList() : m_pcLocalDomainData(NULL)
{

}

CDomainDataList::~CDomainDataList()
{
    /*
     * 本来CDomainDataList也可以像CPuDataList那样在析构的时候对每一个CDomainData调用DelDomainData来删除的
     * 但是CDomainDataList析构时一般是程序退出的时候，所以还是直接退出的好
     *
    vector<string> cDomainIdList;
    GetDomainIdList(cDomainIdList);

    for (vector<string>::const_iterator it = cDomainIdList.begin(); cDomainIdList.end() != it; ++it)
    {
        DelDomainData(*it);
    }
    */
}

bool CDomainDataList::Init(const string &strLocalDomainId, const string &strLocalDomainName, 
    const string &strIp, u16 wPort, const string &strPasswd)
{
    m_pcLocalDomainData = AddDomainData(strLocalDomainId, strLocalDomainName, CDomainData::DomainType_Local);
    if (NULL != m_pcLocalDomainData)
    {
        // 连接
        m_pcLocalDomainData->SetRedisParam(strIp, wPort, strPasswd);
        return m_pcLocalDomainData->GetRedisPipe().Connect();
    }

    return false;
}

bool CDomainDataList::InitDomainList()
{
    if (NULL != m_pcLocalDomainData)
    {
        vector<string> cDomainId;
        if (m_pcLocalDomainData->GetRedisPipe().GetDomainIdList(cDomainId))
        {
            for (vector<string>::const_iterator it = cDomainId.begin(); cDomainId.end() != it; ++it)
            {
                if (m_pcLocalDomainData->GetDomainId() != *it)        // 本域的DomainData在Init时已经创建
                {
                    CDomainData::DomainType emDomainType = GetDomainType(*it);
                    AddDomainData(*it, emDomainType);
                }
            }
        }

        return true;
    }

    return false;
}

bool CDomainDataList::InitDevList()
{
    if (NULL != m_pcLocalDomainData)
    {
        CRedisPipe &cRedisPipe = m_pcLocalDomainData->GetRedisPipe();

        vector<string> cDevId;
        if (m_pcLocalDomainData->GetRedisPipe().GetDevIdList(cDevId))
        {
            // 初始化所有设备及其通道状态
            m_pcLocalDomainData->GetRedisPipe().InitDevStatus(cDevId);

            string strDomainId;
            for (vector<string>::const_iterator it = cDevId.begin(); cDevId.end() != it; ++it)
            {
                if (m_pcLocalDomainData->GetRedisPipe().GetDevDomainId(*it, strDomainId))
                {
                    CDomainData *pcDomainData = GetDomainDataByDomainId(strDomainId);
                    if (NULL != pcDomainData)
                    {
                        pcDomainData->GetPuList().AddPuData(*it);
                    }
                }
            }

            return true;
        }
    }

    return false;
}

void CDomainDataList::Connect()
{
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        CDomainData *pcDomainData = &(it->second);
        if (m_pcLocalDomainData != pcDomainData)
        {
            pcDomainData->Connect();
        }
    }
}

string CDomainDataList::GetDomainNameById(const string &strDomainId)
{
    CDomainDataMap::const_iterator it = m_cDomainList.find(strDomainId);
    if (m_cDomainList.end() != it)
    {
        return it->second.GetDomainName();
    }

    return string();
}

string CDomainDataList::GetDomainIdByName(const string &strDomainName)
{
    for (CDomainDataMap::const_iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        if (it->second.GetDomainName() == strDomainName)
        {
            return it->second.GetDomainId();
        }
    }

    return string();
}

string CDomainDataList::GetDomainNameByNO(const TKDMNO &tDomainNO)
{
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        string strKdmNO = it->second.GetDomainKdmNO();
        if (strKdmNO == tDomainNO.GetNOStr())
        {
            return it->second.GetDomainName();
        }
    }

    return string();
}

CDomainData *CDomainDataList::AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType)
{
    string strDomainName;
    if (m_pcLocalDomainData->GetRedisPipe().GetDomainName(strDomainId, strDomainName))
    {
        return AddDomainData(strDomainId, strDomainName, emDomainType);
    }

    return NULL;
}

void CDomainDataList::DelDomainData(const string &strDomainId)
{
    CDomainDataMap::iterator it = m_cDomainList.find(strDomainId);
    if (m_cDomainList.end() != it)
    {
        it->second.ProcRemoveDomain();

        m_cDomainList.erase(it);
    }
}

CDomainData *CDomainDataList::GetDomainDataByDomainId(const string &strDomainId)
{
    CDomainDataMap::iterator it = m_cDomainList.find(strDomainId);
    if (m_cDomainList.end() != it)
    {
        return &(it->second);
    }

    return NULL;
}

CDomainData *CDomainDataList::GetDomainDataByDomainName(const string &strDomainName)
{
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        if (strDomainName == it->second.GetDomainName())
        {
            return &(it->second);
        }
    }

    return NULL;
}

CDomainData *CDomainDataList::GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe)
{
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        if (pcRedisPipe == &(it->second.GetRedisPipe()))
        {
            return &(it->second);
        }
    }

    return NULL;
}

CPuData *CDomainDataList::CheckPuData(const string &strDomainId, const string &strDevId)
{
    CPuData *pcPuData = NULL;

    CRedisPipe &cRedisPipe = m_pcLocalDomainData->GetRedisPipe();

    string strDevDomainId;
    if (cRedisPipe.GetDevDomainId(strDevId, strDevDomainId) && !strDevDomainId.empty() && strDomainId == strDevDomainId)
    {
        CDomainData *pcDomainData = GetDomainDataByDomainId(strDomainId);
        if (NULL != pcDomainData)
        {
            pcPuData = pcDomainData->GetPuList().AddPuData(strDevId);
        }
    }

    return pcPuData;
}

string CDomainDataList::GetNextDomainNameTo(const string &strDomainId)
{
    if (strDomainId == m_pcLocalDomainData->GetDomainId())
    {
        return m_pcLocalDomainData->GetDomainName();
    }

    string strDstDomainId           = strDomainId;

    //加个次数[域个数不会超过的值]保护，以防万一信息不对导致死循环
    const int MAX_DOMAIN_NUM = 1000;
    u16 wCnt = 0;

    do 
    {
        CDomainData *pcDomainData   = GetDomainDataByDomainId(strDstDomainId);
        if (NULL != pcDomainData)
        {
            CDomainRelationInfo cDomainInfo = pcDomainData->GetDomainRelationInfo();

            strDstDomainId      = cDomainInfo.GetParentDomainID();
            if (GetDomainUUID() == strDstDomainId)
            {
                return cDomainInfo.GetDomainName();
            }
        }
        else                                        // 没找到该域，说明要么该域不存在，要么是上级域，直接返回上级域名
        {
            return ::GetParentDomainName();
        }

    } while (++wCnt < MAX_DOMAIN_NUM);

    return string();
}

CDomainData::DomainType CDomainDataList::GetDomainType(const string &strDomainId)
{
    CMS_ASSERT(NULL != m_pcLocalDomainData && "m_pcLocalDomainData = NULL!!!");
    string strLocalDomainId = m_pcLocalDomainData->GetDomainId();
    if (strLocalDomainId == strDomainId)
    {
        return CDomainData::DomainType_Local;
    }
    else
    {
        string strTmpDomainId = strDomainId;

        const int MAX_DOMAIN_NUM = 1000;
        u16 wCnt = 0;

        do 
        {
            CDomainRelationInfo cInfo;
            if (m_pcLocalDomainData->GetRedisPipe().GetDomainRelationInfo(strTmpDomainId, cInfo))
            {
                strTmpDomainId = cInfo.GetParentDomainID();
                if (strLocalDomainId == cInfo.GetParentDomainID())
                {
                    return CDomainData::DomainType_Child;
                }
            }

        } while (!strTmpDomainId.empty() && ++wCnt < MAX_DOMAIN_NUM);

        return CDomainData::DomainType_Authorizer;
    }
}

u32 CDomainDataList::GetPuTotalNum() const
{
    u32 dwTotalNum = 0;
    for (CDomainDataMap::const_iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        dwTotalNum += it->second.GetPuTotalNum();
    }

    return dwTotalNum;
}

vector<string> CDomainDataList::GetDomainIdList() const
{
    vector<string> cDomainIdList;
    cDomainIdList.reserve(m_cDomainList.size());
    for (CDomainDataMap::const_iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        cDomainIdList.push_back(it->first);
    }

    return cDomainIdList;
}

void CDomainDataList::GetDomainIdList(vector<string> &cDomainIdList)
{
    cDomainIdList.reserve(m_cDomainList.size());
    for (CDomainDataMap::const_iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        cDomainIdList.push_back(it->first);
    }
}

void CDomainDataList::PrintData()
{
    OspPrintf(TRUE, FALSE, "\n---------------------------------DomainList-----------------------------------\n");

    s32 nIdx = 0;
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        CDomainRelationInfo cDomainInfo = it->second.GetDomainRelationInfo();
        OspPrintf(TRUE, FALSE, "%d. DomainId[%32s], DomainName[%16s], ParentDomainId[%32s], DomainType[%1d]\n", 
            ++nIdx, cDomainInfo.GetDomainID().c_str(), cDomainInfo.GetDomainName().c_str(), 
            cDomainInfo.GetParentDomainID().c_str(), it->second.GetDomainType());
    }

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
}

void CDomainDataList::PrintRedisPipeList()
{
    OspPrintf(TRUE, FALSE, "\n---------------------------------RedisList------------------------------------\n");

    s32 nIdx = 0;
    for (CDomainDataMap::iterator it = m_cDomainList.begin(); m_cDomainList.end() != it; ++it)
    {
        CRedisPipe &cRedisPipe = it->second.GetRedisPipe();

        OspPrintf(TRUE, FALSE, "%d. DomainName[%s], RedisConnect[%d], RedisSsChn[%s]\n", 
            ++nIdx, it->second.GetDomainName().c_str(), cRedisPipe.IsConnected(), GetStrFromContainer(cRedisPipe.GetSsChn()).c_str());
    }

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
}

CDomainData *CDomainDataList::AddDomainData(const string &strDomainId, 
    const string &strDomainName, CDomainData::DomainType emDomainType)
{
    CDomainDataMap::iterator it = m_cDomainList.find(strDomainId);
    if (m_cDomainList.end() == it)
    {
        pair<CDomainDataMap::iterator, bool> cRet = 
            m_cDomainList.insert(CDomainDataMap::value_type(strDomainId, CDomainData(strDomainId, strDomainName, emDomainType)));
        if (cRet.second)
        {
            CDomainData *pcDomainData = &(cRet.first->second);
            pcDomainData->GetPuList().Init(pcDomainData);

            return pcDomainData;
        }
    }

    return NULL;
}

//===============================================================================================================================

CCmuData *CCmuData::m_pcSingleton = NULL;

CCmuData::CCmuData()
{
    m_bInit = false;

    m_dwOnlineVidSrcNum = 0;
}

CCmuData::~CCmuData()
{

}

CCmuData *CCmuData::GetInstance()
{
    // 简单的实现单例模式，就不考虑多线程的问题了
    if (NULL == m_pcSingleton)
    {
        m_pcSingleton = new CCmuData();
        CMS_ASSERT(NULL != m_pcSingleton && "new failed...");
    }

    return m_pcSingleton;
}

bool CCmuData::Initialize(const string &strLocalDomainId, const string &strLocalDomainName, 
    const string &strIp, u16 wPort, const string &strPasswd)
{
    if (m_bInit)                // 避免重复初始化
    {
        return true;
    }

    do
    {
        // 1、cmu的数据结构初始化
        if (!m_cDomainList.Init(strLocalDomainId, strLocalDomainName, strIp, wPort, strPasswd))
        {
            cout << "数据初始化失败! DomainId[" << strLocalDomainId << "], IP[" << strIp << "], Port[" << wPort << "], Password[" << strPasswd << "]" << endl;
            break;
        }

        // 2、域列表初始化
        if (!m_cDomainList.InitDomainList())
        {
            cout << "域列表初始化失败! DomainId[" << strLocalDomainId << "], IP[" << strIp << "], Port[" << wPort << "], Password[" << strPasswd << "]" << endl;
            break;
        }

        // 3、设备初始化
        if (!m_cDomainList.InitDevList())
        {
            cout << "设备列表初始化失败! DomainId[" << strLocalDomainId << "], IP[" << strIp << "], Port[" << wPort << "], Password[" << strPasswd << "]" << endl;
            break;
        }
        
        // 4、初始化完成
        m_bInit = true;

        return true;

    } while (false);

    return false;
}

void CCmuData::StartWork()
{
    // 1、开启本域的订阅
    CDomainData *pcDomainData = m_cDomainList.GetLocalDomainData();
    CMS_ASSERT(NULL != pcDomainData && "pcDomainData = NULL!!!");

    // 初始的时候，订阅本域的所有数据信息
    vector<string> cSubList;
    cSubList.push_back(FDB_CHN_TAS_DEV_CHANGE);
    cSubList.push_back(FDB_CHN_TAS_DOMAIN_CHANGE);

    // 上下线状态本来就是cmu来设置的，所以没有必要去向redis订阅
    //cSubList.push_back(RedisGetOnOffCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetConfigCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetServiceCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetAlarmCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetGpsCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetTransDataCatalogSubChn(pcDomainData->GetDomainId()));
    cSubList.push_back(RedisGetDevConfigNtfCatalogSubChn(pcDomainData->GetDomainId()));
    pcDomainData->GetRedisPipe().AddSs(cSubList);

    // 2、开始连接所有的异域cmu
    m_cDomainList.Connect();
}

string CCmuData::GetLocalDomainId()
{
    return m_cDomainList.GetLocalDomainId();
}

string CCmuData::GetLocalDomainName()
{
    return m_cDomainList.GetLocalDomainName();
}

CDomainData *CCmuData::GetLocalDomainData()
{
    return m_cDomainList.GetLocalDomainData();
}

CDomainData *CCmuData::GetDomainDataByDomainId(const string &strDomainId)
{
    return m_cDomainList.GetDomainDataByDomainId(strDomainId);
}

CDomainData *CCmuData::GetDomainDataByDomainName(const string &strDomainName)
{
    return m_cDomainList.GetDomainDataByDomainName(strDomainName);
}

CDomainData *CCmuData::GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe)
{
    return m_cDomainList.GetDomainDataByRedisPipe(pcRedisPipe);
}

CPuData *CCmuData::GetPuData(const string &strDevId, bool bCheckRedis)
{
    CPuData *pcPuData = NULL;
    CDomainData *pcDomainData = m_cDomainList.GetLocalDomainData();
    if (NULL != pcDomainData)
    {
        pcPuData = pcDomainData->GetPuList().GetPuData(strDevId);

        if (NULL == pcPuData && bCheckRedis)
        {
            pcPuData = m_cDomainList.CheckPuData(m_cDomainList.GetLocalDomainId(), strDevId);
        }
    }

    return pcPuData;
}

CPuData *CCmuData::GetPuData(const string &strDomainId, const string &strDevId, bool bCheckRedis)
{
    CPuData *pcPuData = NULL;
    CDomainData *pcDomainData = m_cDomainList.GetDomainDataByDomainId(strDomainId);
    if (NULL != pcDomainData)
    {
        pcPuData = pcDomainData->GetPuList().GetPuData(strDevId);

        if (NULL == pcPuData && bCheckRedis)
        {
            pcPuData = m_cDomainList.CheckPuData(strDomainId, strDevId);
        }
    }

    return pcPuData;
}

CPuData *CCmuData::GetPuDataByUri(const string &strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    return GetPuData(DomainName2DomainId(tDevUri.GetDomain()), tDevUri.GetUser());
}

CPuData *CCmuData::GetPuDataByUri(const TSipURI &tDevUri)
{
    return GetPuData(DomainName2DomainId(tDevUri.GetDomain()), tDevUri.GetUser());
}

string CCmuData::DomainId2DomainName(const string &strDomainId)
{
    return m_cDomainList.GetDomainNameById(strDomainId);
}

string CCmuData::DomainName2DomainId(const string &strDomainName)
{
    return m_cDomainList.GetDomainIdByName(strDomainName);
}

string CCmuData::DomainNO2DomainName(const TKDMNO &tDomainNO)
{
    return m_cDomainList.GetDomainNameByNO(tDomainNO);
}

string CCmuData::GetNextDomainNameTo(const string &strDomainId)
{
    return m_cDomainList.GetNextDomainNameTo(strDomainId);
}

string CCmuData::GetNextDomainNameToFromDevUri(const string &strDevUri)
{
    TSipURI tDevUri;
    tDevUri.SetURIString(strDevUri);

    string strDomainId = DomainName2DomainId(tDevUri.GetDomain());

    return GetNextDomainNameTo(strDomainId);
}

string CCmuData::GetNextDomainNameToFromDevUri(const TSipURI &tDevUri)
{
    string strDomainId = DomainName2DomainId(tDevUri.GetDomain());

    return GetNextDomainNameTo(strDomainId);
}

/*
 * 虽然每一个CDomainData中都有一个CRedisPipe，但是实际的业务模型中，这个CRedisPipe只是用来接收订阅通知的
 * 当需要访问数据的时候，只能是通过本级的CRedisPipe
 */
CRedisPipe &CCmuData::GetLocalRedisPipe()
{
    CDomainData *pcDomainData = m_cDomainList.GetLocalDomainData();
    CMS_ASSERT(NULL != pcDomainData && "pcDomainData = NULL!!!");

    return pcDomainData->GetRedisPipe();
}

CDomainData *CCmuData::AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType)
{
    return m_cDomainList.AddDomainData(strDomainId, emDomainType);
}

void CCmuData::DelDomainData(const string &strDomainId)
{
    return m_cDomainList.DelDomainData(strDomainId);
}

void CCmuData::AddSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList, TSsDevList &tSsSuccessList, TSsErrorList &tSsErrorList)
{
    for (TSsDevList::const_iterator it = tSsDevList.begin(); tSsDevList.end() != it; ++it)
    {
        if (!it->second.empty())
        {
            TSipURI tDevUri;
            tDevUri.SetURIString(it->first);

            CDomainData *pcDomainData = GetDomainDataByDomainName(tDevUri.GetDomain());
            if (NULL != pcDomainData)
            {
                CPuData *pcPuData = pcDomainData->GetPuList().GetPuData(tDevUri.GetUser());
                if (NULL != pcPuData)
                {
                    pcPuData->AddSipSsTask(dwTaskNO, it->second);

                    tSsSuccessList.insert(TSsDevList::value_type(it->first, it->second));
                }
                else
                {
                    tSsErrorList.insert(TSsErrorList::value_type(it->first, ERR_CMU_PU_NOT_IN_TOP));
                }
            }
            else
            {
                tSsErrorList.insert(TSsErrorList::value_type(it->first, ERR_CMU_DOMAIN_NOT_IN_TOP));
            }
        }
        else
        {
            tSsErrorList.insert(TSsErrorList::value_type(it->first, ERR_CMU_SS_TYPES_INVALID));
        }        
    }
}

void CCmuData::DelSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList)
{
    for (TSsDevList::const_iterator it = tSsDevList.begin(); tSsDevList.end() != it; ++it)
    {
        CPuData *pcPuData = GetPuDataByUri(it->first);
        if (NULL != pcPuData)
        {
            pcPuData->DelSipSsTask(dwTaskNO, it->second);
        }
    }
}

s32 CCmuData::AddSipSsTask(u32 dwTaskNO, const string &strDomainName, const TSsTypes &tSsTypes)
{
    CDomainData *pcDomainData = GetDomainDataByDomainName(strDomainName);
    if (NULL != pcDomainData)
    {
        pcDomainData->AddCatalogSsTask(dwTaskNO, tSsTypes);

        return CMS_SUCCESS;
    }
    else
    {
        return ERR_CMU_DOMAIN_NOT_IN_TOP;
    }
}

void CCmuData::DelSipSsTask(u32 dwTaskNO, const string &strDomainName, const TSsTypes &tSsTypes)
{
    CDomainData *pcDomainData = GetDomainDataByDomainName(strDomainName);
    if (NULL != pcDomainData)
    {
        pcDomainData->DelCatalogSsTask(dwTaskNO, tSsTypes);
    }
}

u32 CCmuData::GetPuTotalNum() const
{
    return m_cDomainList.GetPuTotalNum();
}

void CCmuData::PrintDomainList()
{
    m_cDomainList.PrintData();
}

void CCmuData::PrintRedisPipeList()
{
    m_cDomainList.PrintRedisPipeList();
}

void CCmuData::PrintStatistics()
{
    OspPrintf(TRUE, FALSE, "OnlineVidSrcNum[%u]\n", m_dwOnlineVidSrcNum);
}

//===============================================================================================================================

void cbRedisSub(CFastDBInterface *pcInst, const string &strChn, const string &strData, void *pvUserData)
{
    CRedisPipe *pcPipe = reinterpret_cast<CRedisPipe *>(pvUserData);
    if (NULL != pcPipe)
    {
        TFastdbSubCB *ptCB = new TFastdbSubCB();
        if (NULL != ptCB)
        {
            ptCB->pcRedisPipe = pcPipe;
            ptCB->strChn  = strChn;
            ptCB->strData = strData;
    
            s32 nRet = ::OspPost(CMU_CORE_DAEMON, CMU_FASTDB_SUB_CB, &ptCB, sizeof(ptCB));
            if (0 != nRet)
            {
                delete ptCB;
                ptCB = NULL;
            }
        }
    }
}

void cbRedisSdkCallback(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3)
{
    switch (emMsg)
    {
    case RedisSdk_Msg_Redis_Disconnect:
        {
            const CRedisPipe *pcPipe = reinterpret_cast<const CRedisPipe *>(pvParam1);
            if (NULL != pcPipe)
            {
                TRedisSdkDiscCB *ptCB = new TRedisSdkDiscCB();
                if (NULL != ptCB)
                {
                    ptCB->pcRedisPipe = pcPipe;
                    ptCB->emCode      = *(reinterpret_cast<const EFDB_StatusCode *>(pvParam2));

                    s32 nRet = ::OspPost(CMU_CORE_DAEMON, CMU_REDISSDK_DISC_CB, &ptCB, sizeof(ptCB));
                    if (0 != nRet)
                    {
                        delete ptCB;
                        ptCB = NULL;
                    }
                }
            }
        }
        break;
    case RedisSdk_Msg_Subscribe_Notify:
        {
            const CRedisPipe *pcPipe = reinterpret_cast<const CRedisPipe *>(pvParam1);
            if (NULL != pcPipe)
            {
                TRedisSdkSubCB *ptCB = new TRedisSdkSubCB();
                if (NULL != ptCB)
                {
                    ptCB->pcRedisPipe = pcPipe;
                    ptCB->cSsChn      = *(reinterpret_cast<const CRedisSsChannel *>(pvParam2));
                    ptCB->strContent  = *(reinterpret_cast<const string *>(pvParam3));

                    s32 nRet = ::OspPost(CMU_CORE_DAEMON, CMU_REDISSDK_SUB_CB, &ptCB, sizeof(ptCB));
                    if (0 != nRet)
                    {
                        delete ptCB;
                        ptCB = NULL;
                    }
                }
            }
        }
        break;
    default:
        {
            GLOBELOG(CMU_MOD_REDIS, WARNING_LEV, "未知的RedisSdk回调[%d]，pvParam1[%p], pvParam2[%p], pvParam2[%p]！\n", 
                emMsg, pvParam1, pvParam2, pvParam3);
        }
        break;
    }
}


