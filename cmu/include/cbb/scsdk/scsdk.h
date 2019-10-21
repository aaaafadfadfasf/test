/*****************************************************************************
ģ����      : SCSDK(SYSTEM_CONFIGURATION_SDK)
�ļ���      : SCSDK.H
����ļ�    : kdvtype.h
�ļ�ʵ�ֹ���:
����        : ������
�汾        : <Copyright(c) 2003-2012 Suzhou Keda Technology Co.,Ltd. All right reserved.>
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
******************************************************************************/
#ifndef __SCSDK_H__
#define __SCSDK_H__

#include "kdvtype.h"


/*====================================================================
                        �궨��
====================================================================*/
#define SCSDK_MAX_DEVNAME_LEN               32                      //�豸����󳤶�
#define SCSDK_MAX_DEVID_LEN                 32                      //�豸ID��󳤶�
#define SCSDK_MAX_DEVTYPE_LEN               32                      //�豸������󳤶�
#define SCSDK_MAX_MANUFAC_LEN               32                      //�豸��������󳤶�
#define SCSDK_MAX_NAME_LEN                  512                     //������󳤶�
#define SCSDK_MAX_MOUNTPATH_LEN             50                      //����·������󳤶�
#define SCSDK_MAX_ERRORDESCRIPTION_LEN      256                     //������������󳤶�
#define SCSDK_MAX_PART_NUM                  8                       //������������
#define SCSDK_MAX_LOGDESCRIPTION_LEN        256                     //������������󳤶�
#define SCSDK_MAX_DISKGROUPNAME_LEN         64                      //����������
#define SCSDK_MAX_VITUALDISKNAME            256                     //������������
#define SCSDK_MAX_PASSWORD_LEN              64                      //�������󳤶�
#define SCSDK_MAX_DISKSOURCE_LEN            256                     //������Դ��󳤶�(64������)
#define SCSDK_MAX_ADDRESS_LEN               40                      //��ַ��󳤶�
#define SCSDK_MAX_OPDES_LEN                 256                     //������������ӵģ�
#define SCSDK_MAX_ARRAY_TYPE_LEN            31                      //����������󳤶�
#define SCSDK_MAX_DISK_VENDOR_LEN           32
#define SCSDK_MAX_MOUNTOPTION_LEN           128

#define SCSDK_DISK_VENDOR_KDM               "kdm"
/*====================================================================
                        ö�ٶ���
====================================================================*/
typedef enum
{
    ESCSDK_ERRTYPE_OK    = 0,                                               //���سɹ�,�������ͻ��˷���ack
    ESCSDK_ERRTYPE_PASSWORD_ERR,                                            //������󣬼�Ȩʧ��
    ESCSDK_ERRTYPE_HAS_USER,                                                //�û��ѵ�¼���߷����IP���벻��ȷ
    ESCSDK_ERRTYPE_CUSTOMER_NOT_EXIST,                                      //�ͻ���Ϣ������
    ESCSDK_ERRTYPE_LOGIN_FAIL,                                              //��½ʧ��
    ESCSDK_ERRTYPE_NODEINVALID,                                             //node������
    ESCSDK_ERRTYPE_GETDISKFAIL,                                             //��ȡ������Ϣʧ��
    ESCSDK_ERRTYPE_GET_ARRAY_INFO_FAILED,                                   //��ȡ������Ϣʧ��
    ESCSDK_ERRTYPE_GETDISKGROUPFAIL,                                        //��ȡ��������Ϣʧ��
    ESCSDK_ERRTYPE_GETPHYSICALDISKFAIL,                                     //��ȡ���������Ϣʧ��
    ESCSDK_ERRTYPE_GETVIRTUALDISKFAIL,                                      //��ȡ���������Ϣʧ��
    ESCSDK_ERRTYPE_EDITPARTFAIL,                                            //��������ִ��ʧ�ܣ�����ԭ�򣺷�������������Ϸ�������ϵͳԭ����
    ESCSDK_ERRTYPE_OPERATION_IN_PROGRESS,                                   //�������д��̲������ڽ���,�Ȳ����������
    ESCSDK_ERRTYPE_DISK_STATUS_ERROR,                                       //����״̬���󣬻��в������ڽ���
    ESCSDK_ERRTYPE_MOUNTED,                                                 //�����ǹ���״̬
    ESCSDK_ERRTYPE_DEL_UASDB_FAILED,                                        //ɾ�����ݿ���¼���¼ʧ�ܣ������ؽű��Ƿ����
    ESCSDK_ERRTYPE_NOT_EXIST,                                               //���̻�����Ѿ�������
    ESCSDK_ERRTYPE_FDISK_NOT_SUPPORT,                                       //���߲�֧�ֶ���������2T��������СΪ��512�ֽڵĴ��̽��з�������
    ESCSDK_ERRTYPE_FORMATDISKFAIL,                                          //��ʽ������ʧ��
    ESCSDK_ERRTYPE_DISK_CTRL_BUZY,                                          //���̲����߳�æ�����Ժ�����
    ESCSDK_ERRTYPE_MOUNTFAIL,                                               //��������ִ��ʧ�ܣ�����ԭ�򣺶Ͽ����̻�����������ʱ��û������ж�ش��̣���Ҫ�����޸�
    ESCSDK_ERRTYPE_UNFORMATED,                                              //����δ��ʽ��
    ESCSDK_ERRTYPE_DIR_IN_USE,                                              //�������س�ͻ��ͬ��Ŀ¼�Ѿ��ڹ����б��У��������ߴ����Ƿ���ȷж��
    ESCSDK_ERRTYPE_DISK_NUM_OVERLOAD,                                       //�����̷�����������¼���֧�ֵ�������,����Ĵ��̽�����ʹ��,���Ƴ�
    ESCSDK_ERRTYPE_UNMOUNTFAIL,                                             //ж������ִ��ʧ�ܣ�����ԭ�򣺸÷������ڱ�ʹ�ã��ȴ��Զ�ж��
    ESCSDK_ERRTYPE_HAS_UMOUNT,                                              //�����Ѿ�ж��
    ESCSDK_ERRTYPE_FSCKFAIL,                                                //�޸�����ִ��ʧ�ܣ�����ԭ�򣺴������쳣
    ESCSDK_ERRTYPE_REBOOT_REQUIRED,                                         //ϵͳ��Ҫ����
    ESCSDK_ERRTYPE_FS_LEFT_UNCORRECTED,                                     //�ļ�ϵͳ�޷��޸�
    ESCSDK_ERRTYPE_CANCELED,                                                //����ִ�б�ȡ��
    ESCSDK_ERRTYPE_GETCFGFAIL,                                              //��ȡiscsi�б�ʧ��
    ESCSDK_ERRTYPE_ADDISCSIFAIL,                                            //���iscsi����ʧ��
    ESCSDK_ERRTYPE_SCSI_CONFIG_FAILED,                                      //����SCSI�����ļ�ʧ�ܴ���
    ESCSDK_ERRTYPE_DELISCSIFAIL,                                            //ɾ��iscsi����ʧ��
    ESCSDK_ERRTYPE_DISCONNECTISCSIFAIL,                                     //�Ͽ�iscsi����ʧ��
    ESCSDK_ERRTYPE_CONNECTISCSIFAIL,                                        //����iscsi����ʧ��
    ESCSDK_ERRTYPE_STARTISCSIFAIL,                                          //����iscsi����ʧ��
    ESCSDK_ERRTYPE_STOPISCSIFAIL,                                           //ֹͣiscsi����ʧ��
    ESCSDK_ERRTYPE_DISKGROUPNAMEINVALID,                                    //����������Ч
    ESCSDK_ERRTYPE_CANCLEDISKOPFAIL,                                        //ɾ�����̲���ʧ��
    ESCSDK_ERRTYPE_DELDISKFAIL,                                             //ɾ�����ߴ���ʧ��
    ESCSDK_ERRTYPE_HASCONSERV,                                              //�Ѿ������Ϸ�����
    ESCSDK_ERRTYPE_INNERERR,                                                //�ڲ����󣨰�������appʧ�ܣ������Ĳ���ָ��Ϊ�գ���ֱ�ӷ��أ�
    ESCSDK_ERRTYPE_OPERTIMEOUT,                                             //������ʱ
    ESCSDK_ERRTYPE_PARAMINVALID,                                            //������Ч
    ESCSDK_ERRTYPE_DISCONNECTNODEFAIL,                                      //�ͷŽڵ�ʧ��
    ESCSDK_ERRTYPE_DISCONNECTNODE,                                          //�Ѿ��ͷ�node
    ESCSDK_ERRTYPE_POSTFAIL,                                                //osppostʧ��
    ESCSDK_ERRTYPE_DEVICE_NOTCONNECTED,                                     //�豸��δ����
    ESCSDK_ERRTYPE_OUT_OF_MEMORY,                                           //�ڴ治��
    ESCSDK_ERRTYPE_GET_ISCSI_INITIATORNAME_FAIL,                            //��ȡISCSI�ķ�������ʧ��
    ESCSDK_ERRTYPE_FS_LIMITED,                                              //ϵͳ���ƣ����������8TB
    ESCSDK_ERRTYPE_VBFS_IS_DISABLED,                                        //VBFS�ļ�ϵͳ������
    ESCSDK_ERRTYPE_DISK_SIZE_OVERLOAD,                                      //ϵͳ���ƣ�����16TB�Ĵ����޷��������ʣ������iscsi�̣������»���
    ESCSDK_ERRTYPE_SET_DISK_USAGE_FAILED,                                   //���ô�����;ʧ��
    ESCSDK_ERRTYPE_GET_NETFS_FAILED,                                        //��ȡNetFSʧ��
    ESCSDK_ERRTYPE_ADD_NETFS_FAILED,                                        //���NetFSʧ��
    ESCSDK_ERRTYPE_DEL_NETFS_FAILED,                                        //ɾ��NetFSʧ��
    ESCSDK_ERRTYPE_GET_NETFS_EXPORT_LIST_FAILED,                            //��ȡNetFS�����������б�
}ESCSDK_ErrType;

typedef enum
{
    ESCSDK_IPTYPE_IPV4,
    ESCSDK_IPTYPE_IPV6,
    ESCSDK_IPTYPE_UNKNOWN,
}ESCSDK_IpType;

typedef enum
{
    ESCSDK_DISKSTATE_ONLINE,                                                    //����
    ESCSDK_DISKSTATE_OFFLINE,                                                   //����
    ESCSDK_DISKSTATE_INACTIVE,                                                  //δ����
    ESCSDK_DISKSTATE_UNKNOWN,                                                   //δ֪
}ESCSDK_DiskState;

typedef enum
{
    ESCSDK_ARRAYSTATE_ONLINE,                                                   //����
    ESCSDK_ARRAYSTATE_OFFLINE,                                                  //����
    ESCSDK_ARRAYSTATE_UNKNOWN,
}ESCSDK_ArrayState;

typedef enum
{
    ESCSDK_DC_unknown = 0,
    ESCSDK_DC_USB = 1,
    ESCSDK_DC_ISCSI_TRANSPORT = 2,
    ESCSDK_DC_INTERAL_SATA = 4,
    ESCSDK_DC_NETFS = 8,
    ESCSDK_DC_LVM = 16,
    ESCSDK_DC_EXTERNAL = ESCSDK_DC_USB | ESCSDK_DC_ISCSI_TRANSPORT | ESCSDK_DC_NETFS,
}ESCSDK_DeviceClass;

typedef enum
{
    ESCSDK_ISCSISTATE_RUNNING,      //��������
    ESCSDK_ISCSISTATE_STOPED,       //�Ѿ�ֹͣ
    ESCSDK_ISCSISTATE_UNSUPPORTED,  //��֧��
    ESCSDK_ISCSISTATE_UNKNOWN,      //��ȡ״̬ʧ��ʱΪ�˽��
}ESCSDK_IscsiState;

typedef enum
{
    ESCSDK_PARTSTATE_OFFLINE,                                                   //����
    ESCSDK_PARTSTATE_UNFORMATTED,                                               //δ��ʽ��
    ESCSDK_PARTSTATE_FORMATTED,                                                 //�Ѹ�ʽ��
    ESCSDK_PARTSTATE_MOUNTED,                                                   //�ѹ���
    ESCSDK_PARTSTATE_UNMOUNTED,                                                 //��ж��
    ESCSDK_PARTSTATE_UNKNOWN,                                                   //δ֪״̬

}ESCSDK_PartState;

typedef enum
{
    ESCSDK_PARTUSAGE_DATA,          //����
    ESCSDK_PARTUSAGE_SYSTEM,        //ϵͳ
    ESCSDK_PARTUSAGE_BACKUP,        //����
    ESCSDK_PARTUSAGE_SATA,          //����sata
    ESCSDK_PARTUSAGE_APPDADA,       //Ӧ������
    ESCSDK_PARTUSAGE_GPSDATA,       //GPS����
    ESCSDK_PARTUSAGE_MAP,           //���ߵ�ͼ
    ESCSDK_PARTUSAGE_UNKNOWN = -1
}ESCSDK_PartUsage;

typedef enum
{
    ESCSDK_DiskDevOperation_UNKNOWN,                                                    //�޲���
    ESCSDK_DiskDevOperation_STOPOPERATION,                                              //ֹͣ����
    ESCSDK_DiskDevOperation_WAITFORMAT,                                                 //�ȴ���ʽ��
    ESCSDK_DiskDevOperation_FORMATTING,                                                 //���ڸ�ʽ��
    ESCSDK_DiskDevOperation_WAITFSCK,                                                   //�ȴ��޸�
    ESCSDK_DiskDevOperation_FSCKING,                                                    //�����޸�
    ESCSDK_DiskDevOperation_WAITMOUNT,                                                  //�ȴ�����
    ESCSDK_DiskDevOperation_MOUNTING,                                                   //���ڹ���
    ESCSDK_DiskDevOperation_WAITUNMOUNT,                                                //�ȴ�ж��
    ESCSDK_DiskDevOperation_UNMOUNTING,                                                 //����ж��
    ESCSDK_DiskDevOperation_SYSINVALID,                                                 //���ֶ�ж�أ���ж��״̬(10.23���)
    ESCSDK_DiskDevOperation_ERROR,                                                      //��ʽ��ʧ�ܻ��޸�ʧ��
    ESCSDK_DiskDevOperation_REBOOT,
    ESCSDK_DiskDevOperation_UNUSED,                                                     //δʹ��,�����������
}ESCSDK_DiskDevOperation;

typedef enum
{
    ESCSDK_FORMATTYPE_VBFS,                                                     //��ʽ������Ϊvbfs
    ESCSDK_FORMATTYPE_EXT3,                                                     //��ʽ������Ϊext3
    ESCSDK_FORMATTYPE_UNKNOWN,                                                  //�޷�ʶ��
}ESCSDK_FormatType;

typedef enum
{
    ESCSDK_LOGTYPE_EXCEPTION,                                                   //��־������ʾ�쳣
    ESCSDK_LOGTYPE_ERROR,                                                       //����
    ESCSDK_LOGTYPE_WARNING,                                                     //����
    ESCSDK_LOGTYPE_INFO,                                                        //��Ϣ�����ߣ����أ�ж�صȣ�
}ESCSDK_LogType;

typedef enum                                                                    //��������
{
    ESCSDK_ARRAYTYPE_VS200,
    ESCSDK_ARRAYTYPE_VS400,
    ESCSDK_ARRAYTYPE_VS200E,
    ESCSDK_ARRAYTYPE_VS200C,
    ESCSDK_AARAYTYPE_UNKNOWN,
}ESCSDK_ArrayType;

typedef enum                                                                    //������״̬
{
    ESCSDK_DISKGROUPSTATE_CLEAN,
    ESCSDK_DISKGROUPSTATE_DEGRADED,
    ESCSDK_DISKGROUPSTATE_RECOVERING,
    ESCSDK_DISKGROUPSTATE_FAILED,
    ESCSDK_DISKGROUPSTATE_RESYING,
    ESCSDK_DISKGROUPSTATE_UNKNOWN,
}ESCSDK_DiskGroupState;

typedef enum                                                                    //RAID����
{
    ESCSDK_RAIDLEVEL_RAID_0,
    ESCSDK_RAIDLEVEL_RAID_1,
    ESCSDK_RAIDLEVEL_RAID_2,
    ESCSDK_RAIDLEVEL_RAID_3,
    ESCSDK_RAIDLEVEL_RAID_4,
    ESCSDK_RAIDLEVEL_RAID_5,
    ESCSDK_RAIDLEVEL_RAID_6,
    ESCSDK_RAIDLEVEL_RAID_7,
    ESCSDK_RAIDLEVEL_RAID_8,
    ESCSDK_RAIDLEVEL_RAID_9,
    ESCSDK_RAIDLEVEL_RAID_10,
    ESCSDK_RAIDLEVEL_RAID_UNKNOWN,
}ESCSDK_RaidLevel;

typedef enum                                                                    //���̽���״̬
{
    ESCSDK_DISKHEALTH_OK,
    ESCSDK_DISKHEALTH_BAD,
    ESCSDK_DISKHEALTH_UNKNOWN,
}ESCSDK_DiskHealth;

typedef enum                                                                    //�������״̬
{
    ESCSDK_VIRTUALDISKSTATE_ESTABLISHED,
    ESCSDK_VIRTUALDISKSTATE_DROPPED,
    ESCSDK_VIRTUALDISKSTATE_UNKNOWN,
}ESCSDK_VirtualDiskState;

typedef enum                                                                    //�����������
{
    ESCSDK_VIRTUALDISKTYPE_ISCSI,
    ESCSDK_VIRTUALDISKTYPE_UNKNOWN,
}ESCSDK_VirtualDiskType;

typedef enum            //���й���ģ���¼�
{
    ESCSDK_RMEVENT_ADD,
    ESCSDK_RMEVENT_UPDATE,
    ESCSDK_RMEVENT_DEL,
}ESCSDK_RMEvent;

typedef enum
{
    ESCSDK_DISKSTATUS_EMPTY        = 0x01,         /* ��λ��Ӳ�� */
    ESCSDK_DISKSTATUS_FREE         = 0x02,         /* ��λ��Ӳ�̣�����û�б�ʹ�� */
    ESCSDK_DISKSTATUS_USEDFORGROUP = 0x03,         /* ��λ��Ӳ�̣���Ϊ�������Ѿ�ʹ�� */
    ESCSDK_DISKSTATUS_USEDFORSPARE = 0x04,         /* ��λ��Ӳ�̣���Ϊ�ȱ���ʹ�� */
    ESCSDK_DISKSTATUS_BAD          = 0x05,         /* ��λ��Ӳ�̣������Ѿ��� */
}ESCSDK_DiskStatus;

typedef enum
{
    ESCSDK_ABNCODE_DISK_NORMAL = 0,             //��������
    ESCSDK_ABNCODE_DISK_RO = 1<<0,              //�ļ�ϵͳֻ��
    ESCSDK_ABNCODE_FS_ERROR = 1<<1,             //�ļ�ϵͳ����
}ESCSDK_AbnCode;

typedef enum              // �����ļ�ϵͳ����
{
    ESCSDK_MOUNTTYPE_NULL,  // ��ָ��
    ESCSDK_MOUNTTYPE_NFS,
    ESCSDK_MOUNTTYPE_CIFS
}ESCSDK_MountType;
/************************************************************************/
/*                �ص��¼����Ͷ���                                      */
/************************************************************************/
typedef enum
{
    SCSDK_EVENT_BEGIN                    = 2000,

    SCSDK_EVENT_GET_DISKINFO                = SCSDK_EVENT_BEGIN + 1,       //TSCSDK_OperResult+u32(�������Ĵ�������)
    SCSDK_EVENT_UPDATE_DISKINFO_NTF         = SCSDK_EVENT_BEGIN + 2,       //TSCSDK_DeviceID+TSCSDK_DiskInfo
    SCSDK_EVENT_UPDATE_PARTINFO_NTF         = SCSDK_EVENT_BEGIN + 3,       //TSCSDK_DeviceID+TSCSDK_PartInfo
    SCSDK_EVENT_UPDATE_DISKDEVOPSTATE_NTF   = SCSDK_EVENT_BEGIN + 4,       //TSCSDK_DiskDevOpState(ĳһ�豸�ϵĴ��̷�������״̬,������ʽ�������ϱ���)
    SCSDK_EVENT_UPDATE_IOUTIL_NTF           = SCSDK_EVENT_BEGIN + 5,       //TSCSDK_IoUtil(ĳһ�豸�ϵĴ��̷���ʹ����)
    SCSDK_EVENT_VIEW_LOG                    = SCSDK_EVENT_BEGIN + 6,       //TSCSDK_OperResult+TSCSDK_ViewLogHeadInfo(����+��ǰ��N+�������)+N*TSCSDK_ViewLogResult
    SCSDK_EVENT_EDIT_PART                   = SCSDK_EVENT_BEGIN + 7,       //TSCSDK_OperResult
    SCSDK_EVENT_FORMAT_DISKDEV              = SCSDK_EVENT_BEGIN + 8,       //TSCSDK_OperResult
    SCSDK_EVENT_MOUNT_DISKDEV               = SCSDK_EVENT_BEGIN + 9,       //TSCSDK_OperResult
    SCSDK_EVENT_UNMOUNT_DISKDEV             = SCSDK_EVENT_BEGIN + 10,      //TSCSDK_OperResult
    SCSDK_EVENT_FSCK_DISKDEV                = SCSDK_EVENT_BEGIN + 11,      //TSCSDK_OperResult

    //TSCSDK_OperResult + TSCSDK_GetIscsiCfgHeadInfo(�����������и���+�ܵ�״̬) + N * TSCSDK_GetIscsiCfgResult
    SCSDK_EVENT_GET_ISCSICFG                = SCSDK_EVENT_BEGIN + 12,
    SCSDK_EVENT_ADD_ISCSICFG                = SCSDK_EVENT_BEGIN + 13,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_ISCSICFG                = SCSDK_EVENT_BEGIN + 14,      //TSCSDK_OperResult
    SCSDK_EVENT_DISCONNECT_ISCSICFG         = SCSDK_EVENT_BEGIN + 15,      //TSCSDK_OperResult
    SCSDK_EVENT_CONNECT_ISCSICFG            = SCSDK_EVENT_BEGIN + 16,      //TSCSDK_OperResult
    SCSDK_EVENT_START_ISCSI                 = SCSDK_EVENT_BEGIN + 17,      //TSCSDK_OperResult
    SCSDK_EVENT_STOP_ISCSI                  = SCSDK_EVENT_BEGIN + 18,      //TSCSDK_OperResult
    SCSDK_EVENT_VIEW_ARRAYINFO              = SCSDK_EVENT_BEGIN + 19,      //TSCSDK_OperResult+u32(�����������и���)TSCSDK_ViewArrayInfo
    SCSDK_EVENT_VIEW_DISKGROUPINFO          = SCSDK_EVENT_BEGIN + 20,      //TSCSDK_OperResult+u32(�������Ĵ��������)TSCSDK_ViewDiskGroupInfo
    SCSDK_EVENT_VIEW_PHYSICALDISKINFO       = SCSDK_EVENT_BEGIN + 21,      //TSCSDK_OperResult+u32(��������������̸���)TSCSDK_ViewPhysicalDiskInfo
    SCSDK_EVENT_VIEW_VIRTUALDISKINFO        = SCSDK_EVENT_BEGIN + 22,      //TSCSDK_OperResult+u32(��������������̸���)TSCSDK_ViewVirtualDiskInfo
    SCSDK_EVENT_CONNECT_SERV                = SCSDK_EVENT_BEGIN + 23,      //TSCSDK_OperResult
    SCSDK_EVENT_DISCONNECT_SERV             = SCSDK_EVENT_BEGIN + 24,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_DISK                    = SCSDK_EVENT_BEGIN + 25,      //TSCSDK_OperResult
    SCSDK_EVENT_CANCLE_DISKOP               = SCSDK_EVENT_BEGIN + 26,      //TSCSDK_OperResult

    SCSDK_EVENT_UPDATE_ARRAYINFO_NTF        = SCSDK_EVENT_BEGIN + 27,      //TSCSDK_UpdateArrayInfo
    SCSDK_EVENT_UPDATE_DISKGROUPINFO_NTF    = SCSDK_EVENT_BEGIN + 28,      //TSCSDK_UpdateDiskGroupInfo
    SCSDK_EVENT_UPDATE_PHYSICALDISKINFO_NTF = SCSDK_EVENT_BEGIN + 29,      //TSCSDK_UpdatePhysicalDiskInfo
    SCSDK_EVENT_UPDATE_VIRTUALDISKINFO_NTF  = SCSDK_EVENT_BEGIN + 30,      //TSCSDK_UpdateVirtualDiskInfo
    SCSDK_EVENT_SERV_OFFLINE_NTF            = SCSDK_EVENT_BEGIN + 31,      //TSCSDK_DeviceID
//  SCSDK_EVENT_UPDATE_DISKSTATE_NTF        = SCSDK_EVENT_BEGIN + 32,      //TSCSDK_DiskState(ĳһ�豸�ϴ��̵�״̬)(�����޸��Ż�scs)
//  SCSDK_EVENT_UPDATE_PARTSTATE_NTF        = SCSDK_EVENT_BEGIN + 33,      //TSCSDK_PartState(ĳһ�豸�ϵĴ��̷���״̬)
//  SCSDK_EVENT_UPDATE_ABN_NTF              = SCSDK_EVENT_BEGIN + 34,      //TSCSDK_Abn(ĳһ�豸�ϵĴ��̷����쳣��������쳣���뷵��ֵΪ0�����ʾ�쳣�ָ�)

    SCSDK_EVENT_GET_ISCSI_INITIATORNAME     = SCSDK_EVENT_BEGIN + 35,       //TSCSDK_OperResult + TSCSDK_GetIscsiInitiatorName
    SCSDK_EVENT_SET_DISKUSAGE               = SCSDK_EVENT_BEGIN + 36,      //TSCSDK_OperResult
    SCSDK_EVENT_GET_NETFSCFG                = SCSDK_EVENT_BEGIN + 37,      //TSCSDK_OperResult + u32(�����õ�NetFS����) + TSCSDK_GetNetFSRunCfg
    SCSDK_EVENT_ADD_NETFSCFG                = SCSDK_EVENT_BEGIN + 38,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_NETFSCFG                = SCSDK_EVENT_BEGIN + 39,      //TSCSDK_OperResult
    SCSDK_EVENT_GET_NETFSEXPORTLIST         = SCSDK_EVENT_BEGIN + 40,      //TSCSDK_OperResult + u32(NetFS����������Ŀ¼�ܸ���) + u32(���λ�ȡ��ʵ��Ŀ¼����) + TSCSDK_GetNetFSExport
    SCSDK_EVENT_END,

}ESCSDK_EventType;



/*====================================================================
                        �ṹ�嶨��
====================================================================*/
typedef struct tagSCSDK_DelDisk
{
    u16  m_wDiskID;
    u8   m_byVideoRecordFlag; // 0��ʾ����Ҫɾ���豸ID��Ӧ��¼���¼ 1��ʾ��Ҫɾ���豸ID��Ӧ��¼���¼
}TSCSDK_DelDisk;

typedef struct tagSCSDK_DeviceID                                                //�豸ID�Ľṹ��
{
    s8                 m_szDeviceID[SCSDK_MAX_DEVID_LEN+1];
}TSCSDK_DeviceID;

typedef struct tagSCSDK_IpAddress
{
    ESCSDK_IpType   m_eIpType;
    s8              m_szAddress[SCSDK_MAX_ADDRESS_LEN+1];
    u16             m_wPrefixLenth;
}TSCSDK_IpAddress;

typedef struct tagSCSDK_DiskDev                                         //�����豸�ṹ��
{
    u32                  m_dwPartID;                                   //����ID
    u16                  m_wDiskID;                                     //����ID,��Ϊ�������ߺ󣬴������ǿ��Ա����õģ�
                                                                        //�Է��ͻ�����������Ϣ�Ĳ�һ�»���ʱ���µ�������������ΨһID������У���豸��
    s8                   m_szDevName[SCSDK_MAX_NAME_LEN+1];             //�����Ƿ�����Ҳ�����Ǵ�����������DiskIDһ���Ǵ��̵�
}TSCSDK_DiskDev;

typedef struct tagSCSDK_ConnectServ                                     //���ӣ��Ͽ�ý��������Ľṹ��
{
    s8                 m_szDeviceName[SCSDK_MAX_DEVNAME_LEN+1];         //�豸��
    TSCSDK_IpAddress   m_tDeviceIP;                                     //�豸IP
    u16                m_wDevicePort;                                   //�豸�˿�
}TSCSDK_ConnectServ,TSCSDK_DisconnectServ;

typedef struct tagSCSDK_IoUtil                                              //IOʹ���ʽṹ��
{
    TSCSDK_DeviceID        m_tDeviceID;                                     //�豸ID
    TSCSDK_DiskDev         m_tDiskDev;                                      //�����豸�ṹ��
    u8                     m_byIoUtil;                                      //IOʹ����
}TSCSDK_IoUtil;

typedef struct tagSCSDK_Abn                                                 //�쳣�������쳣����
{
    TSCSDK_DeviceID        m_tDeviceID;                                     //�豸ID
    TSCSDK_DiskDev         m_tDiskDev;                                      //�����豸�ṹ��
    u32                    m_dwAbnNum;                                      //�쳣����
    s32                    m_nAbnCode;                                      //�쳣����(��Ҫ�����쳣����ĳ���)
}TSCSDK_Abn;

typedef struct tagSCSDK_PartInfo                                           //������Ϣ�ṹ��
{
    u32                      m_dwPartID;                                   //����ID
    s8                       m_szPartName[SCSDK_MAX_NAME_LEN+1];           //������
    u16                      m_wPartDiskID;                                //����ID
    u32                      m_dwPartsize;                                 //������С����λ��M��
    u32                      m_dwFreeSpace;                                //���ÿռ��С ����λ��M��
    ESCSDK_PartState         m_ePartState;                                 //����״̬
    s8                       m_szFormatType[SCSDK_MAX_DEVTYPE_LEN+1] ;          //������ʽ��vbfs��ext3��
    u32                      m_dwAbnNum;                                   //�쳣����
    s32                      m_nAbnCode;                                   //�쳣����(��Ҫ�����쳣����ĳ���)
    s8                       m_achMountPath[SCSDK_MAX_MOUNTPATH_LEN+1];    //����·��,��uuid�Զ�����
    u32                      m_dwStart;                                    //��ʼ��λ��
    u32                      m_dwEnd;                                      //������λ��
    u8                       m_byPercent;                                  //�ٷֱ�
    s8                       m_szPartOpDes[SCSDK_MAX_OPDES_LEN+1];         //������������ӵģ�
    ESCSDK_DiskDevOperation  m_eDiskDevOperation;                          //�����豸����
    ESCSDK_PartUsage         m_ePartUsage;                                 //��;����
    u8                       m_byIoUtil;                                   //IOʹ����
}TSCSDK_PartInfo;


typedef struct tagSCSDK_DiskInfo                                      //������Ϣ�ṹ��
{
    u16                m_wDiskID;                                     //����ID
    s8                 m_szDiskName[SCSDK_MAX_NAME_LEN+1];            //������
    u32                m_dwDiskSize;                                  //����������λ��M��
    u32                m_dwDiskAbnNum;                                //�쳣����
    ESCSDK_DiskState   m_eDiskState;                                  //����״̬
    s8                 m_achDiskSource[SCSDK_MAX_DISKSOURCE_LEN+1];   //������Դ
    u32                m_dwCylinders;                                 //����������
    u8                 m_byDiskPartNum;                               //������ (�洢����������Ϊ4��ϵͳ����������Ϊ8)
    TSCSDK_PartInfo    m_atPartInfo[SCSDK_MAX_PART_NUM];              //������Ϣ
    s8                 m_szDiskVendor[SCSDK_MAX_DISK_VENDOR_LEN+1];   //�洢�豸���̣�add by zhichun 201600920
    ESCSDK_DeviceClass m_eDeviceClass;                                //�豸����
}TSCSDK_DiskInfo;

typedef struct tagSCSDK_PartCfgLine                                             //���������ķֽ���
{
    u32                m_dwCylinderStart;                                       //��ʼ����
    u32                m_dwCylinders;                                           //����������
    u32                m_dwPartID;                                              //����ID
}TSCSDK_PartCfgLine;

typedef struct tagSCSDK_Time
{
    u32        m_dwyear;
    u8         m_bymonth;
    u8         m_byday;
    u8         m_byhour;
    u8         m_byminute;
    u8         m_bysecond;
}TSCSDK_Time;

typedef struct tagSCSDK_TimeRange                                                   //ʱ��εĽṹ��
{
    u32         m_dwStartTime;
    u32         m_dwEndTime;

}TSCSDK_TimeRange;

 typedef struct tagSCSDK_EdiPart                                            //�����̻��ֳɼ��������Ľṹ��
 {
    TSCSDK_DiskDev          m_tDiskDev;                                     //�����豸�ṹ��
    u8                      m_byDiskPartNum;                                //�����������洢����󲻳���4��ϵͳ�̲�����6��
    TSCSDK_PartCfgLine      m_atPartCfgLine[SCSDK_MAX_PART_NUM];
 }TSCSDK_EditPart;

typedef struct tagSCSDK_ViewLog                                              //��־�Ľṹ��
{
    TSCSDK_DiskDev          m_tDiskDev;                                     //�����豸�ṹ��
    TSCSDK_TimeRange        m_tTimeRange;                                   //ʱ���
}TSCSDK_ViewLog;

typedef struct tagSCSDK_LogHeadInfo
{
    u32     m_dwTotal;                                                      //��־����
    u32     m_dwNum;                                                        //��ǰ��N
    BOOL32  m_dwEnd;                                                        //�������
}TSCSDK_ViewLogHeadInfo;

typedef struct tagSCSDK_ViewLogResult
{
    TSCSDK_Time         m_tTime;                                                    //��־��ʱ��
    ESCSDK_LogType      m_eLogType;                                                 //��־����
    s8                  m_szLogResultDes[SCSDK_MAX_LOGDESCRIPTION_LEN+1];           //��־�������
}TSCSDK_ViewLogResult;

typedef struct tagSCSDK_FormatDiskDev                                            //��ʽ���Ľṹ��
{
    TSCSDK_DiskDev          m_tDiskDev;                                         //�����豸�ṹ��
    ESCSDK_FormatType       m_eFormatType;                                      //��ʽ������

}TSCSDK_FormatDiskDev;

typedef struct tagSCSDK_MountDiskDev                                              //���أ�ж�أ��޸������Ľṹ��
{
    TSCSDK_DiskDev          m_tDiskDev;                                           //�����豸�ṹ��
    s8                      m_achDiskDevMountPath[SCSDK_MAX_MOUNTPATH_LEN+1];     //����·��(Ϊ��ʱ����UUID�Զ�����)
}TSCSDK_MountDiskDev,TSCSDK_UnmountDiskDev,TSCSDK_FsckDiskDev;

typedef struct tagSCSDK_SetDiskDevUsage                                          //���ô�����;�Ľṹ��
{
    TSCSDK_DiskDev          m_tDiskDev;                                         //�����豸�ṹ��
    ESCSDK_PartUsage        m_ePartUsage;                                       //������;
}TSCSDK_SetDiskDevUsage;

typedef struct tagSCSDK_NetFSCfg                                                //NetFS�Ļ�ȡ����ӣ�ɾ���ṹ��
{
    s8                      m_achServerPath[SCSDK_MAX_NAME_LEN+1];              //NetFS������·��
    s8                      m_achUserName[SCSDK_MAX_NAME_LEN+1];                //�û���
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //����
    s8                      m_achOption[SCSDK_MAX_MOUNTOPTION_LEN+1];           //����ѡ��,��Ϊ��
    ESCSDK_MountType        m_eMountType;                                       //NetFS����:nfs/cifs ...���ɲ�ָ��
}TSCSDK_AddNetFSCfg,TSCSDK_DelNetFSCfg,TSCSDK_GetNetFSCfg;

typedef struct tagSCSDK_GetNetFSRunCfg
{
     TSCSDK_GetNetFSCfg      m_tGetNetFSCfg;                                    // NetFS������������Ϣ
     BOOL32                  m_bConStatus;                                      // ����״̬;1:������;0:δ����
} TSCSDK_GetNetFSRunCfg;

typedef struct tagSCSDK_GetNetFSExportCond
{
    TSCSDK_IpAddress m_tNetFSIP;                                          // NetFS��������ַ
    u8 m_byPageNum;                                                       // ����ڼ�ҳ������
    u8 m_byNumPerPage;                                                   // ÿҳ��ʾ�ĸ���
}TSCSDK_GetNetFSExportCond;

typedef struct tagSCSDK_NetFSExport                                       //NetFS����������Ŀ¼�Ľṹ��
{
    s8                      m_achPath[SCSDK_MAX_NAME_LEN+1];              // NetFS������Ŀ¼
    s8                      m_achIPList[SCSDK_MAX_NAME_LEN+1];            // NetFS������Ŀ¼���ڿɷ��ʵĿͻ���
    BOOL32                  m_bIsAdd;                                     // ��Ŀ¼�Ƿ��ڱ��ͻ����Ѿ����
}TSCSDK_GetNetFSExport;

typedef struct tagSCSDK_GetIscsiCfgHeadInfo
{
    u8                  m_byIpSanNum;   //ͷ��������ŵ�TSCSDK_GetIscsiCfgResult�ĸ���
    ESCSDK_IscsiState   m_eIscsiState;  //�����Iscsi����״̬
}TSCSDK_GetIscsiCfgHeadInfo;

typedef struct tagSCSDK_GetIscsiCfgResult
 {
    s8                      m_szUserName[SCSDK_MAX_NAME_LEN+1];                 //�û���
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //����
    TSCSDK_IpAddress        m_tIP;                                              //IP��ַ
    u16                     m_wPort;                                            //�˿�
    ESCSDK_ArrayState       m_eArrayState;                                      //���е�״̬
 }TSCSDK_GetIscsiCfgResult;

typedef struct tagSCSDK_AddIscsiCfg                                             //���е���ӣ�ɾ�������ӣ��Ͽ��ṹ��
 {
    s8                      m_szUserName[SCSDK_MAX_NAME_LEN+1];                 //�û���
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //����
    TSCSDK_IpAddress        m_tIP;                                              //IP��ַ
    u16                     m_wPort;                                            //�˿�
 }TSCSDK_AddIscsiCfg,TSCSDK_DelIscsiCfg,TSCSDK_ConnectIscsiCfg,TSCSDK_DisconnectIscsiCfg;

typedef struct tagSCSDK_Sequence
 {
     u32                m_dwSequenceNO;
}TSCSDK_Sequence;

typedef struct tagSCSDK_EventHandle
{
    ESCSDK_EventType    m_eEventType;
    TSCSDK_Sequence     m_tSequence;
}TSCSDK_EventHandle;

typedef struct tagSCSDK_OperResult                                              //�������
{
    ESCSDK_ErrType          m_eOperResult;
    s8                      m_szErrDesc[SCSDK_MAX_ERRORDESCRIPTION_LEN+1];
}TSCSDK_OperResult;

typedef struct tagSCSDK_DiskState
{
    TSCSDK_DeviceID         m_tDeviceID;                                    //�豸ID
    TSCSDK_DiskDev          m_tDiskDev;                                     //�����豸�ṹ��
    ESCSDK_DiskState        m_eDiskState;                                   //���̵�״̬
}TSCSDK_DiskState;

typedef struct tagSCSDK_PartState
{
    TSCSDK_DeviceID         m_tDeviceID;                                    //�豸ID
    TSCSDK_DiskDev          m_tDiskDev;                                     //�����豸�ṹ��
    ESCSDK_PartState        m_ePartState;                                   //����״̬
}TSCSDK_PartState;

typedef struct tagSCSDK_DiskDevOpState                                              //�����豸����״̬
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //�豸ID
    TSCSDK_DiskDev              m_tDiskDev;                                         //�����豸�ṹ��
    ESCSDK_DiskDevOperation     m_eDiskDevOperation;                                //�����豸����
    u8                          m_byPercent;                                        //�ٷֱ�
    s8                          m_szPartOpDes[SCSDK_MAX_OPDES_LEN+1];               //������������ӵģ�
}TSCSDK_DiskDevOpState;


typedef struct tagSCSDK_ArrayInfo                                               //������Ϣ
{
    TSCSDK_IpAddress     m_tArrayIP;                                            //����IP
    s8                   m_achArrayType[SCSDK_MAX_ARRAY_TYPE_LEN+1];                //�������ͣ�2014.10.16����ʹ��ESCSDK_ArrayTypeö�٣���Ϊֱ���ַ�����������չ.
    u32                  m_dwSendRate;                                          //���緢���ٶ�(Kb/s)
    u32                  m_dwReceiveRate;                                       //��������ٶ�(Kb/s)
    u8                   m_byMemUtil;                                           //�ڴ�ռ����
    u8                   m_byCpuUtil;                                           //cpuռ����
    ESCSDK_ArrayState    m_eArrayState;                                         //����״̬
}TSCSDK_ViewArrayInfo;

typedef struct tagSCSDK_RaidManagerInfo
{
    TSCSDK_IpAddress                m_tArrayIP;                                             //����IP
    s8                              m_szDiskGroupName[SCSDK_MAX_DISKGROUPNAME_LEN+1];       //��������
}TSCSDK_RaidManagerInfo;

typedef struct tagSCSDK_DiskGroupInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                     //����IP�ʹ�������
    ESCSDK_DiskGroupState           m_eDiskGroupState;                                      //������״̬
    ESCSDK_RaidLevel                m_eRaidLevel;                                           //RAID����
    u32                             m_dwDiskBitmap;
    u32                             m_dwSpareBitmap;
}TSCSDK_ViewDiskGroupInfo;

typedef struct tagSCSDK_PhysicalDiskInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                             //����IP�ʹ�������
    u32                             m_dwDiskSlot;                                                   //���̲�λ��
    s8                              m_szVendor[SCSDK_MAX_MANUFAC_LEN+1];                            //������
    s8                              m_szModel[SCSDK_MAX_DEVTYPE_LEN+1];                             //�ͺ�
    u32                             m_dwDiskSize;                                                   //����������M��
    u32                             m_dwDiskState;                                                  //����״̬��ESCSDK_DiskStatus��
    ESCSDK_DiskHealth               m_eDiskHealth;                                                  //���̽���״̬
    u32                             m_dwReallocatedSectors;                                         //���·���������
    BOOL32                          m_bIsBackup;                                                    //�Ƿ��ȱ���
}TSCSDK_ViewPhysicalDiskInfo;

typedef struct tagSCSDK_VirtualDiskInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                 //����IP�ʹ�������
    s8                              m_szVirtualDiskName[SCSDK_MAX_VITUALDISKNAME+1];    //���������
    ESCSDK_VirtualDiskType          m_eVirtualDiskType;                                 //�����������
    u32                             m_dwVirtualDiskSize;                                //�����������(M)
    ESCSDK_VirtualDiskState         m_eVirtualDiskState;                                //�������״̬
    u32                             m_dwCurStateTime;                                   //��ǰ״̬�ĳ���ʱ�� (s)
    u32                             m_dwTargetID;                                       //TargetID
    u32                             m_dwLunID;                                          //LunID
    s8                              m_szLocalDeviceName[SCSDK_MAX_DEVNAME_LEN+1];       //ӳ���豸��
}TSCSDK_ViewVirtualDiskInfo;


//���й����֣�ntf����Ҫ����Ϣ
typedef struct tagSCSDK_UpdateArrayInfo
{
    TSCSDK_DeviceID         m_tDeviceID;                                            //�豸ID
    TSCSDK_ViewArrayInfo    m_tViewArrayInfo;
    ESCSDK_RMEvent          m_eRMEvent;
}TSCSDK_UpdateArrayInfo;

typedef struct tagSCSDK_UpdateDiskGroupInfo
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //�豸ID
    TSCSDK_ViewDiskGroupInfo    m_tViewDiskGroupInfo;
    ESCSDK_RMEvent              m_eRMEvent;
}TSCSDK_UpdateDiskGroupInfo;

typedef struct tagSCSDK_UpdatePhysicalDiskInfo
{
    TSCSDK_DeviceID              m_tDeviceID;                                       //�豸ID
    TSCSDK_ViewPhysicalDiskInfo  m_tViewPhysicalDiskInfo;
    ESCSDK_RMEvent               m_eRMEvent;
}TSCSDK_UpdatePhysicalDiskInfo;

typedef struct tagSCSDK_UpdateVirtualDiskInfo
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //�豸ID
    TSCSDK_ViewVirtualDiskInfo  m_tViewVirtualDiskInfo;
    ESCSDK_RMEvent              m_eRMEvent;
}TSCSDK_UpdateVirtualDiskInfo;

typedef struct tagSCSDK_GetIscsiInitiatorName
{
    s8                          m_szIscsiInitiatorName[SCSDK_MAX_NAME_LEN+1];
}TSCSDK_GetIscsiInitiatorName;

/************************************************************************/
/*          �ص���������                                                */
/************************************************************************/
typedef void (*SCSDKEventCB)(TSCSDK_EventHandle tEventHandle, u8* pbyBuf, u32 dwBufLen, void* pvContext);


/************************************************************************/
/*          �����ӿڶ���                                                */
/************************************************************************/


/*====================================================================
������      :SCSDK_SetEventCallback
����        :ע��ص�����
             1.����SDK������нӿ�Ϊ�첽�ӿڣ�������нӿڵĽ����ͨ��
               ע��Ļص��������أ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :�����ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
----------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���        �޸�����
2012/05/21  1.0         ������        ����
====================================================================*/
ESCSDK_ErrType SCSDK_SetEventCallback(SCSDKEventCB pFunc, void* pvContext);

/*====================================================================
������      ��SCSDK_ConnectServ
����        ������ĳһý��������豸
             1.ͨ������ý��������豸��ID���豸����IP���˿ں�ʹpmc�����ϸ�ý��������豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                        -- [IN]  �����豸ID
             TSCSDK_ConnectMediaServ* ptConnectServ              -- [IN]  ����Ҫ���ӵ�ý���������Ϣ
             TSCSDK_Sequence* ptSequence                         -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ConnectServ (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ConnectServ* ptConnectServ, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_DisconnectServ
����        ���Ͽ�ĳһý��������豸
             1.ͨ������ý��������豸��ID���豸����IP���˿ں�ʹpmc�Ͽ��Ը�ý��������豸������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                           -- [IN]  �����豸ID
             TSCSDK_DisconnectMediaServ* ptDisconnectServ           -- [IN]  ����Ҫ�Ͽ���ý��������豸��Ϣ
             TSCSDK_Sequence* ptSequence                            -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_DisconnectServ (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DisconnectServ* ptDisconnectServ, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_GetDiskInfo
����        ����ȡĳһ�豸�ϴ����б���Ϣ
             1.ͨ�������豸��ID��pmc��ȡ���豸�ϵĴ����б���Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵���������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
==================================================================== */
ESCSDK_ErrType SCSDK_GetDiskInfo(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_EditPart
����        ���༭ĳ���豸��ĳ�����̷���
             1.��ĳ�����̽��з������������������������ܳ���4
             2.����֮��ķֽ���ͨ���ṹ��TSCSDK_PartCfgLine��ʾ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_EditPart* ptEditPart             -- [IN]  �����������������������������ķֽ���
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_EditPart(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_EditPart* ptEditPart, TSCSDK_Sequence* ptSequence);

 /*====================================================================
������      ��SCSDK_ViewLog
����        ���鿴ĳ���̻��߷�������־
             1.�鿴ָ����ĳ�����̻��߷�����ĳһʱ����ڵ���־��
               ͨ��TSCSDK_ViewLogResult�鿴��־������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID         -- [IN]  �����豸ID
             TSCSDK_ViewLog* ptViewLog            -- [IN]  ��������豸�ṹ�壬ʱ���
             TSCSDK_Sequence* ptSequence          -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
  -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ==================================================================== */
ESCSDK_ErrType SCSDK_ViewLog(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ViewLog* ptViewLog,TSCSDK_Sequence* ptSequence);

 /*====================================================================
������      :SCSDK_FormatDiskDev
����        ����ĳ�����豸���и�ʽ��
             1.��ĳ�������豸���и�ʽ�����Ҹ�ʽ����������vbfs��ext3����
             2.ͨ��TSCSDK_DiskDevOpState�Ը�ʽ�����Ƚ��������ϱ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_FormatDiskDev* ptFormatDiskDev       -- [IN]  ��������豸�ṹ�壬��ʽ����
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
  -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ==================================================================== */
 ESCSDK_ErrType SCSDK_FormatDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_FormatDiskDev* ptFormatDiskDev, TSCSDK_Sequence* ptSequence);

 /*====================================================================
������      ��SCSDK_MountDiskDev
����        ���Դ����豸���й���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  �����豸ID
             TSCSDK_MountDiskDev* ptMountDiskDev            -- [IN]  ��������豸�ṹ�壬����·��
             TSCSDK_Sequence* ptSequence                    -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_MountDiskDev (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_MountDiskDev* ptMountDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      :SCSDK_UnmountDiskDev
����        ���Դ����豸����ж��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  �����豸ID
             TSCSDK_UnmountDiskDev* ptUnmountDiskDev        -- [IN]  ��������豸�ṹ�壬ж��·��
             TSCSDK_Sequence* ptSequence                    -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
 ESCSDK_ErrType SCSDK_UnmountDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_UnmountDiskDev* ptUnmountDiskDev, TSCSDK_Sequence* ptSequence);

 /*====================================================================
������      ��SCSDK_FsckDiskDev
����        ���Դ����豸�����޸�
              1.ͨ��TSCSDK_DiskDevOpState���޸����Ƚ��������ϱ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_FsckDiskDev* ptFsckDiskDev           -- [IN]  ��������豸�ṹ�壬�޸�·��
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_FsckDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_FsckDiskDev* ptFsckDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_SetDiskDevUsage
����        �����ô����豸�ķ�����;

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_SetDiskDevUsage* ptSetDiskDevUsage   -- [IN]  ��������豸�ṹ��,����;
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2017/07/19  1.0         ������        ����
====================================================================*/
ESCSDK_ErrType SCSDK_SetDiskDevUsage (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_SetDiskDevUsage* ptSetDiskDevUsage, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_GetNetFSCfg
����        ����ȡ�����õ�NetFS�豸��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2017/07/19  1.0         Ҷ��ɽ        ����
 ====================================================================*/
 ESCSDK_ErrType SCSDK_GetNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      : SCSDK_AddNetFSCfg
����        : ����NetFS�豸
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  �����豸ID
             TSCSDK_AddNetFSCfg* ptAddNetFSCfg          -- [IN]  ��������NetFS��Ϣ
             TSCSDK_Sequence* ptSequence                -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2017/07/19  1.0         Ҷ��ɽ        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_AddNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_AddNetFSCfg* ptAddNetFSCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_DelNetFSCfg
����        ����ĳ��NetFS�豸����ɾ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  �����豸ID
             TSCSDK_DelNetFSCfg* ptDelNetFSCfg          -- [IN]  ����ɾ��NetFS�豸��Ϣ;ֻ��Ҫm_achServerPath;m_achClientPath ������Ϊ��
             TSCSDK_Sequence* ptSequence                -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2017/07/19  1.0         Ҷ��ɽ        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelNetFSCfg* ptDelNetFSCfg, TSCSDK_Sequence* ptSequence);


/*====================================================================
������      ��SCSDK_GetNetFSExportList
����        ����ĳ��NFS�������Ŀɹ����б�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                       -- [IN]  �����豸ID
             TSCSDK_GetNetFSExportCond* ptGetNetFSExportCond    -- [IN]  NetFS������IP�Լ�����ҳ��������
             TSCSDK_Sequence* ptSequence                        -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2017/07/19  1.0         Ҷ��ɽ        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetNetFSExportList(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_GetNetFSExportCond *ptGetNetFSExportCond, TSCSDK_Sequence* ptSequence);


/*====================================================================
������      ��SCSDK_GetIscsiCfg
����        ����ȡĳһ���豸��������Ϣ
              1.�����豸ID���ɻ�ȡ���豸����ӵ�е�������Ϣ���������ߵĺ����ߵģ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
 ESCSDK_ErrType SCSDK_GetIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_AddIscsiCfg
����        ����ĳ���豸����ĳһ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  �����豸ID
             TSCSDK_AddIscsiCfg* ptAddIscsiCfg          -- [IN]  ��������������Ϣ
             TSCSDK_Sequence* ptSequence                -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_AddIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_AddIscsiCfg* ptAddIscsiCfg, TSCSDK_Sequence* ptSequence);

 /*====================================================================
������      ��SCSDK_DelIscsiCfg
����        ����ĳ���豸�ϵ�ĳһ���н���ɾ��
              1.�������������������״̬�����ȶϿ�����֮�����ɾ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  �����豸ID
             TSCSDK_DelIscsiCfg* ptDelIscsiCfg          -- [IN]  ����ɾ��������Ϣ
             TSCSDK_Sequence* ptSequence                -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelIscsiCfg* ptDelIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_DisconnectIscsiCfg
����        ����ĳ���豸�϶Ͽ�ĳһ��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                       -- [IN]  �����豸ID
             TSCSDK_DisconnectIscsiCfg* ptDisconnectIscsiCfg    -- [IN]  ����Ͽ�������Ϣ
             TSCSDK_Sequence* ptSequence                        -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_DisconnectIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DisconnectIscsiCfg* ptDisconnectIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_ConnectIscsiCfg
����        ����ĳ���豸������ĳһ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  �����豸ID
             TSCSDK_ConnectIscsiCfg* ptConnectIscsiCfg      -- [IN]  ����Ҫ���ӵ�������Ϣ
             TSCSDK_Sequence* ptSequence                    -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ConnectIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ConnectIscsiCfg* ptConnectIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_StartIscsi
����        ������ĳһ�豸�ϵ�iscsi����
              1.Ҫָ�������ĸ��豸�ϵ�iscsi������ҪҪ�����豸ID
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                           -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                            -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_StartIscsi(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_StopIscsi
����        ��ֹͣĳһ�豸�ϵ�iscsi����
              1.Ҫָ��ֹͣ�ĸ��豸�ϵ�iscsi������ҪҪ�����豸ID
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                           -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_StopIscsi (TSCSDK_DeviceID*  ptDeviceID,TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_ViewArrayInfo
����        ���鿴������Ϣ

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_Sequence* ptSequence                           -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewArrayInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_ViewDiskGroupInfo
����        ���鿴��������Ϣ

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_IpAddress* ptArrayIP                           -- [IN]  ����IP
             TSCSDK_Sequence* ptSequence                           -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewDiskGroupInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_IpAddress* ptArrayIP, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_ViewPhysicalDiskInfo
����        ���鿴���������Ϣ

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_IpAddress* ptArrayIP                           -- [IN]  ����IP
             TSCSDK_Sequence* ptSequence                           -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewPhysicalDiskInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_IpAddress* ptArrayIP, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_ViewVirtualDiskInfo
����        ���鿴���������Ϣ

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_RaidManagerInfo* ptRaidManagerInfo             -- [IN]  ����IP�ʹ�������
             TSCSDK_Sequence* ptSequence                           -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewVirtualDiskInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_RaidManagerInfo* ptRaidManagerInfo, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_DelOfflineDisk
����        ��ɾ�����ߴ���

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  �����豸ID
             TSCSDK_DelDisk*   ptDelDisk                           -- [IN]  ����ID���Ƿ�ɾ��¼���¼���
             TSCSDK_Sequence*  ptSequence                          -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelOfflineDisk (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelDisk* ptDelDisk, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_CancleDiskOp
����        ��ȡ�������еĴ��̲���

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:TSCSDK_DeviceID*       ptDeviceID                         -- [IN]  �����豸ID
             TSCSDK_DiskDev*        ptDiskDev                          -- [IN]  ������
             TSCSDK_Sequence*       ptSequence                         -- [OUT] �������к�
����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_CancleDiskOp (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DiskDev* ptDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
������      ��SCSDK_GetEventDescriptionFromEventID
����        ����Ϣ����

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:u32 dwEventID      -- [IN]  �����¼�ID

����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2012/05/21  1.0         ������        ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetEventDescriptionFromEventID(u32 dwEventID, s8* szEventDescription, u32 dwLen);

/*====================================================================
������      ��SCSDK_GetIscsiInitiatorName
����        ����ȡiscsi�ķ�������

�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
                TSCSDK_DeviceID*            ptDeviceID                  [IN]  �����豸ID
                TSCSDK_Sequence*        ptSequence                          [OUT] �������к�

����ֵ˵��  �������ɹ�����ESCSDK_ERRTYPE_OK,ʧ�ܷ��ش�����
 -----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2013/05/21  1.0         ����       ����
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetIscsiInitiatorName (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);


#endif
                                                            // __SCSDK_H__
