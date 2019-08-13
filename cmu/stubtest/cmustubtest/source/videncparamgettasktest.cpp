
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "cmustubtestcoreapp.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

#include "culogintasktest.h"
#include "videncparamgettasktest.h"

CVidEncParamGetTask::CVidEncParamGetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CVidEncParamGetTask::~CVidEncParamGetTask()
{
}

void CVidEncParamGetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CVidEncParamGetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CVidEncParamGetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CVidEncParamGetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CVidEncParamGetTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CVidEncParamGetTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CVidEncParamGetTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Wait);
}

void CVidEncParamGetTask::PrintData()
{
    CTask::PrintData();
}


u32 CVidEncParamGetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_VID_ENC_PARAM_GET_REQ:
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

            CPuVidEncParamGetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);

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

u32 CVidEncParamGetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CVidEncParamGetTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_VID_ENC_PARAM_GET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
                return PROCMSG_DEL;
            }

            CPuVidEncParamGetRsp cRsp;
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
                ptPuData->m_tVideoEncodeParam = cRsp.GetVidEncParam();
                ptPuData->m_tSurportedMediaType = cRsp.GetSupportedMediaType();
            }

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //�ɹ�
                TASKLOG(CRITICAL_LEV, "��ȡ��Ƶ��������ɹ�\n");
                return PROCMSG_DEL;
            }
            else
            {
                //ʧ��
                TASKLOG(ERROR_LEV, "��ȡ��Ƶ�������ʧ��\n");
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

u32 CVidEncParamGetTask::OnWaitRspTimer()
{
    //Ӧ��ʱ
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
            m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}