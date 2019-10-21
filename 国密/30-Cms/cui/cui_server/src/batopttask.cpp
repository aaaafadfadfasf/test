#include "batopttask.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/camera/newcamera.h"
#include "cms/tas/tas_proto.h"
#include "StreamDecentrationAdapter.h"
class CUsrOprLogTask;

extern CRedisSdk g_cCuiRedisSdk;

CBatOptTask::CBatOptTask(CInstExt *pcInst, const string &strReq)
	: CSipTask(pcInst)
{
	if ( strReq.empty() )
	{
		CMS_ASSERT(0 && "m_strBatOptReq.empty!");
	}
	m_strBatOptReq = strReq;
	m_eBatOptType = enBatOptTypeInvalid;
	m_pcCuBatOptList = NULL;

	m_pc3acAgent = NULL;

	m_userPri.Clear();

	m_nTotal = 0;
	m_nPos = 0;
	m_allDevVsInGroup.clear();

	m_wCurrTimeProcedRecNum = 0;
}

CBatOptTask::~CBatOptTask()
{
    TerminateTask();

	if (!m_strQryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
		m_strQryTag.clear();
	}
}

void CBatOptTask::TerminateTask()
{
	TBatOptInfo tBatOptInfo;
	tBatOptInfo.m_strBatOptReq = m_strBatOptReq;
	tBatOptInfo.m_nTaskNo = this->GetTaskNO();

	if ( m_pcCuBatOptList != NULL 
		&& m_pcCuBatOptList->PopFrontByBatOptInfo(tBatOptInfo) == false)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "TaskNo[%u]退出时，删除第一条批量操作[%s]失败？\n", this->GetTaskNO(), m_strBatOptReq.c_str());
	}
	else
	{
		TASKLOG(NORMAL_TASK, PROGRESS_LEV, "TaskNo[%u]退出，批量操作[%s]执行结束\n", this->GetTaskNO(), m_strBatOptReq.c_str());
	}

    while(!m_VecNtfReqList.empty())
    { 
        string strNtfMsg = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
        TASKLOG(NORMAL_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
        CNtfRsp cRsp;
        cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
        string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
        cRsp.SetSession(strSession);
        string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
        int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
        cRsp.SetSeqNum(nSeqNum);
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
    }
}

void CBatOptTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CBatOptTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatOptTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CBatOptTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CBatOptTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

	CStateProc cWaitDpsRspProc;
	cWaitDpsRspProc.ProcMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitBatOptRspProcMsg;
	cWaitDpsRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CBatOptTask::WaitBatOptRspTimerPol;
	cWaitDpsRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatOptTask::WaitBatOptRspProcMsg;
	AddRuleProc(WAIT_OPT_RSP_STATE, cWaitDpsRspProc);

    NextState(NOT_QUERY_STATE);
}

u32 CBatOptTask::NotQueryProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(LOGIN_TASK, EVENT_LEV, "[TaskNO %u]收到请求[%s][%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
	case CU_CUI_BATCH_OPT_REQ:
		{
			if (GetBatOptType(m_strBatOptReq, m_cuBatOptReq, m_eBatOptType, m_pcCuBatOptList) == false
				|| !this->Get3aAgentInOnLineStat(m_pc3acAgent))
			{
				delete this;
				return PROCMSG_OK;
			}

			u32 dwRet = PROCMSG_OK;

			bool isNeedQueryUserPri = false;
			if (m_eBatOptType == enBatOptStartPlatRecord || m_eBatOptType == enBatOptStopPlatRecord)
			{
				if (this->GetUserPri())
				{
					if (m_userPri.HasPri(CUserPrivilege::en_Vod_Rec) == false)
					{
						TASKLOG(NORMAL_TASK, EVENT_LEV, "用户[%s]没有平台录像的权限，停止批量操作[%s]，退出TASK[%u]\n",
							m_cuBatOptReq.GetUserId().c_str(), m_strBatOptReq.c_str(), this->GetTaskNO());
						
						NextState(NOT_QUERY_STATE);
						delete this;
						return PROCMSG_OK;
					}
				}
				else
				{
					isNeedQueryUserPri = true;
				}
			}


			if (isNeedQueryUserPri)
			{
				dwRet = this->QueryUserInfo();
			}
			else
			{
				dwRet = this->QueryUserDevVsByGrp();
			}

			if ( dwRet != PROCMSG_OK )
			{
				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			return PROCMSG_OK;
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CBatOptTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
			// 查询失败，只能退出了
            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息，向3A查询失败了\n");
            NextState(NOT_QUERY_STATE);
            delete this;
			return PROCMSG_OK;
        }
        break;
	case TAS_CUSER_QRY_RSP:
		{
			CUserQryRsp cQryRsp;
			pcOspSipMsg->GetMsgBody(cQryRsp);

			if (cQryRsp.GetErrorCode() != CMS_SUCCESS)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u]查询应答[%s--%hu]返回失败，错误码[%d] 退出批量操作[%s]\n",
					GetTaskNO(), cQryRsp.GetEventStr().c_str(), cQryRsp.GetEventId(), cQryRsp.GetErrorCode(), m_strBatOptReq.c_str());

				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			CMS_ASSERT(1 == cQryRsp.GetTotalEntryNum());
			if (cQryRsp.GetTotalEntryNum() <= 0)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 查询应答[%s--%hu]返回值不是预期，TotalNum[%d] 退出批量操作[%s]\n",
					GetTaskNO(), cQryRsp.GetEventStr().c_str(), cQryRsp.GetEventId(), cQryRsp.GetTotalEntryNum(), m_strBatOptReq.c_str());

				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			NextState(WAIT_NOTIFY_STATE);
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP:
		{
			CUserDevGrpVidsrcQryRsp cQryRsp;
			pcOspSipMsg->GetMsgBody(cQryRsp);

			if ( cQryRsp.GetErrorCode() != CMS_SUCCESS )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u]查询应答[%s--%hu]返回失败，错误码[%d] 退出批量操作[%s]\n",
					GetTaskNO(), cQryRsp.GetEventStr().c_str(), cQryRsp.GetEventId(), cQryRsp.GetErrorCode(), m_strBatOptReq.c_str());

				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			if ( !cQryRsp.GetHasRecords() )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "[NO: %u]查询应答[%s--%d]成功，结果为空 退出批量操作[%s]\n",
					GetTaskNO(), cQryRsp.GetEventStr().c_str(), cQryRsp.GetEventId(), m_strBatOptReq.c_str());

				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}
			
			NextState(WAIT_NOTIFY_STATE);
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_QRY_NTF_REQ:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
		{
			if ((m_dwQueryEvent + 2) == pcMsg->event)
			{
				if (pcMsg->content != NULL)
				{
					string strContect((char *)pcMsg->content, pcMsg->length);
					m_VecNtfReqList.push_back(strContect);
					TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到相应的Ntf消息[%s--%hu]\n",
						OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到不对应的Ntf消息[%s--%hu]\n",
					OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CBatOptTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
		TASKLOG(NORMAL_TASK, ERROR_LEV, "批量操作[%s]等待应答消息[%d]超时\n", 
		    m_strBatOptReq.c_str(), m_dwQueryEvent);

		NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CBatOptTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
	{
	case TAS_CUSER_QRY_NTF_REQ:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
		{
			while (!m_VecNtfReqList.empty())
			{
				CMessage cMsg;
				cMsg.event = m_dwQueryEvent + 2;
				string content = m_VecNtfReqList.front();
				m_VecNtfReqList.pop_front();
				cMsg.content = (u8 *)content.c_str();

				ProcNotifyMsg(&cMsg);
			}

			ProcNotifyMsg(pcMsg);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CBatOptTask::ProcNotifyMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]收到服务器[%s]的消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			// 查询失败，只能退出了
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息，向3A查询失败了\n");
			NextState(NOT_QUERY_STATE);
			delete this;
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_QRY_NTF_REQ:
		{
			CUserQryNtfReq cNtf;
			pcOspSipMsg->GetMsgBody(cNtf);

			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%hu]，本次Ntf中含带[%d]条记录\n",
				cNtf.GetEventStr().c_str(), cNtf.GetEventId(), cNtf.GetUserInfo().size());

			vector<CUserInfo> &userInfoList = cNtf.GetUserInfo();
			string userId = GetNameWithoutDomainFromURI(m_cuBatOptReq.GetUserId());
			for (size_t nIndex = 0; nIndex < userInfoList.size(); nIndex++)
			{
				if (userInfoList[nIndex].GetUserID() == userId)
				{
					m_userPri.SetPri(userInfoList[nIndex].GetUserPrivilege());
					break;
				}
			}

			//回复NTF通知
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtf.GetSeqNum());
			cRsp.SetSession(cNtf.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			if (!cNtf.IsLastNtf())
			{
				CMS_ASSERT(0 && "CUserQryNtfReq Not Only!");
			}

			if (!m_strQryTag.empty())
			{
				g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
				m_strQryTag.clear();
			}

			if (m_userPri.HasPri(CUserPrivilege::en_Vod_Rec) == false)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "用户[%s]没有平台录像的权限，停止批量操作[%s]，退出TASK[%u]\n",
					m_cuBatOptReq.GetUserId().c_str(), m_strBatOptReq.c_str(), this->GetTaskNO());

				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			if (this->QueryUserDevVsByGrp() == PROCMSG_OK)
			{
				NextState(WAIT_RESPONSE_STATE);
				return PROCMSG_OK;
			}

			NextState(NOT_QUERY_STATE);
			delete this;
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
		{
			CUserDevGrpVidsrcQryNtfReq cNtf;
			pcOspSipMsg->GetMsgBody(cNtf);

			SaveVsData(cNtf.GetInfos());

			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%hu]，本次Ntf中含带[%d]条记录\n",
				cNtf.GetEventStr().c_str(), cNtf.GetEventId(), cNtf.GetInfos().size());

			//先回复NTF通知
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtf.GetSeqNum());
			cRsp.SetSession(cNtf.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			if (!cNtf.GetLastNtf())
			{
				NextState(WAIT_NOTIFY_STATE);
				return PROCMSG_OK;
			}

			if (!m_strQryTag.empty())
			{
				g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
				m_strQryTag.clear();
			}

			if (this->SendBatOpt() == false)
			{
				NextState(NOT_QUERY_STATE);
				delete this;
				return PROCMSG_OK;
			}

			NextState(WAIT_OPT_RSP_STATE);
			return PROCMSG_OK;
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CBatOptTask::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CBatOptTask[NO: %u]等待NTF消息超时 批量操作 QueryEvent [%s]失败\n",
            GetTaskNO(), m_strQueryEvent.c_str());

		if (!m_strQryTag.empty())
		{
			g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
			m_strQryTag.clear();
		}

        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CBatOptTask::WaitBatOptRspProcMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	int erroCode = CMS_SUCCESS;
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			// 消息发送失败？接着发下一条
			erroCode = OSP_SIP_MSG_PROC_FAIL;
		}
		break;
	case CUI_BAT_OPT_TASK_NTF:
	case CUI_BAT_OPT_TASK_TIMER:
		{
		}
		break;
	case RCS_SET_RECORD_TASK_RSP:
		{
			CRcsSetRecordTaskRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			erroCode = cRsp.GetErrorCode();

			if (!m_strSendBatOptReq.empty())
			{
				CRcsSetRecordTaskReq cReq;
				cReq.FromXml(m_strSendBatOptReq);

				WriteLog(m_cuBatOptReq.GetSession(), cReq.GetPuChn(), cReq.GetEventId(), cRsp.GetErrorCode());
			}
		}
		break;
	case PU_RECORD_START_RSP:
		{
			CPuRecordStartRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			erroCode = cRsp.GetErrorCode();

			if (!m_strSendBatOptReq.empty())
			{
				CPuRecordStartReq cReq;
				cReq.FromXml(m_strSendBatOptReq);

				WriteLog(m_cuBatOptReq.GetSession(), cReq.GetDevChn(), cReq.GetEventId(), cRsp.GetErrorCode());
			}
		}
		break;
	case PU_RECORD_STOP_RSP:
		{
			CPuRecordStopRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			erroCode = cRsp.GetErrorCode();

			if (!m_strSendBatOptReq.empty())
			{
				CPuRecordStopReq cReq;
				cReq.FromXml(m_strSendBatOptReq);

				WriteLog(m_cuBatOptReq.GetSession(), cReq.GetDevChn(), cReq.GetEventId(), cRsp.GetErrorCode());
			}
		}
		break;
	case PU_PTZ_CTRL_RSP:
		{
			CPuPtzCtrlRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			erroCode = cRsp.GetErrorCode();

			if (!m_strSendBatOptReq.empty())
			{
				CPuPtzCtrlReq cReq;
				cReq.FromXml(m_strSendBatOptReq);

				WriteLog(m_cuBatOptReq.GetSession(), cReq.GetDevChn(), cReq.GetEventId(), cRsp.GetErrorCode());
			}
		}
		break;
	case STOP_THIRD_PARTY_CALL_RSP:
		{
			CStopThirdPartyCallRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			erroCode = cRsp.GetErrorCode();

			if (!m_strSendBatOptReq.empty())
			{
				CStopThirdPartyCallReq cReq;
				cReq.FromXml(m_strSendBatOptReq);

				WriteLog(m_cuBatOptReq.GetSession(), cReq.GetDstChn(), cReq.GetEventId(), cRsp.GetErrorCode());
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
			return PROCMSG_OK;
		}
	}

	if (erroCode != CMS_SUCCESS)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 消息处理失败 [%s]\n", this->GetTaskNO(), m_strSendBatOptReq.c_str());
	}

	if (this->SendBatOpt() == false)
	{
		NextState(NOT_QUERY_STATE);
		//delete this;
		return PROCMSG_DEL;
	}

	NextState(WAIT_OPT_RSP_STATE);
	return PROCMSG_OK;
}

u32 CBatOptTask::WaitBatOptRspTimerPol()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 批量操作[%s]等待消息[%s]应答超时\n",
			this->GetTaskNO(), m_strBatOptReq.c_str(), m_strSendBatOptReq.c_str());

		if (!m_strQryTag.empty())
		{
			g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
			m_strQryTag.clear();
		}

		NextState(NOT_QUERY_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

bool CBatOptTask::GetBatOptType(const string &strBatOptReq, CCuBatOptReq &cuBatOptReq, ECuBatOptType &optType, CuBatOptList* &pcCuBatOptList)
{
	pcCuBatOptList = NULL;

	if ( strBatOptReq.empty() )
	{
		CMS_ASSERT(0 && "m_strBatOptReq.empty!");
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 批量操作任务是空的？\n",this->GetTaskNO());
		return false;
	}

	cuBatOptReq.FromXml(strBatOptReq);

	optType = (ECuBatOptType)cuBatOptReq.GetOptType();
	switch (cuBatOptReq.GetOptType())
	{
	case enBatOptStartPlatRecord:
	case enBatOptStopPlatRecord:
		{
			pcCuBatOptList = &g_cCuiDataMgr.m_cPlatBatOptRequestList;
		}
		break;
	case enBatOptStartPuRecord:
	case enBatOptStopPuRecrod:
	case enBatOptStartWiper:
	case enBatOptStopWiper:
	case enBatOptStartPresetCall:
	case enBatOptStopPuCall:
		{
			pcCuBatOptList = &g_cCuiDataMgr.m_cPuBatOptRequestList;
		}
		break;
	case enBatOptTypeInvalid:
	default:
		{
			CMS_ASSERT(0 && "CBatOptTask::GetBatOptType 未知的类型");
			TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 未知的操作类型！\n", this->GetTaskNO());
		}
		break;
	}

	if (pcCuBatOptList == NULL)
	{
		CMS_ASSERT(0 && "CBatOptTask::GetBatOptType pcCuBatOptList == NULL");
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] pcCuBatOptList == NULL！\n", this->GetTaskNO());
		return false;
	}

	TBatOptInfo tBatOptInfo;
	if (pcCuBatOptList->GetFrontBatOptInfo(tBatOptInfo) == false)
	{
		CMS_ASSERT(0 && "没有取到第一条任务？");
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 没有取到第一条任务！\n", this->GetTaskNO());
		return false;
	}

	if (tBatOptInfo.m_nTaskNo != this->GetTaskNO())
	{
		CMS_ASSERT(0 && "TaskNo不正确？");
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 与记录的TaskNo[%u]不一致！\n", this->GetTaskNO(), tBatOptInfo.m_nTaskNo);
		return false;
	}

	return true;
}


CPeAgentBase * CBatOptTask::GetPeAgentInServiceState(const string &strPeName)
{
	string strPeUri = strPeName + "@" + GetDomainName();
	CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strPeUri, GetInstance());

	if (pcPeAgent == NULL)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] [%s]没有对应的agent\n", this->GetTaskNO(), strPeName.c_str());
		return NULL;
	}

	if (CPeAgent::Service != pcPeAgent->GetState())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] [%s]不在服务态\n", this->GetTaskNO(), strPeName.c_str());
		return NULL;
	}

	if (pcPeAgent->m_strSessionID.empty())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] [%s]在服务态,但是session为空？\n", this->GetTaskNO(), strPeName.c_str());
		return NULL;
	}

	return pcPeAgent;
}

bool CBatOptTask::Get3aAgentInOnLineStat(C3acAgent* &pc3acAgent)
{
	pc3acAgent = NULL;
	string str3asName = string(DEV_TYPE_3AS) + "@" + GetDomainName();

	pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(str3asName, GetInstance());
	if (pc3acAgent == NULL)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 3aTask不存在?[%s]\n", this->GetTaskNO(), m_strBatOptReq.c_str());
		return false;
	}

	if (pc3acAgent->GetState() != C3acAgent::ONLINE_STATE)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 3A状态[%s]!", this->GetTaskNO(), pc3acAgent->GetStateName(pc3acAgent->GetState()));
		return false;
	}

	return true;
}

bool CBatOptTask::GetUserPri()
{
	if (m_pc3acAgent == NULL)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "TaskNo[%u] m_pc3acAgent == NULL!", this->GetTaskNO());
		return false;
	}

	string strUserId = m_cuBatOptReq.GetUserId();
	if ( strUserId.empty() )
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "TaskNo[%u] strUserId.empty()!", this->GetTaskNO());
		return false;
	}

	CUserAccount *pcUserAccout = m_pc3acAgent->m_cUserMgr.FindUserAccountByUserID(GetNameWithoutDomainFromURI(strUserId));
	if (pcUserAccout == NULL)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "TaskNo[%u] pcUserAccout == NULL", this->GetTaskNO());
		return false;
	}

	m_userPri = pcUserAccout->GetUserPri();

	return true;
}

u32 CBatOptTask::QueryUserInfo()
{
	string strCuserID = m_cuBatOptReq.GetUserId();
	if (strCuserID.empty())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] UserId IsEmpty\n", this->GetTaskNO());
		return PROCMSG_FAIL;
	}
	//向3AS发送查询用户请求，以便更新用户信息
	CUserQryReq cQryReq;

	m_dwQueryEvent = cQryReq.GetEventId();
	m_strQueryEvent = cQryReq.GetEventStr();

	cQryReq.SetStartEntryIndex(0);
	cQryReq.SetExpectEntryNum(1);
	CUserInfoQueryCondition condition;
	condition.SetUserID(GetNameWithoutDomainFromURI(strCuserID));
	cQryReq.SetUserInfoQueryCondition(condition);

	if (!m_strQryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
		m_strQryTag.clear();
	}

	//将QueryTag插入到会话表中，等待NTF
	g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
	m_strQryTag = cQryReq.GetQueryTag();

	return PostMsgReqTo3AS<CUserQryReq>(cQryReq);
}

u32 CBatOptTask::QueryUserDevVsByGrp()
{
	CUserDevGrpVidsrcQryReq cQryReq;
	m_dwQueryEvent = cQryReq.GetEventId();
	m_strQueryEvent = cQryReq.GetEventStr();

	cQryReq.SetDevtreeId(m_cuBatOptReq.GetTreeId());

	string strCuserID = m_cuBatOptReq.GetUserId();
	if (!strCuserID.empty())
	{
		cQryReq.SetCuserId(GetNameWithoutDomainFromURI(strCuserID));
	}

	string strGroupID = m_cuBatOptReq.GetGroupId();
	if (!strGroupID.empty())
	{
		cQryReq.SetDevGrpId(GetNameWithoutDomainFromURI(strGroupID));
	}
	
	cQryReq.SetRecursive(m_cuBatOptReq.IsRecursive());

	u32 dwRet = PostMsgReqTo3AS<CUserDevGrpVidsrcQryReq>(cQryReq);
	if (dwRet != PROCMSG_OK)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 向TAS发送消息失败 [%s] 批量操作[%s]\n",
			this->GetTaskNO(), cQryReq.ToXml().c_str(), m_cuBatOptReq.ToXml().c_str());

		return PROCMSG_FAIL;
	}

	if (!m_strQryTag.empty())
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQryTag);
		m_strQryTag.clear();
	}

	//将QueryTag插入到会话表中，等待NTF
	g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
	m_strQryTag = cQryReq.GetQueryTag();

	return PROCMSG_OK;
}

bool CBatOptTask::SendBatOpt()
{
	u32 dwRet = PROCMSG_OK;

	string strDevId, strDomainId;
	int nVsId = -1;
	CPrivilege devPri, vsPri;

	do 
	{
		if (GetFrontFromVsData(strDevId, strDomainId, nVsId, devPri, vsPri) == false )
		{
			TASKLOG(NORMAL_TASK, PROGRESS_LEV, "TaskNo[%u]已经没有视频源了，数据都处理完了！\n", this->GetTaskNO());
			return false;
		}

		string strDomainName = m_pc3acAgent->GetDomainNameByDomainID(strDomainId);
		if (strDomainName.empty())
		{
			PopFrontFromNtfData();

			TASKLOG(NORMAL_TASK, EVENT_LEV, "TaskNo[%u] DomainId[%s]转换域名失败 DevId[%s] VSChnId[%d] OptType[%d] 处理失败 -- 级联退网了？\n",
				this->GetTaskNO(), strDomainId.c_str(), strDevId.c_str(), nVsId, m_eBatOptType);
			continue;
		}

		TChannel tChannel;
		tChannel.SetDevUri(strDevId + "@" + strDomainName);
		tChannel.SetChnNO(nVsId);

		TASKLOG(NORMAL_TASK, PROGRESS_LEV, "TaskNo[%u] 处理批量任务: OptType[%d] Channel[%s]\n",
			this->GetTaskNO(), m_eBatOptType, tChannel.ToXml().c_str());

		dwRet = PROCMSG_OK;

		switch (m_eBatOptType)
		{
		case enBatOptStartPlatRecord:
		case enBatOptStopPlatRecord:
			{
				dwRet = SendOptForPlatRec(tChannel);
			}
			break;
		case enBatOptStartPuRecord:
		case enBatOptStopPuRecrod:
			{
				dwRet = SendOptForPuRec(tChannel, devPri);
			}
			break;
		case enBatOptStartWiper:
		case enBatOptStopWiper:
			{
				dwRet = SendOptForPuPTZ(tChannel, vsPri);
			}
			break;
		case enBatOptStartPresetCall:
			{
				dwRet = SendOptForPuPTZ(tChannel, vsPri);
			}
			break;
		case enBatOptStopPuCall:
			{
				dwRet = SendPotForStopPuCall(tChannel);
			}
			break;
		default:
			{
				CMS_ASSERT(0 && "CBatOptTask::SendBatOpt 准备发送消息但不知道批量操作类型？");
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 参数传错了？m_eBatOptType 不支持的类型\n", this->GetTaskNO());
				return PROCMSG_DEL;
			}
			break;
		}
		
		PopFrontFromNtfData();

		if(dwRet == PROCMSG_OK)
		{
			return true;
		}
		
		if(dwRet == PROCMSG_DEL)
		{
			return false;
		}

		TASKLOG(NORMAL_TASK, PROGRESS_LEV, "TaskNo[%u] OptType[%d] VSChn[%s]消息发送失败了，取下一个视频源发送\n",
				this->GetTaskNO(), m_eBatOptType, tChannel.ToXml().c_str());
	} while (true);

	return false;
}

u32 CBatOptTask::SendOptForPlatRec(const TChannel &tChannel)
{
	s32 dwRet = PROCMSG_DEL;

	switch (m_eBatOptType)
	{
	case enBatOptStartPlatRecord:
	case enBatOptStopPlatRecord:
		{
			CCuRcsSetRecordTaskReq cReq;
			cReq.SetPuChn(tChannel);
			cReq.SetRecType(RCS_RECTYPE_HANDLE);

			if (m_eBatOptType == enBatOptStartPlatRecord)
			{
				cReq.SetIsStart(true);
			}
			else
			{
				cReq.SetIsStart(false);
			}

            std::string rcsId, rcsDomain;
            //if( CMS_SUCCESS == g_cCuiRedisSdk.SelectRcs( rcsId, rcsDomain ) )
            //{
            //    TRedisPlatRecType tRecType(RedisPlatRecType_Manual);
            //    TRedisPuRecUnit tPuRecUnit(TSipURI(cReq.GetPuChn().GetDevUri()).GetUser(), cReq.GetPuChn().GetChnNO());
            //    ECMS_ERRORCODE eRet = cReq.GetIsStart() 
            //        ? g_cCuiRedisSdk.AddPlatRecTask(tRecType, tPuRecUnit)
            //        : g_cCuiRedisSdk.DelPlatRecTask(tRecType, tPuRecUnit);

            //    if (!m_strSendBatOptReq.empty())
            //    {
            //        CRcsSetRecordTaskReq cReq;
            //        cReq.FromXml(m_strSendBatOptReq);

            //        WriteLog(m_cuBatOptReq.GetSession(), cReq.GetPuChn(), cReq.GetEventId(), eRet);
            //    }

            //    if (++m_wCurrTimeProcedRecNum >= g_cCuiConfig.GetBatTaskRecNumPerTime())
            //    {
            //        //本次执行数量超过单次执行限制，则启用定时器缓冲CPU
            //        SetTimer(CUI_BAT_OPT_TASK_TIMER, g_cCuiConfig.GetBatTaskIntervalPerTime());
            //        m_wCurrTimeProcedRecNum = 0;
            //    } 
            //    else
            //    {
            //        u32 dwTaskNO = GetTaskNO();
            //        OspPost(CUIAPP_DAEMON, CUI_BAT_OPT_TASK_NTF, &dwTaskNO, sizeof(dwTaskNO));
            //    }

            //    if (CMS_SUCCESS != eRet)
            //    {
            //        TASKLOG(NORMAL_TASK, ERROR_LEV, "Opt PlatRecTask fail\n");
            //        dwRet = PROCMSG_FAIL;
            //    } 
            //    else
            //    {
            //        dwRet = PROCMSG_OK;
            //    }
            //    break;
            //}
            //else
            {			
                CPeAgentBase *peRcsAgent = GetPeAgentInServiceState(DEV_TYPE_RCS);
                if (peRcsAgent == NULL)
                {
                    return PROCMSG_DEL;
                }

                cReq.SetSession(peRcsAgent->m_strSessionID);

                m_strSendBatOptReq = cReq.ToXml().c_str();

                TSipURI tRcsUri;
                CStreamDecentrationAdapter::FindRcsUri( tRcsUri, GetDomainName() );
                dwRet = FakePostMsgReq<CCuRcsSetRecordTaskReq>(cReq, tRcsUri.GetURIString() );
                if (dwRet != PROCMSG_OK)
                {
                    dwRet = PROCMSG_FAIL;
                    break;
                }
            }
		}
		break;
	default:
		{
			CMS_ASSERT(0 && "错误的类型");
			TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] optType[%d] 不是平台录像操作\n", this->GetTaskNO(), m_eBatOptType);
			dwRet = PROCMSG_DEL;
		}
		break;
	}

	return dwRet;
}

u32 CBatOptTask::SendOptForPuRec(const TChannel &tChannel, CPrivilege &devPri)
{
	s32 dwRet = PROCMSG_DEL;

	switch (m_eBatOptType)
	{
	case enBatOptStartPuRecord:
		{
			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] cmuSession是空的，cmu不在线？可以退出了\n", this->GetTaskNO());
				return PROCMSG_DEL;
			}

			CPuRecordStartReq cReq;
			cReq.SetDevChn(tChannel);
			
			if (devPri.HasPri(CEncoderPrivilege::en_Vod_Rec) == false)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s]对设备[%s]没有开启前端录像的权限，继续操作下一个视频源\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), tChannel.ToXml().c_str());
				dwRet = PROCMSG_DEL;
				break;
			}

			cReq.SetSession(g_cCuiDataMgr.m_cCuiState.m_strSession);

			m_strSendBatOptReq = cReq.ToXml().c_str();

			dwRet = PostMsgReqToCMU<CPuRecordStartReq>(cReq);
			if (dwRet != PROCMSG_OK)
			{
				dwRet = PROCMSG_FAIL;
				break;
			}
		}
		break;
	case enBatOptStopPuRecrod:
		{
			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] cui session是空的，cmu不在线，直接退出\n", this->GetTaskNO());
				return PROCMSG_DEL;
			}

			CPuRecordStopReq cReq;
			cReq.SetDevChn(tChannel);

			if (devPri.HasPri(CEncoderPrivilege::en_Vod_Rec) == false)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s]对设备[%s]没有停止前端录像的权限，继续操作下一个视频源\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), tChannel.ToXml().c_str());
				dwRet = PROCMSG_DEL;
				break;
			}

			cReq.SetSession(g_cCuiDataMgr.m_cCuiState.m_strSession);

			m_strSendBatOptReq = cReq.ToXml().c_str();

			dwRet = PostMsgReqToCMU<CPuRecordStopReq>(cReq);
			if (dwRet != PROCMSG_OK)
			{
				dwRet = PROCMSG_FAIL;
				break;
			}
		}
		break;
	default:
		{
			CMS_ASSERT(0 && "错误的类型");
			TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] OptType[%d] 不是前端录像操作\n", this->GetTaskNO(), m_eBatOptType);
			dwRet = PROCMSG_DEL;
		}
		break;
	}

	return dwRet;
}

u32 CBatOptTask::SendOptForPuPTZ(const TChannel &tChannel, CPrivilege &vsPri)
{
	s32 dwRet = PROCMSG_DEL;

	switch (m_eBatOptType)
	{
	case enBatOptStartPresetCall:
		{
			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] cui session是空的，cmu不在线，直接退出\n", this->GetTaskNO());
				dwRet = PROCMSG_DEL;
				break;
			}

			int ptzCtrlPri = vsPri.GetPtzPriLevel();
			if (ptzCtrlPri <= 0)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s]没有视频源[%s]的PTZ权限，无法操作，继续操作下一个视频源\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), tChannel.ToXml().c_str());
				dwRet = PROCMSG_FAIL;
				break;
			}

			if (m_cuBatOptReq.GetParamList().empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 开始载入预置位，但是没有参数，直接退出了！\n", this->GetTaskNO());
				dwRet = PROCMSG_DEL;
				break;
			}

			CPuPtzCtrlReq cPtzCtrlReq;
			cPtzCtrlReq.SetUserName(m_cuBatOptReq.GetUserName() + "(From IP:0.0.0.0)"); // 批量操作，用户可能已经下线了！
			cPtzCtrlReq.SetDevChn(tChannel);

			TCamCtrl tCamCtrl;
			vector<int> ptzParamList;

			stringstream sstream(m_cuBatOptReq.GetParamList().front().c_str());
			int nPresetNum = 0;
			sstream >> nPresetNum;
			ptzParamList.push_back(nPresetNum);
			
			tCamCtrl.SetCommandId(CAM_COMMAND_PRESETCALL);
			tCamCtrl.SetParams(ptzParamList);
			tCamCtrl.SetPriLevel(ptzCtrlPri);
			tCamCtrl.SetHoldTime(CMS_PTZ_DEFAULT_HOLD_TIME);
			cPtzCtrlReq.SetCamCtrl(tCamCtrl);

			TChannel t3aDevChannel = tChannel;
			t3aDevChannel.SetDevUri(GetNameWithoutDomainFromURI(tChannel.GetDevUri()));

			CCuPtzLockInfo tPtzLockInfo;
			if (m_pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(t3aDevChannel, tPtzLockInfo))
			{
				if (tPtzLockInfo.IsValid())
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 设备[%s]被锁定，无法操作，继续操作下一个视频源\n", this->GetTaskNO(), tChannel.ToXml().c_str());
					dwRet = PROCMSG_DEL;
					break;
				}
			}

			cPtzCtrlReq.SetSession(g_cCuiDataMgr.m_cCuiState.m_strSession);

			m_strSendBatOptReq = cPtzCtrlReq.ToXml().c_str();

			dwRet = PostMsgReqToCMU<CPuPtzCtrlReq>(cPtzCtrlReq);
			if (dwRet != PROCMSG_OK)
			{
				dwRet = PROCMSG_FAIL;
			}
		}
		break;
	case enBatOptStartWiper:
	case enBatOptStopWiper:
		{
			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] cui session是空的，cmu不在线，直接退出\n", this->GetTaskNO());
				dwRet = PROCMSG_DEL;
				break;
			}

			int ptzCtrlPri = vsPri.GetPtzPriLevel();
			if (ptzCtrlPri <= 0)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s]没有视频源[%s]的PTZ权限，无法操作，继续操作下一个视频源\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), tChannel.ToXml().c_str());
				dwRet = PROCMSG_FAIL;
				break;
			}

			CPuPtzCtrlReq cPtzCtrlReq;
			cPtzCtrlReq.SetUserName(m_cuBatOptReq.GetUserName() + "(From IP:0.0.0.0)"); // 批量操作，用户可能已经下线了！
			cPtzCtrlReq.SetDevChn(tChannel);

			TCamCtrl tCamCtrl;
			vector<int> ptzParamList;

			if (m_eBatOptType == enBatOptStartWiper)
			{
				tCamCtrl.SetCommandId(CAM_COMMAND_STARTWIPER);

				if (m_cuBatOptReq.GetParamList().empty())
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 开启雨刷，但是没参数，直接退出了！\n", this->GetTaskNO());
					dwRet = PROCMSG_DEL;
					break;
				}

				stringstream sstream(m_cuBatOptReq.GetParamList().front().c_str());
				int nSpeed = 0;
				sstream >> nSpeed;
				ptzParamList.push_back(nSpeed);
			}
			else
			{
				tCamCtrl.SetCommandId(CAM_COMMAND_STOPWIPER);
			}

			tCamCtrl.SetParams(ptzParamList);
			tCamCtrl.SetPriLevel(ptzCtrlPri);
			tCamCtrl.SetHoldTime(CMS_PTZ_DEFAULT_HOLD_TIME);
			cPtzCtrlReq.SetCamCtrl(tCamCtrl);

			TChannel t3aDevChannel = tChannel;
			t3aDevChannel.SetDevUri(GetNameWithoutDomainFromURI(tChannel.GetDevUri()));

			CCuPtzLockInfo tPtzLockInfo;
			if (m_pc3acAgent->m_cPtzlockInfoList.GetCuPtzLockInfoByVs(t3aDevChannel, tPtzLockInfo))
			{
				if (tPtzLockInfo.IsValid())
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 设备[%s]被锁定，无法操作，继续操作下一个视频源\n", this->GetTaskNO(), tChannel.ToXml().c_str());
					dwRet = PROCMSG_DEL;
					break;
				}
			}
			
			cPtzCtrlReq.SetSession(g_cCuiDataMgr.m_cCuiState.m_strSession);

			m_strSendBatOptReq = cPtzCtrlReq.ToXml().c_str();

			dwRet = PostMsgReqToCMU<CPuPtzCtrlReq>(cPtzCtrlReq);
			if (dwRet != PROCMSG_OK)
			{
				dwRet = PROCMSG_FAIL;
			}
		}
		break;
	default:
		{
			CMS_ASSERT(0 && "错误的类型");
			TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] OptType[%d] 不是前端PTZ操作\n", this->GetTaskNO(), m_eBatOptType);
			dwRet = PROCMSG_DEL;
		}
		break;
	}

	return dwRet;
}

CSipTask * CBatOptTask::GetCallPuTask(CCuAgent *pcCuAgent, int nPlayId)
{
	if (NULL == pcCuAgent)
	{
		return NULL;
	}

	u32 dwTaskId = INVALID_TASKNO;
	if (!pcCuAgent->m_cCallPuTaskSet.Find(nPlayId, dwTaskId))
	{
		return NULL;
	}

	if (INVALID_TASKNO == dwTaskId)
	{
		return NULL;
	}

	return (CSipTask *)(GetInstance()->FindTask(dwTaskId));
}

u32 CBatOptTask::SendPotForStopPuCall(const TChannel &tChannel)
{
	s32 dwRet = PROCMSG_OK;

	switch (m_eBatOptType)
	{
	case enBatOptStopPuCall:
		{
			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] cui session是空的，cmu不在线，直接退出\n", this->GetTaskNO());
				dwRet = PROCMSG_DEL;
				break;
			}

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_cuBatOptReq.GetSession(), this->GetInstance());
			if ( pcCuAgent == NULL )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s] userId[%s] session[%s]已经退出，不需要停止音频呼叫，直接结束\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), m_cuBatOptReq.GetUserId().c_str(), m_cuBatOptReq.GetSession().c_str());
				dwRet = PROCMSG_DEL;
				break;
			}

			if (m_cuBatOptReq.GetParamList().empty()
				|| m_cuBatOptReq.GetParamList().front().empty())
			{
				dwRet = PROCMSG_DEL;
				break;
			}

			stringstream sstream(m_cuBatOptReq.GetParamList().front());
			int nPlayId = -1;
			sstream >> nPlayId;

			if (GetCallPuTask(pcCuAgent, nPlayId) == NULL)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] 用户[%s] userId[%s] session[%s]没有发起音频呼叫，直接结束\n",
					this->GetTaskNO(), m_cuBatOptReq.GetUserName().c_str(), m_cuBatOptReq.GetUserId().c_str(), m_cuBatOptReq.GetSession().c_str());
				dwRet = PROCMSG_DEL;
				break;
			}

			CStopThirdPartyCallReq cStopThirdPartyCallReq;

			TChannel tCuChn;
			tCuChn.SetDevUri(m_cuBatOptReq.GetSession());
			tCuChn.SetChnNO(nPlayId);
            const TChannel &tPuChn = tChannel;
			cStopThirdPartyCallReq.SetSrcChn(tCuChn);//音频呼叫 码流 cu => pu ,cu是src
			cStopThirdPartyCallReq.SetDstChn(tPuChn);

			if (g_cCuiDataMgr.m_cCuiState.m_strSession.empty())
			{
				dwRet = PROCMSG_DEL;
				break;
			}

			cStopThirdPartyCallReq.SetSession(g_cCuiDataMgr.m_cCuiState.m_strSession);
			
			m_strSendBatOptReq = cStopThirdPartyCallReq.ToXml().c_str();

            TSipURI tDstUri;
            ECMS_ERRORCODE err = CMS_SUCCESS;
            bool bResult = CStreamDecentrationAdapter::FindPuiUri( tDstUri, tPuChn.GetDevUri(), pcCuAgent->m_tCmuURI, &err );
            if(     !bResult 
                &&  err == ERR_REDISSDK_PU_IS_OFFLINE )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "pu[%s]不在线 \n", tPuChn.GetDevUri().c_str() );
				dwRet = PROCMSG_FAIL;
                break;
            }

            cStopThirdPartyCallReq.SetSession( tPuChn.GetDevUri() );

            //dwRet = PostMsgReqToCMU<CStopThirdPartyCallReq>(cStopThirdPartyCallReq);
            if ( PROCMSG_OK != PostMsgReq<CCuStopThirdPartyCallReq>(cStopThirdPartyCallReq, tDstUri) )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "发送SIP消息失败[%s]\n", cStopThirdPartyCallReq.ToXml().c_str());
				dwRet = PROCMSG_FAIL;
                break;
            }
		}
		break;
	default:
		{
			CMS_ASSERT(0 && "错误的类型");
			TASKLOG(NORMAL_TASK, ERROR_LEV, "TaskNo[%u] OptType[%d] 不是停止音频呼叫操作\n", this->GetTaskNO(), m_eBatOptType);
			dwRet = PROCMSG_DEL;
		}
		break;
	}

	return dwRet;
}

void CBatOptTask::SaveVsData(vector<CUserDevGrpVidSrcInfo> &vsData)
{
	for (size_t nIndex = 0; nIndex < vsData.size(); nIndex++)
	{
		if (vsData[nIndex].GetDevDomainID().empty()
			|| vsData[nIndex].GetDevId().empty()
			|| vsData[nIndex].GetDevCapPri().id != DEVICE_CAP_VIDEOSOURCE
			|| vsData[nIndex].GetDevCapPri().privileges.empty())
		{
			continue;
		}

		TDeviceCapPrivilege &tDevCapPri = vsData[nIndex].GetDevCapPri();
		for (map<int, CBitsContainer>::iterator iter = tDevCapPri.privileges.begin(); iter != tDevCapPri.privileges.end();)
		{
			if (iter->first < 0)
			{
				tDevCapPri.privileges.erase(iter++);
			}
			else
			{
				iter++;
			}
		}

		if ( tDevCapPri.privileges.empty() )
		{
			continue;
		}

		m_allDevVsInGroup.push_back(vsData[nIndex]);
		m_nTotal += vsData[nIndex].GetDevCapPri().privileges.size();
	}
}
bool CBatOptTask::GetFrontFromVsData(string &strDevId, string &strDomainId, int &nVsId, CPrivilege &devPri, CPrivilege &vsPri)
{
	// 只有查不到视频源的时候才会返回失败
	while (!m_allDevVsInGroup.empty())
	{
		TDeviceCapPrivilege &tDevCapPri = m_allDevVsInGroup.front().GetDevCapPri();
		do
		{
			if (tDevCapPri.privileges.empty())
			{
				break;
			}

			map<int, CBitsContainer>::iterator iter = tDevCapPri.privileges.begin();
			if (iter->first >= 0)
			{
				break;
			}

			tDevCapPri.privileges.erase(iter);
		} while (true);

		if (tDevCapPri.privileges.empty())
		{
			m_allDevVsInGroup.pop_front();
			continue;
		}

		strDevId = m_allDevVsInGroup.front().GetDevId();
		strDomainId = m_allDevVsInGroup.front().GetDevDomainID();
		devPri.SetPri(m_allDevVsInGroup.front().GetDevPrivilege());

		map<int, CBitsContainer>::iterator firstIter = m_allDevVsInGroup.front().GetDevCapPri().privileges.begin();
		nVsId = firstIter->first;
		vsPri.SetPri(firstIter->second);

		return true;
	}

	return false;
}

void CBatOptTask::PopFrontFromNtfData()
{
	while (!m_allDevVsInGroup.empty())
	{
		TDeviceCapPrivilege &tDevCapPri = m_allDevVsInGroup.front().GetDevCapPri();
		if (tDevCapPri.privileges.empty())
		{
			m_allDevVsInGroup.pop_front();
			continue;
		}

		map<int, CBitsContainer>::iterator iter = tDevCapPri.privileges.begin();
		tDevCapPri.privileges.erase(iter);

		if (tDevCapPri.privileges.empty())
		{
			m_allDevVsInGroup.pop_front();
		}

		return;
	}

	return ;
}

void CBatOptTask::WriteLog(const string& m_strCuSession, TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri /*= string()*/)
{
	CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strCuSession,
		string(DEV_TYPE_UAS) + "@" + GetDomainName(),
		enOprType,
		dwErrorCode,
		strLogDesc,
		strDevUri
		);
	pcTask->InitStateMachine();
	pcTask->StartSendLog();
}

void CBatOptTask::WriteLog(const string& m_strCuSession,const TChannel& tDevChn, const u32 dwEvent, const u32 dwErrorCode)
{
	CGetPuData *pcTask = new CGetPuData(GetInstance(), m_strCuSession, tDevChn, dwErrorCode, m_strSendBatOptReq, dwEvent, GetDomainName());
	pcTask->InitStateMachine();
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}
