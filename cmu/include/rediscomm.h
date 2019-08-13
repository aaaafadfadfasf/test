
//===============================================================================================================================
// 基本的设计思路：
//     1、g_cRedisDataMng负责和redis进行即时数据的交互
//     2、g_cRedisDataSubMng负责和redis进行数据类的订阅，从业务上来说，就是由3as发布的订阅，包括设备变更、域信息变更等
//     3、g_cRedisStatusSubMng负责和redis进行状态类的订阅，从业务上来说，就是由pui发布的订阅，包括设备配置、告警、GPS、透明数据等
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

    bool   use;                     // 启用标志
    bool   expire;                  // 过期日期
    s32    chnnum;                  // 编码通道数量
    s32    freeLicense;             // 免费点数
    s32    license;                 // 入网时指定的license点数
    s32    creator;                 // 创建者
    string name;                    // 设备名称
    string alias;                   // 设备别名
    string connectIp;               // 设备接入ip
    string puiIp;                   // 设备对应的pui的ip
    bool   chnStatusFlag;           // 设备通道状态标志，false-所有通道均不在线，true-存在在线通道
};


//===============================================================================================================================

class CRedisPipe
{
public:
    CRedisPipe();
    virtual ~CRedisPipe();

    static void SetRedisPipeLogFunc(LogCallBack pf) { SetLogCallBack(pf, NULL); }

    // redis连接参数
    void SetRedisParam(const string &strRedisIp, u16 wRedisPort, const string &strRedisPassword);
    const string &GetRedisIp() const { return m_strRedisIp; }
    u16 GetRedisPort() const { return m_wRedisPort; }
    const string &GetRedisPassword() const { return m_strRedisPassword; }

    bool Connect();	                                                        // 连接到redis
    bool IsConnected();														// 判断redis是否已连接
    void Disconnect();

    // 设备状态初始化（其实就是将设备及其通道状态设置为offline）
    bool InitDevStatus(const vector<string> &cDevId, bool bPublishState = false);

    // 设备批量注册
    bool BatchGetPuRegInfo(const vector<string> &cDevId, vector<TPuRegInfo> &cInfo);

    // 设备批量上线
    bool BatchPuOnline(const vector<string> &cDevId, bool bPublishState = false);

    // 获取所有设备的DevId列表
    bool GetDevIdList(vector<string> &cDevId);
    bool GetDomainIdList(vector<string> &cDomainId);

    // 暴露FastDB实例，方便执行一些没有封装过的命令
    CFastDBInterface &GetFastDBInstance() { return m_cRedisSdk.GetFastDBInstance(); }

    // 域信息相关
    bool GetDomainName(const string &strDomainId, string &strDomainName);
    bool GetDomainKdmNO(const string &strDomainId, string &strKdmNO);
    bool GetDomainRelationInfo(const string &strDomainId, CDomainRelationInfo &cInfo);

    // 前端设备校验
    bool GetPuInUse(const string &strDevId, bool &bInUse);									// 设备启用标志

    // 设备入网数据
    bool GetDevInfo(const string &strDevId, CDeviceInfo &cInfo);
    bool GetDevDomainId(const string &strDevId, string &strDomainId);
    bool GetDevType(const string &strDevId, s32 &nDevType);
    bool GetDevGBFlag(const string &strDevId, s32 &nGBFlag);                  // 获取设备的国标标志（true：国标，false：非国标）
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

    // 设备注册过程中要用到的信息
    bool GetPuConnectAddr(const string &strDevId, string &strAddr);
    bool GetPuVidEncNum(const string &strDevId, s32 &nVidEncNum);
    bool GetPuFreeLicenseNum(const string &strDevId, s32 &nFreeNum);
    bool GetBaseCapSet(const string &strDevId, TPuBaseCapSet &tCapSet);
    bool GetPuiIp(const string &strDevId, string &strPuiIp);

    // 设备上下线状态
    bool GetPuStatus(const string &strDevId, TPuStatus &tPuStatus);
    bool SetPuStatus(const string &strDevId, const TPuStatus &tPuStatus);
    bool GetPuIsOnline(const string &strDevId, bool &bOnline);
    bool SetPuIsOnline(const string &strDevId, bool bOnline);

    // 清理设备通道状态（将上线状态的通道全部设置为下线）
    // 理论上cmu是不应该去做这个事情的，这已经和之前的方案相违背，但是既然这么要求，只能如此实现
    // 另外，即便要这么做，cmu也应该只是在pui不支持redis的情况下去清理通道状态，同样的，既然要求cmu清理所有设备的通道装填，只能如此
    bool CleanupDevChnState(const string &strDevId);
    bool SetPuChnState(const string &strDevId, s32 nChnNO, TPuChnStateConfig &tChnState);
    bool SetPuChnStateSet(const string &strDevId, map<int, TPuChnStateConfig> &tChnStateSet);

    // PuConfig相关
    bool GetPuConfig(const string &strDevId, TPuConfig &tPuConfig);
    bool SetPuConfig(const string &strDevId, TPuConfig &tPuConfig);
    bool GetPuDevConfig(const string &strDevId, TPuDevConfig &tPuDevConfig);
    bool MergePuConfig(const string &strDevId, TPuDevConfig& tDevConfig);
    bool GetExtCapSet(const string &strDevId, map<int,int> &cExtCapSet);
    bool SetExtCapSet(const string &strDevId, const map<int,int> &cExtCapSet);

    // 服务状态（前端录像状态、音频呼叫状态）
    bool GetPuServiceStatus(const string &strDevId, TPuServiceStatus &tPuServiceStatus);
    bool SetPuServiceStatus(const string &strDevId, const TPuServiceStatus &tPuServiceStatus);
    bool GetIsPuRec(const string &strDevId, s32 nChnNO, bool &bIsRec);
    bool SetIsPuRec(const string &strDevId, s32 nChnNO, bool bIsPuRec);
    bool GetPuAudioCallSrc(const string &strDevId, s32 nChnNO, TChannel &tChn);
    bool SetPuAudioCallSrc(const string &strDevId, s32 nChnNO, const TChannel &tSrcChn);
    bool ClearPuServiceStatus(const string &strDevId);

    // 告警信息相关
    bool GetPuAlarmStatus(const string &strDevId, TPuAlarmStatus &tPuAlarmStatus);
    bool SetAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus);      // 设置完整的告警信息，不包含的通道视为没有告警
    bool UpdateAlarmStatus(const string &strDevId, const TPuAlarmStatus &tPuAlarmStatus);   // 合并告警信息，tPuAlarmStatus可以是增量数据，不包含的通道不设置
    bool UpdateAlarmInput(const string &strDevId, const TAlarmInput &tAlarmInput);
    bool ClearAlarmStatus(const string &strDevId);

    // GPS数据相关
    bool GetPuGpsData(const string &strDevId, TGpsData &tGpsData);
    bool SetPuGpsData(const string &strDevId, const TGpsData &tGpsData);

    // 透明数据
    bool GetPuTransparentData(const string &strDevId, TTransparentData &tTransData);
    bool SetPuTransparentData(const string &strDevId, const TTransparentData &tTransData);
    
    // 发布redis通知
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

    // 其他处理函数
    const CRedisSdk *GetRedisSdkPtr() const { return &m_cRedisSdk; }
    CRedisSdk *GetRedisSdkPtr() { return &m_cRedisSdk; }
    const CRedisSdk &GetRedisSdk() const { return m_cRedisSdk; }
    CRedisSdk &GetRedisSdk() { return m_cRedisSdk; }

    static u16 GetInitDevStatusNumPerTime() { return m_swRedisBatchProcNumPerTime; }

    static TRedisSwitchType SdpSessionName2RedisSwitchType(const TSdpSessionName &tSessionName);
    static TSdpSessionName  RedisSwitchType2SdpSessionName(const TRedisSwitchType &tSwitchType);

public:

    // 订阅相关
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


