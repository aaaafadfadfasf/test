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
        TelePrint(" �����Ƿ�");
        return;
    }

    TelePrint("");
	TelePrint("TAS����ʱ��------[%s]\n", CTasConfig::GetStartTime().c_str());
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
    TelePrint(" ��������===========================================================");
    TelePrint(" tashelp         : ��ʾ������Ϣ");
    TelePrint(" tasver          : ��ʾ�汾��Ϣ");
    TelePrint(" tascfg          : ��ʾ������Ϣ");
    TelePrint(" tasstat(level)  : ��ʾ�ڲ�״̬����. levelΪ0ʱ��ӡ����״̬, levelΪ1ʱ��ӡ��ϸ״̬");
    TelePrint(" tasquit         : �˳�����");
    TelePrint(" tasklist        : ��ʾ�����б�����Ϊtts");
    TelePrint(" task(taskno)    : ��ʾָ���������ϸ��Ϣ��tasknoΪ����ţ�����Ϊtts1");
	TelePrint(" unlockalluserpwd  : �������б��������û�");
    TelePrint(" ------------------------------------------------------------------");
    TelePrint(" logpara         : ��ʾ��־ϵͳ��Ϣ���ڲ���ʾ��־�������־ģ�����Ϣ");
    TelePrint(" logall(level)   : ����ȫ��ģ�����־����levelΪ��־����, ���ļ�����ֵ��ο�logpara");
    TelePrint(" logoff          : �ر�ȫ��ģ�����־");
    TelePrint(" tla             : ����ȫ��ģ�����־����Ϊ��󼶱�");
    TelePrint(" tle             : ����ȫ��ģ�����־����Ϊ���󼶱�");
    TelePrint(" taslog(model,level): ����ָ��ģ�����־����ģ�����ֵ����־������ֵ��ο�logpara");
    TelePrint(" cctrllog(level) : ��������app����־����levelΪ��־����, ���ļ�����ֵ��ο�logpara");
    TelePrint(" dblog(level)    : �������ݿ�����̵߳���־����levelΪ��־����, ���ļ�����ֵ��ο�logpara");
    TelePrint(" nmslog(level)   : ��������app����־����levelΪ��־����, ���ļ�����ֵ��ο�logpara");
    TelePrint(" licenselog(level): ����license app����־����levelΪ��־����, ���ļ�����ֵ��ο�logpara");
    TelePrint(" ------------------------------------------------------------------");
    TelePrint(" ospexthelp      : oxpext��������");
    TelePrint(" ospsiphelp      : ospsip��������");
    TelePrint(" osphelp         : osp��������");
    TelePrint(" dblogon         : ����dbi��־");
    TelePrint(" dblogoff        : �ر�dbi��־");
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
