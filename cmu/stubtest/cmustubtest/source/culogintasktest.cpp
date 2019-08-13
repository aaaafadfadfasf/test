
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
                TASKLOG(MOD_CU, ERROR_LEV, "cui��û��ע��\n");
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
                TASKLOG(MOD_CU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuLoginRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            SetSession(cRsp.GetSession());
            TASKLOG(MOD_CU, EVENT_LEV, "�û���¼session[%s]\n",
                GetSession().c_str());
            SetCmuVer(cRsp.GetCmsVer());
            TASKLOG(MOD_CU, EVENT_LEV, "cmsƽ̨version[%s]\n",
                GetCmuVer().c_str());         

            if (dwErrorCode == CMS_SUCCESS)
            {
                //��¼�ɹ�
                TASKLOG(MOD_CU, CRITICAL_LEV, "cu[%s]��¼cmu�ɹ�,��¼session[%s]!\n",
                    m_strUserName.c_str(), GetSession().c_str());

                //��ӵ�CU�б���
                g_cCuList.Insert(GetSession(), this);

                if (g_pcCuiRegCmuTask != NULL)
                {
                    g_pcCuiRegCmuTask->AddTask(this);
                }

                //�������̬
                NextState(Service);

                //��ȥCUI��ַ�б�
                CCuGetCuiListReq cReq;
                cReq.SetSession(m_strSession);
                PostMsgReq(cReq, GetCmuUri());

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(MOD_CU, ERROR_LEV, "cu[%s]��¼cmuʧ��!\n",
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
                TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
                return dwRet;
            }

            //TASK TIMER TEST
            SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
            TASKLOG(EVENT_LEV, "����Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            CCuGetCuiListRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //�ɹ�
                TASKLOG(CRITICAL_LEV, "�û�[%s]��ȡcui��ַ�б�ɹ�\n", 
                    GetUserName().c_str());

                //��ӡipaddrlist
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
                TASKLOG(ERROR_LEV, "�û�[%s]��ȡcui��ַ�б�ʧ�ܣ�������[%d]\n", 
                    GetUserName().c_str(), cRsp.GetErrorCode());

                return PROCMSG_FAIL;
            }
        }
        break;

    case CMU_TASK_TIMER_TEST:
        {
            TASKLOG(PROGRESS_LEV, "�յ�Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

//             SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
//             TASKLOG(PROGRESS_LEV, "����Task��ʱ��[%s-%d]\n", 
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
    TASKLOG(MOD_CU, EVENT_LEV, "cu[%s]-sess[%s]��¼��������\n",
        m_strUserName.c_str(), GetSession().c_str());
    //��������������
    DestroyAllTasks();

    //����cu��¼�б�
    g_cCuList.Erase(GetSession());
}

void CCuLoginTask::DestroyAllTasks()
{
    //����õ�¼�Ự����������
    DestroyChildTasks();
}


void CCuList::PrintData()
{
    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cu��[%u]��\n", GetSize());    
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

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cu��[%u]��\n", GetSize());
}
