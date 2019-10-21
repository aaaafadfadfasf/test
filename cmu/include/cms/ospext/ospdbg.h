/*========================================================================================
模块名    ：ospext.lib
文件名    ：ospdbg.h
相关文件  ：osp.h
实现功能  ：ospext调试控制
作者      ：fanxg
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/04/21         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _OSP_DBG_H_INCLUDED_
#define _OSP_DBG_H_INCLUDED_

#include "cbb/osp/osp.h"
#include <string>
using std::string;

#define OSP_EXT_VER "ospext 1.0 Compile Time: %s, %s\r\n", __DATE__, __TIME__

//上条调试命令
API void u();

//下条调试命令
API void d();

//打印所有缓存调试命令
API void a();

//ospex 帮助命令
API void OspExtHelp();
API void ospexthelp();

//ospex 版本信息
API void OspExtVer();
API void ospextver();

//获取日志级别的字符串描述
const string GetStrLogLev(u8 byLogLev);

//显示日志控制参数
API void OspShowLogParam();

//设置指定AppId-InstId的打印级别
API void OspSetLogLev(u8 byLogLev, u16 wAppId, u16 wInstId = 0);

//设置指定模块的打印级别,如果使用模块式打印控制，必须首先设定模块的打印级别
API void OspSetModLogLev(u8 byLogMod, u8 byLogLev, LPCSTR szModName = NULL);
#define OSP_SET_MOD_LOG(LogMod, LogLev) OspSetModLogLev(LogMod, LogLev, #LogMod)

//设置ospext模块日志级别
API void ospextlog(u8 byLogLev);

//设置日志输出时间：//0:不打印时间; 1:打印秒级时间; 255:打印豪秒级时间
API void OspSetLogTimeLev(u8 byLogLev);

//设置日志输出编译信息(file-line-class-function)：//0:不打印; 1:打印
API void OspLogCompileInfo(u8 byPrint);

//设置日志输出OSP前缀(app-inst-task-state)：//0:不打印; 1:打印
API void OspLogOspPrefix(u8 byPrint);

//根据AppId-InstId打印事务信息
API void OspPrintTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL);

//根据AppId-InstId打印轮询事务信息
API void OspPrintPollTask(u16 wAppId, u16 wInstId=0, LPCSTR szTaskType=NULL);

//根据AppId-InstId-TaskNO打印事务信息
API void OspPrintOneTask(u32 dwTaskNO, u16 wAppId, u16 wInstId =0);

//根据AppId-InstId-TaskNO终止指定事务
API void OspKillTask(u32 dwTaskNO, u16 wAppId, u16 wInstId =0);

//trace指定AppId-InstId输出打印
API void OspTraceInst(u16 wInstId, u16 wAppId);

//trace指定事务输出打印
API void OspTraceTask(u32 dwTaskNO);

//trace下一个新建事务输出打印
API void OspTraceNextTask();

//打印指定AppId的所有Inst状态
API void OspInstState(u16 wAppId);

//是否输出运行日志文件：//0:不输出; 非0:输出
API void OspWriteRunLog(u8 byIsWriteLog);

//是否输出错误日志文件：//0:不输出; 非0:输出
API void OspWriteErrLog(u8 byIsWriteLog);

//设置错误日志文件数：1-255
API void OspSetErrLogFileNum(u32 dwErrLogFileNum);

//设置错误日志文件大小：1M-1024M
API void OspSetErrLogFileSize(u32 dwErrLogFileSize);

//设置运行日志文件数：1-255
API void OspSetRunLogFileNum(u32 dwRunLogFileNum);

//设置运行日志文件大小：1M-1024M
API void OspSetRunLogFileSize(u32 dwRunLogFileSize);

//显示日志邮箱信息：
API void OspShowLogQue();

#ifdef _LINUX_
//linux是否进行mangle name的解析
API void OspSetParseMangleName(u8 byIsParse);
#endif

//是否开启时间性能检测功能
API void OspSetTimeMonitor(u8 byOpen);


#endif  //#ifndef _OSP_DBG_H_INCLUDED_
