#include "file_export_task.h"
#include "cms/utility/mischelper.h"
#include "cms/utility/digital_string.h"
#include "cms/commonconfig/cmcfg.h"
#include <map>
using std::map;

static u32 s_dwFileExportTaskNum = 0;
map<string, CFileExportTask*> s_mapFileExportTask;


CFileExportTask::CFileExportTask( CInstExt *pcInst ):CSipTask(pcInst), m_tInvId(INVALID_DIALOG_ID),m_bAbortExport(false)
{
    ++s_dwFileExportTaskNum;
}

CFileExportTask::~CFileExportTask()
{
    if (m_tInvId != INVALID_DIALOG_ID)
    {
        OspSipReleaseDlg(m_tInvId);
        m_tInvId = INVALID_DIALOG_ID;
    }

    if (!m_cReq.GetSession().empty())
    {
        string strFile("/dev/shm/tas/");
        strFile.append(m_cReq.GetSession());
        strFile.append(".zip");
        remove(strFile.c_str());

        CMS_ASSERT(s_mapFileExportTask.find(m_cReq.GetSession()) != s_mapFileExportTask.end());
        s_mapFileExportTask.erase(m_cReq.GetSession());
    }

    --s_dwFileExportTaskNum;
}

void CFileExportTask::InitStateMachine()
{
    CStateProc cReqState;
    cReqState.ProcMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcReqMsg;
    cReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcErrMsg;
    cReqState.TimerPoll = (CSipTask::PFTimerPoll)&CFileExportTask::WaitReqTimerPoll;
    AddRuleProc(REQ, cReqState);

    CStateProc cAckState;
    cAckState.ProcMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcAckMsg;
    cAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcErrMsg;
    cAckState.TimerPoll = (CSipTask::PFTimerPoll)&CFileExportTask::WaitAckTimerPoll;
    AddRuleProc(ACK, cAckState);

    CStateProc cByeState;
    cByeState.ProcMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcByeMsg;
    cByeState.ProcErrMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcErrMsg;
    cByeState.TimerPoll = (CSipTask::PFTimerPoll)&CFileExportTask::WaitByeTimerPoll;
    AddRuleProc(BYE, cByeState);

    CStateProc cExportState;
    cExportState.ProcMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcExportMsg;
    cExportState.ProcErrMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcExportMsg;
    cExportState.TimerPoll = (CSipTask::PFTimerPoll)&CFileExportTask::WaitExportTimerPoll;
    AddRuleProc(EXPORT, cExportState);

    CStateProc cTransferState;
    cTransferState.ProcMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcTransferMsg;
    cTransferState.ProcErrMsg = (CSipTask::PFProcMsg)&CFileExportTask::ProcErrMsg;
    cTransferState.TimerPoll = (CSipTask::PFTimerPoll)&CFileExportTask::WaitTransferTimerPoll;
    AddRuleProc(TRANSFER, cTransferState);

    NextState(REQ);
}

u32 CFileExportTask::ProcReqMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == FILE_EXPORT_REQ)
    {
        COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
        pcSipMsg->GetMsgBody(m_cReq);
        m_tInvId = pcSipMsg->GetSipDlgID();
        CMS_ASSERT(s_mapFileExportTask.find(m_cReq.GetSession()) == s_mapFileExportTask.end());
        s_mapFileExportTask[m_cReq.GetSession()] = this;

        CFileExportRsp cRsp;
        cRsp.SetSeqNum(m_cReq.GetSeqNum());
        cRsp.SetSession(m_cReq.GetSession());

        string strFileUri;
        strFileUri.append("proxy/");
        strFileUri.append(GetLocalIPAddress());
        strFileUri.append(":");
        strFileUri.append(digital_string(::GetHttpPort()));
        strFileUri.append("/shm/tas/");
        strFileUri.append(m_cReq.GetSession());
        strFileUri.append(".zip?1");
        cRsp.SetFileURL(strFileUri);
        
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

u32 CFileExportTask::WaitReqTimerPoll()
{
    return TIMERPOLL_DONE;
}

u32 CFileExportTask::ProcAckMsg( CMessage *const pcMsg )
{
    u32 dwProcRet = PROCMSG_OK;
    COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    switch(pcMsg->event)
    {
    case FILE_EXPORT_ACK:
        {
            if (m_cReq.GetSrvName() == FILE_EXPORT_CUSER_DEVICEDATA)
            {
                CTaskMsgTransfer<CFileExportReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), m_cReq.GetEventId());
                cTrans.SetMsgObj(m_cReq); 
                if (PostTaskMsgTransfer2DbTaskPool(cTrans))
                {
                    PostProcessNtf(CMS_SUCCESS, 0);
                }
                else
                {
                    PostProcessNtf(ERR_TAS_SYSTEM_BUSY, 0);
                    dwProcRet = PROCMSG_DEL;
                }
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

u32 CFileExportTask::WaitAckTimerPoll()
{
    if (GetCurStateHoldTime() < 60)
    {
        return TIMERPOLL_DONE;
    }

    Log(WARNING_LEV, "detected ack timeout, ObjName is %s\n", GetObjName());
    return TIMERPOLL_DEL;
}

u32 CFileExportTask::ProcExportMsg( CMessage *const pcMsg )
{
    u32 dwProcRet = PROCMSG_OK;
    COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    switch(pcMsg->event)
    {
    case msgTaskMsgTransfer4Db:
        {
            CTaskMsgTransfer<CFileExportRsp> *pcTransfer = *reinterpret_cast<CTaskMsgTransfer<CFileExportRsp>**>(pcMsg->content);
            CFileExportRsp &cRsp = pcTransfer->GetMsgObj();

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                PostProcessNtf(CMS_SUCCESS, 100);
                if (!m_bAbortExport)
                {
                    NextState(TRANSFER);
                }
                else
                {
                    dwProcRet = PROCMSG_DEL;
                }
            }
            else
            {
                PostProcessNtf(ERR_UAS_DBERROR, 0);
                dwProcRet = PROCMSG_DEL;
            }
        }
        break;
    case FILE_COLLECT_PROGRESS_NTF_RSP:
        // 直接返回成功即可
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

            m_bAbortExport = true;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            TASKLOG(ERROR_LEV, "对话异常终止,销毁订阅事务!\n");
            m_tInvId = INVALID_DIALOG_ID;

            m_bAbortExport = true;
        }
        break;
    default:
        break;
    }

    return dwProcRet;
}

u32 CFileExportTask::WaitExportTimerPoll()
{
    return TIMERPOLL_DONE;
}

u32 CFileExportTask::ProcByeMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == OSP_SIP_BYE_RSP)
    {
        return PROCMSG_DEL;
    }

    return PROCMSG_OK;
}

u32 CFileExportTask::WaitByeTimerPoll()
{
    if (GetCurStateHoldTime() < 60)
    {
        return TIMERPOLL_DONE;
    }

    Log(WARNING_LEV, "detected bye timeout, ObjName is %s\n", GetObjName());
    return TIMERPOLL_DEL;
}


u32 CFileExportTask::ProcTransferMsg( CMessage *const pcMsg )
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

u32 CFileExportTask::WaitTransferTimerPoll()
{
    if (GetCurStateHoldTime() > 600)
    {
        PostProcessNtf(ERR_TAS_TIME_OUT, 0);
        Log(WARNING_LEV, "detected transfer timeout, ObjName is %s\n", GetObjName());
    }

    return TIMERPOLL_DONE;
}

u32 CFileExportTask::ProcErrMsg( CMessage *const pcMsg )
{
    if (OSP_SIP_DIALOG_TERMINATE == pcMsg->event)
    {
        TASKLOG(ERROR_LEV, "对话异常终止,销毁订阅事务!\n");
        m_tInvId = INVALID_DIALOG_ID;
        return PROCMSG_DEL;
    }

    return PROCMSG_OK;
}

void CFileExportTask::PostProcessNtf( s32 nErrorCode, s32 nPercent )
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

void CFileExportTask::TryTerminate()
{
    if (GetState() != EXPORT)
    {
        delete this;
    }
    else
    {
        m_bAbortExport = true;
    }
}

void CFileExportTask::PrintData() const
{
    CSipTask::PrintData();

    OspPrintf(TRUE, FALSE, "Cuser Session:%s, bAbortExport:%d\n", m_cReq.GetSession().c_str(), (s32)m_bAbortExport);
}

void TryTerminateFileExportTask( const string &strSessionId )
{
    map<string, CFileExportTask*>::iterator it = s_mapFileExportTask.find(strSessionId);
    if (it != s_mapFileExportTask.end())
    {
        it->second->TryTerminate();
    }
}

bool IsFileExportTaskExisted( const string &strSessionId )
{
    return s_mapFileExportTask.find(strSessionId) != s_mapFileExportTask.end();
}
