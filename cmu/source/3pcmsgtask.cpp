/*****************************************************************************
模块名      : cmu 
文件名      : 3pcmsgtask.cpp
相关文件    : 
文件实现功能: CSwitchCreator实现
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
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
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_DEL;
    }

    switch(pcMsg->event)
    {
    case START_THIRD_PARTY_CALL_REQ:
        {
            //获取消息体
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
            //获取消息体
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
            //收到SIP层的错误应答
            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发生SIP层错误，sip status code[%d]\n", pcOspSipMsg->GetSipErrorCode());

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
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "任务超时，删除该任务\n");
        //this->PrintData();

        //通常这里超时是因为跨域的cmu未连接超时，本身会有相应错误码
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
        CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
        //TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "ospsip 消息为空\n");
        //nErrorCode = ERR_CMU_SIP_BODY_NULL;
        return PROCMSG_FAIL;
    }

    switch(pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //收到SIP层的错误应答

            TASKLOG(CMU_MOD_SWITCH, ERROR_LEV, "发生SIP层错误，sip status code[%d]\n", pcOspSipMsg->GetSipErrorCode());

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
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
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
        TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "任务超时，删除该任务\n");
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

    //开始处理请求，第三方交换的请求是直接发给目的所在域。
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        ////检查Dst的连接会话
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
        //开始创建交换任务
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

    //开始处理请求，第三方交换的请求是直接发给目的所在域。
    if (!IsLocalDomainByDevUri(cReq.GetDstChn().GetDevUri()))
    {
        ////检查Dst的连接会话
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
        //查找相应的交换任务
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

        //删除相应交换
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
         //交换失败：开始拆除交换
         if(m_pcDstSwitchTask)
         {
             m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
             SAFE_DEL_PTR(m_pcDstSwitchTask);
         }
     } 
     else
     {
         //交换成功：把目的交换任务 与 创建者任务关联关系解除
         if(m_pcDstSwitchTask)
         {
             m_pcDstSwitchTask->SetSwitchCreatorTask(NULL);
             m_pcDstSwitchTask = NULL;
         }
     }
}
