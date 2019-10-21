/*****************************************************************************
   ģ����      : pms_event.h 
   �ļ���      : pms_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: pms_event.h ����������PMC_PMS����ϢID
   ����        : 
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
******************************************************************************/

#ifndef _PMS_EVENT_H_
#define _PMS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* pmsЭ�� */
enum 
{
    //�豸��¼���ǳ�ʹ�ù�����Ϣ DEV_LOGIN_REQ/DEV_LOGOUT_REQ

    //PMS_LOGIN_PMS_REQ = PMC_PMS_EVENT_BEGIN,  // ��Ϣ�� CLoginPmsReq  ��¼��ע��
    //PMS_LOGIN_PMS_RSP,  // ��Ϣ�� CLoginPmsRsp
    //PMS_LOGOUT_PMS_REQ, // ��Ϣ�� CLogoutPmsReq
    //PMS_LOGOUT_PMS_RSP, // ��Ϣ�� CLogoutPmsRsp
    //PMS_PMC_ONLINE_REQ, // ��Ϣ�� CPmcOnlineReq  pmc�û�����(pms��֤ͨ������������)
    //PMS_PMC_ONLINE_RSP, // ��Ϣ�� CPmcOnlineRsp  
    //PMS_PMC_OFFLINE_REQ, // ��Ϣ�� CPmcOfflineReq  pmc�û�����
    //PMS_PMC_OFFLINE_RSP, // ��Ϣ�� CPmcOfflineRsp

    PMS_USER_QRY_REQ = PMC_PMS_EVENT_BEGIN,       // ��Ϣ�� CPmsUserQryReq PMS�û�����
    PMS_USER_QRY_RSP,       // ��Ϣ�� CPmsUserQryRsp
    PMS_USER_QRY_NTF_REQ,   // ��Ϣ�� CPmsUserQryNtfReq
    PMS_USER_QRY_NTF_RSP,   // ��Ϣ�� CNtfRsp
    PMS_USER_ADD_REQ,       // ��Ϣ�� CPmsUserAddReq
    PMS_USER_ADD_RSP,       // ��Ϣ�� CPmsUserAddRsp
    PMS_USER_DEL_REQ,       // ��Ϣ�� CPmsUserDelReq
    PMS_USER_DEL_RSP,       // ��Ϣ�� CPmsUserDelRsp
    PMS_USER_MOD_REQ,       // ��Ϣ�� CPmsUserModReq
    PMS_USER_MOD_RSP,       // ��Ϣ�� CPmsUserModRsp


    PMS_PLATFORM_HARDWAREINFO_QRY_REQ,      // ��Ϣ�� CPlatformHardwareInfoQryReq ƽ̨Ӳ����Ϣ
    PMS_PLATFORM_HARDWAREINFO_QRY_RSP,      // ��Ϣ�� CPlatformHardwareInfoQryRsp

    PMS_PLATFORM_SOFTWAREINFO_QRY_REQ,      // ��Ϣ�� CPlatformSoftwareInfoQryReq ƽ̨�����Ϣ
    PMS_PLATFORM_SOFTWAREINFO_QRY_RSP,      // ��Ϣ�� CPlatformSoftwareInfoQryRsp
    PMS_PLATFORM_SOFTWAREINFO_MOD_REQ,      // ��Ϣ�� CPlatformSoftwareInfoModReq �޸ģ�������proxy���á�stun���á�http�˿ڣ���һ��ֻ�����޸�һ��
    PMS_PLATFORM_SOFTWAREINFO_MOD_RSP,      // ��Ϣ�� CPlatformSoftwareInfoModRsp

    PMS_MODULE_STATUS_ITEM_DEL_REQ, // ��Ϣ�� CModuleStatusItemDelReq ɾ����ʷ��ģ��״̬��Ŀ
    PMS_MODULE_STATUS_ITEM_DEL_RSP, // ��Ϣ�� CModuleStatusItemDelRsp

    PMS_SYSTEM_DISK_INFO_GET_REQ,     // ��Ϣ�� CSystemDiskInfoGetReq  ƽ̨ϵͳ����Ϣ
    PMS_SYSTEM_DISK_INFO_GET_RSP,     // ��Ϣ�� CSystemDiskInfoGetRsp

    PMS_PROXY_CONFIGINFO_QRY_REQ,           //��Ϣ�� CProxyConfigInfoQryReq  hzytodo ɾ������
    PMS_PROXY_CONFIGINFO_QRY_RSP,           //��Ϣ�� CProxyConfigInfoQryRsp
    PMS_PROXY_CONFIGINFO_MOD_REQ,           //��Ϣ�� CProxyConfigInfoModReq  �������޸Ĵ�ƽ̨��proxy��ַ
    PMS_PROXY_CONFIGINFO_MOD_RSP,           //��Ϣ�� CProxyConfigInfoModRsp
    
    PMS_STUN_SERVER_CONFIGINFO_QRY_REQ,     //��Ϣ�� CSTUNServerConfigInfoQryReq hzytodo ɾ������
    PMS_STUN_SERVER_CONFIGINFO_QRY_RSP,     //��Ϣ�� CSTUNServerConfigInfoQryRsp
    PMS_STUN_SERVER_CONFIGINFO_MOD_REQ,     //��Ϣ�� CSTUNServerConfigInfoModReq
    PMS_STUN_SERVER_CONFIGINFO_MOD_RSP,     //��Ϣ�� CSTUNServerConfigInfoModRsp

    PMS_NTP_SERVER_CONFIGINFO_QRY_REQ,      //��Ϣ�� CNTPServerConfigInfoQryReq  hzytodo ɾ������
    PMS_NTP_SERVER_CONFIGINFO_QRY_RSP,      //��Ϣ�� CNTPServerConfigInfoQryRsp
    PMS_NTP_SERVER_CONFIGINFO_MOD_REQ,      //��Ϣ�� CNTPServerConfigInfoModReq  
    PMS_NTP_SERVER_CONFIGINFO_MOD_RSP,      //��Ϣ�� CNTPServerConfigInfoModRsp

    PMS_CTRL_PMS_REQ,                       // ��Ϣ�� CCtrlPmsReq   PMS����(�����ػ�����������Ϊ����)
    PMS_CTRL_PMS_RSP,                       // ��Ϣ�� CCtrlPmsRsp

    PMS_PLATFORM_TIMEINFO_QRY_REQ,          // ��Ϣ�� CPlatformTimeInfoQryReq ƽ̨ʱ�����
    PMS_PLATFORM_TIMEINFO_QRY_RSP,          // ��Ϣ�� CPlatformTimeInfoQryRsp
    PMS_PLATFORM_TIMEINFO_MOD_REQ,          // ��Ϣ�� CPlatformTimeInfoModReq
    PMS_PLATFORM_TIMEINFO_MOD_RSP,          // ��Ϣ�� CPlatformTimeInfoModRsp

    PMS_NETCARDINFO_QRY_REQ,                // ��Ϣ�� CNetcardInfoQryReq  ������Ϣ
    PMS_NETCARDINFO_QRY_RSP,                // ��Ϣ�� CNetcardInfoQryRsp
    PMS_NETCARDINFO_MOD_REQ,                // ��Ϣ�� CNetcardInfoModReq
    PMS_NETCARDINFO_MOD_RSP,                // ��Ϣ�� CNetcardInfoModRsp

    PMS_NETROUTEINFO_QRY_REQ,               // ��Ϣ�� CNetRouteQryReq ����·��
    PMS_NETROUTEINFO_QRY_RSP,               // ��Ϣ�� CNetRouteQryRsp
    PMS_NETROUTEINFO_QRY_NTF_REQ,           // ��Ϣ�� CNetRouteQryNtfReq
    PMS_NETROUTEINFO_QRY_NTF_RSP,           // ��Ϣ�� CNtfRsp
    PMS_NETROUTEINFO_ADD_REQ,               // ��Ϣ�� CNetRouteAddReq
    PMS_NETROUTEINFO_ADD_RSP,               // ��Ϣ�� CNetRouteAddRsp
    PMS_NETROUTEINFO_DEL_REQ,               // ��Ϣ�� CNetRouteDelReq
    PMS_NETROUTEINFO_DEL_RSP,               // ��Ϣ�� CNetRouteDelRsp
    PMS_NETDEFAULTROUTE_GET_REQ,            // ��Ϣ�� CNetDefaultRouteGetReq
    PMS_NETDEFAULTROUTE_GET_RSP,            // ��Ϣ�� CNetDefaultRouteGetRsp
    PMS_NETDEFAULTROUTE_SET_REQ,            // ��Ϣ�� CNetDefaultRouteSetReq
    PMS_NETDEFAULTROUTE_SET_RSP,            // ��Ϣ�� CNetDefaultRouteSetRsp

    PMS_NETNAT_QRY_REQ,                     // ��Ϣ�� CNetNatQryReq ����NAT
    PMS_NETNAT_QRY_RSP,                     // ��Ϣ�� CNetNatQryRsp
    PMS_NETNAT_QRY_NTF_REQ,                 // ��Ϣ�� CNetNatQryNtfReq
    PMS_NETNAT_QRY_NTF_RSP,                 // ��Ϣ�� CNtfRsp
    PMS_NETNAT_ADD_REQ,                     // ��Ϣ�� CNetNatAddReq
    PMS_NETNAT_ADD_RSP,                     // ��Ϣ�� CNetNatAddRsp
    PMS_NETNAT_DEL_REQ,                     // ��Ϣ�� CNetNatDelReq
    PMS_NETNAT_DEL_RSP,                     // ��Ϣ�� CNetNatDelRsp

    PMS_PLATFORM_TOPO_QRY_REQ,              // ��Ϣ�� CPlatformTOPOQryReq ƽ̨����
    PMS_PLATFORM_TOPO_QRY_RSP,              // ��Ϣ�� CPlatformTOPOQryRsp
    PMS_PLATFORM_TOPO_QRY_NTF_REQ,          // ��Ϣ�� CPlatformTOPOQryNtfReq
    PMS_PLATFORM_TOPO_QRY_NTF_RSP,          // ��Ϣ�� CNtfRsp

    PMS_PLATFORM_TOPO_EXTERNINFO_GET_REQ,   // ��Ϣ�� CPlatformTopoExternInfoGetReq  ��ȡ����ƽ̨��һЩ������Ϣ��Ϊ�˲����û���֪��Щ��Ϣ���ó����Զ���ȡ
    PMS_PLATFORM_TOPO_EXTERNINFO_GET_RSP,   // ��Ϣ�� CPlatformTopoExternInfoGetRsp

    PMS_PLATFORM_TOPO_ADD_REQ,              // ��Ϣ�� CPlatformTOPOAddReq
    PMS_PLATFORM_TOPO_ADD_RSP,              // ��Ϣ�� CPlatformTOPOAddRsp
    PMS_PLATFORM_TOPO_ADD_NTF_REQ,          // ��Ϣ�� CPlatformTOPOAddNtfReq
    PMS_PLATFORM_TOPO_ADD_NTF_RSP,          // ��Ϣ�� CNtfRsp

    PMS_PLATFORM_TOPO_MOD_REQ,              // ��Ϣ�� CPlatformTOPOModReq 
    PMS_PLATFORM_TOPO_MOD_RSP,              // ��Ϣ�� CPlatformTOPOModRsp
    PMS_PLATFORM_TOPO_MOD_NTF_REQ,          // ��Ϣ�� CPlatformTOPOModNtfReq
    PMS_PLATFORM_TOPO_MOD_NTF_RSP,          // ��Ϣ�� CNtfRsp

    PMS_PLATFORM_TOPO_DEL_REQ,              // ��Ϣ�� CPlatformTOPODelReq
    PMS_PLATFORM_TOPO_DEL_RSP,              // ��Ϣ�� CPlatformTOPODelRsp
    PMS_PLATFORM_TOPO_DEL_NTF_REQ,          // ��Ϣ�� CPlatformTOPODelNtfReq
    PMS_PLATFORM_TOPO_DEL_NTF_RSP,          // ��Ϣ�� CNtfRsp

    PMS_PLATFORM_TOPO_SYNC_REQ,             // ��Ϣ�� CPlatformTOPOSyncReq  PMS֮��TOPOͬ������ͬ��IP��ַ��
    PMS_PLATFORM_TOPO_SYNC_RSP,             // ��Ϣ�� CPlatformTOPOSyncRsp

    PMS_PLATFORM_MODULEINFO_QRY_REQ,        // ��Ϣ�� CPlatformModuleInfoQryReq  ƽ̨ģ������
    PMS_PLATFORM_MODULEINFO_QRY_RSP,        // ��Ϣ�� CPlatformModuleInfoQryRsp
    PMS_PLATFORM_MODULEINFO_QRY_NTF_REQ,    // ��Ϣ�� CPlatformModuleInfoQryNtfReq
    PMS_PLATFORM_MODULEINFO_QRY_NTF_RSP,    // ��Ϣ�� CNtfRsp
    PMS_PLATFORM_MODULEINFO_MOD_REQ,        // ��Ϣ�� CPlatformModuleInfoModReq
    PMS_PLATFORM_MODULEINFO_MOD_RSP,        // ��Ϣ�� CPlatformModuleInfoModRsp

    PMS_MIRROR_EXPORT_BEGIN_REQ,            // ��Ϣ�� CMirrorExportBeginReq  ���񵼳����� 
    PMS_MIRROR_EXPORT_BEGIN_RSP,            // ��Ϣ�� CMirrorExportBeginRsp
	PMS_MIRROR_EXPORT_BEGIN_ACK,			// ��Ϣ�� CMirrorExportBeginAckReq
    PMS_MIRROR_EXPORT_END_REQ,              // ��Ϣ�� CMirrorExportEndReq
    PMS_MIRROR_EXPORT_END_RSP,              // ��Ϣ�� CMirrorExportEndRsp
    PMS_MIRROR_EXPORT_PROGRESS_NTF_REQ,     // ��Ϣ�� CMirrorExportProgressNtfReq
    PMS_MIRROR_EXPORT_PROGRESS_NTF_RSP,     // ��Ϣ�� CNtfRsp
    PMS_MIRROR_IMPORT_BEGIN_REQ,            // ��Ϣ�� CMirrorImportBeginReq  ���������
    PMS_MIRROR_IMPORT_BEGIN_RSP,            // ��Ϣ�� CMirrorImportBeginRsp
	PMS_MIRROR_IMPORT_BEGIN_ACK,			// ��Ϣ�� CMirrorImportBeginAckReq
    PMS_MIRROR_IMPORT_END_REQ,              // ��Ϣ�� CMirrorImportEndReq
    PMS_MIRROR_IMPORT_END_RSP,              // ��Ϣ�� CMirrorImportEndRsp
    PMS_MIRROR_IMPORT_PROGRESS_NTF_REQ,     // ��Ϣ�� CMirrorImportProgressNtfReq
    PMS_MIRROR_IMPORT_PROGRESS_NTF_RSP,     // ��Ϣ�� CNtfRsp
    PMS_MIRROR_RESET_REQ,                   // ��Ϣ�� CMirrorResetReq        ����ָ���������
    PMS_MIRROR_RESET_RSP,                   // ��Ϣ�� CMirrorResetRsp
	PMS_MIRROR_RESET_ACK,					// ��Ϣ�� CEventReq
    PMS_MIRROR_RESET_PROGRESS_NTF_REQ,      // ��Ϣ�� CMirrorResetProgressNtfReq
    PMS_MIRROR_RESET_PROGRESS_NTF_RSP,      // ��Ϣ�� CNtfRsp
    PMS_MIRROR_UPDATE_BEGIN_REQ,            // ��Ϣ�� CMirrorUpdateBeginReq  ��������
    PMS_MIRROR_UPDATE_BEGIN_RSP,            // ��Ϣ�� CMirrorUpdateBeginRsp
	PMS_MIRROR_UPDATE_BEGIN_ACK,			// ��Ϣ�� CMirrorUpdateBeginAckReq
    PMS_MIRROR_UPDATE_END_REQ,              // ��Ϣ�� CMirrorUpdateEndReq
    PMS_MIRROR_UPDATE_END_RSP,              // ��Ϣ�� CMirrorUpdateEndRsp
    PMS_MIRROR_UPDATE_PROGRESS_NTF_REQ,     // ��Ϣ�� CMirrorUpdateProgressNtfReq
    PMS_MIRROR_UPDATE_PROGRESS_NTF_RSP,     // ��Ϣ�� CNtfRsp
    
    PMS_PLATFORM_LOG_QRY_REQ,               // ��Ϣ�� CPlatformLogInfoQryReq  ��־����ƽ̨��־��
    PMS_PLATFORM_LOG_QRY_RSP,               // ��Ϣ�� CPlatformLogInfoQryRsp
    PMS_PLATFORM_LOG_QRY_NTF_REQ,           // ��Ϣ�� CPlatformLogInfoNtfReq
    PMS_PLATFORM_LOG_QRY_NTF_RSP,           // ��Ϣ�� CNtfRsp

    PMS_USER_OPERATION_LOG_ADD_REQ,         // ��Ϣ�� CUserOperationLogAddReq ��־�����û���־��
    PMS_USER_OPERATION_LOG_ADD_RSP,         // ��Ϣ�� CUserOperationLogAddRsp
    PMS_USER_OPERATION_LOG_QRY_REQ,         // ��Ϣ�� CUserOperationLogQryReq 
    PMS_USER_OPERATION_LOG_QRY_RSP,         // ��Ϣ�� CUserOperationLogQryRsp
    PMS_USER_OPERATION_LOG_QRY_NTF_REQ,     // ��Ϣ�� CUserOperationLogNtfReq
    PMS_USER_OPERATION_LOG_QRY_NTF_RSP,     // ��Ϣ�� CNtfRsp

    PMS_NMS_DEVICE_QRY_REQ,         // ��Ϣ�� CNMSDeviceQryReq  ���ܹ��� �ֹ��������ڿͻ�����Ϊ
    PMS_NMS_DEVICE_QRY_RSP,         // ��Ϣ�� CNMSDeviceQryRsp
    PMS_NMS_DEVICE_QRY_NTF_REQ,     // ��Ϣ�� CNMSDeviceQryNtfReq
    PMS_NMS_DEVICE_QRY_NTF_RSP,     // ��Ϣ�� CNtfRsp
    PMS_NMS_DEVICE_SYNC_NTF_REQ,    // ��Ϣ�� CNMSDeviceSyncNtfReq (���й������豸���ͬ����������ɾ����Ϊ)
    PMS_NMS_DEVICE_SYNC_NTF_RSP,    // ��Ϣ�� CNtfRsp (���й������豸���ͬ����������ɾ����Ϊ)

    PMS_NMS_ALARM_QRY_REQ,          // ��Ϣ�� CNMSAlarmQryReq
    PMS_NMS_ALARM_QRY_RSP,          // ��Ϣ�� CNMSAlarmQryRsp
    PMS_NMS_ALARM_QRY_NTF_REQ,      // ��Ϣ�� CNMSAlarmQryNtfReq
    PMS_NMS_ALARM_QRY_NTF_RSP,      // ��Ϣ�� CNtfRsp
    PMS_NMS_ALARM_MOD_REQ,          // ��Ϣ�� CNMSAlarmModReq
    PMS_NMS_ALARM_MOD_RSP,          // ��Ϣ�� CNMSAlarmModRsp
    PMS_NMS_ALARM_SYNC_NTF_REQ,     // ��Ϣ�� CNMSAlarmSyncNtfReq  (���й����и澯���ͬ����������ɾ����Ϊ)
    PMS_NMS_ALARM_SYNC_NTF_RSP,     // ��Ϣ�� CNtfRsp  (���й����и澯���ͬ����������ɾ����Ϊ)

    PMS_MAIL_CFG_SET_REQ,           // ��Ϣ�� CMailCfgSetReq �ʼ��������
    PMS_MAIL_CFG_SET_RSP,           // ��Ϣ�� CMailCfgSetRsp
    PMS_MAIL_CFG_QRY_REQ,           // ��Ϣ�� CMailCfgQryReq
    PMS_MAIL_CFG_QRY_RSP,           // ��Ϣ�� CMailCfgQryRsp
    PMS_SEND_TEST_MAIL_REQ,         // ��Ϣ�� CSendTestMailReq
    PMS_SEND_TEST_MAIL_RSP,         // ��Ϣ�� CSendTestMailRsp

	// UPNP�����Ϣ
	PMS_UPNP_MAP_INFO_GET_MODE_REQ, // ��Ϣ�� CUPnPMapInfoGetModeReq
	PMS_UPNP_MAP_INFO_GET_MODE_RSP, // ��Ϣ�� CUPnPMapInfoGetModeRsp
	PMS_UPNP_MAP_INFO_SET_MODE_REQ, // ��Ϣ�� CUPnPMapInfoSetModeReq
	PMS_UPNP_MAP_INFO_SET_MODE_RSP, // ��Ϣ�� CUPnPMapInfoSetModeRsp

	PMS_UPNP_MAP_INFO_QRY_REQ,      // ��Ϣ�� CUPnPMapInfoQryReq
	PMS_UPNP_MAP_INFO_QRY_RSP,      // ��Ϣ�� CUPnPMapInfoQryRsp
	PMS_UPNP_MAP_INFO_QRY_NTF_REQ,  // ��Ϣ�� CUPnPMapInfoQryNtfReq
	PMS_UPNP_MAP_INFO_QRY_NTF_RSP,  // ��Ϣ�� CUPnPMapInfoQryNtfRsp
	PMS_UPNP_MAP_INFO_ADD_REQ,      // ��Ϣ�� CUPnPMapInfoAddReq
	PMS_UPNP_MAP_INFO_ADD_RSP,      // ��Ϣ�� CUPnPMapInfoAddRsp
	PMS_UPNP_MAP_INFO_DEL_REQ,      // ��Ϣ�� CUPnPMapInfoDelReq
	PMS_UPNP_MAP_INFO_DEL_RSP,      // ��Ϣ�� CUPnPMapInfoDelRsp
	PMS_UPNP_MAP_INFO_MOD_REQ,      // ��Ϣ�� CUPnPMapInfoModReq
	PMS_UPNP_MAP_INFO_MOD_RSP,      // ��Ϣ�� CUPnPMapInfoModRsp

	PMS_UPNP_MAP_INFO_SYNC_REQ,     // ��Ϣ�� CUPnPMapInfoSyncReq
	PMS_UPNP_MAP_INFO_SYNC_RSP,     // ��Ϣ�� CUPnPMapInfoSyncRsp

    //�˿�ӳ�������Ϣ
	PMS_PORT_MAP_INFO_QRY_REQ,      // ��Ϣ�� CPortMapInfoQryReq
	PMS_PORT_MAP_INFO_QRY_RSP,      // ��Ϣ�� CPortMapInfoQryRsp
	PMS_PORT_MAP_INFO_QRY_NTF_REQ,  // ��Ϣ�� CPortMapInfoQryNtfReq
	PMS_PORT_MAP_INFO_QRY_NTF_RSP,  // ��Ϣ�� CPortMapInfoQryNtfRsp
	PMS_PORT_MAP_INFO_ADD_REQ,      // ��Ϣ�� CPortMapInfoAddReq
	PMS_PORT_MAP_INFO_ADD_RSP,      // ��Ϣ�� CPortMapInfoAddRsp
	PMS_PORT_MAP_INFO_DEL_REQ,      // ��Ϣ�� CPortMapInfoDelReq
	PMS_PORT_MAP_INFO_DEL_RSP,      // ��Ϣ�� CPortMapInfoDelRsp
	PMS_PORT_MAP_INFO_MOD_REQ,      // ��Ϣ�� CPortMapInfoModReq
	PMS_PORT_MAP_INFO_MOD_RSP,      // ��Ϣ�� CPortMapInfoModRsp



	// DDNS�������
	PMS_DDNS_STATUS_GET_REQ,        // ��Ϣ�� CDdnsStatusGetReq
	PMS_DDNS_STATUS_GET_RSP,        // ��Ϣ�� CDdnsStatusGetRsp
	PMS_DDNS_STATUS_SET_REQ,        // ��Ϣ�� CDdnsStatusSetReq
	PMS_DDNS_STATUS_SET_RSP,        // ��Ϣ�� CDdnsStatusSetRsp

	PMS_MODULE_CONF_GET_REQ,		// ��Ϣ�� CModuleConfGetReq
	PMS_MODULE_CONF_GET_RSP,		// ��Ϣ�� CModuleConfGetRsp

    PMS_DEFENSE_CROSS_STREAM_SET_REQ,   //��Ϣ��CDefenseCrossStreamSetReq
    PMS_DEFENSE_CROSS_STREAM_SET_RSP,   //��Ϣ��CDefenseCrossStreamSetRsp
    PMS_DEFENSE_CROSS_STREAM_GET_REQ,   //��Ϣ��CDefenseCrossStreamGetReq
    PMS_DEFENSE_CROSS_STREAM_GET_RSP,   //��Ϣ��CDefenseCrossStreamGetRsp

	PMS_QRCODE_IMG_UPDATE_REQ,        //��Ϣ�� CPmsQrcodeImgUpdateReq
	PMS_QRCODE_IMG_UPDATE_RSP,        //��Ϣ�� CPmsQrcodeImgUpdateRsp
    PMS_SECUREMAIL_INFO_GET_REQ,             //��Ϣ��CPmsSecuremailInfoGetReq
    PMS_SECUREMAIL_INFO_GET_RSP,             //��Ϣ��CPmsSecuremailInfoGetRsp
    PMS_RESET_CU_PASSWORD_REQ,              //��Ϣ��CPmsResetCuPasswordReq
    PMS_RESET_CU_PASSWORD_RSP,              //��Ϣ��CPmsResetCuPasswordRsp
    PMS_PMC_PASSWORD_RESET_INFO_GET_REQ, //��Ϣ��CPmsPmcPasswordResetInfoGetReq
    PMS_PMC_PASSWORD_RESET_INFO_GET_RSP, //��Ϣ��CPmsPmcPasswordResetInfoGetRsp
    PMS_PMC_PASSWORD_RESET_REQ,        //��Ϣ��CPmsPmcPasswordResetReq 
    PMS_PMC_PASSWORD_RESET_RSP,        //��Ϣ��CPmsPmcPasswordResetRsp

	PMS_IPTABLES_SET_REQ,             //��Ϣ��CPmsIptablesSetReq 
	PMS_IPTABLES_SET_RSP,             //��Ϣ��CPmsIptablesSetRsp 
	PMS_IPTABLES_GET_REQ,             //��Ϣ��CPmsIptablesGetReq
	PMS_IPTABLES_GET_RSP,             //��Ϣ��CPmsIptablesGetRsp
	PMS_HEARTBEAT_BAKPC_CONF_REQ,     //��Ϣ��CPmsHeartbeatBakpcConfReq
	PMS_HEARTBEAT_BAKPC_CONF_RSP,     //��Ϣ��CPmsHeartbeatBakpcConfRsp
	PMS_DEFAULT_MIRROR_MAKE_ACK,      //��Ϣ��CPmsDefaultMirrorMakeAck
	PMS_HEARTBEAT_HOST_CONF_REQ,      //��Ϣ��CPmsHeartbeatHostConfReq
	PMS_HEARTBEAT_HOST_CONF_RSP,      //��Ϣ��CPmsHeartbeatHostConfRsp
	PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_REQ,  //��Ϣ��CPmsDefaultMirrorMakeProgressNtfReq
	PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_RSP,  //��Ϣ��CPmsDefaultMirrorMakeProgressNtfRsp
	PMS_DEFAULT_MIRROR_MAKE_END_REQ,  //��Ϣ��CPmsDefaultMirrorMakeEndReq
	PMS_DEFAULT_MIRROR_MAKE_END_RSP,  //��Ϣ��CPmsDefaultMirrorMakeEndRsp
	PMS_TCPDUMP_START_REQ,             //��Ϣ��CPmsTcpdumpStartReq
	PMS_TCPDUMP_START_RSP,             //��Ϣ��CPmsTcpdumpStartRsp
	PMS_TCPDUMP_BEGIN_ACK,             //��Ϣ��CTcpdumpBeginAckReq
	PMS_TCPDUMP_STOP_REQ,              //��Ϣ��CPmsTcpdumpStopReq
	PMS_TCPDUMP_STOP_RSP,              //��Ϣ��CPmsTcpdumpStopRsp
	PMS_TCPDUMP_STOP_NTF_REQ,         //��Ϣ��CTcpdumpStopNtfReq
	PMS_TCPDUMP_STOP_NTF_RSP,         //��Ϣ��CTcpdumpStopNtfRsp
	PMS_TCPDUMP_DOWNLOAD_REQ,          //��Ϣ��CPmsTcpdumpDownloadReq
	PMS_TCPDUMP_DOWNLOAD_RSP,          //��Ϣ��CPmsTcpdumpDownloadRsp
	PMS_MIRROR_BAK_CONF_SET_REQ,       //��Ϣ��CMirrorBakConfSetReq
	PMS_MIRROR_BAK_CONF_SET_RSP,       //��Ϣ��CMirrorBakConfSetRsp
	PMS_MIRROR_BAK_CONF_GET_REQ,       //��Ϣ��CMirrorBakConfGetReq
	PMS_MIRROR_BAK_CONF_GET_RSP,       //��Ϣ��CMirrorBakConfGetRsp
	PMS_MIRROR_BAK_FILE_LIST_GET_REQ,  //��Ϣ��CMirrorBakFileListGetReq
	PMS_MIRROR_BAK_FILE_LIST_GET_RSP,  //��Ϣ��CMirrorBakFileListGetRsp
	PMS_HEARTBEAT_CONF_GET_REQ,        //��Ϣ��CPmsHeartbeatConfGetReq
	PMS_HEARTBEAT_CONF_GET_RSP,        //��Ϣ��CPmsHeartbeatConfGetRsp
	PMS_NRU_INFO_QRY_REQ,              //��Ϣ��CNruInfoQryReq
	PMS_NRU_INFO_QRY_RSP,			   //��Ϣ��CNruInfoQryRsp
	PMS_NRU_INFO_UPDATE_REQ,           //��Ϣ��CNruInfoUpdateReq
	PMS_NRU_INFO_UPDATE_RSP,           //��Ϣ��CNruInfoUpdateRsp
	

	PMS_CURRENT_EVENT_END,
    //////////////////////////////////////////////////////////////////////////////
    
};

inline void InitPmcPmsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(PMS_USER_QRY_REQ);                        // ��Ϣ�� CPmsUserQryReq PMS�û�����
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_RSP);                        // ��Ϣ�� CPmsUserQryRsp
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_NTF_REQ);                    // ��Ϣ�� CPmsUserQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_USER_QRY_NTF_RSP);                    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_USER_ADD_REQ);                        // ��Ϣ�� CPmsUserAddReq
    OSP_ADD_EVENT_DESC(PMS_USER_ADD_RSP);                        // ��Ϣ�� CPmsUserAddRsp
    OSP_ADD_EVENT_DESC(PMS_USER_DEL_REQ);                        // ��Ϣ�� CPmsUserDelReq
    OSP_ADD_EVENT_DESC(PMS_USER_DEL_RSP);                        // ��Ϣ�� CPmsUserDelRsp
    OSP_ADD_EVENT_DESC(PMS_USER_MOD_REQ);                        // ��Ϣ�� CPmsUserModReq
    OSP_ADD_EVENT_DESC(PMS_USER_MOD_RSP);                        // ��Ϣ�� CPmsUserModRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_HARDWAREINFO_QRY_REQ);       // ��Ϣ�� CPlatformHardwareInfoQryReq ƽ̨Ӳ����Ϣ
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_HARDWAREINFO_QRY_RSP);       // ��Ϣ�� CPlatformHardwareInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_QRY_REQ);       // ��Ϣ�� CPlatformSoftwareInfoQryReq ƽ̨�����Ϣ
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_QRY_RSP);       // ��Ϣ�� CPlatformSoftwareInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_MOD_REQ);       // ��Ϣ�� CPlatformSoftwareInfoModReq �޸ģ�������proxy���á�stun���á�http�˿ڣ���һ��ֻ�����޸�һ��
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_SOFTWAREINFO_MOD_RSP);       // ��Ϣ�� CPlatformSoftwareInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_MODULE_STATUS_ITEM_DEL_REQ);          // ��Ϣ�� CModuleStatusItemDelReq ɾ����ʷ��ģ��״̬��Ŀ
    OSP_ADD_EVENT_DESC(PMS_MODULE_STATUS_ITEM_DEL_RSP);          // ��Ϣ�� CModuleStatusItemDelRsp
    OSP_ADD_EVENT_DESC(PMS_SYSTEM_DISK_INFO_GET_REQ);            // ��Ϣ�� CSystemDiskInfoGetReq  ƽ̨ϵͳ����Ϣ
    OSP_ADD_EVENT_DESC(PMS_SYSTEM_DISK_INFO_GET_RSP);            // ��Ϣ�� CSystemDiskInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_QRY_REQ);            //��Ϣ�� CProxyConfigInfoQryReq  hzytodo ɾ������
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_QRY_RSP);            //��Ϣ�� CProxyConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_MOD_REQ);            //��Ϣ�� CProxyConfigInfoModReq  �������޸Ĵ�ƽ̨��proxy��ַ
    OSP_ADD_EVENT_DESC(PMS_PROXY_CONFIGINFO_MOD_RSP);            //��Ϣ�� CProxyConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_QRY_REQ);      //��Ϣ�� CSTUNServerConfigInfoQryReq hzytodo ɾ������
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_QRY_RSP);      //��Ϣ�� CSTUNServerConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_MOD_REQ);      //��Ϣ�� CSTUNServerConfigInfoModReq
    OSP_ADD_EVENT_DESC(PMS_STUN_SERVER_CONFIGINFO_MOD_RSP);      //��Ϣ�� CSTUNServerConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_QRY_REQ);       //��Ϣ�� CNTPServerConfigInfoQryReq  hzytodo ɾ������
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_QRY_RSP);       //��Ϣ�� CNTPServerConfigInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_MOD_REQ);       //��Ϣ�� CNTPServerConfigInfoModReq  
    OSP_ADD_EVENT_DESC(PMS_NTP_SERVER_CONFIGINFO_MOD_RSP);       //��Ϣ�� CNTPServerConfigInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_CTRL_PMS_REQ);                        // ��Ϣ�� CCtrlPmsReq   PMS����(�����ػ�����������Ϊ����)
    OSP_ADD_EVENT_DESC(PMS_CTRL_PMS_RSP);                        // ��Ϣ�� CCtrlPmsRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_QRY_REQ);           // ��Ϣ�� CPlatformTimeInfoQryReq ƽ̨ʱ�����
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_QRY_RSP);           // ��Ϣ�� CPlatformTimeInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_MOD_REQ);           // ��Ϣ�� CPlatformTimeInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TIMEINFO_MOD_RSP);           // ��Ϣ�� CPlatformTimeInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_QRY_REQ);                 // ��Ϣ�� CNetcardInfoQryReq  ������Ϣ
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_QRY_RSP);                 // ��Ϣ�� CNetcardInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_MOD_REQ);                 // ��Ϣ�� CNetcardInfoModReq
    OSP_ADD_EVENT_DESC(PMS_NETCARDINFO_MOD_RSP);                 // ��Ϣ�� CNetcardInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_REQ);                // ��Ϣ�� CNetRouteQryReq ����·��
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_RSP);                // ��Ϣ�� CNetRouteQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_NTF_REQ);            // ��Ϣ�� CNetRouteQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_ADD_REQ);                // ��Ϣ�� CNetRouteAddReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_ADD_RSP);                // ��Ϣ�� CNetRouteAddRsp
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_DEL_REQ);                // ��Ϣ�� CNetRouteDelReq
    OSP_ADD_EVENT_DESC(PMS_NETROUTEINFO_DEL_RSP);                // ��Ϣ�� CNetRouteDelRsp
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_GET_REQ);             // ��Ϣ�� CNetDefaultRouteGetReq
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_GET_RSP);             // ��Ϣ�� CNetDefaultRouteGetRsp
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_SET_REQ);             // ��Ϣ�� CNetDefaultRouteSetReq
    OSP_ADD_EVENT_DESC(PMS_NETDEFAULTROUTE_SET_RSP);             // ��Ϣ�� CNetDefaultRouteSetRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_REQ);                      // ��Ϣ�� CNetNatQryReq ����NAT
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_RSP);                      // ��Ϣ�� CNetNatQryRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_NTF_REQ);                  // ��Ϣ�� CNetNatQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_QRY_NTF_RSP);                  // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_ADD_REQ);                      // ��Ϣ�� CNetNatAddReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_ADD_RSP);                      // ��Ϣ�� CNetNatAddRsp
    OSP_ADD_EVENT_DESC(PMS_NETNAT_DEL_REQ);                      // ��Ϣ�� CNetNatDelReq
    OSP_ADD_EVENT_DESC(PMS_NETNAT_DEL_RSP);                      // ��Ϣ�� CNetNatDelRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_REQ);               // ��Ϣ�� CPlatformTOPOQryReq ƽ̨����
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_RSP);               // ��Ϣ�� CPlatformTOPOQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_NTF_REQ);           // ��Ϣ�� CPlatformTOPOQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_QRY_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_EXTERNINFO_GET_REQ);    // ��Ϣ�� CPlatformTopoExternInfoGetReq  ��ȡ����ƽ̨��һЩ������Ϣ��Ϊ�˲����û���֪��Щ��Ϣ���ó����Զ���ȡ
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_EXTERNINFO_GET_RSP);    // ��Ϣ�� CPlatformTopoExternInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_REQ);               // ��Ϣ�� CPlatformTOPOAddReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_RSP);               // ��Ϣ�� CPlatformTOPOAddRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_NTF_REQ);           // ��Ϣ�� CPlatformTOPOAddNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_ADD_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_REQ);               // ��Ϣ�� CPlatformTOPOModReq 
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_RSP);               // ��Ϣ�� CPlatformTOPOModRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_NTF_REQ);           // ��Ϣ�� CPlatformTOPOModNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_MOD_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_REQ);               // ��Ϣ�� CPlatformTOPODelReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_RSP);               // ��Ϣ�� CPlatformTOPODelRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_NTF_REQ);           // ��Ϣ�� CPlatformTOPODelNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_DEL_NTF_RSP);           // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_SYNC_REQ);              // ��Ϣ�� CPlatformTOPOSyncReq  PMS֮��TOPOͬ������ͬ��IP��ַ��
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_TOPO_SYNC_RSP);              // ��Ϣ�� CPlatformTOPOSyncRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_REQ);         // ��Ϣ�� CPlatformModuleInfoQryReq  ƽ̨ģ������
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_RSP);         // ��Ϣ�� CPlatformModuleInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_NTF_REQ);     // ��Ϣ�� CPlatformModuleInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_QRY_NTF_RSP);     // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_MOD_REQ);         // ��Ϣ�� CPlatformModuleInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_MODULEINFO_MOD_RSP);         // ��Ϣ�� CPlatformModuleInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_REQ);             // ��Ϣ�� CMirrorExportBeginReq  ���񵼳����� 
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_RSP);             // ��Ϣ�� CMirrorExportBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_BEGIN_ACK);             // ��Ϣ�� CMirrorExportBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_END_REQ);               // ��Ϣ�� CMirrorExportEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_END_RSP);               // ��Ϣ�� CMirrorExportEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_PROGRESS_NTF_REQ);      // ��Ϣ�� CMirrorExportProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_EXPORT_PROGRESS_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_REQ);             // ��Ϣ�� CMirrorImportBeginReq  ���������
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_RSP);             // ��Ϣ�� CMirrorImportBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_BEGIN_ACK);             // ��Ϣ�� CMirrorImportBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_END_REQ);               // ��Ϣ�� CMirrorImportEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_END_RSP);               // ��Ϣ�� CMirrorImportEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_PROGRESS_NTF_REQ);      // ��Ϣ�� CMirrorImportProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_IMPORT_PROGRESS_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_REQ);                    // ��Ϣ�� CMirrorResetReq        ����ָ���������
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_RSP);                    // ��Ϣ�� CMirrorResetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_ACK);                    // ��Ϣ�� CEventReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_PROGRESS_NTF_REQ);       // ��Ϣ�� CMirrorResetProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_RESET_PROGRESS_NTF_RSP);       // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_REQ);             // ��Ϣ�� CMirrorUpdateBeginReq  ��������
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_RSP);             // ��Ϣ�� CMirrorUpdateBeginRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_BEGIN_ACK);             // ��Ϣ�� CMirrorUpdateBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_END_REQ);               // ��Ϣ�� CMirrorUpdateEndReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_END_RSP);               // ��Ϣ�� CMirrorUpdateEndRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_PROGRESS_NTF_REQ);      // ��Ϣ�� CMirrorUpdateProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_UPDATE_PROGRESS_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_REQ);                // ��Ϣ�� CPlatformLogInfoQryReq  ��־����ƽ̨��־��
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_RSP);                // ��Ϣ�� CPlatformLogInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_NTF_REQ);            // ��Ϣ�� CPlatformLogInfoNtfReq
    OSP_ADD_EVENT_DESC(PMS_PLATFORM_LOG_QRY_NTF_RSP);            // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_ADD_REQ);          // ��Ϣ�� CUserOperationLogAddReq ��־�����û���־��
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_ADD_RSP);          // ��Ϣ�� CUserOperationLogAddRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_REQ);          // ��Ϣ�� CUserOperationLogQryReq 
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_RSP);          // ��Ϣ�� CUserOperationLogQryRsp
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_NTF_REQ);      // ��Ϣ�� CUserOperationLogNtfReq
    OSP_ADD_EVENT_DESC(PMS_USER_OPERATION_LOG_QRY_NTF_RSP);      // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_REQ);                  // ��Ϣ�� CNMSDeviceQryReq  ���ܹ��� �ֹ��������ڿͻ�����Ϊ
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_RSP);                  // ��Ϣ�� CNMSDeviceQryRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_NTF_REQ);              // ��Ϣ�� CNMSDeviceQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_QRY_NTF_RSP);              // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_SYNC_NTF_REQ);             // ��Ϣ�� CNMSDeviceSyncNtfReq (���й������豸���ͬ����������ɾ����Ϊ)
    OSP_ADD_EVENT_DESC(PMS_NMS_DEVICE_SYNC_NTF_RSP);             // ��Ϣ�� CNtfRsp (���й������豸���ͬ����������ɾ����Ϊ)
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_REQ);                   // ��Ϣ�� CNMSAlarmQryReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_RSP);                   // ��Ϣ�� CNMSAlarmQryRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_NTF_REQ);               // ��Ϣ�� CNMSAlarmQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_QRY_NTF_RSP);               // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_MOD_REQ);                   // ��Ϣ�� CNMSAlarmModReq
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_MOD_RSP);                   // ��Ϣ�� CNMSAlarmModRsp
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_SYNC_NTF_REQ);              // ��Ϣ�� CNMSAlarmSyncNtfReq  (���й����и澯���ͬ����������ɾ����Ϊ)
    OSP_ADD_EVENT_DESC(PMS_NMS_ALARM_SYNC_NTF_RSP);              // ��Ϣ�� CNtfRsp  (���й����и澯���ͬ����������ɾ����Ϊ)
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_SET_REQ);                    // ��Ϣ�� CMailCfgSetReq �ʼ��������
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_SET_RSP);                    // ��Ϣ�� CMailCfgSetRsp
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_QRY_REQ);                    // ��Ϣ�� CMailCfgQryReq
    OSP_ADD_EVENT_DESC(PMS_MAIL_CFG_QRY_RSP);                    // ��Ϣ�� CMailCfgQryRsp
    OSP_ADD_EVENT_DESC(PMS_SEND_TEST_MAIL_REQ);                  // ��Ϣ�� CSendTestMailReq
    OSP_ADD_EVENT_DESC(PMS_SEND_TEST_MAIL_RSP);                  // ��Ϣ�� CSendTestMailRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_GET_MODE_REQ);          // ��Ϣ�� CUPnPMapInfoGetModeReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_GET_MODE_RSP);          // ��Ϣ�� CUPnPMapInfoGetModeRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SET_MODE_REQ);          // ��Ϣ�� CUPnPMapInfoSetModeReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SET_MODE_RSP);          // ��Ϣ�� CUPnPMapInfoSetModeRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_REQ);               // ��Ϣ�� CUPnPMapInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_RSP);               // ��Ϣ�� CUPnPMapInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_NTF_REQ);           // ��Ϣ�� CUPnPMapInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_QRY_NTF_RSP);           // ��Ϣ�� CUPnPMapInfoQryNtfRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_ADD_REQ);               // ��Ϣ�� CUPnPMapInfoAddReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_ADD_RSP);               // ��Ϣ�� CUPnPMapInfoAddRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_DEL_REQ);               // ��Ϣ�� CUPnPMapInfoDelReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_DEL_RSP);               // ��Ϣ�� CUPnPMapInfoDelRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_MOD_REQ);               // ��Ϣ�� CUPnPMapInfoModReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_MOD_RSP);               // ��Ϣ�� CUPnPMapInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SYNC_REQ);              // ��Ϣ�� CUPnPMapInfoSyncReq
    OSP_ADD_EVENT_DESC(PMS_UPNP_MAP_INFO_SYNC_RSP);              // ��Ϣ�� CUPnPMapInfoSyncRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_REQ);               // ��Ϣ�� CPortMapInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_RSP);               // ��Ϣ�� CPortMapInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_NTF_REQ);           // ��Ϣ�� CPortMapInfoQryNtfReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_QRY_NTF_RSP);           // ��Ϣ�� CPortMapInfoQryNtfRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_ADD_REQ);               // ��Ϣ�� CPortMapInfoAddReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_ADD_RSP);               // ��Ϣ�� CPortMapInfoAddRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_DEL_REQ);               // ��Ϣ�� CPortMapInfoDelReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_DEL_RSP);               // ��Ϣ�� CPortMapInfoDelRsp
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_MOD_REQ);               // ��Ϣ�� CPortMapInfoModReq
    OSP_ADD_EVENT_DESC(PMS_PORT_MAP_INFO_MOD_RSP);               // ��Ϣ�� CPortMapInfoModRsp
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_GET_REQ);                 // ��Ϣ�� CDdnsStatusGetReq
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_GET_RSP);                 // ��Ϣ�� CDdnsStatusGetRsp
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_SET_REQ);                 // ��Ϣ�� CDdnsStatusSetReq
    OSP_ADD_EVENT_DESC(PMS_DDNS_STATUS_SET_RSP);                 // ��Ϣ�� CDdnsStatusSetRsp
    OSP_ADD_EVENT_DESC(PMS_MODULE_CONF_GET_REQ);                 // ��Ϣ�� CModuleConfGetReq
    OSP_ADD_EVENT_DESC(PMS_MODULE_CONF_GET_RSP);                 // ��Ϣ�� CModuleConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_SET_REQ);        //��Ϣ��CDefenseCrossStreamSetReq
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_SET_RSP);        //��Ϣ��CDefenseCrossStreamSetRsp
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_GET_REQ);        //��Ϣ��CDefenseCrossStreamGetReq
    OSP_ADD_EVENT_DESC(PMS_DEFENSE_CROSS_STREAM_GET_RSP);        //��Ϣ��CDefenseCrossStreamGetRsp
    OSP_ADD_EVENT_DESC(PMS_QRCODE_IMG_UPDATE_REQ);               //��Ϣ�� CPmsQrcodeImgUpdateReq
    OSP_ADD_EVENT_DESC(PMS_QRCODE_IMG_UPDATE_RSP);               //��Ϣ�� CPmsQrcodeImgUpdateRsp
    OSP_ADD_EVENT_DESC(PMS_SECUREMAIL_INFO_GET_REQ);             //��Ϣ��CPmsSecuremailInfoGetReq
    OSP_ADD_EVENT_DESC(PMS_SECUREMAIL_INFO_GET_RSP);             //��Ϣ��CPmsSecuremailInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_RESET_CU_PASSWORD_REQ);               //��Ϣ��CPmsResetCuPasswordReq
    OSP_ADD_EVENT_DESC(PMS_RESET_CU_PASSWORD_RSP);               //��Ϣ��CPmsResetCuPasswordRsp
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_INFO_GET_REQ);     //��Ϣ��CPmsPmcPasswordResetInfoGetReq
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_INFO_GET_RSP);     //��Ϣ��CPmsPmcPasswordResetInfoGetRsp
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_REQ);              //��Ϣ��CPmsPmcPasswordResetReq 
    OSP_ADD_EVENT_DESC(PMS_PMC_PASSWORD_RESET_RSP);              //��Ϣ��CPmsPmcPasswordResetRsp
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_SET_REQ);                    //��Ϣ��CPmsIptablesSetReq 
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_SET_RSP);                    //��Ϣ��CPmsIptablesSetRsp 
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_GET_REQ);                    //��Ϣ��CPmsIptablesGetReq
    OSP_ADD_EVENT_DESC(PMS_IPTABLES_GET_RSP);                    //��Ϣ��CPmsIptablesGetRsp
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_BAKPC_CONF_REQ);            //��Ϣ��CPmsHeartbeatBakpcConfReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_BAKPC_CONF_RSP);            //��Ϣ��CPmsHeartbeatBakpcConfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_ACK);             //��Ϣ��CPmsDefaultMirrorMakeAck
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_HOST_CONF_REQ);             //��Ϣ��CPmsHeartbeatHostConfReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_HOST_CONF_RSP);             //��Ϣ��CPmsHeartbeatHostConfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_REQ);//��Ϣ��CPmsDefaultMirrorMakeProgressNtfReq
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_PROGRESS_NTF_RSP);//��Ϣ��CPmsDefaultMirrorMakeProgressNtfRsp
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_END_REQ);         //��Ϣ��CPmsDefaultMirrorMakeEndReq
    OSP_ADD_EVENT_DESC(PMS_DEFAULT_MIRROR_MAKE_END_RSP);         //��Ϣ��CPmsDefaultMirrorMakeEndRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_START_REQ);                   //��Ϣ��CPmsTcpdumpStartReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_START_RSP);                   //��Ϣ��CPmsTcpdumpStartRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_BEGIN_ACK);                   //��Ϣ��CTcpdumpBeginAckReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_REQ);                    //��Ϣ��CPmsTcpdumpStopReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_RSP);                    //��Ϣ��CPmsTcpdumpStopRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_NTF_REQ);                //��Ϣ��CTcpdumpStopNtfReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_STOP_NTF_RSP);                //��Ϣ��CTcpdumpStopNtfRsp
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_DOWNLOAD_REQ);                //��Ϣ��CPmsTcpdumpDownloadReq
    OSP_ADD_EVENT_DESC(PMS_TCPDUMP_DOWNLOAD_RSP);                //��Ϣ��CPmsTcpdumpDownloadRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_SET_REQ);             //��Ϣ��CMirrorBakConfSetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_SET_RSP);             //��Ϣ��CMirrorBakConfSetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_GET_REQ);             //��Ϣ��CMirrorBakConfGetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_CONF_GET_RSP);             //��Ϣ��CMirrorBakConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_FILE_LIST_GET_REQ);        //��Ϣ��CMirrorBakFileListGetReq
    OSP_ADD_EVENT_DESC(PMS_MIRROR_BAK_FILE_LIST_GET_RSP);        //��Ϣ��CMirrorBakFileListGetRsp
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_CONF_GET_REQ);              //��Ϣ��CPmsHeartbeatConfGetReq
    OSP_ADD_EVENT_DESC(PMS_HEARTBEAT_CONF_GET_RSP);              //��Ϣ��CPmsHeartbeatConfGetRsp
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_QRY_REQ);                    //��Ϣ��CNruInfoQryReq
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_QRY_RSP);                    //��Ϣ��CNruInfoQryRsp
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_UPDATE_REQ);                 //��Ϣ��CNruInfoUpdateReq
    OSP_ADD_EVENT_DESC(PMS_NRU_INFO_UPDATE_RSP);                 //��Ϣ��CNruInfoUpdateRsp
    OSP_ADD_EVENT_DESC(PMS_CURRENT_EVENT_END);                   
}

#endif // _PMS_EVENT_H_







































