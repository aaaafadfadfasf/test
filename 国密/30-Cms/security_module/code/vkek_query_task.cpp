#include "vkek_query_task.h"



CVkekQueryTask::CVkekQueryTask(CInstExt *pcInst) : CSipTask(pcInst)
{
	m_bQryFlag = false;
}


CVkekQueryTask::~CVkekQueryTask()
{

}


void CVkekQueryTask::InitStateMachine()
{

	CStateProc cHandleQryProc;
	cHandleQryProc.ProcMsg = (CSipTask::PFProcMsg)&CVkekQueryTask::HandleQueryProcMsg;
	cHandleQryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekQueryTask::ProcErrMsg;
	cHandleQryProc.TimerPoll = (CSipTask::PFTimerPoll)&CVkekQueryTask::WaitHandleQueryTimerPoll;
	AddRuleProc(REQ, cHandleQryProc);

	CStateProc cWaitDBIXResultProc;
	cWaitDBIXResultProc.ProcMsg = (CSipTask::PFProcMsg)&CVkekQueryTask::WaitDBIXResultProcMsg;
	cWaitDBIXResultProc.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekQueryTask::ProcErrMsg;
	cWaitDBIXResultProc.TimerPoll = (CSipTask::PFTimerPoll)&CVkekQueryTask::WaitDBIXResultTimerPoll;
	AddRuleProc(RSP, cWaitDBIXResultProc);

	NextState(REQ);
}


u32 CVkekQueryTask::HandleQueryProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	pcOspSipMsg->GetMsgBody(m_cReq);

	TASKLOG(EVENT_LEV, "收到REQ消息,event[%s]\n",m_cReq.GetEventStr().c_str());

	m_tTransID = pcOspSipMsg->GetSipTransID();
	m_strSession = m_cReq.GetSession();
	m_dwSeqNum = m_cReq.GetSeqNum();
	m_dwEvent = m_cReq.GetEventId();
	if (m_cReq.GetPuid().empty())
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

	QryFromDB();
	NextState(RSP);

	return dwRet;
}


u32 CVkekQueryTask::WaitHandleQueryTimerPoll()
{

	return TIMERPOLL_DONE;
}


u32 CVkekQueryTask::WaitDBIXResultProcMsg(CMessage *const pcMsg)
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


u32 CVkekQueryTask::WaitDBIXResultTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "Query table [%s], db operate timeout!\n",
			VKEK_INFO_TABLE);

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}



u32 CVkekQueryTask::ProcErrMsg(CMessage *const pcMsg)
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


 u32 CVkekQueryTask::ProcQryRsp(CMessage *const pcMsg, u32 dwErrorCode)
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
			 string strTmp;
			 if (m_bQryFlag)
			 {
				 if (1 == pDbValList->RowCount())
				 {
					 pDbValList->NextRow(0);
					 pDbValList->GetColumnVal(VKIT_TMFROM, strTmp);
					 m_bQryFlag = false;
					 QryFromDB(strTmp);
					 NextState(RSP);
					 return PROCMSG_OK;
				 }
				 else
				 {
					 m_bQryFlag = false;
					 QryFromDB(m_cReq.GetTm_from());
					 NextState(RSP);
					 return PROCMSG_OK;
				 }
			 }
			 else
			 {
				 for (u32 i = 0; i < pDbValList->RowCount(); ++i)
				 {
					 TSecurityModuleVkek tSecurityModuleVkek;
					 string strTmp;

					 pDbValList->NextRow(i);
					 pDbValList->GetColumnVal(VKIT_VKEKS, strTmp);
					 tSecurityModuleVkek.FromXml(strTmp);
					 TASKLOG(ERROR_LEV,"VKER=%s\n", tSecurityModuleVkek.GetVkek().c_str());
					 tSecurityModuleVkek.SetVkek(Decrypt(tSecurityModuleVkek.GetVkek()).substr(0,16));
					 TASKLOG(ERROR_LEV,"VKER=%s\n", tSecurityModuleVkek.GetVkek().c_str());
					 vkeks.push_back(tSecurityModuleVkek);
				 }
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



void CVkekQueryTask::QryFromDB()
{
	string strTmp;
	CDBEntity cEntity(g_cVkekInfoTable);
	
	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);

	CDBCriteria PuId_Cond;
	CDBCriteria ChnId_Cond;
	CDBCriteria Tm_Cond;
	
	CDBCriteria Condition;
	
	PuId_Cond = eq(VKIT_PUID, m_cReq.GetPuid());
	Condition = PuId_Cond;

	ChnId_Cond = eq(VKIT_CHANNELID, m_cReq.GetChnid());
	if (Condition != "")
	{
		Condition = sql_and(Condition, ChnId_Cond);
	}
	else
	{
		Condition = ChnId_Cond;
	}

	TOrderby tOrderby;
	if (!m_cReq.GetTm_from().empty())
	{
		m_bQryFlag = true;
		Tm_Cond = lt(VKIT_TMFROM, m_cReq.GetTm_from());	
		if (Condition != "")
		{
			Condition = sql_and(Condition, Tm_Cond);
		}
		else
		{
			Condition = Tm_Cond;
		}
	}
	
	TOrderbyVal tOrderbyVal(VKIT_TMFROM, false);
	tOrderby.push_back(tOrderbyVal);
	cEntity.SetQryFlag(0, 1, &Condition, &tOrderby);
	OperateDBEntity(cEntity);
}


void CVkekQueryTask::QryFromDB(string strTmFrom)
{
	string strTmp;
	CDBEntity cEntity(g_cVkekInfoTable);
	
	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);

	CDBCriteria PuId_Cond;
	CDBCriteria ChnId_Cond;
	CDBCriteria Tm_Cond;
	
	CDBCriteria Condition;

	PuId_Cond = eq(VKIT_PUID, m_cReq.GetPuid());
	Condition = PuId_Cond;

	ChnId_Cond = eq(VKIT_CHANNELID, m_cReq.GetChnid());
	if (Condition != "")
	{
		Condition = sql_and(Condition, ChnId_Cond);
	}
	else
	{
		Condition = ChnId_Cond;
	}

	if (!m_cReq.GetTm_to().empty())
	{
		Tm_Cond = between(VKIT_TMFROM, strTmFrom, m_cReq.GetTm_to());
	}
	else
	{
		Tm_Cond = gt(VKIT_TMFROM, strTmFrom);
	}

	if (Condition != "")
	{
		Condition = sql_and(Condition, Tm_Cond);
	}
	else
	{
		Condition = Tm_Cond;
	}
	
	cEntity.SetQryFlag(0, 1000, &Condition);
	OperateDBEntity(cEntity);

}


u32 CVkekQueryTask::PostErrRsp()
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