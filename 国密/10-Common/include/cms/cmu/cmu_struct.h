#ifndef __CMU_STRUCT_H__
#define __CMU_STRUCT_H__

#include "cms/cms_struct.h"
#include <algorithm>
#include "cms/pu/vsipdefine.h"
#include "cms/security_module/security_module_proto.h"



//����ͷ����
struct TCamCtrl
{
private:
    int commandId; //��������;

    //��������Ĳ�����ͬ,������������(��newcamera.h)
//     int param1; //����1
//     int param2; //����2
//     int param3; //����3
//     int param4; //����4
    vector<int> params;

    int priLevel; //�����ƾ�����
    int holdTime; //����ռ��ʱ��(��λ:��);
    int matrixIn; //����ǰ�þ��󣬸ò���Ϊ�������ǰ�˵�����˿ں�;

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

    //��ȡ��������
    int GetCommandId() const 
    {
        return commandId;
    }
    int GetCommandId()
    {
        return commandId;
    }
    //������������
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

    //��ȡ�����ƾ�����
    int GetPriLevel() const 
    {
        return priLevel;
    }
    int GetPriLevel()
    {
        return priLevel;
    }
    //���ÿ����ƾ�����
    void SetPriLevel(const int dwPriLevel) 
    {
        priLevel = dwPriLevel;
    }

    //��ȡ����ռ��ʱ��(��λ:��)
    int GetHoldTime() const 
    {
        return holdTime;
    }
    //���ÿ���ռ��ʱ��(��λ:��)
    void SetHoldTime(int dwHoldTime) 
    {
        holdTime = dwHoldTime;
    }

    //��ȡ����ǰ�þ��󣬸ò���Ϊ�������ǰ�˵�����˿ں�
    int GetMatrixIn() const 
    {
        return matrixIn;
    }
    //���ö���ǰ�þ��󣬸ò���Ϊ�������ǰ�˵�����˿ں�
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
    bool isAudEnc;       //�Ƿ���Ƶ����;
    int  directTransNum; //ֱ��·��;

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

    //��ȡ�Ƿ���Ƶ����
    bool GetIsAudEnc() const 
    {
        return isAudEnc;
    }
    //�����Ƿ���Ƶ����
    void SetIsAudEnc(const bool bIsAudEnc) 
    {
        isAudEnc = bIsAudEnc;
    }

    //��ȡֱ��·��
    int GetDirectTransNum() const 
    {
        return directTransNum;
    }
    //����ֱ��·��
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

//ý��payload����
/*��Ƶ*/
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

/*��Ƶ*/
#define   MEDIA_PAYLOAD_MP4	                  (int)97  /*MPEG-4*/
#define   MEDIA_PAYLOAD_H261	              (int)31  /*H.261*/
#define   MEDIA_PAYLOAD_H262	              (int)33  /*H.262 (MPEG-2)*/
#define   MEDIA_PAYLOAD_H263	              (int)34  /*H.263*/
#define   MEDIA_PAYLOAD_H263PLUS              (int)101 /*H.263+*/
#define   MEDIA_PAYLOAD_H264	              (int)106 /*H.264*/
#define   MEDIA_PAYLOAD_MJPEG	              (int)107 /*MJPEG*/
#define   MEDIA_PAYLOAD_SVAC	              (int)107 /*SVAC*/ 
#define   MEDIA_PAYLOAD_H265	              (int)111 /*H.265*/

//��Ƶ��ʽ
#define VID_FMT_SN4         "SN4"
#define VID_FMT_MPEG4       "MPEG4"
#define VID_FMT_H261        "H261"
#define VID_FMT_H263        "H263"
#define VID_FMT_H264        "H264"
#define VID_FMT_JPEG        "JPEG"
#define VID_FMT_MJPEG       "MJPEG"
#define VID_FMT_H265        "H265"
#define VID_FMT_SVAC        "SVAC"

//��Ƶ�ֱ���
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

//��Ƶ��ʽ
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


//ͼ��ֱ���
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


typedef string CVidFormat;  //��Ƶ��ʽ����
typedef string CAudFormat;  //��Ƶ��ʽ����
typedef string CVidRes;     //��Ƶ�ֱ�������
typedef set<CVidRes> CVidResList;
typedef u8 CVidType;

struct TVidType
{
private:
    CVidFormat    format;     //��Ƶ��ʽ
    CVidResList   resSet;        //�ø�ʽ֧�ֵķֱ���

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
    CVidTypeList video;  //����֧�ֵ���Ƶ��ʽ
    CAudTypeList audio;  //����֧�ֵ���Ƶ��ʽ

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


#define VID_QUALITY_PRIOR             0x01 //��������
#define VID_SPEED_PRIOR               0x02 //�ٶ�����


//{{{{{{{{{{{{{{{ add by Royan 20140328 �ֱ�����ֵ��
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
//	//1.�Ϸ���������ö��ֵ�ȴ�С
//	//2.�Ϸ�����С���·���
//	//3.�·����ȿ������ʱ�ȸ�
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
//	//������ǰ�����������ֵ�� VIDEO_RESOLUTION_NONE �����Ը�ֵΪ׼��
//	u8 resolutionType;		//�ֱ������ͣ�VIDEO_RESOLUTION_**
//	u16 width;				//��
//	u16 height;				//��
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
//}}}}}}}}}}}}}}} add by Royan 20140328 �ֱ�����ֵ��

//��Ƶ�������
struct TVideoEncodeParam
{
private:	
    CVidFormat videoType;      //����ѹ����ʽ; <��pu\MtCtrl.h>
    CVidRes videoRes;          //ͼ��ֱ���; <��pu\MtCtrl.h>
    int frameRate;             //֡�� 1-25;
    int quality;               //ͼ������; <��pu\MtCtrl.h>
    int bitRate;               //��������� 64K-4M;
	int	maxQuantization;	   //����������� 1-31 //�߼��������Ex
	int	minQuantization;	   //��С�������� 1-31 //�߼��������Ex
    int interval;              //�ؼ�֡��� 25-2500;
    int brightness;            //���� -128��127
    int contrast;              //�Աȶ� -128��127
    int saturation;            //���Ͷ� -128��127
	int	dnyValue;			   //��̬�غ�ֵ //�߼��������Ex
    int sharpness;             //��� 0: not support; 1-4: available
	int	profileId;			   //ͼ�����Э��ȼ� //1.0���У�����û���õ�������
	int clockRate;             //��Ƶ������
public: 
    TVideoEncodeParam() 
    {
        Clear();
    }

    void Clear()
    {
        videoType.clear();
        videoRes.clear();
        frameRate = 0;    //֡�� 1-25
        quality = 0;      //ͼ������ <��pu\MtCtrl.h>
        bitRate = 0;      //��������� 64K-4M
        interval = 0;     //�ؼ�֡��� 25-2500
        brightness = 0;      //���� -128��127
        contrast = 0;      //�Աȶ� -128��127
        saturation = 0;      //���Ͷ� -128��127
        sharpness = 0;     //��� 0: not support; 1-4: available
		maxQuantization = 0;	   //����������� 1-31 //�߼��������Ex
		minQuantization = 0;	   //��С�������� 1-31 //�߼��������Ex
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

//��Ƶ�������
struct TAudioEncodeParam
{
private:
    CAudFormat audioType;                   //��Ƶ��ʽ
    int volume;               			    //���� 0-25;
    bool aecState;               		    //��������״̬
    int smpRate;                            //������ 
    int smpBits;                            //����λ�� 
    int channel;                            //���� 
    int samplePerF;                         //ÿ֡������
	int clockRate;                          //ʱ���������
	int bitRate;                            //����
	int payLoad;                            //��Ƶpayload

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
    int videoEncNum;               //��Ƶ����ͨ������;
    int videoDecNum;               //��Ƶ����ͨ������;
    int audioEncNum;               //��Ƶ����ͨ������;
    int audioDecNum;               //��Ƶ����ͨ������;
    int serialPortNum;             //��������;
    int inputPinNum;               //������������;
    int outputPinNum;              //�����������;
    int videoInPortNum;            //��Ƶ����˿�����;
	int usingVideoInPortNum;       //ʹ���е���Ƶ����˿���������ȫͨ��ʱ�͵���videoInPortNum��ȫͨ��ʱ����������IDLE��������

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
    int id;         // ����������ID
    int value;      // ����������ֵ

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

#define PU_ENCODER        "ENCODER"   //������
#define PU_DECODER        "DECODER"   //������
#define PU_CODEC          "CODEC"     //�������
#define PU_UNKNOWN        "UNKNOWN"   //δ֪ǰ���豸

typedef set<int> CVidSrcSet;
typedef set<int> CPuChnSet;
typedef string CInputPinEnable;

//�豸��������
struct TPuBaseConfig 
{
private:
    string            devUri;                         //ǰ���豸URI��puid@domain
    TIpAddr           devIp;                          //ǰ���豸IP
    string            devType;                        //�豸���ͣ��������������
    string            devMode;                        //�豸�ͺţ�CU��ʹ�����ж�һЩ��ǰ�˵�������ƽ̨����ʹ��
    string            devName;                        //�豸����
    string            manufacturer;                   //�豸����

    TPuBaseCapSet       baseCapSet;                   //����������
    map<int,int>        extCapSet;                    //��չ������
	map<int,int>        hdmiVidDecCfg;                //hdmi��Ƶ��������<(0-3)hdmiIndex,(0.1.2.4.9.16)DecStyle>
    CInputPinEnable     inputPinEnable;               //��������ͨ��ʹ������

    int                 freeLicenceNum;               //��ѵ�licence����
    int                 directTransNum;               //�豸֧�ֵ�ֱ��·��
    bool                isDecCombined;                //�������Ƿ�ϳ�����

    bool                isGBDevice;                   //�Ƿ��ǹ����豸

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

//�豸ͨ������
struct TPuChnConfig 
{
private:
    TChannel   devChn;                                  //�豸ͨ��
    TSupportMediaType supportMediaType;                 //ͨ����ǰ֧�ֵ�����ý���ʽ(ֻ��)
    TVideoEncodeParam chnVidFormat;                     //ͨ����ǰ����Ƶ��ʽ
	TAudioEncodeParam chnAudFormat;                     //ͨ����ǰ����Ƶ����
    int        chnVidDynValue;                          //���ͨ����Ƶ��ʽ��H264�����ø�ͨ���Ķ�̬�غ�
    CVidSrcSet availVidSrcSet;                           //ͨ�����õ���ƵԴ�б�

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

//��ƵԴͨ������
struct TPuVidSrcConfig 
{
private:
    string     name;                         //��ƵԴ����
    TChannel   vidChn;                       //��ƵԴͨ��
    CPuChnSet  availChnSet;                  //��ƵԴ��ǰ����ʹ�õ�ͨ���б�
    int        camType;                      //��ƵԴ������ͷ����
    int        camId;                        //��ƵԴ������ͷ��ַ��
	string     azimuth;                      //��λ�ǣ�base64���ܺ�����ģ�ʹ��ǰ��base64����

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
	
	int chnState;  //ͨ��״̬���Ͳμ�emChnStatus
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

//�豸����
struct TPuDevConfig 
{
private:
    CPuBaseCfgList propCfgList;                    //�豸��������
    CPuChnCfgList chnCfgList;                      //�豸ͨ������
    CPuVidSrcCfgList vidSrcCfgList;                //��ƵԴͨ������      
	CPuChnStateList chnStateCfgList;               //ͨ��״̬����   add by ywy for  channel state report
           
  


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
            //������ֱ�ӷ���
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

		//{{{{{{{{{{{{{{{ modify by Royan 20140603 IsCompleted���ͨ��״̬�ж�
		if (GetChnStateCfgList().size() != (u32)tPropCfg.GetBaseCapSet().GetVidEncNum())
		{
			return false;
		}
		//}}}}}}}}}}}}}}} modify by Royan 20140603 IsCompleted���ͨ��״̬�ж�

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

//��ģ���ڲ�ʹ�ã��������紫��
struct TPuConfig 
{
private:
    string            devUri;           //ǰ���豸URI��puid@domain
    TIpAddr           devIp;            //ǰ���豸IP
    string            devType;          //�豸���ͣ��������������
    string            devMode;          //�豸�ͺţ�CU��ʹ�����ж�һЩ��ǰ�˵�������ƽ̨����ʹ��
    string            devName;          //�豸����
    string            manufacturer;     //�豸����
    
    TPuBaseCapSet       baseCapSet;     //����������
    map<int,int>        extCapSet;      //��չ������
	map<int,int>        hdmiVidDecCfg;  //hdmi��Ƶ��������<(0-3)hdmiIndex,(0.1.2.4.9.16)DecStyle>
    CInputPinEnable     inputPinEnable; //��������ͨ��ʹ������

    map<int,CVidFormat> chnVidFormat;   //ͨ����ǰ����Ƶ��ʽ
    map<int,int>        chnVidDynValue; //���ͨ����Ƶ��ʽ��H264�����ø�ͨ���Ķ�̬�غ�

    map<int,TSupportMediaType> supportMediaType; //ͨ����ǰ֧�ֵ�����ý���ʽ(ֻ��)
    map<int,TVideoEncodeParam> chnVidEncParam;   //ͨ����ǰ����Ƶ�������
	map<int,TAudioEncodeParam> chnAudEncParam;   //ͨ����ǰ����Ƶ�������  

    map<int,string> vidSrcName;                    //��ƵԴ����

    map<int,CPuChnSet>  vidSrcChnSet;   //��ƵԴ��ǰ����ʹ�õ�ͨ���б�
    map<int,CVidSrcSet> chnVidSrcSet;   //ͨ�����õ���ƵԴ�б�
    map<int,int>        camType;        //��ƵԴ������ͷ����
    map<int,int>        camId;          //��ƵԴ������ͷ��ַ��
    map<int,int>        bitRate;        //ͨ���ı����� 
	map<TChannelID, TPuChnStateConfig>  chnStatusSet;  //ͨ��״̬�� add by ywy for  channel state report

    int                 freeLicenceNum; //��ѵ�licence����
    int                 directTransNum; //�豸֧�ֵ�ֱ��·��
    bool                isDecCombined;  //�������Ƿ�ϳ�����

    bool                isGBDevice;     //�Ƿ��ǹ����豸

	map<int,string>     azimuth;        //��λ�ǣ�base64���ܺ�����ģ�ʹ��ǰ��base64����

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

    //ת���ӿ�
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
		//add by whui:TPuConfig��chnVidFormatʵ����chnVidEncParam���У�����Ҳ����һ��
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

    //������ƵԴͨ��������ƵԴ����
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

    //���ݱ���ͨ��������ƵԴ����
    string GetVidSrcNameByEncChn(int nEncChn) const
    {
        //�ȸ��ݱ���ͨ���ҵ���Ӧ��ƵԴͨ��
        int nVidSrcChn = -1;
        const map<int,CVidSrcSet>::const_iterator ptVidSrcSet = GetChnVidSrcSet().find(nEncChn);
        if (ptVidSrcSet != GetChnVidSrcSet().end())
        {
            const CVidSrcSet& tVidSrcSet = ptVidSrcSet->second;
            for (CVidSrcSet::const_iterator sVidSrcIt = tVidSrcSet.begin(); sVidSrcIt != tVidSrcSet.end(); ++sVidSrcIt)
            {
                //Ŀǰһ������ͨ��ֻ�ܹ�����һ����ƵԴ��tVidSrcSet���ֻ��һ��Ԫ��
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

//pu����/��������/ͣ�ã���/���ߣ���/ͣ��
struct TPuStatus 
{
private:
    bool isInTop;           //�Ƿ�����
    bool isUsed;            //�Ƿ�����
    bool isOnline;          //�Ƿ�����

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
        OspPrintf(TRUE, FALSE, "\n-----------------------------------PU��������Ϣ-----------------------------------\n\n");

        OspPrintf(TRUE, FALSE, "  -----------------------------------������Ϣ-----------------------------------\n");

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

//pu��ҵ��״̬

struct TPuServiceStatus
{
private:
    //map<int,TChannel>    decChnVidSrc;    //���������ŵ���ƵԴͨ����ͨ����PU��
    map<int,TChannel>    audioCallSrc;    //��Ƶ���е�Դ��ͨ����CU��
    map<int,bool>        isPlatPreRec;    //�Ƿ���ƽ̨Ԥ¼
    map<int,TRecStatus>  platRec;         //�Ƿ���ƽ̨¼��
    map<int,bool>        isPuPreRec;      //�Ƿ���ǰ��Ԥ¼
    map<int,bool>        isPuRec;         //�Ƿ���ǰ��¼��

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

//pu�澯����
struct TAlarmInput
{
private:    
    string            devUri;           //ǰ���豸URI��puid@domain
    string            alarmType;        //�澯����: ALARM_TYPE_PIN, ALARM_TYPE_MOVE��
    int               inputId;          //�澯����ͨ��
    string            alarmFlag;        //�澯��־: ALARM_FLAG_OCCUR, ALARM_FLAG_RESTORE
    int               detAreaId;        //�ƶ�����ʾ�澯����ţ��ƶ����ʱ��Ч
    string            alarmTime;        //�澯ʱ�䣬��pui������
    string            alarmData;        //�澯����
	string            alarmAlias;       //�澯����

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
	string            alarmType;        //�澯����: ALARM_TYPE_PIN, ALARM_TYPE_MOVE��
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
    CAlarmInputArray       alarmArray;       //�澯��Ϣ
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

//�澯Դ
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
    string       devUri;              //�澯�豸
    int          inputId;             //�澯����ͨ��
    string       alarmType;           //�澯����

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

//����������Ϊ������
#define    ALARMLINK_PLAT_REC        "REC"
#define    ALARMLINK_TV_WALL         "TVWALL"
#define    ALARMLINK_READ_SEC        "READSEC"
#define    ALARMLINK_IO_OUT          "IOOUT"
#define    ALARMLINK_CAM             "CAM"

typedef set<string>  CLinkActionList;

//�澯Ŀ��ͨ������Ϊ
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
    TChannel        encChn;            //�澯����Ŀ�ı���ͨ��
    CLinkActionList linkActionList;    //������Ϊ�б�

    //������Ҫ����Դ
    //����ͷ
    int             camPrePos;        //����ͷԤ��λ

    //����ǽ   
    //TChannel        decChn;            //������ͨ��
    string          tvWallId;          //����ǽID
    int             tvId;              //���ӻ�ID
    int             tvDivId;                        //���ӷֻ���ID

    //�澯���
    int             alarmOutChn;       //�澯���ͨ����

    //�û�֪ͨ
    CStrList        msgNoList;                         //���ź����б�
    CStrList        vocNoList;                         //��Ѷ�����б�
    CStrList        mailNoList;                        //���������б�

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


//�澯����ͨ�ýṹ�����������Ͷ�ǰ������
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
    string                   alarmAlias;       //�澯����
    bool                     isUsed;           //�����Ƿ����ã��������Ϊ�Ƿ񲼷�
    TCronTimeList            defenceSched;     //��������ʱ�̱�,ʹ��cronTime
    TAlarmLinkSrc            alarmSrc;         //�澯Դ
    CAlarmLinkDstList        alarmDstList;     //�����б�

public:
    const std::string ToXml()const;
    void FromXml(const std::string& xml_string);
    void ToXml(TiXmlDocument& xmlDoc)const;
    void ToXmlWithoutClassNameNode(TiXmlNode& xmlNode)const;
    void FromXml(const TiXmlNode& xmlNode);
};

typedef vector<TAlarmLinkage>  CAlarmLinkageList;


//���������
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
    int    outPutId;                        //���������ͨ��0-2 ��MAX_IO_OUT_CHNNUM-1��
    string outPutStatus;                    //���������ͨ��״̬:  open\close
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
//{{�����������
/////////////////////////////////////////////////////////////////////

const char* GetPayloadTypeName(unsigned int dwPayloadType);

struct TSwitchInfo
{
private:
    TChannel srcChn;      //Դ
    TChannel dstChn;      //Ŀ��
    string   mediaMode;   //����ģʽ��MODE_VIDEO��MODE_AUDIO��MODE_BOTH
    string   transType;   //����ģʽ:TRANS_UDP��TRANS_TCP
    string   videoType;   //��ǰ�ն˱����ʽ��ֻ�Ա�������Ч)

public:
    void Clear()
    {
        srcChn.Clear();
        dstChn.Clear();
        mediaMode.clear();
        transType.clear();
        videoType.clear();
    }

    //��ȡԴͨ��
    const TChannel& GetSrcChn() const 
    {
        return srcChn;
    }
    //��ȡԴͨ��
    TChannel& GetSrcChn() 
    {
        return srcChn;
    }
    //����Դͨ��
    void SetSrcChn(const TChannel& tSrcChn) 
    {
        srcChn = tSrcChn;
    }

    //��ȡĿ��ͨ��
    const TChannel& GetDstChn() const 
    {
        return dstChn;
    }
    //��ȡĿ��ͨ��
    TChannel& GetDstChn() 
    {
        return dstChn;
    }
    //����Ŀ��ͨ��
    void SetDstChn(const TChannel& tDstChn) 
    {
        dstChn = tDstChn;
    }

    //��ȡ����ģʽ��MODE_VIDEO��MODE_AUDIO��MODE_BOTH
    const string& GetMediaMode() const 
    {
        return mediaMode;
    }
    string& GetMediaMode()
    {
        return mediaMode;
    }
    //���ý���ģʽ��MODE_VIDEO��MODE_AUDIO��MODE_BOTH
    void SetMediaMode(const string& strMediaMode) 
    {
        mediaMode = strMediaMode;
    }

    //��ȡ����ģʽ:TRANS_UDP��TRANS_TCP
    const string& GetTransType() const 
    {
        return transType;
    }
    string& GetTransType()
    {
        return transType;
    }
    //���ô���ģʽ:TRANS_UDP��TRANS_TCP
    void SetTransType(const string& strTransType) 
    {
        transType = strTransType;
    }

    //��ȡ��ǰ�ն˱����ʽ��ֻ�Ա�������Ч��
    const string& GetVideoType() const 
    {
        return videoType;
    }
    string& GetVideoType()
    {
        return videoType;
    }
    //���õ�ǰ�ն˱����ʽ��ֻ�Ա�������Ч��
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
//Video���������
#define VIDEO_ENC_PARAM_RESOLUTION      "VideoResolution"       //��Ƶ�ֱ��ʡ�ֵ��string����,������ʽ"W:%d,H:%d"
#define VIDEO_ENC_FRAME_RATE            "VideoFrameRate"        //����֡�ʡ�ֵ��int����
#define VIDEO_ENC_BIT_RATE              "VideoBitRate"          //����������, Kbps��ֵ��int����
#define VIDEO_ENC_KEY_FRAME_INTVL       "VideoKeyFrameIntvl"    //�����Ĺؼ�֡�����ֵ��int����
#define VIDEO_ENC_ENCODE_FORMAT         "VideoEncodeFormat"     //��Ƶ�����ʽ��ֵ��int����
#define VIDEO_ENC_RATE_TYPE             "VideoRateType"         //�������͡�ֵ��int���ͣ�1-�̶����ʣ�2-�ɱ����ʣ�
#define VIDEO_ENC_CLOCK_RATE            "VideoClockRate"        //��Ƶ�����ʡ�ֵ��int����

//Audio���������
#define AUDIO_ENC_SAMPLE_RATE           "AudioSampleRate"       //�����ʡ�ֵ��int����
#define AUDIO_ENC_CHANNEL_NUM           "AudioChannelNum"       //��������ֵ��int����
#define AUDIO_ENC_ENCODE_FORMAT         "AudioEncodeFormat"     //��Ƶ�����ʽ��ֵ��int����
#define AUDIO_ENC_BIT_RATE              "AudioBitRate"          //����������, Kbps��ֵ��int����
#define AUDIO_ENC_CLOCK_RATE            "AudioClockRate"        //ʱ��������ʡ�ֵ��int����


struct TMediaFormat
{
private:
    int    payloadType;             //�����ʽID
    string encodingName;            //�����ʽ������Ӧ��payloadType������
    string manufacture;             //ý��������
    TMediaFormatParam formatParam;  //��ϸ�ĸ�ʽ����������ʵ����Ҫ���ã�����ֱ��ʡ��ؼ�֡�����

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
//    TNetAddr innerAddr;    //������ַ
//    TNetAddr outerAddr;    //��Ӧ��������ַ
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
    TMediaChannelKey mediaChnKey;   //udp����ʱ��Nat���õ�key
    TNetAddrList addrList;          //������ַ��ǰ�棬������ַ����ں��
    TNetAddr sendBindAddr;         //����αװ���͵�ַ

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
    TMidaType               mediaType;              //ý�����ͣ�VIDEO��AUDIO��DATA
    TMediaFormat            mediaFormat;            //ý���ʽ
    TMediaTransProto        mediaTransProto;        //�磺RTP/AVP/UDP;unicast
    TMediaTransChannelList  mediaTransChnList;      //ý�崫��ͨ����UDP����ʱ��ͨ����RTP��RTCPͨ�������ݴ����TCP����ʱ����ͨ���Ϳ���

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

    //ý���ʽ�ӿ�
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

    //ý�崫��Э��ӿ�
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

    //ý�崫��ͨ���ӿ�
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
	TChannel    srcChn;     //�������ͷ�
	TChannel    dstChn;     //�������շ�

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
#define SDP_SESSION_PLAY        "Play"              //ʵʱ����
#define SDP_SESSION_PLAYBACK    "Playback"          //��ʷ�����ط�
#define SDP_SESSION_DOWNLOAD    "Download"          //��ʷ��������
#define SDP_SESSION_AUDIOCALL   "AudioCall"         //��Ƶ����

typedef string TPlaybackType;
#define PLAYBACK_TYPE_PLATFORM    "PLATFORM_PLAYBACK"   //ƽ̨����
#define PLAYBACK_TYPE_PU          "PU_PLAYBACK"         //ǰ�˷���


//******************************************************************************
// ������:    IsRealTimeMediaSession
// ����:      �ж��Ƿ�ʵʩ�����Ự��ʵʩ����������Ҫ�ַ�����ʷ�������ַ�
// �㷨ʵ��:  
// ����Ȩ��:  public 
// ����˵��:  const TSdpSessionName & tSdpSessionName
// ����ֵ:    bool
//-----------------------------------------------------------
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2013/03/19     huangzhichun huangzhichun    ����
//******************************************************************************
inline bool IsRealTimeMediaSession(const TSdpSessionName& tSdpSessionName)
{
    return tSdpSessionName == SDP_SESSION_PLAY || tSdpSessionName == SDP_SESSION_AUDIOCALL;
}

struct TSdpUri
{
private:
    TRecToken   recToken;                     //¼���ļ�url
    TChannel    puChn;              
    TPlaybackType playbackType;    
    string      recDomainName;                  // ¼��������

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
static const string SDP_EXT_INFO_PLAYBACK_SSN = "PlaybackSsn";			//��������ỰUri; ����INVITE_RSPʱ�������ձ�������豸�����ɣ�����Vcr���ƺͽ��Ȳ�ѯʱҪ�á�ֵ��string����
static const string SDP_EXT_INFO_PLAYBACK_START_TIME = "PlaybackStartTime";	//ƽ̨¼��Ϊ֧�ֵ���ʱ������INVITE_RSP���ظ�CU����ʼ����ʱ�䡣ֵ��int����
static const string SDP_EXT_INFO_FILE_PATH = "FilePath";				//¼������INVITE_RSPʱ����¼��ģ��(NRU)���ɣ��ͻ���RPCTRLҪ�á�ֵ��string����
static const string SDP_EXT_INFO_FILE_NAME = "FileName";				//¼������INVITE_RSPʱ����¼��ģ��(NRU)���ɣ��ͻ���RPCTRLҪ�á�ֵ��string����
static const string SDP_EXT_INFO_STREAM_TAG = "StreamTag";	            //������ǩ(��ʽ"����:MEDIA_STREAM_TYPE",��kedacom:RTP_ES)

static const string SDP_EXT_INFO_DIRECT_TRANSFER = "DirectTransfer";	//�Ƿ�����ֱ��, ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_PLAT_DIRECT_TRANSFER = "PlatDirectTransfer";	//�Ƿ�����ƽֱ̨��, ֵ��TRUE_STR/FALSE_STR

static const string SDP_EXT_INFO_OFFLINE_SWITCH = "OfflineSwitch";		//�Ƿ�֧�����߽���; INVITE_REQ��VTDUʱЯ��; ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_NETSEG_NO = "NetSegNO";	            //���κ�; INVITE_REQ��VTDUʱЯ��; ֵ��int����
static const string SDP_EXT_INFO_IS_NEED_PUNCH = "NeedPunch";	        //�Ƿ���ҪPunch
static const string SDP_EXT_INFO_IS_NEED_GBPUNCH = "NeedGBPunch";	    //�Ƿ���ҪGBPunch
static const string SDP_EXT_INFO_GBPUNCH_GBCHNID = "GBPunchChnId";	    //GBPunch��ͨ��ID
static const string SDP_EXT_INFO_GBPUNCH_GBPLATID = "GBPunchPlatId";	//GBPunch��ƽ̨ID
static const string SDP_EXT_INFO_USE_DOMAIN_SOCKET = "DomainSocket";	//�Ƿ�ʹ�����׽��֣�ֵ��TRUE_STR/FALSE_STR

static const string SDP_EXT_INFO_DROP_RTP_EXTEND = "DropRtpExtend";     //�Ƿ�ȥRtp��չͷ��ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_USE_AES256_ENCRYPTION = "AES256Encryption";//�Ƿ�ʹ��AES256���ܣ�ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE = "MrtcEnable";            //�Ƿ�����MRTC��ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE_SRC = "MrtcEnableSrc";     //�Ƿ�������Դ��MRTC��ȱʡ���ã�ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_MRTC_ENABLE_DST = "MrtcEnableDst";     //�Ƿ�����Ŀ�Ĳ�MRTC��ȱʡ���ã�ֵ��TRUE_STR/FALSE_STR
static const string SDP_EXT_INFO_ADDR_CHANGE_SIDE = "AddrChangeSide";   //�޸ĵ�ַһ�ֵ࣬��"SRC"/"DST"
const string ADDR_CHANGE_SIDE_SRC = "SRC";
const string ADDR_CHANGE_SIDE_DST = "DST";

static const string SDP_EXT_INFO_IS_NAT_PACKET_MODE = "NatPacketMode";  //�Ƿ�NAT̽���ģʽ
static const string SDP_EXT_INFO_INTERFACE_IP = "InterfaceIp";          //�ӿ�IP��ַ

static const string SDP_EXT_INFO_TIMESTAMP_TYPE = "TimestampType";          //ʱ������ͣ�ֵΪTIMESTAMP_TYPE_STREAM����TIMESTAMP_TYPE_PROGRESS
#define TIMESTAMP_TYPE_STREAM   ("Stream")
#define TIMESTAMP_TYPE_PROGRESS   ("Progress")

static const string SDP_EXT_INFO_SECURE_TYPE = "SecureType";             //��ȫ���ͣ� ֵΪ SDP_EXT_INFO_SECURE/UNSECURE, ��Ϊ�����ü� ����Ϊ����ȫ
static const string SDP_EXT_INFO_VKEK_LIST   = "VkekList";               // vkek�б� ֵ Ϊ  vector moduleVkek
static const string SDP_EXT_INFO_MTS_BT_RTP_IP    = "mts_bt_rtp_ip";     // vkek�б� ֵ
static const string SDP_EXT_INFO_MTS_BT_RTP_PORT  = "mts_bt_rtp_port";   // vkek�б� ֵ
static const string SDP_EXT_INFO_MTS_AT_RTP_IP	  = "mts_at_rtp_ip";     // vkek�б� ֵ
static const string SDP_EXT_INFO_MTS_AT_RTP_PORT  = "mts_at_rtp_port";   // vkek�б� ֵ
static const string SDP_EXT_INFO_MTS_AT_RTCP_IP	  = "mts_at_rtcp_ip";    // vkek�б� ֵ
static const string SDP_EXT_INFO_MTS_AT_RTCP_PORT = "mts_at_rtcp_port";  // vkek�б� ֵ

static const string SDP_EXT_INFO_TRANSFER_RATE = "Rate";                  //���䱶�٣�ֵ��int����
static const string SDP_EXT_INFO_FILE_SIZE = "FileSize";                  //�ļ���С��ֵ��double���ͣ��ֽڣ�

static const string SDP_EXT_INFO_RESOURCE_ID = "ResourceId";	          //��Դid���ַ�����
static const string SDP_EXT_INFO_SWITCH_FLOW = "CmuFlow";	          //�������̣�Ŀǰ�����֣�һ���ǻ���cmu���ȵ������̣�һ���ǻ���redis�ģ�
#define SWITCH_FLOW_CMU ("Cmu")
#define SWITCH_FLOW_REDIS   ("Redis")

static const string SDP_EXT_INFO_TCPSTREAM = "TcpStream";               //tcp����������͸����չ��Ϣ��ֵ����PASSIVE��/��ACTIVE��
//{{{{{{{{{{{{{{{ modify by Royan 20140605 ����������
static const string SDP_EXT_INFO_GB_STREAM_KEEPALIVE_INFO = "GBStreamKAInfo"; //��������������
#define STREAM_KA_SR        ("SR")
#define STREAM_KA_RR        ("RR")
#define STREAM_KA_SR_RR     ("SRRR")
//}}}}}}}}}}}}}}} modify by Royan 20140605 ����������
//{{{{{{{{{{{{{{{ add by Royan 20140903 SSRC
static const string SDP_EXT_INFO_SSRC = "SSRC";	                        //SSRC
//}}}}}}}}}}}}}}} add by Royan 20140903 SSRC
static const string SDP_EXT_INFO_GB_REC_LOCATION = "GBRecLocation";     //ָ������¼��λ�ã�ֵ��������ID���߹���ǰ��ID��
static const string SDP_EXT_INFO_GMT_LABEL = "GMTLabel";                //�Ƿ�֧������ʱGMT��ʽ


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

//����INVITE_RSPЯ������չ��Ϣ:
//SDP_EXT_INFO_PLAYBACK_SSN, SDP_EXT_INFO_PLAYBACK_START_TIME
//¼������INVITE_RSPЯ������չ��Ϣ:
//SDP_EXT_INFO_PLAYBACK_SSN��SDP_EXT_INFO_FILE_PATH, SDP_EXT_INFO_FILE_NAME

//ʵʱ����INVITE_REQ:
//SDP_EXT_INFO_OFFLINE_SWITCH, SDP_EXT_INFO_DIRECT_TRANSFER

//******************************************************************************
// ������:    TSdp
// ����:      �Ự����Э����
// �㷨ʵ��:  

// TSdpý�������б� ����ʾ����
//�Ƽ���
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

//����
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
// �޸ļ�¼:
// ��      ��     �޸���        �߶���          �޸ļ�¼
// 2011/11/04     huangzhichun huangzhichun    ����
//******************************************************************************
struct TSdp
{
private:
    TChannel            owner;              //sdp�Ĵ����ߣ���mediaDescList������˭��
    TSdpSessionName     sessionName;        //��������
    TSdpUri             uri;                //��������INVITE_REQʱ��������ʱ����
    TTimeRange          timeRange;          //��������INVITE_REQʱ��������ʱ���Բ���
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

		mediaDescList.clear();  //�ƺ����Բ������ԭ�ȵ���Ϣ����
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
            //����tMediaTransChnList
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
                    //��Դ�����Ҳ�����Ӧ��Chn���Key�ÿ�
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
        //����TMediaDescList
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
                //����tMediaTransChnList
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
                //����tMediaTransChnList
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
                //����tMediaTransChnList
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
        //����TMediaDescList
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
        //ͨ������£�ֻ��Video/Audio��RTP/RTCPͨ�����ܹ���4·����ͨ��
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
            //����tMediaTransChnList
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
        //����TMediaDescList
        for(TMediaDescList::const_iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //����TMediaTransChannelList
            const TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
            for(TMediaTransChannelList::const_iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                //����TNetAddrList
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
        //����TMediaDescList
        for(TMediaDescList::iterator itrMediaDesc = mediaDescList.begin(); itrMediaDesc != mediaDescList.end(); itrMediaDesc++)
        {
            //����TMediaTransChannelList
            TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
            for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
            {
                //����TNetAddrList
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

    //TExtInfoϸ���ӿ�
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
	//{{{{{{{{{{{{{{{ modify by Royan 20140606 ����������
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
	//}}}}}}}}}}}}}}} modify by Royan 20140606 ����������
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

//����ͨ��
class TSwitchChannel
{
protected:
    TSwitchSsnID        switchSsnID;            //�����ĻỰID
    string              mediaType;              //ý�����ͣ�VIDEO��AUDIO
    TMediaFormat        mediaFormat;            //ý���ʽ
    TMediaTransProto    mediaTransProto;        //�磺RTP/AVP/UDP;unicast
	TTransChannel		transChnType;           //RTP,RTCP,DATA
    TSwitchType         switchType;             //VTDU��ת����������: M2P/P2P;
    TMediaStreamType    mediaStreamType;         //PS/ES
	bool			    isGapSwitch;            //�Ƿ�Խ��բ
	bool                isDomainSocket;			//�Ƿ�ʹ����socket
    bool                isUseRecvProxy;         //DS�Ƿ�ʹ�ý��մ���ģ���Դ��˿�ת��

    TChannel            srcDevChn;
    TMediaChannelKey    srcMediaChnKey;
    TNetAddrList        srcNetAddrList;

    TChannel            dstDevChn;
    TMediaChannelKey    dstMediaChnKey;
    TNetAddrList        dstNetAddrList;

    TNetAddr            rcvNetAddr;
    TNetAddr            rcvBindNetAddr; //HZC:�鲥���յ�ʱ�����
    TNetAddr            sndBindNetAddr; //HZC:��RawSocketαװ���͵�ʱ�����Ҫ��

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

    //ԴKey�ӿ�
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

    //Դ��ַ�ӿ�
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

    //Ŀ��key�ӿ�
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

    //Ŀ�ĵ�ַ�ӿ�
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

    //���յ�ַ�ӿ�
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

    //���հ󶨽ӿ�
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

    //���Ͱ󶨽ӿ�
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
//}}�����������
/////////////////////////////////////////////////////////////////////


//����ǽ���

struct TTvChannel
{
private:

    string          tvWallId;                       //����ǽID
    int             tvId;                           //���ӻ�ID
    int             tvDivId;                        //���ӷֻ���ID

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
    int           pollIndex;                   //��ѯ���
    TChannel      encChn;                      //������ͨ��
    int           pollTime;                    //��ѯʱ�䣬Ĭ��Ϊ0����ʾ�����һֱ���Ų���ѯ
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

//���ڷֻ���
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
    int              tvWinDivId;                             //���ӷֻ���id��Ҳ���Ƕ�ӦdecChn
    CTvPollList      pollList;                               //��ѯ��Ϣ�б�
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

//����
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
    int              tvWinId;                                //���ӻ�id
    ETvWinStype      style;                                  //���Ӵ��ڷ��Ҳ���ǽ��������
    TTvWinDivList    divList;                                //�����ֻ������Ϣ
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
    string      schemeName;                      //Ԥ������
    //string      schemeId;                        //Ԥ��ID, �´���ʱ����Ҫ���tvs����ΨһID�󷵻ظ��û�
    string      tvWallId;                        //����ǽID
    //string      ownerType;                       //Ԥ��ӵ�����ͣ�public/private
    CTvWinList  tvWinList;                       //����ǽTV�б�Ҳ�ͱ�/�������Ķ�Ӧ��ϵ
    //string      userId;                          //Ԥ���û���ID
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

    //string      schemeId;                        //Ԥ��ID, �´���ʱ����Ҫ���tvs����ΨһID�󷵻ظ��û�
    int         pollIndex;                       //��ѯ���
    string      schemeName;                      //Ԥ������
    int         pollTime;                        //��ѯʱ�䣬Ĭ��Ϊ0����ʾ�����һֱ���Ų���ѯ
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

//Ԥ���飬һ������ǽֻ��Ψһһ��Ԥ����
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
    string      tvWallId;                               //����ǽID
    CTTvWallSchemePollList pollList;                   //��ѯ�б�
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

//���ڷֻ�����ѯ״̬����
#define   WIN_POLL_START       "start"
#define   WIN_POLL_PAUSE       "pause"
#define   WIN_POLL_STOP        "stop"

//���ڷֻ�����������
#define   WIN_MEDIA_SERVICE_REAL_MEDIAL       "real_media"
#define   WIN_MEDIA_SERVICE_PLAT_REC          "plat_rec"
#define   WIN_MEDIA_SERVICE_PU_REC            "pu_rec"

//���ڷֻ���״̬

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
    int              tvDivId;                    //���ӷֻ���ID
    TChannel         tvChnVidSrc;                //���Ӳ��ŵ���ƵԴͨ����ͨ����PU, ����ʱ�Ƿ���session��
    int              playState;                  //��ǰ��ƵԴ�Ĳ���״̬���ɹ�:CMS_SUCCESS��ʧ�ܣ�������
    string           winPollState;               //��ѯ״̬
    string           mediaService;               //ý��������ͣ�WIN_MEDIA_SERVICE_REAL_MEDIAL��

    //��Դ��¼��ʱ����Ҫ���²���
    TRecToken        recToken;                   //¼��Token
    TTimeRange       timeRange;                  //¼��ʱ���
    TChannel         encChn;                     //¼�������ͨ��

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

//����״̬
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
    int                   tvId;                              //���ӻ�id
    TTvWinDivStateList    divStateList;                      //�����ֻ����״̬
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


//Ԥ����ѯ״̬����
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
    TTvWallSchemeGroup  schemeGroup;                //Ԥ����
    string   schemePollState;                       //Ԥ����ѯ״̬: start/pause/stop
    TTvWallScheme   curScheme;                      //��ǰԤ��
    CTvWinStateList winStateList;                   //��ǰԤ��ÿ�����ڵ�״̬  

    //��ϱ�������Ӧ�ı�������cu����������tvwall�ĵ�ǰ״̬
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


//�ƶ��������ṹ
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
    bool enable;                                //���ʹ��
    int  leftTopX;                              //������򶥵������
    int  leftTopY;                              //������򶥵�������
    int  width;                                 //���������
    int  height;                                //�������߶�
    int  alarmOccurRate;                        //�������澯�����ٷֱȣ����ڴ��ٽ�ֵ�澯0-100)
    int  alarmRestoreRate;                      //�����澯���˶���Χ�ٷֱ�С�ڸ�ֵ��ָ��澯(��ֵС��alarmOccurRate,0-100)
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

//ͼ���ڱβ����ṹ
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
    bool enable;                                             //�ڱ�ʹ��
    int  leftTopX;                                           //�ڱ����򶥵������
    int  leftTopY;                                           //�ڱ����򶥵�������
    int  width;                                              //�ڱ�������
    int  height;                                             //�ڱ�����߶�
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

//GPS������أ��Ѿ��޷���ȫ����͸�����ݵķ�ʽʵ��

/* GPS���Ĺ���ʵ��˼·
   1. cu��֪�Ĳ�����a. gps���õ����úͻ�ȡ��b. ����/ȡ������gps���ݡ�
   2. cu������Ҫ����ǰ��gps��ʹ��״̬��
   3. cu����gpsʱ��cmu������pu����gpsʹ������(͸�����ݶ�ƽ̨��͸����)��Ȼ�����cu����gps�������͡�
   4. gps���ݵ���cmuʱ��cmu���ݶ��Ĺ����û��б���gps���ݡ�gps���ݲ���cmu���棬���͸澯��һ����
   5. cuȡ��gps����ʱ��cmu�����жϸ��豸�Ƿ񻹴����û�����gps���ݣ����û�о�ȡ��pu��gpsʹ������(͸�����ݶ�ƽ̨��͸����)��
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
    int        type;                                    //͸����������
    string     param;                                   //gps���ò���
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
    bool       enable;             	                   //�Ƿ���
    int        type;                                   //͸����������
    int        subType;                                //͸������������
    string     param;                                  //gps���Ĳ���
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
    int        type;                                   //͸����������
    int        subType;                                //͸������������
    string     param;                                  //gps��ѯ���Ĳ���
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
    int        type;                                   //͸����������
    int        subType;                                //͸������������ ����byType��Լ�������ʽ������ʱӦ��Ϊ0
    string     data;                                   //gps����
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

//͸������
typedef  string TPuTransDataType;
#define PU_TRANS_DATA_TYPE_COMDATA       "ComData"       //��������
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
    TPuTransDataType  type;                   //͸����������
    string            data;                   //base64��ʽ��͸������
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
    int        type;                  //͸����������
    string     param;                 //���ò���
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

//ǰ��¼�����
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
    string             recType;                    //¼������
    string             fileId;                     //¼���ļ�Ψһ��ʶ
    string             filePath;                   //¼���ļ�·��
    string             fileName;                   //¼���ļ�����
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

    //string����tokenת��
    #define   STR_TOKEN_SEP_FLAG   " "

    string ToString() const
    {
        string strToken;

        //ǰ��ֻ��Ҫ¼�����ͺ��ļ�ID
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
    TPuRecToken           recToken;                    //¼��Token
    TTimeRange            timeRange;                   //ʱ���
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
#define VCRCMD_START    "START"         //��ʼ����
#define VCRCMD_STOP     "STOP"          //ֹͣ����
#define VCRCMD_PAUSE    "PAUSE"         //��ͣ����
#define VCRCMD_RESUME   "RESUME"        //�ָ�����
#define VCRCMD_SEEK     "SEEK"          //ʱ�䶨λ
#define VCRCMD_FASTPLAY "FASTPLAY"      //���
#define VCRCMD_SLOWPLAY "SLOWPLAY"      //����
#define VCRCMD_ONEFRAME "ONEFRAME"      //��֡����
#define VCRCMD_KEYFRAME "KEYFRAME"      //ֻ�Źؼ�֡
#define VCRCMD_KEYSEEK "KEYSEEK"       //ʱ�䶨λ(��ǰһ���ؼ�֡��ʼ)
#define VCRCMD_REVERSEPLAY "REVERSEPLAY"   //����
#define VCRCMD_FORWARDPLAY "FORWARDPLAY"   //����
#define VCRCMD_AUTO_KEYFRAME "AUTOKEYFRAME"			//�Զ�ģʽ���Ƿ��֡��NRUȷ�����ͱ���ȫ֡���߱���ֻ�Źؼ�֡
#define VCRCMD_FORCE_KEYFRAME "FORCEKEYFRAME"		//ǿ��ֻ�Źؼ�֡
#define VCRCMD_FORCE_ALLFRAME "FORCEALLFRAME"		//ǿ�ƻط�ȫ֡
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
    int                 totalPack;              		    //�ܰ���
    int                 curPackIndex;              	        //��ǰ����������,��1��ʼ
    string              data;                               //base64��ʽ�����ݰ�

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
//¼���ļ�/�¼�����/ͼƬ�¼�
#define MTI_TYPE_H264					   0x10 //h264�ļ�����
#define MTI_QUIEY_ALL_EVENT                0    //��ѯ�����¼�
#define MTI_TYPE_PIN_ALARM                 1    //���ڸ澯¼��
#define MTI_TYPE_MOTION_DETECT_ALARM       2    //�ƶ����澯¼��
#define MTI_TYPE_MANUAL                    3    //�ֶ�¼��
#define MTI_TYPE_TIMELY                    4    //��ʱ¼��
#define MTI_TYPE_PERIODLY                  5    //����¼��
#define MTI_QUIEY_FILE                     6    //���ļ���ѯ
*/

//ͼƬ��ѯ��ʽ
#define PU_PIC_QUERY_BY_CHN                  "by_chn"        //��ͨ���Ų�ѯ
#define PU_PIC_QUERY_BY_TIME                 "by_time"       //��ʱ���ѯ
#define PU_PIC_QUERY_BY_EVENT                "by_event"      //���¼���ѯ

//ͼƬ�¼�����
#define PU_PIC_EVENT_ALL                     "all"           //��ѯ�����¼�
#define PU_PIC_EVENT_PIN_ALARM               "pin_alarm"     //���ڸ澯
#define PU_PIC_EVENT_MOTDET_ALARM            "motion_detect" //�ƶ����澯
#define PU_PIC_EVENT_MANUAL                  "manual"        //�ֶ�


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
    string             eventType;                               //ͼƬ�¼�����
    string             resolution;                              //ͼ��ֱ���
    int                fileFormat;                              //�ļ���ʽ
    string             picID;                                   //ͼƬ�ļ�ID
    string             snapTime;                                //ץ��ʱ��
    string             fileName;                                //�ļ�����
    TChannel           devChn;                                  //ǰ��ͨ��
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
    int isHasNat;       //����nat��ϵ              
    int ethNo;          //������                   
    TIpAddr ipAddr;     //����������Ӧ��ip               
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

//ǰ�˳���Ļ����
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
    int state;                         //������Ļ״̬ DISABLE-������ TITLE-���ַ���ģ LOGOBMP-��ͼƬ̨��
    int leftMargin;                    //������Ļ��߾� 1-200;
    int topMargin;                     //������Ļ�ϱ߾� 1-200;
    int color;                         //��ƵԴ��Ļ��ɫ;
    string text;                       //��ƵԴ��Ļ����;
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

//{{{{{{{{{{{{{{{ add by Royan 20140418 ������Ļ
//todo ����ϸ��
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
	int state;                         //������Ļ״̬ DISABLE 0-������; TITLE 1-���ַ���ģ; LOGOBMP 2-��ͼƬ̨��
	int leftMargin;                    //������Ļ��߾� 1-200;
	int topMargin;                     //������Ļ�ϱ߾� 1-200;
	int color;                         //��ƵԴ��Ļ��ɫ;
	string text;                       //��ƵԴ��Ļ����;

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
	int rowNum;                        //��Ļ����
	TOsdParamList osdParamList;        //��Ļ�б�

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
//}}}}}}}}}}}}}}} add by Royan 20140418 ������Ļ

//ǰ��̨�괫�����
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
    int width;                              //ͼ����
    int heigth;                             //ͼ��߶�
    int totalPack;               		    //�ܰ���
    int curPackIndex;               	    //��ǰ����������,��1��ʼ
    string bmpOsdBuf;               	    //BMPOSD���ݣ�Base64����������
    int fontNum;               			    //�ַ�����
    int fontHeight;               		    //�ַ���
    string fontWidth;               	    //�ַ���ȣ�Base64����������
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
		pinAlarmEnable = false;		//���ڸ澯ץͼʹ�� 0�ر�/1����
		mtionDetectEnable = false;	//�ƶ����ץͼʹ�� 0�ر�/1����	
		interval = 0;		
		maxSnapNum = 0;
	}
private:
	//���ڸ澯ץͼʹ�� 0�ر�/1����
	bool pinAlarmEnable;

	//�ƶ����ץͼʹ�� 0�ر�/1����
	bool mtionDetectEnable;

	//ץ��ʱ����(ms)
	int	interval;	

	//���ץ������
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
		baudRate = 0;	  //���ڲ�����
		comPortType = 0;  //�������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
		rS232Enable = false;
		rS422Enable = false;
		rS485Enable = false;	
	}
private:
	//���ں�
	int serialPortID; 

	//���ڲ�����
	int baudRate;

	//�������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
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

//{{{{{{{{{{{{{{{ add by Royan 20140526 ǰ��¼��״̬֪ͨ
struct TPuChnRecStatInfo
{
private:
	bool isManualRecording;		    // �Ƿ����ֶ�¼��
	bool isMotiveAlarmRecording;	// �Ƿ����ƶ����澯¼��
	bool isPinAlarmRecording;	    // �Ƿ��ڲ��ڸ澯¼��
	bool isTimerRecording;	        // �Ƿ��ڶ�ʱ¼��
	bool isWeekRecording;		    // �Ƿ�������¼��
	bool isPreRecording;			// �Ƿ���Ԥ¼

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
//}}}}}}}}}}}}}}} add by Royan 20140526 ǰ��¼��״̬֪ͨ

struct TProbeOnvifPuInfo
{
private:
	string         devIp;             //ǰ���豸ip
	string         devUUID;           //ǰ���豸UUID
	string         devSessionId;      //�豸session��������UUID��

	string         devAuthUserName;    //�豸��Ȩ�û���
	string         devAuthPassWord;   //�豸��Ȩ����

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
	string deviceId;               	// �豸ID.
	int videoSrcId;               	// ��ƵԴID.
	bool deviceOnline;              // ��ƵԴ���ڵ��豸�Ƿ�����
	emChnStatus emChnStat;          // ��ƵԴ��Ӧ��ͨ��״̬
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
	string peerDomainName;          // �Զ�����
	string peerIP;                  // �뱾��ͨ�ŵĶԶ����ַ
	string localIP;                 // ��Զ���ͨ�ŵı����ַ
	TDomainOperateType operateType; // �������ͣ�ADD/DEL

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
	string localIP;        // �����ַ
	int    localPort;      // ����˿�
	string mappingIP;      // ӳ����ַ
	int    mappingPort;    // ӳ���˿�
	TNatProtocolType protocol;	     // Э�����ͣ�TCP/UDP
	TMappingOperateType operateType; // �������ͣ�ADD/DEL/MOD

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
//string��ʽΪ"UDP_1.1.1.1_40000"��Э������_����IP_���ض˿ڡ�
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

//30w�豸�������Ż��ṹ��
enum Enum30WPuChnStat
{
	STAT_PU_CHN_IDLE,
	STAT_PU_CHN_ONLINE,
	STAT_PU_CHN_OFFLINE
};
//key(int)Ϊͨ��id��value(int)ΪEnum30WPuChnStat����
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




