
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/utility/uuid.h"

#include "culogintasktest.h"
#include "cuiregtasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

CCuList g_cCuList;

CCuLoginTask::CCuLoginTask(CInstExt *pcInst) : CLoginSession(pcInst)
{  
}

CCuLoginTask::~CCuLoginTask()
{
    if (g_pcCuiRegCmuTask != NULL)
    {
        g_pcCuiRegCmuTask->DelTask(this);
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
            if (g_pcCuiRegCmuTask == NULL)
            {
                TASKLOG(MOD_CU, ERROR_LEV, "cui还没有注册\n");
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuLoginReq cReq;

            TSipURI tCuUri;
            tCuUri.SetDomain(GetLocalUri().GetDomain());
            tCuUri.SetUser("stubtest_cu");
            string strCuUri = tCuUri.GetURIString();
            string strPassword = "12345678";
            cReq.SetUserName(strCuUri);
            cReq.SetPassword(strPassword);
            SetUserName(strCuUri);
            string strCuVer;
            PostMsgReq(cReq, GetCmuUri());

            dwRet = PROCMSG_OK;
        }
        break;

    case CU_LOGIN_RSP:
        {
            u32 dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(MOD_CU, ERROR_LEV, "ospsip 消息为空\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuLoginRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            SetSession(cRsp.GetSession());
            TASKLOG(MOD_CU, EVENT_LEV, "用户登录session[%s]\n",
                GetSession().c_str());
            SetCmuVer(cRsp.GetCmsVer());
            TASKLOG(MOD_CU, EVENT_LEV, "cms平台version[%s]\n",
                GetCmuVer().c_str());         

            if (dwErrorCode == CMS_SUCCESS)
            {
                //登录成功
                TASKLOG(MOD_CU, CRITICAL_LEV, "cu[%s]登录cmu成功,登录session[%s]!\n",
                    m_strUserName.c_str(), GetSession().c_str());

                //添加到CU列表中
                g_cCuList.Insert(GetSession(), this);

                if (g_pcCuiRegCmuTask != NULL)
                {
                    g_pcCuiRegCmuTask->AddTask(this);
                }

                //进入服务态
                NextState(Service);

                //回去CUI地址列表
                CCuGetCuiListReq cReq;
                cReq.SetSession(m_strSession);
                PostMsgReq(cReq, GetCmuUri());

                dwRet = PROCMSG_OK;
            }
            else
            {
                //注册失败
                TASKLOG(MOD_CU, ERROR_LEV, "cu[%s]登录cmu失败!\n",
                    m_strUserName.c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
    case CU_GETCUILIST_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return dwRet;
            }

            //TASK TIMER TEST
            SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
            TASKLOG(EVENT_LEV, "设置Task定时器[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            CCuGetCuiListRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(CRITICAL_LEV, "用户[%s]获取cui地址列表成功\n", 
                    GetUserName().c_str());

                //打印ipaddrlist
                CIpAddrList& cIpAddrList = cRsp.GetIpAddrList();
                for (size_t i=0; i<cIpAddrList.size(); i++)
                {
                    TASKLOG(CRITICAL_LEV, "%u. [%s:%d]\n",
                        i+1, cIpAddrList[i].GetNetIp().c_str(), cIpAddrList[i].GetNetPort());
                }

                return dwRet;
            }
            else
            {
                TASKLOG(ERROR_LEV, "用户[%s]获取cui地址列表失败，错误码[%d]\n", 
                    GetUserName().c_str(), cRsp.GetErrorCode());

                return PROCMSG_FAIL;
            }
        }
        break;

    case CMU_TASK_TIMER_TEST:
        {
            TASKLOG(PROGRESS_LEV, "收到Task定时器[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

//             SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
//             TASKLOG(PROGRESS_LEV, "设置Task定时器[%s-%d]\n", 
//                 OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            dwRet = PROCMSG_OK;
        }
        break;

    case CU_LOGOUT_REQ:
        {
            LogoutProc(pcMsg);
            dwRet = PROCMSG_DEL;
        }
        break;

    case CMU_CUI_DISCONNECT:
        {
            DisProc(pcMsg);
            dwRet = PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuLoginTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CCuLoginTask::DisProc(CMessage *const pcMsg)
{
    TerminateTask();
}

void CCuLoginTask::LogoutProc(CMessage *const pcMsg)
{
    CCuLogoutReq cReq;
    cReq.SetSession(m_strSession);
    PostMsgReq(cReq, GetCmuUri());

    TerminateTask();
}

void CCuLoginTask::TerminateTask()
{
    TASKLOG(MOD_CU, EVENT_LEV, "cu[%s]-sess[%s]登录事务被销毁\n",
        m_strUserName.c_str(), GetSession().c_str());
    //销毁所有子事务
    DestroyAllTasks();

    //更新cu登录列表
    g_cCuList.Erase(GetSession());
}

void CCuLoginTask::DestroyAllTasks()
{
    //清除该登录会话下所有事务
    DestroyChildTasks();
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
            OspPrintf(TRUE, FALSE, "  %4u. cu[%s]-session[%s]\n", 
                i, pCuLoginTask->GetUserName().c_str(), pCuLoginTask->GetSession().c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "当前在线的cu：[%u]个\n", GetSize());
}
