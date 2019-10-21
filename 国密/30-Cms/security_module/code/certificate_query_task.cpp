#include "certificate_query_task.h"



CCertificateQueryTask::CCertificateQueryTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CCertificateQueryTask::~CCertificateQueryTask()
{

}


void CCertificateQueryTask::InitStateMachine()
{

	CStateProc cState;
	cState.ProcMsg = (CSipTask::PFProcMsg)&CCertificateQueryTask::ProcServerMsg;
	cState.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateQueryTask::ProcServerMsg;
	cState.TimerPoll = &CSipTask::TimerPollSkip;
	AddRuleProc(SERVER, cState);

	NextState(SERVER);
}


u32 CCertificateQueryTask::ProcServerMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "�յ���Ϣ,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "��Ϣ��Ϊ��! ��������\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_DEL;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_QUERY_REQ)
	{
		CSecurityModuleCertificateQueryReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "�յ�REQ��Ϣ,event[%s],IDΪ[%s]\n",cReq.GetEventStr().c_str(), cReq.GetId().c_str());
		
		CSecurityModuleCertificateQueryRsp cRsp;
		
		std::ifstream signatureCertFile((string("certs")+ "/" + cReq.GetId() + "_1").c_str());
		std::ifstream encryptCertFile((string("certs")+ "/" + cReq.GetId() + "_2").c_str()); 
		if (!signatureCertFile || !encryptCertFile)
		{
			cRsp.SetErrorCode(ERR_SECURITY_OPEN_CERTS_FAIL);
			TASKLOG(ERROR_LEV, "��֤���ļ�ʧ��!!!\n");
		}
		else
		{
			std::string strSignatureCert((std::istreambuf_iterator<char>(signatureCertFile)),  
				std::istreambuf_iterator<char>()); 
			signatureCertFile.close();

		 
			std::string strEncryptCert((std::istreambuf_iterator<char>(encryptCertFile)),  
				std::istreambuf_iterator<char>()); 
			encryptCertFile.close();

			if (strEncryptCert.empty() || strSignatureCert.empty())
			{
				cRsp.SetErrorCode(ERR_SECURITY_CERTS_IS_NULL);
				TASKLOG(ERROR_LEV, "֤������Ϊ��!!!\n");
			}
			else
			{
				TASKLOG(EVENT_LEV,"EncryptCert֤������Ϊ��[%s]\n", strEncryptCert.c_str());
				TASKLOG(EVENT_LEV,"strSignatureCert֤������Ϊ��[%s]\n", strSignatureCert.c_str());
				cRsp.SetEncryptCert(strEncryptCert);
				cRsp.SetSignatureCert(strSignatureCert);
				cRsp.SetErrorCode(CMS_SUCCESS);
			}	
		}
			
		cRsp.SetSeqNum(cReq.GetSeqNum());
		cRsp.SetSession(cReq.GetSession());


		if (PROCMSG_OK == PostRsp(KDSIP_EVENT_MESSAGE_RSP,  pcOspSipMsg->GetSipTransID(), cRsp.ToXml()))
		{
			TASKLOG(EVENT_LEV, "������Ϣevent[%s-%d]�ɹ�!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}
		else
		{
			TASKLOG(ERROR_LEV, "������Ϣevent[%s-%d]ʧ��!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}

	}
	else
	{
		TASKLOG(WARNING_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	return dwRet;
}