/*
ͬ������ϵͳ�ķ�����
���ܣ�����ͬ�����ݵĹ��죬���ͺ�ɾ��
*/

#ifndef _SYNCDATA_SEND_H_
#define _SYNCDATA_SEND_H_
#include "inner_common.h"

class CSyncDataSend: public CSipTask
{
public:
    enum
    {
        enStateQueryData = UNKNOWN_STATE + 1, // ��ѯͬ������
        enStateTransferData,                  // ��������
        enStateDeleteRecord                  // ɾ���Ѿ����ͼ�¼
    };
public:
    CSyncDataSend(CInstExt *pcInst, const UUIDString &strDstDomainId, ENSyncType enSyncType, const string &strDstDomainName, string strSessionId)
        : CSipTask(pcInst), m_strDstDomainId(strDstDomainId), m_enSyncType(enSyncType), m_strSessionId(strSessionId)
    {
        m_tPeerUri.SetDomain(strDstDomainName);
        m_tPeerUri = m_tPeerUri.Get3ASURI();
    }

    virtual const char* GetObjName() const{return "CSyncDataSend";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

    virtual u32 QueryDataStateProcMsg(CMessage *const pcMsg);
    u32 QueryDataStateTimerPoll();
    virtual u32 TransferDataStateProcMsg(CMessage *const pcMsg);
    u32 TransferDataStateTimerPoll();
    virtual u32 DeleteRecordStateProcMsg(CMessage *const pcMsg);
    u32 DeleteRecordStateTimerPoll();

    void EnterQueryDataState();
    void EnterTransferDataState();
    void EnterDeleteRecordState();


private:
    void QueryData();
    void TransferData();
    void DeleteRecord();

private:
    bool m_bForbidQueryData;
    vector<TSyncDataInfo> m_tSyncDataInfos;
    bool m_bForbidSendSyncData;
    bool m_bForbidDeleteRecord;

    UUIDString m_strDstDomainId; // Ŀ������
    ENSyncType m_enSyncType;     // ͬ������
    TSipURI m_tPeerUri;          // �Զ�ģ���URI
    string m_strSessionId;       // �����ĻỰ�ı��
};

#endif // _SYNCDATA_SEND_H_
