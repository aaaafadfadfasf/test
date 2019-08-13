/*========================================================================================
模块名    ：cmu
文件名    ：slidingwindowtask.h
相关文件  ：
实现功能  ：目前设备状态在redis中只有一个字段来表示，这样的话，当下级域下线，pui下线时，所有相关的
            设备状态都要逐一的修改，并发布出来，该task就是为了实现这个功能
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2017/01/19         V1.0              xiongxh                                新建文件
=========================================================================================*/

/*
 * 设计思路：
 *   1、该task应该可以有多个实例，且互不影响。比如针对下级域产生一个实例，针对pui（gbu）产生一个，且两个实例间不应该有耦合
 *   2、该task以element为单位来处理数据，每次可以处理一定的数量，处理结束后投递出下一次处理的消息，这样主线程可以在连续处理
 *      之间有一定的空隙去处理其他的事务
 *   3、支持快速的添加、删除element
 *   4、支持同步、异步两种工作模式（异步模式下由使用者来删除element）
 *   5、之前实现了一个类似的机制，即CFlowCtrlMngTask
 *
 *
 */


#ifndef _SLIDING_WINDOW_TASK_H
#define _SLIDING_WINDOW_TASK_H

#include <iostream>
#include <algorithm>
using namespace std;

#include "cms/ospext/osptask.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"

template <typename T>
class CElementMngBase
{
public:
    CElementMngBase()  {}
    ~CElementMngBase() {}

public:
    virtual void AddElement(const T &tElementId) = 0;
    virtual void AddElement(const vector<T> &cElementIds) = 0;
    virtual void AddElement(const set<T> &cElementIdSet) = 0;
    virtual void DelElement(const T &tElementId) = 0;
    virtual void DelElement(const vector<T> &cElementIds) = 0;
    virtual void DelElement(const set<T> &cElementIdSet) = 0;
    virtual const T *GetElements() = 0;
    virtual void     GetElements(u32 dwSize, vector<T> &cElementIds) = 0;
    virtual void     GetElements(u32 dwSize, set<T> &cElementIdSet) = 0;
    virtual bool IsEmpty() const = 0;
    virtual void Reset() = 0;

    virtual u32  GetTotalElement() const = 0;
    virtual u32  GetTopElement()   const = 0;
    virtual u32  GetRestElement()  const = 0;

    virtual void PrintData() const
    {
        OspPrintf(TRUE, FALSE, "\tRest[%u], Processed[%u], Total[%u], Top[%u]\n", 
            GetRestElement(), GetTotalElement() - GetRestElement(), GetTotalElement(), GetTopElement());
    }
};


/* element管理类
 * 特性：todo by xiongxh （和现有设计不符合，待更新）
 *  1、支持排队机制（使用list来保存队列数据，便于快速添加、删除，但是基于list的特性，禁止调用size()方法）
 *  2、禁止重复排队（使用set来保证）
 *  3、支持批量排队（允许一次性操作多个element）
 *  4、支持随机取消排队（即可以从队列中间出队）
 *  5、支持快速的添加、删除element（使用set来进行快速的添加、删除、查找）
 *  6、支持统计，便于后续排查问题和调整参数
 *
 * 实现：
 *      一般情况下，要实现队列机制，可以使用queue，list等，相比较而言，list在添加、删除元素上更快。但是队列一般是不能遍历，也无法从中间
 *  删除元素的，CElementMng必须要支持和排队和取消排队的功能，为了在队列的基础上也能快速的取消排队（即随机删除元素），可以同时使用list和
 *  set，以空间换时间，大体的思路是：list和set中都保存排队数据，当需要取消排队的时候，只从set中删除元素，这样就会出现某些元素存在list中
 *  但是不存在set中的情况，这种情况即是已经取消的排队元素。要注意的是，使用这个机制的话，可能导致重复排队的问题，比如某个元素在取消后再
 *  次排队，这个时候list中将有多个该元素，而set中只有一个，而CElementMng要保证取出来的数据不能重复，这样的话在取数据的时候就需要做些处理
 */
template <typename T>
class COrderedElementMng : public CElementMngBase<T>
{
public:
    COrderedElementMng() { Reset(); }
    virtual ~COrderedElementMng() {}

public:
    virtual void AddElement(const T &tElementId);
    virtual void AddElement(const vector<T> &cElementIds);
    virtual void AddElement(const set<T> &cElementIdSet);
    virtual void DelElement(const T &tElementId);
    virtual void DelElement(const vector<T> &cElementIds);
    virtual void DelElement(const set<T> &cElementIdSet);
    virtual const T *GetElements();
    virtual void     GetElements(u32 dwSize, vector<T> &cElementIds);
    virtual void     GetElements(u32 dwSize, set<T> &cElementIdSet);
    virtual void Reset();
    virtual bool IsEmpty() const { return m_cElementSet.empty(); }

    virtual u32  GetTotalElement() const { return m_dwTotal;             }
    virtual u32  GetTopElement()   const { return m_dwTop;               }
    virtual u32  GetRestElement()  const { return m_cElementSet.size();  }

private:
    list<T>             m_cElementQueue;            // 队列，实现排队机制
    set<T>              m_cElementSet;              // 集合，便于快速查找、添加、删除（空间换时间）

    u32                 m_dwTotal;                  // element总计数
    u32                 m_dwTop;                    // element峰值
};

template <typename T>
void COrderedElementMng<T>::AddElement(const T &tElementId)
{
    if (m_cElementSet.end() == m_cElementSet.find(tElementId))          // 避免重复排队
    {
        m_cElementSet.insert(tElementId);
        m_cElementQueue.push_back(tElementId);

        ++m_dwTotal;
        m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
    }
}

template <typename T>
void COrderedElementMng<T>::AddElement(const vector<T> &cElementIds)
{
    for (typename vector<T>::const_iterator it = cElementIds.begin(); cElementIds.end() != it; ++it)
    {
        typename set<T>::const_iterator it4Id = m_cElementSet.find(*it);
        if (m_cElementSet.end() == it4Id)
        {
            m_cElementSet.insert(*it);
            m_cElementQueue.push_back(*it);

            ++m_dwTotal;
        }
    }

    m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
}

template <typename T>
void COrderedElementMng<T>::AddElement(const set<T> &cElementIdSet)
{
    for (typename set<T>::const_iterator it = cElementIdSet.begin(); cElementIdSet.end() != it; ++it)
    {
        typename set<T>::const_iterator it4Id = m_cElementSet.find(*it);
        if (m_cElementSet.end() == it4Id)
        {
            m_cElementSet.insert(*it);
            m_cElementQueue.push_back(*it);

            ++m_dwTotal;
        }
    }

    m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
}

template <typename T>
void COrderedElementMng<T>::DelElement(const T &tElementId)
{
    m_cElementSet.erase(tElementId);
}

template <typename T>
void COrderedElementMng<T>::DelElement(const vector<T> &cElementIds)
{
    for (typename vector<T>::const_iterator it = cElementIds.begin(); cElementIds.end() != it; ++it)
    {
        m_cElementSet.erase(*it);
    }
}

template <typename T>
void COrderedElementMng<T>::DelElement(const set<T> &cElementIdSet)
{
    for (typename set<T>::const_iterator it = cElementIdSet.begin(); cElementIdSet.end() != it; ++it)
    {
        m_cElementSet.erase(*it);
    }
}

template <typename T>
const T *COrderedElementMng<T>::GetElements()
{
    for (typename list<T>::iterator it = m_cElementQueue.begin(); m_cElementQueue.end() != it; )
    {
        if (m_cElementSet.end() == m_cElementSet.find(*it))
        {
            m_cElementQueue.erase(it++);
        }
        else
        {
            return (&(*it));
        }
    }

    return NULL;
}

template <typename T>
void COrderedElementMng<T>::GetElements(u32 dwSize, vector<T> &cElementIds)
{
    cElementIds.reserve(cElementIds.size() + dwSize);

    u32 i = 0;
    for (typename list<T>::iterator it = m_cElementQueue.begin(); i < dwSize && m_cElementQueue.end() != it; )
    {
        if (m_cElementSet.end() != m_cElementSet.find(*it))
        {
            cElementIds.push_back(*it);
            ++i;
            ++it;
        }
        else
        {
            m_cElementQueue.erase(it++);
        }
    }
}

template <typename T>
void COrderedElementMng<T>::GetElements(u32 dwSize, set<T> &cElementIdSet)
{
    u32 i = 0;
    for (typename list<T>::iterator it = m_cElementQueue.begin(); i < dwSize && m_cElementQueue.end() != it; )
    {
        if (m_cElementSet.end() != m_cElementSet.find(*it))
        {
            cElementIdSet.insert(*it);
            ++i;
            ++it;
        }
        else
        {
            m_cElementQueue.erase(it++);
        }
    }
}

template <typename T>
void COrderedElementMng<T>::Reset()
{
    m_cElementQueue.clear();
    m_cElementSet.clear();

    m_dwTotal    = 0;
    m_dwTop      = 0;
}

//===================================================================================================

template <typename T>
class CUnorderedElementMng : public CElementMngBase<T>
{
public:
    CUnorderedElementMng() { Reset(); }
    virtual ~CUnorderedElementMng() {}

public:
    virtual void AddElement(const T &tElementId);
    virtual void AddElement(const vector<T> &cElementIds);
    virtual void AddElement(const set<T> &cElementIdSet);
    virtual void DelElement(const T &tElementId);
    virtual void DelElement(const vector<T> &cElementIds);
    virtual void DelElement(const set<T> &cElementIdSet);
    virtual const T *GetElements();
    virtual void     GetElements(u32 dwSize, vector<T> &cElementIds);
    virtual void     GetElements(u32 dwSize, set<T> &cElementIdSet);
    virtual void Reset();
    virtual bool IsEmpty() const { return m_cElementSet.empty(); }

    virtual u32  GetTotalElement() const { return m_dwTotal;             }
    virtual u32  GetTopElement()   const { return m_dwTop;               }
    virtual u32  GetRestElement()  const { return m_cElementSet.size();  }

private:
    set<T>              m_cElementSet;              // 保存待处理元素

    u32                 m_dwTotal;                  // element总计数
    u32                 m_dwTop;                    // element峰值
};

template <typename T>
void CUnorderedElementMng<T>::AddElement(const T &tElementId)
{
    if (m_cElementSet.end() == m_cElementSet.find(tElementId))          // 避免重复排队
    {
        m_cElementSet.insert(tElementId);

        ++m_dwTotal;
        m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
    }
}

template <typename T>
void CUnorderedElementMng<T>::AddElement(const vector<T> &cElementIds)
{
    for (typename vector<T>::const_iterator it = cElementIds.begin(); cElementIds.end() != it; ++it)
    {
        typename set<T>::const_iterator it4Id = m_cElementSet.find(*it);
        if (m_cElementSet.end() == it4Id)
        {
            m_cElementSet.insert(*it);

            ++m_dwTotal;
        }
    }

    m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
}

template <typename T>
void CUnorderedElementMng<T>::AddElement(const set<T> &cElementIdSet)
{
    for (typename set<T>::const_iterator it = cElementIdSet.begin(); cElementIdSet.end() != it; ++it)
    {
        typename set<T>::const_iterator it4Id = m_cElementSet.find(*it);
        if (m_cElementSet.end() == it4Id)
        {
            m_cElementSet.insert(*it);

            ++m_dwTotal;
        }
    }

    m_dwTop = m_cElementSet.size() > m_dwTop ? m_cElementSet.size() : m_dwTop;
}

template <typename T>
void CUnorderedElementMng<T>::DelElement(const T &tElementId)
{
    m_cElementSet.erase(tElementId);
}

template <typename T>
void CUnorderedElementMng<T>::DelElement(const vector<T> &cElementIds)
{
    for (typename vector<T>::const_iterator it = cElementIds.begin(); cElementIds.end() != it; ++it)
    {
        m_cElementSet.erase(*it);
    }
}

template <typename T>
void CUnorderedElementMng<T>::DelElement(const set<T> &cElementIdSet)
{
    for (typename set<T>::const_iterator it = cElementIdSet.begin(); cElementIdSet.end() != it; ++it)
    {
        m_cElementSet.erase(*it);
    }
}

template <typename T>
const T *CUnorderedElementMng<T>::GetElements()
{
    if (m_cElementSet.empty())
    {
        return (&(*m_cElementSet.begin()));
    }

    return NULL;
}

template <typename T>
void CUnorderedElementMng<T>::GetElements(u32 dwSize, vector<T> &cElementIds)
{
    cElementIds.reserve(cElementIds.size() + dwSize);

    u32 i = 0;
    for (typename set<T>::iterator it = m_cElementSet.begin(); i < dwSize && m_cElementSet.end() != it; ++it)
    {
        cElementIds.push_back(*it);
        ++i;
    }
}

template <typename T>
void CUnorderedElementMng<T>::GetElements(u32 dwSize, set<T> &cElementIdSet)
{
    u32 i = 0;
    for (typename set<T>::iterator it = m_cElementSet.begin(); i < dwSize && m_cElementSet.end() != it; ++it)
    {
        cElementIdSet.insert(*it);
        ++i;
    }
}

template <typename T>
void CUnorderedElementMng<T>::Reset()
{
    m_cElementSet.clear();

    m_dwTotal    = 0;
    m_dwTop      = 0;
}

//===================================================================================================

/* 滑动窗口类
 * 特性：
 *  1、支持暂停、恢复（外部遇到某些事件，可能需要暂时中止，如和redis断链等）
 *  2、
 *
 * 实现：
 *      PFProc回调函数的返回值为bool，为true，即处理结束，这个时候由内部自动调用Del，否则该element将一直等待到外部调用Del为止
 *      当返回true时，可以视为一个同步任务，返回false时，可以视为一个异步任务。
 *      CSlidingWindowTask允许排队的任务混杂着同步任务和异步任务，
 */

// T1：处理具体业务的类型，T2：Id的类型（默认使用T1中的类型T1::ElementIdType）
template <typename T1, typename T2 = typename T1::ElementIdType, typename T3 = CUnorderedElementMng<T2> >
class CSlidingWindowTask : public CTask
{
public:
    // PFProc回调函数的返回值为bool，为true，即处理结束，这个时候由内部自动调用Del，否则该element将一直等待到外部调用Del为止
    typedef bool (T1::*PFProc1)(const T2 &tId);
    typedef bool (T1::*PFProc2)(const vector<T2> &cIds);

public:
    enum
    {
        Idle = UNKNOWN_STATE + 1,
        Sliding
    };

    enum SlidingWindowState
    {
        SlidingWindow_Normal = 0,
        SlidingWindow_Pause
    };

public:
    CSlidingWindowTask(CInstExt* pcInst, T1 *pcInstance, PFProc1 pf1, PFProc2 pf2);
    virtual ~CSlidingWindowTask();

public:
    virtual const char* GetObjName() const { return "CSlidingWindowTask"; }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case Sliding:
            return "Sliding";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual void InitStateMachine();
    virtual void PrintData() const;
    virtual bool IsNotNeedPollState() { return true; }

public:
    //状态处理
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnSliding(CMessage *const pcMsg);
    u32 OnSlidingTimer();

public:

    // 主要的对外接口
    void Add(const T2 &tId);
    void Add(const vector<T2> &cIds);
    void Add(const set<T2> &cIdSet);

    // 同步模式下处理结束后由内部自动调用相应的Del接口，如果外部调用，相当于取消排队
    void Del(const T2 &tId);
    void Del(const vector<T2> &cIds);
    void Del(const set<T2> &cIdSet);
    void Reset();

    // 提供暂停、恢复处理机制
    void Pause()  { m_emState = SlidingWindow_Pause; }
    void Resume() { m_emState = SlidingWindow_Normal; Proc(); }
    
    u16  GetWindowSize() const    { return m_wWindowSize;  }
    void SetWindowSize(u16 wSize) { m_wWindowSize = wSize;	m_wWindowSize = (m_wWindowSize == 0) ? m_wDefaultWindowSize : m_wWindowSize; Proc(); }

    u32  GetRestSlidingTimes() const  { return m_pcElementMng->GetRestElement() / m_wWindowSize; }
    bool IsFinished() const { return m_pcElementMng->IsEmpty(); }

protected:

    void Proc();

private:

    SlidingWindowState  m_emState;                          // 运行状态

    u16                  m_wWindowSize;                      // 窗口大小
    static const u16     m_wDefaultWindowSize = 50;          // 默认窗口大小
    set<T2>              m_cProcessing;                      // 当前正在处理中的element集合

    CElementMngBase<T2> *m_pcElementMng;                     // element管理类
    T1                  *m_pcInstance;
    PFProc1              m_pfInstanceProc1;                  // 具体事务处理回调
    PFProc2              m_pfInstanceProc2;                  // 具体事务处理回调
};

template <typename T1, typename T2, typename T3>
CSlidingWindowTask<T1, T2, T3>::CSlidingWindowTask(CInstExt* pcInst, T1 *pcInstance, PFProc1 pf1, PFProc2 pf2)
    : CTask(pcInst), m_pcInstance(pcInstance), m_pfInstanceProc1(pf1), m_pfInstanceProc2(pf2)
{
    CMS_ASSERT(NULL != m_pcInstance && "m_pcInstance = NULL!!!");
    CMS_ASSERT((NULL != m_pfInstanceProc1 || NULL != m_pfInstanceProc2) && "m_pfInstanceProc = NULL!!!");

    m_emState     = SlidingWindow_Normal;
    m_wWindowSize = m_wDefaultWindowSize;

    m_pcElementMng = new T3();
}

template <typename T1, typename T2, typename T3>
CSlidingWindowTask<T1, T2, T3>::~CSlidingWindowTask()
{
    if (NULL != m_pcElementMng)
    {
        delete m_pcElementMng;
        m_pcElementMng = NULL;
    }
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::InitStateMachine()
{
    CStateProc cIdleProc;
    cIdleProc.ProcMsg = (CTask::PFProcMsg)&CSlidingWindowTask<T1, T2>::OnIdle;
    cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CSlidingWindowTask<T1, T2>::OnIdle;
    cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CSlidingWindowTask<T1, T2>::TimerPollSkip;
    AddRuleProc(Idle, cIdleProc);

    CStateProc cSlidingProc;
    cSlidingProc.ProcMsg = (CTask::PFProcMsg)&CSlidingWindowTask<T1, T2>::OnSliding;
    cSlidingProc.ProcErrMsg = (CTask::PFProcMsg)&CSlidingWindowTask<T1, T2>::OnSliding;
    cSlidingProc.TimerPoll = (CTask::PFTimerPoll)&CSlidingWindowTask<T1, T2>::TimerPollSkip;
    AddRuleProc(Sliding, cSlidingProc);

    NextState(Idle);
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\tState[%d], WindowSize[%d], Processing[%u]\n", m_emState, m_wWindowSize, m_cProcessing.size());
    m_pcElementMng->PrintData();
}

template <typename T1, typename T2, typename T3>
u32 CSlidingWindowTask<T1, T2, T3>::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    default:
        {
            TASKLOG(CMU_MOD_REDIS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

template <typename T1, typename T2, typename T3>
u32 CSlidingWindowTask<T1, T2, T3>::OnSliding(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case CMU_SLIDING_WINDOW_PROC:
        {
            NextState(Idle);

            Proc();

            dwRet = PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_REDIS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Add(const T2 &tId)
{
    m_pcElementMng->AddElement(tId);
    
    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Add(const vector<T2> &cIds)
{
    m_pcElementMng->AddElement(cIds);

    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Add(const set<T2> &cIdSet)
{
    m_pcElementMng->AddElement(cIdSet);

    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Del(const T2 &tId)
{
    m_pcElementMng->DelElement(tId);

    // 可能是取消，那么实际上这后面的所有语句可以不执行，不过没有关系
    m_cProcessing.erase(tId);

    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Del(const vector<T2> &cIds)
{
    m_pcElementMng->DelElement(cIds);

    for (typename vector<T2>::const_iterator it = cIds.begin(); cIds.end() != it; ++it)
    {
        m_cProcessing.erase(*it);
    }

    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Del(const set<T2> &cIdSet)
{
    m_pcElementMng->DelElement(cIdSet);

    for (typename set<T2>::const_iterator it = cIdSet.begin(); cIdSet.end() != it; ++it)
    {
        m_cProcessing.erase(*it);
    }

    Proc();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Reset()
{
    m_emState     = SlidingWindow_Normal;
    m_wWindowSize = m_wDefaultWindowSize;

    m_cProcessing.clear();
    m_pcElementMng->Reset();
}

template <typename T1, typename T2, typename T3>
void CSlidingWindowTask<T1, T2, T3>::Proc()
{
    if (SlidingWindow_Normal == m_emState)
    {
        if (Idle == GetState())
        {
            if ((u16)m_cProcessing.size() < m_wWindowSize)
            {
                vector<T2> cIds;
                m_pcElementMng->GetElements(m_wWindowSize - (u16)m_cProcessing.size(), cIds);

                if (NULL != m_pfInstanceProc1)
                {
                    for (typename vector<T2>::iterator it = cIds.begin(); cIds.end() != it;)
                    {
                        if ((m_pcInstance->*m_pfInstanceProc1)(*it))
                        {
                            ++it;
                        }
                        else
                        {
                            m_cProcessing.insert(*it);
                            it = cIds.erase(it);
                        }
                    }
                }
                else if (NULL != m_pfInstanceProc2)
                {
                    if (!(m_pcInstance->*m_pfInstanceProc2)(cIds))
                    {
                        for (typename vector<T2>::const_iterator it = cIds.begin(); cIds.end() != it; ++it)
                        {
                            m_cProcessing.insert(*it);
                        }
                        
                        cIds.clear();
                    }
                }

                m_pcElementMng->DelElement(cIds);

                if (!m_pcElementMng->IsEmpty() && (u16)m_cProcessing.size() < m_wWindowSize)
                {
                    u32 dwTaskNO = GetTaskNO();
                    OspPost(CMU_CORE_DAEMON, CMU_SLIDING_WINDOW_PROC, &dwTaskNO, sizeof(dwTaskNO));

                    NextState(Sliding);
                }
            }
        }
    }
}


#endif  //#ifndef _SLIDING_WINDOW_TASK_H

