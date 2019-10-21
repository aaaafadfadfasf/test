#include "vkek_save_task.h"
#include "dbixface.h"



CVkekSaveTask::CVkekSaveTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CVkekSaveTask::~CVkekSaveTask()
{

}


void CVkekSaveTask::InitStateMachine()
{

	CStateProc cReqProc;
	cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CVkekSaveTask::ProcReqMsg;
	cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekSaveTask::ProcErrMsg;
	cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CVkekSaveTask::ProcReqMsgTimerPoll;
	AddRuleProc(REQ, cReqProc);


	CStateProc cOptRspProc;
	cOptRspProc.ProcMsg = (CSipTask::PFProcMsg)&CVkekSaveTask::ProcOptRspMsg;
	cOptRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekSaveTask::ProcErrMsg;
	cOptRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CVkekSaveTask::ProcOptRspMsgTimerPoll;
	AddRuleProc(OPT_RSP, cOptRspProc);

	NextState(REQ);
}


u32 CVkekSaveTask::ProcReqMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "收到消息,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "消息体为空! 销毁事务\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_VKEK_SAVE_REQ)
	{
		CSecurityModuleVkekSaveReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "收到REQ消息,event[%s]\n",cReq.GetEventStr().c_str());

		m_strReqMsg = cReq.ToXml();

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();

		AddVkekReq(cReq);

	}
	else
	{
		TASKLOG(WARNING_LEV, "收到未知消息[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	NextState(OPT_RSP);
	return dwRet;
}


u32 CVkekSaveTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "插入数据库超时,销毁事务!\n");

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}



u32 CVkekSaveTask::ProcOptRspMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	switch(pcMsg->event)
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

u32 CVkekSaveTask::ProcOptRspMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "等待数据库处理超时，销毁事务!\n");
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}



u32 CVkekSaveTask::ProcErrMsg(CMessage *const pcMsg)
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



u32 CVkekSaveTask::PostRspToClient(u32 dwErrorCode)
{
	CSecurityModuleVkekSaveRsp cRsp;
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(dwErrorCode);

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


void CVkekSaveTask::AddVkekReq(const CSecurityModuleVkekSaveReq &cReq)
{

	CDBEntity cEntity(g_cVkekInfoTable);

	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);
	cEntity.SetAddFlag();

	cEntity.SetColumnVal(VKIT_PUID, cReq.GetPuid());
	cEntity.SetColumnVal(VKIT_CHANNELID, cReq.GetChnid());
	cEntity.SetColumnVal(VKIT_TMFROM, cReq.GetTm_from());
	
	TSecurityModuleVkek tSecurityModuleVkek;
	vector<TSecurityModuleVkek> vecVkek = cReq.GetVkeks();
	if (!vecVkek.empty())
	{
		tSecurityModuleVkek = *(vecVkek.begin());
		TASKLOG(ERROR_LEV,"VKER=%s\n", tSecurityModuleVkek.GetVkek().c_str());
		tSecurityModuleVkek.SetVkek(Encrypt(tSecurityModuleVkek.GetVkek()));
		TASKLOG(ERROR_LEV,"VKER=%s\n", tSecurityModuleVkek.GetVkek().c_str());		
	}
	cEntity.SetColumnVal(VKIT_VKEKS, tSecurityModuleVkek.ToXml());

	OperateDBEntity(cEntity);
	NextState(OPT_RSP);

}
