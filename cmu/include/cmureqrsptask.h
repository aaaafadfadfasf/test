/*========================================================================================
模块名    ：cmu
文件名    ：cmureqrsptask.h
相关文件  ：
实现功能  ：cmu向其他模块发送请求-应答类型事务模板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2013/05/13         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _CMU_REQ_RSP_TASK_H
#define _CMU_REQ_RSP_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"

template<class CReq, class CRsp>
class CCmuReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
	CCmuReqRspTask(CInstExt* pInst) : CSipTask(pInst)
	{
        m_nTrySendNum = 0;
	}
	
	~CCmuReqRspTask()
	{	
	}

public:
	virtual const char* GetObjName() const
	{
		return "CCmuReqRspTask";
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine()
    {
        CStateProc cReqProc;
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CCmuReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CCmuReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CCmuReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CCmuReqRspTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

        NextState(Req);
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        //直接打印xml数据
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

    //状态处理
    u32 OnReq(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        TASKLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        return dwRet;
    }

    u32 OnReqTimer()
    {      
        return TIMERPOLL_DONE;
    }

    u32 OnWaitRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        //SIP层错误处理
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_INS, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误，sip status code[%d]\n",
                m_tDstUri.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());

            return dwRet;
        }

        if ((u16)m_tRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(m_tRsp);

            if (m_tRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CMU_MOD_INS, EVENT_LEV, "操作设备[%s]成功 Msg[%s-%d]\n", 
                    m_tDstUri.GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_INS, ERROR_LEV, "操作设备[%s]失败，errCode[%d], Msg[%s-%d]\n", 
                    m_tDstUri.GetURIString().c_str(), m_tRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }

            dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(CMU_MOD_INS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            if (m_nTrySendNum >= NTF_TRY_SEND_NUM)
            {
                //尝试三次后还失败，直接删除通知事务
                TASKLOG(CMU_MOD_INS, ERROR_LEV, "操作[%d]次都失败，直接删除该事务!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_INS, WARNING_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
                m_tDstUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

            StartReq();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }

    //业务处理 
    void StartReq()
    {
        PostMsgReq(m_tReq, m_tDstUri);
        NextState(WaitRsp);
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


//stun地址更新通知
typedef CCmuReqRspTask<CStunUpdateReq, CStunUpdateRsp> CStunUpdateTask;

//cmu断链通知
typedef CCmuReqRspTask<CCmuDisconnectNtfReq, CCmuDisconnectNtfRsp> CCmuDisconnectNtfTask;

//{{{{{{{{{{{{{{{ add by Royan 20140509 智能告警主机
//异域告警
typedef CCmuReqRspTask<CDomainAlarmReq, CDomainAlarmRsp> CCmuDomainAlarmTask;
//}}}}}}}}}}}}}}} add by Royan 20140509 智能告警主机

#endif  //#ifndef _CMU_REQ_RSP_TASK_H