/*
    指定用户指定设备组下递归查询所有视频源（先Rsp后Ntf，不支持分页）
*/

#ifndef _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
#define _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
#include "inner_common.h"
class CSsnTaskbase;

class CDevGrpVidsrcTotalnumGetTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
		enStateGet,
    };

    CDevGrpVidsrcTotalnumGetTask(CSsnTaskbase* pcSsnTask);
    ~CDevGrpVidsrcTotalnumGetTask();
public:
    virtual const char* GetObjName() const{return m_strObjName.c_str();}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnGet(CMessage *const pcMsg);
    u32 OnGetTimerPoll();
    bool Get(CMessage *const pcMsg);
    void DoNextState(u32 enState);

private:
    CSsnTaskbase *m_pcSsnTask; // 依附的会话任务
    string m_strObjName; // 任务名称(附上查询事件名称)，主要用于打印
};

#endif // _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
