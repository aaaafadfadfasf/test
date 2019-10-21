/*
ͬ����������������Ȩ��
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
        enStateServerNonblock,  // �������ش���ͬ������
        enStateServerBlock      // �����ش���ͬ������Ŀǰ������Ȩ�������ѯ������״̬����������ͬ�������ȱ�������ʱ�б�
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
    list<TSyncDataAnalyzeInfo> m_tTempSyncDataLst;   // ��ʱ����ͬ����¼
    set<UUIDString> m_cDomainAuthorizationDevIdSet;   // ������Ȩ�豸��ż���
    bool m_bForbidQueryDomainAuthorizationDevIdList;  // ��ֹ��ѯ����Ȩ���豸�б�����ʧ��ʱ���²�ѯ
    
    TSyncDataAnalyzeInfo m_tQueringDomainAuthorizationData;   // ���ڲ�ѯ�豸������ݵ��豸ԭʼ����
    bool m_bForbidQueryDomainAuthorizationDependence;         // ��ֹ��ѯ������Ȩ�������ݣ�����ʧ��ʱ���²�ѯ
};

#endif // _SYNC_ANALYZER_P2PLIMIT_H_
