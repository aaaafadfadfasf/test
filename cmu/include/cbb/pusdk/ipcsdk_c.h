/*****************************************************************************
ģ����  : ipcv7sdk���ܽӿ�
�ļ���  : ipcv7sdk.h
����ļ���ipcv7sdk.cpp
ʵ�ֹ��ܣ�
����    �����ǿ�
�汾    ��IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����
20141111      v1.0        ���ǿ�                  ����
******************************************************************************/
#ifndef IPCSDK_H_
#define IPCSDK_H_


#include "ipcdefine.h"
#include "ipcstruct.h"


#ifdef __cplusplus
extern "C" {



#ifdef WIN32
	#define IPC_API  extern "C" __declspec(dllexport)

#ifndef CDECL
	#define CDECL  __cdecl
#endif
#else
	#define IPC_API
	#define CDECL
#endif

#endif // __cplusplus


/*=================================================================
��������: IPC_InitDll
��    ��: ��ʼ��SDK����Դ
          ȫ�ֵ���һ�μ��ɣ���Ӧ�ó���ʼ������
		  (MFC������winapp�е��ã���̬�⹤���ڿ���ش�����)
����˵��:
			pzDLLName  ��ʼ���Ķ�̬������(��ipcsdk.dll,ipcmedia.dll)
			wTelnetPort      Telnet�˿�(Ĭ��3000)
			bOpenTelnet     �Ƿ���Telnet(Ĭ�ϴ�)
			pErrorCode   	������
����ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_InitDll (char* pzDLLName = "ipcsdk.dll", short wTelnetPort=3300, bool bOpenTelnet= 0, u32* pErrorCode = NULL);

/*=================================================================
��������: IPC_ReleaseDll
��    ��: �ͷ�SDK�������Դ
          ��Ӧ�ó����˳�ʱ����
����˵��:pErrorCode   	������
����ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ReleaseDll(u32* pErrorCode);


/*=================================================================
��������: IPC_Login
��    ��: �û�ע�ᣬע����Ϣ��ip�󶨣��޸���ip����Ҫ���µ�¼
          
����˵��:
		  pHandle   ǰ�˾��
		  pName		�û���
		  pPassword �û�����
		  pErrorCode   	������
����ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32  CDECL IPC_Login(void *pHandle,char* pName, char* pPassword, u32* pErrorCode);

/*=================================================================
��������: IPC_Logout
��    ��: �û�ע��ע��
          
����˵��:
		  pHandle       ǰ�˾��
		  pErrorCode   	������
����ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Logout(void *pHandle, u32* pErrorCode);

/*=================================================================
��������: IPC_CreateHandle
��    ��: �������
          
����˵��:
		 dwIp      ע��IP
		 wPort     ע��˿�
		 pName	   �û���
		 pPassword �û�����

�� �� ֵ: �ɹ������豸���, ʧ�ܷ��ش�����
=================================================================*/
IPC_API u32 CDECL IPC_CreateHandle(u32 dwIP, u16 wPort, char* pName, char* pPassword);
/*=================================================================
��������: IPC_GetInfoFromHandle
��    ��: ���ݾ����ȡ�豸��Ϣ
          
����˵��:
		 dwHandle      �豸���
		 pParam		   ��������ָ��
		 nLen		   ���ݳ���
		 pErrorCode    ������ָ��

�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetInfoFromHandle(u32 dwHandle, void* pParam, int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_DestroyHandle()
��    ��: ���پ��
          
����˵��:
		 

�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_DestroyHandle();
/*=================================================================
��������: IPC_GetSoftVersion
��    ��: ��ȡ����汾��
����˵��:
          szVersion [out]    ����汾��

�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVersion(char *pchVer,int nMaxLen);

/*=================================================================
��������: IPC_GetCap
��    ��: ��ȡ������
����˵��: nCap -- ����������
          apCapName -- ������������
		  anCapOut -- �������ķ���ֵ
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCap(void *pHandle, u16 nCap, char* apCapName[], u32 anCapOut[],u32* pErrorCode);



/*=================================================================
��������: IPC_GetRecSchedule
		  ��    ��: ��ȡ����ʱ�����
����˵��: pIndex, ����������alarmin,warningline��,����ڵ��
		  pParam, ��������detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],����ʱ������ָ��.
	      nlenTime, ����ʱ�����ݸ���.
	      nLenTimeStruct, ����sizeof(TEVENTTIME)�����ں�������.
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�<eventtype opt=�� detect, shieldalarm, alarmin, alarmout, exception, recchedule, sightchange, warningline��/>
	      <recordtype opt=�� alarm,manual,time,trigger��/>
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetEventTime (void *pHandle, char *pEventType, char *pIndex,void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetRecSchedule
		  ��    ��: ���ò���ʱ�����
����˵��: pIndex, ����������alarmin,warningline��,����ڵ��
		  pParam, ��������detect, shieldalarm, alarmin, alarmout, exception, recchedule, warningline
	      pTimeArr, TEVENTTIME [],����ʱ������ָ��.
	      nlenTime, ����ʱ�����ݸ���.
	      nLenTimeStruct, ����sizeof(TEVENTTIME)�����ں�������.
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetEventTime (void *pHandle, char *pEventType,char *pIndex, void * pTimeArr, int nLenTime ,int nLenTimeStruct,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetTrancparent
		  ��    ��: ʹ��͸����ʽͨ�ţ����ݺͻش��Ĳ���Ϊxml�Ľṹ����
����˵��: pHandle		ǰ�˾��
		  szEventType	�¼�����
	      szParamIn		����Ĳ���
	      nLenIn		����Ĳ�������
	      szParamOut	����Ĳ���
	      nLenOut		����Ĳ�������
	      pErrorCode   	������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SendTransData(void *pHandle, char* szEventType, char* szParamIn, int nLenIn, char* szParamOut, int& nLenOut,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetDeviceInfo
��    ��: ��ȡ�豸������Ϣ
����˵��: TDEVICEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDeviceInfo(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetDeviceInfo
��    ��: �����豸������Ϣ
����˵��: TDEVICEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDeviceInfo (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetSystemTime
��    ��: ��ȡʱ��
����˵��: TSYSTIMEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSystemTime(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSystemTime
��    ��: ����ʱ��
����˵��: TSYSTIMEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSystemTime(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSysTimeSynChronise
��    ��: ����ʱ��ͬ��
����˵��: TSYSTIMEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSysTimeSynChronise(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetTimeAuto
��    ��: �����Զ�Уʱ
����˵��: TTIMEAUTOINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetTimeAuto(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetTimeAuto
��    ��: ��ȡ�Զ�Уʱ
����˵��: TTIMEAUTOINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetTimeAuto(void *pHandle, void* pParam, int& nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetDST
��    ��: ��������ʱ
����˵��: TTIMEAUTOINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetDST
��    ��: ��ȡ����ʱ
����˵��: TTIMEAUTOINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDST(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetRSInfo
��    ��: ��ȡ������Ϣ
����˵��: pParam, PTRSINFO
�˴����ݣ�<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRSInfo(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetRSInfo
����:���ô���ֹͣ
����˵��: pParam, PTRSINFO
�˴����ݣ�<addressnum enable="false"><controlprotocol opt="pelcol"  enable="false">
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRSInfo(void *pHandle, void* ptParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_LedFind
����:�򿪹ر�led��
����˵��: pParam, PTLEDINFO;
szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
�˴����ݣ�<speed opt="5s,10s,30s,1m,30m,60m,24h"/>
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_LedFind(void *pHandle, void* ptParam, int nLen, char* szParamAssist, int nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SystemReboot
����:ϵͳ����
����˵��: 
		pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SystemReboot(void *pHandle,u32 *pErrorCode);
/*=================================================================
��������: IPC_RecoverFactory
����:�豸�ָ�
����˵��: szMode, base(�򵥻ָ�������ip)/full(��ȫ�ָ�)
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_RecoverFactory (void *pHandle, char* szMode,u32 *pErrorCode);
/*=================================================================
��������: IPC_UserActive
����:�û�����(��ȫ�ָ�֮��)
����˵��:
		dwIP         IP(�ָ�֮��)
		wport        �˿�
		pParam       PTUSERACTIVEINFO
		pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_UserActive (u32 dwIP, u16 wPort, void*pParam,u32 *pErrorCode);
/*=================================================================
��������: IPC_ConfigUpload
����:���������ļ�
����˵��: pParam, PTUPLOADINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
��������: IPC_ConfigDownload
����:���������ļ�
����˵��: pParam, PTDOWNLOADINFO
		 pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_ConfigDownload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
��������: IPC_SysUpload
����:�豸����
����˵��: pParam, PTUPLOADINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUpload(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
��������:IPC_ SysUploadState
��    ��:����״̬��ѯ
����˵��:pHandle  		ǰ�˾��
		pParam,  		PTUPLOADSTATE;
		pErrorCode   	������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SysUploadState (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAdvanced 
����:��ȡϵͳ�߼�������Ϣ
����˵��: pParam, PTADVANCEDINFO
szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
�˴����ݣ�<osdpicfont opt=��song,black�� /><mtcfmode opt=��hdr,fps60,stream4,4k,div4,normal,hdsdi enable=true��><aacmode opt=��autolow, normal, low enable=true��>
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAdvanced (void *pHandle, void *pParam, int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAdvanced 
����:����ϵͳ�߼�������Ϣ
����˵��: 	pParam, PTADVANCEDINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAdvanced (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetUserInfo
��    ��: ��ȡ�û���Ϣ
����˵��: pParam, �û�Ȩ������TUSERINFO[]
		  nLen,   ���鳤��
		  nLenStruct,���ݳ���,����sizeof(TUSERINFO)
		  pErrorCode   ������
		  pErrorCode   ������
		  �� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetUserInfo (void *pHandle, void *pParam, int& nLen, int& nLenStruct,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetUserInfo
��    ��: �����û���Ϣ
����˵��: pParam, PTUSERINFO
		  nLenStruct,���ݳ���,����sizeof(TUSERINFO)
		  pErrorCode   ������
		  pErrorCode   ������
		  �� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetUserInfo (void *pHandle, void *pParam, int nLenStruct,u32 *pErrorCode);
/*=================================================================
��������: IPC_AddUser
��    ��: ����û�
����˵��: pParam, PTUSERINFO
		  nLen,���ݳ���,����sizeof(TUSERINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_AddUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_DelUser
��    ��: ɾ���û�
����˵��: pParam, PTUSERDELINFO
		  nLen,���ݳ���,����sizeof(TUSERDELINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_DelUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_UpdateUser
��    ��: �޸��û�
����˵��: pParam, PTUSERUPDATEINFO
		  nLen,���ݳ���,����sizeof(TUSERUPDATEINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_UpdateUser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetPowerInfo
��    ��: ��ȡ�û�Ȩ������
����˵��: pParam, �û�Ȩ������TPOWERINFO[]
		  nLen, ���鳤��
		  nLenStruct,���ݳ���,����sizeof(TPOWERINFO).
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerInfo (void *pHandle, void *pParam, int nLen, int& nLenStruct,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPowerInfo
��    ��: �����û�Ȩ������
����˵��: pParam, PTPOWERINFO
		  nLen, ���鳤��
		  nLenStruct,���ݳ���,����sizeof(TPOWERINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerInfo (void *pHandle, void *pParam, int nLen, int nLenStruct, u32 *pErrorCode);
/*=================================================================
��������: IPC_GetDdnsParam
��    ��: ��ȡDDNS����
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ����� RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetDdns(void *pHandle,void *pParam, int nLen, char* const szParamAssist, int nLenAssist, u32 *pErrorCode);


/*=================================================================
��������: IPC_SetDdns
��    ��: ����DDNS����
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetDdns(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetDdnsState
��    ��: ��ȡDDNS״̬����
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetDdnsState(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetNetPort
��    ��: ��ȡ�˿ڲ���
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetNetPort(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetNetPort
��    ��: ���ö˿ڲ���
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetNetPort(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetIpInfo
��    ��: ��ȡIP����
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetIpInfo
��    ��: ����IP����
����˵��: pParam [out] -- ��������ָ��
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetIpInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetWlanInfo
��    ��: ��ȡWlan����
����˵��: dwHandle      �豸���
		  pParam [out] -- ��������ָ��TWLANINFO
		  nLen -- ���ݳ���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetWlanInfo
��    ��: ����Wlan����
����˵��: dwHandle      �豸���
		  pParam [out] -- ��������ָ��TWLANINFO
		  nLen -- ���ݳ���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWlanInfo(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetPPPoE
��    ��: ��ȡPPPoE����
����˵��: pParam [out] -- ��������ָ��TUPNPPORTMAP
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetPPPoE
��    ��: ����PPPoE����
����˵��: pParam [out] -- ��������ָ��TUPNPPORTMAP
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPPPoE(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetQos
��    ��: ��ȡQos����
����˵��: pParam [out] -- ��������ָ��TQOSINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetQos
��    ��: ����Qos����
����˵��: pParam [out] -- ��������ָ��TQOSINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetQos(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetKSnmp
��    ��: ��ȡKSnmp����
����˵��: pParam [out] -- ��������ָ��TKSNMPINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetKSnmp
��    ��: ����KSnmp����
����˵��: pParam [out] -- ��������ָ��TKSNMPINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetKSnmp(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_Get8021x
		  ��    ��: ��ȡ8021x����
����˵��: pParam, PT8021XINFO
	  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
	  �˴����ݣ�<protocoltype opt=��eap-tls,eap-ttls,eap-peap,eap-leap,eap-fast,eap-md5��>
				<eapolversion opt= ��1, 2��>
	  �� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
==================================================================*/
IPC_API int CDECL IPC_Get8021x(void *pHandle, void *pParam,  int nLen, char* const szParamAssist, int nLenAssist, u32 *pErrorCode);

/*=================================================================
��������: IPC_Set8021x
��    ��: ����802.1x����
����˵��: pParam [out] -- ��������ָ��T8021XINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_Set8021x(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetUPnP
��    ��: ��ȡUPnP����
����˵��: pParamInfo[out] -- TUPNPINFO
		  nlenInfo -- pParamInfo���ݳ���,����sizeof(TUPNPINFO).
		  pParamMap[out] -- �˿�ӳ�������.
		  nlenMap -- �˿�ӳ������ݸ���TUPNPPORTMAP.
		  nLenMapStruct -- ����sizeof(TUPNPPORTMAP)�����ں�������.
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetUPnP
��    ��: ����UPnP����
����˵��: pParamInfo[out] -- TUPNPINFO
		  nlenInfo -- pParamInfo���ݳ���,����sizeof(TUPNPINFO).
		  pParamMap[out] -- �˿�ӳ�������.
		  nlenMap -- �˿�ӳ������ݸ���TUPNPPORTMAP.
		  nLenMapStruct -- ����sizeof(TUPNPPORTMAP)�����ں�������.
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetUPnP (void *pHandle, void * pInfo, int nlenInfo, void * pMap, int nLenMap, int &nLenMapStruct, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVsip
��    ��: ��ȡVsip����
����˵��: pParam [out] -- ��������ָ��TKVISPINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetVsip
��    ��: ����Vsip����
����˵��: pParam [out] -- ��������ָ��TKVISPINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetVsip(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetOnvif
��    ��: ��ȡOnvif����
����˵��: pParam [out] -- ��������ָ��TONVIFINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetOnvif
��    ��: ����Onvif����
����˵��: pParam [out] -- ��������ָ��TONVIFINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetOnvif(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetDpss
��    ��: ��ȡDpss����
����˵��: dwHandle      �豸���
		  pParam, TDPSSINFO
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetDpss
��    ��: ����Dpss����
����˵��: dwHandle      �豸���
		  pParam, TDPSSINFO
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDpss(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetAuthMode
��    ��: ��ȡAuthMode����
����˵��: pParam [out] -- ��������ָ��TAUTHMODEINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetAuthMode
��    ��: ����AuthMode����
����˵��: pParam [out] -- ��������ָ��TAUTHMODEINFO
		  nLen -- ���ݳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetAuthMode(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetGB28181
��    ��: ��ȡGB28181����
����˵��: pInfo, PTONVIFINFO
          byPlateid   ע��ƽ̨��(1~16)
	      pEncChnArr TGBENCCHNINFO[] ����ͨ����������ָ��
	      nlenEncChnInfo ����ͨ�������������
	      pAlarmChnArr TGBALARMCHNINFO[] �澯ͨ����������ָ��
	      nlenAlarmChnInfo�澯ͨ�������������
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�
		  <expand max="1024" opt="plat_domain,requriusedomain,fromtousedomain,
		  longitude,latitude,reg_retry_span,kplv_timeout_max_time,sdpfuseipc,
		  sdptimeisutc,rtcprtpdiff,alarmspan,catantfusecatarsp">
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetGB28181(void *pHandle, char*byPlateid,void *pInfo,  int nLenInfo, void *pEncChnArr, int& nLenEncChnInfo, void *pAlarmChnArr,  int& nLenAlarmChnInfo,char* const szParamAssist, int nLenAssist, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetGB28181
��    ��: ����GB28181����
����˵��: pInfo, PTONVIFINFO
		  byPlateid   ע��ƽ̨��(1~16)
	      pEncChnArr TGBENCCHNINFO[] ����ͨ����������ָ��
	      nlenEncChnInfo ����ͨ�������������
	      pAlarmChnArr TGBALARMCHNINFO[] �澯ͨ����������ָ��
	      nlenAlarmChnInfo�澯ͨ�������������
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetGB28181(void *pHandle,char*byPlateid, void *pInfo,  int nLenInfo, void *pEncChnArr, int nLenEncChnInfo, void *pAlarmChnArr,  int nLenAlarmChnInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetWifiProbe
��    ��: ��ȡwifi̽��
����˵��: pParam, TWIFIPROBEINFO
		  nLen, ���ݳ���,����sizeof(TWIFIPROBEINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetWifiProbe
��    ��: ����wifi̽��
����˵��: pParam, TWIFIPROBEINFO
		  nLen, ���ݳ���,����sizeof(TWIFIPROBEINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWifiProbe (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetWifiProbeList
��    ��: ��ȡwifi̽���豸�б�
����˵��: pParam, TWIFIPROBELISTINFO
		  nLen, ���ݳ���,����sizeof(TWIFIPROBELISTINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWifiProbeList (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetStorageState
		  ��    ��: ��ȡ����״̬
����˵��: pParam, PTSTORAGESTATE
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�
		  <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageState (void *pHandle,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetStorageNum
��    ��: ��ȡ���̸���
����˵��: 
	      nLenDisks    ���̸���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageNum (void *pHandle, int &nLenDisks, u32 *pErrorCode);
/*=================================================================
��������: IPC_GetStorageManager
		  ��    ��: ��ȡ�洢�������
����˵��: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo���ݳ���,����sizeof(TSTORAGEMANAGER).
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�<fullmode opt=��stop,over��/>   
		  <!-- req, xs:string --> <state opt="null,disker,diskrslowish,idel,noformat,formating,identifing">
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
 =================================================================*/
IPC_API BOOL32 CDECL IPC_GetStorageMgr  (void *pHandle, void * pInfo, int nLenInfo, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetStorageManager
		  ��    ��: ���ô洢�������
����˵��: pInfo, TSTORAGEMANAGER
	      nlenInfo, pInfo���ݳ���,����sizeof(TSTORAGEMANAGER).
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetStorageMgr  (void *pHandle, void * pInfo, int nLenInfo, u32 *pErrorCode);
/*=================================================================
��������: IPC_DiskFomat 
��    ��: ���̸�ʽ��
����˵��: pParam, PTSTORAGEFOMAT
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_DiskFomat  (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetFomatProgress
		  ��    ��: ���̸�ʽ������
����˵��: pParam, PTFOMATSTATE
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFomatProgress (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetRecSchedule
		  ��    ��: ��ȡ¼��ƻ�����
����˵��: pParam, PTRECSCHEDULE
	  nlenInfo, pParamInfo���ݳ���,����sizeof(TRECSCHEDULE).
	  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�<preparetime opt=��none,5,10,15,20,25,30,infinite��/>
	  <delaytime opt=��none,5,10,15,20,25,30,infinite��/>
	  <rectype opt=��main,assist,three��/><esps opt=��es, ps��/><time max= 8 index=1>
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRecSchedule (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetRecSchedule
		  ��    ��: ����¼��ƻ�����
����˵��: pParam, PTRECSCHEDULE
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRecSchedule (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetSnapPicConfig
��    ��: ��ȡǰ��ץ�Ĳ���
����˵��: pParam, PTSNAPPICCONFIG
	      nlen,  pParam���ݳ���,����sizeof(TSNAPPICCONFIG).
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�<type opt=��jpeg, jpg,bmp��/>
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapPicCfg  (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSnapPicConfig
��    ��: ����ǰ��ץ�Ĳ���
����˵��: pParam, PTSNAPPICCONFIG
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapPicCfg (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetSnapTime
��    ��: ��ȡ��ʱץ�ĵ�����
����˵��: pHandle  ǰ�˾��
	      pParam, PTSNAPTIME
	      pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSnapTime
��    ��: ���ö�ʱץ�ĵ�����
����˵��: pHandle  ǰ�˾��
	      pParam, PTSNAPTIME
	      pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapTime (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetSnapTime
��    ��: ��ȡ�¼�ץ�ĵ�����
����˵��: pHandle  ǰ�˾��
	      pParam, PTSNAPEVENT
	      pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSnapTime
��    ��: �����¼�ץ�ĵ�����
����˵��: pHandle  ǰ�˾��
	      pParam, PTSNAPEVENT
	      pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSnapEvent (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVideoShield
��    ��: ��ȡ�ƶ�������
����˵��: pParam, TDETECTINFO
	      nLen�� TDETECTINFO���ȣ� sizeof(TDETECTINFO);
	      pAreaArr, TDETECTPOINT��Ƶ�ڱ���������;
	      nLenArea����Ƶ�ڱ���������;
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�
		  <allcolor opt="gray,red,yellow,blue,orange,green,transparent,half-transparent,mosaic">
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetDetect(void *pHandle,  void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetVideoShield
��    ��: �����ƶ�������
����˵��: pParam, TDETECTINFO
	      nLen�� TDETECTINFO���ȣ� sizeof(TDETECTINFO);
	      pAreaArr, TDETECTPOINT��Ƶ�ڱ���������;
	      nLenArea����Ƶ�ڱ���������;
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDetect (void *pHandle, void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetShieldAlarm
��    ��: ��ȡ�ڵ���������
����˵��: pParam,   TALARMINFO
	      nLen��    TALARMINFO���ȣ� sizeof(TALARMINFO);
	      pAreaArr, TDETECTPOINT��Ƶ�ڱ���������;
	      nLenArea����Ƶ�ڱ���������;
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��	
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetShieldAlarm
��    ��: �����ڵ���������
����˵��: pParam,   TALARMINFO
	      nLen��    TALARMINFO���ȣ� sizeof(TALARMINFO);
	      pAreaArr, TDETECTPOINT��Ƶ�ڱ���������;
	      nLenArea����Ƶ�ڱ���������;
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetShieldAlarm (void *pHandle,void *pParam,int nLen, void *pAreaArr,int nLenArea,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetWarningLine
��    ��: ��ȡ�����߲���
����˵��: pParam,   TWARNINGLINEINFO
	      nLen��    TWARNINGLINEINFO���ȣ� sizeof(TWARNINGLINEINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetWarningLine
��    ��: ���þ����߲���
����˵��: pParam,   TWARNINGLINEINFO
	      nLen��    TWARNINGLINEINFO���ȣ� sizeof(TWARNINGLINEINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWarningLine (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAreaDetectEnter
��    ��: ��ȡ�������ֲ���
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAreaDetectEnter
��    ��: �����������ֲ���
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEnter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAreaDetectEntry
��    ��: ��ȡ����������
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAreaDetectEntry
��    ��: ��������������
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectEntry (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAreaDetectExit
��    ��: ��ȡ�����뿪����
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAreaDetectExit
��    ��: ���������뿪����
����˵��: pParam,      TAREADETECTINFO
	      nLen��       TAREADETECTINFO���ȣ� sizeof(TAREADETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAreaDetectExit (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetObjectLeft
��    ��: ��ȡ��Ʒ��������
����˵��: pParam,      TOBJECTDETECTINFO
	      nLen��       TOBJECTDETECTINFO���ȣ� sizeof(TOBJECTDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetObjectLeft
��    ��: ������Ʒ��������
����˵��: pParam,      TOBJECTDETECTINFO
	      nLen��       TOBJECTDETECTINFO���ȣ� sizeof(TOBJECTDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectLeft (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetObjectRemoval
��    ��: ��ȡ��Ʒ��ȡ����
����˵��: pParam,      TOBJECTDETECTINFO
	      nLen��       TOBJECTDETECTINFO���ȣ� sizeof(TOBJECTDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetObjectRemoval
��    ��: ������Ʒ��ȡ����
����˵��: pParam,      TOBJECTDETECTINFO
	      nLen��       TOBJECTDETECTINFO���ȣ� sizeof(TOBJECTDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetObjectRemoval (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetPersonAggregate
��    ��: ��ȡ��Ա�ۼ�����
����˵��: pParam,      TPERSONDETECTINFO
	      nLen��       TPERSONDETECTINFO���ȣ� sizeof(TPERSONDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPersonAggregate
��    ��: ������Ա�ۼ�����
����˵��: pParam,      TPERSONDETECTINFO
	      nLen��       TPERSONDETECTINFO���ȣ� sizeof(TPERSONDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPersonAggregate (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetVirtualFocus
��    ��: ��ȡ�齹������
����˵��: pParam,      TVIRTUALFOCUSINFO
	      nLen��       TVIRTUALFOCUSINFO���ȣ� sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetVirtualFocus
��    ��: �����齹������
����˵��: pParam,      TVIRTUALFOCUSINFO
	      nLen��       TVIRTUALFOCUSINFO���ȣ� sizeof(TVIRTUALFOCUSINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetVirtualFocus (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetSightChange
��    ��: ��ȡ�����������
����˵��: pParam,      TSIGHTCHANGEINFO
	      nLen��       TSIGHTCHANGEINFO���ȣ� sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetSightChange
��    ��: ���ó����������
����˵��: pParam,      TSIGHTCHANGEINFO
	      nLen��       TSIGHTCHANGEINFO���ȣ� sizeof(TSIGHTCHANGEINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSightChange (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAudioExcept
��    ��: ��ȡ�����쳣����
����˵��: pParam,      TAUDIOEXCEPTINFO
	      nLen��       TAUDIOEXCEPTINFO���ȣ� sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAudioExcept
��    ��: ���������쳣����
����˵��: pParam,      TAUDIOEXCEPTINFO
	      nLen��       TAUDIOEXCEPTINFO���ȣ� sizeof(TAUDIOEXCEPTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAudioExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetFaceDetect
��    ��: ��ȡ����������
����˵��: pParam,      TFACEDETECTINFO
	      nLen��       TFACEDETECTINFO���ȣ� sizeof(TFACEDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetFaceDetect
��    ��: ��������������
����˵��: pParam,      TFACEDETECTINFO
	      nLen��       TFACEDETECTINFO���ȣ� sizeof(TFACEDETECTINFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFaceDetect (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAlarmIn
��    ��: ��ȡ�����������
����˵��: pParam,PTALARMININFO
	      nLen�� TALARMININFO���ȣ� sizeof(TALARMININFO);
	      szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  nLenAssist, ������Ϣ����
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAlarmIn(void *pHandle, char*pAlmNum, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAlarmIn
��    ��: ���ñ����������
����˵��: pParam,PTALARMININFO
	      nLen�� TALARMININFO���ȣ� sizeof(TALARMININFO);
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmIn (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetAlarmOut
��    ��: ��ȡ�����������
����˵��: pParam, PTALARMOUT
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetAlarmOut (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetAlarmOut
��    ��: ���ñ����������
����˵��: pParam, PTALARMOUT
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetAlarmOut (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetExceptType
��    ��: ��ȡ�쳣���Ͳ���
����˵��: pParam, PTEXCEPTTYPE
		  nLen�� TEXCEPTTYPE���ȣ� sizeof(TEXCEPTTYPE)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetExceptType (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetExcpet
		  ��    ��: ��ȡ�쳣���ò���
����˵��: pExceptType  �쳣����
          pParam, PTEXCEPTINFO
		  nLen�� TEXCEPTINFO���ȣ� sizeof(TEXCEPTINFO);
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  nLenAssist, ������Ϣ����
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetExcept (void *pHandle, char*pExceptType ,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetExcept
��    ��: �����쳣���ò���
����˵��: pParam, PTEXCEPTINFO
		  nLen�� TEXCEPTINFO���ȣ� sizeof(TEXCEPTINFO);
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetExcept (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_PtzMove
��    ��: ����PTZ�ƶ�(��,��,��,��,��λ��ֹͣ)
����˵��: pParam,PTPTZMOVE
		  nLen�� TPTZMOVE���ȣ� sizeof(TPTZMOVE);
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_PtzMove (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_PtzZoneZoom
��    ��: ����PTZ��������
����˵��: pParam,PTPTZZONEZOOM
		  nLen�� TPTZZONEZOOM���ȣ� sizeof(TPTZZONEZOOM);
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_PtzZoneZoom (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_PtzGotoPoint
��    ��: ����PTZ���ж�λ
����˵��: pParam,TPTZGOTOPOINT
		  nLen�� TPTZGOTOPOINT���ȣ� sizeof(TPTZGOTOPOINT);
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_PtzGotoPoint (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzWiper
��    ��: ������ˢ����/�ر�
����˵��: pParam,TPTZWIPER
		  nLen�� TPTZWIPER���ȣ� sizeof(TPTZWIPER);
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzWiper (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzBase
��    ��: ��ȡ��̨��������
����˵��: 
	      pParam, PTPTZBASE
		  nLen�� TPTZBASE�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzBase (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ SetPtzBase
��    ��: ������̨��������
����˵��: pParam, PTPTZBASE
	      nLen��TPTZBASE�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzBase (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ IPC_GetPtzMainTain
��    ��: ��ȡ��̨�ϵ�ά������
����˵��: 
	      pParam, PTPTZMAINTAIN
		  nLen�� TPTZMAINTAIN�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzMainTain (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ IPC_SetPtzMainTain
��    ��: ������̨�ϵ�ά������
����˵��: pParam, PTPTZMAINTAIN
	      nLen��TPTZMAINTAIN�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzMainTain (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ IPC_SetPtzRefactory
��    ��: ������̨�ָ���������
����˵��: pParam, PTPTZREFACTORY
	      nLen��TPTZREFACTORY�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzRefactory (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzWatchOn
��    ��: ��ȡ��̨�����������
����˵��: 
		  pParam, PTPTZWATCHON
		  nLen�� TPTZWATCHON�ĳ���
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<mode opt=��hscan,vscan,preset,pathcruise,framescan,
		  randscan,fullviewscan,syncscan��>
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzWatchOn (void *pHandle, void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_ SetPtzWatchOn
��    ��: ������̨�����������
����˵��: pParam, PTPTZWATCHON
	      nLen��TPTZWATCHON�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzWatchOn (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzPreset
��    ��: ��ȡԤ��λ����
����˵��: 
	      bySn,��Ҫ��ȡ��Ԥ��λ����ţ�1~255, 0�����ȡ���е�Ԥ��λ��Ϣ
		  pParam, PTPTZPRESET
		  nLen��bySnΪ0 ����TPTZPRESET����ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzPreset (void *pHandle, u8 bySn, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzPreset
��    ��: ����Ԥ��λ����
����˵��: pParam, PTPTZPRESET
	  �� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzPreset (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzLimit
��    ��: ��ȡ��̨��λ�������
����˵��: 
		  pParam, PTPTZLIMIT
		  nLen�� TPTZLIMIT�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzLimit (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ SetPtzLimit
��    ��: ������̨��λ�������
����˵��: pParam, PTPTZLIMIT
	  nLen��TPTZLIMIT�ĳ���
	  �� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzLimit (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzPathCruise
��    ��: ��ȡѲ��ɨ�����
����˵��: 
	      bySn,��Ҫ��ȡ��ɨ��·������ţ�1~8, 0�����ȡ������Ϣ
		  pParam, PTPTZPATHCRUISE
		  nLen��bySnΪ0 ����TPTZPATHCRUISE����ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzPathCruise (void *pHandle, u8 bySn, void *pParam,int& nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzPathCruise
��    ��: ����Ѳ��ɨ�����
����˵��: pParam, TPTZPATHCRUISE[]��ɨ��·����Ϣ����
	      nLen��TPTZPATHCRUISE����ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruise (void *pHandle, u8 bySn,void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzPathCruiseEvent
��    ��: ����Ѳ��ɨ���¼�(����Ѳ��,ֹͣѲ��)
����˵��: pParam, TPTZPATHCRUISEEVT��Ѳ��ɨ���¼�
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzPathCruiseEvent (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetTimeTask
��    ��: ��ȡ��ʱ�������
����˵��: 
	      pParam, PTTIMETASK
		  nLen��sizeof(TTIMETASK);
	      pTimeArr, PTTIMETASKLIST
		  nlenTime,��ʱ����ʱ�����ݸ���.
		  nLenTimeStruct, ����sizeof(TTIMETASKLIST)�����ں�������.
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��˴����ݣ�<mode opt=��close,hscan,vscan,preset,pathcruise,framescan,randscan,
		  fullviewscan,syncscan��>
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetTimeTask (void *pHandle, void *pParam, int nLen, void * pTimeArr, int nLenTime ,int nLenTimeStruct , char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_ SetTimeTask
��    ��: ���ö�ʱ�������
����˵��: 
	      pParam, PTTIMETASK
		  nLen��sizeof(TTIMETASK);
	      pTimeArr, PTTIMETASKLIST
		  nlenTime,��ʱ����ʱ�����ݸ���.
		  nLenTimeStruct, ����sizeof(TTIMETASKLIST)�����ں�������.
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetTimeTask (void *pHandle, void *pParam, int nLen ,void * pTimeArr, int nLenTime ,int nLenTimeStruct,u32 *pErrorCode);
/*=================================================================
��������: IPC_ GetPtzMainTain
��    ��: ��ȡ��̨�������
����˵��: 
	      pParam, PTPTZCOORDINATE
		  nLen�� TPTZCOORDINATE�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzCoorDinate (void *pHandle, void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzCoorDinateEvt
��    ��: ������̨�����¼�
����˵��: 
	      pParam, PTPTZCOORDINATE
		  nLen�� TPTZCOORDINATE�ĳ���
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzCoorDinateEvt(void *pHandle, void *pParam,int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetPtzRatio
��    ��: ��ȡ��̨����
����˵��: 
	      dwHandle      �豸���
		  pParam, TPTZRATIO
		  nLen�� TPTZRATIO�ĳ���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPtzRatio (void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_SetPtzRatio
��    ��: ������̨����
����˵��: 
	      dwHandle      �豸���
		  pParam, TPTZRATIO
		  nLen�� TPTZRATIO�ĳ���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzRatio(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_SetPtzCoorDinateEvt
��    ��: ������̨P,T,Zֵ
����˵��: 
	      dwHandle      �豸���
		  pParam, PTPTZCOORDINATEEX
		  nLen�� TPTZCOORDINATEEX�ĳ���
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzCoorDinateEX(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_GetPtzPatternsInfo
��    ��: ʵ�ֻ���ɨ����Ϣ��ȡ
����˵��: pParam, PTPTZPATTERNSINFO
		  nLen,   ����ɨ���������
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzPatternsInfo (void *pHandle, void *pParam,  int &nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzPatternEvt
��    ��: ���û���ɨ���¼�
����˵��: pParam, PTPTZPATTERNEVT
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetPtzPatternEvt (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_GetPtzHvangle
��    ��: ��ȡ�ӳ�������
����˵��: pParam, PTPTZHVANGLE  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetPtzHVangle (void *pHandle, void *pParam,  int nLen,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetPtzWSInfo
��    ��: ����websocket��Ϣ���豸�������ϴ�ptz��Ϣ
����˵��: 
	      dwHandle      �豸���
		  pParam,		TPTZWSINFO
		  nLen��        TPTZWSINFO����
		  pErrorCode    ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPtzWSInfo(void* pHandle, void *pParam,int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_AddConnectDetect
��    ��: ������·���,һ��ipֻ��һ�����
����˵��:
		  dwIP		-- �豸IP 
		  wPort		-- �豸http�˿� 
		  dwConnectTimeOut	-- ��ⳬʱʱ��
		  dwReConnectTimes -- ������������
		  pcbfFun	-- �ص�����
          pContext	-- ������
�� �� ֵ: �ɹ����� RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_AddConnectDetect(void *pHandle, u32 dwConnectTimeOut, u32 dwReConnectTimes, cbfConnectDetect pcbfFun, void* pContext,u32 *pErrorCode);

/*=================================================================
��������: IPC_DelConnectDetect
��    ��: ɾ����·���,һ��ipֻ��һ�����
����˵��:
		  dwIP		-- �豸IP 

�� �� ֵ: �ɹ����� RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_DelConnectDetect(void *pHandle,u32 *pErrorCode);

/*=================================================================
��������: IPC_IsConnect
��    ��: ����״̬�ж�
          
����˵��:
		 dwHandle      �豸���
		 bIsConnect	   ����״̬
		 pErrorCode    ������ָ��

�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_IsConnect(u32 dwHandle, bool& bIsConnect, u32* pErrorCode);
/*=================================================================
��������: IPC_GetVideoInfo
��    ��: ��ȡrtp����
����˵��: pParam [in]       ����ṹ�����
		  nLen [in]		    ����ṹ�峤��
		  pInfoOut [out]	����ṹ�����
		  nLenInfo [out]	����ṹ�峤��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetVideoInfo(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode);

/*=================================================================
��������: IPC_GetRtspURL
��    ��: ��ȡrtp����,���澯Ԫ����
����˵��: pParam [in]       ����ṹ�����
		  nLen [in]		    ����ṹ�峤��
		  pInfoOut [out]	����ṹ�����
		  nLenInfo [out]	����ṹ�峤��
		  bNoStream			TRUE������tcp������FALSE����tcl����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRtspUrl(void *pHandle, emPlayVideoType eType,void *pParam, int nParamLen, void*pInfoOut,int& nLenInfo, u32* pErrorCode, int bNoStream = FALSE);

/*=================================================================
��������: IPC_StartVideo
��    ��: ����������(TCP,UDP)
����˵��: eType  ������������ type_udp,type_tcp
		  pParam[in]--TVIDEOPARAM
		  nLen --����ṹ�峤��
		  pInfoOut[out]--TPLAYVIDEOINFO
		  nLenInfo[out] --����ṹ��
		  pErrorCode   ������
�� �� ֵ: UDP:�ɹ�����id,ʧ�ܷ���0��ԭ�����pErrorCode,TCP:�ɹ����ŷ���id,ʧ������pErrorCode�Ƿ�ΪRET_FAILD
=================================================================*/
IPC_API u32 CDECL IPC_StartRealPlay(void *pHandle, emPlayVideoType eType,void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode);

/*=================================================================
��������: IPC_SetRtspMetaPort
��    ��: ����Rtsp Meta�˿�
����˵��: eType  ������������ type_udp,type_tcp
		  bVideo	�Ƿ������Ƶ
		  pParam[in]--TPLAYVIDEOPARAM
		  nLen --����ṹ�峤��	
		  pInfoOut[out]--TPLAYVIDEOINFO
		  nLenInfo[out] --����ṹ��
		  pErrorCode   ������
�� �� ֵ: �ɹ�����id,ʧ�ܷ���0��ԭ�����pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_SetRtspMetaPort(void *pHandle, emPlayVideoType eType, bool bVideo, void* pParam, int nLen,void*pInfoOut,int& nLenInfo,u32* pErrorCode);


/*=================================================================
��������: IPC_RemoveRtspMetaPort
��    ��: ȥ��Rtsp Meta�˿�
����˵��: 
		  dwPlayID      ����ID
		  pErrorCode    ������
�� �� ֵ: �ɹ�����id,ʧ�ܷ���0��ԭ�����pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_RemoveRtspMetaPort(void *pHandle, u32 dwPlayID, u32* pErrorCode);

/*=================================================================
��������: IPC_StopVideo
��    ��: ֹͣ����������(TCP)
����˵��: eType         ������������ type_udp,type_tcp
		  dwPlayID      ����ID
		  pErrorCode    ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRealPlay(void *pHandle,emPlayVideoType eType,u32 dwPlayID, u32* pErrorCode);
/*=================================================================
��������: IPC_SendKeyFrame
��    ��: ����ؼ�֡
����˵��: dwPlayID[in]--TRTCPVIDEOINFO��dwPlayID ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_SendKeyFrame(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
��������: IPC_GetVideoVolume_RTCP
��    ��: ��ȡ��Ƶ����
����˵��: pInfoOut[out]--TRTCPVIDEOVOLUMEINFO
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API int CDECL IPC_GetVideoVolume_RTCP(void *pHandle, void *pInfoOut, int nLenInfo, u32* pErrorCode);
/*=================================================================
��������: IPC_SetVideoVolume_RTCP
��    ��: ������Ƶ����
����˵��: pParam[in]--TRTCPVIDEOVOLUMEINFO
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API int CDECL IPC_SetVideoVolume_RTCP(void *pHandle, void* pParam, int nLen, u32* pErrorCode);
/*=================================================================
��������: IPC_VoiceCallState
��    ��: �Ƿ�������
����˵��: pInfoOut [out] TRTCPCALLSTATEINFO
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_VoiceCallState (void *pHandle, void* pInfoOut, int nOutInfo, u32 *pErrorCode);
/*=================================================================
��������: IPC_StartVoiceCall
��    ��: ��ʼ����
����˵��: pHandle		ǰ�˾��
		  emTransType   ���봫���ʽ
		  pInfoOut [out] TRTCPPREPARECALLINFO
		  pErrorCode   	������
�� �� ֵ: �ɹ����غ���ID, ʧ�ܷ���0��ԭ�����pErrorCode
=================================================================*/
IPC_API u32 CDECL IPC_StartVoiceCall(void *pHandle,emTransType eTranstype, void*pInfoOut,int& nLenInfo,u32 *pErrorCode);
/*=================================================================
��������: IPC_StopVoiceCall
��    ��: ֹͣ����
����˵��: dwPlayID[in]--TRTCPPREPARECALLINFO��dwID
pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopVoiceCall(void *pHandle, u32 dwVoiceCallId, u32 *pErrorCode);

/*=================================================================
��������: IPC_PuSnapShot
��    ��: ǰ���ֶ�ץ��
����˵��: 
		  byStreamId[in]--������(1),������(2),������(3),������(4)
		  pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ�·��),��׺Ϊjpeg
		  pErrorCode      ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_PuSnapShot(void *pHandle, u8 byStreamId, const char *pchFileName, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetMultiVideoMode
��    ��: ��ȡ������ģʽ����
����˵��: pInfoOut[in]--TMULTIVIDEOMODEINFO
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetMultiVideoMode(void *pHandle,u8 byVideoSource, void *pInfoOut, int &nLenInfo, u32 *pErrorCode);
/*=================================================================
��������: IPC_SetMultiVideoMode
��    ��: ���ö�����ģʽ����
����˵��: pParam[in]--TMULTIVIDEOMODEPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetMultiVideoMode(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVideoEnc
��    ��: ��ȡ��Ƶ�������
����˵��: pInfoOut[in]--TVIDEOENCINFO
		  byVideoSource[in]--��ƵԴID
		  byStreamId[in]--������(1),������(2),������(3),������(4)
		  szAssistInfo ������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVideoEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetVideoEnc
��    ��: ������Ƶ�������
����˵��: pParam[in]--TVIDEOENCPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetVideoEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetAudioEnc
��    ��: ��ȡ��Ƶ�������
����˵��: pInfoOut[in]--TAUDIOENCINFO
		  byVideoSource[in]--��ƵԴID
		  byStreamId[in]--������(1),������(2),������(3),������(4)
		  szAssistInfo ������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetAudioEnc(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetAudioEnc
��    ��: ������Ƶ�������
����˵��: pParam[in]--TAUDIOENCPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetAudioEnc(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

IPC_API int CDECL IPC_SetAudioEncExt(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetAudioDec
��    ��: ��ȡ��Ƶ�������
����˵��: pInfoOut[in]--TAUDIODECINFO
		  byVideoSource[in]--��ƵԴID
		  byStreamId[in]--������(1),������(2),������(3),������(4)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetAudioDec(void *pHandle, u8 byVideoSource, u8 byStreamId, void *pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetAudioDec
��    ��: ������Ƶ�������
����˵��: pParam[in]--TAUDIODECPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetAudioDec(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVideoShield
��    ��: ��ȡͼ���ڱ�
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TVIDEOSHIELDINFO
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVideoShield(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo,  char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetVideoShield
��    ��: ����ͼ���ڱ�
����˵��: pParam[in] --TVIDEOSHIELDPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetVideoShield(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVideoRoi
��    ��: ��ȡ�����������
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TVIDEOROIINFO
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVideoRoi(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetVideoRoi
��    ��: ���������������
����˵��: pParam[in] --TVIDEOROIPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetVideoRoi(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetVideoEncCut
��    ��: ��ȡ����ü�
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TVIDEOENCCUTINFO
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetVideoEncCut(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetVideoEncCut
��    ��: ���ñ���ü�
����˵��: pParam[in] --TVIDEOENCCUTPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetVideoEncCut(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetOsdCap
��    ��: ��ȡOSD��������֧��
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TOSDCAPINFO
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetOsdCap(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetOsd
��    ��: ��ȡOSDȫ�����Բ���
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TOSDINFO
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetOsd(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetOsd
��    ��: ����OSDȫ�����Բ���
����˵��: pParam[in] --TOSDPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetOsd(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetOsdList
��    ��: ���õ���OSD���Բ���
����˵��: pParam TOSDLISTINFO[] OSD��������ָ��
	      nLen   OSD�����������
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetOsdList(void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetOsdList
��    ��: ��ȡ����OSD���Բ���
����˵��: pInfoOut TOSDLISTINFO[] OSD��������ָ��
	      nLenInfo OSD�����������
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetOsdList(void *pHandle, u8 byVideoSource,void *pInfoOut, int &nLenInfo, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetOsdStateQuery
��    ��: OsdͼƬ�ϴ�״̬��ѯ
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TOSDSTATEINFO
		  szParamAssist,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetOsdStateQuery(void *pHandle, u8 byVideoSource, void* pInfoOut, int &nLenInfo, char* szAssistInfo, int &nAssistLen, u32 *pErrorCode);
/*=================================================================
��������: IPC_SetOsdPrepare
��    ��: OsdͼƬ׼���ϴ�
����˵��: pParam[in] --TOSDPREPAREPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetOsdPrepare(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetOsdUploadFile
��    ��: OsdͼƬ�ϴ�
����˵��: pParam[in] --TOSDUPLOADFILEPARAM
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetOsdUploadFile(void *pHandle, void* pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_RecoveryDef
��    ��: ͼ��ָ�Ĭ��
����˵��: pParam[in] --TIMGDEFPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API int CDECL IPC_Img_Def(void *pHandle,void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetColor
��    ��: ��ȡͼ�񳣹�����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TCOLORINFO
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetColor(void *pHandle, u8 byVideoSource, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetColor
��    ��: ����ͼ�񳣹�����
����˵��: pParam[in] --TCOLORPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetColor(void *pHandle, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetFocus
��    ��: ��ȡ�۽�����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TFOCUSINFO
		  szParamAssist[out] --�۽���ѡ����,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)	  
		  pErrorCode   ������		  
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFocus(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetFocus
��    ��: ���þ۽�����
����˵��: pParam[in] --TFOCUSPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFocus (void *pHandle, void* pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetWhiteBlance
��    ��: ��ȡ��ƽ�����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TWHITEBLANCEINFO
		  szParamAssist[out] --��ƽ���ѡ����,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)		  
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWhiteBlance (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetWhiteBlance
��    ��: ���ð�ƽ�����
����˵��: pParam[in] --TWHITEBLANCEPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWhiteBlance (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetIrcutFilter
��    ��: ��ȡ��ҹģʽ����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TRCUTFILTERINFO
		  szParamAssist[out] --��ҹģʽ��ѡ����,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)	  
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIrcutfilter (void *pHandle, u8 byVideoSource,void *pParam,int nLen, char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetIrcutFilter
��    ��: ������ҹģʽ����
����˵��: pParam[in] --TTRCUTFILTERPARAM
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIrcutfilter (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetIris
��    ��: ��ȡ��Ȧ����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TIRISINFO
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetIris (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetIris
��    ��: ���ù�Ȧ����
����˵��: pParam[in] --TIRISPARAM
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetIris (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetShutter
��    ��: ��ȡ���Ų���
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TSHUTTERINFO
		  szParamAssist[out] --���ſ�ѡ����,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��(���鳤��512����)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetShutter(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetSutter
��    ��: ���ÿ��Ų���
����˵��: pParam[in] --TSHUTTERPARAM
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetShutter (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetGain
��    ��: ��ȡ�������
����˵��: byVideoSource[in] --��ƵԴID
		  pInfoOut[out] --TGAININFO
		  pParam   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGain(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetGain
��    ��: �����������
����˵��: pParam[in] --TGAINPARAM
	      pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGain (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetInfrared
��    ��: ��ȡ�������
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TINFRAREDINFO
		  szParamAssist[out] -- ���������ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<contrlmode opt=��close,auto,farlamp,middlelamp,nearlamp,exnearlamp��/>
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetInfrared (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetInfrared
��    ��: ���ú������
����˵��: pParam[in] --TINFRAREDPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetInfrared (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetLaser
��    ��: ��ȡ�������
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TLASERINFO
		  szParamAssist[out] --����۹�ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��def,littlefacula,bigfaxula,custom1,custom2��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetLaser (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetLaser
��    ��: ���ü������
����˵��: pParam[in] --TLASERPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLaser (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetCorrido
��    ��: ��ȡ���Ȳ���
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TCORRIDOINFO
		  szParamAssist[out] --����ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��close, left,right��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCorridorMode (void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetCorrido
��    ��: �������Ȳ���
����˵��: pParam[in] --TCORRIDOPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetCorridorMode (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetImageMode
��    ��: ��ȡ����ģʽ����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TIMAGEMODEINFO
		  szAssistInfo[out] --����ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴�����:<spotmode opt=��close, horizont, vertical, central��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetImageMode (void *pHandle, u8 byVideoSource, void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);
/*=================================================================
��������: IPC_SetImageMode
��    ��: ���þ������
����˵��: pParam[in] --TIMAGEMODEPARAM
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetImageMode (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetNoiseRedece2D
��    ��: ��ȡ2D�������
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TNOISEREDUCE2DINFO
		  szAssistInfo[out] --2Dģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��auto,open,close��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get2DDenoise (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetNoiseRedece2D
��    ��: ����2D�������
����˵��: pParam[in] --TNOISEREDUCE2DPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set2DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetNoiseRedece3D
��    ��: ��ȡ3D�������
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TNOISEREDUCE3DINFO
		  szAssistInfo[out] --3Dģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��auto,open,close��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Get3DDenoise(void *pHandle, u8 byVideoSource,void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);


/*=================================================================
��������: IPC_SetNoiseRedece3D
��    ��: ����3D�������
����˵��: pParam[in] --TNOISEREDUCE3DPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_Set3DDenoise (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetLocalEcho
��    ��: ��ȡ���ػ��Բ���
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TLOCALECHOINFO
		  szAssistInfo[out] --ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��close, PAL,NTSC,LINE,INTERLEAVE��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetLocalEcho (void *pHandle, u8 byVideoSource, void *pParam, int nLen, char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetLocalEcho
��    ��: ���ñ��ػ��Բ���
����˵��: pParam[in] --TLOCALECHOPARAM
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetLocalEcho (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetPowerLineFrequency
��    ��: ��ȡ����˸����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TPOWERLINEFREQUENCYINFO
		  szAssistInfo[out] --ģʽ,������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  �˴����ݣ�<spotmode opt=��50hz,60hz,auto��/> 
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPowerLineFrequency (void *pHandle, u8 byVideoSource,void *pParam, int nLen,  char* szAssistInfo, int &nAssistLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetPowerLineFrequency
��    ��: ���÷���˸����
����˵��: pParam[in] --TPOWERLINEFREQUENCYPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPowerLineFrequency (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetWDR
��    ��: ��ȡ��̬����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetWDR(void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetWDR
��    ��: ���ÿ�̬����
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊwdr ֻ������nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetWDR (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetHLC
��    ��: ��ȡǿ�����Ʋ���
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetHLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetHLC
��    ��: ����ǿ�����Ʋ���
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊhlc ֻ������nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetHLC (void *pHandle, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetFogThrough
��    ��: ��ȡ͸�����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFogThrough (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetFogThrough
��    ��: ����͸�����
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊfogthrough����close ֻ������nLevel��Ч
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFogThrough (void *pHandle,void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetGamma
��    ��: ��ȡGamma����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGamma (void *pHandle, u8 byVideoSource, void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetGamma
��    ��: ����Gamma����
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊgamma��close ֻ������nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetGamma (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetSmartIR
��    ��: ��ȡSmartIR����
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetSmartIR (void *pHandle, u8 byVideoSource,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetSmartIR
��    ��: ����SmartIR����
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊsmartir��close ֻ������nLevel��Ч
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSmartIR (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetBLC
��    ��: ��ȡ���ⲹ������
����˵��: byVideoSource[in] --��ƵԴID
		  pParam[out] --TDYNAMICMODEINFO ֻ��nLevel��Ч
		  szParamAssist ������Ϣ��xmlƬ�εķ�ʽ����enableѡ���optѡ��
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBLC(void *pHandle, u8 byVideoSource,void *pParam,int nLen,char* szParamAssist, int& nLenAssist,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetBLC
��    ��: ���ñ��ⲹ������
����˵��: pParam[in] --TDYNAMICMODEPARAM  szMode[17]Ĭ��Ϊautoblc,regionblc��close ֻ������nLevel��Ч
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetBLC (void *pHandle, void *pParam, int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetFillLight
��    ��: ��ȡ���ⲹ���
����˵��: 
		  pParam[out] --TDFILLLIGHT
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetFillLight
��    ��: ���ú��ⲹ���
����˵��: pParam[in] --TDFILLLIGHT
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetFillLight (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_SetDynPluginState
��    ��: ���ö�̬���״̬
����˵��: pParam[in] --TDDYNMAICPLUGIN
          pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetDynPluginState (void *pHandle,void *pParam,int nLen,u32 *pErrorCode);

/*=================================================================
��������: IPC_GetRecMonthInfo
��    ��:	��ȡ����ͼ
����˵��:	pHandle		ǰ�˾��
		    pErrorCode   	������
			nYear       ��
		    nMonth		�·� 
		    pDataOut, 		Array of TRECMONTHINFO;
	        nArrayLenOut, 	���鳤��;
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false, ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetRecMonthInfo(void *pHandle, int nYear, int nMonth, void *pDataOut, int& nArrayLenOut ,u32* pErrorCode);
/*=================================================================
��������: IPC_GetRecordData
��    ��:	��ȡ¼������(ͬ��)
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
			u32 dwRecId		¼��id��id == 0 ��ʾͬ����id  != 0 ��Ҫ��׼����ȡ�Ľӿ��ﴴ��ID��
			pParam 		    PTRECPARAM;
			pDataOut		Array of PTRECDATA;
			nArrLenOut		¼���������鳤��,�����ʱ���������ĳ���
�� �� ֵ: �ɹ����ػ�ȡ��¼�����ݸ���, ʧ�ܷ���0, ԭ�����pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_GetRecordData(void *pHandle, u32 dwRecId, void *pParam, void *pDataOut,int& nArrLenOut,u32* pErrorCode);
/*=================================================================
��������: IPC_GetRecordNext
��    ��:	��ȡ¼������(�첽)
����˵��:	pHandle		ǰ�˾��
		    pErrorCode   	������
		    u32 dwTaskId		¼��id��id  != 0 ��Ҫ��׼����ȡ�Ľӿ��ﴴ��ID��
		    dwFirstIndex	��ʼ��ȡ����ʼ����λ��
		    pDataOut		Array of PTRECDATA;
	        nArrLenOut		¼���������鳤��
�� �� ֵ: �ɹ����ػ�ȡ��¼�����ݸ���, ʧ�ܷ���0, ԭ�����pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_GetRecordNext (void *pHandle, u32 dwTaskId, u32 dwFirstIndex,bool &bFinished,void *pDataOut,int nArrLenOut,u32* pErrorCode);
/*=================================================================
��������: IPC_PreLoadRecordList
��    ��:	׼��¼������
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
			pParam 		TRECPARAM
			nParamLen		�ṹ��ĳ���
�� �� ֵ: �ɹ����ز�ѯID, ʧ�ܷ���0, ԭ�����pErrorCode;
=================================================================*/
IPC_API u32 CDECL IPC_PreLoadRecordList(void *pHandle, void *pParam, int nParamLen =0, u32* pErrorCode=0);
/*=================================================================
��������: 	IPC_StartRecordPlay
��    ��:		��ʼ����
����˵��:		pHandle		ǰ�˾��
				pErrorCode   	������
				pParam		PTRECSTARTPLAYPARAM
				nParamLen	��������ĳ���
				pDataOut		PTRECSTARTPLAYINFO
				nDataLen		����ش����ݳ���
				bDec			�Ƿ����
�� �� ֵ: 	�ɹ����ز���id, ʧ�ܷ���0;
=================================================================*/
IPC_API u32 CDECL IPC_StartRecordPlay(void *pHandle, void *pParam, int nParamLen,void *pDataOut, int nDataLen,bool bDec, u32* pErrorCode);

/*=================================================================
��������: 	IPC_StopRecordPlay
��    ��:	ֹͣ����
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
			dwPlayID		����ID
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_StopRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
��������: 	IPC_PauseRecordPlay
��    ��:	��ͣ����
����˵��:	pHandle		ǰ�˾��
			pErrorCode  ������
			dwPlayID	����ID
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_PauseRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
��������: 	IPC_ResumeRecordPlay
��    ��:	�ָ�����
����˵��:	pHandle		ǰ�˾��
			pErrorCode  ������
			dwPlayID	����ID
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_ResumeRecordPlay(void *pHandle, u32 dwPlayID, u32* pErrorCode);
/*=================================================================
��������: 	IPC_MoveRecordPlay
��    ��:	����λ���ƶ�
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
			dwPlayID		����ID
			pParam		PTTIMEEX
			nParamLen		��������
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_MoveRecordPlay(void *pHandle, u32 dwPlayID, void *pParam, int nParamLen,u32* pErrorCode);
/*=================================================================
��������: 	IPC_SetRecordPlayRate
��    ��:	�����ٶȿ���
����˵��:	pHandle			ǰ�˾��
			pErrorCode   	������
			dwPlayID		����ID
			dwPlayRate		emRecPlayRate
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetRecordPlayRate(void *pHandle, u32 dwPlayID,u32 dwPlayRate, u32* pErrorCode);
/*=================================================================
��������: 	IPC_ Getrecplaystate
��    ��:	��ȡ�������
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
			dwPlayID		����ID
			pDataOut		PTRECPLAYSTATEINFO
			nDataOut          TRECPLAYSTATEINFO��С
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_Getrecplaystate (void *pHandle, u32 dwPlayID,void* pDataOut, int &nDataOut,u32* pErrorCode);


/*=================================================================
��������: 	IPC_Setrecdownloadspeed
��    ��:	����¼�������ٶ�
����˵��:	pHandle		ǰ�˾��
			pErrorCode   	������
�� �� ֵ: 	�ɹ�����true, ʧ�ܷ���false,������pErrorCode;
=================================================================*/
IPC_API BOOL32 CDECL IPC_Setrecdownloadspeed (void *pHandle,u32* pErrorCode);

/*=================================================================
��������: IPC_GetCompileTime
��    ��: ��ȡipcsdk�����ʱ��
          
����˵��:
		  szCompileTime sdk�����ʱ��
		  pErrorCode   	������
����ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetCompileTime(char* szCompileTime,u32*pErrorCode);

/*=================================================================
��������: IPC_AssTeleZoomInfo
��    ��: Զ�˷Ŵ�
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)    
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_AssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetAssTeleZoomInfo
��    ��: Զ�˷Ŵ�
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)    
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetAssTeleZoomInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);
/*=================================================================
��������: IPC_AssMalfInfo
��    ��: ���ν���
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)    
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_AssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetAssMalfInfo
��    ��: ���ν���
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)    
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetAssMalfInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetMTCF
��    ��: ���ù���ģʽ
����˵��: pParam, TMTCFPARAM
		  nLen, ���ݳ���,����sizeof(TMTCFPARAM)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetMTCF
��    ��: ��ȡ����ģʽ
����˵��: pParam, TMTCFPARAM
		  nLen, ���ݳ���,����sizeof(TMTCFPARAM)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetMTCF (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetBatteryStatus
��    ��: ��ȡ��ص���
����˵��: pParam, TBATSTATUSINFO
		  nLen, ���ݳ���,����sizeof(TBATSTATUSINFO)
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetBatteryStatus (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetSSH
��    ��: ����SSH����/�ر�
����˵��: bOpen,       �Ƿ���ssh
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetSSH (void *pHandle, bool bOpen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetColorSize
��    ��: ���õ�ɫ��
����˵��: pParam, PTPTZHVANGLE  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SetColorSize (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetColorSize
��    ��: ��ȡ��ɫ��
����˵��: pParam, PTPTZHVANGLE  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GetColorSize (void *pHandle, void *pParam,  int nLen, char* szParamAssist, int& nLenAssist, u32 *pErrorCode);

/*=================================================================
��������: IPC_SethotPointState
��    ��: �����ȵ�׷��״̬
����˵��: pParam, PTPTZHVANGLE  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_SethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GethotPointState
��    ��: ��ȡ�ȵ�׷��״̬
����˵��: pParam, PTPTZHVANGLE  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL IPC_GethotPointState (void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetGpsInfo
��    ��: ��ȡGPS��Ϣ
����˵��: pParam, TPOSITIONINFO  
�� �� ֵ: �ɹ�����RET_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetGpsInfo (void *pHandle, void *pParam, int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetMobilenetwork
��    ��: ��ȡ4g����
����˵��: dwHandle      �豸���
		  pParam, TMOBILENETWORK
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetMobilenetwork
��    ��: ����4g����
����˵��: dwHandle      �豸���
		  pParam, TMOBILENETWORK
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetMobilenetwork(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_GetPubsec
��    ��: ��ȡ��ͼ�����
����˵��: dwHandle      �豸���
		  pParam, TPUBSECPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_GetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

/*=================================================================
��������: IPC_SetPubsec
��    ��: ������ͼ�����
����˵��: dwHandle      �豸���
		  pParam, TPUBSECPARAM
		  pErrorCode   ������
�� �� ֵ: �ɹ�����true, ʧ�ܷ���false��ԭ�����pErrorCode
=================================================================*/
IPC_API BOOL32 CDECL IPC_SetPubsec(void *pHandle, void *pParam,  int nLen, u32 *pErrorCode);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************
ģ����  : ipcmedia���ܽӿ�
�ļ���  : ipcmedia_c.h
����ļ���ipcmedia_c.cpp
ʵ�ֹ��ܣ�
����    ��sunjun
�汾    ��IPC-V7.0-R1.0 Copyright (C) 2009 - 2012 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����
20150602      v1.0        sunjun	                 ����
******************************************************************************/
 #ifndef IPCMEDIA_H_
 #define IPCMEDIA_H_
 
 
 #ifdef __cplusplus
 extern "C" {
 
 #ifdef WIN32
 	#define IPC_API  extern "C" __declspec(dllexport)
 
 #ifndef CDECL
 	#define CDECL  __cdecl
 #endif
 #else
 	#define IPC_API
 	#define CDECL
 #endif
 
 #endif // __cplusplus
 
 #define STDCALL  __stdcall
 #define OUT
 
 	/* ý���� */
 #define PLAYERHANDLE unsigned long
 
 	/* ������ */
 #define ERR_SUCCESS                     (0)    // �ɹ�����
 #define ERR_NOMEDIADLL						(1)    // û���ҵ�SDKDLL
 #define ERR_NOMEDIAFUN						(2)    // û���ҵ�SDKDLL��Ӧ�ӿ�
 #define ERR_INIT_OSP                    (-1)   // OSPͨ��ģ���ʼ��ʧ��
 #define ERR_GET_PLAYER                  (-30)  // ��ȡ����������ʧ��
 #define ERR_INVALID_VD_SRC_ID           (-31)  // ��Ч����ƵԴͨ����
 #define ERR_INIT_DECODER                (-32)  // ��������ʼ��ʧ��
 #define ERR_START_DECODE                (-33)  // ����Ƶ����ʧ��
 #define ERR_INVALID_PLAY_WND            (-35)  // ��Ч�Ĳ��Ŵ��ھ��
 #define ERR_INIT_ENCODER                (-38)  // ��������ʼ��ʧ��
 #define ERR_START_SEND_AUDIO            (-41)  // ��Ƶ���뷢��ʧ��
 #define ERR_STOP_ENCODE                 (-42)  // ������ֹͣ��Ƶ����ʧ��
 #define ERR_REC_WITH_NO_VD_PLAY         (-43)  // ��û����Ƶ���ŵ�����½��б���¼��
 #define ERR_INIT_LOCAL_RECORDER         (-44)  // ����¼�����ʼ��ʧ��
 #define ERR_START_LOCAL_REC             (-45)  // ��ʼ����¼��ʧ��
 #define ERR_STOP_LOCAL_REC              (-46)  // ֹͣ����¼��ʧ��
 #define ERR_SHOT_WITH_NO_VD_PLAY        (-47)  // ��û����Ƶ���ŵ�����½��б���ץ��
 #define ERR_LOCAL_SNAPSHOT              (-48)  // ����ץ��ʧ��
 #define ERR_LOCAL_RECV_PU_REC           (-49)  // ���ؽ���ǰ��¼��ʧ��
 #define ERR_INVALID_REC_FILE_NAME       (-50)  // ���������ļ���Ϊ��
 #define ERR_LOCAL_OPEN_FILE             (-51)  // ���ؽ��������ļ�ʧ��
 #define ERR_LOCAL_PLAY_FILE             (-52)  // ���ؽ����������ļ�ʧ��
 #define ERR_INVALID_PU_REC_INFO         (-97)  // ָ����ǰ��¼����ϢΪ��
 
 // ������������ 
 #define MEDIA_STREAM_TYPE_UDP		    0x00	// UDPͨ��
 #define MEDIA_STREAM_TYPE_TCP		    0x01	// TCPͨ��
 
 // �������� 
 enum EPlayType
 {
 	PLAY_VIDEO = 1,
 	PLAY_PUREC
 };


 // ¼��ص�״̬
 typedef enum EMRecorderCBType
 {
	 MEDREC_START = 1,
	 MEDREC_STOP,
	 MEDREC_FILE_START ,
	 MEDREC_START_TIME,
	 MEDPREREC_START,
	 MEDPREREC_STOP,
	 MEDSTREAM_STOP,
 };

 // ¼��ֹͣԭ��
 typedef enum EMRecorderStopReason
 {
	 RECSTOPREASON_NORMAL = 0,
	 RECSTOPREASON_FORMAT_CHG = 1,  //format change
	 RECSTOPREASON_FILE_FAIL = 2,
	 RECSTOPREASON_DISK_WRITE_FULL =3,//������д��

 };
 
 // ��Ƶ֡ͷ�ṹ
 typedef struct tagIPCFrameHdr
 {
	 u8     m_byMediaType;				// ý������
	 u8    *m_pData;						// ���ݻ���
	 u32    m_dwPreBufSize;				// m_pData����ǰԤ���˶��ٿռ䣬���ڼ�
	 // RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12
	 // (FIXED HEADER + Extence option + Extence bit)
	 u32    m_dwDataSize;				// m_pDataָ���ʵ�ʻ����С�����С
	 u8     m_byFrameRate;				// ����֡��,���ڽ��ն�
	 u8      m_byPadSize;				// ������ݳ���*/
	 u32    m_dwFrameID;					// ֡��ʶ�����ڽ��ն�
	 u32    m_dwTimeStamp;				// ʱ���, ���ڽ��ն�
	 u64    m_llNTPTimeStamp;			// NTP time 
	 u32    m_dwSSRC;					// ͬ��Դ, ���ڽ��ն�
	 union
	 {
		 struct{
			 BOOL32    m_bKeyFrame;		// Ƶ֡���ͣ�I or P��
			 u16       m_wVideoWidth;	// ��Ƶ֡��
			 u16       m_wVideoHeight;	// ��Ƶ֡��
		 }m_tVideoParam;
		 u8    m_byAudioMode;			//��Ƶģʽ
	 };
 } TIPC_FRAMEHDR,*PTIPC_FRAMEHDR;

 // ������ͳ����Ϣ
 typedef struct tagIPCDecStatics
 {
	 unsigned char	byVideoFrameRate;	// ��Ƶ����֡��
	 unsigned short	wVideoBitRate;		// ��Ƶ��������
	 unsigned short	wAudioBitRate;		// ������������
	 unsigned long   dwVideoRecvFrame;	// �յ�����Ƶ֡��
	 unsigned long   dwAudioRecvFrame;	// �յ�������֡��
	 unsigned long   dwVideoLoseFrame;	// ��ʧ����Ƶ֡��
	 unsigned long   dwAudioLoseFrame;	// ��ʧ������֡��
	 unsigned short	wVideoLoseRatio;	// ��Ƶ��ʧ��,��λ��% 	
	 unsigned short	wAudioLoseRatio;	// ��Ƶ��ʧ��,��λ��% 	
	 unsigned long   dwPackError;		// ��֡��
	 unsigned long   dwIndexLose;		// ��Ŷ�֡
	 unsigned long   dwSizeLose;			// ��С��֡
	 unsigned long   dwFullLose;			// ����֡
	 unsigned long   wAvrVideoBitRate;	// 1��������Ƶ����ƽ������
	 unsigned long	wAvrAudioBitRate;	// 1��������������ƽ������
	 int	            bVidCompellingIFrm;	// ��Ƶǿ������I֡
	 unsigned long   dwDecdWidth;		// �����Ŀ�
	 unsigned long   dwDecdHeight;		// �����ĸ�
 } TIPC_DEC_STATICS, *PTIPC_DEC_STATICS;

 // ��������ͳ��
 typedef struct tagIPCRcvStatistics
 {
	 u32    m_dwPackNum;					// �ѽ��յİ���
	 u32    m_dwPackLose;				// �G����
	 u32    m_dwPackIndexError;			// ��������
	 u32    m_dwFrameNum;				// �ѽ��յ�֡��
 }TIPC_RCV_STATICSTICS;

 // ý����Ϣ
 typedef struct tagIPCMediaInfo
 {
	 unsigned long dwLocalIP;			// ����˵�ַ
	 unsigned short wVideoPort;			// ��Ƶ����˿�
	 unsigned short wAudioPort;			// ��Ƶ����˿�
 } TIPC_MEDIA_INFO, *PTIPC_MEDIA_INFO;


 // ����¼���ļ���Ϣ(��������)
 typedef struct tagIPCRecFileInfo
 {
	 unsigned char   byChanID;			                              // ͨ��ID
	 unsigned long   dwStartTime;		                              // ��ʼʱ��
	 unsigned long   dwEndTime;		                                  // ����ʱ��
	 unsigned long   dwRecSize;		                                  // �ļ���С
	 unsigned char   byRecFileType;		                              // ¼���ļ�����(EPuRecQueryType����)
	 unsigned long   dwFileID;
 } TIPC_REC_FILE_INFO, *PTIPC_REC_FILE_INFO;

 typedef struct tagIPCRecordSopInfo
 {
	 WORD m_wStopReason;
	 u64 m_qwTotalDiskUsage;
 } TIPC_REC_STOP_INFO;

 
 // ��Ƶ����ص�(�Ի�)
 typedef void (__stdcall *MEDIA_DRAW_CB)(PLAYERHANDLE hPlayHandle, unsigned long dwPaintDC, unsigned long dwFrmID, unsigned long dwParam);
 
 // ����Ƶ֡���ݻص�
 typedef void (__stdcall *MEDIA_FRAME_CB)(PLAYERHANDLE hPlayHandle, PTIPC_FRAMEHDR pFrmHdr, unsigned long dwParam);

 // �澯�ص�
 typedef void (__stdcall *MEDIA_ALARM_CB) (PLAYERHANDLE hPlayHandle, int& nType, int& nData, unsigned long dwParam); 

 //Ԫ����֡���ݻص�
 typedef void (__stdcall *MEAT_FRAME_CB)(PLAYERHANDLE hPlayHandle, PTIPC_FRAMEHDR pFrmHdr, unsigned long dwParam);
 
 // YUV����������Ϣ�ص�
 typedef void (*PYUVCALLBACK)(void* pParam,int &nWidth,int &nHeight,int &nBufferLen,unsigned char* pBuffer);

 // �����ֱ�Ϊλͼ����ָ�롢λͼ���ߡ�λ��
 typedef void (__stdcall *MEDIA_FRAMEINFO_CB)(PLAYERHANDLE hPlayHandle, unsigned char *pBitmapBuf, unsigned long dwWidth, unsigned long dwHeight, unsigned long dwbitcount,unsigned long dwFrmID, unsigned long dwParam);
 
 // �������
 typedef void (__stdcall *MEDIA_CONNECTLOST_CB)(PLAYERHANDLE hPlayHandle, void *pContext);

 // ¼��״̬�ص�
 typedef void (__stdcall *MEDIA_REC_STATE_CB)(PLAYERHANDLE hPlayHandle, unsigned long dwRecorderID, unsigned long dwCBType, void* pData, int nDataLen, void* pContext);

 // ¼�����ػص�
 typedef void (__stdcall *MEDIA_DOWN_REC_CB)(PLAYERHANDLE hPlayHandle, unsigned long dwStatusCode, unsigned long dwCurTime, void* pContext);
 
 /*=================================================================
 ��������:	MED_InitMedia
 ��    ��:	��ʼ��IPCMEDIA
			ȫ�ֵ���һ�μ��ɣ���Ӧ�ó���ʼ������
			(MFC������winapp�е��ã���̬�⹤���ڿ���ش�����)
 ����˵��:	
			wTelnetPort				Telnet�˿�(Ĭ��3000)
			bOpenTelnet				�Ƿ���Telnet(Ĭ�ϴ�)
			bMedPtRetrieve			�Ƿ����ö˿ڻ��ղ���
 �� �� ֵ:	�ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_InitMedia(char* szDLLFilePath = NULL, unsigned short wTelnetPort = 3000, int bOpenTelnet = 1, int bMedPtRetrieve = 1);
 
 /*=================================================================
 ��������:	MED_FreeMedia
 ��    ��:	�ͷ�IPCMEDIA�������Դ
			��Ӧ�ó����˳�ʱ����
 ����˵��:	void
 �� �� ֵ:	�ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_FreeMedia();
 
 /*=================================================================
 ��������:	MED_GetVideoPlayPort
 ��    ��:	��ȡ���Ŷ˿�
 ����˵��:	
			wVideoPort				��Ƶ�˿�
			wAudioPort				��Ƶ�˿�
			byVideoChan				����ͨ��
 �� �� ֵ:	�ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_GetVideoPlayPort( WORD& wVideoPort,WORD& wAudioPort ,DWORD& dwLocalIp,DWORD dwRemotIp,WORD wRemotPort,BYTE videoChan);
 
 /*=================================================================
 ��������:	MED_StopLocalPlay
 ��    ��:	����ֹͣ������������
 ����˵��:
			hPlayHandle				���������
 			bDestroy				�Ƿ����ٲ�����
 �� �� ֵ:	�ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_StopLocalPlay(PLAYERHANDLE hPlayHandle, int bDestroy = 1);
 
 /*=================================================================
 ��������:	MED_LocalPlay
 ��    ��:	���ؽ�����������
 ����˵��:
			hPlayWnd				ͼ��������ھ��
 			dwRemoteIP				�豸�˵�ַ(���ڶ����ش�)
 			wVideoDecPort			��Ƶ����˿�(��������sdk����)
 			wAudioDecPort			��Ƶ����˿�(��������sdk����)
			wVideoBackRtcp			��ƵRTCP�ش��˿�
			wAudioBackRtcp			��ƵRTCP�ش��˿�
			bDoubleAudio            �Ƿ���˫��Ƶ
			bMeta                   �Ƿ���Ԫ����
			nTransType              ��������
			b4k                     �Ƿ���4k
 �� �� ֵ:	�ɹ����ش���0�Ĳ��������ֵ, ʧ�ܷ��ش�����
 =================================================================*/
IPC_API long CDECL MED_LocalPlay(unsigned long hPlayWnd, unsigned long dwRemoteIP, unsigned char byChanId, unsigned short wVideoDecPort, unsigned short wAudioDecPort,unsigned short wVideoBackRtcp,unsigned short wAudioBackRtcp, unsigned short wAudioBackRtcp2,BOOL bDoubleAudio, BOOL bMeta, UINT nTransType, BOOL b4k = FALSE);

 /*=================================================================
 ��������: MED_PauseLocalPlay
 ��    ��: ��ͣ��Ƶ���
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_PauseLocalPlay(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_ResumeLocalPlay
 ��    ��: �ָ���Ƶ���
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_ResumeLocalPlay(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_IsLocalPlay
 ��    ��: �Ƿ����������Ƶ
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: TRUE-�������, FALSE-�������״̬
 =================================================================*/
 IPC_API int CDECL MED_IsLocalPlay(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_ChangePlayWnd
 ��    ��: ��������������
 ����˵��: 
     hPlayHandle     ���������
     hNewPlayWnd     ������Ƶ�Ĵ��ھ��(�´���)
     bSaveLastFrame  ��Ƶ����ʱ�Ƿ���Ҫ�������һ֡ͼ��
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_ChangePlayWnd(PLAYERHANDLE hPlayHandle, unsigned long hNewPlayWnd, int bSaveLastFrame = 1);
 
 /*=================================================================
 ��������: MED_SetDrawCallBack
 ��    ��: ���û�ͼ����ص�
 ����˵��:
     hPlayHandle     ���������
     fDrawCB         ��ͼ����ص�
     dwContext       �ص����������
 �� �� ֵ: void
 =================================================================*/
 IPC_API int CDECL MED_SetDrawCallBack(PLAYERHANDLE hPlayHandle, MEDIA_DRAW_CB fDrawCB, unsigned long dwContext);
 
 /*=================================================================
 ��������: MED_SetFrameCallBack
 ��    ��: ��������Ƶ֡���ݻص�
 ����˵��:    
     hPlayHandle     ���������
     fFrameCB        ֡ͷ��Ϣ�ص�
     dwContext       �ص����������
 �� �� ֵ: void
 =================================================================*/
 IPC_API int CDECL MED_SetFrameCallBack(PLAYERHANDLE hPlayHandle, MEDIA_FRAME_CB fFrameCB, unsigned long dwContext);

 IPC_API int CDECL MED_SetAlarmCallBack(PLAYERHANDLE hPlayHandle, MEDIA_ALARM_CB fFrameCB, unsigned long dwContext);

 IPC_API void CDECL MED_SetYuvCallBack (unsigned long hPlayHandle, PYUVCALLBACK pFun, u32 dwContext);

 IPC_API int CDECL MED_SetMetadataCallBack(PLAYERHANDLE hPlayHandle, MEAT_FRAME_CB fFrameCB, unsigned long dwContext);
 
 /*=================================================================
 ��������: MED_SetFrameInfoCallBack
 ��    ��: ����YUV����������Ϣ�ص�
 ����˵��:
     hPlayHandle     ���������
     fFrameInfoCB    YUV����������Ϣ�ص�
     dwContext       �ص����������
 �� �� ֵ: void
 =================================================================*/
 IPC_API int CDECL MED_SetFrameInfoCallBack(PLAYERHANDLE hPlayHandle, MEDIA_FRAMEINFO_CB fFrameInfoCB, unsigned long dwContext);
 
 
 /*===============================================================
 ��������: MED_SetVerticalSyncMode
 ��    ��: ������رս���Ĵ�ֱͬ��
 ����˵��: 
     bVerSync        �Ƿ�����ֱͬ��
     hPlayHandle     �������������ָ��������н�������������
 �� �� ֵ: void
 ================================================================*/
 IPC_API int CDECL MED_SetVerticalSyncMode(int bVerSync, PLAYERHANDLE hPlayHandle = 0);
 
 /*===============================================================
 ��������: MED_SetShowMargins
 ��    ��: ����ͼ��ü�
           (ȫ�ֲ���, �ڳ�ʼ��������ǰ����)
 ����˵��: 
     nLeft           �ü������Ե���
 	nTop            �ü����ϱ�Ե���
 	nRight          �ü����ұ�Ե���
 	nBottom         �ü����±�Ե���
 	bAuto           �Ƿ�������ͼ���С�ı�
 �� �� ֵ: void
 ================================================================*/
 IPC_API int CDECL MED_SetShowMargins(int nLeft, int nTop, int nRight, int nBottom, int bAuto = 1);
 
 /*===============================================================
 ��������: MED_SetShowMarginsEx
 ��    ��: ����ͼ��ü�
 ����˵��:    
     nLeft           �ü������Ե���
 	nTop            �ü����ϱ�Ե���
 	nRight          �ü����ұ�Ե���
 	nBottom         �ü����±�Ե���
 	hPlayHandle     ���������(Ϊ������������)
 	bAuto           �Ƿ�������ͼ���С�ı�
 �� �� ֵ: void
 ================================================================*/
 IPC_API int CDECL MED_SetShowMarginsEx(int nLeft, int nTop, int nRight, int nBottom, PLAYERHANDLE hPlayHandle = NULL, int bAuto = 1);
 
 /*===============================================================
 ��������: MED_SetSmoothness
 ��    ��: ������Ƶ������
           (������Ƶ���֮ǰʹ�ã��������ý�������ʼ������)
 ����˵��:
     nSmoothness     ��Ƶ������(Ĭ�Ϸ�Χ1~20)
 �� �� ֵ: void
 ================================================================*/
 IPC_API int CDECL MED_SetSmoothness(int nSmoothness);
 
 /*=================================================================
 ��������: MED_GetDecoderStatis
 ��    ��: ��ý�����Ϣ
 ����˵��:
     hPlayHandle        ���������
     ptDecStatics [out] ����ͳ����Ϣ
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_GetDecoderStatis(PLAYERHANDLE hPlayHandle, TIPC_DEC_STATICS *ptDecStatics);
 
 /*=================================================================
 ��������: MED_GetStatistics
 ��    ��: ��ȡ���ն˵İ���ͳ��
 ����˵��:
     hPlayHandle        ���������
     tKdvVidRcvStatistics [in/out] ��Ƶ����
 	tKdvAudRcvStatistics [in/out] ��Ƶ����
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_GetStatistics(PLAYERHANDLE hPlayHandle, TIPC_RCV_STATICSTICS &tKdvVidRcvStatistics,TIPC_RCV_STATICSTICS &tKdvAudRcvStatistics);
 
 /*=================================================================
 ��������: MED_GetMediaInfo
 ��    ��: ���ý����Ϣ(����˿ڵ�)
 ����˵��:
     hPlayHandle        ���������
     ptMediaInfo [out]  ý����Ϣ
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_GetMediaInfo(PLAYERHANDLE hPlayHandle, TIPC_MEDIA_INFO *ptMediaInfo);
 
 
 /*=================================================================
 ��������: MED_DenoiseFilter
 ��    ��: ����ͼ����(����˿ڵ�)
 ����˵��: hPlayHandle���������; bEnable �Ƿ���ͼ����; nLevelͼ����ȼ�
     
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_DenoiseFilter(PLAYERHANDLE hPlayHandle, BOOL bEnable, int nLevel = 3);
 
 
 /*=================================================================
 ��������: MED_StartAudioCall
 ��    ��: ��ʼ��������
 ����˵��:
     hPlayHandle     ���������
 	dwDesIP         �豸��IP��ַ
 	wDesPort        �豸��Ƶ����˿�
 	wPort           ����RTCP�˿�
     byADecChn       ��Ƶ����ͨ����(��0��ʼ)
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_StartAudioCall(PLAYERHANDLE hPlayHandle, unsigned long dwDesIP, unsigned short wDesPort, unsigned short wPort = 0, char byADecChn = 0);
 
 /*=================================================================
 ��������: MED_StopAudioCall
 ��    ��: ֹͣ��������
 ����˵��:
     hPlayHandle     ���������
     byADecChn       ��Ƶ����ͨ����(��0��ʼ)
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_StopAudioCall(PLAYERHANDLE hPlayHandle, unsigned char byADecChn = 0);
 
 /*=================================================================
 ��������: MED_IsAudioSending
 ��    ��: �Ƿ�����������״̬
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: TRUE-������, FALSE-û�з���
 =================================================================*/
 IPC_API int CDECL MED_IsAudioSending(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_IsAudioMute
 ��    ��: ���ؼ����Ƿ���
 ����˵��: 
     hPlayHandle     ���������
 �� �� ֵ: TRUE-����, FALSE-�Ǿ���
 =================================================================*/
 IPC_API int CDECL MED_IsAudioMute(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_SetMute
 ��    ��: ���ؼ��������Ƿ���
 ����˵��: 
     hPlayHandle     ���������
     bMute           1-����, 0-�Ǿ���
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_SetMute(PLAYERHANDLE hPlayHandle, int bMute);
 
 /*=================================================================
 ��������: MED_SetVolume
 ��    ��: ���ñ��ؼ�������
 ����˵��: 
     hPlayHandle     ���������
     byVolume        ����ֵ 0-25(��0-10)
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_SetVolume(PLAYERHANDLE hPlayHandle, unsigned char byVolume);
 
 /*=================================================================
 ��������: MED_GetVolume
 ��    ��: ��ȡ���ؼ�������
 ����˵��: 
     hPlayHandle     ���������
     pbyVolume [out] ����ֵ
 �� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
 IPC_API int CDECL MED_GetVolume(PLAYERHANDLE hPlayHandle, unsigned char *pbyVolume);
 
 /*=================================================================
 ��������: MED_IsCallMute
 ��    ��: ���������Ƿ�������״̬
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: TRUE-����, FALSE-������
 =================================================================*/
 IPC_API int CDECL MED_IsCallMute(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_SetCallMute
 ��    ��: ����������������״̬
 ����˵��:
     hPlayHandle     ���������
     bMute           1-����, 0-������
 �� �� ֵ: void
 =================================================================*/
 IPC_API int CDECL MED_SetCallMute(PLAYERHANDLE hPlayHandle, int bMute);
 
 /*=================================================================
 ��������: MED_SetCallVolume
 ��    ��: ����������������
 ����˵��:
     hPlayHandle     ���������
     byVolume        ����ֵ 0-25
 �� �� ֵ: void
 =================================================================*/
 IPC_API int CDECL MED_SetCallVolume(PLAYERHANDLE hPlayHandle, unsigned char byVolume);
 
 /*=================================================================
 ��������: MED_GetCallVolume
 ��    ��: ��ȡ������������
 ����˵��:
     hPlayHandle     ���������
 �� �� ֵ: ��������ֵ
 =================================================================*/
 IPC_API unsigned char CDECL MED_GetCallVolume(PLAYERHANDLE hPlayHandle);
 
 /*=================================================================
 ��������: MED_GetCallVolume
 ��    ��: UDP����
 ����˵��:
     hPlayHandle     ���������
 	wTimeOut		��ʱʱ��
 	MEDIA_CONNECTLOST_CB	�����ص�
 	pContext		�ص�������
 �� �� ֵ: ��������ֵ
 =================================================================*/
 IPC_API int CDECL MED_SetConnectLostCallBack(PLAYERHANDLE hPlayHandle, WORD wTimeOut, MEDIA_CONNECTLOST_CB  pEventCallBack , void *pContext);
 
 /*=================================================================
 ��������: MED_SetRealPlay
 ��    ��: ����ʵʱ���
 ����˵��:
     hPlayHandle     ���������
 	bRealPlay		ʵʱ���
 	wBufNum			buf����
 �� �� ֵ: ��������ֵ
 =================================================================*/
 IPC_API int CDECL MED_SetRealPlay(PLAYERHANDLE hPlayHandle, BOOL bRealPlay, WORD wBufNum);
 
 /*=================================================================
 ��������: MED_SetDecTimer
 ��    ��: ���ý��붨ʱ
 ����˵��:
     hPlayHandle     ���������
 	wDecTimer		���붨ʱ
 	�� �� ֵ:		��������ֵ
 =================================================================*/
 IPC_API int CDECL MED_SetDecTimer(PLAYERHANDLE hPlayHandle, WORD wDecTimer);

 /*=================================================================
��������: MED_StartLocalRec
��    ��: ��ʼ����¼��
����˵��:
    hPlayHandle     ���������
    pchFileName     ������ļ���
    pchFilePath     ������ļ�·��
	fRecStatCB      ¼��״̬�ص�
	pContext        �û�������
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL MED_StartLocalRec(PLAYERHANDLE hPlayHandle, const char *pchFileName, const char *pchFilePath, u32 dwCutRecFileLength =0, MEDIA_REC_STATE_CB fRecStatCB = 0, void* pContext = 0);

/*=================================================================
��������: MED_IsLocalRecording
��    ��: �Ƿ��ڱ���¼��״̬
����˵��:
    hPlayHandle     ���������
�� �� ֵ: TRUE-����¼����, FALSE-���ڱ���¼����
=================================================================*/
IPC_API int CDECL MED_IsLocalRecording(PLAYERHANDLE hPlayHandle);

/*=================================================================
��������: MED_StopLocalRec
��    ��: ֹͣ����¼��
����˵��:
    hPlayHandle     ���������
    bStopTransfer   �Ƿ�ֹͣת������
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL MED_StopLocalRec(PLAYERHANDLE hPlayHandle, int bStopTransfer = 1);

/*=================================================================
��������: MED_LocalSnapshot
��    ��: ����ץ��
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)    
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL MED_LocalSnapshot(PLAYERHANDLE hPlayHandle, const char *pchFileName, BOOL bYuv);

/*=================================================================
��������: MED_DownloadPuRec
��    ��: ����ǰ��¼���ļ�
          (ͬһʱ��ֻ������һ̨ǰ�˵�¼��)
����˵��:
    dwDevIP         ǰ�˵�ַ
	wDevPort        ǰ�˷��Ͷ˿�
    ptRecFileInfo   ¼���ļ���Ϣ
    pchSavePath     �����ļ���ȫ·��
	fDownRecCB      ¼�����ػص�
    pContext        �û�������
    bReStart        �Ƿ���������
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL MED_DownloadPuRec(unsigned long dwDevIP, unsigned short wDevPort, const TIPC_REC_FILE_INFO *ptRecFileInfo, const char *pchSavePath, 
										 MEDIA_DOWN_REC_CB fDownRecCB = 0, void* pContext = 0, int bReStart = 1,int bIsNVR = 0);

/*=================================================================
��������: MED_StopDownloadPuRec
��    ��: ֹͣ����ǰ��¼��
����˵��:
    bEndWrite       �Ƿ���ֹд�ļ�
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL MED_StopDownloadPuRec(int bEndWrite);

/*=================================================================
��������: MED_MetaPlay
��    ��: Ԫ���ݴ��俪��(����������,������������,Ԫ���ݿ���)
����˵��:
    hPlayWnd        ���Ŵ��ھ��(���ô���,������NULL)
    dwRemoteIP      Ŀ��IP
	wAudioDecPort   ��Ƶ����˿�
�� �� ֵ: �ɹ����ش���0�Ĳ��������ֵ, ʧ�ܷ��ش�����
=================================================================*/
IPC_API long CDECL MED_MetaPlay(unsigned long hPlayWnd, unsigned long dwRemoteIP, unsigned short wAudioDecPort);

/*=================================================================
��������: MED_StopMetaPlay
��   ��:  Ԫ���ݴ���ر�
����˵��:
		hPlayHandle   ���������
		bDestroy      �Ƿ�����
�� �� ֵ: �ɹ�����ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int CDECL  MED_StopMetaPlay(PLAYERHANDLE hPlayHandle, int bDestroy/*=1*/);


 #ifdef __cplusplus
 }
 #endif // __cplusplus
 

#endif //