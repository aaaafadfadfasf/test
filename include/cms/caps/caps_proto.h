/*****************************************************************************
   模块名      : caps_proto
   文件名      : caps_proto.h
   相关文件    : 
   文件实现功能: caps_proto.h 定义CAPS 图片抓拍/微信平台 服务器的消息协议.
   作者        : 李之兴
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2015-01-16   1.0         李之兴        创建
******************************************************************************/
#pragma once

#ifndef __CAPS_PROTO_H__
#define __CAPS_PROTO_H__

#include "cms/cms_proto.h"
#include "caps_event.h"
#include "caps_struct.h"
#include "cbb/osp/osp.h"
#include "cms/cmu/cmu_struct.h"

//////////////////////////////////////////////////////////////////////////

//获取CAPS配置
class CGetCAPSConfigReq : public CEventReq
{
public:
	CGetCAPSConfigReq()
	{
		event = OspExtEventDesc(CAPS_GET_CONFIG_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}
private:
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
	
public:
};

class CGetCAPSConfigRsp : public CEventRsp
{
public:
	CGetCAPSConfigRsp()
	{
		event = OspExtEventDesc(CAPS_GET_CONFIG_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	int captureIntervalSecs;	// 抓拍间隔,单位秒.
	std::string wechatUrl;		// 微信平台访问的URL.
	std::string wechatTitle;    // 微信网页标题
public:
	void SetCaptureInterval(int nInterval) { this->captureIntervalSecs = nInterval; }
	int  GetCaptureIntercal() { return this->captureIntervalSecs; }
	void SetWechatUrl(std::string strWechatUrl) { this->wechatUrl = strWechatUrl; }
	std::string GetWechatUrl() { return wechatUrl; }
	void SetWechatTitle(std::string strTitle) { this->wechatTitle = strTitle; }
	std::string GetWechatTitle() { return this->wechatTitle; }
};

// 设置.
class CSetCAPSConfigReq : public CEventReq
{
public:
	CSetCAPSConfigReq()
	{
		event = OspExtEventDesc(CAPS_SET_CONFIG_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}
private:
	int captureIntervalSecs;	// 抓拍间隔,单位秒.
	std::string wechatTitle;    // 微信网页标题
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
	void SetCaptureInterval(int nInterval) { this->captureIntervalSecs = nInterval; }
	int  GetCaptureIntercal() { return this->captureIntervalSecs; }
	void SetWechatTitle(std::string strTitle) { this->wechatTitle = strTitle; }
	std::string GetWechatTitle() { return this->wechatTitle; }
};

class CSetCAPSConfigRsp : public CEventRsp
{
public:
	CSetCAPSConfigRsp()
	{
		event = OspExtEventDesc(CAPS_SET_CONFIG_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
public:

};

// 查询设备列表.
class CQueryDeviceListReq : public CQueryReqBase
{
public:
	CQueryDeviceListReq()
	{
		event = OspExtEventDesc(CAPS_QUERY_DEVICE_LIST_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}
private:
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

class CQueryDeviceListRsp : public CQueryRspBase
{
public:
	CQueryDeviceListRsp()
	{
		event = OspExtEventDesc(CAPS_QUERY_DEVICE_LIST_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
public:

};

class CQueryDeviceListNtfReq : public CQueryNtfReqBase
{
public:
	CQueryDeviceListNtfReq()
	{
		event = OspExtEventDesc(CAPS_QUERY_DEVICE_LIST_NTF_REQ);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	CWechatDeviceList devList;               
public:
	void SetDevList(CWechatDeviceList& cDevList)
	{
		devList = cDevList;
	}
	CWechatDeviceList& GetDevList()
	{
		return devList;
	}
	const CWechatDeviceList& GetDevList() const
	{
		return devList;
	}

};

class CQueryDeviceListNtfRsp : public CEventRsp
{
public:
	CQueryDeviceListNtfRsp()
	{
		event = OspExtEventDesc(CAPS_QUERY_DEVICE_LIST_NTF_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

// 添加设备
class CAddDeviceReq : public CEventReq
{
public:
	CAddDeviceReq()
	{
		event = OspExtEventDesc(CAPS_ADD_DEVICE_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}

public:
	void SetDevList(CWechatDeviceList& cDevList)
	{
		devList = cDevList;
	}
	CWechatDeviceList& GetDevList()
	{
		return devList;
	}
	const CWechatDeviceList& GetDevList() const
	{
		return devList;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

private:
	CWechatDeviceList devList;
};

class CAddDeviceRsp : public CEventRsp
{
public:
	CAddDeviceRsp()
	{
		event = OspExtEventDesc(CAPS_ADD_DEVICE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
public:
};

// 修改设备.
class CModifyDeviceReq : public CEventReq
{
public:
	CModifyDeviceReq()
	{
		event = OspExtEventDesc(CAPS_MODIFY_DEVICE_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}

public:
	void GetDevice(CWechatDevice& cDevice) const
	{
		cDevice = this->device;
	}

	void SetDevice(const CWechatDevice& cDevice)
	{
		this->device = cDevice;
	}
private:
	CWechatDevice device;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

class CModifyDeviceRsp : public CEventRsp
{
public:
	CModifyDeviceRsp()
	{
		event = OspExtEventDesc(CAPS_MODIFY_DEVICE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
public:
};

// 删除设备.
class CDelDeviceReq : public CEventReq
{
public:
	CDelDeviceReq()
	{
		event = OspExtEventDesc(CAPS_DEL_DEVICE_REQ);
	}

	void Clear()
	{
		CEventReq::Clear();
	}

public:
	void SetDevId(const string &strDevId) { this->devId = strDevId; }
	string GetDevId() { return this->devId; }
	void SetChanId(int nChanId) { this->chnId = nChanId; }
	int GetChanId() { return this->chnId; }
private:
	string devId;
	int chnId;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
};

class CDelDeviceRsp : public CEventRsp
{
public:
	CDelDeviceRsp()
	{
		event = OspExtEventDesc(CAPS_DEL_DEVICE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
public:
};


// 抓拍一张图片
class CTakePictureReq : public CEventReq
{
public:
	CTakePictureReq()
	{
		event = OspExtEventDesc(CAPS_TAKE_PICTURE_REQ);
	}
	void Clear()
	{
		CEventReq::Clear();
	}
private:
	TChannel devChn;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

public:
	void SetDevChn(const TChannel &chn)
	{
		devChn = chn;
	}
	const TChannel &GetDevChn() const
	{
		return devChn;
	}
};

class CTakePictureRsp : public CEventRsp
{
public:
	CTakePictureRsp()
	{
		event = OspExtEventDesc(CAPS_TAKE_PICTURE_RSP);
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
private:
	TChannel devChn;			//是哪个通道的抓拍请求回复
	string picpath;				//抓拍下来的图片
	string thumbnailpath;		//缩略图
public:
	void SetDevChn(const TChannel &chn)
	{
		devChn = chn;
	}
	const TChannel &GetDevChn() const
	{
		return devChn;
	}
	void SetPicturePath(const string &strPath)
	{
		picpath = strPath;
	}
	const string &GetPicturePath() const
	{
		return picpath;
	}
	void SetThumbNailPath(const string &strPath)
	{
		thumbnailpath = strPath;
	}
	const string &GetThumbNailPath() const
	{
		return thumbnailpath;
	}
};

#endif

















