/*****************************************************************************
   模块名      : caps_struct
   文件名      : caps_struct.h
   相关文件    : 
   文件实现功能: caps_struct.h 定义电子地图外设信令接口用到的结构体
   作者        : 李之兴
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2015-01-16   1.0         李之兴        创建
******************************************************************************/

#ifndef __CAPS_STRUCT_H__
#define __CAPS_STRUCT_H__

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

class CWechatDevice: public TChannel
{
private:
	string name;		// 名称.
	string gbid;			//国标id
	string gbdomainid;//国标域id
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	CWechatDevice()
		:TChannel()
	{
	}
	void SetAlias(const string &alias)
	{
		name = alias;
	}
	const string &GetAlias() const
	{
		return name;
	}
	void SetGBId(const string &strGBId)
	{
		gbid = strGBId;
	}
	const string &GetGBId() const
	{
		return gbid;
	}
	void SetGBDomainId(const string &strGBDomainId)
	{
		gbdomainid = strGBDomainId;
	}
	const string &GetGBDomainId() const
	{
		return gbdomainid;
	}

};

typedef std::vector< CWechatDevice > CWechatDeviceList;

#endif















