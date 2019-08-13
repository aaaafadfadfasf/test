
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "sysparamsettasktest.h"
#include "cmustubtestcoreapp.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

#include "culogintasktest.h"

CPuSysParamSetTask::CPuSysParamSetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CPuSysParamSetTask::~CPuSysParamSetTask()
{
}

void CPuSysParamSetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuSysParamSetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSysParamSetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuSysParamSetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuSysParamSetTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSysParamSetTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuSysParamSetTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Wait);
}

void CPuSysParamSetTask::PrintData()
{
    CTask::PrintData();
}


u32 CPuSysParamSetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_SYS_PARAM_SET_REQ:
        {
            string strDevUri;
            if (pcMsg->content == NULL)
            {
                strDevUri = TEST_PU_URI;
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                strDevUri = szPuUri;
            }

            CCuLoginTask* ptCuLoginTask = NULL;

            Iterator pPos;
            string strSession;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
                {
                    ptCuLoginTask = (CCuLoginTask*)(pTask);
                    break;
                }
            }

            if (ptCuLoginTask == NULL)
            {
                TASKLOG(ERROR_LEV, "û��cu�û���¼\n");
                return PROCMSG_DEL;
            }

            CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
            if (ptPuData == NULL)
            {
                TASKLOG(ERROR_LEV, "�豸[%s]������, �������豸\n",
                    strDevUri.c_str());

                CPuData* ptPuData = new CPuData;
                ptPuData->SetDevUri(strDevUri);
                g_cPuList.AddPuData(ptPuData);
            }

            CPuSysParamSetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());
            cReq.SetDevUri(strDevUri);

            TPuSysParam&  tParam = ptPuData->m_tSysParam;         //ϵͳ����
            cReq.SetPuSysParam(tParam);

            //ת����Ϣ
            PostMsgReq(cReq, GetCmuUri());

            NextState(WaitRsp);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CPuSysParamSetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuSysParamSetTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_SYS_PARAM_SET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
                return PROCMSG_DEL;
            }

            CPuSysParamSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //�ɹ�
                TASKLOG(CRITICAL_LEV, "ǰ��ϵͳ�����ɹ�\n");
                return PROCMSG_DEL;
            }
            else
            {
                //ʧ��
                TASKLOG(ERROR_LEV, "ǰ��ϵͳ����ʧ��\n");
                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return PROCMSG_DEL;
}

u32 CPuSysParamSetTask::OnWaitRspTimer()
{
    //Ӧ��ʱ
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
            m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}