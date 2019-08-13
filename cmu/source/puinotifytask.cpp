
#include "puinotifytask.h"
#include "cmucoreapp.h"

CPuiNotifyList g_cPuiNotifyList;

CPuiNotifyTask::CPuiNotifyTask(CInstExt* pcInst, const TSipURI &tUri, CPuiNotifyList *pcNotifyList)
    : CTask(pcInst), m_tPuiUri(tUri), m_pcNotifyList(pcNotifyList)
{
    if (m_pcNotifyList)
    {
        m_pcNotifyList->m_cList.insert(map<string, CPuiNotifyTask *>::value_type(m_tPuiUri.GetURIString(), this));
    }

    m_pcSlidingWindowTask = NULL;
}

CPuiNotifyTask::~CPuiNotifyTask()
{
    if (m_pcNotifyList)
    {
        m_pcNotifyList->m_cList.erase(m_tPuiUri.GetURIString());
    }

    if (NULL != m_pcSlidingWindowTask)
    {
        delete m_pcSlidingWindowTask;
        m_pcSlidingWindowTask = NULL;
    }
}

void CPuiNotifyTask::InitStateMachine()
{
    CStateProc cWorkingProc;
    cWorkingProc.ProcMsg = (CTask::PFProcMsg)&CPuiNotifyTask::OnWorking;
    cWorkingProc.ProcErrMsg = (CTask::PFProcMsg)&CPuiNotifyTask::OnWorking;
    cWorkingProc.TimerPoll = (CTask::PFTimerPoll)&CPuiNotifyTask::OnWorkingTimer;
    AddRuleProc(Working, cWorkingProc);

    NextState(Working);
}

void CPuiNotifyTask::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\tPuiUri[%s], IsFinished[%d], WorkingTask[%u]\n", 
        m_tPuiUri.GetURIString().c_str(), IsFinished(), 
        (NULL == m_pcSlidingWindowTask ? INVALID_TASKNO : m_pcSlidingWindowTask->GetTaskNO()));
}

u32 CPuiNotifyTask::OnWorking(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    return dwRet;
}

u32 CPuiNotifyTask::OnWorkingTimer()
{
    if (IsFinished())           // 处理结束，可以退出了
    {
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void CPuiNotifyTask::Add(const set<string> &cIdSet)
{
    if (!cIdSet.empty())
    {
        if (NULL == m_pcSlidingWindowTask)
        {
            m_pcSlidingWindowTask = new CSlidingWindowTask<CPuiNotifyTask>(
                GetInstance(), this, NULL, &CPuiNotifyTask::PuOfflineProc);
            m_pcSlidingWindowTask->InitStateMachine();
            m_pcSlidingWindowTask->SetWindowSize(CRedisPipe::GetInitDevStatusNumPerTime());
        }

        m_pcSlidingWindowTask->Add(cIdSet);
    }
}

void CPuiNotifyTask::Add(const vector<ElementIdType> &cIds)
{
    if (!cIds.empty())
    {
        if (NULL == m_pcSlidingWindowTask)
        {
            m_pcSlidingWindowTask = new CSlidingWindowTask<CPuiNotifyTask>(
                GetInstance(), this, NULL, &CPuiNotifyTask::PuOfflineProc);
            m_pcSlidingWindowTask->InitStateMachine();
            m_pcSlidingWindowTask->SetWindowSize(CRedisPipe::GetInitDevStatusNumPerTime());
        }

        m_pcSlidingWindowTask->Add(cIds);
    }
}

bool CPuiNotifyTask::IsFinished() const
{
    if (NULL == m_pcSlidingWindowTask || m_pcSlidingWindowTask->IsFinished())
    {
        return true;
    }

    return false;
}

bool CPuiNotifyTask::PuOfflineProc(const vector<ElementIdType> &cIds)
{
    CDomainData *pcLocalDomainData = GetLocalDomainData();
    if (NULL != pcLocalDomainData)
    {
        if (GetLocalRedisPipe().InitDevStatus(cIds, true))
        {
            for (vector<ElementIdType>::const_iterator it = cIds.begin(); cIds.end() != it; ++it)
            {
                CPuData *pcPuData = GetPuData(*it);
                if (NULL != pcPuData)
                {
                    // 设置缓存的设备状态
                    pcPuData->SetPuIsOnline(false, false);

                    // 给sip订阅者发送设备状态通知
                    pcPuData->PublishSipOnOff();
                }
            }
        }
        else
        {
            TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "设备状态初始化失败, 设备数量[%u]\n", cIds.size());
        }
    }

    return true;
}

bool CPuiNotifyList::IsExistPuiNotifyTask(const TSipURI &tUri)
{
    map<string, CPuiNotifyTask *>::iterator it = m_cList.find(tUri.GetURIString());
    if (m_cList.end() != it && NULL != it->second)
    {
        return true;
    }

    return false;
}

CPuiNotifyTask *CPuiNotifyList::GetPuiNotifyTask(const TSipURI &tUri)
{
    CPuiNotifyTask *pcTask = NULL;
    map<string, CPuiNotifyTask *>::iterator it = m_cList.find(tUri.GetURIString());
    if (m_cList.end() == it || NULL == it->second)
    {
        pcTask = new CPuiNotifyTask(GetWorkInst(), tUri, this);
        pcTask->InitStateMachine();
    }
    else
    {
        pcTask = it->second;
    }

    return pcTask;
}

void CPuiNotifyList::PrintData() const
{
    u32 i = 0;
    for (map<string, CPuiNotifyTask *>::const_iterator it = m_cList.begin(); m_cList.end() != it; ++it)
    {
        u32 dwTaskNO = (it->second == NULL) ? INVALID_TASKNO : it->second->GetTaskNO();
        OspPrintf(TRUE, FALSE, "\t【%u】id[%s], tasknum[%u]\n", ++i, it->first.c_str(), dwTaskNO);
    }
}
