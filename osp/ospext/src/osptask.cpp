#include "cms/ospext/osptask.h"
#include "cms/ospext/ospdbg.h"
#include "cms/ospext/ospinst.h"
#include "taskmng.h"

extern void OspExtPrintf(TOspExtLogContent& tLogContent);

extern u32 g_dwTraceTaskNO;
extern u32 g_dwTraceAppInst;
extern CXMap<u32, TLogModInfo> g_cModuleLogLev;


/************************************CTask ���ʵ��*********************************/

CTask::CTask(CInstExt *pcInst) : COspObj()
{  
    m_dwTaskNO = INVALID_TASKNO;
    m_dwCurState = UNKNOWN_STATE;       //��ʼ��״̬
    m_dwCheckRef = 0;
    m_nPollCounter = -1;                //�����ʼֵ����Ϊ-1, ��ô�´�ֱ�Ӵ�0��ʼ����
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
    //����û���Ҫʵ���Լ�����ѯ��ʱ����, ���Զ������� u32 XXXStateTimerPoll()��������ע�ᵽRuleMap��
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
    TASKLOG(OSPEXT, ALL_LEV, "����[%u]��[%u]״̬תΪ[%u]״̬\n", 
        GetTaskNO(), GetState(), dwState);

    m_dwCurState = dwState;
    m_tCurStateCreateTime = OspGetSecByTicks();
    //ÿ���л�״̬˵���յ����µ���Ϣ, ��Ҫ����ѯ�������
    ClearPollCounter();
  
    if (IsNotNeedPollState())
    {
        //Ŀ��״̬����Ҫ��ѯ
        GetInstance()->EraseTaskFromPollMap(GetTaskNO());
    }
    else
    {
        //Ŀ��״̬��Ҫ��ѯ
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
    if (g_dwTraceAppInst != 0                  //���ڸ���ĳ��app-inst
        && g_dwTraceAppInst != dwCurrAppInst)  //��ǰ���Ǹ��ٵ�app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0     //���ڸ���ĳ������
        && g_dwTraceTaskNO != GetTaskNO())    //��ǰ���Ǹ��ٵ�����
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
    if (g_dwTraceAppInst != 0                  //���ڸ���ĳ��app-inst
        && g_dwTraceAppInst != dwCurrAppInst)  //��ǰ���Ǹ��ٵ�app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0     //���ڸ���ĳ������
        && g_dwTraceTaskNO != GetTaskNO())    //��ǰ���Ǹ��ٵ�����
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
            TASKLOG(OSPEXT, TIMER_LEV, "osp���ö�ʱ���ɹ�[timerEvent:%s-%u, timeIneterval:%u]\n",
                OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
            return true;
        }
        else
        {
            TASKLOG(OSPEXT, ERROR_LEV, "osp���ö�ʱ��ʧ��[timerEvent:%s-%u, timeIneterval:%u]\n",
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

/************************************CTaskNO ���ʵ��*********************************/

CTaskNO::CTaskNO()
{
    m_dwTaskNOGenerator = 1;
}

u32 CTaskNO::GenTaskNO()
{
    //����
    CAutoLock cLock(m_cLock);

    u32 dwPollCnt = 0;
    u32 dwRetNO = INVALID_TASKNO;
    while ( dwPollCnt<=MAX_TASKNO )
    {
        if ( !m_cUsedTaskNOSet.Exist(m_dwTaskNOGenerator) )
        {
            dwRetNO = m_dwTaskNOGenerator;
            m_cUsedTaskNOSet.Insert(m_dwTaskNOGenerator);
            NextTaskNO();   //������m_wTaskNOGenerator��������, ��ֹ�����һֱ�ظ�
            return dwRetNO;
        }

        NextTaskNO();
        dwPollCnt++;
    }
    
    return INVALID_TASKNO;
}

u32 CTaskNO::PeekNextTaskNO()
{
    //����
    CAutoLock cLock(m_cLock);

    //��֤͵��ǰ��CTaskNO����״̬����
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
    //����
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



/************************************CTaskManager ���ʵ��*********************************/

//CTaskNO CTaskManager::m_tTaskNOPool;       //��������ų�

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
        CLASSLOG(OSPEXT, ERROR_LEV, " ע��������Ϊ��\n");
        return FALSE;
    }

    if (FindTask(pcTask->GetTaskNO()) != NULL)
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "ע��������������Ѿ�����\n");
        return FALSE;
    }

    InsertTaskToMap(pcTask->GetTaskNO(), pcTask);

    //Ĭ���ǽ�task��ӵ���ѯ�б��е�
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
 * ����Ҫע��һ��, ��������ȥDelete CTaskָ��, ������������Ұָ�������
 */
void CTaskManager::UnRegisterTask(CTask * pcTask)
{
    if ( NULL == pcTask )
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "NULL == pcTask\n");
        return;
    }

    EraseTaskFromMap(pcTask->GetTaskNO());

    //���taskʱ, ��ѯ�б�ҲҪ���
    EraseTaskFromPollMap(pcTask->GetTaskNO());

    if ( INVALID_TASKNO != pcTask->GetTaskNO() )
    {
        CTaskManager::RevokeTaskNO(pcTask->GetTaskNO());
    }

    //���û��Լ�delete pcTask
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
	//2019/1/15 zhusenyong�߶� ��ݿ����Ǹ�����������ʲô��Ҫ�𣿣���
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
                CLASSLOG(OSPEXT, ERROR_LEV, "��Ч�����[%u]����Ч����ָ��[%p]\n",
                    dwTaskNO, pTask);
                continue;
            }
			//2019/1/15 zhusenyong�߶� ���ﲻ�Ƕ�����𣿣�
            if (!m_tPollTaskPtrMap.Exist(dwTaskNO))
            {
                continue;
            }

            if (pTask->GetCheckRef() > m_dwTaskCheckRef)
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "����[%s]TaskNO[%u]��ѯ���ü���[%u]����ϵͳ��ѯ���ü���[%u]������������\n",
                    pTask->GetObjName(), pTask->GetTaskNO(), pTask->GetCheckRef(), m_dwTaskCheckRef);
                continue;
            }

            CLASSLOG(OSPEXT, TRIVIAL_LEV, "����[%s]TaskNO[%u]��ʼ��ѯ\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            u32 dwPollRet = pTask->TimerPoll();

            pTask->AddPollCounter();

            if ( TIMERPOLL_DONE == dwPollRet )
            {
                dwCheckCount++;
                pTask->SetCheckRef( m_dwTaskCheckRef + 1 );
                CLASSLOG(OSPEXT, TRIVIAL_LEV, "����[%s]TaskNO[%u]����ȷִ����ѯ\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            }
            else if ( TIMERPOLL_DEL == dwPollRet )
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "����[%s]TaskNO[%u]��Ҫ��ɾ��\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
                delete pTask;
                pTask = NULL;
            }
            else
            {
                CLASSLOG(OSPEXT, TIMER_LEV, "����[%s]TaskNO[%u]��ѯ���δ֪\n",
                    pTask->GetObjName(), pTask->GetTaskNO());
            }

			if( dwCheckCount >= MAX_TASK_EACH_POLL_COUNT )
			{
				CLASSLOG(OSPEXT, TIMER_LEV, "������ѯ��ҵ��ִ�д�������[%u], ������ѯ\n",
					MAX_TASK_EACH_POLL_COUNT);
				break;
			}

        }
    }

    //���һ�α���
    if (pPos.End())
    {
        m_dwTaskCheckRef += 1;
        
        //���������������ü������������ü�������
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
                        CLASSLOG(OSPEXT, ERROR_LEV, "��Ч�����[%u]����Ч����ָ��[%p]\n", dwTaskNO, pTask);
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
    //�������񱻷�Ϊ����
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

    //��������������ѯ���
    u32 dwTaskPollInterval = TASK_POLL_INTERVAL / dwGroupNum;
    if (dwTaskPollInterval <= 1)
    {
        CLASSLOG(OSPEXT, WARNING_LEV, "��ǰ�����������[%u]���󣬷���ﵽ[%u], ���ܳ����쳣\n", 
            GetPollTaskNum(), dwGroupNum);

        dwTaskPollInterval = 10;  //����Ϊ10ms
    }

    return dwTaskPollInterval;
}

void CTaskManager::EmptyTask()
{
    /*�޸������б�ɾ���Ĵ����߼�  fanxg-20120411
      1.�Ƚ�task�б�����һ�ݣ�����task�б��б����u32��ָ�룬�����Ͽ��Խ��ܡ�
      2.���������б��Ԫ�أ�����Ԫ�ص�key����ʵ��task�б������ָ�룬�ҵ��ĲŽ���ɾ����
      3.���������ԭ��
           Iterate�ӿڷ��ص���һ��Ԫ�صĵ�����pPos������ɾ����ǰ����ʱʧЧ����������
           ���������������б�ɾ��������ʱ������Ҳһ��ɾ������������͸������Ǵ����ͬһ��
           ������еģ�����ɾ���������������ǵ�����pPos����ôpPosָ���Ԫ���Ѿ�ʧЧ��
           �������pPos���������б�ʱ����ɱ�����
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

        //���ڸ����б�����key
        if (tSaveTaskList.Iterate(pPos, dwKey, pTask))
        {
            //����key�����Ƿ��������ָ�룬������ֱ�ӵ�������pTask����ʹ�ã���Ϊ����������Ч��
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
                OspPrintf(TRUE, FALSE, "��%u��--- ", i);
                //��ӡ�����б�ʱǿ�Ƶ��û����PrintData�ӿڣ�����ֻ���ӡ����Ļ�����Ϣ
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
                OspPrintf(TRUE, FALSE, "��%u��--- ", i);
                //��ӡ�����б�ʱǿ�Ƶ��û����PrintData�ӿڣ�����ֻ���ӡ����Ļ�����Ϣ
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
            //��ӡָ������ʱֱ�ӵ�����ӿ�PrintData��������า����PrintData��ʵ�֣�
            //�ͻ��ӡ��������ݡ��������Ҳ���ӡһ������Ļ�����Ϣ��������ʵ�֣�
            /*
               CXXXTask::PrintData()
               {
                  //������Ϣ
                   CTask::PrintData();

                   //����ʵ�������Լ��Ĵ�ӡ
                   //......
               }
            */
            pTask->PrintData();
        }
    }
    else
    {
        OspPrintf(TRUE, FALSE, "\n**************�����������Ϊ[%u]����**************\n", dwTaskNO);
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

//CTaskTimerManager��ʵ��
u16 CTaskTimerManager::GenTaskTimerID(const TTaskTimerInfo& tUserTimerInfo)
{
    //1. �����ڲ�task-timerID
    u16 wInnerTimerID = INVALID_TASK_TIMER_ID;
    if (m_cUserTimerIDMapInnerTimer.Find(tUserTimerInfo, wInnerTimerID))
    {
        //��task�Ѿ�������ͬ��ʱ��������֮ǰ�Ķ�ʱ��
        CLASSLOG(OSPEXT, WARNING_LEV, "Task[%u]-UserTimerID[%s-%u],InnerTimerID[%u]�Ѿ����ڣ�����֮ǰ�Ķ�ʱ��\n",
            tUserTimerInfo.dwTaskNO, OspExtEventDesc(tUserTimerInfo.wUserTimerID).c_str(),
            tUserTimerInfo.wUserTimerID, wInnerTimerID);
    }
    else
    {
        //�����µĶ�ʱ��
        wInnerTimerID = m_cInnerTimerIDPool.PopIndex();
        if (wInnerTimerID == CU16IndexPool::INVALID_INDEX)
        {
            CLASSLOG(OSPEXT, ERROR_LEV, "�����ڲ�TIME_IDʧ��\n");
            return INVALID_TASK_TIMER_ID;
        }

        //2. ���InnerTimerID��UserTimerInfoӳ���ϵ
        m_cInnerTimerIDMapUserTimer.Insert(wInnerTimerID, tUserTimerInfo);
        m_cUserTimerIDMapInnerTimer.Insert(tUserTimerInfo, wInnerTimerID);
    }

    return wInnerTimerID;
}

u16 CTaskTimerManager::RevokeTaskTimerID(u16 wInnerTimerID)
{
    //1. �����ڲ�task-timerID
    m_cInnerTimerIDPool.PushIndex(wInnerTimerID);

    //2. ��ȡUserTimerID
    TTaskTimerInfo tUserTimerInfo;
    if (m_cInnerTimerIDMapUserTimer.Find(wInnerTimerID, tUserTimerInfo))
    {
        //3. ɾ��InnerTimerID��UserTimerIDӳ���ϵ
        m_cInnerTimerIDMapUserTimer.Erase(wInnerTimerID);
        m_cUserTimerIDMapInnerTimer.Erase(tUserTimerInfo);
    }
    else
    {
        CLASSLOG(OSPEXT, ERROR_LEV, "InnerTimerID[%u]�����ڶ�Ӧ��UserTimerID\n", wInnerTimerID);
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
    OspPrintf(TRUE, FALSE, "  ����ʱ���б�: \n");
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

