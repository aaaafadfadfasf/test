/*
初始化以客户端身份连接tas的各个任务
*/
#ifndef _TAS_CONN_TASK_POWERON_H_
#define _TAS_CONN_TASK_POWERON_H_

#include "inner_common.h"

class CTasConnTaskPowerOn: public CTask
{
public:
    enum
    {
        enStateQueryDstDomainListing = UNKNOWN_STATE + 1, // 正在查询目的域列表
        enStateQueryDstDomainListTry,                     // 失败后尝试重新查询目的域列表
    };
    CTasConnTaskPowerOn(CInstExt *pcInst);

    virtual const char* GetObjName() const{return "CTasConnTaskPowerOn";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

private:
    virtual u32 QueryDstDomainListingStateProcMsg(CMessage *const pcMsg);
    virtual u32 QueryDstDomainListTryStateProcMsg(CMessage *const pcMsg);
    u32 QueryDstDomainListTryStateTimerPoll();

private:
    void EnterQueryDstDomainListingState();
};

#endif // _TAS_CONN_TASK_POWERON_H_
