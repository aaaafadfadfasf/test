#ifndef _LOG_SYSTEM_H_
#define _LOG_SYSTEM_H_
#include "osp.h"
#include <string>
using std::string;
#include "ospext/osplog.h"

/* 日志子系统 */
enum ENLogSrc  // 日志来源枚举
{
    enLogSrcAll = MOD_ALL, 
    enLogSrcSystem = USER_MODULE_START+1, // 系统级别
    enLogSrcNMSConn,    // nms模块
    enLogSrcDbTaskPool, // DbTaskPool模块
    enLogSrcCCtrl,      // CCtrl模块
    enLogSrcBackground, // Background模块
    enLogSrcLicense,    // license模块
    enLogSrcSyncAnalyzer// SyncAnalyzer模块
};

API void logpara();
API void logall(u32 dwLogLevel); // 设置所有模块的日志级别
API void logoff();               // 关闭所有模块的日志级别

API void taslog(ENLogSrc enSrc, u32 dwLogLevel); // 设置模块日志级别

API void tla(); // 设置最大日志级别

API void tle(); // 设置错误日志级别

API void cctrllog(u32 dwLevel);
API void dblog(u32 dwLevel);
API void nmslog(u32 dwLevel);
API void licenselog(u32 dwLevel);

#endif // _LOG_SYSTEM_H_
