
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "culogintask.h"
#include "cuiregtask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"

CCuList g_cCuList;

CCuLoginTask::CCuLoginTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
}

CCuLoginTask::~CCuLoginTask()
{
    ReleaseResource();
}

void CCuLoginTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_CU, EVENT_LEV, "cu[%s]-sess[%s]登录事务被销毁\n",
        m_tUserName.GetURIString().c_str(), GetSession().c_str());

    switch(GetState())
    {
    case WaitLogin:
        {
        }
        break;

    case Service:
        {
            //向uas写设备下线日志
            //((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            //清除该登录会话下所有事务
            DestroyChildTasks();

            //更新cu登录列表
            g_cCuList.Erase(GetSession());

            //删除cui中登记的cu登录事务
            CCuiRegTask* ptCuiRegTask = GetCuiRegTask();
            if (ptCuiRegTask != NULL)
            {
                ptCuiRegTask->DelTask(this);
            }
        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "未知事务状态[%d]\n", GetState());
        }
        break;
    }
}

void CCuLoginTask::InitStateMachine()
{
    CStateProc cWaitLoginProc;
    cWaitLoginProc.ProcMsg = (CTask::PFProcMsg)&CCuLoginTask::OnWaitLogin;
    cWaitLoginProc.ProcErrMsg = (CTask::PFProcMsg)&CCuLoginTask::OnWaitLogin;
    cWaitLoginProc.TimerPoll = (CTask::PFTimerPoll)&CCuLoginTask::OnWaitLoginTimer;
    AddRuleProc(WaitLogin, cWaitLoginProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCuLoginTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCuLoginTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCuLoginTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitLogin);
}

u32 CCuLoginTask::OnWaitLogin(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CU_LOGIN_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuLoginReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tUserName.SetURIString(cReq.GetUserName());
            //生成登录session
            CUUID cCuLoginSess;  //构造函数自动生成Session ID
            TSipURI tSessUri;    //session号也采用session@domain的方式，便于CMU统一处理
            tSessUri.SetUser(cCuLoginSess);
            tSessUri.SetDomain(GetLocalUri().GetDomain());
            SetSession(tSessUri.GetURIString());
            m_strCuVer = cReq.GetVersion();
            m_tCuiUri = pcOspSipMsg->GetSipFromUri();

            TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "CU[%s]通过CUI[%s]向CMU注册\n",
                m_tUserName.GetURIString().c_str(), m_tCuiUri.GetURIString().c_str());

            CCuiRegTask* ptCuiRegTask = GetCuiRegTask();
            if (ptCuiRegTask == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu登录使用的cui[%s]未向cmu注册\n",
                    m_tCuiUri.GetURIString().c_str());
                dwErrorCode = ERR_CMU_CUI_NOT_REG;
            }

            CCuLoginRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(GetSession());
			cRsp.SetCmsVer(GetCommonCmsVer());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //登录成功
                TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "cu[%s]登录cmu成功!\n",
                    m_tUserName.GetURIString().c_str());

                SetDevUri(GetSession());  //cu使用登录session作为DevUri
                SetDevType(DEV_TYPE_CU);
                SetServerUri(OspSipGetLocalURI().GetURIString());
                SetCuiIp(cReq.GetUsedCuiIp());

                //设置punch属性
                if (cReq.GetIsSupportNA())
                {
                    SetIsNeedPunch(true);
                }
                else
                {
                    SetIsNeedPunch(false);
                }

                //添加到CU列表中
                g_cCuList.Insert(GetSession(), this);

                //添加到CUI中，CUI断链时需要使用
                ptCuiRegTask->AddTask(this);

                //进入服务态
                NextState(Service);

                //向uas写设备日志
                // ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                dwRet = PROCMSG_OK;
            }
            else
            {
                //注册失败
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu[%s]登录cmu失败!\n",
                    m_tUserName.GetURIString().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuLoginTask::OnWaitLoginTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCuLoginTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case UPDATE_CULOGIN_INFO_REQ:
		{
			u32 dwErrorCode = CMS_SUCCESS;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip 消息为空\n");
				dwErrorCode = ERR_CMU_SIP_BODY_NULL;
				return dwRet;
			}

			CUpdateCuLoginInfoReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "UpdateCuLoginInfoReq[%s]\n", cReq.ToXml().c_str());
			SetIsNeedPunch(cReq.GetIsSupportNA());

			CUpdateCuLoginInfoRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			dwRet = PROCMSG_OK;
		}
		break;

    case CU_GETCUILIST_REQ:
        {
            u32 dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                return dwRet;
            }

            CCuGetCuiListReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CCuGetCuiListRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());

            //填写ipaddrlist
            CIpAddrList& cIpAddrList = cRsp.GetIpAddrList();
            Iterator pPos;
            string strUri;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuiList.Iterate(pPos, strUri, pTask) && pTask != NULL)
                {
                    CCuiRegTask* pCuiRegTask = (CCuiRegTask*)pTask;
                    const CIpAddrList& cCuiIpAddrList = pCuiRegTask->GetIpAddrList();
                    cIpAddrList.insert(cIpAddrList.begin(), cCuiIpAddrList.begin(), cCuiIpAddrList.end());
                }
            }

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case CU_LOGOUT_REQ:
        {
            LogoutProc(pcMsg);
            dwRet = PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuLoginTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CCuLoginTask::LogoutProc(CMessage *const pcMsg)
{
    int dwErrorCode = CMS_SUCCESS;
    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip 消息为空\n");
        return;
    }

    CCuLogoutReq cReq;
    pcOspSipMsg->GetMsgBody(cReq);

    CCuLogoutRsp cRsp;
    cRsp.SetErrorCode(dwErrorCode);
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(GetSession());
    PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
}

CCuiRegTask* CCuLoginTask::GetCuiRegTask()
{
    CLoginSession* ptCuiRegTask = NULL;
    g_cCuiList.Find(m_tCuiUri.GetURIString(), ptCuiRegTask);
    return (CCuiRegTask*)ptCuiRegTask;
}

size_t CCuLoginTask::GetDevNetSegNO()
{
    size_t nCuNetSegNO = 0;
    CCuiRegTask* pcCuiTask = GetCuiRegTask();
    if (pcCuiTask == NULL)
    {
        TASKLOG(CMU_MOD_CU, ERROR_LEV, "pcCuiTask == NULL\n");
        return nCuNetSegNO;
    }

    //const CIpAddrList& cCuiAddrList = pcCuiTask->GetDevAddrList();
    //for (size_t i=0; i<cCuiAddrList.size(); i++)
    //{
    //    const TNetAddr& tCuiAddr = cCuiAddrList.at(i);
    //    if (tCuiAddr.GetNetIp() == GetCuiIp())
    //    {
    //        nCuNetSegNO = i;
    //        TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "Cu[%s]-usedCuiIp[%s]使用cui[%s]的NetSegNO[%u]接入\n", 
    //            GetDevUri().c_str(), GetCuiIp().c_str(), 
    //            pcCuiTask->GetDevUri().c_str(), nCuNetSegNO);

    //        return nCuNetSegNO;
    //    }
    //}
	// 修改为使用CUI自己报上来的网段信息 [1/26/2015 pangubing]

	//todo Royan 20150318
	//const TNetSegmentList&  cCuiNetSegmentList = pcCuiTask->GetNetSegmentList();
	//for (size_t i=0; i<cCuiNetSegmentList.size(); i++)
	//{
	//	const TNetSegment& tCuiAddr = cCuiNetSegmentList.at(i);
	//	if (tCuiAddr.GetSegmentIp() == GetCuiIp())
	//	{
	//		nCuNetSegNO = tCuiAddr.GetSegmentNum();
	//		TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "Cu[%s]-usedCuiIp[%s]使用cui[%s]的NetSegNO[%u]接入\n", 
	//			GetDevUri().c_str(), GetCuiIp().c_str(), 
	//			pcCuiTask->GetDevUri().c_str(), nCuNetSegNO);

	//		return nCuNetSegNO;
	//	}
	//}


    TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu[%s]-usedCuiIp[%s]没有找到cui[%s]对应的NetSegNO\n", 
        GetDevUri().c_str(), GetCuiIp().c_str(), pcCuiTask->GetDevUri().c_str());

    return nCuNetSegNO;
}

string CCuLoginTask::GetJoinInterfaceIp(const string &strSession)
{
	return m_strCuiIp;
}

void CCuList::PrintData()
{
    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "当前在线的cu：[%u]个\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strSession, pTask) && pTask != NULL)
        {
            CCuLoginTask* pCuLoginTask = (CCuLoginTask*)(pTask);
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. cu[%s]-taskNO[%u]-CuiIp[%s]-session[%s]\n", i, 
                pCuLoginTask->GetUserName().GetURIString().c_str(), pTask->GetTaskNO(),
                pCuLoginTask->GetCuiIp().c_str(),pCuLoginTask->GetSession().c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "当前在线的cu：[%u]个\n", GetSize());
}
