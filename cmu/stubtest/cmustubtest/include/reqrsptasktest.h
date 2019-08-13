/*========================================================================================
ģ����    ��cmu
�ļ���    ��reqrsptasktest.h
����ļ�  ��
ʵ�ֹ���  ������-Ӧ����������ģ��
����      ��huangzhichun
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/08/10         V1.0              huangzhichun                          �½��ļ�
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

    //״̬����
    u32 OnReq(CMessage *const pcMsg)
    {
       u32 dwRet = PROCMSG_OK;

       if ((u16)m_tReq.GetEventId() == pcMsg->event)
       {
           if (pcMsg->content == NULL)
           {
               TASKLOG(ERROR_LEV, "osp ��ϢΪ��\n");
               dwRet = PROCMSG_DEL;
               return dwRet;
           }

           m_tReq.FromXml((s8*)pcMsg->content);

		   //�����û�req.session
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
			   TASKLOG(ERROR_LEV, "û��cu�û���¼\n");
			   return PROCMSG_DEL;
		   }

		   m_tReq.SetSession(ptCuLoginTask->GetSession());

		   //��������
           PostMsgReq(m_tReq, GetCmuUri());

           //�ȴ�Ӧ��
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

        //SIP�������
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG( ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }


            TASKLOG( ERROR_LEV, "����ʧ��, ����SIP�����sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG( ERROR_LEV, "ospsip ��ϢΪ��\n");
                return dwRet;
            }

            pcOspSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				//�ɹ�
				TASKLOG(CRITICAL_LEV, "�����ɹ�\n");
				return PROCMSG_DEL;
			}
			else
			{
				//ʧ��
				TASKLOG(ERROR_LEV, "����ʧ��\n");
				return PROCMSG_DEL;
			}
        }
        else
        {
            TASKLOG( WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        //�����������ɾ��֮
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //Ӧ��ʱ
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG( WARNING_LEV, "Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
                m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

	
private:
	CReq m_tReq;
};


//ʵ����task��
typedef CReqRspTask<CStartThirdPartyCallReq, CStartThirdPartyCallRsp> CStart3pcTask;
typedef CReqRspTask<CStopThirdPartyCallReq, CStopThirdPartyCallRsp> CStop3pcTask;



#endif  //#ifndef _REQ_RSP_TASK_TEST_H