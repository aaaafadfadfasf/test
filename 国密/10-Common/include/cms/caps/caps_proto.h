/*****************************************************************************
   ģ����      : caps_proto
   �ļ���      : caps_proto.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: caps_proto.h ����CAPS ͼƬץ��/΢��ƽ̨ ����������ϢЭ��.
   ����        : ��֮��
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2015-01-16   1.0         ��֮��        ����
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

//��ȡCAPS����
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
	int captureIntervalSecs;	// ץ�ļ��,��λ��.
	std::string wechatUrl;		// ΢��ƽ̨���ʵ�URL.
	std::string wechatTitle;    // ΢����ҳ����
public:
	void SetCaptureInterval(int nInterval) { this->captureIntervalSecs = nInterval; }
	int  GetCaptureIntercal() { return this->captureIntervalSecs; }
	void SetWechatUrl(std::string strWechatUrl) { this->wechatUrl = strWechatUrl; }
	std::string GetWechatUrl() { return wechatUrl; }
	void SetWechatTitle(std::string strTitle) { this->wechatTitle = strTitle; }
	std::string GetWechatTitle() { return this->wechatTitle; }
};

// ����.
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
	int captureIntervalSecs;	// ץ�ļ��,��λ��.
	std::string wechatTitle;    // ΢����ҳ����
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

// ��ѯ�豸�б�.
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

// ����豸
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

// �޸��豸.
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

// ɾ���豸.
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


// ץ��һ��ͼƬ
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
	TChannel devChn;			//���ĸ�ͨ����ץ������ظ�
	string picpath;				//ץ��������ͼƬ
	string thumbnailpath;		//����ͼ
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

















