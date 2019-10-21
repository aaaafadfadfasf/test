#include "child_tas_ssn_task.h"
#include "cctrl.h"
#include "data_sync/syncdata_recv.h"
#include "common/hb_detect_multi_task.h"
#include "commonconfig/cmcfg.h"

CChildTasSsnTask::CChildTasSsnTask( CInstExt *pcInst )
    : CSsnTaskbase(pcInst), m_pcSyncDataRecvTask(NULL)
{
    AppData().TasSessTasks().insert(this);
}

CChildTasSsnTask::~CChildTasSsnTask()
{
    if (NULL != m_pcSyncDataRecvTask)
    {
        delete m_pcSyncDataRecvTask;
        m_pcSyncDataRecvTask = NULL;
    }

    StopMultiHb(m_tPeerTasUri, GetTaskNO(), GetCreateTime());

    AppData().TasSessTasks().erase(this);
}

const char* CChildTasSsnTask::GetStateName( u32 dwState ) const
{
    if (enStateInit == dwState)
    {
        return "InitState";
    }
    else if (enStateLogining == dwState)
    {
        return "LoginingState";
    }
    else if (enStateLogined == dwState)
    {
        return "LoginedState";
    }
    else
    {
        return "unknown";
    }
}

void CChildTasSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CChildTasSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CChildTasSsnTask::LoginingStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateLogining, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CChildTasSsnTask::LoginedStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSsnTaskbase::PollChangeNotifyLst);
    AddRuleProc(enStateLogined, cProc);
}

u32 CChildTasSsnTask::InitStateProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CTaskMsgTransfer<CLoginTasReq> cTransfer(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
    pcSipMsg->GetMsgBody(cTransfer.GetMsgObj());

    CLoginTasReq &cLoginReq = cTransfer.GetMsgObj();

    TSipURI tModuleUri = pcSipMsg->GetSipFromUri();
    s8 szBuf[10];
    sprintf(szBuf, "_%u", enSyncTypeParent);
    tModuleUri.SetUser(tModuleUri.GetUser()+szBuf);

    CLoginTasRsp cLoginRsp;
    if (AppData().IsClientModuleUriExist(tModuleUri.GetURIString()))
    {
        cLoginRsp.SetErrorCode(ERR_TAS_LOGINED);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

        return PROCMSG_DEL;
    }  

    if (PostTaskMsgTransfer2DbTaskPool(cTransfer))
    {
        SetClientSipURI(tModuleUri);
        NextState(enStateLogining);

        m_tPeerTasUri = pcSipMsg->GetSipFromUri();
        return PROCMSG_OK;
    }
    else
    {
        cLoginRsp.SetErrorCode(ERR_TAS_SYSTEM_BUSY);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());
        return PROCMSG_DEL;
    }
}

u32 CChildTasSsnTask::LoginingStateProcMsg( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db == pcMsg->event)
    {
        CTaskMsgTransfer<CLoginTasRsp> *pcTransfer = *reinterpret_cast<CTaskMsgTransfer<CLoginTasRsp>**>(pcMsg->content);

        CLoginTasRsp &cLoginTasRsp = pcTransfer->GetMsgObj();

        if (CMS_SUCCESS == cLoginTasRsp.GetErrorCode())
        {
            cLoginTasRsp.SetSession(GetSessionId());

			TParentSyncRspLoginExternInfo tExtInfo;
			tExtInfo.platVersion = GetCommonCmsVer();
			cLoginTasRsp.SetExtraInfo(tExtInfo.ToXml());

            PostMsgRsp(pcTransfer->GetSipTransId(), cLoginTasRsp);
            if (!SetMultiHb(m_tPeerTasUri, GetTaskNO(), GetCreateTime()))
            {
                return PROCMSG_DEL;
            }
            
            EnterLoginedState();
            return PROCMSG_OK;
        }
        else
        {
            return PROCMSG_DEL;
        }
    }
    else
    {
        Log(WARNING_LEV, "CChildTasSsnTask::LoginingStateProcMsg recved unknown message\n");
        return PROCMSG_FAIL;
    }
}

u32 CChildTasSsnTask::LoginedStateProcMsg( CMessage *const pcMsg )
{
    if (TAS_SYNC_TAS_DATA_REQ == pcMsg->event)
    {
        return m_pcSyncDataRecvTask->ProcMsg(pcMsg);
    }

    if (!((OSP_SIP_DISCONNECT == pcMsg->event) || (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)))
    {
        Log(WARNING_LEV, "CChildTasSsnTask::LoginedStateProcMsg recved unknown message, eventId=%u\n", pcMsg->event);
        return PROCMSG_FAIL;
    }

    return DefaultProc(pcMsg);
}

void CChildTasSsnTask::EnterLoginedState()
{
    NextState(enStateLogined);
    
    m_pcSyncDataRecvTask = new CSyncDataRecv(GetInstance(), m_tPeerTasUri, enSyncTypeParent);
    m_pcSyncDataRecvTask->InitStateMachine();
}

void CChildTasSsnTask::EnterInitState()
{
    if (NULL != m_pcSyncDataRecvTask)
    {
        delete m_pcSyncDataRecvTask;
        m_pcSyncDataRecvTask = NULL;
    }

    NextState(enStateInit);
}
