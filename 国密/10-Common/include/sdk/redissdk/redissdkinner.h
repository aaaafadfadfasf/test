
/*========================================================================================
ģ����    ��RedisSdk
�ļ���    ��redis_struct.h
����ļ�  ��
ʵ�ֹ���  ������redis�����ݽṹ
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2017 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2017/06/07         V1.0              xiongxh                                �½��ļ�
=========================================================================================*/

#ifndef _REDISSDK_INNER_H
#define _REDISSDK_INNER_H

#include <string>
using std::string;

#include "kdvtype.h"

typedef string TKeyString;

class CRedisObj
{
public:
    CRedisObj()  {}
    ~CRedisObj() {}

    // �ӿڲ�����麯������
    virtual void FromString(const string &strString) = 0;
    virtual string ToString() const = 0;
    virtual bool IsValid() const = 0;
};

/*
 * RedisSdk��key����ɹ���type:data ���������ı��
 * |---------------------|--------------------------------------------------------------------|
 * | �ֶ�                | ˵��                                                               |
 * |---------------------|--------------------------------------------------------------------|
 * | type                |����key�����ͣ��ο�TRedisKeyType����                                |
 * |---------------------|--------------------------------------------------------------------|
 * | data                |����key������ֵ����ͬ��type��Ӧ��data�����в�ͬ�ĺ��壬             |
 * |                     |��typeΪagent��data������id                                         |
 * |---------------------|--------------------------------------------------------------------|
 *
 * ˵����
 *    1. ��������Ĺ������ÿһ��key������һ����֮��Ӧ��set���͵�key����data�ֶ�Ϊall��key��
 *       ��Ҫ��������ȡָ��type��key�ļ��ϡ����磺��2��vtdu��key�ֱ�Ϊ��vtdu:aaa��vtdu:bbb����
 *       ����һ����Ӧ��key��vtdu:all������Ϊset�������ÿһ��Ԫ�ض���һ��vtdu������key��������
 *       ��ͨ��vtdu:all����ȡ����vtdu����Ϣ
 *
 * ע�⣺
 *     1. redis��key��һ�������ṹ����type��data���м��Ƿָ�����һ����":")������ʵ������Щkey����
 *        �м��ж���ָ����������ж༶�ṹ��ʵ��Ȼ��������Ϊ��Щkey��data��������һ��key���Դ�
 *        ˵������key֮��Ĺ�����ϵ�����Խ�����ʱ���Ե�һ���ָ���ǰ��Ĳ�����Ϊtype������Ĳ���
 *        ����data
 */
typedef string TRedisKeyType;
#define RedisKeyType_Lua_Sha                         ("lua-sha")
#define RedisKeyType_Device                          ("dev")
#define RedisKeyType_Cmu                             ("cmu")
#define RedisKeyType_Cui                             ("cui")
#define RedisKeyType_Pui                             ("pui")
#define RedisKeyType_Rcs                             ("rcs")
#define RedisKeyType_Vtdu                            ("vtdu")
#define RedisKeyType_Vtdu_Resource                   ("vtdu-resource")
#define RedisKeyType_Resource_Item                   ("resource-item")
#define RedisKeyType_Resource_Id                     ("resource-id")
#define RedisKeyType_Cu_Agent                        ("cu")
#define RedisKeyType_Nru                             ("nru")
#define RedisKeyType_Module_Address                  ("module-address")
#define RedisKeyType_Vtdu_Resource                   ("vtdu-resource")
#define RedisKeyType_Partition                       ("partition")
#define RedisKeyType_Vtdu_Src_Switch_Agent           ("vtdu-src-switch-agent")
#define RedisKeyType_Pu_To_Vtdu                      ("pu-to-vtdu")
#define RedisKeyType_Switch_Agent_Status             ("switch-agent-status")
#define RedisKeyType_Partition                       ("partition")
#define RedisKeyType_Partition_Nru                   ("partition-nru")
#define RedisKeyType_Nru_Load                        ("nru-load")
#define RedisKeyType_Nru_Stream_Limit                ("nru-stream-limit")
#define RedisKeyType_Nru_Stream_Cur                  ("nru-stream-cur")
#define RedisKeyType_Nru_Record_Info                 ("nru-record-info")
#define RedisKeyType_Plat_Config                     ("plat-config")
#define RedisKeyType_Plat_Rec_Param                  ("plat-rec-param")
#define RedisKeyType_Plat_Rec_Param_Plan_Time        ("plat-rec-param-plan-time")

#define RedisKeyType_Plat_Rec_Task_Time_List         ("plat-rec-task-time-list")
#define RedisKeyType_Switch                          ("switch")
#define RedisKeyType_Switch_Unit                     ("switch-unit")
#define RedisKeyType_Switch_Agent                    ("switch-agent")


// ¼���������
#define RedisKeyType_Plat_Rec_Task                   ("plat-rec-task")
#define RedisKeyType_Plat_Rec_Task_Types             ("plat-rec-task_types")
#define RedisKeyType_Plat_Rec_Task_Queue             ("plat-rec-task-queue")

typedef string TRedisKeyData;


class CRedisKey : public CRedisObj
{
public:
    CRedisKey() {}
    CRedisKey(const TRedisKeyType &tType) : m_tType(tType) {}
    CRedisKey(const TRedisKeyType &tType, const TRedisKeyData &tData)
        : m_tType(tType), m_tData(tData) {}

    virtual void FromString(const string &strString);
    virtual string ToString() const { return m_tType + RedisKey_Separator + m_tData; }
    virtual bool IsValid() const;

    bool operator<(const CRedisKey &cKey) const;

public:
    const TRedisKeyType &GetType() const { return m_tType; }
    TRedisKeyType &GetType() { return m_tType; }
    void SetType(const TRedisKeyType &tType) { m_tType = tType; }

    const TRedisKeyData &GetData() const { return m_tData; }
    TRedisKeyData &GetData() { return m_tData; }
    void SetData(const TRedisKeyData &tData) { m_tData = tData; }
private:
    TRedisKeyType m_tType;
    TRedisKeyData m_tData;

    static const string RedisKey_Separator;                   // type��data֮��ķָ���
    static const string RedisKeyData_General;                 // �����ݣ����ڻ�ȡ����ָ��type�����ݣ�
};

//============================================================================================

/*
 * RedisSdk�Ķ���ͨ������ɹ���type#data���������ı��
 * |---------------------|--------------------------------------------------------------------|
 * | �ֶ�                | ˵��                                                               |
 * |---------------------|--------------------------------------------------------------------|
 * | type                |��������ͨ�������ͣ��ο�TRedisSsChannelType����                     |
 * |---------------------|--------------------------------------------------------------------|
 * | data                |��������ͨ��������ֵ����ͬ��type��Ӧ��data�����в�ͬ�ĺ��壬        |
 * |                     |��typeΪmodule-type2lua����dataΪTModuleType                        |
 * |---------------------|--------------------------------------------------------------------|
 *
 */
typedef string TRedisSsChannelType;
#define RedisSsChannelType_RedisSdk                          ("redissdk")
#define RedisSsChannelType_Plat_Config                       ("plat-config")
#define RedisSsChannelType_Module_Status                     ("module-status")
#define RedisSsChannelType_Module_Type_Status                ("module-type-status")
#define RedisSsChannelType_Plat_Rec_Status                   ("plat-rec-status")
#define RedisSsChannelType_Plat_Rec_Duration                 ("plat-rec-param-duration")
#define RedisSsChannelType_Plat_Rec_Task_Queue               ("plat-rec-task-queue")
#define RedisSsChannelType_Partition                         ("partition")
#define RedisSsChannelType_Nru_Plat_Status                   ("nru-plat-status")

typedef string TRedisSsChannelData;
#define RedisSsChannelData_Lua_Sha                           ("lua-sha")
#define RedisSsChannelData_Plat_Rec_Cover_Policy             ("plat-rec-cover-policy")
#define RedisSsChannelData_Plat_Rec_Reserve_Days             ("plat-rec-reserve-days")
#define RedisSsChannelData_Plat_Rec_No_Stream_Check_Param    ("plat-rec-no-stream-check-param")
#define RedisSsChannelData_Plat_Rec_File_Cut_Policy          ("plat-rec-file-cut-policy")
#define RedisSsChannelData_Nru_Topo                          ("nru-topo")


class CRedisSsChannel : public CRedisObj
{
public:
    CRedisSsChannel() {}
    CRedisSsChannel(const TRedisSsChannelType &tType) : m_tType(tType) {}
    CRedisSsChannel(const TRedisSsChannelType &tType, const TRedisSsChannelData &tData)
        : m_tType(tType), m_tData(tData) {}

    virtual void FromString(const string &strString);
    virtual string ToString() const { return m_tType + RedisSsChannel_Separator + m_tData; }
    virtual bool IsValid() const;

    bool operator<(const CRedisSsChannel &cChn) const;

public:
    const TRedisSsChannelType &GetType() const { return m_tType; }
    TRedisSsChannelType &GetType() { return m_tType; }
    void SetType(const TRedisSsChannelType &tType) { m_tType = tType; }

    const TRedisSsChannelData &GetData() const { return m_tData; }
    TRedisSsChannelData &GetData() { return m_tData; }
    void SetData(const TRedisSsChannelData &tData) { m_tData = tData; }

private:
    TRedisSsChannelType m_tType;                                    // ����ͨ������
    TRedisSsChannelData m_tData;                                    // ����ͨ���ض����ݣ����Ͳ�ͬ�����ݲ�ͬ��

    static const string RedisSsChannel_Separator;                   // type��data֮��ķָ���
};

//============================================================================================

/*
 * redis�ṩ������֪ͨ���ƣ�����keyspace�ģ����л���keyevent�ģ�ǰ�߹�עkey�ı仯�����߹�ע
 * ������set��del�ȣ��ı仯
 *
 *     RedisSdk�ṩע��ģ��Ķ��������ƣ���Ҫ������expire����������redis��expireʵ�ֻ��Ƶ�
 * ���ƣ���������expire��key̫���ʱ��expire�޷�����ʵʱ����ȡ�˷���ʱ���Ļ��ƣ����Կ���
 * �������صĹ����ӳ�֪ͨ��������Ҫ���ʹ�ö�ʱ���ķ�ʽ
 *
 * redis��֪ͨ���ƣ��ɲο���https://redis.io/topics/notifications��
 */
typedef string TKeySpaceChnString;

class CRedisKeySpaceChannel
{
public:
    CRedisKeySpaceChannel() : m_byDBNum(0) {}
    CRedisKeySpaceChannel(const TKeyString &tString) : m_byDBNum(0), m_tKeyString(tString) {}
    CRedisKeySpaceChannel(u8 byNum, const TKeyString &tString)
        : m_byDBNum(byNum), m_tKeyString(tString) {}

    void FromString(const TKeySpaceChnString &tString);
    TKeySpaceChnString ToString() const;
    bool IsValid() const;
    bool operator<(const CRedisKey &cKey) const;

public:
    u8 GetDBNum() const { return m_byDBNum; }
    void SetDBNum(u8 byNum) { m_byDBNum = byNum; }

    const TKeyString &GetKeyString() const { return m_tKeyString; }
    TKeyString &GetKeyString() { return m_tKeyString; }
    void SetKeyString(const TKeyString &tString) { m_tKeyString = tString; }

private:
    u8 m_byDBNum;                           // ���ݿ���
    TKeyString m_tKeyString;                // key
};

// redis֧���ڶ���¼�������ֻ�г����õ���һ����
typedef string TRedisKeyEvent;
#define RedisKeyEvent_Set                   ("set")
#define RedisKeyEvent_Expired               ("expired")

typedef string TKeyEventChnString;

class CRedisKeyEventChannel
{
public:
public:
    CRedisKeyEventChannel() : m_byDBNum(0) {}
    CRedisKeyEventChannel(const TRedisKeyEvent &tEvent) : m_byDBNum(0), m_tKeyEvent(tEvent) {}
    CRedisKeyEventChannel(u8 byNum, const TRedisKeyEvent &tEvent)
        : m_byDBNum(byNum), m_tKeyEvent(tEvent) {}

    void FromString(const TKeyEventChnString &tString);
    TKeyEventChnString ToString() const;
    bool IsValid() const;
    bool operator<(const CRedisKey &cKey) const;

public:
    u8 GetDBNum() const { return m_byDBNum; }
    void SetDBNum(u8 byNum) { m_byDBNum = byNum; }

    const TRedisKeyEvent &GetKeyEvent() const { return m_tKeyEvent; }
    TRedisKeyEvent &GetKeyEvent() { return m_tKeyEvent; }
    void SetKeyEvent(const TRedisKeyEvent &tEvent) { m_tKeyEvent = tEvent; }

private:
    u8 m_byDBNum;                           // ���ݿ���
    TRedisKeyEvent m_tKeyEvent;             // event
};




//============================================================================================

/*
 * RedisSdk�ڲ����ڻ�ȡlua�ű�����sha1֮��ӳ���ϵ��lua�ű�
 *
 * ˵����
 *     1. ��redis�У�lua�ű�������sha1����֮���ӳ���ϵ����string����������
 *        ����ű�select-vtdu.lua��sha1�����ӳ�䣬��redis�б�ʾΪ��key[lua2sha:select-vtdu.lua]��value[xxxxxx]
 *     2. ���յĽ������һά�������ʽ���ص��ͻ��ˣ����磺{'select-vtdu.lua', 'xxxxxx', 'select-nru.lua', 'yyyyyy'}
 */
const string RedisLuaScriptLua2Sha(
    "local lock_flag = redis.call('exists', 'redissdk:lock') ~= 0; \n"
    "if lock_flag then \n"
    "  return 28004; \n"
    "end; \n"
    "local list=redis.call('smembers', 'lua-sha:all'); \n"
    "local result={}; \n"
    "local cnt=0; \n"
    "for i=1,#list do \n"
    "  if #list[i] ~= 0 then \n"
    "    cnt=cnt+1;\n"
    "    result[2*cnt-1]=list[i]; \n"
    "    result[2*cnt]=redis.call('get', 'lua-sha:'..list[i]) or ''; \n"
    "  end; \n"
    "end; \n"
    "return result;"
    );

//============================================================================================



#endif

