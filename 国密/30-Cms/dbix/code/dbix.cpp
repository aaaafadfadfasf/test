#include "inner_common.h"
#include "connect_pool.h"
#include "task_queue.h"
#include "thread_pool.h"
#include "dbi_layer.h"

DBIX_NS_BEGIN

extern void RegisterDbixCmd();

bool g_bInited = false;
TDBIXConf2 g_tDBIXConf;

CConnectPool g_cConnectPool;
vector<CTaskQueue*> g_vecpcTaskQueue;
vector<CThreadPool*> g_vecpcThreadPool;

bool DBIXInit(const TDBIXConf &tConf)
{
    TDBIXConf2 tConfImp;
    tConfImp.cbDBResult = tConf.cbDBResult;
    TTaskQueueConf tQueueConf;
    tQueueConf.tDBConnectCfg = tConf.tDBConnectCfg;
    tQueueConf.dwThreadNum = tConf.dwThreadNum;
    tConfImp.vecTaskQueueConf.push_back(tQueueConf);

    return DBIXInit(tConfImp);
}

bool DBIXInit( const TDBIXConf2 &tConf )
{
    if (g_bInited)
    {
        log(enLOGLVL_ERROR, "dbix reinited");
        return true;
    }

    do 
    {
        if (tConf.cbDBResult == NULL || tConf.vecTaskQueueConf.size() == 0)
        {
            log(enLOGLVL_ERROR, "invalid agrument");
            break;
        }
        
        dbixlog(enLOGLVL_WARN);

        g_tDBIXConf = tConf;

        if (!DBIInit(g_tDBIXConf.vecTaskQueueConf.front().tDBConnectCfg.enDbType))
        {
            break;
        }

        if (!g_cConnectPool.Init(g_tDBIXConf.vecTaskQueueConf.front().tDBConnectCfg))
        {
            break;
        }

        vector<TTaskQueueConf>::const_iterator it = g_tDBIXConf.vecTaskQueueConf.begin();
        for (u16 wTaskQueueIndex = 0; it != g_tDBIXConf.vecTaskQueueConf.end(); ++it, ++wTaskQueueIndex)
        {
            g_vecpcTaskQueue.push_back(new CTaskQueue());
            g_vecpcThreadPool.push_back(new CThreadPool());

            if (!g_vecpcTaskQueue.back()->Init())
            {
                break;
            }

            vector<CDBConnection*> cDBConns;
            u32 i;
            for (i = 0; i < it->dwThreadNum; ++i)
            {
                cDBConns.push_back(AllocateDBConnection(&it->tDBConnectCfg));
                if (cDBConns.back() == NULL)
                {
                    break;
                }
            }
            if (i != it->dwThreadNum)
            {
                break;
            }

            if (!g_vecpcThreadPool.back()->Init(g_vecpcTaskQueue.back(), cDBConns, wTaskQueueIndex))
            {
                break;
            }
        }
        if (it != g_tDBIXConf.vecTaskQueueConf.end())
        {
            break;
        }

        RegisterDbixCmd();

        g_bInited = true;
        log(enLOGLVL_CRITICAL_PATH, "dbix init success");
        return true;
    } while (false);

    DBIXQuit();
    log(enLOGLVL_CRITICAL_PATH, "dbix init fail");
    return false;
}

void DBIXQuit()
{
    for (vector<CThreadPool*>::iterator it = g_vecpcThreadPool.begin(); it != g_vecpcThreadPool.end(); ++it)
    {
        (*it)->Quit();
        delete (*it);
    }
    for (vector<CTaskQueue*>::iterator it = g_vecpcTaskQueue.begin(); it != g_vecpcTaskQueue.end(); ++it)
    {
        (*it)->Quit();
        delete (*it);
    }
    g_cConnectPool.Quit();

    g_bInited = false;

    log(enLOGLVL_CRITICAL_PATH, "dbix quit");
}

DBIX_NS_END
