/*
外设模块(ps-Peripheral server)服务端会话任务
*/
#ifndef _PS_SSN_TASK_H_
#define _PS_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CPsSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit   = UNKNOWN_STATE + 1,
        enStateServer
    };
public:
    CPsSsnTask(CInstExt *pcInst);
    ~CPsSsnTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const{return "CPsSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
};

#endif // _PS_SSN_TASK_H_
