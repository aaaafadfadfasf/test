/*
ͬ������������, �����ͬ�����������ݿ����֪ͨͬ�����ݷ���������д��䡣
�㷨��ͨ��Analyze�ӿڴ���ͬ���������󣬺����Ĵ�������ͨ����Ϣ��ת��
*/

#ifndef _SYNC_ANALYZER_BASE_H_
#define _SYNC_ANALYZER_BASE_H_
#include "inner_common.h"

class CSyncAnalyzerBase: public CTask
{
public:
    CSyncAnalyzerBase(CInstExt *pcInst, const string &strDstDomainId, ENSyncType enSyncType)
        : CTask(pcInst), m_strDstDomainId(strDstDomainId), m_enSyncType(enSyncType), m_bForbidSendStoreReq(false){}
    virtual ~CSyncAnalyzerBase(){}

public:
    // ʵ���㷨�����������ܱ�����������
    virtual void Analyze(const TSyncDataAnalyzeInfo &tData);
    
    u32 WaitingSyncDataRecordNumber(); // �ȴ�ͬ�����ݼ�¼������
    u32 PollSyncDataLst();             // ��ʱ��ѭ�������Է��ʹ洢����
    

protected:
    u32 DefaultProc(CMessage *const pcMsg);
    void SendStoreReq();

protected:
    string m_strDstDomainId;                   // Ŀ������
    ENSyncType m_enSyncType;                   // ͬ������
    list<TSyncDataRecordStoreReq> m_cSyncData; // �����ͬ����¼����Ҫ���򱣴浽DB��
    bool m_bForbidSendStoreReq;                // ��ֹ���ʹ洢�����־, һ������һ��Ӧ�������
};

#endif // _SYNC_ANALYZER_BASE_H_
