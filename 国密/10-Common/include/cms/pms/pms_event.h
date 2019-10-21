/*****************************************************************************
   模块名      : pms_event.h 
   文件名      : pms_event.h
   相关文件    : 
   文件实现功能: pms_event.h 定义了所有PMC_PMS的消息ID
   作者        : 
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
******************************************************************************/

#ifndef _PMS_EVENT_H_
#define _PMS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* pms协议 */
enum 
{
    //设备登录、登出使用公共消息 DEV_LOGIN_REQ/DEV_LOGOUT_REQ

    //PMS_LOGIN_PMS_REQ = PMC_PMS_EVENT_BEGIN,  // 消息体 CLoginPmsReq  登录与注销
    //PMS_LOGIN_PMS_RSP,  // 消息体 CLoginPmsRsp
    //PMS_LOGOUT_PMS_REQ, // 消息体 CLogoutPmsReq
    //PMS_LOGOUT_PMS_RSP, // 消息体 CLogoutPmsRsp
    //PMS_PMC_ONLINE_REQ, // 消息体 CPmcOnlineReq  pmc用户上线(pms验证通过才允许上线)
    //PMS_PMC_ONLINE_RSP, // 消息体 CPmcOnlineRsp  
    //PMS_PMC_OFFLINE_REQ, // 消息体 CPmcOfflineReq  pmc用户下线
    //PMS_PMC_OFFLINE_RSP, // 消息体 CPmcOfflineRsp

    PMS_USER_QRY_REQ = PMC_PMS_EVENT_BEGIN,       // 消息体 CPmsUserQryReq PMS用户管理
    PMS_USER_QRY_RSP,       // 消息体 CPmsUserQryRsp
    PMS_USER_QRY_NTF_REQ,   // 消息体 CPmsUserQryNtfReq
    PMS_USER_QRY_NTF_RSP,   // 消息体 CNtfRsp
    PMS_USER_ADD_REQ,       // 消息体 CPmsUserAddReq
    PMS_USER_ADD_RSP,       // 消息体 CPmsUserAddRsp
    PMS_USER_DEL_REQ,       // 消息体 CPmsUserDelReq
    PMS_USER_DEL_RSP,       // 消息体 CPmsUserDelRsp
    PMS_USER_MOD_REQ,       // 消息体 CPmsUserModReq
    PMS_USER_MOD_RSP,       // 消息体 CPmsUserModRsp


    PMS_PLATFORM_HARDWAREINFO_QRY_REQ,      // 消息体 CPlatformHardwareInfoQryReq 平台硬件信息
    PMS_PLATFORM_HARDWAREINFO_QRY_RSP,      // 消息体 CPlatformHardwareInfoQryRsp

    PMS_PLATFORM_SOFTWAREINFO_QRY_REQ,      // 消息体 CPlatformSoftwareInfoQryReq 平台软件信息
    PMS_PLATFORM_SOFTWAREINFO_QRY_RSP,      // 消息体 CPlatformSoftwareInfoQryRsp
    PMS_PLATFORM_SOFTWAREINFO_MOD_REQ,      // 消息体 CPlatformSoftwareInfoModReq 修改（域名、proxy配置、stun配置、http端口），一次只允许修改一种
    PMS_PLATFORM_SOFTWAREINFO_MOD_RSP,      // 消息体 CPlatformSoftwareInfoModRsp

    PMS_MODULE_STATUS_ITEM_DEL_REQ, // 消息体 CModuleStatusItemDelReq 删除历史的模块状态条目
    PMS_MODULE_STATUS_ITEM_DEL_RSP, // 消息体 CModuleStatusItemDelRsp

    PMS_SYSTEM_DISK_INFO_GET_REQ,     // 消息体 CSystemDiskInfoGetReq  平台系统盘信息
    PMS_SYSTEM_DISK_INFO_GET_RSP,     // 消息体 CSystemDiskInfoGetRsp

    PMS_PROXY_CONFIGINFO_QRY_REQ,           //消息体 CProxyConfigInfoQryReq  hzytodo 删除它们
    PMS_PROXY_CONFIGINFO_QRY_RSP,           //消息体 CProxyConfigInfoQryRsp
    PMS_PROXY_CONFIGINFO_MOD_REQ,           //消息体 CProxyConfigInfoModReq  仅限于修改从平台的proxy地址
    PMS_PROXY_CONFIGINFO_MOD_RSP,           //消息体 CProxyConfigInfoModRsp
    
    PMS_STUN_SERVER_CONFIGINFO_QRY_REQ,     //消息体 CSTUNServerConfigInfoQryReq hzytodo 删除它们
    PMS_STUN_SERVER_CONFIGINFO_QRY_RSP,     //消息体 CSTUNServerConfigInfoQryRsp
    PMS_STUN_SERVER_CONFIGINFO_MOD_REQ,     //消息体 CSTUNServerConfigInfoModReq
    PMS_STUN_SERVER_CONFIGINFO_MOD_RSP,     //消息体 CSTUNServerConfigInfoModRsp

    PMS_NTP_SERVER_CONFIGINFO_QRY_REQ,      //消息体 CNTPServerConfigInfoQryReq  hzytodo 删除它们
    PMS_NTP_SERVER_CONFIGINFO_QRY_RSP,      //消息体 CNTPServerConfigInfoQryRsp
    PMS_NTP_SERVER_CONFIGINFO_MOD_REQ,      //消息体 CNTPServerConfigInfoModReq  
    PMS_NTP_SERVER_CONFIGINFO_MOD_RSP,      //消息体 CNTPServerConfigInfoModRsp

    PMS_CTRL_PMS_REQ,                       // 消息体 CCtrlPmsReq   PMS控制(包括关机，重启等行为控制)
    PMS_CTRL_PMS_RSP,                       // 消息体 CCtrlPmsRsp

    PMS_PLATFORM_TIMEINFO_QRY_REQ,          // 消息体 CPlatformTimeInfoQryReq 平台时间管理
    PMS_PLATFORM_TIMEINFO_QRY_RSP,          // 消息体 CPlatformTimeInfoQryRsp
    PMS_PLATFORM_TIMEINFO_MOD_REQ,          // 消息体 CPlatformTimeInfoModReq
    PMS_PLATFORM_TIMEINFO_MOD_RSP,          // 消息体 CPlatformTimeInfoModRsp

    PMS_NETCARDINFO_QRY_REQ,                // 消息体 CNetcardInfoQryReq  网卡信息
    PMS_NETCARDINFO_QRY_RSP,                // 消息体 CNetcardInfoQryRsp
    PMS_NETCARDINFO_MOD_REQ,                // 消息体 CNetcardInfoModReq
    PMS_NETCARDINFO_MOD_RSP,                // 消息体 CNetcardInfoModRsp

    PMS_NETROUTEINFO_QRY_REQ,               // 消息体 CNetRouteQryReq 网络路由
    PMS_NETROUTEINFO_QRY_RSP,               // 消息体 CNetRouteQryRsp
    PMS_NETROUTEINFO_QRY_NTF_REQ,           // 消息体 CNetRouteQryNtfReq
    PMS_NETROUTEINFO_QRY_NTF_RSP,           // 消息体 CNtfRsp
    PMS_NETROUTEINFO_ADD_REQ,               // 消息体 CNetRouteAddReq
    PMS_NETROUTEINFO_ADD_RSP,               // 消息体 CNetRouteAddRsp
    PMS_NETROUTEINFO_DEL_REQ,               // 消息体 CNetRouteDelReq
    PMS_NETROUTEINFO_DEL_RSP,               // 消息体 CNetRouteDelRsp
    PMS_NETDEFAULTROUTE_GET_REQ,            // 消息体 CNetDefaultRouteGetReq
    PMS_NETDEFAULTROUTE_GET_RSP,            // 消息体 CNetDefaultRouteGetRsp
    PMS_NETDEFAULTROUTE_SET_REQ,            // 消息体 CNetDefaultRouteSetReq
    PMS_NETDEFAULTROUTE_SET_RSP,            // 消息体 CNetDefaultRouteSetRsp

    PMS_NETNAT_QRY_REQ,                     // 消息体 CNetNatQryReq 网络NAT
    PMS_NETNAT_QRY_RSP,                     // 消息体 CNetNatQryRsp
    PMS_NETNAT_QRY_NTF_REQ,                 // 消息体 CNetNatQryNtfReq
    PMS_NETNAT_QRY_NTF_RSP,                 // 消息体 CNtfRsp
    PMS_NETNAT_ADD_REQ,                     // 消息体 CNetNatAddReq
    PMS_NETNAT_ADD_RSP,                     // 消息体 CNetNatAddRsp
    PMS_NETNAT_DEL_REQ,                     // 消息体 CNetNatDelReq
    PMS_NETNAT_DEL_RSP,                     // 消息体 CNetNatDelRsp

    PMS_PLATFORM_TOPO_QRY_REQ,              // 消息体 CPlatformTOPOQryReq 平台组网
    PMS_PLATFORM_TOPO_QRY_RSP,              // 消息体 CPlatformTOPOQryRsp
    PMS_PLATFORM_TOPO_QRY_NTF_REQ,          // 消息体 CPlatformTOPOQryNtfReq
    PMS_PLATFORM_TOPO_QRY_NTF_RSP,          // 消息体 CNtfRsp

    PMS_PLATFORM_TOPO_EXTERNINFO_GET_REQ,   // 消息体 CPlatformTopoExternInfoGetReq  获取接入平台的一些附加信息，为了不让用户感知那些信息而让程序自动获取
    PMS_PLATFORM_TOPO_EXTERNINFO_GET_RSP,   // 消息体 CPlatformTopoExternInfoGetRsp

    PMS_PLATFORM_TOPO_ADD_REQ,              // 消息体 CPlatformTOPOAddReq
    PMS_PLATFORM_TOPO_ADD_RSP,              // 消息体 CPlatformTOPOAddRsp
    PMS_PLATFORM_TOPO_ADD_NTF_REQ,          // 消息体 CPlatformTOPOAddNtfReq
    PMS_PLATFORM_TOPO_ADD_NTF_RSP,          // 消息体 CNtfRsp

    PMS_PLATFORM_TOPO_MOD_REQ,              // 消息体 CPlatformTOPOModReq 
    PMS_PLATFORM_TOPO_MOD_RSP,              // 消息体 CPlatformTOPOModRsp
    PMS_PLATFORM_TOPO_MOD_NTF_REQ,          // 消息体 CPlatformTOPOModNtfReq
    PMS_PLATFORM_TOPO_MOD_NTF_RSP,          // 消息体 CNtfRsp

    PMS_PLATFORM_TOPO_DEL_REQ,              // 消息体 CPlatformTOPODelReq
    PMS_PLATFORM_TOPO_DEL_RSP,              // 消息体 CPlatformTOPODelRsp
    PMS_PLATFORM_TOPO_DEL_NTF_REQ,          // 消息体 CPlatformTOPODelNtfReq
    PMS_PLATFORM_TOPO_DEL_NTF_RSP,          // 消息体 CNtfRsp

    PMS_PLATFORM_TOPO_SYNC_REQ,             // 消息体 CPlatformTOPOSyncReq  PMS之间TOPO同步请求（同步IP地址）
    PMS_PLATFORM_TOPO_SYNC_RSP,             // 消息体 CPlatformTOPOSyncRsp

    PMS_PLATFORM_MODULEINFO_QRY_REQ,        // 消息体 CPlatformModuleInfoQryReq  平台模块配置
    PMS_PLATFORM_MODULEINFO_QRY_RSP,        // 消息体 CPlatformModuleInfoQryRsp
    PMS_PLATFORM_MODULEINFO_QRY_NTF_REQ,    // 消息体 CPlatformModuleInfoQryNtfReq
    PMS_PLATFORM_MODULEINFO_QRY_NTF_RSP,    // 消息体 CNtfRsp
    PMS_PLATFORM_MODULEINFO_MOD_REQ,        // 消息体 CPlatformModuleInfoModReq
    PMS_PLATFORM_MODULEINFO_MOD_RSP,        // 消息体 CPlatformModuleInfoModRsp

    PMS_MIRROR_EXPORT_BEGIN_REQ,            // 消息体 CMirrorExportBeginReq  镜像导出管理 
    PMS_MIRROR_EXPORT_BEGIN_RSP,            // 消息体 CMirrorExportBeginRsp
	PMS_MIRROR_EXPORT_BEGIN_ACK,			// 消息体 CMirrorExportBeginAckReq
    PMS_MIRROR_EXPORT_END_REQ,              // 消息体 CMirrorExportEndReq
    PMS_MIRROR_EXPORT_END_RSP,              // 消息体 CMirrorExportEndRsp
    PMS_MIRROR_EXPORT_PROGRESS_NTF_REQ,     // 消息体 CMirrorExportProgressNtfReq
    PMS_MIRROR_EXPORT_PROGRESS_NTF_RSP,     // 消息体 CNtfRsp
    PMS_MIRROR_IMPORT_BEGIN_REQ,            // 消息体 CMirrorImportBeginReq  镜像导入管理
    PMS_MIRROR_IMPORT_BEGIN_RSP,            // 消息体 CMirrorImportBeginRsp
	PMS_MIRROR_IMPORT_BEGIN_ACK,			// 消息体 CMirrorImportBeginAckReq
    PMS_MIRROR_IMPORT_END_REQ,              // 消息体 CMirrorImportEndReq
    PMS_MIRROR_IMPORT_END_RSP,              // 消息体 CMirrorImportEndRsp
    PMS_MIRROR_IMPORT_PROGRESS_NTF_REQ,     // 消息体 CMirrorImportProgressNtfReq
    PMS_MIRROR_IMPORT_PROGRESS_NTF_RSP,     // 消息体 CNtfRsp
    PMS_MIRROR_RESET_REQ,                   // 消息体 CMirrorResetReq        镜像恢复出厂配置
    PMS_MIRROR_RESET_RSP,                   // 消息体 CMirrorResetRsp
	PMS_MIRROR_RESET_ACK,					// 消息体 CEventReq
    PMS_MIRROR_RESET_PROGRESS_NTF_REQ,      // 消息体 CMirrorResetProgressNtfReq
    PMS_MIRROR_RESET_PROGRESS_NTF_RSP,      // 消息体 CNtfRsp
    PMS_MIRROR_UPDATE_BEGIN_REQ,            // 消息体 CMirrorUpdateBeginReq  镜像升级
    PMS_MIRROR_UPDATE_BEGIN_RSP,            // 消息体 CMirrorUpdateBeginRsp
	PMS_MIRROR_UPDATE_BEGIN_ACK,			// 消息体 CMirrorUpdateBeginAckReq
    PMS_MIRROR_UPDATE_END_REQ,              // 消息体 CMirrorUpdateEndReq
    PMS_MIRROR_UPDATE_END_RSP,              // 消息体 CMirrorUpdateEndRsp
    PMS_MIRROR_UPDATE_PROGRESS_NTF_REQ,     // 消息体 CMirrorUpdateProgressNtfReq
    PMS_MIRROR_UPDATE_PROGRESS_NTF_RSP,     // 消息体 CNtfRsp
    
    PMS_PLATFORM_LOG_QRY_REQ,               // 消息体 CPlatformLogInfoQryReq  日志管理（平台日志）
    PMS_PLATFORM_LOG_QRY_RSP,               // 消息体 CPlatformLogInfoQryRsp
    PMS_PLATFORM_LOG_QRY_NTF_REQ,           // 消息体 CPlatformLogInfoNtfReq
    PMS_PLATFORM_LOG_QRY_NTF_RSP,           // 消息体 CNtfRsp

    PMS_USER_OPERATION_LOG_ADD_REQ,         // 消息体 CUserOperationLogAddReq 日志管理（用户日志）
    PMS_USER_OPERATION_LOG_ADD_RSP,         // 消息体 CUserOperationLogAddRsp
    PMS_USER_OPERATION_LOG_QRY_REQ,         // 消息体 CUserOperationLogQryReq 
    PMS_USER_OPERATION_LOG_QRY_RSP,         // 消息体 CUserOperationLogQryRsp
    PMS_USER_OPERATION_LOG_QRY_NTF_REQ,     // 消息体 CUserOperationLogNtfReq
    PMS_USER_OPERATION_LOG_QRY_NTF_RSP,     // 消息体 CNtfRsp

    PMS_NMS_DEVICE_QRY_REQ,         // 消息体 CNMSDeviceQryReq  网管功能 手工销警属于客户端行为
    PMS_NMS_DEVICE_QRY_RSP,         // 消息体 CNMSDeviceQryRsp
    PMS_NMS_DEVICE_QRY_NTF_REQ,     // 消息体 CNMSDeviceQryNtfReq
    PMS_NMS_DEVICE_QRY_NTF_RSP,     // 消息体 CNtfRsp
    PMS_NMS_DEVICE_SYNC_NTF_REQ,    // 消息体 CNMSDeviceSyncNtfReq (运行过程中设备变更同步，包括增删改行为)
    PMS_NMS_DEVICE_SYNC_NTF_RSP,    // 消息体 CNtfRsp (运行过程中设备变更同步，包括增删改行为)

    PMS_NMS_ALARM_QRY_REQ,          // 消息体 CNMSAlarmQryReq
    PMS_NMS_ALARM_QRY_RSP,          // 消息体 CNMSAlarmQryRsp
    PMS_NMS_ALARM_QRY_NTF_REQ,      // 消息体 CNMSAlarmQryNtfReq
    PMS_NMS_ALARM_QRY_NTF_RSP,      // 消息体 CNtfRsp
    PMS_NMS_ALARM_MOD_REQ,          // 消息体 CNMSAlarmModReq
    PMS_NMS_ALARM_MOD_RSP,          // 消息体 CNMSAlarmModRsp
    PMS_NMS_ALARM_SYNC_NTF_REQ,     // 消息体 CNMSAlarmSyncNtfReq  (运行过程中告警变更同步，包括增删改行为)
    PMS_NMS_ALARM_SYNC_NTF_RSP,     // 消息体 CNtfRsp  (运行过程中告警变更同步，包括增删改行为)

    PMS_MAIL_CFG_SET_REQ,           // 消息体 CMailCfgSetReq 邮件相关设置
    PMS_MAIL_CFG_SET_RSP,           // 消息体 CMailCfgSetRsp
    PMS_MAIL_CFG_QRY_REQ,           // 消息体 CMailCfgQryReq
    PMS_MAIL_CFG_QRY_RSP,           // 消息体 CMailCfgQryRsp
    PMS_SEND_TEST_MAIL_REQ,         // 消息体 CSendTestMailReq
    PMS_SEND_TEST_MAIL_RSP,         // 消息体 CSendTestMailRsp

	// UPNP相关信息
	PMS_UPNP_MAP_INFO_GET_MODE_REQ, // 消息体 CUPnPMapInfoGetModeReq
	PMS_UPNP_MAP_INFO_GET_MODE_RSP, // 消息体 CUPnPMapInfoGetModeRsp
	PMS_UPNP_MAP_INFO_SET_MODE_REQ, // 消息体 CUPnPMapInfoSetModeReq
	PMS_UPNP_MAP_INFO_SET_MODE_RSP, // 消息体 CUPnPMapInfoSetModeRsp

	PMS_UPNP_MAP_INFO_QRY_REQ,      // 消息体 CUPnPMapInfoQryReq
	PMS_UPNP_MAP_INFO_QRY_RSP,      // 消息体 CUPnPMapInfoQryRsp
	PMS_UPNP_MAP_INFO_QRY_NTF_REQ,  // 消息体 CUPnPMapInfoQryNtfReq
	PMS_UPNP_MAP_INFO_QRY_NTF_RSP,  // 消息体 CUPnPMapInfoQryNtfRsp
	PMS_UPNP_MAP_INFO_ADD_REQ,      // 消息体 CUPnPMapInfoAddReq
	PMS_UPNP_MAP_INFO_ADD_RSP,      // 消息体 CUPnPMapInfoAddRsp
	PMS_UPNP_MAP_INFO_DEL_REQ,      // 消息体 CUPnPMapInfoDelReq
	PMS_UPNP_MAP_INFO_DEL_RSP,      // 消息体 CUPnPMapInfoDelRsp
	PMS_UPNP_MAP_INFO_MOD_REQ,      // 消息体 CUPnPMapInfoModReq
	PMS_UPNP_MAP_INFO_MOD_RSP,      // 消息体 CUPnPMapInfoModRsp

	PMS_UPNP_MAP_INFO_SYNC_REQ,     // 消息体 CUPnPMapInfoSyncReq
	PMS_UPNP_MAP_INFO_SYNC_RSP,     // 消息体 CUPnPMapInfoSyncRsp

    //端口映射相关消息
	PMS_PORT_MAP_INFO_QRY_REQ,      // 消息体 CPortMapInfoQryReq
	PMS_PORT_MAP_INFO_QRY_RSP,      // 消息体 CPortMapInfoQryRsp
	PMS_PORT_MAP_INFO_QRY_NTF_REQ,  // 消息体 CPortMapInfoQryNtfReq
	PMS_PORT_MAP_INFO_QRY_NTF_RSP,  // 消息体 CPortMapInfoQryNtfRsp
	PMS_PORT_MAP_INFO_ADD_REQ,      // 消息体 CPortMapInfoAddReq
	PMS_PORT_MAP_INFO_ADD_RSP,      // 消息体 CPortMapInfoAddRsp
	PMS_PORT_MAP_INFO_DEL_REQ,      // 消息体 CPortMapInfoDelReq
	PMS_PORT_MAP_INFO_DEL_RSP,      // 消息体 CPortMapInfoDelRsp
	PMS_PORT_MAP_INFO_MOD_REQ,      // 消息体 CPortMapInfoModReq
	PMS_PORT_MAP_INFO_MOD_RSP,      // 消息体 CPortMapInfoModRsp



	// DDNS相关信令
	PMS_DDNS_STATUS_GET_REQ,        // 消息体 CDdnsStatusGetReq
	PMS_DDNS_STATUS_GET_RSP,        // 消息体 CDdnsStatusGetRsp
	PMS_DDNS_STATUS_SET_REQ,        // 消息体 CDdnsStatusSetReq
	PMS_DDNS_STATUS_SET_RSP,        // 消息体 CDdnsStatusSetRsp

	PMS_MODULE_CONF_GET_REQ,		// 消息体 CModuleConfGetReq
	PMS_MODULE_CONF_GET_RSP,		// 消息体 CModuleConfGetRsp

    PMS_DEFENSE_CROSS_STREAM_SET_REQ,   //消息体CDefenseCrossStreamSetReq
    PMS_DEFENSE_CROSS_STREAM_SET_RSP,   //消息体CDefenseCrossStreamSetRsp
    PMS_DEFENSE_CROSS_STREAM_GET_REQ,   //消息体CDefenseCrossStreamGetReq
    PMS_DEFENSE_CROSS_STREAM_GET_RSP,   //消息体CDefenseCrossStreamGetRsp

	PMS_QRCODE_IMG_UPDATE_REQ,        //消息体 CPmsQrcodeImgUpdateReq
	PMS_QRCODE_IMG_UPDATE_RSP,        //消息体 CPmsQrcodeImgUpdateRsp
    PMS_SECUREMAIL_INFO_GET_REQ,             //消息体CPmsSecuremailInfoGetReq
    PMS_SECUREMAIL_INFO_GET_RSP,             //消息体CPmsSecuremailInfoGetRsp
    PMS_RESET_CU_PASSWORD_REQ,              //消息体CPmsResetCuPasswordReq
    PMS_RESET_CU_PASSWORD_RSP,              //消息体CPmsResetCuPasswordRsp
    PMS_PMC_PASSWORD_RESET_INFO_GET_REQ, //消息体CPmsPmcPasswordResetInfoGetReq
    PMS_PMC_PASSWORD_RESET_INFO_GET_RSP, //消息体CPmsPmcPasswordResetInfoGetRsp
    PMS_PMC_PASSWORD_RESET_REQ,        //消息体CPmsPmcPasswordResetReq 
    PMS_PMC_PASSWORD_RESET_RSP,        //消息体CPmsPmcPasswordResetRsp

	PMS_IPTABLES_SET_REQ,             //消息体CPmsIptablesSetReq 
	PMS_IPTABLES_SET_RSP,             //消息体CPmsIptablesSetRsp 
	PMS_IPTABLES_GET_REQ,             //消息体CPmsIptablesGetReq
	PMS_IPTABLES_GET_RSP,             //消息体CPmsIptablesGetRsp
	PMS_HEARTBEAT_BAKPC_CONF_REQ,     //消息体CPmsHeartbeatBakpcConfReq
	PMS_HEARTBEAT_BAKPC_CONF_RSP,     //消息体CPmsHeartbeatBakpcConfRsp
	PMS_DEFAULT_MIRROR_MAKE_ACK,      //消息体CPmsDefaultMirrorMakeAck
	PMS_HEARTBEAT_HOST_CONF_REQ,      //消息体CPmsHeartbeatHostConfReq
	PMS_HEARTBEAT_HOST_CONF_RSP,      //消息体CPmsHeartbeatHostConfRsp
	PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_REQ,  //消息体CPmsDefaultMirrorMakeProgressNtfReq
	PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_RSP,  //消息体CPmsDefaultMirrorMakeProgressNtfRsp
	PMS_DEFAULT_MIRROR_MAKE_END_REQ,  //消息体CPmsDefaultMirrorMakeEndReq
	PMS_DEFAULT_MIRROR_MAKE_END_RSP,  //消息体CPmsDefaultMirrorMakeEndRsp
	PMS_TCPDUMP_START_REQ,             //消息体CPmsTcpdumpStartReq
	PMS_TCPDUMP_START_RSP,             //消息体CPmsTcpdumpStartRsp
	PMS_TCPDUMP_BEGIN_ACK,             //消息体CTcpdumpBeginAckReq
	PMS_TCPDUMP_STOP_REQ,              //消息体CPmsTcpdumpStopReq
	PMS_TCPDUMP_STOP_RSP,              //消息体CPmsTcpdumpStopRsp
	PMS_TCPDUMP_STOP_NTF_REQ,         //消息体CTcpdumpStopNtfReq
	PMS_TCPDUMP_STOP_NTF_RSP,         //消息体CTcpdumpStopNtfRsp
	PMS_TCPDUMP_DOWNLOAD_REQ,          //消息体CPmsTcpdumpDownloadReq
	PMS_TCPDUMP_DOWNLOAD_RSP,          //消息体CPmsTcpdumpDownloadRsp
	PMS_MIRROR_BAK_CONF_SET_REQ,       //消息体CMirrorBakConfSetReq
	PMS_MIRROR_BAK_CONF_SET_RSP,       //消息体CMirrorBakConfSetRsp
	PMS_MIRROR_BAK_CONF_GET_REQ,       //消息体CMirrorBakConfGetReq
	PMS_MIRROR_BAK_CONF_GET_RSP,       //消息体CMirrorBakConfGetRsp
	PMS_MIRROR_BAK_FILE_LIST_GET_REQ,  //消息体CMirrorBakFileListGetReq
	PMS_MIRROR_BAK_FILE_LIST_GET_RSP,  //消息体CMirrorBakFileListGetRsp
	PMS_HEARTBEAT_CONF_GET_REQ,        //消息体CPmsHeartbeatConfGetReq
	PMS_HEARTBEAT_CONF_GET_RSP,        //消息体CPmsHeartbeatConfGetRsp
	PMS_NRU_INFO_QRY_REQ,              //消息体CNruInfoQryReq
	PMS_NRU_INFO_QRY_RSP,			   //消息体CNruInfoQryRsp
	PMS_NRU_INFO_UPDATE_REQ,           //消息体CNruInfoUpdateReq
	PMS_NRU_INFO_UPDATE_RSP,           //消息体CNruInfoUpdateRsp
	

	PMS_CURRENT_EVENT_END,
    //////////////////////////////////////////////////////////////////////////////
    
};

inline void InitPmcPmsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(PMS_USER_QRY_REQ);                        // 消息体 CPmsUserQryReq PMS用户管理
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_RSP);                        // 消息体 CPmsUserQryRsp
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_NTF_REQ);                    // 消息体 CPmsUserQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_NTF_RSP);                    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_USER_ADD_REQ);                        // 消息体 CPmsUserAddReq
    OSP_ADD_EVENT_DESC(PMS_USER_ADD_RSP);                        // 消息体 CPmsUserAddRsp
    OSP_ADD_EVENT_DESC(PMS_USER_DEL_REQ);                        // 消息体 CPmsUserDelReq
    OSP_ADD_EVENT_DESC(PMS_USER_DEL_RSP);                        // 消息体 CPmsUserDelRsp
    OSP_ADD_EVENT_DESC(PMS_USER_MOD_REQ);                        // 消息体 CPmsUserModReq
    OSP_ADD_EVENT_DESC(PMS_USER_MOD_RSP);                        // 消息体 CPmsUserModRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_HARDWAREINFO_QRY_REQ);       // 消息体 CPlatformHardwareInfoQryReq 平台硬件信息
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_HARDWAREINFO_QRY_RSP);       // 消息体 CPlatformHardwareInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_QRY_REQ);       // 消息体 CPlatformSoftwareInfoQryReq 平台软件信息
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_QRY_RSP);       // 消息体 CPlatformSoftwareInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_MOD_REQ);       // 消息体 CPlatformSoftwareInfoModReq 修改（域名、proxy配置、stun配置、http端口），一次只允许修改一种
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_MOD_RSP);       // 消息体 CPlatformSoftwareInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_MODULE_STATUS_ITEM_DEL_REQ);          // 消息体 CModuleStatusItemDelReq 删除历史的模块状态条目
    OSP_ADD_EVENT_DESC(PMS_MODULE_STATUS_ITEM_DEL_RSP);          // 消息体 CModuleStatusItemDelRsp
    OSP_ADD_EVENT_DESC(PMS_SYSTEM_DISK_INFO_GET_REQ);            // 消息体 CSystemDiskInfoGetReq  平台系统盘信息
    OSP_ADD_EVENT_DESC(PMS_SYSTEM_DISK_INFO_GET_RSP);            // 消息体 CSystemDiskInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_QRY_REQ);            //消息体 CProxyConfigInfoQryReq  hzytodo 删除它们
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_QRY_RSP);            //消息体 CProxyConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_MOD_REQ);            //消息体 CProxyConfigInfoModReq  仅限于修改从平台的proxy地址
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_MOD_RSP);            //消息体 CProxyConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_QRY_REQ);      //消息体 CSTUNServerConfigInfoQryReq hzytodo 删除它们
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_QRY_RSP);      //消息体 CSTUNServerConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_MOD_REQ);      //消息体 CSTUNServerConfigInfoModReq
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_MOD_RSP);      //消息体 CSTUNServerConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_QRY_REQ);       //消息体 CNTPServerConfigInfoQryReq  hzytodo 删除它们
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_QRY_RSP);       //消息体 CNTPServerConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_MOD_REQ);       //消息体 CNTPServerConfigInfoModReq  
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_MOD_RSP);       //消息体 CNTPServerConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_CTRL_PMS_REQ);                        // 消息体 CCtrlPmsReq   PMS控制(包括关机，重启等行为控制)
    OSP_ADD_EVENT_DESC(PMS_CTRL_PMS_RSP);                        // 消息体 CCtrlPmsRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_QRY_REQ);           // 消息体 CPlatformTimeInfoQryReq 平台时间管理
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_QRY_RSP);           // 消息体 CPlatformTimeInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_MOD_REQ);           // 消息体 CPlatformTimeInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_MOD_RSP);           // 消息体 CPlatformTimeInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_QRY_REQ);                 // 消息体 CNetcardInfoQryReq  网卡信息
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_QRY_RSP);                 // 消息体 CNetcardInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_MOD_REQ);                 // 消息体 CNetcardInfoModReq
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_MOD_RSP);                 // 消息体 CNetcardInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_REQ);                // 消息体 CNetRouteQryReq 网络路由
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_RSP);                // 消息体 CNetRouteQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_NTF_REQ);            // 消息体 CNetRouteQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_ADD_REQ);                // 消息体 CNetRouteAddReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_ADD_RSP);                // 消息体 CNetRouteAddRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_DEL_REQ);                // 消息体 CNetRouteDelReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_DEL_RSP);                // 消息体 CNetRouteDelRsp
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_GET_REQ);             // 消息体 CNetDefaultRouteGetReq
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_GET_RSP);             // 消息体 CNetDefaultRouteGetRsp
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_SET_REQ);             // 消息体 CNetDefaultRouteSetReq
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_SET_RSP);             // 消息体 CNetDefaultRouteSetRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_REQ);                      // 消息体 CNetNatQryReq 网络NAT
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_RSP);                      // 消息体 CNetNatQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_NTF_REQ);                  // 消息体 CNetNatQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_NTF_RSP);                  // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_ADD_REQ);                      // 消息体 CNetNatAddReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_ADD_RSP);                      // 消息体 CNetNatAddRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_DEL_REQ);                      // 消息体 CNetNatDelReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_DEL_RSP);                      // 消息体 CNetNatDelRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_REQ);               // 消息体 CPlatformTOPOQryReq 平台组网
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_RSP);               // 消息体 CPlatformTOPOQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_NTF_REQ);           // 消息体 CPlatformTOPOQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_EXTERNINFO_GET_REQ);    // 消息体 CPlatformTopoExternInfoGetReq  获取接入平台的一些附加信息，为了不让用户感知那些信息而让程序自动获取
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_EXTERNINFO_GET_RSP);    // 消息体 CPlatformTopoExternInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_REQ);               // 消息体 CPlatformTOPOAddReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_RSP);               // 消息体 CPlatformTOPOAddRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_NTF_REQ);           // 消息体 CPlatformTOPOAddNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_REQ);               // 消息体 CPlatformTOPOModReq 
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_RSP);               // 消息体 CPlatformTOPOModRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_NTF_REQ);           // 消息体 CPlatformTOPOModNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_REQ);               // 消息体 CPlatformTOPODelReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_RSP);               // 消息体 CPlatformTOPODelRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_NTF_REQ);           // 消息体 CPlatformTOPODelNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_NTF_RSP);           // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_SYNC_REQ);              // 消息体 CPlatformTOPOSyncReq  PMS之间TOPO同步请求（同步IP地址）
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_SYNC_RSP);              // 消息体 CPlatformTOPOSyncRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_REQ);         // 消息体 CPlatformModuleInfoQryReq  平台模块配置
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_RSP);         // 消息体 CPlatformModuleInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_NTF_REQ);     // 消息体 CPlatformModuleInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_NTF_RSP);     // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_MOD_REQ);         // 消息体 CPlatformModuleInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_MOD_RSP);         // 消息体 CPlatformModuleInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_REQ);             // 消息体 CMirrorExportBeginReq  镜像导出管理 
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_RSP);             // 消息体 CMirrorExportBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_ACK);             // 消息体 CMirrorExportBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_END_REQ);               // 消息体 CMirrorExportEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_END_RSP);               // 消息体 CMirrorExportEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_PROGRESS_NTF_REQ);      // 消息体 CMirrorExportProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_PROGRESS_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_REQ);             // 消息体 CMirrorImportBeginReq  镜像导入管理
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_RSP);             // 消息体 CMirrorImportBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_ACK);             // 消息体 CMirrorImportBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_END_REQ);               // 消息体 CMirrorImportEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_END_RSP);               // 消息体 CMirrorImportEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_PROGRESS_NTF_REQ);      // 消息体 CMirrorImportProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_PROGRESS_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_REQ);                    // 消息体 CMirrorResetReq        镜像恢复出厂配置
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_RSP);                    // 消息体 CMirrorResetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_ACK);                    // 消息体 CEventReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_PROGRESS_NTF_REQ);       // 消息体 CMirrorResetProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_PROGRESS_NTF_RSP);       // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_REQ);             // 消息体 CMirrorUpdateBeginReq  镜像升级
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_RSP);             // 消息体 CMirrorUpdateBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_ACK);             // 消息体 CMirrorUpdateBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_END_REQ);               // 消息体 CMirrorUpdateEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_END_RSP);               // 消息体 CMirrorUpdateEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_PROGRESS_NTF_REQ);      // 消息体 CMirrorUpdateProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_PROGRESS_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_REQ);                // 消息体 CPlatformLogInfoQryReq  日志管理（平台日志）
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_RSP);                // 消息体 CPlatformLogInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_NTF_REQ);            // 消息体 CPlatformLogInfoNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_NTF_RSP);            // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_ADD_REQ);          // 消息体 CUserOperationLogAddReq 日志管理（用户日志）
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_ADD_RSP);          // 消息体 CUserOperationLogAddRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_REQ);          // 消息体 CUserOperationLogQryReq 
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_RSP);          // 消息体 CUserOperationLogQryRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_NTF_REQ);      // 消息体 CUserOperationLogNtfReq
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_NTF_RSP);      // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_REQ);                  // 消息体 CNMSDeviceQryReq  网管功能 手工销警属于客户端行为
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_RSP);                  // 消息体 CNMSDeviceQryRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_NTF_REQ);              // 消息体 CNMSDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_NTF_RSP);              // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_SYNC_NTF_REQ);             // 消息体 CNMSDeviceSyncNtfReq (运行过程中设备变更同步，包括增删改行为)
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_SYNC_NTF_RSP);             // 消息体 CNtfRsp (运行过程中设备变更同步，包括增删改行为)
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_REQ);                   // 消息体 CNMSAlarmQryReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_RSP);                   // 消息体 CNMSAlarmQryRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_NTF_REQ);               // 消息体 CNMSAlarmQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_NTF_RSP);               // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_MOD_REQ);                   // 消息体 CNMSAlarmModReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_MOD_RSP);                   // 消息体 CNMSAlarmModRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_SYNC_NTF_REQ);              // 消息体 CNMSAlarmSyncNtfReq  (运行过程中告警变更同步，包括增删改行为)
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_SYNC_NTF_RSP);              // 消息体 CNtfRsp  (运行过程中告警变更同步，包括增删改行为)
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_SET_REQ);                    // 消息体 CMailCfgSetReq 邮件相关设置
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_SET_RSP);                    // 消息体 CMailCfgSetRsp
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_QRY_REQ);                    // 消息体 CMailCfgQryReq
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_QRY_RSP);                    // 消息体 CMailCfgQryRsp
    OSP_ADD_EVENT_DESC(PMS_SEND_TEST_MAIL_REQ);                  // 消息体 CSendTestMailReq
    OSP_ADD_EVENT_DESC(PMS_SEND_TEST_MAIL_RSP);                  // 消息体 CSendTestMailRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_GET_MODE_REQ);          // 消息体 CUPnPMapInfoGetModeReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_GET_MODE_RSP);          // 消息体 CUPnPMapInfoGetModeRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SET_MODE_REQ);          // 消息体 CUPnPMapInfoSetModeReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SET_MODE_RSP);          // 消息体 CUPnPMapInfoSetModeRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_REQ);               // 消息体 CUPnPMapInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_RSP);               // 消息体 CUPnPMapInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_NTF_REQ);           // 消息体 CUPnPMapInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_NTF_RSP);           // 消息体 CUPnPMapInfoQryNtfRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_ADD_REQ);               // 消息体 CUPnPMapInfoAddReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_ADD_RSP);               // 消息体 CUPnPMapInfoAddRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_DEL_REQ);               // 消息体 CUPnPMapInfoDelReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_DEL_RSP);               // 消息体 CUPnPMapInfoDelRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_MOD_REQ);               // 消息体 CUPnPMapInfoModReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_MOD_RSP);               // 消息体 CUPnPMapInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SYNC_REQ);              // 消息体 CUPnPMapInfoSyncReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SYNC_RSP);              // 消息体 CUPnPMapInfoSyncRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_REQ);               // 消息体 CPortMapInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_RSP);               // 消息体 CPortMapInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_NTF_REQ);           // 消息体 CPortMapInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_NTF_RSP);           // 消息体 CPortMapInfoQryNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_ADD_REQ);               // 消息体 CPortMapInfoAddReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_ADD_RSP);               // 消息体 CPortMapInfoAddRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_DEL_REQ);               // 消息体 CPortMapInfoDelReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_DEL_RSP);               // 消息体 CPortMapInfoDelRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_MOD_REQ);               // 消息体 CPortMapInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_MOD_RSP);               // 消息体 CPortMapInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_GET_REQ);                 // 消息体 CDdnsStatusGetReq
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_GET_RSP);                 // 消息体 CDdnsStatusGetRsp
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_SET_REQ);                 // 消息体 CDdnsStatusSetReq
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_SET_RSP);                 // 消息体 CDdnsStatusSetRsp
    OSP_ADD_EVENT_DESC(PMS_MODULE_CONF_GET_REQ);                 // 消息体 CModuleConfGetReq
    OSP_ADD_EVENT_DESC(PMS_MODULE_CONF_GET_RSP);                 // 消息体 CModuleConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_SET_REQ);        //消息体CDefenseCrossStreamSetReq
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_SET_RSP);        //消息体CDefenseCrossStreamSetRsp
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_GET_REQ);        //消息体CDefenseCrossStreamGetReq
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_GET_RSP);        //消息体CDefenseCrossStreamGetRsp
    OSP_ADD_EVENT_DESC(PMS_QRCODE_IMG_UPDATE_REQ);               //消息体 CPmsQrcodeImgUpdateReq
    OSP_ADD_EVENT_DESC(PMS_QRCODE_IMG_UPDATE_RSP);               //消息体 CPmsQrcodeImgUpdateRsp
    OSP_ADD_EVENT_DESC(PMS_SECUREMAIL_INFO_GET_REQ);             //消息体CPmsSecuremailInfoGetReq
    OSP_ADD_EVENT_DESC(PMS_SECUREMAIL_INFO_GET_RSP);             //消息体CPmsSecuremailInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_RESET_CU_PASSWORD_REQ);               //消息体CPmsResetCuPasswordReq
    OSP_ADD_EVENT_DESC(PMS_RESET_CU_PASSWORD_RSP);               //消息体CPmsResetCuPasswordRsp
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_INFO_GET_REQ);     //消息体CPmsPmcPasswordResetInfoGetReq
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_INFO_GET_RSP);     //消息体CPmsPmcPasswordResetInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_REQ);              //消息体CPmsPmcPasswordResetReq 
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_RSP);              //消息体CPmsPmcPasswordResetRsp
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_SET_REQ);                    //消息体CPmsIptablesSetReq 
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_SET_RSP);                    //消息体CPmsIptablesSetRsp 
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_GET_REQ);                    //消息体CPmsIptablesGetReq
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_GET_RSP);                    //消息体CPmsIptablesGetRsp
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_BAKPC_CONF_REQ);            //消息体CPmsHeartbeatBakpcConfReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_BAKPC_CONF_RSP);            //消息体CPmsHeartbeatBakpcConfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_ACK);             //消息体CPmsDefaultMirrorMakeAck
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_HOST_CONF_REQ);             //消息体CPmsHeartbeatHostConfReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_HOST_CONF_RSP);             //消息体CPmsHeartbeatHostConfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_REQ);//消息体CPmsDefaultMirrorMakeProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_RSP);//消息体CPmsDefaultMirrorMakeProgressNtfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_END_REQ);         //消息体CPmsDefaultMirrorMakeEndReq
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_END_RSP);         //消息体CPmsDefaultMirrorMakeEndRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_START_REQ);                   //消息体CPmsTcpdumpStartReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_START_RSP);                   //消息体CPmsTcpdumpStartRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_BEGIN_ACK);                   //消息体CTcpdumpBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_REQ);                    //消息体CPmsTcpdumpStopReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_RSP);                    //消息体CPmsTcpdumpStopRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_NTF_REQ);                //消息体CTcpdumpStopNtfReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_NTF_RSP);                //消息体CTcpdumpStopNtfRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_DOWNLOAD_REQ);                //消息体CPmsTcpdumpDownloadReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_DOWNLOAD_RSP);                //消息体CPmsTcpdumpDownloadRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_SET_REQ);             //消息体CMirrorBakConfSetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_SET_RSP);             //消息体CMirrorBakConfSetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_GET_REQ);             //消息体CMirrorBakConfGetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_GET_RSP);             //消息体CMirrorBakConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_FILE_LIST_GET_REQ);        //消息体CMirrorBakFileListGetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_FILE_LIST_GET_RSP);        //消息体CMirrorBakFileListGetRsp
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_CONF_GET_REQ);              //消息体CPmsHeartbeatConfGetReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_CONF_GET_RSP);              //消息体CPmsHeartbeatConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_QRY_REQ);                    //消息体CNruInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_QRY_RSP);                    //消息体CNruInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_UPDATE_REQ);                 //消息体CNruInfoUpdateReq
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_UPDATE_RSP);                 //消息体CNruInfoUpdateRsp
    OSP_ADD_EVENT_DESC(PMS_CURRENT_EVENT_END);                   
}

#endif // _PMS_EVENT_H_







































