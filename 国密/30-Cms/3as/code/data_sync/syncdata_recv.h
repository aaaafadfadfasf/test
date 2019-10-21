/*
ͬ������ϵͳ�Ľ���������
���ܣ�����ͬ�����ݵĽ��գ�����Ӧ��
*/

#ifndef _SYNCDATA_RECV_H_
#define _SYNCDATA_RECV_H_
#include "inner_common.h"

class CSyncDataRecv: public CSipTask
{
public:
    enum
    {
        enStateRecvData = UNKNOWN_STATE + 1, // ��������
        enStateStoreData,                    // �洢����
    };

public:
    CSyncDataRecv(CInstExt *pcInst, const TSipURI &tPeerUri, ENSyncType enSyncType)
        : CSipTask(pcInst), m_tPeerUri(tPeerUri), m_enSyncType(enSyncType)
    {
    }

    virtual const char* GetObjName() const{return "CSyncDataRecv";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();

    virtual u32 RecvDataStateProcMsg(CMessage *const pcMsg);
    u32 RecvDataStateTimerPoll();
    virtual u32 StoreDataStateProcMsg(CMessage *const pcMsg);
    u32 StoreDataStateTimerPoll();

private:
    u32 m_dwReqSeq;                // ͬ�����������seq
    string m_strReqSessId;         // ͬ�����������session
    TKDSIP_TRANS_ID m_tReqTransId; // ͬ�����������TransId
     
    TSipURI m_tPeerUri;      // �Զ�ģ���URI
    ENSyncType m_enSyncType; // ͬ������
};

#endif // _SYNCDATA_RECV_H_

