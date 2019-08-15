/*****************************************************************************
   ģ����      : CMSSDK
   �ļ���      : cmssdk_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cmssdk��Ϣ����
   ����        : fanxg
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/09/12  1.0         fanxg        ����
******************************************************************************/

#ifndef __CMS_SDK_EVENT_H__
#define __CMS_SDK_EVENT_H__


//�¼�����
enum ECMS_SDK_EVENT
{
    CMS_SDK_MSG_BEGIN = 27501,                        //CmsSdk��Ϣ������ʼ: CMSSDK_EVENT_BEGIN = 27501

    CMS_SDK_MSG_UNKNOWN = CMS_SDK_MSG_BEGIN+1,        //δ֪��Ϣ

    //��CMU�Ľ�����Ϣ��Ŀǰ�Ѿ�ʹ�õ�27761����һ�����õ�Ϊ27764��
    CMS_SDK_CMU_MSG_BEGIN                 = 27601,

	//cmssdk��ʼ����ɺ�Э��ջ�����Ϣ֪ͨ���ϲ㣬����ӿ�:USER<--SDK
	CMS_SDK_PROTOCOL_INFO_NTF             = 27602,    //cmssdkЭ��ջ��Ϣ֪ͨ��TCmsRealProtocolInfo

    //�����豸��¼CMU������ӿ�:USER<--SDK
    CMS_CMU_DISCONNECT_NTF                = 27603,    //CMU����֪ͨ��cmssdk���Լ���������
    CMS_CMU_CONNECT_NTF                   = 27604,    //CMU����֪ͨ��TConnInfo

	//���������֪ͨ������ӿ�:USER<--SDK
	CMS_SDK_BACKEND_CAPABILITIES_NTF      = 27745,    //���������֪ͨ��TCmsBackendCapabilities

    //ǰ���豸ע�ᣬ����ӿ�:USER-->SDK
    CMS_DEV_REG_REQ                       = 27605,    //ǰ���豸ע������CCmsDevRegReq
    CMS_DEV_REG_RSP                       = 27606,    //ǰ���豸ע��Ӧ��CCmsDevRegRsp

	//����cmu�Ķ���֪ͨ������ӿ�:USER<--SDK
	CMS_CMU_DISCONNECT_NTF_REQ            = 27607,	  //����cmu����֪ͨ����: CCmsCmuDisconnectNtfReq
	CMS_CMU_DISCONNECT_NTF_RSP            = 27608,	  //����cmu����֪ͨӦ��: CCmsCmuDisconnectNtfRsp

    //ǰ���豸ע��������ӿ�:USER-->SDK
	CMS_DEV_UNREG_REQ                     = 27609,     //ǰ���豸ע������CCmsDevUnRegReq
	CMS_DEV_UNREG_RSP                     = 27610,     //ǰ���豸ע��Ӧ��CCmsDevUnRegRsp

    //�豸״̬�����á��澯���֪ͨ������ӿ�:USER-->SDK
	CMS_DEV_OFFLINE_NTF                   = 27611,     //�豸����: CCmsDevOfflineNtf 
	CMS_DEV_ONLINE_NTF                    = 27612,     //�豸����: CCmsDevOnlineNtf 
	CMS_DEV_INFO_MOD_NTF                  = 27613,     //�豸��Ϣ�ı�: CCmsDevInfoModNtf   
	CMS_DEV_ALARM_NTF                     = 27614,     //�豸�澯�ϱ�: CCmsDevAlarmNtf

	//ǰ�˱���������á���ȡ��˫��ӿڣ�USER<-->SDK
	CMS_PU_VID_ENC_PARAM_SET_REQ          = 27615,		//����ǰ�˱����������CCmsPuVidEncParamSetReq
	CMS_PU_VID_ENC_PARAM_SET_RSP          = 27616,		//����ǰ�˱������Ӧ��CCmsPuVidEncParamSetRsp
	CMS_PU_VID_ENC_PARAM_GET_REQ          = 27617,		//��ȡǰ�˱����������CCmsPuVidEncParamGetReq
	CMS_PU_VID_ENC_PARAM_GET_RSP          = 27618,		//��ȡǰ�˱������Ӧ��CCmsPuVidEncParamGetRsp

    //�߼������������
    CMS_PU_VID_ENC_PARAM_SET_EX_REQ       = 27733,      //��Ϣ��: CCmsPuVidEncParamSetExReq
    CMS_PU_VID_ENC_PARAM_SET_EX_RSP       = 27734,      //��Ϣ��: CCmsPuVidEncParamSetExRsp
                                                      
    CMS_PU_VID_ENC_PARAM_GET_EX_REQ       = 27735,      //��Ϣ��: CCmsPuVidEncParamGetExReq
    CMS_PU_VID_ENC_PARAM_GET_EX_RSP       = 27736,      //��Ϣ��: CCmsPuVidEncParamGetExRsp

	//��Ƶ����������á���ȡ������ӿڣ�USER-->SDK
	CMS_PU_AUD_ENC_PARAM_SET_REQ          = 27711,		//��Ƶ���������������: CCmsPuAudEncParamSetReq
	CMS_PU_AUD_ENC_PARAM_SET_RSP          = 27712,		//��Ƶ�����������Ӧ��: CCmsPuAudEncParamSetRsp
	CMS_PU_AUD_ENC_PARAM_GET_REQ          = 27713,		//��Ƶ���������ȡ����: CCmsPuAudEncParamGetReq
	CMS_PU_AUD_ENC_PARAM_GET_RSP          = 27714,		//��Ƶ���������ȡӦ��: CCmsPuAudEncParamGetRsp

	//ǰ��͸���������ݣ�����ӿ�:USER-->SDK
	CMS_PU_TRANSPARENT_OPERATE_REQ        = 27619,		//͸���������ݷ�������CCmsPuTransparentOperateReq
	CMS_PU_TRANSPARENT_OPERATE_RSP        = 27620,		//͸���������ݷ���Ӧ��CCmsPuTransparentOperateRsp

	//͸���������õ����á���ȡ������ӿ�:USER-->SDK
	CMS_PU_TRANSDATA_CFG_SET_REQ          = 27715,		//͸���������õ���������: CCmsPuTransdataCfgSetReq
	CMS_PU_TRANSDATA_CFG_SET_RSP          = 27716,		//͸���������õ�����Ӧ��: CCmsPuTransdataCfgSetRsp
	CMS_PU_TRANSDATA_CFG_GET_REQ          = 27717,		//͸���������õĻ�ȡ����: CCmsPuTransdataCfgGetReq
	CMS_PU_TRANSDATA_CFG_GET_RSP          = 27718,		//͸���������õĻ�ȡӦ��: CCmsPuTransdataCfgGetRsp

    //����ʵʱ�����˫��ӿ�:USER<-->SDK
    CMS_REALMEDIA_INVITE_REQ              = 27621,      //ʵʱ�������CCmsRealMediaInviteReq
	CMS_REALMEDIA_INVITE_RSP              = 27622,      //ʵʱ���Ӧ��CCmsRealMediaInviteRsp
	CMS_REALMEDIA_INVITE_ACK              = 27623,      //ʵʱ���ȷ�ϣ�CCmsRealMediaInviteAck

    //���͹ؼ�֡������˫��ӿ�:USER<-->SDK
    CMS_KEYFREAM_SEND_REQ                 = 27624,       //ǿ�ƹؼ�֡����CCmsKeyFrameSendReq
    CMS_KEYFREAM_SEND_RSP                 = 27625,       //ǿ�ƹؼ�֡Ӧ��CCmsKeyFrameSendRsp

    //PTZ���ƣ�˫��ӿ�:USER<-->SDK
    CMS_PTZ_CTRL_REQ                      = 27626,        //PTZ��������CCmsPtzCtrlReq
    CMS_PTZ_CTRL_RSP                      = 27627,        //PTZ����Ӧ��CCmsPtzCtrlRsp

    //�ر�ʵʱ�����˫��ӿ�:USER<-->SDK
    CMS_REALMEDIA_BYE_REQ                 = 27628,        //ֹͣʵʱ�������CCmsRealMediaByeReq
    CMS_REALMEDIA_BYE_RSP                 = 27629,        //ֹͣʵʱ���Ӧ��CCmsRealMediaByeRsp

    //������ֹͣ��Ƶ���У�����ӿ�:USER-->SDK
	CMS_START_AUDIO_CALL_REQ              = 27630,		  //������Ƶ��������CCmsStartAudioCallReq
	CMS_START_AUDIO_CALL_RSP              = 27631,		  //������Ƶ����Ӧ��CCmsStartAudioCallRsp
	CMS_STOP_AUDIO_CALL_REQ               = 27632,		  //ֹͣ��Ƶ��������CCmsStopAudioCallReq
	CMS_STOP_AUDIO_CALL_RSP               = 27633,		  //ֹͣ��Ƶ����Ӧ��CCmsStopAudioCallRsp

	//���н������֪ͨ������ӿڣ�USER<--SDK
	CMS_AUDIO_CALL_DESTORY_NTF_REQ        = 27634,		  //���н������֪ͨ��CCmsAudioCallDestoryNtfReq
	CMS_AUDIO_CALL_DESTORY_NTF_RSP        = 27635,		  //���н������֪ͨӦ��CCmsAudioCallDestoryNtfRsp

	//������Ƶ���У�����ӿ�:USER<--SDK
    CMS_AUDIOCALL_INVITE_REQ              = 27636,        //��Ƶ��������CCmsAudioCallInviteReq
	CMS_AUDIOCALL_INVITE_RSP              = 27637,        //��Ƶ����Ӧ��CCmsAudioCallInviteRsp
	CMS_AUDIOCALL_INVITE_ACK              = 27638,        //��Ƶ����ȷ�ϣ�CCmsAudioCallInviteAck

    //�ر���Ƶ���У�����ӿ�:USER<--SDK
    CMS_AUDIOCALL_BYE_REQ                 = 27639,        //ֹͣ��Ƶ��������CCmsAudioCallByeReq
    CMS_AUDIOCALL_BYE_RSP                 = 27640,        //ֹͣ��Ƶ����Ӧ��CCmsAudioCallByeRsp

	//������Ƶ�������̣�Ϊ֧��GBS�����㲥��ӣ�
	//������ֹͣ��Ƶ���У�����ӿ�:USER-->SDK
	CMS_BROADCAST_START_AUDIO_CALL_REQ    = 27747,		  //�����㲥������Ƶ��������CCmsBroadcastStartAudioCallReq
	CMS_BROADCAST_START_AUDIO_CALL_RSP    = 27748,		  //�����㲥������Ƶ����Ӧ��CCmsBroadcastStartAudioCallRsp
	CMS_BROADCAST_STOP_AUDIO_CALL_REQ     = 27749,		  //�����㲥ֹͣ��Ƶ��������CCmsBroadcastStopAudioCallReq
	CMS_BROADCAST_STOP_AUDIO_CALL_RSP     = 27750,		  //�����㲥ֹͣ��Ƶ����Ӧ��CCmsBroadcastStopAudioCallRsp

	//���н������֪ͨ������ӿڣ�USER<--SDK��CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ��CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ��ʱ����ȥ����
	//CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_REQ = 27751,	  //�����㲥���н������֪ͨ��CCmsBroadcastAudioCallDestoryNtfReq
	//CMS_BROADCAST_AUDIO_CALL_DESTORY_NTF_RSP = 27752,	  //�����㲥���н������֪ͨӦ��CCmsBroadcastAudioCallDestoryNtfRsp

	//������Ƶ���У�����ӿ�:USER<--SDK
	CMS_BROADCAST_AUDIOCALL_INVITE_REQ    = 27753,        //�����㲥��Ƶ��������CCmsBroadcastAudioCallInviteReq
	CMS_BROADCAST_AUDIOCALL_INVITE_RSP    = 27754,        //�����㲥��Ƶ����Ӧ��CCmsBroadcastAudioCallInviteRsp
	CMS_BROADCAST_AUDIOCALL_INVITE_ACK    = 27755,        //�����㲥��Ƶ����ȷ�ϣ�CCmsBroadcastAudioCallInviteAck

	//�ر���Ƶ���У�����ӿ�:USER<--SDK
	CMS_BROADCAST_AUDIOCALL_BYE_REQ       = 27756,        //�����㲥ֹͣ��Ƶ��������CCmsBroadcastAudioCallByeReq
	CMS_BROADCAST_AUDIOCALL_BYE_RSP       = 27757,        //�����㲥ֹͣ��Ƶ����Ӧ��CCmsBroadcastAudioCallByeRsp

    //¼���ѯ��˫��ӿ�:USER<-->SDK
    //��ѯ��ʽ����ҳ��ѯ��һ�β�ѯ���֪ͨ��
	CMS_REC_QUERY_REQ                     = 27641,        //¼���ѯ����   CCmsRecQueryReq
	CMS_REC_QUERY_RSP                     = 27642,        //¼���ѯӦ��   CCmsRecQueryRsp
	CMS_REC_QUERY_NTF_REQ                 = 27643,        //¼���ѯ֪ͨ��   CCmsRecQueryNtfReq
	CMS_REC_QUERY_NTF_RSP                 = 27644,        //¼���ѯ֪ͨӦ��CCmsRecQueryNtfRsp

    //����¼��طţ�˫��ӿ�:USER<-->SDK
    //����������������ʧ�ܡ�
    CMS_REC_PLAY_INVITE_REQ               = 27645,        //¼��طſ�������CCmsRecPlayInviteReq
	CMS_REC_PLAY_INVITE_RSP               = 27646,        //¼��طſ���Ӧ��CCmsRecPlayInviteRsp
	CMS_REC_PLAY_INVITE_ACK               = 27647,        //¼��طſ���ȷ�ϣ�CCmsRecPlayInviteAck

    //�������֪ͨ��˫��ӿ�:USER<-->SDK
    CMS_REC_PLAY_PROG_NTF_REQ             = 27648,        //¼��طŽ����ϱ�����CCmsRecPlayProgNtfReq
	CMS_REC_PLAY_PROG_NTF_RSP             = 27649,        //¼��طŽ����ϱ�Ӧ��CCmsRecPlayProgNtfRsp
	CMS_REC_PLAY_END_NTF_REQ              = 27650,        //¼��طŽ���֪ͨ����CCmsRecPlayEndNtyReq
	CMS_REC_PLAY_END_NTF_RSP              = 27651,        //¼��طŽ���֪ͨӦ��CCmsRecPlayEndNtyRsp
	CMS_REC_PLAY_PROG_QRY_REQ             = 27652,        //¼��طŽ��Ȳ�ѯ����CCmsRecPlayProgQryReq
	CMS_REC_PLAY_PROG_QRY_RSP             = 27653,		  //¼��طŽ��Ȳ�ѯӦ��CCmsRecPlayProgQryRsp

    //����VCR���ƣ�˫��ӿ�:USER<-->SDK
    CMS_REC_PLAY_VCR_CTRL_REQ             = 27654,        //VCR��������CCmsRecPlayVcrCtrlReq
    CMS_REC_PLAY_VCR_CTRL_RSP             = 27655,        //VCR����Ӧ��CCmsRecPlayVcrCtrlRsp

    //ֹͣ¼��طţ�˫��ӿ�:USER<-->SDK
    CMS_REC_PLAY_BYE_REQ                  = 27656,        //¼��ط�ֹͣ����CCmsRecPlayByeReq
    CMS_REC_PLAY_BYE_RSP                  = 27657,        //¼��ط�ֹͣӦ��CCmsRecPlayByeRsp

    //����¼�����أ�˫��ӿ�:USER<-->SDK
    //����������������ʧ�ܡ�
    CMS_REC_DOWNLOAD_INVITE_REQ           = 27658,        //��ʼ¼����������CCmsRecDownloadInviteReq
    CMS_REC_DOWNLOAD_INVITE_RSP           = 27659,        //��ʼ¼������Ӧ��CCmsRecDownloadInviteRsp
    CMS_REC_DOWNLOAD_INVITE_ACK           = 27660,        //��ʼ¼������ȷ�ϣ�CCmsRecDownloadInviteAck

	//���ؽ���֪ͨ��˫��ӿ�:USER<-->SDK
	CMS_REC_DOWNLOAD_END_NTF_REQ          = 27661,        //¼�����ؽ���֪ͨ����CCmsRecDownloadEndNtyReq
	CMS_REC_DOWNLOAD_END_NTF_RSP          = 27662,        //¼�����ؽ���֪ͨӦ��CCmsRecDownloadEndNtyRsp

    //ֹͣ¼�����أ�˫��ӿ�:USER<-->SDK
    CMS_REC_DOWNLOAD_BYE_REQ              = 27663,         //¼������ֹͣ����CCmsRecDownloadByeReq
    CMS_REC_DOWNLOAD_BYE_RSP              = 27664,         //¼������ֹͣӦ��CCmsRecDownloadByeRsp

    //Ŀ¼���ģ�����ӿ�:USER-->SDK
    CMS_CATALOG_STATUS_SS_REQ             = 27665,         //Ŀ¼��������: CCmsCatalogStatusSsReq
    CMS_CATALOG_STATUS_SS_RSP             = 27666,         //Ŀ¼����Ӧ��: CCmsCatalogStatusSsRsp

    //Ŀ¼����ˢ�£�����ӿ�:USER-->SDK
    CMS_CATALOG_STATUS_REFRESH_SS_REQ     = 27667,        //Ŀ¼����ˢ������: CCmsCatalogStatusRefreshSsReq
    CMS_CATALOG_STATUS_REFRESH_SS_RSP     = 27668,        //Ŀ¼����ˢ��Ӧ��: CCmsCatalogStatusRefreshSsRsp

    //Ŀ¼����ȡ��������ӿ�:USER-->SDK
    CMS_CATALOG_STATUS_UNSS_REQ           = 27669,        //Ŀ¼����ȡ������: CCmsCatalogStatusUnSsReq
    CMS_CATALOG_STATUS_UNSS_RSP           = 27670,        //Ŀ¼����ȡ��Ӧ��: CCmsCatalogStatusUnSsRsp

    //�豸�б��ģ�����ӿ�:USER-->SDK
    CMS_PULIST_STATUS_SS_REQ              = 27671,        //�豸�б�������: CCmsPuListStatusSsReq
    CMS_PULIST_STATUS_SS_RSP              = 27672,        //�豸�б���Ӧ��: CCmsPuListStatusSsRsp

    //�豸�б���ˢ�£�����ӿ�:USER-->SDK
    CMS_PULIST_STATUS_REFRESH_SS_REQ      = 27673,        //�豸�б���ˢ������: CCmsPuListStatusRefreshSsReq
    CMS_PULIST_STATUS_REFRESH_SS_RSP      = 27674,        //�豸�б���ˢ��Ӧ��: CCmsPuListStatusRefreshSsRsp

    //�豸�б���ȡ��������ӿ�:USER-->SDK
    CMS_PULIST_STATUS_UNSS_REQ            = 27675,         //�豸�б���ȡ������: CCmsPuListStatusUnSsReq
    CMS_PULIST_STATUS_UNSS_RSP            = 27676,         //�豸�б���ȡ��Ӧ��: CCmsPuListStatusUnSsRsp

    //�豸���ģ�����ӿ�:USER-->SDK
    CMS_PU_STATUS_SS_REQ                  = 27677,          //�豸��������: CCmsPuStatusSsReq
    CMS_PU_STATUS_SS_RSP                  = 27678,          //�豸����Ӧ��: CCmsPuStatusSsRsp

    //�豸����ˢ�£�����ӿ�:USER-->SDK
    CMS_PU_STATUS_REFRESH_SS_REQ          = 27679,          //�豸����ˢ������: CCmsPuStatusRefreshSsReq
    CMS_PU_STATUS_REFRESH_SS_RSP          = 27680,          //�豸����ˢ��Ӧ��: CCmsPuStatusRefreshSsRsp

    //�豸����ȡ��������ӿ�:USER-->SDK
    CMS_PU_STATUS_UNSS_REQ                = 27681,          //�豸����ȡ������: CCmsPuStatusUnSsReq
    CMS_PU_STATUS_UNSS_RSP                = 27682,          //�豸����ȡ��Ӧ��: CCmsPuStatusUnSsRsp

    //�豸����״̬֪ͨ������ӿ�:USER<--SDK
    CMS_PU_STATUS_NTF_REQ                 = 27683,          //�豸������֪ͨ����: CCmsPuStatusNtyReq
    CMS_PU_STATUS_NTF_RSP                 = 27684,          //�豸������֪ͨӦ��: CCmsPuStatusNtyRsp

    //�豸������Ϣ֪ͨ������ӿ�:USER<--SDK
    CMS_PU_CONFIG_NTF_REQ                 = 27685,          //�豸������Ϣ֪ͨ����: CCmsPuConfigNtyReq
    CMS_PU_CONFIG_NTF_RSP                 = 27686,          //�豸������Ϣ֪ͨӦ��: CCmsPuConfigNtyRsp

    //�豸����״̬֪ͨ������ӿ�:USER<--SDK
    CMS_PU_SERVICE_STATUS_NTF_REQ         = 27687,          //�豸����״̬֪ͨ����: CCmsPuServiceStatusNtyReq
    CMS_PU_SERVICE_STATUS_NTF_RSP         = 27688,          //�豸����״̬֪ͨӦ��: CCmsPuServiceStatusNtyRsp

    //�豸�澯״̬֪ͨ������ӿ�:USER<--SDK
    CMS_PU_ALARM_STATUS_NTF_REQ           = 27689,          //�豸�澯״̬֪ͨ����: CCmsPuAlarmStatusNtyReq
    CMS_PU_ALARM_STATUS_NTF_RSP           = 27690,          //�豸�澯״̬֪ͨӦ��: CCmsPuAlarmStatusNtyRsp

	//͸������״̬֪ͨ������ӿ�:USER<--SDK
	CMS_PU_TRANSDATA_STATUS_NTF_REQ       = 27691,          //�豸��������֪ͨ����: CCmsPuTransDataStatusNtyReq
	CMS_PU_TRANSDATA_STATUS_NTF_RSP       = 27692,          //�豸��������֪ͨӦ��: CCmsPuTransDataStatusNtyRsp

    //GPS״̬֪ͨ������ӿ�:USER<--SDK
    CMS_PU_GPS_STATUS_NTF_REQ             = 27693,          //�豸GPS״̬֪ͨ����: CCmsPuGpsStatusNtyReq
    CMS_PU_GPS_STATUS_NTF_RSP             = 27694,          //�豸GPS״̬֪ͨӦ��: CCmsPuGpsStatusNtyRsp

	//����֪ͨ������ӿ�:USER<--SDK
	CMS_PU_BATCH_NTF_REQ                  = 27760,			// ���ĵ�����֪ͨ����CCmsPuBatchNtyReq
	CMS_PU_BATCH_NTF_RSP                  =	27761,			// ���ĵ�����֪ͨӦ��CCmsPuBatchNtyRsp

    //����/�ر�GPS��Ϣ�ϱ�������ӿڣ�USER<--SDK
    CMS_SUBSCRIBE_GPS_REQ                 = 27695,          //����/�ر�GPS��Ϣ�ϱ�����CCmsSubscribeGpsReq
    CMS_SUBSCRIBE_GPS_RSP                 = 27696,          //����/�ر�GPS��Ϣ�ϱ�Ӧ��CCmsSubscribeGpsRsp
    CMS_DEV_GPS_DATA_NTF                  = 27697,          //�豸gps��Ϣ�ı�֪ͨ:CCmsDevGpsDataNty

	//����ͷ�������ã�����ӿڣ�USER<-->SDK
	CMS_PU_CAMERA_TYPE_SET_REQ            = 27698,			//����ͷ������������: CCmsPuCameraTypeSetReq
	CMS_PU_CAMERA_TYPE_SET_RSP            = 27699,			//����ͷ��������Ӧ��: CCmsPuCameraTypeSetRsp

    //�ƶ����
    CMS_PU_MOTION_DETECT_SET_REQ          = 27719,          //��Ϣ��: CCmsPuMotionDetectSetReq
    CMS_PU_MOTION_DETECT_SET_RSP          = 27720,          //��Ϣ��: CCmsPuMotionDetectSetRsp

    CMS_PU_MOTION_DETECT_GET_REQ          = 27721,          //��Ϣ��: CCmsPuMotionDetectGetReq
    CMS_PU_MOTION_DETECT_GET_RSP          = 27722,          //��Ϣ��: CCmsPuMotionDetectGetRsp

    //ͼ���ڱ�
    CMS_PU_AREA_SHADE_SET_REQ             = 27723,          //��Ϣ��: CCmsPuAreaShadeSetReq
    CMS_PU_AREA_SHADE_SET_RSP             = 27724,          //��Ϣ��: CCmsPuAreaShadeSetRsp
                                                     
    CMS_PU_AREA_SHADE_GET_REQ             = 27725,          //��Ϣ��: CCmsPuAreaShadeGetReq
    CMS_PU_AREA_SHADE_GET_RSP             = 27726,          //��Ϣ��: CCmsPuAreaShadeGetRsp

	//ͼƬץ�ģ�����ӿڣ�USER-->SDK
	CMS_PU_PIC_SNAPSHOT_REQ               = 27700,			//ͼƬץ������: CCmsPuPicSnapshotReq
	CMS_PU_PIC_SNAPSHOT_RSP               = 27701,			//ͼƬץ��Ӧ��: CCmsPuPicSnapshotRsp

	//ǰ��¼��������ѯ������ӿڣ�USER-->SDK
	CMS_PU_REC_DAYS_QRY_REQ               = 27702,			//ǰ��¼��������ѯ����: CCmsPuRecDaysQryReq
	CMS_PU_REC_DAYS_QRY_RSP               = 27703,			//ǰ��¼��������ѯӦ��: CCmsPuRecDaysQryRsp

	//ǰ�˳���Ļ���á���ȡ������ӿڣ�USER-->SDK
	CMS_PU_LONG_OSD_SET_REQ               = 27727,			//ǰ�˳���Ļ��������: CCmsPuLongOsdSetReq
	CMS_PU_LONG_OSD_SET_RSP               = 27728,			//ǰ�˳���Ļ����Ӧ��: CCmsPuLongOsdSetRsp
	CMS_PU_LONG_OSD_GET_REQ               = 27729,			//ǰ�˳���Ļ��ȡ����: CCmsPuLongOsdGetReq
	CMS_PU_LONG_OSD_GET_RSP               = 27730,			//ǰ�˳���Ļ��ȡӦ��: CCmsPuLongOsdGetRsp

	//����̨�꣬����ӿڣ�USER-->SDK
	CMS_PU_LOGO_BMPOSD_TRANSMIT_REQ       = 27731,			//̨�괫������: CCmsPuLogoBmpOsdTransmitReq
	CMS_PU_LOGO_BMPOSD_TRANSMIT_RSP       = 27732,			//̨�괫��Ӧ��: CCmsPuLogoBmpOsdTransmitRsp

    //ǰ��¼������ֹͣ������ӿڣ�USER-->SDK
    CMS_PU_RECORD_START_REQ               = 27704,          //��Ϣ��: CCmsPuRecordStartReq
    CMS_PU_RECORD_START_RSP               = 27705,          //��Ϣ��: CCmsPuRecordStartRsp
    
    CMS_PU_RECORD_STOP_REQ                = 27706,          //��Ϣ��: CCmsPuRecordStopReq
    CMS_PU_RECORD_STOP_RSP                = 27707,          //��Ϣ��: CCmsPuRecordStopRsp

    //ǰ��¼��ɾ��������ӿڣ�USER-->SDK
    CMS_PU_RECORD_DEL_REQ                 = 27708,          //��Ϣ��: CCmsPuRecordDelReq
    CMS_PU_RECORD_DEL_RSP                 = 27709,          //��Ϣ��: CCmsPuRecordDelRsp

	//ǰ��ץ�Ĳ�������
	CMS_PU_SNAPSHOT_CFG_GET_REQ           = 27737,          //��Ϣ��: CCmsPuSnapShotCfgGetReq 
	CMS_PU_SNAPSHOT_CFG_GET_RSP           = 27738,          //��Ϣ��: CCmsPuSnapShotCfgGetRsp 
	CMS_PU_SNAPSHOT_CFG_SET_REQ           = 27739,          //��Ϣ��: CCmsPuSnapShotCfgSetReq 
	CMS_PU_SNAPSHOT_CFG_SET_RSP           = 27740,          //��Ϣ��: CCmsPuSnapShotCfgSetRsp 
	//ǰ�˴��ڲ�������
	CMS_PU_SERIALPARAM_CFG_GET_REQ        = 27741,          //��Ϣ�壺CCmsPuSerialParamCfgGetReq
	CMS_PU_SERIALPARAM_CFG_GET_RSP        = 27742,          //��Ϣ�壺CCmsPuSerialParamCfgGetRsp
	CMS_PU_SERIALPARAM_CFG_SET_REQ        = 27743,          //��Ϣ�壺CCmsPuSerialParamCfgSetReq
	CMS_PU_SERIALPARAM_CFG_SET_RSP        = 27744,          //��Ϣ�壺CCmsPuSerialParamCfgSetRsp

	
	CMS_PU_TRANSPARENT_DATA_NTF_REQ        = 27745,          //��Ϣ�壺CCmsPuTransparentDataNtfReq
	CMS_PU_TRANSPARENT_DATA_NTF_RSP        = 27746,          //��Ϣ�壺CCmsPuTransparentDataNtfRsp

	CMS_CMU_DISCONNECT_PU_NTF              = 27758,			 //��Ϣ��: CCmsCmuDisconnectPuNtf

	CMS_MODULE_ON_OFF_LOG_NTF              = 27759,			 //��Ϣ��: CCmsModuleOnOffLogNtf

    //�ֶ�����������ӿ�:USER<-->SDK
    CMS_PU_ALARM_RESET_REQ                 = 27762,          //��Ϣ�壺CCmsPuAlarmResetReq
    CMS_PU_ALARM_RESET_RSP                 = 27763,          //��Ϣ�壺CCmsPuAlarmResetRsp

    CMS_SDK_CMU_MSG_END                   = 27799,

    //��TAS�Ľ�����Ϣ
    CMS_SDK_3AS_MSG_BEGIN				  = 27801,

    //�����豸��¼3as������ӿ�:USER<--SDK
    CMS_3AS_DISCONNECT_NTF                = 27802,          //3AS����֪ͨ��cmssdk���Լ���������
    CMS_3AS_CONNECT_NTF                   = 27803,          //3AS����֪ͨ��TConnInfo

    // �豸����ɾ���ġ��飬����ӿ�:USER-->SDK
	CMS_3AS_DEVICE_ADD_REQ                = 27804,          //�豸��������CCmsDeviceAddReq
	CMS_3AS_DEVICE_ADD_RSP                = 27805,          //�豸����Ӧ��CCmsDeviceAddRsp
	CMS_3AS_DEVICE_ADD_NTF_REQ            = 27806,          //�豸����֪ͨ��CCmsDeviceAddNtfReq
	CMS_3AS_DEVICE_ADD_NTF_RSP            = 27807,          //�豸����֪ͨӦ��CCmsDeviceAddNtfRsp
	CMS_3AS_DEVICE_DEL_REQ                = 27808,          //�豸��������CCmsDeviceDelReq
	CMS_3AS_DEVICE_DEL_RSP                = 27809,          //�豸����Ӧ��CCmsDeviceDelRsp
	CMS_3AS_DEVICE_DEL_NTF_REQ            = 27810,          //�豸����֪ͨ��CCmsDeviceDelNtfReq
	CMS_3AS_DEVICE_DEL_NTF_RSP            = 27811,          //�豸����֪ͨӦ��CCmsDeviceDelNtfRsp
	CMS_3AS_DEVICE_MOD_REQ                = 27812,          //�豸�޸�����CCmsDeviceModReq
	CMS_3AS_DEVICE_MOD_RSP                = 27813,          //�豸�޸�Ӧ��CCmsDeviceModRsp
	CMS_3AS_DEVICE_MOD_NTF_REQ            = 27814,          //�豸�޸�֪ͨ��CCmsDeviceModNtfReq
	CMS_3AS_DEVICE_MOD_NTF_RSP            = 27815,          //�豸�޸�֪ͨӦ��CCmsDeviceModNtfRsp
	CMS_3AS_DEVICE_QRY_REQ                = 27816,          //�豸��ѯ����CCmsDeviceQryReq
	CMS_3AS_DEVICE_QRY_RSP                = 27817,          //�豸��ѯӦ��CCmsDeviceQryRsp
	CMS_3AS_DEVICE_QRY_NTF_REQ            = 27818,          //�豸��ѯ֪ͨ��CCmsDeviceQryNtfReq
	CMS_3AS_DEVICE_QRY_NTF_RSP            = 27819,          //�豸��ѯ֪ͨӦ��CCmsDeviceQryNtfRsp

    // �豸������ɾ���ġ��飬����ӿ�:USER-->SDK
    CMS_3AS_DEVICE_GROUP_ADD_REQ          = 27820,          //�豸���������CCmsDeviceGroupAddReq
	CMS_3AS_DEVICE_GROUP_ADD_RSP          = 27821,          //�豸�����Ӧ��CCmsDeviceGroupAddRsp
	CMS_3AS_DEVICE_GROUP_ADD_NTF_REQ      = 27822,          //�豸�����֪ͨ��CCmsDeviceGroupAddNtfReq
	CMS_3AS_DEVICE_GROUP_ADD_NTF_RSP      = 27823,          //�豸�����֪ͨӦ��CCmsDeviceGroupAddNtfRsp
	CMS_3AS_DEVICE_GROUP_DEL_REQ          = 27824,          //�豸��ɾ������CCmsDeviceGroupDelReq
	CMS_3AS_DEVICE_GROUP_DEL_RSP          = 27825,          //�豸��ɾ��Ӧ��CCmsDeviceGroupDelRsp
	CMS_3AS_DEVICE_GROUP_DEL_NTF_REQ      = 27826,          //�豸�����֪ͨ��CCmsDeviceGroupDelNtfReq
	CMS_3AS_DEVICE_GROUP_DEL_NTF_RSP      = 27827,          //�豸�����֪ͨӦ��CCmsDeviceGroupDelNtfRsp
	CMS_3AS_DEVICE_GROUP_MOD_REQ          = 27828,          //�豸���޸�����CCmsDeviceGroupModReq
	CMS_3AS_DEVICE_GROUP_MOD_RSP          = 27829,          //�豸���޸�Ӧ��CCmsDeviceGroupModRsp
	CMS_3AS_DEVICE_GROUP_MOD_NTF_REQ      = 27830,          //�豸�����֪ͨ��CCmsDeviceGroupModNtfReq
	CMS_3AS_DEVICE_GROUP_MOD_NTF_RSP      = 27831,          //�豸�����֪ͨӦ��CCmsDeviceGroupModNtfRsp
	CMS_3AS_DEVICE_GROUP_QRY_REQ          = 27832,          //�豸���ѯ����CCmsDeviceGroupQryReq
	CMS_3AS_DEVICE_GROUP_QRY_RSP          = 27833,          //�豸���ѯӦ��CCmsDeviceGroupQryRsp
	CMS_3AS_DEVICE_GROUP_QRY_NTF_REQ      = 27834,          //�豸���ѯ֪ͨ��CCmsDeviceGroupQryNtfReq
	CMS_3AS_DEVICE_GROUP_QRY_NTF_RSP      = 27835,          //�豸���ѯ֪ͨӦ��CCmsDeviceGroupQryNtfRsp

    // �豸���顢ɾ������ѯ������ӿ�:USER-->SDK
    CMS_3AS_DEVICE_GROUP_DEVICE_ADD_REQ     = 27836,        //�豸���豸��������CCmsDeviceGroupDeviceAddReq
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_RSP     = 27837,        //�豸���豸����Ӧ��CCmsDeviceGroupDeviceAddRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ = 27838,        //�豸���豸����֪ͨ��CCmsDeviceGroupDeviceAddNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP = 27839,        //�豸���豸����֪ͨӦ��CCmsDeviceGroupDeviceAddNtfRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_REQ     = 27840,        //�豸���豸��������CCmsDeviceGroupDeviceDelReq
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_RSP     = 27841,        //�豸���豸����Ӧ��CCmsDeviceGroupDeviceDelRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ = 27842,        //�豸���豸����֪ͨ��CCmsDeviceGroupDeviceDelNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP = 27843,        //�豸���豸����֪ͨӦ��CCmsDeviceGroupDeviceDelNtfRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_REQ     = 27844,        //�豸���µ��豸��ѯ����CCmsDeviceGroupDeviceQryReq
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_RSP     = 27845,        //�豸���µ��豸��ѯӦ��CCmsDeviceGroupDeviceQryRsp
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ = 27846,        //�豸���µ��豸��ѯ֪ͨ��CCmsDeviceGroupDeviceQryNtfReq
	CMS_3AS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP = 27847,        //�豸���µ��豸��ѯ֪ͨӦ��CCmsDeviceGroupDeviceQryNtfRsp

	// ��ƵԴ������ȡ������ӿ�:USER-->SDK
	CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_REQ   = 27848,		//��ƵԴ������ȡ����CCmsDeviceCapIndexNameGetReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_GET_RSP   = 27849,		//��ƵԴ������ȡӦ��CCmsDeviceCapIndexNameGetRsp

	// ��ƵԴ�������ã�����ӿ�:USER-->SDK
	CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_REQ   = 27850,		//��ƵԴ������������CCmsDeviceCapIndexNameSetReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_SET_RSP   = 27851,		//��ƵԴ��������Ӧ��CCmsDeviceCapIndexNameSetRsp
	CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ = 27852,		//�豸���������޸�֪ͨ��CCmsDeviceCapIndexNameModNtfReq
	CMS_3AS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP = 27853,		//�豸���������޸�֪ͨӦ��CCmsDeviceCapIndexNameModNtfRsp

    // ��ƵԴ������ѯ������ӿ�:USER-->SDK
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_REQ  = 27900,         //��ƵԴ������ѯ���� CCmsDeviceCapIndexNameQryReq
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_RSP  = 27901,         //��ƵԴ������ѯӦ�� CCmsDeviceCapIndexNameQryRsp
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ = 27902,      //��ƵԴ������ѯ֪ͨ�� CCmsDeviceCapIndexNameQryNtfReq
    CMS_3AS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP = 27903,      //��ƵԴ������ѯ֪ͨӦ��CCmsDeviceCapIndexNameQryNtfRsp

	// ���ϵ��ѯ������ӿ�:USER-->SDK
	CMS_3AS_DOMAIN_RELATION_QRY_REQ        = 27854,			//���ϵ��ѯ����CCmsDomainRelationQryReq
	CMS_3AS_DOMAIN_RELATION_QRY_RSP        = 27855,			//���ϵ��ѯӦ��CCmsDomainRelationQryRsp
	CMS_3AS_DOMAIN_RELATION_QRY_NTF_REQ    = 27856,			//���ϵ��ѯ֪ͨ��CCmsDomainRelationQryNtfReq
	CMS_3AS_DOMAIN_RELATION_QRY_NTF_RSP    = 27857,			//���ϵ��ѯ֪ͨӦ��CCmsDomainRelationQryNtfRsp

	// ���ϵ���֪ͨ������ӿ�:USER<--SDK
    CMS_3AS_DOMAIN_RELATION_ADD_NTF_REQ    = 27858,         //���ϵ���֪ͨ��CCmsDomainRelationAddNtfReq
	CMS_3AS_DOMAIN_RELATION_ADD_NTF_RSP    = 27859,         //���ϵ���֪ͨӦ��CCmsDomainRelationAddNtfRsp
	CMS_3AS_DOMAIN_RELATION_DEL_NTF_REQ    = 27860,         //���ϵɾ��֪ͨ��CCmsDomainRelationDelNtfReq
	CMS_3AS_DOMAIN_RELATION_DEL_NTF_RSP    = 27861,         //���ϵɾ��֪ͨӦ��CCmsDomainRelationDelNtfRsp
	CMS_3AS_DOMAIN_RELATION_MOD_NTF_REQ    = 27862,         //���ϵ�޸�֪ͨ��CCmsDomainRelationModNtfReq
	CMS_3AS_DOMAIN_RELATION_MOD_NTF_RSP    = 27863,         //���ϵ�޸�֪ͨӦ��CCmsDomainRelationModNtfRsp

	// ����ǽ����ɾ���ġ��飬����ӿ�:USER->SDK
	CMS_3AS_TVWALL_ADD_REQ                 = 27864,			// ����ǽ�������CCmsTvwallAddReq 
	CMS_3AS_TVWALL_ADD_RSP                 = 27865,			// ����ǽ���Ӧ��CCmsTvwallAddRsp
	CMS_3AS_TVWALL_ADD_NTF_REQ             = 27866,			// ����ǽ���֪ͨ��CCmsTvwallAddNtfReq
	CMS_3AS_TVWALL_ADD_NTF_RSP             = 27867,			// ����ǽ���֪ͨӦ��CCmsTvwallAddNtfRsp
	CMS_3AS_TVWALL_DEL_REQ                 = 27868,			// ����ǽɾ������CCmsTvwallDelReq
	CMS_3AS_TVWALL_DEL_RSP                 = 27869,			// ����ǽɾ��Ӧ��CCmsTvwallDelRsp
	CMS_3AS_TVWALL_DEL_NTF_REQ             = 27870,			// ����ǽɾ��֪ͨ��CCmsTvwallDelNtfReq
	CMS_3AS_TVWALL_DEL_NTF_RSP             = 27871,			// ����ǽɾ��֪ͨӦ��CCmsTvwallDelNtfRsp
	CMS_3AS_TVWALL_MOD_REQ                 = 27872,			// ����ǽ�޸�����CCmsTvwallModReq
	CMS_3AS_TVWALL_MOD_RSP                 = 27873,			// ����ǽ�޸�Ӧ��CCmsTvwallModRsp
	CMS_3AS_TVWALL_MOD_NTF_REQ             = 27874,			// ����ǽ�޸�֪ͨ��CCmsTvwallModNtfReq
	CMS_3AS_TVWALL_MOD_NTF_RSP             = 27875,			// ����ǽ�޸�֪ͨӦ��CCmsTvwallModNtfRsp
	CMS_3AS_TVWALL_QRY_REQ                 = 27876,			// ����ǽ��ѯ����CCmsTvwallQryReq
	CMS_3AS_TVWALL_QRY_RSP                 = 27877,			// ����ǽ��ѯӦ��CCmsTvwallQryRsp
	CMS_3AS_TVWALL_QRY_NTF_REQ             = 27878,			// ����ǽ��ѯ֪ͨ��CCmsTvwallQryNtfReq
	CMS_3AS_TVWALL_QRY_NTF_RSP             = 27879,			// ����ǽ��ѯ֪ͨӦ��CCmsTvwallQryNtfRsp

	//�û��б��ѯ
	CMS_3AS_CUSER_QRY_REQ                  = 27880,			//�û��б��ѯ����CCmsUserQryReq
	CMS_3AS_CUSER_QRY_RSP                  = 27881,			//�û��б��ѯӦ��CCmsUserQryRsp
	CMS_3AS_CUSER_QRY_NTF_REQ              = 27882,			//�û��б��ѯ֪ͨ��CCmsUserQryNtfReq
	CMS_3AS_CUSER_QRY_NTF_RSP              = 27883,			//�û��б��ѯ֪ͨӦ��CCmsUserQryNtfRsp

	//��ѯ�û��豸
	CMS_3AS_CUSER_DEVICE_QRY_REQ           = 27884,			//�û��豸��ѯ����CCmsUserDeviceQryReq
	CMS_3AS_CUSER_DEVICE_QRY_RSP           = 27885,			//�û��豸��ѯӦ��CCmsUserDeviceQryRsp
	CMS_3AS_CUSER_DEVICE_QRY_NTF_REQ       = 27886,			//�û��豸��ѯ֪ͨ��CCmsUserDeviceQryNtfReq
	CMS_3AS_CUSER_DEVICE_QRY_NTF_RSP       = 27887,			//�û��豸��ѯ֪ͨӦ��CCmsUserDeviceQryNtfRsp

	//cu����������
	CMS_3AS_CU_ONLINE_REQ                  = 27888,			//cu��������CCmsCuOnlineReq
	CMS_3AS_CU_ONLINE_RSP                  = 27889,			//cu����Ӧ��CCmsCuOnlineRsp
	CMS_3AS_CU_OFFLINE_REQ                 = 27890,			//cu��������CCmsCuOfflineReq
	CMS_3AS_CU_OFFLINE_RSP                 = 27891,			//cu����Ӧ��CCmsCuOfflineRsp

    CMS_3AS_CUSER_ADD_NTF_REQ              = 27892,         //��Ϣ�� CCmsUserAddNtfReq
    CMS_3AS_CUSER_ADD_NTF_RSP              = 27893,         //��Ϣ�� CCmsUserAddNtfRsp

    CMS_3AS_CUSER_DEL_NTF_REQ              = 27894,         //��Ϣ�� CCmsUserDelNtfReq
    CMS_3AS_CUSER_DEL_NTF_RSP              = 27895,         //��Ϣ�� CCmsUserDelNtfRsp

    CMS_3AS_CUSER_MOD_NTF_REQ              = 27896,         //��Ϣ�� CCmsUserModNtfReq
    CMS_3AS_CUSER_MOD_NTF_RSP              = 27897,         //��Ϣ�� CCmsUserModNtfRsp

    CMS_3AS_GATEWAY_DEL_REQ                = 27898,         // ��Ϣ�� CCmsGatewayDelReq
    CMS_3AS_GATEWAY_DEL_RSP                = 27899,         // ��Ϣ�� CCmsGatewayDelRsp

    CMS_SDK_3AS_MSG_END                    = 27999,

    //��RCS�Ľ�����Ϣ
    CMS_SDK_RCS_MSG_BEGIN				   = 28001,

    //�����豸��¼rcs������ӿ�:USER<--SDK
	CMS_RCS_DISCONNECT_NTF                 = 28002,          //RCS����֪ͨ��cmssdk���Լ���������
	CMS_RCS_CONNECT_NTF                    = 28003,          //RCS����֪ͨ��TConnInfo

	//rcs¼��״̬���ġ�ȡ�����ģ�����ӿ�:USER-->SDK
	CMS_RCS_SUBSCRIBE_PU_STATUS_REQ        = 28004,			//¼��״̬��������CCmsRcsSubscribePuStatusReq
	CMS_RCS_SUBSCRIBE_PU_STATUS_RSP        = 28005,			//¼��״̬����Ӧ��CCmsRcsSubscribePuStatusRsp
	CMS_RCS_PU_RECORD_STATUS_NTF_REQ       = 28006,			//¼��״̬����֪ͨ��CCmsRcsPuRecordStatusNtyReq
	CMS_RCS_PU_RECORD_STATUS_NTF_RSP       = 28007,			//¼��״̬����֪ͨӦ��CCmsRcsPuRecordStatusNtyRsp
	CMS_RCS_UNSUBSCRIBE_PU_STATUS_REQ      = 28008,			//ȡ��¼��״̬��������CCmsRcsUnsubscribePuStatusReq
	CMS_RCS_UNSUBSCRIBE_PU_STATUS_RSP      = 28009,			//ȡ��¼��״̬����Ӧ��CCmsRcsUnsubscribePuStatusRsp

	//��ȡ¼��ϵͳ����������ӿڣ�USER-->SDK
	CMS_RCS_GET_SYSPARAM_REQ                = 28010,		//��ȡ¼��ϵͳ��������CCmsRcsGetSysParamReq
	CMS_RCS_GET_SYSPARAM_RSP                = 28011,		//��ȡ¼��ϵͳ����Ӧ��CCmsRcsGetSysParamRsp

	//����¼������(��Ҫ���ֶ����澯)������ӿڣ�USER-->SDK
	CMS_RCS_SET_RECORD_TASK_REQ             = 28012,		//����¼����������CCmsRcsSetRecordTaskReq
	CMS_RCS_SET_RECORD_TASK_RSP             = 28013,		//����¼������Ӧ��CCmsRcsSetRecordTaskRsp

	//¼���¼ɾ��������ӿڣ�USER-->SDK
	CMS_RCS_DELETE_RECORD_REQ               = 28014,		//¼���¼ɾ������CCmsRcsDeleteRecordReq
	CMS_RCS_DELETE_RECORD_RSP               = 28015,		//¼���¼ɾ��Ӧ��CCmsRcsDeleteRecordRsp

	//����¼�����������ӿڣ�USER-->SDK
	CMS_RCS_SET_RECORD_ALIAS_REQ            = 28016,		//����¼���������CCmsRcsSetRecordAliasReq
	CMS_RCS_SET_RECORD_ALIAS_RSP            = 28017,		//����¼�����Ӧ��CCmsRcsSetRecordAliasRsp

	//¼������/����������ӿڣ�USER-->SDK
	CMS_RCS_RECORD_LOCK_OPR_REQ             = 28018,		//¼������/��������: CCmsRcsRecordLockOprReq
	CMS_RCS_RECORD_LOCK_OPR_RSP             = 28019,		//¼������/����Ӧ��: CCmsRcsRecordLockOprRsp

    //��ѯNRU�豸������ӿڣ�USER-->SDK
	CMS_RCS_QUERY_NRU_REQ                   = 28020,        //��Ϣ��: CCmsRcsQueryNruReq
	CMS_RCS_QUERY_NRU_RSP                   = 28021,        //��Ϣ��: CCmsRcsQueryNruRsp
	CMS_RCS_QUERY_NRU_NTF_REQ               = 28022,        //��Ϣ��: CCmsRcsQueryNruNtfReq
	CMS_RCS_QUERY_NRU_NTF_RSP               = 28023,        //��Ϣ��: CCmsRcsQueryNruNtfRsp

	//��ѯ����nru����ϸ��Ϣ���ݲ�֧�ַ�ҳ������ӿڣ�USER-->SDK
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_REQ     = 28024,   //��ѯ����nru����ϸ��Ϣ����: CCmsRcsQueryOnlineNruDetailInfoReq
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_RSP     = 28025,   //��ѯ����nru����ϸ��ϢӦ��: CCmsRcsQueryOnlineNruDetailInfoRsp
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_REQ = 28026,   //��ѯ����nru����ϸ��Ϣ֪ͨ: CCmsRcsQueryOnlineNruDetailInfoNtfReq
	CMS_RCS_QUERY_ONLINE_NRU_DETAIL_INFO_NTF_RSP = 28027,   //��ѯ����nru����ϸ��Ϣ֪ͨӦ��: CCmsRcsQueryOnlineNruDetailInfoNtfRsp

    //������Ϣ��ѯ������ӿڣ�USER-->SDK
	CMS_RCS_QUERY_DISK_INFO_REQ             = 28028,        //��Ϣ��: CCmsRcsQueryDiskInfoReq
	CMS_RCS_QUERY_DISK_INFO_RSP             = 28029,        //��Ϣ��: CCmsRcsQueryDiskInfoRsp
	CMS_RCS_QUERY_DISK_INFO_NTF_REQ         = 28030,        //��Ϣ��: CCmsRcsQueryDiskInfoNtfReq
	CMS_RCS_QUERY_DISK_INFO_NTF_RSP         = 28031,        //��Ϣ��: CCmsRcsQueryDiskInfoNtfRsp

	//������Ϣ���ģ�����ӿڣ�USER-->SDK
	CMS_RCS_SUBSCRIBE_DISKINFO_REQ          = 28032,		//������Ϣ��������: CCmsRcsSubscribeDiskInfoReq
	CMS_RCS_SUBSCRIBE_DISKINFO_RSP          = 28033,		//������Ϣ����Ӧ��: CCmsRcsSubscribeDiskInfoRsp
	CMS_RCS_DISK_PART_ALARM_NTF_REQ         = 28034,		//���̷����澯֪ͨ��CCmsRcsDiskPartAlarmNtfReq
	CMS_RCS_DISK_PART_ALARM_NTF_RSP         = 28035,		//���̷����澯֪ͨӦ��CCmsRcsDiskPartAlarmNtfRsp
	CMS_RCS_UNSUBSCRIBE_DISKINFO_REQ        = 28036,		//ȡ��������Ϣ��������: CCmsRcsUnsubscribeDiskInfoReq
	CMS_RCS_UNSUBSCRIBE_DISKINFO_RSP        = 28037,		//ȡ��������Ϣ����Ӧ��: CCmsRcsUnsubscribeDiskInfoRsp

	//rcs¼��״̬Ŀ¼���ġ�ȡ�����ģ�����ӿ�:USER-->SDK
	CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_REQ   = 28038,  	//¼��״̬Ŀ¼��������CCmsRcsCatalogSubscribePuStatusReq
	CMS_RCS_CATALOG_SUBSCRIBE_PU_STATUS_RSP   = 28039,  	//¼��״̬Ŀ¼����Ӧ��CCmsRcsCatalogSubscribePuStatusRsp
	CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_REQ  = 28040, 		//ˢ��¼��״̬Ŀ¼��������CCmsRcsCatalogRefreshSsPuStatusReq
	CMS_RCS_CATALOG_REFRESH_SS_PU_STATUS_RSP  = 28041, 		//ˢ��¼��״̬Ŀ¼����Ӧ��CCmsRcsCatalogRefreshSsPuStatusRsp
	CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_REQ = 28042,		//ȡ��¼��״̬Ŀ¼��������CCmsRcsCatalogUnsubscribePuStatusReq
	CMS_RCS_CATALOG_UNSUBSCRIBE_PU_STATUS_RSP = 28043,		//ȡ��¼��״̬Ŀ¼����Ӧ��CCmsRcsCatalogUnsubscribePuStatusRsp

    //¼��������ݲ�֧�ַ�ҳ��ѯ
	CMS_RCS_SET_RECORD_PARAM_REQ            = 28044,        //��Ϣ��: CCmsRcsSetRecordParamReq
	CMS_RCS_SET_RECORD_PARAM_RSP            = 28045,        //��Ϣ��: CCmsRcsSetRecordParamRsp
	CMS_RCS_QUERY_RECORD_PARAM_REQ          = 28046,        //��Ϣ��: CCmsRcsQueryRecordParamReq
	CMS_RCS_QUERY_RECORD_PARAM_RSP          = 28047,        //��Ϣ��: CCmsRcsQueryRecordParamRsp
    CMS_RCS_QUERY_RECORD_PARAM_NTF_REQ      = 28048,        //��Ϣ��: CCmsRcsQueryRecordParamNtfReq
    CMS_RCS_QUERY_RECORD_PARAM_NTF_RSP      = 28049,        //��Ϣ��: CCmsRcsQueryRecordParamNtfRsp

 	CMS_RCS_QUERY_RECORD_DAYS_REQ           = 28050,        //��Ϣ��: CCmsRcsQueryRecordDaysReq
 	CMS_RCS_QUERY_RECORD_DAYS_RSP           = 28051,        //��Ϣ��: CCmsRcsQueryRecordDaysRsp

    CMS_SDK_RCS_MSG_END                     = 28199,

	//��TVS�Ľ�����Ϣ(��һ�����õ�28255)
	CMS_SDK_TVS_MSG_BEGIN				    = 28201,

	//�����豸��¼tvs֪ͨ������ӿ�:USER<--SDK
	CMS_TVS_DISCONNECT_NTF                  = 28202,        //RCS����֪ͨ��cmssdk���Լ���������
	CMS_TVS_CONNECT_NTF                     = 28203,        //RCS����֪ͨ��TConnInfo

	//����ǽѡ��������ӿڣ�USER-->SDK
	CMS_TVS_REAL_MEDIA_ON_TV_REQ            = 28204,		//ѡ������ǽ����CCmsRealMediaOnTvReq
	CMS_TVS_REAL_MEDIA_ON_TV_RSP            = 28205,		//ѡ������ǽӦ��CCmsRealMediaOnTvRsp
	CMS_TVS_REAL_MEDIA_OFF_TV_REQ           = 28206,		//ֹͣѡ������ǽ����CCmsRealMediaOffTvReq
	CMS_TVS_REAL_MEDIA_OFF_TV_RSP           = 28207,		//ֹͣѡ������ǽӦ��CCmsRealMediaOffTvRsp

    //¼����ǽ������ӿڣ�USER-->SDK
    CMS_TVS_REC_ON_TV_REQ                   = 28248,        //¼����ǽ����: CCmsRecOnTvReq
    CMS_TVS_REC_ON_TV_RSP                   = 28249,        //¼����ǽӦ��: CCmsRecOnTvRsp
    CMS_TVS_REC_ON_TV_ACK                   = 28250,        //¼����ǽAck: CCmsRecOnTvAck

    //ֹͣ¼��طţ�˫��ӿ�:USER<-->SDK
    CMS_TVS_REC_ON_TV_BYE_REQ               = 28251,        //¼����ǽֹͣ����CCmsRecOnTvByeReq
    CMS_TVS_REC_ON_TV_BYE_RSP               = 28252,        //¼����ǽֹͣӦ��CCmsRecOnTvByeRsp

	//����ǽ��ѯ���á���ȡ������ӿڣ�USER-->SDK
	CMS_TVS_TVWALL_SCHEME_GROUP_SET_REQ     = 28208,		//����ǽ��ѯ��������CCmsTvWallSchemeGroupSetReq
	CMS_TVS_TVWALL_SCHEME_GROUP_SET_RSP     = 28209,		//����ǽ��ѯ����Ӧ��CCmsTvWallSchemeGroupSetRsp
	CMS_TVS_TVWALL_SCHEME_GROUP_GET_REQ     = 28210,		//����ǽ��ѯ��ȡ����CCmsTvWallSchemeGroupGetReq
	CMS_TVS_TVWALL_SCHEME_GROUP_GET_RSP     = 28211,		//����ǽ��ѯ��ȡӦ��CCmsTvWallSchemeGroupGetRsp

	//����ǽ��ѯ������ӿڣ�USER-->SDK
	CMS_TVS_TV_POLL_OPERATE_REQ             = 28212,		//����ǽ��ѯ����CCmsTvPollOperateReq
	CMS_TVS_TV_POLL_OPERATE_RSP             = 28213,		//����ǽ��ѯӦ��CCmsTvPollOperateRsp

	//����ǽԤ������ɾ���顢���룬����ӿڣ�USER-->SDK
	CMS_TVS_TVWALL_SCHEME_SAVE_REQ          = 28214,		//����ǽԤ���������CCmsTvWallSchemeSaveReq
	CMS_TVS_TVWALL_SCHEME_SAVE_RSP          = 28215,		//����ǽԤ�����Ӧ��CCmsTvWallSchemeSaveRsp
	CMS_TVS_TVWALL_SCHEME_DEL_REQ           = 28216,		//����ǽԤ��ɾ������CCmsTvWallSchemeDelReq
	CMS_TVS_TVWALL_SCHEME_DEL_RSP           = 28217,		//����ǽԤ��ɾ��Ӧ��CCmsTvWallSchemeDelRsp
	CMS_TVS_TVWALL_SCHEME_QRY_REQ           = 28218,		//����ǽԤ����ѯ����CCmsTvWallSchemeQryReq
	CMS_TVS_TVWALL_SCHEME_QRY_RSP           = 28219,		//����ǽԤ����ѯӦ��CCmsTvWallSchemeQryRsp
	CMS_TVS_TVWALL_SCHEME_QRY_NTF_REQ       = 28220,		//����ǽԤ����ѯ֪ͨ��CCmsTvWallSchemeQryNtfReq
	CMS_TVS_TVWALL_SCHEME_QRY_NTF_RSP       = 28221,		//����ǽԤ����ѯ֪ͨӦ��CCmsTvWallSchemeQryNtfRsp
	CMS_TVS_TVWALL_SCHEME_LOAD_REQ          = 28222,		//����ǽԤ����������CCmsTvWallSchemeLoadReq
	CMS_TVS_TVWALL_SCHEME_LOAD_RSP          = 28223,		//����ǽԤ������Ӧ��CCmsTvWallSchemeLoadRsp

	//����ǽ���ġ�ȡ�����ģ�����ӿڣ�USER-->SDK
	CMS_TVS_TVWALL_STATUS_SS_REQ            = 28224,		//����ǽ��������CCmsTvWallStatusSsReq
	CMS_TVS_TVWALL_STATUS_SS_RSP            = 28225,		//����ǽ����Ӧ��CCmsTvWallStatusSsRsp
	CMS_TVS_TVWALL_STATUS_UNSS_REQ          = 28226,		//����ǽȡ����������CCmsTvWallStatusUnSsReq
	CMS_TVS_TVWALL_STATUS_UNSS_RSP          = 28227,		//����ǽȡ������Ӧ��CCmsTvWallStatusUnSsRsp
	CMS_TVS_TVWALL_STATUS_NTF_REQ           = 28228,		//����ǽ����֪ͨ��CCmsTvWallStatusNtyReq
	CMS_TVS_TVWALL_STATUS_NTF_RSP           = 28229,		//����ǽ����֪ͨӦ��CCmsTvWallStatusNtyRsp

	//����ǽ���������á���ȡ������ӿڣ�USER-->SDK
	CMS_TVS_TV_WIN_STYLE_SET_REQ            = 28230,		//����ǽ��������������: CCmsTvWinStyleSetReq
	CMS_TVS_TV_WIN_STYLE_SET_RSP            = 28231,		//����ǽ����������Ӧ��: CCmsTvWinStyleSetRsp
	CMS_TVS_TV_WIN_STYLE_GET_REQ            = 28232,		//����ǽ�������ȡ����: CCmsTvWinStyleGetReq
	CMS_TVS_TV_WIN_STYLE_GET_RSP            = 28233,		//����ǽ�������ȡӦ��: CCmsTvWinStyleGetRsp

    //������ѡ��ͨ������,����������ӿڣ�USER-->SDK
    CMS_TV_CHANNEL_LOCK_REQ                 = 28234,        //��Ϣ��: CCmsTvChannelLockReq
    CMS_TV_CHANNEL_LOCK_RSP                 = 28235,        //��Ϣ��: CCmsTvChannelLockRsp
    
    CMS_TV_CHANNEL_UNLOCK_REQ               = 28236,        //��Ϣ��: CCmsTvChannelUnlockReq
    CMS_TV_CHANNEL_UNLOCK_RSP               = 28237,        //��Ϣ��: CCmsTvChannelUnlockRsp
    
    //������ͨ���ۿ���״̬���棬����,������ӿڣ�USER-->SDK
	CMS_TV_CHANNEL_WATCH_POINT_SAVE_REQ     = 28238,        //��Ϣ��: CCmsTvChannelWatchPointSaveReq
	CMS_TV_CHANNEL_WATCH_POINT_SAVE_RSP     = 28239,        //��Ϣ��: CCmsTvChannelWatchPointSaveRsp
    
    CMS_TV_CHANNEL_WATCH_POINT_LOAD_REQ     = 28240,         //��Ϣ��: CCmsTvChannelWatchPointLoadReq
    CMS_TV_CHANNEL_WATCH_POINT_LOAD_RSP     = 28241,         //��Ϣ��: CCmsTvChannelWatchPointLoadRsp
    
    CMS_TV_CHANNEL_WATCH_INFO_GET_REQ       = 28242,         //��Ϣ��: CCmsTvChannelWatchInfoGetReq
    CMS_TV_CHANNEL_WATCH_INFO_GET_RSP       = 28243,         //��Ϣ��: CCmsTvChannelWatchInfoGetRsp
    
    //����ǽԤ������������������ӿڣ�USER-->SDK
    CMS_TVWALL_SCHEME_LOCK_REQ              = 28244,           //��Ϣ��: CCmsTvWallSchemeLockReq
    CMS_TVWALL_SCHEME_LOCK_RSP              = 28245,           //��Ϣ��: CCmsTvWallSchemeLockRsp
    
    CMS_TVWALL_SCHEME_UNLOCK_REQ            = 28246,           //��Ϣ��: CCmsTvWallSchemeUnlockReq
    CMS_TVWALL_SCHEME_UNLOCK_RSP            = 28247,           //��Ϣ��: CCmsTvWallSchemeUnlockRsp

    // ����ӿڣ�USER-->SDK
    CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ    = 28253,           //��Ϣ�壺CCmsRealMediaSwitchOccurTypeReq
    CMS_REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP    = 28254,           //��Ϣ�壺CCmsRealMediaSwitchOccurTypeRsp

	CMS_SDK_TVS_MSG_END                     = 28399,

	//��UAS�Ľ�����Ϣ
	CMS_SDK_UAS_MSG_BEGIN				    = 28401,

	//�����豸��¼uas֪ͨ������ӿ�:USER<--SDK
	CMS_UAS_DISCONNECT_NTF                  = 28402,           //UAS����֪ͨ��cmssdk���Լ���������
	CMS_UAS_CONNECT_NTF                     = 28403,           //UAS����֪ͨ��TConnInfo

	//��ƵԴԤ��λ��ӡ�ɾ������ѯ����������ӿ�:USER-->SDK
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ     = 28404,		  //Ԥ��λ�������CCmsVideosourcePresetcfgAddReq�����ô���ʱ�����޸�
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP     = 28405,		  //Ԥ��λ���Ӧ��CCmsVideosourcePresetcfgAddRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ = 28406,		  //Ԥ��λ���֪ͨ��CCmsVideosourcePresetcfgAddNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP = 28407,		  //Ԥ��λ���֪ͨӦ��CCmsVideosourcePresetcfgAddNtfRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ     = 28408,		  //Ԥ��λɾ������CCmsVideosourcePresetcfgDelReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP     = 28409,		  //Ԥ��λɾ��Ӧ��CCmsVideosourcePresetcfgDelRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ = 28410,		  //Ԥ��λɾ��֪ͨ��CCmsVideosourcePresetcfgDelNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP = 28411,		  //Ԥ��λɾ��֪ͨӦ��CCmsVideosourcePresetcfgDelNtfRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ     = 28412,		  //Ԥ��λ��ѯ����CCmsVideosourcePresetcfgQryReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP     = 28413,		  //Ԥ��λ��ѯӦ��CCmsVideosourcePresetcfgQryRsp
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ = 28414,		  //Ԥ��λ��ѯ֪ͨ��CCmsVideosourcePresetcfgQryNtfReq
	CMS_UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP = 28415,		  //Ԥ��λ��ѯ֪ͨӦ��CCmsVideosourcePresetcfgQryNtfRsp

    //�û�������־��ӡ���ѯ������ӿ�
    CMS_UAS_OPERATE_LOG_ADD_REQ               = 28416,        // ��Ϣ�� CCmsOperateLogAddReq ������־����
    CMS_UAS_OPERATE_LOG_ADD_RSP               = 28417,        // ��Ϣ�� CCmsOperateLogAddRsp
    CMS_UAS_OPERATE_LOG_QRY_REQ               = 28418,        // ��Ϣ�� CCmsOperateLogQryReq
    CMS_UAS_OPERATE_LOG_QRY_RSP               = 28419,        // ��Ϣ�� CCmsOperateLogQryRsp
    CMS_UAS_OPERATE_LOG_QRY_NTF_REQ           = 28420,        // ��Ϣ�� CCmsOperateLogQryNtfReq
    CMS_UAS_OPERATE_LOG_QRY_NTF_RSP           = 28421,        // ��Ϣ�� CCmsOperateLogQryNtfRsp

	CMS_SDK_UAS_MSG_END                       = 28599,

    CMS_SDK_ALS_MSG_BEGIN                     = 28601,

    //�澯��������������
    CMS_ALARMLINK_CFG_SET_REQ             = 28602,     //��Ϣ��: CCmsAlarmLinkCfgSetReq
    CMS_ALARMLINK_CFG_SET_RSP             = 28603,     //��Ϣ��: CCmsAlarmLinkCfgSetRsp

    CMS_ALARMLINK_CFG_GET_REQ             = 28604,     //��Ϣ��: CCmsAlarmLinkCfgGetReq
    CMS_ALARMLINK_CFG_GET_RSP             = 28605,     //��Ϣ��: CCmsAlarmLinkCfgGetRsp

    //�����豸��¼ALS������ӿ�:USER<--SDK
    CMS_ALS_DISCONNECT_NTF                = 28606,    //ALS����֪ͨ��cmssdk���Լ���������
    CMS_ALS_CONNECT_NTF                   = 28607,    //ALS����֪ͨ��TConnInfo

	CMS_SUBSCRIBE_TERMINATE_NTF           = 28608,    //�����쳣��ֹ֪ͨ:��

    CMS_SDK_ALS_MSG_END                       = 28799,

	CMS_SDK_MSG_END                           = 28999,                                    //CmsSdk��Ϣ�������
};


#endif // __CMS_SDK_EVENT_H__









