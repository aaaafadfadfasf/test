/*****************************************************************************
   ģ����      : rcs_event
   �ļ���      : rcs_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: rcs_event.h ����������RCS����ϢID
   ����        : 
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���              �޸�����
   2012/06/11   1.0        huangzhichun       ����
******************************************************************************/
#ifndef __RCS_EVENT_H__
#define __RCS_EVENT_H__

#include "../cms_eventid.h"
#include "../cms_proto.h"

/////////////////////////////////////////////////////////////////////
//RCS�ӿ���Ϣ
/////////////////////////////////////////////////////////////////////
enum ERcsMsg
{
    //¼��ϵͳ�������縲�ǲ���
    RCS_SET_SYSPARAM_REQ = RCS_EVENT_BEGIN, //��Ϣ��: CRcsSetSysParamReq
    RCS_SET_SYSPARAM_RSP,               //��Ϣ��: CRcsSetSysParamRsp
    RCS_GET_SYSPARAM_REQ,               //��Ϣ��: CRcsGetSysParamReq
    RCS_GET_SYSPARAM_RSP,               //��Ϣ��: CRcsGetSysParamRsp

    //PuChn���̰�
    RCS_BINDING_PUCHN_REQ,              //��Ϣ��: CRcsBindingPuChnReq
    RCS_BINDING_PUCHN_RSP,              //��Ϣ��: CRcsBindingPuChnRsp
    RCS_UNBINDING_PUCHN_REQ,            //��Ϣ��: CRcsUnbindingPuChnReq
    RCS_UNBINDING_PUCHN_RSP,            //��Ϣ��: CRcsUnbindingPuChnRsp

    //���̰󶨲�ѯ���ݲ�֧�ַ�ҳ
    RCS_QUERY_PUCHN_BINDING_REQ,        //��Ϣ��: CRcsQueryPuChnBindingReq
    RCS_QUERY_PUCHN_BINDING_RSP,        //��Ϣ��: CRcsQueryPuChnBindingRsp
    RCS_QUERY_PUCHN_BINDING_NTF_REQ,    //��Ϣ��: CRcsQueryPuChnBindingNtfReq
    RCS_QUERY_PUCHN_BINDING_NTF_RSP,    //��Ϣ��: CRcsQueryPuChnBindingNtfRsp

    //¼��������ݲ�֧�ַ�ҳ��ѯ
    RCS_SET_RECORD_PARAM_REQ,            //��Ϣ��: CRcsSetRecordParamReq
    RCS_SET_RECORD_PARAM_RSP,            //��Ϣ��: CRcsSetRecordParamRsp
    RCS_QUERY_RECORD_PARAM_REQ,          //��Ϣ��: CRcsQueryRecordParamReq
    RCS_QUERY_RECORD_PARAM_RSP,          //��Ϣ��: CRcsQueryRecordParamRsp
    RCS_QUERY_RECORD_PARAM_NTF_REQ,      //��Ϣ��: CRcsQueryRecordParamNtfReq
    RCS_QUERY_RECORD_PARAM_NTF_RSP,      //��Ϣ��: CRcsQueryRecordParamNtfRsp

    //����¼������(��Ҫ���ֶ����澯)
    RCS_SET_RECORD_TASK_REQ,            //��Ϣ��: CRcsSetRecordTaskReq
    RCS_SET_RECORD_TASK_RSP,            //��Ϣ��: CRcsSetRecordTaskRsp

	//¼������/����
	RCS_LOCK_RECORD_REQ,                //��Ϣ��: CRcsLockRecordReq
	RCS_LOCK_RECORD_RSP,                //��Ϣ��: CRcsLockRecordRsp

    //¼���¼��ѯ���ݲ�֧�ַ�ҳ
    RCS_QUERY_RECORD_REQ,               //��Ϣ��: CRcsQueryRecordReq
    RCS_QUERY_RECORD_RSP,               //��Ϣ��: CRcsQueryRecordRsp
    RCS_QUERY_RECORD_NTF_REQ,           //��Ϣ��: CRcsQueryRecordNtfReq
    RCS_QUERY_RECORD_NTF_RSP,           //��Ϣ��: CRcsQueryRecordNtfRsp

    //¼���¼ɾ��
    RCS_DELETE_RECORD_REQ,              //��Ϣ�壺CRcsDeleteRecordReq
    RCS_DELETE_RECORD_RSP,              //��Ϣ�壺CRcsDeleteRecordRsp

	//¼��ɾ������ͨ��+ʱ��Σ�
	RCS_REC_DEL_REQ,			        //��Ϣ��: CRcsRecDelReq
	RCS_REC_DEL_RSP,			        //��Ϣ��: CRcsRecDelRsp

    //¼��������ѯ
    RCS_QUERY_RECORD_DAYS_REQ,          //��Ϣ��: CRcsQueryRecordDaysReq
    RCS_QUERY_RECORD_DAYS_RSP,          //��Ϣ��: CRcsQueryRecordDaysRsp

	//����¼�����
	RCS_SET_RECORD_ALIAS_REQ,			//��Ϣ�壺CRcsSetRecordAliasReq
	RCS_SET_RECORD_ALIAS_RSP,			//��Ϣ�壺CRcsSetRecordAliasRsp

    //¼�����أ���Invite
    //¼�񲥷ţ���Invite

    //¼���ǩ
    RCS_ADD_RECTAG_REQ,                 //��Ϣ��: CRcsAddRecTagReq
    RCS_ADD_RECTAG_RSP,                 //��Ϣ��: CRcsAddRecTagRsp
    RCS_DEL_RECTAG_REQ,                 //��Ϣ��: CRcsDelRecTagReq
    RCS_DEL_RECTAG_RSP,                 //��Ϣ��: CRcsDelRecTagRsp
    RCS_UPDATE_RECTAG_REQ,              //��Ϣ��: CRcsUpdateRecTagReq
    RCS_UPDATE_RECTAG_RSP,              //��Ϣ��: CRcsUpdateRecTagRsp
    RCS_QUERY_RECTAG_REQ,               //��Ϣ��: CRcsQueryRecTagReq
    RCS_QUERY_RECTAG_RSP,               //��Ϣ��: CRcsQueryRecTagRsp
    RCS_QUERY_RECTAG_NTF_REQ,           //��Ϣ��: CRcsQueryRecTagNtfReq
    RCS_QUERY_RECTAG_NTF_RSP,           //��Ϣ��: CRcsQueryRecTagNtfRsp

    //PU¼��״̬����
    RCS_SUBSCRIBE_PU_STATUS_REQ,        //��Ϣ��: CRcsSubscribePuStatusReq
    RCS_SUBSCRIBE_PU_STATUS_RSP,        //��Ϣ��: CRcsSubscribePuStatusRsp

    RCS_UNSUBSCRIBE_PU_STATUS_REQ,      //��Ϣ��: CRcsUnsubscribePuStatusReq
    RCS_UNSUBSCRIBE_PU_STATUS_RSP,      //��Ϣ��: CRcsUnsubscribePuStatusRsp

    RCS_PU_RECORD_STATUS_NTF_REQ,       //��Ϣ��: CRcsPuRecordStatusNtyReq
    RCS_PU_RECORD_STATUS_NTF_RSP,       //��Ϣ��: CRcsPuRecordStatusNtyRsp

	//PU¼��״̬�б���
	RCS_LIST_SUBSCRIBE_PU_STATUS_REQ,        //��Ϣ��: CRcsListSubscribePuStatusReq
	RCS_LIST_SUBSCRIBE_PU_STATUS_RSP,        //��Ϣ��: CRcsListSubscribePuStatusRsp
	RCS_LIST_REFRESH_SS_PU_STATUS_REQ,       //��Ϣ��: CRcsListRefreshSsPuStatusReq
	RCS_LIST_REFRESH_SS_PU_STATUS_RSP,       //��Ϣ��: CRcsListRefreshSsPuStatusRsp
	RCS_LIST_UNSUBSCRIBE_PU_STATUS_REQ,      //��Ϣ��: CRcsListUnsubscribePuStatusReq
	RCS_LIST_UNSUBSCRIBE_PU_STATUS_RSP,      //��Ϣ��: CRcsListUnsubscribePuStatusRsp

	//PU¼��״̬Ŀ¼����
	RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ,        //��Ϣ��: CRcsCatalogSubscribePuStatusReq
	RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP,        //��Ϣ��: CRcsCatalogSubscribePuStatusRsp
	RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ,       //��Ϣ��: CRcsCatalogRefreshSsPuStatusReq
	RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP,       //��Ϣ��: CRcsCatalogRefreshSsPuStatusRsp
	RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ,      //��Ϣ��: CRcsCatalogUnsubscribePuStatusReq
	RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP,      //��Ϣ��: CRcsCatalogUnsubscribePuStatusRsp

	//¼������/����
	RCS_RECORD_LOCK_OPR_REQ,			//��Ϣ��: CRcsRecordLockOprReq
	RCS_RECORD_LOCK_OPR_RSP,			//��Ϣ��: CRcsRecordLockOprRsp


    //////////////////////////////////////////////////////////////////////
    //NRU����

	//��������,������Ϣ
	RCS_START_DISCOVERY_NRU_REQ,            //��Ϣ��: CRcsStartDiscoveryNruReq
    RCS_START_DISCOVERY_NRU_RSP,            //��Ϣ��: CRcsStartDiscoveryNruRsp
	RCS_STOP_DISCOVERY_NRU_REQ,             //��Ϣ��: CRcsStopDiscoveryNruReq
    RCS_STOP_DISCOVERY_NRU_RSP,             //��Ϣ��: CRcsStopDiscoveryNruRsp
	RCS_DISCOVERY_NRU_NTF_REQ,              //��Ϣ��: CRcsDiscoveryNruNtfReq
    RCS_DISCOVERY_NRU_NTF_RSP,              //��Ϣ��: CRcsDiscoveryNruNtfRsp

    //NRU�豸������ɾ���ġ���
	RCS_ADD_NRU_REQ,                    //��Ϣ��: CRcsAddNruReq
    RCS_ADD_NRU_RSP,                    //��Ϣ��: CRcsAddNruRsp

	RCS_MOD_NRU_REQ,                    //��Ϣ��: CRcsModNruReq
    RCS_MOD_NRU_RSP,                    //��Ϣ��: CRcsModNruRsp

	RCS_DEL_NRU_REQ,                    //��Ϣ��: CRcsDelNruReq
    RCS_DEL_NRU_RSP,                    //��Ϣ��: CRcsDelNruRsp

    //��ѯnru�豸���ݲ�֧�ַ�ҳ
	RCS_QUERY_NRU_REQ,                  //��Ϣ��: CRcsQueryNruReq
    RCS_QUERY_NRU_RSP,                  //��Ϣ��: CRcsQueryNruRsp
	RCS_QUERY_NRU_NTF_REQ,              //��Ϣ��: CRcsQueryNruNtfReq
    RCS_QUERY_NRU_NTF_RSP,              //��Ϣ��: CRcsQueryNruNtfRsp

	//��ѯ��������nru�豸����ϸ��Ϣ���ݲ�֧�ַ�ҳ
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ,			//��Ϣ��: CRcsQueryOnlineNruDetailInfoReq
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP,  			//��Ϣ��: CRcsQueryOnlineNruDetailInfoRsp
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ,		//��Ϣ��: CRcsQueryOnlineNruDetailInfoNtfReq
	RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP,		//��Ϣ��: CRcsQueryOnlineNruDetailInfoNtfRsp

    RCS_SUBSCRIBE_NRU_REQ,              //��Ϣ��: CRcsSubscribeNruReq
    RCS_SUBSCRIBE_NRU_RSP,              //��Ϣ��: CRcsSubscribeNruRsp
    RCS_UNSUBSCRIBE_NRU_REQ,            //��Ϣ��: CRcsUnsubscribeNruReq
    RCS_UNSUBSCRIBE_NRU_RSP,            //��Ϣ��: CRcsUnsubscribeNruRsp
	RCS_NRU_NTF_REQ,                    //��Ϣ��: CRcsNruNtfReq
    RCS_NRU_NTF_RSP,                    //��Ϣ��: CRcsNruNtfRsp

    //��������
	RCS_MOD_NRU_NETWORK_REQ,         //��Ϣ��: CRcsModNruNetworkReq
    RCS_MOD_NRU_NETWORK_RSP,         //��Ϣ��: CRcsModNruNetworkRsp

    //���̲���
    RCS_FDISK_REQ,                      //��Ϣ��: CRcsFdiskReq
    RCS_FDISK_RSP,                      //��Ϣ��: CRcsFdiskRsp

    RCS_FORMAT_DISK_REQ,                //��Ϣ��: CRcsFormatDiskReq
    RCS_FORMAT_DISK_RSP,                //��Ϣ��: CRcsFormatDiskRsp

    RCS_CHECK_DISK_REQ,                 //��Ϣ��: CRcsCheckDiskReq
    RCS_CHECK_DISK_RSP,                 //��Ϣ��: CRcsCheckDiskRsp

    RCS_MOUNT_DISK_REQ,                 //��Ϣ��: CRcsMountDiskReq
    RCS_MOUNT_DISK_RSP,                 //��Ϣ��: CRcsMountDiskRsp

    RCS_UMOUNT_DISK_REQ,                //��Ϣ��: CRcsUnmountDiskReq
    RCS_UMOUNT_DISK_RSP,                //��Ϣ��: CRcsUnmountDiskRsp

    RCS_CANCEL_DISK_OP_REQ,             //��Ϣ��: CRcsCancleDiskOpReq
    RCS_CANCEL_DISK_OP_RSP,             //��Ϣ��: CRcsCancleDiskOpRsp

    RCS_DEL_OFFLINE_DISK_REQ,           //��Ϣ��: CRcsDelOfflineDiskReq
    RCS_DEL_OFFLINE_DISK_RSP,           //��Ϣ��: CRcsDelOfflineDiskRsp

	//������Ϣ��ѯ���ݲ�֧�ַ�ҳ
    RCS_QUERY_DISK_INFO_REQ,             //��Ϣ��: CRcsQueryDiskInfoReq
    RCS_QUERY_DISK_INFO_RSP,             //��Ϣ��: CRcsQueryDiskInfoRsp
    RCS_QUERY_DISK_INFO_NTF_REQ,         //��Ϣ��: CRcsQueryDiskInfoNtfReq
    RCS_QUERY_DISK_INFO_NTF_RSP,         //��Ϣ��: CRcsQueryDiskInfoNtfRsp

    //����״̬����
    RCS_SUBSCRIBE_DISKINFO_REQ,           //��Ϣ��: CRcsSubscribeDiskInfoReq
    RCS_SUBSCRIBE_DISKINFO_RSP,           //��Ϣ��: CRcsSubscribeDiskInfoRsp
    RCS_UNSUBSCRIBE_DISKINFO_REQ,         //��Ϣ��: CRcsUnsubscribeDiskInfoReq
    RCS_UNSUBSCRIBE_DISKINFO_RSP,         //��Ϣ��: CRcsUnsubscribeDiskInfoRsp
    RCS_REFRESH_SS_DISKINFO_REQ,         //��Ϣ��: CRcsRefreshSsNruDiskInfoReq
    RCS_REFRESH_SS_DISKINFO_RSP,         //��Ϣ��: CRcsRefreshSsNruDiskInfoRsp

    RCS_DISK_INFO_NTF_REQ,              //��Ϣ��: CRcsDiskInfoNtfReq
    RCS_DISK_INFO_NTF_RSP,              //��Ϣ��: CRcsDiskInfoNtfRsp
    RCS_DISK_PART_INFO_NTF_REQ,         //��Ϣ��: CRcsDiskPartInfoNtfReq
    RCS_DISK_PART_INFO_NTF_RSP,         //��Ϣ��: CRcsDiskPartInfoNtfRsp
    RCS_DISK_STATE_NTF_REQ,             //��Ϣ��: CRcsDiskStateNtfReq  [Ŀǰ������δʹ��]
    RCS_DISK_STATE_NTF_RSP,             //��Ϣ��: CRcsDiskStateNtfRsp  [Ŀǰ������δʹ��]
    RCS_DISK_PART_STATE_NTF_REQ,        //��Ϣ��: CRcsDiskPartStateNtfReq  [Ŀǰ������δʹ��]
    RCS_DISK_PART_STATE_NTF_RSP,        //��Ϣ��: CRcsDiskPartStateNtfRsp  [Ŀǰ������δʹ��]
    RCS_DISK_OP_STATE_NTF_REQ,          //��Ϣ��: CRcsDiskOpStateNtfReq
    RCS_DISK_OP_STATE_NTF_RSP,          //��Ϣ��: CRcsDiskOpStateNtfRsp
    RCS_DISK_IOUTIL_NTF_REQ,            //��Ϣ��: CRcsDiskIoUtilNtfReq
    RCS_DISK_IOUTIL_NTF_RSP,            //��Ϣ��: CRcsDiskIoUtilNtfRsp
	RCS_DISK_PART_ALARM_NTF_REQ,		//��Ϣ�壺CRcsDiskPartAlarmNtfReq
	RCS_DISK_PART_ALARM_NTF_RSP,		//��Ϣ�壺CRcsDiskPartAlarmNtfRsp

    //�����쳣״̬֪ͨ
    RCS_DISK_ABN_NTF_REQ,               //��Ϣ��: CRcsDiskAbnNtfReq    [Ŀǰ������δʹ��]
    RCS_DISK_ABN_NTF_RSP,               //��Ϣ��: CRcsDiskAbnNtfRsp    [Ŀǰ������δʹ��]

	//������־��ѯ���ݲ�֧�ַ�ҳ
    RCS_QUERY_DISK_LOG_REQ,             //��Ϣ��: CRcsQueryDiskLogReq
    RCS_QUERY_DISK_LOG_RSP,             //��Ϣ��: CRcsQueryDiskLogRsp
    RCS_QUERY_DISK_LOG_NTF_REQ,         //��Ϣ��: CRcsQueryDiskLogNtfReq
    RCS_QUERY_DISK_LOG_NTF_RSP,         //��Ϣ��: CRcsQueryDiskLogNtfRsp

	//NetFS�������
	RCS_GET_NETFS_CFG_REQ,              //��Ϣ��: CRcsGetNetFSCfgReq
	RCS_GET_NETFS_CFG_RSP,              //��Ϣ��: CRcsGetNetFSCfgRsp

	RCS_ADD_NETFS_CFG_REQ,              //��Ϣ��: CRcsAddNetFSCfgReq
	RCS_ADD_NETFS_CFG_RSP,              //��Ϣ��: CRcsAddNetFSCfgRsp

	RCS_DEL_NETFS_CFG_REQ,              //��Ϣ��: CRcsDelNetFSCfgReq
	RCS_DEL_NETFS_CFG_RSP,              //��Ϣ��: CRcsDelNetFSCfgRsp

	RCS_SET_DISK_USAGE_REQ,              //��Ϣ��: CRcsSetDiskUsageReq
	RCS_SET_DISK_USAGE_RSP,              //��Ϣ��: CRcsSetDiskUsageRsp

	RCS_GET_NETFS_EXPORT_LIST_REQ,      //��Ϣ��: CRcsGetNetFSExportListReq
	RCS_GET_NETFS_EXPORT_LIST_RSP,      //��Ϣ��: CRcsGetNetFSExportListRsp

    //iscsi�������
    RCS_GET_ISCSI_CONF_REQ,             //��Ϣ��: CRcsGetIscsiConfReq
    RCS_GET_ISCSI_CONF_RSP,             //��Ϣ��: CRcsGetIscsiConfRsp

    RCS_ADD_ISCSI_CONF_REQ,             //��Ϣ��: CRcsAddIscsiConfReq
    RCS_ADD_ISCSI_CONF_RSP,             //��Ϣ��: CRcsAddIscsiConfRsp

    RCS_DEL_ISCSI_CONF_REQ,             //��Ϣ��: CRcsDelIscsiConfReq
    RCS_DEL_ISCSI_CONF_RSP,             //��Ϣ��: CRcsDelIscsiConfRsp

    RCS_CTRL_ISCSI_REQ,                 //��Ϣ��: CRcsCtrlIscsiReq
    RCS_CTRL_ISCSI_RSP,                 //��Ϣ��: CRcsCtrlIscsiRsp

    RCS_DISCONNECT_ISCSI_REQ,           //��Ϣ��: CRcsDisconnectIscsiReq
    RCS_DISCONNECT_ISCSI_RSP,           //��Ϣ��: CRcsDisconnectIscsiRsp

    //��ȡ������Ϣ
    RCS_GET_ARRAY_INFO_REQ,             //��Ϣ��: CRcsGetArrayInfoReq
    RCS_GET_ARRAY_INFO_RSP,             //��Ϣ��: CRcsGetArrayInfoRsp

    //��ȡ��������Ϣ
    RCS_GET_DISK_GROUP_INFO_REQ,        //��Ϣ��: CRcsGetDiskGroupInfoReq
    RCS_GET_DISK_GROUP_INFO_RSP,        //��Ϣ��: CRcsGetDiskGroupInfoRsp

    //��ȡ���������Ϣ��Ϣ
    RCS_GET_PHYSICAL_DISK_INFO_REQ,     //��Ϣ��: CRcsGetPhysicalDiskInfoReq
    RCS_GET_PHYSICAL_DISK_INFO_RSP,     //��Ϣ��: CRcsGetPhysicalDiskInfoRsp

    //��ȡ���������Ϣ
    RCS_GET_VIRTUAL_DISK_INFO_REQ,      //��Ϣ��: CRcsGetVirtualDiskInfoReq
    RCS_GET_VIRTUAL_DISK_INFO_RSP,      //��Ϣ��: CRcsGetVirtualDiskInfoRsp

    //��ȡiscsi�ķ�������
    RCS_GET_ISCSI_INITIATORNAME_REQ,    //��Ϣ�壺CRcsGetIscsiInitatorNameReq
    RCS_GET_ISCSI_INITIATORNAME_RSP,    //��Ϣ�壺CRcsGetIscsiInitatorNameRsp

    //����TaskStart��Ϣ 
    RCS_TASK_START_REQ,                 //��Ϣ�壺CRcsTaskStartReq
    RCS_TASK_START_RSP,                 //��Ϣ�壺CRcsTaskStartRsp
									    
    //��ȡ�⳧�̷�����Ϣ 			   
    RCS_GET_USED_OTHER_STORAGE_SIZE_REQ,//��Ϣ�壺CRcsGetUsedOtherStorageSizeReq
    RCS_GET_USED_OTHER_STORAGE_SIZE_RSP,//��Ϣ�壺CRcsGetUsedOtherStorageSizeRsp
									    
    RCS_CURRENT_EVENT_END,
};

inline void InitRcsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(RCS_SET_SYSPARAM_REQ);                    //��Ϣ��: CRcsSetSysParamReq
    OSP_ADD_EVENT_DESC(RCS_SET_SYSPARAM_RSP);                    //��Ϣ��: CRcsSetSysParamRsp
    OSP_ADD_EVENT_DESC(RCS_GET_SYSPARAM_REQ);                    //��Ϣ��: CRcsGetSysParamReq
    OSP_ADD_EVENT_DESC(RCS_GET_SYSPARAM_RSP);                    //��Ϣ��: CRcsGetSysParamRsp
    OSP_ADD_EVENT_DESC(RCS_BINDING_PUCHN_REQ);                   //��Ϣ��: CRcsBindingPuChnReq
    OSP_ADD_EVENT_DESC(RCS_BINDING_PUCHN_RSP);                   //��Ϣ��: CRcsBindingPuChnRsp
    OSP_ADD_EVENT_DESC(RCS_UNBINDING_PUCHN_REQ);                 //��Ϣ��: CRcsUnbindingPuChnReq
    OSP_ADD_EVENT_DESC(RCS_UNBINDING_PUCHN_RSP);                 //��Ϣ��: CRcsUnbindingPuChnRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_REQ);             //��Ϣ��: CRcsQueryPuChnBindingReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_RSP);             //��Ϣ��: CRcsQueryPuChnBindingRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_NTF_REQ);         //��Ϣ��: CRcsQueryPuChnBindingNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_PUCHN_BINDING_NTF_RSP);         //��Ϣ��: CRcsQueryPuChnBindingNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_PARAM_REQ);                //��Ϣ��: CRcsSetRecordParamReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_PARAM_RSP);                //��Ϣ��: CRcsSetRecordParamRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_REQ);              //��Ϣ��: CRcsQueryRecordParamReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_RSP);              //��Ϣ��: CRcsQueryRecordParamRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_NTF_REQ);          //��Ϣ��: CRcsQueryRecordParamNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_PARAM_NTF_RSP);          //��Ϣ��: CRcsQueryRecordParamNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_TASK_REQ);                 //��Ϣ��: CRcsSetRecordTaskReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_TASK_RSP);                 //��Ϣ��: CRcsSetRecordTaskRsp
    OSP_ADD_EVENT_DESC(RCS_LOCK_RECORD_REQ);                     //��Ϣ��: CRcsLockRecordReq
    OSP_ADD_EVENT_DESC(RCS_LOCK_RECORD_RSP);                     //��Ϣ��: CRcsLockRecordRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_REQ);                    //��Ϣ��: CRcsQueryRecordReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_RSP);                    //��Ϣ��: CRcsQueryRecordRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_NTF_REQ);                //��Ϣ��: CRcsQueryRecordNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_NTF_RSP);                //��Ϣ��: CRcsQueryRecordNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DELETE_RECORD_REQ);                   //��Ϣ�壺CRcsDeleteRecordReq
    OSP_ADD_EVENT_DESC(RCS_DELETE_RECORD_RSP);                   //��Ϣ�壺CRcsDeleteRecordRsp
    OSP_ADD_EVENT_DESC(RCS_REC_DEL_REQ);                         //��Ϣ��: CRcsRecDelReq
    OSP_ADD_EVENT_DESC(RCS_REC_DEL_RSP);                         //��Ϣ��: CRcsRecDelRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_DAYS_REQ);               //��Ϣ��: CRcsQueryRecordDaysReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECORD_DAYS_RSP);               //��Ϣ��: CRcsQueryRecordDaysRsp
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_ALIAS_REQ);                //��Ϣ�壺CRcsSetRecordAliasReq
    OSP_ADD_EVENT_DESC(RCS_SET_RECORD_ALIAS_RSP);                //��Ϣ�壺CRcsSetRecordAliasRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_RECTAG_REQ);                      //��Ϣ��: CRcsAddRecTagReq
    OSP_ADD_EVENT_DESC(RCS_ADD_RECTAG_RSP);                      //��Ϣ��: CRcsAddRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_RECTAG_REQ);                      //��Ϣ��: CRcsDelRecTagReq
    OSP_ADD_EVENT_DESC(RCS_DEL_RECTAG_RSP);                      //��Ϣ��: CRcsDelRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_UPDATE_RECTAG_REQ);                   //��Ϣ��: CRcsUpdateRecTagReq
    OSP_ADD_EVENT_DESC(RCS_UPDATE_RECTAG_RSP);                   //��Ϣ��: CRcsUpdateRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_REQ);                    //��Ϣ��: CRcsQueryRecTagReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_RSP);                    //��Ϣ��: CRcsQueryRecTagRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_NTF_REQ);                //��Ϣ��: CRcsQueryRecTagNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_RECTAG_NTF_RSP);                //��Ϣ��: CRcsQueryRecTagNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_PU_STATUS_REQ);             //��Ϣ��: CRcsSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_PU_STATUS_RSP);             //��Ϣ��: CRcsSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_PU_STATUS_REQ);           //��Ϣ��: CRcsUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_PU_STATUS_RSP);           //��Ϣ��: CRcsUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_PU_RECORD_STATUS_NTF_REQ);            //��Ϣ��: CRcsPuRecordStatusNtyReq
    OSP_ADD_EVENT_DESC(RCS_PU_RECORD_STATUS_NTF_RSP);            //��Ϣ��: CRcsPuRecordStatusNtyRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_SUBSCRIBE_PU_STATUS_REQ);        //��Ϣ��: CRcsListSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_SUBSCRIBE_PU_STATUS_RSP);        //��Ϣ��: CRcsListSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_REFRESH_SS_PU_STATUS_REQ);       //��Ϣ��: CRcsListRefreshSsPuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_REFRESH_SS_PU_STATUS_RSP);       //��Ϣ��: CRcsListRefreshSsPuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_LIST_UNSUBSCRIBE_PU_STATUS_REQ);      //��Ϣ��: CRcsListUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_LIST_UNSUBSCRIBE_PU_STATUS_RSP);      //��Ϣ��: CRcsListUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ);     //��Ϣ��: CRcsCatalogSubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP);     //��Ϣ��: CRcsCatalogSubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ);    //��Ϣ��: CRcsCatalogRefreshSsPuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP);    //��Ϣ��: CRcsCatalogRefreshSsPuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ);   //��Ϣ��: CRcsCatalogUnsubscribePuStatusReq
    OSP_ADD_EVENT_DESC(RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP);   //��Ϣ��: CRcsCatalogUnsubscribePuStatusRsp
    OSP_ADD_EVENT_DESC(RCS_RECORD_LOCK_OPR_REQ);                 //��Ϣ��: CRcsRecordLockOprReq
    OSP_ADD_EVENT_DESC(RCS_RECORD_LOCK_OPR_RSP);                 //��Ϣ��: CRcsRecordLockOprRsp
    OSP_ADD_EVENT_DESC(RCS_START_DISCOVERY_NRU_REQ);             //��Ϣ��: CRcsStartDiscoveryNruReq
    OSP_ADD_EVENT_DESC(RCS_START_DISCOVERY_NRU_RSP);             //��Ϣ��: CRcsStartDiscoveryNruRsp
    OSP_ADD_EVENT_DESC(RCS_STOP_DISCOVERY_NRU_REQ);              //��Ϣ��: CRcsStopDiscoveryNruReq
    OSP_ADD_EVENT_DESC(RCS_STOP_DISCOVERY_NRU_RSP);              //��Ϣ��: CRcsStopDiscoveryNruRsp
    OSP_ADD_EVENT_DESC(RCS_DISCOVERY_NRU_NTF_REQ);               //��Ϣ��: CRcsDiscoveryNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISCOVERY_NRU_NTF_RSP);               //��Ϣ��: CRcsDiscoveryNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_NRU_REQ);                         //��Ϣ��: CRcsAddNruReq
    OSP_ADD_EVENT_DESC(RCS_ADD_NRU_RSP);                         //��Ϣ��: CRcsAddNruRsp
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_REQ);                         //��Ϣ��: CRcsModNruReq
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_RSP);                         //��Ϣ��: CRcsModNruRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_NRU_REQ);                         //��Ϣ��: CRcsDelNruReq
    OSP_ADD_EVENT_DESC(RCS_DEL_NRU_RSP);                         //��Ϣ��: CRcsDelNruRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_REQ);                       //��Ϣ��: CRcsQueryNruReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_RSP);                       //��Ϣ��: CRcsQueryNruRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_NTF_REQ);                   //��Ϣ��: CRcsQueryNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_NRU_NTF_RSP);                   //��Ϣ��: CRcsQueryNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ);    //��Ϣ��: CRcsQueryOnlineNruDetailInfoReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP);    //��Ϣ��: CRcsQueryOnlineNruDetailInfoRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ);//��Ϣ��: CRcsQueryOnlineNruDetailInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP);//��Ϣ��: CRcsQueryOnlineNruDetailInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_NRU_REQ);                   //��Ϣ��: CRcsSubscribeNruReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_NRU_RSP);                   //��Ϣ��: CRcsSubscribeNruRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_NRU_REQ);                 //��Ϣ��: CRcsUnsubscribeNruReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_NRU_RSP);                 //��Ϣ��: CRcsUnsubscribeNruRsp
    OSP_ADD_EVENT_DESC(RCS_NRU_NTF_REQ);                         //��Ϣ��: CRcsNruNtfReq
    OSP_ADD_EVENT_DESC(RCS_NRU_NTF_RSP);                         //��Ϣ��: CRcsNruNtfRsp
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_NETWORK_REQ);                 //��Ϣ��: CRcsModNruNetworkReq
    OSP_ADD_EVENT_DESC(RCS_MOD_NRU_NETWORK_RSP);                 //��Ϣ��: CRcsModNruNetworkRsp
    OSP_ADD_EVENT_DESC(RCS_FDISK_REQ);                           //��Ϣ��: CRcsFdiskReq
    OSP_ADD_EVENT_DESC(RCS_FDISK_RSP);                           //��Ϣ��: CRcsFdiskRsp
    OSP_ADD_EVENT_DESC(RCS_FORMAT_DISK_REQ);                     //��Ϣ��: CRcsFormatDiskReq
    OSP_ADD_EVENT_DESC(RCS_FORMAT_DISK_RSP);                     //��Ϣ��: CRcsFormatDiskRsp
    OSP_ADD_EVENT_DESC(RCS_CHECK_DISK_REQ);                      //��Ϣ��: CRcsCheckDiskReq
    OSP_ADD_EVENT_DESC(RCS_CHECK_DISK_RSP);                      //��Ϣ��: CRcsCheckDiskRsp
    OSP_ADD_EVENT_DESC(RCS_MOUNT_DISK_REQ);                      //��Ϣ��: CRcsMountDiskReq
    OSP_ADD_EVENT_DESC(RCS_MOUNT_DISK_RSP);                      //��Ϣ��: CRcsMountDiskRsp
    OSP_ADD_EVENT_DESC(RCS_UMOUNT_DISK_REQ);                     //��Ϣ��: CRcsUnmountDiskReq
    OSP_ADD_EVENT_DESC(RCS_UMOUNT_DISK_RSP);                     //��Ϣ��: CRcsUnmountDiskRsp
    OSP_ADD_EVENT_DESC(RCS_CANCEL_DISK_OP_REQ);                  //��Ϣ��: CRcsCancleDiskOpReq
    OSP_ADD_EVENT_DESC(RCS_CANCEL_DISK_OP_RSP);                  //��Ϣ��: CRcsCancleDiskOpRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_OFFLINE_DISK_REQ);                //��Ϣ��: CRcsDelOfflineDiskReq
    OSP_ADD_EVENT_DESC(RCS_DEL_OFFLINE_DISK_RSP);                //��Ϣ��: CRcsDelOfflineDiskRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_REQ);                 //��Ϣ��: CRcsQueryDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_RSP);                 //��Ϣ��: CRcsQueryDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_NTF_REQ);             //��Ϣ��: CRcsQueryDiskInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_INFO_NTF_RSP);             //��Ϣ��: CRcsQueryDiskInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_DISKINFO_REQ);              //��Ϣ��: CRcsSubscribeDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_SUBSCRIBE_DISKINFO_RSP);              //��Ϣ��: CRcsSubscribeDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_DISKINFO_REQ);            //��Ϣ��: CRcsUnsubscribeDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_UNSUBSCRIBE_DISKINFO_RSP);            //��Ϣ��: CRcsUnsubscribeDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_REFRESH_SS_DISKINFO_REQ);             //��Ϣ��: CRcsRefreshSsNruDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_REFRESH_SS_DISKINFO_RSP);             //��Ϣ��: CRcsRefreshSsNruDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_INFO_NTF_REQ);                   //��Ϣ��: CRcsDiskInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_INFO_NTF_RSP);                   //��Ϣ��: CRcsDiskInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_INFO_NTF_REQ);              //��Ϣ��: CRcsDiskPartInfoNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_INFO_NTF_RSP);              //��Ϣ��: CRcsDiskPartInfoNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_STATE_NTF_REQ);                  //��Ϣ��: CRcsDiskStateNtfReq  [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_DISK_STATE_NTF_RSP);                  //��Ϣ��: CRcsDiskStateNtfRsp  [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_STATE_NTF_REQ);             //��Ϣ��: CRcsDiskPartStateNtfReq  [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_STATE_NTF_RSP);             //��Ϣ��: CRcsDiskPartStateNtfRsp  [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_DISK_OP_STATE_NTF_REQ);               //��Ϣ��: CRcsDiskOpStateNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_OP_STATE_NTF_RSP);               //��Ϣ��: CRcsDiskOpStateNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_IOUTIL_NTF_REQ);                 //��Ϣ��: CRcsDiskIoUtilNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_IOUTIL_NTF_RSP);                 //��Ϣ��: CRcsDiskIoUtilNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_ALARM_NTF_REQ);             //��Ϣ�壺CRcsDiskPartAlarmNtfReq
    OSP_ADD_EVENT_DESC(RCS_DISK_PART_ALARM_NTF_RSP);             //��Ϣ�壺CRcsDiskPartAlarmNtfRsp
    OSP_ADD_EVENT_DESC(RCS_DISK_ABN_NTF_REQ);                    //��Ϣ��: CRcsDiskAbnNtfReq    [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_DISK_ABN_NTF_RSP);                    //��Ϣ��: CRcsDiskAbnNtfRsp    [Ŀǰ������δʹ��]
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_REQ);                  //��Ϣ��: CRcsQueryDiskLogReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_RSP);                  //��Ϣ��: CRcsQueryDiskLogRsp
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_NTF_REQ);              //��Ϣ��: CRcsQueryDiskLogNtfReq
    OSP_ADD_EVENT_DESC(RCS_QUERY_DISK_LOG_NTF_RSP);              //��Ϣ��: CRcsQueryDiskLogNtfRsp
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_CFG_REQ);                   //��Ϣ��: CRcsGetNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_CFG_RSP);                   //��Ϣ��: CRcsGetNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_NETFS_CFG_REQ);                   //��Ϣ��: CRcsAddNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_ADD_NETFS_CFG_RSP);                   //��Ϣ��: CRcsAddNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_NETFS_CFG_REQ);                   //��Ϣ��: CRcsDelNetFSCfgReq
    OSP_ADD_EVENT_DESC(RCS_DEL_NETFS_CFG_RSP);                   //��Ϣ��: CRcsDelNetFSCfgRsp
    OSP_ADD_EVENT_DESC(RCS_SET_DISK_USAGE_REQ);                  //��Ϣ��: CRcsSetDiskUsageReq
    OSP_ADD_EVENT_DESC(RCS_SET_DISK_USAGE_RSP);                  //��Ϣ��: CRcsSetDiskUsageRsp
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_EXPORT_LIST_REQ);           //��Ϣ��: CRcsGetNetFSExportListReq
    OSP_ADD_EVENT_DESC(RCS_GET_NETFS_EXPORT_LIST_RSP);           //��Ϣ��: CRcsGetNetFSExportListRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_CONF_REQ);                  //��Ϣ��: CRcsGetIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_CONF_RSP);                  //��Ϣ��: CRcsGetIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_ADD_ISCSI_CONF_REQ);                  //��Ϣ��: CRcsAddIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_ADD_ISCSI_CONF_RSP);                  //��Ϣ��: CRcsAddIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_DEL_ISCSI_CONF_REQ);                  //��Ϣ��: CRcsDelIscsiConfReq
    OSP_ADD_EVENT_DESC(RCS_DEL_ISCSI_CONF_RSP);                  //��Ϣ��: CRcsDelIscsiConfRsp
    OSP_ADD_EVENT_DESC(RCS_CTRL_ISCSI_REQ);                      //��Ϣ��: CRcsCtrlIscsiReq
    OSP_ADD_EVENT_DESC(RCS_CTRL_ISCSI_RSP);                      //��Ϣ��: CRcsCtrlIscsiRsp
    OSP_ADD_EVENT_DESC(RCS_DISCONNECT_ISCSI_REQ);                //��Ϣ��: CRcsDisconnectIscsiReq
    OSP_ADD_EVENT_DESC(RCS_DISCONNECT_ISCSI_RSP);                //��Ϣ��: CRcsDisconnectIscsiRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ARRAY_INFO_REQ);                  //��Ϣ��: CRcsGetArrayInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_ARRAY_INFO_RSP);                  //��Ϣ��: CRcsGetArrayInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_DISK_GROUP_INFO_REQ);             //��Ϣ��: CRcsGetDiskGroupInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_DISK_GROUP_INFO_RSP);             //��Ϣ��: CRcsGetDiskGroupInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_PHYSICAL_DISK_INFO_REQ);          //��Ϣ��: CRcsGetPhysicalDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_PHYSICAL_DISK_INFO_RSP);          //��Ϣ��: CRcsGetPhysicalDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_VIRTUAL_DISK_INFO_REQ);           //��Ϣ��: CRcsGetVirtualDiskInfoReq
    OSP_ADD_EVENT_DESC(RCS_GET_VIRTUAL_DISK_INFO_RSP);           //��Ϣ��: CRcsGetVirtualDiskInfoRsp
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_INITIATORNAME_REQ);         //��Ϣ�壺CRcsGetIscsiInitatorNameReq
    OSP_ADD_EVENT_DESC(RCS_GET_ISCSI_INITIATORNAME_RSP);         //��Ϣ�壺CRcsGetIscsiInitatorNameRsp
    OSP_ADD_EVENT_DESC(RCS_TASK_START_REQ);                      //��Ϣ�壺CRcsTaskStartReq
    OSP_ADD_EVENT_DESC(RCS_TASK_START_RSP);                      //��Ϣ�壺CRcsTaskStartRsp
    OSP_ADD_EVENT_DESC(RCS_GET_USED_OTHER_STORAGE_SIZE_REQ);     //��Ϣ�壺CRcsGetUsedOtherStorageSizeReq
    OSP_ADD_EVENT_DESC(RCS_GET_USED_OTHER_STORAGE_SIZE_RSP);     //��Ϣ�壺CRcsGetUsedOtherStorageSizeRsp
    OSP_ADD_EVENT_DESC(RCS_CURRENT_EVENT_END);                   
}


#endif  //#ifndef __RCS_EVENT_H__





























