/*
license管理任务
*/
#ifndef _LICENSE_MNG_TASK_H_
#define _LICENSE_MNG_TASK_H_

#include "inner_common.h"

class CLicenseMngTask: public CSipTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1,
    };

    CLicenseMngTask(CInstExt *pcInst);
    ~CLicenseMngTask();

    virtual const char* GetObjName() const{return "CLicenseMngTask";}
    virtual const char* GetStateName(u32 dwState) const{return "ServerState";}
    virtual void InitStateMachine();

private:
    u32 OnServer(CMessage *const pcMsg);
};

#endif // _LICENSE_MNG_TASK_H_
