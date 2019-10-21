
#ifndef _REDISSDK_SWITCH_STRUCT_H
#define _REDISSDK_SWITCH_STRUCT_H

#include <string>
#include <list>
using std::string;
using std::list;

#include "kdvtype.h"
#include "sdk/redissdk/module_struct.h"

typedef string TRedisSwitchFlow;
#define RedisSwitchFlow_Cmu                   ("cmu")                   // 由cmu进行调度的老交换流程
#define RedisSwitchFlow_Redis                 ("redis")                 // v2r3b3版本中引入的由redis调度的新交换流程

struct TRedisSwitchAgent : public TRedisParam, public TRedisJson
{
    TRedisSwitchAgent() {}
    TRedisSwitchAgent(const string &strIdVal, s32 nChn) : strId(strIdVal), nChannel(nChn) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;
    s32 nChannel;                             // 通道
};

typedef string TRedisSwitchType;
#define RedisSwitchType_General                 ("general")
#define RedisSwitchType_Play                    ("play")
#define RedisSwitchType_Playback                ("playback")
#define RedisSwitchType_Download                ("download")
#define RedisSwitchType_Audiocall               ("audiocall")



#endif

