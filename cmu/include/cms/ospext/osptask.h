/*========================================================================================
模块名    ：ospext.lib
文件名    ：osptask.h
相关文件  ：cospobj.h
实现功能  ：基于OSP的事务处理模型
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/21         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_TASK_H_INCLUDED_
#define _OSP_TASK_H_INCLUDED_
#include "cms/ospext/osptool.h"
#include "cms/container/cospobj.h"
#include "cms/container/xmap.h"
#include "cms/ospext/osplog.h"

#define INVALID_IID     (0)          //无效IID
#define INVALID_TASKNO  (0)          //无效事务号
#define INVALID_SEQ_NO  (0)          //无效序列号

//定时轮询返回结果, 0表示本次轮询被忽略, 1表示完成轮询, 2表示事务在轮询后被删除

//#define TIMERPOLL_SKIP 0    //删除TIMERPOLL_SKIP，TIMERPOLL_SKIP语意和TIMERPOLL_DONE一致
#define TIMERPOLL_DONE 1
#define TIMERPOLL_DEL  2

//事务或状态消息处理返回结果, 0表示消息处理成功, 1表示消息处理失败, 2表示事务在消息处理后被删除
typedef u32 PROCMSG_RESULT;
#define PROCMSG_OK   0
#define PROCMSG_FAIL 1
#define PROCMSG_DEL  2

class CInstExt;

struct TOspTransaction
{
	u32 dwIID;    //appid,instid
	u32 dwTaskID;  //task id
	u32 dwSeqNO;  //seq no

	TOspTransaction()
	{
		dwIID = INVALID_IID;
		dwTaskID = INVALID_TASKNO;
		dwSeqNO = INVALID_SEQ_NO;
	}

	bool operator==(const TOspTransaction& tOther) const
	{
		return (tOther.dwIID == dwIID && tOther.dwTaskID == dwTaskID);  //注意，这里不比较SeqNO
	}
};

typedef TOspTransaction TOspTransation;

template <> struct CHashFunctor<TOspTransation>
{
	size_t operator()(const TOspTransation& key) const
	{
		unsigned char abyKey[sizeof(key.dwIID) + sizeof(key.dwTaskID)] = {0};
		memcpy(abyKey, &key.dwIID, sizeof(key.dwIID));
		memcpy(abyKey+sizeof(key.dwIID), &key.dwTaskID, sizeof(key.dwTaskID));

		return hash_mysql((LPCSTR)abyKey, sizeof(key.dwIID) + sizeof(key.dwTaskID));
	}
};


/*
 * CTask是所有事务的基类, 这里定义了一个事务的基本属性
 */
class CTask : public COspObj
{
public:
    enum //必须在子类中自定义的状态枚举
    {
        UNKNOWN_STATE = 0,
    };
    typedef u32 (CTask::*PFProcMsg)(CMessage *const pcMsg);
    typedef u32 (CTask::*PFTimerPoll)(void);
    class CStateProc
    {
    public:
        CStateProc()
        {
            ProcMsg = NULL;
            TimerPoll = NULL;
            ProcErrMsg = NULL;
        }
        PFProcMsg ProcMsg;
        PFTimerPoll TimerPoll;
        PFProcMsg ProcErrMsg;
    };
    typedef CXMap<u32, CStateProc> CTaskRuleMap;
    

    CTask(CInstExt *pcInst);
    virtual ~CTask();

private:
    CTask(const CTask& cTask);
    CTask& operator=(const CTask& cTask);

public:
    virtual u32 ProcMsg(CMessage *const pcMsg)
    {
        CStateProc cProc;
        if ( !GetStateProc(GetState(), cProc) )
        {
            CMS_ASSERT( 0 && "[CTask::ProcMsg]获取CStateProc失败，是否忘记覆盖InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.ProcMsg && "[CTask::ProcMsg]ProcMsg函数指针为空，是否忘记覆盖InitStateMachine" );

        PFProcMsg pfProcMsg = (PFProcMsg)cProc.ProcMsg;
        return (this->*pfProcMsg)(pcMsg);
    }

    /*====================================================================
    函数名      : TimerPoll
    功能        : 每个事务都有一个定时轮询事件, 该事件由事务管理类触发
                  每个事务都可以在此函数内处理事务超时或者事务维护消息
    算法实现    : 当事务在本函数中做了实际上的处理, 例如发送过网络消息, 则应该返回TRUE
                  当事务没有做任何处理, 或者尝试处理但是没有成功, 则应该返回FALSE

                  为了防止消息突发, 事务管理类会在定时器到来时处理部分事务,
                  所以会根据事务的TimerPoll判断是否继续处理下一个事务
    引用全局变量:
    输入参数说明:
    返回值说明  : 没有做动作返回0, 如果有动作返回1, 如果处理的事务消亡返回2
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2010/19/7               liangli       create
    ====================================================================*/
    virtual u32 TimerPoll()
    {
        CStateProc cProc;
        if ( !GetStateProc(GetState(), cProc) )
        {
            CMS_ASSERT( 0 && "[CTask::TimerPoll]获取CStateProc失败，是否忘记覆盖InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.TimerPoll && "[CTask::TimerPoll]TimerPoll函数指针为空，是否忘记覆盖InitStateMachine" );

        PFTimerPoll pfTimerPoll = (PFTimerPoll)cProc.TimerPoll;
        return (this->*pfTimerPoll)();
    }
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const = 0; //必须覆盖的方法
    virtual const char* GetStateName(u32 dwState) const = 0; //必须覆盖的方法 
    virtual u32 TimerPollSkip();

    //上层定义持久事务时必须实现该方法，该方法指明哪些状态下事务是不需要轮询的。
    virtual bool IsNotNeedPollState(){return false;}

    /*====================================================================
    函数名      : InitStateMachine
    功能        : 用状态枚举+函数取代CState，整个CTask采用表驱动方式
                  此方法初始化状态表
    算法实现    : 在CTask构造函数中调用此方法
    引用全局变量:
    输入参数说明:
    返回值说明  : 没有做动作返回0, 如果有动作返回1, 如果处理的事务消亡返回2
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2010/19/7               liangli       create
    ====================================================================*/
    virtual void InitStateMachine() = 0; //必须覆盖的方法
public:
    //日志输出

    //无模块控制
    void Log(u8 byLogLevel, const char* szFormat, ...) const;

    //无模块控制
    void Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const;

    typedef CXMemFunLog<CTask> CTaskLog;

    #define TASKLOG (CTaskLog(this, FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

    //
    virtual void PrintData() const;

public:

    CTask *GetThisTask(){return this;}  //便于在迭代STL容器时无须类型转换

    //
    u32 GetState() const;

    //
    u32 GetCurStateHoldTime() const;

    bool GetStateProc(const u32 dwState, CStateProc &cProc);

    //
    u32 GetTaskNO() const;

    //
    void SetTaskNO(const u32 dwTaskNO);

    //
    bool IsTask(const char* szTaskName) const;

    //
    CInstExt* GetInstance() const;

    //
    u16 GetAppID() const;

    //
    u16 GetInsID() const;

	//
	TOspTransaction GetOspTrans() const;

    //
    u32 GetCheckRef() const;

    //
    void SetCheckRef(const u32 dwCheckRef);

    //
    void ClearPollCounter();

    //
    s32 GetPollCounter() const;

    //
    void AddPollCounter();

    //
    time_t GetCreateTime() const;

    //
    void SetCreateTime(time_t tCreateTime);

public:
    /*
      事务定时器。
      注意和轮询定时器区别：
      轮询定时器属于instance，它驱动实例下的所有事务定时执行某种操作，
      所有事务共享，一直存在。并且轮询定时器的时间间隔基本保持在一秒。

      事务定时器是每个事务独有，并且是一次性的。它可以任意指定定时器
      时间间隔以及定时器事件。它存在的目的是可以让事务可以精确的执行
      定时操作，比如摄像头控制事务的步进式控制，用轮询定时器就无法完成。
    */

    /*====================================================================
    函数名      : SetTimer
    功能        : 设置定时器
    算法实现    : 借用Osp的定时器实现
    引用全局变量:
                  dwTimerEvent：  定时器事件
                  dwMilliSeconds：定时器时间间隔，单位是：ms
    输入参数说明:
    返回值说明  : 成功为true，失败为fasle
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/02/23              fanxg          create
    ====================================================================*/
    bool SetTimer(u32 dwTimerEvent, long dwMilliSeconds);
    
    /*====================================================================
    函数名      : KillTimer
    功能        : 删除定时器
    算法实现    : 借用Osp的定时器实现
    引用全局变量:
                  dwTimerEvent：  定时器事件
    输入参数说明:
    返回值说明  : 成功为true，失败为fasle
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2012/02/23              fanxg          create
    ====================================================================*/
    bool KillTimer(u32 dwTimerEvent);
    


protected:
    /*  
      事务的状态切换：
      1.更新状态创建时间;
      2.清空轮询计数
      3.允许重复切换到当前状态，便于对同一消息(例如NtfReq)的多次超时检测
    */
    void NextState(const u32 dwState);

    //
    void InitCreateTime();

    //
    void AddRuleProc(u32 dwState, const CStateProc &cProc);

private:
    u32       m_dwTaskNO;          //事务流水号, 有效ID从 1 到 0xFFFF 无符号
    u32       m_dwCurState;       //当前状态
    u32       m_dwCheckRef;       //轮询处理引用计数
    time_t    m_tCreateTime;      //事务的创建时间
    s32       m_nPollCounter;     //轮询计数, 用于判断事务超时
    u64    m_tCurStateCreateTime;  //当前状态创建时间，用来计算状态的持续时间进行超时判断
    CInstExt* m_pcInstance;       //用于访问App实例接口
    CTaskRuleMap m_mapRule;       //记录所有状态枚举以及处理函数入口
private:
};

#endif //#ifndef _OSP_TASK_H_INCLUDED_

