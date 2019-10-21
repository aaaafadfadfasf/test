/*========================================================================================
模块名    ：ospext.lib
文件名    ：osptool.h
相关文件  ：osp.h xstring.h
实现功能  ：基于OSP的一些工具类和函数
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/21         V1.0              fanxg                                  新建文件
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

//无效OSP事件
#define INVALID_OSP_EVENT  (0)

//OSP 消息描述
#define OSP_ADD_EVENT_DESC(T) OspExtAddEventDescAndID(#T, T)

u16          OspExtEventID(const char* szEventDesc);
const string OspExtEventDesc( u16 wEventID );
void         OspExtAddEventDescAndID(const char * desc, u16 event);

//网络序--->主机序
template <class type> type NetToHost(type nVal);

template <> inline u_short NetToHost(u_short nVal)
{
    return ntohs(nVal);
}

template <> inline u_long NetToHost(u_long nVal)
{
    return ntohl(nVal);
}

//主机序--->网络序
template <class type> type HostToNet(type nVal);

template <> inline u_short HostToNet(u_short nVal)
{
    return htons(nVal);
}

template <> inline u_long HostToNet(u_long nVal)
{
    return htonl(nVal);
}

//时间
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

//获取当前ms时间
TMsTime OspGetMsTime();

//如果两个时间间隔超过1193个小时，u32将无法保存，该函数会溢出
//业务程序应该使用该函数获取短时间内的毫秒差
u32 OspGetDiffMs(const TMsTime& tEndTime, const TMsTime& tStartTime);

//根据系统ticks来获取秒级时间，避免依赖时间差的操作因为修改了系统时间而出现异常
u64 OspGetSecByTicks();

class COspTime
{
public:
    COspTime();
    COspTime(time_t tTime);

public:
    COspTime(const COspTime& tObj);
    COspTime& operator = (const COspTime& tObj);

    //获取当前时间
    static COspTime GetCurrTime();

    //获取当前字符串格式时间(精确到秒)
    static string GetCurrStrTime();

    //获取当前字符串格式时间(精确到豪秒)
    static string GetCurrStrTime_ms();

    //时间清零
    void Clear();

    //设置时间
    void SetTime( const time_t *ptTime );

    //得到时间结构
    void GetTime( time_t &tTime ) const;

    //得到时间结构
    time_t GetTime(void) const;

    //时间比较
    BOOL32 operator == ( const COspTime &tObj ) const;

    //获取时间对应的格式字串
    BOOL32 GetString(s8* pachString, s32 nStringLen) const;

    //获取时间对应的格式字串
    string GetString() const;

private:
    u16  m_wYear;   //年
    u8   m_byMonth; //月
    u8   m_byMDay;  //日
    u8   m_byHour;  //时
    u8   m_byMinute;//分
    u8   m_bySecond;//秒
    u16  m_wMillSec;//毫秒
};

//二元信号锁
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
    //上锁
	BOOL32 Lock(u32 dwTimeout);

	//上锁
	BOOL32 Lock();

    //解锁
    BOOL32 UnLock();

    //等同于Lock(), 看个人使用习惯
    BOOL32 Take(u32 dwTimeout);

	//等同于Lock(), 看个人使用习惯
	BOOL32 Take();

    //等同于UnLock(), 看个人使用习惯
    BOOL32 Give();

private:
    SEMHANDLE m_hSemaphore;
};

//信号量
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

//自动锁，简化加锁、解锁调用
class CAutoLock
{
public:
    //构造函数上锁
    CAutoLock(CXSemLock& rLock);

    //析构函数解锁
    ~CAutoLock();
private:
	CAutoLock& operator=(const CAutoLock&);
private:
    CXSemLock& m_Lock;
};


//整数-字符串映射
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
函数名      : GenSequenceNum
功能        : 生成一个消息流水号
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/10/22/             liangli
====================================================================*/
const u32 GenSequenceNum();

//文件操作相关
/*====================================================================
函数名      : GetProcessName
功能        : 获取当前进程名称(不包含路径)
算法实现    : 
引用全局变量:
输入参数说明:
返回值说明  : 当前进程名称
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessName();

/*====================================================================
函数名      : GetProcessPath
功能        : 获取当前进程路径(不包含名称)
算法实现    : 
引用全局变量:
输入参数说明:
返回值说明  : 当前进程路径
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessPath();

/*====================================================================
函数名      : GetProcessFullPath
功能        : 获取当前进程完整路径，包含名称
算法实现    : 
引用全局变量:
输入参数说明:
返回值说明  : 当前进程完整路径
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/02/20  0.1         fanxg
====================================================================*/
string GetProcessFullPath();

//字符集和多国语言接口
using std::wstring;

typedef string CCharset;
#define  ICONV_CHARSET_GBK       "GBK"
#define  ICONV_CHARSET_UTF8      "UTF-8"
#define  ICONV_CHARSET_UNICODE   "UCS2"
#define  ICONV_CHARSET_CP932      "CP932"    //日语
#define  ICONV_CHARSET_GB2312     "GB2312"    //汉语
#define  ICONV_CHARSET_CP949      "CP949"    //韩语
#define  ICONV_CHARSET_CP1254     "CP1254"  //土耳其语

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

//获取网络信息
struct TNetIfParam
{
    string   strIfName;             
    string   strNetAddr;             
    string   strNetMask;  
};
typedef vector<TNetIfParam> TNetIfParamList;

TNetIfParamList GetNetIfParamList();








#endif  //#ifndef _OSP_TOOL_H_INCLUDED_