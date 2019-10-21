//==========================================================================================
//  模块名   : g800dll
//  文件名   : g800dll.h
//  相关文件 : 
//  实现功能 : 封装与G800的通讯，以接口的方式提供给上层应用
//  作者     : 李清平
//  版权     : <Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
//-------------------------------------------------------------------------------------------
//修改记录:
// 日 期      版本    修改人    走读人   修改记录
// 2013-4-17  1.0     李清平             创建
//===========================================================================================
#ifndef __G800_DLL_H__
#define __G800_DLL_H__

#include "kdvtype.h"

#ifdef WIN32
#ifdef G800DLL_EXPORTS
#define G800DLLAPI __declspec(dllexport)
#else
#define G800DLLAPI __declspec(dllimport)
#endif //endif G800DLL_EXPORTS
#else
#define G800DLLAPI 
#endif //endif WIN32

#ifdef __cplusplus
extern "C" {
#endif

/********************************************
	<?xml version="1.0" encoding="utf-8"?>
	<kedacomxmldata ver="v2.0"> 
	<command>conng800</command>
	<IP></IP>
	<Port></Port>
	
	//注册时，填写表明身份的字符串
	<desc></desc>
	</ kedacomxmldata >
*********************************************/


//响应消息回调
typedef void (*CBXmlCmdRsp)(const s8* szXmlCmdRsp);

//平台连接：bConn返回true，平台断开：bConn返回false
typedef void (*CBConnectG800Status)(BOOL32 bConn, u32 dwSessId);

#ifdef WIN32
#pragma pack(push, 1)
#endif

struct TG800ConnInfo 
{
	s8                    m_szConnInfo[256];
	CBXmlCmdRsp           m_pfXmlCmdRsp;
	CBConnectG800Status   m_pfConnStatus;
}
#ifndef WIN32
__attribute__ ((packed))
#endif
;

#ifdef WIN32
#pragma pack(pop)
#endif

//初始化库
G800DLLAPI BOOL32 InitG800dll();

//异步，连接成功返回true，否则返回false
G800DLLAPI BOOL32 ConnectG800(TG800ConnInfo tConnInfo);

//断开与G800的连接
G800DLLAPI void DisConnG800();

//异步，发送成功返回true，否则返回false
G800DLLAPI BOOL32 TransXmlCmd(const s8* szXmlCmd);

//销毁库
G800DLLAPI void QuitG800dll();

#ifdef __cplusplus
}
#endif

#endif //endif __G800_DLL_H__
