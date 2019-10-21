/*****************************************************************************
   模块名      : CMSSDK
   文件名      : cmssdk_proto.h
   相关文件    : 
   文件实现功能: 监控平台2.0接入协议
   作者        : fanxg
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/09/12  1.0         fanxg        创建
******************************************************************************/

#ifndef __CMS_SDK_STRUCT_H__
#define __CMS_SDK_STRUCT_H__

//使用自动协议生成
#define WAIT_AUTO_PROTO

#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdio>
#include <sstream>
using std::ostream;
using std::ostringstream;
#include <algorithm>

using namespace std;

//xml前置声明
class TiXmlDocument;
class TiXmlNode;



#define  STRING_NULL ("");

// /* 公共常量定义，业务无特殊要求请使用它们的常量值 */
// const int UUID_STRING_LEN = 32;
// const int USER_NAME_MAX_LEN = 256;
// const int USER_PASSWORD_MAX_LEN = 256;
// const int TMP_BUFFER_LEN = 256;
// 
// /* CU用户常量 */
// const int CUSER_NAME_MAX_LEN = USER_NAME_MAX_LEN;
// const int CUSER_PASSWORD_MAX_LEN = USER_PASSWORD_MAX_LEN;
// const int CUSER_TYPE_SUPERADMIN   = 0;   // 超级管理员
// const int CUSER_TYPE_COMMONADMIN  = 1;   // 管理员
// const int CUSER_TYPE_OPERATOR     = 2;   // 操作员 CU用户类型

//公共结构体定义
typedef string TCmsIpAddr;

typedef vector<string>  TCmsDevUriList;

//避免编译问题
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
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif  //#ifndef CMS_CTRANSPARENTDATA


struct TCmsRealProtocolInfo
{
public:
	TCmsRealProtocolInfo()
	{
		Clear();
	}

	~TCmsRealProtocolInfo()
	{
		Clear();
	}

	void Clear()
	{
		localIp = STRING_NULL;
		localPort = 0;
		proxyIp = STRING_NULL;
		proxyPort = 0;
	}

	const string &GetLocalIp() const
	{
		return localIp;
	}
	string &GetLocalIp()
	{
		return localIp;
	}
	void SetLocalIp(const string &strIp)
	{
		localIp = strIp;
	}

	int GetLocalPort() const
	{
		return localPort;
	}
	void SetLocalPort(int nPort)
	{
		localPort = nPort;
	}

	const string &GetProxyIp() const
	{
		return proxyIp;
	}
	string &GetProxyIp()
	{
		return proxyIp;
	}
	void SetProxyIp(const string &strIp)
	{
		proxyIp = strIp;
	}

	int GetProxyPort() const
	{
		return proxyPort;
	}
	void SetProxyPort(int nPort)
	{
		proxyPort = nPort;
	}

private:
	string localIp;
	int localPort;
	string proxyIp;
	int proxyPort;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TConnInfo
{
public:
	TConnInfo()
	{

	}

	void SetSession(const string &strSession)
	{
		session = strSession;
	}
	const string &GetSession() const
	{
		return session;
	}
	string &GetSession()
	{
		return session;
	}
private:
	string session;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TCmsBackendCapId;
#define CMS_VIDEO_CLOCK_RATE_NEGO              1             // 后端能力集---采样率协商

typedef int TCmsBackendCapValue;
#define CMS_CAP_VALUE_PLAT_ENABLE              1
#define CMS_CAP_VALUE_PLAT_DISABLE             0

typedef map<TCmsBackendCapId, TCmsBackendCapValue> TCmsBackendCapList;

struct TCmsBackendCapabilities
{
public:
	TCmsBackendCapabilities()
	{
		Clear();
	}

	~TCmsBackendCapabilities()
	{
		Clear();
	}

	void Clear()
	{
		backendCapList.clear();
	}

	const TCmsBackendCapList &GetBackendCapList() const
	{
		return backendCapList;
	}
	TCmsBackendCapList &GetBackendCapList()
	{
		return backendCapList;
	}
	void SetBackendCapList(const TCmsBackendCapList &tCapList)
	{
		backendCapList = tCapList;
	}

private:
	TCmsBackendCapList backendCapList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsNetAddr
{
private:
    TCmsIpAddr ip;                           
    int        port;                                  

public:
    TCmsNetAddr() 
    {
        Clear();
    }

    void Clear()
    {
        ip = STRING_NULL;
        port = 0;
    }
public:
    void SetIp(const TCmsIpAddr& strIp)
    {
        ip = strIp;
    }
    TCmsIpAddr& GetIp()
    {
        return ip;
    }
    const TCmsIpAddr& GetIp() const
    {
        return ip;
    }

    void SetPort(int nPort)
    {
        port = nPort;
    }
    int GetPort() const
    {
        return port;
    }



	const TCmsIpAddr& GetNetIp() const 
	{
		return ip;
	}

	TCmsIpAddr& GetNetIp()
	{
		return ip;
	}

	void SetNetIp(const TCmsIpAddr& tNetIp) 
	{
		ip = tNetIp;
	}

	unsigned long GetNetIpv4() const;
	void SetNetIpv4(unsigned long dwIp);

	int GetNetPort() const 
	{
		return port;
	}

	void SetNetPort(int dwNetPort) 
	{
		port = dwNetPort;
	}



public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsNetAddr& obj)
	{
		output << obj.ip << " :"<<obj.port;
		return output;
	}
	std::string ToString() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}
	
	bool operator == (const TCmsNetAddr& obj) const
	{
		if (this->ip == obj.ip && this->port == obj.port)
		{
			return true;
		}
		return false;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TCmsDevType;
#define CMS_PU_ENCODER        "ENCODER"   //编码器
#define CMS_PU_DECODER        "DECODER"   //解码器
#define CMS_PU_CODEC          "CODEC"     //编解码器
#define CMS_PU_UNKNOWN        "UNKNOWN"   //未知前端设备

//视频格式
typedef string CCmsVidFormat;  //视频格式类型
#define CMS_VID_FMT_SN4         "SN4"
#define CMS_VID_FMT_MPEG4       "MPEG4"
#define CMS_VID_FMT_H261        "H261"
#define CMS_VID_FMT_H263        "H263"
#define CMS_VID_FMT_H264        "H264"
#define CMS_VID_FMT_JPEG        "JPEG"
#define CMS_VID_FMT_MJPEG       "MJPEG"
#define CMS_VID_FMT_H265        "H265"
#define CMS_VID_FMT_SVAC        "SVAC"

//视频分辨率
typedef string CCmsVidRes;     //视频分辨率类型
#define CMS_VID_RES_AUTO        "Auto"
#define CMS_VID_RES_QCIF        "QCIF"
#define CMS_VID_RES_CIF         "CIF"
#define CMS_VID_RES_2CIF        "2CIF"
#define CMS_VID_RES_4CIF        "4CIF"
#define CMS_VID_RES_D1          "D1"
#define CMS_VID_RES_QQCIF       "QQCIF"
#define CMS_VID_RES_VGA         "VGA"
#define CMS_VID_RES_QVGA        "QVGA"
#define CMS_VID_RES_720P        "720P"
#define CMS_VID_RES_960P        "960P"
#define CMS_VID_RES_1080P       "1080P"
#define CMS_VID_RES_QXGA        "QXGA"
#define CMS_VID_RES_4K          "4K"

typedef set<CCmsVidRes> CCmsVidResList;

//音频格式
typedef string CCmsAudFormat;  //音频格式类型
#define CMS_AUD_FMT_PCMA        "PCMA"
#define CMS_AUD_FMT_PCMU        "PCMU"
#define CMS_AUD_FMT_GSM         "GSM"
#define CMS_AUD_FMT_G729        "G729"
#define CMS_AUD_FMT_ADPCM       "ADPCM"
#define CMS_AUD_FMT_G7221C      "G7221C"
#define CMS_AUDIO_FMT_G722      "G722"
#define CMS_AUDIO_FMT_AACLC     "AACLC"

typedef int TCmsPuCap;	//前端能力
#define CMS_PU_CAP_DEVICE					(0x00)
#define CMS_PU_CAP_VIDEODECODER             (0x01)
#define CMS_PU_CAP_VIDEOENCODER             (0x02)
#define CMS_PU_CAP_AUDIODECODER             (0x03)
#define CMS_PU_CAP_AUDIOENCODER             (0x04)
#define CMS_PU_CAP_SERIALPORT               (0x05)
#define CMS_PU_CAP_INPUTPIN                 (0x06)
#define CMS_PU_CAP_OUTPUTPIN                (0x07)
#define CMS_PU_CAP_PTZMOTOR                 (0x08)
#define CMS_PU_CAP_PTZCONTROL               (0x09)
#define CMS_PU_CAP_ARCHIVER                 (0x0A)
#define CMS_PU_CAP_VIDEOIN		            (0x0C)
#define CMS_PU_CAP_ETHERNET				    (0x0D)  //0x01~0x0C为基本能力集  Ox0D~0x2F是扩展能力集
#define CMS_PU_CAP_WIRELESS		            (0x0E)
#define CMS_PU_CAP_XIANTELE				    (0x0F)
#define CMS_PU_CAP_VIDIOADPTER	            (0x10)
#define CMS_PU_CAP_VIDIOCOMP	            (0x11)
#define CMS_PU_CAP_VIDEOSRCPOLL             (0x12)
#define CMS_PU_CAP_AUDIOMIX                 (0x13)
#define CMS_PU_CAP_SHADEAREANUM             (0x14)
#define CMS_PU_CAP_DEVPOWERTYPE             (0x15)
#define CMS_PU_CAP_MAXSTREAMSEND            (0x16)
#define CMS_PU_CAP_MAXSTREAMDIRSEND         (0x17)
#define CMS_PU_CAP_ENCRYPT		            (0x18)
#define CMS_PU_CAP_RECORDER				    (0x19)
#define CMS_PU_CAP_PLAYER			        (0x1A)
#define CMS_PU_CAP_FILEDOWNLOAD	            (0x1B)
#define CMS_PU_CAP_VGAOUT			        (0x1C)
#define CMS_PU_CAP_VIDEOOUT		            (0x1D) 
#define CMS_PU_CAP_OSDCOLOR				    (0x1E)
#define CMS_PU_CAP_PLATFORMTYPE			    (0x1F)
#define CMS_PU_CAP_MOTIONDETECTAREANUM		(0x20)
#define CMS_PU_CAP_SNAPSHOTCHANNUM			(0x21)
#define CMS_PU_CAP_SLOTINFONUM				(0x22)
#define CMS_PU_CAP_CBRVBR					(0x23)    //是否能够设置图像质量
#define CMS_PU_CAP_MAXENCBITRATE			(0x24)	  //最大有效码率
#define CMS_PU_CAP_MULTICHAN                (0x25)    //是否支持双流
#define CMS_PU_CAP_MATRIX                   (0x26)    //是否支持矩阵接入
#define CMS_PU_CAP_VIDEOENCSLAVECHANNELNUM  (0x2F)    //视频编码辅流通道数  20090509
#define CMS_PU_CAP_TRANSPORTDATA			(0x29)    //是否使用透明数据传输
#define CMS_PU_CAP_GPS         			    (0x35)    //GPS功能
#define CMS_PU_CAP_EXTINPUTPIN      		(0x36)    //扩展并口能力(只有NVR使用)add G.J. 20091020
#define CMS_PU_CAP_DECODEEXT                (0x3B)    //是否支持解码器第三方扩展  //add GJ 20100317
#define CMS_PU_CAP_SUPPORT_RESOLUTION_WH    (0x3D)	  //是否支持字符串形式的上报分辨率. 
#define CMS_PU_CAP_EXT_OSD					(0xF0)    //是否支持多行字幕
#define CMS_PU_CAP_SUPPORT_TOTAL_PICTURE    (0xF2)    //是否支持整体图元 
#define CMS_PU_CAP_MAX_FRAMERATE			(0xF4)    //支持的最大编码帧率
#define CMS_PU_CAP_DEVTYPE                  (0xF5)	  //设备类型能力集
#define CMS_PU_CAP_CTRLTYPE                 (0xF6)	  //控制台类型能力集
#define CMS_PU_CAP_MAX_ENC_CHANNEL			(0xF7)	  //最大编码通道数能力集
#define CMS_PU_CAP_CHANNEL_CAPS			    (0xF8)    //设备具有通道能力
#define CMS_PU_CAP_RECLIST_QRY				(0x50)	  //是否支持录像的日历查询.
#define CMS_PU_CAP_CHANNL_STATE_REPORT		(0x51)	  //是否支持通道状态上报
#define CMS_PU_CAP_HMDI_COUNT				(0x60)    //HMDI总数
#define CMS_PU_CAP_HMDI_1_DEC_CHAN_COUNT	(0x61)    //HMDI_1的解码通道数
#define CMS_PU_CAP_HMDI_2_DEC_CHAN_COUNT	(0x62)    //HMDI_2的解码通道数
#define CMS_PU_CAP_HMDI_3_DEC_CHAN_COUNT	(0x63)    //HMDI_3的解码通道数
#define CMS_PU_CAP_HMDI_4_DEC_CHAN_COUNT	(0x64)    //HMDI_4的解码通道数
#define CMS_PU_CAPmartTab					(0x97)	  //设备支持拼接(图像缩放)功能.
#define CMS_PU_CAP_SUPPORT_BD_TRACK_MODE    (0xD7)    //支持智能枪球联动
#define CMS_PU_CAP_G100_NOT_SUPPORT_VIDEONAME_SYNC (0x65)   //不支持视频源名称同步能力集

struct TCmsVidType
{
private:
    CCmsVidFormat    format;                                     //视频格式
    CCmsVidResList   resSet;                                     //该格式支持的分辨率

public:
    void Clear()
    {
        format = STRING_NULL;
        resSet.clear();
    }
public:
    void SetFormat(const CCmsVidFormat& strFormat)
    {
        format = strFormat;
    }
    CCmsVidFormat& GetFormat()
    {
        return format;
    }
    const CCmsVidFormat& GetFormat() const
    {
        return format;
    }

    void SetResSet(const CCmsVidResList& cResSet)
    {
        resSet = cResSet;
    }
    CCmsVidResList& GetResSet()
    {
        return resSet;
    }
    const CCmsVidResList& GetResSet() const
    {
        return resSet;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef map<CCmsVidFormat, TCmsVidType> CCmsVidTypeList;
typedef set<CCmsAudFormat> CCmsAudTypeList;

struct TCmsSupportMediaType
{
private:
    CCmsVidTypeList video;                              //所有支持的视频格式
    CCmsAudTypeList audio;                              //所有支持的音频格式

public:
    void Clear()
    {
        video.clear();
        audio.clear();
    }
public:
    void SetVideo(const CCmsVidTypeList& cVideo)
    {
        video = cVideo;
    }
    CCmsVidTypeList& GetVideo()
    {
        return video;
    }
    const CCmsVidTypeList& GetVideo() const
    {
        return video;
    }

    void SetAudio(const CCmsAudTypeList& cAudio)
    {
        audio = cAudio;
    }
    CCmsAudTypeList& GetAudio()
    {
        return audio;
    }
    const CCmsAudTypeList& GetAudio() const
    {
        return audio;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#define CMS_VID_QUALITY_PRIOR             0x01 //质量优先
#define CMS_VID_SPEED_PRIOR               0x02 //速度优先

//视频编码参数
struct TCmsVideoEncodeParam
{
private:
    CCmsVidFormat videoType;                               //数据压缩格式
    CCmsVidRes videoRes;                                   //图象分辨率
    int frameRate;                                         //帧率 1-25;
    int quality;                                           //图象质量
    int bitRate;                                           //编码比特率 64K-4M;
    int	maxQuantization;	                               //最大量化因子 1-31 //高级编码参数Ex
    int	minQuantization;	                               //最小量化因子 1-31 //高级编码参数Ex
    int interval;                                          //关键帧间隔 25-2500;
    int brightness;                                        //亮度 -128～127
    int contrast;                                          //对比度 -128～127
    int saturation;                                        //饱和度 -128～127
    int	dnyValue;		                            	   //动态载荷值 //高级编码参数Ex
    int sharpness;                                         //锐度 0: not support; 1-4: available
    int	profileId;			                               //图像编码协议等级 //1.0中有，但是没有用到，保留
	int clockRate;                                         //视频采样率

public: 
    TCmsVideoEncodeParam() 
    {
        Clear();
    }

    void Clear()
    {
        videoType = STRING_NULL;
        videoRes = STRING_NULL;
        frameRate = 0;    
        quality = 0;      
        bitRate = 0;      
        interval = 0;     
        brightness = 0;   
        contrast = 0;     
        saturation = 0;   
        sharpness = 0;    
        maxQuantization = 0;	   //最大量化因子 1-31 //高级编码参数Ex
        minQuantization = 0;	   //最小量化因子 1-31 //高级编码参数Ex
        dnyValue = 0;
        profileId = 0;
    }
public:
    void SetVideoType(const CCmsVidFormat& strVideoType)
    {
        videoType = strVideoType;
    }
    CCmsVidFormat& GetVideoType()
    {
        return videoType;
    }
    const CCmsVidFormat& GetVideoType() const
    {
        return videoType;
    }

    void SetVideoRes(const CCmsVidRes& strVideoRes)
    {
        videoRes = strVideoRes;
    }
    CCmsVidRes& GetVideoRes()
    {
        return videoRes;
    }
    const CCmsVidRes& GetVideoRes() const
    {
        return videoRes;
    }

    void SetFrameRate(int nFrameRate)
    {
        frameRate = nFrameRate;
    }
    int GetFrameRate() const
    {
        return frameRate;
    }

    void SetQuality(int nQuality)
    {
        quality = nQuality;
    }
    int GetQuality() const
    {
        return quality;
    }

    void SetBitRate(int nBitRate)
    {
        bitRate = nBitRate;
    }
    int GetBitRate() const
    {
        return bitRate;
    }

    void SetMaxQuantization(int nMaxQuantization)
    {
        maxQuantization = nMaxQuantization;
    }
    int GetMaxQuantization() const
    {
        return maxQuantization;
    }

    void SetMinQuantization(int nMinQuantization)
    {
        minQuantization = nMinQuantization;
    }
    int GetMinQuantization() const
    {
        return minQuantization;
    }

    void SetInterval(int nInterval)
    {
        interval = nInterval;
    }
    int GetInterval() const
    {
        return interval;
    }

    void SetBrightness(int nBrightness)
    {
        brightness = nBrightness;
    }
    int GetBrightness() const
    {
        return brightness;
    }

    void SetContrast(int nContrast)
    {
        contrast = nContrast;
    }
    int GetContrast() const
    {
        return contrast;
    }

    void SetSaturation(int nSaturation)
    {
        saturation = nSaturation;
    }
    int GetSaturation() const
    {
        return saturation;
    }

    void SetDnyValue(int nDnyValue)
    {
        dnyValue = nDnyValue;
    }
    int GetDnyValue() const
    {
        return dnyValue;
    }

    void SetSharpness(int nSharpness)
    {
        sharpness = nSharpness;
    }
    int GetSharpness() const
    {
        return sharpness;
    }

    void SetProfileId(int nProfileId)
    {
        profileId = nProfileId;
    }
    int GetProfileId() const
    {
        return profileId;
    }

	void SetClockRate(int nClockRate)
	{
		clockRate = nClockRate;
	}
	int GetClockRate() const
	{
		return clockRate;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//音频编码参数
struct TCmsAudioEncodeParam
{
private:
	CCmsAudFormat audioType;                //音频格式
	int volume;               			    //音量 0-25;
	bool aecState;               		    //回声抵消状态
	int smpRate;                            //采样率 
	int smpBits;                            //采样位数 
	int channel;                            //声道 
	int samplePerF;                         //每帧样本数
	int clockRate;                          //时间戳采样率
	int bitRate;                            //码率

public: 
	TCmsAudioEncodeParam() 
	{
		Clear();
	}

	~TCmsAudioEncodeParam()
	{
		Clear();
	}

	void Clear()
	{
		audioType = STRING_NULL;
		volume = 0;
		aecState = false;
		smpRate = 0;
		smpBits = 0;
		channel = 0;
		samplePerF = 0;
		clockRate  = 0;
		bitRate    = 0;
	}
public:
	void SetAudioType(const CCmsAudFormat& strAudioType)
	{
		audioType = strAudioType;
	}
	CCmsAudFormat& GetAudioType()
	{
		return audioType;
	}
	const CCmsAudFormat& GetAudioType() const
	{
		return audioType;
	}

	void SetVolume(int nVolume)
	{
		volume = nVolume;
	}
	int GetVolume() const
	{
		return volume;
	}

	void SetAecState(bool bAecState)
	{
		aecState = bAecState;
	}
	bool GetAecState() const
	{
		return aecState;
	}

	void SetSmpRate(int nSmpRate)
	{
		smpRate = nSmpRate;
	}
	int GetSmpRate() const
	{
		return smpRate;
	}

	void SetSmpBits(int nSmpBits)
	{
		smpBits = nSmpBits;
	}
	int GetSmpBits() const
	{
		return smpBits;
	}

	void SetChannel(int nChannel)
	{
		channel = nChannel;
	}
	int GetChannel() const
	{
		return channel;
	}

	void SetSamplePerF(int nSamplePerF)
	{
		samplePerF = nSamplePerF;
	}
	int GetSamplePerF() const
	{
		return samplePerF;
	}

	void SetClockRate(int nClockRate)
	{
		clockRate = nClockRate;
	}
	int GetClockRate() const
	{
		return clockRate;
	}

	void SetBitRate(int nBitRate)
	{
		bitRate = nBitRate;
	}
	int GetBitRate() const
	{
		return bitRate;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//编码通道属性列表，map的key为编码通道号
typedef set<int> CCmsVidSrcSet;  //视频源集合
typedef map<int,CCmsVidSrcSet> CCmsVidSrcSetList;
typedef map<int,CCmsVidFormat> CCmsVidFormatList;
typedef map<int,TCmsSupportMediaType> CCmsSupportMediaList;
typedef map<int,TCmsVideoEncodeParam> CCmsVideoEncodeParamList;
typedef map<int,TCmsAudioEncodeParam> CCmsAudioEncodeParamList;

//视频源通道属性列表，map的key为视频源通道
typedef set<int> CCmsPuChnSet;   //编码通道集合
typedef map<int,CCmsPuChnSet> CCmsPuChnSetList;
typedef map<int,string> CCmsVidSrcNameList;
typedef map<int,int> CCmsCamTypeList;
typedef map<int,int> CCmsCamIdList;

struct TCmsChannel
{
private:
	string devUri;
	int    chnNO;    //通道有可能是编码通道，有可能是视频源，还有可能是放像通道, 要根据协议具体应用场景判断

public:
	TCmsChannel()
	{
		Clear();
	}

	TCmsChannel(const string& strDevUri, int nChnNO)
	{
		devUri = strDevUri;
		chnNO = nChnNO;
	}

	void Clear()
	{
		devUri = STRING_NULL;
		chnNO = 0;
	}

	const string& GetDevUri() const 
	{
		return devUri;
	}

	string& GetDevUri() 
	{
		return devUri;
	}

	void SetDevUri(const string& strDevUri) 
	{
		devUri = strDevUri;
	}

	int GetChnNO() const
	{
		return chnNO;
	}


	void SetChnNO(int dwChnNO)
	{
		chnNO = dwChnNO;
	}

	bool operator == (const TCmsChannel& tValue) const
	{
		if (&tValue == this
			||(tValue.devUri == devUri && tValue.chnNO == chnNO))
		{
			return true;
		}
		return false;
	}

	bool operator != (const TCmsChannel& tValue) const
	{
		return !(operator ==(tValue));
	}

	bool IsNull() const
	{
		return devUri.empty();
	}

	bool IsValid() const
	{
		if (devUri.empty())
		{
			return false;
		}

		string::size_type strPos = devUri.find('@');
		if (strPos == string::npos)
		{
			return false;
		}

		string strUser;
		string strDomain;
		strUser.assign(devUri, 0, strPos);
		strDomain.assign(devUri, strPos+1, devUri.size()-strPos-1);

		if (strUser.empty() || strDomain.empty())
		{
			return false;
		}

		if (chnNO < 0)
		{
			return false;
		}

		return true;
	}

public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsChannel& obj)
	{
		if(obj.devUri.length() != 0)
		{
			output<<obj.devUri<<"_"<<obj.chnNO;
		}
		return output;
	}
	std::string ToString() const
	{
		std::ostringstream oss;
		oss << *this;
		return oss.str();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TCmsChannel> TCmsChannelList;

struct TCmsPuChnRecDays
{
private:
	TCmsChannel    puChn;
	int            dayBitMask;

public:
	TCmsPuChnRecDays()
	{
		Clear();
	}

	void Clear()
	{
		puChn.Clear();
		dayBitMask = 0;
	}

public:

	//puChn
	const TCmsChannel& GetPuChn() const
	{
		return puChn;
	}
	TCmsChannel& GetPuChn()
	{
		return puChn;
	}
	void SetPuChn(const TCmsChannel& var)
	{
		puChn = var;
	}

	//dayBitMask
	int GetDayBitMask() const
	{
		return dayBitMask;
	}
	void SetDayBitMask(int var)
	{
		dayBitMask = var;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsPuChnRecDays> TCmsPuChnRecDaysList;

struct TCmsTransDataCfgParam
{
public:
	TCmsTransDataCfgParam()
	{
		Clear();
	}

	~TCmsTransDataCfgParam()
	{
		Clear();
	}

	void Clear()
	{
		type = 0;
		param = STRING_NULL;
	}

private:
	int        type;                  //透明数据类型
	string     param;                 //配置参数
public:
	void SetType(int nType)
	{
		type = nType;
	}
	int GetType() const
	{
		return type;
	}

	void SetParam(const string& strParam)
	{
		param = strParam;
	}
	string& GetParam()
	{
		return param;
	}
	const string& GetParam() const
	{
		return param;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//摄像头的类型
enum TCamType{
	CAM_TYPE_BEGIN = 0,
	CAM_TYPE_SONY,	        //	SONY
	CAM_TYPE_AD,			//	AD
	CAM_TYPE_PELCO,			//	PELCO-D
	CAM_TYPE_PIH,			//	利凌
	CAM_TYPE_PELCO_P,		//	PELCO-P
	CAM_TYPE_PARCO,			//	PARCO柏高
	CAM_TYPE_AB,			//	AB
	CAM_TYPE_YUNTAI,		//	独立云台
	CAM_TYPE_Samsung,		//	三星
	CAM_TYPE_YAAN,			//	亚安
	CAM_TYPE_AD168,			//	AD168类型
	CAM_TYPE_KALATEL,		//	卡拉特
	CAM_TYPE_JN,			//	JN伟华
	CAM_TYPE_WV_CS650,		//	松下,是850的老协议
	CAM_TYPE_WV_CS850,		//	松下,是850的新协议
	CAM_TYPE_ALEC,		    //	艾立克
	CAM_TYPE_AB_P,          //	AB-P
	CAM_TYPE_AB_D,          //	AB-D
	CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
	CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
	CAM_TYPE_TIANDY,        //  天地伟业
	CAM_TYPE_G100,          //  G100
	CAM_TYPE_368ZT,         //  368转台插件
	CAM_TYPE_GaoDeTI,       //  GaoDeTI红外插件
	CAM_TYPE_DaliTI,        //  DaliTI红外插件
	CAM_TYPE_2130_PELCO_P,  //  非标准PELCO_P协议
	CAM_TYPE_K_PELCO_D,
	CAM_TYPE_JVCTKC676,
	CAM_TYPE_INFINOVA_PELCO_D,
	CAM_TYPE_PEARMAIN_PELCO_D,
	CAM_TYPE_PEARMAIN_PELCO_P,
	CAM_TYPE_YAAN_PELCO_D,
	CAM_TYPE_CAMERA_NUM,

	CAM_TYPE_CAMERA_NOCTLR = 63,    //不可控设备
	CAM_TYPE_MATRIX=128,	//	矩阵协议
	MATRIX_TYPE_AB=128,		//  AB 矩阵
	CAM_TYPE_ROBOT,			//  ROBOT 画面合成矩阵
	MATRIX_TYPE_SUNNHO,		//  SUNNHO	矩阵
	MATRIX_TYPE_PEARMAIN,   //  红苹果  矩阵
	MATRIX_TYPE_AD,         //  AD 矩阵
	MATRIX_TYPE_INFORNOVA,
	MATRIX_TYPE_PECLO,      
	MATRIX_TYPE_HONEWELL,  //霍尼韦尔
	MATRIX_TYPE_TIANDI,
	MATRIX_TYPE_HUATONG,//华通矩阵
	MATRIX_TYPE_TONGCHUANG,//同创矩阵
	MATRIX_TYPE_BOCOM,     //博康矩阵
	MATRIX_TYPE_EXTRON,    //爱思创
	MATRIX_TYPE_ST_MS650,  //金三立
	MATRIX_TYPE_LIKEN,     //利肯
	MATRIX_TYPE_VINET,     //微耐特
	MATRIX_TYPE_WEPUS,     //威普士
	CAM_TYPE_MATRIX_NUM
};

//摄像头的控制命令
enum TCamCommand{
	CAM_COMMAND_MOVEUP=1,			//(u8)1		//向上移动
	CAM_COMMAND_MOVEDOWN,	    	//(u8)2		//向下移动
	CAM_COMMAND_MOVELEFT,			//(u8)3		//向左移动
	CAM_COMMAND_MOVERIGHT,			//(u8)4		//向右移动
	CAM_COMMAND_MOVEHOME,			//(u8)5		//回归
	CAM_COMMAND_MOVESTOP,			//(u8)6		//停止移动
	CAM_COMMAND_ZOOMTELE,		    //(u8)7		//拉近摄像头
	CAM_COMMAND_ZOOMWIDE,			//(u8)8		//拉远摄像头
	CAM_COMMAND_ZOOMSTOP,			//(u8)9		//视野调节停止
	CAM_COMMAND_FOCUSFAR,		    //(u8)10	//将焦距调远
	CAM_COMMAND_FOCUSNEAR,			//(u8)11	//将焦距调近
	CAM_COMMAND_FOCUSAUTO,			//(u8)12	//自动调焦
	CAM_COMMAND_FOCUSSTOP,			//(u8)13	//调焦停止
	CAM_COMMAND_PRESETSET,	    	//(u8)14	//摄象头预存
	CAM_COMMAND_PRESETCALL,			//(u8)15	//调摄象头预存
	CAM_COMMAND_CAMERASET,			//(u8)16	//初始化摄像头
	CAM_COMMAND_BRIGHTUP,			//(u8)17	//画面调亮
	CAM_COMMAND_BRIGHTDOWN,			//(u8)18	//画面调暗
	CAM_COMMAND_BRIGHTSTOP,			//(u8)19	//停止调亮
	CAM_COMMAND_NEW,				//(u8)20	//新命令
	CAM_COMMAND_APPEND,				//(u8)21	//附加命令
	CAM_COMMAND_GOTOPOINT,			//(u8)22	//中心定位
	CAM_COMMAND_ZOOMPART,			//(u8)23	//局部放大

	CAM_COMMAND_PANSPEED,			//(u8)24	//艾立克水平移动速度设置命令，（内部使用）
	CAM_COMMAND_TILTSPEED,			//(u8)25	//艾立克垂直移动速度设置命令，（内部使用）
	CAM_COMMAND_SETBRIGHT,			//(u8)26	//sony亮度调节启动命令（内部使用）
	CAM_COMMAND_MATRIX_TRANSIO,		//(u8)27
	CAM_COMMAND_MATRIX_SWTICH=27,	//(u8)27	//切换视频
	CAM_COMMAND_MATRIX_VIDMIX4,		//(u8)28	//画面合成2×2
	CAM_COMMAND_MATRIX_VIDMIX9,		//(u8)29	//画面合成3×3
	CAM_COMMAND_MATRIX_VIDMIX16,	//(u8)30	//画面合成4×4
	CAM_COMMAND_MATRIX_VIDMIXPP,	//(u8)31	//画面合成画中画

	CAM_COMMAND_ZOOMWHOLE,			//(u8)32    //局部缩小 

	////菜单//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_MENU_UP,			//(u8)33	//菜单上
	CAM_COMMAND_MENU_DOWN,			//(u8)34	//菜单下
	CAM_COMMAND_MENU_LEFT,			//(u8)35	//菜单左
	CAM_COMMAND_MENU_RIGHT,			//(u8)36	//菜单右

	CAM_COMMAND_MENU_SHOW,			//(u8)37	//菜单显示
	CAM_COMMAND_MENU_HIDE,			//(u8)38	//菜单隐藏

	CAM_COMMAND_MENU_ENTER,			//(u8)39	//菜单确定
	CAM_COMMAND_MENU_EXIT,		    //(u8)40	//菜单取消

	CAM_COMMAND_PRESETDEL,			//(u8)41	//删除摄象头预存
	////矩阵切换//////////////////////////////////////////////////////////////////////
	CAM_COMMAND_MATRIX_SWTICHASC,	//(u8)42	//正向切换
	CAM_COMMAND_MATRIX_SWTICHDESC,	//(u8)43	//反向切换
	CAM_COMMAND_MATRIX_IMAGEHOLD,	//(U8)44	//图像保持

	CAM_COMMAND_MOVELEFTUP,			//(u8)45		//向左上移动
	CAM_COMMAND_MOVELEFTDOWN,	    //(u8)46		//向左下移动
	CAM_COMMAND_MOVERIGHTUP,		//(u8)47		//向右上移动
	CAM_COMMAND_MOVERIGHTDOWN,		//(u8)48		//向右下移动

	////巡航//////////////////////////////////////////////////////////////////////	
	CAM_COMMAND_STARTAUTOSCAN=128,  //(u8)128	//开始自动巡航
	CAM_COMMAND_STOPAUTOSCAN,       //(u8)129	//停止自动巡航	
	CAM_COMMAND_STARTPRESETSCAN,	//(u8)130	//开始巡检
	CAM_COMMAND_STOPPRESETSCAN,     //(u8)131	//停止巡检
	CAM_COMMAND_SCANSPEED_SET,		//(u8)132	//水平扫描速度（影响自动扫描速度）bySpeed(1-40s)
};

//#define MAX_TYPE_DISCRIBE		(u8)10    //最大协议长度
#define MAX_PAN_SPEED			(u8)15    //pan speed 0-MAX_PAN_SPEED
#define MAX_TILT_SPEED			(u8)15    //tilt speed 0-MAX_TILT_SPEED
#define MAX_ZOOM_SPEED			(u8)15    //zoom speed 0-MAX_ZOOM_SPEED
#define MAX_PREPOS_RANGE		(u8)16    //预置位号范围
#define MAX_ALEC_WIDE           (u8)254   //艾立克局部放大矩形最大宽度
#define MAX_ALEC_HIGH           (u8)254   //艾立克局部放大矩形最大高度
#define MIN_ALEC_WIDE           (u8)11    //艾立克局部放大矩形最小宽度
#define MIN_ALEC_HIGH           (u8)11    //艾立克局部放大矩形最小高度
/*命令参数表*/
/*
命令:CAM_COMMAND_MOVEUP,CAM_COMMAND_MOVEDOWN,CAM_COMMAND_MOVELEFT,
CAM_COMMAND_MOVERIGHT,CAM_COMMAND_MOVEHOME,CAM_COMMAND_MOVESTOP
参数:(m_byParam1,m_byParam2都有意义)
m_byParam1/范围:0-MAX_PAN_SPEED
m_byParam2/范围:0-MAX_TILT_SPEED

命令:CAM_COMMAND_ZOOMTELE,CAM_COMMAND_ZOOMWIDE
参数:(m_byParam1有意义)
m_byParam1/范围:0-MAX_ZOOM_SPEED

命令:CAM_COMMAND_ZOOMSTOP,CAM_COMMAND_FOCUSFAR,CAM_COMMAND_FOCUSNEAR,
CAM_COMMAND_FOCUSAUTO,CAM_COMMAND_FOCUSSTOP
参数:(m_byParam1,m_byParam2都无意义)

命令:CAM_COMMAND_PRESETSET,CAM_COMMAND_PRESETCALL
参数:(m_byParam1有意义)
m_byParam1/范围:1-16

命令:CAM_COMMAND_CAMERASET
参数:(m_byParam1,m_byParam2都有意义)
*/
struct TCmsCamCtrl
{
private:
	int commandId; //命令类型;

	//各种命令的参数不同,详见命令参数表(见newcamera.h)
	//     int param1; //参数1，各种命令的参数含义不同,详见命令参数表
	//     int param2; //参数2
	//     int param3; //参数3，参数3，4为艾立克局部放大功能使用
	//     int param4; //参数4
	vector<int> params;

	int priLevel; //控制云镜级别
	int holdTime; //控制占用时间(单位:秒);
	int matrixIn; //对于前置矩阵，该参数为矩阵接入前端的输入端口号;

public:
	TCmsCamCtrl(void) 
	{
		Clear();
	}

	void Clear()
	{
		commandId = 0;
		params.clear();
		priLevel = 0;
		holdTime = 0;
		matrixIn = 0;
	}

	//获取命令类型
	int GetCommandId() const 
	{
		return commandId;
	}
	int GetCommandId()
	{
		return commandId;
	}
	//设置命令类型
	void SetCommandId(const int dwCommandId) 
	{
		commandId = dwCommandId;
	}

	vector<int> &GetParams()
	{
		return params;
	}
	const vector<int> &GetParams() const
	{
		return params;
	}
	void SetParams(const vector<int> &vctParams)
	{
		params = vctParams;
	}

	//获取控制云镜级别
	int GetPriLevel() const 
	{
		return priLevel;
	}
	int GetPriLevel()
	{
		return priLevel;
	}
	//设置控制云镜级别
	void SetPriLevel(const int dwPriLevel) 
	{
		priLevel = dwPriLevel;
	}

	//获取控制占用时间(单位:秒)
	int GetHoldTime() const 
	{
		return holdTime;
	}
	//设置控制占用时间(单位:秒)
	void SetHoldTime(int dwHoldTime) 
	{
		holdTime = dwHoldTime;
	}

	//获取对于前置矩阵，该参数为矩阵接入前端的输入端口号
	int GetMatrixIn() const 
	{
		return matrixIn;
	}
	//设置对于前置矩阵，该参数为矩阵接入前端的输入端口号
	void SetMatrixIn(int dwMatrixIn) 
	{
		matrixIn = dwMatrixIn;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef string CCmsInputPinEnable;
typedef map<TCmsPuCap,int> CCmsPuExtCapSet;
typedef map<int,int> CCmsVidDynValue;
typedef map<int,string> CCmsVidSrcName;
typedef map<int,int> CCmsBitRateList;

struct TCmsPuBaseCapSet
{
private:
    int videoEncNum;                                           //视频编码通道数量;
    int videoDecNum;                                           //视频解码通道数量;
    int audioEncNum;                                           //音频编码通道数量;
    int audioDecNum;                                           //音频解码通道数量;
    int serialPortNum;                                         //串口数量;
    int inputPinNum;                                           //并口输入数量;
    int outputPinNum;                                          //并口输出数量;
    int videoInPortNum;                                        //视频输入端口数量;
//    int slaveStreamNum;                                        //辅流数目;

public:
    TCmsPuBaseCapSet()
    {
        Clear();
    }

    ~TCmsPuBaseCapSet()
    {
        Clear();
    }

    void Clear()
    {
        videoEncNum = 0;
        videoDecNum = 0;
        audioEncNum = 0;
        audioDecNum = 0;
        serialPortNum = 0;
        inputPinNum = 0;
        outputPinNum = 0;            
        videoInPortNum = 0;
//        slaveStreamNum = 0;
    }
public:
    void SetVideoEncNum(int nVideoEncNum)
    {
        videoEncNum = nVideoEncNum;
    }
    int GetVideoEncNum() const
    {
        return videoEncNum;
    }

    void SetVideoDecNum(int nVideoDecNum)
    {
        videoDecNum = nVideoDecNum;
    }
    int GetVideoDecNum() const
    {
        return videoDecNum;
    }

    void SetAudioEncNum(int nAudioEncNum)
    {
        audioEncNum = nAudioEncNum;
    }
    int GetAudioEncNum() const
    {
        return audioEncNum;
    }

    void SetAudioDecNum(int nAudioDecNum)
    {
        audioDecNum = nAudioDecNum;
    }
    int GetAudioDecNum() const
    {
        return audioDecNum;
    }


	int GetVidEncNum() const 
	{
		return videoEncNum;
	}
	void SetVidEncNum(int dwVideoEncNum) 
	{
		videoEncNum = dwVideoEncNum;
	}

	int GetVidDecNum() const 
	{
		return videoDecNum;
	}
	void SetVidDecNum(int dwVideoDecNum) 
	{
		videoDecNum = dwVideoDecNum;
	}

	int GetAudEncNum() const 
	{
		return audioEncNum;
	}
	void SetAudEncNum(int dwAudioEncNum) 
	{
		audioEncNum = dwAudioEncNum;
	}

	int GetAudDecNum() const 
	{
		return audioDecNum;
	}
	void SetAudDecNum(int dwAudioDecNum) 
	{
		audioDecNum = dwAudioDecNum;
	}

    void SetSerialPortNum(int nSerialPortNum)
    {
        serialPortNum = nSerialPortNum;
    }
    int GetSerialPortNum() const
    {
        return serialPortNum;
    }

    void SetInputPinNum(int nInputPinNum)
    {
        inputPinNum = nInputPinNum;
    }
    int GetInputPinNum() const
    {
        return inputPinNum;
    }

    void SetOutputPinNum(int nOutputPinNum)
    {
        outputPinNum = nOutputPinNum;
    }
    int GetOutputPinNum() const
    {
        return outputPinNum;
    }

    void SetVideoInPortNum(int nVideoInPortNum)
    {
        videoInPortNum = nVideoInPortNum;
    }
    int GetVideoInPortNum() const
    {
        return videoInPortNum;
    }

	//   void SetSlaveStreamNum(int nSlaveStreamNum)
	//{
	//slaveStreamNum = nSlaveStreamNum;
	//}
	//int GetSlaveStreamNum() const
	//{
	//return slaveStreamNum;
	//}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


#define PU_ENCODER        "ENCODER"   //编码器
#define PU_DECODER        "DECODER"   //解码器
#define PU_CODEC          "CODEC"     //编解码器
#define PU_UNKNOWN        "UNKNOWN"   //未知前端设备


//设备属性配置
struct TCmsPuBaseConfig 
{
private:
	string            devUri;                         //前端设备URI：puid@domain
	TCmsIpAddr           devIp;                          //前端设备IP
	string            devType;                        //设备类型：编码器或解码器
	string            devMode;                        //设备型号，CU会使用它判断一些老前端的能力，平台不会使用
	string            devName;                        //设备名称
	string            manufacturer;                   //设备厂商

	TCmsPuBaseCapSet       baseCapSet;                   //基本能力集
	map<int,int>        extCapSet;                    //扩展能力集
	map<int,int>        hdmiVidDecCfg;                    //HDMI视频解码配置
	CCmsInputPinEnable     inputPinEnable;               //并口输入通道使能配置

	int                 freeLicenceNum;               //免费的licence点数
	int                 directTransNum;               //设备支持的直传路数
	bool                isDecCombined;                //解码器是否合成码流

public:
	TCmsPuBaseConfig()
	{
		Clear();
	}

	~TCmsPuBaseConfig()
	{
		Clear();
	}

	void Clear()
	{
		devUri = STRING_NULL;
		devIp = STRING_NULL;
		devType = STRING_NULL;
		devMode = STRING_NULL;
		devName = STRING_NULL;
		manufacturer = STRING_NULL;
		baseCapSet.Clear();
		extCapSet.clear();      
		hdmiVidDecCfg.clear();      
		inputPinEnable = STRING_NULL;
		freeLicenceNum = 0;
		directTransNum = 0;
		isDecCombined = false; 
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetDevUri(const string& strDevUri)
	{
		devUri = strDevUri;
	}
	string& GetDevUri()
	{
		return devUri;
	}
	const string& GetDevUri() const
	{
		return devUri;
	}

	void SetDevIp(const TCmsIpAddr& strDevIp)
	{
		devIp = strDevIp;
	}
	TCmsIpAddr& GetDevIp()
	{
		return devIp;
	}
	const TCmsIpAddr& GetDevIp() const
	{
		return devIp;
	}

	void SetDevType(const string& strDevType)
	{
		devType = strDevType;
	}
	string& GetDevType()
	{
		return devType;
	}
	const string& GetDevType() const
	{
		return devType;
	}

	void SetDevMode(const string& strDevMode)
	{
		devMode = strDevMode;
	}
	string& GetDevMode()
	{
		return devMode;
	}
	const string& GetDevMode() const
	{
		return devMode;
	}

	void SetBaseCapSet(const TCmsPuBaseCapSet& cBaseCapSet)
	{
		baseCapSet = cBaseCapSet;
	}
	TCmsPuBaseCapSet& GetBaseCapSet()
	{
		return baseCapSet;
	}
	const TCmsPuBaseCapSet& GetBaseCapSet() const
	{
		return baseCapSet;
	}

	void SetExtCapSet(const map<int,int>& cExtCapSet)
	{
		extCapSet = cExtCapSet;
	}
	map<int,int>& GetExtCapSet()
	{
		return extCapSet;
	}
	const map<int,int>& GetExtCapSet() const
	{
		return extCapSet;
	}

	void SetHdmiVidDecCfg(const map<int,int>& cHdmiVidDecCfg)
	{
		hdmiVidDecCfg = cHdmiVidDecCfg;
	}
	map<int,int>& GetHdmiVidDecCfg()
	{
		return hdmiVidDecCfg;
	}
	const map<int,int>& GetHdmiVidDecCfg() const
	{
		return hdmiVidDecCfg;
	}

	void SetInputPinEnable(const CCmsInputPinEnable& strInputPinEnable)
	{
		inputPinEnable = strInputPinEnable;
	}
	CCmsInputPinEnable& GetInputPinEnable()
	{
		return inputPinEnable;
	}
	const CCmsInputPinEnable& GetInputPinEnable() const
	{
		return inputPinEnable;
	}

	void SetFreeLicenceNum(int nFreeLicenceNum)
	{
		freeLicenceNum = nFreeLicenceNum;
	}
	int GetFreeLicenceNum() const
	{
		return freeLicenceNum;
	}

	void SetDirectTransNum(int nDirectTransNum)
	{
		directTransNum = nDirectTransNum;
	}
	int GetDirectTransNum() const
	{
		return directTransNum;
	}

	void SetIsDecCombined(bool bIsDecCombined)
	{
		isDecCombined = bIsDecCombined;
	}
	bool GetIsDecCombined() const
	{
		return isDecCombined;
	}

public:
	void SetDevName(const string& strDevName)
	{
		devName = strDevName;
	}
	string& GetDevName()
	{
		return devName;
	}
	const string& GetDevName() const
	{
		return devName;
	}

public:
	void SetManufacturer(const string& strManufacturer)
	{
		manufacturer = strManufacturer;
	}
	string& GetManufacturer()
	{
		return manufacturer;
	}
	const string& GetManufacturer() const
	{
		return manufacturer;
	}

};

typedef vector<TCmsPuBaseConfig> CCmsPuBaseCfgList;

//设备通道配置
struct TCmsPuChnConfig 
{
private:
	TCmsChannel   devChn;                                  //设备通道
	TCmsSupportMediaType supportMediaType;                 //通道当前支持的所有媒体格式(只读)
	TCmsVideoEncodeParam chnVidFormat;                     //通道当前的视频格式
	TCmsAudioEncodeParam chnAudFormat;                     //通道当前的音频参数
	int        chnVidDynValue;                             //如果通道视频格式是H264，设置该通道的动态载荷
	CCmsVidSrcSet availVidSrcSet;                          //通道可用的视频源列表

public:
	TCmsPuChnConfig()
	{
		Clear();
	}

	~TCmsPuChnConfig()
	{
		Clear();
	}

	void Clear()
	{
		devChn.Clear();
		supportMediaType.Clear();
		chnVidFormat.Clear();
		chnAudFormat.Clear();
		chnVidDynValue = 0;
		availVidSrcSet.clear();
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetDevChn(const TCmsChannel& cDevChn)
	{
		devChn = cDevChn;
	}
	TCmsChannel& GetDevChn()
	{
		return devChn;
	}
	const TCmsChannel& GetDevChn() const
	{
		return devChn;
	}

	void SetSupportMediaType(const TCmsSupportMediaType& cSupportMediaType)
	{
		supportMediaType = cSupportMediaType;
	}
	TCmsSupportMediaType& GetSupportMediaType()
	{
		return supportMediaType;
	}
	const TCmsSupportMediaType& GetSupportMediaType() const
	{
		return supportMediaType;
	}

	void SetChnVidFormat(const TCmsVideoEncodeParam& cChnVidFormat)
	{
		chnVidFormat = cChnVidFormat;
	}
	TCmsVideoEncodeParam& GetChnVidFormat()
	{
		return chnVidFormat;
	}
	const TCmsVideoEncodeParam& GetChnVidFormat() const
	{
		return chnVidFormat;
	}

	void SetChnAudFormat(const TCmsAudioEncodeParam& cChnAudFormat)
	{
		chnAudFormat = cChnAudFormat;
	}
	TCmsAudioEncodeParam& GetChnAudFormat()
	{
		return chnAudFormat;
	}
	const TCmsAudioEncodeParam& GetChnAudFormat() const
	{
		return chnAudFormat;
	}

	void SetChnVidDynValue(int nChnVidDynValue)
	{
		chnVidDynValue = nChnVidDynValue;
	}
	int GetChnVidDynValue() const
	{
		return chnVidDynValue;
	}

	void SetAvailVidSrcSet(const CCmsVidSrcSet& cAvailVidSrcSet)
	{
		availVidSrcSet = cAvailVidSrcSet;
	}
	CCmsVidSrcSet& GetAvailVidSrcSet()
	{
		return availVidSrcSet;
	}
	const CCmsVidSrcSet& GetAvailVidSrcSet() const
	{
		return availVidSrcSet;
	}

};

typedef map<int, TCmsPuChnConfig> CCmsPuChnCfgList;

//视频源通道配置
struct TCmsPuVidSrcConfig 
{
private:
	string     name;                         //视频源名称
	TCmsChannel   vidChn;                       //视频源通道
	CCmsPuChnSet  availChnSet;                  //视频源当前正在使用的通道列表
	int        camType;                      //视频源的摄像头类型
	int        camId;                        //视频源的摄像头地址码

public:
	TCmsPuVidSrcConfig()
	{
		Clear();
	}

	~TCmsPuVidSrcConfig()
	{
		Clear();
	}

	void Clear()
	{
		name = STRING_NULL;
		vidChn.Clear();
		availChnSet.clear();
		camType = 0;
		camId = 0;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
public:
	void SetVidChn(const TCmsChannel& cVidChn)
	{
		vidChn = cVidChn;
	}
	TCmsChannel& GetVidChn()
	{
		return vidChn;
	}
	const TCmsChannel& GetVidChn() const
	{
		return vidChn;
	}

	void SetAvailChnSet(const CCmsPuChnSet& cAvailChnSet)
	{
		availChnSet = cAvailChnSet;
	}
	CCmsPuChnSet& GetAvailChnSet()
	{
		return availChnSet;
	}
	const CCmsPuChnSet& GetAvailChnSet() const
	{
		return availChnSet;
	}

	void SetCamType(int nCamType)
	{
		camType = nCamType;
	}
	int GetCamType() const
	{
		return camType;
	}

	void SetCamId(int nCamId)
	{
		camId = nCamId;
	}
	int GetCamId() const
	{
		return camId;
	}

public:
	void SetName(const string& strName)
	{
		name = strName;
	}
	string& GetName()
	{
		return name;
	}
	const string& GetName() const
	{
		return name;
	}

};

typedef map<int, TCmsPuVidSrcConfig> CCmsPuVidSrcCfgList;

enum TCmsChnStatus
{
	CMS_CHNSTATUS_IDEL = 0,      
	CMS_CHNSTATUS_ONLINE,
	CMS_CHNSTATUS_OFFLINE ,
	CMS_CHNSTATUS_MAX  = 256
};

struct TCmsPuChnStateConfig
{
private:
	TCmsChnStatus chnState;			// 通道状态
public:
	TCmsPuChnStateConfig()
	{
		Clear();
	}

	~TCmsPuChnStateConfig()
	{
		Clear();
	}

	void Clear()
	{
		chnState = CMS_CHNSTATUS_IDEL;
	}

	void SetChnState(TCmsChnStatus nChnState)
	{
		chnState = nChnState;
	}
	TCmsChnStatus GetChnState() const
	{
		return chnState;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef map<int, TCmsPuChnStateConfig> CCmsPuChnStateList;

struct TCmsPuDevConfig 
{
private:
    CCmsPuBaseCfgList		propCfgList;                    //设备属性配置
    CCmsPuChnCfgList		chnCfgList;                      //设备通道配置
    CCmsPuVidSrcCfgList	vidSrcCfgList;                //视频源通道配置
	CCmsPuChnStateList chnStateCfgList;				//设备通道状态配置
public:
    TCmsPuDevConfig()
    {
        Clear();
    }

    ~TCmsPuDevConfig()
    {
        Clear();
    }

    void Clear()
    {
        propCfgList.clear();
        chnCfgList.clear();
        vidSrcCfgList.clear();
		chnStateCfgList.clear();
    }

    void MergeDevConfig(const TCmsPuDevConfig& tCfg)
    {
        if (tCfg.GetBaseCfgList().size()>0)
        {
            SetBaseCfgList(tCfg.GetBaseCfgList());
        }

        CCmsPuChnCfgList tChnCfgList = tCfg.GetChnCfgList();
        typedef CCmsPuChnCfgList::const_iterator ChnMapIt;
        for (ChnMapIt mChnIt = tChnCfgList.begin(); mChnIt != tChnCfgList.end(); ++mChnIt)
        {
            chnCfgList[mChnIt->first] = mChnIt->second;          
        }

        CCmsPuVidSrcCfgList tVidSrcCfgList = tCfg.GetVidSrcCfgList();
        typedef CCmsPuVidSrcCfgList::const_iterator VidSrcMapIt;
        for (VidSrcMapIt mVidSrcIt = tVidSrcCfgList.begin(); mVidSrcIt != tVidSrcCfgList.end(); ++mVidSrcIt)
        {
            vidSrcCfgList[mVidSrcIt->first] = mVidSrcIt->second;          
        }

		const CCmsPuChnStateList &tChnStateCfgList = tCfg.GetChnStateCfgList();
		map<int, TCmsPuChnStateConfig>::const_iterator tPuChnStateCfgItor = tChnStateCfgList.begin();
		for (; tPuChnStateCfgItor != tChnStateCfgList.end(); tPuChnStateCfgItor++)
		{
			chnStateCfgList[tPuChnStateCfgItor->first] = tPuChnStateCfgItor->second;
		}
    }

    bool IsCompleted() const
    {
        if (GetBaseCfgList().size() == 0)
        {
            return false;
        }

        const TCmsPuBaseConfig& tPropCfg =  GetBaseCfgList().at(0);
        if (tPropCfg.GetDevType() == PU_DECODER)
        {
            //解码器直接返回
            return true;
        }

        if (GetChnCfgList().size() != tPropCfg.GetBaseCapSet().GetVidEncNum())
        {
            return false;
        }

        if (GetVidSrcCfgList().size() != tPropCfg.GetBaseCapSet().GetVideoInPortNum())
        {
            return false;
        }

        return true;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetBaseCfgList(const CCmsPuBaseCfgList& cPropCfgList)
    {
        propCfgList = cPropCfgList;
    }
    CCmsPuBaseCfgList& GetBaseCfgList()
    {
        return propCfgList;
    }
    const CCmsPuBaseCfgList& GetBaseCfgList() const
    {
        return propCfgList;
    }

    void SetChnCfgList(const CCmsPuChnCfgList& cChnCfgList)
    {
        chnCfgList = cChnCfgList;
    }
    CCmsPuChnCfgList& GetChnCfgList()
    {
        return chnCfgList;
    }
    const CCmsPuChnCfgList& GetChnCfgList() const
    {
        return chnCfgList;
    }

    void SetVidSrcCfgList(const CCmsPuVidSrcCfgList& cVidSrcCfgList)
    {
        vidSrcCfgList = cVidSrcCfgList;
    }
    CCmsPuVidSrcCfgList& GetVidSrcCfgList()
    {
        return vidSrcCfgList;
    }
    const CCmsPuVidSrcCfgList& GetVidSrcCfgList() const
    {
        return vidSrcCfgList;
    }

	void SetChnStateCfgList(const CCmsPuChnStateList& cCmsChnStateCfgList)
	{
		chnStateCfgList = cCmsChnStateCfgList;
	}
	CCmsPuChnStateList& GetChnStateCfgList()
	{
		return chnStateCfgList;
	}
	const CCmsPuChnStateList& GetChnStateCfgList() const
	{
		return chnStateCfgList;
	}
};

struct TCmsPuConfig 
{
private:
    //基本信息
    string            devUri;                                                     //前端uri: puid@domain
    TCmsIpAddr        devIp;                                                      //前端设备IP，网关IP
    TCmsDevType       devType;                                                    //设备类型：编码器或解码器
	string            devMode;													  //设备型号，CU会使用它判断一些老前端的能力，平台不会使用
	string            devName;													  //设备名称
	string            manufacturer;											      //设备厂商

    TCmsPuBaseCapSet	baseCapSet;                                               //基本能力集
	CCmsPuExtCapSet     extCapSet;												  //扩展能力集
	map<int,int>        hdmiVidDecCfg;											  //hdmi视频解码配置<(0-3)hdmiIndex,(0.1.2.4.9.16)DecStyle>
	CCmsInputPinEnable  inputPinEnable;											  //并口输入通道使能配置

    //编码通道信息
    CCmsVidFormatList         chnVidFormat;                                        //通道当前的视频格式
	CCmsVidDynValue           chnVidDynValue;									   //如果通道视频格式是H264，设置该通道的动态载荷
    CCmsSupportMediaList      supportMediaType;                                    //通道支持的所有媒体格式(只读)
    CCmsVideoEncodeParamList  chnVidEncParam;                                      //通道当前的视频编码参数
	CCmsAudioEncodeParamList  chnAudEncParam;                                      //通道当前的音频编码参数  

	CCmsVidSrcName			  vidSrcName;											//视频源别名

	//视频源通道信息
	CCmsPuChnSetList		  vidSrcChnSet;											//视频源使用的通道列表  
	CCmsVidSrcSetList         chnVidSrcSet;                                         //通道可用的视频源列表

    CCmsCamTypeList			  camType;												//视频源的摄像头类型
    CCmsCamIdList			  camId;												//视频源的摄像头地址码
	CCmsBitRateList		      bitRate;												//通道的比特率 
	CCmsPuChnStateList        chnStatusSet;											//通道状态集

	int						  freeLicenceNum;									    //免费的licence点数
	int                       directTransNum;									    //设备支持的直传路数
	bool                      isDecCombined;									    //解码器是否合成码流
     
public:
    TCmsPuConfig()
    {
        Clear();
    }

    ~TCmsPuConfig()
    {
        Clear();
    }

    void Clear()
    {
        devUri = STRING_NULL;
        devIp = STRING_NULL;
        devType = STRING_NULL;
		devMode = STRING_NULL;
		devName = STRING_NULL;
		manufacturer = STRING_NULL;

        baseCapSet.Clear(); 
		extCapSet.clear();
		hdmiVidDecCfg.clear();
		inputPinEnable = STRING_NULL;

        vidSrcChnSet.clear();
        chnVidFormat.clear();  
		chnVidDynValue.clear();
        supportMediaType.clear();
        chnVidEncParam.clear();
		chnAudEncParam.clear();
		vidSrcName.clear();

        chnVidSrcSet.clear();   
        camType.clear();        
        camId.clear();  
		bitRate.clear();
		chnStatusSet.clear();
		freeLicenceNum = 0;
		directTransNum = 0;
		isDecCombined = false;
    }
public:

	//转换接口
	TCmsPuBaseConfig GetBaseConfig() const
	{ 
		TCmsPuBaseConfig tCfg;
		tCfg.SetDevUri(GetDevUri());
		tCfg.SetDevIp(GetDevIp());
		tCfg.SetDevType(GetDevType());
		tCfg.SetDevMode(GetDevMode());
		tCfg.SetDevName(GetDevName());
		tCfg.SetManufacturer(GetManufacturer());
		tCfg.SetBaseCapSet(GetBaseCapSet());
		tCfg.SetExtCapSet(GetExtCapSet());
		tCfg.SetHdmiVidDecCfg(GetHdmiVidDecCfg());
		tCfg.SetInputPinEnable(GetInputPinEnable());
		tCfg.SetFreeLicenceNum(GetFreeLicenceNum());
		tCfg.SetDirectTransNum(GetDirectTransNum());
		tCfg.SetIsDecCombined(GetIsDecCombined());

		return tCfg;
	}
	void SetBaseConfig(const TCmsPuBaseConfig& tCfg)
	{
		SetDevUri(tCfg.GetDevUri());
		SetDevIp(tCfg.GetDevIp());
		SetDevType(tCfg.GetDevType());
		SetDevMode(tCfg.GetDevMode());
		SetDevName(tCfg.GetDevName());
		SetManufacturer(tCfg.GetManufacturer());
		SetBaseCapSet(tCfg.GetBaseCapSet());
		SetExtCapSet(tCfg.GetExtCapSet());
		SetHdmiVidDecCfg(tCfg.GetHdmiVidDecCfg());
		SetInputPinEnable(tCfg.GetInputPinEnable());
		SetFreeLicenceNum(tCfg.GetFreeLicenceNum());
		SetDirectTransNum(tCfg.GetDirectTransNum());
		SetIsDecCombined(tCfg.GetIsDecCombined());
	}

	TCmsPuChnConfig GetChnConfig(int nChnNO) const
	{
		TCmsPuChnConfig tCfg;

		map<int,int>::const_iterator ptDynValue = GetVidDynValue().find(nChnNO);
		if (ptDynValue != GetVidDynValue().end())
		{
			tCfg.SetChnVidDynValue(ptDynValue->second);
		}

		map<int,TCmsVideoEncodeParam>::const_iterator ptCurVidEncParam = GetChnVidEncParam().find(nChnNO);
		if (ptCurVidEncParam != GetChnVidEncParam().end())
		{
			tCfg.SetChnVidFormat(ptCurVidEncParam->second);
		}      

		map<int,TCmsAudioEncodeParam>::const_iterator ptCurAudEncParam = GetChnAudEncParam().find(nChnNO);
		if (ptCurAudEncParam != GetChnAudEncParam().end())
		{
			tCfg.SetChnAudFormat(ptCurAudEncParam->second);
		}

		map<int,TCmsSupportMediaType>::const_iterator ptSupportVidEncParam = GetSupportMediaType().find(nChnNO);
		if (ptSupportVidEncParam != GetSupportMediaType().end())
		{
			tCfg.SetSupportMediaType(ptSupportVidEncParam->second);
		} 

		const map<int,CCmsVidSrcSet>::const_iterator ptVidSrcSet = GetChnVidSrcSet().find(nChnNO);
		if (ptVidSrcSet != GetChnVidSrcSet().end())
		{
			tCfg.SetAvailVidSrcSet(ptVidSrcSet->second);
		}

		return tCfg;
	}
	void SetChnConfig(int nChnNO, const TCmsPuChnConfig& tCfg)
	{
		if (nChnNO < 0 && nChnNO > (GetBaseCapSet().GetVidEncNum()-1))
		{
			return;
		}

		GetVidDynValue()[nChnNO] = tCfg.GetChnVidDynValue();
		GetChnVidEncParam()[nChnNO] = tCfg.GetChnVidFormat();
		GetChnAudEncParam()[nChnNO] = tCfg.GetChnAudFormat();
		GetSupportMediaType()[nChnNO] = tCfg.GetSupportMediaType();
		GetChnVidSrcSet()[nChnNO] = tCfg.GetAvailVidSrcSet();
	}

	TCmsPuChnStateConfig GetChnStateConfig(int nChnNO) const
	{		
		CCmsPuChnStateList::const_iterator it = chnStatusSet.find(nChnNO);
		if (chnStatusSet.end() != it)
		{
			return it->second;
		}

		return TCmsPuChnStateConfig();
	}

	void SetChnState(int nChnNO, const TCmsPuChnStateConfig& tCmsPuChnStateCfg)
	{
		chnStatusSet[nChnNO] = tCmsPuChnStateCfg;
	}

    TCmsPuVidSrcConfig GetVidSrcConfig(int nChnNO) const
    {
        TCmsPuVidSrcConfig tCfg;

        map<int,string>::const_iterator ptVidSrcName = GetVidSrcName().find(nChnNO);
        if (ptVidSrcName != GetVidSrcName().end())
        {
            tCfg.SetName(ptVidSrcName->second);
        }

        map<int,int>::const_iterator ptCamId = GetCamId().find(nChnNO);
        if (ptCamId != GetCamId().end())
        {
            tCfg.SetCamId(ptCamId->second);
        }

        map<int,int>::const_iterator ptCamType = GetCamType().find(nChnNO);
        if (ptCamType != GetCamType().end())
        {
            tCfg.SetCamType(ptCamType->second);
        }

        const map<int,CCmsPuChnSet>::const_iterator ptChnSet = GetVidSrcChnSet().find(nChnNO);
        if (ptChnSet != GetVidSrcChnSet().end())
        {
            tCfg.SetAvailChnSet(ptChnSet->second);
        }     

        return tCfg;
    }
    void SetVidSrcConfig(int nChnNO, const TCmsPuVidSrcConfig& tCfg)
    {
        if (nChnNO < 0 && nChnNO > (GetBaseCapSet().GetVideoInPortNum()-1))
        {
            return;
        }

        GetVidSrcName()[nChnNO] = tCfg.GetName();
        GetCamId()[nChnNO] = tCfg.GetCamId();
        GetCamType()[nChnNO] = tCfg.GetCamType();
        GetVidSrcChnSet()[nChnNO] = tCfg.GetAvailChnSet();
    }

    TCmsPuDevConfig GetDevConfig() const
    {
        TCmsPuDevConfig tCfg;

        if (!GetDevUri().empty())
        {
            tCfg.GetBaseCfgList().push_back(GetBaseConfig());
        }

        for (int nVidEndIdx=0; nVidEndIdx<GetBaseCapSet().GetVidEncNum(); nVidEndIdx++)
        {
            if (GetChnVidEncParam().find(nVidEndIdx) != GetChnVidEncParam().end())
            {
                tCfg.GetChnCfgList()[nVidEndIdx] = GetChnConfig(nVidEndIdx);
            }        
        }

        for (int nVidSrcIdx=0; nVidSrcIdx<GetBaseCapSet().GetVideoInPortNum(); nVidSrcIdx++)
        {
            if (GetCamType().find(nVidSrcIdx) != GetCamType().end())
            {
                tCfg.GetVidSrcCfgList()[nVidSrcIdx]= GetVidSrcConfig(nVidSrcIdx);
            }       
        }

		for (unsigned int iChnStateIndex = 0; iChnStateIndex < GetChnStateSet().size(); iChnStateIndex++)
		{
			tCfg.GetChnStateCfgList()[iChnStateIndex]= GetChnStateConfig(iChnStateIndex);      
		}

        return tCfg;
    }

    void SetDevConfig(const TCmsPuDevConfig& tCfg)
    {
        if (tCfg.GetBaseCfgList().size()>0)
        {
            SetBaseConfig(tCfg.GetBaseCfgList().at(0));
        }

        CCmsPuChnCfgList tChnCfgList = tCfg.GetChnCfgList();
        typedef CCmsPuChnCfgList::const_iterator ChnMapIt;
        for (ChnMapIt mChnIt = tChnCfgList.begin(); mChnIt != tChnCfgList.end(); ++mChnIt)
        {
            SetChnConfig(mChnIt->first, mChnIt->second);          
        }

        CCmsPuVidSrcCfgList tVidSrcCfgList = tCfg.GetVidSrcCfgList();
        typedef CCmsPuVidSrcCfgList::const_iterator VidSrcMapIt;
        for (VidSrcMapIt mVidSrcIt = tVidSrcCfgList.begin(); mVidSrcIt != tVidSrcCfgList.end(); ++mVidSrcIt)
        {
            SetVidSrcConfig(mVidSrcIt->first, mVidSrcIt->second);          
        }

		CCmsPuChnStateList tChnStateCfgList = tCfg.GetChnStateCfgList();
		typedef CCmsPuChnStateList::const_iterator ChnStateIt;
		for (ChnStateIt mStateIt = tChnStateCfgList.begin(); mStateIt != tChnStateCfgList.end(); ++mStateIt)
		{
			SetChnState(mStateIt->first, mStateIt->second);
		}
    }

    void MergeDevConfig(const TCmsPuDevConfig& tCfg)
    {
        SetDevConfig(tCfg);
    }


    bool IsCompleted() const
    {
        return GetDevConfig().IsCompleted();
    }


    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetDevIp(const TCmsIpAddr& strDevIp)
    {
        devIp = strDevIp;
    }
    TCmsIpAddr& GetDevIp()
    {
        return devIp;
    }
    const TCmsIpAddr& GetDevIp() const
    {
        return devIp;
    }

    void SetDevType(const TCmsDevType& strDevType)
    {
        devType = strDevType;
    }
    TCmsDevType& GetDevType()
    {
        return devType;
    }
    const TCmsDevType& GetDevType() const
    {
        return devType;
    }

	const string& GetDevMode() const
	{
		return devMode;
	}
	string& GetDevMode()
	{
		return devMode;
	}
	void SetDevMode(const string& strDevMode)
	{
		devMode = strDevMode;
	}

	void SetDevName(const string& strDevName)
	{
		devName = strDevName;
	}
	string& GetDevName()
	{
		return devName;
	}
	const string& GetDevName() const
	{
		return devName;
	}

	void SetManufacturer(const string& strManufacturer)
	{
		manufacturer = strManufacturer;
	}
	string& GetManufacturer()
	{
		return manufacturer;
	}
	const string& GetManufacturer() const
	{
		return manufacturer;
	}

    void SetBaseCapSet(const TCmsPuBaseCapSet& cBaseCapSet)
    {
        baseCapSet = cBaseCapSet;
    }
    TCmsPuBaseCapSet& GetBaseCapSet()
    {
        return baseCapSet;
    }
    const TCmsPuBaseCapSet& GetBaseCapSet() const
    {
        return baseCapSet;
    }

	const map<int,int>& GetExtCapSet() const
	{
		return extCapSet;
	}

	map<int,int>& GetExtCapSet()
	{
		return extCapSet;
	}
	void SetExtCapSet(const map<int,int>& tExtCapSet)
	{
		extCapSet = tExtCapSet;
	}

	const map<int,int>& GetHdmiVidDecCfg() const
	{
		return hdmiVidDecCfg;
	}

	map<int,int>& GetHdmiVidDecCfg()
	{
		return hdmiVidDecCfg;
	}
	void SetHdmiVidDecCfg(const map<int,int>& tHdmiVidDecCfg)
	{
		hdmiVidDecCfg = tHdmiVidDecCfg;
	}

	const CCmsInputPinEnable& GetInputPinEnable() const
	{
		return inputPinEnable;
	}
	CCmsInputPinEnable& GetInputPinEnable()
	{
		return inputPinEnable;
	}
	void SetInputPinEnable(const CCmsInputPinEnable& tInputPinEnable)
	{
		inputPinEnable = tInputPinEnable;
	}

    void SetChnVidSrcSet(const CCmsVidSrcSetList& cChnVidSrcSet)
    {
        chnVidSrcSet = cChnVidSrcSet;
    }
    CCmsVidSrcSetList& GetChnVidSrcSet()
    {
        return chnVidSrcSet;
    }
    const CCmsVidSrcSetList& GetChnVidSrcSet() const
    {
        return chnVidSrcSet;
    }

    void SetChnVidFormat(const CCmsVidFormatList& cChnVidFormat)
    {
        chnVidFormat = cChnVidFormat;
    }
    CCmsVidFormatList& GetChnVidFormat()
    {
        return chnVidFormat;
    }
    const CCmsVidFormatList& GetChnVidFormat() const
    {
        return chnVidFormat;
    }

	const map<int,int>& GetVidDynValue() const
	{
		return chnVidDynValue;
	}
	map<int,int>& GetVidDynValue()
	{
		return chnVidDynValue;
	}
	void SetVidDynValue(const map<int,int>& tVidDynValue)
	{
		chnVidDynValue = tVidDynValue;
	}

    void SetSupportMediaType(const CCmsSupportMediaList& cSupportMediaType)
    {
        supportMediaType = cSupportMediaType;
    }
    CCmsSupportMediaList& GetSupportMediaType()
    {
        return supportMediaType;
    }
    const CCmsSupportMediaList& GetSupportMediaType() const
    {
        return supportMediaType;
    }

    void SetChnVidEncParam(const CCmsVideoEncodeParamList& cChnVidEncParam)
    {
        chnVidEncParam = cChnVidEncParam;
    }
    CCmsVideoEncodeParamList& GetChnVidEncParam()
    {
        return chnVidEncParam;
    }
    const CCmsVideoEncodeParamList& GetChnVidEncParam() const
    {
        return chnVidEncParam;
    }

	const CCmsAudioEncodeParamList& GetChnAudEncParam() const
	{
		return chnAudEncParam;
	}
	CCmsAudioEncodeParamList& GetChnAudEncParam()
	{
		return chnAudEncParam;
	}
	void SetChnAudEncParam(const CCmsAudioEncodeParamList& cChnAudEncParam)
	{
		chnAudEncParam = cChnAudEncParam;
	}

    void SetVidSrcChnSet(const CCmsPuChnSetList& cVidSrcChnSet)
    {
        vidSrcChnSet = cVidSrcChnSet;
    }
    CCmsPuChnSetList& GetVidSrcChnSet()
    {
        return vidSrcChnSet;
    }
    const CCmsPuChnSetList& GetVidSrcChnSet() const
    {
        return vidSrcChnSet;
    }

	void SetVidSrcName(const map<int,string>& cVidSrcName)
	{
		vidSrcName = cVidSrcName;
	}
	map<int,string>& GetVidSrcName()
	{
		return vidSrcName;
	}
	const map<int,string>& GetVidSrcName() const
	{
		return vidSrcName;
	}

    void SetCamType(const CCmsCamTypeList& cCamType)
    {
        camType = cCamType;
    }
    CCmsCamTypeList& GetCamType()
    {
        return camType;
    }
    const CCmsCamTypeList& GetCamType() const
    {
        return camType;
    }

    void SetCamId(const CCmsCamIdList& cCamId)
    {
        camId = cCamId;
    }
    CCmsCamIdList& GetCamId()
    {
        return camId;
    }
    const CCmsCamIdList& GetCamId() const
    {
        return camId;
    }

	const map<int,int>& GetBitRate() const
	{
		return bitRate;
	}
	map<int,int>& GetBitRate()
	{
		return bitRate;
	}
	void SetBitRate(const map<int,int>& tBitRate)
	{
		bitRate = tBitRate;
	}

	const CCmsPuChnStateList& GetChnStateSet() const
	{
		return chnStatusSet;
	}

	CCmsPuChnStateList& GetChnStateSet()
	{
		return chnStatusSet;
	}
	void SetChnStateSet(const CCmsPuChnStateList& tChnStateSet)
	{
		chnStatusSet = tChnStateSet;
	}

	int GetFreeLicenceNum() const
	{
		return freeLicenceNum;
	}
	void SetFreeLicenceNum(int tFreeLicenceNum)
	{
		freeLicenceNum = tFreeLicenceNum;
	}

	int GetDirectTransNum() const
	{
		return directTransNum;
	}
	void SetDirectTransNum(int tDirectTransNum)
	{
		directTransNum = tDirectTransNum;
	}

	bool GetIsDecCombined() const
	{
		return isDecCombined;
	}
	void SetIsDecCombined(bool bIsDecCombined)
	{
		isDecCombined = bIsDecCombined;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};



//告警标志
typedef string CCmsAlarmFlag;
#define    ALARM_FLAG_OCCUR         "occur"           //产生
#define    ALARM_FLAG_RESTORE       "restore"         //恢复

//告警类型
typedef string CCmsAlarmType;
#define    ALARM_TYPE_PIN             "pin"          //并口
#define    ALARM_TYPE_MOVE            "move"         //移动监测
#define    ALARM_TYPE_VIDLOST         "vid lost"     //视频源丢失
#define    ALARM_TYPE_DISKFULL        "disk full"    //磁盘满
#define    ALARM_TYPE_INTELLIGENT     "intelligent"  //智能
#define    ALARM_TYPE_VIDEOPOLL       "video poll"   //视质轮询
//添加基础智能告警子类型
#define    ALARM_TYPE_EXCEPTION_EVENT "exception event" //异常事件消息
#define    ALARM_TYPE_DEFOCUS         "defocus"         //虚焦检测
#define    ALARM_TYPE_SCENE_CHANGE    "sceme change"    //场景变换
#define    ALARM_TYPE_REGION_INVASION "region invasion" //区域入侵
#define    ALARM_TYPE_REGION_LEAVING  "region leaving"  //区域离开
#define    ALARM_TYPE_OBJECT_TAKEN    "object taken"    //物品拿取
#define    ALARM_TYPE_OBJECT_LEFT     "object left"     //物品遗留
#define    ALARM_TYPE_PEOPLE_GATHER   "people gather"   //人员聚集
#define    ALARM_TYPE_CORGON          "corgon"          //警戒线
#define    ALARM_TYPE_AUDIO_ABNORMAL  "audio abnormal"  //声音异常
#define    ALARM_TYPE_LUMA_CHANGE     "luma change"     //亮度变化


//pu告警输入
struct TCmsAlarmInput
{
private:    
    string            devUri;                          //前端设备URI：puid@domain
    CCmsAlarmType     alarmType;                       //告警类型: ALARM_TYPE_PIN, ALARM_TYPE_MOVE等
    int               inputId;                         //告警输入通道
    CCmsAlarmFlag     alarmFlag;                       //告警标志: ALARM_FLAG_OCCUR, ALARM_FLAG_RESTORE
    int               detAreaId;                       //移动侦测表示告警区域号，移动监测时有效
    string            alarmTime;                       //告警时间
    string            alarmData;                       //告警数据

public:
    TCmsAlarmInput()
    {
        Clear();
    }

    ~TCmsAlarmInput()
    {
        Clear();
    }

    void Clear()
    {
        inputId = 0;
        detAreaId = 0;
        devUri = STRING_NULL;
        alarmType = STRING_NULL;
        alarmFlag = STRING_NULL;
        alarmTime = STRING_NULL;
        alarmData = STRING_NULL;
    }
public:
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    const string& GetDevUri() const
    {
        return devUri;
    }

    void SetAlarmType(const CCmsAlarmType& strAlarmType)
    {
        alarmType = strAlarmType;
    }
    CCmsAlarmType& GetAlarmType()
    {
        return alarmType;
    }
    const CCmsAlarmType& GetAlarmType() const
    {
        return alarmType;
    }

    void SetInputId(int nInputId)
    {
        inputId = nInputId;
    }
    int GetInputId() const
    {
        return inputId;
    }

    void SetAlarmFlag(const CCmsAlarmFlag& strAlarmFlag)
    {
        alarmFlag = strAlarmFlag;
    }
    CCmsAlarmFlag& GetAlarmFlag()
    {
        return alarmFlag;
    }
    const CCmsAlarmFlag& GetAlarmFlag() const
    {
        return alarmFlag;
    }

    void SetDetAreaId(int nDetAreaId)
    {
        detAreaId = nDetAreaId;
    }
    int GetDetAreaId() const
    {
        return detAreaId;
    }

    void SetAlarmTime(const string& strAlarmTime)
    {
        alarmTime = strAlarmTime;
    }
    string& GetAlarmTime()
    {
        return alarmTime;
    }
    const string& GetAlarmTime() const
    {
        return alarmTime;
    }

    void SetAlarmData(const string& strAlarmData)
    {
        alarmData = strAlarmData;
    }
    string& GetAlarmData()
    {
        return alarmData;
    }
    const string& GetAlarmData() const
    {
        return alarmData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef    vector<TCmsAlarmInput>   CCmsAlarmInputArray;


typedef string TCmsSdpSessionName;
#define SDP_SESSION_PLAY        "Play"              //实时码流
#define SDP_SESSION_PLAYBACK    "Playback"          //历史码流回放
#define SDP_SESSION_DOWNLOAD    "Download"          //历史码流下载
#define SDP_SESSION_AUDIOCALL   "AudioCall"         //音频呼叫

typedef string TCmsRecToken;
typedef string TCmsPlaybackType;
#define PLAYBACK_TYPE_PLATFORM    "PLATFORM_PLAYBACK"   //平台放像
#define PLAYBACK_TYPE_PU          "PU_PLAYBACK"         //前端放像

typedef string TCmsRecLockOprType;
#define CMS_RCS_REC_LOCK               "lock"
#define CMS_RCS_REC_UNLOCK             "unlock"
#define CMS_RCS_REC_LOCK_GET           "get"

struct TCmsSdpUri
{
private:
	TCmsRecToken		recToken;                     //录像文件url
	TCmsChannel			puChn;              
	TCmsPlaybackType	playbackType;   
    string              recDomainName;                // 录像所在域

public:
	TCmsSdpUri()
	{
		Clear();
	}

	void Clear()
	{
		recToken = STRING_NULL;
		puChn.Clear();
		playbackType = STRING_NULL;
	}

	bool IsEmpty()
	{
		if (recToken.empty() && puChn.IsNull())
		{
			return true;
		}

		return false;
	}

	friend std::ostream& operator<<(std::ostream& output, const TCmsSdpUri& obj)
	{
		output<<std::endl;
		output<<"recToken: "<<obj.recToken<<std::endl;
		output<<"puChn: "<<obj.puChn<<std::endl;
		output<<"playbackType: "<<obj.playbackType<<std::endl;
        output<<"recDomainName"<<obj.recDomainName<<std::endl;

		return output;
	}

public:
	void SetRecToken(const TCmsRecToken& strRecToken)
	{
		recToken = strRecToken;
	}
	TCmsRecToken& GetRecToken()
	{
		return recToken;
	}
	const TCmsRecToken& GetRecToken() const
	{
		return recToken;
	}

	void SetPuChn(const TCmsChannel& cPuChn)
	{
		puChn = cPuChn;
	}
	TCmsChannel& GetPuChn()
	{
		return puChn;
	}
	const TCmsChannel& GetPuChn() const
	{
		return puChn;
	}

	void SetPlaybackType(const TCmsPlaybackType& strPlaybackType)
	{
		playbackType = strPlaybackType;
	}
	TCmsPlaybackType& GetPlaybackType()
	{
		return playbackType;
	}
	const TCmsPlaybackType& GetPlaybackType() const
	{
		return playbackType;
	}

    void SetRecDomainName(const string& strDomainName)
    {
        recDomainName = strDomainName;
    }
    string& GetRecDomainName()
    {
        return recDomainName;
    }
    const string& GetRecDomainName() const
    {
        return recDomainName;
    }

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef std::string TCmsMediaStreamType;

struct TCmsStreamTag
{
private:
	string manufacture;
	TCmsMediaStreamType mediaStreamType;

public:
	TCmsStreamTag()
	{

	}

	TCmsStreamTag(const string& strManufacture, const TCmsMediaStreamType& tMediaStreamType)
	{
		manufacture = strManufacture;
		mediaStreamType = tMediaStreamType;
	}

	void Clear()
	{
		manufacture = STRING_NULL;
		mediaStreamType = STRING_NULL;
	}

	void SetManufacture(const string& strManufacture)
	{
		manufacture = strManufacture;
	}
	string& GetManufacture()
	{
		return manufacture;
	}
	const string& GetManufacture() const
	{
		return manufacture;
	}

	void SetMediaStreamType(const TCmsMediaStreamType& cMediaStreamType)
	{
		mediaStreamType = cMediaStreamType;
	}
	TCmsMediaStreamType& GetMediaStreamType()
	{
		return mediaStreamType;
	}
	const TCmsMediaStreamType& GetMediaStreamType() const
	{
		return mediaStreamType;
	}

	bool Parse(const string& strStreamTag)
	{
        char achManufacture[128] = {0};
		char achMediaStreamType[128] = {0};
		achManufacture[sizeof(achManufacture)-1] = '\0';
		achMediaStreamType[sizeof(achMediaStreamType)-1] = '\0';
		if (sscanf(strStreamTag.c_str(), "%127[^:]:%127s", achManufacture, achMediaStreamType) != 2)
		{
			return false;
		}

		manufacture = achManufacture;
		mediaStreamType = achMediaStreamType;

		return true;
	}

	string ToString() const
	{
		return manufacture+":"+mediaStreamType;
	}

	bool IsSupportRTPResend() const
	{
		if ( mediaStreamType.empty() )
		{
			return false;
		}

		if ( string::npos == mediaStreamType.find("RTP") )
		{
			return false;
		}

		return true;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

/*
//struct TCmsTime 
//{
//	u16		wYear;		//年
//	u8		byMonth;	//月 1-12
//	u8		byDay;		//日 1-31
//	u8		byHour;		//时 0-23
//	u8		byMinute;	//分 0-59
//	u8		bySecond;	//秒 0-59
//
//public:
//	void Clear()
//	{
//		wYear = 0;
//		byMonth = 0;
//		byDay = 0;
//		byHour = 0;
//		byMinute = 0;
//		bySecond = 0;
//	}
//
//	friend std::ostream& operator<<(std::ostream& output, const TCmsTime& obj)
//	{
//		output<<std::endl;
//		output<<"Year: "  << obj.wYear    <<std::endl;
//		output<<"Month: " << obj.byMonth  <<std::endl;
//		output<<"Day: "   << obj.byDay    <<std::endl;
//		output<<"Hour: "  << obj.byHour	  <<std::endl;
//		output<<"Minute: "<< obj.byMinute <<std::endl;
//		output<<"Second: "<< obj.bySecond <<std::endl;
//
//		return output;
//	}
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
*/

typedef string TCmsTime;

struct TCmsTimeRange
{
private:
	TCmsTime          startTime;
	TCmsTime          endTime;

public:
	TCmsTimeRange()
	{
		Clear();
	}

	void Clear()
	{
		startTime = STRING_NULL;
		endTime = STRING_NULL;
	}

public:

	//startTime
	const TCmsTime& GetStartTime() const
	{
		return startTime;
	}
	TCmsTime& GetStartTime()
	{
		return startTime;
	}
	void SetStartTime(const TCmsTime& var)
	{
		startTime = var;
	}

	//endTime
	const TCmsTime& GetEndTime() const
	{
		return endTime;
	}
	TCmsTime& GetEndTime()
	{
		return endTime;
	}
	void SetEndTime(const TCmsTime& var)
	{
		endTime = var;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsTimeRange> TCmsTimeRangeList;

typedef std::string TCmsMidaType;
#define CMS_MEDIA_TYPE_VIDEO      "VIDEO"
#define CMS_MEDIA_TYPE_AUDIO      "AUDIO"
#define CMS_MEDIA_TYPE_DATA       "DATA"
#define CMS_MEDIA_TYPE_UNKNOWN    "UNKNOWN"


typedef map<string, string> TCmsMediaFormatParam;
//Video编码参数：
#define CMS_VIDEO_ENC_PARAM_RESOLUTION      "VideoResolution"       //视频分辨率。值：string类型,解析格式"W:%d,H:%d"
#define CMS_VIDEO_ENC_FRAME_RATE            "VideoFrameRate"        //码流帧率。值：int类型
#define CMS_VIDEO_ENC_BIT_RATE              "VideoBitRate"          //码流比特率, Kbps。值：int类型
#define CMS_VIDEO_ENC_KEY_FRAME_INTVL       "VideoKeyFrameIntvl"    //码流的关键帧间隔。值：int类型
#define CMS_VIDEO_ENC_ENCODE_FORMAT         "VideoEncodeFormat"     //视频编码格式。值：int类型
#define CMS_VIDEO_ENC_RATE_TYPE             "VideoRateType"         //码率类型。值：int类型（1-固定码率，2-可变码率）
#define CMS_VIDEO_ENC_CLOCK_RATE            "VideoClockRate"        //视频采样率。值：int类型

//Audio编码参数：
#define CMS_AUDIO_ENC_SAMPLE_RATE           "AudioSampleRate"       //采样率。值：int类型
#define CMS_AUDIO_ENC_CHANNEL_NUM           "AudioChannelNum"       //声道数。值：int类型
#define CMS_AUDIO_ENC_ENCODE_FORMAT         "AudioEncodeFormat"     //音频编码格式。值：int类型
#define CMS_AUDIO_ENC_BIT_RATE              "AudioBitRate"          //码流比特率, Kbps。值：int类型
#define CMS_AUDIO_ENC_CLOCK_RATE            "AudioClockRate"        //时间戳采样率。值：int类型

#define MEDIA_MANUFACTURE_KEDACOM "kedacom"

struct TCmsMediaFormat
{
private:
	int    payloadType;             //编码格式ID
	string encodingName;            //编码格式名，对应于payloadType的名称
	string manufacture;             //媒体流厂商
	TCmsMediaFormatParam formatParam;  //详细的格式参数。根据实际需要设置，比如分辨率、关键帧间隔等

public:
	TCmsMediaFormat()
	{
		SetPayloadType(0);
		manufacture = MEDIA_MANUFACTURE_KEDACOM;
	}

	TCmsMediaFormat(unsigned int dwPayloadType, const string& strManufacture)
	{
		SetPayloadType(dwPayloadType);
		manufacture = strManufacture;
	}

	int GetPayloadType() const
	{
		return payloadType;
	}

	void SetPayloadType(int dwValue)
	{
		payloadType = dwValue;
	}

	const string& GetEncodingName() const
	{
		return encodingName;
	}
	string& GetEncodingName()
	{
		return encodingName;
	}
	void SetEncodingName(const string& strValue)
	{
		encodingName = strValue;
	}

	const string& GetManufacture() const
	{
		return manufacture;
	}
	string& GetManufacture()
	{
		return manufacture;
	}
	void SetManufacture(const string& strValue)
	{
		manufacture = strValue;
	}

	const TCmsMediaFormatParam& GetFormatParam() const
	{
		return formatParam;
	}
	TCmsMediaFormatParam& GetFormatParam()
	{
		return formatParam;
	}
	void SetFormatParam(const TCmsMediaFormatParam& tValue)
	{
		formatParam = tValue;
	}

	void AddFormatParam(const string& key, const string& value)
	{
		formatParam[key] = value;
	}

	void SetFormatParam(const string& strKey, const string& strValue)
	{
		formatParam[strKey] = strValue;
	}
	void SetFormatParam(const string& strKey, int nValue)
	{
		char buf[256]={0};
		sprintf( buf, "%d", nValue );
		formatParam[strKey] = buf;
	}
	void SetFormatParam(const string& strKey, double lfValue)
	{
		char buf[256]={0};
		sprintf( buf, "%lf", lfValue );
		formatParam[strKey] = buf;
	}

	bool GetFormatParam(const string& strKey, string& strValue) const
	{
		TCmsMediaFormatParam::const_iterator itr = formatParam.find(strKey);
		if (itr != formatParam.end())
		{
			strValue = itr->second;

			return true;
		}

		return false;
	}
	bool GetFormatParam(const string& strKey, int& nValue) const
	{
		TCmsMediaFormatParam::const_iterator itr = formatParam.find(strKey);
		if (itr != formatParam.end())
		{
			nValue = atoi(itr->second.c_str());

			return true;
		}

		return false;
	}
	bool GetFormatParam(const string& strKey, double& lfValue) const
	{
		TCmsMediaFormatParam::const_iterator itr = formatParam.find(strKey);
		if (itr != formatParam.end())
		{
			lfValue = atof(itr->second.c_str());

			return true;
		}

		return false;
	}


public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsMediaFormat& obj)
	{
		output<<std::endl;
		output<<"payloadType: "<<obj.payloadType <<"[" <<obj.encodingName<<"]"<<std::endl;
		output<<"manufacture: "<<obj.manufacture<<std::endl;

		if(obj.formatParam.size() > 0)
		{
			TCmsMediaFormatParam::const_iterator itr = obj.formatParam.begin();
			for(; itr != obj.formatParam.end(); itr++)
			{
				output<<itr->first<<": "<<itr->second<<"; ";
			}
			output<<std::endl;
		}

		return output;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef std::string TCmsMediaTransport;
#define CMS_MEDIA_TRANS_RTP       "RTP"         //实时传输协议,  MediaType[VIDEO] TransProto[RTP] TransChannelList[RTP,RTCP]
//              MediaType[AUDIO] TransProto[RTP] TransChannelList[RTP,RTCP]
#define MEDIA_TRANS_RTSP      "RTSP"
#define MEDIA_TRANS_DATA      "DATA"        //单通道数据传输协议，MediaType[DATA] TransProto[DATA] TransChannelList[DATA]
#define MEDIA_TRANS_DATARTP   "DATARTP"     //双通道数据传输协议，MediaType[DATA] TransProto[DATARTP] TransChannelList[RTP,RTCP]

typedef std::string TCmsLowerTransport;
#define CMS_LOWER_TRANS_UDP    "UDP"
#define CMS_LOWER_TRANS_TCP    "TCP"
#define CMS_LOWER_TRANS_UNKNOWN    "UNKNOWN"

typedef std::string TCmsTransCastMode;
#define CMS_TRANS_CAST_MODE_UNICAST   "unicast"
#define CMS_TRANS_CAST_MODE_MULTICAST "multicast"
#define CMS_TRANS_CAST_MODE_UNKNOWN   "unknown"

struct TCmsMediaTransProto
{
private:
	TCmsMediaTransport  transport;          //RTP
	string				profile;            //AVP
	TCmsLowerTransport  lowerTransport;     //TCP,UDP
	TCmsTransCastMode   castMode;           //TRANS_CAST_MODE_XXX: unicast | multicast.

public:
	TCmsMediaTransProto()
	{
		transport = CMS_MEDIA_TRANS_RTP;
		profile = "AVP";
		lowerTransport = CMS_LOWER_TRANS_UDP;
		castMode = CMS_TRANS_CAST_MODE_UNICAST;
	}

	TCmsMediaTransProto(const string& strTransport, const string& strLowerTransport, const string& strCastMode=CMS_TRANS_CAST_MODE_UNICAST)
	{
		transport = strTransport;
		lowerTransport = strLowerTransport;
		castMode = strCastMode;
	}

	const TCmsMediaTransport& GetTransport() const
	{
		return transport;
	}
	TCmsMediaTransport& GetTransport()
	{
		return transport;
	}
	void SetTransport(const TCmsMediaTransport& strValue)
	{
		transport = strValue;
	}

	const string& GetProfile() const
	{
		return profile;
	}
	string& GetProfile()
	{
		return profile;
	}
	void SetProfile(const string& strValue)
	{
		profile = strValue;
	}

	const TCmsLowerTransport& GetLowerTransport() const
	{
		return lowerTransport;
	}
	TCmsLowerTransport& GetLowerTransport()
	{
		return lowerTransport;
	}
	void SetLowerTransport(const TCmsLowerTransport& strValue)
	{
		lowerTransport = strValue;
	}

	const TCmsTransCastMode& GetCastMode() const
	{
		return castMode;
	}
	TCmsTransCastMode& GetCastMode()
	{
		return castMode;
	}
	void SetCastMode(const TCmsTransCastMode& strValue)
	{
		castMode = strValue;
	}

public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsMediaTransProto& obj)
	{
		output<<obj.transport<<"/"<<obj.profile<<"/"<<obj.lowerTransport<<";"<<obj.castMode;
		return output;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef std::string TCmsTransChannel;
#define CMS_MEDIA_TRANS_CHN_RTP       "RTP"
#define CMS_MEDIA_TRANS_CHN_RTCP      "RTCP"
#define CMS_MEDIA_TRANS_CHN_DATA      "DATA"
#define CMS_MEDIA_TRANS_CHN_UNKNOWN   "UNKNOWN"


typedef std::string TCmsMediaChannelKey;

typedef string TCmsIpAddr;

//
//struct TCmsNetAddr
//{
//private:
//	TCmsIpAddr ip;
//	int     port;
//
//public:
//	TCmsNetAddr() 
//	{
//		Clear();
//	}
//
//	TCmsNetAddr(const TCmsIpAddr& tNetIp, int dwNetPort) 
//	{
//		SetNetIp(tNetIp);
//		SetNetPort(dwNetPort);
//	}
//
//	void Clear()
//	{
//		ip.clear();
//		port = 0;
//	}
//
//	const TCmsIpAddr& GetNetIp() const 
//	{
//		return ip;
//	}
//
//	TCmsIpAddr& GetNetIp()
//	{
//		return ip;
//	}
//
//	void SetNetIp(const TCmsIpAddr& tNetIp) 
//	{
//		ip = tNetIp;
//	}
//
//	unsigned long GetNetIpv4() const;
//	void SetNetIpv4(unsigned long dwIp);
//
//	int GetNetPort() const 
//	{
//		return port;
//	}
//
//	void SetNetPort(int dwNetPort) 
//	{
//		port = dwNetPort;
//	}
//
//public:
//	friend std::ostream& operator<<(std::ostream& output, const TCmsNetAddr& obj)
//	{
//		output << obj.ip << " :"<<obj.port;
//		return output;
//	}
//	std::string ToString() const
//	{
//		std::ostringstream oss;
//		oss << *this;
//		return oss.str();
//	}
//
//	bool operator == (const TCmsNetAddr& obj) const
//	{
//		if (this->ip == obj.ip && this->port == obj.port)
//		{
//			return true;
//		}
//		return false;
//	}
//
//public:
//	const std::string ToXml()const;
//	void FromXml(const std::string& xml_string);
//	void ToXml(TiXmlDocument& xmlDoc)const;
//	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//	void FromXml(const TiXmlNode& xmlNode);
//};
typedef vector<TCmsNetAddr> TCmsNetAddrList;
static std::ostream& operator<<(std::ostream& output, const TCmsNetAddrList& obj)
{
	TCmsNetAddrList::const_iterator itr = obj.begin();
	for(; itr != obj.end(); itr++)
	{
		output<<*itr<<" ";
	}

	return output;
}

struct TCmsMediaTransChannel
{
private:
	TCmsTransChannel transChnType;     //RTP,RTCP,DATA
	TCmsMediaChannelKey mediaChnKey;   //udp传输时，Nat打洞用的key
	TCmsNetAddrList addrList;          //内网地址放前面，外网地址添加在后边
	TCmsNetAddr sendBindAddr;         //用于伪装发送地址

public:

	TCmsMediaTransChannel()
	{
	}

	void CopyBasicInfoFrom(const TCmsMediaTransChannel& var)
	{
		transChnType = var.transChnType;
		mediaChnKey = var.mediaChnKey;
	}

public:
	TCmsMediaTransChannel(const string& strTransChnType)
	{
		transChnType = strTransChnType;
	}

	TCmsMediaTransChannel(const string& strTransChnType, const TCmsNetAddrList tAddrList)
	{
		transChnType = strTransChnType;
		addrList = tAddrList;
	}

	TCmsMediaTransChannel(const string& strTransChnType, const TCmsMediaChannelKey& tMediaChnKey, const TCmsNetAddrList tAddrList)
	{
		transChnType = strTransChnType;
		mediaChnKey = tMediaChnKey;
		addrList = tAddrList;
	}

	const TCmsTransChannel& GetTransChnType() const
	{
		return transChnType;
	}
	TCmsTransChannel& GetTransChnType()
	{
		return transChnType;
	}
	void SetTransChnType(const TCmsTransChannel& strValue)
	{
		transChnType = strValue;
	}

	const TCmsMediaChannelKey& GetMediaChannelKey() const
	{
		return mediaChnKey;
	}
	TCmsMediaChannelKey& GetMediaChannelKey()
	{
		return mediaChnKey;
	}
	void SetMediaChannelKey(const TCmsMediaChannelKey& tValue)
	{
		mediaChnKey = tValue;
	}

	const TCmsNetAddrList& GetAddrList() const
	{
		return addrList;
	}
	TCmsNetAddrList& GetAddrList()
	{
		return addrList;
	}
	void SetAddrList(const TCmsNetAddrList& tValue)
	{
		addrList = tValue;
	}
	void AddAddrAtFront(const TCmsNetAddr& tValue)
	{
		TCmsNetAddrList::iterator itr = find(addrList.begin(), addrList.end(), tValue);
		if (itr != addrList.end())
		{
			return;
		}

		addrList.insert(addrList.begin(), tValue);
	}
	void AddAddrAtBack(const TCmsNetAddr& tValue)
	{
		TCmsNetAddrList::iterator itr = find(addrList.begin(), addrList.end(), tValue);
		if (itr != addrList.end())
		{
			return;
		}

		addrList.push_back(tValue);
	}
	void ResetAddr(const TCmsNetAddr& tValue)
	{
		addrList.clear();
		addrList.push_back(tValue);
	}

	void SetSendBindAddr(const TCmsNetAddr& cSendBindAddr)
	{
		sendBindAddr = cSendBindAddr;
	}
	TCmsNetAddr& GetSendBindAddr()
	{
		return sendBindAddr;
	}
	const TCmsNetAddr& GetSendBindAddr() const
	{
		return sendBindAddr;
	}

public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsMediaTransChannel& obj)
	{
		output<<std::endl;
		output<<"transChnType: "<<obj.transChnType<<std::endl;
		output<<"mediaChnKey: "<<obj.mediaChnKey<<std::endl;
		output<<"addrList: "<<obj.addrList<<std::endl;
		output<<"sendBindAddr: "<<obj.sendBindAddr<<std::endl;

		return output;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsMediaTransChannel> TCmsMediaTransChannelList;
static std::ostream& operator<<(std::ostream& output, const TCmsMediaTransChannelList& obj)
{
	TCmsMediaTransChannelList::const_iterator itr = obj.begin();
	for(; itr != obj.end(); itr++)
	{
		output<<*itr;
	}

	return output;
}

class TCmsMediaDesc
{
private:
	TCmsMidaType               mediaType;              //媒体类型：VIDEO，AUDIO，DATA
	TCmsMediaFormat            mediaFormat;            //媒体格式
	TCmsMediaTransProto        mediaTransProto;        //如：RTP/AVP/UDP;unicast
	TCmsMediaTransChannelList  mediaTransChnList;      //媒体传输通道。UDP传输时，通常是RTP与RTCP通道；数据传输或TCP传输时，单通道就可以

public:
	TCmsMediaDesc()
	{
	}

	void CopyBasicInfoFrom(const TCmsMediaDesc& var)
	{
		mediaType = var.mediaType;
		mediaTransProto = var.mediaTransProto;
		mediaTransChnList.clear();
		for(TCmsMediaTransChannelList::const_iterator itrMediaTransChn = var.mediaTransChnList.begin(); itrMediaTransChn!=var.mediaTransChnList.end(); itrMediaTransChn++)
		{
			TCmsMediaTransChannel tMediaTransChannel;
			tMediaTransChannel.CopyBasicInfoFrom(*itrMediaTransChn);
			mediaTransChnList.push_back(tMediaTransChannel);
		}
	}

	bool HasMediaDes() const
	{
		if(mediaType.empty() || mediaTransChnList.size()==0)
		{
			return false;
		}
		else
		{
			for(TCmsMediaTransChannelList::const_iterator itrMediaTransChn = mediaTransChnList.begin(); itrMediaTransChn!=mediaTransChnList.end(); itrMediaTransChn++)
			{
				if(itrMediaTransChn->GetAddrList().size() != 0)
				{
					return true;
				}
			}

			return false;
		}
	}

	TCmsMediaTransChannel* FindMediaTransChnByChnKey(const TCmsMediaChannelKey& tMediaChnKey)
	{
		TCmsMediaTransChannelList::iterator itrTransChn = mediaTransChnList.begin();
		for(; itrTransChn!=mediaTransChnList.end(); itrTransChn++)
		{
			if(itrTransChn->GetMediaChannelKey() == tMediaChnKey)
			{
				return &(*itrTransChn);
			}
		}

		return NULL;
	}

public:
	const TCmsMidaType& GetMediaType() const
	{
		return mediaType;
	}
	TCmsMidaType& GetMediaType()
	{
		return mediaType;
	}
	void SetMediaType(const TCmsMidaType& strValue)
	{
		mediaType = strValue;
	}

	//媒体格式接口
	const TCmsMediaFormat& GetMediaFormat() const
	{
		return mediaFormat;
	}
	TCmsMediaFormat& GetMediaFormat()
	{
		return mediaFormat;
	}
	void SetMediaFormat(const TCmsMediaFormat& tValue)
	{
		mediaFormat = tValue;
	}

	//媒体传输协议接口
	const TCmsMediaTransProto& GetMediaTransProto() const
	{
		return mediaTransProto;
	}
	TCmsMediaTransProto& GetMediaTransProto()
	{
		return mediaTransProto;
	}
	void SetMediaTransProto(const TCmsMediaTransProto& tValue)
	{
		mediaTransProto = tValue;
	}

	//媒体传输通道接口
	const TCmsMediaTransChannelList& GetMediaTransChnList() const
	{
		return mediaTransChnList;
	}
	TCmsMediaTransChannelList& GetMediaTransChnList()
	{
		return mediaTransChnList;
	}
	void SetMediaTransChnList(const TCmsMediaTransChannelList& tValue)
	{
		mediaTransChnList = tValue;
	}

	void AddMediaTransChn(const TCmsMediaTransChannel& tValue)
	{
		mediaTransChnList.push_back(tValue);
	}

	int GetMediaTransChnNum() const
	{
		return mediaTransChnList.size();
	}

	//const TMediaTransChannel& GetMediaTransChn(int nIdx) const
	//{
	//	return mediaTransChnList[nIdx];
	//}
	//void AddMediaTransChn(int nIdx, const TMediaTransChannel& tValue)
	//{
	//	mediaTransChnList[nIdx] = tValue;
	//}

public:
	friend std::ostream& operator<<(std::ostream& output, const TCmsMediaDesc& obj)
	{
		output<<std::endl;
		output<<"mediaType: "<<obj.mediaType<<std::endl;
		output<<"mediaFormat: "<<obj.mediaFormat<<std::endl;
		output<<"mediaTransProto: "<<obj.mediaTransProto<<std::endl;
		output<<"mediaTransChnList: "<<obj.mediaTransChnList<<std::endl;

		return output;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef std::vector<TCmsMediaDesc> TCmsMediaDescList;
static std::ostream& operator<<(std::ostream& output, const TCmsMediaDescList& obj)
{
	TCmsMediaDescList::const_iterator itr = obj.begin();
	for(; itr != obj.end(); itr++)
	{
		output<<*itr;
	}

	return output;
}

typedef std::map<string, string> TCmsExtInfo;
static const string CMS_SDP_EXT_INFO_PLAYBACK_SSN = "PlaybackSsn";			//放像任务会话Uri; 放像INVITE_RSP时，由最终被请求的设备端生成，后续Vcr控制和进度查询时要用。值：string类型
static const string CMS_SDP_EXT_INFO_PLAYBACK_START_TIME = "PlaybackStartTime";	//平台录像为支持倒放时，放像INVITE_RSP返回给CU的起始放像时间。值：int类型
static const string CMS_SDP_EXT_INFO_FILE_PATH = "FilePath";				//录像下载INVITE_RSP时，由录像模块(NRU)生成，客户端RPCTRL要用。值：string类型
static const string CMS_SDP_EXT_INFO_FILE_NAME = "FileName";				//录像下载INVITE_RSP时，由录像模块(NRU)生成，客户端RPCTRL要用。值：string类型
static const string CMS_SDP_EXT_INFO_STREAM_TAG = "StreamTag";	            //码流标签(格式"厂商:MEDIA_STREAM_TYPE",如kedacom:RTP_ES)

static const string CMS_SDP_EXT_INFO_DIRECT_TRANSFER = "DirectTransfer";	//是否优先直传, 值：TRUE_STR/FALSE_STR
static const string CMS_SDP_EXT_INFO_PLAT_DIRECT_TRANSFER = "PlatDirectTransfer";	//是否优先平台直传, 值：TRUE_STR/FALSE_STR

static const string CMS_SDP_EXT_INFO_OFFLINE_SWITCH = "OfflineSwitch";		//是否支持离线交换; INVITE_REQ给VTDU时携带; 值：TRUE_STR/FALSE_STR
static const string CMS_SDP_EXT_INFO_NETSEG_NO = "NetSegNO";	            //网段号; INVITE_REQ给VTDU时携带; 值：int类型
static const string CMS_SDP_EXT_INFO_IS_NEED_PUNCH = "NeedPunch";	        //是否需要Punch
static const string CMS_SDP_EXT_INFO_IS_NEED_GBPUNCH = "NeedGBPunch";	    //是否需要GBPunch
static const string CMS_SDP_EXT_INFO_GBPUNCH_GBCHNID = "GBPunchChnId";	    //GBPunch的通道ID
static const string CMS_SDP_EXT_INFO_USE_DOMAIN_SOCKET = "DomainSocket";	//是否使用域套接字，值：TRUE_STR/FALSE_STR

static const string CMS_SDP_EXT_INFO_DROP_RTP_EXTEND = "DropRtpExtend";     //是否去Rtp扩展头，值：TRUE_STR/FALSE_STR
static const string CMS_SDP_EXT_INFO_USE_AES256_ENCRYPTION = "AES256Encryption";//是否使用AES256加密，值：TRUE_STR/FALSE_STR
static const string CMS_SDP_EXT_INFO_MRTC_ENABLE = "MrtcEnable";            //是否启用MRTC，值：TRUE_STR/FALSE_STR
static const string CMS_SDP_EXT_INFO_ADDR_CHANGE_SIDE = "AddrChangeSide";   //修改地址一侧，值："SRC"/"DST"
const string CMS_ADDR_CHANGE_SIDE_SRC = "SRC";
const string CMS_ADDR_CHANGE_SIDE_DST = "DST";

static const string CMS_SDP_EXT_INFO_IS_NAT_PACKET_MODE = "NatPacketMode";  //是否NAT探测包模式
static const string CMS_SDP_EXT_INFO_INTERFACE_IP = "InterfaceIp";          //接口IP地址
static const string CMS_SDP_EXT_INFO_TCPSTREAM = "TcpStream";               //tcp码流，国标透传扩展信息，值：STREAM_MODE_TCPACTIVE/STREAM_MODE_TCPPASSIVE

static const string CMS_SDP_EXT_INFO_TIMESTAMP_TYPE = "TimestampType";          //时间戳类型，值为TIMESTAMP_TYPE_STREAM或者TIMESTAMP_TYPE_PROGRESS
#define CMS_TIMESTAMP_TYPE_STREAM   ("Stream")
#define CMS_TIMESTAMP_TYPE_PROGRESS   ("Progress")

static const string CMS_SDP_EXT_INFO_SECURE_TYPE = "SecureType";            //安全类型， 值为 SDP_EXT_INFO_SECURE/UNSECURE, 若为检测出该键 则视为不安全
static const string CMS_SDP_EXT_INFO_VKEK_LIST   = "VkekList";              // vkek列表， 值

static const string CMS_SDP_EXT_INFO_TRANSFER_RATE = "Rate";                  //传输倍速，值：int类型
static const string CMS_SDP_EXT_INFO_FILE_SIZE = "FileSize";                  //文件大小，值：double类型（字节）

//{{{{{{{{{{{{{{{ modify by Royan 20140605 国标流保活
static const string CMS_SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO = "GBStreamKAInfo"; //国标流保活信息
#define CMS_STREAM_KA_SR        ("SR")
#define CMS_STREAM_KA_RR        ("RR")
#define CMS_STREAM_KA_SR_RR     ("SRRR")
//}}}}}}}}}}}}}}} modify by Royan 20140605 国标流保活
//{{{{{{{{{{{{{{{ add by Royan 20140903 SSRC
static const string CMS_SDP_EXT_INFO_SSRC = "SSRC";	                        //SSRC
//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC
static const string CMS_SDP_EXT_INFO_GB_REC_LOCATION = "GBRecLocation";     //指定国标录像位置（值：国标域ID或者国标前端ID）

struct TCmsSecurityModuleVkek
{
	std::string vkek;                
	std::string version;                
public:
	void SetVkek(const std::string& strVkek)
	{
		vkek = strVkek;
	}
	std::string& GetVkek()
	{
		return vkek;
	}
	const std::string& GetVkek() const
	{
		return vkek;
	}

	void SetVersion(const std::string& strVersion)
	{
		version = strVersion;
	}
	std::string& GetVersion()
	{
		return version;
	}
	const std::string& GetVersion() const
	{
		return version;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsVkekList
{
public:
	TCmsVkekList()
	{
		Clear();
	}
	void Clear()
	{
		vkeks.clear();
	}
public:
	std::vector<TCmsSecurityModuleVkek> vkeks;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsSdp
{
private:
	TCmsChannel            owner;              //sdp的创建者，即mediaDescList是属于谁的
	TCmsSdpSessionName     sessionName;        //操作类型
	TCmsSdpUri             uri;                //放像、下载INVITE_REQ时必须填；浏览时不填
	TCmsTimeRange		   timeRange;          //放像、下载INVITE_REQ时必须填；浏览时可以不填
	TCmsMediaDescList      mediaDescList;
	TCmsExtInfo            extInfo;

public:
	TCmsSdp()
	{
		Clear();
	}

	void Clear()
	{
		owner.Clear();
		sessionName = STRING_NULL;
		uri.Clear();
		timeRange.Clear();
		mediaDescList.clear();
		extInfo.clear();
	}

	void CreateEmptyMediaList(const TCmsSdpSessionName& tSessionName = TCmsSdpSessionName())
	{
		if(!tSessionName.empty())
		{
			sessionName = tSessionName;
		}

		this->mediaDescList.clear();

		if(SDP_SESSION_AUDIOCALL == sessionName)
		{            
			TCmsMediaDesc tAudioMediaDesc;
			tAudioMediaDesc.SetMediaType(CMS_MEDIA_TYPE_AUDIO);
			//tAudioMediaDesc.SetMediaFormat();
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTP));
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTCP));
			this->AddMediaDesc(tAudioMediaDesc);
		}
		//{{{{{{{{{{{{{{{ add by Royan 20140321
		else if(SDP_SESSION_DOWNLOAD == sessionName)
		{            
			TCmsMediaDesc tAudioMediaDesc;
			tAudioMediaDesc.SetMediaType(CMS_MEDIA_TYPE_DATA);
			//tAudioMediaDesc.SetMediaFormat();
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTP));
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTCP));
			this->AddMediaDesc(tAudioMediaDesc);
		}
		//}}}}}}}}}}}}}}} add by Royan 20140321 
		else
		{
			TCmsMediaDesc tVideoMediaDesc;
			tVideoMediaDesc.SetMediaType(CMS_MEDIA_TYPE_VIDEO);
			//tVideoMediaDesc.SetMediaFormat();
			tVideoMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTP));
			tVideoMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTCP));
			this->AddMediaDesc(tVideoMediaDesc);

			TCmsMediaDesc tAudioMediaDesc;
			tAudioMediaDesc.SetMediaType(CMS_MEDIA_TYPE_AUDIO);
			//tAudioMediaDesc.SetMediaFormat();
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTP));
			tAudioMediaDesc.AddMediaTransChn(TCmsMediaTransChannel(CMS_MEDIA_TRANS_CHN_RTCP));
			this->AddMediaDesc(tAudioMediaDesc);
		}
	}

	void CopySdpHeaderFrom(const TCmsSdp& var)
	{
		sessionName = var.sessionName;
		uri = var.uri;
		timeRange = var.timeRange;
	}

	void CopyBasicInfoFrom(const TCmsSdp& var)
	{
		CopySdpHeaderFrom(var);

		mediaDescList.clear();  //似乎可以不用清空原先的信息？？
		for(TCmsMediaDescList::const_iterator itrMediaDesc = var.mediaDescList.begin(); itrMediaDesc!=var.mediaDescList.end(); itrMediaDesc++)
		{
			TCmsMediaDesc tMediaDesc;
			tMediaDesc.CopyBasicInfoFrom(*itrMediaDesc);
			mediaDescList.push_back(tMediaDesc);
		}
	}

	void CopyMediaChnKeyFrom(const TCmsSdp& tSdpFrom)
	{
		for(TCmsMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			//遍历tMediaTransChnList
			TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

			for(TCmsMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				TCmsMediaTransChannel  tMediaTransChnFound;
				if (tSdpFrom.FindMediaTransChn(itrMediaDesc->GetMediaType(), itrTransChn->GetTransChnType(), /*OUT*/tMediaTransChnFound))
				{
					itrTransChn->SetMediaChannelKey(tMediaTransChnFound.GetMediaChannelKey());
				}
				else
				{
					//在源对象找不到相应的Chn则把Key置空
					itrTransChn->SetMediaChannelKey(TCmsMediaChannelKey());
				}
			}
		}
	}

	void CopyExtInfoFrom(const TCmsSdp& tSdpFrom)
	{
		const TCmsExtInfo& tExtInfoFrom = tSdpFrom.GetExtInfo();

		for (TCmsExtInfo::const_iterator itr = tExtInfoFrom.begin(); itr!= tExtInfoFrom.end(); itr++)
		{
			if (extInfo.end() == extInfo.find(itr->first))
			{
				extInfo.insert(*itr);
			}
		}
	}

	friend std::ostream& operator<<(std::ostream& output, const TCmsSdp& obj)
	{
		output<<std::endl;
		output<<"owner: "<<obj.owner.ToString()<<std::endl;
		output<<"sessionName: "<<obj.sessionName<<std::endl;
		output<<"uri: "<<obj.uri<<std::endl;
		output<<"timeRange: "<<obj.timeRange.GetStartTime()<<","<<obj.timeRange.GetEndTime()<<std::endl;
		output<<"mediaTransChnList: "<<obj.mediaDescList<<std::endl;
		for (TCmsExtInfo::const_iterator itr = obj.extInfo.begin(); itr != obj.extInfo.end(); itr++)
		{
			output<<itr->first<<"="<<itr->second<<std::endl;
		}

		return output;
	}
/*
	void PrintData(bool bVerbose) const
	{
		std::ostringstream oss;

		//OspPrintf(TRUE, FALSE, "--------------------------------------------------\n");
		oss.str("");
		oss<<*this;
		OspPrintf(TRUE, FALSE, "Sdp:\n{{%s}}\n", oss.str().c_str());
		OspPrintf(TRUE, FALSE, "--------------------------------------------------\n");
	} */

public:
	bool HasMediaDes() const
	{
		if(mediaDescList.size() == 0)
		{
			return false;
		}
		else
		{
			for(TCmsMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc!=mediaDescList.end(); itrMediaDesc++)
			{
				if(itrMediaDesc->HasMediaDes())
				{
					return true;
				}
			}

			return false;
		}
	}

	void AddMediaDesc(const TCmsMediaDesc& tMediaDesc)
	{
		mediaDescList.push_back(tMediaDesc);
	}

	bool FindMediaDesc(const string& strMeidaType, /*OUT*/TCmsMediaDesc*& ptMediaDesc)
	{
		//遍历TMediaDescList
		for(TCmsMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			if(itrMediaDesc->GetMediaType() == strMeidaType)
			{
				ptMediaDesc = &(*itrMediaDesc);
				return true;
			}
		}

		return false;
	}

	bool FindMediaTransChn(const string& strMeidaType, const string& strTransChnType, /*OUT*/TCmsMediaTransChannel& tMediaTransChannel) const
	{
		for(TCmsMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			if (itrMediaDesc->GetMediaType() == strMeidaType)
			{
				//遍历tMediaTransChnList
				const TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

				for(TCmsMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (itrTransChn->GetTransChnType() == strTransChnType)
					{
						tMediaTransChannel = *itrTransChn;
						return true;
					}
				}
			}
		}

		return false;
	}

	bool FindMediaTransChn(const string& strMeidaType, const string& strTransChnType, /*OUT*/TCmsMediaTransChannel*& ptMediaTransChannel)
	{
		for(TCmsMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			if (itrMediaDesc->GetMediaType() == strMeidaType)
			{
				//遍历tMediaTransChnList
				TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

				for(TCmsMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (itrTransChn->GetTransChnType() == strTransChnType)
					{
						ptMediaTransChannel = &(*itrTransChn);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool FindMediaTransChnByChnKey(/*IN*/const TCmsMediaChannelKey& tMediaChnKey, /*OUT*/TCmsMidaType& tMediaType, /*OUT*/TCmsMediaTransChannel*& ptMediaTransChannel)
	{
		//遍历TMediaDescList
		for(TCmsMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			TCmsMediaTransChannel* ptChannel = itrMediaDesc->FindMediaTransChnByChnKey(tMediaChnKey);
			if(NULL != ptChannel)
			{
				tMediaType = itrMediaDesc->GetMediaType();
				ptMediaTransChannel = ptChannel;

				return true;
			}
		}

		return false;
	}

	int GetTransChnNum() const
	{
		//通常情况下，只有Video/Audio的RTP/RTCP通道，总共是4路传输通道
		unsigned long dwTransChnNum = 0;
		TCmsMediaDescList::const_iterator itr = mediaDescList.begin();

		for(; itr != mediaDescList.end(); itr++)
		{
			dwTransChnNum += itr->GetMediaTransChnNum();
		}

		return dwTransChnNum;
	}

	bool IsAllMediaChnKeyEmpty() const
	{
		for(TCmsMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			//遍历tMediaTransChnList
			const TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

			for(TCmsMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (!itrTransChn->GetMediaChannelKey().empty())
				{
					return false;
				}
			}
		}

		return true;
	}

	bool IsExistIP(const TCmsIpAddr& tIpAddr) const
	{
		//遍历TMediaDescList
		for(TCmsMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			//遍历TMediaTransChannelList
			const TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TCmsMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				//遍历TNetAddrList
				const TCmsNetAddrList& tNetAddrList = itrTransChn->GetAddrList();
				for (TCmsNetAddrList::const_iterator itrAddr = tNetAddrList.begin(); itrAddr != tNetAddrList.end(); itrAddr++)
				{
					if (tIpAddr == itrAddr->GetNetIp())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	void TrimAddrExceptFor(const TCmsIpAddr& tIpAddr)
	{
		//遍历TMediaDescList
		for(TCmsMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
		{
			//遍历TMediaTransChannelList
			TCmsMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TCmsMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				//遍历TNetAddrList
				TCmsNetAddrList& tNetAddrList = itrTransChn->GetAddrList();
				for (TCmsNetAddrList::iterator itrAddr = tNetAddrList.begin(); itrAddr != tNetAddrList.end(); )
				{
					if (tIpAddr != itrAddr->GetNetIp())
					{
						itrAddr = tNetAddrList.erase(itrAddr);
					}
					else
					{
						itrAddr++;
					}
				}
			}
		}
	}

public:
	void SetOwner(const TCmsChannel& tOwner)
	{
		owner = tOwner;
	}
	TCmsChannel& GetOwner()
	{
		return owner;
	}
	const TCmsChannel& GetOwner() const
	{
		return owner;
	}

	void SetSessionName(const TCmsSdpSessionName& strSessionName)
	{
		sessionName = strSessionName;
	}
	TCmsSdpSessionName& GetSessionName()
	{
		return sessionName;
	}
	const TCmsSdpSessionName& GetSessionName() const
	{
		return sessionName;
	}

	void SetUri(const TCmsSdpUri& tUri)
	{
		uri = tUri;
	}
	TCmsSdpUri& GetUri()
	{
		return uri;
	}
	const TCmsSdpUri& GetUri() const
	{
		return uri;
	}

	void SetTimeRange(const TCmsTimeRange& cTimeRange)
	{
		timeRange = cTimeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}

	void SetMediaDescList(const TCmsMediaDescList& cMediaDescList)
	{
		mediaDescList = cMediaDescList;
	}
	TCmsMediaDescList& GetMediaDescList()
	{
		return mediaDescList;
	}
	const TCmsMediaDescList& GetMediaDescList() const
	{
		return mediaDescList;
	}

	//tExtInf
	void SetExtInfo(const TCmsExtInfo& tExtInf)
	{
		extInfo = tExtInf;
	}
	TCmsExtInfo& GetExtInfo()
	{
		return extInfo;
	}
	const TCmsExtInfo& GetExtInfo() const
	{
		return extInfo;
	}

	void SetExtInfo(const string& strKey, const string& strValue)
	{
		extInfo[strKey] = strValue;
	}
	void SetExtInfo(const string& strKey, int nValue)
	{
		char buf[256]={0};
		sprintf( buf, "%d", nValue );
		extInfo[strKey] = buf;
	}
	void SetExtInfo(const string& strKey, double lfValue)
	{
		char buf[256]={0};
		sprintf( buf, "%lf", lfValue );
		extInfo[strKey] = buf;
	}

	void SetExtInfo(const string& strKey, bool bValue)
	{
		extInfo[strKey] = bValue? "true" : "false";
	}

	bool GetExtInfo(const string& strKey, string& strValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			strValue = itr->second;

			return true;
		}

		return false;
	}
	bool GetExtInfo(const string& strKey, int& nValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			nValue = atoi(itr->second.c_str());

			return true;
		}

		return false;
	}
	bool GetExtInfo(const string& strKey, double& lfValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			lfValue = atof(itr->second.c_str());

			return true;
		}

		return false;
	}

	bool GetExtInfo(const string& strKey, bool& bValue) const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			bValue = (itr->second == "true");

			return true;
		}

		return false;
	}

	//TCmsExtInfo细化接口
	void SetGBRecLocation(const string& var)
	{
		extInfo[CMS_SDP_EXT_INFO_GB_REC_LOCATION] = var;
	}
	string GetGBRecLocation() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_GB_REC_LOCATION);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}
	//{{{{{{{{{{{{{{{ modify by Royan 20140606 国标流保活
	void SetGBStreamKAInfo(const string& var)
	{
		extInfo[CMS_SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO] = var;
	}
	string GetGBStreamKAInfo() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}
	//}}}}}}}}}}}}}}} modify by Royan 20140606 国标流保活
	//{{{{{{{{{{{{{{{ add by Royan 20140903 SSRC
	void SetSSRC(int nVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_SSRC, nVar);
	}
	int GetSSRC() const
	{
		int nVar = 0;

		GetExtInfo(CMS_SDP_EXT_INFO_SSRC, nVar);

		return nVar;
	}
	//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC

	void SetPlaybackSessUri(const string& strPlaybackSessUri)
	{
		extInfo[CMS_SDP_EXT_INFO_PLAYBACK_SSN] = strPlaybackSessUri;
	}
	string GetPlaybackSessUri() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_PLAYBACK_SSN);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetPlaybackStartTime(int nVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_PLAYBACK_START_TIME, nVar);
	}
	int GetPlaybackStartTime() const
	{
		int nVar = 0;

		GetExtInfo(CMS_SDP_EXT_INFO_PLAYBACK_START_TIME, nVar);

		return nVar;
	}

	void SetDLFimePath(const string& strVar)
	{
		extInfo[CMS_SDP_EXT_INFO_FILE_PATH] = strVar;
	}
	string GetDLFimePath() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_FILE_PATH);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetDLFimeName(const string& strVar)
	{
		extInfo[CMS_SDP_EXT_INFO_FILE_NAME] = strVar;
	}
	string GetDLFimeName() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_FILE_NAME);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetOfflineSwitch(bool bEnable)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_OFFLINE_SWITCH, bEnable);
	}
	bool GetOfflineSwitch() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_OFFLINE_SWITCH, bVar);

		return bVar;
	}

	void SetDirectTransfer(bool bEnable)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_DIRECT_TRANSFER, bEnable);
	}
	bool GetDirectTransfer() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_DIRECT_TRANSFER, bVar);

		return bVar;
	}

	void SetPlatDirectTransfer(bool bEnable)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_PLAT_DIRECT_TRANSFER, bEnable);
	}
	bool GetPlatDirectTransfer() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_PLAT_DIRECT_TRANSFER, bVar);

		return bVar;
	}

	void SetNetSegNo(int nVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_NETSEG_NO, nVar);
	}
	int GetNetSegNo() const
	{
		int nVar = 0;

		GetExtInfo(CMS_SDP_EXT_INFO_NETSEG_NO, nVar);

		return nVar;
	}

	void SetIsNeedPunch(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_IS_NEED_PUNCH, bVar);
	}
	bool GetIsNeedPunch() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_IS_NEED_PUNCH, bVar);

		return bVar;
	}

	void SetIsNeedGBPunch(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_IS_NEED_GBPUNCH, bVar);
	}
	bool GetIsNeedGBPunch() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_IS_NEED_GBPUNCH, bVar);

		return bVar;
	}

	void SetGBPunchChnID(const string& strVar)
	{
		extInfo[CMS_SDP_EXT_INFO_GBPUNCH_GBCHNID] = strVar;
	}
	string GetGBPunchChnID() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_GBPUNCH_GBCHNID);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetDomainSocket(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_USE_DOMAIN_SOCKET, bVar);
	}
	bool GetDomainSocket() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_USE_DOMAIN_SOCKET, bVar);

		return bVar;
	}

	void SetDropRtpExtend(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);
	}
	bool GetDropRtpExtend() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);

		return bVar;
	}

	void SetAES256Encryption(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_USE_AES256_ENCRYPTION, bVar);
	}
	bool GetAES256Encryption() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_USE_AES256_ENCRYPTION, bVar);

		return bVar;
	}

	void SetMrtcEnable(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_MRTC_ENABLE, bVar);
	}
	bool GetMrtcEnable() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_MRTC_ENABLE, bVar);

		return bVar;
	}

	void SetIsNatPacketMode(bool bVar)
	{
		SetExtInfo(CMS_SDP_EXT_INFO_IS_NAT_PACKET_MODE, bVar);
	}
	bool GetIsNatPacketMode() const
	{
		bool bVar = false;

		GetExtInfo(CMS_SDP_EXT_INFO_IS_NAT_PACKET_MODE, bVar);

		return bVar;
	}

	void SetInterfaceIp(const string& var)
	{
		extInfo[CMS_SDP_EXT_INFO_INTERFACE_IP] = var;
	}
	string GetInterfaceIp() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_INTERFACE_IP);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetTcpStream(const string& var)
	{
		extInfo[CMS_SDP_EXT_INFO_TCPSTREAM]=var;
	}
	string GetTcpStream()
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_TCPSTREAM);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetStreamTag(const string& strVar)
	{
		extInfo[CMS_SDP_EXT_INFO_STREAM_TAG] = strVar;
	}
	string GetStreamTag() const
	{
		TCmsExtInfo::const_iterator itr = extInfo.find(CMS_SDP_EXT_INFO_STREAM_TAG);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetStreamTag(const TCmsStreamTag& tStreamTag)
	{
		SetStreamTag(tStreamTag.ToString());
	}
	bool GetStreamTag(TCmsStreamTag& tStreamTag) const
	{
		return tStreamTag.Parse(GetStreamTag());
	}

    void SetTransferRate(int nVar)
    {
        SetExtInfo(CMS_SDP_EXT_INFO_TRANSFER_RATE, nVar);
    }

    int GetTransferRate() const
    {
        int nVar = 1;

        GetExtInfo(CMS_SDP_EXT_INFO_TRANSFER_RATE, nVar);

        return nVar;
    }

    void SetFileSize(double lfVar)
    {
        SetExtInfo(CMS_SDP_EXT_INFO_FILE_SIZE, lfVar);
    }

    double GetFileSize() const
    {
        double lfVar = 0.00;

        GetExtInfo(CMS_SDP_EXT_INFO_FILE_SIZE, lfVar);

        return lfVar;
    }

	void SetVkekList(const TCmsVkekList &vkeks )
	{
		SetExtInfo( CMS_SDP_EXT_INFO_VKEK_LIST, vkeks.ToXml() );
	}

	bool  GetVkekList(TCmsVkekList &vkeks) const
	{
		vkeks.Clear();
		string strVkeks;
		bool result = GetExtInfo( CMS_SDP_EXT_INFO_VKEK_LIST, strVkeks );
		if( !result )
		{
			return false;
		}
		vkeks.FromXml( strVkeks );
		return true;
	}

	void SetSecure( bool isSecure )
	{
		SetExtInfo( CMS_SDP_EXT_INFO_SECURE_TYPE, isSecure );
	}

	bool GetSecure() const
	{
		bool result = false;
		bool isSecure = false;
		result = GetExtInfo( CMS_SDP_EXT_INFO_SECURE_TYPE, isSecure );
		return result && isSecure;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};


//////////////////////////////////////////////////////////////////////////

typedef string TCmsCronTime;                   //用CCronTime类解析
typedef vector<TCmsCronTime> TCmsCronTimeList;


#define CMS_SDK_ENUM_STR(x)		static const char* x = #x

typedef string TCmsRecMode;
static const char* CMS_RCS_RECMODE_ALL = "RCS_RECMODE_ALL";                       //既录视频又录音频
static const char* CMS_RCS_RECMODE_ONLYVIDEO = "RCS_RECMODE_ONLYVIDEO";           //只录视频
static const char* CMS_RCS_RECMODE_ONLYKEYFRAME = "RCS_RECMODE_ONLYKEYFRAME";     //只录关键帧
static const char* CMS_RCS_RECMODE_UNKNOWN = "RCS_RECMODE_UNKNOWN";               //未知模式
//CMS_SDK_ENUM_STR(CMS_RCS_RECMODE_ALL);                   //既录视频又录音频
//CMS_SDK_ENUM_STR(CMS_RCS_RECMODE_ONLYVIDEO);             //只录视频
//CMS_SDK_ENUM_STR(CMS_RCS_RECMODE_ONLYKEYFRAME);          //只录关键帧
//CMS_SDK_ENUM_STR(CMS_RCS_RECMODE_UNKNOWN);               //未知模式

//前端录像相关
struct TCmsPuRecToken
{
public:
	TCmsPuRecToken()
	{
		Clear();
	}
	void Clear()
	{
		recType = STRING_NULL;
		fileId = STRING_NULL;
		filePath = STRING_NULL;
		fileName = STRING_NULL;
	}

private:
	string             recType;                    //录像类型
	string             fileId;                     //录像文件唯一标识
	string             filePath;                   //录像文件路径
	string             fileName;                   //录像文件名称
public:
	void SetRecType(const string& strRecType)
	{
		recType = strRecType;
	}
	string& GetRecType()
	{
		return recType;
	}
	const string& GetRecType() const
	{
		return recType;
	}

	void SetFileId(const string& strFileId)
	{
		fileId = strFileId;
	}
	string& GetFileId()
	{
		return fileId;
	}
	const string& GetFileId() const
	{
		return fileId;
	}

	void SetFilePath(const string& strFilePath)
	{
		filePath = strFilePath;
	}
	string& GetFilePath()
	{
		return filePath;
	}
	const string& GetFilePath() const
	{
		return filePath;
	}

	void SetFileName(const string& strFileName)
	{
		fileName = strFileName;
	}
	string& GetFileName()
	{
		return fileName;
	}
	const string& GetFileName() const
	{
		return fileName;
	}

	//string类型token转换
#define   STR_TOKEN_SEP_FLAG   " "

	string ToString() const
	{
		string strToken;

		//前端只需要录像类型和文件ID
		strToken += recType;
		strToken += STR_TOKEN_SEP_FLAG;
		strToken += fileId;
		strToken += STR_TOKEN_SEP_FLAG;
		strToken += filePath;
		strToken += STR_TOKEN_SEP_FLAG;
		strToken += fileName;

		return strToken;
	}

	void FromString(const string& strToken)
	{
		string strRemain = strToken;
		string strSepFlag = STR_TOKEN_SEP_FLAG;
		string::size_type PosSepFlag = 0;

		vector<string> cFieldList;
		while((PosSepFlag = strRemain.find(strSepFlag)) != string::npos)
		{
			string strField;
			strField.assign(strRemain, 0, PosSepFlag);
			cFieldList.push_back(strField);
			strRemain.assign(strRemain, PosSepFlag+1, strRemain.size()-PosSepFlag-1);         
		}

		if (!strRemain.empty())
		{
			cFieldList.push_back(strRemain);
		}

		if (cFieldList.size() >= 4)
		{
			recType = cFieldList[0];
			fileId = cFieldList[1];
			filePath = cFieldList[2];
			fileName = cFieldList[3];
		}
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

typedef string TCmsVcrCmd;
#define CMS_VCRCMD_START    "START"         //开始放像
#define CMS_VCRCMD_STOP     "STOP"          //停止放像
#define CMS_VCRCMD_PAUSE    "PAUSE"         //暂停放像
#define CMS_VCRCMD_RESUME   "RESUME"        //恢复放像
#define CMS_VCRCMD_SEEK     "SEEK"          //时间定位
#define CMS_VCRCMD_FASTPLAY "FASTPLAY"      //快放
#define CMS_VCRCMD_SLOWPLAY "SLOWPLAY"      //慢放
#define CMS_VCRCMD_ONEFRAME "ONEFRAME"      //单帧播放
#define CMS_VCRCMD_KEYFRAME "KEYFRAME"      //只放关键帧
#define CMS_VCRCMD_KEYSEEK  "KEYSEEK"       //时间定位(从前一个关键帧开始)
#define CMS_VCRCMD_REVERSEPLAY "REVERSEPLAY"   //倒放
#define CMS_VCRCMD_FORWARDPLAY "FORWARDPLAY"   //正放
#define CMS_VCRCMD_UNKNOWN  "UNKNOWN"


typedef string TCmsRecMediaType;
#define CMS_ALL_REC_MEDIA               "all"           // 所有录像
#define CMS_PU_REC_MEDIA                "pu"            // 前端录像
#define CMS_PLAT_REC_MEDIA              "plat"          // 平台录像

struct TCmsPuRecInfo
{
public:
	TCmsPuRecInfo()
	{
		Clear();
	}
	void Clear()
	{
		recToken.Clear();
		timeRange.Clear();
	}

private:
	TCmsPuRecToken           recToken;						  //录像Token
	TCmsTimeRange            timeRange;						  //时间段
public:
	void SetRecToken(const TCmsPuRecToken& cRecToken)
	{
		recToken = cRecToken;
	}
	TCmsPuRecToken& GetRecToken()
	{
		return recToken;
	}
	const TCmsPuRecToken& GetRecToken() const
	{
		return recToken;
	}

	void SetTimeRange(const TCmsTimeRange& cTimeRange)
	{
		timeRange = cTimeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef   vector<TCmsPuRecInfo>    TCmsPuRecInfoList;


//订阅相关
typedef std::string TCmsSsType;
typedef set<TCmsSsType> TCmsSsTypes;
#define  CMS_PU_ON_OFF_STATUS						"PU_ON_OFF"
#define  CMS_PU_CONFIG_STATUS						"PU_CONFIG"
#define  CMS_PU_SERVICE_STATUS						"PU_SERVICE"
#define  CMS_PU_ALARM_STATUS						"PU_ALARM"
#define  CMS_PU_GPS_STATUS							"PU_GPS"
#define  CMS_PU_TRANSDATA_STATUS					"PU_TRANSDATA"
#define  CMS_SS_TYPE_PU_PLATFORM_RECORD_STATUS		"PU_PLATFORM_RECORD"		// rcs录像状态订阅
#define  CMS_SS_TYPE_DISK_PART_ALARM				"DISK_PART_ALARM"			// 磁盘分区告警

typedef string TCmsSsOpType;
#define CMS_SS_OP_TYPE_ADD   "ADD"
#define CMS_SS_OP_TYPE_DEL   "DEL"

//pu的入/退网，起/停用，上/下线，起/停线
struct TCmsPuStatus 
{
private:
    bool isInTop;           //是否入网
    bool isUsed;            //是否启用
    bool isOnline;          //是否在线

public:
    TCmsPuStatus()
    {
        Clear();
    }

    ~TCmsPuStatus()
    {
        Clear();
    }

    void Clear()
    {
        isInTop = true; 
        isUsed = true;  
        isOnline = false;
    }

public:

    bool GetIsInTop() const
    {
        return isInTop;
    }
    void SetIsInTop(bool bIsInTop)
    {
        isInTop = bIsInTop;
    }

    bool GetIsUsed() const
    {
        return isUsed;
    }
    void SetIsUsed(bool bIsUsed)
    {
        isUsed = bIsUsed;
    }

    bool GetIsOnline() const
    {
        return isOnline;
    }
    void SetIsOnline(bool bIsOnline)
    {
        isOnline = bIsOnline;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//pu的业务状态

struct TCmsPuServiceStatus
{
private:
    map<int,TCmsChannel>    audioCallSrc;    //音频呼叫的源（通常是CU）
    map<int,bool>        isPuPreRec;      //是否在前端预录
    map<int,bool>        isPuRec;         //是否在前端录像

public:
    TCmsPuServiceStatus()
    {
        Clear();
    }

    ~TCmsPuServiceStatus()
    {
        Clear();
    }

    void Clear()
    {
        audioCallSrc.clear();   
        isPuPreRec.clear();  
        isPuRec.clear();      
    }

public:
    const map<int,TCmsChannel>& GetAudioCallSrc() const
    {
        return audioCallSrc;
    }
    map<int,TCmsChannel>& GetAudioCallSrc()
    {
        return audioCallSrc;
    }
    void SetAudioCallSrc(const map<int,TCmsChannel>& tAudioCallSrc)
    {
        audioCallSrc = tAudioCallSrc;
    }

    const map<int,bool>& GetIsPuPreRec() const
    {
        return isPuPreRec;
    }
    map<int,bool>& GetIsPuPreRec()
    {
        return isPuPreRec;
    }
    void SetIsPuPreRec(const map<int,bool>& tIsPuPreRec)
    {
        isPuPreRec = tIsPuPreRec;
    }

    const map<int,bool>& GetIsPuRec() const
    {
        return isPuRec;
    }
    map<int,bool>& GetIsPuRec()
    {
        return isPuRec;
    }
    void SetIsPuRec(const map<int,bool>& tIsPuRec)
    {
        isPuRec = tIsPuRec;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsPuAlarmStatus
{
private:
    CCmsAlarmInputArray       alarmArray;       //告警信息
public:
    TCmsPuAlarmStatus()
    {
        Clear();
    }

    void Clear()
    {
        alarmArray.clear();
    }

public:

    const CCmsAlarmInputArray& GetAlarmArray() const
    {
        return alarmArray;
    }
    CCmsAlarmInputArray& GetAlarmArray()
    {
        return alarmArray;
    }
    void SetAlarmArray(const CCmsAlarmInputArray& tAlarmArray)
    {
        alarmArray = tAlarmArray;
    }

    void AddAlarmItem(const TCmsAlarmInput& tAlarmInput)
    {
        alarmArray.push_back(tAlarmInput);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsGpsData
{
public:
    TCmsGpsData()
    {
        Clear();
    }
    void Clear()
    {
        type = 0;
        subType = 0;
        data = STRING_NULL;
    }

private:
    int        type;                                   //透明数据类型
    int        subType;                                //透明数据子类型 根据byType来约定具体格式，不用时应置为0
    string     data;                                   //gps数据
public:
    void SetType(int nType)
    {
        type = nType;
    }
    int GetType() const
    {
        return type;
    }

    void SetSubType(int nSubType)
    {
        subType = nSubType;
    }
    int GetSubType() const
    {
        return subType;
    }

    void SetData(const string& strData)
    {
        data = strData;
    }
    string& GetData()
    {
        return data;
    }
    const string& GetData() const
    {
        return data;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsModuleOnOffLogInfo
{
public:
	string devUri;          // 设备Uri
	string devName;         // 设备名称
	string devAlias;        // 设备别名
	string happenedTime;    // 发生时间
	string netAddr;         // 网络地址(设备的IP地址信息)
	bool   online;			// 上下线标志（true为上线，false为下线）

public:
	TCmsModuleOnOffLogInfo()
	{
		Clear();
	}

	void Clear()
	{
		devUri = STRING_NULL;
		devName = STRING_NULL;
		devAlias = STRING_NULL;
		happenedTime = STRING_NULL;
		netAddr = STRING_NULL;
		online = true;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsLongOsdParam
{  
public:
	TCmsLongOsdParam()
	{
		Clear();
	}

	~TCmsLongOsdParam()
	{
		Clear();
	}

	void Clear()
	{
		state = 0;
		leftMargin = 0;
		topMargin = 0;
		color = 0;
		text = STRING_NULL;
	}

private:  
	int state;                         //编码字幕状态 DISABLE-无内容 TITLE-有字符字模 LOGOBMP-有图片台标
	int leftMargin;                    //编码字幕左边距 1-200;
	int topMargin;                     //编码字幕上边距 1-200;
	int color;                         //视频源字幕颜色;
	string text;                       //视频源字幕内容;
public:
	void SetState(int nState)
	{
		state = nState;
	}
	int GetState() const
	{
		return state;
	}

	void SetLeftMargin(int nLeftMargin)
	{
		leftMargin = nLeftMargin;
	}
	int GetLeftMargin() const
	{
		return leftMargin;
	}

	void SetTopMargin(int nTopMargin)
	{
		topMargin = nTopMargin;
	}
	int GetTopMargin() const
	{
		return topMargin;
	}

	void SetColor(int nColor)
	{
		color = nColor;
	}
	int GetColor() const
	{
		return color;
	}

	void SetText(const string& strText)
	{
		text = strText;
	}
	string& GetText()
	{
		return text;
	}
	const string& GetText() const
	{
		return text;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsLogoBmpOsdParam
{
public:
	TCmsLogoBmpOsdParam()
	{
		Clear();
	}

	~TCmsLogoBmpOsdParam()
	{
		Clear();
	}

	void Clear()
	{
		width = 0;
		heigth = 0;
		totalPack = 0;
		curPackIndex = 0;
		bmpOsdBuf = STRING_NULL;
		fontNum = 0;
		fontHeight = 0;
		fontWidth = STRING_NULL;
	}

private:  
	int width;                              //图像宽度
	int heigth;                             //图像高度
	int totalPack;               		    //总包数
	int curPackIndex;               	    //当前包的索引号,从1开始
	string bmpOsdBuf;               	    //BMPOSD内容，Base64编码后的数据
	int fontNum;               			    //字符数量
	int fontHeight;               		    //字符高
	string fontWidth;               	    //字符宽度，Base64编码后的数据
public:
	void SetWidth(int nWidth)
	{
		width = nWidth;
	}
	int GetWidth() const
	{
		return width;
	}

	void SetHeigth(int nHeigth)
	{
		heigth = nHeigth;
	}
	int GetHeigth() const
	{
		return heigth;
	}

	void SetTotalPack(int nTotalPack)
	{
		totalPack = nTotalPack;
	}
	int GetTotalPack() const
	{
		return totalPack;
	}

	void SetCurPackIndex(int nCurPackIndex)
	{
		curPackIndex = nCurPackIndex;
	}
	int GetCurPackIndex() const
	{
		return curPackIndex;
	}

	void SetBmpOsdBuf(const string& strBmpOsdBuf)
	{
		bmpOsdBuf = strBmpOsdBuf;
	}
	string& GetBmpOsdBuf()
	{
		return bmpOsdBuf;
	}
	const string& GetBmpOsdBuf() const
	{
		return bmpOsdBuf;
	}

	void SetFontNum(int nFontNum)
	{
		fontNum = nFontNum;
	}
	int GetFontNum() const
	{
		return fontNum;
	}

	void SetFontHeight(int nFontHeight)
	{
		fontHeight = nFontHeight;
	}
	int GetFontHeight() const
	{
		return fontHeight;
	}

	void SetFontWidth(const string& strFontWidth)
	{
		fontWidth = strFontWidth;
	}
	string& GetFontWidth()
	{
		return fontWidth;
	}
	const string& GetFontWidth() const
	{
		return fontWidth;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

enum TCmsCamType
{
	CMS_CAM_TYPE_BEGIN = 0,
	CMS_CAM_TYPE_SONY,	        //	SONY
	CMS_CAM_TYPE_AD,			//	AD
	CMS_CAM_TYPE_PELCO,			//	PELCO-D
	CMS_CAM_TYPE_PIH,			//	利凌
	CMS_CAM_TYPE_PELCO_P,		//	PELCO-P
	CMS_CAM_TYPE_PARCO,			//	PARCO柏高
	CMS_CAM_TYPE_AB,			//	AB
	CMS_CAM_TYPE_YUNTAI,		//	独立云台
	CMS_CAM_TYPE_Samsung,		//	三星
	CMS_CAM_TYPE_YAAN,			//	亚安
	CMS_CAM_TYPE_AD168,			//	AD168类型
	CMS_CAM_TYPE_KALATEL,		//	卡拉特
	CMS_CAM_TYPE_JN,			//	JN伟华
	CMS_CAM_TYPE_WV_CS650,		//	松下,是850的老协议
	CMS_CAM_TYPE_WV_CS850,		//	松下,是850的新协议
	CMS_CAM_TYPE_ALEC,		    //	艾立克
	CMS_CAM_TYPE_AB_P,          //	AB-P
	CMS_CAM_TYPE_AB_D,          //	AB-D
	CMS_CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
	CMS_CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
	CMS_CAM_TYPE_TIANDY,        //  天地伟业
	CMS_CAM_TYPE_G100,          //  G100
	CMS_CAM_TYPE_368ZT,         //  368转台插件
	CMS_CAM_TYPE_GaoDeTI,       //  GaoDeTI红外插件
	CMS_CAM_TYPE_DaliTI,        //  DaliTI红外插件
	CMS_CAM_TYPE_2130_PELCO_P,  //  非标准PELCO_P协议
	CMS_CAM_TYPE_K_PELCO_D,
	CMS_CAM_TYPE_JVCTKC676,
	CMS_CAM_TYPE_INFINOVA_PELCO_D,
	CMS_CAM_TYPE_PEARMAIN_PELCO_D,
	CMS_CAM_TYPE_PEARMAIN_PELCO_P,
	CMS_CAM_TYPE_YAAN_PELCO_D,
	CMS_CAM_TYPE_CAMERA_NUM,

	CMS_CAM_TYPE_CAMERA_NOCTLR = 63,    //不可控设备
	CMS_CAM_TYPE_MATRIX=128,	//	矩阵协议
	CMS_MATRIX_TYPE_AB=128,		//  AB 矩阵
	CMS_CAM_TYPE_ROBOT,			//  ROBOT 画面合成矩阵
	CMS_MATRIX_TYPE_SUNNHO,		//  SUNNHO	矩阵
	CMS_MATRIX_TYPE_PEARMAIN,   //  红苹果  矩阵
	CMS_MATRIX_TYPE_AD,         //  AD 矩阵
	CMS_MATRIX_TYPE_INFORNOVA,
	CMS_MATRIX_TYPE_PECLO,      
	CMS_MATRIX_TYPE_HONEWELL,  //霍尼韦尔
	CMS_MATRIX_TYPE_TIANDI,
	CMS_MATRIX_TYPE_HUATONG,//华通矩阵
	CMS_MATRIX_TYPE_TONGCHUANG,//同创矩阵
	CMS_MATRIX_TYPE_BOCOM,     //博康矩阵
	CMS_MATRIX_TYPE_EXTRON,    //爱思创
	CMS_MATRIX_TYPE_ST_MS650,  //金三立
	CMS_MATRIX_TYPE_LIKEN,     //利肯
	CMS_MATRIX_TYPE_VINET,     //微耐特
	CMS_MATRIX_TYPE_WEPUS,     //威普士
	CMS_CAM_TYPE_MATRIX_NUM
};

//---------------------------------------------------------------------------------
//                                3as功能相关部分
//---------------------------------------------------------------------------------

// 设备类型
typedef int TCmsDeviceType;
#define CMS_DEVICE_TYPE_INVALID						  0		// 非法设备类型
#define CMS_DEVICE_TYPE_ENCODER                       1     // 编码器
#define CMS_DEVICE_TYPE_DECODER                       2     // 解码器
#define CMS_DEVICE_TYPE_CODECER                       3     // 编解码器
#define CMS_DEVICE_TYPE_TV_WALL                       4     // 电视墙

typedef int TCmsDeviceCapType;
#define CMS_DEVICE_CAP_INVALID                        0		// 非法设备能力类型
#define CMS_DEVICE_CAP_DUMMY                          6		// 设备能力-伪能力(能力数值固定为1)
#define CMS_DEVICE_CAP_VIDEOSOURCE                    1		// 设备能力-视频源
#define CMS_DEVICE_CAP_LOCAL_STORAGE                  2		// 设备能力-本地存储
#define CMS_DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT      3		// 设备能力-并口告警
#define CMS_DEVICE_CAP_ENCODE_CHANNEL                 4		// 设备能力-编码通道
#define CMS_DEVICE_CAP_DECODE_CHANNEL                 5		// 设备能力-解码通道
#define CMS_DEVICE_CAP_DECODE_OUTPUTCHANNEL          13     // 设备能力-解码输出通道 比如公司的D04F设备,就具有多输出通道
#define CMS_DEVICE_CAP_LABEL_ENCODE_NVR               7		// 设备能力标签-NVR
#define CMS_DEVICE_CAP_LABEL_ENCODE_SVR               8		// 设备能力标签-SVR
#define CMS_DEVICE_CAP_LABEL_ENCODE_IPC_BULLET        9		// 设备能力标签-IPC枪机
#define CMS_DEVICE_CAP_LABEL_ENCODE_IPC_DOME         10		// 设备能力标签-IPC球机
#define CMS_DEVICE_CAP_LABEL_ENCODE_GENERAL          11     // 设备能力标签-普通编码器
#define CMS_DEVICE_CAP_LABEL_ENCODE_WIRELESS         12		// 设备能力标签-无线编码器
#define CMS_DEVICE_CAP_LABEL_GBDEVICE                14		// 设备能力标签-国标设备
#define CMS_DEVICE_CAP_LABEL_CONFDEVICE              15     // 设备能力标签-会议终端
#define CMS_DEVICE_CAP_LABEL_GBDRAWSTREAM            16     // 设备能力标签-国标原始码流

// 设备属性标志
typedef int TCmsPropertyFlag;
#define CMS_UNKNOWN                                   0     // 未知
#define CMS_PrivateDev                                1     // 私有设备
#define CMS_P2PDev_DISABLE_REASSIGN                   2     // 授权设备且禁止二次划归
#define CMS_P2PDev_ABLE_REASSIGN                      3     // 授权设备且允许二次划归

// 线路接入方式
typedef int TCmsNetType;
const int CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT = 0;      //CMU在NAT外，PU在NAT外标记
const int CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_INNAT  = 1;      //CMU在NAT外，PU在NAT内标记
const int CMS_DEVICE_LINEMODE_CMU_INNAT_PU_OUTNAT  = 2;      //CMU在NAT内，PU在NAT外标记
const int CMS_DEVICE_LINEMODE_CMU_INNAT_PU_INNAT   = 4;      //CMU在NAT内，PU在NAT内标记

// 兼容考虑，保留CMS_DEVICE_LINEMODE_INTRANET和CMS_DEVICE_LINEMODE_EXTRANET(最好不要再使用，直接使用上面的4个值）
#define CMS_DEVICE_LINEMODE_INTRANET CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT    //CMU与PU在同一网络
#define CMS_DEVICE_LINEMODE_EXTRANET CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_INNAT     //CMU与PU在不同网络


struct TCmsDeviceCapInfo
{
private:
	TCmsDeviceCapType type;		// 设备能力类型
	int num;					// 设备能力数目(也即具有多少个能力，比如具体多少个编码通道)
	int license;				// 收取license数目

public:
	TCmsDeviceCapInfo()
	{
		Clear();
	}
    TCmsDeviceCapInfo(TCmsDeviceCapType tType, int nNum, int nLicense)
    {
        type    = tType;
        num     = nNum;
        license = nLicense;
    }

	~TCmsDeviceCapInfo()
	{
		Clear();
	}

	void Clear()
	{
		type = CMS_DEVICE_CAP_INVALID;
		num  = 0;
		license = 0;
	}

	TCmsDeviceCapType GetType() const
	{
		return type;
	}

	void SetType(TCmsDeviceCapType val)
	{
		type = val;
	}

	int GetNum() const
	{
		return num;
	}

	void SetNum(int val)
	{
		num = val;
	}

	int GetLicense() const
	{
		return license;
	}

	void SetLicense(int val)
	{
		license = val;
	}
	
public:
	bool operator==(const TCmsDeviceCapInfo& rh) const
	{
		return (type == rh.GetType() && num == rh.GetNum() && license == rh.GetLicense());
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TCmsCreater;
#define CMSSDK_CREATE_BY_PLAT                  0		   // 由平台内部创建
#define CMSSDK_CREATE_BY_CMSSDK                1		   // 由cmssdk创建
#define CMSSDK_CREATE_BY_GBS                   2           // 由gbs创建，只限本域设备
#define CMSSDK_CREATE_BY_GBS_FOREIGN_DOMAIN    3           // 由gbs创建，只限异域设备


typedef string TCmsDevExtDataType;
#define CMS_DEV_EXT_FIXADDR					"fixAddr"		// 设备安装位置
#define CMS_DEV_EXT_KDMNO					"kdmNO"			// 1.0设备编号
#define CMS_DEV_EXT_GBID					"gbId"			// 国标编号

struct TCmsDevExtData
{
public:
	TCmsDevExtData()
	{
		Clear();
	}

	~TCmsDevExtData()
	{
		Clear();
	}

	void Clear()
	{
		extData.clear();
	}

	const map<TCmsDevExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TCmsDevExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TCmsDevExtDataType, string> &val)
	{
		extData = val;
	}

private:
	map<TCmsDevExtDataType, string> extData;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TCmsDomainExtDataType;
#define CMS_DOMAIN_EXT_KDMNO        "kdmNO"     // 1.0域编号
#define CMS_DOMAIN_EXT_FIXADDR      "fixAddr"   // 域的安装位置

struct TCmsDomainExtData
{
public:
    TCmsDomainExtData()
    {
    }

    void Clear()
    {
        extData.clear();
    }

private:
    map<TCmsDomainExtDataType, string> extData;                                  
public:
    void SetExtData(const map<TCmsDomainExtDataType, string>& cExtData)
    {
        extData = cExtData;
    }
    map<TCmsDomainExtDataType, string>& GetExtData()
    {
        return extData;
    }
    const map<TCmsDomainExtDataType, string>& GetExtData() const
    {
        return extData;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDeviceInfo
{
private:
    string            uri;                                 // 设备uri，devId@domain
	string            domainId;							   // 域Id
    string            name;                                // 设备名
    string            alias;                               // 设备别名
    string            description;                         // 设备描述
    TCmsDeviceType    type;                                // 设备类型
	string            modelName;						   // 设备型号名称（为空由cmssdk自动生成）
    string            manufacturer;                        // 设备制造商
	vector<TCmsDeviceCapInfo> capInfo;					   // 设备能力
    TCmsPropertyFlag  propertyFlag;                        // 设备属性标志
    bool              usedFlag;                            // 启用标志：false-停用，true-启用
    TCmsNetType       netType;                             // 线路接入方式
    string            netAddress;                          // 设备网络地址，IP或域名
    string            expireDate;                          // 使用截止日期（格式：YYYY-MM-DD）
    string            createDate;                          // 入网日期（格式：YYYY-MM-DD）
	TCmsCreater       creater;							   // 创建者
	TCmsDevExtData    devData;							   // 设备扩展信息
	string            groupUri;                            // 设备所属分组Uri
	string            owner;                                              // gbu、gbs专用（用于整域退网）

public:
    TCmsDeviceInfo()
    {
        Clear();
    }

    ~TCmsDeviceInfo()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
        name = STRING_NULL;
        alias = STRING_NULL;
        description = STRING_NULL;
        type = CMS_DEVICE_TYPE_INVALID;
        modelName = STRING_NULL;
        manufacturer = STRING_NULL;
		capInfo.clear();
        propertyFlag = CMS_UNKNOWN;
        usedFlag = true;
        netType  = CMS_DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT;
        netAddress = STRING_NULL;
        expireDate = STRING_NULL;
        createDate = STRING_NULL;
		devData.Clear();
		creater = CMSSDK_CREATE_BY_CMSSDK;
		groupUri = STRING_NULL;
    }

    const string &GetUri() const
    {
        return uri;
    }

    void SetUri(const string &val)
    {
        uri = val;
    }

	const string &GetDomainId() const
	{
		return domainId;
	}

	void SetDomainId(const string &val)
	{
		domainId = val;
	}

    const string &GetName() const
    {
        return name;
    }

    void SetName(const string &val)
    {
        name = val;
    }

    const string &GetAlias() const
    {
        return alias;
    }

    void SetAlias(const string &val)
    {
        alias = val;
    }

    const string &GetDescription() const
    {
        return description;
    }

    void SetDescription(const string &val)
    {
        description = val;
    }

    TCmsDeviceType GetType() const
    {
        return type;
    }

    void SetType(TCmsDeviceType val)
    {
        type = val;
    }

    const string &GetModelName() const
    {
        return modelName;
    }

    void SetModelName(const string &val)
    {
        modelName = val;
    }

    const string &GetManufacturer() const
    {
        return manufacturer;
    }

    void SetManufacturer(const string &val)
    {
        manufacturer = val;
    }

    const vector<TCmsDeviceCapInfo> &GetCapInfo() const
    {
        return capInfo;
    }

    void SetCapInfo(const vector<TCmsDeviceCapInfo> &val)
    {
        capInfo = val;
    }

    TCmsPropertyFlag GetPropertyFlag() const
    {
        return propertyFlag;
    }

    void SetPropertyFlag(TCmsPropertyFlag val)
    {
        propertyFlag = val;
    }

    bool GetUsedFlag() const
    {
        return usedFlag;
    }

    void SetUsedFlag(bool val)
    {
        usedFlag = val;
    }

    TCmsNetType GetNetType() const
    {
        return netType;
    }

    void SetNetType(TCmsNetType val)
    {
        netType = val;
    }

    const string &GetNetAddress() const
    {
        return netAddress;
    }

    void SetNetAddress(const string &val)
    {
        netAddress = val;
    }

    const string &GetExpireDate() const
    {
        return expireDate;
    }

    void SetExpireDate(const string &val)
    {
        expireDate = val;
    }

    const string &GetCreateDate() const
    {
        return createDate;
    }

    void SetCreateDate(const string &val)
    {
        createDate = val;
    }

	TCmsCreater GetCreater() const
	{
		return creater;
	}

	void SetCreater(TCmsCreater val)
	{
		creater = val;
	}

	const TCmsDevExtData &GetDevData() const
	{
		return devData;
	}
	TCmsDevExtData &GetDevData()
	{
		return devData;
	}
	void SetDevData(const TCmsDevExtData &val)
	{
		devData = val;
	}

	const string &GetGroupUri() const
	{
		return groupUri;
	}
	string &GetGroupUri()
	{
		return groupUri;
	}
	void SetGroupUri(const string &val)
	{
		groupUri = val;
	}

	void SetOwner(const string& strOwner)
	{
		owner = strOwner;
	}
	string& GetOwner()
	{
		return owner;
	}
	const string& GetOwner() const
	{
		return owner;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

};

class CCmsOptionField
{
private:
    set<string> fields;

public:
    CCmsOptionField()
    {
        Clear();
    }

    ~CCmsOptionField()
    {
        Clear();
    }

    void Clear()
    {
        fields.clear();
    }

    void SetOption(const string &opt)
    {
        fields.insert(opt);
    }

    bool HasOption(const string &opt) const
    {
        return (fields.end() != fields.find(opt));
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsSortItem
{
private:
    string column;                      // 排序字段名
    bool   asc;                         // 按升序排列

public:
    TCmsSortItem()
    {
        Clear();
    }

    ~TCmsSortItem()
    {
        Clear();
    }

    void Clear()
    {
        column = STRING_NULL;
        asc = true;
    }

    bool operator==(const TCmsSortItem &opd)
    {
        if (column == opd.GetColumn() && asc == opd.GetAsc())
            return true;
        else
            return false;
    }

    const string &GetColumn() const
    {
        return column;
    }

    void SetColumn(const string &val)
    {
        column = val;
    }

    bool GetAsc() const
    {
        return asc;
    }

    void SetAsc(bool val)
    {
        asc = val;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDeviceGroupInfo
{
private:
    string		uri;                 // 组uri
    string		parentUri;           // 组为域内根组时，Uri为空
    string		name;                // 组名
    string		description;         // 组描述信息
    string		createDate;          // 创建日期（格式：YYYY-MM-DD）
	TCmsCreater creater;			 // 创建者
	string      domainId;			 // 域Id
	string      parentDomainId;		 // 父组域Id
	string      owner;               // gbu、gbs专用（用于整域退网）
public:
    TCmsDeviceGroupInfo()
    {
        Clear();
    }

    ~TCmsDeviceGroupInfo()
    {
        Clear();
    }

    void Clear()
    {
        uri = STRING_NULL;
        parentUri = STRING_NULL;
        name = STRING_NULL;
        description = STRING_NULL;
        createDate = STRING_NULL;
    }

    const string &GetUri() const
    {
        return uri;
    }

    void SetUri(const string &val)
    {
        uri = val;
    }

    const string &GetParentUri() const
    {
        return parentUri;
    }

    void SetParentUri(const string &val)
    {
        parentUri = val;
    }

    const string &GetName() const
    {
        return name;
    }

    void SetName(const string &val)
    {
        name = val;
    }

    const string &GetDescription() const
    {
        return description;
    }

    void SetDescription(const string &val)
    {
        description = val;
    }

    const string &GetCreateDate() const
    {
        return createDate;
    }

    void SetCreateDate(const string &val)
    {
        createDate = val;
    }

	TCmsCreater GetCreater() const
	{
		return creater;
	}

	void SetCreater(TCmsCreater val)
	{
		creater = val;
	}

	const string &GetDomainId() const
	{
		return domainId;
	}

	void SetDomainId(const string &val)
	{
		domainId = val;
	}

	const string &GetParentDomainId() const
	{
		return parentDomainId;
	}

	void SetParentDomainId(const string &val)
	{
		parentDomainId = val;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetOwner(const string& strOwner)
    {
        owner = strOwner;
    }
    string& GetOwner()
    {
        return owner;
    }
    const string& GetOwner() const
    {
        return owner;
    }

};

struct TCmsDeviceCapIndexs
{
private:
	TCmsDeviceCapType	id;						// 设备能力
    vector<int>			indexs;					// 设备能力索引

public:
    TCmsDeviceCapIndexs()
    {
        Clear();
    }

    ~TCmsDeviceCapIndexs()
    {
        Clear();
    }

    void Clear()
    {
		id = CMS_DEVICE_CAP_INVALID;
        indexs.clear();
    }

	TCmsDeviceCapType GetCapType() const
	{
		return id;
	}

	void SetCapType(TCmsDeviceCapType val)
	{
		id = val;
	}

    const vector<int> &GetIndexs() const
    {
        return indexs;
    }

    void SetIndexs(const vector<int> &val)
    {
        indexs = val;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsDeviceGroupDeviceInfo
{
public:
	CCmsDeviceGroupDeviceInfo()
	{
		Clear();
	}

	~CCmsDeviceGroupDeviceInfo()
	{
		Clear();
	}

	void Clear()
	{
		groupUri = STRING_NULL;
		deviceUri = STRING_NULL;
		deviceCapIndexs.clear();
	}

	const string& GetDeviceGroupUri() const
	{
		return groupUri;
	}
	void SetDeviceGroupUri(const string &val)
	{
		groupUri = val;
	}

	const string& GetDeviceUri() const
	{
		return deviceUri;
	}
	void SetDeviceUri(const string &val)
	{
		deviceUri = val;
	}

	const vector<TCmsDeviceCapIndexs>& GetDeviceCapIndexs() const
	{
		return deviceCapIndexs;
	}
	void SetDeviceCapIndexs(const vector<TCmsDeviceCapIndexs>& val)
	{
		deviceCapIndexs = val;
	}

private:
	string groupUri;
	string deviceUri;
	vector<TCmsDeviceCapIndexs> deviceCapIndexs;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TCmsDeviceCapExtDataType;
#define CMS_DEVICE_CAP_EXT_DATA_TYPE_LON					"Lon"		//经度
#define CMS_DEVICE_CAP_EXT_DATA_TYPE_LAT					"Lat"		//纬度

struct TCmsDeviceCapExtData
{
public:
	TCmsDeviceCapExtData()
	{
	}

	void Clear()
	{
		extData.clear();
	}

	const map<TCmsDeviceCapExtDataType, string> &GetExtData() const
	{
		return extData;
	}
	map<TCmsDeviceCapExtDataType, string> &GetExtData()
	{
		return extData;
	}
	void SetExtData(const map<TCmsDeviceCapExtDataType, string> &val)
	{
		extData = val;
	}

	bool GetLon(double &val) const
	{
		return GetDoubleData(CMS_DEVICE_CAP_EXT_DATA_TYPE_LON, val);
	}
	void SetLon(double val)
	{
		SetDoubleData(CMS_DEVICE_CAP_EXT_DATA_TYPE_LON, val);
	}

	bool GetLat(double &val) const
	{
		return GetDoubleData(CMS_DEVICE_CAP_EXT_DATA_TYPE_LAT, val);
	}
	void SetLat(double val)
	{
		SetDoubleData(CMS_DEVICE_CAP_EXT_DATA_TYPE_LAT, val);
	}

private:
	bool GetDoubleData(const TCmsDeviceCapExtDataType &tType, double &val) const
	{
		map<TCmsDeviceCapExtDataType, string>::const_iterator it = extData.find(tType);
		if (extData.end() != it)
		{
			istringstream iss(it->second);
			iss>>val;
			return true;
		}

		return false;
	}
	void SetDoubleData(const TCmsDeviceCapExtDataType &tType, double val)
	{
		ostringstream oss;
		oss.precision(15);//覆盖默认精度
		oss<<val;
		extData[tType] = oss.str();
	}

private:
	map<TCmsDeviceCapExtDataType, string> extData;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDeviceCapIndexName
{
	TCmsDeviceCapIndexName()
	{
		Clear();
	}

	~TCmsDeviceCapIndexName()
	{
		Clear();
	}

	void Clear()
	{
		index = 0;
		name = STRING_NULL;
	}

public:

	int GetIndex() const
	{
		return index;
	}
	void SetIndex(int val)
	{
		index = val;
	}

	const string &GetName() const
	{
		return name;
	}

	void SetName(const string &val)
	{
		name = val;
	}

	const TCmsDeviceCapExtData &GetExtData() const
	{
		return extData;
	}
	TCmsDeviceCapExtData &GetExtData()
	{
		return extData;
	}
	void SetExtData(const TCmsDeviceCapExtData &val)
	{
		extData = val;
	}

private:
	int index;   // 设备能力索引
	string name; // 设备能力索引对应的名称
	TCmsDeviceCapExtData extData;	//设备能力对应的扩展信息
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDeviceCapName
{
public:
	TCmsDeviceCapName()
	{
		Clear();
	}

	~TCmsDeviceCapName()
	{
		Clear();
	}

	void Clear()
	{
		deviceUri = STRING_NULL;
		capId = CMS_DEVICE_CAP_INVALID;
	}

public:
	const string& GetDeviceUri() const
	{
		return deviceUri;
	}
	void SetDeviceUri(const string &val)
	{
		deviceUri = val;
	}

	TCmsDeviceCapType GetCapId() const
	{
		return capId;
	}
	void SetCapId(TCmsDeviceCapType val)
	{
		capId = val;
	}

	const vector<TCmsDeviceCapIndexName>& GetDeviceCapIndexNames() const
	{
		return deviceCapIndexNames;
	}
	vector<TCmsDeviceCapIndexName>& GetDeviceCapIndexNames()
	{
		return deviceCapIndexNames;
	}
	void SetDeviceCapIndexNames(const vector<TCmsDeviceCapIndexName> &val)
	{
		deviceCapIndexNames = val;
	}

private:
	string            deviceUri;									// 设备Uri
	TCmsDeviceCapType capId;										// 能力标识, 也即能力类型
	vector<TCmsDeviceCapIndexName> deviceCapIndexNames;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//以下是各个请求/应答的协议类，自动生成cpp

typedef string TGbRecType;           //国标录像类型
#define  GB_RECTYPE_ALL    "all"     //国标录像类型：所有类型
#define  GB_RECTYPE_MANUAL "manual"  //国标录像类型：手动录像
#define  GB_RECTYPE_TIME   "time"    //国标录像类型：周期录像
#define  GB_RECTYPE_ALARM  "alarm"   //国标录像类型：告警录像

typedef string TKdRecType; //科达录像类型

typedef string TCmsPlatRecType;
#define CMS_RCS_RECTYPE_ALARM					"RCS_RECTYPE_ALARM"				//告警录像
#define CMS_RCS_RECTYPE_HANDLE					"RCS_RECTYPE_HANDLE"			//手动录像
#define CMS_RCS_RECTYPE_PLAN					"RCS_RECTYPE_PLAN"				//计划录像，包括定时录像和周期录像
#define CMS_RCS_RECTYPE_UNKNOWN					"RCS_RECTYPE_UNKNOWN"

typedef string TCmsPuRecType;
#define CMS_PU_EVENT_TYPE_ALL                   "all"							//查询所有事件
#define CMS_PU_EVENT_TYPE_PIN_ALARM             "pin_alarm"						//并口告警录象
#define CMS_PU_EVENT_TYPE_MOTION_DETECT_ALARM   "motion_detect"					//移动侦测告警录象
#define CMS_PU_EVENT_TYPE_MANUAL                "manual"						//手动录象
#define CMS_PU_EVENT_TYPE_TIMELY                "timely"						//定时录象
#define CMS_PU_EVENT_TYPE_PERIODLY              "periodly"						//周期录象
#define CMS_PU_EVENT_TYPE_FILE                  "file"							//按文件查询
#define CMS_PU_EVENT_TYPE_H264					"h264"							//h264文件类型

#define MAX_REC_FILE_NUM						 (20)               //单页最大录象文件数目


typedef string TCmsRecLockState;
#define CMS_RCS_REC_LOCKED                "locked"
#define CMS_RCS_REC_UNLOCKED              "unlocked"
#define CMS_RCS_REC_UNKNOWN               "unknown"

struct TCmsRecTimeRange
{
public:
	TCmsRecTimeRange()
	{
		Clear();
	}

	~TCmsRecTimeRange()
	{
		Clear();
	}

	void Clear()
	{
		recType = STRING_NULL;
		timeRange.Clear();
	}

public:
	const TCmsPlatRecType& GetRecType() const
	{
		return recType;
	}
	TCmsPlatRecType& GetRecType()
	{
		return recType;
	}
	void SetRecType(const TCmsPlatRecType& var)
	{
		recType = var;
	}

	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	void SetTimeRange(const TCmsTimeRange& var)
	{
		timeRange = var;
	}
private:
	TCmsPlatRecType    recType;
	TCmsTimeRange      timeRange;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsRecTimeRange> TCmsRecTimeRangeList;


struct TCmsRecDuration
{
public:
	TCmsRecDuration()
	{
		Clear();
	}

	~TCmsRecDuration()
	{
		Clear();
	}

	void Clear()
	{
		puChn.Clear();
		recDurationToken = STRING_NULL;
		timeRange.Clear();
		recTimeRangeList.clear();
	}

public:
	const TCmsChannel& GetPuChn() const
	{
		return puChn;
	}
	TCmsChannel& GetPuChn()
	{
		return puChn;
	}
	void SetPuChn(const TCmsChannel& var)
	{
		puChn = var;
	}

	const TCmsRecToken& GetRecDurationToken() const
	{
		return recDurationToken;
	}
	TCmsRecToken& GetRecDurationToken()
	{
		return recDurationToken;
	}
	void SetRecDurationToken(const TCmsRecToken& var)
	{
		recDurationToken = var;
	}

	const int &GetRecSize() const
	{
		return recSize;
	}
	void SetRecSize(int var)
	{
		recSize = var;
	}

	const string& GetRecAlias() const
	{
		return recAlias;
	}
	string& GetRecAlias()
	{
		return recAlias;
	}
	void SetRecAlias(const string& var)
	{
		recAlias = var;
	}

	bool GetRecLocked() const
	{
		return recLocked;
	}
	void SetRecLocked(bool bRecLocked)
	{
		recLocked = bRecLocked;
	}

	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	void SetTimeRange(const TCmsTimeRange& var)
	{
		timeRange = var;
	}

	const TCmsRecTimeRangeList& GetRecTimeRangeList() const
	{
		return recTimeRangeList;
	}
	TCmsRecTimeRangeList& GetRecTimeRangeList()
	{
		return recTimeRangeList;
	}
	void SetRecTimeRangeList(const TCmsRecTimeRangeList& var)
	{
		recTimeRangeList = var;
	}

private:
	TCmsChannel            puChn;
	TCmsRecToken           recDurationToken;
	int                    recSize;						// 录像文件大小
	string                 recAlias;					// 录像别名
	bool                   recLocked;					// 录像是否锁定
	TCmsTimeRange          timeRange;
	TCmsRecTimeRangeList   recTimeRangeList;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsRecDuration> TCmsRecDurationList;



//平台录像状态定义
typedef string TCmsPlatRecStatus;
#define   CMS_PLAT_REC_NO           "PlatRecNo"
#define   CMS_PLAT_REC_OK           "PlatRecOk"
#define   CMS_PLAT_REC_TRY          "PlatRecTry"
#define   CMS_PLAT_REC_STOPPING     "PlatRecStopping"

struct TCmsRecStatus
{
private:
	TCmsPlatRecStatus recStatus;               // 录像状态
	int               lastError;               // 最后的错误
public:
	TCmsRecStatus()
	{
		Clear();
	}

	~TCmsRecStatus()
	{
		Clear();
	}

	void Clear()
	{
		recStatus = CMS_PLAT_REC_NO;
		lastError = 0;
	}

	void SetRecStatus(const TCmsPlatRecStatus& strRecStatus)
	{
		recStatus = strRecStatus;
	}
	TCmsPlatRecStatus& GetRecStatus()
	{
		return recStatus;
	}
	const TCmsPlatRecStatus& GetRecStatus() const
	{
		return recStatus;
	}

	void SetLastError(int nLastError)
	{
		lastError = nLastError;
	}
	int GetLastError() const
	{
		return lastError;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsRcsPuRecordStatus
{
public:
	TCmsRcsPuRecordStatus()
	{
		Clear();
	}

	~TCmsRcsPuRecordStatus()
	{
		Clear();
	}

	void Clear()
	{
		platRec.clear();      
	}
public:
	void SetPlatRec(const map<int,TCmsRecStatus>& cPlatRec)
	{
		platRec = cPlatRec;
	}
	map<int,TCmsRecStatus>& GetPlatRec()
	{
		return platRec;
	}
	const map<int,TCmsRecStatus>& GetPlatRec() const
	{
		return platRec;
	}

private:
	map<int,TCmsRecStatus>    platRec;    //通道的平台录像状态
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsTvDecoderBinder
{
	TCmsTvDecoderBinder()
	{
		Clear();
	}

	~TCmsTvDecoderBinder()
	{
		Clear();
	}

	void Clear()
	{
		tvId = 0;
		decoderOutputId = 0;
		decoderUri = STRING_NULL;
	}

	int GetTvId() const
	{
		return tvId;
	}

	void SetTvId(int nTvId)
	{
		tvId = nTvId;
	}

	int GetDecoderOutputId() const
	{
		return decoderOutputId;
	}

	void SetDecoderOutputId(int nDecoderOutputId)
	{
		decoderOutputId = nDecoderOutputId;
	}

	const string &GetDecoderUri() const
	{
		return decoderUri;
	}

	void SetDecoderUri(const string &strDecoderUri)
	{
		decoderUri = strDecoderUri;
	}

private:
	int tvId;				// 电视编号
	int decoderOutputId;	// 解码器视频输出通道Id
	string decoderUri;		// 解码器Uri
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsTvwallInfo : public TCmsDeviceInfo
{
public:
	CCmsTvwallInfo()
	{
		Clear();
	}

	~CCmsTvwallInfo()
	{
		Clear();
	}

	void Clear()
	{
		tvNum = 0;
		tvwallStyle = 0;
		tvwallCustomStyleData = STRING_NULL;
		tvDecoderBinders.clear();
	}

private:
	int tvNum;            // 电视机数目
	int tvwallStyle;      // 电视墙风格
	string tvwallCustomStyleData;					// 电视墙自定义风格数据
	vector<TCmsTvDecoderBinder> tvDecoderBinders;	// 电视与解码器绑定关系

public:
	void SetTvNum(int nTvNum)
	{
		tvNum = nTvNum;
	}
	int GetTvNum() const
	{
		return tvNum;
	}

	void SetTvwallStyle(int nTvwallStyle)
	{
		tvwallStyle = nTvwallStyle;
	}
	int GetTvwallStyle() const
	{
		return tvwallStyle;
	}

	void SetTvwallCustomStyleData(const string& strTvwallCustomStyleData)
	{
		tvwallCustomStyleData = strTvwallCustomStyleData;
	}
	string& GetTvwallCustomStyleData()
	{
		return tvwallCustomStyleData;
	}
	const string& GetTvwallCustomStyleData() const
	{
		return tvwallCustomStyleData;
	}

	void SetTvDecoderBinders(const vector<TCmsTvDecoderBinder>& cTvDecoderBinders)
	{
		tvDecoderBinders = cTvDecoderBinders;
	}
	vector<TCmsTvDecoderBinder>& GetTvDecoderBinders()
	{
		return tvDecoderBinders;
	}
	const vector<TCmsTvDecoderBinder>& GetTvDecoderBinders() const
	{
		return tvDecoderBinders;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsNruBaseInfo
{
private:
    string nruID;
    string devName;     //设备名
    string devType;   //<设备类型>@<部署方式>
    string manuFactory; //设备厂商

public:
    TCmsNruBaseInfo()
    {
    }

    void Clear()
    {
        nruID = STRING_NULL;
        devName = STRING_NULL;
        devType = STRING_NULL;
        manuFactory = STRING_NULL;
    }

public:
    //nruID
    const string& GetNruID() const
    {
        return nruID;
    }
    string& GetNruID()
    {
        return nruID;
    }
    void SetNruID(const string& var)
    {
        nruID = var;
    }

    //devName
    const string& GetDevName() const
    {
        return devName;
    }
    string& GetDevName()
    {
        return devName;
    }
    void SetDevName(const string& var)
    {
        devName = var;
    }

    //devType
    const string& GetDevType() const
    {
        return devType;
    }
    string& GetDevType()
    {
        return devType;
    }
    void SetDevType(const string& var)
    {
        devType = var;
    }

    //manuFactory
    const string& GetManuFactory() const
    {
        return manuFactory;
    }
    string& GetManuFactory()
    {
        return manuFactory;
    }
    void SetManuFactory(const string& var)
    {
        manuFactory = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsNetworkInfo
{
private:
    string ifName;      //网卡名
    string ipAddr;      //ip地址
    string netMask;      //ip地址
    string gateWay;     //网关地址

public:
    TCmsNetworkInfo()
    {
    }

    void Clear()
    {
        ifName = STRING_NULL;
        ipAddr = STRING_NULL;
        netMask = STRING_NULL;
        gateWay = STRING_NULL;
    }

public:

    //ifName
    const string& GetIfName() const
    {
        return ifName;
    }
    string& GetIfName()
    {
        return ifName;
    }
    void SetIfName(const string& var)
    {
        ifName = var;
    }

    //ipAddr
    const string& GetIpAddr() const
    {
        return ipAddr;
    }
    string& GetIpAddr()
    {
        return ipAddr;
    }
    void SetIpAddr(const string& var)
    {
        ipAddr = var;
    }

    //netMask
    const string& GetNetMask() const
    {
        return netMask;
    }
    string& GetNetMask()
    {
        return netMask;
    }
    void SetNetMask(const string& var)
    {
        netMask = var;
    }

    //gateWay
    const string& GetGateWay() const
    {
        return gateWay;
    }
    string& GetGateWay()
    {
        return gateWay;
    }
    void SetGateWay(const string& var)
    {
        gateWay = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


struct TCmsNruInfo
{
private:
    TCmsNruBaseInfo    nruBaseInfo;
    TCmsNetworkInfo    networkInfo;
    string         svrAddr;            //注册的RMS服务器地址
    int             maxBandWidth;       //nru的最大数据带宽，单位：Kb/s
    bool            isOnline;
    bool            isScOnline;
    bool            isUsed;             //是否启用

public:
    TCmsNruInfo()
    {
        Clear();
    }

    void Clear()
    {
        nruBaseInfo.Clear();
        networkInfo.Clear();
        svrAddr = STRING_NULL;
        maxBandWidth = 0;
        isOnline = false;
        isScOnline = false;
        isUsed = false;
    }

public:

    //nruBaseInfo
    const TCmsNruBaseInfo& GetNruBaseInfo() const
    {
        return nruBaseInfo;
    }
    TCmsNruBaseInfo& GetNruBaseInfo()
    {
        return nruBaseInfo;
    }
    void SetNruBaseInfo(const TCmsNruBaseInfo& var)
    {
        nruBaseInfo = var;
    }


    //networkInfo
    const TCmsNetworkInfo& GetNetworkInfo() const
    {
        return networkInfo;
    }
    TCmsNetworkInfo& GetNetworkInfo()
    {
        return networkInfo;
    }
    void SetNetworkInfo(const TCmsNetworkInfo& var)
    {
        networkInfo = var;
    }

    //svrAddr
    const string& GetSvrAddr() const
    {
        return svrAddr;
    }
    string& GetSvrAddr()
    {
        return svrAddr;
    }
    void SetSvrAddr(const string& var)
    {
        svrAddr = var;
    }

    //maxBandWidth
    int GetMaxBandWidth() const
    {
        return maxBandWidth;
    }
    void SetMaxBandWidth(int var)
    {
        maxBandWidth = var;
    }

    //isOnline
    bool GetIsOnline() const
    {
        return isOnline;
    }
    void SetIsOnline(bool var)
    {
        isOnline = var;
    }

    //isScOnline
    bool GetIsScOnlie() const
    {
        return isScOnline;
    }
    void SetIsScOnline(bool var)
    {
        isScOnline = var;
    }

    //nru是否提供完整服务（录像业务及配置业务）
    bool IsNruFullyAvailable() const
    {
        return isOnline && isScOnline;
    }

    //isUsed
    bool GetIsUsed() const
    {
        return isUsed;
    }
    void SetIsUsed(bool var)
    {
        isUsed = var;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDiskDevOpState
{
public:
    TCmsDiskDevOpState()
    {
        Clear();
    }

    void Clear()
    {
        operation = STRING_NULL;
        percent = 0;
        desc = STRING_NULL;
    }
    string ToString() const
    {
        std::ostringstream ossObj;
        return ossObj.str();
    }

    friend std::ostream& operator<<(std::ostream& output, const TCmsDiskDevOpState& obj)
    {
        output<<obj.operation<<"("<<obj.percent<<")";

        return output;
    }

private:
    string              operation;                   //操作状态
    int                 percent;                     //进度百分比                    
    string              desc;                        //操作状态描述

public:
    void SetOperation(const string& strOperation)
    {
        operation = strOperation;
    }
    string& GetOperation()
    {
        return operation;
    }
    const string& GetOperation() const
    {
        return operation;
    }

    void SetPercent(int nPercent)
    {
        percent = nPercent;
    }
    int GetPercent() const
    {
        return percent;
    }

    void SetDesc(const string& strDesc)
    {
        desc = strDesc;
    }
    string& GetDesc()
    {
        return desc;
    }
    const string& GetDesc() const
    {
        return desc;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef int TCmsPartAlarmLevel;
#define CMS_RCS_ALARM_LEVEL_NORMAL        0						//磁盘空间当前状况良好
#define CMS_RCS_ALARM_LEVEL_ONE           1						//磁盘空间一级告警
#define CMS_RCS_ALARM_LEVEL_TWO           2						//磁盘空间二级告警

typedef string TCmsPartState;
#define	CMS_RCS_PARTSTATE_OFFLINE       "OFFLINE"           //下线
#define	CMS_RCS_PARTSTATE_UNFORMATTED   "UNFORMATTED"       //未格式化
#define	CMS_RCS_PARTSTATE_FORMATTED     "FORMATTED"         //已格式化
#define	CMS_RCS_PARTSTATE_MOUNTED       "MOUNTED"           //已挂载
#define	CMS_RCS_PARTSTATE_UMOUNTED      "UMOUNTED"          //已卸载
#define	CMS_RCS_PARTSTATE_UNKNOWN       "UNKNOWN"           //未知状态

typedef string TCmsPartType;
#define CMS_RCS_PARTTYPE_VBFS            "vbfs"				//分区格式为vbfs
#define CMS_RCS_PARTTYPE_EXT3            "ext3"				//分区格式为ext3
#define CMS_RCS_PARTTYPE_EXT4            "ext4"             //分区格式为ext4
#define CMS_RCS_PARTTYPE_UNKNOWN         "unknow"			//无法识别

struct TCmsPartInfo
{
private:
    int			 			diskID;             			//磁盘ID
    int 					partID;             			//分区ID
    string		     		partName;             			//分区名
    string      			mountPath;             			//挂载路径,通常以/mnt、/vbfs开头
    int 		 			totalSize;             			//分区大小（单位：M）;
    int	    	 			freeSize;             			//可用空间大小 （单位：M）; 
	int                     preAllocSize;                   //预留空间大小 （单位：M）
	int                     lockSize;                       //录像锁定空间大小（单位：M）
    TCmsPartState		    partState;             			//分区状态
    TCmsPartType		    partType;             			//分区格式（vbfs，ext3）
    int						start;             				//起始块位置
    int						end;             				//结束块位置
	TCmsPartAlarmLevel      alarmLevel;                     //磁盘分区告警级别（CMS_RCS_ALARM_LEVEL_NORMAL时为告警产生，其他为告警取消）

    //动态信息
    TCmsDiskDevOpState		opState;						//操作状态
    int    					ioUtil;							//IO使用率
    int						abnNum;             			//异常次数
    string      			abnCode;						//异常代码
    int						recordNum;             			//录像数

public:
    TCmsPartInfo()
    {
        Clear();
    }

    void Clear()
    {
        diskID = 0;
        partID = 0;
        partName = STRING_NULL;
        mountPath = STRING_NULL;
        totalSize = 0;
        freeSize = 0;
		preAllocSize = 0;
		lockSize = 0;
        partState = STRING_NULL;
        partType = STRING_NULL;
        start = 0;
        end = 0;
		alarmLevel = CMS_RCS_ALARM_LEVEL_NORMAL;

        opState.Clear();
        ioUtil = 0;
        abnNum = 0;
        abnCode = STRING_NULL;
        recordNum = 0;
    }

    void PrintData() const;

public:
    void SetDiskID(int nDiskID)
    {
        diskID = nDiskID;
    }
    int GetDiskID() const
    {
        return diskID;
    }

    void SetPartID(int nPartID)
    {
        partID = nPartID;
    }
    int GetPartID() const
    {
        return partID;
    }

    void SetPartName(const string& strPartName)
    {
        partName = strPartName;
    }
    string& GetPartName()
    {
        return partName;
    }
    const string& GetPartName() const
    {
        return partName;
    }

    void SetMountPath(const string& strMountPath)
    {
        mountPath = strMountPath;
    }
    string& GetMountPath()
    {
        return mountPath;
    }
    const string& GetMountPath() const
    {
        return mountPath;
    }

    void SetTotalSize(int var)
    {
        totalSize = var;
    }
    int GetTotalSize() const
    {
        return totalSize;
    }

    void SetFreeSize(int var)
    {
        freeSize = var;
    }
    int GetFreeSize() const
    {
        return freeSize;
    }

	void SetPreAllocSize(int var)
	{
		preAllocSize = var;
	}
	int GetPreAllocSize() const
	{
		return preAllocSize;
	}

	void SetLockSize(int var)
	{
		lockSize = var;
	}
	int GetLockSize() const
	{
		return lockSize;
	}

    void SetPartState(const TCmsPartState& strPartState)
    {
        partState = strPartState;
    }
    TCmsPartState& GetPartState()
    {
        return partState;
    }
    const TCmsPartState& GetPartState() const
    {
        return partState;
    }

    void SetPartType(const TCmsPartType& strPartType)
    {
        partType = strPartType;
    }
    TCmsPartType& GetPartType()
    {
        return partType;
    }
    const TCmsPartType& GetPartType() const
    {
        return partType;
    }

    void SetStart(int nStart)
    {
        start = nStart;
    }
    int GetStart() const
    {
        return start;
    }

    void SetEnd(int nEnd)
    {
        end = nEnd;
    }
    int GetEnd() const
    {
        return end;
    }

	TCmsPartAlarmLevel GetPartAlarmLevel() const
	{
		return alarmLevel;
	}
	void SetPartAlarmLevel(TCmsPartAlarmLevel var)
	{
		alarmLevel = var;
	}

    //opState
    const TCmsDiskDevOpState& GetOpState() const
    {
        return opState;
    }
    TCmsDiskDevOpState& GetOpState()
    {
        return opState;
    }
    void SetOpState(const TCmsDiskDevOpState& var)
    {
        opState = var;
    }

    //ioUtil
    void SetIoUtil(int nIoUtil)
    {
        ioUtil = nIoUtil;
    }
    int GetIoUtil() const
    {
        return ioUtil;
    }

    //abnNum
    void SetAbnNum(int nAbnNum)
    {
        abnNum = nAbnNum;
    }
    int GetAbnNum() const
    {
        return abnNum;
    }

    //abnCode
    void SetAbnCode(const string& var)
    {
        abnCode = var;
    }
    string& GetAbnCode()
    {
        return abnCode;
    }
    const string& GetAbnCode() const
    {
        return abnCode;
    }

    //recordNum
    void SetRecordNum(int nRecordNum)
    {
        recordNum = nRecordNum;
    }
    int GetRecordNum() const
    {
        return recordNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TCmsDiskState;
#define CMS_RCS_DISKSTATE_ONLINE					"ONLINE"
#define CMS_RCS_DISKSTATE_OFFLINE					"OFFLINE"
#define CMS_RCS_DISKSTATE_UNKNOWN					"UNKNOWN"

typedef string TCmsDiskUsage;
#define CMS_RCS_DISK_USAGE_DATA                      "DATA"         //数据盘
#define CMS_RCS_DISK_USAGE_SYSTEM                    "SYSTEM"       //系统盘
#define CMS_RCS_DISK_USAGE_BACKUP                    "BACKUP"       //备份盘
#define CMS_RCS_DISK_USAGE_SATA                      "SATA"         //内置sata盘
#define CMS_RCS_DISK_USAGE_UNKNOWN                   "UNKNOWN"

struct TCmsDiskInfo
{
private:
    int                  diskID;                                  //磁盘ID
    string               diskName;                                //磁盘名
    int                  diskSize;                                //总容量（单位：M）; 
    int                  diskAbnNum;                              //异常次数
    TCmsDiskState        diskState;                               //磁盘状态
    string               diskSource;                              //磁盘来源
    int 			     cylinders;                               //磁盘柱面数
    int 			     heads;                                   //磁盘磁头数,HZCTODO废弃
    int                  sectors;                                 //扇区数,HZCTODO废弃
    TCmsDiskUsage        diskUsage;                               //磁盘用途   
    vector<TCmsPartInfo> partList;                                //分区信息

public:
    TCmsDiskInfo()
    {
        Clear();
    }

    void Clear()
    {
        diskID = 0;
        diskName = STRING_NULL;
        diskSize = 0;
        diskAbnNum = 0;
        diskState = STRING_NULL;
        diskSource = STRING_NULL;
        cylinders = 0;
        heads = 0;
        sectors = 0;
        partList.clear();
    }

    void PrintData() const;

public:
    void SetDiskID(int nDiskID)
    {
        diskID = nDiskID;
    }
    int GetDiskID() const
    {
        return diskID;
    }

    void SetDiskName(const string& strDiskName)
    {
        diskName = strDiskName;
    }
    string& GetDiskName()
    {
        return diskName;
    }
    const string& GetDiskName() const
    {
        return diskName;
    }

    void SetDiskSize(int var)
    {
        diskSize = var;
    }
    int GetDiskSize() const
    {
        return diskSize;
    }

    void SetDiskAbnNum(int nDiskAbnNum)
    {
        diskAbnNum = nDiskAbnNum;
    }
    int GetDiskAbnNum() const
    {
        return diskAbnNum;
    }

    void SetDiskState(const TCmsDiskState& strDiskState)
    {
        diskState = strDiskState;
    }
    TCmsDiskState& GetDiskState()
    {
        return diskState;
    }
    const TCmsDiskState& GetDiskState() const
    {
        return diskState;
    }

    void SetDiskSource(const string& cDiskSource)
    {
        diskSource = cDiskSource;
    }
    string& GetDiskSource()
    {
        return diskSource;
    }
    const string& GetDiskSource() const
    {
        return diskSource;
    }

    void SetCylinders(int nCylinders)
    {
        cylinders = nCylinders;
    }
    int GetCylinders() const
    {
        return cylinders;
    }

    void SetHeads(int nHeads)
    {
        heads = nHeads;
    }
    int GetHeads() const
    {
        return heads;
    }

    void SetSectors(int nSectors)
    {
        sectors = nSectors;
    }
    int GetSectors() const
    {
        return sectors;
    }

    void SetDiskUsage(const TCmsDiskUsage& cDiskUsage)
    {
        diskUsage = cDiskUsage;
    }
    TCmsDiskUsage& GetDiskUsage()
    {
        return diskUsage;
    }
    const TCmsDiskUsage& GetDiskUsage() const
    {
        return diskUsage;
    }

    void SetPartList(const vector<TCmsPartInfo>& cPartList)
    {
        partList = cPartList;
    }
    vector<TCmsPartInfo>& GetPartList()
    {
        return partList;
    }
    const vector<TCmsPartInfo>& GetPartList() const
    {
        return partList;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsDiskInfo> TCmsDiskInfoList;

struct TCmsNruDetailInfo
{
private:
	string nruID;
	TCmsDiskInfoList diskInfoList;

public:
	TCmsNruDetailInfo()
	{
		Clear();
	}

	~TCmsNruDetailInfo()
	{
		Clear();
	}

	void Clear()
	{
		nruID = STRING_NULL;
		diskInfoList.clear();
	}

	const string &GetNruID() const
	{
		return nruID;
	}
	string &GetNruID()
	{
		return nruID;
	}
	void SetNruID(const string &var)
	{
		nruID = var;
	}

	const TCmsDiskInfoList &GetNruInfoList() const
	{
		return diskInfoList;
	}
	TCmsDiskInfoList &GetNruInfoList()
	{
		return diskInfoList;
	}
	void SetNruInfoList(const TCmsDiskInfoList &var)
	{
		diskInfoList = var;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsNruDetailInfo> TCmsNruDetailInfoList;


typedef string TCmsCoverPolicy;
#define CMS_RCS_COVERPOLICY_STOPRECORD				"RCS_COVERPOLICY_STOPRECORD"       //通道空间满停录像
#define CMS_RCS_COVERPOLICY_COVERALL				"RCS_COVERPOLICY_COVERALL"			//通道空间满覆盖所有录像
#define CMS_RCS_COVERPOLICY_COVERNORMAL				"RCS_COVERPOLICY_COVERNORMAL"		//通道空间满覆盖普通录像
#define CMS_RCS_COVERPOLICY_UNKNOWN					"RCS_COVERPOLICY_UNKNOWN"

//电视操作类型
typedef string TCmsTvOccurType;
#define CMS_TV_OCCUR_TYPE_ALARM						"alarm"             //告警上墙
#define CMS_TV_OCCUR_TYPE_HAND						"hand"              //手动上墙

typedef string TCmsOprType;
#define CMS_POLL_OPERATE_CONFIG						"config"
#define CMS_POLL_OPERATE_START						"start"
#define CMS_POLL_OPERATE_PAUSE						"pause"
#define CMS_POLL_OPERATE_RESUME						"resume"
#define CMS_POLL_OPERATE_STOP						"stop"

struct TCmsTvChannel
{
private:
	string          tvWallUri;                      //电视墙Uri
	int             tvId;                           //电视机ID
	int             tvDivId;                        //电视分画面ID

public:
	TCmsTvChannel()
	{
		Clear();
	}

	~TCmsTvChannel()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		tvId = 0;
		tvDivId = 0;
	}
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetTvId(int nTvId)
	{
		tvId = nTvId;
	}
	int GetTvId() const
	{
		return tvId;
	}

	void SetTvDivId(int nTvDivId)
	{
		tvDivId = nTvDivId;
	}
	int GetTvDivId() const
	{
		return tvDivId;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsTvPollUnit
{
public:
	TCmsTvPollUnit()
	{
		Clear();
	}

	~TCmsTvPollUnit()
	{
		Clear();
	}

	void Clear()
	{
		pollIndex = 0;
		encChn.Clear();
		pollTime = 0;
	}

private:
	int           pollIndex;                   //轮询序号
	TCmsChannel   encChn;                      //编码器通道
	int           pollTime;                    //轮询时间，默认为0，表示无穷大，一直播放不轮询
public:
	void SetPollInex(int nPollInex)
	{
		pollIndex = nPollInex;
	}
	int GetPollInex() const
	{
		return pollIndex;
	}

	void SetEncChn(const TCmsChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TCmsChannel& GetEncChn()
	{
		return encChn;
	}
	const TCmsChannel& GetEncChn() const
	{
		return encChn;
	}

	void SetPollTime(int nPollTime)
	{
		pollTime = nPollTime;
	}
	int GetPollTime() const
	{
		return pollTime;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsTvPollUnit> CCmsTvPollList;

struct TCmsTvWallSchemePollUnit
{
public:
	TCmsTvWallSchemePollUnit()
	{
		Clear();
	}

	~TCmsTvWallSchemePollUnit()
	{
		Clear();
	}

	void Clear()
	{
		schemeName = STRING_NULL;
		pollIndex = 0;
		pollTime = 0;
	}

	int         pollIndex;                       //轮询序号
	string      schemeName;                      //预案名称
	int         pollTime;                        //轮询时间，默认为0，表示无穷大，一直播放不轮询
public:

	void SetPollInex(int nPollInex)
	{
		pollIndex = nPollInex;
	}
	int GetPollInex() const
	{
		return pollIndex;
	}

	void SetSchemeName(const string& strSchemeName)
	{
		schemeName = strSchemeName;
	}
	string& GetSchemeName()
	{
		return schemeName;
	}
	const string& GetSchemeName() const
	{
		return schemeName;
	}

	void SetPollTime(int nPollTime)
	{
		pollTime = nPollTime;
	}
	int GetPollTime() const
	{
		return pollTime;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef    vector<TCmsTvWallSchemePollUnit>    CCmsTTvWallSchemePollList;

//预案组，一个电视墙只有唯一一个预案组
struct TCmsTvWallSchemeGroup
{
public:
	TCmsTvWallSchemeGroup()
	{
		Clear();
	}

	~TCmsTvWallSchemeGroup()
	{
		Clear();
	}

	void Clear()
	{
		tvWallUri = STRING_NULL;
		pollList.clear();
	}

private:
	string      tvWallUri;                              //电视墙ID
	CCmsTTvWallSchemePollList pollList;                 //轮询列表
public:
	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetPollList(const CCmsTTvWallSchemePollList& cPollList)
	{
		pollList = cPollList;
	}
	CCmsTTvWallSchemePollList& GetPollList()
	{
		return pollList;
	}
	const CCmsTTvWallSchemePollList& GetPollList() const
	{
		return pollList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//窗口分画面
struct TCmsTvWinDiv
{
public:
	TCmsTvWinDiv()
	{
		Clear();
	}

	~TCmsTvWinDiv()
	{
		Clear();
	}

	void Clear()
	{
		tvWinDivId = 0;
		pollList.clear();
	}

private:
	int              tvWinDivId;                             //电视分画面id，也就是对应decChn
	CCmsTvPollList   pollList;                               //轮询信息列表
public:
	void SetTvWinDivId(int nTvWinDivId)
	{
		tvWinDivId = nTvWinDivId;
	}
	int GetTvWinDivId() const
	{
		return tvWinDivId;
	}

	void SetPollList(const CCmsTvPollList& cPollList)
	{
		pollList = cPollList;
	}
	CCmsTvPollList& GetPollList()
	{
		return pollList;
	}
	const CCmsTvPollList& GetPollList() const
	{
		return pollList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef     vector<TCmsTvWinDiv> TCmsTvWinDivList;

typedef int TCmsTvWinStyle;
#define CMS_TV_WIN_STYLE_1				1
#define CMS_TV_WIN_STYLE_2				2
#define CMS_TV_WIN_STYLE_4				4
#define CMS_TV_WIN_STYLE_9				9
#define CMS_TV_WIN_STYLE_16				16

struct TCmsTvWin
{
public:
	TCmsTvWin()
	{
		Clear();
	}

	~TCmsTvWin()
	{
		Clear();
	}

	void Clear()
	{
		tvWinId = 0;
		style = CMS_TV_WIN_STYLE_1;
		divList.clear();
	}

private:
	int              tvWinId;                                //电视机id
	TCmsTvWinStyle   style;                                  //电视窗口风格，也就是解码器风格
	TCmsTvWinDivList divList;                                //各个分画面的信息
public:
	void SetTvWinId(int nTvWinId)
	{
		tvWinId = nTvWinId;
	}
	int GetTvWinId() const
	{
		return tvWinId;
	}

	void SetStyle(TCmsTvWinStyle nStyle)
	{
		style = nStyle;
	}
	TCmsTvWinStyle GetStyle() const
	{
		return style;
	}

	void SetDivList(const TCmsTvWinDivList& cDivList)
	{
		divList = cDivList;
	}
	TCmsTvWinDivList& GetDivList()
	{
		return divList;
	}
	const TCmsTvWinDivList& GetDivList() const
	{
		return divList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef     vector<TCmsTvWin>      CCmsTvWinList;

struct TCmsTvWallScheme
{
public:
	TCmsTvWallScheme()
	{
		Clear();
	}

	~TCmsTvWallScheme()
	{
		Clear();
	}

	void Clear()
	{
		schemeName = STRING_NULL;
		tvWallUri = STRING_NULL;
		tvWinList.clear();
	}

private:
	string      schemeName;                      //预案名称
	string      tvWallUri;                       //电视墙ID
	CCmsTvWinList  tvWinList;                    //电视墙TV列表，也就编/解码器的对应关系
public:
	void SetSchemeName(const string& strSchemeName)
	{
		schemeName = strSchemeName;
	}
	string& GetSchemeName()
	{
		return schemeName;
	}
	const string& GetSchemeName() const
	{
		return schemeName;
	}

	void SetTvWallUri(const string& strTvWallUri)
	{
		tvWallUri = strTvWallUri;
	}
	string& GetTvWallUri()
	{
		return tvWallUri;
	}
	const string& GetTvWallUri() const
	{
		return tvWallUri;
	}

	void SetTvWinList(const CCmsTvWinList& cTvWinList)
	{
		tvWinList = cTvWinList;
	}
	CCmsTvWinList& GetTvWinList()
	{
		return tvWinList;
	}
	const CCmsTvWinList& GetTvWinList() const
	{
		return tvWinList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef    vector<TCmsTvWallScheme>    CCmsTvWallSchemeList;

//窗口分画面码流类型
typedef string TCmsWinDivMediaType;
#define   WIN_MEDIA_SERVICE_REAL_MEDIAL       "real_media"
#define   WIN_MEDIA_SERVICE_PLAT_REC          "plat_rec"
#define   WIN_MEDIA_SERVICE_PU_REC            "pu_rec"

struct TCmsTvWinDivState
{
public:
	TCmsTvWinDivState()
	{
		Clear();
	}

	~TCmsTvWinDivState()
	{
		Clear();
	}

	void Clear()
	{
		tvDivId = 0;
		tvChnVidSrc.Clear();
		playState = 0;
		winPollState = STRING_NULL;
		mediaService = STRING_NULL;
		recToken = STRING_NULL;
		timeRange.Clear();
		encChn.Clear();
	}

private:
	int              tvDivId;                   //电视分画面ID
	TCmsChannel      tvChnVidSrc;				//电视播放的视频源通道（通常是PU, 放像时是放像session）
	int              playState;                 //当前视频源的播放状态：成功:CMS_SUCCESS，失败：错误码
	string           winPollState;              //轮询状态
	TCmsWinDivMediaType mediaService;           //媒体服务类型

	//当源是录像时，需要以下参数
	TCmsRecToken     recToken;                   //录像Token
	TCmsTimeRange    timeRange;                  //录像时间段
	TCmsChannel      encChn;                     //录像编码器通道

public:
	void SetWinPollState(const string& strWinPollState)
	{
		winPollState = strWinPollState;
	}
	string& GetWinPollState()
	{
		return winPollState;
	}
	const string& GetWinPollState() const
	{
		return winPollState;
	}

	void SetPlayState(int nPlayState)
	{
		playState = nPlayState;
	}
	int GetPlayState() const
	{
		return playState;
	}

	void SetMediaService(const string& strMediaService)
	{
		mediaService = strMediaService;
	}
	string& GetMediaService()
	{
		return mediaService;
	}
	const string& GetMediaService() const
	{
		return mediaService;
	}

	void SetRecToken(const TCmsRecToken& cRecToken)
	{
		recToken = cRecToken;
	}
	TCmsRecToken& GetRecToken()
	{
		return recToken;
	}
	const TCmsRecToken& GetRecToken() const
	{
		return recToken;
	}

	void SetTimeRange(const TCmsTimeRange& cTimeRange)
	{
		timeRange = cTimeRange;
	}
	TCmsTimeRange& GetTimeRange()
	{
		return timeRange;
	}
	const TCmsTimeRange& GetTimeRange() const
	{
		return timeRange;
	}

	void SetEncChn(const TCmsChannel& cEncChn)
	{
		encChn = cEncChn;
	}
	TCmsChannel& GetEncChn()
	{
		return encChn;
	}
	const TCmsChannel& GetEncChn() const
	{
		return encChn;
	}

	void SetTvDivId(int nTvDivId)
	{
		tvDivId = nTvDivId;
	}
	int GetTvDivId() const
	{
		return tvDivId;
	}

	void SetTvChnVidSrc(const TCmsChannel& cTvChnVidSrc)
	{
		tvChnVidSrc = cTvChnVidSrc;
	}
	TCmsChannel& GetTvChnVidSrc()
	{
		return tvChnVidSrc;
	}
	const TCmsChannel& GetTvChnVidSrc() const
	{
		return tvChnVidSrc;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef     vector<TCmsTvWinDivState>      TCmsTvWinDivStateList;

struct TCmsTvWinState
{
public:
	TCmsTvWinState()
	{
		Clear();
	}

	~TCmsTvWinState()
	{
		Clear();
	}

	void Clear()
	{
		tvId = 0;
		divStateList.clear();
	}

private:
	int                   tvId;                              //电视机id
	TCmsTvWinDivStateList    divStateList;                      //各个分画面的状态
public:

	void SetDivStateList(const TCmsTvWinDivStateList& cDivStateList)
	{
		divStateList = cDivStateList;
	}
	TCmsTvWinDivStateList& GetDivStateList()
	{
		return divStateList;
	}
	const TCmsTvWinDivStateList& GetDivStateList() const
	{
		return divStateList;
	}

	void SetTvId(int nTvId)
	{
		tvId = nTvId;
	}
	int GetTvId() const
	{
		return tvId;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef     vector<TCmsTvWinState> CCmsTvWinStateList;

typedef string TCmsPollState;
#define CMS_POLL_STATE_START						"start"
#define CMS_POLL_STATE_PAUSE						"pause"
#define CMS_POLL_STATE_STOP							"stop"

struct TCmsTvWallState
{
public:
	TCmsTvWallState()
	{
		Clear();
	}

	~TCmsTvWallState()
	{
		Clear();
	}

	void Clear()
	{
		schemeGroup.Clear();
		schemePollState = STRING_NULL;
		curScheme.Clear();
		winStateList.clear();
	}

private:
	TCmsTvWallSchemeGroup	schemeGroup;					//预案组
	TCmsPollState			schemePollState;				//预案轮询状态
	TCmsTvWallScheme		curScheme;						//当前预案
	CCmsTvWinStateList		winStateList;					//当前预案每个窗口的状态  

	//结合编码器对应的编码器，cu可完整复现tvwall的当前状态
public:
	void SetSchemeGroup(const TCmsTvWallSchemeGroup& cSchemeGroup)
	{
		schemeGroup = cSchemeGroup;
	}
	TCmsTvWallSchemeGroup& GetSchemeGroup()
	{
		return schemeGroup;
	}
	const TCmsTvWallSchemeGroup& GetSchemeGroup() const
	{
		return schemeGroup;
	}

	void SetSchemePollState(const TCmsPollState& strSchemePollState)
	{
		schemePollState = strSchemePollState;
	}
	TCmsPollState& GetSchemePollState()
	{
		return schemePollState;
	}
	const TCmsPollState& GetSchemePollState() const
	{
		return schemePollState;
	}

	void SetCurScheme(const TCmsTvWallScheme& cCurScheme)
	{
		curScheme = cCurScheme;
	}
	TCmsTvWallScheme& GetCurScheme()
	{
		return curScheme;
	}
	const TCmsTvWallScheme& GetCurScheme() const
	{
		return curScheme;
	}

	void SetWinStateList(const CCmsTvWinStateList& cWinStateList)
	{
		winStateList = cWinStateList;
	}
	CCmsTvWinStateList& GetWinStateList()
	{
		return winStateList;
	}
	const CCmsTvWinStateList& GetWinStateList() const
	{
		return winStateList;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//todo by xiongxh 对应的宏前面应该加上CMS_
typedef string TCmsPuTransOprType;
#define PU_TRANS_OPR_SEND_COMDATA       "SendComData"       //发送串口数据
#define PU_TRANS_OPR_EXT_PTZ_CTRL       "ExtPtzCtrl"        //扩展PTZ操作
#define PU_TRANS_OPR_SEND_EXT_DATA      "SendExtData"       //发送透明数据

//透明数据
typedef  string TCmsPuTransDataType;
#define CMS_PU_TRANS_DATA_TYPE_COMDATA       "ComData"       //串口数据


struct TCmsTransparentData
{
public:
	TCmsTransparentData()
	{
		Clear();
	}

	~TCmsTransparentData()
	{
		Clear();
	}

	void Clear()
	{
		type = STRING_NULL;
		data = STRING_NULL;
	}

private:
	TCmsPuTransDataType	type;                   //透明数据类型
	string				data;                   //透明数据
public:
	void SetType(const TCmsPuTransDataType& strType)
	{
		type = strType;
	}
	TCmsPuTransDataType& GetType()
	{
		return type;
	}
	const TCmsPuTransDataType& GetType() const
	{
		return type;
	}

	void SetData(const string& strData)
	{
		data = strData;
	}
	string& GetData()
	{
		return data;
	}
	const string& GetData() const
	{
		return data;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//---------------------------------------------------------------------------------
//                                uas功能相关部分
//---------------------------------------------------------------------------------
struct TCmsPresetPositionName
{
public:
	TCmsPresetPositionName()
	{
		Clear();
	}

	~TCmsPresetPositionName()
	{
		Clear();
	}

	void Clear()
	{
		posId = 0;
		posName = STRING_NULL;
	}

	int GetPosId() const
	{
		return posId;
	}

	void SetPosId(int val)
	{
		posId = val;
	}

	const string &GetPosName() const
	{
		return posName;
	}
	void SetPosName(const string &val)
	{
		posName = val;
	}

private:
	int posId;      // 预置位号
	string posName; // 预置位别名
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsVideosourcePresetCfgInfo
{
public:
	TCmsVideosourcePresetCfgInfo()
	{
		Clear();
	}

	~TCmsVideosourcePresetCfgInfo()
	{
		Clear();
	}

	void Clear()
	{
		devUri = STRING_NULL;
		videosourceId = 0;
		posNames.clear();
	}

	const string &GetDevUri() const
	{
		return devUri;
	}
	void SetDevUri(const string &val)
	{
		devUri = val;
	}

	int GetVideosourceId() const
	{
		return videosourceId;
	}
	void SetVideosourceId(int val)
	{
		videosourceId = val;
	}

	const vector<TCmsPresetPositionName> &GetPosNames() const
	{
		return posNames;
	}
	void SetPosNames(const vector<TCmsPresetPositionName> &val)
	{
		posNames = val;
	}

private:
	string devUri;          // 设备Uri
	int videosourceId;      // 视频源号
	vector<TCmsPresetPositionName> posNames; // 预置位别名
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsVideosourcePresetCfgInfoKey
{             
public:
	CCmsVideosourcePresetCfgInfoKey()
	{
		Clear();
	}

	~CCmsVideosourcePresetCfgInfoKey()
	{
		Clear();
	}

	void Clear()
	{
		devUri = STRING_NULL;
		videosourceId = 0;
		positionId = 0;
	}

	void SetDevUri(const string& strDevId)
	{
		devUri = strDevId;
	}
	string& GetDevUri()
	{
		return devUri;
	}
	const string& GetDevUri() const
	{
		return devUri;
	}

	void SetVideosourceId(int nVideosourceId)
	{
		videosourceId = nVideosourceId;
	}
	int GetVideosourceId() const
	{
		return videosourceId;
	}

	void SetPositionId(int nPositionId)
	{
		positionId = nPositionId;
	}
	int GetPositionId() const
	{
		return positionId;
	}

private:
	string devUri;              
	int videosourceId;                 
	int positionId; 
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 查询有效字段标识类
class CCmsQryOptionField
{
public:
    CCmsQryOptionField(){}
    void SetOption(const string &opt)
    {
        if (fields.end() == find(fields.begin(), fields.end(), opt))
            fields.push_back(opt);
    }
    bool HasOption(const string &opt) const
    {
        return fields.end() != find(fields.begin(), fields.end(), opt);
    }
private:
    vector<string> fields;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

const int CMS_CUSER_TYPE_SUPERADMIN   = 0;   // 超级管理员
const int CMS_CUSER_TYPE_COMMONADMIN  = 1;   // 管理员
const int CMS_CUSER_TYPE_OPERATOR     = 2;   // 操作员 CU用户类型

class CCmsUserInfoQueryCondition
{
public:
    CCmsUserInfoQueryCondition()
    {
        type = CMS_CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
    }
    bool GetCuserID(string &val) const
    {
        if (!optionField.HasOption("cuserID"))
            return false;
        val = cuserID;
        return true;
    }
    void SetCuserID(const string &val)
    {
        cuserID = val;
        optionField.SetOption("cuserID");
    }
    bool GetUserUri(string &val) const
    {
        if (!optionField.HasOption("uri"))
            return false;
        val = uri;
        return true;
    }
    void SetUserUri(const string &val)
    {
        uri = val;
        optionField.SetOption("uri");
    }
    bool GetUserName(string &val) const
    {
        if (!optionField.HasOption("name"))
            return false;
        val = name;
        return true;
    }
    void SetUserName(const string &val)
    {
        name = val;
        optionField.SetOption("name");
    }
    bool GetUserPwd(string &val) const
    {
        if (!optionField.HasOption("password"))
            return false;
        val = password;
        return true;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
        optionField.SetOption("password");
    }
    bool GetUserType(int &val) const
    {
        if (!optionField.HasOption("type"))
            return false;
        val = type;
        return true;
    }
    void SetUserType(int val)
    {
        type = val;
        optionField.SetOption("type");
    }
    bool GetMLoginFlag(bool &val) const
    {
        if (!optionField.HasOption("mLoginFlag"))
            return false;
        val = mLoginFlag;
        return true;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
        optionField.SetOption("mLoginFlag");
    }
    bool GetEnabledFlag(bool &val) const
    {
        if (!optionField.HasOption("enabledFlag"))
            return false;
        val = enabledFlag;
        return true;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
        optionField.SetOption("enabledFlag");
    }
    bool GetDescription(string &val) const
    {
        if (!optionField.HasOption("description"))
            return false;
        val = description;
        return true;
    }
    void SetDescription(const string &val)
    {
        description = val;
        optionField.SetOption("description");
    }
    bool GetCreatorNO(string &val) const
    {
        if (!optionField.HasOption("creatorNO"))
            return false;
        val = creatorNO;
        return true;
    }
    void SetCreatorNO(const string &val)
    {
        creatorNO = val;
        optionField.SetOption("creatorNO");
    }
    bool GetNameExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_From"))
            return false;
        val = nameExpireDate_From;
        return true;
    }
    void SetNameExpireDate_From(const string &val)
    {
        nameExpireDate_From = val;
        optionField.SetOption("nameExpireDate_From");
    }
    bool GetNameExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("nameExpireDate_To"))
            return false;
        val = nameExpireDate_To;
        return true;
    }
    void SetNameExpireDate_To(const string &val)
    {
        nameExpireDate_To = val;
        optionField.SetOption("nameExpireDate_To");
    }
    bool GetPwdExpireDate_From(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_From"))
            return false;
        val = pwdExpireDate_From;
        return true;
    }
    void SetPwdExpireDate_From(const string &val)
    {
        pwdExpireDate_From = val;
        optionField.SetOption("pwdExpireDate_From");
    }
    bool GetPwdExpireDate_To(string &val) const
    {
        if (!optionField.HasOption("pwdExpireDate_To"))
            return false;
        val = pwdExpireDate_To;
        return true;
    }
    void SetPwdExpireDate_To(const string &val)
    {
        pwdExpireDate_To = val;
        optionField.SetOption("pwdExpireDate_To");
    }
    bool GetCreateDate_From(string &val) const
    {
        if (!optionField.HasOption("createDate_From"))
            return false;
        val = createDate_From;
        return true;
    }
    void SetCreateDate_From(const string &val)
    {
        createDate_From = val;
        optionField.SetOption("createDate_From");
    }
    bool GetCreateDate_To(string &val) const
    {
        if (!optionField.HasOption("createDate_To"))
            return false;
        val = createDate_To;
        return true;
    }
    void SetCreateDate_To(const string &val)
    {
        createDate_To = val;
        optionField.SetOption("createDate_To");
    }
private:
    CCmsQryOptionField optionField; // 查询字段标识
    string cuserID;          // 发起查询的CU用户编号，指定时能够限制返回的数据为该用户权限范围内的(目前指定该字段时不支持下面的条件过滤!!!)

    string uri; // 用户编号
    string name; // 用户名
    string password; // 登陆密码, 密文形式
    int type; // CU用户类型：参见常量定义
    bool mLoginFlag; // 多次登陆标志
    bool enabledFlag; // 启停状态标志
    string description; // 描述
    string creatorNO; // 创建者编号
    string nameExpireDate_From; //帐户使用截止日期_查询起点
    string nameExpireDate_To; //帐户使用截止日期_查询终点
    string pwdExpireDate_From; //密码使用截止日期_查询起点
    string pwdExpireDate_To; //密码使用截止日期_查询终点
    string createDate_From; //帐户创建日期_查询起点
    string createDate_To; //帐户创建日期_查询终点
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsBitsContainer
{
public:
    CCmsBitsContainer(){}
    void SetBit(unsigned int dwIndex, bool bOn = true)
    {
        char ch = bOn? '1':'0';

        if ((int)bits.size() <= dwIndex)
        {
            bits.resize(dwIndex+1, '0');
        }

        bits[dwIndex] = ch;
    }
    bool IsBitOn(unsigned int dwIndex) const
    {
        if ((int)bits.size() <= dwIndex)
        {
            return false;
        }
        else
        {
            return bits[dwIndex] == '1';
        }
    }
    int Convert2Int() const
    {
        unsigned int dwVal = 0;
        for (unsigned int i = 0; i < bits.size(); ++i)
        {
            if (bits[i] == '1') dwVal |= (0x1 << i);
        }
        return dwVal;
    }
    void ConvertFromInt(unsigned int dwVal)
    {
        bits = STRING_NULL;

        unsigned int dwMask = 0x1;
        for (unsigned int i = 0; i < 32; ++i)
        {
            if (dwVal & (dwMask<<i))
            {
                SetBit(i);
            }
        }
    }
    void ConvertFromBitsContainer(const CCmsBitsContainer &val)
    {
        bits = val.bits;
    }
    void Clear()
    {
        bits = STRING_NULL;
    }
    bool IsSuperSet(const CCmsBitsContainer &val) // 判断是否为val的超集
    {
        size_t i;

        size_t size = bits.size()<val.bits.size()? bits.size():val.bits.size();
        for (i = 0; i < size; ++i)
        {
            if (bits[i] < val.bits[i]) return false;
        }
        for (; i < val.bits.size(); ++i)
        {
            if (val.bits[i] == '1') return false; 
        }

        return true;
    }
private:
    string bits; // 位有效时相应位数值为1，否则为0. 此成员不要公布给外部

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsUserPrivilege: public CCmsBitsContainer
{ // cu用户权限位
public:
    CCmsUserPrivilege(){}
    enum TCmsPrivilege
    {
        en_Vod_Rec              = 1, // 平台录像 第一位
        en_Vod_Play             = 2, // 平台放像
        en_Vod_Del              = 3, // 平台录像删除
        en_Vod_Download         = 4, // 平台录像下载
        en_Vod_Local_Rec        = 5, // 本地录像，本地抓拍
        en_Rec_Query_Remote     = 6, // 跨域录像查询

        en_Call_Ctrl            = 7,  // 呼叫控制
        en_Alarm_Manage         = 8,  // 现场告警管理

        // 下面两权限暂时用不到, 先注释掉
        //en_Explore              = 9,  // 视频浏览权限
        //en_Cycle_Explore        = 10, // 轮巡权限
    };
    void SetPrivilege(CCmsUserPrivilege::TCmsPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CCmsUserPrivilege::TCmsPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsUserInfo
{
public:
    CCmsUserInfo()
    {
        type = CMS_CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
    }

    void Clear()
    {
        uri = STRING_NULL;
        grpUri = STRING_NULL;
        name = STRING_NULL;
        password = STRING_NULL;
        type = CMS_CUSER_TYPE_SUPERADMIN;
        mLoginFlag = true;
        enabledFlag = true;
        description = STRING_NULL;
        nameExpireDate = STRING_NULL;
        pwdExpireDate = STRING_NULL;
        createDate = STRING_NULL;
        creatorUri = STRING_NULL;
        privilege.Clear();
        deviceTreeVer = STRING_NULL;
    }

    string& GetUserUri() 
    {
        return uri;
    }
    const string& GetUserUri() const
    {
        return uri;
    }
    void SetUserUri(const string &val)
    {
        uri = val;
    }
    string& GetUsergrpUri() 
    {
        return grpUri;
    }
    const string& GetUsergrpUri() const
    {
        return grpUri;
    }
    void SetUsergrpUri(const string &val)
    {
        grpUri = val;
    }
    string& GetUserName() 
    {
        return name;
    }
    const string& GetUserName() const
    {
        return name;
    }
    void SetUserName(const string &val)
    {
        name = val;
    }
    string& GetUserPwd() 
    {
        return password;
    }
    const string& GetUserPwd() const
    {
        return password;
    }
    void SetUserPwd(const string &val)
    {
        password = val;
    }
    int GetUserType() 
    {
        return type;
    }
    int GetUserType() const
    {
        return type;
    }
    void SetUserType(int val)
    {
        type = val;
    }
    bool GetMLoginFlag() 
    {
        return mLoginFlag;
    }
    const bool GetMLoginFlag() const
    {
        return mLoginFlag;
    }
    void SetMLoginFlag(bool val)
    {
        mLoginFlag = val;
    }
    bool GetEnabledFlag() 
    {
        return enabledFlag;
    }
    bool GetEnabledFlag() const
    {
        return enabledFlag;
    }
    void SetEnabledFlag(bool val)
    {
        enabledFlag = val;
    }
    string& GetDescription()
    {
        return description;
    }
    const string& GetDescription() const
    {
        return description;
    }
    void SetDescription(const string &val)
    {
        description = val;
    }
    string& GetNameExpireDate() 
    {
        return nameExpireDate;
    }
    const string& GetNameExpireDate() const
    {
        return nameExpireDate;
    }
    void SetNameExpireDate(const string &val)
    {
        nameExpireDate = val;
    }
    string& GetPwdExpireDate() 
    {
        return pwdExpireDate;
    }
    const string& GetPwdExpireDate() const
    {
        return pwdExpireDate;
    }
    void SetPwdExpireDate(const string &val)
    {
        pwdExpireDate = val;
    }
    string& GetCreateDate() 
    {
        return createDate;
    }
    const string& GetCreateDate() const
    {
        return createDate;
    }
    void SetCreateDate(const string &val)
    {
        createDate = val;
    }
    string& GetCreatorUri()
    {
        return creatorUri;
    }
    const string& GetCreatorUri() const
    {
        return creatorUri;
    }
    void SetCreatorUri(const string &val)
    {
        creatorUri = val;
    }
    CCmsUserPrivilege& GetUserPrivilege() 
    {
        return privilege;
    }
    const CCmsUserPrivilege& GetUserPrivilege() const
    {
        return privilege;
    }
    void SetUserPrivilege(const CCmsUserPrivilege &val)
    {
        privilege = val;
    }
    void SetDeviceTreeVer(const string& strDeviceTreeVer)
    {
        deviceTreeVer = strDeviceTreeVer;
    }
    string& GetDeviceTreeVer()
    {
        return deviceTreeVer;
    }
    const string& GetDeviceTreeVer() const
    {
        return deviceTreeVer;
    }
private:
    string uri;    // 用户编号
    string grpUri; // 用户所属组
    string name;     // 用户名
    string password; // 登陆密码, 密文形式
    int type;        // CU用户类型：参见常量定义
    bool mLoginFlag; // 多次登陆标志, 1表示允许多人登录
    bool enabledFlag;   // 启停状态标志
    string description; // 描述
    string nameExpireDate; // 帐户使用截止日期
    string pwdExpireDate;  // 密码使用截止日期
    string createDate;    // 创建日期
    string creatorUri; // 创建者编号
    CCmsUserPrivilege privilege;
    string deviceTreeVer; // 设备树版本号
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsUserDeviceInfoQueryCondition
{
public:
    CCmsUserDeviceInfoQueryCondition()
    {
        capID = CMS_DEVICE_CAP_INVALID;
        capIndex = -1;
    }
    bool GetUserUri(string &val) const
    {
        if (!optionField.HasOption("userUri"))
            return false;
        val = userUri;
        return true;
    }
    void SetUserUri(const string &val)
    {
        userUri = val;
        optionField.SetOption("userUri");
    }
    bool GetDeviceUri(string &val) const
    {
        if (!optionField.HasOption("deviceUri"))
            return false;
        val = deviceUri;
        return true;
    }
    void SetDeviceUri(const string &val)
    {
        deviceUri = val;
        optionField.SetOption("deviceUri");
    }
    bool GetCapID(int &val) const
    {
        if (!optionField.HasOption("capID"))
            return false;
        val = capID;
        return true;
    }
    void SetCapID(int val)
    {
        capID = val;
        optionField.SetOption("capID");
    }
    bool GetCapIndex(int &val) const
    {
        if (!optionField.HasOption("capIndex"))
            return false;
        val = capIndex;
        return true;
    }
    void SetCapIndex(int val)
    {
        capIndex = val;
        optionField.SetOption("capIndex");
    }

    bool GetDevGrpUri(string &val) const
    {
        if (!optionField.HasOption("devGrpUri"))
            return false;
        val = devGrpUri;
        return true;
    }
    void SetDevGrpUri(const string &val)
    {
        devGrpUri = val;
        optionField.SetOption("devGrpUri");
    }
private:
    CCmsQryOptionField optionField; // 查询字段标识
    string userUri;
    string deviceUri;
    int capID;    // 设备能力ID
    int capIndex; // 设备能力索引

    string devGrpUri; // 设备所属组限制

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsEncoderPrivilege: public CCmsBitsContainer
{ // 编码器权限
public:
    CCmsEncoderPrivilege(){}
    enum TCmsPrivilege
    {
        en_Vod_Rec              = 1, // 前端录像
        en_Vod_Play             = 2, // 前端放像
        en_Vod_Del              = 3, // 前端录像删除
        en_Vod_Download         = 4, // 前端录像下载

        en_Param_Config         = 5, // 前端参数设置
        en_IO_Ctrl              = 6, // 前端开关量输出控制
        en_Defence_Ctrl         = 7, // 布防/撤防
    };
    void SetPrivilege(CCmsEncoderPrivilege::TCmsPrivilege pri)
    {
        SetBit(pri-1);
    }
    bool HasPrivilege(CCmsEncoderPrivilege::TCmsPrivilege pri) const
    {
        return IsBitOn(pri-1);
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

// 设备权限位
class CCmsDevicePrivilege: public CCmsBitsContainer
{
public:
    CCmsDevicePrivilege(){}
    CCmsEncoderPrivilege GetEncoderPrivilege() 
    {
        CCmsEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
        return val;
    }
    CCmsEncoderPrivilege GetEncoderPrivilege() const
    {
        CCmsEncoderPrivilege val;
        val.ConvertFromBitsContainer(*this);
        return val;
    }
    void SetEncoderPrivilege(const CCmsEncoderPrivilege &val)
    {
        ConvertFromBitsContainer(val);
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsDeviceCapPrivilege
{
    TCmsDeviceCapPrivilege()
    {
        id = CMS_DEVICE_CAP_INVALID;
    }

    int id;  // 能力标识
    map<int, CCmsBitsContainer> privileges; // 能力索引权限, key为能力索引, value为权限数值

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsUserDeviceInfo
{
public:
    CCmsUserDeviceInfo(){}

    void Clear()
    {
        userUri = STRING_NULL;
        deviceUri = STRING_NULL;
        devDomainID = STRING_NULL;
        devicePrivilege.Clear();
        deviceCapPrivilege.clear();
    }

    string& GetUserUri()
    {
        return userUri;
    }
    const string& GetUserUri() const
    {
        return userUri;
    }
    void SetUserUri(const string &val)
    {
        userUri = val;
    }
    string& GetDeviceUri() 
    {
        return deviceUri;
    }
    const string& GetDeviceUri() const
    {
        return deviceUri;
    }
    void SetDeviceUri(const string &val)
    {
        deviceUri = val;
    }
    string& GetDevDomainID()
    {
        return devDomainID;
    }
    const string& GetDevDomainID() const
    {
        return devDomainID;
    }
    void SetDevDomainID(const string& val)
    {
        devDomainID = val;
    }

    CCmsDevicePrivilege& GetDevicePrivilege() 
    {
        return devicePrivilege;
    }
    const CCmsDevicePrivilege& GetDevicePrivilege() const
    {
        return devicePrivilege;
    }
    void SetDevicePrivilege(const CCmsDevicePrivilege &val)
    {
        devicePrivilege = val;
    }
    vector<TCmsDeviceCapPrivilege>& GetDeviceCapPrivilege()
    {
        return deviceCapPrivilege;
    }
    const vector<TCmsDeviceCapPrivilege>& GetDeviceCapPrivilege() const
    {
        return deviceCapPrivilege;
    }
    void SetDeviceCapPrivilege(const vector<TCmsDeviceCapPrivilege>& val)
    {
        deviceCapPrivilege = val;
    }

private:
    string userUri;
    string deviceUri;
    string devDomainID; // 设备所属域Id, 发给客户端时TAS拼装出来
    CCmsDevicePrivilege devicePrivilege;
    vector<TCmsDeviceCapPrivilege> deviceCapPrivilege;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//用户操作日志
enum TCmsOperateType
{
    enCmsUserOpTypeInvalid = 0, 
    enCmsUserOpTypeLogin = 1, 
    enCmsUserOpTypeLiveStream = 2, 
    enCmsUserOpTypePlatformRecord = 3, // 废弃
    enCmsUserOpTypePuRecord = 4,       // 废弃
    enCmsUserOpTypeTVWall = 5, 
    enCmsUserOpTypeCall = 6, 
    enCmsUserOpTypeUserManage = 7, 
    enCmsUserOpTypeNRUManage =8, 
    enCmsUserOpTypeMapConfig = 9, 
    enCmsUserOpTypePuConfig = 10, 
    enCmsUserOpTypePtzCtrl = 11,
    enCmsUserOpTypeAlarmConfig = 12,
    enCmsUserOpTypeSysParamConfig = 13,
    enCmsUserOpTypeRecordStart = 15,
    enCmsUserOpTypeRecordStop  = 16,
    enCmsUserOpTypeRecordPlayback = 17,
    enCmsUserOpTypeRecordDownload = 18,
    enCmsUserOpTypeCuLocalRecord  = 19,

    enCmsUserOpTypeOther = 14
};


struct TCmsOperateLogInfo
{
    string logId;     // 日志编号
    string logTag;    // 日志标签(可以满足同一用户的不同次登录日志区别开来的应用需求)
    string userUri;       // 用户Uri
    string userName;      // 用户名
    string happenedTime;  // 发生时间
    int operateType;      // 操作类型
    int operateResult;    // 操作结果
    string netAddr;       // 网络地址(用户登录的IP地址信息)
    string operateDesc;   // 操作描述

public:
    TCmsOperateLogInfo()
    {
        operateType   = enCmsUserOpTypeInvalid;
        operateResult = 0;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

class CCmsOperateLogInfoQueryCondition
{
public:
    bool GetLogTag(string &val) const
    {
        if (!optionField.HasOption("logTag"))
            return false;
        val = logTag;
        return true;
    }
    void SetLogTag(const string &val)
    {
        logTag = val;
        optionField.SetOption("logTag");
    }
    bool GetUserUri(string &val) const
    {
        if (!optionField.HasOption("userUri"))
            return false;
        val = userUri;
        return true;
    }
    void SetUserUri(const string &val)
    {
        userUri = val;
        optionField.SetOption("userUri");
    }
    bool GetDomainName(string &val) const
    {
        if (!optionField.HasOption("domainName"))
            return false;
        val = domainName;
        return true;
    }
    void SetDomainName(const string &val)
    {
        domainName = val;
        optionField.SetOption("domainName");
    }
    bool GetHappenedTimeBegin(string &val) const
    {
        if (!optionField.HasOption("happenedTimeBegin"))
            return false;
        val = happenedTimeBegin;
        return true;
    }
    void SetHappenedTimeBegin(const string &val)
    {
        happenedTimeBegin = val;
        optionField.SetOption("happenedTimeBegin");
    }
    bool GetHappenedTimeEnd(string &val) const
    {
        if (!optionField.HasOption("happenedTimeEnd"))
            return false;
        val = happenedTimeEnd;
        return true;
    }
    void SetHappenedTimeEnd(const string &val)
    {
        happenedTimeEnd = val;
        optionField.SetOption("happenedTimeEnd");
    }
    bool GetOperateType(int &val) const
    {
        if (!optionField.HasOption("operateType"))
            return false;
        val = operateType;
        return true;
    }
    void SetOperateType(int val)
    {
        operateType = val;
        optionField.SetOption("operateType");
    }
private:
    CCmsQryOptionField optionField;

    string logTag;
    string userUri;
    string domainName;
    string happenedTimeBegin;
    string happenedTimeEnd;
    int operateType;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


struct TCmsRecordParam
{
private:
    TCmsChannel        puChn;
    TCmsRecMode        recMode;            //录像模式：既录视频又录音频,只录视频,只录关键帧
    int             preRecDuration;     //预录时长。0 表示不预录。单位:s
    int             alarmRecDelayTime;  //告警恢复后，延迟停止录像时间。单位:s
    bool            isManualRecOn;      //注意：外部设置录像参数时此参数无效
    TCmsCronTimeList   planTimeList;

public:
    TCmsRecordParam()
    {
        Clear();
        recMode = CMS_RCS_RECMODE_ALL;
    }

    void Clear()
    {
        puChn.Clear();
        recMode = STRING_NULL;
        planTimeList.clear();
        preRecDuration = 0;
        alarmRecDelayTime = 60;
        isManualRecOn = false;
    }

public:
    //puChn
    const TCmsChannel& GetPuChn() const
    {
        return puChn;
    }
    TCmsChannel& GetPuChn()
    {
        return puChn;
    }
    void SetPuChn(const TCmsChannel& var)
    {
        puChn = var;
    }

    //recMode
    const TCmsRecMode& GetRecMode() const
    {
        return recMode;
    }
    TCmsRecMode& GetRecMode()
    {
        return recMode;
    }
    void SetRecMode(const TCmsRecMode& var)
    {
        recMode = var;
    }

    //preRecDuration
    int GetPreRecDuration() const
    {
        return preRecDuration;
    }
    void SetPreRecDuration(int var)
    {
        preRecDuration = var;
    }

    //alarmRecDelayTime
    int GetAlarmRecDelayStopTime() const
    {
        return alarmRecDelayTime;
    }
    void SetAlarmRecDelayStopTime(int var)
    {
        alarmRecDelayTime = var;
    }

    //isManualRecOn
    bool GetIsManualRecOn() const
    {
        return isManualRecOn;
    }
    void SetIsManualRecOn(bool var)
    {
        isManualRecOn = var;
    }

    //planTimeList
    const TCmsCronTimeList& GetPlanTimeList() const
    {
        return planTimeList;
    }
    TCmsCronTimeList& GetPlanTimeList()
    {
        return planTimeList;
    }
    void SetPlanTimeList(const TCmsCronTimeList& var)
    {
        planTimeList = var;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef vector<TCmsRecordParam> TCmsRecordParamList;

typedef map<string,string> TCmsFieldValueMap;        //查询的的条件字段，或 需要修改的字段

template<class T>
bool GetCmsFieldObjVar(const TCmsFieldValueMap& fieldValueMap, const string key, T& var)
{
    TCmsFieldValueMap::const_iterator itr = fieldValueMap.find(key);
    if ( itr!= fieldValueMap.end())
    {
        var.FromXml(itr->second);

        return true;
    }

    return false;
}

template<class T>
void SetCmsFieldObjVar(TCmsFieldValueMap& fieldValueMap, const string key, const T& var)
{
    fieldValueMap[key] = var.ToXml();
}

struct TCmsPuSnapShotCfg
{
public:
	TCmsPuSnapShotCfg()
	{
		Clear();
	}

	void Clear()
	{
		pinAlarmEnable = false;		//并口告警抓图使能 0关闭/1开启
		mtionDetectEnable = false;	//移动侦测抓图使能 0关闭/1开启	
		interval = 0;		
		maxSnapNum = 0;
	}
private:
	bool pinAlarmEnable;           //并口告警抓图使能 0关闭/1开启
	bool mtionDetectEnable;        //移动侦测抓图使能 0关闭/1开启
	int	interval;	               //抓拍时间间隔(ms)
	int maxSnapNum;		           //最大抓拍数量

public:
	void SetPinAlarmEnable(bool bPinAlarmEnable)
	{
		pinAlarmEnable = bPinAlarmEnable;
	}
	bool GetPinAlarmEnable() const
	{
		return pinAlarmEnable;
	}

	void SetMtionDetectEnable(bool bMtionDetectEnable)
	{
		mtionDetectEnable = bMtionDetectEnable;
	}
	bool GetMtionDetectEnable() const
	{
		return mtionDetectEnable;
	}

	void SetInterval(int nInterval)
	{
		interval = nInterval;
	}
	int GetInterval() const
	{
		return interval;
	}

	void SetMaxSnapNum(int nMaxSnapNum)
	{
		maxSnapNum = nMaxSnapNum;
	}
	int GetMaxSnapNum() const
	{
		return maxSnapNum;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsPuSerialPortParam
{	
public:
	TCmsPuSerialPortParam()
	{
		Clear();
	}

	void Clear()
	{
		serialPortID = 0;  
		baudRate = 0;	  //串口波特率
		comPortType = 0;  //串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
		rS232Enable = false;
		rS422Enable = false;
		rS485Enable = false;	
	}
private:
	int serialPortID;        //串口号
	int baudRate;            //串口波特率
	int comPortType;         //串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485

	bool rS232Enable;
	bool rS422Enable;
	bool rS485Enable;

public:
	void SetSerialPortID(int nSerialPortID)
	{
		serialPortID = nSerialPortID;
	}
	int GetSerialPortID() const
	{
		return serialPortID;
	}

	void SetBaudRate(int nBaudRate)
	{
		baudRate = nBaudRate;
	}
	int GetBaudRate() const
	{
		return baudRate;
	}

	void SetComPortType(int nComPortType)
	{
		comPortType = nComPortType;
	}
	int GetComPortType() const
	{
		return comPortType;
	}

	void SetRS232Enable(bool bRS232Enable)
	{
		rS232Enable = bRS232Enable;
	}
	bool GetRS232Enable() const
	{
		return rS232Enable;
	}

	void SetRS422Enable(bool bRS422Enable)
	{
		rS422Enable = bRS422Enable;
	}
	bool GetRS422Enable() const
	{
		return rS422Enable;
	}

	void SetRS485Enable(bool bRS485Enable)
	{
		rS485Enable = bRS485Enable;
	}
	bool GetRS485Enable() const
	{
		return rS485Enable;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//移动侦测参数结构
struct TCmsMotionDetectParam
{
    TCmsMotionDetectParam()
    {
        Clear();
    }
    void Clear()
    {
        enable = false;
        leftTopX = 0;
        leftTopY = 0;
        width = 0;
        height = 0;
        alarmOccurRate = 0;
        alarmRestoreRate = 0;
    }

private:
    bool enable;                                //侦测使能
    int  leftTopX;                              //侦测区域顶点横坐标
    int  leftTopY;                              //侦测区域顶点纵坐标
    int  width;                                 //侦测区域宽度
    int  height;                                //侦测区域高度
    int  alarmOccurRate;                        //侦测区域告警产生百分比（大于此临界值告警0-100)
    int  alarmRestoreRate;                      //产生告警后运动范围百分比小于该值则恢复告警(该值小于alarmOccurRate,0-100)
public:
    void SetEnable(bool bEnable)
    {
        enable = bEnable;
    }
    bool GetEnable() const
    {
        return enable;
    }

    void SetLeftTopX(int nLeftTopX)
    {
        leftTopX = nLeftTopX;
    }
    int GetLeftTopX() const
    {
        return leftTopX;
    }

    void SetLeftTopY(int nLeftTopY)
    {
        leftTopY = nLeftTopY;
    }
    int GetLeftTopY() const
    {
        return leftTopY;
    }

    void SetWidth(int nWidth)
    {
        width = nWidth;
    }
    int GetWidth() const
    {
        return width;
    }

    void SetHeight(int nHeight)
    {
        height = nHeight;
    }
    int GetHeight() const
    {
        return height;
    }

    void SetAlarmOccurRate(int nAlarmOccurRate)
    {
        alarmOccurRate = nAlarmOccurRate;
    }
    int GetAlarmOccurRate() const
    {
        return alarmOccurRate;
    }

    void SetAlarmRestoreRate(int nAlarmRestoreRate)
    {
        alarmRestoreRate = nAlarmRestoreRate;
    }
    int GetAlarmRestoreRate() const
    {
        return alarmRestoreRate;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef   vector<TCmsMotionDetectParam>     TCmsMotionDetectList;

//图像遮蔽参数结构
struct TCmsAreaShadeParam
{
    TCmsAreaShadeParam()
    {
        Clear();
    }
    void Clear()
    {
        enable = false;
        leftTopX = 0;
        leftTopY = 0;
        width = 0;
        height = 0;
    }

private:
    bool enable;                                             //遮蔽使能
    int  leftTopX;                                           //遮蔽区域顶点横坐标
    int  leftTopY;                                           //遮蔽区域顶点纵坐标
    int  width;                                              //遮蔽区域宽度
    int  height;                                             //遮蔽区域高度
public:
    void SetEnable(bool bEnable)
    {
        enable = bEnable;
    }
    bool GetEnable() const
    {
        return enable;
    }

    void SetLeftTopX(int nLeftTopX)
    {
        leftTopX = nLeftTopX;
    }
    int GetLeftTopX() const
    {
        return leftTopX;
    }

    void SetLeftTopY(int nLeftTopY)
    {
        leftTopY = nLeftTopY;
    }
    int GetLeftTopY() const
    {
        return leftTopY;
    }

    void SetWidth(int nWidth)
    {
        width = nWidth;
    }
    int GetWidth() const
    {
        return width;
    }

    void SetHeight(int nHeight)
    {
        height = nHeight;
    }
    int GetHeight() const
    {
        return height;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef   vector<TCmsAreaShadeParam>     TCmsAreaShadeList;

//告警联动
//告警源
struct TCmsAlarmLinkSrc
{
public:
    TCmsAlarmLinkSrc()
    {
        Clear();
    }

    void Clear()
    {
        inputId = 0;
        devUri = STRING_NULL;
        alarmType = STRING_NULL;
    }

    bool operator == ( const TCmsAlarmLinkSrc& tOther ) const
    {
        return (inputId == tOther.inputId 
            && devUri == tOther.devUri
            && alarmType == tOther.alarmType);
    }

    bool operator != ( const TCmsAlarmLinkSrc& tOther ) const
    {
        return !(operator ==(tOther));
    }

public:
    const string& GetDevUri() const
    {
        return devUri;
    }
    string& GetDevUri()
    {
        return devUri;
    }
    void SetDevUri(const string& strDevUri)
    {
        devUri = strDevUri;
    }

    const string& GetAlarmType() const
    {
        return alarmType;
    }
    string& GetAlarmType()
    {
        return alarmType;
    }
    void SetAlarmType(const string& strAlarmType)
    {
        alarmType = strAlarmType;
    }

    int GetInputId() const 
    {
        return inputId;
    }
    void SetInputId(int dwInputId) 
    {
        inputId = dwInputId;
    }

private:
    string       devUri;              //告警设备
    int          inputId;             //告警输入通道
    string       alarmType;           //告警类型

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//定义联动行为的类型
#define    CMS_ALARMLINK_PLAT_REC        "REC"
#define    CMS_ALARMLINK_TV_WALL         "TVWALL"
#define    CMS_ALARMLINK_READ_SEC        "READSEC"
#define    CMS_ALARMLINK_IO_OUT          "IOOUT"
#define    CMS_ALARMLINK_CAM             "CAM"

typedef set<string>  CCmsLinkActionList;

//告警目的通道和行为
typedef vector<string> CStrList;
struct TCmsAlarmLinkDst
{
public:
    TCmsAlarmLinkDst()
    {
        Clear();
    }

    void Clear()
    {
        encChn.Clear();
        linkActionList.clear();
        camPrePos = 0;
        tvWallId = STRING_NULL;
        tvId = 0;
        tvDivId = 0;
        alarmOutChn = 0;
        msgNoList.clear();
        vocNoList.clear();
        mailNoList.clear();
    }

public:
    const TCmsChannel& GetEncChn() const
    {
        return encChn;
    }
    TCmsChannel& GetEncChn()
    {
        return encChn;
    }
    void SetEncChn(const TCmsChannel& tChn)
    {
        encChn = tChn;
    }

    const CCmsLinkActionList& GetLinkActionList() const
    {
        return linkActionList;
    }
    CCmsLinkActionList& GetLinkActionList()
    {
        return linkActionList;
    }
    void SetLinkActionList(const CCmsLinkActionList& cLinkActionList)
    {
        linkActionList = cLinkActionList;
    }

    int GetCamPrePos() const 
    {
        return camPrePos;
    }
    void SetCamPrePos(int dwCamPrePos) 
    {
        camPrePos = dwCamPrePos;
    }

    //     const TChannel& GetDecChn() const
    //     {
    //         return decChn;
    //     }
    //     TChannel& GetDecChn()
    //     {
    //         return decChn;
    //     }
    //     void SetDecChn(const TChannel& tChn)
    //     {
    //         decChn = tChn;
    //     }

    const string& GetTvWallId() const
    {
        return tvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }

    //     int GetTvWallId() const 
    //     {
    //         return tvWallId;
    //     }
    //     void SetTvWallId(int dwTvWallId) 
    //     {
    //         tvWallId = dwTvWallId;
    //     }

    int GetTvId() const 
    {
        return tvId;
    }
    void SetTvId(int dwTvId) 
    {
        tvId = dwTvId;
    }

    int GetAlarmOutChn() const 
    {
        return alarmOutChn;
    }
    void SetAlarmOutChn(int dwAlarmOutChn) 
    {
        alarmOutChn = dwAlarmOutChn;
    }

private:
    TCmsChannel        encChn;            //告警联动目的编码通道
    CCmsLinkActionList linkActionList;    //联动行为列表

    //联动需要的资源
    //摄像头
    int             camPrePos;        //摄象头预置位

    //电视墙   
    //TChannel        decChn;            //解码器通道
    string          tvWallId;          //电视墙ID
    int             tvId;              //电视机ID
    int             tvDivId;                        //电视分画面ID

    //告警输出
    int             alarmOutChn;       //告警输出通道号

    //用户通知
    CStrList        msgNoList;                         //短信号码列表
    CStrList        vocNoList;                         //声讯号码列表
    CStrList        mailNoList;                        //电子邮箱列表

public:
    void SetTvDivId(int nTvDivId)
    {
        tvDivId = nTvDivId;
    }
    int GetTvDivId() const
    {
        return tvDivId;
    }

    void SetMsgNoList(const CStrList& cMsgNoList)
    {
        msgNoList = cMsgNoList;
    }
    CStrList& GetMsgNoList()
    {
        return msgNoList;
    }
    const CStrList& GetMsgNoList() const
    {
        return msgNoList;
    }

    void SetVocNoList(const CStrList& cVocNoList)
    {
        vocNoList = cVocNoList;
    }
    CStrList& GetVocNoList()
    {
        return vocNoList;
    }
    const CStrList& GetVocNoList() const
    {
        return vocNoList;
    }

    void SetMailNoList(const CStrList& cMailNoList)
    {
        mailNoList = cMailNoList;
    }
    CStrList& GetMailNoList()
    {
        return mailNoList;
    }
    const CStrList& GetMailNoList() const
    {
        return mailNoList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


//告警联动通用结构，包含联动和多前端联动
typedef vector<TCmsAlarmLinkDst>  CCmsAlarmLinkDstList;

struct TCmsAlarmLinkage
{
public:
    TCmsAlarmLinkage()
    {
        Clear();
    }
    void Clear()
    {
        alarmAlias = STRING_NULL;
        isUsed = false;
        defenceSched.clear();
        alarmSrc.Clear();
        alarmDstList.clear();
    }

public:

    const string& GetAlarmAlias() const
    {
        return alarmAlias;
    }
    string& GetAlarmAlias()
    {
        return alarmAlias;
    }
    void SetAlarmAlias(const string& strAlarmAlias)
    {
        alarmAlias = strAlarmAlias;
    }

    bool GetIsUsed() const
    {
        return isUsed;
    }
    void SetIsUsed(bool bIsUsed)
    {
        isUsed = bIsUsed;
    }

    const TCmsCronTimeList& GetDefenceSched() const
    {
        return defenceSched;
    }
    TCmsCronTimeList& GetDefenceSched()
    {
        return defenceSched;
    }
    void SetDefenceSched(const TCmsCronTimeList& tDefenceSched)
    {
        defenceSched = tDefenceSched;
    }

    const TCmsAlarmLinkSrc& GetAlarmLinkSrc() const
    {
        return alarmSrc;
    }
    TCmsAlarmLinkSrc& GetAlarmLinkSrc()
    {
        return alarmSrc;
    }
    void SetAlarmLinkSrc(const TCmsAlarmLinkSrc& tAlarmLinkSrc)
    {
        alarmSrc = tAlarmLinkSrc;
    }

    const CCmsAlarmLinkDstList& GetAlarmLinkDstList() const
    {
        return alarmDstList;
    }
    CCmsAlarmLinkDstList& GetAlarmLinkDstList()
    {
        return alarmDstList;
    }
    void SetAlarmLinkDstList(const CCmsAlarmLinkDstList& tAlarmLinkDstList)
    {
        alarmDstList = tAlarmLinkDstList;
    }

private:
    string                   alarmAlias;       //告警别名
    bool                     isUsed;           //联动是否启用，可以理解为是否布防
    TCmsCronTimeList            defenceSched;     //布防撤防时刻表,使用cronTime
    TCmsAlarmLinkSrc            alarmSrc;         //告警源
    CCmsAlarmLinkDstList        alarmDstList;     //联动列表
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TCmsGeneralData
{
public:
	TCmsGeneralData()
	{

	}

	string           type;

	// 这里必须要说明一下，平台的TGeneralData中的content类型是CTransparentData，
	// 这里之所以设置为string，是因为ConvertHelper支持CTransparentData类型，如果这里创建一个新的CCmsTransparentData类型，
	// 要么要修改ConvertHelper，要么手动编写CCmsTransparentData的ToXml()和FromXml()，还要修改CMS_AUTO以避免生成ToXml()和FromXml()，都挺麻烦的。
	// 这里使用string类型的话，由于string和CTransparentData的解析是一样的，所以也能获取到平台发送过来的正确结果，目前主要是这种应用场景，
	// 如果要向平台发送TCmsGeneralData数据的话，由于string和CTransparentData不同，会带来略微的性能下降，而且目前没有这种应用，故暂时设置为string类型。
	string           content;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TCmsAlarmLinkage>  CCmsAlarmLinkageList;

struct TCmsGpsSsParam
{
public:
	TCmsGpsSsParam()
	{
		Clear();
	}
	void Clear()
	{
		enable = false;
		type = 0;
		subType = 0;
		param = STRING_NULL;
	}

private:
	bool       enable;             	                   //是否订阅
	int        type;                                   //透明数据类型
	int        subType;                                //透明数据子类型
	string     param;                                  //gps订阅参数
public:
	void SetEnable(bool bEnable)
	{
		enable = bEnable;
	}
	bool GetEnable() const
	{
		return enable;
	}

	void SetType(int nType)
	{
		type = nType;
	}
	int GetType() const
	{
		return type;
	}

	void SetSubType(int nSubType)
	{
		subType = nSubType;
	}
	int GetSubType() const
	{
		return subType;
	}

	void SetParam(const string& strParam)
	{
		param = strParam;
	}
	string& GetParam()
	{
		return param;
	}
	const string& GetParam() const
	{
		return param;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

#endif // __CMS_SDK_STRUCT_H__


