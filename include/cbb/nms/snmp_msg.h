/*
���ܣ�������ƽ̨2.0ʱ�ڵ�ҵ����Ϣ
*/
#ifndef _SNMP_MSG_H_
#define _SNMP_MSG_H_
#include "snmpadp.h"
#include <string>
using std::string;

/////////////////////////
#define TNM_MODULE_NAME_LEN         (64)      // �豸���Ƴ���
#define TNM_URI_LEN                 (64)      // ģ��URI
#define TNM_MODULE_ADDR_LEN         (20)      // ģ���ַ
#define TNM_MODULE_RESERVED_LEN     (4)       // ģ����Ϣ�����ֶγ���
#define TNM_DEVALARM_RESERVED_LEN   (7)       // ���ø澯�ṹ�屣���ֶ�
#define TNM_UUID_LEN                (36)      //UUID����
#define TNM_DISK_NAME_LEN           (36)      //����������
#define TNM_MODULE_ID_LEN           (128)     //ģ��ID����
#define TNM_MODULE_IP_LEN           (128)     //ģ��IP����
#define TNM_DEV_NAME_LEN            (128)     //�豸������
#define TNM_CHNID_LEN               (8)       //CHnId����
#define TNM_DEV_ADDR_LEN            (128)     //�豸��ַ����
#define TNM_PARAM_LEN               (128)     //�澯����������

#define KDMCONF_BGN2 (KDMCONF_BGN + 400)

/* 2.0��ʹ�õ��� dwMsgType �ϵ��б����£���Ϣ��ο�snmpadp.h�е�) */
// TRAP_TNM_DEV_ALARM  2.0���µĸ澯�루���ĸ澯����ΪTTnmDevAlarmBase����ʼ����Ϊ20000����ǰ���ú��룺20031
//// TAS�õ��ĸ澯�룺TNM_ALARM_CODE_DISCONNECT_USBKEY��TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY��TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS
//// RCS�õ��ĸ澯�룺TNM_ALARM_CODE_NRU_DISK_ABNORMAL, TNM_ALARM_CODE_NRU_RECORD_TOO_MUCH
//// CMU�õ��ĸ澯��: TNM_ALARM_CODE_CU_TOO_MUCH, TNM_ALARM_CODE_PU_TOO_MUCH, TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH, TNM_ALARM_CODE_MODULE_DISCONNECT

/* PMS�õ��ĸ澯�� TNM_ALARM_CODE_CPU_STATUS TNM_ALARM_CODE_MEMORY_STATUS TNM_ALARM_CODE_DISKFULL_STATUS TNM_ALARM_CODE_PMS_USBBAK_DISK_FULL TNM_ALARM_CODE_DISCONNECT_DATABASE
TNM_ALARM_CODE_MIRROR_EXPORT_FAIL TNM_ALARM_CODE_MIRROR_IMPORT_FAIL TNM_ALARM_CODE_SYSDISK_READONLY TNM_ALARM_CODE_USBBAK_DISK_DAMAGED      */

/* 2.0��ʹ�õ��� dwMsgType �´������б����� */
#define NODE_CMU_PFMINFO2 (KDMCONF_BGN2+1)      // �澯����: TTnmCmuPfmInfo2
#define NODE_CMU_ONLINE_MODULE (KDMCONF_BGN2+2) // �澯���ݣ�u32 ģ����� �� TTnmModuleInfo����
#define NODE_RCS_PFMINFO (KDMCONF_BGN2+3)       // �澯����: TTnmRcsPfmInfo

/* 2.0���µĸ澯���б����� */
#define TNM_ALARM_CODE_NRU_DISK_ABNORMAL         20000  // �澯����: TnmAlarmCodeNruDiskAbnormal
#define TNM_ALARM_CODE_NRU_RECORD_TOO_MUCH       20004  // �澯���ݣ�TnmAlarmCodeNruRecordTooMuch

#define TNM_ALARM_CODE_CU_TOO_MUCH               20001  // �澯���ݣ�TTnmDevAlarmBase
#define TNM_ALARM_CODE_PU_TOO_MUCH               20002  // �澯���ݣ�TTnmDevAlarmBase
#define TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH      20003  // �澯���ݣ�TTnmDevAlarmBase
#define TNM_ALARM_CODE_MODULE_DISCONNECT         20005  // �澯����: TnmAlarmCodeModuleDisconnect

//   add by zgh
#define TNM_ALARM_CODE_TAS_DISCONNECT_USBKEY         TNM_ALARM_CODE_DISCONNECT_USBKEY           //δ������USBKEY
#define TNM_ALARM_CODE_TAS_AT_TERM_REMIND_USBKEY     TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY       //USBKEY��������
#define TNM_ALARM_CODE_TAS_LICENSE_VALIDATE_NOT_PASS TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS   //У�鲻ͨ��
// #define TNM_ALARM_CODE_PMS_CPU_STATUS                TNM_ALARM_CODE_CPU_STATUS          //CPUʹ���ʸ���ָ����ֵ�澯
// #define TNM_ALARM_CODE_PMS_MEMORY_STATUS             TNM_ALARM_CODE_MEMORY_STATUS       //�ڴ�ʹ�ó���ָ����ֵ�澯
// #define TNM_ALARM_CODE_PMS_DISCONNECT_DATABASE       TNM_ALARM_CODE_DISCONNECT_DATABASE //���ݿ�����ʧ��(��)
// #define TNM_ALARM_CODE_VTDU_LOSEPACKET               TNM_ALARM_CODE_LOSEPACKET          //����
// #define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         TNM_ALARM_CODE_RECV_NOBITSTREAM    //����������
// #define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           TNM_ALARM_CODE_ETHCARD_RESTORE     //����ǧ�װ��ײ���
// #define TNM_ALARM_CODE_RCS_DISK_OFFLINE              TNM_ALARM_CODE_DISK_OFFLINE        //��������
// #define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          TNM_ALARM_CODE_DISK_PART_R_ONLY    //���̷���ֻ��

#define TNM_ALARM_CODE_PMS_USBBAK_DISK_NOT_EXIST     20006  // �澯���ݣ�TTnmDevAlarmBase  �����̶�ʧ
#define TNM_ALARM_CODE_PMS_USBBAK_DISK_FULL          20007  // �澯���ݣ�TTnmDevAlarmBase  ��������
#define TNM_ALARM_CODE_PMS_OPT_DISKFULL_STATUS       20008  // �澯���ݣ�TTnmDevAlarmBase  ҵ������
#define TNM_ALARM_CODE_PMS_DB_DISKFULL_STATUS        20009  // �澯���ݣ�TTnmDevAlarmBase  ��������
#define TNM_ALARM_CODE_PMS_LOG_DISKFULL_STATUS       20010  // �澯���ݣ�TTnmDevAlarmBase  ��־����
#define TNM_ALARM_CODE_PMS_OPT_DISK_PART_R_ONLY      20011  // �澯���ݣ�TTnmDevAlarmBase  ҵ���̱�Ϊֻ��
#define TNM_ALARM_CODE_PMS_DB_DISK_PART_R_ONLY       20012  // �澯���ݣ�TTnmDevAlarmBase  �����̱�Ϊֻ��
#define TNM_ALARM_CODE_PMS_LOG_DISK_PART_R_ONLY      20013  // �澯���ݣ�TTnmDevAlarmBase  ��־�̱�Ϊֻ��
#define TNM_ALARM_CODE_PMS_USBKEY_DISK_PART_R_ONLY   20014  // �澯���ݣ�TTnmDevAlarmBase  �����̱�Ϊֻ��
#define TNM_ALARM_CODE_PMS_MIRROR_EXPORT_FAIL        20015  // �澯���ݣ�TTnmDevAlarmBase  ��ʱ��������ʧ��
#define TNM_ALARM_CODE_PMS_MIRROR_IMPORT_FAIL        20016  // �澯���ݣ�TTnmDevAlarmBase  �������쳣
#define TNM_ALARM_CODE_PMS_USBBAK_DISK_DAMAGED       20017  // �澯���ݣ�TTnmDevAlarmBase  ��������
#define TNM_ALARM_CODE_PMS_DISK_IO_STATE             20018  // �澯���ݣ�TTnmDevAlarmBase  ����IO ����ֵ
#define TNM_ALARM_CODE_PMS_NETCARD_IO_STATE          20019  // �澯���ݣ�TTnmDevAlarmBase  ���������� ����ֵ
#define TNM_ALARM_CODE_RCS_DICK_FULL                 20020  // �澯���ݣ�TTnmDevAlarmBase  �洢�ռ���
#define TNM_ALARM_CODE_RCS_RUNNING_ERR               20021  // �澯���ݣ�TTnmDevAlarmBase  ����״̬�쳣
#define TNM_ALARM_CODE_CMU_PU_OFFLINE                20022  // �澯���ݣ�TTnmDevAlarmBase  ǰ������

#define TNM_ALARM_CODE_TAS_ADDPLTFRM_LOOP            20024  // �澯���ݣ�TTnmDevAlarmBase  �����ɻ�
#define TNM_ALARM_CODE_RMS_DEL_VIDEO                 20025  // �澯���ݣ�TTnmDevAlarmBase  RMS��С�ļ�ɾ��¼���ļ�
#define TNM_ALARM_CODE_PMS_NETCARD_OFFLINE           20026  // �澯���ݣ�TTnmDevAlarmBase  ������������������
#define TNM_ALARM_CODE_PMS_TIME_ERROR                20027  // �澯���ݣ�TTnmDevAlarmBase  ƽ̨ʱ�����
#define TNM_ALARM_CODE_PMS_CPU_STATUS                20028  //CPUʹ���ʸ���ָ����ֵ�澯
#define TNM_ALARM_CODE_PMS_MEMORY_STATUS             20029  //�ڴ�ʹ�ó���ָ����ֵ�澯
#define TNM_ALARM_CODE_PMS_DISCONNECT_DATABASE       20030  //���ݿ�����ʧ��(��)
#define TNM_ALARM_CODE_VTDU_LOSEPACKET               22031  //����
#define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         22032  //����������
#define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           22033  //����ǧ�װ��ײ���
#define TNM_ALARM_CODE_RCS_DISK_OFFLINE              22034  //��������
#define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          22035  //���̷���ֻ��

#define TNM_ALARM_CODE_VTDU_STREAM_CHANGING          20023  // �澯���ݣ�TnmAlarmCodeStreamErr  ������
#define TNM_ALARM_CODE_VTDU_LOSEPACKET               22031  //����
#define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         22032  //����������
#define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           22033  //����ǧ�װ��ײ���
#define TNM_ALARM_CODE_RCS_DISK_OFFLINE              22034  //��������
#define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          22035  //���̷���ֻ��
#define TNM_ALARM_CODE_GPS_STORE_CONFIG_ERROR        22036  //����GPS�켣�洢δ����
#define TNM_ALARM_CODE_DOMAIN_ALIAS_NOT_VAILD        20037  //CUI1->nms, ��������Ϸ�
#define TNM_ALARM_CODE_DOMAIN_RELATION_NOT_LEGAL     20038  //cui1->nms,���ϵ���Ϸ�
#define TNM_ALARM_CODE_SUBDOMAIN_DIFF_DEVICE_KDMNO   20039  //cui1->nms,�¼��������Ϣ��KDMNO��ƥ��
#define TNM_ALARM_CODE_DEVICE_TYPE_ERROR             20040  //�豸���Ͳ�ƥ��
#define TNM_ALARM_CODE_MANAGE_LICENSE_TOO_MUCH       20041  //�澯���ݣ�TTnmDevAlarmBase ����license����
#define TNM_ALARM_CODE_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS     20042 //�澯���ݣ� TTnmDevAlarmBase  key��ȡ����У��ʧ��
#define TNM_ALARM_CODE_OTHER_STORAGE_OVER_LICENSE_LIMIT          20043 //�澯���ݣ�����

typedef struct tagTTnmModuleInfo
{
    tagTTnmModuleInfo()
    {
        memset(this, 0, sizeof(*this));
    }

    s8 name[TNM_MODULE_NAME_LEN];     // ģ������
    s8 address[TNM_MODULE_ADDR_LEN];  // ģ���ַ
    s8 uri[TNM_URI_LEN];      // ģ��uri
    s8 reserved[TNM_MODULE_RESERVED_LEN];  // �����ֶΣ����ڶ���
}PACKED TTnmModuleInfo;

typedef struct tagTnmCmuPfmInfo2
{
    tagTnmCmuPfmInfo2()
    {
        memset(this, 0, sizeof(*this));
    }

    u32 switchNum;           // ת������·��
    u32 switchTotalBitRate;  // ת��������
    u32 onlineVideosourceNum;// ������ƵԴ����
    u32 onlineCuNum;         // ����CU����
}PACKED TTnmCmuPfmInfo2;

typedef struct tagTTnmRcsPfmInfo
{
    tagTTnmRcsPfmInfo()
    {
        memset(this, 0, sizeof(*this));
    }

    u32 recTaskNum;  // ¼������·��
    u32 recBitRate;  // ¼��������
    u32 recVodNum;   // ¼��㲥·��
    u32 recVodTotalBitRate; // ¼��㲥������
}PACKED TTnmRcsPfmInfo;

struct TTnmDevAlarmBase
{
public:
    TTnmDevAlarmBase()
    {
        alarmStatus = TNM_STATUS_ALARM;
        memset(reserved, 0, sizeof(reserved));
    }

    TTnmDevAlarm tDevAlarm;
    u8 alarmStatus; // ֵΪTNM_STATUS_ALARM��TNM_STATUS_NORMAL
    u8 reserved[TNM_DEVALARM_RESERVED_LEN]; // �����ֶ�

    void NetToHostBase()
    {
        tDevAlarm.NetToHost();
    }
    void HostToNetBase()
    {
        tDevAlarm.HostToNet();
    }
} PACKED;

struct TnmAlarmCodeNruDiskAbnormal: public TTnmDevAlarmBase
{
    TnmAlarmCodeNruDiskAbnormal()
    {
        nruName[0] = '\0';
        nruUUID[0] = '\0';
        abnormalDiskName[0] = '\0';
    }
    void NetToHost()
    {
        NetToHostBase();
    }
    void HostToNet()
    {
        HostToNetBase();
    }

    s8 nruName[TNM_MODULE_NAME_LEN];
    s8 nruUUID[TNM_UUID_LEN];
    s8 abnormalDiskName[TNM_DISK_NAME_LEN];
} PACKED;

struct TnmAlarmCodeNruRecordTooMuch: public TTnmDevAlarmBase
{
    TnmAlarmCodeNruRecordTooMuch()
    {
        nruName[0] = '\0';
        nruUUID[0] = '\0';
    }
    void NetToHost()
    {
        NetToHostBase();
    }
    void HostToNet()
    {
        HostToNetBase();
    }

    s8 nruName[TNM_MODULE_NAME_LEN];
    s8 nruUUID[TNM_UUID_LEN];
} PACKED;

typedef TnmAlarmCodeNruRecordTooMuch TnmNruAlarm;

struct TnmAlarmCodeModuleDisconnect: public TTnmDevAlarmBase
{
    TnmAlarmCodeModuleDisconnect()
    {
        moduleName[0] = '\0';
        moduleId[0] = '\0';
        moduleIp[0] = '\0';
    }
    void NetToHost()
    {
        NetToHostBase();
    }
    void HostToNet()
    {
        HostToNetBase();
    }

    s8 moduleName[TNM_MODULE_NAME_LEN];
    s8 moduleId[TNM_MODULE_ID_LEN];
    s8 moduleIp[TNM_MODULE_IP_LEN];
} PACKED;

struct TnmAlarmCodeStreamErr: public TTnmDevAlarmBase
{
    TnmAlarmCodeStreamErr()
    {
        memset(m_szDevURI, 0, sizeof(m_szDevURI));
        memset(m_szDevName, 0, sizeof(m_szDevName));
        memset(m_szChnId, 0, sizeof(m_szChnId));
        memset(m_szDevAddr, 0, sizeof(m_szDevAddr));
        memset(m_szParam, 0, sizeof(m_szParam));
    }
    void NetToHost()
    {
        NetToHostBase();
    }
    void HostToNet()
    {
        HostToNetBase();
    }

    s8 m_szDevURI[TNM_URI_LEN+1];
    s8 m_szDevName[TNM_DEV_NAME_LEN+1];
    s8 m_szChnId[TNM_CHNID_LEN+1];
    s8 m_szDevAddr[TNM_DEV_ADDR_LEN+1];   //�����ĵ�ַ
    /*
        ���������ַ���Ϊ��
        ��������Ϊ�쳣��ַ��
        �����������ٷֱ���ֵ��intתstring������%
    */
    s8 m_szParam[TNM_PARAM_LEN+1];
} PACKED;

//cui1�����ܸ澯��Ϣ����

struct TnmAlarmCodeDomainAliasInvaild: public TTnmDevAlarmBase
{
	TnmAlarmCodeDomainAliasInvaild()
	{
		domainName[0] = '\0';
		domainAlias[0] = '\0';
		subErrCode = 0;
		tDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DOMAIN_ALIAS_NOT_VAILD;
	}
	void NetToHost()
	{
		NetToHostBase();

	}
	void HostToNet()
	{
		HostToNetBase();
	}

	s8 domainName[TNM_UUID_LEN];
	s8 domainAlias[TNM_UUID_LEN];
	u8 subErrCode;
} PACKED;


struct TnmAlarmCodeDomainRelationNotLegal: public TTnmDevAlarmBase
{
	TnmAlarmCodeDomainRelationNotLegal()
	{
		domainName[0] = '\0';
		domainAlias[0] = '\0';
		tDevAlarm.dwAlarmCode = TNM_ALARM_CODE_DOMAIN_RELATION_NOT_LEGAL;
	}
	void NetToHost()
	{
		NetToHostBase();

	}
	void HostToNet()
	{
		HostToNetBase();
	}

	s8 domainName[TNM_UUID_LEN];
	s8 domainAlias[TNM_UUID_LEN];
} PACKED;


struct TnmAlarmCodeSubdomainDiffDeviceKdmNo: public TTnmDevAlarmBase
{
	TnmAlarmCodeSubdomainDiffDeviceKdmNo()
	{
		domainName[0] = '\0';
		domainAlias[0] = '\0';
		deviceID[0] = '\0';
		devKDMNO.SetNull();
		tDevAlarm.dwAlarmCode = TNM_ALARM_CODE_SUBDOMAIN_DIFF_DEVICE_KDMNO;
	}
	void NetToHost()
	{
		NetToHostBase();

	}
	void HostToNet()
	{
		HostToNetBase();
	}

	s8 domainName[TNM_UUID_LEN];
	s8 domainAlias[TNM_UUID_LEN];
	s8 deviceID[TNM_UUID_LEN];
	TTnmDeviceId devKDMNO;
} PACKED;


/* ƽ̨��Ԫ�����ͳ��� */
typedef string TNmcSubType;
#define NMC_SUBTYPE_TAS       "TAS"
#define NMC_SUBTYPE_UAS       "UAS"
#define NMC_SUBTYPE_CMU       "CMU"
#define NMC_SUBTYPE_CUI       "CUI"
#define NMC_SUBTYPE_PUI       "PUI"
#define NMC_SUBTYPE_SIPPROXY  "SIPPROXY"
#define NMC_SUBTYPE_PMS       "PMS"
#define NMC_SUBTYPE_PMCS      "PMCS"
#define NMC_SUBTYPE_VTDU      "VTDU"
#define NMC_SUBTYPE_NRU       "NRU"
#define NMC_SUBTYPE_RCS       "RCS"
#define NMC_SUBTYPE_GBS       "GBS"
#define NMC_SUBTYPE_CUIV1     "CUI1.0"
#define NMC_SUBTYPE_GBU       "GBU"
#endif
