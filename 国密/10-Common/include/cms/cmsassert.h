/********************************************************************
	注释日期:	2010/07/29
	author:		张应能
	
	purpose:	实现 CMS_ASSERT 与 CMS_ASSERT_WITH_MSG
                CMS_ASSERT 与 ASSERT 功效一致
                CMS_ASSERT_WITH_MSG , 允许多加一个提示参数,参数中可填写提示.
                当 OspOpenLogFile 了的时候,会将 Assertion 写入log文件中.
                这些断言在 Debug 版本有效,定义了 ALPHA_RELEASE 的 Release 版本也有效.
                在 Release 版本无效.
                在 UNIT_TEST 版本无效,自动化单元测试时通常要测试函数的强壮性.

                _CMS_ASSERT_WITH_MSG_IMPL 是内部实现,外部不要调用.
    modify:     liangli
                为了防止用户在exp语句中直接写具有操作含义的表达式, 导致在Release版本中没有执行操作
                修改了CMS_ASSERT的定义, 执行exp;
                lizhixing 2012.2.17
                原先的断言触发后直接exit(1)退出程序，导致无法看到错误信息，甚至让程序员定位不到错误原因
                所以修改为throw方式，对windows和linux下同样有效
                不过，此版本不能自动退出程序后拉起新进程，所以非release版本严禁对外正式发布
    版权        : Copyright(C) 2003-2009 Suzhou Keda Technology Co., Ltd. All rights reserved
*********************************************************************/

#ifndef CMS_ASSERT_H
#define CMS_ASSERT_H

#include <cassert>
//#include "osp.h"
#include "kdvtype.h"
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#include <WinSock2.h>
#include <Windows.h>
#include <time.h>

// 用于支持对类名函数名的打印。
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif

#endif

#ifdef _LINUX_
#include <unistd.h>
#include <string.h>
#endif

/*====================================================================
函数名：TaskDelay
功能：任务延时
算法实现：（可选项）
引用全局变量：
输入参数说明：uMs: 时间间隔(ms)

返回值说明：
====================================================================*/
inline void TaskDelay(int dwMs)
{
#ifdef WIN32
    Sleep(dwMs);
#endif

#ifdef _LINUX_
    usleep(dwMs*1000);
#endif
}

/** 写Assert原因文件日志。 */
inline void DumpAssertMsg( const char* exp, const char* pFileName, int lineNum, const char* funName, const char* szMsg )
{
	std::stringstream ssAssert;
	ssAssert << "Assertion: " << exp 
		<< " , Fun: " << funName
		<< " , File: " << pFileName
		<< " , Line: " << lineNum
		<< " , Msg: " << szMsg
		<< std::endl;
	
	// 打印到命令行。
	std::cout << ssAssert.str();
	// 打印到错误输出。
	std::cerr << ssAssert.str();
	// 写到断言文件。
	std::stringstream ssAssertFileName;
	tm localT;
	memset( &localT, 0, sizeof( localT ) );
	time_t now = time( 0 );
#ifdef _LINUX_
    localtime_r(&now, &localT);
#else
    tm *pLocalT = ::localtime( &now );
    if ( pLocalT )
    {
        localT = *pLocalT;
    }
#endif
		
	ssAssertFileName << "Assertion_" << localT.tm_year + 1900 << localT.tm_mon + 1 << 
		localT.tm_mday << localT.tm_hour << localT.tm_min << localT.tm_sec
		<< ".txt";
	std::ofstream fAssert( ssAssertFileName.str().c_str() );
	fAssert << ssAssert.str() << std::endl;
	fAssert.close();
}

//// _CMS_ASSERT_WITH_MSG_IMPL 内部实现,外部勿调用.
// 使用 throw 抛出异常的方式, 可以通过信号将 gdb或windows调试器引导至代码. 
// Windows调试时在点击 "继续" 后程序仍能继续进行. linux下程序将退出..
// TaskDelay 用来等待其它异步的日志打印或写日志文件完成。
#define _CMS_ASSERT_WITH_MSG_IMPL(exp,msg) do{\
            if(!(exp)){\
			DumpAssertMsg( #exp , __FILE__, __LINE__, __FUNCTION__, static_cast<const char*>(msg) );\
			TaskDelay( 100 );\
			throw( msg );\
            }\
        }while(0)

//// Release 版本与 Alpha Release 版本.
#ifdef NDEBUG    //release version

    #ifdef ALPHA_RELEASE ////enable assert in Alpha Release Version

        #define CMS_ASSERT_WITH_MSG _CMS_ASSERT_WITH_MSG_IMPL

        #define CMS_ASSERT(exp) do{\
            CMS_ASSERT_WITH_MSG(exp,"");\
        }while(0)
        
    #else    ////disable assert in Release Version

        #define CMS_ASSERT_WITH_MSG(exp,msg) do {  \
                                            } while(0)
        #define CMS_ASSERT(exp)     do {  \
                                    } while(0)
        
    #endif

#else 
////debug version
#define CMS_ASSERT_WITH_MSG _CMS_ASSERT_WITH_MSG_IMPL

#define CMS_ASSERT(exp) do{\
            CMS_ASSERT_WITH_MSG(exp,"");\
        }while(0)

#endif 

////disable assert in automatic unittest
#ifdef UNIT_TEST
    #undef CMS_ASSERT_WITH_MSG
    #define CMS_ASSERT_WITH_MSG(exp,msg)     do { exp; \
                                            } while(0)

    #undef CMS_ASSERT
    #define CMS_ASSERT(exp)     do { exp; \
                                } while(0)
#endif

#endif

/////////////////////////////////////// END OF FILE /////////////////////////////////////
