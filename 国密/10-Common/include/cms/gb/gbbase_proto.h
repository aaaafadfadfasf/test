/*
���ﱣ���˹���Э���еĻ����࣬�̶�����
*/

#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "cms/cms_const.h"

#ifndef NeedAutoGen    //������Ҫ�Զ����ɽӿڵ����ݳ�Ա
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
    //CTransparentData �Ѿ�����ԭ�����ʹ�������Ҫ��������½ӿ�
    // public:
    //     const std::string ToXml() const;
    //     void FromXml(const std::string& xml_string);
    //     void ToXml(TiXmlDocument& xmlDoc)const;
    //     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    //     void FromXml(const TiXmlNode& xmlNode);
};

#endif  //#ifndef CMS_CTRANSPARENTDATA

//�����ԭ������������ RawDataType = ["int", "bool", "double", "enum", "string", "CTransparentData", "vector" ]

#include "system/converthelper/toxml.h"
#include "system/converthelper/tostruct.h"
#include "system/converthelper/adpfromstdsettovector.h"

//������������
#define GB_REQCMD_CONTROL   "Control"
#define GB_REQCMD_QUERY     "Query"
#define GB_REQCMD_NOTIFY    "Notify"
//Ӧ����������
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


//�����������
#define GBTYPENAME_RECORD_TYPE_START    "Record"
#define GBTYPENAME_RECORD_TYPE_STOP      "StopRecord"

//��������CmdType����
#define GBCMDTYPE_KDMRegister       "KDMRegister"
#define GBCMDTYPE_KDMAudioCall      "KDMAudioCall"
#define GBCMDTYPE_KDMStatus         "KDMStatus"
#define GBCMDTYPE_KDMCAPABILITY     "KdmCapability"

#define GBRESULT_OK "OK"
#define GBRESULT_ERROR "ERROR"

#define GBCERTIFICATE_TYPE_SIGNATURE "Signature"
#define GBCERTIFICATE_TYPE_ENCRYPTION "Encryption"

//��������
enum EMGBAlarmPriority
{
    GBAlarmPriority_ALL = 0,    //���м���
    GBAlarmPriority_One,        //һ������
    GBAlarmPriority_Two,        //��������
    GBAlarmPriority_Three,      //��������
    GBAlarmPriority_Four,       //�ļ�����
};

//������ʽ
enum EMGBAlarmMethod
{
    GBAlarmMethod_ALL = 0,      //���б���
    GBAlarmMethod_Phone = 1,        //�绰����
    GBAlarmMethod_Device = 2,       //�豸����
    GBAlarmMethod_Note  = 3,         //���ű���
    GBAlarmMethod_GPS  = 4,          //GPS����
    GBAlarmMethod_Video = 5,        //��Ƶ����
    GBAlarmMethod_DevTrouble  = 6,   //�豸���ϱ���
    GBAlarmMethod_Other       //��������
};

//�豸�����ı�������  GBAlarmMethod_Device
enum EMGBAlarmMethodDeviceType
{
    GB_ALARM_METHOD_DEVICE_TYPE_VIDLOST = 1,      //��Ƶ��ʧ����
    GB_ALARM_METHOD_DEVICE_TYPE_DEVDISASSEMBLY = 2,       //���𱨾�
    GB_ALARM_METHOD_DEVICE_TYPE_DISKFULL = 3,     //ý��洢����������
    GB_ALARM_METHOD_DEVICE_TYPE_OVERHEAT = 4,   //�豸���±���
    GB_ALARM_METHOD_DEVICE_TYPE_OVERCOLD = 5,   //�豸���±���

    GB_ALARM_METHOD_DEVICE_TYPE_KDEXT_SIMDATASERVICEOVERUSED = 8000    //�ƴ���������ֵ����
};


//GPS�����ı�������  GBAlarmMethod_GPS
enum EMGBAlarmMethodGpsType
{
    GB_ALARM_METHOD_GPS_TYPE_KDEXT_OVERSPEED = 8000,      //GPS���ٱ���
};


//��Ƶ�����ı������� GBAlarmMethod_Video
enum EMGBAlarmMethodVideoType
{
    GB_ALARM_METHOD_VIDEO_TYPE_ARTIFICIALSCENE = 1,                       //�˹���Ƶ����
    GB_ALARM_METHOD_VIDEO_TYPE_OBJECTMOTION = 2, //�˶�Ŀ���ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_LEFT = 3,                            //��������
    GB_ALARM_METHOD_VIDEO_TYPE_TAKEN = 4,                          //�����Ƴ���ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_TRIPWIREDETECT = 5,           //���߱���
    GB_ALARM_METHOD_VIDEO_TYPE_INVASIONORLEAVING = 6,        //����or�뿪��ⱨ�� ����ӦGB_ALARM_TYPEPARAM_INVASION��GB_ALARM_TYPEPARAM_LEAVING
    GB_ALARM_METHOD_VIDEO_TYPE_RETROGRADE = 7,                       //���м�ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_MOVINGAROUND = 8,        //�ǻ���ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_FLOW_STATICS = 9,          //����ͳ�Ʊ���
    GB_ALARM_METHOD_VIDEO_TYPE_THCKNESSSATISTICS = 10,         //�ܶȼ�ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_VIDEOEXECPTION = 11,    //��Ƶ�쳣��ⱨ��
    GB_ALARM_METHOD_VIDEO_TYPE_MOVINGSWIFTLY = 12,                 //�����ƶ�����

    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_VIDCOVERED = 8000,    //ͼ���ڵ�
    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_MOVE = 8001,               //�ƶ����
    GB_ALARM_METHOD_VIDEO_TYPE_KDEXT_INTELLIGENT = 8003,     //���ܱ���
};

//�豸���ϱ��������ı������� GBAlarmMethod_DevTrouble
enum EMGBAlarmMethodDevTroubleType
{
    GB_ALARM_METHOD_DEVTROUBLE_TYPE_STORAGEDEVICE_DISKFAULT = 1,               //�洢�豸���̹���
    GB_ALARM_METHOD_DEVTROUBLE_TYPE_STORAGEDEVICE_FAULT = 2,               //�洢�豸���ȹ���
};

//�ɵĿƴ���չЭ��2.0 ���屨�����ͣ�Ϊ�˼��ݱ���
#define GB_ALARM_TYPE_MOVE        "MotionDection"
#define GB_ALARM_TYPE_PIN         "AlarmIn"
#define GB_ALARM_TYPE_VIDCOVERED  "VideoCovered"
#define GB_ALARM_TYPE_INTELLIGENT "Intelligent"

//
#define GB_ALARM_TYPEPARAM_INVASION      (1)
#define GB_ALARM_TYPEPARAM_LEAVING   (2)
//�澯֪ͨ��չ�ֶΣ��澯����
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
    //�����Ա����DeviceID����Ա����string��OffSet = "    "����������Ϊstring(struct)/int(double)/ �ù���ģʽ��
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
	<���������>
	<CmdType>DeviceStatus</CmdType> 
	<SN>248</SN> 
	<DeviceID>34020000001130000001</DeviceID> 
	//�����������...
	</���������>
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
	1��������������
	<������ڵ�>
	<CmdType>DeviceStatus</CmdType> 
	<SN>248</SN> 
	<DeviceID>34020000001130000001</DeviceID> 
	</������ڵ�>
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

//�����ǳ���Ĵ�����
//////////////////////////////////////////////////////////////////////////
//�����Ǹ�������/Ӧ��Ļ��࣬�ǹ̶�����Ĳ��֣����е�class��Ϊ����ؼ��֣��Զ�����ʱ�ų�


//�豸������
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
		CGBControl::ToXmlWithoutClassNameNode(*pclass_nameNode);  //����ָ��CGBControl::
	}
	/*
	1������<Control></Control>�ڵ�
	2������CmdType = "DeviceControl"
	3������CGBEvent::ToXmlWithoutClassNameNode()����<Control></Control>�ڵ�����
	<CmdType>Alarm</CmdType> 
	<SN>1</SN> 
	<DeviceID>64010000001340000101</DeviceID> 
	�����ڵ�
	*/
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
	{
		//����û�����ݳ�Ա��Ҫд��xml��ֱ�ӵ��ø����ToXmlWithoutClassNameNode����
		CGBEvent::ToXmlWithoutClassNameNode(xmlNode);
	}
	virtual bool FromXml(const std::string& xml_string) = 0;
	/*
	1���жϸ��ڵ��Ƿ�<Control>
	2������CGBEvent::FromXml()������������
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

//֪ͨ��
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
        CGBNotify::ToXmlWithoutClassNameNode(*pclass_nameNode);  //����ָ��CNotify::
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        //����û�����ݳ�Ա��Ҫд��xml��ֱ�ӵ��ø����ToXmlWithoutClassNameNode����
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

//��ѯ��
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
        CGBQuery::ToXmlWithoutClassNameNode(*pclass_nameNode);  //����ָ��CGBQuery::
    }
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const
    {
        //����û�����ݳ�Ա��Ҫд��xml��ֱ�ӵ��ø����ToXmlWithoutClassNameNode����
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

//���е�Ӧ����
class CGBResponse : public CGBEvent
{

protected:
    string Result;
public:
    CGBResponse()
    {
        //��Щ����Ӧ����Ϣ������Result�ֶ�
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
        CGBResponse::ToXmlWithoutClassNameNode(*pclass_nameNode);  //����ָ��CGBControl::
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


