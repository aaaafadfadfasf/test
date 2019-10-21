#include "p2p_tas_conn_task.h"
#include "data_sync/syncdata_send.h"
#include "common/hb_detect_multi_task.h"

CP2PTasConnTask::CP2PTasConnTask( CInstExt *pcInst, const string &strPeerDomainId, const string &strPeerDomainName, ENSyncType enSyncType )
    : CConnTaskbase(pcInst, enSyncType), m_pcSyncDataSendTask(NULL)
{
    m_tPeerUri.SetDomain(strPeerDomainName);
    m_tPeerUri = m_tPeerUri.Get3ASURI();

    m_strPeerDomainId = strPeerDomainId;
}


CP2PTasConnTask::~CP2PTasConnTask()
{
    if (NULL != m_pcSyncDataSendTask)
    {
        delete m_pcSyncDataSendTask;
        m_pcSyncDataSendTask = NULL;
    }

    StopMultiHb(m_tPeerUri, GetTaskNO(), GetCreateTime());
}


const char* CP2PTasConnTask::GetStateName( u32 dwState ) const
{
    if (enStateLogining == dwState)
    {
        return "LoginingState";
    }
    else if (enStateLoginTry == dwState)
    {
        return "LoginTryState";
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

void CP2PTasConnTask::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CP2PTasConnTask::LoginingStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CP2PTasConnTask::LoginingStateTimerPoll;
    AddRuleProc(enStateLogining, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CP2PTasConnTask::LoginTryStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CP2PTasConnTask::LoginTryStateTimerPoll;
    AddRuleProc(enStateLoginTry, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CP2PTasConnTask::LoginedStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(enStateLogined, cProc);

    EnterLoginingState();
}

u32 CP2PTasConnTask::LoginingStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event == TAS_LOGIN_RSP)
    {
        CLoginTasRsp cLoginRsp;
        reinterpret_cast<COspSipMsg* const>(pcMsg->content)->GetMsgBody(cLoginRsp);

        if (cLoginRsp.GetErrorCode() == CMS_SUCCESS)
        {
            SetMultiHb(m_tPeerUri, GetTaskNO(), GetCreateTime());
            m_strSessionId = cLoginRsp.GetSession();
            EnterLoginedState();
        }
        else
        {
            NextState(enStateLoginTry);
        }
    }

    if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
    {
        // 出错重试
        NextState(enStateLoginTry);
    }

    return PROCMSG_OK; 
}


u32 CP2PTasConnTask::LoginingStateTimerPoll()
{
    if (GetCurStateHoldTime() > 30)
    {
        EnterLoginingState();
    }

    return TIMERPOLL_DONE;
}


u32 CP2PTasConnTask::LoginTryStateProcMsg( CMessage *const pcMsg )
{
    // 什么都不用做
    return PROCMSG_OK;
}

u32 CP2PTasConnTask::LoginTryStateTimerPoll()
{
    if (GetCurStateHoldTime() > 1)
    {
        EnterLoginingState();
    }

    return TIMERPOLL_DONE;
}

u32 CP2PTasConnTask::LoginedStateProcMsg( CMessage *const pcMsg )
{
    return PROCMSG_OK;
}

void CP2PTasConnTask::EnterLoginedState()
{
    NextState(enStateLogined);

    m_pcSyncDataSendTask = new CSyncDataSend(GetInstance(), m_strPeerDomainId, 
        enSyncTypeP2PLimit, m_tPeerUri.GetDomain(), m_strSessionId);
    m_pcSyncDataSendTask->InitStateMachine();
}

void CP2PTasConnTask::EnterLoginingState()
{
    NextState(enStateLogining);
    if (NULL != m_pcSyncDataSendTask)
    {
        delete m_pcSyncDataSendTask;
        m_pcSyncDataSendTask = NULL;
    }

    TSipURI tSelfUri;
    tSelfUri.SetDomain(CurrentDomainName());
    tSelfUri = tSelfUri.Get3ASURI();

    CLoginTasReq cReq;
    cReq.SetLoginType(CLoginTasReq::enLoginType_P2PTAS);
    cReq.SetUserName(tSelfUri.GetURIString());
    cReq.SetPassword(CurrentDomainId());
    s8 szSyncType[20];
    sprintf(szSyncType, "%d", (s32)m_enSyncType);
    cReq.SetExtraInfo(string(szSyncType));

    if (PROCMSG_OK == PostMsgReq(cReq, m_tPeerUri))
    {
    }
    else
    {
        NextState(enStateLoginTry);
    }
}

u32 CP2PTasConnTask::OnDisconnect()
{
    EnterLoginingState();
    return PROCMSG_OK;
}
