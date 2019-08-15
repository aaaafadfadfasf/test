/*****************************************************************************
   模块名      : Helper
   文件名      : HELPERCOMMON.h
   相关文件    : 
   文件实现功能: 定义公共辅助函数和宏
   作者        : 黄至春
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2009/07/18  1.0         黄至春                     创建
******************************************************************************/
#ifndef _HELPERCOMMON_H_
#define _HELPERCOMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cbb/osp/osp.h"

#ifndef LOG_HELP_CRITICAL
#define LOG_HELP_CRITICAL	0
#endif

#ifndef LOG_HELP_ERROR
#define LOG_HELP_ERROR		1
#endif

#ifndef LOG_HELP_WARNING
#define LOG_HELP_WARNING	2
#endif

#ifndef LOG_HELP_INFO
#define LOG_HELP_INFO		3
#endif

#ifndef LOG_HELP_DEBUG
#define LOG_HELP_DEBUG		4
#endif

extern unsigned char g_byHelperLogLevel;
extern char* g_lpszHelperLogLevel;
API void sethelperlog(unsigned char byLevel);
void HelperLog(unsigned char byLevel, const char * pInfo, ...);


/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
#undef ASSERT
#define ASSERT(exp)    \
{ \
    if ( !( exp ) ) \
{ \
    printf((s8*)"Assert File %s, Line %d (%s)\n", \
    __FILE__, __LINE__, (s8*)#exp ); \
} \
}
#else
#undef ASSERT
#define ASSERT( exp )    {}
#endif

#define ASSERT_RET_VOID(assertVal) \
	if(!(assertVal))\
{\
	ASSERT(FALSE);\
	return;\
}

#define ASSERT_RET(assertVal, retVal) \
	if(!(assertVal))\
{\
	ASSERT(FALSE);\
	return (retVal);\
}

#ifndef ARRAY_LEN
#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef GET_SHELL_RETURN
#ifdef _LINUX_ 
#define GET_SHELL_RETURN(ret) ( WIFEXITED(ret) ) ? WEXITSTATUS(ret) : -1
#else
#define GET_SHELL_RETURN(ret) 0
#endif
#endif

typedef void (*CBStringOutput)(s8* lpszBuf, void* pUserArg);


#define KDM_MIN(x,y) ((x)<(y)?(x):(y))
#define KDM_MAX(x,y) ((x)>(y)?(x):(y))

#endif // _HELPERCOMMON_H_
