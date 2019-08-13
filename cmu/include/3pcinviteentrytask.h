
#ifndef _3PC_INVITE_ENTRY_TASK_H
#define _3PC_INVITE_ENTRY_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "3pcinvitetask.h"

class C3PCInviteEntryTask : public C3PCSwitchEntryTask
{
public:
    enum TSwitchTaskState
    {
        Idle,
        WaitAck, 
        WaitRsp,
        Service
    };

public:
    C3PCInviteEntryTask(CInstExt *pcInst);
    virtual ~C3PCInviteEntryTask();

public:
    virtual const char* GetObjName() const
    {
        return "C3PCInviteEntryTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRsp:
            return "WaitRsp";
        case WaitAck:
            return "WaitAck";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState() { return (Idle == GetState() || Service == GetState()); }

public:
    //×´Ì¬´¦Àí
    u32 OnIdle(CMessage *const pcMsg);

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    u32 OnWaitAck(CMessage *const pcMsg);
    u32 OnWaitAckTimer();

    u32 OnService(CMessage *const pcMsg);

    void ProcByeReq(CMessage *const pcMsg);

    virtual void ProcSwitchFinished();
    virtual void ProcSwitchExit(CRedis3PCDstSwitchUnitTask *pcDstSwitch);

private:
    TKDSIP_TRANS_ID  m_tTransId;
    TKDSIP_DIALOG_ID m_tDlgId;
    CInviteThirdPartyCallReq m_cReq;
};


#endif  //#ifndef _SWITCH_CREATOR_ENTRY_TASK_H