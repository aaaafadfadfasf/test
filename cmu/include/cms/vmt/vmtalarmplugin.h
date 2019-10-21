/*****************************************************************************
模块名      : VMT
文件名      : vmt.h
相关文件    : 
文件实现功能: VMT模块对外接口定义
作者        : 
版本        : Copyright(C) 2009-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2010/04/28  1.0                      create          
******************************************************************************/

#ifndef __VMT_ALARM_PLUGIN_H__
#define __VMT_ALARM_PLUGIN_H__

#include "osp.h"
// #ifdef WIN32
//     #include <windows.h>
//     #include <stdafx>
// #endif
#ifdef WIN32
#include <atlstr.h> //modify by Royan 20121009
#endif

//告警类型
#define ALARM_INPUTPIN  (10002)   //并口输入告警
#define ALARM_MOVE      (10003)   //移动侦测
#define ALARM_DISKFULL  (10004)   //磁盘满告警
#define ALARM_VIDEOLOST (10005)   //视频源丢失
#define ALARM_INTELLIGE (10000)   // 智能告警

// EVMonitor 验证成功与否的类型
#define EV_MTI_ALARM_SECURE_SUCC  (10009)   // EV用户验证成功
#define EV_MTI_ALARM_SECURE_FAIL  (10010)   // EV用户验证失败
// evMonitor 连接服务器成功与否
#define  EV_MTI_ALARM_CONN_SUCC    (10011)  // 连接服务器成功
#define  EV_MTI_ALARM_CONN_FAIL    (10012)  // 连接服务器失败
//告警开关
#define ALARM_OCCUR     (0)       //告警产生
#define ALARM_RESTORE   (1)       //告警恢复

#define ERROR_LEVEL  	1	      //错误
#define WARNING_LEVEL	2	      //警告
#define EVENT_LEVEL	    3	      //普通事件
#define ALL_LEVEL	    4	      //所有级别

enum enumAlgoType
{
	    ALGO_TRIPWIRE		= 0,
		ALGO_REMOVAL		= 1,
		ALGO_CAMERA			= 2,
		ALGO_TRACKING		= 3,
		ALGO_PTZ			= 4,
		ALGO_UNATTENDEDOBJ  = 5,
		ALGO_FACEDETECT		= 6,
		ALGO_LPR			= 7,
		ALGO_RISEDETECT		= 8,
		ALGO_TRIPWIRE_INDOOR= 9,
		ALGO_ONEMETERLINE   = 10,
		ALGO_AREADETECT     = 11,
		ALGO_SUPERVISOR		= 12,
		ALGO_WHISPER		= 13,
		ALGO_AUTOVIDEO      = 14,
		ALGO_EVLPR          = 15,
		ALGO_EVBANKNOTE     = 16,
		ALGO_EVWANDER       = 17,
		ALGO_FIGHT          = 18,
		ALGO_PERSONMONITOR  = 19,
		ALGO_PERSONCOUNT    = 20,
		ALGO_PERSONCARCLASSIFY = 21,
		ALGO_HeadlightDETECT = 22,
		ALGO_DENSITY         = 23,
		ALGO_COUNTER         = 24,
		ALGO_METROTICKETGATE = 25,
		ALGO_TYPE_COUNT      = 26
};
// 告警类型 
 #define  ALARM_TYPE_INVALID   (0)   // 无效
// #define  ALARM_TYPE_VEHICLE   (0)    // 车牌
// #define  ALARM_TYPE_FACE      (1)    // 人脸
// #define  ALARM_TYPE_OTHER     (2)    // 其他


#define  PUID_LEN       (33)
#define  MAX_CHANNEL_LENGTH  (4)

#define  MAX_ASIA_LEN          (128)
#define  MAX_CHN_NAME_LEN     (128)
typedef struct tag_TTelegentAlarmInfo  
{	
	s8  m_szChannelID[MAX_CHANNEL_LENGTH];  //  channel id
	s8  m_szChnName[MAX_CHN_NAME_LEN];   // channel name
	s8  m_szAsia[MAX_ASIA_LEN];    // 别名
	s8  m_szPuid[PUID_LEN]; // puid
	u8  m_byAlarmType; // vehicle or face recognize
	u8  m_byMatchFlag; // white black others 0 white 1 black 2 others
	u8  m_byState; // occure or recover
	tag_TTelegentAlarmInfo()
	{
		memset(this, 0, sizeof(tag_TTelegentAlarmInfo));
	}
}TTelegentAlarmInfo, *PTTelegentAlarmInfo;

#define  MAX_PU_CHAN_LENGTH  (36)

// 初始登录上报各个通道的状态以及Puid和通道的对应关系
typedef struct tagPuInfoReport
{
	s8 m_szPuChn[MAX_PU_CHAN_LENGTH];     // puid
	s8 m_szAsia[MAX_ASIA_LEN];            //别名
	s8 m_szChnName[MAX_CHN_NAME_LEN];     // 通道名
	s32 m_nChannelID;
 	s32 m_nDeviceID;   
	tagPuInfoReport()
	{
		memset(this, 0, sizeof(tagPuInfoReport));
	}
}TPuInfoReport, *PTPuInfoReport;

/*=================================================================
函数说明:  InitAlarmSource
功    能:  初始化插件告警源
参    数:  u32 dwMaxPuNum               最大Pu个数
           u32 dwMaxAlarmPin            每个Pu最大并口输入个数
           u32 dwMaxMotionDetect        每个Pu最大移动侦测个数
           const char *lpszConfigFile   插件配置文件
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef BOOL32 (*lpInitAlarmSource)(u32 dwMaxPuNum, u32 dwMaxAlarmPin, u32 dwMaxMotionDetect, const char *lpszConfigFile);

/*=================================================================
函数说明:  CloseAlarmSource
功    能:  关闭告警源
参    数:  无
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef BOOL32 (*lpCloseAlarmSource)(void);

/*=================================================================
函数说明:  SetAlarmCallBack
功    能:  设置告警通知回调
参    数:  PAlarmNotify pcbAlarmNotify 设置告警回调通知
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef void (*PAlarmNotify)(u32 dwPuIndex, u16 wAlarmInputChnId, u32 dwAlarmType, BOOL32 bAlarm, void *pContext, u8* pbyPuid);
typedef BOOL32 (*lpSetAlarmCallBack)(PAlarmNotify, void *);


/*=================================================================
函数说明:  SetTelegentAlarmCB
功    能:  设置智能告警通知回调
参    数:  PAlarmNotify pcbAlarmNotify 设置告警回调通知
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef void (*PTelegentAlarmNotify)(TTelegentAlarmInfo tTelegentAlarmInfo, void *pContext);
typedef BOOL32 (*lpSetTelegentAlarmCB)(PTelegentAlarmNotify, void *);


/*=================================================================
函数说明:  SetTelegentAlarmCB
功    能:  设置通道配置信息获取回调
参    数:  PAlarmNotify pcbAlarmNotify 设置告警回调通知
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef void (*PPuInfoNotify)(TPuInfoReport tTPuReportInfo, void *pContext);
typedef BOOL32 (*lpSetPuInfoCB)(PPuInfoNotify, void *);


/*=================================================================
函数说明:  SetLogCallBack
功    能:  设置告警日志回调
参    数:  PAlarmLog pcbAlarmLog 设置告警日志回调打印
返 回 值:  TRUE表示成功, FALSE表示失败
==================================================================*/
typedef void (*PAlarmLog)(u8 byLogLevel, const char* szFormat, ...);
typedef BOOL32 (*lpSetLogCallBack)(PAlarmLog);

typedef void (*PEnableStatus)(u32* pData, u16 wLength, void* pContent);
typedef void (*pSetEnableStatusCB)(PEnableStatus);

#ifdef WIN32
//宏定义函数指针类型
typedef int(*lpDtoBFun)(CString); 
#endif

#endif//__VMT_ALARM_PLUGIN_H__