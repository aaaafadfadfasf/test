
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "cmustubtestcoreapp.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"

#include "culogintasktest.h"
#include "punormalcfgtasktest.h"

CPuNormalCfgTask::CPuNormalCfgTask(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CPuNormalCfgTask::~CPuNormalCfgTask()
{
}

void CPuNormalCfgTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuNormalCfgTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuNormalCfgTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuNormalCfgTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CPuNormalCfgTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuNormalCfgTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuNormalCfgTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    NextState(Wait);
}

void CPuNormalCfgTask::PrintData()
{
    CTask::PrintData();
}


u32 CPuNormalCfgTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

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

    switch(pcMsg->event)
    {
    case PU_MOTION_DETECT_SET_REQ:
        {
            //�ƶ��������
            CPuMotionDetectSetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);

            TMotionDetectList& tParamList = cReq.GetParam();
            TMotionDetectParam tParam;
            tParam.SetEnable(true);
            tParam.SetLeftTopX(8);
            tParam.SetLeftTopY(16);
            tParam.SetWidth(80);
            tParam.SetHeight(160);
            tParam.SetAlarmRestoreRate(50);
            tParam.SetAlarmOccurRate(60);
            tParamList.push_back(tParam);

            cReq.SetParam(tParamList);

            //ת����Ϣ
            PostMsgReq(cReq, GetCmuUri());

            NextState(Rsp);

            dwRet = PROCMSG_OK;
           
            
        }
        break;

    case PU_MOTION_DETECT_GET_REQ:
        {
            //�ƶ�����ȡ
            CPuMotionDetectGetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);

            //ת����Ϣ
            PostMsgReq(cReq, GetCmuUri());

            NextState(Rsp);

            dwRet = PROCMSG_OK;      
        }
        break;

    case PU_AREA_SHADE_SET_REQ:
        {
            //ͼ���ڱ�����
            CPuAreaShadeSetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);

            TAreaShadeList& tParamList = cReq.GetParam();
            TAreaShadeParam tParam;
            tParam.SetEnable(true);
            tParam.SetLeftTopX(8);
            tParam.SetLeftTopY(16);
            tParam.SetWidth(80);
            tParam.SetHeight(160);
            tParamList.push_back(tParam);

            cReq.SetParam(tParamList);

            //ת����Ϣ
            PostMsgReq(cReq, GetCmuUri());

            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case PU_AREA_SHADE_GET_REQ:
        {
            //�ƶ��������
            CPuAreaShadeGetReq cReq;

            cReq.SetSession(ptCuLoginTask->GetSession());
            cReq.SetUserUri(ptCuLoginTask->GetUserName());

            TChannel tDevChn;    //�豸ͨ��
            tDevChn.SetDevUri(strDevUri);
            tDevChn.SetChnNO(0);
            cReq.SetDevChn(tDevChn);           

            //ת����Ϣ
            PostMsgReq(cReq, GetCmuUri());

            NextState(Rsp);

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

u32 CPuNormalCfgTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuNormalCfgTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
        return PROCMSG_DEL;
    }

    switch(pcMsg->event)
    {
    case PU_MOTION_DETECT_SET_RSP:
        {
            CPuMotionDetectSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(EVENT_LEV, "�ƶ�������óɹ�\n");             
            }
            else
            {
                TASKLOG(ERROR_LEV, "�ƶ��������ʧ�ܣ�errCode[%d]\n", cRsp.GetErrorCode());
            }

            dwRet = PROCMSG_DEL;
        }
        break;

    case PU_AREA_SHADE_SET_RSP:
        {
            CPuAreaShadeSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(EVENT_LEV, "ͼ���ڱ����óɹ�\n");             
            }
            else
            {
                TASKLOG(ERROR_LEV, "ͼ���ڱ�����ʧ�ܣ�errCode[%d]\n", cRsp.GetErrorCode());
            }

            dwRet = PROCMSG_DEL;
        }
        break;

    case PU_MOTION_DETECT_GET_RSP:
        {
            CPuMotionDetectGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(EVENT_LEV, "�ƶ�����ȡ�ɹ�\n");             
            }
            else
            {
                TASKLOG(ERROR_LEV, "�ƶ�����ȡʧ�ܣ�errCode[%d]\n", cRsp.GetErrorCode());
            }

            dwRet = PROCMSG_DEL;
        }
        break;

    case PU_AREA_SHADE_GET_RSP:
        {
            CPuAreaShadeGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(EVENT_LEV, "ͼ���ڱλ�ȡ�ɹ�\n");             
            }
            else
            {
                TASKLOG(ERROR_LEV, "ͼ���ڱλ�ȡʧ�ܣ�errCode[%d]\n", cRsp.GetErrorCode());
            }

            dwRet = PROCMSG_DEL;
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

u32 CPuNormalCfgTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "set pu config timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}