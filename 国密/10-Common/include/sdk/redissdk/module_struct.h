
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
 * redis�п���ͨ��TRedisModuleIdΨһ�ı�ʶһ��Module
 *
 * ����ʵ��ҵ�����TRedisModuleId�е�strId��ʵ�Ѿ���ȫ��Ψһ���ˣ�
 * ���Ǹ���redis��key����ƹ��򣬻�����Ҫ�ṩTRedisModuleType���ܷ���ı�ʶΨһ��Module
 */
typedef struct TRedisModuleId : public TRedisParam, public TRedisJson
{
    TRedisModuleId() : tModuleType(RedisModuleType_Invalid) {}
    TRedisModuleId(const TRedisModuleType &tTypeVal) : tModuleType(tTypeVal) {}
    TRedisModuleId(const TRedisModuleType &tTypeVal, const string &strIdVal) : tModuleType(tTypeVal), strId(strIdVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // �ַ�����
    string ToString() const;
    void   FromString(const string &strString);

    TRedisModuleType tModuleType;               // ���ͣ����Զ�Module���з��黮��
    string strId;                               // id

    static const string RedisModuleId_Separator;
}TRedisModuleId;

typedef string TRedisModuleAddress;

typedef string TRedisModuleLoad;
#define RedisModuleLoad_Invalid    ("invalid")	// ��Ч����
#define RedisModuleLoad_Low        ("low")		// �͸���
#define RedisModuleLoad_Medium     ("medium")	// �жȸ���
#define RedisModuleLoad_High       ("high")		// �߸���
#define RedisModuleLoad_Max        ("max")		// ������

/*
 * CRedisModule������redis�и���ģ��Ļ�����Ϣ
 *
 * ģ�鲻ͬ��TRedisModuleId��ͬ�������Ȳ��Բ�ͬ��������ĵ��Ȳ��ԣ�������lua�ű���ʵ�ֵ�
 * ����vtdu����ʹ��select-vtdu.lua��Ϊvtdu�ĵ��Ƚű�������Ҫ����vtdu��ʱ����RedisSdk�Զ����øýű�
 */
typedef struct TRedisModule : public TRedisParam, public TRedisJson
{
    TRedisModule() : dwStartTime(0), tLoad(RedisModuleLoad_Low) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleId tId;
    string strDomain;                         // ��id
    u32 dwStartTime;                          // ģ��������ʱ���
	TRedisModuleLoad tLoad;                   // ����
}TRedisModule;

typedef string TRedisModuleResourceId;

typedef struct TRedisModuleVtduResource : public TRedisParam, public TRedisJson
{
    TRedisModuleVtduResource() : dwResourceChn(cdwInvalidChn), dwPortChn(cdwInvalidChn) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
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

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleId tId;
    string strDomainName;
    TRedisModuleState tState;
}TRedisModuleStatus;

typedef struct TRedisCuAgent : public TRedisParam, public TRedisJson
{
    TRedisCuAgent() {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strSession;                          // cu�ĵ�¼session
    string strCuiId;                            // cu��½ʱ��Ӧ��cui��id
    string strUserName;                         // cu�ĵ�¼�û���
    string strAddress;                          // cu��¼ʱ�ͻ�������������ip
	string strCuiIp;                            // cu��¼ʱ��Ӧ��cui��ip
}TRedisCuAgent;

typedef struct TRedisUri : public TRedisParam, public TRedisJson
{
    TRedisUri() {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;
    string strDomainName;
}TRedisUri;

typedef struct TRedisModuleDetailInfo : public TRedisJson
{
    TRedisModuleDetailInfo() {tModule.dwStartTime = 0; tModule.tLoad = RedisModuleLoad_Invalid; }

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModule tModule;
}TRedisModuleDetailInfo;

#endif

