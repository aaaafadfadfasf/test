/*========================================================================================
模块名    ：cmu
文件名    ：cmudata.h
相关文件  ：
实现功能  ：cmu设备数据管理
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_DATA_H
#define _CMU_DATA_H

#include <map>
#include <list>
#include <algorithm>
using namespace std;

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"
#include "cms/container/xqueue.h"
#include "cms/tas/tas_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/cmu/cmu_utility.h"
#include "cms/commonconfig/cmcfg.h"
#include "ssbasetask.h"
#include "cms/cmu/cmu_data.h"
#include "rediscomm.h"
#include "cms/tas/tas_redis.h"
#include "cms/cmu/kdmstruct.h"
#include "cmuconf.h"
#include "rediscomm.h"
#include "cmuconst.h"


class  CCmuCoreInst;
class  CPuRegBaseTask;
class  CLoginSession;
class  CCmuConnecterTask;
class  CDomainCatalogSserTask;
class  CDomainPuListSserTask;
class  CPuData;
class  CPuDataList;
class  CDomainData;
class  CDomainDataList;
class  CCmuData;
struct TSsUnit;

// class CPuSubscribeTask;
// class CPuListSubscribeTask;
// class CPuDataList;
// class CVtduRegTask;
// class CSsBaseTask;
// class CCatalogSubscribeTask;
// 


typedef string PUID, SIPURI;
typedef map<PUID, SIPURI> TPuIdUriMapList;
typedef CStrMap<CTask*>   CPuQueryTaskList;
extern CPuQueryTaskList g_cPuQueryTaskList;
extern TPuIdUriMapList  g_cPuIdUriMapList; //1.0puid和2.0puUri映射表
extern bool g_bDebugOn;

//===============================================================================================================================
// cmudata.h中提供的主要方法：
//===============================================================================================================================

TRedisModuleType DevType2RedisModuleType(const string &tDevType);
string RedisModuleType2DevType(const TRedisModuleType &tModuleType);

CCmuCoreInst*   GetWorkInst();
vector<CTask *> GetDomainSserTasksByDomainName(const string &strDomainName);

CRedisPipe &GetLocalRedisPipe();

string  GetDevDomainId(const string &strDevId);
TSipURI GetDevUriByDevId(const string &strDevId);

// 判断是否本域等
TSipURI GetLocalUri();
bool    IsLocalDomainByDomainId(const string &strDomainId);
bool    IsLocalDomainByDomainName(const string &strDomainName);
bool    IsLocalDomainByRedisPipe(const CRedisPipe *pcRedisPipe);
bool    IsLocalDomainByDevUri(const string &strDevUri);
bool    IsLocalDomainByDevUri(const TSipURI &tDevUri);

// 域名和域Id转换
string  DomainId2DomainName(const string &strDomainId);
string  DomainName2DomainId(const string &strDomainName);
string  DomainNO2DomainName(const TKDMNO &tDomainNO);

// 获取CPuData
CPuData     *GetPuData(const string &strDevId);
CPuData     *GetPuData(const string &strDomainId, const string &strDevId);
CPuData     *GetPuDataByUri(const string &strDevUri);
CPuData     *GetPuDataByUri(const TSipURI &tDevUri);
CDomainData *GetDomainDataByDomainId(const string &strDomainId);
CDomainData *GetDomainDataByDomainName(const string &strDomainName);
CDomainData *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe);
CDomainData *GetLocalDomainData();

// 判断指定域是否组网
bool    IsDomainInTopByDomainId(const string &strDomainId);
bool    IsDomainInTopByDomainName(const string &strDomainName);
bool    IsDomainInTopByDevUri(const string &strDevUri);
bool    IsDomainInTopByDevUri(const TSipURI &tDevUri);

// 获取cmu的uri
TSipURI GetCmuUriFromDomainName(const string &strDomainName);
TSipURI GetCmuUriFromDevUri(const string &strDevUri);
TSipURI GetCmuUriFromDevUri(const TSipURI &tDevUri);

// 路由相关
CTask             *GetQueryTask(const string& strQueryTag);
CCmuConnecterTask *GetConnectTaskByDomainId(const string &strDomainId);
CCmuConnecterTask *GetConnectTaskByDomainName(const string &strDomainName);
CLoginSession     *GetLoginSession(const string &strSession);
TSipURI            GetPuRouter(const string &strPuUri);
string             GetDevRouter(const string& strSession, bool bRouteFollowTopo = false);

// 其他
string GetSsTypesStr(const TSsTypes &tSsTypes);                // 将TSsTypes转换为字符串，以便打印
string GetSsersStr(const set<u32> &cSsers);                    // 将订阅者转换为字符串，以便打印
string IpListToStr(const CIpList& cIpList);                    // 将ip列表转换为字符串，以便打印
bool   IsCmuSurpportEvent(u16 wEvent);

template <typename T>
string GetStrFromContainer(const T &cContainer)
{
    ostringstream oss;
    for (typename T::const_iterator it = cContainer.begin(); cContainer.end() != it; ++it)
    {
        oss << *it << string(" ");
    }

    return oss.str();
}



struct TCmuStat
{
    TCmuStat()
    {
        bIsRegSipProxy = false;
        bIsConnectNms = false;
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\nCMU状态信息：\n");

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
        OspPrintf(TRUE, FALSE, "  CMU启动时间   :[%s]\n", strStartTime.c_str());
        OspPrintf(TRUE, FALSE, "  CMU状态       :[%s]\n", strCmuState.c_str());
        OspPrintf(TRUE, FALSE, "  是否注册PROXY :[%d]\n", bIsRegSipProxy);
        OspPrintf(TRUE, FALSE, "  注册TAS状态   :[%s]\n", strTasState.c_str());
        OspPrintf(TRUE, FALSE, "  是否连接NMS   :[%d]\n", bIsConnectNms);

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    }

    string strStartTime;
    string strCmuState;
    bool   bIsRegSipProxy;
    string strTasState;
    bool   bIsConnectNms;
};

// CPuData目前保存了设备的下线时间表、订阅关系，其他的设备数据已经放到了redis
class CPuData
{
    friend class CCmuData;
    friend class CPuDataList;

public:
    CPuData(CPuDataList *pcPuList, const string &strDevId);
    ~CPuData();

public:
    bool IsUTF8(IN unsigned char *pBuf, IN int strLen)
    {
        const unsigned char byMask6 = 0xFC;//11111100//6个字节
        const unsigned char byMask5 = 0xF8;//11111000//5个字节
        const unsigned char byMask4 = 0xF0;//11110000//4个字节
        const unsigned char byMask3 = 0xE0;//11100000//3个字节
        const unsigned char byMask2 = 0xC0;//11000000//2个字节
        const unsigned char byMask1 = 0x80;//10000000//小于此值的1个字节
        for (int i = 0; i < strLen; i++)
        {
            unsigned char uch = pBuf[i];//首字节
            if (uch == 0 && i < strLen - 1)
            {
                //不是结尾发现'\0'，那肯定不是utf8了
                return false;
            }
            if (uch < byMask1)
            {
                continue;
            }
            if ((uch & 0xC0) == 0x80)//首字节在 0x80 和0xc0之间，就是非法
            {
                return false;
            }
            for (int n = 2; n <= 6; n++)
            {
                //这个掩码表示的后面还有几个字节数
                int nNeedChar = 7 - n;
                //生成掩码
                unsigned char byMask = ((0xff >> n) << n);
                if (uch >= byMask)
                {
                    //检测剩余字节的数量是否符合utf8规则
                    if (strLen - i - 1 < nNeedChar)
                    {
                        return false;
                    }
                    //检测剩余字节数的数值是否符合utf8规则
                    for (int k = 0; k < nNeedChar; k++)
                    {
                        //每个字节都必须符合 10xxxxxx 格式
                        if ((pBuf[i + k + 1] & 0xC0) != 0x80)
                        {
                            return false;
                        }
                    }
                    //都符合了
                    i += nNeedChar;
                    break;
                }
            }

        }
        return true;
    }

    string GetUTF8String(const string& strOri)
    {
        bool bUTF8 = IsUTF8((u8 *)strOri.c_str(), strOri.length());
        return bUTF8 ? strOri : GBKToUTF8(strOri);
    }


    TSipURI GetDevUri() const;

    // 设备注册相关接口
    bool IsPuInUse() const;
    bool IsPuExpired() const;
    bool IsPuRegistered() const               { return (NULL != m_pcRegTask); }

    CPuRegBaseTask *GetPuRegTask()            { return m_pcRegTask;           }
    void SetPuRegTask(CPuRegBaseTask *pcTask) { m_pcRegTask = pcTask;         }

    // 设备入网相关数据接口
    CDeviceInfo GetDevInfo() const;                          // 获取设备的入网信息
    s32         GetDevType() const;
    string      GetDevTypeStr() const;
    bool        GetDevGBFlag() const;                        // 获取设备的国标标志
    string      GetDevName() const;
    string      GetDevAlias() const;
    string      GetDevNetAddr() const;
    string      GetPuConnectAddr() const;
    string      GetDevModelId() const;
    string      GetDevManufacturer() const;
    s32         GetNetType() const;
    s32         GetCreater() const;
    string      GetOwner() const;
    string      GetExpireDate() const;
    s32         GetPuVidSrcNum() const;
    s32         GetPuVidEncNum();
    s32         GetPuHdmiOutputNum() const;
    u8          GetDevState() const;
    string      GetKdmNO() const;
    bool        IsAllChnNotOnline(CPuChnStateList const *pcChnState = NULL) const;

    // PuConfig中常用字段的获取接口
    TVideoEncodeParam GetVideoEncodeParam(s32 nChnNO) const;
    TAudioEncodeParam GetAudioEncodeParam(s32 nChnNO) const;
    TPuChnStateConfig           GetChnStateConfig(s32 nChnNO) const;
    map<int, TPuChnStateConfig> GetChnStateConfigSet() const;
    CVidSrcSet           GetChnVidSrc(s32 nChnNO) const;
    map<int, CVidSrcSet> GetChnVidSrcSet() const;
    CPuChnSet            GetVidSrcChn(s32 nVidSrcNO) const;
    map<int, CPuChnSet>  GetVidSrcChnSet() const;
    s32                  GetDirectTransNum() const;

    // 设备注册license验证相关
    s32           GetPuFreeLicenseNum();
    TPuBaseCapSet GetBaseCapSet();
    string        GetPuiIp();

    // 设备状态接口
    TPuStatus   GetPuStatus() const;
    bool        SetPuStatus(const TPuStatus &tPuStatus);
    bool        GetPuIsOnline() const;
    bool        SetPuIsOnline(bool bOnline, bool bUpdateRedis = true);

    // 将设备的所有在线通道设置为下线状态
    bool        CleanupDevChnState();

    // PuConfig接口
    TPuConfig    GetPuConfig() const;                              // 获取设备的PuConfig
    bool         MergePuConfig(TPuDevConfig &tDevConfig) const;
    bool         SetPuConfig(TPuConfig &tPuConfig) const;
    TPuDevConfig GetPuDevConfig() const;
    map<int,int> GetExtCapSet() const;
    bool         SetExtCapSet(const map<int,int> &cExtCapSet) const;

    // 服务状态接口（前端录像状态、音频呼叫状态）
    TPuServiceStatus GetPuServiceStatus() const;
    bool             SetPuServiceStatus(const TPuServiceStatus &tPuServiceStatus) const;
    bool             ClearPuServiceStatus() const;
    bool GetIsPuRec(s32 nChnNO) const;
    bool SetIsPuRec(s32 nChnNO, bool bIsPuRec) const;
    bool GetIsPuAudCall(s32 nChnNO, TChannel &tChn) const;
    bool SetIsPuAudCall(s32 nChnNO, const TChannel &tChn) const;

    // 告警信息相关
    TPuAlarmStatus GetPuAlarmStatus() const;
    bool           SetAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const;
    bool           UpdateAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const;
    bool           UpdateAlarmInput(const TAlarmInput &tAlarmInput) const;
    bool           ClearAlarmStatus() const;

    // GPS数据
    const TGpsData GetPuGpsData() const { return m_tGpsData; }
    void SetPuGpsData(const TGpsData &tGpsData) { m_tGpsData = tGpsData; }

    // 透明数据
    TTransparentData GetPuTransparentData() const;
    bool SetPuTransparentData(const TTransparentData &tTransData) const;

    // 视频源别名
    map<int, string> GetVidSrcAliasMap() const;
    string           GetVidSrcAlias(s32 nVidSrcIdx) const;
    map<int, string> GetPuVidSrcAliasMap() const;
    string           GetPuVidSrcAlias(s32 nVidSrcIdx) const;

    // 上下线检测
    void SetOfflineTime();
    void ClearOutOfTimeRange();
    bool IsOfflineTooBrief(string &strContent);

    TDeviceLogInfo GetDevLog();
    TDeviceLogInfo GetDevLog(const string &strDevName, const string &strDevAlias, const string &strPuConnectAddr);

public:

    // 判断当前是否有Sip订阅者
    bool IsNoneSipSserBySsType(const TSsType &tSsType);

    // 订阅发布接口，包括redis发布和sip发布
    void PublishOnOff(TSsMode tSsMode = ALL_SS);
    void PublishOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode = ALL_SS);

    void PublishConfig(TSsMode tSsMode = ALL_SS);
    void PublishConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode = ALL_SS);

    void PublishService(TSsMode tSsMode = ALL_SS);
    void PublishService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode = ALL_SS);

    void PublishAlarm(TSsMode tSsMode = ALL_SS);
    void PublishAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode = ALL_SS);
    void PublishAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode = ALL_SS);

    void PublishGpsData(TSsMode tSsMode = ALL_SS);
    void PublishGpsData(const TGpsData &tGpsData, TSsMode tSsMode = ALL_SS);

    void PublishTransData(TSsMode tSsMode = ALL_SS);
    void PublishTransData(const TTransparentData &tTransData, TSsMode tSsMode = ALL_SS);

    // redis订阅发布接口
    void PublishRedisOnOff(TSsMode tSsMode = ALL_SS);
    void PublishRedisOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode = ALL_SS);

    // redis下的PuConfig通知必须发送全量通知，因为订阅者不再缓存PuConfig，发送增量的话，订阅者还要再从redis中取
    void PublishRedisConfig(TSsMode tSsMode = ALL_SS);
    void PublishRedisConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode = ALL_SS);

    void PublishRedisService(TSsMode tSsMode = ALL_SS);
    void PublishRedisService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode = ALL_SS);

    void PublishRedisAlarm(TSsMode tSsMode = ALL_SS);
    void PublishRedisAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode = ALL_SS);
    void PublishRedisAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode = ALL_SS);

    void PublishRedisGpsData(TSsMode tSsMode = ALL_SS);
    void PublishRedisGpsData(const TGpsData &tGpsData, TSsMode tSsMode = ALL_SS);

    void PublishRedisTransData(TSsMode tSsMode = ALL_SS);
    void PublishRedisTransData(const TTransparentData &tTransData, TSsMode tSsMode = ALL_SS);

    // sip订阅发布接口
    void PublishSipOnOff(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishSipConfig(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipConfig(const TPuDevConfig &tPuDevConfig, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishSipService(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipService(const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishSipAlarm(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipAlarm(const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);
    void PublishSipAlarm(const TAlarmInput &tAlarmInput, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishSipGpsData(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipGpsData(const TGpsData &tGpsData, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishSipTransData(TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);
    void PublishSipTransData(const TTransparentData &tTransData, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO);

    void PublishCertainSipSsTypes(const TSsTypes &tSsTypes, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO, bool bGenContent = true);

    // 对外提供的Gps接口，开启Gps或者关闭Gps
    void EnableGps();
    void DisableGps();

    // 拆包用，将PuDevConfig拆为多个CPuConfigNtyReq
    static void SplitPuConfig2ConfigNtyReq(const TPuConfig &tPuConfig,       vector<TPuDevConfig> &vctConfig);
    static void SplitPuConfig2ConfigNtyReq(const TPuDevConfig &tPuDevConfig, vector<TPuDevConfig> &vctConfig);

public:
    void PrintData();
    void PrintSsList() const;

protected:

    // 订阅相关（这几个接口只是添加、删除订阅关系，并非对外接口）
    void AddSipSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void AddSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);
    void DelSipSsTask(u32 dwTaskNO);
    void DelSipSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void DelSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);

    void GetSipSserTasksBySsType(const TSsType &tSsType, set<u32> &cTasks) const;
    void PublishSipNotify(const TSsType &tSsType, CEventReq &cNtfReq, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO); 

    void ProcRemovePu();

    // GPS操作相关
    void SetPuGpsSs(bool bIsEnable);

private:

    string                  m_strDevId;                      // 设备Id
    CPuDataList            *m_pcPuList;                      // CPuData所属的CPuDataList对象指针
    CPuRegBaseTask         *m_pcRegTask;                     // 设备对应的注册Task（目前有多种注册方式，单个注册和批量注册）
    vector<time_t>          m_cOfflineTimeList;              // 设备下线时间记录
    map<TSsType, set<u32> > m_cSsTaskList;                   // 订阅列表，包含每个订阅类型的所有订阅者信息

    // 以下数据在redis中已经保存了，但是由于在cmu内部会很频繁的访问，这里再保存一份，以空间换时间
    bool   m_bOnline;

    // gpsdata和transdata在redis中不实时更新，直接保存在内存中
    TGpsData m_tGpsData;    
};
typedef map<string, CPuData> CPuDataMap;                     // key为DevId

class CPuDataList
{
public:
    CPuDataList();
    ~CPuDataList();

    bool Init(CDomainData *pcDomainData);

    // PuData管理
    CPuData        *AddPuData(const string &strDevId);
    void            DelPuData(const string &strDevId);
    void            ModPuData(const string &strDevId);
    CPuData        *GetPuData(const string &strDevId);
    CPuDataMap     &GetPuDataMap() { return m_cPuList; }

    // 离线设备统计
    u32 GetOnlineVidSrcNum();
    u32 GetOnlineVidSrcNum2();
    TDeviceVideoSrcIdList GetOfflineDevList(s32 &nOffNum, s32 &nIdleNum);

    // 获取域相关信息
    string  GetDomainId();
    string  GetDomainName();
    CDomainData *GetDomainData() { return m_pcDomainData; }
    CRedisPipe &GetRedisPipe();

    vector<string> GetPuIdList() const;
    void GetPuIdList(vector<string> &cPuIdList) const;
    void GetOnlinePuIdList(set<string> &cPuIdList) const;

    u32 GetPuTotalNum() const { return m_cPuList.size(); }

public:
    // 调试打印相关
    void PrintData(bool bSummaryInfo = true, u8 byState = 0);
    void GetDevNum(u32 &dwTotalNum, u32 &dwOnlineNum, u32 &dwSimuRegedNum, u32 &dwOthers, 
        map<string, u32> &cOnlineNum, map<string, u32> &cSimuRegedNum);


private:
    CDomainData            *m_pcDomainData;                  // CPuDataList所属的CDomainData对象指针
    CPuDataMap              m_cPuList;                       // 使用CPuDataMap来存储设备数据
};

class CDomainData
{
    friend class CCmuData;
    friend class CDomainDataList;

public:
    enum DomainType { DomainType_Local, DomainType_Child, DomainType_Authorizer };
public:
    CDomainData(const string &strDomainId, const string &strDomainName, DomainType emDomainType);
    ~CDomainData();

    void SetDomainName(const string &strDomainName) { m_strDomainName = strDomainName; }
    void SetRedisParam(const string &strRedisIp, u16 wRedisPort, const string &strRedisPassword) { m_cRedisPipe.SetRedisParam(strRedisIp, wRedisPort, strRedisPassword); }
    bool IsRedisConnected() { return m_cRedisPipe.IsConnected(); }

    bool Connect();

    // 获取CPuDataList
    CPuDataList &GetPuList() { return m_cPuList; }

    // 获取DomainId、DomainName等
    const string &GetDomainId() const   { return m_strDomainId; }
    const string &GetDomainName() const { return m_strDomainName; }
    CDomainRelationInfo GetDomainRelationInfo();
    string GetDomainKdmNO() const;
    
    CRedisPipe &GetRedisPipe() { return m_cRedisPipe; }
    CCmuConnecterTask *GetConnectTask() { return m_pcConnectTask; }
    DomainType GetDomainType() const { return m_emDomainType; }
    
    u32  GetPuTotalNum() const { return m_cPuList.GetPuTotalNum(); }

    bool     IsNoneCatalogSserBySsType(const TSsType  &tSsType) const;
    set<u32> GetCatalogSserBySsType(const TSsType  &tSsType) const;
    void     GetCatalogSserBySsType(const TSsType  &tSsType, set<u32> &cTasks) const;

    void ExitCmuConnectTask(const CCmuConnecterTask *pcTask);

public:
    void PrintData();

protected:

    void ProcRemoveDomain();

    // 订阅相关（这几个接口只是添加、删除订阅关系，并非对外接口）
    void AddCatalogSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void AddCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);
    void DelCatalogSsTask(u32 dwTaskNO);
    void DelCatalogSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void DelCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);

private:

    string                  m_strDomainId;                   // 域Id
    CPuDataList             m_cPuList;                       // 使用CPuDataList来管理设备数据

    DomainType              m_emDomainType;                  // 域类型

    CRedisPipe              m_cRedisPipe;                    // redis管道，访问和订阅管道
    CCmuConnecterTask      *m_pcConnectTask;                 // 域间cmu连接Task

    map<TSsType, set<u32> > m_cSsTaskList;                   // 订阅列表，包含每个订阅类型的所有订阅者信息

    // 以下数据在redis中已经保存了，但是由于在cmu内部会很频繁的访问，这里再保存一份，以空间换时间
    string                  m_strDomainName;
};
typedef map<string, CDomainData> CDomainDataMap;             // key为DomainId

class CDomainDataList
{
public:
    CDomainDataList();
    ~CDomainDataList();

    // 本来将Init放在构造函数中是比较合适的，但是Init中要用到this，所以还是由使用者手动调用了
    bool Init(const string &strLocalDomainId, const string &strLocalDomainName, 
        const string &strIp, u16 wPort, const string &strPasswd);
    bool InitDomainList();
    bool InitDevList();

    // 连接到所有异域的cmu
    void Connect();

    // 获取本域的DomainId、DomainName
    string GetLocalDomainId()    { return m_pcLocalDomainData->GetDomainId();   }
    string GetLocalDomainName()  { return m_pcLocalDomainData->GetDomainName(); }

    // DomainName和DomainId之间的转换
    string GetDomainNameById(const string &strDomainId);
    string GetDomainIdByName(const string &strDomainName);
    string GetDomainNameByNO(const TKDMNO &tDomainNO);

    // 域数据管理，主要有增、删、查
    CDomainData *AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType);
    void         DelDomainData(const string &strDomainId);
    CDomainData *GetDomainDataByDomainId(const string &strDomainId);
    CDomainData *GetDomainDataByDomainName(const string &strDomainName);
    CDomainData *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe);
    CDomainData *GetLocalDomainData() { return m_pcLocalDomainData; }
    CPuData     *CheckPuData(const string &strDomainId, const string &strDevId);

    // 根据网络拓扑关系获取本域和指定域通信的域名
    string GetNextDomainNameTo(const string &strDomainId);

    CDomainData::DomainType GetDomainType(const string &strDomainId);

    u32  GetPuTotalNum() const;

    vector<string> GetDomainIdList() const;
    void GetDomainIdList(vector<string> &cPuIdList);

public:
    void PrintData();
    void PrintRedisPipeList();

private:
    CDomainData *AddDomainData(const string &strDomainId, const string &strDomainName, CDomainData::DomainType emDomainType);

private:
    CDomainData            *m_pcLocalDomainData;             // 本域的DomainData指针
    CDomainDataMap          m_cDomainList;                   // 使用CDomainDataMap来存储域信息  
};

// cmu全局数据，唯一的一份，使用单例模式
class CCmuData
{
private:
    CCmuData();
    ~CCmuData();

public:
    static CCmuData *GetInstance();                          // 获取CCmuData对象的指针，实现简单的单例模式

    bool Initialize(const string &strLocalDomainId, const string &strLocalDomainName, 
        const string &strIp, u16 wPort, const string &strPasswd);// 数据初始化
    void StartWork();

    // 域、设备相关简化接口
    string          GetLocalDomainId();
    string          GetLocalDomainName();
    CDomainData    *GetLocalDomainData();
    CDomainData    *GetDomainDataByDomainId(const string &strDomainId);
    CDomainData    *GetDomainDataByDomainName(const string &strDomainName);
    CDomainData    *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe);
    CPuData        *GetPuData(const string &strDevId, bool bCheckRedis = false);
    CPuData        *GetPuData(const string &strDomainId, const string &strDevId, bool bCheckRedis = false);
    CPuData        *GetPuDataByUri(const string &strDevUri);
    CPuData        *GetPuDataByUri(const TSipURI &tDevUri);

    string          DomainId2DomainName(const string &strDomainId);
    string          DomainName2DomainId(const string &strDomainName);
    string          DomainNO2DomainName(const TKDMNO &tDomainNO);

    string          GetNextDomainNameTo(const string &strDomainId);
    string          GetNextDomainNameToFromDevUri(const string &strDevUri);
    string          GetNextDomainNameToFromDevUri(const TSipURI &tDevUri);

    CRedisPipe     &GetLocalRedisPipe();

    // 域操作接口
    CDomainData * AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType);
    void DelDomainData(const string &strDomainId);

    // 订阅相关简化接口
    void AddSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList, TSsDevList &tSsSuccessList, TSsErrorList &tSsErrorList);           // 适用于列表订阅
    void DelSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList);
    s32  AddSipSsTask(u32 dwTaskNO, const string &strDomainName, const TSsTypes &tSsTypes);                                          // 适用于目录订阅
    void DelSipSsTask(u32 dwTaskNO, const string &strDomainName, const TSsTypes &tSsTypes);

    u32  GetOnlineVidSrcNum()                   { return m_dwOnlineVidSrcNum; }
    void AddOnlineVidSrcNum(u16 wVidSrcNum)     { m_dwOnlineVidSrcNum += wVidSrcNum; }
    void DelOnlineVidSrcNum(u16 wVidSrcNum)     { if (m_dwOnlineVidSrcNum > wVidSrcNum) m_dwOnlineVidSrcNum -= wVidSrcNum; }

    u32  GetPuTotalNum() const;

public:
    void PrintDomainList();
    void PrintRedisPipeList();
    void PrintStatistics();

private:
    static CCmuData      *m_pcSingleton;
    bool                  m_bInit;
    CDomainDataList       m_cDomainList;

    // 一些统计数据
    u32                   m_dwOnlineVidSrcNum;
};


//===============================================================================================================================

struct TFastdbSubCB
{
    const CRedisPipe *pcRedisPipe;
    string     strChn;
    string     strData;
};

struct TRedisSdkDiscCB
{
    const CRedisPipe *pcRedisPipe;
    EFDB_StatusCode emCode;
};

struct TRedisSdkSubCB
{
    const CRedisPipe *pcRedisPipe;
    CRedisSsChannel cSsChn;
    string          strContent;
};

// Redis断链回调函数和订阅回调函数
void cbRedisDisc(CFastDBInterface *, EFDB_StatusCode, void *, void *);
void cbRedisSub(CFastDBInterface *, const string &, const string &, void *);
inline void cbRedisLog(int nLogLevel, const char *szMsg, void *pvUserData) { GLOBELOG(CMU_MOD_REDIS, nLogLevel, szMsg); }

void cbRedisSdkCallback(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3);


//===============================================================================================================================

template<class TDataType>
class CArrayList
{
    struct TNode
    {
        TNode()
        {
            ptNext = NULL;
        }

        TDataType tData;
        TNode* ptNext;
    };
public:
    CArrayList()
    {
        m_ptFree = NULL;
        m_ptHead = NULL;
        m_nMaxSize = 0;
        m_nFreeSize = 0;
    }

    ~CArrayList()
    {
        if (m_ptHead)
        {
            delete[] m_ptHead;
            m_ptHead = NULL;
        }
    }

    BOOL32 Init(s32 nSize)
    {
        if (nSize <= 0)
        {
            return FALSE;
        }

        m_nFreeSize = m_nMaxSize = nSize;

        m_ptFree = m_ptHead = new TNode[nSize];
        if (NULL == m_ptFree)
        {
            CLASSLOG(ERROR_LEV, "m_ptFree == NULL\n");
            return FALSE;
        }

        m_ptFree[0].tData = 0;
        for (s32 i = 1; i < nSize; i++)
        {
            m_ptFree[i].tData = i;
            m_ptFree[i - 1].ptNext = &m_ptFree[i];
        }
        m_ptFree[nSize - 1].ptNext = NULL;

        return TRUE;
    }

    TDataType Pop()
    {
        if (m_ptFree == NULL)
        {
            CLASSLOG(ERROR_LEV, "m_ptFree == NULL\n");
            return 0;
        }

        m_nFreeSize--;
        TNode* ptAlloc = m_ptFree;

        m_ptFree = m_ptFree->ptNext;
        ptAlloc->ptNext = NULL;

        return ptAlloc->tData;
    }

    void PushHead(TDataType tVar)
    {
        if (tVar > m_nMaxSize || tVar < 0)
        {
            return;
        }

        m_nFreeSize++;
        TNode* ptNode = &m_ptHead[tVar];

        ptNode->ptNext = m_ptFree;
        m_ptFree = ptNode;
    }

    u32 GetFreeSize()
    {
        return m_nFreeSize;
    }

    void PrintData() const
    {
        OspPrintf(TRUE, FALSE, "m_nMaxSize: [%d], m_nFreeSize: [%d]\n[", m_nMaxSize, m_nFreeSize);
        for (TNode* ptItr = m_ptFree; ptItr != NULL; ptItr = ptItr->ptNext)
        {
            OspPrintf(TRUE, FALSE, "%d ", ptItr->tData);
        }
        OspPrintf(TRUE, FALSE, "]\n");
    }

private:
    TNode* m_ptFree;
    TNode* m_ptHead;
    u32	   m_nMaxSize;
    u32	   m_nFreeSize;
};

//nms告警相关
//流水号池，保证分配流水号唯一性
class CIndexPool
{
public:
    enum
    {
        MAX_INDEX = 0xFFFF,
        INVALID_INDEX = 0xFFFF,
    };
public:
    CIndexPool(u16 wMaxIndex = MAX_INDEX)
    {
        Init(wMaxIndex);
    }

    ~CIndexPool()
    {
        m_cIndexQue.Empty();
        m_cIndexSet.Empty();
    }

public:

    u16 PopIndex()
    {
        u16 wIndex = INVALID_INDEX;
        if (!m_cIndexQue.Pop(wIndex))
        {
            return INVALID_INDEX;
        }

        if (!m_cIndexSet.Erase(wIndex))
        {
            return INVALID_INDEX;
        }

        return wIndex;
    }

    void PushIndex(u16 wIndex)
    {
        if (m_cIndexSet.Exist(wIndex))
        {
            return;
        }

        m_cIndexSet.Insert(wIndex);

        m_cIndexQue.Push(wIndex);
    }

private:
    void Init(u16 wMaxIndex)
    {       
        for(u16 i=0; i<wMaxIndex; i++)
        {
            m_cIndexQue.Push(i);
            m_cIndexSet.Insert(i);
        }		
    }

private:
    CXQueue<u16> m_cIndexQue;  //快速存取Index
    CXSet<u16> m_cIndexSet;    //保证Index的唯一性
};

struct TNmsAlarm
{
    TNmsAlarm()
    {
        dwAlarmCode = 0;
        dwAlarmSN = 0;
        byAlarmStatus = 0;
    }

    u32	dwAlarmCode;
    u32 dwAlarmSN;
    u8 byAlarmStatus;
    string strModuleName;
    string strModuleId;
    string strModuleIp;
};

struct TNmsAlarmSrc
{
    TNmsAlarmSrc()
    {
        dwAlarmCode = 0;
    }

    bool operator == ( const TNmsAlarmSrc& tOther ) const
    {
        return (strAlarmDev == tOther.strAlarmDev 
            && dwAlarmCode == tOther.dwAlarmCode);
    }

    string strAlarmDev;
    u32 dwAlarmCode;
};

template <> struct CHashFunctor<TNmsAlarmSrc>
{
    size_t operator()(const TNmsAlarmSrc& key) const
    {
        string strKey = key.strAlarmDev;
        u32 dwAlarmCode = key.dwAlarmCode;
        char szAlarmCode[64] = {0};
        sprintf(szAlarmCode, "%u", (unsigned int)dwAlarmCode);
        strKey += szAlarmCode;

        return hash_ms(strKey.c_str(), 0);
    }
};

template <> struct CHashFunctor<TChannel>
{
	size_t operator()(const TChannel& key) const
	{
		return hash_ms(key.ToString().c_str(), 0);
	}
};



#endif  //#ifndef _CMU_DATA_H