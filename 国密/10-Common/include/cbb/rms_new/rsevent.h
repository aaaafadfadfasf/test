#ifndef _RS_EVENT_H_
#define _RS_EVENT_H_

#define EV_RS_BASE                 10000

#define EV_RS_INNER_BASE           (EV_RS_BASE)
#define EV_RS_OUTER_BASE           (EV_RS_BASE + 20000)

/************************************************************************/
/*       录像服务器各模块内部的消息范围定义                             */
/************************************************************************/

/*录像系统的SDK内部消息范围*/
#define EV_RMSSDK_BEGIN             (EV_RS_BASE + 1)
#define EV_RMSSDK_END               (EV_RMSSDK_BEGIN + 999)

/*录像管理服务器RMS内部的消息范围*/
#define EV_RMS_BEGIN               (EV_RMSSDK_END + 1)
#define EV_RMS_END                 (EV_RMS_BEGIN + 999)

/*录像机NRU内部消息范围*/
#define EV_NRU_BEGIN               (EV_RMS_END + 1)
#define EV_NRU_END                 (EV_NRU_BEGIN + 999)

/*录像管理客户端RMC内部的消息范围*/
#define EV_RMC_BEGIN               (EV_NRU_END + 1)
#define EV_RMC_END                 (EV_RMC_BEGIN + 999)

/************************************************************************/
/*       录像服务器模块之间消息范围定义                                 */
/************************************************************************/

/*SDK与RMS之间的消息*/
#define EV_RMSSDK_RMS_BEGIN                  (EV_RS_OUTER_BASE + 1)
#define EV_RMSSDK_RMS_END                    (EV_RMSSDK_RMS_BEGIN + 999)

/*RMS与NRU之间的消息*/
#define EV_RMS_NRU_BEGIN                  (EV_RMSSDK_RMS_END + 1)
#define EV_RMS_NRU_END                    (EV_RMS_NRU_BEGIN + 999)

/*RMC与RMS之间的消息*/
#define EV_RMC_RMS_BEGIN                  (EV_RMS_NRU_END + 1)
#define EV_RMC_RMS_END                    (EV_RMC_RMS_BEGIN + 999)


typedef enum
{
	RMSSDK_RMS_REG_REQ          = EV_RMSSDK_RMS_BEGIN,
	RMS_RMSSDK_REG_RSP,

	RMSSDK_RMS_DISCOVERY_NRU_CMD,                  /*无*/
	RMS_RMSSDK_NRUINFO_NOTIFY,                     /*TRSComm_Num + TRSComm_DiscoveryNruInfo*/

	RMSSDK_RMS_ADD_NRU_REQ,                        /*TRSComm_NruInfo*/
	RMS_RMSSDK_ADD_NRU_RSP,                        /*TRSComm_NruInfo*/

	RMSSDK_RMS_CHANGE_NRUINFO_REQ,                 /*TRSComm_ChangeNruInfo*/
	RMS_RMSSDK_CHANGE_NRUINFO_RSP,                 /*TRSComm_ChangeNruInfo*/

	RMSSDK_RMS_DEL_NRU_REQ,                        /*TRSComm_DevID*/
	RMS_RMSSDK_DEL_NRU_RSP,                        /*TRSComm_DevID*/

	RMSSDK_RMS_MODIFY_NRU_NETWORK_REQ,             /*TRSComm_DevID + TRSComm_NetWork*/
	RMS_RMSSDK_MODIFY_NRU_NETWORK_RSP,             /*TRSComm_DevID + TRSComm_NetWork*/

	RMSSDK_RMS_GET_ALLNRUINFO_REQ,                 /*无*/
	RMS_RMSSDK_GET_ALLNRUINFO_RSP,                 /*TRSComm_Num*/
	RMS_RMSSDK_GET_ALLNRUINFO_NOTIFY,              /*TRSComm_QueryResultNtfHeader + TRSComm_NruInfo列表*/

	RMS_RMSSDK_NRUONLINE_NOTIFY,					/*TRSComm_DevBaseInfo + TRSComm_MediaStreamInfo + TRSComm_NetWork*/
	RMS_RMSSDK_NRUOFFLINE_NOTIFY,					/*TRSComm_DevID*/

	RMSSDK_RMS_GET_NRUSYSSTATUS_REQ,                /*TRSComm_DevID*/
	RMS_RMSSDK_GET_NRUSYSSTATUS_RSP,                /*TRSComm_DevID + TRSComm_SysStatusParam*/

	RMS_RMSSDK_PARTITIONSINFO_NOTIFY,               /*TRSComm_Num + n*TRSComm_PartitionInfo*/
	RMS_RMSSDK_PARTITION_OFFLINE_NOTIFY,            /*TRSComm_Num + n*TRSComm_PartitionInfo*/

	RMSSDK_RMS_SET_SYSPARAM_REQ,					/*TRSComm_RecSysParam*/
	RMS_RMSSDK_SET_SYSPARAM_RSP,					/*TRSComm_RecSysParam*/

	RMSSDK_RMS_QUERY_SYSPARAM_REQ,
	RMS_RMSSDK_QUERY_SYSPARAM_RSP,					/*TRSComm_RecSysParam*/

	RMSSDK_RMS_BINDING_PUCHN_REQ,					/*TRSComm_PuChnBindingHeader + TRSComm_DevChn列表*/
	RMS_RMSSDK_BINDING_PUCHN_RSP,					/*TRSComm_PuChnBindingHeader + TRSComm_DevChn列表*/

	RMSSDK_RMS_UNBINDING_PUCHN_REQ,					/*TRSComm_PuChnUnBindingHeader + TRSComm_DevChn列表*/
	RMS_RMSSDK_UNBINDING_PUCHN_RSP,					/*TRSComm_PuChnUnBindingHeader + TRSComm_DevChn列表*/

	RMSSDK_RMS_QUERY_PUCHN_BINDINGRELATIVE_REQ,     /*TRSComm_PuChnBindingRelativeQueryCond*/
	RMS_RMSSDK_QUERY_PUCHN_BINDINGRELATIVE_RSP,     /*TRSComm_QueryResultBaseInfo*/
	RMS_RMSSDK_QUERY_PUCHN_BINDINGRELATIVE_NOTIFY,  /*TRSComm_QueryResultNtfHeader + TRSComm_PuChnBindingRelative列表*/

	/*录像*/
	RMSSDK_RMS_ALLOC_RECRESOURCE_REQ,			 /*TRSComm_RecordParam + TRSComm_MediaInfoExt*/
	RMS_RMSSDK_ALLOC_RECRESOURCE_RSP,			 /*TRSComm_RecordParam + TRSComm_TransParamHeader  + TRSComm_MediaAddr列表*/

	RMSSDK_RMS_RELEASE_RECRESOURCE_REQ,			 /*TRSComm_Handle*/
	RMS_RMSSDK_RELEASE_RECRESOURCE_RSP,			 /*TRSComm_Handle*/

	RMSSDK_RMS_START_RECORD_REQ,				 /*TRSComm_Handle + TRSComm_TransParamHeader  + TRSComm_MediaAddr列表*/
	RMS_RMSSDK_START_RECORD_RSP,				 /*TRSComm_Handle + TRSComm_TransParamHeader  + TRSComm_MediaAddr列表*/

	RMSSDK_RMS_STOP_RECORD_REQ,					 /*TRSComm_Handle*/
	RMS_RMSSDK_STOP_RECORD_RSP,					 /*TRSComm_Handle*/

	RMS_RMSSDK_RECORD_STOPPED_NOTIFY,            /*TRSComm_Handle*/

	RMSSDK_RMS_QUERY_RECORD_REQ,				 /*TRSComm_RecQueryCond*/
	RMS_RMSSDK_QUERY_RECORD_RSP,				 /*TRSComm_Num（录像文件总数，即 TRSComm_RecFileDetailedInfo 结构体个数）*/
	RMS_RMSSDK_QUERY_RECORD_NOTIFY,				 /*TRSComm_QueryResultNtfHeader + TRSComm_RecFileDetailedInfo + TRSComm_RecFileSubTimeRangeInfo列表 + TRSComm_RecFileDetailedInfo + TRSComm_RecFileSubTimeRangeInfo列表...*/

	RMSSDK_RMS_UPDATE_RECTYPE_REQ,				 /*TRSComm_Handle + TRSComm_RecTypeChange*/
	RMS_RMSSDK_UPDATE_RECTYPE_RSP,				 /*TRSComm_Handle + TRSComm_RecTypeChange*/

	RMSSDK_RMS_ADD_RECTAG_REQ,                   /*TRSComm_RecTagAdd*/
	RMS_RMSSDK_ADD_RECTAG_RSP,                   /*TRSComm_RecTagAdd*/

	RMSSDK_RMS_DEL_RECTAG_REQ,                   /*TRSComm_RecTagDel*/
	RMS_RMSSDK_DEL_RECTAG_RSP,                   /*TRSComm_RecTagDel*/

	RMSSDK_RMS_UPDATE_RECTAG_REQ,                /*TRSComm_RecTagUpdate*/
	RMS_RMSSDK_UPDATE_RECTAG_RSP,                /*TRSComm_RecTagUpdate*/

	RMSSDK_RMS_QUERY_RECTAG_REQ,                 /*TRSComm_RecTagQueryCond*/
	RMS_RMSSDK_QUERY_RECTAG_RSP,                 /*TRSComm_Num*/
	RMS_RMSSDK_QUERY_RECTAG_NOTIFY,              /*TRSComm_QueryResultNtfHeader + TRSComm_RecTag 列表*/

	RMSSDK_RMS_QUERY_RECDAYS_REQ,                /*TRSComm_Num(查询的前端通道数量) + TRSComm_DevChn 列表 + TRSComm_TimeRange(查询的时间范围)*/
	RMS_RMSSDK_QUERY_RECDAYS_RSP,                /*TRSComm_Num(TRSComm_OnePuChnRecDays结构体个数) + TRSComm_OnePuChnRecDays 列表 + TRSComm_TimeRange(查询的时间范围)*/

	/*放像*/
	RMSSDK_RMS_ALLOC_PLAYRESOURCE_REQ,			 /*TRSComm_PlayParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	RMS_RMSSDK_ALLOC_PLAYRESOURCE_RSP,			 /*TRSComm_PlayParam  + TRSComm_TransParamHeader  + TRSComm_MediaAddr列表
																	  + TRSComm_MediaInfoDescription + TRSComm_AudioEncParam(..) + TRSComm_VideoEncParam(..)  + TRSComm_MediaInfoExt*/
	
	RMSSDK_RMS_RELEASE_PLAYRESOURCE_REQ,		 /*TRSComm_Handle*/
	RMS_RMSSDK_RELEASE_PLAYRESOURCE_RSP,		 /*TRSComm_Handle*/
	
	RMSSDK_RMS_START_PLAY_REQ,					 /*TRSComm_Handle */
	RMS_RMSSDK_START_PLAY_RSP,					 /*TRSComm_Handle*/
	
	RMSSDK_RMS_STOP_PLAY_REQ,					 /*TRSComm_Handle*/
	RMS_RMSSDK_STOP_PLAY_RSP,					 /*TRSComm_Handle*/

	RMS_RMSSDK_STOPPED_PLAY_NOTIFY,                 /*TRSComm_Handle*/
	RMSSDK_RMS_VCRCTRL_PLAY_REQ,                 /*TRSComm_Handle + TRSComm_VCRCtrl*/
	RMS_RMSSDK_VCRCTRL_PLAY_RSP,                 /*TRSComm_Handle + TRSComm_VCRCtrl*/

	RMS_RMSSDK_PLAYPROG_NOTIFY,                  /*TRSComm_PlayProg*/

	/*下载*/
	RMSSDK_RMS_ALLOC_DOWNLOADRESOURCE_REQ,		 /*TRSComm_DownloadParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	RMS_RMSSDK_ALLOC_DOWNLOADRESOURCE_RSP,		 /*TRSComm_DownloadParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表 + TRSComm_MediaInfoExt*/
	
	RMSSDK_RMS_RELEASE_DOWNLOADRESOURCE_REQ,	 /*TRSComm_Handle*/
	RMS_RMSSDK_RELEASE_DOWNLOADRESOURCE_RSP,	 /*TRSComm_Handle*/
	
	RMSSDK_RMS_START_DOWNLOAD_REQ,               /*TRSComm_Handle*/
	RMS_RMSSDK_START_DOWNLOAD_RSP,               /*TRSComm_Handle*/
	
	RMSSDK_RMS_STOP_DOWNLOAD_REQ,                /*TRSComm_Handle*/
	RMS_RMSSDK_STOP_DOWNLOAD_RSP,                /*TRSComm_Handle*/

	RMSSDK_RMS_SET_IPADDROFSTUNSRV_REQ,           /*TRSComm_NetAddr*/
	RMS_RMSSDK_SET_IPADDROFSTUNSRV_RSP,           /*TRSComm_NetAddr*/
	
	RMS_RMSSDK_DEVICE_ALARM_NOTIFY,               /*TRSComm_DeviceAlarm*/

	RMSSDK_RMS_DELETE_RECORD_REQ,                 /*TRSComm_RecFileSchema*/
	RMS_RMSSDK_DELETE_RECORD_RSP,                 /*TRSComm_RecFileSchema*/

	RMSSDK_RMS_REC_LOCK_REQ,                      /*TRSComm_RecLock*/
	RMS_RMSSDK_REC_LOCK_RSP,                      /*TRSComm_RecLock*/

	RMSSDK_RMS_SET_FILEALIAS_REQ,                 /*TRSComm_RecFileSchema*/
	RMS_RMSSDK_SET_FILEALIAS_RSP,                 /*TRSComm_RecFileSchema*/

	RMSSDK_RMS_UPDATE_OTHER_STORAGE_LICENSE_REQ,  /*TRSComm_OtherStorageLicense*/
	RMS_RMSSDK_UPDATE_OTHER_STORAGE_LICENSE_RSP,  /*TRSComm_OtherStorageLicense*/

	RMSSDK_RMS_REC_DEL_EX_REQ,                    /*TRSComm_RecDelCond*/
	RMS_RMSSDK_REC_DEL_EX_RSP,                    /*TRSComm_RecDelCond*/

}ESdkRmsMsg;

typedef enum
{
	NRU_RMS_REG_REQ  = EV_RMS_NRU_BEGIN,                /*TRSComm_NruRegInfo + TRSComm_Num + N * TRSComm_NetSegConfig*/
	RMS_NRU_REG_RSP,

	RMS_NRU_TIMESYN_CMD,                                /*TRSComm_Time*/

	RMS_NRU_GET_NRUSYSSTATUS_REQ,                       /*TRSComm_DevID*/
	NRU_RMS_GET_NRUSYSSTATUS_RSP,                       /*TRSComm_DevID + TRSComm_SysStatusParam*/

	NRU_RMS_PARTITIONSINFO_NOTIFY,                      /*TRSComm_Num + n*TRSComm_PartitionInfo*/
	NRU_RMS_PARTITION_OFFLINE_NOTIFY,                   /*TRSComm_PartitionInfo*/

	/*录像*/
	RMS_NRU_ALLOC_RECRESOURCE_REQ,                      /*TRSComm_RecordTask + TRSComm_MediaInfoExt*/
	NRU_RMS_ALLOC_RECRESOURCE_RSP,                      /*TRSComm_RecordTask + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	
	RMS_NRU_RELEASE_RECRESOURCE_REQ,                    /*TRSComm_RecordTask*/
	NRU_RMS_RELEASE_RECRESOURCE_RSP,                    /*TRSComm_RecordTask*/
  
	RMS_NRU_START_RECORD_REQ,                           /*TRSComm_RecordTask + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	NRU_RMS_START_RECORD_RSP,							/*TRSComm_RecordTask + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/

	RMS_NRU_STOP_RECORD_REQ,                            /*TRSComm_Handle*/
	NRU_RMS_STOP_RECORD_RSP,                            /*TRSComm_Handle*/
	
	NRU_RMS_RECORD_STARTED_NOTIFY,                       /*TRSComm_Handle*/

	NRU_RMS_RECORD_STOPPED_NOTIFY,                      /*TRSComm_Handle*/

	NRU_RMS_RECORD_STATUS_NOTIFY,                       /*TRSComm_Num + n * TRSComm_RecordStatus*/

	RMS_NRU_START_SWITCH_BIG_REC_FILE_NOTIFY,           /*TRSComm_Handle*/
	NRU_RMS_END_SWITCH_BIG_REC_FILE_NOTIFY,             /*TRSComm_RecordTask + TRSComm_TimeRange*/

	RMS_NRU_DEL_RECORD_REQ,                             /*TRSComm_RecFileSchema*/
	NRU_RMS_DEL_RECORD_RSP,                             /*TRSComm_RecFileSchema*/

	NRU_RMS_DEL_VBFS_RECORD_REQ,                        /*TRSComm_RecFileSchema*/
	RMS_NRU_DEL_VBFS_RECORD_RSP,                        /*TRSComm_RecFileSchema*/

	/*放像*/
	RMS_NRU_ALLOC_PLAYRESOURCE_REQ,                     /*TRSComm_PlayParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	NRU_RMS_ALLOC_PLAYRESOURCE_RSP,                     /*TRSComm_PlayParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表
	                                                                        + TRSComm_MediaInfoDescription + TRSComm_AudioEncParam(..) + TRSComm_VideoEncParam(..)  + TRSComm_MediaInfoExt*/
	
	RMS_NRU_RELEASE_PLAYRESOURCE_REQ,                   /*TRSComm_Handle*/
	NRU_RMS_RELEASE_PLAYRESOURCE_RSP,                   /*TRSComm_Handle*/
	
	RMS_NRU_START_PLAY_REQ,                             /*TRSComm_Handle*/
	NRU_RMS_START_PLAY_RSP,                             /*TRSComm_Handle*/
	
	RMS_NRU_STOP_PLAY_REQ,                              /*TRSComm_Handle*/
	NRU_RMS_STOP_PLAY_RSP,                              /*TRSComm_Handle*/

	NRU_RMS_STOPPED_PLAY_NOTIFY,                        /*TRSComm_Handle*/

	RMS_NRU_VCRCTRL_PLAY_REQ,                           /*TRSComm_Handle + TRSComm_VCRCtrl*/
	NRU_RMS_VCRCTRL_PLAY_RSP,                           /*TRSComm_Handle + TRSComm_VCRCtrl*/

	NRU_RMS_PLAYPROG_NOTIFY,                            /*TRSComm_PlayProg*/

	/*下载*/
	RMS_NRU_ALLOC_DOWNLOADRESOURCE_REQ,                 /*TRSComm_DownloadParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	NRU_RMS_ALLOC_DOWNLOADRESOURCE_RSP,                 /*TRSComm_DownloadParam + TRSComm_TransParamHeader + TRSComm_MediaAddr列表 + TRSComm_MediaInfoExt*/
	
	RMS_NRU_RELEASE_DOWNLOADRESOURCE_REQ,               /*TRSComm_Handle*/
	NRU_RMS_RELEASE_DOWNLOADRESOURCE_RSP,               /*TRSComm_Handle*/
	
	RMS_NRU_START_DOWNLOAD_REQ,                         /*TRSComm_Handle*/
	NRU_RMS_START_DOWNLOAD_RSP,                         /*TRSComm_Handle*/
	
	RMS_NRU_STOP_DOWNLOAD_REQ,                          /*TRSComm_Handle*/
	NRU_RMS_STOP_DOWNLOAD_RSP,                          /*TRSComm_Handle*/

	//status
	NRU_RMS_SYSSTATUS_NOTIFY,                           /*TRSComm_SysStatusParam*/

	RMS_NRU_START_SWITCH_RECORD_NOTIFY,                 /*TRSComm_SwitchRecord*/
	NRU_RMS_END_SWITCH_RECORD_NOTIFY,                   /*TRSComm_SwitchRecord + TRSComm_TimeRange*/

	RMS_NRU_SET_IPADDROFSTUNSRV_NOTIFY,                 /*TRSComm_NetAddr*/

	RMS_NRU_REC_LOCK_REQ,                               /*TRSComm_RecLock*/
	NRU_RMS_REC_LOCK_RSP,                               /*TRSComm_RecLock*/

	RMS_NRU_SET_SYSPARAM_REQ,					        /*TRSComm_RecSysParam*/
	NRU_RMS_SET_SYSPARAM_RSP,				 	        /*TRSComm_RecSysParam*/

	NRU_RMS_ADD_RECORD_NOTIFY,                          /*TRSComm_DBRecord*/

	NRU_RMS_SYNC_RECORD_REQ,                            /*TRSComm_Num + N * TRSComm_SyncRecord*/
	RMS_NRU_SYNC_RECORD_RSP,                            /*TRSComm_Num + N * TRSComm_SyncRecord*/

	RMS_NRU_SET_COVERPARAM_REQ,						    /*TRSComm_Num + N * TRSComm_CoverParam*/
	NRU_RMS_SET_COVERPARAM_RSP,                         /*TRSComm_Num + N * TRSComm_CoverParam*/

	RMS_NRU_DEL_DB_RECORD_REQ,                          /*TRSComm_TimeRange*/
	NRU_RMS_DEL_DB_RECORD_RSP,                          /*TRSComm_TimeRange*/

	RMS_NRU_BATCH_DELETING_RECORD_FILE_REQ,             /*TRSComm_Num + N * TRSComm_DeleteFile*/
	NRU_RMS_BATCH_DELETING_RECORD_FILE_RSP,             /*TRSComm_Num(总数) + TRSComm_Num(删除成功的数量) + N * TRSComm_DeleteFile*/

	RMS_NRU_SET_REC_STATUS_DETECTION_PARAM_REQ,			/*TRSComm_RecStatusDetectionParam*/
	NRU_RMS_SET_REC_STATUS_DETECTION_PARAM_RSP,			/*TRSComm_RecStatusDetectionParam*/

	NRU_RMS_REC_PLAY_DOWN_TASKINFO_NOTIFY,                          /*TRSComm_DBRecord*/

	/*重置录像回放目的端参数*/
	RMS_NRU_RESET_PLAY_DSTPARAM_REQ = 31063,            /*TRSComm_Handle + TRSComm_TransParamHeader + TRSComm_MediaAddr列表*/
	NRU_RMS_RESET_PLAY_DSTPARAM_RSP,                    /*TRSComm_Handle*/

}ERmsNruMsg;

#endif
