/*****************************************************************************
   ģ����      : uas_event
   �ļ���      : uas_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: uas_event.h ������uas���ⲿ��ϢID
   ����        : ��־��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2012/05/28  1.0         huzhiyun   ����
******************************************************************************/
#ifndef _UAS_EVENT_H_
#define _UAS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* uasЭ�� */
enum 
{
    UAS_LOGIN_REQ = UAS_EVENT_BEGIN, // ��Ϣ�� CLoginUasReq  ��¼����, �Ѿ������������ͳһ�������¼����
    UAS_LOGIN_RSP,  // ��Ϣ�� CLoginUasRsp
    UAS_LOGOUT_REQ, // ��Ϣ�� CLogoutUasReq
    UAS_LOGOUT_RSP, // ��Ϣ�� CLogoutUasRsp

    UAS_OPERATE_LOG_ADD_REQ, // ��Ϣ�� COperateLogAddReq ������־����
    UAS_OPERATE_LOG_ADD_RSP, // ��Ϣ�� COperateLogAddRsp
    UAS_OPERATE_LOG_QRY_REQ, // ��Ϣ�� COperateLogQryReq
    UAS_OPERATE_LOG_QRY_RSP, // ��Ϣ�� COperateLogQryRsp
    UAS_OPERATE_LOG_QRY_NTF_REQ, // ��Ϣ�� COperateLogQryNtfReq
	UAS_OPERATE_LOG_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
	UAS_DEVICE_LOG_ADD_REQ, // ��Ϣ�� CDeviceLogAddReq �豸��־����
	UAS_DEVICE_LOG_ADD_RSP, // ��Ϣ�� CDeviceLogAddRsp
	UAS_DEVICE_LOG_QRY_REQ, // ��Ϣ�� CDeviceLogQryReq
	UAS_DEVICE_LOG_QRY_RSP, // ��Ϣ�� CDeviceLogQryRsp
	UAS_DEVICE_LOG_QRY_NTF_REQ, // ��Ϣ�� CDeviceLogQryNtfReq
	UAS_DEVICE_LOG_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp
	UAS_ALARM_LOG_ADD_REQ, // ��Ϣ�� CAlarmLogAddReq �澯��־����
	UAS_ALARM_LOG_ADD_RSP, // ��Ϣ�� CAlarmLogAddRsp
	UAS_ALARM_LOG_QRY_REQ, // ��Ϣ�� CAlarmLogQryReq
	UAS_ALARM_LOG_QRY_RSP, // ��Ϣ�� CAlarmLogQryRsp
	UAS_ALARM_LOG_QRY_NTF_REQ, // ��Ϣ�� CAlarmLogQryNtfReq
	UAS_ALARM_LOG_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    // CU�û��Զ������ݹ���
    UAS_CUSER_CUSTOMDATA_ADD_REQ, // ��Ϣ�� CUserCustomDataAddReq  �Զ������ݴ���ʱ�����޸�
    UAS_CUSER_CUSTOMDATA_ADD_RSP, // ��Ϣ�� CUserCustomDataAddRsp
    UAS_CUSER_CUSTOMDATA_ADD_NTF_REQ, // ��Ϣ�� CUserCustomDataAddNtfReq
    UAS_CUSER_CUSTOMDATA_ADD_NTF_RSP, // ��Ϣ�� CNtfRsp
    UAS_CUSER_CUSTOMDATA_DEL_REQ, // ��Ϣ�� CUserCustomDataDelReq
    UAS_CUSER_CUSTOMDATA_DEL_RSP, // ��Ϣ�� CUserCustomDataDelRsp
    UAS_CUSER_CUSTOMDATA_DEL_NTF_REQ, // ��Ϣ�� CUserCustomDataDelNtfReq
    UAS_CUSER_CUSTOMDATA_DEL_NTF_RSP, // ��Ϣ�� CNtfRsp
    UAS_CUSER_CUSTOMDATA_QRY_REQ, // ��Ϣ�� CUserCustomDataQryReq
    UAS_CUSER_CUSTOMDATA_QRY_RSP, // ��Ϣ�� CUserCustomDataQryRsp
    UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ, // ��Ϣ�� CUserCustomDataQryNtfReq
    UAS_CUSER_CUSTOMDATA_QRY_NTF_RSP, // ��Ϣ�� CNtfRsp

    // ǰ���������ݹ���(��ƵԴ��������)
    UAS_VIDEOSOURCE_BASICCFG_ADD_REQ,   // ��Ϣ�� CVideosourceBasiccfgAddReq   ���ô���ʱ�����޸�
    UAS_VIDEOSOURCE_BASICCFG_ADD_RSP,   // ��Ϣ�� CVideosourceBasiccfgAddRsp
    UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_REQ,   // ��Ϣ�� CVideosourceBasiccfgAddNtfReq
    UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_RSP,   // ��Ϣ�� CNtfRsp
    UAS_VIDEOSOURCE_BASICCFG_DEL_REQ,   // ��Ϣ�� CVideosourceBasiccfgDelReq
    UAS_VIDEOSOURCE_BASICCFG_DEL_RSP,   // ��Ϣ�� CVideosourceBasiccfgDelRsp
    UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_REQ,   // ��Ϣ�� CVideosourceBasiccfgDelNtfReq
    UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_RSP,   // ��Ϣ�� CNtfRsp
    UAS_VIDEOSOURCE_BASICCFG_QRY_REQ,   // ��Ϣ�� CVideosourceBasiccfgQryReq
    UAS_VIDEOSOURCE_BASICCFG_QRY_RSP,   // ��Ϣ�� CVideosourceBasiccfgQryRsp
    UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_REQ,   // ��Ϣ�� CVideosourceBasiccfgQryNtfReq
    UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_RSP,   // ��Ϣ�� CNtfRsp

    // ǰ���������ݹ���(��ƵԴԤ��λ����)
    UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ,   // ��Ϣ�� CVideosourcePresetcfgAddReq   ���ô���ʱ�����޸�
    UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP,   // ��Ϣ�� CVideosourcePresetcfgAddRsp
    UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ,   // ��Ϣ�� CVideosourcePresetcfgAddNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP,   // ��Ϣ�� CNtfRsp
    UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ,   // ��Ϣ�� CVideosourcePresetcfgDelReq
    UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP,   // ��Ϣ�� CVideosourcePresetcfgDelRsp
    UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ,   // ��Ϣ�� CVideosourcePresetcfgDelNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP,   // ��Ϣ�� CNtfRsp
    UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ,   // ��Ϣ�� CVideosourcePresetcfgQryReq
    UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP,   // ��Ϣ�� CVideosourcePresetcfgQryRsp
    UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ,   // ��Ϣ�� CVideosourcePresetcfgQryNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP,   // ��Ϣ�� CNtfRsp

    UAS_CONFIG_UAS_SET_REQ,   // ��Ϣ�� CConfigUasSetReq  uas����(����: a. ��־�����������)
    UAS_CONFIG_UAS_SET_RSP,   // ��Ϣ�� CConfigUasSetRsp
    UAS_CONFIG_UAS_GET_REQ,   // ��Ϣ�� CConfigUasGetReq
    UAS_CONFIG_UAS_GET_RSP,   // ��Ϣ�� CConfigUasGetRsp
};

inline void InitUasEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(UAS_LOGIN_REQ);                           // ��Ϣ�� CLoginUasReq  ��¼����, �Ѿ������������ͳһ�������¼����
    OSP_ADD_EVENT_DESC(UAS_LOGIN_RSP);                           // ��Ϣ�� CLoginUasRsp
    OSP_ADD_EVENT_DESC(UAS_LOGOUT_REQ);                          // ��Ϣ�� CLogoutUasReq
    OSP_ADD_EVENT_DESC(UAS_LOGOUT_RSP);                          // ��Ϣ�� CLogoutUasRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_ADD_REQ);                 // ��Ϣ�� COperateLogAddReq ������־����
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_ADD_RSP);                 // ��Ϣ�� COperateLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_REQ);                 // ��Ϣ�� COperateLogQryReq
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_RSP);                 // ��Ϣ�� COperateLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_NTF_REQ);             // ��Ϣ�� COperateLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_NTF_RSP);             // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_ADD_REQ);                  // ��Ϣ�� CDeviceLogAddReq �豸��־����
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_ADD_RSP);                  // ��Ϣ�� CDeviceLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_REQ);                  // ��Ϣ�� CDeviceLogQryReq
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_RSP);                  // ��Ϣ�� CDeviceLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_NTF_REQ);              // ��Ϣ�� CDeviceLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_NTF_RSP);              // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_ADD_REQ);                   // ��Ϣ�� CAlarmLogAddReq �澯��־����
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_ADD_RSP);                   // ��Ϣ�� CAlarmLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_REQ);                   // ��Ϣ�� CAlarmLogQryReq
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_RSP);                   // ��Ϣ�� CAlarmLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_NTF_REQ);               // ��Ϣ�� CAlarmLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_NTF_RSP);               // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_REQ);            // ��Ϣ�� CUserCustomDataAddReq  �Զ������ݴ���ʱ�����޸�
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_RSP);            // ��Ϣ�� CUserCustomDataAddRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_NTF_REQ);        // ��Ϣ�� CUserCustomDataAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_REQ);            // ��Ϣ�� CUserCustomDataDelReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_RSP);            // ��Ϣ�� CUserCustomDataDelRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_NTF_REQ);        // ��Ϣ�� CUserCustomDataDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_REQ);            // ��Ϣ�� CUserCustomDataQryReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_RSP);            // ��Ϣ�� CUserCustomDataQryRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ);        // ��Ϣ�� CUserCustomDataQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_NTF_RSP);        // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_REQ);        // ��Ϣ�� CVideosourceBasiccfgAddReq   ���ô���ʱ�����޸�
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_RSP);        // ��Ϣ�� CVideosourceBasiccfgAddRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_REQ);    // ��Ϣ�� CVideosourceBasiccfgAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_RSP);    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_REQ);        // ��Ϣ�� CVideosourceBasiccfgDelReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_RSP);        // ��Ϣ�� CVideosourceBasiccfgDelRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_REQ);    // ��Ϣ�� CVideosourceBasiccfgDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_RSP);    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_REQ);        // ��Ϣ�� CVideosourceBasiccfgQryReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_RSP);        // ��Ϣ�� CVideosourceBasiccfgQryRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_REQ);    // ��Ϣ�� CVideosourceBasiccfgQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_RSP);    // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ);       // ��Ϣ�� CVideosourcePresetcfgAddReq   ���ô���ʱ�����޸�
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP);       // ��Ϣ�� CVideosourcePresetcfgAddRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ);   // ��Ϣ�� CVideosourcePresetcfgAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ);       // ��Ϣ�� CVideosourcePresetcfgDelReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP);       // ��Ϣ�� CVideosourcePresetcfgDelRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ);   // ��Ϣ�� CVideosourcePresetcfgDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ);       // ��Ϣ�� CVideosourcePresetcfgQryReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP);       // ��Ϣ�� CVideosourcePresetcfgQryRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ);   // ��Ϣ�� CVideosourcePresetcfgQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP);   // ��Ϣ�� CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_SET_REQ);                  // ��Ϣ�� CConfigUasSetReq  uas����(����: a. ��־�����������)
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_SET_RSP);                  // ��Ϣ�� CConfigUasSetRsp
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_GET_REQ);                  // ��Ϣ�� CConfigUasGetReq
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_GET_RSP);                  // ��Ϣ�� CConfigUasGetRsp
}

#endif // _UAS_EVENT_H_










