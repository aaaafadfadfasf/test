/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospinst.h
相关文件  ：osp.h
实现功能  ：扩展OSP的功能
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/18         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_INSTANCE_EXT_H
#define _OSP_INSTANCE_EXT_H

#include <vector>
using std::vector;

#include "cbb/osp/osp.h"
#include "cms/container/xmap.h"
#include "cms/ospext/osplog.h"


#ifdef _DEBUG
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

#define SELF_IID     (0xFFFFFFFF) //本INSTANCE的IID
#define SELF_NID     (0)          //本地osp结点

class CTask;

class CTaskManager;

class CTaskTimerManager;

/*
  扩展OSP的功能:
  1. 支持事务管理
  2. 支持日志输出管理
*/
class  CInstExt : public CInstance
{
public:
    friend class CTask;
public:
    CInstExt();
    virtual ~CInstExt();

private:
    CInstExt(const CInstExt&);
    void operator=(const CInstExt&);

private:
    //daemon实例入口
    virtual void DaemonInstanceEntry(CMessage *const, CApp*);

    //normal实例入口
    virtual void InstanceEntry(CMessage* const pcMsg);

    //内部消息入口
    BOOL32 OnInnerMsg(CMessage* const pcMsg);

    //用户自定义的通用内部消息处理，可覆盖此函数；一般用于Daemon和Inst无论在那种状态都需要处理的消息
    virtual BOOL32 OnUserInnerCommonMsg(CMessage* const pcMsg){return FALSE;};

    //通过private的access禁止上层直接使用以下方法, 避免造成任务管理的不可控

    //向实例中添加事务
    BOOL32 AddTask(CTask* pTask);

    //从实例中删除事务
    BOOL32 DelTask(CTask* pTask);

    //本实例中所有事务的轮询处理
    void OnTimerPollTasks();

public:
    //获取实例状态名称，需要根据子类状态实现该函数
    virtual LPCSTR GetStrState() const
    {
        return "NA";
    }

    //获取当前APP名称
    LPCSTR GetStrApp() const;

    //获取APP指针
    CApp* GetAppPt() const;

    //以下三个方法隐藏了osp基类的非const实现
    //查询当前实例状态机
    u32 CurState() const;

    //查询应用号
    u16 GetAppID() const;

    //查询实例号
    u16 GetInsID() const;

    //设置APP指针
    void SetAppPt(CApp* pApp);

    //获取APP的DATA的引用, APPTYPE必须是COspApp模板类生成的类
#define GETAPPDATA(APPTYPE) ((APPTYPE::AppDataType&)((APPTYPE*)(GetAppPt()))->GetAppData())

    //获取OSPIID
    u32 GetOspIID() const;

    //daemon实例入口
    virtual void DaemonEntry(CMessage* const pcMsg);

    //normal实例入口
    virtual void NormalEntry(CMessage* const pcMsg){};

    //消息出口, 如果 dwDstNode = SELF_IID, dwDstIId = 0, 消息发给本实例
    virtual BOOL32 PostMsg(u16 wMsgId, const void* pMsgBuf = 0, u16 wMsgLen = 0, u32 wDstIId = SELF_IID, u32 dwDstNode = SELF_NID);

    //事务管理
    //根据事件创建事务,子类根据事务的管理方式决定是否覆盖该方法
    virtual CTask* CreateTask(u16 wEvent) {return NULL;};

    //事务消息处理，统一处理事务的返回结果
    virtual void ProcTaskMsg(CTask* pTask, CMessage* const pcMsg);

    //清空实例中所有事务
    void EmptyTask();

    //打印实例中的事务信息
    void PrintTask(LPCSTR szTaskType=NULL) const;

    //打印实例中的轮询事务信息
    void PrintPollTask(LPCSTR szTaskType=NULL) const;

    //根据任务号找INST中的任务指针
    CTask* FindTask(u32 dwTaskNO) const;

    //根据任务号从本实例所属的APP中找任务指针
    CTask* FindTaskFromApp(u32 dwTaskNO) const;

    //获取本实例的事务数量
    u32 GetTaskNum() const;

    //获取本实例需要轮询的事务数量
    u32 GetPollTaskNum() const;

    //
    void AddTaskToPollMap(u32 dwTaskNO);
    void EraseTaskFromPollMap(u32 dwTaskNO);


    //获取指定类型task的指针集合的map, szTaskType为task的类名，szTaskType为空时获取所有任务指针
    CXMap<u32, CTask*> GetTasksMap(LPCSTR szTaskType = NULL);

    //获取指定类型task的指针集合的array, szTaskType为task的类名，szTaskType为空时获取所有任务指针
    vector<CTask*> GetTasksArray(LPCSTR szTaskType = NULL);

    //设置Task定时器
    bool SetTaskTimer(u32 dwTimerEvent, long dwMilliSeconds, u32 dwTaskNO);

    //删除Task定时器
    bool KillTaskTimer(u32 dwTimerEvent, u32 dwTaskNO);

    //日志输出

    //无模块控制
    void Log(u8 byLogLevel, const char* szFormat, ...) const;

    //有模块控制
    void Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const;

    typedef CXMemFunLog<CInstExt> CInstLog;

    #define INSTLOG (CInstLog(this, FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

private:

    //本instance的所有任务
    CTaskManager* m_pTaskMng;

    //task级别的定时器管理器
    CTaskTimerManager* m_pTaskTimerMng;

    //事务轮询定时器状态
    BOOL32 m_bTimerIsRunning;

    //实例的打印级别
    u8 m_byLogLev;

    //实例所属的APP的指针
    CApp* m_pApp;

    //清空实例数据
    void ClearData();
};

#endif  //#ifndef _OSP_INSTANCE_EXT_H
