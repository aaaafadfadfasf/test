#include "cuidbg.h"
#include "cuiapp.h"
#include "sdk/redissdk/redissdk.h"

#define PrintFunGroupDesc(x) OspPrintf(TRUE, FALSE, x"\n")
#define PrintFunDesc(fun, funDesc) OspPrintf(TRUE, FALSE, "\t%2d "#fun":\t"funDesc"\n", wIndex++)

extern CRedisSdk  g_cCuiRedisSdk;

API void cuihelp()
{
    cuiver();
    u16 wIndex = 1;
    PrintFunGroupDesc("\nCUI帮助命令");
    PrintFunDesc(cuihelp(), "CUI帮助命令");
    PrintFunDesc(cuiver(), "打印CUI版本信息和编译信息");
    PrintFunDesc(cuicfg(), "打印CUI配置信息");
    PrintFunDesc(cuistat(), "打印CUI的状态");
    PrintFunDesc(cuiquit(), "退出CUI");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    PrintFunDesc(logpara(),"显示log参数信息\n");
    PrintFunDesc(logall(u32 bLogAll), "打开所有模块的所有等级的日志，日志级别参见logparam");
    PrintFunDesc(logoff(), "关闭所有模块的所有等级的日志");
    PrintFunDesc(cuilogvc(u32 bLogErr), "打开APP模块的日志, 0 表示EVENT等级，1 表示ERROR等级");
    PrintFunDesc(cuilogerr(), "开打所有模块的错误级别的日志");
    PrintFunDesc(cuilogtask(u32 bLogErr), "打开LOGIN_TASK模块的日志, 0 表示EVENT等级，1 表示ERROR等级");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    //事务控制命令
	PrintFunDesc(vtdunatclear(LPCSTR szUserSess), "清楚当前用户所有NAT探测包信息,cuSession为空则清楚所有nat探测包");
	PrintFunDesc(getvslist(), "打印CUI缓存的视频源状态列表");
    PrintFunDesc(tasklist(LPCSTR szTaskType), "打印当前所有任务");
    PrintFunDesc(task(u32 dwTaskNO), "根据taskNO打印指定任务");
    PrintFunDesc(taskson(u32 dwTaskNO), "根据taskNO打印指定任务子事务列表");
    PrintFunDesc(IWillKillZombieTask(u32 dwTaskNO), "kill指定僵尸事务");
    PrintFunDesc(tracet(u16 wTaskNO), "trace指定事务");
    PrintFunDesc(tracent(), "trace下一个新建事务");
    PrintFunDesc(cuitaskshow(), "打印所有Task信息，简称ts()");
    PrintFunDesc(taslist(u32 bPrintUserDevList), "打印所有TAS信息，默认 0 表示不打印用户设备列表，1 表示打印用户设备列表");
    PrintFunDesc(ds(), "打印所有SIP Dialog绑定信息");
    PrintFunDesc(hb(), "打印所有SIP的HeartBeat列表");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    PrintFunDesc(culist(u32 bPrintSubTask), "打印在线CU列表，默认 0 表示不打印子任务列表，1 表示打印子任务的详细信息");
    PrintFunDesc(domain(), "打印域列表信息");
    PrintFunDesc(taskmap(), "打印Task映射表，目前有三种列表");
    PrintFunDesc(mailsize(), "打印邮箱中的消息个数");
    PrintFunDesc(pe(), "打印当前连接的外设模块");
    PrintFunDesc(userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri), "打印用户设备相关信息，订阅情况，参数:用户ID，设备ID");
    PrintFunDesc(pudata(LPCSTR szDevUrl), "查看获取到的设备名称，视频源别名, 指定设备的订阅情况");
    PrintFunDesc(pcq(LPCSTR szCuSession), "打印指定cu会话的请求消息内容,会话号为空时表示不打印");
    PrintFunDesc(toptime(), "查看比较耗时的事件");
    PrintFunDesc(settoptime(u32 dwNum), "当为0时清空toptime队列信息,为非0时设置队列大小");
	PrintFunDesc(getptzlock(LPCSTR szDevId), "查看设备下锁定PTZ的视频源，devId为空时打印所有有PTZ锁定的视频源");
	PrintFunDesc(getbatopt(), "查看CU发起的批量操作");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
	OspPrintf(TRUE, FALSE, "设置 CUI配置信息\n");
	PrintFunDesc(setcuntfsize(s32 nCuNtfMaxSize), "设置CU通知消息队列最大限制，默认为0(int 最大值2147483647), 最小值1000");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "%-20s: osp帮助命令\n","  osphelp()");
    OspPrintf(TRUE, FALSE, "%-20s: ospext帮助命令\n","  ospexthelp()");
    OspPrintf(TRUE, FALSE, "%-20s: ospsip帮助命令\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "%-20s: vtdu帮助命令\n","  vtduhelp()");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
}

void CuiRegHelpCmd()
{
#ifdef _LINUX_
	OspRegCommandEx("cuihelp", (void*)cuihelp, "", "");
	OspRegCommandEx("cuiver", (void*)cuiver, "", "");
	OspRegCommandEx("cuicfg", (void*)cuicfg, "", "");
	OspRegCommandEx("cuistat", (void*)cuistat, "", "");
	OspRegCommandEx("cuiquit", (void*)cuiquit, "", "i");
	OspRegCommandEx("logpara", (void*)logpara, "", "");                   
	OspRegCommandEx("logall", (void*)logall, "", "i");
	OspRegCommandEx("logoff", (void*)logoff, "", "");
	OspRegCommandEx("cuilogvc", (void*)cuilogvc, "", "i");
	OspRegCommandEx("cuilogerr", (void*)cuilogerr, "", "");
	OspRegCommandEx("cuilogtask", (void*)cuilogtask, "", "i");
	OspRegCommandEx("vtdunatclear", (void*)vtdunatclear, "", "s");
	OspRegCommandEx("getptzlock", (void*)getptzlock, "", "s");
	OspRegCommandEx("getvslist", (void*)getvslist, "", "");
	OspRegCommandEx("tasklist", (void*)tasklist, "", "s");
	OspRegCommandEx("task", (void*)task, "", "i");
	OspRegCommandEx("taskson", (void*)taskson, "", "i");
	OspRegCommandEx("IWillKillZombieTask", (void*)IWillKillZombieTask, "", "i");
	OspRegCommandEx("tracet", (void*)tracet, "", "i");
	OspRegCommandEx("tracent", (void*)tracent, "", "");
	OspRegCommandEx("cuitaskshow", (void*)cuitaskshow, "", "");
	OspRegCommandEx("ts", (void*)ts, "", "");
	OspRegCommandEx("ds", (void*)ds, "", "");
	OspRegCommandEx("hb", (void*)hb, "", "");
	OspRegCommandEx("culist", (void*)culist, "", "i");
	OspRegCommandEx("taslist", (void*)taslist, "", "i");
	OspRegCommandEx("domain", (void*)domain, "", "");
	OspRegCommandEx("taskmap", (void*)taskmap, "", "");
	OspRegCommandEx("mailsize", (void*)mailsize, "", "");
	OspRegCommandEx("pe", (void*)pe, "", "");
	OspRegCommandEx("userdevinfo", (void*)userdevinfo, "", "s");
	OspRegCommandEx("pudata", (void*)pudata, "", "s");
	OspRegCommandEx("pcq", (void*)pcq, "", "s");
	OspRegCommandEx("toptime", (void*)toptime, "", "");
	OspRegCommandEx("settoptime", (void*)settoptime, "", "i");
	OspRegCommandEx("logparam", (void*)logparam, "", "");
	OspRegCommandEx("pt", (void*)pt, "", "i");
	OspRegCommandEx("tt", (void*)tt, "", "i");
	OspRegCommandEx("emulate_sipmsg", (void*)emulate_sipmsg, "", "iiis");
	OspRegCommandEx("setcuntfsize", (void*)setcuntfsize, "", "i");
	OspRegCommandEx("getbatopt", (void*)getbatopt, "", "");
	OspRegCommandEx("redissdk", (void*)GetRedis, "", "");

#endif
}

API void GetRedis()
{
    g_cCuiRedisSdk.PrintData();
}

API void logpara()
{
    OspShowLogParam();
}

API void logall(u8 bLogAll)
{
//     u8 byLogLev = EVENT_LEV;
//     if ( bLogAll )
//     {
//         byLogLev = ALL_LEV;
//     }
    liblog(bLogAll);

    OspSetLogLev(bLogAll, CUIAPP_AID);
    OSP_SET_MOD_LOG(CUI_APP, bLogAll);
    OSP_SET_MOD_LOG(CUI_DATA, bLogAll);
    OSP_SET_MOD_LOG(LOGIN_TASK, bLogAll);
    OSP_SET_MOD_LOG(NORMAL_TASK, bLogAll);
	OSP_SET_MOD_LOG(TCP_THREAD, bLogAll);
	OSP_SET_MOD_LOG(REDIS_LOG, bLogAll);
}

void liblog(u8 byLog)
{
    ospsiplog(byLog);
    ospextlog(byLog);
    OSP_SET_MOD_LOG(OSPEXT, byLog);
    OSP_SET_MOD_LOG(OSPSIP, byLog);
}

API void logoff()
{
    ospsiplog(NOLOG_LEV);
    ospextlog(NOLOG_LEV);
    OspSetLogLev(NOLOG_LEV, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, NOLOG_LEV);
    OSP_SET_MOD_LOG(CUI_DATA, NOLOG_LEV);
    OSP_SET_MOD_LOG(LOGIN_TASK, NOLOG_LEV);
    OSP_SET_MOD_LOG(NORMAL_TASK, NOLOG_LEV);
    OSP_SET_MOD_LOG(TCP_THREAD, NOLOG_LEV);
}

API void cuilogvc( u32 bLogErr )
{
    u8 byLogLev  = ERROR_LEV;
    if ( bLogErr == 0 )
    {
        byLogLev  = EVENT_LEV;
    }
    ospsiplog(byLogLev);
    ospextlog(byLogLev);
    OspSetLogLev(byLogLev, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, byLogLev);
}

API void cuilogerr()
{
    ospsiplog(ERROR_LEV);
    ospextlog(ERROR_LEV);
    OspSetLogLev(ERROR_LEV, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, ERROR_LEV);
    OSP_SET_MOD_LOG(CUI_DATA, ERROR_LEV);
    OSP_SET_MOD_LOG(LOGIN_TASK, ERROR_LEV);
    OSP_SET_MOD_LOG(NORMAL_TASK, ERROR_LEV);
    OSP_SET_MOD_LOG(TCP_THREAD, ERROR_LEV);
}

API void cuilogtask( u32 bLogErr )
{
    u8 byLogLev  = ERROR_LEV;
    if ( bLogErr == 0 )
    {
        byLogLev  = EVENT_LEV;
    }
    ospsiplog(byLogLev);
    ospextlog(byLogLev);
    OspSetLogLev(byLogLev, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(LOGIN_TASK, byLogLev);
    OSP_SET_MOD_LOG(NORMAL_TASK, byLogLev);
}

//打印所有事务
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CUIAPP_AID, 0, szTaskType);
}

//打印指定事务信息
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CUIAPP_AID, 0);
}

//根据事务号打印指定事务的子事务
API void taskson(u32 dwTaskNO)
{
    culist(dwTaskNO);
}

//杀死指定僵尸事务，用于故障恢复或者测试，故意使用恶心的名称避免用户随意调用
API void IWillKillZombieTask(u32 dwTaskNO)
{
    OspKillTask(dwTaskNO, CUIAPP_AID, 0);
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

API void culist(u32 bPrintSubTask)
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_CU_LIST, &bPrintSubTask, sizeof(bPrintSubTask));
}

API void cuitaskshow()
{
    OspPrintTask(CUIAPP_AID);
}

API void ts()
{
    cuitaskshow();
}


API void taslist(u32 bPrintUserDevList)
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_TAS_LIST, &bPrintUserDevList, sizeof(bPrintUserDevList));
}

API void domain()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_DOMAIN_LIST);
}

API void ds()
{
    OspPrintf(TRUE, FALSE, "-----------------------OSP SIP的Dialog列表-----------------------\n");
    ospsipdlg();
    OspPrintf(TRUE, FALSE, "-----------------------OSP SIP的Transaction列表------------------\n");
    ospsiptrans();
}

API void hb()
{
    ospsiphblist();
}

API void cuiver()
{
    OspPrintf(TRUE, FALSE, CUI_VER);
}

API void cuicfg()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_CONFIG);
}

API void logparam()
{
    OspShowLogParam();
}

API void pt( u32 dwTaskNO )
{
    OspPrintOneTask(dwTaskNO, CUIAPP_AID, 0);
}

API void tt( u32 dwTaskNO )
{
    OspTraceTask(dwTaskNO);
}

API void cuistat()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_STATUS);
}

API void taskmap()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_TASK_MAP);
}

API void mailsize()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_MAIL_SIZE);
}

API void pe()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_PE_AGENT);
}

API void userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri/* = NULL*/)
{
	TUserDevData tUserDevData;
    if(szUserSess == NULL)
    {
        OspPrintf(TRUE, FALSE, "szUserSess不能为空\n");
        return;
    }

	string strUserUUid(szUserSess);

	TSipURI tSipUri;
	if (tSipUri.SetURIString(strUserUUid))
	{
		strUserUUid = tSipUri.GetUser();
	}
	strncpy(tUserDevData.szDevUUid, strUserUUid.c_str(), sizeof(s8) * 32);

	if (szDevUri != NULL)
	{
		string strDevUUid(szDevUri);

		if ( tSipUri.SetURIString(strDevUUid) )
		{
			strDevUUid = tSipUri.GetUser();
		}
		
		strncpy(tUserDevData.szDevUUid, strDevUUid.c_str(), sizeof(s8)*32);
	}

    ::OspPost(CUIAPP_DAEMON, CUI_PRINT_USER_DEVINFO, &tUserDevData, sizeof(TUserDevData));
}

API void pudata(LPCSTR szDevUrl)
{
    if(szDevUrl == NULL)
	{
		OspPrintf(TRUE, FALSE, "szDevUrl不能为空\n");
        return;
    }
    ::OspPost(CUIAPP_DAEMON, CUI_PRINT_PUDATA, szDevUrl, (u16)(strlen(szDevUrl)+1));
}

API void toptime()
{
    g_cElapsedTimeList.PrintData();
}

API void settoptime(u32 dwNum)
{
    if(0 == dwNum)
    {
        g_cElapsedTimeList.Clear();
    }
    else
    {
        g_cElapsedTimeList.SetMaxNum(dwNum);
    }
}

API void pcq(LPCSTR szCuSession)
{
    if (NULL == szCuSession)
    {
        ::OspPost(CUIAPP_DAEMON, CUI_PRINT_CUREQ);
    }
    else
    {
        ::OspPost(CUIAPP_DAEMON, CUI_PRINT_CUREQ, szCuSession, (u16)(strlen(szCuSession)+1));
    }
}

API void emulate_sipmsg(u32 dwTaskNo, s32 nSipError, s32 nEvent, LPCSTR szEventContent)
{
    TCuiEmulateSipMsg tMsg;
    tMsg.dwDstTaskno = dwTaskNo;
    tMsg.nSipErrorno = nSipError;
    tMsg.nEvent = nEvent;
    if (NULL != szEventContent)
    {
        strncpy(tMsg.szMsgBody, szEventContent, sizeof(tMsg.szMsgBody));
        tMsg.szMsgBody[sizeof(tMsg.szMsgBody)-1] = '\0';
    }
    
    ::OspPost(CUIAPP_DAEMON, CUI_EMULATE_SIPMSG, &tMsg, sizeof(tMsg));
}

API void vtdunatclear(LPCSTR szUserSess) //清空用户NAT探测包
{
	u16 len = 0;
	if ( szUserSess == NULL )
	{
		::OspPrintf(TRUE,FALSE,"ClearAllVtduNatData\n");
	}
	else
	{
		len=(u16)(strlen(szUserSess)+1);
	}
	
	::OspPost(CUIAPP_DAEMON,CUI_CLEAR_VTDUNATPACKET,szUserSess, len);
}

API void getptzlock(LPCSTR szDevId)
{
	u16 len = 0;
	if (szDevId == NULL)
	{
		::OspPrintf(TRUE, FALSE, "打印所有被PTZ锁定的设备视频源\n");
	}
	else
	{
		len = (u16)(strlen(szDevId) + 1);
	}

	::OspPost(CUIAPP_DAEMON, CUI_PRINT_PTZ_LOCK_DEV_VS_LIST, szDevId, len);
}

API void getvslist()
{
	OspPost(CUIAPP_DAEMON, CUI_PRINT_VS_STATUS_LIST);
}

API void setcuntfsize(s32 nCuNtfMaxSize /*= 0*/)
{
	OspPost(CUIAPP_DAEMON, CUI_SET_CU_NTF_MAX_SIZE, &nCuNtfMaxSize, sizeof(nCuNtfMaxSize));
}

API void getbatopt()
{
	OspPost(CUIAPP_DAEMON, CUI_PRINT_CU_BAT_OPT_LIST);
}
