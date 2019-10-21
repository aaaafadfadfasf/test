
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

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u32 dwMaxMobileNum;                         // ����ƶ��ͻ��˽�����
    u32 dwOnlineMobileNum;                      // ��ǰ�����ƶ��ͻ�������
}TRedisModuleCui;

#endif

