/*
同步传输系统的接收器基类
功能：负责同步数据的接收，入库和应答
*/

#ifndef _SYNCDATA_RECV_H_
#define _SYNCDATA_RECV_H_
#include "inner_common.h"

class CSyncDataRecv: public CSipTask
{
public:
    enum
    {
        enStateRecvData = UNKNOWN_STATE + 1, // 接收数据
        enStateStoreData,                    // 存储数据
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
    u32 m_dwReqSeq;                // 同步数据请求的seq
    string m_strReqSessId;         // 同步数据请求的session
    TKDSIP_TRANS_ID m_tReqTransId; // 同步数据请求的TransId
     
    TSipURI m_tPeerUri;      // 对端模块的URI
    ENSyncType m_enSyncType; // 同步类型
};

#endif // _SYNCDATA_RECV_H_

