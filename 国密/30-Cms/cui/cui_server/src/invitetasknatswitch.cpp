/*****************************************************************************
ģ����      : cui_server 
�ļ���      : invitetasknatswitch.h
����ļ�    : 
�ļ�ʵ�ֹ���: invitetasknatswitch.h ��������CU������������
����        : sunqiang
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2015/09/17   1.0         sunqiang        ����
******************************************************************************/
#include "invitetasknatswitch.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "redisdb.h"
#include "redisssmng.h"

extern CRedisDBInterface g_cRedisDb;
extern CRedisSdk g_cCuiRedisSdk;

//CInviteInterActionNatSwitch
//////////////////////////////////////////////////////////////////////////
//CInviteTaskNatSwitch
CInviteTaskNatSwitch::CInviteTaskNatSwitch(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId) : CSipTask(pcInst), m_cStrmNumRestorer( pcInst )
{
    CMS_ASSERT(NULL != pcCuAgent);
    CMS_ASSERT(NULL != pcInviteInterActTask);
    m_pcCuAgent = pcCuAgent;
    m_pcInviteInterActTask = pcInviteInterActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    m_bByeWhileWaitInviteRsp = false;
	m_nPlayId = nPlayId;
    m_cStrmNumRestorer.SetUserLoginSession( pcCuAgent->m_strSession );
}

CInviteTaskNatSwitch::~CInviteTaskNatSwitch()
{
    TerminateTask();
}

void CInviteTaskNatSwitch::TerminateTask()
{
    Iterator pPos;
    int nSeqNo = 0;
    int nTaskNo = INVALID_TASKNO;

	TASKLOG(NORMAL_TASK, ERROR_LEV, "TerminateTask TaskNo [%d]\n", this->GetTaskNO());

	// �������
	RemoveCuiSwitch();

	while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, nTaskNo) )
        {
            CTask* pcTask = NULL;
            pcTask = GetInstance()->FindTask(nTaskNo);
            if(pcTask != NULL)
            {
                delete pcTask;
                pcTask = NULL;
            }        
        }
    }
    if ( m_tInviteID != INVALID_DIALOG_ID )
    { //�ͷŻỰ
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
        //OspSipReleaseDlg(m_tInviteID);
        m_tInviteID = INVALID_DIALOG_ID;
    }

    //ע��CuAgent�е��������
    if ( INVALID_PLAY_ID != m_tReq.GetDstChn().GetChnNO() )
    {
        m_pcCuAgent->m_cInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        m_tReq.GetDstChn().SetChnNO(INVALID_PLAY_ID);

		PopMediaTask(m_pcCuAgent->m_strSession,m_nPlayId);
    }

    m_pcInviteInterActTask = NULL;
}

void CInviteTaskNatSwitch::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::NotReqProcMsg;
    cNotReqState.TimerPoll = &CSipTask::TimerPollSkip;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotReqState);

    CStateProc cWaitSetupRspState;
    cWaitSetupRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitSetupRspProcMsg;
    cWaitSetupRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskNatSwitch::WaitSetupRspTimerPoll;
    cWaitSetupRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitSetupRspProcMsg;
    AddRuleProc(WAIT_SETUP_RSP_STATE, cWaitSetupRspState);

    CStateProc cWaitSetupAckState;
    cWaitSetupAckState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitSetupAckProcMsg;
    cWaitSetupAckState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskNatSwitch::WaitAckTimerPoll;
    cWaitSetupAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitSetupAckProcMsg;
    AddRuleProc(WAIT_SETUP_ACK_STATE, cWaitSetupAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    CStateProc cWaitByeRspState;
    cWaitByeRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitByeRspProcMsg;
    cWaitByeRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskNatSwitch::WaitByeRspTimerPoll;
    cWaitByeRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskNatSwitch::WaitByeRspProcMsg;
    AddRuleProc(WAIT_BYE_RSP_STATE, cWaitByeRspState);

    NextState(NOT_REQUEST_STATE);
}

bool CInviteTaskNatSwitch::IsNotNeedPollState()
{
	if (GetState() == INVITE_HOLD_STATE)
	{
		return true;
	}

	return false;
}

u32 CInviteTaskNatSwitch::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "���յ�δ֪��Ϣ[%s--%hu]��������\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CInviteTask::NotReqProcMsg�յ�δ֪��Ϣ");
    return PROCMSG_OK;
}

u32 CInviteTaskNatSwitch::WaitSetupRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case INVITE_RSP:
        {
            //�ж��Ƿ�ҵ���ʧ��
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //����DlgID
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
			m_tRsp = cRsp;

			m_vtduChn = cRsp.GetSdp().GetOwner();

            if (CMS_SUCCESS == cRsp.GetErrorCode() && m_bByeWhileWaitInviteRsp)
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED_BY_CMU);
            }
			else if ( CMS_SUCCESS == cRsp.GetErrorCode() && !CuiAddNatSwitch())
			{
				// Դ�����ɹ�����Ŀ�Ľ������ɹ�
				cRsp.SetErrorCode(ERR_CUI_ADD_SWITCH_FAILED);
			}

			TNetAddr cuiNetAddr;
			if ( !this->GetCuiNatAddrForCu(cuiNetAddr) )
			{
				cRsp.SetErrorCode(ERR_CUI_ADD_SWITCH_FAILED);
				RemoveCuiSwitch();
			}

			if ( cRsp.GetErrorCode() == CMS_SUCCESS )
			{
				this->SwitchToCuiNatSwitchAddress(cRsp.GetSdp(), cuiNetAddr);

				cRsp.GetSdp().SetIsNeedPunch(false);

				string strRsp = cRsp.ToXml();
				pcOspSipMsg->SetMsgBody(&strRsp, strlen(strRsp.c_str()+1));
			}

			CMessage cInviteRsp;
			cInviteRsp.event = INVITE_RSP;
			string content = cRsp.ToXml();
			COspSipMsg sipmsg;
			sipmsg.SetMsgBody(content.c_str(), content.length()+1);
			sipmsg.SetSipDlgID(m_tInviteID);
			cInviteRsp.content = (u8*)sipmsg.GetMsgBuf();
			cInviteRsp.length = sipmsg.GetMsgBodyLen();
			
			//����Ϣ�ص���InviteInterAckTask��
			if ( m_pcInviteInterActTask != NULL )
			{
				u32 dwRet = m_pcInviteInterActTask->ProcMsg(&cInviteRsp);
				if(PROCMSG_DEL == dwRet)
				{
					delete m_pcInviteInterActTask;
					m_pcInviteInterActTask = NULL;
				}
			}

			if ( INVALID_DIALOG_ID == m_tInviteID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ���InviteRsp����DialogIDΪ 0 !!!!ֱ������CInviteTask[TaskNO %u]\n",
					GetTaskNO());
				m_tInviteID = INVALID_DIALOG_ID;
				NextState(NOT_REQUEST_STATE);
				delete this;
				return PROCMSG_DEL;
			}

			if ( CMS_SUCCESS != cRsp.GetErrorCode() )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "Invite����ʧ�ܣ�������[%d]������CInviteTask[TaskNO: %u]������Sip Dialog: [%u]\n",
					cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);

				StartSendInviteByeNatSwitch();
				m_tInviteID = INVALID_DIALOG_ID;
				delete this;
				return PROCMSG_DEL;
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "Invite����ɹ�����CInviteTask[TaskNO: %u, PlayId: %d]�Ǽǵ�CCuAgent�������б���\n",
					GetTaskNO(), m_tReq.GetDstChn().GetChnNO());

				//����¼��ʱ��
				CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
				CCMSTime cEndTime(m_tReq.GetSdp().GetTimeRange().GetEndTime());

				m_tNtfReq.SetCurPlayTime((u32)cStartTime.GetTime());
				m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
				if(cEndTime.GetTime() >= cStartTime.GetTime())
				{
					m_tNtfReq.SetTotalPlayTime((u32)(cEndTime.GetTime() - cStartTime.GetTime()));
				}
				else
				{
					m_tNtfReq.SetTotalPlayTime(0);
				}
				m_tNtfReq.SetIsPlayFinished(false);

				NextState(WAIT_SETUP_ACK_STATE);
				StartSendInviteAckNatSwitch();
			}
        }
		break;
    case OSP_SIP_BYE_REQ:   //��ʵ���ﲻ���ܻ��յ�Bye����Ϊ���л�δ������Ҫô�ɹ�Ҫô�ܾ����յ�403������
		{
			if ( m_pcInviteInterActTask != NULL )
			{
				m_pcInviteInterActTask->ProcMsg(pcMsg);
			}

			if(pcOspSipMsg->GetMsgBodyLen() >0)
			{
				CByeReq cByeReq;
				pcOspSipMsg->GetMsgBody(cByeReq);
				NotifyStop2Cu(cByeReq.GetReason());
			}
			else
			{
				NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
			}

			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NOT_REQUEST_STATE);
			delete this;
			return PROCMSG_DEL;
			//CMS_ASSERT(0&&"��������WaitSetupRsp״̬���յ�OSP_SIP_BYE_REQ��Ϣ");
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			//����Ϣ�ص���InviteInterAckTask��
			if ( m_pcInviteInterActTask != NULL )
			{
				m_pcInviteInterActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "�յ�InviteRspʧ����Ϣ������CInviteTask[TaskNO: %u]\n",
				GetTaskNO());
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NOT_REQUEST_STATE);
			delete this;
			return PROCMSG_DEL;
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTaskNatSwitch::WaitSetupRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]�ȴ�InviteRsp��ʱ������CInviteTask[TaskNO: %u]\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO());

        /*
        ���ﲻ��֪ͨCInviteInterActionNatSwitch������ʱ
        ֻ��ȡ��m_pcCuAgent->m_cInviteTaskSet�еĵǼ�
        ��CInviteInterActionNatSwitch��Ϣ�Լ���ʱ����

        ��Ϊ��ʱ��δ�����Ự������Ҳ���跢��Bye���Լ�ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CInviteTaskNatSwitch::WaitSetupAckProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�InviteBye Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //����Ϣ�ص���InviteInterAckTask��
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //����Ϣ�ص���InviteInterAckTask��
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CInviteTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTaskNatSwitch::WaitAckTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]�ȴ�InviteAck��ʱ��ֱ�ӷ���InviteBye����Dlg\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		StartSendInviteByeNatSwitch();
		//return TIMERPOLL_DEL;
		return TIMERPOLL_DONE;  //����ByeReq֮���л���WaitByeRsp״̬�ˣ�����ɾ��
	}
	return TIMERPOLL_DONE;
}

u32 CInviteTaskNatSwitch::WaitByeRspProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
		OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
		pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
	u32 dwRet = PROCMSG_DEL;

	switch ( pcMsg->event )
	{
	case OSP_SIP_BYE_REQ:   
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�InviteBye Req��DialogID[%u]���ظ�Ӧ�������CInviteTask\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			//����Ϣ�ص���InviteInterAckTask��
			if ( m_pcInviteInterActTask != NULL )
			{
				m_pcInviteInterActTask->ProcMsg(pcMsg);
			}

			//             CInviteStopNotify cInviteStopNtf;
			//             cInviteStopNtf.playId = m_tCuChn.GetChnNO();
			//             cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
			//             m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

			string strNullMsg;
			PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
			//�Ѿ�ɾ����OspSip��Dialog�󶨣�����ֱ��ɾ��task
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	case OSP_SIP_BYE_RSP:
		{
			if ( m_pcInviteInterActTask != NULL )
			{
				m_pcInviteInterActTask->ProcMsg(pcMsg);
			}

			TASKLOG(NORMAL_TASK, EVENT_LEV, "OSP_SIP_BYE_RSP ����CInviteTask[TaskNO: %u]������Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			m_tInviteID = INVALID_DIALOG_ID;
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			//����Ϣ�ص���InviteInterAckTask��
			if ( m_pcInviteInterActTask != NULL )
			{
				m_pcInviteInterActTask->ProcMsg(pcMsg);
			}
			TASKLOG(NORMAL_TASK, ERROR_LEV, "����CInviteTask[TaskNO: %u]������Sip Dialog: [%u]\n",
				GetTaskNO(), m_tInviteID);
			//? ��鲻��ֱ������Ϊ��Ч
			//m_tInviteID = INVALID_DIALOG_ID;
			NextState(NOT_REQUEST_STATE);
			delete this;
		}
		break;
	case VCR_CTRL_RSP:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�VCR Ctrl Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
			if(pcTask != NULL)
			{
				pcTask->ProcMsg(pcMsg);
			}
			dwRet = PROCMSG_OK;
		}
		break;
	case PLAYBACK_PROGRESS_NTF_REQ:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�PlayBack Process Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
			CPlaybackProgressNtfReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			CPlaybackProgressNtfRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
				cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
			PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
			m_tNtfReq = cReq;
			CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
			m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
			dwRet =  PROCMSG_OK;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
			dwRet = PROCMSG_FAIL;
		}
		break;
	}
	return dwRet;
}

u32 CInviteTaskNatSwitch::WaitByeRspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]�ȴ�Bye Rsp��ʱ������Dialog\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		m_tInviteID = INVALID_DIALOG_ID;
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CInviteTaskNatSwitch::InviteHoldProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]�յ�ҵ��Ӧ����Ϣ[%s--%d] (SIPӦ����Ϣ[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
	case CUI_NATDETECT_CALLBACK:
		{
			ProcNatCBMsg(pcMsg);
		}
		break;
	case UPDATE_SWITCH_ADDRESS_RSP:
		{
			CUpdateSwitchAddressRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CInviteTaskExSwitch[TaskNO %u]���½�����ַʧ�ܣ�DialogID[%u],������[%d].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID(),cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTaskExSwitch[TaskNO %u]���½�����ַ�ɹ���DialogID[%u].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			}
		}
		break;
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTaskExSwitch[TaskNO %u]�յ�InviteBye Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //����Ϣ�ص���InviteInterAckTask��
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //����Ϣ�ص���InviteInterAckTask��
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "recv OSP_SIP_MSG_PROC_FAIL, sipErrorCode=%d, invite req:%s\n", 
                    pcOspSipMsg->GetSipErrorCode(), m_tReq.ToXml().c_str());
                StartSendInviteByeNatSwitch();
            }

            TASKLOG(NORMAL_TASK, ERROR_LEV, "����CInviteTask[TaskNO: %u]������Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NOT_REQUEST_STATE);

            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

			if (m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteByeNatSwitch();
			}
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�PlayBack Process Ntf Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());

			if (m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteByeNatSwitch();
			}
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]�յ�VCR Ctrl Req��DialogID[%u]���Զ��ظ�Ӧ��\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }

        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

s32 CInviteTaskNatSwitch::StartSendInviteReqNatSwitch(CInviteReq &cReq)
{
	m_tReq = cReq;//2018/8/22 zsy
    if( !m_cStrmNumRestorer.Start() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s] ���������������ƣ��������󱻾ܾ�\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str() );
        m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;
        return PROCMSG_FAIL;
    }

	CMS_ASSERT( NOT_REQUEST_STATE == GetState() );

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]����Invite���󣬽���Դͨ��[%s--%d]��Ŀ��ͨ��[%s--%d]�Ľ�����CUIת��\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
		cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO(),
		cReq.GetDstChn().GetDevUri().c_str(), cReq.GetDstChn().GetChnNO());

	//m_tReq = cReq;//2018/8/22 zsy ��Ҫ��ǰ����һ�����ܵ�return ֮ǰ������ǰ��return�ˣ��������ٱ������ʱ����޷��ͷ�invite�ˡ�
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU PlayBack Process Ntf Req--StartTime[%s]\n",
		cReq.GetSdp().GetTimeRange().GetStartTime().c_str());

	m_cuChn = cReq.GetSrcChn();

	// ƽ̨�ڲ�������ѡ��ģʽ,
	m_tReq.GetSdp().SetIsNeedPunch(false);
	m_tReq.GetSdp().SetIsNatPacketMode(false);

	//cu��NAT�ȣ����ôa����ַ����NAT���ַ
	SwitchToNatAddressByNatPacket(m_tReq.GetSdp());
	
	CInviteReq cuiReq = m_tReq;

	TNetAddr tCuiNetAddr;
	this->GetCuiNatAddrForPlat(tCuiNetAddr);
	// ����ʱ����Դ��ַ�ĳ�CUI������ַ
	SwitchToCuiNatSwitchAddress(cuiReq.GetSdp(), tCuiNetAddr);

	// ���û����Ƶ�˿���������Ƶ�˿�
	AddAudioNatSwitchAddressForPlat(cuiReq.GetSdp());

#ifdef _UNIT_TEST_
	g_cTaskNOQue.push(GetTaskNO());
	cout << "Post InviteReq To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
	cout << cReq.ToXml().c_str() << endl;
#else
	PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, cuiReq, m_pcCuAgent->m_tCmuURI);
#endif

	NextState(WAIT_SETUP_RSP_STATE);
	return 0;
}

s32 CInviteTaskNatSwitch::StartSendInviteAckNatSwitch()
{
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTaskNatSwitch::StartSendInviteAck CU�û�[%s]�Ự[%s]ͨ��TaskNO[%u]����InviteACK����, Sip DialogIDΪ %u\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

    CInviteACK cAck;
    cAck.SetSession(m_pcCuAgent->m_strSession);

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteAck To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cAck.ToXml().c_str() << endl;
#else
    PostInDlgReq<CInviteACK>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
#endif

    NextState(INVITE_HOLD_STATE);
    return 0;
}

s32 CInviteTaskNatSwitch::StartSendInviteByeNatSwitch()
{
	if ( m_pcCuAgent == NULL )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV,"m_pcCuAgent == NULL\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;
    if ( m_tInviteID != INVALID_DIALOG_ID )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV,"CU�û�[%s]�Ự[%s]ͨ��TaskNO[%u]����InviteBye����, Sip DialogIDΪ %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

        string strNULL;
		dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
    }

    NextState(WAIT_BYE_RSP_STATE);
    //BYE Rsp�ɶԶ�SIPЭ��ջֱ�ӻظ����ȴ��ظ���delete task
    return dwRet;
}

bool CInviteTaskNatSwitch::TryBye(CSipTask *pcActTask)
{
    if (GetState() != WAIT_SETUP_RSP_STATE)
    {
		// �Ȳ𽻻�
		this->RemoveCuiSwitch();

		m_pcInviteInterActTask = pcActTask;
		StartSendInviteByeNatSwitch();
		return true;
    }
    else
    {
        m_bByeWhileWaitInviteRsp = true;
        return false;
    }
}

s32 CInviteTaskNatSwitch::StartVcrCtrl(const CCuVcrCtrlReq&cCuReq)
{
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]ͨ��TaskNO[%u]����VcrCtrl����, Sip DialogIDΪ %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

        CVcrCtrlReq cReq;
        cReq.SetSeqNum(cCuReq.GetSeqNum());
        cReq.SetSession(m_pcCuAgent->m_strSession);
        cReq.SetPlaybackSessUri(m_tRsp.GetSdp().GetPlaybackSessUri());
        cReq.SetPlayRate(cCuReq.GetPlayRate());
        cReq.SetSeekTime(cCuReq.GetSeekTime());
        cReq.SetVcrCmd(cCuReq.GetVcrCmd());
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
    }

    return PROCMSG_OK;
}
CPlaybackProgressNtfReq &CInviteTaskNatSwitch::GetPlayBackProgressNtf()
{
    return m_tNtfReq;
}
const CPlaybackProgressNtfReq &CInviteTaskNatSwitch::GetPlayBackProgressNtf() const
{
    return m_tNtfReq;
}

void CInviteTaskNatSwitch::NotifyStop2Cu(s32 dwErrorCode)
{
    CInviteStopNotify cInviteStopNtf;
    cInviteStopNtf.playId = m_tReq.GetDstChn().GetChnNO();
    cInviteStopNtf.errorCode = dwErrorCode;
    m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
}

void CInviteTaskNatSwitch::SwitchToNatAddressByNatPacket(TSdp &tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if ((itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				|| (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP))
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý������[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}
}

bool CInviteTaskNatSwitch::SwitchToCuiNatSwitchAddress(TSdp &tSdp, const TNetAddr &tCuiNatAddr)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
	
	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
				|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					*itrNatAddr = tCuiNatAddr;
				}
				itrTransChn->SetMediaChannelKey("");
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý������[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}
	return true;
}

bool CInviteTaskNatSwitch::IsPlayStreamInvite(TSdp &tSdp)
{
	if (tSdp.GetSessionName() == SDP_SESSION_PLAY
		/*|| tSdp.GetSessionName() == SDP_SESSION_PLAYBACK*/)
	{
		return true;
	}

	return false;
}

bool CInviteTaskNatSwitch::HasAudioNatSwitchAddress(TSdp &tSdp)
{
	if ( !IsPlayStreamInvite(tSdp) )
	{
		// ֻ�����������Ƶ
		return false;
	}

	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if ( itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			return true;
		}
	}

	return false;
}

void CInviteTaskNatSwitch::AddAudioNatSwitchAddressForPlat(TSdp &tSdp)
{
	if ( !IsPlayStreamInvite(tSdp) )
	{
		// ֻ�����������Ƶ,��һ·û����Ƶ�����ܺ��治������Ƶ
		return ;
	}

	bool bHasAudio = this->HasAudioNatSwitchAddress(tSdp);

	// û����Ƶ��������Ƶ
	if ( !bHasAudio )
	{
		TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

		TMediaDescList::iterator itrVideoMediaDesc = tMediaDescList.begin();
		for(; itrVideoMediaDesc!=tMediaDescList.end(); itrVideoMediaDesc++)
		{
			if ( itrVideoMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO )
			{
				break;
			}
		}

		if ( itrVideoMediaDesc != tMediaDescList.end() )
		{
			TMediaDesc audioMediaDesc;
			audioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);

			TMediaFormat &videoFormat = itrVideoMediaDesc->GetMediaFormat();
			TMediaFormat audioFormat;
			audioFormat.SetEncodingName(videoFormat.GetEncodingName());
			audioFormat.SetPayloadType(0);
			audioFormat.SetManufacture(videoFormat.GetManufacture());

			audioMediaDesc.SetMediaFormat(audioFormat);

			audioMediaDesc.SetMediaTransProto(itrVideoMediaDesc->GetMediaTransProto());

			// CUIת�������£���Ƶ����Ƶ����ͬһ��IP���˿�
			audioMediaDesc.SetMediaTransChnList(itrVideoMediaDesc->GetMediaTransChnList());

			// �����Ƶ�˿�
			tMediaDescList.push_back(audioMediaDesc);
		}
	}
}

void CInviteTaskNatSwitch::DelAudioNatSwitchAddress(TSdp &tSdp)
{
	if ( !IsPlayStreamInvite(tSdp) )
	{
		// ֻ�����ɾ����Ƶ
		return ;
	}

	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if ( itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			tMediaDescList.erase(itrMediaDesc);
		}
	}
}

//NAT̽����ص���ַ�������ַ�б仯��Ҫ֪ͨVTDU ���Ľ�����ַ
void CInviteTaskNatSwitch::ProcNatCBMsg(CMessage *const pcMsg)
{
	if ( pcMsg == NULL || pcMsg->content == NULL )
	{
		return;
	}
	
	TNatPacketInfo tNatPacket = *(reinterpret_cast<TNatPacketInfo*>(pcMsg->content));

	TMediaDescList& tMediaDescList = m_tReq.GetSdp().GetMediaDescList();

	bool bChnAddrChanged = false;

	//nat̽�����reserve��Ч�����ϵ��滻���̣�Դ��ַ�ͽ�����ַƥ����滻nat��ַ��
	//reserve��Ч���ߣ�sdk���µ��滻���̣�nat��ַ�͵�ǰ������ַ��ƥ����滻nat��ַ��
	TASKLOG(NORMAL_TASK, EVENT_LEV, "Reserver[%d]\n", tNatPacket.m_byReserve);
	if (tNatPacket.IsReserveInvalid())//������
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if ( itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
					|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP )
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
						{
							itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
							itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);

							bChnAddrChanged = true;
						}
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý������[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
		}
	} 
	else//������
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (MEDIA_TYPE_VIDEO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_AUDIO == itrMediaDesc->GetMediaType() 
				&& (NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_DATA == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_DATA == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
								bChnAddrChanged = true;
							}
						}
					}
				}
			}
		}
	}	

	if ( bChnAddrChanged )
	{
		this->RemoveCuiSwitch();
		this->GenSwitchChn();
		this->AddCuiSwitch();
	}
}

bool CInviteTaskNatSwitch::CuiAddNatSwitch()
{
	if (!this->GenSwitchChn())
	{
		return false;
	}

	return AddCuiSwitch();
}

bool CInviteTaskNatSwitch::GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr)
{
	if ( m_pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();

	string strPlatCuiIp = m_pcCuAgent->m_tNatInfo.GetIpAddr();
	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "m_tNatInfo.GetIpAddr() is empty!\n");
		return false;
	}
	
	int nPlatCuiPort = g_cCuiConfig.GetNatPort();
	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "g_cCuiConfig.GetNatPort() is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForPlat[%s:%d]!\n", strPlatCuiIp.c_str(), nPlatCuiPort);
	return true;
}

bool CInviteTaskNatSwitch::GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr)
{
	if ( m_pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();

	string strPlatCuiIp;
	int nPlatCuiPort = 0;

	if ( NATModeInner == m_pcCuAgent->m_ePlatNat && !g_cCuiConfig.GetMapedNatIP().empty())
	{
		//cCuPunchAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		//�����ж���NATʱ��ƽ̨����ͬʱ��һ���˿ڣ�8001��ӳ�䵽��һ��͵ڶ���
		//����ֱ��ʹ��cu�ĵ�½ip����Ϊƽ̨�ڵ������ӳ��IP
		strPlatCuiIp = m_pcCuAgent->m_strHostIp;
		if (m_pcCuAgent->m_tNatInfo.GetIpAddr() != m_pcCuAgent->m_strHostIp)
		{
			nPlatCuiPort = g_cCuiConfig.GetMapedNatPort();//��Ҫ�ж�hostip�Ƿ���Ǳ���ip�����Ǳ���ip������ʹ��natport��
		}
		else
		{
			nPlatCuiPort = g_cCuiConfig.GetNatPort();
		}
	}
	else
	{
		strPlatCuiIp = m_pcCuAgent->m_tNatInfo.GetIpAddr();
		nPlatCuiPort = g_cCuiConfig.GetNatPort();
	}

	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "m_tNatInfo.GetIpAddr() is empty!\n");
		return false;
	}

	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "g_cCuiConfig.GetNatPort() is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForCu[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}

bool CInviteTaskNatSwitch::GenSwitchChn()
{
	//һ���������ֻ��һ������Sess�ţ���Sess�ű���������ͨ������
	m_tSwitchSessID = GetTaskNO();

	TNetAddr tCuiNatAddr;
	if ( !this->GetCuiNatAddrForPlat(tCuiNatAddr) )
	{
		return false;
	}

	TMediaDescList& tDstCuMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstCuMediaTransDescList.size(); i++)
	{
		TMediaDesc& tCuMediaDesc = tDstCuMediaTransDescList[i];

		TMediaTransChannelList& tCuMediaChnList = tCuMediaDesc.GetMediaTransChnList();
		for (size_t nChnIdx=0; nChnIdx<tCuMediaChnList.size(); nChnIdx++)
		{
			TMediaTransChannel& tCuMediaChn = tCuMediaChnList[nChnIdx];
			TSwitchChannel tSwitchChn;
			tSwitchChn.SetSwitchSsnID(m_tSwitchSessID);
			tSwitchChn.SetMediaType(tCuMediaDesc.GetMediaType());
			tSwitchChn.SetMediaFormat(tCuMediaDesc.GetMediaFormat());
			tSwitchChn.SetMediaTransProto(tCuMediaDesc.GetMediaTransProto());
			tSwitchChn.SetSwitchType(SWITCH_TYPE_M2P);
			tSwitchChn.SetTransChnType(tCuMediaChn.GetTransChnType());
			tSwitchChn.SetGapSwitch(false);
			
			tSwitchChn.SetRcvNetAddr(tCuiNatAddr);
			if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_VIDEO)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                
					
					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduVideoRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					//tSwitchChn.SetSrcMediaChnKey(this->GetVtduVideoRtpChnKey());

					//tSwitchChn.SetSrcDevChn(m_puChn);
					tSwitchChn.SetSrcDevChn(m_vtduChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tVideoRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduVideoRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);
					//tSwitchChn.SetDstMediaChnKey(this->GetVtduVideoRtcpChnKey());

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_vtduChn);
					//tSwitchChn.SetDstDevChn(m_puChn);

					m_tVideoRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý��ͨ������[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}              
			}
			else if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_AUDIO)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					//audio rtp ͨ��

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduAudioRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					//tSwitchChn.SetSrcMediaChnKey(this->GetVtduAudioRtpChnKey());

					//tSwitchChn.SetSrcDevChn(m_puChn);
					tSwitchChn.SetSrcDevChn(m_vtduChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tAudioRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					//audio rtcp ͨ��

					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduAudioRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);
					//tSwitchChn.SetDstMediaChnKey(this->GetVtduAudioRtcpChnKey());

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_vtduChn);
					//tSwitchChn.SetDstDevChn(m_puChn);

					m_tAudioRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý��ͨ������[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}  
			}
			else if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_DATA)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					//audio rtp ͨ��

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduDataRtpChn());
					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					//tSwitchChn.SetSrcMediaChnKey(this->GetVtduAudioRtpChnKey());

					//tSwitchChn.SetSrcDevChn(m_puChn);
					tSwitchChn.SetSrcDevChn(m_vtduChn);
					tSwitchChn.SetDstDevChn(m_cuChn);

					m_tDataRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					//audio rtcp ͨ��

					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduDataRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tSrcVtduNetAddrList);
					//tSwitchChn.SetDstMediaChnKey(this->GetVtduAudioRtcpChnKey());

					tSwitchChn.SetSrcDevChn(m_cuChn);
					tSwitchChn.SetDstDevChn(m_vtduChn);
					//tSwitchChn.SetDstDevChn(m_puChn);

					m_tDataRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý��ͨ������[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}  
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "δ֪ý������[%s]\n", tCuMediaDesc.GetMediaType().c_str());
			}
		}
	}

	return true;
}

bool CInviteTaskNatSwitch::IsSendVideo()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_VIDEO)
		{
			return true;
		}
	}
	return false;
}

bool CInviteTaskNatSwitch::IsSendAudio()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			return true;
		}
	}
	return false;
}

bool CInviteTaskNatSwitch::IsSendData()
{
	const TMediaDescList& tDstMediaTransDescList = m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tDstMediaTransDescList.size(); i++)
	{
		const TMediaDesc& tMediaDesc = tDstMediaTransDescList[i];
		if (tMediaDesc.GetMediaType() == MEDIA_TYPE_DATA)
		{
			return true;
		}
	}
	return false;
}

bool CInviteTaskNatSwitch::AddCuiSwitch()
{
	if ( IsSendData() )
	{
		if (AddSwitch(m_tDataRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add data rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tDataRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add data rtcp switch fail\n");
			return false;
		}
		// ������ͬʱ�������ݺ�����Ƶ����
		return  true;
	}

	if (IsSendVideo())
	{
		if (AddSwitch(m_tVideoRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tVideoRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add video rtcp switch fail\n");
			return false;
		}     
	}

	if (IsSendAudio())
	{
		if (AddSwitch(m_tAudioRtpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtp switch fail\n");
			return false;
		}

		if (AddSwitch(m_tAudioRtcpChn) != CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtcp switch fail\n");
			return false;
		}  
	}

	return true;
}

void CInviteTaskNatSwitch::RemoveCuiSwitch()
{
	if (m_tSwitchSessID != 0)
	{
		if (RemoveSwitch(m_tSwitchSessID) == CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, WARNING_LEV,"ɾ������[m_tSwitchSessID = %u]�ɹ�\n", u32(m_tSwitchSessID));
			m_tSwitchSessID = 0;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "ɾ������[m_tSwitchSessID = %u]ʧ��\n", u32(m_tSwitchSessID));
		}
	}
	else
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "m_tSwitchSessID == 0\n");
	}
}

TIpAddr CInviteTaskNatSwitch::GetVtduIp()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO
			|| descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO
			|| descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if ( chnList[iTransChn].GetAddrList().size() > 0 )
				{
					return chnList[iTransChn].GetAddrList()[0].GetNetIp();
				}
			}
		}
	}
	return "";
}

s32 CInviteTaskNatSwitch::GetVtduVideoRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CInviteTaskNatSwitch::GetVtduVideoRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_VIDEO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CInviteTaskNatSwitch::GetVtduAudioRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CInviteTaskNatSwitch::GetVtduAudioRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CInviteTaskNatSwitch::GetVtduDataRtpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CInviteTaskNatSwitch::GetVtduDataRtcpChn()
{
	TSdp &tsdp = m_tRsp.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_DATA )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

TChannel& CInviteTaskNatSwitch::GetDevChn()
{
    return m_tReq.GetSrcChn();
}

void CInviteTaskNatSwitch::AddChildTask(int nSeqNo, int nTaskNo)
{
    if(nTaskNo == INVALID_TASKNO)
    {
        TASKLOG(ERROR_LEV, "TaskNo is Invalid");
        return;
    }
    m_cChildTaskList.Insert(nSeqNo, nTaskNo);
}
void CInviteTaskNatSwitch::DelChildTask(int nSeqNo)
{
    m_cChildTaskList.Erase(nSeqNo);
}
CTask* CInviteTaskNatSwitch::FindChildTask(int nSeqNo)
{
    int nTaskNo = INVALID_TASKNO;
    m_cChildTaskList.Find(nSeqNo, nTaskNo);
    return GetInstance()->FindTask(nTaskNo);
}

//CInviteTaskNatSwitch
//////////////////////////////////////////////////////////////////////////
//CCallPuTaskNatSwitch

CCallPuTaskNatSwitch::CCallPuTaskNatSwitch(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask) : CSipTask(pcInst)//, m_cStrmNumRestorer(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(NULL != pcTask);

	m_pcCallPuInterActTask = pcTask;
	m_strCuSession = strCuSession;
	m_strCuCallPuReq = strCallPuReq;
	m_tDialogID = INVALID_DIALOG_ID;

    //m_cStrmNumRestorer.SetUserLoginSession( strCuSession );
    m_dwErrorCode = CMS_SUCCESS;
}

CCallPuTaskNatSwitch::~CCallPuTaskNatSwitch()
{
	TerminateTask();
	//m_tPuChnIDList.clear();
    if (!m_strModuleId.empty())
	{
		CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
	}
}

void CCallPuTaskNatSwitch::TerminateTask()
{
	RemoveCuiSwitch();

	if ( m_tDialogID != INVALID_DIALOG_ID )
	{ //�ͷŻỰ
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tDialogID);
		//OspSipReleaseDlg(m_tDialogID);
		m_tDialogID = INVALID_DIALOG_ID;
	}

	//ע��CuAgent�е��������
	if ( INVALID_PLAY_ID != m_tCuChn.GetChnNO() )
	{
		//m_pcCuAgent->m_cInviteTaskSet.Erase(m_tCuChn.GetChnNO());
		CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
		//������CuAgent������Invite�ȴ�bye��Ӧʱ��CuAgent�Ѿ����ɵ��ˣ�����ȥ��CMS_ASSERT
		//CMS_ASSERT(pcCuAgent != NULL);
		if(NULL != pcCuAgent)
		{
			pcCuAgent->m_cCallPuTaskSet.Erase(m_tCuChn.GetChnNO());
		}

		PopMediaTask(m_strCuSession,m_tCuChn.GetChnNO());

		m_tCuChn.SetChnNO(INVALID_PLAY_ID);
	}

	m_pcCallPuInterActTask = NULL;
}

bool CCallPuTaskNatSwitch::IsNotNeedPollState()
{
	if (GetState() == INVITE_HOLD_STATE)
	{
		return true;
	}

	return false;
}

void CCallPuTaskNatSwitch::InitStateMachine()
{
	CStateProc cWaitSetUpReqProc;
	cWaitSetUpReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::WaitSetUpReqProcMsg;
	cWaitSetUpReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTaskNatSwitch::WaitSetUpReqTimerPoll;
	cWaitSetUpReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::WaitSetUpReqProcMsg;
	AddRuleProc(WAIT_SETUP_REQ_STATE, cWaitSetUpReqProc);

	CStateProc cWaitAckState;
	cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::WaitAckProcMsg;
	cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTaskNatSwitch::WaitAckTimerPoll;
	cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::WaitAckProcMsg;
	AddRuleProc(WAIT_ACK_STATE, cWaitAckState);

	CStateProc cInviteHoldState;
	cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::InviteHoldProcMsg;
	cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
	cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskNatSwitch::InviteHoldProcMsg;
	AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

	NextState(WAIT_SETUP_REQ_STATE);
}

u32 CCallPuTaskNatSwitch::WaitSetUpReqProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskNatSwitch�յ�CU�û��Ự[%s]������[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;

	switch(pcMsg->event)
	{
	case INVITE_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CMS_ASSERT(pcOspSipMsg && "ospsip ��ϢΪ��\n");

			CInviteReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			m_strInviteReq = cReq.ToXml();

			m_vtduChn = cReq.GetSdp().GetOwner();

			m_tDialogID = pcOspSipMsg->GetSipDlgID();
			m_tTransId = pcOspSipMsg->GetSipTransID();
			//1 ��Ȩ
			bool bAuthRet = true; //��Ȩ���
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if(NULL == pcCuAgent)
			{
				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "reply ERR_CUI_CU_NOT_ONLINE\n");
				//delete this;
				return PROCMSG_DEL;
			}

    //        if( !m_cStrmNumRestorer.Start() )
    //        {
    //            TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s] �Ѵﵽ��󲢷������������󱻾ܾ�\n", m_strCuSession.c_str() );
    //            m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;

    //            CInviteRsp cRsp;
				//cRsp.SetErrorCode(ERR_CUI_USER_EXCEEDED_MAX_STREAM);
				//cRsp.SetSeqNum(cReq.GetSeqNum());
				//cRsp.SetSession(cReq.GetSession());
    //            
    //            PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
    //            return PROCMSG_DEL;
    //        }

			do 
			{                             
				string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetSrcChn().GetDevUri());
				if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}
				
				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
				if(!cPri.IsEmpty())
				{
					if(cPri.HasPri(CUserPrivilege::en_Call_Ctrl))
					{
						bAuthRet = true;
						break;
					}
					else
					{
						bAuthRet = false;
					}
				}
				else
				{
					bAuthRet = false;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(cReq.GetDstChn().GetDevUri());
				if ( !g_cRedisDb.IsUserHasDev(pcCuAgent->m_pcUserAccount->m_tUserID, uuid) )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "�û�[%s]û���豸[%s]\n", pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str());
					bAuthRet = false;
					break;
				}
			} while (0);

			if (!bAuthRet)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]����Invite���󣬵��Ƕ�Դͨ��[%s--%d]��Ȩ��\n",
					m_strCuSession.c_str(),
					cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				//delete this;
				return PROCMSG_DEL;
			}

			if(INVALID_TRANS_ID ==  m_tDialogID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]����Invite���󣬵���TransID��Ч,DevUri[%s],SrcChnNo[%d]\n",
					m_strCuSession.c_str(),
					cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				//delete this;
				return PROCMSG_DEL;
				
			}
			
			CCallPuInterAction *pcCallPuInterActTask= (CCallPuInterAction *)m_pcCallPuInterActTask;
			CMS_ASSERT(NULL != pcCallPuInterActTask);
			if(pcCallPuInterActTask != NULL)
			{
				CCuStartThirdPartyCallReq cCuStartThirdPartyCallReq;
				cCuStartThirdPartyCallReq.FromXml(pcCallPuInterActTask->m_strSrcReq);

				m_tCuChn = cCuStartThirdPartyCallReq.GetSrcChn();               

				CCuStartThirdPartyCallReq cCuStartThirdPartyCallReqNatSwitch;
				cCuStartThirdPartyCallReqNatSwitch.FromXml(pcCallPuInterActTask->m_strSrcReqNatSwitch);

				CInviteRsp cRsp;

				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());
				cRsp.GetSdp().SetOwner(m_tCuChn);
				cRsp.GetSdp().SetSessionName(cReq.GetSdp().GetSessionName());
				cRsp.GetSdp().SetMediaDescList(cCuStartThirdPartyCallReqNatSwitch.GetSdp().GetMediaDescList());

				cRsp.GetSdp().SetIsNatPacketMode(false);
				cRsp.GetSdp().SetIsNeedPunch(false);

				m_vtduChn = cReq.GetSdp().GetOwner();

				// Դ VTDU
				TSdp srcSdp = cCuStartThirdPartyCallReqNatSwitch.GetSdp();

				// Ŀ�� vtdu
				TSdp dstSdp = cReq.GetSdp();

				//���ɽ���ͨ��
				if ( !GenSwitchChn() )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��������ͨ��ʧ��\n");
					dwRet = PROCMSG_DEL;
				}

				//��pu��vtdu֮�佻��
				if (dwRet != PROCMSG_OK || !AddCuiSwitch())
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��ӽ���ʧ��\n");
					dwRet = PROCMSG_DEL;
				}

				if ( dwRet != PROCMSG_OK )
				{
					RemoveCuiSwitch();
					cRsp.SetErrorCode(ERR_CUI_ADD_SWITCH_FAILED);
				}

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, m_tTransId/*pcOspSipMsg->GetSipTransID()*/, cRsp);
			}
			else
			{
				CInviteRsp cRsp;

				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());
				cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, m_tTransId/*pcOspSipMsg->GetSipTransID()*/, cRsp);

				TASKLOG(NORMAL_TASK, ERROR_LEV,"pcCallPuInterActTask == NULL!\n");
				dwRet = PROCMSG_DEL;
			}

            m_strModuleId = pcOspSipMsg->GetSipFromUri().GetUser();
            CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		    CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());

			NextState(WAIT_ACK_STATE);
		}
		break;
	default:
		{
			dwRet = PROCMSG_FAIL;
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	}
	return dwRet;
}

u32 CCallPuTaskNatSwitch::WaitSetUpReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Invite����ʱ\n",
			m_strCuSession.c_str());

		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCallPuTaskNatSwitch::WaitAckProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskNatSwitch::WaitAckProcMsg �յ�CU�û��Ự[%s]����Ϣ[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;

	switch(pcMsg->event)
	{
	case INVITE_ACK:
		{
			//COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			//if (NULL != pcOspSipMsg)
			//{
			//	CInviteACK cAck;
			//	pcOspSipMsg->GetMsgBody(cAck);

			//	CInviteReq cReq;
			//	cReq.FromXml(m_strInviteReq);
			//	cReq.SetSdp(cAck.GetSdp());

			//	m_strInviteReq = cReq.ToXml();
			//}
			//ACK��ʧ�ܵ�ʱ��,ֱ��״̬�л�
			NextState(INVITE_HOLD_STATE);
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]�յ�OSP_SIP_MGS_PROC_FAIL��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();
				cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

				string strNullMsg;
				PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	default:
		{
			dwRet = PROCMSG_FAIL;
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CCallPuTaskNatSwitch::WaitAckTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU�û��Ự[%s]�ȴ�Invite����ʱ\n",
			m_strCuSession.c_str());

		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCallPuTaskNatSwitch::InviteHoldProcMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskNatSwitch::InviteHoldProcMsg�յ�CU�û��Ự[%s]������[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;
	switch ( pcMsg->event )
	{
	case CUI_NATDETECT_CALLBACK:
		{
			ProcNatCBMsg(pcMsg);
		}
		break;
	case UPDATE_SWITCH_ADDRESS_RSP:
		{
			CUpdateSwitchAddressRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CCallPuTaskNatSwitch[TaskNO %u]���½�����ַʧ�ܣ�DialogID[%u],������[%d].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID(),cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskNatSwitch[TaskNO %u]���½�����ַ�ɹ���DialogID[%u].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			}
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]�յ�OSP_SIP_MGS_PROC_FAIL��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();
				cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
				string strNullMsg;

				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]�յ�OSP_SIP_MGS_PROC_FAIL��DialogID[%u]���Զ��ظ�Ӧ��\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{                
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();

				if(pcOspSipMsg->GetMsgBodyLen() > 0)
				{
					CByeReq cByeReq;
					pcOspSipMsg->GetMsgBody(cByeReq);

					cInviteStopNtf.errorCode = cByeReq.GetReason();
				}
				else
				{
					cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
				}

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
				string strNullMsg;

				PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "CCallPuTaskNatSwitch::InviteHoldProcMsg �յ�δ֪��Ϣ[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
		}
		break;
	}
	return dwRet;
}

//NAT̽����ص���ַ�������ַ�б仯��Ҫ֪ͨVTDU ���Ľ�����ַ
void CCallPuTaskNatSwitch::ProcNatCBMsg(CMessage *const pcMsg)
{
	CCuStartThirdPartyCallReq tReq;
	tReq.FromXml(m_strCuCallPuReq);

	TNatPacketInfo tNatPacket = *(reinterpret_cast<TNatPacketInfo*>(pcMsg->content));
	TMediaDescList& tMediaDescList = tReq.GetSdp().GetMediaDescList();

	bool bChnAddrChanged = false;
	//nat̽�����reserve��Ч�����ϵ��滻���̣�Դ��ַ�ͽ�����ַƥ����滻nat��ַ��
	//reserve��Ч���ߣ�sdk���µ��滻���̣�nat��ַ�͵�ǰ������ַ��ƥ����滻nat��ַ��
	TASKLOG(NORMAL_TASK, EVENT_LEV, "Reserver[%d]\n", tNatPacket.m_byReserve);
	if (tNatPacket.IsReserveInvalid())//������
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
						|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
							{
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);

								bChnAddrChanged = true;
							}
						}
					}
					else
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "��Ƶ���в�֧�ֵ�ý������[%s]\n",
							itrTransChn->GetTransChnType().c_str());
					}
				}
			}
		}
	}
	else//������
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (MEDIA_TYPE_AUDIO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());

								bChnAddrChanged = true;
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());

								bChnAddrChanged = true;
							}
						}
					}
				}
			}
		}
	}
	
	if ( bChnAddrChanged )
	{
		m_strCuCallPuReq = tReq.ToXml();
		this->RemoveCuiSwitch();
		this->GenSwitchChn();
		this->AddCuiSwitch();
	}
}

bool CCallPuTaskNatSwitch::GenSwitchChn()
{
	// Դ CU
	CCuStartThirdPartyCallReq puCallReq;
	puCallReq.FromXml(m_strCuCallPuReq);
	TSdp srcSdp = puCallReq.GetSdp();

	TNetAddr tCuiNatAddr;
	if (!GetCuiNatAddrForPlat(tCuiNatAddr))
	{
		return false;
	}

	// Ŀ�� VTDU
	CInviteReq inviteReq;
	inviteReq.FromXml(m_strInviteReq);
	TSdp dstSdp = inviteReq.GetSdp();
	
	//һ���������ֻ��һ������Sess�ţ���Sess�ű���������ͨ������
	m_tSwitchSessID = GetTaskNO();

	TMediaDescList& tCuMediaTransDescList = srcSdp.GetMediaDescList();//m_tReq.GetSdp().GetMediaDescList();

	for(size_t i=0; i<tCuMediaTransDescList.size(); i++)
	{
		TMediaDesc& tCuMediaDesc = tCuMediaTransDescList[i];

		TMediaTransChannelList& tCuMediaChnList = tCuMediaDesc.GetMediaTransChnList();

		for (size_t nChnIdx=0; nChnIdx<tCuMediaChnList.size(); nChnIdx++)
		{
			TMediaTransChannel& tCuMediaChn = tCuMediaChnList[nChnIdx];
			TSwitchChannel tSwitchChn;
			tSwitchChn.SetSwitchSsnID(m_tSwitchSessID);
			tSwitchChn.SetMediaType(tCuMediaDesc.GetMediaType());
			tSwitchChn.SetMediaFormat(tCuMediaDesc.GetMediaFormat());
			tSwitchChn.SetMediaTransProto(tCuMediaDesc.GetMediaTransProto());
			tSwitchChn.SetSwitchType(SWITCH_TYPE_M2P);
			tSwitchChn.SetTransChnType(tCuMediaChn.GetTransChnType());
			tSwitchChn.SetGapSwitch(false);

			tSwitchChn.SetRcvNetAddr(tCuiNatAddr);
			if (tCuMediaDesc.GetMediaType() == MEDIA_TYPE_AUDIO)
			{
				if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					//audio rtp ͨ��
					tSwitchChn.SetSrcNetAddrList(tCuMediaChn.GetAddrList());                

					TNetAddrList tDstVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduAudioRtpChn());
					tDstVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetDstNetAddrList(tDstVtduNetAddrList);

					tSwitchChn.SetDstDevChn(m_vtduChn);
					tSwitchChn.SetSrcDevChn(m_cuChn);

					m_tAudioRtpChn = tSwitchChn;
				}
				else if (tCuMediaChn.GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					//audio rtcp ͨ��

					tSwitchChn.SetDstNetAddrList(tCuMediaChn.GetAddrList());   

					TNetAddrList tSrcVtduNetAddrList;
					TNetAddr tVtduNetAddr;
					tVtduNetAddr.SetNetIp(this->GetVtduIp());
					tVtduNetAddr.SetNetPort(this->GetVtduAudioRtcpChn());

					tSrcVtduNetAddrList.push_back(tVtduNetAddr);
					tSwitchChn.SetSrcNetAddrList(tSrcVtduNetAddrList);

					tSwitchChn.SetDstDevChn(m_cuChn);
					tSwitchChn.SetSrcDevChn(m_vtduChn);

					m_tAudioRtcpChn = tSwitchChn;
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "��֧�ֵ�ý��ͨ������[%s]\n",
						tCuMediaChn.GetTransChnType().c_str());
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "δ֪ý������[%s]\n", tCuMediaDesc.GetMediaType().c_str());
			}
		}
	}
	return true;
}

bool CCallPuTaskNatSwitch::AddCuiSwitch()
{
	if (AddSwitch(m_tAudioRtpChn) != CMS_SUCCESS)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtp switch fail\n");
		return false;
	}

	if (AddSwitch(m_tAudioRtcpChn) != CMS_SUCCESS)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "add audio rtcp switch fail\n");
		return false;
	}

	return true;
}

void CCallPuTaskNatSwitch::RemoveCuiSwitch()
{
	if (m_tSwitchSessID != 0)
	{
		if (RemoveSwitch(m_tSwitchSessID) == CMS_SUCCESS)
		{
			TASKLOG(NORMAL_TASK, WARNING_LEV,"ɾ������[m_tSwitchSessID = %u]�ɹ�\n", u32(m_tSwitchSessID));
			m_tSwitchSessID = 0;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "ɾ������[m_tSwitchSessID = %u]ʧ��\n", u32(m_tSwitchSessID));
		}

	}
	else
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "m_tSwitchSessID == 0\n");
	}
}

TIpAddr CCallPuTaskNatSwitch::GetVtduIp()
{
	CInviteReq cReq;
	cReq.FromXml(m_strInviteReq);

	TSdp &tsdp = cReq.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if ( chnList[iTransChn].GetAddrList().size() > 0 )
				{
					return chnList[iTransChn].GetAddrList()[0].GetNetIp();
				}
			}
		}
	}
	return "";
}

s32 CCallPuTaskNatSwitch::GetVtduAudioRtpChn()
{
	CInviteReq cReq;
	cReq.FromXml(m_strInviteReq);

	TSdp &tsdp = cReq.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

s32 CCallPuTaskNatSwitch::GetVtduAudioRtcpChn()
{
	CInviteReq cReq;
	cReq.FromXml(m_strInviteReq);

	TSdp &tsdp = cReq.GetSdp();
	TMediaDescList &descList = tsdp.GetMediaDescList();
	for ( int iDesc = 0; iDesc < descList.size(); iDesc++)
	{
		if ( descList[iDesc].GetMediaType() == MEDIA_TYPE_AUDIO )
		{
			TMediaTransChannelList &chnList = descList[iDesc].GetMediaTransChnList();
			for ( int iTransChn = 0; iTransChn < chnList.size(); iTransChn++ )
			{
				if (chnList[iTransChn].GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					if ( chnList[iTransChn].GetAddrList().size() > 0 )
					{
						return chnList[iTransChn].GetAddrList()[0].GetNetPort();
					}
				}
			}
		}
	}
	return 0;
}

bool CCallPuTaskNatSwitch::GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr)
{
	CCuAgent* pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();
	string strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();

	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "ServiceIp is empty!\n");
		return false;
	}

	int nPlatCuiPort = g_cCuiConfig.GetNatPort();
	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CuiNatPort is empty!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForPlat[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}

bool CCallPuTaskNatSwitch::GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr)
{
	CCuAgent* pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();

	string strPlatCuiIp;
	int nPlatCuiPort = 0;

	if ( NATModeInner == pcCuAgent->m_ePlatNat && !g_cCuiConfig.GetMapedNatIP().empty())
	{
		//cCuPunchAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		//�����ж���NATʱ��ƽ̨����ͬʱ��һ���˿ڣ�8001��ӳ�䵽��һ��͵ڶ���
		//����ֱ��ʹ��cu�ĵ�½ip����Ϊƽ̨�ڵ������ӳ��IP
		strPlatCuiIp = pcCuAgent->m_strHostIp;
		if (pcCuAgent->m_tNatInfo.GetIpAddr() != pcCuAgent->m_strHostIp)
		{
			nPlatCuiPort = g_cCuiConfig.GetMapedNatPort();//��Ҫ�ж�hostip�Ƿ���Ǳ���ip�����Ǳ���ip������ʹ��natport��
		}
		else
		{
			nPlatCuiPort = g_cCuiConfig.GetNatPort();
		}
	}
	else
	{
		strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();
		nPlatCuiPort = g_cCuiConfig.GetNatPort();
	}

	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "ServiceIp is empty!\n");
		return false;
	}

	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CuiNatPort is empty!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CuiNatAddrForCu[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}

//CCallPuTaskNatSwitch
//////////////////////////////////////////////////////////////////////////