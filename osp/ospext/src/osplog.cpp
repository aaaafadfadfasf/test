
#include "cms/ospext/osplog.h"
#include "osplogfile.h"

//ȫ�ִ�ӡ�ӿ�
void GlobeLog(u8 byLogLevel, const char* szFormat, ...);
//����ģ����ƵĴ�ӡ
void GlobeModLog(u8 byLogMod, u8 byLogLevel, const char* szFormat, ...);

//��־����ӿ�
void OspExtPrintf(TOspExtLogContent& tLogContent);

#ifdef DEBUG

u8 g_byGlbLogLev = EVENT_LEV;     //ȫ�ִ�ӡ����

u8 g_byPrintTimeLev = 1;       //0:����ӡʱ��; 1:��ӡ�뼶ʱ��; 255:��ӡ���뼶ʱ��

u8 g_byPrintCompileInfo = 1;   //0:����ӡ������Ϣ(file-line-class-function); 1:��ӡ������Ϣ

u8 g_byPrintOspPrefix = 1;     //0:����ӡOSPǰ׺(app-inst-task-state); 1:��ӡOSPǰ׺

BOOL32 g_bIsMonitorTimePerform = FALSE;  //FALSE:���������ܼ�⣬TRUE������

#else

u8 g_byGlbLogLev = ERROR_LEV;     //ȫ�ִ�ӡ����

u8 g_byPrintTimeLev = 1;       //0:����ӡʱ��; 1:��ӡ�뼶ʱ��; 255:��ӡ���뼶ʱ��

u8 g_byPrintCompileInfo = 0;   //0:����ӡ������Ϣ(file-line-class-function); 1:��ӡ������Ϣ

u8 g_byPrintOspPrefix = 0;     //0:����ӡOSPǰ׺(app-inst-task-state); 1:��ӡOSPǰ׺

BOOL32 g_bIsMonitorTimePerform = FALSE;  //FALSE:���������ܼ�⣬TRUE������

#endif

u32 g_dwTraceAppInst = 0;      //0:�ر�trace; ��0:trace ָ��APP��ָ��INST

u32 g_dwTraceTaskNO = 0;        //0:�ر�trace; ��0:trace ָ������

BOOL32 g_bIsOspExtWriteRunLog = TRUE; //�Ƿ����������־�ļ���Ŀǰд���ļ������ݺ���Ļ�Ĵ�ӡ����һ��

BOOL32 g_bIsOspExtWriteErrLog = TRUE; //�Ƿ����������־�ļ���Ŀǰд���ļ������ݺ���Ļ�Ĵ�ӡ����һ��

#ifdef _LINUX_
BOOL32 g_bIsParseMangleName = FALSE; //linux�Ƿ����mangle name�Ľ�������Ϊ�ǳ���ʱ��Ĭ�ϲ����н���
#endif

u8 g_byLogFileLev = PROGRESS_LEV;  //������־�ļ�����

//ģ���ӡ����
CXMap<u32, TLogModInfo> g_cModuleLogLev;

API bool IsPrintCI()
{
    return (g_byPrintCompileInfo != 0);
}

CXLog::CXLog(const char* szFileName, int nFileLine, const char* szClassName, const char* szFunName)
        : m_szFileName(szFileName), m_nFileLine(nFileLine), 
        m_szClassName(szClassName), m_szFunName(szFunName)
{

}

void CXLog::operator()(unsigned char byLogLev, const char* szFormat, ...) const
{
//     if (g_byGlbLogLev < byLogLev)
//     {
//         return;
//     }

    char szMsg[MAX_LOG_BUF_LEN] = {0};

    GETVLISTSTR(szFormat, szMsg);
        
    string strLogFull = AppendCompileInfo(szMsg);
    
    GlobeLog(byLogLev, "%s", strLogFull.c_str());
}

void CXLog::operator()(unsigned char byLogMod, unsigned char byLogLev, const char* szFormat, ...) const
{
//     if (g_cModuleLogLev[byLogMod].dwLogLev < byLogLev)
//     {
//         return;
//     }

    char szMsg[MAX_LOG_BUF_LEN] = {0};

    GETVLISTSTR(szFormat, szMsg);

    string strLogFull = AppendCompileInfo(szMsg);

    GlobeModLog(byLogMod, byLogLev, "%s", strLogFull.c_str());
}

string CXLog::GetCompileInfo() const
{
    string strLogHead;
    
    strLogHead += GetFileNameFromPath(m_szFileName);

    strLogHead += "(";
    s8 szFileLine[64] = {0};
    sprintf(szFileLine, "%d", m_nFileLine);
    strLogHead += szFileLine;
    strLogHead += ")";

    strLogHead += " ";
    if (m_szClassName != NULL)
    {
        strLogHead += GetTypeName(m_szClassName);
        strLogHead += "::";
    }

    if (m_szFunName != NULL)
    {
        strLogHead += m_szFunName;
        strLogHead += "()";
    }
        
    strLogHead += "\n";
    
    return strLogHead;
}

string CXLog::GetFileNameFromPath(const char* szPath)
{
    string strFileName;
    if (szPath == NULL)
    {
        return strFileName;
    }
    
    const char* pNameStartPos = szPath + strlen(szPath);
    
    while (pNameStartPos > szPath)
    {
        if (*pNameStartPos == '/' || *pNameStartPos == '\\')
        {
            pNameStartPos++;
            break;
        }
        
        pNameStartPos--;
    }
    
    strFileName = pNameStartPos;
    
    return strFileName;
}

string CXLog::GetTypeName(const string& szTypeMangleName)
{
    string strTypeName = szTypeMangleName;

    if (strTypeName.length() == 0)
    {
        return strTypeName;
    }

#ifdef WIN32
    //win32��ֱ�Ӿ�����ʵ��������
    return strTypeName;    
#endif

#ifdef _LINUX_

    //Ŀǰ����mangle name�ķ�ʽ�ǳ���ʱ��Ĭ�ϲ����н���
    if (g_bIsParseMangleName == TRUE)
    {
        //ʹ��shell����c++filt�õ���ʵ��������
        char szTypeName[256] = {0};
        char achCmd[256] = {0};

        sprintf(achCmd, "c++filt %s", szTypeMangleName.c_str()); 

        //ʹ�ùܵ�ִ������õ�������
        FILE *fp = popen(achCmd, "r");
        if (fp == NULL) 
        {
            return strTypeName;
        }

        char* lpszRedStr = fgets(szTypeName, sizeof(szTypeName), fp);
        if(NULL != lpszRedStr)
        {
            size_t nLen=strlen(szTypeName);
            if(szTypeName[nLen-1] == '\n' || szTypeName[nLen-1] == '\r')
            {
                szTypeName[nLen-1] = '\0';
            }

            strTypeName = szTypeName;
        }

        pclose(fp);
    }

    return strTypeName;
    
#endif
}


string CXLog::AppendCompileInfo(const char* szLog) const
{
    string strLogFull;

    if (szLog == NULL || strlen(szLog) == 0)
    {
        return strLogFull;
    }

    strLogFull = szLog;
    
    //�Ƿ����������Ϣ
    if (IsPrintCI())
    {
        string::size_type pos = string::npos;
        string strMark = "\n";
        while((pos = strLogFull.rfind(strMark, pos)) != string::npos)
        {         
            strLogFull.erase(pos);                
            break;        
        }
            
        strLogFull += " --- ";
        strLogFull += GetCompileInfo();
        strLogFull += "\n";
    }

    return strLogFull;
}

#define ADD_LOG_LEV_DESC(x) AddLogLevDesc(x, #x)
class CLogLevDesc
{
public:
    CLogLevDesc()
    {
        InitLogLevDesc();
    }

public:
    void AddLogLevDesc(size_t nval, LPCSTR szval)
    {
        m_tLogLevDesc.AddEventDesc(szval, (u16)nval);
    }

    void InitLogLevDesc()
    {
        ADD_LOG_LEV_DESC(NOLOG_LEV);
        ADD_LOG_LEV_DESC(ERROR_LEV);
        ADD_LOG_LEV_DESC(CRITICAL_LEV);
        ADD_LOG_LEV_DESC(WARNING_LEV);
        ADD_LOG_LEV_DESC(EVENT_LEV);
        ADD_LOG_LEV_DESC(PROGRESS_LEV);
        ADD_LOG_LEV_DESC(TIMER_LEV);      
        ADD_LOG_LEV_DESC(ALL_LEV);
        ADD_LOG_LEV_DESC(TRIVIAL_LEV);
    }

    const string GetLogLevDesc(u8 byLogLev)
    {
        if (byLogLev<ALL_LEV)
        {
            return m_tLogLevDesc.GetEventDesc(byLogLev);
        }
        
        return m_tLogLevDesc.GetEventDesc(ALL_LEV);
    }

private:
    CEventDescMap m_tLogLevDesc;
};

static CLogLevDesc g_tLogLevDesc;

const string GetStrLogLev(u8 byLogLev)
{
    return g_tLogLevDesc.GetLogLevDesc(byLogLev);
}

//OspExtLogData ���ڴ�ӡ���ݣ���������־������Ϣ
void OspExtLogData(u8 byLogLevel, const char* szFormat, ...)
{ 
    UNREFERENCED_PARAMETER(byLogLevel);
    UNREFERENCED_PARAMETER(szFormat);
    
}

//��־����ӿ�
void OspExtPrintf(TOspExtLogContent& tLogContent)
{
    string strFullMsg;

    string strTime;
    if (g_byPrintTimeLev == 255)
    {
        strTime = COspTime::GetCurrStrTime_ms();
    }
    else
    {
        strTime = COspTime::GetCurrStrTime();    
    }

    tLogContent.strTimeField += "[";
    tLogContent.strTimeField += strTime;
    tLogContent.strTimeField += "]: ";

    if (g_byPrintTimeLev != 0)
    {
        strFullMsg += tLogContent.strTimeField;
    }

    strFullMsg += tLogContent.strModLev;

    if (g_byPrintOspPrefix != 0)
    {
        strFullMsg += tLogContent.strOspPrifixField;
    }

    strFullMsg += tLogContent.strBodyField;

    if (g_byPrintCompileInfo != 0)
    {
        strFullMsg += tLogContent.strCompileField;
    }

    OspPrintf(tLogContent.bIsPrintScreen, FALSE, "%s", const_cast<s8*>(strFullMsg.c_str()));

    //��ʼ��־�ļ�
    if (tLogContent.byLogLev == TRIVIAL_LEV)
    {
        //����TRIVIAL_LEV(255)��ospext�ڲ�����Ƶ���Ĵ�ӡ����(����������ѯ)����д��־�ļ�
        return;
    }

    u32 dwLogFileTypeSet = LOG_FILE_NONE;

    //�Ƿ�д������־
    if (g_bIsOspExtWriteRunLog == TRUE && g_byLogFileLev >= tLogContent.byLogLev)
    {
        dwLogFileTypeSet += LOG_FILE_RUN;
    }

    //�Ƿ�д������־
    if (g_bIsOspExtWriteErrLog == TRUE)
    {
        if (tLogContent.byLogLev == ERROR_LEV || tLogContent.byLogLev == WARNING_LEV)
        {
            dwLogFileTypeSet += LOG_FILE_ERR;
        }
    }

    if (dwLogFileTypeSet != LOG_FILE_NONE)
    {
        TLogData* ptLogData = new TLogData;
        ptLogData->dwLogType = dwLogFileTypeSet;

        //��־�ļ�д��־��������Ϣ
        ptLogData->strLog += tLogContent.strTimeField
            += tLogContent.strModLev
            += tLogContent.strOspPrifixField
            += tLogContent.strBodyField
            += tLogContent.strCompileField;

        WriteLogFile(ptLogData);
    }  
}

static u32 get_tid()
{
#if defined(WIN32)
    return (u32)GetCurrentThreadId();
#elif defined(_LINUX_)
    return (u32)syscall(224);
#endif
}

static string get_tid_str()
{
    char szTid[256] = {0};
    sprintf(szTid, "ThreadId[%u]: ", get_tid());
    return szTid;
}

void GlobeLog(u8 byLogLevel, const char* szFormat, ...)
{
    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;
    if (g_dwTraceAppInst != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_byGlbLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    tLogContent.strModLev = ("[" + GetStrLogLev(byLogLevel) +"]");
    tLogContent.strOspPrifixField = get_tid_str();

    OspExtPrintf(tLogContent);
}

void GlobeModLog(unsigned char byLogMod, u8 byLogLevel, const char* szFormat, ...)
{
    char szMsg[MAX_LOG_BUF_LEN] = {0};
    GETVLISTSTR(szFormat, szMsg); 

    TOspExtLogContent tLogContent;
    tLogContent.strBodyField = szMsg;
    tLogContent.byLogLev = byLogLevel;
    if (g_dwTraceAppInst != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_dwTraceTaskNO != 0)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    if (g_cModuleLogLev[byLogMod].dwLogLev < byLogLevel)
    {
        tLogContent.bIsPrintScreen =  FALSE;
    }

    tLogContent.strModLev = ("[" + g_cModuleLogLev[byLogMod].strModName + "]");
    tLogContent.strModLev += ("[" + GetStrLogLev(byLogLevel) + "]");
    tLogContent.strOspPrifixField = get_tid_str();

    OspExtPrintf(tLogContent);
}