/*
    处理设计有NtfReq的查询请求, 它依附于具体的会话Task任务.
    数据库查询行为：先发通知最后发送应答。
*/

#ifndef _QUERY_TASK_H_
#define _QUERY_TASK_H_
#include "inner_common.h"
class CSsnTaskbase;

class CQueryTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
        enStateQuery,    // 查询状态，数据库的结果还未返回过来
        enStateTransRsp, // 数据传输的状态(传输Rsp阶段)
        enStateTransNtf, // 数据传输的状态(传输Ntf阶段)
    };

    CQueryTask(CSsnTaskbase* pcSsnTask);
    ~CQueryTask();
    virtual void PrintData() const;

public:
    virtual const char* GetObjName() const{return m_strObjName.c_str();}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnQuery(CMessage *const pcMsg);
    u32 QueryTimerPoll();
    u32 OnTransRsp(CMessage *const pcMsg);
    u32 TransRspTimerPoll();
    u32 OnTransNtf(CMessage *const pcMsg);
    u32 TransNtfTimerPoll();
    bool Query(CMessage *const pcMsg);
    bool DoTransfer(bool bRsp);

    void DoNextState(u32 enState);
private:
    list<const CTaskMsgTransferBase*> m_cQryNtfLst; // 查询通知列表，它们需要顺序发送出去
    CTaskMsgTransferBase* m_pcQryRsp; // 查询应答
    string m_strQueryTag;    // 查询标签(用于标识查询通知所属的查询)
    string m_strQueryReq;    // 查询请求
    TSipURI m_tSipUri;    // Ntf发送给的UA标识
    CSsnTaskbase *m_pcSsnTask; // 依附的会话任务
    string m_strObjName; // 任务名称(附上查询事件名称)，主要用于打印
};

#endif // _QUERY_TASK_H_
