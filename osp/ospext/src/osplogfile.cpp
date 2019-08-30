#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#endif

#include "cms/ospext/osplog.h"
#include "cms/ospext/ospmailbox.h"
#include "osplogfile.h"

#ifdef _LINUX_
//#include <dirent.h>

//使用scandir遍历目录，该接口可以不改变当前工作目录
#define  USE_SCANDIR

//释放scandir返回的内存
void FreeDirNameList(struct dirent** ppNamelist, int nNum)
{
    if ( nNum < 0 || NULL == ppNamelist )
    {
        return;
    }   

    while (nNum--)
    {
        free(ppNamelist[nNum]);
    }
    free(ppNamelist);
}

#endif



bool g_bIsWriteLogFileThreadRun = true;

extern CXLogFile g_cErrLogFile;
extern CXLogFile g_cRunLogFile;

static CXMailbox<TLogData*> g_cLogMsgQue("logfile");

static void WriteErrLogFile(const string& strLog)
{
    g_cErrLogFile.WriteLogFileNew(strLog);
}

static void WriteRunLogFile(const string& strLog)
{
    g_cRunLogFile.WriteLogFileNew(strLog);
}

void WriteLogFile(TLogData* ptLogData)
{
    if (ptLogData != NULL)
    {
        if (!g_cLogMsgQue.PostMsg(ptLogData))
        {
            //投递失败，直接删除
            delete ptLogData;
            ptLogData = NULL;
        }
    }
}

//显示日志邮箱信息
void LogQuePrint()
{
    g_cLogMsgQue.Print();
}


void* WriteLogFileThread(void* pArg)
{
    UNREFERENCED_PARAMETER(pArg);

	while(g_bIsWriteLogFileThreadRun)
	{
        TLogData* ptLogData = NULL;
        if (g_cLogMsgQue.GetMsg(ptLogData))
        {
            if (ptLogData != NULL)
            {
                if (ptLogData->dwLogType & LOG_FILE_ERR)
                {
                    WriteErrLogFile(ptLogData->strLog);
                }

                if (ptLogData->dwLogType & LOG_FILE_RUN)
                {
                    WriteRunLogFile(ptLogData->strLog);
                }

                delete ptLogData;
                ptLogData = NULL;
            }
        }       
	}

	return NULL;
}

//日志文件
#define CAPACITY_1K    (1024)
//#define CAPACITY_1M    (1024*CAPACITY_1K)
//#define CAPACITY_1G    (1024*CAPACITY_1M)

static bool ExistFileOrDir(LPCSTR szFileName)
{
    if (szFileName == NULL)
    {
        return false;
    }

#ifdef _LINUX_
    return (access(szFileName, 0) != -1);
#else
    return (_access(szFileName, 0) != -1);
#endif  
}

static bool MakeDir(LPCSTR szDirName)
{
    if (szDirName == NULL)
    {
        return false;
    }

#ifdef _LINUX_
    if (0 == mkdir(szDirName, S_IRWXU|S_IRWXG|S_IRWXO))
    {
        return true;
    } 
#else
    if (TRUE == CreateDirectory(szDirName, NULL))
    {
        return true;
    } 
#endif

    printf("make dir[%s] fail\n", szDirName);
    return false;
}

#ifdef _LINUX_

#else
static void replace_str(string& strBody, const string& strSrc, const string& strDst) 
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type dstLen = strDst.size();
    while((pos = strBody.find(strSrc, pos)) != string::npos)
    {         
        strBody.replace(pos, srcLen, strDst);                
        pos += dstLen;        
    }
}
#endif

#ifdef _LINUX_
#define DIR_SEP  "/"
#else
#define DIR_SEP  "\\"
#endif

static bool RecureMakeDir(const string& strDirPath)
{
    if (strDirPath.empty())
    {
        return false;
    }

    string::size_type posSep = strDirPath.find(DIR_SEP, 0);
    do
    {
        posSep = strDirPath.find(DIR_SEP, posSep+1);
        if(posSep != string::npos)
        {
            string subDir = strDirPath.substr(0, posSep);
            if (!ExistFileOrDir(subDir.c_str()))
            {
                MakeDir(subDir.c_str());
            }          
        }
    }
    while(posSep != string::npos);

    return true;
}

static bool DelFile(LPCSTR szFileName)
{
    if (szFileName == NULL)
    {
        return false;
    }

#ifdef _LINUX_
    if (0 == remove(szFileName))
    {
        return true;
    } 
#else
    if (TRUE == DeleteFile(szFileName))
    {
        return true;
    } 
#endif

    printf("remove file[%s] fail\n", szFileName);
    return false;
}

CXLogFile::CXLogFile()
{
    SetLogFileParam(DEFAULT_LOG_DIR_NAME, DEFAULT_LOG_FILE_SIZE_kb, DEFAULT_LOG_FILE_NUM, LOG_FILE_ERR);   
}

CXLogFile::~CXLogFile()
{
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

void CXLogFile::SetLogFileParam(const string& strDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType)
{
    CAutoLock cAutoLock(m_cLock);

    m_dwLogType = dwLogType;

    m_strLogDir = strDir;

#ifdef _LINUX_

#else
    //windows也可能使用"/"作为相对目录分隔符，首先替换掉
    replace_str(m_strLogDir, "/", "\\");
#endif

    //补成完整目录路径
    string strDirSep = DIR_SEP;
    if (m_strLogDir[m_strLogDir.length()-1] != strDirSep[0])
    {
        m_strLogDir += DIR_SEP;
    }

    m_strLogName = GetProcessName();
    m_strLogName += GetSuffixName();
    m_nLogSizeMax = nLogSizeMax_kb;
    m_nLogNumMax = nLogNumMax;

    m_nCurLogIdx = 0;
    m_pFile = NULL;
    m_strCurFile.clear();
}

void CXLogFile::WriteLogFile(const string& strLog)
{
    CAutoLock cAutoLock(m_cLock);

    if (strLog.empty())
    {
        return;
    }

    if (!ExistFileOrDir(m_strLogDir.c_str()))
    {
        MakeDir(m_strLogDir.c_str());
    }

    char szFileName[512] = {0};
    sprintf(szFileName, "%s%u-%s", m_strLogDir.c_str(), m_nCurLogIdx, m_strLogName.c_str());

    const char* szOpenMode = "a+";   //默认打开方式,追加

    u32 nSaveCurLogIdx = m_nCurLogIdx;

    //寻找可写文件
    while(m_nCurLogIdx < m_nLogNumMax)
    {
        if (ExistFileOrDir(szFileName)) //文件存在    
        {
            struct stat fileStat;
            stat(szFileName, &fileStat);
            long dwLogSize = fileStat.st_size;

            if (dwLogSize >= (long)(m_nLogSizeMax*CAPACITY_1K)) //超过最大log容量,更改文件
            {
                //先关闭当前文件
                if (m_pFile != NULL)
                {
                    fclose(m_pFile);
                    m_pFile = NULL;
                }

                m_nCurLogIdx++;
                if(m_nCurLogIdx >= m_nLogNumMax)  //文件数目达到最大数
                {
                    if (nSaveCurLogIdx >= m_nLogNumMax - 1)
                    {
                        //从第一个文件开始覆盖写
                        m_nCurLogIdx = 0;                    
                    }
                    else
                    {
                        //从下一个文件开始覆盖写
                        m_nCurLogIdx = nSaveCurLogIdx + 1;
                    }

                    sprintf(szFileName, "%s%u-%s", m_strLogDir.c_str(), m_nCurLogIdx, m_strLogName.c_str());
                    szOpenMode = "w+";		//打开方式,破坏
                    break;
                }
                else
                {
                    //继续寻找当前可写文件
                    sprintf(szFileName, "%s%u-%s", m_strLogDir.c_str(), m_nCurLogIdx, m_strLogName.c_str());
                    continue;
                }
            }
            else
            {
                //文件存在，并且未达到最大容量，是可写文件
                break;
            }
        }
        else //文件不存在，肯定可写，直接创建
        {
            //但文件句柄不为空，可能是用户手动删除的日志文件，是无效句柄，需要close
            if (m_pFile != NULL)
            {
                fclose(m_pFile);
                m_pFile = NULL;
            }
            break;
        }
    }
    
    if (m_pFile == NULL)
    {
        m_pFile = fopen(szFileName, szOpenMode);
        if(m_pFile == NULL)
        {
            printf("ospext-CXLogFile::WriteLogFile-fopen fail\n");
        }
    }

    if(m_pFile != NULL)
    {
        fwrite(strLog.c_str(), strLog.size(), 1, m_pFile);
        fflush(m_pFile);
    }
}

void CXLogFile::WriteLogFileNew(const string& strLog)
{
    CAutoLock cAutoLock(m_cLock);

    if (strLog.empty())
    {
        return;
    }

    if (!ExistFileOrDir(m_strLogDir.c_str()))
    {
        RecureMakeDir(m_strLogDir.c_str());
    }

    if (m_strCurFile.empty())
    {
        m_strCurFile = GetCurFile();
    }

    //寻找可写文件
    if (ExistFileOrDir(m_strCurFile.c_str())) //文件存在    
    {
        struct stat fileStat;
        stat(m_strCurFile.c_str(), &fileStat);
        long dwLogSize = fileStat.st_size;

        if (dwLogSize >= (long)(m_nLogSizeMax*CAPACITY_1K)) //超过最大log容量,更改文件
        {
            //先关闭当前文件
            if (m_pFile != NULL)
            {
                fclose(m_pFile);
                m_pFile = NULL;
            }

            //删除最老的文件
            if (GetFileNum() >= m_nLogNumMax)
            {
                string strFirstFile = GetFirstFile();
                DelFile(strFirstFile.c_str());
            }

            //创建新文件
            m_strCurFile = GetNewFile();      
        }
        else
        {
            //文件存在，并且未达到最大容量，是可写文件
        }
    }
    else //文件不存在，肯定可写，直接创建
    {
        //但文件句柄不为空，可能是用户手动删除的日志文件，是无效句柄，需要close
        if (m_pFile != NULL)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
    }

    if (m_pFile == NULL)
    {
        const char* szOpenMode = "a+";   //默认打开方式,追加
        m_pFile = fopen(m_strCurFile.c_str(), szOpenMode);
        if(m_pFile == NULL)
        {
            printf("ospext-CXLogFile::WriteLogFileNew-fopen[%s] fail\n", m_strCurFile.c_str());
        }
    }

    if(m_pFile != NULL)
    {
        if (fwrite(strLog.c_str(), strLog.size(), 1, m_pFile) != 1)
        {
#ifdef _LINUX_
			//失败后就写到messages，对messages来讲就是灾难。此处屏蔽
            //syslog(LOG_INFO, "ospext-CXLogFile::WriteLogFileNew-fwrite file[%s] fail, size[%u], buf[%s]\n",
            //    m_strCurFile.c_str(),  strLog.size(), strLog.c_str());
#endif
            return;
        }
        
        if (fflush(m_pFile) != 0)
        {
#ifdef _LINUX_
			//失败后就写到messages，对messages来讲就是灾难。此处屏蔽
            //syslog(LOG_INFO, "ospext-CXLogFile::WriteLogFileNew-fflush file[%s] fail, size[%u], buf[%s]\n",
            //    m_strCurFile.c_str(),  strLog.size(), strLog.c_str());
#endif
            return;
        }      
    }
}

string CXLogFile::GetCurFile() const
{
    string strFileName;

    if (GetFileNum() == 0)
    {
        //新建一个文件
        strFileName = GetNewFile();
    }
    else
    {
        //找一个最新的文件
        strFileName = GetLastFile();
    }
   
    return strFileName;
}

string CXLogFile::GetNewFile() const
{
    //创建新文件
    string strFileName;
    strFileName = GetFileStamp();
    strFileName += GetSuffixName();

    //添加目录
    strFileName = m_strLogDir + strFileName;

    return strFileName;
}

string CXLogFile::GetSuffixName() const
{
    if (m_dwLogType == LOG_FILE_ERR)
    {
        return string(".errlog");
    }
    else if (m_dwLogType == LOG_FILE_RUN)
    {
        return string(".runlog");
    }
    else
    {
        return string(DEFAULT_LOG_SUFFIX);
    }
}

string CXLogFile::GetFirstFile() const
{
    //获取最老的文件
    string strFileName;

    time_t tModifyFileTime = 0;
    string strModifyFile;

#ifdef _LINUX_

#ifdef USE_SCANDIR
    struct dirent** ppNamelist = NULL;
    s32 nEntryNum = 0;

    nEntryNum = scandir(m_strLogDir.c_str(), &ppNamelist, 0, alphasort);
    if (nEntryNum < 0 || ppNamelist == NULL)
    {
        printf("scandir [%s] fail!!!\n", m_strLogDir.c_str());
        return strFileName;
    }

    s32 nSaveEntryNum = nEntryNum;
    struct stat statFile;
    while(nEntryNum--) 
    {
        struct dirent* pFileEntry = ppNamelist[nEntryNum];

        if(pFileEntry->d_name[0] == '.') 
            continue; 

        string strFullPath = m_strLogDir + pFileEntry->d_name;
        stat(strFullPath.c_str(), &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        if (tModifyFileTime == 0)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;      
        } 

        if (statFile.st_mtime < tModifyFileTime)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;
        }
    }

    FreeDirNameList(ppNamelist, nSaveEntryNum);
#else
    DIR* pDir = opendir(m_strLogDir.c_str()); 
    if(pDir == NULL) 
    { 
        return strFileName;
    }

    string strSaveWorkDir = GetProcessPath();
    chdir(m_strLogDir.c_str());

    struct dirent* pFileEntry = NULL;
    struct stat statFile;

    while((pFileEntry = readdir(pDir)) != NULL) 
    { 
        if(pFileEntry->d_name[0] == '.') 
            continue; 

        stat(pFileEntry->d_name, &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        if (tModifyFileTime == 0)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;      
        } 

        if (statFile.st_mtime < tModifyFileTime)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;
        }
    } 

    chdir(strSaveWorkDir.c_str()); 
    closedir(pDir);
#endif
    
#else
    string strRoot = m_strLogDir;
    strRoot += "*.*";
    intptr_t handle; 
    struct _finddata_t tFindData;
    handle=_findfirst(strRoot.c_str(), &tFindData);

    if(-1==handle)
        return strFileName;

    while(0 == _findnext(handle, &tFindData))
    {
        if(tFindData.name[0] == '.') 
            continue; 

        if (tFindData.attrib&_A_SUBDIR)
        {
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(tFindData.name))
        {
            continue;
        }

        if (tModifyFileTime == 0)
        {
            strModifyFile = tFindData.name;
            tModifyFileTime = tFindData.time_write;        
        }       

        if (tFindData.time_write < tModifyFileTime)
        {
            strModifyFile = tFindData.name;
            tModifyFileTime = tFindData.time_write;
        }
    }

    _findclose(handle);   
#endif

    //添加目录
    strFileName = m_strLogDir + strModifyFile;

    return strFileName;
}

string CXLogFile::GetLastFile() const
{
    //获取最新的文件
    string strFileName;

    time_t tModifyFileTime = 0;
    string strModifyFile;

#ifdef _LINUX_

#ifdef USE_SCANDIR
    struct dirent** ppNamelist = NULL;
    s32 nEntryNum = 0;

    nEntryNum = scandir(m_strLogDir.c_str(), &ppNamelist, 0, alphasort);
    if (nEntryNum < 0 || ppNamelist == NULL)
    {
        printf("scandir [%s] fail!!!\n", m_strLogDir.c_str());
        return strFileName;
    }

    s32 nSaveEntryNum = nEntryNum;
    struct stat statFile;
    while(nEntryNum--) 
    {
        struct dirent* pFileEntry = ppNamelist[nEntryNum];

        if(pFileEntry->d_name[0] == '.') 
            continue; 

        string strFullPath = m_strLogDir + pFileEntry->d_name;
        stat(strFullPath.c_str(), &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        if (statFile.st_mtime > tModifyFileTime)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;
        }     
    }

    FreeDirNameList(ppNamelist, nSaveEntryNum);
#else

    DIR* pDir = opendir(m_strLogDir.c_str()); 
    if(pDir == NULL) 
    { 
        return strFileName;
    }

    string strSaveWorkDir = GetProcessPath();
    chdir(m_strLogDir.c_str());

    struct dirent* pFileEntry = NULL;
    struct stat statFile;

    while((pFileEntry = readdir(pDir)) != NULL) 
    { 
        if(pFileEntry->d_name[0] == '.') 
            continue; 

        stat(pFileEntry->d_name, &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        if (statFile.st_mtime > tModifyFileTime)
        {
            strModifyFile = pFileEntry->d_name;
            tModifyFileTime = statFile.st_mtime;
        }
    } 

    chdir(strSaveWorkDir.c_str()); 
    closedir(pDir);

#endif
    
#else

    string strRoot = m_strLogDir;
    strRoot += "*.*";
    intptr_t handle; 
    struct _finddata_t tFindData;
    handle=_findfirst(strRoot.c_str(), &tFindData);

    if(-1==handle)
        return strFileName;

    while(0 == _findnext(handle, &tFindData))
    {
        if(tFindData.name[0] == '.') 
            continue; 

        if (tFindData.attrib&_A_SUBDIR)
        {
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(tFindData.name))
        {
            continue;
        }

        if (tFindData.time_write > tModifyFileTime)
        {
            strModifyFile = tFindData.name;
            tModifyFileTime = tFindData.time_write;
        }
    }

    _findclose(handle);   

#endif

    //添加目录
    strFileName = m_strLogDir + strModifyFile;

    return strFileName;
}



u32 CXLogFile::GetFileNum() const
{
    u32 nFileNum = 0;

#ifdef _LINUX_

#ifdef USE_SCANDIR
    struct dirent** ppNamelist = NULL;
    s32 nEntryNum = 0;

    nEntryNum = scandir(m_strLogDir.c_str(), &ppNamelist, 0, alphasort);
    if (nEntryNum < 0 || ppNamelist == NULL)
    {
        printf("scandir [%s] fail!!!\n", m_strLogDir.c_str());
        return 0;
    }

    s32 nSaveEntryNum = nEntryNum;
    struct stat statFile;
    while(nEntryNum--) 
    {
        struct dirent* pFileEntry = ppNamelist[nEntryNum];

        if(pFileEntry->d_name[0] == '.') 
            continue; 

        string strFullPath = m_strLogDir + pFileEntry->d_name;

        stat(strFullPath.c_str(), &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        nFileNum++;
    }

    FreeDirNameList(ppNamelist, nSaveEntryNum);

#else
    DIR* pDir = opendir(m_strLogDir.c_str()); 
    if(pDir == NULL) 
    { 
        return 0;
    }

    string strSaveWorkDir = GetProcessPath();
    chdir(m_strLogDir.c_str());

    struct dirent* pFileEntry = NULL;
    struct stat statFile;

    while((pFileEntry = readdir(pDir)) != NULL) 
    { 
        if(pFileEntry->d_name[0] == '.') 
            continue; 

        stat(pFileEntry->d_name, &statFile); 
        if(S_ISDIR(statFile.st_mode)) 
        { 
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(pFileEntry->d_name))
        {
            continue;
        }

        nFileNum++;
    } 

    chdir( strSaveWorkDir.c_str()); 
    closedir(pDir);
#endif

#else

    string strRoot = m_strLogDir;
    strRoot += "*.*";
    intptr_t handle; 
    struct _finddata_t tFindData;
    handle=_findfirst(strRoot.c_str(), &tFindData);

    if(-1==handle)
        return 0;

    while(0 == _findnext(handle, &tFindData))
    {
        if(tFindData.name[0] == '.') 
            continue; 

        if (tFindData.attrib&_A_SUBDIR)
        {
            continue;
        }

        if (GetSuffixName() != GetSuffixFromFile(tFindData.name))
        {
            continue;
        }

        nFileNum++;
    }

    _findclose(handle);   
#endif

    return nFileNum;
}

string CXLogFile::GetFileStamp()
{
    //文件名称的时间标识
    TOspTimeInfo tTimeInfo;
    memset(&tTimeInfo, 0, sizeof(tTimeInfo));
    OspGetTimeInfo(&tTimeInfo);
    s8 szTimeBuf[512] = {0};
    sprintf(szTimeBuf, "%s-%04d-%02d-%02d-%02d-%02d-%02d",
        GetProcessName().c_str(), 
        tTimeInfo.m_wYear, tTimeInfo.m_wMonth, tTimeInfo.m_wDay,
        tTimeInfo.m_wHour, tTimeInfo.m_wMinute, tTimeInfo.m_wSecond);

    return string(szTimeBuf);
}

string CXLogFile::GetSuffixFromFile(const string& strFileName)
{
    string strSuffix;
    if (strFileName.empty())
    {
        return strSuffix;
    }

    const char* pPosLastDot = strrchr(strFileName.c_str(), '.');
    if (pPosLastDot != NULL)
    {
        strSuffix = pPosLastDot;
    }

    return strSuffix;
}

/*====================================================================
函数名      : OspExtOpenLogFile
功能        : 开启ospext日志文件功能
算法实现    : 日志可以分类型
引用全局变量:
输入参数说明:
             strLogDir: 日志文件的输出目录，格式: "./runlog/" 或 "/opt/kdm/cmu/runlog/"
             nLogSizeMax_kb: 日志文件的容量，单位：kb
             nLogNumMax: 日志文件的最大数目
             dwLogType: 日志文件类型: LOG_FILE_ERR 或 LOG_FILE_RUN
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/03/01  0.1         fanxg
====================================================================*/
bool OspExtOpenLogFile(const string& strLogDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType)
{
    if (dwLogType == LOG_FILE_RUN)
    {
        g_cRunLogFile.SetLogFileParam(strLogDir, nLogSizeMax_kb, nLogNumMax, dwLogType);
        return true;
    }
    else if (dwLogType == LOG_FILE_ERR)
    {
        g_cErrLogFile.SetLogFileParam(strLogDir, nLogSizeMax_kb, nLogNumMax, dwLogType);
        return true;
    }
    else
    {

    }

    return false;
}

