#include "inner_common.h"
#include "cms/ospext/ospext.h"

void DBResultHandler(ENDBResult enDBResult, CDbResultDataBase *pcResultData, TCBParm tParm);

void DbCmdDemo1Test(u32);
void DbCmdDemo2Test();
void DbCmdDemo3Test();
void DbEntityAdd();
void DbEntityDel();
void DbEntityMod();
void DbEntityQry();


SEMHANDLE g_hSem  = NULL;
API void quit()
{
    if (NULL != g_hSem)
    {
        OspSemGive(g_hSem);
    }
}

int main(int argc, const char* argv[])
{
    TDBIXConf2 tConf;
    tConf.cbDBResult = DBResultHandler;

    TTaskQueueConf tQueueConf;
    tQueueConf.tDBConnectCfg.enDbType = enDBTypeMysql;
    tQueueConf.tDBConnectCfg.strDbServerIP = "172.16.251.230";
    tQueueConf.tDBConnectCfg.wDbServerPort = 3306;
    tQueueConf.tDBConnectCfg.strDbName = "test";
    tQueueConf.tDBConnectCfg.strDbUserName = "root";
    tQueueConf.tDBConnectCfg.strDbUserPassword = "kedacom@123";
    tQueueConf.dwThreadNum = 8;
    tConf.vecTaskQueueConf.push_back(tQueueConf);

    TTaskQueueConf tQueueConf2;
    tQueueConf2.tDBConnectCfg.enDbType = enDBTypeMysql;
    tQueueConf2.tDBConnectCfg.strDbServerIP = "172.16.251.230";
    tQueueConf2.tDBConnectCfg.wDbServerPort = 3306;
    tQueueConf2.tDBConnectCfg.strDbName = "test";
    tQueueConf2.tDBConnectCfg.strDbUserName = "root";
    tQueueConf2.tDBConnectCfg.strDbUserPassword = "kedacom@123";
    tQueueConf2.dwThreadNum = 8;
    tConf.vecTaskQueueConf.push_back(tQueueConf2);

    
    bool bResult;

    OspInit(TRUE);
#ifdef _LINUX_
    OspRegCommand("quit", (void*)quit, "");
#endif // _LINUX_
    OspExtInit();
    bResult = DBIXInit(tConf);
    printf("DBIX Init %s\n", (bResult? "success":"failed"));
    OspTaskDelay(1000);

    DbCmdDemo1Test(0);
    DbCmdDemo1Test(1);
    DbCmdDemo2Test();
    DbCmdDemo3Test();

    DbEntityAdd();
    DbEntityQry();
    OspTaskDelay(1000);
    DbEntityMod();
    DbEntityQry();
    OspTaskDelay(1000);
    DbEntityDel();
    DbEntityQry();

    OspSemBCreate(&g_hSem);
    OspSemTake(g_hSem);
    OspSemTake(g_hSem);

    DBIXQuit();
    OspExtQuit();
    OspQuit();

    return 0;
}
