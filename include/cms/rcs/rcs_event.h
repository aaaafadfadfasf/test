/*****************************************************************************
   模块名      : rcs_event
   文件名      : rcs_event.h
   相关文件    : 
   文件实现功能: rcs_event.h 定义了所有RCS的消息ID
   作者        : 
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人              修改内容
   2012/06/11   1.0        huangzhichun       创建
******************************************************************************/
#ifndef __RCS_EVENT_H__
#define __RCS_EVENT_H__

#include "../cms_eventid.h"
#include "../cms_proto.h"

/////////////////////////////////////////////////////////////////////
//RCS接口消息
/////////////////////////////////////////////////////////////////////
enum ERcsMsg
{
    //录像系统参数，如覆盖策略
    RCS_SET_SYSPARAM_REQ = RCS_EVENT_BEGIN, //消息体: CRcsSetSysParamReq
    RCS_SET_SYSPARAM_RSP,               //消息体: CRcsSetSysParamRsp
    RCS_GET_SYSPARAM_REQ,               //消息体: CRcsGetSysParamReq
    RCS_GET_SYSPARAM_RSP,               //消息体: CRcsGetSysParamRsp

    //PuChn磁盘绑定
    RCS_BINDING_PUCHN_REQ,              //消息体: CRcsBindingPuChnReq
    RCS_BINDING_PUCHN_RSP,              //消息体: CRcsBindingPuChnRsp
    RCS_UNBINDING_PUCHN_REQ,            //消息体: CRcsUnbindingPuChnReq
    RCS_UNBINDING_PUCHN_RSP,            //消息体: CRcsUnbindingPuChnRsp

    //磁盘绑定查询，暂不支持分页
    RCS_QUERY_PUCHN_BINDING_REQ,        //消息体: CRcsQueryPuChnBindingReq
    RCS_QUERY_PUCHN_BINDING_RSP,        //消息体: CRcsQueryPuChnBindingRsp
    RCS_QUERY_PUCHN_BINDING_NTF_REQ,    //消息体: CRcsQueryPuChnBindingNtfReq
    RCS_QUERY_PUCHN_BINDING_NTF_RSP,    //消息体: CRcsQueryPuChnBindingNtfRsp

    //录像参数，暂不支持分页查询
    RCS_SET_RECORD_PARAM_REQ,            //消息体: CRcsSetRecordParamReq
    RCS_SET_RECORD_PARAM_RSP,            //消息体: CRcsSetRecordParamRsp
    RCS_QUERY_RECORD_PARAM_REQ,          //消息体: CRcsQueryRecordParamReq
    RCS_QUERY_RECORD_PARAM_RSP,          //消息体: CRcsQueryRecordParamRsp
    RCS_QUERY_RECORD_PARAM_NTF_REQ,      //消息体: CRcsQueryRecordParamNtfReq
    RCS_QUERY_RECORD_PARAM_NTF_RSP,      //消息体: CRcsQueryRecordParamNtfRsp

    //设置录像任务(主要是手动、告警)
    RCS_SET_RECORD_TASK_REQ,            //消息体: CRcsSetRecordTaskReq
    RCS_SET_RECORD_TASK_RSP,            //消息体: CRcsSetRecordTaskRsp

	//录像锁定/解锁
	RCS_LOCK_RECORD_REQ,                //消息体: CRcsLockRecordReq
	RCS_LOCK_RECORD_RSP,                //消息体: CRcsLockRecordRsp

    //录像记录查询，暂不支持分页
    RCS_QUERY_RECORD_REQ,               //消息体: CRcsQueryRecordReq
    RCS_QUERY_RECORD_RSP,               //消息体: CRcsQueryRecordRsp
    RCS_QUERY_RECORD_NTF_REQ,           //消息体: CRcsQueryRecordNtfReq
    RCS_QUERY_RECORD_NTF_RSP,           //消息体: CRcsQueryRecordNtfRsp

    //录像记录删除
    RCS_DELETE_RECORD_REQ,              //消息体：CRcsDeleteRecordReq
    RCS_DELETE_RECORD_RSP,              //消息体：CRcsDeleteRecordRsp

	//录像删除（按通道+时间段）
	RCS_REC_DEL_REQ,			        //消息体: CRcsRecDelReq
	RCS_REC_DEL_RSP,			        //消息体: CRcsRecDelRsp

    //录像日历查询
    RCS_QUERY_RECORD_DAYS_REQ,          //消息体: CRcsQueryRecordDaysReq
    RCS_QUERY_RECORD_DAYS_RSP,          //消息体: CRcsQueryRecordDaysRsp

	//设置录像别名
	RCS_SET_RECORD_ALIAS_REQ,			//消息体：CRcsSetRecordAliasReq
	RCS_SET_RECORD_ALIAS_RSP,			//消息体：CRcsSetRecordAliasRsp

    //录像下载，用Invite
    //录像播放，用Invite

    //录像标签
    RCS_ADD_RECTAG_REQ,                 //消息体: CRcsAddRecTagReq
    RCS_ADD_RECTAG_RSP,                 //消息体: CRcsAddRecTagRsp
    RCS_DEL_RECTAG_REQ,                 //消息体: CRcsDelRecTagReq
    RCS_DEL_RECTAG_RSP,                 //消息体: CRcsDelRecTagRsp
    RCS_UPDATE_RECTAG_REQ,              //消息体: CRcsUpdateRecTagReq
    RCS_UPDATE_RECTAG_RSP,              //消息体: CRcsUpdateRecTagRsp
    RCS_QUERY_RECTAG_REQ,               //消息体: CRcsQueryRecTagReq
    RCS_QUERY_RECTAG_RSP,               //消息体: CRcsQueryRecTagRsp
    RCS_QUERY_RECTAG_NTF_REQ,           //消息体: CRcsQueryRecTagNtfReq
    RCS_QUERY_RECTAG_NTF_RSP,           //消息体: CRcsQueryRecTagNtfRsp

    //PU录像状态订阅
    RCS_SUBSCRIBE_PU_STATUS_REQ,        //消息体: CRcsSubscribePuStatusReq
    RCS_SUBSCRIBE_PU_STATUS_RSP,        //消息体: CRcsSubscribePuStatusRsp

    RCS_UNSUBSCRIBE_PU_STATUS_REQ,      //消息体: CRcsUnsubscribePuStatusReq
    RCS_UNSUBSCRIBE_PU_STATUS_RSP,      //消息体: CRcsUnsubscribePuStatusRsp

    RCS_PU_RECORD_STATUS_NTF_REQ,       //消息体: CRcsPuRecordStatusNtyReq
    RCS_PU_RECORD_STATUS_NTF_RSP,       //消息体: CRcsPuRecordStatusNtyRsp

	//PU录像状态列表订阅
	RCS_LIST_SUBSCRIBE_PU_STATUS_REQ,        //消息体: CRcsListSubscribePuStatusReq
	RCS_LIST_SUBSCRIBE_PU_STATUS_RSP,        //消息体: CRcsListSubscribePuStatusRsp
	RCS_LIST_REFRESH_SS_PU_STATUS_REQ,       //消息体: CRcsListRefreshSsPuStatusReq
	RCS_LIST_REFRESH_SS_PU_STATUS_RSP,       //消息体: CRcsListRefreshSsPuStatusRsp
	RCS_LIST_UNSUBSCRIBE_PU_STATUS_REQ,      //消息体: CRcsListUnsubscribePuStatusReq
	RCS_LIST_UNSUBSCRIBE_PU_STATUS_RSP,      //消息体: CRcsListUnsubscribePuStatusRsp

	//PU录像状态目录订阅
	RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ,        //消息体: CRcsCatalogSubscribePuStatusReq
	RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP,        //消息体: CRcsCatalogSubscribePuStatusRsp
	RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ,       //消息体: CRcsCatalogRefreshSsPuStatusReq
	RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP,       //消息体: CRcsCatalogRefreshSsPuStatusRsp
	RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ,      //消息体: CRcsCatalogUnsubscribePuStatusReq
	RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP,      //消息体: CRcsCatalogUnsubscribePuStatusRsp

	//录像锁定/解锁
	RCS_RECORD_LOCK_OPR_REQ,			//消息体: CRcsRecordLockOprReq
	RCS_RECORD_LOCK_OPR_RSP,			//消息体: CRcsRecordLockOprRsp


    //////////////////////////////////////////////////////////////////////
    //NRU管理

	//搜索发现,订阅消息
	RCS_START_DISCOVERY_NRU_REQ,            //消息体: CRcsStartDiscoveryNruReq
    RCS_START_DISCOVERY_NRU_RSP,            //消息体: CRcsStartDiscoveryNruRsp
	RCS_STOP_DISCOVERY_NRU_REQ,             //消息体: CRcsStopDiscoveryNruReq
    RCS_STOP_DISCOVERY_NRU_RSP,             //消息体: CRcsStopDiscoveryNruRsp
	RCS_DISCOVERY_NRU_NTF_REQ,              //消息体: CRcsDiscoveryNruNtfReq
    RCS_DISCOVERY_NRU_NTF_RSP,              //消息体: CRcsDiscoveryNruNtfRsp

    //NRU设备的增、删、改、查
	RCS_ADD_NRU_REQ,                    //消息体: CRcsAddNruReq
    RCS_ADD_NRU_RSP,                    //消息体: CRcsAddNruRsp

	RCS_MOD_NRU_REQ,                    //消息体: CRcsModNruReq
    RCS_MOD_NRU_RSP,                    //消息体: CRcsModNruRsp

	RCS_DEL_NRU_REQ,                    //消息体: CRcsDelNruReq
    RCS_DEL_NRU_RSP,                    //消息体: CRcsDelNruRsp

    //查询nru设备，暂不支持分页
	RCS_QUERY_NRU_REQ,                  //消息体: CRcsQueryNruReq
    RCS_QUERY_NRU_RSP,                  //消息体: CRcsQueryNruRsp
	RCS_QUERY_NRU_NTF_REQ,              //消息体: CRcsQueryNruNtfReq
    RCS_QUERY_NRU_NTF_RSP,              //消息体: CRcsQueryNruNtfRsp

	//查询所有在线nru设备的详细信息，暂不支持分页
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ,			//消息体: CRcsQueryOnlineNruDetailInfoReq
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP,  			//消息体: CRcsQueryOnlineNruDetailInfoRsp
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ,		//消息体: CRcsQueryOnlineNruDetailInfoNtfReq
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP,		//消息体: CRcsQueryOnlineNruDetailInfoNtfRsp

    RCS_SUBSCRIBE_NRU_REQ,              //消息体: CRcsSubscribeNruReq
    RCS_SUBSCRIBE_NRU_RSP,              //消息体: CRcsSubscribeNruRsp
    RCS_UNSUBSCRIBE_NRU_REQ,            //消息体: CRcsUnsubscribeNruReq
    RCS_UNSUBSCRIBE_NRU_RSP,            //消息体: CRcsUnsubscribeNruRsp
	RCS_NRU_NTF_REQ,                    //消息体: CRcsNruNtfReq
    RCS_NRU_NTF_RSP,                    //消息体: CRcsNruNtfRsp

    //网络配置
	RCS_MOD_NRU_NETWORK_REQ,         //消息体: CRcsModNruNetworkReq
    RCS_MOD_NRU_NETWORK_RSP,         //消息体: CRcsModNruNetworkRsp

    //磁盘操作
    RCS_FDISK_REQ,                      //消息体: CRcsFdiskReq
    RCS_FDISK_RSP,                      //消息体: CRcsFdiskRsp

    RCS_FORMAT_DISK_REQ,                //消息体: CRcsFormatDiskReq
    RCS_FORMAT_DISK_RSP,                //消息体: CRcsFormatDiskRsp

    RCS_CHECK_DISK_REQ,                 //消息体: CRcsCheckDiskReq
    RCS_CHECK_DISK_RSP,                 //消息体: CRcsCheckDiskRsp

    RCS_MOUNT_DISK_REQ,                 //消息体: CRcsMountDiskReq
    RCS_MOUNT_DISK_RSP,                 //消息体: CRcsMountDiskRsp

    RCS_UMOUNT_DISK_REQ,                //消息体: CRcsUnmountDiskReq
    RCS_UMOUNT_DISK_RSP,                //消息体: CRcsUnmountDiskRsp

    RCS_CANCEL_DISK_OP_REQ,             //消息体: CRcsCancleDiskOpReq
    RCS_CANCEL_DISK_OP_RSP,             //消息体: CRcsCancleDiskOpRsp

    RCS_DEL_OFFLINE_DISK_REQ,           //消息体: CRcsDelOfflineDiskReq
    RCS_DEL_OFFLINE_DISK_RSP,           //消息体: CRcsDelOfflineDiskRsp

	//磁盘信息查询，暂不支持分页
    RCS_QUERY_DISK_INFO_REQ,             //消息体: CRcsQueryDiskInfoReq
    RCS_QUERY_DISK_INFO_RSP,             //消息体: CRcsQueryDiskInfoRsp
    RCS_QUERY_DISK_INFO_NTF_REQ,         //消息体: CRcsQueryDiskInfoNtfReq
    RCS_QUERY_DISK_INFO_NTF_RSP,         //消息体: CRcsQueryDiskInfoNtfRsp

    //磁盘状态订阅
    RCS_SUBSCRIBE_DISKINFO_REQ,           //消息体: CRcsSubscribeDiskInfoReq
    RCS_SUBSCRIBE_DISKINFO_RSP,           //消息体: CRcsSubscribeDiskInfoRsp
    RCS_UNSUBSCRIBE_DISKINFO_REQ,         //消息体: CRcsUnsubscribeDiskInfoReq
    RCS_UNSUBSCRIBE_DISKINFO_RSP,         //消息体: CRcsUnsubscribeDiskInfoRsp
    RCS_REFRESH_SS_DISKINFO_REQ,         //消息体: CRcsRefreshSsNruDiskInfoReq
    RCS_REFRESH_SS_DISKINFO_RSP,         //消息体: CRcsRefreshSsNruDiskInfoRsp

    RCS_DISK_INFO_NTF_REQ,              //消息体: CRcsDiskInfoNtfReq
    RCS_DISK_INFO_NTF_RSP,              //消息体: CRcsDiskInfoNtfRsp
    RCS_DISK_PART_INFO_NTF_REQ,         //消息体: CRcsDiskPartInfoNtfReq
    RCS_DISK_PART_INFO_NTF_RSP,         //消息体: CRcsDiskPartInfoNtfRsp
    RCS_DISK_STATE_NTF_REQ,             //消息体: CRcsDiskStateNtfReq  [目前保留，未使用]
    RCS_DISK_STATE_NTF_RSP,             //消息体: CRcsDiskStateNtfRsp  [目前保留，未使用]
    RCS_DISK_PART_STATE_NTF_REQ,        //消息体: CRcsDiskPartStateNtfReq  [目前保留，未使用]
    RCS_DISK_PART_STATE_NTF_RSP,        //消息体: CRcsDiskPartStateNtfRsp  [目前保留，未使用]
    RCS_DISK_OP_STATE_NTF_REQ,          //消息体: CRcsDiskOpStateNtfReq
    RCS_DISK_OP_STATE_NTF_RSP,          //消息体: CRcsDiskOpStateNtfRsp
    RCS_DISK_IOUTIL_NTF_REQ,            //消息体: CRcsDiskIoUtilNtfReq
    RCS_DISK_IOUTIL_NTF_RSP,            //消息体: CRcsDiskIoUtilNtfRsp
	RCS_DISK_PART_ALARM_NTF_REQ,		//消息体：CRcsDiskPartAlarmNtfReq
	RCS_DISK_PART_ALARM_NTF_RSP,		//消息体：CRcsDiskPartAlarmNtfRsp

    //磁盘异常状态通知
    RCS_DISK_ABN_NTF_REQ,               //消息体: CRcsDiskAbnNtfReq    [目前保留，未使用]
    RCS_DISK_ABN_NTF_RSP,               //消息体: CRcsDiskAbnNtfRsp    [目前保留，未使用]

	//磁盘日志查询，暂不支持分页
    RCS_QUERY_DISK_LOG_REQ,             //消息体: CRcsQueryDiskLogReq
    RCS_QUERY_DISK_LOG_RSP,             //消息体: CRcsQueryDiskLogRsp
    RCS_QUERY_DISK_LOG_NTF_REQ,         //消息体: CRcsQueryDiskLogNtfReq
    RCS_QUERY_DISK_LOG_NTF_RSP,         //消息体: CRcsQueryDiskLogNtfRsp

	//NetFS配置相关
	RCS_GET_NETFS_CFG_REQ,              //消息体: CRcsGetNetFSCfgReq
	RCS_GET_NETFS_CFG_RSP,              //消息体: CRcsGetNetFSCfgRsp

	RCS_ADD_NETFS_CFG_REQ,              //消息体: CRcsAddNetFSCfgReq
	RCS_ADD_NETFS_CFG_RSP,              //消息体: CRcsAddNetFSCfgRsp

	RCS_DEL_NETFS_CFG_REQ,              //消息体: CRcsDelNetFSCfgReq
	RCS_DEL_NETFS_CFG_RSP,              //消息体: CRcsDelNetFSCfgRsp

	RCS_SET_DISK_USAGE_REQ,              //消息体: CRcsSetDiskUsageReq
	RCS_SET_DISK_USAGE_RSP,              //消息体: CRcsSetDiskUsageRsp

	RCS_GET_NETFS_EXPORT_LIST_REQ,      //消息体: CRcsGetNetFSExportListReq
	RCS_GET_NETFS_EXPORT_LIST_RSP,      //消息体: CRcsGetNetFSExportListRsp

    //iscsi配置相关
    RCS_GET_ISCSI_CONF_REQ,             //消息体: CRcsGetIscsiConfReq
    RCS_GET_ISCSI_CONF_RSP,             //消息体: CRcsGetIscsiConfRsp

    RCS_ADD_ISCSI_CONF_REQ,             //消息体: CRcsAddIscsiConfReq
    RCS_ADD_ISCSI_CONF_RSP,             //消息体: CRcsAddIscsiConfRsp

    RCS_DEL_ISCSI_CONF_REQ,             //消息体: CRcsDelIscsiConfReq
    RCS_DEL_ISCSI_CONF_RSP,             //消息体: CRcsDelIscsiConfRsp

    RCS_CTRL_ISCSI_REQ,                 //消息体: CRcsCtrlIscsiReq
    RCS_CTRL_ISCSI_RSP,                 //消息体: CRcsCtrlIscsiRsp

    RCS_DISCONNECT_ISCSI_REQ,           //消息体: CRcsDisconnectIscsiReq
    RCS_DISCONNECT_ISCSI_RSP,           //消息体: CRcsDisconnectIscsiRsp

    //获取阵列信息
    RCS_GET_ARRAY_INFO_REQ,             //消息体: CRcsGetArrayInfoReq
    RCS_GET_ARRAY_INFO_RSP,             //消息体: CRcsGetArrayInfoRsp

    //获取磁盘组信息
    RCS_GET_DISK_GROUP_INFO_REQ,        //消息体: CRcsGetDiskGroupInfoReq
    RCS_GET_DISK_GROUP_INFO_RSP,        //消息体: CRcsGetDiskGroupInfoRsp

    //获取物理磁盘信息信息
    RCS_GET_PHYSICAL_DISK_INFO_REQ,     //消息体: CRcsGetPhysicalDiskInfoReq
    RCS_GET_PHYSICAL_DISK_INFO_RSP,     //消息体: CRcsGetPhysicalDiskInfoRsp

    //获取虚拟磁盘信息
    RCS_GET_VIRTUAL_DISK_INFO_REQ,      //消息体: CRcsGetVirtualDiskInfoReq
    RCS_GET_VIRTUAL_DISK_INFO_RSP,      //消息体: CRcsGetVirtualDiskInfoRsp

    //获取iscsi的发起者名
    RCS_GET_ISCSI_INITIATORNAME_REQ,    //消息体：CRcsGetIscsiInitatorNameReq
    RCS_GET_ISCSI_INITIATORNAME_RSP,    //消息体：CRcsGetIscsiInitatorNameRsp

    //发送TaskStart消息 
    RCS_TASK_START_REQ,                 //消息体：CRcsTaskStartReq
    RCS_TASK_START_RSP,                 //消息体：CRcsTaskStartRsp
									    
    //获取外厂商分区信息 			   
    RCS_GET_USED_OTHER_STORAGE_SIZE_REQ,//消息体：CRcsGetUsedOtherStorageSizeReq
    RCS_GET_USED_OTHER_STORAGE_SIZE_RSP,//消息体：CRcsGetUsedOtherStorageSizeRsp
									    
    RCS_CURRENT_EVENT_END,
};

inline void InitRcsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(RCS_SET_SYSPARAM_REQ);                    //消息体: CRcsSetSysParamReq
    OSP_ADD_EVENT_DESC(RCS_SET_SYSPARAM_RSP);                    //消息体: CRcsSetSysParamRsp
    OSP_ADD_EVENT_DESC(RCS_GET_SYSPARAM_REQ);                    //消息体: CRcsGetSysParamReq
    OSP_ADD_EVENT_DESC(RCS_GET_SYSPARAM_RSP);                    //消息体: CRcsGetSysParamRsp
    OSP_ADD_EVENT_DESC(RCS_BINDING_PUCHN_REQ);                   //消息体: CRcsBindingPuChnReq
    OSP_ADD_EVENT_DESC(RCS_BINDING_PUCHN_RSP);                   //消息体: CRcsBindingPuChnRsp
    OSP_ADD_EVENT_DESC(RCS_UNBINDING_PUCHN_REQ);                 //消息体: CRcsUnbindingPuChnReq
    OSP_ADD_EVENT_DESC(RCS_UNBINDING_PUCHN_RSP);                 //消息体: CRcsUnbindingPuChnRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_REQ);             //消息体: CRcsQueryPuChnBindingReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_RSP);             //消息体: CRcsQueryPuChnBindingRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_NTF_REQ);         //消息体: CRcsQueryPuChnBindingNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_NTF_RSP);         //消息体: CRcsQueryPuChnBindingNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_PARAM_REQ);                //消息体: CRcsSetRecordParamReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_PARAM_RSP);                //消息体: CRcsSetRecordParamRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_REQ);              //消息体: CRcsQueryRecordParamReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_RSP);              //消息体: CRcsQueryRecordParamRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_NTF_REQ);          //消息体: CRcsQueryRecordParamNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_NTF_RSP);          //消息体: CRcsQueryRecordParamNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_TASK_REQ);                 //消息体: CRcsSetRecordTaskReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_TASK_RSP);                 //消息体: CRcsSetRecordTaskRsp
    OSP_ADD_EVENT_DESC(RCS_LOCK_RECORD_REQ);                     //消息体: CRcsLockRecordReq
    OSP_ADD_EVENT_DESC(RCS_LOCK_RECORD_RSP);                     //消息体: CRcsLockRecordRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_REQ);                    //消息体: CRcsQueryRecordReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_RSP);                    //消息体: CRcsQueryRecordRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_NTF_REQ);                //消息体: CRcsQueryRecordNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_NTF_RSP);                //消息体: CRcsQueryRecordNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DELETE_RECORD_REQ);                   //消息体：CRcsDeleteRecordReq
    OSP_ADD_EVENT_DESC(RCS_DELETE_RECORD_RSP);                   //消息体：CRcsDeleteRecordRsp
    OSP_ADD_EVENT_DESC(RCS_REC_DEL_REQ);                         //消息体: CRcsRecDelReq
    OSP_ADD_EVENT_DESC(RCS_REC_DEL_RSP);                         //消息体: CRcsRecDelRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_DAYS_REQ);               //消息体: CRcsQueryRecordDaysReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_DAYS_RSP);               //消息体: CRcsQueryRecordDaysRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_ALIAS_REQ);                //消息体：CRcsSetRecordAliasReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_ALIAS_RSP);                //消息体：CRcsSetRecordAliasRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_RECTAG_REQ);                      //消息体: CRcsAddRecTagReq
    OSP_ADD_EVENT_DESC(RCS_ADD_RECTAG_RSP);                      //消息体: CRcsAddRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_RECTAG_REQ);                      //消息体: CRcsDelRecTagReq
    OSP_ADD_EVENT_DESC(RCS_DEL_RECTAG_RSP);                      //消息体: CRcsDelRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_UPDATE_RECTAG_REQ);                   //消息体: CRcsUpdateRecTagReq
    OSP_ADD_EVENT_DESC(RCS_UPDATE_RECTAG_RSP);                   //消息体: CRcsUpdateRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_REQ);                    //消息体: CRcsQueryRecTagReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_RSP);                    //消息体: CRcsQueryRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_NTF_REQ);                //消息体: CRcsQueryRecTagNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_NTF_RSP);                //消息体: CRcsQueryRecTagNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_PU_STATUS_REQ);             //消息体: CRcsSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_PU_STATUS_RSP);             //消息体: CRcsSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_PU_STATUS_REQ);           //消息体: CRcsUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_PU_STATUS_RSP);           //消息体: CRcsUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_PU_RECORD_STATUS_NTF_REQ);            //消息体: CRcsPuRecordStatusNtyReq
    OSP_ADD_EVENT_DESC(RCS_PU_RECORD_STATUS_NTF_RSP);            //消息体: CRcsPuRecordStatusNtyRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_SUBSCRIBE_PU_STATUS_REQ);        //消息体: CRcsListSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_SUBSCRIBE_PU_STATUS_RSP);        //消息体: CRcsListSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_REFRESH_SS_PU_STATUS_REQ);       //消息体: CRcsListRefreshSsPuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_REFRESH_SS_PU_STATUS_RSP);       //消息体: CRcsListRefreshSsPuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_UNSUBSCRIBE_PU_STATUS_REQ);      //消息体: CRcsListUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_UNSUBSCRIBE_PU_STATUS_RSP);      //消息体: CRcsListUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ);     //消息体: CRcsCatalogSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP);     //消息体: CRcsCatalogSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ);    //消息体: CRcsCatalogRefreshSsPuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP);    //消息体: CRcsCatalogRefreshSsPuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ);   //消息体: CRcsCatalogUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP);   //消息体: CRcsCatalogUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_RECORD_LOCK_OPR_REQ);                 //消息体: CRcsRecordLockOprReq
    OSP_ADD_EVENT_DESC(RCS_RECORD_LOCK_OPR_RSP);                 //消息体: CRcsRecordLockOprRsp
    OSP_ADD_EVENT_DESC(RCS_START_DISCOVERY_NRU_REQ);             //消息体: CRcsStartDiscoveryNruReq
    OSP_ADD_EVENT_DESC(RCS_START_DISCOVERY_NRU_RSP);             //消息体: CRcsStartDiscoveryNruRsp
    OSP_ADD_EVENT_DESC(RCS_STOP_DISCOVERY_NRU_REQ);              //消息体: CRcsStopDiscoveryNruReq
    OSP_ADD_EVENT_DESC(RCS_STOP_DISCOVERY_NRU_RSP);              //消息体: CRcsStopDiscoveryNruRsp
    OSP_ADD_EVENT_DESC(RCS_DISCOVERY_NRU_NTF_REQ);               //消息体: CRcsDiscoveryNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISCOVERY_NRU_NTF_RSP);               //消息体: CRcsDiscoveryNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_NRU_REQ);                         //消息体: CRcsAddNruReq
    OSP_ADD_EVENT_DESC(RCS_ADD_NRU_RSP);                         //消息体: CRcsAddNruRsp
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_REQ);                         //消息体: CRcsModNruReq
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_RSP);                         //消息体: CRcsModNruRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_NRU_REQ);                         //消息体: CRcsDelNruReq
    OSP_ADD_EVENT_DESC(RCS_DEL_NRU_RSP);                         //消息体: CRcsDelNruRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_REQ);                       //消息体: CRcsQueryNruReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_RSP);                       //消息体: CRcsQueryNruRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_NTF_REQ);                   //消息体: CRcsQueryNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_NTF_RSP);                   //消息体: CRcsQueryNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ);    //消息体: CRcsQueryOnlineNruDetailInfoReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP);    //消息体: CRcsQueryOnlineNruDetailInfoRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ);//消息体: CRcsQueryOnlineNruDetailInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP);//消息体: CRcsQueryOnlineNruDetailInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_NRU_REQ);                   //消息体: CRcsSubscribeNruReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_NRU_RSP);                   //消息体: CRcsSubscribeNruRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_NRU_REQ);                 //消息体: CRcsUnsubscribeNruReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_NRU_RSP);                 //消息体: CRcsUnsubscribeNruRsp
    OSP_ADD_EVENT_DESC(RCS_NRU_NTF_REQ);                         //消息体: CRcsNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_NRU_NTF_RSP);                         //消息体: CRcsNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_NETWORK_REQ);                 //消息体: CRcsModNruNetworkReq
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_NETWORK_RSP);                 //消息体: CRcsModNruNetworkRsp
    OSP_ADD_EVENT_DESC(RCS_FDISK_REQ);                           //消息体: CRcsFdiskReq
    OSP_ADD_EVENT_DESC(RCS_FDISK_RSP);                           //消息体: CRcsFdiskRsp
    OSP_ADD_EVENT_DESC(RCS_FORMAT_DISK_REQ);                     //消息体: CRcsFormatDiskReq
    OSP_ADD_EVENT_DESC(RCS_FORMAT_DISK_RSP);                     //消息体: CRcsFormatDiskRsp
    OSP_ADD_EVENT_DESC(RCS_CHECK_DISK_REQ);                      //消息体: CRcsCheckDiskReq
    OSP_ADD_EVENT_DESC(RCS_CHECK_DISK_RSP);                      //消息体: CRcsCheckDiskRsp
    OSP_ADD_EVENT_DESC(RCS_MOUNT_DISK_REQ);                      //消息体: CRcsMountDiskReq
    OSP_ADD_EVENT_DESC(RCS_MOUNT_DISK_RSP);                      //消息体: CRcsMountDiskRsp
    OSP_ADD_EVENT_DESC(RCS_UMOUNT_DISK_REQ);                     //消息体: CRcsUnmountDiskReq
    OSP_ADD_EVENT_DESC(RCS_UMOUNT_DISK_RSP);                     //消息体: CRcsUnmountDiskRsp
    OSP_ADD_EVENT_DESC(RCS_CANCEL_DISK_OP_REQ);                  //消息体: CRcsCancleDiskOpReq
    OSP_ADD_EVENT_DESC(RCS_CANCEL_DISK_OP_RSP);                  //消息体: CRcsCancleDiskOpRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_OFFLINE_DISK_REQ);                //消息体: CRcsDelOfflineDiskReq
    OSP_ADD_EVENT_DESC(RCS_DEL_OFFLINE_DISK_RSP);                //消息体: CRcsDelOfflineDiskRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_REQ);                 //消息体: CRcsQueryDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_RSP);                 //消息体: CRcsQueryDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_NTF_REQ);             //消息体: CRcsQueryDiskInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_NTF_RSP);             //消息体: CRcsQueryDiskInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_DISKINFO_REQ);              //消息体: CRcsSubscribeDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_DISKINFO_RSP);              //消息体: CRcsSubscribeDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_DISKINFO_REQ);            //消息体: CRcsUnsubscribeDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_DISKINFO_RSP);            //消息体: CRcsUnsubscribeDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_REFRESH_SS_DISKINFO_REQ);             //消息体: CRcsRefreshSsNruDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_REFRESH_SS_DISKINFO_RSP);             //消息体: CRcsRefreshSsNruDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_INFO_NTF_REQ);                   //消息体: CRcsDiskInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_INFO_NTF_RSP);                   //消息体: CRcsDiskInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_INFO_NTF_REQ);              //消息体: CRcsDiskPartInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_INFO_NTF_RSP);              //消息体: CRcsDiskPartInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_STATE_NTF_REQ);                  //消息体: CRcsDiskStateNtfReq  [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_DISK_STATE_NTF_RSP);                  //消息体: CRcsDiskStateNtfRsp  [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_STATE_NTF_REQ);             //消息体: CRcsDiskPartStateNtfReq  [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_STATE_NTF_RSP);             //消息体: CRcsDiskPartStateNtfRsp  [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_DISK_OP_STATE_NTF_REQ);               //消息体: CRcsDiskOpStateNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_OP_STATE_NTF_RSP);               //消息体: CRcsDiskOpStateNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_IOUTIL_NTF_REQ);                 //消息体: CRcsDiskIoUtilNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_IOUTIL_NTF_RSP);                 //消息体: CRcsDiskIoUtilNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_ALARM_NTF_REQ);             //消息体：CRcsDiskPartAlarmNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_ALARM_NTF_RSP);             //消息体：CRcsDiskPartAlarmNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_ABN_NTF_REQ);                    //消息体: CRcsDiskAbnNtfReq    [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_DISK_ABN_NTF_RSP);                    //消息体: CRcsDiskAbnNtfRsp    [目前保留，未使用]
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_REQ);                  //消息体: CRcsQueryDiskLogReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_RSP);                  //消息体: CRcsQueryDiskLogRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_NTF_REQ);              //消息体: CRcsQueryDiskLogNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_NTF_RSP);              //消息体: CRcsQueryDiskLogNtfRsp
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_CFG_REQ);                   //消息体: CRcsGetNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_CFG_RSP);                   //消息体: CRcsGetNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_NETFS_CFG_REQ);                   //消息体: CRcsAddNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_ADD_NETFS_CFG_RSP);                   //消息体: CRcsAddNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_NETFS_CFG_REQ);                   //消息体: CRcsDelNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_DEL_NETFS_CFG_RSP);                   //消息体: CRcsDelNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_SET_DISK_USAGE_REQ);                  //消息体: CRcsSetDiskUsageReq
    OSP_ADD_EVENT_DESC(RCS_SET_DISK_USAGE_RSP);                  //消息体: CRcsSetDiskUsageRsp
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_EXPORT_LIST_REQ);           //消息体: CRcsGetNetFSExportListReq
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_EXPORT_LIST_RSP);           //消息体: CRcsGetNetFSExportListRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_CONF_REQ);                  //消息体: CRcsGetIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_CONF_RSP);                  //消息体: CRcsGetIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_ISCSI_CONF_REQ);                  //消息体: CRcsAddIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_ADD_ISCSI_CONF_RSP);                  //消息体: CRcsAddIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_ISCSI_CONF_REQ);                  //消息体: CRcsDelIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_DEL_ISCSI_CONF_RSP);                  //消息体: CRcsDelIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_CTRL_ISCSI_REQ);                      //消息体: CRcsCtrlIscsiReq
    OSP_ADD_EVENT_DESC(RCS_CTRL_ISCSI_RSP);                      //消息体: CRcsCtrlIscsiRsp
    OSP_ADD_EVENT_DESC(RCS_DISCONNECT_ISCSI_REQ);                //消息体: CRcsDisconnectIscsiReq
    OSP_ADD_EVENT_DESC(RCS_DISCONNECT_ISCSI_RSP);                //消息体: CRcsDisconnectIscsiRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ARRAY_INFO_REQ);                  //消息体: CRcsGetArrayInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_ARRAY_INFO_RSP);                  //消息体: CRcsGetArrayInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_DISK_GROUP_INFO_REQ);             //消息体: CRcsGetDiskGroupInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_DISK_GROUP_INFO_RSP);             //消息体: CRcsGetDiskGroupInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_PHYSICAL_DISK_INFO_REQ);          //消息体: CRcsGetPhysicalDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_PHYSICAL_DISK_INFO_RSP);          //消息体: CRcsGetPhysicalDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_VIRTUAL_DISK_INFO_REQ);           //消息体: CRcsGetVirtualDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_VIRTUAL_DISK_INFO_RSP);           //消息体: CRcsGetVirtualDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_INITIATORNAME_REQ);         //消息体：CRcsGetIscsiInitatorNameReq
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_INITIATORNAME_RSP);         //消息体：CRcsGetIscsiInitatorNameRsp
    OSP_ADD_EVENT_DESC(RCS_TASK_START_REQ);                      //消息体：CRcsTaskStartReq
    OSP_ADD_EVENT_DESC(RCS_TASK_START_RSP);                      //消息体：CRcsTaskStartRsp
    OSP_ADD_EVENT_DESC(RCS_GET_USED_OTHER_STORAGE_SIZE_REQ);     //消息体：CRcsGetUsedOtherStorageSizeReq
    OSP_ADD_EVENT_DESC(RCS_GET_USED_OTHER_STORAGE_SIZE_RSP);     //消息体：CRcsGetUsedOtherStorageSizeRsp
    OSP_ADD_EVENT_DESC(RCS_CURRENT_EVENT_END);                   
}


#endif  //#ifndef __RCS_EVENT_H__





























