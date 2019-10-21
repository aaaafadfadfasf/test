
#ifndef _REDISSDK_H
#define _REDISSDK_H

#include <string>
#include <map>
#include <set>
#include <vector>
using namespace std;

#include "cms/cms_errorcode.h"
#include "sdk/fastdbsdk/fastdb.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/module_struct.h"
#include "sdk/redissdk/switch_struct.h"
#include "sdk/redissdk/nru_struct.h"
#include "sdk/redissdk/partition_struct.h"
#include "sdk/redissdk/record_struct.h"
#include "sdk/redissdk/config_struct.h"
#include "sdk/redissdk/vtdu_struct.h"
#include "sdk/redissdk/cui_struct.h"
#include "sdk/redissdk/redissdkinner.h"

typedef string TRedisSdkOperType;
#define RedisSdkOperType_Set             ("set")           // 设置全量数据
#define RedisSdkOperType_Add             ("add")           // 添加数据
#define RedisSdkOperType_Del             ("del")           // 删除数据

typedef string TRedisBalanceType;
#define RedisBalanceType_Num             ("num")           // 按路数均衡
#define RedisBalanceType_Band            ("band")          // 按带宽均衡

// 类型声明
class CFastDBInterface;


/*
 * todo by xiongxh
 *
 * RedisSdk提供的主要功能如下：
 *     1. 
 *
 * 几个开发过程中暂未实现的优化点：
 *     1. 根据不同的功能，将key分布在不同的数据库空间中，目前所有的key全部集中在第0个数据库中。
 *        按照功能来分库的话，由于之前fastdb的接口都是简单的get、set命令来实现，没有数据库编号，那么为了防止切换
 *        到其他的数据库后又去调用以前的接口，导致获取不到数据的问题，可以在新的实现中每次执行结束后都切换回第0
 *        个数据库
 *     2. 模块注册后，如果后续在刷新时失败，则如何处理？是否需要一种机制将这个信息通知到应用层？
 *
 */
class CRedisSdk
{
public:
    CRedisSdk();
    virtual ~CRedisSdk();

public:
    typedef void (* RedisSdkCallback)(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3);

public:
    /*
     * 初始化和反初始化接口
     * 
     * 注意：
     *     初始化接口，进行的操作如下：
     *       1. 连接redis
     *       2. 订阅并获取lua脚本的sha1映射信息，构建本地缓存的脚本名到sha1之间的映射表
     *
     *     反初始化接口，进行的操作如下：
     *       1. 模块如果已经注册（即调用RegisterModule），则自动取消注册
     *       2. 和redis断开连接
     *       3. 清除本地的缓存信息
     */
    ECMS_ERRORCODE Initialize(const string &strClientName, const string &strServer, u16 wPort, 
        const string &strPassword, RedisSdkCallback pfCallback, void *pvUserData);
    void Quit();

    /*
     * 返回Fastdb的实例
     *
     * 注意：
     *     1. RedisSdk在内部还是通过fastdb来访问redis的，部分功能如果RedisSdk没有封装，可以通过获取fastdb的实例来调用
     */
    CFastDBInterface *GetFastDBInstancePtr() { return &m_cFastDBInstance; }
    CFastDBInterface &GetFastDBInstance()    { return m_cFastDBInstance; }

    /*
     * 模块调度接口
     * 
     * 注意：
     *     1. 模块调度接口分为两类，一类只返回调度算法选择的模块信息，另一类还返回锁定的资源id，
     *        有些操作不涉及到资源相关的，使用前者，而有些操作则是有资源限制的，需要使用后者
     *        比如调度nru，如果是需要进行录像，由于录像资源是有限的，所以调度时需要锁定资源，避免超过资源上限，
     *        如果只是进行录像查询等操作，这类操作没有做数量限制，可以使用不带资源锁定的调度函数
     *     2. 对于带资源锁定的接口，调度成功后，资源会锁定一段时间，然后自动释放（基于redis的expire实现），
     *        应用层也可以手动调用ReleaseResource主动释放
     */
    ECMS_ERRORCODE SelectCui(const string &strSession, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectPui(const string &strDevId, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectCmu(string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectRcs(string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectNru(const TRedisModuleNruSelectCond &tCond, string &strId, string &strDomainName);
    ECMS_ERRORCODE SelectVtdu(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst, 
        string &strId, string &strDomainName, TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource *ptResource = NULL);
    ECMS_ERRORCODE SelectVtduEx(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst, 
        const string &strDstAddr, bool& bSameIpVtdu, string &strId, string &strDomainName, 
        TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource *ptResource = NULL);

    // 申请、获取、释放vtdu的资源（通过调度算法获取的资源是有时限的，超过时限自动释放，如果觉得自动释放时间太长，也可通过ReleaseVtduResource主动释放）
    ECMS_ERRORCODE AllocVtduResource(const TRedisModuleResourceId &tOriginResourceId, TRedisModuleResourceId &tResourceId, 
        TRedisModuleVtduResource *ptResource = NULL);
    ECMS_ERRORCODE GetVtduResource(const TRedisModuleResourceId &tResourceId, TRedisModuleVtduResource &tResource);
    ECMS_ERRORCODE ReleaseVtduResource(const TRedisModuleResourceId &tResourceId);

    // 更新模块负载（负载不同，调度策略不同）
    ECMS_ERRORCODE UpdateModuleLoad(const string &strId, const TRedisModuleLoad &tLoad);

    // 清理模块的私有数据
    ECMS_ERRORCODE CleanModulePrivateData(const TRedisModuleId &tId);

    // 获取模块信息
    ECMS_ERRORCODE GetModuleDetailInfo(const string &strId, TRedisModuleDetailInfo &tModuleDetailInfo);

    /*
     * 订阅接口
     */
    ECMS_ERRORCODE UpdateSsChannel(const CRedisSsChannel &cSsChn, const TRedisSdkOperType &tType = RedisSdkOperType_Set);
    ECMS_ERRORCODE UpdateSsChannel(const set<CRedisSsChannel> &cSsList, const TRedisSdkOperType &tType = RedisSdkOperType_Set);

    /*
     * 兼容Fastdb的订阅通道，使得Fastdb和RedisSdk的订阅通道能一致的处理（除非清楚具体的含义，否则请不要使用）
     */
    ECMS_ERRORCODE UpdateCompaSsChannel(const string &strSsChn, const TRedisSdkOperType &tType = RedisSdkOperType_Set);
    ECMS_ERRORCODE UpdateCompaSsChannel(const set<string> &cSsList, const TRedisSdkOperType &tType = RedisSdkOperType_Set);

    // 获取平台模块上下线的订阅通道（通知回调中的pvParam2内容为TRedisModuleStatus进行Json序列化的内容订阅）
    set<CRedisSsChannel> GetModuleSsChannelById(const set<string> &cModuleId);
    CRedisSsChannel      GetModuleSsChannelById(const string &strModuleId)
    { 
        return CRedisSsChannel(RedisSsChannelType_Module_Status, strModuleId);
    }

    set<CRedisSsChannel> GetModuleSsChannelByType(const set<TRedisModuleType> &cTypes);
    CRedisSsChannel      GetModuleSsChannelByType(const TRedisModuleType &tType)
    { 
        return CRedisSsChannel(RedisSsChannelType_Module_Type_Status, tType);
    }

    // 获取平台录像策略的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecCoverPolicy进行Json序列化的内容订阅）
    CRedisSsChannel      GetPlatRecCoverPolicySsChannel()
    { 
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_Cover_Policy); 
    }

    // 获取平台录像无码流检测参数的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecNoStreamCheckParam进行Json序列化的内容）
    CRedisSsChannel      GetPlatRecNoStreamCheckParamSsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_No_Stream_Check_Param);
    }

    // 获取平台录像文件切换策略的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecFileCutPolicy进行Json序列化的内容）
    CRedisSsChannel      GetPlatRecFileCutPolicySsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_File_Cut_Policy);
    }

    // 获取平台录像保留天数的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecReserveDays进行Json序列化的内容）
    CRedisSsChannel      GetPlatRecReserveDaysSsChannel()
    { 
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Plat_Rec_Reserve_Days); 
    }

    // 获取设备录像时长的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecDuration进行Json序列化的内容）
    set<CRedisSsChannel> GetPlatRecDurationSsChannel(const set<TRedisPuRecUnit> &cRecUnits);
    CRedisSsChannel      GetPlatRecDurationSsChannel(const TRedisPuRecUnit &tRecUnit)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Duration, tRecUnit.ToString());
    }

    // 获取设备平台录像状态的订阅通道（通知回调中的pvParam2内容为TRedisPlatRecStatus进行Json序列化的内容）
    set<CRedisSsChannel> GetPlatRecStatusSsChannelByRecUnit(const set<TRedisPuRecUnit> &cRecUnits);
    CRedisSsChannel      GetPlatRecStatusSsChannelByRecUnit(const TRedisPuRecUnit &tRecUnit)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, tRecUnit.ToString());
    }

    set<CRedisSsChannel> GetPlatRecStatusSsChannelByDevice(const set<string> &cDevId);
    CRedisSsChannel      GetPlatRecStatusSsChannelByDevice(const string &strDevId)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, strDevId);
    }

    set<CRedisSsChannel> GetPlatRecStatusSsChannelByDomain(const set<string> &cDomain);
    CRedisSsChannel      GetPlatRecStatusSsChannelByDomain(const string &strDomain)
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Rec_Status, strDomain);
    }

    // 获取设备平台录像状态的订阅通道（通知回调中的pvParam2内容为vector<TRedisPartition>进行Json序列化的内容）
    set<CRedisSsChannel> GetPartitionSsChannel(const set<string> &cNruIds);
    CRedisSsChannel      GetPartitionSsChannel(const string &strNruId)
    {
        return CRedisSsChannel(RedisSsChannelType_Partition, strNruId);
    }

    // 获取nru的变更信息订阅通道（通知回调中的pvParam2内容为TRedisNruTopoNotify进行Json序列化的内容）
    CRedisSsChannel      GetNruTopoSsChannel()
    {
        return CRedisSsChannel(RedisSsChannelType_Plat_Config, RedisSsChannelData_Nru_Topo);
    }

    // 获取nru的平台状态订阅通道（通知回调中的pvParam2内容为TRedisNruPlatStatus进行Json序列化的内容）
    set<CRedisSsChannel> GetNruPlatStatusSsChannel(const set<string> &cNruIds);
    CRedisSsChannel      GetNruPlatStatusSsChannel(const string &strNruId)
    {
        return CRedisSsChannel(RedisSsChannelType_Nru_Plat_Status, strNruId);
    }

public:

    /*
     * cui的数据接口
     */
    ECMS_ERRORCODE CuOnline(const string &strCuiId, const TRedisCuAgent &tCuAgent);
    ECMS_ERRORCODE CuOffline(const string &strCuiId, const string &strSession);
	ECMS_ERRORCODE GetCuInfo(const string &strCuId, TRedisCuAgent &tCuAgent);

    ECMS_ERRORCODE UpdateCuiInfo(const string &strCuiId, const TRedisModuleCui &tCuiInfo);
    ECMS_ERRORCODE UpdateCuiOnlineMobileNum(const string &strCuiId, u32 dwOnlineMobileNum);

    ECMS_ERRORCODE SetPlatRecParam(const TRedisPuRecUnit &tRecUnit, const TRedisPlatRecParam &tRecParam, 
        bool bUpdateComplete = true, bool bAutoPublish = true);
    ECMS_ERRORCODE GetPlatRecParam(const TRedisPuRecUnit &tRecUnit, TRedisPlatRecParam &tRecParam);

    ECMS_ERRORCODE AddPlatRecTask(const TRedisPlatRecType &tRecType, const TRedisPuRecUnit &tRecUnit);
	ECMS_ERRORCODE DelPlatRecTask(const TRedisPlatRecType &tRecType, const TRedisPuRecUnit &tRecUnit);

	ECMS_ERRORCODE GetPlatRecTaskTypes(const TRedisPuRecUnit &tRecUnit, set<TRedisPlatRecType> &setRecType);

    ECMS_ERRORCODE UpdatePlatRecCoverPolicy(const TRedisPlatRecCoverPolicy &tPolicy, bool bAutoPublish = true);     // 更新设备的平台录像覆盖策略
    ECMS_ERRORCODE UpdatePlatRecNoStreamCheckParam(u8 byTimes, u16 wInterval, bool bAutoPublish = true);            // 更新平台录像无码流检测参数
    ECMS_ERRORCODE UpdatePlatRecFileCutPolicy(const TRedisPlatRecFileCutPolicy &tPolicy, bool bAutoPublish = true); // 更新平台录像文件切换策略
    ECMS_ERRORCODE UpdatePlatRecGbBlindFlag(bool bFlag);                                                            // 更新平台录像国标设备盲录标志
    ECMS_ERRORCODE UpdatePlatRecReserveDays(const TRedisPlatRecReserveDays &tReserveDays, 
        bool bAutoPublish = true);                                                                                  // 更新全局录像保留天数
    ECMS_ERRORCODE UpdatePlatRecDuration(const TRedisPuRecUnit &tRecUnit, 
        u16 wRecDuration, bool bAutoPublish = true);                                                                // 更新设备的平台录像时长

    ECMS_ERRORCODE UpdatePlatRecPartitionBind(const string &strNruId, const string &strPartitionMountName, 
        const vector<TRedisPuRecUnit> &cRecUnits, const TRedisSdkOperType &tType = RedisSdkOperType_Set);           // 磁盘分区绑定
    ECMS_ERRORCODE GetPlatRecPartitionBind(const string &strNruId, const string &strPartitionMountName, 
        vector<TRedisPuRecUnitWithDomain> &cRecUnits);                                                              // 获取磁盘分区信息

    // UpdatePlatRecBalanceTime这个接口，目前没起到作用，nru不需要（todo by xiongxh）
    ECMS_ERRORCODE UpdatePlatRecBalanceTime(const u16 wInterval, bool bAutoPublish = true);                         // 更新均衡时间间隔
    
    /*
     * pui的数据接口
     */
    ECMS_ERRORCODE PuOnline(const string &strPuiId, const string &strDevId);
    ECMS_ERRORCODE PuOffline(const string &strPuiId, const string &strDevId);

    /*
     * vtdu的数据接口
     */
    ECMS_ERRORCODE UpdateVtduInfo(const string &strVtduId, const TRedisModuleVtdu &tVtduInfo);                      // 更新vtdu信息

    /*
     * rcs的数据接口
     */
    ECMS_ERRORCODE GetNruListByPlatRecUnit(const TRedisPuRecUnit &tRecUnit, vector<TRedisUri> &cNruList);

    ECMS_ERRORCODE AddNru(const string &strNruId, bool bEnable, const TRedisModuleNruBaseInfo &tTopInfo, 
        bool bAutoPublish = true);                                                                                  // 入网nru
    ECMS_ERRORCODE ModNru(const string &strNruId, bool bEnable, const string &strNruName, 
        bool bAutoPublish = true);                                                                                  // 修改已入网的nru信息
    ECMS_ERRORCODE DelNru(const string &strNruId, bool bAutoPublish = true);                                        // 退网nru
    ECMS_ERRORCODE ClearPuPlatRecConfig(const string &strPuId);                                         //清理redis中的pu配置信息(partition-map-rec-unit:xxx && plat-rec-param:xxx:x)

    /*
     * nru的数据接口
     */
    ECMS_ERRORCODE GetNruPlatStatus(const string &strNruId, TRedisNruPlatStatus &tStatus);                          // 获取Nru的平台状态（是否入网、是否启用）                                                                              // 更新nru管理的所有分区信息
    ECMS_ERRORCODE UpdatePartitionInfo(const string &strNruId, const vector<TRedisPartition> &cPartitionInfo, 
        bool bAutoPublish = true);                                                                                  // 更新nru管理的所有分区信息
    ECMS_ERRORCODE UpdateNruInfo(const string &strNruId, const TRedisModuleNru &tNruInfo);                          // 更新nru整体信息
    ECMS_ERRORCODE UpdateNruLoad(const string &strNruId, const TRedisModuleNruLoad &tLoad);                         // 更新nru负载信息
    ECMS_ERRORCODE UpdateNruStreamLimit(const string &strNruId, const TRedisModuleNruStreamLimit &tStreamLimit);    // 更新nru码流限制信息
    ECMS_ERRORCODE UpdateNruStreamCur(const string &strNruId, const TRedisModuleNruStreamCur &tStreamCur);          // 更新nru当前的码流信息
    ECMS_ERRORCODE UpdateNruStreamRec(const string &strNruId, u16 wNum, u64 qwBandWidth);                           // 更新nru当前的录像信息
    ECMS_ERRORCODE UpdateNruStreamPlay(const string &strNruId, u16 wNum, u64 qwBandWidth);                          // 更新nru当前的放像信息
    ECMS_ERRORCODE UpdateNruStreamDownload(const string &strNruId, u16 wNum, u64 qwBandWidth);                      // 更新nru当前的下载信息
    ECMS_ERRORCODE UpdateNruRecordInfo(const string &strNruId, const TRedisModuleNruRecordInfo &tRecordInfo, 
        const TRedisSdkOperType &tType = RedisSdkOperType_Set);                                                     // 更新nru上录像前端的信息
    
    ECMS_ERRORCODE JoinPlatRecSubsystem(const string &strNruId);                                                    // nru加入平台录像子系统
    ECMS_ERRORCODE GetPlatRecTask(const string &strNruId, u32 dwExpectedNum, vector<TRedisPlatRecTask> &cRecTasks, 
        u32 &dwRestSize, u32 &dwUnassignedSize);
    ECMS_ERRORCODE GetPlatRecTaskParam(const TRedisPuRecUnit &tRecUnit, TRedisPlatRecTaskParam &tTaskParam, 
        string &strChnState, string &strVideoEncodeParam, string &strAudioEncodeParam);                             // nru在处理录像任务时获取任务的相关参数（如设备是否在线等）
    ECMS_ERRORCODE ReassignPlatRecTask(const string &strNruId, u32 dwExpectedNum, u32 &dwReassignNum, 
        u32 &dwExpiredNum, u32 &dwRestSize);                                                                        // nru请求将unassigned任务分配出去

    ECMS_ERRORCODE TransferPlatRecTask(const set<TRedisPuRecUnit> &cRecUnits);                                      // nru请求将录像任务中转到unassigned
    ECMS_ERRORCODE TransferNruPlatRecTask(const string &strNruId);                                                  // nru请求将录像任务中转到unassigned
    ECMS_ERRORCODE TransferPartitionPlatRecTask(const string &strNruId, const string &strPartitionMountName);       // nru请求将指定分区上的任务中转到unassigned

    // 需要调整录像任务的几种情况
    // 1. 绑定分区上线 2. 解除绑定分区 3. 其他暂未实现（如均衡的需要）
    ECMS_ERRORCODE AdjustPlatRecTask(const TRedisPuRecUnit &tRecUnit, 
        string &strNewPartitionName, string &strNruId);                                                             // 调整录像任务

    ECMS_ERRORCODE ClearPlatRecTask(const set<TRedisPuRecUnit> &cRecUnits);                                         // 清理录像任务相关数据
    ECMS_ERRORCODE RemoveExpiredPlatRecTask(const string &strNruId, const TRedisPuRecUnit &tRecUnit);               // 移除过期的录像任务
    
    ECMS_ERRORCODE BalancePlatRecTask(TRedisPuRecUnit & recUnit, string &oldNruId, string &newNruId, string &oldPartitionName, string &newPartitionName);   // 请求进行录像任务负载均衡
    ECMS_ERRORCODE GetNruPlatRecTask(const string &strNruId, set<TRedisPuRecUnit> &cRecUnits);                      // 获取指定nru的所有录像任务

    ECMS_ERRORCODE UpdatePlatRecStatus(const string &strNruId, const TRedisPlatRecStatus &tRecStatus, 
        bool bBindRecordToNru = false, bool bAutoPublish = true);                                                   // 更新设备的平台录像状态


    /*
     * proxy接口
     */
    ECMS_ERRORCODE RegisterModule(const TRedisModule &tModule, const TRedisModuleState &tState = RedisModuleState_Start);   // 注册模块
    ECMS_ERRORCODE UnregisterModule(const string &tModuleId, const TRedisModuleState &tState = RedisModuleState_Stop);      // 取消注册模块

    /*
     * 通用数据访问接口
     */
    ECMS_ERRORCODE GetPlatRecTaskAmount(u32 &dwAmount);                                                             // 获取平台正在录像路数
    ECMS_ERRORCODE GetPlatSwitchAmount(u32 &dwAmount);                                                              // 获取平台交换路数
    ECMS_ERRORCODE GetPlatRecCoverPolicy(TRedisPlatRecCoverPolicy &tPolicy);                                        // 获取设备的平台录像覆盖策略
    ECMS_ERRORCODE GetPlatRecNoStreamCheckParam(u8 &byTimes, u16 &wInterval);                                       // 获取平台录像无码流检测参数
    ECMS_ERRORCODE GetPlatRecFileCutPolicy(TRedisPlatRecFileCutPolicy &tPolicy);                                    // 获取平台录像文件切换策略
    ECMS_ERRORCODE GetPlatRecDuration(const TRedisPuRecUnit &tRecUnit, u16 &wRecDuration);                          // 获取设备的平台录像时长
    ECMS_ERRORCODE GetPlatRecReserveDays(TRedisPlatRecReserveDays &tReserveDays);                                   // 获取设备的全局录像保留天数
    ECMS_ERRORCODE GetPlatRecStatusByDevice(const string &strDevId, TRedisDevicePlatRecStatus &tDeviceRecStatus);   // 获取设备的平台录像状态
    ECMS_ERRORCODE GetNruList(vector<TRedisModuleNruDetailInfo> &cDetailInfo, 
        bool bInTopo = true, bool bNotInTopo = true);                                                               // 获取已入网、未入网nru的基础信息
    ECMS_ERRORCODE GetNruDetailInfo(const string &strNruId, TRedisModuleNruDetailInfo &tDetailInfo);                // 获取单个nru的详细信息
    ECMS_ERRORCODE GetPartitionInfo(const string &strNruId, vector<TRedisPartition> &cPartitionInfo);               // 获取指定nru相关的分区信息
    ECMS_ERRORCODE GetPlatRegisterModule(set<string> &cModules);                                                    // 获取平台所有已注册模块
    ECMS_ERRORCODE GetPlatRegisterModuleByType(const TRedisModuleType &tModuleType, set<string> &cModules);         // 获取平台指定类型的已注册模块

    // todo by xiongxh 暂时不需要，脚本没有实现
    ECMS_ERRORCODE GetDomainMap(map<string, string> &cDomainMap);                                                   // 获取平台域关系信息

    /*
     * 临时接口（目前是v2r3b3版本中特有的，后续版本可能会删掉，主要是在新旧交换流程中其兼容作用）
     */
    TRedisSwitchFlow GetSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc, const TRedisSwitchAgent &tDst);
    TRedisSwitchFlow GetSrcSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tSrc);
    TRedisSwitchFlow GetDstSwitchFlow(const TRedisSwitchType &tSwitchType, const TRedisSwitchAgent &tDst);

public:

    void PrintData() const;

public:

    /*
     * Fastdb需要的回调函数相关
     *
     *     1. fastdb所需要的连接状态回调
     *     2. fastdb所需要的订阅回调函数
     */
    static void FastdbStatusCallback(CFastDBInterface *pcInst, EFDB_StatusCode emCode, void *, void *pvUserData);
    static void FastdbSubscribeCallback(CFastDBInterface *pcInst, const string &strChannel, const string &strContent, void *pvUserData);
    static void FastdbCompaSubscribeCallback(CFastDBInterface *pcInst, const string &strChannel, const string &strContent, void *pvUserData);

    // Fastdb回调实际执行业务的函数
    void FastdbDisconnectProc(EFDB_StatusCode emCode);
    void FastdbNotifyProc(const string &strChannel, const string &strContent);              // RedisSdk中的订阅通道的回调处理
    void FastdbCompaNotifyProc(const string &strChannel, const string &strContent);         // Fastdb中定义的订阅通道的回调处理

protected:

    /*
     * lua脚本名、sha1之间的映射转换等
     */
    string          LuaScript2Sha(const string &strScript) const;
    ECMS_ERRORCODE  LoadLua2Sha();

    /*
     * 内部用来执行lua脚本的函数
     */
    ECMS_ERRORCODE ExecuteLuaByScriptContent(const string &strLuaScript, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptContent(const string &strLuaScript, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);
    ECMS_ERRORCODE ExecuteLuaByScriptFile(const string &strFile, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptFile(const string &strFile, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);
    ECMS_ERRORCODE ExecuteLuaByScriptSha(const string &strSha, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams);
    ECMS_ERRORCODE ExecuteLuaByScriptSha(const string &strSha, const vector<string> &cKeyParams, 
        const vector<string> &cArgvParams, vector<string> &cResult);

    /*
     * CRedisSsChannel和字符串之间的转换
     */
    ECMS_ERRORCODE RedisSsChannel2String(const set<CRedisSsChannel> &cSsChn, set<string> &cChnString);
    ECMS_ERRORCODE RedisSsChannel2String(const set<CRedisSsChannel> &cSsChn, vector<string> &cChnString);

    /*
     * 判断指定的订阅通道在App层或者Sdk层是否订阅
     */
    bool IsAppSsChn(const CRedisSsChannel &cSsChn) const { return (m_cAppSsList.end() != m_cAppSsList.find(cSsChn)); }
    bool IsSdkSsChn(const CRedisSsChannel &cSsChn) const { return (m_cSdkSsList.end() != m_cSdkSsList.find(cSsChn)); }

    /*
     * RedisSdk内部处理订阅回调的方法
     */
    void ProcInnerSubscribeNotify(const CRedisSsChannel &cSsChn, const string &strContent);

private:
    CFastDBInterface  m_cFastDBInstance;               // RedisSdk是基于fastdb对特定业务的封装层
    RedisSdkCallback  m_pfCallback;                    // 回调函数，RedisSdk需要主动通知应用层的时候调用
    void *m_pvUserData;                                // 用户数据指针，初始化的时候由上层设置，回调的时候作为pvParam1返回给上层

    set<CRedisSsChannel> m_cSdkSsList;                 // RedisSdk内部的订阅通道列表
    set<CRedisSsChannel> m_cAppSsList;                 // 应用层的订阅通道列表
    set<string>          m_cAppCompaSsList;            // 应用层的兼容订阅通道列表

    /*
     * 作用：保存脚本名到sha1编码的映射（key：lua脚本名；value：sha1编码）
     * 
     * 注意：
     *     为了尽可能提高redis的lua脚本的性能，使用evalsha（而不是eval）来执行lua脚本，
     * 由于sha1编码和lua脚本内容有关（只要lua脚本内容变更，哪怕只是一个空格，对应的sha1编码都将改变），
     * 所以程序中不可能直接使用lua脚本的sha1编码。为了使lua脚本具备动态调整的能力（修改lua脚本不需要重启程序或者redis-server），
     * 采用如下机制：
     *     1. lua脚本和模块代码分离，lua脚本以文本文件形式部署在平台上
     *     2. 每一个lua脚本对应一个具体的功能点，比如选择vtdu功能，对应的lua脚本名为：select-vtdu.lua
     *     3. redis-server启动后，将平台中所有的lua脚本加载到redis中，生成对应的sha1编码，保存到redis中并发布。
     *        比如select-vtdu.lua，使用script load命令将其内容加载到redis中，生成脚本文件select-vtdu.lua到sha1编码xxxxxx的映射，并发布
     *     4. 模块启动后，由RedisSdk订阅这个映射关系，然后获取并缓存这个映射关系的初始数据，保存到m_cLua2Sha中
     *     5. 这样当需要调用某个lua脚本时，RedisSdk只需要使用脚本文件名即可。
     *        比如要调用select-vtdu.lua，程序中写的是需要执行的脚本名select-vtdu.lua，然后由RedisSdk将脚本名映射为sha1编码，然后通过evalsha
     *        命令执行即可。当需要临时调整vtdu的选择算法时，只要修改select-vtdu.lua脚本，然后使用script load命令加载新的脚本，再更新
     *        脚本名和新sha1的映射关系，并发布出去，这样模块就能通过RedisSdk自动感知到新的select-vtdu.lua对应的sha1编码
     */
    map<string, string> m_cLua2Sha;

    // 类内部使用的常量定义(主要是内部要用到的所有lua脚本的名字)
    static const string m_cstrScriptRegisterModule;
    static const string m_cstrScriptUnregisterModule;
    static const string m_cstrScriptSelectCmu;
    static const string m_cstrScriptSelectCui;
    static const string m_cstrScriptSelectPui;
    static const string m_cstrScriptSelectRcs;
    static const string m_cstrScriptSelectVtdu;
    static const string m_cstrScriptSelectVtduEx;
    static const string m_cstrScriptSelectNru;
    static const string m_cstrScriptUpdateModuleLoad;
    static const string m_cstrScriptCleanModulePrivateData;
    static const string m_cstrScriptGetModuleDetailInfo;
    static const string m_cstrScriptAllocVtduResource;
    static const string m_cstrScriptGetVtduResource;
    static const string m_cstrScriptReleaseVtduResource;
    static const string m_cstrScriptUpdatePartitionInfo;
    static const string m_cstrScriptUpdateNruInfo;
    static const string m_cstrScriptUpdateNruLoad;
    static const string m_cstrScriptUpdateNruStreamLimit;
    static const string m_cstrScriptUpdateNruStreamCur;
    static const string m_cstrScriptUpdateNruStreamRec;
    static const string m_cstrScriptUpdateNruStreamPlay;
    static const string m_cstrScriptUpdateNruStreamDownload;
    static const string m_cstrScriptUpdateNruRecordInfo;
    static const string m_cstrScriptUpdatePlatRecCoverPolicy;
    static const string m_cstrScriptUpdatePlatRecNoStreamCheckParam;
    static const string m_cstrScriptUpdatePlatRecBalanceTime;
    static const string m_cstrScriptUpdatePlatRecGbBlindFlag;
    static const string m_cstrScriptUpdatePlatRecReserveDays;
    static const string m_cstrScriptUpdatePlatRecFileCutPolicy;
    static const string m_cstrScriptUpdatePlatRecDuration;
    static const string m_cstrScriptUpdatePlatRecPartitionBind;
    static const string m_cstrScriptGetPlatRecPartitionBind;
    static const string m_cstrScriptUpdatePlatRecStatus;
    static const string m_cstrScriptReassignPlatRecTask;
    static const string m_cstrScriptTransferPlatRecTask;
    static const string m_cstrScriptClearPlatRecTask;
    static const string m_cstrScriptRemoveExpiredPlatRecTask;
    static const string m_cstrScriptTransferNruPlatRecTask;
    static const string m_cstrScriptTransferPartitionPlatRecTask;
    static const string m_cstrScriptAdjustPlatRecTask;
    static const string m_cstrScriptBalancePlatRecTask;
    static const string m_cstrScriptGetNruPlatRecTask;
    static const string m_cstrScriptGetPlatRecCoverPolicy;
    static const string m_cstrScriptGetPlatRecNoStreamCheckParam;
    static const string m_cstrScriptGetPlatRecFileCutPolicy;
    static const string m_cstrScriptGetPlatRecDuration;
    static const string m_cstrScriptGetPlatRecReserveDays;
    static const string m_cstrScriptGetPlatRecStatusByDevice;
    static const string m_cstrScriptJoinPlatRecSubsystem;
    static const string m_cstrScriptSetPlatRecParam;
    static const string m_cstrScriptGetPlatRecParam;
    static const string m_cstrScriptAddPlatRecTask;
	static const string m_cstrScriptDelPlatRecTask;
	static const string m_cstrScriptGetPlatRecTaskTypes;
    static const string m_cstrScriptGetPlatRecTask;
    static const string m_cstrScriptGetNruPlatStatus;
    static const string m_cstrScriptGetPlatRecTaskParam;
    static const string m_cstrScriptUpdateVtduInfo;
    static const string m_cstrScriptGetPlatSwitchAmount;
    static const string m_cstrScriptGetPlatRecTaskAmount;
    static const string m_cstrScriptCuOnline;
    static const string m_cstrScriptCuOffline;
	static const string m_cstrScriptGetCuInfo;
    static const string m_cstrScriptPuOnline;
    static const string m_cstrScriptPuOffline;
    static const string m_cstrScriptGetNruListByPlatRecUnit;
    static const string m_cstrScriptAddNru;
    static const string m_cstrScriptModNru;
    static const string m_cstrScriptDelNru;
    static const string m_cstrScriptClearPuPlatRecConfig;
    static const string m_cstrScriptGetNruList;
    static const string m_cstrScriptGetNruDetailInfo;
    static const string m_cstrScriptGetPartitionInfo;
    static const string m_cstrScriptUpdateCuiInfo;
    static const string m_cstrScriptUpdateCuiOnlineMobileNum;
    static const string m_cstrScriptGetSwitchFlow;
    static const string m_cstrScriptGetPlatRegisterModule;
    static const string m_cstrScriptGetPlatRegisterModuleByType;
    static const string m_cstrScriptGetDomainMap;
};



#endif

