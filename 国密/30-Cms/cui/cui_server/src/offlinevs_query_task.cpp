#include "offlinevs_query_task.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/md5.h"
#include "cms/utility/flow_limit_counter.h"

COfflineVsQueryTask::COfflineVsQueryTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
	m_hasSendToCu = true;
    m_pRsp = NULL;
    m_pc3acAgent = NULL;
    m_dwReqEvent = 0;
    m_pcCuAgent->AddSubTask(this);
}

COfflineVsQueryTask::~COfflineVsQueryTask()
{
	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);
	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( !cuSession.empty() && !domainName.empty() )
	{
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].Clear();
	}

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

void COfflineVsQueryTask::TerminateTask()
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

void COfflineVsQueryTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = (CSipTask::PFTimerPoll)&COfflineVsQueryTask::WaitQueryTimerPoll;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&COfflineVsQueryTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&COfflineVsQueryTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&COfflineVsQueryTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

    NextState(NOT_QUERY_STATE);
}

u32 COfflineVsQueryTask::NotQueryProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[COfflineVsQueryTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
	{
	case CU_CUI_OFFLINEDEV_QRY_REQ:
		{
			bool isFirstReq = m_strReq.empty();
			CuQueryOffLineVSReq(pcMsg);

			if ( !isFirstReq )
			{
				// 在idle状态下，第二次请求及以后的请求只能是在完成查询后
				NextState(NOT_QUERY_STATE);
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

u32 COfflineVsQueryTask::WaitQueryTimerPoll()
{
	switch( m_dwReqEvent )
	{
	case CMU_PU_OFFLINEDEV_QRY_REQ:
		{
			CuQueryOffLineVsWait();

			// 设置35秒超时状态
			if ( GetCurStateHoldTime() > CUI_QUERY_CMU_OFFLINE_VS_OVER_TIME )
			{
				if ( m_hasSendToCu )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitQuerySendToCuPoll 35秒超时删除本TASK\n");
					m_ptRecvNode = NULL;
					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
					m_strQueryTag.clear();
					return TIMERPOLL_DEL;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "35秒超时应删除本TASK,但还没有CU消息未返回，暂时不做任何操作\n");
				}
			}
		}
		break;
	default:
		{
		}
		break;
	}
	return TIMERPOLL_DONE;
}

u32 COfflineVsQueryTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "COfflineVsQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
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
            CMS_ASSERT( 0 && "COfflineVsQueryTask::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
	case CU_CUI_OFFLINEDEV_QRY_REQ:
		{
			// 正常情况下不应该收到这个
			TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitRspProcMsg CU_CUI_OFFLINEDEV_QRY_REQ 正常情况下不应该收到这个\n");

			CCuOffLineDevQryReq *pcReq = (CCuOffLineDevQryReq*)pcMsg->content;
			CCuOffLineDevQryRsp m_Rsp;
			m_Rsp.SetSession(pcReq->GetSession());
			m_Rsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);

			SendRspToCu(m_Rsp, m_ptRecvNode);
			NextState(WAIT_RESPONSE_STATE);
		}
		break;
	case CMU_PU_OFFLINEDEV_QRY_RSP:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitRspProcMsg CMU_PU_OFFLINEDEV_QRY_RSP\n");
			CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);
			GetOfflineVSRspFromCMU(pcMsg);
			// 考虑异常情况 数据为0和错误
		}
		break;
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
		{
			// 异常处理收到的NTF消息
			SaveOfflineVSNtf(pcMsg);
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

u32 COfflineVsQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
	}
    return TIMERPOLL_DONE;
}

u32 COfflineVsQueryTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
	case CU_CUI_OFFLINEDEV_QRY_REQ:
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
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

u32 COfflineVsQueryTask::ProcNotifyMsg(CMessage *const pcMsg)
{
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "COfflineVsQueryTask::WaitNotifyProcMsg不可能出现SIP异常" );
        }
        break;
	case CU_CUI_OFFLINEDEV_QRY_REQ:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitNotifyProcMsg CU_CUI_OFFLINEDEV_QRY_REQ\n");
			CuQueryOffLineVSReq(pcMsg);
			NextState(WAIT_NOTIFY_STATE);
		}
		break;
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
		{
			SaveOfflineVSNtf(pcMsg);

			// 处理收到的NTF消息
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			COffLineDevQryNtfReq cNtf;
			pcOspSipMsg->GetMsgBody(cNtf);

			if ( cNtf.IsLastNtf() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitNotifyProcMsg CMU_PU_OFFLINEDEV_QRY_NTF_REQ NOT_QUERY_STATE\n");
				NextState(NOT_QUERY_STATE);
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitNotifyProcMsg CMU_PU_OFFLINEDEV_QRY_NTF_REQ WAIT_NOTIFY_STATE\n");
				NextState(WAIT_NOTIFY_STATE);
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

u32 COfflineVsQueryTask::WaitNotifyTimerPoll()
{
	switch( m_dwReqEvent )
	{
	case CMU_PU_OFFLINEDEV_QRY_REQ:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitNotifyTimerPoll CMU_PU_OFFLINEDEV_QRY_REQ\n");
			CuQueryOffLineVsWait();
		}
		break;
	default:
		{
		}
		break;
	}

    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "COfflineVsQueryTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]. m_ptRecvNode is %s\n",
            GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), (NULL == m_ptRecvNode)? "NULL":"NotNULL");
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        if (NULL != m_ptRecvNode) SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void COfflineVsQueryTask::GetOfflineVSRspFromCMU(CMessage *const pcMsg)
{
	// 收到平台返回的RSP
	// 填充查询到的信息
	CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	COffLineDevQryRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);

	// 存到数据库中
	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);
	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "session或者domainName是空的\n");
		return;
	}

	//TASKLOG(NORMAL_TASK, ERROR_LEV, "GetOfflineVSRspFromCMU m_strReq=%s\n", m_strReq.c_str());
	TASKLOG(NORMAL_TASK, EVENT_LEV, "cmuRsp=%s\n", cRsp.ToXml().c_str());

	g_mapDevVideoSrcStatList[cuSession][domainName].SetOnlineDeviceNum(cRsp.GetOnlineDevNum());
	g_mapDevVideoSrcStatList[cuSession][domainName].SetIdleDeviceNum(cRsp.GetIdleDevNum());
	g_mapDevVideoSrcStatList[cuSession][domainName].SetOfflineDeviceNum(cRsp.GetOfflineDevNum());
	g_mapDevVideoSrcStatList[cuSession][domainName].SetErrorCode(cRsp.GetErrorCode());

	if ( cRsp.GetIdleDevNum() == 0 && cRsp.GetOfflineDevNum() == 0 )
	{
		// 如果没有不在线设备，那么不会再有通知到来
		g_mapDevVideoSrcStatList[cuSession][domainName].SetFinished();
		NextState(NOT_QUERY_STATE);
	}
	else
	{
		NextState(WAIT_NOTIFY_STATE);
	}

	return;
}

void COfflineVsQueryTask::SaveOfflineVSNtf(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	COffLineDevQryNtfReq cNtf;
	pcOspSipMsg->GetMsgBody(cNtf);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
		cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
		cNtf.GetEntryNum());

	TASKLOG(NORMAL_TASK, EVENT_LEV, "PROGRESS_LEV=%s", cNtf.ToXml().c_str());

	//先回复NTF通知
	COffLineDevQryNtfRsp cRsp;
	cRsp.SetEvent(pcMsg->event+1);
	cRsp.SetSeqNum(cNtf.GetSeqNum());
	cRsp.SetSession(cNtf.GetSession());
	PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

	//存储数据
	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);

	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "session或者domainName是空的!\n" );
		return;
	}

	g_mapDevVideoSrcStatList[cuSession][domainName].AddVideoSrcStatList(cNtf.GetDevVideoSrcList(), cNtf.IsLastNtf());
}

void COfflineVsQueryTask::ClearOffLineVSData()
{
	if ( m_strReq.empty() )
	{
		return;
	}

	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);
	
	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "cuSession或者domainName是空的？\n");
		return;
	}

	g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].Clear();
}

void COfflineVsQueryTask::CuQueryOffLineVSReq(CMessage *const pcMsg)
{
	CMS_ASSERT( NULL != pcMsg);
	CCuOffLineDevQryReq *pcReq = (CCuOffLineDevQryReq*)pcMsg->content;
	string cuSession = pcReq->GetSession();
	string domainName = pcReq->GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CuQueryOffLineVSReq session或者domainName是空的？\n");
		return;
	}

	m_hasSendToCu = false;

	if ( m_strReq.empty() )
	{
		// 还没有请求过数据
		COffLineDevQryReq cReq = *pcReq;
		cReq.SetEvent(CMU_PU_OFFLINEDEV_QRY_REQ);
		cReq.SetStartEntryIndex(0);
		cReq.SetExpectEntryNum(0);

		OnNotQueryMsg<COffLineDevQryReq, CCuOffLineDevQryRsp>(cReq,SERVER_CMU);

		// 保存taskNo
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetCuQueryTag(pcReq->GetCuQueryTag());
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetTaskQueryTag(cReq.GetQueryTag());
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetTaskNo(digital_string(GetTaskNO()));
		
		m_strReq = pcReq->ToXml();
	}
	else
	{
		m_strReq = pcReq->ToXml(); // CuQueryOfflineVSRsp 需要用到m_strReq,提前添加
		CuQueryOffLineVSRsp();
	}
}

void COfflineVsQueryTask::CuQueryOffLineVSRsp()
{
	// 判断是否有收到CU请求且没有返回
	if ( !m_strReq.empty() &&  !m_hasSendToCu )
	{
		CCuOffLineDevQryReq cReq;
		cReq.FromXml(m_strReq);

		string cuSession = cReq.GetSession();
		string domainName = cReq.GetDomanName();

		if ( cuSession.empty()|| domainName.empty() )
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CuQueryOffLineVSRsp session或者domainName是空的\n");
			return;
		}

		// 已经请求过数据,就直接取数据,不要改变状态
		int queryMaxNum = cReq.GetStartEntryIndex() + cReq.GetExpectEntryNum();
		int vsStatListSize = g_mapDevVideoSrcStatList[cuSession][domainName].GetVideoSrcStatListSize();

		bool isFinished = g_mapDevVideoSrcStatList[cuSession][domainName].IsFinished();

		TASKLOG(NORMAL_TASK, EVENT_LEV, "cu请求的数据总大小%d, 缓存的数据总大小:%d, 完成情况=%d\n", queryMaxNum, vsStatListSize, (int)isFinished);

		if ( queryMaxNum <= vsStatListSize // 请求的最大值小于等于收到数据的最大值
			|| isFinished // 如果列表已经请求完成
			)
		{
			// 如果已经有足够数据返回，就直接返回给CU
			CCuOffLineDevQryRsp cuRsp = *(CCuOffLineDevQryRsp*)m_pRsp;

			int onlineVSNum = g_mapDevVideoSrcStatList[cuSession][domainName].GetOnlineDeviceNum();
			int idleVSNum = g_mapDevVideoSrcStatList[cuSession][domainName].GetIdleDeviceNum();
			int offlineVSNum = g_mapDevVideoSrcStatList[cuSession][domainName].GetOfflineDeviceNum();

			cuRsp.SetOnlineVideoSrcNum(onlineVSNum);
			cuRsp.SetIdleDevNum(idleVSNum);
			cuRsp.SetOfflineDevNum(offlineVSNum);

			//g_mapDevVideoSrcStatList[cuSession][domainName].PrintData();

			TDeviceVideoSrcIdList vsList;
			g_mapDevVideoSrcStatList[cuSession][domainName].GetVideoSrcStatList(cReq.GetStartEntryIndex()
				, cReq.GetExpectEntryNum()
				, vsList );
			cuRsp.SetDevVideoSrcList(vsList);
			cuRsp.SetErrorCode(g_mapDevVideoSrcStatList[cuSession][domainName].GetErrorCode());

			if ( m_ptRecvNode != NULL )
			{
				SendRspToCu(cuRsp, m_ptRecvNode);
			}

			// 标记该请求已经返回给CU了
			m_hasSendToCu = true;
		}
		else
		{
			// 没有数据就不做操作
			TASKLOG(NORMAL_TASK, EVENT_LEV, "没有足够数据，不做任何操作\n");
		}
	}
}

void COfflineVsQueryTask::CuQueryOffLineVsWait()
{
	if ( m_strReq.empty() )
	{
		return;
	}

	if ( m_hasSendToCu == true )
	{
		return;
	}

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuQueryOffLineVsWait 已经收到CU请求,需要返回结果\n");

	CuQueryOffLineVSRsp();

	if ( !m_hasSendToCu )
	{
		// 如果还是没有数据返回给CU，需要校验是否超时并返回错误码给CU
		// 判断数据是否足够返回给CU,如果超时，返回错误
		if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
			CMS_ASSERT(NULL != m_ptRecvNode);
			m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			//NextState(NOT_QUERY_STATE);
			//return TIMERPOLL_DEL;
		}
	}
}

