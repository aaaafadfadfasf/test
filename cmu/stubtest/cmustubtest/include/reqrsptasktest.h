/*========================================================================================
模块名    ：cmu
文件名    ：reqrsptasktest.h
相关文件  ：
实现功能  ：请求-应答类型事务模板
作者      ：huangzhichun
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/08/10         V1.0              huangzhichun                          新建文件
=========================================================================================*/
#ifndef _REQ_RSP_TASK_TEST_H
#define _REQ_RSP_TASK_TEST_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


template<class CReq, class CRsp>
class CReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
	CReqRspTask(CInstExt* pInst) : CSipTask(pInst)
	{
	}
	
	~CReqRspTask()
	{	
	}

public:
	virtual const char* GetObjName() const
	{
		return "CReqRspTask";
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
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CReqRspTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

        NextState(Req);
    }

    //状态处理
    u32 OnReq(CMessage *const pcMsg)
    {
       u32 dwRet = PROCMSG_OK;

       if ((u16)m_tReq.GetEventId() == pcMsg->event)
       {
           if (pcMsg->content == NULL)
           {
               TASKLOG(ERROR_LEV, "osp 消息为空\n");
               dwRet = PROCMSG_DEL;
               return dwRet;
           }

           m_tReq.FromXml((s8*)pcMsg->content);

		   //设置用户req.session
		   CCuLoginTask* ptCuLoginTask = NULL;

		   Iterator pPos;
		   string strSession;
		   CLoginSession* pTask = NULL;
		   while (!pPos.End())
		   {
			   if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
			   {
				   ptCuLoginTask = (CCuLoginTask*)(pTask);
				   break;
			   }
		   }

		   if (ptCuLoginTask == NULL)
		   {
			   TASKLOG(ERROR_LEV, "没有cu用户登录\n");
			   return PROCMSG_DEL;
		   }

		   m_tReq.SetSession(ptCuLoginTask->GetSession());

		   //发送请求
           PostMsgReq(m_tReq, GetCmuUri());

           //等待应答
           NextState(WaitRsp);
       }
       else
       {
           TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
           dwRet = PROCMSG_DEL;
       }

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
            if (pcOspSipMsg == NULL)
            {
                TASKLOG( ERROR_LEV, "ospsip 消息为空\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }


            TASKLOG( ERROR_LEV, "请求失败, 发生SIP层错误，sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG( ERROR_LEV, "ospsip 消息为空\n");
                return dwRet;
            }

            pcOspSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				//成功
				TASKLOG(CRITICAL_LEV, "操作成功\n");
				return PROCMSG_DEL;
			}
			else
			{
				//失败
				TASKLOG(ERROR_LEV, "操作失败\n");
				return PROCMSG_DEL;
			}
        }
        else
        {
            TASKLOG( WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        //事务处理结束，删除之
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //应答超时
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG( WARNING_LEV, "应答超时, Msg[%s-%d]请求失败\n", 
                m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

	
private:
	CReq m_tReq;
};


//实例化task类
typedef CReqRspTask<CStartThirdPartyCallReq, CStartThirdPartyCallRsp> CStart3pcTask;
typedef CReqRspTask<CStopThirdPartyCallReq, CStopThirdPartyCallRsp> CStop3pcTask;



#endif  //#ifndef _REQ_RSP_TASK_TEST_H