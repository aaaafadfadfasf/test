/*
父亲TAS客户端会话任务
*/
#ifndef _PARENT_TAS_CONN_TASK_H_
#define _PARENT_TAS_CONN_TASK_H_
#include "common/conn_taskbase.h"
#include "data_sync/syncdata_send.h"
#include "commonconfig/cmcfg.h"

#define ParentTasConnTaskName "CParentTasConnTask"

#define PLAT_VERSION_V2R2B1   "Phoenix 2.2.1.0.1 2014.11.28"

extern map<string, TParentSyncRspLoginExternInfo> g_cParentSyncExtInfoList;

class CParentTasConnTask : public CConnTaskbase
{
public:
    enum
    {
        enStateLogining = UNKNOWN_STATE + 1,
        enStateLoginTry,
        enStateLogined
    };
public:
    CParentTasConnTask(CInstExt *pcInst, const string &strPeerDomainId, const string &strPeerDomainName);
    ~CParentTasConnTask();
    virtual void PrintData() const;

public:
    //子类需要重写的方法
    virtual const char* GetObjName() const{return ParentTasConnTaskName;}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 LoginingStateProcMsg(CMessage *const pcMsg);
    u32 LoginingStateTimerPoll();
    virtual u32 LoginTryStateProcMsg(CMessage *const pcMsg);
    u32 LoginTryStateTimerPoll();
    virtual u32 LoginedStateProcMsg(CMessage *const pcMsg);

    void EnterLoginingState();
    void EnterLoginedState();

    u32 OnDisconnect();

private:
    CSyncDataSend *m_pcSyncDataSendTask;
};

#endif // _PARENT_TAS_CONN_TASK_H_
