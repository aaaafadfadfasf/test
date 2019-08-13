
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "sysparamgettasktest.h"
#include "cmustubtestcoreapp.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

#include "culogintasktest.h"

CPuSysParamGetTask::CPuSysParamGetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CPuSysParamGetTask::~CPuSysParamGetTask()
{
}

void CPuSysParamGetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuSysParamGetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSysParamGetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuSysParamGetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuSysParamGetTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSysParamGetTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuSysParamGetTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Wait);
}

void CPuSysParamGetTask::PrintData()
{
    CTask::PrintData();
}


u32 CPuSysParamGetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_SYS_PARAM_GET_REQ:
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

            CPuSysParamGetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());
            cReq.SetDevUri(strDevUri);

            SetReq(cReq);

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

u32 CPuSysParamGetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuSysParamGetTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_SYS_PARAM_GET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
                return PROCMSG_DEL;
            }

            CPuSysParamGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            string strDevUri = GetReq().GetDevUri();
            CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
            if (ptPuData == NULL)
            {
                TASKLOG(ERROR_LEV, "�豸[%s]������\n",
                    strDevUri.c_str());
            }
            else
            {
                ptPuData->m_tSysParam = cRsp.GetPuSysParam();
            }

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //�ɹ�
                TASKLOG(CRITICAL_LEV, "��ȡǰ��ϵͳ�����ɹ�\n");
                return PROCMSG_DEL;
            }
            else
            {
                //ʧ��
                TASKLOG(ERROR_LEV, "��ȡǰ��ϵͳ����ʧ��\n");
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

u32 CPuSysParamGetTask::OnWaitRspTimer()
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