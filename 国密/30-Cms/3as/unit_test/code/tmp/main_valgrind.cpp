#include "inner_common.h"
#include "teleapp.h"
#include "configfile.h"
extern CTeleApp g_cTeleApp;

#define PrintAndBreak(desc) {printf(desc); break;}

SEMHANDLE g_hTastestExitSem  = NULL;       // 程序退出同步访问信号量

int main(int argc,char** argv)
{
    do 
    {
        if (!OspInit(FALSE, 0, "tas_test"))
        {
            PrintAndBreak("OspInit failed");
        }
        OspSetPrompt("tas_test");
        extern void RegCmd();
        RegCmd();

        InitTacTasEventDesc();
        TOspSipConf tOspSipConf;
        tOspSipConf.tLocalAddr.tIpAddr = "172.16.251.6";
        tOspSipConf.tLocalAddr.wPort = 33333;
        tOspSipConf.tProxyAddr.tIpAddr = GetProxyIP();
        tOspSipConf.tProxyAddr.wPort = GetProxyPort();
        tOspSipConf.tLocalURI.SetNOString("tas_test@china");
        tOspSipConf.dwDefaultOspIID = MAKEIID(AID_TELEAPP, CInstance::DAEMON);
        TSipStackConf tSipStackConf;

        /*if (KDSIP_OK != KDSipInit("172.16.251.6", 33333))
        {
            printf("KDSipInit failed");
            return -1;
        }*/
        //if(!OspSipInit(tOspSipConf, tSipStackConf))
        //{
        //    PrintAndBreak("InitOspSip failed");
        //}
        //KDSipSetLogLev(KDSIP_LOG_EVENT); // hzytodo del
        //kdsiplog(0);
        //ospsiplog(0);

        //if (!g_cTeleApp.CreateOspApp("CTeleApp", AID_TELEAPP, 80, 1000))
        //{
        //    PrintAndBreak("CTeleApp create failed");
        //}

        //// 激活App
        //OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), OSP_POWERON);

        // 注册proxy
        /*TKDSIP_DIALOG_ID tRegId = OspSipRegProxy(240);
        if (tRegId == INVALID_DIALOG_ID)
        {
            PrintAndBreak("OspSipRegProxy failed");
        }*/

        if (FALSE == OspSemBCreate(&g_hTastestExitSem))
        {
            PrintAndBreak("TasExitSem create failed");
        }
        OspSemTake(g_hTastestExitSem);
        OspSemTake(g_hTastestExitSem); // 等待退出信号
    } while (false);

    //KDSipQuit();
    //OspSipQuit();
    OspQuit();

    return 0;
}
