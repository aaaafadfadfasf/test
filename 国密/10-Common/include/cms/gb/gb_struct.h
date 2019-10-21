/*
这里保存了国标协议中的协议类用到的struct
*/

#pragma once
#include "gbbase_proto.h"
#include "gbspecial_struct.h"

//以下是各个协议类用到的struct，自动生成cpp



//keda扩展国标视频编码通道配置结构体
struct  TGBVideoEncStreamType : public TGBStruct
{
public:
    TGBVideoEncStreamType()
    {
        StreamName.clear();
        MaxQuantization = 0;
        MinQuantization = 0;
        Interval = 0;
        H264Profile.clear();
        H264Level.clear();
        Mpeg4Profile.clear();
        Mpeg4Level.clear();
    }
private:
    string StreamName;
    int MaxQuantization;
    int MinQuantization;
    int Interval;         //关键帧间隔
    string H264Profile;   //包含Baseline,Main,Extended,High四个档次
    string H264Level;     //级别，从1~4
    string Mpeg4Profile;  //档次，SP和ASP
    string Mpeg4Level;    //级别，从L0~L5
public:
    virtual bool empty() const
    {
        return StreamName.empty();
    }
public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStreamName(const string& strStreamName)
    {
        StreamName = strStreamName;
    }
    const string& GetStreamName() const
    {
        return StreamName;
    }
    string& GetStreamName()
    {
        return StreamName;
    }
    void SetMaxQuantization(const int nMaxQuantization)
    {
        MaxQuantization = nMaxQuantization;
    }
    const int GetMaxQuantization() const
    {
        return MaxQuantization;
    }
    void SetMinQuantization(const int nMinQuantization)
    {
        MinQuantization = nMinQuantization;
    }
    const int GetMinQuantization() const
    {
        return MinQuantization;
    }
    void SetInterval(const int nInterval)
    {
        Interval = nInterval;
    }
    const int GetInterval() const
    {
        return Interval;
    }
    void SetH264Profile(const string& strH264Profile)
    {
        H264Profile = strH264Profile;
    }
    const string& GetH264Profile() const
    {
        return H264Profile;
    }
    string& GetH264Profile()
    {
        return H264Profile;
    }
    void SetH264Level(const string& strH264Level)
    {
        H264Level = strH264Level;
    }
    const string& GetH264Level() const
    {
        return H264Level;
    }
    string& GetH264Level()
    {
        return H264Level;
    }
    void SetMpeg4Profile(const string& strMpeg4Profile)
    {
        Mpeg4Profile = strMpeg4Profile;
    }
    const string& GetMpeg4Profile() const
    {
        return Mpeg4Profile;
    }
    string& GetMpeg4Profile()
    {
        return Mpeg4Profile;
    }
    void SetMpeg4Level(const string& strMpeg4Level)
    {
        Mpeg4Level = strMpeg4Level;
    }
    const string& GetMpeg4Level() const
    {
        return Mpeg4Level;
    }
    string& GetMpeg4Level()
    {
        return Mpeg4Level;
    }
};


struct TGBProtocolExtensionVersionInfo : public TGBStruct
{
public:
    TGBProtocolExtensionVersionInfo()
    {
        Major = 0;
        Minor = 0;
    }
private:
    int Major;
    int Minor;
public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    virtual bool empty() const
    {
        return Major==0 && Minor==0 ;
    }
    void SetMajor(const int nMajor)
    {
        Major = nMajor;
    }
    const int GetMajor() const
    {
        return Major;
    }
    void SetMinor(const int nMinor)
    {
        Minor = nMinor;
    }
    const int GetMinor() const
    {
        return Minor;
    }
};

struct TGBProtocolExtensionInfo : public TGBStruct
{
public:
    TGBProtocolExtensionInfo()
    {
        ProtocolName.clear();
        ExtensionName.clear();
        Version.Clear();
    }
private:
    string ProtocolName;
    string ExtensionName;
    TGBProtocolExtensionVersionInfo Version;

public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);

public:
    virtual bool empty() const
    {
        return ProtocolName.empty() && ExtensionName.empty() && Version.empty();
    }
    void SetProtocolName(const string& strProtocolName)
    {
        ProtocolName = strProtocolName;
    }
    const string& GetProtocolName() const
    {
        return ProtocolName;
    }
    string& GetProtocolName()
    {
        return ProtocolName;
    }
    void SetExtensionName(const string& strExtensionName)
    {
        ExtensionName = strExtensionName;
    }
    const string& GetExtensionName() const
    {
        return ExtensionName;
    }
    string& GetExtensionName()
    {
        return ExtensionName;
    }
    void SetVersion(const TGBProtocolExtensionVersionInfo& tVersion)
    {
        Version = tVersion;
    }
    const TGBProtocolExtensionVersionInfo& GetVersion() const
    {
        return Version;
    }
    TGBProtocolExtensionVersionInfo& GetVersion()
    {
        return Version;
    }
};


struct TGBPTZInfo : public TGBStruct
{
public:
    TGBPTZInfo()
    {
        ControlPriority = 0;
    }
private:
    int ControlPriority;
public:
    virtual bool empty() const
    {
        return false;
    }
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetControlPriority(const int nControlPriority)
    {
        ControlPriority = nControlPriority;
    }
    const int GetControlPriority() const
    {
        return ControlPriority;
    }
};
struct TDeviceInfoExt :public TGBStruct
{
	TDeviceInfoExt()
	{

	}
public:
	virtual bool empty() const
	{
		return true;
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
};
struct TGBAlarmstatus : public TGBStruct
{
public:
    TGBAlarmstatus()
    {
    }
private:
    string DeviceID;
    string DutyStatus;
public:
    virtual bool empty() const
    {
        return DeviceID.empty() && DutyStatus.empty();
    }
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
    }
    void SetDutyStatus(const string& strDutyStatus)
    {
        DutyStatus = strDutyStatus;
    }
    const string& GetDutyStatus() const
    {
        return DutyStatus;
    }
    string& GetDutyStatus()
    {
        return DutyStatus;
    }
};

struct TGBRecordInfo : public TGBStruct
{
public:
    TGBRecordInfo()
    {
		Secrecy = 0;
    }
private:
    string DeviceID;
    string Name;
    string FilePath;
    string Address;
    string StartTime;
    string EndTime;
    int Secrecy;
    string Type;
    string RecorderID;
public:
    virtual bool empty() const
    {
        return false;
    }
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
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
};

//国标外设结构体
struct TKDMGBRegisterInfo : public TGBStruct
{
public:
    TKDMGBRegisterInfo()
    {
    }
private:
    string Name;
public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
    virtual bool empty() const
    {
        return Name.empty();
    }
public:
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
};

struct TKDMGBRecordInfoExt : public TGBStruct
{
public:
    TKDMGBRecordInfoExt()
    {
    }
private:
    string Domain;
    string FileAddress;
public:
    virtual bool empty() const
    {
        return Domain.empty() && FileAddress.empty();
    }
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDomain(const string& strDomain)
    {
        Domain = strDomain;
    }
    const string& GetDomain() const
    {
        return Domain;
    }
    string& GetDomain()
    {
        return Domain;
    }
    void SetFileAddress(const string& strFileAddress)
    {
        FileAddress = strFileAddress;
    }
    const string& GetFileAddress() const
    {
        return FileAddress;
    }
    string& GetFileAddress()
    {
        return FileAddress;
    }
};

struct TDeviceConfigBasicParam: public TGBStruct
{
public:
    TDeviceConfigBasicParam()
    {
        Expiration=3600;
    }
    virtual bool empty() const
    {
        return Expiration==0;
    }
private:
    // string Name;
    int Expiration;
    // 其它字段先不实现
public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetExpiration(const int nExpiration)
    {
        Expiration = nExpiration;
    }
    const int GetExpiration() const
    {
        return Expiration;
    }
};

struct TConfigBasicParamSimpleType : public TGBStruct
{
public:
	TConfigBasicParamSimpleType()
	{
		Name.clear();
		DeviceID.clear();
		SIPServerID.clear();
		SIPServerIP.clear();
		SIPServerPort = 0;
		DomainName.clear();
		Expiration = 0;
		Password.clear();
		HeartBeatInterval = 0;
		HeartBeatCount = 0;
	}
private:
    string Name;
    string DeviceID;
    string SIPServerID;
    string SIPServerIP;
    int    SIPServerPort;
    string DomainName;
    int    Expiration;
    string Password;
    int    HeartBeatInterval;
    int    HeartBeatCount;
public:
	virtual bool empty() const
	{
		return Name.empty() && DeviceID.empty() && SIPServerID.empty() && SIPServerIP.empty() && SIPServerPort==0
			   && DomainName.empty() && Expiration==0 && Password.empty() && HeartBeatInterval==0 && HeartBeatCount==0;
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
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
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
    }
    void SetSIPServerID(const string& strSIPServerID)
    {
        SIPServerID = strSIPServerID;
    }
    const string& GetSIPServerID() const
    {
        return SIPServerID;
    }
    string& GetSIPServerID()
    {
        return SIPServerID;
    }
    void SetSIPServerIP(const string& strSIPServerIP)
    {
        SIPServerIP = strSIPServerIP;
    }
    const string& GetSIPServerIP() const
    {
        return SIPServerIP;
    }
    string& GetSIPServerIP()
    {
        return SIPServerIP;
    }
    void SetSIPServerPort(const int nSIPServerPort)
    {
        SIPServerPort = nSIPServerPort;
    }
    const int GetSIPServerPort() const
    {
        return SIPServerPort;
    }
    void SetDomainName(const string& strDomainName)
    {
        DomainName = strDomainName;
    }
    const string& GetDomainName() const
    {
        return DomainName;
    }
    string& GetDomainName()
    {
        return DomainName;
    }
    void SetExpiration(const int nExpiration)
    {
        Expiration = nExpiration;
    }
    const int GetExpiration() const
    {
        return Expiration;
    }
    void SetPassword(const string& strPassword)
    {
        Password = strPassword;
    }
    const string& GetPassword() const
    {
        return Password;
    }
    string& GetPassword()
    {
        return Password;
    }
    void SetHeartBeatInterval(const int nHeartBeatInterval)
    {
        HeartBeatInterval = nHeartBeatInterval;
    }
    const int GetHeartBeatInterval() const
    {
        return HeartBeatInterval;
    }
    void SetHeartBeatCount(const int nHeartBeatCount)
    {
        HeartBeatCount = nHeartBeatCount;
    }
    const int GetHeartBeatCount() const
    {
        return HeartBeatCount;
    }
};

struct TConfigBasicParam : public TGBStruct
{
public:
	TConfigBasicParam()
	{
		simpleType.Clear();
	}
private:
    TConfigBasicParamSimpleType simpleType;
public:
	virtual bool empty() const
	{
		return simpleType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetsimpleType(const TConfigBasicParamSimpleType& tSimpleType)
    {
        simpleType = tSimpleType;
    }
    const TConfigBasicParamSimpleType& GetsimpleType() const
    {
        return simpleType;
    }
    TConfigBasicParamSimpleType& GetsimpleType()
    {
        return simpleType;
    }
};

struct TConfigVideoParamOpt : public TGBStruct
{
public:
	TConfigVideoParamOpt()
	{
		VideoFormatOpt.clear();
		ResolutionOpt.clear();
		FrameRateOpt = 0;
		BitRateTypeOpt = 0;
		VideoBitRateOpt = 0;
		DownloadSpeedOpt.clear();
	}
private:
    string VideoFormatOpt;
    string ResolutionOpt;
    int    FrameRateOpt;
    int    BitRateTypeOpt;
    int    VideoBitRateOpt;
    string DownloadSpeedOpt;
public:
	virtual bool empty() const
	{
		return VideoFormatOpt.empty() && ResolutionOpt.empty() && FrameRateOpt == 0
			&& BitRateTypeOpt == 0 && VideoBitRateOpt == 0 && DownloadSpeedOpt.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetVideoFormatOpt(const string& strVideoFormatOpt)
    {
        VideoFormatOpt = strVideoFormatOpt;
    }
    const string& GetVideoFormatOpt() const
    {
        return VideoFormatOpt;
    }
    string& GetVideoFormatOpt()
    {
        return VideoFormatOpt;
    }
    void SetResolutionOpt(const string& strResolutionOpt)
    {
        ResolutionOpt = strResolutionOpt;
    }
    const string& GetResolutionOpt() const
    {
        return ResolutionOpt;
    }
    string& GetResolutionOpt()
    {
        return ResolutionOpt;
    }
    void SetFrameRateOpt(const int nFrameRateOpt)
    {
        FrameRateOpt = nFrameRateOpt;
    }
    const int GetFrameRateOpt() const
    {
        return FrameRateOpt;
    }
    void SetBitRateTypeOpt(const int nBitRateTypeOpt)
    {
        BitRateTypeOpt = nBitRateTypeOpt;
    }
    const int GetBitRateTypeOpt() const
    {
        return BitRateTypeOpt;
    }
    void SetVideoBitRateOpt(const int nVideoBitRateOpt)
    {
        VideoBitRateOpt = nVideoBitRateOpt;
    }
    const int GetVideoBitRateOpt() const
    {
        return VideoBitRateOpt;
    }
    void SetDownloadSpeedOpt(const string& strDownloadSpeedOpt)
    {
        DownloadSpeedOpt = strDownloadSpeedOpt;
    }
    const string& GetDownloadSpeedOpt() const
    {
        return DownloadSpeedOpt;
    }
    string& GetDownloadSpeedOpt()
    {
        return DownloadSpeedOpt;
    }
};

struct TVideoParamAttributeType : public TGBStruct
{
public:
	TVideoParamAttributeType()
	{
		StreamName.clear();
		VideoFormat.clear();
		Resolution.clear();
		FrameRate.clear();
		BitRateType.clear();
		VideoBitRate.clear();
	}
private:
    string StreamName;
    string VideoFormat;
    string Resolution;
    string FrameRate;
    string BitRateType;
    string VideoBitRate;
public:
	virtual bool empty() const
	{
		return StreamName.empty() && VideoFormat.empty() && Resolution.empty()
			&& FrameRate.empty() && BitRateType.empty() && VideoBitRate.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStreamName(const string& strStreamName)
    {
        StreamName = strStreamName;
    }
    const string& GetStreamName() const
    {
        return StreamName;
    }
    string& GetStreamName()
    {
        return StreamName;
    }
    void SetVideoFormat(const string& strVideoFormat)
    {
        VideoFormat = strVideoFormat;
    }
    const string& GetVideoFormat() const
    {
        return VideoFormat;
    }
    string& GetVideoFormat()
    {
        return VideoFormat;
    }
    void SetResolution(const string& strResolution)
    {
        Resolution = strResolution;
    }
    const string& GetResolution() const
    {
        return Resolution;
    }
    string& GetResolution()
    {
        return Resolution;
    }
    void SetFrameRate(const string& strFrameRate)
    {
        FrameRate = strFrameRate;
    }
    const string& GetFrameRate() const
    {
        return FrameRate;
    }
    string& GetFrameRate()
    {
        return FrameRate;
    }
    void SetBitRateType(const string& strBitRateType)
    {
        BitRateType = strBitRateType;
    }
    const string& GetBitRateType() const
    {
        return BitRateType;
    }
    string& GetBitRateType()
    {
        return BitRateType;
    }
    void SetVideoBitRate(const string& strVideoBitRate)
    {
        VideoBitRate = strVideoBitRate;
    }
    const string& GetVideoBitRate() const
    {
        return VideoBitRate;
    }
    string& GetVideoBitRate()
    {
        return VideoBitRate;
    }
};

struct TConfigAudioParamOptSimpleType : public TGBStruct
{
public:
	TConfigAudioParamOptSimpleType()
	{
		AudioFormatOpt.clear();
		AudioBitRateOpt = 0;
		SamplingRateOpt.clear();
	}
private:
    string AudioFormatOpt;
    int    AudioBitRateOpt;
    string SamplingRateOpt;
public:
	virtual bool empty() const
	{
		return AudioFormatOpt.empty() && AudioBitRateOpt==0 && SamplingRateOpt.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetAudioFormatOpt(const string& strAudioFormatOpt)
    {
        AudioFormatOpt = strAudioFormatOpt;
    }
    const string& GetAudioFormatOpt() const
    {
        return AudioFormatOpt;
    }
    string& GetAudioFormatOpt()
    {
        return AudioFormatOpt;
    }
    void SetAudioBitRateOpt(const int nAudioBitRateOpt)
    {
        AudioBitRateOpt = nAudioBitRateOpt;
    }
    const int GetAudioBitRateOpt() const
    {
        return AudioBitRateOpt;
    }
    void SetSamplingRateOpt(const string& strSamplingRateOpt)
    {
        SamplingRateOpt = strSamplingRateOpt;
    }
    const string& GetSamplingRateOpt() const
    {
        return SamplingRateOpt;
    }
    string& GetSamplingRateOpt()
    {
        return SamplingRateOpt;
    }
};

struct TConfigAudioParamOpt : public TGBStruct
{
public:
	TConfigAudioParamOpt()
	{
		simpleType.Clear();
	}
private:
    TConfigAudioParamOptSimpleType simpleType;
public:
	virtual bool empty() const
	{
		return simpleType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetsimpleType(const TConfigAudioParamOptSimpleType& tSimpleType)
    {
        simpleType = tSimpleType;
    }
    const TConfigAudioParamOptSimpleType& GetsimpleType() const
    {
        return simpleType;
    }
    TConfigAudioParamOptSimpleType& GetsimpleType()
    {
        return simpleType;
    }
};

struct TAudioParamAttributeType : public TGBStruct
{
public:
	TAudioParamAttributeType()
	{
		StreamName.clear();
		AudioFormat.clear();
		AudioBitRate.clear();
		SamplingRate.clear();
	}
private:
    string StreamName;
    string AudioFormat;
    string AudioBitRate;
    string SamplingRate;
public:
	virtual bool empty() const
	{
		return StreamName.empty() && AudioFormat.empty() 
			&& AudioBitRate.empty() && SamplingRate.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetStreamName(const string& strStreamName)
    {
        StreamName = strStreamName;
    }
    const string& GetStreamName() const
    {
        return StreamName;
    }
    string& GetStreamName()
    {
        return StreamName;
    }
    void SetAudioFormat(const string& strAudioFormat)
    {
        AudioFormat = strAudioFormat;
    }
    const string& GetAudioFormat() const
    {
        return AudioFormat;
    }
    string& GetAudioFormat()
    {
        return AudioFormat;
    }
    void SetAudioBitRate(const string& strAudioBitRate)
    {
        AudioBitRate = strAudioBitRate;
    }
    const string& GetAudioBitRate() const
    {
        return AudioBitRate;
    }
    string& GetAudioBitRate()
    {
        return AudioBitRate;
    }
    void SetSamplingRate(const string& strSamplingRate)
    {
        SamplingRate = strSamplingRate;
    }
    const string& GetSamplingRate() const
    {
        return SamplingRate;
    }
    string& GetSamplingRate()
    {
        return SamplingRate;
    }
};

struct TRoiParamItem : public TGBStruct
{
public:
	TRoiParamItem()
	{
		ROISeq = 0;
		TopLeft = 0;
		BottomRight = 0;
		ROIQP = 0;
	}
private:
    int ROISeq;
    int TopLeft;
    int BottomRight;
    int ROIQP;
	
public:
	virtual bool empty() const
	{
		return (ROISeq == 0)&&(TopLeft == 0)&&(BottomRight == 0)&&(ROIQP == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetROISeq(const int nROISeq)
    {
        ROISeq = nROISeq;
    }
    const int GetROISeq() const
    {
        return ROISeq;
    }
    void SetTopLeft(const int nTopLeft)
    {
        TopLeft = nTopLeft;
    }
    const int GetTopLeft() const
    {
        return TopLeft;
    }
    void SetBottomRight(const int nBottomRight)
    {
        BottomRight = nBottomRight;
    }
    const int GetBottomRight() const
    {
        return BottomRight;
    }
    void SetROIQP(const int nROIQP)
    {
        ROIQP = nROIQP;
    }
    const int GetROIQP() const
    {
        return ROIQP;
    }
};

struct TConfigROIParamComplexTypeSequence : public TGBStruct
{
public:
	TConfigROIParamComplexTypeSequence()
	{
		ROIFlag = 0;
		ROINumber = 0;
		Item.Clear();
		BackGroundQP = 0;
		BackGroundSkipFlag = 0;
	}
private:
    int ROIFlag;
    int ROINumber;
    TRoiParamItem Item;
    int BackGroundQP;
    int BackGroundSkipFlag;

public:
	virtual bool empty() const
	{
		return (ROIFlag == 0)&&(ROINumber == 0)&&(Item.empty())
			&& (BackGroundQP == 0) &&(BackGroundSkipFlag == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetROIFlag(const int nROIFlag)
    {
        ROIFlag = nROIFlag;
    }
    const int GetROIFlag() const
    {
        return ROIFlag;
    }
    void SetROINumber(const int nROINumber)
    {
        ROINumber = nROINumber;
    }
    const int GetROINumber() const
    {
        return ROINumber;
    }
    void SetItem(const TRoiParamItem& tItem)
    {
        Item = tItem;
    }
    const TRoiParamItem& GetItem() const
    {
        return Item;
    }
    TRoiParamItem& GetItem()
    {
        return Item;
    }
public:
    void SetBackGroundQP(const int nBackGroundQP)
    {
        BackGroundQP = nBackGroundQP;
    }
    const int GetBackGroundQP() const
    {
        return BackGroundQP;
    }
    void SetBackGroundSkipFlag(const int nBackGroundSkipFlag)
    {
        BackGroundSkipFlag = nBackGroundSkipFlag;
    }
    const int GetBackGroundSkipFlag() const
    {
        return BackGroundSkipFlag;
    }
};

struct TConfigROIParamComplexType : public TGBStruct
{
public:
	TConfigROIParamComplexType()
	{
		sequence.Clear();
	}
private:
    TConfigROIParamComplexTypeSequence sequence;
public:
	virtual bool empty() const
	{
		return sequence.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void Setsequence(const TConfigROIParamComplexTypeSequence& tSequence)
    {
        sequence = tSequence;
    }
    const TConfigROIParamComplexTypeSequence& Getsequence() const
    {
        return sequence;
    }
    TConfigROIParamComplexTypeSequence& Getsequence()
    {
        return sequence;
    }
};

struct TConfigROIParam : public TGBStruct
{
public:
	TConfigROIParam()
	{
		complexType.Clear();
	}
private:
    TConfigROIParamComplexType complexType;
public:
	virtual bool empty() const
	{
		return complexType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetcomplexType(const TConfigROIParamComplexType& tComplexType)
    {
        complexType = tComplexType;
    }
    const TConfigROIParamComplexType& GetcomplexType() const
    {
        return complexType;
    }
    TConfigROIParamComplexType& GetcomplexType()
    {
        return complexType;
    }
};

struct TConfigSVCParamSimpleType : public TGBStruct
{
public:
	TConfigSVCParamSimpleType()
	{
		SVCFlag = 0;
		SVCSTMMode = 0;
		SVCSpaceDomainMode = 0;
		SVCTimeDomainMode = 0;
	}
private:
    int SVCFlag;
    int SVCSTMMode;
    int SVCSpaceDomainMode;
    int SVCTimeDomainMode;
public:
	virtual bool empty() const
	{
		return (SVCFlag == 0)&&(SVCSTMMode == 0)&&(SVCSpaceDomainMode == 0)&&(SVCSpaceDomainMode == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSVCFlag(const int nSVCFlag)
    {
        SVCFlag = nSVCFlag;
    }
    const int GetSVCFlag() const
    {
        return SVCFlag;
    }
    void SetSVCSTMMode(const int nSVCSTMMode)
    {
        SVCSTMMode = nSVCSTMMode;
    }
    const int GetSVCSTMMode() const
    {
        return SVCSTMMode;
    }
    void SetSVCSpaceDomainMode(const int nSVCSpaceDomainMode)
    {
        SVCSpaceDomainMode = nSVCSpaceDomainMode;
    }
    const int GetSVCSpaceDomainMode() const
    {
        return SVCSpaceDomainMode;
    }
    void SetSVCTimeDomainMode(const int nSVCTimeDomainMode)
    {
        SVCTimeDomainMode = nSVCTimeDomainMode;
    }
    const int GetSVCTimeDomainMode() const
    {
        return SVCTimeDomainMode;
    }
};

struct TConfigSVCEncodeParam : public TGBStruct
{
public:
	TConfigSVCEncodeParam()
	{
		simpleType.Clear();
	}
private:
    TConfigSVCParamSimpleType simpleType;
public:
	virtual bool empty() const
	{
		return simpleType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetsimpleType(const TConfigSVCParamSimpleType& tSimpleType)
    {
        simpleType = tSimpleType;
    }
    const TConfigSVCParamSimpleType& GetsimpleType() const
    {
        return simpleType;
    }
    TConfigSVCParamSimpleType& GetsimpleType()
    {
        return simpleType;
    }
};

struct TConfigSurveillanceParamSimpleTypeSequence : public TGBStruct
{
public:
	TConfigSurveillanceParamSimpleTypeSequence()
	{
		TimeFlag = 0;
		EventFlag = 0;
		AlertFlag = 0;
	}
private:
    int TimeFlag;
    int EventFlag;
    int AlertFlag;
public:
	virtual bool empty() const
	{
		return (TimeFlag == 0)&&(EventFlag == 0)&&(AlertFlag == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetTimeFlag(const int nTimeFlag)
    {
        TimeFlag = nTimeFlag;
    }
    const int GetTimeFlag() const
    {
        return TimeFlag;
    }
    void SetEventFlag(const int nEventFlag)
    {
        EventFlag = nEventFlag;
    }
    const int GetEventFlag() const
    {
        return EventFlag;
    }
    void SetAlertFlag(const int nAlertFlag)
    {
        AlertFlag = nAlertFlag;
    }
    const int GetAlertFlag() const
    {
        return AlertFlag;
    }
};

struct TConfigSurveillanceParamSimpleType : public TGBStruct
{
public:
	TConfigSurveillanceParamSimpleType()
	{
		sequence.Clear();
	}
private:
    TConfigSurveillanceParamSimpleTypeSequence sequence;
public:
	virtual bool empty() const
	{
		return sequence.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void Setsequence(const TConfigSurveillanceParamSimpleTypeSequence& tSequence)
    {
        sequence = tSequence;
    }
    const TConfigSurveillanceParamSimpleTypeSequence& Getsequence() const
    {
        return sequence;
    }
    TConfigSurveillanceParamSimpleTypeSequence& Getsequence()
    {
        return sequence;
    }
};

struct TConfigSVCEncodeSurveillanceParam : public TGBStruct
{
public:
	TConfigSVCEncodeSurveillanceParam()
	{
		simpleType.Clear();
	}
private:
    TConfigSurveillanceParamSimpleType simpleType;
public:
	virtual bool empty() const
	{
		return simpleType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetsimpleType(const TConfigSurveillanceParamSimpleType& tSimpleType)
    {
        simpleType = tSimpleType;
    }
    const TConfigSurveillanceParamSimpleType& GetsimpleType() const
    {
        return simpleType;
    }
    TConfigSurveillanceParamSimpleType& GetsimpleType()
    {
        return simpleType;
    }
};

struct TConfigEncryptParamSimpleTypeSequence : public TGBStruct
{
public:
	TConfigEncryptParamSimpleTypeSequence()
	{
		EncryptionFlag = 0;
		AuthenticationFlag = 0;
	}
private:
    int EncryptionFlag;
    int AuthenticationFlag;
public:
	virtual bool empty() const
	{
		return (EncryptionFlag == 0)&&(AuthenticationFlag == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetEncryptionFlag(const int nEncryptionFlag)
    {
        EncryptionFlag = nEncryptionFlag;
    }
    const int GetEncryptionFlag() const
    {
        return EncryptionFlag;
    }
    void SetAuthenticationFlag(const int nAuthenticationFlag)
    {
        AuthenticationFlag = nAuthenticationFlag;
    }
    const int GetAuthenticationFlag() const
    {
        return AuthenticationFlag;
    }
};

struct TConfigEncryptParamSimpleType : public TGBStruct
{
public:
	TConfigEncryptParamSimpleType()
	{
		sequence.Clear();
	}
private:
    TConfigEncryptParamSimpleTypeSequence sequence;
public:
	virtual bool empty() const
	{
		return sequence.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void Setsequence(const TConfigEncryptParamSimpleTypeSequence& tSequence)
    {
        sequence = tSequence;
    }
    const TConfigEncryptParamSimpleTypeSequence& Getsequence() const
    {
        return sequence;
    }
    TConfigEncryptParamSimpleTypeSequence& Getsequence()
    {
        return sequence;
    }
};

struct TConfigEncryptParam : public TGBStruct
{
public:
	TConfigEncryptParam()
	{
		simpleType.Clear();
	}
private:
    TConfigEncryptParamSimpleType simpleType;
public:
	virtual bool empty() const
	{
		return simpleType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetsimpleType(const TConfigEncryptParamSimpleType& tSimpleType)
    {
        simpleType = tSimpleType;
    }
    const TConfigEncryptParamSimpleType& GetsimpleType() const
    {
        return simpleType;
    }
    TConfigEncryptParamSimpleType& GetsimpleType()
    {
        return simpleType;
    }
};

struct TConfigAudioParam : public TGBStruct
{
public:
	TConfigAudioParam()
	{
		AudioRecognitionFlag = 0;
	}
private:
    int AudioRecognitionFlag;
public:
	virtual bool empty() const
	{
		return AudioRecognitionFlag == 0;
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetAudioRecognitionFlag(const int nAudioRecognitionFlag)
    {
        AudioRecognitionFlag = nAudioRecognitionFlag;
    }
    const int GetAudioRecognitionFlag() const
    {
        return AudioRecognitionFlag;
    }
};

struct TConfigSVACEncodeComplexTypeSequence : public TGBStruct
{
public:
	TConfigSVACEncodeComplexTypeSequence()
	{
		ROIParam.Clear();
	}
private:
	TConfigROIParam ROIParam;
	TConfigSVCEncodeParam SVCParam;
	TConfigSVCEncodeSurveillanceParam SurveillanceParam;
	TConfigEncryptParam EncryptParam;
	TConfigAudioParam AudioParam;
public:
	virtual bool empty() const
	{
		return ROIParam.empty();
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
	void SetROIParam(const TConfigROIParam& tROIParam)
	{
		ROIParam = tROIParam;
	}
	const TConfigROIParam& GetROIParam() const
	{
		return ROIParam;
	}
	TConfigROIParam& GetROIParam()
	{
		return ROIParam;
	}
public:
	void SetSVCParam(const TConfigSVCEncodeParam& tSVCParam)
	{
		SVCParam = tSVCParam;
	}
	const TConfigSVCEncodeParam& GetSVCParam() const
	{
		return SVCParam;
	}
	TConfigSVCEncodeParam& GetSVCParam()
	{
		return SVCParam;
	}
	void SetSurveillanceParam(const TConfigSVCEncodeSurveillanceParam& tSurveillanceParam)
	{
		SurveillanceParam = tSurveillanceParam;
	}
	const TConfigSVCEncodeSurveillanceParam& GetSurveillanceParam() const
	{
		return SurveillanceParam;
	}
	TConfigSVCEncodeSurveillanceParam& GetSurveillanceParam()
	{
		return SurveillanceParam;
	}
	void SetEncryptParam(const TConfigEncryptParam& tEncryptParam)
	{
		EncryptParam = tEncryptParam;
	}
	const TConfigEncryptParam& GetEncryptParam() const
	{
		return EncryptParam;
	}
	TConfigEncryptParam& GetEncryptParam()
	{
		return EncryptParam;
	}
	void SetAudioParam(const TConfigAudioParam& tAudioParam)
	{
		AudioParam = tAudioParam;
	}
	const TConfigAudioParam& GetAudioParam() const
	{
		return AudioParam;
	}
	TConfigAudioParam& GetAudioParam()
	{
		return AudioParam;
	}
};

struct TConfigSVACEncodeComplexType : public TGBStruct
{
public:
	TConfigSVACEncodeComplexType()
	{
		sequence.Clear();
	}
private:
	TConfigSVACEncodeComplexTypeSequence sequence;
public:
	virtual bool empty() const
	{
		return sequence.empty();
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
	void Setsequence(const TConfigSVACEncodeComplexTypeSequence& tSequence)
	{
		sequence = tSequence;
	}
	const TConfigSVACEncodeComplexTypeSequence& Getsequence() const
	{
		return sequence;
	}
	TConfigSVACEncodeComplexTypeSequence& Getsequence()
	{
		return sequence;
	}
};

struct TConfigSVACEncode : public TGBStruct
{
public:
	TConfigSVACEncode()
	{
		complexType.Clear();
	}
private:
	TConfigSVACEncodeComplexType complexType;
public:
	virtual bool empty() const
	{
		return complexType.empty(); 
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
	void SetcomplexType(const TConfigSVACEncodeComplexType& tComplexType)
	{
		complexType = tComplexType;
	}
	const TConfigSVACEncodeComplexType& GetcomplexType() const
	{
		return complexType;
	}
	TConfigSVACEncodeComplexType& GetcomplexType()
	{
		return complexType;
	}
};

struct TConfigSVCDecodeParam : public TGBStruct
{
public:
	TConfigSVCDecodeParam()
	{
		SVCSTMMode = 0;
	}
private:
    int SVCSTMMode;
public:
	virtual bool empty() const
	{
		return SVCSTMMode == 0;
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSVCSTMMode(const int nSVCSTMMode)
    {
        SVCSTMMode = nSVCSTMMode;
    }
    const int GetSVCSTMMode() const
    {
        return SVCSTMMode;
    }
};

struct TConfigSVCDecodeSurveillanceParam : public TGBStruct
{
public:
	TConfigSVCDecodeSurveillanceParam()
	{
		TimeShowFlag = 0;
		EventShowFlag = 0;
		AlertShowFlag = 0;
	}
private:
    int TimeShowFlag;
    int EventShowFlag;
    int AlertShowFlag;
public:
	virtual bool empty() const
	{
		return (TimeShowFlag == 0)&&(EventShowFlag == 0)&&(AlertShowFlag == 0);
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetTimeShowFlag(const int nTimeShowFlag)
    {
        TimeShowFlag = nTimeShowFlag;
    }
    const int GetTimeShowFlag() const
    {
        return TimeShowFlag;
    }
    void SetEventShowFlag(const int nEventShowFlag)
    {
        EventShowFlag = nEventShowFlag;
    }
    const int GetEventShowFlag() const
    {
        return EventShowFlag;
    }
    void SetAlertShowFlag(const int nAlertShowFlag)
    {
        AlertShowFlag = nAlertShowFlag;
    }
    const int GetAlertShowFlag() const
    {
        return AlertShowFlag;
    }
};

struct TConfigSVACDecodeComplexTypeSequence : public TGBStruct
{
public: 
	TConfigSVACDecodeComplexTypeSequence()
	{
		SVCParam.Clear();
		SurveillanceParam.Clear();
	}
private:
    TConfigSVCDecodeParam SVCParam;
    TConfigSVCDecodeSurveillanceParam SurveillanceParam;
public:
	virtual bool empty() const
	{
		return SVCParam.empty()&&SurveillanceParam.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetSVCParam(const TConfigSVCDecodeParam& tSVCParam)
    {
        SVCParam = tSVCParam;
    }
    const TConfigSVCDecodeParam& GetSVCParam() const
    {
        return SVCParam;
    }
    TConfigSVCDecodeParam& GetSVCParam()
    {
        return SVCParam;
    }
    void SetSurveillanceParam(const TConfigSVCDecodeSurveillanceParam& tSurveillanceParam)
    {
        SurveillanceParam = tSurveillanceParam;
    }
    const TConfigSVCDecodeSurveillanceParam& GetSurveillanceParam() const
    {
        return SurveillanceParam;
    }
    TConfigSVCDecodeSurveillanceParam& GetSurveillanceParam()
    {
        return SurveillanceParam;
    }
};


struct TConfigSVACDecodeComplexType : public TGBStruct
{
public:
	TConfigSVACDecodeComplexType()
	{
		sequence.Clear();
	}
private:
    TConfigSVACDecodeComplexTypeSequence sequence;
public:
	virtual bool empty() const
	{
		return sequence.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void Setsequence(const TConfigSVACDecodeComplexTypeSequence& tSequence)
    {
        sequence = tSequence;
    }
    const TConfigSVACDecodeComplexTypeSequence& Getsequence() const
    {
        return sequence;
    }
    TConfigSVACDecodeComplexTypeSequence& Getsequence()
    {
        return sequence;
    }
};

struct TConfigSVACDecode : public TGBStruct
{
public:
	TConfigSVACDecode()
	{
		complexType.Clear();
	}
private:
    TConfigSVACDecodeComplexType complexType;
public:
	virtual bool empty() const
	{
		return complexType.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetcomplexType(const TConfigSVACDecodeComplexType& tComplexType)
    {
        complexType = tComplexType;
    }
    const TConfigSVACDecodeComplexType& GetcomplexType() const
    {
        return complexType;
    }
    TConfigSVACDecodeComplexType& GetcomplexType()
    {
        return complexType;
    }
};

struct TAlarmGurdInfoExt : public TGBStruct
{
public:
    TAlarmGurdInfoExt()
    {
        AlarmMethod.clear();
        AlarmType.clear();
        AlarmTypeDes.clear();
    }
private:
    string AlarmMethod;
    string AlarmType;
    string AlarmTypeDes;
public:
    virtual bool empty() const
    {
        return AlarmMethod.empty() && AlarmType.empty() && AlarmTypeDes.empty();
    }

public:
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
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
    void SetAlarmType(const string& strAlarmType)
    {
        AlarmType = strAlarmType;
    }
    const string& GetAlarmType() const
    {
        return AlarmType;
    }
    string& GetAlarmType()
    {
        return AlarmType;
    }
    void SetAlarmTypeDes(const string& strAlarmTypeDes)
    {
        AlarmTypeDes = strAlarmTypeDes;
    }
    const string& GetAlarmTypeDes() const
    {
        return AlarmTypeDes;
    }
    string& GetAlarmTypeDes()
    {
        return AlarmTypeDes;
    }
};

struct TAlarmTypeParam : public TGBStruct
{
public:
    TAlarmTypeParam()
    {
        EventType=0;
    }
private:
    int EventType;
public:
	virtual bool empty() const
	{
		return (EventType ==0);
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetEventType(int dwEventType)
    {
        EventType = dwEventType;
    }
    const int& GetEventType() const
    {
        return EventType;
    }
     int& GetEventType()
    {
        return EventType;
    }
};


struct TAlarmInfoExt :public TGBStruct
{
public:
	TAlarmInfoExt()
	{
		AlarmType.clear();
		AlarmStatus.clear();
        AlarmTypeParam.Clear();
	}
private:
	string AlarmType;			//告警类型（可选）（科达扩展字段） 
    TAlarmTypeParam AlarmTypeParam;      //报警扩展参数（可选）
    string AlarmTypeDes;  //告警类型附加参数
    string AlarmStatus;		//告警状态（可选）（科达扩展字段）
public:
	virtual bool empty() const
	{
		return AlarmType.empty() && AlarmStatus.empty();
	}
	virtual void Clear();
	virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode) const;
	virtual bool FromXml(const TiXmlNode& xmlNode);
public:
	void SetAlarmType(const string& strAlarmType)
	{
		AlarmType = strAlarmType;
	}
	const string& GetAlarmType() const
	{
		return AlarmType;
	}
	string& GetAlarmType()
	{
		return AlarmType;
	}
	void SetAlarmTypeParam(const TAlarmTypeParam& strAlarmTypeParam)
	{
		AlarmTypeParam = strAlarmTypeParam;
	}
	const TAlarmTypeParam& GetAlarmTypeParam() const
	{
		return AlarmTypeParam;
	}
	TAlarmTypeParam& GetAlarmTypeParam()
	{
		return AlarmTypeParam;
	}
	void SetAlarmStatus(const string& strAlarmStatus)
	{
		AlarmStatus = strAlarmStatus;
	}
	const string& GetAlarmStatus() const
	{
		return AlarmStatus;
	}
	string& GetAlarmStatus()
	{
		return AlarmStatus;
	}
public:
    void SetAlarmTypeDes(const string& strAlarmTypeDes)
    {
        AlarmTypeDes = strAlarmTypeDes;
    }
    const string& GetAlarmTypeDes() const
    {
        return AlarmTypeDes;
    }
    string& GetAlarmTypeDes()
    {
        return AlarmTypeDes;
    }
};

struct TDragZoom :public TGBStruct
{
private:
    int Length;         //播放窗口长度像素值（必选）
    int Width;          //播放窗口宽度像素值（必选）
    int MidPointX;      //拉框中心的横轴坐标像素值（必选）
    int MidPointY;      //拉框中心的纵轴坐标像素值（必选）
    int LengthX;        //拉框长度像素值（必选）
    int LengthY;        //拉框宽度像素值（必选）
public:
    virtual bool empty() const
    {
        return (Length == 0) && (Width == 0) && (MidPointX == 0) && (MidPointY == 0) && (LengthX == 0) && (LengthY == 0);
    }
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetLength(const int nLength)
    {
        Length = nLength;
    }
    const int GetLength() const
    {
        return Length;
    }
    void SetWidth(const int nWidth)
    {
        Width = nWidth;
    }
    const int GetWidth() const
    {
        return Width;
    }
    void SetMidPointX(const int nMidPointX)
    {
        MidPointX = nMidPointX;
    }
    const int GetMidPointX() const
    {
        return MidPointX;
    }
    void SetMidPointY(const int nMidPointY)
    {
        MidPointY = nMidPointY;
    }
    const int GetMidPointY() const
    {
        return MidPointY;
    }
    void SetLengthX(const int nLengthX)
    {
        LengthX = nLengthX;
    }
    const int GetLengthX() const
    {
        return LengthX;
    }
    void SetLengthY(const int nLengthY)
    {
        LengthY = nLengthY;
    }
    const int GetLengthY() const
    {
        return LengthY;
    }
};

struct TGbGpsItem:public TGBStruct
{
private:
    string DeviceID;
    string Time;
    double Longitude;
    double Latitude;
    double Speed;
    double Elevation;
    double Azimuth;
public:
	virtual bool empty() const
	{
		return DeviceID.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
    void SetDeviceID(const string& strDeviceID)
    {
        DeviceID = strDeviceID;
    }
    const string& GetDeviceID() const
    {
        return DeviceID;
    }
    string& GetDeviceID()
    {
        return DeviceID;
    }
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
    void SetElevation(const double fElevation)
    {
        Elevation = fElevation;
    }
    const double GetElevation() const
    {
        return Elevation;
    }
    void SetAzimuth(const double fAzimuth)
    {
        Azimuth = fAzimuth;
    }
    const double GetAzimuth() const
    {
        return Azimuth;
    }
};

struct TGbCertificate:public TGBStruct
{
    string Type;
    string SerialNumber;
public:
	virtual bool empty() const
	{
	     return Type.empty() && SerialNumber.empty();
	}
    virtual void Clear();
    virtual void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    virtual bool FromXml(const TiXmlNode& xmlNode);
public:
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
    void SetSerialNumber(const string& strSerialNumber)
    {
        SerialNumber = strSerialNumber;
    }
    const string& GetSerialNumber() const
    {
        return SerialNumber;
    }
    string& GetSerialNumber()
    {
        return SerialNumber;
    }
};
//End of file


