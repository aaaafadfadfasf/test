/*****************************************************************************
   模块名      : cmu_event
   文件名      : cmu_event.h
   相关文件    : 
   文件实现功能: cmu_event.h 定义了所有3AC_3AS的消息ID
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli        创建
******************************************************************************/
#ifndef __CMU_EVENT_H__
#define __CMU_EVENT_H__

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum ECMS_EVENT
{
    PUI_REG_REQ = CMS_EVENT_BEGIN,             //消息体: CPuiRegReq
    PUI_REG_RSP,             //消息体: CPuiRegRsp

    PU_REG_REQ,              //消息体: CPuRegReq
    PU_REG_RSP,              //消息体: CPuRegRsp

    PU_UNREG_REQ,            //消息体: CPuUnRegReq
    PU_UNREG_RSP,            //消息体: CPuUnRegRsp

    CMU_DISCONNECT_PU_REQ,   //消息体: CCmuDisconnectPuReq
    CMU_DISCONNECT_PU_RSP,   //消息体: CCmuDisconnectPuRsp

    CU_LOGIN_REQ,            //消息体: CCuLoginReq
    CU_LOGIN_RSP,            //消息体: CCuLoginRsp

    CU_LOGOUT_REQ,           //消息体: CCuLogoutReq
    CU_LOGOUT_RSP,           //消息体: CCuLogoutRsp

    CU_DISCONNECT_NTF_REQ,   //消息体: CCuDisconnectNtfReq
    CU_DISCONNECT_NTF_RSP,   //消息体: CCuDisconnectNtfRsp

    CU_GETCUILIST_REQ,       //消息体: CCuGetCuiListReq
    CU_GETCUILIST_RSP,       //消息体: CCuGetCuiListRsp

    CUI_REG_REQ,             //消息体: CCuiRegReq
    CUI_REG_RSP,             //消息体: CCuiRegRsp

	CUI_GET_AZIMUTH_REQ,     //消息体: CCuiGetAzimuthReq
	CUI_GET_AZIMUTH_RSP,     //消息体: CCuiGetAzimuthRsp

    BATCH_OPERATE_REQ,      // 消息体：CBatchOperateReq
    BATCH_OPERATE_RSP,      // 消息体：CBatchOperateRsp

    BATCH_PU_REG_REQ,      // 消息体：CBatchPuRegReq
    BATCH_PU_REG_RSP,      // 消息体：CBatchPuRegRsp

    //整体订阅(目录订阅)
    CATALOG_STATUS_SS_REQ,        //消息体: CCatalogStatusSsReq
    CATALOG_STATUS_SS_RSP,        //消息体: CCatalogStatusSsRsp

    CATALOG_STATUS_REFRESH_SS_REQ,        //消息体: CCatalogStatusRefreshSsReq
    CATALOG_STATUS_REFRESH_SS_RSP,        //消息体: CCatalogStatusRefreshSsRsp

    CATALOG_STATUS_UNSS_REQ,        //消息体: CCatalogStatusUnSsReq
    CATALOG_STATUS_UNSS_RSP,        //消息体: CCatalogStatusUnSsRsp

    //整体订阅(列表订阅)
    PULIST_STATUS_SS_REQ,        //消息体: CPuListStatusSsReq
    PULIST_STATUS_SS_RSP,        //消息体: CPuListStatusSsRsp

    PULIST_STATUS_REFRESH_SS_REQ,        //消息体: CPuListStatusRefreshSsReq
    PULIST_STATUS_REFRESH_SS_RSP,        //消息体: CPuListStatusRefreshSsRsp

    PULIST_STATUS_UNSS_REQ,        //消息体: CPuListStatusUnSsReq
    PULIST_STATUS_UNSS_RSP,        //消息体: CPuListStatusUnSsRsp

	/*
     * 单点订阅即将被废弃，停止使用【xiongxh】
     */
    //单个设备订阅
    PU_STATUS_SS_REQ,        //消息体: CPuStatusSsReq
    PU_STATUS_SS_RSP,        //消息体: CPuStatusSsRsp

    PU_STATUS_REFRESH_SS_REQ,        //消息体: CPuStatusRefreshSsReq
    PU_STATUS_REFRESH_SS_RSP,        //消息体: CPuStatusRefreshSsRsp

    PU_STATUS_UNSS_REQ,        //消息体: CPuStatusUnSsReq
    PU_STATUS_UNSS_RSP,        //消息体: CPuStatusUnSsRsp

    PU_STATUS_NTF_REQ,       //消息体: CPuStatusNtyReq
    PU_STATUS_NTF_RSP,       //消息体: CPuStatusNtyRsp

    //pui通过MESSAGE方法向cmu上报前端配置，cmu处理该消息
    PU_DEV_CONFIG_NTF_REQ,       //消息体: CPuDevConfigNtyReq
    PU_DEV_CONFIG_NTF_RSP,       //消息体: CPuDevConfigNtyRsp

    //cmu通过NOTIFY方法向订阅者发布前端配置，订阅者处理该消息
    PU_CONFIG_NTF_REQ,       //消息体: CPuConfigNtyReq
    PU_CONFIG_NTF_RSP,       //消息体: CPuConfigNtyRsp

    PU_SERVICE_STATUS_NTF_REQ,       //消息体: CPuServiceStatusNtyReq
    PU_SERVICE_STATUS_NTF_RSP,       //消息体: CPuServiceStatusNtyRsp

    PU_ALARM_STATUS_NTF_REQ,       //消息体: CPuAlarmStatusNtyReq
    PU_ALARM_STATUS_NTF_RSP,       //消息体: CPuAlarmStatusNtyRsp

    PU_GPS_STATUS_NTF_REQ,       //消息体: CPuGpsStatusNtyReq
    PU_GPS_STATUS_NTF_RSP,       //消息体: CPuGpsStatusNtyRsp

    //该接口用于cmu向订阅者发布透明数据状态(使用的是SUBSCRIBE+NOTFY方法)
    PU_TRANSDATA_STATUS_NTF_REQ,      //消息体: CPuTransDataStatusNtyReq
    PU_TRANSDATA_STATUS_NTF_RSP,      //消息体: CPuTransDataStatusNtyRsp

	// 批量通知
	PU_BATCH_NTF_REQ,			//消息体：CPuBatchNtyReq
	PU_BATCH_NTF_RSP,			//消息体：CPuBatchNtyRsp

    PU_PTZ_CTRL_REQ,         //消息体: CPuPtzCtrlReq
    PU_PTZ_CTRL_RSP,         //消息体: CPuPtzCtrlRsp

    PU_PTZ_LOCK_REQ,         //消息体: CPuPtzLockReq
    PU_PTZ_LOCK_RSP,         //消息体: CPuPtzLockRsp

    PU_PTZ_UNLOCK_REQ,       //消息体: CPuPtzUnlockReq
    PU_PTZ_UNLOCK_RSP,       //消息体: CPuPtzUnlockRsp

    //该接口没有使用，考虑废弃，避免产生歧义
    PU_SEND_TRANSPARENT_DATA_REQ, //消息体: CPuSendTransparentDataReq
    PU_SEND_TRANSPARENT_DATA_RSP, //消息体: CPuSendTransparentDataRsp

    PU_VID_ENC_PARAM_SET_REQ,     //消息体: CPuVidEncParamSetReq
    PU_VID_ENC_PARAM_SET_RSP,     //消息体: CPuVidEncParamSetRsp

    PU_VID_ENC_PARAM_GET_REQ,     //消息体: CPuVidEncParamGetReq
    PU_VID_ENC_PARAM_GET_RSP,     //消息体: CPuVidEncParamGetRsp

	PU_VID_ENC_PARAM_SET_EX_REQ,     //消息体: CPuVidEncParamExSetReq
	PU_VID_ENC_PARAM_SET_EX_RSP,     //消息体: CPuVidEncParamExSetRsp

	PU_VID_ENC_PARAM_GET_EX_REQ,     //消息体: CPuVidEncParamGetExReq
	PU_VID_ENC_PARAM_GET_EX_RSP,     //消息体: CPuVidEncParamGetExRsp

    PU_AUD_ENC_PARAM_SET_REQ,     //消息体: CPuAudEncParamSetReq
    PU_AUD_ENC_PARAM_SET_RSP,     //消息体: CPuAudEncParamSetRsp

    PU_AUD_ENC_PARAM_GET_REQ,     //消息体: CPuAudEncParamGetReq
    PU_AUD_ENC_PARAM_GET_RSP,     //消息体: CPuAudEncParamGetRsp

    //该接口使用VSIP隧道实现在cu端直接使用pu的客户端设置前端配置
    PU_VSIP_OPERATE_REQ,      //消息体: CPuVsipOperateReq
    PU_VSIP_OPERATE_RSP,      //消息体: CPuVsipOperateRsp

    //该接口透传CU对PU的透明操作
    PU_TRANSPARENT_OPERATE_REQ,      //消息体: CPuTransparentOperateReq
    PU_TRANSPARENT_OPERATE_RSP,      //消息体: CPuTransparentOperateRsp

    //该接口支持req-rsp-ntf模型的透明数据(比如支持IPCV7的HTTP隧道)
    PU_TRANSPARENT_QRY_REQ,           //消息体: CPuTransparentQryReq
    PU_TRANSPARENT_QRY_RSP,           //消息体: CPuTransparentQryRsp
    PU_TRANSPARENT_QRY_NTF_REQ,       //消息体: CPuTransparentQryNtfReq
    PU_TRANSPARENT_QRY_NTF_RSP,       //消息体: CPuTransparentQryNtfRsp

    //该接口用于pui向cmu上报透明数据(使用的是MESSAGE方法)
    PU_TRANSPARENT_DATA_NTF_REQ,      //消息体: CPuTransparentDataNtfReq
    PU_TRANSPARENT_DATA_NTF_RSP,      //消息体: CPuTransparentDataNtfRsp

    PU_SAVE_CFG_REQ,          //消息体: CPuSaveCfgReq
    PU_SAVE_CFG_RSP,          //消息体: CPuSaveCfgRsp

    PU_SYS_PARAM_SET_REQ,     //消息体: CPuSysParamSetReq
    PU_SYS_PARAM_SET_RSP,     //消息体: CPuSysParamSetRsp

    PU_SYS_PARAM_GET_REQ,     //消息体: CPuSysParamGetReq
    PU_SYS_PARAM_GET_RSP,     //消息体: CPuSysParamGetRsp

    PU_SEND_REAL_MEDIA_REQ,   //消息体: CPuSendRealMediaReq
    PU_SEND_REAL_MEDIA_RSP,   //消息体: CPuSendRealMediaRsp
    PU_SEND_REAL_MEDIA_ACK,   //消息体: CInviteThirdPartyCallAck

    PU_STOP_SEND_REAL_MEDIA_REQ,   //消息体: CPuStopSendRealMediaReq
    PU_STOP_SEND_REAL_MEDIA_RSP,   //消息体: CPuStopSendRealMediaRsp

    PU_REBOOT_REQ,            //消息体: CPuRebootReq
    PU_REBOOT_RSP,            //消息体: CPuRebootRsp

    INVITE_REQ,             //消息体: CInviteReq
    INVITE_RSP,             //消息体: CInviteRsp
    INVITE_ACK,             //消息体: CInviteACK

	BYE_REQ,				//消息体: CByeReq
	BYE_RSP,				//消息体: CByeRsp

    //{{Message方法的第三方呼叫
    START_THIRD_PARTY_CALL_REQ,     //消息体: CStartThirdPartyCallReq
    START_THIRD_PARTY_CALL_RSP,     //消息体: CStartThirdPartyCallRsp ,不带媒体信息

    STOP_THIRD_PARTY_CALL_REQ,      //消息体: CStopThirdPartyCallReq
    STOP_THIRD_PARTY_CALL_RSP,      //消息体: CStopThirdPartyCallRsp
    //}}

    //{{Invite方法的第三方呼叫,停止也是用Bye
    INVITE_THIRD_PARTY_CALL_REQ,    //消息体: CInviteThirdPartyCallReq
    INVITE_THIRD_PARTY_CALL_RSP,    //消息体: CInviteThirdPartyCallRsp
    INVITE_THIRD_PARTY_CALL_ACK,    //消息体: CInviteThirdPartyCallAck
    //}}

	//更新已经建好的交换地址
	UPDATE_SWITCH_ADDRESS_REQ,    //消息体：CUpdateSwitchAddressReq
	UPDATE_SWITCH_ADDRESS_RSP,    //消息体：CUpdateSwitchAddressRsp

    //关键帧
    KEY_FRAME_REQ,                  //消息体: CKeyFrameReq
    KEY_FRAME_RSP,                  //消息体: CKeyFrameRsp

    //放像进度查询
    PLAYBACK_PROGRESS_QUERY_REQ,      //消息体: CPlaybackProgressQueryReq
    PLAYBACK_PROGRESS_QUERY_RSP,      //消息体: CPlaybackProgressQueryRsp

    PLAYBACK_PROGRESS_NTF_REQ,      //消息体: CPlaybackProgressNtfReq
    PLAYBACK_PROGRESS_NTF_RSP,      //消息体: CPlaybackProgressNtfRsp

    //会话内message，HZCTODO：CEndOfFileNtfReq::playbackSessUri可以去除
    END_OF_FILE_NTF_REQ,            //消息体: CEndOfFileNtfReq
    END_OF_FILE_NTF_RSP,            //消息体: CEndOfFileNtfRsp

    //放像控制
    VCR_CTRL_REQ,                   //消息体: CVcrCtrlReq
    VCR_CTRL_RSP,                   //消息体: CVcrCtrlRsp

    //cmu之间的建链请求
    CMU_CONNECT_REQ,        //消息体: CCmuConnectReq
    CMU_CONNECT_RSP,        //消息体: CCmuConnectRsp

    //cmu之间的断链通知
    CMU_DISCONNECT_NTF_REQ,        //消息体: CCmuDisconnectNtfReq
    CMU_DISCONNECT_NTF_RSP,        //消息体: CCmuDisconnectNtfRsp

    //作为通用的设备登录请求，消息体应包括: devUri, devType, devAddrList
    DEV_LOGIN_REQ,          //消息体: CDevLoginReq
    DEV_LOGIN_RSP,          //消息体: CDevLoginRsp

    DEV_LOGOUT_REQ,         //消息体: CDevLogoutReq
    DEV_LOGOUT_RSP,         //消息体: CDevLogoutRsp

    //获取模块的IP地址信息
    NETADDR_QUERY_REQ,       //消息体: CNetAddrQueryReq
    NETADDR_QUERY_RSP,       //消息体: CNetAddrQueryRsp

    //使用INVITE方法作为通用文件上传、下载请求
    //下载
    FILE_EXPORT_REQ,          //消息体: CFileExportReq
    FILE_EXPORT_RSP,          //消息体: CFileExportRsp
    FILE_EXPORT_ACK,          //消息体: CFileExportAck

    FILE_COLLECT_PROGRESS_NTF_REQ,         //消息体: CFileCollectProgressNtfReq
    FILE_COLLECT_PROGRESS_NTF_RSP,         //消息体: CFileCollectProgressNtfRsp

    //上传
    FILE_IMPORT_REQ,          //消息体: CFileImportReq
    FILE_IMPORT_RSP,          //消息体: CFileImportRsp
    FILE_IMPORT_ACK,          //消息体: CFileImportAck

    FILE_TRANS_END_REQ,      //消息体: CFileTransEndReq
    FILE_TRANS_END_RSP,      //消息体: CFileTransEndRsp

    STUN_UPDATE_REQ,        //消息体: CStunUpdateReq
    STUN_UPDATE_RSP,        //消息体: CStunUpdateRsp

    //告警、告警联动相关
    PU_ALARM_INPUT_NTF_REQ, //消息体: CPuAlarmInputNtyReq
    PU_ALARM_INPUT_NTF_RSP, //消息体: CPuAlarmInputNtyRsp

    PU_READ_SEC_REQ,        //消息体: CPuReadSecReq
    PU_READ_SEC_RSP,        //消息体: CPuReadSecRsp

    PU_IO_OUT_REQ,       //消息体: CPuIoOutReq
    PU_IO_OUT_RSP,       //消息体: CPuIoOutRsp

	//告警恢复
	PU_ALARM_RESET_REQ,  //消息体：CPuAlarmResetReq
	PU_ALARM_RESET_RSP,  //消息体：CPuAlarmResetRsp

    //PU_CAM_PREPOS_REQ = PU_PTZ_CTRL_REQ,      //消息体: CPuPtzCtrlReq
    //PU_CAM_PREPOS_RSP = PU_PTZ_CTRL_RSP,      //消息体: CPuPtzCtrlRsp  

    ALARMLINK_CFG_SET_REQ,     //消息体: CAlarmLinkCfgSetReq
    ALARMLINK_CFG_SET_RSP,     //消息体: CAlarmLinkCfgSetRsp

    ALARMLINK_CFG_GET_REQ,     //消息体: CAlarmLinkCfgGetReq
    ALARMLINK_CFG_GET_RSP,     //消息体: CAlarmLinkCfgGetRsp

    ALARMLINK_VERSION_GET_REQ,  //消息体：CAlarmLinkVersionGetReq
    ALARMLINK_VERSION_GET_RSP,  //消息体：CAlarmLinkVersionGetRsp

    ALARMLINK_QUERY_REQ,           //消息体: CAlarmLinkQueryReq
    ALARMLINK_QUERY_RSP,           //消息体: CAlarmLinkQueryRsp

    ALARMLINK_NTF_REQ,             //消息体: CAlarmLinkNtfReq
    ALARMLINK_NTF_RSP,             //消息体: CAlarmLinkNtfRsp

    ALARMLINK_MOD_NTF_REQ,             //消息体: CAlarmLinkModNtfReq
    ALARMLINK_MOD_NTF_RSP,             //消息体: CAlarmLinkModNtfRsp


    //电视墙相关
    REAL_MEDIA_ON_TV_REQ,      //消息体: CRealMediaOnTvReq
    REAL_MEDIA_ON_TV_RSP,      //消息体: CRealMediaOnTvRsp

    REAL_MEDIA_OFF_TV_REQ,     //消息体: CRealMediaOffTvReq
    REAL_MEDIA_OFF_TV_RSP,     //消息体: CRealMediaOffTvRsp

    //录像上墙
    REC_ON_TV_REQ,              //消息体: CRecOnTvReq
    REC_ON_TV_RSP,              //消息体: CRecOnTvRsp
    REC_ON_TV_ACK,              //消息体: CRecOnTvAck

    TV_POLL_OPERATE_REQ,       //消息体: CTvPollOperateReq
    TV_POLL_OPERATE_RSP,       //消息体: CTvPollOperateRsp

    TV_WIN_STYLE_SET_REQ,      //消息体: CTvWinStyleSetReq
    TV_WIN_STYLE_SET_RSP,      //消息体: CTvWinStyleSetRsp

    TV_WIN_STYLE_GET_REQ,      //消息体: CTvWinStyleGetReq
    TV_WIN_STYLE_GET_RSP,      //消息体: CTvWinStyleGetRsp

    TVWALL_SCHEME_SAVE_REQ,            //消息体: CTvWallSchemeSaveReq
    TVWALL_SCHEME_SAVE_RSP,            //消息体: CTvWallSchemeSaveRsp

    TVWALL_SCHEME_DEL_REQ,             //消息体: CTvWallSchemeDelReq
    TVWALL_SCHEME_DEL_RSP,             //消息体: CTvWallSchemeDelRsp

    TVWALL_SCHEME_QUERY_REQ,           //消息体: CTvWallSchemeQueryReq
    TVWALL_SCHEME_QUERY_RSP,           //消息体: CTvWallSchemeQueryRsp

    TVWALL_SCHEME_NTF_REQ,             //消息体: CTvWallSchemeNtfReq
    TVWALL_SCHEME_NTF_RSP,             //消息体: CTvWallSchemeNtfRsp

    TVWALL_SCHEME_LOAD_REQ,            //消息体: CTvWallSchemeLoadReq
    TVWALL_SCHEME_LOAD_RSP,            //消息体: CTvWallSchemeLoadRsp

    TVWALL_SCHEME_GROUP_SET_REQ,       //消息体: CTvWallSchemeGroupSetReq
    TVWALL_SCHEME_GROUP_SET_RSP,       //消息体: CTvWallSchemeGroupSetRsp

    TVWALL_SCHEME_GROUP_GET_REQ,       //消息体: CTvWallSchemeGroupGetReq
    TVWALL_SCHEME_GROUP_GET_RSP,       //消息体: CTvWallSchemeGroupGetRsp

    TVWALL_SCHEME_POLL_OPERATE_REQ,    //消息体: CTvWallSchemePollOperateReq
    TVWALL_SCHEME_POLL_OPERATE_RSP,    //消息体: CTvWallSchemePollOperateRsp

    PLAT_REC_ON_TV_REQ,      //消息体: CPlatRecOnTvReq
    PLAT_REC_ON_TV_RSP,      //消息体: CPlatRecOnTvRsp

    PLAT_REC_OFF_TV_REQ,     //消息体: CPlatRecOffTvReq
    PLAT_REC_OFF_TV_RSP,     //消息体: CPlatRecOffTvRsp

    PU_REC_ON_TV_REQ,        //消息体: CPuRecOnTvReq
    PU_REC_ON_TV_RSP,        //消息体: CPuRecOnTvRsp

    PU_REC_OFF_TV_REQ,       //消息体: CPuRecOffTvReq  (暂不实现)
    PU_REC_OFF_TV_RSP,       //消息体: CPuRecOffTvRsp  (暂不实现)

    TVWALL_STATUS_SS_REQ,    //消息体: CTvWallStatusSsReq
    TVWALL_STATUS_SS_RSP,    //消息体: CTvWallStatusSsRsp

    TVWALL_STATUS_UNSS_REQ,    //消息体: CTvWallStatusUnSsReq
    TVWALL_STATUS_UNSS_RSP,    //消息体: CTvWallStatusUnSsRsp

    TVWALL_STATUS_NTF_REQ,   //消息体: CTvWallStatusNtyReq
    TVWALL_STATUS_NTF_RSP,   //消息体: CTvWallStatusNtyRsp

    //UOI中使用的接口---->begin
    TV_CHANNEL_LOCK_REQ,              //消息体: CTvChannelLockReq
    TV_CHANNEL_LOCK_RSP,              //消息体: CTvChannelLockRsp

    TV_CHANNEL_UNLOCK_REQ,            //消息体: CTvChannelUnlockReq
    TV_CHANNEL_UNLOCK_RSP,            //消息体: CTvChannelUnlockRsp

    TV_CHANNEL_WATCH_POINT_SAVE_REQ,  //消息体: CTvChannelWatchPointSaveReq
    TV_CHANNEL_WATCH_POINT_SAVE_RSP,  //消息体: CTvChannelWatchPointSaveRsp

    TV_CHANNEL_WATCH_POINT_LOAD_REQ,  //消息体: CTvChannelWatchPointLoadReq
    TV_CHANNEL_WATCH_POINT_LOAD_RSP,  //消息体: CTvChannelWatchPointLoadRsp

    TV_CHANNEL_WATCH_INFO_GET_REQ,    //消息体: CTvChannelWatchInfoGetReq
    TV_CHANNEL_WATCH_INFO_GET_RSP,    //消息体: CTvChannelWatchInfoGetRsp

    TVWALL_SCHEME_LOCK_REQ,           //消息体: CTvWallSchemeLockReq
    TVWALL_SCHEME_LOCK_RSP,           //消息体: CTvWallSchemeLockRsp

    TVWALL_SCHEME_UNLOCK_REQ,         //消息体: CTvWallSchemeUnlockReq
    TVWALL_SCHEME_UNLOCK_RSP,         //消息体: CTvWallSchemeUnlockRsp
    //UOI中使用的接口---->end

	//摄像头类型
    PU_CAMERA_TYPE_SET_REQ,  //消息体: CPuCameraTypeSetReq
    PU_CAMERA_TYPE_SET_RSP,  //消息体: CPuCameraTypeSetRsp
	
    //移动侦测
    PU_MOTION_DETECT_SET_REQ,  //消息体: CPuMotionDetectSetReq
    PU_MOTION_DETECT_SET_RSP,  //消息体: CPuMotionDetectSetRsp

    PU_MOTION_DETECT_GET_REQ,  //消息体: CPuMotionDetectGetReq
    PU_MOTION_DETECT_GET_RSP,  //消息体: CPuMotionDetectGetRsp

    //图像遮蔽
    PU_AREA_SHADE_SET_REQ,    //消息体: CPuAreaShadeSetReq
    PU_AREA_SHADE_SET_RSP,    //消息体: CPuAreaShadeSetRsp

    PU_AREA_SHADE_GET_REQ,    //消息体: CPuAreaShadeGetReq
    PU_AREA_SHADE_GET_RSP,    //消息体: CPuAreaShadeGetRsp

    //GPS订阅
    PU_GPS_CFG_SET_REQ,        //消息体: CPuGpsCfgSetReq
    PU_GPS_CFG_SET_RSP,        //消息体: CPuGpsCfgSetRsp

    PU_GPS_CFG_GET_REQ,        //消息体: CPuGpsCfgGetReq
    PU_GPS_CFG_GET_RSP,        //消息体: CPuGpsCfgGetRsp

    PU_GPS_SS_SET_REQ,        //消息体: CPuGpsSsSetReq
    PU_GPS_SS_SET_RSP,        //消息体: CPuGpsSsSetRsp

    PU_GPS_SS_GET_REQ,        //消息体: CPuGpsSsGetReq
    PU_GPS_SS_GET_RSP,        //消息体: CPuGpsSsGetRsp

    PU_GPS_DATA_NTF_REQ,      //消息体: CPuGpsDataNtyReq
    PU_GPS_DATA_NTF_RSP,      //消息体: CPuGpsDataNtyRsp

    //透明数据配置的设置、获取
    PU_TRANSDATA_CFG_SET_REQ,        //消息体: CPuTransdataCfgSetReq
    PU_TRANSDATA_CFG_SET_RSP,        //消息体: CPuTransdataCfgSetRsp

    PU_TRANSDATA_CFG_GET_REQ,        //消息体: CPuTransdataCfgGetReq
    PU_TRANSDATA_CFG_GET_RSP,        //消息体: CPuTransdataCfgGetRsp

    //前端录像开启、停止
    PU_RECORD_START_REQ,      //消息体: CPuRecordStartReq
    PU_RECORD_START_RSP,      //消息体: CPuRecordStartRsp

	//{{{{{{{{{{{{{{{ add by Royan 20140526 前端录像状态通知
	PU_RECORED_STATUS_NTF_REQ,   //消息体: CPuRecordStatusNtfReq
	PU_RECORED_STATUS_NTF_RSP,   //消息体: CPuRecordStatusNtfRsp
	//}}}}}}}}}}}}}}} add by Royan 20140526 前端录像状态通知

    PU_RECORD_STOP_REQ,       //消息体: CPuRecordStopReq
    PU_RECORD_STOP_RSP,       //消息体: CPuRecordStopRsp

    //前端录像删除
    PU_RECORD_DEL_REQ,       //消息体: CPuRecordDelReq
    PU_RECORD_DEL_RSP,       //消息体: CPuRecordDelRsp

    //前端录像查询
    PU_REC_QRY_REQ,           //消息体: CPuRecQryReq
    PU_REC_QRY_RSP,           //消息体: CPuRecQryRsp
    PU_REC_QRY_NTF_REQ,       //消息体: CPuRecQryNtfReq
    PU_REC_QRY_NTF_RSP,       //消息体: CPuRecQryNtfRsp

    //前端录像日历查询
    PU_REC_DAYS_QRY_REQ,          //消息体: CPuRecDaysQryReq
    PU_REC_DAYS_QRY_RSP,          //消息体: CPuRecDaysQryRsp

    //解码器风格配置、获取
    PU_DEC_STYLE_SET_REQ,     //消息体: CPuDecStyleSetReq
    PU_DEC_STYLE_SET_RSP,     //消息体: CPuDecStyleSetRsp

    PU_DEC_STYLE_GET_REQ,     //消息体: CPuDecStyleGetReq
    PU_DEC_STYLE_GET_RSP,     //消息体: CPuDecStyleGetRsp

    //前端图片抓拍
    PU_PIC_SNAPSHOT_REQ,      //消息体: CPuPicSnapshotReq
    PU_PIC_SNAPSHOT_RSP,      //消息体: CPuPicSnapshotRsp

    //前端图片下载
    PU_PIC_DOWNLOAD_REQ,      //消息体: CPuPicDownloadReq
    PU_PIC_DOWNLOAD_RSP,      //消息体: CPuPicDownloadRsp

    //前端图片查询
    PU_PIC_QRY_REQ,           //消息体: CPuPicQryReq
    PU_PIC_QRY_RSP,           //消息体: CPuPicQryRsp
    PU_PIC_QRY_NTF_REQ,       //消息体: CPuPicQryNtfReq
    PU_PIC_QRY_NTF_RSP,       //消息体: CPuPicQryNtfRsp

    //前端长字幕设置、获取
    PU_LONG_OSD_SET_REQ,      //消息体: CPuLongOsdSetReq
    PU_LONG_OSD_SET_RSP,      //消息体: CPuLongOsdSetRsp
    PU_LONG_OSD_GET_REQ,      //消息体: CPuLongOsdGetReq
    PU_LONG_OSD_GET_RSP,      //消息体: CPuLongOsdGetRsp

    //传输台标
    PU_LOGO_BMPOSD_TRANSMIT_REQ,   //消息体: CPuLogoBmpOsdTransmitReq
    PU_LOGO_BMPOSD_TRANSMIT_RSP,   //消息体: CPuLogoBmpOsdTransmitRsp

    //UA路由登记请求
    UA_ROUTER_REG_REQ,        //消息体: CUaRouterRegReq
    UA_ROUTER_REG_RSP,        //消息体: CUaRouterRegRsp

    //UA路由订阅请求
    UA_ROUTER_SS_REQ,         //消息体: CUaRouterSsReq
    UA_ROUTER_SS_RSP,         //消息体: CUaRouterSsRsp

    UA_ROUTER_NTF_REQ,        //消息体: CUaRouterNtfReq
    UA_ROUTER_NTF_RSP,        //消息体: CUaRouterNtfRsp

	//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
	PU_MULTI_OSD_SET_REQ,      //消息体: CPuMultiOsdSetReq
	PU_MULTI_OSD_SET_RSP,      //消息体: CPuMultiOsdSetRsp
	PU_MULTI_OSD_GET_REQ,      //消息体: CPuMultiOsdGetReq
	PU_MULTI_OSD_GET_RSP,      //消息体: CPuMultiOsdGetRsp
	//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕

	//{{{{{{{{{{{{{{{ add by Royan 20140509 智能告警主机
	DOMAIN_ALARM_REQ,          //消息体: CDomainAlarmReq
	DOMAIN_ALARM_RSP,          //消息体: CDomainAlarmRsp
	//}}}}}}}}}}}}}}} add by Royan 20140509 智能告警主机

	//前端抓拍参数设置 //add by ywy 2014.05.12
	PU_SNAPSHOT_CFG_GET_REQ,   //消息体: CPuSnapShotCfgGetReq 
	PU_SNAPSHOT_CFG_GET_RSP,   //消息体: CPuSnapShotCfgGetRsp 
	PU_SNAPSHOT_CFG_SET_REQ,   //消息体: CPuSnapShotCfgSetReq 
	PU_SNAPSHOT_CFG_SET_RSP,   //消息体: CPuSnapShotCfgSetRsp 

	//串口参数配置
	PU_SERIALPARAM_CFG_GET_REQ, //消息体:CPuSerialParamCfgGetReq
	PU_SERIALPARAM_CFG_GET_RSP, //消息体:CPuSerialParamCfgGetRsp
	PU_SERIALPARAM_CFG_SET_REQ, //消息体:CPuSerialParamCfgSetReq
	PU_SERIALPARAM_CFG_SET_RSP, //消息体:CPuSerialParamCfgSetRsp
    
	//同步时间到pu
	PU_TIME_SYNC_REQ,           //消息体：CPuSyncTimeReq
	PU_TIME_SYNC_RSP,           //消息体：CPuSyncTimeRsp

	//porbe onvif 前端
	PROBE_ONVIFPU_REQ,          //消息体CProbeOnvifPuReq
	PROBE_ONVIFPU_RSP,          //消息体CProbeOnvifPuRsp

	UPDATE_REG_NETSEG_REQ,           //消息体: CUpdateRegNetSegReq
	UPDATE_REG_NETSEG_RSP,          //消息体: CUpdateRegNetSegRsp

	UPDATE_CULOGIN_INFO_REQ,           //消息体: CUpdateCuLoginInfoReq
	UPDATE_CULOGIN_INFO_RSP,            //消息体: CUpdateCuLoginInfoRsp

	// 离线统计
	CMU_PU_OFFLINEDEV_QRY_REQ,           //消息体: COffLineDevQryReq
	CMU_PU_OFFLINEDEV_QRY_RSP,           //消息体: COffLineDevQryRsp
	CMU_PU_OFFLINEDEV_QRY_NTF_REQ,       //消息体: COffLineDevQryNtfReq
	CMU_PU_OFFLINEDEV_QRY_NTF_RSP,       //消息体: COffLineDevQryNtfRsp

	RECORD_ON_TV_REQ,      //消息体: CRecordOnTvReq
	RECORD_ON_TV_RSP,      //消息体: CRecordOnTvRsp

	RECORD_OFF_TV_REQ,     //消息体: CRecordOffTvReq
	RECORD_OFF_TV_RSP,     //消息体: CRecordOffTvRsp

	//录像回放上墙时请求解码器接收录像码流
	INVITE_DECORDER_CALL_REQ,    //消息体: CInviteDecoderCallReq
	INVITE_DECORDER_CALL_RSP,    //消息体: CInviteDecoderCallRsp
	INVITE_DECORDER_CALL_ACK,    //消息体: CInviteDecoderCallAck

	//前端视频源参数设置
	PU_VIDEOSRC_SET_REQ,         //消息体：CPuVideosrcSetReq
	PU_VIDEOSRC_SET_RSP,         //消息体：CPuVideosrcSetRsp

	//配置同步开关设置
	CONFIG_SYNC_SWITCH_SET_REQ,     //消息体：CCfgSyncSwitchSetReq
	CONFIG_SYNC_SWITCH_SET_RSP,     //消息体：CCfgSyncSwitchSetRsp

	CONFIG_SYNC_SWITCH_GET_REQ,     //消息体：CCfgSyncSwitchGetReq
	CONFIG_SYNC_SWITCH_GET_RSP,     //消息体：CCfgSyncSwitchGetRsp

	PU_VIDSRCCFG_REFRESH_REQ,       //消息体：CPuVidSrcCfgRefreshReq
	PU_VIDSRCCFG_REFRESH_RSP,       //消息体：CPuVidSrcCfgRefreshRsp
	
	MODULE_ON_OFF_LOG_NTF_REQ,		//消息体：CModuleOnOffLogNtfReq
	MODULE_ON_OFF_LOG_NTF_RSP,		//消息体：CModuleOnOffLogNtfRsp

	ALARM_EVENT_NTF_REQ,			//消息体：CAlarmEventNtfReq
	ALARM_EVENT_NTF_RSP,			//消息体：CAlarmEventNtfRsp

	MAPPING_TABLE_NTF_REQ,			//消息体：CMappingTableNtfReq
	MAPPING_TABLE_NTF_RSP,			//消息体：CMappingTableNtfRsp

	DOMAIN_TABLE_NTF_REQ,			//消息体：CDomainTableNtfReq
	DOMAIN_TABLE_NTF_RSP,			//消息体：CDomainTableNtfRsp

	PU_ONLINE_NTF_REQ,	     		//消息体：CPuOnlineNtfReq
	PU_ONLINE_NTF_RSP,		    	//消息体：CPuOnlineNtfRsp

	PU_VIDSRC_OSD_SET_REQ,          //消息体：CPuVidSrcOsdSetReq
	PU_VIDSRC_OSD_SET_RSP,          //消息体：CPuVidSrcOsdSetRsp
	PU_VIDSRC_OSD_GET_REQ,          //消息体：CPuVidSrcOsdGetReq
	PU_VIDSRC_OSD_GET_RSP,          //消息体：CPuVidSrcOsdGetRsp

    //modified by zzc
    //用于 处理 tvs 上墙类型切换
    //参见 
    //REAL_MEDIA_ON / OFF _TV_REQ
    REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ,//消息体：CRealMediaSwitchOccurTypeReq
    REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP,//消息体：CRealMediaSwitchOccurTypeRsp

	//modified by hrj
	ALARM_TYPE_GET_REQ,              //消息体：CGetAlarmTypesReq
	ALARM_TYPE_GET_RSP,              //消息体：CGetAlarmTypesRsp

	UPDATE_PU_VKEKS_NTF_REQ,        //消息体：CUpdatePuVkeksNtfReq
	UPDATE_PU_VKEKS_NTF_RSP,        //消息体：CUpdatePuVkeksNtfRsp
	PU_SIGNATURE_CTRL_REQ,			//消息体：CPuSignatrueCtrlReq
	PU_SIGNATURE_CTRL_RSP,			//消息体： CPuSignatrueCtrlRsp
	PU_ENCRYPTION_CTRL_REQ,			//消息体： CPuEncryptionCtrlReq
	PU_ENCRYPTION_CTRL_RSP,			//消息体： CPuEncryptionCtrlRsp

    CMS_CURRENT_EVENT_END,
};

/////////////////////////////////////////////////////////////////////
//VTDU接口消息
/////////////////////////////////////////////////////////////////////
enum EVtduMsg
{
    VTDU_REG_REQ = VTDU_EVENT_BEGIN,    //VTDU向CMU注册请求，消息体: CVtduRegReq
    VTDU_REG_RSP,						//CMU向VTDU的注册应答，消息体: CVtduRegRsp

    VTDU_ADDR_ASSIGN_REQ,               //向VTDU分配端口请求，消息体: CVtduAddrAssignReq
    VTDU_ADDR_ASSIGN_RSP,               //向VTDU分配端口应答，消息体: CVtduAddrAssignRsp

    VTDU_ADDSWITCH_REQ,                 //向VTDU添加交换请求，消息体: CVtduAddSwitchReq
    VTDU_ADDSWITCH_RSP,                 //向VTDU添加交换应答，消息体: CVtduAddSwitchRsp

    VTDU_REMOVESWITCH_REQ,              //向VTDU的移除交换请求，消息体: 为CVtduRemoveSwitchReq
    VTDU_REMOVESWITCH_RSP,              //向VTDU的移除交换应答，消息体: CVtduRemoveSwitchRsp

    VTDU_PUNCH_RESULT_NTF_REQ,          //vtdu与源或目的端是否打洞成功，消息体: CVtduPunchResultNtfReq
    VTDU_PUNCH_RESULT_NTF_RSP,          //vtdu与源或目的端是否打洞成功，消息体: CVtduPunchResultNtfRsp

    VTDU_QUERY_MAPPED_ADDR_NTF,         //vtdulib查询外网地址通知，Osp消息：TVtduStunResult
    VTDU_PUNCH_RESULT_NTF,              //vtdulibPunch结果通知，Osp消息：TVtduPunchResult

    //VTDU_FORCE_DISCONNECT_BY_CMU,       //Vtdu退网命令，消息体无

    VTDU_EVENT_NUM                      //VTDU消息个数
};

inline void InitCmsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(PUI_REG_REQ);                             //消息体: CPuiRegReq
    OSP_ADD_EVENT_DESC(PUI_REG_RSP);                             //消息体: CPuiRegRsp
    OSP_ADD_EVENT_DESC(PU_REG_REQ);                              //消息体: CPuRegReq
    OSP_ADD_EVENT_DESC(PU_REG_RSP);                              //消息体: CPuRegRsp
    OSP_ADD_EVENT_DESC(PU_UNREG_REQ);                            //消息体: CPuUnRegReq
    OSP_ADD_EVENT_DESC(PU_UNREG_RSP);                            //消息体: CPuUnRegRsp
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_PU_REQ);                   //消息体: CCmuDisconnectPuReq
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_PU_RSP);                   //消息体: CCmuDisconnectPuRsp
    OSP_ADD_EVENT_DESC(CU_LOGIN_REQ);                            //消息体: CCuLoginReq
    OSP_ADD_EVENT_DESC(CU_LOGIN_RSP);                            //消息体: CCuLoginRsp
    OSP_ADD_EVENT_DESC(CU_LOGOUT_REQ);                           //消息体: CCuLogoutReq
    OSP_ADD_EVENT_DESC(CU_LOGOUT_RSP);                           //消息体: CCuLogoutRsp
    OSP_ADD_EVENT_DESC(CU_DISCONNECT_NTF_REQ);                   //消息体: CCuDisconnectNtfReq
    OSP_ADD_EVENT_DESC(CU_DISCONNECT_NTF_RSP);                   //消息体: CCuDisconnectNtfRsp
    OSP_ADD_EVENT_DESC(CU_GETCUILIST_REQ);                       //消息体: CCuGetCuiListReq
    OSP_ADD_EVENT_DESC(CU_GETCUILIST_RSP);                       //消息体: CCuGetCuiListRsp
    OSP_ADD_EVENT_DESC(CUI_REG_REQ);                             //消息体: CCuiRegReq
    OSP_ADD_EVENT_DESC(CUI_REG_RSP);                             //消息体: CCuiRegRsp
    OSP_ADD_EVENT_DESC(CUI_GET_AZIMUTH_REQ);                     //消息体: CCuiGetAzimuthReq
    OSP_ADD_EVENT_DESC(CUI_GET_AZIMUTH_RSP);                     //消息体: CCuiGetAzimuthRsp
    OSP_ADD_EVENT_DESC(BATCH_OPERATE_REQ);                       // 消息体：CBatchOperateReq
    OSP_ADD_EVENT_DESC(BATCH_OPERATE_RSP);                       // 消息体：CBatchOperateRsp
    OSP_ADD_EVENT_DESC(BATCH_PU_REG_REQ);                        // 消息体：CBatchPuRegReq
    OSP_ADD_EVENT_DESC(BATCH_PU_REG_RSP);                        // 消息体：CBatchPuRegRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_SS_REQ);                   //消息体: CCatalogStatusSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_SS_RSP);                   //消息体: CCatalogStatusSsRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_REFRESH_SS_REQ);           //消息体: CCatalogStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_REFRESH_SS_RSP);           //消息体: CCatalogStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_UNSS_REQ);                 //消息体: CCatalogStatusUnSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_UNSS_RSP);                 //消息体: CCatalogStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_SS_REQ);                    //消息体: CPuListStatusSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_SS_RSP);                    //消息体: CPuListStatusSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_REFRESH_SS_REQ);            //消息体: CPuListStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_REFRESH_SS_RSP);            //消息体: CPuListStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_UNSS_REQ);                  //消息体: CPuListStatusUnSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_UNSS_RSP);                  //消息体: CPuListStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_SS_REQ);                        //消息体: CPuStatusSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_SS_RSP);                        //消息体: CPuStatusSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_REFRESH_SS_REQ);                //消息体: CPuStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_REFRESH_SS_RSP);                //消息体: CPuStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_UNSS_REQ);                      //消息体: CPuStatusUnSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_UNSS_RSP);                      //消息体: CPuStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_NTF_REQ);                       //消息体: CPuStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_STATUS_NTF_RSP);                       //消息体: CPuStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_DEV_CONFIG_NTF_REQ);                   //消息体: CPuDevConfigNtyReq
    OSP_ADD_EVENT_DESC(PU_DEV_CONFIG_NTF_RSP);                   //消息体: CPuDevConfigNtyRsp
    OSP_ADD_EVENT_DESC(PU_CONFIG_NTF_REQ);                       //消息体: CPuConfigNtyReq
    OSP_ADD_EVENT_DESC(PU_CONFIG_NTF_RSP);                       //消息体: CPuConfigNtyRsp
    OSP_ADD_EVENT_DESC(PU_SERVICE_STATUS_NTF_REQ);               //消息体: CPuServiceStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_SERVICE_STATUS_NTF_RSP);               //消息体: CPuServiceStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_STATUS_NTF_REQ);                 //消息体: CPuAlarmStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_ALARM_STATUS_NTF_RSP);                 //消息体: CPuAlarmStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_GPS_STATUS_NTF_REQ);                   //消息体: CPuGpsStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_GPS_STATUS_NTF_RSP);                   //消息体: CPuGpsStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_STATUS_NTF_REQ);             //消息体: CPuTransDataStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_STATUS_NTF_RSP);             //消息体: CPuTransDataStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_BATCH_NTF_REQ);                        //消息体：CPuBatchNtyReq
    OSP_ADD_EVENT_DESC(PU_BATCH_NTF_RSP);                        //消息体：CPuBatchNtyRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_CTRL_REQ);                         //消息体: CPuPtzCtrlReq
    OSP_ADD_EVENT_DESC(PU_PTZ_CTRL_RSP);                         //消息体: CPuPtzCtrlRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_LOCK_REQ);                         //消息体: CPuPtzLockReq
    OSP_ADD_EVENT_DESC(PU_PTZ_LOCK_RSP);                         //消息体: CPuPtzLockRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_UNLOCK_REQ);                       //消息体: CPuPtzUnlockReq
    OSP_ADD_EVENT_DESC(PU_PTZ_UNLOCK_RSP);                       //消息体: CPuPtzUnlockRsp
    OSP_ADD_EVENT_DESC(PU_SEND_TRANSPARENT_DATA_REQ);            //消息体: CPuSendTransparentDataReq
    OSP_ADD_EVENT_DESC(PU_SEND_TRANSPARENT_DATA_RSP);            //消息体: CPuSendTransparentDataRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_REQ);                //消息体: CPuVidEncParamSetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_RSP);                //消息体: CPuVidEncParamSetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_REQ);                //消息体: CPuVidEncParamGetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_RSP);                //消息体: CPuVidEncParamGetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_EX_REQ);             //消息体: CPuVidEncParamExSetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_EX_RSP);             //消息体: CPuVidEncParamExSetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_EX_REQ);             //消息体: CPuVidEncParamGetExReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_EX_RSP);             //消息体: CPuVidEncParamGetExRsp
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_SET_REQ);                //消息体: CPuAudEncParamSetReq
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_SET_RSP);                //消息体: CPuAudEncParamSetRsp
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_GET_REQ);                //消息体: CPuAudEncParamGetReq
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_GET_RSP);                //消息体: CPuAudEncParamGetRsp
    OSP_ADD_EVENT_DESC(PU_VSIP_OPERATE_REQ);                     //消息体: CPuVsipOperateReq
    OSP_ADD_EVENT_DESC(PU_VSIP_OPERATE_RSP);                     //消息体: CPuVsipOperateRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_OPERATE_REQ);              //消息体: CPuTransparentOperateReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_OPERATE_RSP);              //消息体: CPuTransparentOperateRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_REQ);                  //消息体: CPuTransparentQryReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_RSP);                  //消息体: CPuTransparentQryRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_NTF_REQ);              //消息体: CPuTransparentQryNtfReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_NTF_RSP);              //消息体: CPuTransparentQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_DATA_NTF_REQ);             //消息体: CPuTransparentDataNtfReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_DATA_NTF_RSP);             //消息体: CPuTransparentDataNtfRsp
    OSP_ADD_EVENT_DESC(PU_SAVE_CFG_REQ);                         //消息体: CPuSaveCfgReq
    OSP_ADD_EVENT_DESC(PU_SAVE_CFG_RSP);                         //消息体: CPuSaveCfgRsp
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_SET_REQ);                    //消息体: CPuSysParamSetReq
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_SET_RSP);                    //消息体: CPuSysParamSetRsp
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_GET_REQ);                    //消息体: CPuSysParamGetReq
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_GET_RSP);                    //消息体: CPuSysParamGetRsp
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_REQ);                  //消息体: CPuSendRealMediaReq
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_RSP);                  //消息体: CPuSendRealMediaRsp
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_ACK);                  //消息体: CInviteThirdPartyCallAck
    OSP_ADD_EVENT_DESC(PU_STOP_SEND_REAL_MEDIA_REQ);             //消息体: CPuStopSendRealMediaReq
    OSP_ADD_EVENT_DESC(PU_STOP_SEND_REAL_MEDIA_RSP);             //消息体: CPuStopSendRealMediaRsp
    OSP_ADD_EVENT_DESC(PU_REBOOT_REQ);                           //消息体: CPuRebootReq
    OSP_ADD_EVENT_DESC(PU_REBOOT_RSP);                           //消息体: CPuRebootRsp
    OSP_ADD_EVENT_DESC(INVITE_REQ);                              //消息体: CInviteReq
    OSP_ADD_EVENT_DESC(INVITE_RSP);                              //消息体: CInviteRsp
    OSP_ADD_EVENT_DESC(INVITE_ACK);                              //消息体: CInviteACK
    OSP_ADD_EVENT_DESC(BYE_REQ);                                 //消息体: CByeReq
    OSP_ADD_EVENT_DESC(BYE_RSP);                                 //消息体: CByeRsp
    OSP_ADD_EVENT_DESC(START_THIRD_PARTY_CALL_REQ);              //消息体: CStartThirdPartyCallReq
    OSP_ADD_EVENT_DESC(START_THIRD_PARTY_CALL_RSP);              //消息体: CStartThirdPartyCallRsp ,不带媒体信息
    OSP_ADD_EVENT_DESC(STOP_THIRD_PARTY_CALL_REQ);               //消息体: CStopThirdPartyCallReq
    OSP_ADD_EVENT_DESC(STOP_THIRD_PARTY_CALL_RSP);               //消息体: CStopThirdPartyCallRsp
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_REQ);             //消息体: CInviteThirdPartyCallReq
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_RSP);             //消息体: CInviteThirdPartyCallRsp
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_ACK);             //消息体: CInviteThirdPartyCallAck
    OSP_ADD_EVENT_DESC(UPDATE_SWITCH_ADDRESS_REQ);               //消息体：CUpdateSwitchAddressReq
    OSP_ADD_EVENT_DESC(UPDATE_SWITCH_ADDRESS_RSP);               //消息体：CUpdateSwitchAddressRsp
    OSP_ADD_EVENT_DESC(KEY_FRAME_REQ);                           //消息体: CKeyFrameReq
    OSP_ADD_EVENT_DESC(KEY_FRAME_RSP);                           //消息体: CKeyFrameRsp
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_QUERY_REQ);             //消息体: CPlaybackProgressQueryReq
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_QUERY_RSP);             //消息体: CPlaybackProgressQueryRsp
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_NTF_REQ);               //消息体: CPlaybackProgressNtfReq
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_NTF_RSP);               //消息体: CPlaybackProgressNtfRsp
    OSP_ADD_EVENT_DESC(END_OF_FILE_NTF_REQ);                     //消息体: CEndOfFileNtfReq
    OSP_ADD_EVENT_DESC(END_OF_FILE_NTF_RSP);                     //消息体: CEndOfFileNtfRsp
    OSP_ADD_EVENT_DESC(VCR_CTRL_REQ);                            //消息体: CVcrCtrlReq
    OSP_ADD_EVENT_DESC(VCR_CTRL_RSP);                            //消息体: CVcrCtrlRsp
    OSP_ADD_EVENT_DESC(CMU_CONNECT_REQ);                         //消息体: CCmuConnectReq
    OSP_ADD_EVENT_DESC(CMU_CONNECT_RSP);                         //消息体: CCmuConnectRsp
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_NTF_REQ);                  //消息体: CCmuDisconnectNtfReq
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_NTF_RSP);                  //消息体: CCmuDisconnectNtfRsp
    OSP_ADD_EVENT_DESC(DEV_LOGIN_REQ);                           //消息体: CDevLoginReq
    OSP_ADD_EVENT_DESC(DEV_LOGIN_RSP);                           //消息体: CDevLoginRsp
    OSP_ADD_EVENT_DESC(DEV_LOGOUT_REQ);                          //消息体: CDevLogoutReq
    OSP_ADD_EVENT_DESC(DEV_LOGOUT_RSP);                          //消息体: CDevLogoutRsp
    OSP_ADD_EVENT_DESC(NETADDR_QUERY_REQ);                       //消息体: CNetAddrQueryReq
    OSP_ADD_EVENT_DESC(NETADDR_QUERY_RSP);                       //消息体: CNetAddrQueryRsp
    OSP_ADD_EVENT_DESC(FILE_EXPORT_REQ);                         //消息体: CFileExportReq
    OSP_ADD_EVENT_DESC(FILE_EXPORT_RSP);                         //消息体: CFileExportRsp
    OSP_ADD_EVENT_DESC(FILE_EXPORT_ACK);                         //消息体: CFileExportAck
    OSP_ADD_EVENT_DESC(FILE_COLLECT_PROGRESS_NTF_REQ);           //消息体: CFileCollectProgressNtfReq
    OSP_ADD_EVENT_DESC(FILE_COLLECT_PROGRESS_NTF_RSP);           //消息体: CFileCollectProgressNtfRsp
    OSP_ADD_EVENT_DESC(FILE_IMPORT_REQ);                         //消息体: CFileImportReq
    OSP_ADD_EVENT_DESC(FILE_IMPORT_RSP);                         //消息体: CFileImportRsp
    OSP_ADD_EVENT_DESC(FILE_IMPORT_ACK);                         //消息体: CFileImportAck
    OSP_ADD_EVENT_DESC(FILE_TRANS_END_REQ);                      //消息体: CFileTransEndReq
    OSP_ADD_EVENT_DESC(FILE_TRANS_END_RSP);                      //消息体: CFileTransEndRsp
    OSP_ADD_EVENT_DESC(STUN_UPDATE_REQ);                         //消息体: CStunUpdateReq
    OSP_ADD_EVENT_DESC(STUN_UPDATE_RSP);                         //消息体: CStunUpdateRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_INPUT_NTF_REQ);                  //消息体: CPuAlarmInputNtyReq
    OSP_ADD_EVENT_DESC(PU_ALARM_INPUT_NTF_RSP);                  //消息体: CPuAlarmInputNtyRsp
    OSP_ADD_EVENT_DESC(PU_READ_SEC_REQ);                         //消息体: CPuReadSecReq
    OSP_ADD_EVENT_DESC(PU_READ_SEC_RSP);                         //消息体: CPuReadSecRsp
    OSP_ADD_EVENT_DESC(PU_IO_OUT_REQ);                           //消息体: CPuIoOutReq
    OSP_ADD_EVENT_DESC(PU_IO_OUT_RSP);                           //消息体: CPuIoOutRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_RESET_REQ);                      //消息体：CPuAlarmResetReq
    OSP_ADD_EVENT_DESC(PU_ALARM_RESET_RSP);                      //消息体：CPuAlarmResetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_SET_REQ);                   //消息体: CAlarmLinkCfgSetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_SET_RSP);                   //消息体: CAlarmLinkCfgSetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_GET_REQ);                   //消息体: CAlarmLinkCfgGetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_GET_RSP);                   //消息体: CAlarmLinkCfgGetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_VERSION_GET_REQ);               //消息体：CAlarmLinkVersionGetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_VERSION_GET_RSP);               //消息体：CAlarmLinkVersionGetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_QUERY_REQ);                     //消息体: CAlarmLinkQueryReq
    OSP_ADD_EVENT_DESC(ALARMLINK_QUERY_RSP);                     //消息体: CAlarmLinkQueryRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_NTF_REQ);                       //消息体: CAlarmLinkNtfReq
    OSP_ADD_EVENT_DESC(ALARMLINK_NTF_RSP);                       //消息体: CAlarmLinkNtfRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_MOD_NTF_REQ);                   //消息体: CAlarmLinkModNtfReq
    OSP_ADD_EVENT_DESC(ALARMLINK_MOD_NTF_RSP);                   //消息体: CAlarmLinkModNtfRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_ON_TV_REQ);                    //消息体: CRealMediaOnTvReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_ON_TV_RSP);                    //消息体: CRealMediaOnTvRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_OFF_TV_REQ);                   //消息体: CRealMediaOffTvReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_OFF_TV_RSP);                   //消息体: CRealMediaOffTvRsp
    OSP_ADD_EVENT_DESC(REC_ON_TV_REQ);                           //消息体: CRecOnTvReq
    OSP_ADD_EVENT_DESC(REC_ON_TV_RSP);                           //消息体: CRecOnTvRsp
    OSP_ADD_EVENT_DESC(REC_ON_TV_ACK);                           //消息体: CRecOnTvAck
    OSP_ADD_EVENT_DESC(TV_POLL_OPERATE_REQ);                     //消息体: CTvPollOperateReq
    OSP_ADD_EVENT_DESC(TV_POLL_OPERATE_RSP);                     //消息体: CTvPollOperateRsp
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_SET_REQ);                    //消息体: CTvWinStyleSetReq
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_SET_RSP);                    //消息体: CTvWinStyleSetRsp
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_GET_REQ);                    //消息体: CTvWinStyleGetReq
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_GET_RSP);                    //消息体: CTvWinStyleGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_SAVE_REQ);                  //消息体: CTvWallSchemeSaveReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_SAVE_RSP);                  //消息体: CTvWallSchemeSaveRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_DEL_REQ);                   //消息体: CTvWallSchemeDelReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_DEL_RSP);                   //消息体: CTvWallSchemeDelRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_QUERY_REQ);                 //消息体: CTvWallSchemeQueryReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_QUERY_RSP);                 //消息体: CTvWallSchemeQueryRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_NTF_REQ);                   //消息体: CTvWallSchemeNtfReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_NTF_RSP);                   //消息体: CTvWallSchemeNtfRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOAD_REQ);                  //消息体: CTvWallSchemeLoadReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOAD_RSP);                  //消息体: CTvWallSchemeLoadRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_SET_REQ);             //消息体: CTvWallSchemeGroupSetReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_SET_RSP);             //消息体: CTvWallSchemeGroupSetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_GET_REQ);             //消息体: CTvWallSchemeGroupGetReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_GET_RSP);             //消息体: CTvWallSchemeGroupGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_POLL_OPERATE_REQ);          //消息体: CTvWallSchemePollOperateReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_POLL_OPERATE_RSP);          //消息体: CTvWallSchemePollOperateRsp
    OSP_ADD_EVENT_DESC(PLAT_REC_ON_TV_REQ);                      //消息体: CPlatRecOnTvReq
    OSP_ADD_EVENT_DESC(PLAT_REC_ON_TV_RSP);                      //消息体: CPlatRecOnTvRsp
    OSP_ADD_EVENT_DESC(PLAT_REC_OFF_TV_REQ);                     //消息体: CPlatRecOffTvReq
    OSP_ADD_EVENT_DESC(PLAT_REC_OFF_TV_RSP);                     //消息体: CPlatRecOffTvRsp
    OSP_ADD_EVENT_DESC(PU_REC_ON_TV_REQ);                        //消息体: CPuRecOnTvReq
    OSP_ADD_EVENT_DESC(PU_REC_ON_TV_RSP);                        //消息体: CPuRecOnTvRsp
    OSP_ADD_EVENT_DESC(PU_REC_OFF_TV_REQ);                       //消息体: CPuRecOffTvReq  (暂不实现)
    OSP_ADD_EVENT_DESC(PU_REC_OFF_TV_RSP);                       //消息体: CPuRecOffTvRsp  (暂不实现)
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_SS_REQ);                    //消息体: CTvWallStatusSsReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_SS_RSP);                    //消息体: CTvWallStatusSsRsp
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_UNSS_REQ);                  //消息体: CTvWallStatusUnSsReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_UNSS_RSP);                  //消息体: CTvWallStatusUnSsRsp
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_NTF_REQ);                   //消息体: CTvWallStatusNtyReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_NTF_RSP);                   //消息体: CTvWallStatusNtyRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_LOCK_REQ);                     //消息体: CTvChannelLockReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_LOCK_RSP);                     //消息体: CTvChannelLockRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_UNLOCK_REQ);                   //消息体: CTvChannelUnlockReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_UNLOCK_RSP);                   //消息体: CTvChannelUnlockRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_SAVE_REQ);         //消息体: CTvChannelWatchPointSaveReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_SAVE_RSP);         //消息体: CTvChannelWatchPointSaveRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_LOAD_REQ);         //消息体: CTvChannelWatchPointLoadReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_LOAD_RSP);         //消息体: CTvChannelWatchPointLoadRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_INFO_GET_REQ);           //消息体: CTvChannelWatchInfoGetReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_INFO_GET_RSP);           //消息体: CTvChannelWatchInfoGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOCK_REQ);                  //消息体: CTvWallSchemeLockReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOCK_RSP);                  //消息体: CTvWallSchemeLockRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_UNLOCK_REQ);                //消息体: CTvWallSchemeUnlockReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_UNLOCK_RSP);                //消息体: CTvWallSchemeUnlockRsp
    OSP_ADD_EVENT_DESC(PU_CAMERA_TYPE_SET_REQ);                  //消息体: CPuCameraTypeSetReq
    OSP_ADD_EVENT_DESC(PU_CAMERA_TYPE_SET_RSP);                  //消息体: CPuCameraTypeSetRsp
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_SET_REQ);                //消息体: CPuMotionDetectSetReq
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_SET_RSP);                //消息体: CPuMotionDetectSetRsp
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_GET_REQ);                //消息体: CPuMotionDetectGetReq
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_GET_RSP);                //消息体: CPuMotionDetectGetRsp
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_SET_REQ);                   //消息体: CPuAreaShadeSetReq
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_SET_RSP);                   //消息体: CPuAreaShadeSetRsp
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_GET_REQ);                   //消息体: CPuAreaShadeGetReq
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_GET_RSP);                   //消息体: CPuAreaShadeGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_SET_REQ);                      //消息体: CPuGpsCfgSetReq
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_SET_RSP);                      //消息体: CPuGpsCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_GET_REQ);                      //消息体: CPuGpsCfgGetReq
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_GET_RSP);                      //消息体: CPuGpsCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_SS_SET_REQ);                       //消息体: CPuGpsSsSetReq
    OSP_ADD_EVENT_DESC(PU_GPS_SS_SET_RSP);                       //消息体: CPuGpsSsSetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_SS_GET_REQ);                       //消息体: CPuGpsSsGetReq
    OSP_ADD_EVENT_DESC(PU_GPS_SS_GET_RSP);                       //消息体: CPuGpsSsGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_DATA_NTF_REQ);                     //消息体: CPuGpsDataNtyReq
    OSP_ADD_EVENT_DESC(PU_GPS_DATA_NTF_RSP);                     //消息体: CPuGpsDataNtyRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_SET_REQ);                //消息体: CPuTransdataCfgSetReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_SET_RSP);                //消息体: CPuTransdataCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_GET_REQ);                //消息体: CPuTransdataCfgGetReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_GET_RSP);                //消息体: CPuTransdataCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_START_REQ);                     //消息体: CPuRecordStartReq
    OSP_ADD_EVENT_DESC(PU_RECORD_START_RSP);                     //消息体: CPuRecordStartRsp
    OSP_ADD_EVENT_DESC(PU_RECORED_STATUS_NTF_REQ);               //消息体: CPuRecordStatusNtfReq
    OSP_ADD_EVENT_DESC(PU_RECORED_STATUS_NTF_RSP);               //消息体: CPuRecordStatusNtfRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_STOP_REQ);                      //消息体: CPuRecordStopReq
    OSP_ADD_EVENT_DESC(PU_RECORD_STOP_RSP);                      //消息体: CPuRecordStopRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_DEL_REQ);                       //消息体: CPuRecordDelReq
    OSP_ADD_EVENT_DESC(PU_RECORD_DEL_RSP);                       //消息体: CPuRecordDelRsp
    OSP_ADD_EVENT_DESC(PU_REC_QRY_REQ);                          //消息体: CPuRecQryReq
    OSP_ADD_EVENT_DESC(PU_REC_QRY_RSP);                          //消息体: CPuRecQryRsp
    OSP_ADD_EVENT_DESC(PU_REC_QRY_NTF_REQ);                      //消息体: CPuRecQryNtfReq
    OSP_ADD_EVENT_DESC(PU_REC_QRY_NTF_RSP);                      //消息体: CPuRecQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_REC_DAYS_QRY_REQ);                     //消息体: CPuRecDaysQryReq
    OSP_ADD_EVENT_DESC(PU_REC_DAYS_QRY_RSP);                     //消息体: CPuRecDaysQryRsp
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_SET_REQ);                    //消息体: CPuDecStyleSetReq
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_SET_RSP);                    //消息体: CPuDecStyleSetRsp
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_GET_REQ);                    //消息体: CPuDecStyleGetReq
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_GET_RSP);                    //消息体: CPuDecStyleGetRsp
    OSP_ADD_EVENT_DESC(PU_PIC_SNAPSHOT_REQ);                     //消息体: CPuPicSnapshotReq
    OSP_ADD_EVENT_DESC(PU_PIC_SNAPSHOT_RSP);                     //消息体: CPuPicSnapshotRsp
    OSP_ADD_EVENT_DESC(PU_PIC_DOWNLOAD_REQ);                     //消息体: CPuPicDownloadReq
    OSP_ADD_EVENT_DESC(PU_PIC_DOWNLOAD_RSP);                     //消息体: CPuPicDownloadRsp
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_REQ);                          //消息体: CPuPicQryReq
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_RSP);                          //消息体: CPuPicQryRsp
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_NTF_REQ);                      //消息体: CPuPicQryNtfReq
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_NTF_RSP);                      //消息体: CPuPicQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_SET_REQ);                     //消息体: CPuLongOsdSetReq
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_SET_RSP);                     //消息体: CPuLongOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_GET_REQ);                     //消息体: CPuLongOsdGetReq
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_GET_RSP);                     //消息体: CPuLongOsdGetRsp
    OSP_ADD_EVENT_DESC(PU_LOGO_BMPOSD_TRANSMIT_REQ);             //消息体: CPuLogoBmpOsdTransmitReq
    OSP_ADD_EVENT_DESC(PU_LOGO_BMPOSD_TRANSMIT_RSP);             //消息体: CPuLogoBmpOsdTransmitRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_REQ);                       //消息体: CUaRouterRegReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_RSP);                       //消息体: CUaRouterRegRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_REQ);                        //消息体: CUaRouterSsReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_RSP);                        //消息体: CUaRouterSsRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_REQ);                       //消息体: CUaRouterNtfReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_RSP);                       //消息体: CUaRouterNtfRsp
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_SET_REQ);                    //消息体: CPuMultiOsdSetReq
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_SET_RSP);                    //消息体: CPuMultiOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_GET_REQ);                    //消息体: CPuMultiOsdGetReq
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_GET_RSP);                    //消息体: CPuMultiOsdGetRsp
    OSP_ADD_EVENT_DESC(DOMAIN_ALARM_REQ);                        //消息体: CDomainAlarmReq
    OSP_ADD_EVENT_DESC(DOMAIN_ALARM_RSP);                        //消息体: CDomainAlarmRsp
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_GET_REQ);                 //消息体: CPuSnapShotCfgGetReq 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_GET_RSP);                 //消息体: CPuSnapShotCfgGetRsp 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_SET_REQ);                 //消息体: CPuSnapShotCfgSetReq 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_SET_RSP);                 //消息体: CPuSnapShotCfgSetRsp 
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_GET_REQ);              //消息体:CPuSerialParamCfgGetReq
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_GET_RSP);              //消息体:CPuSerialParamCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_SET_REQ);              //消息体:CPuSerialParamCfgSetReq
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_SET_RSP);              //消息体:CPuSerialParamCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_TIME_SYNC_REQ);                        //消息体：CPuSyncTimeReq
    OSP_ADD_EVENT_DESC(PU_TIME_SYNC_RSP);                        //消息体：CPuSyncTimeRsp
    OSP_ADD_EVENT_DESC(PROBE_ONVIFPU_REQ);                       //消息体CProbeOnvifPuReq
    OSP_ADD_EVENT_DESC(PROBE_ONVIFPU_RSP);                       //消息体CProbeOnvifPuRsp
    OSP_ADD_EVENT_DESC(UPDATE_REG_NETSEG_REQ);                   //消息体: CUpdateRegNetSegReq
    OSP_ADD_EVENT_DESC(UPDATE_REG_NETSEG_RSP);                   //消息体: CUpdateRegNetSegRsp
    OSP_ADD_EVENT_DESC(UPDATE_CULOGIN_INFO_REQ);                 //消息体: CUpdateCuLoginInfoReq
    OSP_ADD_EVENT_DESC(UPDATE_CULOGIN_INFO_RSP);                 //消息体: CUpdateCuLoginInfoRsp
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_REQ);               //消息体: COffLineDevQryReq
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_RSP);               //消息体: COffLineDevQryRsp
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_NTF_REQ);           //消息体: COffLineDevQryNtfReq
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_NTF_RSP);           //消息体: COffLineDevQryNtfRsp
    OSP_ADD_EVENT_DESC(RECORD_ON_TV_REQ);                        //消息体: CRecordOnTvReq
    OSP_ADD_EVENT_DESC(RECORD_ON_TV_RSP);                        //消息体: CRecordOnTvRsp
    OSP_ADD_EVENT_DESC(RECORD_OFF_TV_REQ);                       //消息体: CRecordOffTvReq
    OSP_ADD_EVENT_DESC(RECORD_OFF_TV_RSP);                       //消息体: CRecordOffTvRsp
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_REQ);                //消息体: CInviteDecoderCallReq
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_RSP);                //消息体: CInviteDecoderCallRsp
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_ACK);                //消息体: CInviteDecoderCallAck
    OSP_ADD_EVENT_DESC(PU_VIDEOSRC_SET_REQ);                     //消息体：CPuVideosrcSetReq
    OSP_ADD_EVENT_DESC(PU_VIDEOSRC_SET_RSP);                     //消息体：CPuVideosrcSetRsp
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_SET_REQ);              //消息体：CCfgSyncSwitchSetReq
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_SET_RSP);              //消息体：CCfgSyncSwitchSetRsp
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_GET_REQ);              //消息体：CCfgSyncSwitchGetReq
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_GET_RSP);              //消息体：CCfgSyncSwitchGetRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRCCFG_REFRESH_REQ);                //消息体：CPuVidSrcCfgRefreshReq
    OSP_ADD_EVENT_DESC(PU_VIDSRCCFG_REFRESH_RSP);                //消息体：CPuVidSrcCfgRefreshRsp
    OSP_ADD_EVENT_DESC(MODULE_ON_OFF_LOG_NTF_REQ);               //消息体：CModuleOnOffLogNtfReq
    OSP_ADD_EVENT_DESC(MODULE_ON_OFF_LOG_NTF_RSP);               //消息体：CModuleOnOffLogNtfRsp
    OSP_ADD_EVENT_DESC(ALARM_EVENT_NTF_REQ);                     //消息体：CAlarmEventNtfReq
    OSP_ADD_EVENT_DESC(ALARM_EVENT_NTF_RSP);                     //消息体：CAlarmEventNtfRsp
    OSP_ADD_EVENT_DESC(MAPPING_TABLE_NTF_REQ);                   //消息体：CMappingTableNtfReq
    OSP_ADD_EVENT_DESC(MAPPING_TABLE_NTF_RSP);                   //消息体：CMappingTableNtfRsp
    OSP_ADD_EVENT_DESC(DOMAIN_TABLE_NTF_REQ);                    //消息体：CDomainTableNtfReq
    OSP_ADD_EVENT_DESC(DOMAIN_TABLE_NTF_RSP);                    //消息体：CDomainTableNtfRsp
    OSP_ADD_EVENT_DESC(PU_ONLINE_NTF_REQ);                       //消息体：CPuOnlineNtfReq
    OSP_ADD_EVENT_DESC(PU_ONLINE_NTF_RSP);                       //消息体：CPuOnlineNtfRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_SET_REQ);                   //消息体：CPuVidSrcOsdSetReq
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_SET_RSP);                   //消息体：CPuVidSrcOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_GET_REQ);                   //消息体：CPuVidSrcOsdGetReq
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_GET_RSP);                   //消息体：CPuVidSrcOsdGetRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ);        //消息体：CRealMediaSwitchOccurTypeReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP);        //消息体：CRealMediaSwitchOccurTypeRsp
    OSP_ADD_EVENT_DESC(ALARM_TYPE_GET_REQ);                      //消息体：CGetAlarmTypesReq
    OSP_ADD_EVENT_DESC(ALARM_TYPE_GET_RSP);                      //消息体：CGetAlarmTypesRsp
    OSP_ADD_EVENT_DESC(UPDATE_PU_VKEKS_NTF_REQ);                 //消息体：CUpdatePuVkeksNtfReq
    OSP_ADD_EVENT_DESC(UPDATE_PU_VKEKS_NTF_RSP);                 //消息体：CUpdatePuVkeksNtfRsp
    OSP_ADD_EVENT_DESC(PU_SIGNATURE_CTRL_REQ);                   //消息体：CPuSignatrueCtrlReq
    OSP_ADD_EVENT_DESC(PU_SIGNATURE_CTRL_RSP);                   //消息体： CPuSignatrueCtrlRsp
    OSP_ADD_EVENT_DESC(PU_ENCRYPTION_CTRL_REQ);                  //消息体： CPuEncryptionCtrlReq
    OSP_ADD_EVENT_DESC(PU_ENCRYPTION_CTRL_RSP);                  //消息体： CPuEncryptionCtrlRsp
    OSP_ADD_EVENT_DESC(VTDU_REG_REQ);                            //VTDU向CMU注册请求，消息体: CVtduRegReq
    OSP_ADD_EVENT_DESC(VTDU_REG_RSP);                            //CMU向VTDU的注册应答，消息体: CVtduRegRsp
    OSP_ADD_EVENT_DESC(VTDU_ADDR_ASSIGN_REQ);                    //向VTDU分配端口请求，消息体: CVtduAddrAssignReq
    OSP_ADD_EVENT_DESC(VTDU_ADDR_ASSIGN_RSP);                    //向VTDU分配端口应答，消息体: CVtduAddrAssignRsp
    OSP_ADD_EVENT_DESC(VTDU_ADDSWITCH_REQ);                      //向VTDU添加交换请求，消息体: CVtduAddSwitchReq
    OSP_ADD_EVENT_DESC(VTDU_ADDSWITCH_RSP);                      //向VTDU添加交换应答，消息体: CVtduAddSwitchRsp
    OSP_ADD_EVENT_DESC(VTDU_REMOVESWITCH_REQ);                   //向VTDU的移除交换请求，消息体: 为CVtduRemoveSwitchReq
    OSP_ADD_EVENT_DESC(VTDU_REMOVESWITCH_RSP);                   //向VTDU的移除交换应答，消息体: CVtduRemoveSwitchRsp
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF_REQ);               //vtdu与源或目的端是否打洞成功，消息体: CVtduPunchResultNtfReq
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF_RSP);               //vtdu与源或目的端是否打洞成功，消息体: CVtduPunchResultNtfRsp
    OSP_ADD_EVENT_DESC(VTDU_QUERY_MAPPED_ADDR_NTF);              //vtdulib查询外网地址通知，Osp消息：TVtduStunResult
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF);                   //vtdulibPunch结果通知，Osp消息：TVtduPunchResult
}


#endif  //#ifndef __CMU_EVENT_H__




