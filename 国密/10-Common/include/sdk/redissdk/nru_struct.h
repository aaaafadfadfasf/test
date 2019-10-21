
#ifndef _REDISSDK_NRU_STRUCT_H
#define _REDISSDK_NRU_STRUCT_H

#include <string>
#include <set>
using std::string;
using std::set;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "sdk/redissdk/record_struct.h"
#include "sdk/redissdk/module_struct.h"

typedef string TRedisModuleNruRunMode;                              // nru������ģʽ��nruģʽ����iScsiģʽ��
#define RedisModuleNruRunMode_Nru         ("nru")
#define RedisModuleNruRunMode_iScsi       ("iScsi")

typedef TRedisModuleAddress TRedisModuleNruInTopoAddress;           // nru��������ַ��Ϣ

typedef struct TRedisModuleNruBaseInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruBaseInfo() {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strName;                                                 // ��������
    string strModel;                                                // �ͺ�
    string strManufacturer;                                         // ����
    TRedisModuleNruInTopoAddress tAddress;                          // ������ַ
}TRedisModuleNruBaseInfo;

typedef struct TRedisModuleNruLoad : public TRedisParam, public TRedisJson
{
    TRedisModuleNruLoad() : byMemUsage(0), byCpuUsage(0), qwInLost(0), qwFullLost(0) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u8  byMemUsage;                                                 // �ڴ�ʹ����
    u8  byCpuUsage;                                                 // cpuʹ����
    u64 qwInLost;                                                   // ¼��ʱ�����������cpu���ߵ��µĶ�����
    u64 qwFullLost;                                                 // ¼��ʱ¼�񻺳�������Ķ�����
}TRedisModuleNruLoad;

typedef struct TRedisModuleNruStreamLimit : public TRedisParam, public TRedisJson
{
    TRedisModuleNruStreamLimit() : qwPreRecBandWidth(0), dwRecNum(0), qwRecBandWidth(0), wPlayNum(0), wDownloadNum(0) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u64 qwPreRecBandWidth;                                          // Ԥ¼������ֵ����λ��kbps��
    u32 dwRecNum;                                                   // ¼��·����ֵ
    u64 qwRecBandWidth;                                             // ¼�������ֵ����λ��kbps��
    u16 wPlayNum;                                                   // ����·������
    u16 wDownloadNum;                                               // ����·������
}TRedisModuleNruStreamLimit;

typedef struct TRedisModuleNruStreamCur : public TRedisParam, public TRedisJson
{
    TRedisModuleNruStreamCur() : wPreRecNum(0), qwPreRecBandWidth(0), wRecNum(0), qwRecBandWidth(0), 
        wPlayNum(0), qwPlayBandWidth(0), wDownloadNum(0), qwDownloadBandWidth(0) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    u16 wPreRecNum;                                                 // Ԥ¼·��
    u64 qwPreRecBandWidth;                                          // Ԥ¼������λ��kbps��
    u16 wRecNum;                                                    // ¼��·��
    u64 qwRecBandWidth;                                             // ¼�������λ��kbps��
    u16 wPlayNum;                                                   // ����·��
    u64 qwPlayBandWidth;                                            // ���������λ��kbps��
    u16 wDownloadNum;                                               // ����·��
    u64 qwDownloadBandWidth;                                        // ���ش�����λ��kbps��
}TRedisModuleNruStreamCur;

/*
 * ��ǰ�汾��ʵ���У�nruֻ�ϱ�����Ϊ��Щǰ�˽��й�¼�񣬶����ϱ���ص�ʱ����Ϣ�������汾����Ҫ���Ż����
 */
typedef struct TRedisModuleNruRecordInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruRecordInfo() {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    set<TRedisPuRecUnit> cRecUnits;                                   // nru�������й�¼���ǰ���б�
}TRedisModuleNruRecordInfo;

typedef struct TRedisModuleNruPartitionInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruPartitionInfo() {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    set<string> cPartitionMountName;                                    // nru������Ĵ��̷����б�
}TRedisModuleNruPartitionInfo;

typedef set<TRedisModuleAddress> TRedisModuleNruAddress;                // nru������ʱ��ַ��Ϣ

typedef struct TRedisModuleNru : public TRedisParam, public TRedisJson
{
    TRedisModuleNru() : tRunMode(RedisModuleNruRunMode_Nru) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleNruRunMode tRunMode;                                // ����ģʽ����nruģʽ��iScsiģʽ��
    TRedisModuleNruAddress tAddress;                                // nru�ĵ�ַ��Ϣ
    TRedisModuleNruLoad    tLoad;                                   // nru�������
    TRedisModuleNruStreamLimit   tStreamLimit;                      // nru��Դ��ֵ
    TRedisModuleNruStreamCur     tStreamCur;                        // nru��ǰ����Դʹ��
    TRedisModuleNruRecordInfo    tRecordInfo;                       // nru�����й�¼���ǰ���б�
}TRedisModuleNru;

typedef struct TRedisModuleNruDetailInfo : public TRedisParam, public TRedisJson
{
    TRedisModuleNruDetailInfo() : bEnable(false), bOnline(false) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;                                                   // nru��id
    bool bEnable;                                                   // nru�����ñ�־
    bool bOnline;                                                   // nru������״̬
    TRedisModuleNruAddress  tAddress;                               // nru������ʱ��ַ��Ϣ
    TRedisModuleNruBaseInfo tBaseInfo;                              // nru��������Ϣ
}TRedisModuleNruDetailInfo;

typedef string TRedisModuleNruBusiness;                             // nru��ҵ������
#define RedisModuleNruBusiness_Normal       ("normal")
#define RedisModuleNruBusiness_Playback     ("playback")
#define RedisModuleNruBusiness_Download     ("download")


typedef struct TRedisModuleNruSelectCond : public TRedisParam, public TRedisJson
{
    TRedisModuleNruSelectCond() :tBusiness(RedisModuleNruBusiness_Normal){}
    TRedisModuleNruSelectCond(const string &strPartitionMountNameVal, const TRedisModuleNruBusiness &tBusinessVal)
        : strPartitionMountName(strPartitionMountNameVal),tBusiness(tBusinessVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisModuleNruBusiness tBusiness;
    string strPartitionMountName;
}TRedisModuleNruSelectCond;

typedef string TRedisTopoOperation;
#define RedisTopoOperation_Add     ("add")
#define RedisTopoOperation_Mod     ("mod")
#define RedisTopoOperation_Del     ("mod")

typedef struct TRedisNruTopoNotify : public TRedisJson
{
    TRedisNruTopoNotify() : tOperation(RedisTopoOperation_Add) {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisTopoOperation tOperation;                     // ��������
    TRedisModuleNruDetailInfo tDetailInfo;              // ��������ϸ��Ϣ
}TRedisNruTopoNotify;

typedef struct TRedisNruPlatStatus : public TRedisJson
{
    TRedisNruPlatStatus() : bInTopo(false), bEnable(false) {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    bool bInTopo;                                       // �Ƿ�����
    bool bEnable;                                       // �Ƿ�����
}TRedisNruPlatStatus;

#endif

