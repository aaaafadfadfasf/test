/*
tas¿ØÖÆÈÎÎñ
*/
#ifndef _TAS_CTRL_TASK_H_
#define _TAS_CTRL_TASK_H_

#include "inner_common.h"

class CTasCtrlTask: public CSipTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1,
    };

    CTasCtrlTask(CInstExt *pcInst);
    ~CTasCtrlTask();

    virtual const char* GetObjName() const{return "CTasCtrlTask";}
    virtual const char* GetStateName(u32 dwState) const{return "ServerState";}
    virtual void InitStateMachine();

private:
    u32 OnServer(CMessage *const pcMsg);
};

#endif // _TAS_CTRL_TASK_H_
