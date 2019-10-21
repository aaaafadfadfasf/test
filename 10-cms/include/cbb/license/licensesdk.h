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
#define NAME_LEN			    40      //�û�������
#define MAX_PATH				260

//��������ض���
enum SDK_ERRCODE
{
    SDK_NO_ERR = 0,			//�޴���
	SDK_PARAM_ERR,			//��������
	SDK_INIT_ERR,			//��ʼ���������
	SDK_CONNECT_ERR,		//���ӷ���ʧ��
	SDK_TIMEOUT,			//��ʱ����
	SDK_UNKOWN,				//δ֪����
	SDK_OPRKEY_ERR,			//����key����
	SDK_NO_FUNCTION,		//��ʱ�޴˹���
//	SDK_APPLY_OK,			//����License�ɹ�
	SDK_APPLY_UUID_ERR,		//����Licenseʱ�������UUID��ռ�ã���Ҫ����reset�ӿ� 
	SDK_APPLY_LICENSEOVER,	//���������Ѿ�û�д�license
};

enum SDK_MESG_TYPE
{
	MESG_DISSCONNECT,		//�����������
	MESG_STOP_AUTHORIZE,	//��Ȩ����������ֹ
};

/*========================================================================
�� �� ����pfEventCB
��    �ܣ�SDK�ص�����
��    ����dwMsg����Ϣ���Ͷ��� ��ϸ��ο�SDK_MESG_TYPE
		  pchParam��<param><InitHandle>0</InitHandle></param>
		  pUserData: �û�����
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
typedef void (stdcall *pfEventCB)(s32 dwMsg, s8* pchParam, void *pUser);

typedef struct tagServerInfo
{				
	s8	m_achIp[IP_LEN];		//license������IP	    [����]
    u16	m_wPort;				//license������port		[����]
	s8	m_achUser[NAME_LEN];	//license�������û���   [����]
	s8	m_achPwd[NAME_LEN];		//license����������     [����]	
	s8	m_achCfgPath[MAX_PATH]; //�洢�豸��ϢXml�ļ�·�� *��ʽ��"./test/"	[����]
	s8	m_achDevName[NAME_LEN];	//�豸����	[�ɲ���]
	s8  m_achDevId[NAME_LEN];   //�豸ID,��Ϊmac��ַ���豸ΨһId *��ʽ��"000C294CC1FA" [����]
	u32	m_dwCheckCount;			//���dwCheckCount������Ӧ��Ϊ����	[����]	
	u32	m_dwCheckTime;			//ÿ�μ����ʱ��ΪdwCheckTime		[����]
	pfEventCB	m_pfEvent;		//�¼��ص�����
	void*		m_pUserData;	//�û�����

	tagServerInfo()
	{
		memset(this,0,sizeof(tagServerInfo));
	}
}LICENSE_SERVER_INFO;

typedef struct tagReq_Param
{
	s8	m_achModel[NAME_LEN];	//��Ʒ����
	s8	m_achKeyId[NAME_LEN];	//Ҫ����key id������������Զ�����
	u16	m_dwApplylicNum;		//����License����Ȩ���������ͬһ��license�����ҵ����Ȩʱ������Ϊ2500
    u32	m_dwFreeTime;			//����ʧЧʱ��,sdk��⵽�����󣬻��dwFreeTime��֪ͨҵ�����license��
	
	tagReq_Param()
	{
		memset(this,0,sizeof(tagReq_Param));
	}
}REQ_PARAM;


/*========================================================================
�� �� ����SDK_Init
��    �ܣ�SDKʼ���ӿ�
��    ����LICENSE_SERVER_INFO &tServerInfo	[in]license�����������Ϣ
		  u32 *InitHandle					[out]���س�ʼ���ɹ�handle,
		  
�� �� ֵ��SDK_ERRCODE ������
˵	  ������ʼ�������Դ����һ�����ӡ�����ע����ز���
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API SDK_ERRCODE SDK_Init(LICENSE_SERVER_INFO &tServerInfo, u32 *InitHandle);

/*========================================================================
�� �� ����SDK_Quit
��    �ܣ�����ʼ��SDK�ӿ�(���ͷ�֮ǰ��Ȩ)
��    ����u32 InitHandle					[in]��ʼ���ɹ�handle
		  
�� �� ֵ��SDK_ERRCODE ������
˵	  ����
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API SDK_ERRCODE SDK_Quit(u32 InitHandle);

/*========================================================================
�� �� ����SDK_ApplyLicense
��    �ܣ�������Ȩ
��    ����u32 InitHandle					[in]��ʼ���ɹ�handle
		  REQ_PARAM &tReqParam				[in]�ͷ���Ȩ����
		  s8*  pchParam						[in/out]�������������� ʹ��ʱ�����1024�ֽ�
		  s32 &nApplyNum					[out]���뵽��license����

�� �� ֵ��SDK_ERRCODE ������
˵	  �����ýӿڵ��óɹ��󣬿�ͨ��SDK_GetValue��ȡ��Ӧ��ֵ
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API SDK_ERRCODE SDK_ApplyLicense(u32 InitHandle, REQ_PARAM  &tReqParam, s32 &nApplyNum, s8*  pchParam = 0);




SDK_API SDK_ERRCODE SDK_ReleaseLicense(u32 InitHandle, REQ_PARAM &tReqParam, s8*  pchParam = 0);

/*========================================================================
�� �� ����SDK_SetUserData
��    �ܣ������û��Զ�������
��    ����u32 InitHandle					[in]��ʼ���ɹ�handle
		  s8 *pchUserdata					[in]�û�����
		  s32 &nValueLen					[in]�û�������Ч����
�� �� ֵ��SDK_ERRCODE ������
˵	  ����������������Զ������ݣ���������������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API SDK_ERRCODE SDK_SetUserData(u32 InitHandle, s8 * pchUserdata, s32 nValueLen);

/*========================================================================
�� �� ����SDK_GetUserData
��    �ܣ���ȡ�û��Զ�������
��    ����u32 InitHandle					[in]��ʼ���ɹ�handle
		  s8 *pchUserdata					[out]�û�����
		  s32 &nValueLen					[in/out]�û�������Ч����
�� �� ֵ��SDK_ERRCODE ������
˵	  �������������ȡ֮ǰ������Զ�������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API SDK_ERRCODE SDK_GetUserData(u32 InitHandle, s8 * pchUserdata, s32 &nValueLen);

//���ļ�key�ӿ�
/*========================================================================
�� �� ����SDK_GetKeyValueNum
��    �ܣ���ȡLicense�ļ����ж���Key��valueֵ��
��    ����u32 InitHandle				[in]��ʼ���ɹ�handle,Ϊ0��ֱ�Ӷ�ȡ�ڴ�������
�� �� ֵ��Key��valueֵ����
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_GetKeyValueNum(u32 InitHandle = NULL);

/*========================================================================
�� �� ����SDK_EnumKeyList
��    �ܣ�ö��License�ļ����ж���Keyֵ
��    ����TKeyValue *pchKeyValueList	[out]Key-Valueֵ�б�
		  s32 &nListLen					[in/out]Key-Valueֵ�б�ռ��С/Key-Valueֵ��
		  u32 InitHandle				[in]��ʼ���ɹ�handle,Ϊ0��ֱ�Ӷ�ȡ�ڴ�������
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_EnumKeyList(TLMSKeyValue *pchKeyValueList, s32 &nListLen, u32 InitHandle = NULL);

/*========================================================================
�� �� ����LMS_GetValue
��    �ܣ�����Keyֵ��ȡ��Ӧ��ֵ
��    ����s8 *pchKey					[in]Keyֵ
		  s8 *pchValue					[out]ֵ
		  s32 &nValueLen				[in/out]ֵ��Ч����
		  u32 InitHandle				[in]��ʼ���ɹ�handle,Ϊ0��ֱ�Ӷ�ȡ�ڴ�������
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_GetValue(s8 *pchKey, s8 *pchValue, s32 &nValueLen, u32 InitHandle = NULL);

//�����Ǽ���֮ǰ��key����
/*========================================================================
�� �� ����SDK_ReadLicenseFile
��    �ܣ���ȡLicense�ļ�
��    ����s8 *pchPath					[in]License�ļ���(ȫ·����������'\0'��β)
		  BOOL32 bCheck					[in]�Ƿ�У��License�Ϸ���(Ĭ��У��)
		  
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_ReadLicenseFile(s8 *pchPath, BOOL32 bCheck = TRUE);

/*========================================================================
�� �� ����SDK_WriteLicenseFile
��    �ܣ�дLicense�ļ�
��    ����BOOL32 bToUsb = FALSE             [in]    �Ƿ�д��USB�豸
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_WriteLicenseFile(BOOL32 bToUsb = FALSE);

/*=============================================================================
�� �� ��: SDK_SetValue
��    ��: ���ڴ���������
��    ��: LPCSTR lpszKey                    [in]    ����
          LPCSTR lpszValue                  [in]    ��ֵ
ע    ��: ��ˢ�¶�ȡ���������ڴ�
�� �� ֵ: ������
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2015/06/25	1.0		����	����
=============================================================================*/
SDK_API u16 SDK_SetValue(LPCSTR lpszKey, LPCSTR lpszValue);

/*========================================================================
�� �� ����LMS_GetEncryptMachineNO
��    �ܣ���ȡ���ܺ�Ļ����루����linuxϵͳ��Ч��windows�·���ΪFALSE��

��    ����s8 *pchEncryptMachineNO		[out]���ܺ������		  
�� �� ֵ��BOOL32 TRUE : �ɹ�   FALSE : ʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API BOOL32 SDK_GetEncryptMachineNO(s8 *pchEncryptMachineNO);

/*========================================================================
�� �� ����LMS_GetErrorCodeDescription
��    �ܣ���ȡ���ò���key�����Ĵ����������
��    ����u16 wErrorCode				[in]������
		  s8 *pchDescription			[out]����������
		  s32 &nLen						[in/out]������������Ч����
		  s8 *pchIniFilePath			[in]�����������ļ�ȫ·��(�����ļ���)������'\0'��β
�� �� ֵ��BOOL32 TRUE : �ɹ�   FALSE : ʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API BOOL32 SDK_GetErrorCodeDescription(u16 wErrorCode, s8 *pchDescription, s32 &nLen, s8 *pchIniFilePath);

/*=============================================================================
�� �� ��: LMS_IsUsbKeyOnline
��    ��: �ж��Ƿ���USBKey����
��    ��: void
ע    ��: ��ˢ�¶�ȡ���������ڴ�
�� �� ֵ: BOOL32 TRUE : ��   FALSE : û��
-------------------------------------------------------------------------------
�޸ļ�¼:
��      ��  �汾    �޸���  �޸�����
2015/06/25	1.0		����	����
=============================================================================*/
SDK_API BOOL32 SDK_IsUsbKeyOnline(void);

/*========================================================================
�� �� ����LMS_GetUsbkeyNum
��    �ܣ���ȡ��ǰPC����usbkey������ö��Usbkey���к�
��    ����TUsbkeySN *ptUsbKeySNList		[out]���к��б�
		  s32 &nLen						[in/out]usbkey����
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_GetUsbkeyNum(TLMSUsbkeySN *ptUsbKeySNList, s32 &nLen);

/*========================================================================
�� �� ����LMS_ReadLicenseFileBySN
��    �ܣ�ͨ��usbkey���кŶ�ȡUsbkey
��    ����TUsbkeySN tUsbKeySN			[in]���к�
�� �� ֵ��u16 ������
--------------------------------------------------------------------------
�޸ļ�¼��
��		��	�汾	�޸���	�޸�����
2015/06/25	1.0		����	����
========================================================================*/
SDK_API u16 SDK_ReadLicenseFileBySN(TLMSUsbkeySN tUsbKeySN);

#endif  // endif __LICENSE_SDK_H__