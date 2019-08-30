
#include "ospexevt.h"
#include "taskmng.h"

//
extern u32 g_dwTraceAppInst;
extern u32 g_dwTraceTaskNO;
extern CXMap<u32, TLogModInfo> g_cModuleLogLev;
extern void OspExtPrintf(TOspExtLogContent& tLogContent);
extern BOOL32 g_bIsMonitorTimePerform;

CInstExt::CInstExt()
{
    ClearData();

    m_pTaskMng = new CTaskManager;

    m_pTaskTimerMng = new CTaskTimerManager;
}

CInstExt::~CInstExt()
{
    ClearData();
    
    if (m_pTaskMng != NULL)
    {
        m_pTaskMng->EmptyTask();

        delete m_pTaskMng;
        m_pTaskMng = NULL;
    }

    if (m_pTaskTimerMng != NULL)
    {
        delete m_pTaskTimerMng;
        m_pTaskTimerMng = NULL;
    }
}

void CInstExt::ClearData()
{
#ifdef DEBUG
    m_byLogLev = EVENT_LEV;
#else
    m_byLogLev = ERROR_LEV;
#endif
    
    m_pApp = NULL;

    m_bTimerIsRunning = FALSE;
}

LPCSTR CInstExt::GetStrApp() const
{
    if (m_pApp != NULL)
    {
        return m_pApp->NameGet();
    }
    
    return "Unknow App Name";
}

CApp* CInstExt::GetAppPt() const
{
    return m_pApp;
}

u32 CInstExt::CurState() const
{
    return m_curState;
}

u16 CInstExt::GetAppID() const
{
    return m_appId;
}

u16 CInstExt::GetInsID() const
{
    return m_instId;
}

void CInstExt::SetAppPt(CApp* pApp)
{
    m_pApp = pApp;
}

void CInstExt::Log(u8 byLogLevel, const char* szFormat, ...) const
{
    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;

    u32 dwCurrAppInst = MAKEIID(GetAppID(), GetInsID());
    if (g_dwTraceAppInst != 0  //正在跟踪某个app-inst
        && g_dwTraceAppInst != dwCurrAppInst)    //当前不是跟踪的app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (m_byLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    char szModLev[256] = {0};
    sprintf(szModLev, "[%s]: ", GetStrLogLev(byLogLevel).c_str());
    tLogContent.strModLev = szModLev;

    char szOspPrefix[256] = {0};
    sprintf(szOspPrefix, "App[%s-%u]-inst[%u][%s]: ", GetStrApp(), GetAppID(), GetInsID(), GetStrState());
    tLogContent.strOspPrifixField = szOspPrefix;

    OspExtPrintf(tLogContent);
}

void CInstExt::Log(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...) const
{
    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;

    u32 dwCurrAppInst = MAKEIID(GetAppID(), GetInsID());
    if (g_dwTraceAppInst != 0  //正在跟踪某个app-inst
        && g_dwTraceAppInst != dwCurrAppInst)    //当前不是跟踪的app-inst
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (m_byLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_cModuleLogLev[byLogMod].dwLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    char szModLev[256] = {0};
    sprintf(szModLev, "[%s][%s]: ", g_cModuleLogLev[byLogMod].strModName.c_str(), GetStrLogLev(byLogLevel).c_str());
    tLogContent.strModLev = szModLev;

    char szOspPrefix[256] = {0};
    sprintf(szOspPrefix, "App[%s-%u]-inst[%u][%s]: ", GetStrApp(), GetAppID(), GetInsID(), GetStrState());
    tLogContent.strOspPrifixField = szOspPrefix;

    OspExtPrintf(tLogContent);
}

u32 CInstExt::GetOspIID() const
{
    return MAKEIID(GetAppID(), GetInsID());
}

void CInstExt::DaemonInstanceEntry(CMessage *const pcMsg, CApp* pcApp)
{
    INSTLOG(OSPEXT, TRIVIAL_LEV, "Receive Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    if (OnInnerMsg(pcMsg) == TRUE)
    {
        return;
    }

    //将事务定时器的innerTimerID映射为userTimerID
    if (m_pTaskTimerMng->IsTaskTimerID(pcMsg->event))
    {
        pcMsg->event = m_pTaskTimerMng->RevokeTaskTimerID(pcMsg->event);
		if (INVALID_TASK_TIMER_ID == pcMsg->event)
		{
			//无效的EVENT_ID不需要返回给上层
			return;
		}
    }

    //事件的执行时间，用于考察性能
    if (g_bIsMonitorTimePerform == TRUE)
    {
        TMsTime tStartMsTime = OspGetMsTime();
        DaemonEntry(pcMsg);
        TMsTime tEndMsTime = OspGetMsTime();

        u32 dwInterval = OspGetDiffMs(tEndMsTime, tStartMsTime);
        if (dwInterval > 30)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "处理Msg[%s-%hu]严重耗时，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else if (dwInterval > 10)
        {
            INSTLOG(OSPEXT, WARNING_LEV, "处理Msg[%s-%hu]比较耗时，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else
        {
            INSTLOG(OSPEXT, EVENT_LEV, "处理Msg[%s-%hu]正常，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }  
    }
    else
    {
        DaemonEntry(pcMsg);
    }

    UNREFERENCED_PARAMETER(pcApp);
}

BOOL32 CInstExt::PostMsg(u16 wMsgId, const void* pMsgBuf, u16 wMsgLen, u32 dwDstIId, u32 dwDstNode)
{
    //如果 dwDstNode = 0, dwDstIId = 0, 消息发给本实例
    if (dwDstNode == SELF_NID && dwDstIId == SELF_IID)
    {
        dwDstIId = MAKEIID(GetAppID(), GetInsID());
    }

    if (OSP_OK != post(dwDstIId, wMsgId, pMsgBuf, wMsgLen, dwDstNode))
    {
        INSTLOG(OSPEXT, ERROR_LEV, "Send %u bytes Msg[%s-%u] to node[%u], app[%u], inst[%u] fail!\n",
            wMsgLen, OspExtEventDesc(wMsgId).c_str(), wMsgId, 
            dwDstNode, GETAPP(dwDstIId), GETINS(dwDstIId));
        return FALSE;
    }
    else
    {
        INSTLOG(OSPEXT, ALL_LEV, "Send %u bytes Msg[%s-%u] to node[%u], app[%u], inst[%u] successfully!\n",
            wMsgLen, OspExtEventDesc(wMsgId).c_str(), wMsgId, 
            dwDstNode, GETAPP(dwDstIId), GETINS(dwDstIId));
        return TRUE;
    }
}

void CInstExt::ProcTaskMsg(CTask* pTask, CMessage* const pcMsg)
{
    if (pTask == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "pTask == NULL---msg[%s-%u]\n", 
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        return;
    }

    u32 dwRet = pTask->ProcMsg(pcMsg);
    if (dwRet == PROCMSG_DEL)
    {
        INSTLOG(OSPEXT, ALL_LEV, "事务[%s-%u]处理后需要删除---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        delete pTask;
        pTask = NULL;
    }
    else if (dwRet == PROCMSG_FAIL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "事务[%s-%u]处理失败---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else if (dwRet == PROCMSG_OK)
    {
        INSTLOG(OSPEXT, ALL_LEV, "事务[%s-%u]处理成功---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else
    {
        INSTLOG(OSPEXT, ERROR_LEV, "事务[%s-%u]处理结果未知---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
}

void CInstExt::DaemonEntry(CMessage* const pcMsg)
{    
    INSTLOG(OSPEXT, EVENT_LEV, "Receive Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
}

void CInstExt::InstanceEntry(CMessage *const pcMsg)
{
    INSTLOG(OSPEXT, TRIVIAL_LEV, "Receive Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    if (OnInnerMsg(pcMsg) == TRUE)
    {
        return;
    }

    //将事务定时器的innerTimerID映射为userTimerID
    if (m_pTaskTimerMng->IsTaskTimerID(pcMsg->event))
    {
        pcMsg->event = m_pTaskTimerMng->RevokeTaskTimerID(pcMsg->event);
		if (INVALID_TASK_TIMER_ID == pcMsg->event)
		{
			//无效的EVENT_ID不需要返回给上层
			return;
		}
    }


    //事件的执行时间，用于考察性能
    if (g_bIsMonitorTimePerform == TRUE)
    {
        TMsTime tStartMsTime = OspGetMsTime();
        NormalEntry(pcMsg);
        TMsTime tEndMsTime = OspGetMsTime();

        u32 dwInterval = OspGetDiffMs(tEndMsTime, tStartMsTime);
        if (dwInterval > 30)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "处理Msg[%s-%hu]严重耗时，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else if (dwInterval > 10)
        {
            INSTLOG(OSPEXT, WARNING_LEV, "处理Msg[%s-%hu]比较耗时，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else
        {
            INSTLOG(OSPEXT, EVENT_LEV, "处理Msg[%s-%hu]正常，需要[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }  
    }
    else
    {
        NormalEntry(pcMsg);
    } 
}

BOOL32 CInstExt::OnInnerMsg(CMessage* const pcMsg)
{
    BOOL32 bIsProcessMsg = TRUE;

    CApp* pApp = m_pApp;
    if (pApp == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]的app point is null\n",
                GetStrApp(), GetInsID());
        return FALSE;
    }

    switch(pcMsg->event)
    {
    case OSP_EXT_EVENT_SET_LOG_LEV:
        {
            u8 byLev = *(u8*)pcMsg->content;
            m_byLogLev = byLev;
            
            OspPrintf(TRUE, FALSE, "设置app[%s]-inst[%u]的打印级别为[%s-%d]\n",
                GetStrApp(), GetInsID(), GetStrLogLev(byLev).c_str(), byLev);
        }
        break;

    case OSP_EXT_EVENT_SET_APP_LOG_LEV:
        {
            if (GetInsID() == CInstance::DAEMON)
            {
                u8 byLev = *(u8*)pcMsg->content;

                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;
                    
                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }
                    
                    pInst = pApp->GetInstance(wInstId);
                    
                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    pInstExt->m_byLogLev = byLev;
                }

                OspPrintf(TRUE, FALSE, "设置app[%s]所有实例的打印级别为[%s-%d]\n",
                    GetStrApp(), GetStrLogLev(byLev).c_str(), byLev);
            }
        }
        break;
        
    case OSP_EXT_EVENT_POLL_TASK:
        {
            if (CurState() == 0)
            {
				// 这里如果跳出来了，那么定时器就再也没机会触发了，整个task框架的定时轮询机制就停了
				// 所以这里必须再次设定定时器 2019/1/15 zhusenyong 
				KillTimer(OSP_EXT_EVENT_POLL_TASK);
				SetTimer(OSP_EXT_EVENT_POLL_TASK, (long)m_pTaskMng->GetPollInterval());
                INSTLOG(OSPEXT, TIMER_LEV, "在IDLE状态收到事务轮询消息Msg[%s-%hu], 该状态不处理\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                break;
            }

            INSTLOG(OSPEXT, TRIVIAL_LEV, "Receive 事务轮询消息Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            KillTimer(OSP_EXT_EVENT_POLL_TASK);
            
            if (GetPollTaskNum() > 0)
            {
                OnTimerPollTasks();
                SetTimer(OSP_EXT_EVENT_POLL_TASK, (long)m_pTaskMng->GetPollInterval());
                m_bTimerIsRunning = TRUE;
            }
            else
            {
                INSTLOG(OSPEXT, TIMER_LEV, "Task num == 0,暂停轮询定时器\n");
                m_bTimerIsRunning = FALSE;
                break;
            }            
        }
        break;

    case OSP_EXT_EVENT_PRINT_APP_TASK:
        {
			LPCSTR lpszTaskType = (LPCSTR)pcMsg->content;

            if (GetInsID() == CInstance::DAEMON)
            {
                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;
                    
                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }
                    
                    pInst = pApp->GetInstance(wInstId);
                    
                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    pInstExt->PrintTask(lpszTaskType);
                }
            }
        }
        break;

    case OSP_EXT_EVENT_PRINT_INST_TASK:
        {
			LPCSTR lpszTaskType = (LPCSTR)pcMsg->content;
            PrintTask(lpszTaskType);        
        }
        break;

    case OSP_EXT_EVENT_PRINT_APP_POLL_TASK:
        {
            LPCSTR lpszTaskType = (LPCSTR)pcMsg->content;

            if (GetInsID() == CInstance::DAEMON)
            {
                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;

                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }

                    pInst = pApp->GetInstance(wInstId);

                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    pInstExt->PrintPollTask(lpszTaskType);
                }
            }
        }
        break;

    case OSP_EXT_EVENT_PRINT_INST_POLL_TASK:
        {
            LPCSTR lpszTaskType = (LPCSTR)pcMsg->content;
            PrintPollTask(lpszTaskType);        
        }
        break;
        
    case OSP_EXT_EVENT_PRINT_ONE_TASK:
        {
            u32 dwTaskNO = *(u32*)pcMsg->content;
            BOOL32 bFind = FALSE;
            
            if (GetInsID() == CInstance::DAEMON)
            {
                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;
                    
                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }
                    
                    pInst = pApp->GetInstance(wInstId);
                    
                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CTask* pTask = pInstExt->FindTask(dwTaskNO);
                    if (pTask)
                    {
                        pTask->PrintData();
                        bFind = TRUE;
                        break;
                    }
                }
            }
            else
            {
                CTask* pTask = FindTask(dwTaskNO);
                if (pTask)
                {
                    pTask->PrintData();
                    bFind = TRUE;
                }
            }

            if (bFind == FALSE)
            {
                INSTLOG(OSPEXT, ERROR_LEV, "不存在TaskNO[%u]的事务\n", dwTaskNO);
            }
        }
        break;

    case OSP_EXT_EVENT_KILL_TASK:
        {
            u32 dwTaskNO = *(u32*)pcMsg->content;
            BOOL32 bFind = FALSE;

            if (GetInsID() == CInstance::DAEMON)
            {
                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;

                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }

                    pInst = pApp->GetInstance(wInstId);

                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    CTask* pTask = pInstExt->FindTask(dwTaskNO);
                    if (pTask)
                    {
                        delete pTask;
                        pTask = NULL;
                        bFind = TRUE;
                        break;
                    }
                }
            }
            else
            {
                CTask* pTask = FindTask(dwTaskNO);
                if (pTask)
                {
                    delete pTask;
                    pTask = NULL;
                    bFind = TRUE;
                }
            }

            if (bFind == FALSE)
            {
                INSTLOG(OSPEXT, ERROR_LEV, "不存在TaskNO[%u]的事务\n", dwTaskNO);
            }
        }
        break;

    case OSP_EXT_EVENT_TRACE_INST:
        {
            u16 wTraceInstId = *(u16*)pcMsg->content;
            
            if (GetInsID() == CInstance::DAEMON)
            {
                //查找所有实例            
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                for (u16 i=0; i<=wInstNum; i++)
                {
                    CInstance* pInst = NULL;
                    
                    u16 wInstId = CInstance::INVALID;
                    if (i==0)
                    {
                        wInstId = CInstance::DAEMON;            
                    }
                    else
                    {
                        wInstId = i;
                    }
                
                    pInst = pApp->GetInstance(wInstId);
                    
                    if (pInst == NULL)
                    {
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        continue;
                    }

                    if (wInstId == wTraceInstId)
                    {
                        pInstExt->m_byLogLev = ALL_LEV;
                    }
                    else
                    {
                        pInstExt->m_byLogLev = NOLOG_LEV;
                    }                    
                }
            }
        }
        break;

    case OSP_EXT_EVENT_PRINT_INST_STATE:
        {
            if (GetInsID() == CInstance::DAEMON)
            {            
                //查找所有实例                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                OspPrintf(TRUE, FALSE, "\napp[%s-%u]共有[%u]个普通实例, 状态如下:\n", 
                    GetStrApp(), GetAppID(), wInstNum);

                OspPrintf(TRUE, FALSE, "    DAEMON实例状态: [%s], tasknum[%u], polltasknum[%u], PollTimer[run:%d, time:%ums, count:%u], loglev[%s-%u]\n", 
                    GetStrState(), GetTaskNum(), GetPollTaskNum(), m_bTimerIsRunning, m_pTaskMng->GetPollInterval(), 
                    MAX_TASK_EACH_POLL_COUNT, GetStrLogLev(m_byLogLev).c_str(), m_byLogLev);

                m_pTaskTimerMng->PrintData();

                u16 wIdleNum = 0;
                u16 i=0;
                for (i=0; i<wInstNum; i++)
                {
                    CInstance* pInst = NULL;
                    
                    u16 wInstId = i+1;
                    
                    pInst = pApp->GetInstance(wInstId);
                    
                    if (pInst == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    if (pInstExt->CurState() != 0)
                    {
                        OspPrintf(TRUE, FALSE, "    实例[%u]状态: [%s], tasknum[%u], polltasknum[%u], PollTimer[run:%d, time:%ums, count:%u], loglev[%s-%u]\n", 
                            pInstExt->GetInsID(), pInstExt->GetStrState(), GetTaskNum(), GetPollTaskNum(),
                            pInstExt->m_bTimerIsRunning, pInstExt->m_pTaskMng->GetPollInterval(), MAX_TASK_EACH_POLL_COUNT,
                            GetStrLogLev(pInstExt->m_byLogLev).c_str(), pInstExt->m_byLogLev);

                        pInstExt->m_pTaskTimerMng->PrintData();
                    }
                    else
                    {
                        wIdleNum++;
                    }
                }

                OspPrintf(TRUE, FALSE, "    空闲实例数[%u]个\n", wIdleNum);
            }
        }
        break;
        
    default:
        {
            bIsProcessMsg = OnUserInnerCommonMsg(pcMsg);
        }
        break;
    }
    
    return bIsProcessMsg;
}

BOOL32 CInstExt::AddTask(CTask* pTask)
{
    if (pTask == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "pTask == NULL\n");
        return FALSE;
    }

    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return FALSE;
    }

    if (m_pTaskMng->RegisterTask(pTask) == FALSE)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "RegisterTask() Fail\n");
        return FALSE;
    }

    if (m_bTimerIsRunning == FALSE && GetPollTaskNum() > 0)
    {
        SetTimer(OSP_EXT_EVENT_POLL_TASK, (long)m_pTaskMng->GetPollInterval());

        m_bTimerIsRunning = TRUE;
    }

    INSTLOG(OSPEXT, EVENT_LEV, "Add Task, TaskNO [%u]\n", /*pTask->GetObjName(), */pTask->GetTaskNO());
    
    return TRUE;
}

BOOL32 CInstExt::DelTask(CTask* pTask)
{
    if (pTask == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "pTask == NULL\n");
        return FALSE;
    }

    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return FALSE;
    }

    m_pTaskMng->UnRegisterTask(pTask);

    if (GetPollTaskNum() == 0 && m_bTimerIsRunning == TRUE)
    {
        m_bTimerIsRunning = FALSE;
    }

    INSTLOG(OSPEXT, EVENT_LEV, "Del Task, TaskNO [%u]\n", /*pTask->GetObjName(), */pTask->GetTaskNO());

    return TRUE;
}

void CInstExt::EmptyTask()
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return ;
    }
    
    m_pTaskMng->EmptyTask();

    if (m_bTimerIsRunning == TRUE)
    {
        m_bTimerIsRunning = FALSE;
    }
}

void CInstExt::PrintTask(LPCSTR szTaskType) const
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return ;
    }
    
	m_pTaskMng->PrintTaskByType(szTaskType);
}

void  CInstExt::PrintPollTask(LPCSTR szTaskType) const
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return ;
    }

    m_pTaskMng->PrintPollTaskByType(szTaskType);
}

CTask* CInstExt::FindTask(u32 dwTaskNO) const
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        return NULL;
    }

    return m_pTaskMng->FindTask(dwTaskNO);
}

CTask* CInstExt::FindTaskFromApp(u32 dwTaskNO) const
{
    if (dwTaskNO == INVALID_TASKNO)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "无效任务号\n");
        return NULL;
    }

    CApp* pApp = m_pApp;
    if (pApp == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]的app point is null\n",
            GetStrApp(), GetInsID());
        return NULL;
    }
    
    //查找所有实例                
    u16 wInstNum = (u16)pApp->GetInstanceNumber();
    u16 i=0;
    for (i=0; i<=wInstNum; i++)
    {
        CInstance* pInst = NULL;
        
        u16 wInstId = CInstance::INVALID;
        if (i==0)
        {
            wInstId = CInstance::DAEMON;            
        }
        else
        {
            wInstId = i;
        }
        
        pInst = pApp->GetInstance(wInstId);
        
        if (pInst == NULL)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例指针为空\n", pApp->NameGet(), wInstId);
            continue;
        }
        
        CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
        if (pInstExt == NULL)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]实例不是CInstExt类型\n", pApp->NameGet(), wInstId);
            continue;
        }

        CTask* pTask = pInstExt->FindTask(dwTaskNO);

        if (pTask != NULL)
        {
            return pTask;
        }
    }

    return NULL;
}

//
u32 CInstExt::GetTaskNum() const
{
    if (m_pTaskMng != NULL)
    {
        return m_pTaskMng->GetTaskNum();
    }

    return 0;
}

u32 CInstExt::GetPollTaskNum() const
{
    if (m_pTaskMng != NULL)
    {
        return m_pTaskMng->GetPollTaskNum();
    }

    return 0;
}

void CInstExt::AddTaskToPollMap(u32 dwTaskNO)
{
    if (m_pTaskMng != NULL)
    {
        return m_pTaskMng->AddTaskToPollMap(dwTaskNO);
    }
}

void CInstExt::EraseTaskFromPollMap(u32 dwTaskNO)
{
    if (m_pTaskMng != NULL)
    {
        return m_pTaskMng->EraseTaskFromPollMap(dwTaskNO);
    }
}

//获取指定类型task的指针集合的map, szTaskType为task的类名，szTaskType为空时获取所有任务指针
CXMap<u32, CTask*> CInstExt::GetTasksMap(LPCSTR szTaskType)
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        CXMap<u32, CTask*> tTaskNullMap;
        return tTaskNullMap;
    }

    return m_pTaskMng->GetTasksMap(szTaskType);
}

//获取指定类型task的指针集合的array, szTaskType为task的类名，szTaskType为空时获取所有任务指针
vector<CTask*> CInstExt::GetTasksArray(LPCSTR szTaskType)
{
    if (m_pTaskMng == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "m_pTaskMng == NULL\n");
        vector<CTask*> tTaskNullArray;
        return tTaskNullArray;
    }
    
    return m_pTaskMng->GetTasksArray(szTaskType);
}

//
void CInstExt::OnTimerPollTasks()
{
    if (m_pTaskMng != NULL)
    {
        m_pTaskMng->OnTimerPollTasks();
    }
}

//设置Task定时器
bool CInstExt::SetTaskTimer(u32 dwTimerEvent, long dwMilliSeconds, u32 dwTaskNO)
{
    TTaskTimerInfo tUserTimerInfo;
    tUserTimerInfo.dwTaskNO = dwTaskNO;
    tUserTimerInfo.wUserTimerID = u16(dwTimerEvent);
    u16 wInnerTimerID = m_pTaskTimerMng->GenTaskTimerID(tUserTimerInfo);
    if (wInnerTimerID == INVALID_TASK_TIMER_ID)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "无空闲InnerTimerID，设置Task[%u]定时器失败[timerEvent:%s-%u, timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
        return false;
    }

    if (OSP_OK == SetTimer(wInnerTimerID, dwMilliSeconds, dwTaskNO))
    {
        INSTLOG(OSPEXT, TIMER_LEV, "osp设置Task[%u]定时器成功[timerEvent:%s-%u, InnerTimerID[%u], timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, wInnerTimerID, dwMilliSeconds);
        return true;
    }
    else
    {
        INSTLOG(OSPEXT, ERROR_LEV, "osp设置Task[%u]定时器失败[timerEvent:%s-%u, timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
        return false;
    }
}

//删除Task定时器
bool CInstExt::KillTaskTimer(u32 dwTimerEvent, u32 dwTaskNO)
{
    TTaskTimerInfo tUserTimerInfo;
    tUserTimerInfo.dwTaskNO = dwTaskNO;
    tUserTimerInfo.wUserTimerID = u16(dwTimerEvent);

    u16 wInnerTimerID = m_pTaskTimerMng->FindInnerTimerIDByUserTimer(tUserTimerInfo);
    if (wInnerTimerID == INVALID_TASK_TIMER_ID)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "无效定时器Task[%u][timerEvent:%s-%u]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent);
        return false;
    }

    m_pTaskTimerMng->RevokeTaskTimerID(wInnerTimerID);

    INSTLOG(OSPEXT, EVENT_LEV, "删除Task[%u]定时器[timerEvent:%s-%u, InnerTimerID[%u]\n",
        dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, wInnerTimerID);

    return (OSP_OK == KillTimer(wInnerTimerID));   
}
