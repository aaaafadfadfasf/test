#include <sys/stat.h>
#ifdef _LINUX_
#include <sys/mman.h>
#endif
#include <fcntl.h>

#include "cms/ospext/ospext.h"
#include "ospexevt.h"
#include "kdvsys.h"
#include "osplogfile.h"

CXLogFile g_cErrLogFile;
CXLogFile g_cRunLogFile;

extern bool g_bIsWriteLogFileThreadRun ;
extern BOOL32 g_bIsOspExtWriteRunLog;

//注册Ospext的打印命令
#ifdef _LINUX_
extern void RegOspExtHelpCmd();
extern void InitNipDebug();
#endif //#ifdef _LINUX_

//初始化ospext的内部消息描述
static void InitOspExtEvtDesc()
{
    OSP_ADD_EVENT_DESC(OSP_EXT_INNER_EVENT_BEGIN);
    
    //设置指定APP-INST的打印级别
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_SET_LOG_LEV);

    //设置指定APP的打印级别
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_SET_APP_LOG_LEV);
    
    //INST中轮询事务的定时器
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_POLL_TASK);
    
    //打印指定APP的事务信息
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_APP_TASK);
    
    //打印指定APP-INST的事务信息
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_TASK);

    //打印指定APP的轮询事务信息
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_APP_POLL_TASK);

    //打印指定APP-INST的轮询事务信息
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_POLL_TASK);
    
    //打印指定APP-INST-TaskNO的事务信息
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_ONE_TASK);

    //终止指定APP-INST-TaskNO的事务
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_KILL_TASK);
    
    //TRACE指定INST
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_TRACE_INST);

    //打印指定AppId的所有Inst状态
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_STATE);
    
    OSP_ADD_EVENT_DESC(OSP_EXT_INNER_EVENT_END);
}

extern u8 g_byLogFileLev;
bool TOspExtConf::ReadKeyCfg( const s8* szConfPath )
{
	if (szConfPath == NULL)
	{
		printf("ospext配置文件路径为空\n");
		return false;
	}

	s32 nVal = 0;

	GetRegKeyInt(szConfPath, SECTION_OSPEXT, KEY_RUNLOG_FILENUM, RUN_LOG_FILE_NUM, &nVal);
	if (nVal<MIN_LOG_FILE_NUM||nVal>MAX_LOG_FILE_NUM)
	{
		dwRunLogFileNum = RUN_LOG_FILE_NUM;
	} 
	else
	{
		dwRunLogFileNum = nVal;
	}
	

	GetRegKeyInt(szConfPath, SECTION_OSPEXT, KEY_RUNLOG_FILESIZE, MIN_LOG_FILE_SIZE, &nVal);
	if (nVal<MIN_LOG_FILE_SIZE||nVal>MAX_LOG_FILE_SIZE)
	{
		dwRunLogFileSize_kb = RUN_LOG_FILE_SIZE_kb;
	} 
	else
	{
		dwRunLogFileSize_kb = nVal*DEFAULT_LOG_FILE_SIZE_kb;
	}

	GetRegKeyInt(szConfPath, SECTION_OSPEXT, KEY_ERRLOG_FILENUM, ERR_LOG_FILE_NUM, &nVal);
	if (nVal<MIN_LOG_FILE_NUM||nVal>MAX_LOG_FILE_NUM)
	{
		dwErrLogFileNum = ERR_LOG_FILE_NUM;
	} 
	else
	{
		dwErrLogFileNum = nVal;
	}
	

	GetRegKeyInt(szConfPath, SECTION_OSPEXT, KEY_ERRLOG_FILESIZE, MIN_LOG_FILE_SIZE, &nVal);
	if (nVal<MIN_LOG_FILE_SIZE||nVal>MAX_LOG_FILE_SIZE)
	{
		dwErrLogFileSize_kb = ERR_LOG_FILE_SIZE_kb;
	} 
	else
	{
		dwErrLogFileSize_kb = nVal*DEFAULT_LOG_FILE_SIZE_kb;
	}

	GetRegKeyInt(szConfPath, SECTION_OSPEXT, KEY_WRITE_RUNLOG_LEV, 0, &nVal);
	byRunLogFileLev=nVal;


	return true;
}


static BOOL32 g_bOspExtInited = FALSE;

BOOL32 OspExtInit(const TOspExtConf& tOspExtConf)
{
    if (g_bOspExtInited == TRUE)
    {
        printf("ospext has already init!!!\n");
        return TRUE;
    }
    
    string strModuleName;
    if (tOspExtConf.strModuleName.empty())
    {
        strModuleName = GetProcessName();
    }
    else
    {
        strModuleName = tOspExtConf.strModuleName;
    }

    //初始化OSP
    if ( !IsOspInitd() )
    {       
        BOOL32 bInitOsp = FALSE;
        
#if defined( _LINUX_ ) || defined( DEBUG )
        bInitOsp = OspInit(TRUE, 2500, const_cast<LPSTR>(strModuleName.c_str()), tOspExtConf.dwMaxNodeNum, tOspExtConf.dwMaxDispatchMsg);
#else
        bInitOsp = OspInit(FALSE, 2500, const_cast<LPSTR>(strModuleName.c_str()), tOspExtConf.dwMaxNodeNum, tOspExtConf.dwMaxDispatchMsg);
#endif

        if (bInitOsp == FALSE)
        {
            printf("OspInit fail!!!\n");
            return FALSE;
        }           
    }
    else
    {
        printf("osp has already init!!!\n");
    }

    //设置telnet提示符
    OspSetPrompt(strModuleName.c_str());

    //注册模块名称
    OspRegistModule(strModuleName.c_str());

    //初始化ospext消息描述
    static BOOL32 s_bIsInitOspExtEvtDesc = FALSE;
    if (s_bIsInitOspExtEvtDesc == FALSE)
    {
        InitOspExtEvtDesc();
        s_bIsInitOspExtEvtDesc = TRUE;
    }

    //设置ospext库的默认打印级别
#ifdef DEBUG
    OSP_SET_MOD_LOG(OSPEXT, EVENT_LEV);
#else
    OSP_SET_MOD_LOG(OSPEXT, CRITICAL_LEV);

    //fanxg-2014-08-11: 为了避免频繁的运行日志对CIF卡造成不良影响，默认关闭运行日志
    OspWriteRunLog(tOspExtConf.byRunLogFileLev);
#endif

    //初始化ospext日志文件：分为三种日志文件
    //1. 运行日志(runlog):记录程序的运行信息
    //2. 错误日志(errlog):记录程序的错误信息
    //3. 异常日志(corelog):使用nipdebug记录程序崩溃时的堆栈信息

    static BOOL32 s_bIsInitOspLogFile = FALSE;
    if (s_bIsInitOspLogFile == FALSE)
    {
        //使用ospext实现runlog日志文件
        OspExtOpenLogFile(tOspExtConf.strRunLogDir, tOspExtConf.dwRunLogFileSize_kb, 
            tOspExtConf.dwRunLogFileNum, LOG_FILE_RUN);

        //使用ospext实现errlog日志文件
        OspExtOpenLogFile(tOspExtConf.strErrLogDir, tOspExtConf.dwErrLogFileSize_kb, 
            tOspExtConf.dwErrLogFileNum, LOG_FILE_ERR);


        //创建写日志文件线程
        TASKHANDLE hWriteLogFile = OspTaskCreate(WriteLogFileThread, "OspExtLogThread", 255, 400<<10, 0, 0);
        if (!hWriteLogFile)
        {
            printf ("ospext create write log file thread fail!!!\n");
            return FALSE;
        }
        else
        {
            printf ("ospext create write log file thread success!!!\n");
        }

#ifdef _LINUX_
        //corelog: 使用nipdebug

        InitNipDebug();
        	
#endif
        
        s_bIsInitOspLogFile = TRUE;
    }

    //linux注册调试打印
#ifdef _LINUX_
    static BOOL32 s_bIsRegOspExtHelpCmd = FALSE;
    if (s_bIsRegOspExtHelpCmd == FALSE)
    {
        RegOspExtHelpCmd();
        s_bIsRegOspExtHelpCmd = TRUE;
    }    
#endif 

    g_bOspExtInited = TRUE;

    return TRUE;
}

void OspExtQuit()
{
#ifdef _LINUX_
    pid_t pid = getpid();
    s8 szPidFile[64] = {0};
    snprintf(szPidFile, sizeof(szPidFile), "ospext_exit_pid/%u", (u32)pid);
    mkdir("/dev/shm/ospext_exit_pid/", 0755);
    int fd = shm_open(szPidFile, O_CREAT | O_WRONLY | O_TRUNC, 0755);
    if (fd > 0)
    {
        close(fd);
    }
#endif

    //停止日志文件线程
    g_bIsWriteLogFileThreadRun = false;

    //该打印不要删除，它可以激活日志文件线程退出
    g_bIsOspExtWriteRunLog = TRUE;
    GLOBELOG(OSPEXT, CRITICAL_LEV, "ospext quit\n");

    //稍微等待一下，让日志线程安全退出
    OspDelay(1000);

    //最后再退出OSP
    OspQuit();
}

void OspExtSetSignalProcCB(PfProcSignal pfSigProcCB)
{
#ifdef  _LINUX_
    if (pfSigProcCB != NULL)
    {
        for (int nSig=USER_SIGRTMIN; nSig<=USER_SIGRTMAX; nSig++)
        {
            signal(nSig, pfSigProcCB);   		
        }
    }
    else
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "pfSigProcCB == NULL,信号回调设置失败\n");
    }
#else
    GLOBELOG(OSPEXT, ERROR_LEV, "winows版本暂不支持signal处理\n");
    UNREFERENCED_PARAMETER(pfSigProcCB);
#endif

}

BOOL32 COspAppImpl::CreateOspApp(CApp* pApp, char *name, u16 aid, u8 pri, u16 queuesize, u32 uStackSize ) const
{    
    if (pApp == NULL || name == NULL)
    {
        printf("pApp == NULL || name == NULL, CreateOspApp() fail\n");
        OspPrintf(TRUE, FALSE, "pApp == NULL || name == NULL, CreateOspApp() fail\n");
        return FALSE;
    }
    
    if (pApp->CreateApp(name, aid, pri, queuesize, uStackSize) == OSP_ERROR)
    {
        printf("CApp::CreateApp()调用失败, CreateOspApp() fail\n");
        OspPrintf(TRUE, FALSE, "CApp::CreateApp()调用失败, CreateOspApp() fail\n");
        return FALSE;
    }
    
    u16 wInstNum = (u16)pApp->GetInstanceNumber();
    for (u16 i=0; i<=wInstNum; i++)
    {
        CInstance* pInst = NULL;
        
        u16 wInstId = CInstance::INVALID;
        if (i==0)
        {
            wInstId = CInstance::DAEMON;            
        }
        else
        {
            wInstId = i;
        }
        
        pInst = pApp->GetInstance(wInstId);
        
        if (pInst == NULL)
        {
            printf("app[%s]-inst[%u]实例指针为空\n", name, wInstId);
            OspPrintf(TRUE, FALSE, "app[%s]-inst[%u]实例指针为空\n", name, wInstId);
            return FALSE;
        }
        
        CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
        if (pInstExt == NULL)
        {
            printf("app[%s]-inst[%u]实例不是CInstExt类型\n", name, wInstId);
            OspPrintf(TRUE, FALSE, "app[%s]-inst[%u]实例不是CInstExt类型\n", name, wInstId);
            return FALSE;
        }
        
        //设置inst的APP指针
        pInstExt->SetAppPt(pApp);
    }

    return TRUE;
}