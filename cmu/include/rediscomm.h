
//===============================================================================================================================
// ���������˼·��
//     1��g_cRedisDataMng�����redis���м�ʱ���ݵĽ���
//     2��g_cRedisDataSubMng�����redis����������Ķ��ģ���ҵ������˵��������3as�����Ķ��ģ������豸���������Ϣ�����
//     3��g_cRedisStatusSubMng�����redis����״̬��Ķ��ģ���ҵ������˵��������pui�����Ķ��ģ������豸���á��澯��GPS��͸�����ݵ�
//===============================================================================================================================

#ifndef _REDIS_COMM_H
#define _REDIS_COMM_H

#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/redissdk/redissdk.h"
#include "ssbasetask.h"

class CPuDataList;
class CDomainDataList;
class CTasDevChangeNtf;
class CTasDevVideoNameNtf;
class CTasDevVideoGpsNtf;
struct CTasDomainChangeNtf;

struct TRedisDiscCB;
struct TFastdbSubCB;
struct TPuBaseCapSet;


struct TPuRegInfo
{
public:
    TPuRegInfo()
    {
        use = false;
        expire = true;
        chnnum = 0;
        freeLicense = 0;
        creator = CREATE_BY_PLAT;
        chnStatusFlag = false;
    }

    bool   use;                     // ���ñ�־
    bool   expire;                  // ��������
    s32    chnnum;                  // ����ͨ������
    s32    freeLicense;             // ��ѵ���
    s32    license;                 // ����ʱָ����license����
    s32    creator;                 // ������
    string name;                    // �豸����
    string alias;                   // �豸����
    string connectIp;               // �豸����ip
    string puiIp;                   // �豸��Ӧ��pui��ip
    bool   chnStatusFlag;           // �豸ͨ��״̬��־��false-����ͨ���������ߣ�true-��������ͨ��
};


//===============================================================================================================================

class CRedisPipe
{
public:
    CRedisPipe();
    virtual ~CRedisPipe();

    static void SetRedisPipeLogFunc(LogCallBack pf) { SetLogCallBack(pf, NULL); }

    // redis���Ӳ���
    void SetRedisParam(const string &strRedisIp, u16 wRedisPort, const string &strRedisPassword);
    const string &GetRedisIp() const { return m_strRedisIp; }
    u16 GetRedisPort() const { return m_wRedisPort; }
    const string &GetRedisPassword() const { return m_strRedisPassword; }

    bool Connect();	                                                        // ���ӵ�redis
    bool IsConnected();														// �ж�redis�Ƿ�������
    void Disconnect();

    // �豸״̬��ʼ������ʵ���ǽ��豸����ͨ��״̬����Ϊoffline��
    bool InitDevStatus(const vector<string> &cDevId, bool bPublishState = false);

    // �豸����ע��
    bool BatchGetPuRegInfo(const vector<string> &cDevId, vector<TPuRegInfo> &cInfo);

    // �豸��������
    bool BatchPuOnline(const vector<string> &cDevId, bool bPublishState = false);

    // ��ȡ�����豸��DevId�б�
    bool GetDevIdList(vector<string> &cDevId);
    bool GetDomainIdList(vector<string> &cDomainId);

    // ��¶FastDBʵ��������ִ��һЩû�з�װ��������
    CFastDBInterface &GetFastDBInstance() { return m_cRedisSdk.GetFastDBInstance(); }

    // ����Ϣ���
    bool GetDomainName(const string &strDomainId, string &strDomainName);
    bool GetDomainKdmNO(const string &strDomainId, string &strKdmNO);
    bool GetDomainRelationInfo(const string &strDomainId, CDomainRelationInfo &cInfo);

    // ǰ���豸У��
    bool GetPuInUse(const string &strDevId, bool &bInUse);									// �豸���ñ�־

    // �豸��������
    bool GetDevInfo(const string &strDevId, CDeviceInfo &cInfo);
    bool GetDevDomainId(const string &strDevId, string &strDomainId);
    bool GetDevType(const string &strDevId, s32 &nDevType);
    bool GetDevGBFlag(const string &strDevId, s32 &nGBFlag);                  // ��ȡ�豸�Ĺ����־��true�����꣬false���ǹ��꣩
    bool GetDevName(const string &strDevId, string &strDevName);
    bool GetDevAlias(const string &strDevId, string &strAlias);
    bool GetDevNetAddr(const string &strDevId, string &strAddr);
    bool GetDevModelId(const string &strDevId, string &strModelId);
    bool GetDevManufacturer(const string &strDevId, string &strManufacturer);
    bool GetNetType(const string &strDevId, s32 &nNetType);
    bool GetCreater(const string &strDevId, s32 &nCreater);
    bool GetOwner(const string &strDevId, string &strOwner);
    bool GetExpireDate(const string &strDevId, string &strExpireDate);
    bool GetKdmNO(const string &strDevId, string &strKdmNO);
    bool GetPuVidSrcNum(const string &strDevId, s32 &nVidSrcNum);
    bool GetPuHdmiOutputNum(const string &strDevId, s32 &nHdmiOutputNum);
    bool GetDirectTransNum(const string &strDevId, s32 &nTransNum);

    bool GetVidSrcAlias(const string &strDevId, s32 nVidSrcIdx, string &strAliasMap);
    bool GetVidSrcAliasMap(const string &strDevId, map<int, string> &cAliasMap);
    bool GetPuVidSrcAlias(const string &strDevId, s32 nVidSrcIdx, string &strAliasMap);
    bool GetPuVidSrcAliasMap(const string &strDevId, map<int, string> &cAliasMap);
    bool GetVideoEncodeParam(const string &strDevId, s32 nChnNO, TVideoEncodeParam &tParam);
    bool GetAudioEncodeParam(const string &strDevId, s32 nChnNO, TAudioEncodeParam &tParam);
    bool GetVidSrcChn(const string &strDevId, s32 nVidSrcNO, CPuChnSet &cChn);
    bool GetVidSrcChnSet(const string &strDevId, map<int, CPuChnSet> &cChnSet);
    bool GetChnVidSrc(const string &strDevId, s32 nChnNO, CVidSrcSet &cVidSrc);
    bool GetChnVidSrcSet(const string &strDevId, map<int, CPuChnSet> &cChnSet);
    bool GetPuChnState(const string &strDevId, s32 nChnNO, TPuChnStateConfig &tChnState);
    bool GetPuChnStateSet(const string &strDevId, map<int, TPuChnStateConfig> &tChnStateSet);

    // �豸ע�������Ҫ�õ�����Ϣ
    bool GetPuConnectAddr(const string &strDevId, string &strAddr);
    bool GetPuVidEncNum(const string &strDevId, s32 &nVidEncNum);
    bool GetPuFreeLicenseNum(const string &strDevId, s32 &nFreeNum);
    bool GetBaseCapSet(const string &strDevId, TPuBaseCapSet &tCapSet);
    bool GetPuiIp(const string &strDevId, string &strPuiIp);

    // �豸������״̬
    bool GetPuStatus(const string &strDevId, TPuStatus &tPuStatus);
    bool SetPuStatus(const string &strDevId, const TPuStatus &tPuStatus);
    bool GetPuIsOnline(const string &strDevId, bool &bOnline);
    bool SetPuIsOnline(const string &strDevId, bool bOnline);

    // �����豸ͨ��״̬��������״̬��ͨ��ȫ������Ϊ���ߣ�
    // ������cmu�ǲ�Ӧ��ȥ���������ģ����Ѿ���֮ǰ�ķ�����Υ�������Ǽ�Ȼ��ôҪ��ֻ�����ʵ��
    // ���⣬����Ҫ��ô����cmuҲӦ��ֻ����pui��֧��redis�������ȥ����ͨ��״̬��ͬ���ģ���ȻҪ��cmu���������豸��ͨ��װ�ֻ�����
    bool CleanupDevChnState(const string &strDevId);
    bool SetPuChnState(const string &strDevId, s32 nChnNO, TPuChnStateConfig &tChnState);
    bool SetPuChnStateSet(const string &strDevId, map<int, TPuChnStateConfig> &tChnStateSet);

    // PuConfig���
    bool GetPuConfig(const string &strDevId, TPuConfig &tPuConfig);
    bool SetPuConfig(const string &strDevId, TPuConfig &tPuConfig);
    bool GetPuDevConfig(const string &strDevId, TPuDevConfig &tPuDevConfig);
    bool MergePuConfig(const string &strDevId, TPuDevConfig& tDevConfig);
    bool GetExtCapSet(const string &strDevId, map<int,int> &cExtCapSet);
    bool SetExtCapSet(const string &strDevId, const map<int,int> &cExtCapSet);

    // ����״̬��ǰ��¼��״̬����Ƶ����״̬��
    bool GetPuServiceStatus(const string &strDevId, TPuServiceStatus &tPuServiceStatus);
    bool SetPuServiceStatus(const string &strDevId, const TPuServiceStatus &tPuServiceStatus);
    bool GetIsPuRec(const string &strDevId, s32 nChnNO, bool &bIsRec);
    bool SetIsPuRec(const string &strDevId, s32 nChnNO, bool bIsPuRec);
    bool GetPuAudioCallSrc(const string &strDevId, s32 nChnNO, TChannel &tChn);
    bool SetPuAudioCallSrc(const string &strDevId, s32 nChnNO, const TChannel &tSrcChn);
    bool ClearPuServiceStatus(const string &strDevId);

    // �澯��Ϣ���
    bool GetPuAlarmStatus(const string &strDevId, TPuAlarmStatus &tPuAlarmStatus);
    bool SetAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus);      // ���������ĸ澯��Ϣ����������ͨ����Ϊû�и澯
    bool UpdateAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus);   // �ϲ��澯��Ϣ��tPuAlarmStatus�������������ݣ���������ͨ��������
    bool UpdateAlarmInput(const string &strDevId, const TAlarmInput &tAlarmInput);
    bool ClearAlarmStatus(const string &strDevId);

    // GPS�������
    bool GetPuGpsData(const string &strDevId, TGpsData &tGpsData);
    bool SetPuGpsData(const string &strDevId, const TGpsData &tGpsData);

    // ͸������
    bool GetPuTransparentData(const string &strDevId, TTransparentData &tTransData);
    bool SetPuTransparentData(const string &strDevId, const TTransparentData &tTransData);
    
    // ����redis֪ͨ
    void PublishPuStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TPuStatus &tPuStatus, TSsMode tSsMode = ALL_SS);
    void PublishPuConfig(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TPuDevConfig &tPuDevConfig, TSsMode tSsMode = ALL_SS);
    void PublishPuServiceStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TPuServiceStatus &tPuServiceStatus, TSsMode tSsMode = ALL_SS);
    void PublishPuAlarmStatus(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TPuAlarmStatus &tPuAlarmStatus, TSsMode tSsMode = ALL_SS);
    void PublishPuGpsData(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TGpsData &tGpsData, TSsMode tSsMode = ALL_SS);
    void PublishPuTransparentData(const string &strDomainId, const string &strDomainName, const string &strDevId, 
        const TTransparentData &tTransData, TSsMode tSsMode = ALL_SS);

    // ����������
    const CRedisSdk *GetRedisSdkPtr() const { return &m_cRedisSdk; }
    CRedisSdk *GetRedisSdkPtr() { return &m_cRedisSdk; }
    const CRedisSdk &GetRedisSdk() const { return m_cRedisSdk; }
    CRedisSdk &GetRedisSdk() { return m_cRedisSdk; }

    static u16 GetInitDevStatusNumPerTime() { return m_swRedisBatchProcNumPerTime; }

    static TRedisSwitchType SdpSessionName2RedisSwitchType(const TSdpSessionName &tSessionName);
    static TSdpSessionName  RedisSwitchType2SdpSessionName(const TRedisSwitchType &tSwitchType);

public:

    // �������
    void ClearSs() { m_cSsList.clear(); }
    void AddSs(const string &strChn);
    void AddSs(const vector<string> &cChn);
    void DelSs(const string &strChn);
    void DelSs(const vector<string> &cChn);
    const set<string> &GetSsChn() const { return m_cSsList; }

protected:

    static void RedisReply2PuRegInfo(redisReply *reply, TPuRegInfo &tInfo);

private:
    CRedisSdk              m_cRedisSdk;

    string                 m_strRedisIp;
    u16                    m_wRedisPort;
    string                 m_strRedisPassword;

    set<string>            m_cSsList;

    static const u16       m_swRedisBatchProcNumPerTime = 100;
};


#endif /*_REDIS_COMM_H*/


