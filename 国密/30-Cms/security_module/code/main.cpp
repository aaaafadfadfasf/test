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
    //��ʼ��OSPEXT
    if (FALSE == OspExtInit(tOspExtConf))
    {
        printf("ospext��ʼ��ʧ��\n");
        return -1;
    }

	RegSecurityCmd();

    //ע����Ϣ�¼�
    InitSecurityModuleEventDesc();
	g_pcCoreApp = new CCoreApp();
    //����OSP APP
    if (g_pcCoreApp->CreateOspApp("cctrlApp", AID_COREAPP, 80) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return -1;
    }

    //��ʼ��SIP
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
    string strSipLogPath = GetLogDir() + "security_module/";   //ֻҪ���õ�ģ�鼴�ɣ�kdsip���Լ�������Ŀ¼
    tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

	//��ʼ��DBIX
	TDBIXConf& tDbixConf = g_cConf.m_tDbixConf;
	//tDbixConf.cbDBResult = DBResultHandlerCB;
	tDbixConf.cbDBResult = cbDbResultHandler;
	if(!DBIXInit(tDbixConf))
	{
		printf("DBIXInit fail!!!\n");
		return 0;
	}
	printf("DBIXInit��ʼ���ɹ�!\n");

    OspPost(COREAPPIID, ModulePowon);

    if (FALSE == OspSemBCreate(&g_hSecurityModuleExitSem))
    {
        g_hSecurityModuleExitSem = NULL;
        PrintAndReturn("SecurityModuleExitSem create failed");
    }

    OspSemTake(g_hSecurityModuleExitSem);
    OspSemTake(g_hSecurityModuleExitSem); // �ȴ��˳��ź�

    printf("exitting step1...\n");
    OspPost(COREAPPIID, ModulePowoff);
    OspSemTakeByTime(g_hSecurityModuleExitSem, 1000*10); // �ȴ�ҵ���ͷ���Դ
    printf("exitting step2...\n");
    OspSemDelete(g_hSecurityModuleExitSem);

	DBIXQuit();
	OspSipQuit();
	OspExtQuit();
    return 0;
}