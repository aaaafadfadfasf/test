#include "cmu_ssn_task.h"
#include "cctrl.h"
#include "license_mng.h"

CCmuSsnTask::CCmuSsnTask( CInstExt *pcInst ): CSsnTaskbase(pcInst)
{
    AppData().GetChangeNtfSubscribeTasks().insert(this);

    m_cNtfFilterTypes.clear();

    //m_cNtfFilterTypes.insert(TasResourceGeneralDev);
    //m_cNtfFilterTypes.insert(TasResourceDevCapIndexName);
    //m_cNtfFilterTypes.insert(TasResourceDomainRelation);
}

CCmuSsnTask::~CCmuSsnTask()
{
    AppData().GetChangeNtfSubscribeTasks().erase(this);
}

const char* CCmuSsnTask::GetStateName( u32 dwState ) const
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

void CCmuSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CCmuSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CCmuSsnTask::ServerStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSsnTaskbase::PollChangeNotifyLst);
    AddRuleProc(enStateServer, cProc);
}

u32 CCmuSsnTask::InitStateProcMsg( CMessage *const pcMsg )
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
    if (!SetHb())
    {
        return PROCMSG_DEL;
    }

    NextState(enStateServer);
    return PROCMSG_OK;
}

u32 CCmuSsnTask::ServerStateProcMsg( CMessage *const pcMsg )
{
    if (g_enLicenseValidateResult != CMS_SUCCESS)
    {
        Log(ERROR_LEV, "CuiSsnTask is killed, because of the license validate failed, errno=%d\n", (s32)g_enLicenseValidateResult);
        return PROCMSG_DEL;
    }

    return DefaultProc(pcMsg);
}
