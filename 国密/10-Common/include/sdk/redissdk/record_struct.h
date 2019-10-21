
#ifndef _REDISSDK_RECORD_STRUCT_H
#define _REDISSDK_RECORD_STRUCT_H

#include <string>
using std::string;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "cms/cms_errorcode.h"

typedef string TRedisPuRecState;
#define RedisPuRecordState_Trying            ("trying")
#define RedisPuRecordState_OK                ("ok")
#define RedisPuRecordState_Stopped           ("stopped")

typedef string TRedisPuRecMode;
#define RedisPuRecMode_All                   ("RCS_RECMODE_ALL")                // 既录视频又录音频
#define RedisPuRecMode_Video_Only            ("RCS_RECMODE_ONLYVIDEO")          // 只录视频
#define RedisPuRecMode_Frame_Only            ("RCS_RECMODE_ONLYKEYFRAME")       // 只录关键帧
#define RedisPuRecMode_Unknown               ("RCS_RECMODE_UNKNOWN")            // 未知模式

typedef struct TRedisPuRecUnit : public TRedisParam, public TRedisJson
{
    TRedisPuRecUnit() : nChannel(0) {}
    TRedisPuRecUnit(const string &strIdVal, s32 nChannelVal = 0) : strId(strIdVal), nChannel(nChannelVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // 字符串化
    string ToString() const;
    void   FromString(const string &strString);

    bool operator<(const TRedisPuRecUnit &tRecUnit) const;
    bool operator==(const TRedisPuRecUnit &tRecUnit) const;

    string strId;                               // 设备id
    s32 nChannel;                               // 通道

    static const string RedisPuRecUnit_Separator;

}TRedisPuRecUnit;

//用于查询分区绑定信息
typedef struct TRedisPuRecUnitWithDomain : public TRedisParam, public TRedisJson
{
    TRedisPuRecUnitWithDomain(){}
    TRedisPuRecUnitWithDomain(const TRedisPuRecUnit &tRecUnitVal, const string &strDomainNameVal)
        : tRecUnit(tRecUnitVal), strDomainName(strDomainNameVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit     tRecUnit;                  // 前端录像通道
    string              strDomainName;             // 域名
}TRedisPuRecUnitWithDomain;

typedef struct TRedisPlatRecStatus : public TRedisParam, public TRedisJson
{
    TRedisPlatRecStatus() : tRecState(RedisPuRecordState_Stopped), dwErrCode(CMS_SUCCESS) {}
    TRedisPlatRecStatus(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal) {}
    TRedisPlatRecStatus(const TRedisPuRecUnit &tRecUnitVal, const TRedisPuRecState &tStateVal, u32 dwErrCodeVal)
        : tRecUnit(tRecUnitVal), tRecState(tStateVal), dwErrCode(dwErrCodeVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // 重载小于号，这样可以以set的形式保存TRedisPlatRecStatus
    bool operator<(const TRedisPlatRecStatus &tRecStatus) const;

    TRedisPuRecUnit  tRecUnit;                  // 前端录像通道
    TRedisPuRecState tRecState;                 // 录像状态
    u32              dwErrCode;                 // 错误码（使用平台错误码）
}TRedisPlatRecStatus;

typedef struct TRedisDevicePlatRecStatus : public TRedisJson
{
    TRedisDevicePlatRecStatus() {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;                               // 设备的id
    string strDomain;                           // 域名
    set<TRedisPlatRecStatus> cRecStatus;        // 录像状态
}TRedisDevicePlatRecStatus;

typedef struct TRedisPlatRecDuration : public TRedisParam, public TRedisJson
{
    TRedisPlatRecDuration() : wRecDuration(cwDefaultRecDuration) {}
    TRedisPlatRecDuration(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal), wRecDuration(cwDefaultRecDuration) {}
    TRedisPlatRecDuration(const TRedisPuRecUnit &tRecUnitVal, u16 wRecDurationVal)
        : tRecUnit(tRecUnitVal), wRecDuration(wRecDurationVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit  tRecUnit;                  // 前端录像通道
    u16 wRecDuration;                           // 录像时长

    static const u16 cwDefaultRecDuration = 30;
}TRedisPlatRecDuration;

typedef u8 TReidsPlatRecStreamType;
#define ReidsPlatRecStreamType_Major       (0)
#define ReidsPlatRecStreamType_Minor1      (1)
#define ReidsPlatRecStreamType_Minor2      (2)
#define ReidsPlatRecStreamType_Minor3      (3)

typedef struct TRedisPlatRecParam : public TRedisParam, public TRedisJson
{
    TRedisPlatRecParam() : tRecMode(RedisPuRecMode_All), nPreRecDuration(0), nAlarmRecDelayTime(60), 
        bEnableRecDuration(false), wRecDuration(0), tStreamType(ReidsPlatRecStreamType_Major){}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecMode tRecMode;                       // 录像模式（只录视频，只录关键帧等）
    s32 nPreRecDuration;                            // 预录时长（单位：秒）
    s32 nAlarmRecDelayTime;                         // 告警录像停止延迟时间（单位：秒）
    bool bEnableRecDuration;                        // 是否启用录像时长
    u16 wRecDuration;                               // 录像时长（单位：天）
    TReidsPlatRecStreamType tStreamType;            // 录像码流类型（主流、辅流等）
    vector<string> cPlanTimeList;                   // 定时录像时间段（内容为CCronTime ToString()的结果）
}TRedisPlatRecParam;

typedef string TRedisPlatRecType;
#define RedisPlatRecType_Manual         ("manual")
#define RedisPlatRecType_Plan           ("plan")
#define RedisPlatRecType_Alarm          ("alarm")

typedef string TRedisPlatRecOperType;
#define RedisPlatRecOperType_Start       ("start")
#define RedisPlatRecOperType_Stop        ("stop")
#define RedisPlatRecOperType_Transfer    ("transfer")
#define RedisPlatRecOperType_Clear       ("clear")

typedef struct TRedisPlatRecTask : public TRedisParam, public TRedisJson
{
    TRedisPlatRecTask() : tOperType(RedisPlatRecOperType_Start) {}
    TRedisPlatRecTask(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal), 
        tOperType(RedisPlatRecOperType_Start) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit tRecUnit;                       // 前端录像通道
    string strDomain;                               // 前端设备所在域id
    string strNruId;                                // 录像所分配的nru
    string strPartitionMountName;                   // 录像所在分区
    set<TRedisPlatRecType> cRecTypes;               // 录像类型
    TRedisPlatRecOperType tOperType;                // 操作类型
    TRedisPlatRecParam tRecParam;                   // 录像参数
}TRedisPlatRecTask;

typedef struct TRedisPlatRecTaskParam : public TRedisJson
{
    TRedisPlatRecTaskParam() : bOnline(false), bGbDev(false), bGbBlindFlag(false) {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    bool bOnline;                                   // 前端设备在线标志
    string strDevName;                              // 设备名称
    string strDevIp;                                // 设备的ip
    string strDevManufacturer;                      // 设备厂商
    bool   bGbDev;                                  // 国标标志（true：国标设备；false：非国标设备）
    bool   bGbBlindFlag;                            // 国标盲录标志（true：国标设备盲录；false：国标设备非盲录）
    TRedisPlatRecParam tRecParam;                   // 录像参数
}TRedisPlatRecTaskParam;

typedef struct TRedisPlatRecTaskNotify : public TRedisJson
{
    TRedisPlatRecTaskNotify() : dwQueueSize(0), dwUnassignedSize(0) {}
    TRedisPlatRecTaskNotify(const string &strId) : strNruId(strId), dwQueueSize(0), dwUnassignedSize(0) {}
    TRedisPlatRecTaskNotify(const string &strId, u32 dwQueueSizeVal, u32 dwUnassignedSizeVal)
        : strNruId(strId), dwQueueSize(dwQueueSizeVal), dwUnassignedSize(dwUnassignedSizeVal) {}

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strNruId;                                // nru的id
    u32 dwQueueSize;                                // 队列长度
    u32 dwUnassignedSize;                           // 未分配任务数量
}TRedisPlatRecTaskNotify;

#endif

