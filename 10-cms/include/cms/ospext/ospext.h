#ifndef _OSP_EXT_H_
#define _OSP_EXT_H_

/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospext.h
相关文件  ：osp.h ospexevt.h osplog.h osptool.h osptask.h ospinst.h ospapp.h
实现功能  ：扩展OSP的功能
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/21         V1.0              fanxg                                  新建文件
=========================================================================================*/

#include "cbb/osp/osp.h"
#include "cms/cms_eventid.h"
#include "cms/cms_const.h"
#include "cms/ospext/osplog.h"
#include "cms/ospext/ospdbg.h"
#include "cms/ospext/osptool.h"
#include "cms/ospext/osptask.h"
#include "cms/ospext/ospinst.h"
#include "cms/ospext/ospapp.h"

//信号处理回调函数
typedef void (*PfProcSignal)(int);

//读取kdsip重要的配置项
#define SECTION_OSPEXT         ( "OSPEXT" )            //OSPEXT配置项
#define KEY_WRITE_RUNLOG_LEV   ( "WriteRunLogLev" )    //输出运行日志文件及级别：0:不输出; 非0:输出指定级别
#define KEY_RUNLOG_FILENUM     ( "RunLogFileNum" )     //运行日志文件数：1-255
#define KEY_RUNLOG_FILESIZE    ( "RunLogFileSize" )    //运行日志文件大小：1M-1024M
#define KEY_ERRLOG_FILENUM     ( "ErrLogFileNum" )     //错误日志文件数：1-255
#define KEY_ERRLOG_FILESIZE    ( "ErrLogFileSize" )    //错误日志文件大小：1M-1024M

#define MIN_LOG_FILE_NUM   (1)       //最小日志文件数1
#define MAX_LOG_FILE_NUM   (255)     //最大日志文件数255
#define MIN_LOG_FILE_SIZE  (1)       //每个日志文件最小为1M
#define MAX_LOG_FILE_SIZE  (1024)    //每个日志文件最大为1024M

//文件日志默认参数
//runlog
#define RUN_LOG_DIR_NAME      "./runlog/"
#define RUN_LOG_FILE_SIZE_kb  (1*1024)     //每个运行日志文件最大为1M
#define RUN_LOG_FILE_NUM      (10)         //10文件循环覆盖

//errlog
#define ERR_LOG_DIR_NAME      "./errlog/"
#define ERR_LOG_FILE_SIZE_kb  (1*1024)    //每个错误日志文件最大为1M
#define ERR_LOG_FILE_NUM      (5)         //5文件循环覆盖

struct TOspExtConf 
{
    TOspExtConf()
    {
        dwMaxNodeNum = 32;  //设置成1会导致osp内部的心跳检测app(OspNodeMan)线程的消息队列太小，容易丢消息，这里适当放大一点
        dwMaxDispatchMsg = 1024;

        strRunLogDir = RUN_LOG_DIR_NAME;
        dwRunLogFileSize_kb = RUN_LOG_FILE_SIZE_kb;
        dwRunLogFileNum = RUN_LOG_FILE_NUM;

        strErrLogDir = ERR_LOG_DIR_NAME;
        dwErrLogFileSize_kb = ERR_LOG_FILE_SIZE_kb;
        dwErrLogFileNum = ERR_LOG_FILE_NUM;

		byRunLogFileLev=0;
    }

    string strModuleName;    //模块名，当进程中使用包含osp的dll时用于指定dll的telnet窗口。为空时直接取进程名。

    u32 dwMaxNodeNum;        //osp创建的最大Node数, 如果不使用osp通信，设置为1即可
    u32 dwMaxDispatchMsg;    //osp最大消息分发数,默认为1024

    string strRunLogDir;     //运行日志存放目录
    u32 dwRunLogFileSize_kb; //运行日志文件大小
    u32 dwRunLogFileNum;     //运行日志文件数目

    string strErrLogDir;     //错误日志存放目录
    u32 dwErrLogFileSize_kb; //错误日志文件大小
    u32 dwErrLogFileNum;     //错误日志文件数目

	// 增加runlog日志级别 [11/16/2015 pangubing]
	u8 byRunLogFileLev;

	/*pangubing  [11/16/2015 pangubing]
	  读取keda的ini格式的配置文件直接使用该接口
	 （注意）其他格式的配置文件，上层自行定义读取方法将配置信息读取到TOspExtConf中  */
	bool ReadKeyCfg(const s8* szConfPath);

	
};



/*====================================================================
函数名      : OspExtInit
功能        : OspExt初始化：
              1.osp初始化
              2.ospext日志文件初始化
              3.nipdebug初始化
算法实现    : 每个进程只需要调用一次
引用全局变量:
输入参数说明:
             dwMaxNodeNum : [in] osp创建的最大Node数, 如果不使用osp通信，设置为1即可
             dwMaxDispatchMsg : [in] osp最大消息分发数
返回值说明  : 成功:TRUE，失败:FALSE
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/02/20  0.1         fanxg
====================================================================*/
BOOL32 OspExtInit(const TOspExtConf& tOspExtConf = TOspExtConf());


/*====================================================================
函数名      : OspExtQuit
功能        : OspExt退出
算法实现    : 每个进程只需要调用一次
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/02/20  0.1         fanxg
====================================================================*/
void OspExtQuit();

/*====================================================================
函数名      : OspExtSetSignalProcCB
功能        : 设置信号处理回调
算法实现    : 
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/19  0.1         fanxg
====================================================================*/
void OspExtSetSignalProcCB(PfProcSignal pfSigProcCB);


#endif //#ifndef _OSP_EXT_H_