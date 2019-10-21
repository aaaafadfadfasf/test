/*
这里保存了国标协议中的具体消息类
*/

#pragma once
#include "gbbase_proto.h"
#include "gb_struct.h"
#include "cms/utility/cmstime.h"

//以下是各个请求/应答的协议类，自动生成cpp

//模糊查询
typedef string TInDistinctQuery;     
#define IS_INDISTINCT_QUERY "1"      //是
#define NOT_INDISTINCT_QUERY "0"     //否

//国标告警复位控制请求
class CGBAlarmResetReq : public CGBControl
{
private:
    string AlarmCmd;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetAlarmCmd(const string& strAlarmCmd)
    {
        AlarmCmd = strAlarmCmd;
    }
    const string& GetAlarmCmd() const
    {
        return AlarmCmd;
    }
    string& GetAlarmCmd()
    {
        return AlarmCmd;
    }
};



//国标告警布撤防控制请求
class CGBAlarmGuardReq : public CGBControl
{
private:
    string GuardCmd;
    TAlarmGurdInfoExt Info;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetGuardCmd(const string& strGuardCmd)
    {
        GuardCmd = strGuardCmd;
    }
    const string& GetGuardCmd() const
    {
        return GuardCmd;
    }
    string& GetGuardCmd()
    {
        return GuardCmd;
    }
    void SetInfo(const TAlarmGurdInfoExt& tInfo)
    {
        Info = tInfo;
    }
    const TAlarmGurdInfoExt& GetInfo() const
    {
        return Info;
    }
    TAlarmGurdInfoExt& GetInfo()
    {
        return Info;
    }
};



//国标配置请求
class CGBConfigSetReq : public CGBQuery  
{
public:
	CGBConfigSetReq()
	{
		CmdType = GBCMDTYPE_DeviceConfig;
	}
private:
    vector<TVideoParamAttributeType> VideoParamConfig;
	//之后为国标扩展
    vector<TGBVideoEncStreamType> KdmVideoEncConfig;
    TGBVideoInInfConfigType KdmVideoInInfConfig;
    TKDMGBMotionDectionConfigType KdmMotionDectionConfig;
    TKDMGBVideoCoveredConfigType KdmVideoCoveredConfig ;



public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetVideoParamConfig(const vector<TVideoParamAttributeType>& tVideoParamConfig)
    {
        VideoParamConfig = tVideoParamConfig;
    }
    const vector<TVideoParamAttributeType>& GetVideoParamConfig() const
    {
        return VideoParamConfig;
    }
    vector<TVideoParamAttributeType>& GetVideoParamConfig()
    {
        return VideoParamConfig;
    }
    void SetKdmVideoEncConfig(const vector<TGBVideoEncStreamType>& tKdmVideoEncConfig)
    {
        KdmVideoEncConfig = tKdmVideoEncConfig;
    }
    const vector<TGBVideoEncStreamType>& GetKdmVideoEncConfig() const
    {
        return KdmVideoEncConfig;
    }
    vector<TGBVideoEncStreamType>& GetKdmVideoEncConfig()
    {
        return KdmVideoEncConfig;
    }
    void SetKdmVideoInInfConfig(const TGBVideoInInfConfigType& tKdmVideoInInfConfig)
    {
        KdmVideoInInfConfig = tKdmVideoInInfConfig;
    }
    const TGBVideoInInfConfigType& GetKdmVideoInInfConfig() const
    {
        return KdmVideoInInfConfig;
    }
    TGBVideoInInfConfigType& GetKdmVideoInInfConfig()
    {
        return KdmVideoInInfConfig;
    }
    void SetKdmMotionDetionConfig(const TKDMGBMotionDectionConfigType& tKdmMotionDetionConfig)
    {
        KdmMotionDectionConfig = tKdmMotionDetionConfig;
    }
    const TKDMGBMotionDectionConfigType& GetKdmMotionDetionConfig() const
    {
        return KdmMotionDectionConfig;
    }
    TKDMGBMotionDectionConfigType& GetKdmMotionDetionConfig()
    {
        return KdmMotionDectionConfig;
    }

    void SetKdmVideoCoveredConfig(const TKDMGBVideoCoveredConfigType& tKdmVideoCoveredConfig)
    {
        KdmVideoCoveredConfig = tKdmVideoCoveredConfig;
    }
    const TKDMGBVideoCoveredConfigType& GetKdmVideoCoveredConfig() const
    {
        return KdmVideoCoveredConfig;
    }
    TKDMGBVideoCoveredConfigType& GetKdmVideoCoveredConfig()
    {
        return KdmVideoCoveredConfig;
    }



};

//国标配置应答
class CGBConfigSetRsp : public CGBResponse
{
public:
	CGBConfigSetRsp()
	{
		CmdType = GBCMDTYPE_DeviceConfig;
	}

public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBDeviceConfigReq: public CGBControl
{
public:
    CGBDeviceConfigReq()
    {
        CmdType = GBCMDTYPE_DeviceConfig;
    }

private:
    TDeviceConfigBasicParam BasicParam;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetBasicParam(const TDeviceConfigBasicParam& tBasicParam)
    {
        BasicParam = tBasicParam;
    }
    const TDeviceConfigBasicParam& GetBasicParam() const
    {
        return BasicParam;
    }
    TDeviceConfigBasicParam& GetBasicParam()
    {
        return BasicParam;
    }
};

class CGBRegisterRsp : public CGBResponse
{
public:
    CGBRegisterRsp()
    {
        CmdType = GBCMDTYPE_REGISTER;
    }
private:
    string Manufacturer;
    TGBProtocolExtensionInfo ProtocolExtension;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetManufacturer(const string& strManufacturer)
    {
        Manufacturer = strManufacturer;
    }
    const string& GetManufacturer() const
    {
        return Manufacturer;
    }
    string& GetManufacturer()
    {
        return Manufacturer;
    }
    void SetProtocolExtension(const TGBProtocolExtensionInfo& tProtocolExtension)
    {
        ProtocolExtension = tProtocolExtension;
    }
    const TGBProtocolExtensionInfo& GetProtocolExtension() const
    {
        return ProtocolExtension;
    }
    TGBProtocolExtensionInfo& GetProtocolExtension()
    {
        return ProtocolExtension;
    }
};

class CGBMotionDectionConfigReq : public CGBQuery
{
public:
    CGBMotionDectionConfigReq()
    {
        CmdType = GBCMDTYPE_DeviceConfig;
    }

private:
    TKDMGBMotionDectionConfigType KdmMotionDectionConfig;

public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdmMotionDetionConfig(const TKDMGBMotionDectionConfigType& tKdmMotionDetionConfig)
    {
        KdmMotionDectionConfig = tKdmMotionDetionConfig;
    }
    const TKDMGBMotionDectionConfigType& GetKdmMotionDetionConfig() const
    {
        return KdmMotionDectionConfig;
    }
    TKDMGBMotionDectionConfigType& GetKdmMotionDetionConfig()
    {
        return KdmMotionDectionConfig;
    }
};

class CGBMotionDectionConfigRsp : public CGBResponse
{
public:
    CGBMotionDectionConfigRsp()
    {
        CmdType = GBCMDTYPE_DeviceConfig;
    }

public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};


class CGBPTZCmd : public CGBControl
{
private:
    string PTZCmd;
    TGBPTZInfo Info;
public:
	CGBPTZCmd()
	{

	}
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetPTZCmd(const string& strPTZCmd)
    {
        PTZCmd = strPTZCmd;
    }
    const string& GetPTZCmd() const
    {
        return PTZCmd;
    }
    string& GetPTZCmd()
    {
        return PTZCmd;
    }
    void SetInfo(const TGBPTZInfo& tInfo)
    {
        Info = tInfo;
    }
    const TGBPTZInfo& GetInfo() const
    {
        return Info;
    }
    TGBPTZInfo& GetInfo()
    {
        return Info;
    }
};

class CTeleBootCmd: public CGBControl
{
private:
    string TeleBoot; 
public:
    CTeleBootCmd()
    {
        TeleBoot = "Boot";
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetTeleBoot(const string& strTeleBoot)
    {
        TeleBoot = strTeleBoot;
    }
    const string& GetTeleBoot() const
    {
        return TeleBoot;
    }
    string& GetTeleBoot()
    {
        return TeleBoot;
    }
};

class CKeyFrameCmd: public CGBControl
{
public:
    CKeyFrameCmd()
    {
        CmdType = GBCMDTYPE_KEY_FRAME; // 科达平台1.0扩展的协议，用于请求关键帧
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBRecordReq : public CGBControl
{
private:
    string RecordCmd;
public:
    CGBRecordReq()
    {

    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetRecordCmd(const string& strRecordCmd)
    {
        RecordCmd = strRecordCmd;
    }
    const string& GetRecordCmd() const
    {
        return RecordCmd;
    }
    string& GetRecordCmd()
    {
        return RecordCmd;
    }
};

class CGBCommonRsp : public CGBResponse
{
public:
    CGBCommonRsp()
    {
        CmdType = GBCMDTYPE_DEVICE_CONTROL;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

typedef CGBCommonRsp CGBRecordRsp;

class CAlarmOccurNotify : public CGBNotify
{
private:
    int AlarmPriority;
    int AlarmMethod;
    string AlarmTime;
    string AlarmDescription;
    double Longitude;
    double Latitude;
    TAlarmInfoExt Info;
public:
    CAlarmOccurNotify()
    {
        CmdType = GBCMDTYPE_ALARM;
        Longitude = INVALID_LONGITUDE;
        Latitude = INVALID_LATITUDE;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetAlarmPriority(const int nAlarmPriority)
    {
        AlarmPriority = nAlarmPriority;
    }
    const int GetAlarmPriority() const
    {
        return AlarmPriority;
    }
    void SetAlarmMethod(const int nAlarmMethod)
    {
        AlarmMethod = nAlarmMethod;
    }
    const int GetAlarmMethod() const
    {
        return AlarmMethod;
    }
    void SetAlarmTime(const string& strAlarmTime)
    {
        AlarmTime = strAlarmTime;
    }
    const string& GetAlarmTime() const
    {
        return AlarmTime;
    }
    string& GetAlarmTime()
    {
        return AlarmTime;
    }
    void SetAlarmDescription(const string& strAlarmDescription)
    {
        AlarmDescription = strAlarmDescription;
    }
    const string& GetAlarmDescription() const
    {
        return AlarmDescription;
    }
    string& GetAlarmDescription()
    {
        return AlarmDescription;
    }
    void SetLongitude(const double fLongitude)
    {
        Longitude = fLongitude;
    }
    const double GetLongitude() const
    {
        return Longitude;
    }
    void SetLatitude(const double fLatitude)
    {
        Latitude = fLatitude;
    }
    const double GetLatitude() const
    {
        return Latitude;
    }
    void SetInfo(const TAlarmInfoExt& tInfo)
    {
        Info = tInfo;
    }
    const TAlarmInfoExt& GetInfo() const
    {
        return Info;
    }
    TAlarmInfoExt& GetInfo()
    {
        return Info;
    }
};

class CGBAlarmOccurRsp : public CGBResponse
{
public:
    CGBAlarmOccurRsp()
    {
        CmdType = GBCMDTYPE_ALARM;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBCatalogQryReq : public CGBQuery
{
public:
    CGBCatalogQryReq()
    {
        CmdType = GBCMDTYPE_CATALOG;
    }
private:
    string StartTime;
    string EndTime;

public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartTime(const string& strStartTime)
    {
        StartTime = strStartTime;
    }
    const string& GetStartTime() const
    {
        return StartTime;
    }
    string& GetStartTime()
    {
        return StartTime;
    }
    void SetEndTime(const string& strEndTime)
    {
        EndTime = strEndTime;
    }
    const string& GetEndTime() const
    {
        return EndTime;
    }
    string& GetEndTime()
    {
        return EndTime;
    }
};

class CGBCataLogQryRsp : public CGBResponse
{
public:
    CGBCataLogQryRsp()
    {
        CmdType = GBCMDTYPE_CATALOG;
    }
private:
    int SumNum;
    vector< TGBDeviceCataLog > DeviceList;  //dev list
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSumNum(const int nSumNum)
    {
        SumNum = nSumNum;
    }
    const int GetSumNum() const
    {
        return SumNum;
    }
    void SetDeviceList(const vector<TGBDeviceCataLog>& tDeviceList)
    {
        DeviceList = tDeviceList;
    }
    const vector<TGBDeviceCataLog>& GetDeviceList() const
    {
        return DeviceList;
    }
    vector<TGBDeviceCataLog>& GetDeviceList()
    {
        return DeviceList;
    }
};

class CGBCatalogNtf : public CGBNotify
{
public:
	CGBCatalogNtf()
	{
		CmdType = GBCMDTYPE_CATALOG;
	}
    int SumNum;
    vector< TGBDeviceCataLog > DeviceList;  //dev list
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSumNum(const int nSumNum)
    {
        SumNum = nSumNum;
    }
    const int GetSumNum() const
    {
        return SumNum;
    }
    void SetDeviceList(const vector<TGBDeviceCataLog>& tDeviceList)
    {
        DeviceList = tDeviceList;
    }
    const vector<TGBDeviceCataLog>& GetDeviceList() const
    {
        return DeviceList;
    }
    vector<TGBDeviceCataLog>& GetDeviceList()
    {
        return DeviceList;
    }
};
class CGBCataLogQryRecvRsp : public CGBResponse
{
public:
    CGBCataLogQryRecvRsp()
    {
        CmdType = GBCMDTYPE_CATALOG;
        Result = GBRESULT_OK;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBDevInfoQryReq : public CGBQuery
{
public:
    CGBDevInfoQryReq()
    {
        CmdType = GBCMDTYPE_DEVICEINFO;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBDevInfoQryRsp : public CGBResponse
{
public:
    CGBDevInfoQryRsp()
    {
        CmdType = GBCMDTYPE_DEVICEINFO;
    }
private:
    string DeviceName;
    string Manufacturer;
    string Model;
    string Firmware;
    int Channel;
    TDeviceInfoExt Info;
    
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetManufacturer(const string& strManufacturer)
    {
        Manufacturer = strManufacturer;
    }
    const string& GetManufacturer() const
    {
        return Manufacturer;
    }
    string& GetManufacturer()
    {
        return Manufacturer;
    }
    void SetModel(const string& strModel)
    {
        Model = strModel;
    }
    const string& GetModel() const
    {
        return Model;
    }
    string& GetModel()
    {
        return Model;
    }
    void SetFirmware(const string& strFirmware)
    {
        Firmware = strFirmware;
    }
    const string& GetFirmware() const
    {
        return Firmware;
    }
    string& GetFirmware()
    {
        return Firmware;
    }
public:
    void SetDeviceName(const string& strDeviceName)
    {
        DeviceName = strDeviceName;
    }
    const string& GetDeviceName() const
    {
        return DeviceName;
    }
    string& GetDeviceName()
    {
        return DeviceName;
    }
    void SetChannel(const int nChannel)
    {
        Channel = nChannel;
    }
    const int GetChannel() const
    {
        return Channel;
    }
    void SetInfo(const TDeviceInfoExt& tInfo)
    {
        Info = tInfo;
    }
    const TDeviceInfoExt& GetInfo() const
    {
        return Info;
    }
    TDeviceInfoExt& GetInfo()
    {
        return Info;
    }
};

class CGBDevStatusQryReq : public CGBQuery
{
public:
    CGBDevStatusQryReq()
    {
        CmdType = GBCMDTYPE_DEVICESTATUS;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBDevStatusQryRsp : public CGBResponse
{
public:
    CGBDevStatusQryRsp()
    {
        CmdType = GBCMDTYPE_DEVICESTATUS;
    }
private:
    string Online;
    string Status;
    string Encode;
    string Record;
    string DeviceTime;
    vector<TGBAlarmstatus> Alarmstatus;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetOnline(const string& strOnline)
    {
        Online = strOnline;
    }
    const string& GetOnline() const
    {
        return Online;
    }
    string& GetOnline()
    {
        return Online;
    }
    void SetStatus(const string& strStatus)
    {
        Status = strStatus;
    }
    const string& GetStatus() const
    {
        return Status;
    }
    string& GetStatus()
    {
        return Status;
    }
    void SetEncode(const string& strEncode)
    {
        Encode = strEncode;
    }
    const string& GetEncode() const
    {
        return Encode;
    }
    string& GetEncode()
    {
        return Encode;
    }
    void SetRecord(const string& strRecord)
    {
        Record = strRecord;
    }
    const string& GetRecord() const
    {
        return Record;
    }
    string& GetRecord()
    {
        return Record;
    }
    void SetDeviceTime(const string& strDeviceTime)
    {
        DeviceTime = strDeviceTime;
    }
    const string& GetDeviceTime() const
    {
        return DeviceTime;
    }
    string& GetDeviceTime()
    {
        return DeviceTime;
    }
    void SetAlarmstatus(const vector<TGBAlarmstatus>& tAlarmstatus)
    {
        Alarmstatus = tAlarmstatus;
    }
    const vector<TGBAlarmstatus>& GetAlarmstatus() const
    {
        return Alarmstatus;
    }
    vector<TGBAlarmstatus>& GetAlarmstatus()
    {
        return Alarmstatus;
    }
};

class CGBRecordInfoQryReq : public CGBQuery
{
public:
    CGBRecordInfoQryReq()
    {
        CmdType = GBCMDTYPE_RecordInfo;
		Secrecy = 0;
		IndistinctQuery = NOT_INDISTINCT_QUERY;
    }
private:
    string StartTime;
    string EndTime;
    string FilePath;
	string Address;
    int Secrecy;
    string Type;
    string RecorderID;
    TKDMGBRecordInfoExt KDM;
    TInDistinctQuery IndistinctQuery;  
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartTime(const string& strStartTime)
    {
        StartTime = strStartTime;
    }
    const string& GetStartTime() const
    {
        return StartTime;
    }
    string& GetStartTime()
    {
        return StartTime;
    }
    void SetEndTime(const string& strEndTime)
    {
        EndTime = strEndTime;
    }
    const string& GetEndTime() const
    {
        return EndTime;
    }
    string& GetEndTime()
    {
        return EndTime;
    }
    void SetFilePath(const string& strFilePath)
    {
        FilePath = strFilePath;
    }
    const string& GetFilePath() const
    {
        return FilePath;
    }
    string& GetFilePath()
    {
        return FilePath;
    }
    void SetAddress(const string& strAddress)
    {
        Address = strAddress;
    }
    const string& GetAddress() const
    {
        return Address;
    }
    string& GetAddress()
    {
        return Address;
    }
    void SetSecrecy(const int nSecrecy)
    {
        Secrecy = nSecrecy;
    }
    const int GetSecrecy() const
    {
        return Secrecy;
    }
    void SetType(const string& strType)
    {
        Type = strType;
    }
    const string& GetType() const
    {
        return Type;
    }
    string& GetType()
    {
        return Type;
    }
    void SetRecorderID(const string& strRecorderID)
    {
        RecorderID = strRecorderID;
    }
    const string& GetRecorderID() const
    {
        return RecorderID;
    }
    string& GetRecorderID()
    {
        return RecorderID;
    }
    void SetKDM(const TKDMGBRecordInfoExt& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBRecordInfoExt& GetKDM() const
    {
        return KDM;
    }
    TKDMGBRecordInfoExt& GetKDM()
    {
        return KDM;
    }
public:
    void SetIndistinctQuery(const TInDistinctQuery& tIndistinctQuery)
    {
        IndistinctQuery = tIndistinctQuery;
    }
    const TInDistinctQuery& GetIndistinctQuery() const
    {
        return IndistinctQuery;
    }
    TInDistinctQuery& GetIndistinctQuery()
    {
        return IndistinctQuery;
    }
};

class CGBRecordInfoQryRsp : public CGBResponse
{
public:
    CGBRecordInfoQryRsp()
    {
        CmdType = GBCMDTYPE_RecordInfo;
    }
private:
    TKDMGBRecordInfoExt KDM;
    string Name;
    int SumNum;
    vector<TGBRecordInfo> RecordList;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKDM(const TKDMGBRecordInfoExt& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBRecordInfoExt& GetKDM() const
    {
        return KDM;
    }
    TKDMGBRecordInfoExt& GetKDM()
    {
        return KDM;
    }
    void SetName(const string& strName)
    {
        Name = strName;
    }
    const string& GetName() const
    {
        return Name;
    }
    string& GetName()
    {
        return Name;
    }
    void SetSumNum(const int nSumNum)
    {
        SumNum = nSumNum;
    }
    const int GetSumNum() const
    {
        return SumNum;
    }
    void SetRecordList(const vector<TGBRecordInfo>& tRecordList)
    {
        RecordList = tRecordList;
    }
    const vector<TGBRecordInfo>& GetRecordList() const
    {
        return RecordList;
    }
    vector<TGBRecordInfo>& GetRecordList()
    {
        return RecordList;
    }
};

class CGBMediaStatusNotifyReq : public CGBNotify
{
public:
    CGBMediaStatusNotifyReq()
    {
        CmdType = GBCMDTYPE_MediaStatus;
    }
private:
    int NotifyType;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetNotifyType(const int nNotifyType)
    {
        NotifyType = nNotifyType;
    }
    const int GetNotifyType() const
    {
        return NotifyType;
    }
};

class CGBKeepaliveReq: public CGBNotify
{
public:
    CGBKeepaliveReq()
    {
        CmdType = GBCMDTYPE_Keepalive;
    }
private:
    string Status;
    TFaultChnSet Info;  //故障列表
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStatus(const string& strStatus)
    {
        Status = strStatus;
    }
    const string& GetStatus() const
    {
        return Status;
    }
    string& GetStatus()
    {
        return Status;
    }
public:
    void SetInfo(const TFaultChnSet& tInfo)
    {
        Info = tInfo;
    }
    const TFaultChnSet& GetInfo() const
    {
        return Info;
    }
    TFaultChnSet& GetInfo()
    {
        return Info;
    }
};

//国标外设协议
class CKDMGBRegisterReq:public CGBQuery
{
public:
    CKDMGBRegisterReq()
    {
        CmdType = GBCMDTYPE_KDMRegister;
    }
private:
    TKDMGBRegisterInfo KDM;  //科达国标外设扩展信息
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKDM(const TKDMGBRegisterInfo& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBRegisterInfo& GetKDM() const
    {
        return KDM;
    }
    TKDMGBRegisterInfo& GetKDM()
    {
        return KDM;
    }
};

class CKDMGBAudioCallReq : public CGBControl
{
public:
    CKDMGBAudioCallReq()
    {
        CmdType = GBCMDTYPE_KDMAudioCall;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CKDMGBAudioCallRsp : public CGBResponse
{
public:
    CKDMGBAudioCallRsp()
    {
        CmdType = GBCMDTYPE_KDMAudioCall;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CKDMGBSubscribeDevReq : public CGBQuery
{
public:
    CKDMGBSubscribeDevReq()
    {
        CmdType = GBCMDTYPE_KDMStatus;
    }
private:
    TKDMGBDevStatusEventTypes KDM;
    string StartTime;
    string EndTime;

public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKDM(const TKDMGBDevStatusEventTypes& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBDevStatusEventTypes& GetKDM() const
    {
        return KDM;
    }
    TKDMGBDevStatusEventTypes& GetKDM()
    {
        return KDM;
    }
    void SetStartTime(const string& strStartTime)
    {
        StartTime = strStartTime;
    }
    const string& GetStartTime() const
    {
        return StartTime;
    }
    string& GetStartTime()
    {
        return StartTime;
    }
    void SetEndTime(const string& strEndTime)
    {
        EndTime = strEndTime;
    }
    const string& GetEndTime() const
    {
        return EndTime;
    }
    string& GetEndTime()
    {
        return EndTime;
    }
};

class CKDMGBSubscribeDevRsp : public CGBResponse
{
public:
    CKDMGBSubscribeDevRsp()
    {
        CmdType = GBCMDTYPE_KDMStatus;
    }
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CKDMGBDevStatusNotifyReq : public CGBNotify
{
public:
    CKDMGBDevStatusNotifyReq()
    {
        CmdType = GBCMDTYPE_KDMStatus;
    }
private:
    TKDMGBDevStatusEventTypes KDM;
    string Online;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKDM(const TKDMGBDevStatusEventTypes& tKDM)
    {
        KDM = tKDM;
    }
    const TKDMGBDevStatusEventTypes& GetKDM() const
    {
        return KDM;
    }
    TKDMGBDevStatusEventTypes& GetKDM()
    {
        return KDM;
    }
    void SetOnline(const string& strOnline)
    {
        Online = strOnline;
    }
    const string& GetOnline() const
    {
        return Online;
    }
    string& GetOnline()
    {
        return Online;
    }
};

typedef CKDMGBSubscribeDevRsp CKDMGBDevStatusNotifyRsp;

class CGBTransOperateReq : public CGBControl
{
public:
	CGBTransOperateReq()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
	}
public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);
private:
    int KD_AppCmd;
    TGBTransparentData KD_Data;
    
public:
    void SetKD_AppCmd(const int nKD_AppCmd)
    {
        KD_AppCmd = nKD_AppCmd;
    }
    const int GetKD_AppCmd() const
    {
        return KD_AppCmd;
    }
	int GetKD_AppCmd()
	{
		return KD_AppCmd;
	}
public:
    void SetKD_Data(const TGBTransparentData& strKD_Data)
    {
        KD_Data = strKD_Data;
    }
    const TGBTransparentData& GetKD_Data() const
    {
        return KD_Data;
    }
    TGBTransparentData& GetKD_Data()
    {
        return KD_Data;
    }
};

class CGBTransOperateRsp:public CGBResponse
{
public:
	CGBTransOperateRsp()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
		Result = GBRESULT_OK;
	}
private:
    int KD_AppCmd;
    TGBTransparentData KD_Data;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);

public:
    void SetKD_AppCmd(const int nKD_AppCmd)
    {
        KD_AppCmd = nKD_AppCmd;
    }
    const int GetKD_AppCmd() const
    {
        return KD_AppCmd;
    }
	int GetKD_AppCmd()
	{
		return KD_AppCmd;
	}
public:
    void SetKD_Data(const TGBTransparentData& strKD_Data)
    {
        KD_Data = strKD_Data;
    }
    const TGBTransparentData& GetKD_Data() const
    {
        return KD_Data;
    }
    TGBTransparentData& GetKD_Data()
    {
        return KD_Data;
    }
};

class CGBTransDataSubscribeReq:public CGBQuery
{
public:
	CGBTransDataSubscribeReq()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
	}
	
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBTransDataSubscribeRsp:public CGBResponse
{
public:
	CGBTransDataSubscribeRsp()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
		Result = GBRESULT_OK;
	}
public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBTransDataNtfReq : public CGBNotify
{
public:
	CGBTransDataNtfReq()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
	}
private:
    string KD_AppCmd;
    TGBTransparentData KD_Data;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKD_AppCmd(const string& strKD_AppCmd)
    {
        KD_AppCmd = strKD_AppCmd;
    }
    const string& GetKD_AppCmd() const
    {
        return KD_AppCmd;
    }
    string& GetKD_AppCmd()
    {
        return KD_AppCmd;
    }
public:
    void SetKD_Data(const TGBTransparentData& strKD_Data)
    {
        KD_Data = strKD_Data;
    }
    const TGBTransparentData& GetKD_Data() const
    {
        return KD_Data;
    }
    TGBTransparentData& GetKD_Data()
    {
        return KD_Data;
    }
};

class CGBTransDataNtfRsp : public CGBResponse
{
public:
	CGBTransDataNtfRsp()
	{
		CmdType = GBCMDTYPE_KD_AppMessage;
		Result = GBRESULT_OK;
	}
public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);

};

class CGBConfigDownloadReq : public CGBQuery
{
public:
	CGBConfigDownloadReq()
	{
		CmdType = GBCMDTYPE_ConfigDownload;
	}
private:
    string ConfigType;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetConfigType(const string& strConfigType)
    {
        ConfigType = strConfigType;
    }
    const string& GetConfigType() const
    {
        return ConfigType;
    }
    string& GetConfigType()
    {
        return ConfigType;
    }
};

class CGBConfigDownloadRsp : public CGBResponse
{
public:
	CGBConfigDownloadRsp()
	{
		CmdType = GBCMDTYPE_ConfigDownload;
	}
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
private:
    TConfigBasicParam BasicParam;
    TConfigVideoParamOpt VideoParamOpt;
    vector<TVideoParamAttributeType> VideoParamConfig;
    TConfigAudioParamOpt AudioParamOpt;
    vector<TAudioParamAttributeType> AudioParamConfig;
    TConfigSVACEncode SVACEncodeConfig;
    TConfigSVACDecode SVACDecodeConfig;
	//gbext
	vector<TGBVideoEncStreamType> KdmVideoEncConfig;
	TGBVideoInInfConfigType KdmVideoInInfConfig;

    TKDMGBVideoCoveredConfigType KdmVideoCoveredConfig;      //图像遮挡
    TKDMGBMotionDectionConfigType KdmMotionDectionConfig ;   //移动侦测
    string Info;
   
public:
    void SetBasicParam(const TConfigBasicParam& tBasicParam)
    {
        BasicParam = tBasicParam;
    }
    const TConfigBasicParam& GetBasicParam() const
    {
        return BasicParam;
    }
    TConfigBasicParam& GetBasicParam()
    {
        return BasicParam;
    }
    void SetVideoParamOpt(const TConfigVideoParamOpt& tVideoParamOpt)
    {
        VideoParamOpt = tVideoParamOpt;
    }
    const TConfigVideoParamOpt& GetVideoParamOpt() const
    {
        return VideoParamOpt;
    }
    TConfigVideoParamOpt& GetVideoParamOpt()
    {
        return VideoParamOpt;
    }
    void SetVideoParamConfig(const vector<TVideoParamAttributeType>& tVideoParamConfig)
    {
        VideoParamConfig = tVideoParamConfig;
    }
    const vector<TVideoParamAttributeType>& GetVideoParamConfig() const
    {
        return VideoParamConfig;
    }
    vector<TVideoParamAttributeType>& GetVideoParamConfig()
    {
        return VideoParamConfig;
    }
    void SetAudioParamOpt(const TConfigAudioParamOpt& tAudioParamOpt)
    {
        AudioParamOpt = tAudioParamOpt;
    }
    const TConfigAudioParamOpt& GetAudioParamOpt() const
    {
        return AudioParamOpt;
    }
    TConfigAudioParamOpt& GetAudioParamOpt()
    {
        return AudioParamOpt;
    }
    void SetAudioParamConfig(const vector<TAudioParamAttributeType>& tAudioParamConfig)
    {
        AudioParamConfig = tAudioParamConfig;
    }
    const vector<TAudioParamAttributeType>& GetAudioParamConfig() const
    {
        return AudioParamConfig;
    }
    vector<TAudioParamAttributeType>& GetAudioParamConfig()
    {
        return AudioParamConfig;
    }
    void SetSVACEncodeConfig(const TConfigSVACEncode& tSVACEncodeConfig)
    {
        SVACEncodeConfig = tSVACEncodeConfig;
    }
    const TConfigSVACEncode& GetSVACEncodeConfig() const
    {
        return SVACEncodeConfig;
    }
    TConfigSVACEncode& GetSVACEncodeConfig()
    {
        return SVACEncodeConfig;
    }
    void SetSVACDecodeConfig(const TConfigSVACDecode& tSVACDecodeConfig)
    {
        SVACDecodeConfig = tSVACDecodeConfig;
    }
    const TConfigSVACDecode& GetSVACDecodeConfig() const
    {
        return SVACDecodeConfig;
    }
    TConfigSVACDecode& GetSVACDecodeConfig()
    {
        return SVACDecodeConfig;
    }
	void SetKdmVideoEncConfig(const vector<TGBVideoEncStreamType>& tKdmVideoEncConfig)
	{
		KdmVideoEncConfig = tKdmVideoEncConfig;
	}
	const vector<TGBVideoEncStreamType>& GetKdmVideoEncConfig() const
	{
		return KdmVideoEncConfig;
	}
	vector<TGBVideoEncStreamType>& GetKdmVideoEncConfig()
	{
		return KdmVideoEncConfig;
	}
	void SetKdmVideoInInfConfig(const TGBVideoInInfConfigType& tKdmVideoInInfConfig)
	{
		KdmVideoInInfConfig = tKdmVideoInInfConfig;
	}
	const TGBVideoInInfConfigType& GetKdmVideoInInfConfig() const
	{
		return KdmVideoInInfConfig;
	}
	TGBVideoInInfConfigType& GetKdmVideoInInfConfig()
	{
		return KdmVideoInInfConfig;
	}
    void SetKdmVideoCoveredConfig(const TKDMGBVideoCoveredConfigType&  tKdmVideoCoveredConfig)
    {
        KdmVideoCoveredConfig = tKdmVideoCoveredConfig;
    }
    const TKDMGBVideoCoveredConfigType GetKdmVideoCoveredConfig() const
    {
        return KdmVideoCoveredConfig;
    }
    TKDMGBVideoCoveredConfigType& GetKdmVideoCoveredConfig()
    {
        return KdmVideoCoveredConfig;
    }
    void SetKdmMotionDectionConfig(const TKDMGBMotionDectionConfigType&  tKdmMotionDectionConfig)
    {
        KdmMotionDectionConfig = tKdmMotionDectionConfig;
    }
    const TKDMGBMotionDectionConfigType GetKdmMotionDectionConfig() const
    {
        return KdmMotionDectionConfig;
    }
    TKDMGBMotionDectionConfigType& GetKdmMotionDectionConfig()
    {
        return KdmMotionDectionConfig;
    }
};

class CGBAudioCallMessageReq: public CGBNotify
{
public:
	CGBAudioCallMessageReq()
	{
		CmdType = GBCMDTYPE_BROADCAST;
	}
private:
    string SourceID;
    string TargetID;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSourceID(const string& strSourceID)
    {
        SourceID = strSourceID;
    }
    const string& GetSourceID() const
    {
        return SourceID;
    }
    string& GetSourceID()
    {
        return SourceID;
    }
    void SetTargetID(const string& strTargetID)
    {
        TargetID = strTargetID;
    }
    const string& GetTargetID() const
    {
        return TargetID;
    }
    string& GetTargetID()
    {
        return TargetID;
    }
};

class CGBAudioCallMessageRsp: public CGBResponse
{
public:
	CGBAudioCallMessageRsp()
	{
		CmdType = GBCMDTYPE_BROADCAST;
	}
private:
    string TargetID;
    string Result;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetTargetID(const string& strTargetID)
    {
        TargetID = strTargetID;
    }
    const string& GetTargetID() const
    {
        return TargetID;
    }
    string& GetTargetID()
    {
        return TargetID;
    }
    void SetResult(const string& strResult)
    {
        Result = strResult;
    }
    const string& GetResult() const
    {
        return Result;
    }
    string& GetResult()
    {
        return Result;
    }
};

class CGBAlarmSubscribeReq : public CGBQuery
{
public:
	CGBAlarmSubscribeReq()
	{
		CmdType = GBCMDTYPE_ALARM;
	}
private:
    string	StartAlarmPriority;	//报警起始级别(可选)
    string	EndAlarmPriority;	//报警终止级别(可选)
    string	AlarmMethod;		//报警方式(可选)
    string	StartAlarmTime;		//报警发生时间（可选）
    string	EndAlarmTime;		//报警终止时间（可选）

    TAlarmInfoExt Info;			//扩展信息
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartAlarmPriority(const string& strStartAlarmPriority)
    {
        StartAlarmPriority = strStartAlarmPriority;
    }
    const string& GetStartAlarmPriority() const
    {
        return StartAlarmPriority;
    }
    string& GetStartAlarmPriority()
    {
        return StartAlarmPriority;
    }
    void SetEndAlarmPriority(const string& strEndAlarmPriority)
    {
        EndAlarmPriority = strEndAlarmPriority;
    }
    const string& GetEndAlarmPriority() const
    {
        return EndAlarmPriority;
    }
    string& GetEndAlarmPriority()
    {
        return EndAlarmPriority;
    }
    void SetAlarmMethod(const string& strAlarmMethod)
    {
        AlarmMethod = strAlarmMethod;
    }
    const string& GetAlarmMethod() const
    {
        return AlarmMethod;
    }
    string& GetAlarmMethod()
    {
        return AlarmMethod;
    }
    void SetStartAlarmTime(const string& strStartAlarmTime)
    {
        StartAlarmTime = strStartAlarmTime;
    }
    const string& GetStartAlarmTime() const
    {
        return StartAlarmTime;
    }
    string& GetStartAlarmTime()
    {
        return StartAlarmTime;
    }
    void SetEndAlarmTime(const string& strEndAlarmTime)
    {
        EndAlarmTime = strEndAlarmTime;
    }
    const string& GetEndAlarmTime() const
    {
        return EndAlarmTime;
    }
    string& GetEndAlarmTime()
    {
        return EndAlarmTime;
    }
    void SetInfo(const TAlarmInfoExt& tInfo)
    {
        Info = tInfo;
    }
    const TAlarmInfoExt& GetInfo() const
    {
        return Info;
    }
    TAlarmInfoExt& GetInfo()
    {
        return Info;
    }
};

class CGBAlarmNtfReq : public CGBNotify
{
public:
	CGBAlarmNtfReq()
	{
		CmdType = GBCMDTYPE_ALARM;
		Longitude = INVALID_LONGITUDE;
		Latitude = INVALID_LATITUDE;
	}
private:
    int		AlarmPriority;			//报警级别（必选）
    int 	AlarmMethod;			//报警方式（必选）
    string	AlarmTime;				//报警时间（必选）
    string	AlarmDescription;		//报警内容描述（可选）
    double  Longitude;				//经度（可选）
    double  Latitude;				//纬度（可选）

    TAlarmInfoExt Info;				//扩展信息
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetAlarmPriority(const int nAlarmPriority)
    {
        AlarmPriority = nAlarmPriority;
    }
    const int GetAlarmPriority() const
    {
        return AlarmPriority;
    }
    void SetAlarmMethod(const int nAlarmMethod)
    {
        AlarmMethod = nAlarmMethod;
    }
    const int GetAlarmMethod() const
    {
        return AlarmMethod;
    }
    void SetAlarmTime(const string& strAlarmTime)
    {
        AlarmTime = strAlarmTime;
    }
    const string& GetAlarmTime() const
    {
        return AlarmTime;
    }
    string& GetAlarmTime()
    {
        return AlarmTime;
    }
    void SetAlarmDescription(const string& strAlarmDescription)
    {
        AlarmDescription = strAlarmDescription;
    }
    const string& GetAlarmDescription() const
    {
        return AlarmDescription;
    }
    string& GetAlarmDescription()
    {
        return AlarmDescription;
    }
    void SetLongitude(const double fLongitude)
    {
        Longitude = fLongitude;
    }
    const double GetLongitude() const
    {
        return Longitude;
    }
    void SetLatitude(const double fLatitude)
    {
        Latitude = fLatitude;
    }
    const double GetLatitude() const
    {
        return Latitude;
    }
    void SetInfo(const TAlarmInfoExt& tInfo)
    {
        Info = tInfo;
    }
    const TAlarmInfoExt& GetInfo() const
    {
        return Info;
    }
    TAlarmInfoExt& GetInfo()
    {
        return Info;
    }
};

class CGBAlarmSubscribeRsp : public CGBResponse
{
public:
	CGBAlarmSubscribeRsp()
	{
		CmdType = GBCMDTYPE_ALARM;
	}
private:
    string Result;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetResult(const string& strResult)
    {
        Result = strResult;
    }
    const string& GetResult() const
    {
        return Result;
    }
    string& GetResult()
    {
        return Result;
    }
};

typedef CGBAlarmSubscribeRsp CGBAlarmNtyRsp;

class CGBDragZoomInReq : public CGBControl
{
public:
    CGBDragZoomInReq()
    {
        CmdType = GBCMDTYPE_DEVICE_CONTROL;
    }
private:
    TDragZoom DragZoomIn;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDragZoomIn(const TDragZoom& tDragZoomIn)
    {
        DragZoomIn = tDragZoomIn;
    }
    const TDragZoom& GetDragZoomIn() const
    {
        return DragZoomIn;
    }
    TDragZoom& GetDragZoomIn()
    {
        return DragZoomIn;
    }
};

class CGBDragZoomOutReq : public CGBControl
{
public:
    CGBDragZoomOutReq()
    {
        CmdType = GBCMDTYPE_DEVICE_CONTROL;
    }
private:
    TDragZoom DragZoomOut;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDragZoomOut(const TDragZoom& tDragZoomOut)
    {
        DragZoomOut = tDragZoomOut;
    }
    const TDragZoom& GetDragZoomOut() const
    {
        return DragZoomOut;
    }
    TDragZoom& GetDragZoomOut()
    {
        return DragZoomOut;
    }
};

class CGBGpsSubcribeReq : public CGBQuery
{
public:
	CGBGpsSubcribeReq()
	{
		CmdType = GBCMTYPE_GPS;
	}
private:
    string StartTime;
    string EndTime;
public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStartTime(const xsd__dateTimer& tStartTime)
    {
        StartTime = tStartTime;
    }
    const xsd__dateTimer& GetStartTime() const
    {
        return StartTime;
    }
    xsd__dateTimer& GetStartTime()
    {
        return StartTime;
    }
    void SetEndTime(const xsd__dateTimer& tEndTime)
    {
        EndTime = tEndTime;
    }
    const xsd__dateTimer& GetEndTime() const
    {
        return EndTime;
    }
    xsd__dateTimer& GetEndTime()
    {
        return EndTime;
    }
};

class CGBGpsSubscribeRsp : public CGBResponse
{
public:
	CGBGpsSubscribeRsp()
	{
		CmdType = GBCMTYPE_GPS;
		Result = GBRESULT_OK;
	}

public:
	virtual void Clear();
	virtual const std::string ToXml()const;
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const std::string& xml_string);
	virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGBGpsDataNtfReq : public CGBNotify
{
public:
	CGBGpsDataNtfReq()
	{
		CmdType = GBCMTYPE_GPS;
	}
private:
    int SumNum; 
    vector<TGbGpsItem> DeviceList;  //dev list

    
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSumNum(const int nSumNum)
    {
        SumNum = nSumNum;
    }
    const int GetSumNum() const
    {
        return SumNum;
    }
    void SetDeviceList(const vector<TGbGpsItem>& tDeviceList)
    {
        DeviceList = tDeviceList;
    }
    const vector<TGbGpsItem>& GetDeviceList() const
    {
        return DeviceList;
    }
    vector<TGbGpsItem>& GetDeviceList()
    {
        return DeviceList;
    }
};

class CGBGpsDataNtfRsp : public CGBResponse
{
public:
	CGBGpsDataNtfRsp()
	{
		CmdType = GBCMTYPE_GPS;
		Result = GBRESULT_OK;
	}
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
};

class CGbMobilePositionSubscribeReq : public CGBQuery
{
public:
	CGbMobilePositionSubscribeReq()
	{
		CmdType = GBCMDTYPE_MobilePosition;
		Interval = 5;
	}
private:
    int Interval; //移动设备位置信息上报时间间隔，可选，默认5(单位：s)
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetInterval(const int nInterval)
    {
        Interval = nInterval;
    }
    const int GetInterval() const
    {
        return Interval;
    }
};


class CGbMobilePositionDataNtfExtReq:public CGBNotify
{
public:
    CGbMobilePositionDataNtfExtReq()
    {
        CmdType = GBCMDTYPE_MobilePosition;        
    }
private:
    vector<TGbMobilePositionDataExtend> GpsItemList;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetGpsItemList(const vector<TGbMobilePositionDataExtend>& tGpsitemList)
    {
        GpsItemList = tGpsitemList;
    }
    const vector<TGbMobilePositionDataExtend>& GetGpsItemList() const
    {
        return GpsItemList;
    }
    vector<TGbMobilePositionDataExtend>& GetGpsItemList()
    {
        return GpsItemList;
    }
};

class CGbMobilePositionDataNtfReq:public CGBNotify
{
public:
	CGbMobilePositionDataNtfReq()
	{
		CmdType = GBCMDTYPE_MobilePosition;
	}
private:
    string Time;
    double Longitude;
    double Latitude;
    double Speed;
    double Direction;
    double Altitude;

public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetTime(const string& strTime)
    {
        Time = strTime;
    }
    const string& GetTime() const
    {
        return Time;
    }
    string& GetTime()
    {
        return Time;
    }
    void SetLongitude(const double fLongitude)
    {
        Longitude = fLongitude;
    }
    const double GetLongitude() const
    {
        return Longitude;
    }
    void SetLatitude(const double fLatitude)
    {
        Latitude = fLatitude;
    }
    const double GetLatitude() const
    {
        return Latitude;
    }
    void SetSpeed(const double fSpeed)
    {
        Speed = fSpeed;
    }
    const double GetSpeed() const
    {
        return Speed;
    }
    void SetDirection(const double fDirection)
    {
        Direction = fDirection;
    }
    const double GetDirection() const
    {
        return Direction;
    }
public:
    void SetAltitude(const double fAltitude)
    {
        Altitude = fAltitude;
    }
    const double GetAltitude() const
    {
        return Altitude;
    }
};

class CGbIFameCmd : public CGBControl
{
public:
	CGbIFameCmd()
	{
		IFameCmd = "Send";
	}

private:
    string IFameCmd;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetIFameCmd(const string& strIFameCmd)
    {
        IFameCmd = strIFameCmd;
    }
    const string& GetIFameCmd() const
    {
        return IFameCmd;
    }
    string& GetIFameCmd()
    {
        return IFameCmd;
    }
};

class CGbKdmCapabilityReq : public CGBQuery
{
public:
	CGbKdmCapabilityReq()
	{
		CmdType = GBCMDTYPE_KDMCAPABILITY;
	}
private:
    TKdmCapability KdmCapability;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdmCapability(const TKdmCapability& tKdmCapability)
    {
        KdmCapability = tKdmCapability;
    }
    const TKdmCapability& GetKdmCapability() const
    {
        return KdmCapability;
    }
    TKdmCapability& GetKdmCapability()
    {
        return KdmCapability;
    }
};

class CGbKdmCapabilityRsp : public CGBResponse
{
public:
	CGbKdmCapabilityRsp()
	{
		CmdType = GBCMDTYPE_KDMCAPABILITY;
	}
private:
    TKdmCapability KdmCapability;
public:
    virtual void Clear();
    virtual const std::string ToXml()const;
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const std::string& xml_string);
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetKdmCapability(const TKdmCapability& tKdmCapability)
    {
        KdmCapability = tKdmCapability;
    }
    const TKdmCapability& GetKdmCapability() const
    {
        return KdmCapability;
    }
    TKdmCapability& GetKdmCapability()
    {
        return KdmCapability;
    }
};

//End of file
