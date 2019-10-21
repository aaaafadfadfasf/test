#include "export_task.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "cuiapp.h"
#include <map>
using std::map;

static map<string, string> s_mapCuSession2FileUri;

CExportTaskInterAction::CExportTaskInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(ptRecvNode != NULL);
    m_ptRecvNode = ptRecvNode;
    m_strCuSession = strCuSession;
}
CExportTaskInterAction::~CExportTaskInterAction()
{
    TerminateTask();
}
void CExportTaskInterAction::TerminateTask()
{
    if( !m_stFileUrl.empty())
    {
        CExportTask *pcTask = (CExportTask *)GetExportTask(m_stFileUrl);
        if( NULL != pcTask)
        {
            if ( pcTask->m_pcExportActTask == this )
            {
                pcTask->m_pcExportActTask = NULL;
            }
        }
    }
}
void CExportTaskInterAction::InitStateMachine()
{
    CStateProc cReqRroc;
    cReqRroc.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskInterAction::ReqProcMsg;
    cReqRroc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskInterAction::ReqTimerPoll;
    cReqRroc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskInterAction::ReqProcMsg;
    AddRuleProc(Req, cReqRroc);
    CStateProc cRspProc;
    cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskInterAction::RspProcMsg;
    cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskInterAction::RspTimerPoll;
    cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskInterAction::RspProcMsg;
    AddRuleProc(Rsp, cRspProc);
    NextState(Req);
}
u32 CExportTaskInterAction::ReqProcMsg( CMessage *const pcMsg )
{
    u32 dwRet = PROCMSG_OK;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�CU�û��Ự[%s]������[%s--%hu]\n",
        m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    m_dwReqEventID = pcMsg->event;
    switch (pcMsg->event)
    {
    case FILE_EXPORT_REQ:
        {
            CCuFileExportReq *pcReq = (CCuFileExportReq *)pcMsg->content;
            CExportTask* pcTask = new CExportTask(GetInstance(), m_strCuSession, this, g_cCuiConfig.GetLocalURI().GetUasURI().GetURIString());
            pcTask->InitStateMachine();
            pcTask->StartSendExportReq(*pcReq);

            NextState(Rsp);
        }
        break;
    case CU_CUI_STOP_EXPORT_REQ:
        {
            CCuStopExportReq* pcReq = (CCuStopExportReq *)pcMsg->content;
            CExportTask *pcTask = (CExportTask *)GetExportTask(pcReq->GetExportFileUrl());
            if( NULL != pcTask)
            {
                pcTask->StartSendExportBye();
                pcTask->m_pcExportActTask = this;
                NextState(Rsp);
            }
            else
            {
                CCuStopExportRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                SendRspToCu<CCuStopExportRsp>(cRsp, m_ptRecvNode);
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��Exportû�ж�ӦTask\n",
                    m_strCuSession.c_str());
                dwRet = PROCMSG_DEL;
                delete this;
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTaskInterAction::ReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ\n",
            m_strCuSession.c_str());
        PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        NextState(Req);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTaskInterAction::RspProcMsg( CMessage *const pcMsg )
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ���ϢӦ��[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "��CU�û��Ự[%s]����Ӧ����Ϣʧ�ܣ�������[%d]\n",
                m_strCuSession.c_str(), pcOspSipMsg->GetSipErrorCode());
            NextState(Req);
            dwRet =  PROCMSG_DEL;
        }
        break;
    case FILE_EXPORT_RSP:
        {
            CCuFileExportRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            m_stFileUrl = cRsp.GetFileURL();
            if (INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
            }
            SendRspToCu<CCuFileExportRsp>(cRsp, m_ptRecvNode);
            NextState(Req);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            CCuStopExportRsp cRsp;
            SendRspToCu<CCuStopExportRsp>(cRsp, m_ptRecvNode);
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
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTaskInterAction::RspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Ӧ����Ϣ��ʱ\n",
            m_strCuSession.c_str());
        PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        NextState(Req);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
CTask *CExportTaskInterAction::GetExportTask(const string& strFileUrl)
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
void CExportTaskInterAction::PostErrorMsg2Cu(u32 dwErrorCode)
{
    switch( m_dwReqEventID)
    {
    case FILE_EXPORT_REQ:
        {
            SendErrorMsg2Cu<CCuFileExportRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_STOP_EXPORT_REQ:
        {
            SendErrorMsg2Cu<CCuStopExportRsp>(dwErrorCode);
        }
        break;
    default:
        {
            SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
        }
        break;
    }
}

CExportTask4TasInterAction::CExportTask4TasInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(ptRecvNode != NULL);
    m_ptRecvNode = ptRecvNode;
    m_strCuSession = strCuSession;
}
CExportTask4TasInterAction::~CExportTask4TasInterAction()
{
    TerminateTask();
}
void CExportTask4TasInterAction::TerminateTask()
{
    if( !m_cExportRsp.GetFileURL().empty())
    {
        CExportTask4Tas *pcTask = (CExportTask4Tas *)GetExportTask(m_cExportRsp.GetFileURL());
        if( NULL != pcTask)
        {
            if ( pcTask->m_pcExportActTask == this )
            {
                pcTask->m_pcExportActTask = NULL;
            }
        }
    }
}
void CExportTask4TasInterAction::InitStateMachine()
{
    CStateProc cReqRroc;
    cReqRroc.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4TasInterAction::ReqProcMsg;
    cReqRroc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4TasInterAction::ReqTimerPoll;
    cReqRroc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4TasInterAction::ReqProcMsg;
    AddRuleProc(Req, cReqRroc);
    CStateProc cRspProc;
    cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4TasInterAction::RspProcMsg;
    cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4TasInterAction::RspTimerPoll;
    cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4TasInterAction::RspProcMsg;
    AddRuleProc(Rsp, cRspProc);
    NextState(Req);
}
u32 CExportTask4TasInterAction::ReqProcMsg( CMessage *const pcMsg )
{
    u32 dwRet = PROCMSG_OK;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�CU�û��Ự[%s]������[%s--%hu]\n",
        m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    m_dwReqEventID = pcMsg->event;
    switch (pcMsg->event)
    {
    case FILE_EXPORT_REQ:
        {
            CCuFileExportReq *pcReq = (CCuFileExportReq *)pcMsg->content;

            CExportTask4Tas* pcTask = new CExportTask4Tas(GetInstance(), m_strCuSession, this, g_cCuiConfig.GetLocalURI().Get3ASURI().GetURIString());
            pcTask->InitStateMachine();
            pcTask->StartSendExportReq(*pcReq);

            NextState(Rsp);
        }
        break;
    case CU_CUI_STOP_EXPORT_REQ:
        {
            CCuStopExportReq* pcReq = (CCuStopExportReq *)pcMsg->content;
            string strFileUri(pcReq->GetExportFileUrl());
            if (strFileUri.empty())
            {
                map<string, string>::const_iterator it = s_mapCuSession2FileUri.find(pcReq->GetSession());
                if (it != s_mapCuSession2FileUri.end())
                {
                    strFileUri = it->second;
                }
            }

            CExportTask4Tas *pcTask = (CExportTask4Tas *)GetExportTask(strFileUri);
            if( NULL != pcTask)
            {
                pcTask->StartSendExportBye();
                pcTask->m_pcExportActTask = this;
                NextState(Rsp);
            }
            else
            {
                CCuStopExportRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                SendRspToCu<CCuStopExportRsp>(cRsp, m_ptRecvNode);
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��Exportû�ж�ӦTask\n",
                    m_strCuSession.c_str());
                dwRet = PROCMSG_DEL;
                delete this;
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask4TasInterAction::ReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ\n",
            m_strCuSession.c_str());
        PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        NextState(Req);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask4TasInterAction::RspProcMsg( CMessage *const pcMsg )
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ���ϢӦ��[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "��CU�û��Ự[%s]����Ӧ����Ϣʧ�ܣ�������[%d]\n",
                m_strCuSession.c_str(), pcOspSipMsg->GetSipErrorCode());
            NextState(Req);
            dwRet =  PROCMSG_DEL;
        }
        break;
    case FILE_EXPORT_RSP:
        {
            pcOspSipMsg->GetMsgBody(m_cExportRsp);
            if (INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
            {
                m_cExportRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
            }

            if (m_cExportRsp.GetErrorCode() != CMS_SUCCESS || m_cExportRsp.GetFileURL().empty())
            {
                SendRspToCu<CCuFileExportRsp>(m_cExportRsp, m_ptRecvNode);
                NextState(Req);
                dwRet = PROCMSG_DEL;
            }
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            CFileCollectProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            if (cReq.GetPercent() == 100)
            {
                SendRspToCu<CCuFileExportRsp>(m_cExportRsp, m_ptRecvNode);
                NextState(Req);
                dwRet = PROCMSG_DEL;
            }
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            CCuStopExportRsp cRsp;
            SendRspToCu<CCuStopExportRsp>(cRsp, m_ptRecvNode);
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
                m_cExportRsp.SetErrorCode(cByeReq.GetReason());
            }
            else
            {
                m_cExportRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED);
            }
            SendRspToCu<CCuFileExportRsp>(m_cExportRsp, m_ptRecvNode);
            NextState(Req);
            dwRet = PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask4TasInterAction::RspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Ӧ����Ϣ��ʱ\n",
            m_strCuSession.c_str());
        PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        NextState(Req);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
CTask *CExportTask4TasInterAction::GetExportTask(const string& strFileUrl)
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
void CExportTask4TasInterAction::PostErrorMsg2Cu(u32 dwErrorCode)
{
    switch( m_dwReqEventID)
    {
    case FILE_EXPORT_REQ:
        {
            SendErrorMsg2Cu<CCuFileExportRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_STOP_EXPORT_REQ:
        {
            SendErrorMsg2Cu<CCuStopExportRsp>(dwErrorCode);
        }
        break;
    default:
        {
            SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
        }
        break;
    }
}

CExportTask::CExportTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());
    CMS_ASSERT(NULL != pcExportActTask);
    m_pcExportActTask = pcExportActTask;
    m_tInviteID = INVALID_DIALOG_ID;
	m_tNtfReq.Clear();
}
CExportTask::~CExportTask()
{
    TerminateTask();
}
void CExportTask::TerminateTask()
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
    m_pcExportActTask = NULL;
}
void CExportTask::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask::NotReqProcMsg;
    cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask::NotReqTimerPoll;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask::NotReqProcMsg;
    AddRuleProc(NotReq, cNotReqState);
    CStateProc cWaitRspState;
    cWaitRspState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask::WaitRspProcMsg;
    cWaitRspState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask::WaitRspTimerPoll;
    cWaitRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask::WaitRspProcMsg;
    AddRuleProc(WaitRsp, cWaitRspState);
    CStateProc cWaitAckState;
    cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask::WaitAckProcMsg;
    cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask::WaitAckTimerPoll;
    cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask::WaitAckProcMsg;
    AddRuleProc(WaitAck, cWaitAckState);
    CStateProc cHoldState;
    cHoldState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask::HoldProcMsg;
    cHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask::HoldProcMsg;
    AddRuleProc(Hold, cHoldState);
    CStateProc cWaitByeState;
    cWaitByeState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask::WaitByeProcMsg;
    cWaitByeState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask::WaitByeTimerPoll;
    cWaitByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask::WaitByeProcMsg;
    AddRuleProc(WaitBye, cWaitByeState);
    NextState(NotReq);
}
u32 CExportTask::StartSendExportReq(CFileExportReq &cReq)
{
    u32 dwRet = PROCMSG_OK;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]������־��������\n",
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
            dwRet = PostReq<CFileExportReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
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
u32 CExportTask::StartSendExportAck()
{
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportAck����, Sip DialogIDΪ %u\n",
        m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
    CFileExportAck cAck;
    cAck.SetSession(m_strPeSession);
    u32 dwRet = PROCMSG_OK;
    dwRet = PostInDlgReq<CFileExportAck>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
    NextState(Hold);
    return dwRet;
}
u32 CExportTask::StartSendExportBye()
{
    u32 dwRet = PROCMSG_OK;
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportBye����, Sip DialogIDΪ %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
        string strNULL;
        dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
    }
    NextState(WaitBye);
    return dwRet;
}
u32 CExportTask::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;
    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(NULL != pcPeAgent);
    if(pcPeAgent->GetState() != CPeAgent::Service)
    {
        if(m_pcExportActTask != NULL)
        {
            CExportTaskInterAction *pcExportActTask= (CExportTaskInterAction *)m_pcExportActTask;
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
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����[FILE_EXPORT_REQ]��Ϣʧ��\n",
                m_strCuSession.c_str());
            return dwRet;
        }
        NextState(WaitRsp);
    }  
    return dwRet;
}
CFileCollectProgressNtfReq &CExportTask::GetProcessNtfReq()
{
    return m_tNtfReq;
}

void CExportTask::NotifyStop2Cu(s32 dwErrorCode)
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

u32 CExportTask::NotReqProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CExportTask::NotReqProcMsg]�յ�δ֪��Ϣ[%s--%hu]��������\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CExportTask::NotReqProcMsg�յ�δ֪��Ϣ");
    return PROCMSG_OK;
}
u32 CExportTask::NotReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�ע������ģ�鳬ʱ������CExportTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask::WaitRspProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //��ʵ���ﲻ���ܻ��յ�Bye����Ϊ���л�δ������Ҫô�ɹ�Ҫô�ܾ����յ�403������
        {
            CMS_ASSERT(0&&"��������WaitRsp״̬���յ�BYE��Ϣ");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            if ( m_pcExportActTask != NULL )
            {
                dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL ==  dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTask[TaskNO: %u](SIP��Ϣ[%s--%d])\n",
                GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_EXPORT_RSP:
        {
            CFileExportRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;
            m_strExportFileUrl = cRsp.GetFileURL();
            if ( m_pcExportActTask != NULL )
            {
                if(PROCMSG_DEL == m_pcExportActTask->ProcMsg(pcMsg))
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���DialogID��Ч !!!!ֱ������CExportTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NotReq);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }
            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "��Ϣ����ʧ�ܣ�������[%d]������CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO:%u]�û�[%s]��־��������ɹ�\n",
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
                StartSendExportAck();  
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ������CExportTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask::WaitAckProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO %u]�յ�Bye Req��Ϣ��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcExportActTask != NULL )
            {
                dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTask[TaskNO: %u](SIPӦ����Ϣ[%s--%d])\n",
                GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            delete this;
            dwRet = PROCMSG_DEL;
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
    return dwRet;
}
u32 CExportTask::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "�ȴ�CU�û��Ự[%s]����Ack��Ϣ��ʱ��ֱ�ӷ���Bye����\n",
            m_strCuSession.c_str());
        StartSendExportBye();
        return TIMERPOLL_DONE;  //����ByeReq֮���л���WaitByeRsp״̬�ˣ�����ɾ��
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask::HoldProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if( m_pcExportActTask != NULL)
            {
                dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            if ( m_pcExportActTask != NULL )
            {
                m_pcExportActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
                GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            pcOspSipMsg->GetMsgBody(m_tNtfReq);
            CFileCollectProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(m_tNtfReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask::WaitByeProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d]\n",
        m_strCuSession.c_str(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO %u]�յ�Bye Req��DialogID[%u]���ظ�Ӧ�������CExportTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, EVENT_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
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
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            pcOspSipMsg->GetMsgBody(m_tNtfReq);
            CFileCollectProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(m_tNtfReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask::WaitByeTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Bye Rsp��ʱ������Dialog\n",
            m_strCuSession.c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}



CExportTask4Tas::CExportTask4Tas(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strTasUri) : CCallBackTask(pcInst, strCuSession, strTasUri)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strTasUri.empty());
    CMS_ASSERT(NULL != pcExportActTask);
    m_pcExportActTask = pcExportActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    
    CCuAgent *pcCuAgent = dynamic_cast<CCuAgent *>(g_cCuiDataMgr.GetTaskPtrBySession(strCuSession, pcInst));
    if (NULL != pcCuAgent)
    {
        m_strCuSessionInTas = pcCuAgent->m_strCuSessionIn3AS;
    }
}
CExportTask4Tas::~CExportTask4Tas()
{
    TerminateTask();
}
void CExportTask4Tas::TerminateTask()
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
    }
    m_pcExportActTask = NULL;

    if (!m_tRsp.GetSession().empty())
    {
        s_mapCuSession2FileUri.erase(m_tReq.GetSession());
    }
}
void CExportTask4Tas::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::NotReqProcMsg;
    cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4Tas::NotReqTimerPoll;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::NotReqProcMsg;
    AddRuleProc(NotReq, cNotReqState);
    CStateProc cWaitRspState;
    cWaitRspState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitRspProcMsg;
    cWaitRspState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4Tas::WaitRspTimerPoll;
    cWaitRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitRspProcMsg;
    AddRuleProc(WaitRsp, cWaitRspState);
    CStateProc cWaitAckState;
    cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitAckProcMsg;
    cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4Tas::WaitAckTimerPoll;
    cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitAckProcMsg;
    AddRuleProc(WaitAck, cWaitAckState);
    CStateProc cHoldState;
    cHoldState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::HoldProcMsg;
    cHoldState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4Tas::HoldTimerPoll;
    cHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::HoldProcMsg;
    AddRuleProc(Hold, cHoldState);
    CStateProc cWaitByeState;
    cWaitByeState.ProcMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitByeProcMsg;
    cWaitByeState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTask4Tas::WaitByeTimerPoll;
    cWaitByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTask4Tas::WaitByeProcMsg;
    AddRuleProc(WaitBye, cWaitByeState);
    NextState(NotReq);
}
u32 CExportTask4Tas::StartSendExportReq(CFileExportReq &cReq)
{
    u32 dwRet = PROCMSG_OK;
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    m_tReq = cReq;

    cReq.SetSession(m_strCuSessionInTas);
    dwRet = PostReq<CFileExportReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
    if(dwRet != PROCMSG_OK)
    {
        return dwRet;
    }
    NextState(WaitRsp);
    return dwRet;
}
u32 CExportTask4Tas::StartSendExportAck()
{
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportAck����, Sip DialogIDΪ %u\n",
        m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
    CFileExportAck cAck;
    cAck.SetSession(m_strCuSessionInTas);
    u32 dwRet = PROCMSG_OK;
    dwRet = PostInDlgReq<CFileExportAck>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
    NextState(Hold);
    return dwRet;
}
u32 CExportTask4Tas::StartSendExportBye()
{
    u32 dwRet = PROCMSG_OK;
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportBye����, Sip DialogIDΪ %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
        string strNULL;
        dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
    }
    NextState(WaitBye);
    return dwRet;
}
u32 CExportTask4Tas::StartSendReq()
{
    // TAS�Ǳ߲�ʵ��
    u32 dwRet = PROCMSG_OK;
    return dwRet;
}
CFileCollectProgressNtfReq &CExportTask4Tas::GetProcessNtfReq()
{
    return m_tNtfReq;
}

void CExportTask4Tas::NotifyStop2Cu(s32 dwErrorCode)
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

u32 CExportTask4Tas::NotReqProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CExportTask4Tas::NotReqProcMsg]�յ�δ֪��Ϣ[%s--%hu]��������\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CExportTask4Tas::NotReqProcMsg�յ�δ֪��Ϣ");
    return PROCMSG_OK;
}
u32 CExportTask4Tas::NotReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�ע������ģ�鳬ʱ������CExportTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask4Tas::WaitRspProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //��ʵ���ﲻ���ܻ��յ�Bye����Ϊ���л�δ������Ҫô�ɹ�Ҫô�ܾ����յ�403������
        {
            CMS_ASSERT(0&&"��������WaitRsp״̬���յ�BYE��Ϣ");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //if ( m_pcExportActTask != NULL )
            //{
            //    dwRet = m_pcExportActTask->ProcMsg(pcMsg);
            //    if(PROCMSG_DEL ==  dwRet)
            //    {
            //        delete m_pcExportActTask;
            //        m_pcExportActTask = NULL;
            //    }
            //}
			m_pcExportActTask = NULL;//CExportTask4TasInterAction�����г�ʱɾ�����˴�����ɾ���ˡ�
            TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTask[TaskNO: %u](SIP��Ϣ[%s--%d])\n",
                GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_EXPORT_RSP:
        {
            CFileExportRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;
            m_strExportFileUrl = cRsp.GetFileURL();
            if ( m_pcExportActTask != NULL )
            {
                if(PROCMSG_DEL == m_pcExportActTask->ProcMsg(pcMsg))
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���DialogID��Ч !!!!ֱ������CExportTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NotReq);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }
			if (m_strExportFileUrl.empty())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "FileUrl is Empty!!!!\n");
				m_tInviteID = INVALID_DIALOG_ID;
				NextState(NotReq);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "��Ϣ����ʧ�ܣ�������[%d]������CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO:%u]�û�[%s]�ļ�[%s]��������ɹ�\n",
                    GetTaskNO(), m_strCuSession.c_str(), m_strExportFileUrl.c_str());
                CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
                if(pcCuAgent != NULL)
                {
					u32 dwOldTaskNO = 0;
					if (pcCuAgent->m_cFileExportTaskSet.Find(m_strExportFileUrl, dwOldTaskNO))
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "ExportFileUrl[%s] is Exist, OldTaskNO[%u], CurrTaskNO[%u]\n",
							m_strExportFileUrl.c_str(), dwOldTaskNO, GetTaskNO());
						dwRet = PROCMSG_DEL;
						delete this;
						break;
					}
                    pcCuAgent->m_cFileExportTaskSet.Insert(m_strExportFileUrl, GetTaskNO());
                }
                CMS_ASSERT(s_mapCuSession2FileUri.find(m_tReq.GetSession()) == s_mapCuSession2FileUri.end());
                s_mapCuSession2FileUri[m_tReq.GetSession()] = m_strExportFileUrl;
                NextState(WaitAck);
                StartSendExportAck();  
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask4Tas::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ������CExportTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask4Tas::WaitAckProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO %u]�յ�Bye Req��Ϣ��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcExportActTask != NULL )
            {
                dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            //if ( m_pcExportActTask != NULL )
            //{
            //    u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
            //    if(PROCMSG_DEL == dwRet)
            //    {
            //        delete m_pcExportActTask;
            //        m_pcExportActTask = NULL;
            //    }
            //}
			m_pcExportActTask = NULL;//CExportTask4TasInterAction�����г�ʱɾ�����˴�����ɾ���ˡ�
            TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTask[TaskNO: %u](SIPӦ����Ϣ[%s--%d])\n",
                GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            delete this;
            dwRet = PROCMSG_DEL;
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
    return dwRet;
}
u32 CExportTask4Tas::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "�ȴ�CU�û��Ự[%s]����Ack��Ϣ��ʱ��ֱ�ӷ���Bye����\n",
            m_strCuSession.c_str());
        StartSendExportBye();
        return TIMERPOLL_DONE;  //����ByeReq֮���л���WaitByeRsp״̬�ˣ�����ɾ��
    }
    return TIMERPOLL_DONE;
}
u32 CExportTask4Tas::HoldProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if( m_pcExportActTask != NULL)
            {
                dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            //if ( m_pcExportActTask != NULL )
            //{
            //    m_pcExportActTask->ProcMsg(pcMsg);
            //}
			m_pcExportActTask = NULL;//CExportTask4TasInterAction�����г�ʱɾ�����˴�����ɾ���ˡ�
            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
                GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            pcOspSipMsg->GetMsgBody(m_tNtfReq);
            CFileCollectProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(m_tNtfReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            if ( m_pcExportActTask != NULL )
            {
                if(PROCMSG_DEL == m_pcExportActTask->ProcMsg(pcMsg))
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask4Tas::WaitByeProcMsg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d]\n",
        m_strCuSession.c_str(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO %u]�յ�Bye Req��DialogID[%u]���ظ�Ӧ�������CExportTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
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
            if ( m_pcExportActTask != NULL )
            {
                u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcExportActTask;
                    m_pcExportActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, EVENT_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
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
            //if ( m_pcExportActTask != NULL )
            //{
            //    u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
            //    if(PROCMSG_DEL == dwRet)
            //    {
            //        delete m_pcExportActTask;
            //        m_pcExportActTask = NULL;
            //    }
            //}
			m_pcExportActTask = NULL;//CExportTask4TasInterAction�����г�ʱɾ�����˴�����ɾ���ˡ�
            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NotReq);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTask4Tas[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            pcOspSipMsg->GetMsgBody(m_tNtfReq);
            CFileCollectProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(m_tNtfReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}
u32 CExportTask4Tas::WaitByeTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Bye Rsp��ʱ������Dialog\n",
            m_strCuSession.c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

void CExportTask4Tas::PrintData() const
{
    CCallBackTask::PrintData();

    OspPrintf(TRUE, FALSE, "Cuser Session:%s, FileUri:%s\n", m_tReq.GetSession().c_str(), m_tRsp.GetFileURL().c_str());
}

u32 CExportTask4Tas::HoldTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]����Hold��ʱ������bye\n",
            m_strCuSession.c_str());
        StartSendExportBye();
    }
    return TIMERPOLL_DONE;
}

CExportTaskExInterAction::CExportTaskExInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(ptRecvNode != NULL);
	m_ptRecvNode = ptRecvNode;
	m_strCuSession = strCuSession;
	m_dwInviteReqTaskNo = 0;
}
CExportTaskExInterAction::~CExportTaskExInterAction()
{
	TerminateTask();
}
void CExportTaskExInterAction::TerminateTask()
{
	if( !m_stFileUrl.empty())
	{
		CExportTaskEx *pcTask = (CExportTaskEx *)GetExportTask(m_stFileUrl);
		if( NULL != pcTask)
		{
			if ( pcTask->m_pcExportActTask == this )
			{
				pcTask->m_pcExportActTask = NULL;
			}
		}
	}
}
void CExportTaskExInterAction::InitStateMachine()
{
	CStateProc cReqRroc;
	cReqRroc.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskExInterAction::ReqProcMsg;
	cReqRroc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskExInterAction::ReqTimerPoll;
	cReqRroc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskExInterAction::ReqProcMsg;
	AddRuleProc(Req, cReqRroc);
	CStateProc cRspProc;
	cRspProc.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskExInterAction::RspProcMsg;
	cRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskExInterAction::RspTimerPoll;
	cRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskExInterAction::RspProcMsg;
	AddRuleProc(Rsp, cRspProc);
	NextState(Req);
}
u32 CExportTaskExInterAction::ReqProcMsg( CMessage *const pcMsg )
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "�յ�CU�û��Ự[%s]������[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	m_dwReqEventID = pcMsg->event;
	switch (pcMsg->event)
	{
	case CU_CUI_FILE_EXPORT_START_EX_REQ:
		{
			CCuFileExportStartExReq *pcReq = (CCuFileExportStartExReq *)pcMsg->content;

			string strPeUri = g_cCuiConfig.GetLocalURI().GetUasURI().GetURIString();

			if ( pcReq->GetSrvName() == FILE_EXPORT_GB_CATALOG
				|| pcReq->GetSrvName() == FILE_EXPORT_GB_CATALOG_REPORT_CFG)
			{
				strPeUri = string(DEV_TYPE_GBS) + "@" + GetDomainName();
			}
			else if ( pcReq->GetSrvName() == FILE_EXPORT_DEVICE_SATATISTICS)
			{
				strPeUri = string(DEV_TYPE_DPS) + "@" + GetDomainName();
			}

			CFileExportReq cReq;
			cReq.SetHeadFromReq(*pcReq);
			cReq.SetSrvName(pcReq->GetSrvName());
			cReq.SetFileToken(pcReq->GetFileToken());
			cReq.SetFileTimeZone(pcReq->GetFileTimeZone());
			cReq.SetFileSenderUri(pcReq->GetFileSenderUri());
			cReq.SetFileReceiverUri(pcReq->GetFileReceiverUri());
			cReq.SetFileCharacterset(pcReq->GetFileCharacterset());
			cReq.CustomParames() = pcReq->GetCustomParams();

			CExportTaskEx* pcTask = new CExportTaskEx(GetInstance(), m_strCuSession, this, strPeUri);
			pcTask->InitStateMachine();
			pcTask->StartSendExportReq(cReq);
			m_dwInviteReqTaskNo = pcTask->GetTaskNO();

			NextState(Rsp);
		}
		break;
	case CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ:
		{
			CCuFileExportGetProcessExReq* pcReq = (CCuFileExportGetProcessExReq *)pcMsg->content;

			CExportTaskEx *pcTask = (CExportTaskEx *)(this->GetInstance()->FindTask(pcReq->GetTaskNo()));

			CCuFileExportGetProcessExRsp cCuRsp;
			if( NULL != pcTask)
			{
				CFileCollectProgressNtfReq ntfReq = pcTask->GetProcessNtfReq();

				cCuRsp.SetPercent(ntfReq.GetPercent());
				cCuRsp.SetErrorCode(CMS_SUCCESS);
			}
			else
			{
				cCuRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��Exportû�ж�ӦTask[%u]\n",
					m_strCuSession.c_str(), pcReq->GetTaskNo());
			}

			SendRspToCu<CCuFileExportGetProcessExRsp>(cCuRsp, m_ptRecvNode);
			dwRet = PROCMSG_DEL;
			delete this;
		}
		break;
	case CU_CUI_FILE_EXPORT_STOP_EX_REQ:
		{
			CCuFileExportStopExReq* pcReq = (CCuFileExportStopExReq *)pcMsg->content;

			CExportTaskEx *pcTask = (CExportTaskEx *)(this->GetInstance()->FindTask(pcReq->GetTaskNo()));
			if( NULL != pcTask)
			{
				pcTask->StartSendExportBye();
				pcTask->m_pcExportActTask = this;
				NextState(Rsp);
			}
			else
			{
				CCuFileImportStopExRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				SendRspToCu<CCuFileImportStopExRsp>(cRsp, m_ptRecvNode);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]��Exportû�ж�ӦTask[%u]\n",
					m_strCuSession.c_str(), pcReq->GetTaskNo());
				dwRet = PROCMSG_DEL;
				delete this;
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CExportTaskExInterAction::ReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CExportTaskExInterAction::RspProcMsg( CMessage *const pcMsg )
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ���ϢӦ��[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "��CU�û��Ự[%s]����Ӧ����Ϣʧ�ܣ�������[%d]\n",
				m_strCuSession.c_str(), pcOspSipMsg->GetSipErrorCode());
			NextState(Req);
			dwRet =  PROCMSG_DEL;
		}
		break;
	case FILE_EXPORT_RSP:
		{
			CFileExportRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_stFileUrl = cRsp.GetFileURL();

			CCuFileExportStartExRsp cCuRsp;
			cCuRsp.SetSession(cRsp.GetSession());
			cCuRsp.SetFileURL(cRsp.GetFileURL());
			cCuRsp.SetTaskNo(m_dwInviteReqTaskNo);
			cCuRsp.SetErrorCode(cRsp.GetErrorCode());
			cCuRsp.SetTaskType(DEF_TASK_TYPE_FILE_EXPORT_TASK_EX);

			if (INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
			{
				cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			}
			SendRspToCu<CCuFileExportStartExRsp>(cCuRsp, m_ptRecvNode);
			NextState(Req);
			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			CCuFileImportStopExRsp cRsp;
			SendRspToCu<CCuFileImportStopExRsp>(cRsp, m_ptRecvNode);
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
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CExportTaskExInterAction::RspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Ӧ����Ϣ��ʱ\n",
			m_strCuSession.c_str());
		PostErrorMsg2Cu(ERR_CUI_OPR_TIME_OUT);
		NextState(Req);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
CTask *CExportTaskExInterAction::GetExportTask(const string& strFileUrl)
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
void CExportTaskExInterAction::PostErrorMsg2Cu(u32 dwErrorCode)
{
	switch( m_dwReqEventID)
	{
	case FILE_EXPORT_REQ:
		{
			SendErrorMsg2Cu<CCuFileImportStartExRsp>(dwErrorCode);
		}
		break;
	case CU_CUI_STOP_EXPORT_REQ:
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

CExportTaskEx::CExportTaskEx(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(!strPeUri.empty());
	CMS_ASSERT(NULL != pcExportActTask);
	m_pcExportActTask = pcExportActTask;
	m_tInviteID = INVALID_DIALOG_ID;
	m_tNtfReq.Clear();
}
CExportTaskEx::~CExportTaskEx()
{
	TerminateTask();
}
void CExportTaskEx::TerminateTask()
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
	m_pcExportActTask = NULL;
}
void CExportTaskEx::InitStateMachine()
{
	CStateProc cNotReqState;
	cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskEx::NotReqProcMsg;
	cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskEx::NotReqTimerPoll;
	cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskEx::NotReqProcMsg;
	AddRuleProc(NotReq, cNotReqState);
	CStateProc cWaitRspState;
	cWaitRspState.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitRspProcMsg;
	cWaitRspState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskEx::WaitRspTimerPoll;
	cWaitRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitRspProcMsg;
	AddRuleProc(WaitRsp, cWaitRspState);
	CStateProc cWaitAckState;
	cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitAckProcMsg;
	cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskEx::WaitAckTimerPoll;
	cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitAckProcMsg;
	AddRuleProc(WaitAck, cWaitAckState);
	CStateProc cHoldState;
	cHoldState.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskEx::HoldProcMsg;
	cHoldState.TimerPoll = &CSipTask::TimerPollSkip;
	cHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskEx::HoldProcMsg;
	AddRuleProc(Hold, cHoldState);
	CStateProc cWaitByeState;
	cWaitByeState.ProcMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitByeProcMsg;
	cWaitByeState.TimerPoll = (CSipTask::PFTimerPoll)&CExportTaskEx::WaitByeTimerPoll;
	cWaitByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CExportTaskEx::WaitByeProcMsg;
	AddRuleProc(WaitBye, cWaitByeState);
	NextState(NotReq);
}
u32 CExportTaskEx::StartSendExportReq(CFileExportReq &cReq)
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�����ļ���������\n",
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
			dwRet = PostReq<CFileExportReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
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
u32 CExportTaskEx::StartSendExportAck()
{
	CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportAck����, Sip DialogIDΪ %u\n",
		m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
	CFileExportAck cAck;
	cAck.SetSession(m_strPeSession);
	u32 dwRet = PROCMSG_OK;
	dwRet = PostInDlgReq<CFileExportAck>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
	NextState(Hold);
	return dwRet;
}
u32 CExportTaskEx::StartSendExportBye()
{
	u32 dwRet = PROCMSG_OK;
	if( m_tInviteID != INVALID_DIALOG_ID)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]ͨ��TaskNO[%u]����ExportBye����, Sip DialogIDΪ %u\n",
			m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
		string strNULL;
		dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
	}
	NextState(WaitBye);
	return dwRet;
}
u32 CExportTaskEx::StartSendReq()
{
	u32 dwRet = PROCMSG_OK;
	CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
	CMS_ASSERT(NULL != pcPeAgent);
	if(pcPeAgent->GetState() != CPeAgent::Service)
	{
		if(m_pcExportActTask != NULL)
		{
			CExportTaskExInterAction *pcExportActTask= (CExportTaskExInterAction *)m_pcExportActTask;
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
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]����[FILE_EXPORT_REQ]��Ϣʧ��\n",
				m_strCuSession.c_str());
			return dwRet;
		}
		NextState(WaitRsp);
	}  
	return dwRet;
}
CFileCollectProgressNtfReq &CExportTaskEx::GetProcessNtfReq()
{
	return m_tNtfReq;
}

void CExportTaskEx::NotifyStop2Cu(s32 dwErrorCode)
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

u32 CExportTaskEx::NotReqProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, CRITICAL_LEV, "��[CExportTaskEx::NotReqProcMsg]�յ�δ֪��Ϣ[%s--%hu]��������\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	CMS_ASSERT(0&&"CExportTaskEx::NotReqProcMsg�յ�δ֪��Ϣ");
	return PROCMSG_OK;
}
u32 CExportTaskEx::NotReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�ע������ģ�鳬ʱ������CExportTaskEx[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CExportTaskEx::WaitRspProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:   //��ʵ���ﲻ���ܻ��յ�Bye����Ϊ���л�δ������Ҫô�ɹ�Ҫô�ܾ����յ�403������
		{
			CMS_ASSERT(0&&"��������WaitRsp״̬���յ�BYE��Ϣ");
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			if ( m_pcExportActTask != NULL )
			{
				dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL ==  dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTaskEx[TaskNO: %u](SIP��Ϣ[%s--%d])\n",
				GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_EXPORT_RSP:
		{
			CFileExportRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			m_tInviteID = pcOspSipMsg->GetSipDlgID();
			m_tRsp = cRsp;
			m_strExportFileUrl = cRsp.GetFileURL();
			if ( m_pcExportActTask != NULL )
			{
				if(PROCMSG_DEL == m_pcExportActTask->ProcMsg(pcMsg))
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
				}
			}
			if ( INVALID_DIALOG_ID == m_tInviteID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���DialogID��Ч !!!!ֱ������CExportTaskEx[TaskNO %u]\n",
					GetTaskNO());
				m_tInviteID = INVALID_DIALOG_ID;
				NextState(NotReq);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			if ( CMS_SUCCESS != cRsp.GetErrorCode() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "��Ϣ����ʧ�ܣ�������[%d]������CExportTaskEx[TaskNO: %u]������Sip Dialog: [%u]\n",
					cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);
				dwRet = PROCMSG_DEL;
				delete this;
				break;
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO:%u]�û�[%s]�ļ���������ɹ�\n",
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
				StartSendExportAck();  
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CExportTaskEx::WaitRspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ���ϢӦ��ʱ������CExportTaskEx[TaskNO: %u]\n",
			m_strCuSession.c_str(), GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
u32 CExportTaskEx::WaitAckProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO %u]�յ�Bye Req��Ϣ��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if ( m_pcExportActTask != NULL )
			{
				dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
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
			if ( m_pcExportActTask != NULL )
			{
				u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "������Ϣʧ�ܣ�����CExportTaskEx[TaskNO: %u](SIPӦ����Ϣ[%s--%d])\n",
				GetTaskNO(),pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			delete this;
			dwRet = PROCMSG_DEL;
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
	return dwRet;
}
u32 CExportTaskEx::WaitAckTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "�ȴ�CU�û��Ự[%s]����Ack��Ϣ��ʱ��ֱ�ӷ���Bye����\n",
			m_strCuSession.c_str());
		StartSendExportBye();
		return TIMERPOLL_DONE;  //����ByeReq֮���л���WaitByeRsp״̬�ˣ�����ɾ��
	}
	return TIMERPOLL_DONE;
}
u32 CExportTaskEx::HoldProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_strCuSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO %u]�յ�Bye Req����DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if( m_pcExportActTask != NULL)
			{
				dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
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
			if ( m_pcExportActTask != NULL )
			{
				m_pcExportActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTaskEx[TaskNO: %u]������Sip Dialog: [%u] (SIPӦ����Ϣ[%s--%d])\n",
				GetTaskNO(), m_tInviteID, pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			pcOspSipMsg->GetMsgBody(m_tNtfReq);
			CFileCollectProgressNtfRsp cRsp;
			cRsp.SetHeadFromReq(m_tNtfReq);
			PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CExportTaskEx::WaitByeProcMsg(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û��Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d]\n",
		m_strCuSession.c_str(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:   
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO %u]�յ�Bye Req��DialogID[%u]���ظ�Ӧ�������CExportTaskEx\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			if ( m_pcExportActTask != NULL )
			{
				u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
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
			if ( m_pcExportActTask != NULL )
			{
				u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, EVENT_LEV, "����CExportTaskEx[TaskNO: %u]������Sip Dialog: [%u]\n",
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
			if ( m_pcExportActTask != NULL )
			{
				u32 dwRet = m_pcExportActTask->ProcMsg(pcMsg);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcExportActTask;
					m_pcExportActTask = NULL;
				}
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CExportTaskEx[TaskNO: %u]������Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			NextState(NotReq);
			delete this;
			dwRet = PROCMSG_DEL;
		}
		break;
	case FILE_COLLECT_PROGRESS_NTF_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CExportTaskEx[TaskNO %u]�յ�File Collect Progress Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			pcOspSipMsg->GetMsgBody(m_tNtfReq);
			CFileCollectProgressNtfRsp cRsp;
			cRsp.SetHeadFromReq(m_tNtfReq);
			PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}
u32 CExportTaskEx::WaitByeTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Bye Rsp��ʱ������Dialog\n",
			m_strCuSession.c_str());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}
