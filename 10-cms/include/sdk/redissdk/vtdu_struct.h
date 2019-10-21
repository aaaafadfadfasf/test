
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

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wDispatchNum;                   // ת��·��
    TRedisModuleVtduAddress tAddress;   // ��ַ��Ϣ
}TRedisModuleVtdu;

#endif

