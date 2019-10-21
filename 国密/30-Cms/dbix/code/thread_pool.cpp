#include "thread_pool.h"
#include "kdvsys.h"

DBIX_NS_BEGIN

extern vector<CThreadPool*> g_vecpcThreadPool;

bool CThreadPool::Init( CTaskQueue *pcQueue, const vector<CDBConnection*> &cDBConns, u16 wThreadPoolIndex )
{
    if (cDBConns.size() == 0 || NULL == pcQueue)
    {
        log(enLOGLVL_ERROR, "CThreadPool::Init detected invalid parms");
        return false;
    }

    u32 dwThreadNum = cDBConns.size();
    m_pcQueue = pcQueue;

    m_bQuit = false;
    do 
    {
        m_cTaskDbConnects = cDBConns;

        TASKHANDLE hThread;
        u32 i;
        for (i = 0; i < dwThreadNum; ++i)
        {
            u32 *pThreadParam=new u32(MAKEIID(wThreadPoolIndex, (u16)i));
            hThread = OspTaskCreate(CThreadPool::ProcTaskEntry, "DbOprTask", 90, 1024*1024, (KD_PTR)pThreadParam, 0);
            if (0 == hThread) 
            {
                log(enLOGLVL_ERROR, "CThreadPool create thread failed");
                break;
            }

            m_cTaskHandles.push_back(hThread);
        }
        if (i != dwThreadNum) break;

        return true;
    } while (false);
    
    Quit();
    return false;
}

void CThreadPool::Quit()
{
    m_bQuit = true;
    u32 i;
    // 投递伪任务，以释放线程
    for (i = 0; i < m_cTaskHandles.size(); ++i)
    {
        ExecDBCmd<CDBCmd>(CDBCmd());
    }
    OspTaskDelay(500);
    for (i = 0; i < m_cTaskHandles.size(); ++i)
    {
        OspTaskTerminate(m_cTaskHandles[i]);
    }
    m_cTaskHandles.clear();

    for (i = 0; i < m_cTaskDbConnects.size(); ++i)
    {
        DeallocateDBConnection(m_cTaskDbConnects[i]);
    }
    m_cTaskDbConnects.clear();
}

void* CThreadPool::ProcTaskEntry( void* parm )
{
	u32 *pThreadParam=(u32*)parm;
	if (!pThreadParam)
	{
		log(enLOGLVL_ERROR, "ProcTaskEntry parm is NULL");
		return NULL;
	}
	u32 dwParm = *pThreadParam;
	u32 dwPoolIndex = GETAPP(dwParm);
	u32 dwThreadIndex = GETINS(dwParm);

	delete pThreadParam;
	pThreadParam=NULL;

    CThreadPool &cThreadPool = *g_vecpcThreadPool[dwPoolIndex];
    // 预建立连接
    cThreadPool.m_cTaskDbConnects[dwThreadIndex]->CheckConnection();

    u32 dwDequeueTimeoutTimes = 0;
    TDbTaskItem tDbTaskItem;
    for(;;)
    {
        if (cThreadPool.m_pcQueue->Dequeue(tDbTaskItem, 180000))
        {
            if (cThreadPool.m_bQuit) break;

            cThreadPool.m_cTaskDbConnects[dwThreadIndex]->CheckConnection();
            cThreadPool.ProcTask(dwThreadIndex, tDbTaskItem);
            delete tDbTaskItem;
        }
        else
        {
            if (cThreadPool.m_bQuit) break;

            if (++dwDequeueTimeoutTimes > 3)
            {
                cThreadPool.m_cTaskDbConnects[dwThreadIndex]->CheckConnection(true);
                dwDequeueTimeoutTimes = 0;
            }
        }
    }

    return 0;
}

void CThreadPool::ProcTask( u32 dwThreadIndex, const TDbTaskItem &tDbTaskItem )
{
    log(enLOGLVL_DEBUG, "ProcTask begin, threadIndex=%u\n", dwThreadIndex);
    CDbResultDataBase *pcDBResult = NULL;
    ENDBResult enResult = tDbTaskItem->Exec(m_cTaskDbConnects[dwThreadIndex], pcDBResult);

    log(enLOGLVL_DEBUG, "ProcTask call resultHandler, threadIndex=%u\n", dwThreadIndex);
    // 返回结果
    (*g_tDBIXConf.cbDBResult)(enResult, static_cast<CDBEntityValue*>(pcDBResult), tDbTaskItem->GetCBParm());
    log(enLOGLVL_DEBUG, "ProcTask end, threadIndex=%u\n", dwThreadIndex);
}

void CThreadPool::Dump()
{
    OspPrintf(TRUE, FALSE, " 线程池状态信息===========================================================\n");
    OspPrintf(TRUE, FALSE, " 连接对象数目:%u\n", (u32)m_cTaskDbConnects.size());
    OspPrintf(TRUE, FALSE, " 线程句柄数目:%u\n", (u32)m_cTaskHandles.size());
    OspPrintf(TRUE, FALSE, " 退出标志:%s\n", m_bQuit? "quit":"run");
}

DBIX_NS_END
