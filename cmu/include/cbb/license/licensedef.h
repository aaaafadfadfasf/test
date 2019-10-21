#ifndef __LICENSEDEF_H__
#define __LICENSEDEF_H__

/////////////////////////////////////////////////////////////
//	Linux 操作系统头文件
/////////////////////////////////////////////////////////////
#ifdef _LINUX_
#include <stdio.h>
#include <stdlib.h>
#endif //_LINUX_
// ///////////////////////////////////////////////////////////////////
// //	Win32 操作系统头文件
// ///////////////////////////////////////////////////////////////////
// #ifdef _MSC_VER
// #include <afx.h>
// #endif	// _MSC_VER

#include <string.h>
#include "kdvtype.h"

#ifndef LPCSTR
typedef const char *LPCSTR;
#endif

#define  KEY_MAX_CONTENT		4096	//license内容最大长度
#define  MAX_KEY_VALUE_NUM		100		// 可读取的Key-Value对最大数

// 错误码 ////////////////////////////////////////////////////////////////
#define SUCCESS_LICENSE						(u16)0                      // 成功
#define ERR_LICENSE_AUTH_FAILED				(u16)1                      // 授权码验证失败
#define ERR_LICENSE_FORMAT_FAILED			(u16)2                      // 信息格式错误
#define ERR_LICENSE_NO_VER_FAILED			(u16)3                      // 无版本信息
#define ERR_LICENSE_DEV_INFO_FAILED			(u16)4                      // 设备信息不全
#define ERR_LICENSE_ENCRYPT_FAILED			(u16)5                      // 加密失败
#define ERR_LICENSE_WRITEKEY_FAILED			(u16)6                      // 写license文件失败
#define ERR_LICENSE_SMALL_VALUE				(u16)7                      // 数值小于下限
#define ERR_LICENSE_BIG_VALUE				(u16)8                      // 数值大于上限
#define ERR_LICENSE_LONG_STRING				(u16)9                      // 字符串长度大于上限
#define ERR_LICENSE_NO_ITEM_ENUM			(u16)10                     // 没有此选项
#define ERR_LICENSE_OUT_DATE				(u16)11                     // 日期超出期限
#define ERR_LICENSE_NO_KEY					(u16)12                     // 不支持此KEY值
#define ERR_LICENSE_READ_CFG_FAIL			(u16)13                     // 读配置文件失败
#define ERR_LICENSE_INVALID_PARAM			(u16)14                     // 参数非法
#define ERR_LICENSE_NO_KEY_TYPE				(u16)15                     // 不支持此类型
#define ERR_LICENSE_NO_USB_KEY				(u16)16                     // 无USBKEY
#define ERR_LICENSE_USB_CONNECT_FAIL		(u16)17                     // 连接USB设备失败
#define ERR_LICENSE_USB_GET_FAIL			(u16)18                     // 独占USB设备失败
#define ERR_LICENSE_OPENFILE_FAILED			(u16)19                     // 打开文件错误
#define ERR_LICENSE_INVALID_USBKEY			(u16)20                     // License文件与USB设备不符
#define ERR_LICENSE_USB_FORMAT_FAIL			(u16)21                     // 格式化USB设备失败
#define ERR_LICENSE_USB_VERY_PASSWORD		(u16)22                     // 校验USB设备口令失败
#define ERR_LICENSE_USB_CREATE_FILE_FAIL	(u16)23                     // USB设备创建文件失败
#define ERR_LICENSE_USB_OPEN_FILE_FAIL		(u16)24                     // USB设备打开文件失败
#define ERR_LICENSE_USB_DELETE_FILE_FAIL	(u16)25                     // USB设备擦除文件失败
#define ERR_LICENSE_READKEY_FAILED			(u16)26                     // 读license文件失败
#define ERR_LICENSE_TOO_MANY_CHAR           (u16)27                     // 授权信息过长
#define ERR_LICENSE_DECRYPT_FAILED			(u16)28                      // 解密失败
#define ERR_LICENSE_VERIFY_SOPIN			(u16)29                      // 验证管理员密码错误
#define ERR_LICENSE_CHANGE_DIR				(u16)30                      // 切换目录出错
#define ERR_LICENSE_CREATE_CONTEXT			(u16)31                      // 创建上下文失败
#define ERR_LICENSE_CLOSE_FILE				(u16)32                      // 关闭文件失败
#define ERR_LICENSE_CREATE_MUTEX			(u16)33	                     // 创建锁失败
#define ERR_LICENSE_UNKNOWN					(u16)1000                   // 不明原因的错误
//////////////////////////////////////////////////////////////////////////

// 必须的KEY值 ///////////////////////////////////////////////////////////
#define KEY_LICENSE_FILE_PATH				(LPCSTR)"KEY_LICENSE_FILE_PATH"		// 写License文件路径
#define KEY_LICENSE_KEY						(LPCSTR)"KEY_LICENSE_KEY"			// 许可证编码(36位UUID)
//////////////////////////////////////////////////////////////////////////

typedef struct tagLMSKey_Value 
{
	tagLMSKey_Value()
	{
		memset(achKey, 0, sizeof(achKey));
		memset(achValue, 0, sizeof(achValue));
	}
	s8 achKey[1024];
	s8 achValue[1024];
}TLMSKeyValue;

enum emLMSKeyType
{
	em_LMSNoKey = 0,
	em_LMSHaiKey,
	em_LMSFTKey,
	em_LMSSLKey,
};

typedef struct tagLMSUsbkeySN
{
	tagLMSUsbkeySN()
	{
		memset(achSN, 0, sizeof(achSN));
		byType = em_LMSNoKey;// 未知
	}
	s8 achSN[128];
	u8 byType;	//0--hkey,1--ft
}TLMSUsbkeySN;

#endif  //__LICENSEDEF_H__