
#include "cms/cms_errorcode.h"


#include "cmustubtestdata.h"
#include "cmustubtestconst.h"
#include "culogintasktest.h"
#include "cmustubtestcoreapp.h"
#include "puptzctrltasktest.h"
#include "cms/camera/newcamera.h"

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
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnHoldTimer;
    AddRuleProc(Hold, cHoldProc);

    NextState(Req);
}

u32 CPuPtzCtrlTask::OnReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

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
        dwRet = PROCMSG_DEL;
        return dwRet;
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

    m_tReq.SetSession(ptCuLoginTask->GetSession());
    m_tReq.SetUserName(ptCuLoginTask->GetUserName());

    TChannel tDevChn;    //�豸ͨ��
    tDevChn.SetDevUri(strDevUri);
    tDevChn.SetChnNO(0);
    m_tReq.SetDevChn(tDevChn);

    TCamCtrl tCamCtrl;   //������Ϣ
    tCamCtrl.SetCommandId(CAM_COMMAND_MOVEUP);
    vector<int>& tParams = tCamCtrl.GetParams();
    tParams.push_back(2);
    tCamCtrl.SetHoldTime(60);
    tCamCtrl.SetPriLevel(3);

    m_tReq.SetCamCtrl(tCamCtrl);

    //ת����Ϣ
    PostMsgReq(m_tReq, GetCmuUri());

    //�ȴ�Ӧ��
    NextState(WaitRsp);

    return dwRet;
}

u32 CPuPtzCtrlTask::OnReqTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuPtzCtrlTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(ERROR_LEV, "ospsip ��ϢΪ��\n");
        return dwRet;
    }

    CRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
        //ptz���Ƴɹ�
        TASKLOG(CRITICAL_LEV, "�û�[%s]���豸[%s-%d]PTZ���Ƴɹ�\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
            m_tReq.GetDevChn().GetChnNO());
        return dwRet;
    }
    else
    {
        TASKLOG(ERROR_LEV, "�û�[%s]���豸[%s-%d]PTZ����ʧ�ܣ�������[%d]\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
            m_tReq.GetDevChn().GetChnNO(), cRsp.GetErrorCode());

        return PROCMSG_DEL;
    }
}

u32 CPuPtzCtrlTask::OnWaitRspTimer()
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

u32 CPuPtzCtrlTask::OnHold(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}

u32 CPuPtzCtrlTask::OnHoldTimer()
{
    return TIMERPOLL_DONE;
}

