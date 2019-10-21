#ifndef __LICENSEDEF_H__
#define __LICENSEDEF_H__

/////////////////////////////////////////////////////////////
//	Linux ����ϵͳͷ�ļ�
/////////////////////////////////////////////////////////////
#ifdef _LINUX_
#include <stdio.h>
#include <stdlib.h>
#endif //_LINUX_
// ///////////////////////////////////////////////////////////////////
// //	Win32 ����ϵͳͷ�ļ�
// ///////////////////////////////////////////////////////////////////
// #ifdef _MSC_VER
// #include <afx.h>
// #endif	// _MSC_VER

#include <string.h>
#include "kdvtype.h"

#ifndef LPCSTR
typedef const char *LPCSTR;
#endif

#define  KEY_MAX_CONTENT		4096	//license������󳤶�
#define  MAX_KEY_VALUE_NUM		100		// �ɶ�ȡ��Key-Value�������

// ������ ////////////////////////////////////////////////////////////////
#define SUCCESS_LICENSE						(u16)0                      // �ɹ�
#define ERR_LICENSE_AUTH_FAILED				(u16)1                      // ��Ȩ����֤ʧ��
#define ERR_LICENSE_FORMAT_FAILED			(u16)2                      // ��Ϣ��ʽ����
#define ERR_LICENSE_NO_VER_FAILED			(u16)3                      // �ް汾��Ϣ
#define ERR_LICENSE_DEV_INFO_FAILED			(u16)4                      // �豸��Ϣ��ȫ
#define ERR_LICENSE_ENCRYPT_FAILED			(u16)5                      // ����ʧ��
#define ERR_LICENSE_WRITEKEY_FAILED			(u16)6                      // дlicense�ļ�ʧ��
#define ERR_LICENSE_SMALL_VALUE				(u16)7                      // ��ֵС������
#define ERR_LICENSE_BIG_VALUE				(u16)8                      // ��ֵ��������
#define ERR_LICENSE_LONG_STRING				(u16)9                      // �ַ������ȴ�������
#define ERR_LICENSE_NO_ITEM_ENUM			(u16)10                     // û�д�ѡ��
#define ERR_LICENSE_OUT_DATE				(u16)11                     // ���ڳ�������
#define ERR_LICENSE_NO_KEY					(u16)12                     // ��֧�ִ�KEYֵ
#define ERR_LICENSE_READ_CFG_FAIL			(u16)13                     // �������ļ�ʧ��
#define ERR_LICENSE_INVALID_PARAM			(u16)14                     // �����Ƿ�
#define ERR_LICENSE_NO_KEY_TYPE				(u16)15                     // ��֧�ִ�����
#define ERR_LICENSE_NO_USB_KEY				(u16)16                     // ��USBKEY
#define ERR_LICENSE_USB_CONNECT_FAIL		(u16)17                     // ����USB�豸ʧ��
#define ERR_LICENSE_USB_GET_FAIL			(u16)18                     // ��ռUSB�豸ʧ��
#define ERR_LICENSE_OPENFILE_FAILED			(u16)19                     // ���ļ�����
#define ERR_LICENSE_INVALID_USBKEY			(u16)20                     // License�ļ���USB�豸����
#define ERR_LICENSE_USB_FORMAT_FAIL			(u16)21                     // ��ʽ��USB�豸ʧ��
#define ERR_LICENSE_USB_VERY_PASSWORD		(u16)22                     // У��USB�豸����ʧ��
#define ERR_LICENSE_USB_CREATE_FILE_FAIL	(u16)23                     // USB�豸�����ļ�ʧ��
#define ERR_LICENSE_USB_OPEN_FILE_FAIL		(u16)24                     // USB�豸���ļ�ʧ��
#define ERR_LICENSE_USB_DELETE_FILE_FAIL	(u16)25                     // USB�豸�����ļ�ʧ��
#define ERR_LICENSE_READKEY_FAILED			(u16)26                     // ��license�ļ�ʧ��
#define ERR_LICENSE_TOO_MANY_CHAR           (u16)27                     // ��Ȩ��Ϣ����
#define ERR_LICENSE_DECRYPT_FAILED			(u16)28                      // ����ʧ��
#define ERR_LICENSE_VERIFY_SOPIN			(u16)29                      // ��֤����Ա�������
#define ERR_LICENSE_CHANGE_DIR				(u16)30                      // �л�Ŀ¼����
#define ERR_LICENSE_CREATE_CONTEXT			(u16)31                      // ����������ʧ��
#define ERR_LICENSE_CLOSE_FILE				(u16)32                      // �ر��ļ�ʧ��
#define ERR_LICENSE_CREATE_MUTEX			(u16)33	                     // ������ʧ��
#define ERR_LICENSE_UNKNOWN					(u16)1000                   // ����ԭ��Ĵ���
//////////////////////////////////////////////////////////////////////////

// �����KEYֵ ///////////////////////////////////////////////////////////
#define KEY_LICENSE_FILE_PATH				(LPCSTR)"KEY_LICENSE_FILE_PATH"		// дLicense�ļ�·��
#define KEY_LICENSE_KEY						(LPCSTR)"KEY_LICENSE_KEY"			// ���֤����(36λUUID)
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
		byType = em_LMSNoKey;// δ֪
	}
	s8 achSN[128];
	u8 byType;	//0--hkey,1--ft
}TLMSUsbkeySN;

#endif  //__LICENSEDEF_H__