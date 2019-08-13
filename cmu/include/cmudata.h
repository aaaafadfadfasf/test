/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmudata.h
����ļ�  ��
ʵ�ֹ���  ��cmu�豸���ݹ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
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
extern TPuIdUriMapList  g_cPuIdUriMapList; //1.0puid��2.0puUriӳ���
extern bool g_bDebugOn;

//===============================================================================================================================
// cmudata.h���ṩ����Ҫ������
//===============================================================================================================================

TRedisModuleType DevType2RedisModuleType(const string &tDevType);
string RedisModuleType2DevType(const TRedisModuleType &tModuleType);

CCmuCoreInst*   GetWorkInst();
vector<CTask *> GetDomainSserTasksByDomainName(const string &strDomainName);

CRedisPipe &GetLocalRedisPipe();

string  GetDevDomainId(const string &strDevId);
TSipURI GetDevUriByDevId(const string &strDevId);

// �ж��Ƿ����
TSipURI GetLocalUri();
bool    IsLocalDomainByDomainId(const string &strDomainId);
bool    IsLocalDomainByDomainName(const string &strDomainName);
bool    IsLocalDomainByRedisPipe(const CRedisPipe *pcRedisPipe);
bool    IsLocalDomainByDevUri(const string &strDevUri);
bool    IsLocalDomainByDevUri(const TSipURI &tDevUri);

// ��������Idת��
string  DomainId2DomainName(const string &strDomainId);
string  DomainName2DomainId(const string &strDomainName);
string  DomainNO2DomainName(const TKDMNO &tDomainNO);

// ��ȡCPuData
CPuData     *GetPuData(const string &strDevId);
CPuData     *GetPuData(const string &strDomainId, const string &strDevId);
CPuData     *GetPuDataByUri(const string &strDevUri);
CPuData     *GetPuDataByUri(const TSipURI &tDevUri);
CDomainData *GetDomainDataByDomainId(const string &strDomainId);
CDomainData *GetDomainDataByDomainName(const string &strDomainName);
CDomainData *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe);
CDomainData *GetLocalDomainData();

// �ж�ָ�����Ƿ�����
bool    IsDomainInTopByDomainId(const string &strDomainId);
bool    IsDomainInTopByDomainName(const string &strDomainName);
bool    IsDomainInTopByDevUri(const string &strDevUri);
bool    IsDomainInTopByDevUri(const TSipURI &tDevUri);

// ��ȡcmu��uri
TSipURI GetCmuUriFromDomainName(const string &strDomainName);
TSipURI GetCmuUriFromDevUri(const string &strDevUri);
TSipURI GetCmuUriFromDevUri(const TSipURI &tDevUri);

// ·�����
CTask             *GetQueryTask(const string& strQueryTag);
CCmuConnecterTask *GetConnectTaskByDomainId(const string &strDomainId);
CCmuConnecterTask *GetConnectTaskByDomainName(const string &strDomainName);
CLoginSession     *GetLoginSession(const string &strSession);
TSipURI            GetPuRouter(const string &strPuUri);
string             GetDevRouter(const string& strSession, bool bRouteFollowTopo = false);

// ����
string GetSsTypesStr(const TSsTypes &tSsTypes);                // ��TSsTypesת��Ϊ�ַ������Ա��ӡ
string GetSsersStr(const set<u32> &cSsers);                    // ��������ת��Ϊ�ַ������Ա��ӡ
string IpListToStr(const CIpList& cIpList);                    // ��ip�б�ת��Ϊ�ַ������Ա��ӡ
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
        OspPrintf(TRUE, FALSE, "\nCMU״̬��Ϣ��\n");

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
        OspPrintf(TRUE, FALSE, "  CMU����ʱ��   :[%s]\n", strStartTime.c_str());
        OspPrintf(TRUE, FALSE, "  CMU״̬       :[%s]\n", strCmuState.c_str());
        OspPrintf(TRUE, FALSE, "  �Ƿ�ע��PROXY :[%d]\n", bIsRegSipProxy);
        OspPrintf(TRUE, FALSE, "  ע��TAS״̬   :[%s]\n", strTasState.c_str());
        OspPrintf(TRUE, FALSE, "  �Ƿ�����NMS   :[%d]\n", bIsConnectNms);

        OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    }

    string strStartTime;
    string strCmuState;
    bool   bIsRegSipProxy;
    string strTasState;
    bool   bIsConnectNms;
};

// CPuDataĿǰ�������豸������ʱ������Ĺ�ϵ���������豸�����Ѿ��ŵ���redis
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
        const unsigned char byMask6 = 0xFC;//11111100//6���ֽ�
        const unsigned char byMask5 = 0xF8;//11111000//5���ֽ�
        const unsigned char byMask4 = 0xF0;//11110000//4���ֽ�
        const unsigned char byMask3 = 0xE0;//11100000//3���ֽ�
        const unsigned char byMask2 = 0xC0;//11000000//2���ֽ�
        const unsigned char byMask1 = 0x80;//10000000//С�ڴ�ֵ��1���ֽ�
        for (int i = 0; i < strLen; i++)
        {
            unsigned char uch = pBuf[i];//���ֽ�
            if (uch == 0 && i < strLen - 1)
            {
                //���ǽ�β����'\0'���ǿ϶�����utf8��
                return false;
            }
            if (uch < byMask1)
            {
                continue;
            }
            if ((uch & 0xC0) == 0x80)//���ֽ��� 0x80 ��0xc0֮�䣬���ǷǷ�
            {
                return false;
            }
            for (int n = 2; n <= 6; n++)
            {
                //��������ʾ�ĺ��滹�м����ֽ���
                int nNeedChar = 7 - n;
                //��������
                unsigned char byMask = ((0xff >> n) << n);
                if (uch >= byMask)
                {
                    //���ʣ���ֽڵ������Ƿ����utf8����
                    if (strLen - i - 1 < nNeedChar)
                    {
                        return false;
                    }
                    //���ʣ���ֽ�������ֵ�Ƿ����utf8����
                    for (int k = 0; k < nNeedChar; k++)
                    {
                        //ÿ���ֽڶ�������� 10xxxxxx ��ʽ
                        if ((pBuf[i + k + 1] & 0xC0) != 0x80)
                        {
                            return false;
                        }
                    }
                    //��������
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

    // �豸ע����ؽӿ�
    bool IsPuInUse() const;
    bool IsPuExpired() const;
    bool IsPuRegistered() const               { return (NULL != m_pcRegTask); }

    CPuRegBaseTask *GetPuRegTask()            { return m_pcRegTask;           }
    void SetPuRegTask(CPuRegBaseTask *pcTask) { m_pcRegTask = pcTask;         }

    // �豸����������ݽӿ�
    CDeviceInfo GetDevInfo() const;                          // ��ȡ�豸��������Ϣ
    s32         GetDevType() const;
    string      GetDevTypeStr() const;
    bool        GetDevGBFlag() const;                        // ��ȡ�豸�Ĺ����־
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

    // PuConfig�г����ֶεĻ�ȡ�ӿ�
    TVideoEncodeParam GetVideoEncodeParam(s32 nChnNO) const;
    TAudioEncodeParam GetAudioEncodeParam(s32 nChnNO) const;
    TPuChnStateConfig           GetChnStateConfig(s32 nChnNO) const;
    map<int, TPuChnStateConfig> GetChnStateConfigSet() const;
    CVidSrcSet           GetChnVidSrc(s32 nChnNO) const;
    map<int, CVidSrcSet> GetChnVidSrcSet() const;
    CPuChnSet            GetVidSrcChn(s32 nVidSrcNO) const;
    map<int, CPuChnSet>  GetVidSrcChnSet() const;
    s32                  GetDirectTransNum() const;

    // �豸ע��license��֤���
    s32           GetPuFreeLicenseNum();
    TPuBaseCapSet GetBaseCapSet();
    string        GetPuiIp();

    // �豸״̬�ӿ�
    TPuStatus   GetPuStatus() const;
    bool        SetPuStatus(const TPuStatus &tPuStatus);
    bool        GetPuIsOnline() const;
    bool        SetPuIsOnline(bool bOnline, bool bUpdateRedis = true);

    // ���豸����������ͨ������Ϊ����״̬
    bool        CleanupDevChnState();

    // PuConfig�ӿ�
    TPuConfig    GetPuConfig() const;                              // ��ȡ�豸��PuConfig
    bool         MergePuConfig(TPuDevConfig &tDevConfig) const;
    bool         SetPuConfig(TPuConfig &tPuConfig) const;
    TPuDevConfig GetPuDevConfig() const;
    map<int,int> GetExtCapSet() const;
    bool         SetExtCapSet(const map<int,int> &cExtCapSet) const;

    // ����״̬�ӿڣ�ǰ��¼��״̬����Ƶ����״̬��
    TPuServiceStatus GetPuServiceStatus() const;
    bool             SetPuServiceStatus(const TPuServiceStatus &tPuServiceStatus) const;
    bool             ClearPuServiceStatus() const;
    bool GetIsPuRec(s32 nChnNO) const;
    bool SetIsPuRec(s32 nChnNO, bool bIsPuRec) const;
    bool GetIsPuAudCall(s32 nChnNO, TChannel &tChn) const;
    bool SetIsPuAudCall(s32 nChnNO, const TChannel &tChn) const;

    // �澯��Ϣ���
    TPuAlarmStatus GetPuAlarmStatus() const;
    bool           SetAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const;
    bool           UpdateAlarmStatus(const TPuAlarmStatus &tPuAlarmStatus) const;
    bool           UpdateAlarmInput(const TAlarmInput &tAlarmInput) const;
    bool           ClearAlarmStatus() const;

    // GPS����
    const TGpsData GetPuGpsData() const { return m_tGpsData; }
    void SetPuGpsData(const TGpsData &tGpsData) { m_tGpsData = tGpsData; }

    // ͸������
    TTransparentData GetPuTransparentData() const;
    bool SetPuTransparentData(const TTransparentData &tTransData) const;

    // ��ƵԴ����
    map<int, string> GetVidSrcAliasMap() const;
    string           GetVidSrcAlias(s32 nVidSrcIdx) const;
    map<int, string> GetPuVidSrcAliasMap() const;
    string           GetPuVidSrcAlias(s32 nVidSrcIdx) const;

    // �����߼��
    void SetOfflineTime();
    void ClearOutOfTimeRange();
    bool IsOfflineTooBrief(string &strContent);

    TDeviceLogInfo GetDevLog();
    TDeviceLogInfo GetDevLog(const string &strDevName, const string &strDevAlias, const string &strPuConnectAddr);

public:

    // �жϵ�ǰ�Ƿ���Sip������
    bool IsNoneSipSserBySsType(const TSsType &tSsType);

    // ���ķ����ӿڣ�����redis������sip����
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

    // redis���ķ����ӿ�
    void PublishRedisOnOff(TSsMode tSsMode = ALL_SS);
    void PublishRedisOnOff(const TPuStatus &tPuStatus, TSsMode tSsMode = ALL_SS);

    // redis�µ�PuConfig֪ͨ���뷢��ȫ��֪ͨ����Ϊ�����߲��ٻ���PuConfig�����������Ļ��������߻�Ҫ�ٴ�redis��ȡ
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

    // sip���ķ����ӿ�
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

    // �����ṩ��Gps�ӿڣ�����Gps���߹ر�Gps
    void EnableGps();
    void DisableGps();

    // ����ã���PuDevConfig��Ϊ���CPuConfigNtyReq
    static void SplitPuConfig2ConfigNtyReq(const TPuConfig &tPuConfig,       vector<TPuDevConfig> &vctConfig);
    static void SplitPuConfig2ConfigNtyReq(const TPuDevConfig &tPuDevConfig, vector<TPuDevConfig> &vctConfig);

public:
    void PrintData();
    void PrintSsList() const;

protected:

    // ������أ��⼸���ӿ�ֻ����ӡ�ɾ�����Ĺ�ϵ�����Ƕ���ӿڣ�
    void AddSipSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void AddSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);
    void DelSipSsTask(u32 dwTaskNO);
    void DelSipSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void DelSipSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);

    void GetSipSserTasksBySsType(const TSsType &tSsType, set<u32> &cTasks) const;
    void PublishSipNotify(const TSsType &tSsType, CEventReq &cNtfReq, TSsMode tSsMode = ALL_SS, u32 dwTaskNO = INVALID_TASKNO); 

    void ProcRemovePu();

    // GPS�������
    void SetPuGpsSs(bool bIsEnable);

private:

    string                  m_strDevId;                      // �豸Id
    CPuDataList            *m_pcPuList;                      // CPuData������CPuDataList����ָ��
    CPuRegBaseTask         *m_pcRegTask;                     // �豸��Ӧ��ע��Task��Ŀǰ�ж���ע�᷽ʽ������ע�������ע�ᣩ
    vector<time_t>          m_cOfflineTimeList;              // �豸����ʱ���¼
    map<TSsType, set<u32> > m_cSsTaskList;                   // �����б�����ÿ���������͵����ж�������Ϣ

    // ����������redis���Ѿ������ˣ�����������cmu�ڲ����Ƶ���ķ��ʣ������ٱ���һ�ݣ��Կռ任ʱ��
    bool   m_bOnline;

    // gpsdata��transdata��redis�в�ʵʱ���£�ֱ�ӱ������ڴ���
    TGpsData m_tGpsData;    
};
typedef map<string, CPuData> CPuDataMap;                     // keyΪDevId

class CPuDataList
{
public:
    CPuDataList();
    ~CPuDataList();

    bool Init(CDomainData *pcDomainData);

    // PuData����
    CPuData        *AddPuData(const string &strDevId);
    void            DelPuData(const string &strDevId);
    void            ModPuData(const string &strDevId);
    CPuData        *GetPuData(const string &strDevId);
    CPuDataMap     &GetPuDataMap() { return m_cPuList; }

    // �����豸ͳ��
    u32 GetOnlineVidSrcNum();
    u32 GetOnlineVidSrcNum2();
    TDeviceVideoSrcIdList GetOfflineDevList(s32 &nOffNum, s32 &nIdleNum);

    // ��ȡ�������Ϣ
    string  GetDomainId();
    string  GetDomainName();
    CDomainData *GetDomainData() { return m_pcDomainData; }
    CRedisPipe &GetRedisPipe();

    vector<string> GetPuIdList() const;
    void GetPuIdList(vector<string> &cPuIdList) const;
    void GetOnlinePuIdList(set<string> &cPuIdList) const;

    u32 GetPuTotalNum() const { return m_cPuList.size(); }

public:
    // ���Դ�ӡ���
    void PrintData(bool bSummaryInfo = true, u8 byState = 0);
    void GetDevNum(u32 &dwTotalNum, u32 &dwOnlineNum, u32 &dwSimuRegedNum, u32 &dwOthers, 
        map<string, u32> &cOnlineNum, map<string, u32> &cSimuRegedNum);


private:
    CDomainData            *m_pcDomainData;                  // CPuDataList������CDomainData����ָ��
    CPuDataMap              m_cPuList;                       // ʹ��CPuDataMap���洢�豸����
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

    // ��ȡCPuDataList
    CPuDataList &GetPuList() { return m_cPuList; }

    // ��ȡDomainId��DomainName��
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

    // ������أ��⼸���ӿ�ֻ����ӡ�ɾ�����Ĺ�ϵ�����Ƕ���ӿڣ�
    void AddCatalogSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void AddCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);
    void DelCatalogSsTask(u32 dwTaskNO);
    void DelCatalogSsTask(u32 dwTaskNO, const TSsType  &tSsType);
    void DelCatalogSsTask(u32 dwTaskNO, const TSsTypes &tSsTypes);

private:

    string                  m_strDomainId;                   // ��Id
    CPuDataList             m_cPuList;                       // ʹ��CPuDataList�������豸����

    DomainType              m_emDomainType;                  // ������

    CRedisPipe              m_cRedisPipe;                    // redis�ܵ������ʺͶ��Ĺܵ�
    CCmuConnecterTask      *m_pcConnectTask;                 // ���cmu����Task

    map<TSsType, set<u32> > m_cSsTaskList;                   // �����б�����ÿ���������͵����ж�������Ϣ

    // ����������redis���Ѿ������ˣ�����������cmu�ڲ����Ƶ���ķ��ʣ������ٱ���һ�ݣ��Կռ任ʱ��
    string                  m_strDomainName;
};
typedef map<string, CDomainData> CDomainDataMap;             // keyΪDomainId

class CDomainDataList
{
public:
    CDomainDataList();
    ~CDomainDataList();

    // ������Init���ڹ��캯�����ǱȽϺ��ʵģ�����Init��Ҫ�õ�this�����Ի�����ʹ�����ֶ�������
    bool Init(const string &strLocalDomainId, const string &strLocalDomainName, 
        const string &strIp, u16 wPort, const string &strPasswd);
    bool InitDomainList();
    bool InitDevList();

    // ���ӵ����������cmu
    void Connect();

    // ��ȡ�����DomainId��DomainName
    string GetLocalDomainId()    { return m_pcLocalDomainData->GetDomainId();   }
    string GetLocalDomainName()  { return m_pcLocalDomainData->GetDomainName(); }

    // DomainName��DomainId֮���ת��
    string GetDomainNameById(const string &strDomainId);
    string GetDomainIdByName(const string &strDomainName);
    string GetDomainNameByNO(const TKDMNO &tDomainNO);

    // �����ݹ�����Ҫ������ɾ����
    CDomainData *AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType);
    void         DelDomainData(const string &strDomainId);
    CDomainData *GetDomainDataByDomainId(const string &strDomainId);
    CDomainData *GetDomainDataByDomainName(const string &strDomainName);
    CDomainData *GetDomainDataByRedisPipe(const CRedisPipe *pcRedisPipe);
    CDomainData *GetLocalDomainData() { return m_pcLocalDomainData; }
    CPuData     *CheckPuData(const string &strDomainId, const string &strDevId);

    // �����������˹�ϵ��ȡ�����ָ����ͨ�ŵ�����
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
    CDomainData            *m_pcLocalDomainData;             // �����DomainDataָ��
    CDomainDataMap          m_cDomainList;                   // ʹ��CDomainDataMap���洢����Ϣ  
};

// cmuȫ�����ݣ�Ψһ��һ�ݣ�ʹ�õ���ģʽ
class CCmuData
{
private:
    CCmuData();
    ~CCmuData();

public:
    static CCmuData *GetInstance();                          // ��ȡCCmuData�����ָ�룬ʵ�ּ򵥵ĵ���ģʽ

    bool Initialize(const string &strLocalDomainId, const string &strLocalDomainName, 
        const string &strIp, u16 wPort, const string &strPasswd);// ���ݳ�ʼ��
    void StartWork();

    // ���豸��ؼ򻯽ӿ�
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

    // ������ӿ�
    CDomainData * AddDomainData(const string &strDomainId, CDomainData::DomainType emDomainType);
    void DelDomainData(const string &strDomainId);

    // ������ؼ򻯽ӿ�
    void AddSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList, TSsDevList &tSsSuccessList, TSsErrorList &tSsErrorList);           // �������б���
    void DelSipSsTask(u32 dwTaskNO, const TSsDevList &tSsDevList);
    s32  AddSipSsTask(u32 dwTaskNO, const string &strDomainName, const TSsTypes &tSsTypes);                                          // ������Ŀ¼����
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

    // һЩͳ������
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

// Redis�����ص������Ͷ��Ļص�����
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

//nms�澯���
//��ˮ�ųأ���֤������ˮ��Ψһ��
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
    CXQueue<u16> m_cIndexQue;  //���ٴ�ȡIndex
    CXSet<u16> m_cIndexSet;    //��֤Index��Ψһ��
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