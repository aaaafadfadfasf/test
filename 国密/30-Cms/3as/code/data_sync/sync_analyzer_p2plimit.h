/*
同步分析器（跨域授权）
*/

#ifndef _SYNC_ANALYZER_P2PLIMIT_H_
#define _SYNC_ANALYZER_P2PLIMIT_H_

#include "sync_analyzer_base.h"

class CSyncAnalyzerP2PLimit: public CSyncAnalyzerBase
{
public:
    enum
    {
        enStateInit = UNKNOWN_STATE + 1,
        enStateServerNonblock,  // 非阻塞地处理同步请求
        enStateServerBlock      // 阻塞地处理同步请求（目前跨域授权依赖项查询会进入该状态），新来的同步请求先保存于临时列表
    };
public:
    CSyncAnalyzerP2PLimit(CInstExt *pcInst, const string &strDstDomainId)
        : CSyncAnalyzerBase(pcInst, strDstDomainId, enSyncTypeP2PLimit),
        m_bForbidQueryDomainAuthorizationDevIdList(false), m_bForbidQueryDomainAuthorizationDependence(true){}
    ~CSyncAnalyzerP2PLimit(){}

    virtual const char* GetObjName() const{return "SyncAnalyzerP2PLimit";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

public:
    void Analyze(const TSyncDataAnalyzeInfo &tData);

private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    u32 InitStateTimerPoll();
    virtual u32 ServerNonblockStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerBlockStateProcMsg(CMessage *const pcMsg);
    u32 ServerBlockStateTimerPoll();

    void EnterServerNonblockState();

    void QueryDomainAuthorizationDevIdList(); 
    void QueryDomainAuthorizationDependence(const TSyncDataAnalyzeInfo *ptNewSyncData = NULL);
    void HandleTempSyncDataList();

private:
    list<TSyncDataAnalyzeInfo> m_tTempSyncDataLst;   // 暂时缓存同步记录
    set<UUIDString> m_cDomainAuthorizationDevIdSet;   // 跨域授权设备编号集合
    bool m_bForbidQueryDomainAuthorizationDevIdList;  // 禁止查询跨域权限设备列表，用于失败时重新查询
    
    TSyncDataAnalyzeInfo m_tQueringDomainAuthorizationData;   // 正在查询设备相关数据的设备原始数据
    bool m_bForbidQueryDomainAuthorizationDependence;         // 禁止查询跨域授权依赖数据，用于失败时重新查询
};

#endif // _SYNC_ANALYZER_P2PLIMIT_H_
