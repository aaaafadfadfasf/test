
#ifndef _REDISSDK_NRU_STRUCT_H
#define _REDISSDK_NRU_STRUCT_H

#include <string>
#include <set>
using std::string;
using std::set;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/record_struct.h"
#include "sdk/redissdk/module_struct.h"

typedef string TRedisModuleNruRunMode;                              // nru的运行模式（nru模式或者iScsi模式）
#define RedisModuleNruRunMode_Nru         ("nru")
#define RedisModuleNruRunMode_iScsi       ("iScsi")

typedef TRedisModuleAddress TRedisModuleNruInTopoAddress;           // nru的入网地址信息

typedef struct TRedisModuleNruBaseInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruBaseInfo() {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strName;                                                 // 入网名称
    string strModel;                                                // 型号
    string strManufacturer;                                         // 厂商
    TRedisModuleNruInTopoAddress tAddress;                          // 入网地址
}TRedisModuleNruBaseInfo;

typedef struct TRedisModuleNruLoad : public TRedisParam, public TRedisJson
{
    TRedisModuleNruLoad() : byMemUsage(0), byCpuUsage(0), qwInLost(0), qwFullLost(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u8  byMemUsage;                                                 // 内存使用率
    u8  byCpuUsage;                                                 // cpu使用率
    u64 qwInLost;                                                   // 录像时由于网络或者cpu过高导致的丢包数
    u64 qwFullLost;                                                 // 录像时录像缓冲满引起的丢包数
}TRedisModuleNruLoad;

typedef struct TRedisModuleNruStreamLimit : public TRedisParam, public TRedisJson
{
    TRedisModuleNruStreamLimit() : qwPreRecBandWidth(0), dwRecNum(0), qwRecBandWidth(0), wPlayNum(0), wDownloadNum(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u64 qwPreRecBandWidth;                                          // 预录带宽阈值（单位：kbps）
    u32 dwRecNum;                                                   // 录像路数阈值
    u64 qwRecBandWidth;                                             // 录像带宽阈值（单位：kbps）
    u16 wPlayNum;                                                   // 放像路数限制
    u16 wDownloadNum;                                               // 下载路数限制
}TRedisModuleNruStreamLimit;

typedef struct TRedisModuleNruStreamCur : public TRedisParam, public TRedisJson
{
    TRedisModuleNruStreamCur() : wPreRecNum(0), qwPreRecBandWidth(0), wRecNum(0), qwRecBandWidth(0), 
        wPlayNum(0), qwPlayBandWidth(0), wDownloadNum(0), qwDownloadBandWidth(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wPreRecNum;                                                 // 预录路数
    u64 qwPreRecBandWidth;                                          // 预录带宽（单位：kbps）
    u16 wRecNum;                                                    // 录像路数
    u64 qwRecBandWidth;                                             // 录像带宽（单位：kbps）
    u16 wPlayNum;                                                   // 放像路数
    u64 qwPlayBandWidth;                                            // 放像带宽（单位：kbps）
    u16 wDownloadNum;                                               // 下载路数
    u64 qwDownloadBandWidth;                                        // 下载带宽（单位：kbps）
}TRedisModuleNruStreamCur;

/*
 * 当前版本的实现中，nru只上报曾经为哪些前端进行过录像，而不上报相关的时间信息，后续版本有需要再优化设计
 */
typedef struct TRedisModuleNruRecordInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruRecordInfo() {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    set<TRedisPuRecUnit> cRecUnits;                                   // nru下所进行过录像的前端列表
}TRedisModuleNruRecordInfo;

typedef struct TRedisModuleNruPartitionInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruPartitionInfo() {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    set<string> cPartitionMountName;                                    // nru所管理的磁盘分区列表
}TRedisModuleNruPartitionInfo;

typedef set<TRedisModuleAddress> TRedisModuleNruAddress;                // nru的运行时地址信息

typedef struct TRedisModuleNru : public TRedisParam, public TRedisJson
{
    TRedisModuleNru() : tRunMode(RedisModuleNruRunMode_Nru) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleNruRunMode tRunMode;                                // 运行模式（即nru模式或iScsi模式）
    TRedisModuleNruAddress tAddress;                                // nru的地址信息
    TRedisModuleNruLoad    tLoad;                                   // nru负载情况
    TRedisModuleNruStreamLimit   tStreamLimit;                      // nru资源阈值
    TRedisModuleNruStreamCur     tStreamCur;                        // nru当前的资源使用
    TRedisModuleNruRecordInfo    tRecordInfo;                       // nru所进行过录像的前端列表
}TRedisModuleNru;

typedef struct TRedisModuleNruDetailInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruDetailInfo() : bEnable(false), bOnline(false) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;                                                   // nru的id
    bool bEnable;                                                   // nru的启用标志
    bool bOnline;                                                   // nru的在线状态
    TRedisModuleNruAddress  tAddress;                               // nru的运行时地址信息
    TRedisModuleNruBaseInfo tBaseInfo;                              // nru的入网信息
}TRedisModuleNruDetailInfo;

typedef string TRedisModuleNruBusiness;                             // nru的业务类型
#define RedisModuleNruBusiness_Normal       ("normal")
#define RedisModuleNruBusiness_Playback     ("playback")
#define RedisModuleNruBusiness_Download     ("download")


typedef struct TRedisModuleNruSelectCond : public TRedisParam, public TRedisJson
{
    TRedisModuleNruSelectCond() :tBusiness(RedisModuleNruBusiness_Normal){}
    TRedisModuleNruSelectCond(const string &strPartitionMountNameVal, const TRedisModuleNruBusiness &tBusinessVal)
        : strPartitionMountName(strPartitionMountNameVal),tBusiness(tBusinessVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleNruBusiness tBusiness;
    string strPartitionMountName;
}TRedisModuleNruSelectCond;

typedef string TRedisTopoOperation;
#define RedisTopoOperation_Add     ("add")
#define RedisTopoOperation_Mod     ("mod")
#define RedisTopoOperation_Del     ("mod")

typedef struct TRedisNruTopoNotify : public TRedisJson
{
    TRedisNruTopoNotify() : tOperation(RedisTopoOperation_Add) {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisTopoOperation tOperation;                     // 操作类型
    TRedisModuleNruDetailInfo tDetailInfo;              // 变更后的详细信息
}TRedisNruTopoNotify;

typedef struct TRedisNruPlatStatus : public TRedisJson
{
    TRedisNruPlatStatus() : bInTopo(false), bEnable(false) {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    bool bInTopo;                                       // 是否入网
    bool bEnable;                                       // 是否启用
}TRedisNruPlatStatus;

#endif

