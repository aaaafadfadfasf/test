/*
ͬ�����������ϼ�ͬ����
*/
#ifndef _SYNC_ANALYZER_PARENT_H_
#define _SYNC_ANALYZER_PARENT_H_

#include "sync_analyzer_base.h"

class CSyncAnalyzerParent: public CSyncAnalyzerBase
{
public:
    enum
    {
        enStateServerNonblock = UNKNOWN_STATE + 1,  // �������ش���ͬ������
        enStateServerBlock      // �����ش���ͬ������Ŀǰ�豸��Ϣ�ķ������ܻ�����״̬����������ͬ�������ȱ�������ʱ�б�
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
    list<TSyncDataAnalyzeInfo> m_tTempSyncDataLst;   // ��ʱ����ͬ����¼

    TSyncDataAnalyzeInfo m_tQueringDevInfoRelateData; // ���ڲ�ѯ���豸�������ݵ�ԭʼ����
    bool m_bForbidQueringDevInfoRelateData;           // ��ֹ��ѯ�豸��Ϣ�������ݱ�־������ʧ��ʱ���²�ѯ
};

#endif // _SYNC_ANALYZER_PARENT_H_
