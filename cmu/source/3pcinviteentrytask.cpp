
#include "3pcinviteentrytask.h"
#include "inviteswitchtask.h"
#include "cmudata.h"


C3PCInviteEntryTask::C3PCInviteEntryTask(CInstExt *pcInst)
    : C3PCSwitchEntryTask(pcInst), m_tTransId(INVALID_TRANS_ID), m_tDlgId(INVALID_DIALOG_ID)
{
    
}

C3PCInviteEntryTask::~C3PCInviteEntryTask()
{
    CLoginSession *pcLoginSession = GetLoginSession(m_cReq.GetSession());
    if (NULL != pcLoginSession)
    {
        pcLoginSession->DelTask(this);
    }

    if (INVALID_TRANS_ID != m_tTransId)
    {
        CInviteThirdPartyCallRsp cRsp;
        cRsp.SetHeadFromReq(m_cReq);
        cRsp.SetErrorCode(m_nErrorCode);
        PostRsp(KDSIP_EVENT_INVITE_RSP, m_tTransId, cRsp);
        m_tTransId = INVALID_TRANS_ID;
    }

    if (INVALID_DIALOG_ID != m_tDlgId)
    {
        CByeReq cReq;
        cReq.SetSession(m_cReq.GetSession());
        cReq.SetReason(m_nErrorCode);
        PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_tDlgId);
        m_tDlgId = INVALID_DIALOG_ID;
    }
}

void C3PCInviteEntryTask::InitStateMachine()
{
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnIdle;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnIdle;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteEntryTask::TimerPollSkip;
    AddRuleProc(Idle, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnWaitRsp;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnWaitRsp;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteEntryTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnWaitAck;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnWaitAck;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteEntryTask::OnWaitAckTimer;
    AddRuleProc(WaitAck, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnService;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteEntryTask::OnService;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteEntryTask::TimerPollSkip;
    AddRuleProc(Service, cProc);

    NextState(Idle);
}

u32 C3PCInviteEntryTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch(pcMsg->event)
    {
    case INVITE_THIRD_PARTY_CALL_REQ:
        {
            pcOspSipMsg->GetMsgBody(m_cReq);

            const TChannel &tSrcChn = m_cReq.GetSrcChn();
            const TChannel &tDstChn = m_cReq.GetDstChn();
            const TSdp     &tSdp    = m_cReq.GetSdp();
            const TSdpSessionName &tSessionName = tSdp.GetSessionName();

            TASKLOG(EVENT_LEV, "cmu receive 3pc invite switch request, Dst[%s] Src[%s] SessionName[%s]!\n", 
                tDstChn.ToString().c_str(), 
                tSrcChn.ToString().c_str(), 
                tSessionName.c_str());

            // 判断源是否启用新的交换流程
            CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
            TRedisSwitchAgent tSrc(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO());
            TRedisSwitchFlow tSwitchFlow = cRedisSdk.GetSrcSwitchFlow(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), tSrc);
            bool bIsCallLocalDomainDev   = IsLocalDomainByDevUri( tDstChn.GetDevUri() ); 
            bool bRedisSwitch  = (tSwitchFlow == RedisSwitchFlow_Redis);
            bool bDirectSwitch = false;
            if (bRedisSwitch && !bDirectSwitch 
                && bIsCallLocalDomainDev )
            {
                do 
                {
                    m_tTransId = pcOspSipMsg->GetSipTransID();
                    m_tDlgId   = pcOspSipMsg->GetSipDlgID();

                    // 0. 检查发起第三方交换的模块是否注册 
                    CLoginSession *pcLoginSession = GetLoginSession(m_cReq.GetSession());
                    if (NULL == pcLoginSession)
                    {
                        TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", m_cReq.GetSession().c_str());

                        m_nErrorCode = ERR_CMU_SWITCH_DEV_NOT_AVAILABLE;
                        break;
                    }

                    pcLoginSession->AddTask(this);

                    // 1. 检查交换是否可路由
                    CLoginSession *pcSrcStreamSession = GetLoginSession(tSrcChn.GetDevUri());
                    if (NULL == pcSrcStreamSession)
                    {
                        TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tSrcChn.GetDevUri().c_str());

                        m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;
                        break;
                    }

                    CLoginSession *pcDstStreamSession = GetLoginSession(tDstChn.GetDevUri());
                    if (NULL == pcDstStreamSession)
                    {
                        TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tDstChn.GetDevUri().c_str());

                        m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;
                        break;
                    }

                    bool bFollowTopo = g_cCmuConf.m_bSwitchFollowTopo;

                    TChannel tDstCmdChn;
                    tDstCmdChn.SetDevUri(GetDevRouter(tDstChn.GetDevUri(), bFollowTopo));

                    CLoginSession *pcDstCmdSession = NULL;

                    TSipURI tDevUri(tDstChn.GetDevUri());
                    CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(tDevUri.GetUser());
                    if (NULL != pcPuData)
                    {
                        CPuRegBaseTask *pcTask = pcPuData->GetPuRegTask();
                        if (NULL != pcTask)
                        {
                            pcDstCmdSession = pcTask->GetLoginSession(tDevUri.GetUser());
                        }
                    }
                    else
                    {
                        pcDstCmdSession = GetLoginSession(tDstCmdChn.GetDevUri());
                    }

                    if (NULL == pcDstCmdSession)
                    {
                        TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tDstCmdChn.GetDevUri().c_str());

                        m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;
                        break;
                    }
                    
                    TDstSwitchKey tKey(tSessionName, tDstChn);
                    map<TDstSwitchKey, CRedisDstSwitchUnitTask *>::iterator it = g_cRedisDstSwitchList.find(tKey);
                    if (g_cRedisDstSwitchList.end() != it && it->second != NULL && it->second->GetInput()->GetStreamChn() != tSrcChn)
                    {
                        m_nErrorCode = ERR_CMU_SWITCH_DST_EXIST_SWITCH;
                        break;
                    }

                    // 2. 调度vtdu
                    string strId, strDomainName;
                    TRedisModuleResourceId   tResourceId;
                    TRedisModuleVtduResource tResource;
                    ECMS_ERRORCODE emRet = cRedisSdk.SelectVtdu(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), 
                        TRedisSwitchAgent(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO()), 
                        TRedisSwitchAgent(TSipURI(tDstChn.GetDevUri()).GetUser(), tDstChn.GetChnNO()), 
                        strId, strDomainName, tResourceId, &tResource);
                    if (CMS_SUCCESS != emRet)
                    {
                        TASKLOG(ERROR_LEV, "SelectVtdu failed, ErrCode[%d], SessionName[%s], Src[%s], Dst[%s]\n", 
                            emRet, tSessionName.c_str(), tSrcChn.ToString().c_str(), tDstChn.ToString().c_str());

                        m_nErrorCode = emRet;
                        break;
                    }

                    TChannel tSrcCmdChn(TSipURI(strId, strDomainName).GetURIString(), tResource.dwPortChn);

                    CLoginSession *pcSrcCmdSession = GetLoginSession(tSrcCmdChn.GetDevUri());
                    if (NULL == pcSrcCmdSession)
                    {
                        TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tSrcCmdChn.GetDevUri().c_str());

                        cRedisSdk.ReleaseVtduResource(tResourceId);

                        m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;
                        break;
                    }

                    // 3. 创建源交换
                    CRedisSrcSwitchUnitTask *pcSrcSwitch = new CRedisSrcSwitchUnitTask(GetInstance(), 
                        tSessionName, tSrcChn, tSrcCmdChn, pcSrcStreamSession, pcSrcCmdSession, tResourceId);
                    CMS_ASSERT(NULL != pcSrcSwitch && "pcSrcSwitch = NULL!!!");
                    pcSrcSwitch->InitStateMachine();

                    // 4. 创建目的交换
                    m_pcDstSwitch = new CRedis3PCInviteDstSwitchUnitTask(GetInstance(), 
                        tSessionName, tDstChn, tDstCmdChn, pcDstStreamSession, pcDstCmdSession, pcSrcSwitch, this);
                    CMS_ASSERT(NULL != m_pcDstSwitch && "m_pcDstSwitchTask = NULL!!!");
                    m_pcDstSwitch->InitStateMachine();

                    // 5. 开始处理交换
                    m_nErrorCode = m_pcDstSwitch->Start(pcMsg);

                } while (false);

                if (CMS_SUCCESS == m_nErrorCode)
                {
                    NextState(WaitRsp);
                }
                else
                {
                    dwRet = PROCMSG_DEL;
                }
            }
            else
            {
                CTask *pTask = new C3PCInviteTask(GetInstance());
                pTask->InitStateMachine();
                GetInstance()->ProcTaskMsg(pTask, pcMsg);

                dwRet = PROCMSG_DEL;
            }
        }
        break;
    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteEntryTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    // C3PCInviteEntryTask在WaitRsp状态是由ProcSwitchFinish推动的，实际OnWaitRsp不处理消息
    switch (pcMsg->event)
    {
    case OSP_SIP_DIALOG_TERMINATE:
        {
            m_nErrorCode = ERR_CMU_DIALOG_TERMINATE;

            TASKLOG(ERROR_LEV, "收到SIP层Dialog异常终止, SipFromUri[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode(),
                GetStateName(this->GetState()), m_nErrorCode);

            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(ERROR_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteEntryTask::OnWaitRspTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 C3PCInviteEntryTask::OnWaitAck(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch (pcMsg->event)
    {
    case INVITE_THIRD_PARTY_CALL_ACK:
        {
            TASKLOG(ERROR_LEV, "收到3pc invite Ack，TaskNO[%u]\n", GetTaskNO());

            NextState(Service);
        }
        break;
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            m_nErrorCode = ERR_CMU_DIALOG_TERMINATE;

            TASKLOG(ERROR_LEV, "收到SIP层Dialog异常终止, SipFromUri[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode(),
                GetStateName(this->GetState()), m_nErrorCode);

            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;

            TASKLOG(ERROR_LEV, "SIP层消息发送失败, SipFromUri[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode(),
                GetStateName(this->GetState()), m_nErrorCode);

            dwRet = PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(ERROR_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteEntryTask::OnWaitAckTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 C3PCInviteEntryTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch (pcMsg->event)
    {
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            m_nErrorCode = ERR_CMU_DIALOG_TERMINATE;

            TASKLOG(ERROR_LEV, "收到SIP层Dialog异常终止, SipFromUri[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n", 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode(),
                GetStateName(this->GetState()), m_nErrorCode);

            dwRet = PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(ERROR_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

void C3PCInviteEntryTask::ProcSwitchFinished()
{
    CMS_ASSERT(WaitRsp == GetState() && "impossible!!!");
    CMS_ASSERT(CMS_SUCCESS == m_nErrorCode && "impossible!!!");

    CInviteThirdPartyCallRsp cRsp;
    cRsp.SetHeadFromReq(m_cReq);
    cRsp.SetErrorCode(m_nErrorCode);
    PostRsp(KDSIP_EVENT_INVITE_RSP, m_tTransId, cRsp);
    m_tTransId = INVALID_TRANS_ID;

    TASKLOG(EVENT_LEV, "收到3pc invite交换建立完成通知，TaskNO[%u]\n", GetTaskNO());

    NextState(WaitAck);
}

void C3PCInviteEntryTask::ProcSwitchExit(CRedis3PCDstSwitchUnitTask *pcDstSwitch)
{
    TASKLOG(EVENT_LEV, "收到3pc invite交换拆除通知，TaskNO[%u]\n", GetTaskNO());

    m_nErrorCode = pcDstSwitch->GetErrorCode();

    m_pcDstSwitch = NULL;

    delete this;
}

void C3PCInviteEntryTask::ProcByeReq(CMessage *const pcMsg)
{
    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    s32 nByeReason = CMS_SUCCESS;
    if (pcOspSipMsg->GetMsgBodyLen() != 0)
    {
        CByeReq cByeReq;
        pcOspSipMsg->GetMsgBody(cByeReq);

        nByeReason = cByeReq.GetReason();
    }

    if (CMS_SUCCESS != nByeReason)
    {
        m_nErrorCode = nByeReason;
    }
    else
    {
        m_nErrorCode = ERR_CMU_STOP_BY_BYE;
    }
    
    TASKLOG(ERROR_LEV, "收到设备[%s]DlgID[%lu]的ByeReq, Reason[%ld]！\n", 
        pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), m_tDlgId, nByeReason);

    CByeRsp cRsp;
    PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
    m_tDlgId = INVALID_DIALOG_ID;
}
