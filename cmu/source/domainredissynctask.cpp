
#include "domainredissynctask.h"
#include "cmucoreapp.h"
#include "cmuconnecttask.h"

CDomainRedisSyncTask::CDomainRedisSyncTask(CInstExt* pcInst, CDomainData *pcSrcDomainData, CCmuConnecterTask *pcLoginTask) 
    : CTask(pcInst), m_pcSrcDomainData(pcSrcDomainData), m_pcConnectTask(pcLoginTask)
{
    m_dwSyncNumOnce = DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE;
    m_dwTop   = 0;
    m_dwTotal = 0;

    CMS_ASSERT(NULL != m_pcConnectTask && "unreachable...");
    m_pcConnectTask->AddTask(this);
}

CDomainRedisSyncTask::~CDomainRedisSyncTask()
{
    m_pcConnectTask->ExitDomainRedisSyncTask(this);
    m_pcConnectTask->DelTask(this);
}

void CDomainRedisSyncTask::InitStateMachine()
{
    CStateProc cIdleProc;
    cIdleProc.ProcMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnIdle;
    cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnIdle;
    cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CDomainRedisSyncTask::OnIdleTimer;
    AddRuleProc(Idle, cIdleProc);

    CStateProc cSyncProc;
    cSyncProc.ProcMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnSync;
    cSyncProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnSync;
    cSyncProc.TimerPoll = (CTask::PFTimerPoll)&CDomainRedisSyncTask::OnSyncTimer;
    AddRuleProc(Sync, cSyncProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainRedisSyncTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CDomainRedisSyncTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(Idle);
}

bool CDomainRedisSyncTask::IsNotNeedPollState()
{
    return (Service == GetState());
}

void CDomainRedisSyncTask::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\nSrcDomain[%s], SyncNumOnce[%u], RestNum[%u], Top[%u], Total[%u], RestTimes[%u]s\n", 
        m_pcSrcDomainData->GetDomainName().c_str(), m_dwSyncNumOnce, 
        m_cSyncList.size(), m_dwTop, m_dwTotal, GetRestSyncTimes());

    if (g_bDebugOn && !m_cSyncList.empty())
    {
        OspPrintf(TRUE, FALSE, "\nNext device waiting for sync[%s]\n", m_cSyncList.front().strDevId.c_str());
    }
}

u32 CDomainRedisSyncTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    default:
        {
            TASKLOG(CMU_MOD_REDIS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CDomainRedisSyncTask::OnIdleTimer()
{
    if (GetCurStateHoldTime() >= CMU_REQ_RSP_TASK_TIMEOUT)
    {
        StartSync();
    }

    return TIMERPOLL_DONE;
}

u32 CDomainRedisSyncTask::OnSync(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case CMU_DOMAIN_REDIS_SYNC:
        {
            // 接着同步
            StartSync();

            dwRet = PROCMSG_OK;
        }
        break;
    case CMU_CMU_DISCONNECT:
        {
            Reset();

            dwRet = PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_REDIS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CDomainRedisSyncTask::OnSyncTimer()
{
    return TIMERPOLL_DONE;
}

u32 CDomainRedisSyncTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case CMU_CMU_DISCONNECT:
        {
            Reset();

            dwRet = PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_REDIS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CDomainRedisSyncTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CDomainRedisSyncTask::StartSync()
{
    CRedisPipe &cSrcRedisPipe = m_pcSrcDomainData->GetRedisPipe();
    if (cSrcRedisPipe.IsConnected())
    {
        for (u32 i = 0; !m_cSyncList.empty() && i < m_dwSyncNumOnce; ++i)
        {
            const TRedisSyncUnit &tSyncUnit = m_cSyncList.front();
            const string &strDevId   = tSyncUnit.strDevId;
            CPuData *pcPuData = m_pcSrcDomainData->GetPuList().GetPuData(strDevId);
            if (NULL != pcPuData)
            {
                const TSsTypes &tSsTypes = tSyncUnit.tSsTypes;

                // 优先同步PU_ON_OFF类型
                bool bSyncPuStatus = false;
                if (tSsTypes.end() != tSsTypes.find(PU_ON_OFF_STATUS))
                {
                    TPuStatus tPuStatus;
                    if (m_pcSrcDomainData->GetRedisPipe().GetPuStatus(strDevId, tPuStatus))
                    {
                        // 同步上下线状态信息
                        pcPuData->SetPuStatus(tPuStatus);

                        // 同步完成之后，需要发布一下
                        pcPuData->PublishOnOff(tPuStatus);

                        bSyncPuStatus = true;
                    }
                    else
                    {
                        TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                            m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), PU_ON_OFF_STATUS);
                    }
                }

                // 只有设备已经在线了，才有必要进一步同步其他的信息
                if (pcPuData->GetPuIsOnline())
                {
                    for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
                    {
                        if (PU_ON_OFF_STATUS == *it)
                        {
                            // 上下线已经同步过了
                            continue;
                        }
                        else if (PU_CONFIG_STATUS == *it)
                        {
                            TPuConfig tPuConfig;
                            if (m_pcSrcDomainData->GetRedisPipe().GetPuConfig(strDevId, tPuConfig))
                            {
                                pcPuData->SetPuConfig(tPuConfig);

                                pcPuData->PublishConfig(tPuConfig.GetDevConfig());
                            }
                            else
                            {
                                TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                                    m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), it->c_str());
                            }
                        }
                        else if (PU_SERVICE_STATUS == *it)
                        {
                            TPuServiceStatus tPuServiceStatus;
                            if (m_pcSrcDomainData->GetRedisPipe().GetPuServiceStatus(strDevId, tPuServiceStatus))
                            {
                                pcPuData->SetPuServiceStatus(tPuServiceStatus);

                                pcPuData->PublishService(tPuServiceStatus);
                            }
                            else
                            {
                                TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                                    m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), it->c_str());
                            }
                        }
                        else if (PU_ALARM_STATUS == *it)
                        {
                            TPuAlarmStatus tPuAlarmStatus;
                            if (m_pcSrcDomainData->GetRedisPipe().GetPuAlarmStatus(strDevId, tPuAlarmStatus))
                            {
                                pcPuData->SetAlarmStatus(tPuAlarmStatus);

                                pcPuData->PublishAlarm(tPuAlarmStatus);
                            }
                            else
                            {
                                TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                                    m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), it->c_str());
                            }
                        }
                        else if (PU_GPS_STATUS == *it)
                        {
                            TGpsData tGpsData;
                            if (m_pcSrcDomainData->GetRedisPipe().GetPuGpsData(strDevId, tGpsData))
                            {
                                pcPuData->SetPuGpsData(tGpsData);

                                pcPuData->PublishGpsData(tGpsData);
                            }
                            else
                            {
                                TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                                    m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), it->c_str());
                            }
                        }
                        else if (PU_TRANSDATA_STATUS == *it)
                        {
                            TTransparentData tTransData;
                            if (m_pcSrcDomainData->GetRedisPipe().GetPuTransparentData(strDevId, tTransData))
                            {
                                pcPuData->SetPuTransparentData(tTransData);

                                pcPuData->PublishTransData(tTransData);
                            }
                            else
                            {
                                TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "同步数据时获取域[%s]的数据出错，DevId[%s], Type[%s]\n", 
                                    m_pcSrcDomainData->GetDomainName().c_str(), strDevId.c_str(), it->c_str());
                            }
                        }
                        else
                        {
                            CMS_ASSERT(false && "unreachable!!!");
                        }
                    }
                }
            }

            m_cSyncList.pop_front();
        }

        if (m_cSyncList.empty())
        {
            NextState(Service);
        }
        else
        {
            u32 dwTaskNO = GetTaskNO();
            OspPost(CMU_CORE_DAEMON, CMU_DOMAIN_REDIS_SYNC, &dwTaskNO, sizeof(dwTaskNO));

            NextState(Sync);
        }
    }
    else
    {
        NextState(Idle);
    }
}

void CDomainRedisSyncTask::Reset()
{
    m_cSyncList.clear();
    m_dwTotal = 0;
    m_dwTop = 0;

    NextState(Idle);
}

void CDomainRedisSyncTask::AddSync()
{
    vector<string> cDevId;
    m_pcSrcDomainData->GetPuList().GetPuIdList(cDevId);

    AddSync(cDevId);
}

void CDomainRedisSyncTask::AddSync(const string &strDevId)
{
    vector<string> cDevId;
    cDevId.push_back(strDevId);

    AddSync(cDevId);
}

void CDomainRedisSyncTask::AddSync(const vector<string> &cDevId)
{
    for (vector<string>::const_iterator it = cDevId.begin(); cDevId.end() != it; ++it)
    {
        TSsTypes tSsTypes;
        tSsTypes.insert(PU_ON_OFF_STATUS);
        tSsTypes.insert(PU_CONFIG_STATUS);
        tSsTypes.insert(PU_SERVICE_STATUS);
        tSsTypes.insert(PU_ALARM_STATUS);
        tSsTypes.insert(PU_GPS_STATUS);
        tSsTypes.insert(PU_TRANSDATA_STATUS);

        Post(*it, tSsTypes);
    }

    // 如果已经是Sync状态，直接投递即可，后续按照排队处理
    if (Sync != GetState())
    {
        StartSync();
    }
}

void CDomainRedisSyncTask::Post(const string &strDevId, const TSsTypes &tSsTypes)
{
    TRedisSyncUnit tSyncUnit;
    tSyncUnit.strDevId = strDevId;
    tSyncUnit.tSsTypes = tSsTypes;

    m_cSyncList.push_back(tSyncUnit);

    // 统计信息
    u32 dwQueueSize = m_cSyncList.size();
    if (m_dwTop < dwQueueSize)
    {
        m_dwTop = dwQueueSize;
    }

    ++m_dwTotal;
}
