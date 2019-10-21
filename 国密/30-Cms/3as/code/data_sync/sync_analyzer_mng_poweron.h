/*
同步数据分析子系统，上电处理任务
*/

#ifndef _SYNC_ANALYZER_MNG_POWERON_H_
#define _SYNC_ANALYZER_MNG_POWERON_H_

#include "inner_common.h"

class CSyncAnalyzerMngPoweron: public CTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1
    };
    enum ENDataState
    {
        enDataStateInit,    // 初始化状态
        enDataStateWaiting, // 等待数据
        enDataStateFailed,  // 等待数据失败
        enDataStateReady    // 等待数据就绪
    };

    CSyncAnalyzerMngPoweron(CInstExt *pcInst);
    ~CSyncAnalyzerMngPoweron();

    virtual const char* GetObjName() const{return "SyncAnalyzerMngPoweron";}
    virtual const char* GetStateName(u32 dwState) const{return "enStateServer";}
    virtual void InitStateMachine();

private:
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);
    u32 ServerTimerPoll();

private:
    void QueryPrivateDevIdList();
    void QuerySubdomainIdList();
    void QuerySyncDstDomainList();
    bool IsAllReady();
    void CreateSyncAnalyzers();

private:
    ENDataState m_enPrivateDevIdListState;   // 私有设备获取状态
    ENDataState m_enSubdomainIdListState;    // 下级域编号列表获取状态
    ENDataState m_enSyncDstDomainListState;  // 同步目标域列表获取状态

    vector<TSyncDstDomainInfo> m_cSyncDstDomainList; // 同步目标域列表获取
};

#endif // _SYNC_ANALYZER_MNG_POWERON_H_
