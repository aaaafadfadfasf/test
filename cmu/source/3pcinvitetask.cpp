/*****************************************************************************
模块名      : cmu 
文件名      : 3pcinvitetask.cpp
相关文件    : 
文件实现功能: CSwitchCreator实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
******************************************************************************/

#include "cmudata.h"
#include "3pcinvitetask.h"
#include "3pcmsgtask.h"
#include "inviteswitchtask.h"
#include "cms/pu/vsipdefine.h"
#include "regtastask.h"

extern CRegTasTask* g_pcRegTasTask;

///////////////////////////////////////////////////////////////////////////////////////
//C3PCInviteTask
///////////////////////////////////////////////////////////////////////////////////////
C3PCInviteTask::C3PCInviteTask(CInstExt *pcInst): C3PCTask(pcInst)
{
    m_pDevSsnTask = NULL;
	m_bCreateDecoderAudioCall = false;
}

C3PCInviteTask::~C3PCInviteTask()
{
    ReleaseResource();
}

void C3PCInviteTask::InitStateMachine()
{
    CStateProc cIdleProc;
    cIdleProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnIdle;
    cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnIdle;
    cIdleProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteTask::OnIdleTimer;
    AddRuleProc(Idle, cIdleProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cWaitAckProc;
    cWaitAckProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnWaitAck;
    cWaitAckProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnWaitAck;
    cWaitAckProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteTask::OnWaitAckTimer;
    AddRuleProc(WaitAck, cWaitAckProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCInviteTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&C3PCInviteTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(Idle);
}

u32 C3PCInviteTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_DEL;
    }

    switch(pcMsg->event)
    {
    case INVITE_THIRD_PARTY_CALL_REQ:
        {
            //获取消息体
            ProcSipReq2<CInviteThirdPartyCallReq, CInviteThirdPartyCallRsp>(pcMsg);

            CMS_RESULT nErrorCode = ProcInvite3PCReq();
            if (ERR_CMU_CMU_NOT_CONN == nErrorCode)
            {
                break;
            }
            else if (CMS_SUCCESS != nErrorCode)
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                NextState(WaitRsp);
            }
        }
        break;

    case CMU_CMU_CONNECT_SUCCESS:
        {
            //异域cmu登录成功, 可以发送请求

            if (ProcInvite3PCReq() != CMS_SUCCESS)
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                NextState(WaitRsp);
            }
        }
        break;

    case CMU_CMU_DISCONNECT:
    case CMU_CMU_DELETE:
        {
            dwRet = PROCMSG_DEL;
            break;
        }
        break;

    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_SIP_MSG_PROC_FAIL:
        {
            ProcOspSipException(pcMsg);
            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;

    default:
        {
            dwRet = PROCMSG_DEL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteTask::OnIdleTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "任务超时，删除该任务\n");
        //this->PrintData();

        //通常这里超时是因为跨域的cmu未连接超时，本身会有相应错误码
        //this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 C3PCInviteTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_DEL;
    }

    switch(pcMsg->event)
    {
    case CMU_SWITCH_FINISH_CB:
        {
            if (ProcSwitchFinishCB(pcMsg) != CMS_SUCCESS)
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                NextState(WaitAck);
            }
        }
        break;

    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_SIP_MSG_PROC_FAIL:
        {
            ProcOspSipException(pcMsg);
            dwRet = PROCMSG_DEL;
            return dwRet;
        }
        break;

    default:
        {
            dwRet = PROCMSG_DEL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteTask::OnWaitRspTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "任务超时，删除该任务\n");
        //this->PrintData();

        this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 C3PCInviteTask::OnWaitAck(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_FAIL;
    }

    switch(pcMsg->event)
    {
	case TAS_VIDSRC_INFO_QRY_RSP:
		{
			ProcVidSrcInfoQryRsp(pcMsg);
		}
		break;
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
		{
			ProcVidSrcInfoQryNtfReq(pcMsg);
		}
		break;
	case PU_VIDSRC_OSD_SET_RSP:
		{
			ProcVidSrcOsdSetRsp(pcMsg);
		}
		break;
    case INVITE_THIRD_PARTY_CALL_ACK:
        {
            if (ProcInvite3PCAck(pcOspSipMsg) != CMS_SUCCESS)
            {
                dwRet = PROCMSG_FAIL;
            }
            else
            {
                NextState(Service);
            }
        }
        break;

        //{{通用消息处理
    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_SIP_MSG_PROC_FAIL:
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

    case OSP_SIP_BYE_RSP:
    case BYE_RSP:
        {
            if (!ProcByeRsp(pcMsg))
            {
                dwRet = PROCMSG_FAIL;
            }

            return dwRet;
        }
        break;
        //}}通用消息处理

    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteTask::OnWaitAckTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "任务超时，删除该任务\n");
        //this->PrintData();

        this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 C3PCInviteTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_FAIL;
    }

    switch(pcMsg->event)
    {
	case TAS_VIDSRC_INFO_QRY_RSP:
		{
			ProcVidSrcInfoQryRsp(pcMsg);
		}
		break;
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
		{
			ProcVidSrcInfoQryNtfReq(pcMsg);
		}
		break;
	case PU_VIDSRC_OSD_SET_RSP:
		{
			ProcVidSrcOsdSetRsp(pcMsg);
		}
		break;
    case VCR_CTRL_REQ:
        {
            if (NULL != m_pcDstSwitchTask)
            {
                m_pcDstSwitchTask->ProcMsg(pcMsg);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            //SendSwitchTask转来的消息
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            DoPostInDlgReq(pcOspSipMsg->GetSipReq(), cReq);
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            //SendSwitchTask转来的消息
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            DoPostInDlgReq(pcOspSipMsg->GetSipReq(), cReq);
        }
        break;
    case PLAYBACK_PROGRESS_NTF_RSP:
    case END_OF_FILE_NTF_RSP:
        {
            TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "收到放像通知的应答 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
        //{{通用消息处理
    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_SIP_MSG_PROC_FAIL:
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

    case OSP_SIP_BYE_RSP:
    case BYE_RSP:
        {
            if (!ProcByeRsp(pcMsg))
            {
                dwRet = PROCMSG_FAIL;
            }

            return dwRet;
        }
        break;
        //}}通用消息处理

    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCInviteTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

CMS_RESULT C3PCInviteTask::ProcInvite3PCReq()
{
    this->SetErrorCode(CMS_SUCCESS);

    CStartThirdPartyCallReq& cReq = *this->GetRealReq<CStartThirdPartyCallReq>();

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s].\n", 
        cReq.GetDstChn().ToString().c_str(), cReq.GetSrcChn().ToString().c_str());

	m_tEncChn = cReq.GetSrcChn();
	m_tDecChn = cReq.GetDstChn();

    //暂时不支持异域目的的第三方交换
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        this->SetErrorCode(ERR_CMU_NOT_SUPPORT);
        return this->GetErrorCode();
    }

    //把交换任务添加到设备会话任务中，便于该设备断链时能正确清空所有子任务
    CLoginSession* pSessTask = GetLoginSession(cReq.GetSession());
    if(NULL == pSessTask)
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "Dev[%s] not exist!\n", cReq.GetSession().c_str());

        this->SetErrorCode(ERR_CMU_SWITCH_DEV_NOT_AVAILABLE);
        return this->GetErrorCode();
    }

    pSessTask->AddTask(this);
    m_pDevSsnTask = pSessTask;


    //开始交换
    CMS_RESULT nErrorCode = CSwitchTask::StartSwitch(cReq.GetDstChn(), cReq.GetSrcChn(), cReq.GetSdp(), this);
    this->SetErrorCode(nErrorCode);

    return this->GetErrorCode();
}

CMS_RESULT C3PCInviteTask::ProcSwitchFinishCB(CMessage *const pcMsg)
{
    u32 dwTskNo = *(u32*)pcMsg->content;
    CMS_ASSERT(m_pcDstSwitchTask->GetTaskNO() == dwTskNo);

    this->SetErrorCode(m_pcDstSwitchTask->GetErrorCode());

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SwitchFinish[%s].\n", m_pcDstSwitchTask->GetReceiver().GetChannel().ToString().c_str());

    //发送应答
    CInviteThirdPartyCallRsp cRsp;
    DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);

	if (g_cCmuConf.m_bEnableAutoDecoderAudioCall)		// 平台开启了自动解码器呼叫的功能
	{
		// 支持1.0平台的模拟落地方案，对于符合条件的解码器选看，由cmu自动创建解码器呼叫的交换
		ConstructDecoderAudioCallSwitch();
	}

	if (!g_cCmuConf.m_bDisplayAreaInfo)
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "平台配置不叠加字幕\n");
		return this->GetErrorCode();
	}

	//获取辖区信息	
	map<TDevExtDataType, string> mapDevExtData = GetPuDataByUri(m_tEncChn.GetDevUri())->GetDevInfo().GetDevData().GetExtData();
	if (mapDevExtData.end() != mapDevExtData.find(DEV_EXT_FIXADDR) && !(mapDevExtData.find(DEV_EXT_FIXADDR)->second.empty()))
	{
		string strFixAddr = mapDevExtData.find(DEV_EXT_FIXADDR)->second;
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "使用监控点[%s-%d]安装信息[%s]\n",
			m_tEncChn.GetDevUri().c_str(), m_tEncChn.GetChnNO(), strFixAddr.c_str());
		//叠加辖区字幕
		DisplayAreaInfo(strFixAddr);
	} 
	else
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "无安装位置，获取监控点[%s-%d]分组信息\n",
			m_tEncChn.GetDevUri().c_str(), m_tEncChn.GetChnNO());
		CVidsrcInfoQryReq cReq;
		cReq.SetSession(g_pcRegTasTask->GetSession());
		cReq.SetQueryTag(CUUID());
		CDevVidsrc cDevVidsrc;
		TSipURI tDevUri(m_tEncChn.GetDevUri());
		cDevVidsrc.SetDevId(tDevUri.GetUser());
		u16 wVidSrcNO = GetEncVidSrcNO();
		cDevVidsrc.SetVidSrcIdx(wVidSrcNO);
		cReq.GetDevVidsrcs().push_back(cDevVidsrc);
		PostMsgReq(cReq, g_pcRegTasTask->GetTasUri());
		m_strQueryTag = cReq.GetQueryTag();
		g_cPuQueryTaskList.Insert(m_strQueryTag, this);
	}

    return this->GetErrorCode();
}

s32 C3PCInviteTask::GetSrcChnForDecoderAudioCall()
{
	// 该函数用于计算解码器呼叫中源端（即解码器）的通道
	// 由于目前模拟落地方案中，解码器是D01E/D01G，都只有一路音频编码能力，所以计算出来的结果肯定都是0

	// 如果以后模拟落地需要支持其他的解码器，有多视频输出和多音频编码能力（即实际应用时，一个解码器对应有多个编码器）
	// 这种情况下，就需要根据上墙操作中解码器的通道来计算进行解码器呼叫时使用的通道

	// 暂时直接返回0，以后有需要的时候再扩展
	return 0;
}

s32 C3PCInviteTask::GetDstChnForDecoderAudioCall()
{
	// 该函数用于计算解码器呼叫中目的端（即前端IPC）的通道
	// 由于目前前端IPC基本上都是只有一路音频解码能力，所以计算出来的结果肯定都是0

	// 如果前端IPC是NVR或者SVR，上面接有多个视频源，每个视频源又有音频解码能力，那么就要根据前端IPC上墙的通道
	// 来计算该通道对应的音频解码通道
	CStartThirdPartyCallReq& cReq = *this->GetRealReq<CStartThirdPartyCallReq>();
	const TChannel &tChn = cReq.GetSrcChn();		// 上墙的源端，对应的即是解码器呼叫的目的端
    CPuData *pcPuData = GetPuDataByUri(tChn.GetDevUri());
	if (NULL != pcPuData)
	{
		if (pcPuData->GetBaseCapSet().GetAudDecNum() <= 1)		// 只有一路音频解码能力，直接返回0
		{
			return 0;
		}

		// 当前端有多路音频解码能力的时候，先根据浏览的通道号，获取出其视频源编号，进而获取解码通道号
        CVidSrcSet cVidSrcSet = pcPuData->GetChnVidSrc(tChn.GetChnNO());
        if (!cVidSrcSet.empty())
        {
            u32 dwVidSrcSize = cVidSrcSet.size();
            if (dwVidSrcSize > 1)
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "上墙的前端通道[%u]对应着多个[%u]视频源，呼叫取第一个视频源上的音频通道!\n", 
                    tChn.GetChnNO(), dwVidSrcSize);
            }

            // 这里假定每一个视频源只有一个音频解码通道，即前端视频源索引号和音频解码索引号一致
            // 如果一个视频源有多个音频解码通道，则需要进一步根据视频源索引号来计算音频解码索引号
            return *(cVidSrcSet.begin());
        }
	}

	// 其他所有情况，都返回0通道
	return 0;
}

CMS_RESULT C3PCInviteTask::ProcInvite3PCAck(const COspSipMsg*const pcOspSipMsg)
{
	TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "ProcInvite3PCAck.\n");

    return this->GetErrorCode();
}

void C3PCInviteTask::ReleaseResource()
{
    C3PCTask::ReleaseResource();

	if (!m_strQueryTag.empty())
	{
		g_cPuQueryTaskList.Erase(m_strQueryTag);
		m_strQueryTag.clear();
	}

    //开始拆除交换
    if(m_pcDstSwitchTask)
    {
        m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
        SAFE_DEL_PTR(m_pcDstSwitchTask);
    }

    //删除任务间的依赖关系
    if(m_pDevSsnTask)
    {
        m_pDevSsnTask->DelTask(this);   //解除交换任务与发起请求的设备登录会话的关系
    }

    //处理对话资源
	if(INVALID_TRANS_ID != this->GetSipTransID())
	{
        //任务还未完成（未应答）就销毁，肯定是失败了，错误码不应该还是CMS_SUCCESS
        if (this->GetErrorCode() == CMS_SUCCESS)
        {
            CMS_ASSERT(false && "任务还未完成（未应答）就销毁，肯定是失败了，错误码不应该还是CMS_SUCCESS");
            this->SetErrorCode(ERR_CMU_SWITCH_EXCEPTION);
        }

		//发送应答
        CInviteThirdPartyCallRsp cRsp;
        DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);

        //应答失败的不再发送Bye
        if(INVALID_DIALOG_ID != this->GetSipDlgID())
        {
            OspSipReleaseDlg(this->GetSipDlgID());
            this->SetSipDlgID(INVALID_DIALOG_ID);
        }
	}

    //向3PCC端发送Bye请求
    if(INVALID_DIALOG_ID != this->GetSipDlgID())
    {
        CByeReq cReq;
		if (m_pDevSsnTask)
		{
			cReq.SetSession(m_pDevSsnTask->GetSession());
		}
        cReq.SetReason(this->GetErrorCode());

        DoPostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq);
    }

	if (g_cCmuConf.m_bEnableAutoDecoderAudioCall)
	{
		//拆除与之关联的解码器呼叫交换
		DestructDecoderAudioCallSwitch();
	}
}

void C3PCInviteTask::ConstructDecoderAudioCallSwitch()
{
	const CStartThirdPartyCallReq &cReq = *this->GetRealReq<CStartThirdPartyCallReq>();
	CPuData *pcSrcPuData = GetPuDataByUri(cReq.GetSrcChn().GetDevUri());
	if (NULL == pcSrcPuData 
		|| pcSrcPuData->GetBaseCapSet().GetAudDecNum() <= 0)		// 上墙的前端如果没有音频解码能力则无法呼叫
	{
		return;
	}

	// 支持1.0平台的模拟落地方案，对于符合条件的解码器选看，由cmu自动创建解码器呼叫的交换
	CPuData *pcPuData = GetPuDataByUri(cReq.GetDstChn().GetDevUri());
	if (NULL != pcPuData)
	{
		map<int,int> tExtCapSet = pcPuData->GetExtCapSet();

		// 音频编码数量大于0且具有解码器呼叫的扩展能力
		if (0 < pcPuData->GetBaseCapSet().GetAudEncNum() 
			&& tExtCapSet.end() != tExtCapSet.find(VSIP_ENTITY_DECODER_AUDIO_CALL))
		{
			// 支持模拟落地方案，自动建立一路从解码器到编码器的语音呼叫，关键是确定解码器和编码器的逻辑通道号
			// 根据1.0的方案，解码器呼叫时，经pui计算后解码器的实际通道为0，cmu内部逻辑通道为150；
			// 至于编码器的通道，则需要根据解码器选看对应的编码器通道号来确定

			// 构造解码器呼叫的源、目的通道信息
			TChannel tDstChn = cReq.GetSrcChn();
			tDstChn.SetChnNO(GetDstChnForDecoderAudioCall());
			TChannel tSrcChn = cReq.GetDstChn();
			tSrcChn.SetChnNO(GetSrcChnForDecoderAudioCall());		// 解码器呼叫的逻辑通道采用1.0的规则，固定为0，pui计算后的端口为0

			// 模拟构造一个sip请求的消息
			CStartThirdPartyCallReq c3PCReq;
			c3PCReq.SetDstChn(tDstChn);
			c3PCReq.SetSrcChn(tSrcChn);
			c3PCReq.GetSdp().SetSessionName(SDP_SESSION_AUDIOCALL);

			COspSipMsg cSipMsg;
			cSipMsg.SetOspEventID(c3PCReq.GetEventId());

			// 由于这个第三方呼叫的消息是内部构造的，无需应答，所以设置TransId为无效
			cSipMsg.SetSipTransID(INVALID_TRANS_ID);

			string strMsg = c3PCReq.ToXml();
			cSipMsg.SetMsgBody(strMsg.c_str(), strMsg.length() + 1);
			CMessage cMsg;
			cMsg.event   = cSipMsg.GetOspEventID();
			cMsg.content = (u8 *)(&cSipMsg);
			cMsg.length  = cSipMsg.GetMsgLen();

			// 处理模拟的音频呼叫消息
			C3PCMsgTask *pcTask = new C3PCMsgTask(GetInstance());
			if (NULL != pcTask)
			{
				pcTask->InitStateMachine();

				TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "解码器符合解码器呼叫的条件，由cmu自动创建解码器呼叫，Task[%u]!\n", 
					pcTask->GetTaskNO());
				
				if (PROCMSG_DEL == pcTask->ProcMsg(&cMsg))
				{
					TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "cmu自动创建解码器呼叫时失败!Task[%u]!\n", 
						pcTask->GetTaskNO());

					delete pcTask;
					pcTask = NULL;
				}
				else
				{
					m_bCreateDecoderAudioCall = true;	// 解码器呼叫交换正在建立中，设置标记，便于后续拆除交换
				}
			}
		}
	}
}

void C3PCInviteTask::DestructDecoderAudioCallSwitch()
{
	if (m_bCreateDecoderAudioCall)
	{
		const CStartThirdPartyCallReq &cReq = *this->GetRealReq<CStartThirdPartyCallReq>();

		// 构造解码器呼叫的源、目的通道信息
		TChannel tDstChn = cReq.GetSrcChn();
		tDstChn.SetChnNO(GetDstChnForDecoderAudioCall());
		TChannel tSrcChn = cReq.GetDstChn();
		tSrcChn.SetChnNO(GetSrcChnForDecoderAudioCall());		// 解码器呼叫的逻辑通道采用1.0的规则，固定为0，pui计算后的端口为0

		// 模拟构造一个sip请求的消息
		CStopThirdPartyCallReq c3PCReq;
		c3PCReq.SetDstChn(tDstChn);
		c3PCReq.SetSrcChn(tSrcChn);

		COspSipMsg cSipMsg;
		cSipMsg.SetOspEventID(c3PCReq.GetEventId());

		// 由于这个第三方呼叫的消息是内部构造的，无需应答，所以设置TransId为无效
		cSipMsg.SetSipTransID(INVALID_TRANS_ID);

		string strMsg = c3PCReq.ToXml();
		cSipMsg.SetMsgBody(strMsg.c_str(), strMsg.length() + 1);
		CMessage cMsg;
		cMsg.event   = cSipMsg.GetOspEventID();
		cMsg.content = (u8 *)(&cSipMsg);
		cMsg.length  = cSipMsg.GetMsgLen();

		// 处理模拟的音频呼叫消息
		C3PCMsgTask *pcTask = new C3PCMsgTask(GetInstance());
		if (NULL != pcTask)
		{
			pcTask->InitStateMachine();

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "开始拆除平台自动创建的解码器呼叫交换，Task[%u]!\n", 
				pcTask->GetTaskNO());
			
			if (PROCMSG_DEL == pcTask->ProcMsg(&cMsg))
			{
				delete pcTask;
				pcTask = NULL;
			}
		}
	}
}

void C3PCInviteTask::DisplayAreaInfo(const string& strAreaInfo)
{
	//不支持异域Dec
	if (!IsLocalDomainByDevUri(m_tDecChn.GetDevUri()))
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] 叠加辖区字幕不支持异域Dec[%s]\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}	

	////是否需要叠加辖区字幕
	//if (!g_cCmuConf.m_bDisplayAreaInfo)
	//{
	//	TASKLOG(EVENT_LEV, "[DisplayAreaInfo] 平台配置不叠加辖区字幕\n");
	//	return;
	//}

	//判断此Dec是否支持新字幕
	CPuData* pcDecData = GetPuDataByUri(m_tDecChn.GetDevUri());
	if (NULL == pcDecData)
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] 获取Dec[%s]失败\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}
	if (pcDecData->GetExtCapSet().end() == pcDecData->GetExtCapSet().find(VSIP_ENTITY_NEW_OSD))
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]不支持新字幕\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}

	//确定此Dec路由
	TSipURI tPuRouterUri = GetPuRouter(m_tDecChn.GetDevUri());
	if (tPuRouterUri.IsNull())
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]无法路由, 失败\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}

	if (NULL == pcDecData->GetPuRegTask())
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]不在线\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}

	CPuVidSrcOsdSetReq cOsdReq;
	cOsdReq.SetSession(pcDecData->GetPuRegTask()->GetSession());
	cOsdReq.SetPuChn(m_tDecChn);
	TPuVidSrcOsdCfg tOsdCfg;
	TPuVidSrcOsdCfgItem tOsdCfgItem;
	tOsdCfgItem.SetPuVidSrcOsdType(PU_VIDSRC_OSD_TYPE_LOGO);
	tOsdCfgItem.SetPuVidSrcOsdId(0);
	tOsdCfgItem.SetPuVidSrcOsdEnable(TRUE);
	tOsdCfgItem.SetPuVidSrcOsdContent(strAreaInfo);
	tOsdCfg.push_back(tOsdCfgItem);
	cOsdReq.SetPuVidSrcOsdCfg(tOsdCfg);
	PostMsgReq(cOsdReq, tPuRouterUri);
}

u16  C3PCInviteTask::GetEncVidSrcNO()
{
	u16 wEncVidSrcNO = 0;

	CVidSrcSet cVidSrcSet = GetPuDataByUri(m_tEncChn.GetDevUri())->GetChnVidSrc(m_tEncChn.GetChnNO());
	if (cVidSrcSet.size() == 1)
	{
		wEncVidSrcNO = *(cVidSrcSet.begin());
	} 
	else
	{
		TPuBaseCapSet tPuBaseCapSet = GetPuDataByUri(m_tEncChn.GetDevUri())->GetBaseCapSet();
		u16 wVidSrcNum = tPuBaseCapSet.GetVideoInPortNum();
		u16 wVidEncNum = tPuBaseCapSet.GetVidEncNum();
		if (wVidEncNum % wVidSrcNum == 0)
		{//IPC/NVR
			wEncVidSrcNO = m_tEncChn.GetChnNO() % wVidSrcNum;
		} 
		else if ((wVidEncNum+1) % wVidSrcNum == 0)
		{//SVR
			wEncVidSrcNO = (m_tEncChn.GetChnNO()<wVidSrcNum) ? m_tEncChn.GetChnNO():((m_tEncChn.GetChnNO()+1)%wVidSrcNum);
		}
		else
		{
			TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "编码器[%s]的视频源数目[%d]和编码通道数目[%d]有误！！！\n",
				m_tEncChn.GetDevUri().c_str(), wVidSrcNum, wVidEncNum);
		}
	}

	return wEncVidSrcNO;
}

void C3PCInviteTask::ProcVidSrcInfoQryRsp(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcInfoQryRsp] ospsip 消息为空\n");
		return;
	}

	CVidsrcInfoQryRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);
	if (CMS_SUCCESS != cRsp.GetErrorCode())
	{
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "[ProcVidSrcInfoQryRsp] ErrCode[%d]\n", cRsp.GetErrorCode());
	}
}

void C3PCInviteTask::ProcVidSrcInfoQryNtfReq(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcInfoQryNtfReq] ospsip 消息为空\n");
		return;
	}

	CVidsrcInfoQryNtfReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	if (cReq.GetDevVidsrcInfo().size() != 1)
	{
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "[ProcVidSrcInfoQryNtfReq] 结果数量[%d]错误\n",
			cReq.GetDevVidsrcInfo().size());	
	} 
	else
	{
		string strAreaInfo = TSipURI(m_tEncChn.GetDevUri()).GetDomain() + ":" + cReq.GetDevVidsrcInfo().at(0).GetDevGrpName();
		string strCutedAreaInfo = CutDevGrpGbId(strAreaInfo);
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "[ProcVidSrcInfoQryNtfReq] AreaInfo[%s], CutedAreaInfo[%s]\n", 
			strAreaInfo.c_str(), strCutedAreaInfo.c_str());
		DisplayAreaInfo(strCutedAreaInfo);
	}

	CNtfRsp cRsp;
	u32 dwResult = PostRsp(KDSIP_EVENT_MESSAGE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "发送KDSIP_EVENT_MESSAGE_RSP, ResultCode[%lu].\n",
		dwResult);
}

void C3PCInviteTask::ProcVidSrcOsdSetRsp(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcOsdSetRsp] ospsip 消息为空\n");
		return;
	}

	CPuVidSrcOsdSetRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);
	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "[ProcVidSrcOsdSetRsp] Enc(%s:%d), Dec(%s:%d), 设置完成\n",
			m_tEncChn.GetDevUri().c_str(),
			m_tEncChn.GetChnNO(),
			m_tDecChn.GetDevUri().c_str(),
			m_tDecChn.GetChnNO());
	}
	else
	{
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "[ProcVidSrcOsdSetRsp] Enc(%s:%d), Dec(%s:%d), ErrCode(%d)\n",
			m_tEncChn.GetDevUri().c_str(),
			m_tEncChn.GetChnNO(),
			m_tDecChn.GetDevUri().c_str(),
			m_tDecChn.GetChnNO(),
			cRsp.GetErrorCode());
	}
}

string C3PCInviteTask::CutDevGrpGbId(const string& strOrgName)
{
	/*条件：
	 带国标编码的格式：上海市-3121000000
	 1、长度大于等于12
	 2、倒数第11位为‘-’
	 3、最后10位为数字
	*/
	string newName=strOrgName;

	size_t nStrlen=strOrgName.length();
	if (nStrlen>=12)
	{
		if (strOrgName.substr(nStrlen-11,1)=="-")
		{
			if (IsAllDigit(strOrgName.substr(nStrlen-10)))
			{
				newName=strOrgName.substr(0,nStrlen-11);
			}
		}
	}

	return newName;
}

bool C3PCInviteTask::IsAllDigit(const string& str)
{
	for (u32 i=0;i<str.length();i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}

	return true;
}

