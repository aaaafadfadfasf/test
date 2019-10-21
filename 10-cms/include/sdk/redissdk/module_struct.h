
#ifndef _REDISSDK_MODULE_STRUCT_H
#define _REDISSDK_MODULE_STRUCT_H

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

#include "kdvtype.h"
#include "cms/utility/cmstime.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/redissdkinner.h"

typedef string TRedisModuleType;
#define RedisModuleType_Invalid    ("")
#define RedisModuleType_Cui        ("cui")
#define RedisModuleType_Cmu        ("cmu")
#define RedisModuleType_Pui        ("pui")
#define RedisModuleType_Vtdu       ("vtdu")
#define RedisModuleType_Nru        ("nru")
#define RedisModuleType_Rcs        ("rcs")
#define RedisModuleType_SipProxy   ("sipproxy")
#define RedisModuleType_RecRecover ("RecoverRecover")

/*
 * redis中可以通过TRedisModuleId唯一的标识一个Module
 *
 * 根据实际业务规则，TRedisModuleId中的strId其实已经是全局唯一的了，
 * 但是根据redis中key的设计规则，还是需要提供TRedisModuleType才能方便的标识唯一的Module
 */
typedef struct TRedisModuleId : public TRedisParam, public TRedisJson
{
    TRedisModuleId() : tModuleType(RedisModuleType_Invalid) {}
    TRedisModuleId(const TRedisModuleType &tTypeVal) : tModuleType(tTypeVal) {}
    TRedisModuleId(const TRedisModuleType &tTypeVal, const string &strIdVal) : tModuleType(tTypeVal), strId(strIdVal) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // 字符串化
    string ToString() const;
    void   FromString(const string &strString);

    TRedisModuleType tModuleType;               // 类型，可以对Module进行分组划分
    string strId;                               // id

    static const string RedisModuleId_Separator;
}TRedisModuleId;

typedef string TRedisModuleAddress;

typedef string TRedisModuleLoad;
#define RedisModuleLoad_Invalid    ("invalid")	// 无效负载
#define RedisModuleLoad_Low        ("low")		// 低负载
#define RedisModuleLoad_Medium     ("medium")	// 中度负载
#define RedisModuleLoad_High       ("high")		// 高负载
#define RedisModuleLoad_Max        ("max")		// 满负荷

/*
 * CRedisModule抽象了redis中各个模块的基础信息
 *
 * 模块不同（TRedisModuleId不同），调度策略不同，而具体的调度策略，则是由lua脚本来实现的
 * 比如vtdu，则使用select-vtdu.lua作为vtdu的调度脚本，当需要调度vtdu的时候，由RedisSdk自动调用该脚本
 */
typedef struct TRedisModule : public TRedisParam, public TRedisJson
{
    TRedisModule() : dwStartTime(0), tLoad(RedisModuleLoad_Low) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleId tId;
    string strDomain;                         // 域id
    u32 dwStartTime;                          // 模块启动的时间戳
	TRedisModuleLoad tLoad;                   // 负载
}TRedisModule;

typedef string TRedisModuleResourceId;

typedef struct TRedisModuleVtduResource : public TRedisParam, public TRedisJson
{
    TRedisModuleVtduResource() : dwResourceChn(cdwInvalidChn), dwPortChn(cdwInvalidChn) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;
    u32 dwResourceChn;
    u32 dwPortChn;

    static const u32 cdwInvalidChn = 0xFFFFFFFF;
}TRedisModuleVtduResource;

typedef string TRedisModuleState;
const string RedisModuleState_Invalid("invalid");
const string RedisModuleState_Start("start");
const string RedisModuleState_Restart("restart");
const string RedisModuleState_Stop("stop");

typedef struct TRedisModuleStatus : public TRedisParam, public TRedisJson
{
    TRedisModuleStatus() : tState(RedisModuleState_Start) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleId tId;
    string strDomainName;
    TRedisModuleState tState;
}TRedisModuleStatus;

typedef struct TRedisCuAgent : public TRedisParam, public TRedisJson
{
    TRedisCuAgent() {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strSession;                          // cu的登录session
    string strCuiId;                            // cu登陆时对应的cui的id
    string strUserName;                         // cu的登录用户名
    string strAddress;                          // cu登录时客户端所在主机的ip
	string strCuiIp;                            // cu登录时对应的cui的ip
}TRedisCuAgent;

typedef struct TRedisUri : public TRedisParam, public TRedisJson
{
    TRedisUri() {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;
    string strDomainName;
}TRedisUri;

typedef struct TRedisModuleDetailInfo : public TRedisJson
{
    TRedisModuleDetailInfo() {tModule.dwStartTime = 0; tModule.tLoad = RedisModuleLoad_Invalid; }

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModule tModule;
}TRedisModuleDetailInfo;

#endif

