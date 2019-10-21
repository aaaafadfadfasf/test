/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cuievent.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: ����cuifcgi��cuiserver֮����ڲ���Ϣ
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/1/6   1.0         liangli        ����
******************************************************************************/
#ifndef _CUI_EVENT_H_
#define _CUI_EVENT_H_

//#include "cms/cms_eventid.h"
//#include "cms/cms_proto.h"

enum ECUI_EVENT
{
	CU_CUI_INNER_EVENT_BEGIN  = USER_INNER_MSG_BEGIN,
    CU_CUI_WEBSERVICE_REQ = CU_CUI_INNER_EVENT_BEGIN,       //����WebService�ӿ���SIPһһ��Ӧ����Ϣ�ô���Ϣ��
    CUI_CU_WEBSERVICE_RSP,
    CUI_REG_CMU_TIMER,              //��CMUע�ᶨʱ��, ���������ʹ��: 1, ������Proxy��2����CMU������
	CUI_WAIT_CMU_REG_RSP_TIMER,     //�ȴ�CMU��ע��Ӧ��ʱ�����Է���ʱ
	CUI_CONNECT_REDIS_TIMER,            //����REDIS���ݿⶨʱ��
	CUI_UPDATE_NETSEG_TIMER,        //����ע��������Ϣ��ʱ��
    CU_CUI_GET_NOTIFY_REQ,          //CU��CUI������������, ��Ϣ��CGetNotifyReq
    CUI_CU_GET_NOTIFY_RSP,          //CUI��CUӦ�����е�֪ͨ��Ϣ, ��Ϣ��CGetNotifyRsp
    CU_CUI_SUBSCRIBE_PUSTATUS_REQ,  //����PU״̬���󣬿��ܶ�������״̬
    CUI_CU_SUBSCRIBE_PUSTATUS_RSP,  //����PU״̬Ӧ��
    CU_CUI_UNSUBSCRIBE_PUSTATUS_REQ,  //ȡ������PU״̬���󣬿���ȡ������״̬����
    CUI_CU_UNSUBSCRIBE_PUSTATUS_RSP,  //ȡ������PU״̬Ӧ��
    CU_CUI_INVITE_ACK_REQ,
    CUI_CU_INVITE_ACK_RSP,          //��ΪCUI��������Ӧ��ģʽ��Ϊ������INVITE_ACK��Ӧ������
    CU_CUI_INVITE_BYE_REQ,
    CUI_CU_INVITE_BYE_RSP,          //BYEʵ���ϲ���Ҫ��Ϣ�壬����Ϊ������CUI����Ӧ��ģʽ�����
    CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ,
    //CU_CUI_INVITE_PLAY_PLATFORMRECORD_RSP,
    CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ,
    CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_RSP,
    CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ,
    //CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_RSP,
    CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ,
    //CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_RSP,
    CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ,
    CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_RSP,
    CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ,
    //CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_RSP,
    CU_CUI_CHANGE_PASSWORD_REQ,
    CU_CUI_PLAYBACK_PROGRESS_QUERY_REQ,
    CU_CUI_PLAYBACK_PROGRESS_QUERY_RSP,
    CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ,
    CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ,
    CU_CUI_INVITE_PLAY_PURECORD_REQ,
    CU_CUI_INVITE_STOP_PURECORD_REQ,
    CU_CUI_GET_EXPORT_PROCESS_REQ,		// CCuGetExportProcessReq �����ں�3A UASͨ�ţ��ɽӿ�
	CU_CUI_GET_EXPORT_PROCESS_RSP,		// CCuGetExportProcessRsp 
    CU_CUI_STOP_EXPORT_REQ,
    CU_CUI_STOP_EXPORT_RSP,
    CU_CUI_PU_REC_QRY_REQ,
    CU_CUI_PU_REC_QRY_RSP,
	CUI_NATDETECT_CALLBACK,  // NAT̽����ص�
	CU_CUI_CU_PROPERTY_REQ,
	CU_CUI_CU_PROPERTY_RSP,
	CU_CUI_INNER_EVENT_END,          
	CUI_CLEAR_VTDUNATPACKET, //���vtdu NAT̽�����Ϣ
	CU_CUI_OFFLINEDEV_QRY_REQ,
	CU_CUI_OFFLINEDEV_QRY_RSP,
	CUI_CLEAR_VTDUNATPACKET_TIMER,  // cu�˳����ٴ����nat̽���
	CU_CUI_FILE_EXPORT_START_EX_REQ,		// CCuFileExportStartExReq
	CU_CUI_FILE_EXPORT_START_EX_RSP,		// CCuFileExportStartExRsp
	CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ,	// CCuFileExportGetProcessExReq
	CU_CUI_FILE_EXPORT_GET_PROCESS_EX_RSP,	// CCuFileExportGetProcessExRsp
	CU_CUI_FILE_EXPORT_STOP_EX_REQ,			// CCuFileExportStopExReq
	CU_CUI_FILE_EXPORT_STOP_EX_RSP,			// CCuFileExportStopExRsp
	CU_CUI_FILE_IMPORT_START_EX_REQ,		// CCuFileImportStartExReq
	CU_CUI_FILE_IMPORT_START_EX_RSP,		// CCuFileImportStartExRsp
	CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ,	// CCuFileImportSetProcessExReq
	CU_CUI_FILE_IMPORT_SET_PROCESS_EX_RSP,	// CCuFileImportSetProcessExRsp
	CU_CUI_FILE_IMPORT_STOP_EX_REQ,			// CCuFileImportStopExReq
	CU_CUI_FILE_IMPORT_STOP_EX_RSP,			// CCuFileImportStopExRsp
	CUI_REDIS_NTF_CALLBACK,					// redis����֪ͨ�Ļص�
	CUI_REDIS_DISC,				            // redis����
	CU_CUI_DPS_SEARCH_REQ,                  // CCuDPSSearchReq
	CU_CUI_DPS_SEARCH_RSP,                  // CCuDPSSearchRsp
	CU_CUI_DPS_GET_SEARCH_RESULT_REQ,       // CCuDPSGetResultReq
	CU_CUI_DPS_GET_SEARCH_RESULT_RSP,       // CCuDPSGetResultRsp
	CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_REQ, // CCuDPSGetResultByGroupReq
	CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_RSP, // CCuDPSGetResultByGroupRsp
	CU_CUI_DPS_SEARCH_STOP_REQ,             // CCuDPSStopReq
	CU_CUI_DPS_SEARCH_STOP_RSP,             // CCuDPSStopRsp
    CU_CUI_GET_USER_VIDEOSRC_LIST_REQ,      // CCuUserDevGrpVidsrcQryReq
    CU_CUI_GET_USER_VIDEOSRC_LIST_RSP,      // CCuUserDevGrpVidsrcQryRsp
	CU_CUI_BATCH_OPT_REQ,					// CCuBatOptReq
	CU_CUI_BATCH_OPT_RSP,					// CCuBatOptRsp
	CU_CUI_GET_DEV_INFO_AND_STATUS_REQ,     // CCuGetDevInfoAndStatusReq
	CU_CUI_GET_DEV_INFO_AND_STATUS_RSP,     // CCuGetDevInfoAndStatusRsp
	CU_CUI_START_ENCODER_TRANS_NTF_REQ,		// CCuStartEncoderTransNtfReq
	CU_CUI_START_ENCODER_TRANS_NTF_RSP,		// CCuStartEncoderTransNtfRsp
    CU_CUI_DEV_RECORD_SVR_QRY_REQ,          // CDeviceRecordSvrQryReq
    CU_CUI_DEV_RECORD_SVR_QRY_RSP,          // CDeviceRecordSvrQryRsp
	CUI_BAT_OPT_TASK_NTF,           //��������ѭ��֪ͨ
	CUI_BAT_OPT_TASK_TIMER,         //��������ѭ����ʱ��

	CU_CUI_SECURE_CERTIFICATE_QRY_REQ,      // CCuCertificateQryReq
	CU_CUI_SECURE_CERTIFICATE_QRY_RSP,      // CCuCertificateQryRsp
	CU_CUI_SECURITY_AUTHORIZATION_FIRST_REQ, // CCuSecurityAuthorizationFirstReq
	CU_CUI_SECURITY_AUTHORIZATION_FIRST_RSP, // CCuSecurityAuthorizationFirstRsp
	CU_CUI_SECURITY_AUTHORIZATION_SECOND_REQ,// CCuSecurityAuthorizationSecondReq
	CU_CUI_SECURITY_AUTHORIZATION_SECOND_RSP,// CCuSecurityAuthorizationSecondRsp
};


inline void InitCuiEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(CU_CUI_WEBSERVICE_REQ);                   //����WebService�ӿ���SIPһһ��Ӧ����Ϣ�ô���Ϣ��
    OSP_ADD_EVENT_DESC(CUI_CU_WEBSERVICE_RSP);                   
    OSP_ADD_EVENT_DESC(CUI_REG_CMU_TIMER);                       //��CMUע�ᶨʱ��, ���������ʹ��: 1, ������Proxy��2����CMU������
    OSP_ADD_EVENT_DESC(CUI_WAIT_CMU_REG_RSP_TIMER);              //�ȴ�CMU��ע��Ӧ��ʱ�����Է���ʱ
    OSP_ADD_EVENT_DESC(CUI_CONNECT_REDIS_TIMER);                 //����REDIS���ݿⶨʱ��
    OSP_ADD_EVENT_DESC(CUI_UPDATE_NETSEG_TIMER);                 //����ע��������Ϣ��ʱ��
    OSP_ADD_EVENT_DESC(CU_CUI_GET_NOTIFY_REQ);                   //CU��CUI������������, ��Ϣ��CGetNotifyReq
    OSP_ADD_EVENT_DESC(CUI_CU_GET_NOTIFY_RSP);                   //CUI��CUӦ�����е�֪ͨ��Ϣ, ��Ϣ��CGetNotifyRsp
    OSP_ADD_EVENT_DESC(CU_CUI_SUBSCRIBE_PUSTATUS_REQ);           //����PU״̬���󣬿��ܶ�������״̬
    OSP_ADD_EVENT_DESC(CUI_CU_SUBSCRIBE_PUSTATUS_RSP);           //����PU״̬Ӧ��
    OSP_ADD_EVENT_DESC(CU_CUI_UNSUBSCRIBE_PUSTATUS_REQ);         //ȡ������PU״̬���󣬿���ȡ������״̬����
    OSP_ADD_EVENT_DESC(CUI_CU_UNSUBSCRIBE_PUSTATUS_RSP);         //ȡ������PU״̬Ӧ��
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_ACK_REQ);                   
    OSP_ADD_EVENT_DESC(CUI_CU_INVITE_ACK_RSP);                   //��ΪCUI��������Ӧ��ģʽ��Ϊ������INVITE_ACK��Ӧ������
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_BYE_REQ);                   
    OSP_ADD_EVENT_DESC(CUI_CU_INVITE_BYE_RSP);                   //BYEʵ���ϲ���Ҫ��Ϣ�壬����Ϊ������CUI����Ӧ��ģʽ�����
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ);   
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_RSP);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_RSP);
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ);
    OSP_ADD_EVENT_DESC(CU_CUI_CHANGE_PASSWORD_REQ);              
    OSP_ADD_EVENT_DESC(CU_CUI_PLAYBACK_PROGRESS_QUERY_REQ);      
    OSP_ADD_EVENT_DESC(CU_CUI_PLAYBACK_PROGRESS_QUERY_RSP);      
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ); 
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ); 
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_PLAY_PURECORD_REQ);         
    OSP_ADD_EVENT_DESC(CU_CUI_INVITE_STOP_PURECORD_REQ);         
    OSP_ADD_EVENT_DESC(CU_CUI_GET_EXPORT_PROCESS_REQ);           // CCuGetExportProcessReq �����ں�3A UASͨ�ţ��ɽӿ�
    OSP_ADD_EVENT_DESC(CU_CUI_GET_EXPORT_PROCESS_RSP);           // CCuGetExportProcessRsp 
    OSP_ADD_EVENT_DESC(CU_CUI_STOP_EXPORT_REQ);                  
    OSP_ADD_EVENT_DESC(CU_CUI_STOP_EXPORT_RSP);                  
    OSP_ADD_EVENT_DESC(CU_CUI_PU_REC_QRY_REQ);                   
    OSP_ADD_EVENT_DESC(CU_CUI_PU_REC_QRY_RSP);                   
    OSP_ADD_EVENT_DESC(CUI_NATDETECT_CALLBACK);                  // NAT̽����ص�
    OSP_ADD_EVENT_DESC(CU_CUI_CU_PROPERTY_REQ);                  
    OSP_ADD_EVENT_DESC(CU_CUI_CU_PROPERTY_RSP);                  
    OSP_ADD_EVENT_DESC(CUI_CLEAR_VTDUNATPACKET);                 //���vtdu NAT̽�����Ϣ
    OSP_ADD_EVENT_DESC(CU_CUI_OFFLINEDEV_QRY_REQ);               
    OSP_ADD_EVENT_DESC(CU_CUI_OFFLINEDEV_QRY_RSP);               
    OSP_ADD_EVENT_DESC(CUI_CLEAR_VTDUNATPACKET_TIMER);           // cu�˳����ٴ����nat̽���
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_START_EX_REQ);         // CCuFileExportStartExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_START_EX_RSP);         // CCuFileExportStartExRsp
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_GET_PROCESS_EX_REQ);   // CCuFileExportGetProcessExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_GET_PROCESS_EX_RSP);   // CCuFileExportGetProcessExRsp
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_STOP_EX_REQ);          // CCuFileExportStopExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_EXPORT_STOP_EX_RSP);          // CCuFileExportStopExRsp
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_START_EX_REQ);         // CCuFileImportStartExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_START_EX_RSP);         // CCuFileImportStartExRsp
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_SET_PROCESS_EX_REQ);   // CCuFileImportSetProcessExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_SET_PROCESS_EX_RSP);   // CCuFileImportSetProcessExRsp
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_STOP_EX_REQ);          // CCuFileImportStopExReq
    OSP_ADD_EVENT_DESC(CU_CUI_FILE_IMPORT_STOP_EX_RSP);          // CCuFileImportStopExRsp
    OSP_ADD_EVENT_DESC(CUI_REDIS_NTF_CALLBACK);                  // redis����֪ͨ�Ļص�
    OSP_ADD_EVENT_DESC(CUI_REDIS_DISC);                          // redis����
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_SEARCH_REQ);                   // CCuDPSSearchReq
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_SEARCH_RSP);                   // CCuDPSSearchRsp
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_GET_SEARCH_RESULT_REQ);        // CCuDPSGetResultReq
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_GET_SEARCH_RESULT_RSP);        // CCuDPSGetResultRsp
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_REQ);// CCuDPSGetResultByGroupReq
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_GET_SEARCH_RESULT_BY_GROUP_RSP);// CCuDPSGetResultByGroupRsp
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_SEARCH_STOP_REQ);              // CCuDPSStopReq
    OSP_ADD_EVENT_DESC(CU_CUI_DPS_SEARCH_STOP_RSP);              // CCuDPSStopRsp
    OSP_ADD_EVENT_DESC(CU_CUI_GET_USER_VIDEOSRC_LIST_REQ);       // CCuUserDevGrpVidsrcQryReq
    OSP_ADD_EVENT_DESC(CU_CUI_GET_USER_VIDEOSRC_LIST_RSP);       // CCuUserDevGrpVidsrcQryRsp
    OSP_ADD_EVENT_DESC(CU_CUI_BATCH_OPT_REQ);                    // CCuBatOptReq
    OSP_ADD_EVENT_DESC(CU_CUI_BATCH_OPT_RSP);                    // CCuBatOptRsp
    OSP_ADD_EVENT_DESC(CU_CUI_GET_DEV_INFO_AND_STATUS_REQ);      // CCuGetDevInfoAndStatusReq
    OSP_ADD_EVENT_DESC(CU_CUI_GET_DEV_INFO_AND_STATUS_RSP);      // CCuGetDevInfoAndStatusRsp
    OSP_ADD_EVENT_DESC(CU_CUI_START_ENCODER_TRANS_NTF_REQ);      // CCuStartEncoderTransNtfReq
    OSP_ADD_EVENT_DESC(CU_CUI_START_ENCODER_TRANS_NTF_RSP);      // CCuStartEncoderTransNtfRsp
    OSP_ADD_EVENT_DESC(CU_CUI_DEV_RECORD_SVR_QRY_REQ);           // CDeviceRecordSvrQryReq
    OSP_ADD_EVENT_DESC(CU_CUI_DEV_RECORD_SVR_QRY_RSP);           // CDeviceRecordSvrQryRsp
    OSP_ADD_EVENT_DESC(CUI_BAT_OPT_TASK_NTF);                    //��������ѭ��֪ͨ
    OSP_ADD_EVENT_DESC(CUI_BAT_OPT_TASK_TIMER);                  //��������ѭ����ʱ��
    OSP_ADD_EVENT_DESC(CU_CUI_SECURE_CERTIFICATE_QRY_REQ);       // CCuCertificateQryReq
    OSP_ADD_EVENT_DESC(CU_CUI_SECURE_CERTIFICATE_QRY_RSP);       // CCuCertificateQryRsp
    OSP_ADD_EVENT_DESC(CU_CUI_SECURITY_AUTHORIZATION_FIRST_REQ); // CCuSecurityAuthorizationFirstReq
    OSP_ADD_EVENT_DESC(CU_CUI_SECURITY_AUTHORIZATION_FIRST_RSP); // CCuSecurityAuthorizationFirstRsp
    OSP_ADD_EVENT_DESC(CU_CUI_SECURITY_AUTHORIZATION_SECOND_REQ);// CCuSecurityAuthorizationSecondReq
    OSP_ADD_EVENT_DESC(CU_CUI_SECURITY_AUTHORIZATION_SECOND_RSP);// CCuSecurityAuthorizationSecondRsp
}

//#define CU_PTZ_CTRL_HOLD_TIME 60 //Ĭ��CU��PTZ���ưѳ�ʱ��(��)

#endif

