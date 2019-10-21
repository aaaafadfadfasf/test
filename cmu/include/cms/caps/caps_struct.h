/*****************************************************************************
   ģ����      : caps_struct
   �ļ���      : caps_struct.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: caps_struct.h ������ӵ�ͼ��������ӿ��õ��Ľṹ��
   ����        : ��֮��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2015-01-16   1.0         ��֮��        ����
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
	string name;		// ����.
	string gbid;			//����id
	string gbdomainid;//������id
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















