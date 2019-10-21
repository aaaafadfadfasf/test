#include "querydevgrouptask.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/dps/dps_proto.h"

CDevGroupQueryTask::CDevGroupQueryTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_pRsp = NULL;
    m_pc3acAgent = NULL;
    m_dwReqEvent = 0;
	m_dwTotalTime = 0;
    m_pcCuAgent->AddSubTask(this);
}

CDevGroupQueryTask::~CDevGroupQueryTask()
{
    TerminateTask();
    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }

    if ( NULL != m_pRsp )
    {
        delete m_pRsp;
        m_pRsp = NULL;
    }
}

void CDevGroupQueryTask::TerminateTask()
{
    m_pcCuAgent->DelSubTask(this);
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

void CDevGroupQueryTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CDevGroupQueryTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CDevGroupQueryTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

	CStateProc cWaitDpsRspProc;
	cWaitDpsRspProc.ProcMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitDpsRspProcMsg;
	cWaitDpsRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CDevGroupQueryTask::WaitDpsRspTimerPoll;
	cWaitDpsRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CDevGroupQueryTask::WaitDpsRspProcMsg;
	AddRuleProc(WAIT_DPS_RSP_STATE, cWaitDpsRspProc);

    NextState(NOT_QUERY_STATE);
}

u32 CDevGroupQueryTask::NotQueryProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[CDevGroupQueryTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case TAS_DEVICE_GROUP_QRY_REQ:
        {
            CCuUserDevGroupQryReq *pcReq = (CCuUserDevGroupQryReq *)pcMsg->content;

			string strCuserID;
			pcReq->GetDeviceGroupInfoQueryCondition().GetCuserID(strCuserID);
			if (!strCuserID.empty())
			{
				pcReq->GetDeviceGroupInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strCuserID));
			}

			string strGroupID;
			pcReq->GetDeviceGroupInfoQueryCondition().GetDeviceGroupID(strGroupID);
			if (!strGroupID.empty())
			{
				pcReq->GetDeviceGroupInfoQueryCondition().SetDeviceGroupID(GetNameWithoutDomainFromURI(strGroupID));
			}

			string strParentID;
			pcReq->GetDeviceGroupInfoQueryCondition().GetDeviceParentGroupID(strParentID);
			if (!strParentID.empty())
			{
				pcReq->GetDeviceGroupInfoQueryCondition().SetDeviceParentGroupID(GetNameWithoutDomainFromURI(strParentID));
			}

			return this->OnNotQueryMsg<CDeviceGroupQryReq, CCuUserDevGroupQryRsp>(*pcReq);
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

u32 CDevGroupQueryTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CDevGroupQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
            CMS_ASSERT(NULL != m_ptRecvNode);
            m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            m_strRspOfErr = m_pRsp->ToXml();
			if ( m_ptRecvNode != NULL )
			{
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				m_ptRecvNode = NULL;
			}
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        break;
    case TAS_DEVICE_GROUP_QRY_RSP:
        {
			m_dwTotalTime = GetCurStateHoldTime();
            return this->OnWaitRspMsg<CDeviceGroupQryRsp, CCuUserDevGroupQryRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
        {
            if((m_dwReqEvent+2) == pcMsg->event)
            {
                if(pcMsg->content != NULL)
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

u32 CDevGroupQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
        CMS_ASSERT(NULL != m_ptRecvNode);
		if (m_ptRecvNode != NULL )
		{
			m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			m_ptRecvNode = NULL;
		}
		NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CDevGroupQueryTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
        {
            while(!m_VecNtfReqList.empty())
            {
                CMessage cMsg;
                cMsg.event = m_dwReqEvent+2;
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

u32 CDevGroupQueryTask::ProcNotifyMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CDevGroupQueryTask[TaskNO %u]收到服务器[%s]的消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
        {
			CDeviceGroupQryNtfReq cNtf;
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

			// 直接保存到rsp中，待返回给CU

			if (m_pcCuAgent != NULL)
			{
				AddNtfForCu(cNtf.GetDeviceGroupInfo(), cNtf.GetDeviceGroupHasDevice(), m_pRsp->GetUserDevGroupInfo());
				AddNtfForDps(cNtf.GetDeviceGroupInfo(), m_devGroupIdList);

				m_pRsp->SetActualResultNum(m_pRsp->GetUserDevGroupInfo().size());
			}

			//判断是否后续还有Ntf
			if (cNtf.IsLastNtf())
			{
				if (!m_strQueryTag.empty())
				{
					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
					m_strQueryTag.clear();
				}

				m_dwTotalTime += GetCurStateHoldTime();

				if ( m_dwTotalTime >= DEFAULT_TIMEOUT_RES_TO_CU
					|| PROCMSG_OK != SendReqToDps())
				{
					if (NULL != m_ptRecvNode)
					{
						m_pRsp->SetErrorCode(CMS_SUCCESS);
						SendRspToCu<CCuUserDevGroupQryRsp>(*m_pRsp, m_ptRecvNode);
						m_ptRecvNode = NULL;
					}

					NextState(NOT_QUERY_STATE);
					delete this;
					break;
				}

				NextState(WAIT_DPS_RSP_STATE);
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

u32 CDevGroupQueryTask::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CDevGroupQueryTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]. m_ptRecvNode is %s\n",
            GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), (NULL == m_ptRecvNode)? "NULL":"NotNULL");
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
		if (NULL != m_ptRecvNode)
		{
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			m_ptRecvNode = NULL;
		}
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CDevGroupQueryTask::WaitDpsRspProcMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CDevGroupQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
			CMS_ASSERT(NULL != m_ptRecvNode);
			m_pRsp->SetErrorCode(CMS_SUCCESS);
			m_strRspOfErr = m_pRsp->ToXml();
			if ( m_ptRecvNode != NULL )
			{
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				m_ptRecvNode = NULL;
			}
			NextState(NOT_QUERY_STATE);
			delete this;
		}
		break;
	case DPS_FILTER_EMPTY_DEVICE_GROUP_RSP:
		{
			CDPSFilterEmptyDeviceGroupRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() == CMS_SUCCESS && m_pcCuAgent != NULL)
			{
				CompareNtfFromDps(m_pRsp->GetUserDevGroupInfo(), cRsp.DeviceGroupIdList());
			}

			if (NULL != m_ptRecvNode)
			{
				SendRspToCu<CCuUserDevGroupQryRsp>(*m_pRsp, m_ptRecvNode);
				m_ptRecvNode = NULL;
			}

			NextState(NOT_QUERY_STATE);
			delete this;
			break;
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

u32 CDevGroupQueryTask::WaitDpsRspTimerPoll()
{
	if ( (m_dwTotalTime + GetCurStateHoldTime()) >= DEFAULT_TIMEOUT_RES_TO_CU)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CDevGroupQueryTask[NO: %u]等待DPS消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]. m_ptRecvNode is %s\n",
			GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), (NULL == m_ptRecvNode) ? "NULL" : "NotNULL");
		m_pRsp->SetErrorCode(CMS_SUCCESS);
		m_strRspOfErr = m_pRsp->ToXml();

		if (NULL != m_ptRecvNode)
		{
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			m_ptRecvNode = NULL;
		}

		if ( !m_strQueryTag.empty() )
		{
			g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
			m_strQueryTag.clear();
		}

		NextState(NOT_QUERY_STATE);
		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

void CDevGroupQueryTask::AddNtfForCu(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, const vector<bool> &hasDevList, vector<CCuUserDeviceGroupInfo> &devGroupIdList)
{
	vector<CDeviceGroupInfo>::const_iterator pItem = cCuDevGroupInfoList.begin();
	u32 nIndex = 0;
	while (pItem != cCuDevGroupInfoList.end())
	{
		CCuUserDeviceGroupInfo cCuDeviceGroupInfo;
		cCuDeviceGroupInfo = *pItem;

		C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, this->GetInstance());
		if (pc3acAgent != NULL)
		{
			if (!cCuDeviceGroupInfo.GetDeviceGroupID().empty())
			{
				cCuDeviceGroupInfo.SetDeviceGroupID(cCuDeviceGroupInfo.GetDeviceGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(cCuDeviceGroupInfo.GetDomainUUID()));
			}

			if (!cCuDeviceGroupInfo.GetDeviceParentGroupID().empty())
			{
				cCuDeviceGroupInfo.SetDeviceParentGroupID(cCuDeviceGroupInfo.GetDeviceParentGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(cCuDeviceGroupInfo.GetParentDomainUUID()));
			}
		}

		if ( nIndex < hasDevList.size() )
		{
			cCuDeviceGroupInfo.SetGroupHasDev(hasDevList[nIndex]);
		}
		else
		{
			cCuDeviceGroupInfo.SetGroupHasDev(true);
		}

		devGroupIdList.push_back(cCuDeviceGroupInfo);

		pItem++;
		nIndex++;
	};
}

void CDevGroupQueryTask::AddNtfForDps(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, std::vector<string> &devGroupIdList)
{
	for (int nIndex = 0; nIndex < cCuDevGroupInfoList.size(); nIndex++)
	{
		devGroupIdList.push_back(cCuDevGroupInfoList[nIndex].GetDeviceGroupID());
	}
}

void CDevGroupQueryTask::CompareNtfFromDps(vector<CCuUserDeviceGroupInfo> &cCuDevGroupInfoList, const set<string> &devGroupIdList)
{
	set<string>::const_iterator devGroupIter;

	for (int nIndex = 0; nIndex < cCuDevGroupInfoList.size(); nIndex++)
	{
		// 给cu的DevGroupId带有域名，要先去掉域名进行对比
		string devGoupId = GetNameWithoutDomainFromURI(cCuDevGroupInfoList[nIndex].GetDeviceGroupID());
		if ( devGoupId.empty())
		{
			continue;
		}

		devGroupIter = devGroupIdList.find(devGoupId);
		if ( devGroupIter == devGroupIdList.end() )
		{
			cCuDevGroupInfoList[nIndex].SetEmptyDevGroup(true);
		}
	}
}

CPeAgentBase * CDevGroupQueryTask::GetPeAgentInServiceState(const string &strPeName)
{
	string strDpsUri = strPeName + "@" + GetDomainName();
	CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strDpsUri, GetInstance());

	if (pcPeAgent == NULL)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "没有DPS的agent,taskNo[%d]\n", GetTaskNO());
		return NULL;
	}

	if (CPeAgent::Service != pcPeAgent->GetState())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "DPS不在服务态,taskNo[%d]\n", GetTaskNO());
		return NULL;
	}

	if (pcPeAgent->m_strSessionID.empty())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "DPS在服务态,但是session为空？taskNo[%d]\n", GetTaskNO());
		return NULL;
	}

	return pcPeAgent;
}


u32 CDevGroupQueryTask::SendReqToDps()
{
	CPeAgentBase *peDpsAgent = GetPeAgentInServiceState(DEV_TYPE_DPS);
	if ( peDpsAgent == NULL )
	{
		return PROCMSG_FAIL;
	}
	
	CDPSFilterEmptyDeviceGroupReq cReq;

	cReq.SetSession(peDpsAgent->m_strSessionID);
	cReq.DeviceGroupIdList() = m_devGroupIdList;

	string userId = GetNameWithoutDomainFromURI(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	if ( userId.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "向DPS发送消息,userId是空的？taskNo[%d], event[%d]\n", GetTaskNO(), cReq.GetEventId());
		return PROCMSG_FAIL;
	}
	cReq.UserId() = userId;
	
	if ( m_pRsp->GetUserDevGroupInfo().empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "没有设备组ID，就不用向DPS发送消息了！！taskNo[%d], event[%d]\n", GetTaskNO(), cReq.GetEventId());
		return PROCMSG_FAIL;
	}

	// treeId可为空，且不带域名不用转换
	cReq.TreeId() = m_pRsp->GetUserDevGroupInfo().front().GetDevtreeId();

	u32 dwRet = FakePostMsgReq(cReq, peDpsAgent->m_tPeURI);
	if (dwRet != PROCMSG_OK)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "向DPS发送消息失败,taskNo[%d], event[%d]\n", GetTaskNO(), cReq.GetEventId());
	}

	return dwRet;
}
