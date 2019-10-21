/*========================================================================================
ģ����    ��ospext.lib
�ļ���    ��osplog.h
����ļ�  ��osp.h typeinfo xstring.h
ʵ�ֹ���  ����־������Ϳ���
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/05/21         V1.0              fanxg                                  �½��ļ�
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
//WIN32�²�ֱ��֧�ֺ�__FUNCTION__����˵����ͨ��WIN32����չwindbg��ʵ�֣�
//����Ȥ��WIN32������Ա����ȥʵ�֣�Ŀǰֻ�Ƕ���һ��Ĭ��ֵ��
//LINUX��ֱ��֧�ָú�.
//vs2010֧�ָú꣬����__FUNCTION__ = "class::MemFun"
  #ifndef __FUNCTION__
    #define __FUNCTION__ "win32Fun"
  #endif

//��ȡ����
  #ifndef __CLASS__
    #define __CLASS__  ""
  #endif

#endif

#ifdef _LINUX_

  //��ȡ����
  #ifndef __CLASS__
    #define __CLASS__  (typeid(*this).name())
  #endif

#endif

//log�Ƿ�֧�����������Ϣ
API bool IsPrintCI();

#define  FILE_NAME  (IsPrintCI() ? __FILE__ : NULL)

#define  FILE_LINE  (IsPrintCI() ? __LINE__ : -1)

#define  TYPE_NAME  (IsPrintCI() ? __CLASS__ : NULL)

#define  FUNC_NAME  (IsPrintCI() ? __FUNCTION__ : NULL)

//��ȡ����ַ���
#define GETVLISTSTR(Format, Output) \
            va_list pvlist;\
			memset(&pvlist, 0, sizeof(pvlist));\
            va_start(pvlist, Format);\
            int nLen = vsnprintf((char *)Output,sizeof(Output) -1, Format, pvlist);\
            if( nLen <= 0 || nLen >= (int)sizeof(Output) ) Output[sizeof(Output)-1] = 0;\
            va_end(pvlist);

//����ͨ�õ�LOG����
enum
{
    NOLOG_LEV = 0,    //���κδ�ӡ
    ERROR_LEV,        //���󼶱�
    WARNING_LEV,      //���漶��
    CRITICAL_LEV,     //�ؼ�����
    EVENT_LEV,        //��ͨ����
    PROGRESS_LEV,     //���ȼ���
    TIMER_LEV,        //��ʱ����
    ALL_LEV,          //���м���

    //
    LOG_LEV_NUM,

    TRIVIAL_LEV = 255,      //����Ƶ���ļ���
};

//���幫����ģ�飬������־���
enum
{
    MOD_ALL = 0,       //����ģ��
    OSPEXT,            //ospext
    OSPSIP,            //ospsip
    DBIX,              //dbix

    COMMON_NUM = 100,  //100������ģ���㹻

    USER_MODULE_START, //�ϲ�ģ���ɴ˿�ʼ

    MODULE_MAX = 255,  //��������255��ģ����־���
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

//��־�ṹ
struct TOspExtLogContent 
{
    TOspExtLogContent()
    {
        byLogLev = NOLOG_LEV;
        bIsPrintScreen = TRUE;
    }

    string strBodyField;       //��־����
    string strModLev;          //��־ģ�鼶��
    string strTimeField;       //��־ʱ��
    string strOspPrifixField;  //ospǰ׺
    string strCompileField;    //������Ϣ
    u8     byLogLev;           //��־����
    BOOL32 bIsPrintScreen;     //�Ƿ���Ļ���
}; 

//��Ļ��־

//���������Ϣ(file-line-class-function)��LOG
class CXLog
{
public:

    CXLog(const char* szFileName=NULL, int nFileLine=-1, const char* szClassName=NULL, const char* szFunName=NULL);

    void operator()(unsigned char byLogLev, const char* szFormat, ...) const;

    //����ģ����ƵĴ�ӡ
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

//������ʹ��
#define CLASSLOG (CXLog(FILE_NAME, FILE_LINE, TYPE_NAME, FUNC_NAME))

//ȫ��ʹ�û���ľ�̬��Ա������ʹ��
#define GLOBELOG (CXLog(FILE_NAME, FILE_LINE, NULL, FUNC_NAME))

//�󶨳�Ա����LOG
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


//OspExtLogData ���ڴ�ӡ���ݣ���������־������Ϣ
void OspExtLogData(u8 byLogLevel, const char* szFormat, ...);

#endif //#ifndef _OSP_LOG_H_INCLUDED_
