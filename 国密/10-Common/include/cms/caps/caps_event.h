/*****************************************************************************
   模块名      : caps_event
   文件名      : caps_event.h
   相关文件    : 
   文件实现功能: caps_event.h 定义了所有CAPS的消息
   作者        : 李之兴
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2015-01-16   1.0          李之兴        创建
******************************************************************************/
#pragma once

#ifndef __CAPS_EVENT_H__
#define __CAPS_EVENT_H__
#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"
#include "cms/cmu/cmu_event.h"

enum ECAPS_EVENT
{
	CAPS_EVENT_BEGIN1 = CAPS_EVENT_BEGIN,	//消息体： 

	// 获取配置
	CAPS_GET_CONFIG_REQ,	// CGetCAPSConfigReq
	CAPS_GET_CONFIG_RSP,	// CGetCAPSConfigRsp

	// 设置配置.
	CAPS_SET_CONFIG_REQ,	// CSetCAPSConfigReq
	CAPS_SET_CONFIG_RSP,	// CSetCAPSConfigRsp

	// 获取已经划归的通道列表.
	CAPS_QUERY_DEVICE_LIST_REQ,		// CQueryDeviceListReq
	CAPS_QUERY_DEVICE_LIST_RSP,		// CQueryDeviceListRsp

	CAPS_QUERY_DEVICE_LIST_NTF_REQ,	// CQueryDeviceListNtfReq
	CAPS_QUERY_DEVICE_LIST_NTF_RSP,	// CQueryDeviceListNtfRsp

	// 划归一个通道.
	CAPS_ADD_DEVICE_REQ,			// CAddDeviceReq
	CAPS_ADD_DEVICE_RSP,			// CAddDeviceRsp

	// 修改一个通道.
	CAPS_MODIFY_DEVICE_REQ,			// CModifyDeviceRsp
	CAPS_MODIFY_DEVICE_RSP,			// CModifyDeviceRsp

	// 删除一个通道
	CAPS_DEL_DEVICE_REQ,			// CDelDeviceReq
	CAPS_DEL_DEVICE_RSP,			// CDelDeviceRsp

	//让指定通道抓拍一张图片
	CAPS_TAKE_PICTURE_REQ,		//CTakePictureReq
	CAPS_TAKE_PICTURE_RSP,		//CTakePictureRsp

    CAPS_CURRENT_EVENT_END
};

// 内部消息定义.
enum ECAPSInnerEvent
{
	CAPS_INNER_EVENT_BEGIN = USER_INNER_MSG_BEGIN,

	CAPS_START,
	CAPS_QUIT,
	
	CAPS_START_CAPTURE_MNG_TASK,	// 无消息体，启动抓拍管理task
	CAPS_INTERVAL_CHANGED,						//无消息体，只是抓拍间隔时间变化了，队列要重置一下

	// 调试用消息.
	CAPS_PRINT_CONF,
	CAPS_PRINT_STAT,
	CAPS_PRINT_PU_LIST,

	//将内存中的已划归设备参数写入到文件
	CAPS_SAVE_DEVINFO_TO_FILE,
	CAPS_SAVE_DEVINFO_TIMER,

	CAPS_INNER_EVENT_END,
};



inline void InitCapsEventDesc()
{
    //多个模块重复调用时，只需初始化一次
    static bool bInit = false;
    if(bInit)
    {
        return;
    }
    bInit = true;

    OSP_ADD_EVENT_DESC(CAPS_EVENT_BEGIN1);                       //消息体： 
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
    OSP_ADD_EVENT_DESC(CAPS_START_CAPTURE_MNG_TASK);             // 无消息体，启动抓拍管理task
    OSP_ADD_EVENT_DESC(CAPS_INTERVAL_CHANGED);                   //无消息体，只是抓拍间隔时间变化了，队列要重置一下
    OSP_ADD_EVENT_DESC(CAPS_PRINT_CONF);                         
    OSP_ADD_EVENT_DESC(CAPS_PRINT_STAT);                         
    OSP_ADD_EVENT_DESC(CAPS_PRINT_PU_LIST);                      
    OSP_ADD_EVENT_DESC(CAPS_SAVE_DEVINFO_TO_FILE);               
    OSP_ADD_EVENT_DESC(CAPS_SAVE_DEVINFO_TIMER);                 
    OSP_ADD_EVENT_DESC(CAPS_INNER_EVENT_END);                    
}


#endif  //#ifndef __CAPS_EVENT_H__



























