
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

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strPartitionName;                                        // ������
    string strPartitionMountName;                                   // ��������·��
    string strStorageVendor;                                        // �洢�豸����
    TRedisPartitionFormat tPartitionFormat;                         // ������ʽ
    TRedisPartitionState tPartitionState;                           // ����״̬
    u64 qwPartitionTotalSize;                                       // �����ܿռ��С����λ��M
    u64 qwPartitionRemainSize;                                      // ����ʣ��ռ��С����λ��M
    u64 qwPartitionLockSize;                                        // ���������ռ��С����λ��M
    u64 qwPartitionReservedSize;                                    // ���������ռ��С����λ��M
    u32 dwRemainRecTimeOfStopRecord;                                // ����ʣ��ռ����¼��ʱ����ֵ���ڲ���Ϊͣ¼��ʱ��ʣ��ռ䲻�������������ͣ¼��
    u16 wPartitionAbnormalTimes;                                    // �����쳣����
    u16 wPartitionRecordNum;                                        // ������¼�������
    u64 qwPartitionRecordBW;                                        // ������¼��Ĵ�����λ��kbps
    u32 dwRemainRecTime;                                            // ����ʣ��¼��ʱ������λ��s
}TRedisPartition;

#endif

