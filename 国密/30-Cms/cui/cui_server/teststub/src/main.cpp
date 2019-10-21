#include "cms/cppunitlite/cppunitlite.h"
#include "cms/cppunitlite/testresultxml.h"
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/cmu/cmu_event.h"
#include "cms/tas/tas_event.h"
#include "cms/cms_const.h"
#include "cuiproto.h"
#include "stubconfig.h"
#include "genxml.h"
#include "fakesvrapp.h"

CXSemLock g_MainSemLock;
CFakeSvrApp g_cApp;
extern string g_strDomainName;
//#define _UNIT_TEST_
#define CUIAPP_AID (200)

int main(int argc,char** argv)
{
    if ( ! ReadCommonConfig() )
    {
        printf("初始化公共配置文件失败, 直接退出\n");
        return -1;
    }

    //初始化Osp
    OspExtInit();

    //初始化所有业务消息
    InitCuiEventDesc();
    InitCmsEventDesc();
    InitTacTasEventDesc();

#if 0
    GenXml();
    return 0;
#endif
    
    LoadMsg(".\\xmlexp");
    g_cApp.CreateOspApp("CuiApp", CUIAPP_AID, 80);

    TASKHANDLE hRecvThread = OspTaskCreate( HbThread, "SelectRecvTask", 100, 1<<20, 0, 0);

    //初始化SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr.tIpAddr = "172.16.64.95";
    tOspSipConf.tLocalAddr.wPort = CUI_SIP_UDP_PORT + 1;
    tOspSipConf.tProxyAddr.tIpAddr = GetProxyIPAddress();
    tOspSipConf.tProxyAddr.wPort = GetProxyPort();
    TSipURI tLocal;
    tLocal.SetUser("server");
    tLocal.SetDomain(g_strDomainName);
    tOspSipConf.tLocalURI = tLocal;
    tOspSipConf.tProxyURI = tLocal.GetProxyURI();
    tOspSipConf.dwDefaultOspIID = MAKEIID(CUIAPP_AID, CInstance::DAEMON);

    TSipStackConf tSipStackConf;
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

    TKDSIP_DIALOG_ID tRegId = OspSipRegProxy(240);
    if (tRegId == INVALID_DIALOG_ID)
    {
        printf("OspSipRegProxy[%s] fail!!!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
        return 0;
    }

    //设置打印级别
    OspSetTimeMonitor(0);
    OspSetLogTimeLev(0);
    OspLogCompileInfo(0);
    OspSetLogLev(EVENT_LEV, CUIAPP_AID);

    {
        g_MainSemLock.Lock();
        g_MainSemLock.Lock();
    }

//     char achModName[256] = {0};
//     strncpy(achModName, argv[0], sizeof(achModName)-1);
// 
//     std::string szFilePath(achModName);
//     szFilePath += ".xml";
//     TestResultXml tr(szFilePath);

    //TestRegistry::runAllTests(tr);
    
    OspSipQuit();
    OspExtQuit();
    return 9;
    
    //return tr.wasSucessful() ? 0 : 1;
}

