
#include "3pcmsgentrytask.h"
#include "inviteswitchtask.h"
#include "cmudata.h"


C3PCMsgEntryTask::C3PCMsgEntryTask(CInstExt *pcInst) : C3PCSwitchEntryTask(pcInst), m_tTransId(INVALID_TRANS_ID)
{
    
}

C3PCMsgEntryTask::~C3PCMsgEntryTask()
{
    if (INVALID_TRANS_ID != m_tTransId)
    {
        CStartThirdPartyCallRsp cRsp;
        cRsp.SetHeadFromReq(m_cReq);
        cRsp.SetErrorCode(m_nErrorCode);
        PostRsp(KDSIP_EVENT_MESSAGE_RSP, m_tTransId, cRsp);
        m_tTransId = INVALID_TRANS_ID;
    }
}

void C3PCMsgEntryTask::InitStateMachine()
{
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCMsgEntryTask::OnIdle;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCMsgEntryTask::OnIdle;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCMsgEntryTask::TimerPollSkip;
    AddRuleProc(Idle, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&C3PCMsgEntryTask::OnWaitRsp;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCMsgEntryTask::OnWaitRsp;
    cProc.TimerPoll = (CTask::PFTimerPoll)&C3PCMsgEntryTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cProc);

    NextState(Idle);
}

u32 C3PCMsgEntryTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch(pcMsg->event)
    {
    case START_THIRD_PARTY_CALL_REQ:
        {
            pcOspSipMsg->GetMsgBody(m_cReq);

            const TChannel &tSrcChn = m_cReq.GetSrcChn();
            const TChannel &tDstChn = m_cReq.GetDstChn();
            const TSdp     &tSdp    = m_cReq.GetSdp();
            const TSdpSessionName &tSessionName = tSdp.GetSessionName();

            TASKLOG(EVENT_LEV, "cmu receive 3pc switch request, Dst[%s] Src[%s] SessionName[%s]!\n", 
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
					pcSrcSwitch->SetNatPacketMode(tSdp.GetIsNatPacketMode());

                    // 4. 创建目的交换
                    m_pcDstSwitch = new CRedis3PCMsgDstSwitchUnitTask(GetInstance(), 
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
                CTask *pTask = new C3PCMsgTask(GetInstance());
                pTask->InitStateMachine();
                GetInstance()->ProcTaskMsg(pTask, pcMsg);

                dwRet = PROCMSG_DEL;
            }
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            CStopThirdPartyCallReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            const TChannel &tSrcChn = cReq.GetSrcChn();
            const TChannel &tDstChn = cReq.GetDstChn();
            TSdpSessionName tSessionName = SDP_SESSION_AUDIOCALL;

            TASKLOG(EVENT_LEV, "cmu receive 3pc stop request, Dst[%s] Src[%s] SessionName[%s]!\n", 
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
                    TDstSwitchKey tKey(tSessionName, tDstChn);
                    map<TDstSwitchKey, CRedisDstSwitchUnitTask *>::iterator it = g_cRedisDstSwitchList.find(tKey);
                    if (g_cRedisDstSwitchList.end() != it && it->second != NULL && it->second->GetInput()->GetStreamChn() == tSrcChn)
                    {
                        delete it->second;
                    }
                    else
                    {
                        m_nErrorCode = ERR_CMU_SWITCH_DST_NODE_NOT_EXIST;
                        break;
                    }

                } while (false);

                CStopThirdPartyCallRsp cRsp;
                cRsp.SetHeadFromReq(cReq);
                PostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            }
            else
            {
                CTask *pTask = new C3PCMsgTask(GetInstance());
                pTask->InitStateMachine();
                GetInstance()->ProcTaskMsg(pTask, pcMsg);
            }

            dwRet = PROCMSG_DEL;
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

u32 C3PCMsgEntryTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    // C3PCMsgEntryTask在WaitRsp状态是由ProcSwitchFinish推动的，实际OnWaitRsp不处理消息
    switch (pcMsg->event)
    {
    default:
        {
            TASKLOG(ERROR_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 C3PCMsgEntryTask::OnWaitRspTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void C3PCMsgEntryTask::ProcSwitchFinished()
{
    delete this;
}

void C3PCMsgEntryTask::ProcSwitchExit(CRedis3PCDstSwitchUnitTask *pcDstSwitch)
{
    if (m_pcDstSwitch == pcDstSwitch)
    {
        m_nErrorCode = m_pcDstSwitch->GetErrorCode();

        m_pcDstSwitch = NULL;

        delete this;
    }
}
