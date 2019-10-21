/*=======================================================================
模块名      :
文件名      : readlicense.h
相关文件    :
文件实现功能 : 读取license文件的内容
作者        : 邵云东
版本        : KCMV1.0  Copyright(C) 2003-2008 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2007/01/09  KCM1.0      邵云东      创建
2007/04/24  KCM1.0      周雷        新增精锐4S锁
=======================================================================*/
#ifndef _READLICENSE_H_
#define _READLICENSE_H_

//#include "osp.h"

///////////////////////////////////////////////////////////////////
//	Linux 操作系统头文件
///////////////////////////////////////////////////////////////////
#ifdef _LINUX_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <assert.h>
	#include <stdarg.h>
	#include <malloc.h>
	#include <errno.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <semaphore.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <termios.h>
	#include <signal.h>	
	#include "syslog.h"

#endif //_LINUX_


///////////////////////////////////////////////////////////////////
//	Win32 操作系统头文件
///////////////////////////////////////////////////////////////////
#ifdef _MSC_VER

#include <malloc.h>

	#ifndef _WINDOWS_
		#include <afx.h>
		#include <afxwin.h>
		#include <winsock2.h>
	#else
		#include <windows.h>
		#include <winsock2.h>
	#endif

	#include <assert.h>
	#pragma comment(lib,"Ws2_32.lib")

#endif	// _MSC_VER
#include "kdvsys.h"

#define  KDC_CALL	extern "C"

typedef struct tagKey_Value 
{
	tagKey_Value()
	{
		memset(achKey, 0, sizeof(achKey));
		memset(achValue, 0, sizeof(achValue));
	}
	s8 achKey[1024];
	s8 achValue[1024];
}TKeyValue;

enum emKeyType
{
	em_NoKey = 0,
	em_HaiKey,
	em_FTKey,
	em_SLKey,
	em_SL_S4Key
};

typedef struct tagUsbkeySN
{
	tagUsbkeySN()
	{
		memset(achSN, 0, sizeof(achSN));
		byType = em_NoKey;// 未知
	}
	s8 achSN[128];
	u8 byType;	//0--hkey,1--ft
}TUsbkeySN;

#define MAX_KEY_VALUE_NUM		100 // 可读取的Key-Value对最大数

/*========================================================================
函 数 名：GenkeyValueList
功    能：从一段buff生成key-value到全局对象
参    数：s8 *pachContentBuf			[in]一段License buffer
		  u16 wBufLen					[in]buffer长度
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
u16 GenkeyValueList(s8 *pachContentBuf, u16 wBufLen);

/*========================================================================
函 数 名：ShowElemts
功    能：遍历所有key-value 键值对(输出到控制台)
参    数：void
返 回 值：void
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
void ShowElemts();

/*========================================================================
函 数 名：ReadLicenseFile
功    能：读取License文件
参    数：s8 *pchPath					[in]License文件名(全路径名－请以'\0'结尾)
		  
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 ReadLicenseFile(s8 *pchPath);

/*========================================================================
函 数 名：WriteLicenseFile
功    能：写License文件
参    数：BOOL32 bToUsb = FALSE             [in]    是否写入USB设备
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
2007/06/12  1.0     王昊    允许写入USB设备
========================================================================*/
KDC_CALL u16 WriteLicenseFile(BOOL32 bToUsb = FALSE);

/*========================================================================
函 数 名：GetKeyValueNum
功    能：获取License文件中有多少Key－value值对
参    数：
返 回 值：Key－value值对数
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/23	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 GetKeyValueNum();
/*========================================================================
函 数 名：EnumKeyList
功    能：枚举License文件中有多少Key值
参    数：TKeyValue *pchKeyValueList	[out]Key-Value值列表
		  s32 &nListLen					[in/out]Key-Value值列表空间大小/Key-Value值数
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 EnumKeyList(TKeyValue *pchKeyValueList, s32 &nListLen);
/*========================================================================
函 数 名：GetValue
功    能：根据Key值获取相应的值
参    数：s8 *pchKey					[in]Key值
		  s8 *pchValue					[out]值
		  s32 &nValueLen				[in/out]值有效长度
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL u16 GetValue(s8 *pchKey, s8 *pchValue, s32 &nValueLen);
/*========================================================================
函 数 名：GetErrorCodeDescription
功    能：获取调用产生的错误码的描述
参    数：u16 wErrorCode				[in]错误码
		  s8 *pchDescription			[out]错误码描述
		  s32 &nLen						[in/out]错误码描述有效长度
		  s8 *pchIniFilePath			[in]错误码描述文件全路径(包括文件名)，请以'\0'结尾
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/01/09	KCM1.0	邵云东	创建
========================================================================*/
KDC_CALL BOOL32 GetErrorCodeDescription(u16 wErrorCode, s8 *pchDescription, s32 &nLen, s8 *pchIniFilePath);

/*=============================================================================
函 数 名: SetValue
功    能: 向内存设置数据
参    数: LPCSTR lpszKey                    [in]    段名
          LPCSTR lpszValue                  [in]    键值
注    意: 仅刷新读取出的数据内存
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2007/06/12  1.0     王昊    创建
=============================================================================*/
KDC_CALL u16 SetValue(LPCSTR lpszKey, LPCSTR lpszValue);

/*=============================================================================
函 数 名: IsUsbKeyOnline
功    能: 判断是否有USBKey连接(仅支持海泰和飞天诚信key)
参    数: void
注    意: 仅刷新读取出的数据内存
返 回 值: BOOL32
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2008/12/11  1.0     韩旺    创建
=============================================================================*/
KDC_CALL BOOL32 IsUsbKeyOnline(void);

/*========================================================================
函 数 名：GetEncryptMachineNO
功    能：获取加密后的机器码（仅对linux系统有效，windows下返回为FALSE）

参    数：s8 *pchEncryptMachineNO		[out]加密后机器码		  
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2009/10/27	1.0		李庚	创建
========================================================================*/
KDC_CALL BOOL32 GetEncryptMachineNO(s8 *pchEncryptMachineNO);

/*========================================================================
函 数 名：GetUsbkeyNum
功    能：获取当前PC插入usbkey个数，枚举Usbkey序列号
参    数：TUsbkeySN *ptUsbKeySNList		[out]序列号列表
		  s32 &nLen						[in/out]usbkey个数
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/11/24	KDM5.0	李庚	创建
========================================================================*/
KDC_CALL u16 GetUsbkeyNum(TUsbkeySN *ptUsbKeySNList, s32 &nLen);

/*========================================================================
函 数 名：ReadLicenseFileBySN
功    能：通过usbkey序列号读取Usbkey
参    数：TUsbkeySN tUsbKeySN			[in]序列号
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/11/24	KDM5.0	李庚	创建
========================================================================*/
KDC_CALL u16 ReadLicenseFileBySN(TUsbkeySN tUsbKeySN);

/*========================================================================
函 数 名：WriteLicenseFileBySn
功    能：写内容到指定序列号的usbkey
参    数：TUsbkeySN tUsbKeySN			[in]序列号
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2007/11/24	KDM5.0	李庚	创建
========================================================================*/
KDC_CALL u16 WriteLicenseFileBySn(TUsbkeySN tUsbKeySN);

/*========================================================================
函 数 名：GetSLDevno
功    能：获取senselock锁的厂商编号(8 bytes 16进制格式)
参    数：s8 *pchBuf					[out]厂商编号
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL BOOL32 GetSLDevno(s8 *pchBuf); 

/*========================================================================
函 数 名：GetSLDeviceSN
功    能：获取senselock锁的序列号(20 bytes 字符串)
参    数：char *snStr					[out]序列号
		  unsigned long snLen			[in]序列号长度
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL BOOL32 GetSLDeviceSN(char *snStr, unsigned long snLen );

/*========================================================================
函 数 名：ReadLicenseFileContent
功    能：读取lisence的内容至Content内存
参    数：s8 *pchPath					[in]License文件名(全路径名－请以'\0'结尾)
		  s8* pchContent				[out]传出License到内存中
		  u16 &wContentLen				[in/out]Content有效长度
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL u16 ReadLicenseFileContent(s8 *pchPath, s8* pchContent, u16 &wContentLen);

/*========================================================================
函 数 名：GetkeyValueListByContent
功    能：从Content中获取键值对
参    数：s8* pchContent					[in]ReadLicenseFileContent传出的Content
		  u16 wContentLen					[in]Content有效长度
		  TKeyValue *pchKeyValueList		[out]序列号列表
		  s32 &nListSize					[in/out]Key-Value值列表空间大小/Key-Value值数
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL u16 GetkeyValueListByContent(s8* pchContent, u16 wContentLen, TKeyValue *pchKeyValueList, s32 &nListSize);

/*========================================================================
函 数 名：WriteLicenseFileContent
功    能：把lisence内容写到lisence文件中
参    数：s8* pchContent					[in]lisence内容
		  u16 wContentLen					[in]Content有效长度
		  BOOL32 bToUSB = FALSE				[in]是否写入到Usb设备
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL u16 WriteLicenseFileContent(s8* pchContent, u16 wContentLen, BOOL32 bToUSB = FALSE);

/*========================================================================
函 数 名：EraseKeyValue
功    能：把内存里Value值清空
参    数：void
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2016/10/29	1.0		周雷	创建
========================================================================*/
KDC_CALL u16 EraseKeyValue(void);


#endif // _READLICENSE_H_

