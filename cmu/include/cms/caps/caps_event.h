/*****************************************************************************
   ģ����      : caps_event
   �ļ���      : caps_event.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: caps_event.h ����������CAPS����Ϣ
   ����        : ��֮��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2015-01-16   1.0          ��֮��        ����
******************************************************************************/
#pragma once

#ifndef __CAPS_EVENT_H__
#define __CAPS_EVENT_H__
#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"
#include "cms/cmu/cmu_event.h"

enum ECAPS_EVENT
{
	CAPS_EVENT_BEGIN1 = CAPS_EVENT_BEGIN,	//��Ϣ�壺 

	// ��ȡ����
	CAPS_GET_CONFIG_REQ,	// CGetCAPSConfigReq
	CAPS_GET_CONFIG_RSP,	// CGetCAPSConfigRsp

	// ��������.
	CAPS_SET_CONFIG_REQ,	// CSetCAPSConfigReq
	CAPS_SET_CONFIG_RSP,	// CSetCAPSConfigRsp

	// ��ȡ�Ѿ������ͨ���б�.
	CAPS_QUERY_DEVICE_LIST_REQ,		// CQueryDeviceListReq
	CAPS_QUERY_DEVICE_LIST_RSP,		// CQueryDeviceListRsp

	CAPS_QUERY_DEVICE_LIST_NTF_REQ,	// CQueryDeviceListNtfReq
	CAPS_QUERY_DEVICE_LIST_NTF_RSP,	// CQueryDeviceListNtfRsp

	// ����һ��ͨ��.
	CAPS_ADD_DEVICE_REQ,			// CAddDeviceReq
	CAPS_ADD_DEVICE_RSP,			// CAddDeviceRsp

	// �޸�һ��ͨ��.
	CAPS_MODIFY_DEVICE_REQ,			// CModifyDeviceRsp
	CAPS_MODIFY_DEVICE_RSP,			// CModifyDeviceRsp

	// ɾ��һ��ͨ��
	CAPS_DEL_DEVICE_REQ,			// CDelDeviceReq
	CAPS_DEL_DEVICE_RSP,			// CDelDeviceRsp

	//��ָ��ͨ��ץ��һ��ͼƬ
	CAPS_TAKE_PICTURE_REQ,		//CTakePictureReq
	CAPS_TAKE_PICTURE_RSP,		//CTakePictureRsp

    CAPS_CURRENT_EVENT_END
};

// �ڲ���Ϣ����.
enum ECAPSInnerEvent
{
	CAPS_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

	CAPS_START,
	CAPS_QUIT,
	
	CAPS_START_CAPTURE_MNG_TASK,	// ����Ϣ�壬����ץ�Ĺ���task
	CAPS_INTERVAL_CHANGED,						//����Ϣ�壬ֻ��ץ�ļ��ʱ��仯�ˣ�����Ҫ����һ��

	// ��������Ϣ.
	CAPS_PRINT_CONF,
	CAPS_PRINT_STAT,
	CAPS_PRINT_PU_LIST,

	//���ڴ��е��ѻ����豸����д�뵽�ļ�
	CAPS_SAVE_DEVINFO_TO_FILE,
	CAPS_SAVE_DEVINFO_TIMER,

	CAPS_INNER_EVENT_END,
};



inline void InitCapsEventDesc()
{
    //���ģ���ظ�����ʱ��ֻ���ʼ��һ��
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(CAPS_EVENT_BEGIN1);                       //��Ϣ�壺 
    OSP_ADD_EVENT_DESC(CAPS_GET_CONFIG_REQ);                     // CGetCAPSConfigReq
    OSP_ADD_EVENT_DESC(CAPS_GET_CONFIG_RSP);                     // CGetCAPSConfigRsp
    OSP_ADD_EVENT_DESC(CAPS_SET_CONFIG_REQ);                     // CSetCAPSConfigReq
    OSP_ADD_EVENT_DESC(CAPS_SET_CONFIG_RSP);                     // CSetCAPSConfigRsp
    OSP_ADD_EVENT_DESC(CAPS_QUERY_DEVICE_LIST_REQ);              // CQueryDeviceListReq
    OSP_ADD_EVENT_DESC(CAPS_QUERY_DEVICE_LIST_RSP);              // CQueryDeviceListRsp
    OSP_ADD_EVENT_DESC(CAPS_QUERY_DEVICE_LIST_NTF_REQ);          // CQueryDeviceListNtfReq
    OSP_ADD_EVENT_DESC(CAPS_QUERY_DEVICE_LIST_NTF_RSP);          // CQueryDeviceListNtfRsp
    OSP_ADD_EVENT_DESC(CAPS_ADD_DEVICE_REQ);                     // CAddDeviceReq
    OSP_ADD_EVENT_DESC(CAPS_ADD_DEVICE_RSP);                     // CAddDeviceRsp
    OSP_ADD_EVENT_DESC(CAPS_MODIFY_DEVICE_REQ);                  // CModifyDeviceRsp
    OSP_ADD_EVENT_DESC(CAPS_MODIFY_DEVICE_RSP);                  // CModifyDeviceRsp
    OSP_ADD_EVENT_DESC(CAPS_DEL_DEVICE_REQ);                     // CDelDeviceReq
    OSP_ADD_EVENT_DESC(CAPS_DEL_DEVICE_RSP);                     // CDelDeviceRsp
    OSP_ADD_EVENT_DESC(CAPS_TAKE_PICTURE_REQ);                   //CTakePictureReq
    OSP_ADD_EVENT_DESC(CAPS_TAKE_PICTURE_RSP);                   //CTakePictureRsp
    OSP_ADD_EVENT_DESC(CAPS_CURRENT_EVENT_END);                  
    OSP_ADD_EVENT_DESC(CAPS_INNER_EVENT_BEGIN);                  
    OSP_ADD_EVENT_DESC(CAPS_START);                              
    OSP_ADD_EVENT_DESC(CAPS_QUIT);                               
    OSP_ADD_EVENT_DESC(CAPS_START_CAPTURE_MNG_TASK);             // ����Ϣ�壬����ץ�Ĺ���task
    OSP_ADD_EVENT_DESC(CAPS_INTERVAL_CHANGED);                   //����Ϣ�壬ֻ��ץ�ļ��ʱ��仯�ˣ�����Ҫ����һ��
    OSP_ADD_EVENT_DESC(CAPS_PRINT_CONF);                         
    OSP_ADD_EVENT_DESC(CAPS_PRINT_STAT);                         
    OSP_ADD_EVENT_DESC(CAPS_PRINT_PU_LIST);                      
    OSP_ADD_EVENT_DESC(CAPS_SAVE_DEVINFO_TO_FILE);               
    OSP_ADD_EVENT_DESC(CAPS_SAVE_DEVINFO_TIMER);                 
    OSP_ADD_EVENT_DESC(CAPS_INNER_EVENT_END);                    
}


#endif  //#ifndef __CAPS_EVENT_H__



























