#include "p2p_tas_ssn_task.h"
#include "cctrl.h"
#include "data_sync/syncdata_recv.h"
#include "common/hb_detect_multi_task.h"
// hzytodo 参照child_tas_ssn_task调整uri内容

CP2PTasSsnTask::CP2PTasSsnTask( CInstExt *pcInst )
    : CSsnTaskbase(pcInst), m_pcSyncDataRecvTask(NULL)
{
    AppData().TasSessTasks().insert(this);
}

CP2PTasSsnTask::~CP2PTasSsnTask()
{
    if (NULL != m_pcSyncDataRecvTask)
    {
        delete m_pcSyncDataRecvTask;
        m_pcSyncDataRecvTask = NULL;
    }

    StopMultiHb(GetClientSipURI(), GetTaskNO(), GetCreateTime());

    AppData().TasSessTasks().erase(this);
}

const char* CP2PTasSsnTask::GetStateName( u32 dwState ) const
{
    if (enStateInit == dwState)
    {
        return "InitState";
    }
    else if (enStateServer == dwState)
    {
        return "ServerState";
    }
    else
    {
        return "unknown";
    }
}

void CP2PTasSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CP2PTasSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CP2PTasSsnTask::ServerStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateServer, cProc);
}

u32 CP2PTasSsnTask::InitStateProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CLoginTasReq cLoginReq;
    pcSipMsg->GetMsgBody(cLoginReq);

    string strModuleUri;
    strModuleUri = pcSipMsg->GetSipFromUri().GetURIString();

    if (AppData().IsClientModuleUriExist(strModuleUri))
    {
        CLoginTasRsp cLoginRsp;
        cLoginRsp.SetErrorCode(ERR_TAS_LOGINED);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

        return PROCMSG_DEL;
    }

    CLoginTasRsp cLoginRsp;
    cLoginRsp.SetErrorCode(CMS_SUCCESS);
    cLoginRsp.SetSession(GetSessionId());
    PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

    SetClientSipURI(pcSipMsg->GetSipFromUri());
    if (!SetMultiHb(GetClientSipURI(), GetTaskNO(), GetCreateTime()))
    {
        return PROCMSG_DEL;
    }

    NextState(enStateServer);
    m_pcSyncDataRecvTask = new CSyncDataRecv(GetInstance(), pcSipMsg->GetSipFromUri(), 
        static_cast<ENSyncType>(atoi(cLoginReq.GetExtraInfo().c_str())));
    m_pcSyncDataRecvTask->InitStateMachine();

    return PROCMSG_OK;
}

u32 CP2PTasSsnTask::ServerStateProcMsg( CMessage *const pcMsg )
{
    if (TAS_SYNC_TAS_DATA_REQ == pcMsg->event)
    {
        m_pcSyncDataRecvTask->ProcMsg(pcMsg);
    }

    if (!((OSP_SIP_DISCONNECT == pcMsg->event) || (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)))
    {
        Log(WARNING_LEV, "CChildTasSsnTask::LoginedStateProcMsg recved unknown message, eventId=%u\n", pcMsg->event);
        return PROCMSG_FAIL;
    }

    return DefaultProc(pcMsg);
}

