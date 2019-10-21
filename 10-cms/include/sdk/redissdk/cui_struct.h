
#ifndef _REDISSDK_CUI_STRUCT_H
#define _REDISSDK_CUI_STRUCT_H

#include <string>
#include <set>
using std::string;
using std::set;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"


typedef struct TRedisModuleCui : public TRedisParam, public TRedisJson
{
    TRedisModuleCui() : dwMaxMobileNum(0), dwOnlineMobileNum(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u32 dwMaxMobileNum;                         // 最大移动客户端接入数
    u32 dwOnlineMobileNum;                      // 当前在线移动客户端数量
}TRedisModuleCui;

#endif

