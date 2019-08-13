/*****************************************************************************
模块名      : cmu 
文件名      : uaslogtask.h
相关文件    : 
文件实现功能: CUasLogTask定义
作者        : 
版本        : vs_phoenix  Copyright(C) 2009-2016 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2016/7/6       1.0         zhou        创建
******************************************************************************/

#ifndef _UAS_LOG_TASK_H
#define _UAS_LOG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/uas/uas_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmucoreapp.h"
#include <queue>

using std::queue;


class CUasLogTask : public CSipTask
{
public:
    enum
    {
        WaitRsp = UNKNOWN_STATE+1,
        Service,
    };

public:
    CUasLogTask(CInstExt *pcInst);
    virtual ~CUasLogTask();

public:
    virtual const char* GetObjName() const
    {
        return "CUasLogTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitRsp:
            return "WaitRsp";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return false;
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData() const;

public:
    //状态处理
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
	void AddAlarmLog(const TAlarmLogInfo& var);
	void AddDeviceLog(const TDeviceLogInfo& var);
	void AddPuLog(const TDeviceLogInfo& var);
	void AddOperateLog(const TOperateLogInfo& var);
	void SendLog();

private:
	queue<TAlarmLogInfo>   m_tAlarmLogList;   //告警日志队列
	queue<TDeviceLogInfo>  m_tDeviceLogList;  //设备日志队列
	queue<TOperateLogInfo> m_tOperateLogList; //操作日志队列
	u32                    m_dwAlarmLogTop;   //告警日志队列Top值
	u32                    m_dwDeviceLogTop;  //设备日志队列Top值
	u32                    m_dwOperateLogTop; //操作日志队列Top值
	string                 m_strUasUri;       //UasUri
	CBatchOperateReq       m_cBatchReq;       //批量日志请求
	u16                    m_wReqSendedTime;  //批量日志请求已发送次数
};

#endif  //#ifndef _UAS_LOG_TASK_H