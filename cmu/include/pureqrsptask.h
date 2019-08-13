/*========================================================================================
ģ����    ��cmu
�ļ���    ��pureqrsptask.h
����ļ�  ��
ʵ�ֹ���  ��pu����-Ӧ����������ģ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/
#ifndef _PU_REQ_RSP_TASK_H
#define _PU_REQ_RSP_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "puregtask.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"

template<class CReq, class CRsp>
class CPuReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
	CPuReqRspTask(CInstExt* pInst) : CSipTask(pInst)
	{
        m_tSipTransId = INVALID_TRANS_ID;
	}
	
	~CPuReqRspTask()
	{	
        ReleaseResource();
	}

    void ReleaseResource()
    {
        if (IsLocalDomainByDevUri(m_tReq.GetDevUri()))
        {
            return;
        }

        //���������Ҫɾ����¼����
        TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
        CLoginSession* ptCmuTask = NULL;
        g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
        if (ptCmuTask != NULL)
        {
            ptCmuTask->DelTask(this);
        }
    }

public:
	virtual const char* GetObjName() const
	{
		return "CPuReqRspTask";
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
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask::OnWaitRspTimer;
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
       u32 dwRet = PROCMSG_OK;
       if ((u16)m_tReq.GetEventId() == pcMsg->event)
       {
           COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
           m_tSipTransId = pcOspSipMsg->GetSipTransID();

           CReq cReq;
           pcOspSipMsg->GetMsgBody(cReq);
           m_tReq = cReq;

           //ȷ��pu·��,ת����Ϣ
           TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevUri());
           if (tPuRouterUri.IsNull())
           {
               TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]�޷�·��, Msg[%s-%d]����ʧ��\n", 
                   m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

               SendRsp(ERR_CMU_DEV_NO_ROUTER);
               dwRet = PROCMSG_DEL;
               return dwRet;
           }

           if (IsLocalDomainByDevUri(tPuRouterUri))
           {
               TSipURI tDevUri;
               tDevUri.SetURIString(cReq.GetDevUri());
               CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(tDevUri.GetUser());
               if (NULL == pcPuData)
               {
                   TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]δ����, Msg[%s-%d]����ʧ��\n", 
                       cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_PU_NOT_IN_TOP);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }

               CPuRegBaseTask *ptPuRegTask = pcPuData->GetPuRegTask();
               if (ptPuRegTask == NULL)
               {
                   TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]������, Msg[%s-%d]����ʧ��\n", 
                       cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_PU_NOT_ONLINE);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }

               //����pu����Ϣ��Ҫ��session��дpu�ĵ�¼session
               cReq.SetSession(ptPuRegTask->GetSession());
           }
           else
           {
               if (!IsDomainInTopByDevUri(tPuRouterUri))
               {
                   TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]������δ����, Msg[%s-%d]����ʧ��\n", 
                       m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }   
           }

           SendReq(cReq, tPuRouterUri);
       }
       else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
       {
           //����cmu��¼�ɹ�, ���Է�������
           TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
           if (!IsLocalDomainByDevUri(tPuRouterUri))
           {
               SendReq(m_tReq, tPuRouterUri);
           }
           else
           {
               TASKLOG(CMU_MOD_PU, ERROR_LEV, "�������豸[%s], Receive Msg[%s-%d]\n", 
                   m_tReq.GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
           }
       }
       else
       {
           TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
           dwRet = PROCMSG_FAIL;
       }

       return dwRet;
    }

    u32 OnReqTimer()
    {
        //�ö�ʱ���������ʱ���õ�
        if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
        {
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "��¼����cmu[%s]��ʱ, Msg[%s-%d]����ʧ��\n", 
                tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_CMU_NOT_CONN);
            return TIMERPOLL_DEL;
        }

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
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }


            TASKLOG(CMU_MOD_PU, ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����sip status code[%d]\n",
                m_tReq.GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            SendRsp((u32)pcOspSipMsg->GetSipErrorCode());
            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                SendRsp(ERR_CMU_SIP_BODY_NULL);
                return dwRet;
            }

            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
                m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    void SendReq(const CReq& cReq, const TSipURI& tDstUri)
    {
        if (IsLocalDomainByDevUri(tDstUri))
//        if (IsLocalDomain(tDstUri))
        {
            //����ֱ��Ͷ��
            PostMsgReq(cReq, tDstUri);
            NextState(WaitRsp);
        }
        else
        {
            //���������û�е�½��������Ҫ�ȵ�¼����¼�ɹ����ٷ����������
            CCmuConnecterTask *pcCmuTask = GetConnectTaskByDomainName(tDstUri.GetDomain());
            CMS_ASSERT(NULL != pcCmuTask && "pcCmuTask = NULL!!!");
            //cmu���ӳɹ���ŷ�����
            if (pcCmuTask->GetState() == CCmuConnecterTask::Service)
            {
                PostMsgReq(cReq, GetCmuUriFromDomainName(tDstUri.GetDomain()));
                NextState(WaitRsp);
            }

            //����������Ҫ�Ǽǵ�cmu��¼�Ự��
            //���ڳ������������Ϊ�������ʱ�ͷų����������Դ
            //����˲ʱ���������Ϊ��cmu��¼�ɹ���ʱ֪ͨ��������
            //����ɾ��ʱ��������������Ҫȥ����������cmu��¼�Ự�еĵǼ�
            pcCmuTask->AddTask(this);
        }
    }

    bool SendRsp(u32 dwError)
    {
        CRsp cRsp;
        cRsp.SetSeqNum(m_tReq.GetSeqNum());
        cRsp.SetSession(m_tReq.GetSession());
        cRsp.SetErrorCode(dwError);

        if (m_tSipTransId != INVALID_TRANS_ID)
        {
            return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
        }
        else
        {
            return true;
        }   
    }

    bool SendRsp(const CRsp& cRsp)
    {
        if (m_tSipTransId != INVALID_TRANS_ID)
        {
            return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
        }
        else
        {
            return true;
        }
    }

public: 
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }

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
	
private:
    TKDSIP_TRANS_ID m_tSipTransId;
	CReq m_tReq;
};

template<class CReq, class CRsp>
class CPuReqRspTask2 : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE+1,
		WaitRsp,
	};

public:
	CPuReqRspTask2(CInstExt* pInst) : CSipTask(pInst)
	{
		m_tSipTransId = INVALID_TRANS_ID;
	}

	~CPuReqRspTask2()
	{	
		ReleaseResource();
	}

	void ReleaseResource()
	{
		if (IsLocalDomainByDevUri(m_tReq.GetDevUri()))
		{
			return;
		}

		//���������Ҫɾ����¼����
		TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
		CLoginSession* ptCmuTask = NULL;
		g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
		if (ptCmuTask != NULL)
		{
			ptCmuTask->DelTask(this);
		}
	}

public:
	virtual const char* GetObjName() const
	{
		return "CPuReqRspTask2";
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
		cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnReq;
		cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnReq;
		cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask2::OnReqTimer;
		AddRuleProc(Req, cReqProc);

		CStateProc cWaitRspProc;
		cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnWaitRsp;
		cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnWaitRsp;
		cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask2::OnWaitRspTimer;
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
		u32 dwRet = PROCMSG_OK;
		if ((u16)m_tReq.GetEventId() == pcMsg->event)
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			m_tSipTransId = pcOspSipMsg->GetSipTransID();

			CReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tReq = cReq;

			//ȷ��pu·��,ת����Ϣ
			TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevUri());
			if (tPuRouterUri.IsNull())
			{
				TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]�޷�·��, Msg[%s-%d]����ʧ��\n", 
					m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

				SendRsp(ERR_CMU_DEV_NO_ROUTER);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			if (IsLocalDomainByDevUri(tPuRouterUri))
			{
				TSipURI tDevUri;
				tDevUri.SetURIString(cReq.GetDevUri());
				CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(tDevUri.GetUser());
				if (NULL == pcPuData)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]δ����, Msg[%s-%d]����ʧ��\n", 
						cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_PU_NOT_IN_TOP);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}

				CPuRegBaseTask *ptPuRegTask = pcPuData->GetPuRegTask();
				if (ptPuRegTask == NULL)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]������, Msg[%s-%d]����ʧ��\n", 
						cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_PU_NOT_ONLINE);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}

				//����pu����Ϣ��Ҫ��session��дpu�ĵ�¼session
				cReq.SetSession(ptPuRegTask->GetSession());
			}
			else
			{
				if (!IsDomainInTopByDevUri(tPuRouterUri))
				{
					TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]������δ����, Msg[%s-%d]����ʧ��\n", 
						m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}   
			}

			SendReq(cReq, tPuRouterUri);
		}
		else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
		{
			//����cmu��¼�ɹ�, ���Է�������
			TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
			if (!IsLocalDomainByDevUri(tPuRouterUri))
			{
				SendReq(m_tReq, tPuRouterUri);
			}
			else
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "�������豸[%s], Receive Msg[%s-%d]\n", 
					m_tReq.GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		else
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_FAIL;
		}

		return dwRet;
	}

	u32 OnReqTimer()
	{
		//�ö�ʱ���������ʱ���õ�
		if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
		{
			TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "��¼����cmu[%s]��ʱ, Msg[%s-%d]����ʧ��\n", 
				tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
			SendRsp(ERR_CMU_CMU_NOT_CONN);
			return TIMERPOLL_DEL;
		}

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
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
				dwRet = PROCMSG_FAIL;
				return dwRet;
			}


			TASKLOG(CMU_MOD_PU, ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����sip status code[%d]\n",
				m_tReq.GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

			SendRsp((u32)pcOspSipMsg->GetSipErrorCode());
			return dwRet;
		}

		CRsp cRsp;
		if ((u16)cRsp.GetEventId() == pcMsg->event)
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
				SendRsp(ERR_CMU_SIP_BODY_NULL);
				return dwRet;
			}

			pcOspSipMsg->GetMsgBody(cRsp);
			cRsp.SetSession(m_tReq.GetSession());
			SendRsp(cRsp);
		}
		else
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
				m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
			SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
			return TIMERPOLL_DEL;
		}

		return TIMERPOLL_DONE;
	}

	void SendReq(const CReq& cReq, const TSipURI& tDstUri)
	{
		if (IsLocalDomainByDevUri(tDstUri))
			//        if (IsLocalDomain(tDstUri))
		{
			//����ֱ��Ͷ��
			PostMsgReq(cReq, tDstUri);
			NextState(WaitRsp);
		}
		else
		{
			//���������û�е�½��������Ҫ�ȵ�¼����¼�ɹ����ٷ����������
			CCmuConnecterTask *pcCmuTask = GetConnectTaskByDomainName(tDstUri.GetDomain());
			CMS_ASSERT(NULL != pcCmuTask && "pcCmuTask = NULL!!!");
			//cmu���ӳɹ���ŷ�����
			if (pcCmuTask->GetState() == CCmuConnecterTask::Service)
			{
				PostMsgReq(cReq, GetCmuUriFromDomainName(tDstUri.GetDomain()));
				NextState(WaitRsp);
			}

			//����������Ҫ�Ǽǵ�cmu��¼�Ự��
			//���ڳ������������Ϊ�������ʱ�ͷų����������Դ
			//����˲ʱ���������Ϊ��cmu��¼�ɹ���ʱ֪ͨ��������
			//����ɾ��ʱ��������������Ҫȥ����������cmu��¼�Ự�еĵǼ�
			pcCmuTask->AddTask(this);
		}
	}

	bool SendRsp(u32 dwError)
	{
		CRsp cRsp;
		cRsp.SetSeqNum(m_tReq.GetSeqNum());
		cRsp.SetSession(m_tReq.GetSession());
		cRsp.SetErrorCode(dwError);

		if (m_tSipTransId != INVALID_TRANS_ID)
		{
			return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
		}
		else
		{
			return true;
		}   
	}

	bool SendRsp(const CRsp& cRsp)
	{
		if (m_tSipTransId != INVALID_TRANS_ID)
		{
			return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
		}
		else
		{
			return true;
		}
	}

public: 
	const TKDSIP_TRANS_ID GetSipTransID() const
	{
		return m_tSipTransId;
	}
	void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
	{
		m_tSipTransId = tSipTransID;
	}

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

private:
	TKDSIP_TRANS_ID m_tSipTransId;
	CReq m_tReq;
};


//puϵͳ������ȡ������
typedef CPuReqRspTask<CPuSysParamGetReq, CPuSysParamGetRsp> CGetPuSysParamTask;
typedef CPuReqRspTask<CPuSysParamSetReq, CPuSysParamSetRsp> CSetPuSysParamTask;

//����pu����
typedef CPuReqRspTask<CPuSaveCfgReq, CPuSaveCfgRsp> CSavePuCfgTask;

//pu��Ƶ���������ȡ������
typedef CPuReqRspTask<CPuVidEncParamGetReq, CPuVidEncParamGetRsp> CGetPuVidEncParamTask;
typedef CPuReqRspTask<CPuVidEncParamSetReq, CPuVidEncParamSetRsp> CSetPuVidEncParamTask;

//pu�澯���
typedef CPuReqRspTask<CPuIoOutReq, CPuIoOutRsp> CPuIoOutTask;

//pu�澯����
typedef CPuReqRspTask<CPuReadSecReq, CPuReadSecRsp> CPuReadSecTask;

//pu�澯��λ
typedef CPuReqRspTask2<CPuAlarmResetReq, CPuAlarmResetRsp> CPuAlarmResetTask;

//����ͷ����
typedef CPuReqRspTask<CPuCameraTypeSetReq, CPuCameraTypeSetRsp> CSetPuCameraTypeTask;

//�ƶ����
typedef CPuReqRspTask<CPuMotionDetectSetReq, CPuMotionDetectSetRsp> CSetPuMotionDetectTask;
typedef CPuReqRspTask<CPuMotionDetectGetReq, CPuMotionDetectGetRsp> CGetPuMotionDetectTask;

//ͼ���ڱ�
typedef CPuReqRspTask<CPuAreaShadeSetReq, CPuAreaShadeSetRsp> CSetPuAreaShadeTask;
typedef CPuReqRspTask<CPuAreaShadeGetReq, CPuAreaShadeGetRsp> CGetPuAreaShadeTask;

//������������á���ȡ
typedef CPuReqRspTask<CPuDecStyleSetReq, CPuDecStyleSetRsp> CSetPuDecStyleTask;
typedef CPuReqRspTask<CPuDecStyleGetReq, CPuDecStyleGetRsp> CGetPuDecStyleTask;

//��ƵԴ��Ļ���á���ȡ
typedef CPuReqRspTask<CPuVidSrcOsdSetReq, CPuVidSrcOsdSetRsp> CSetPuVidSrcOsdTask;
typedef CPuReqRspTask<CPuVidSrcOsdGetReq, CPuVidSrcOsdGetRsp> CGetPuVidSrcOsdTask;

//ǰ��VSIP���ݲ���
typedef CPuReqRspTask<CPuVsipOperateReq, CPuVsipOperateRsp> CPuVsipOperateTask;

//ǰ��͸�����ݲ���
typedef CPuReqRspTask<CPuTransparentOperateReq, CPuTransparentOperateRsp> CPuTransparentOperateTask;

//����ؼ�֡
typedef CPuReqRspTask<CKeyFrameReq, CKeyFrameRsp> CKeyFrameTask;

//ǰ��ͼƬץ��
typedef CPuReqRspTask<CPuPicSnapshotReq, CPuPicSnapshotRsp> CPuPicSnapshotTask;

//ǰ��ͼƬ����
typedef CPuReqRspTask<CPuPicDownloadReq, CPuPicDownloadRsp> CPuPicDownloadTask;

//ǰ��¼��������ѯ
typedef CPuReqRspTask<CPuRecDaysQryReq, CPuRecDaysQryRsp> CPuRecDaysQryTask;

//GPS��������
typedef CPuReqRspTask<CPuGpsCfgSetReq, CPuGpsCfgSetRsp> CPuGpsCfgSetTask;
typedef CPuReqRspTask<CPuGpsCfgGetReq, CPuGpsCfgGetRsp> CPuGpsCfgGetTask;

//GPS��������
typedef CPuReqRspTask<CPuGpsSsSetReq, CPuGpsSsSetRsp> CPuGpsSsSetTask;
typedef CPuReqRspTask<CPuGpsSsGetReq, CPuGpsSsGetRsp> CPuGpsSsGetTask;

//ǰ��¼��
typedef CPuReqRspTask<CPuRecordStartReq, CPuRecordStartRsp> CPuRecordStartTask;
typedef CPuReqRspTask<CPuRecordStopReq, CPuRecordStopRsp> CPuRecordStopTask;
typedef CPuReqRspTask<CPuRecordDelReq, CPuRecordDelRsp> CPuRecordDelTask;

//͸���������õ����á���ȡ
typedef CPuReqRspTask<CPuTransdataCfgSetReq, CPuTransdataCfgSetRsp> CPuTransdataCfgSetTask;
typedef CPuReqRspTask<CPuTransdataCfgGetReq, CPuTransdataCfgGetRsp> CPuTransdataCfgGetTask;

//����̨��
typedef CPuReqRspTask<CPuLogoBmpOsdTransmitReq, CPuLogoBmpOsdTransmitRsp> CPuLogoBmpOsdTransmitTask;

//����Ļ����
typedef CPuReqRspTask<CPuLongOsdSetReq, CPuLongOsdSetRsp> CPuLongOsdSetTask;
typedef CPuReqRspTask<CPuLongOsdGetReq, CPuLongOsdGetRsp> CPuLongOsdGetTask;

//{{{{{{{{{{{{{{{ add by Royan 20140418 ������Ļ
//������Ļ����
typedef CPuReqRspTask<CPuMultiOsdSetReq, CPuMultiOsdSetRsp> CPuMultiOsdSetTask;
typedef CPuReqRspTask<CPuMultiOsdGetReq, CPuMultiOsdGetRsp> CPuMultiOsdGetTask;
//}}}}}}}}}}}}}}} add by Royan 20140418 ������Ļ

//��Ƶ��������
typedef CPuReqRspTask<CPuAudEncParamSetReq, CPuAudEncParamSetRsp> CPuAudEncParamSetTask;
typedef CPuReqRspTask<CPuAudEncParamGetReq, CPuAudEncParamGetRsp> CPuAudEncParamGetTask;



//ǰ��ץ�Ĳ�������
typedef CPuReqRspTask<CPuSnapShotCfgGetReq, CPuSnapShotCfgGetRsp> CPuSnapShotCfgGetTask;
typedef CPuReqRspTask<CPuSnapShotCfgSetReq, CPuSnapShotCfgSetRsp> CPuSnapShotCfgSetTask;

//���ڲ�������
typedef CPuReqRspTask<CPuSerialParamCfgGetReq, CPuSerialParamCfgGetRsp> CPuSerialParamCfgGetTask;
typedef CPuReqRspTask<CPuSerialParamCfgSetReq, CPuSerialParamCfgSetRsp> CPuSerialParamCfgSetTask;



#endif  //#ifndef _PU_REQ_RSP_TASK_H