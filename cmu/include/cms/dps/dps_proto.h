/*****************************************************************************
ģ����      : dps
�ļ���      : dps_protocol.h
����ļ�    :
�ļ�ʵ�ֹ���: DPSģ���������xml��
����        : lizhixing
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2016-04-06   1.0        lizhixing        ����
******************************************************************************/

#pragma once

#include "cms/cms_proto.h"
#include "cms/dps/dps_event.h"
#include "cms/dps/dps_struct.h"
#include "cms/cmu/cmu_proto.h"

// ��ѯ����
class CDPSSearchReq : public CEventReq
{
public:
	CDPSSearchReq()
	{
		event = OspExtEventDesc( DPS_SEARCH_REQ );
		Clear();
	}

	void Clear()
	{
		key.clear();
		userId.clear();
		maxNum = -1;
		maxTimeMSec = -1;
		exportFile = false;
		CEventReq::Clear();
	}

	const string& Key()const { return key; }
	string& Key(){ return key; }

	int MaxNum() const{ return maxNum; }
	void SetMaxNum(int n){
		maxNum = n;
	};

	int MaxTimeMSec() const{ return maxTimeMSec; }
	void SetMaxTimeMSec(int n){
		maxTimeMSec = n;
	};

	const string& UserId()const{ return userId; }
	void SetUserId(const string& id){ userId = id; }

	bool IsExportFile()const {
		return this->exportFile;
	}

	void SetExportFile(bool ef){
		this->exportFile = ef;
	}
private:
	string key;	// �����ؼ���. 
	string userId;	// �����ߵ��û�Id.
	int maxNum;	// ���Ľ������С,�����ĺ���.
	int maxTimeMSec;// ��������ʱ,�������ʱ���ֹͣ����.

	bool exportFile;	// �Ƿ���Ҫ�����ļ�.
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};


// ��ѯӦ��
class CDPSSearchRsp : public CEventRsp
{
public:
	CDPSSearchRsp()
	{
		Clear();
		event = OspExtEventDesc(DPS_SEARCH_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();
		token.clear();
		exportFileUrl.clear();
	}

	const string& Token() const
	{
		return token;
	}

	string& Token()
	{
		return token;
	}

	const string& ExportFileUrl()const{
		return this->exportFileUrl;
	}

	string& ExportFileUrl(){
		return this->exportFileUrl;
	}
private:
	string token;							  // ������token.

	string exportFileUrl;	// �����ļ�������Url.
public:


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯACK
class CDPSSearchAck :public CInviteACK
{
public:
	CDPSSearchAck()
	{
		event = OspExtEventDesc( DPS_SEARCH_ACK);
	}

	void Clear()
	{
		CInviteACK::Clear();
	}

private:
public:


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ���Ȼ�ȡ
class CDPSSearchProgressNtfReq : public CEventReq
{
public:
	CDPSSearchProgressNtfReq()
	{
		event = OspExtEventDesc( DPS_SEARCH_PROGRESS_NTF_REQ );
		Clear();
	}

	void Clear()
	{
		percent = 0;
		errorCode = 0;
		CEventReq::Clear();
	}

private:
	int percent;						// ���Ȱٷֱ�[0-100]���ﵽ100ʱ
	int errorCode;						// 0�ǳɹ�, �����Ǵ�����.
public:
	void SetPercent(int nPercent)
	{
		percent = nPercent;
	}
	int GetPercent() const
	{
		return percent;
	}

	void SetErrorCode(int er)
	{
		this->errorCode = er;
	}

	int GetErrorCode()const
	{
		return this->errorCode;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CDPSSearchProgressNtfRsp : public CEventRsp
{
public:
	CDPSSearchProgressNtfRsp()
	{
		event = OspExtEventDesc(DPS_SEARCH_PROGRESS_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯ�������
class CDPSGetResultReq : public CQueryReqBase
{
public:
	CDPSGetResultReq()
	{
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_REQ);
		Clear();
	}

	void Clear()
	{
		CEventReq::Clear();
		token.clear();
	}
	
	string& SearchToken()
	{
		return this->token;
	}
	 
	const string& SearchToken() const{
		return this->token;
	}
private:
	string token;	// ����key.
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

// ��ѯӦ��
class CDPSGetResultRsp : public CQueryRspBase
{
public:
	CDPSGetResultRsp()
	{
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_RSP);
		Clear();
	}

	void Clear()
	{
		CEventRsp::Clear();
		lastPage = false;
	}

	bool IsLastPage()const { return this->lastPage; }
	void SetLastPage(bool l){ this->lastPage = l; }
private:
	bool lastPage;
public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ���豸���ѯ�������
class CDPSGetResultByGroupReq : public CQueryReqBase
{
public:
	CDPSGetResultByGroupReq()
	{
		Clear();
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_BY_GROUP_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
		groupId.clear();
		type = CDPSSearchResult::SearchResultTypeInvalid;
		token.clear();
	}

	string& GroupId()
	{
		return groupId;
	}

	const string& GroupId()const
	{
		return groupId;
	}

	int GetItemType() const
	{
		return type;
	}

	void SetItemType(int t)
	{
		type = t;
	}

	string& SearchToken()
	{
		return this->token;
	}

	const string& SearchToken() const{
		return this->token;
	}
private:
	string groupId;
	int type;		// ����, �豸��, ��ƵԴ, �豸.

	string token;	// ����key.
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

// ��ѯӦ��
class CDPSGetResultByGroupRsp : public CQueryRspBase
{
public:
	CDPSGetResultByGroupRsp()
	{
		Clear();
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_BY_GROUP_RSP);
	}

	void Clear()
	{
		CEventRsp::Clear();

		lastPage = false;
		allVideoSrcNum = 0;
		onlineVideoSrcNum = 0;
		disabledVideoSrcNum = 0;
	}

	bool IsLastPage()const { return this->lastPage; }
	void SetLastPage(bool l){ this->lastPage = l; }

	int GetOnlineVideoSrcNum() const { return onlineVideoSrcNum;  }
	void SetOnlineVideoSrcNum(int num) { onlineVideoSrcNum = num;  }

	int GetAllVideoSrcNum() const { return allVideoSrcNum; }
	void SetAllVideoSrcNum(int num) { allVideoSrcNum = num; }

	int GetDisabledVideoSrcNum() const { return disabledVideoSrcNum; }
	void SetDisabledVideoSrcNum(int num) { disabledVideoSrcNum = num; }
private:
	bool lastPage;

	int allVideoSrcNum;		// �������.  statVS=yes ʱ���ô��ֶ�, ���򷵻�0.
	int onlineVideoSrcNum;	// ������ƵԴ��Ŀ. statVS=yes ʱ���ô��ֶ�, ���򷵻�0.
	int disabledVideoSrcNum;	// ͣ����ƵԴ��Ŀ. statVS=yes ʱ���ô��ֶ�, ���򷵻�0. +by lzx@2018-09-04 V2R3B3SP3

public:

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// ��ѯ���֪ͨ����
class CDPSGetResultNtfReq : public CQueryNtfReqBase
{
public:
	CDPSGetResultNtfReq()
	{
		Clear();
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_NTF_REQ);
	}

	void Clear()
	{
		CQueryNtfReqBase::Clear();
		result.clear();
	}

	const CDPSSearchResultList& Result()const
	{
		return result;
	}

	CDPSSearchResultList& Result()
	{
		return result;
	}

private:

	// ���.
	CDPSSearchResultList result;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};
typedef std::vector< CDPSGetResultNtfReq > CDPSGetResultNtfReqList;


// ��ѯ���֪ͨӦ��
class CDPSGetResultNtfRsp : public CEventRsp
{
public:
	CDPSGetResultNtfRsp()
	{
		event = OspExtEventDesc(DPS_SEARCH_GET_RESULT_NTF_RSP);
		Clear();
	}

	void Clear()
	{
		CEventRsp::Clear();
	}

private:
	
public:


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

class CDPSGetDeviceStatusReq : public CEventReq
{
public:
	CDPSGetDeviceStatusReq()
	{
		event = OspExtEventDesc(DPS_GET_DEVICE_STATUS_REQ);
		Clear();
	}

	void Clear()
	{
		CEventReq::Clear();
		deviceId.clear();
	}

	string& DevcieId(){ return deviceId; }
	const string& DeviceId() const { return deviceId; }

private:
	string deviceId;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

class CDPSGetDeviceStatusRsp : public CEventRsp
{
public:
	CDPSGetDeviceStatusRsp()
	{
		Clear();
		event = OspExtEventDesc(DPS_GET_DEVICE_STATUS_RSP);
	}

	void Clear()
	{
		deviceId.clear();
		online = false;
		configStatus.Clear();
	}

	string& DevcieId(){ return deviceId; }
	const string& DeviceId() const { return deviceId; }

	bool IsOnline()const{ return online; }
	void SetOnline(bool b){ this->online = b; }

	TPuDevConfig& ConfigStatus(){ return this->configStatus; }
	const TPuDevConfig& ConfigStatus()const{ return this->configStatus; }

private:
	string deviceId;
	bool online;
	TPuDevConfig configStatus;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};


class CDPSFilterEmptyDeviceGroupReq : public CEventReq
{
public:
	CDPSFilterEmptyDeviceGroupReq()
	{
		Clear();
		event = OspExtEventDesc(DPS_FILTER_EMPTY_DEVICE_GROUP_REQ);
	}

	void Clear()
	{
		userId.clear();
		treeId.clear();
		deviceGroupIdList.clear();
	}

	string& UserId(){ return this->userId; }
	const string& UserId()const{ return this->userId; }

	string& TreeId(){ return this->treeId; }
	const string& TreeId()const{ return this->treeId; }

	std::vector<string>& DeviceGroupIdList(){ return this->deviceGroupIdList; }
	const std::vector<string>& DeviceGroupIdList()const{ return this->deviceGroupIdList; }

private:
	string userId;
	string treeId;
	std::vector<string> deviceGroupIdList;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

class CDPSFilterEmptyDeviceGroupRsp : public CEventRsp
{
public:
	CDPSFilterEmptyDeviceGroupRsp()
	{
		Clear();
		event = OspExtEventDesc(DPS_FILTER_EMPTY_DEVICE_GROUP_RSP);
	}

	void Clear()
	{
		userId.clear();
		treeId.clear();
		nonEmptyDeviceGroupIdList.clear();
	}

	string& UserId(){ return this->userId; }
	const string& UserId()const{ return this->userId; }

	string& TreeId(){ return this->treeId; }
	const string& TreeId()const{ return this->treeId; }

	std::set<string>& DeviceGroupIdList(){ return this->nonEmptyDeviceGroupIdList; }
	const std::set<string>& DeviceGroupIdList()const{ return this->nonEmptyDeviceGroupIdList; }

private:
	string userId;
	string treeId;
	std::set<string> nonEmptyDeviceGroupIdList;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};