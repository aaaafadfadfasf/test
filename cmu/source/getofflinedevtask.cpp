#include "getofflinedevtask.h"
#include "cms/cms_errorcode.h"

CGetOfflineDevTask::CGetOfflineDevTask(CInstExt* pInst) : CSipTask(pInst)
{
	m_tSipTransId = INVALID_TRANS_ID;
	m_dwSendIndex = 0;
	m_dwOnlineVidSrcNum=0;
}

CGetOfflineDevTask::~CGetOfflineDevTask()
{
	ReleaseResource();
}

void CGetOfflineDevTask::ReleaseResource()
{
	//�Ӳ�ѯ�����б���ɾ��
	g_cPuQueryTaskList.Erase(m_tReq.GetQueryTag());

	if (IsLocalDomainByDomainName(m_tReq.GetDomanName()))
	{
		return;
	}

	//���������Ҫɾ����¼����
	CLoginSession* ptCmuTask = NULL;
	g_cCmuList.Find(m_tCmuUri.GetURIString(), ptCmuTask);
	if (ptCmuTask != NULL)
	{
		ptCmuTask->DelTask(this);
	}
}

void CGetOfflineDevTask::InitStateMachine()
{
	CStateProc cReqProc;
	cReqProc.ProcMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnReq;
	cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnReq;
	cReqProc.TimerPoll = (CTask::PFTimerPoll)&CGetOfflineDevTask::OnReqTimer;
	AddRuleProc(Req, cReqProc);

	CStateProc cWaitRspProc;
	cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnWaitRsp;
	cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnWaitRsp;
	cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CGetOfflineDevTask::OnWaitRspTimer;
	AddRuleProc(WaitRsp, cWaitRspProc);

	CStateProc cNtfProc;
	cNtfProc.ProcMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnNtf;
	cNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CGetOfflineDevTask::OnNtf;
	cNtfProc.TimerPoll = (CTask::PFTimerPoll)&CGetOfflineDevTask::OnNtfTimer;
	AddRuleProc(Ntf, cNtfProc);

	NextState(Req);
}


u32 CGetOfflineDevTask::OnReq(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	if ((u16)m_tReq.GetEventId() == pcMsg->event)
	{
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		m_tSipTransId = pcOspSipMsg->GetSipTransID();
		m_tFromUri = pcOspSipMsg->GetSipFromUri();

		COffLineDevQryReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);
		m_tReq = cReq;

        CDomainData *pcDomainData = GetDomainDataByDomainName(cReq.GetDomanName());
		if (NULL == pcDomainData)
		{	
			TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Domain[%s]û�ж�Ӧ��Ϣ, Msg[%s-%d]����ʧ��\n", 
				m_tReq.GetDomanName().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

			SendRsp(ERR_CMU_CMU_INVALID_CMU_URI);
			dwRet = PROCMSG_DEL;
			return dwRet;
		}

        if (pcDomainData->GetPuTotalNum() > s_dwMaxCompatibleNum)
        {
            TASKLOG(CMU_MOD_CMU, WARNING_LEV, "ƽ̨[%s]�����豸������������ֵ[%u]����֧�ָ�����[%s - %d]\n", 
                pcDomainData->GetDomainName().c_str(), s_dwMaxCompatibleNum, 
                m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

            SendRsp(ERR_CMU_UNCOMPATIBLE_REQ);
            dwRet = PROCMSG_DEL;
            return dwRet;
        }

		// ��������Ϣ [2/9/2015 pangubing]
		m_tCmuUri.SetDomain(cReq.GetDomanName());
		m_tCmuUri.SetUser(DEV_TYPE_CMU);

		dwRet=SendReq(cReq, m_tCmuUri);
		g_cPuQueryTaskList.Insert(m_tReq.GetQueryTag(), this);
	}
	else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
	{
		//����cmu��¼�ɹ�, ���Է�������
        if (!IsLocalDomainByDevUri(m_tCmuUri))	//У��һ��Ŀ���豸�Ƿ�������
		{
			SendReq(m_tReq, m_tCmuUri);
		}
		else
		{
			TASKLOG(CMU_MOD_CMU, ERROR_LEV, "������[%s], Receive Msg[%s-%d]\n", 
				m_tReq.GetDomanName().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
	}
	else if (CMU_CMU_CONNECT_FAIL== pcMsg->event )
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Domain[%s]����ʧ��, Msg[%s-%d]����ʧ��\n", 
				m_tReq.GetDomanName().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

		SendRsp(ERR_CMU_SUBDOMAIN_OFFLINE);
		dwRet = PROCMSG_DEL;
	}
	else
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		dwRet = PROCMSG_FAIL;
	}

	return dwRet;
}

u32 CGetOfflineDevTask::OnReqTimer()
{
	//�ö�ʱ���������ʱ���õ�
	if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "��¼����cmu[%s]��ʱ, Msg[%s-%d]����ʧ��\n", 
			m_tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
		SendRsp(ERR_CMU_CMU_NOT_CONN);
		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

u32 CGetOfflineDevTask::OnWaitRsp(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_DEL;

	//SIP�������
	if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
	{
		//����������յ�SIP��Ĵ���Ӧ��
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

		TASKLOG(CMU_MOD_CMU, ERROR_LEV, "CGetOfflineDevTask����ʧ��, ����SIP�����sip status code[%d]\n",pcOspSipMsg->GetSipErrorCode());

		SendRsp((u32)pcOspSipMsg->GetSipErrorCode());          
		return dwRet;
	}

	COffLineDevQryRsp cRsp;
	if ((u16)cRsp.GetEventId() == pcMsg->event)
	{
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		pcOspSipMsg->GetMsgBody(cRsp);
		cRsp.SetSession(m_tReq.GetSession());
		SendRsp(cRsp);

		if (cRsp.GetErrorCode() == CMS_SUCCESS)
		{
			TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "QryTotalNum[%d], Msg[%s-%d]\n", 
				cRsp.GetTotalEntryNum(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

			if (cRsp.GetEntryNum() > 0)
			{
				NextState(Ntf);
				dwRet = PROCMSG_OK;
			}
		}
		else
		{
			TASKLOG(CMU_MOD_CMU, ERROR_LEV, "��ѯʧ�ܣ�errCode[%d], Msg[%s-%d]\n", 
				cRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}

	}
	else
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		dwRet = PROCMSG_FAIL;
	}

	//�����������ɾ��֮
	return dwRet;
}

u32 CGetOfflineDevTask::OnWaitRspTimer()
{
	//Ӧ��ʱ
	if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "CGetOfflineDevTaskӦ��ʱ, Msg[%s-%d]����ʧ��\n", m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
		SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

u32 CGetOfflineDevTask::OnNtf(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	
	if (CMU_PU_OFFLINEDEV_QRY_NTF_REQ == pcMsg->event)
	{
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		COffLineDevQryNtfReq cNtfReq;
		pcOspSipMsg->GetMsgBody(cNtfReq);
		cNtfReq.SetHeadFromReq(m_tReq);
		PostMsgReq(cNtfReq,m_tFromUri);

		//Ӧ��֪ͨ�յ�
		COffLineDevQryNtfRsp cNtfRsp;
		cNtfRsp.SetHeadFromReq(cNtfReq);
		cNtfRsp.SetErrorCode(CMS_SUCCESS);
		PostMsgRsp(pcOspSipMsg->GetSipTransID(), cNtfRsp);

		//֪ͨ������ɾ������
		if (cNtfReq.IsLastNtf())
		{
			TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "��ѯMsg[%s-%d]����ȫ��֪ͨ����\n", m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

			dwRet = PROCMSG_DEL;
		}
	}
	else if (CMU_PU_OFFLINEDEV_QRY_NTF_RSP==pcMsg->event)
	{
        if (IsLocalDomainByDevUri(m_tCmuUri))
		{
			if (m_dwSendIndex < m_tDevOffLinelist.size())
			{
				SendNtfReq();
			}
			else
			{
				TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "��ѯMsg[%s-%d]����ȫ��֪ͨ����\n", m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
				dwRet = PROCMSG_DEL;
			}
		}
	}
	else
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		dwRet = PROCMSG_FAIL;
	}

	NextState(Ntf);

	return dwRet;
}

u32 CGetOfflineDevTask::OnNtfTimer()
{
	//Ӧ��ʱ
	if (GetCurStateHoldTime() > CMS_MSG_TIMEOUT)
	{
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "CGetOfflineDevTask֪ͨ��ʱ, Msg[%s-%d]����ʧ��\n", m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
		SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CGetOfflineDevTask::SendReq(const COffLineDevQryReq& cReq, const TSipURI& tDstUri)
{
	u32 dwRet = PROCMSG_OK;
    if (IsLocalDomainByDevUri(tDstUri))
	{
		if (m_tReq.GetStartEntryIndex() < 0)
		{
			m_tReq.SetStartEntryIndex(0);
		}
		if ((u32)m_tReq.GetExpectEntryNum() > ENTRY_MAX_NUM_ONE_PAGE || m_tReq.GetExpectEntryNum() == 0)
		{
			m_tReq.SetExpectEntryNum(ENTRY_MAX_NUM_ONE_PAGE);
		}

		int nOfflineDevNum=0,nIdleDevNum=0;
        CPuDataList &cPuList = CCmuData::GetInstance()->GetLocalDomainData()->GetPuList();
        TDeviceVideoSrcIdList tDevOffLinelist = cPuList.GetOfflineDevList(nOfflineDevNum, nIdleDevNum);
		m_dwOnlineVidSrcNum = cPuList.GetOnlineVidSrcNum2();
		TASKLOG(CMU_MOD_CMU, WARNING_LEV, "TDeviceVideoSrcIdList size[%d] OnLineSize[%d],offline num[%d],idle num[%d]\n",
			tDevOffLinelist.size(),m_dwOnlineVidSrcNum,nOfflineDevNum,nIdleDevNum);

		u32 dwEndIndex = m_tReq.GetStartEntryIndex()+m_tReq.GetExpectEntryNum();
		if (dwEndIndex > tDevOffLinelist.size())
		{
			dwEndIndex = tDevOffLinelist.size();
		}


		for (u32 i = m_tReq.GetStartEntryIndex(); i < dwEndIndex; ++i)
		{
			m_tDevOffLinelist.push_back(tDevOffLinelist[i]);
		}


		COffLineDevQryRsp cRsp;
		cRsp.SetErrorCode(CMS_SUCCESS);
		cRsp.SetHeadFromReq(m_tReq);
		cRsp.SetEntryNum(m_tDevOffLinelist.size());
		cRsp.SetTotalEntryNum(tDevOffLinelist.size());		
		cRsp.SetQueryTag(m_tReq.GetQueryTag());
		cRsp.SetOnlineDevNum(m_dwOnlineVidSrcNum);
		cRsp.SetOfflineDevNum(nOfflineDevNum);
		cRsp.SetIdleDevNum(nIdleDevNum);
		SendRsp(cRsp);

		if (!m_tDevOffLinelist.empty())
		{
			SendNtfReq();
			NextState(Ntf);
		}
		else
		{
			dwRet = PROCMSG_DEL;
		}
	}
	else
	{
		//���������û�е�½��������Ҫ�ȵ�¼����¼�ɹ����ٷ����������
		CLoginSession* ptCmuTask = NULL;
		g_cCmuList.Find(m_tCmuUri.GetURIString(), ptCmuTask);
        CMS_ASSERT(NULL != ptCmuTask && "ptCmuTask = NULL!!!");

        //cmu���ӳɹ���ŷ�����
        if (ptCmuTask->GetState() == CCmuConnecterTask::Service)
        {
            PostMsgReq(cReq, m_tCmuUri);
            NextState(WaitRsp);
        }

        //����������Ҫ�Ǽǵ�cmu��¼�Ự��
        //���ڳ������������Ϊ�������ʱ�ͷų����������Դ
        //����˲ʱ���������Ϊ��cmu��¼�ɹ���ʱ֪ͨ��������
        //����ɾ��ʱ��������������Ҫȥ����������cmu��¼�Ự�еĵǼ�
        ptCmuTask->AddTask(this);
	}

	return dwRet;
}

u32 CGetOfflineDevTask::SendRsp(u32 dwError)
{
	COffLineDevQryRsp cRsp;
	cRsp.SetSeqNum(m_tReq.GetSeqNum());
	cRsp.SetSession(m_tReq.GetSession());
	cRsp.SetErrorCode(dwError);

	return  PostMsgRsp(m_tSipTransId, cRsp);
}

u32 CGetOfflineDevTask::SendRsp(const COffLineDevQryRsp& cRsp)
{
	return PostMsgRsp(m_tSipTransId, cRsp);
}

u32 CGetOfflineDevTask::SendNtfReq()
{
	static const int nNtfPageSize=60;

	COffLineDevQryNtfReq cNtfReq;

	cNtfReq.SetSession(m_tReq.GetSession());
	cNtfReq.SetSeqNum(m_tReq.GetSeqNum());
	cNtfReq.SetQueryTag(m_tReq.GetQueryTag());

	for (u32 i = m_dwSendIndex; i < m_dwSendIndex+nNtfPageSize && i < m_tDevOffLinelist.size(); ++i)
	{
		cNtfReq.GetDevVideoSrcList().push_back(m_tDevOffLinelist[i]);
	}

	cNtfReq.SetEntryNum(cNtfReq.GetDevVideoSrcList().size());
	cNtfReq.SetLastNtf(m_tDevOffLinelist.size() <= m_dwSendIndex+nNtfPageSize);
	cNtfReq.SetOnlineVideoSrcNum(m_dwOnlineVidSrcNum);

	m_dwSendIndex += nNtfPageSize;

	return PostMsgReq(cNtfReq,m_tFromUri);
}
