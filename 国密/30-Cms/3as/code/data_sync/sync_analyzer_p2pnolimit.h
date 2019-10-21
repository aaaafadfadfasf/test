/*
同步分析器（全联通场景）
*/
#ifndef _SYNC_ANALYZER_P2PNOLIMIT_H_
#define _SYNC_ANALYZER_P2PNOLIMIT_H_

#include "sync_analyzer_base.h"

class CSyncAnalyzerP2PNoLimit: public CSyncAnalyzerBase
{
public:
    CSyncAnalyzerP2PNoLimit(CInstExt *pcInst, const string &strDstDomainId)
        : CSyncAnalyzerBase(pcInst, strDstDomainId, enSyncTypeP2PNoLimit){}
    ~CSyncAnalyzerP2PNoLimit(){}

    virtual const char* GetObjName() const{return "CSyncAnalyzerP2PNoLimit";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

public:
    void Analyze(const TSyncDataAnalyzeInfo &tData);

};

#endif // _SYNC_ANALYZER_P2PNOLIMIT_H_
