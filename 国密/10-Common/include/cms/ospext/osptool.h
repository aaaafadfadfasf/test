/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��osptool.h
����ļ�  ��osp.h xstring.h
ʵ�ֹ���  ������OSP��һЩ������ͺ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/04/21         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _OSP_TOOL_H_INCLUDED_
#define _OSP_TOOL_H_INCLUDED_

#include "cbb/osp/osp.h"
#include "cms/container/xmap.h"
#include "cms/cmsassert.h"
#include <vector>
using std::vector;

#ifndef UNREFERENCED_PARAMETER   //pclint:715
#define UNREFERENCED_PARAMETER(P)          (sizeof(P)!=0)
#endif

//��ЧOSP�¼�
#define INVALID_OSP_EVENT  (0)

//OSP ��Ϣ����
#define OSP_ADD_EVENT_DESC(T) OspExtAddEventDescAndID(#T, T)

u16          OspExtEventID(const char* szEventDesc);
const string OspExtEventDesc( u16 wEventID );
void         OspExtAddEventDescAndID(const char * desc, u16 event);

//������--->������
template <class type> type NetToHost(type nVal);

template <> inline u_short NetToHost(u_short nVal)
{
    return ntohs(nVal);
}

template <> inline u_long NetToHost(u_long nVal)
{
    return ntohl(nVal);
}

//������--->������
template <class type> type HostToNet(type nVal);

template <> inline u_short HostToNet(u_short nVal)
{
    return htons(nVal);
}

template <> inline u_long HostToNet(u_long nVal)
{
    return htonl(nVal);
}

//ʱ��
struct TMsTime
{
    TMsTime()
    {
        tSecond = 0;
        tMilliSecond = 0;
    }

    time_t tSecond;
    u16 tMilliSecond;
};

//��ȡ��ǰmsʱ��
TMsTime OspGetMsTime();

//�������ʱ��������1193��Сʱ��u32���޷����棬�ú��������
//ҵ�����Ӧ��ʹ�øú�����ȡ��ʱ���ڵĺ����
u32 OspGetDiffMs(const TMsTime& tEndTime, const TMsTime& tStartTime);

//����ϵͳticks����ȡ�뼶ʱ�䣬��������ʱ���Ĳ�����Ϊ�޸���ϵͳʱ��������쳣
u64 OspGetSecByTicks();

class COspTime
{
public:
    COspTime();
    COspTime(time_t tTime);

public:
    COspTime(const COspTime& tObj);
    COspTime& operator = (const COspTime& tObj);

    //��ȡ��ǰʱ��
    static COspTime GetCurrTime();

    //��ȡ��ǰ�ַ�����ʽʱ��(��ȷ����)
    static string GetCurrStrTime();

    //��ȡ��ǰ�ַ�����ʽʱ��(��ȷ������)
    static string GetCurrStrTime_ms();

    //ʱ������
    void Clear();

    //����ʱ��
    void SetTime( const time_t *ptTime );

    //�õ�ʱ��ṹ
    void GetTime( time_t &tTime ) const;

    //�õ�ʱ��ṹ
    time_t GetTime(void) const;

    //ʱ��Ƚ�
    BOOL32 operator == ( const COspTime &tObj ) const;

    //��ȡʱ���Ӧ�ĸ�ʽ�ִ�
    BOOL32 GetString(s8* pachString, s32 nStringLen) const;

    //��ȡʱ���Ӧ�ĸ�ʽ�ִ�
    string GetString() const;

private:
    u16  m_wYear;   //��
    u8   m_byMonth; //��
    u8   m_byMDay;  //��
    u8   m_byHour;  //ʱ
    u8   m_byMinute;//��
    u8   m_bySecond;//��
    u16  m_wMillSec;//����
};

//��Ԫ�ź���
#ifndef INFINITE
    #define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

class CXSemLock
{
public:
    CXSemLock();
    ~CXSemLock();

private:
    CXSemLock(const CXSemLock&);
    void operator=(const CXSemLock&);

public:
    //����
	BOOL32 Lock(u32 dwTimeout);

	//����
	BOOL32 Lock();

    //����
    BOOL32 UnLock();

    //��ͬ��Lock(), ������ʹ��ϰ��
    BOOL32 Take(u32 dwTimeout);

	//��ͬ��Lock(), ������ʹ��ϰ��
	BOOL32 Take();

    //��ͬ��UnLock(), ������ʹ��ϰ��
    BOOL32 Give();

private:
    SEMHANDLE m_hSemaphore;
};

//�ź���
#define MAX_SEMAPHORE_COUNT 100000
class CXSemaphore
{		
public:		
    CXSemaphore(u32 dwInitCnt = 0, u32 dwMaxCnt = MAX_SEMAPHORE_COUNT);
    ~CXSemaphore();

private:    
    CXSemaphore(const CXSemaphore&);
    void operator=(const CXSemaphore&);

public:
    BOOL32 Take(u32 dwTimeout);
	BOOL32 Take( );
    BOOL32 Give();

private:
    SEMHANDLE m_hSemaphore;
};

//�Զ������򻯼�������������
class CAutoLock
{
public:
    //���캯������
    CAutoLock(CXSemLock& rLock);

    //������������
    ~CAutoLock();
private:
	CAutoLock& operator=(const CAutoLock&);
private:
    CXSemLock& m_Lock;
};


//����-�ַ���ӳ��
class CEventDescMap
{
public:
    void AddEventDesc (const char *  desc, u16 event);
    u16 GetEventID(const char* szEventDesc);
    const string GetEventDesc( u16 wEventID );
private:
    CStrMap<u16> m_cEventDescToIDMap;
    CXMap<u16, string> m_cIDToEventDescMap;
    CXSemLock m_cEventMapLock;
};

/*====================================================================
������      : GenSequenceNum
����        : ����һ����Ϣ��ˮ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/10/22/             liangli
====================================================================*/
const u32 GenSequenceNum();

//�ļ��������
/*====================================================================
������      : GetProcessName
����        : ��ȡ��ǰ��������(������·��)
�㷨ʵ��    : 
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : ��ǰ��������
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessName();

/*====================================================================
������      : GetProcessPath
����        : ��ȡ��ǰ����·��(����������)
�㷨ʵ��    : 
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : ��ǰ����·��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessPath();

/*====================================================================
������      : GetProcessFullPath
����        : ��ȡ��ǰ��������·������������
�㷨ʵ��    : 
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : ��ǰ��������·��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessFullPath();

//�ַ����Ͷ�����Խӿ�
using std::wstring;

typedef string CCharset;
#define  ICONV_CHARSET_GBK       "GBK"
#define  ICONV_CHARSET_UTF8      "UTF-8"
#define  ICONV_CHARSET_UNICODE   "UCS2"
#define  ICONV_CHARSET_CP932      "CP932"    //����
#define  ICONV_CHARSET_GB2312     "GB2312"    //����
#define  ICONV_CHARSET_CP949      "CP949"    //����
#define  ICONV_CHARSET_CP1254     "CP1254"  //��������

#ifdef   _LINUX_
#include <iconv.h>

string CharsetConvert(const CCharset& cFrom, const CCharset& cTo, const string& strIn);
#endif

wstring MbsToUcs(const CCharset& cFrom, const string& strIn);
string UcsToMbs(const CCharset& cTo, const wstring& wstrIn);

wstring Utf8ToUcs(const string& strIn);
string UcsToUtf8(const wstring& wstrIn);

string Utf8ToMbs(const CCharset& cTo, const string& strIn);
string MbsToUtf8(const CCharset& cFrom, const string& strIn);


string GBKToUTF8(const string& strGBK);
string UTF8ToGBK(const string& strUTF8);
string DotNETNameToLinuxCharSet(const string& DotNETName);

wstring UTF8ToUnicode(const string& strUTF8);
string UnicodeToUTF8(const wstring& wstrSrc);

wstring GBKToUnicode(const string& strGBK);
string UnicodeToGBK(const wstring& wstrSrc);

//��ȡ������Ϣ
struct TNetIfParam
{
    string   strIfName;             
    string   strNetAddr;             
    string   strNetMask;  
};
typedef vector<TNetIfParam> TNetIfParamList;

TNetIfParamList GetNetIfParamList();








#endif  //#ifndef _OSP_TOOL_H_INCLUDED_