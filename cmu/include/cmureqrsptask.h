/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmureqrsptask.h
����ļ�  ��
ʵ�ֹ���  ��cmu������ģ�鷢������-Ӧ����������ģ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2013/05/13         V1.0              fanxg                                  �½��ļ�
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

        //ֱ�Ӵ�ӡxml����
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

    //״̬����
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

        //SIP�������
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_INS, ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����sip status code[%d]\n",
                m_tDstUri.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());

            return dwRet;
        }

        if ((u16)m_tRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(m_tRsp);

            if (m_tRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CMU_MOD_INS, EVENT_LEV, "�����豸[%s]�ɹ� Msg[%s-%d]\n", 
                    m_tDstUri.GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_INS, ERROR_LEV, "�����豸[%s]ʧ�ܣ�errCode[%d], Msg[%s-%d]\n", 
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
                //�������κ�ʧ�ܣ�ֱ��ɾ��֪ͨ����
                TASKLOG(CMU_MOD_INS, ERROR_LEV, "����[%d]�ζ�ʧ�ܣ�ֱ��ɾ��������!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_INS, WARNING_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
                m_tDstUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

            StartReq();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }

    //ҵ���� 
    void StartReq()
    {
        PostMsgReq(m_tReq, m_tDstUri);
        NextState(WaitRsp);
        m_nTrySendNum++;
    }

    public:
        //���ݻ�ȡ   
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


//stun��ַ����֪ͨ
typedef CCmuReqRspTask<CStunUpdateReq, CStunUpdateRsp> CStunUpdateTask;

//cmu����֪ͨ
typedef CCmuReqRspTask<CCmuDisconnectNtfReq, CCmuDisconnectNtfRsp> CCmuDisconnectNtfTask;

//{{{{{{{{{{{{{{{ add by Royan 20140509 ���ܸ澯����
//����澯
typedef CCmuReqRspTask<CDomainAlarmReq, CDomainAlarmRsp> CCmuDomainAlarmTask;
//}}}}}}}}}}}}}}} add by Royan 20140509 ���ܸ澯����

#endif  //#ifndef _CMU_REQ_RSP_TASK_H