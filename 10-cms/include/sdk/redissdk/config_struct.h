
#ifndef _REDISSDK_CONFIG_STRUCT_H
#define _REDISSDK_CONFIG_STRUCT_H

#include <string>
using std::string;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/module_struct.h"

typedef string TRedisPlatRecCoverMode;
#define RedisPlatRecCoverMode_Cover             ("cover")
#define RedisPlatRecCoverMode_Stop              ("stop")

typedef string TRedisPlatRecFileCutMode;
#define RedisPlatRecFileCutMode_Time            ("time")
#define RedisPlatRecFileCutMode_Size            ("size")

typedef struct TRedisPlatRecCoverPolicy : public TRedisParam, public TRedisJson
{
    TRedisPlatRecCoverPolicy() : tMode(RedisPlatRecCoverMode_Cover), 
        dwValue(cdwDefaultCoverRestRecTime), wReservePermil(cdwDefaultCoverReservePermil) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPlatRecCoverMode tMode;

    // tModeΪRedisPlatRecCoverMode_Coverʱ��ʾ��¼��ʱ�䣨��λ���룩
    u32 dwValue;

    // ����ʣ��ռ��ǧ�ֱȣ��������������븲��
    u16 wReservePermil;

    static const u32 cdwDefaultCoverRestRecTime   = 3600;
    static const u32 cdwDefaultCoverReservePermil = 25;
}TRedisPlatRecCoverPolicy;

typedef struct TRedisPlatRecFileCutPolicy : public TRedisParam, public TRedisJson
{
    TRedisPlatRecFileCutPolicy() : tMode(RedisPlatRecFileCutMode_Time), dwValue(cdwDefaultFileCutValue) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPlatRecFileCutMode tMode;

    // tCondΪRedisPlatRecFileCutCond_Timeʱ��ʾʱ�䣨��λ���룩��ΪRedisPlatRecFileCutCond_Sizeʱ��ʾ��С����λ��M��
    u32 dwValue;

    static const u32 cdwDefaultFileCutValue = 3600;
}TRedisPlatRecFileCutPolicy;

typedef struct TRedisPlatRecReserveDays : public TRedisParam, public TRedisJson
{
    TRedisPlatRecReserveDays() : wReserveDays(cwDefaultReserveDays) {}
    TRedisPlatRecReserveDays(u16 wDays) : wReserveDays(wDays) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wReserveDays;

    static const u32 cwDefaultReserveDays = 0;
}TRedisPlatRecReserveDays;

typedef struct TRedisPlatRecNoStreamCheckParam : public TRedisParam, public TRedisJson
{
    TRedisPlatRecNoStreamCheckParam() : byTimes(cbyDefaultCheckTimes), wInterval(cwDefaultCheckInterval) {}
    TRedisPlatRecNoStreamCheckParam(u8 byTimesVal) : byTimes(byTimesVal), wInterval(cwDefaultCheckInterval) {}
    TRedisPlatRecNoStreamCheckParam(u8 byTimesVal, u16 wIntervalVal) : byTimes(byTimesVal), wInterval(wIntervalVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u8  byTimes;
    u16 wInterval;

    static const u8 cbyDefaultCheckTimes   = 3;
    static const u8 cwDefaultCheckInterval = 30;
}TRedisPlatRecNoStreamCheckParam;

typedef struct TRedisPlatBalanceTimePara : public TRedisParam, public TRedisJson
{
    TRedisPlatBalanceTimePara() : wBalanceTime(cwDefaultBalanceTime) {}
    TRedisPlatBalanceTimePara(u16 wValue) : wBalanceTime(wValue) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wBalanceTime;

    static const u16 cwDefaultBalanceTime   = 90;                   //��λ����
}TRedisPlatBalanceTimePara;

typedef struct TRedisPlatDiskReservePermilPara : public TRedisParam, public TRedisJson
{
    TRedisPlatDiskReservePermilPara() : wDiskReservePermil(cwDefaultDiskReservePermil){}
    TRedisPlatDiskReservePermilPara(u16 wValue) : wDiskReservePermil(wValue){}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wDiskReservePermil;

    static const u16 cwDefaultDiskReservePermil   = 25;                   //��λǧ��֮
}TRedisPlatDiskReservePermilPara;

typedef struct TRedisPlatRecReserveDayPara : public TRedisParam, public TRedisJson
{
    TRedisPlatRecReserveDayPara() : wRecReserveDay(cwDefaultRecReserveDay) {}
    TRedisPlatRecReserveDayPara(u16 wValue) : wRecReserveDay(wValue) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wRecReserveDay;

    static const u16 cwDefaultRecReserveDay   = 10;                   //��λ��
}TRedisPlatRecReserveDayPara;

typedef string TRedisLuaScriptMapType;
#define RedisLuaScriptMapType_Update     ("update")
#define RedisLuaScriptMapType_Delete     ("delete")

typedef struct TRedisLuaScriptMap : public TRedisJson
{
    TRedisLuaScriptMap() : tType(RedisLuaScriptMapType_Update) {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisLuaScriptMapType tType;
    string strScript;
    string strSha;
}TRedisLuaScriptMap;

#endif

