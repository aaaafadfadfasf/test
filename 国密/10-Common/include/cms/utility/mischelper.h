/*****************************************************************************
   模块名      : Helper
   文件名      : MISCHELPER.h
   相关文件    : 
   文件实现功能: 定义公共辅助函数和宏
   作者        : 黄至春
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2009/07/18  1.0         黄至春                     创建
   2012/12/27  1.0         胡志云                     修正ExecCmd等的实现和返回值说明
******************************************************************************/
#ifndef _MISCHELPER_H_
#define _MISCHELPER_H_

#include "helpercommon.h"
#include <string>
using std::string;

//_snprintf_s的参数与其他函数不一样，暂时注释
//#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
//// Microsoft visual studio, version 2005 and higher.
//#define SNPRINTF _snprintf_s
//#define SNSCANF  _snscanf_s
//#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define SNPRINTF _snprintf
#define SNSCANF  _snscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define SNPRINTF snprintf
#define SNSCANF  snscanf
#endif

#ifdef WIN32
    #define POPEN _popen
	#define PCLOSE _pclose
#else
    #define POPEN popen
	#define PCLOSE pclose
#endif

#define StrIsEmpty(lpszPath) (NULL == (lpszPath) || 0 == (lpszPath)[0]) 
char *KdmStrCpyN(char* s1, const char* s2, size_t n);
char* StrCpy_S(char* lpszDst, size_t nDstCnt, const char* lpszSrc);
char* StrTrimR(char* s, int ch = ' ');
char* StrTrimL(char* s, int ch = ' ');
char* StrTrimRL(char* s, int ch = ' ');

s8** AllocTable(u32 dwItemNum, u32 dwItemLen);
void FreeTable(s8** lpszTable, u32 dwItemNum );
/*===========================================================
函数名    ExecCmd
功能      ：执行Shell脚本命令
算法实现  ：
参数说明  ：
			[in]const char* lpszCmdFormat 格式化字符串
			[in]...,可变变量列表
返回值说明：int: -1 调用失败(通过errno判断具体错误), 其它 表示shell的返回值
-------------------------------------------------------------
修改记录  ：
日  期		版本		修改人		走读人        修改记录
2009/07/17  1.0     黄至春                    新建
2012/12/27  1.0     胡志云                    修改返回值说明  
============================================================*/
#define MAX_SHELL_CMD_LEN 1000
int ExecCmd(const char* lpszCmdFormat, ...);
int PGets(char* pBuf, size_t size, const char* lpszCmdFormat, ...);

int PExecCmd(CBStringOutput pfOutput, void* pUserArg, const char* lpszCmdFormat, ...);

// 功能：字符串替换（替换src中的str1为str2, 结果保存于dst）
void StrReplace(const s8 *src, const s8 *str1, const s8 *str2, s8 *dst);

// 功能：不区分大小写字符串比较, 返回true表示相等，否则表示不相等
bool Stricmp(const char * lpszStr1, const char * lpszStr2);

//格式化时间段描述
s8* GetTimeSpanDes(u32 dwTimeSpan);

//功能：修改文件所有者(仅Linux), 返回true表示成功，否则表示不成功
bool ChangeOwner(const char *lpszUserName, const char *lpszFilePath);

//功能：修改文件权限为nnn(使用类似777,520的形式)（仅Linux）,返回true表示成功，否则表示不成功
bool ChangeMod(u32 dwMod, const char *lpszFilePath);



/*===========================================================
函数名    TruncateUtf8String
功能      ：根据指定的字符个数截取Utf8字符串中第一段符合要求的子串
算法实现  ：
参数说明  ：
			[in][out]string& strUTF8  utf8格式字符串
			[in]u32 dwCharactersNum,utf8字符个数(若为0，则返回原字符串)
返回值说明：无
-------------------------------------------------------------
修改记录  ：
日  期		版本		修改人		走读人        修改记录
2016/03/01  1.0         刘永涛                    新建 
============================================================*/
void TruncateUtf8String(string& strUTF8, u32 dwCharactersNum);
bool IsUTF8Begin(u8 ch);

//功能：实现linux下的cp  Whui[2016/3/2], 0 表示成功
int cp(const char *from, const char *to);
#endif // _MISCHELPER_H_
