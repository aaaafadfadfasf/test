/*========================================================================================
模块名    ：cmu
文件名    ：uaswritelogtask.h
相关文件  ：
实现功能  ：向uas写日志的事务模板，包括设备日志和告警日志
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/11/01         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _UAS_WRITE_LOG_TASK_H
#define _UAS_WRITE_LOG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/uas/uas_proto.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"

template<class CReq, class CRsp>
class CUasWriteLogTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CUasWriteLogTask(CInstExt* pcInst) : CSipTask(pcInst)
    {
        m_nTrySendNum = 0;
    }
    virtual ~CUasWriteLogTask()
    {

    }

public:
    virtual const char* GetObjName() const
    {
        return "CUasWriteLogTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine()
    {
        CStateProc cWaitProc;
        cWaitProc.ProcMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnWait;
        cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnWait;
        cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CUasWriteLogTask::OnWaitTimer;
        AddRuleProc(Wait, cWaitProc);

        CStateProc cRspProc;
        cRspProc.ProcMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnRsp;
        cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnRsp;
        cRspProc.TimerPoll = (CTask::PFTimerPoll)&CUasWriteLogTask::OnRspTimer;
        AddRuleProc(Rsp, cRspProc);

        NextState(Wait);
    }
    virtual void PrintData() const
    {
        CTask::PrintData();

        //直接打印xml数据
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

public:
    //状态处理
    u32 OnWait(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        TASKLOG(CMU_MOD_UAS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        return dwRet;
    }
    u32 OnWaitTimer()
    {
        return TIMERPOLL_DONE;
    }

    u32 OnRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_FAIL;
        if ((u16)m_tRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(m_tRsp);

            if (m_tRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CMU_MOD_UAS, EVENT_LEV, "向uas写日志成功 Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_UAS, EVENT_LEV, "向uas写日志失败，errCode[%d], Msg[%s-%d]\n", 
                    m_tRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            
            dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(CMU_MOD_UAS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return dwRet;
    }
    u32 OnRspTimer()
    {
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            if (m_nTrySendNum >= NTF_TRY_SEND_NUM)
            {
                //尝试三次后还失败，直接删除通知事务
                TASKLOG(CMU_MOD_UAS, ERROR_LEV, "向uas写日志[%d]次都失败，直接删除该事务!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_UAS, ERROR_LEV, "向uas写日志超时，重试...!\n");
            StartWrite();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }


public:
    //业务处理 
    void StartWrite()
    {
        NextState(Rsp);
        PostMsgReq(m_tReq, m_tDstUri);

        m_nTrySendNum++;
    }

public:

    //数据获取   
    const CReq& GetReq() const
    {
        return m_tReq;
    }
    CReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CReq& tReq)
    {
        m_tReq = tReq;
    }

    const TSipURI& GetDstUri() const
    {
        return m_tDstUri;
    }
    TSipURI& GetDstUri()
    {
        return m_tDstUri;
    }
    void SetDstUri(const TSipURI& tUri)
    {
        m_tDstUri = tUri;
    }

private:
    CReq m_tReq;
    CRsp m_tRsp;
    TSipURI m_tDstUri;
    s32 m_nTrySendNum;
};

//写设备日志
typedef CUasWriteLogTask<CDeviceLogAddReq, CDeviceLogAddRsp> CUasWriteDevLogTask;

//写告警日志
typedef CUasWriteLogTask<CAlarmLogAddReq, CAlarmLogAddRsp> CUasWriteAlarmLogTask;

//写用户操作日志
typedef CUasWriteLogTask<COperateLogAddReq, COperateLogAddRsp> CUasWriteUserOptLogTask;

#endif  //#ifndef _UAS_WRITE_LOG_TASK_H