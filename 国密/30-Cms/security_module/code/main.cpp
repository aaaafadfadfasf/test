#include "cms/commonconfig/cmcfg.h"
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "mesg.h"
#include "cms/security_module/security_module_event.h"
#include "conf.h"
#include "cms/ospext/ospapp.h"
#include "coreapp.h"
#include "securitydbg.h"


SEMHANDLE g_hSecurityModuleExitSem;
#define PrintAndReturn(desc) {printf("%s\n", desc); return -1;}

int main()
{
	g_cConf.Load();

    string strLogDir = GetLogDir();
    TOspExtConf tOspExtConf;

    tOspExtConf.strErrLogDir = strLogDir + "security_module/errlog/";
    tOspExtConf.strRunLogDir = strLogDir + "security_module/runlog/";
	tOspExtConf.ReadKeyCfg(ConfPath);
    //初始化OSPEXT
    if (FALSE == OspExtInit(tOspExtConf))
    {
        printf("ospext初始化失败\n");
        return -1;
    }

	RegSecurityCmd();

    //注册消息事件
    InitSecurityModuleEventDesc();
	g_pcCoreApp = new CCoreApp();
    //创建OSP APP
    if (g_pcCoreApp->CreateOspApp("cctrlApp", AID_COREAPP, 80) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return -1;
    }

    //初始化SIP
	TSipURI tUri;
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr.tIpAddr=GetLocalIPAddress();
    tOspSipConf.tLocalAddr.wPort=g_cConf.GetListenPort();
    tOspSipConf.tProxyAddr.tIpAddr=GetProxyIPAddress();
    tOspSipConf.tProxyAddr.wPort=GetProxyPort();
	tOspSipConf.tProxyURI.SetUser("proxy"); 
	tOspSipConf.tProxyURI.SetDomain(GetDomainName());
    tOspSipConf.tLocalURI.SetUser(tUri.GetSecureModuleURI().GetUser());
    tOspSipConf.tLocalURI.SetDomain(GetDomainName());
    tOspSipConf.dwDefaultOspIID = COREAPPIID;

    TSipStackConf tSipStackConf;
    tSipStackConf.ReadKeyCfg("./conf/security_module.ini");
    string strSipLogPath = GetLogDir() + "security_module/";   //只要设置到模块即可，kdsip会自己创建子目录
    tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

	//初始化DBIX
	TDBIXConf& tDbixConf = g_cConf.m_tDbixConf;
	//tDbixConf.cbDBResult = DBResultHandlerCB;
	tDbixConf.cbDBResult = cbDbResultHandler;
	if(!DBIXInit(tDbixConf))
	{
		printf("DBIXInit fail!!!\n");
		return 0;
	}
	printf("DBIXInit初始化成功!\n");

    OspPost(COREAPPIID, ModulePowon);

    if (FALSE == OspSemBCreate(&g_hSecurityModuleExitSem))
    {
        g_hSecurityModuleExitSem = NULL;
        PrintAndReturn("SecurityModuleExitSem create failed");
    }

    OspSemTake(g_hSecurityModuleExitSem);
    OspSemTake(g_hSecurityModuleExitSem); // 等待退出信号

    printf("exitting step1...\n");
    OspPost(COREAPPIID, ModulePowoff);
    OspSemTakeByTime(g_hSecurityModuleExitSem, 1000*10); // 等待业务释放资源
    printf("exitting step2...\n");
    OspSemDelete(g_hSecurityModuleExitSem);

	DBIXQuit();
	OspSipQuit();
	OspExtQuit();
    return 0;
}