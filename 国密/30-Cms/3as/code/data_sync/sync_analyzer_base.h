/*
同步分析器基类, 保存待同步数据至数据库表并且通知同步数据发送任务进行传输。
算法：通过Analyze接口处理同步分析请求，后续的处理流程通过消息流转。
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
    // 实现算法：无条件接受被分析的数据
    virtual void Analyze(const TSyncDataAnalyzeInfo &tData);
    
    u32 WaitingSyncDataRecordNumber(); // 等待同步数据记录的条数
    u32 PollSyncDataLst();             // 定时轮循处理，尝试发送存储请求
    

protected:
    u32 DefaultProc(CMessage *const pcMsg);
    void SendStoreReq();

protected:
    string m_strDstDomainId;                   // 目的域编号
    ENSyncType m_enSyncType;                   // 同步类型
    list<TSyncDataRecordStoreReq> m_cSyncData; // 保存待同步记录，需要按序保存到DB中
    bool m_bForbidSendStoreReq;                // 禁止发送存储请求标志, 一个请求一个应答按序进行
};

#endif // _SYNC_ANALYZER_BASE_H_
