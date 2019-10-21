/*
这里保存了国标协议中的基本类，固定不变
*/

#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "cms/cms_const.h"

#ifndef NeedAutoGen    //定义需要自动生成接口的数据成员
#define NeedAutoGen
#endif

#ifndef CMS_CTRANSPARENTDATA
#define CMS_CTRANSPARENTDATA

class CTransparentData : public std::string
{
public:
    CTransparentData()
    {

    }
    CTransparentData(const string& cOther)
    {
        assign(cOther);
    }
    CTransparentData& operator=(const string& cOther)
    {
        if (this != &cOther)
        {
            assign(cOther);
        }

        return *this;
    }
    //CTransparentData 已经当做原生类型处理，不需要再添加以下接口
    // public:
    //     const std::string ToXml() const;
    //     void FromXml(const std::string& xml_string);
    //     void ToXml(TiXmlDocument& xmlDoc)const;
    //     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    //     void FromXml(const TiXmlNode& xmlNode);
};

#endif  //#ifndef CMS_CTRANSPARENTDATA

//允许的原生数据类型有 RawDataType = ["int", "bool", "double", "enum", "string", "CTransparentData", "vector" ]

#include "system/converthelper/toxml.h"
#include "system/converthelper/tostruct.h"
#include "system/converthelper/adpfromstdsettovector.h"

//请求命令类型
#define GB_REQCMD_CONTROL   "Control"
#define GB_REQCMD_QUERY     "Query"
#define GB_REQCMD_NOTIFY    "Notify"
//应答命令类型
#define GB_RSPCMD_RESPONSE  "Response"

#define GBCMDTYPE_DEVICE_CONTROL    "DeviceControl"
#define GBCMDTYPE_SIGNATURE_CONTROL "SignatureControl"
#define GBCMDTYPE_ENCRYPTION_CONTROL "EncryptionControl"
#define GBCMDTYPE_KEY_FRAME         "KeyFrame"
#define GBCMDTYPE_ALARM             "Alarm"
#define GBCMDTYPE_CATALOG           "Catalog"
#define GBCMDTYPE_DEVICEINFO        "DeviceInfo"
#define GBCMDTYPE_DEVICESTATUS      "DeviceStatus"
#define GBCMDTYPE_RecordInfo        "RecordInfo"
#define GBCMDTYPE_MediaStatus       "MediaStatus"
#define GBCMDTYPE_Keepalive         "Keepalive"
#define GBCMDTYPE_DeviceConfig      "DeviceConfig"
#define GBCMDTYPE_ConfigDownload    "ConfigDownload"
#define GBCMDTYPE_BROADCAST			"Broadcast"
#define GBCMDTYPE_KD_AppMessage     "KD_AppMessage"
#define GBCMTYPE_GPS                "KdmGPS"
#define GBCMDTYPE_REGISTER          "Register"
#define GBCMDTYPE_VKEK_NTF          "ClientVKEKNotify"
#define GBCMDTYPE_GET_CERTIFICATE   "GetCertificate"
#define GBCMDTYPE_MobilePosition    "MobilePosition"


//国标控制请求
#define GBTYPENAME_RECORD_TYPE_START    "Record"
#define GBTYPENAME_RECORD_TYPE_STOP      "StopRecord"

//国标外设CmdType类型
#define GBCMDTYPE_KDMRegister       "KDMRegister"
#define GBCMDTYPE_KDMAudioCall      "KDMAudioCall"
#define GBCMDTYPE_KDMStatus         "KDMStatus"
#define GBCMDTYPE_KDMCAPABILITY     "KdmCapability"

#define GBRESULT_OK "OK"
#define GBRESULT_ERROR "ERROR"

#define GBCERTIFICATE_TYPE_SIGNATURE "Signature"
#define GBCERTIFICATE_TYPE_ENCRYPTION "Encryption"

//报警级别
enum EMGBAlarmPriority
{
    GBAlarmPriority_ALL = 0,    //所有级别
    GBAlarmPriority_One,        //一级警情
    GBAlarmPriority_Two,        //二级警情
    GBAlarmPriority_Three,      //三级警情
    GBAlarmPriority_Four,       //四级警情
};

//报警方式
enum EMGBAlarmMethod
{
    GBAlarmMethod_ALL = 0,      //所有报警
    GBAlarmMethod_Phone = 1,        //电话报警
    GBAlarmMethod_Device = 2,       //设备报警
    GBAlarmMethod_Note  = 3,         //短信报警
    GBAlarmMethod_GPS  = 4,          //GPS报警
    GBAlarmMethod_Video = 5,        //视频报警
    GBAlarmMethod_DevTrouble  = 6,   //设备故障报警
    GBAlarmMethod_Other       //其他报警
};

//设备报警的报警类型  GBAlarmMethod_Device
enum EMGBAlarmMethodDeviceType
{
    GB_ALARM_METHOD_DEVICE_TYPE_VIDLOST = 1,      //视频丢失报警
    GB_ALARM_METHOD_DEVICE_TYPE_DEVDISASSEMBLY = 2,       //防拆报警
    GB_ALARM_METHOD_DEVICE_TYPE_DISKFULL = 3,     //媒体存储磁盘满报警
    GB_ALARM_METHOD_DEVICE_TYPE_OVERHEAT = 4,   //设备高温报警
    GB_ALARM_METHOD_DEVICE_TYPE_OVERCOLD = 5,   //设备低温报警

    GB_ALARM_METHOD_DEVICE_TYPE_KDEXT_SIMDATASERVICEOVERUSED = 8000    //科达流量超阈值报警
};


//GPS报警的报警类型  GBAlarmMethod_GPS
enum EMGBAlarmMethodGpsType
{
    GB_ALARM_METHOD_GPS_TYPE_KDEXT_OVERSPEED = 8000,      //GPS超速报警
};


//视频报警的报警类型 GBAlarmMethod_Video
enum EMGBAlarmMethodVideoType
{
    GB_ALARM_METHOD_VIDEO_TYPE_ARTIFICIALSCENE = 1,                       //人工视频报警
    GB_ALARM_METHOD_VIDEO_TYPE_OBJECTMOTION = 2, //运动目标检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_LEFT = 3,                            //移留物检测
    GB_ALARM_METHOD_VIDEO_TYPE_TAKEN = 4,                          //物体移除检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_TRIPWIREDETECT = 5,           //绊线报警
    GB_ALARM_METHOD_VIDEO_TYPE_INVASIONORLEAVING = 6,        //入侵or离开检测报警 ，对应GB_ALARM_TYPEPARAM_INVASION和GB_ALARM_TYPEPARAM_LEAVING
    GB_ALARM_METHOD_VIDEO_TYPE_RETROGRADE = 7,                       //逆行检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_MOVINGAROUND = 8,        //徘徊检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_FLOW_STATICS = 9,          //流量统计报警
    GB_ALARM_METHOD_VIDEO_TYPE_THCKNESSSATISTICS = 10,         //密度检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_VIDEOEXECPTION = 11,    //视频异常检测报警
    GB_ALARM_METHOD_VIDEO_TYPE_MOVINGSWIFTLY = 12,                 //快速移动报警

    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_VIDCOVERED = 8000,    //图像遮挡
    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_MOVE = 8001,               //移动侦测
    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_INTELLIGENT = 8003,     //智能报警
};

//设备故障报警报警的报警类型 GBAlarmMethod_DevTrouble
enum EMGBAlarmMethodDevTroubleType
{
    GB_ALARM_METHOD_DEVTROUBLE_TYPE_STORAGEDEVICE_DISKFAULT = 1,               //存储设备磁盘故障
    GB_ALARM_METHOD_DEVTROUBLE_TYPE_STORAGEDEVICE_FAULT = 2,               //存储设备风扇故障
};

//旧的科达扩展协议2.0 定义报警类型，为了兼容保留
#define GB_ALARM_TYPE_MOVE        "MotionDection"
#define GB_ALARM_TYPE_PIN         "AlarmIn"
#define GB_ALARM_TYPE_VIDCOVERED  "VideoCovered"
#define GB_ALARM_TYPE_INTELLIGENT "Intelligent"

//
#define GB_ALARM_TYPEPARAM_INVASION      (1)
#define GB_ALARM_TYPEPARAM_LEAVING   (2)
//告警通知扩展字段：告警类型
#define GBALARMNTF_KDALARMTYPE_ON   "ON"
#define GBALARMNTF_KDALARMTYPE_OFF  "OFF"
#define GB_CMD_CONTROL_ALARM_RESET "ResetAlarm"

class CGBEvent
{
protected:
	string CmdType;
	unsigned int SN;
	string DeviceID;
protected:
	mutable TiXmlDocument xmlDoc;
public:
	CGBEvent()
	{
        Clear();
	}
	virtual ~CGBEvent(){};
public:
	virtual void Clear()
	{
		CmdType.clear();
		SN = 0;
		DeviceID.clear();

        ClearXmlDocument();
	}
	TiXmlDocument &GetXmlDoc()
	{
		return xmlDoc;
	}
	TiXmlNode &GetXmlNode() const
	{
		return *xmlDoc.FirstChild()->NextSibling();
	}
    void SetCmdType(const string& strCmdType)
    {
        CmdType = strCmdType;
    }
    string& GetCmdType()
    {
        return CmdType;
    }
    const string& GetCmdType() const
    {
        return CmdType;
    }
	const unsigned int GetSN() const
	{
		return SN;
	}
    void SetSN(const unsigned int dwSN)
    {
        SN = dwSN;
    }
    //输入成员名称DeviceID，成员类型string，OffSet = "    "，函数类型为string(struct)/int(double)/ 用工程模式？
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
public:
	virtual const std::string ToXml()const = 0;
	
	virtual void ToXml(TiXmlDocument& xmlDoc)const = 0;
	/*
	<子类的名称>
	<CmdType>DeviceStatus</CmdType> 
	<SN>248</SN> 
	<DeviceID>34020000001130000001</DeviceID> 
	//孙子类的内容...
	</子类的名称>
	*/
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		if(!CmdType.empty())
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"CmdType","string",CmdType);
		}

		ConvertHelper::ToXml::AddChildNode(xmlNode,"SN","int",SN);

		if(!DeviceID.empty())
		{
			ConvertHelper::ToXml::AddChildNode(xmlNode,"DeviceID","string",DeviceID);
		}
	}
	virtual bool FromXml(const std::string& xml_string) = 0;
	/*
	1，解析以下内容
	<子类根节点>
	<CmdType>DeviceStatus</CmdType> 
	<SN>248</SN> 
	<DeviceID>34020000001130000001</DeviceID> 
	</子类根节点>
	*/
	virtual bool FromXml(const TiXmlNode& xmlNode)
	{
		CGBEvent::Clear();

		ConvertHelper::ToStruct::ParseChildNode(xmlNode,"CmdType",CmdType);

		ConvertHelper::ToStruct::ParseChildNode(xmlNode,"SN",SN);

		ConvertHelper::ToStruct::ParseChildNode(xmlNode,"DeviceID",DeviceID);

        return true;
	}
protected:
    void ClearXmlDocument() const
    {
        xmlDoc.Clear();
        TiXmlDeclaration tDeclare("1.0", "", "");
        xmlDoc.InsertEndChild(tDeclare);
    }
private:
};

struct TGBStruct
{
public:
    virtual ~TGBStruct(){};
    virtual bool empty() const = 0;
public:
    virtual void Clear()
    {

    }
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const = 0;
	virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        return true;
    }
private:
};

//以上是抽象的纯虚类
//////////////////////////////////////////////////////////////////////////
//以下是各个请求/应答的基类，是固定不变的部分，所有的class作为特殊关键字，自动生成时排除


//设备控制类
class CGBControl : public CGBEvent
{
public:

	CGBControl()
	{
		CmdType = GBCMDTYPE_DEVICE_CONTROL;
	}
    virtual ~CGBControl(){};
    virtual void Clear()
    {
        CGBEvent::Clear();
    }
public:
	virtual const std::string ToXml()const = 0;
	virtual void ToXml(TiXmlDocument& xmlDoc)const
	{
		TiXmlElement* pclass_nameNode = new TiXmlElement(GB_REQCMD_CONTROL);
		xmlDoc.LinkEndChild(pclass_nameNode);
		CGBControl::ToXmlWithoutClassNameNode(*pclass_nameNode);  //必须指定CGBControl::
	}
	/*
	1，创建<Control></Control>节点
	2，设置CmdType = "DeviceControl"
	3，调用CGBEvent::ToXmlWithoutClassNameNode()，在<Control></Control>节点生成
	<CmdType>Alarm</CmdType> 
	<SN>1</SN> 
	<DeviceID>64010000001340000101</DeviceID> 
	三个节点
	*/
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		//本类没有数据成员需要写入xml，直接调用父类的ToXmlWithoutClassNameNode函数
		CGBEvent::ToXmlWithoutClassNameNode(xmlNode);
	}
	virtual bool FromXml(const std::string& xml_string) = 0;
	/*
	1，判断根节点是否<Control>
	2，调用CGBEvent::FromXml()解析以下内容
	<Control> 
	<CmdType>DeviceControl</CmdType> 
	<SN>11</SN> 
	<DeviceID>64010000041310000345</DeviceID> 
	</Control> 
	*/
	virtual bool FromXml(const TiXmlNode& xmlNode)
	{
        if(xmlNode.ValueStr() != GB_REQCMD_CONTROL)
        {
            CMS_ASSERT(0&&"error xml format, not root of class");
            return false;
        }
        return CGBEvent::FromXml(xmlNode);
	}
};

//通知类
class CGBNotify : public CGBEvent
{
public:
    CGBNotify()
    {

    }
    virtual ~CGBNotify(){};
    virtual void Clear()
    {
        CGBEvent::Clear();
    }
public:
    virtual const std::string ToXml()const = 0;
    virtual void ToXml(TiXmlDocument& xmlDoc)const
    {
        TiXmlElement* pclass_nameNode = new TiXmlElement(GB_REQCMD_NOTIFY);
        xmlDoc.LinkEndChild(pclass_nameNode);
        CGBNotify::ToXmlWithoutClassNameNode(*pclass_nameNode);  //必须指定CNotify::
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        //本类没有数据成员需要写入xml，直接调用父类的ToXmlWithoutClassNameNode函数
        CGBEvent::ToXmlWithoutClassNameNode(xmlNode);
    }
    virtual bool FromXml(const std::string& xml_string) = 0;
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        if(xmlNode.ValueStr() != GB_REQCMD_NOTIFY)
        {
            CMS_ASSERT(0&&"error xml format, not root of class");
            return false;
        }
        return CGBEvent::FromXml(xmlNode);
    }
};

//查询类
class CGBQuery : public CGBEvent
{
public:
    CGBQuery()
    {

    }
    virtual ~CGBQuery(){};
    virtual void Clear()
    {
        CGBEvent::Clear();
    }
public:
    virtual const std::string ToXml()const = 0;
    virtual void ToXml(TiXmlDocument& xmlDoc)const
    {
        TiXmlElement* pclass_nameNode = new TiXmlElement(GB_REQCMD_QUERY);
        xmlDoc.LinkEndChild(pclass_nameNode);
        CGBQuery::ToXmlWithoutClassNameNode(*pclass_nameNode);  //必须指定CGBQuery::
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        //本类没有数据成员需要写入xml，直接调用父类的ToXmlWithoutClassNameNode函数
        CGBEvent::ToXmlWithoutClassNameNode(xmlNode);
    }
    virtual bool FromXml(const std::string& xml_string) = 0;
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        if(xmlNode.ValueStr() != GB_REQCMD_QUERY)
        {
            CMS_ASSERT(0&&"error xml format, not root of class");
            return false;
        }
        return CGBEvent::FromXml(xmlNode);
    }
};

//所有的应答类
class CGBResponse : public CGBEvent
{

protected:
    string Result;
public:
    CGBResponse()
    {
        //有些国标应答消息不含带Result字段
        //Result = GBRESULT_OK;
    }
    virtual ~CGBResponse(){};
    virtual void Clear()
    {
        CGBEvent::Clear();
    }
    void SetResult(const string& strResult)
    {
        Result = strResult;
    }
    string& GetResult()
    {
        return Result;
    }
    const string& GetResult() const
    {
        return Result;
    }
public:
    virtual const std::string ToXml()const = 0;
    virtual void ToXml(TiXmlDocument& xmlDoc)const
    {
        TiXmlElement* pclass_nameNode = new TiXmlElement(GB_RSPCMD_RESPONSE);
        xmlDoc.LinkEndChild(pclass_nameNode);
        CGBResponse::ToXmlWithoutClassNameNode(*pclass_nameNode);  //必须指定CGBControl::
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        CGBEvent::ToXmlWithoutClassNameNode(xmlNode);

        if ( !Result.empty() )
        {
            ConvertHelper::ToXml::AddChildNode(xmlNode,"Result","string",Result);
        }
    }
    virtual bool FromXml(const std::string& xml_string) = 0;
    virtual bool FromXml(const TiXmlNode& xmlNode)
    {
        bool bRet = true;
        if(xmlNode.ValueStr() != GB_RSPCMD_RESPONSE)
        {
            CMS_ASSERT(0&&"error xml format, not root of class");
            return false;
        }
        bRet &= CGBEvent::FromXml(xmlNode);

        ConvertHelper::ToStruct::ParseChildNode(xmlNode,"Result",Result);

        return bRet;
    }
};


//End of file


