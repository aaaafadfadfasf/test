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
	TASKLOG(EVENT_LEV, "收到消息,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "消息体为空! 销毁事务\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_GET_REQ)
	{
		CSecurityModuleCertificateGetReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "收到REQ消息,event[%s]\n", cReq.GetEventStr().c_str());

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();
		/*1.根据id从数据库取出证书所有数据
		   2.使用usbkey中的对称密钥进行sm4解密
		   3.对解密的数据进行sm3杂凑
		   4.对比杂凑值和数据库中的杂凑值是否一致
		   一致则返回解密值，否则返回错误代码*/

		
	}
	else
	{
		TASKLOG(WARNING_LEV, "收到未知消息[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	NextState(WaitResult);
	return dwRet;
}

u32 CCertificateGetTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "插入数据库超时,销毁事务!\n");

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
			TASKLOG(ERROR_LEV, "操作数据表[VKEK_INFO_TABLE], db operate fail, db-errCode[%d]!\n",
				pDbMsg->GetDBResult());
			dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
		}
		else if (/*查询证书为空*/)
		{
			dwErrorCode = ERR_SECURITY_GET_CRT_EMPTY;
			TASKLOG(ERROR_LEV, "查询数据库证书为空!\n");
		}
		else
		{
			TASKLOG(EVENT_LEV, "操作数据表[VKEK_INFO_TABLE], db operate success!\n");

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
		TASKLOG(ERROR_LEV, "等待数据库处理超时，销毁事务!\n");
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
		//发起的请求收到SIP层的错误应答
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		if (NULL == pcOspSipMsg)
		{
			TASKLOG(ERROR_LEV, "消息体为NULL!\n");
		}

		TASKLOG(ERROR_LEV, "发生SIP层错误，sip status code[%d]\n"
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
		TASKLOG(ERROR_LEV, "发送应答失败!\n");
	}
	else
	{
		TASKLOG(ERROR_LEV, "发送应答成功!\n");
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