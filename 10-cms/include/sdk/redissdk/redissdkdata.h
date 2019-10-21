
#ifndef _REDISSDK_DATA_H
#define _REDISSDK_DATA_H

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <sstream>
using namespace std;

#include "kdvtype.h"
#include "sdk/redissdk/cJSON.h"

enum RedisSdk_Msg
{
    RedisSdk_Msg_Begin = 1000,                  // 起始消息，作为标记用

    RedisSdk_Msg_Redis_Disconnect,              // RedisSdk和redis连接断开（pvParam1为初始化时上层设置的pvUserData，pvParam2为错误码的指针，即const EFDB_StatusCode *，pvParam3为NULL）

    RedisSdk_Msg_Subscribe_Notify,              // 订阅通知（pvParam1为初始化时上层设置的pvUserData，pvParam2为const CRedisSsChannel *， pvParam3为const string *）
    RedisSdk_Msg_Compa_Subscribe_Notify,        // Fastdb兼容订阅通知（pvParam1为初始化时上层设置的pvUserData，pvParam2为const string *， pvParam3为const string *）

    RedisSdk_Msg_Plat_Rec_Task,                 // 平台录像任务分配（pvParam1为初始化时上层设置的pvUserData，pvParam2为const CRedisSsChannel *，即录像任务订阅通道，pvParam3为const string *，表示TRedisPlatRecTaskNotify）

    RedisSdk_Msg_End                            // 结束消息，作为标记用
};

// RedisSdk内部相关订阅接口定义


/*
 * RedisSdk和redis交互时的参数化接口
 *
 * 注意：
 *     1. RedisSdk和redis交互过程中，作为入参需要将数据结构转换为redis命令的参数形式（即vector<string>），类似OOP的序列化
 *     2. redis返回结果后，RedisSdk最初以vector<string>的形式获取到所有的结果，然后再还原为程序需要的数据结构，类似OOP的反序列化
 */
struct TRedisParam
{
    // ToString要求只将自身的数据追加进去，不能修改参数的原有内容
    virtual void ToVector(vector<string> &cString) const = 0;

    // FromString要求从头开始解析，只解析自身需要的数据
    // （如果参数数据太多，则只取前面的；如果参数数据太少，则后面的不解析）
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it) = 0;
};

/*
 * RedisSdk和redis交互时的Json化接口
 *
 * 注意：
 *     1. RedisSdk在发布数据时，需要将高级语言的数据结构转换为字符串，即OOP的序列化
 *     2. RedisSdk收到订阅通知后，需要将字符串转换为高级语言的数据结构，即OOP的反序列化
 */
struct TRedisJson
{
    virtual void ParseJson(cJSON *pcRoot) = 0;
    virtual void GenerateJson(cJSON *pcRoot) const = 0;

    virtual string ToJson() const
    {
        string strJson;
        cJSON *pcRoot = cJSON_CreateObject();
        if (NULL != pcRoot)
        {
            GenerateJson(pcRoot);
			char *pTmp = cJSON_PrintUnformatted(pcRoot);			
			if (pTmp)
			{
				strJson = pTmp;
				cJSON_free(pTmp);
				pTmp = NULL;
			}
            cJSON_Delete(pcRoot);
            pcRoot = NULL;
        }

        return strJson;
    }

    virtual void FromJson(const string &strJson)
    {
        cJSON *pcRoot = cJSON_Parse(strJson.c_str());
        if (NULL != pcRoot)
        {
            ParseJson(pcRoot);

            cJSON_Delete(pcRoot);
            pcRoot = NULL;
        }
    }

    
};

namespace Convert
{
    // 实现基础类型转换为字符串类型，以便序列化
    template <typename T>
    inline string ToString(const T &tData);
    
    // 实现字符串类型转换为基础类型，以便反序列化
    template <typename T>
    inline T FromString(const string &strString);

    /*
     * 实现redis调用的参数、返回值和sdk之间的数据转换
     */
    namespace RedisParam
    {
        namespace To
        {
            // ToVector模板
            template <typename T>
            inline void ToVector(vector<string> &cString, const T &tData);
    
            // ToVector模板偏特化1
            template <typename T>
            inline void ToVector(vector<string> &cString, const vector<T> &tData);
    
            // ToVector模板偏特化2
            template <typename T>
            inline void ToVector(vector<string> &cString, const list<T> &tData);
    
            // ToVector模板偏特化3
            template <typename T>
            inline void ToVector(vector<string> &cString, const set<T> &tData);

            // ToVector模板特化0
            template <>
            inline void ToVector(vector<string> &cString, const bool &tData);
    
            // ToVector模板特化1
            template <>
            inline void ToVector(vector<string> &cString, const u8 &tData);
    
            // ToVector模板特化2
            template <>
            inline void ToVector(vector<string> &cString, const s8 &tData);
    
            // ToVector模板特化3
            template <>
            inline void ToVector(vector<string> &cString, const u16 &tData);
    
            // ToVector模板特化4
            template <>
            inline void ToVector(vector<string> &cString, const s16 &tData);
    
            // ToVector模板特化5
            template <>
            inline void ToVector(vector<string> &cString, const u32 &tData);
    
            // ToVector模板特化6
            template <>
            inline void ToVector(vector<string> &cString, const s32 &tData);
    
            // ToVector模板特化7
            template <>
            inline void ToVector(vector<string> &cString, const u64 &tData);
    
            // ToVector模板特化8
            template <>
            inline void ToVector(vector<string> &cString, const s64 &tData);
    
            // ToVector模板特化9
            template <>
            inline void ToVector(vector<string> &cString, const float &tData);
    
            // ToVector模板特化10
            template <>
            inline void ToVector(vector<string> &cString, const double &tData);
    
            // ToVector模板特化11
            template <>
            inline void ToVector(vector<string> &cString, const string &tData);
    
            // ToVector模板重载
            template <typename T1, typename T2>
            inline void ToVector(vector<string> &cString, const map<T1, T2> &tData);
        }
    
        namespace From
        {
            // FromVector模板
            template <typename T>
            inline void FromVector(T &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板偏特化1
            template <typename T>
            inline void FromVector(vector<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板偏特化2
            template <typename T>
            inline void FromVector(list<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板偏特化3
            template <typename T>
            inline void FromVector(set<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);

            // FromVector模板特化0
            template <>
            inline void FromVector(bool &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化1
            template <>
            inline void FromVector(u8 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化2
            template <>
            inline void FromVector(s8 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化3
            template <>
            inline void FromVector(u16 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化4
            template <>
            inline void FromVector(s16 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化5
            template <>
            inline void FromVector(u32 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化6
            template <>
            inline void FromVector(s32 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化7
            template <>
            inline void FromVector(u64 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化8
            template <>
            inline void FromVector(s64 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化9
            template <>
            inline void FromVector(float &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化10
            template <>
            inline void FromVector(double &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板特化11
            template <>
            inline void FromVector(string &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVector模板重载
            template <typename T1, typename T2>
            inline void FromVector(map<T1, T2> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
        }
    }

    namespace RedisJSON
    {
        namespace To
        {
            // ToJson模板（自动创建根节点）
            template <typename T>
            inline string ToJson(const char *pchNodeName, const T &tData);

            // ToJson模板
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const T &tData);

            // ToJson模板偏特化1
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const vector<T> &tData);

            // ToJson模板偏特化2
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const list<T> &tData);

            // ToJson模板偏特化3
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const set<T> &tData);

            // ToJson模板特化0
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const bool &tData);

            // ToJson模板特化1
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u8 &tData);

            // ToJson模板特化2
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s8 &tData);

            // ToJson模板特化3
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u16 &tData);

            // ToJson模板特化4
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s16 &tData);

            // ToJson模板特化5
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u32 &tData);

            // ToJson模板特化6
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s32 &tData);

            // ToJson模板特化7
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u64 &tData);

            // ToJson模板特化8
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s64 &tData);

            // ToJson模板特化9
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const float &tData);

            // ToJson模板特化10
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const double &tData);

            // ToJson模板特化11
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const string &tData);

            // ToJson模板重载
            template <typename T1, typename T2>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const map<T1, T2> &tData);
        }

        namespace From
        {
            // FromJson模板(自动解析根节点)
            template <typename T>
            inline void FromJson(T &tData, const char *strJson, const char *pchNodeName);

            // FromJson模板
            template <typename T>
            inline void FromJson(T &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板偏特化1
            template <typename T>
            inline void FromJson(vector<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板偏特化2
            template <typename T>
            inline void FromJson(list<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板偏特化3
            template <typename T>
            inline void FromJson(set<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化0
            template <>
            inline void FromJson(bool &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化1
            template <>
            inline void FromJson(u8 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化2
            template <>
            inline void FromJson(s8 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化3
            template <>
            inline void FromJson(u16 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化4
            template <>
            inline void FromJson(s16 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化5
            template <>
            inline void FromJson(u32 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化6
            template <>
            inline void FromJson(s32 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化7
            template <>
            inline void FromJson(u64 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化8
            template <>
            inline void FromJson(s64 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化9
            template <>
            inline void FromJson(float &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化10
            template <>
            inline void FromJson(double &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板特化11
            template <>
            inline void FromJson(string &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJson模板重载
            template <typename T1, typename T2>
            inline void FromJson(map<T1, T2> &tData, cJSON *pcRoot, const char *pchNodeName);
        }
    }
}

template <typename T>
string Convert::ToString(const T &tData)
{
    ostringstream oss;
    oss.precision(20);
    oss << tData;

    return string(oss.str());
}

template <typename T>
T Convert::FromString(const string &strString)
{
    T tData;
    istringstream iss(strString);
    iss.precision(20);
    iss >> tData;

    return tData;
}

template <typename T>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const T &tData)
{
    tData.ToVector(cString);
}

template <typename T>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const vector<T> &tData)
{
    size_t dwSize = tData.size();
    if (cString.capacity() - cString.size() < dwSize + 1)           // vector序列化以后长度为size + 1
    {
        cString.reserve(cString.size() + dwSize + 1);
    }

    cString.push_back(ToString(dwSize));
    for (typename vector<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
    {
        ToVector(cString, *it);
    }
}

template <typename T>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const list<T> &tData)
{
    size_t dwSize = tData.size();
    if (cString.capacity() - cString.size() < dwSize + 1)           // list序列化以后长度为size + 1
    {
        cString.reserve(cString.size() + dwSize + 1);
    }

    cString.push_back(ToString(dwSize));
    for (typename list<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
    {
        ToVector(cString, *it);
    }
}

template <typename T>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const set<T> &tData)
{
    size_t dwSize = tData.size();
    if (cString.capacity() - cString.size() < dwSize + 1)           // set序列化以后长度为size + 1
    {
        cString.reserve(cString.size() + dwSize + 1);
    }

    cString.push_back(ToString(dwSize));
    for (typename set<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
    {
        ToVector(cString, *it);
    }
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const bool &tData)    
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const u8 &tData)    
{
    cString.push_back(ToString((u16)tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const s8 &tData)
{
    cString.push_back(ToString((s16)tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const u16 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const s16 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const u32 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const s32 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const u64 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const s64 &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const float &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const double &tData)
{
    cString.push_back(ToString(tData));
}

template <>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const string &tData)
{
    cString.push_back(tData);
}

template <typename T1, typename T2>
void Convert::RedisParam::To::ToVector(vector<string> &cString, const map<T1, T2> &tData)
{
    size_t dwSize = tData.size();
    if (cString.capacity() - cString.size() < dwSize * 2 + 1)           // map序列化以后长度为size * 2 + 1
    {
        cString.reserve(cString.size() + dwSize * 2 + 1);
    }

    cString.push_back(ToString(dwSize));
    for (typename map<T1, T2>::const_iterator it = tData.begin(); tData.end() != it; ++it)
    {
        ToVector(cString, it->first);
        ToVector(cString, it->second);
    }
}

template <typename T>
void Convert::RedisParam::From::FromVector(T &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    tData.FromVector(cString, it);
}

template <typename T>
void Convert::RedisParam::From::FromVector(vector<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    size_t dwSize = 0;
    if (cString.end() != it) dwSize = FromString<size_t>(*it++);

    if (tData.capacity() - tData.size() < dwSize)
    {
        tData.reserve(tData.size() + dwSize);
    }

    T tDataItem = T();
    for (size_t i = 0; i < dwSize && cString.end() != it; ++i)
    {
        FromVector(tDataItem, cString, it);
        tData.push_back(tDataItem);
    }
}

template <typename T>
void Convert::RedisParam::From::FromVector(list<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    size_t dwSize = 0;
    if (cString.end() != it) dwSize = FromString<size_t>(*it++);

    T tDataItem = T();
    for (size_t i = 0; i < dwSize && cString.end() != it; ++i)
    {
        FromVector(tDataItem, cString, it);
        tData.push_back(tDataItem);
    }
}

template <typename T>
void Convert::RedisParam::From::FromVector(set<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    size_t dwSize = 0;
    if (cString.end() != it) dwSize = FromString<size_t>(*it++);

    T tDataItem = T();
    for (size_t i = 0; i < dwSize && cString.end() != it; ++i)
    {
        FromVector(tDataItem, cString, it);
        tData.insert(tDataItem);
    }
}

template <>
void Convert::RedisParam::From::FromVector(bool &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<bool>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(u8 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = (u8)FromString<u16>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(s8 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = (s8)FromString<s16>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(u16 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<u16>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(s16 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<s16>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(u32 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<u32>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(s32 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<s32>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(u64 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<u64>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(s64 &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<s64>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(float &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<float>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(double &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = FromString<double>(*it++);
}

template <>
void Convert::RedisParam::From::FromVector(string &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    if (cString.end() != it) tData = *it++;
}

template <typename T1, typename T2>
void Convert::RedisParam::From::FromVector(map<T1, T2> &tData, const vector<string> &cString, vector<string>::const_iterator &it)
{
    size_t dwSize = 0;
    if (cString.end() != it) dwSize = FromString<size_t>(*it++);

    T1 tKey = T1();
    for (size_t i = 0; i < dwSize && cString.end() != it; ++i)
    {
        if (i % 2)
        {
            tData[tKey] = FromString<T2>(*it++);
        }
        else
        {
            tKey = FromString<T1>(*it++);
        }
    }
}

template <typename T>
inline string Convert::RedisJSON::To::ToJson(const char *pchNodeName, const T &tData)
{
    string strJson;
    if (NULL != pchNodeName)
    {
        cJSON *pcRoot = cJSON_CreateObject();
        if (NULL != pcRoot)
        {
            ToJson(pcRoot, pchNodeName, tData);           
			char *pTmp = cJSON_PrintUnformatted(pcRoot);
			if (pTmp)
			{
				strJson = pTmp;
				cJSON_free(pTmp);
				pTmp = NULL;
			}
            cJSON_Delete(pcRoot);
            pcRoot = NULL;
        }
    }

    return strJson;
}

template <typename T>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const T &tData)
{
    cJSON *pcNode = cJSON_CreateObject();
    if (NULL != pcNode)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcNode) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcNode);
        tData.GenerateJson(pcNode);
    }
}

template <typename T>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const vector<T> &tData)
{
    cJSON *pcNode = cJSON_CreateArray();
    if (NULL != pcNode)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcNode) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcNode);

        for (typename vector<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
        {
            ToJson(pcNode, NULL, *it);
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const list<T> &tData)
{
    cJSON *pcNode = cJSON_CreateArray();
    if (NULL != pcNode)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcNode) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcNode);

        for (typename list<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
        {
            ToJson(pcNode, NULL, *it);
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const set<T> &tData)
{
    cJSON *pcNode = cJSON_CreateArray();
    if (NULL != pcNode)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcNode) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcNode);

        for (typename set<T>::const_iterator it = tData.begin(); tData.end() != it; ++it)
        {
            ToJson(pcNode, NULL, *it);
        }
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const bool &tData)
{
    cJSON *pcItem = cJSON_CreateBool(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const u8 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const s8 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const u16 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const s16 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const u32 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const s32 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const u64 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const s64 &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const float &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const double &tData)
{
    cJSON *pcItem = cJSON_CreateNumber(tData);
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const string &tData)
{
    cJSON *pcItem = cJSON_CreateString(tData.c_str());
    if (NULL != pcItem)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcItem) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcItem);
    }
}

template <typename T1, typename T2>
inline void Convert::RedisJSON::To::ToJson(cJSON *pcRoot, const char *pchNodeName, const map<T1, T2> &tData)
{
    cJSON *pcNode = cJSON_CreateArray();
    if (NULL != pcNode)
    {
        (NULL == pchNodeName) ? cJSON_AddItemToArray(pcRoot, pcNode) : cJSON_AddItemToObject(pcRoot, pchNodeName, pcNode);

        for (typename map<T1, T2>::const_iterator it = tData.begin(); tData.end() != it; ++it)
        {
            cJSON *pcItem = cJSON_CreateObject();
            if (NULL != pcItem)
            {
                cJSON_AddItemToArray(pcNode, pcItem);
                ToJson(pcItem, "key", it->first);
                ToJson(pcItem, "value", it->second);
            }
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::From::FromJson(T &tData, const char *strJson, const char *pchNodeName)
{
    if (NULL != pchNodeName)
    {
        cJSON *pcRoot = cJSON_Parse(strJson);
        if (NULL != pcRoot)
        {
            FromJson(tData, pcRoot, pchNodeName);

            cJSON_Delete(pcRoot);
            pcRoot = NULL;
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::From::FromJson(T &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode)
    {
        tData.ParseJson(pcNode);
    }
}

template <typename T>
inline void Convert::RedisJSON::From::FromJson(vector<T> &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Array == pcNode->type)
    {
        s32 nSize = cJSON_GetArraySize(pcNode);
        if (tData.capacity() - tData.size() < nSize)               // 空间不够的话提前调整一下，防止数量太多导致频繁调整空间
        {
            tData.reserve(tData.size() + nSize);
        }

        T tItem = T();
        for (s32 i = 0; i < nSize; ++i)
        {
            cJSON *pcItem = cJSON_GetArrayItem(pcNode, i);
            if (NULL != pcItem)
            {
                FromJson(tItem, pcItem, NULL);
                tData.push_back(tItem);
            }
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::From::FromJson(list<T> &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Array == pcNode->type)
    {
        s32 nSize = cJSON_GetArraySize(pcNode);

        T tItem = T();
        for (s32 i = 0; i < nSize; ++i)
        {
            cJSON *pcItem = cJSON_GetArrayItem(pcNode, i);
            if (NULL != pcItem)
            {
                FromJson(tItem, pcItem, NULL);
                tData.push_back(tItem);
            }
        }
    }
}

template <typename T>
inline void Convert::RedisJSON::From::FromJson(set<T> &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Array == pcNode->type)
    {
        s32 nSize = cJSON_GetArraySize(pcNode);

        T tItem = T();
        for (s32 i = 0; i < nSize; ++i)
        {
            cJSON *pcItem = cJSON_GetArrayItem(pcNode, i);
            if (NULL != pcItem)
            {
                FromJson(tItem, pcItem, NULL);
                tData.insert(tItem);
            }
        }
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(bool &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    tData  = (NULL != pcNode && cJSON_True == pcNode->type);
}

template <>
inline void Convert::RedisJSON::From::FromJson(u8 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(s8 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(u16 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(s16 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(u32 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(s32 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(u64 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(s64 &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(float &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(double &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Number == pcNode->type)
    {
        tData = pcNode->valueint;
    }
}

template <>
inline void Convert::RedisJSON::From::FromJson(string &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_String == pcNode->type)
    {
        tData = pcNode->valuestring;
    }
}

template <typename T1, typename T2>
inline void Convert::RedisJSON::From::FromJson(map<T1, T2> &tData, cJSON *pcRoot, const char *pchNodeName)
{
    cJSON *pcNode = (NULL == pchNodeName) ? pcRoot : cJSON_GetObjectItem(pcRoot, pchNodeName);
    if (NULL != pcNode && cJSON_Array == pcNode->type)
    {
        s32 nSize = cJSON_GetArraySize(pcNode);

        T1 tKey = T1();
        T2 tValue = T2();
        for (s32 i = 0; i < nSize; ++i)
        {
            cJSON *pcItem = cJSON_GetArrayItem(pcNode, i);
            if (NULL != pcItem && cJSON_Object == pcNode->type)
            {
                FromJson(tKey,   pcItem, "key");
                FromJson(tValue, pcItem, "value");
                tData[tKey] = tValue;
            }
        }
    }
}

#endif

