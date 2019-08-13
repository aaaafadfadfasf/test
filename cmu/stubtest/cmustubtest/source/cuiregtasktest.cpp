
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmustubtestconst.h"
#include "culogintasktest.h"
#include "cuiregtasktest.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"
#include "cmustubtestconf.h"

CCuiRegTask* g_pcCuiRegCmuTask = NULL;

CCuiList g_cCuiList;

CCuiRegTask::CCuiRegTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
//     CCmuStubTestCoreInst* pInst = (CCmuStubTestCoreInst*)GetInstance();
//     CAppData& cAppData = pInst->GetAppData();
//     cAppData.m_tTestTask.Insert(GetTaskNO(), this);
//     u32 dwAppPt = (u32)(&cAppData);
//     OspPrintf(TRUE, FALSE, "appdata = %u!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", dwAppPt);
//     printf("appdata = %u!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", dwAppPt);
}

CCuiRegTask::~CCuiRegTask()
{
    g_pcCuiRegCmuTask = NULL;
//     CCmuStubTestCoreInst* pInst = (CCmuStubTestCoreInst*)GetInstance();
//     u32 dwAppPt = (u32)(&(pInst->GetAppData()));
//     OspPrintf(TRUE, FALSE, "appdata = %u!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", dwAppPt);
//     printf("appdata = %u!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", dwAppPt);
// 
//     pInst->GetAppData().m_tTestTask.Erase(GetTaskNO());
}

void CCuiRegTask::InitStateMachine()
{
    CStateProc cWaitRegProc;
    cWaitRegProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnWaitReg;
    cWaitRegProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnWaitRegTimer;
    AddRuleProc(WaitReg, cWaitRegProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCuiRegTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCuiRegTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitReg);
}

u32 CCuiRegTask::OnWaitReg(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CUI_REG_REQ:
        {
            CCuiRegReq cReq;
            string strCuiUri = GetLocalUri().GetURIString();
            cReq.SetDevUri(strCuiUri);
            cReq.SetSession(strCuiUri);
            m_tDevURI.SetURIString(cReq.GetDevUri());
            if (g_cCuiList.Exist(m_tDevURI.GetURIString()))
            {
                TASKLOG(MOD_CUI, ERROR_LEV, "�Ѵ��ڵ�cui[%s]��¼cmu\n",
                    m_tDevURI.GetURIString().c_str());
                return PROCMSG_DEL;
            }

            CIpAddrList& cIpAddrList = cReq.GetIpAddrList();
            TNetAddr tCuiWSAddr;
            tCuiWSAddr.SetNetIp(g_cCmuConf.m_tLocalAddr.GetIpStr());
            tCuiWSAddr.SetNetPort(80);
            cIpAddrList.push_back(tCuiWSAddr);

            PostMsgReq(cReq, GetCmuUri());

            dwRet = PROCMSG_OK;
        }
        break;

    case CUI_REG_RSP:
        {
            u32 dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(MOD_CUI, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuiRegRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            dwErrorCode = cRsp.GetErrorCode();
          
            if (dwErrorCode == CMS_SUCCESS)
            {
                //ע��ɹ�
                TASKLOG(MOD_CUI, CRITICAL_LEV, "cui[%s]��¼cmu�ɹ�!\n",
                    m_tDevURI.GetURIString().c_str());

                //��ʼ����
                SetHBParam(GetCmuUri());

                //��ӵ��б���
                g_cCuiList.Insert(m_tDevURI.GetURIString(), this);

                //�������̬
                NextState(Service);

                //TASK TIMER TEST
                SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
                TASKLOG(EVENT_LEV, "����Task��ʱ��[%s-%d]\n", 
                    OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(MOD_CUI, ERROR_LEV, "cui[%s]��¼cmuʧ��!\n",
                    m_tDevURI.GetURIString().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnWaitRegTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCuiRegTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CMU_TASK_TIMER_TEST:
        {
            TASKLOG(EVENT_LEV, "�յ�Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            SetTimer(CMU_TASK_TIMER_TEST, 5*1000);
            TASKLOG(PROGRESS_LEV, "����Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            SetTimer(CMU_TASK_TIMER_UPDATE, 2*1000);
            TASKLOG(PROGRESS_LEV, "����Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_UPDATE).c_str(), CMU_TASK_TIMER_UPDATE);

            dwRet = PROCMSG_OK;
        }
        break;

    case CMU_TASK_TIMER_UPDATE:
        {
            TASKLOG(EVENT_LEV, "�յ�Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            SetTimer(CMU_TASK_TIMER_TEST, 120*1000);
            TASKLOG(PROGRESS_LEV, "����Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            SetTimer(CMU_TASK_TIMER_KILL, 30*1000);
            TASKLOG(EVENT_LEV, "����Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_UPDATE).c_str(), CMU_TASK_TIMER_UPDATE);

            dwRet = PROCMSG_OK;
        }
        break;

    case CMU_TASK_TIMER_KILL:
        {
            TASKLOG(EVENT_LEV, "�յ�Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            KillTimer(CMU_TASK_TIMER_TEST);
            TASKLOG(EVENT_LEV, "ɾ��Task��ʱ��[%s-%d]\n", 
                OspExtEventDesc(CMU_TASK_TIMER_TEST).c_str(), CMU_TASK_TIMER_TEST);

            dwRet = PROCMSG_OK;
        }
        break;

    case OSP_SIP_DISCONNECT:
        {
            if (DisProc(pcMsg))
            {
                dwRet = PROCMSG_DEL;
            }
            else
            {
                dwRet = PROCMSG_FAIL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_CUI, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuiRegTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

bool CCuiRegTask::DisProc(CMessage *const pcMsg)
{
    //��������

    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(MOD_CUI, ERROR_LEV, "ospsip ��ϢΪ��\n");
        return false;
    }

    TASKLOG(MOD_CUI, CRITICAL_LEV, "cui[%s]��cmu����\n", m_tDevURI.GetNOString());

    //���б���ɾ��
    g_cCuiList.Erase(m_tDevURI.GetURIString());

    //cui������֪ͨ��cui������cu����
    Iterator pPos;
    CTask* pTask = NULL;

    COspSipMsg cOspSipMsg;
    cOspSipMsg.SetOspEventID(CMU_CUI_DISCONNECT);

    while (!pPos.End())
    {
        if (IterateChildTasks(pPos, pTask) && pTask != NULL)
        {
            cOspSipMsg.SetOspTaskNO(pTask->GetTaskNO());
            GetInstance()->PostMsg(CMU_CUI_DISCONNECT, cOspSipMsg.GetMsgBuf(), cOspSipMsg.GetMsgLen());
        }
    }

    return true;
}

void CCuiList::PrintData()
{
    Iterator pPos;
    string strUri;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cui��[%u]��\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strUri, pTask))
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. cui[%s]\n", i, strUri.c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cui��[%u]��\n", GetSize());
}
