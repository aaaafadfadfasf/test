#include "long_query_task.h"
#include "ssn_taskbase.h"

CLongQueryTask::CLongQueryTask( CSsnTaskbase* pcSsnTask )
	:CSipTask(pcSsnTask->GetInstance()), m_pcSsnTask(pcSsnTask)
{
	m_tSipUri = m_pcSsnTask->GetClientSipURI();
	m_pcQryRsp = NULL;
	m_strObjName = "CLongQueryTask";
	m_bHasRecvLastNtf = false;
	m_bWaitNtfRsp = false;
}

CLongQueryTask::~CLongQueryTask()
{
	while (!m_cQryNtfLst.empty())
	{
		delete m_cQryNtfLst.front();
		m_cQryNtfLst.pop_front();
	}
	if (NULL != m_pcQryRsp)
	{
		delete m_pcQryRsp;
	}

	m_pcSsnTask->ReverseDeleteQueryTaskRecord(this);
}

const char* CLongQueryTask::GetStateName( u32 dwState ) const
{
	if (dwState == enStateIdle) return "enStateIdle";
	if (dwState == enStateQuery) return "enStateQuery";
	if (dwState == enStateTransRsp) return "enStateTransRsp";
	if (dwState == enStateTransNtf) return "enStateTransNtf";
	return "unknown";
}

void CLongQueryTask::InitStateMachine()
{
	DoNextState(enStateIdle);

	CStateProc cProc;

	cProc.ProcMsg = (CTask::PFProcMsg)&CLongQueryTask::OnIdle;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(enStateIdle, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CLongQueryTask::OnQuery;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CLongQueryTask::QueryTimerPoll;
	AddRuleProc(enStateQuery, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CLongQueryTask::OnTransRsp;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CLongQueryTask::TransRspTimerPoll;
	AddRuleProc(enStateTransRsp, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CLongQueryTask::OnTransNtf;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CLongQueryTask::TransNtfTimerPoll;
	AddRuleProc(enStateTransNtf, cProc);
}

u32 CLongQueryTask::OnIdle( CMessage *const pcMsg )
{
	if (Query(pcMsg))
	{
		DoNextState(enStateQuery);
		return PROCMSG_OK;
	}

	Log(WARNING_LEV, "CLongQueryTask::OnIdle delete\n");
	return PROCMSG_DEL;
}

u32 CLongQueryTask::OnQuery( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db != pcMsg->event)
	{
		CMS_ASSERT(false);
		Log(WARNING_LEV, "CLongQueryTask::OnQuery delete(invalid message)\n");
		return PROCMSG_DEL;
	}

	DoNextState(enStateQuery); // �յ��˲�ѯӦ�����½���״̬���µ�ǰ״̬Holdtime

	ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
	CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

	bool bIsRsp = IsRspType(pcTrans->GetEventId());

	// ���:Rsp��Ntf
	if (bIsRsp)
	{
		CMS_ASSERT(m_pcQryRsp == NULL);
		m_pcQryRsp = pcTrans;
		CQueryRspBase *pcBase = reinterpret_cast<CQueryRspBase*>(pcTrans->GetMsgObjAddr());
		*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // �Լ�����֮���ÿգ�����CCtrl�Ų���Delete����

		Log(PROGRESS_LEV, "CLongQueryTask::OnQuery rsp's entryNum:%d, ntf num:%u\n", 
			pcBase->GetEntryNum(), m_cQryNtfLst.size());
		pcBase->SetQueryTag(m_strQueryTag);

		DoNextState(enStateTransRsp);
		if (DoTransfer(true))
		{
			if (pcBase->GetErrorCode() != CMS_SUCCESS || pcBase->GetEntryNum() == 0)
			{
				// ��ѯ�����������Ϊ0, ���跢��Ntf��ֱ��ɾ����
				return PROCMSG_DEL;
			}

			DoNextState(enStateTransNtf);
			if (!m_cQryNtfLst.empty()&&!m_bWaitNtfRsp)
			{
				DoTransfer(false);
			}
			
			return PROCMSG_OK;
		}
		else
		{
			Log(WARNING_LEV, "CLongQueryTask::OnQuery delete\n");
			return PROCMSG_DEL;
		}
	}
	else
	{
		CQueryNtfReqBase *pcBase = reinterpret_cast<CQueryNtfReqBase*>(pcTrans->GetMsgObjAddr());
		pcBase->SetQueryTag(m_strQueryTag);
		m_cQryNtfLst.push_back(pcTrans);
		*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // �Լ�����֮���ÿգ�����CCtrl�Ų���Delete����
	}

	return PROCMSG_OK;
}

u32 CLongQueryTask::OnTransRsp( CMessage *const pcMsg )
{
	// ���״̬û��ʲô������Ҫ��
	CMS_ASSERT(false);
	Log(WARNING_LEV, "CLongQueryTask::OnTransRsp delete\n");
	return PROCMSG_DEL;
}

u32 CLongQueryTask::OnTransNtf( CMessage *const pcMsg )
{
	Log(PROGRESS_LEV, "CLongQueryTask::OnTransferNtf recv event[%u-%s]\n", 
		pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());

	switch(pcMsg->event)
	{
	case  msgTaskMsgTransfer4Db:
		{
			DoNextState(enStateTransNtf); 

			ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
			CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

			// ��ѯ���Ntf
			if (!IsRspType(pcTrans->GetEventId()))
			{
				CQueryNtfReqBase *pcBase = reinterpret_cast<CQueryNtfReqBase*>(pcTrans->GetMsgObjAddr());
				pcBase->SetQueryTag(m_strQueryTag);
				m_cQryNtfLst.push_back(pcTrans);
				if (pcBase->IsLastNtf())
				{
					m_bHasRecvLastNtf = true;
				}
				*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // �Լ�����֮���ÿգ�����CCtrl�Ų���Delete����
				if (!m_bWaitNtfRsp)
				{
					DoTransfer(false);
				}
			}
		}
		break;
	case TAS_CUSER_DEVICE_QRY_NTF_RSP:
		{
			// �յ�NtfRsp��Ϣ, �ж����к��Ƿ�Ϊ������Ϣ��Ӧ��
			string strSeqNum = GetSipSeqNO(reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody());
			const CTaskMsgTransferBase *pcTrans = m_cQryNtfLst.front();

			if (reinterpret_cast<CEventReq*>(pcTrans->GetMsgObjAddr())->GetSeqNum()
				== atoi(strSeqNum.c_str()))
			{
				DoNextState(enStateTransNtf); // �յ���NtfReq��Ӧ�����½���״̬���µ�ǰ״̬Holdtime
				m_bWaitNtfRsp = false;
				delete pcTrans;
				m_cQryNtfLst.pop_front();
				
				if (m_cQryNtfLst.empty())
				{
					if (m_bHasRecvLastNtf)
					{
						Log(PROGRESS_LEV, "CLongQueryTask::OnTransferNtf finished transfer\n");
						return PROCMSG_DEL;
					}
				}
				else
				{
					DoTransfer(false);
				}
			}
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
		{
			// SIP�㴦�����򵥷���fail
			return PROCMSG_FAIL;
		}
		break;
	default:
		{
			Log(PROGRESS_LEV, "CLongQueryTask::OnTransferNtf recv unknown event[%u-%s]\n", 
				pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CLongQueryTask::TransRspTimerPoll()
{
	if (GetCurStateHoldTime() < 15)
	{
		return TIMERPOLL_DONE;
	}
	else
	{
		Log(WARNING_LEV, "TransRsp timeout");
		return TIMERPOLL_DEL;
	}

	// ������ѯ������ط�����
	//if (++m_dwDelayTimes > 3)
	//{
	//    return TIMERPOLL_DEL; // �ۼ�3���޷�����QryRsp��ɾ������
	//}

	//if (DoTransfer(true)) // �ط�
	//{
	//    CDbQueryResultBase* pcBase = reinterpret_cast<CDbQueryResultBase*>(m_pcQryRsp);
	//    if (pcBase->GetErrorCode() != CMS_SUCCESS || pcBase->GetTotalEntryNum() == 0)
	//    {
	//        return TIMERPOLL_DEL;
	//    }

	//    DoNextState(enStateTransNtf);
	//    DoTransfer(false);
	//}
	//return TIMERPOLL_DONE;
}

u32 CLongQueryTask::TransNtfTimerPoll()
{
	if (GetCurStateHoldTime() < 60*2) 
	{
		return TIMERPOLL_DONE;
	}
	else
	{
		CQueryRspBase* pcResult = reinterpret_cast<CQueryRspBase*>(m_pcQryRsp->GetMsgObjAddr());
		Log(WARNING_LEV, "TransNtf timeout, Rsp EntryNum:%u, leave Ntf Num:%u\n", (u32)pcResult->GetEntryNum(), (u32)m_cQryNtfLst.size());
		return TIMERPOLL_DEL;
	}

	// ������ѯ������ط�����
	//if (++m_dwDelayTimes > 60*2)
	//{
	//    return TIMERPOLL_DEL; // �ۼ�120�δ���Ӧ�𲻷��ؾ�ɾ������
	//}
	//if (GetPollCounter() < 5)
	//{
	//    return TIMERPOLL_DONE; // ��ѭ5�βų����ط�
	//}
	//else
	//{
	//    ClearPollCounter();
	//}

	//DoTransfer(false); // �ط�
	//return TIMERPOLL_DONE; 
}

u32 CLongQueryTask::QueryTimerPoll( )
{
	return (GetCurStateHoldTime() > 60*5)? TIMERPOLL_DEL: TIMERPOLL_DONE; // �ۼ�300�����ݿ��ѯ�����ؾ�ɾ������
}

bool CLongQueryTask::DoTransfer(bool bRsp)
{
	bool bRet;
	string strMsg;
	if (bRsp)
	{
		const CTaskMsgTransferBase *pcTrans = m_pcQryRsp;
		strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
		bRet = (PROCMSG_OK == PostMsgRsp(pcTrans->GetSipTransId(), strMsg));
	}
	else
	{
		const CTaskMsgTransferBase *pcTrans = m_cQryNtfLst.front();

		// ��ª�����⴦�����˵��豸��������Ĺ������ [10/16/2015 pangubing]
		if (TAS_DEVICE_GROUP_QRY_NTF_REQ==pcTrans->GetEventId()&&!m_pcSsnTask->GetIsNeedDevGrpGbId())
		{
			Log(EVENT_LEV, "CLongQueryTask m_tSipUri[%s] need cut group gb id.\n",m_tSipUri.GetURIString().c_str());
			CDeviceGroupQryNtfReq* pNtfReq = reinterpret_cast<CDeviceGroupQryNtfReq*>(pcTrans->GetMsgObjAddr());
			vector<CDeviceGroupInfo> &infos=pNtfReq->GetDeviceGroupInfo();
			vector<CDeviceGroupInfo>::iterator itr=infos.begin();
			for (;itr!=infos.end();itr++)
			{
				itr->GetDeviceGroupName()=CutDevGrpGbId(itr->GetDeviceGroupName());
			}

			strMsg = pNtfReq->ToXml();
		}
		else
		{
			strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
		}

		bRet = (PROCMSG_OK == PostMsgReq(strMsg, m_tSipUri));
		if (bRet)
		{
			m_bWaitNtfRsp = true;
		}
	}

	if (!bRet)
	{
		Log(WARNING_LEV, "CLongQueryTask[%s-%u] send message failed, queryTag:%s\n", 
			GetObjName(), GetTaskNO(), m_strQueryTag.c_str());
	}
	else
	{
		CQueryRspBase *pcRsp = static_cast<CQueryRspBase*>(m_pcQryRsp->GetMsgObjAddr());
		Log(PROGRESS_LEV, "CLongQueryTask[%s-%u] send message success, queryTag:%s, bRsp:%s, entryNum:%d\n", 
			GetObjName(), GetTaskNO(), m_strQueryTag.c_str(), bRsp? "yes":"no", pcRsp->GetEntryNum());
	}

	return bRet;
}

void CLongQueryTask::DoNextState( u32 enState )
{
	NextState(enState);
}

#define DECODE_SIP_AND_POST2DbTaskPool(msgid, msgtype) \
    case msgid:\
{\
	CTaskMsgTransfer<msgtype> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);\
	pcSipMsg->GetMsgBody(cTrans.GetMsgObj());\
	m_strQueryTag = static_cast<CQueryReqBase*>(cTrans.GetMsgObjAddr())->GetQueryTag(); \
	bRet = PostTaskMsgTransfer2DbTaskPool(cTrans);\
}break

bool CLongQueryTask::Query( CMessage *const pcMsg )
{
	bool bRet = false;
	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	switch(pcMsg->event)
	{
		/*DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_QRY_REQ, CDeviceQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_TVWALL_QRY_REQ, CTvwallQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MODEL_QRY_REQ, CDeviceModelQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ, CDeviceCapIndexNameQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MANUFACTURER_QRY_REQ, CDeviceManufacturerQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ, CCuserDevicefullinfoAndPrivilegeQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_QRY_REQ, CUserQryReq);*/
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_QRY_REQ, CUserDeviceQryReq);
		/*DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_QRY_REQ, CDeviceGroupQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_QRY_REQ, CDeviceGroupDeviceQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_QRY_REQ, CUserGroupQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_QRY_REQ, CUserGroupDeviceQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DOMAIN_RELATION_QRY_REQ, CDomainRelationQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ, CDeviceAutoAssignToUserQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ, CDeviceAutoAssignToUserGroupQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVTREE_QRY_REQ, CDevTreeQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUPPATH_QRY_REQ, CDeviceGrpPathQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_PTZLOCK_QRY_REQ, CPtzLockQryReq);*/
	default:
		{
			Log(ERROR_LEV, "CLongQueryTask::Query recv unknow message, [%u-%s]\n", static_cast<u32>(pcMsg->event),
				OspExtEventDesc(pcMsg->event).c_str());
		}
		break;
	}
	if (bRet)
	{
		m_strObjName.append("[");
		m_strObjName.append(OspExtEventDesc(pcMsg->event));
		m_strObjName.append("]");
	}

	return bRet;
}
