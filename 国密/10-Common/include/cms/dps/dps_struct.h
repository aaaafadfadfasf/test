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
	int type;		// ����, ����μ� cusdk__SearchResultType, �豸��/�豸/��ƵԴ
	string id;	// Id
	int vsId;		// ��ƵԴid. ���Ϊ�豸��/�豸ʱ������.
	string name;	// ����

	CDeviceGroupPathItemList dgPath;	// ����ڵ�֮�ϵ������豸��ڵ�Id�б�. 

	bool online;	// �Ƿ�����.
	CDeviceCapLabelList capList;	// �豸��������ǩ�б�. (���/ǹ��) cusdk__EDeviceCapLabel

	string domainId;	// ��Id.
	string manu;		// ������.

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);


	
};

typedef std::vector<CDPSSearchResult> CDPSSearchResultList;
