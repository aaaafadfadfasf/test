#pragma once
/*****************************************************************************
模块名      : dps
文件名      : dps_struct.h
相关文件    :
文件实现功能: DPS模块的信令定义和xml化
作者        : lizhixing
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2016-04-06   1.0        lizhixing        创建
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

// 查询结果.
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
	int type;		// 类型, 定义参见 cusdk__SearchResultType, 设备组/设备/视频源
	string id;	// Id
	int vsId;		// 视频源id. 结果为设备组/设备时无意义.
	string name;	// 名称

	CDeviceGroupPathItemList dgPath;	// 结果节点之上的所有设备组节点Id列表. 

	bool online;	// 是否在线.
	CDeviceCapLabelList capList;	// 设备的能力标签列表. (球机/枪机) cusdk__EDeviceCapLabel

	string domainId;	// 域Id.
	string manu;		// 制造商.

	bool disabled;	// 是否被停用.

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
	DPSDeviceGroupStatInvalid,	// 无效值.
	DPSDeviceGroupStatSub,		// 设备组统计信息订阅
	DPSDeviceGroupStatUnsub,	// 设备组统计信息取消订阅
	DPSDeviceGroupStatQuit,		// 取消所有订阅(CU注销)
};

enum EDPSDeviceGroupStatType
{
	DPSDeviceGroupStatTypeNoStat = 0,		// 不统计.
	DPSDeviceGroupStatTypeVideosrc = 1,		// 按视频源统计数量.
	DPSDeviceGroupStatTypeDevice,			// 按设备统计数量.
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
	string userId;			// 用户ID
	string treeId;			// 设备树ID
	string deviceGroupId;	// 设备组ID
	string cuSessionId;		// 用户登录会话.
	bool recursive;			// 是否递归设备组.
	int statType;			// 统计类型, EDPSDeviceGroupStatType. 按视频源或按设备等方式来统计.
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
	CDPSDeviceGroupSubKey subKey;	// 订阅信息.

	int totalNum;					// 总数
	int onlineNum;					// 在线数

	int errorCode;					// 错误码.
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



