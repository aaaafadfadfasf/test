#include "culogintask.h"
#include "import_task.h"
#include "cuiproto.h"
#include "cuiapp.h"
#include <map>
using std::map;

//static map<string, string> s_mapCuSession2FileUri;

CImportTaskExInterAction::CImportTaskExInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(ptRecvNode != NULL);
	m_ptRecvNode = ptRecvNode;
	m_strCuSession = strCuSession;
	m_dwInviteReqTaskNo = 0;
}

CImportTaskExInterAction::~CImportTaskExInterAction()
{
	TerminateTask();
}

void CImportTaskExInterAction::TerminateTask()
{
	if( !m_stFileUrl.empty())
	{
		CImportTaskEx *pcTask = (CImportTaskEx *)GetImportTask(m_stFileUrl);
		if( NULL != pcTask)
		{
			if ( pcTask->m_pcImportActTask == this )
			{
				pcTask->m_pcImportActTask = NULL;
			}
		}
	}
}

void CImportTaskExInterAction::InitStateMachine()
{
	CStateProc cReqRroc;
	cReqRroc.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskExInterAction::ReqProcMsg;
	cReqRroc.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskExInterAction::ReqTimerPoll;
	cReqRroc.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskExInterAction::ReqProcMsg;
	AddRuleProc(Req, cReqRroc);
	CStateProc cRspProc;
	cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskExInterAction::RspProcMsg;
	cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskExInterAction::RspTimerPoll;
	cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskExInterAction::RspProcMsg;
	AddRuleProc(Rsp, cRspProc);
	NextState(Req);
}
u32 CImportTaskExInterAction::ReqProcMsg( CMessage *const pcMsg )
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]的请求[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	m_dwReqEventID = pcMsg->event;
	switch (pcMsg->event)
	{
	case CU_CUI_FILE_IMPORT_START_EX_REQ:
		{
			CCuFileImportStartExReq *pcReq = (CCuFileImportStartExReq *)pcMsg->content;

			string strPeUri = g_cCuiConfig.GetLocalURI().GetUasURI().GetURIString();
			if ( pcReq->GetSrvName() == FILE_IMPORT_GB_CATALOG_REPORT_CFG)
			{
				strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			}

			CImportTaskEx* pcTask = new CImportTaskEx(GetInstance(), m_strCuSession, this, strPeUri);
			pcTask->InitStateMachine();

			m_dwInviteReqTaskNo = pcTask->GetTaskNO();

			CFileImportReq cFileImportReq;

			cFileImportReq.SetSrvName(pcReq->GetSrvName());
			cFileImportReq.SetFileToken(pcReq->GetFileToken());
			cFileImportReq.SetFileSenderUri(pcReq->GetFileSenderUri());
			cFileImportReq.SetFileReceiverUri(pcReq->GetFileReceiverUri());

			dwRet = pcTask->StartSendImportReq(cFileImportReq);

			NextState(Rsp);
		}
		break;
	case CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ:
		{
			CCuFileImportSetProcessExReq* pcReq = (CCuFileImportSetProcessExReq *)pcMsg->content;

			u32 nTaskNo = pcReq->GetTaskNo();
			if (nTaskNo == INVALID_TASKNO)
			{
				CCuFileImportSetProcessExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileImportSetProcessExRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的CU_CUI_SET_IMPORT_PROCESS_REQ没有对应TaskNo为无效值\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
			}

			CImportTaskEx* pcTask = (CImportTaskEx *)(this->GetInstance()->FindTask(nTaskNo));
			if( NULL != pcTask)
			{
				pcTask->SendImportProcessReq(*pcReq);
				pcTask->m_pcImportActTask = this;
				dwRet = PROCMSG_OK;
			}
			else
			{
				CCuFileImportSetProcessExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileImportSetProcessExRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的CU_CUI_SET_IMPORT_PROCESS_REQ没有对应Task[%d]\n",
					m_strCuSession.c_str(), nTaskNo);
				dwRet = PROCMSG_DEL;
				delete this;
			}
			NextState(Rsp);
		}
		break;
	case CU_CUI_FILE_IMPORT_STOP_EX_REQ:
		{
			CCuFileImportStopExReq* pcReq = (CCuFileImportStopExReq *)pcMsg->content;

			u32 nTaskNo = pcReq->GetTaskNo();
			CImportTaskEx *pcTask = (CImportTaskEx*)(GetInstance()->FindTask(nTaskNo));
			if (pcTask != NULL)
			{
				if ( pcReq->GetCancelImport() )
				{
					dwRet = pcTask->SendImportBye();
				}
				else
				{
					dwRet = pcTask->SendImportEnd();
				}

				if ( dwRet == PROCMSG_DEL)
				{
					CCuFileImportStopExRsp cRsp;
					cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
					SendRspToCu<CCuFileImportStopExRsp>(cRsp, m_ptRecvNode);
					TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送消息失败\n",
						m_strCuSession.c_str());
					dwRet = PROCMSG_DEL;
					delete this;
				}
				else
				{
					pcTask->m_pcImportActTask = this;
					NextState(Rsp);
					dwRet = PROCMSG_OK;
				}
			}
			else
			{
				CCuFileImportStopExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileImportStopExRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Export没有对应Task\n",
					m_strCuSession.c_str());
				dwRet = PROCMSG_DEL;
				delete this;
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CImportTaskExInterAction::ReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待消息应答超时\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CImportTaskExInterAction::RspProcMsg( CMessage *const pcMsg )
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到消息应答[%s--%d] (SIP应答消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户会话[%s]发送应答消息失败，错误码[%d]\n",
				m_strCuSession.c_str(), pcOspSipMsg->GetSipErrorCode());
			NextState(Req);
			dwRet =  PROCMSG_DEL;
		}
		break;
	case FILE_IMPORT_RSP:
		{
			CFileImportRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_stFileUrl = cRsp.GetDirURL();

			CCuFileImportStartExRsp cCuRsp;

			cCuRsp.SetTaskNo( m_dwInviteReqTaskNo );
			cCuRsp.SetErrorCode(cRsp.GetErrorCode());
			cCuRsp.SetDirURL(cRsp.GetDirURL());

			if (INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
			{
				cCuRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			}

			SendRspToCu<CCuFileImportStartExRsp>(cCuRsp, m_ptRecvNode);
			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到File Collect Progress Ntf Rsp，DialogID[%u]\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CFileCollectProgressNtfRsp ntfRsp;
			pcOspSipMsg->GetMsgBody(ntfRsp);

			CCuFileImportSetProcessExRsp cCuRsp;
			cCuRsp.SetErrorCode( ntfRsp.GetErrorCode() );

			SendRspToCu<CCuFileImportSetProcessExRsp>(cCuRsp, m_ptRecvNode);

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_TRANS_END_RSP:
		{
			CFileTransEndRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			
			CCuFileImportStopExRsp cCuStopRsp;
			cCuStopRsp.SetErrorCode(cRsp.GetErrorCode());
			
			SendRspToCu<CCuFileImportStopExRsp>(cCuStopRsp, m_ptRecvNode);

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			CCuFileImportStopExRsp cCuStopRsp;
			cCuStopRsp.SetErrorCode(CMS_SUCCESS);

			if (m_ptRecvNode != NULL)
			{
				SendRspToCu<CCuFileImportStopExRsp>(cCuStopRsp, m_ptRecvNode);
			}

			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			if(pcOspSipMsg->GetMsgBodyLen() > 0)
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
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CImportTaskExInterAction::RspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待应答消息超时\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
CTask *CImportTaskExInterAction::GetImportTask(const string& strFileUrl)
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	u32 dwTaskId = INVALID_TASKNO;
	if(pcCuAgent != NULL)
	{
		if ( ! pcCuAgent->m_cFileExportTaskSet.Find(strFileUrl, dwTaskId) )
		{
			return NULL;
		}
		if ( INVALID_TASKNO == dwTaskId )
		{
			return NULL;
		}
		return GetInstance()->FindTask(dwTaskId);
	}
	return NULL;
}
void CImportTaskExInterAction::PostErrorMsg2Cu(u32 dwErrorCode)
{
	switch( m_dwReqEventID)
	{
	case CU_CUI_FILE_IMPORT_START_EX_REQ:
		{
			SendErrorMsg2Cu<CCuFileImportStartExRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ:
		{
			SendErrorMsg2Cu<CCuFileImportSetProcessExRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_FILE_IMPORT_STOP_EX_REQ:
		{
			SendErrorMsg2Cu<CCuFileImportStopExRsp>(dwErrorCode);
		}
		break;
	default:
		{
			SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
		}
		break;
	}
}


CImportTaskEx::CImportTaskEx(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(!strPeUri.empty());
	CMS_ASSERT(NULL != pcExportActTask);
	m_pcImportActTask = pcExportActTask;
	m_tInviteID = INVALID_DIALOG_ID;
}
CImportTaskEx::~CImportTaskEx()
{
	TerminateTask();
}
void CImportTaskEx::TerminateTask()
{
	if( m_tInviteID != INVALID_DIALOG_ID)
	{
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
		//OspSipReleaseDlg(m_tInviteID);
		m_tInviteID = INVALID_DIALOG_ID;
	}
	if(!m_strExportFileUrl.empty())
	{
		CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
		if(NULL != pcCuAgent)
		{
			pcCuAgent->m_cFileExportTaskSet.Erase(m_strExportFileUrl);
		}
		CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		if(NULL != pcPeAgent)
		{
			pcPeAgent->m_cFileExportTaskSet.Erase(m_strExportFileUrl);
		}
	}
	m_pcImportActTask = NULL;
}
void CImportTaskEx::InitStateMachine()
{
	CStateProc cNotReqState;
	cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskEx::NotReqProcMsg;
	cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskEx::NotReqTimerPoll;
	cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskEx::NotReqProcMsg;
	AddRuleProc(NotReq, cNotReqState);
	CStateProc cWaitRspState;
	cWaitRspState.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitRspProcMsg;
	cWaitRspState.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskEx::WaitRspTimerPoll;
	cWaitRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitRspProcMsg;
	AddRuleProc(WaitRsp, cWaitRspState);
	CStateProc cWaitAckState;
	cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitAckProcMsg;
	cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskEx::WaitAckTimerPoll;
	cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitAckProcMsg;
	AddRuleProc(WaitAck, cWaitAckState);
	CStateProc cWaitProcessState;
	cWaitProcessState.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitProcessProcMsg;
	cWaitProcessState.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskEx::WaitProcessTimerPoll;
	cWaitProcessState.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitProcessProcMsg;
	AddRuleProc(WaitProcess, cWaitProcessState);
	CStateProc cWaitByeState;
	cWaitByeState.ProcMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitByeProcMsg;
	cWaitByeState.TimerPoll = (CSipTask::PFTimerPoll)&CImportTaskEx::WaitByeTimerPoll;
	cWaitByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CImportTaskEx::WaitByeProcMsg;
	AddRuleProc(WaitBye, cWaitByeState);
	NextState(NotReq);
}
u32 CImportTaskEx::StartSendImportReq(CFileImportReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送文件导出请求\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
	m_tReq = cReq;
	if(!m_strPeSession.empty())
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if(pcPeAgent != NULL)
		{
			cReq.SetSession(m_strPeSession);
			dwRet = PostReq<CFileImportReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
			if(dwRet != PROCMSG_OK)
			{
				return dwRet;
			}
			NextState(WaitRsp);
		}
		dwRet = PROCMSG_FAIL;
	}
	return dwRet;
}
u32 CImportTaskEx::StartSendImportAck()
{
	CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送ImportAck请求, Sip DialogID为 %u\n",
		m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
	CFileImportAck cAck;
	cAck.SetSession(m_strPeSession);
	u32 dwRet = PROCMSG_OK;
	dwRet = PostInDlgReq<CFileImportAck>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
	NextState(WaitProcess);
	return dwRet;
}
u32 CImportTaskEx::SendImportProcessReq(CCuFileImportSetProcessExReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送文件导入进度请求\n",
		m_strCuSession.c_str());
	CCuiInst *pcInst = (CCuiInst *)GetInstance();
	m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
	m_tSetProcessReq = cReq;
	if(!m_strPeSession.empty())
	{
		CPeAgentBase *pcPeAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
		CMS_ASSERT(NULL != pcPeAgent);
		if(pcPeAgent != NULL)
		{
			CFileCollectProgressNtfReq ntfReq;

			int percent = 0;
			if ( cReq.GetProgress() < 0 || cReq.GetTotal() < 0)
			{
				percent = 0;
			}
			else if ( cReq.GetProgress() >= cReq.GetTotal() )
			{
				percent = 100;
			}
			else
			{
				float fPercent = cReq.GetProgress()/cReq.GetTotal();
				percent = fPercent*100;
			}
			
			ntfReq.SetPercent(percent);
			ntfReq.SetSession(m_strPeSession);
			
			dwRet = PostInDlgReq<CFileCollectProgressNtfReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, ntfReq, m_tInviteID);
			if(dwRet != PROCMSG_OK)
			{
				return PROCMSG_FAIL;
			}
			return dwRet;
		}
		dwRet = PROCMSG_FAIL;
	}
	return dwRet;
}
u32 CImportTaskEx::SendImportBye()
{
	u32 dwRet = PROCMSG_OK;
	if( m_tInviteID != INVALID_DIALOG_ID)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送ImportBye请求, Sip DialogID为 %u\n",
			m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
		string strNULL;
		dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
	}
	NextState(WaitBye);
	return dwRet;
}

u32 CImportTaskEx::SendImportEnd()
{
	CFileTransEndReq endReq;

	u32 dwRet = PROCMSG_OK;
	if( m_tInviteID != INVALID_DIALOG_ID)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送ImportEnd请求, Sip DialogID为 %u\n",
			m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
		
		dwRet = PostInDlgReq(KDSIP_EVENT_MESSAGE_DIALOG_REQ, endReq, m_tInviteID);
	}
	NextState(WaitBye);
	return dwRet;
}

u32 CImportTaskEx::StartSendReq()
{
	u32 dwRet = PROCMSG_OK;
	CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
	CMS_ASSERT(NULL != pcPeAgent);
	if(pcPeAgent->GetState() != CPeAgent::Service)
	{
		if(m_pcImportActTask != NULL)
		{
			CImportTaskExInterAction *pcExportActTask= (CImportTaskExInterAction *)m_pcImportActTask;
			pcExportActTask->PostErrorMsg2Cu(ERR_CUI_PE_OFFLINE);
			delete pcExportActTask;
			pcExportActTask = NULL;
			return PROCMSG_DEL;
		}
	}
	if( pcPeAgent )
	{
		m_strPeSession = pcPeAgent->m_strSessionID;
		m_tReq.SetSession(m_strPeSession);
		dwRet = PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, m_tPeUri);
		if(dwRet != PROCMSG_OK)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送[FILE_EXPORT_REQ]消息失败\n",
				m_strCuSession.c_str());
			return dwRet;
		}
		NextState(WaitRsp);
	}  
	return dwRet;
}

void CImportTaskEx::NotifyStop2Cu(s32 dwErrorCode)
{
	CCuExportFailNotify cExportFailNtf;
	cExportFailNtf.exportFileUrl = m_strExportFileUrl;
	cExportFailNtf.errorCode = dwErrorCode;

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession,GetInstance());
	CMS_ASSERT(pcCuAgent != NULL);
	if(pcCuAgent != NULL)
	{
		pcCuAgent->m_cNotifyPool.InsertExportFailNotify(cExportFailNtf);
	}
}

u32 CImportTaskEx::NotReqProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CImportTask::NotReqProcMsg]收到未知消息[%s--%hu]，不处理\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	CMS_ASSERT(0&&"CImportTask::NotReqProcMsg收到未知消息");
	return PROCMSG_OK;
}
u32 CImportTaskEx::NotReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待注册外设模块超时，销毁CImportTask[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CImportTaskEx::WaitRspProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:   //其实这里不可能会收到Bye，因为呼叫还未建立，要么成功要么拒绝，收到403错误码
		{
			CMS_ASSERT(0&&"不可能在WaitRsp状态下收到BYE消息");
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if ( m_pcImportActTask != NULL )
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL ==  dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "接收消息失败，销毁CImportTask[TaskNO: %u](SIP消息[%s--%d])\n",
				GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_IMPORT_RSP:
		{
			CFileImportRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_tInviteID = pcOspSipMsg->GetSipDlgID();
			m_tRsp = cRsp;
			m_strExportFileUrl = cRsp.GetDirURL();

			if ( m_pcImportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			if ( INVALID_DIALOG_ID == m_tInviteID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的DialogID无效 !!!!直接销毁CImportTask[TaskNO %u]\n",
					GetTaskNO());
				m_tInviteID = INVALID_DIALOG_ID;
				NextState(NotReq);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			if ( CMS_SUCCESS != cRsp.GetErrorCode() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "消息请求失败，错误码[%d]，销毁CImportTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
					cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO:%u]用户[%s]文件导入请求成功\n",
					GetTaskNO(), m_strCuSession.c_str());
				CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
				if(pcCuAgent != NULL)
				{
					pcCuAgent->m_cFileExportTaskSet.Insert(m_strExportFileUrl, GetTaskNO());
				}
				CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
				if(pcPeAgent != NULL)
				{
					pcPeAgent->m_cFileExportTaskSet.Insert(m_strExportFileUrl, GetTaskNO());
				}   
				NextState(WaitAck);
				StartSendImportAck();  
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CImportTaskEx::WaitRspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待消息应答超时，销毁CImportTask[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CImportTaskEx::WaitAckProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到Bye Req消息，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if ( m_pcImportActTask != NULL )
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;

			if(pcOspSipMsg->GetMsgBodyLen() >0)
			{
				CByeReq cByeReq;
				pcOspSipMsg->GetMsgBody(cByeReq);
				NotifyStop2Cu(cByeReq.GetReason());
			}
			else
			{
				NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
			}

			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if ( m_pcImportActTask != NULL )
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "接收消息失败，销毁CImportTask[TaskNO: %u](SIP应答消息[%s--%d])\n",
				GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		{
			// 正常情况下不会收到这个
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到File Collect Progress Ntf Rsp，DialogID[%u]\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			pcOspSipMsg->GetMsgBody(m_tNtfRsp);

			if ( m_pcImportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
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
	return dwRet;
}
u32 CImportTaskEx::WaitAckTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "等待CU用户会话[%s]发送Ack消息超时，直接发送Bye结束\n",
			m_strCuSession.c_str());
		SendImportBye();
		return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
	}
	return TIMERPOLL_DONE;
}

u32 CImportTaskEx::WaitProcessProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到Bye Req请求，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if( m_pcImportActTask != NULL)
			{
				dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;
	
			if(pcOspSipMsg->GetMsgBodyLen() > 0)
			{
				CByeReq cByeReq;
				pcOspSipMsg->GetMsgBody(cByeReq);
				NotifyStop2Cu(cByeReq.GetReason());
			}
			else
			{
				NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
			}
	
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if ( m_pcImportActTask != NULL )
			{
				m_pcImportActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CImportTask[TaskNO: %u]，结束Sip Dialog: [%u] (SIP应答消息[%s--%d])\n",
				GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到File Collect Progress Ntf Rsp，DialogID[%u]\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			pcOspSipMsg->GetMsgBody(m_tNtfRsp);

			if ( m_pcImportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			
			NextState(WaitProcess);
			dwRet = PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}

u32 CImportTaskEx::WaitProcessTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "等待CU用户会话[%s]设置导入文件进度消息超时，直接发送Bye结束\n",
			m_strCuSession.c_str());
		SendImportBye();
		return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
	}
	return TIMERPOLL_DONE;
}

u32 CImportTaskEx::WaitByeProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d]\n",
		m_strCuSession.c_str(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:   
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到Bye Req，DialogID[%u]，回复应答后销毁CImportTask\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if ( m_pcImportActTask != NULL )
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
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
			if ( m_pcImportActTask != NULL )
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CImportTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
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
			if ( m_pcImportActTask != NULL )
			{
				u32 dwRet = m_pcImportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CImportTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO %u]收到File Collect Progress Ntf Rsp，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			pcOspSipMsg->GetMsgBody(m_tNtfRsp);
			
			if ( m_pcImportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}

			dwRet = PROCMSG_OK;
		}
		break;
	case FILE_TRANS_END_RSP:
		{
			CFileTransEndRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			
			m_tInviteID = pcOspSipMsg->GetSipDlgID();
			if ( m_pcImportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcImportActTask->ProcMsg(pcMsg))
				{
					delete m_pcImportActTask;
					m_pcImportActTask = NULL;
				}
			}

			if ( INVALID_DIALOG_ID == m_tInviteID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的DialogID无效 !!!!直接销毁CImportTask[TaskNO %u]\n",
					GetTaskNO());
				m_tInviteID = INVALID_DIALOG_ID;
				NextState(NotReq);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}

			if ( CMS_SUCCESS != cRsp.GetErrorCode() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "结束消息请求失败，错误码[%d]，销毁CImportTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
					cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CImportTask[TaskNO:%u]用户[%s]文件导入结束成功\n",
					GetTaskNO(), m_strCuSession.c_str());

				NextState(WaitBye);
				break;
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CImportTaskEx::WaitByeTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Bye Rsp超时，结束Dialog\n",
			m_strCuSession.c_str());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
