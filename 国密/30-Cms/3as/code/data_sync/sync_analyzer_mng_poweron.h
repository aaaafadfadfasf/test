/*
ͬ�����ݷ�����ϵͳ���ϵ紦������
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
        enDataStateInit,    // ��ʼ��״̬
        enDataStateWaiting, // �ȴ�����
        enDataStateFailed,  // �ȴ�����ʧ��
        enDataStateReady    // �ȴ����ݾ���
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
    ENDataState m_enPrivateDevIdListState;   // ˽���豸��ȡ״̬
    ENDataState m_enSubdomainIdListState;    // �¼������б��ȡ״̬
    ENDataState m_enSyncDstDomainListState;  // ͬ��Ŀ�����б��ȡ״̬

    vector<TSyncDstDomainInfo> m_cSyncDstDomainList; // ͬ��Ŀ�����б��ȡ
};

#endif // _SYNC_ANALYZER_MNG_POWERON_H_
