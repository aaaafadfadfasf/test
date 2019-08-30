#include "cms/ospext/ospdbg.h"
#include "cms/ospext/osptask.h"
#include "cbb/sysdbg/sysdbg.h"

#include "taskmng.h"
#include "ospexevt.h"
#include "osplogfile.h"

extern u8 g_byGlbLogLev;
extern u8 g_byPrintTimeLev;
extern u8 g_byPrintCompileInfo;
extern u8 g_byPrintOspPrefix;
extern u32 g_dwTraceAppInst;
extern u32 g_dwTraceTaskNO;
extern BOOL32 g_bIsOspExtWriteRunLog;
extern BOOL32 g_bIsOspExtWriteErrLog;
extern BOOL32 g_bIsMonitorTimePerform;
#ifdef _LINUX_
extern BOOL32 g_bIsParseMangleName; //linux是否进行mangle name的解析
#endif
extern u8 g_byLogFileLev;
extern CXMap<u32, TLogModInfo> g_cModuleLogLev;

extern CXLogFile g_cErrLogFile;
extern CXLogFile g_cRunLogFile;

//缓存使用过的调试命令，用于次使用时的提示
class CDbgCmdCache
{
    enum{ DEL_CMD_CACHE_NUM = 20, MAX_CMD_CACHE_NUM = 50};
public:
    CDbgCmdCache() : m_cDbgCmdArray(MAX_CMD_CACHE_NUM), m_nCurrIdx(0)
    {
    }
    
    string UpCmd()
    {
        if (m_cDbgCmdArray.size() == 0)
        {
            m_nCurrIdx = 0;
            return "no cmd";
        }

        if (m_nCurrIdx >= m_cDbgCmdArray.size())
        {
            m_nCurrIdx = m_cDbgCmdArray.size() - 1;
            return "no cmd";        
        }

        if (m_nCurrIdx == 0)
        {
            return m_cDbgCmdArray[m_nCurrIdx];
        }
        
        return m_cDbgCmdArray[m_nCurrIdx--];
    }
    
    string DownCmd()
    {
        if (m_cDbgCmdArray.size() == 0)
        {
            m_nCurrIdx = 0;
            return "no cmd";
        }

        m_nCurrIdx++;
        
        if (m_nCurrIdx >= m_cDbgCmdArray.size())
        {
            m_nCurrIdx = m_cDbgCmdArray.size() - 1;
            return m_cDbgCmdArray[m_nCurrIdx];
        }
        
        return m_cDbgCmdArray[m_nCurrIdx];
    }
    
    void AddCmd(const string& strCmd)
    {
        if (m_cDbgCmdArray.size() >= MAX_CMD_CACHE_NUM)
        {
            //如果缓存超过50个，删除前20个
            for (size_t i=0; i<DEL_CMD_CACHE_NUM; i++)
            {
                m_cDbgCmdArray.pop_back();
            }
        }

        m_cDbgCmdArray.push_back(strCmd);
        
        m_nCurrIdx = m_cDbgCmdArray.size() -1;
    }

    void PrintAllCmd()
    {
        for (size_t i=0; i<m_cDbgCmdArray.size(); i++)
        {
            OspPrintf(TRUE, FALSE, "%s\n", m_cDbgCmdArray[i].c_str());
        }
    }

private:  
    vector<string> m_cDbgCmdArray;
    size_t m_nCurrIdx; 
};

static CDbgCmdCache g_tDbgCmd;

#define ADD_DBG_CMD() g_tDbgCmd.AddCmd(__FUNCTION__)

//上条调试命令
API void u()
{
    OspPrintf(TRUE, FALSE, "%s\n", g_tDbgCmd.UpCmd().c_str());
}

//下条调试命令
API void d()
{
    OspPrintf(TRUE, FALSE, "%s\n", g_tDbgCmd.DownCmd().c_str());
}

//打印所有缓存调试命令
API void a()
{
    g_tDbgCmd.PrintAllCmd();
}

API void OspExtHelp()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE, "%-60s: ospext帮助命令\n","  ospexthelp()");
    OspPrintf(TRUE, FALSE, "%-60s: ospext版本信息\n","  ospextver()");
    OspPrintf(TRUE, FALSE, "%-60s: 设置ospext的日志级别\n","  ospextlog(u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: 显示日志控制参数\n","  OspShowLogParam()");
    OspPrintf(TRUE, FALSE, "%-60s: 设置实例打印级别\n","  OspSetLogLev(u8 byLogLev, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: 设置模块打印级别\n","  OspSetModLogLev(u8 byLogMod, u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: 设置时间打印级别\n","  OspSetLogTimeLev(u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: 设置打印编译信息\n","  OspLogCompileInfo(u8 byPrint)");
    OspPrintf(TRUE, FALSE, "%-60s: 设置打印OSP前缀 \n","  OspLogOspPrefix(u8 byPrint)");
    OspPrintf(TRUE, FALSE, "%-60s: 打印实例状态信息\n","  OspInstState(u16 wAppId)");
    OspPrintf(TRUE, FALSE, "%-60s: 打印实例事务信息\n","  OspPrintTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL)");
    OspPrintf(TRUE, FALSE, "%-60s: 打印实例轮询事务信息\n","  OspPrintPollTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL)");
    OspPrintf(TRUE, FALSE, "%-60s: 打印指定事务信息\n","  OspPrintOneTask(u16 wTaskNO, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: 终止指定事务\n","  OspKillTask(u16 wTaskNO, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: 跟踪指定实例日志\n","  OspTraceInst(u16 wInstId, u16 wAppId)");
    OspPrintf(TRUE, FALSE, "%-60s: 跟踪指定事务日志\n","  OspTraceTask(u32 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-60s: 跟踪新建事务日志\n","  OspTraceNextTask()");
    OspPrintf(TRUE, FALSE, "%-60s: 设置输出运行日志文件\n","  OspWriteRunLog(u8 byRunLogLev):param表示日志级别");
    OspPrintf(TRUE, FALSE, "%-60s: 设置输出错误日志文件\n","  OspWriteErrLog(u8 byIsWriteLog):param表示打开关闭");
	OspPrintf(TRUE, FALSE, "%-60s: 设置运行日志文件数：1-255\n","  OspSetRunLogFileNum(u32 dwErrLogFileNum):param表示最大文件数");
	OspPrintf(TRUE, FALSE, "%-60s: 设置运行日志文件大小：1M-1024M\n","  OspSetRunLogFileSize(u32 dwErrLogFileSize):param表示最大文件大小");
	OspPrintf(TRUE, FALSE, "%-60s: 设置错误日志文件数：1-255\n","  OspSetErrLogFileNum(u32 dwErrLogFileNum):param表示最大文件数");
	OspPrintf(TRUE, FALSE, "%-60s: 设置错误日志文件大小：1M-1024M\n","  OspSetErrLogFileSize(u32 dwErrLogFileSize):param表示最大文件大小");
    OspPrintf(TRUE, FALSE, "%-60s: 显示日志邮箱信息\n","  OspShowLogQue()");

#ifdef  _LINUX_
    OspPrintf(TRUE, FALSE, "%-60s: 设置是否解析Mangle Name\n","  OspSetParseMangleName(u8 byIsParse)");
#endif
    OspPrintf(TRUE, FALSE, "%-60s: 设置是否开启时间检测\n","  OspSetTimeMonitor(u8 byOpen)");

    OspPrintf(TRUE, FALSE, "%-60s: 显示上条调试命令\n","  u()");
    OspPrintf(TRUE, FALSE, "%-60s: 显示下条调试命令\n","  d()");
    OspPrintf(TRUE, FALSE, "%-60s: 显示缓存调试命令\n","  a()");
}

//注册Ospext的打印命令
#ifdef _LINUX_
void RegOspExtHelpCmd()
{
    OspRegCommandEx("u", (void *)u, "", "");
    OspRegCommandEx("d", (void *)d, "", "");
    OspRegCommandEx("a", (void *)a, "", "");

    OspRegCommandEx("OspExtHelp", (void *)OspExtHelp, "", "");
    OspRegCommandEx("ospexthelp", (void *)ospexthelp, "", "");
    OspRegCommandEx("ospextver", (void *)ospextver, "", "");
    OspRegCommandEx("ospextlog", (void *)ospextlog, "", "i");
    OspRegCommandEx("OspShowLogParam", (void *)OspShowLogParam, "", "");    
    OspRegCommandEx("OspSetLogLev", (void *)OspSetLogLev, "", "iii");
    OspRegCommandEx("OspSetModLogLev", (void *)OspSetModLogLev, "", "iis");
    OspRegCommandEx("OspSetLogTimeLev", (void *)OspSetLogTimeLev, "", "i");
    OspRegCommandEx("OspLogCompileInfo", (void *)OspLogCompileInfo, "", "i");
    OspRegCommandEx("OspLogOspPrefix", (void *)OspLogOspPrefix, "", "i");
    OspRegCommandEx("OspPrintTask", (void *)OspPrintTask, "", "iis");
    OspRegCommandEx("OspPrintPollTask", (void *)OspPrintPollTask, "", "iis");
    OspRegCommandEx("OspPrintOneTask", (void *)OspPrintOneTask, "", "iii");
    OspRegCommandEx("OspKillTask", (void *)OspKillTask, "", "iii");
    OspRegCommandEx("OspTraceInst", (void *)OspTraceInst,"", "ii");
    OspRegCommandEx("OspTraceTask", (void *)OspTraceTask,"", "i");
    OspRegCommandEx("OspTraceNextTask", (void *)OspTraceNextTask,"", "");
    OspRegCommandEx("OspInstState", (void *)OspInstState,"", "i");
    OspRegCommandEx("OspWriteRunLog", (void *)OspWriteRunLog,"", "i");
    OspRegCommandEx("OspWriteErrLog", (void *)OspWriteErrLog,"", "i");
	OspRegCommandEx("OspSetRunLogFileNum", (void *)OspSetRunLogFileNum,"", "i");
	OspRegCommandEx("OspSetRunLogFileSize", (void *)OspSetRunLogFileSize,"", "i");
	OspRegCommandEx("OspSetErrLogFileNum", (void *)OspSetErrLogFileNum,"", "i");
	OspRegCommandEx("OspSetErrLogFileSize", (void *)OspSetErrLogFileSize,"", "i");
    OspRegCommandEx("OspShowLogQue", (void *)OspShowLogQue,"", "");
    OspRegCommandEx("OspSetParseMangleName", (void *)OspSetParseMangleName,"", "i");
    OspRegCommandEx("OspSetTimeMonitor", (void *)OspSetTimeMonitor,"", "i");   
}
#endif //#ifdef _LINUX_

//启用nipbug调试
#ifdef _LINUX_
void InitNipDebug()
{
    s8 szCoreLogDir[256] = "/var/log/kdm/corelog";
    //创建目录
#ifdef WIN32
    CreateDirectory(szCoreLogDir, NULL);
#elif _LINUX_
    mkdir(szCoreLogDir, S_IRWXU|S_IRWXG|S_IRWXO);
#endif
    //设置corelog名称
    string strCoreLogName = "/var/log/kdm/corelog/";

    //文件名称，由程序启动时间生成
    TOspTimeInfo tTimeInfo;
    memset(&tTimeInfo, 0, sizeof(tTimeInfo));
    OspGetTimeInfo(&tTimeInfo);
    s8 szTimeBuf[512] = {0};
    string strLogStamp = CXLogFile::GetFileStamp();
    sprintf(szTimeBuf, "%s.corelog", strLogStamp.c_str());

    strCoreLogName += szTimeBuf;

    //初始化nipdebug
    __init_sysdbg(0, strCoreLogName.c_str(), 0,NF_NO_DEBUGTHREAD);

    //如果没有程序没有崩溃过，就不产生无意义corelog的文件，便于问题定位
    //创建文件
    /*
    FILE *hCoreLogFile = fopen(strCoreLogName.c_str(), "w+b");//破坏原有文件内容，重新标记文件EOF
        if (NULL != hCoreLogFile)
        {
            fseek(hCoreLogFile, 0, SEEK_SET);
    
            string strModuleName;
            string strModuleStartInfo = GetProcessFullPath();
            strModuleStartInfo += " - start : ";
            strModuleStartInfo += COspTime::GetCurrStrTime();
            strModuleStartInfo += "\n";
    
            s32 nLen = fwrite(strModuleStartInfo.c_str(), strModuleStartInfo.size(), 1, hCoreLogFile);
            if (0 == nLen)
            {
                OspPrintf(TRUE, FALSE, "corelog[%s]写初始数据失败\n", strCoreLogName.c_str());;
            }
    
            fclose(hCoreLogFile);
    
            //初始化nipdebug
            init_nipdebug(0, NULL,  strCoreLogName.c_str(), 0);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "创建corelog[%s]失败\n", strCoreLogName.c_str());
        }*/
    
}
#endif //#ifdef _LINUX_

API void ospexthelp()
{
    OspExtHelp();
}

//ospex 版本信息
API void OspExtVer()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE, OSP_EXT_VER);
}

API void ospextver()
{
    OspExtVer();
}

//显示日志控制参数
API void OspShowLogParam()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE,"全局日志级别: %s\n", GetStrLogLev(g_byGlbLogLev).c_str());
    OspPrintf(TRUE, FALSE,"日志时间级别: %u  (0:不输出时间, 非0:秒级时间, 255: 毫秒级时间)\n", g_byPrintTimeLev);
    OspPrintf(TRUE, FALSE,"输出编译信息: %u  (0:不输出, 非0:输出)\n", g_byPrintCompileInfo);
    OspPrintf(TRUE, FALSE,"输出OSP前缀: %u  (0:不输出, 非0:输出)\n", g_byPrintOspPrefix);
    OspPrintf(TRUE, FALSE,"当前跟踪实例: [%u-%u] ([0-0]:不跟踪, 非[0-0]:跟踪的app-inst号)\n", 
        GETAPP(g_dwTraceAppInst), GETINS(g_dwTraceAppInst));
    OspPrintf(TRUE, FALSE,"当前跟踪事务: %u  (0:不跟踪, 非0:跟踪事务号)\n", g_dwTraceTaskNO);
    OspPrintf(TRUE, FALSE,"是否写运行日志文件: %u  (0:不写, 非0:写)\n", g_bIsOspExtWriteRunLog);
    OspPrintf(TRUE, FALSE,"是否写错误日志文件: %u  (0:不写, 非0:写)\n", g_bIsOspExtWriteErrLog);
	OspPrintf(TRUE, FALSE,"错误日志文件数:[%u] 大小:[%u]\n", g_cErrLogFile.GetLogNumMax(), g_cErrLogFile.GetLogSizeMax());
	OspPrintf(TRUE, FALSE,"运行日志文件数:[%u] 大小:[%u]\n", g_cRunLogFile.GetLogNumMax(), g_cRunLogFile.GetLogSizeMax());
#ifdef _LINUX_
    OspPrintf(TRUE, FALSE,"是否解析Mangle Name: %u  (0:不解析, 非0:解析)\n", g_bIsParseMangleName);
#endif
    OspPrintf(TRUE, FALSE, "是否开启时间检测: %u  (0:不检测, 非0:检测)\n", g_bIsMonitorTimePerform);

    OspPrintf(TRUE, FALSE, "\n日志级别列表: \n");
    for(u8 byLogLev = NOLOG_LEV; byLogLev < LOG_LEV_NUM; byLogLev++)
    {
        OspPrintf(TRUE, FALSE,"  %s------%u\n", GetStrLogLev(byLogLev).c_str(), byLogLev);
    }

    if (g_cModuleLogLev.GetSize() > 0)
    {
        OspPrintf(TRUE, FALSE,"\n用户设定的模块日志级别: \n");
        Iterator pPos;
        while(!pPos.End())
        {
            u32 dwLogMod = 0;
            TLogModInfo tLogModInfo;
            if (g_cModuleLogLev.Iterate(pPos, dwLogMod, tLogModInfo))
            {
                OspPrintf(TRUE, FALSE,"[%s(%u)] : [%s-%u]\n", 
                    tLogModInfo.strModName.c_str(), dwLogMod, 
                    GetStrLogLev((u8)tLogModInfo.dwLogLev).c_str(), tLogModInfo.dwLogLev);
            }
        }       
    }
}

API void OspSetLogLev(u8 byLogLev, u16 wAppId, u16 wInstId)
{
    ADD_DBG_CMD();

    g_byGlbLogLev = byLogLev;

    if (wInstId == 0)
    {        
        ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_SET_APP_LOG_LEV, &byLogLev, sizeof(byLogLev));
    }
    else
    {
        ::OspPost(MAKEIID(wAppId, wInstId), OSP_EXT_EVENT_SET_LOG_LEV, &byLogLev, sizeof(byLogLev));        
    }    
}

API void OspSetModLogLev(u8 byLogMod, u8 byLogLev, LPCSTR szModName)
{
    ADD_DBG_CMD();

    g_cModuleLogLev[byLogMod].dwLogLev = byLogLev;
    if (szModName != NULL)
    {
        g_cModuleLogLev[byLogMod].strModName = szModName;
    }
}

API void ospextlog(u8 byLogLev)
{
    OspSetModLogLev(OSPEXT, byLogLev);
}

API void OspSetLogTimeLev(u8 byLogLev)
{
    ADD_DBG_CMD();

    g_byPrintTimeLev = byLogLev;
}

API void OspLogCompileInfo(u8 byPrint)
{
    ADD_DBG_CMD();

    g_byPrintCompileInfo = byPrint;
}

API void OspLogOspPrefix(u8 byPrint)
{
    ADD_DBG_CMD();

    g_byPrintOspPrefix = byPrint;
}

API void OspPrintTask(u16 wAppId, u16 wInstId, LPCSTR szTaskType)
{
    ADD_DBG_CMD();

	u16 nLen = 0;
	if(szTaskType)
	{
		nLen = (u16)strlen(szTaskType) + 1;
	}

    if (wInstId == 0)
    {        
        ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_PRINT_APP_TASK, szTaskType, nLen);
    }
    else
    {
        ::OspPost(MAKEIID(wAppId, wInstId), OSP_EXT_EVENT_PRINT_INST_TASK, szTaskType, nLen);
    }
}

API void OspPrintPollTask(u16 wAppId, u16 wInstId, LPCSTR szTaskType)
{
    ADD_DBG_CMD();

    u16 nLen = 0;
    if(szTaskType)
    {
        nLen = (u16)strlen(szTaskType) + 1;
    }

    if (wInstId == 0)
    {        
        ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_PRINT_APP_POLL_TASK, szTaskType, nLen);
    }
    else
    {
        ::OspPost(MAKEIID(wAppId, wInstId), OSP_EXT_EVENT_PRINT_INST_POLL_TASK, szTaskType, nLen);
    }
}

API void OspKillTask(u32 dwTaskNO, u16 wAppId, u16 wInstId)
{
    ADD_DBG_CMD();

    if (wInstId == 0)
    {        
        ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_KILL_TASK, &dwTaskNO, sizeof(dwTaskNO));
    }
    else
    {
        ::OspPost(MAKEIID(wAppId, wInstId), OSP_EXT_EVENT_KILL_TASK, &dwTaskNO, sizeof(dwTaskNO));        
    }
}

API void OspPrintOneTask(u32 dwTaskNO, u16 wAppId, u16 wInstId)
{
    ADD_DBG_CMD();

    if (wInstId == 0)
    {        
        ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_PRINT_ONE_TASK, &dwTaskNO, sizeof(dwTaskNO));
    }
    else
    {
        ::OspPost(MAKEIID(wAppId, wInstId), OSP_EXT_EVENT_PRINT_ONE_TASK, &dwTaskNO, sizeof(dwTaskNO));        
    }
}

API void OspTraceInst(u16 wInstId, u16 wAppId)
{
    ADD_DBG_CMD();

    //停止trace task
    g_dwTraceTaskNO = 0;

    //只要inst和app有一个为0，即认为取消跟踪
    if (wInstId == 0 || wAppId == 0)
    {
        wInstId = 0;
        wAppId = 0;
    }

    g_dwTraceAppInst = MAKEIID(wAppId, wInstId);
}

API void OspTraceTask(u32 dwTaskNO)
{
    ADD_DBG_CMD();

    //停止trace inst
    g_dwTraceAppInst = 0;

    g_dwTraceTaskNO = dwTaskNO;
}

API void OspTraceNextTask()
{
    ADD_DBG_CMD();

    u32 dwNextTaskNO = CTaskManager::PeekTaskNO();
    if (dwNextTaskNO == INVALID_TASKNO)
    {
        OspPrintf(TRUE, FALSE, "Next task is invalid, trace fail!\n");
        return;
    }

    //停止trace inst
    g_dwTraceAppInst = 0;    

    g_dwTraceTaskNO = dwNextTaskNO;
}

//打印指定AppId的所有Inst状态
API void OspInstState(u16 wAppId)
{
    ADD_DBG_CMD();
        
    ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_PRINT_INST_STATE);
}

//设置输出运行日志文件：//0:不输出; 非0:输出
API void OspWriteRunLog(u8 byIsWriteLog)
{
    ADD_DBG_CMD();
    if (byIsWriteLog == 0)
    {
        g_bIsOspExtWriteRunLog = FALSE;
        g_byLogFileLev = 0;
    }
    else
    {
        g_bIsOspExtWriteRunLog = TRUE;
        g_byLogFileLev = byIsWriteLog;
    }
}

//设置输出错误日志文件：//0:不输出; 非0:输出
API void OspWriteErrLog(u8 byIsWriteLog)
{
    ADD_DBG_CMD();
    if (byIsWriteLog == 0)
    {
        g_bIsOspExtWriteErrLog = FALSE;
    }
    else
    {
        g_bIsOspExtWriteErrLog = TRUE;
    }
}

//显示日志邮箱信息：
API void OspShowLogQue()
{
    ADD_DBG_CMD();
    LogQuePrint();
}

#ifdef _LINUX_
//linux是否进行mangle name的解析
API void OspSetParseMangleName(u8 byIsParse)
{
    ADD_DBG_CMD();
    if (byIsParse == 0)
    {
        g_bIsParseMangleName = FALSE;
    }
    else
    {
        g_bIsParseMangleName = TRUE;
    }
}
#endif

//是否开启时间性能检测功能
API void OspSetTimeMonitor(u8 byOpen)
{
    ADD_DBG_CMD();
    if (byOpen == 0)
    {
        g_bIsMonitorTimePerform = FALSE;
    }
    else
    {
        g_bIsMonitorTimePerform = TRUE;
    }
}


API void OspSetErrLogFileNum( u32 dwErrLogFileNum )
{
	if (dwErrLogFileNum<MIN_LOG_FILE_NUM||dwErrLogFileNum>MAX_LOG_FILE_NUM)
	{
		return;
	} 
	else
	{
		g_cErrLogFile.SetLogNumMax(dwErrLogFileNum);
	}
}

API void OspSetErrLogFileSize( u32 dwErrLogFileSize )
{
	if (dwErrLogFileSize<MIN_LOG_FILE_SIZE||dwErrLogFileSize>MAX_LOG_FILE_SIZE)
	{
		return;
	} 
	else
	{
		g_cErrLogFile.SetLogSizeMax(dwErrLogFileSize*DEFAULT_LOG_FILE_SIZE_kb);
	}
}

API void OspSetRunLogFileNum( u32 dwRunLogFileNum )
{
	if (dwRunLogFileNum<MIN_LOG_FILE_NUM||dwRunLogFileNum>MAX_LOG_FILE_NUM)
	{
		return;
	} 
	else
	{
		g_cRunLogFile.SetLogNumMax(dwRunLogFileNum);
	}
}

API void OspSetRunLogFileSize( u32 dwRunLogFileSize )
{
	if (dwRunLogFileSize<MIN_LOG_FILE_SIZE||dwRunLogFileSize>MAX_LOG_FILE_SIZE)
	{
		return;
	} 
	else
	{
		g_cRunLogFile.SetLogSizeMax(dwRunLogFileSize*DEFAULT_LOG_FILE_SIZE_kb);
	}
}
