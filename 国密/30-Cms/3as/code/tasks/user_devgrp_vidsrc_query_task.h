/*
    指定用户指定设备组下递归查询所有视频源（先Rsp后Ntf，不支持分页）
*/

#ifndef _USER_DEVGRP_VIDSRC_QUERY_TASK_H_
#define _USER_DEVGRP_VIDSRC_QUERY_TASK_H_
#include "inner_common.h"
class CSsnTaskbase;

class CUserDevGrpVidsrcQryTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
		enStateQuery,
        enStateTransNtf, // 数据传输的状态(传输Ntf阶段)
    };

    CUserDevGrpVidsrcQryTask(CSsnTaskbase* pcSsnTask);
    ~CUserDevGrpVidsrcQryTask();
public:
    virtual const char* GetObjName() const{return m_strObjName.c_str();}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnQuery(CMessage *const pcMsg);
    u32 QueryTimerPoll();
    u32 OnTransNtf(CMessage *const pcMsg);
    u32 TransNtfTimerPoll();
    bool Query(CMessage *const pcMsg);
    bool DoTransfer(bool bRsp);

    void DoNextState(u32 enState);
private:
	bool m_bHasGetLastNtf;
	bool m_bWaitNtfRsp;
    list<const CTaskMsgTransferBase*> m_cQryNtfLst; // 查询通知列表，它们需要顺序发送出去
    CTaskMsgTransferBase* m_pcQryRsp; // 查询应答
    string m_strQueryTag;    // 查询标签(用于标识查询通知所属的查询)
    TSipURI m_tSipUri;    // Ntf发送给的UA标识
    CSsnTaskbase *m_pcSsnTask; // 依附的会话任务
    string m_strObjName; // 任务名称(附上查询事件名称)，主要用于打印
};

#endif // _USER_DEVGRP_VIDSRC_QUERY_TASK_H_
