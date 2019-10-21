/*
功能：定义监控平台2.0时期的业务消息
*/
#ifndef _SNMP_MSG_H_
#define _SNMP_MSG_H_
#include "snmpadp.h"
#include <string>
using std::string;

/////////////////////////
#define TNM_MODULE_NAME_LEN         (64)      // 设备名称长度
#define TNM_URI_LEN                 (64)      // 模块URI
#define TNM_MODULE_ADDR_LEN         (20)      // 模块地址
#define TNM_MODULE_RESERVED_LEN     (4)       // 模块信息保留字段长度
#define TNM_DEVALARM_RESERVED_LEN   (7)       // 设置告警结构体保留字段
#define TNM_UUID_LEN                (36)      //UUID长度
#define TNM_DISK_NAME_LEN           (36)      //磁盘名长度
#define TNM_MODULE_ID_LEN           (128)     //模块ID长度
#define TNM_MODULE_IP_LEN           (128)     //模块IP长度
#define TNM_DEV_NAME_LEN            (128)     //设备名长度
#define TNM_CHNID_LEN               (8)       //CHnId长度
#define TNM_DEV_ADDR_LEN            (128)     //设备地址长度
#define TNM_PARAM_LEN               (128)     //告警填充参数长度

#define KDMCONF_BGN2 (KDMCONF_BGN + 400)

/* 2.0中使用到的 dwMsgType 老的列表如下（消息体参考snmpadp.h中的) */
// TRAP_TNM_DEV_ALARM  2.0中新的告警码（它的告警基类为TTnmDevAlarmBase）起始号码为20000，当前可用号码：20031
//// TAS用到的告警码：TNM_ALARM_CODE_DISCONNECT_USBKEY，TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY，TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS
//// RCS用到的告警码：TNM_ALARM_CODE_NRU_DISK_ABNORMAL, TNM_ALARM_CODE_NRU_RECORD_TOO_MUCH
//// CMU用到的告警码: TNM_ALARM_CODE_CU_TOO_MUCH, TNM_ALARM_CODE_PU_TOO_MUCH, TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH, TNM_ALARM_CODE_MODULE_DISCONNECT

/* PMS用到的告警码 TNM_ALARM_CODE_CPU_STATUS TNM_ALARM_CODE_MEMORY_STATUS TNM_ALARM_CODE_DISKFULL_STATUS TNM_ALARM_CODE_PMS_USBBAK_DISK_FULL TNM_ALARM_CODE_DISCONNECT_DATABASE
TNM_ALARM_CODE_MIRROR_EXPORT_FAIL TNM_ALARM_CODE_MIRROR_IMPORT_FAIL TNM_ALARM_CODE_SYSDISK_READONLY TNM_ALARM_CODE_USBBAK_DISK_DAMAGED      */

/* 2.0中使用到的 dwMsgType 新创建的列表如下 */
#define NODE_CMU_PFMINFO2 (KDMCONF_BGN2+1)      // 告警内容: TTnmCmuPfmInfo2
#define NODE_CMU_ONLINE_MODULE (KDMCONF_BGN2+2) // 告警内容：u32 模块个数 ＋ TTnmModuleInfo数组
#define NODE_RCS_PFMINFO (KDMCONF_BGN2+3)       // 告警内容: TTnmRcsPfmInfo

/* 2.0中新的告警码列表如下 */
#define TNM_ALARM_CODE_NRU_DISK_ABNORMAL         20000  // 告警内容: TnmAlarmCodeNruDiskAbnormal
#define TNM_ALARM_CODE_NRU_RECORD_TOO_MUCH       20004  // 告警内容：TnmAlarmCodeNruRecordTooMuch

#define TNM_ALARM_CODE_CU_TOO_MUCH               20001  // 告警内容：TTnmDevAlarmBase
#define TNM_ALARM_CODE_PU_TOO_MUCH               20002  // 告警内容：TTnmDevAlarmBase
#define TNM_ALARM_CODE_VTDU_SWITCH_TOO_MUCH      20003  // 告警内容：TTnmDevAlarmBase
#define TNM_ALARM_CODE_MODULE_DISCONNECT         20005  // 告警内容: TnmAlarmCodeModuleDisconnect

//   add by zgh
#define TNM_ALARM_CODE_TAS_DISCONNECT_USBKEY         TNM_ALARM_CODE_DISCONNECT_USBKEY           //未连接上USBKEY
#define TNM_ALARM_CODE_TAS_AT_TERM_REMIND_USBKEY     TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY       //USBKEY到期提醒
#define TNM_ALARM_CODE_TAS_LICENSE_VALIDATE_NOT_PASS TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS   //校验不通过
// #define TNM_ALARM_CODE_PMS_CPU_STATUS                TNM_ALARM_CODE_CPU_STATUS          //CPU使用率高于指定阀值告警
// #define TNM_ALARM_CODE_PMS_MEMORY_STATUS             TNM_ALARM_CODE_MEMORY_STATUS       //内存使用超过指定阀值告警
// #define TNM_ALARM_CODE_PMS_DISCONNECT_DATABASE       TNM_ALARM_CODE_DISCONNECT_DATABASE //数据库连接失败(损坏)
// #define TNM_ALARM_CODE_VTDU_LOSEPACKET               TNM_ALARM_CODE_LOSEPACKET          //丢包
// #define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         TNM_ALARM_CODE_RECV_NOBITSTREAM    //交换无码流
// #define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           TNM_ALARM_CODE_ETHCARD_RESTORE     //网络千兆百兆不符
// #define TNM_ALARM_CODE_RCS_DISK_OFFLINE              TNM_ALARM_CODE_DISK_OFFLINE        //磁阵下线
// #define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          TNM_ALARM_CODE_DISK_PART_R_ONLY    //磁盘分区只读

#define TNM_ALARM_CODE_PMS_USBBAK_DISK_NOT_EXIST     20006  // 告警内容：TTnmDevAlarmBase  备份盘丢失
#define TNM_ALARM_CODE_PMS_USBBAK_DISK_FULL          20007  // 告警内容：TTnmDevAlarmBase  备份盘满
#define TNM_ALARM_CODE_PMS_OPT_DISKFULL_STATUS       20008  // 告警内容：TTnmDevAlarmBase  业务盘满
#define TNM_ALARM_CODE_PMS_DB_DISKFULL_STATUS        20009  // 告警内容：TTnmDevAlarmBase  数据盘满
#define TNM_ALARM_CODE_PMS_LOG_DISKFULL_STATUS       20010  // 告警内容：TTnmDevAlarmBase  日志盘满
#define TNM_ALARM_CODE_PMS_OPT_DISK_PART_R_ONLY      20011  // 告警内容：TTnmDevAlarmBase  业务盘变为只读
#define TNM_ALARM_CODE_PMS_DB_DISK_PART_R_ONLY       20012  // 告警内容：TTnmDevAlarmBase  数据盘变为只读
#define TNM_ALARM_CODE_PMS_LOG_DISK_PART_R_ONLY      20013  // 告警内容：TTnmDevAlarmBase  日志盘变为只读
#define TNM_ALARM_CODE_PMS_USBKEY_DISK_PART_R_ONLY   20014  // 告警内容：TTnmDevAlarmBase  备份盘变为只读
#define TNM_ALARM_CODE_PMS_MIRROR_EXPORT_FAIL        20015  // 告警内容：TTnmDevAlarmBase  定时导出镜像失败
#define TNM_ALARM_CODE_PMS_MIRROR_IMPORT_FAIL        20016  // 告警内容：TTnmDevAlarmBase  镜像导入异常
#define TNM_ALARM_CODE_PMS_USBBAK_DISK_DAMAGED       20017  // 告警内容：TTnmDevAlarmBase  备份盘损坏
#define TNM_ALARM_CODE_PMS_DISK_IO_STATE             20018  // 告警内容：TTnmDevAlarmBase  磁盘IO 超阈值
#define TNM_ALARM_CODE_PMS_NETCARD_IO_STATE          20019  // 告警内容：TTnmDevAlarmBase  网卡吞吐量 超阈值
#define TNM_ALARM_CODE_RCS_DICK_FULL                 20020  // 告警内容：TTnmDevAlarmBase  存储空间满
#define TNM_ALARM_CODE_RCS_RUNNING_ERR               20021  // 告警内容：TTnmDevAlarmBase  运行状态异常
#define TNM_ALARM_CODE_CMU_PU_OFFLINE                20022  // 告警内容：TTnmDevAlarmBase  前端下线

#define TNM_ALARM_CODE_TAS_ADDPLTFRM_LOOP            20024  // 告警内容：TTnmDevAlarmBase  组网成环
#define TNM_ALARM_CODE_RMS_DEL_VIDEO                 20025  // 告警内容：TTnmDevAlarmBase  RMS因小文件删除录像文件
#define TNM_ALARM_CODE_PMS_NETCARD_OFFLINE           20026  // 告警内容：TTnmDevAlarmBase  正常工作的网卡掉线
#define TNM_ALARM_CODE_PMS_TIME_ERROR                20027  // 告警内容：TTnmDevAlarmBase  平台时间错误
#define TNM_ALARM_CODE_PMS_CPU_STATUS                20028  //CPU使用率高于指定阀值告警
#define TNM_ALARM_CODE_PMS_MEMORY_STATUS             20029  //内存使用超过指定阀值告警
#define TNM_ALARM_CODE_PMS_DISCONNECT_DATABASE       20030  //数据库连接失败(损坏)
#define TNM_ALARM_CODE_VTDU_LOSEPACKET               22031  //丢包
#define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         22032  //交换无码流
#define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           22033  //网络千兆百兆不符
#define TNM_ALARM_CODE_RCS_DISK_OFFLINE              22034  //磁阵下线
#define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          22035  //磁盘分区只读

#define TNM_ALARM_CODE_VTDU_STREAM_CHANGING          20023  // 告警内容：TnmAlarmCodeStreamErr  串码流
#define TNM_ALARM_CODE_VTDU_LOSEPACKET               22031  //丢包
#define TNM_ALARM_CODE_VTDU_RECV_NOBITSTREAM         22032  //交换无码流
#define TNM_ALARM_CODE_PMS_ETHCARD_RESTORE           22033  //网络千兆百兆不符
#define TNM_ALARM_CODE_RCS_DISK_OFFLINE              22034  //磁阵下线
#define TNM_ALARM_CODE_PMS_DISK_PART_R_ONLY          22035  //磁盘分区只读
#define TNM_ALARM_CODE_GPS_STORE_CONFIG_ERROR        22036  //订阅GPS轨迹存储未配置
#define TNM_ALARM_CODE_DOMAIN_ALIAS_NOT_VAILD        20037  //CUI1->nms, 域别名不合法
#define TNM_ALARM_CODE_DOMAIN_RELATION_NOT_LEGAL     20038  //cui1->nms,域关系不合法
#define TNM_ALARM_CODE_SUBDOMAIN_DIFF_DEVICE_KDMNO   20039  //cui1->nms,下级域别名信息与KDMNO不匹配
#define TNM_ALARM_CODE_DEVICE_TYPE_ERROR             20040  //设备类型不匹配
#define TNM_ALARM_CODE_MANAGE_LICENSE_TOO_MUCH       20041  //告警内容：TTnmDevAlarmBase 管理license超标
#define TNM_ALARM_CODE_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS     20042 //告警内容： TTnmDevAlarmBase  key获取工具校验失败
#define TNM_ALARM_CODE_OTHER_STORAGE_OVER_LICENSE_LIMIT          20043 //告警内容：管理

typedef struct tagTTnmModuleInfo
{
    tagTTnmModuleInfo()
    {
        memset(this, 0, sizeof(*this));
    }

    s8 name[TNM_MODULE_NAME_LEN];     // 模块名称
    s8 address[TNM_MODULE_ADDR_LEN];  // 模块地址
    s8 uri[TNM_URI_LEN];      // 模块uri
    s8 reserved[TNM_MODULE_RESERVED_LEN];  // 保留字段，用于对齐
}PACKED TTnmModuleInfo;

typedef struct tagTnmCmuPfmInfo2
{
    tagTnmCmuPfmInfo2()
    {
        memset(this, 0, sizeof(*this));
    }

    u32 switchNum;           // 转发码流路数
    u32 switchTotalBitRate;  // 转发总码率
    u32 onlineVideosourceNum;// 在线视频源总数
    u32 onlineCuNum;         // 在线CU总数
}PACKED TTnmCmuPfmInfo2;

typedef struct tagTTnmRcsPfmInfo
{
    tagTTnmRcsPfmInfo()
    {
        memset(this, 0, sizeof(*this));
    }

    u32 recTaskNum;  // 录像任务路数
    u32 recBitRate;  // 录像总码率
    u32 recVodNum;   // 录像点播路数
    u32 recVodTotalBitRate; // 录象点播总码率
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
    u8 alarmStatus; // 值为TNM_STATUS_ALARM或TNM_STATUS_NORMAL
    u8 reserved[TNM_DEVALARM_RESERVED_LEN]; // 保留字段

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
    s8 m_szDevAddr[TNM_DEV_ADDR_LEN+1];   //正常的地址
    /*
        无码流：字符串为空
        串码流：为异常地址对
        丢包：丢包百分比数值，int转string，不带%
    */
    s8 m_szParam[TNM_PARAM_LEN+1];
} PACKED;

//cui1的网管告警消息定义

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


/* 平台网元子类型常量 */
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
