/*
��ʼ���Կͻ����������tas�ĸ�������
*/
#ifndef _TAS_CONN_TASK_POWERON_H_
#define _TAS_CONN_TASK_POWERON_H_

#include "inner_common.h"

class CTasConnTaskPowerOn: public CTask
{
public:
    enum
    {
        enStateQueryDstDomainListing = UNKNOWN_STATE + 1, // ���ڲ�ѯĿ�����б�
        enStateQueryDstDomainListTry,                     // ʧ�ܺ������²�ѯĿ�����б�
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
