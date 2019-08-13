
#ifndef _SWITCH_ENTRY_TASK_H
#define _SWITCH_ENTRY_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

class CSwitchEntryTask : public CSipTask
{
public:
    enum TSwitchTaskState
    {
        Idle
    };

public:
    CSwitchEntryTask(CInstExt *pcInst);
    virtual ~CSwitchEntryTask();

public:
    virtual const char* GetObjName() const
    {
        return "CSwitchEntryTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
	virtual bool IsNotNeedPollState() { return true; }

public:
    //×´Ì¬´¦Àí
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();
};


#endif  //#ifndef _SWITCH_ENTRY_TASK_H