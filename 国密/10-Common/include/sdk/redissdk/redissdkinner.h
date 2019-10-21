
/*========================================================================================
模块名    ：RedisSdk
文件名    ：redis_struct.h
相关文件  ：
实现功能  ：描述redis中数据结构
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2017 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2017/06/07         V1.0              xiongxh                                新建文件
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

    // 接口层面的虚函数定义
    virtual void FromString(const string &strString) = 0;
    virtual string ToString() const = 0;
    virtual bool IsValid() const = 0;
};

/*
 * RedisSdk的key的组成规则：type:data （详见下面的表格）
 * |---------------------|--------------------------------------------------------------------|
 * | 字段                | 说明                                                               |
 * |---------------------|--------------------------------------------------------------------|
 * | type                |描述key的类型，参考TRedisKeyType定义                                |
 * |---------------------|--------------------------------------------------------------------|
 * | data                |描述key的特征值，不同的type对应的data可以有不同的含义，             |
 * |                     |如type为agent，data可以是id                                         |
 * |---------------------|--------------------------------------------------------------------|
 *
 * 说明：
 *    1. 根据上面的规则定义的每一种key，还有一个与之对应的set类型的key，即data字段为all的key，
 *       主要是用来获取指定type的key的集合。比如：有2个vtdu，key分别为：vtdu:aaa，vtdu:bbb，则
 *       还有一个对应的key：vtdu:all，类型为set，里面的每一个元素都是一个vtdu的完整key，这样可
 *       以通过vtdu:all来获取所有vtdu的信息
 *
 * 注意：
 *     1. redis中key是一个二级结构，即type和data，中间是分隔符（一般是":")，但是实际上有些key可能
 *        中间有多个分隔符，像是有多级结构，实则不然，这是因为有些key的data部分是另一个key，以此
 *        说明两个key之间的关联关系，所以解析的时候，以第一个分隔符前面的部分作为type，后面的部分
 *        都是data
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


// 录像任务相关
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

    static const string RedisKey_Separator;                   // type、data之间的分隔符
    static const string RedisKeyData_General;                 // 泛数据（用于获取所有指定type的数据）
};

//============================================================================================

/*
 * RedisSdk的订阅通道的组成规则：type#data（详见下面的表格）
 * |---------------------|--------------------------------------------------------------------|
 * | 字段                | 说明                                                               |
 * |---------------------|--------------------------------------------------------------------|
 * | type                |描述订阅通道的类型，参考TRedisSsChannelType定义                     |
 * |---------------------|--------------------------------------------------------------------|
 * | data                |描述订阅通道的特征值，不同的type对应的data可以有不同的含义，        |
 * |                     |如type为module-type2lua，则data为TModuleType                        |
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
    TRedisSsChannelType m_tType;                                    // 订阅通道类型
    TRedisSsChannelData m_tData;                                    // 订阅通道特定数据（类型不同，数据不同）

    static const string RedisSsChannel_Separator;                   // type、data之间的分隔符
};

//============================================================================================

/*
 * redis提供了两种通知机制，基于keyspace的，还有基于keyevent的，前者关注key的变化，后者关注
 * 操作（set、del等）的变化
 *
 *     RedisSdk提供注册模块的断链检测机制，主要是利用expire，但是由于redis的expire实现机制的
 * 限制（当设置了expire的key太多的时候，expire无法做到实时，采取了访问时检测的机制，所以可能
 * 导致严重的过期延迟通知），还需要配合使用定时检测的方式
 *
 * redis的通知机制，可参考（https://redis.io/topics/notifications）
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
    u8 m_byDBNum;                           // 数据库编号
    TKeyString m_tKeyString;                // key
};

// redis支持众多的事件，这里只列出了用到的一部分
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
    u8 m_byDBNum;                           // 数据库编号
    TRedisKeyEvent m_tKeyEvent;             // event
};




//============================================================================================

/*
 * RedisSdk内部用于获取lua脚本名和sha1之间映射关系的lua脚本
 *
 * 说明：
 *     1. 在redis中，lua脚本名和其sha1编码之间的映射关系采用string类型来保存
 *        比如脚本select-vtdu.lua和sha1编码的映射，在redis中表示为：key[lua2sha:select-vtdu.lua]，value[xxxxxx]
 *     2. 最终的结果采用一维数组的形式返回到客户端，比如：{'select-vtdu.lua', 'xxxxxx', 'select-nru.lua', 'yyyyyy'}
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

