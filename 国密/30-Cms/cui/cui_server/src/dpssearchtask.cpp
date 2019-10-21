#include "culogintask.h"
#include "dpssearchtask.h"
#include "cuiproto.h"
#include "cuiapp.h"
#include <map>
using std::map;

//static map<string, string> s_mapCuSession2FileUri;

CDpsSearchInterAction::CDpsSearchInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(ptRecvNode != NULL);
	m_ptRecvNode = ptRecvNode;
	m_strCuSession = strCuSession;
	m_dwInviteTaskNo = INVALID_TASKNO;
}

CDpsSearchInterAction::~CDpsSearchInterAction()
{
	TerminateTask();
}

void CDpsSearchInterAction::TerminateTask()
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if (pcCuAgent == NULL)
	{
		return;
	}
	
	CDpsSearchTask *pcTask = (CDpsSearchTask *)GetDpsSearchTask(m_dwInviteTaskNo);
	if ( pcTask != NULL )
	{
		if (pcTask->m_pcImportActTask == this)
		{
			pcTask->m_pcImportActTask = NULL;
		}
	}
}

void CDpsSearchInterAction::InitStateMachine()
{
	CStateProc cReqRroc;
	cReqRroc.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchInterAction::ReqProcMsg;
	cReqRroc.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchInterAction::ReqTimerPoll;
	cReqRroc.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchInterAction::ReqProcMsg;
	AddRuleProc(Req, cReqRroc);
	CStateProc cRspProc;
	cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchInterAction::RspProcMsg;
	cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchInterAction::RspTimerPoll;
	cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchInterAction::RspProcMsg;
	AddRuleProc(Rsp, cRspProc);
	NextState(Req);
}
u32 CDpsSearchInterAction::ReqProcMsg(CMessage *const pcMsg)
{
	//COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�CU�û��Ự[%s]������[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	m_dwReqEventID = pcMsg->event;
	switch (pcMsg->event)
	{
	case CU_CUI_DPS_SEARCH_REQ:
		{
			CCuDPSSearchReq* pcReq = (CCuDPSSearchReq *)pcMsg->content;

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (pcCuAgent == NULL)
			{
				CCuDPSSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSSearchRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] ��ѯCCuAgentʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			string strPeUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();

			CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strPeUri, GetInstance());
			CMS_ASSERT(NULL != pcPeAgent);
			if (pcPeAgent == NULL || CPeAgent::Service != pcPeAgent->GetState())
			{
				CCuDPSSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PE_OFFLINE);
				SendRspToCu<CCuDPSSearchRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] DPS���ڷ���̬\n",
					m_strCuSession.c_str());

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDpsSearchTask *pcTask = new CDpsSearchTask(GetInstance(), m_strCuSession, this, strPeUri);
			if ( pcTask == NULL )
			{
				CCuDPSSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSSearchRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]���� DPS��ѯTASKʧ��\n",
					m_strCuSession.c_str());

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			pcTask->InitStateMachine();
			m_dwInviteTaskNo = pcTask->GetTaskNO();
			
			CDPSSearchReq cDPSSearchReq;
			cDPSSearchReq.SetUserId(pcCuAgent->m_pcUserAccount->m_tUserID);
			cDPSSearchReq.Key() = pcReq->GetKeywords();
			cDPSSearchReq.SetMaxNum(pcReq->GetMaxResultItemNum());
			cDPSSearchReq.SetMaxTimeMSec(pcReq->GetMaxSearchTimeMSec());
			
			if ( pcTask->StartDpsSearchReq(cDPSSearchReq) != PROCMSG_OK )
			{
				CCuDPSSearchRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PE_OFFLINE);
				SendRspToCu<CCuDPSSearchRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] DPS��������ʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			pcCuAgent->m_setDpsSearchTaskNo.insert(m_dwInviteTaskNo);

			NextState(Rsp);
		}
		break;
	case CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ:
		{
			CCuFileExportGetProcessExReq* pcReq = (CCuFileExportGetProcessExReq *)pcMsg->content;

			CCuFileExportGetProcessExRsp cCuRsp;

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (pcCuAgent == NULL)
			{
				cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileExportGetProcessExRsp>(cCuRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] ��ѯCCuAgentʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			u32 nTaskNo = pcReq->GetTaskNo();
			if (nTaskNo == INVALID_TASKNO || pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo) == pcCuAgent->m_setDpsSearchTaskNo.end())
			{
				cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileExportGetProcessExRsp>(cCuRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]��Ч\n",
					m_strCuSession.c_str(), nTaskNo);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDpsSearchTask* pcTask = (CDpsSearchTask *)(this->GetInstance()->FindTask(nTaskNo));
			if (pcTask == NULL)
			{
				cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileExportGetProcessExRsp>(cCuRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]������\n",
					m_strCuSession.c_str(), nTaskNo);

				// ɾ������taskno
				set<u32>::iterator item = pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo);
				if (item != pcCuAgent->m_setDpsSearchTaskNo.end())
				{
					pcCuAgent->m_setDpsSearchTaskNo.erase(item);
				}

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			cCuRsp.SetPercent(pcTask->GetProgressNtf().GetPercent());
			cCuRsp.SetErrorCode(CMS_SUCCESS);

			SendRspToCu<CCuFileExportGetProcessExRsp>(cCuRsp, m_ptRecvNode);
			dwRet = PROCMSG_DEL;
			delete this;
			break;
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_REQ:
		{
			CCuDPSGetResultReq* pcReq = (CCuDPSGetResultReq *)pcMsg->content;

			string strPeUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();
			CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strPeUri, GetInstance());
			CMS_ASSERT(NULL != pcPeAgent);
			if (pcPeAgent == NULL || CPeAgent::Service != pcPeAgent->GetState())
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PE_OFFLINE);
				SendRspToCu<CCuDPSGetResultRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] DPS���ڷ���̬\n",
					m_strCuSession.c_str());

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (pcCuAgent == NULL)
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] ��ѯCCuAgentʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			u32 nTaskNo = pcReq->GetTaskNo();
			if (nTaskNo == INVALID_TASKNO || pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo) == pcCuAgent->m_setDpsSearchTaskNo.end())
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]��Ч\n",
					m_strCuSession.c_str(), nTaskNo);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDpsSearchTask* pcTask = (CDpsSearchTask *)(this->GetInstance()->FindTask(nTaskNo));
			if (pcTask == NULL)
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]������\n",
					m_strCuSession.c_str(), nTaskNo);

				// ɾ������taskno
				set<u32>::iterator item = pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo);
				if ( item != pcCuAgent->m_setDpsSearchTaskNo.end())
				{
					pcCuAgent->m_setDpsSearchTaskNo.erase(item);
				}

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			
			CDPSGetResultReq cDPSGetResultReq;
			cDPSGetResultReq.SetStartEntryIndex(pcReq->GetStartEntryIndex());
			cDPSGetResultReq.SetExpectEntryNum(pcReq->GetExpectEntryNum());

			if ( pcTask->SendGetResultReq(cDPSGetResultReq) != PROCMSG_OK )
			{
				CCuDPSGetResultRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_STATE_ERROR);
				SendRspToCu<CCuDPSGetResultRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ������� ������Ϣʧ�� taskno[%d]\n",
					m_strCuSession.c_str(), nTaskNo);

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			m_cReq = *pcReq;

			m_dwInviteTaskNo = nTaskNo;
			pcTask->m_pcImportActTask = this;
			dwRet = PROCMSG_OK;

			NextState(Rsp);
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_REQ:
		{
			CCuDPSGetResultByGroupReq* pcReq = (CCuDPSGetResultByGroupReq *)pcMsg->content;

			string strPeUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();
			CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strPeUri, GetInstance());
			CMS_ASSERT(NULL != pcPeAgent);
			if (pcPeAgent == NULL || CPeAgent::Service != pcPeAgent->GetState())
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_PE_OFFLINE);
				SendRspToCu<CCuDPSGetResultByGroupRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] DPS���ڷ���̬\n",
					m_strCuSession.c_str());

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (pcCuAgent == NULL)
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultByGroupRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] ��ѯCCuAgentʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			u32 nTaskNo = pcReq->GetTaskNo();
			if (nTaskNo == INVALID_TASKNO || pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo) == pcCuAgent->m_setDpsSearchTaskNo.end())
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultByGroupRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]��Ч\n",
					m_strCuSession.c_str(), nTaskNo);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDpsSearchTask* pcTask = (CDpsSearchTask *)(this->GetInstance()->FindTask(nTaskNo));
			if (pcTask == NULL)
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSGetResultByGroupRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]������\n",
					m_strCuSession.c_str(), nTaskNo);

				// ɾ������taskno
				set<u32>::iterator item = pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo);
				if (item != pcCuAgent->m_setDpsSearchTaskNo.end())
				{
					pcCuAgent->m_setDpsSearchTaskNo.erase(item);
				}

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDPSGetResultByGroupReq cDPSGetResultByGroupReq;
			cDPSGetResultByGroupReq.SetStartEntryIndex(pcReq->GetStartEntryIndex());
			cDPSGetResultByGroupReq.SetExpectEntryNum(pcReq->GetExpectEntryNum());
			cDPSGetResultByGroupReq.GroupId() = pcReq->GetDevGroupId();
			cDPSGetResultByGroupReq.SetItemType(pcReq->GetType());

			if (pcTask->SendGetResultByGroupReq(cDPSGetResultByGroupReq) != PROCMSG_OK)
			{
				CCuDPSGetResultByGroupRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_STATE_ERROR);
				SendRspToCu<CCuDPSGetResultByGroupRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ������� ������Ϣʧ�� taskno[%d]\n",
					m_strCuSession.c_str(), nTaskNo);

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			m_cGroupReq = *pcReq;

			m_dwInviteTaskNo = nTaskNo;
			pcTask->m_pcImportActTask = this;
			dwRet = PROCMSG_OK;

			NextState(Rsp);
		}
		break;
	case CU_CUI_DPS_SEARCH_STOP_REQ:
		{
			CCuDPSSearchStopReq* pcReq = (CCuDPSSearchStopReq *)pcMsg->content;

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (pcCuAgent == NULL)
			{
				CCuDPSSearchStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSSearchStopRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] ��ѯCCuAgentʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			u32 nTaskNo = pcReq->GetTaskNo();
			if (nTaskNo == INVALID_TASKNO || pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo) == pcCuAgent->m_setDpsSearchTaskNo.end())
			{
				CCuDPSSearchStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSSearchStopRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]��Ч\n",
					m_strCuSession.c_str(), nTaskNo);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			CDpsSearchTask* pcTask = (CDpsSearchTask *)(this->GetInstance()->FindTask(nTaskNo));
			if (pcTask == NULL)
			{
				CCuDPSSearchStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuDPSSearchStopRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��ѯ��TaskNo[%d]������\n",
					m_strCuSession.c_str(), nTaskNo);

				// ɾ������taskno
				set<u32>::iterator item = pcCuAgent->m_setDpsSearchTaskNo.find(nTaskNo);
				if (item != pcCuAgent->m_setDpsSearchTaskNo.end())
				{
					pcCuAgent->m_setDpsSearchTaskNo.erase(item);
				}

				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			dwRet = pcTask->SendSearchBye();
			if (dwRet == PROCMSG_DEL)
			{
				CCuDPSSearchStopRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
				SendRspToCu<CCuDPSSearchStopRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]������Ϣʧ��\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			m_dwInviteTaskNo = nTaskNo;
			pcTask->m_pcImportActTask = this;
			NextState(Rsp);
			dwRet = PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CDpsSearchInterAction::ReqTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CDpsSearchInterAction::RspProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	if ( pcMsg == NULL )
	{
		OspPrintf(TRUE, FALSE, "pcMsg== NULL\n");
	}
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	//TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ���ϢӦ��[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
	//	m_strCuSession.c_str(),
	//	OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
	//	pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
		case OSP_SIP_MSG_PROC_FAIL:
		case OSP_SIP_DIALOG_TERMINATE:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "��CU�û��Ự[%s]����Ӧ����Ϣʧ�ܣ�������[%d]\n",
				m_strCuSession.c_str(), pcOspSipMsg->GetSipErrorCode());
			CEventRsp cRsp;
			cRsp.SetEvent(m_dwReqEventID + 1);
			cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
			SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case DPS_SEARCH_RSP:
		{
			CDPSSearchRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			CCuDPSSearchRsp cCuRsp;

			cCuRsp.SetTaskNo(m_dwInviteTaskNo);
			cCuRsp.SetTaskType(DEF_TASK_TYPE_DPS_SEARCH_TASK);
			cCuRsp.SetErrorCode(cRsp.GetErrorCode());

			if (INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
			{
				cCuRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			}

			SendRspToCu<CCuDPSSearchRsp>(cCuRsp, m_ptRecvNode);

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_RSP:
		{
			CCuDPSGetResultRsp cCuRsp;

			pcOspSipMsg->GetMsgBody(cCuRsp);

			cCuRsp.SetHeadFromReq(m_cReq);
			
			u32 nret = SendRspToCu<CCuDPSGetResultRsp>(cCuRsp, m_ptRecvNode);

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_RSP:
		{
			CCuDPSGetResultByGroupRsp cCuRsp;

			pcOspSipMsg->GetMsgBody(cCuRsp);

			cCuRsp.SetHeadFromReq(m_cGroupReq);

			u32 nret = SendRspToCu<CCuDPSGetResultByGroupRsp>(cCuRsp, m_ptRecvNode);

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			// �쳣�����е�����

			if ( m_ptRecvNode != NULL
				&& m_dwReqEventID == CU_CUI_DPS_SEARCH_STOP_REQ)
			{
				CCuDPSSearchStopRsp cCuStopRsp;
				SendRspToCu<CCuDPSSearchStopRsp>(cCuStopRsp, m_ptRecvNode);
			}
		
			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			if (pcOspSipMsg->GetMsgBodyLen() > 0)
			{
				CByeReq cByeReq;
				pcOspSipMsg->GetMsgBody(cByeReq);
				PostErrorMsg2Cu((u32)cByeReq.GetReason());
			}
			else
			{
				PostErrorMsg2Cu(ERR_CUI_DIALOG_TERMINATED);
			}
			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CDpsSearchInterAction::RspTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Ӧ����Ϣ��ʱ\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}


CTask * CDpsSearchInterAction::GetDpsSearchTask(u32 dwSearchTaskNo)
{
	if ( dwSearchTaskNo == INVALID_TASKNO )
	{
		return NULL;
	}

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if (pcCuAgent != NULL)
	{
		if ( pcCuAgent->m_setDpsSearchTaskNo.find(dwSearchTaskNo) == pcCuAgent->m_setDpsSearchTaskNo.end() )
		{
			return NULL;
		}
		
		return GetInstance()->FindTask(dwSearchTaskNo);
	}
	return NULL;
}

void CDpsSearchInterAction::PostErrorMsg2Cu(u32 dwErrorCode)
{
	switch (m_dwReqEventID)
	{
	case CU_CUI_DPS_SEARCH_REQ:
		{
			SendErrorMsg2Cu<CCuDPSSearchRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_DPS_GET_SEARCH_RESULT_REQ:
		{
			SendErrorMsg2Cu<CCuDPSGetResultRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_DPS_SEARCH_STOP_REQ:
		{
			SendErrorMsg2Cu<CCuDPSSearchStopRsp>(dwErrorCode);
		}
		break;
	default:
		{
			SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
		}
		break;
	}
}

//  -------  CDpsSearchTask  -------
CDpsSearchTask::CDpsSearchTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(!strPeUri.empty());
	CMS_ASSERT(NULL != pcExportActTask);
	m_pcImportActTask = pcExportActTask;
	m_tInviteID = INVALID_DIALOG_ID;
}
CDpsSearchTask::~CDpsSearchTask()
{
	TerminateTask();
}
void CDpsSearchTask::TerminateTask()
{
	if (m_tInviteID != INVALID_DIALOG_ID)
	{
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
		//OspSipReleaseDlg(m_tInviteID);
		m_tInviteID = INVALID_DIALOG_ID;
	}

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if (NULL != pcCuAgent)
	{
		set<u32>::iterator item = pcCuAgent->m_setDpsSearchTaskNo.find(this->GetTaskNO());
		if ( item != pcCuAgent->m_setDpsSearchTaskNo.end())
		{
			pcCuAgent->m_setDpsSearchTaskNo.erase(item);
		}
	}

	CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
	if (NULL != pcPeAgent)
	{
		set<u32>::iterator item = pcPeAgent->m_setDpsSearchTaskNo.find(this->GetTaskNO());
		if (item != pcPeAgent->m_setDpsSearchTaskNo.end())
		{
			pcPeAgent->m_setDpsSearchTaskNo.erase(item);
		}
	}

	ClearQueryMsg();

	m_pcImportActTask = NULL;
}
void CDpsSearchTask::InitStateMachine()
{
	CStateProc cNotReqState;
	cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::NotReqProcMsg;
	cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::NotReqTimerPoll;
	cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::NotReqProcMsg;
	AddRuleProc(NotReq, cNotReqState);
	CStateProc cWaitRspState;
	cWaitRspState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitRspProcMsg;
	cWaitRspState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::WaitRspTimerPoll;
	cWaitRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitRspProcMsg;
	AddRuleProc(WaitRsp, cWaitRspState);
	CStateProc cWaitGetResultReqState;
	cWaitGetResultReqState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultReqProcMsg;
	cWaitGetResultReqState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::WaitGetResultReqProcTimerPoll;
	cWaitGetResultReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultReqProcMsg;
	AddRuleProc(WaitGetResultReq, cWaitGetResultReqState);
	CStateProc cWaitGetResultRspState;
	cWaitGetResultRspState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultRspProcMsg;
	cWaitGetResultRspState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::WaitGetResultRspProcTimerPoll;
	cWaitGetResultRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultRspProcMsg;
	AddRuleProc(WaitGetResultRsp, cWaitGetResultRspState);
	CStateProc cWaitGetResultNtfState;
	cWaitGetResultNtfState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultNtfProcMsg;
	cWaitGetResultNtfState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::WaitGetResultNtfProcTimerPoll;
	cWaitGetResultNtfState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitGetResultNtfProcMsg;
	AddRuleProc(WaitGetResultNtf, cWaitGetResultNtfState);
	CStateProc cWaitByeState;
	cWaitByeState.ProcMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitByeProcMsg;
	cWaitByeState.TimerPoll = (CSipTask::PFTimerPoll)&CDpsSearchTask::WaitByeTimerPoll;
	cWaitByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CDpsSearchTask::WaitByeProcMsg;
	AddRuleProc(WaitBye, cWaitByeState);
	NextState(NotReq);
}

u32 CDpsSearchTask::StartDpsSearchReq(CDPSSearchReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����DSP��������\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
	m_tReq = cReq;
	if (!m_strPeSession.empty() /*&& pcPeAgent->GetState() != CPeAgent::Service*/)
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if (pcPeAgent != NULL&&pcPeAgent->GetState() == CPeAgent::Service)
		{
			cReq.SetSession(m_strPeSession);
			dwRet = PostReq<CDPSSearchReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
			if (dwRet != PROCMSG_OK)
			{
				return dwRet;
			}

			NextState(WaitRsp);

			return PROCMSG_OK;
		}
		else
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬��\n");
		}

		dwRet = PROCMSG_FAIL;
	}
	else
	{
		TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬��\n");
	}
	return dwRet;
}

u32 CDpsSearchTask::SendDpsSearchAck()
{
	CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
	if ( m_tInviteID == INVALID_DIALOG_ID )
	{
		return PROCMSG_FAIL;
	}

	CEventReq cAck;
	cAck.SetEvent(DPS_SEARCH_ACK);
	cAck.SetSession(m_strPeSession);

	u32 dwRet = PROCMSG_OK;
	dwRet = PostInDlgReq<CEventReq>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);


	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ImportAck����, Sip DialogIDΪ %u, dwRet[%u]\n",
		m_strCuSession.c_str(), GetTaskNO(), m_tInviteID, dwRet);

	NextState(WaitGetResultReq);
	return dwRet;
}

u32 CDpsSearchTask::SendGetResultReq(CDPSGetResultReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����DSP���������ѯ����\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
	
	if ( this->GetState() != WaitGetResultReq )
	{
		dwRet = PROCMSG_FAIL;
		return dwRet;
	}

	if (!m_strPeSession.empty())
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if (pcPeAgent != NULL && CPeAgent::Service == pcPeAgent->GetState())
		{
			cReq.SetSession(m_strPeSession);

			dwRet = PostInDlgReq<CDPSGetResultReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

			if (dwRet != PROCMSG_OK)
			{
				return dwRet;
			}

			g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());

			m_tGetResultReq = cReq;
			m_GetResultReqEvent = DPS_SEARCH_GET_RESULT_REQ;

			NextState(WaitGetResultRsp);
			return dwRet;
		}
		else
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬��\n");
		}
	}
	else
	{
		TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬�� m_strPeSession Ϊ��\n");
	}
	dwRet = PROCMSG_FAIL;

	return dwRet;
}

u32 CDpsSearchTask::SendGetResultByGroupReq(CDPSGetResultByGroupReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����DSP���������ѯ����\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());

	if (this->GetState() != WaitGetResultReq)
	{
		dwRet = PROCMSG_FAIL;
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CUI��ǰ״̬���� �ȴ���ѯ״̬session[%s] taskNo[%d]\n",
			m_strCuSession.c_str(), this->GetTaskNO());
		return dwRet;
	}

	if (!m_strPeSession.empty())
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if (pcPeAgent != NULL && CPeAgent::Service == pcPeAgent->GetState())
		{
			cReq.SetSession(m_strPeSession);

			dwRet = PostInDlgReq<CDPSGetResultByGroupReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

			if (dwRet != PROCMSG_OK)
			{
				return dwRet;
			}

			g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());

			m_tGetResultByGroupReq = cReq;
			m_GetResultReqEvent = DPS_SEARCH_GET_RESULT_BY_GROUP_REQ;

			NextState(WaitGetResultRsp);
			return dwRet;
		}
		else
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬��\n");
		}
	}
	else
	{
		TASKLOG(NORMAL_TASK, CRITICAL_LEV, "DPS���ڷ���̬�� m_strPeSession Ϊ��\n");
	}
	dwRet = PROCMSG_FAIL;

	return dwRet;
}

void CDpsSearchTask::SendGetResultNtfRsp(CMessage *const pcMsg)
{
	if ( pcMsg == NULL )
	{
		return;
	}

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	if ( pcOspSipMsg == NULL )
	{
		return;
	}

	CDPSGetResultNtfReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	//CDPSGetResultNtfRsp cRsp;
	//cRsp.SetHeadFromReq(cReq);
	//PostRsp(KDSIP_EVENT_INFO_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	CNtfRsp cRsp;
	cRsp.SetHeadFromReq(cReq);
	cRsp.SetErrorCode(CMS_SUCCESS);
	cRsp.SetEvent(DPS_SEARCH_GET_RESULT_NTF_RSP);
	//PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
	PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	//PostInDlgReq<CDPSGetResultReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);
}

u32 CDpsSearchTask::SendSearchBye()
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����DSP������������\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());

	if (!m_strPeSession.empty())
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if (pcPeAgent != NULL)
		{
			string strNULL;
			dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
			if (dwRet != PROCMSG_OK)
			{
				return dwRet;
			}

			ClearQueryMsg();

			NextState(WaitBye);
		}
		else
		{
			dwRet = PROCMSG_FAIL;
		}
	}
	else
	{
		dwRet = PROCMSG_FAIL;
	}
	return dwRet;
}

void CDpsSearchTask::ClearQueryMsg()
{
	g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_tGetResultReq.GetQueryTag());
	g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_tGetResultByGroupReq.GetQueryTag());

	m_tGetResultReq.Clear();
	m_tGetResultRsp.Clear();

	m_tGetResultByGroupReq.Clear();
	m_tGetResultByGroupRsp.Clear();

	m_GetResultReqEvent = 0;

	m_tResultList.clear();
}

void CDpsSearchTask::SendProcNtfRsp(CMessage *const pcMsg)
{
	if (pcMsg == NULL)
	{
		return;
	}

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		return;
	}

	CDPSSearchProgressNtfReq cNtfReq;
	pcOspSipMsg->GetMsgBody(cNtfReq);

	m_tProgressNtfReq = cNtfReq;

	CDPSSearchProgressNtfRsp cNtfRsp;
	cNtfRsp.SetHeadFromReq(cNtfReq);
	cNtfRsp.SetErrorCode(CMS_SUCCESS);
	PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);
}

u32 CDpsSearchTask::StartSendReq()
{
	u32 dwRet = PROCMSG_OK;
	CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
	CMS_ASSERT(NULL != pcPeAgent);
	if ( pcPeAgent != NULL && CPeAgent::Service != pcPeAgent->GetState())
	{
		m_strPeSession = pcPeAgent->m_strSessionID;
		m_tReq.SetSession(m_strPeSession);
		dwRet = PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, m_tPeUri);
		if (dwRet != PROCMSG_OK)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����[DPS_SEARCH_REQ]��Ϣʧ��\n",
				m_strCuSession.c_str());
			return dwRet;
		}
		else
		{
			NextState(WaitRsp);
		}
	}
	else if (m_pcImportActTask != NULL)
	{
		CDpsSearchInterAction *pcExportActTask = (CDpsSearchInterAction *)m_pcImportActTask;
		pcExportActTask->PostErrorMsg2Cu(ERR_CUI_PE_OFFLINE);
		delete pcExportActTask;
		pcExportActTask = NULL;
		dwRet = PROCMSG_DEL;
	}

	return dwRet;
}

u32 CDpsSearchTask::NotReqProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CDpsSearchTask::NotReqProcMsg]�յ�δ֪��Ϣ[%s--%hu]��������\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	CMS_ASSERT(0 && "CDpsSearchTask::NotReqProcMsg�յ�δ֪��Ϣ");
	return PROCMSG_OK;
}
u32 CDpsSearchTask::NotReqTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�ע������ģ�鳬ʱ������CDpsSearchTask[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CDpsSearchTask::WaitRspProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:   //��ʵ���ﲻ���ܻ��յ�Bye����Ϊ���л�δ������Ҫô�ɹ�Ҫô�ܾ����յ�403������
		{
			CMS_ASSERT(0 && "��������WaitRsp״̬���յ�BYE��Ϣ");
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if (m_pcImportActTask != NULL)
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CDpsSearchTask[TaskNO: %u](SIP��Ϣ[%s--%d])\n",
				GetTaskNO(), pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case DPS_SEARCH_RSP:
		{
			CDPSSearchRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_tInviteID = pcOspSipMsg->GetSipDlgID();
			m_tRsp = cRsp;

			if ( cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				if (dwRet != PROCMSG_OK)
				{
					cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
				}
				else
				{
					dwRet = SendDpsSearchAck();
				}
			}

			if (m_pcImportActTask != NULL)
			{
				if (PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}

			if (INVALID_DIALOG_ID == m_tInviteID)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���DialogID��Ч !!!!ֱ������CDpsSearchTask[TaskNO %u]\n",
					GetTaskNO());
				NextState(NotReq);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "��Ϣ����ʧ�ܣ�������[%d]������CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u]\n",
					cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			TASKLOG(NORMAL_TASK, EVENT_LEV, "CDpsSearchTask[TaskNO:%u]�û�[%s]����DPS�����ɹ�\n",
				GetTaskNO(), m_strCuSession.c_str());

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (NULL != pcCuAgent)
			{
				pcCuAgent->m_setDpsSearchTaskNo.insert(GetTaskNO());
			}

			CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
			if (NULL != pcPeAgent)
			{
				pcPeAgent->m_setDpsSearchTaskNo.insert(GetTaskNO());
			}
		}
		break;
	case DPS_SEARCH_PROGRESS_NTF_REQ:
		{
			SendProcNtfRsp(pcMsg);
			dwRet = PROCMSG_OK;
			break;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CDpsSearchTask::WaitRspTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ������CDpsSearchTask[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CDpsSearchTask::WaitGetResultReqProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CDpsSearchTask[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			if (m_pcImportActTask != NULL)
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;

			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if (m_pcImportActTask != NULL)
			{
				m_pcImportActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
				GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case DPS_SEARCH_PROGRESS_NTF_REQ:
		{
			SendProcNtfRsp(pcMsg);
			dwRet = PROCMSG_OK;
			break;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}

u32 CDpsSearchTask::WaitGetResultReqProcTimerPoll()
{
	// ��CUЭ�̺󣬳���50���Ӳ����������Զ�����
	if (GetCurStateHoldTime() > (DEFAULT_TIMEOUT*10*60) )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ѯ����ʱ������CDpsSearchTask[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CDpsSearchTask::WaitGetResultRspProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CDpsSearchTask[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			if (m_pcImportActTask != NULL)
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;

			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if (m_pcImportActTask != NULL)
			{

				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
				GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case DPS_SEARCH_GET_RESULT_RSP:
		{
			CDPSGetResultRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			TKDSIP_DIALOG_ID tInviteID = pcOspSipMsg->GetSipDlgID();

			if (m_tInviteID != tInviteID)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��INVITE ID[TaskNO %u]\n",
					GetTaskNO());

				dwRet = PROCMSG_OK;
				break;
			}
			
			if (m_tGetResultReq.GetQueryTag() != cRsp.GetQueryTag())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��QueryTag[TaskNO %u] ReqTag[%s] RspTag[%s]\n",
					GetTaskNO(), m_tGetResultReq.GetQueryTag().c_str(), cRsp.GetQueryTag().c_str());
				dwRet = PROCMSG_OK;
				break;
			}

			if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetTotalEntryNum() == 0)
			{
				// �յ��������û�н���ǾͲ��õ�NTF��ֱ�ӷ���
				if (m_pcImportActTask != NULL)
				{
					CMessage cuMsg;
					cuMsg.event = CU_CUI_DPS_GET_SEARCH_RESULT_RSP;

					CCuDPSGetResultRsp cuRsp;

					cuRsp.SetTotalEntryNum(m_tGetResultRsp.GetTotalEntryNum());
					//cuRsp.SetSearchResultList(m_tResultList);
					cuRsp.SetActualResultNum(m_tGetResultRsp.GetEntryNum());
					cuRsp.SetLastPage(true);
					cuRsp.SetErrorCode(cRsp.GetErrorCode());
					string content = cuRsp.ToXml();
					
					COspSipMsg sipmsg;
					sipmsg.SetMsgBody(content.c_str(), content.length() + 1);
					sipmsg.SetSipDlgID(m_tInviteID);
					cuMsg.content = (u8*)sipmsg.GetMsgBuf();
					cuMsg.length = sipmsg.GetMsgBodyLen();
					
					if (PROCMSG_DEL == m_pcImportActTask->ProcMsg(&cuMsg))
					{
						delete m_pcImportActTask;
						m_pcImportActTask = NULL;
					}
				}

				ClearQueryMsg();
				NextState(WaitGetResultReq);
				
				dwRet = PROCMSG_OK;
				break;
			}
			else
			{
				m_tGetResultRsp = cRsp;

				// �ȴ�NTF
				NextState(WaitGetResultNtf);
				dwRet = PROCMSG_OK;
				break;
			}
		}
		break;
	case DPS_SEARCH_GET_RESULT_BY_GROUP_RSP:
		{
			CDPSGetResultByGroupRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			TKDSIP_DIALOG_ID tInviteID = pcOspSipMsg->GetSipDlgID();

			if (m_tInviteID != tInviteID)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��INVITE ID[TaskNO %u]\n",
					GetTaskNO());

				dwRet = PROCMSG_OK;
				break;
			}

			if (m_tGetResultByGroupReq.GetQueryTag() != cRsp.GetQueryTag())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��QueryTag[TaskNO %u] ReqTag[%s] RspTag[%s]\n",
					GetTaskNO(), m_tGetResultByGroupReq.GetQueryTag().c_str(), cRsp.GetQueryTag().c_str());
				dwRet = PROCMSG_OK;
				break;
			}

			if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetTotalEntryNum() == 0)
			{
				// �յ��������û�н���ǾͲ��õ�NTF��ֱ�ӷ���
				if (m_pcImportActTask != NULL)
				{
					CMessage cuMsg;
					cuMsg.event = CU_CUI_DPS_GET_SEARCH_RESULT_RSP;

					CCuDPSGetResultByGroupRsp cuRsp;
					m_tGetResultByGroupRsp = cRsp;
					cuRsp.SetTotalEntryNum(m_tGetResultByGroupRsp.GetTotalEntryNum());
					//cuRsp.SetSearchResultList(m_tResultList);
					cuRsp.SetActualResultNum(m_tGetResultByGroupRsp.GetEntryNum());
					cuRsp.SetOnlineVidSrcNum(m_tGetResultByGroupRsp.GetOnlineVideoSrcNum());
					cuRsp.SetVidSrcNum(m_tGetResultByGroupRsp.GetAllVideoSrcNum());
					cuRsp.SetLastPage(true);
					cuRsp.SetErrorCode(cRsp.GetErrorCode());
					string content = cuRsp.ToXml();

					COspSipMsg sipmsg;
					sipmsg.SetMsgBody(content.c_str(), content.length() + 1);
					sipmsg.SetSipDlgID(m_tInviteID);
					cuMsg.content = (u8*)sipmsg.GetMsgBuf();
					cuMsg.length = sipmsg.GetMsgBodyLen();

					if (PROCMSG_DEL == m_pcImportActTask->ProcMsg(&cuMsg))
					{
						delete m_pcImportActTask;
						m_pcImportActTask = NULL;
					}
				}

				ClearQueryMsg();
				NextState(WaitGetResultReq);

				dwRet = PROCMSG_OK;
				break;
			}
			else
			{
				m_tGetResultByGroupRsp = cRsp;

				// �ȴ�NTF
				NextState(WaitGetResultNtf);
				dwRet = PROCMSG_OK;
				break;
			}
		}
		break;
	case DPS_SEARCH_PROGRESS_NTF_REQ:
		{
			SendProcNtfRsp(pcMsg);
			dwRet = PROCMSG_OK;
			break;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}

	return dwRet;
}

u32 CDpsSearchTask::WaitGetResultRspProcTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ��ɾ��֮ǰ�� �����ѯ[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());

		//ClearQueryMsg();
		NextState(WaitGetResultReq);
	}

	return TIMERPOLL_DONE;
}

u32 CDpsSearchTask::WaitGetResultNtfProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CDpsSearchTask[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			if (m_pcImportActTask != NULL)
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;

			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if (m_pcImportActTask != NULL)
			{
				m_pcImportActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
				GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case DPS_SEARCH_GET_RESULT_NTF_REQ:
		{
			CDPSGetResultNtfReq cNtfReq;
			pcOspSipMsg->GetMsgBody(cNtfReq);
			TKDSIP_DIALOG_ID tInviteID = pcOspSipMsg->GetSipDlgID();

			// �����������Ȼظ�
			this->SendGetResultNtfRsp(pcMsg);

			if (m_tInviteID != tInviteID)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��INVITE ID[TaskNO %u]\n",
					GetTaskNO());

				dwRet = PROCMSG_OK;
				break;
			}

			if (m_tGetResultReq.GetQueryTag() != cNtfReq.GetQueryTag()
				&& m_tGetResultByGroupReq.GetQueryTag() != cNtfReq.GetQueryTag() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��Ĳ���ͬһ��QueryTag[TaskNO %u]\n",
					GetTaskNO());
				dwRet = PROCMSG_OK;
				break;
			}

			m_tResultList.insert(m_tResultList.end(), cNtfReq.Result().begin(), cNtfReq.Result().end());

			SendGetResultNtfRsp(pcMsg);

			if ( cNtfReq.IsLastNtf())
			{
				// �յ����һ��NTF
				if (m_pcImportActTask != NULL)
				{
					CMessage cuMsg;
					COspSipMsg sipmsg;
					
					if (m_GetResultReqEvent == DPS_SEARCH_GET_RESULT_BY_GROUP_REQ)
					{
						cuMsg.event = CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_RSP;

						TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���ѯ�豸����������QueryTag[TaskNO %u]\n",
							GetTaskNO());

						CCuDPSGetResultByGroupRsp cuRsp;

						cuRsp.SetTotalEntryNum(m_tGetResultByGroupRsp.GetTotalEntryNum());
						cuRsp.SetSearchResultList(m_tResultList);
						cuRsp.SetActualResultNum(m_tGetResultByGroupRsp.GetEntryNum());
						cuRsp.SetLastPage(m_tGetResultByGroupRsp.IsLastPage());
						cuRsp.SetVidSrcNum(m_tGetResultByGroupRsp.GetAllVideoSrcNum());
						cuRsp.SetOnlineVidSrcNum(m_tGetResultByGroupRsp.GetOnlineVideoSrcNum());

						string content = cuRsp.ToXml();
						sipmsg.SetMsgBody(content.c_str(), content.length() + 1);
					}
					else 
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ��豸�������������QueryTag[TaskNO %u]\n",
							GetTaskNO());
						 // m_GetResultReqEvent == DPS_SEARCH_GET_RESULT_REQ 
						cuMsg.event = CU_CUI_DPS_GET_SEARCH_RESULT_RSP;

						CCuDPSGetResultRsp cuRsp;

						cuRsp.SetTotalEntryNum(m_tGetResultRsp.GetTotalEntryNum());
						cuRsp.SetSearchResultList(m_tResultList);
						cuRsp.SetActualResultNum(m_tGetResultRsp.GetEntryNum());
						cuRsp.SetLastPage(m_tGetResultRsp.IsLastPage());

						string content = cuRsp.ToXml();
						sipmsg.SetMsgBody(content.c_str(), content.length() + 1);
					}
					//sipmsg.SetSipDlgID(m_tInviteID);
					cuMsg.content = (u8*)sipmsg.GetMsgBuf();
					cuMsg.length = sipmsg.GetMsgBodyLen();

					if (PROCMSG_DEL == m_pcImportActTask->ProcMsg(&cuMsg))
					{
						delete m_pcImportActTask;
						m_pcImportActTask = NULL;
					}

				}

				ClearQueryMsg();
				NextState(WaitGetResultReq);
			}
			else
			{
				// �ȴ�NTF
				NextState(WaitGetResultNtf);
			}

			dwRet = PROCMSG_OK;
			break;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}

	return dwRet;
}

u32 CDpsSearchTask::WaitGetResultNtfProcTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ��ɾ��֮ǰ�� �����ѯ[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		//ClearQueryMsg();
		NextState(WaitGetResultReq);
	}

	return TIMERPOLL_DONE;
}

u32 CDpsSearchTask::WaitByeProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d]\n",
		m_strCuSession.c_str(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CDpsSearchTask[TaskNO %u]�յ�Bye Req��DialogID[%u]���ظ�Ӧ�������CDpsSearchTask\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			if (m_pcImportActTask != NULL)
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			if (m_pcImportActTask != NULL)
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, EVENT_LEV, "����CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if (m_pcImportActTask != NULL)
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if (PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CDpsSearchTask[TaskNO: %u]������Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CDpsSearchTask::WaitByeTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Bye Rsp��ʱ������Dialog\n",
			m_strCuSession.c_str());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

const CDPSSearchProgressNtfReq & CDpsSearchTask::GetProgressNtf()
{
	return m_tProgressNtfReq;
}
