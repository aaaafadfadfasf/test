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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "��δ�����NTF��Ϣ[%s],ֱ�ӻظ�\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
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
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[COfflineVsQueryTask]�յ�CU�û�[%s]�Ự[%s]������[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
	{
	case CU_CUI_OFFLINEDEV_QRY_REQ:
		{
			bool isFirstReq = m_strReq.empty();
			CuQueryOffLineVSReq(pcMsg);

			if ( !isFirstReq )
			{
				// ��idle״̬�£��ڶ��������Ժ������ֻ��������ɲ�ѯ��
				NextState(NOT_QUERY_STATE);
			}
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
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

			// ����35�볬ʱ״̬
			if ( GetCurStateHoldTime() > CUI_QUERY_CMU_OFFLINE_VS_OVER_TIME )
			{
				if ( m_hasSendToCu )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitQuerySendToCuPoll 35�볬ʱɾ����TASK\n");
					m_ptRecvNode = NULL;
					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
					m_strQueryTag.clear();
					return TIMERPOLL_DEL;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "35�볬ʱӦɾ����TASK,����û��CU��Ϣδ���أ���ʱ�����κβ���\n");
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
    TASKLOG(NORMAL_TASK, EVENT_LEV, "COfflineVsQueryTask[TaskNO %u]�յ�������[%s]��Ӧ����Ϣ[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�[OSP_SIP_MSG_PROC_FAIL]��Ϣ\n");
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
            CMS_ASSERT( 0 && "COfflineVsQueryTask::WaitRspProcMsg�����ܳ��ֻỰ����Ϣ" );
        }
        break;
	case CU_CUI_OFFLINEDEV_QRY_REQ:
		{
			// ��������²�Ӧ���յ����
			TASKLOG(NORMAL_TASK, ERROR_LEV, "WaitRspProcMsg CU_CUI_OFFLINEDEV_QRY_REQ ��������²�Ӧ���յ����\n");

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
			// �����쳣��� ����Ϊ0�ʹ���
		}
		break;
	case CMU_PU_OFFLINEDEV_QRY_NTF_REQ:
		{
			// �쳣�����յ���NTF��Ϣ
			SaveOfflineVSNtf(pcMsg);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 COfflineVsQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]�ȴ�Ӧ����Ϣ[%s--%d]��ʱ:SeqNum[%d]\n", 
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
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
            CMS_ASSERT( 0 && "COfflineVsQueryTask::WaitNotifyProcMsg�����ܳ���SIP�쳣" );
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

			// �����յ���NTF��Ϣ
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
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
        TASKLOG(NORMAL_TASK, ERROR_LEV, "COfflineVsQueryTask[NO: %u]�ȴ�NTF��Ϣ��ʱSeqNum[%d]��ֱ����CU�ظ�Ӧ����Ϣ[%s--%d]. m_ptRecvNode is %s\n",
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
	// �յ�ƽ̨���ص�RSP
	// ����ѯ������Ϣ
	CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	COffLineDevQryRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);

	// �浽���ݿ���
	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);
	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "session����domainName�ǿյ�\n");
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
		// ���û�в������豸����ô��������֪ͨ����
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

	TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�Ntf��Ϣ[%s--%d]������Ntf�к���[%d]����¼\n",
		cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
		cNtf.GetEntryNum());

	TASKLOG(NORMAL_TASK, EVENT_LEV, "PROGRESS_LEV=%s", cNtf.ToXml().c_str());

	//�Ȼظ�NTF֪ͨ
	COffLineDevQryNtfRsp cRsp;
	cRsp.SetEvent(pcMsg->event+1);
	cRsp.SetSeqNum(cNtf.GetSeqNum());
	cRsp.SetSession(cNtf.GetSession());
	PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

	//�洢����
	CCuOffLineDevQryReq cReq;
	cReq.FromXml(m_strReq);

	string cuSession = cReq.GetSession();
	string domainName = cReq.GetDomanName();

	if ( cuSession.empty()|| domainName.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "session����domainName�ǿյ�!\n" );
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
		TASKLOG(NORMAL_TASK, EVENT_LEV, "cuSession����domainName�ǿյģ�\n");
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
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CuQueryOffLineVSReq session����domainName�ǿյģ�\n");
		return;
	}

	m_hasSendToCu = false;

	if ( m_strReq.empty() )
	{
		// ��û�����������
		COffLineDevQryReq cReq = *pcReq;
		cReq.SetEvent(CMU_PU_OFFLINEDEV_QRY_REQ);
		cReq.SetStartEntryIndex(0);
		cReq.SetExpectEntryNum(0);

		OnNotQueryMsg<COffLineDevQryReq, CCuOffLineDevQryRsp>(cReq,SERVER_CMU);

		// ����taskNo
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetCuQueryTag(pcReq->GetCuQueryTag());
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetTaskQueryTag(cReq.GetQueryTag());
		g_mapDevVideoSrcStatList[cReq.GetSession()][cReq.GetDomanName()].SetTaskNo(digital_string(GetTaskNO()));
		
		m_strReq = pcReq->ToXml();
	}
	else
	{
		m_strReq = pcReq->ToXml(); // CuQueryOfflineVSRsp ��Ҫ�õ�m_strReq,��ǰ���
		CuQueryOffLineVSRsp();
	}
}

void COfflineVsQueryTask::CuQueryOffLineVSRsp()
{
	// �ж��Ƿ����յ�CU������û�з���
	if ( !m_strReq.empty() &&  !m_hasSendToCu )
	{
		CCuOffLineDevQryReq cReq;
		cReq.FromXml(m_strReq);

		string cuSession = cReq.GetSession();
		string domainName = cReq.GetDomanName();

		if ( cuSession.empty()|| domainName.empty() )
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CuQueryOffLineVSRsp session����domainName�ǿյ�\n");
			return;
		}

		// �Ѿ����������,��ֱ��ȡ����,��Ҫ�ı�״̬
		int queryMaxNum = cReq.GetStartEntryIndex() + cReq.GetExpectEntryNum();
		int vsStatListSize = g_mapDevVideoSrcStatList[cuSession][domainName].GetVideoSrcStatListSize();

		bool isFinished = g_mapDevVideoSrcStatList[cuSession][domainName].IsFinished();

		TASKLOG(NORMAL_TASK, EVENT_LEV, "cu����������ܴ�С%d, ����������ܴ�С:%d, ������=%d\n", queryMaxNum, vsStatListSize, (int)isFinished);

		if ( queryMaxNum <= vsStatListSize // ��������ֵС�ڵ����յ����ݵ����ֵ
			|| isFinished // ����б��Ѿ��������
			)
		{
			// ����Ѿ����㹻���ݷ��أ���ֱ�ӷ��ظ�CU
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

			// ��Ǹ������Ѿ����ظ�CU��
			m_hasSendToCu = true;
		}
		else
		{
			// û�����ݾͲ�������
			TASKLOG(NORMAL_TASK, EVENT_LEV, "û���㹻���ݣ������κβ���\n");
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

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuQueryOffLineVsWait �Ѿ��յ�CU����,��Ҫ���ؽ��\n");

	CuQueryOffLineVSRsp();

	if ( !m_hasSendToCu )
	{
		// �������û�����ݷ��ظ�CU����ҪУ���Ƿ�ʱ�����ش������CU
		// �ж������Ƿ��㹻���ظ�CU,�����ʱ�����ش���
		if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]�ȴ�Ӧ����Ϣ[%s--%d]��ʱ:SeqNum[%d]\n", 
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

