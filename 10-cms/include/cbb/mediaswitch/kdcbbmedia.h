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

//ý�����Ե�key����
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

//֡���ݴ�С����
#define KD_MAX_FRAM_SIZE    (512*1024)

//NTP UTC ʱ��ת��
#define  CBB_JAN_1970   (2208988800UL)    /*(3600 * 24 *(365 * 70 + 17))*/
#define  CBB_USEC(x) (((u64)x*1000000) >> 32)
#define  CBB_NTPFRAC(x) ((((u64)x)<<32)/1000000)



#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef WIN32 //vc6 ��֧��inline
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
/* ö�ٶ���                                                             */
/************************************************************************/

//ý��������
typedef enum tagKDTrackType
{
    E_KD_TrackType_Video = 1,
    E_KD_TrackType_Audio,
    E_KD_TrackType_MetaData,
    E_KD_TrackType_Unknown,
}EKDTrackType;

//���ݷ�װ��ʽ
typedef enum tagKDDataPackFormat
{
    E_KD_DataPackFormat_Pack  = 1,             /*������ԴΪRTP���ݰ�TKDPack*/
    E_KD_DataPackFormat_Frame =2,              /*������ԴΪ֡����TKDFrame*/
	E_KD_DataPackForamt_Raw	  = 3,			   /*������ԴΪδ֪�������ͣ���������������*/
}EKDDataPackFormat;

// ���ݴ�������ģʽ
typedef enum tagKDGetDataMode
{
    E_KD_GET_DATA_MODE_PUSH,            /* ����Դ��������*/
    E_KD_GET_DATA_MODE_PULL,            /*����Ŀ������������*/
}EKDGetDataMode;

/************************************************************************/
/*          �ṹ�嶨��                                                  */
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
    u8     m_byMediaType;            /*ý������*/
    u8    *m_pData;                /*֡���ݻ���*/
    u32    m_dwPreBufSize;        /*m_pData����ǰԤ���˶��ٿռ䣬���ڼ�*/
                                /* RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12*/
                                /* (FIXED HEADER + Extence option + Extence bit)*/
    u32    m_dwDataSize;            /*m_pDataָ���ʵ�ʻ����С�����С*/
    u8     m_byFrameRate;            /*����֡��,���ڽ��ն�*/
    u8     m_byPadSize;            /*������ݳ���*/
    u32    m_dwFrameID;            /*֡��ʶ�����ڽ��ն�*/
    u32    m_dwTimeStamp;            /*ʱ���, ���ڽ��ն�*/
    u32    m_dwSSRC;                /*ͬ��Դ, ���ڽ��ն�*/
    u64    m_llNTPTime;            /*NTPʱ���*/
    union
    {
        struct
        {
            BOOL32    m_bKeyFrame;    /*Ƶ֡���ͣ�I or P��*/
            u16       m_wVideoWidth;  /*��Ƶ֡��*/
            u16       m_wVideoHeight; /*��Ƶ֡��*/
        }m_tVideoParam;
        struct
        {
            u32       m_dwSample;        /*��Ƶ������*/
            u16       m_wChannel;        /*������*/
            u16       m_wBitsPerSample;    /*λ��*/
        }m_tAudioParam;
    }x;
	u8   m_byStreamID;//PES���������Ƶ 0-��Ƶ��0 1-��Ƶ��1
	//u32    m_dwRtpTimeStamp;          /*ʱ��������յ�RTPЭ����Я����ʱ���*/
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
	u16    m_wPackNum;           //�˰����ڵ�֡��Ӧ��RTP�����ܸ�����
	u64    m_qwSysTime;          //UTCʱ�䣬RTP����NVR�󣬽���ģ���ȡ����ϵͳʱ�䣻��λMS
    union
    {
        struct
        {
            BOOL32    m_bKeyFrame;    /*Ƶ֡���ͣ�I or P��*/
            u16       m_wVideoWidth;  /*��Ƶ֡��*/
            u16       m_wVideoHeight; /*��Ƶ֡��*/
        }m_tVideoParam;
        struct
        {
            u32       m_dwSample;           /*��Ƶ������*/
            u16       m_wChannel;           /*������*/
            u16       m_wBitsPerSample;     /*λ��*/
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
��������������������������������������������������������������������������������������������������������������������������������
E_KD_TrackType_Video        |        0             |    ��һ·��Ƶ��
��������������������������������������������������������������������������������������������������������������������������������
E_KD_TrackType_Video        |        1            |    �ڶ�·��Ƶ��
��������������������������������������������������������������������������������������������������������������������������������
E_KD_TrackType_Audio        |        0            |    ��һ·��Ƶ��
��������������������������������������������������������������������������������������������������������������������������������
E_KD_TrackType_MetaData    |        0            |    ��һ·Ԫ������
��������������������������������������������������������������������������������������������������������������������������������
*/
typedef struct tagRPTrackID
{
    EKDTrackType m_eTrackType;
    u32            m_dwTrackIndx;      /*��Բ�ͬ��TrackType,������ֵ����0��ʼ*/
}TKDTrackID;

/*
typedef struct tagKDPad
{
    TKDPad* ptPeerPad;         // ͨ�����
    TKDTrackID tTrackID;        // ͨ��ý������
    EKDGetDataMode m_eGetDataMode;
    TKDElement*  m_ptParent;

    KDRESULT (*KD_PushData)(TKDPad* ptPad, IN const EKDDataPackFormat ePackFormat, IN void* pbyData);

    // pull�ӿ�???
    KDRESULT (*KD_PullData)(TKDPad* ptPad, IN TKDPos tPos, OUT EKDDataPackFormat* pePackFormat, OUT void* pbyData);

    // ??
    KDRESULT (*KD_GetPos)(TKDPad* ptPad, IN s32 nOffset, IN s32 nWhere, OUT TKDPos* ptPos);
}TKDPad;

// �趨���ڵ������Ŀ�Ľڵ����ͨ����������������ģʽ
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
������      :KD_UriEncode
����        :���ݸ�������Ϣ������URI
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:const TKDUri* ptinUri   ---  ����URI����Ҫ����Ϣ�ṹ�壬
             s8* pszoutUri           ---  ����õ�URI

����ֵ˵��  :0      -- �ɹ�
             ����ֵ -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��       �汾        �޸���        �޸�����
2016/03/04    1.0           �����            ����
====================================================================*/
KDRESULT  KD_UriEncode(IN const TKDUri* ptinUri, OUT s8* pszoutUri);

/*====================================================================
������      :KD_UriDecode
����        :����URI
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:const s8* pszUri        ---  ��Ҫ������URI��
             TKDUri *ptoutUri        ---  �������������

����ֵ˵��  :0      -- �ɹ�
             ����ֵ -- ʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��       �汾        �޸���        �޸�����
2016/03/04    1.0           �����            ����
====================================================================*/
KDRESULT  KD_UriDecode(IN const s8* pszUri, OUT TKDUri *ptoutUri);


#endif
