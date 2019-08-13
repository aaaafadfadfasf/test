
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/utility/uuid.h"

#include "pusstasktest.h"
#include "cmustubtestconst.h"
#include "cmustubtestdata.h"
#include "culogintasktest.h"
#include "cmustubtestcoreapp.h"

CPuSubscribeTask::CPuSubscribeTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSsDlgId = INVALID_DIALOG_ID;
}

CPuSubscribeTask::~CPuSubscribeTask()
{
}

void CPuSubscribeTask::InitStateMachine()
{
    CStateProc cWaitSsProc;
    cWaitSsProc.ProcMsg = (CTask::PFProcMsg)&CPuSubscribeTask::OnWaitSs;
    cWaitSsProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSubscribeTask::OnWaitSs;
    cWaitSsProc.TimerPoll = (CTask::PFTimerPoll)&CPuSubscribeTask::OnWaitSsTimer;
    AddRuleProc(WaitSs, cWaitSsProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CPuSubscribeTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CPuSubscribeTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CPuSubscribeTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitSs);
}

void CPuSubscribeTask::TerminateTask()
{
    if (m_tSsDlgId != INVALID_DIALOG_ID)
    {
        OspSipReleaseDlg(m_tSsDlgId);
        m_tSsDlgId = INVALID_DIALOG_ID;
    }
}

u32 CPuSubscribeTask::OnWaitSs(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_STATUS_SS_REQ:
        {
            if (pcMsg->content == NULL)
            {
                SetDevUri(TEST_PU_URI);
            }
            else
            {
                const char* szPuUri = (const char*) (pcMsg->content);
                SetDevUri(szPuUri);
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

            SetSession(ptCuLoginTask->GetSession());
            SetUserUri(ptCuLoginTask->GetUserName());

            set<string> tStatusTypes;
            tStatusTypes.insert(PU_ON_OFF_STATUS);
            tStatusTypes.insert(PU_GPS_STATUS);
            //tStatusTypes.insert(PU_CONFIG_STATUS);
            //tStatusTypes.insert(PU_SERVICE_STATUS);
            //tStatusTypes.insert(PU_ALARM_STATUS);
            SetStatusType(tStatusTypes);

            CPuStatusSsReq cReq;
            cReq.SetSession(GetSession());
            cReq.SetDevUri(GetDevUri());
            cReq.SetUserUri(GetUserUri());
            cReq.SetSsTypes(GetStatusType());

            PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, cReq, GetCmuUri());

            string strTypes;
            typedef set<string>::const_iterator SIT;
            for (SIT sIt = m_tStatusTypes.begin(); sIt != m_tStatusTypes.end(); ++sIt)
            {
                strTypes += *sIt;
                strTypes += " ";
            }  
            TASKLOG(EVENT_LEV, "用户[%s]订阅PU[%s]信息，订阅类型:[%s]\n",
                m_strUserUri.c_str(), m_strdevUri.c_str(), strTypes.c_str());

            dwRet = PROCMSG_OK;
        }
        break;

    case PU_STATUS_SS_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            m_tSsDlgId = pcOspSipMsg->GetSipDlgID();

            CPuStatusSsRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            u32 dwErrorCode = cRsp.GetErrorCode();

            if (dwErrorCode == CMS_SUCCESS)
            {
                TASKLOG(CRITICAL_LEV, "订阅PU[%s]成功\n",
                    m_strdevUri.c_str());

                CLoginSession* ptTask = NULL;
                g_cCuList.Find(GetSession(), ptTask);
                if (ptTask != NULL)
                {
                    ptTask->AddTask(this);
                }

                CPuData* ptPuData = new CPuData;
                ptPuData->SetDevUri(GetDevUri());
                g_cPuList.AddPuData(ptPuData);

                NextState(Service);
            }
            else
            {
                TASKLOG(ERROR_LEV, "订阅PU[%s]失败，错误码[%u]\n",
                    m_strdevUri.c_str(), dwErrorCode);

                dwRet = PROCMSG_DEL;
            }  
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

void CPuSubscribeTask::OnOffNtf(const TPuStatus& tPuStatus)
{
    if (m_tStatusTypes.find(PU_ON_OFF_STATUS) != m_tStatusTypes.end())  //是否订阅
    {
        TASKLOG(EVENT_LEV, "设备[%s]是否在线[%u]\n",
            m_strdevUri.c_str(), tPuStatus.GetIsOnline());

        CPuData* ptPuData = g_cPuList.FindPuData(GetDevUri());
        if (ptPuData != NULL)
        {
            ptPuData->SetPuStatus(tPuStatus);
        }
    }  
}

void CPuSubscribeTask::ConfigNtf(const TPuDevConfig& tPuConfig)
{
    if (m_tStatusTypes.find(PU_CONFIG_STATUS) != m_tStatusTypes.end())  //是否订阅
    {
        TASKLOG(EVENT_LEV, "设备[%s]配置信息通知\n",
            m_strdevUri.c_str());

        CPuData* ptPuData = g_cPuList.FindPuData(GetDevUri());
        if (ptPuData != NULL)
        {
            ptPuData->SetPuConfig(tPuConfig);
        }
    }  
}

void CPuSubscribeTask::ServiceNtf(const TPuServiceStatus& tPuService)
{
    if (m_tStatusTypes.find(PU_SERVICE_STATUS) != m_tStatusTypes.end())  //是否订阅
    {
        //订阅了设备的业务状态
        TASKLOG(EVENT_LEV, "设备[%s]服务状态通知\n",
            m_strdevUri.c_str());

        CPuData* ptPuData = g_cPuList.FindPuData(GetDevUri());
        if (ptPuData != NULL)
        {
            ptPuData->SetPuServiceStatus(tPuService);
        }
    }  
}

void CPuSubscribeTask::AlarmNtf(const TPuAlarmStatus& tPuAlarm)
{
    if (m_tStatusTypes.find(PU_ALARM_STATUS) != m_tStatusTypes.end())  //是否订阅
    {
        //订阅了设备的告警状态
        TASKLOG(EVENT_LEV, "设备[%s]告警状态通知\n",
            m_strdevUri.c_str());

        CPuData* ptPuData = g_cPuList.FindPuData(GetDevUri());
        if (ptPuData != NULL)
        {
            ptPuData->SetPuAlarmStatus(tPuAlarm);
        }
    }   
}

void CPuSubscribeTask::GpsDataNtf(const TGpsData& tPuAlarm)
{
    if (m_tStatusTypes.find(PU_GPS_STATUS) != m_tStatusTypes.end())  //是否订阅
    {
        //订阅了设备的告警状态
        TASKLOG(EVENT_LEV, "设备[%s]GPS数据通知\n",
            m_strdevUri.c_str());
    }   
}

u32 CPuSubscribeTask::OnWaitSsTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPuSubscribeTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case PU_STATUS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_FAIL;
            }

            CPuStatusNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            OnOffNtf(cReq.GetOnOffStatus());

            CPuStatusNtyRsp cRsp;
            cRsp.SetEvent(PU_STATUS_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;

    case PU_CONFIG_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_FAIL;
            }

            CPuConfigNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            ConfigNtf(cReq.GetConfig());

            CPuConfigNtyRsp cRsp;
            cRsp.SetEvent(PU_CONFIG_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;

    case PU_SERVICE_STATUS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_FAIL;
            }

            CPuServiceStatusNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            ServiceNtf(cReq.GetServiceStatus());

            CPuServiceStatusNtyRsp cRsp;
            cRsp.SetEvent(PU_SERVICE_STATUS_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;

    case PU_ALARM_STATUS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_FAIL;
            }

            CPuAlarmStatusNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            AlarmNtf(cReq.GetAlarmStatus());

            CPuAlarmStatusNtyRsp cRsp;
            cRsp.SetEvent(PU_ALARM_STATUS_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;

    case PU_GPS_STATUS_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
                return PROCMSG_FAIL;
            }

            CPuGpsStatusNtyReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            GpsDataNtf(cReq.GetGpsData());

            CPuGpsStatusNtyRsp cRsp;
            cRsp.SetEvent(PU_GPS_STATUS_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;

    case PU_STATUS_REFRESH_SS_REQ:
        {
            dwRet = PROCMSG_OK;    
        }
        break;

    case PU_STATUS_UNSS_REQ:
        {
            CPuStatusUnSsReq cReq;
            cReq.SetUserUri(GetUserUri());
            cReq.SetDevUri(GetDevUri());

            PostInDlgReq(KDSIP_EVENT_UNSUBSCRIBE_REQ, cReq, m_tSsDlgId);

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

u32 CPuSubscribeTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}


