/*****************************************************************************
ģ����      : cmu 
�ļ���      : 3pcinvitetask.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: CSwitchCreatorʵ��
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2012/08/02     1.0        ������        ����
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
        CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip ��ϢΪ��\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_DEL;
    }

    switch(pcMsg->event)
    {
    case INVITE_THIRD_PARTY_CALL_REQ:
        {
            //��ȡ��Ϣ��
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
            //����cmu��¼�ɹ�, ���Է�������

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
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "����ʱ��ɾ��������\n");
        //this->PrintData();

        //ͨ�����ﳬʱ����Ϊ�����cmuδ���ӳ�ʱ�����������Ӧ������
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
        CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip ��ϢΪ��\n");
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
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "����ʱ��ɾ��������\n");
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
        CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip ��ϢΪ��\n");
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

        //{{ͨ����Ϣ����
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
        //}}ͨ����Ϣ����

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
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "����ʱ��ɾ��������\n");
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
        CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip ��ϢΪ��\n");
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
            //SendSwitchTaskת������Ϣ
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            DoPostInDlgReq(pcOspSipMsg->GetSipReq(), cReq);
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            //SendSwitchTaskת������Ϣ
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            DoPostInDlgReq(pcOspSipMsg->GetSipReq(), cReq);
        }
        break;
    case PLAYBACK_PROGRESS_NTF_RSP:
    case END_OF_FILE_NTF_RSP:
        {
            TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "�յ�����֪ͨ��Ӧ�� Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
        //{{ͨ����Ϣ����
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
        //}}ͨ����Ϣ����

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

    //��ʱ��֧������Ŀ�ĵĵ���������
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        this->SetErrorCode(ERR_CMU_NOT_SUPPORT);
        return this->GetErrorCode();
    }

    //�ѽ���������ӵ��豸�Ự�����У����ڸ��豸����ʱ����ȷ�������������
    CLoginSession* pSessTask = GetLoginSession(cReq.GetSession());
    if(NULL == pSessTask)
    {
        TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "Dev[%s] not exist!\n", cReq.GetSession().c_str());

        this->SetErrorCode(ERR_CMU_SWITCH_DEV_NOT_AVAILABLE);
        return this->GetErrorCode();
    }

    pSessTask->AddTask(this);
    m_pDevSsnTask = pSessTask;


    //��ʼ����
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

    //����Ӧ��
    CInviteThirdPartyCallRsp cRsp;
    DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);

	if (g_cCmuConf.m_bEnableAutoDecoderAudioCall)		// ƽ̨�������Զ����������еĹ���
	{
		// ֧��1.0ƽ̨��ģ����ط��������ڷ��������Ľ�����ѡ������cmu�Զ��������������еĽ���
		ConstructDecoderAudioCallSwitch();
	}

	if (!g_cCmuConf.m_bDisplayAreaInfo)
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "ƽ̨���ò�������Ļ\n");
		return this->GetErrorCode();
	}

	//��ȡϽ����Ϣ	
	map<TDevExtDataType, string> mapDevExtData = GetPuDataByUri(m_tEncChn.GetDevUri())->GetDevInfo().GetDevData().GetExtData();
	if (mapDevExtData.end() != mapDevExtData.find(DEV_EXT_FIXADDR) && !(mapDevExtData.find(DEV_EXT_FIXADDR)->second.empty()))
	{
		string strFixAddr = mapDevExtData.find(DEV_EXT_FIXADDR)->second;
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "ʹ�ü�ص�[%s-%d]��װ��Ϣ[%s]\n",
			m_tEncChn.GetDevUri().c_str(), m_tEncChn.GetChnNO(), strFixAddr.c_str());
		//����Ͻ����Ļ
		DisplayAreaInfo(strFixAddr);
	} 
	else
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "�ް�װλ�ã���ȡ��ص�[%s-%d]������Ϣ\n",
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
	// �ú������ڼ��������������Դ�ˣ�������������ͨ��
	// ����Ŀǰģ����ط����У���������D01E/D01G����ֻ��һ·��Ƶ�������������Լ�������Ľ���϶�����0

	// ����Ժ�ģ�������Ҫ֧�������Ľ��������ж���Ƶ����Ͷ���Ƶ������������ʵ��Ӧ��ʱ��һ����������Ӧ�ж����������
	// ��������£�����Ҫ������ǽ�����н�������ͨ����������н���������ʱʹ�õ�ͨ��

	// ��ʱֱ�ӷ���0���Ժ�����Ҫ��ʱ������չ
	return 0;
}

s32 C3PCInviteTask::GetDstChnForDecoderAudioCall()
{
	// �ú������ڼ��������������Ŀ�Ķˣ���ǰ��IPC����ͨ��
	// ����Ŀǰǰ��IPC�����϶���ֻ��һ·��Ƶ�������������Լ�������Ľ���϶�����0

	// ���ǰ��IPC��NVR����SVR��������ж����ƵԴ��ÿ����ƵԴ������Ƶ������������ô��Ҫ����ǰ��IPC��ǽ��ͨ��
	// �������ͨ����Ӧ����Ƶ����ͨ��
	CStartThirdPartyCallReq& cReq = *this->GetRealReq<CStartThirdPartyCallReq>();
	const TChannel &tChn = cReq.GetSrcChn();		// ��ǽ��Դ�ˣ���Ӧ�ļ��ǽ��������е�Ŀ�Ķ�
    CPuData *pcPuData = GetPuDataByUri(tChn.GetDevUri());
	if (NULL != pcPuData)
	{
		if (pcPuData->GetBaseCapSet().GetAudDecNum() <= 1)		// ֻ��һ·��Ƶ����������ֱ�ӷ���0
		{
			return 0;
		}

		// ��ǰ���ж�·��Ƶ����������ʱ���ȸ��������ͨ���ţ���ȡ������ƵԴ��ţ�������ȡ����ͨ����
        CVidSrcSet cVidSrcSet = pcPuData->GetChnVidSrc(tChn.GetChnNO());
        if (!cVidSrcSet.empty())
        {
            u32 dwVidSrcSize = cVidSrcSet.size();
            if (dwVidSrcSize > 1)
            {
                TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��ǽ��ǰ��ͨ��[%u]��Ӧ�Ŷ��[%u]��ƵԴ������ȡ��һ����ƵԴ�ϵ���Ƶͨ��!\n", 
                    tChn.GetChnNO(), dwVidSrcSize);
            }

            // ����ٶ�ÿһ����ƵԴֻ��һ����Ƶ����ͨ������ǰ����ƵԴ�����ź���Ƶ����������һ��
            // ���һ����ƵԴ�ж����Ƶ����ͨ��������Ҫ��һ��������ƵԴ��������������Ƶ����������
            return *(cVidSrcSet.begin());
        }
	}

	// �������������������0ͨ��
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

    //��ʼ�������
    if(m_pcDstSwitchTask)
    {
        m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
        SAFE_DEL_PTR(m_pcDstSwitchTask);
    }

    //ɾ��������������ϵ
    if(m_pDevSsnTask)
    {
        m_pDevSsnTask->DelTask(this);   //������������뷢��������豸��¼�Ự�Ĺ�ϵ
    }

    //����Ի���Դ
	if(INVALID_TRANS_ID != this->GetSipTransID())
	{
        //����δ��ɣ�δӦ�𣩾����٣��϶���ʧ���ˣ������벻Ӧ�û���CMS_SUCCESS
        if (this->GetErrorCode() == CMS_SUCCESS)
        {
            CMS_ASSERT(false && "����δ��ɣ�δӦ�𣩾����٣��϶���ʧ���ˣ������벻Ӧ�û���CMS_SUCCESS");
            this->SetErrorCode(ERR_CMU_SWITCH_EXCEPTION);
        }

		//����Ӧ��
        CInviteThirdPartyCallRsp cRsp;
        DoPostRsp(KDSIP_EVENT_INVITE_RSP, cRsp);

        //Ӧ��ʧ�ܵĲ��ٷ���Bye
        if(INVALID_DIALOG_ID != this->GetSipDlgID())
        {
            OspSipReleaseDlg(this->GetSipDlgID());
            this->SetSipDlgID(INVALID_DIALOG_ID);
        }
	}

    //��3PCC�˷���Bye����
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
		//�����֮�����Ľ��������н���
		DestructDecoderAudioCallSwitch();
	}
}

void C3PCInviteTask::ConstructDecoderAudioCallSwitch()
{
	const CStartThirdPartyCallReq &cReq = *this->GetRealReq<CStartThirdPartyCallReq>();
	CPuData *pcSrcPuData = GetPuDataByUri(cReq.GetSrcChn().GetDevUri());
	if (NULL == pcSrcPuData 
		|| pcSrcPuData->GetBaseCapSet().GetAudDecNum() <= 0)		// ��ǽ��ǰ�����û����Ƶ�����������޷�����
	{
		return;
	}

	// ֧��1.0ƽ̨��ģ����ط��������ڷ��������Ľ�����ѡ������cmu�Զ��������������еĽ���
	CPuData *pcPuData = GetPuDataByUri(cReq.GetDstChn().GetDevUri());
	if (NULL != pcPuData)
	{
		map<int,int> tExtCapSet = pcPuData->GetExtCapSet();

		// ��Ƶ������������0�Ҿ��н��������е���չ����
		if (0 < pcPuData->GetBaseCapSet().GetAudEncNum() 
			&& tExtCapSet.end() != tExtCapSet.find(VSIP_ENTITY_DECODER_AUDIO_CALL))
		{
			// ֧��ģ����ط������Զ�����һ·�ӽ����������������������У��ؼ���ȷ���������ͱ��������߼�ͨ����
			// ����1.0�ķ���������������ʱ����pui������������ʵ��ͨ��Ϊ0��cmu�ڲ��߼�ͨ��Ϊ150��
			// ���ڱ�������ͨ��������Ҫ���ݽ�����ѡ����Ӧ�ı�����ͨ������ȷ��

			// ������������е�Դ��Ŀ��ͨ����Ϣ
			TChannel tDstChn = cReq.GetSrcChn();
			tDstChn.SetChnNO(GetDstChnForDecoderAudioCall());
			TChannel tSrcChn = cReq.GetDstChn();
			tSrcChn.SetChnNO(GetSrcChnForDecoderAudioCall());		// ���������е��߼�ͨ������1.0�Ĺ��򣬹̶�Ϊ0��pui�����Ķ˿�Ϊ0

			// ģ�⹹��һ��sip�������Ϣ
			CStartThirdPartyCallReq c3PCReq;
			c3PCReq.SetDstChn(tDstChn);
			c3PCReq.SetSrcChn(tSrcChn);
			c3PCReq.GetSdp().SetSessionName(SDP_SESSION_AUDIOCALL);

			COspSipMsg cSipMsg;
			cSipMsg.SetOspEventID(c3PCReq.GetEventId());

			// ����������������е���Ϣ���ڲ�����ģ�����Ӧ����������TransIdΪ��Ч
			cSipMsg.SetSipTransID(INVALID_TRANS_ID);

			string strMsg = c3PCReq.ToXml();
			cSipMsg.SetMsgBody(strMsg.c_str(), strMsg.length() + 1);
			CMessage cMsg;
			cMsg.event   = cSipMsg.GetOspEventID();
			cMsg.content = (u8 *)(&cSipMsg);
			cMsg.length  = cSipMsg.GetMsgLen();

			// ����ģ�����Ƶ������Ϣ
			C3PCMsgTask *pcTask = new C3PCMsgTask(GetInstance());
			if (NULL != pcTask)
			{
				pcTask->InitStateMachine();

				TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "���������Ͻ��������е���������cmu�Զ��������������У�Task[%u]!\n", 
					pcTask->GetTaskNO());
				
				if (PROCMSG_DEL == pcTask->ProcMsg(&cMsg))
				{
					TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "cmu�Զ���������������ʱʧ��!Task[%u]!\n", 
						pcTask->GetTaskNO());

					delete pcTask;
					pcTask = NULL;
				}
				else
				{
					m_bCreateDecoderAudioCall = true;	// ���������н������ڽ����У����ñ�ǣ����ں����������
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

		// ������������е�Դ��Ŀ��ͨ����Ϣ
		TChannel tDstChn = cReq.GetSrcChn();
		tDstChn.SetChnNO(GetDstChnForDecoderAudioCall());
		TChannel tSrcChn = cReq.GetDstChn();
		tSrcChn.SetChnNO(GetSrcChnForDecoderAudioCall());		// ���������е��߼�ͨ������1.0�Ĺ��򣬹̶�Ϊ0��pui�����Ķ˿�Ϊ0

		// ģ�⹹��һ��sip�������Ϣ
		CStopThirdPartyCallReq c3PCReq;
		c3PCReq.SetDstChn(tDstChn);
		c3PCReq.SetSrcChn(tSrcChn);

		COspSipMsg cSipMsg;
		cSipMsg.SetOspEventID(c3PCReq.GetEventId());

		// ����������������е���Ϣ���ڲ�����ģ�����Ӧ����������TransIdΪ��Ч
		cSipMsg.SetSipTransID(INVALID_TRANS_ID);

		string strMsg = c3PCReq.ToXml();
		cSipMsg.SetMsgBody(strMsg.c_str(), strMsg.length() + 1);
		CMessage cMsg;
		cMsg.event   = cSipMsg.GetOspEventID();
		cMsg.content = (u8 *)(&cSipMsg);
		cMsg.length  = cSipMsg.GetMsgLen();

		// ����ģ�����Ƶ������Ϣ
		C3PCMsgTask *pcTask = new C3PCMsgTask(GetInstance());
		if (NULL != pcTask)
		{
			pcTask->InitStateMachine();

			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "��ʼ���ƽ̨�Զ������Ľ��������н�����Task[%u]!\n", 
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
	//��֧������Dec
	if (!IsLocalDomainByDevUri(m_tDecChn.GetDevUri()))
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] ����Ͻ����Ļ��֧������Dec[%s]\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}	

	////�Ƿ���Ҫ����Ͻ����Ļ
	//if (!g_cCmuConf.m_bDisplayAreaInfo)
	//{
	//	TASKLOG(EVENT_LEV, "[DisplayAreaInfo] ƽ̨���ò�����Ͻ����Ļ\n");
	//	return;
	//}

	//�жϴ�Dec�Ƿ�֧������Ļ
	CPuData* pcDecData = GetPuDataByUri(m_tDecChn.GetDevUri());
	if (NULL == pcDecData)
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] ��ȡDec[%s]ʧ��\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}
	if (pcDecData->GetExtCapSet().end() == pcDecData->GetExtCapSet().find(VSIP_ENTITY_NEW_OSD))
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]��֧������Ļ\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}

	//ȷ����Dec·��
	TSipURI tPuRouterUri = GetPuRouter(m_tDecChn.GetDevUri());
	if (tPuRouterUri.IsNull())
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]�޷�·��, ʧ��\n", 
			m_tDecChn.GetDevUri().c_str());
		return;
	}

	if (NULL == pcDecData->GetPuRegTask())
	{
		TASKLOG(ERROR_LEV, "[DisplayAreaInfo] Dec[%s]������\n", 
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
			TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "������[%s]����ƵԴ��Ŀ[%d]�ͱ���ͨ����Ŀ[%d]���󣡣���\n",
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
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcInfoQryRsp] ospsip ��ϢΪ��\n");
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
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcInfoQryNtfReq] ospsip ��ϢΪ��\n");
		return;
	}

	CVidsrcInfoQryNtfReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	if (cReq.GetDevVidsrcInfo().size() != 1)
	{
		TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "[ProcVidSrcInfoQryNtfReq] �������[%d]����\n",
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
	TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "����KDSIP_EVENT_MESSAGE_RSP, ResultCode[%lu].\n",
		dwResult);
}

void C3PCInviteTask::ProcVidSrcOsdSetRsp(CMessage *const pcMsg)
{
	COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (pcOspSipMsg == NULL)
	{
		CMS_ASSERT(pcOspSipMsg && "[ProcVidSrcOsdSetRsp] ospsip ��ϢΪ��\n");
		return;
	}

	CPuVidSrcOsdSetRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);
	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		TASKLOG(CMU_MOD_SWITCH, EVENT_LEV, "[ProcVidSrcOsdSetRsp] Enc(%s:%d), Dec(%s:%d), �������\n",
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
	/*������
	 ���������ĸ�ʽ���Ϻ���-3121000000
	 1�����ȴ��ڵ���12
	 2��������11λΪ��-��
	 3�����10λΪ����
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

