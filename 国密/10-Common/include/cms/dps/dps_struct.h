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

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);


	
};

typedef std::vector<CDPSSearchResult> CDPSSearchResultList;
