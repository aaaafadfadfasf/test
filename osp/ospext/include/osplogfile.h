/*========================================================================================
模块名    ：ospext.lib
文件名    ：osplogfile.h
相关文件  ：osp.h osptool.h
实现功能  ：写日志文件
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/03/01         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_LOG_FILE_H_INCLUDED_
#define _OSP_LOG_FILE_H_INCLUDED_

#include "cbb/osp/osp.h"
#include "cms/ospext/osptool.h"
#include <string>
using std::string;

#define DEFAULT_LOG_DIR_NAME       ("./cmslog/")
#define DEFAULT_LOG_FILE_SIZE_kb   (1024)   //1024*1kb = 1M
#define DEFAULT_LOG_FILE_NUM       (10)
#define DEFAULT_LOG_SUFFIX         (".log")

enum EOspExtLogFileType
{
    LOG_FILE_NONE = 0,      //错误日志
    LOG_FILE_ERR = 1,       //错误日志
    LOG_FILE_RUN = 1<<1,    //运行日志
    //LOG_FILE_XXX = 1<<2,        //XXXX日志

};

/*====================================================================
函数名      : OspExtOpenLogFile
功能        : 开启ospext日志文件功能
算法实现    : 日志可以分类型
引用全局变量:
输入参数说明:
             strLogDir: 日志文件的输出目录，格式: "./log/" 或 "/opt/kdm/cmu/log/"
             nLogSizeMax_kb: 日志文件的容量，单位：kb
             nLogNumMax: 日志文件的最大数目
             dwLogType: 日志文件类型: LOG_FILE_ERR 或 LOG_FILE_RUN
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/03/01  0.1         fanxg
====================================================================*/
bool OspExtOpenLogFile(const string& strLogDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType);

class CXLogFile
{
public:
    CXLogFile();
    ~CXLogFile();

private:
    CXLogFile(const CXLogFile&);  
    void operator=(const CXLogFile&);
public: 
    //以0.1.2.3.4.5...的方式为日志文件命名
    void WriteLogFile(const string& strLog);

    //以日志创建时间为日志文件命名
    void WriteLogFileNew(const string& strLog);

    //设置日志文件参数
    void SetLogFileParam(const string& strDir, u32 nLogSizeMax_kb, u32 nLogNumMax, u32 dwLogType);

    //获取进程名和当前时间组成的文件戳
    static string GetFileStamp();

    //从文件名获取后缀名，可以没有
    static string GetSuffixFromFile(const string& strFileName);

private:
    string GetCurFile() const;
    string GetFirstFile() const;
    string GetLastFile() const;
    u32 GetFileNum() const;
    string GetNewFile() const;
    string GetSuffixName() const;
public:
	u32 GetLogSizeMax() const { return m_nLogSizeMax; }
	void SetLogSizeMax(u32 val) { m_nLogSizeMax = val; }
	u32 GetLogNumMax() const { return m_nLogNumMax; }
	void SetLogNumMax(u32 val) { m_nLogNumMax = val; }
private:
    u32 m_dwLogType;     //日志类型
    string m_strLogDir;  //日志目录
    string m_strLogName; //日志文件初始名称
    u32 m_nLogSizeMax;   //日志文件最大值：单位：kb
	u32 m_nLogNumMax;    //日志文件数目
	u32 m_nCurLogIdx;    //当前日志文件索引
    string m_strCurFile; //当前可写文件
    FILE* m_pFile;       //当前打开的文件句柄，进程运行中文件一直打开，并不每次写日志都开文件和关文件
    CXSemLock m_cLock;   //可能多线程写文件，加锁保护
};

struct TLogData
{
    TLogData()
    {
        dwLogType = 0;
    }

    string strLog;
    u32    dwLogType;
};

//写日志文件接口
void WriteLogFile(TLogData* ptLogData);

//显示日志邮箱信息
void LogQuePrint();

//写日志线程
void* WriteLogFileThread(void* pArg);



#endif //#ifndef _OSP_LOG_FILE_H_INCLUDED_
