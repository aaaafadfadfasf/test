
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
        TASKLOG(ERROR_LEV, "没有cu用户登录\n");
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

    CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
    if (ptPuData == NULL)
    {
        TASKLOG(ERROR_LEV, "设备[%s]不存在, 创建新设备\n",
            strDevUri.c_str());

        CPuData* ptPuData = new CPuData;
        ptPuData->SetDevUri(strDevUri);
        g_cPuList.AddPuData(ptPuData);
    }

    m_tReq.SetSession(ptCuLoginTask->GetSession());
    m_tReq.SetUserName(ptCuLoginTask->GetUserName());

    TChannel tDevChn;    //设备通道
    tDevChn.SetDevUri(strDevUri);
    tDevChn.SetChnNO(0);
    m_tReq.SetDevChn(tDevChn);

    TCamCtrl tCamCtrl;   //控制信息
    tCamCtrl.SetCommandId(CAM_COMMAND_MOVEUP);
    vector<int>& tParams = tCamCtrl.GetParams();
    tParams.push_back(2);
    tCamCtrl.SetHoldTime(60);
    tCamCtrl.SetPriLevel(3);

    m_tReq.SetCamCtrl(tCamCtrl);

    //转发消息
    PostMsgReq(m_tReq, GetCmuUri());

    //等待应答
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
        TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
        return dwRet;
    }

    CRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
        //ptz控制成功
        TASKLOG(CRITICAL_LEV, "用户[%s]对设备[%s-%d]PTZ控制成功\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
            m_tReq.GetDevChn().GetChnNO());
        return dwRet;
    }
    else
    {
        TASKLOG(ERROR_LEV, "用户[%s]对设备[%s-%d]PTZ控制失败，错误码[%d]\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
            m_tReq.GetDevChn().GetChnNO(), cRsp.GetErrorCode());

        return PROCMSG_DEL;
    }
}

u32 CPuPtzCtrlTask::OnWaitRspTimer()
{
    //应答超时
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
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

