/*
儿子TAS服务端会话任务
*/

#ifndef _CHILD_TAS_SSN_TASK_H_
#define _CHILD_TAS_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CSyncDataRecv;

class CChildTasSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit = UNKNOWN_STATE + 1,
        enStateLogining,
        enStateLogined
    };
public:
    CChildTasSsnTask(CInstExt *pcInst);
    ~CChildTasSsnTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const{return "CChildTasSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginingStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginedStateProcMsg(CMessage *const pcMsg);

    void EnterInitState();    // 进入初始化状态
    void EnterLoginedState(); // 进入已经登录状态

private:
    TSipURI m_tPeerTasUri; // 对端TAS的URI
    CSyncDataRecv *m_pcSyncDataRecvTask; // 同步数据接收任务
};

#endif // _CHILD_TAS_SSN_TASK_H_
