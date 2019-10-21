//==========================================================================================
//  ģ����   : g800dll
//  �ļ���   : g800dll.h
//  ����ļ� : 
//  ʵ�ֹ��� : ��װ��G800��ͨѶ���Խӿڵķ�ʽ�ṩ���ϲ�Ӧ��
//  ����     : ����ƽ
//  ��Ȩ     : <Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
//-------------------------------------------------------------------------------------------
//�޸ļ�¼:
// �� ��      �汾    �޸���    �߶���   �޸ļ�¼
// 2013-4-17  1.0     ����ƽ             ����
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
	
	//ע��ʱ����д������ݵ��ַ���
	<desc></desc>
	</ kedacomxmldata >
*********************************************/


//��Ӧ��Ϣ�ص�
typedef void (*CBXmlCmdRsp)(const s8* szXmlCmdRsp);

//ƽ̨���ӣ�bConn����true��ƽ̨�Ͽ���bConn����false
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

//��ʼ����
G800DLLAPI BOOL32 InitG800dll();

//�첽�����ӳɹ�����true�����򷵻�false
G800DLLAPI BOOL32 ConnectG800(TG800ConnInfo tConnInfo);

//�Ͽ���G800������
G800DLLAPI void DisConnG800();

//�첽�����ͳɹ�����true�����򷵻�false
G800DLLAPI BOOL32 TransXmlCmd(const s8* szXmlCmd);

//���ٿ�
G800DLLAPI void QuitG800dll();

#ifdef __cplusplus
}
#endif

#endif //endif __G800_DLL_H__
