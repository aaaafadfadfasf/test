
#ifndef _REDISSDK_SWITCH_STRUCT_H
#define _REDISSDK_SWITCH_STRUCT_H

#include <string>
#include <list>
using std::string;
using std::list;

#include "kdvtype.h"
#include "sdk/redissdk/module_struct.h"

typedef string TRedisSwitchFlow;
#define RedisSwitchFlow_Cmu                   ("cmu")                   // ��cmu���е��ȵ��Ͻ�������
#define RedisSwitchFlow_Redis                 ("redis")                 // v2r3b3�汾���������redis���ȵ��½�������

struct TRedisSwitchAgent : public TRedisParam, public TRedisJson
{
    TRedisSwitchAgent() {}
    TRedisSwitchAgent(const string &strIdVal, s32 nChn) : strId(strIdVal), nChannel(nChn) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;
    s32 nChannel;                             // ͨ��
};

typedef string TRedisSwitchType;
#define RedisSwitchType_General                 ("general")
#define RedisSwitchType_Play                    ("play")
#define RedisSwitchType_Playback                ("playback")
#define RedisSwitchType_Download                ("download")
#define RedisSwitchType_Audiocall               ("audiocall")



#endif

