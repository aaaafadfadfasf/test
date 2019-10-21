#include "gb_report_catalog_file_export.h"
#include "cms/utility/mischelper.h"
#include "cms/utility/digital_string.h"
#include "cms/commonconfig/cmcfg.h"
#include <map>
using std::map;


CGbReportCatalogFileExportTask::CGbReportCatalogFileExportTask( CInstExt *pcInst ):CSipTask(pcInst), m_tInvId(INVALID_DIALOG_ID)
{
	m_dwProgress = 0;
	m_cReq.Clear();
}

CGbReportCatalogFileExportTask::~CGbReportCatalogFileExportTask()
{
	if (m_tInvId != INVALID_DIALOG_ID)
	{
		OspSipReleaseDlg(m_tInvId);
		m_tInvId = INVALID_DIALOG_ID;
	}

	if (!m_cReq.GetSession().empty())
	{
		string strFile("/dev/shm/tas/");
		strFile.append(m_cReq.GetFileToken());
		strFile.append(".xml");
		remove(strFile.c_str());

	}

}

void CGbReportCatalogFileExportTask::InitStateMachine()
{
	CStateProc cReqState;
	cReqState.ProcMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcReqMsg;
	cReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcErrMsg;
	cReqState.TimerPoll = (CSipTask::PFTimerPoll)&CGbReportCatalogFileExportTask::WaitReqTimerPoll;
	AddRuleProc(REQ, cReqState);

	CStateProc cAckState;
	cAckState.ProcMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcAckMsg;
	cAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcErrMsg;
	cAckState.TimerPoll = (CSipTask::PFTimerPoll)&CGbReportCatalogFileExportTask::WaitAckTimerPoll;
	AddRuleProc(ACK, cAckState);

	CStateProc cByeState;
	cByeState.ProcMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcByeMsg;
	cByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcErrMsg;
	cByeState.TimerPoll = (CSipTask::PFTimerPoll)&CGbReportCatalogFileExportTask::WaitByeTimerPoll;
	AddRuleProc(BYE, cByeState);

	CStateProc cExportState;
	cExportState.ProcMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcExportMsg;
	cExportState.ProcErrMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcExportMsg;
	cExportState.TimerPoll = (CSipTask::PFTimerPoll)&CGbReportCatalogFileExportTask::WaitExportTimerPoll;
	AddRuleProc(EXPORT, cExportState);

	CStateProc cTransferState;
	cTransferState.ProcMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcTransferMsg;
	cTransferState.ProcErrMsg = (CSipTask::PFProcMsg)&CGbReportCatalogFileExportTask::ProcErrMsg;
	cTransferState.TimerPoll = (CSipTask::PFTimerPoll)&CGbReportCatalogFileExportTask::WaitTransferTimerPoll;
	AddRuleProc(TRANSFER, cTransferState);

	NextState(REQ);
}
u32 CGbReportCatalogFileExportTask::ProcReqMsg( CMessage *const pcMsg )
{
	if (pcMsg->event == FILE_EXPORT_REQ)
	{
		COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
		pcSipMsg->GetMsgBody(m_cReq);
		m_tInvId = pcSipMsg->GetSipDlgID();

		CFileExportRsp cRsp;
		cRsp.SetSeqNum(m_cReq.GetSeqNum());
		cRsp.SetSession(m_cReq.GetSession());


		string strFileUri;
		strFileUri.append("http://");
		strFileUri.append(GetLocalIPAddress());
		strFileUri.append(":");
		strFileUri.append(digital_string(::GetHttpPort()));
		strFileUri.append("/shm/tas/");
		strFileUri.append(m_cReq.GetFileToken());
		strFileUri.append(".xml");
		cRsp.SetFileURL(strFileUri);
		TASKLOG(EVENT_LEV,"FILE_EXPORT_REQ,TreeId[%s],fileUri[%s]\n",m_cReq.GetFileToken().c_str(),strFileUri.c_str());

		if (PROCMSG_OK == PostRsp(KDSIP_EVENT_INVITE_RSP, pcSipMsg->GetSipTransID(), cRsp))
		{
			NextState(ACK);
			return PROCMSG_OK;
		}
		else
		{
			return PROCMSG_DEL;
		}
	}
	else
	{
		return PROCMSG_DEL;
	}
}

u32 CGbReportCatalogFileExportTask::WaitReqTimerPoll()
{
	return TIMERPOLL_DONE;
}

u32 CGbReportCatalogFileExportTask::ProcAckMsg( CMessage *const pcMsg )
{
	u32 dwProcRet = PROCMSG_OK;
	COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

	switch(pcMsg->event)
	{
	case FILE_EXPORT_ACK:
		{
			if (m_cReq.GetSrvName() == FILE_EXPORT_GB_CATALOG)
			{
				CDbMsgTransfer<string> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgTableDataToPushXmlReq);
				cTrans.GetMsgObj() = m_cReq.GetFileToken();
				if (!PostDbMsgTransfer2DbTaskPool(cTrans))
				{
					TASKLOG(ERROR_LEV,"PostDbMsgTransfer2DbTaskPool fail!!\n");
					PostProcessNtf(ERR_TAS_SYSTEM_BUSY, 0);
					dwProcRet = PROCMSG_DEL;
				}
				else
					PostProcessNtf(CMS_SUCCESS,0);

			}
			else
			{
				PostProcessNtf(ERR_INVALIAD_REQ_PARAM, 0);
				dwProcRet = PROCMSG_DEL;
			}

			if (PROCMSG_OK == dwProcRet)
			{
				NextState(EXPORT);
			}
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			string strNull;
			if (PROCMSG_OK != PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcSipMsg->GetSipTransID(), strNull))
			{
				TASKLOG(ERROR_LEV, "发送BYE应答失败!\n");
			}
			else
			{
				TASKLOG(WARNING_LEV, "发送BYE应答成功!\n");
			}

			dwProcRet = PROCMSG_DEL;
		}
		break;
	default:
		break;
	}

	return dwProcRet;
}

u32 CGbReportCatalogFileExportTask::WaitAckTimerPoll()
{
	if (GetCurStateHoldTime() < 60)
	{
		return TIMERPOLL_DONE;
	}

	Log(WARNING_LEV, "detected ack timeout, ObjName is %s\n", GetObjName());
	return TIMERPOLL_DEL;
}

u32 CGbReportCatalogFileExportTask::ProcExportMsg( CMessage *const pcMsg )
{
	u32 dwProcRet = PROCMSG_OK;
	COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

	switch(pcMsg->event)
	{
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		{
		}
		// 直接返回成功即可
		break;
	case msgDbMsgTransfer:
		{
			ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
			const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);
			if ( msgTableDataToPushXmlRsp == pcMsgTrans->GetEventId() )
			{
				const string *pcRsp = static_cast<const string*>(pcMsgTrans->GetMsgObjAddr());
				PostProcessNtf(CMS_SUCCESS,100);
			}
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			string strNull;
			if (PROCMSG_OK != PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcSipMsg->GetSipTransID(), strNull))
			{
				TASKLOG(ERROR_LEV, "发送BYE应答失败!\n");
			}
			else
			{
				TASKLOG(ERROR_LEV, "发送BYE应答成功!\n");
			}
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			TASKLOG(ERROR_LEV, "对话异常终止,销毁订阅事务!\n");
			m_tInvId = INVALID_DIALOG_ID;

		}
		break;
	default:
		break;
	}

	return dwProcRet;
}

u32 CGbReportCatalogFileExportTask::WaitExportTimerPoll()
{
	if (GetCurStateHoldTime()%2 == 0)
	{
		m_dwProgress++;
		if (m_dwProgress > 99)
			m_dwProgress = 99;
		PostProcessNtf(CMS_SUCCESS,m_dwProgress);
	}
	if (GetCurStateHoldTime() > 100)
	{
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CGbReportCatalogFileExportTask::ProcByeMsg( CMessage *const pcMsg )
{
	if (pcMsg->event == OSP_SIP_BYE_RSP)
	{
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

u32 CGbReportCatalogFileExportTask::WaitByeTimerPoll()
{
	if (GetCurStateHoldTime() < 60)
	{
		return TIMERPOLL_DONE;
	}

	Log(WARNING_LEV, "detected bye timeout, ObjName is %s\n", GetObjName());
	return TIMERPOLL_DEL;
}


u32 CGbReportCatalogFileExportTask::ProcTransferMsg( CMessage *const pcMsg )
{
	u32 dwProcRet = PROCMSG_OK;
	COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

	switch(pcMsg->event)
	{
	case OSP_SIP_BYE_REQ:
		{
			string strNull;
			if (PROCMSG_OK != PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcSipMsg->GetSipTransID(), strNull))
			{
				TASKLOG(ERROR_LEV, "发送BYE应答失败!\n");
			}
			else
			{
				TASKLOG(ERROR_LEV, "发送BYE应答成功!\n");
			}

			dwProcRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_RSP:
		// 直接返回OK即可
		break;
	default:
		break;
	}

	return dwProcRet;
}

u32 CGbReportCatalogFileExportTask::WaitTransferTimerPoll()
{
	if (GetCurStateHoldTime() > 600)
	{
		PostProcessNtf(ERR_TAS_TIME_OUT, 0);
		Log(WARNING_LEV, "detected transfer timeout, ObjName is %s\n", GetObjName());
	}

	return TIMERPOLL_DONE;
}

u32 CGbReportCatalogFileExportTask::ProcErrMsg( CMessage *const pcMsg )
{
	if (OSP_SIP_DIALOG_TERMINATE == pcMsg->event)
	{
		TASKLOG(ERROR_LEV, "对话异常终止,销毁订阅事务!\n");
		m_tInvId = INVALID_DIALOG_ID;
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

void CGbReportCatalogFileExportTask::PostProcessNtf( s32 nErrorCode, s32 nPercent )
{
	if (CMS_SUCCESS == nErrorCode)
	{
		CFileCollectProgressNtfReq cReq;
		cReq.SetSession(m_cReq.GetSession());
		cReq.SetPercent(nPercent);
		PostInDlgReq(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInvId);
	}
	else
	{
		CByeReq cBye;
		cBye.SetReason(nErrorCode);
		cBye.SetSession(m_cReq.GetSession());

		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cBye, m_tInvId);
		NextState(BYE);
	}
}

void CGbReportCatalogFileExportTask::PrintData() const
{
	CSipTask::PrintData();

	OspPrintf(TRUE, FALSE, "gbs Session:%s\n", m_cReq.GetSession().c_str());
}

