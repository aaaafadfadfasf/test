#include "nvkek_query_task.h"



CNVkekQueryTask::CNVkekQueryTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CNVkekQueryTask::~CNVkekQueryTask()
{

}


void CNVkekQueryTask::InitStateMachine()
{

	CStateProc cHandleQryProc;
	cHandleQryProc.ProcMsg = (CSipTask::PFProcMsg)&CNVkekQueryTask::HandleQueryProcMsg;
	cHandleQryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CNVkekQueryTask::ProcErrMsg;
	cHandleQryProc.TimerPoll = (CSipTask::PFTimerPoll)&CNVkekQueryTask::WaitHandleQueryTimerPoll;
	AddRuleProc(REQ, cHandleQryProc);

	CStateProc cWaitDBIXResultProc;
	cWaitDBIXResultProc.ProcMsg = (CSipTask::PFProcMsg)&CNVkekQueryTask::WaitDBIXResultProcMsg;
	cWaitDBIXResultProc.ProcErrMsg = (CSipTask::PFProcMsg)&CNVkekQueryTask::ProcErrMsg;
	cWaitDBIXResultProc.TimerPoll = (CSipTask::PFTimerPoll)&CNVkekQueryTask::WaitDBIXResultTimerPoll;
	AddRuleProc(RSP, cWaitDBIXResultProc);

	NextState(REQ);
}


u32 CNVkekQueryTask::HandleQueryProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	CSecurityModuleNVkekQueryReq cReq;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	pcOspSipMsg->GetMsgBody(cReq);

	TASKLOG(EVENT_LEV, "收到REQ消息,event[%s]\n",cReq.GetEventStr().c_str());

	m_tTransID = pcOspSipMsg->GetSipTransID();
	m_strSession = cReq.GetSession();
	m_dwSeqNum = cReq.GetSeqNum();
	m_dwEvent = cReq.GetEventId();

	if (cReq.GetPuid().empty())
	{
		CSecurityModuleVkekQueryRsp cRsp;
		cRsp.SetSeqNum(m_dwSeqNum);
		cRsp.SetSession(m_strSession);
		cRsp.SetErrorCode(ERR_SECURITY_PUID_IS_NULL);

		if (PROCMSG_OK == PostRsp(KDSIP_EVENT_MESSAGE_RSP,  m_tTransID, cRsp.ToXml()))
		{
			TASKLOG(EVENT_LEV, "发送消息event[%s-%d]成功!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}
		else
		{
			TASKLOG(ERROR_LEV, "发送消息event[%s-%d]失败!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
		}
	}

	QryFromDB(cReq);
	NextState(RSP);

	return dwRet;
}


u32 CNVkekQueryTask::WaitHandleQueryTimerPoll()
{

	return TIMERPOLL_DONE;
}


u32 CNVkekQueryTask::WaitDBIXResultProcMsg(CMessage *const pcMsg)
{

	NextState(RSP);
	u32 dwRet = PROCMSG_OK;
	switch(pcMsg->event)
	{
	case DBIX_RESULT_CB:
		{
			CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;

			u32 dwErrorCode = CMS_SUCCESS;

			if (pDbMsg == NULL)
			{
				TASKLOG(ERROR_LEV, "查询表[%s], dbix消息为空!\n",
					VKEK_INFO_TABLE);
				dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
			}

			if (pDbMsg->GetDBResult() != enDBResultSuccess)
			{
				TASKLOG(ERROR_LEV, "查询数据表[%s], db operate fail, db-errCode[%d]!\n",
					VKEK_INFO_TABLE, pDbMsg->GetDBResult());
				dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
			}  

			//发送应答或通知
			dwRet = ProcQryRsp(pcMsg, dwErrorCode);
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


u32 CNVkekQueryTask::WaitDBIXResultTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "Query table [%s], db operate timeout!\n",
			VKEK_INFO_TABLE);

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}



u32 CNVkekQueryTask::ProcErrMsg(CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			//发起的请求收到SIP层的错误应答
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if(NULL == pcOspSipMsg)
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


 u32 CNVkekQueryTask::ProcQryRsp(CMessage *const pcMsg, u32 dwErrorCode)
 {

	 NextState(RSP);
	 CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;
	 u32 dwRet = PROCMSG_DEL;
	 CSecurityModuleVkekQueryRsp cRsp;
	 vector<TSecurityModuleVkek> vkeks;

	 if (dwErrorCode == CMS_SUCCESS)
	 {
		 CDBEntityValue* pDbValList = (CDBEntityValue*)pDbMsg->GetDBData();
		 if (pDbValList != NULL)
		 { 
		
			 for (u32 i = 0; i < pDbValList->RowCount(); ++i)
			 {
				 TSecurityModuleVkek tSecurityModuleVkek;
				 string strTmp;

				 pDbValList->NextRow(i);
				 pDbValList->GetColumnVal(VKIT_VKEKS, strTmp);
				 tSecurityModuleVkek.FromXml(strTmp);	
				 vkeks.push_back(tSecurityModuleVkek);
			 }			 
		 }
		 else
		 {
			 dwRet = PostErrRsp();
			 return dwRet;
		 }
	 }
	
	cRsp.SetVkeks(vkeks);
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(dwErrorCode);

	if (PROCMSG_OK == PostRsp(KDSIP_EVENT_MESSAGE_RSP,  m_tTransID, cRsp.ToXml()))
	{
		TASKLOG(EVENT_LEV, "发送消息event[%s-%d]成功!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
	}
	else
	{
		TASKLOG(ERROR_LEV, "发送消息event[%s-%d]失败!\n",cRsp.GetEventStr().c_str(), cRsp.GetEventId());
	}

	return dwRet;

 }



void CNVkekQueryTask::QryFromDB(CSecurityModuleNVkekQueryReq &cReq)
{
	string strTmp;
	CDBEntity cEntity(g_cVkekInfoTable);
	
	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);

	CDBCriteria PuId_Cond;
	CDBCriteria ChnId_Cond;
	CDBCriteria Tm_Cond;
	
	CDBCriteria Condition;
	
	PuId_Cond = eq(VKIT_PUID, cReq.GetPuid());
	Condition = PuId_Cond;

	TOrderby tOrderby;

	TOrderbyVal tOrderbyVal(VKIT_TMFROM, false);
	tOrderby.push_back(tOrderbyVal);
	cEntity.SetQryFlag(0, cReq.GetNum(), &Condition, &tOrderby);
	OperateDBEntity(cEntity);
}



u32 CNVkekQueryTask::PostErrRsp()
{
	CSecurityModuleVkekQueryRsp cRsp;

	cRsp.SetErrorCode(ERR_SECURITY_DB_DATA_NOT_EXIST);
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);

	if (PROCMSG_OK != PostMsgRsp(m_tTransID, cRsp))
	{
		TASKLOG(ERROR_LEV, "发送应答消息 [%s-%d] 失败!\n", 
			OspExtEventDesc(cRsp.GetEventId()).c_str(), cRsp.GetEventId());
	}
	else
	{
		TASKLOG(EVENT_LEV, "发送应答消息 [%s-%d] 成功!\n", 
			OspExtEventDesc(cRsp.GetEventId()).c_str(), cRsp.GetEventId());
	}

	return PROCMSG_DEL;
}