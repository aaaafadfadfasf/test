
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
    RedisSdk_Msg_Begin = 1000,                  // ��ʼ��Ϣ����Ϊ�����

    RedisSdk_Msg_Redis_Disconnect,              // RedisSdk��redis���ӶϿ���pvParam1Ϊ��ʼ��ʱ�ϲ����õ�pvUserData��pvParam2Ϊ�������ָ�룬��const EFDB_StatusCode *��pvParam3ΪNULL��

    RedisSdk_Msg_Subscribe_Notify,              // ����֪ͨ��pvParam1Ϊ��ʼ��ʱ�ϲ����õ�pvUserData��pvParam2Ϊconst CRedisSsChannel *�� pvParam3Ϊconst string *��
    RedisSdk_Msg_Compa_Subscribe_Notify,        // Fastdb���ݶ���֪ͨ��pvParam1Ϊ��ʼ��ʱ�ϲ����õ�pvUserData��pvParam2Ϊconst string *�� pvParam3Ϊconst string *��

    RedisSdk_Msg_Plat_Rec_Task,                 // ƽ̨¼��������䣨pvParam1Ϊ��ʼ��ʱ�ϲ����õ�pvUserData��pvParam2Ϊconst CRedisSsChannel *����¼��������ͨ����pvParam3Ϊconst string *����ʾTRedisPlatRecTaskNotify��

    RedisSdk_Msg_End                            // ������Ϣ����Ϊ�����
};

// RedisSdk�ڲ���ض��Ľӿڶ���


/*
 * RedisSdk��redis����ʱ�Ĳ������ӿ�
 *
 * ע�⣺
 *     1. RedisSdk��redis���������У���Ϊ�����Ҫ�����ݽṹת��Ϊredis����Ĳ�����ʽ����vector<string>��������OOP�����л�
 *     2. redis���ؽ����RedisSdk�����vector<string>����ʽ��ȡ�����еĽ����Ȼ���ٻ�ԭΪ������Ҫ�����ݽṹ������OOP�ķ����л�
 */
struct TRedisParam
{
    // ToStringҪ��ֻ�����������׷�ӽ�ȥ�������޸Ĳ�����ԭ������
    virtual void ToVector(vector<string> &cString) const = 0;

    // FromStringҪ���ͷ��ʼ������ֻ����������Ҫ������
    // �������������̫�࣬��ֻȡǰ��ģ������������̫�٣������Ĳ�������
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it) = 0;
};

/*
 * RedisSdk��redis����ʱ��Json���ӿ�
 *
 * ע�⣺
 *     1. RedisSdk�ڷ�������ʱ����Ҫ���߼����Ե����ݽṹת��Ϊ�ַ�������OOP�����л�
 *     2. RedisSdk�յ�����֪ͨ����Ҫ���ַ���ת��Ϊ�߼����Ե����ݽṹ����OOP�ķ����л�
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
    // ʵ�ֻ�������ת��Ϊ�ַ������ͣ��Ա����л�
    template <typename T>
    inline string ToString(const T &tData);
    
    // ʵ���ַ�������ת��Ϊ�������ͣ��Ա㷴���л�
    template <typename T>
    inline T FromString(const string &strString);

    /*
     * ʵ��redis���õĲ���������ֵ��sdk֮�������ת��
     */
    namespace RedisParam
    {
        namespace To
        {
            // ToVectorģ��
            template <typename T>
            inline void ToVector(vector<string> &cString, const T &tData);
    
            // ToVectorģ��ƫ�ػ�1
            template <typename T>
            inline void ToVector(vector<string> &cString, const vector<T> &tData);
    
            // ToVectorģ��ƫ�ػ�2
            template <typename T>
            inline void ToVector(vector<string> &cString, const list<T> &tData);
    
            // ToVectorģ��ƫ�ػ�3
            template <typename T>
            inline void ToVector(vector<string> &cString, const set<T> &tData);

            // ToVectorģ���ػ�0
            template <>
            inline void ToVector(vector<string> &cString, const bool &tData);
    
            // ToVectorģ���ػ�1
            template <>
            inline void ToVector(vector<string> &cString, const u8 &tData);
    
            // ToVectorģ���ػ�2
            template <>
            inline void ToVector(vector<string> &cString, const s8 &tData);
    
            // ToVectorģ���ػ�3
            template <>
            inline void ToVector(vector<string> &cString, const u16 &tData);
    
            // ToVectorģ���ػ�4
            template <>
            inline void ToVector(vector<string> &cString, const s16 &tData);
    
            // ToVectorģ���ػ�5
            template <>
            inline void ToVector(vector<string> &cString, const u32 &tData);
    
            // ToVectorģ���ػ�6
            template <>
            inline void ToVector(vector<string> &cString, const s32 &tData);
    
            // ToVectorģ���ػ�7
            template <>
            inline void ToVector(vector<string> &cString, const u64 &tData);
    
            // ToVectorģ���ػ�8
            template <>
            inline void ToVector(vector<string> &cString, const s64 &tData);
    
            // ToVectorģ���ػ�9
            template <>
            inline void ToVector(vector<string> &cString, const float &tData);
    
            // ToVectorģ���ػ�10
            template <>
            inline void ToVector(vector<string> &cString, const double &tData);
    
            // ToVectorģ���ػ�11
            template <>
            inline void ToVector(vector<string> &cString, const string &tData);
    
            // ToVectorģ������
            template <typename T1, typename T2>
            inline void ToVector(vector<string> &cString, const map<T1, T2> &tData);
        }
    
        namespace From
        {
            // FromVectorģ��
            template <typename T>
            inline void FromVector(T &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ��ƫ�ػ�1
            template <typename T>
            inline void FromVector(vector<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ��ƫ�ػ�2
            template <typename T>
            inline void FromVector(list<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ��ƫ�ػ�3
            template <typename T>
            inline void FromVector(set<T> &tData, const vector<string> &cString, vector<string>::const_iterator &it);

            // FromVectorģ���ػ�0
            template <>
            inline void FromVector(bool &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�1
            template <>
            inline void FromVector(u8 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�2
            template <>
            inline void FromVector(s8 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�3
            template <>
            inline void FromVector(u16 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�4
            template <>
            inline void FromVector(s16 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�5
            template <>
            inline void FromVector(u32 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�6
            template <>
            inline void FromVector(s32 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�7
            template <>
            inline void FromVector(u64 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�8
            template <>
            inline void FromVector(s64 &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�9
            template <>
            inline void FromVector(float &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�10
            template <>
            inline void FromVector(double &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ���ػ�11
            template <>
            inline void FromVector(string &tData, const vector<string> &cString, vector<string>::const_iterator &it);
    
            // FromVectorģ������
            template <typename T1, typename T2>
            inline void FromVector(map<T1, T2> &tData, const vector<string> &cString, vector<string>::const_iterator &it);
        }
    }

    namespace RedisJSON
    {
        namespace To
        {
            // ToJsonģ�壨�Զ��������ڵ㣩
            template <typename T>
            inline string ToJson(const char *pchNodeName, const T &tData);

            // ToJsonģ��
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const T &tData);

            // ToJsonģ��ƫ�ػ�1
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const vector<T> &tData);

            // ToJsonģ��ƫ�ػ�2
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const list<T> &tData);

            // ToJsonģ��ƫ�ػ�3
            template <typename T>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const set<T> &tData);

            // ToJsonģ���ػ�0
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const bool &tData);

            // ToJsonģ���ػ�1
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u8 &tData);

            // ToJsonģ���ػ�2
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s8 &tData);

            // ToJsonģ���ػ�3
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u16 &tData);

            // ToJsonģ���ػ�4
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s16 &tData);

            // ToJsonģ���ػ�5
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u32 &tData);

            // ToJsonģ���ػ�6
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s32 &tData);

            // ToJsonģ���ػ�7
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const u64 &tData);

            // ToJsonģ���ػ�8
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const s64 &tData);

            // ToJsonģ���ػ�9
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const float &tData);

            // ToJsonģ���ػ�10
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const double &tData);

            // ToJsonģ���ػ�11
            template <>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const string &tData);

            // ToJsonģ������
            template <typename T1, typename T2>
            inline void ToJson(cJSON *pcRoot, const char *pchNodeName, const map<T1, T2> &tData);
        }

        namespace From
        {
            // FromJsonģ��(�Զ��������ڵ�)
            template <typename T>
            inline void FromJson(T &tData, const char *strJson, const char *pchNodeName);

            // FromJsonģ��
            template <typename T>
            inline void FromJson(T &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ��ƫ�ػ�1
            template <typename T>
            inline void FromJson(vector<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ��ƫ�ػ�2
            template <typename T>
            inline void FromJson(list<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ��ƫ�ػ�3
            template <typename T>
            inline void FromJson(set<T> &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�0
            template <>
            inline void FromJson(bool &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�1
            template <>
            inline void FromJson(u8 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�2
            template <>
            inline void FromJson(s8 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�3
            template <>
            inline void FromJson(u16 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�4
            template <>
            inline void FromJson(s16 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�5
            template <>
            inline void FromJson(u32 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�6
            template <>
            inline void FromJson(s32 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�7
            template <>
            inline void FromJson(u64 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�8
            template <>
            inline void FromJson(s64 &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�9
            template <>
            inline void FromJson(float &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�10
            template <>
            inline void FromJson(double &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ���ػ�11
            template <>
            inline void FromJson(string &tData, cJSON *pcRoot, const char *pchNodeName);

            // FromJsonģ������
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
    if (cString.capacity() - cString.size() < dwSize + 1)           // vector���л��Ժ󳤶�Ϊsize + 1
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
    if (cString.capacity() - cString.size() < dwSize + 1)           // list���л��Ժ󳤶�Ϊsize + 1
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
    if (cString.capacity() - cString.size() < dwSize + 1)           // set���л��Ժ󳤶�Ϊsize + 1
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
    if (cString.capacity() - cString.size() < dwSize * 2 + 1)           // map���л��Ժ󳤶�Ϊsize * 2 + 1
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
        if (tData.capacity() - tData.size() < nSize)               // �ռ䲻���Ļ���ǰ����һ�£���ֹ����̫�ർ��Ƶ�������ռ�
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

