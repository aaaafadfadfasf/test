
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
    if (g_dwTraceAppInst != 0  //���ڸ���ĳ��app-inst
        && g_dwTraceAppInst != dwCurrAppInst)    //��ǰ���Ǹ��ٵ�app-inst
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
    if (g_dwTraceAppInst != 0  //���ڸ���ĳ��app-inst
        && g_dwTraceAppInst != dwCurrAppInst)    //��ǰ���Ǹ��ٵ�app-inst
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

    //������ʱ����innerTimerIDӳ��ΪuserTimerID
    if (m_pTaskTimerMng->IsTaskTimerID(pcMsg->event))
    {
        pcMsg->event = m_pTaskTimerMng->RevokeTaskTimerID(pcMsg->event);
		if (INVALID_TASK_TIMER_ID == pcMsg->event)
		{
			//��Ч��EVENT_ID����Ҫ���ظ��ϲ�
			return;
		}
    }

    //�¼���ִ��ʱ�䣬���ڿ�������
    if (g_bIsMonitorTimePerform == TRUE)
    {
        TMsTime tStartMsTime = OspGetMsTime();
        DaemonEntry(pcMsg);
        TMsTime tEndMsTime = OspGetMsTime();

        u32 dwInterval = OspGetDiffMs(tEndMsTime, tStartMsTime);
        if (dwInterval > 30)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "����Msg[%s-%hu]���غ�ʱ����Ҫ[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else if (dwInterval > 10)
        {
            INSTLOG(OSPEXT, WARNING_LEV, "����Msg[%s-%hu]�ȽϺ�ʱ����Ҫ[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else
        {
            INSTLOG(OSPEXT, EVENT_LEV, "����Msg[%s-%hu]��������Ҫ[%u(ms)]\n", 
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
    //��� dwDstNode = 0, dwDstIId = 0, ��Ϣ������ʵ��
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
        INSTLOG(OSPEXT, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        delete pTask;
        pTask = NULL;
    }
    else if (dwRet == PROCMSG_FAIL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else if (dwRet == PROCMSG_OK)
    {
        INSTLOG(OSPEXT, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else
    {
        INSTLOG(OSPEXT, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
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

    //������ʱ����innerTimerIDӳ��ΪuserTimerID
    if (m_pTaskTimerMng->IsTaskTimerID(pcMsg->event))
    {
        pcMsg->event = m_pTaskTimerMng->RevokeTaskTimerID(pcMsg->event);
		if (INVALID_TASK_TIMER_ID == pcMsg->event)
		{
			//��Ч��EVENT_ID����Ҫ���ظ��ϲ�
			return;
		}
    }


    //�¼���ִ��ʱ�䣬���ڿ�������
    if (g_bIsMonitorTimePerform == TRUE)
    {
        TMsTime tStartMsTime = OspGetMsTime();
        NormalEntry(pcMsg);
        TMsTime tEndMsTime = OspGetMsTime();

        u32 dwInterval = OspGetDiffMs(tEndMsTime, tStartMsTime);
        if (dwInterval > 30)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "����Msg[%s-%hu]���غ�ʱ����Ҫ[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else if (dwInterval > 10)
        {
            INSTLOG(OSPEXT, WARNING_LEV, "����Msg[%s-%hu]�ȽϺ�ʱ����Ҫ[%u(ms)]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event, dwInterval); 
        }
        else
        {
            INSTLOG(OSPEXT, EVENT_LEV, "����Msg[%s-%hu]��������Ҫ[%u(ms)]\n", 
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
        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]��app point is null\n",
                GetStrApp(), GetInsID());
        return FALSE;
    }

    switch(pcMsg->event)
    {
    case OSP_EXT_EVENT_SET_LOG_LEV:
        {
            u8 byLev = *(u8*)pcMsg->content;
            m_byLogLev = byLev;
            
            OspPrintf(TRUE, FALSE, "����app[%s]-inst[%u]�Ĵ�ӡ����Ϊ[%s-%d]\n",
                GetStrApp(), GetInsID(), GetStrLogLev(byLev).c_str(), byLev);
        }
        break;

    case OSP_EXT_EVENT_SET_APP_LOG_LEV:
        {
            if (GetInsID() == CInstance::DAEMON)
            {
                u8 byLev = *(u8*)pcMsg->content;

                //��������ʵ��                
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    pInstExt->m_byLogLev = byLev;
                }

                OspPrintf(TRUE, FALSE, "����app[%s]����ʵ���Ĵ�ӡ����Ϊ[%s-%d]\n",
                    GetStrApp(), GetStrLogLev(byLev).c_str(), byLev);
            }
        }
        break;
        
    case OSP_EXT_EVENT_POLL_TASK:
        {
            if (CurState() == 0)
            {
				// ��������������ˣ���ô��ʱ������Ҳû���ᴥ���ˣ�����task��ܵĶ�ʱ��ѯ���ƾ�ͣ��
				// ������������ٴ��趨��ʱ�� 2019/1/15 zhusenyong 
				KillTimer(OSP_EXT_EVENT_POLL_TASK);
				SetTimer(OSP_EXT_EVENT_POLL_TASK, (long)m_pTaskMng->GetPollInterval());
                INSTLOG(OSPEXT, TIMER_LEV, "��IDLE״̬�յ�������ѯ��ϢMsg[%s-%hu], ��״̬������\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                break;
            }

            INSTLOG(OSPEXT, TRIVIAL_LEV, "Receive ������ѯ��ϢMsg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            KillTimer(OSP_EXT_EVENT_POLL_TASK);
            
            if (GetPollTaskNum() > 0)
            {
                OnTimerPollTasks();
                SetTimer(OSP_EXT_EVENT_POLL_TASK, (long)m_pTaskMng->GetPollInterval());
                m_bTimerIsRunning = TRUE;
            }
            else
            {
                INSTLOG(OSPEXT, TIMER_LEV, "Task num == 0,��ͣ��ѯ��ʱ��\n");
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
                //��������ʵ��                
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
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
                //��������ʵ��                
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
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
                //��������ʵ��                
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
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
                INSTLOG(OSPEXT, ERROR_LEV, "������TaskNO[%u]������\n", dwTaskNO);
            }
        }
        break;

    case OSP_EXT_EVENT_KILL_TASK:
        {
            u32 dwTaskNO = *(u32*)pcMsg->content;
            BOOL32 bFind = FALSE;

            if (GetInsID() == CInstance::DAEMON)
            {
                //��������ʵ��                
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
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
                INSTLOG(OSPEXT, ERROR_LEV, "������TaskNO[%u]������\n", dwTaskNO);
            }
        }
        break;

    case OSP_EXT_EVENT_TRACE_INST:
        {
            u16 wTraceInstId = *(u16*)pcMsg->content;
            
            if (GetInsID() == CInstance::DAEMON)
            {
                //��������ʵ��            
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
                //��������ʵ��                
                u16 wInstNum = (u16)pApp->GetInstanceNumber();
                OspPrintf(TRUE, FALSE, "\napp[%s-%u]����[%u]����ͨʵ��, ״̬����:\n", 
                    GetStrApp(), GetAppID(), wInstNum);

                OspPrintf(TRUE, FALSE, "    DAEMONʵ��״̬: [%s], tasknum[%u], polltasknum[%u], PollTimer[run:%d, time:%ums, count:%u], loglev[%s-%u]\n", 
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
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
                        continue;
                    }
                    
                    CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
                    if (pInstExt == NULL)
                    {
                        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
                        continue;
                    }

                    if (pInstExt->CurState() != 0)
                    {
                        OspPrintf(TRUE, FALSE, "    ʵ��[%u]״̬: [%s], tasknum[%u], polltasknum[%u], PollTimer[run:%d, time:%ums, count:%u], loglev[%s-%u]\n", 
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

                OspPrintf(TRUE, FALSE, "    ����ʵ����[%u]��\n", wIdleNum);
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
        INSTLOG(OSPEXT, ERROR_LEV, "��Ч�����\n");
        return NULL;
    }

    CApp* pApp = m_pApp;
    if (pApp == NULL)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]��app point is null\n",
            GetStrApp(), GetInsID());
        return NULL;
    }
    
    //��������ʵ��                
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
            INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", pApp->NameGet(), wInstId);
            continue;
        }
        
        CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
        if (pInstExt == NULL)
        {
            INSTLOG(OSPEXT, ERROR_LEV, "app[%s]-inst[%u]ʵ������CInstExt����\n", pApp->NameGet(), wInstId);
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

//��ȡָ������task��ָ�뼯�ϵ�map, szTaskTypeΪtask��������szTaskTypeΪ��ʱ��ȡ��������ָ��
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

//��ȡָ������task��ָ�뼯�ϵ�array, szTaskTypeΪtask��������szTaskTypeΪ��ʱ��ȡ��������ָ��
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

//����Task��ʱ��
bool CInstExt::SetTaskTimer(u32 dwTimerEvent, long dwMilliSeconds, u32 dwTaskNO)
{
    TTaskTimerInfo tUserTimerInfo;
    tUserTimerInfo.dwTaskNO = dwTaskNO;
    tUserTimerInfo.wUserTimerID = u16(dwTimerEvent);
    u16 wInnerTimerID = m_pTaskTimerMng->GenTaskTimerID(tUserTimerInfo);
    if (wInnerTimerID == INVALID_TASK_TIMER_ID)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "�޿���InnerTimerID������Task[%u]��ʱ��ʧ��[timerEvent:%s-%u, timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
        return false;
    }

    if (OSP_OK == SetTimer(wInnerTimerID, dwMilliSeconds, dwTaskNO))
    {
        INSTLOG(OSPEXT, TIMER_LEV, "osp����Task[%u]��ʱ���ɹ�[timerEvent:%s-%u, InnerTimerID[%u], timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, wInnerTimerID, dwMilliSeconds);
        return true;
    }
    else
    {
        INSTLOG(OSPEXT, ERROR_LEV, "osp����Task[%u]��ʱ��ʧ��[timerEvent:%s-%u, timeIneterval:%ld]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, dwMilliSeconds);
        return false;
    }
}

//ɾ��Task��ʱ��
bool CInstExt::KillTaskTimer(u32 dwTimerEvent, u32 dwTaskNO)
{
    TTaskTimerInfo tUserTimerInfo;
    tUserTimerInfo.dwTaskNO = dwTaskNO;
    tUserTimerInfo.wUserTimerID = u16(dwTimerEvent);

    u16 wInnerTimerID = m_pTaskTimerMng->FindInnerTimerIDByUserTimer(tUserTimerInfo);
    if (wInnerTimerID == INVALID_TASK_TIMER_ID)
    {
        INSTLOG(OSPEXT, ERROR_LEV, "��Ч��ʱ��Task[%u][timerEvent:%s-%u]\n",
            dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent);
        return false;
    }

    m_pTaskTimerMng->RevokeTaskTimerID(wInnerTimerID);

    INSTLOG(OSPEXT, EVENT_LEV, "ɾ��Task[%u]��ʱ��[timerEvent:%s-%u, InnerTimerID[%u]\n",
        dwTaskNO, OspExtEventDesc((u16)dwTimerEvent).c_str(), dwTimerEvent, wInnerTimerID);

    return (OSP_OK == KillTimer(wInnerTimerID));   
}
