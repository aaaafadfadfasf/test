/*****************************************************************************
   模块名      : uas_event
   文件名      : uas_event.h
   相关文件    : 
   文件实现功能: uas_event.h 定义了uas的外部消息ID
   作者        : 胡志云
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2012/05/28  1.0         huzhiyun   创建
******************************************************************************/
#ifndef _UAS_EVENT_H_
#define _UAS_EVENT_H_

#include "cms/cms_eventid.h"
#include "cms/cms_proto.h"

/* uas协议 */
enum 
{
    UAS_LOGIN_REQ = UAS_EVENT_BEGIN, // 消息体 CLoginUasReq  登录管理, 已经废弃。请采用统一的外设登录信令
    UAS_LOGIN_RSP,  // 消息体 CLoginUasRsp
    UAS_LOGOUT_REQ, // 消息体 CLogoutUasReq
    UAS_LOGOUT_RSP, // 消息体 CLogoutUasRsp

    UAS_OPERATE_LOG_ADD_REQ, // 消息体 COperateLogAddReq 操作日志管理
    UAS_OPERATE_LOG_ADD_RSP, // 消息体 COperateLogAddRsp
    UAS_OPERATE_LOG_QRY_REQ, // 消息体 COperateLogQryReq
    UAS_OPERATE_LOG_QRY_RSP, // 消息体 COperateLogQryRsp
    UAS_OPERATE_LOG_QRY_NTF_REQ, // 消息体 COperateLogQryNtfReq
	UAS_OPERATE_LOG_QRY_NTF_RSP, // 消息体 CNtfRsp
	UAS_DEVICE_LOG_ADD_REQ, // 消息体 CDeviceLogAddReq 设备日志管理
	UAS_DEVICE_LOG_ADD_RSP, // 消息体 CDeviceLogAddRsp
	UAS_DEVICE_LOG_QRY_REQ, // 消息体 CDeviceLogQryReq
	UAS_DEVICE_LOG_QRY_RSP, // 消息体 CDeviceLogQryRsp
	UAS_DEVICE_LOG_QRY_NTF_REQ, // 消息体 CDeviceLogQryNtfReq
	UAS_DEVICE_LOG_QRY_NTF_RSP, // 消息体 CNtfRsp
	UAS_ALARM_LOG_ADD_REQ, // 消息体 CAlarmLogAddReq 告警日志管理
	UAS_ALARM_LOG_ADD_RSP, // 消息体 CAlarmLogAddRsp
	UAS_ALARM_LOG_QRY_REQ, // 消息体 CAlarmLogQryReq
	UAS_ALARM_LOG_QRY_RSP, // 消息体 CAlarmLogQryRsp
	UAS_ALARM_LOG_QRY_NTF_REQ, // 消息体 CAlarmLogQryNtfReq
	UAS_ALARM_LOG_QRY_NTF_RSP, // 消息体 CNtfRsp

    // CU用户自定义数据管理
    UAS_CUSER_CUSTOMDATA_ADD_REQ, // 消息体 CUserCustomDataAddReq  自定义数据存在时进行修改
    UAS_CUSER_CUSTOMDATA_ADD_RSP, // 消息体 CUserCustomDataAddRsp
    UAS_CUSER_CUSTOMDATA_ADD_NTF_REQ, // 消息体 CUserCustomDataAddNtfReq
    UAS_CUSER_CUSTOMDATA_ADD_NTF_RSP, // 消息体 CNtfRsp
    UAS_CUSER_CUSTOMDATA_DEL_REQ, // 消息体 CUserCustomDataDelReq
    UAS_CUSER_CUSTOMDATA_DEL_RSP, // 消息体 CUserCustomDataDelRsp
    UAS_CUSER_CUSTOMDATA_DEL_NTF_REQ, // 消息体 CUserCustomDataDelNtfReq
    UAS_CUSER_CUSTOMDATA_DEL_NTF_RSP, // 消息体 CNtfRsp
    UAS_CUSER_CUSTOMDATA_QRY_REQ, // 消息体 CUserCustomDataQryReq
    UAS_CUSER_CUSTOMDATA_QRY_RSP, // 消息体 CUserCustomDataQryRsp
    UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ, // 消息体 CUserCustomDataQryNtfReq
    UAS_CUSER_CUSTOMDATA_QRY_NTF_RSP, // 消息体 CNtfRsp

    // 前端配置数据管理(视频源基本配置)
    UAS_VIDEOSOURCE_BASICCFG_ADD_REQ,   // 消息体 CVideosourceBasiccfgAddReq   配置存在时进行修改
    UAS_VIDEOSOURCE_BASICCFG_ADD_RSP,   // 消息体 CVideosourceBasiccfgAddRsp
    UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_REQ,   // 消息体 CVideosourceBasiccfgAddNtfReq
    UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_RSP,   // 消息体 CNtfRsp
    UAS_VIDEOSOURCE_BASICCFG_DEL_REQ,   // 消息体 CVideosourceBasiccfgDelReq
    UAS_VIDEOSOURCE_BASICCFG_DEL_RSP,   // 消息体 CVideosourceBasiccfgDelRsp
    UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_REQ,   // 消息体 CVideosourceBasiccfgDelNtfReq
    UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_RSP,   // 消息体 CNtfRsp
    UAS_VIDEOSOURCE_BASICCFG_QRY_REQ,   // 消息体 CVideosourceBasiccfgQryReq
    UAS_VIDEOSOURCE_BASICCFG_QRY_RSP,   // 消息体 CVideosourceBasiccfgQryRsp
    UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_REQ,   // 消息体 CVideosourceBasiccfgQryNtfReq
    UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_RSP,   // 消息体 CNtfRsp

    // 前端配置数据管理(视频源预置位配置)
    UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ,   // 消息体 CVideosourcePresetcfgAddReq   配置存在时进行修改
    UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP,   // 消息体 CVideosourcePresetcfgAddRsp
    UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ,   // 消息体 CVideosourcePresetcfgAddNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP,   // 消息体 CNtfRsp
    UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ,   // 消息体 CVideosourcePresetcfgDelReq
    UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP,   // 消息体 CVideosourcePresetcfgDelRsp
    UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ,   // 消息体 CVideosourcePresetcfgDelNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP,   // 消息体 CNtfRsp
    UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ,   // 消息体 CVideosourcePresetcfgQryReq
    UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP,   // 消息体 CVideosourcePresetcfgQryRsp
    UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ,   // 消息体 CVideosourcePresetcfgQryNtfReq
    UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP,   // 消息体 CNtfRsp

    UAS_CONFIG_UAS_SET_REQ,   // 消息体 CConfigUasSetReq  uas配置(包括: a. 日志保存参数配置)
    UAS_CONFIG_UAS_SET_RSP,   // 消息体 CConfigUasSetRsp
    UAS_CONFIG_UAS_GET_REQ,   // 消息体 CConfigUasGetReq
    UAS_CONFIG_UAS_GET_RSP,   // 消息体 CConfigUasGetRsp
};

inline void InitUasEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(UAS_LOGIN_REQ);                           // 消息体 CLoginUasReq  登录管理, 已经废弃。请采用统一的外设登录信令
    OSP_ADD_EVENT_DESC(UAS_LOGIN_RSP);                           // 消息体 CLoginUasRsp
    OSP_ADD_EVENT_DESC(UAS_LOGOUT_REQ);                          // 消息体 CLogoutUasReq
    OSP_ADD_EVENT_DESC(UAS_LOGOUT_RSP);                          // 消息体 CLogoutUasRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_ADD_REQ);                 // 消息体 COperateLogAddReq 操作日志管理
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_ADD_RSP);                 // 消息体 COperateLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_REQ);                 // 消息体 COperateLogQryReq
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_RSP);                 // 消息体 COperateLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_NTF_REQ);             // 消息体 COperateLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_OPERATE_LOG_QRY_NTF_RSP);             // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_ADD_REQ);                  // 消息体 CDeviceLogAddReq 设备日志管理
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_ADD_RSP);                  // 消息体 CDeviceLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_REQ);                  // 消息体 CDeviceLogQryReq
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_RSP);                  // 消息体 CDeviceLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_NTF_REQ);              // 消息体 CDeviceLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_DEVICE_LOG_QRY_NTF_RSP);              // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_ADD_REQ);                   // 消息体 CAlarmLogAddReq 告警日志管理
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_ADD_RSP);                   // 消息体 CAlarmLogAddRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_REQ);                   // 消息体 CAlarmLogQryReq
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_RSP);                   // 消息体 CAlarmLogQryRsp
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_NTF_REQ);               // 消息体 CAlarmLogQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_ALARM_LOG_QRY_NTF_RSP);               // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_REQ);            // 消息体 CUserCustomDataAddReq  自定义数据存在时进行修改
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_RSP);            // 消息体 CUserCustomDataAddRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_NTF_REQ);        // 消息体 CUserCustomDataAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_ADD_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_REQ);            // 消息体 CUserCustomDataDelReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_RSP);            // 消息体 CUserCustomDataDelRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_NTF_REQ);        // 消息体 CUserCustomDataDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_DEL_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_REQ);            // 消息体 CUserCustomDataQryReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_RSP);            // 消息体 CUserCustomDataQryRsp
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ);        // 消息体 CUserCustomDataQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_CUSER_CUSTOMDATA_QRY_NTF_RSP);        // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_REQ);        // 消息体 CVideosourceBasiccfgAddReq   配置存在时进行修改
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_RSP);        // 消息体 CVideosourceBasiccfgAddRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_REQ);    // 消息体 CVideosourceBasiccfgAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_ADD_NTF_RSP);    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_REQ);        // 消息体 CVideosourceBasiccfgDelReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_RSP);        // 消息体 CVideosourceBasiccfgDelRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_REQ);    // 消息体 CVideosourceBasiccfgDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_DEL_NTF_RSP);    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_REQ);        // 消息体 CVideosourceBasiccfgQryReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_RSP);        // 消息体 CVideosourceBasiccfgQryRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_REQ);    // 消息体 CVideosourceBasiccfgQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_BASICCFG_QRY_NTF_RSP);    // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ);       // 消息体 CVideosourcePresetcfgAddReq   配置存在时进行修改
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP);       // 消息体 CVideosourcePresetcfgAddRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_REQ);   // 消息体 CVideosourcePresetcfgAddNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_ADD_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ);       // 消息体 CVideosourcePresetcfgDelReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP);       // 消息体 CVideosourcePresetcfgDelRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_REQ);   // 消息体 CVideosourcePresetcfgDelNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_DEL_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ);       // 消息体 CVideosourcePresetcfgQryReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP);       // 消息体 CVideosourcePresetcfgQryRsp
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ);   // 消息体 CVideosourcePresetcfgQryNtfReq
    OSP_ADD_EVENT_DESC(UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_RSP);   // 消息体 CNtfRsp
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_SET_REQ);                  // 消息体 CConfigUasSetReq  uas配置(包括: a. 日志保存参数配置)
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_SET_RSP);                  // 消息体 CConfigUasSetRsp
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_GET_REQ);                  // 消息体 CConfigUasGetReq
    OSP_ADD_EVENT_DESC(UAS_CONFIG_UAS_GET_RSP);                  // 消息体 CConfigUasGetRsp
}

#endif // _UAS_EVENT_H_










