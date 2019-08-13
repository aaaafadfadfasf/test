#include "cms/ospext/osptool.h"
#include "cms/ospext/osplog.h"
#include "cms/ospext/ospdbg.h"
#include <sys/timeb.h>

#ifdef WIN32
#include "IPHlpApi.h"
#else /*WIN32*/
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#endif

#define MAX_MSG_SEQ_NO ((u32)0x1FFFFFFF)

static CEventDescMap g_cEventDescMap;
static u32 g_dwSeqGenerator = 1;


void CEventDescMap::AddEventDesc (const char *  desc, u16 event)
{
    CAutoLock cLock(m_cEventMapLock);
    string strEvent;
    if ( !m_cIDToEventDescMap.Find(event, strEvent) )
    {
        m_cEventDescToIDMap.Insert(desc, event);
        m_cIDToEventDescMap.Insert(event, desc);

        //����ʹ��Osp����Ϣӳ��
    }
    else
    {
        OspPrintf(TRUE, FALSE, "�Ѿ�����������Ϣ[%s--%d]��ӵ�CEventDescMap����!!!��Ϣ[%s--%d]���ʧ��\n",
            strEvent.c_str(), event, desc, event);
    }
}

u16 CEventDescMap::GetEventID(const char* szEventDesc)
{
    CAutoLock cLock(m_cEventMapLock);
    u16 *pwEventId = m_cEventDescToIDMap.Find(szEventDesc);
    if ( pwEventId )
    {
        return *pwEventId;
    }
    return INVALID_OSP_EVENT;
}

const string CEventDescMap::GetEventDesc( u16 wEventID )
{
    CAutoLock cLock(m_cEventMapLock);
    string strEvent;
    if ( m_cIDToEventDescMap.Find(wEventID, strEvent) )
    {
        return strEvent;
    }
    return string("");
}


u16 OspExtEventID(const char* szEventDesc)
{
    return g_cEventDescMap.GetEventID(szEventDesc);
}

const string OspExtEventDesc( u16 wEventID )
{
    return g_cEventDescMap.GetEventDesc(wEventID);
}

void OspExtAddEventDescAndID(const char * desc, u16 event)
{
//    OspAddEventDesc(desc,event);    //����ʹ��Osp����Ϣ����

    g_cEventDescMap.AddEventDesc(desc, event);
}

const u32 GenSequenceNum()
{
    u32 dwRet = g_dwSeqGenerator;
    if ( g_dwSeqGenerator >= MAX_MSG_SEQ_NO )
    {
        g_dwSeqGenerator = 1;
    }
    else
    {
        ++g_dwSeqGenerator;
    }
    return dwRet;
}

//ʱ����ʵ��
/*
    ֮���Բ���GetTick��ԭ�����£�
    1������Ƶ��ϵͳ����
    2��Linux��Tick��10msΪ��λ������ȷ
*/
TMsTime OspGetMsTime()
{
    struct timeb tp;
    ftime(&tp);
    TMsTime tMsTime;
    tMsTime.tSecond = tp.time;
    tMsTime.tMilliSecond = tp.millitm;

    return tMsTime;
}
u32 OspGetDiffMs(const TMsTime& tEndTime, const TMsTime& tStartTime)
{
    //�������ʱ��������1193��Сʱ��u32���޷����棬�ú��������
    //ҵ�����Ӧ��ʹ�øú�����ȡ��ʱ���ڵĺ����
    u32 dwDiffMs = 1000*(u32(tEndTime.tSecond - tStartTime.tSecond)) + (tEndTime.tMilliSecond - tStartTime.tMilliSecond);
    return dwDiffMs;
}

//����ϵͳticks����ȡ�뼶ʱ�䣬��������ʱ���Ĳ�����Ϊ�޸���ϵͳʱ��������쳣
u64 OspGetSecByTicks()
{
    //��ȡticks���ýӿڲ������������
    u64 lwTicks = OspTickGet64();

    //ticks/sec
    u32 dwClkRate = OspClkRateGet();

    //���������
    return (lwTicks / dwClkRate);
}

//�̰߳�ȫ�Ļ�ȡlocaltime
tm GetLocalTime_r(const time_t* t)
{
    tm tTm = { 0 };  
#ifdef _LINUX_
    localtime_r(t, &tTm);
    return tTm;
#else
    tm* ptTm = localtime(t);
    if (ptTm != NULL)
    {
        tTm = *ptTm;
    }
#endif

    return tTm;
}

COspTime::COspTime()
{
    Clear();
}

COspTime::COspTime(time_t tTime)
{
    SetTime(&tTime);
}

COspTime::COspTime(const COspTime& tObj)
{
    (*this) = tObj;
}

COspTime& COspTime::operator = (const COspTime& tObj)
{
    if (this != &tObj)
    {
        m_wYear    = tObj.m_wYear;
        m_byMonth  = tObj.m_byMonth;
        m_byMDay   = tObj.m_byMDay;
        m_byHour   = tObj.m_byHour;
        m_byMinute = tObj.m_byMinute;
        m_bySecond = tObj.m_bySecond;
        m_wMillSec = tObj.m_wMillSec;
    }

    return *this;
}

void COspTime::Clear()
{
    m_wYear    = 0;
    m_byMonth  = 0;
    m_byMDay   = 0;
    m_byHour   = 0;
    m_byMinute = 0;
    m_bySecond = 0;
    m_wMillSec = 0;
}

void COspTime::SetTime( const time_t *ptTime )
{
    if (NULL == ptTime)
    {
        return;
    }

    tm tLocalTime = GetLocalTime_r(ptTime);
    tm *ptLocalTime = &tLocalTime;
    if (NULL != ptLocalTime)
    {
        m_wYear    = (u16)ptLocalTime->tm_year + 1900;
        m_byMonth  = (u8)ptLocalTime->tm_mon + 1;
        m_byMDay   = (u8)ptLocalTime->tm_mday;
        m_byHour   = (u8)ptLocalTime->tm_hour;
        m_byMinute = (u8)ptLocalTime->tm_min;
        m_bySecond = (u8)ptLocalTime->tm_sec;
        m_wMillSec = 0;
    }
    else
    {
        Clear();
    }
}


void COspTime::GetTime( time_t &tTime ) const
{
    tTime = GetTime();
}

time_t COspTime::GetTime(void) const
{    
    tm  tTmCurTime;
    
    //���뽫�ñ�����ʼ��Ϊ-1��������������ʱ�ĵط���ʱ���п��ܻ����1��Сʱ��ʱ��
    tTmCurTime.tm_isdst = -1;

    u16 wYear = m_wYear;
    if (wYear >= 1900)
    {
        tTmCurTime.tm_year = wYear - 1900;
    }
    else
    {
        tTmCurTime.tm_year = 106;
    }
    if (m_byMonth > 0 && m_byMonth <= 12)
    {
        tTmCurTime.tm_mon  = m_byMonth - 1;
    }
    else
    {
        tTmCurTime.tm_mon  = 0;
    }
    if (m_byMDay > 0 && m_byMDay <= 31)
    {
        tTmCurTime.tm_mday = m_byMDay;
    }
    else
    {
        tTmCurTime.tm_mday = 1;
    }
    if (m_byHour < 24)
    {
        tTmCurTime.tm_hour = m_byHour;
    }
    if (m_byMinute < 60)
    {
        tTmCurTime.tm_min  = m_byMinute;
    }
    if (m_bySecond < 60)
    {
        tTmCurTime.tm_sec  = m_bySecond;
    }
    
    return ::mktime( &tTmCurTime );
}

BOOL32 COspTime::operator == ( const COspTime &tObj ) const
{
    if (m_wYear    == tObj.m_wYear&&
        m_byMonth  == tObj.m_byMonth&&
        m_byMDay   == tObj.m_byMDay&&
        m_byHour   == tObj.m_byHour&&
        m_byMinute == tObj.m_byMinute&&
        m_bySecond == tObj.m_bySecond&&
        m_wMillSec == tObj.m_wMillSec)
    {
        return TRUE;
    }
    return FALSE;
}

BOOL32 COspTime::GetString(s8* pachString, s32 nStringLen) const
{
    if (nStringLen < 20)
    {
        return FALSE;
    }
    sprintf(pachString, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 
            m_wYear, m_byMonth, m_byMDay, m_byHour, m_byMinute, m_bySecond);         
    return TRUE;
}

COspTime COspTime::GetCurrTime()
{
    COspTime tOspTime;

    struct timeb tp;
    ftime(&tp);

    tm tLocalTime = GetLocalTime_r(&tp.time);
    tm *ptLocalTime = &tLocalTime;

    if (NULL != ptLocalTime)
    {
        tOspTime.m_wYear    = (u16)ptLocalTime->tm_year + 1900;
        tOspTime.m_byMonth  = (u8)ptLocalTime->tm_mon + 1;
        tOspTime.m_byMDay   = (u8)ptLocalTime->tm_mday;
        tOspTime.m_byHour   = (u8)ptLocalTime->tm_hour;
        tOspTime.m_byMinute = (u8)ptLocalTime->tm_min;
        tOspTime.m_bySecond = (u8)ptLocalTime->tm_sec;
        tOspTime.m_wMillSec = tp.millitm;
    }

    return tOspTime;
}

string COspTime::GetCurrStrTime()
{
    char szStrTime[64] = {0};
    
    TOspTimeInfo tTimeInfo;
    memset(&tTimeInfo, 0, sizeof(tTimeInfo));
    OspGetTimeInfo(&tTimeInfo);
    
    sprintf(szStrTime, "%04u-%02u-%02u %02u:%02u:%02u",
        tTimeInfo.m_wYear,
        tTimeInfo.m_wMonth,
        tTimeInfo.m_wDay,
        tTimeInfo.m_wHour,
        tTimeInfo.m_wMinute,
        tTimeInfo.m_wSecond);
    
    return string(szStrTime);
}

string COspTime::GetCurrStrTime_ms()
{
    char szStrTime[64] = {0};
    
    struct timeb tp;
    ftime(&tp);
    
    TOspTimeInfo tTimeInfo;
    memset(&tTimeInfo, 0, sizeof(tTimeInfo));
    
    tm tLocalTime = GetLocalTime_r(&tp.time);
    tm *ptLocalTime = &tLocalTime;
    if (NULL != ptLocalTime)
    {
        tTimeInfo.m_wYear    = (u16)ptLocalTime->tm_year + 1900;
        tTimeInfo.m_wMonth  = (u16)ptLocalTime->tm_mon + 1;
        tTimeInfo.m_wDay   = (u16)ptLocalTime->tm_mday;
        tTimeInfo.m_wHour   = (u16)ptLocalTime->tm_hour;
        tTimeInfo.m_wMinute = (u16)ptLocalTime->tm_min;
        tTimeInfo.m_wSecond = (u16)ptLocalTime->tm_sec;
    }
    
    sprintf(szStrTime, "%04u-%02u-%02u %02u:%02u:%02u-%03u",
        tTimeInfo.m_wYear,
        tTimeInfo.m_wMonth,
        tTimeInfo.m_wDay,
        tTimeInfo.m_wHour,
        tTimeInfo.m_wMinute,
        tTimeInfo.m_wSecond,
        u16(tp.millitm));
    
    return string(szStrTime);
}

string COspTime::GetString() const
{
    char szStrTime[64] = {0};

    sprintf(szStrTime, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 
            m_wYear, m_byMonth, m_byMDay, m_byHour, m_byMinute, m_bySecond);

    return string(szStrTime);
}

//����ʵ��
CXSemLock::CXSemLock()
{
    if (OspSemBCreate(&m_hSemaphore) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemBCreate() ����ʧ��\n");
    }
}

CXSemLock::~CXSemLock()
{
    if (m_hSemaphore != NULL)
    {
        if (OspSemDelete(m_hSemaphore) == FALSE)
        {
            OspPrintf(FALSE, TRUE, "OspSemDelete() ����ʧ��\n");
        }

        m_hSemaphore = NULL;
    }
}

BOOL32 CXSemLock::Lock(u32 dwTimeout)
{
    if (OspSemTakeByTime(m_hSemaphore, dwTimeout) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemTakeByTime() ����ʧ��\n");
        return FALSE;
    }
    
    return TRUE;
}

BOOL32 CXSemLock::Lock()
{
	if (OspSemTake(m_hSemaphore) == FALSE)
	{
		OspPrintf(FALSE, TRUE, "OspSemTake() ����ʧ��\n");
		return FALSE;
	}

	return TRUE;
}

BOOL32 CXSemLock::UnLock()
{
    if (OspSemGive(m_hSemaphore) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemGive() ����ʧ��\n");
        return FALSE;
    }
    
    return TRUE;
}

BOOL32 CXSemLock::Take(u32 dwTimeout)
{
    return Lock(dwTimeout);
}

BOOL32 CXSemLock::Take()
{
	 return Lock();
}

BOOL32 CXSemLock::Give()
{        
    return UnLock();
}

//�ź���
CXSemaphore::CXSemaphore(u32 dwInitCnt, u32 dwMaxCnt)
{
    if (OspSemCCreate(&m_hSemaphore, dwInitCnt, dwMaxCnt) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemCCreate() ����ʧ��\n");
    }
}

CXSemaphore::~CXSemaphore()
{
    if (m_hSemaphore != NULL)
    {
        if (OspSemDelete(m_hSemaphore) == FALSE)
        {
            OspPrintf(FALSE, TRUE, "OspSemDelete() ����ʧ��\n");
        }

        m_hSemaphore = NULL;
    }
}

BOOL32 CXSemaphore::Take(u32 dwTimeout)
{
    if (OspSemTakeByTime(m_hSemaphore, dwTimeout) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemTakeByTime() ����ʧ��\n");
        return FALSE;
    }

    return TRUE;
}

BOOL32 CXSemaphore::Take()
{
	if (OspSemTake(m_hSemaphore) == FALSE)
	{
		OspPrintf(FALSE, TRUE, "OspSemTake() ����ʧ��\n");
		return FALSE;
	}

	return TRUE;
}

BOOL32 CXSemaphore::Give()
{
    if (OspSemGive(m_hSemaphore) == FALSE)
    {
        OspPrintf(FALSE, TRUE, "OspSemGive() ����ʧ��\n");
        return FALSE;
    }

    return TRUE;
}

//�Զ���ʵ��
CAutoLock::CAutoLock(CXSemLock& rLock) : m_Lock(rLock)
{
    m_Lock.Lock();
}

CAutoLock::~CAutoLock()
{
    m_Lock.UnLock();
}

//��ȡ��������
string GetProcessName()
{
    string strProcessName = "";
    s8 szFullPath[1024] = {0};

#ifdef _LINUX_
    readlink("/proc/self/exe", szFullPath, sizeof(szFullPath));
    strProcessName = (strrchr(szFullPath, '/') + 1);
#else
    GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
    strProcessName = (strrchr(szFullPath, '\\') + 1);
#endif

    return strProcessName;
}

//��ȡ����·��
string GetProcessPath()
{
    string strProcessPath = "";
    s8 szFullPath[1024] = {0};

#ifdef _LINUX_
    readlink("/proc/self/exe", szFullPath, sizeof(szFullPath));
    *(strrchr(szFullPath, '/') + 1) = '\0';  
#else
    GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
    *(strrchr(szFullPath, '\\') + 1) = '\0';
#endif

    strProcessPath = szFullPath;
    return strProcessPath;
}

string GetProcessFullPath()
{
    string strProcessPath = "";
    s8 szFullPath[1024] = {0};

#ifdef _LINUX_
    readlink("/proc/self/exe", szFullPath, sizeof(szFullPath));
#else
    GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
#endif

    strProcessPath = szFullPath;
    return strProcessPath;
}


//�ַ����Ͷ�����Խӿ�
#ifdef   _LINUX_
string CharsetConvert(const CCharset& cFrom, const CCharset& cTo, const string& strIn)
{
    string strOut;

    iconv_t cd;
    cd = iconv_open(cTo.c_str(), cFrom.c_str());
    if(cd == (iconv_t) -1)
    {
		GLOBELOG(OSPEXT, ERROR_LEV, "iconv_open fail, errno[%s-%d]\n", strerror(errno), errno);
        return strOut;
    }

	// ת����ĳ��Ȳ�����ת��ǰ���ȵ�3��
	size_t nOutLen = 3 * strIn.length();
	char *szOut = new char[nOutLen + 1];
	memset(szOut, 0, nOutLen + 1);
	CMS_ASSERT(NULL != szOut && "szOut = NULL!!!");
	char *szOutBuf = szOut;

    char* szIn = (char*)strIn.c_str();
    size_t nInLen = strIn.size();
    size_t nRet = iconv(cd, &szIn, &nInLen, &szOutBuf, &nOutLen);
    iconv_close(cd);

    if (nRet == size_t(-1))
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "iconv fail, errno[%s-%d]\n", strerror(errno), errno);
    }
    else
    {
        strOut = string(szOut);
    }

	if (NULL != szOut)
	{
		delete[] szOut;
		szOut = NULL;
	}

    return strOut;
}
#endif

wstring MbsToUcs(const CCharset& cFrom, const string& strIn)
{
#ifdef _LINUX_
    string strUnicode = CharsetConvert(cFrom, ICONV_CHARSET_UNICODE, strIn);
    wchar_t* pwch = (wchar_t*)strUnicode.c_str();
    return wstring(pwch);
#else

    UNREFERENCED_PARAMETER(cFrom);
    wstring wstrUnicode;

    int size = MultiByteToWideChar( 
        CP_ACP,
        0,
        strIn.c_str(),
        -1,
        NULL,
        0);

    if (size <= 0)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "MultiByteToWideChar failed\n");
        return wstrUnicode;
    }

    wchar_t* buffer = new wchar_t[size_t(size)];

    size = ::MultiByteToWideChar(
        CP_ACP, 
        0, 
        strIn.c_str(), 
        -1, 
        buffer, 
        size);

    wstrUnicode = buffer;

    delete[] buffer;
    buffer = NULL;

    return wstrUnicode;
#endif
}
string UcsToMbs(const CCharset& cTo, const wstring& wstrIn)
{
#ifdef _LINUX_
    char* pch = (char*)wstrIn.c_str();
    string strMbs = CharsetConvert(ICONV_CHARSET_UNICODE, cTo, pch);
    return strMbs;
#else
    UNREFERENCED_PARAMETER(cTo);

    string strMbs;

    int size = ::WideCharToMultiByte(
        CP_ACP, 
        0, 
        wstrIn.c_str(), 
        -1, 
        NULL, 
        0, 
        NULL, 
        NULL);

    if (size <= 0)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "MultiByteToWideChar failed\n");
        return strMbs;
    }

    char* buffer = new char[size_t(size)];

    size = ::WideCharToMultiByte(
        CP_ACP, 
        NULL, 
        wstrIn.c_str(), 
        -1, 
        buffer, 
        size, 
        NULL, 
        NULL);

    strMbs = buffer;

    delete[] buffer;
    buffer = NULL;

    return strMbs;
#endif
}

wstring Utf8ToUcs(const string& strIn)
{
    {
#ifdef _LINUX_
        string strUnicode = CharsetConvert(ICONV_CHARSET_UTF8, ICONV_CHARSET_UNICODE, strIn);
        wchar_t* pwch = (wchar_t*)strUnicode.c_str();
        return wstring(pwch);
#else
        wstring wstrUnicode;

        int size = MultiByteToWideChar( 
            CP_UTF8,
            0,
            strIn.c_str(),
            -1,
            NULL,
            0);

        if (size <= 0)
        {
            GLOBELOG(OSPEXT, ERROR_LEV, "MultiByteToWideChar failed\n");
            return wstrUnicode;
        }

        wchar_t* buffer = new wchar_t[(size_t)size];

        size = ::MultiByteToWideChar(
            CP_UTF8, 
            0, 
            strIn.c_str(), 
            -1, 
            buffer, 
            size);

        wstrUnicode = buffer;

        delete[] buffer;
        buffer = NULL;

        return wstrUnicode;
#endif
    }
}
string UcsToUtf8(const wstring& wstrIn)
{
#ifdef _LINUX_
    char* pch = (char*)wstrIn.c_str();
    string strUTF8 = CharsetConvert(ICONV_CHARSET_UNICODE, ICONV_CHARSET_UTF8, pch);
    return strUTF8;
#else
    string strUTF8;

    int size = ::WideCharToMultiByte(
        CP_UTF8, 
        0, 
        wstrIn.c_str(), 
        -1, 
        NULL, 
        0, 
        NULL, 
        NULL);

    if (size <= 0)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "MultiByteToWideChar failed\n");
        return strUTF8;
    }

    char* buffer = new char[(size_t)size];

    size = ::WideCharToMultiByte(
        CP_UTF8, 
        NULL, 
        wstrIn.c_str(), 
        -1, 
        buffer, 
        size, 
        NULL, 
        NULL);

    strUTF8 = buffer;

    delete[] buffer;
    buffer = NULL;

    return strUTF8;
#endif
}

string Utf8ToMbs(const CCharset& cTo, const string& strIn)
{
#ifdef _LINUX_
    return CharsetConvert(ICONV_CHARSET_UTF8, cTo, strIn);
#else
    wstring wstrUnicode = Utf8ToUcs(strIn);
    return UcsToMbs(cTo, wstrUnicode);
#endif
}
string MbsToUtf8(const CCharset& cFrom, const string& strIn)
{
#ifdef _LINUX_
    return CharsetConvert(cFrom, ICONV_CHARSET_UTF8, strIn);
#else
    wstring wstrUnicode = MbsToUcs(cFrom, strIn);
    return UcsToUtf8(wstrUnicode);
#endif
}


wstring UTF8ToUnicode(const string& strUTF8)
{
    return Utf8ToUcs(strUTF8);
}

string UnicodeToGBK(const wstring& wstrSrc)
{
    return UcsToMbs(ICONV_CHARSET_GBK, wstrSrc);
}

string UTF8ToGBK(const string& strUTF8)
{
    return Utf8ToMbs(ICONV_CHARSET_GBK, strUTF8);
}

wstring GBKToUnicode(const string& strGBK)
{
    return MbsToUcs(ICONV_CHARSET_GBK, strGBK);
}

string UnicodeToUTF8(const wstring& wstrSrc)
{
    return UcsToUtf8(wstrSrc);
}

string GBKToUTF8(const string& strGBK)
{
    return MbsToUtf8(ICONV_CHARSET_GBK, strGBK);
}

string DotNETNameToLinuxCharSet(const string& DotNETName)
{
  if ( DotNETName== "ibm857")
    {
        return ICONV_CHARSET_CP1254;
    }
    else if (DotNETName == "ks_c_5601-1987")
    {
        return ICONV_CHARSET_CP949;
    }
    else if (DotNETName == "gb2312")
    {
        return ICONV_CHARSET_GB2312;
    }
    else
    {
        return  ICONV_CHARSET_GBK;
    }
}
#define MAX_IP_NUM 20
#define IP_127_0_0_1    0x0100007f
#define IP_LEN 32

//IP��ַ��ʽ����ʹ�÷�ʽ��sprintf((s8 *)pchIp, "%d.%d.%d.%d", FORMAT_IP((u32)dwIp));
#define FORMAT_IP(addr) \
    (int)(((unsigned char *)&(addr))[0]), \
    (int)(((unsigned char *)&(addr))[1]), \
    (int)(((unsigned char *)&(addr))[2]), \
    (int)(((unsigned char *)&(addr))[3])

//��ȡ����������
#ifdef _LINUX_
#define ETHTOOL_GLINK 0x0000000a /* Get link status (ethtool_value) */
/* for passing single values */
struct ethtool_value
{
    u32 cmd;
    u32 data;
};
#endif

struct _TAddrTable
{
    u32 dwLocalAddr;
    u32 dwInterface;
    u32 dwMask;
};

char *IPtoStr(u32 dwIP, char szIpStr[], u32 dwLen)
{
    memset(szIpStr, 0, dwLen);

    sprintf(szIpStr, "%d.%d.%d.%d", FORMAT_IP(dwIP));

    return szIpStr;
}


/*========================================================================
�� �� ����IsNicConnect
��    �ܣ������Ƿ�����
��    ���������豸��"eth0"/"eth1" ..../"eth16" ��֧�ֵ�16
�� �� ֵ��TRUEΪ����״̬ FALSEΪ�Ͽ�״̬
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
˵�������´��뿽����cnetmanager.h�У�������Ҫ��Cʵ�֣�����ֲ
========================================================================*/
BOOL32 IsNicConnect(const s8 *achDevName)
{
#ifdef _LINUX_
    ifreq ifr;
    ethtool_value tIsConnect;
    s32 nSocket = 0;

    memset(&ifr, 0, sizeof(ifr));
    tIsConnect.cmd = ETHTOOL_GLINK;

    /* Open control socket. */
    nSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (nSocket < 0)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "[IsNicConnet]Create Socket Failed\n");
        return FALSE;
    }
    strncpy(ifr.ifr_name, achDevName, sizeof(ifr.ifr_name)-1);
    ifr.ifr_data = (char *) &tIsConnect;

    if (ioctl(nSocket, SIOCETHTOOL, &ifr) == -1)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "[IsNicConnet]ETHTOOL_GLINK failed, errno:[%d]%s\n", errno, strerror(errno));
        close(nSocket);
        return FALSE;
    }

    close(nSocket);
    //return (tIsConnect.data ? FALSE : TRUE);
    return (tIsConnect.data ? TRUE : FALSE);    //ʵ������������������ tIsConnect.data = 1
#endif
    return TRUE;
}

bool GetAddrList(TNetIfParamList& tList)
{
#ifdef _MSC_VER

    PMIB_IPADDRTABLE ptIpTable = NULL;
    u32 dwOutBufLen = NULL;
    GetIpAddrTable(ptIpTable, (PULONG)&dwOutBufLen,TRUE);
    ptIpTable = (PMIB_IPADDRTABLE)malloc(dwOutBufLen);
    if (NULL == ptIpTable)
    {
        return false;
    }
    if (NO_ERROR != GetIpAddrTable(ptIpTable, (PULONG)&dwOutBufLen,TRUE))
    {
        free(ptIpTable);
        return false;
    }

    _TAddrTable* ptAddrTable = new _TAddrTable[ptIpTable->dwNumEntries];
    if (NULL == ptAddrTable)
    {
        free(ptIpTable);
        return false;
    }
    size_t dwSize = 0;
    s8 achIp[IP_LEN]  = {0};
    for (u32 dw = 0; dw < ptIpTable->dwNumEntries; dw++)
    {
        if (IP_127_0_0_1 != ptIpTable->table[dw].dwAddr && 0 != ptIpTable->table[dw].dwAddr)
        {
            TNetIfParam tNetIfParam;

            ptAddrTable[dwSize].dwLocalAddr = /*ntohl*/(ptIpTable->table[dw].dwAddr);
            IPtoStr(ptAddrTable[dwSize].dwLocalAddr, achIp, IP_LEN);
            tNetIfParam.strNetAddr = achIp;

            ptAddrTable[dwSize].dwInterface = ptIpTable->table[dw].dwIndex;
            IPtoStr(ptAddrTable[dwSize].dwInterface, achIp, IP_LEN);
            tNetIfParam.strIfName = achIp;

            ptAddrTable[dwSize].dwMask = /*ntohl*/(ptIpTable->table[dw].dwMask);
            IPtoStr(ptAddrTable[dwSize].dwMask, achIp, IP_LEN);
            tNetIfParam.strNetMask = achIp;

            tList.push_back(tNetIfParam);

            dwSize++;
        }
    }

    free(ptIpTable);

    delete[] ptAddrTable;
    ptAddrTable = NULL;

#else /*_MSC_VER*/

    u16 wIpNum = 0;  // ʵ����ЧIP����, �ų�lo
    SOCKHANDLE tSocketRef;
    struct ifconf tIfconf;
    struct ifreq *ptIfreq;
    s8 achBuffer[MAX_IP_NUM * sizeof(struct ifreq)];
    u32 wIndex = 0;
    u32 dwGetIp = 0;
    struct sockaddr_in *ptSockaddrIn;
    u16 wDevIndex = 0;
    _TAddrTable * ptTempTable;

    // ��ȡ socket ���
    tSocketRef = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == tSocketRef)
    {
        GLOBELOG(OSPEXT, ERROR_LEV, "create socket failed, errno:[%d]%s\n", errno, strerror(errno));
        return false;
    }

    // ���� ifconfig �ṹ����������
    tIfconf.ifc_len = MAX_IP_NUM * sizeof(struct ifreq);
    tIfconf.ifc_buf = achBuffer;
    // ��ȡ�������� IP �б�
    if (ioctl(tSocketRef, SIOCGIFCONF, &tIfconf) < 0)
    {
        SockClose(tSocketRef);
        GLOBELOG(OSPEXT, ERROR_LEV, "ioctl socket failed, errno:[%d]%s\n", errno, strerror(errno));
        return false;
    }
    //ʵ�ʻ�ȡ�� IP ��
    size_t dwSize = tIfconf.ifc_len / sizeof(struct ifreq);
    _TAddrTable* ptAddrTable = new _TAddrTable[dwSize];
    if (NULL == ptAddrTable)
    {
        SockClose(tSocketRef);    
        return false;
    }

    ptIfreq = tIfconf.ifc_req;
    s8 achIp[IP_LEN]  = {0};
    //�ȱ�������ioctlö�ٵ�IP���ų�lo��Ȼ������豸������ֵ���뵽��ʱIP�б���
    for (wIndex = 0 ; wIndex < dwSize ; wIndex++, ptIfreq++)
    {
        ptSockaddrIn = (struct sockaddr_in*)(&ptIfreq->ifr_addr);
        dwGetIp = ptSockaddrIn->sin_addr.s_addr;
        if (ioctl(tSocketRef, SIOCGIFFLAGS, ptIfreq) < 0)
        {
            break;
        }
        ptTempTable = ptAddrTable + wIndex;
        if ((0 == (ptIfreq->ifr_flags & IFF_LOOPBACK)) && (ptIfreq->ifr_flags & IFF_UP))
        {
            TNetIfParam tNetIfParam;

            ptTempTable->dwLocalAddr = /*ntohl*/(dwGetIp);
            IPtoStr(ptTempTable->dwLocalAddr, achIp, IP_LEN);
            tNetIfParam.strNetAddr = achIp;
            if (ioctl(tSocketRef, SIOCGIFINDEX, ptIfreq) < 0)
            {
                break;
            }
            ptTempTable->dwInterface = ptIfreq->ifr_ifindex;
            IPtoStr(ptTempTable->dwInterface, achIp, IP_LEN);
            memset(achIp, 0, IP_LEN);
            sprintf(achIp, "%s", ptIfreq->ifr_ifrn.ifrn_name);
            tNetIfParam.strIfName = achIp;
            if (ioctl(tSocketRef, SIOCGIFNETMASK, ptIfreq) < 0)
            {
                break;
            }
            ptSockaddrIn = (struct sockaddr_in*)(&ptIfreq->ifr_netmask);
            ptTempTable->dwMask = /*ntohl*/(ptSockaddrIn->sin_addr.s_addr);
            IPtoStr(ptTempTable->dwMask, achIp, IP_LEN);
            tNetIfParam.strNetMask = achIp;

            if (TRUE == IsNicConnect(tNetIfParam.strIfName.c_str()))
            {
                tList.push_back(tNetIfParam);
            }
        }
    }

    delete[] ptAddrTable;
    ptAddrTable = NULL;
    SockClose(tSocketRef);    
#endif

    return true;
}

TNetIfParamList GetNetIfParamList()
{
    TNetIfParamList tNetIfParamList;
    GetAddrList(tNetIfParamList);
    return tNetIfParamList;
}
