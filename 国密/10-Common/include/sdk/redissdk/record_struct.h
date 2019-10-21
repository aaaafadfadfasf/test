
#ifndef _REDISSDK_RECORD_STRUCT_H
#define _REDISSDK_RECORD_STRUCT_H

#include <string>
using std::string;

#include "kdvtype.h"
#include "sdk/redissdk/redissdkdata.h"
#include "cms/cms_errorcode.h"

typedef string TRedisPuRecState;
#define RedisPuRecordState_Trying            ("trying")
#define RedisPuRecordState_OK                ("ok")
#define RedisPuRecordState_Stopped           ("stopped")

typedef string TRedisPuRecMode;
#define RedisPuRecMode_All                   ("RCS_RECMODE_ALL")                // ��¼��Ƶ��¼��Ƶ
#define RedisPuRecMode_Video_Only            ("RCS_RECMODE_ONLYVIDEO")          // ֻ¼��Ƶ
#define RedisPuRecMode_Frame_Only            ("RCS_RECMODE_ONLYKEYFRAME")       // ֻ¼�ؼ�֡
#define RedisPuRecMode_Unknown               ("RCS_RECMODE_UNKNOWN")            // δ֪ģʽ

typedef struct TRedisPuRecUnit : public TRedisParam, public TRedisJson
{
    TRedisPuRecUnit() : nChannel(0) {}
    TRedisPuRecUnit(const string &strIdVal, s32 nChannelVal = 0) : strId(strIdVal), nChannel(nChannelVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // �ַ�����
    string ToString() const;
    void   FromString(const string &strString);

    bool operator<(const TRedisPuRecUnit &tRecUnit) const;
    bool operator==(const TRedisPuRecUnit &tRecUnit) const;

    string strId;                               // �豸id
    s32 nChannel;                               // ͨ��

    static const string RedisPuRecUnit_Separator;

}TRedisPuRecUnit;

//���ڲ�ѯ��������Ϣ
typedef struct TRedisPuRecUnitWithDomain : public TRedisParam, public TRedisJson
{
    TRedisPuRecUnitWithDomain(){}
    TRedisPuRecUnitWithDomain(const TRedisPuRecUnit &tRecUnitVal, const string &strDomainNameVal)
        : tRecUnit(tRecUnitVal), strDomainName(strDomainNameVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit     tRecUnit;                  // ǰ��¼��ͨ��
    string              strDomainName;             // ����
}TRedisPuRecUnitWithDomain;

typedef struct TRedisPlatRecStatus : public TRedisParam, public TRedisJson
{
    TRedisPlatRecStatus() : tRecState(RedisPuRecordState_Stopped), dwErrCode(CMS_SUCCESS) {}
    TRedisPlatRecStatus(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal) {}
    TRedisPlatRecStatus(const TRedisPuRecUnit &tRecUnitVal, const TRedisPuRecState &tStateVal, u32 dwErrCodeVal)
        : tRecUnit(tRecUnitVal), tRecState(tStateVal), dwErrCode(dwErrCodeVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    // ����С�ںţ�����������set����ʽ����TRedisPlatRecStatus
    bool operator<(const TRedisPlatRecStatus &tRecStatus) const;

    TRedisPuRecUnit  tRecUnit;                  // ǰ��¼��ͨ��
    TRedisPuRecState tRecState;                 // ¼��״̬
    u32              dwErrCode;                 // �����루ʹ��ƽ̨�����룩
}TRedisPlatRecStatus;

typedef struct TRedisDevicePlatRecStatus : public TRedisJson
{
    TRedisDevicePlatRecStatus() {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strId;                               // �豸��id
    string strDomain;                           // ����
    set<TRedisPlatRecStatus> cRecStatus;        // ¼��״̬
}TRedisDevicePlatRecStatus;

typedef struct TRedisPlatRecDuration : public TRedisParam, public TRedisJson
{
    TRedisPlatRecDuration() : wRecDuration(cwDefaultRecDuration) {}
    TRedisPlatRecDuration(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal), wRecDuration(cwDefaultRecDuration) {}
    TRedisPlatRecDuration(const TRedisPuRecUnit &tRecUnitVal, u16 wRecDurationVal)
        : tRecUnit(tRecUnitVal), wRecDuration(wRecDurationVal) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit  tRecUnit;                  // ǰ��¼��ͨ��
    u16 wRecDuration;                           // ¼��ʱ��

    static const u16 cwDefaultRecDuration = 30;
}TRedisPlatRecDuration;

typedef u8 TReidsPlatRecStreamType;
#define ReidsPlatRecStreamType_Major       (0)
#define ReidsPlatRecStreamType_Minor1      (1)
#define ReidsPlatRecStreamType_Minor2      (2)
#define ReidsPlatRecStreamType_Minor3      (3)

typedef struct TRedisPlatRecParam : public TRedisParam, public TRedisJson
{
    TRedisPlatRecParam() : tRecMode(RedisPuRecMode_All), nPreRecDuration(0), nAlarmRecDelayTime(60), 
        bEnableRecDuration(false), wRecDuration(0), tStreamType(ReidsPlatRecStreamType_Major){}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecMode tRecMode;                       // ¼��ģʽ��ֻ¼��Ƶ��ֻ¼�ؼ�֡�ȣ�
    s32 nPreRecDuration;                            // Ԥ¼ʱ������λ���룩
    s32 nAlarmRecDelayTime;                         // �澯¼��ֹͣ�ӳ�ʱ�䣨��λ���룩
    bool bEnableRecDuration;                        // �Ƿ�����¼��ʱ��
    u16 wRecDuration;                               // ¼��ʱ������λ���죩
    TReidsPlatRecStreamType tStreamType;            // ¼���������ͣ������������ȣ�
    vector<string> cPlanTimeList;                   // ��ʱ¼��ʱ��Σ�����ΪCCronTime ToString()�Ľ����
}TRedisPlatRecParam;

typedef string TRedisPlatRecType;
#define RedisPlatRecType_Manual         ("manual")
#define RedisPlatRecType_Plan           ("plan")
#define RedisPlatRecType_Alarm          ("alarm")

typedef string TRedisPlatRecOperType;
#define RedisPlatRecOperType_Start       ("start")
#define RedisPlatRecOperType_Stop        ("stop")
#define RedisPlatRecOperType_Transfer    ("transfer")
#define RedisPlatRecOperType_Clear       ("clear")

typedef struct TRedisPlatRecTask : public TRedisParam, public TRedisJson
{
    TRedisPlatRecTask() : tOperType(RedisPlatRecOperType_Start) {}
    TRedisPlatRecTask(const TRedisPuRecUnit &tRecUnitVal) : tRecUnit(tRecUnitVal), 
        tOperType(RedisPlatRecOperType_Start) {}

    // ���л��ͷ����л�����
    virtual void ToVector(vector<string> &cString) const;
    virtual void FromVector(const vector<string> &cString, vector<string>::const_iterator &it);

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    TRedisPuRecUnit tRecUnit;                       // ǰ��¼��ͨ��
    string strDomain;                               // ǰ���豸������id
    string strNruId;                                // ¼���������nru
    string strPartitionMountName;                   // ¼�����ڷ���
    set<TRedisPlatRecType> cRecTypes;               // ¼������
    TRedisPlatRecOperType tOperType;                // ��������
    TRedisPlatRecParam tRecParam;                   // ¼�����
}TRedisPlatRecTask;

typedef struct TRedisPlatRecTaskParam : public TRedisJson
{
    TRedisPlatRecTaskParam() : bOnline(false), bGbDev(false), bGbBlindFlag(false) {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    bool bOnline;                                   // ǰ���豸���߱�־
    string strDevName;                              // �豸����
    string strDevIp;                                // �豸��ip
    string strDevManufacturer;                      // �豸����
    bool   bGbDev;                                  // �����־��true�������豸��false���ǹ����豸��
    bool   bGbBlindFlag;                            // ����ä¼��־��true�������豸ä¼��false�������豸��ä¼��
    TRedisPlatRecParam tRecParam;                   // ¼�����
}TRedisPlatRecTaskParam;

typedef struct TRedisPlatRecTaskNotify : public TRedisJson
{
    TRedisPlatRecTaskNotify() : dwQueueSize(0), dwUnassignedSize(0) {}
    TRedisPlatRecTaskNotify(const string &strId) : strNruId(strId), dwQueueSize(0), dwUnassignedSize(0) {}
    TRedisPlatRecTaskNotify(const string &strId, u32 dwQueueSizeVal, u32 dwUnassignedSizeVal)
        : strNruId(strId), dwQueueSize(dwQueueSizeVal), dwUnassignedSize(dwUnassignedSizeVal) {}

    // Json���ɺͽ�������
    virtual void ParseJson(cJSON *pcRoot);
    virtual void GenerateJson(cJSON *pcRoot) const;

    string strNruId;                                // nru��id
    u32 dwQueueSize;                                // ���г���
    u32 dwUnassignedSize;                           // δ������������
}TRedisPlatRecTaskNotify;

#endif

