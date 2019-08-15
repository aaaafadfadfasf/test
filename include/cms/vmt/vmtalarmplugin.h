/*****************************************************************************
ģ����      : VMT
�ļ���      : vmt.h
����ļ�    : 
�ļ�ʵ�ֹ���: VMTģ�����ӿڶ���
����        : 
�汾        : Copyright(C) 2009-2010 Suzhou Keda Technology Co.Ltd
              All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
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

//�澯����
#define ALARM_INPUTPIN  (10002)   //��������澯
#define ALARM_MOVE      (10003)   //�ƶ����
#define ALARM_DISKFULL  (10004)   //�������澯
#define ALARM_VIDEOLOST (10005)   //��ƵԴ��ʧ
#define ALARM_INTELLIGE (10000)   // ���ܸ澯

// EVMonitor ��֤�ɹ���������
#define EV_MTI_ALARM_SECURE_SUCC  (10009)   // EV�û���֤�ɹ�
#define EV_MTI_ALARM_SECURE_FAIL  (10010)   // EV�û���֤ʧ��
// evMonitor ���ӷ������ɹ����
#define  EV_MTI_ALARM_CONN_SUCC    (10011)  // ���ӷ������ɹ�
#define  EV_MTI_ALARM_CONN_FAIL    (10012)  // ���ӷ�����ʧ��
//�澯����
#define ALARM_OCCUR     (0)       //�澯����
#define ALARM_RESTORE   (1)       //�澯�ָ�

#define ERROR_LEVEL  	1	      //����
#define WARNING_LEVEL	2	      //����
#define EVENT_LEVEL	    3	      //��ͨ�¼�
#define ALL_LEVEL	    4	      //���м���

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
// �澯���� 
 #define  ALARM_TYPE_INVALID   (0)   // ��Ч
// #define  ALARM_TYPE_VEHICLE   (0)    // ����
// #define  ALARM_TYPE_FACE      (1)    // ����
// #define  ALARM_TYPE_OTHER     (2)    // ����


#define  PUID_LEN       (33)
#define  MAX_CHANNEL_LENGTH  (4)

#define  MAX_ASIA_LEN          (128)
#define  MAX_CHN_NAME_LEN     (128)
typedef struct tag_TTelegentAlarmInfo  
{	
	s8  m_szChannelID[MAX_CHANNEL_LENGTH];  //  channel id
	s8  m_szChnName[MAX_CHN_NAME_LEN];   // channel name
	s8  m_szAsia[MAX_ASIA_LEN];    // ����
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

// ��ʼ��¼�ϱ�����ͨ����״̬�Լ�Puid��ͨ���Ķ�Ӧ��ϵ
typedef struct tagPuInfoReport
{
	s8 m_szPuChn[MAX_PU_CHAN_LENGTH];     // puid
	s8 m_szAsia[MAX_ASIA_LEN];            //����
	s8 m_szChnName[MAX_CHN_NAME_LEN];     // ͨ����
	s32 m_nChannelID;
 	s32 m_nDeviceID;   
	tagPuInfoReport()
	{
		memset(this, 0, sizeof(tagPuInfoReport));
	}
}TPuInfoReport, *PTPuInfoReport;

/*=================================================================
����˵��:  InitAlarmSource
��    ��:  ��ʼ������澯Դ
��    ��:  u32 dwMaxPuNum               ���Pu����
           u32 dwMaxAlarmPin            ÿ��Pu��󲢿��������
           u32 dwMaxMotionDetect        ÿ��Pu����ƶ�������
           const char *lpszConfigFile   ��������ļ�
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef BOOL32 (*lpInitAlarmSource)(u32 dwMaxPuNum, u32 dwMaxAlarmPin, u32 dwMaxMotionDetect, const char *lpszConfigFile);

/*=================================================================
����˵��:  CloseAlarmSource
��    ��:  �رո澯Դ
��    ��:  ��
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef BOOL32 (*lpCloseAlarmSource)(void);

/*=================================================================
����˵��:  SetAlarmCallBack
��    ��:  ���ø澯֪ͨ�ص�
��    ��:  PAlarmNotify pcbAlarmNotify ���ø澯�ص�֪ͨ
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef void (*PAlarmNotify)(u32 dwPuIndex, u16 wAlarmInputChnId, u32 dwAlarmType, BOOL32 bAlarm, void *pContext, u8* pbyPuid);
typedef BOOL32 (*lpSetAlarmCallBack)(PAlarmNotify, void *);


/*=================================================================
����˵��:  SetTelegentAlarmCB
��    ��:  �������ܸ澯֪ͨ�ص�
��    ��:  PAlarmNotify pcbAlarmNotify ���ø澯�ص�֪ͨ
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef void (*PTelegentAlarmNotify)(TTelegentAlarmInfo tTelegentAlarmInfo, void *pContext);
typedef BOOL32 (*lpSetTelegentAlarmCB)(PTelegentAlarmNotify, void *);


/*=================================================================
����˵��:  SetTelegentAlarmCB
��    ��:  ����ͨ��������Ϣ��ȡ�ص�
��    ��:  PAlarmNotify pcbAlarmNotify ���ø澯�ص�֪ͨ
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef void (*PPuInfoNotify)(TPuInfoReport tTPuReportInfo, void *pContext);
typedef BOOL32 (*lpSetPuInfoCB)(PPuInfoNotify, void *);


/*=================================================================
����˵��:  SetLogCallBack
��    ��:  ���ø澯��־�ص�
��    ��:  PAlarmLog pcbAlarmLog ���ø澯��־�ص���ӡ
�� �� ֵ:  TRUE��ʾ�ɹ�, FALSE��ʾʧ��
==================================================================*/
typedef void (*PAlarmLog)(u8 byLogLevel, const char* szFormat, ...);
typedef BOOL32 (*lpSetLogCallBack)(PAlarmLog);

typedef void (*PEnableStatus)(u32* pData, u16 wLength, void* pContent);
typedef void (*pSetEnableStatusCB)(PEnableStatus);

#ifdef WIN32
//�궨�庯��ָ������
typedef int(*lpDtoBFun)(CString); 
#endif

#endif//__VMT_ALARM_PLUGIN_H__