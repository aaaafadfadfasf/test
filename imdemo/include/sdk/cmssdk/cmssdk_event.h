/*****************************************************************************
   模块名      : CMSSDK
   文件名      : cmssdk_event.h
   相关文件    : 
   文件实现功能: cmssdk消息定义
   作者        : fanxg
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/09/12  1.0         fanxg        创建
******************************************************************************/

#ifndef __CMS_SDK_EVENT_H__
#define __CMS_SDK_EVENT_H__


//事件定义
enum ECMS_SDK_EVENT
{
    CMS_SDK_MSG_BEGIN = 27501,                        //CmsSdk消息定义起始: CMSSDK_EVENT_BEGIN = 27501

    CMS_SDK_MSG_UNKNOWN = CMS_SDK_MSG_BEGIN+1,        //未知消息

    //与CMU的交互消息（目前已经使用到27761，下一个可用的为27764）
    CMS_SDK_CMU_MSG_BEGIN                 = 27601,

	//cmssdk初始化完成后将协议栈相关信息通知给上层，单向接口:USER<--SDK
	CMS_SDK_PROTOCOL_INFO_NTF             = 27602,    //cmssdk协议栈信息通知：TCmsRealProtocolInfo

    //网关设备登录CMU，单向接口:USER<--SDK
    CMS_CMU_DISCONNECT_NTF                = 27603,    //CMU断链通知，cmssdk会自己重连：无
    CMS_CMU_CONNECT_NTF                   = 27604,    //CMU建链通知：TConnInfo

	//后端能力集通知，单向接口:USER<--SDK
	CMS_SDK_BACKEND_CAPABILITIES_NTF      = 27745,    //后端能力集通知：TCmsBackendCapabilities

    //前端设备注册，单向接口:USER-->SDK
    CMS_DEV_REG_REQ                       = 27605,    //前端设备注册请求：CCmsDevRegReq
    CMS_DEV_REG_RSP                       = 27606,    //前端设备注册应答：CCmsDevRegRsp

	//异域cmu的断链通知，单向接口:USER<--SDK
	CMS_CMU_DISCONNECT_NTF_REQ            = 27607,	  //异域cmu断链通知请求: CCmsCmuDisconnectNtfReq
	CMS_CMU_DISCONNECT_NTF_RSP            = 27608,	  //异域cmu断链通知应答: CCmsCmuDisconnectNtfRsp

    //前端设备注销，单向接口:USER-->SDK
	CMS_DEV_UNREG_REQ                     = 27609,     //前端设备注销请求：CCmsDevUnRegReq
	CMS_DEV_UNREG_RSP                     = 27610,     //前端设备注销应答：CCmsDevUnRegRsp

    //设备状态、配置、告警变更通知，单向接口:USER-->SDK
	CMS_DEV_OFFLINE_NTF                   = 27611,     //设备下线: CCmsDevOfflineNtf 
	CMS_DEV_ONLINE_NTF                    = 27612,     //设备上线: CCmsDevOnlineNtf 
	CMS_DEV_INFO_MOD_NTF                  = 27613,     //设备信息改变: CCmsDevInfoModNtf   
	CMS_DEV_ALARM_NTF                     = 27614,     //设备告警上报: CCmsDevAlarmNtf

	//前端编码参数设置、获取，双向接口：USER<-->SDK
	CMS_PU_VID_ENC_PARAM_SET_REQ          = 27615,		//设置前端编码参数请求：CCmsPuVidEncParamSetReq
	CMS_PU_VID_ENC_PARAM_SET_RSP          = 27616,		//设置前端编码参数应答：CCmsPuVidEncParamSetRsp
	CMS_PU_VID_ENC_PARAM_GET_REQ          = 27617,		//获取前端编码参数请求：CCmsPuVidEncParamGetReq
	CMS_PU_VID_ENC_PARAM_GET_RSP          = 27618,		//获取前端编码参数应答：CCmsPuVidEncParamGetRsp

    //高级编码参数设置
    CMS_PU_VID_ENC_PARAM_SET_EX_REQ       = 27733,      //消息体: CCmsPuVidEncParamSetExReq
    CMS_PU_VID_ENC_PARAM_SET_EX_RSP       = 27734,      //消息体: CCmsPuVidEncParamSetExRsp
                                                      
    CMS_PU_VID_ENC_PARAM_GET_EX_REQ       = 27735,      //消息体: CCmsPuVidEncParamGetExReq
    CMS_PU_VID_ENC_PARAM_GET_EX_RSP       = 27736,      //消息体: CCmsPuVidEncParamGetExRsp

	//音频编码参数设置、获取，单向接口：USER-->SDK
	CMS_PU_AUD_ENC_PARAM_SET_REQ          = 27711,		//音频编码参数设置请求: CCmsPuAudEncParamSetReq
	CMS_PU_AUD_ENC_PARAM_SET_RSP          = 27712,		//音频编码参数设置应答: CCmsPuAudEncParamSetRsp
	CMS_PU_AUD_ENC_PARAM_GET_REQ          = 27713,		//音频编码参数获取请求: CCmsPuAudEncParamGetReq
	CMS_PU_AUD_ENC_PARAM_GET_RSP          = 27714,		//音频编码参数获取应答: CCmsPuAudEncParamGetRsp

	//前端透明串口数据，单向接口:USER-->SDK
	CMS_PU_TRANSPARENT_OPERATE_REQ        = 27619,		//透明串口数据发送请求：CCmsPuTransparentOperateReq
	CMS_PU_TRANSPARENT_OPERATE_RSP        = 27620,		//透明串口数据发送应答：CCmsPuTransparentOperateRsp

	//透明数据配置的设置、获取，单向接口:USER-->SDK
	CMS_PU_TRANSDATA_CFG_SET_REQ          = 27715,		//透明数据配置的设置请求: CCmsPuTransdataCfgSetReq
	CMS_PU_TRANSDATA_CFG_SET_RSP          = 27716,		//透明数据配置的设置应答: CCmsPuTransdataCfgSetRsp
	CMS_PU_TRANSDATA_CFG_GET_REQ          = 27717,		//透明数据配置的获取请求: CCmsPuTransdataCfgGetReq
	CMS_PU_TRANSDATA_CFG_GET_RSP          = 27718,		//透明数据配置的获取应答: CCmsPuTransdataCfgGetRsp

    //开启实时浏览，双向接口:USER<-->SDK
    CMS_REALMEDIA_INVITE_REQ              = 27621,      //实时浏览请求：CCmsRealMediaInviteReq
	CMS_REALMEDIA_INVITE_RSP              = 27622,      //实时浏览应答：CCmsRealMediaInviteRsp
	CMS_REALMEDIA_INVITE_ACK              = 27623,      //实时浏览确认：CCmsRealMediaInviteAck

    //发送关键帧操作，双向接口:USER<-->SDK
    CMS_KEYFREAM_SEND_REQ                 = 27624,       //强制关键帧请求：CCmsKeyFrameSendReq
    CMS_KEYFREAM_SEND_RSP                 = 27625,       //强制关键帧应答：CCmsKeyFrameSendRsp

    //PTZ控制，双向接口:USER<-->SDK
    CMS_PTZ_CTRL_REQ                      = 27626,        //PTZ控制请求：CCmsPtzCtrlReq
    CMS_PTZ_CTRL_RSP                      = 27627,        //PTZ控制应答：CCmsPtzCtrlRsp

    //关闭实时浏览，双向接口:USER<-->SDK
    CMS_REALMEDIA_BYE_REQ                 = 27628,        //停止实时浏览请求：CCmsRealMediaByeReq
    CMS_REALMEDIA_BYE_RSP                 = 27629,        //停止实时浏览应答：CCmsRealMediaByeRsp

    //开启、停止音频呼叫，单向接口:USER-->SDK
	CMS_START_AUDIO_CALL_REQ              = 27630,		  //开启音频呼叫请求：CCmsStartAudioCallReq
	CMS_START_AUDIO_CALL_RSP              = 27631,		  //开启音频呼叫应答：CCmsStartAudioCallRsp
	CMS_STOP_AUDIO_CALL_REQ               = 27632,		  //停止音频呼叫请求：CCmsStopAudioCallReq
	CMS_STOP_AUDIO_CALL_RSP               = 27633,		  //停止音频呼叫应答：CCmsStopAudioCallRsp

	//呼叫交换拆除通知，单向接口：USER<--SDK
	CMS_AUDIO_CALL_DESTORY_NTF_REQ        = 27634,		  //呼叫交换拆除通知：CCmsAudioCallDestoryNtfReq
	CMS_AUDIO_CALL_DESTORY_NTF_RSP        = 27635,		  //呼叫交换拆除通知应答：CCmsAudioCallDestoryNtfRsp

	//开启音频呼叫，单向接口:USER<--SDK
    CMS_AUDIOCALL_INVITE_REQ              = 27636,        //音频呼叫请求：CCmsAudioCallInviteReq
	CMS_AUDIOCALL_INVITE_RSP              = 27637,        //音频呼叫应答：CCmsAudioCallInviteRsp
	CMS_AUDIOCALL_INVITE_ACK              = 27638,        //音频呼叫确认：CCmsAudioCallInviteAck

    //关闭音频呼叫，单向接口:USER<--SDK
    CMS_AUDIOCALL_BYE_REQ                 = 27639,        //停止音频呼叫请求：CCmsAudioCallByeReq
    CMS_AUDIOCALL_BYE_RSP                 = 27640,        //停止音频呼叫应答：CCmsAudioCallByeRsp

	//国标音频呼叫流程（为支持GBS语音广播添加）
	//开启、停止音频呼叫，单向接口:USER-->SDK
	CMS_BROADCAST_START_AUDIO_CALL_REQ    = 27747,		  //语音广播开启音频呼叫请求：CCmsBroadcastStartAudioCallReq
	CMS_BROADCAST_START_AUDIO_CALL_RSP    = 27748,		  //语音广播开启音频呼叫应答：CCmsBroadcastStartAudioCallRsp
	CMS_BROADCAST_STOP_AUDIO_CALL_REQ     = 27749,		  //语音广播停止音频呼叫请求：CCmsBroadcastStopAudioCallReq
	CMS_BROADCAST_STOP_AUDIO_CALL_RSP     = 27750,		  //语音广播停止音频呼叫应答：CCmsBroadcastStopAudioCallRsp

	//呼叫交换拆除通知，单向接口：USER<--SDK（CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ、CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ暂时可以去掉）
	//CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ = 27751,	  //语音广播呼叫交换拆除通知：CCmsBroadcastAudioCallDestoryNtfReq
	//CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_RSP = 27752,	  //语音广播呼叫交换拆除通知应答：CCmsBroadcastAudioCallDestoryNtfRsp

	//开启音频呼叫，单向接口:USER<--SDK
	CMS_BROADCAST_AUDIOCALL_INVITE_REQ    = 27753,        //语音广播音频呼叫请求：CCmsBroadcastAudioCallInviteReq
	CMS_BROADCAST_AUDIOCALL_INVITE_RSP    = 27754,        //语音广播音频呼叫应答：CCmsBroadcastAudioCallInviteRsp
	CMS_BROADCAST_AUDIOCALL_INVITE_ACK    = 27755,        //语音广播音频呼叫确认：CCmsBroadcastAudioCallInviteAck

	//关闭音频呼叫，单向接口:USER<--SDK
	CMS_BROADCAST_AUDIOCALL_BYE_REQ       = 27756,        //语音广播停止音频呼叫请求：CCmsBroadcastAudioCallByeReq
	CMS_BROADCAST_AUDIOCALL_BYE_RSP       = 27757,        //语音广播停止音频呼叫应答：CCmsBroadcastAudioCallByeRsp

    //录像查询，双向接口:USER<-->SDK
    //查询方式：分页查询，一次查询多个通知。
	CMS_REC_QUERY_REQ                     = 27641,        //录像查询请求：   CCmsRecQueryReq
	CMS_REC_QUERY_RSP                     = 27642,        //录像查询应答：   CCmsRecQueryRsp
	CMS_REC_QUERY_NTF_REQ                 = 27643,        //录像查询通知：   CCmsRecQueryNtfReq
	CMS_REC_QUERY_NTF_RSP                 = 27644,        //录像查询通知应答：CCmsRecQueryNtfRsp

    //开启录像回放，双向接口:USER<-->SDK
    //超过放像能力开启失败。
    CMS_REC_PLAY_INVITE_REQ               = 27645,        //录像回放开启请求：CCmsRecPlayInviteReq
	CMS_REC_PLAY_INVITE_RSP               = 27646,        //录像回放开启应答：CCmsRecPlayInviteRsp
	CMS_REC_PLAY_INVITE_ACK               = 27647,        //录像回放开启确认：CCmsRecPlayInviteAck

    //放像进度通知，双向接口:USER<-->SDK
    CMS_REC_PLAY_PROG_NTF_REQ             = 27648,        //录像回放进度上报请求：CCmsRecPlayProgNtfReq
	CMS_REC_PLAY_PROG_NTF_RSP             = 27649,        //录像回放进度上报应答：CCmsRecPlayProgNtfRsp
	CMS_REC_PLAY_END_NTF_REQ              = 27650,        //录像回放结束通知请求：CCmsRecPlayEndNtyReq
	CMS_REC_PLAY_END_NTF_RSP              = 27651,        //录像回放结束通知应答：CCmsRecPlayEndNtyRsp
	CMS_REC_PLAY_PROG_QRY_REQ             = 27652,        //录像回放进度查询请求：CCmsRecPlayProgQryReq
	CMS_REC_PLAY_PROG_QRY_RSP             = 27653,		  //录像回放进度查询应答：CCmsRecPlayProgQryRsp

    //放像VCR控制，双向接口:USER<-->SDK
    CMS_REC_PLAY_VCR_CTRL_REQ             = 27654,        //VCR控制请求：CCmsRecPlayVcrCtrlReq
    CMS_REC_PLAY_VCR_CTRL_RSP             = 27655,        //VCR控制应答：CCmsRecPlayVcrCtrlRsp

    //停止录像回放，双向接口:USER<-->SDK
    CMS_REC_PLAY_BYE_REQ                  = 27656,        //录像回放停止请求：CCmsRecPlayByeReq
    CMS_REC_PLAY_BYE_RSP                  = 27657,        //录像回放停止应答：CCmsRecPlayByeRsp

    //开启录像下载，双向接口:USER<-->SDK
    //超过下载能力开启失败。
    CMS_REC_DOWNLOAD_INVITE_REQ           = 27658,        //开始录像下载请求：CCmsRecDownloadInviteReq
    CMS_REC_DOWNLOAD_INVITE_RSP           = 27659,        //开始录像下载应答：CCmsRecDownloadInviteRsp
    CMS_REC_DOWNLOAD_INVITE_ACK           = 27660,        //开始录像下载确认：CCmsRecDownloadInviteAck

	//下载结束通知，双向接口:USER<-->SDK
	CMS_REC_DOWNLOAD_END_NTF_REQ          = 27661,        //录像下载结束通知请求：CCmsRecDownloadEndNtyReq
	CMS_REC_DOWNLOAD_END_NTF_RSP          = 27662,        //录像下载结束通知应答：CCmsRecDownloadEndNtyRsp

    //停止录像下载，双向接口:USER<-->SDK
    CMS_REC_DOWNLOAD_BYE_REQ              = 27663,         //录像下载停止请求：CCmsRecDownloadByeReq
    CMS_REC_DOWNLOAD_BYE_RSP              = 27664,         //录像下载停止应答：CCmsRecDownloadByeRsp

    //目录订阅，单向接口:USER-->SDK
    CMS_CATALOG_STATUS_SS_REQ             = 27665,         //目录订阅请求: CCmsCatalogStatusSsReq
    CMS_CATALOG_STATUS_SS_RSP             = 27666,         //目录订阅应答: CCmsCatalogStatusSsRsp

    //目录订阅刷新，单向接口:USER-->SDK
    CMS_CATALOG_STATUS_REFRESH_SS_REQ     = 27667,        //目录订阅刷新请求: CCmsCatalogStatusRefreshSsReq
    CMS_CATALOG_STATUS_REFRESH_SS_RSP     = 27668,        //目录订阅刷新应答: CCmsCatalogStatusRefreshSsRsp

    //目录订阅取消，单向接口:USER-->SDK
    CMS_CATALOG_STATUS_UNSS_REQ           = 27669,        //目录订阅取消请求: CCmsCatalogStatusUnSsReq
    CMS_CATALOG_STATUS_UNSS_RSP           = 27670,        //目录订阅取消应答: CCmsCatalogStatusUnSsRsp

    //设备列表订阅，单向接口:USER-->SDK
    CMS_PULIST_STATUS_SS_REQ              = 27671,        //设备列表订阅请求: CCmsPuListStatusSsReq
    CMS_PULIST_STATUS_SS_RSP              = 27672,        //设备列表订阅应答: CCmsPuListStatusSsRsp

    //设备列表订阅刷新，单向接口:USER-->SDK
    CMS_PULIST_STATUS_REFRESH_SS_REQ      = 27673,        //设备列表订阅刷新请求: CCmsPuListStatusRefreshSsReq
    CMS_PULIST_STATUS_REFRESH_SS_RSP      = 27674,        //设备列表订阅刷新应答: CCmsPuListStatusRefreshSsRsp

    //设备列表订阅取消，单向接口:USER-->SDK
    CMS_PULIST_STATUS_UNSS_REQ            = 27675,         //设备列表订阅取消请求: CCmsPuListStatusUnSsReq
    CMS_PULIST_STATUS_UNSS_RSP            = 27676,         //设备列表订阅取消应答: CCmsPuListStatusUnSsRsp

    //设备订阅，单向接口:USER-->SDK
    CMS_PU_STATUS_SS_REQ                  = 27677,          //设备订阅请求: CCmsPuStatusSsReq
    CMS_PU_STATUS_SS_RSP                  = 27678,          //设备订阅应答: CCmsPuStatusSsRsp

    //设备订阅刷新，单向接口:USER-->SDK
    CMS_PU_STATUS_REFRESH_SS_REQ          = 27679,          //设备订阅刷新请求: CCmsPuStatusRefreshSsReq
    CMS_PU_STATUS_REFRESH_SS_RSP          = 27680,          //设备订阅刷新应答: CCmsPuStatusRefreshSsRsp

    //设备订阅取消，单向接口:USER-->SDK
    CMS_PU_STATUS_UNSS_REQ                = 27681,          //设备订阅取消请求: CCmsPuStatusUnSsReq
    CMS_PU_STATUS_UNSS_RSP                = 27682,          //设备订阅取消应答: CCmsPuStatusUnSsRsp

    //设备在线状态通知，单向接口:USER<--SDK
    CMS_PU_STATUS_NTF_REQ                 = 27683,          //设备上下线通知请求: CCmsPuStatusNtyReq
    CMS_PU_STATUS_NTF_RSP                 = 27684,          //设备上下线通知应答: CCmsPuStatusNtyRsp

    //设备配置信息通知，单向接口:USER<--SDK
    CMS_PU_CONFIG_NTF_REQ                 = 27685,          //设备配置信息通知请求: CCmsPuConfigNtyReq
    CMS_PU_CONFIG_NTF_RSP                 = 27686,          //设备配置信息通知应答: CCmsPuConfigNtyRsp

    //设备服务状态通知，单向接口:USER<--SDK
    CMS_PU_SERVICE_STATUS_NTF_REQ         = 27687,          //设备服务状态通知请求: CCmsPuServiceStatusNtyReq
    CMS_PU_SERVICE_STATUS_NTF_RSP         = 27688,          //设备服务状态通知应答: CCmsPuServiceStatusNtyRsp

    //设备告警状态通知，单向接口:USER<--SDK
    CMS_PU_ALARM_STATUS_NTF_REQ           = 27689,          //设备告警状态通知请求: CCmsPuAlarmStatusNtyReq
    CMS_PU_ALARM_STATUS_NTF_RSP           = 27690,          //设备告警状态通知应答: CCmsPuAlarmStatusNtyRsp

	//透明数据状态通知，单向接口:USER<--SDK
	CMS_PU_TRANSDATA_STATUS_NTF_REQ       = 27691,          //设备串口数据通知请求: CCmsPuTransDataStatusNtyReq
	CMS_PU_TRANSDATA_STATUS_NTF_RSP       = 27692,          //设备串口数据通知应答: CCmsPuTransDataStatusNtyRsp

    //GPS状态通知，单向接口:USER<--SDK
    CMS_PU_GPS_STATUS_NTF_REQ             = 27693,          //设备GPS状态通知请求: CCmsPuGpsStatusNtyReq
    CMS_PU_GPS_STATUS_NTF_RSP             = 27694,          //设备GPS状态通知应答: CCmsPuGpsStatusNtyRsp

	//批量通知，单向接口:USER<--SDK
	CMS_PU_BATCH_NTF_REQ                  = 27760,			// 订阅的批量通知请求：CCmsPuBatchNtyReq
	CMS_PU_BATCH_NTF_RSP                  =	27761,			// 订阅的批量通知应答：CCmsPuBatchNtyRsp

    //开启/关闭GPS信息上报，单向接口：USER<--SDK
    CMS_SUBSCRIBE_GPS_REQ                 = 27695,          //开启/关闭GPS信息上报请求：CCmsSubscribeGpsReq
    CMS_SUBSCRIBE_GPS_RSP                 = 27696,          //开启/关闭GPS信息上报应答：CCmsSubscribeGpsRsp
    CMS_DEV_GPS_DATA_NTF                  = 27697,          //设备gps信息改变通知:CCmsDevGpsDataNty

	//摄像头类型设置，单向接口：USER<-->SDK
	CMS_PU_CAMERA_TYPE_SET_REQ            = 27698,			//摄像头类型设置请求: CCmsPuCameraTypeSetReq
	CMS_PU_CAMERA_TYPE_SET_RSP            = 27699,			//摄像头类型设置应答: CCmsPuCameraTypeSetRsp

    //移动侦测
    CMS_PU_MOTION_DETECT_SET_REQ          = 27719,          //消息体: CCmsPuMotionDetectSetReq
    CMS_PU_MOTION_DETECT_SET_RSP          = 27720,          //消息体: CCmsPuMotionDetectSetRsp

    CMS_PU_MOTION_DETECT_GET_REQ          = 27721,          //消息体: CCmsPuMotionDetectGetReq
    CMS_PU_MOTION_DETECT_GET_RSP          = 27722,          //消息体: CCmsPuMotionDetectGetRsp

    //图像遮蔽
    CMS_PU_AREA_SHADE_SET_REQ             = 27723,          //消息体: CCmsPuAreaShadeSetReq
    CMS_PU_AREA_SHADE_SET_RSP             = 27724,          //消息体: CCmsPuAreaShadeSetRsp
                                                     
    CMS_PU_AREA_SHADE_GET_REQ             = 27725,          //消息体: CCmsPuAreaShadeGetReq
    CMS_PU_AREA_SHADE_GET_RSP             = 27726,          //消息体: CCmsPuAreaShadeGetRsp

	//图片抓拍，单向接口：USER-->SDK
	CMS_PU_PIC_SNAPSHOT_REQ               = 27700,			//图片抓拍请求: CCmsPuPicSnapshotReq
	CMS_PU_PIC_SNAPSHOT_RSP               = 27701,			//图片抓拍应答: CCmsPuPicSnapshotRsp

	//前端录像日历查询，单向接口：USER-->SDK
	CMS_PU_REC_DAYS_QRY_REQ               = 27702,			//前端录像日历查询请求: CCmsPuRecDaysQryReq
	CMS_PU_REC_DAYS_QRY_RSP               = 27703,			//前端录像日历查询应答: CCmsPuRecDaysQryRsp

	//前端长字幕设置、获取，单向接口：USER-->SDK
	CMS_PU_LONG_OSD_SET_REQ               = 27727,			//前端长字幕设置请求: CCmsPuLongOsdSetReq
	CMS_PU_LONG_OSD_SET_RSP               = 27728,			//前端长字幕设置应答: CCmsPuLongOsdSetRsp
	CMS_PU_LONG_OSD_GET_REQ               = 27729,			//前端长字幕获取请求: CCmsPuLongOsdGetReq
	CMS_PU_LONG_OSD_GET_RSP               = 27730,			//前端长字幕获取应答: CCmsPuLongOsdGetRsp

	//传输台标，单向接口：USER-->SDK
	CMS_PU_LOGO_BMPOSD_TRANSMIT_REQ       = 27731,			//台标传输请求: CCmsPuLogoBmpOsdTransmitReq
	CMS_PU_LOGO_BMPOSD_TRANSMIT_RSP       = 27732,			//台标传输应答: CCmsPuLogoBmpOsdTransmitRsp

    //前端录像开启、停止，单向接口：USER-->SDK
    CMS_PU_RECORD_START_REQ               = 27704,          //消息体: CCmsPuRecordStartReq
    CMS_PU_RECORD_START_RSP               = 27705,          //消息体: CCmsPuRecordStartRsp
    
    CMS_PU_RECORD_STOP_REQ                = 27706,          //消息体: CCmsPuRecordStopReq
    CMS_PU_RECORD_STOP_RSP                = 27707,          //消息体: CCmsPuRecordStopRsp

    //前端录像删除，单向接口：USER-->SDK
    CMS_PU_RECORD_DEL_REQ                 = 27708,          //消息体: CCmsPuRecordDelReq
    CMS_PU_RECORD_DEL_RSP                 = 27709,          //消息体: CCmsPuRecordDelRsp

	//前端抓拍参数设置
	CMS_PU_SNAPSHOT_CFG_GET_REQ           = 27737,          //消息体: CCmsPuSnapShotCfgGetReq 
	CMS_PU_SNAPSHOT_CFG_GET_RSP           = 27738,          //消息体: CCmsPuSnapShotCfgGetRsp 
	CMS_PU_SNAPSHOT_CFG_SET_REQ           = 27739,          //消息体: CCmsPuSnapShotCfgSetReq 
	CMS_PU_SNAPSHOT_CFG_SET_RSP           = 27740,          //消息体: CCmsPuSnapShotCfgSetRsp 
	//前端串口参数配置
	CMS_PU_SERIALPARAM_CFG_GET_REQ        = 27741,          //消息体：CCmsPuSerialParamCfgGetReq
	CMS_PU_SERIALPARAM_CFG_GET_RSP        = 27742,          //消息体：CCmsPuSerialParamCfgGetRsp
	CMS_PU_SERIALPARAM_CFG_SET_REQ        = 27743,          //消息体：CCmsPuSerialParamCfgSetReq
	CMS_PU_SERIALPARAM_CFG_SET_RSP        = 27744,          //消息体：CCmsPuSerialParamCfgSetRsp

	
	CMS_PU_TRANSPARENT_DATA_NTF_REQ        = 27745,          //消息体：CCmsPuTransparentDataNtfReq
	CMS_PU_TRANSPARENT_DATA_NTF_RSP        = 27746,          //消息体：CCmsPuTransparentDataNtfRsp

	CMS_CMU_DISCONNECT_PU_NTF              = 27758,			 //消息体: CCmsCmuDisconnectPuNtf

	CMS_MODULE_ON_OFF_LOG_NTF              = 27759,			 //消息体: CCmsModuleOnOffLogNtf

    //手动消警，单向接口:USER<-->SDK
    CMS_PU_ALARM_RESET_REQ                 = 27762,          //消息体：CCmsPuAlarmResetReq
    CMS_PU_ALARM_RESET_RSP                 = 27763,          //消息体：CCmsPuAlarmResetRsp

    CMS_SDK_CMU_MSG_END                   = 27799,

    //与TAS的交换消息
    CMS_SDK_3AS_MSG_BEGIN				  = 27801,

    //网关设备登录3as，单向接口:USER<--SDK
    CMS_3AS_DISCONNECT_NTF                = 27802,          //3AS断链通知，cmssdk会自己重连：无
    CMS_3AS_CONNECT_NTF                   = 27803,          //3AS建链通知：TConnInfo

    // 设备增、删、改、查，单向接口:USER-->SDK
	CMS_3AS_DEVICE_ADD_REQ                = 27804,          //设备入网请求：CCmsDeviceAddReq
	CMS_3AS_DEVICE_ADD_RSP                = 27805,          //设备入网应答：CCmsDeviceAddRsp
	CMS_3AS_DEVICE_ADD_NTF_REQ            = 27806,          //设备入网通知：CCmsDeviceAddNtfReq
	CMS_3AS_DEVICE_ADD_NTF_RSP            = 27807,          //设备入网通知应答：CCmsDeviceAddNtfRsp
	CMS_3AS_DEVICE_DEL_REQ                = 27808,          //设备退网请求：CCmsDeviceDelReq
	CMS_3AS_DEVICE_DEL_RSP                = 27809,          //设备退网应答：CCmsDeviceDelRsp
	CMS_3AS_DEVICE_DEL_NTF_REQ            = 27810,          //设备退网通知：CCmsDeviceDelNtfReq
	CMS_3AS_DEVICE_DEL_NTF_RSP            = 27811,          //设备退网通知应答：CCmsDeviceDelNtfRsp
	CMS_3AS_DEVICE_MOD_REQ                = 27812,          //设备修改请求：CCmsDeviceModReq
	CMS_3AS_DEVICE_MOD_RSP                = 27813,          //设备修改应答：CCmsDeviceModRsp
	CMS_3AS_DEVICE_MOD_NTF_REQ            = 27814,          //设备修改通知：CCmsDeviceModNtfReq
	CMS_3AS_DEVICE_MOD_NTF_RSP            = 27815,          //设备修改通知应答：CCmsDeviceModNtfRsp
	CMS_3AS_DEVICE_QRY_REQ                = 27816,          //设备查询请求：CCmsDeviceQryReq
	CMS_3AS_DEVICE_QRY_RSP                = 27817,          //设备查询应答：CCmsDeviceQryRsp
	CMS_3AS_DEVICE_QRY_NTF_REQ            = 27818,          //设备查询通知：CCmsDeviceQryNtfReq
	CMS_3AS_DEVICE_QRY_NTF_RSP            = 27819,          //设备查询通知应答：CCmsDeviceQryNtfRsp

    // 设备组增、删、改、查，单向接口:USER-->SDK
    CMS_3AS_DEVICE_GROUP_ADD_REQ          = 27820,          //设备组添加请求：CCmsDeviceGroupAddReq
	CMS_3AS_DEVICE_GROUP_ADD_RSP          = 27821,          //设备组添加应答：CCmsDeviceGroupAddRsp
	CMS_3AS_DEVICE_GROUP_ADD_NTF_REQ      = 27822,          //设备组添加通知：CCmsDeviceGroupAddNtfReq
	CMS_3AS_DEVICE_GROUP_ADD_NTF_RSP      = 27823,          //设备组添加通知应答：CCmsDeviceGroupAddNtfRsp
	CMS_3AS_DEVICE_GROUP_DEL_REQ          = 27824,          //设备组删除请求：CCmsDeviceGroupDelReq
	CMS_3AS_DEVICE_GROUP_DEL_RSP          = 27825,          //设备组删除应答：CCmsDeviceGroupDelRsp
	CMS_3AS_DEVICE_GROUP_DEL_NTF_REQ      = 27826,          //设备组添加通知：CCmsDeviceGroupDelNtfReq
	CMS_3AS_DEVICE_GROUP_DEL_NTF_RSP      = 27827,          //设备组添加通知应答：CCmsDeviceGroupDelNtfRsp
	CMS_3AS_DEVICE_GROUP_MOD_REQ          = 27828,          //设备组修改请求：CCmsDeviceGroupModReq
	CMS_3AS_DEVICE_GROUP_MOD_RSP          = 27829,          //设备组修改应答：CCmsDeviceGroupModRsp
	CMS_3AS_DEVICE_GROUP_MOD_NTF_REQ      = 27830,          //设备组添加通知：CCmsDeviceGroupModNtfReq
	CMS_3AS_DEVICE_GROUP_MOD_NTF_RSP      = 27831,          //设备组添加通知应答：CCmsDeviceGroupModNtfRsp
	CMS_3AS_DEVICE_GROUP_QRY_REQ          = 27832,          //设备组查询请求：CCmsDeviceGroupQryReq
	CMS_3AS_DEVICE_GROUP_QRY_RSP          = 27833,          //设备组查询应答：CCmsDeviceGroupQryRsp
	CMS_3AS_DEVICE_GROUP_QRY_NTF_REQ      = 27834,          //设备组查询通知：CCmsDeviceGroupQryNtfReq
	CMS_3AS_DEVICE_GROUP_QRY_NTF_RSP      = 27835,          //设备组查询通知应答：CCmsDeviceGroupQryNtfRsp

    // 设备划归、删除、查询，单向接口:USER-->SDK
    CMS_3AS_DEVICE_GROUP_DEVICE_ADD_REQ     = 27836,        //设备组设备划归请求：CCmsDeviceGroupDeviceAddReq
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_RSP     = 27837,        //设备组设备划归应答：CCmsDeviceGroupDeviceAddRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ = 27838,        //设备组设备划归通知：CCmsDeviceGroupDeviceAddNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP = 27839,        //设备组设备划归通知应答：CCmsDeviceGroupDeviceAddNtfRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_REQ     = 27840,        //设备组设备划归请求：CCmsDeviceGroupDeviceDelReq
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_RSP     = 27841,        //设备组设备划归应答：CCmsDeviceGroupDeviceDelRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ = 27842,        //设备组设备划归通知：CCmsDeviceGroupDeviceDelNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP = 27843,        //设备组设备划归通知应答：CCmsDeviceGroupDeviceDelNtfRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_REQ     = 27844,        //设备组下的设备查询请求：CCmsDeviceGroupDeviceQryReq
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_RSP     = 27845,        //设备组下的设备查询应答：CCmsDeviceGroupDeviceQryRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ = 27846,        //设备组下的设备查询通知：CCmsDeviceGroupDeviceQryNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP = 27847,        //设备组下的设备查询通知应答：CCmsDeviceGroupDeviceQryNtfRsp

	// 视频源别名获取，单向接口:USER-->SDK
	CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_REQ   = 27848,		//视频源别名获取请求：CCmsDeviceCapIndexNameGetReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_RSP   = 27849,		//视频源别名获取应答：CCmsDeviceCapIndexNameGetRsp

	// 视频源别名设置，单向接口:USER-->SDK
	CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_REQ   = 27850,		//视频源别名设置请求：CCmsDeviceCapIndexNameSetReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_RSP   = 27851,		//视频源别名设置应答：CCmsDeviceCapIndexNameSetRsp
	CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ = 27852,		//设备能力别名修改通知：CCmsDeviceCapIndexNameModNtfReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP = 27853,		//设备能力别名修改通知应答：CCmsDeviceCapIndexNameModNtfRsp

    // 视频源别名查询，单向接口:USER-->SDK
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_REQ  = 27900,         //视频源别名查询请求： CCmsDeviceCapIndexNameQryReq
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_RSP  = 27901,         //视频源别名查询应答： CCmsDeviceCapIndexNameQryRsp
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ = 27902,      //视频源别名查询通知： CCmsDeviceCapIndexNameQryNtfReq
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP = 27903,      //视频源别名查询通知应答：CCmsDeviceCapIndexNameQryNtfRsp

	// 域关系查询，单向接口:USER-->SDK
	CMS_3AS_DOMAIN_RELATION_QRY_REQ        = 27854,			//域关系查询请求：CCmsDomainRelationQryReq
	CMS_3AS_DOMAIN_RELATION_QRY_RSP        = 27855,			//域关系查询应答：CCmsDomainRelationQryRsp
	CMS_3AS_DOMAIN_RELATION_QRY_NTF_REQ    = 27856,			//域关系查询通知：CCmsDomainRelationQryNtfReq
	CMS_3AS_DOMAIN_RELATION_QRY_NTF_RSP    = 27857,			//域关系查询通知应答：CCmsDomainRelationQryNtfRsp

	// 域关系变更通知，单向接口:USER<--SDK
    CMS_3AS_DOMAIN_RELATION_ADD_NTF_REQ    = 27858,         //域关系添加通知：CCmsDomainRelationAddNtfReq
	CMS_3AS_DOMAIN_RELATION_ADD_NTF_RSP    = 27859,         //域关系添加通知应答：CCmsDomainRelationAddNtfRsp
	CMS_3AS_DOMAIN_RELATION_DEL_NTF_REQ    = 27860,         //域关系删除通知：CCmsDomainRelationDelNtfReq
	CMS_3AS_DOMAIN_RELATION_DEL_NTF_RSP    = 27861,         //域关系删除通知应答：CCmsDomainRelationDelNtfRsp
	CMS_3AS_DOMAIN_RELATION_MOD_NTF_REQ    = 27862,         //域关系修改通知：CCmsDomainRelationModNtfReq
	CMS_3AS_DOMAIN_RELATION_MOD_NTF_RSP    = 27863,         //域关系修改通知应答：CCmsDomainRelationModNtfRsp

	// 电视墙增、删、改、查，单向接口:USER->SDK
	CMS_3AS_TVWALL_ADD_REQ                 = 27864,			// 电视墙添加请求：CCmsTvwallAddReq 
	CMS_3AS_TVWALL_ADD_RSP                 = 27865,			// 电视墙添加应答：CCmsTvwallAddRsp
	CMS_3AS_TVWALL_ADD_NTF_REQ             = 27866,			// 电视墙添加通知：CCmsTvwallAddNtfReq
	CMS_3AS_TVWALL_ADD_NTF_RSP             = 27867,			// 电视墙添加通知应答：CCmsTvwallAddNtfRsp
	CMS_3AS_TVWALL_DEL_REQ                 = 27868,			// 电视墙删除请求：CCmsTvwallDelReq
	CMS_3AS_TVWALL_DEL_RSP                 = 27869,			// 电视墙删除应答：CCmsTvwallDelRsp
	CMS_3AS_TVWALL_DEL_NTF_REQ             = 27870,			// 电视墙删除通知：CCmsTvwallDelNtfReq
	CMS_3AS_TVWALL_DEL_NTF_RSP             = 27871,			// 电视墙删除通知应答：CCmsTvwallDelNtfRsp
	CMS_3AS_TVWALL_MOD_REQ                 = 27872,			// 电视墙修改请求：CCmsTvwallModReq
	CMS_3AS_TVWALL_MOD_RSP                 = 27873,			// 电视墙修改应答：CCmsTvwallModRsp
	CMS_3AS_TVWALL_MOD_NTF_REQ             = 27874,			// 电视墙修改通知：CCmsTvwallModNtfReq
	CMS_3AS_TVWALL_MOD_NTF_RSP             = 27875,			// 电视墙修改通知应答：CCmsTvwallModNtfRsp
	CMS_3AS_TVWALL_QRY_REQ                 = 27876,			// 电视墙查询请求：CCmsTvwallQryReq
	CMS_3AS_TVWALL_QRY_RSP                 = 27877,			// 电视墙查询应答：CCmsTvwallQryRsp
	CMS_3AS_TVWALL_QRY_NTF_REQ             = 27878,			// 电视墙查询通知：CCmsTvwallQryNtfReq
	CMS_3AS_TVWALL_QRY_NTF_RSP             = 27879,			// 电视墙查询通知应答：CCmsTvwallQryNtfRsp

	//用户列表查询
	CMS_3AS_CUSER_QRY_REQ                  = 27880,			//用户列表查询请求：CCmsUserQryReq
	CMS_3AS_CUSER_QRY_RSP                  = 27881,			//用户列表查询应答：CCmsUserQryRsp
	CMS_3AS_CUSER_QRY_NTF_REQ              = 27882,			//用户列表查询通知：CCmsUserQryNtfReq
	CMS_3AS_CUSER_QRY_NTF_RSP              = 27883,			//用户列表查询通知应答：CCmsUserQryNtfRsp

	//查询用户设备
	CMS_3AS_CUSER_DEVICE_QRY_REQ           = 27884,			//用户设备查询请求：CCmsUserDeviceQryReq
	CMS_3AS_CUSER_DEVICE_QRY_RSP           = 27885,			//用户设备查询应答：CCmsUserDeviceQryRsp
	CMS_3AS_CUSER_DEVICE_QRY_NTF_REQ       = 27886,			//用户设备查询通知：CCmsUserDeviceQryNtfReq
	CMS_3AS_CUSER_DEVICE_QRY_NTF_RSP       = 27887,			//用户设备查询通知应答：CCmsUserDeviceQryNtfRsp

	//cu上下线请求
	CMS_3AS_CU_ONLINE_REQ                  = 27888,			//cu上线请求：CCmsCuOnlineReq
	CMS_3AS_CU_ONLINE_RSP                  = 27889,			//cu上线应答：CCmsCuOnlineRsp
	CMS_3AS_CU_OFFLINE_REQ                 = 27890,			//cu下线请求：CCmsCuOfflineReq
	CMS_3AS_CU_OFFLINE_RSP                 = 27891,			//cu下线应答：CCmsCuOfflineRsp

    CMS_3AS_CUSER_ADD_NTF_REQ              = 27892,         //消息体 CCmsUserAddNtfReq
    CMS_3AS_CUSER_ADD_NTF_RSP              = 27893,         //消息体 CCmsUserAddNtfRsp

    CMS_3AS_CUSER_DEL_NTF_REQ              = 27894,         //消息体 CCmsUserDelNtfReq
    CMS_3AS_CUSER_DEL_NTF_RSP              = 27895,         //消息体 CCmsUserDelNtfRsp

    CMS_3AS_CUSER_MOD_NTF_REQ              = 27896,         //消息体 CCmsUserModNtfReq
    CMS_3AS_CUSER_MOD_NTF_RSP              = 27897,         //消息体 CCmsUserModNtfRsp

    CMS_3AS_GATEWAY_DEL_REQ                = 27898,         // 消息体 CCmsGatewayDelReq
    CMS_3AS_GATEWAY_DEL_RSP                = 27899,         // 消息体 CCmsGatewayDelRsp

    CMS_SDK_3AS_MSG_END                    = 27999,

    //与RCS的交换消息
    CMS_SDK_RCS_MSG_BEGIN				   = 28001,

    //网关设备登录rcs，单向接口:USER<--SDK
	CMS_RCS_DISCONNECT_NTF                 = 28002,          //RCS断链通知，cmssdk会自己重连：无
	CMS_RCS_CONNECT_NTF                    = 28003,          //RCS建链通知：TConnInfo

	//rcs录像状态订阅、取消订阅，单向接口:USER-->SDK
	CMS_RCS_SUBSCRIBE_PU_STATUS_REQ        = 28004,			//录像状态订阅请求：CCmsRcsSubscribePuStatusReq
	CMS_RCS_SUBSCRIBE_PU_STATUS_RSP        = 28005,			//录像状态订阅应答：CCmsRcsSubscribePuStatusRsp
	CMS_RCS_PU_RECORD_STATUS_NTF_REQ       = 28006,			//录像状态订阅通知：CCmsRcsPuRecordStatusNtyReq
	CMS_RCS_PU_RECORD_STATUS_NTF_RSP       = 28007,			//录像状态订阅通知应答：CCmsRcsPuRecordStatusNtyRsp
	CMS_RCS_UNSUBSCRIBE_PU_STATUS_REQ      = 28008,			//取消录像状态订阅请求：CCmsRcsUnsubscribePuStatusReq
	CMS_RCS_UNSUBSCRIBE_PU_STATUS_RSP      = 28009,			//取消录像状态订阅应答：CCmsRcsUnsubscribePuStatusRsp

	//获取录像系统参数，单向接口：USER-->SDK
	CMS_RCS_GET_SYSPARAM_REQ                = 28010,		//获取录像系统参数请求：CCmsRcsGetSysParamReq
	CMS_RCS_GET_SYSPARAM_RSP                = 28011,		//获取录像系统参数应答：CCmsRcsGetSysParamRsp

	//设置录像任务(主要是手动、告警)，单向接口：USER-->SDK
	CMS_RCS_SET_RECORD_TASK_REQ             = 28012,		//设置录像任务请求：CCmsRcsSetRecordTaskReq
	CMS_RCS_SET_RECORD_TASK_RSP             = 28013,		//设置录像任务应答：CCmsRcsSetRecordTaskRsp

	//录像记录删除，单向接口：USER-->SDK
	CMS_RCS_DELETE_RECORD_REQ               = 28014,		//录像记录删除请求：CCmsRcsDeleteRecordReq
	CMS_RCS_DELETE_RECORD_RSP               = 28015,		//录像记录删除应答：CCmsRcsDeleteRecordRsp

	//设置录像别名，单向接口：USER-->SDK
	CMS_RCS_SET_RECORD_ALIAS_REQ            = 28016,		//设置录像别名请求：CCmsRcsSetRecordAliasReq
	CMS_RCS_SET_RECORD_ALIAS_RSP            = 28017,		//设置录像别名应答：CCmsRcsSetRecordAliasRsp

	//录像锁定/解锁，单向接口：USER-->SDK
	CMS_RCS_RECORD_LOCK_OPR_REQ             = 28018,		//录像锁定/解锁请求: CCmsRcsRecordLockOprReq
	CMS_RCS_RECORD_LOCK_OPR_RSP             = 28019,		//录像锁定/解锁应答: CCmsRcsRecordLockOprRsp

    //查询NRU设备，单向接口：USER-->SDK
	CMS_RCS_QUERY_NRU_REQ                   = 28020,        //消息体: CCmsRcsQueryNruReq
	CMS_RCS_QUERY_NRU_RSP                   = 28021,        //消息体: CCmsRcsQueryNruRsp
	CMS_RCS_QUERY_NRU_NTF_REQ               = 28022,        //消息体: CCmsRcsQueryNruNtfReq
	CMS_RCS_QUERY_NRU_NTF_RSP               = 28023,        //消息体: CCmsRcsQueryNruNtfRsp

	//查询在线nru的详细信息，暂不支持分页，单向接口：USER-->SDK
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ     = 28024,   //查询在线nru的详细信息请求: CCmsRcsQueryOnlineNruDetailInfoReq
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP     = 28025,   //查询在线nru的详细信息应答: CCmsRcsQueryOnlineNruDetailInfoRsp
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ = 28026,   //查询在线nru的详细信息通知: CCmsRcsQueryOnlineNruDetailInfoNtfReq
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP = 28027,   //查询在线nru的详细信息通知应答: CCmsRcsQueryOnlineNruDetailInfoNtfRsp

    //磁盘信息查询，单向接口：USER-->SDK
	CMS_RCS_QUERY_DISK_INFO_REQ             = 28028,        //消息体: CCmsRcsQueryDiskInfoReq
	CMS_RCS_QUERY_DISK_INFO_RSP             = 28029,        //消息体: CCmsRcsQueryDiskInfoRsp
	CMS_RCS_QUERY_DISK_INFO_NTF_REQ         = 28030,        //消息体: CCmsRcsQueryDiskInfoNtfReq
	CMS_RCS_QUERY_DISK_INFO_NTF_RSP         = 28031,        //消息体: CCmsRcsQueryDiskInfoNtfRsp

	//磁盘信息订阅，单向接口：USER-->SDK
	CMS_RCS_SUBSCRIBE_DISKINFO_REQ          = 28032,		//磁盘信息订阅请求: CCmsRcsSubscribeDiskInfoReq
	CMS_RCS_SUBSCRIBE_DISKINFO_RSP          = 28033,		//磁盘信息订阅应答: CCmsRcsSubscribeDiskInfoRsp
	CMS_RCS_DISK_PART_ALARM_NTF_REQ         = 28034,		//磁盘分区告警通知：CCmsRcsDiskPartAlarmNtfReq
	CMS_RCS_DISK_PART_ALARM_NTF_RSP         = 28035,		//磁盘分区告警通知应答：CCmsRcsDiskPartAlarmNtfRsp
	CMS_RCS_UNSUBSCRIBE_DISKINFO_REQ        = 28036,		//取消磁盘信息订阅请求: CCmsRcsUnsubscribeDiskInfoReq
	CMS_RCS_UNSUBSCRIBE_DISKINFO_RSP        = 28037,		//取消磁盘信息订阅应答: CCmsRcsUnsubscribeDiskInfoRsp

	//rcs录像状态目录订阅、取消订阅，单向接口:USER-->SDK
	CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ   = 28038,  	//录像状态目录订阅请求：CCmsRcsCatalogSubscribePuStatusReq
	CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP   = 28039,  	//录像状态目录订阅应答：CCmsRcsCatalogSubscribePuStatusRsp
	CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ  = 28040, 		//刷新录像状态目录订阅请求：CCmsRcsCatalogRefreshSsPuStatusReq
	CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP  = 28041, 		//刷新录像状态目录订阅应答：CCmsRcsCatalogRefreshSsPuStatusRsp
	CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ = 28042,		//取消录像状态目录订阅请求：CCmsRcsCatalogUnsubscribePuStatusReq
	CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP = 28043,		//取消录像状态目录订阅应答：CCmsRcsCatalogUnsubscribePuStatusRsp

    //录像参数，暂不支持分页查询
	CMS_RCS_SET_RECORD_PARAM_REQ            = 28044,        //消息体: CCmsRcsSetRecordParamReq
	CMS_RCS_SET_RECORD_PARAM_RSP            = 28045,        //消息体: CCmsRcsSetRecordParamRsp
	CMS_RCS_QUERY_RECORD_PARAM_REQ          = 28046,        //消息体: CCmsRcsQueryRecordParamReq
	CMS_RCS_QUERY_RECORD_PARAM_RSP          = 28047,        //消息体: CCmsRcsQueryRecordParamRsp
    CMS_RCS_QUERY_RECORD_PARAM_NTF_REQ      = 28048,        //消息体: CCmsRcsQueryRecordParamNtfReq
    CMS_RCS_QUERY_RECORD_PARAM_NTF_RSP      = 28049,        //消息体: CCmsRcsQueryRecordParamNtfRsp

 	CMS_RCS_QUERY_RECORD_DAYS_REQ           = 28050,        //消息体: CCmsRcsQueryRecordDaysReq
 	CMS_RCS_QUERY_RECORD_DAYS_RSP           = 28051,        //消息体: CCmsRcsQueryRecordDaysRsp

    CMS_SDK_RCS_MSG_END                     = 28199,

	//与TVS的交换消息(下一个可用的28255)
	CMS_SDK_TVS_MSG_BEGIN				    = 28201,

	//网关设备登录tvs通知，单向接口:USER<--SDK
	CMS_TVS_DISCONNECT_NTF                  = 28202,        //RCS断链通知，cmssdk会自己重连：无
	CMS_TVS_CONNECT_NTF                     = 28203,        //RCS建链通知：TConnInfo

	//电视墙选看，单向接口：USER-->SDK
	CMS_TVS_REAL_MEDIA_ON_TV_REQ            = 28204,		//选看电视墙请求：CCmsRealMediaOnTvReq
	CMS_TVS_REAL_MEDIA_ON_TV_RSP            = 28205,		//选看电视墙应答：CCmsRealMediaOnTvRsp
	CMS_TVS_REAL_MEDIA_OFF_TV_REQ           = 28206,		//停止选看电视墙请求：CCmsRealMediaOffTvReq
	CMS_TVS_REAL_MEDIA_OFF_TV_RSP           = 28207,		//停止选看电视墙应答：CCmsRealMediaOffTvRsp

    //录像上墙，单向接口：USER-->SDK
    CMS_TVS_REC_ON_TV_REQ                   = 28248,        //录像上墙请求: CCmsRecOnTvReq
    CMS_TVS_REC_ON_TV_RSP                   = 28249,        //录像上墙应答: CCmsRecOnTvRsp
    CMS_TVS_REC_ON_TV_ACK                   = 28250,        //录像上墙Ack: CCmsRecOnTvAck

    //停止录像回放，双向接口:USER<-->SDK
    CMS_TVS_REC_ON_TV_BYE_REQ               = 28251,        //录像上墙停止请求：CCmsRecOnTvByeReq
    CMS_TVS_REC_ON_TV_BYE_RSP               = 28252,        //录像上墙停止应答：CCmsRecOnTvByeRsp

	//电视墙轮询设置、获取，单向接口：USER-->SDK
	CMS_TVS_TVWALL_SCHEME_GROUP_SET_REQ     = 28208,		//电视墙轮询设置请求：CCmsTvWallSchemeGroupSetReq
	CMS_TVS_TVWALL_SCHEME_GROUP_SET_RSP     = 28209,		//电视墙轮询设置应答：CCmsTvWallSchemeGroupSetRsp
	CMS_TVS_TVWALL_SCHEME_GROUP_GET_REQ     = 28210,		//电视墙轮询获取请求：CCmsTvWallSchemeGroupGetReq
	CMS_TVS_TVWALL_SCHEME_GROUP_GET_RSP     = 28211,		//电视墙轮询获取应答：CCmsTvWallSchemeGroupGetRsp

	//电视墙轮询，单向接口：USER-->SDK
	CMS_TVS_TV_POLL_OPERATE_REQ             = 28212,		//电视墙轮询请求：CCmsTvPollOperateReq
	CMS_TVS_TV_POLL_OPERATE_RSP             = 28213,		//电视墙轮询应答：CCmsTvPollOperateRsp

	//电视墙预案增、删、查、载入，单向接口：USER-->SDK
	CMS_TVS_TVWALL_SCHEME_SAVE_REQ          = 28214,		//电视墙预案添加请求：CCmsTvWallSchemeSaveReq
	CMS_TVS_TVWALL_SCHEME_SAVE_RSP          = 28215,		//电视墙预案添加应答：CCmsTvWallSchemeSaveRsp
	CMS_TVS_TVWALL_SCHEME_DEL_REQ           = 28216,		//电视墙预案删除请求：CCmsTvWallSchemeDelReq
	CMS_TVS_TVWALL_SCHEME_DEL_RSP           = 28217,		//电视墙预案删除应答：CCmsTvWallSchemeDelRsp
	CMS_TVS_TVWALL_SCHEME_QRY_REQ           = 28218,		//电视墙预案查询请求：CCmsTvWallSchemeQryReq
	CMS_TVS_TVWALL_SCHEME_QRY_RSP           = 28219,		//电视墙预案查询应答：CCmsTvWallSchemeQryRsp
	CMS_TVS_TVWALL_SCHEME_QRY_NTF_REQ       = 28220,		//电视墙预案查询通知：CCmsTvWallSchemeQryNtfReq
	CMS_TVS_TVWALL_SCHEME_QRY_NTF_RSP       = 28221,		//电视墙预案查询通知应答：CCmsTvWallSchemeQryNtfRsp
	CMS_TVS_TVWALL_SCHEME_LOAD_REQ          = 28222,		//电视墙预案载入请求：CCmsTvWallSchemeLoadReq
	CMS_TVS_TVWALL_SCHEME_LOAD_RSP          = 28223,		//电视墙预案载入应答：CCmsTvWallSchemeLoadRsp

	//电视墙订阅、取消订阅，单向接口：USER-->SDK
	CMS_TVS_TVWALL_STATUS_SS_REQ            = 28224,		//电视墙订阅请求：CCmsTvWallStatusSsReq
	CMS_TVS_TVWALL_STATUS_SS_RSP            = 28225,		//电视墙订阅应答：CCmsTvWallStatusSsRsp
	CMS_TVS_TVWALL_STATUS_UNSS_REQ          = 28226,		//电视墙取消订阅请求：CCmsTvWallStatusUnSsReq
	CMS_TVS_TVWALL_STATUS_UNSS_RSP          = 28227,		//电视墙取消订阅应答：CCmsTvWallStatusUnSsRsp
	CMS_TVS_TVWALL_STATUS_NTF_REQ           = 28228,		//电视墙订阅通知：CCmsTvWallStatusNtyReq
	CMS_TVS_TVWALL_STATUS_NTF_RSP           = 28229,		//电视墙订阅通知应答：CCmsTvWallStatusNtyRsp

	//电视墙解码风格设置、获取，单向接口：USER-->SDK
	CMS_TVS_TV_WIN_STYLE_SET_REQ            = 28230,		//电视墙解码风格设置请求: CCmsTvWinStyleSetReq
	CMS_TVS_TV_WIN_STYLE_SET_RSP            = 28231,		//电视墙解码风格设置应答: CCmsTvWinStyleSetRsp
	CMS_TVS_TV_WIN_STYLE_GET_REQ            = 28232,		//电视墙解码风格获取请求: CCmsTvWinStyleGetReq
	CMS_TVS_TV_WIN_STYLE_GET_RSP            = 28233,		//电视墙解码风格获取应答: CCmsTvWinStyleGetRsp

    //解码器选看通道锁定,解锁，单向接口：USER-->SDK
    CMS_TV_CHANNEL_LOCK_REQ                 = 28234,        //消息体: CCmsTvChannelLockReq
    CMS_TV_CHANNEL_LOCK_RSP                 = 28235,        //消息体: CCmsTvChannelLockRsp
    
    CMS_TV_CHANNEL_UNLOCK_REQ               = 28236,        //消息体: CCmsTvChannelUnlockReq
    CMS_TV_CHANNEL_UNLOCK_RSP               = 28237,        //消息体: CCmsTvChannelUnlockRsp
    
    //解码器通道观看点状态保存，加载,，单向接口：USER-->SDK
	CMS_TV_CHANNEL_WATCH_POINT_SAVE_REQ     = 28238,        //消息体: CCmsTvChannelWatchPointSaveReq
	CMS_TV_CHANNEL_WATCH_POINT_SAVE_RSP     = 28239,        //消息体: CCmsTvChannelWatchPointSaveRsp
    
    CMS_TV_CHANNEL_WATCH_POINT_LOAD_REQ     = 28240,         //消息体: CCmsTvChannelWatchPointLoadReq
    CMS_TV_CHANNEL_WATCH_POINT_LOAD_RSP     = 28241,         //消息体: CCmsTvChannelWatchPointLoadRsp
    
    CMS_TV_CHANNEL_WATCH_INFO_GET_REQ       = 28242,         //消息体: CCmsTvChannelWatchInfoGetReq
    CMS_TV_CHANNEL_WATCH_INFO_GET_RSP       = 28243,         //消息体: CCmsTvChannelWatchInfoGetRsp
    
    //电视墙预案锁定，解锁，单向接口：USER-->SDK
    CMS_TVWALL_SCHEME_LOCK_REQ              = 28244,           //消息体: CCmsTvWallSchemeLockReq
    CMS_TVWALL_SCHEME_LOCK_RSP              = 28245,           //消息体: CCmsTvWallSchemeLockRsp
    
    CMS_TVWALL_SCHEME_UNLOCK_REQ            = 28246,           //消息体: CCmsTvWallSchemeUnlockReq
    CMS_TVWALL_SCHEME_UNLOCK_RSP            = 28247,           //消息体: CCmsTvWallSchemeUnlockRsp

    // 单向接口：USER-->SDK
    CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ    = 28253,           //消息体：CCmsRealMediaSwitchOccurTypeReq
    CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP    = 28254,           //消息体：CCmsRealMediaSwitchOccurTypeRsp

	CMS_SDK_TVS_MSG_END                     = 28399,

	//与UAS的交换消息
	CMS_SDK_UAS_MSG_BEGIN				    = 28401,

	//网关设备登录uas通知，单向接口:USER<--SDK
	CMS_UAS_DISCONNECT_NTF                  = 28402,           //UAS断链通知，cmssdk会自己重连：无
	CMS_UAS_CONNECT_NTF                     = 28403,           //UAS建链通知：TConnInfo

	//视频源预置位添加、删除、查询操作，单向接口:USER-->SDK
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ     = 28404,		  //预置位添加请求：CCmsVideosourcePresetcfgAddReq，配置存在时进行修改
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP     = 28405,		  //预置位添加应答：CCmsVideosourcePresetcfgAddRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ = 28406,		  //预置位添加通知：CCmsVideosourcePresetcfgAddNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP = 28407,		  //预置位添加通知应答：CCmsVideosourcePresetcfgAddNtfRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ     = 28408,		  //预置位删除请求：CCmsVideosourcePresetcfgDelReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP     = 28409,		  //预置位删除应答：CCmsVideosourcePresetcfgDelRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ = 28410,		  //预置位删除通知：CCmsVideosourcePresetcfgDelNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP = 28411,		  //预置位删除通知应答：CCmsVideosourcePresetcfgDelNtfRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ     = 28412,		  //预置位查询请求：CCmsVideosourcePresetcfgQryReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP     = 28413,		  //预置位查询应答：CCmsVideosourcePresetcfgQryRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ = 28414,		  //预置位查询通知：CCmsVideosourcePresetcfgQryNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP = 28415,		  //预置位查询通知应答：CCmsVideosourcePresetcfgQryNtfRsp

    //用户操作日志添加、查询，单向接口
    CMS_UAS_OPERATE_LOG_ADD_REQ               = 28416,        // 消息体 CCmsOperateLogAddReq 操作日志管理
    CMS_UAS_OPERATE_LOG_ADD_RSP               = 28417,        // 消息体 CCmsOperateLogAddRsp
    CMS_UAS_OPERATE_LOG_QRY_REQ               = 28418,        // 消息体 CCmsOperateLogQryReq
    CMS_UAS_OPERATE_LOG_QRY_RSP               = 28419,        // 消息体 CCmsOperateLogQryRsp
    CMS_UAS_OPERATE_LOG_QRY_NTF_REQ           = 28420,        // 消息体 CCmsOperateLogQryNtfReq
    CMS_UAS_OPERATE_LOG_QRY_NTF_RSP           = 28421,        // 消息体 CCmsOperateLogQryNtfRsp

	CMS_SDK_UAS_MSG_END                       = 28599,

    CMS_SDK_ALS_MSG_BEGIN                     = 28601,

    //告警联动，布防撤防
    CMS_ALARMLINK_CFG_SET_REQ             = 28602,     //消息体: CCmsAlarmLinkCfgSetReq
    CMS_ALARMLINK_CFG_SET_RSP             = 28603,     //消息体: CCmsAlarmLinkCfgSetRsp

    CMS_ALARMLINK_CFG_GET_REQ             = 28604,     //消息体: CCmsAlarmLinkCfgGetReq
    CMS_ALARMLINK_CFG_GET_RSP             = 28605,     //消息体: CCmsAlarmLinkCfgGetRsp

    //网关设备登录ALS，单向接口:USER<--SDK
    CMS_ALS_DISCONNECT_NTF                = 28606,    //ALS断链通知，cmssdk会自己重连：无
    CMS_ALS_CONNECT_NTF                   = 28607,    //ALS建链通知：TConnInfo

	CMS_SUBSCRIBE_TERMINATE_NTF           = 28608,    //订阅异常终止通知:无

    CMS_SDK_ALS_MSG_END                       = 28799,

	CMS_SDK_MSG_END                           = 28999,                                    //CmsSdk消息定义结束
};


#endif // __CMS_SDK_EVENT_H__









