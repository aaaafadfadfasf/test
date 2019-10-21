#include "inner_common.h"
#include "teleapp.h"
#include "configfile.h"
extern CTeleApp g_cTeleApp;
extern void ProtocolTest();
extern void GenerateXmlContent();

#define PrintAndBreak(desc) {printf(desc); break;}

SEMHANDLE g_hTastestExitSem  = NULL;       // 程序退出同步访问信号量

int main(int argc,char* argv[])
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
        tOspSipConf.tLocalAddr.tIpAddr = GetLocalIP();
        tOspSipConf.tLocalAddr.wPort = GetLocalPort();
        tOspSipConf.tProxyAddr.tIpAddr = GetProxyIP_();
        tOspSipConf.tProxyAddr.wPort = GetProxyPort_();
        tOspSipConf.tLocalURI.SetNOString(GetSelfUri());
        tOspSipConf.dwDefaultOspIID = MAKEIID(AID_TELEAPP, CInstance::DAEMON);
        TSipStackConf tSipStackConf;
        tSipStackConf.tKdSipConfig.nReceiveBufferSize = GetSipStackSize();

        printf("OspSipInit...\n");
        bool bVal = OspSipInit(tOspSipConf, tSipStackConf);
        printf("OspSipInit ret=%d\n", bVal);
        if(!bVal)
        {
            PrintAndBreak("InitOspSip failed");
        }

        kdsiplog(2);
        ospsiplog(3);

        if (!g_cTeleApp.CreateOspApp("CTeleApp", AID_TELEAPP, 80, 1000))
        {
            PrintAndBreak("CTeleApp create failed");
        }

        // 激活App
        OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), OSP_POWERON);

        // 注册proxy
        TKDSIP_DIALOG_ID tRegId = OspSipRegProxy(240);
        if (tRegId == INVALID_DIALOG_ID)
        {
            PrintAndBreak("OspSipRegProxy failed");
        }

        if (FALSE == OspSemBCreate(&g_hTastestExitSem))
        {
            PrintAndBreak("TasExitSem create failed");
        }

        // ProtocolTest();
        // GenerateXmlContent();

        OspSemTake(g_hTastestExitSem);
        OspSemTake(g_hTastestExitSem); // 等待退出信号
    } while (false);

    OspSipQuit();
    OspQuit();

    return 0;
}
