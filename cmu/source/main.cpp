#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/natagent/natagent.h"
#include "cmucoreapp.h"
#include "cmuconf.h"
#include "cmudbg.h"
#include "nmstask.h"
#include "cmudata.h"
#include "system/tcmalloc/malloc_extension.h"

CXSemLock g_cMainSemLock;

void NALogCB(EmLogLev emLogLev, const s8 *pchLog)
{
	u8 byLogLevel = EVENT_LEV;
	switch(emLogLev)
	{
	case emError:
		byLogLevel = ERROR_LEV;
		break;
	case emWarning:
		byLogLevel = WARNING_LEV;
		break;
	case emInfo:
		byLogLevel = CRITICAL_LEV;
		break;
	default:break;
	}

	GLOBELOG(byLogLevel, "%s", pchLog);
}

void NAQryAddrCB(EmResult emResult,
    u32                 dwStunResultNum,
    TStunResult        *ptStunResultList,
    u32                 dwSeqNum,
    void               *context)
{
    //do nothing
}

void NAPunchResultCB(EmResult emResult,
    u32                 dwPunchResultNum,
    TPunchResult       *ptPunchResultList,
    u32                 dwSeqNum,
    void               *context)
{
    //do nothing
}

int main(int argc, char* argv[])
{
    //读取cmu配置文件
    if (g_cCmuConf.ReadConf(cmu_config) == FALSE)
    {
        printf("读取配置文件失败\n");
        return 0;
    }
    printf("读取配置文件成功!\n");
	int nTMLevel = g_cCmuConf.GetTcmallocLevel();
	printf("tcmalloc level = %d\n", nTMLevel);
	MallocExtension::instance()->SetMemoryReleaseRate(nTMLevel);

    // 数据初始化
    CRedisPipe::SetRedisPipeLogFunc(cbRedisLog);
    CCommonConfig cConfig = GetCommonConfig();
    if (!CCmuData::GetInstance()->Initialize(cConfig.GetDomainUUID(), cConfig.GetDomainName(), 
        cConfig.GetRedisIP(), cConfig.GetRedisPort(), cConfig.GetRedisPassword()))
    {
        printf("数据初始化失败!\n");
        return 0;
    }

    string strLogDir = GetLogDir();
    TOspExtConf tOspExtConf;
#ifdef _LINUX_
    tOspExtConf.strErrLogDir = strLogDir + "cmu/errlog/";
    tOspExtConf.strRunLogDir = strLogDir + "cmu/runlog/";
#else
    tOspExtConf.strErrLogDir = strLogDir + "cmu\\errlog\\";
    tOspExtConf.strRunLogDir = strLogDir + "cmu\\runlog\\";
#endif

    //初始化OSPEXT
	tOspExtConf.ReadKeyCfg(cmu_config);
    if (FALSE == OspExtInit(tOspExtConf))
    {
        printf("ospext初始化失败\n");
        return 0;
    }
    printf("ospext初始化成功!\n");

    //linux下注册telnet调试命令
#ifdef _LINUX_
    RegCmuCmd();
#endif

    //初始化消息描述
    InitCmuMsgDesc();

    //初始化cmu内各模块日志级别
    InitCmuModuleLog();

    //创建OSP APP
    if (g_cCmuCoreApp.CreateOspApp("cmucore", CMU_CORE_APP_ID, 80, CMU_CORE_APP_QUEUE_SIZE) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return 0;
    }

    //初始化SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr = g_cCmuConf.m_tLocalAddr;
    tOspSipConf.tProxyAddr = g_cCmuConf.m_tProxyAddr;
    tOspSipConf.tLocalURI = g_cCmuConf.m_tLocalUri;
    tOspSipConf.dwDefaultOspIID = CMU_CORE_DAEMON;
    tOspSipConf.tUAType = UA_TYPE_CMU;

    TSipStackConf tSipStackConf;
    tSipStackConf.ReadKeyCfg(cmu_config);
    //tSipStackConf.tKdSipConfig.nReceiveBufferSize = (35*1024);
#ifdef _LINUX_
    string strSipLogPath = GetLogDir() + "cmu/";   //只要设置到模块即可，kdsip会自己创建子目录
#else
    string strSipLogPath = GetLogDir() + "cmu\\";
#endif
    tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }
    printf("OspSip初始化成功!\n");

    //初始化网管库
    if(!g_cNmsLib.Init(g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalAddr.GetIpStr(),
                   GetNmsPort(),
                   g_cCmuConf.m_tLocalAddr.GetIpStr()))
    {
        printf("InitNmsLib fail!!!\n");
        return 0;
    }
    printf("NmsLib初始化成功!!!\n");

    //初始化natagent
    TInitInfo tInitInfo;
    NASetInitInfoToNULL(&tInitInfo);

    strcpy(tInitInfo.m_tSTUNAddr.m_achIP, ::GetStunIPAddress().c_str());
    tInitInfo.m_tSTUNAddr.m_wPort = ::GetStunPort();

    tInitInfo.m_fLogPrinter = &NALogCB;
    tInitInfo.m_fQueryMappedAddr = &NAQryAddrCB;
    tInitInfo.m_fPunchResult = &NAPunchResultCB;

    if (NATAGENT_OK != NAInitLib(&tInitInfo))
    {
        printf("Failed to NAInitLib!\n");
        return 0;
    }
    printf("NatagentLib初始化成功!!!\n");

    //启动coreApp
    ::OspPost(CMU_CORE_DAEMON, CMU_START);

    printf("cmu启动成功!\n");
    GLOBELOG(CRITICAL_LEV, "cmu启动成功!\n");

	//启动tcmalloc自动释放内存task
	int nReleaseInterval = g_cCmuConf.GetTcmallocReleaseInterval();
	GLOBELOG(CRITICAL_LEV, "tcmalloc release interval = %d\n", nReleaseInterval);

    //阻塞主线程
    g_cMainSemLock.Take();
    g_cMainSemLock.Take();

    //coreApp退出
    ::OspPost(CMU_CORE_DAEMON, CMU_QUIT);
    g_cMainSemLock.Take();

    //退出NA
    NAQuitLib();

    //退出网管
    g_cNmsLib.Quit();
    
    //退出SIP
    OspSipQuit();

    //退出OSPEXT
    OspExtQuit();

    return 0;
}







