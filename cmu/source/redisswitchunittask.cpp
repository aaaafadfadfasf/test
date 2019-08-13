#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_utility.h"
#include "redisswitchunittask.h"
#include "redisssmng.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "3pcmsgentrytask.h"

map<TDstSwitchKey, CRedisDstSwitchUnitTask *> g_cRedisDstSwitchList;

//============================================================================================================================

CRedisSwitchUnitTask::CRedisSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession)
    : CSipTask(pcInst), m_tSessionName(tSessionName), m_tStreamChn(tStreamChn), m_tCmdChn(tCmdChn), 
    m_pcStreamSession(pcStreamSession), m_pcCmdSession(pcCmdSession), 
    m_tDlgID(INVALID_DIALOG_ID),  m_nErrorCode(CMS_SUCCESS)
{
    CMS_ASSERT(NULL != pcStreamSession && "pcStreamSession = NULL!!!");
    CMS_ASSERT(NULL != m_pcCmdSession  && "m_pcCmdSession = NULL!!!");

    m_pcStreamSession->AddTask(this);
    m_pcCmdSession->AddTask(this);
}

CRedisSwitchUnitTask::~CRedisSwitchUnitTask()
{
    if (INVALID_DIALOG_ID != m_tDlgID)
    {
        CByeReq cReq;
        cReq.SetSession(m_pcCmdSession->GetSession());
        cReq.SetReason(m_nErrorCode);
        PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_tDlgID);
        m_tDlgID = INVALID_DIALOG_ID;
    }

    if (NULL != m_pcStreamSession)
    {
        m_pcStreamSession->DelTask(this);
    }

    if (NULL != m_pcCmdSession)
    {
        m_pcCmdSession->DelTask(this);
    }
}

bool CRedisSwitchUnitTask::ProcDisconnect(const TDevOnOffCBMsg* ptDevOnOffCBMsg)
{
    bool bProcess = false;
    /* todo by xiongxh
    LPCSTR lpszDevUri = ptDevOnOffCBMsg->GetDevUri().c_str();
    string strDevType = ptDevOnOffCBMsg->GetDevType();

    CLoginSession* pSenderSsnTask = this->GetSender().GetDevSsnTask();
    CLoginSession* pReceiverSsnTask = this->GetReceiver().GetDevSsnTask();


    if (NULL != pSenderSsnTask && pSenderSsnTask->GetSession() == lpszDevUri)
    {
        strDevType = pSenderSsnTask->GetDevType();

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Sender[%s] DevType[%s] 设备下线\n!",
            lpszDevUri, strDevType.c_str());

        //下线设备不需要发送Bye请求，这里把Dialog资源释放并重置，不再发送ByeReq.
        if (INVALID_DIALOG_ID != this->GetSender().GetDlgID())
        {
            OspSipReleaseDlg(this->GetSender().GetDlgID());
            this->GetSender().SetDlgID(INVALID_DIALOG_ID);
        }

        bProcess = true;
    }
    else if (NULL != pReceiverSsnTask && pReceiverSsnTask->GetSession() == lpszDevUri)
    {
        strDevType = pReceiverSsnTask->GetDevType();

        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Receiver[%s] DevType[%s] 设备下线\n!",
            lpszDevUri, strDevType.c_str());


        //下线设备不需要发送Bye请求，这里把Dialog资源释放并重置，不再发送ByeReq.
        if (INVALID_DIALOG_ID != this->GetReceiver().GetDlgID())
        {
            OspSipReleaseDlg(this->GetReceiver().GetDlgID());
            this->GetReceiver().SetDlgID(INVALID_DIALOG_ID);
        }

        bProcess = true;
    }
    else
    {
        TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Unknown Dev[%s]下线\n!", lpszDevUri);
    }

    if (DEV_TYPE_VTDU == strDevType)
    {
        this->SetErrorCode(ERR_CMU_SWITCH_AGAIN);
    }
    else
    {
        this->SetErrorCode(ERR_CMU_SWITCH_DEV_NOT_AVAILABLE);
    }
    */

    return bProcess;
}

bool CRedisSwitchUnitTask::ProcOspSipException(CMessage *const pcMsg)
{
    switch(pcMsg->event)
    {
        //Invite会话终止
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            m_nErrorCode = ERR_CMU_DEV_NO_ROUTER;

            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "收到SipMethod[%s]SIP层错误应答, StreamChn[%s], CmdChn[%s], SipErrorCode[%d]\n",
                pcOspSipMsg->GetSipEventStr().c_str(), 
                m_tStreamChn.ToString().c_str(), m_tCmdChn.ToString().c_str(), 
                pcOspSipMsg->GetSipErrorCode());
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //SIP层Dialog异常终止通知
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            m_nErrorCode = ERR_CMU_DIALOG_TERMINATE;
            m_tDlgID     = INVALID_DIALOG_ID;

            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "收到SIP层Dialog异常终止, "
                "StreamChn[%s], CmdChn[%s], SipFrom[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n",
                m_tStreamChn.ToString().c_str(), m_tCmdChn.ToString().c_str(), 
                pcOspSipMsg->GetSipFromUri().GetURIString().c_str(),
                pcOspSipMsg->GetSipErrorCode(), GetStateName(GetState()), GetErrorCode());
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return false;
        }
    }

    return true;
}

bool CRedisSwitchUnitTask::ProcByeReq(CMessage *const pcMsg)
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
        m_nErrorCode = ERR_CMU_SWITCH_STOP_BY_BYE;
    }

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "收到DlgID[%lu]的ByeReq, StreamChn[%s], CmdChn[%s], Reason[%d]！\n", 
        m_tDlgID, m_tStreamChn.ToString().c_str(), m_tCmdChn.ToString().c_str(), nByeReason);

    PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), CByeRsp());
    m_tDlgID = INVALID_DIALOG_ID;

    return true;
}


//============================================================================================================================

CRedisDstSwitchUnitTask::CRedisDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput)
    : CRedisSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession), m_pcInput(pcInput)
{
    CMS_ASSERT(NULL != m_pcInput && "m_pcInput = NULL!!!");

    m_pcInput->ProcDstSwitchUnitCreate(this);

    TDstSwitchKey tKey(GetSessionName(), GetStreamChn());
    map<TDstSwitchKey, CRedisDstSwitchUnitTask *>::iterator it = g_cRedisDstSwitchList.find(tKey);
    if (g_cRedisDstSwitchList.end() != it)
    {
        CRedisDstSwitchUnitTask *pcTask = it->second;
        if (NULL != pcTask)
        {
            delete pcTask;
            pcTask = NULL;
        }
    }

    g_cRedisDstSwitchList[tKey] = this;
}

CRedisDstSwitchUnitTask::~CRedisDstSwitchUnitTask()
{
    if (NULL != m_pcInput)
    {
        //删除节点间的依赖关系
        m_pcInput->ProcDstSwitchUnitExit(this);
    }

    TDstSwitchKey tKey(GetSessionName(), GetStreamChn());
    g_cRedisDstSwitchList.erase(tKey);

	//清除子task
	ClearChildTask();
}

void CRedisDstSwitchUnitTask::ClearChildTask()
{
	if (! m_cChildTasks.empty())
	{
		for (map<s32, u32>::iterator it = m_cChildTasks.begin(); it != m_cChildTasks.end(); ++it)
		{
			CTask* pcTask = ((CCmuCoreInst*)GetInstance())->FindTask(it->second);
			if (NULL != pcTask)
			{
				delete pcTask;
				pcTask = NULL;
			}
		}

		m_cChildTasks.clear();
	}
}

CRedisSwitchUnitTask *CRedisDstSwitchUnitTask::GetRelativeTask()
{
    return m_pcInput;
}

void CRedisDstSwitchUnitTask::ProcSrcSwitchUnitExit(CRedisSrcSwitchUnitTask *pcSrc)
{
    if (NULL != m_pcInput && pcSrc == m_pcInput)
    {
        m_nErrorCode = m_pcInput->GetErrorCode();

        m_pcInput = NULL;

        delete this;
    }
}

CRedisCallDstSwitchUnitTask::CRedisCallDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput)
    : CRedisDstSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession, pcInput), 
    m_tTransID(INVALID_TRANS_ID)
{
    
}

CRedisCallDstSwitchUnitTask::~CRedisCallDstSwitchUnitTask()
{
    TASKLOG(EVENT_LEV, "析构CRedisCallDstSwitchUnitTask: TaskNO[%lu], StreamChn[%s], CmdChn[%s], State[%s]\n", 
        GetTaskNO(), GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), GetStateName(GetState()));

    if (INVALID_TRANS_ID != m_tTransID)
    {
        m_cInviteRsp.SetHeadFromReq(m_cInviteReq);
        m_cInviteRsp.SetErrorCode(m_nErrorCode);
        PostRsp(KDSIP_EVENT_INVITE_RSP, m_tTransID, m_cInviteRsp);
        m_tTransID = INVALID_TRANS_ID;
    }
}

void CRedisCallDstSwitchUnitTask::InitStateMachine()
{
    // 所有状态先进入OnState，然后再根据状态分类处理，OnState提供了所有消息的统一入口点，便于处理所有状态都要处理的消息
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&CRedisCallDstSwitchUnitTask::OnState;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CRedisCallDstSwitchUnitTask::OnState;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CRedisCallDstSwitchUnitTask::OnStateTimer;

    AddRuleProc(WaitReq, cProc);
    AddRuleProc(WaitRsp, cProc);
    AddRuleProc(WaitAck, cProc);
    AddRuleProc(Service, cProc);

    NextState(WaitReq);
}

u32 CRedisCallDstSwitchUnitTask::OnState(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            ProcOspSipException(pcMsg);
            if (Service == GetState())
            {
                dwRet = PROCMSG_OK;
            }
            else
            {
                dwRet = PROCMSG_DEL;		
            }
            return dwRet;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            ProcOspSipException(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case CMU_CMU_DISCONNECT:
    case CMU_CMU_DELETE:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TDevOnOffCBMsg tDevOnOffCBMsg;
            tDevOnOffCBMsg.SetDevType(DEV_TYPE_CMU);
            tDevOnOffCBMsg.SetDevUri(pcOspSipMsg->GetMsgBody());
            ProcDisconnect(&tDevOnOffCBMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case CMU_DEV_OFFLINE_NTF:
        {
            ProcDisconnect((const TDevOnOffCBMsg*)pcMsg->content);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    default:
        {
            switch (GetState())
            {
            case WaitReq:
                {
                    dwRet = OnWaitReq(pcMsg);
                }
                break;
            case WaitRsp:
                {
                    dwRet = OnWaitRsp(pcMsg);
                }
                break;
            case WaitAck:
                {
                    dwRet = OnWaitAck(pcMsg);
                }
                break;
            case Service:
                {
                    dwRet = OnService(pcMsg);
                }
                break;
            default:
                {
                    TASKLOG(WARNING_LEV, "Receive unknown Msg[%s-%d], CurState[%s]\n", 
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, GetStateName(GetState()));

                    dwRet = PROCMSG_FAIL;
                }
                break;
            }
        }
        break;
    }

    return dwRet;
}

u32 CRedisCallDstSwitchUnitTask::OnStateTimer()
{
    u32 dwRet = TIMERPOLL_DONE;

    switch (GetState())
    {
    case WaitReq:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitRsp:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitAck:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_VTDU_WAIT_ACK_TIMEOUT;

                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case Service:
        {
            dwRet = TIMERPOLL_DONE;
        }
        break;
    default:
        {
            TASKLOG(WARNING_LEV, "Receive StateTimer, CurState[%s]\n", GetStateName(GetState()));

            dwRet = TIMERPOLL_DEL;
        }
        break;
    }

    return dwRet;
}

u32 CRedisCallDstSwitchUnitTask::OnWaitReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CRedisCallDstSwitchUnitTask初始是由Start推动的，实际OnWaitReq不处理消息
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

u32 CRedisCallDstSwitchUnitTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CRedisCallDstSwitchUnitTask在WaitRsp状态是由ForwardInviteRsp推动的，实际OnWaitRsp不处理消息
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

u32 CRedisCallDstSwitchUnitTask::OnWaitAck(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case INVITE_ACK:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            pcOspSipMsg->GetMsgBody(m_cInviteAck);

            NextState(Service);

            m_pcInput->ForwardInviteAck(this);
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

u32 CRedisCallDstSwitchUnitTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch(pcMsg->event)
    {
    case UPDATE_SWITCH_ADDRESS_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CRedisUpdateSwitchAddressTask* pcTask = new CRedisUpdateSwitchAddressTask(GetInstance(), this);
            pcTask->InitStateMachine();
            if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
        break;
    case VCR_CTRL_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CRedisVcrCtrlTask* pcTask = new CRedisVcrCtrlTask(GetInstance(), this);
            pcTask->InitStateMachine();
            if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
        break;
    case UPDATE_SWITCH_ADDRESS_RSP:
    case VCR_CTRL_RSP:
        {
            map<s32, u32>::iterator it = m_cChildTasks.find(pcOspSipMsg->GetOspSeqNO());
            if (m_cChildTasks.end() != it)
            {
                CTask *pcTask = ((CCmuCoreInst*)GetInstance())->FindTask(it->second);
                if (NULL != pcTask)
                {
                    if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
                    {
                        delete pcTask;
                        pcTask = NULL;
                    }
                }
            }
        }
        break;
    case END_OF_FILE_NTF_RSP:
    case PLAYBACK_PROGRESS_NTF_RSP:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到通知应答，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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

CMS_RESULT CRedisCallDstSwitchUnitTask::Start(CMessage *const pcMsg)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitReq == GetState() && "impossible!!!");

    do 
    {
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        m_tDlgID   = pcOspSipMsg->GetSipDlgID();
        m_tTransID = pcOspSipMsg->GetSipTransID();
        pcOspSipMsg->GetMsgBody(m_cInviteReq);
		bool bDomainSocket = m_tSdp.GetDomainSocket();
        m_tSdp = m_cInviteReq.GetSdp();
		m_tSdp.SetDomainSocket(bDomainSocket);

        // 开始处理业务前先设置为WaitRsp，说明收到了invite_req，等待源交换的invite_rsp
        NextState(WaitRsp);

        nRet = m_pcInput->ForwardInviteReq(this);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

CMS_RESULT CRedisCallDstSwitchUnitTask::ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitRsp == GetState() && "impossible!!!");

    do 
    {
        m_cInviteRsp.SetSession(GetCmdSession()->GetSession());
        m_cInviteRsp.SetHeadFromReq(m_cInviteReq);
        m_cInviteRsp.SetSdp(pcSrc->GetSdp());

        PostRsp(KDSIP_EVENT_INVITE_RSP, m_tTransID, m_cInviteRsp);
        m_tTransID = INVALID_TRANS_ID;

        NextState(WaitAck);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

CMS_RESULT CRedisCallDstSwitchUnitTask::ForwardSrcInDlgMsg(CMessage *const pcMsg)
{
    CMS_RESULT nRet = CMS_SUCCESS;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "Forward Msg[%s-%d] from src switch, StreamChn[%s], CmdChn[%s]\n", 
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, 
        GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str());

    switch(pcMsg->event)
    {
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            cReq.SetSession(GetCmdSession()->GetSession());
            PostInDlgReq(pcOspSipMsg->GetSipEventID(), cReq, m_tDlgID);
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            cReq.SetSession(GetCmdSession()->GetSession());
            PostInDlgReq(pcOspSipMsg->GetSipEventID(), cReq, m_tDlgID);
        }
        break;
    default:
        {
            nRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return nRet;
}

CRedis3PCDstSwitchUnitTask::CRedis3PCDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator)
    : CRedisDstSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession, pcInput), 
    m_pcCreator(pcCreator)
{
    CMS_ASSERT(NULL != m_pcCreator && "m_pcCreator = NULL!!!");
}

CRedis3PCDstSwitchUnitTask::~CRedis3PCDstSwitchUnitTask()
{
    if (NULL != m_pcCreator)
    {
        m_pcCreator->ProcSwitchExit(this);
    }
}

void CRedis3PCDstSwitchUnitTask::InitStateMachine()
{
    // 所有状态先进入OnState，然后再根据状态分类处理，OnState提供了所有消息的统一入口点，便于处理所有状态都要处理的消息
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&CRedis3PCDstSwitchUnitTask::OnState;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CRedis3PCDstSwitchUnitTask::OnState;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CRedis3PCDstSwitchUnitTask::OnStateTimer;

    AddRuleProc(WaitReq, cProc);
    AddRuleProc(WaitRsp, cProc);
    AddRuleProc(WaitSrc, cProc);
    AddRuleProc(Service, cProc);

    NextState(WaitReq);
}

u32 CRedis3PCDstSwitchUnitTask::OnState(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            ProcOspSipException(pcMsg);
            if (Service == GetState())
            {
                dwRet = PROCMSG_OK;
            }
            else
            {
                dwRet = PROCMSG_DEL;		
            }
            return dwRet;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            ProcOspSipException(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case CMU_CMU_DISCONNECT:
    case CMU_CMU_DELETE:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TDevOnOffCBMsg tDevOnOffCBMsg;
            tDevOnOffCBMsg.SetDevType(DEV_TYPE_CMU);
            tDevOnOffCBMsg.SetDevUri(pcOspSipMsg->GetMsgBody());

            ProcDisconnect(&tDevOnOffCBMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case CMU_DEV_OFFLINE_NTF:
        {
            ProcDisconnect((const TDevOnOffCBMsg*)pcMsg->content);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    default:
        {
            switch (GetState())
            {
            case WaitReq:
                {
                    dwRet = OnWaitReq(pcMsg);
                }
                break;
            case WaitRsp:
                {
                    dwRet = OnWaitRsp(pcMsg);
                }
                break;
            case WaitSrc:
                {
                    dwRet = OnWaitSrc(pcMsg);
                }
                break;
            case Service:
                {
                    dwRet = OnService(pcMsg);
                }
                break;
            default:
                {
                    TASKLOG(WARNING_LEV, "Receive unknown Msg[%s-%d], CurState[%s]\n", 
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, GetStateName(GetState()));

                    dwRet = PROCMSG_FAIL;
                }
                break;
            }
        }
        break;
    }

    return dwRet;
}

u32 CRedis3PCDstSwitchUnitTask::OnStateTimer()
{
    u32 dwRet = TIMERPOLL_DONE;

    switch (GetState())
    {
    case WaitReq:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitRsp:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitSrc:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_VTDU_WAIT_ACK_TIMEOUT;

                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case Service:
        {
            dwRet = TIMERPOLL_DONE;
        }
        break;
    default:
        {
            TASKLOG(WARNING_LEV, "Receive StateTimer, CurState[%s]\n", GetStateName(GetState()));

            dwRet = TIMERPOLL_DEL;
        }
        break;
    }

    return dwRet;
}

u32 CRedis3PCDstSwitchUnitTask::OnWaitReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CRedis3PCDstSwitchUnitTask初始是由Start推动的，实际OnWaitReq不处理消息
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

u32 CRedis3PCDstSwitchUnitTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case INVITE_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            m_tDlgID = pcOspSipMsg->GetSipDlgID();
            pcOspSipMsg->GetMsgBody(m_cInviteRsp);
            m_tSdp = m_cInviteRsp.GetSdp();

            // 国标解码器在上墙过程中返回的sdp中的SessionName是国标id，需要纠正
            if (GetSessionName() != m_tSdp.GetSessionName())
            {
                m_tSdp.SetSessionName(GetSessionName());
            }

            m_nErrorCode = m_cInviteRsp.GetErrorCode();

            if (CMS_SUCCESS == m_nErrorCode)
            {
                NextState(WaitSrc);

                m_pcInput->ForwardInviteReq(this);
            }
            else
            {
                dwRet = PROCMSG_DEL;
            }
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

u32 CRedis3PCDstSwitchUnitTask::OnWaitSrc(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CRedis3PCDstSwitchUnitTask在WaitSrc状态是由ForwardInviteRsp推动的，实际OnWaitSrc不处理消息
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

u32 CRedis3PCDstSwitchUnitTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch(pcMsg->event)
    {
    case UPDATE_SWITCH_ADDRESS_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CRedisUpdateSwitchAddressTask* pcTask = new CRedisUpdateSwitchAddressTask(GetInstance(), this);
            pcTask->InitStateMachine();
            if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
        break;
    case VCR_CTRL_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CRedisVcrCtrlTask* pcTask = new CRedisVcrCtrlTask(GetInstance(), this);
            pcTask->InitStateMachine();
            if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
        break;
    case UPDATE_SWITCH_ADDRESS_RSP:
    case VCR_CTRL_RSP:
        {
            map<s32, u32>::iterator it = m_cChildTasks.find(pcOspSipMsg->GetOspSeqNO());
            if (m_cChildTasks.end() != it)
            {
                CTask *pcTask = ((CCmuCoreInst*)GetInstance())->FindTask(it->second);
                if (NULL != pcTask)
                {
                    if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
                    {
                        delete pcTask;
                        pcTask = NULL;
                    }
                }
            }
        }
        break;
    case END_OF_FILE_NTF_RSP:
    case PLAYBACK_PROGRESS_NTF_RSP:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到通知应答，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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

CMS_RESULT CRedis3PCDstSwitchUnitTask::Start(CMessage *const pcMsg)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitReq == GetState() && "impossible!!!");

    do 
    {
        m_cInviteReq.SetSession(GetCmdSession()->GetSession());
        m_cInviteReq.SetSrcChn(m_pcInput->GetStreamChn());
        m_cInviteReq.SetDstChn(GetStreamChn());
        m_cInviteReq.GetSdp().SetSessionName(GetSessionName());
        m_cInviteReq.GetSdp().CreateEmptyMediaList(GetSessionName());

        PostReq(KDSIP_EVENT_INVITE_REQ, m_cInviteReq, TSipURI(GetCmdChn().GetDevUri()));

        NextState(WaitRsp);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

void CRedis3PCDstSwitchUnitTask::ProcSwitchFinished(C3PCSwitchEntryTask *pcCreator)
{
    // 通知creator交换建立成功
    if (NULL != m_pcCreator)
    {
        m_pcCreator->ProcSwitchFinished();
    }
}

CMS_RESULT CRedis3PCDstSwitchUnitTask::ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitSrc == GetState() && "impossible!!!");

    do 
    {
        m_cInviteAck.SetSession(GetCmdSession()->GetSession());
        m_cInviteAck.SetSdp(pcSrc->GetSdp());

        PostInDlgReq(KDSIP_EVENT_INVITE_ACK, m_cInviteAck, m_tDlgID);

        m_pcInput->ForwardInviteAck(this);

        NextState(Service);

        ProcSwitchFinished(m_pcCreator);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

CMS_RESULT CRedis3PCDstSwitchUnitTask::ForwardSrcInDlgMsg(CMessage *const pcMsg)
{
    CMS_RESULT nRet = CMS_SUCCESS;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "Forward Msg[%s-%d] from src switch, StreamChn[%s], CmdChn[%s]\n", 
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, 
        GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str());

    switch(pcMsg->event)
    {
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            if (NULL != m_pcCreator)
            {
                m_pcCreator->ProcMsg(pcMsg);
            }
            else
            {
                cReq.SetSession(GetCmdSession()->GetSession());
                PostInDlgReq(pcOspSipMsg->GetSipEventID(), cReq, m_tDlgID);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            if (NULL != m_pcCreator)
            {
                m_pcCreator->ProcMsg(pcMsg);
            }
            else
            {
                cReq.SetSession(GetCmdSession()->GetSession());
                PostInDlgReq(pcOspSipMsg->GetSipEventID(), cReq, m_tDlgID);
            }
        }
        break;
    default:
        {
            nRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return nRet;
}

CRedis3PCMsgDstSwitchUnitTask::CRedis3PCMsgDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator)
    : CRedis3PCDstSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession, pcInput, pcCreator)
{
    
}

CRedis3PCMsgDstSwitchUnitTask::~CRedis3PCMsgDstSwitchUnitTask()
{
    u32 dwTaskNO = (NULL == m_pcCreator) ? INVALID_TASKNO : m_pcCreator->GetTaskNO();
    TASKLOG(EVENT_LEV, "析构CRedis3PCMsgDstSwitchUnitTask: TaskNO[%lu], StreamChn[%s], CmdChn[%s], State[%s], CreatorTask[%u]\n", 
        GetTaskNO(), GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
        GetStateName(GetState()), dwTaskNO);

    if (Service == GetState())
    {
        PublishState(false);
    }
}

void CRedis3PCMsgDstSwitchUnitTask::ProcSwitchFinished(C3PCSwitchEntryTask *pcCreator)
{
    if (NULL != m_pcCreator && m_pcCreator == pcCreator)
    {
        PublishState(true);
    }

    CRedis3PCDstSwitchUnitTask::ProcSwitchFinished(pcCreator);
}

void CRedis3PCMsgDstSwitchUnitTask::ProcCreatorExit(C3PCSwitchEntryTask *pcCreator)
{
    if (NULL != m_pcCreator && m_pcCreator == pcCreator)
    {
        m_pcCreator = NULL;

        if (pcCreator->GetErrorCode() != CMS_SUCCESS)
        {
            m_nErrorCode = pcCreator->GetErrorCode();

            delete this;
        }
    }
}

void CRedis3PCMsgDstSwitchUnitTask::PublishState(bool bStart) const
{
    const TChannel &tDstChn = GetStreamChn();
    CPuData *pcPuData = GetPuDataByUri(tDstChn.GetDevUri());
    if (NULL != pcPuData)
    {
        TChannel tSrcChn;
        if (bStart && NULL != m_pcInput)
        {
            tSrcChn = m_pcInput->GetStreamChn();
        }

        //更新状态
        pcPuData->SetIsPuAudCall(tDstChn.GetChnNO(), tSrcChn);

        TPuServiceStatus tPuServiceStatus;
        tPuServiceStatus.GetAudioCallSrc().insert(map<int,TChannel>::value_type(tDstChn.GetChnNO(), tSrcChn));
        pcPuData->PublishRedisService(tPuServiceStatus);
    }
}

CRedis3PCInviteDstSwitchUnitTask::CRedis3PCInviteDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator)
    : CRedis3PCDstSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession, pcInput, pcCreator)
{

}

CRedis3PCInviteDstSwitchUnitTask::~CRedis3PCInviteDstSwitchUnitTask()
{
    u32 dwTaskNO = (NULL == m_pcCreator) ? INVALID_TASKNO : m_pcCreator->GetTaskNO();
    TASKLOG(EVENT_LEV, "析构CRedis3PCInviteDstSwitchUnitTask: TaskNO[%lu], StreamChn[%s], CmdChn[%s], State[%s], CreatorTask[%u]\n", 
        GetTaskNO(), GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
        GetStateName(GetState()), dwTaskNO);
}

void CRedis3PCInviteDstSwitchUnitTask::ProcCreatorExit(C3PCSwitchEntryTask *pcCreator)
{
    if (NULL != m_pcCreator && m_pcCreator == pcCreator)
    {
        m_pcCreator = NULL;

        m_nErrorCode = pcCreator->GetErrorCode();

        delete this;
    }
}

//============================================================================================================================

CRedisSrcSwitchUnitTask::CRedisSrcSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
    const TChannel &tStreamChn, const TChannel &tCmdChn, 
    CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, 
    const TRedisModuleResourceId &tResourceId)
    : CRedisSwitchUnitTask(pcInst, tSessionName, tStreamChn, tCmdChn, pcStreamSession, pcCmdSession), 
    m_tResourceId(tResourceId), m_pcOutput(NULL)
{
    CMS_ASSERT(!m_tResourceId.empty() && "m_tResourceId is empty!!!");
}

CRedisSrcSwitchUnitTask::~CRedisSrcSwitchUnitTask()
{
    TASKLOG(EVENT_LEV, "析构CSrcSwitchUnitTask: TaskNO[%lu], StreamChn[%s], CmdChn[%s], State[%s]\n", 
        GetTaskNO(), GetStreamChn().ToString().c_str(), 
        GetCmdChn().ToString().c_str(), GetStateName(GetState()));

    if (NULL != m_pcOutput)
    {
        m_pcOutput->ProcSrcSwitchUnitExit(this);

        m_pcOutput = NULL;
    }
}

CRedisSwitchUnitTask *CRedisSrcSwitchUnitTask::GetRelativeTask()
{
    return m_pcOutput;
}

void CRedisSrcSwitchUnitTask::InitStateMachine()
{
    // 所有状态先进入OnState，然后再根据状态分类处理，OnState提供了所有消息的统一入口点，便于处理所有状态都要处理的消息
    CStateProc cProc;
    cProc.ProcMsg = (CTask::PFProcMsg)&CRedisSrcSwitchUnitTask::OnState;
    cProc.ProcErrMsg = (CTask::PFProcMsg)&CRedisSrcSwitchUnitTask::OnState;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CRedisSrcSwitchUnitTask::OnStateTimer;

    AddRuleProc(WaitReq, cProc);
    AddRuleProc(WaitRsp, cProc);
    AddRuleProc(WaitAck, cProc);
    AddRuleProc(Service, cProc);

    NextState(WaitReq);
}

u32 CRedisSrcSwitchUnitTask::OnState(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            ProcOspSipException(pcMsg);
            if (Service == GetState())
            {
                dwRet = PROCMSG_OK;
            }
            else
            {
                dwRet = PROCMSG_DEL;		
            }
            return dwRet;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            ProcOspSipException(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    case OSP_SIP_BYE_REQ:
    case BYE_REQ:
        {
            ProcByeReq(pcMsg);

            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;
    default:
        {
            switch (GetState())
            {
            case WaitReq:
                {
                    dwRet = OnWaitReq(pcMsg);
                }
                break;
            case WaitRsp:
                {
                    dwRet = OnWaitRsp(pcMsg);
                }
                break;
            case WaitAck:
                {
                    dwRet = OnWaitAck(pcMsg);
                }
                break;
            case Service:
                {
                    dwRet = OnService(pcMsg);
                }
                break;
            default:
                {
                    TASKLOG(WARNING_LEV, "Receive unknown Msg[%s-%d], CurState[%s]\n", 
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, GetStateName(GetState()));

                    dwRet = PROCMSG_FAIL;
                }
                break;
            }
        }
        break;
    }

    return dwRet;
}

u32 CRedisSrcSwitchUnitTask::OnStateTimer()
{
    u32 dwRet = TIMERPOLL_DONE;

    switch (GetState())
    {
    case WaitReq:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitRsp:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_CMU_SWITCH_TASK_TIMEOUT;
                
                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case WaitAck:
        {
            if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
            {
                m_nErrorCode = ERR_VTDU_WAIT_ACK_TIMEOUT;

                dwRet = TIMERPOLL_DEL;
            }
        }
        break;
    case Service:
        {
            dwRet = TIMERPOLL_DONE;
        }
        break;
    default:
        {
            TASKLOG(WARNING_LEV, "Receive StateTimer, CurState[%s]\n", GetStateName(GetState()));

            dwRet = TIMERPOLL_DEL;
        }
        break;
    }

    return dwRet;
}

u32 CRedisSrcSwitchUnitTask::OnWaitReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CSrcSwitchUnitTask初始是由ForwardInviteReq推动的，实际OnWaitReq不处理消息
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

u32 CRedisSrcSwitchUnitTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case INVITE_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            m_tDlgID   = pcOspSipMsg->GetSipDlgID();
            pcOspSipMsg->GetMsgBody(m_cInviteRsp);
            m_tSdp = m_cInviteRsp.GetSdp();

            m_nErrorCode = m_cInviteRsp.GetErrorCode();

            if (CMS_SUCCESS == m_nErrorCode)
            {
                NextState(WaitAck);

                m_pcOutput->ForwardInviteRsp(this);
            }
            else
            {
                dwRet = PROCMSG_DEL;
            }
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

u32 CRedisSrcSwitchUnitTask::OnWaitAck(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    // CSrcSwitchUnitTask在WaitAck是由ForwardInviteAck推动的，实际OnWaitAck不处理消息
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

u32 CRedisSrcSwitchUnitTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    switch(pcMsg->event)
    {
    case END_OF_FILE_NTF_REQ:
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到源交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(),
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            if (NULL != m_pcOutput)
            {
                m_pcOutput->ForwardSrcInDlgMsg(pcMsg);
            }
        }
        break;
    case UPDATE_SWITCH_ADDRESS_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CUpdateSwitchAddressReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            cReq.SetSession(GetCmdSession()->GetSession());
            PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_tDlgID);
        }
        break;
    case VCR_CTRL_REQ:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到目的交换的会话内请求，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(), 
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            CVcrCtrlReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            cReq.SetSession(GetCmdSession()->GetSession());
            PostInDlgReq(pcOspSipMsg->GetSipReq(), cReq, m_tDlgID);
        }
        break;
    case UPDATE_SWITCH_ADDRESS_RSP:
    case VCR_CTRL_RSP:
        {
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "收到源交换的会话内应答，StreamChn[%s], CmdChn[%s], DlgId[%u], Msg[%s-%d]\n", 
                GetStreamChn().ToString().c_str(), GetCmdChn().ToString().c_str(),
                m_tDlgID, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            if (NULL != m_pcOutput)
            {
                m_pcOutput->ProcMsg(pcMsg);
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

CMS_RESULT CRedisSrcSwitchUnitTask::ForwardInviteReq(CRedisDstSwitchUnitTask *pcDst)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitReq == GetState() && "impossible!!!");

    do 
    {
        m_cInviteReq.SetSession(GetCmdSession()->GetSession());
        m_cInviteReq.SetSrcChn(GetStreamChn());
        m_cInviteReq.SetDstChn(m_pcOutput->GetStreamChn());
        
        TSdp tSdp(pcDst->GetSdp());
        tSdp.SetOwner(m_pcOutput->GetStreamChn());
        tSdp.SetResourceId(m_tResourceId);
        tSdp.SetInterfaceIp(pcDst->GetStreamSession()->GetJoinInterfaceIp(TSipURI(GetStreamChn().GetDevUri()).GetUser()));
		tSdp.SetIsNatPacketMode(m_bNatPacketMode);

        m_cInviteReq.SetSdp(tSdp);

        PostReq(KDSIP_EVENT_INVITE_REQ, m_cInviteReq, TSipURI(GetCmdChn().GetDevUri()));

        NextState(WaitRsp);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

CMS_RESULT CRedisSrcSwitchUnitTask::ForwardInviteAck(CRedisDstSwitchUnitTask *pcDst)
{
    CMS_RESULT nRet = CMS_SUCCESS;
    CMS_ASSERT(WaitAck == GetState() && "impossible!!!");

    do 
    {
        m_cInviteReq.SetSession(GetCmdSession()->GetSession());
        PostInDlgReq(KDSIP_EVENT_INVITE_ACK, m_cInviteAck, m_tDlgID);

        NextState(Service);

    } while (false);

    if (CMS_SUCCESS != nRet)
    {
        m_nErrorCode = nRet;
    }

    return nRet;
}

CMS_RESULT CRedisSrcSwitchUnitTask::ForwardDstInDlgMsg(CMessage *const pcMsg)
{
    CMS_RESULT nRet = CMS_SUCCESS;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

    TASKLOG(CMU_MOD_SWITCH, PROGRESS_LEV, "Forward Msg[%s-%d] from dst switch\n", 
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch(pcMsg->event)
    {
    default:
        {
            nRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return nRet;
}

void CRedisSrcSwitchUnitTask::ProcDstSwitchUnitCreate(CRedisDstSwitchUnitTask *pcDst)
{
    CMS_ASSERT(NULL == m_pcOutput && "m_pcOutput != NULL!!!");

    m_pcOutput = pcDst;

    CMS_ASSERT(NULL != m_pcOutput && "m_pcOutput = NULL!!!");
}

void CRedisSrcSwitchUnitTask::ProcDstSwitchUnitExit(CRedisDstSwitchUnitTask *pcDst)
{
    if (NULL != m_pcOutput && pcDst == m_pcOutput)
    {
        m_nErrorCode = m_pcOutput->GetErrorCode();

        m_pcOutput = NULL;

        delete this;
    }
}

