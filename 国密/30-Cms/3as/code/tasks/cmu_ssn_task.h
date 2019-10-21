/*
CMUģ�����˻Ự����
*/
#ifndef _CMU_SSN_TASK_H_
#define _CMU_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CCmuSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit   = UNKNOWN_STATE + 1,
        enStateServer
    };
public:
    CCmuSsnTask(CInstExt *pcInst);
    ~CCmuSsnTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const{return "CCmuSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
};

#endif // _CMU_SSN_TASK_H_
