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

//ע��Ospext�Ĵ�ӡ����
#ifdef _LINUX_
extern void RegOspExtHelpCmd();
extern void InitNipDebug();
#endif //#ifdef _LINUX_

//��ʼ��ospext���ڲ���Ϣ����
static void InitOspExtEvtDesc()
{
    OSP_ADD_EVENT_DESC(OSP_EXT_INNER_EVENT_BEGIN);
    
    //����ָ��APP-INST�Ĵ�ӡ����
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_SET_LOG_LEV);

    //����ָ��APP�Ĵ�ӡ����
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_SET_APP_LOG_LEV);
    
    //INST����ѯ����Ķ�ʱ��
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_POLL_TASK);
    
    //��ӡָ��APP��������Ϣ
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_APP_TASK);
    
    //��ӡָ��APP-INST��������Ϣ
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_TASK);

    //��ӡָ��APP����ѯ������Ϣ
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_APP_POLL_TASK);

    //��ӡָ��APP-INST����ѯ������Ϣ
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_POLL_TASK);
    
    //��ӡָ��APP-INST-TaskNO��������Ϣ
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_ONE_TASK);

    //��ָֹ��APP-INST-TaskNO������
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_KILL_TASK);
    
    //TRACEָ��INST
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_TRACE_INST);

    //��ӡָ��AppId������Inst״̬
    OSP_ADD_EVENT_DESC(OSP_EXT_EVENT_PRINT_INST_STATE);
    
    OSP_ADD_EVENT_DESC(OSP_EXT_INNER_EVENT_END);
}

extern u8 g_byLogFileLev;
bool TOspExtConf::ReadKeyCfg( const s8* szConfPath )
{
	if (szConfPath == NULL)
	{
		printf("ospext�����ļ�·��Ϊ��\n");
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

    //��ʼ��OSP
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

    //����telnet��ʾ��
    OspSetPrompt(strModuleName.c_str());

    //ע��ģ������
    OspRegistModule(strModuleName.c_str());

    //��ʼ��ospext��Ϣ����
    static BOOL32 s_bIsInitOspExtEvtDesc = FALSE;
    if (s_bIsInitOspExtEvtDesc == FALSE)
    {
        InitOspExtEvtDesc();
        s_bIsInitOspExtEvtDesc = TRUE;
    }

    //����ospext���Ĭ�ϴ�ӡ����
#ifdef DEBUG
    OSP_SET_MOD_LOG(OSPEXT, EVENT_LEV);
#else
    OSP_SET_MOD_LOG(OSPEXT, CRITICAL_LEV);

    //fanxg-2014-08-11: Ϊ�˱���Ƶ����������־��CIF����ɲ���Ӱ�죬Ĭ�Ϲر�������־
    OspWriteRunLog(tOspExtConf.byRunLogFileLev);
#endif

    //��ʼ��ospext��־�ļ�����Ϊ������־�ļ�
    //1. ������־(runlog):��¼�����������Ϣ
    //2. ������־(errlog):��¼����Ĵ�����Ϣ
    //3. �쳣��־(corelog):ʹ��nipdebug��¼�������ʱ�Ķ�ջ��Ϣ

    static BOOL32 s_bIsInitOspLogFile = FALSE;
    if (s_bIsInitOspLogFile == FALSE)
    {
        //ʹ��ospextʵ��runlog��־�ļ�
        OspExtOpenLogFile(tOspExtConf.strRunLogDir, tOspExtConf.dwRunLogFileSize_kb, 
            tOspExtConf.dwRunLogFileNum, LOG_FILE_RUN);

        //ʹ��ospextʵ��errlog��־�ļ�
        OspExtOpenLogFile(tOspExtConf.strErrLogDir, tOspExtConf.dwErrLogFileSize_kb, 
            tOspExtConf.dwErrLogFileNum, LOG_FILE_ERR);


        //����д��־�ļ��߳�
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
        //corelog: ʹ��nipdebug

        InitNipDebug();
        	
#endif
        
        s_bIsInitOspLogFile = TRUE;
    }

    //linuxע����Դ�ӡ
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

    //ֹͣ��־�ļ��߳�
    g_bIsWriteLogFileThreadRun = false;

    //�ô�ӡ��Ҫɾ���������Լ�����־�ļ��߳��˳�
    g_bIsOspExtWriteRunLog = TRUE;
    GLOBELOG(OSPEXT, CRITICAL_LEV, "ospext quit\n");

    //��΢�ȴ�һ�£�����־�̰߳�ȫ�˳�
    OspDelay(1000);

    //������˳�OSP
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
        GLOBELOG(OSPEXT, ERROR_LEV, "pfSigProcCB == NULL,�źŻص�����ʧ��\n");
    }
#else
    GLOBELOG(OSPEXT, ERROR_LEV, "winows�汾�ݲ�֧��signal����\n");
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
        printf("CApp::CreateApp()����ʧ��, CreateOspApp() fail\n");
        OspPrintf(TRUE, FALSE, "CApp::CreateApp()����ʧ��, CreateOspApp() fail\n");
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
            printf("app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", name, wInstId);
            OspPrintf(TRUE, FALSE, "app[%s]-inst[%u]ʵ��ָ��Ϊ��\n", name, wInstId);
            return FALSE;
        }
        
        CInstExt* pInstExt = dynamic_cast<CInstExt*>(pInst);
        if (pInstExt == NULL)
        {
            printf("app[%s]-inst[%u]ʵ������CInstExt����\n", name, wInstId);
            OspPrintf(TRUE, FALSE, "app[%s]-inst[%u]ʵ������CInstExt����\n", name, wInstId);
            return FALSE;
        }
        
        //����inst��APPָ��
        pInstExt->SetAppPt(pApp);
    }

    return TRUE;
}