#include "cms/cmu/cmu_event.h"
#include "cms/tas/tas_event.h"
#include "cms/uas/uas_event.h"
#include "cms/ospsip/ospsip.h"

#include "cmudbg.h"
#include "cmuconst.h"
#include "cms/natagent/natagent.h"

#include "cmuconf.h"
#include "system/tcmalloc/malloc_extension.h"

extern TDomainTable g_tDomainTable;
extern CCmuConf g_cCmuConf;
extern CXSemLock g_cMainSemLock;
bool g_bDebugOn = false;

//cmu帮助命令
API void cmuhelp()
{
    //模块基本命令
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "%-45s: cmu帮助命令\n","  cmuhelp()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu版本信息\n","  cmuver()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu配置信息\n","  cmucfg()");
    OspPrintf(TRUE, FALSE, "%-45s: redissdk相关信息\n","  redissdk()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu状态信息\n","  cmustat()");
    OspPrintf(TRUE, FALSE, "%-45s: 退出进程\n","  cmuquit()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //日志级别相关命令
    OspPrintf(TRUE, FALSE, "%-45s: 显示log参数信息\n","  logpara()");
    OspPrintf(TRUE, FALSE, "%-45s: 设置所有模块打印级别\n","  logall(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 关闭所有打印\n","  logoff()");
    OspPrintf(TRUE, FALSE, "%-45s: 打开调试模式（Task失败时会打印对象信息）\n","  debug(bool bVal)");
    //OspPrintf(TRUE, FALSE, "%-45s: 设置中心App打印级别\n","  loglev(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置core inst打印级别\n","  corelog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置pui会话打印级别\n","  puilog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置cui会话打印级别\n","  cuilog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置pu会话打印级别\n","  pulog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置cu会话打印级别\n","  culog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置tas会话打印级别\n","  taslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置uas会话打印级别\n","  uaslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置nms会话打印级别\n","  nmslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置异域cmu会话打印级别\n","  domainlog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置vtdu会话打印级别\n","  vtdulog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置订阅事务打印级别\n","  sslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置交换事务打印级别\n","  switchlog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: 设置redis日志打印级别\n","  redislog(u8 byLev)");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //事务控制命令
    OspPrintf(TRUE, FALSE, "%-45s: 打印当前所有任务（szTaskName:为空时打印所有Task，否则打印指定名称Task）\n","  tasklist(LPCSTR szTaskName)");
    OspPrintf(TRUE, FALSE, "%-45s: 根据taskNO打印指定任务\n","  task(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: 根据taskNO打印指定任务子事务列表\n","  taskson(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: kill指定僵尸事务\n","  IWillKillZombieTask(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: 根据taskNO打印指定任务子事务列表\n","  pnst(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: 根据SessID打印指定任务子事务列表\n","  psst(LPCSTR szSession)");
    OspPrintf(TRUE, FALSE, "%-45s: trace指定事务\n","  tracet(u16 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: trace下一个新建事务\n","  tracent()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //业务数据命令
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定域的设备统计信息\n","  sumpu(LPCSTR szDomainName)");
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定域的设备列表（byState：0-所有状态，1-在线，2-虚拟注册）\n","  pulist(LPCSTR szDomainName, u8 byState)");
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定前端的详细信息\n","  pu(LPCSTR szDevUri)");
	OspPrintf(TRUE, FALSE, "%-45s: 打印域列表\n","  domainlist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定域的信息\n","  domain(LPCSTR szDomainName)");
	OspPrintf(TRUE, FALSE, "%-45s: 打印PuId和Uri映射列表\n","  map12list()");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: 打印异域cmu的连接列表\n","  cmulist()");
	OspPrintf(TRUE, FALSE, "%-45s: 打印vtdu列表\n","  vtdulist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印pui列表\n","  puilist(LPCSTR szPuiType)");
    OspPrintf(TRUE, FALSE, "%-45s: 打印cui列表\n","  cuilist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印cu列表\n","  culist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印外设模块列表\n","  pelist()");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: 打印sip订阅事务列表\n","  sslist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印sip异域订阅事务列表\n","  dsslist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印redis订阅事务列表\n","  rsslist()");
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定设备的订阅者列表\n","  sserlist(LPCSTR szDevUri)");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: 打印交换列表（szDevChn：前端和通道，为空表示所有，格式：DevUri + \"_\" + 通道号，如：abcd@keda_1）\n","  sl/switchlist(szDevChn)");
	OspPrintf(TRUE, FALSE, "%-45s: 打印指定前端的交换信息（szDevChn：前端和通道，格式：DevUri + \"_\" + 通道号，如：abcd@keda_1）\n","  pusl(LPCSTR szDevChn)");
    OspPrintf(TRUE, FALSE, "%-45s: 打印指定cu的交换信息（szSession：cu的Session，通过culist可以获取到）\n","  cusl(LPCSTR szSession)");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: 打印断链告警列表\n","  nmsdalist()");
	OspPrintf(TRUE, FALSE, "%-45s: 打印过载告警列表\n","  nmsoalist()");
	OspPrintf(TRUE, FALSE, "%-45s: NA调试命令入口\n","  nacmd(u16 wCmdNum)");
	OspPrintf(TRUE, FALSE, "%-45s: 打印域列表信息\n","  pdt()");
    
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //基础模块的help命令
    OspPrintf(TRUE, FALSE, "%-45s: osp帮助命令\n","  osphelp()");
	OspPrintf(TRUE, FALSE, "%-45s: ospext帮助命令\n","  ospexthelp()");
	OspPrintf(TRUE, FALSE, "%-45s: ospsip帮助命令\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "%-45s: natagent帮助命令\n","  nahelp()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
}

#ifdef _LINUX_
void RegCmuCmd()
{
    //模块基本命令
    OspRegCommandEx("cmuhelp", (void *)cmuhelp, "", "");
    OspRegCommandEx("cmuver", (void *)cmuver, "", "");
    OspRegCommandEx("cmucfg", (void *)cmucfg, "", "");
    OspRegCommandEx("redissdk", (void *)redissdk, "", "");
    OspRegCommandEx("cmustat", (void *)cmustat, "", "");
    OspRegCommandEx("cmuquit", (void *)cmuquit,"", "");

    //日志级别相关命令
    OspRegCommandEx("logpara", (void *)logpara, "", "");
    //OspRegCommandEx("loglev", (void *)loglev, "", "i");
    OspRegCommandEx("corelog", (void *)corelog, "", "i");
    OspRegCommandEx("puilog", (void *)puilog, "", "i");
    OspRegCommandEx("cuilog", (void *)cuilog, "", "i");
    OspRegCommandEx("pulog", (void *)pulog, "", "i");
    OspRegCommandEx("culog", (void *)culog, "", "i");
    OspRegCommandEx("taslog", (void *)taslog, "", "ii");
    OspRegCommandEx("uaslog", (void *)uaslog, "", "i");
    OspRegCommandEx("nmslog", (void *)nmslog, "", "i");
    OspRegCommandEx("domainlog", (void *)domainlog, "", "i");
    OspRegCommandEx("sslog", (void *)sslog, "", "i");
    OspRegCommandEx("vtdulog", (void *)vtdulog, "", "i");
    OspRegCommandEx("switchlog", (void *)switchlog, "", "i");
    OspRegCommandEx("redislog", (void *)redislog, "", "i");
    OspRegCommandEx("logall", (void *)logall, "", "i");
    OspRegCommandEx("logoff", (void *)logoff, "", "");
    OspRegCommandEx("debug", (void *)debug, "", "i");

    //事务控制命令
    OspRegCommandEx("tasklist", (void *)tasklist, "", "s");
    OspRegCommandEx("task", (void *)task, "", "i");
    OspRegCommandEx("taskson", (void *)taskson, "", "i");
    OspRegCommandEx("IWillKillZombieTask", (void *)IWillKillZombieTask, "", "i");
    OspRegCommandEx("pnst", (void *)pnst, "", "i");
    OspRegCommandEx("psst", (void *)psst, "", "s");
    OspRegCommandEx("tracet", (void *)tracet, "", "i");
    OspRegCommandEx("tracent", (void *)tracent, "", "");

    //业务数据命令
	OspRegCommandEx("sumpu", (void *)sumpu,"", "s");
    OspRegCommandEx("pulist", (void *)pulist,"", "si");
    OspRegCommandEx("pu", (void *)pu,"", "s");
    OspRegCommandEx("domainlist", (void *)domainlist,"", "");
    OspRegCommandEx("syncparam", (void *)syncparam,"", "si");
	OspRegCommandEx("map12list", (void *)map12list,"", "");

    OspRegCommandEx("domain", (void *)domain,"", "s");
    OspRegCommandEx("cmulist", (void *)cmulist,"", "");
	OspRegCommandEx("vtdulist", (void *)vtdulist,"", "");
    OspRegCommandEx("puilist", (void *)puilist,"", "s");
    OspRegCommandEx("cuilist", (void *)cuilist,"", "");
    OspRegCommandEx("culist", (void *)culist,"", "");
    OspRegCommandEx("pelist", (void *)pelist,"", "");

    OspRegCommandEx("sslist", (void *)sslist,"", "");
    OspRegCommandEx("dsslist", (void *)dsslist,"", "");
    OspRegCommandEx("rsslist", (void *)rsslist,"", "");
    OspRegCommandEx("sserlist", (void *)sserlist,"", "s");

    OspRegCommandEx("switchlist", (void *)switchlist,"", "s");
    OspRegCommandEx("sl", (void *)switchlist,"", "s");
	OspRegCommandEx("pusl", (void *)pusl,"", "s");
    OspRegCommandEx("cusl", (void *)cusl,"", "s"); 

	OspRegCommandEx("adt", (void *)adt,"", "ssss");
	OspRegCommandEx("pdt", (void *)pdt,"", "");
	OspRegCommandEx("area", (void *)area,"", "i");

	OspRegCommandEx("nmsdalist", (void *)nmsdalist,"", "");
	OspRegCommandEx("nmsoalist", (void *)nmsoalist,"", "");

	//NA调试命令
	OspRegCommandEx("nahelp", (void *)nahelp,"", "");
	OspRegCommandEx("nacmd", (void *)nacmd,"", "i");

	OspRegCommandEx("SetMemoryReleaseRate", (void *)SetMemoryReleaseRate, "", "i");
	OspRegCommandEx("GetMemoryReleaseRate", (void *)GetMemoryReleaseRate, "", "");
	OspRegCommandEx("ReleaseFreeMemory", (void *)ReleaseFreeMemory, "", "");
	OspRegCommandEx("SetMemroyReleaseTimer", (void *)SetMemroyReleaseTimer, "", "i");
	
}
#endif

#define CMU_VER FORMAT_VER("cmu")

//cmu版本信息
API void cmuver()
{
    OspPrintf(TRUE, FALSE, CMU_VER);
}

//cmu配置信息
API void cmucfg()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CONF);
}

//redissdk信息
API void redissdk()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_REDISSDK);
}

//cmu状态信息
API void cmustat()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_STAT);
}

//显示log参数信息
API void logpara()
{
    OspShowLogParam();
}

//设置log级别
API void loglev(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
}

//设置corelog级别
API void corelog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_INS, byLev);
}

//设置puilog级别
API void puilog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_PUI, byLev);
}

//设置cuilog级别
API void cuilog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CUI, byLev);
}

//设置pulog级别
API void pulog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_PU, byLev);
}

//设置culog级别
API void culog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CU, byLev);
}

//设置3aslog级别
API void taslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_3AS, byLev);
}

//设置uaslog级别
API void uaslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_UAS, byLev);
}

//设置nmslog级别
API void nmslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_NMS, byLev);
}

//设置异域cmulog级别
API void domainlog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CMU, byLev);
}

//设置订阅log级别
API void sslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_SS, byLev);
}

//设置vtdulog级别
API void vtdulog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_VTDU, byLev);
}

//设置交换log级别
API void switchlog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_SWITCH, byLev);
}

//设置交换log级别
API void redislog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_REDIS, byLev);
}

//同时设置所有模块log级别
API void logall(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);

    OspSetModLogLev(OSPEXT, byLev);
    OspSetModLogLev(OSPSIP, byLev);

    OspSetModLogLev(CMU_MOD_INS, byLev);
    OspSetModLogLev(CMU_MOD_PUI, byLev);
    OspSetModLogLev(CMU_MOD_CUI, byLev);
    OspSetModLogLev(CMU_MOD_PU, byLev);
    OspSetModLogLev(CMU_MOD_BATCH, byLev);
    OspSetModLogLev(CMU_MOD_CU, byLev);
    OspSetModLogLev(CMU_MOD_3AS, byLev);
    OspSetModLogLev(CMU_MOD_UAS, byLev);
    OspSetModLogLev(CMU_MOD_NMS, byLev);
    OspSetModLogLev(CMU_MOD_CMU, byLev);
    OspSetModLogLev(CMU_MOD_VTDU, byLev);
    OspSetModLogLev(CMU_MOD_SS, byLev);
    OspSetModLogLev(CMU_MOD_SWITCH, byLev);
    OspSetModLogLev(CMU_MOD_REDIS, byLev);

    ospsiplog(byLev);
    ospextlog(byLev);
}

//关闭所有模块log级别
API void logoff()
{
    logall(0);
}

//打开调试模式
API void debug(bool bVal)
{
    g_bDebugOn = bVal;
}

//打印所有事务
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CMU_CORE_APP_ID, 0, szTaskType);
}

//打印指定事务信息
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CMU_CORE_APP_ID, 0);
}

//根据事务号打印指定事务的子事务，等同于pnst
API void taskson(u32 dwTaskNO)
{
    pnst(dwTaskNO);
}

//杀死指定僵尸事务，用于故障恢复或者测试，故意使用恶心的名称避免用户随意调用
API void IWillKillZombieTask(u32 dwTaskNO)
{
    OspKillTask(dwTaskNO, CMU_CORE_APP_ID, 0);
}

//根据事务号打印指定事务的子事务
API void pnst(u32 dwTaskNO)
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SON_TASK_LIST_BY_NO, &dwTaskNO, sizeof(dwTaskNO));
}

//根据sesskon号打印指定事务的子事务
API void psst(LPCSTR szSession)
{
    if (szSession == NULL)
    {
        GLOBELOG(ERROR_LEV, "szSession == NULL\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SON_TASK_LIST_BY_SESS, szSession, (u16)strlen(szSession)+1);
}

//trace 指定事务
API void tracet(u32 dwTaskNO)
{
    OspTraceTask(dwTaskNO);
}

//trace 下一个新建的事务
API void tracent()
{
    OspTraceNextTask();
}

//在线的pui列表
API void puilist(LPCSTR szPuiType)
{
    if (NULL != szPuiType)
    {
        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUI_LIST, szPuiType, (u16)strlen(szPuiType) + 1);
    }
    else
    {
        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUI_LIST);
    }
}

//pu列表
API void sumpu(LPCSTR szDomainName)
{
    TPrintDevParam tParam;

    if (NULL != szDomainName)
    {
        strncpy(tParam.m_achDomainName, szDomainName, sizeof(tParam.m_achDomainName) - 1);
    }

	::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SUM_PU, &tParam, sizeof(tParam));
}

API void pulist(LPCSTR szDomainName, u8 byState)
{
    TPrintDevParam tParam;
    tParam.m_byState = byState;

    if (NULL != szDomainName)
    {
        strncpy(tParam.m_achDomainName, szDomainName, sizeof(tParam.m_achDomainName) - 1);
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DEV_LIST, &tParam, sizeof(tParam));
}

//打印指定pu信息
API void pu(LPCSTR szDevUri)
{
    if (szDevUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "puid不能为空\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PU_INFO, szDevUri, (u16)strlen(szDevUri)+1);
}

//domain信息列表
API void domainlist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_LIST);
}

//域同步Task
API void syncparam(LPCSTR szDomainName, u32 dwSyncNumOnce)
{
    if (NULL != szDomainName)
    {
        TDomainSyncParam tParam;
        strncpy(tParam.szDomainName, szDomainName, sizeof(tParam.szDomainName) - 1);
        tParam.dwSyncNumOnce = dwSyncNumOnce;

        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_SYNC_PARAM, &tParam, sizeof(tParam));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "invalid param!\n");
    }
}

//{{{{{{{{{{{{{{{ add by Royan 20140513 智能告警主机
//1.0puid和2.0puUri映射表
API void map12list()
{
	::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUID_URI_MAP_LIST);
}
//}}}}}}}}}}}}}}} add by Royan 20140513 智能告警主机

//domain数据
API void domain(LPCSTR szDomainName)
{
    if (szDomainName == NULL)
    {
        OspPrintf(TRUE, FALSE, "szDomainName不能为空\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_DATA, szDomainName, (u16)strlen(szDomainName)+1);
}

//连接的cmu列表
API void cmulist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CMU_LIST);
}

//连接的vtdu列表
API void vtdulist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_VTDU_LIST);
}

//在线的cui列表
API void cuilist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CUI_LIST);
}

//在线的cu列表
API void culist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CU_LIST);
}

//在线的外设列表
API void pelist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PE_LIST);
}

//本域设备订阅事务列表
API void sslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SS_LIST);
}

//异域设备订阅事务列表
API void dsslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DSS_LIST);
}

//redis订阅事务列表
API void rsslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_RSS_LIST);
}

//查找设备的订阅者列表
API void sserlist(LPCSTR szDevUri)
{
    if (szDevUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szUserSess不能为空\n");
        return;
    }
	else
	{
		::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SSER_LIST, szDevUri, (u16)strlen(szDevUri)+1);
	}
}

//SwitchNode列表,DevChn格式："7d53960277f341858f00b043fb8636a3@china_0"
API void switchlist(const char* szDevChn)
{
    TPrintSwitchParam tParam;

    if (szDevChn)
    {
		string strDevChn(szDevChn);
		string strDevUri = strDevChn.substr(0, strDevChn.find_first_of("_"));
		string strChnNO  = strDevChn.substr(strDevChn.find_first_of("_") + 1);
		strncpy(tParam.m_achDevUri, strDevUri.c_str(), sizeof(tParam.m_achDevUri) - 1);
        tParam.m_byChn = (u8)atoi(strChnNO.c_str());
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SWITCH_NODE_LIST, &tParam, sizeof(tParam));
}

API void pusl(const char* szDevChn)
{
	if (NULL != szDevChn)
	{
		string strDevChn(szDevChn);

		TPrintSwitchParam tParam;
		string strDevUri = strDevChn.substr(0, strDevChn.find_first_of("_"));
		string strChnNO  = strDevChn.substr(strDevChn.find_first_of("_") + 1);
		strncpy(tParam.m_achDevUri, strDevUri.c_str(), sizeof(tParam.m_achDevUri) - 1);
		tParam.m_byChn = (u8)atoi(strChnNO.c_str());

		::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CERTAIN_PU_SWITCH_INFO, &tParam, sizeof(tParam));
	}
	else
	{
		OspPrintf(TRUE, FALSE, "szDevChn为空!!!\n");
	}
}

API void cusl(const char* szSession)
{
    if (NULL != szSession)
    {
        TPrintSwitchParam tParam;
        strncpy(tParam.m_achDevUri, szSession, sizeof(tParam.m_achDevUri) - 1);

        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CERTAIN_CU_SWITCH_INFO, &tParam, sizeof(tParam));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "szSession为空!!!\n");
    }
}

//add domain table
API void adt(LPCSTR szPeerDomainName, LPCSTR szPeerIP, LPCSTR szLocalIP, LPCSTR szOpType)
{
	TDomainItem tItem;
	tItem.SetPeerDomainName(szPeerDomainName);
	tItem.SetPeerIP(szPeerIP);
	tItem.SetLocalIP(szLocalIP);
	tItem.SetOperateType(DOMAIN_OP_TYPE_ADD);

	if (string(DOMAIN_OP_TYPE_ADD) == string(szOpType))
	{
		g_tDomainTable[szPeerDomainName] = tItem;
		OspPrintf(TRUE, FALSE, "Add Domain Table, Domain[%s]\n", szPeerDomainName);
	}
	else
	{
		g_tDomainTable.erase(szPeerDomainName);
		OspPrintf(TRUE, FALSE, "Del Domain Table, Domain[%s]\n", szPeerDomainName);
	}
}
//print domain table
API void pdt()
{
	for (TDomainTable::const_iterator it=g_tDomainTable.begin(); it!=g_tDomainTable.end(); it++)
	{
		OspPrintf(TRUE, FALSE, "Domain[%s], PeerIP[%s], LocalIP[%s], OpType[%s]\n",
			it->second.GetPeerDomainName().c_str(),
			it->second.GetPeerIP().c_str(),
			it->second.GetLocalIP().c_str(),
			it->second.GetOperateType().c_str());
	}

	OspPrintf(TRUE, FALSE, "DomainNum[%d]\n", g_tDomainTable.size());
}
//set display area info
API void area(u8 byCfg)
{
	
	bool bCfg = (0 == byCfg) ? false : true;
	g_cCmuConf.UpdateDisplayAreaInfoSwitch(bCfg);
}


//打印断链告警列表
API void nmsdalist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_NMS_DIS_ALARM_LIST);    
}

//打印过载告警列表
API void nmsoalist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_NMS_OVERLOAD_ALARM_LIST);
}

//Natagent帮助
API void nahelp()
{
	NAHelp();
}

//Natagent调试命令入口
API void nacmd(u16 wCmdNum)
{
	NACmd(wCmdNum);
}

//退出cmu
API void cmuquit()
{
    g_cMainSemLock.Give();
}


API void SetMemoryReleaseRate(int nRange)
{
	double range = nRange;
	OspPrintf(TRUE, FALSE, "Set Memory Release Rate = %g\n", range);
	MallocExtension::instance()->SetMemoryReleaseRate(range);
}
API void GetMemoryReleaseRate()
{
	double range = MallocExtension::instance()->GetMemoryReleaseRate();
	OspPrintf(TRUE, FALSE, "Memory Release Rate = %g\n", range);
}
API void ReleaseFreeMemory()
{
	MallocExtension::instance()->ReleaseFreeMemory();
}
API void SetMemroyReleaseTimer(int nSeconds)
{	
	OspPrintf(TRUE, FALSE, "Memory Release Timer from %d change to %d sec\n", g_cCmuConf.GetTcmallocReleaseInterval(), nSeconds);
	g_cCmuConf.SetTcmallocReleaseInterval(nSeconds);	
}
//初始化消息描述
void InitCmuMsgDesc()
{
    //内部消息
    OSP_ADD_EVENT_DESC(CMU_START);
    OSP_ADD_EVENT_DESC(CMU_QUIT);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CONF);
    OSP_ADD_EVENT_DESC(CMU_PRINT_REDISSDK);
    OSP_ADD_EVENT_DESC(CMU_PRINT_STAT);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PUI_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PU_INFO);
	OSP_ADD_EVENT_DESC(CMU_PRINT_DOMAIN_LIST);
	OSP_ADD_EVENT_DESC(CMU_PRINT_PUID_URI_MAP_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_DOMAIN_DATA);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CUI_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_VTDU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CMU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PE_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_DSS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_RSS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SSER_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SWITCH_NODE_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_NMS_DIS_ALARM_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_NMS_OVERLOAD_ALARM_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SON_TASK_LIST_BY_NO);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SON_TASK_LIST_BY_SESS);
    OSP_ADD_EVENT_DESC(CMU_CMU_DELETE);
    OSP_ADD_EVENT_DESC(CMU_CMU_DISCONNECT);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_SUCCESS);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_FAIL);
    OSP_ADD_EVENT_DESC(CMU_PU_STATUS_UPDATE);
    OSP_ADD_EVENT_DESC(CMU_SWITCH_FINISH_CB);
    OSP_ADD_EVENT_DESC(CMU_DEV_ONLINE_NTF);
    OSP_ADD_EVENT_DESC(CMU_DEV_OFFLINE_NTF);
	OSP_ADD_EVENT_DESC(CMU_PRINT_CERTAIN_PU_SWITCH_INFO);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CERTAIN_CU_SWITCH_INFO);

    OSP_ADD_EVENT_DESC(CMU_FASTDB_SUB_CB);
    OSP_ADD_EVENT_DESC(CMU_REDISSDK_DISC_CB);
    OSP_ADD_EVENT_DESC(CMU_REDISSDK_SUB_CB);
    OSP_ADD_EVENT_DESC(CMU_DOMAIN_REDIS_SYNC);
    OSP_ADD_EVENT_DESC(CMU_SLIDING_WINDOW_PROC);

    //网络消息
    InitCmsEventDesc();
    InitTacTasEventDesc();
    InitUasEventDesc();
}

//初始化cmu内各模块日志级别
void InitCmuModuleLog()
{

#ifdef DEBUG
    u8 byCmuLogLev = EVENT_LEV;
#else
    u8 byCmuLogLev = ERROR_LEV;
#endif

    OSP_SET_MOD_LOG(CMU_MOD_INS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CUI, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_PUI, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_PU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_BATCH, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_3AS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_UAS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_NMS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CMU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_VTDU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_SWITCH, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_SS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_REDIS, byCmuLogLev);
}