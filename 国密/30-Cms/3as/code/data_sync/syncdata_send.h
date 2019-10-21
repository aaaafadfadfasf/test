/*
同步传输系统的发送器
功能：负责同步数据的构造，发送和删除
*/

#ifndef _SYNCDATA_SEND_H_
#define _SYNCDATA_SEND_H_
#include "inner_common.h"

class CSyncDataSend: public CSipTask
{
public:
    enum
    {
        enStateQueryData = UNKNOWN_STATE + 1, // 查询同步数据
        enStateTransferData,                  // 传输数据
        enStateDeleteRecord                  // 删除已经发送记录
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

    UUIDString m_strDstDomainId; // 目的域编号
    ENSyncType m_enSyncType;     // 同步类型
    TSipURI m_tPeerUri;          // 对端模块的URI
    string m_strSessionId;       // 依附的会话的编号
};

#endif // _SYNCDATA_SEND_H_
