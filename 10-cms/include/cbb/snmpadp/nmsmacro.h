/*=======================================================================
ģ����      :
�ļ���      : nmsmacro.h
����ļ�    :
�ļ�ʵ�ֹ��� :
����        : ���ǿ
�汾        : V3.9  Copyright(C) 2003-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2006/02/22  3.9         ���ǿ      ����3.9�������������
=======================================================================*/
#ifndef NMS_MACRO_H
#define NMS_MACRO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "kdmdef.h"
#include "eventid.h"

#define NMS_APP_NAME            "NmsServer 0xbe8e2ce1"
#define NMS_WATCHDOG_APP_NAME   "NmsWatchDog 0xbe8e2ce1"
#define NMS_WATCHDOG_PORT       (u16)7777

//////////////////////////////////////////////////////////////////////////
// NMS���Ź���Ϣ
// ���Ź�ע�����ID
OSPEVENT(EV_WATCHDOG_REG_PROCESSID,             EV_NMS_BGN + 300);
// ���Ź�ע��
OSPEVENT(EV_WATCHDOG_UNREG_NMS,                 EV_NMS_BGN + 302);
// ���Ź���ȡʳ����Ϣ
OSPEVENT(EV_WATCHDOG_ASK_FOOD,                  EV_NMS_BGN + 304);
// NMSι����Ϣ
OSPEVENT(EV_WATCHDOG_FEED_DOG,                  EV_NMS_BGN + 305);
// �������Ź���ʱ��Ϣ
OSPEVENT(EV_WATCHDOG_START_DOG_TIMER,           EV_NMS_BGN + 306);
// ���Ź�ע������ACK��Ϣ��ʱ
OSPEVENT(EV_WATCHDOG_UNREG_ACK_TIMER,           EV_NMS_BGN + 307);
// �ϴ��汾�Ľ��Զ�ʱ
OSPEVENT(EV_UPDATE_PROGRESS_TIMER,              EV_NMS_BGN + 310);
// ���õ�Ԫ��ʱ���Ķ�ʱ
OSPEVENT(EV_PUB_UNIT_TIMER,                     EV_NMS_BGN + 315);
//ȡ���ܷ�����CPU��Ϣ��ʱ��
OSPEVENT(EV_GET_CPUINFO_TIMER,                  EV_NMS_BGN + 318);
//���Ź�ע����Ϣ��
OSPEVENT(EV_WATCHDOG_REG_MSG,                   EV_NMS_BGN + 320);
//���Ź���ʼ���ɹ���Ϣ
OSPEVENT(EV_DOG_INIT_MSG,                       EV_NMS_BGN + 323);
//����topo��Ϣ��ʱ��
OSPEVENT(EV_ASK_TOPO_TIMER,                     EV_NMS_BGN + 325);
#ifdef _PRIVATENET_
//�ڴ����ݿ������ݵ�������ļ���ʱ��
OSPEVENT(EV_LOADOUT_MEMDB_TIMER,                EV_NMS_BGN + 330);
#endif

//ι����app������
#define NMC_APP_TYPE                            (u8)0
#define SVR_APP_TYPE                            (u8)1

//���������Ź���ע��ṹ
typedef struct tagNmsRegDog
{
    tagNmsRegDog()
    {
        m_dwProcessId = 0;
        m_dwFeedInterval = 0;
        m_wStaveCount = 0;
    }
    u32 m_dwProcessId;
    u32 m_dwFeedInterval;
    u16 m_wStaveCount;
    u16 wReserved;
}TNmsRegDog;


#define START_DOG_TIMER_INTERVAL                1000        // ÿ���ӳ������������ӿ��Ź�һ��
#ifdef WIN32
#define DOG_ASK_FOOD_INTERVAL                   10000       // ÿ������Ҫιһ��
#endif
#ifdef _LINUX_
#define DOG_ASK_FOOD_INTERVAL                   10000       // ÿ������Ҫιһ�� LINUX�µȽ�ʱ�䳤һЩ
#endif // _LINUX_
#define MAX_TRY_TIMES_ASK_FOOD                  3           // �����ٴεò���ʳ�������NMS
#define MAX_TIME_FROM_UNREG_ACK                 1000        //ע������ACK��ʱʱ��
#define PUB_UNIT_TIME_INTERVAL                  1000        // ���ö�ʱ��һ��ص�һ��

#define UPDATE_PROGRESS_TIMEOUT                 1000        // ÿ1���ϱ�һ�ν���

#define GET_CPUINFO_TIMEOUT                     3000        // ÿ��3��ȡһ��cpu��Ϣ
#define ASK_TOPO_TIMEOUT                        15000       // 15��ȥ���˳�ʱ

#define MAX_LOADOUT_MEMDB_TIMEOUT               1800000     //ÿ����ʮ���ӱ���һ���ڴ����ݿ�


//��ͻ��˷������ݵ����鳤��
#define ARRAY_DEVICE_LEN        (u32)40
#define ARRAY_USER_LEN          (u32)20
#define ARRAY_ALARM_LEN         (u32)50

// �������
#define SUCESS_VALUE            (u8)0           // �����ɹ�
#define FAILE_VALUE             (u8)1           // ����ʧ��

//�û�������Ϣ����
#define MSG_TYPE_GROUP          (u8)1           // �û�����Ϣ
#define MSG_TYPE_USER           (u8)2           // �û���Ϣ

#define INVALID_INDEX           (u32)0xFFFFFFFF  // ��Ч��������

#define ROOT_PARENTDEVICE_SN    (u32)0           //���ڵ�ĸ��豸��ˮ��

#define NMS_DEVICE_SN           (u32)1           // ���ܷ�����������豸��ˮ��

#define INVALID_SERIAL_NO       (u32)0xFFFFFFFF  // �Ƿ���ID(�����û�,�û���, �豸��������NMS���ɵ���ˮ�ŵ�)
#define SUPER_ADMIN_SN          (u32)0x00000001  // ��������Ա���û���ˮ��
#define SUPER_ADMIN_GROUP_ID    (u32)0x00000000  // ��������Ա���û����



#define MASTER_NMS              (u8)0x1         //������(����3as)
#define SLAVE_NMS               (u8)0x0         //��������

//ϵͳ���е���������
#define PUBLIC_NET              (u8)0x0         //����
#define PRIVATE_NET             (u8)0x1         //ר��


//EMAIL��������֤��ʽ
#define NMS_EMAIL_NO_AUTH               (u8)0   //������֤
#define NMS_EMAIL_AUTH_LOGIN            (u8)1   //login
#define NMS_EMAIL_AUTH_PLAIN            (u8)2   //plain
#define NMS_EMAIL_AUTH_CRAM_MD5         (u8)3   //cram_md5
#define NMS_EMAIL_AUTH_DIGEST_MD5       (u8)4   //digest_md5


// ������
enum
{
    ERROR_CMD_SUCCESS = 0,                  // �ٹ��ɹ�
    // ��¼
    ERROR_NMS_INIT_FAIL,                    // ���ܷ�������ʼ��ʧ��
    ERROR_USER_ID_PWD,                      // ��¼�û������������
    ERROR_IP_RESTRICT,                      // ��¼IP������
    ERROR_EXCEED_MAX_NMC,                   // �ͻ��˵�¼���ﵽ����

    // �û�����
    ERROR_USER_ONLINE,                      // �û����ڵ�¼
    
    ERROR_ADD_GROUP_ALREADY_EXIST,          // �����û����Ѵ���
    ERROR_ADD_GROUP_BEYOND_LIMIT,           // ϵͳ�û������Ѵ�����
    ERROR_MODIFY_GROUP,                     // �޸��û������
    ERROR_DEL_GROUP_NOT_EXIST,              // ɾ���û��鲻����

    ERROR_ADD_USER_ALREADY_EXIST,           // �����û��Ѵ��
    ERROR_ADD_USER_BEYOND_LIMIT,            // ϵͳ�û����Ѿ��ﵽ����
    ERROR_MODIFY_USER,                      // �û������޸ĳ���
    ERROR_CHANGE_PWD,                       // �޸�����ʧ��
    ERROR_DEL_USER_NOT_EXIST,               // ɾ�����û�������

    // ���ù���
    ERROR_GET_DEV_CFG,                      // ��ȡ�豸���ò���ʧ��
    ERROR_SET_DEV_CFG,                      // �����豸���ò���ʧ�� 

    // �澯
    // IP����
    ERROR_ADD_IPRULES_ALREADY_EXIST,        // ��ӵ�IP�����Ѵ���
    ERROR_MODIFY_IPRULES,                   // �޸�IP�������
    ERROR_DEL_IPRULES_NOT_EXIST,            // ɾ��IP���򲻴���

    // �澯���˹���
    ERROR_ADD_ALARM_FILETER_ALREADY_EXIST,  // ��ӵĸ澯���˹����Ѵ���
    ERROR_MODIFY_ALARM_FILETER,             // �޸ĸ澯���˹������
    ERROR_DEL_ALARM_FILETER_NOT_EXIST,      // ɾ���澯���˹��򲻴���

    // �ʼ����͹���
    ERROR_ADD_MAIL_RULES_ALREADY_EXIST,     // ��ӵ��ʼ����͹����Ѵ���
    ERROR_MODIFY_MAIL_RULES,                // �޸��ʼ����͹������
    ERROR_DEL_MAIL_RULES_NOT_EXIST,         // ɾ���ʼ����͹��򲻴���

    ERROR_ALARM_QUERY_NOMATCH,              // û�з��ϲ�ѯ�����ĸ澯
    ERROR_ALARM_QUERY_FAIL,                 // �澯��ѯʧ��

    ERROR_ACTION_QUERY_NOMATCH,             // û�з��ϲ�ѯ�����Ĳ�����־
    ERROR_ACTION_QUERY_FAIL,                // ��־��ѯʧ��

    ERROR_PING_DEVICE_CANNOT_ARRIVE,        //�ﵽ����ҪPing���豸
    ERROR_PING_DEVICE_TIMEOUT,              //ping��ʱ

    //���ݿⱸ����ָ�
    ERROR_BACKUP_DB_PATH_NOTEXIT,               //�����ļ�·��������
    ERROR_RESTORE_DB_FILE_NOTEXIT,              //Ҫ�ָ��������ļ������� 
    ERROR_DBCHARSET_NOMATCH,                 //���ݿ��ַ�����ƥ��
    ERROR_DB_FAULT_OR_BUSY,                     //���ݿ������ʱ

    //ϵͳ����
    ERROR_NMS_START_ERR_OR_FAIL,             //����������ʧ�ܻ����

    //�����豸��
    ERROR_ADD_DEV_GROUP_ALREADY_EXIST,          // �����豸�����Ѵ���
    ERROR_MODIFY_DEV_GROUP_ALREADY_EXIST,      // �޸��豸�����Ѵ���
    
    //�ͻ�����������汾��һ��
    ERROR_SYSTEM_VERSION_ERROR
};


// Session ģ��App ID����
#define NMC_APP_AID                 AID_TNMS_BGN           // CNmcAppIns��ID
#define SERVICE_APP_AID             (AID_TNMS_BGN + 1)     // CServiceAppIns��ID    
#define DEVAGENT_APP_AID            (AID_TNMS_BGN + 2)     // CAgentAppIns��ID
#define DB_MANAGER_APP_AID          (AID_TNMS_BGN + 3)     // CDbAppIns��ID
#define NMS_WATCHDOG_APP_AID        (AID_TNMS_BGN + 4)     // ���Ź�����Ӧ�õ�ID
#define REGDOG_APP_AID              (AID_TNMS_BGN + 5)     // ����ע�ῴ�Ź�Ӧ��ID

#define NMS_DBAGENT_AID             (AID_TNMS_BGN + 20)     // ��ʵӦ��ID


#define SYNC_TIME_LAP               (u8)15                  // ��3ASʱ��ͬ��������������

//////////////////////////////////////////////////////////////////////////
//Ipv4�ĵ�ַ����
const u8 MAX_IPV4_LEN = 16;
//IPv6�ĵ�ַ����
const u8 MAX_IPV6_LEN = 32;

// ���ܷ�����ϵͳ����
const s32 MAX_FIELD_NUM             = 128;      // ��������
const s32 MAX_GROUP_NUM             = 1024;     // ����û�����
const s32 MAX_GROUP_USER_NUM        = 1024;     // ÿ��������û���
const s32 MAX_USER_NUM              = 1024;     // ϵͳ����û���
const s32 MAX_NMC_SESSION_NUM       = 256;      // ���ͬʱ���߿ͻ�������
const s32 MAX_AGENT_CMD_NUM         = 256;      // ���һ�β����豸���������

//�û���Ϣ����
const s32 MAX_USER_ID_LEN           = 32;
const s32 MAX_USER_PWD_LEN          = 32;
const s32 MAX_USER_NAME_LEN         = 10;
const s32 MAX_DEPT_NAME_LEN         = 32;
const s32 MAX_WORK_ID_LEN           = 12;
const s32 MAX_RANK_LEN              = 10;
const s32 MAX_ADDR_LEN              = 32;
const s32 MAX_TELEPHONE_LEN         = 32;
const s32 MAX_MOBILE_LEN            = 32;
const s32 MAX_EMAIL_LEN             = 32;
const s32 MAX_REMARK_LEN            = 128;
const s32 MAX_SMS_LEN               = 32;

//�û�����Ϣ
const s32 MAX_GROUP_NAME_LEN        = 32;
const s32 MAX_GROUP_DESC_LEN        = 128;


// �澯��Ϣ��������
const s32 MAX_ALARM_DESC_LEN        = 255;

// ��־��Ϣ��Ŀ�ĳ���
const s32 MAX_LOG_INFO_LEN          = 128;

// �澯֪ʶ��
const s32 MAX_KNOW_ITEM_DESC_LEN        = 64;
const s32 MAX_KNOW_ITEM_CAUSE_LEN       = 64;
const s32 MAX_KNOW_ITEM_SOLUTION_LEN    = 128;
const s32 MAX_KNOW_ITEM_REMARK_LEN      = 128;


// �豸��������
const s32 MAX_DEVICE_ALIAS_LEN      = 32;

// �������Ƴ���
const s32 MAX_RULES_NAME_LEN            = 32;   // �������Ƴ���
// IP���ʹ���
const s32 MAX_IP_RULES_IPLIST_SIZE      = 32;   // ����IP���ʹ����IP�б��С
const s32 MAX_IP_RULES_IPSEGMENT_SIZE   = 10;   // ����IP���ʹ�����������ζ���
const s32 MAX_IP_RULES_IPZOOM_SIZE      = 5;    // ����IP���ʹ���������������

// �ʼ����͹���
const s32 MAX_MAIL_RULES_DEV_TYPE_SIZE  = 20;       // ����Email�������豸���͵������Ŀ
const s32 MAX_MAIL_RULES_DEV_ID_SIZE    = 15;       // ����Email�������豸ID�������Ŀ
const s32 MAX_MAIL_RULES_DEV_NAME_SIZE  = 15;       // ����Email�������豸�����������Ŀ
const s32 MAX_MAIL_RULES_DEV_IP_SIZE    = 20;       // ����Email�������豸IP�������Ŀ
const s32 MAX_MAIL_RULES_ALARM_CODE_SIZE = 20;  // ����Email�����и澯��������Ŀ
const s32 MAX_MAIL_RULES_ADDR_NUM   = 15;           // ����Email�����з����ʼ�������ַ��
const s32 MAX_MAIL_RULES_NUM            = 256;      // ����Email��������Ŀ
const s32 MAX_SMS_RULES_ADDR_NUM    = 15;           // ����Email�����з��Ͷ��ŵ�����ַ��

// �澯���˹���
const s32 MAX_FILTER_RULES_CODE_NUM         = 20;   // �������˹��������澯����Ŀ
const s32 MAX_FILTER_RULES_SERVICE_TYPE_NUM = 20;   // �������˹��������ҵ��������Ŀ
const s32 MAX_FILTER_RULES_ALIAS_NUM        = 8;    // �������˹���������豸����
const s32 MAX_FILTER_RULES_DEVID_NUM        = 8;    // �������˹���������豸��ID��

// Ȩ��
const s32 MAX_FIELD_RIGHT_NUM   = 8;                // �û������Ȩ������Ŀ
const s32 MAX_DEVICE_RIGHT_NUM  = 20;               // �û������Ȩ������Ŀ


//���ݿ�������
const s32 MAX_DB_NAME_LEN = 16; 
// ���ݿ����������
const s32 MAX_DB_LINK_NUM = MAX_NMC_SESSION_NUM;    

//���ݿ�������
const s32 MAX_BK_FILE_NAME_LEN = 32;            // ���ݿⱸ�ݵ��ļ���ǰ׺��󳤶�   

//ָʾ��״̬����        
const s32 LED_STATUS_LEN  = 24; 

const u8 MAX_DBCONNECT_TIME     = 30;

//�������Ĵ����ļ������Ϣ����
const u16 MAX_FILEPATH_LEN = 256;
//windows message
#define WM_INITAPP_OVER    WM_USER+1//
#define WM_INITAPP_FIN     WM_USER+2//
#define WM_NMSAPP_QUIT     WM_USER+3

// �Ա�
#define GENDER_MALE                 (u8)1       // ����
#define GENDER_FEMALE               (u8)2       // Ů��

// �������
//#define AGENT_HB_TIME             (u32)20     // ��������ʱ��
//#define AGENT_HB_NUM              (u32)3      // �������Ĵ���
#define ROOT_VIRDEV_DEVICE_ID       "11111111111111111111111111111111"      // NMS��Ϊ����ϵͳ�ĸ��ڵ��ID
#define AAA_DEVICE_ID               "11111111111111111111111111111112"      // 3ASָ��������豸ID

//AES������Կ
#define NMS_AES_PUB_KEY         "BBECD8EF49244540"      //������Կ
#define NMS_AES_INIT_KEY        "1111111111111111"                      //��ʼ��Կ
#define SMS_TIME_OUT                    (u8)60          // ���ų�ʱʱ��60s
#endif // NMS_MACRO_H

