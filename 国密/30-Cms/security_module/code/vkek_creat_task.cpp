#include "vkek_creat_task.h"


CVkekCreateTask::CVkekCreateTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CVkekCreateTask::~CVkekCreateTask()
{

}


void CVkekCreateTask::InitStateMachine()
{

	CStateProc cState;
	cState.ProcMsg = (CSipTask::PFProcMsg)&CVkekCreateTask::ProcReqMsg;
	cState.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekCreateTask::ProcReqMsg;
	cState.TimerPoll = (CSipTask::PFTimerPoll)&CVkekCreateTask::ProcReqMsgTimerPoll;
	AddRuleProc(REQ, cState);

	cState.ProcMsg = (CSipTask::PFProcMsg)&CVkekCreateTask::ProcOptRspMsg;
	cState.ProcErrMsg = (CSipTask::PFProcMsg)&CVkekCreateTask::ProcOptRspMsg;
	cState.TimerPoll = (CSipTask::PFTimerPoll)&CVkekCreateTask::ProcOptRspMsgTimerPoll;
	AddRuleProc(OPT_RSP, cState);

	NextState(REQ);
}


u32 CVkekCreateTask::ProcReqMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "收到消息,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "消息体为空! 销毁事务\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_NEW_VKEK_REQ)
	{

		CSecurityModuleNewVkekReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);
		TASKLOG(EVENT_LEV, "收到REQ消息,event[%s]\n",cReq.GetEventStr().c_str());

		m_strReqMsg = cReq.ToXml();

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();

		AddVkekReq(cReq);
		NextState(OPT_RSP);
	}
	else
	{
		TASKLOG(WARNING_LEV, "收到未知消息[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	return dwRet;
}


u32 CVkekCreateTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "插入数据库超时,销毁事务!\n");

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}



u32 CVkekCreateTask::ProcOptRspMsg(CMessage *const pcMsg)
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


u32 CVkekCreateTask::ProcOptRspMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "等待数据库处理超时，销毁事务!\n");
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}


u32 CVkekCreateTask::PostRspToClient(u32 dwErrorCode)
{
	CSecurityModuleNewVkekRsp cRsp;
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(CMS_SUCCESS);
	cRsp.SetVkek(m_tVkek);

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


void CVkekCreateTask::AddVkekReq(const CSecurityModuleNewVkekReq &cReq)
{

	CDBEntity cEntity(g_cVkekInfoTable);

	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);
	cEntity.SetAddFlag();

	string strVkekEncrypt = cReq.GetWithId() + cReq.GetVkekVersion();
	string strVkekDecrypt = "";
	MD5(strVkekEncrypt, strVkekDecrypt);
	TASKLOG(ERROR_LEV,"VKER=%s\n", strVkekDecrypt.substr(0,16).c_str());
	TASKLOG(ERROR_LEV,"VKER=%s\n", Encrypt(strVkekDecrypt.substr(0,16)).c_str());
	m_tVkek.SetVkek(Encrypt(strVkekDecrypt.substr(0,16)));
	m_tVkek.SetVersion(cReq.GetVkekVersion());

	cEntity.SetColumnVal(VKIT_PUID, cReq.GetWithId());
	cEntity.SetColumnVal(VKIT_CHANNELID, -1);
	cEntity.SetColumnVal(VKIT_TMFROM, CCMSTime().ToString());
	cEntity.SetColumnVal(VKIT_VKEKS, m_tVkek.ToXml());

	OperateDBEntity(cEntity);
	m_tVkek.SetVkek(strVkekDecrypt.substr(0,16));
	NextState(OPT_RSP);

}