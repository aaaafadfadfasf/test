/*
同步分析器（上级同步）
*/
#ifndef _SYNC_ANALYZER_PARENT_H_
#define _SYNC_ANALYZER_PARENT_H_

#include "sync_analyzer_base.h"

class CSyncAnalyzerParent: public CSyncAnalyzerBase
{
public:
    enum
    {
        enStateServerNonblock = UNKNOWN_STATE + 1,  // 非阻塞地处理同步请求
        enStateServerBlock      // 阻塞地处理同步请求（目前设备信息的分析可能会进入该状态），新来的同步请求先保存于临时列表
    };
public:
    CSyncAnalyzerParent(CInstExt *pcInst, const string &strDstDomainId)
        : CSyncAnalyzerBase(pcInst, strDstDomainId, enSyncTypeParent){}
    ~CSyncAnalyzerParent(){}

    virtual const char* GetObjName() const{return "SyncAnalyzerParent";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

    void Analyze(const TSyncDataAnalyzeInfo &tData);

private:
    virtual u32 ServerNonblockStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerBlockStateProcMsg(CMessage *const pcMsg);
    u32 ServerBlockStateTimerPoll();

    void EnterServerNonblockState();

    void AnalyzeDevInfo(const TSyncDataAnalyzeInfo &tSyncData);
    void QueryDevInfoRelationData(const TSyncDataAnalyzeInfo *ptSyncData = NULL);
    void HandleTempSyncDataList();

private:
    list<TSyncDataAnalyzeInfo> m_tTempSyncDataLst;   // 暂时缓存同步记录

    TSyncDataAnalyzeInfo m_tQueringDevInfoRelateData; // 正在查询的设备关联数据的原始数据
    bool m_bForbidQueringDevInfoRelateData;           // 禁止查询设备信息依赖数据标志，用于失败时重新查询
};

#endif // _SYNC_ANALYZER_PARENT_H_
