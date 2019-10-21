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
    OspPrintf(TRUE, FALSE, " dbixȫ��������Ϣ================================================\n");
    OspPrintf(TRUE, FALSE, " ����ص�������ַ:%s\n", (g_tDBIXConf.cbDBResult!=NULL? "��Ч":"��Ч"));
    OspPrintf(TRUE, FALSE, " ���ݿ������������:\n");
    for (vector<TTaskQueueConf>::const_iterator it = g_tDBIXConf.vecTaskQueueConf.begin(); 
        it != g_tDBIXConf.vecTaskQueueConf.end(); ++it)
    {
        OspPrintf(TRUE, FALSE, " \t���ݿ�����(1-sybase 2-mysql 3-oracle)��%d\n", (s32)it->tDBConnectCfg.enDbType);
        OspPrintf(TRUE, FALSE, " \t���ݿ������IP��%s\n", it->tDBConnectCfg.strDbServerIP.c_str());
        OspPrintf(TRUE, FALSE, " \t���ݿ�������˿ڣ�%u\n", (u32)it->tDBConnectCfg.wDbServerPort);
        OspPrintf(TRUE, FALSE, " \t���ݿ��������¼���ƣ�%s\n", it->tDBConnectCfg.strDbUserName.c_str());
        OspPrintf(TRUE, FALSE, " \t���ݿ��������¼���룺%s\n", it->tDBConnectCfg.strDbUserPassword.c_str());
        OspPrintf(TRUE, FALSE, " \t���ݿ�������������ݿ⣺%s\n", it->tDBConnectCfg.strDbName.c_str());

        OspPrintf(TRUE, FALSE, " \t���ݿ⹤���߳���Ŀ��%u\n\n", it->dwThreadNum);
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
    OspPrintf(TRUE, FALSE, " ��������===========================================================\n");
    OspPrintf(TRUE, FALSE, " dbixhelp         : ��ʾ������Ϣ\n");
    OspPrintf(TRUE, FALSE, " dbixver          : ��ʾ�汾��Ϣ\n");
    OspPrintf(TRUE, FALSE, " dbixstat         : ��ʾ״̬����\n");
    OspPrintf(TRUE, FALSE, " dbixlog(level)   : ������־����. level��ֵ, 1--��Ҫ���� 2--�ؼ����� 3--������� 7--���Լ���\n");
    OspPrintf(TRUE, FALSE, " dbi����������б� ��dbiver, dblogon, dblogoff, pstat, cstat\n");
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
