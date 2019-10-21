/*========================================================================================
模块名    ：ospext.lib
文件名    ：osplog.h
相关文件  ：osp.h typeinfo xstring.h
实现功能  ：日志的输出和控制
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/05/21         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_LOG_H_INCLUDED_
#define _OSP_LOG_H_INCLUDED_

#ifdef _DEBUG
  #ifndef DEBUG
    #define DEBUG
  #endif
#endif

#include "cbb/osp/osp.h"
#include <typeinfo>
#include <string>
using std::string;

#define MAX_LOG_BUF_LEN    4096

#ifdef _MSC_VER
//WIN32下不直接支持宏__FUNCTION__，据说可以通过WIN32的扩展windbg库实现，
//有兴趣的WIN32开发人员可以去实现，目前只是定义一个默认值。
//LINUX下直接支持该宏.
//vs2010支持该宏，并且__FUNCTION__ = "class::MemFun"
  #ifndef __FUNCTION__
    #define __FUNCTION__ "win32Fun"
  #endif

//获取类名
  #ifndef __CLASS__
    #define __CLASS__  ""
  #endif

#endif

#ifdef _LINUX_

  //获取类名
  #ifndef __CLASS__
    #define __CLASS__  (typeid(*this).name())
  #endif

#endif

//log是否支持输出编译信息
API bool IsPrintCI();

#define  FILE_NAME  (IsPrintCI() ? __FILE__ : NULL)

#define  FILE_LINE  (IsPrintCI() ? __LINE__ : -1)

#define  TYPE_NAME  (IsPrintCI() ? __CLASS__ : NULL)

#define  FUNC_NAME  (IsPrintCI() ? __FUNCTION__ : NULL)

//获取变参字符串
#define GETVLISTSTR(Format, Output) \
            va_list pvlist;\
			memset(&pvlist, 0, sizeof(pvlist));\
            va_start(pvlist, Format);\
            int nLen = vsnprintf((char *)Output,sizeof(Output) -1, Format, pvlist);\
            if( nLen <= 0 || nLen >= (int)sizeof(Output) ) Output[sizeof(Output)-1] = 0;\
            va_end(pvlist);

//定义通用的LOG级别
enum
{
    NOLOG_LEV = 0,    //无任何打印
    ERROR_LEV,        //错误级别
    WARNING_LEV,      //警告级别
    CRITICAL_LEV,     //关键级别
    EVENT_LEV,        //普通级别
    PROGRESS_LEV,     //进度级别
    TIMER_LEV,        //定时级别
    ALL_LEV,          //所有级别

    //
    LOG_LEV_NUM,

    TRIVIAL_LEV = 255,      //极其频繁的级别
};

//定义公共库模块，用于日志输出
enum
{
    MOD_ALL = 0,       //所有模块
    OSPEXT,            //ospext
    OSPSIP,            //ospsip
    DBIX,              //dbix

    COMMON_NUM = 100,  //100个公共模块足够

    USER_MODULE_START, //上层模块由此开始

    MODULE_MAX = 255,  //最大可设置255个模块日志输出
};

struct TLogModInfo
{
    TLogModInfo()
    {
        dwLogLev = NOLOG_LEV;
        strModName = "unknwon";
    }
    u32 dwLogLev;
    string strModName;
};

//日志结构
struct TOspExtLogContent 
{
    TOspExtLogContent()
    {
        byLogLev = NOLOG_LEV;
        bIsPrintScreen = TRUE;
    }

    string strBodyField;       //日志正文
    string strModLev;          //日志模块级别
    string strTimeField;       //日志时间
    string strOspPrifixField;  //osp前缀
    string strCompileField;    //编译信息
    u8     byLogLev;           //日志级别
    BOOL32 bIsPrintScreen;     //是否屏幕输出
}; 

//屏幕日志

//输出编译信息(file-line-class-function)的LOG
class CXLog
{
public:

    CXLog(const char* szFileName=NULL, int nFileLine=-1, const char* szClassName=NULL, const char* szFunName=NULL);

    void operator()(unsigned char byLogLev, const char* szFormat, ...) const;

    //进行模块控制的打印
    void operator()(unsigned char byLogMod, unsigned char byLogLev, const char* szFormat, ...) const;

protected:
    string GetCompileInfo() const;
    string AppendCompileInfo(const char* szLog) const;
    static string GetFileNameFromPath(const char* szPath);
    static string GetTypeName(const string& szTypeMangleName);

private:
    const char* m_szFileName;
    int m_nFileLine;
    const char* m_szClassName;
    const char* m_szFunName;
};

//在类中使用
#define CLASSLOG (CXLog(FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

//全局使用或类的静态成员函数中使用
#define GLOBELOG (CXLog(FILE_NAME, FILE_LINE, NULL, FUNC_NAME))

//绑定成员函数LOG
template <class T>
class CXMemFunLog : public CXLog
{
public:

    CXMemFunLog(
          const T* const pObj,
          const char* szFileName,
          int nFileLine,
          const char* szClassName,
          const char* szFunName
          )
          :CXLog(szFileName, nFileLine, szClassName, szFunName),
          m_pObj(pObj)
    {

    }

    void operator()(unsigned char byLogLev, const char* szFormat, ...)
    {
        char szMsg[MAX_LOG_BUF_LEN] = {0};

        GETVLISTSTR(szFormat, szMsg);

        string strLogFull = AppendCompileInfo(szMsg);

        if (m_pObj != NULL)
        {
            m_pObj->Log(byLogLev, "%s", strLogFull.c_str());
        }
        else
        {
            OspPrintf(TRUE, FALSE, "CXMemFunLog error: m_pObj = 0 || m_pmfLog = 0 [%s]\n", strLogFull.c_str());
        }
    }

    void operator()(unsigned char byLogMod, unsigned char byLogLev, const char* szFormat, ...)
    {
        char szMsg[MAX_LOG_BUF_LEN] = {0};

        GETVLISTSTR(szFormat, szMsg);

        string strLogFull = AppendCompileInfo(szMsg);

        if (m_pObj != NULL)
        {
            m_pObj->Log(byLogMod, byLogLev, "%s", strLogFull.c_str());
        }
        else
        {
            OspPrintf(TRUE, FALSE, "CXMemFunLog error: m_pObj = 0 || m_pmfLog = 0 [%s]\n", strLogFull.c_str());
        }
    }

private:
    const T* const m_pObj;
};


//OspExtLogData 用于打印数据，不会有日志附加信息
void OspExtLogData(u8 byLogLevel, const char* szFormat, ...);

#endif //#ifndef _OSP_LOG_H_INCLUDED_
