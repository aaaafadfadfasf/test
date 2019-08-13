
#include "switchentrytask.h"
#include "redisswitchunittask.h"
#include "inviteswitchtask.h"
#include "cmudata.h"

CSwitchEntryTask::CSwitchEntryTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    
}

CSwitchEntryTask::~CSwitchEntryTask()
{
    
}

void CSwitchEntryTask::InitStateMachine()
{
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&CSwitchEntryTask::OnIdle;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CSwitchEntryTask::OnIdle;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CSwitchEntryTask::TimerPollSkip;
    AddRuleProc(Idle, cProc);

    NextState(Idle);
}

u32 CSwitchEntryTask::OnIdle(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    
    CInviteReq cReq;
    pcOspSipMsg->GetMsgBody(cReq);

    const TChannel &tSrcChn = cReq.GetSrcChn();
    const TChannel &tDstChn = cReq.GetDstChn();
    const TSdp     &tSdp    = cReq.GetSdp();
    const TSdpSessionName &tSessionName = tSdp.GetSessionName();

    TASKLOG(EVENT_LEV, "cmu receive switch request, Dst[%s] Src[%s] tSdp.Owner[%s] SessionName[%s] IsNatPacketMode[%d] InterfaceIp[%s]!\n", 
        tDstChn.ToString().c_str(), 
        tSrcChn.ToString().c_str(), 
        tSdp.GetOwner().ToString().c_str(),
        tSessionName.c_str(),
        tSdp.GetIsNatPacketMode(),
		tSdp.GetInterfaceIp().c_str());

    CMS_RESULT nRet = CMS_SUCCESS;

    // 判断源是否启用新的交换流程
    CRedisSdk &cRedisSdk = GetLocalRedisPipe().GetRedisSdk();
    TRedisSwitchAgent tSrc(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO());
    TRedisSwitchFlow tSwitchFlow = cRedisSdk.GetSrcSwitchFlow(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), tSrc);

    bool bRedisSwitch  = (tSwitchFlow == RedisSwitchFlow_Redis);
    bool bDirectSwitch = false;
    if (bRedisSwitch && !bDirectSwitch)
    {
        do 
        {
            // 1. 检查交换是否可路由
            CLoginSession *pcSrcStreamSession = GetLoginSession(tSrcChn.GetDevUri());
            if (NULL == pcSrcStreamSession)
            {
                TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tSrcChn.GetDevUri().c_str());

                nRet = ERR_CMU_DEV_NO_ROUTER;
                break;
            }

            CLoginSession *pcDstStreamSession = GetLoginSession(tDstChn.GetDevUri());
            if (NULL == pcDstStreamSession)
            {
                TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tDstChn.GetDevUri().c_str());

                nRet = ERR_CMU_DEV_NO_ROUTER;
                break;
            }

            TChannel tDstCmdChn;
            tDstCmdChn.SetDevUri(pcOspSipMsg->GetSipFromUri().GetURIString());
            CLoginSession *pcDstCmdSession = GetLoginSession(tDstCmdChn.GetDevUri());
            if (NULL == pcDstCmdSession)
            {
                TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tDstCmdChn.GetDevUri().c_str());

                nRet = ERR_CMU_DEV_NO_ROUTER;
                break;
            }

            // 2. 调度vtdu
            string strId, strDomainName;
            TRedisModuleResourceId   tResourceId;
            TRedisModuleVtduResource tResource;

            string strDstAddr;
            if (pcDstCmdSession->GetDevType() == DEV_TYPE_RCS)
            {
                bool bFoundDstAddr = false;
                const TMediaDescList &tMediaList = tSdp.GetMediaDescList();
                for (TMediaDescList::const_iterator it4Media = tMediaList.begin();
                    !bFoundDstAddr && tMediaList.end() != it4Media; ++it4Media)
                {
                    if (MEDIA_TYPE_VIDEO == it4Media->GetMediaType() || MEDIA_TYPE_VIDEO_LOW == it4Media->GetMediaType())
                    {
                        const TMediaTransChannelList &tTransList = it4Media->GetMediaTransChnList();
                        for (TMediaTransChannelList::const_iterator it4Trans = tTransList.begin(); 
                            !bFoundDstAddr && tTransList.end() != it4Trans; ++it4Trans)
                        {
                            if (MEDIA_TRANS_CHN_RTP == it4Trans->GetTransChnType())
                            {
                                const TNetAddrList &tAddrList = it4Trans->GetAddrList();
                                if (!tAddrList.empty())
                                {
                                    strDstAddr = tAddrList.at(0).GetNetIp();
                                }

                                bFoundDstAddr = true;
                            }                    
                        }
                    }
                }
            }

            ECMS_ERRORCODE emRet = CMS_SUCCESS;
			bool bSameIpVtdu = false;
            if (!strDstAddr.empty())
            {
                emRet = cRedisSdk.SelectVtduEx(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), 
                    TRedisSwitchAgent(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO()), 
                    TRedisSwitchAgent(TSipURI(tDstChn.GetDevUri()).GetUser(), tDstChn.GetChnNO()), 
                    strDstAddr, bSameIpVtdu, strId, strDomainName, tResourceId, &tResource);
            }
            else
            {
                emRet = cRedisSdk.SelectVtdu(CRedisPipe::SdpSessionName2RedisSwitchType(tSessionName), 
                    TRedisSwitchAgent(TSipURI(tSrcChn.GetDevUri()).GetUser(), tSrcChn.GetChnNO()), 
                    TRedisSwitchAgent(TSipURI(tDstChn.GetDevUri()).GetUser(), tDstChn.GetChnNO()), 
                    strId, strDomainName, tResourceId, &tResource);
            }
           
            if (CMS_SUCCESS != emRet)
            {
                TASKLOG(ERROR_LEV, "SelectVtdu failed, ErrCode[%d], SessionName[%s], Src[%s], Dst[%s]\n", 
                    emRet, tSessionName.c_str(), tSrcChn.ToString().c_str(), tDstChn.ToString().c_str());

                nRet = emRet;
                break;
            }

            TChannel tSrcCmdChn(TSipURI(strId, strDomainName).GetURIString(), tResource.dwPortChn);

            CLoginSession *pcSrcCmdSession = GetLoginSession(tSrcCmdChn.GetDevUri());
            if (NULL == pcSrcCmdSession)
            {
                TASKLOG(ERROR_LEV, "GetLoginSession failed, uri[%s]\n", tSrcCmdChn.GetDevUri().c_str());

                cRedisSdk.ReleaseVtduResource(tResourceId);

                nRet = ERR_CMU_DEV_NO_ROUTER;
                break;
            }

            // 3. 创建源交换
            CRedisSrcSwitchUnitTask *pcSrcSwitch = new CRedisSrcSwitchUnitTask(GetInstance(), 
                tSessionName, tSrcChn, tSrcCmdChn, pcSrcStreamSession, pcSrcCmdSession, tResourceId);
            CMS_ASSERT(NULL != pcSrcSwitch && "pcSrcSwitch = NULL!!!");
            pcSrcSwitch->InitStateMachine();
			pcSrcSwitch->SetNatPacketMode(tSdp.GetIsNatPacketMode());
            
            // 4. 创建目的交换
            CRedisCallDstSwitchUnitTask *pcDstSwitch = new CRedisCallDstSwitchUnitTask(GetInstance(), 
                tSessionName, tDstChn, tDstCmdChn, pcDstStreamSession, pcDstCmdSession, pcSrcSwitch);
            CMS_ASSERT(NULL != pcDstSwitch && "pcDstSwitch = NULL!!!");
            pcDstSwitch->InitStateMachine();

            // 5. 开始处理交换
			pcDstSwitch->GetSdp().SetDomainSocket(bSameIpVtdu);
            nRet = pcDstSwitch->Start(pcMsg);
            if (CMS_SUCCESS != nRet)
            {
                delete pcDstSwitch;
                pcDstSwitch = NULL;
            }

        } while (false);

        if (CMS_SUCCESS != nRet)                   // 错误码不是CMS_SUCCESS时由CSwitchEntryTask应答对端
        {
            CInviteRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(nRet);
            PostRsp(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
    }
    else
    {
        CTask *pTask = g_cSwitchList.CreateInviteReceiverSwitchTask(GetInstance());
        pTask->InitStateMachine();
        GetInstance()->ProcTaskMsg(pTask, pcMsg);
    }    

    return PROCMSG_DEL;
}


