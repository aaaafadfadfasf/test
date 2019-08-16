#pragma once

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"
#include "cms/cmu/cmu_event.h"

enum EDPS_EVENT
{
	DPS_EVENT_BEGIN1 = DPS_EVENT_BEGIN,	// 28501 消息体： 

	//使用INVITE方法实现搜索
	DPS_SEARCH_REQ,          // 28502 消息体: CDPSSearchReq
	DPS_SEARCH_RSP,          // 28503 消息体: CDPSSearchRsp
	DPS_SEARCH_ACK,          // 28504 消息体: null //CDPSSearchAck

	DPS_SEARCH_PROGRESS_NTF_REQ,         // 28505 消息体: CDPSSearchProgressNtfReq // 不确定是否需要.
	DPS_SEARCH_PROGRESS_NTF_RSP,         // 28506 消息体: CDPSSearchProgressNtfRsp	// 不确定是否需要.

	DPS_SEARCH_GET_RESULT_REQ,			 // 28507 消息体: CDPSGetResultReq 应答 DPS_SEARCH_GET_RESULT_RSP, 通知: DPS_SEARCH_GET_RESULT_NTF_REQ
	DPS_SEARCH_GET_RESULT_RSP,			// 28508 消息体: CDPSGetResultRsp

	DPS_SEARCH_GET_RESULT_NTF_REQ,		// 28509 消息体: CDPSGetResultNtfReq	
	DPS_SEARCH_GET_RESULT_NTF_RSP,		// 28510 消息体: CDPSGetResultNtfRsp

	// 按需获取结果集.
	DPS_SEARCH_GET_RESULT_BY_GROUP_REQ,			 // 28511 消息体: CDPSGetResultByGroupReq 应答 DPS_SEARCH_GET_RESULT_RSP, 通知: DPS_SEARCH_GET_RESULT_NTF_REQ
	DPS_SEARCH_GET_RESULT_BY_GROUP_RSP,			// 28512 消息体: CDPSGetResultByGroupRsp
	
	DPS_GET_DEVICE_STATUS_REQ,			//  28513 获取设备状态.	CDPSGetDeviceStatusReq
	DPS_GET_DEVICE_STATUS_RSP,			// 28514 获取设备状态应答.	CDPSGetDeviceStatusRsp

	DPS_FILTER_EMPTY_DEVICE_GROUP_REQ,	// 28515 获取非空设备列表.	CDPS
	DPS_FILTER_EMPTY_DEVICE_GROUP_RSP,	// 28516 获取非空设备列表

	DPS_PRIVATE_TEST_CMD,				//  28517 DPS内部消息,测试搜索, 发给CTestTask.
	DPS_PRIVATE_TIMER,					// 28518 DPS内部定时器.

	DPS_PRINT_CONFIG,					// 28519 内部消息,打印配置.
	DPS_PRINT_SUBLIST,					// 28520 内部消息,打印订阅列表.

	DPS_QUIT,							// 28521 内部消息,正常退出进程.


	DPS_EVENT_END1 = DPS_EVENT_END,		
};


inline void InitDPSEvent()
{
	//多个模块重复调用时，只需初始化一次
	static bool bInit = false;
	if (bInit)
	{
		return;
	}
	bInit = true;

	OSP_ADD_EVENT_DESC(DPS_SEARCH_REQ);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_RSP);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_ACK);

	OSP_ADD_EVENT_DESC(DPS_SEARCH_PROGRESS_NTF_REQ);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_PROGRESS_NTF_RSP);

	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_REQ);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_RSP);

	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_NTF_REQ);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_NTF_RSP);

	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_BY_GROUP_REQ);
	OSP_ADD_EVENT_DESC(DPS_SEARCH_GET_RESULT_BY_GROUP_RSP);

	OSP_ADD_EVENT_DESC(DPS_GET_DEVICE_STATUS_REQ);
	OSP_ADD_EVENT_DESC(DPS_GET_DEVICE_STATUS_RSP);

	OSP_ADD_EVENT_DESC(DPS_FILTER_EMPTY_DEVICE_GROUP_REQ);
	OSP_ADD_EVENT_DESC(DPS_FILTER_EMPTY_DEVICE_GROUP_RSP);

	OSP_ADD_EVENT_DESC(DPS_PRIVATE_TEST_CMD);
	OSP_ADD_EVENT_DESC(DPS_PRIVATE_TIMER);
	OSP_ADD_EVENT_DESC(DPS_PRINT_CONFIG);
	OSP_ADD_EVENT_DESC(DPS_PRINT_SUBLIST);
	OSP_ADD_EVENT_DESC(DPS_QUIT);
}

// DPS服务状态的Redis Key定义
#define  KEY_DPS_SERVICE_STATUS "dps_service_status"

#define	FIELD_DPS_DEVICE_GROUP_STAT_SERVICE "dps_device_group_stat"		// 设备组统计服务.
#define  FIELD_DPS_DEVICE_GROUP_STAT_SERVICE_CHECK_TIME "dps_device_group_stat_last_check_time"	// 最近一次检测的时间.

#define  KEY_DEVICE_GROUP_SUB_MSG_QUEUE_KEY_SET "DeviceGroupSubMsgQueueKeySet"

// 获取设备组统计订阅的消息队列名称.
inline std::string MakeDeviceGroupSubMsgQueueKey(const std::string& cuiId, const std::string& userId, const std::string& treeId, const std::string& sessionId)
{
	return std::string("DeviceGroupSubMsgQueue:") + cuiId + ":" + userId + ":" + treeId + ":" + sessionId;
}

// 拆分字符串
inline std::vector<std::string> StringSplit(const std::string& str, const std::string& token)
{
	std::vector< std::string > splitedArray;
	int posBegin = 0;
	while (1)
	{
		int posEnd = str.find(token, posBegin);
		std::string tmp = str.substr(posBegin, posEnd==std::string::npos ? posEnd : (posEnd - posBegin) );
		splitedArray.push_back(tmp);

		if (posEnd == std::string::npos)
		{
			break;
		}
		posBegin = posEnd + token.size();
	} ;

	return splitedArray;
}

// 解析消息队列的名称.
inline bool ParseDeviceGroupSubMsgQueueKey(const std::string& key
	, std::string& cuiId, std::string& userId, std::string& treeId, std::string& sessionId)
{
	std::vector<std::string> splitedArray = StringSplit(key, ":");
	if ( splitedArray.size() == 5 )
	{
		cuiId = splitedArray[1];
		userId = splitedArray[2];
		treeId = splitedArray[3];
		sessionId = splitedArray[4];
		return true;
	}
	else
	{
		return false;
	}
}

// 获取设备组统计通知订阅/发布的通道名称.
inline std::string MakeDeviceGroupStatNotifyChnName(const std::string& userId, const std::string& treeId, const std::string& sessionId)
{
	return std::string("DeviceGroupStatNotify:") + userId + ":" + treeId + ":" + sessionId;
}

// 