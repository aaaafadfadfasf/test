/*****************************************************************************
   ģ����      : KDM
   �ļ���      : kdmconst.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDM���ýӿڵĳ�������ͷ�ļ�
   ����        : ����
   �汾        : V1.0  Copyright(C) 2006-2007 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���        �޸�����
   2006/02/20  1.0         ����        ����
******************************************************************************/

#ifndef _CONST_KDM_H_
#define _CONST_KDM_H_

///////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kdvtype.h"
//#include "kdmdef.h"


//1. ��ŵ���ض���

#define   LEN_KDM_NO                    (u8)32 //AAA�еĸ�������Ψһ��ŵĳ���

//1.1 ��������������: CMS + NO����� + ʣ��

#define   FIELD_LEN_CMS                 (u8)18 //CMS      ����γ���
#define   FIELD_LEN_NOTYPE              (u8)1  //NO����� ����γ���
#define   FIELD_LEN_ELSE                (u8)13 //ʣ��     ����γ���

//1.1.1 CMS��������������:  [2*8λ]����� + [2λ]CMUƽ̨���
//      Ӧ��������ƽ̨һһ��Ӧ������һ��������Ϊ���CMUƽ̨���

#define   MAXNUM_DOMAIN_LEVEL           (u8)8  //�����㼶
#define   FIELD_LEN_SINGLE_DOMAIN       (u8)2  //��һ����󳤶�
#define   FIELD_LEN_DOMAIN              (u8)(MAXNUM_DOMAIN_LEVEL*FIELD_LEN_SINGLE_DOMAIN)
#define   FIELD_LEN_CMU                 (u8)2  //CMUƽ̨��� ����γ��ȣ�2006-09-07��Ϊ��Ϊ��ҵ���

//1.1.2 NO����������������һЩ����

//
#define   NOFIELD_TYPE_INVALID          (u8)0  //��Ч�������, ��Ӧ����
#define   NOFIELD_CHAR_INVALID          (s8)'0'//��Ч�������, ��Ӧ�ַ�
#define   NOFIELD_TYPE_PRECEDINGDEVICE  (u8)1  //ǰ���豸�������
#define   NOFIELD_TYPE_PLATFORMDEVICE   (u8)2  //ƽ̨�豸�������
#define   NOFIELD_TYPE_CUSTOMER         (u8)3  //�ͻ��������
#define   NOFIELD_TYPE_CUSER            (u8)4  //�ͻ��û��������
#define   NOFIELD_TYPE_DOMAINORCMU      (u8)5  //���CMUƽ̨�������
#define   NOFIELD_TYPE_TUSER            (u8)6  //�����û��������
#define   NOFIELD_TYPE_CDGROUP          (u8)7  //�ͻ��豸��������
#define   NOFIELD_TYPE_CUGROUP          (u8)8  //�ͻ��û���������

//1.1.3 ʣ�ಿ�ָ��ݲ�ͬ��NO������ɸ��Ե����

//1.1.3.1 ǰ���豸ʣ�ಿ�ֱ��: PDTYPE + PDSUBTYPE + PDSN, ĩβ����
#define   FIELD_LEN_PDTYPE              (u8)1  //ǰ���豸����   ����γ���, �������Ͳμ�4.2
#define   FIELD_LEN_PDSUBTYPE           (u8)1  //ǰ�����豸���� ����γ���, �������Ͳμ�4.2
#define   FIELD_LEN_PDSN                (u8)6  //ǰ���豸���   ����γ���

//1.1.3.2 ƽ̨�豸ʣ�ಿ�ֱ��: CDTYPE + CDSN, ĩβ����
#define   FIELD_LEN_CDTYPE              (u8)1  //ƽ̨�豸����   ����γ���, �������Ͳμ�4.1
#define   FIELD_LEN_CDSN                (u8)3  //ƽ̨�豸���   ����γ���
// hanwang 2008-8-22 ƽ̨�豸��չ����
#define   FIELD_LEN_CDTYPE_EX           (u8)2  // ��λ

//1.1.3.3 �ͻ�ʣ�ಿ�ֱ��: CUSTOMERSN, ĩβ����
#define   FIELD_LEN_CUSTOMERSN          (u8)5  //�ͻ����       ����γ���

//1.1.3.4 �ͻ��û�ʣ�ಿ�ֱ��: CUSTOMERSN + CUSTOMERUSERDSN, ĩβ����
#define   FIELD_LEN_CUSTOMERUSERSN      (u8)3  //�ͻ��û����   ����γ���

//1.1.3.5 ���CMUƽ̨ʣ�ಿ�ֱ��: ȫ��

//1.1.3.6 �����û�ʣ�ಿ�ֱ��: TELUSERSN, ĩβ����
#define   FIELD_LEN_TELUSERSN           (u8)8  //�����û����   ����γ���

//1.1.3.7 �ͻ��豸��ʣ�ಿ�ֱ��: CUSTOMERSN + CDGROUPSN, ĩβ����
#define   FIELD_LEN_CDGROUPSN           (u8)5  //�ͻ��豸����� ����γ���

//1.1.3.8 �ͻ��û���ʣ�ಿ�ֱ��: CUSTOMERSN + CUGROUPSN, ĩβ����
#define   FIELD_LEN_CUGROUPSN           (u8)3  //�ͻ��û������ ����γ���


//1.2
#define   MAXNUM_SUBDOMAIN_ONEDOAMIN       (u32)99      //һ�����µ����������
#define   MAXNUM_SUBCMU_ONEDOAMIN          (u32)99      //һ�����µ���CMU�����
#define   MAXNUM_TUSER_ONEDOAMIN           (u32)99999999//һ�����µĵ����û������
#define   MAXNUM_PREDEVICE_ONEDOAMIN       (u32)999999  //һ�����µ�ֱ��ǰ���豸�����
#define   MAXNUM_PLATDEVICE_ONEDOAMIN      (u32)999     //һ�����µ�ֱ��ƽ̨�豸�����
#define   MAXNUM_CUSTOMER_ONEDOAMIN        (u32)99999   //һ�����µĿͻ������
#define   MAXNUM_CUSER_ONECUSTOMER         (u32)999     //һ���ͻ��µĿͻ��û������
#define   MAXNUM_CUGROUP_ONECUSTOMER       (u32)999     //һ���ͻ��µĿͻ��û��������
#define   MAXNUM_CDGROUP_ONECUSTOMER       (u32)99999   //һ���ͻ��µĿͻ��豸�������


//2. ����е� ��/�û�/�ͻ�/�豸���ơ��������ض���

#define   MAXLEN_WHOLE_DOMAIN_NAME      (u16)64 //��ȫ������󳤶�
#define   MAXLEN_SHORT_DOMAIN_NAME      (u16)8  //���Ƶ���󳤶�
#define   MAXLEN_CUSTOMER_NAME          (u16)128//�ͻ�������󳤶�(��������)
#define   MAXLEN_DEVICE_NAME            (u16)64 //�豸������󳤶�(��������)
#define   MAXLEN_DGROUP_NAME            (u16)64 //�豸��������󳤶�(��������)
#define   MAXLEN_TUSER_NAME             (u16)64 //�����û�������󳤶�(��������)
#define   MAXLEN_CUSER_NAME             (u16)64 //�ͻ��û�������󳤶�(�����������ͻ���)
#define   MAXLEN_CUGROUP_NAME           (u16)64 //�ͻ��û���������󳤶�(�����������ͻ���)
#define   MAXLEN_APOLICY_NAME           (u16)64 //�ƷѲ���������󳤶�
#define   MAXLEN_HISTORYFILE_NAME       (u16)64 //�Զ�ά���γɵ���ʷ�����ļ�������󳤶�

//��¼��������@�ָ�,����ü��
#define   MAXLEN_DOMAIN_FULLPOSTFIX     (u16)(MAXLEN_SHORT_DOMAIN_NAME+1)*MAXNUM_DOMAIN_LEVEL//���׺����󳤶�(@������List)
#define   MAXLEN_WHOLE_TUSER_NAME       (u16)160//�����û��ĵ�½�û�������󳤶�(TUserName��@������)
#define   MAXLEN_WHOLE_CUSTOMER_NAME    (u16)208//�ͻ��ĵ�½�û�������󳤶�(CustomerName��@������)
#define   MAXLEN_WHOLE_CUSER_NAME       (u16)256//ƫС,�ͻ��û��ĵ�½�û�������󳤶�(CUserName��@�ͻ�����@������)

#define   MAXLEN_PWD                    (u16)32 //����/�ͻ��û��ĵ�½�������󳤶�

#define   MAXLEN_VERSION_STRING         (u16)32 //�汾�ִ�����󳤶�

#define   SEPARATE_CHAR_LOGINNAME       (s8)'@'//��¼���ķָ��ַ�
#define   SEPARATE_CHAR_POSTFIX         (s8)'.'//��¼��׺�ķָ��ַ�

//3. ����е� ����Ļ�����Ϣ����ض���

#define   TUSER_TYPE_OPERATOR           (u8)0   //�����û����ͣ�����Ա
#define   TUSER_TYPE_COMMONADMIN        (u8)1   //�����û����ͣ�����Ա
#define   TUSER_TYPE_SUPERADMIN         (u8)2   //�����û����ͣ���������Ա

#define   CUSER_TYPE_OPERATOR           (u8)0   //�ͻ��û����ͣ�����Ա
#define   CUSER_TYPE_COMMONADMIN        (u8)1   //�ͻ��û����ͣ�����Ա
#define   CUSER_TYPE_SUPERADMIN         (u8)2   //�ͻ��û����ͣ������û�

#define   AUDIOCALL_FLAG_DISABLE        (u8)0   //֧����Ƶ���б�־������
#define   AUDIOCALL_FLAG_ENABLE         (u8)1   //֧����Ƶ���б�־������

#define   ALARMMANAGE_FLAG_DISABLE      (u8)0   //֧���ֳ��澯�����־������
#define   ALARMMANAGE_FLAG_ENABLE       (u8)1   //֧���ֳ��澯�����־������

#define   PARAMCONFIG_FLAG_DISABLE      (u8)0   //֧�ֲ������ñ�־������
#define   PARAMCONFIG_FLAG_ENABLE       (u8)1   //֧�ֲ������ñ�־������

#define   MAXNUM_CASTOMER_AUDIO_CALL    (u8)16  //�ͻ�����Ƶ������󲢷�·��

#define   USER_SEX_MALE                 (u8)0   //�û��Ա���
#define   USER_SEX_FEMALE               (u8)1   //�û��Ա�Ů

#define   USE_FLAG_DISABLE              (u8)0   //��ͣ״̬��־������
#define   USE_FLAG_ENABLE               (u8)1   //��ͣ״̬��־������

#define   MLOGIN_FLAG_DISABLE           (u8)0   //��ε�½��־��������
#define   MLOGIN_FLAG_ENABLE            (u8)1   //��ε�½��־������

#define   FORCEUNIFORM_FLAG_DISABLE     (u8)0   //ǿ���û��������û�Ȩ������һ�±�־����ǿ��
#define   FORCEUNIFORM_FLAG_ENABLE      (u8)1   //ǿ���û��������û�Ȩ������һ�±�־��ǿ��

//2006-03-27 WANCL ADD
#define   IP_NET_RANGE_NO_LIMIT         (u8)0   //��½���Ʋ�����Ч��ʽ��������
#define   IP_NET_RANGE_REFUSED          (u8)1   //��½���Ʋ�����Ч��ʽ����������
#define   IP_NET_RANGE_ALLOWED          (u8)2   //��½���Ʋ�����Ч��ʽ����������

#define   NET_TYPE_LAN                  (u8)0   //������·��ʽ��LAN
#define   NET_TYPE_ADSL                 (u8)1   //������·��ʽ��ADSL
#define   NET_TYPE_NAT                  (u8)2   //������·��ʽ��NAT

#define   MAXLEN_COMPANY_NAME           (u16)128//������λ����󳤶�
#define   MAXLEN_DEPT_NUM               (u16)16 //���ŵ���󳤶�
#define   MAXLEN_TELEPHONE              (u16)32 //��ϵ�绰����󳤶�
#define   MAXLEN_EMAIL                  (u16)32 //�ʼ�����󳤶�
#define   MAXLEN_DESCRIPTION            (u16)256//��������󳤶�
#define   MAXLEN_ADDRESS                (u16)128//סַ����󳤶�
#define   MAXLEN_POSTALCODE             (u16)32 //�ʱ����󳤶�
#define   MAXLEN_REGION                 (u16)32 //�������󳤶�
#define   MAXLEN_LINKMAN                (u16)64 //��ϵ�˵���󳤶�
#define   MAXLEN_FAXCODE                (u16)32 //����������󳤶�
#define   MAXLEN_BANKNAME               (u16)128//�������е���󳤶�
#define   MAXLEN_BANKACCOUNT            (u16)64 //�����ʺŵ���󳤶�

#define   MAXLEN_DEVICE_STATE           (u16)32 //�豸ά��״̬����󳤶�(����/����/�����Զ��塣��)
#define   MAXLEN_DEVICE_NETADDRESS      (u16)32 //�豸�����ַ����󳤶�(IP������)
#define   MAXLEN_DEVICE_FIXADDRESS      (u16)32 //�豸��װλ�õ���󳤶�
#define   MAXLEN_DEVICE_MANUFACTURER    (u16)32 //��������Ϣ����󳤶�
#define   MAXLEN_DEVICE_OWNER           (u16)32 //�豸�ʲ�ӵ������Ϣ����󳤶�
#define   MAXLEN_DEVICE_BUSINESSTYPE    (u16)16 //�豸ҵ��������Ե���󳤶�
#define   MAXLEN_POWERSUPPLY_MODE       (u16)16 //���緽ʽ
#define   MAXLEN_DEVICE_MODEINFO        (u16)16 //�ͺ�
#define   MAXLEN_DEVICE_DEFINITION      (u16)16 //������
#define   MAXLEN_DEVICE_SORT            (u16)16 //���
#define   MAXLEN_FENSE_GRADE            (u16)16 //�����ȼ�
#define   MAXLEN_FIXFORMAT              (u16)32 //��װ��ʽ
#define   MAXLEN_DEVICE_LEVEL           (u16)16 //���
#define   MAXLEN_LOG_CONTENT            (u16)256//��־��������󳤶�

#define   MAXLEN_POLICY_NAME            (u16)64 //�ƷѲ���������󳤶�
#define   MAXLEN_DEVICE_PASSWORD        (u16)64 //�豸������󳤶�

//2006-04-15 WANCL ADD
#define   UNUSED_TIMELINE_NO_LIMIT      (u8)0   //ʹ�ý�ֹʱ���־��������

#define   QUERY_SUBDOMAIN_DISABLE       (u8)0   //��ѯʱ�Ƿ�������򣺲�����
#define   QUERY_SUBDOMAIN_ENABLE        (u8)1   //��ѯʱ�Ƿ�������򣺲�����

//4. ����е� �豸���Ͷ���

#define   DEVICE_TYPE_INVALID           (u8)0   //��Ч�豸, ��Ӧ����

//4. �豸��Ϊǰ���豸��ƽ̨�豸�����ࣺ

#define   DEVICE_TYPE_PRECEDING         NOFIELD_TYPE_PRECEDINGDEVICE  //(u8)1 ǰ���豸
#define   DEVICE_TYPE_PLATFORM          NOFIELD_TYPE_PLATFORMDEVICE   //(u8)2 ƽ̨�豸

//4.1. ƽ̨�豸��Ϊ���漸�ࣺ
#define   PLATFORM_DEVICE_TYPE_VTDU     (u8)1   //����ת��
#define   PLATFORM_DEVICE_TYPE_NRU      (u8)2   //����¼����
#define   PLATFORM_DEVICE_TYPE_VPU      (u8)3   //���渴�� 4??
#define   PLATFORM_DEVICE_TYPE_GAS      (u8)4   //GPSӦ�÷�����
#define   PLATFORM_DEVICE_TYPE_UAS      (u8)5   //CMU�ĸ��Ի����ݷ�����
#define   PLATFORM_DEVICE_TYPE_DNS      (u8)6   //DNS�豸
//wdl
#define   PLATFORM_DEVICE_TYPE_PUI      (u8)7   //PU����
#define   PLATFORM_DEVICE_TYPE_CUI      (u8)8   //CU����
//
// 2009-5-19
#define   PLATFORM_DEVICE_TYPE_EX		(u8)9   //��չ�豸����
// ����ƽ̨�豸ʹ����չ��š���11��ʼ��99��
#define   PLATFORM_DEVICE_TYPE_EX_IMU   (u8)11	// IMU
#define   PLATFORM_DEVICE_TYPE_EX_IAS   (u8)12	// IAS
#define   PLATFORM_DEVICE_TYPE_EX_NPU   (u8)13  // NPU
#define   PLATFORM_DEVICE_TYPE_EX_TAS   (u8)14  // 3AS
#define   PLATFORM_DEVICE_TYPE_EX_MTU   (u8)15  // MTU


//4.2. ǰ���豸��Ϊ���漸�ࣺmain/periphery
#define   PRECEDING_DEVICE_TYPE_MAIN    (u8)1   //���豸����Ƶ�������ȸ�������Ƶ���벢�����紫�͵��豸
#define   PRECEDING_DEVICE_TYPE_PER     (u8)2   //��Χ�豸�����豸���ӵ����������̨���豸

//4.2.1. ���豸��Ϊ���漸�ࣺ
#define   MAIN_DEVICE_TYPE_DVR          (u8)1   //DVR���洢�ı������������ش洢����Ƶ��������
#define   MAIN_DEVICE_TYPE_ENCODER      (u8)2   //DV ���������������ش洢����Ƶ��������
#define   MAIN_DEVICE_TYPE_IPCAMERA     (u8)3   //IP-Camera
#define   MAIN_DEVICE_TYPE_DECODER      (u8)4   //������

//4.2.2. ��Χ�豸��Ϊ���漸�ࣺ
#define   PER_DEVICE_TYPE_VIDICON       (u8)1   //�����
#define   PER_DEVICE_TYPE_YT            (u8)2   //��̨
#define   PER_DEVICE_TYPE_IOIN          (u8)3   //���������豸��������/ģ����������⡢�̸С��Ŵŵ�
#define   PER_DEVICE_TYPE_INOUT         (u8)4   //��������豸:�羯�ơ�����
#define   PER_DEVICE_TYPE_AIN           (u8)5   //���������豸
#define   PER_DEVICE_TYPE_AOUT          (u8)6   //��������豸
#define   PER_DEVICE_TYPE_MATRIX        (u8)7   //����

//5. �豸����ͳһ����
//   (����е��豸���ͷ����㣬������һ��ͳһ���壬���ڽӿ�ͨѶ������
//    3A�б��ϱ�����ͳһ����ָ����һ����)

#define   DEVICE_GLOBAL_TYPE_INVALID    (u8)0   //��Ч�豸

#define   DEVICE_GLOBAL_TYPE_CMU        (u8)1   //CMU

#define   DEVICE_GLOBAL_TYPE_VTDU       (u8)11  //����ת��
#define   DEVICE_GLOBAL_TYPE_NRU        (u8)12  //����¼����
#define   DEVICE_GLOBAL_TYPE_VPU        (u8)13  //���渴��
#define   DEVICE_GLOBAL_TYPE_UAS        (u8)14  //CMU�ĸ��Ի����ݷ�����
#define   DEVICE_GLOBAL_TYPE_PUI        (u8)15  //PU����
#define   DEVICE_GLOBAL_TYPE_CUI        (u8)16  //CU����
#define   DEVICE_GLOBAL_TYPE_NPU        (u8)17
#define	  DEVICE_GLOBAL_TYPE_IMU		(u8)18 
#define	  DEVICE_GLOBAL_TYPE_IAS		(u8)19
#define	  DEVICE_GLOBAL_TYPE_TAS		(u8)20  // 3AS

#define   DEVICE_GLOBAL_TYPE_DVR        (u8)21  //DVR���洢�ı������������ش洢����Ƶ��������
#define   DEVICE_GLOBAL_TYPE_ENCODER    (u8)22  //DV ���������������ش洢����Ƶ��������
#define   DEVICE_GLOBAL_TYPE_IPCAMERA   (u8)23  //IP-Camera
#define   DEVICE_GLOBAL_TYPE_DECODER    (u8)24  //������
#define   DEVICE_GLOBAL_TYPE_DNS        (u8)25  //DNS�豸

#define	  DEVICE_GLOBAL_TYPE_MTU		(u8)26  // MTU // ƽ̨�豸

#define   DEVICE_GLOBAL_TYPE_VIDICON    (u8)31  //�����
#define   DEVICE_GLOBAL_TYPE_YT         (u8)32  //��̨
#define   DEVICE_GLOBAL_TYPE_IOIN       (u8)33  //���������豸��������/ģ����������⡢�̸С��Ŵŵ�
#define   DEVICE_GLOBAL_TYPE_INOUT      (u8)34  //��������豸:�羯�ơ�����
#define   DEVICE_GLOBAL_TYPE_AIN        (u8)35  //���������豸
#define   DEVICE_GLOBAL_TYPE_AOUT       (u8)36  //��������豸
#define   DEVICE_GLOBAL_TYPE_MATRIX     (u8)37  //����

#define   DEVICE_GLOBAL_TYPE_GAS        (u8)41  //GAS


//6. �ͻ��û�/�û����ϵͳȨ�޶��� (Customer.User.System.Privilege)
enum EmCUserSPrivilege
{
	em_StartCUserSPrivilege     = 0, //..

	em_Cms_Vod_Rec              = 1, //ƽ̨¼�� ��һλ
	em_Cms_Vod_Play             = 2, //ƽ̨����
	em_Cms_Vod_Del              = 3, //ƽ̨¼��ɾ��
	em_Cms_Vod_Download         = 4, //ƽ̨¼������
	em_Cms_Vod_Local_Rec        = 5, //����¼�񣬱���ץ��
	em_Cms_Rec_Query_Remote     = 6, //����¼���ѯ

	em_Cms_Call_Ctrl            = 9, //���п���
	em_Cms_Alarm_Manage         = 10,//�ֳ��澯����

	//�ĸ�Bit��ʾ�û��ĸ���Ȩ��
	em_Cms_Route_Pri	    	= 16,   //�û��ĸ���Ȩ��
	em_Cms_Route_Pri_bit2       = 17,   //����Ȩ��Bit2
	em_Cms_Route_Pri_bit3       = 18,   //����Ȩ��Bit3
	em_Cms_Route_Pri_bit4       = 19,   //����Ȩ��Bit4

    em_Cms_Explore              = 20,   //��Ƶ���Ȩ��
    em_Cms_Cycle_Explore        = 21,   //��ѲȨ��

    cm_Cms_DevGroup_Inherit	= 27,	//����ʱ�豸����̳�Ȩ�ޡ�20110624 huangzhichun

	//����λ��ʾUsbkey��½��֤��ʽ
	em_Cms_Login_type			= 29, 
	em_Cms_Login_type_bit2		= 30,
	em_Cms_Login_type_bit3		= 31,//��¼��֤��ʽ,�ӵ�30λ��ʼ����32λ,��3��bitλ,��¼5�ֵ�¼״̬

	em_EndCUserSPrivilege       = 33//..
};

//7. �豸��Ȩ�޶�����Ϣ (Device.Privilege)

//7.1. ǰ���豸��Ȩ�޶�����Ϣ (Preceding.Device.Privilege)

//7.1.1. ������
enum EmPUPrivilege
{
	em_StartPUPrivilege        = 0, //..
	
	em_PU_YJ_Ctrl              = 1, //�ƾ�����(1-10)�� ��һλ��ʼ����ռ��4λ
    em_PU_Audio                = 8, //ǰ����Ƶ
	
	em_PU_Vod_Rec              = 9, //ǰ��¼��
	em_PU_Vod_Play             = 10,//ǰ�˷���
	em_PU_Vod_Del              = 11,//ǰ��¼��ɾ��
	em_PU_Vod_Download         = 12,//ǰ��¼������

	em_PU_Param_Config         = 17,//ǰ�˲�������
	em_PU_IO_Ctrl              = 18,//ǰ�˿������������
	em_PU_Defence_Ctrl         = 19,//����/����
	em_PU_Alarm_Host		   = 20,//�澯����
	
	em_EndPUPrivilege          = 33//..
};

//7.2. ƽ̨�豸��Ȩ�޶�����Ϣ (Platform.Device.Privilege)

//7.2.1. VPU
enum EmVPUPrivilege
{
	em_StartVPUPrivilege        = 0, //..

	em_VPU_Explore              = 1,//VPU���
	em_VPU_Defence_Ctrl         = 2,//VPU����

	em_EndVPUPrivilege          = 33//..
};


//8. ����ҵ��������

#define   QUERY_RESULT_ALL_ENTRY        (u32)0 //��ѯ����Ĳ�ѯ���ؽ�� Ϊ���м�¼

#define   LASTPACK_IN_QRYNTF_FLAG       (u8)1  //��ѯ����иð��Ƿ�Ϊ�������ı�־

#define   MAXENTRYNUM_CMUQRYCUSTOMER_ONEPACK  (u32)36 //CMU��ѯ�ͻ���ͨ��Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYDEVICE_ONEPACK       (u32)20 //��ѯ�豸�б���Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYCUSER_ONEPACK        (u32)12 //��ѯ�ͻ��û��б���Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYDGROUP_ONEPACK       (u32)28 //��ѯ�ͻ��豸���б���Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYDGROUP2DEV_ONEPACK   (u32)50 //��ѯ�ͻ��豸�����豸��ϵ�б���Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYUGROUP_ONEPACK       (u32)28 //��ѯ�ͻ��û����б���Ϣʱ���صĵ�������¼��Ŀ
#define   MAXENTRYNUM_QRYCUDPRIVILEGE_ONEPACK (u32)18//��ѯ�ͻ��û�/�û�����-�豸���-�豸Ȩ����Ϣʱ���صĵ�������¼��Ŀ

#define   QUERY_ALL_DEVICE              (u8)0 //��ѯ����  �豸��¼(�ѷ���+δ����)
#define   QUERY_ASSIGNED_DEVICE         (u8)1 //��ѯ�ѷ����豸��¼(�ѷ���)
#define   QUERY_UNASSIGNED_DEVICE       (u8)2 //��ѯδ�����豸��¼(��ɢ)


//9. �������ַ�������
#define		MANU_STR_DC         (LPCSTR)"dacheng"
#define		MANU_STR_KDC        (LPCSTR)"kedacom"
#define		MANU_STR_HAIKANG    (LPCSTR)"haikang"
#define		MANU_STR_DH			(LPCSTR)"dahua"
#define		MANU_STR_VCOM		(LPCSTR)"vcom"
#define		MANU_STR_SANTACHI	(LPCSTR)"santachi"
#define		MANU_STR_CHUANGSHI  (LPCSTR)"chuangshi"  //����
#define		MANU_STR_DALI       (LPCSTR)"dali"       //����
#define		MANU_STR_ELSE       (LPCSTR)"other"
#define     MANU_STR_GBKDC      (LPCSTR)"GB_kedacom"
#define     MANU_STR_GBOTHER    (LPCSTR)"GB_other"

#define     MANU_STR_GB_PREFIX  (LPCSTR)"GB_"

//10.��Կ�ִ�����(��Ч����16�ֽ�,���øĶ�)
//#define     KDM_EDCRYPT_KEY     (LPCSTR)"**KDM3.9KDM3.9**"

//10.  �û���֤��Ϣ����
#define AUTH_DEFAULT_AES_KEY        (s8 *)"kedacom.shanghai"
//#define AUTH_DEFAULT_AES_MODE       MODE_CBC
#define AUTH_DEFAULT_AES_INITKEY    (s8 *)"2003091919190900"

//11. ����ID��Чֵ
#define REGION_ID_INVALITY         (u32)0    //����ID��Чֵ

//12. �豸�ͺ��ַ�������
#define	MODE_STR_ALARMHOST         (LPCSTR)"AlarmHost"

#endif /* _CONST_KDM_H_ */
//END OF FILE
