#include "dbix_debug.h"
#include "cbb/osp/osp.h"
#include "cms/ospext/osplog.h"
#include "cms/ospext/ospdbg.h"
#include "cms/cmsassert.h"
#include "connect_pool.h"
#include "task_queue.h"
#include "thread_pool.h"

extern void InitDbixResultCbMsgDesc();

DBIX_NS_BEGIN

extern CConnectPool g_cConnectPool;
extern vector<CTaskQueue*> g_vecpcTaskQueue;
extern vector<CThreadPool*> g_vecpcThreadPool;

void log(TLogLvl tLogLvl, const s8* lpcstrFormat, ...)
{
    //assert(enLOGLVL_ERROR != tLogLvl);

    s8 szBuf[1024];
    va_list pvlist;
    va_start(pvlist, lpcstrFormat);
    s32 nLen = vsnprintf(szBuf, sizeof(szBuf) -1, lpcstrFormat, pvlist);
    if( nLen <= 0 || nLen >= (s32)sizeof(szBuf) ) szBuf[sizeof(szBuf)-1] = '\0';
    va_end(pvlist);

    CMS_ASSERT_WITH_MSG((enLOGLVL_ERROR != tLogLvl), szBuf);

    GLOBELOG(DBIX, (u8)tLogLvl, "%s\n", szBuf);
}

API void dbixlog(u8 byLvl)
{
    OspSetModLogLev(DBIX, byLvl, "DBIX");
}

API void dbixstat()
{
    OspPrintf(TRUE, FALSE, " dbix全局配置信息================================================\n");
    OspPrintf(TRUE, FALSE, " 结果回调函数地址:%s\n", (g_tDBIXConf.cbDBResult!=NULL? "有效":"无效"));
    OspPrintf(TRUE, FALSE, " 数据库任务队列配置:\n");
    for (vector<TTaskQueueConf>::const_iterator it = g_tDBIXConf.vecTaskQueueConf.begin(); 
        it != g_tDBIXConf.vecTaskQueueConf.end(); ++it)
    {
        OspPrintf(TRUE, FALSE, " \t数据库类型(1-sybase 2-mysql 3-oracle)：%d\n", (s32)it->tDBConnectCfg.enDbType);
        OspPrintf(TRUE, FALSE, " \t数据库服务器IP：%s\n", it->tDBConnectCfg.strDbServerIP.c_str());
        OspPrintf(TRUE, FALSE, " \t数据库服务器端口：%u\n", (u32)it->tDBConnectCfg.wDbServerPort);
        OspPrintf(TRUE, FALSE, " \t数据库服务器登录名称：%s\n", it->tDBConnectCfg.strDbUserName.c_str());
        OspPrintf(TRUE, FALSE, " \t数据库服务器登录密码：%s\n", it->tDBConnectCfg.strDbUserPassword.c_str());
        OspPrintf(TRUE, FALSE, " \t数据库服务器工作数据库：%s\n", it->tDBConnectCfg.strDbName.c_str());

        OspPrintf(TRUE, FALSE, " \t数据库工作线程数目：%u\n\n", it->dwThreadNum);
    }

    OspPrintf(TRUE, FALSE, "\n");
    g_cConnectPool.Dump();
    OspPrintf(TRUE, FALSE, "\n");
    for (vector<CTaskQueue*>::iterator it = g_vecpcTaskQueue.begin(); it != g_vecpcTaskQueue.end(); ++it)
    {
        (*it)->Dump();
        OspPrintf(TRUE, FALSE, "\n");
    }
    for (vector<CThreadPool*>::iterator it = g_vecpcThreadPool.begin(); it != g_vecpcThreadPool.end(); ++it)
    {
        (*it)->Dump();
        OspPrintf(TRUE, FALSE, "\n");
    }
}

#define DBIX_VER "dbix v1.0  Compile Time: %s, %s\r\n", __DATE__, __TIME__
API void dbixver()
{
    OspPrintf(TRUE, FALSE, " ===========================================================\n");
    OspPrintf(TRUE, FALSE, DBIX_VER);
}

API void dbixhelp()
{
    OspPrintf(TRUE, FALSE, " 帮助命令===========================================================\n");
    OspPrintf(TRUE, FALSE, " dbixhelp         : 显示帮助信息\n");
    OspPrintf(TRUE, FALSE, " dbixver          : 显示版本信息\n");
    OspPrintf(TRUE, FALSE, " dbixstat         : 显示状态数据\n");
    OspPrintf(TRUE, FALSE, " dbixlog(level)   : 控制日志级别. level数值, 1--重要错误 2--关键流程 3--警告错误 7--调试级别\n");
    OspPrintf(TRUE, FALSE, " dbi库帮助命令列表 ：dbiver, dblogon, dblogoff, pstat, cstat\n");
}

void RegisterDbixCmd()
{
#ifdef _LINUX_
    OspRegCommandEx("dbixhelp", (void*)dbixhelp, "dbixhelp", "");
    OspRegCommandEx("dbixver", (void*)dbixver, "dbixver", "");
    OspRegCommandEx("dbixstat", (void*)dbixstat, "dbixstat", "");
    OspRegCommandEx("dbixlog", (void*)dbixlog, "dbixlog", "i");
#endif

    ::InitDbixResultCbMsgDesc();
}

void DbixLoglvl(TLogLvl tLvl)
{
    dbixlog(tLvl);
}

DBIX_NS_END
