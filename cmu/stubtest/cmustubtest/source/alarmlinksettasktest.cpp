
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "alarmlinksettasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"


CAlarmLinkSetTask::CAlarmLinkSetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
}

CAlarmLinkSetTask::~CAlarmLinkSetTask()
{
}

void CAlarmLinkSetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CAlarmLinkSetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CAlarmLinkSetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CAlarmLinkSetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CAlarmLinkSetTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CAlarmLinkSetTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CAlarmLinkSetTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    NextState(Wait);
}

void CAlarmLinkSetTask::PrintData()
{
    CTask::PrintData();
}

u32 CAlarmLinkSetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case ALARMLINK_CFG_SET_REQ:
        {         
            string strDevUri;
            if (pcMsg->content == NULL)
            {
                strDevUri = GetTestPuUri(1);
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                strDevUri = szPuUri;
            }

            static int s_CamPrePos = 0;

            static int s_alarmType = 0;

            CAlarmLinkCfgSetReq cReq;
            cReq.GetCfg().SetIsUsed(true);
            cReq.GetCfg().GetAlarmLinkSrc().SetDevUri(strDevUri);

            if (s_alarmType%2 == 0)
            {
                cReq.GetCfg().GetAlarmLinkSrc().SetAlarmType(ALARM_TYPE_PIN);
            }
            else
            {
                cReq.GetCfg().GetAlarmLinkSrc().SetAlarmType(ALARM_TYPE_MOVE);
            }

            s_alarmType++;
            
            cReq.GetCfg().GetAlarmLinkSrc().SetInputId(0);

            TAlarmLinkDst tDst;
            cReq.GetCfg().SetAlarmAlias("room alarm");
            
            /*
            TChannel        encChn;            //告警联动目的编码通道
            string          alarmAlias;        //告警别名
            CLinkActionList linkActionList;    //联动行为列表

            //联动需要的资源
            //摄像头
            int              camPrePos;        //摄象头预置位

            //电视墙   
            TChannel        decChn;            //解码器通道
            string          tvWallId;          //电视墙ID
            int             tvId;              //电视机ID

            //告警输出
            int              alarmOutChn;      //告警输出通道号

            //用户通知
            string          msgNo;            //短信号码
            string          vocNo;            //声讯号码
            string          mailNo;           //电子邮箱
            */

            CAlarmLinkDstList tAlarmLinkDstList;

            for (int i=0; i<1; i++)
            {
                tDst.GetEncChn().SetDevUri(GetTestPuUri(1));
                tDst.GetEncChn().SetChnNO(i);

                //tDst.GetLinkActionList().insert(ALARMLINK_PLAT_REC);
                tDst.GetLinkActionList().insert(ALARMLINK_TV_WALL);
                //tDst.GetLinkActionList().insert(ALARMLINK_READ_SEC);
                //tDst.GetLinkActionList().insert(ALARMLINK_IO_OUT);
                //tDst.GetLinkActionList().insert(ALARMLINK_CAM);

                tDst.SetCamPrePos(s_CamPrePos++);

                tDst.SetTvWallId("bff1bace1caa4523b4a3b601ddd7e28a@jiangsu");
                tDst.SetTvId(1);

                tAlarmLinkDstList.push_back(tDst);
            }
            
            cReq.GetCfg().SetAlarmLinkDstList(tAlarmLinkDstList);

            m_tReq = cReq;

            PostMsgReq(cReq, GetAlsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(MOD_ALS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CAlarmLinkSetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CAlarmLinkSetTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case ALARMLINK_CFG_SET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CAlarmLinkCfgSetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_ALS, EVENT_LEV, "告警联动参数设置成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_ALS, ERROR_LEV, "告警联动参数设置失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(MOD_ALS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CAlarmLinkSetTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > 3000)
    {
        TASKLOG(MOD_ALS, ERROR_LEV, "set alarm link param timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

bool CAlarmLinkSetTask::SendRsp(u32 dwError)
{
    CAlarmLinkCfgSetRsp cRsp;
    cRsp.SetHeadFromReq(m_tReq);
    cRsp.SetErrorCode(dwError);

    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}

bool CAlarmLinkSetTask::SendRsp(const CAlarmLinkCfgSetRsp& cRsp)
{
    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}