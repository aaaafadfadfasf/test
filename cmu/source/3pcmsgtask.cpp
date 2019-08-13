/*****************************************************************************
ģ����      : cmu 
�ļ���      : 3pcmsgtask.cpp
����ļ�    : 
�ļ�ʵ�ֹ���: CSwitchCreatorʵ��
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2012/08/02     1.0        ������        ����
******************************************************************************/

#include "3pcmsgtask.h"
#include "inviteswitchtask.h"


map<string, string> g_mapInterfaceIpList;
///////////////////////////////////////////////////////////////////////////////////////
//C3PCMsgTask
///////////////////////////////////////////////////////////////////////////////////////
C3PCMsgTask::C3PCMsgTask(CInstExt *pcInst): C3PCTask(pcInst)
{
}

C3PCMsgTask::~C3PCMsgTask()
{
    ReleaseResource();
}

void C3PCMsgTask::InitStateMachine()
{
    CStateProc cIdleProc;
    cIdleProc.ProcMsg = (CTask::PFProcMsg)&C3PCMsgTask::OnIdle;
    cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCMsgTask::OnIdle;
    cIdleProc.TimerPoll = (CTask::PFTimerPoll)&C3PCMsgTask::OnIdleTimer;
    AddRuleProc(Idle, cIdleProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&C3PCMsgTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&C3PCMsgTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&C3PCMsgTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Idle);
}

u32 C3PCMsgTask::OnIdle(CMessage *const pcMsg)
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
    case START_THIRD_PARTY_CALL_REQ:
        {
            //��ȡ��Ϣ��
            ProcSipReq2<CStartThirdPartyCallReq, CStartThirdPartyCallRsp>(pcMsg);

            CMS_RESULT nErrorCode = ProcStart3PCReq();
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

    case STOP_THIRD_PARTY_CALL_REQ:
        {
            //��ȡ��Ϣ��
            ProcSipReq2<CStopThirdPartyCallReq, CStopThirdPartyCallRsp>(pcMsg);

            dwRet = ProcStop3PCReq();
        }
        break;

    case CMU_CMU_CONNECT_SUCCESS:
        {
            if (this->GetReq()->GetEventId() == START_THIRD_PARTY_CALL_REQ)
            {
                if (ProcStart3PCReq() != CMS_SUCCESS)
                {
                    dwRet = PROCMSG_DEL;
                }
                else
                {
                    NextState(WaitRsp);
                }
            }
            else if (this->GetReq()->GetEventId() == STOP_THIRD_PARTY_CALL_REQ)
            {
                dwRet = ProcStop3PCReq();
            }
            else
            {
                TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "Unkonwn Event[%s-%d]\n", this->GetReq()->GetEventStr().c_str(), this->GetReq()->GetEventId());
            }
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�յ�SIP��Ĵ���Ӧ��
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����SIP�����sip status code[%d]\n", pcOspSipMsg->GetSipErrorCode());

            this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

            dwRet = PROCMSG_DEL;
            break;
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

u32 C3PCMsgTask::OnIdleTimer()
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

u32 C3PCMsgTask::OnWaitRsp(CMessage *const pcMsg)
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
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�յ�SIP��Ĵ���Ӧ��

            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "����SIP�����sip status code[%d]\n", pcOspSipMsg->GetSipErrorCode());

            this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

            dwRet = PROCMSG_DEL;
            break;
        }
        break;
	case START_THIRD_PARTY_CALL_RSP:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip ��ϢΪ��\n");
				SetErrorCode(ERR_CMU_SIP_BODY_NULL);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			CStartThirdPartyCallRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			SetErrorCode(cRsp.GetErrorCode());
			dwRet = PROCMSG_DEL;
			break;
		}

    case CMU_SWITCH_FINISH_CB:
    case STOP_THIRD_PARTY_CALL_RSP:
        {
            dwRet = PROCMSG_DEL;
            break;
        }
        break;

    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 C3PCMsgTask::OnWaitRspTimer()
{
    if (GetCurStateHoldTime() > CMU_SWITCH_TIMEOUT)
    {
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "����ʱ��ɾ��������\n");
        //this->PrintData();

        //this->SetErrorCode(ERR_CMU_SWITCH_TASK_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

CMS_RESULT C3PCMsgTask::ProcStart3PCReq()
{
    this->SetErrorCode(CMS_SUCCESS);

    CStartThirdPartyCallReq& cReq = *this->GetRealReq<CStartThirdPartyCallReq>();

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s].\n", 
        cReq.GetDstChn().ToString().c_str(), cReq.GetSrcChn().ToString().c_str());

    //��ʼ�������󣬵�����������������ֱ�ӷ���Ŀ��������
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        ////���Dst�����ӻỰ
        //CLoginSession* pDstSess = NULL;
        //TSipURI tRouteUri;
        //CheckDevRouter(cReq.GetDstChn().GetDevUri(), false, tRouteUri, &pDstSess);
        //if (this->GetErrorCode() !=  CMS_SUCCESS)
        //{
        //    return this->GetErrorCode();
        //}

        if (IsLocalDomainByDevUri(cReq.GetSrcChn().GetDevUri())
            && !(cReq.GetSdp().GetInterfaceIp().empty()))
		{
			m_strSrcChn = cReq.GetSrcChn().ToString();
			g_mapInterfaceIpList[m_strSrcChn] = cReq.GetSdp().GetInterfaceIp();
			TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn[%s], InterfaceIp[%s].\n", 
				m_strSrcChn.c_str(), cReq.GetSdp().GetInterfaceIp().c_str());
		}

        DoPostMsgReq(cReq, cReq.GetDstChn().GetDevUri());
    }
    else
    {
        //��ʼ������������
        CMS_RESULT nErrorCode = CSwitchTask::StartSwitch(cReq.GetDstChn(), cReq.GetSrcChn(), cReq.GetSdp(), this);
        this->SetErrorCode(nErrorCode);
    }

    return this->GetErrorCode();
}

PROCMSG_RESULT C3PCMsgTask::ProcStop3PCReq()
{
    this->SetErrorCode(CMS_SUCCESS);

    CStopThirdPartyCallReq& cReq = *this->GetRealReq<CStopThirdPartyCallReq>();

    TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "Dst[%s], Src[%s].\n", 
        cReq.GetDstChn().ToString().c_str(), cReq.GetSrcChn().ToString().c_str());

    //��ʼ�������󣬵�����������������ֱ�ӷ���Ŀ��������
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        ////���Dst�����ӻỰ
        //CLoginSession* pDstSess = NULL;
        //TSipURI tRouteUri;
        //CheckDevRouter(cReq.GetDstChn().GetDevUri(), false, tRouteUri, &pDstSess);
        //if (this->GetErrorCode() !=  CMS_SUCCESS)
        //{
        //    return this->GetErrorCode();
        //}

        if (DoPostMsgReq(cReq, cReq.GetDstChn().GetDevUri()))
        {
            NextState(WaitRsp);
            return PROCMSG_OK;
        }
        else
        {
            return PROCMSG_FAIL;
        }
    }
    else
    {
        //������Ӧ�Ľ�������
        CSwitchTask* pDstSwitchTask = g_cSwitchList.FindSwitchNode(cReq.GetDstChn(), CHANNEL_TYPE_RECVONLY);
        if (NULL == pDstSwitchTask || cReq.GetDstChn() != pDstSwitchTask->GetReceiver().GetChannel())
        {
            this->SetErrorCode(ERR_CMU_SWITCH_DST_NODE_NOT_EXIST);
            return PROCMSG_DEL;
        }

        CSwitchTask* pSrcSwitchTask = pDstSwitchTask->GetTopSrc();
        if (NULL == pSrcSwitchTask || cReq.GetSrcChn() != pSrcSwitchTask->GetSender().GetChannel())
        {
            this->SetErrorCode(ERR_CMU_SWITCH_SRC_NODE_NOT_EXIST);
            return PROCMSG_DEL;
        }

        //ɾ����Ӧ����
        SAFE_DEL_PTR(pDstSwitchTask);

        return PROCMSG_DEL;
    }
}

void C3PCMsgTask::ReleaseResource()
{
     C3PCTask::ReleaseResource();

	 if (!m_strSrcChn.empty())
	 {
		 g_mapInterfaceIpList.erase(m_strSrcChn);
		 TASKLOG(CMU_MOD_SWITCH, CRITICAL_LEV, "SrcChn[%s], InterfaceIpList size[%d]\n", 
			 m_strSrcChn.c_str(), g_mapInterfaceIpList.size());
		 m_strSrcChn.clear();
	 }

     if (this->GetErrorCode() != CMS_SUCCESS)
     {
         //����ʧ�ܣ���ʼ�������
         if(m_pcDstSwitchTask)
         {
             m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
             SAFE_DEL_PTR(m_pcDstSwitchTask);
         }
     } 
     else
     {
         //�����ɹ�����Ŀ�Ľ������� �� ���������������ϵ���
         if(m_pcDstSwitchTask)
         {
             m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
             m_pcDstSwitchTask = NULL;
         }
     }
}
