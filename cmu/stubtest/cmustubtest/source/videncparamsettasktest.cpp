
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "cmustubtestcoreapp.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

#include "culogintasktest.h"
#include "videncparamsettasktest.h"

CVidEncParamSetTask::CVidEncParamSetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CVidEncParamSetTask::~CVidEncParamSetTask()
{
}

void CVidEncParamSetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CVidEncParamSetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CVidEncParamSetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CVidEncParamSetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CVidEncParamSetTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CVidEncParamSetTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CVidEncParamSetTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Wait);
}

void CVidEncParamSetTask::PrintData()
{
    CTask::PrintData();
}


u32 CVidEncParamSetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_VID_ENC_PARAM_SET_REQ:
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

            CPuVidEncParamSetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);

            TVideoEncodeParam&  tParam = ptPuData->m_tVideoEncodeParam;         //��Ƶ�������
            TSupportMediaType&  tSurportedMediaType = ptPuData->m_tSurportedMediaType;  //����֧�ֵ�ý���ʽ
            cReq.SetVidEncParam(tParam);
            //cReq.SetSupportedMediaType(tSurportedMediaType);

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

u32 CVidEncParamSetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CVidEncParamSetTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_VID_ENC_PARAM_SET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
                return PROCMSG_DEL;
            }

            CPuVidEncParamSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //�ɹ�
                TASKLOG(CRITICAL_LEV, "������Ƶ��������ɹ�\n");
                return PROCMSG_DEL;
            }
            else
            {
                //ʧ��
                TASKLOG(ERROR_LEV, "������Ƶ�������ʧ��\n");
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

u32 CVidEncParamSetTask::OnWaitRspTimer()
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