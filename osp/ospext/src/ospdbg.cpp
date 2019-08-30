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
extern BOOL32 g_bIsParseMangleName; //linux�Ƿ����mangle name�Ľ���
#endif
extern u8 g_byLogFileLev;
extern CXMap<u32, TLogModInfo> g_cModuleLogLev;

extern CXLogFile g_cErrLogFile;
extern CXLogFile g_cRunLogFile;

//����ʹ�ù��ĵ���������ڴ�ʹ��ʱ����ʾ
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
            //������泬��50����ɾ��ǰ20��
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

//������������
API void u()
{
    OspPrintf(TRUE, FALSE, "%s\n", g_tDbgCmd.UpCmd().c_str());
}

//������������
API void d()
{
    OspPrintf(TRUE, FALSE, "%s\n", g_tDbgCmd.DownCmd().c_str());
}

//��ӡ���л����������
API void a()
{
    g_tDbgCmd.PrintAllCmd();
}

API void OspExtHelp()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE, "%-60s: ospext��������\n","  ospexthelp()");
    OspPrintf(TRUE, FALSE, "%-60s: ospext�汾��Ϣ\n","  ospextver()");
    OspPrintf(TRUE, FALSE, "%-60s: ����ospext����־����\n","  ospextlog(u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ʾ��־���Ʋ���\n","  OspShowLogParam()");
    OspPrintf(TRUE, FALSE, "%-60s: ����ʵ����ӡ����\n","  OspSetLogLev(u8 byLogLev, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: ����ģ���ӡ����\n","  OspSetModLogLev(u8 byLogMod, u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: ����ʱ���ӡ����\n","  OspSetLogTimeLev(u8 byLogLev)");
    OspPrintf(TRUE, FALSE, "%-60s: ���ô�ӡ������Ϣ\n","  OspLogCompileInfo(u8 byPrint)");
    OspPrintf(TRUE, FALSE, "%-60s: ���ô�ӡOSPǰ׺ \n","  OspLogOspPrefix(u8 byPrint)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ӡʵ��״̬��Ϣ\n","  OspInstState(u16 wAppId)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ӡʵ��������Ϣ\n","  OspPrintTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ӡʵ����ѯ������Ϣ\n","  OspPrintPollTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ӡָ��������Ϣ\n","  OspPrintOneTask(u16 wTaskNO, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: ��ָֹ������\n","  OspKillTask(u16 wTaskNO, u16 wAppId, u16 wInstId)");
    OspPrintf(TRUE, FALSE, "%-60s: ����ָ��ʵ����־\n","  OspTraceInst(u16 wInstId, u16 wAppId)");
    OspPrintf(TRUE, FALSE, "%-60s: ����ָ��������־\n","  OspTraceTask(u32 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-60s: �����½�������־\n","  OspTraceNextTask()");
    OspPrintf(TRUE, FALSE, "%-60s: �������������־�ļ�\n","  OspWriteRunLog(u8 byRunLogLev):param��ʾ��־����");
    OspPrintf(TRUE, FALSE, "%-60s: �������������־�ļ�\n","  OspWriteErrLog(u8 byIsWriteLog):param��ʾ�򿪹ر�");
	OspPrintf(TRUE, FALSE, "%-60s: ����������־�ļ�����1-255\n","  OspSetRunLogFileNum(u32 dwErrLogFileNum):param��ʾ����ļ���");
	OspPrintf(TRUE, FALSE, "%-60s: ����������־�ļ���С��1M-1024M\n","  OspSetRunLogFileSize(u32 dwErrLogFileSize):param��ʾ����ļ���С");
	OspPrintf(TRUE, FALSE, "%-60s: ���ô�����־�ļ�����1-255\n","  OspSetErrLogFileNum(u32 dwErrLogFileNum):param��ʾ����ļ���");
	OspPrintf(TRUE, FALSE, "%-60s: ���ô�����־�ļ���С��1M-1024M\n","  OspSetErrLogFileSize(u32 dwErrLogFileSize):param��ʾ����ļ���С");
    OspPrintf(TRUE, FALSE, "%-60s: ��ʾ��־������Ϣ\n","  OspShowLogQue()");

#ifdef  _LINUX_
    OspPrintf(TRUE, FALSE, "%-60s: �����Ƿ����Mangle Name\n","  OspSetParseMangleName(u8 byIsParse)");
#endif
    OspPrintf(TRUE, FALSE, "%-60s: �����Ƿ���ʱ����\n","  OspSetTimeMonitor(u8 byOpen)");

    OspPrintf(TRUE, FALSE, "%-60s: ��ʾ������������\n","  u()");
    OspPrintf(TRUE, FALSE, "%-60s: ��ʾ������������\n","  d()");
    OspPrintf(TRUE, FALSE, "%-60s: ��ʾ�����������\n","  a()");
}

//ע��Ospext�Ĵ�ӡ����
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

//����nipbug����
#ifdef _LINUX_
void InitNipDebug()
{
    s8 szCoreLogDir[256] = "/var/log/kdm/corelog";
    //����Ŀ¼
#ifdef WIN32
    CreateDirectory(szCoreLogDir, NULL);
#elif _LINUX_
    mkdir(szCoreLogDir, S_IRWXU|S_IRWXG|S_IRWXO);
#endif
    //����corelog����
    string strCoreLogName = "/var/log/kdm/corelog/";

    //�ļ����ƣ��ɳ�������ʱ������
    TOspTimeInfo tTimeInfo;
    memset(&tTimeInfo, 0, sizeof(tTimeInfo));
    OspGetTimeInfo(&tTimeInfo);
    s8 szTimeBuf[512] = {0};
    string strLogStamp = CXLogFile::GetFileStamp();
    sprintf(szTimeBuf, "%s.corelog", strLogStamp.c_str());

    strCoreLogName += szTimeBuf;

    //��ʼ��nipdebug
    __init_sysdbg(0, strCoreLogName.c_str(), 0,NF_NO_DEBUGTHREAD);

    //���û�г���û�б��������Ͳ�����������corelog���ļ����������ⶨλ
    //�����ļ�
    /*
    FILE *hCoreLogFile = fopen(strCoreLogName.c_str(), "w+b");//�ƻ�ԭ���ļ����ݣ����±���ļ�EOF
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
                OspPrintf(TRUE, FALSE, "corelog[%s]д��ʼ����ʧ��\n", strCoreLogName.c_str());;
            }
    
            fclose(hCoreLogFile);
    
            //��ʼ��nipdebug
            init_nipdebug(0, NULL,  strCoreLogName.c_str(), 0);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "����corelog[%s]ʧ��\n", strCoreLogName.c_str());
        }*/
    
}
#endif //#ifdef _LINUX_

API void ospexthelp()
{
    OspExtHelp();
}

//ospex �汾��Ϣ
API void OspExtVer()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE, OSP_EXT_VER);
}

API void ospextver()
{
    OspExtVer();
}

//��ʾ��־���Ʋ���
API void OspShowLogParam()
{
    ADD_DBG_CMD();

    OspPrintf(TRUE, FALSE,"ȫ����־����: %s\n", GetStrLogLev(g_byGlbLogLev).c_str());
    OspPrintf(TRUE, FALSE,"��־ʱ�伶��: %u  (0:�����ʱ��, ��0:�뼶ʱ��, 255: ���뼶ʱ��)\n", g_byPrintTimeLev);
    OspPrintf(TRUE, FALSE,"���������Ϣ: %u  (0:�����, ��0:���)\n", g_byPrintCompileInfo);
    OspPrintf(TRUE, FALSE,"���OSPǰ׺: %u  (0:�����, ��0:���)\n", g_byPrintOspPrefix);
    OspPrintf(TRUE, FALSE,"��ǰ����ʵ��: [%u-%u] ([0-0]:������, ��[0-0]:���ٵ�app-inst��)\n", 
        GETAPP(g_dwTraceAppInst), GETINS(g_dwTraceAppInst));
    OspPrintf(TRUE, FALSE,"��ǰ��������: %u  (0:������, ��0:���������)\n", g_dwTraceTaskNO);
    OspPrintf(TRUE, FALSE,"�Ƿ�д������־�ļ�: %u  (0:��д, ��0:д)\n", g_bIsOspExtWriteRunLog);
    OspPrintf(TRUE, FALSE,"�Ƿ�д������־�ļ�: %u  (0:��д, ��0:д)\n", g_bIsOspExtWriteErrLog);
	OspPrintf(TRUE, FALSE,"������־�ļ���:[%u] ��С:[%u]\n", g_cErrLogFile.GetLogNumMax(), g_cErrLogFile.GetLogSizeMax());
	OspPrintf(TRUE, FALSE,"������־�ļ���:[%u] ��С:[%u]\n", g_cRunLogFile.GetLogNumMax(), g_cRunLogFile.GetLogSizeMax());
#ifdef _LINUX_
    OspPrintf(TRUE, FALSE,"�Ƿ����Mangle Name: %u  (0:������, ��0:����)\n", g_bIsParseMangleName);
#endif
    OspPrintf(TRUE, FALSE, "�Ƿ���ʱ����: %u  (0:�����, ��0:���)\n", g_bIsMonitorTimePerform);

    OspPrintf(TRUE, FALSE, "\n��־�����б�: \n");
    for(u8 byLogLev = NOLOG_LEV; byLogLev < LOG_LEV_NUM; byLogLev++)
    {
        OspPrintf(TRUE, FALSE,"  %s------%u\n", GetStrLogLev(byLogLev).c_str(), byLogLev);
    }

    if (g_cModuleLogLev.GetSize() > 0)
    {
        OspPrintf(TRUE, FALSE,"\n�û��趨��ģ����־����: \n");
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

    //ֹͣtrace task
    g_dwTraceTaskNO = 0;

    //ֻҪinst��app��һ��Ϊ0������Ϊȡ������
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

    //ֹͣtrace inst
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

    //ֹͣtrace inst
    g_dwTraceAppInst = 0;    

    g_dwTraceTaskNO = dwNextTaskNO;
}

//��ӡָ��AppId������Inst״̬
API void OspInstState(u16 wAppId)
{
    ADD_DBG_CMD();
        
    ::OspPost(MAKEIID(wAppId, CInstance::DAEMON), OSP_EXT_EVENT_PRINT_INST_STATE);
}

//�������������־�ļ���//0:�����; ��0:���
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

//�������������־�ļ���//0:�����; ��0:���
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

//��ʾ��־������Ϣ��
API void OspShowLogQue()
{
    ADD_DBG_CMD();
    LogQuePrint();
}

#ifdef _LINUX_
//linux�Ƿ����mangle name�Ľ���
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

//�Ƿ���ʱ�����ܼ�⹦��
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
