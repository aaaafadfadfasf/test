/*
����ģ��(ps-Peripheral server)����˻Ự����
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
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const{return "CPsSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
};

#endif // _PS_SSN_TASK_H_
