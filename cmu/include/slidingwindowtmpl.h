/*========================================================================================
ģ����    ��cmu
�ļ���    ��slidingwindowtask.h
����ļ�  ��
ʵ�ֹ���  ��Ŀǰ�豸״̬��redis��ֻ��һ���ֶ�����ʾ�������Ļ������¼������ߣ�pui����ʱ��������ص�
            �豸״̬��Ҫ��һ���޸ģ���������������task����Ϊ��ʵ���������
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2017/01/19         V1.0              xiongxh                                �½��ļ�
=========================================================================================*/

/*
 * ���˼·��
 *   1����taskӦ�ÿ����ж��ʵ�����һ���Ӱ�졣��������¼������һ��ʵ�������pui��gbu������һ����������ʵ���䲻Ӧ�������
 *   2����task��elementΪ��λ���������ݣ�ÿ�ο��Դ���һ�������������������Ͷ�ݳ���һ�δ������Ϣ���������߳̿�������������
 *      ֮����һ���Ŀ�϶ȥ��������������
 *   3��֧�ֿ��ٵ���ӡ�ɾ��element
 *   4��֧��ͬ�����첽���ֹ���ģʽ���첽ģʽ����ʹ������ɾ��element��
 *   5��֮ǰʵ����һ�����ƵĻ��ƣ���CFlowCtrlMngTask
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


/* element������
 * ���ԣ�todo by xiongxh ����������Ʋ����ϣ������£�
 *  1��֧���Ŷӻ��ƣ�ʹ��list������������ݣ����ڿ�����ӡ�ɾ�������ǻ���list�����ԣ���ֹ����size()������
 *  2����ֹ�ظ��Ŷӣ�ʹ��set����֤��
 *  3��֧�������Ŷӣ�����һ���Բ������element��
 *  4��֧�����ȡ���Ŷӣ������ԴӶ����м���ӣ�
 *  5��֧�ֿ��ٵ���ӡ�ɾ��element��ʹ��set�����п��ٵ���ӡ�ɾ�������ң�
 *  6��֧��ͳ�ƣ����ں����Ų�����͵�������
 *
 * ʵ�֣�
 *      һ������£�Ҫʵ�ֶ��л��ƣ�����ʹ��queue��list�ȣ���Ƚ϶��ԣ�list����ӡ�ɾ��Ԫ���ϸ��졣���Ƕ���һ���ǲ��ܱ�����Ҳ�޷����м�
 *  ɾ��Ԫ�صģ�CElementMng����Ҫ֧�ֺ��ŶӺ�ȡ���ŶӵĹ��ܣ�Ϊ���ڶ��еĻ�����Ҳ�ܿ��ٵ�ȡ���Ŷӣ������ɾ��Ԫ�أ�������ͬʱʹ��list��
 *  set���Կռ任ʱ�䣬�����˼·�ǣ�list��set�ж������Ŷ����ݣ�����Ҫȡ���Ŷӵ�ʱ��ֻ��set��ɾ��Ԫ�أ������ͻ����ĳЩԪ�ش���list��
 *  ���ǲ�����set�е������������������Ѿ�ȡ�����Ŷ�Ԫ�ء�Ҫע����ǣ�ʹ��������ƵĻ������ܵ����ظ��Ŷӵ����⣬����ĳ��Ԫ����ȡ������
 *  ���Ŷӣ����ʱ��list�н��ж����Ԫ�أ���set��ֻ��һ������CElementMngҪ��֤ȡ���������ݲ����ظ��������Ļ���ȡ���ݵ�ʱ�����Ҫ��Щ����
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
    list<T>             m_cElementQueue;            // ���У�ʵ���Ŷӻ���
    set<T>              m_cElementSet;              // ���ϣ����ڿ��ٲ��ҡ���ӡ�ɾ�����ռ任ʱ�䣩

    u32                 m_dwTotal;                  // element�ܼ���
    u32                 m_dwTop;                    // element��ֵ
};

template <typename T>
void COrderedElementMng<T>::AddElement(const T &tElementId)
{
    if (m_cElementSet.end() == m_cElementSet.find(tElementId))          // �����ظ��Ŷ�
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
    set<T>              m_cElementSet;              // ���������Ԫ��

    u32                 m_dwTotal;                  // element�ܼ���
    u32                 m_dwTop;                    // element��ֵ
};

template <typename T>
void CUnorderedElementMng<T>::AddElement(const T &tElementId)
{
    if (m_cElementSet.end() == m_cElementSet.find(tElementId))          // �����ظ��Ŷ�
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

/* ����������
 * ���ԣ�
 *  1��֧����ͣ���ָ����ⲿ����ĳЩ�¼���������Ҫ��ʱ��ֹ�����redis�����ȣ�
 *  2��
 *
 * ʵ�֣�
 *      PFProc�ص������ķ���ֵΪbool��Ϊtrue����������������ʱ�����ڲ��Զ�����Del�������element��һֱ�ȴ����ⲿ����DelΪֹ
 *      ������trueʱ��������Ϊһ��ͬ�����񣬷���falseʱ��������Ϊһ���첽����
 *      CSlidingWindowTask�����Ŷӵ����������ͬ��������첽����
 */

// T1���������ҵ������ͣ�T2��Id�����ͣ�Ĭ��ʹ��T1�е�����T1::ElementIdType��
template <typename T1, typename T2 = typename T1::ElementIdType, typename T3 = CUnorderedElementMng<T2> >
class CSlidingWindowTask : public CTask
{
public:
    // PFProc�ص������ķ���ֵΪbool��Ϊtrue����������������ʱ�����ڲ��Զ�����Del�������element��һֱ�ȴ����ⲿ����DelΪֹ
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
    //״̬����
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnSliding(CMessage *const pcMsg);
    u32 OnSlidingTimer();

public:

    // ��Ҫ�Ķ���ӿ�
    void Add(const T2 &tId);
    void Add(const vector<T2> &cIds);
    void Add(const set<T2> &cIdSet);

    // ͬ��ģʽ�´�����������ڲ��Զ�������Ӧ��Del�ӿڣ�����ⲿ���ã��൱��ȡ���Ŷ�
    void Del(const T2 &tId);
    void Del(const vector<T2> &cIds);
    void Del(const set<T2> &cIdSet);
    void Reset();

    // �ṩ��ͣ���ָ��������
    void Pause()  { m_emState = SlidingWindow_Pause; }
    void Resume() { m_emState = SlidingWindow_Normal; Proc(); }
    
    u16  GetWindowSize() const    { return m_wWindowSize;  }
    void SetWindowSize(u16 wSize) { m_wWindowSize = wSize;	m_wWindowSize = (m_wWindowSize == 0) ? m_wDefaultWindowSize : m_wWindowSize; Proc(); }

    u32  GetRestSlidingTimes() const  { return m_pcElementMng->GetRestElement() / m_wWindowSize; }
    bool IsFinished() const { return m_pcElementMng->IsEmpty(); }

protected:

    void Proc();

private:

    SlidingWindowState  m_emState;                          // ����״̬

    u16                  m_wWindowSize;                      // ���ڴ�С
    static const u16     m_wDefaultWindowSize = 50;          // Ĭ�ϴ��ڴ�С
    set<T2>              m_cProcessing;                      // ��ǰ���ڴ����е�element����

    CElementMngBase<T2> *m_pcElementMng;                     // element������
    T1                  *m_pcInstance;
    PFProc1              m_pfInstanceProc1;                  // ����������ص�
    PFProc2              m_pfInstanceProc2;                  // ����������ص�
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

    // ������ȡ������ôʵ�������������������Բ�ִ�У�����û�й�ϵ
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

