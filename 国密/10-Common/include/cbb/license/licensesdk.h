#ifndef __LICENSE_SDK_H__
#define __LICENSE_SDK_H__

#include "kdvtype.h"
#include <memory.h>
#include "licensedef.h"

#ifdef _LINUX_
#define SDK_API extern "C"
#else
#ifdef _WIN32
#define SDK_API extern "C" __declspec(dllexport)
#else
#define SDK_API extern "C"
#endif
#endif

#ifdef _LINUX_
#ifndef stdcall
#define stdcall
#endif
#else
#ifndef stdcall
#define stdcall _stdcall
#endif
#endif

#define IP_LEN					14
#define NAME_LEN			    40      //用户名长度
#define MAX_PATH				260

//错误码相关定义
enum SDK_ERRCODE
{
    SDK_NO_ERR = 0,			//无错误
	SDK_PARAM_ERR,			//参数错误
	SDK_INIT_ERR,			//初始化组件错误
	SDK_CONNECT_ERR,		//连接服务失败
	SDK_TIMEOUT,			//超时错误
	SDK_UNKOWN,				//未知错误
	SDK_OPRKEY_ERR,			//操作key错误
	SDK_NO_FUNCTION,		//暂时无此功能
//	SDK_APPLY_OK,			//申请License成功
	SDK_APPLY_UUID_ERR,		//申请License时，随机码UUID被占用，需要调用reset接口 
	SDK_APPLY_LICENSEOVER,	//服务器上已经没有此license
};

enum SDK_MESG_TYPE
{
	MESG_DISSCONNECT,		//与服务器断链
	MESG_STOP_AUTHORIZE,	//授权被服务器终止
};

/*========================================================================
函 数 名：pfEventCB
功    能：SDK回调函数
参    数：dwMsg：消息类型定义 详细请参考SDK_MESG_TYPE
		  pchParam：<param><InitHandle>0</InitHandle></param>
		  pUserData: 用户数据
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
typedef void (stdcall *pfEventCB)(s32 dwMsg, s8* pchParam, void *pUser);

typedef struct tagServerInfo
{				
	s8	m_achIp[IP_LEN];		//license服务器IP	    [必填]
    u16	m_wPort;				//license服务器port		[必填]
	s8	m_achUser[NAME_LEN];	//license服务器用户名   [待定]
	s8	m_achPwd[NAME_LEN];		//license服务器密码     [待定]	
	s8	m_achCfgPath[MAX_PATH]; //存储设备信息Xml文件路径 *格式如"./test/"	[必填]
	s8	m_achDevName[NAME_LEN];	//设备别名	[可不填]
	s8  m_achDevId[NAME_LEN];   //设备ID,可为mac地址或设备唯一Id *格式如"000C294CC1FA" [必填]
	u32	m_dwCheckCount;			//检测dwCheckCount次无响应认为断链	[必填]	
	u32	m_dwCheckTime;			//每次检测间隔时间为dwCheckTime		[必填]
	pfEventCB	m_pfEvent;		//事件回调函数
	void*		m_pUserData;	//用户参数

	tagServerInfo()
	{
		memset(this,0,sizeof(tagServerInfo));
	}
}LICENSE_SERVER_INFO;

typedef struct tagReq_Param
{
	s8	m_achModel[NAME_LEN];	//产品名称
	s8	m_achKeyId[NAME_LEN];	//要申请key id不填则服务器自动分配
	u16	m_dwApplylicNum;		//申请License中授权个数，针对同一个license给多个业务授权时，设置为2500
    u32	m_dwFreeTime;			//设置失效时间,sdk检测到断链后，会过dwFreeTime秒通知业务回收license。
	
	tagReq_Param()
	{
		memset(this,0,sizeof(tagReq_Param));
	}
}REQ_PARAM;


/*========================================================================
函 数 名：SDK_Init
功    能：SDK始化接口
参    数：LICENSE_SERVER_INFO &tServerInfo	[in]license服务器相关信息
		  u32 *InitHandle					[out]返回初始化成功handle,
		  
返 回 值：SDK_ERRCODE 错误码
说	  明：初始化相关资源，做一次连接、不做注册相关操作
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API SDK_ERRCODE SDK_Init(LICENSE_SERVER_INFO &tServerInfo, u32 *InitHandle);

/*========================================================================
函 数 名：SDK_Quit
功    能：反初始化SDK接口(不释放之前授权)
参    数：u32 InitHandle					[in]初始化成功handle
		  
返 回 值：SDK_ERRCODE 错误码
说	  明：
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API SDK_ERRCODE SDK_Quit(u32 InitHandle);

/*========================================================================
函 数 名：SDK_ApplyLicense
功    能：申请授权
参    数：u32 InitHandle					[in]初始化成功handle
		  REQ_PARAM &tReqParam				[in]释放授权参数
		  s8*  pchParam						[in/out]服务器返回数据 使用时请分配1024字节
		  s32 &nApplyNum					[out]申请到的license数量

返 回 值：SDK_ERRCODE 错误码
说	  明：该接口调用成功后，可通过SDK_GetValue获取对应键值
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API SDK_ERRCODE SDK_ApplyLicense(u32 InitHandle, REQ_PARAM  &tReqParam, s32 &nApplyNum, s8*  pchParam = 0);




SDK_API SDK_ERRCODE SDK_ReleaseLicense(u32 InitHandle, REQ_PARAM &tReqParam, s8*  pchParam = 0);

/*========================================================================
函 数 名：SDK_SetUserData
功    能：设置用户自定义数据
参    数：u32 InitHandle					[in]初始化成功handle
		  s8 *pchUserdata					[in]用户数据
		  s32 &nValueLen					[in]用户数据有效长度
返 回 值：SDK_ERRCODE 错误码
说	  明：向服务器发送自定义数据，服务器帮助保存
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API SDK_ERRCODE SDK_SetUserData(u32 InitHandle, s8 * pchUserdata, s32 nValueLen);

/*========================================================================
函 数 名：SDK_GetUserData
功    能：获取用户自定义数据
参    数：u32 InitHandle					[in]初始化成功handle
		  s8 *pchUserdata					[out]用户数据
		  s32 &nValueLen					[in/out]用户数据有效长度
返 回 值：SDK_ERRCODE 错误码
说	  明：向服务器获取之前保存的自定义数据
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API SDK_ERRCODE SDK_GetUserData(u32 InitHandle, s8 * pchUserdata, s32 &nValueLen);

//读文件key接口
/*========================================================================
函 数 名：SDK_GetKeyValueNum
功    能：获取License文件中有多少Key－value值对
参    数：u32 InitHandle				[in]初始化成功handle,为0则直接读取内存中数据
返 回 值：Key－value值对数
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_GetKeyValueNum(u32 InitHandle = NULL);

/*========================================================================
函 数 名：SDK_EnumKeyList
功    能：枚举License文件中有多少Key值
参    数：TKeyValue *pchKeyValueList	[out]Key-Value值列表
		  s32 &nListLen					[in/out]Key-Value值列表空间大小/Key-Value值数
		  u32 InitHandle				[in]初始化成功handle,为0则直接读取内存中数据
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_EnumKeyList(TLMSKeyValue *pchKeyValueList, s32 &nListLen, u32 InitHandle = NULL);

/*========================================================================
函 数 名：LMS_GetValue
功    能：根据Key值获取相应的值
参    数：s8 *pchKey					[in]Key值
		  s8 *pchValue					[out]值
		  s32 &nValueLen				[in/out]值有效长度
		  u32 InitHandle				[in]初始化成功handle,为0则直接读取内存中数据
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_GetValue(s8 *pchKey, s8 *pchValue, s32 &nValueLen, u32 InitHandle = NULL);

//以下是兼容之前读key操作
/*========================================================================
函 数 名：SDK_ReadLicenseFile
功    能：读取License文件
参    数：s8 *pchPath					[in]License文件名(全路径名－请以'\0'结尾)
		  BOOL32 bCheck					[in]是否校验License合法性(默认校验)
		  
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_ReadLicenseFile(s8 *pchPath, BOOL32 bCheck = TRUE);

/*========================================================================
函 数 名：SDK_WriteLicenseFile
功    能：写License文件
参    数：BOOL32 bToUsb = FALSE             [in]    是否写入USB设备
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_WriteLicenseFile(BOOL32 bToUsb = FALSE);

/*=============================================================================
函 数 名: SDK_SetValue
功    能: 向内存设置数据
参    数: LPCSTR lpszKey                    [in]    段名
          LPCSTR lpszValue                  [in]    键值
注    意: 仅刷新读取出的数据内存
返 回 值: 错误码
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2015/06/25	1.0		唐柳	创建
=============================================================================*/
SDK_API u16 SDK_SetValue(LPCSTR lpszKey, LPCSTR lpszValue);

/*========================================================================
函 数 名：LMS_GetEncryptMachineNO
功    能：获取加密后的机器码（仅对linux系统有效，windows下返回为FALSE）

参    数：s8 *pchEncryptMachineNO		[out]加密后机器码		  
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API BOOL32 SDK_GetEncryptMachineNO(s8 *pchEncryptMachineNO);

/*========================================================================
函 数 名：LMS_GetErrorCodeDescription
功    能：获取调用操作key产生的错误码的描述
参    数：u16 wErrorCode				[in]错误码
		  s8 *pchDescription			[out]错误码描述
		  s32 &nLen						[in/out]错误码描述有效长度
		  s8 *pchIniFilePath			[in]错误码描述文件全路径(包括文件名)，请以'\0'结尾
返 回 值：BOOL32 TRUE : 成功   FALSE : 失败
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API BOOL32 SDK_GetErrorCodeDescription(u16 wErrorCode, s8 *pchDescription, s32 &nLen, s8 *pchIniFilePath);

/*=============================================================================
函 数 名: LMS_IsUsbKeyOnline
功    能: 判断是否有USBKey连接
参    数: void
注    意: 仅刷新读取出的数据内存
返 回 值: BOOL32 TRUE : 有   FALSE : 没有
-------------------------------------------------------------------------------
修改纪录:
日      期  版本    修改人  修改内容
2015/06/25	1.0		唐柳	创建
=============================================================================*/
SDK_API BOOL32 SDK_IsUsbKeyOnline(void);

/*========================================================================
函 数 名：LMS_GetUsbkeyNum
功    能：获取当前PC插入usbkey个数，枚举Usbkey序列号
参    数：TUsbkeySN *ptUsbKeySNList		[out]序列号列表
		  s32 &nLen						[in/out]usbkey个数
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_GetUsbkeyNum(TLMSUsbkeySN *ptUsbKeySNList, s32 &nLen);

/*========================================================================
函 数 名：LMS_ReadLicenseFileBySN
功    能：通过usbkey序列号读取Usbkey
参    数：TUsbkeySN tUsbKeySN			[in]序列号
返 回 值：u16 错误码
--------------------------------------------------------------------------
修改记录：
日		期	版本	修改人	修改内容
2015/06/25	1.0		唐柳	创建
========================================================================*/
SDK_API u16 SDK_ReadLicenseFileBySN(TLMSUsbkeySN tUsbKeySN);

#endif  // endif __LICENSE_SDK_H__