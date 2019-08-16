
#ifndef _REDISSDK_PARTITION_STRUCT_H
#define _REDISSDK_PARTITION_STRUCT_H

#include <string>
using std::string;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"

typedef string TRedisPartitionFormat;
#define RedisPartitionFormat_Unknown        ("unknown")
#define RedisPartitionFormat_Normal         ("normal")
#define RedisPartitionFormat_VBFS           ("vbfs")
#define RedisPartitionFormat_Cloud          ("cloud")

typedef string TRedisPartitionState;
#define RedisPartitionStatus_Offline        ("offline")
#define RedisPartitionStatus_Online         ("online")
#define RedisPartitionStatus_Alarm1         ("alarm1")
#define RedisPartitionStatus_Alarm2         ("alarm2") 

typedef struct TRedisPartition : public TRedisParam, public TRedisJson
{
    TRedisPartition() : tPartitionFormat(RedisPartitionFormat_Unknown), tPartitionState(RedisPartitionStatus_Offline), 
        qwPartitionTotalSize(0), qwPartitionRemainSize(0), qwPartitionLockSize(0), qwPartitionReservedSize(0), 
        dwRemainRecTimeOfStopRecord(0), wPartitionAbnormalTimes(0), wPartitionRecordNum(0), qwPartitionRecordBW(0), 
        dwRemainRecTime(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strPartitionName;                                        // 分区名
    string strPartitionMountName;                                   // 分区挂载路径
    string strStorageVendor;                                        // 存储设备厂商
    TRedisPartitionFormat tPartitionFormat;                         // 分区格式
    TRedisPartitionState tPartitionState;                           // 分区状态
    u64 qwPartitionTotalSize;                                       // 分区总空间大小，单位：M
    u64 qwPartitionRemainSize;                                      // 分区剩余空间大小，单位：M
    u64 qwPartitionLockSize;                                        // 分区锁定空间大小，单位：M
    u64 qwPartitionReservedSize;                                    // 分区保留空间大小，单位：M
    u32 dwRemainRecTimeOfStopRecord;                                // 分区剩余空间可以录像时长阈值（在策略为停录像时，剩余空间不满足该条件，则停录像）
    u16 wPartitionAbnormalTimes;                                    // 分区异常次数
    u16 wPartitionRecordNum;                                        // 分区上录像的数量
    u64 qwPartitionRecordBW;                                        // 分区上录像的带宽，单位：kbps
    u32 dwRemainRecTime;                                            // 分区剩余录像时长，单位：s
}TRedisPartition;

#endif

