#include "cms/ospext/osptask.h"
#include "cms/ospext/ospdbg.h"
#include "cms/ospext/ospinst.h"
#include "taskmng.h"

extern void OspExtPrintf(TOspExtLogContent& tLogContent);

extern u32 g_dwTraceTaskNO;
extern u32 g_dwTraceAppInst;
extern CXMap<u32, TLogModInfo> g_cModuleLogLev;


/************************************CTask 类的实现*********************************/

CTask::CTask(CInstExt *pcInst) : COspObj()
{  
    m_dwTaskNO = INVALID_TASKNO;
    m_dwCurState = UNKNOWN_STATE;       //初始空状态
    m_dwCheckRef = 0;
    m_nPollCounter = -1;                //这里初始值设置为-1, 那么下次直接从0开始计数
    m_tCreateTime = 0;
    InitCreateTime();
    //InitStateMachine();
    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    m_mapRule.Insert(m_dwCurState, cProc);
    m_tCurStateCreateTime = OspGetSecByTicks();

    m_pcInstance = pcInst;
    
    if (pcInst)
    {
        pcInst->AddTask(this);
    }
}

CTask::~CTask()
{
    CInstExt* pcInst = (CInstExt*)GetInstance();
    
    if (pcInst)
    {
        pcInst->DelTask(this);
    }

    m_dwCurState = UNKNOWN_STATE;
    m_mapRule.Empty();

    m_dwTaskNO = INVALID_TASKNO;
    m_dwCheckRef = 0;

    m_pcInstance = NULL;
}

u32 CTask::TimerPollSkip()
{
    //如果用户需要实现自己的轮询超时处理, 请自定义形如 u32 XXXStateTimerPoll()函数，并注册到RuleMap中
    return TIMERPOLL_DONE;
}

//
void CTask::PrintData() const
{
    COspTime tTaskCreateTime(m_tCreateTime);
    char szTime[64];    
    tTaskCreateTime.GetString(szTime, 64);

    const char* szAppName = "Unkown App";
    u16 wAppId = 0;
    u16 wInstId = CInstance::INVALID;
    if (m_pcInstance)
    {
        szAppName = m_pcInstance->GetStrApp();
        wAppId = m_pcInstance->GetAppID();
        wInstId = m_pcInstance->GetInsID();
    }
    
    OspPrintf(TRUE, FALSE, "App[%s-%u], Inst[%u], Task[%s-%u], CreateTime[%s], PollCnt[%d], CurState[%s]\n", 
        szAppName, wAppId, wInstId, GetObjName(), GetTaskNO(), 
        szTime, GetPollCounter(), GetStateName(GetState()));
}

//
bool CTask::IsTask(const char* szTaskName) const
{
    if (szTaskName == NULL)
    {
        return false;
    }
    
    if (0 == strcmp("CTask", szTaskName))
    {
        return true;
    }
    
    return (0 == strcmp(this->GetObjName(), szTaskName));
}
    
//
u32 CTask::GetTaskNO() const
{
    return m_dwTaskNO;
}
    
void CTask::SetTaskNO(const u32 dwTaskNO)
{
    m_dwTaskNO = dwTaskNO;
}

u32 CTask::GetCheckRef() const
{
    return m_dwCheckRef;
}

void CTask::SetCheckRef(const u32 dwCheckRef)
{
    m_dwCheckRef = dwCheckRef;
}

void CTask::NextState(u32 dwState)
{
    TASKLOG(OSPEXT, ALL_LEV, "任务[%u]由[%u]状态转为[%u]状态\n", 
        GetTaskNO(), GetState(), dwState);

    m_dwCurState = dwState;
    m_tCurStateCreateTime = OspGetSecByTicks();
    //每次切换状态说明收到的新的消息, 需要将轮询计数清空
    ClearPollCounter();
  
    if (IsNotNeedPollState())
    {
        //目标状态不需要轮询
        GetInstance()->EraseTaskFromPollMap(GetTaskNO());
    }
    else
    {
        //目标状态需要轮询
        GetInstance()->AddTaskToPollMap(GetTaskNO());
    }
}

//
CInstExt* CTask::GetInstance() const
{
    return m_pcInstance;
}
    
//
u16 CTask::GetAppID() const
{
    if (m_pcInstance == NULL)
    {
        return INVALID_APP;
    }

    return m_pcInstance->GetAppID();
}

//
u16 CTask::GetInsID() const
{
    if (m_pcInstance == NULL)
    {
        return INVALID_INS;
    }

    return m_pcInstance->GetInsID();
}

//
TOspTransaction CTask::GetOspTrans() const
{
    TOspTransaction tOspTrans;
    if (m_pcInstance != NULL)
    {
        tOspTrans.dwIID = m_pcInstance->GetOspIID();
    }
    
    tOspTrans.dwTaskID = m_dwTaskNO;
    tOspTrans.dwSeqNO = INVALID_SEQ_NO;

    return tOspTrans;
}
    
//
u32 CTask::GetState() const
{
    return m_dwCurState;
}

u32 CTask::GetCurStateHoldTime() const
{
    u64 tCurTime = OspGetSecByTicks();
    return u32(tCurTime - m_tCurStateCreateTime);
}

bool CTask::GetStateProc(const u32 dwState, CStateProc &cProc)
{
    return m_mapRule.Find(dwState, cProc);
}
    
// 
void CTask::ClearPollCounter()
{
    m_nPollCounter = -1;
}

s32 CTask::GetPollCounter() const
{
    return m_nPollCounter;
}

void CTask::AddPollCounter()
{
    m_nPollCounter++;
}

void CTask::Log(u8 byLogLevel, const char* szFormat, ...) const
{
    if (m_pcInstance == NULL)
    {
        return;
    }

    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;

    if (m_pcInstance->m_byLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    u32 dwCurrAppInst = MAKEIID(m_pcInstance->GetAppID(), m_pcInstance->GetInsID());
    if (g_dwTraceAppInst != 0                  //正在跟踪某个app-inst
        && g_dwTraceAppInst != dwCurrAppInst)  //当前不是跟踪的app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0     //正在跟踪某个事务
        && g_dwTraceTaskNO != GetTaskNO())    //当前不是跟踪的事务
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }   

    char szModLev[256] = {0};
    sprintf(szModLev, "[%s]: ", GetStrLogLev(byLogLevel).c_str());
    tLogContent.strModLev = szModLev;

    char szOspPrefix[256] = {0};
    sprintf(szOspPrefix, "App[%s-%u]-inst[%u][%s]-task[%s-%u][%s]: ", 
        m_pcInstance->GetStrApp(), m_pcInstance->GetAppID(), 
        m_pcInstance->GetInsID(), m_pcInstance->GetStrState(),
        GetObjName(), m_dwTaskNO, GetStateName(GetState()));
    tLogContent.strOspPrifixField = szOspPrefix;

    OspExtPrintf(tLogContent);
}

void CTask::Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const
{
    //
    if (m_pcInstance == NULL)
    {
        return;
    }

    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;

    if (m_pcInstance->m_byLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_cModuleLogLev[byLogMod].dwLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen = FALSE;
    }

    u32 dwCurrAppInst = MAKEIID(m_pcInstance->GetAppID(), m_pcInstance->GetInsID());
    if (g_dwTraceAppInst != 0                  //正在跟踪某个app-inst
        && g_dwTraceAppInst != dwCurrAppInst)  //当前不是跟踪的app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0     //正在跟踪某个事务
        && g_dwTraceTaskNO != GetTaskNO())    //当前不是跟踪的事务
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }   

    char szModLev[256] = {0};
    sprintf(szModLev, "[%s][%s]: ", g_cModuleLogLev[byLogMod].strModName.c_str(), GetStrLogLev(byLogLevel).c_str());
    tLogContent.strModLev = szModLev;

    char szOspPrefix[256] = {0};
    sprintf(szOspPrefix, "App[%s-%u]-inst[%u][%s]-task[%s-%u][%s]: ", 
        m_pcInstance->GetStrApp(), m_pcInstance->GetAppID(), 
        m_pcInstance->GetInsID(), m_pcInstance->GetStrState(),
        GetObjName(), m_dwTaskNO, GetStateName(GetState()));
    tLogContent.strOspPrifixField = szOspPrefix;

    OspExtPrintf(tLogContent);
}

//
time_t CTask::GetCreateTime() const
{
    return m_tCreateTime;
}
    
//
void CTask::SetCreateTime(time_t tCreateTime)
{
    m_tCreateTime = tCreateTime;
}
    
//
void CTask::InitCreateTime()
{
    time(&m_tCreateTime);
}

//
void CTask::AddRuleProc(u32 dwState, const CStateProc &cProc)
{
    m_mapRule.Insert(dwState, cProc);
}

bool CTask::SetTimer(u32 dwTimerEvent, long dwMilliSeconds)
{
    CInstExt* pInst = (CInstExt*)GetInstance();
    if (pInst != NULL)
    {
        if (pInst->SetTaskTimer(dwTimerEvent, dwMilliSeconds, GetTaskNO()))
        {
            TASKLOG(OSPEXT, TIMER_LEV, "osp设置定时器成功[timerEvent:%s-%u, timeIneterval:%u]\n",
                OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
            return true;
        }
        else
        {
            TASKLOG(OSPEXT, ERROR_LEV, "osp设置定时器失败[timerEvent:%s-%u, timeIneterval:%u]\n",
                OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
            return false;
        }
    }
    else
    {
        TASKLOG(OSPEXT, ERROR_LEV, "pInst != NULL\n");
        return false;
    }  
}
    
bool CTask::KillTimer(u32 dwTimerEvent)
{
    CInstExt* pInst = (CInstExt*)GetInstance();
    if (pInst != NULL)
    {
        return (pInst->KillTaskTimer(dwTimerEvent, GetTaskNO()));
    }
    else
    {
        TASKLOG(OSPEXT, ERROR_LEV, "pInst != NULL\n");
        return false;
    } 
}

/************************************CTaskNO 类的实现*********************************/

CTaskNO::CTaskNO()
{
    m_dwTaskNOGenerator = 1;
}

u32 CTaskNO::GenTaskNO()
{
    //上锁
    CAutoLock cLock(m_cLock);

    u32 dwPollCnt = 0;
    u32 dwRetNO = INVALID_TASKNO;
    while ( dwPollCnt<=MAX_TASKNO )
    {
        if ( !m_cUsedTaskNOSet.Exist(m_dwTaskNOGenerator) )
        {
            dwRetNO = m_dwTaskNOGenerator;
            m_cUsedTaskNOSet.Insert(m_dwTaskNOGenerator);
            NextTaskNO();   //这里让m_wTaskNOGenerator不断自增, 防止任务号一直重复
            return dwRetNO;
        }

        NextTaskNO();
        dwPollCnt++;
    }
    
    return INVALID_TASKNO;
}

u32 CTaskNO::PeekNextTaskNO()
{
    //上锁
    CAutoLock cLock(m_cLock);

    //保证偷看前后CTaskNO各种状态不变
    u32 dwSaveTaskNOGenerator = m_dwTaskNOGenerator;

    u32 dwPollCnt = 0;
    u32 dwRetNO = INVALID_TASKNO;
    while ( dwPollCnt<MAX_TASKNO )
    {
        if ( !m_cUsedTaskNOSet.Exist(m_dwTaskNOGenerator) )
        {
            dwRetNO = m_dwTaskNOGenerator;
            break;
        }

        NextTaskNO();
        dwPollCnt++;
    }

    m_dwTaskNOGenerator = dwSaveTaskNOGenerator;

    return dwRetNO;
}

void CTaskNO::RevokeTaskNO(const u32 dwTaskNO)
{
    //上锁
    CAutoLock cLock(m_cLock);

    if ( dwTaskNO <= MAX_TASKNO )
    {
        m_cUsedTaskNOSet.Erase(dwTaskNO);
    }
}

u32 CTaskNO::NextTaskNO()
{
    m_dwTaskNOGenerator = ++m_dwTaskNOGenerator % MAX_TASKNO;
    if ( INVALID_TASKNO == m_dwTaskNOGenerator )
    {
        ++m_dwTaskNOGenerator;
    }
    return m_dwTaskNOGenerator;
}



/************************************CTaskManager 类的实现*********************************/

//CTaskNO CTaskManager::m_tTaskNOPool;       //空闲任务号池

void CTaskManager::InsertTaskToMap(const u32 dwTaskNO, LPTask lpTask)
{
    if ( INVALID_TASKNO == dwTaskNO || NULL == lpTask )
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "INVALID_TASKNO == wTaskNO || NULL == lpTask\n");
        return;
    }
    m_tTaskPtrMap.Insert(dwTaskNO, lpTask);
}

BOOL32 CTaskManager::FindTaskOnMap(const u32 dwTaskNO, LPTask &lpTask)
{
    if ( INVALID_TASKNO == dwTaskNO)
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "INVALID_TASKNO == wTaskNO\n");
        lpTask = NULL;
        return FALSE;
    }
    return m_tTaskPtrMap.Find( dwTaskNO, lpTask );
}
void CTaskManager::EraseTaskFromMap(const u32 dwTaskNO)
{
    m_tTaskPtrMap.Erase(dwTaskNO);
}

CTaskManager::CTaskManager()
{
    m_dwTaskCheckRef = 0;
}

BOOL32 CTaskManager::RegisterTask( CTask *pcTask )
{
    if ( NULL == pcTask )
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "NULL == pcTask\n");
        return FALSE;
    }

    pcTask->SetTaskNO(CTaskManager::GenerateTaskNO());
    if ( INVALID_TASKNO == pcTask->GetTaskNO() )
    {
        CLASSLOG(OSPEXT, ERROR_LEV, " 注册的任务号为空\n");
        return FALSE;
    }

    if (FindTask(pcTask->GetTaskNO()) != NULL)
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "注册的任务的任务号已经存在\n");
        return FALSE;
    }

    InsertTaskToMap(pcTask->GetTaskNO(), pcTask);

    //默认是将task添加到轮询列表中的
    AddTaskToPollMap(pcTask->GetTaskNO());

    return TRUE;
}

LPTask CTaskManager::FindTask(const u32 dwTaskNO)
{
    if ( INVALID_TASKNO == dwTaskNO )
    {
        CLASSLOG(OSPEXT, ALL_LEV, "INVALID_TASKNO == wTaskNO\n");
        return NULL;
    }

    LPTask lpTask = NULL;
    if ( FindTaskOnMap( dwTaskNO, lpTask ) )
    {
        return lpTask;
    }

    return NULL;
}

/*
 * 这里要注意一点, 不能主动去Delete CTask指针, 否则会产生访问野指针的问题
 */
void CTaskManager::UnRegisterTask(CTask * pcTask)
{
    if ( NULL == pcTask )
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "NULL == pcTask\n");
        return;
    }

    EraseTaskFromMap(pcTask->GetTaskNO());

    //清除task时, 轮询列表也要清除
    EraseTaskFromPollMap(pcTask->GetTaskNO());

    if ( INVALID_TASKNO != pcTask->GetTaskNO() )
    {
        CTaskManager::RevokeTaskNO(pcTask->GetTaskNO());
    }

    //由用户自己delete pcTask
}

u32 CTaskManager::GenerateTaskNO()
{
    return m_tTaskNOPool.GenTaskNO();
}

void CTaskManager::RevokeTaskNO(const u32 dwTaskNO)
{
    m_tTaskNOPool.RevokeTaskNO(dwTaskNO);
}

u32 CTaskManager::PeekTaskNO()
{
    return m_tTaskNOPool.PeekNextTaskNO();
}

void CTaskManager::OnTimerPollTasks()
{
	//2019/1/15 zhusenyong走读 这份拷贝是个大买卖，有什么必要吗？？？
    TTaskPtrMap tSaveTaskList = m_tPollTaskPtrMap;

    u32 dwCheckCount = 0;

    Iterator pPos;
    while (!pPos.End())
    {
        u32 dwTaskNO = INVALID_TASKNO;
        LPTask pTask = NULL;
        if (tSaveTaskList.Iterate(pPos, dwTaskNO, pTask))
        {
            if (dwTaskNO == INVALID_TASKNO || pTask == NULL)
            {
                CLASSLOG(OSPEXT, ERROR_LEV, "无效任务号[%u]或无效任务指针[%p]\n",
                    dwTaskNO, pTask);
                continue;
            }
			//2019/1/15 zhusenyong走读 这里不是多余的吗？？
            if (!m_tPollTaskPtrMap.Exist(dwTaskNO))
            {
                continue;
            }

            if (pTask->GetCheckRef() > m_dwTaskCheckRef)
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "任务[%s]TaskNO[%u]轮询引用计数[%u]大于系统轮询引用计数[%u]，跳过该任务\n",
                    pTask->GetObjName(), pTask->GetTaskNO(), pTask->GetCheckRef(), m_dwTaskCheckRef);
                continue;
            }

            CLASSLOG(OSPEXT, TRIVIAL_LEV, "任务[%s]TaskNO[%u]开始轮询\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            u32 dwPollRet = pTask->TimerPoll();

            pTask->AddPollCounter();

            if ( TIMERPOLL_DONE == dwPollRet )
            {
                dwCheckCount++;
                pTask->SetCheckRef( m_dwTaskCheckRef + 1 );
                CLASSLOG(OSPEXT, TRIVIAL_LEV, "任务[%s]TaskNO[%u]被正确执行轮询\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            }
            else if ( TIMERPOLL_DEL == dwPollRet )
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "任务[%s]TaskNO[%u]需要被删除\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
                delete pTask;
                pTask = NULL;
            }
            else
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "任务[%s]TaskNO[%u]轮询结果未知\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            }

			if( dwCheckCount >= MAX_TASK_EACH_POLL_COUNT )
			{
				CLASSLOG(OSPEXT, TIMER_LEV, "单次轮询的业务执行次数大于[%u], 跳出轮询\n",
					MAX_TASK_EACH_POLL_COUNT);
				break;
			}

        }
    }

    //完成一次遍历
    if (pPos.End())
    {
        m_dwTaskCheckRef += 1;
        
        //如果大于最大检测引用计数，所有引用计数清零
        if (m_dwTaskCheckRef >= MAX_CHECK_TASK_REF_COUNT)
        {
            Iterator pPos1;
            while (!pPos1.End())
            {
                u32 dwTaskNO = INVALID_TASKNO;
                LPTask pTask = NULL;
                if (m_tPollTaskPtrMap.Iterate(pPos1, dwTaskNO, pTask))
                {
                    if (dwTaskNO == INVALID_TASKNO || pTask == NULL)
                    {
                        CLASSLOG(OSPEXT, ERROR_LEV, "无效任务号[%u]或无效任务指针[%p]\n", dwTaskNO, pTask);
                        continue;
                    }

                    pTask->SetCheckRef(0);
                }
            }

            m_dwTaskCheckRef = 0;
        }
    }
}

size_t CTaskManager::GetTaskNum()
{
    return (size_t)m_tTaskPtrMap.GetSize();
}

size_t CTaskManager::GetPollTaskNum()
{
    return (size_t)m_tPollTaskPtrMap.GetSize();
}

void CTaskManager::AddTaskToPollMap(u32 dwTaskNO)
{
    if (!m_tPollTaskPtrMap.Exist(dwTaskNO))
    {
        CTask* pcTask = FindTask(dwTaskNO);

        if (pcTask != NULL)
        {
            pcTask->SetCheckRef(0);
            m_tPollTaskPtrMap.Insert(dwTaskNO, pcTask);
        }
    }  
}

void CTaskManager::EraseTaskFromPollMap(u32 dwTaskNO)
{
    if (m_tPollTaskPtrMap.Exist(dwTaskNO))
    {
        m_tPollTaskPtrMap.Erase(dwTaskNO);
    }  
}

u32 CTaskManager::GetPollInterval()
{
    //计算任务被分为几组
    u32 dwGroupNum = GetPollTaskNum() / MAX_TASK_EACH_POLL_COUNT;
    u32 dwMod = GetPollTaskNum() % MAX_TASK_EACH_POLL_COUNT;

    if (dwMod > 0)
    {
        dwGroupNum++;
    }

    if (dwGroupNum == 0)
    {
        dwGroupNum = 1;
    }

    //根据组数计算轮询间隔
    u32 dwTaskPollInterval = TASK_POLL_INTERVAL / dwGroupNum;
    if (dwTaskPollInterval <= 1)
    {
        CLASSLOG(OSPEXT, WARNING_LEV, "当前处理的事务量[%u]过大，分组达到[%u], 可能出现异常\n", 
            GetPollTaskNum(), dwGroupNum);

        dwTaskPollInterval = 10;  //调整为10ms
    }

    return dwTaskPollInterval;
}

void CTaskManager::EmptyTask()
{
    /*修改事务列表删除的处理逻辑  fanxg-20120411
      1.先将task列表拷贝出一份，由于task列表中保存的u32和指针，性能上可以接受。
      2.遍历拷贝列表的元素，根据元素的key查找实际task列表的事务指针，找到的才进行删除。
      3.这样处理的原因：
           Iterate接口返回的下一个元素的迭代器pPos可能在删除当前事务时失效，这是由于
           事务会存在子事务列表，删除父事务时子事务也一并删除，而子事务和父事务是存放在同一个
           事务表中的，当被删除的子事务正好是迭代器pPos，那么pPos指向的元素已经失效，
           如果再用pPos访问事务列表时会造成崩溃。
    */
    /*
    Iterator pPos;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;
        if (m_tTaskPtrMap.Iterate(pPos, dwKey, pTask))
        {
            if (pTask != NULL)
            {
                delete pTask;
                pTask = NULL;
            }
        }
    }
    */
    

    TTaskPtrMap tSaveTaskList = m_tTaskPtrMap;
    Iterator pPos;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;

        //先在副本中遍历出key
        if (tSaveTaskList.Iterate(pPos, dwKey, pTask))
        {
            //根据key查找是否存在事务指针，副本中直接迭代出的pTask不被使用，因为它可能是无效的
            CTask* pRealTask = NULL;
            if (m_tTaskPtrMap.Find(dwKey, pRealTask) && pRealTask != NULL)
            {
                delete pRealTask;
                pRealTask = NULL;
            }
        }
    }

    m_tTaskPtrMap.Empty();
}

void CTaskManager::PrintAllTask()
{
    PrintTaskByType("CTask");
}


void CTaskManager::PrintTaskByType(LPCSTR szTaskType)
{
    if (szTaskType == NULL)
    {
        szTaskType = "CTask";
    }

    Iterator pPos;
    size_t i = 0;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;
        if (m_tTaskPtrMap.Iterate(pPos, dwKey, pTask))
        {
            if (pTask != NULL && pTask->IsTask(szTaskType))
            {
                i++;
                OspPrintf(TRUE, FALSE, "【%u】--- ", i);
                //打印事务列表时强制调用基类的PrintData接口，这样只会打印事务的基本信息
                pTask->CTask::PrintData();
                OspPrintf(TRUE, FALSE, "\n");                
            }
        }
    }
}

void CTaskManager::PrintPollTaskByType(LPCSTR szTaskType)
{
    if (szTaskType == NULL)
    {
        szTaskType = "CTask";
    }

    Iterator pPos;
    size_t i = 0;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;
        if (m_tPollTaskPtrMap.Iterate(pPos, dwKey, pTask))
        {
            if (pTask != NULL && pTask->IsTask(szTaskType))
            {
                i++;
                OspPrintf(TRUE, FALSE, "【%u】--- ", i);
                //打印事务列表时强制调用基类的PrintData接口，这样只会打印事务的基本信息
                pTask->CTask::PrintData();
                OspPrintf(TRUE, FALSE, "\n");                
            }
        }
    }
}

void CTaskManager::PrintTaskByNO(u32 dwTaskNO)
{
    CTask* pTask = NULL;
    if (m_tTaskPtrMap.Find(dwTaskNO, pTask))
    {
        if (pTask != NULL)
        {
            //打印指定事务时直接调用虚接口PrintData，如果子类覆盖了PrintData的实现，
            //就会打印子类的数据。如果子类也想打印一下事务的基本信息可以这样实现：
            /*
               CXXXTask::PrintData()
               {
                  //基本信息
                   CTask::PrintData();

                   //这里实现子类自己的打印
                   //......
               }
            */
            pTask->PrintData();
        }
    }
    else
    {
        OspPrintf(TRUE, FALSE, "\n**************不存在任务号为[%u]任务**************\n", dwTaskNO);
    }    
}

CXMap<u32, CTask*> CTaskManager::GetTasksMap(LPCSTR szTaskType)
{
    if (szTaskType == NULL)
    {
        szTaskType = "CTask";
    }

    CXMap<u32, CTask*> tTasksMap;

    Iterator pPos;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;
        if (m_tTaskPtrMap.Iterate(pPos, dwKey, pTask))
        {
            if (pTask != NULL && pTask->IsTask(szTaskType))
            {
                tTasksMap.Insert(pTask->GetTaskNO(), pTask);        
            }
        }
    }

    return tTasksMap;
}

vector<CTask*> CTaskManager::GetTasksArray(LPCSTR szTaskType)
{
    if (szTaskType == NULL)
    {
        szTaskType = "CTask";
    }
    
    vector<CTask*> tTasksArray;
    
    Iterator pPos;
    while(!pPos.End())
    {
        u32 dwKey = 0;
        CTask* pTask = NULL;
        if (m_tTaskPtrMap.Iterate(pPos, dwKey, pTask))
        {
            if (pTask != NULL && pTask->IsTask(szTaskType))
            {
                tTasksArray.push_back(pTask);        
            }
        }
    }
    
    return tTasksArray;
}

//CTaskTimerManager的实现
u16 CTaskTimerManager::GenTaskTimerID(const TTaskTimerInfo& tUserTimerInfo)
{
    //1. 生成内部task-timerID
    u16 wInnerTimerID = INVALID_TASK_TIMER_ID;
    if (m_cUserTimerIDMapInnerTimer.Find(tUserTimerInfo, wInnerTimerID))
    {
        //该task已经存在相同定时器，覆盖之前的定时器
        CLASSLOG(OSPEXT, WARNING_LEV, "Task[%u]-UserTimerID[%s-%u],InnerTimerID[%u]已经存在，覆盖之前的定时器\n",
            tUserTimerInfo.dwTaskNO, OspExtEventDesc(tUserTimerInfo.wUserTimerID).c_str(),
            tUserTimerInfo.wUserTimerID, wInnerTimerID);
    }
    else
    {
        //生成新的定时器
        wInnerTimerID = m_cInnerTimerIDPool.PopIndex();
        if (wInnerTimerID == CU16IndexPool::INVALID_INDEX)
        {
            CLASSLOG(OSPEXT, ERROR_LEV, "分配内部TIME_ID失败\n");
            return INVALID_TASK_TIMER_ID;
        }

        //2. 添加InnerTimerID和UserTimerInfo映射关系
        m_cInnerTimerIDMapUserTimer.Insert(wInnerTimerID, tUserTimerInfo);
        m_cUserTimerIDMapInnerTimer.Insert(tUserTimerInfo, wInnerTimerID);
    }

    return wInnerTimerID;
}

u16 CTaskTimerManager::RevokeTaskTimerID(u16 wInnerTimerID)
{
    //1. 回收内部task-timerID
    m_cInnerTimerIDPool.PushIndex(wInnerTimerID);

    //2. 获取UserTimerID
    TTaskTimerInfo tUserTimerInfo;
    if (m_cInnerTimerIDMapUserTimer.Find(wInnerTimerID, tUserTimerInfo))
    {
        //3. 删除InnerTimerID和UserTimerID映射关系
        m_cInnerTimerIDMapUserTimer.Erase(wInnerTimerID);
        m_cUserTimerIDMapInnerTimer.Erase(tUserTimerInfo);
    }
    else
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "InnerTimerID[%u]不存在对应的UserTimerID\n", wInnerTimerID);
    }

    return tUserTimerInfo.wUserTimerID;
}

u16 CTaskTimerManager::FindInnerTimerIDByUserTimer(const TTaskTimerInfo& tUserTimerInfo) const
{
    u16 wInnerTimerID = INVALID_TASK_TIMER_ID;
    m_cUserTimerIDMapInnerTimer.Find(tUserTimerInfo, wInnerTimerID);
    return wInnerTimerID;
}

/*static*/bool CTaskTimerManager::IsTaskTimerID(u16 wEvent)
{
    return (wEvent >= OSP_EXT_EVENT_TASK_TIMER_ID_BEGIN && wEvent <= OSP_EXT_EVENT_TASK_TIMER_ID_END);
}

void CTaskTimerManager::PrintData() const
{
    OspPrintf(TRUE, FALSE, "  事务定时器列表: \n");
    u32 i = 0;
    Iterator pPos;
    while (!pPos.End())
    {
        u16 wInnerTimerID = INVALID_TASK_TIMER_ID;
        TTaskTimerInfo tUserTimerInfo;
        if (m_cInnerTimerIDMapUserTimer.Iterate(pPos, wInnerTimerID, tUserTimerInfo))
        {
            i++;
            OspPrintf(TRUE, FALSE, "    [%u.]Task[%u]-UserTimerID[%s-%u],InnerTimerID[%u] \n",
                i, tUserTimerInfo.dwTaskNO, OspExtEventDesc(tUserTimerInfo.wUserTimerID).c_str(),
                tUserTimerInfo.wUserTimerID, wInnerTimerID);
        }
    }
}

