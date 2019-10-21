#ifndef __CMU_STRUCT_H__
#define __CMU_STRUCT_H__

#include "cms/cms_struct.h"
#include <algorithm>
#include "cms/pu/vsipdefine.h"
#include "cms/security_module/security_module_proto.h"



//摄像头控制
struct TCamCtrl
{
private:
    int commandId; //命令类型;

    //各种命令的参数不同,详见命令参数表(见newcamera.h)
//     int param1; //参数1
//     int param2; //参数2
//     int param3; //参数3
//     int param4; //参数4
    vector<int> params;

    int priLevel; //控制云镜级别
    int holdTime; //控制占用时间(单位:秒);
    int matrixIn; //对于前置矩阵，该参数为矩阵接入前端的输入端口号;

public:
    TCamCtrl(void) 
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

struct TPuSysParam
{
private:
    bool isAudEnc;       //是否音频编码;
    int  directTransNum; //直传路数;

public:
    TPuSysParam() 
    {
        Clear();
    }

    void Clear()
    {
        isAudEnc = false;
        directTransNum = 0;
    }

    //获取是否音频编码
    bool GetIsAudEnc() const 
    {
        return isAudEnc;
    }
    //设置是否音频编码
    void SetIsAudEnc(const bool bIsAudEnc) 
    {
        isAudEnc = bIsAudEnc;
    }

    //获取直传路数
    int GetDirectTransNum() const 
    {
        return directTransNum;
    }
    //设置直传路数
    void SetDirectTransNum(int dwDirectTransNum) 
    {
        directTransNum = dwDirectTransNum;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//媒体payload定义
/*音频*/
#define	  MEDIA_PAYLOAD_MP3	                  (int)96  /*mp3 mode 0-4*/

#define   MEDIA_PAYLOAD_G7221C	              (int)98  /*G722.1.C Siren14*/

#define   MEDIA_PAYLOAD_PCMA		          (int)8   /*G.711 Alaw  mode 5*/

#define   MEDIA_PAYLOAD_PCMU		          (int)0   /*G.711 ulaw  mode 6*/

#define   MEDIA_PAYLOAD_G721		          (int)2   /*G.721*/

#define   MEDIA_PAYLOAD_G722		          (int)9   /*G.722*/

#define	  MEDIA_PAYLOAD_G7231		          (int)4   /*G.7231*/

#define   MEDIA_PAYLOAD_ADPCM                 (int)5   /*DVI4 ADPCM*/

#define	  MEDIA_PAYLOAD_G728		          (int)15  /*G.728*/

#define	  MEDIA_PAYLOAD_G729		          (int)18  /*G.729*/
#define   MEDIA_PAYLOAD_G7221                 (int)13  /*G.7221*/

#define   MEDIA_PAYLOAD_AACLC                 (int)102 /*AAC LC*/
#define   MEDIA_PAYLOAD_AACLD                 (int)103 /*AAC LD*/
#define   MEDIA_PAYLOAD_AACLC_PCM			  (int)104
#define   MEDIA_PAYLOAD_AMR                   (int)105

/*视频*/
#define   MEDIA_PAYLOAD_MP4	                  (int)97  /*MPEG-4*/
#define   MEDIA_PAYLOAD_H261	              (int)31  /*H.261*/
#define   MEDIA_PAYLOAD_H262	              (int)33  /*H.262 (MPEG-2)*/
#define   MEDIA_PAYLOAD_H263	              (int)34  /*H.263*/
#define   MEDIA_PAYLOAD_H263PLUS              (int)101 /*H.263+*/
#define   MEDIA_PAYLOAD_H264	              (int)106 /*H.264*/
#define   MEDIA_PAYLOAD_MJPEG	              (int)107 /*MJPEG*/
#define   MEDIA_PAYLOAD_SVAC	              (int)107 /*SVAC*/ 
#define   MEDIA_PAYLOAD_H265	              (int)111 /*H.265*/

//视频格式
#define VID_FMT_SN4         "SN4"
#define VID_FMT_MPEG4       "MPEG4"
#define VID_FMT_H261        "H261"
#define VID_FMT_H263        "H263"
#define VID_FMT_H264        "H264"
#define VID_FMT_JPEG        "JPEG"
#define VID_FMT_MJPEG       "MJPEG"
#define VID_FMT_H265        "H265"
#define VID_FMT_SVAC        "SVAC"

//视频分辨率
#define VID_RES_AUTO        "Auto"
#define VID_RES_QCIF        "QCIF"
#define VID_RES_CIF         "CIF"
#define VID_RES_2CIF        "2CIF"
#define VID_RES_4CIF        "4CIF"
#define VID_RES_QQCIF       "QQCIF"
#define VID_RES_VGA         "VGA"
#define VID_RES_QVGA        "QVGA"
#define VID_RES_720P        "720P"
#define VID_RES_960P        "960P"
#define VID_RES_1080P       "1080P"
#define VID_RES_QXGA        "QXGA"

//音频格式
#define AUD_FMT_UNKNOWN     "UNKNOWN"
#define AUD_FMT_PCMA        "PCMA"
#define AUD_FMT_PCMU        "PCMU"
#define AUD_FMT_GSM         "GSM"
#define AUD_FMT_G729        "G729"
#define AUD_FMT_ADPCM       "ADPCM"
#define AUD_FMT_G7221C      "G7221C"
#define AUDIO_FMT_G722      "G722"
#define AUDIO_FMT_AACLC     "AACLC"
#define AUDIO_FMT_G711      "G711"
#define AUDIO_FMT_G726      "G726"
#define AUDIO_FMT_AAC       "AAC"


//图象分辨率
#define VIDEO_RESOLUTION_NONE			0x00
#define VIDEO_RESOLUTION_AUTO			0x01
#define VIDEO_RESOLUTION_QCIF			0x02
#define VIDEO_RESOLUTION_CIF			0x03
#define VIDEO_RESOLUTION_2CIF			0x04
#define VIDEO_RESOLUTION_4CIF			0x05
#define VIDEO_RESOLUTION_QQCIF			0x06
#define VIDEO_RESOLUTION_VGA			0x07
#define VIDEO_RESOLUTION_QVGA			0x08 
#define VIDEO_RESOLUTION_720P           0x09
#define VIDEO_RESOLUTION_1080P          0x0A
#define VIDEO_RESOLUTION_QXGA			0x0B
#define VIDEO_RESOLUTION_UXGA			0x0C
#define VIDEO_RESOLUTION_QuadVGA		0x0D

#define  VIDEO_RESOLUTION_WIDTH_1920     1920
#define  VIDEO_RESOLUTION_WIDTH_1280     1280
#define  VIDEO_RESOLUTION_WIDTH_704      704
#define  VIDEO_RESOLUTION_WIDTH_352      352

#define  VIDEO_RESOLUTION_HEIGHT_1080    1080
#define  VIDEO_RESOLUTION_HEIGHT_960     960
#define  VIDEO_RESOLUTION_HEIGHT_720     720
#define  VIDEO_RESOLUTION_HEIGHT_480     480
#define  VIDEO_RESOLUTION_HEIGHT_288     288


typedef string CVidFormat;  //视频格式类型
typedef string CAudFormat;  //音频格式类型
typedef string CVidRes;     //视频分辨率类型
typedef set<CVidRes> CVidResList;
typedef u8 CVidType;

struct TVidType
{
private:
    CVidFormat    format;     //视频格式
    CVidResList   resSet;        //该格式支持的分辨率

public:
    void Clear()
    {
        format.clear();
        resSet.clear();
    }

    const CVidFormat& GetVidFormat() const
    {
        return format;
    }

    CVidFormat& GetVidFormat()
    {
        return format;
    }

    void SetVidFormat(const CVidFormat& strVidFormat)
    {
        format = strVidFormat;
    }

    const CVidResList& GetVidResSet() const
    {
        return resSet;
    }

    CVidResList& GetVidResSet()
    {
        return resSet;
    }

    void SetVidResSet(const CVidResList& strVidResSet)
    {
        resSet = strVidResSet;
    }

    void AddVidRes(const CVidRes& cVidRes)
    {
        resSet.insert(cVidRes);
    }

    void EraseVidRes(const CVidRes& cVidRes)
    {
        resSet.erase(cVidRes);
    }

    bool IsAuto() const 
    {
        return (resSet.find(VID_RES_AUTO) != resSet.end());
    }
    void SetAuto(bool bIsAuto) 
    {
        if(bIsAuto) 
        {
            resSet.insert(VID_RES_AUTO); 
        }
        else 
        {
            resSet.erase(VID_RES_AUTO); 
        }
    }

    bool IsQCIF() const 
    {
        return (resSet.find(VID_RES_QCIF) != resSet.end());
    }
    void SetQCIF(bool bIsQCIF) 
    {
        if(bIsQCIF) 
        {
            resSet.insert(VID_RES_QCIF);
        }
        else 
        {
            resSet.erase(VID_RES_QCIF); 
        }
    }

    bool IsCIF() const 
    {
        return (resSet.find(VID_RES_CIF) != resSet.end());
    }
    void SetCIF(bool bIsCIF) 
    {
        if(bIsCIF) 
        {
            resSet.insert(VID_RES_CIF);
        }
        else 
        {
            resSet.erase(VID_RES_CIF);
        }
    }

    bool Is2CIF() const 
    {
        return (resSet.find(VID_RES_2CIF) != resSet.end());
    }
    void Set2CIF(bool bIs2CIF) 
    {
        if(bIs2CIF) 
        {
            resSet.insert(VID_RES_2CIF);
        }
        else 
        {
            resSet.erase(VID_RES_2CIF);
        }
    }

    bool Is4CIF() const 
    {
        return (resSet.find(VID_RES_4CIF) != resSet.end());
    }
    void Set4CIF(bool bIs4CIF) 
    {
        if(bIs4CIF) 
        {
            resSet.insert(VID_RES_4CIF);
        }
        else 
        {
            resSet.erase(VID_RES_4CIF);
        }
    }

    bool IsVGA() const 
    {
        return (resSet.find(VID_RES_VGA) != resSet.end());
    }
    void SetVGA(bool bIsVGA) 
    {
        if(bIsVGA) 
        {
            resSet.insert(VID_RES_VGA);
        }
        else 
        {
            resSet.erase(VID_RES_VGA);
        }
    }

    bool IsQQCIF() const 
    {
        return (resSet.find(VID_RES_QQCIF) != resSet.end());
    }
    void SetQQCIF(bool bIsQQCIF) 
    {
        if(bIsQQCIF) 
        {
            resSet.insert(VID_RES_QQCIF);
        }
        else 
        {
            resSet.erase(VID_RES_QQCIF);
        }
    }

    bool IsQVGA() const 
    {
        return (resSet.find(VID_RES_QVGA) != resSet.end());
    }
    void SetQVGA(bool bIsQVGA) 
    {
        if(bIsQVGA) 
        {
            resSet.insert(VID_RES_QVGA);
        }
        else 
        {
            resSet.erase(VID_RES_QVGA);
        }
    }

    bool Is720P() const 
    {
        return (resSet.find(VID_RES_720P) != resSet.end());
    }
    void Set720P(bool bIs720P) 
    {
        if(bIs720P) 
        {
            resSet.insert(VID_RES_720P);
        }
        else 
        {
            resSet.erase(VID_RES_720P);
        }
    }

	bool Is960P() const 
	{
		return (resSet.find(VID_RES_960P) != resSet.end());
	}
	void Set960P(bool bIs960P) 
	{
		if(bIs960P) 
		{
			resSet.insert(VID_RES_960P);
		}
		else 
		{
			resSet.erase(VID_RES_960P);
		}
	}

    bool Is1080P() const 
    {
        return (resSet.find(VID_RES_1080P) != resSet.end());
    }
    void Set1080P(bool bIs1080P) 
    {
        if(bIs1080P) 
        {
            resSet.insert(VID_RES_1080P);
        }
        else 
        {
            resSet.erase(VID_RES_1080P);
        }
    }

    bool IsQXGA() const 
    {
        return (resSet.find(VID_RES_QXGA) != resSet.end());
    }
    void SetQXGA(bool bIsQXGA) 
    {
        if(bIsQXGA) 
        {
            resSet.insert(VID_RES_QXGA);
        }
        else 
        {
            resSet.erase(VID_RES_QXGA);
        }
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef map<CVidFormat, TVidType> CVidTypeList;
typedef set<CAudFormat> CAudTypeList;

struct TSupportMediaType
{
private:
    CVidTypeList video;  //所有支持的视频格式
    CAudTypeList audio;  //所有支持的音频格式

public:
    void Clear()
    {
        video.clear();
        audio.clear();
    }

    const CVidTypeList& GetVidTypes() const
    {
        return video;
    }

    CVidTypeList& GetVidTypes()
    {
        return video;
    }

    void SetVidTypes(const CVidTypeList& cVidTypeList)
    {
        video = cVidTypeList;
    }

    void AddVidType(const TVidType& tVidType)
    {
        video[tVidType.GetVidFormat()] = tVidType;
    }

    void EraseVidType(const TVidType& tVidType)
    {
        video.erase(tVidType.GetVidFormat());
    }

    void EraseVidType(const CVidFormat& tVidFmt)
    {
        video.erase(tVidFmt);
    }

    const CAudTypeList& GetAudTypes() const
    {
        return audio;
    }

    CAudTypeList& GetAudTypes()
    {
        return audio;
    }

    void SetAudTypes(const CAudTypeList& cAudTypeList)
    {
        audio = cAudTypeList;
    }

    void AddAudType(const CAudFormat& cAudType)
    {
        audio.insert(cAudType);
    }

    void EraseAudType(const CAudFormat& cAudType)
    {
        audio.erase(cAudType);
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


#define VID_QUALITY_PRIOR             0x01 //质量优先
#define VID_SPEED_PRIOR               0x02 //速度优先


//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
//struct TVidResItem
//{
//public:
//	TVidResItem()
//	{
//		Clear();
//	}
//	void Clear()
//	{
//		resolutionType = VIDEO_RESOLUTION_NONE;
//		height = 0xffff;
//		width = 0xffff;
//	}
//	void SetResolutionType(const u8 byType)
//	{
//		resolutionType = byType;
//	}
//	u8 GetResolutionType() const
//	{
//		return resolutionType;
//	}
//	void SetHeight(const u16 wHeight)
//	{
//		//ASSERT(resolutionType == 0);
//		height = wHeight;
//	}
//	u16 GetHeight() const
//	{
//		return height;
//	}
//	void SetWidth(const u16 wWidth)
//	{
//		//ASSERT(resolutionType == 0);
//		width = wWidth;
//	}
//	u16 GetWidth() const
//	{
//		return width;
//	}
//	//1.老方案，按照枚举值比大小
//	//2.老方案均小于新方案
//	//3.新方案比宽，宽相等时比高
//	bool operator<(const TVidResItem&tItem) const
//	{
//		if (resolutionType == VIDEO_RESOLUTION_NONE && tItem.resolutionType == VIDEO_RESOLUTION_NONE)
//		{
//			return (width < tItem.width) || (width == tItem.width && height < tItem.height); 
//		}
//		if (resolutionType == VIDEO_RESOLUTION_NONE)
//			return false;
//		if (tItem.resolutionType == VIDEO_RESOLUTION_NONE)
//			return true;
//		return resolutionType < tItem.resolutionType;
//	}
//
//protected:
//	//兼容以前方案，如果该值非 VIDEO_RESOLUTION_NONE ，则以该值为准。
//	u8 resolutionType;		//分辨率类型，VIDEO_RESOLUTION_**
//	u16 width;				//宽
//	u16 height;				//高
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//
//typedef std::set<TVidResItem> TVidResSet;
//typedef std::map<CVidType, TVidResSet> TVidTypeResMap;
//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

//视频编码参数
struct TVideoEncodeParam
{
private:	
    CVidFormat videoType;      //数据压缩格式; <见pu\MtCtrl.h>
    CVidRes videoRes;          //图象分辨率; <见pu\MtCtrl.h>
    int frameRate;             //帧率 1-25;
    int quality;               //图象质量; <见pu\MtCtrl.h>
    int bitRate;               //编码比特率 64K-4M;
	int	maxQuantization;	   //最大量化因子 1-31 //高级编码参数Ex
	int	minQuantization;	   //最小量化因子 1-31 //高级编码参数Ex
    int interval;              //关键帧间隔 25-2500;
    int brightness;            //亮度 -128～127
    int contrast;              //对比度 -128～127
    int saturation;            //饱和度 -128～127
	int	dnyValue;			   //动态载荷值 //高级编码参数Ex
    int sharpness;             //锐度 0: not support; 1-4: available
	int	profileId;			   //图像编码协议等级 //1.0中有，但是没有用到，保留
	int clockRate;             //视频采样率
public: 
    TVideoEncodeParam() 
    {
        Clear();
    }

    void Clear()
    {
        videoType.clear();
        videoRes.clear();
        frameRate = 0;    //帧率 1-25
        quality = 0;      //图象质量 <见pu\MtCtrl.h>
        bitRate = 0;      //编码比特率 64K-4M
        interval = 0;     //关键帧间隔 25-2500
        brightness = 0;      //亮度 -128～127
        contrast = 0;      //对比度 -128～127
        saturation = 0;      //饱和度 -128～127
        sharpness = 0;     //锐度 0: not support; 1-4: available
		maxQuantization = 0;	   //最大量化因子 1-31 //高级编码参数Ex
		minQuantization = 0;	   //最小量化因子 1-31 //高级编码参数Ex
		dnyValue = 0;
		profileId = 0;
		clockRate = 0;
    }


public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);

public:
    void SetVideoType(const CVidFormat& strVideoType)
    {
        videoType = strVideoType;
    }
    CVidFormat& GetVideoType()
    {
        return videoType;
    }
    const CVidFormat& GetVideoType() const
    {
        return videoType;
    }

    void SetVideoRes(const CVidRes& strVideoRes)
    {
        videoRes = strVideoRes;
    }
    CVidRes& GetVideoRes()
    {
        return videoRes;
    }
    const CVidRes& GetVideoRes() const
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

    void SetBright(int nBrightness)
    {
        brightness = nBrightness;
    }
    int GetBright() const
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
};

//音频编码参数
struct TAudioEncodeParam
{
private:
    CAudFormat audioType;                   //音频格式
    int volume;               			    //音量 0-25;
    bool aecState;               		    //回声抵消状态
    int smpRate;                            //采样率 
    int smpBits;                            //采样位数 
    int channel;                            //声道 
    int samplePerF;                         //每帧样本数
	int clockRate;                          //时间戳采样率
	int bitRate;                            //码率
	int payLoad;                            //音频payload

public: 
    TAudioEncodeParam() 
    {
        Clear();
    }

    void Clear()
    {
        audioType.clear();
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
    void SetAudioType(const CAudFormat& strAudioType)
    {
        audioType = strAudioType;
    }
    CAudFormat& GetAudioType()
    {
        return audioType;
    }
    const CAudFormat& GetAudioType() const
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

	int GetPayLoad() const
	{
		return payLoad;
	}
	void SetPayLoad(int nPayLoad)
	{
		payLoad = nPayLoad;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TPuBaseCapSet
{
private:
    int videoEncNum;               //视频编码通道数量;
    int videoDecNum;               //视频解码通道数量;
    int audioEncNum;               //音频编码通道数量;
    int audioDecNum;               //音频解码通道数量;
    int serialPortNum;             //串口数量;
    int inputPinNum;               //并口输入数量;
    int outputPinNum;              //并口输出数量;
    int videoInPortNum;            //视频输入端口数量;
	int usingVideoInPortNum;       //使用中的视频输入端口数量，非全通道时就等于videoInPortNum，全通道时则按照主流非IDLE总数计算

public:
    TPuBaseCapSet()
    {
        Clear();
    }

    ~TPuBaseCapSet()
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
 		usingVideoInPortNum = 0;     
   }

public:
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

    int GetSerialPortNum() const 
    {
        return serialPortNum;
    }
    void SetSerialPortNum(int dwSerialPortNum) 
    {
        serialPortNum = dwSerialPortNum;
    }

    int GetAlarmInNum() const 
    {
        return inputPinNum;
    }
    void SetAlarmInNum(int dwAlarmInNum) 
    {
        inputPinNum = dwAlarmInNum;
    }

    int GetAlarmOutNum() const 
    {
        return outputPinNum;
    }
    void SetAlarmOutNum(int dwAlarmOutNum) 
    {
        outputPinNum = dwAlarmOutNum;
    }

    int GetVideoInPortNum() const 
    {
        return videoInPortNum;
    }
    void SetVideoInPortNum(int dwVideoInPortNum) 
    {
        videoInPortNum = dwVideoInPortNum;
    }

	int GetUsingVideoInPortNum() const 
	{
		return usingVideoInPortNum;
	}
	void SetUsingVideoInPortNum(int dwUsingVideoInPortNum) 
	{
		usingVideoInPortNum = dwUsingVideoInPortNum;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TPuExtCap
{
private:
    int id;         // 能力集类型ID
    int value;      // 能力集类型值

public:
    TPuExtCap()
    {
        Clear();
    }
    ~TPuExtCap()
    {
        Clear();
    }
    void Clear()
    {
        id = 0;
        value = 0;
    }

public:
    int GetCapId() const
    {
        return id;
    }
    void SetCapId(int dwCapId)
    {
        id = dwCapId;
    }

    int GetCapValue() const
    {
        return value;
    }
    void SetCapValue(int dwCapValue)
    {
        value = dwCapValue;
    }

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

typedef set<int> CVidSrcSet;
typedef set<int> CPuChnSet;
typedef string CInputPinEnable;

//设备属性配置
struct TPuBaseConfig 
{
private:
    string            devUri;                         //前端设备URI：puid@domain
    TIpAddr           devIp;                          //前端设备IP
    string            devType;                        //设备类型：编码器或解码器
    string            devMode;                        //设备型号，CU会使用它判断一些老前端的能力，平台不会使用
    string            devName;                        //设备名称
    string            manufacturer;                   //设备厂商

    TPuBaseCapSet       baseCapSet;                   //基本能力集
    map<int,int>        extCapSet;                    //扩展能力集
	map<int,int>        hdmiVidDecCfg;                //hdmi视频解码配置<(0-3)hdmiIndex,(0.1.2.4.9.16)DecStyle>
    CInputPinEnable     inputPinEnable;               //并口输入通道使能配置

    int                 freeLicenceNum;               //免费的licence点数
    int                 directTransNum;               //设备支持的直传路数
    bool                isDecCombined;                //解码器是否合成码流

    bool                isGBDevice;                   //是否是国标设备

public:
    TPuBaseConfig()
    {
        Clear();
    }

    ~TPuBaseConfig()
    {
        Clear();
    }

    void Clear()
    {
        devUri.clear(); 
        devIp.clear();  
        devType.clear();
        devMode.clear();
        devName.clear();
        manufacturer.clear();
        baseCapSet.Clear();
		extCapSet.clear();      
		hdmiVidDecCfg.clear();      
        inputPinEnable.clear(); 
        freeLicenceNum = 0;
        directTransNum = 0;
        isDecCombined = false;
        isGBDevice = false;
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

    void SetDevIp(const TIpAddr& strDevIp)
    {
        devIp = strDevIp;
    }
    TIpAddr& GetDevIp()
    {
        return devIp;
    }
    const TIpAddr& GetDevIp() const
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

    void SetBaseCapSet(const TPuBaseCapSet& cBaseCapSet)
    {
        baseCapSet = cBaseCapSet;
    }
    TPuBaseCapSet& GetBaseCapSet()
    {
        return baseCapSet;
    }
    const TPuBaseCapSet& GetBaseCapSet() const
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

    void SetInputPinEnable(const CInputPinEnable& strInputPinEnable)
    {
        inputPinEnable = strInputPinEnable;
    }
    CInputPinEnable& GetInputPinEnable()
    {
        return inputPinEnable;
    }
    const CInputPinEnable& GetInputPinEnable() const
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

public:
    void SetIsGBDevice(bool bIsGBDevice)
    {
        isGBDevice = bIsGBDevice;
    }
    bool GetIsGBDevice() const
    {
        return isGBDevice;
    }
};

typedef vector<TPuBaseConfig> CPuBaseCfgList;

//设备通道配置
struct TPuChnConfig 
{
private:
    TChannel   devChn;                                  //设备通道
    TSupportMediaType supportMediaType;                 //通道当前支持的所有媒体格式(只读)
    TVideoEncodeParam chnVidFormat;                     //通道当前的视频格式
	TAudioEncodeParam chnAudFormat;                     //通道当前的音频参数
    int        chnVidDynValue;                          //如果通道视频格式是H264，设置该通道的动态载荷
    CVidSrcSet availVidSrcSet;                           //通道可用的视频源列表

public:
    TPuChnConfig()
    {
        Clear();
    }

    ~TPuChnConfig()
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
    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

    void SetSupportMediaType(const TSupportMediaType& cSupportMediaType)
    {
        supportMediaType = cSupportMediaType;
    }
    TSupportMediaType& GetSupportMediaType()
    {
        return supportMediaType;
    }
    const TSupportMediaType& GetSupportMediaType() const
    {
        return supportMediaType;
    }

    void SetChnVidFormat(const TVideoEncodeParam& cChnVidFormat)
    {
        chnVidFormat = cChnVidFormat;
    }
    TVideoEncodeParam& GetChnVidFormat()
    {
        return chnVidFormat;
    }
    const TVideoEncodeParam& GetChnVidFormat() const
    {
        return chnVidFormat;
    }

	void SetChnAudFormat(const TAudioEncodeParam& cChnAudFormat)
	{
		chnAudFormat = cChnAudFormat;
	}
	TAudioEncodeParam& GetChnAudFormat()
	{
		return chnAudFormat;
	}
	const TAudioEncodeParam& GetChnAudFormat() const
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

    void SetAvailVidSrcSet(const CVidSrcSet& cAvailVidSrcSet)
    {
        availVidSrcSet = cAvailVidSrcSet;
    }
    CVidSrcSet& GetAvailVidSrcSet()
    {
        return availVidSrcSet;
    }
    const CVidSrcSet& GetAvailVidSrcSet() const
    {
        return availVidSrcSet;
    }



};

typedef map<int, TPuChnConfig> CPuChnCfgList;

//视频源通道配置
struct TPuVidSrcConfig 
{
private:
    string     name;                         //视频源名称
    TChannel   vidChn;                       //视频源通道
    CPuChnSet  availChnSet;                  //视频源当前正在使用的通道列表
    int        camType;                      //视频源的摄像头类型
    int        camId;                        //视频源的摄像头地址码
	string     azimuth;                      //方位角，base64加密后的密文，使用前先base64解密

public:
    TPuVidSrcConfig()
    {
        Clear();
    }

    ~TPuVidSrcConfig()
    {
        Clear();
    }

    void Clear()
    {
        name.clear();
        vidChn.Clear();
        availChnSet.clear();
        camType = 0;
        camId = 0;
		azimuth.clear();
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetVidChn(const TChannel& cVidChn)
    {
        vidChn = cVidChn;
    }
    TChannel& GetVidChn()
    {
        return vidChn;
    }
    const TChannel& GetVidChn() const
    {
        return vidChn;
    }

    void SetAvailChnSet(const CPuChnSet& cAvailChnSet)
    {
        availChnSet = cAvailChnSet;
    }
    CPuChnSet& GetAvailChnSet()
    {
        return availChnSet;
    }
    const CPuChnSet& GetAvailChnSet() const
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

	void SetAzimuth(const string& strAzimuth)
	{
		azimuth = strAzimuth;
	}
	string& GetAzimuth()
	{
		return azimuth;
	}
	const string& GetAzimuth() const
	{
		return azimuth;
	}
};


typedef map<int, TPuVidSrcConfig> CPuVidSrcCfgList;


enum emChnStatus
{
	CHNSTATUS_IDEL = 0,      
	CHNSTATUS_ONLINE,
	CHNSTATUS_OFFLINE ,
	CHNSTATUS_MAX  = 256
};

struct TPuChnStateConfig
{
private:
	
	int chnState;  //通道状态类型参见emChnStatus
public:
	TPuChnStateConfig()
	{
		chnState = CHNSTATUS_IDEL;	
	}
	~TPuChnStateConfig()
	{
	
	}
    void SetChnState(int nChnState)
    {
        chnState = nChnState;
    }
    int GetChnState() const
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


typedef map<TChannelID, TPuChnStateConfig> CPuChnStateList; //add by ywy for  channel state report

//设备配置
struct TPuDevConfig 
{
private:
    CPuBaseCfgList propCfgList;                    //设备属性配置
    CPuChnCfgList chnCfgList;                      //设备通道配置
    CPuVidSrcCfgList vidSrcCfgList;                //视频源通道配置      
	CPuChnStateList chnStateCfgList;               //通道状态配置   add by ywy for  channel state report
           
  


public:
    TPuDevConfig()
    {
        Clear();
    }

    ~TPuDevConfig()
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

    void MergeDevConfig(const TPuDevConfig& tCfg)
    {
        if (tCfg.GetBaseCfgList().size()>0)
        {
            SetBaseCfgList(tCfg.GetBaseCfgList());
        }

		if (GetBaseCfgList().empty())
		{
			return;
		}

		CPuChnCfgList tChnCfgList = tCfg.GetChnCfgList();
		typedef CPuChnCfgList::const_iterator ChnMapIt;
		int nVidEncNum = GetBaseCfgList().at(0).GetBaseCapSet().GetVidEncNum();
		for (ChnMapIt mChnIt = tChnCfgList.begin(); mChnIt != tChnCfgList.end() && mChnIt->first<nVidEncNum; ++mChnIt)
		{
			chnCfgList[mChnIt->first] = mChnIt->second;          
		}

		CPuVidSrcCfgList tVidSrcCfgList = tCfg.GetVidSrcCfgList();
		typedef CPuVidSrcCfgList::const_iterator VidSrcMapIt;
		int nVidSrcNum = GetBaseCfgList().at(0).GetBaseCapSet().GetVideoInPortNum();
		for (VidSrcMapIt mVidSrcIt = tVidSrcCfgList.begin(); mVidSrcIt != tVidSrcCfgList.end() && mVidSrcIt->first<nVidSrcNum; ++mVidSrcIt)
		{
			vidSrcCfgList[mVidSrcIt->first] = mVidSrcIt->second;          
		}

		const CPuChnStateList &tChnStateCfgList = tCfg.GetChnStateCfgList(); //add by ywy 2014.3.12
		map<TChannelID, TPuChnStateConfig>::const_iterator tPuChnStateCfgItor = tChnStateCfgList.begin();
		for (; tPuChnStateCfgItor != tChnStateCfgList.end() && tPuChnStateCfgItor->first<nVidEncNum; tPuChnStateCfgItor++)
		{
			chnStateCfgList[tPuChnStateCfgItor->first] = tPuChnStateCfgItor->second;
		}
    }

    bool IsCompleted() const
    {
        if (GetBaseCfgList().empty())
        {
            return false;
        }

        const TPuBaseConfig& tPropCfg =  GetBaseCfgList().at(0);
        if (tPropCfg.GetDevType() == PU_DECODER)
        {
            //解码器直接返回
            return true;
        }

        if (GetChnCfgList().size() != (u32)tPropCfg.GetBaseCapSet().GetVidEncNum())
        {
            return false;
        }

        if (GetVidSrcCfgList().size() != (u32)tPropCfg.GetBaseCapSet().GetVideoInPortNum())
        {
            return false;
        }

		//{{{{{{{{{{{{{{{ modify by Royan 20140603 IsCompleted添加通道状态判断
		if (GetChnStateCfgList().size() != (u32)tPropCfg.GetBaseCapSet().GetVidEncNum())
		{
			return false;
		}
		//}}}}}}}}}}}}}}} modify by Royan 20140603 IsCompleted添加通道状态判断

        return true;;
    }
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetBaseCfgList(const CPuBaseCfgList& cPropCfgList)
    {
        propCfgList = cPropCfgList;
    }
    CPuBaseCfgList& GetBaseCfgList()
    {
        return propCfgList;
    }
    const CPuBaseCfgList& GetBaseCfgList() const
    {
        return propCfgList;
    }

    void SetChnCfgList(const CPuChnCfgList& cChnCfgList)
    {
        chnCfgList = cChnCfgList;
    }
    CPuChnCfgList& GetChnCfgList()
    {
        return chnCfgList;
    }
    const CPuChnCfgList& GetChnCfgList() const
    {
        return chnCfgList;
    }

    void SetVidSrcCfgList(const CPuVidSrcCfgList& cVidSrcCfgList)
    {
        vidSrcCfgList = cVidSrcCfgList;
    }
    CPuVidSrcCfgList& GetVidSrcCfgList()
    {
        return vidSrcCfgList;
    }
    const CPuVidSrcCfgList& GetVidSrcCfgList() const
    {
        return vidSrcCfgList;
    }

public:
    void SetChnStateCfgList(const CPuChnStateList& cChnStateCfgList)
    {
        chnStateCfgList = cChnStateCfgList;
    }
    CPuChnStateList& GetChnStateCfgList()
    {
        return chnStateCfgList;
    }
    const CPuChnStateList& GetChnStateCfgList() const
    {
        return chnStateCfgList;
    }

};

//各模块内部使用，不在网络传输
struct TPuConfig 
{
private:
    string            devUri;           //前端设备URI：puid@domain
    TIpAddr           devIp;            //前端设备IP
    string            devType;          //设备类型：编码器或解码器
    string            devMode;          //设备型号，CU会使用它判断一些老前端的能力，平台不会使用
    string            devName;          //设备名称
    string            manufacturer;     //设备厂商
    
    TPuBaseCapSet       baseCapSet;     //基本能力集
    map<int,int>        extCapSet;      //扩展能力集
	map<int,int>        hdmiVidDecCfg;  //hdmi视频解码配置<(0-3)hdmiIndex,(0.1.2.4.9.16)DecStyle>
    CInputPinEnable     inputPinEnable; //并口输入通道使能配置

    map<int,CVidFormat> chnVidFormat;   //通道当前的视频格式
    map<int,int>        chnVidDynValue; //如果通道视频格式是H264，设置该通道的动态载荷

    map<int,TSupportMediaType> supportMediaType; //通道当前支持的所有媒体格式(只读)
    map<int,TVideoEncodeParam> chnVidEncParam;   //通道当前的视频编码参数
	map<int,TAudioEncodeParam> chnAudEncParam;   //通道当前的音频编码参数  

    map<int,string> vidSrcName;                    //视频源别名

    map<int,CPuChnSet>  vidSrcChnSet;   //视频源当前正在使用的通道列表
    map<int,CVidSrcSet> chnVidSrcSet;   //通道可用的视频源列表
    map<int,int>        camType;        //视频源的摄像头类型
    map<int,int>        camId;          //视频源的摄像头地址码
    map<int,int>        bitRate;        //通道的比特率 
	map<TChannelID, TPuChnStateConfig>  chnStatusSet;  //通道状态集 add by ywy for  channel state report

    int                 freeLicenceNum; //免费的licence点数
    int                 directTransNum; //设备支持的直传路数
    bool                isDecCombined;  //解码器是否合成码流

    bool                isGBDevice;     //是否是国标设备

	map<int,string>     azimuth;        //方位角，base64加密后的密文，使用前先base64解密

public:
    TPuConfig()
    {
        Clear();
    }

    ~TPuConfig()
    {
        Clear();
    }

    void Clear()
    {
        devUri.clear(); 
        devIp.clear();  
        devType.clear();
        devMode.clear();
        devName.clear();
        manufacturer.clear();
        baseCapSet.Clear();
		extCapSet.clear();      
		hdmiVidDecCfg.clear();      
        inputPinEnable.clear(); 
        chnVidFormat.clear();   
        chnVidDynValue.clear();
        supportMediaType.clear();
        chnVidEncParam.clear();
		chnAudEncParam.clear();
        vidSrcName.clear();
        vidSrcChnSet.clear();   
        chnVidSrcSet.clear();   
        camType.clear();        
        camId.clear();          
        bitRate.clear();
		chnStatusSet.clear(); //add by ywy for  channel state reprot
        freeLicenceNum = 0;
        directTransNum = 0;
        isDecCombined = false;
        isGBDevice = false;
		azimuth.clear();
    }

    //转换接口
    TPuBaseConfig GetBaseConfig() const
    { 
        TPuBaseConfig tCfg;
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
        tCfg.SetIsGBDevice(GetIsGBDevice());

        return tCfg;
    }
    void SetBaseConfig(const TPuBaseConfig& tCfg)
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
        SetIsGBDevice(tCfg.GetIsGBDevice());
    }

    TPuChnConfig GetChnConfig(int nChnNO) const
    {
        TPuChnConfig tCfg;

        map<int,int>::const_iterator ptDynValue = GetVidDynValue().find(nChnNO);
        if (ptDynValue != GetVidDynValue().end())
        {
            tCfg.SetChnVidDynValue(ptDynValue->second);
        }

        map<int,TVideoEncodeParam>::const_iterator ptCurVidEncParam = GetVideoEncodeParam().find(nChnNO);
        if (ptCurVidEncParam != GetVideoEncodeParam().end())
        {
            tCfg.SetChnVidFormat(ptCurVidEncParam->second);
        }      

		map<int,TAudioEncodeParam>::const_iterator ptCurAudEncParam = GetAudioEncodeParam().find(nChnNO);
		if (ptCurAudEncParam != GetAudioEncodeParam().end())
		{
			tCfg.SetChnAudFormat(ptCurAudEncParam->second);
		}

        map<int,TSupportMediaType>::const_iterator ptSupportVidEncParam = GetSupportMediaType().find(nChnNO);
        if (ptSupportVidEncParam != GetSupportMediaType().end())
        {
            tCfg.SetSupportMediaType(ptSupportVidEncParam->second);
        } 

        const map<int,CVidSrcSet>::const_iterator ptVidSrcSet = GetChnVidSrcSet().find(nChnNO);
        if (ptVidSrcSet != GetChnVidSrcSet().end())
        {
            tCfg.SetAvailVidSrcSet(ptVidSrcSet->second);
        }

        return tCfg;
    }
    void SetChnConfig(int nChnNO, const TPuChnConfig& tCfg)
    {
        if (nChnNO < 0 && nChnNO > (GetBaseCapSet().GetVidEncNum()-1))
        {
            return;
        }
		//add by whui:TPuConfig中chnVidFormat实则在chnVidEncParam已有，这里也设置一下
		GetCurrVidFormat()[nChnNO] = tCfg.GetChnVidFormat().GetVideoType();
        GetVidDynValue()[nChnNO] = tCfg.GetChnVidDynValue();
        GetVideoEncodeParam()[nChnNO] = tCfg.GetChnVidFormat();    
		GetAudioEncodeParam()[nChnNO] = tCfg.GetChnAudFormat();
        GetSupportMediaType()[nChnNO] = tCfg.GetSupportMediaType();
        GetChnVidSrcSet()[nChnNO] = tCfg.GetAvailVidSrcSet();
    }

	TPuChnStateConfig GetChnStateConfig(int nChnNO) const //add by ywy for  channel state report
	{	
		TPuChnStateConfig puChnStateCfg;
		map<TChannelID, TPuChnStateConfig>::const_iterator ptChnStateItor = GetChnStateSet().find(nChnNO);
		if (GetChnStateSet().end() != ptChnStateItor)
		{
			puChnStateCfg = ptChnStateItor->second;
			
		}
		else if (extCapSet.end() == extCapSet.find(VSIP_ENTITY_CHANNL_STATE_REPORT))
		{
			puChnStateCfg.SetChnState(CHNSTATUS_OFFLINE);
		}
		return puChnStateCfg;		
	}

	void SetChnState(int nChnNO, const TPuChnStateConfig& tPuChnStateCfg) //add by ywy for  channel state report
	{
		GetChnStateSet()[nChnNO] = tPuChnStateCfg;
	}


    TPuVidSrcConfig GetVidSrcConfig(int nChnNO) const
    {
        TPuVidSrcConfig tCfg;

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

		const map<int,CPuChnSet>::const_iterator ptChnSet = GetVidSrcChnSet().find(nChnNO);
		if (ptChnSet != GetVidSrcChnSet().end())
		{
			tCfg.SetAvailChnSet(ptChnSet->second);
		}

		const map<int,string>::const_iterator ptAzimuth = GetAzimuth().find(nChnNO);
		if (ptAzimuth != GetAzimuth().end())
		{
			tCfg.SetAzimuth(ptAzimuth->second);
		}

        return tCfg;
    }
    void SetVidSrcConfig(int nChnNO, const TPuVidSrcConfig& tCfg)
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

    TPuDevConfig GetDevConfig() const
    {
        TPuDevConfig tCfg;

        if (!GetDevUri().empty())
        {
            tCfg.GetBaseCfgList().push_back(GetBaseConfig());
        }
        
        for (int nVidEndIdx=0; nVidEndIdx<GetBaseCapSet().GetVidEncNum(); nVidEndIdx++)
        {
            if (GetVideoEncodeParam().find(nVidEndIdx) != GetVideoEncodeParam().end())
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

		map<TChannelID, TPuChnStateConfig> &tPuStateCfgSet = tCfg.GetChnStateCfgList(); //add by ywy 2014.3.12
		map<TChannelID, TPuChnStateConfig>::const_iterator puStateCfgItor = GetChnStateSet().begin();
		for (; puStateCfgItor != GetChnStateSet().end(); puStateCfgItor++) 
		{
			tPuStateCfgSet[puStateCfgItor->first] = puStateCfgItor->second;      
		}
        
        return tCfg;
    }
    void SetDevConfig(const TPuDevConfig& tCfg)
    {
        if (tCfg.GetBaseCfgList().size()>0)
        {
            SetBaseConfig(tCfg.GetBaseCfgList().at(0));
        }

        CPuChnCfgList tChnCfgList = tCfg.GetChnCfgList();
        typedef CPuChnCfgList::const_iterator ChnMapIt;
        for (ChnMapIt mChnIt = tChnCfgList.begin(); mChnIt != tChnCfgList.end(); ++mChnIt)
        {
            SetChnConfig(mChnIt->first, mChnIt->second);          
        }

        CPuVidSrcCfgList tVidSrcCfgList = tCfg.GetVidSrcCfgList();
        typedef CPuVidSrcCfgList::const_iterator VidSrcMapIt;
        for (VidSrcMapIt mVidSrcIt = tVidSrcCfgList.begin(); mVidSrcIt != tVidSrcCfgList.end(); ++mVidSrcIt)
        {
            SetVidSrcConfig(mVidSrcIt->first, mVidSrcIt->second);          
        }

		CPuChnStateList tChnStateCfgList = tCfg.GetChnStateCfgList();
		map<TChannelID, TPuChnStateConfig>::const_iterator tPuChnStateCfgItor = tChnStateCfgList.begin();
		for (; tPuChnStateCfgItor != tChnStateCfgList.end(); tPuChnStateCfgItor++)
		{
			SetChnState(tPuChnStateCfgItor->first, tPuChnStateCfgItor->second);
		}

    }

    void MergeDevConfig(const TPuDevConfig& tCfg)
    {
        SetDevConfig(tCfg);
    }


    bool IsCompleted() const
    {
        return GetDevConfig().IsCompleted();
    }
    
    void PrintData() const;

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

    const TIpAddr& GetDevIp() const
    {
        return devIp;
    }
    TIpAddr& GetDevIp()
    {
        return devIp;
    }
    void SetDevIp(const TIpAddr& tDevIp)
    {
        devIp = tDevIp;
    }

    const string& GetDevType() const
    {
        return devType;
    }
    string& GetDevType()
    {
        return devType;
    }
    void SetDevType(const string& strDevType)
    {
        devType = strDevType;
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

    const TPuBaseCapSet& GetBaseCapSet() const
    {
        return baseCapSet;
    }
    TPuBaseCapSet& GetBaseCapSet()
    {
        return baseCapSet;
    }
    void SetBaseCapSet(const TPuBaseCapSet& tBaseCapSet)
    {
        baseCapSet = tBaseCapSet;
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
	void SetHdmiVidDecCfg(const map<int,int>& mapHdmiVidDecCfg)
	{
		hdmiVidDecCfg = mapHdmiVidDecCfg;
	}

    const CInputPinEnable& GetInputPinEnable() const
    {
        return inputPinEnable;
    }
    CInputPinEnable& GetInputPinEnable()
    {
        return inputPinEnable;
    }
    void SetInputPinEnable(const CInputPinEnable& tInputPinEnable)
    {
        inputPinEnable = tInputPinEnable;
    }

    const map<int,CVidFormat>& GetCurrVidFormat() const
    {
        return chnVidFormat;
    }
    map<int,CVidFormat>& GetCurrVidFormat()
    {
        return chnVidFormat;
    }
    void SetCurrVidFormat(const map<int,CVidFormat>& tCurrVidFormat)
    {
        chnVidFormat = tCurrVidFormat;
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

    const map<int,TSupportMediaType>& GetSupportMediaType() const
    {
        return supportMediaType;
    }
    map<int,TSupportMediaType>& GetSupportMediaType()
    {
        return supportMediaType;
    }
    void SetSupportMediaType(map<int,TSupportMediaType>& tSupportMediaType)
    {
        supportMediaType = tSupportMediaType;
    }

    const map<int,TVideoEncodeParam>& GetVideoEncodeParam() const
    {
        return chnVidEncParam;
    }
    map<int,TVideoEncodeParam>& GetVideoEncodeParam()
    {
        return chnVidEncParam;
    }
    void SetVideoEncodeParam(map<int,TVideoEncodeParam>& tVideoEncodeParam)
    {
        chnVidEncParam = tVideoEncodeParam;
    }

	const map<int,TAudioEncodeParam>& GetAudioEncodeParam() const
	{
		return chnAudEncParam;
	}
	map<int,TAudioEncodeParam>& GetAudioEncodeParam()
	{
		return chnAudEncParam;
	}
	void SetAudioEncodeParam(map<int,TAudioEncodeParam>& tAudioEncodeParam)
	{
		chnAudEncParam = tAudioEncodeParam;
	}

    const map<int,CPuChnSet>&  GetVidSrcChnSet() const
    {
        return vidSrcChnSet;
    }
    map<int,CPuChnSet>& GetVidSrcChnSet()
    {
        return vidSrcChnSet;
    }
    void SetVidSrcChnSet(const map<int,CPuChnSet>& tVidSrcChnSet)
    {
        vidSrcChnSet = tVidSrcChnSet;
    }

    const map<int,CVidSrcSet>& GetChnVidSrcSet() const
    {
        return chnVidSrcSet;
    }
    map<int,CVidSrcSet>& GetChnVidSrcSet()
    {
        return chnVidSrcSet;
    }
    void SetChnVidSrcSet(const map<int,CVidSrcSet>& tChnVidSrcSet)
    {
        chnVidSrcSet = tChnVidSrcSet;
    }

    const map<int,int>& GetCamType() const
    {
        return camType;
    }
    map<int,int>& GetCamType()
    {
        return camType;
    }
    void SetCamType(const map<int,int>& tCamType)
    {
        camType = tCamType;
    }

    const map<int,int>& GetCamId() const
    {
        return camId;
    }
    map<int,int>& GetCamId()
    {
        return camId;
    }
    void SetCamId(const map<int,int>& tCamId)
    {
        camId = tCamId;
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

	const map<TChannelID, TPuChnStateConfig>& GetChnStateSet() const //add by ywy for  channel state report
	{
		return chnStatusSet;
	}

	map<TChannelID, TPuChnStateConfig>& GetChnStateSet() //add by ywy for  channel state report
	{
		return chnStatusSet;
	}
	void SetChnStateSet(const map<TChannelID, TPuChnStateConfig>& tChnStateSet) //add by ywy for  channel state report
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

	const map<int,string>& GetAzimuth() const
	{
		return azimuth;
	}
	map<int,string>& GetAzimuth()
	{
		return azimuth;
	}
	void SetAzimuth(const map<int,string>& tAzimuth)
	{
		azimuth = tAzimuth;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
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

    //根据视频源通道查找视频源名称
    string GetVidSrcNameByVidSrcChn(int nVidSrcChn) const
    {
        string strName;
        const map<int,string>::const_iterator ptVidSrcName = GetVidSrcName().find(nVidSrcChn);
        if (ptVidSrcName != GetVidSrcName().end())
        {
            strName = ptVidSrcName->second;
        }

        return strName;
    }

    //根据编码通道查找视频源名称
    string GetVidSrcNameByEncChn(int nEncChn) const
    {
        //先根据编码通道找到对应视频源通道
        int nVidSrcChn = -1;
        const map<int,CVidSrcSet>::const_iterator ptVidSrcSet = GetChnVidSrcSet().find(nEncChn);
        if (ptVidSrcSet != GetChnVidSrcSet().end())
        {
            const CVidSrcSet& tVidSrcSet = ptVidSrcSet->second;
            for (CVidSrcSet::const_iterator sVidSrcIt = tVidSrcSet.begin(); sVidSrcIt != tVidSrcSet.end(); ++sVidSrcIt)
            {
                //目前一个编码通道只能归属于一个视频源，tVidSrcSet最多只有一个元素
                nVidSrcChn = *sVidSrcIt;
                break;
            }
        }
      
        return GetVidSrcNameByVidSrcChn(nVidSrcChn);
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
	BOOL32 BeSupportExtCap(int iCapID)
	{
		//map<int, int>& tExtCapSet = tPuConfig.GetExtCapSet();
		map<int,int>::const_iterator capSetItor = extCapSet.find(iCapID);
		if (extCapSet.end() != capSetItor)
		{
			return TRUE;
		}
		return FALSE;

	}

public:
    void SetIsGBDevice(bool bIsGBDevice)
    {
        isGBDevice = bIsGBDevice;
    }
    bool GetIsGBDevice() const
    {
        return isGBDevice;
    }
};

//pu的入/退网，起/停用，上/下线，起/停线
struct TPuStatus 
{
private:
    bool isInTop;           //是否入网
    bool isUsed;            //是否启用
    bool isOnline;          //是否在线

public:
    TPuStatus()
    {
        Clear();
    }

    ~TPuStatus()
    {
        Clear();
    }

    void Clear()
    {
        isInTop = true; 
        isUsed = true;  
        isOnline = false;
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU上下线信息-----------------------------------\n\n");

        OspPrintf(TRUE, FALSE, "  -----------------------------------基本信息-----------------------------------\n");

        OspPrintf(TRUE, FALSE, "isInTop[%d], isUsed[%d], isOnline[%d] \n", isInTop, isUsed, isOnline);
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

struct TPuServiceStatus
{
private:
    //map<int,TChannel>    decChnVidSrc;    //解码器播放的视频源通道（通常是PU）
    map<int,TChannel>    audioCallSrc;    //音频呼叫的源（通常是CU）
    map<int,bool>        isPlatPreRec;    //是否在平台预录
    map<int,TRecStatus>  platRec;         //是否在平台录像
    map<int,bool>        isPuPreRec;      //是否在前端预录
    map<int,bool>        isPuRec;         //是否在前端录像

public:
    TPuServiceStatus()
    {
        Clear();
    }

    ~TPuServiceStatus()
    {
        Clear();
    }

    void Clear()
    {
        //decChnVidSrc.clear();
        audioCallSrc.clear();
        isPlatPreRec.clear();
        platRec.clear();   
        isPuPreRec.clear();  
        isPuRec.clear();      
    }

public:
//     const map<int,TChannel>& GetDecChnVidSrc() const
//     {
//         return decChnVidSrc;
//     }
//     map<int,TChannel>& GetDecChnVidSrc()
//     {
//         return decChnVidSrc;
//     }
//     void SetDecChnVidSrc(const map<int,TChannel>& tDecChnVidSrc)
//     {
//         decChnVidSrc = tDecChnVidSrc;
//     }

    const map<int,TChannel>& GetAudioCallSrc() const
    {
        return audioCallSrc;
    }
    map<int,TChannel>& GetAudioCallSrc()
    {
        return audioCallSrc;
    }
    void SetAudioCallSrc(const map<int,TChannel>& tAudioCallSrc)
    {
        audioCallSrc = tAudioCallSrc;
    }

    const map<int,bool>& GetIsPlatPreRec() const
    {
        return isPlatPreRec;
    }
    map<int,bool>& GetIsPlatPreRec()
    {
        return isPlatPreRec;
    }
    void SetIsPlatPreRec(const map<int,bool>& tIsPlatPreRec)
    {
        isPlatPreRec = tIsPlatPreRec;
    }

    const map<int,TRecStatus>& GetPlatRec() const
    {
        return platRec;
    }
    map<int,TRecStatus>& GetPlatRec()
    {
        return platRec;
    }
    void SetPlatRec(const map<int,TRecStatus>& tPlatRec)
    {
        platRec = tPlatRec;
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

//pu告警输入
struct TAlarmInput
{
private:    
    string            devUri;           //前端设备URI：puid@domain
    string            alarmType;        //告警类型: ALARM_TYPE_PIN, ALARM_TYPE_MOVE等
    int               inputId;          //告警输入通道
    string            alarmFlag;        //告警标志: ALARM_FLAG_OCCUR, ALARM_FLAG_RESTORE
    int               detAreaId;        //移动侦测表示告警区域号，移动监测时有效
    string            alarmTime;        //告警时间，在pui端生成
    string            alarmData;        //告警数据
	string            alarmAlias;       //告警别名

public:
    TAlarmInput()
    {
        Clear();
    }

    ~TAlarmInput()
    {
        Clear();
    }

    void Clear()
    {
        inputId = 0;
        detAreaId = 0;
        devUri.clear();
        alarmType.clear();
        alarmFlag.clear();
        alarmTime.clear();
        alarmData.clear();
		alarmAlias.clear();
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

    const string& GetAlarmFlag() const
    {
        return alarmFlag;
    }
    string& GetAlarmFlag()
    {
        return alarmFlag;
    }
    void SetAlarmFlag(const string& strAlarmFlag)
    {
        alarmFlag = strAlarmFlag;
    }

    const string& GetAlarmTime() const
    {
        return alarmTime;
    }
    string& GetAlarmTime()
    {
        return alarmTime;
    }
    void SetAlarmTime(const string& strAlarmTime)
    {
        alarmTime = strAlarmTime;
    }

    const string& GetAlarmData() const
    {
        return alarmData;
    }
    string& GetAlarmData()
    {
        return alarmData;
    }
    void SetAlarmData(const string& strAlarmData)
    {
        alarmData = strAlarmData;
    }

    int GetInputId() const 
    {
        return inputId;
    }
    void SetInputId(int dwInputId) 
    {
        inputId = dwInputId;
    }

    int GetDetAreaId() const 
    {
        return detAreaId;
    }
    void SetDetAreaId(int dwDetAreaId) 
    {
        detAreaId = dwDetAreaId;
    }

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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef    vector<TAlarmInput>   CAlarmInputArray;


struct TAlarmType
{
private:
	string            alarmType;        //告警类型: ALARM_TYPE_PIN, ALARM_TYPE_MOVE等
	string            alarmTypeName;
public:
	TAlarmType()
	{
		Clear();
	}

	~TAlarmType()
	{
		Clear();
	}

	void Clear()
	{
		alarmType.clear();
		alarmTypeName.clear();
	}

public:
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
	const string& GetAlarmTypeName() const
	{
		return alarmTypeName;
	}
	string& GetAlarmTypeName()
	{
		return alarmTypeName;
	}
	void SetAlarmTypeName(const string& strAlarmTypeName)
	{
		alarmTypeName = strAlarmTypeName;
	}
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef    vector<TAlarmType>    CAlarmTypeArray;

struct TPuAlarmStatus
{
private:
    CAlarmInputArray       alarmArray;       //告警信息
public:
    TPuAlarmStatus()
    {
        Clear();
    }

    void Clear()
    {
        alarmArray.clear();
    }

public:

    const CAlarmInputArray& GetAlarmArray() const
    {
        return alarmArray;
    }
    CAlarmInputArray& GetAlarmArray()
    {
        return alarmArray;
    }
    void SetAlarmArray(const CAlarmInputArray& tAlarmArray)
    {
        alarmArray = tAlarmArray;
    }

    void AddAlarmItem(const TAlarmInput& tAlarmInput)
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

//告警源
struct TAlarmLinkSrc
{
public:
    TAlarmLinkSrc()
    {
        Clear();
    }

    void Clear()
    {
        inputId = 0;
        devUri.clear();
        alarmType.clear();
    }

    bool operator < (const TAlarmLinkSrc &tOther) const
    {
        if (devUri < tOther.GetDevUri())
        {
            return true;
        }
        else
        {
            if (tOther.GetDevUri() < devUri)
            {
                return false;
            }
            else            // devUri == tOther.GetDevUri()
            {
                if (inputId < tOther.GetInputId())
                {
                    return true;
                }
                else
                {
                    if (tOther.GetInputId() < inputId)
                    {
                        return false;
                    }
                    else        // devUri == tOther.GetDevUri() && inputId == tOther.GetInputId()
                    {
                        return alarmType < tOther.GetAlarmType();
                    }
                }
            }
        }
    }

    bool operator == ( const TAlarmLinkSrc& tOther ) const
    {
        return (inputId == tOther.inputId 
            && devUri == tOther.devUri
            && alarmType == tOther.alarmType);
    }

    bool operator != ( const TAlarmLinkSrc& tOther ) const
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
#define    ALARMLINK_PLAT_REC        "REC"
#define    ALARMLINK_TV_WALL         "TVWALL"
#define    ALARMLINK_READ_SEC        "READSEC"
#define    ALARMLINK_IO_OUT          "IOOUT"
#define    ALARMLINK_CAM             "CAM"

typedef set<string>  CLinkActionList;

//告警目的通道和行为
struct TAlarmLinkDst
{
public:
    TAlarmLinkDst()
    {
        Clear();
    }

    void Clear()
    {
        encChn.Clear();
        linkActionList.clear();
        camPrePos = 0;
        tvWallId.clear();
        tvId = 0;
        tvDivId = 0;
        alarmOutChn = 0;
        msgNoList.clear();
        vocNoList.clear();
        mailNoList.clear();
    }

public:
    const TChannel& GetEncChn() const
    {
        return encChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    void SetEncChn(const TChannel& tChn)
    {
        encChn = tChn;
    }

    const CLinkActionList& GetLinkActionList() const
    {
        return linkActionList;
    }
    CLinkActionList& GetLinkActionList()
    {
        return linkActionList;
    }
    void SetLinkActionList(const CLinkActionList& cLinkActionList)
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
    TChannel        encChn;            //告警联动目的编码通道
    CLinkActionList linkActionList;    //联动行为列表

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
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
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

};


//告警联动通用结构，包含联动和多前端联动
typedef vector<TAlarmLinkDst>  CAlarmLinkDstList;

struct TAlarmLinkage
{
public:
    TAlarmLinkage()
    {
        Clear();
    }
    void Clear()
    {
        alarmAlias.clear();
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

    const TCronTimeList& GetDefenceSched() const
    {
        return defenceSched;
    }
    TCronTimeList& GetDefenceSched()
    {
        return defenceSched;
    }
    void SetDefenceSched(const TCronTimeList& tDefenceSched)
    {
        defenceSched = tDefenceSched;
    }

    const TAlarmLinkSrc& GetAlarmLinkSrc() const
    {
        return alarmSrc;
    }
    TAlarmLinkSrc& GetAlarmLinkSrc()
    {
        return alarmSrc;
    }
    void SetAlarmLinkSrc(const TAlarmLinkSrc& tAlarmLinkSrc)
    {
        alarmSrc = tAlarmLinkSrc;
    }

    const CAlarmLinkDstList& GetAlarmLinkDstList() const
    {
        return alarmDstList;
    }
    CAlarmLinkDstList& GetAlarmLinkDstList()
    {
        return alarmDstList;
    }
    void SetAlarmLinkDstList(const CAlarmLinkDstList& tAlarmLinkDstList)
    {
        alarmDstList = tAlarmLinkDstList;
    }

private:
    string                   alarmAlias;       //告警别名
    bool                     isUsed;           //联动是否启用，可以理解为是否布防
    TCronTimeList            defenceSched;     //布防撤防时刻表,使用cronTime
    TAlarmLinkSrc            alarmSrc;         //告警源
    CAlarmLinkDstList        alarmDstList;     //联动列表

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TAlarmLinkage>  CAlarmLinkageList;


//开关量输出
#define     IO_OUT_CLOSE    "close"
#define     IO_OUT_OPEN     "open"
struct TIoOutPut
{
public:
    TIoOutPut()
    {
        Clear();
    }
    void Clear()
    {
        outPutId = 0;
        outPutStatus.clear();
    }

private:
    int    outPutId;                        //开关量输出通道0-2 （MAX_IO_OUT_CHNNUM-1）
    string outPutStatus;                    //开关量输出通道状态:  open\close
public:
    void SetOutPutId(int nOutPutId)
    {
        outPutId = nOutPutId;
    }
    int GetOutPutId() const
    {
        return outPutId;
    }

    void SetOutPutStatus(const string& strOutPutStatus)
    {
        outPutStatus = strOutPutStatus;
    }
    string& GetOutPutStatus()
    {
        return outPutStatus;
    }
    const string& GetOutPutStatus() const
    {
        return outPutStatus;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};


/////////////////////////////////////////////////////////////////////
//{{码流交换相关
/////////////////////////////////////////////////////////////////////

const char* GetPayloadTypeName(unsigned int dwPayloadType);

struct TSwitchInfo
{
private:
    TChannel srcChn;      //源
    TChannel dstChn;      //目的
    string   mediaMode;   //交换模式：MODE_VIDEO，MODE_AUDIO，MODE_BOTH
    string   transType;   //传输模式:TRANS_UDP，TRANS_TCP
    string   videoType;   //当前终端编码格式（只对编码器有效)

public:
    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
        mediaMode.clear();
        transType.clear();
        videoType.clear();
    }

    //获取源通道
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    //获取源通道
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    //设置源通道
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //获取目的通道
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    //获取目的通道
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    //设置目的通道
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //获取交换模式：MODE_VIDEO，MODE_AUDIO，MODE_BOTH
    const string& GetMediaMode() const 
    {
        return mediaMode;
    }
    string& GetMediaMode()
    {
        return mediaMode;
    }
    //设置交换模式：MODE_VIDEO，MODE_AUDIO，MODE_BOTH
    void SetMediaMode(const string& strMediaMode) 
    {
        mediaMode = strMediaMode;
    }

    //获取传输模式:TRANS_UDP，TRANS_TCP
    const string& GetTransType() const 
    {
        return transType;
    }
    string& GetTransType()
    {
        return transType;
    }
    //设置传输模式:TRANS_UDP，TRANS_TCP
    void SetTransType(const string& strTransType) 
    {
        transType = strTransType;
    }

    //获取当前终端编码格式（只对编码器有效）
    const string& GetVideoType() const 
    {
        return videoType;
    }
    string& GetVideoType()
    {
        return videoType;
    }
    //设置当前终端编码格式（只对编码器有效）
    void SetVideoType(const string& strVideoType) 
    {
        videoType = strVideoType;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef string TMediaChannelKey;
// struct TMediaChannelKey
// {
// private:
//     std::string key;
// public:
//     TMediaChannelKey()
//     {
// 
//     }
//     TMediaChannelKey(const u8* pBuffer,u16 wLen): key(reinterpret_cast<const s8*>(pBuffer),wLen)
//     {
// 
//     }
// 
//     explicit TMediaChannelKey(const std::string& szString):key(szString)
//     {
//     }
// 
//     TMediaChannelKey(const TMediaChannelKey& buffer)
//     {
//         this->operator =(buffer);
//     }
// 
//     TMediaChannelKey& operator = (const TMediaChannelKey& buffer)
//     {
//         if (&buffer == this)
//         {
//             return *this;
//         }
//         key.assign(buffer.key);
//         return *this;
//     }
// 
//     bool operator == (const TMediaChannelKey& buffer) const
//     {
//         if (&buffer == this)
//         {
//             return true;
//         }
//         return buffer.key == key;
//     }
// 
//     void FromBuf(const u8* pBuffer,u16 wLen)
//     {
//         key.assign((s8*)pBuffer, wLen);
//     }
// 
//     void FromString(const std::string& szString)
//     {
//         key = szString;
//     }
// 
//     bool operator<(const TMediaChannelKey& buffer)const
//     {
//         return key<buffer.key;
//     }
// 
//     const std::string& ToString()const
//     {
//         return key;
//     }
// 
//     const u8* GetBuffer()const
//     {
//         return reinterpret_cast<const u8*>(key.data());
//     }
//     u16 GetLen()const
//     {
//         return key.length();
//     }
//     void Clear()
//     {
//         key = "";
//     }
//     bool IsEmpty()const
//     {
//         return key.empty();
//     }
// 
// public:
//     const std::string ToXml()const;
//     void FromXml(const std::string& xml_string);
//     void ToXml(TiXmlDocument& xmlDoc)const;
//     void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//     void FromXml(const TiXmlNode& xmlNode);
// };

struct TMediaTransProto
{
private:
    TMediaTransport transport;          //RTP
    string          profile;            //AVP
    TLowerTransport lowerTransport;     //TCP,UDP
    TTransCastMode  castMode;           //TRANS_CAST_MODE_XXX: unicast | multicast.

public:
    TMediaTransProto()
    {
        transport = MEDIA_TRANS_RTP;
        profile = "AVP";
        lowerTransport = LOWER_TRANS_UDP;
        castMode = TRANS_CAST_MODE_UNICAST;
    }

    TMediaTransProto(const string& strTransport, const string& strLowerTransport, const string& strCastMode=TRANS_CAST_MODE_UNICAST)
    {
        transport = strTransport;
        lowerTransport = strLowerTransport;
        castMode = strCastMode;
    }

    const TMediaTransport& GetTransport() const
    {
        return transport;
    }
    TMediaTransport& GetTransport()
    {
        return transport;
    }
    void SetTransport(const TMediaTransport& strValue)
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

    const TLowerTransport& GetLowerTransport() const
    {
        return lowerTransport;
    }
    TLowerTransport& GetLowerTransport()
    {
        return lowerTransport;
    }
    void SetLowerTransport(const TLowerTransport& strValue)
    {
        lowerTransport = strValue;
    }

    const TTransCastMode& GetCastMode() const
    {
        return castMode;
    }
    TTransCastMode& GetCastMode()
    {
        return castMode;
    }
    void SetCastMode(const TTransCastMode& strValue)
    {
        castMode = strValue;
    }

public:
    friend std::ostream& operator<<(std::ostream& output, const TMediaTransProto& obj)
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

typedef map<string, string> TMediaFormatParam;
//Video编码参数：
#define VIDEO_ENC_PARAM_RESOLUTION      "VideoResolution"       //视频分辨率。值：string类型,解析格式"W:%d,H:%d"
#define VIDEO_ENC_FRAME_RATE            "VideoFrameRate"        //码流帧率。值：int类型
#define VIDEO_ENC_BIT_RATE              "VideoBitRate"          //码流比特率, Kbps。值：int类型
#define VIDEO_ENC_KEY_FRAME_INTVL       "VideoKeyFrameIntvl"    //码流的关键帧间隔。值：int类型
#define VIDEO_ENC_ENCODE_FORMAT         "VideoEncodeFormat"     //视频编码格式。值：int类型
#define VIDEO_ENC_RATE_TYPE             "VideoRateType"         //码率类型。值：int类型（1-固定码率，2-可变码率）
#define VIDEO_ENC_CLOCK_RATE            "VideoClockRate"        //视频采样率。值：int类型

//Audio编码参数：
#define AUDIO_ENC_SAMPLE_RATE           "AudioSampleRate"       //采样率。值：int类型
#define AUDIO_ENC_CHANNEL_NUM           "AudioChannelNum"       //声道数。值：int类型
#define AUDIO_ENC_ENCODE_FORMAT         "AudioEncodeFormat"     //音频编码格式。值：int类型
#define AUDIO_ENC_BIT_RATE              "AudioBitRate"          //码流比特率, Kbps。值：int类型
#define AUDIO_ENC_CLOCK_RATE            "AudioClockRate"        //时间戳采样率。值：int类型


struct TMediaFormat
{
private:
    int    payloadType;             //编码格式ID
    string encodingName;            //编码格式名，对应于payloadType的名称
    string manufacture;             //媒体流厂商
    TMediaFormatParam formatParam;  //详细的格式参数。根据实际需要设置，比如分辨率、关键帧间隔等

public:
    TMediaFormat()
    {
        SetPayloadType(0);
        manufacture = MEDIA_MANUFACTURE_KEDACOM;
    }

    TMediaFormat(unsigned int dwPayloadType, const string& strManufacture)
    {
        SetPayloadType(dwPayloadType);
        manufacture = strManufacture;
    }

    int GetPayloadType() const
    {
        return payloadType;
    }
    void SetPayloadType(int dwValue);

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

    const TMediaFormatParam& GetFormatParam() const
    {
        return formatParam;
    }
    TMediaFormatParam& GetFormatParam()
    {
        return formatParam;
    }
    void SetFormatParam(const TMediaFormatParam& tValue)
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
        TMediaFormatParam::const_iterator itr = formatParam.find(strKey);
        if (itr != formatParam.end())
        {
            strValue = itr->second;

            return true;
        }

        return false;
    }
    bool GetFormatParam(const string& strKey, int& nValue) const
    {
        TMediaFormatParam::const_iterator itr = formatParam.find(strKey);
        if (itr != formatParam.end())
        {
            nValue = atoi(itr->second.c_str());

            return true;
        }

        return false;
    }
    bool GetFormatParam(const string& strKey, double& lfValue) const
    {
        TMediaFormatParam::const_iterator itr = formatParam.find(strKey);
        if (itr != formatParam.end())
        {
            lfValue = atof(itr->second.c_str());

            return true;
        }

        return false;
    }


public:
    friend std::ostream& operator<<(std::ostream& output, const TMediaFormat& obj)
    {
        output<<std::endl;
        output<<"payloadType: "<<obj.payloadType <<"[" <<obj.encodingName<<"]"<<std::endl;
        output<<"manufacture: "<<obj.manufacture<<std::endl;

        if(obj.formatParam.size() > 0)
        {
            TMediaFormatParam::const_iterator itr = obj.formatParam.begin();
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

//struct TInOutNetAddrPair
//{
//private:
//    TNetAddr innerAddr;    //内网地址
//    TNetAddr outerAddr;    //对应的外网地址
//
//public:
//    const TNetAddr& GetInnerAddr() const
//    {
//        return innerAddr;
//    }
//    TNetAddr& GetInnerAddr()
//    {
//        return innerAddr;
//    }
//    void SetInnerAddr(const TNetAddr& tValue)
//    {
//        innerAddr = tValue;
//    }
//
//    const TNetAddr& GetOuterAddr() const
//    {
//        return outerAddr;
//    }
//    TNetAddr& GetOuterAddr()
//    {
//        return outerAddr;
//    }
//    void SetOuterAddr(const TNetAddr& tValue)
//    {
//        outerAddr = tValue;
//    }
//
//public:
//    const std::string ToXml()const;
//    void FromXml(const std::string& xml_string);
//    void ToXml(TiXmlDocument& xmlDoc)const;
//    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
//    void FromXml(const TiXmlNode& xmlNode);
//};
//typedef vector<TInOutNetAddrPair> TInOutNetAddrPairList;

struct TMediaTransChannel
{
private:
    TTransChannel transChnType;     //RTP,RTCP,DATA
    TMediaChannelKey mediaChnKey;   //udp传输时，Nat打洞用的key
    TNetAddrList addrList;          //内网地址放前面，外网地址添加在后边
    TNetAddr sendBindAddr;         //用于伪装发送地址

public:

    TMediaTransChannel()
    {
    }

	void CopyBasicInfoFrom(const TMediaTransChannel& var)
	{
		transChnType = var.transChnType;
        mediaChnKey = var.mediaChnKey;
	}

public:
    TMediaTransChannel(const string& strTransChnType)
    {
        transChnType = strTransChnType;
    }

    TMediaTransChannel(const string& strTransChnType, const TNetAddrList tAddrList)
    {
        transChnType = strTransChnType;
        addrList = tAddrList;
    }

    TMediaTransChannel(const string& strTransChnType, const TMediaChannelKey& tMediaChnKey, const TNetAddrList tAddrList)
    {
        transChnType = strTransChnType;
        mediaChnKey = tMediaChnKey;
        addrList = tAddrList;
    }

    const TTransChannel& GetTransChnType() const
    {
        return transChnType;
    }
	TTransChannel& GetTransChnType()
	{
		return transChnType;
	}
    void SetTransChnType(const TTransChannel& strValue)
    {
        transChnType = strValue;
    }

    const TMediaChannelKey& GetMediaChannelKey() const
    {
        return mediaChnKey;
    }
    TMediaChannelKey& GetMediaChannelKey()
    {
        return mediaChnKey;
    }
    void SetMediaChannelKey(const TMediaChannelKey& tValue)
    {
        mediaChnKey = tValue;
    }

    const TNetAddrList& GetAddrList() const
    {
        return addrList;
    }
    TNetAddrList& GetAddrList()
    {
        return addrList;
    }
    void SetAddrList(const TNetAddrList& tValue)
    {
        addrList = tValue;
    }
    void AddAddrAtFront(const TNetAddr& tValue)
    {
        TNetAddrList::iterator itr = find(addrList.begin(), addrList.end(), tValue);
        if (itr != addrList.end())
        {
            return;
        }

        addrList.insert(addrList.begin(), tValue);
    }
    void AddAddrAtBack(const TNetAddr& tValue)
    {
        TNetAddrList::iterator itr = find(addrList.begin(), addrList.end(), tValue);
        if (itr != addrList.end())
        {
            return;
        }

        addrList.push_back(tValue);
    }
    void ResetAddr(const TNetAddr& tValue)
    {
        addrList.clear();
        addrList.push_back(tValue);
    }

    void SetSendBindAddr(const TNetAddr& cSendBindAddr)
    {
        sendBindAddr = cSendBindAddr;
    }
    TNetAddr& GetSendBindAddr()
    {
        return sendBindAddr;
    }
    const TNetAddr& GetSendBindAddr() const
    {
        return sendBindAddr;
    }

public:
    friend std::ostream& operator<<(std::ostream& output, const TMediaTransChannel& obj)
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
typedef vector<TMediaTransChannel> TMediaTransChannelList;

static std::ostream& operator<<(std::ostream& output, const TMediaTransChannelList& obj)
{
    TMediaTransChannelList::const_iterator itr = obj.begin();
    for(; itr != obj.end(); itr++)
    {
        output<<*itr;
    }

    return output;
}

class TMediaDesc
{
private:
    TMidaType               mediaType;              //媒体类型：VIDEO，AUDIO，DATA
    TMediaFormat            mediaFormat;            //媒体格式
    TMediaTransProto        mediaTransProto;        //如：RTP/AVP/UDP;unicast
    TMediaTransChannelList  mediaTransChnList;      //媒体传输通道。UDP传输时，通常是RTP与RTCP通道；数据传输或TCP传输时，单通道就可以

public:
    TMediaDesc()
    {
    }

	void CopyBasicInfoFrom(const TMediaDesc& var)
    {
        mediaType = var.mediaType;
        mediaTransProto = var.mediaTransProto;
		mediaTransChnList.clear();
		for(TMediaTransChannelList::const_iterator itrMediaTransChn = var.mediaTransChnList.begin(); itrMediaTransChn!=var.mediaTransChnList.end(); itrMediaTransChn++)
		{
			TMediaTransChannel tMediaTransChannel;
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
            for(TMediaTransChannelList::const_iterator itrMediaTransChn = mediaTransChnList.begin(); itrMediaTransChn!=mediaTransChnList.end(); itrMediaTransChn++)
            {
                if(itrMediaTransChn->GetAddrList().size() != 0)
                {
                    return true;
                }
            }

            return false;
        }
    }

    TMediaTransChannel* FindMediaTransChnByChnKey(const TMediaChannelKey& tMediaChnKey)
    {
        TMediaTransChannelList::iterator itrTransChn = mediaTransChnList.begin();
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
    const TMidaType& GetMediaType() const
    {
        return mediaType;
    }
	TMidaType& GetMediaType()
	{
		return mediaType;
	}
    void SetMediaType(const TMidaType& strValue)
    {
        mediaType = strValue;
    }

    //媒体格式接口
    const TMediaFormat& GetMediaFormat() const
    {
        return mediaFormat;
    }
	TMediaFormat& GetMediaFormat()
	{
		return mediaFormat;
	}
    void SetMediaFormat(const TMediaFormat& tValue)
    {
        mediaFormat = tValue;
    }

    //媒体传输协议接口
    const TMediaTransProto& GetMediaTransProto() const
    {
        return mediaTransProto;
    }
    TMediaTransProto& GetMediaTransProto()
    {
        return mediaTransProto;
    }
    void SetMediaTransProto(const TMediaTransProto& tValue)
    {
        mediaTransProto = tValue;
    }

    //媒体传输通道接口
    const TMediaTransChannelList& GetMediaTransChnList() const
    {
        return mediaTransChnList;
    }
    TMediaTransChannelList& GetMediaTransChnList()
    {
        return mediaTransChnList;
    }
	void SetMediaTransChnList(const TMediaTransChannelList& tValue)
	{
        mediaTransChnList = tValue;
	}

	void AddMediaTransChn(const TMediaTransChannel& tValue)
	{
        mediaTransChnList.push_back(tValue);
	}

	s32 GetMediaTransChnNum() const
	{
		return mediaTransChnList.size();
	}

	//const TMediaTransChannel& GetMediaTransChn(s32 nIdx) const
	//{
	//	return mediaTransChnList[nIdx];
	//}
	//void AddMediaTransChn(s32 nIdx, const TMediaTransChannel& tValue)
	//{
	//	mediaTransChnList[nIdx] = tValue;
	//}

public:
    friend std::ostream& operator<<(std::ostream& output, const TMediaDesc& obj)
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

typedef std::vector<TMediaDesc> TMediaDescList;

static std::ostream& operator<<(std::ostream& output, const TMediaDescList& obj)
{
    TMediaDescList::const_iterator itr = obj.begin();
    for(; itr != obj.end(); itr++)
    {
        output<<*itr;
    }

    return output;
}


struct TInviteSubject 
{
protected:
	TChannel    srcChn;     //码流发送方
	TChannel    dstChn;     //码流接收方

public:

	TInviteSubject(void) 
	{
		Clear();
	}

	TInviteSubject(TChannel tDstChn, TChannel tSrcChn) 
	{
		dstChn = tDstChn;
		srcChn = tSrcChn;
	}

	void Clear()
	{
		srcChn.Clear();
		dstChn.Clear();
	}

public:
	//srcChn
	const TChannel& GetSrcChn() const 
	{
		return srcChn;
	}
	TChannel& GetSrcChn() 
	{
		return srcChn;
	}
	void SetSrcChn(const TChannel& tSrcChn) 
	{
		srcChn = tSrcChn;
	}

	//dstChn
	const TChannel& GetDstChn() const 
	{
		return dstChn;
	}
	TChannel& GetDstChn() 
	{
		return dstChn;
	}
	void SetDstChn(const TChannel& tDstChn) 
	{
		dstChn = tDstChn;
	}

public:
    friend std::ostream& operator<<(std::ostream& output, const TInviteSubject& obj)
    {
        output << obj.dstChn << " <- "<<obj.srcChn;
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


typedef string TSdpSessionName;
#define SDP_SESSION_PLAY        "Play"              //实时码流
#define SDP_SESSION_PLAYBACK    "Playback"          //历史码流回放
#define SDP_SESSION_DOWNLOAD    "Download"          //历史码流下载
#define SDP_SESSION_AUDIOCALL   "AudioCall"         //音频呼叫

typedef string TPlaybackType;
#define PLAYBACK_TYPE_PLATFORM    "PLATFORM_PLAYBACK"   //平台放像
#define PLAYBACK_TYPE_PU          "PU_PLAYBACK"         //前端放像


//******************************************************************************
// 函数名:    IsRealTimeMediaSession
// 功能:      判断是否实施码流会话。实施码流交换需要分发，历史码流不分发
// 算法实现:  
// 访问权限:  public 
// 参数说明:  const TSdpSessionName & tSdpSessionName
// 返回值:    bool
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2013/03/19     huangzhichun huangzhichun    创建
//******************************************************************************
inline bool IsRealTimeMediaSession(const TSdpSessionName& tSdpSessionName)
{
    return tSdpSessionName == SDP_SESSION_PLAY || tSdpSessionName == SDP_SESSION_AUDIOCALL;
}

struct TSdpUri
{
private:
    TRecToken   recToken;                     //录像文件url
    TChannel    puChn;              
    TPlaybackType playbackType;    
    string      recDomainName;                  // 录像所在域

public:
    TSdpUri()
    {
		Clear();
    }

    void Clear()
    {
        recToken.clear();
        puChn.Clear();
        playbackType.clear();
    }

    bool IsEmpty()
    {
        if (recToken.empty() && puChn.IsNull())
        {
            return true;
        }

        return false;
    }

    friend std::ostream& operator<<(std::ostream& output, const TSdpUri& obj)
    {
        output<<std::endl;
        output<<"recToken: "<<obj.recToken<<std::endl;
        output<<"puChn: "<<obj.puChn<<std::endl;
        output<<"playbackType: "<<obj.playbackType<<std::endl;
        output<<"recDomainName"<<obj.recDomainName<<std::endl;

        return output;
    }

public:
    void SetRecToken(const TRecToken& strRecToken)
    {
        recToken = strRecToken;
    }
    TRecToken& GetRecToken()
    {
        return recToken;
    }
    const TRecToken& GetRecToken() const
    {
        return recToken;
    }

    void SetPuChn(const TChannel& cPuChn)
    {
        puChn = cPuChn;
    }
    TChannel& GetPuChn()
    {
        return puChn;
    }
    const TChannel& GetPuChn() const
    {
        return puChn;
    }

    void SetPlaybackType(const TPlaybackType& strPlaybackType)
    {
        playbackType = strPlaybackType;
    }
    TPlaybackType& GetPlaybackType()
    {
        return playbackType;
    }
    const TPlaybackType& GetPlaybackType() const
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

struct TStreamTag
{
private:
    string manufacture;
    TMediaStreamType mediaStreamType;

public:
    TStreamTag()
    {

    }

    TStreamTag(const string& strManufacture, const TMediaStreamType& tMediaStreamType)
    {
        manufacture = strManufacture;
        mediaStreamType = tMediaStreamType;
    }

    void Clear()
    {
        manufacture.clear();
        mediaStreamType.clear();
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

    void SetMediaStreamType(const TMediaStreamType& cMediaStreamType)
    {
        mediaStreamType = cMediaStreamType;
    }
    TMediaStreamType& GetMediaStreamType()
    {
        return mediaStreamType;
    }
    const TMediaStreamType& GetMediaStreamType() const
    {
        return mediaStreamType;
    }

    bool Parse(const string& strStreamTag)
    {
        char achManufacture[128];
        char achMediaStreamType[128];
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

typedef std::map<string, string> TExtInfo;
static const string SDP_EXT_INFO_PLAYBACK_SSN = "PlaybackSsn";			//放像任务会话Uri; 放像INVITE_RSP时，由最终被请求的设备端生成，后续Vcr控制和进度查询时要用。值：string类型
static const string SDP_EXT_INFO_PLAYBACK_START_TIME = "PlaybackStartTime";	//平台录像为支持倒放时，放像INVITE_RSP返回给CU的起始放像时间。值：int类型
static const string SDP_EXT_INFO_FILE_PATH = "FilePath";				//录像下载INVITE_RSP时，由录像模块(NRU)生成，客户端RPCTRL要用。值：string类型
static const string SDP_EXT_INFO_FILE_NAME = "FileName";				//录像下载INVITE_RSP时，由录像模块(NRU)生成，客户端RPCTRL要用。值：string类型
static const string SDP_EXT_INFO_STREAM_TAG = "StreamTag";	            //码流标签(格式"厂商:MEDIA_STREAM_TYPE",如kedacom:RTP_ES)

static const string SDP_EXT_INFO_DIRECT_TRANSFER = "DirectTransfer";	//是否优先直传, 值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_PLAT_DIRECT_TRANSFER = "PlatDirectTransfer";	//是否优先平台直传, 值：TRUE_STR/FALSE_STR

static const string SDP_EXT_INFO_OFFLINE_SWITCH = "OfflineSwitch";		//是否支持离线交换; INVITE_REQ给VTDU时携带; 值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_NETSEG_NO = "NetSegNO";	            //网段号; INVITE_REQ给VTDU时携带; 值：int类型
static const string SDP_EXT_INFO_IS_NEED_PUNCH = "NeedPunch";	        //是否需要Punch
static const string SDP_EXT_INFO_IS_NEED_GBPUNCH = "NeedGBPunch";	    //是否需要GBPunch
static const string SDP_EXT_INFO_GBPUNCH_GBCHNID = "GBPunchChnId";	    //GBPunch的通道ID
static const string SDP_EXT_INFO_GBPUNCH_GBPLATID = "GBPunchPlatId";	//GBPunch的平台ID
static const string SDP_EXT_INFO_USE_DOMAIN_SOCKET = "DomainSocket";	//是否使用域套接字，值：TRUE_STR/FALSE_STR

static const string SDP_EXT_INFO_DROP_RTP_EXTEND = "DropRtpExtend";     //是否去Rtp扩展头，值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_USE_AES256_ENCRYPTION = "AES256Encryption";//是否使用AES256加密，值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE = "MrtcEnable";            //是否启用MRTC，值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE_SRC = "MrtcEnableSrc";     //是否启用来源侧MRTC，缺省启用，值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE_DST = "MrtcEnableDst";     //是否启用目的侧MRTC，缺省启用，值：TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_ADDR_CHANGE_SIDE = "AddrChangeSide";   //修改地址一侧，值："SRC"/"DST"
const string ADDR_CHANGE_SIDE_SRC = "SRC";
const string ADDR_CHANGE_SIDE_DST = "DST";

static const string SDP_EXT_INFO_IS_NAT_PACKET_MODE = "NatPacketMode";  //是否NAT探测包模式
static const string SDP_EXT_INFO_INTERFACE_IP = "InterfaceIp";          //接口IP地址

static const string SDP_EXT_INFO_TIMESTAMP_TYPE = "TimestampType";          //时间戳类型，值为TIMESTAMP_TYPE_STREAM或者TIMESTAMP_TYPE_PROGRESS
#define TIMESTAMP_TYPE_STREAM   ("Stream")
#define TIMESTAMP_TYPE_PROGRESS   ("Progress")

static const string SDP_EXT_INFO_SECURE_TYPE = "SecureType";             //安全类型， 值为 SDP_EXT_INFO_SECURE/UNSECURE, 若为检测出该键 则视为不安全
static const string SDP_EXT_INFO_VKEK_LIST   = "VkekList";               // vkek列表， 值 为  vector moduleVkek
static const string SDP_EXT_INFO_MTS_BT_RTP_IP    = "mts_bt_rtp_ip";     // vkek列表， 值
static const string SDP_EXT_INFO_MTS_BT_RTP_PORT  = "mts_bt_rtp_port";   // vkek列表， 值
static const string SDP_EXT_INFO_MTS_AT_RTP_IP	  = "mts_at_rtp_ip";     // vkek列表， 值
static const string SDP_EXT_INFO_MTS_AT_RTP_PORT  = "mts_at_rtp_port";   // vkek列表， 值
static const string SDP_EXT_INFO_MTS_AT_RTCP_IP	  = "mts_at_rtcp_ip";    // vkek列表， 值
static const string SDP_EXT_INFO_MTS_AT_RTCP_PORT = "mts_at_rtcp_port";  // vkek列表， 值

static const string SDP_EXT_INFO_TRANSFER_RATE = "Rate";                  //传输倍速，值：int类型
static const string SDP_EXT_INFO_FILE_SIZE = "FileSize";                  //文件大小，值：double类型（字节）

static const string SDP_EXT_INFO_RESOURCE_ID = "ResourceId";	          //资源id（字符串）
static const string SDP_EXT_INFO_SWITCH_FLOW = "CmuFlow";	          //交换流程（目前有两种，一种是基于cmu调度的老流程，一种是基于redis的）
#define SWITCH_FLOW_CMU ("Cmu")
#define SWITCH_FLOW_REDIS   ("Redis")

static const string SDP_EXT_INFO_TCPSTREAM = "TcpStream";               //tcp码流，国标透传扩展信息，值：“PASSIVE”/“ACTIVE”
//{{{{{{{{{{{{{{{ modify by Royan 20140605 国标流保活
static const string SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO = "GBStreamKAInfo"; //国标流保活类型
#define STREAM_KA_SR        ("SR")
#define STREAM_KA_RR        ("RR")
#define STREAM_KA_SR_RR     ("SRRR")
//}}}}}}}}}}}}}}} modify by Royan 20140605 国标流保活
//{{{{{{{{{{{{{{{ add by Royan 20140903 SSRC
static const string SDP_EXT_INFO_SSRC = "SSRC";	                        //SSRC
//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC
static const string SDP_EXT_INFO_GB_REC_LOCATION = "GBRecLocation";     //指定国标录像位置（值：国标域ID或者国标前端ID）
static const string SDP_EXT_INFO_GMT_LABEL = "GMTLabel";                //是否支持夏令时GMT格式


typedef std::vector<TSecurityModuleVkek> VKEK_GROUP;

struct TVkekList
{
public:
	TVkekList()
	{
		Clear();
	}
	void Clear()
	{
		vkeks.clear();
	}
public:
	VKEK_GROUP vkeks;

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

//放像INVITE_RSP携带的扩展信息:
//SDP_EXT_INFO_PLAYBACK_SSN, SDP_EXT_INFO_PLAYBACK_START_TIME
//录像下载INVITE_RSP携带的扩展信息:
//SDP_EXT_INFO_PLAYBACK_SSN，SDP_EXT_INFO_FILE_PATH, SDP_EXT_INFO_FILE_NAME

//实时码流INVITE_REQ:
//SDP_EXT_INFO_OFFLINE_SWITCH, SDP_EXT_INFO_DIRECT_TRANSFER

//******************************************************************************
// 函数名:    TSdp
// 功能:      会话描述协议体
// 算法实现:  

// TSdp媒体描述列表 创建示例：
//推荐：
//TSdp tSdp;
//
//TMediaDesc tVideoMediaDesc;
//tVideoMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
//tVideoMediaDesc.SetMediaFormat();
//tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));
//tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
//tSdp.AddMediaDesc(tVideoMediaDesc);
//
//TMediaDesc tAudioMediaDesc;
//tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
//tAudioMediaDesc.SetMediaFormat();
//tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));
//tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
//tSdp.AddMediaDesc(tAudioMediaDesc);

//或者
//TSdp tSdp;
//
//CMediaDescListAdapter CMediaDescListAdapter(tSdp.GetMediaDescList());
//
//TMediaFormat tMediaFormat;
//tMediaFormat.SetXXX();
//CMediaDescListAdapter.SetMediaFormat(MEDIA_TYPE_VIDEO, tMediaFormat);

//CMediaDescListAdapter.AddMediaTransChannel(MEDIA_TYPE_VIDEO, MEDIA_TRANS_CHN_RTP, tAddrList);
//CMediaDescListAdapter.AddMediaTransChannel(MEDIA_TYPE_VIDEO, MEDIA_TRANS_CHN_RTCP, tAddrList);
//
//tMediaFormat.SetXXX();
//CMediaDescListAdapter.SetMediaFormat(MEDIA_TYPE_AUDIO, tMediaFormat);
//
//CMediaDescListAdapter.AddMediaTransChannel(MEDIA_TYPE_AUDIO, MEDIA_TRANS_CHN_RTP, tAddrList);
//CMediaDescListAdapter.AddMediaTransChannel(MEDIA_TYPE_AUDIO, MEDIA_TRANS_CHN_RTCP, tAddrList);
//
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2011/11/04     huangzhichun huangzhichun    创建
//******************************************************************************
struct TSdp
{
private:
    TChannel            owner;              //sdp的创建者，即mediaDescList是属于谁的
    TSdpSessionName     sessionName;        //操作类型
    TSdpUri             uri;                //放像、下载INVITE_REQ时必须填；浏览时不填
    TTimeRange          timeRange;          //放像、下载INVITE_REQ时必须填；浏览时可以不填
    TMediaDescList      mediaDescList;
    TExtInfo            extInfo;

public:
    TSdp()
    {
        Clear();
    }

    void Clear()
    {
        owner.Clear();
        sessionName.clear();
        uri.Clear();
        timeRange.Clear();
        mediaDescList.clear();
        extInfo.clear();
    }

    void CreateEmptyMediaList(const TSdpSessionName& tSessionName = TSdpSessionName())
    {
        if(!tSessionName.empty())
        {
            sessionName = tSessionName;
        }

        this->mediaDescList.clear();

        if(SDP_SESSION_AUDIOCALL == sessionName)
        {            
            TMediaDesc tAudioMediaDesc;
            tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
            //tAudioMediaDesc.SetMediaFormat();
            tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP));
            tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP));
            this->AddMediaDesc(tAudioMediaDesc);
        }
        else
        {
            TMediaDesc tVideoMediaDesc;
            tVideoMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
            //tVideoMediaDesc.SetMediaFormat();
            tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP));
            tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP));
            this->AddMediaDesc(tVideoMediaDesc);
            
            TMediaDesc tAudioMediaDesc;
            tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
            //tAudioMediaDesc.SetMediaFormat();
            tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP));
            tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP));
            this->AddMediaDesc(tAudioMediaDesc);
        }
    }

    void CopySdpHeaderFrom(const TSdp& var)
    {
        sessionName = var.sessionName;
        uri = var.uri;
        timeRange = var.timeRange;
    }

	void CopyBasicInfoFrom(const TSdp& var)
	{
        CopySdpHeaderFrom(var);

		mediaDescList.clear();  //似乎可以不用清空原先的信息？？
		for(TMediaDescList::const_iterator itrMediaDesc = var.mediaDescList.begin(); itrMediaDesc!=var.mediaDescList.end(); itrMediaDesc++)
		{
			TMediaDesc tMediaDesc;
			tMediaDesc.CopyBasicInfoFrom(*itrMediaDesc);
			mediaDescList.push_back(tMediaDesc);
		}
	}

    void CopyMediaChnKeyFrom(const TSdp& tSdpFrom)
    {
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //遍历tMediaTransChnList
            TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

            for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                TMediaTransChannel tMediaTransChnFound;
                if (tSdpFrom.FindMediaTransChn(itrMediaDesc->GetMediaType(), itrTransChn->GetTransChnType(), /*OUT*/tMediaTransChnFound))
                {
                    itrTransChn->SetMediaChannelKey(tMediaTransChnFound.GetMediaChannelKey());
                }
                else
                {
                    //在源对象找不到相应的Chn则把Key置空
                    itrTransChn->SetMediaChannelKey(TMediaChannelKey());
                }
            }
        }
    }

    void CopyExtInfoFrom(const TSdp& tSdpFrom)
    {
        const TExtInfo& tExtInfoFrom = tSdpFrom.GetExtInfo();

        for (TExtInfo::const_iterator itr = tExtInfoFrom.begin(); itr!= tExtInfoFrom.end(); itr++)
        {
            if (extInfo.end() == extInfo.find(itr->first))
            {
                extInfo.insert(*itr);
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& output, const TSdp& obj)
    {
        output<<std::endl;
        output<<"owner: "<<obj.owner.ToString()<<std::endl;
        output<<"sessionName: "<<obj.sessionName<<std::endl;
        output<<"uri: "<<obj.uri<<std::endl;
        output<<"timeRange: "<<obj.timeRange.GetStartTime()<<","<<obj.timeRange.GetEndTime()<<std::endl;
        output<<"mediaTransChnList: "<<obj.mediaDescList<<std::endl;
        for (TExtInfo::const_iterator itr = obj.extInfo.begin(); itr != obj.extInfo.end(); itr++)
        {
            output<<itr->first<<"="<<itr->second<<std::endl;
        }

        return output;
    }

    void PrintData(bool bVerbose) const
    {
		if (bVerbose)
		{
			std::ostringstream oss;

			oss.str("");
			oss<<*this;
			OspPrintf(TRUE, FALSE, "Sdp:\n{{%s}}\n", oss.str().c_str());
			OspPrintf(TRUE, FALSE, "--------------------------------------------------\n");
		}
    }

public:
    bool HasMediaDes() const
    {
        if(mediaDescList.size() == 0)
        {
            return false;
        }
        else
        {
            for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc!=mediaDescList.end(); itrMediaDesc++)
            {
                if(itrMediaDesc->HasMediaDes())
                {
                    return true;
                }
            }

            return false;
        }
    }

    void AddMediaDesc(const TMediaDesc& tMediaDesc)
    {
        mediaDescList.push_back(tMediaDesc);
    }

    bool FindMediaDesc(const string& strMeidaType, /*OUT*/TMediaDesc*& ptMediaDesc)
    {
        //遍历TMediaDescList
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            if(itrMediaDesc->GetMediaType() == strMeidaType)
            {
                ptMediaDesc = &(*itrMediaDesc);
                return true;
            }
        }

        return false;
    }

    bool FindMediaTransChn(const string& strMeidaType, const string& strTransChnType, /*OUT*/TMediaTransChannel& tMediaTransChannel) const
    {
        for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            if (itrMediaDesc->GetMediaType() == strMeidaType)
            {
                //遍历tMediaTransChnList
                const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

                for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
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

    bool FindMediaTransChn(const string& strMeidaType, const string& strTransChnType, const TMediaTransChannel*& ptMediaTransChannel) const
    {
        for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            if (itrMediaDesc->GetMediaType() == strMeidaType)
            {
                //遍历tMediaTransChnList
                const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

                for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
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

    bool FindMediaTransChn(const string& strMeidaType, const string& strTransChnType, /*OUT*/TMediaTransChannel*& ptMediaTransChannel)
    {
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            if (itrMediaDesc->GetMediaType() == strMeidaType)
            {
                //遍历tMediaTransChnList
                TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

                for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
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

    bool FindMediaTransChnByChnKey(/*IN*/const TMediaChannelKey& tMediaChnKey, /*OUT*/TMidaType& tMediaType, /*OUT*/TMediaTransChannel*& ptMediaTransChannel)
    {
        //遍历TMediaDescList
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            TMediaTransChannel* ptChannel = itrMediaDesc->FindMediaTransChnByChnKey(tMediaChnKey);
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
        u32 dwTransChnNum = 0;
        TMediaDescList::const_iterator itr = mediaDescList.begin();

        for(; itr != mediaDescList.end(); itr++)
        {
            dwTransChnNum += itr->GetMediaTransChnNum();
        }

        return dwTransChnNum;
    }

    bool IsAllMediaChnKeyEmpty() const
    {
        for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //遍历tMediaTransChnList
            const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();

            for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                if (!itrTransChn->GetMediaChannelKey().empty())
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool IsExistIP(const TIpAddr& tIpAddr) const
    {
        //遍历TMediaDescList
        for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //遍历TMediaTransChannelList
            const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
            for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                //遍历TNetAddrList
                const TNetAddrList& tNetAddrList = itrTransChn->GetAddrList();
                for (TNetAddrList::const_iterator itrAddr = tNetAddrList.begin(); itrAddr != tNetAddrList.end(); itrAddr++)
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

    void TrimAddrExceptFor(const TIpAddr& tIpAddr)
    {
        //遍历TMediaDescList
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //遍历TMediaTransChannelList
            TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
            for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                //遍历TNetAddrList
                TNetAddrList& tNetAddrList = itrTransChn->GetAddrList();
                for (TNetAddrList::iterator itrAddr = tNetAddrList.begin(); itrAddr != tNetAddrList.end(); )
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
    void SetOwner(const TChannel& tOwner)
    {
        owner = tOwner;
    }
    TChannel& GetOwner()
    {
        return owner;
    }
    const TChannel& GetOwner() const
    {
        return owner;
    }

    void SetSessionName(const TSdpSessionName& strSessionName)
    {
        sessionName = strSessionName;
    }
    TSdpSessionName& GetSessionName()
    {
        return sessionName;
    }
    const TSdpSessionName& GetSessionName() const
    {
        return sessionName;
    }

    void SetUri(const TSdpUri& tUri)
    {
        uri = tUri;
    }
    TSdpUri& GetUri()
    {
        return uri;
    }
    const TSdpUri& GetUri() const
    {
        return uri;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

    void SetMediaDescList(const TMediaDescList& cMediaDescList)
    {
        mediaDescList = cMediaDescList;
    }
    TMediaDescList& GetMediaDescList()
    {
        return mediaDescList;
    }
    const TMediaDescList& GetMediaDescList() const
    {
        return mediaDescList;
    }

    //tExtInf
    void SetExtInfo(const TExtInfo& tExtInf)
    {
        extInfo = tExtInf;
    }
    TExtInfo& GetExtInfo()
    {
        return extInfo;
    }
    const TExtInfo& GetExtInfo() const
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
        extInfo[strKey] = BoolStr(bValue);
    }

    bool GetExtInfo(const string& strKey, string& strValue) const
    {
        TExtInfo::const_iterator itr = extInfo.find(strKey);
        if (itr != extInfo.end())
        {
            strValue = itr->second;

            return true;
        }

        return false;
    }
    bool GetExtInfo(const string& strKey, int& nValue) const
    {
        TExtInfo::const_iterator itr = extInfo.find(strKey);
        if (itr != extInfo.end())
        {
            nValue = atoi(itr->second.c_str());

            return true;
        }

        return false;
    }
    bool GetExtInfo(const string& strKey, double& lfValue) const
    {
        TExtInfo::const_iterator itr = extInfo.find(strKey);
        if (itr != extInfo.end())
        {
            lfValue = atof(itr->second.c_str());

            return true;
        }

        return false;
    }

    bool GetExtInfo(const string& strKey, bool& bValue) const
    {
        TExtInfo::const_iterator itr = extInfo.find(strKey);
        if (itr != extInfo.end())
        {
            bValue = (itr->second == TRUE_STR);

            return true;
        }

        return false;
    }

    //TExtInfo细化接口
	void SetGBRecLocation(const string& var)
	{
		extInfo[SDP_EXT_INFO_GB_REC_LOCATION] = var;
	}
	string GetGBRecLocation() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_GB_REC_LOCATION);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}
	//{{{{{{{{{{{{{{{ modify by Royan 20140606 国标流保活
	void SetGBStreamKAInfo(const string& var)
	{
		extInfo[SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO] = var;
	}
	string GetGBStreamKAInfo() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO);
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
		SetExtInfo(SDP_EXT_INFO_SSRC, nVar);
	}
	int GetSSRC() const
	{
		int nVar = 0;

		GetExtInfo(SDP_EXT_INFO_SSRC, nVar);

		return nVar;
	}
	bool GetSSRC(int& nSSRC) const
	{
		return GetExtInfo(SDP_EXT_INFO_SSRC, nSSRC);
	}
	//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC
    void SetPlaybackSessUri(const string& strPlaybackSessUri)
    {
        extInfo[SDP_EXT_INFO_PLAYBACK_SSN] = strPlaybackSessUri;
    }
    string GetPlaybackSessUri() const
    {
        TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_PLAYBACK_SSN);
        if (itr != extInfo.end())
        {
            return itr->second;
        }

        return "";
    }

    void SetPlaybackStartTime(int nVar)
    {
        SetExtInfo(SDP_EXT_INFO_PLAYBACK_START_TIME, nVar);
    }
    int GetPlaybackStartTime() const
    {
        int nVar = 0;

        GetExtInfo(SDP_EXT_INFO_PLAYBACK_START_TIME, nVar);

        return nVar;
    }

    void SetDLFimePath(const string& strVar)
    {
        extInfo[SDP_EXT_INFO_FILE_PATH] = strVar;
    }
    string GetDLFimePath() const
    {
        TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_FILE_PATH);
        if (itr != extInfo.end())
        {
            return itr->second;
        }

        return "";
    }

    void SetDLFimeName(const string& strVar)
    {
        extInfo[SDP_EXT_INFO_FILE_NAME] = strVar;
    }
    string GetDLFimeName() const
    {
        TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_FILE_NAME);
        if (itr != extInfo.end())
        {
            return itr->second;
        }

        return "";
    }

    void SetOfflineSwitch(bool bEnable)
    {
        SetExtInfo(SDP_EXT_INFO_OFFLINE_SWITCH, bEnable);
    }
    bool GetOfflineSwitch() const
    {
        bool bVar = false;

        GetExtInfo(SDP_EXT_INFO_OFFLINE_SWITCH, bVar);

        return bVar;
    }

	void SetDirectTransfer(bool bEnable)
	{
        SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, bEnable);
	}
	bool GetDirectTransfer() const
	{
        bool bVar = false;

        GetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, bVar);

        return bVar;
	}

	void SetPlatDirectTransfer(bool bEnable)
	{
		SetExtInfo(SDP_EXT_INFO_PLAT_DIRECT_TRANSFER, bEnable);
	}
	bool GetPlatDirectTransfer() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_PLAT_DIRECT_TRANSFER, bVar);

		return bVar;
	}
    
    void SetNetSegNo(int nVar)
    {
        SetExtInfo(SDP_EXT_INFO_NETSEG_NO, nVar);
    }
    int GetNetSegNo() const
    {
        int nVar = 0;
        
        GetExtInfo(SDP_EXT_INFO_NETSEG_NO, nVar);

        return nVar;
    }

	void SetIsNeedPunch(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_IS_NEED_PUNCH, bVar);
	}
	bool GetIsNeedPunch() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_IS_NEED_PUNCH, bVar);

		return bVar;
	}

	void SetIsNeedGBPunch(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_IS_NEED_GBPUNCH, bVar);
	}
	bool GetIsNeedGBPunch() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_IS_NEED_GBPUNCH, bVar);

		return bVar;
	}

	void SetGBPunchChnID(const string& strVar)
	{
		extInfo[SDP_EXT_INFO_GBPUNCH_GBCHNID] = strVar;
	}
	string GetGBPunchChnID() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_GBPUNCH_GBCHNID);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetGBPunchPlatID(const string& strVar)
	{
		extInfo[SDP_EXT_INFO_GBPUNCH_GBPLATID] = strVar;
	}
	string GetGBPunchPlatID() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_GBPUNCH_GBPLATID);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}

	void SetDomainSocket(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_USE_DOMAIN_SOCKET, bVar);
	}
	bool GetDomainSocket() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_USE_DOMAIN_SOCKET, bVar);

		return bVar;
	}

	void SetDropRtpExtend(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);
	}
	bool GetDropRtpExtend() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);

		return bVar;
	}

	void SetAES256Encryption(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_USE_AES256_ENCRYPTION, bVar);
	}
	bool GetAES256Encryption() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_USE_AES256_ENCRYPTION, bVar);

		return bVar;
	}

	void SetMrtcEnable(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_MRTC_ENABLE, bVar);
	}
	bool GetMrtcEnable() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_MRTC_ENABLE, bVar);

		return bVar;
	}

	void SetIsNatPacketMode(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_IS_NAT_PACKET_MODE, bVar);
	}
	bool GetIsNatPacketMode() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_IS_NAT_PACKET_MODE, bVar);

		return bVar;
	}

	void SetInterfaceIp(const string& var)
	{
		extInfo[SDP_EXT_INFO_INTERFACE_IP] = var;
	}
	string GetInterfaceIp() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_INTERFACE_IP);
		if (itr != extInfo.end())
		{
			return itr->second;
		}

		return "";
	}


    void SetStreamTag(const string& strVar)
    {
        extInfo[SDP_EXT_INFO_STREAM_TAG] = strVar;
    }
    string GetStreamTag() const
    {
        TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_STREAM_TAG);
        if (itr != extInfo.end())
        {
            return itr->second;
        }

        return "";
    }

    void SetStreamTag(const TStreamTag& tStreamTag)
    {
        SetStreamTag(tStreamTag.ToString());
    }
    bool GetStreamTag(TStreamTag& tStreamTag) const
    {
        return tStreamTag.Parse(GetStreamTag());
    }

    void SetTransferRate(int nVar)
    {
        SetExtInfo(SDP_EXT_INFO_TRANSFER_RATE, nVar);
    }

    int GetTransferRate() const
    {
        int nVar = 1;

        GetExtInfo(SDP_EXT_INFO_TRANSFER_RATE, nVar);

        return nVar;
    }

    void SetFileSize(double lfVar)
    {
        SetExtInfo(SDP_EXT_INFO_FILE_SIZE, lfVar);
    }

    double GetFileSize() const
    {
        double lfVar = 0.00;

        GetExtInfo(SDP_EXT_INFO_FILE_SIZE, lfVar);

        return lfVar;
    }

    void SetResourceId(const string &strId)
    {
        SetExtInfo(SDP_EXT_INFO_RESOURCE_ID, strId);
    }

    string GetResourceId() const
    {
        string strId;

        GetExtInfo(SDP_EXT_INFO_RESOURCE_ID, strId);

        return strId;
    }

    void SetSwitchFlow(const string &strFlow)
    {
        SetExtInfo(SDP_EXT_INFO_SWITCH_FLOW, strFlow);
    }

    string GetSwitchFlow() const
    {
        string strFlow;

        GetExtInfo(SDP_EXT_INFO_SWITCH_FLOW, strFlow);

        return strFlow;
    }

	void SetTcpStream(const string &strTcpType)
	{
		SetExtInfo(SDP_EXT_INFO_TCPSTREAM, strTcpType);
	}
	string GetTcpStream() const
	{
		string strTcpType;

		GetExtInfo(SDP_EXT_INFO_TCPSTREAM, strTcpType);

		return strTcpType;
	}

	void SetVkekList(const TVkekList &vkeks )
	{
		SetExtInfo( SDP_EXT_INFO_VKEK_LIST, vkeks.ToXml() );
	}

	bool  GetVkekList(TVkekList &vkeks) const
	{
		vkeks.Clear();
		string strVkeks;
		bool result = GetExtInfo( SDP_EXT_INFO_VKEK_LIST, strVkeks );
		if( !result )
		{
			return false;
		}
		vkeks.FromXml( strVkeks );
		return true;
	}

	void SetSecure( bool isSecure )
	{
		SetExtInfo( SDP_EXT_INFO_SECURE_TYPE, isSecure );
	}

	bool GetSecure() const
	{
		bool result = false;
		bool isSecure = false;
		result = GetExtInfo( SDP_EXT_INFO_SECURE_TYPE, isSecure );
		return result && isSecure;
	}

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef unsigned int TSwitchSsnID;
#define INVALID_SWITCH_SSN_ID (TSwitchSsnID)0

//交换通道
class TSwitchChannel
{
protected:
    TSwitchSsnID        switchSsnID;            //交换的会话ID
    string              mediaType;              //媒体类型：VIDEO，AUDIO
    TMediaFormat        mediaFormat;            //媒体格式
    TMediaTransProto    mediaTransProto;        //如：RTP/AVP/UDP;unicast
	TTransChannel		transChnType;           //RTP,RTCP,DATA
    TSwitchType         switchType;             //VTDU的转发交换类型: M2P/P2P;
    TMediaStreamType    mediaStreamType;         //PS/ES
	bool			    isGapSwitch;            //是否穿越网闸
	bool                isDomainSocket;			//是否使用域socket
    bool                isUseRecvProxy;         //DS是否使用接收代理，模拟半源半端口转发

    TChannel            srcDevChn;
    TMediaChannelKey    srcMediaChnKey;
    TNetAddrList        srcNetAddrList;

    TChannel            dstDevChn;
    TMediaChannelKey    dstMediaChnKey;
    TNetAddrList        dstNetAddrList;

    TNetAddr            rcvNetAddr;
    TNetAddr            rcvBindNetAddr; //HZC:组播接收的时候才填
    TNetAddr            sndBindNetAddr; //HZC:用RawSocket伪装发送的时候才需要填

	TExtInfo            extInfo;

public:
    TSwitchChannel()
    {
        switchSsnID = 0;
        switchType = SWITCH_TYPE_M2P;
        isGapSwitch = false;
		isDomainSocket = false;
        isUseRecvProxy = false;
		extInfo.clear();
    }

    const TSwitchSsnID& GetSwitchSsnID() const
    {
        return switchSsnID;
    }
	TSwitchSsnID& GetSwitchSsnID()
	{
		return switchSsnID;
	}
    void SetSwitchSsnID(const TSwitchSsnID& tValue)
    {
        switchSsnID = tValue;
    }

	const string& GetMediaType() const
	{
		return mediaType;
	}
	string& GetMediaType()
	{
		return mediaType;
	}
	void SetMediaType(const string& strValue)
	{
		mediaType = strValue;
	}

	const TMediaFormat& GetMediaFormat() const
	{
		return mediaFormat;
	}
	TMediaFormat& GetMediaFormat()
	{
		return mediaFormat;
	}
	void SetMediaFormat(const TMediaFormat& tValue)
	{
		mediaFormat = tValue;
	}

	const TMediaTransProto& GetMediaTransProto() const
	{
		return mediaTransProto;
	}
	TMediaTransProto& GetMediaTransProto()
	{
		return mediaTransProto;
	}
	void SetMediaTransProto(const TMediaTransProto& tValue)
	{
		mediaTransProto = tValue;
	}

	const TTransChannel& GetTransChnType() const
	{
		return transChnType;
	}
	TTransChannel& GetTransChnType()
	{
		return transChnType;
	}
	void SetTransChnType(const TTransChannel& var)
	{
		transChnType = var;
	}

	TSwitchType GetSwitchType() const
	{
		return switchType;
	}
	void SetSwitchType(TSwitchType tValue)
	{
		switchType = tValue;
	}

    const TMediaStreamType& GetMediaStreamType() const
    {
        return mediaStreamType;
    }
    void SetMediaStreamType( const TMediaStreamType &type )
    {
        mediaStreamType = type;
    }

	void SetGapSwitch(bool bValue)
	{
		isGapSwitch = bValue;
	}
	bool IsGAPSwitch() const
	{
		return isGapSwitch;
	}

	void SetDomainSocket(bool bValue)
	{
		isDomainSocket = bValue;
	}
	bool IsDomainSocket() const
	{
		return isDomainSocket;
	}

    const TChannel& GetSrcDevChn() const
    {
        return srcDevChn;
    }
    TChannel& GetSrcDevChn()
    {
        return srcDevChn;
    }
    void SetSrcDevChn(const TChannel& cValue)
    {
        srcDevChn = cValue;
    }

    //源Key接口
	const TMediaChannelKey& GetSrcMediaChnKey() const
	{
		return srcMediaChnKey;
	}
    TMediaChannelKey& GetSrcMediaChnKey()
	{
		return srcMediaChnKey;
	}
	void SetSrcMediaChnKey(const TMediaChannelKey& cValue)
	{
		srcMediaChnKey = cValue;
	}

    //源地址接口
    const TNetAddrList& GetSrcNetAddrList() const
    {
        return srcNetAddrList;
    }
    TNetAddrList& GetSrcNetAddrList()
    {
        return srcNetAddrList;
    }
	void SetSrcNetAddrList(const TNetAddrList& tValue)
	{
        srcNetAddrList = tValue;
	}

	int GetSrcNetAddrNum() const
	{
		return srcNetAddrList.size();
	}

	const TNetAddr& GetSrcNetAddr(s32 nIdx) const
	{
		return srcNetAddrList[nIdx];
	}
    TNetAddr& GetSrcNetAddr(s32 nIdx)
	{
		return srcNetAddrList[nIdx];
	}

	void AddSrcNetAddr(const TNetAddr& tValue)
	{
        srcNetAddrList.push_back(tValue);
	}

    const TChannel& GetDstDevChn() const
    {
        return dstDevChn;
    }
    TChannel& GetDstDevChn()
    {
        return dstDevChn;
    }
    void SetDstDevChn(const TChannel& cValue)
    {
        dstDevChn = cValue;
    }

    //目的key接口
	const TMediaChannelKey& GetDstMediaChnKey() const
	{
		return dstMediaChnKey;
	}
	TMediaChannelKey& GetDstMediaChnKey()
	{
		return dstMediaChnKey;
	}
	void SetDstMediaChnKey(const TMediaChannelKey& cValue)
	{
		dstMediaChnKey = cValue;
	}

    //目的地址接口
    const TNetAddrList& GetDstNetAddrList() const
    {
        return dstNetAddrList;
    }
    TNetAddrList& GetDstNetAddrList()
    {
        return dstNetAddrList;
    }
	void SetDstNetAddrList(const TNetAddrList& tValue)
	{
        dstNetAddrList = tValue;
	}

	int GetDstNetAddrNum() const
	{
		return dstNetAddrList.size();
	}

	const TNetAddr& GetDstNetAddr(s32 nIdx) const
	{
		return dstNetAddrList[nIdx];
	}
	TNetAddr& GetDstNetAddr(s32 nIdx)
	{
		return dstNetAddrList[nIdx];
	}

	void AddDstNetAddr(const TNetAddr& tValue)
	{
        dstNetAddrList.push_back(tValue);
	}

    //接收地址接口
	const TNetAddr& GetRcvNetAddr() const
	{
		return rcvNetAddr;
	}
	TNetAddr& GetRcvNetAddr()
	{
		return rcvNetAddr;
	}
	void SetRcvNetAddr(const TNetAddr& tValue)
	{
		rcvNetAddr = tValue;
	}

    //接收绑定接口
	const TNetAddr& GetRcvBindNetAddr() const
	{
		return rcvBindNetAddr;
	}
	TNetAddr& GetRcvBindNetAddr()
	{
		return rcvBindNetAddr;
	}
	void SetRcvBindNetAddr(const TNetAddr& tValue)
	{
		rcvBindNetAddr = tValue;
	}

    //发送绑定接口
	const TNetAddr& GetSndBindNetAddr() const
	{
		return sndBindNetAddr;
	}
	TNetAddr& GetSndBindNetAddr()
	{
		return sndBindNetAddr;
	}
	void SetSndBindNetAddr(const TNetAddr& tValue)
	{
		sndBindNetAddr = tValue;
	}

public:
    bool IsDsRuleEqualTo(const TSwitchChannel& obj) const
    {
        if ( SWITCH_TYPE_P2P == switchType)
        {
            if (this->rcvNetAddr == obj.rcvNetAddr
                && this->dstNetAddrList.front() == obj.dstNetAddrList.front())
            {
                return true;
            }
        }
        else    //SWITCH_TYPE_M2P
        {
            if (this->rcvNetAddr == obj.rcvNetAddr
                && this->dstNetAddrList.front() == obj.dstNetAddrList.front()
                && this->srcNetAddrList.front() == obj.srcNetAddrList.front())
            {
                return true;
            }
        }

        return false;
    }

public:
	const TExtInfo& GetExtInfo() const
	{
		return extInfo;
	}
	TExtInfo& GetExtInfo()
	{
		return extInfo;
	}
	void SetExtInfo(const TExtInfo& var)
	{
		extInfo = var;
	}

	void SetExtInfo(const string& strKey, bool bValue)
	{
		extInfo[strKey] = BoolStr(bValue);
	}
	bool GetExtInfo(const string& strKey, bool& bValue) const
	{
		TExtInfo::const_iterator itr = extInfo.find(strKey);
		if (itr != extInfo.end())
		{
			bValue = (itr->second == TRUE_STR);

			return true;
		}

		return false;
	}

	void SetDropRtpExtend(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);
	}
	bool GetDropRtpExtend() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_DROP_RTP_EXTEND, bVar);

		return bVar;
	}

	void SetSrcMrtcEnable(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_MRTC_ENABLE_SRC, bVar);
	}
	bool GetSrcMrtcEnable() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_MRTC_ENABLE_SRC, bVar);

		return bVar;
	}
	void SetDstMrtcEnable(bool bVar)
	{
		SetExtInfo(SDP_EXT_INFO_MRTC_ENABLE_DST, bVar);
	}
	bool GetDstMrtcEnable() const
	{
		bool bVar = false;

		GetExtInfo(SDP_EXT_INFO_MRTC_ENABLE_DST, bVar);

		return bVar;
	}

	void SetSrcAddrChangeFlag()
	{
		extInfo[SDP_EXT_INFO_ADDR_CHANGE_SIDE] = ADDR_CHANGE_SIDE_SRC;
	}
	void SetDstAddrChangeFlag()
	{
		extInfo[SDP_EXT_INFO_ADDR_CHANGE_SIDE] = ADDR_CHANGE_SIDE_DST;
	}
	bool IsSrcAddrChange() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_ADDR_CHANGE_SIDE);
		if (itr != extInfo.end())
		{
			return (itr->second == ADDR_CHANGE_SIDE_SRC);
		}

		return false;
	}
	bool IsDstAddrChange() const
	{
		TExtInfo::const_iterator itr = extInfo.find(SDP_EXT_INFO_ADDR_CHANGE_SIDE);
		if (itr != extInfo.end())
		{
			return (itr->second == ADDR_CHANGE_SIDE_DST);
		}

		return false;
	}

public:
    friend std::ostream& operator<<(std::ostream& output, const TSwitchChannel& obj)
    {
        output<<std::endl;
        output<<"switchSsnID: "<<obj.switchSsnID<<std::endl;
        output<<"mediaType: "<<obj.mediaType<<std::endl;
        output<<"mediaFormat: "<<obj.mediaFormat<<std::endl;
        output<<"mediaTransProto: "<<obj.mediaTransProto<<std::endl;
        output<<"transChnType: "<<obj.transChnType<<std::endl;
		output<<"switchType: "<<obj.switchType<<std::endl;
		output<<"isGapSwitch: "<<obj.isGapSwitch<<std::endl;
		output<<"isDomainSocket: "<<obj.isDomainSocket<<std::endl;
		output<<"isUseRecvProxy: "<<obj.isUseRecvProxy<<std::endl;

        output<<"srcDevChn: "<<obj.srcDevChn<<std::endl;
        output<<"srcMediaChnKey: "<<obj.srcMediaChnKey<<std::endl;
        output<<"srcNetAddrList: "<<obj.srcNetAddrList<<std::endl;

        output<<"dstDevChn: "<<obj.dstDevChn<<std::endl;
        output<<"dstMediaChnKey: "<<obj.dstMediaChnKey<<std::endl;
        output<<"dstNetAddrList: "<<obj.dstNetAddrList<<std::endl;

        output<<"rcvNetAddr: "<<obj.rcvNetAddr<<std::endl;
        output<<"rcvBindNetAddr: "<<obj.rcvBindNetAddr<<std::endl;
        output<<"sndBindNetAddr: "<<obj.sndBindNetAddr<<std::endl;

        output<<std::endl;
        return output;
    }

    void PrintData() const;

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetIsUseRecvProxy(bool bIsUseRecvProxy)
    {
        isUseRecvProxy = bIsUseRecvProxy;
    }
    bool GetIsUseRecvProxy() const
    {
        return isUseRecvProxy;
    }

};

typedef vector<TSwitchChannel> TSwitchChannelList;
/////////////////////////////////////////////////////////////////////
//}}码流交换相关
/////////////////////////////////////////////////////////////////////


//电视墙相关

struct TTvChannel
{
private:

    string          tvWallId;                       //电视墙ID
    int             tvId;                           //电视机ID
    int             tvDivId;                        //电视分画面ID

public:
    TTvChannel()
    {
        Clear();
    }

    ~TTvChannel()
    {
        Clear();
    }

    void Clear()
    {
        tvWallId.clear();
        tvId = 0;
        tvDivId = 0;
    }
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
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

struct TTvPollUnit
{
public:
    TTvPollUnit()
    {
    }

    void Clear()
    {
        pollIndex = 0;
        encChn.Clear();
        pollTime = 0;
    }

private:
    int           pollIndex;                   //轮询序号
    TChannel      encChn;                      //编码器通道
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

    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
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

typedef     vector<TTvPollUnit>     CTvPollList;


#define    TVWALL_SCHEME_OWNER_TYPE_PUBLIC         "public"
#define    TVWALL_SCHEME_OWNER_TYPE_PRIVATE        "private"


enum ETvWinStype
{
    ETV_WIN_STYLE_1 = 1,
    ETV_WIN_STYLE_2 = 2,
	ETV_WIN_STYLE_3 = 3,
    ETV_WIN_STYLE_4 = 4,
	ETV_WIN_STYLE_6 = 6,
	ETV_WIN_STYLE_8 = 8,
    ETV_WIN_STYLE_9 = 9,
    ETV_WIN_STYLE_16 = 16,
	ETV_WIN_STYLE_6_EQUAL = 106, //20170725 add by Royan Support6EqualStyle
};

//窗口分画面
struct TTvWinDiv
{
public:
    TTvWinDiv()
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
    CTvPollList      pollList;                               //轮询信息列表
public:
    void SetTvWinDivId(int nTvWinDivId)
    {
        tvWinDivId = nTvWinDivId;
    }
    int GetTvWinDivId() const
    {
        return tvWinDivId;
    }

    void SetPollList(const CTvPollList& cPollList)
    {
        pollList = cPollList;
    }
    CTvPollList& GetPollList()
    {
        return pollList;
    }
    const CTvPollList& GetPollList() const
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

typedef     vector<TTvWinDiv>      TTvWinDivList;

//窗口
struct TTvWin
{
public:
    TTvWin()
    {
        Clear();
    }

    void Clear()
    {
        tvWinId = 0;
        style = ETV_WIN_STYLE_1;
        divList.clear();
    }

private:
    int              tvWinId;                                //电视机id
    ETvWinStype      style;                                  //电视窗口风格，也就是解码器风格
    TTvWinDivList    divList;                                //各个分画面的信息
public:
    void SetTvWinId(int nTvWinId)
    {
        tvWinId = nTvWinId;
    }
    int GetTvWinId() const
    {
        return tvWinId;
    }

    void SetStyle(ETvWinStype eStyle)
    {
        style = eStyle;
    }
    ETvWinStype GetStyle() const
    {
        return style;
    }

    void SetDivList(const TTvWinDivList& cDivList)
    {
        divList = cDivList;
    }
    TTvWinDivList& GetDivList()
    {
        return divList;
    }
    const TTvWinDivList& GetDivList() const
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

typedef     vector<TTvWin>      CTvWinList;

struct TTvWallScheme
{
public:
    TTvWallScheme()
    {
    }

    void Clear()
    {
        schemeName.clear();
        //schemeId.clear();
        tvWallId.clear();
        //ownerType.clear();
        tvWinList.clear();
        //userId.clear();
    }

private:
    string      schemeName;                      //预案名称
    //string      schemeId;                        //预案ID, 新创建时不需要填，由tvs生成唯一ID后返回给用户
    string      tvWallId;                        //电视墙ID
    //string      ownerType;                       //预案拥有类型：public/private
    CTvWinList  tvWinList;                       //电视墙TV列表，也就编/解码器的对应关系
    //string      userId;                          //预案用户的ID
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

//     void SetSchemeId(const string& strSchemeId)
//     {
//         schemeId = strSchemeId;
//     }
//     string& GetSchemeId()
//     {
//         return schemeId;
//     }
//     const string& GetSchemeId() const
//     {
//         return schemeId;
//     }

    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

//     void SetOwnerType(const string& strOwnerType)
//     {
//         ownerType = strOwnerType;
//     }
//     string& GetOwnerType()
//     {
//         return ownerType;
//     }
//     const string& GetOwnerType() const
//     {
//         return ownerType;
//     }

    void SetTvWinList(const CTvWinList& cTvWinList)
    {
        tvWinList = cTvWinList;
    }
    CTvWinList& GetTvWinList()
    {
        return tvWinList;
    }
    const CTvWinList& GetTvWinList() const
    {
        return tvWinList;
    }

//     void SetUserId(const string& strUserId)
//     {
//         userId = strUserId;
//     }
//     string& GetUserId()
//     {
//         return userId;
//     }
//     const string& GetUserId() const
//     {
//         return userId;
//     }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef    vector<string>    CTTvWallSchemeList;

typedef     vector<string>     CTvWallSchemeNameList;

struct TTvWallSchemePollUnit
{
public:
    TTvWallSchemePollUnit()
    {
    }

    void Clear()
    {
        schemeName.clear();
        pollIndex = 0;
        pollTime = 0;
    }

    //string      schemeId;                        //预案ID, 新创建时不需要填，由tvs生成唯一ID后返回给用户
    int         pollIndex;                       //轮询序号
    string      schemeName;                      //预案名称
    int         pollTime;                        //轮询时间，默认为0，表示无穷大，一直播放不轮询
public:
//     void SetSchemeId(const string& strSchemeId)
//     {
//         schemeId = strSchemeId;
//     }
//     string& GetSchemeId()
//     {
//         return schemeId;
//     }
//     const string& GetSchemeId() const
//     {
//         return schemeId;
//     }

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

typedef    vector<TTvWallSchemePollUnit>    CTTvWallSchemePollList;

//预案组，一个电视墙只有唯一一个预案组
struct TTvWallSchemeGroup
{
public:
    TTvWallSchemeGroup()
    {
    }

    void Clear()
    {
        tvWallId.clear();
        pollList.clear();
    }

private:
    string      tvWallId;                               //电视墙ID
    CTTvWallSchemePollList pollList;                   //轮询列表
public:
    void SetTvWallId(const string& strTvWallId)
    {
        tvWallId = strTvWallId;
    }
    string& GetTvWallId()
    {
        return tvWallId;
    }
    const string& GetTvWallId() const
    {
        return tvWallId;
    }

    void SetPollList(const CTTvWallSchemePollList& cPollList)
    {
        pollList = cPollList;
    }
    CTTvWallSchemePollList& GetPollList()
    {
        return pollList;
    }
    const CTTvWallSchemePollList& GetPollList() const
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

//窗口分画面轮询状态定义
#define   WIN_POLL_START       "start"
#define   WIN_POLL_PAUSE       "pause"
#define   WIN_POLL_STOP        "stop"

//窗口分画面码流类型
#define   WIN_MEDIA_SERVICE_REAL_MEDIAL       "real_media"
#define   WIN_MEDIA_SERVICE_PLAT_REC          "plat_rec"
#define   WIN_MEDIA_SERVICE_PU_REC            "pu_rec"

//窗口分画面状态

struct TTvWinDivState
{
public:
    TTvWinDivState()
    {
        Clear();
    }

    void Clear()
    {
        tvDivId = 0;
        tvChnVidSrc.Clear();
        playState = 0;
        winPollState.clear();
        mediaService.clear();
        recToken.clear();
        timeRange.Clear();
        encChn.Clear();
    }

private:
    int              tvDivId;                    //电视分画面ID
    TChannel         tvChnVidSrc;                //电视播放的视频源通道（通常是PU, 放像时是放像session）
    int              playState;                  //当前视频源的播放状态：成功:CMS_SUCCESS，失败：错误码
    string           winPollState;               //轮询状态
    string           mediaService;               //媒体服务类型：WIN_MEDIA_SERVICE_REAL_MEDIAL等

    //当源是录像时，需要以下参数
    TRecToken        recToken;                   //录像Token
    TTimeRange       timeRange;                  //录像时间段
    TChannel         encChn;                     //录像编码器通道

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

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:

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

    void SetRecToken(const TRecToken& cRecToken)
    {
        recToken = cRecToken;
    }
    TRecToken& GetRecToken()
    {
        return recToken;
    }
    const TRecToken& GetRecToken() const
    {
        return recToken;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
    {
        return timeRange;
    }

    void SetEncChn(const TChannel& cEncChn)
    {
        encChn = cEncChn;
    }
    TChannel& GetEncChn()
    {
        return encChn;
    }
    const TChannel& GetEncChn() const
    {
        return encChn;
    }

public:
    void SetTvDivId(int nTvDivId)
    {
        tvDivId = nTvDivId;
    }
    int GetTvDivId() const
    {
        return tvDivId;
    }

    void SetTvChnVidSrc(const TChannel& cTvChnVidSrc)
    {
        tvChnVidSrc = cTvChnVidSrc;
    }
    TChannel& GetTvChnVidSrc()
    {
        return tvChnVidSrc;
    }
    const TChannel& GetTvChnVidSrc() const
    {
        return tvChnVidSrc;
    }

};

typedef     vector<TTvWinDivState>      TTvWinDivStateList;

//窗口状态
struct TTvWinState
{
public:
    TTvWinState()
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
    TTvWinDivStateList    divStateList;                      //各个分画面的状态
public:

    void SetDivStateList(const TTvWinDivStateList& cDivStateList)
    {
        divStateList = cDivStateList;
    }
    TTvWinDivStateList& GetDivStateList()
    {
        return divStateList;
    }
    const TTvWinDivStateList& GetDivStateList() const
    {
        return divStateList;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetTvId(int nTvId)
    {
        tvId = nTvId;
    }
    int GetTvId() const
    {
        return tvId;
    }

};

typedef     vector<TTvWinState>      CTvWinStateList;


//预案轮询状态定义
#define   SCHEME_POLL_START       "start"
#define   SCHEME_POLL_PAUSE       "pause"
#define   SCHEME_POLL_STOP        "stop"

struct TTvWallState
{
public:
    TTvWallState()
    {
        Clear();
    }

    void Clear()
    {
        schemeGroup.Clear();
        schemePollState.clear();
        curScheme.Clear();
        winStateList.clear();
    }

private:
    TTvWallSchemeGroup  schemeGroup;                //预案组
    string   schemePollState;                       //预案轮询状态: start/pause/stop
    TTvWallScheme   curScheme;                      //当前预案
    CTvWinStateList winStateList;                   //当前预案每个窗口的状态  

    //结合编码器对应的编码器，cu可完整复现tvwall的当前状态
public:
    void SetSchemeGroup(const TTvWallSchemeGroup& cSchemeGroup)
    {
        schemeGroup = cSchemeGroup;
    }
    TTvWallSchemeGroup& GetSchemeGroup()
    {
        return schemeGroup;
    }
    const TTvWallSchemeGroup& GetSchemeGroup() const
    {
        return schemeGroup;
    }

    void SetSchemePollState(const string& strSchemePollState)
    {
        schemePollState = strSchemePollState;
    }
    string& GetSchemePollState()
    {
        return schemePollState;
    }
    const string& GetSchemePollState() const
    {
        return schemePollState;
    }

    void SetCurScheme(const TTvWallScheme& cCurScheme)
    {
        curScheme = cCurScheme;
    }
    TTvWallScheme& GetCurScheme()
    {
        return curScheme;
    }
    const TTvWallScheme& GetCurScheme() const
    {
        return curScheme;
    }

    void SetWinStateList(const CTvWinStateList& cWinStateList)
    {
        winStateList = cWinStateList;
    }
    CTvWinStateList& GetWinStateList()
    {
        return winStateList;
    }
    const CTvWinStateList& GetWinStateList() const
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


//移动侦测参数结构
struct TMotionDetectParam
{
    TMotionDetectParam()
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

typedef   vector<TMotionDetectParam>     TMotionDetectList;

//图像遮蔽参数结构
struct TAreaShadeParam
{
    TAreaShadeParam()
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

typedef   vector<TAreaShadeParam>     TAreaShadeList;

//GPS订阅相关，已经无法完全采用透明数据的方式实现

/* GPS订阅功能实现思路
   1. cu感知的操作：a. gps配置的设置和获取；b. 订阅/取消订阅gps数据。
   2. cu不再需要配置前端gps的使能状态。
   3. cu订阅gps时，cmu首先向pu进行gps使能设置(透明数据对平台不透明了)，然后给该cu增加gps订阅类型。
   4. gps数据到达cmu时，cmu根据订阅过的用户列表发布gps数据。gps数据不在cmu保存，这点和告警不一样。
   5. cu取消gps订阅时，cmu首先判断该设备是否还存在用户订阅gps数据，如果没有就取消pu的gps使能设置(透明数据对平台不透明了)。
*/

struct TGpsCfgParam
{
public:
    TGpsCfgParam()
    {
        Clear();
    }
    void Clear()
    {
        type = 0;
        param.clear();
    }

private:
    int        type;                                    //透明数据类型
    string     param;                                   //gps配置参数
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

struct TGpsSsParam
{
public:
    TGpsSsParam()
    {
        Clear();
    }
    void Clear()
    {
        enable = false;
        type = 0;
        subType = 0;
        param.clear();
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

struct TGpsSsInquiry
{
public:
    TGpsSsInquiry()
    {
        Clear();
    }
    void Clear()
    {
        type = 0;
        subType = 0;
        param.clear();
    }

private:
    int        type;                                   //透明数据类型
    int        subType;                                //透明数据子类型
    string     param;                                  //gps查询订阅参数
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

struct TGpsData
{
public:
    TGpsData()
    {
        Clear();
    }
    void Clear()
    {
        type = 0;
        subType = 0;
        data.clear();
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

//透明数据
typedef  string TPuTransDataType;
#define PU_TRANS_DATA_TYPE_COMDATA       "ComData"       //串口数据
struct TTransparentData
{
public:
    TTransparentData()
    {
        Clear();
    }
    void Clear()
    {
        type.clear();
        data.clear();
    }

private:
    TPuTransDataType  type;                   //透明数据类型
    string            data;                   //base64格式的透明数据
public:
    void SetType(const TPuTransDataType& strType)
    {
        type = strType;
    }
    TPuTransDataType& GetType()
    {
        return type;
    }
    const TPuTransDataType& GetType() const
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

struct TTransDataCfgParam
{
public:
    TTransDataCfgParam()
    {
        Clear();
    }
    void Clear()
    {
        type = 0;
        param.clear();
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

//前端录像相关
struct TPuRecToken
{
public:
    TPuRecToken()
    {
        Clear();
    }
    void Clear()
    {
        recType.clear();
        fileId.clear();
        filePath.clear();
        fileName.clear();
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
		else if (cFieldList.size() >= 2)
		{
			recType = cFieldList[0];
			fileId = cFieldList[1];
		}
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

struct TPuRecInfo
{
public:
    TPuRecInfo()
    {
        Clear();
    }
    void Clear()
    {
        recToken.Clear();
        timeRange.Clear();
    }

private:
    TPuRecToken           recToken;                    //录像Token
    TTimeRange            timeRange;                   //时间段
public:
    void SetRecToken(const TPuRecToken& cRecToken)
    {
        recToken = cRecToken;
    }
    TPuRecToken& GetRecToken()
    {
        return recToken;
    }
    const TPuRecToken& GetRecToken() const
    {
        return recToken;
    }

    void SetTimeRange(const TTimeRange& cTimeRange)
    {
        timeRange = cTimeRange;
    }
    TTimeRange& GetTimeRange()
    {
        return timeRange;
    }
    const TTimeRange& GetTimeRange() const
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

typedef   vector<TPuRecInfo>    CPuRecInfoList;

typedef string TVcrCmd;
#define VCRCMD_START    "START"         //开始放像
#define VCRCMD_STOP     "STOP"          //停止放像
#define VCRCMD_PAUSE    "PAUSE"         //暂停放像
#define VCRCMD_RESUME   "RESUME"        //恢复放像
#define VCRCMD_SEEK     "SEEK"          //时间定位
#define VCRCMD_FASTPLAY "FASTPLAY"      //快放
#define VCRCMD_SLOWPLAY "SLOWPLAY"      //慢放
#define VCRCMD_ONEFRAME "ONEFRAME"      //单帧播放
#define VCRCMD_KEYFRAME "KEYFRAME"      //只放关键帧
#define VCRCMD_KEYSEEK "KEYSEEK"       //时间定位(从前一个关键帧开始)
#define VCRCMD_REVERSEPLAY "REVERSEPLAY"   //倒放
#define VCRCMD_FORWARDPLAY "FORWARDPLAY"   //正放
#define VCRCMD_AUTO_KEYFRAME "AUTOKEYFRAME"			//自动模式，是否抽帧由NRU确定：低倍速全帧，高倍速只放关键帧
#define VCRCMD_FORCE_KEYFRAME "FORCEKEYFRAME"		//强制只放关键帧
#define VCRCMD_FORCE_ALLFRAME "FORCEALLFRAME"		//强制回放全帧
#define VCRCMD_UNKNOWN  "UNKNOWN"

struct TBase64DataPack
{
public:
    TBase64DataPack()
    {
        Clear();
    }

    void Clear()
    {
        totalPack = 0;
        curPackIndex = 0;
        data.clear();
    }

private:
    int                 totalPack;              		    //总包数
    int                 curPackIndex;              	        //当前包的索引号,从1开始
    string              data;                               //base64格式的数据包

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
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

};

/*
//录象文件/事件类型/图片事件
#define MTI_TYPE_H264					   0x10 //h264文件类型
#define MTI_QUIEY_ALL_EVENT                0    //查询所有事件
#define MTI_TYPE_PIN_ALARM                 1    //并口告警录象
#define MTI_TYPE_MOTION_DETECT_ALARM       2    //移动侦测告警录象
#define MTI_TYPE_MANUAL                    3    //手动录象
#define MTI_TYPE_TIMELY                    4    //定时录象
#define MTI_TYPE_PERIODLY                  5    //周期录象
#define MTI_QUIEY_FILE                     6    //按文件查询
*/

//图片查询方式
#define PU_PIC_QUERY_BY_CHN                  "by_chn"        //按通道号查询
#define PU_PIC_QUERY_BY_TIME                 "by_time"       //按时间查询
#define PU_PIC_QUERY_BY_EVENT                "by_event"      //按事件查询

//图片事件类型
#define PU_PIC_EVENT_ALL                     "all"           //查询所有事件
#define PU_PIC_EVENT_PIN_ALARM               "pin_alarm"     //并口告警
#define PU_PIC_EVENT_MOTDET_ALARM            "motion_detect" //移动侦测告警
#define PU_PIC_EVENT_MANUAL                  "manual"        //手动


struct TPuPicInfo
{
public:
    TPuPicInfo()
    {
        Clear();
    }
    void Clear()
    {
        eventType.clear();
        resolution.clear();
        fileFormat = 0;
        picID.clear();
        snapTime.clear();
        fileName.clear();
        devChn.Clear();
    }

private:
    string             eventType;                               //图片事件类型
    string             resolution;                              //图像分辨率
    int                fileFormat;                              //文件格式
    string             picID;                                   //图片文件ID
    string             snapTime;                                //抓拍时间
    string             fileName;                                //文件别名
    TChannel           devChn;                                  //前端通道
public:
    void SetEventType(const string& strEventType)
    {
        eventType = strEventType;
    }
    string& GetEventType()
    {
        return eventType;
    }
    const string& GetEventType() const
    {
        return eventType;
    }

    void SetResolution(const string& strResolution)
    {
        resolution = strResolution;
    }
    string& GetResolution()
    {
        return resolution;
    }
    const string& GetResolution() const
    {
        return resolution;
    }

    void SetPicID(const string& strPicID)
    {
        picID = strPicID;
    }
    string& GetPicID()
    {
        return picID;
    }
    const string& GetPicID() const
    {
        return picID;
    }

    void SetSnapTime(const string& strSnapTime)
    {
        snapTime = strSnapTime;
    }
    string& GetSnapTime()
    {
        return snapTime;
    }
    const string& GetSnapTime() const
    {
        return snapTime;
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

    void SetDevChn(const TChannel& cDevChn)
    {
        devChn = cDevChn;
    }
    TChannel& GetDevChn()
    {
        return devChn;
    }
    const TChannel& GetDevChn() const
    {
        return devChn;
    }

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetFileFormat(int nFileFormat)
    {
        fileFormat = nFileFormat;
    }
    int GetFileFormat() const
    {
        return fileFormat;
    }

};

typedef   vector<TPuPicInfo>    CPuPicInfoList;

struct TNatInfo
{
public:
    TNatInfo()
    {
        isHasNat = DEVICE_LINEMODE_INTRANET;
        ethNo = 0;
    }
private:
    int isHasNat;       //存在nat关系              
    int ethNo;          //网卡号                   
    TIpAddr ipAddr;     //上述网卡对应的ip               
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
public:
    void SetIsHasNat(int nIsHasNat)
    {
        isHasNat = nIsHasNat;
    }
    int GetIsHasNat() const
    {
        return isHasNat;
    }
    void SetEthNo(int nEthNo)
    {
        ethNo = nEthNo;
    }
    int GetEthNo() const
    {
        return ethNo;
    }
    void SetIpAddr(const TIpAddr& strIpAddr)
    {
        ipAddr = strIpAddr;
    }
    TIpAddr& GetIpAddr()
    {
        return ipAddr;
    }
    const TIpAddr& GetIpAddr() const
    {
        return ipAddr;
    }
};

//前端长字幕参数
enum EOsdState
{
    OSD_DISABLE = 0, 
    OSD_TITLE, 
    OSD_LOGOBMP
};

struct TLongOsdParam
{  
public:
    TLongOsdParam()
    {
        Clear();
    }

    void Clear()
    {
        state = OSD_DISABLE;
        leftMargin = 0;
        topMargin = 0;
        color = 0;
        text.clear();
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

//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
//todo 参数细化
struct TOsdParam
{  
public:
	TOsdParam()
	{
		Clear();
	}

	void Clear()
	{
		state = 0;
		leftMargin = 0;
		topMargin = 0;
		color = 0;
		text.clear();
	}

private:  
	int state;                         //编码字幕状态 DISABLE 0-无内容; TITLE 1-有字符字模; LOGOBMP 2-有图片台标
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

typedef map<int, TOsdParam> TOsdParamList;

struct TMultiOsdParam
{  
public:
	TMultiOsdParam()
	{
		Clear();
	}

	void Clear()
	{
		rowNum = 0;
		osdParamList.clear();
	}

private:  
	int rowNum;                        //字幕行数
	TOsdParamList osdParamList;        //字幕列表

public:
	void SetRowNum(int nRowNum)
	{
		rowNum = nRowNum;
	}
	int GetRowNum() const
	{
		return rowNum;
	}

	void SetOsdParamList(const TOsdParamList& tList)
	{
		osdParamList = tList;
	}
	TOsdParamList& GetOsdParamList()
	{
		return osdParamList;
	}
	const TOsdParamList& GetOsdParamList() const
	{
		return osdParamList;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕

//前端台标传输参数
struct TLogoBmpOsdParam
{
public:
    TLogoBmpOsdParam()
    {
        Clear();
    }

    void Clear()
    {
        width = 0;
        heigth = 0;
        totalPack = 0;
        curPackIndex = 0;
        bmpOsdBuf.clear();
        fontNum = 0;
        fontHeight = 0;
        fontWidth.clear();
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


struct TPuSnapShotCfg
{
public:
	TPuSnapShotCfg()
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
	//并口告警抓图使能 0关闭/1开启
	bool pinAlarmEnable;

	//移动侦测抓图使能 0关闭/1开启
	bool mtionDetectEnable;

	//抓拍时间间隔(ms)
	int	interval;	

	//最大抓拍数量
	int maxSnapNum;		

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);

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

};


struct TPuSerialPortParam
{	
public:
	TPuSerialPortParam()
	{
		serialPortID = 0;  
		baudRate = 0;	  //串口波特率
		comPortType = 0;  //串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
		rS232Enable = false;
		rS422Enable = false;
		rS485Enable = false;	
	}
private:
	//串口号
	int serialPortID; 

	//串口波特率
	int baudRate;

	//串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
	int comPortType; 

	bool rS232Enable;
	bool rS422Enable;
	bool rS485Enable;


public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
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

};

//{{{{{{{{{{{{{{{ add by Royan 20140526 前端录像状态通知
struct TPuChnRecStatInfo
{
private:
	bool isManualRecording;		    // 是否在手动录像
	bool isMotiveAlarmRecording;	// 是否在移动侦测告警录像
	bool isPinAlarmRecording;	    // 是否在并口告警录像
	bool isTimerRecording;	        // 是否在定时录像
	bool isWeekRecording;		    // 是否在周期录像
	bool isPreRecording;			// 是否在预录

public:
	TPuChnRecStatInfo() 
	{
		Clear();
	}

	void Clear()
	{
		isManualRecording = false;
		isMotiveAlarmRecording = false;
		isPinAlarmRecording = false;
		isTimerRecording = false;
		isWeekRecording = false;
		isPreRecording = false;
	}   

public:
	void SetManualRecStat(bool recStat)
	{
		isManualRecording = recStat;
	}
	bool GetManualRecStat() const
	{
		return isManualRecording;
	}

	void SetMotiveAlarmRecStat(bool recStat)
	{
		isMotiveAlarmRecording = recStat;
	}
	bool GetMotiveAlarmRecStat() const
	{
		return isMotiveAlarmRecording;
	}

	void SetPinAlarmRecStat(bool recStat)
	{
		isPinAlarmRecording = recStat;
	}
	bool GetPinAlarmRecStat() const
	{
		return isPinAlarmRecording;
	}

	void SetTimerRecStat(bool recStat)
	{
		isTimerRecording = recStat;
	}
	bool GetTimerRecStat() const
	{
		return isTimerRecording;
	}

	void SetWeekRecStat(bool recStat)
	{
		isWeekRecording = recStat;
	}
	bool GetWeekRecStat() const
	{
		return isWeekRecording;
	}

	void SetPreRecStat(bool recStat)
	{
		isPreRecording = recStat;
	}
	bool GetPreRecStat() const
	{
		return isPreRecording;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//}}}}}}}}}}}}}}} add by Royan 20140526 前端录像状态通知

struct TProbeOnvifPuInfo
{
private:
	string         devIp;             //前端设备ip
	string         devUUID;           //前端设备UUID
	string         devSessionId;      //设备session（即入网UUID）

	string         devAuthUserName;    //设备鉴权用户名
	string         devAuthPassWord;   //设备鉴权密码

public:
	TProbeOnvifPuInfo()
	{
		Clear();
	}
	void Clear()
	{
		devIp = "";
		devUUID = "";
		devSessionId = "";
		devAuthUserName = "";
		devAuthPassWord = "";
	}

public:
	const string& GetDevIp() const
	{
		return devIp;
	}
	string& GetDevIp()
	{
		return devIp;
	}
	void SetDevIp(const string& strDevIp)
	{
		devIp = strDevIp;
	}

	const string& GetDevUUID() const
	{
		return devUUID;
	}
	string& GetDevUUID()
	{
		return devUUID;
	}
	void SetDevUUID(const string& strUUID)
	{
		devUUID = strUUID;
	}

	const string& GetDevSessionId() const
	{
		return devSessionId;
	}
	string& GetDevSessionId()
	{
		return devSessionId;
	}
	void SetDevSessionId(const string& strDevSessionId)
	{
		devSessionId = strDevSessionId;
	}

	const string& GetDevAuthUserName() const
	{
		return devAuthUserName;
	}
	string& GetDevAuthUserName()
	{
		return devAuthUserName;
	}
	void SetDevAuthUserName(const string& strDevAuthUserName)
	{
		devAuthUserName = strDevAuthUserName;
	}

	const string& GetDevAuthPassWord() const
	{
		return devAuthPassWord;
	}
	string& GetDevAuthPassWord()
	{
		return devAuthPassWord;
	}
	void SetDevAuthPassWord(const string& strDevAuthPassWord)
	{
		devAuthPassWord = strDevAuthPassWord;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef std::vector<TProbeOnvifPuInfo> TProbeOnvifPuList;

struct TDeviceVideoSrcId
{
public:
	TDeviceVideoSrcId()
	{
		Clear();
	}
	void Clear()
	{
		deviceId = "";
		videoSrcId=-1;
		emChnStat = CHNSTATUS_IDEL;
	}
private:
	string deviceId;               	// 设备ID.
	int videoSrcId;               	// 视频源ID.
	bool deviceOnline;              // 视频源所在的设备是否在线
	emChnStatus emChnStat;          // 视频源对应的通道状态
public:
    void SetDeviceId(const string& strDeviceId)
    {
        deviceId = strDeviceId;
    }
    string& GetDeviceId()
    {
        return deviceId;
    }
    const string& GetDeviceId() const
    {
        return deviceId;
    }

    void SetVideoSrcId(int nVideoSrcId)
    {
        videoSrcId = nVideoSrcId;
    }
    int GetVideoSrcId() const
    {
        return videoSrcId;
    }

	void SetDeviceOnline(bool bDeviceOnline)
	{
		deviceOnline = bDeviceOnline;
	}
	bool GetDeviceOnline() const
	{
		return deviceOnline;
	}

	void SetChnStat(emChnStatus enChnStat)
	{
		emChnStat = enChnStat;
	}
	emChnStatus GetChnStat() const
	{
		return emChnStat;
	}
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};
typedef std::vector<TDeviceVideoSrcId> TDeviceVideoSrcIdList;

typedef string TDomainOperateType;
#define DOMAIN_OP_TYPE_ADD "ADD"
#define DOMAIN_OP_TYPE_DEL "DEL"
struct TDomainItem
{
public:
	TDomainItem()
	{
		Clear();
	}
	void Clear()
	{
		operateType = DOMAIN_OP_TYPE_ADD;
	}

private:
	string peerDomainName;          // 对端域名
	string peerIP;                  // 与本域通信的对端域地址
	string localIP;                 // 与对端域通信的本域地址
	TDomainOperateType operateType; // 操作类型，ADD/DEL

public:
	void SetPeerDomainName(const string& var)
	{
		peerDomainName = var;
	}
	string& GetPeerDomainName()
	{
		return peerDomainName;
	}
	const string& GetPeerDomainName() const
	{
		return peerDomainName;
	}

	void SetPeerIP(const string& var)
	{
		peerIP = var;
	}
	string& GetPeerIP()
	{
		return peerIP;
	}
	const string& GetPeerIP() const
	{
		return peerIP;
	}

	void SetLocalIP(const string& var)
	{
		localIP = var;
	}
	string& GetLocalIP()
	{
		return localIP;
	}
	const string& GetLocalIP() const
	{
		return localIP;
	}

	void SetOperateType(const TDomainOperateType& var)
	{
		operateType = var;
	}
	TDomainOperateType& GetOperateType()
	{
		return operateType;
	}
	const TDomainOperateType& GetOperateType() const
	{
		return operateType;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
typedef map<string, TDomainItem> TDomainTable;
#define DOMAIN_NAME_OF_PARENT "parent@domain"

typedef string TMappingOperateType;
#define MAPPING_OP_TYPE_ADD "ADD"
#define MAPPING_OP_TYPE_DEL "DEL"
#define MAPPING_OP_TYPE_MOD "MOD"
struct TMappingItem
{
public:
	TMappingItem()
	{
		Clear();
	}
	void Clear()
	{
		localPort   = 0;
		mappingPort = 0;  
		protocol    = NAT_PROTOCOL_TCP;
		operateType = MAPPING_OP_TYPE_ADD;
	}

private:
	string localIP;        // 自身地址
	int    localPort;      // 自身端口
	string mappingIP;      // 映射后地址
	int    mappingPort;    // 映射后端口
	TNatProtocolType protocol;	     // 协议类型，TCP/UDP
	TMappingOperateType operateType; // 操作类型，ADD/DEL/MOD

public:
	void SetLocalIP(const string& var)
	{
		localIP = var;
	}
	string& GetLocalIP()
	{
		return localIP;
	}
	const string& GetLocalIP() const
	{
		return localIP;
	}

	void SetLocalPort(const int& var)
	{
		localPort = var;
	}
	int& GetLocalPort()
	{
		return localPort;
	}
	const int& GetLocalPort() const
	{
		return localPort;
	}

	void SetMappingIP(const string& var)
	{
		mappingIP = var;
	}
	string& GetMappingIP()
	{
		return mappingIP;
	}
	const string& GetMappingIP() const
	{
		return mappingIP;
	}

	void SetMappingPort(const int& var)
	{
		mappingPort = var;
	}
	int& GetMappingPort()
	{
		return mappingPort;
	}
	const int& GetMappingPort() const
	{
		return mappingPort;
	}

	void SetProtocol(const TNatProtocolType& var)
	{
		protocol = var;
	}
	TNatProtocolType& GetProtocol()
	{
		return protocol;
	}
	const TNatProtocolType& GetProtocol() const
	{
		return protocol;
	}

	void SetOperateType(const TMappingOperateType& var)
	{
		operateType = var;
	}
	TMappingOperateType& GetOperateType()
	{
		return operateType;
	}
	const TMappingOperateType& GetOperateType() const
	{
		return operateType;
	}

public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
//string格式为"UDP_1.1.1.1_40000"，协议类型_本地IP_本地端口。
typedef map<string, TMappingItem> TMappingTable;

struct TGeneralData
{
public:
	TGeneralData()
	{
		type.clear();
		content.clear();
	}

	string           type;
	CTransparentData content;
public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//30w设备接入量优化结构体
enum Enum30WPuChnStat
{
	STAT_PU_CHN_IDLE,
	STAT_PU_CHN_ONLINE,
	STAT_PU_CHN_OFFLINE
};
//key(int)为通道id，value(int)为Enum30WPuChnStat类型
typedef map<int, int> TMap30WPuChnStat;
struct T30WPuReg
{
public:
	T30WPuReg()
	{
		Clear();
	}

	void Clear()
	{
		devUri.clear();
		devIp.clear();
		devName.clear();
		devMode.clear();
		manufacturer.clear();
		vidEncNum = 0;
		inputPinNum = 0;
		videoInportNum = 0;
		hasGpsCap = false;
		hasFreeLicence = false;
		gbDomainId.clear();
		chnStat.clear();
	}

public:
	string devUri;
	string devIp;
	string devName;
	string devMode;
	string manufacturer;
	int    vidEncNum;
	int    inputPinNum;
	int    videoInportNum;
	bool   hasGpsCap;
	bool   hasFreeLicence;
	string gbDomainId;
	TMap30WPuChnStat chnStat;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
struct T30WPuUnreg
{
public:
	T30WPuUnreg()
	{
		Clear();
	}

	void Clear()
	{
		devUri.clear();
	}

public:
	string devUri;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
struct T30WPuChnStat
{
public:
	T30WPuChnStat()
	{
		Clear();
	}

	void Clear()
	{
		devUri.clear();
		chnStat.clear();
	}

public:
	string devUri;
	TMap30WPuChnStat chnStat;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};
struct T30WDomainOffline
{
public:
	T30WDomainOffline()
	{
		Clear();
	}

	void Clear()
	{
		gbDomainId.clear();
	}

public:
	string gbDomainId;
public:
	const std::string ToXml()const;
	void FromXml(const std::string& xml_string);
	void ToXml(TiXmlDocument& xmlDoc)const;
	void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
	void FromXml(const TiXmlNode& xmlNode);
};

#endif  //#ifndef __CMS_STRUCT_H__




