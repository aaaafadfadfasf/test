/************************************************************************
ģ����      : dau
�ļ���      : alarmcode.h
����ļ�    :
ʵ�ֹ���    : �澯�����䶨��
����        : ������
��Ȩ        : <Copyright(C) 2016-2026 Suzhou Keda Technology Co., Ltd. All rights reserved.>
----------------------------------------------------------------------
�޸ļ�¼��
�� ��		�汾        �޸���      �߶���          �޸ļ�¼
2016/07/20  V1.0        ������                      ����
************************************************************************/
#ifndef _DAU_ALARMCODE_H_
#define _DAU_ALARMCODE_H_

#define INVALID_ALARM_CODE      0
#define ALARMCODE_BASE          10000

/************************************************************************/
/* ����澯�����䶨��                                                   */
/************************************************************************/
//common�澯������     (10001 - 20000)
#define COMMON_ALARM_BEGIN      (ALARMCODE_BASE+1)
#define COMMON_ALARM_END        (COMMON_ALARM_BEGIN+9999)

//ƽ̨�澯������       (20001 - 30000)
#define PLAT_ALARM_BEGIN        (COMMON_ALARM_END+1)
#define PLAT_ALARM_END          (PLAT_ALARM_BEGIN+9999)

//ǰ�˸澯������       (30001 - 40000)
#define PU_ALARM_BEGIN          (PLAT_ALARM_END+1)
#define PU_ALARM_END            (PU_ALARM_BEGIN+9999)

//�洢�澯������       (40001 - 50000)
#define STORAGE_ALARM_BEGIN     (PU_ALARM_END+1)
#define STORAGE_ALARM_END       (STORAGE_ALARM_BEGIN+9999)

//���������澯�����䶨��

// �澯״̬
enum EMAlarmStatus
{
	ALARM_RESTORE = 0,	// �澯�ָ�
	ALARM_OCCUR,		// �澯����
};

/************************************************************************/
/* common�澯�붨��                                                     */
/************************************************************************/
enum EMCommonAlarmCode
{
    ALARM_COMMON_CPU_STATUS = COMMON_ALARM_BEGIN,               // CPUʹ���ʸ���ָ����ֵ�澯
    ALARM_COMMON_MEMORY_STATUS,                                 // �ڴ�ʹ�ó���ָ����ֵ�澯
    ALARM_COMMON_DEV_ONLINE,                                    // �豸����
    ALARM_COMMON_DEV_OFFLINE,                                   // �豸����
    ALARM_COMMON_POWER_STATUS,                                  // ��Դ��ѹ����ָ����ֵ�澯
    ALARM_COMMON_FAN_STATUS,                                    // ����ת�ٹ��͸澯
    ALARM_COMMON_ETHCARD_RESTORE,                               // ��̫���ָ��澯
    ALARM_COMMON_LED_STATUS,                                    // ���ƿ�����������״̬�����ı�
    ALARM_COMMON_SAME_REG_ID,                                   // �����豸��ID���뱾�豸��ͬ��ͬʱ��NMSע��
    ALARM_COMMON_DISKDETECT_ERROR,                              // �豸����޴��̻�û�д��̿ɹ����澯
    ALARM_COMMON_NODISK,                                        // ��Ӳ��
    ALARM_COMMON_DISK_ERROR,                                    // Ӳ�̷�����
    ALARM_COMMON_DISK_NOFORMAT,                                 // Ӳ�̷���δ��ʽ��
    ALARM_COMMON_DISK_PART_R_ONLY,                              // ���̷���ֻ��
    ALARM_COMMON_DISK_FS_ERROR,                                 // �����ļ�ϵͳ����
    ALARM_COMMON_DISK_OFFLINE,                                  // ��������
    ALARM_COMMON_STORAGE_ABNORMAL,                              // �������쳣
	ALARM_COMMON_FILEBEAT_ERROR,                             //filebeat�쳣

    ALARM_COMMON_MAX_ALARM_CODE
};


/************************************************************************/
/* ƽ̨�澯��                                                           */
/************************************************************************/
/*����������
    1��common
    2��3as
    3��cmu
    4��pui
    5��cui
    6��nru
    7��vtdu
    8��uas
    9��gas
    10��mtu
    11��dns
*/
// ƽ̨common�澯������    (20001 - 20200)
#define PLAT_COMMON_ALARM_BEGIN         (PLAT_ALARM_BEGIN ) 
#define PLAT_COMMON_ALARM_END           (PLAT_COMMON_ALARM_BEGIN + 199)

// 3as�澯������           (20201 - 20400)
#define TAS_ALARM_BEGIN                 (PLAT_COMMON_ALARM_END + 1 ) 
#define TAS_ALARM_END                   (TAS_ALARM_BEGIN + 199)

// CMU�澯������           (20401 - 20600)
#define CMU_ALARM_BEGIN                 (TAS_ALARM_END + 1) 
#define CMU_ALARM_END                   (CMU_ALARM_BEGIN + 199)

// PUI�澯������           (20601 - 20800)
#define PUI_ALARM_BEGIN                 (CMU_ALARM_END + 1) 
#define PUI_ALARM_END                   (PUI_ALARM_BEGIN + 199)

// CUI�澯������           (20801 - 21000)
#define CUI_ALARM_BEGIN                 (PUI_ALARM_END + 1) 
#define CUI_ALARM_END                   (CUI_ALARM_BEGIN + 199)

// NRU�澯������           (21001 - 21200)
#define NRU_ALARM_BEGIN                 (CUI_ALARM_END + 1) 
#define NRU_ALARM_END                   (NRU_ALARM_BEGIN + 199)

// VTDU�澯������          (21201 - 21400)
#define VTDU_ALARM_BEGIN                (NRU_ALARM_END + 1) 
#define VTDU_ALARM_END                  (VTDU_ALARM_BEGIN + 199)

// UAS�澯������           (21401 - 21600)
#define UAS_ALARM_BEGIN                 (VTDU_ALARM_END + 1) 
#define UAS_ALARM_END                   (UAS_ALARM_BEGIN + 199)

// GAS�澯������           (21601 - 21800)
#define GAS_ALARM_BEGIN                 (UAS_ALARM_END + 1) 
#define GAS_ALARM_END                   (GAS_ALARM_BEGIN + 199)

// MTU�澯������           (21801 - 22000)
#define MTU_ALARM_BEGIN                 (GAS_ALARM_END + 1) 
#define MTU_ALARM_END                   (MTU_ALARM_BEGIN + 199)

// DNS�澯������           (22001 - 22200)
#define DNS_ALARM_BEGIN                 (MTU_ALARM_END + 1) 
#define DNS_ALARM_END                   (DNS_ALARM_BEGIN + 199)

//////////////////////////////////////////////////////////////////////////
//���¶���ƽ̨����ظ澯
//ƽ̨common�澯
enum EMPlatCommonAlarmCode
{
    ALARM_PLAT_COMMON_DISCONNECT_CMU = PLAT_COMMON_ALARM_BEGIN,     // CUIδ������CMU
    ALARM_PLAT_COMMON_PLATFORM_IN_REPLACE_MODE,                     // ƽ̨�����ڱ����滻ģʽ
    ALARM_PLAT_COMMON_DETECT_NULL_IP_NIC,                           // ̽�⵽��IP������
    ALARM_PLAT_COMMON_PLATFORM_DISK_NUM_OVERLOAD,                   // ƽ̨�������̷�����������¼���֧�ֵ�������
    ALARM_PLAT_COMMON_DETECT_USELESS_NIC,                           // ��������������Ч����
    ALARM_PLAT_COMMON_BACKUP_USB_DISCONNECT,                        // ������δ����
    ALARM_PLAT_COMMON_BACKUP_PART_CONFLICT,                         // ���ڶ�����ݷ��������ֶ�ѡ����ء�
    ALARM_PLAT_COMMON_DATABASE_PART_CONFLICT,                       // ���ڶ�����ݿ���������ֶ�ѡ����ء�
    ALARM_PLAT_COMMON_NONE_MOUNTED_DATABASE_PART,                   // û�й������ݿ����
    ALARM_PLAT_COMMON_PLATFORM_DISK_SPACE_OVERLOAD,                 // ƽ̨�������̷����ܴ�С����֧�ֵ����ֵ
    ALARM_PLAT_COMMON_PMS_DISKFULL_STATUS,                          // pms�Լ�����Ĵ������澯
    ALARM_PLAT_COMMON_REG_CMU,                                      // PU������ģ��ע��CMUʧ�ܸ澯
    ALARM_PLAT_CONMON_CODE_MAP_STATUS,                              // MAP�����쳣�澯
    //ALARM_PLAT_COMMON_DISCONNECT_DATABASE,                          // δ���������ݿ�

	ALARM_PLAT_COMMON_PUI_DISCONNECT_CMU,							// PUIδ������CMU
	ALARM_PLAT_COMMON_VTDU_DISCONNECT_CMU,							// VTDUδ������CMU
	ALARM_PLAT_COMMON_NRU_DISCONNECT_CMU,							// NRUδ������CMU
	ALARM_PLAT_COMMON_PMS_DISCONNECT_CMU,							// PMSδ������CMU

	ALARM_PLAT_COMMON_3AS_OFFLINE,									// 3AS����
	ALARM_PLAT_COMMON_UAS_OFFLINE,									// UAS����
	ALARM_PLAT_COMMON_CMU_OFFLINE,									// CMU����
	ALARM_PLAT_COMMON_CUI_OFFLINE,									// CUI����
	ALARM_PLAT_COMMON_PUI_OFFLINE,									// PUI����
	ALARM_PLAT_COMMON_VTDU_OFFLINE,									// VTDU����
	ALARM_PLAT_COMMON_NRU_OFFLINE,									// NRU����
	ALARM_PLAT_COMMON_TVS_OFFLINE,									// TVS����
	ALARM_PLAT_COMMON_MPS_OFFLINE,									// MPS����
	ALARM_PLAT_COMMON_RCS_OFFLINE,									// RCS����
	ALARM_PLAT_COMMON_ALS_OFFLINE,									// ALS����
    ALARM_PLAT_COMMON_PLATFORM_OFFLINE,                             // PLATFORM����
    
    ALARM_PLAT_COMMON_MAX_ALARM_CODE
};
//3as�澯��
enum EMPlatTasAlarmCode
{    
    ALARM_TAS_DISCONNECT_USBKEY = TAS_ALARM_BEGIN,              // δ������USBKEY
    ALARM_TAS_AT_TERM_REMIND_USBKEY,                            // USB��������
    ALARM_TAS_EXPIRED_USBKEY,                                   // ���֤�ѹ���
    ALARM_TAS_LICENSE_PU_NUM_OVERFLOW,                          // ǰ�˳�license����
    ALARM_TAS_LICENSE_OTHER_MANUFACTURER_PU_NUM_OVERFLOW,       // �⳧��ǰ�˳�license����
	ALARM_TAS_DISCONNECT_DATABASE,							  // 3asδ���������ݿ�

    ALARM_TAS_MAX_ALARM_CODE
};

//cmu�澯��
enum EMPlatCmuAlarmCode
{
    ALARM_CMU_DISCONNECT_GAS = CMU_ALARM_BEGIN,                 // δ������GAS
    ALARM_CMU_DISCONNECT_3AS,                                   // δ������3AS
    ALARM_CMU_DISCONNECT_UAS,                                   // δ������UAS
    ALARM_CMU_DISCONNECT_PUI,                                   // δ������PUI
    ALARM_CMU_DISCONNECT_CUI,                                   // δ������CUI
    ALARM_CMU_DISCONNECT_NRU,                                   // δ������NRU
    ALARM_CMU_DISCONNECT_VTDU,                                  // δ������VTDU
    ALARM_CMU_DISCONNECT_SUPER_CMU,                             // δ�������ϼ�CMU
    ALARM_CMU_DISCONNECT_SUB_CMU,                               // �¼�CMU�Ͽ�����
    ALARM_CMU_DISCONNECT_BCMU_CMU,                              // δ������ƽ��CMU
    ALARM_CMU_NRU_NET_SEG_DIFFERENT_FROM_CMU,                   // Nru��Cmu ���β�һ��
    ALARM_CMU_VTDU_NET_SEG_DIFFERENT_FROM_CMU,                  // vtdu ��cmu���β�һ��
    ALARM_CMU_VIDEONUMBER_OVERLOAD,                             // ��ƵԴ��������ֵ

    ALARM_CMU_MAX_ALARM_CODE
};

//PUI�澯��
enum EMPlatPuiAlarmCode
{
    ALARM_PUI_PU_OVERLOAD = PUI_ALARM_BEGIN,                    // ��¼PU��������ֵ
    ALARM_PUI_STOPREC_LOGERR,                                   // ǰ��ͣ¼ - ��־ϵͳ��ͣ¼
    ALARM_PUI_STOPREC_DISKERR,                                  // ǰ��ͣ¼ - Ӳ�̻�ͣ¼
    ALARM_PUI_MOTIONDETECT_STATUS,                              // �ƶ����澯
    ALARM_PUI_LPT_STATUS,                                       // ���ڸ澯�ϱ�
    ALARM_PUI_LOSEVIDEOSRC_STATUS,                              // ǰ���豸��ƵԴ��ʧ�澯

    ALARM_PUI_MAX_ALARM_CODE
};

//CUI�澯��
enum EMPlatCuiAlarmCode
{
    ALARM_CUI_CU_OVERLOAD = CUI_ALARM_BEGIN,                    // ��¼CU��������ֵ
    ALARM_CUI_CMU_CU_ALARM_LOSEPACKET,                          // CUI�����澯
    ALARM_CUI_CMU_CU_ALARM_REQUISTION,                          // CUI�����쳣�澯

    ALARM_CUI_MAX_ALARM_CODE
};

//NRU�澯��
enum EMPlatNruAlarmCode
{
    ALARM_NRU_DISKFULL_STATUS = NRU_ALARM_BEGIN,                // �������澯
    ALARM_NRU_RAID_DISK_OFFLINE,                                // ����Ӳ���𻵻��ȱ
    ALARM_NRU_RAID_DISK_HEALTH_BAD,                             // ����Ӳ�̽���״̬�쳣
    ALARM_NRU_RAID_DISKGROUP_STATUS_ABNORMAL,                   // ���д�����״̬������
    ALARM_NRU_RAID_ISCSI_DROPPED,                               // ����ISCSI����
    ALARM_NRU_RAID_ABNORMAL,                                    // �����쳣
    ALARM_NRU_PLATFORM_VBFS_DISK_SPACE_OVERLOAD,                // vbfs����

    ALARM_NRU_MAX_ALARM_CODE
};

//VTDU�澯��
enum EMPlatVtduAlarmCode
{
    ALARM_VTDU_RECV_NOBITSTREAM = VTDU_ALARM_BEGIN,             // ����������
    ALARM_VTDU_LOSEPACKET,                                      // �������ն�������ָ����ֵ�澯
    ALARM_VTDU_OVERLOAD,                                        // VTDUת��·��������ֵ

    ALARM_VTDU_MAX_ALARM_CODE
};

//uas�澯��
enum EMPlatUasAlarmCode
{
    ALARM_UAS_DISCONNECT_DATABASE = UAS_ALARM_BEGIN,            // UASδ���������ݿ�

    ALARM_UAS_MAX_ALARM_CODE = UAS_ALARM_BEGIN,
};

//������ƽ̨����ظ澯
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* ǰ�˸澯��                                                           */
/************************************************************************/
/*����������
    1��IPC
    2��NVR
    3��SVR
    4��Decoder
    5��Encoder
*/
// ǰ��common�澯������    (30001 - 30200)
#define PU_COMMON_ALARM_BEGIN           (PU_ALARM_BEGIN ) 
#define PU_COMMON_ALARM_END             (PU_COMMON_ALARM_BEGIN + 199)

// IPC�澯������           (30201 - 30400)
#define IPC_ALARM_BEGIN                 (PU_COMMON_ALARM_END +1) 
#define IPC_ALARM_END                   (IPC_ALARM_BEGIN + 199)

// NVR�澯������           (30401 - 30600)
#define NVR_ALARM_BEGIN                 (IPC_ALARM_END + 1) 
#define NVR_ALARM_END                   (NVR_ALARM_BEGIN + 199)

// SVR�澯������           (30601 - 30800)
#define SVR_ALARM_BEGIN                 (NVR_ALARM_END + 1) 
#define SVR_ALARM_END                   (SVR_ALARM_BEGIN + 199)

// Decoder�澯������       (30801 - 31000)
#define DEC_ALARM_BEGIN                 (SVR_ALARM_END + 1) 
#define DEC_ALARM_END                   (DEC_ALARM_BEGIN + 199)

// Encoder�澯������       (31001 - 31200)
#define ENC_ALARM_BEGIN                 (DEC_ALARM_END + 1) 
#define ENC_ALARM_END                   (ENC_ALARM_BEGIN + 199)

//////////////////////////////////////////////////////////////////////////
//���¶���ǰ������ظ澯
//pu common�澯
enum EMPuCommonAlarmCode
{

    ALARM_PU_COMMON_MAX_ALARM_CODE
};

enum EMPuIpcAlarmCode
{
    ALARM_IPC_FUZZY = IPC_ALARM_BEGIN,                      // IPC��Ƶģ���澯
    ALARM_IPC_BRIGHT,                                       // IPC��Ƶ�����澯
    ALARM_IPC_DARK,                                         // IPC��Ƶ�����澯
    ALARM_IPC_SHADE,                                        // IPCͼ���ڵ��澯
    ALARM_IPC_PARTIAL,                                      // IPCͼ��ƫɫ�澯
    ALARM_IPC_MOVING,                                       // IPC�ƶ��澯
    ALARM_IPC_PIN,                                          // IPC���ڸ澯
    ALARM_IPC_DEFOCUS,                                      // IPC�齹���澯
    ALARM_IPC_CORDON,                                       // IPC�����߸澯
    ALARM_IPC_SECENECHANGE,                                 // IPC�����任�澯
    ALARM_IPC_REGIONINVASION,                               // IPC�������澯
    ALARM_IPC_REGIONLEAVING,                                // IPC�����뿪�澯
    ALARM_IPC_AUDIOABNORMAL,                                // IPC�����쳣�澯
    ALARM_IPC_OBJECTTAKEN,                                  // IPC��Ʒ��ȡ�澯
    ALARM_IPC_OBJECTLEFT,                                   // IPC��Ʒ�����澯
    ALARM_IPC_PEOPLEGATHER,                                 // IPC��Ա�ۼ��澯
    ALARM_IPC_REGIONENTER,                                  // IPC�������ָ澯

    ALARM_IPC_MAX_ALARM_CODE
};

enum EMPuSvrAlarmCode
{
    //ͨ���޹�
    ALARM_SVR_BATTERYLOW= SVR_ALARM_BEGIN,                  //��ص�����
    ALARM_SVR_IPCONFLICT,                                   //ip��ַ��ͻ
    ALARM_SVR_NETBROKEN,                                    //����Ͽ�
    ALARM_SVR_NETSPEEDDROP,                                 //�����½�
    ALARM_SVR_SYSTIMEERR,                                   //ϵͳʱ�����
    ALARM_SVR_BURNERR,                                      //��¼����
    ALARM_SVR_DISKERR,                                      //���̳���
    ALARM_SVR_NODISK,                                       //��Ӳ��
    ALARM_SVR_MP4DISKSPACEFULL,                             //MP4�����ռ佫��
    ALARM_SVR_MP4FREESPACELACK,                             //MP4����ʣ��ռ䲻��
    ALARM_SVR_MULIMEDIAOFFLINE,                             //��ý����������
    ALARM_SVR_DISKSPACELACK,                                //����ʣ��ռ䲻��
    ALARM_SVR_IRLEARNSUCCESS,                               //����ѧϰ�ɹ�
    ALARM_SVR_NOAUDIO,                                      //����Ƶ����(�ϳ���Ƶ)
    ALARM_SVR_ONTRAIL,                                      //��Ѷ�����ź�

    //ͨ�����
    ALARM_SVR_IPCOFFLINE,                                   //ǰ�˵���
    ALARM_SVR_VIDEOLOST,                                    //ǰ����ƵԴ��ʧ
    ALARM_SVR_PININPUT,                                     //��������
    ALARM_SVR_IPCSENDCAPLACK,                               //����IPC������������
    ALARM_SVR_MOVDET,                                       //�ƶ����
    ALARM_SVR_NOSTREAM,                                     //�ϳ�ͨ��������
    ALARM_SVR_DEFOCUS,                                      //ʧ��
    ALARM_SVR_LUMINANCECHANGE,                              //���ȱ仯
    ALARM_SVR_SCENECHANGE,                                  //�����任
    ALARM_SVR_PERSONELGATHER,                               //��Ա�ۼ�

    ALARM_SVR_MAX_ALARM_CODE
};

//������ǰ������ظ澯
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/* �洢�澯��                                                           */
/************************************************************************/
/*����������
    1��RAID
    2��CLOUD

*/
// �洢common�澯������    (40001 - 40200)
#define STORAGE_COMMON_ALARM_BEGIN          (STORAGE_ALARM_BEGIN ) 
#define STORAGE_COMMON_ALARM_END            (STORAGE_COMMON_ALARM_BEGIN + 199)

// ����澯������          (40201 - 40400)
#define RAID_ALARM_BEGIN                    (STORAGE_COMMON_ALARM_END + 1) 
#define RAID_ALARM_END                      (RAID_ALARM_BEGIN + 199)

// �ƴ洢�澯������        (40401 - 40600)
#define CLOUD_ALARM_BEGIN                    (RAID_ALARM_END + 1) 
#define CLOUD_ALARM_END                      (CLOUD_ALARM_BEGIN + 199)

//////////////////////////////////////////////////////////////////////////
//���¶���洢����ظ澯

enum RAID_ALARM_CODE
{
	ALARM_RAID_POWER1_ABNORMAL = RAID_ALARM_BEGIN,  //��Դ1�쳣
	ALARM_RAID_POWER2_ABNORMAL,   			        //��Դ2�쳣
    ALARM_RAID_MODULE_OFFLINE,                      //����ģ������
};

//�����Ǵ洢����ظ澯
//////////////////////////////////////////////////////////////////////////



#endif //_DAU_ALARMCODE_H_
