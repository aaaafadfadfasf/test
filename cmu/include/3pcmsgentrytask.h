
#ifndef _3PC_MSG_ENTRY_TASK_H
#define _3PC_MSG_ENTRY_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "3pcmsgtask.h"

class C3PCMsgEntryTask : public C3PCSwitchEntryTask
{
public:
    enum TSwitchTaskState
    {
        Idle,
        WaitRsp
    };

public:
    C3PCMsgEntryTask(CInstExt *pcInst);
    virtual ~C3PCMsgEntryTask();

public:
    virtual const char* GetObjName() const
    {
        return "C3PCMsgEntryTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRsp:
            return "WaitRsp";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();

public:
    //×´Ì¬´¦Àí
    u32 OnIdle(CMessage *const pcMsg);

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    virtual void ProcSwitchFinished();
    virtual void ProcSwitchExit(CRedis3PCDstSwitchUnitTask *pcDstSwitch);

private:
    TKDSIP_TRANS_ID m_tTransId;
    CStartThirdPartyCallReq m_cReq;
};


#endif  //#ifndef _SWITCH_CREATOR_ENTRY_TASK_H