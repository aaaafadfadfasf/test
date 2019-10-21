#include "parent_tas_conn_task.h"
#include "common/hb_detect_multi_task.h"

map<string, TParentSyncRspLoginExternInfo> g_cParentSyncExtInfoList;

CParentTasConnTask::CParentTasConnTask( CInstExt *pcInst, const string &strPeerDomainId, const string &strPeerDomainName )
    : CConnTaskbase(pcInst, enSyncTypeParent), m_pcSyncDataSendTask(NULL)
{
    m_tPeerUri.SetDomain(strPeerDomainName);
    m_tPeerUri = m_tPeerUri.Get3ASURI();

    m_strPeerDomainId = strPeerDomainId;
}

CParentTasConnTask::~CParentTasConnTask()
{
    if (NULL != m_pcSyncDataSendTask)
    {
        delete m_pcSyncDataSendTask;
        m_pcSyncDataSendTask = NULL;
    }

    StopMultiHb(m_tPeerUri, GetTaskNO(), GetCreateTime());
}

const char* CParentTasConnTask::GetStateName( u32 dwState ) const
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

void CParentTasConnTask::InitStateMachine()
{
    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CParentTasConnTask::LoginingStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CParentTasConnTask::LoginingStateTimerPoll;
    AddRuleProc(enStateLogining, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CParentTasConnTask::LoginTryStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CParentTasConnTask::LoginTryStateTimerPoll;
    AddRuleProc(enStateLoginTry, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CParentTasConnTask::LoginedStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(enStateLogined, cProc);

    EnterLoginingState();
}

u32 CParentTasConnTask::LoginingStateProcMsg( CMessage *const pcMsg )
{
    if (TAS_LOGIN_RSP == pcMsg->event)
    {
        CLoginTasRsp cLoginRsp;
        reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody(cLoginRsp);

        if (cLoginRsp.GetErrorCode() == CMS_SUCCESS)
        {
            SetMultiHb(m_tPeerUri, GetTaskNO(), GetCreateTime());
            m_strSessionId = cLoginRsp.GetSession();

			TParentSyncRspLoginExternInfo &tExtInfo = g_cParentSyncExtInfoList[m_strPeerDomainId];
			if (!cLoginRsp.GetExtraInfo().empty())
			{
				tExtInfo.FromXml(cLoginRsp.GetExtraInfo());
			}
			else
			{
				// 如果上级平台没有返回其版本信息，则认为其是v2r2b1或者更早的版本
				tExtInfo.platVersion = string(PLAT_VERSION_V2R2B1);
			}
			
            EnterLoginedState();
        }
        else
        {
            NextState(enStateLoginTry);
        }
    }

    // 怀疑底层多发此消息，故先注释掉
    // if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
    //{
    //    Log(CRITICAL_LEV, "CParentTasConnTask::LoginingStateProcMsg OSP_SIP_MSG_PROC_FAIL's sub error code:%u\n", 
    //        (u32)(reinterpret_cast<const TOspSipMsgHead*>(pcMsg->content)->m_eSipErrorCode));
    //    // 出错重试
    //    NextState(enStateLoginTry);
    //}

    return PROCMSG_OK; 
}


u32 CParentTasConnTask::LoginingStateTimerPoll()
{
    if (GetCurStateHoldTime() > 30)
    {
        EnterLoginingState();
    }

    return TIMERPOLL_DONE;
}


u32 CParentTasConnTask::LoginTryStateProcMsg( CMessage *const pcMsg )
{
    // 什么都不用做
    return PROCMSG_OK;
}


u32 CParentTasConnTask::LoginTryStateTimerPoll()
{
    if (GetCurStateHoldTime() > 5)
    {
        EnterLoginingState();
    }

    return TIMERPOLL_DONE;
}


u32 CParentTasConnTask::LoginedStateProcMsg( CMessage *const pcMsg )
{
    return PROCMSG_OK;
}

void CParentTasConnTask::EnterLoginingState()
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
    cReq.SetLoginType(CLoginTasReq::enLoginType_SubTAS);
    cReq.SetUserName(tSelfUri.GetURIString());
    cReq.SetPassword(CurrentDomainId());

	TParentSyncLoginExternInfo tExtInfo;
	tExtInfo.platVersion = GetCommonCmsVer();
	cReq.SetExtraInfo(tExtInfo.ToXml());

    Log(EVENT_LEV, "CLoginTasReq content:%s in CParentTasConnTask\n", cReq.ToXml().c_str());

    if (PROCMSG_OK == PostMsgReq(cReq, m_tPeerUri))
    {
    }
    else
    {
        NextState(enStateLoginTry);
    }
}

void CParentTasConnTask::EnterLoginedState()
{
    NextState(enStateLogined);

    m_pcSyncDataSendTask = new CSyncDataSend(GetInstance(), m_strPeerDomainId, enSyncTypeParent
        , m_tPeerUri.GetDomain(), m_strSessionId);
    m_pcSyncDataSendTask->InitStateMachine();
}

u32 CParentTasConnTask::OnDisconnect()
{
    EnterLoginingState();
    return PROCMSG_OK;
}

void CParentTasConnTask::PrintData() const
{
    TelePrint("PeerUri:%s, PeerDomainId:%s", 
        m_tPeerUri.GetURIString().c_str(), m_strPeerDomainId.c_str());
}

