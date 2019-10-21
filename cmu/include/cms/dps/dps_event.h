#pragma once

#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"
#include "cms/cmu/cmu_event.h"

enum EDPS_EVENT
{
	DPS_EVENT_BEGIN1 = DPS_EVENT_BEGIN,	// 28501 ��Ϣ�壺 

	//ʹ��INVITE����ʵ������
	DPS_SEARCH_REQ,          // 28502 ��Ϣ��: CDPSSearchReq
	DPS_SEARCH_RSP,          // 28503 ��Ϣ��: CDPSSearchRsp
	DPS_SEARCH_ACK,          // 28504 ��Ϣ��: null //CDPSSearchAck

	DPS_SEARCH_PROGRESS_NTF_REQ,         // 28505 ��Ϣ��: CDPSSearchProgressNtfReq // ��ȷ���Ƿ���Ҫ.
	DPS_SEARCH_PROGRESS_NTF_RSP,         // 28506 ��Ϣ��: CDPSSearchProgressNtfRsp	// ��ȷ���Ƿ���Ҫ.

	DPS_SEARCH_GET_RESULT_REQ,			 // 28507 ��Ϣ��: CDPSGetResultReq Ӧ�� DPS_SEARCH_GET_RESULT_RSP, ֪ͨ: DPS_SEARCH_GET_RESULT_NTF_REQ
	DPS_SEARCH_GET_RESULT_RSP,			// 28508 ��Ϣ��: CDPSGetResultRsp

	DPS_SEARCH_GET_RESULT_NTF_REQ,		// 28509 ��Ϣ��: CDPSGetResultNtfReq	
	DPS_SEARCH_GET_RESULT_NTF_RSP,		// 28510 ��Ϣ��: CDPSGetResultNtfRsp

	// �����ȡ�����.
	DPS_SEARCH_GET_RESULT_BY_GROUP_REQ,			 // 28511 ��Ϣ��: CDPSGetResultByGroupReq Ӧ�� DPS_SEARCH_GET_RESULT_RSP, ֪ͨ: DPS_SEARCH_GET_RESULT_NTF_REQ
	DPS_SEARCH_GET_RESULT_BY_GROUP_RSP,			// 28512 ��Ϣ��: CDPSGetResultByGroupRsp
	
	DPS_GET_DEVICE_STATUS_REQ,			//  28513 ��ȡ�豸״̬.	CDPSGetDeviceStatusReq
	DPS_GET_DEVICE_STATUS_RSP,			// 28514 ��ȡ�豸״̬Ӧ��.	CDPSGetDeviceStatusRsp

	DPS_FILTER_EMPTY_DEVICE_GROUP_REQ,	// 28515 ��ȡ�ǿ��豸�б�.	CDPS
	DPS_FILTER_EMPTY_DEVICE_GROUP_RSP,	// 28516 ��ȡ�ǿ��豸�б�

	DPS_PRIVATE_TEST_CMD,				//  28517 DPS�ڲ���Ϣ,��������, ����CTestTask.
	DPS_PRIVATE_TIMER,					// 28518 DPS�ڲ���ʱ��.

	DPS_PRINT_CONFIG,					// 28519 �ڲ���Ϣ,��ӡ����.
	DPS_PRINT_SUBLIST,					// 28520 �ڲ���Ϣ,��ӡ�����б�.

	DPS_QUIT,							// 28521 �ڲ���Ϣ,�����˳�����.


	DPS_EVENT_END1 = DPS_EVENT_END,		
};


inline void InitDPSEvent()
{
	//���ģ���ظ�����ʱ��ֻ���ʼ��һ��
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

// DPS����״̬��Redis Key����
#define  KEY_DPS_SERVICE_STATUS "dps_service_status"

#define	FIELD_DPS_DEVICE_GROUP_STAT_SERVICE "dps_device_group_stat"		// �豸��ͳ�Ʒ���.
#define  FIELD_DPS_DEVICE_GROUP_STAT_SERVICE_CHECK_TIME "dps_device_group_stat_last_check_time"	// ���һ�μ���ʱ��.

#define  KEY_DEVICE_GROUP_SUB_MSG_QUEUE_KEY_SET "DeviceGroupSubMsgQueueKeySet"

// ��ȡ�豸��ͳ�ƶ��ĵ���Ϣ��������.
inline std::string MakeDeviceGroupSubMsgQueueKey(const std::string& cuiId, const std::string& userId, const std::string& treeId, const std::string& sessionId)
{
	return std::string("DeviceGroupSubMsgQueue:") + cuiId + ":" + userId + ":" + treeId + ":" + sessionId;
}

// ����ַ���
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

// ������Ϣ���е�����.
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

// ��ȡ�豸��ͳ��֪ͨ����/������ͨ������.
inline std::string MakeDeviceGroupStatNotifyChnName(const std::string& userId, const std::string& treeId, const std::string& sessionId)
{
	return std::string("DeviceGroupStatNotify:") + userId + ":" + treeId + ":" + sessionId;
}

// 