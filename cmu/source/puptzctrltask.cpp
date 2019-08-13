
#include "cms/cms_errorcode.h"

#include "cmudata.h"
#include "cmuconst.h"
#include "cmucoreapp.h"
#include "puptzctrltask.h"
#include "puregtask.h"
#include "cmuconnecttask.h"
#include "culogintask.h"

CPuPtzCtrlTask::CPuPtzCtrlTask(CInstExt* pInst) : CSipTask(pInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
}

CPuPtzCtrlTask::~CPuPtzCtrlTask()
{	
    ReleaseResource();
}

void CPuPtzCtrlTask::ReleaseResource()
{	
	/* 析构的时候只要找到对象了就Del，不要管Add的时候有没有条件限制 2018/9/26 zsy
    switch(GetState())
    {
    case Req:
        {
            if (!IsLocalDomainByDevUri(m_tReq.GetDevChn().GetDevUri()))
            {
                //异域操作需要删除登录依赖
                TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
                CLoginSession* ptCmuTask = NULL;
                g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
                if (ptCmuTask != NULL)
                {
                    ptCmuTask->DelTask(this);
                }
            }            
        }
        break;

    case WaitRsp:
        {
            //异域操作需要删除登录依赖
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
            if (ptCmuTask != NULL)
            {
                ptCmuTask->DelTask(this);
            }           
        }
        break;

    case Hold:
        {
            //异域操作需要删除登录依赖
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
            if (ptCmuTask != NULL)
            {
                ptCmuTask->DelTask(this);
            }          

            CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
            if (NULL != pcPuData && NULL != pcPuData->GetPuRegTask())
            {
                pcPuData->GetPuRegTask()->ErasePtzCtrlTask(m_tReq.GetDevChn().GetChnNO());
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }
	*/

	TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
	CLoginSession* ptCmuTask = NULL;
	g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
	if (ptCmuTask != NULL)
	{
		ptCmuTask->DelTask(this);
	}


	CLoginSession* ptUserLoginTask = GetLoginSession(m_tReq.GetSession());
	if (ptUserLoginTask != NULL)
	{
		ptUserLoginTask->DelTask(this);
	}

	if (GetState() == Hold)//只有Hold状态，表示当前用户是具有PTZ实际控制的，如果此时自己要Release了，才可以释放ErasePtzCtrlTask，否则那些因为抢占失败的任务进入本流程也会清掉这个有权限的Task导致，后续的PTZ控制都不受限制了 2018/11/28 zsy
	{
		CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
		if (NULL != pcPuData && NULL != pcPuData->GetPuRegTask())
		{
			pcPuData->GetPuRegTask()->ErasePtzCtrlTask(m_tReq.GetDevChn());
		}
	}
}


void CPuPtzCtrlTask::InitStateMachine()
{
    CStateProc cReqProc;
    cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnReq;
    cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnReq;
    cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnReqTimer;
    AddRuleProc(Req, cReqProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cHoldProc;
    cHoldProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnHold;
    cHoldProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnHold;
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnHoldTimer;
    AddRuleProc(Hold, cHoldProc);

    NextState(Req);
}

void CPuPtzCtrlTask::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  opr_dev[%s-%d] - opr_user[%s-%s]  SIP-trans[%u]\n",
        m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetDevChn().GetChnNO(),
        m_tReq.GetUserName().c_str(), m_tReq.GetSession().c_str(), m_tSipTransId);

    const TCamCtrl& tCamCtrl = m_tReq.GetCamCtrl();
    const vector<int>& tParams = tCamCtrl.GetParams();

    OspPrintf(TRUE, FALSE, "  commandId[%d], priLevel[%d], holdTime[%d], matrixIn[%d]\n",
        tCamCtrl.GetCommandId(), tCamCtrl.GetPriLevel(),
        tCamCtrl.GetHoldTime(), tCamCtrl.GetMatrixIn());

    for (size_t i=0; i<tParams.size(); i++)
    {
        char szParam[64] = {0};
        sprintf(szParam, "Param[%u]: %d", i, tParams[i]);
        OspPrintf(TRUE, FALSE, "    %s\n", szParam);
    }

    OspPrintf(TRUE, FALSE, "\n");
}

u32 CPuPtzCtrlTask::OnReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case PU_PTZ_CTRL_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            m_tSipTransId = pcOspSipMsg->GetSipTransID();

            CReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tReq = cReq;

            //确定pu路由
            TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevChn().GetDevUri());
            if (tPuRouterUri.IsNull())
            {
                TASKLOG(ERROR_LEV, "Dev[%s]无法路由, Msg[%s-%d]请求失败\n", 
                    m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                SendRsp(ERR_CMU_DEV_NO_ROUTER);
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            //用户是否登录
            CLoginSession* ptUserLoginTask = GetLoginSession(cReq.GetSession());
            if (ptUserLoginTask == NULL)
            {
                TASKLOG(CRITICAL_LEV, "用户[%s]登录session[%s]非法\n",
                    cReq.GetUserName().c_str(), cReq.GetSession().c_str());

                if (IsLocalDomainByDevUri(cReq.GetSession()))
                {
                    SendRsp(ERR_CMU_CU_LOGIN_SESS_ILLEGAL);
                }
                else
                {
                    SendRsp(ERR_CMU_CMU_NOT_CONN);
                }
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            //本域设备先进行控制权限判断，异域设备在异域判断
            if (IsLocalDomainByDevUri(tPuRouterUri))
            {
                //查找目标视频源通道是否有PTZ控制事务
                CPuRegBaseTask* ptPuRegTask = NULL;
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(cReq.GetDevChn().GetDevUri());
                if (NULL != pcPuData)
                {
                    ptPuRegTask = pcPuData->GetPuRegTask();
                }

                if (ptPuRegTask == NULL)
                {
                    TASKLOG(ERROR_LEV, "Dev[%s]不在线, Msg[%s-%d]请求失败\n", 
                        cReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_ONLINE);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                //发往pu的消息需要将session改写pu的登录session
                cReq.SetSession(ptPuRegTask->GetSession());

                CPuPtzCtrlTask* ptExistTask = (CPuPtzCtrlTask*)ptPuRegTask->GetPtzCtrlTask(m_tReq.GetDevChn());

                //如果存在，判断当前操作是否有权限抢占，同一登录会话可以直接抢占
                if (ptExistTask != NULL)
                {
                    if (ptExistTask->GetReq().GetSession() == m_tReq.GetSession())
                    {
                        //直接抢占
                    }
                    else
                    {
                        CReq& cExistReq = ptExistTask->GetReq();
                        if (m_tReq.GetCamCtrl().GetPriLevel() >= cExistReq.GetCamCtrl().GetPriLevel())
                        {
                            //新的控制权限大于当前控制权限，可以抢占
                            TASKLOG(ERROR_LEV, "用户[%s]-PriLevel[%d]抢占用户[%s]-PriLevel[%d]PTZ控制成功\n", 
                                cReq.GetUserName().c_str(), cReq.GetCamCtrl().GetPriLevel(),
                                cExistReq.GetUserName().c_str(), cExistReq.GetCamCtrl().GetPriLevel());
                        }
                        else
                        {
                            //抢占失败
                            TASKLOG(ERROR_LEV, "用户[%s]-PriLevel[%d]抢占用户[%s]-PriLevel[%d]PTZ控制失败\n", 
                                cReq.GetUserName().c_str(), cReq.GetCamCtrl().GetPriLevel(),
                                cExistReq.GetUserName().c_str(), cExistReq.GetCamCtrl().GetPriLevel());

                            CRsp cRsp;
                            cRsp.SetHeadFromReq(m_tReq);
                            cRsp.SetErrorCode(ERR_CMU_USER_PTZ_PRI_LOW);

                            //填写高权限的用户名，让抢占用户感知失败原因
                            cRsp.SetErrorDesc(cExistReq.GetUserName());
                            SendRsp(cRsp);
                            dwRet = PROCMSG_DEL;
                            return dwRet;
                        }
                    }
                }
            }
            else
            {
                if (!IsDomainInTopByDevUri(tPuRouterUri))
                {
                    TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]所在域未组网, Msg[%s-%d]请求失败\n", 
                        cReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }   
            }

            //转发消息
            SendReq(cReq, tPuRouterUri);

            ptUserLoginTask->AddTask(this);
        }
        break;

    case CMU_CMU_CONNECT_SUCCESS:
        {
            //异域cmu登录成功, 可以发送请求
            TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevChn().GetDevUri());
            if (!IsLocalDomainByDevUri(tPuRouterUri))
            {
                SendReq(m_tReq, tPuRouterUri);
            }
            else
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "非异域设备[%s], Receive Msg[%s-%d]\n", 
                    m_tReq.GetDevChn().GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_DEL;
        }
        break;
    }

    return dwRet;
}

u32 CPuPtzCtrlTask::OnReqTimer()
{
    //该定时器异域操作时会用到
    if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
    {
        TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
        TASKLOG(CMU_MOD_PU, WARNING_LEV, "登录异域cmu[%s]超时, Msg[%s-%d]请求失败\n", 
            tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
        SendRsp(ERR_CMU_CMU_NOT_CONN);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuPtzCtrlTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;
    switch(pcMsg->event)
    {
    case PU_PTZ_CTRL_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //ptz控制成功，进入hold状态
                NextState(Hold);

                CPuRegBaseTask* ptPuRegTask = NULL;
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
                if (NULL != pcPuData)
                {
                    ptPuRegTask = pcPuData->GetPuRegTask();
                }

                if (ptPuRegTask != NULL)
                {
                    ptPuRegTask->SetPtzCtrlTask(m_tReq.GetDevChn(), this);
                }
                return PROCMSG_OK;
            }
            else
            {
                TASKLOG(ERROR_LEV, "用户[%s]对设备[%s-%d]PTZ控制失败，错误码[%d]\n", 
                    m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
                    m_tReq.GetDevChn().GetChnNO(), cRsp.GetErrorCode());

                return dwRet;
            }
        }

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误，sip status code[%d]\n",
                m_tReq.GetDevChn().GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            SendRsp(ERR_CMU_DEV_NO_ROUTER);

            return dwRet;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 CPuPtzCtrlTask::OnWaitRspTimer()
{
    //应答超时
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
            m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuPtzCtrlTask::OnHold(CMessage *const pcMsg)
{
    //处理停止控制的逻辑，考虑使用设置holdtime为0来终止对ptz的控制
    //todo

    return PROCMSG_OK;
}

u32 CPuPtzCtrlTask::OnHoldTimer()
{
    if ((int)GetCurStateHoldTime() > m_tReq.GetCamCtrl().GetHoldTime())
    {
        TASKLOG(WARNING_LEV, "用户[%s]对设备[%s-%d]PTZ控制时间到期，停止该用户的控制\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetDevChn().GetChnNO());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void CPuPtzCtrlTask::SendReq(const CReq& cReq, const TSipURI& tDstUri)
{
    if (IsLocalDomainByDevUri(tDstUri))
    {
        //本域直接投递
        PostMsgReq(cReq, tDstUri);
        NextState(WaitRsp);
    }
    else
    {
        //异域处理，如果没有登陆过异域需要先登录，登录成功后再发起操作请求
        TSipURI tCmuUri = GetCmuUriFromDevUri(tDstUri);
        CLoginSession* ptCmuTask = NULL;
        g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
        CMS_ASSERT(NULL != ptCmuTask && "ptCmuTask = NULL!!!");         // 前面已经检查过域时存在的
		if (ptCmuTask)
		{
			if (ptCmuTask->GetState() == CCmuConnecterTask::Service)
			{
				PostMsgReq(cReq, tCmuUri);
				NextState(WaitRsp);
			}

			//跨域事务都需要登记到cmu登录会话中
			//对于长期事务，添加是为了域断链时释放长期事务的资源
			//对于瞬时事务，添加是为了cmu登录成功后即时通知到本事务
			//事务删除时，析构函数中需要去除本事务在cmu登录会话中的登记
			ptCmuTask->AddTask(this);
		}
    }
}

bool CPuPtzCtrlTask::SendRsp(u32 dwError)
{
    CRsp cRsp;
    cRsp.SetSeqNum(m_tReq.GetSeqNum());
    cRsp.SetSession(m_tReq.GetSession());
    cRsp.SetErrorCode((int)dwError);

    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}

bool CPuPtzCtrlTask::SendRsp(const CRsp& cRsp)
{
    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}
