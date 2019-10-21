/*****************************************************************************
模块名      : SCSDK(SYSTEM_CONFIGURATION_SDK)
文件名      : SCSDK.H
相关文件    : kdvtype.h
文件实现功能:
作者        : 张文新
版本        : <Copyright(c) 2003-2012 Suzhou Keda Technology Co.,Ltd. All right reserved.>
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
******************************************************************************/
#ifndef __SCSDK_H__
#define __SCSDK_H__

#include "kdvtype.h"


/*====================================================================
                        宏定义
====================================================================*/
#define SCSDK_MAX_DEVNAME_LEN               32                      //设备名最大长度
#define SCSDK_MAX_DEVID_LEN                 32                      //设备ID最大长度
#define SCSDK_MAX_DEVTYPE_LEN               32                      //设备类型最大长度
#define SCSDK_MAX_MANUFAC_LEN               32                      //设备厂商名最大长度
#define SCSDK_MAX_NAME_LEN                  512                     //名字最大长度
#define SCSDK_MAX_MOUNTPATH_LEN             50                      //挂载路径名最大长度
#define SCSDK_MAX_ERRORDESCRIPTION_LEN      256                     //错误描述的最大长度
#define SCSDK_MAX_PART_NUM                  8                       //磁盘最大分区数
#define SCSDK_MAX_LOGDESCRIPTION_LEN        256                     //错误描述的最大长度
#define SCSDK_MAX_DISKGROUPNAME_LEN         64                      //最大磁盘组名
#define SCSDK_MAX_VITUALDISKNAME            256                     //最大虚拟磁盘名
#define SCSDK_MAX_PASSWORD_LEN              64                      //密码的最大长度
#define SCSDK_MAX_DISKSOURCE_LEN            256                     //磁盘来源最大长度(64不够大)
#define SCSDK_MAX_ADDRESS_LEN               40                      //地址最大长度
#define SCSDK_MAX_OPDES_LEN                 256                     //操作描述（添加的）
#define SCSDK_MAX_ARRAY_TYPE_LEN            31                      //阵列类型最大长度
#define SCSDK_MAX_DISK_VENDOR_LEN           32
#define SCSDK_MAX_MOUNTOPTION_LEN           128

#define SCSDK_DISK_VENDOR_KDM               "kdm"
/*====================================================================
                        枚举定义
====================================================================*/
typedef enum
{
    ESCSDK_ERRTYPE_OK    = 0,                                               //返回成功,服务端向客户端发送ack
    ESCSDK_ERRTYPE_PASSWORD_ERR,                                            //密码错误，鉴权失败
    ESCSDK_ERRTYPE_HAS_USER,                                                //用户已登录或者服务端IP输入不正确
    ESCSDK_ERRTYPE_CUSTOMER_NOT_EXIST,                                      //客户信息不存在
    ESCSDK_ERRTYPE_LOGIN_FAIL,                                              //登陆失败
    ESCSDK_ERRTYPE_NODEINVALID,                                             //node不存在
    ESCSDK_ERRTYPE_GETDISKFAIL,                                             //获取磁盘信息失败
    ESCSDK_ERRTYPE_GET_ARRAY_INFO_FAILED,                                   //获取阵列信息失败
    ESCSDK_ERRTYPE_GETDISKGROUPFAIL,                                        //获取磁盘组信息失败
    ESCSDK_ERRTYPE_GETPHYSICALDISKFAIL,                                     //获取物理磁盘信息失败
    ESCSDK_ERRTYPE_GETVIRTUALDISKFAIL,                                      //获取虚拟磁盘信息失败
    ESCSDK_ERRTYPE_EDITPARTFAIL,                                            //分区命令执行失败，可能原因：分区命令参数不合法或其他系统原因导致
    ESCSDK_ERRTYPE_OPERATION_IN_PROGRESS,                                   //分区上有磁盘操作正在进行,等操作完成再试
    ESCSDK_ERRTYPE_DISK_STATUS_ERROR,                                       //磁盘状态错误，或有操作正在进行
    ESCSDK_ERRTYPE_MOUNTED,                                                 //分区是挂载状态
    ESCSDK_ERRTYPE_DEL_UASDB_FAILED,                                        //删除数据库中录像记录失败，检查相关脚本是否存在
    ESCSDK_ERRTYPE_NOT_EXIST,                                               //磁盘或分区已经不存在
    ESCSDK_ERRTYPE_FDISK_NOT_SUPPORT,                                       //工具不支持对容量超过2T且扇区大小为非512字节的磁盘进行分区操作
    ESCSDK_ERRTYPE_FORMATDISKFAIL,                                          //格式化磁盘失败
    ESCSDK_ERRTYPE_DISK_CTRL_BUZY,                                          //磁盘操作线程忙，请稍候再试
    ESCSDK_ERRTYPE_MOUNTFAIL,                                               //挂载命令执行失败，可能原因：断开磁盘或重启服务器时，没有正常卸载磁盘，需要进行修复
    ESCSDK_ERRTYPE_UNFORMATED,                                              //分区未格式化
    ESCSDK_ERRTYPE_DIR_IN_USE,                                              //分区挂载冲突，同名目录已经在挂载列表中，请检查下线磁盘是否被正确卸载
    ESCSDK_ERRTYPE_DISK_NUM_OVERLOAD,                                       //数据盘分区总数超过录像机支持的最大个数,多余的磁盘将不被使用,请移除
    ESCSDK_ERRTYPE_UNMOUNTFAIL,                                             //卸载命令执行失败，可能原因：该分区正在被使用，等待自动卸载
    ESCSDK_ERRTYPE_HAS_UMOUNT,                                              //分区已经卸载
    ESCSDK_ERRTYPE_FSCKFAIL,                                                //修复命令执行失败，可能原因：磁盘有异常
    ESCSDK_ERRTYPE_REBOOT_REQUIRED,                                         //系统需要重启
    ESCSDK_ERRTYPE_FS_LEFT_UNCORRECTED,                                     //文件系统无法修复
    ESCSDK_ERRTYPE_CANCELED,                                                //命令执行被取消
    ESCSDK_ERRTYPE_GETCFGFAIL,                                              //获取iscsi列表失败
    ESCSDK_ERRTYPE_ADDISCSIFAIL,                                            //添加iscsi阵列失败
    ESCSDK_ERRTYPE_SCSI_CONFIG_FAILED,                                      //设置SCSI配置文件失败错误
    ESCSDK_ERRTYPE_DELISCSIFAIL,                                            //删除iscsi阵列失败
    ESCSDK_ERRTYPE_DISCONNECTISCSIFAIL,                                     //断开iscsi阵列失败
    ESCSDK_ERRTYPE_CONNECTISCSIFAIL,                                        //重连iscsi阵列失败
    ESCSDK_ERRTYPE_STARTISCSIFAIL,                                          //启动iscsi服务失败
    ESCSDK_ERRTYPE_STOPISCSIFAIL,                                           //停止iscsi服务失败
    ESCSDK_ERRTYPE_DISKGROUPNAMEINVALID,                                    //磁盘组名无效
    ESCSDK_ERRTYPE_CANCLEDISKOPFAIL,                                        //删除磁盘操作失败
    ESCSDK_ERRTYPE_DELDISKFAIL,                                             //删除下线磁盘失败
    ESCSDK_ERRTYPE_HASCONSERV,                                              //已经连接上服务器
    ESCSDK_ERRTYPE_INNERERR,                                                //内部错误（包括创建app失败，函数的参数指针为空，则直接返回）
    ESCSDK_ERRTYPE_OPERTIMEOUT,                                             //操作超时
    ESCSDK_ERRTYPE_PARAMINVALID,                                            //参数无效
    ESCSDK_ERRTYPE_DISCONNECTNODEFAIL,                                      //释放节点失败
    ESCSDK_ERRTYPE_DISCONNECTNODE,                                          //已经释放node
    ESCSDK_ERRTYPE_POSTFAIL,                                                //osppost失败
    ESCSDK_ERRTYPE_DEVICE_NOTCONNECTED,                                     //设备尚未连接
    ESCSDK_ERRTYPE_OUT_OF_MEMORY,                                           //内存不足
    ESCSDK_ERRTYPE_GET_ISCSI_INITIATORNAME_FAIL,                            //获取ISCSI的发起者名失败
    ESCSDK_ERRTYPE_FS_LIMITED,                                              //系统限制，单分区最大8TB
    ESCSDK_ERRTYPE_VBFS_IS_DISABLED,                                        //VBFS文件系统被禁用
    ESCSDK_ERRTYPE_DISK_SIZE_OVERLOAD,                                      //系统限制，超过16TB的磁盘无法正常访问，如果是iscsi盘，请重新划分
    ESCSDK_ERRTYPE_SET_DISK_USAGE_FAILED,                                   //设置磁盘用途失败
    ESCSDK_ERRTYPE_GET_NETFS_FAILED,                                        //获取NetFS失败
    ESCSDK_ERRTYPE_ADD_NETFS_FAILED,                                        //添加NetFS失败
    ESCSDK_ERRTYPE_DEL_NETFS_FAILED,                                        //删除NetFS失败
    ESCSDK_ERRTYPE_GET_NETFS_EXPORT_LIST_FAILED,                            //获取NetFS服务器导出列表
}ESCSDK_ErrType;

typedef enum
{
    ESCSDK_IPTYPE_IPV4,
    ESCSDK_IPTYPE_IPV6,
    ESCSDK_IPTYPE_UNKNOWN,
}ESCSDK_IpType;

typedef enum
{
    ESCSDK_DISKSTATE_ONLINE,                                                    //在线
    ESCSDK_DISKSTATE_OFFLINE,                                                   //下线
    ESCSDK_DISKSTATE_INACTIVE,                                                  //未启用
    ESCSDK_DISKSTATE_UNKNOWN,                                                   //未知
}ESCSDK_DiskState;

typedef enum
{
    ESCSDK_ARRAYSTATE_ONLINE,                                                   //在线
    ESCSDK_ARRAYSTATE_OFFLINE,                                                  //下线
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
    ESCSDK_ISCSISTATE_RUNNING,      //正在运行
    ESCSDK_ISCSISTATE_STOPED,       //已经停止
    ESCSDK_ISCSISTATE_UNSUPPORTED,  //不支持
    ESCSDK_ISCSISTATE_UNKNOWN,      //获取状态失败时为此结果
}ESCSDK_IscsiState;

typedef enum
{
    ESCSDK_PARTSTATE_OFFLINE,                                                   //下线
    ESCSDK_PARTSTATE_UNFORMATTED,                                               //未格式化
    ESCSDK_PARTSTATE_FORMATTED,                                                 //已格式化
    ESCSDK_PARTSTATE_MOUNTED,                                                   //已挂载
    ESCSDK_PARTSTATE_UNMOUNTED,                                                 //已卸载
    ESCSDK_PARTSTATE_UNKNOWN,                                                   //未知状态

}ESCSDK_PartState;

typedef enum
{
    ESCSDK_PARTUSAGE_DATA,          //数据
    ESCSDK_PARTUSAGE_SYSTEM,        //系统
    ESCSDK_PARTUSAGE_BACKUP,        //备份
    ESCSDK_PARTUSAGE_SATA,          //内置sata
    ESCSDK_PARTUSAGE_APPDADA,       //应用数据
    ESCSDK_PARTUSAGE_GPSDATA,       //GPS数据
    ESCSDK_PARTUSAGE_MAP,           //离线地图
    ESCSDK_PARTUSAGE_UNKNOWN = -1
}ESCSDK_PartUsage;

typedef enum
{
    ESCSDK_DiskDevOperation_UNKNOWN,                                                    //无操作
    ESCSDK_DiskDevOperation_STOPOPERATION,                                              //停止操作
    ESCSDK_DiskDevOperation_WAITFORMAT,                                                 //等待格式化
    ESCSDK_DiskDevOperation_FORMATTING,                                                 //正在格式化
    ESCSDK_DiskDevOperation_WAITFSCK,                                                   //等待修复
    ESCSDK_DiskDevOperation_FSCKING,                                                    //正在修复
    ESCSDK_DiskDevOperation_WAITMOUNT,                                                  //等待挂载
    ESCSDK_DiskDevOperation_MOUNTING,                                                   //正在挂载
    ESCSDK_DiskDevOperation_WAITUNMOUNT,                                                //等待卸载
    ESCSDK_DiskDevOperation_UNMOUNTING,                                                 //正在卸载
    ESCSDK_DiskDevOperation_SYSINVALID,                                                 //被手动卸载，已卸载状态(10.23添加)
    ESCSDK_DiskDevOperation_ERROR,                                                      //格式化失败或修复失败
    ESCSDK_DiskDevOperation_REBOOT,
    ESCSDK_DiskDevOperation_UNUSED,                                                     //未使用,如磁盘数过多
}ESCSDK_DiskDevOperation;

typedef enum
{
    ESCSDK_FORMATTYPE_VBFS,                                                     //格式化类型为vbfs
    ESCSDK_FORMATTYPE_EXT3,                                                     //格式化类型为ext3
    ESCSDK_FORMATTYPE_UNKNOWN,                                                  //无法识别
}ESCSDK_FormatType;

typedef enum
{
    ESCSDK_LOGTYPE_EXCEPTION,                                                   //日志类型显示异常
    ESCSDK_LOGTYPE_ERROR,                                                       //错误
    ESCSDK_LOGTYPE_WARNING,                                                     //警告
    ESCSDK_LOGTYPE_INFO,                                                        //信息（上线，挂载，卸载等）
}ESCSDK_LogType;

typedef enum                                                                    //阵列类型
{
    ESCSDK_ARRAYTYPE_VS200,
    ESCSDK_ARRAYTYPE_VS400,
    ESCSDK_ARRAYTYPE_VS200E,
    ESCSDK_ARRAYTYPE_VS200C,
    ESCSDK_AARAYTYPE_UNKNOWN,
}ESCSDK_ArrayType;

typedef enum                                                                    //磁盘组状态
{
    ESCSDK_DISKGROUPSTATE_CLEAN,
    ESCSDK_DISKGROUPSTATE_DEGRADED,
    ESCSDK_DISKGROUPSTATE_RECOVERING,
    ESCSDK_DISKGROUPSTATE_FAILED,
    ESCSDK_DISKGROUPSTATE_RESYING,
    ESCSDK_DISKGROUPSTATE_UNKNOWN,
}ESCSDK_DiskGroupState;

typedef enum                                                                    //RAID级别
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

typedef enum                                                                    //磁盘健康状态
{
    ESCSDK_DISKHEALTH_OK,
    ESCSDK_DISKHEALTH_BAD,
    ESCSDK_DISKHEALTH_UNKNOWN,
}ESCSDK_DiskHealth;

typedef enum                                                                    //虚拟磁盘状态
{
    ESCSDK_VIRTUALDISKSTATE_ESTABLISHED,
    ESCSDK_VIRTUALDISKSTATE_DROPPED,
    ESCSDK_VIRTUALDISKSTATE_UNKNOWN,
}ESCSDK_VirtualDiskState;

typedef enum                                                                    //虚拟磁盘类型
{
    ESCSDK_VIRTUALDISKTYPE_ISCSI,
    ESCSDK_VIRTUALDISKTYPE_UNKNOWN,
}ESCSDK_VirtualDiskType;

typedef enum            //阵列管理模块事件
{
    ESCSDK_RMEVENT_ADD,
    ESCSDK_RMEVENT_UPDATE,
    ESCSDK_RMEVENT_DEL,
}ESCSDK_RMEvent;

typedef enum
{
    ESCSDK_DISKSTATUS_EMPTY        = 0x01,         /* 槽位无硬盘 */
    ESCSDK_DISKSTATUS_FREE         = 0x02,         /* 槽位有硬盘，但是没有被使用 */
    ESCSDK_DISKSTATUS_USEDFORGROUP = 0x03,         /* 槽位有硬盘，作为磁盘组已经使用 */
    ESCSDK_DISKSTATUS_USEDFORSPARE = 0x04,         /* 槽位有硬盘，作为热备盘使用 */
    ESCSDK_DISKSTATUS_BAD          = 0x05,         /* 槽位有硬盘，但是已经损坏 */
}ESCSDK_DiskStatus;

typedef enum
{
    ESCSDK_ABNCODE_DISK_NORMAL = 0,             //磁盘正常
    ESCSDK_ABNCODE_DISK_RO = 1<<0,              //文件系统只读
    ESCSDK_ABNCODE_FS_ERROR = 1<<1,             //文件系统错误
}ESCSDK_AbnCode;

typedef enum              // 挂载文件系统类型
{
    ESCSDK_MOUNTTYPE_NULL,  // 不指定
    ESCSDK_MOUNTTYPE_NFS,
    ESCSDK_MOUNTTYPE_CIFS
}ESCSDK_MountType;
/************************************************************************/
/*                回调事件类型定义                                      */
/************************************************************************/
typedef enum
{
    SCSDK_EVENT_BEGIN                    = 2000,

    SCSDK_EVENT_GET_DISKINFO                = SCSDK_EVENT_BEGIN + 1,       //TSCSDK_OperResult+u32(搜索到的磁盘数量)
    SCSDK_EVENT_UPDATE_DISKINFO_NTF         = SCSDK_EVENT_BEGIN + 2,       //TSCSDK_DeviceID+TSCSDK_DiskInfo
    SCSDK_EVENT_UPDATE_PARTINFO_NTF         = SCSDK_EVENT_BEGIN + 3,       //TSCSDK_DeviceID+TSCSDK_PartInfo
    SCSDK_EVENT_UPDATE_DISKDEVOPSTATE_NTF   = SCSDK_EVENT_BEGIN + 4,       //TSCSDK_DiskDevOpState(某一设备上的磁盘分区操作状态,包括格式化进度上报等)
    SCSDK_EVENT_UPDATE_IOUTIL_NTF           = SCSDK_EVENT_BEGIN + 5,       //TSCSDK_IoUtil(某一设备上的磁盘分区使用率)
    SCSDK_EVENT_VIEW_LOG                    = SCSDK_EVENT_BEGIN + 6,       //TSCSDK_OperResult+TSCSDK_ViewLogHeadInfo(总数+当前数N+结束标记)+N*TSCSDK_ViewLogResult
    SCSDK_EVENT_EDIT_PART                   = SCSDK_EVENT_BEGIN + 7,       //TSCSDK_OperResult
    SCSDK_EVENT_FORMAT_DISKDEV              = SCSDK_EVENT_BEGIN + 8,       //TSCSDK_OperResult
    SCSDK_EVENT_MOUNT_DISKDEV               = SCSDK_EVENT_BEGIN + 9,       //TSCSDK_OperResult
    SCSDK_EVENT_UNMOUNT_DISKDEV             = SCSDK_EVENT_BEGIN + 10,      //TSCSDK_OperResult
    SCSDK_EVENT_FSCK_DISKDEV                = SCSDK_EVENT_BEGIN + 11,      //TSCSDK_OperResult

    //TSCSDK_OperResult + TSCSDK_GetIscsiCfgHeadInfo(搜索到的阵列个数+总的状态) + N * TSCSDK_GetIscsiCfgResult
    SCSDK_EVENT_GET_ISCSICFG                = SCSDK_EVENT_BEGIN + 12,
    SCSDK_EVENT_ADD_ISCSICFG                = SCSDK_EVENT_BEGIN + 13,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_ISCSICFG                = SCSDK_EVENT_BEGIN + 14,      //TSCSDK_OperResult
    SCSDK_EVENT_DISCONNECT_ISCSICFG         = SCSDK_EVENT_BEGIN + 15,      //TSCSDK_OperResult
    SCSDK_EVENT_CONNECT_ISCSICFG            = SCSDK_EVENT_BEGIN + 16,      //TSCSDK_OperResult
    SCSDK_EVENT_START_ISCSI                 = SCSDK_EVENT_BEGIN + 17,      //TSCSDK_OperResult
    SCSDK_EVENT_STOP_ISCSI                  = SCSDK_EVENT_BEGIN + 18,      //TSCSDK_OperResult
    SCSDK_EVENT_VIEW_ARRAYINFO              = SCSDK_EVENT_BEGIN + 19,      //TSCSDK_OperResult+u32(搜索到的阵列个数)TSCSDK_ViewArrayInfo
    SCSDK_EVENT_VIEW_DISKGROUPINFO          = SCSDK_EVENT_BEGIN + 20,      //TSCSDK_OperResult+u32(搜索到的磁盘组个数)TSCSDK_ViewDiskGroupInfo
    SCSDK_EVENT_VIEW_PHYSICALDISKINFO       = SCSDK_EVENT_BEGIN + 21,      //TSCSDK_OperResult+u32(搜索到的物理磁盘个数)TSCSDK_ViewPhysicalDiskInfo
    SCSDK_EVENT_VIEW_VIRTUALDISKINFO        = SCSDK_EVENT_BEGIN + 22,      //TSCSDK_OperResult+u32(搜索到的虚拟磁盘个数)TSCSDK_ViewVirtualDiskInfo
    SCSDK_EVENT_CONNECT_SERV                = SCSDK_EVENT_BEGIN + 23,      //TSCSDK_OperResult
    SCSDK_EVENT_DISCONNECT_SERV             = SCSDK_EVENT_BEGIN + 24,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_DISK                    = SCSDK_EVENT_BEGIN + 25,      //TSCSDK_OperResult
    SCSDK_EVENT_CANCLE_DISKOP               = SCSDK_EVENT_BEGIN + 26,      //TSCSDK_OperResult

    SCSDK_EVENT_UPDATE_ARRAYINFO_NTF        = SCSDK_EVENT_BEGIN + 27,      //TSCSDK_UpdateArrayInfo
    SCSDK_EVENT_UPDATE_DISKGROUPINFO_NTF    = SCSDK_EVENT_BEGIN + 28,      //TSCSDK_UpdateDiskGroupInfo
    SCSDK_EVENT_UPDATE_PHYSICALDISKINFO_NTF = SCSDK_EVENT_BEGIN + 29,      //TSCSDK_UpdatePhysicalDiskInfo
    SCSDK_EVENT_UPDATE_VIRTUALDISKINFO_NTF  = SCSDK_EVENT_BEGIN + 30,      //TSCSDK_UpdateVirtualDiskInfo
    SCSDK_EVENT_SERV_OFFLINE_NTF            = SCSDK_EVENT_BEGIN + 31,      //TSCSDK_DeviceID
//  SCSDK_EVENT_UPDATE_DISKSTATE_NTF        = SCSDK_EVENT_BEGIN + 32,      //TSCSDK_DiskState(某一设备上磁盘的状态)(后期修改优化scs)
//  SCSDK_EVENT_UPDATE_PARTSTATE_NTF        = SCSDK_EVENT_BEGIN + 33,      //TSCSDK_PartState(某一设备上的磁盘分区状态)
//  SCSDK_EVENT_UPDATE_ABN_NTF              = SCSDK_EVENT_BEGIN + 34,      //TSCSDK_Abn(某一设备上的磁盘分区异常情况，若异常代码返回值为0，则表示异常恢复)

    SCSDK_EVENT_GET_ISCSI_INITIATORNAME     = SCSDK_EVENT_BEGIN + 35,       //TSCSDK_OperResult + TSCSDK_GetIscsiInitiatorName
    SCSDK_EVENT_SET_DISKUSAGE               = SCSDK_EVENT_BEGIN + 36,      //TSCSDK_OperResult
    SCSDK_EVENT_GET_NETFSCFG                = SCSDK_EVENT_BEGIN + 37,      //TSCSDK_OperResult + u32(已配置的NetFS个数) + TSCSDK_GetNetFSRunCfg
    SCSDK_EVENT_ADD_NETFSCFG                = SCSDK_EVENT_BEGIN + 38,      //TSCSDK_OperResult
    SCSDK_EVENT_DEL_NETFSCFG                = SCSDK_EVENT_BEGIN + 39,      //TSCSDK_OperResult
    SCSDK_EVENT_GET_NETFSEXPORTLIST         = SCSDK_EVENT_BEGIN + 40,      //TSCSDK_OperResult + u32(NetFS服务器导出目录总个数) + u32(本次获取的实际目录个数) + TSCSDK_GetNetFSExport
    SCSDK_EVENT_END,

}ESCSDK_EventType;



/*====================================================================
                        结构体定义
====================================================================*/
typedef struct tagSCSDK_DelDisk
{
    u16  m_wDiskID;
    u8   m_byVideoRecordFlag; // 0表示不需要删除设备ID对应的录像记录 1表示需要删除设备ID对应的录像记录
}TSCSDK_DelDisk;

typedef struct tagSCSDK_DeviceID                                                //设备ID的结构体
{
    s8                 m_szDeviceID[SCSDK_MAX_DEVID_LEN+1];
}TSCSDK_DeviceID;

typedef struct tagSCSDK_IpAddress
{
    ESCSDK_IpType   m_eIpType;
    s8              m_szAddress[SCSDK_MAX_ADDRESS_LEN+1];
    u16             m_wPrefixLenth;
}TSCSDK_IpAddress;

typedef struct tagSCSDK_DiskDev                                         //磁盘设备结构体
{
    u32                  m_dwPartID;                                   //分区ID
    u16                  m_wDiskID;                                     //磁盘ID,因为磁盘下线后，磁盘名是可以被复用的，
                                                                        //以防客户端与服务端信息的不一致或延时导致的误操作。加这个唯一ID来进行校验设备名
    s8                   m_szDevName[SCSDK_MAX_NAME_LEN+1];             //可以是分区名也可以是磁盘名，但是DiskID一定是磁盘的
}TSCSDK_DiskDev;

typedef struct tagSCSDK_ConnectServ                                     //连接，断开媒体服务器的结构体
{
    s8                 m_szDeviceName[SCSDK_MAX_DEVNAME_LEN+1];         //设备名
    TSCSDK_IpAddress   m_tDeviceIP;                                     //设备IP
    u16                m_wDevicePort;                                   //设备端口
}TSCSDK_ConnectServ,TSCSDK_DisconnectServ;

typedef struct tagSCSDK_IoUtil                                              //IO使用率结构体
{
    TSCSDK_DeviceID        m_tDeviceID;                                     //设备ID
    TSCSDK_DiskDev         m_tDiskDev;                                      //磁盘设备结构体
    u8                     m_byIoUtil;                                      //IO使用率
}TSCSDK_IoUtil;

typedef struct tagSCSDK_Abn                                                 //异常次数和异常代码
{
    TSCSDK_DeviceID        m_tDeviceID;                                     //设备ID
    TSCSDK_DiskDev         m_tDiskDev;                                      //磁盘设备结构体
    u32                    m_dwAbnNum;                                      //异常次数
    s32                    m_nAbnCode;                                      //异常代码(需要定义异常代码的常量)
}TSCSDK_Abn;

typedef struct tagSCSDK_PartInfo                                           //分区信息结构体
{
    u32                      m_dwPartID;                                   //分区ID
    s8                       m_szPartName[SCSDK_MAX_NAME_LEN+1];           //分区名
    u16                      m_wPartDiskID;                                //磁盘ID
    u32                      m_dwPartsize;                                 //分区大小（单位：M）
    u32                      m_dwFreeSpace;                                //可用空间大小 （单位：M）
    ESCSDK_PartState         m_ePartState;                                 //分区状态
    s8                       m_szFormatType[SCSDK_MAX_DEVTYPE_LEN+1] ;          //分区格式（vbfs，ext3）
    u32                      m_dwAbnNum;                                   //异常次数
    s32                      m_nAbnCode;                                   //异常代码(需要定义异常代码的常量)
    s8                       m_achMountPath[SCSDK_MAX_MOUNTPATH_LEN+1];    //挂载路径,由uuid自动生成
    u32                      m_dwStart;                                    //起始块位置
    u32                      m_dwEnd;                                      //结束块位置
    u8                       m_byPercent;                                  //百分比
    s8                       m_szPartOpDes[SCSDK_MAX_OPDES_LEN+1];         //操作描述（添加的）
    ESCSDK_DiskDevOperation  m_eDiskDevOperation;                          //磁盘设备操作
    ESCSDK_PartUsage         m_ePartUsage;                                 //用途类型
    u8                       m_byIoUtil;                                   //IO使用率
}TSCSDK_PartInfo;


typedef struct tagSCSDK_DiskInfo                                      //磁盘信息结构体
{
    u16                m_wDiskID;                                     //磁盘ID
    s8                 m_szDiskName[SCSDK_MAX_NAME_LEN+1];            //磁盘名
    u32                m_dwDiskSize;                                  //总容量（单位：M）
    u32                m_dwDiskAbnNum;                                //异常次数
    ESCSDK_DiskState   m_eDiskState;                                  //磁盘状态
    s8                 m_achDiskSource[SCSDK_MAX_DISKSOURCE_LEN+1];   //磁盘来源
    u32                m_dwCylinders;                                 //磁盘柱面数
    u8                 m_byDiskPartNum;                               //分区数 (存储盘最大分区数为4，系统盘最大分区数为8)
    TSCSDK_PartInfo    m_atPartInfo[SCSDK_MAX_PART_NUM];              //分区信息
    s8                 m_szDiskVendor[SCSDK_MAX_DISK_VENDOR_LEN+1];   //存储设备厂商，add by zhichun 201600920
    ESCSDK_DeviceClass m_eDeviceClass;                                //设备类型
}TSCSDK_DiskInfo;

typedef struct tagSCSDK_PartCfgLine                                             //各个分区的分界线
{
    u32                m_dwCylinderStart;                                       //起始柱面
    u32                m_dwCylinders;                                           //磁盘柱面数
    u32                m_dwPartID;                                              //分区ID
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

typedef struct tagSCSDK_TimeRange                                                   //时间段的结构体
{
    u32         m_dwStartTime;
    u32         m_dwEndTime;

}TSCSDK_TimeRange;

 typedef struct tagSCSDK_EdiPart                                            //将磁盘划分成几个分区的结构体
 {
    TSCSDK_DiskDev          m_tDiskDev;                                     //磁盘设备结构体
    u8                      m_byDiskPartNum;                                //分区个数（存储盘最大不超过4，系统盘不超过6）
    TSCSDK_PartCfgLine      m_atPartCfgLine[SCSDK_MAX_PART_NUM];
 }TSCSDK_EditPart;

typedef struct tagSCSDK_ViewLog                                              //日志的结构体
{
    TSCSDK_DiskDev          m_tDiskDev;                                     //磁盘设备结构体
    TSCSDK_TimeRange        m_tTimeRange;                                   //时间段
}TSCSDK_ViewLog;

typedef struct tagSCSDK_LogHeadInfo
{
    u32     m_dwTotal;                                                      //日志总数
    u32     m_dwNum;                                                        //当前数N
    BOOL32  m_dwEnd;                                                        //结束标记
}TSCSDK_ViewLogHeadInfo;

typedef struct tagSCSDK_ViewLogResult
{
    TSCSDK_Time         m_tTime;                                                    //日志的时间
    ESCSDK_LogType      m_eLogType;                                                 //日志类型
    s8                  m_szLogResultDes[SCSDK_MAX_LOGDESCRIPTION_LEN+1];           //日志结果描述
}TSCSDK_ViewLogResult;

typedef struct tagSCSDK_FormatDiskDev                                            //格式化的结构体
{
    TSCSDK_DiskDev          m_tDiskDev;                                         //磁盘设备结构体
    ESCSDK_FormatType       m_eFormatType;                                      //格式化类型

}TSCSDK_FormatDiskDev;

typedef struct tagSCSDK_MountDiskDev                                              //挂载，卸载，修复分区的结构体
{
    TSCSDK_DiskDev          m_tDiskDev;                                           //磁盘设备结构体
    s8                      m_achDiskDevMountPath[SCSDK_MAX_MOUNTPATH_LEN+1];     //挂载路径(为空时，由UUID自动生成)
}TSCSDK_MountDiskDev,TSCSDK_UnmountDiskDev,TSCSDK_FsckDiskDev;

typedef struct tagSCSDK_SetDiskDevUsage                                          //设置磁盘用途的结构体
{
    TSCSDK_DiskDev          m_tDiskDev;                                         //磁盘设备结构体
    ESCSDK_PartUsage        m_ePartUsage;                                       //分区用途
}TSCSDK_SetDiskDevUsage;

typedef struct tagSCSDK_NetFSCfg                                                //NetFS的获取，添加，删除结构体
{
    s8                      m_achServerPath[SCSDK_MAX_NAME_LEN+1];              //NetFS服务器路径
    s8                      m_achUserName[SCSDK_MAX_NAME_LEN+1];                //用户名
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //密码
    s8                      m_achOption[SCSDK_MAX_MOUNTOPTION_LEN+1];           //挂载选项,可为空
    ESCSDK_MountType        m_eMountType;                                       //NetFS类型:nfs/cifs ...，可不指定
}TSCSDK_AddNetFSCfg,TSCSDK_DelNetFSCfg,TSCSDK_GetNetFSCfg;

typedef struct tagSCSDK_GetNetFSRunCfg
{
     TSCSDK_GetNetFSCfg      m_tGetNetFSCfg;                                    // NetFS服务器基本信息
     BOOL32                  m_bConStatus;                                      // 连接状态;1:已连接;0:未连接
} TSCSDK_GetNetFSRunCfg;

typedef struct tagSCSDK_GetNetFSExportCond
{
    TSCSDK_IpAddress m_tNetFSIP;                                          // NetFS服务器地址
    u8 m_byPageNum;                                                       // 请求第几页的数据
    u8 m_byNumPerPage;                                                   // 每页显示的个数
}TSCSDK_GetNetFSExportCond;

typedef struct tagSCSDK_NetFSExport                                       //NetFS服务器导出目录的结构体
{
    s8                      m_achPath[SCSDK_MAX_NAME_LEN+1];              // NetFS服务器目录
    s8                      m_achIPList[SCSDK_MAX_NAME_LEN+1];            // NetFS服务器目录对于可访问的客户端
    BOOL32                  m_bIsAdd;                                     // 此目录是否在本客户端已经添加
}TSCSDK_GetNetFSExport;

typedef struct tagSCSDK_GetIscsiCfgHeadInfo
{
    u8                  m_byIpSanNum;   //头后面紧跟着的TSCSDK_GetIscsiCfgResult的个数
    ESCSDK_IscsiState   m_eIscsiState;  //服务端Iscsi服务状态
}TSCSDK_GetIscsiCfgHeadInfo;

typedef struct tagSCSDK_GetIscsiCfgResult
 {
    s8                      m_szUserName[SCSDK_MAX_NAME_LEN+1];                 //用户名
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //密码
    TSCSDK_IpAddress        m_tIP;                                              //IP地址
    u16                     m_wPort;                                            //端口
    ESCSDK_ArrayState       m_eArrayState;                                      //阵列的状态
 }TSCSDK_GetIscsiCfgResult;

typedef struct tagSCSDK_AddIscsiCfg                                             //阵列的添加，删除，连接，断开结构体
 {
    s8                      m_szUserName[SCSDK_MAX_NAME_LEN+1];                 //用户名
    s8                      m_achPassWord[SCSDK_MAX_PASSWORD_LEN+1];            //密码
    TSCSDK_IpAddress        m_tIP;                                              //IP地址
    u16                     m_wPort;                                            //端口
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

typedef struct tagSCSDK_OperResult                                              //操作结果
{
    ESCSDK_ErrType          m_eOperResult;
    s8                      m_szErrDesc[SCSDK_MAX_ERRORDESCRIPTION_LEN+1];
}TSCSDK_OperResult;

typedef struct tagSCSDK_DiskState
{
    TSCSDK_DeviceID         m_tDeviceID;                                    //设备ID
    TSCSDK_DiskDev          m_tDiskDev;                                     //磁盘设备结构体
    ESCSDK_DiskState        m_eDiskState;                                   //磁盘的状态
}TSCSDK_DiskState;

typedef struct tagSCSDK_PartState
{
    TSCSDK_DeviceID         m_tDeviceID;                                    //设备ID
    TSCSDK_DiskDev          m_tDiskDev;                                     //磁盘设备结构体
    ESCSDK_PartState        m_ePartState;                                   //分区状态
}TSCSDK_PartState;

typedef struct tagSCSDK_DiskDevOpState                                              //磁盘设备操作状态
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //设备ID
    TSCSDK_DiskDev              m_tDiskDev;                                         //磁盘设备结构体
    ESCSDK_DiskDevOperation     m_eDiskDevOperation;                                //磁盘设备操作
    u8                          m_byPercent;                                        //百分比
    s8                          m_szPartOpDes[SCSDK_MAX_OPDES_LEN+1];               //操作描述（添加的）
}TSCSDK_DiskDevOpState;


typedef struct tagSCSDK_ArrayInfo                                               //阵列信息
{
    TSCSDK_IpAddress     m_tArrayIP;                                            //阵列IP
    s8                   m_achArrayType[SCSDK_MAX_ARRAY_TYPE_LEN+1];                //阵列类型，2014.10.16废弃使用ESCSDK_ArrayType枚举，改为直传字符串，便于扩展.
    u32                  m_dwSendRate;                                          //网络发送速度(Kb/s)
    u32                  m_dwReceiveRate;                                       //网络接收速度(Kb/s)
    u8                   m_byMemUtil;                                           //内存占用率
    u8                   m_byCpuUtil;                                           //cpu占用率
    ESCSDK_ArrayState    m_eArrayState;                                         //阵列状态
}TSCSDK_ViewArrayInfo;

typedef struct tagSCSDK_RaidManagerInfo
{
    TSCSDK_IpAddress                m_tArrayIP;                                             //阵列IP
    s8                              m_szDiskGroupName[SCSDK_MAX_DISKGROUPNAME_LEN+1];       //磁盘组名
}TSCSDK_RaidManagerInfo;

typedef struct tagSCSDK_DiskGroupInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                     //阵列IP和磁盘组名
    ESCSDK_DiskGroupState           m_eDiskGroupState;                                      //磁盘组状态
    ESCSDK_RaidLevel                m_eRaidLevel;                                           //RAID级别
    u32                             m_dwDiskBitmap;
    u32                             m_dwSpareBitmap;
}TSCSDK_ViewDiskGroupInfo;

typedef struct tagSCSDK_PhysicalDiskInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                             //阵列IP和磁盘组名
    u32                             m_dwDiskSlot;                                                   //磁盘槽位号
    s8                              m_szVendor[SCSDK_MAX_MANUFAC_LEN+1];                            //厂商名
    s8                              m_szModel[SCSDK_MAX_DEVTYPE_LEN+1];                             //型号
    u32                             m_dwDiskSize;                                                   //磁盘容量（M）
    u32                             m_dwDiskState;                                                  //磁盘状态（ESCSDK_DiskStatus）
    ESCSDK_DiskHealth               m_eDiskHealth;                                                  //磁盘健康状态
    u32                             m_dwReallocatedSectors;                                         //重新分配扇区数
    BOOL32                          m_bIsBackup;                                                    //是否热备份
}TSCSDK_ViewPhysicalDiskInfo;

typedef struct tagSCSDK_VirtualDiskInfo
{
    TSCSDK_RaidManagerInfo          m_tRaidManagerInfo;                                 //阵列IP和磁盘组名
    s8                              m_szVirtualDiskName[SCSDK_MAX_VITUALDISKNAME+1];    //虚拟磁盘名
    ESCSDK_VirtualDiskType          m_eVirtualDiskType;                                 //虚拟磁盘类型
    u32                             m_dwVirtualDiskSize;                                //虚拟磁盘容量(M)
    ESCSDK_VirtualDiskState         m_eVirtualDiskState;                                //虚拟磁盘状态
    u32                             m_dwCurStateTime;                                   //当前状态的持续时间 (s)
    u32                             m_dwTargetID;                                       //TargetID
    u32                             m_dwLunID;                                          //LunID
    s8                              m_szLocalDeviceName[SCSDK_MAX_DEVNAME_LEN+1];       //映射设备名
}TSCSDK_ViewVirtualDiskInfo;


//阵列管理部分，ntf所需要的消息
typedef struct tagSCSDK_UpdateArrayInfo
{
    TSCSDK_DeviceID         m_tDeviceID;                                            //设备ID
    TSCSDK_ViewArrayInfo    m_tViewArrayInfo;
    ESCSDK_RMEvent          m_eRMEvent;
}TSCSDK_UpdateArrayInfo;

typedef struct tagSCSDK_UpdateDiskGroupInfo
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //设备ID
    TSCSDK_ViewDiskGroupInfo    m_tViewDiskGroupInfo;
    ESCSDK_RMEvent              m_eRMEvent;
}TSCSDK_UpdateDiskGroupInfo;

typedef struct tagSCSDK_UpdatePhysicalDiskInfo
{
    TSCSDK_DeviceID              m_tDeviceID;                                       //设备ID
    TSCSDK_ViewPhysicalDiskInfo  m_tViewPhysicalDiskInfo;
    ESCSDK_RMEvent               m_eRMEvent;
}TSCSDK_UpdatePhysicalDiskInfo;

typedef struct tagSCSDK_UpdateVirtualDiskInfo
{
    TSCSDK_DeviceID             m_tDeviceID;                                        //设备ID
    TSCSDK_ViewVirtualDiskInfo  m_tViewVirtualDiskInfo;
    ESCSDK_RMEvent              m_eRMEvent;
}TSCSDK_UpdateVirtualDiskInfo;

typedef struct tagSCSDK_GetIscsiInitiatorName
{
    s8                          m_szIscsiInitiatorName[SCSDK_MAX_NAME_LEN+1];
}TSCSDK_GetIscsiInitiatorName;

/************************************************************************/
/*          回调函数定义                                                */
/************************************************************************/
typedef void (*SCSDKEventCB)(TSCSDK_EventHandle tEventHandle, u8* pbyBuf, u32 dwBufLen, void* pvContext);


/************************************************************************/
/*          公共接口定义                                                */
/************************************************************************/


/*====================================================================
函数名      :SCSDK_SetEventCallback
功能        :注册回调函数
             1.由于SDK库的所有接口为异步接口，因此所有接口的结果均通过
               注册的回调函数返回；
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
----------------------------------------------------------------------
修改记录:
日  期      版本        修改人        修改内容
2012/05/21  1.0         张文新        创建
====================================================================*/
ESCSDK_ErrType SCSDK_SetEventCallback(SCSDKEventCB pFunc, void* pvContext);

/*====================================================================
函数名      ：SCSDK_ConnectServ
功能        ：连接某一媒体服务器设备
             1.通过输入媒体服务器设备的ID，设备名，IP，端口号使pmc连接上该媒体服务器设备
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                        -- [IN]  输入设备ID
             TSCSDK_ConnectMediaServ* ptConnectServ              -- [IN]  输入要连接的媒体服务器信息
             TSCSDK_Sequence* ptSequence                         -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ConnectServ (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ConnectServ* ptConnectServ, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_DisconnectServ
功能        ：断开某一媒体服务器设备
             1.通过输入媒体服务器设备的ID，设备名，IP，端口号使pmc断开对该媒体服务器设备的连接
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                           -- [IN]  输入设备ID
             TSCSDK_DisconnectMediaServ* ptDisconnectServ           -- [IN]  输入要断开的媒体服务器设备信息
             TSCSDK_Sequence* ptSequence                            -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_DisconnectServ (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DisconnectServ* ptDisconnectServ, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_GetDiskInfo
功能        ：获取某一设备上磁盘列表信息
             1.通过输入设备的ID，pmc获取该设备上的磁盘列表信息
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
==================================================================== */
ESCSDK_ErrType SCSDK_GetDiskInfo(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_EditPart
功能        ：编辑某个设备上某个磁盘分区
             1.对某个磁盘进行分区操作，且最大分区个数不能超过4
             2.分区之间的分界线通过结构体TSCSDK_PartCfgLine表示
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_EditPart* ptEditPart             -- [IN]  输入磁盘名，分区个数，各分区间的分界线
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_EditPart(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_EditPart* ptEditPart, TSCSDK_Sequence* ptSequence);

 /*====================================================================
函数名      ：SCSDK_ViewLog
功能        ：查看某磁盘或者分区的日志
             1.查看指定的某个磁盘或者分区在某一时间段内的日志，
               通过TSCSDK_ViewLogResult查看日志输出结果
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID         -- [IN]  输入设备ID
             TSCSDK_ViewLog* ptViewLog            -- [IN]  输入磁盘设备结构体，时间段
             TSCSDK_Sequence* ptSequence          -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
  -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ==================================================================== */
ESCSDK_ErrType SCSDK_ViewLog(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ViewLog* ptViewLog,TSCSDK_Sequence* ptSequence);

 /*====================================================================
函数名      :SCSDK_FormatDiskDev
功能        ：对某磁盘设备进行格式化
             1.对某个磁盘设备进行格式化，且格式化的类型有vbfs和ext3两种
             2.通过TSCSDK_DiskDevOpState对格式化进度进行主动上报
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_FormatDiskDev* ptFormatDiskDev       -- [IN]  输入磁盘设备结构体，格式类型
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
  -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ==================================================================== */
 ESCSDK_ErrType SCSDK_FormatDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_FormatDiskDev* ptFormatDiskDev, TSCSDK_Sequence* ptSequence);

 /*====================================================================
函数名      ：SCSDK_MountDiskDev
功能        ：对磁盘设备进行挂载
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  输入设备ID
             TSCSDK_MountDiskDev* ptMountDiskDev            -- [IN]  输入磁盘设备结构体，挂载路径
             TSCSDK_Sequence* ptSequence                    -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_MountDiskDev (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_MountDiskDev* ptMountDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      :SCSDK_UnmountDiskDev
功能        ：对磁盘设备进行卸载
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  输入设备ID
             TSCSDK_UnmountDiskDev* ptUnmountDiskDev        -- [IN]  输入磁盘设备结构体，卸载路径
             TSCSDK_Sequence* ptSequence                    -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
 ESCSDK_ErrType SCSDK_UnmountDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_UnmountDiskDev* ptUnmountDiskDev, TSCSDK_Sequence* ptSequence);

 /*====================================================================
函数名      ：SCSDK_FsckDiskDev
功能        ：对磁盘设备进行修复
              1.通过TSCSDK_DiskDevOpState对修复进度进行主动上报
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_FsckDiskDev* ptFsckDiskDev           -- [IN]  输入磁盘设备结构体，修复路径
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_FsckDiskDev(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_FsckDiskDev* ptFsckDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_SetDiskDevUsage
功能        ：设置磁盘设备的分区用途

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_SetDiskDevUsage* ptSetDiskDevUsage   -- [IN]  输入磁盘设备结构体,及用途
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2017/07/19  1.0         黄至春        创建
====================================================================*/
ESCSDK_ErrType SCSDK_SetDiskDevUsage (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_SetDiskDevUsage* ptSetDiskDevUsage, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_GetNetFSCfg
功能        ：获取已配置的NetFS设备信息
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2017/07/19  1.0         叶宝山        创建
 ====================================================================*/
 ESCSDK_ErrType SCSDK_GetNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      : SCSDK_AddNetFSCfg
功能        : 新增NetFS设备
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  输入设备ID
             TSCSDK_AddNetFSCfg* ptAddNetFSCfg          -- [IN]  输入新增NetFS信息
             TSCSDK_Sequence* ptSequence                -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2017/07/19  1.0         叶宝山        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_AddNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_AddNetFSCfg* ptAddNetFSCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_DelNetFSCfg
功能        ：对某个NetFS设备进行删除
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  输入设备ID
             TSCSDK_DelNetFSCfg* ptDelNetFSCfg          -- [IN]  输入删除NetFS设备信息;只需要m_achServerPath;m_achClientPath 其他可为空
             TSCSDK_Sequence* ptSequence                -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2017/07/19  1.0         叶宝山        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelNetFSCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelNetFSCfg* ptDelNetFSCfg, TSCSDK_Sequence* ptSequence);


/*====================================================================
函数名      ：SCSDK_GetNetFSExportList
功能        ：对某个NFS服务器的可挂载列表
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                       -- [IN]  输入设备ID
             TSCSDK_GetNetFSExportCond* ptGetNetFSExportCond    -- [IN]  NetFS服务器IP以及请求页数、个数
             TSCSDK_Sequence* ptSequence                        -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2017/07/19  1.0         叶宝山        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetNetFSExportList(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_GetNetFSExportCond *ptGetNetFSExportCond, TSCSDK_Sequence* ptSequence);


/*====================================================================
函数名      ：SCSDK_GetIscsiCfg
功能        ：获取某一个设备的阵列信息
              1.输入设备ID，可获取该设备上所拥有的阵列信息（包括在线的和下线的）
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                 -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
 ESCSDK_ErrType SCSDK_GetIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_AddIscsiCfg
功能        ：对某个设备新增某一阵列
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  输入设备ID
             TSCSDK_AddIscsiCfg* ptAddIscsiCfg          -- [IN]  输入新增阵列信息
             TSCSDK_Sequence* ptSequence                -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_AddIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_AddIscsiCfg* ptAddIscsiCfg, TSCSDK_Sequence* ptSequence);

 /*====================================================================
函数名      ：SCSDK_DelIscsiCfg
功能        ：对某个设备上的某一阵列进行删除
              1.如果该阵列正处于连接状态，请先断开连接之后进行删除操作
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID               -- [IN]  输入设备ID
             TSCSDK_DelIscsiCfg* ptDelIscsiCfg          -- [IN]  输入删除阵列信息
             TSCSDK_Sequence* ptSequence                -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelIscsiCfg* ptDelIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_DisconnectIscsiCfg
功能        ：在某个设备上断开某一阵列连接
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                       -- [IN]  输入设备ID
             TSCSDK_DisconnectIscsiCfg* ptDisconnectIscsiCfg    -- [IN]  输入断开阵列信息
             TSCSDK_Sequence* ptSequence                        -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_DisconnectIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DisconnectIscsiCfg* ptDisconnectIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_ConnectIscsiCfg
功能        ：在某个设备上连接某一阵列
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                   -- [IN]  输入设备ID
             TSCSDK_ConnectIscsiCfg* ptConnectIscsiCfg      -- [IN]  输入要连接的阵列信息
             TSCSDK_Sequence* ptSequence                    -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ConnectIscsiCfg (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_ConnectIscsiCfg* ptConnectIscsiCfg, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_StartIscsi
功能        ：启动某一设备上的iscsi服务
              1.要指定启动哪个设备上的iscsi服务，所要要输入设备ID
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                           -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                            -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_StartIscsi(TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_StopIscsi
功能        ：停止某一设备上的iscsi服务
              1.要指定停止哪个设备上的iscsi服务，所要要输入设备ID
算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                           -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_StopIscsi (TSCSDK_DeviceID*  ptDeviceID,TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_ViewArrayInfo
功能        ：查看阵列信息

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_Sequence* ptSequence                           -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewArrayInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_ViewDiskGroupInfo
功能        ：查看磁盘组信息

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_IpAddress* ptArrayIP                           -- [IN]  阵列IP
             TSCSDK_Sequence* ptSequence                           -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewDiskGroupInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_IpAddress* ptArrayIP, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_ViewPhysicalDiskInfo
功能        ：查看物理磁盘信息

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_IpAddress* ptArrayIP                           -- [IN]  阵列IP
             TSCSDK_Sequence* ptSequence                           -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewPhysicalDiskInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_IpAddress* ptArrayIP, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_ViewVirtualDiskInfo
功能        ：查看虚拟磁盘信息

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_RaidManagerInfo* ptRaidManagerInfo             -- [IN]  阵列IP和磁盘组名
             TSCSDK_Sequence* ptSequence                           -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_ViewVirtualDiskInfo (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_RaidManagerInfo* ptRaidManagerInfo, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_DelOfflineDisk
功能        ：删除下线磁盘

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*  ptDeviceID                          -- [IN]  输入设备ID
             TSCSDK_DelDisk*   ptDelDisk                           -- [IN]  磁盘ID和是否删除录像记录标记
             TSCSDK_Sequence*  ptSequence                          -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_DelOfflineDisk (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DelDisk* ptDelDisk, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_CancleDiskOp
功能        ：取消队列中的磁盘操作

算法实现    :
引用全局变量:
输入参数说明:TSCSDK_DeviceID*       ptDeviceID                         -- [IN]  输入设备ID
             TSCSDK_DiskDev*        ptDiskDev                          -- [IN]  分区名
             TSCSDK_Sequence*       ptSequence                         -- [OUT] 操作序列号
返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_CancleDiskOp (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_DiskDev* ptDiskDev, TSCSDK_Sequence* ptSequence);

/*====================================================================
函数名      ：SCSDK_GetEventDescriptionFromEventID
功能        ：消息描述

算法实现    :
引用全局变量:
输入参数说明:u32 dwEventID      -- [IN]  输入事件ID

返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/05/21  1.0         张文新        创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetEventDescriptionFromEventID(u32 dwEventID, s8* szEventDescription, u32 dwLen);

/*====================================================================
函数名      ：SCSDK_GetIscsiInitiatorName
功能        ：获取iscsi的发起者名

算法实现    :
引用全局变量:
输入参数说明:
                TSCSDK_DeviceID*            ptDeviceID                  [IN]  输入设备ID
                TSCSDK_Sequence*        ptSequence                          [OUT] 操作序列号

返回值说明  ：操作成功返回ESCSDK_ERRTYPE_OK,失败返回错误码
 -----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2013/05/21  1.0         刘向东       创建
 ====================================================================*/
ESCSDK_ErrType SCSDK_GetIscsiInitiatorName (TSCSDK_DeviceID*  ptDeviceID, TSCSDK_Sequence* ptSequence);


#endif
                                                            // __SCSDK_H__
