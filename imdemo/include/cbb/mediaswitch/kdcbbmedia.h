#ifndef _KD_CBB_MEDIA_H_
#define _KD_CBB_MEDIA_H_

#include "kdvtype.h"

#define KD_MAX_LEN_URI                          511

#define KD_MAX_LEN_PAIR_KEY                     64
#define KD_MAX_LEN_PAIR_VALUE                   64
#define KD_MAX_LEN_URI_SCHEME                   8
#define KD_MAX_LEN_URI_SCHEME_SPECIFIC_PART     256
#define KD_MAX_LEN_URI_SEGMENT                  128
#define KD_MAX_NUM_TRACK                        8
#define KD_MAX_LEN_NAME                         256
#define KD_MAX_LEN_NETCARD_NAME                 32
#define KD_MAX_NUM_NET_CARD                     3

//媒体属性的key定义
#define KD_MEDIA_PROPERTY_KEY_MEDIATYPE             "MeidaType"
#define KD_MEDIA_PROPERTY_KEY_ENCODINGNAME          "EncodingName"
#define KD_MEDIA_PROPERTY_KEY_WIDTH                 "Width"
#define KD_MEDIA_PROPERTY_KEY_HEIGHT                "Height"
#define KD_MEDIA_PROPERTY_KEY_BITRATE               "BitRate"
#define KD_MEDIA_PROPERTY_KEY_FRAMERATE             "FrameRate"
#define KD_MEDIA_PROPERTY_KEY_KEYFRAMEINTVAL        "KeyFrameIntval"
#define KD_MEDIA_PROPERTY_KEY_CHANNELNUM            "ChannelNum"
#define KD_MEDIA_PROPERTY_KEY_SAMPLERATE            "SampleRate"
#define KD_MEDIA_PROPERTY_KEY_BITPERSAMPLE          "BitPerSample"

//帧数据大小限制
#define KD_MAX_FRAM_SIZE    (512*1024)

//NTP UTC 时间转换
#define  CBB_JAN_1970   (2208988800UL)    /*(3600 * 24 *(365 * 70 + 17))*/
#define  CBB_USEC(x) (((u64)x*1000000) >> 32)
#define  CBB_NTPFRAC(x) ((((u64)x)<<32)/1000000)



#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef WIN32 //vc6 不支持inline
//#define inline _inline
#endif

static __inline u64 Cbb_UtcToNtpTime(u64 qwUtcTime)
{
	u32 dwSec, dwUSec;
	u64 qwNtpTime;

	dwSec = (u32)(qwUtcTime/1000) + CBB_JAN_1970;
	dwUSec = CBB_NTPFRAC((qwUtcTime%1000)*1000);
	//*((u32*)(&qwNtpTime) + 1) = dwSec;
	//*((u32*)(&qwNtpTime)) = dwUSec;
	qwNtpTime = dwSec;
	qwNtpTime = (qwNtpTime<<32)|dwUSec;
	return qwNtpTime;
}

static __inline u64 Cbb_NtpToUtcTime(u64 qwNtpTime)
{
	u32 dwSec = 0;
	u32 dwUsec = 0;
	dwSec = (u32)(qwNtpTime >> 32) -  CBB_JAN_1970;
	dwUsec = (u32)(qwNtpTime & 0xffffffff);
	dwUsec = CBB_USEC(dwUsec);
	return ((u64)dwSec)*1000 + (dwUsec/1000);
}


/************************************************************************/
/* 枚举定义                                                             */
/************************************************************************/

//媒体流类型
typedef enum tagKDTrackType
{
    E_KD_TrackType_Video = 1,
    E_KD_TrackType_Audio,
    E_KD_TrackType_MetaData,
    E_KD_TrackType_Unknown,
}EKDTrackType;

//数据封装格式
typedef enum tagKDDataPackFormat
{
    E_KD_DataPackFormat_Pack  = 1,             /*数据来源为RTP数据包TKDPack*/
    E_KD_DataPackFormat_Frame =2,              /*数据来源为帧数据TKDFrame*/
	E_KD_DataPackForamt_Raw	  = 3,			   /*数据来源为未知数据类型，包括第三方码流*/
}EKDDataPackFormat;

// 数据传输推拉模式
typedef enum tagKDGetDataMode
{
    E_KD_GET_DATA_MODE_PUSH,            /* 数据源主动推送*/
    E_KD_GET_DATA_MODE_PULL,            /*数据目的主动拉数据*/
}EKDGetDataMode;

/************************************************************************/
/*          结构体定义                                                  */
/************************************************************************/

typedef u32 KDRESULT;
//typedef u16  TKDPos;
/*
Scheme://scheme_specific_part#segment
file://sdcard/ipw200/video/20150101120809-11.mp4
file//filepath/filename
kfs://diskid=|taskid=
*/

typedef struct tagKDUri
{
    s8    m_szScheme[KD_MAX_LEN_URI_SCHEME+1];
    s8    m_szSchemeSpecificPart[KD_MAX_LEN_URI_SCHEME_SPECIFIC_PART+1];
    s8    m_szSegment[KD_MAX_LEN_URI_SEGMENT+1];
}TKDUri;

typedef struct tagKdPair
{
    s8 m_aszKey[KD_MAX_LEN_PAIR_KEY+1];
    union
    {
        s64 m_intVar;
        double m_dbVar;
        s8 m_szVar[KD_MAX_LEN_URI_SCHEME_SPECIFIC_PART+1];
    }m_unValue;
}TKdPair;


typedef struct tagKDFrame
{
    u8     m_byMediaType;            /*媒体类型*/
    u8    *m_pData;                /*帧数据缓冲*/
    u32    m_dwPreBufSize;        /*m_pData缓冲前预留了多少空间，用于加*/
                                /* RTP option的时候偏移指针一般为12+4+12*/
                                /* (FIXED HEADER + Extence option + Extence bit)*/
    u32    m_dwDataSize;            /*m_pData指向的实际缓冲大小缓冲大小*/
    u8     m_byFrameRate;            /*发送帧率,用于接收端*/
    u8     m_byPadSize;            /*填充数据长度*/
    u32    m_dwFrameID;            /*帧标识，用于接收端*/
    u32    m_dwTimeStamp;            /*时间戳, 用于接收端*/
    u32    m_dwSSRC;                /*同步源, 用于接收端*/
    u64    m_llNTPTime;            /*NTP时间戳*/
    union
    {
        struct
        {
            BOOL32    m_bKeyFrame;    /*频帧类型（I or P）*/
            u16       m_wVideoWidth;  /*视频帧宽*/
            u16       m_wVideoHeight; /*视频帧宽*/
        }m_tVideoParam;
        struct
        {
            u32       m_dwSample;        /*音频采样率*/
            u16       m_wChannel;        /*声道数*/
            u16       m_wBitsPerSample;    /*位宽*/
        }m_tAudioParam;
    }x;
	u8   m_byStreamID;//PES包里面的音频 0-音频流0 1-音频流1
	//u32    m_dwRtpTimeStamp;          /*时间戳，接收的RTP协议中携带的时间戳*/
}TKDFrame;

typedef struct tagKDPack
{
    u32    m_dwTS;
    u32    m_dwSSRC;
    u16    m_wSn;
    u8     m_byMediaType;
	u8     m_byPrever;
    u8*    m_pbyPackBuffer;
    u16    m_wPackBufferSize;
    BOOL32 m_bFirstPackOfFrame;
    BOOL32 m_bLastPackOfFrame;
	u16    m_wPackNum;           //此包所在的帧对应的RTP包的总个数；
	u64    m_qwSysTime;          //UTC时间，RTP包到NVR后，接收模块获取到的系统时间；单位MS
    union
    {
        struct
        {
            BOOL32    m_bKeyFrame;    /*频帧类型（I or P）*/
            u16       m_wVideoWidth;  /*视频帧宽*/
            u16       m_wVideoHeight; /*视频帧宽*/
        }m_tVideoParam;
        struct
        {
            u32       m_dwSample;           /*音频采样率*/
            u16       m_wChannel;           /*声道数*/
            u16       m_wBitsPerSample;     /*位宽*/
        }m_tAudioParam;
    }x;
}TKDPack;

typedef struct tagKDRange
{
    u64 m_qwStart;
    u64 m_qwEnd;
}TKDRange;

/*

m_eTrackType                |   m_dwTrackIndx        |
————————————————————————————————————————————————————————————————
E_KD_TrackType_Video        |        0             |    第一路视频流
————————————————————————————————————————————————————————————————
E_KD_TrackType_Video        |        1            |    第二路视频流
————————————————————————————————————————————————————————————————
E_KD_TrackType_Audio        |        0            |    第一路音频流
————————————————————————————————————————————————————————————————
E_KD_TrackType_MetaData    |        0            |    第一路元数据流
————————————————————————————————————————————————————————————————
*/
typedef struct tagRPTrackID
{
    EKDTrackType m_eTrackType;
    u32            m_dwTrackIndx;      /*针对不同的TrackType,该索引值均从0开始*/
}TKDTrackID;

/*
typedef struct tagKDPad
{
    TKDPad* ptPeerPad;         // 通道句柄
    TKDTrackID tTrackID;        // 通道媒体属性
    EKDGetDataMode m_eGetDataMode;
    TKDElement*  m_ptParent;

    KDRESULT (*KD_PushData)(TKDPad* ptPad, IN const EKDDataPackFormat ePackFormat, IN void* pbyData);

    // pull接口???
    KDRESULT (*KD_PullData)(TKDPad* ptPad, IN TKDPos tPos, OUT EKDDataPackFormat* pePackFormat, OUT void* pbyData);

    // ??
    KDRESULT (*KD_GetPos)(TKDPad* ptPad, IN s32 nOffset, IN s32 nWhere, OUT TKDPos* ptPos);
}TKDPad;

// 设定本节点和数据目的节点的连通道参数和数据推拉模式
KDRESULT KD_LinkPad(TKDPad* ptSelfPad, TKDPad* ptPeerPad);
{
    ptSelfPad.ptPeerPad = ptPeerPad;
    ptPeerPad.ptPeerPad = ptSelfPad;
}

typedef enum tagPadType
{
    E_KD_PadType_Src = 1,
    E_KD_PadType_Sink = 2,
}EKDPadType;

typedef struct tagKDElement
{
    KDRESULT (*KD_GetPad)(IN EKDPadType ePadType, IN TKDTrackID tTrackID, OUT TKDPad* ptPad);
}TKDElement;
*/

/*====================================================================
函数名      :KD_UriEncode
功能        :根据给定的信息，构造URI
算法实现    :
引用全局变量:
输入参数说明:const TKDUri* ptinUri   ---  构造URI所需要的信息结构体，
             s8* pszoutUri           ---  构造好的URI

返回值说明  :0      -- 成功
             其它值 -- 失败
----------------------------------------------------------------------
修改记录    :
日  期       版本        修改人        修改内容
2016/03/04    1.0           林莉芬            创建
====================================================================*/
KDRESULT  KD_UriEncode(IN const TKDUri* ptinUri, OUT s8* pszoutUri);

/*====================================================================
函数名      :KD_UriDecode
功能        :解析URI
算法实现    :
引用全局变量:
输入参数说明:const s8* pszUri        ---  需要解析的URI，
             TKDUri *ptoutUri        ---  解析后的输出结果

返回值说明  :0      -- 成功
             其它值 -- 失败
----------------------------------------------------------------------
修改记录    :
日  期       版本        修改人        修改内容
2016/03/04    1.0           林莉芬            创建
====================================================================*/
KDRESULT  KD_UriDecode(IN const s8* pszUri, OUT TKDUri *ptoutUri);


#endif
