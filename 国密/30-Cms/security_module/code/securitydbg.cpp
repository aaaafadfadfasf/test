#include "securitydbg.h"
#include "cms/ospext/osptool.h"
#include "cms/cms_proto.h"
#include "cms/ospsip/ospsip.h"
using namespace ::std;

API void securityhelp()
{
	OspPrintf(TRUE, FALSE, "%-20s: security帮助命令\n","  securityhelp");
	OspPrintf(TRUE, FALSE, "%-20s: security版本信息\n","  securityver");
	OspPrintf(TRUE, FALSE, "%-20s: security配置信息\n","  securitycfg()");
	OspPrintf(TRUE, FALSE, "%-20s: 打印security的状态\n","  securitystat()");
	OspPrintf(TRUE, FALSE, "%-20s: 退出进程\n","  securityquit()");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");


	OspPrintf(TRUE, FALSE, "%-20s   : 显示log参数信息\n","  logpara()");
	OspPrintf(TRUE, FALSE, "%-20s   : 打开log参数信息\n","  logall(u8 byLev)");
	OspPrintf(TRUE, FALSE, "%-20s   : 关闭log参数信息\n","  logoff()");


	OspPrintf(TRUE, FALSE, "\n  所有模块使用的日志级别列表:\n");
	OspPrintf(TRUE, FALSE, "  NOLOG_LEV------0            //无log输出:\n");
	OspPrintf(TRUE, FALSE, "  ERROR_LEV------1            //错误级别:\n");
	OspPrintf(TRUE, FALSE, "  WARNING_LEV------2          //警告级别:\n");
	OspPrintf(TRUE, FALSE, "  CRITICAL_LEV------3         //关键级别，比如设备登录登出，录像开启关闭:\n");
	OspPrintf(TRUE, FALSE, "  EVENT_LEV------4            //普通级别，比如收到一条消息打印一下:\n");
	OspPrintf(TRUE, FALSE, "  PROGRESS_LEV------5         //进度通知级别，比如前端放像进度通知，大批量查询通知:\n");
	OspPrintf(TRUE, FALSE, "  TIMER_LEV------6            //频繁的定时器级别，比如定时心跳等:\n");
	OspPrintf(TRUE, FALSE, "  ALL_LEV------7              //所有级别放开:\n");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

	OspPrintf(TRUE, FALSE, "%-20s: 打印当前所有任务\n","  tasklist(LPCSTR szTaskType = NULL)");
	OspPrintf(TRUE, FALSE, "%-20s: 根据taskNO打印指定任务\n","  task(u32 dwTaskNO)");
	OspPrintf(TRUE, FALSE, "%-20s: trace指定事务\n","  tracet(u16 wTaskNO)");
	OspPrintf(TRUE, FALSE, "%-20s: trace下一个新建事务\n","  tracent()");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

	//基础模块的help命令
	OspPrintf(TRUE, FALSE, "%-20s: osp帮助命令\n","  osphelp()");
	OspPrintf(TRUE, FALSE, "%-20s: ospext帮助命令\n","  ospexthelp()");
	OspPrintf(TRUE, FALSE, "%-20s: ospsip帮助命令\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

}



void RegSecurityCmd()
{
	OspRegCommand("securityhelp", (void *)securityhelp, "");
	OspRegCommand("securityver", (void *)securityver, "");
	OspRegCommand("securitycfg", (void *)securitycfg, "");
	OspRegCommand("securitystat", (void*)securitystat, "");
	OspRegCommand("securityquit", (void *)securityquit,"");

	OspRegCommand("logpara", (void *)logpara, "");
	OspRegCommand("loglev", (void *)loglev, "");
	OspRegCommand("logall", (void *)logall, "");
	OspRegCommand("logoff", (void *)logoff, "");


	OspRegCommand("tasklist", (void *)tasklist, "");
	OspRegCommand("task", (void *)task, "");
	OspRegCommand("tracet", (void *)tracet, "");
	OspRegCommand("tracent", (void *)tracent, "");

	OspRegCommand("newvkek", (void *)newvkek, "");
}


API void newvkek()
{
	CMessage *const pcMsg = new CMessage();
	COspSipMsg *pcOspSipMsg = new COspSipMsg();
	CSecurityModuleNewVkekReq *cReq = new CSecurityModuleNewVkekReq();
	cReq->SetModuleId("123456");
	cReq->SetVkekVersion("1.0");
	cReq->SetWithId("9999");
	//pcOspSipMsg->SetMsgBody(cReq, (cReq->ToXml()).size());
	pcMsg->content = (u8 *)pcOspSipMsg;
	pcMsg->event = SECURITY_MODULE_NEW_VKEK_REQ;
	::OspPost(COREAPPIID, SECURITY_MODULE_NEW_VKEK_REQ, (void *)pcMsg, sizeof(CMessage));
}


#define SECURITY_VER FORMAT_VER("security")


API void securityver()
{
	OspPrintf(TRUE, FALSE, SECURITY_VER);
}


//security配置信息
API void securitycfg()
{
	//g_cPmcsCfg.PrintData();
}


//显示log参数信息
API void logpara()
{
	OspShowLogParam();
}


//设置log级别
API void loglev(u8 byLev)
{
	OspSetLogLev(byLev, AID_COREAPP);
}


API void logall(u8 byLev)
{
	OspSetLogLev(byLev, AID_COREAPP);

	OspSetModLogLev(OSPEXT, byLev);
	OspSetModLogLev(OSPSIP, byLev);

	ospsiplog(byLev);
	ospextlog(byLev);
}

API void logoff()
{
	logall(0);
}


//打印所有事务
API void tasklist()
{
	OspPrintTask(AID_COREAPP, 0);
}


//打印指定事务信息
API void task(u16 wTaskNO)
{
	OspPrintOneTask(wTaskNO, AID_COREAPP, 0);
}


//trace 指定事务
API void tracet(u16 wTaskNO)
{
	OspTraceTask(wTaskNO);
}

//trace 下一个新建的事务
API void tracent()
{
	OspTraceNextTask();
}


API void securitystat()
{
	OspPost(COREAPPIID, ModulePrintStat);
}

//退出cmu
API void securityquit()
{
	OspSemGive(g_hSecurityModuleExitSem);
}