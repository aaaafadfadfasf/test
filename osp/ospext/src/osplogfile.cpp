#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#endif

#include "cms/ospext/osplog.h"
#include "cms/ospext/ospmailbox.h"
#include "osplogfile.h"

#ifdef _LINUX_
//#include <dirent.h>

//ʹ��scandir����Ŀ¼���ýӿڿ��Բ��ı䵱ǰ����Ŀ¼
#define  USE_SCANDIR

//�ͷ�scandir���ص��ڴ�
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
            //Ͷ��ʧ�ܣ�ֱ��ɾ��
            delete ptLogData;
            ptLogData = NULL;
        }
    }
}

//��ʾ��־������Ϣ
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

//��־�ļ�
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
    //windowsҲ����ʹ��"/"��Ϊ���Ŀ¼�ָ����������滻��
    replace_str(m_strLogDir, "/", "\\");
#endif

    //��������Ŀ¼·��
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

    const char* szOpenMode = "a+";   //Ĭ�ϴ򿪷�ʽ,׷��

    u32 nSaveCurLogIdx = m_nCurLogIdx;

    //Ѱ�ҿ�д�ļ�
    while(m_nCurLogIdx < m_nLogNumMax)
    {
        if (ExistFileOrDir(szFileName)) //�ļ�����    
        {
            struct stat fileStat;
            stat(szFileName, &fileStat);
            long dwLogSize = fileStat.st_size;

            if (dwLogSize >= (long)(m_nLogSizeMax*CAPACITY_1K)) //�������log����,�����ļ�
            {
                //�ȹرյ�ǰ�ļ�
                if (m_pFile != NULL)
                {
                    fclose(m_pFile);
                    m_pFile = NULL;
                }

                m_nCurLogIdx++;
                if(m_nCurLogIdx >= m_nLogNumMax)  //�ļ���Ŀ�ﵽ�����
                {
                    if (nSaveCurLogIdx >= m_nLogNumMax - 1)
                    {
                        //�ӵ�һ���ļ���ʼ����д
                        m_nCurLogIdx = 0;                    
                    }
                    else
                    {
                        //����һ���ļ���ʼ����д
                        m_nCurLogIdx = nSaveCurLogIdx + 1;
                    }

                    sprintf(szFileName, "%s%u-%s", m_strLogDir.c_str(), m_nCurLogIdx, m_strLogName.c_str());
                    szOpenMode = "w+";		//�򿪷�ʽ,�ƻ�
                    break;
                }
                else
                {
                    //����Ѱ�ҵ�ǰ��д�ļ�
                    sprintf(szFileName, "%s%u-%s", m_strLogDir.c_str(), m_nCurLogIdx, m_strLogName.c_str());
                    continue;
                }
            }
            else
            {
                //�ļ����ڣ�����δ�ﵽ����������ǿ�д�ļ�
                break;
            }
        }
        else //�ļ������ڣ��϶���д��ֱ�Ӵ���
        {
            //���ļ������Ϊ�գ��������û��ֶ�ɾ������־�ļ�������Ч�������Ҫclose
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

    //Ѱ�ҿ�д�ļ�
    if (ExistFileOrDir(m_strCurFile.c_str())) //�ļ�����    
    {
        struct stat fileStat;
        stat(m_strCurFile.c_str(), &fileStat);
        long dwLogSize = fileStat.st_size;

        if (dwLogSize >= (long)(m_nLogSizeMax*CAPACITY_1K)) //�������log����,�����ļ�
        {
            //�ȹرյ�ǰ�ļ�
            if (m_pFile != NULL)
            {
                fclose(m_pFile);
                m_pFile = NULL;
            }

            //ɾ�����ϵ��ļ�
            if (GetFileNum() >= m_nLogNumMax)
            {
                string strFirstFile = GetFirstFile();
                DelFile(strFirstFile.c_str());
            }

            //�������ļ�
            m_strCurFile = GetNewFile();      
        }
        else
        {
            //�ļ����ڣ�����δ�ﵽ����������ǿ�д�ļ�
        }
    }
    else //�ļ������ڣ��϶���д��ֱ�Ӵ���
    {
        //���ļ������Ϊ�գ��������û��ֶ�ɾ������־�ļ�������Ч�������Ҫclose
        if (m_pFile != NULL)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
    }

    if (m_pFile == NULL)
    {
        const char* szOpenMode = "a+";   //Ĭ�ϴ򿪷�ʽ,׷��
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
			//ʧ�ܺ��д��messages����messages�����������ѡ��˴�����
            //syslog(LOG_INFO, "ospext-CXLogFile::WriteLogFileNew-fwrite file[%s] fail, size[%u], buf[%s]\n",
            //    m_strCurFile.c_str(),  strLog.size(), strLog.c_str());
#endif
            return;
        }
        
        if (fflush(m_pFile) != 0)
        {
#ifdef _LINUX_
			//ʧ�ܺ��д��messages����messages�����������ѡ��˴�����
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
        //�½�һ���ļ�
        strFileName = GetNewFile();
    }
    else
    {
        //��һ�����µ��ļ�
        strFileName = GetLastFile();
    }
   
    return strFileName;
}

string CXLogFile::GetNewFile() const
{
    //�������ļ�
    string strFileName;
    strFileName = GetFileStamp();
    strFileName += GetSuffixName();

    //���Ŀ¼
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
    //��ȡ���ϵ��ļ�
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

    //���Ŀ¼
    strFileName = m_strLogDir + strModifyFile;

    return strFileName;
}

string CXLogFile::GetLastFile() const
{
    //��ȡ���µ��ļ�
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

    //���Ŀ¼
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
    //�ļ����Ƶ�ʱ���ʶ
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
������      : OspExtOpenLogFile
����        : ����ospext��־�ļ�����
�㷨ʵ��    : ��־���Է�����
����ȫ�ֱ���:
�������˵��:
             strLogDir: ��־�ļ������Ŀ¼����ʽ: "./runlog/" �� "/opt/kdm/cmu/runlog/"
             nLogSizeMax_kb: ��־�ļ�����������λ��kb
             nLogNumMax: ��־�ļ��������Ŀ
             dwLogType: ��־�ļ�����: LOG_FILE_ERR �� LOG_FILE_RUN
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
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

