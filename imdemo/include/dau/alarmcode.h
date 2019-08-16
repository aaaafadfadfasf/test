/************************************************************************
模块名      : dau
文件名      : alarmcode.h
相关文件    :
实现功能    : 告警码区间定义
作者        : 陈文龙
版权        : <Copyright(C) 2016-2026 Suzhou Keda Technology Co., Ltd. All rights reserved.>
----------------------------------------------------------------------
修改记录：
日 期		版本        修改人      走读人          修改记录
2016/07/20  V1.0        陈文龙                      创建
************************************************************************/
#ifndef _DAU_ALARMCODE_H_
#define _DAU_ALARMCODE_H_

#define INVALID_ALARM_CODE      0
#define ALARMCODE_BASE          10000

/************************************************************************/
/* 各类告警码区间定义                                                   */
/************************************************************************/
//common告警码区间     (10001 - 20000)
#define COMMON_ALARM_BEGIN      (ALARMCODE_BASE+1)
#define COMMON_ALARM_END        (COMMON_ALARM_BEGIN+9999)

//平台告警码区间       (20001 - 30000)
#define PLAT_ALARM_BEGIN        (COMMON_ALARM_END+1)
#define PLAT_ALARM_END          (PLAT_ALARM_BEGIN+9999)

//前端告警码区间       (30001 - 40000)
#define PU_ALARM_BEGIN          (PLAT_ALARM_END+1)
#define PU_ALARM_END            (PU_ALARM_BEGIN+9999)

//存储告警码区间       (40001 - 50000)
#define STORAGE_ALARM_BEGIN     (PU_ALARM_END+1)
#define STORAGE_ALARM_END       (STORAGE_ALARM_BEGIN+9999)

//后续其他告警码区间定义

// 告警状态
enum EMAlarmStatus
{
	ALARM_RESTORE = 0,	// 告警恢复
	ALARM_OCCUR,		// 告警产生
};

/************************************************************************/
/* common告警码定义                                                     */
/************************************************************************/
enum EMCommonAlarmCode
{
    ALARM_COMMON_CPU_STATUS = COMMON_ALARM_BEGIN,               // CPU使用率高于指定阀值告警
    ALARM_COMMON_MEMORY_STATUS,                                 // 内存使用超过指定阀值告警
    ALARM_COMMON_DEV_ONLINE,                                    // 设备上线
    ALARM_COMMON_DEV_OFFLINE,                                   // 设备下线
    ALARM_COMMON_POWER_STATUS,                                  // 电源电压低于指定阀值告警
    ALARM_COMMON_FAN_STATUS,                                    // 风扇转速过低告警
    ALARM_COMMON_ETHCARD_RESTORE,                               // 以太网恢复告警
    ALARM_COMMON_LED_STATUS,                                    // 面板灯快闪、慢闪等状态发生改变
    ALARM_COMMON_SAME_REG_ID,                                   // 其他设备的ID号与本设备相同，同时向NMS注册
    ALARM_COMMON_DISKDETECT_ERROR,                              // 设备检测无磁盘或没有磁盘可工作告警
    ALARM_COMMON_NODISK,                                        // 无硬盘
    ALARM_COMMON_DISK_ERROR,                                    // 硬盘分区损坏
    ALARM_COMMON_DISK_NOFORMAT,                                 // 硬盘分区未格式化
    ALARM_COMMON_DISK_PART_R_ONLY,                              // 磁盘分区只读
    ALARM_COMMON_DISK_FS_ERROR,                                 // 磁盘文件系统错误
    ALARM_COMMON_DISK_OFFLINE,                                  // 磁盘下线
    ALARM_COMMON_STORAGE_ABNORMAL,                              // 磁盘有异常
	ALARM_COMMON_FILEBEAT_ERROR,                             //filebeat异常

    ALARM_COMMON_DISK_STATUS,                                   // 系统盘使用超过指定阀值告警
    ALARM_COMMON_NETADAPTER_DROP_STATUS,                        // 网卡丢包告警
    ALARM_COMMON_NETADAPTER_BANDWIDTH_STATUS,                   // 网卡带宽使用超过指定阀值告警

    ALARM_COMMON_MAX_ALARM_CODE
};


/************************************************************************/
/* 平台告警码                                                           */
/************************************************************************/
/*包含子类型
    1、common
    2、3as
    3、cmu
    4、pui
    5、cui
    6、nru
    7、vtdu
    8、uas
    9、gas
    10、mtu
    11、dns
*/
// 平台common告警码区间    (20001 - 20200)
#define PLAT_COMMON_ALARM_BEGIN         (PLAT_ALARM_BEGIN ) 
#define PLAT_COMMON_ALARM_END           (PLAT_COMMON_ALARM_BEGIN + 199)

// 3as告警码区间           (20201 - 20400)
#define TAS_ALARM_BEGIN                 (PLAT_COMMON_ALARM_END + 1 ) 
#define TAS_ALARM_END                   (TAS_ALARM_BEGIN + 199)

// CMU告警码区间           (20401 - 20600)
#define CMU_ALARM_BEGIN                 (TAS_ALARM_END + 1) 
#define CMU_ALARM_END                   (CMU_ALARM_BEGIN + 199)

// PUI告警码区间           (20601 - 20800)
#define PUI_ALARM_BEGIN                 (CMU_ALARM_END + 1) 
#define PUI_ALARM_END                   (PUI_ALARM_BEGIN + 199)

// CUI告警码区间           (20801 - 21000)
#define CUI_ALARM_BEGIN                 (PUI_ALARM_END + 1) 
#define CUI_ALARM_END                   (CUI_ALARM_BEGIN + 199)

// NRU告警码区间           (21001 - 21200)
#define NRU_ALARM_BEGIN                 (CUI_ALARM_END + 1) 
#define NRU_ALARM_END                   (NRU_ALARM_BEGIN + 199)

// VTDU告警码区间          (21201 - 21400)
#define VTDU_ALARM_BEGIN                (NRU_ALARM_END + 1) 
#define VTDU_ALARM_END                  (VTDU_ALARM_BEGIN + 199)

// UAS告警码区间           (21401 - 21600)
#define UAS_ALARM_BEGIN                 (VTDU_ALARM_END + 1) 
#define UAS_ALARM_END                   (UAS_ALARM_BEGIN + 199)

// GAS告警码区间           (21601 - 21800)
#define GAS_ALARM_BEGIN                 (UAS_ALARM_END + 1) 
#define GAS_ALARM_END                   (GAS_ALARM_BEGIN + 199)

// MTU告警码区间           (21801 - 22000)
#define MTU_ALARM_BEGIN                 (GAS_ALARM_END + 1) 
#define MTU_ALARM_END                   (MTU_ALARM_BEGIN + 199)

// DNS告警码区间           (22001 - 22200)
#define DNS_ALARM_BEGIN                 (MTU_ALARM_END + 1) 
#define DNS_ALARM_END                   (DNS_ALARM_BEGIN + 199)

//////////////////////////////////////////////////////////////////////////
//以下定义平台类相关告警
//平台common告警
enum EMPlatCommonAlarmCode
{
    ALARM_PLAT_COMMON_DISCONNECT_CMU = PLAT_COMMON_ALARM_BEGIN,     // CUI未连接上CMU
    ALARM_PLAT_COMMON_PLATFORM_IN_REPLACE_MODE,                     // 平台运行在备机替换模式
    ALARM_PLAT_COMMON_DETECT_NULL_IP_NIC,                           // 探测到无IP的网卡
    ALARM_PLAT_COMMON_PLATFORM_DISK_NUM_OVERLOAD,                   // 平台的数据盘分区总数超过录像机支持的最大个数
    ALARM_PLAT_COMMON_DETECT_USELESS_NIC,                           // 网段配置中有无效网卡
    ALARM_PLAT_COMMON_BACKUP_USB_DISCONNECT,                        // 备份盘未连接
    ALARM_PLAT_COMMON_BACKUP_PART_CONFLICT,                         // 存在多个备份分区，请手动选择挂载。
    ALARM_PLAT_COMMON_DATABASE_PART_CONFLICT,                       // 存在多个数据库分区，请手动选择挂载。
    ALARM_PLAT_COMMON_NONE_MOUNTED_DATABASE_PART,                   // 没有挂载数据库分区
    ALARM_PLAT_COMMON_PLATFORM_DISK_SPACE_OVERLOAD,                 // 平台的数据盘分区总大小超过支持的最大值
    ALARM_PLAT_COMMON_PMS_DISKFULL_STATUS,                          // pms自检产生的磁盘满告警
    ALARM_PLAT_COMMON_REG_CMU,                                      // PU或其他模块注册CMU失败告警
    ALARM_PLAT_CONMON_CODE_MAP_STATUS,                              // MAP死、异常告警
    //ALARM_PLAT_COMMON_DISCONNECT_DATABASE,                          // 未连接上数据库

	ALARM_PLAT_COMMON_PUI_DISCONNECT_CMU,							// PUI未连接上CMU
	ALARM_PLAT_COMMON_VTDU_DISCONNECT_CMU,							// VTDU未连接上CMU
	ALARM_PLAT_COMMON_NRU_DISCONNECT_CMU,							// NRU未连接上CMU
	ALARM_PLAT_COMMON_PMS_DISCONNECT_CMU,							// PMS未连接上CMU

	ALARM_PLAT_COMMON_3AS_OFFLINE,									// 3AS下线
	ALARM_PLAT_COMMON_UAS_OFFLINE,									// UAS下线
	ALARM_PLAT_COMMON_CMU_OFFLINE,									// CMU下线
	ALARM_PLAT_COMMON_CUI_OFFLINE,									// CUI下线
	ALARM_PLAT_COMMON_PUI_OFFLINE,									// PUI下线
	ALARM_PLAT_COMMON_VTDU_OFFLINE,									// VTDU下线
	ALARM_PLAT_COMMON_NRU_OFFLINE,									// NRU下线
	ALARM_PLAT_COMMON_TVS_OFFLINE,									// TVS下线
	ALARM_PLAT_COMMON_MPS_OFFLINE,									// MPS下线
	ALARM_PLAT_COMMON_RCS_OFFLINE,									// RCS下线
	ALARM_PLAT_COMMON_ALS_OFFLINE,									// ALS下线
    ALARM_PLAT_COMMON_PLATFORM_OFFLINE,                             // PLATFORM下线

    ALARM_PLAT_COMMON_WRITE_FAIL,                                   // 写入失败
    ALARM_PLAT_COMMON_DISK_OFFLINE,                                 // 磁盘掉线
    ALARM_PLAT_COMMON_STREAM_FULL_LOST,                             // 接收码流大于存储码流
    ALARM_PLAT_COMMON_STREAM_IN_LOST,                               // 接收码流丢包
    ALARM_PLAT_COMMON_FILE_BE_LEFT,                                 // 目录(文件)残留
    
    ALARM_PLAT_COMMON_MAX_ALARM_CODE
};

//3as告警码
enum EMPlatTasAlarmCode
{    
    ALARM_TAS_DISCONNECT_USBKEY = TAS_ALARM_BEGIN,              // 未连接上USBKEY
    ALARM_TAS_AT_TERM_REMIND_USBKEY,                            // USB到期提醒
    ALARM_TAS_EXPIRED_USBKEY,                                   // 许可证已过期
    ALARM_TAS_LICENSE_PU_NUM_OVERFLOW,                          // 前端超license限制
    ALARM_TAS_LICENSE_OTHER_MANUFACTURER_PU_NUM_OVERFLOW,       // 外厂商前端超license限制
	ALARM_TAS_DISCONNECT_DATABASE,							  // 3as未连接上数据库

    ALARM_TAS_MAX_ALARM_CODE
};

//cmu告警码
enum EMPlatCmuAlarmCode
{
    ALARM_CMU_DISCONNECT_GAS = CMU_ALARM_BEGIN,                 // 未连接上GAS
    ALARM_CMU_DISCONNECT_3AS,                                   // 未连接上3AS
    ALARM_CMU_DISCONNECT_UAS,                                   // 未连接上UAS
    ALARM_CMU_DISCONNECT_PUI,                                   // 未连接上PUI
    ALARM_CMU_DISCONNECT_CUI,                                   // 未连接上CUI
    ALARM_CMU_DISCONNECT_NRU,                                   // 未连接上NRU
    ALARM_CMU_DISCONNECT_VTDU,                                  // 未连接上VTDU
    ALARM_CMU_DISCONNECT_SUPER_CMU,                             // 未连接上上级CMU
    ALARM_CMU_DISCONNECT_SUB_CMU,                               // 下级CMU断开连接
    ALARM_CMU_DISCONNECT_BCMU_CMU,                              // 未连接上平级CMU
    ALARM_CMU_NRU_NET_SEG_DIFFERENT_FROM_CMU,                   // Nru与Cmu 网段不一致
    ALARM_CMU_VTDU_NET_SEG_DIFFERENT_FROM_CMU,                  // vtdu 与cmu网段不一致
    ALARM_CMU_VIDEONUMBER_OVERLOAD,                             // 视频源数超过阀值

    ALARM_CMU_MAX_ALARM_CODE
};

//PUI告警码
enum EMPlatPuiAlarmCode
{
    ALARM_PUI_PU_OVERLOAD = PUI_ALARM_BEGIN,                    // 登录PU数超过阀值
    ALARM_PUI_STOPREC_LOGERR,                                   // 前端停录 - 日志系统坏停录
    ALARM_PUI_STOPREC_DISKERR,                                  // 前端停录 - 硬盘坏停录
    ALARM_PUI_MOTIONDETECT_STATUS,                              // 移动侦测告警
    ALARM_PUI_LPT_STATUS,                                       // 并口告警上报
    ALARM_PUI_LOSEVIDEOSRC_STATUS,                              // 前端设备视频源丢失告警

    ALARM_PUI_MAX_ALARM_CODE
};

//CUI告警码
enum EMPlatCuiAlarmCode
{
    ALARM_CUI_CU_OVERLOAD = CUI_ALARM_BEGIN,                    // 登录CU数超过阀值
    ALARM_CUI_CMU_CU_ALARM_LOSEPACKET,                          // CUI丢包告警
    ALARM_CUI_CMU_CU_ALARM_REQUISTION,                          // CUI请求异常告警

    ALARM_CUI_MAX_ALARM_CODE
};

//NRU告警码
enum EMPlatNruAlarmCode
{
    ALARM_NRU_DISKFULL_STATUS = NRU_ALARM_BEGIN,                // 磁盘满告警
    ALARM_NRU_RAID_DISK_OFFLINE,                                // 阵列硬盘损坏或空缺
    ALARM_NRU_RAID_DISK_HEALTH_BAD,                             // 阵列硬盘健康状态异常
    ALARM_NRU_RAID_DISKGROUP_STATUS_ABNORMAL,                   // 阵列磁盘组状态不正常
    ALARM_NRU_RAID_ISCSI_DROPPED,                               // 阵列ISCSI断链
    ALARM_NRU_RAID_ABNORMAL,                                    // 阵列异常
    ALARM_NRU_PLATFORM_VBFS_DISK_SPACE_OVERLOAD,                // vbfs超限

    ALARM_NRU_MAX_ALARM_CODE
};

//VTDU告警码
enum EMPlatVtduAlarmCode
{
    ALARM_VTDU_RECV_NOBITSTREAM = VTDU_ALARM_BEGIN,             // 接收无码流
    ALARM_VTDU_LOSEPACKET,                                      // 码流接收丢包超过指定阀值告警
    ALARM_VTDU_OVERLOAD,                                        // VTDU转发路数超过阀值

    ALARM_VTDU_MAX_ALARM_CODE
};

//uas告警码
enum EMPlatUasAlarmCode
{
    ALARM_UAS_DISCONNECT_DATABASE = UAS_ALARM_BEGIN,            // UAS未连接上数据库

    ALARM_UAS_MAX_ALARM_CODE = UAS_ALARM_BEGIN,
};

//以上是平台类相关告警
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* 前端告警码                                                           */
/************************************************************************/
/*包含子类型
    1、IPC
    2、NVR
    3、SVR
    4、Decoder
    5、Encoder
*/
// 前端common告警码区间    (30001 - 30200)
#define PU_COMMON_ALARM_BEGIN           (PU_ALARM_BEGIN ) 
#define PU_COMMON_ALARM_END             (PU_COMMON_ALARM_BEGIN + 199)

// IPC告警码区间           (30201 - 30400)
#define IPC_ALARM_BEGIN                 (PU_COMMON_ALARM_END +1) 
#define IPC_ALARM_END                   (IPC_ALARM_BEGIN + 199)

// NVR告警码区间           (30401 - 30600)
#define NVR_ALARM_BEGIN                 (IPC_ALARM_END + 1) 
#define NVR_ALARM_END                   (NVR_ALARM_BEGIN + 199)

// SVR告警码区间           (30601 - 30800)
#define SVR_ALARM_BEGIN                 (NVR_ALARM_END + 1) 
#define SVR_ALARM_END                   (SVR_ALARM_BEGIN + 199)

// Decoder告警码区间       (30801 - 31000)
#define DEC_ALARM_BEGIN                 (SVR_ALARM_END + 1) 
#define DEC_ALARM_END                   (DEC_ALARM_BEGIN + 199)

// Encoder告警码区间       (31001 - 31200)
#define ENC_ALARM_BEGIN                 (DEC_ALARM_END + 1) 
#define ENC_ALARM_END                   (ENC_ALARM_BEGIN + 199)

// 云存储告警码区间		   (31201 - 31400)
#define CLOUD_ALARM_BEGIN                 (ENC_ALARM_END + 1) 
#define CLOUD_ALARM_END                   (CLOUD_ALARM_BEGIN + 199)

// 通用服务器告警码区间        (31401 - 32400)
#define VAS_ALARM_BEGIN                    (CLOUD_ALARM_END + 1) 
#define VAS_ALARM_END                      (VAS_ALARM_BEGIN + 999)

//////////////////////////////////////////////////////////////////////////
///以下定义通用服务器相关告警
enum VAS_ALARM_CODE
{
	ALARM_TASK_OVERRUN = VAS_ALARM_BEGIN, //1000 任务已达最大数量 （注意：这里的注释中的值为vas中对应值，后面同理）
    ALAEM_GLOBAL_TASK_OVERRUN, // 1001 全局任务已达最大数量
    ALARM_GET_LICENSE_FAILED, //1002 请求许可证失败！
    ALARM_LICENSE_INVALIDATION, //1003 许可证无效，请及时联系管理员处理！
    ALARM_LICENSE_EXPIRY,//1004 许可证已过期，请及时联系管理员处理！
    ALARM_VIDEO_SOURCE_OVERRUN,//1005 视频源数目超限，请更新许可证！
    ALARM_INSIDE_NET_VIDEO_SOURCE_OVERRUN,//1006 已入网视频源数目达到许可证最大视频源数目，建议扩容！
    ALARM_INSIDE_NET_VIDEO_SOURCE_WILL_OVERRUN,//1007 已入网视频源数目即将超过许可证最大视频源数目，建议扩容！

    ALARM_NO_TOP_PLATFORM,//1021 找不到顶级平台
    ALARM_TOP_PLATFORM_OFFLINE,//1022 顶级平台不在线
    ALARM_DEVICE_TREE_NOT_EXIST,//1023 设备树不存在！
    ALARM_DEVICE_SOURCE_NOT_READY,//1024 设备树资源未就绪！
    ALARM_CONNECT_MYSQL_FAILED,//1025 数据库连接失败！
    ALARM_CONNECT_REDIS_FAILED,//1026 redis连接失败！
    ALARM_CONNECT_MYSQL_AND_REDIS_FAILED,//1027 数据库、redis连接失败！
    ALARM_USER_NOT_EXIST,//1028 登录用户名不存在，用户认证失败！
    ALARM_LOGIN_PASSWORD_ERROR,//1029 登录密码错误，用户认证失败！
    ALARM_USER_EXPIRY,//1030 登录用户使用期限已到，用户认证失败！
    ALARM_LOGIN_IP_NOT_AUTHORITY,//1031 登录用户的登录地址受限，用户认证失败！
    ALARM_LOGIN_REPEAT,//1032 同一用户帐号已登录，用户认证失败！
    ALARM_USER_BLOCK_UP,//1033 登录用户已被停用，用户认证失败！
    ALARM_PLATFORM_DISCONNECT,//1034 平台断链！

    ALARM_IAU_CONNECT_MYSQL,//1051 分析模块连接数据库！
    ALARM_IAU_CONNECT_ES,//1052 分析模块连接ES！
    ALARM_IAU_CONNECT_REDIS,//1053 分析模块连接redis！
	AlARM_IAU_OFFLINE,//1054 分析模块不在线
	ALAEM_IAU_DISK_FULL,//1061 分析模块磁盘满！
	ALAEM_IAU_FUNCTION_NOT_ENOUGH,//1062 分析模块性能不足！
	ALAEM_IAU_IAU_EXPAND_UNMOUNT,//1063 分析模块扩展盘未挂载
};
///以上是通用服务器相关告警
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//以下定义前端类相关告警
//pu common告警
enum EMPuCommonAlarmCode
{

    ALARM_PU_COMMON_MAX_ALARM_CODE
};

enum EMPuIpcAlarmCode
{
    ALARM_IPC_FUZZY = IPC_ALARM_BEGIN,                      // IPC视频模糊告警
    ALARM_IPC_BRIGHT,                                       // IPC视频过亮告警
    ALARM_IPC_DARK,                                         // IPC视频过暗告警
    ALARM_IPC_SHADE,                                        // IPC图像遮挡告警
    ALARM_IPC_PARTIAL,                                      // IPC图像偏色告警
    ALARM_IPC_MOVING,                                       // IPC移动告警
    ALARM_IPC_PIN,                                          // IPC并口告警
    ALARM_IPC_DEFOCUS,                                      // IPC虚焦检测告警
    ALARM_IPC_CORDON,                                       // IPC警戒线告警
    ALARM_IPC_SECENECHANGE,                                 // IPC场景变换告警
    ALARM_IPC_REGIONINVASION,                               // IPC区域进入告警
    ALARM_IPC_REGIONLEAVING,                                // IPC区域离开告警
    ALARM_IPC_AUDIOABNORMAL,                                // IPC声音异常告警
    ALARM_IPC_OBJECTTAKEN,                                  // IPC物品拿取告警
    ALARM_IPC_OBJECTLEFT,                                   // IPC物品遗留告警
    ALARM_IPC_PEOPLEGATHER,                                 // IPC人员聚集告警
    ALARM_IPC_REGIONENTER,                                  // IPC区域入侵告警

    ALARM_IPC_MAX_ALARM_CODE
};

enum EMPuSvrAlarmCode
{
    //通道无关
    ALARM_SVR_BATTERYLOW= SVR_ALARM_BEGIN,                  //电池电量低
    ALARM_SVR_IPCONFLICT,                                   //ip地址冲突
    ALARM_SVR_NETBROKEN,                                    //网络断开
    ALARM_SVR_NETSPEEDDROP,                                 //网速下降
    ALARM_SVR_SYSTIMEERR,                                   //系统时间错误
    ALARM_SVR_BURNERR,                                      //刻录出错
    ALARM_SVR_DISKERR,                                      //磁盘出错
    ALARM_SVR_NODISK,                                       //无硬盘
    ALARM_SVR_MP4DISKSPACEFULL,                             //MP4分区空间将满
    ALARM_SVR_MP4FREESPACELACK,                             //MP4分区剩余空间不足
    ALARM_SVR_MULIMEDIAOFFLINE,                             //多媒体主机断链
    ALARM_SVR_DISKSPACELACK,                                //光盘剩余空间不足
    ALARM_SVR_IRLEARNSUCCESS,                               //红外学习成功
    ALARM_SVR_NOAUDIO,                                      //无音频接入(合成音频)
    ALARM_SVR_ONTRAIL,                                      //审讯控制信号

    //通道相关
    ALARM_SVR_IPCOFFLINE,                                   //前端掉线
    ALARM_SVR_VIDEOLOST,                                    //前端视频源丢失
    ALARM_SVR_PININPUT,                                     //并口输入
    ALARM_SVR_IPCSENDCAPLACK,                               //主动IPC发送能力不足
    ALARM_SVR_MOVDET,                                       //移动侦测
    ALARM_SVR_NOSTREAM,                                     //合成通道无码流
    ALARM_SVR_DEFOCUS,                                      //失焦
    ALARM_SVR_LUMINANCECHANGE,                              //亮度变化
    ALARM_SVR_SCENECHANGE,                                  //场景变换
    ALARM_SVR_PERSONELGATHER,                               //人员聚集

    ALARM_SVR_MAX_ALARM_CODE
};

//以上是前端类相关告警
//////////////////////////////////////////////////////////////////////////


/************************************************************************/
/* 存储告警码                                                           */
/************************************************************************/
/*包含子类型
    1、RAID
    2、CLOUD

*/
// 存储common告警码区间    (40001 - 40200)
#define STORAGE_COMMON_ALARM_BEGIN          (STORAGE_ALARM_BEGIN ) 
#define STORAGE_COMMON_ALARM_END            (STORAGE_COMMON_ALARM_BEGIN + 199)

// 磁阵告警码区间          (40201 - 40400)
#define RAID_ALARM_BEGIN                    (STORAGE_COMMON_ALARM_END + 1) 
#define RAID_ALARM_END                      (RAID_ALARM_BEGIN + 199)

//////////////////////////////////////////////////////////////////////////
//以下定义存储类相关告警

enum RAID_ALARM_CODE
{
	ALARM_RAID_POWER1_ABNORMAL = RAID_ALARM_BEGIN,  //电源1异常
	ALARM_RAID_POWER2_ABNORMAL,   			        //电源2异常
    ALARM_RAID_MODULE_OFFLINE,                      //磁阵模块下线
};

//以上是存储类相关告警
//////////////////////////////////////////////////////////////////////////

// 以下是云存储相关告警
enum CLOUD_ALARM_CODE
{
	ALARM_SYS_RESOURCE = CLOUD_ALARM_BEGIN,  // 系统资源使用过度
	ALARM_BBU_BATERRY,	// BBU电池告警
	ALARM_CLUSTER_SPACE,	// 集群剩余空间不足告警
	ALARM_META_SERVICE,	// Meta服务异常告警
	ALARM_CHUNK_SERVICE, // Chunk服务异常告警
	ALARM_NET_BANDWIDTH, // 网络带宽异常告警
	ALARM_DISK_HEALTH, // 磁盘健康状态异常告警
	ALARM_DISK_TEMPERATURE,	// 磁盘温度过高告警
	ALARM_DISK_BAD_SECTOR,	//	磁盘坏扇区告警
	ALARM_DISK_OUT_CLUSTER,		// 磁盘未在集群中告警
	ALARM_CHUNK_WRITE,	// chunk服务写磁盘告警
	ALARM_CHUNK_READ,	// CHUNK服务读磁盘告警
	ALARM_META_READ,	// Meta服务写磁盘告警
	ALARM_META_WRITE,	// Meta服务读磁盘告警
	ALARM_SYS_SPACE,	// 系统空间不足告警
	ALARM_NRU_SERVICE,	// NRU服务异常告警
	ALARM_COREDUMP,	// 服务存在coredump告警
	ALARM_BBUCHECK,	// bbucheck服务异常告警
	ALARM_BBUBURNED,	// BBUBRUNED文件异常告警
	ALARM_RSYNC,	//Rsync服务异常告警
	ALARM_NET_LINKDOWN,	// 网络链接断开告警
	ALARM_PING_META,	// Meta节点无法ping通告警
	ALARM_REST_SERVICE,	// S3 REST 服务异常告警
	ALARM_FTP_SERVICE,	// ftp服务异常告警
	ALARM_NFS_SERVICE,	// NFS服务异常告警
	ALARM_BBU_LEVEL_LOW,	// BBU电量不足告警
	ALARM_BBU_LEVEL_HIGH,	// BBU电量严重不足告警
	ALARM_ZOOKEEPER_SERVICE, // ZK服务异常告警
	ALARM_KAFKA_SERVICE,	//Kafka服务异常告警
	ALARM_SAMBA_SERVICE,	// Samba服务异常告警
	ALARM_NTP_SERVICE,	// NTP服务异常告警	
    ALARM_META_DISK_SPACE_LOW,      //meta元数据磁盘空间使用率超过50%
    ALARM_META_DISK_SPACE_HIGH,     //meta元数据磁盘空间使用率超过80%
    ALARM_CHUNK_DISK_SPACE_LOW,     //chunk元数据磁盘空间使用率超过50%
    ALARM_CHUNK_DISK_SPACE_HIGH,    //chunk元数据磁盘空间使用率超过80%
    ALARM_KCSPROXY_SERVER,          //kcsproxy服务异常告警
    ALARM_DISK_OFFLINE,             //磁盘不在线告警
    ALARM_QUOTA_SPACE_USAGE,        //卷中配额空间使用率超过90%
};

#endif //_DAU_ALARMCODE_H_
