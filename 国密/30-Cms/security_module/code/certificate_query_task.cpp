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
	TASKLOG(EVENT_LEV, "收到消息,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "消息体为空! 销毁事务\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_DEL;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_QUERY_REQ)
	{
		CSecurityModuleCertificateQueryReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "收到REQ消息,event[%s],ID为[%s]\n",cReq.GetEventStr().c_str(), cReq.GetId().c_str());
		
		CSecurityModuleCertificateQueryRsp cRsp;
		
		std::ifstream signatureCertFile((string("certs")+ "/" + cReq.GetId() + "_1").c_str());
		std::ifstream encryptCertFile((string("certs")+ "/" + cReq.GetId() + "_2").c_str()); 
		if (!signatureCertFile || !encryptCertFile)
		{
			cRsp.SetErrorCode(ERR_SECURITY_OPEN_CERTS_FAIL);
			TASKLOG(ERROR_LEV, "打开证书文件失败!!!\n");
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
				TASKLOG(ERROR_LEV, "证书内容为空!!!\n");
			}
			else
			{
				TASKLOG(EVENT_LEV,"EncryptCert证书内容为：[%s]\n", strEncryptCert.c_str());
				TASKLOG(EVENT_LEV,"strSignatureCert证书内容为：[%s]\n", strSignatureCert.c_str());
				cRsp.SetEncryptCert(strEncryptCert);
				cRsp.SetSignatureCert(strSignatureCert);
				cRsp.SetErrorCode(CMS_SUCCESS);
			}	
		}
			
		cRsp.SetSeqNum(cReq.GetSeqNum());
		cRsp.SetSession(cReq.GetSession());


		if (PROCMSG_OK == PostRsp(KDSIP_EVENT_MESSAGE_RSP,  pcOspSipMsg->GetSipTransID(), cRsp.ToXml()))
		{
			TASKLOG(EVENT_LEV, "发送消息event[%s-%d]成功!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}
		else
		{
			TASKLOG(ERROR_LEV, "发送消息event[%s-%d]失败!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}

	}
	else
	{
		TASKLOG(WARNING_LEV, "收到未知消息[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	return dwRet;
}