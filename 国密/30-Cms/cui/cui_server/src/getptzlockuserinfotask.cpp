#include "getptzlockuserinfotask.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/md5.h"
#include "cms/utility/flow_limit_counter.h"
#include "redisdb.h"

extern CRedisDBInterface g_cRedisDb;

CGetPuPtzLockUserInfoTask::CGetPuPtzLockUserInfoTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
	CMS_ASSERT(pcCuAgent != NULL);
	CMS_ASSERT(ptRecvNode != NULL);
	m_pcCuAgent = pcCuAgent;
	m_ptRecvNode = ptRecvNode;
	m_pc3acAgent = NULL;
	m_dwReqEvent = 0;
	m_pcCuAgent->AddSubTask(this);
	m_pRsp = NULL;
	m_dwRspEventId = 0;
	m_bIsSuperAdmin = false;
}

void CGetPuPtzLockUserInfoTask::SetPuPtzCtrlRsp(const CPuPtzCtrlRsp& cRsp)
{
	m_pRsp = new CPuPtzCtrlRsp();
	m_dwRspEventId = PU_PTZ_CTRL_RSP;
	if ( m_pRsp != NULL )
	{
		*m_pRsp = cRsp;
	}
}

void CGetPuPtzLockUserInfoTask::SetPuTransOperateRsp(const CCuPuTransparentOperateRsp& cRsp)
{
	m_pRsp = new CCuPuTransparentOperateRsp();
	m_dwRspEventId = PU_TRANSPARENT_OPERATE_RSP;
	if ( m_pRsp != NULL )
	{
		*m_pRsp = cRsp;
	}
}

CGetPuPtzLockUserInfoTask::~CGetPuPtzLockUserInfoTask()
{
	TerminateTask();
	if (!m_strQueryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
		m_strQueryTag.clear();
	}

	if (NULL != m_pRsp)
	{
		delete m_pRsp;
		m_pRsp = NULL;
	}
}

void CGetPuPtzLockUserInfoTask::TerminateTask()
{
	m_pcCuAgent->DelSubTask(this);
	while (!m_VecNtfReqList.empty())
	{
		string strNtfMsg = m_VecNtfReqList.front();
		m_VecNtfReqList.pop_front();
		COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
		TASKLOG(NORMAL_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
		if (PU_PTZ_CTRL_RSP == m_dwRspEventId)
		{
			CPuPtzCtrlRsp cRsp;
			cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
			string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
			cRsp.SetSession(strSession);
			string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
			int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
			cRsp.SetSeqNum(nSeqNum);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);		
		} 
		else
		{
			CCuPuTransparentOperateRsp cRsp;
			cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
			string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
			cRsp.SetSession(strSession);
			string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
			int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
			cRsp.SetSeqNum(nSeqNum);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
		}
	}
}

void CGetPuPtzLockUserInfoTask::InitStateMachine()
{
	CStateProc cNotQueryProc;
	cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::NotQueryProcMsg;
	cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
	cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::NotQueryProcMsg;
	AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

	CStateProc cWaitRspProc;
	cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::WaitRspProcMsg;
	cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetPuPtzLockUserInfoTask::WaitRspTimerPoll;
	cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::WaitRspProcMsg;
	AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

	CStateProc cWaitNotifyProc;
	cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::WaitNotifyProcMsg;
	cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetPuPtzLockUserInfoTask::WaitNotifyTimerPoll;
	cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetPuPtzLockUserInfoTask::WaitNotifyProcMsg;
	AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

	NextState(NOT_QUERY_STATE);
}

u32 CGetPuPtzLockUserInfoTask::NotQueryProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, EVENT_LEV, "[CGetPuPtzLockUserInfoTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CGetPuPtzLockUserInfoTask::WaitRspProcMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
			CMS_ASSERT(NULL != m_ptRecvNode);
			m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			NextState(NOT_QUERY_STATE);
			delete this;
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			CMS_ASSERT(0 && "CGetPuPtzLockUserInfoTask::WaitRspProcMsg不可能出现会话内消息");
		}
		break;
	case TAS_CUSER_QRY_RSP:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CUserQryRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
		
			if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0)
			{ //查询失败 或者 无结果
				if (cRsp.GetErrorCode() != CMS_SUCCESS)
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
						GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
				}
				else
				{
					TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
						GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
				}

				m_strRspOfErr = m_pRsp->ToXml();
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}
			NextState(WAIT_NOTIFY_STATE);
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_DEVICE_QRY_RSP:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CUserDeviceQryRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0)
			{ //查询失败 或者 无结果
				if (cRsp.GetErrorCode() != CMS_SUCCESS)
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
						GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
				}
				else
				{
					TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
						GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
				}

				m_strRspOfErr = m_pRsp->ToXml();
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			NextState(WAIT_NOTIFY_STATE);
			return PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CGetPuPtzLockUserInfoTask::WaitRspTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
			m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
		CMS_ASSERT(NULL != m_ptRecvNode);
		m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
		m_strRspOfErr = m_pRsp->ToXml();
		SendRspToCu(m_strRspOfErr, m_ptRecvNode);
		NextState(NOT_QUERY_STATE);

		// 加上超时
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CGetPuPtzLockUserInfoTask::WaitNotifyProcMsg(CMessage *const pcMsg)
{
	while (m_VecNtfReqList.size() != 0)
	{
		CMessage cMsg;
		cMsg.event = m_dwReqEvent + 2;
		string content = m_VecNtfReqList.front();
		m_VecNtfReqList.pop_front();
		cMsg.content = (u8 *)content.c_str();

		ProcNotifyMsg(&cMsg);
	}
	ProcNotifyMsg(pcMsg);

	return PROCMSG_OK;
}

u32 CGetPuPtzLockUserInfoTask::ProcNotifyMsg(CMessage *const pcMsg)
{
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			CMS_ASSERT(0 && "CGetPuPtzLockUserInfoTask::WaitNotifyProcMsg不可能出现SIP异常");
		}
		break;
	case TAS_CUSER_QRY_NTF_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CUserQryNtfReq cNtf;
			pcOspSipMsg->GetMsgBody(cNtf);

			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
				cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
				cNtf.GetEntryNum());

			//先回复NTF通知
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtf.GetSeqNum());
			cRsp.SetSession(cNtf.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			for (std::vector<CUserInfo>::const_iterator userIter = cNtf.GetUserInfo().begin();
			userIter != cNtf.GetUserInfo().end(); userIter++)
			{
				if (userIter->GetUserID() == GetNameWithoutDomainFromURI(m_strUserId))
				{
					if (userIter->GetUserType() == CUSER_TYPE_SUPERADMIN)
					{
						m_bIsSuperAdmin = true;
					}

					if (PU_PTZ_CTRL_RSP == m_dwRspEventId)
					{
						CPuPtzCtrlRsp *pcRsp = (CPuPtzCtrlRsp*)m_pRsp;
						if (pcRsp != NULL)
						{
							pcRsp->SetUsername(userIter->GetUserName());
						}
					} 
					else
					{
						CCuPuTransparentOperateRsp *pcRsp = (CCuPuTransparentOperateRsp*)m_pRsp;
						if (pcRsp != NULL)
						{
							pcRsp->SetUsername(userIter->GetUserName());
						}
					}
				}
			}

			//判断是否后续还有Ntf
			if (cNtf.IsLastNtf())
			{
				g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
				m_strQueryTag.clear();
				NextState(NOT_QUERY_STATE);

				this->GetUserDevVsPtzPri();
			}
			else
			{
				NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断

			}
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_DEVICE_QRY_NTF_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CUserDeviceQryNtfReq cNtf;
			pcOspSipMsg->GetMsgBody(cNtf);
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
				cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
				cNtf.GetEntryNum());

			//先回复NTF通知
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtf.GetSeqNum());
			cRsp.SetSession(cNtf.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			for (std::vector<CUserDeviceInfo>::const_iterator devIter = cNtf.GetCuserDevInfo().begin();
			devIter != cNtf.GetCuserDevInfo().end(); devIter++)
			{
				if (devIter->GetDeviceID() == m_tVsChn.GetDevUri())
				{
					int nPtzPri = 0;

					if ( m_bIsSuperAdmin )
					{
						nPtzPri = CMS_PTZ_MAX_PRI_LEV;
					}
					else
					{ 
						string uuid = GetNameWithoutDomainFromURI(m_tVsChn.GetDevUri());
						CVideosourcePrivilege cPri;
						string errMsg;
						EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(m_strUserId, uuid, m_tVsChn.GetChnNO(), cPri, errMsg);
						if ( !IsFdbResultPositive(er) )
						{
							TASKLOG(NORMAL_TASK, CRITICAL_LEV, "用户 id[%s] 没有视频源[%s]权限 errmsg[%s] er[%d]\n",
								m_strUserId.c_str(), m_tVsChn.ToXml().c_str(), errMsg.c_str(), er);
						}
						else
						{
							nPtzPri = cPri.GetPTZCtrl();
						}
					}

					if (PU_PTZ_CTRL_RSP == m_dwRspEventId)
					{
						CPuPtzCtrlRsp *pcRsp = (CPuPtzCtrlRsp*)m_pRsp;
						if (pcRsp != NULL)
						{
							pcRsp->SetPriPtzLevel(nPtzPri);
						}
					} 
					else
					{
						CCuPuTransparentOperateRsp *pcRsp = (CCuPuTransparentOperateRsp*)m_pRsp;
						if (pcRsp != NULL)
						{
							pcRsp->SetPriPtzLevel(nPtzPri);
						}
					}

					break;
				}
			}

			//判断是否后续还有Ntf
			if (cNtf.IsLastNtf())
			{

				if (PU_PTZ_CTRL_RSP == m_pRsp->GetEventId())
				{
					CPuPtzCtrlRsp *pcRsp = (CPuPtzCtrlRsp*)m_pRsp;
					CCuPtzLockInfo ptzLockInfo;
					if (m_pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(m_tVsChn, ptzLockInfo))
					{
						pcRsp->SetBeginTime(ptzLockInfo.GetBeginTime());
						pcRsp->SetEndTime(ptzLockInfo.GetEndTime());
					}
					SendRspToCu<CPuPtzCtrlRsp>(*pcRsp, m_ptRecvNode);
				} 
				else
				{
					CCuPuTransparentOperateRsp *pcRsp = (CCuPuTransparentOperateRsp*)m_pRsp;
					CCuPtzLockInfo ptzLockInfo;
					if (m_pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(m_tVsChn, ptzLockInfo))
					{
						pcRsp->SetBeginTime(ptzLockInfo.GetBeginTime());
						pcRsp->SetEndTime(ptzLockInfo.GetEndTime());
					}
					SendRspToCu<CCuPuTransparentOperateRsp>(*pcRsp, m_ptRecvNode);
				}
				g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
				m_strQueryTag.clear();
				NextState(NOT_QUERY_STATE);
				delete this;
			}
			else
			{
				NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断
			}
			return PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CGetPuPtzLockUserInfoTask::WaitNotifyTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]\n",
			GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId());
		CMS_ASSERT(NULL != m_ptRecvNode);
		m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
		m_strRspOfErr = m_pRsp->ToXml();
		SendRspToCu(m_strRspOfErr, m_ptRecvNode);
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
		m_strQueryTag.clear();
		NextState(NOT_QUERY_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

bool CGetPuPtzLockUserInfoTask::GetUserNameInfo(const string &strUserId, const TChannel &tDevVsChn)
{
	m_strUserId = strUserId;
	m_tVsChn = tDevVsChn;

	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());;
	CMS_ASSERT(NULL != pc3acAgent);
	CMS_ASSERT(0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent"));
	if (pc3acAgent == NULL)
	{
		return false;
	}

	m_pc3acAgent = pc3acAgent;

	CUserQryReq cQryReq;
	cQryReq.SetStartEntryIndex(0);
	cQryReq.SetExpectEntryNum(1);
	cQryReq.SetSession(m_pc3acAgent->m_strSessionID);
	CUserInfoQueryCondition userInfoQueryCondition;
	userInfoQueryCondition.SetUserID(GetNameWithoutDomainFromURI(strUserId));
	cQryReq.SetUserInfoQueryCondition(userInfoQueryCondition);

	TASKLOG(LOGIN_TASK, EVENT_LEV, "以会话ID[%s]向TAS[%s]查询[%s]账户信息\n",
		m_pc3acAgent->m_strSessionID.c_str(), m_pc3acAgent->m_t3asURI.GetURIString().c_str(), strUserId.c_str());

	m_dwReqEvent = (u16)cQryReq.GetEventId();

	FakePostMsgReq(cQryReq, m_pc3acAgent->m_t3asURI);
	if (!m_strQueryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
	}
	g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
	m_strQueryTag = cQryReq.GetQueryTag();
	
	NextState(WAIT_RESPONSE_STATE);

	return true;
}

bool CGetPuPtzLockUserInfoTask::GetUserDevVsPtzPri()
{
	CCuGetUserDeviceReq cQryReq;
	cQryReq.SetStartEntryIndex(0);
	cQryReq.SetExpectEntryNum(0);
	cQryReq.SetSession(m_pc3acAgent->m_strSessionID);

	m_dwReqEvent = (u16)cQryReq.GetEventId();

	cQryReq.GetUserDeviceInfoQueryCondition().SetUserID(m_strUserId);
	cQryReq.GetUserDeviceInfoQueryCondition().SetDeviceID(m_tVsChn.GetDevUri());

	TASKLOG(LOGIN_TASK, EVENT_LEV, "以会话ID[%s]向TAS[%s]查询[%s]用户设备信息\n",
		m_pc3acAgent->m_strSessionID.c_str(), m_pc3acAgent->m_t3asURI.GetURIString().c_str(), m_strUserId.c_str());

	FakePostMsgReq(cQryReq, m_pc3acAgent->m_t3asURI);
	if (!m_strQueryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
	}

	g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
	m_strQueryTag = cQryReq.GetQueryTag();

	NextState(WAIT_RESPONSE_STATE);
	return true;
}
