/*****************************************************************************
   ģ����      : cmu_event
   �ļ���      : cmu_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cmu_event.h ����������3AC_3AS����ϢID
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/22   1.0         liangli        ����
******************************************************************************/
#ifndef __CMU_EVENT_H__
#define __CMU_EVENT_H__

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"

enum ECMS_EVENT
{
    PUI_REG_REQ = CMS_EVENT_BEGIN,             //��Ϣ��: CPuiRegReq
    PUI_REG_RSP,             //��Ϣ��: CPuiRegRsp

    PU_REG_REQ,              //��Ϣ��: CPuRegReq
    PU_REG_RSP,              //��Ϣ��: CPuRegRsp

    PU_UNREG_REQ,            //��Ϣ��: CPuUnRegReq
    PU_UNREG_RSP,            //��Ϣ��: CPuUnRegRsp

    CMU_DISCONNECT_PU_REQ,   //��Ϣ��: CCmuDisconnectPuReq
    CMU_DISCONNECT_PU_RSP,   //��Ϣ��: CCmuDisconnectPuRsp

    CU_LOGIN_REQ,            //��Ϣ��: CCuLoginReq
    CU_LOGIN_RSP,            //��Ϣ��: CCuLoginRsp

    CU_LOGOUT_REQ,           //��Ϣ��: CCuLogoutReq
    CU_LOGOUT_RSP,           //��Ϣ��: CCuLogoutRsp

    CU_DISCONNECT_NTF_REQ,   //��Ϣ��: CCuDisconnectNtfReq
    CU_DISCONNECT_NTF_RSP,   //��Ϣ��: CCuDisconnectNtfRsp

    CU_GETCUILIST_REQ,       //��Ϣ��: CCuGetCuiListReq
    CU_GETCUILIST_RSP,       //��Ϣ��: CCuGetCuiListRsp

    CUI_REG_REQ,             //��Ϣ��: CCuiRegReq
    CUI_REG_RSP,             //��Ϣ��: CCuiRegRsp

	CUI_GET_AZIMUTH_REQ,     //��Ϣ��: CCuiGetAzimuthReq
	CUI_GET_AZIMUTH_RSP,     //��Ϣ��: CCuiGetAzimuthRsp

    BATCH_OPERATE_REQ,      // ��Ϣ�壺CBatchOperateReq
    BATCH_OPERATE_RSP,      // ��Ϣ�壺CBatchOperateRsp

    BATCH_PU_REG_REQ,      // ��Ϣ�壺CBatchPuRegReq
    BATCH_PU_REG_RSP,      // ��Ϣ�壺CBatchPuRegRsp

    //���嶩��(Ŀ¼����)
    CATALOG_STATUS_SS_REQ,        //��Ϣ��: CCatalogStatusSsReq
    CATALOG_STATUS_SS_RSP,        //��Ϣ��: CCatalogStatusSsRsp

    CATALOG_STATUS_REFRESH_SS_REQ,        //��Ϣ��: CCatalogStatusRefreshSsReq
    CATALOG_STATUS_REFRESH_SS_RSP,        //��Ϣ��: CCatalogStatusRefreshSsRsp

    CATALOG_STATUS_UNSS_REQ,        //��Ϣ��: CCatalogStatusUnSsReq
    CATALOG_STATUS_UNSS_RSP,        //��Ϣ��: CCatalogStatusUnSsRsp

    //���嶩��(�б���)
    PULIST_STATUS_SS_REQ,        //��Ϣ��: CPuListStatusSsReq
    PULIST_STATUS_SS_RSP,        //��Ϣ��: CPuListStatusSsRsp

    PULIST_STATUS_REFRESH_SS_REQ,        //��Ϣ��: CPuListStatusRefreshSsReq
    PULIST_STATUS_REFRESH_SS_RSP,        //��Ϣ��: CPuListStatusRefreshSsRsp

    PULIST_STATUS_UNSS_REQ,        //��Ϣ��: CPuListStatusUnSsReq
    PULIST_STATUS_UNSS_RSP,        //��Ϣ��: CPuListStatusUnSsRsp

	/*
     * ���㶩�ļ�����������ֹͣʹ�á�xiongxh��
     */
    //�����豸����
    PU_STATUS_SS_REQ,        //��Ϣ��: CPuStatusSsReq
    PU_STATUS_SS_RSP,        //��Ϣ��: CPuStatusSsRsp

    PU_STATUS_REFRESH_SS_REQ,        //��Ϣ��: CPuStatusRefreshSsReq
    PU_STATUS_REFRESH_SS_RSP,        //��Ϣ��: CPuStatusRefreshSsRsp

    PU_STATUS_UNSS_REQ,        //��Ϣ��: CPuStatusUnSsReq
    PU_STATUS_UNSS_RSP,        //��Ϣ��: CPuStatusUnSsRsp

    PU_STATUS_NTF_REQ,       //��Ϣ��: CPuStatusNtyReq
    PU_STATUS_NTF_RSP,       //��Ϣ��: CPuStatusNtyRsp

    //puiͨ��MESSAGE������cmu�ϱ�ǰ�����ã�cmu�������Ϣ
    PU_DEV_CONFIG_NTF_REQ,       //��Ϣ��: CPuDevConfigNtyReq
    PU_DEV_CONFIG_NTF_RSP,       //��Ϣ��: CPuDevConfigNtyRsp

    //cmuͨ��NOTIFY���������߷���ǰ�����ã������ߴ������Ϣ
    PU_CONFIG_NTF_REQ,       //��Ϣ��: CPuConfigNtyReq
    PU_CONFIG_NTF_RSP,       //��Ϣ��: CPuConfigNtyRsp

    PU_SERVICE_STATUS_NTF_REQ,       //��Ϣ��: CPuServiceStatusNtyReq
    PU_SERVICE_STATUS_NTF_RSP,       //��Ϣ��: CPuServiceStatusNtyRsp

    PU_ALARM_STATUS_NTF_REQ,       //��Ϣ��: CPuAlarmStatusNtyReq
    PU_ALARM_STATUS_NTF_RSP,       //��Ϣ��: CPuAlarmStatusNtyRsp

    PU_GPS_STATUS_NTF_REQ,       //��Ϣ��: CPuGpsStatusNtyReq
    PU_GPS_STATUS_NTF_RSP,       //��Ϣ��: CPuGpsStatusNtyRsp

    //�ýӿ�����cmu�����߷���͸������״̬(ʹ�õ���SUBSCRIBE+NOTFY����)
    PU_TRANSDATA_STATUS_NTF_REQ,      //��Ϣ��: CPuTransDataStatusNtyReq
    PU_TRANSDATA_STATUS_NTF_RSP,      //��Ϣ��: CPuTransDataStatusNtyRsp

	// ����֪ͨ
	PU_BATCH_NTF_REQ,			//��Ϣ�壺CPuBatchNtyReq
	PU_BATCH_NTF_RSP,			//��Ϣ�壺CPuBatchNtyRsp

    PU_PTZ_CTRL_REQ,         //��Ϣ��: CPuPtzCtrlReq
    PU_PTZ_CTRL_RSP,         //��Ϣ��: CPuPtzCtrlRsp

    PU_PTZ_LOCK_REQ,         //��Ϣ��: CPuPtzLockReq
    PU_PTZ_LOCK_RSP,         //��Ϣ��: CPuPtzLockRsp

    PU_PTZ_UNLOCK_REQ,       //��Ϣ��: CPuPtzUnlockReq
    PU_PTZ_UNLOCK_RSP,       //��Ϣ��: CPuPtzUnlockRsp

    //�ýӿ�û��ʹ�ã����Ƿ����������������
    PU_SEND_TRANSPARENT_DATA_REQ, //��Ϣ��: CPuSendTransparentDataReq
    PU_SEND_TRANSPARENT_DATA_RSP, //��Ϣ��: CPuSendTransparentDataRsp

    PU_VID_ENC_PARAM_SET_REQ,     //��Ϣ��: CPuVidEncParamSetReq
    PU_VID_ENC_PARAM_SET_RSP,     //��Ϣ��: CPuVidEncParamSetRsp

    PU_VID_ENC_PARAM_GET_REQ,     //��Ϣ��: CPuVidEncParamGetReq
    PU_VID_ENC_PARAM_GET_RSP,     //��Ϣ��: CPuVidEncParamGetRsp

	PU_VID_ENC_PARAM_SET_EX_REQ,     //��Ϣ��: CPuVidEncParamExSetReq
	PU_VID_ENC_PARAM_SET_EX_RSP,     //��Ϣ��: CPuVidEncParamExSetRsp

	PU_VID_ENC_PARAM_GET_EX_REQ,     //��Ϣ��: CPuVidEncParamGetExReq
	PU_VID_ENC_PARAM_GET_EX_RSP,     //��Ϣ��: CPuVidEncParamGetExRsp

    PU_AUD_ENC_PARAM_SET_REQ,     //��Ϣ��: CPuAudEncParamSetReq
    PU_AUD_ENC_PARAM_SET_RSP,     //��Ϣ��: CPuAudEncParamSetRsp

    PU_AUD_ENC_PARAM_GET_REQ,     //��Ϣ��: CPuAudEncParamGetReq
    PU_AUD_ENC_PARAM_GET_RSP,     //��Ϣ��: CPuAudEncParamGetRsp

    //�ýӿ�ʹ��VSIP���ʵ����cu��ֱ��ʹ��pu�Ŀͻ�������ǰ������
    PU_VSIP_OPERATE_REQ,      //��Ϣ��: CPuVsipOperateReq
    PU_VSIP_OPERATE_RSP,      //��Ϣ��: CPuVsipOperateRsp

    //�ýӿ�͸��CU��PU��͸������
    PU_TRANSPARENT_OPERATE_REQ,      //��Ϣ��: CPuTransparentOperateReq
    PU_TRANSPARENT_OPERATE_RSP,      //��Ϣ��: CPuTransparentOperateRsp

    //�ýӿ�֧��req-rsp-ntfģ�͵�͸������(����֧��IPCV7��HTTP���)
    PU_TRANSPARENT_QRY_REQ,           //��Ϣ��: CPuTransparentQryReq
    PU_TRANSPARENT_QRY_RSP,           //��Ϣ��: CPuTransparentQryRsp
    PU_TRANSPARENT_QRY_NTF_REQ,       //��Ϣ��: CPuTransparentQryNtfReq
    PU_TRANSPARENT_QRY_NTF_RSP,       //��Ϣ��: CPuTransparentQryNtfRsp

    //�ýӿ�����pui��cmu�ϱ�͸������(ʹ�õ���MESSAGE����)
    PU_TRANSPARENT_DATA_NTF_REQ,      //��Ϣ��: CPuTransparentDataNtfReq
    PU_TRANSPARENT_DATA_NTF_RSP,      //��Ϣ��: CPuTransparentDataNtfRsp

    PU_SAVE_CFG_REQ,          //��Ϣ��: CPuSaveCfgReq
    PU_SAVE_CFG_RSP,          //��Ϣ��: CPuSaveCfgRsp

    PU_SYS_PARAM_SET_REQ,     //��Ϣ��: CPuSysParamSetReq
    PU_SYS_PARAM_SET_RSP,     //��Ϣ��: CPuSysParamSetRsp

    PU_SYS_PARAM_GET_REQ,     //��Ϣ��: CPuSysParamGetReq
    PU_SYS_PARAM_GET_RSP,     //��Ϣ��: CPuSysParamGetRsp

    PU_SEND_REAL_MEDIA_REQ,   //��Ϣ��: CPuSendRealMediaReq
    PU_SEND_REAL_MEDIA_RSP,   //��Ϣ��: CPuSendRealMediaRsp
    PU_SEND_REAL_MEDIA_ACK,   //��Ϣ��: CInviteThirdPartyCallAck

    PU_STOP_SEND_REAL_MEDIA_REQ,   //��Ϣ��: CPuStopSendRealMediaReq
    PU_STOP_SEND_REAL_MEDIA_RSP,   //��Ϣ��: CPuStopSendRealMediaRsp

    PU_REBOOT_REQ,            //��Ϣ��: CPuRebootReq
    PU_REBOOT_RSP,            //��Ϣ��: CPuRebootRsp

    INVITE_REQ,             //��Ϣ��: CInviteReq
    INVITE_RSP,             //��Ϣ��: CInviteRsp
    INVITE_ACK,             //��Ϣ��: CInviteACK

	BYE_REQ,				//��Ϣ��: CByeReq
	BYE_RSP,				//��Ϣ��: CByeRsp

    //{{Message�����ĵ���������
    START_THIRD_PARTY_CALL_REQ,     //��Ϣ��: CStartThirdPartyCallReq
    START_THIRD_PARTY_CALL_RSP,     //��Ϣ��: CStartThirdPartyCallRsp ,����ý����Ϣ

    STOP_THIRD_PARTY_CALL_REQ,      //��Ϣ��: CStopThirdPartyCallReq
    STOP_THIRD_PARTY_CALL_RSP,      //��Ϣ��: CStopThirdPartyCallRsp
    //}}

    //{{Invite�����ĵ���������,ֹͣҲ����Bye
    INVITE_THIRD_PARTY_CALL_REQ,    //��Ϣ��: CInviteThirdPartyCallReq
    INVITE_THIRD_PARTY_CALL_RSP,    //��Ϣ��: CInviteThirdPartyCallRsp
    INVITE_THIRD_PARTY_CALL_ACK,    //��Ϣ��: CInviteThirdPartyCallAck
    //}}

	//�����Ѿ����õĽ�����ַ
	UPDATE_SWITCH_ADDRESS_REQ,    //��Ϣ�壺CUpdateSwitchAddressReq
	UPDATE_SWITCH_ADDRESS_RSP,    //��Ϣ�壺CUpdateSwitchAddressRsp

    //�ؼ�֡
    KEY_FRAME_REQ,                  //��Ϣ��: CKeyFrameReq
    KEY_FRAME_RSP,                  //��Ϣ��: CKeyFrameRsp

    //������Ȳ�ѯ
    PLAYBACK_PROGRESS_QUERY_REQ,      //��Ϣ��: CPlaybackProgressQueryReq
    PLAYBACK_PROGRESS_QUERY_RSP,      //��Ϣ��: CPlaybackProgressQueryRsp

    PLAYBACK_PROGRESS_NTF_REQ,      //��Ϣ��: CPlaybackProgressNtfReq
    PLAYBACK_PROGRESS_NTF_RSP,      //��Ϣ��: CPlaybackProgressNtfRsp

    //�Ự��message��HZCTODO��CEndOfFileNtfReq::playbackSessUri����ȥ��
    END_OF_FILE_NTF_REQ,            //��Ϣ��: CEndOfFileNtfReq
    END_OF_FILE_NTF_RSP,            //��Ϣ��: CEndOfFileNtfRsp

    //�������
    VCR_CTRL_REQ,                   //��Ϣ��: CVcrCtrlReq
    VCR_CTRL_RSP,                   //��Ϣ��: CVcrCtrlRsp

    //cmu֮��Ľ�������
    CMU_CONNECT_REQ,        //��Ϣ��: CCmuConnectReq
    CMU_CONNECT_RSP,        //��Ϣ��: CCmuConnectRsp

    //cmu֮��Ķ���֪ͨ
    CMU_DISCONNECT_NTF_REQ,        //��Ϣ��: CCmuDisconnectNtfReq
    CMU_DISCONNECT_NTF_RSP,        //��Ϣ��: CCmuDisconnectNtfRsp

    //��Ϊͨ�õ��豸��¼������Ϣ��Ӧ����: devUri, devType, devAddrList
    DEV_LOGIN_REQ,          //��Ϣ��: CDevLoginReq
    DEV_LOGIN_RSP,          //��Ϣ��: CDevLoginRsp

    DEV_LOGOUT_REQ,         //��Ϣ��: CDevLogoutReq
    DEV_LOGOUT_RSP,         //��Ϣ��: CDevLogoutRsp

    //��ȡģ���IP��ַ��Ϣ
    NETADDR_QUERY_REQ,       //��Ϣ��: CNetAddrQueryReq
    NETADDR_QUERY_RSP,       //��Ϣ��: CNetAddrQueryRsp

    //ʹ��INVITE������Ϊͨ���ļ��ϴ�����������
    //����
    FILE_EXPORT_REQ,          //��Ϣ��: CFileExportReq
    FILE_EXPORT_RSP,          //��Ϣ��: CFileExportRsp
    FILE_EXPORT_ACK,          //��Ϣ��: CFileExportAck

    FILE_COLLECT_PROGRESS_NTF_REQ,         //��Ϣ��: CFileCollectProgressNtfReq
    FILE_COLLECT_PROGRESS_NTF_RSP,         //��Ϣ��: CFileCollectProgressNtfRsp

    //�ϴ�
    FILE_IMPORT_REQ,          //��Ϣ��: CFileImportReq
    FILE_IMPORT_RSP,          //��Ϣ��: CFileImportRsp
    FILE_IMPORT_ACK,          //��Ϣ��: CFileImportAck

    FILE_TRANS_END_REQ,      //��Ϣ��: CFileTransEndReq
    FILE_TRANS_END_RSP,      //��Ϣ��: CFileTransEndRsp

    STUN_UPDATE_REQ,        //��Ϣ��: CStunUpdateReq
    STUN_UPDATE_RSP,        //��Ϣ��: CStunUpdateRsp

    //�澯���澯�������
    PU_ALARM_INPUT_NTF_REQ, //��Ϣ��: CPuAlarmInputNtyReq
    PU_ALARM_INPUT_NTF_RSP, //��Ϣ��: CPuAlarmInputNtyRsp

    PU_READ_SEC_REQ,        //��Ϣ��: CPuReadSecReq
    PU_READ_SEC_RSP,        //��Ϣ��: CPuReadSecRsp

    PU_IO_OUT_REQ,       //��Ϣ��: CPuIoOutReq
    PU_IO_OUT_RSP,       //��Ϣ��: CPuIoOutRsp

	//�澯�ָ�
	PU_ALARM_RESET_REQ,  //��Ϣ�壺CPuAlarmResetReq
	PU_ALARM_RESET_RSP,  //��Ϣ�壺CPuAlarmResetRsp

    //PU_CAM_PREPOS_REQ = PU_PTZ_CTRL_REQ,      //��Ϣ��: CPuPtzCtrlReq
    //PU_CAM_PREPOS_RSP = PU_PTZ_CTRL_RSP,      //��Ϣ��: CPuPtzCtrlRsp  

    ALARMLINK_CFG_SET_REQ,     //��Ϣ��: CAlarmLinkCfgSetReq
    ALARMLINK_CFG_SET_RSP,     //��Ϣ��: CAlarmLinkCfgSetRsp

    ALARMLINK_CFG_GET_REQ,     //��Ϣ��: CAlarmLinkCfgGetReq
    ALARMLINK_CFG_GET_RSP,     //��Ϣ��: CAlarmLinkCfgGetRsp

    ALARMLINK_VERSION_GET_REQ,  //��Ϣ�壺CAlarmLinkVersionGetReq
    ALARMLINK_VERSION_GET_RSP,  //��Ϣ�壺CAlarmLinkVersionGetRsp

    ALARMLINK_QUERY_REQ,           //��Ϣ��: CAlarmLinkQueryReq
    ALARMLINK_QUERY_RSP,           //��Ϣ��: CAlarmLinkQueryRsp

    ALARMLINK_NTF_REQ,             //��Ϣ��: CAlarmLinkNtfReq
    ALARMLINK_NTF_RSP,             //��Ϣ��: CAlarmLinkNtfRsp

    ALARMLINK_MOD_NTF_REQ,             //��Ϣ��: CAlarmLinkModNtfReq
    ALARMLINK_MOD_NTF_RSP,             //��Ϣ��: CAlarmLinkModNtfRsp


    //����ǽ���
    REAL_MEDIA_ON_TV_REQ,      //��Ϣ��: CRealMediaOnTvReq
    REAL_MEDIA_ON_TV_RSP,      //��Ϣ��: CRealMediaOnTvRsp

    REAL_MEDIA_OFF_TV_REQ,     //��Ϣ��: CRealMediaOffTvReq
    REAL_MEDIA_OFF_TV_RSP,     //��Ϣ��: CRealMediaOffTvRsp

    //¼����ǽ
    REC_ON_TV_REQ,              //��Ϣ��: CRecOnTvReq
    REC_ON_TV_RSP,              //��Ϣ��: CRecOnTvRsp
    REC_ON_TV_ACK,              //��Ϣ��: CRecOnTvAck

    TV_POLL_OPERATE_REQ,       //��Ϣ��: CTvPollOperateReq
    TV_POLL_OPERATE_RSP,       //��Ϣ��: CTvPollOperateRsp

    TV_WIN_STYLE_SET_REQ,      //��Ϣ��: CTvWinStyleSetReq
    TV_WIN_STYLE_SET_RSP,      //��Ϣ��: CTvWinStyleSetRsp

    TV_WIN_STYLE_GET_REQ,      //��Ϣ��: CTvWinStyleGetReq
    TV_WIN_STYLE_GET_RSP,      //��Ϣ��: CTvWinStyleGetRsp

    TVWALL_SCHEME_SAVE_REQ,            //��Ϣ��: CTvWallSchemeSaveReq
    TVWALL_SCHEME_SAVE_RSP,            //��Ϣ��: CTvWallSchemeSaveRsp

    TVWALL_SCHEME_DEL_REQ,             //��Ϣ��: CTvWallSchemeDelReq
    TVWALL_SCHEME_DEL_RSP,             //��Ϣ��: CTvWallSchemeDelRsp

    TVWALL_SCHEME_QUERY_REQ,           //��Ϣ��: CTvWallSchemeQueryReq
    TVWALL_SCHEME_QUERY_RSP,           //��Ϣ��: CTvWallSchemeQueryRsp

    TVWALL_SCHEME_NTF_REQ,             //��Ϣ��: CTvWallSchemeNtfReq
    TVWALL_SCHEME_NTF_RSP,             //��Ϣ��: CTvWallSchemeNtfRsp

    TVWALL_SCHEME_LOAD_REQ,            //��Ϣ��: CTvWallSchemeLoadReq
    TVWALL_SCHEME_LOAD_RSP,            //��Ϣ��: CTvWallSchemeLoadRsp

    TVWALL_SCHEME_GROUP_SET_REQ,       //��Ϣ��: CTvWallSchemeGroupSetReq
    TVWALL_SCHEME_GROUP_SET_RSP,       //��Ϣ��: CTvWallSchemeGroupSetRsp

    TVWALL_SCHEME_GROUP_GET_REQ,       //��Ϣ��: CTvWallSchemeGroupGetReq
    TVWALL_SCHEME_GROUP_GET_RSP,       //��Ϣ��: CTvWallSchemeGroupGetRsp

    TVWALL_SCHEME_POLL_OPERATE_REQ,    //��Ϣ��: CTvWallSchemePollOperateReq
    TVWALL_SCHEME_POLL_OPERATE_RSP,    //��Ϣ��: CTvWallSchemePollOperateRsp

    PLAT_REC_ON_TV_REQ,      //��Ϣ��: CPlatRecOnTvReq
    PLAT_REC_ON_TV_RSP,      //��Ϣ��: CPlatRecOnTvRsp

    PLAT_REC_OFF_TV_REQ,     //��Ϣ��: CPlatRecOffTvReq
    PLAT_REC_OFF_TV_RSP,     //��Ϣ��: CPlatRecOffTvRsp

    PU_REC_ON_TV_REQ,        //��Ϣ��: CPuRecOnTvReq
    PU_REC_ON_TV_RSP,        //��Ϣ��: CPuRecOnTvRsp

    PU_REC_OFF_TV_REQ,       //��Ϣ��: CPuRecOffTvReq  (�ݲ�ʵ��)
    PU_REC_OFF_TV_RSP,       //��Ϣ��: CPuRecOffTvRsp  (�ݲ�ʵ��)

    TVWALL_STATUS_SS_REQ,    //��Ϣ��: CTvWallStatusSsReq
    TVWALL_STATUS_SS_RSP,    //��Ϣ��: CTvWallStatusSsRsp

    TVWALL_STATUS_UNSS_REQ,    //��Ϣ��: CTvWallStatusUnSsReq
    TVWALL_STATUS_UNSS_RSP,    //��Ϣ��: CTvWallStatusUnSsRsp

    TVWALL_STATUS_NTF_REQ,   //��Ϣ��: CTvWallStatusNtyReq
    TVWALL_STATUS_NTF_RSP,   //��Ϣ��: CTvWallStatusNtyRsp

    //UOI��ʹ�õĽӿ�---->begin
    TV_CHANNEL_LOCK_REQ,              //��Ϣ��: CTvChannelLockReq
    TV_CHANNEL_LOCK_RSP,              //��Ϣ��: CTvChannelLockRsp

    TV_CHANNEL_UNLOCK_REQ,            //��Ϣ��: CTvChannelUnlockReq
    TV_CHANNEL_UNLOCK_RSP,            //��Ϣ��: CTvChannelUnlockRsp

    TV_CHANNEL_WATCH_POINT_SAVE_REQ,  //��Ϣ��: CTvChannelWatchPointSaveReq
    TV_CHANNEL_WATCH_POINT_SAVE_RSP,  //��Ϣ��: CTvChannelWatchPointSaveRsp

    TV_CHANNEL_WATCH_POINT_LOAD_REQ,  //��Ϣ��: CTvChannelWatchPointLoadReq
    TV_CHANNEL_WATCH_POINT_LOAD_RSP,  //��Ϣ��: CTvChannelWatchPointLoadRsp

    TV_CHANNEL_WATCH_INFO_GET_REQ,    //��Ϣ��: CTvChannelWatchInfoGetReq
    TV_CHANNEL_WATCH_INFO_GET_RSP,    //��Ϣ��: CTvChannelWatchInfoGetRsp

    TVWALL_SCHEME_LOCK_REQ,           //��Ϣ��: CTvWallSchemeLockReq
    TVWALL_SCHEME_LOCK_RSP,           //��Ϣ��: CTvWallSchemeLockRsp

    TVWALL_SCHEME_UNLOCK_REQ,         //��Ϣ��: CTvWallSchemeUnlockReq
    TVWALL_SCHEME_UNLOCK_RSP,         //��Ϣ��: CTvWallSchemeUnlockRsp
    //UOI��ʹ�õĽӿ�---->end

	//����ͷ����
    PU_CAMERA_TYPE_SET_REQ,  //��Ϣ��: CPuCameraTypeSetReq
    PU_CAMERA_TYPE_SET_RSP,  //��Ϣ��: CPuCameraTypeSetRsp
	
    //�ƶ����
    PU_MOTION_DETECT_SET_REQ,  //��Ϣ��: CPuMotionDetectSetReq
    PU_MOTION_DETECT_SET_RSP,  //��Ϣ��: CPuMotionDetectSetRsp

    PU_MOTION_DETECT_GET_REQ,  //��Ϣ��: CPuMotionDetectGetReq
    PU_MOTION_DETECT_GET_RSP,  //��Ϣ��: CPuMotionDetectGetRsp

    //ͼ���ڱ�
    PU_AREA_SHADE_SET_REQ,    //��Ϣ��: CPuAreaShadeSetReq
    PU_AREA_SHADE_SET_RSP,    //��Ϣ��: CPuAreaShadeSetRsp

    PU_AREA_SHADE_GET_REQ,    //��Ϣ��: CPuAreaShadeGetReq
    PU_AREA_SHADE_GET_RSP,    //��Ϣ��: CPuAreaShadeGetRsp

    //GPS����
    PU_GPS_CFG_SET_REQ,        //��Ϣ��: CPuGpsCfgSetReq
    PU_GPS_CFG_SET_RSP,        //��Ϣ��: CPuGpsCfgSetRsp

    PU_GPS_CFG_GET_REQ,        //��Ϣ��: CPuGpsCfgGetReq
    PU_GPS_CFG_GET_RSP,        //��Ϣ��: CPuGpsCfgGetRsp

    PU_GPS_SS_SET_REQ,        //��Ϣ��: CPuGpsSsSetReq
    PU_GPS_SS_SET_RSP,        //��Ϣ��: CPuGpsSsSetRsp

    PU_GPS_SS_GET_REQ,        //��Ϣ��: CPuGpsSsGetReq
    PU_GPS_SS_GET_RSP,        //��Ϣ��: CPuGpsSsGetRsp

    PU_GPS_DATA_NTF_REQ,      //��Ϣ��: CPuGpsDataNtyReq
    PU_GPS_DATA_NTF_RSP,      //��Ϣ��: CPuGpsDataNtyRsp

    //͸���������õ����á���ȡ
    PU_TRANSDATA_CFG_SET_REQ,        //��Ϣ��: CPuTransdataCfgSetReq
    PU_TRANSDATA_CFG_SET_RSP,        //��Ϣ��: CPuTransdataCfgSetRsp

    PU_TRANSDATA_CFG_GET_REQ,        //��Ϣ��: CPuTransdataCfgGetReq
    PU_TRANSDATA_CFG_GET_RSP,        //��Ϣ��: CPuTransdataCfgGetRsp

    //ǰ��¼������ֹͣ
    PU_RECORD_START_REQ,      //��Ϣ��: CPuRecordStartReq
    PU_RECORD_START_RSP,      //��Ϣ��: CPuRecordStartRsp

	//{{{{{{{{{{{{{{{ add by Royan 20140526 ǰ��¼��״̬֪ͨ
	PU_RECORED_STATUS_NTF_REQ,   //��Ϣ��: CPuRecordStatusNtfReq
	PU_RECORED_STATUS_NTF_RSP,   //��Ϣ��: CPuRecordStatusNtfRsp
	//}}}}}}}}}}}}}}} add by Royan 20140526 ǰ��¼��״̬֪ͨ

    PU_RECORD_STOP_REQ,       //��Ϣ��: CPuRecordStopReq
    PU_RECORD_STOP_RSP,       //��Ϣ��: CPuRecordStopRsp

    //ǰ��¼��ɾ��
    PU_RECORD_DEL_REQ,       //��Ϣ��: CPuRecordDelReq
    PU_RECORD_DEL_RSP,       //��Ϣ��: CPuRecordDelRsp

    //ǰ��¼���ѯ
    PU_REC_QRY_REQ,           //��Ϣ��: CPuRecQryReq
    PU_REC_QRY_RSP,           //��Ϣ��: CPuRecQryRsp
    PU_REC_QRY_NTF_REQ,       //��Ϣ��: CPuRecQryNtfReq
    PU_REC_QRY_NTF_RSP,       //��Ϣ��: CPuRecQryNtfRsp

    //ǰ��¼��������ѯ
    PU_REC_DAYS_QRY_REQ,          //��Ϣ��: CPuRecDaysQryReq
    PU_REC_DAYS_QRY_RSP,          //��Ϣ��: CPuRecDaysQryRsp

    //������������á���ȡ
    PU_DEC_STYLE_SET_REQ,     //��Ϣ��: CPuDecStyleSetReq
    PU_DEC_STYLE_SET_RSP,     //��Ϣ��: CPuDecStyleSetRsp

    PU_DEC_STYLE_GET_REQ,     //��Ϣ��: CPuDecStyleGetReq
    PU_DEC_STYLE_GET_RSP,     //��Ϣ��: CPuDecStyleGetRsp

    //ǰ��ͼƬץ��
    PU_PIC_SNAPSHOT_REQ,      //��Ϣ��: CPuPicSnapshotReq
    PU_PIC_SNAPSHOT_RSP,      //��Ϣ��: CPuPicSnapshotRsp

    //ǰ��ͼƬ����
    PU_PIC_DOWNLOAD_REQ,      //��Ϣ��: CPuPicDownloadReq
    PU_PIC_DOWNLOAD_RSP,      //��Ϣ��: CPuPicDownloadRsp

    //ǰ��ͼƬ��ѯ
    PU_PIC_QRY_REQ,           //��Ϣ��: CPuPicQryReq
    PU_PIC_QRY_RSP,           //��Ϣ��: CPuPicQryRsp
    PU_PIC_QRY_NTF_REQ,       //��Ϣ��: CPuPicQryNtfReq
    PU_PIC_QRY_NTF_RSP,       //��Ϣ��: CPuPicQryNtfRsp

    //ǰ�˳���Ļ���á���ȡ
    PU_LONG_OSD_SET_REQ,      //��Ϣ��: CPuLongOsdSetReq
    PU_LONG_OSD_SET_RSP,      //��Ϣ��: CPuLongOsdSetRsp
    PU_LONG_OSD_GET_REQ,      //��Ϣ��: CPuLongOsdGetReq
    PU_LONG_OSD_GET_RSP,      //��Ϣ��: CPuLongOsdGetRsp

    //����̨��
    PU_LOGO_BMPOSD_TRANSMIT_REQ,   //��Ϣ��: CPuLogoBmpOsdTransmitReq
    PU_LOGO_BMPOSD_TRANSMIT_RSP,   //��Ϣ��: CPuLogoBmpOsdTransmitRsp

    //UA·�ɵǼ�����
    UA_ROUTER_REG_REQ,        //��Ϣ��: CUaRouterRegReq
    UA_ROUTER_REG_RSP,        //��Ϣ��: CUaRouterRegRsp

    //UA·�ɶ�������
    UA_ROUTER_SS_REQ,         //��Ϣ��: CUaRouterSsReq
    UA_ROUTER_SS_RSP,         //��Ϣ��: CUaRouterSsRsp

    UA_ROUTER_NTF_REQ,        //��Ϣ��: CUaRouterNtfReq
    UA_ROUTER_NTF_RSP,        //��Ϣ��: CUaRouterNtfRsp

	//{{{{{{{{{{{{{{{ add by Royan 20140418 ������Ļ
	PU_MULTI_OSD_SET_REQ,      //��Ϣ��: CPuMultiOsdSetReq
	PU_MULTI_OSD_SET_RSP,      //��Ϣ��: CPuMultiOsdSetRsp
	PU_MULTI_OSD_GET_REQ,      //��Ϣ��: CPuMultiOsdGetReq
	PU_MULTI_OSD_GET_RSP,      //��Ϣ��: CPuMultiOsdGetRsp
	//}}}}}}}}}}}}}}} add by Royan 20140418 ������Ļ

	//{{{{{{{{{{{{{{{ add by Royan 20140509 ���ܸ澯����
	DOMAIN_ALARM_REQ,          //��Ϣ��: CDomainAlarmReq
	DOMAIN_ALARM_RSP,          //��Ϣ��: CDomainAlarmRsp
	//}}}}}}}}}}}}}}} add by Royan 20140509 ���ܸ澯����

	//ǰ��ץ�Ĳ������� //add by ywy 2014.05.12
	PU_SNAPSHOT_CFG_GET_REQ,   //��Ϣ��: CPuSnapShotCfgGetReq 
	PU_SNAPSHOT_CFG_GET_RSP,   //��Ϣ��: CPuSnapShotCfgGetRsp 
	PU_SNAPSHOT_CFG_SET_REQ,   //��Ϣ��: CPuSnapShotCfgSetReq 
	PU_SNAPSHOT_CFG_SET_RSP,   //��Ϣ��: CPuSnapShotCfgSetRsp 

	//���ڲ�������
	PU_SERIALPARAM_CFG_GET_REQ, //��Ϣ��:CPuSerialParamCfgGetReq
	PU_SERIALPARAM_CFG_GET_RSP, //��Ϣ��:CPuSerialParamCfgGetRsp
	PU_SERIALPARAM_CFG_SET_REQ, //��Ϣ��:CPuSerialParamCfgSetReq
	PU_SERIALPARAM_CFG_SET_RSP, //��Ϣ��:CPuSerialParamCfgSetRsp
    
	//ͬ��ʱ�䵽pu
	PU_TIME_SYNC_REQ,           //��Ϣ�壺CPuSyncTimeReq
	PU_TIME_SYNC_RSP,           //��Ϣ�壺CPuSyncTimeRsp

	//porbe onvif ǰ��
	PROBE_ONVIFPU_REQ,          //��Ϣ��CProbeOnvifPuReq
	PROBE_ONVIFPU_RSP,          //��Ϣ��CProbeOnvifPuRsp

	UPDATE_REG_NETSEG_REQ,           //��Ϣ��: CUpdateRegNetSegReq
	UPDATE_REG_NETSEG_RSP,          //��Ϣ��: CUpdateRegNetSegRsp

	UPDATE_CULOGIN_INFO_REQ,           //��Ϣ��: CUpdateCuLoginInfoReq
	UPDATE_CULOGIN_INFO_RSP,            //��Ϣ��: CUpdateCuLoginInfoRsp

	// ����ͳ��
	CMU_PU_OFFLINEDEV_QRY_REQ,           //��Ϣ��: COffLineDevQryReq
	CMU_PU_OFFLINEDEV_QRY_RSP,           //��Ϣ��: COffLineDevQryRsp
	CMU_PU_OFFLINEDEV_QRY_NTF_REQ,       //��Ϣ��: COffLineDevQryNtfReq
	CMU_PU_OFFLINEDEV_QRY_NTF_RSP,       //��Ϣ��: COffLineDevQryNtfRsp

	RECORD_ON_TV_REQ,      //��Ϣ��: CRecordOnTvReq
	RECORD_ON_TV_RSP,      //��Ϣ��: CRecordOnTvRsp

	RECORD_OFF_TV_REQ,     //��Ϣ��: CRecordOffTvReq
	RECORD_OFF_TV_RSP,     //��Ϣ��: CRecordOffTvRsp

	//¼��ط���ǽʱ�������������¼������
	INVITE_DECORDER_CALL_REQ,    //��Ϣ��: CInviteDecoderCallReq
	INVITE_DECORDER_CALL_RSP,    //��Ϣ��: CInviteDecoderCallRsp
	INVITE_DECORDER_CALL_ACK,    //��Ϣ��: CInviteDecoderCallAck

	//ǰ����ƵԴ��������
	PU_VIDEOSRC_SET_REQ,         //��Ϣ�壺CPuVideosrcSetReq
	PU_VIDEOSRC_SET_RSP,         //��Ϣ�壺CPuVideosrcSetRsp

	//����ͬ����������
	CONFIG_SYNC_SWITCH_SET_REQ,     //��Ϣ�壺CCfgSyncSwitchSetReq
	CONFIG_SYNC_SWITCH_SET_RSP,     //��Ϣ�壺CCfgSyncSwitchSetRsp

	CONFIG_SYNC_SWITCH_GET_REQ,     //��Ϣ�壺CCfgSyncSwitchGetReq
	CONFIG_SYNC_SWITCH_GET_RSP,     //��Ϣ�壺CCfgSyncSwitchGetRsp

	PU_VIDSRCCFG_REFRESH_REQ,       //��Ϣ�壺CPuVidSrcCfgRefreshReq
	PU_VIDSRCCFG_REFRESH_RSP,       //��Ϣ�壺CPuVidSrcCfgRefreshRsp
	
	MODULE_ON_OFF_LOG_NTF_REQ,		//��Ϣ�壺CModuleOnOffLogNtfReq
	MODULE_ON_OFF_LOG_NTF_RSP,		//��Ϣ�壺CModuleOnOffLogNtfRsp

	ALARM_EVENT_NTF_REQ,			//��Ϣ�壺CAlarmEventNtfReq
	ALARM_EVENT_NTF_RSP,			//��Ϣ�壺CAlarmEventNtfRsp

	MAPPING_TABLE_NTF_REQ,			//��Ϣ�壺CMappingTableNtfReq
	MAPPING_TABLE_NTF_RSP,			//��Ϣ�壺CMappingTableNtfRsp

	DOMAIN_TABLE_NTF_REQ,			//��Ϣ�壺CDomainTableNtfReq
	DOMAIN_TABLE_NTF_RSP,			//��Ϣ�壺CDomainTableNtfRsp

	PU_ONLINE_NTF_REQ,	     		//��Ϣ�壺CPuOnlineNtfReq
	PU_ONLINE_NTF_RSP,		    	//��Ϣ�壺CPuOnlineNtfRsp

	PU_VIDSRC_OSD_SET_REQ,          //��Ϣ�壺CPuVidSrcOsdSetReq
	PU_VIDSRC_OSD_SET_RSP,          //��Ϣ�壺CPuVidSrcOsdSetRsp
	PU_VIDSRC_OSD_GET_REQ,          //��Ϣ�壺CPuVidSrcOsdGetReq
	PU_VIDSRC_OSD_GET_RSP,          //��Ϣ�壺CPuVidSrcOsdGetRsp

    //modified by zzc
    //���� ���� tvs ��ǽ�����л�
    //�μ� 
    //REAL_MEDIA_ON / OFF _TV_REQ
    REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ,//��Ϣ�壺CRealMediaSwitchOccurTypeReq
    REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP,//��Ϣ�壺CRealMediaSwitchOccurTypeRsp

	//modified by hrj
	ALARM_TYPE_GET_REQ,              //��Ϣ�壺CGetAlarmTypesReq
	ALARM_TYPE_GET_RSP,              //��Ϣ�壺CGetAlarmTypesRsp

	UPDATE_PU_VKEKS_NTF_REQ,        //��Ϣ�壺CUpdatePuVkeksNtfReq
	UPDATE_PU_VKEKS_NTF_RSP,        //��Ϣ�壺CUpdatePuVkeksNtfRsp
	PU_SIGNATURE_CTRL_REQ,			//��Ϣ�壺CPuSignatrueCtrlReq
	PU_SIGNATURE_CTRL_RSP,			//��Ϣ�壺 CPuSignatrueCtrlRsp
	PU_ENCRYPTION_CTRL_REQ,			//��Ϣ�壺 CPuEncryptionCtrlReq
	PU_ENCRYPTION_CTRL_RSP,			//��Ϣ�壺 CPuEncryptionCtrlRsp

    CMS_CURRENT_EVENT_END,
};

/////////////////////////////////////////////////////////////////////
//VTDU�ӿ���Ϣ
/////////////////////////////////////////////////////////////////////
enum EVtduMsg
{
    VTDU_REG_REQ = VTDU_EVENT_BEGIN,    //VTDU��CMUע��������Ϣ��: CVtduRegReq
    VTDU_REG_RSP,						//CMU��VTDU��ע��Ӧ����Ϣ��: CVtduRegRsp

    VTDU_ADDR_ASSIGN_REQ,               //��VTDU����˿�������Ϣ��: CVtduAddrAssignReq
    VTDU_ADDR_ASSIGN_RSP,               //��VTDU����˿�Ӧ����Ϣ��: CVtduAddrAssignRsp

    VTDU_ADDSWITCH_REQ,                 //��VTDU��ӽ���������Ϣ��: CVtduAddSwitchReq
    VTDU_ADDSWITCH_RSP,                 //��VTDU��ӽ���Ӧ����Ϣ��: CVtduAddSwitchRsp

    VTDU_REMOVESWITCH_REQ,              //��VTDU���Ƴ�����������Ϣ��: ΪCVtduRemoveSwitchReq
    VTDU_REMOVESWITCH_RSP,              //��VTDU���Ƴ�����Ӧ����Ϣ��: CVtduRemoveSwitchRsp

    VTDU_PUNCH_RESULT_NTF_REQ,          //vtdu��Դ��Ŀ�Ķ��Ƿ�򶴳ɹ�����Ϣ��: CVtduPunchResultNtfReq
    VTDU_PUNCH_RESULT_NTF_RSP,          //vtdu��Դ��Ŀ�Ķ��Ƿ�򶴳ɹ�����Ϣ��: CVtduPunchResultNtfRsp

    VTDU_QUERY_MAPPED_ADDR_NTF,         //vtdulib��ѯ������ַ֪ͨ��Osp��Ϣ��TVtduStunResult
    VTDU_PUNCH_RESULT_NTF,              //vtdulibPunch���֪ͨ��Osp��Ϣ��TVtduPunchResult

    //VTDU_FORCE_DISCONNECT_BY_CMU,       //Vtdu���������Ϣ����

    VTDU_EVENT_NUM                      //VTDU��Ϣ����
};

inline void InitCmsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(PUI_REG_REQ);                             //��Ϣ��: CPuiRegReq
    OSP_ADD_EVENT_DESC(PUI_REG_RSP);                             //��Ϣ��: CPuiRegRsp
    OSP_ADD_EVENT_DESC(PU_REG_REQ);                              //��Ϣ��: CPuRegReq
    OSP_ADD_EVENT_DESC(PU_REG_RSP);                              //��Ϣ��: CPuRegRsp
    OSP_ADD_EVENT_DESC(PU_UNREG_REQ);                            //��Ϣ��: CPuUnRegReq
    OSP_ADD_EVENT_DESC(PU_UNREG_RSP);                            //��Ϣ��: CPuUnRegRsp
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_PU_REQ);                   //��Ϣ��: CCmuDisconnectPuReq
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_PU_RSP);                   //��Ϣ��: CCmuDisconnectPuRsp
    OSP_ADD_EVENT_DESC(CU_LOGIN_REQ);                            //��Ϣ��: CCuLoginReq
    OSP_ADD_EVENT_DESC(CU_LOGIN_RSP);                            //��Ϣ��: CCuLoginRsp
    OSP_ADD_EVENT_DESC(CU_LOGOUT_REQ);                           //��Ϣ��: CCuLogoutReq
    OSP_ADD_EVENT_DESC(CU_LOGOUT_RSP);                           //��Ϣ��: CCuLogoutRsp
    OSP_ADD_EVENT_DESC(CU_DISCONNECT_NTF_REQ);                   //��Ϣ��: CCuDisconnectNtfReq
    OSP_ADD_EVENT_DESC(CU_DISCONNECT_NTF_RSP);                   //��Ϣ��: CCuDisconnectNtfRsp
    OSP_ADD_EVENT_DESC(CU_GETCUILIST_REQ);                       //��Ϣ��: CCuGetCuiListReq
    OSP_ADD_EVENT_DESC(CU_GETCUILIST_RSP);                       //��Ϣ��: CCuGetCuiListRsp
    OSP_ADD_EVENT_DESC(CUI_REG_REQ);                             //��Ϣ��: CCuiRegReq
    OSP_ADD_EVENT_DESC(CUI_REG_RSP);                             //��Ϣ��: CCuiRegRsp
    OSP_ADD_EVENT_DESC(CUI_GET_AZIMUTH_REQ);                     //��Ϣ��: CCuiGetAzimuthReq
    OSP_ADD_EVENT_DESC(CUI_GET_AZIMUTH_RSP);                     //��Ϣ��: CCuiGetAzimuthRsp
    OSP_ADD_EVENT_DESC(BATCH_OPERATE_REQ);                       // ��Ϣ�壺CBatchOperateReq
    OSP_ADD_EVENT_DESC(BATCH_OPERATE_RSP);                       // ��Ϣ�壺CBatchOperateRsp
    OSP_ADD_EVENT_DESC(BATCH_PU_REG_REQ);                        // ��Ϣ�壺CBatchPuRegReq
    OSP_ADD_EVENT_DESC(BATCH_PU_REG_RSP);                        // ��Ϣ�壺CBatchPuRegRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_SS_REQ);                   //��Ϣ��: CCatalogStatusSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_SS_RSP);                   //��Ϣ��: CCatalogStatusSsRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_REFRESH_SS_REQ);           //��Ϣ��: CCatalogStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_REFRESH_SS_RSP);           //��Ϣ��: CCatalogStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_UNSS_REQ);                 //��Ϣ��: CCatalogStatusUnSsReq
    OSP_ADD_EVENT_DESC(CATALOG_STATUS_UNSS_RSP);                 //��Ϣ��: CCatalogStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_SS_REQ);                    //��Ϣ��: CPuListStatusSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_SS_RSP);                    //��Ϣ��: CPuListStatusSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_REFRESH_SS_REQ);            //��Ϣ��: CPuListStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_REFRESH_SS_RSP);            //��Ϣ��: CPuListStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(PULIST_STATUS_UNSS_REQ);                  //��Ϣ��: CPuListStatusUnSsReq
    OSP_ADD_EVENT_DESC(PULIST_STATUS_UNSS_RSP);                  //��Ϣ��: CPuListStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_SS_REQ);                        //��Ϣ��: CPuStatusSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_SS_RSP);                        //��Ϣ��: CPuStatusSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_REFRESH_SS_REQ);                //��Ϣ��: CPuStatusRefreshSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_REFRESH_SS_RSP);                //��Ϣ��: CPuStatusRefreshSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_UNSS_REQ);                      //��Ϣ��: CPuStatusUnSsReq
    OSP_ADD_EVENT_DESC(PU_STATUS_UNSS_RSP);                      //��Ϣ��: CPuStatusUnSsRsp
    OSP_ADD_EVENT_DESC(PU_STATUS_NTF_REQ);                       //��Ϣ��: CPuStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_STATUS_NTF_RSP);                       //��Ϣ��: CPuStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_DEV_CONFIG_NTF_REQ);                   //��Ϣ��: CPuDevConfigNtyReq
    OSP_ADD_EVENT_DESC(PU_DEV_CONFIG_NTF_RSP);                   //��Ϣ��: CPuDevConfigNtyRsp
    OSP_ADD_EVENT_DESC(PU_CONFIG_NTF_REQ);                       //��Ϣ��: CPuConfigNtyReq
    OSP_ADD_EVENT_DESC(PU_CONFIG_NTF_RSP);                       //��Ϣ��: CPuConfigNtyRsp
    OSP_ADD_EVENT_DESC(PU_SERVICE_STATUS_NTF_REQ);               //��Ϣ��: CPuServiceStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_SERVICE_STATUS_NTF_RSP);               //��Ϣ��: CPuServiceStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_STATUS_NTF_REQ);                 //��Ϣ��: CPuAlarmStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_ALARM_STATUS_NTF_RSP);                 //��Ϣ��: CPuAlarmStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_GPS_STATUS_NTF_REQ);                   //��Ϣ��: CPuGpsStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_GPS_STATUS_NTF_RSP);                   //��Ϣ��: CPuGpsStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_STATUS_NTF_REQ);             //��Ϣ��: CPuTransDataStatusNtyReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_STATUS_NTF_RSP);             //��Ϣ��: CPuTransDataStatusNtyRsp
    OSP_ADD_EVENT_DESC(PU_BATCH_NTF_REQ);                        //��Ϣ�壺CPuBatchNtyReq
    OSP_ADD_EVENT_DESC(PU_BATCH_NTF_RSP);                        //��Ϣ�壺CPuBatchNtyRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_CTRL_REQ);                         //��Ϣ��: CPuPtzCtrlReq
    OSP_ADD_EVENT_DESC(PU_PTZ_CTRL_RSP);                         //��Ϣ��: CPuPtzCtrlRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_LOCK_REQ);                         //��Ϣ��: CPuPtzLockReq
    OSP_ADD_EVENT_DESC(PU_PTZ_LOCK_RSP);                         //��Ϣ��: CPuPtzLockRsp
    OSP_ADD_EVENT_DESC(PU_PTZ_UNLOCK_REQ);                       //��Ϣ��: CPuPtzUnlockReq
    OSP_ADD_EVENT_DESC(PU_PTZ_UNLOCK_RSP);                       //��Ϣ��: CPuPtzUnlockRsp
    OSP_ADD_EVENT_DESC(PU_SEND_TRANSPARENT_DATA_REQ);            //��Ϣ��: CPuSendTransparentDataReq
    OSP_ADD_EVENT_DESC(PU_SEND_TRANSPARENT_DATA_RSP);            //��Ϣ��: CPuSendTransparentDataRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_REQ);                //��Ϣ��: CPuVidEncParamSetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_RSP);                //��Ϣ��: CPuVidEncParamSetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_REQ);                //��Ϣ��: CPuVidEncParamGetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_RSP);                //��Ϣ��: CPuVidEncParamGetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_EX_REQ);             //��Ϣ��: CPuVidEncParamExSetReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_SET_EX_RSP);             //��Ϣ��: CPuVidEncParamExSetRsp
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_EX_REQ);             //��Ϣ��: CPuVidEncParamGetExReq
    OSP_ADD_EVENT_DESC(PU_VID_ENC_PARAM_GET_EX_RSP);             //��Ϣ��: CPuVidEncParamGetExRsp
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_SET_REQ);                //��Ϣ��: CPuAudEncParamSetReq
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_SET_RSP);                //��Ϣ��: CPuAudEncParamSetRsp
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_GET_REQ);                //��Ϣ��: CPuAudEncParamGetReq
    OSP_ADD_EVENT_DESC(PU_AUD_ENC_PARAM_GET_RSP);                //��Ϣ��: CPuAudEncParamGetRsp
    OSP_ADD_EVENT_DESC(PU_VSIP_OPERATE_REQ);                     //��Ϣ��: CPuVsipOperateReq
    OSP_ADD_EVENT_DESC(PU_VSIP_OPERATE_RSP);                     //��Ϣ��: CPuVsipOperateRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_OPERATE_REQ);              //��Ϣ��: CPuTransparentOperateReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_OPERATE_RSP);              //��Ϣ��: CPuTransparentOperateRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_REQ);                  //��Ϣ��: CPuTransparentQryReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_RSP);                  //��Ϣ��: CPuTransparentQryRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_NTF_REQ);              //��Ϣ��: CPuTransparentQryNtfReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_QRY_NTF_RSP);              //��Ϣ��: CPuTransparentQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_DATA_NTF_REQ);             //��Ϣ��: CPuTransparentDataNtfReq
    OSP_ADD_EVENT_DESC(PU_TRANSPARENT_DATA_NTF_RSP);             //��Ϣ��: CPuTransparentDataNtfRsp
    OSP_ADD_EVENT_DESC(PU_SAVE_CFG_REQ);                         //��Ϣ��: CPuSaveCfgReq
    OSP_ADD_EVENT_DESC(PU_SAVE_CFG_RSP);                         //��Ϣ��: CPuSaveCfgRsp
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_SET_REQ);                    //��Ϣ��: CPuSysParamSetReq
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_SET_RSP);                    //��Ϣ��: CPuSysParamSetRsp
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_GET_REQ);                    //��Ϣ��: CPuSysParamGetReq
    OSP_ADD_EVENT_DESC(PU_SYS_PARAM_GET_RSP);                    //��Ϣ��: CPuSysParamGetRsp
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_REQ);                  //��Ϣ��: CPuSendRealMediaReq
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_RSP);                  //��Ϣ��: CPuSendRealMediaRsp
    OSP_ADD_EVENT_DESC(PU_SEND_REAL_MEDIA_ACK);                  //��Ϣ��: CInviteThirdPartyCallAck
    OSP_ADD_EVENT_DESC(PU_STOP_SEND_REAL_MEDIA_REQ);             //��Ϣ��: CPuStopSendRealMediaReq
    OSP_ADD_EVENT_DESC(PU_STOP_SEND_REAL_MEDIA_RSP);             //��Ϣ��: CPuStopSendRealMediaRsp
    OSP_ADD_EVENT_DESC(PU_REBOOT_REQ);                           //��Ϣ��: CPuRebootReq
    OSP_ADD_EVENT_DESC(PU_REBOOT_RSP);                           //��Ϣ��: CPuRebootRsp
    OSP_ADD_EVENT_DESC(INVITE_REQ);                              //��Ϣ��: CInviteReq
    OSP_ADD_EVENT_DESC(INVITE_RSP);                              //��Ϣ��: CInviteRsp
    OSP_ADD_EVENT_DESC(INVITE_ACK);                              //��Ϣ��: CInviteACK
    OSP_ADD_EVENT_DESC(BYE_REQ);                                 //��Ϣ��: CByeReq
    OSP_ADD_EVENT_DESC(BYE_RSP);                                 //��Ϣ��: CByeRsp
    OSP_ADD_EVENT_DESC(START_THIRD_PARTY_CALL_REQ);              //��Ϣ��: CStartThirdPartyCallReq
    OSP_ADD_EVENT_DESC(START_THIRD_PARTY_CALL_RSP);              //��Ϣ��: CStartThirdPartyCallRsp ,����ý����Ϣ
    OSP_ADD_EVENT_DESC(STOP_THIRD_PARTY_CALL_REQ);               //��Ϣ��: CStopThirdPartyCallReq
    OSP_ADD_EVENT_DESC(STOP_THIRD_PARTY_CALL_RSP);               //��Ϣ��: CStopThirdPartyCallRsp
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_REQ);             //��Ϣ��: CInviteThirdPartyCallReq
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_RSP);             //��Ϣ��: CInviteThirdPartyCallRsp
    OSP_ADD_EVENT_DESC(INVITE_THIRD_PARTY_CALL_ACK);             //��Ϣ��: CInviteThirdPartyCallAck
    OSP_ADD_EVENT_DESC(UPDATE_SWITCH_ADDRESS_REQ);               //��Ϣ�壺CUpdateSwitchAddressReq
    OSP_ADD_EVENT_DESC(UPDATE_SWITCH_ADDRESS_RSP);               //��Ϣ�壺CUpdateSwitchAddressRsp
    OSP_ADD_EVENT_DESC(KEY_FRAME_REQ);                           //��Ϣ��: CKeyFrameReq
    OSP_ADD_EVENT_DESC(KEY_FRAME_RSP);                           //��Ϣ��: CKeyFrameRsp
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_QUERY_REQ);             //��Ϣ��: CPlaybackProgressQueryReq
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_QUERY_RSP);             //��Ϣ��: CPlaybackProgressQueryRsp
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_NTF_REQ);               //��Ϣ��: CPlaybackProgressNtfReq
    OSP_ADD_EVENT_DESC(PLAYBACK_PROGRESS_NTF_RSP);               //��Ϣ��: CPlaybackProgressNtfRsp
    OSP_ADD_EVENT_DESC(END_OF_FILE_NTF_REQ);                     //��Ϣ��: CEndOfFileNtfReq
    OSP_ADD_EVENT_DESC(END_OF_FILE_NTF_RSP);                     //��Ϣ��: CEndOfFileNtfRsp
    OSP_ADD_EVENT_DESC(VCR_CTRL_REQ);                            //��Ϣ��: CVcrCtrlReq
    OSP_ADD_EVENT_DESC(VCR_CTRL_RSP);                            //��Ϣ��: CVcrCtrlRsp
    OSP_ADD_EVENT_DESC(CMU_CONNECT_REQ);                         //��Ϣ��: CCmuConnectReq
    OSP_ADD_EVENT_DESC(CMU_CONNECT_RSP);                         //��Ϣ��: CCmuConnectRsp
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_NTF_REQ);                  //��Ϣ��: CCmuDisconnectNtfReq
    OSP_ADD_EVENT_DESC(CMU_DISCONNECT_NTF_RSP);                  //��Ϣ��: CCmuDisconnectNtfRsp
    OSP_ADD_EVENT_DESC(DEV_LOGIN_REQ);                           //��Ϣ��: CDevLoginReq
    OSP_ADD_EVENT_DESC(DEV_LOGIN_RSP);                           //��Ϣ��: CDevLoginRsp
    OSP_ADD_EVENT_DESC(DEV_LOGOUT_REQ);                          //��Ϣ��: CDevLogoutReq
    OSP_ADD_EVENT_DESC(DEV_LOGOUT_RSP);                          //��Ϣ��: CDevLogoutRsp
    OSP_ADD_EVENT_DESC(NETADDR_QUERY_REQ);                       //��Ϣ��: CNetAddrQueryReq
    OSP_ADD_EVENT_DESC(NETADDR_QUERY_RSP);                       //��Ϣ��: CNetAddrQueryRsp
    OSP_ADD_EVENT_DESC(FILE_EXPORT_REQ);                         //��Ϣ��: CFileExportReq
    OSP_ADD_EVENT_DESC(FILE_EXPORT_RSP);                         //��Ϣ��: CFileExportRsp
    OSP_ADD_EVENT_DESC(FILE_EXPORT_ACK);                         //��Ϣ��: CFileExportAck
    OSP_ADD_EVENT_DESC(FILE_COLLECT_PROGRESS_NTF_REQ);           //��Ϣ��: CFileCollectProgressNtfReq
    OSP_ADD_EVENT_DESC(FILE_COLLECT_PROGRESS_NTF_RSP);           //��Ϣ��: CFileCollectProgressNtfRsp
    OSP_ADD_EVENT_DESC(FILE_IMPORT_REQ);                         //��Ϣ��: CFileImportReq
    OSP_ADD_EVENT_DESC(FILE_IMPORT_RSP);                         //��Ϣ��: CFileImportRsp
    OSP_ADD_EVENT_DESC(FILE_IMPORT_ACK);                         //��Ϣ��: CFileImportAck
    OSP_ADD_EVENT_DESC(FILE_TRANS_END_REQ);                      //��Ϣ��: CFileTransEndReq
    OSP_ADD_EVENT_DESC(FILE_TRANS_END_RSP);                      //��Ϣ��: CFileTransEndRsp
    OSP_ADD_EVENT_DESC(STUN_UPDATE_REQ);                         //��Ϣ��: CStunUpdateReq
    OSP_ADD_EVENT_DESC(STUN_UPDATE_RSP);                         //��Ϣ��: CStunUpdateRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_INPUT_NTF_REQ);                  //��Ϣ��: CPuAlarmInputNtyReq
    OSP_ADD_EVENT_DESC(PU_ALARM_INPUT_NTF_RSP);                  //��Ϣ��: CPuAlarmInputNtyRsp
    OSP_ADD_EVENT_DESC(PU_READ_SEC_REQ);                         //��Ϣ��: CPuReadSecReq
    OSP_ADD_EVENT_DESC(PU_READ_SEC_RSP);                         //��Ϣ��: CPuReadSecRsp
    OSP_ADD_EVENT_DESC(PU_IO_OUT_REQ);                           //��Ϣ��: CPuIoOutReq
    OSP_ADD_EVENT_DESC(PU_IO_OUT_RSP);                           //��Ϣ��: CPuIoOutRsp
    OSP_ADD_EVENT_DESC(PU_ALARM_RESET_REQ);                      //��Ϣ�壺CPuAlarmResetReq
    OSP_ADD_EVENT_DESC(PU_ALARM_RESET_RSP);                      //��Ϣ�壺CPuAlarmResetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_SET_REQ);                   //��Ϣ��: CAlarmLinkCfgSetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_SET_RSP);                   //��Ϣ��: CAlarmLinkCfgSetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_GET_REQ);                   //��Ϣ��: CAlarmLinkCfgGetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_CFG_GET_RSP);                   //��Ϣ��: CAlarmLinkCfgGetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_VERSION_GET_REQ);               //��Ϣ�壺CAlarmLinkVersionGetReq
    OSP_ADD_EVENT_DESC(ALARMLINK_VERSION_GET_RSP);               //��Ϣ�壺CAlarmLinkVersionGetRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_QUERY_REQ);                     //��Ϣ��: CAlarmLinkQueryReq
    OSP_ADD_EVENT_DESC(ALARMLINK_QUERY_RSP);                     //��Ϣ��: CAlarmLinkQueryRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_NTF_REQ);                       //��Ϣ��: CAlarmLinkNtfReq
    OSP_ADD_EVENT_DESC(ALARMLINK_NTF_RSP);                       //��Ϣ��: CAlarmLinkNtfRsp
    OSP_ADD_EVENT_DESC(ALARMLINK_MOD_NTF_REQ);                   //��Ϣ��: CAlarmLinkModNtfReq
    OSP_ADD_EVENT_DESC(ALARMLINK_MOD_NTF_RSP);                   //��Ϣ��: CAlarmLinkModNtfRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_ON_TV_REQ);                    //��Ϣ��: CRealMediaOnTvReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_ON_TV_RSP);                    //��Ϣ��: CRealMediaOnTvRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_OFF_TV_REQ);                   //��Ϣ��: CRealMediaOffTvReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_OFF_TV_RSP);                   //��Ϣ��: CRealMediaOffTvRsp
    OSP_ADD_EVENT_DESC(REC_ON_TV_REQ);                           //��Ϣ��: CRecOnTvReq
    OSP_ADD_EVENT_DESC(REC_ON_TV_RSP);                           //��Ϣ��: CRecOnTvRsp
    OSP_ADD_EVENT_DESC(REC_ON_TV_ACK);                           //��Ϣ��: CRecOnTvAck
    OSP_ADD_EVENT_DESC(TV_POLL_OPERATE_REQ);                     //��Ϣ��: CTvPollOperateReq
    OSP_ADD_EVENT_DESC(TV_POLL_OPERATE_RSP);                     //��Ϣ��: CTvPollOperateRsp
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_SET_REQ);                    //��Ϣ��: CTvWinStyleSetReq
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_SET_RSP);                    //��Ϣ��: CTvWinStyleSetRsp
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_GET_REQ);                    //��Ϣ��: CTvWinStyleGetReq
    OSP_ADD_EVENT_DESC(TV_WIN_STYLE_GET_RSP);                    //��Ϣ��: CTvWinStyleGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_SAVE_REQ);                  //��Ϣ��: CTvWallSchemeSaveReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_SAVE_RSP);                  //��Ϣ��: CTvWallSchemeSaveRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_DEL_REQ);                   //��Ϣ��: CTvWallSchemeDelReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_DEL_RSP);                   //��Ϣ��: CTvWallSchemeDelRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_QUERY_REQ);                 //��Ϣ��: CTvWallSchemeQueryReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_QUERY_RSP);                 //��Ϣ��: CTvWallSchemeQueryRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_NTF_REQ);                   //��Ϣ��: CTvWallSchemeNtfReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_NTF_RSP);                   //��Ϣ��: CTvWallSchemeNtfRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOAD_REQ);                  //��Ϣ��: CTvWallSchemeLoadReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOAD_RSP);                  //��Ϣ��: CTvWallSchemeLoadRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_SET_REQ);             //��Ϣ��: CTvWallSchemeGroupSetReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_SET_RSP);             //��Ϣ��: CTvWallSchemeGroupSetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_GET_REQ);             //��Ϣ��: CTvWallSchemeGroupGetReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_GROUP_GET_RSP);             //��Ϣ��: CTvWallSchemeGroupGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_POLL_OPERATE_REQ);          //��Ϣ��: CTvWallSchemePollOperateReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_POLL_OPERATE_RSP);          //��Ϣ��: CTvWallSchemePollOperateRsp
    OSP_ADD_EVENT_DESC(PLAT_REC_ON_TV_REQ);                      //��Ϣ��: CPlatRecOnTvReq
    OSP_ADD_EVENT_DESC(PLAT_REC_ON_TV_RSP);                      //��Ϣ��: CPlatRecOnTvRsp
    OSP_ADD_EVENT_DESC(PLAT_REC_OFF_TV_REQ);                     //��Ϣ��: CPlatRecOffTvReq
    OSP_ADD_EVENT_DESC(PLAT_REC_OFF_TV_RSP);                     //��Ϣ��: CPlatRecOffTvRsp
    OSP_ADD_EVENT_DESC(PU_REC_ON_TV_REQ);                        //��Ϣ��: CPuRecOnTvReq
    OSP_ADD_EVENT_DESC(PU_REC_ON_TV_RSP);                        //��Ϣ��: CPuRecOnTvRsp
    OSP_ADD_EVENT_DESC(PU_REC_OFF_TV_REQ);                       //��Ϣ��: CPuRecOffTvReq  (�ݲ�ʵ��)
    OSP_ADD_EVENT_DESC(PU_REC_OFF_TV_RSP);                       //��Ϣ��: CPuRecOffTvRsp  (�ݲ�ʵ��)
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_SS_REQ);                    //��Ϣ��: CTvWallStatusSsReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_SS_RSP);                    //��Ϣ��: CTvWallStatusSsRsp
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_UNSS_REQ);                  //��Ϣ��: CTvWallStatusUnSsReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_UNSS_RSP);                  //��Ϣ��: CTvWallStatusUnSsRsp
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_NTF_REQ);                   //��Ϣ��: CTvWallStatusNtyReq
    OSP_ADD_EVENT_DESC(TVWALL_STATUS_NTF_RSP);                   //��Ϣ��: CTvWallStatusNtyRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_LOCK_REQ);                     //��Ϣ��: CTvChannelLockReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_LOCK_RSP);                     //��Ϣ��: CTvChannelLockRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_UNLOCK_REQ);                   //��Ϣ��: CTvChannelUnlockReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_UNLOCK_RSP);                   //��Ϣ��: CTvChannelUnlockRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_SAVE_REQ);         //��Ϣ��: CTvChannelWatchPointSaveReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_SAVE_RSP);         //��Ϣ��: CTvChannelWatchPointSaveRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_LOAD_REQ);         //��Ϣ��: CTvChannelWatchPointLoadReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_POINT_LOAD_RSP);         //��Ϣ��: CTvChannelWatchPointLoadRsp
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_INFO_GET_REQ);           //��Ϣ��: CTvChannelWatchInfoGetReq
    OSP_ADD_EVENT_DESC(TV_CHANNEL_WATCH_INFO_GET_RSP);           //��Ϣ��: CTvChannelWatchInfoGetRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOCK_REQ);                  //��Ϣ��: CTvWallSchemeLockReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_LOCK_RSP);                  //��Ϣ��: CTvWallSchemeLockRsp
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_UNLOCK_REQ);                //��Ϣ��: CTvWallSchemeUnlockReq
    OSP_ADD_EVENT_DESC(TVWALL_SCHEME_UNLOCK_RSP);                //��Ϣ��: CTvWallSchemeUnlockRsp
    OSP_ADD_EVENT_DESC(PU_CAMERA_TYPE_SET_REQ);                  //��Ϣ��: CPuCameraTypeSetReq
    OSP_ADD_EVENT_DESC(PU_CAMERA_TYPE_SET_RSP);                  //��Ϣ��: CPuCameraTypeSetRsp
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_SET_REQ);                //��Ϣ��: CPuMotionDetectSetReq
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_SET_RSP);                //��Ϣ��: CPuMotionDetectSetRsp
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_GET_REQ);                //��Ϣ��: CPuMotionDetectGetReq
    OSP_ADD_EVENT_DESC(PU_MOTION_DETECT_GET_RSP);                //��Ϣ��: CPuMotionDetectGetRsp
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_SET_REQ);                   //��Ϣ��: CPuAreaShadeSetReq
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_SET_RSP);                   //��Ϣ��: CPuAreaShadeSetRsp
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_GET_REQ);                   //��Ϣ��: CPuAreaShadeGetReq
    OSP_ADD_EVENT_DESC(PU_AREA_SHADE_GET_RSP);                   //��Ϣ��: CPuAreaShadeGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_SET_REQ);                      //��Ϣ��: CPuGpsCfgSetReq
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_SET_RSP);                      //��Ϣ��: CPuGpsCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_GET_REQ);                      //��Ϣ��: CPuGpsCfgGetReq
    OSP_ADD_EVENT_DESC(PU_GPS_CFG_GET_RSP);                      //��Ϣ��: CPuGpsCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_SS_SET_REQ);                       //��Ϣ��: CPuGpsSsSetReq
    OSP_ADD_EVENT_DESC(PU_GPS_SS_SET_RSP);                       //��Ϣ��: CPuGpsSsSetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_SS_GET_REQ);                       //��Ϣ��: CPuGpsSsGetReq
    OSP_ADD_EVENT_DESC(PU_GPS_SS_GET_RSP);                       //��Ϣ��: CPuGpsSsGetRsp
    OSP_ADD_EVENT_DESC(PU_GPS_DATA_NTF_REQ);                     //��Ϣ��: CPuGpsDataNtyReq
    OSP_ADD_EVENT_DESC(PU_GPS_DATA_NTF_RSP);                     //��Ϣ��: CPuGpsDataNtyRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_SET_REQ);                //��Ϣ��: CPuTransdataCfgSetReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_SET_RSP);                //��Ϣ��: CPuTransdataCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_GET_REQ);                //��Ϣ��: CPuTransdataCfgGetReq
    OSP_ADD_EVENT_DESC(PU_TRANSDATA_CFG_GET_RSP);                //��Ϣ��: CPuTransdataCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_START_REQ);                     //��Ϣ��: CPuRecordStartReq
    OSP_ADD_EVENT_DESC(PU_RECORD_START_RSP);                     //��Ϣ��: CPuRecordStartRsp
    OSP_ADD_EVENT_DESC(PU_RECORED_STATUS_NTF_REQ);               //��Ϣ��: CPuRecordStatusNtfReq
    OSP_ADD_EVENT_DESC(PU_RECORED_STATUS_NTF_RSP);               //��Ϣ��: CPuRecordStatusNtfRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_STOP_REQ);                      //��Ϣ��: CPuRecordStopReq
    OSP_ADD_EVENT_DESC(PU_RECORD_STOP_RSP);                      //��Ϣ��: CPuRecordStopRsp
    OSP_ADD_EVENT_DESC(PU_RECORD_DEL_REQ);                       //��Ϣ��: CPuRecordDelReq
    OSP_ADD_EVENT_DESC(PU_RECORD_DEL_RSP);                       //��Ϣ��: CPuRecordDelRsp
    OSP_ADD_EVENT_DESC(PU_REC_QRY_REQ);                          //��Ϣ��: CPuRecQryReq
    OSP_ADD_EVENT_DESC(PU_REC_QRY_RSP);                          //��Ϣ��: CPuRecQryRsp
    OSP_ADD_EVENT_DESC(PU_REC_QRY_NTF_REQ);                      //��Ϣ��: CPuRecQryNtfReq
    OSP_ADD_EVENT_DESC(PU_REC_QRY_NTF_RSP);                      //��Ϣ��: CPuRecQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_REC_DAYS_QRY_REQ);                     //��Ϣ��: CPuRecDaysQryReq
    OSP_ADD_EVENT_DESC(PU_REC_DAYS_QRY_RSP);                     //��Ϣ��: CPuRecDaysQryRsp
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_SET_REQ);                    //��Ϣ��: CPuDecStyleSetReq
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_SET_RSP);                    //��Ϣ��: CPuDecStyleSetRsp
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_GET_REQ);                    //��Ϣ��: CPuDecStyleGetReq
    OSP_ADD_EVENT_DESC(PU_DEC_STYLE_GET_RSP);                    //��Ϣ��: CPuDecStyleGetRsp
    OSP_ADD_EVENT_DESC(PU_PIC_SNAPSHOT_REQ);                     //��Ϣ��: CPuPicSnapshotReq
    OSP_ADD_EVENT_DESC(PU_PIC_SNAPSHOT_RSP);                     //��Ϣ��: CPuPicSnapshotRsp
    OSP_ADD_EVENT_DESC(PU_PIC_DOWNLOAD_REQ);                     //��Ϣ��: CPuPicDownloadReq
    OSP_ADD_EVENT_DESC(PU_PIC_DOWNLOAD_RSP);                     //��Ϣ��: CPuPicDownloadRsp
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_REQ);                          //��Ϣ��: CPuPicQryReq
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_RSP);                          //��Ϣ��: CPuPicQryRsp
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_NTF_REQ);                      //��Ϣ��: CPuPicQryNtfReq
    OSP_ADD_EVENT_DESC(PU_PIC_QRY_NTF_RSP);                      //��Ϣ��: CPuPicQryNtfRsp
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_SET_REQ);                     //��Ϣ��: CPuLongOsdSetReq
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_SET_RSP);                     //��Ϣ��: CPuLongOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_GET_REQ);                     //��Ϣ��: CPuLongOsdGetReq
    OSP_ADD_EVENT_DESC(PU_LONG_OSD_GET_RSP);                     //��Ϣ��: CPuLongOsdGetRsp
    OSP_ADD_EVENT_DESC(PU_LOGO_BMPOSD_TRANSMIT_REQ);             //��Ϣ��: CPuLogoBmpOsdTransmitReq
    OSP_ADD_EVENT_DESC(PU_LOGO_BMPOSD_TRANSMIT_RSP);             //��Ϣ��: CPuLogoBmpOsdTransmitRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_REQ);                       //��Ϣ��: CUaRouterRegReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_RSP);                       //��Ϣ��: CUaRouterRegRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_REQ);                        //��Ϣ��: CUaRouterSsReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_RSP);                        //��Ϣ��: CUaRouterSsRsp
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_REQ);                       //��Ϣ��: CUaRouterNtfReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_RSP);                       //��Ϣ��: CUaRouterNtfRsp
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_SET_REQ);                    //��Ϣ��: CPuMultiOsdSetReq
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_SET_RSP);                    //��Ϣ��: CPuMultiOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_GET_REQ);                    //��Ϣ��: CPuMultiOsdGetReq
    OSP_ADD_EVENT_DESC(PU_MULTI_OSD_GET_RSP);                    //��Ϣ��: CPuMultiOsdGetRsp
    OSP_ADD_EVENT_DESC(DOMAIN_ALARM_REQ);                        //��Ϣ��: CDomainAlarmReq
    OSP_ADD_EVENT_DESC(DOMAIN_ALARM_RSP);                        //��Ϣ��: CDomainAlarmRsp
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_GET_REQ);                 //��Ϣ��: CPuSnapShotCfgGetReq 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_GET_RSP);                 //��Ϣ��: CPuSnapShotCfgGetRsp 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_SET_REQ);                 //��Ϣ��: CPuSnapShotCfgSetReq 
    OSP_ADD_EVENT_DESC(PU_SNAPSHOT_CFG_SET_RSP);                 //��Ϣ��: CPuSnapShotCfgSetRsp 
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_GET_REQ);              //��Ϣ��:CPuSerialParamCfgGetReq
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_GET_RSP);              //��Ϣ��:CPuSerialParamCfgGetRsp
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_SET_REQ);              //��Ϣ��:CPuSerialParamCfgSetReq
    OSP_ADD_EVENT_DESC(PU_SERIALPARAM_CFG_SET_RSP);              //��Ϣ��:CPuSerialParamCfgSetRsp
    OSP_ADD_EVENT_DESC(PU_TIME_SYNC_REQ);                        //��Ϣ�壺CPuSyncTimeReq
    OSP_ADD_EVENT_DESC(PU_TIME_SYNC_RSP);                        //��Ϣ�壺CPuSyncTimeRsp
    OSP_ADD_EVENT_DESC(PROBE_ONVIFPU_REQ);                       //��Ϣ��CProbeOnvifPuReq
    OSP_ADD_EVENT_DESC(PROBE_ONVIFPU_RSP);                       //��Ϣ��CProbeOnvifPuRsp
    OSP_ADD_EVENT_DESC(UPDATE_REG_NETSEG_REQ);                   //��Ϣ��: CUpdateRegNetSegReq
    OSP_ADD_EVENT_DESC(UPDATE_REG_NETSEG_RSP);                   //��Ϣ��: CUpdateRegNetSegRsp
    OSP_ADD_EVENT_DESC(UPDATE_CULOGIN_INFO_REQ);                 //��Ϣ��: CUpdateCuLoginInfoReq
    OSP_ADD_EVENT_DESC(UPDATE_CULOGIN_INFO_RSP);                 //��Ϣ��: CUpdateCuLoginInfoRsp
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_REQ);               //��Ϣ��: COffLineDevQryReq
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_RSP);               //��Ϣ��: COffLineDevQryRsp
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_NTF_REQ);           //��Ϣ��: COffLineDevQryNtfReq
    OSP_ADD_EVENT_DESC(CMU_PU_OFFLINEDEV_QRY_NTF_RSP);           //��Ϣ��: COffLineDevQryNtfRsp
    OSP_ADD_EVENT_DESC(RECORD_ON_TV_REQ);                        //��Ϣ��: CRecordOnTvReq
    OSP_ADD_EVENT_DESC(RECORD_ON_TV_RSP);                        //��Ϣ��: CRecordOnTvRsp
    OSP_ADD_EVENT_DESC(RECORD_OFF_TV_REQ);                       //��Ϣ��: CRecordOffTvReq
    OSP_ADD_EVENT_DESC(RECORD_OFF_TV_RSP);                       //��Ϣ��: CRecordOffTvRsp
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_REQ);                //��Ϣ��: CInviteDecoderCallReq
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_RSP);                //��Ϣ��: CInviteDecoderCallRsp
    OSP_ADD_EVENT_DESC(INVITE_DECORDER_CALL_ACK);                //��Ϣ��: CInviteDecoderCallAck
    OSP_ADD_EVENT_DESC(PU_VIDEOSRC_SET_REQ);                     //��Ϣ�壺CPuVideosrcSetReq
    OSP_ADD_EVENT_DESC(PU_VIDEOSRC_SET_RSP);                     //��Ϣ�壺CPuVideosrcSetRsp
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_SET_REQ);              //��Ϣ�壺CCfgSyncSwitchSetReq
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_SET_RSP);              //��Ϣ�壺CCfgSyncSwitchSetRsp
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_GET_REQ);              //��Ϣ�壺CCfgSyncSwitchGetReq
    OSP_ADD_EVENT_DESC(CONFIG_SYNC_SWITCH_GET_RSP);              //��Ϣ�壺CCfgSyncSwitchGetRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRCCFG_REFRESH_REQ);                //��Ϣ�壺CPuVidSrcCfgRefreshReq
    OSP_ADD_EVENT_DESC(PU_VIDSRCCFG_REFRESH_RSP);                //��Ϣ�壺CPuVidSrcCfgRefreshRsp
    OSP_ADD_EVENT_DESC(MODULE_ON_OFF_LOG_NTF_REQ);               //��Ϣ�壺CModuleOnOffLogNtfReq
    OSP_ADD_EVENT_DESC(MODULE_ON_OFF_LOG_NTF_RSP);               //��Ϣ�壺CModuleOnOffLogNtfRsp
    OSP_ADD_EVENT_DESC(ALARM_EVENT_NTF_REQ);                     //��Ϣ�壺CAlarmEventNtfReq
    OSP_ADD_EVENT_DESC(ALARM_EVENT_NTF_RSP);                     //��Ϣ�壺CAlarmEventNtfRsp
    OSP_ADD_EVENT_DESC(MAPPING_TABLE_NTF_REQ);                   //��Ϣ�壺CMappingTableNtfReq
    OSP_ADD_EVENT_DESC(MAPPING_TABLE_NTF_RSP);                   //��Ϣ�壺CMappingTableNtfRsp
    OSP_ADD_EVENT_DESC(DOMAIN_TABLE_NTF_REQ);                    //��Ϣ�壺CDomainTableNtfReq
    OSP_ADD_EVENT_DESC(DOMAIN_TABLE_NTF_RSP);                    //��Ϣ�壺CDomainTableNtfRsp
    OSP_ADD_EVENT_DESC(PU_ONLINE_NTF_REQ);                       //��Ϣ�壺CPuOnlineNtfReq
    OSP_ADD_EVENT_DESC(PU_ONLINE_NTF_RSP);                       //��Ϣ�壺CPuOnlineNtfRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_SET_REQ);                   //��Ϣ�壺CPuVidSrcOsdSetReq
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_SET_RSP);                   //��Ϣ�壺CPuVidSrcOsdSetRsp
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_GET_REQ);                   //��Ϣ�壺CPuVidSrcOsdGetReq
    OSP_ADD_EVENT_DESC(PU_VIDSRC_OSD_GET_RSP);                   //��Ϣ�壺CPuVidSrcOsdGetRsp
    OSP_ADD_EVENT_DESC(REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ);        //��Ϣ�壺CRealMediaSwitchOccurTypeReq
    OSP_ADD_EVENT_DESC(REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP);        //��Ϣ�壺CRealMediaSwitchOccurTypeRsp
    OSP_ADD_EVENT_DESC(ALARM_TYPE_GET_REQ);                      //��Ϣ�壺CGetAlarmTypesReq
    OSP_ADD_EVENT_DESC(ALARM_TYPE_GET_RSP);                      //��Ϣ�壺CGetAlarmTypesRsp
    OSP_ADD_EVENT_DESC(UPDATE_PU_VKEKS_NTF_REQ);                 //��Ϣ�壺CUpdatePuVkeksNtfReq
    OSP_ADD_EVENT_DESC(UPDATE_PU_VKEKS_NTF_RSP);                 //��Ϣ�壺CUpdatePuVkeksNtfRsp
    OSP_ADD_EVENT_DESC(PU_SIGNATURE_CTRL_REQ);                   //��Ϣ�壺CPuSignatrueCtrlReq
    OSP_ADD_EVENT_DESC(PU_SIGNATURE_CTRL_RSP);                   //��Ϣ�壺 CPuSignatrueCtrlRsp
    OSP_ADD_EVENT_DESC(PU_ENCRYPTION_CTRL_REQ);                  //��Ϣ�壺 CPuEncryptionCtrlReq
    OSP_ADD_EVENT_DESC(PU_ENCRYPTION_CTRL_RSP);                  //��Ϣ�壺 CPuEncryptionCtrlRsp
    OSP_ADD_EVENT_DESC(VTDU_REG_REQ);                            //VTDU��CMUע��������Ϣ��: CVtduRegReq
    OSP_ADD_EVENT_DESC(VTDU_REG_RSP);                            //CMU��VTDU��ע��Ӧ����Ϣ��: CVtduRegRsp
    OSP_ADD_EVENT_DESC(VTDU_ADDR_ASSIGN_REQ);                    //��VTDU����˿�������Ϣ��: CVtduAddrAssignReq
    OSP_ADD_EVENT_DESC(VTDU_ADDR_ASSIGN_RSP);                    //��VTDU����˿�Ӧ����Ϣ��: CVtduAddrAssignRsp
    OSP_ADD_EVENT_DESC(VTDU_ADDSWITCH_REQ);                      //��VTDU��ӽ���������Ϣ��: CVtduAddSwitchReq
    OSP_ADD_EVENT_DESC(VTDU_ADDSWITCH_RSP);                      //��VTDU��ӽ���Ӧ����Ϣ��: CVtduAddSwitchRsp
    OSP_ADD_EVENT_DESC(VTDU_REMOVESWITCH_REQ);                   //��VTDU���Ƴ�����������Ϣ��: ΪCVtduRemoveSwitchReq
    OSP_ADD_EVENT_DESC(VTDU_REMOVESWITCH_RSP);                   //��VTDU���Ƴ�����Ӧ����Ϣ��: CVtduRemoveSwitchRsp
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF_REQ);               //vtdu��Դ��Ŀ�Ķ��Ƿ�򶴳ɹ�����Ϣ��: CVtduPunchResultNtfReq
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF_RSP);               //vtdu��Դ��Ŀ�Ķ��Ƿ�򶴳ɹ�����Ϣ��: CVtduPunchResultNtfRsp
    OSP_ADD_EVENT_DESC(VTDU_QUERY_MAPPED_ADDR_NTF);              //vtdulib��ѯ������ַ֪ͨ��Osp��Ϣ��TVtduStunResult
    OSP_ADD_EVENT_DESC(VTDU_PUNCH_RESULT_NTF);                   //vtdulibPunch���֪ͨ��Osp��Ϣ��TVtduPunchResult
}


#endif  //#ifndef __CMU_EVENT_H__




