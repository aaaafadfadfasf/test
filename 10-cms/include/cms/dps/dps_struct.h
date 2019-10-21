#pragma once
/*****************************************************************************
ģ����      : dps
�ļ���      : dps_struct.h
����ļ�    :
�ļ�ʵ�ֹ���: DPSģ���������xml��
����        : lizhixing
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2016-04-06   1.0        lizhixing        ����
******************************************************************************/

#include "kdvtype.h"
#include "cms/cms_struct.h"
#include "cms/cmsassert.h"
#include "cms/utility/uuid.h"
#include "cms/cms_proto.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::string;
using std::vector;
using std::map;

class TiXmlDocument;
class TiXmlNode;

class CDeviceGroupPathItem
{
public:
	CDeviceGroupPathItem()
	{
		Clear();
	}
	void Clear()
	{
		GroupId().clear();
		GroupName().clear();
	}

public:
	std::string& GroupId()
	{
		return this->id;
	}

	const std::string& GroupId()const
	{
		return this->id;
	}

	std::string& GroupName()
	{
		return this->name;
	}

	const std::string& GroupName() const
	{
		return this->name;
	}


private:
	string id;
	string name;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef std::vector< CDeviceGroupPathItem > CDeviceGroupPathItemList;

typedef std::vector<int> CDeviceCapLabelList;

// ��ѯ���.
class CDPSSearchResult
{
public:
	CDPSSearchResult()
	{
		Clear();
	}
	void Clear()
	{
		type = 0;
		id.clear();
		vsId = 0;
		name.clear();
		dgPath.clear();
		online = true;
		disabled = false;
		CapList().clear();
		this->SetType(SearchResultTypeInvalid);
	}

	enum ESearchResultType
	{
		SearchResultTypeInvalid,
		SearchResultDeviceGroup,
		SearchResultDevice,
		SearchResultVideosrc,
	};

public:
	ESearchResultType GetType()const{
		return (ESearchResultType)this->type;
	}

	void SetType(int t)
	{
		this->type = t;
	}

	const string& Id()const
	{
		return this->id;
	}

	string& Id()
	{
		return this->id;
	}

	int GetVSId()const
	{
		return this->vsId;
	}

	void SetVSId(int vs)
	{
		this->vsId = vs;
	}

	const string& Name()const
	{
		return this->name;
	}

	string& Name()
	{
		return this->name;
	}

	CDeviceGroupPathItemList& DGPath()
	{
		return this->dgPath;
	}

	const CDeviceGroupPathItemList& DGPath()const
	{
		return this->dgPath;
	}

	bool IsOnline() const
	{
		return this->online;
	}

	void SetOnline(bool ol)
	{
		this->online = ol;
	}

	bool IsDisabled() const
	{
		return this->disabled;
	}

	void SetDisabled(bool dis)
	{
		this->disabled = dis;
	}

	CDeviceCapLabelList& CapList()
	{
		return this->capList;
	}

	const CDeviceCapLabelList& CapList()const
	{
		return this->capList;
	}

	string& DomainId(){ return this->domainId; }
	const string& DomainId()const{ return this->domainId; }

	string& Manufactor(){ return this->manu; }
	const string& Manufactor()const{ return this->manu; }

private:
	int type;		// ����, ����μ� cusdk__SearchResultType, �豸��/�豸/��ƵԴ
	string id;	// Id
	int vsId;		// ��ƵԴid. ���Ϊ�豸��/�豸ʱ������.
	string name;	// ����

	CDeviceGroupPathItemList dgPath;	// ����ڵ�֮�ϵ������豸��ڵ�Id�б�. 

	bool online;	// �Ƿ�����.
	CDeviceCapLabelList capList;	// �豸��������ǩ�б�. (���/ǹ��) cusdk__EDeviceCapLabel

	string domainId;	// ��Id.
	string manu;		// ������.

	bool disabled;	// �Ƿ�ͣ��.

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);


	
};

typedef std::vector<CDPSSearchResult> CDPSSearchResultList;

enum EDPSDeviceGroupNotifyCmd
{
	DPSDeviceGroupStatInvalid,	// ��Чֵ.
	DPSDeviceGroupStatSub,		// �豸��ͳ����Ϣ����
	DPSDeviceGroupStatUnsub,	// �豸��ͳ����Ϣȡ������
	DPSDeviceGroupStatQuit,		// ȡ�����ж���(CUע��)
};

enum EDPSDeviceGroupStatType
{
	DPSDeviceGroupStatTypeNoStat = 0,		// ��ͳ��.
	DPSDeviceGroupStatTypeVideosrc = 1,		// ����ƵԴͳ������.
	DPSDeviceGroupStatTypeDevice,			// ���豸ͳ������.
};

class CDPSDeviceGroupSubKey
{
public:
	void Clear()
	{
		userId.clear();
		treeId.clear();
		deviceGroupId.clear();
		CuSessionId().clear();
		recursive = false;
		statType = 0;
	}

	CDPSDeviceGroupSubKey(const string& uId, const string& tId, const string& gId, const string& sId, bool r, EDPSDeviceGroupStatType type)
		:userId(uId), treeId(tId), deviceGroupId(gId), cuSessionId(sId), recursive(r), statType( type )
	{	}

	CDPSDeviceGroupSubKey()
	{
		this->Clear();
	}

	const string& UserId() const
	{
		return this->userId;
	};
	string& UserId()
	{
		return this->userId;
	}

	const string& TreeId() const
	{
		return this->treeId;
	}
	string& TreeId()
	{
		return this->treeId;
	}

	const string& DeviceGroupId() const
	{
		return this->deviceGroupId;
	}

	string& DeviceGroupId()
	{
		return this->deviceGroupId;
	}

	const string& CuSessionId() const
	{
		return this->cuSessionId;
	}

	string& CuSessionId()
	{
		return this->cuSessionId;
	}

	bool IsRecursive() const
	{
		return this->recursive;
	}

	void SetRecursive(bool r)
	{
		this->recursive = r;
	}

	int GetStatType() const
	{
		return statType;
	}

	void SetStatType(EDPSDeviceGroupStatType type)
	{
		this->statType = type;
	}

	bool operator < (const CDPSDeviceGroupSubKey& another)const
	{
		if ( userId != another.userId )
		{
			return userId < another.userId;
		}

		if (treeId != another.treeId)
		{
			return treeId < another.treeId;
		}

		if ( deviceGroupId != another.deviceGroupId )
		{
			return deviceGroupId < another.deviceGroupId;
		}

		if ( CuSessionId() != another.CuSessionId() )
		{
			return CuSessionId() < another.CuSessionId();
		}

		if (recursive != another.recursive)
		{
			return recursive < another.recursive;
		}

		if ( statType != another.statType )
		{
			return statType < another.statType;
		}

		return false;
	}


	string ToCacheKey( const string& pre ) const
	{
		string cacheKey = pre + ":" + userId + ":" + treeId + ":" + deviceGroupId;
		if ( recursive )
		{
			cacheKey += ":r";
		}
		else
		{
			cacheKey += ":nr";
		}

		if ( DPSDeviceGroupStatTypeVideosrc == statType )
		{
			cacheKey += ":vs";
		}
		else if (DPSDeviceGroupStatTypeDevice == statType)
		{
			cacheKey += ":dev";
		}
		return cacheKey;
	}

private:
	string userId;			// �û�ID
	string treeId;			// �豸��ID
	string deviceGroupId;	// �豸��ID
	string cuSessionId;		// �û���¼�Ự.
	bool recursive;			// �Ƿ�ݹ��豸��.
	int statType;			// ͳ������, EDPSDeviceGroupStatType. ����ƵԴ���豸�ȷ�ʽ��ͳ��.
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef std::set<CDPSDeviceGroupSubKey> CDPSDeviceGroupSubKeySet;

class CDPSDeviceGroupSubCmd
{
public:
	void Clear()
	{
		subCmd = DPSDeviceGroupStatInvalid;
		subKey.Clear();
	}
	CDPSDeviceGroupSubCmd()
	{
		this->Clear();
	}

	EDPSDeviceGroupNotifyCmd GetSubCmd() const
	{
		return (EDPSDeviceGroupNotifyCmd)this->subCmd;
	}

	void SetSubCmd(EDPSDeviceGroupNotifyCmd cmd)
	{
		this->subCmd = cmd;
	}

	const CDPSDeviceGroupSubKey& DPSDeviceGroupSubKey() const
	{
		return this->subKey;
	}

	CDPSDeviceGroupSubKey& DPSDeviceGroupSubKey()
	{
		return this->subKey;
	}

private:
	int subCmd;				// EDPSDeviceGroupNotifyCmd
	CDPSDeviceGroupSubKey subKey;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


class CDPSDeviceGroupStatNotify
{
public:
	void Clear()
	{
		subKey.Clear();
		totalNum = 0;
		onlineNum = 0;
		errorCode = 0;
	}

	CDPSDeviceGroupStatNotify()
	{
		this->Clear();
	}

	const CDPSDeviceGroupSubKey& DPSDeviceGroupSubKey() const
	{
		return this->subKey;
	}

	CDPSDeviceGroupSubKey& DPSDeviceGroupSubKey()
	{
		return this->subKey;
	}

	int GetTotalNum() const
	{
		return totalNum;
	}

	void SetTotalNum(int total)
	{
		this->totalNum = total;
	}

	int GetOnlineNum() const
	{
		return this->onlineNum;
	}

	void SetOnlineNum(int online)
	{
		this->onlineNum = online;
	}

	int GetErrorCode() const
	{
		return this->errorCode;
	}

	void SetErrorCode(int er)
	{
		this->errorCode = er;
	}

private:
	CDPSDeviceGroupSubKey subKey;	// ������Ϣ.

	int totalNum;					// ����
	int onlineNum;					// ������

	int errorCode;					// ������.
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



