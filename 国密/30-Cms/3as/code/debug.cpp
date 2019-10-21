#include "osp/osp.h"
#include "cms/cms_proto.h"
#include "db_task_pool.h"
#include "config_util/tas_config.h"
#include "log_system.h"
#include "nmsconn.h"

extern CDbTaskPool g_cDbTaskPool;
extern SEMHANDLE g_hTasExitSem;

API void tasquit()
{
    OspSemGive(g_hTasExitSem);
}

API void tts()
{
    OspPrintTask(AID_CCtrl, 0);
    OspPrintTask(AID_SyncAnalyze, 0);
    OspPrintTask(AID_License, 0);
}

API void tts1(u32 dwTaskNo)
{
    OspPrintOneTask(dwTaskNo, AID_CCtrl, 0);
}

API void tascfg()
{
    CTasConfig::Dump();
}

API void tasstat(u32 dwCode)
{
    if (!(0 == dwCode || 1 == dwCode))
    {
        TelePrint(" 参数非法");
        return;
    }

    TelePrint("");
	TelePrint("TAS启动时间------[%s]\n", CTasConfig::GetStartTime().c_str());
    OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgDump, &dwCode, sizeof(dwCode));
    OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgDump, &dwCode, sizeof(dwCode));
    OspPost(MAKEIID(AID_License, CInstance::DAEMON), msgDump, &dwCode, sizeof(dwCode));
    g_cDbTaskPool.Dump(dwCode);
    TelePrint("\nNMS state: %s\n", g_cNmcLib.HasConnectedNms()? "connected":"disconnected");
}

#define TAS_VER FORMAT_VER("tas")
API void tasver()
{
    TelePrint(TAS_VER);
}

API void unlockalluserpwd()
{
	g_cUserPwdLockMng.UnlockAllUserPwd();
}

API void tashelp()
{
    TelePrint(" 帮助命令===========================================================");
    TelePrint(" tashelp         : 显示帮助信息");
    TelePrint(" tasver          : 显示版本信息");
    TelePrint(" tascfg          : 显示配置信息");
    TelePrint(" tasstat(level)  : 显示内部状态数据. level为0时打印基本状态, level为1时打印详细状态");
    TelePrint(" tasquit         : 退出进程");
    TelePrint(" tasklist        : 显示任务列表，别名为tts");
    TelePrint(" task(taskno)    : 显示指定任务的详细信息，taskno为任务号，别名为tts1");
	TelePrint(" unlockalluserpwd  : 解锁所有被锁定的用户");
    TelePrint(" ------------------------------------------------------------------");
    TelePrint(" logpara         : 显示日志系统信息，内部显示日志级别和日志模块号信息");
    TelePrint(" logall(level)   : 设置全部模块的日志级别，level为日志级别, 它的级别数值请参考logpara");
    TelePrint(" logoff          : 关闭全部模块的日志");
    TelePrint(" tla             : 设置全部模块的日志级别为最大级别");
    TelePrint(" tle             : 设置全部模块的日志级别为错误级别");
    TelePrint(" taslog(model,level): 设置指定模块的日志级别，模块号数值和日志级别数值请参考logpara");
    TelePrint(" cctrllog(level) : 设置中心app的日志级别，level为日志级别, 它的级别数值请参考logpara");
    TelePrint(" dblog(level)    : 设置数据库操作线程的日志级别，level为日志级别, 它的级别数值请参考logpara");
    TelePrint(" nmslog(level)   : 设置网管app的日志级别，level为日志级别, 它的级别数值请参考logpara");
    TelePrint(" licenselog(level): 设置license app的日志级别，level为日志级别, 它的级别数值请参考logpara");
    TelePrint(" ------------------------------------------------------------------");
    TelePrint(" ospexthelp      : oxpext帮助命令");
    TelePrint(" ospsiphelp      : ospsip帮助命令");
    TelePrint(" osphelp         : osp帮助命令");
    TelePrint(" dblogon         : 开启dbi日志");
    TelePrint(" dblogoff        : 关闭dbi日志");
}

void RegisterDebugAPI()
{
#ifdef _LINUX_
    OspRegCommandEx("tashelp", (void*)tashelp, "tashelp", "");
    OspRegCommandEx("tasver", (void*)tasver, "tasver", "");
    OspRegCommandEx("tascfg", (void*)tascfg, "tascfg", "");
    OspRegCommandEx("tasstat", (void*)tasstat, "tasstat", "i");
    OspRegCommandEx("tasquit", (void*)tasquit, "tasquit", "");
    OspRegCommandEx("tasklist", (void*)tts, "show tas tasks, alias is tts", "");
    OspRegCommandEx("tts", (void*)tts, "show tas tasks", "");
    OspRegCommandEx("task", (void*)tts1, "show tas's one task, alias is tts1", "i");
    OspRegCommandEx("tts1", (void*)tts1, "show tas's one task", "i");
    OspRegCommandEx("logpara", (void*)logpara, "show tas log system info", "");
    OspRegCommandEx("logall", (void*)logall, "set tas to the certain log level, refer to logpara for the value of loglevel", "i");
    OspRegCommandEx("logoff", (void*)logoff, "set tas to the none log level", "");
    OspRegCommandEx("tla", (void*)tla, "set tas to the max log level", "");
    OspRegCommandEx("tle", (void*)tle, "set tas to the error log level", "");
    OspRegCommandEx("taslog", (void*)taslog, "set tas module log level", "i");
    OspRegCommandEx("cctrllog", (void*)cctrllog, "set tas cctrl log level", "i");
    OspRegCommandEx("dblog", (void*)dblog, "set tas db log level", "i");
    OspRegCommandEx("nmslog", (void*)nmslog, "set tas nms log level", "i");
    OspRegCommandEx("licenselog", (void*)licenselog, "set tas license log level", "i");
	OspRegCommandEx("unlockalluserpwd", (void*)unlockalluserpwd, "unlock all user", "");
#endif
}
