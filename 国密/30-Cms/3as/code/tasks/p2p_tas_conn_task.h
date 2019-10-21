/*
点对点同步客户端会话任务
*/
#ifndef _P2P_TAS_CONN_TASK_H_
#define _P2P_TAS_CONN_TASK_H_
#include "common/conn_taskbase.h"

class CSyncDataSend;

class CP2PTasConnTask : public CConnTaskbase
{
public:
    enum
    {
        enStateLogining = UNKNOWN_STATE + 1,
        enStateLoginTry,
        enStateLogined
    };
public:
    CP2PTasConnTask(CInstExt *pcInst, const string &strPeerDomainId, const string &strPeerDomainName, ENSyncType enSyncType);
    ~CP2PTasConnTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const{return "CP2PTasConnTask";}
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

#endif // _P2P_TAS_CONN_TASK_H_
