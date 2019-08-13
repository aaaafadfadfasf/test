
#include "cms/cms_const.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "alarmlinkgettasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "cmustubtestcoreapp.h"


CAlarmLinkGetTask::CAlarmLinkGetTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
}

CAlarmLinkGetTask::~CAlarmLinkGetTask()
{
    if (!m_strQryTag.empty())
    {
        g_cStubQryTaskList.Erase(m_strQryTag);
    }  
}

void CAlarmLinkGetTask::InitStateMachine()
{
    CStateProc cWaitProc;
    cWaitProc.ProcMsg = (CTask::PFProcMsg)&CAlarmLinkGetTask::OnWait;
    cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CAlarmLinkGetTask::OnWait;
    cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CAlarmLinkGetTask::OnWaitTimer;
    AddRuleProc(Wait, cWaitProc);

    CStateProc cRspProc;
    cRspProc.ProcMsg = (CTask::PFProcMsg)&CAlarmLinkGetTask::OnRsp;
    cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CAlarmLinkGetTask::OnRsp;
    cRspProc.TimerPoll = (CTask::PFTimerPoll)&CAlarmLinkGetTask::OnRspTimer;
    AddRuleProc(Rsp, cRspProc);

    NextState(Wait);
}

void CAlarmLinkGetTask::PrintData()
{
    CTask::PrintData();
}

u32 CAlarmLinkGetTask::OnWait(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case ALARMLINK_CFG_GET_REQ:
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

            CAlarmLinkCfgGetReq cReq;
            cReq.GetAlarmSrc().SetDevUri(strDevUri);
            cReq.GetAlarmSrc().SetAlarmType(ALARM_TYPE_PIN);
            cReq.GetAlarmSrc().SetInputId(0);

            m_tReq = cReq;

            PostMsgReq(cReq, GetAlsUri());

            //等待Rsp
            NextState(Rsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case ALARMLINK_QUERY_REQ:
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

            CAlarmLinkQueryReq cReq;

            PostMsgReq(cReq, GetAlsUri());

            //等待Rsp
            NextState(Rsp);

            g_cStubQryTaskList[cReq.GetQueryTag()] = this;

            m_strQryTag = cReq.GetQueryTag();

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

u32 CAlarmLinkGetTask::OnWaitTimer()
{
    return TIMERPOLL_DONE;
}

u32 CAlarmLinkGetTask::OnRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case ALARMLINK_CFG_GET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CAlarmLinkCfgGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_ALS, EVENT_LEV, "告警联动参数获取成功\n");
                return PROCMSG_DEL;
            }
            else
            {
                //失败
                TASKLOG(MOD_ALS, ERROR_LEV, "告警联动参数获取失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case ALARMLINK_QUERY_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CAlarmLinkQueryRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //成功
                TASKLOG(MOD_ALS, EVENT_LEV, "告警联动参数查询成功. 总数[%d]\n",
                    cRsp.GetTotalEntryNum());

                if (cRsp.GetTotalEntryNum() == 0)
                {
                    dwRet = PROCMSG_DEL;
                }
                else
                {
                    dwRet = PROCMSG_OK;
                }
            }
            else
            {
                //失败
                TASKLOG(MOD_ALS, ERROR_LEV, "告警联动参数查询失败\n");
                return PROCMSG_DEL;
            }
        }
        break;

    case ALARMLINK_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_DEL;
            }

            CAlarmLinkNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CAlarmLinkNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (cReq.IsLastNtf())
            {
                TASKLOG(MOD_ALS, ERROR_LEV, "告警联动参数通知结束\n");
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

u32 CAlarmLinkGetTask::OnRspTimer()
{
    if (GetCurStateHoldTime() > 30000)
    {
        TASKLOG(MOD_ALS, ERROR_LEV, "get alarm link param timeout!\n");
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

bool CAlarmLinkGetTask::SendRsp(u32 dwError)
{
    CAlarmLinkCfgSetRsp cRsp;
    cRsp.SetHeadFromReq(m_tReq);
    cRsp.SetErrorCode(dwError);

    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}

bool CAlarmLinkGetTask::SendRsp(const CAlarmLinkCfgGetRsp& cRsp)
{
    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}