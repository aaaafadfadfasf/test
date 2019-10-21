#include "certificate_get_task.h"

CCertificateGetTask::CCertificateGetTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CCertificateGetTask::~CCertificateGetTask()
{

}


void CCertificateGetTask::InitStateMachine()
{

	CStateProc cReqProc;
	cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcReqMsg;
	cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcErrMsg;
	cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateGetTask::ProcReqMsgTimerPoll;
	AddRuleProc(REQ, cReqProc);


	CStateProc cResultProc;
	cResultProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcWaitResult;
	cResultProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcErrMsg;
	cResultProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateGetTask::ProcWaitResultTimerPoll;
	AddRuleProc(WaitResult, cResultProc);

	NextState(REQ);
}

u32 CCertificateGetTask::ProcReqMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "�յ���Ϣ,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "��Ϣ��Ϊ��! ��������\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_GET_REQ)
	{
		CSecurityModuleCertificateGetReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "�յ�REQ��Ϣ,event[%s]\n", cReq.GetEventStr().c_str());

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();
		/*1.����id�����ݿ�ȡ��֤����������
		   2.ʹ��usbkey�еĶԳ���Կ����sm4����
		   3.�Խ��ܵ����ݽ���sm3�Ӵ�
		   4.�Ա��Ӵ�ֵ�����ݿ��е��Ӵ�ֵ�Ƿ�һ��
		   һ���򷵻ؽ���ֵ�����򷵻ش������*/

		
	}
	else
	{
		TASKLOG(WARNING_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	NextState(WaitResult);
	return dwRet;
}

u32 CCertificateGetTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�������ݿⳬʱ,��������!\n");

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

u32 CCertificateGetTask::ProcWaitResult(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	switch (pcMsg->event)
	{
	case DBIX_RESULT_CB:
	{
		CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;
		dwRet = PROCMSG_DEL;

		u32 dwErrorCode = CMS_SUCCESS;

		if (pDbMsg->GetDBResult() != enDBResultSuccess)
		{
			TASKLOG(ERROR_LEV, "�������ݱ�[VKEK_INFO_TABLE], db operate fail, db-errCode[%d]!\n",
				pDbMsg->GetDBResult());
			dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
		}
		else if (/*��ѯ֤��Ϊ��*/)
		{
			dwErrorCode = ERR_SECURITY_GET_CRT_EMPTY;
			TASKLOG(ERROR_LEV, "��ѯ���ݿ�֤��Ϊ��!\n");
		}
		else
		{
			TASKLOG(EVENT_LEV, "�������ݱ�[VKEK_INFO_TABLE], db operate success!\n");

		}

		dwRet = PostRspToClient(dwErrorCode);
	}
	break;
	default:
	{
		TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	break;
	}

	return dwRet;
}

u32 CCertificateGetTask::ProcWaitResultTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�ȴ����ݿ⴦��ʱ����������!\n");
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCertificateGetTask::ProcErrMsg(CMessage *const pcMsg)
{
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	{
		//����������յ�SIP��Ĵ���Ӧ��
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		if (NULL == pcOspSipMsg)
		{
			TASKLOG(ERROR_LEV, "��Ϣ��ΪNULL!\n");
		}

		TASKLOG(ERROR_LEV, "����SIP�����sip status code[%d]\n"
			, pcOspSipMsg->GetSipErrorCode());
	}
	break;
	default:
	{
		TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n",
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	break;
	}
	return PROCMSG_FAIL;
}

u32 CCertificateGetTask::PostRspToClient(u32 dwErrorCode)
{
	CSecurityModuleCertificateGetRsp cRsp;
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(dwErrorCode);
	if (!m_strCertContent.empty())
	{
		cRsp.SetCertContent(m_strCertContent);
	}
	if (PROCMSG_OK != PostMsgRsp(m_tTransId, cRsp))
	{
		TASKLOG(ERROR_LEV, "����Ӧ��ʧ��!\n");
	}
	else
	{
		TASKLOG(ERROR_LEV, "����Ӧ��ɹ�!\n");
	}

	return PROCMSG_DEL;
}

bool CCertificateGetTask::GetDbCertInfo(const string& strOwnerid)
{
	return true;
}
void CCertificateGetTask::GetUsbKeySm4Passwd(string& strUsbKeyPasswd)
{

}
void CCertificateGetTask::DecryptCertInfoBySm4(const string& strUsbKeyPasswd)
{

}
bool CCertificateGetTask::CompareCertHashAndSrcHash()
{
	return true;
}