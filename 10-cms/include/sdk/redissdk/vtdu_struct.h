
#ifndef _REDISSDK_VTDU_STRUCT_H
#define _REDISSDK_VTDU_STRUCT_H

#include <string>
#include <set>
using std::string;
using std::set;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/module_struct.h"

typedef set<TRedisModuleAddress> TRedisModuleVtduAddress;

typedef struct TRedisModuleVtdu : public TRedisParam, public TRedisJson
{
    TRedisModuleVtdu() : wDispatchNum(0) {}

    // 序列化和反序列化函数
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json生成和解析函数
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wDispatchNum;                   // 转发路数
    TRedisModuleVtduAddress tAddress;   // 地址信息
}TRedisModuleVtdu;

#endif

