
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

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPlatRecCoverMode tMode;

    // tMode为RedisPlatRecCoverMode_Cover时表示可录像时间（单位：秒）
    u32 dwValue;

    // 磁盘剩余空间的千分比，满足此条件则进入覆盖
    u16 wReservePermil;

    static const u32 cdwDefaultCoverRestRecTime   = 3600;
    static const u32 cdwDefaultCoverReservePermil = 25;
}TRedisPlatRecCoverPolicy;

typedef struct TRedisPlatRecFileCutPolicy : public TRedisParam, public TRedisJson
{
    TRedisPlatRecFileCutPolicy() : tMode(RedisPlatRecFileCutMode_Time), dwValue(cdwDefaultFileCutValue) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPlatRecFileCutMode tMode;

    // tCond为RedisPlatRecFileCutCond_Time时表示时间（单位：秒），为RedisPlatRecFileCutCond_Size时表示大小（单位：M）
    u32 dwValue;

    static const u32 cdwDefaultFileCutValue = 3600;
}TRedisPlatRecFileCutPolicy;

typedef struct TRedisPlatRecReserveDays : public TRedisParam, public TRedisJson
{
    TRedisPlatRecReserveDays() : wReserveDays(cwDefaultReserveDays) {}
    TRedisPlatRecReserveDays(u16 wDays) : wReserveDays(wDays) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
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

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
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

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wBalanceTime;

    static const u16 cwDefaultBalanceTime   = 90;                   //单位分钟
}TRedisPlatBalanceTimePara;

typedef struct TRedisPlatDiskReservePermilPara : public TRedisParam, public TRedisJson
{
    TRedisPlatDiskReservePermilPara() : wDiskReservePermil(cwDefaultDiskReservePermil){}
    TRedisPlatDiskReservePermilPara(u16 wValue) : wDiskReservePermil(wValue){}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wDiskReservePermil;

    static const u16 cwDefaultDiskReservePermil   = 25;                   //单位千分之
}TRedisPlatDiskReservePermilPara;

typedef struct TRedisPlatRecReserveDayPara : public TRedisParam, public TRedisJson
{
    TRedisPlatRecReserveDayPara() : wRecReserveDay(cwDefaultRecReserveDay) {}
    TRedisPlatRecReserveDayPara(u16 wValue) : wRecReserveDay(wValue) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wRecReserveDay;

    static const u16 cwDefaultRecReserveDay   = 10;                   //单位天
}TRedisPlatRecReserveDayPara;

typedef string TRedisLuaScriptMapType;
#define RedisLuaScriptMapType_Update     ("update")
#define RedisLuaScriptMapType_Delete     ("delete")

typedef struct TRedisLuaScriptMap : public TRedisJson
{
    TRedisLuaScriptMap() : tType(RedisLuaScriptMapType_Update) {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisLuaScriptMapType tType;
    string strScript;
    string strSha;
}TRedisLuaScriptMap;

#endif

