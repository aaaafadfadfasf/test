#ifndef _SIP_TOOL_H_
#define _SIP_TOOL_H_
#include "kdvtype.h"

//���ұ�׼GB/T 28181�\2011����ȫ������Ƶ�������ϵͳ��Ϣ���佻�����Ƽ���Ҫ���޸Ĳ����ļ� 2014
//a�ֶ����ӵ���������
//���ٲ��� �����ļ�����ʱ���������ٶ� ȡֵΪ���Σ�a=downloadspeed:���ر��٣�
#define GB28181_ATTIBUTE_NAME_DOWNLOAD_SPEED "downloadspeed"
//�ļ���С �����ļ�����ʱ���ȼ��� ��λbyte  ��a=filesize:�ļ���С��
#define GB28181_ATTIBUTE_NAME_FILE_SIZE      "filesize"

typedef void* HTOOL_MEMLIST;
typedef enum
{
    SIP_TOOL_OK           = 0,
    SIP_TOOL_INVALID_ARG,
    SIP_TOOL_NO_MEMORY,
    SIP_TOOL_BUF_TOO_SMALL,
    SIP_TOOL_FAIL,
}ESIP_TOOL_RESULT;

HTOOL_MEMLIST SIP_TOOL_CreateMemList();
void SIP_TOOL_FreeMemList(HTOOL_MEMLIST hMemList);

 /*-------------------------------------------------------------------------------*
  | GB28181 PTZ                                                                   |
  *-------------------------------------------------------------------------------*/
/* PTZ����ṹ�� */
typedef void*   PTGB_PTZCMD_STRUCT;
/* PTZ�������� */
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ      = 1,    /// PTZ����
    GB_MANSCDP_PTZCMD_TYPE_FI,      /// 2   /// FI����
    GB_MANSCDP_PTZCMD_TYPE_PRESENT, /// 3   /// Ԥ��λ����
    GB_MANSCDP_PTZCMD_TYPE_CRUISE,  /// 4   /// Ѳ������
    GB_MANSCDP_PTZCMD_TYPE_SCAN,    /// 5   /// ɨ�����
        //                 GB_MANSCDP_PTZCMD_TYPE_STOP,    /// 6   /// ֹͣ
}EGB_MANSCDP_PTZCMD_TYPE;

typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_PAN_LEFT = 1,    /// ��
        GB_MANSCDP_PTZCMD_TYPE_PTZ_PAN_RIGHT,   /// 2   /// ��
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_PAN;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_TILT_UP = 1,    /// ��
        GB_MANSCDP_PTZCMD_TYPE_PTZ_TILT_DOWN,   /// 2   /// ��
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_TILT;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM_IN   = 1,    /// �Ŵ�
        GB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM_OUT,    /// 2   /// ��С
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_FI_FOCUS_NEAR    = 1,    /// ��
        GB_MANSCDP_PTZCMD_TYPE_FI_FOCUS_FAR,    /// 2   /// Զ
}EGB_MANSCDP_PTZCMD_TYPE_FI_FOCUS;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_FI_IRIS_REDUCE   = 1,    /// ��С
        GB_MANSCDP_PTZCMD_TYPE_FI_IRIS_IMPROVE, /// 2   /// �Ŵ�
}EGB_MANSCDP_PTZCMD_TYPE_FI_IRIS;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PRESENT_SET      = 1,    /// ����Ԥ��λ
        GB_MANSCDP_PTZCMD_TYPE_PRESENT_GOTO,    /// 2   /// ����Ԥ��λ
        GB_MANSCDP_PTZCMD_TYPE_PRESENT_REMOVE,  /// 3   /// ɾ��Ԥ��λ
}EGB_MANSCDP_PTZCMD_TYPE_PRESENT;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_CRUISE_SET       = 1,    /// ����Ѳ����
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_REMOVE,   /// 2   /// ɾ��Ѳ����
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_SETSPEED, /// 3   /// ����Ѳ���ٶ�
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_SETPAUSE, /// 4   /// ����Ѳ��ͣ��ʱ��
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_START,    /// 5   /// ��ʼѲ��
}EGB_MANSCDP_PTZCMD_TYPE_CRUISE;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_SCAN_START       = 1,    /// ��ʼ�Զ�ɨ��
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETLEFT,    /// 2   /// �����Զ�ɨ����߽�
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETRIGHT,   /// 3   /// �����Զ�ɨ���ұ߽�
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETSPEED,   /// 4   /// �����Զ�ɨ���ٶ�
}EGB_MANSCDP_PTZCMD_TYPE_SCAN;


/* PTZ���� �ձ�ʾֹͣ��Ӧ����*/
typedef struct
{
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_PAN*    pePan;
    u8*                                 pbyPanSpeed;    /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_TILT*   peTilt;
    u8*                                 pbyTiltSpeed;   /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM*   peZoom;
    u8*                                 pbyZoomSpeed;   /// 0-15
}TGB_MANSCDP_PTZCMD_TYPE_PTZ;
/* FI���� �ձ�ʾֹͣ��Ӧ����*/
typedef struct
{
    EGB_MANSCDP_PTZCMD_TYPE_FI_FOCUS*   peFocus;
    u8*                                 pbyFocusSpeed;   /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_FI_IRIS*    peIris;
    u8*                                 pbyIrisSpeed;    /// 0-255
}TGB_MANSCDP_PTZCMD_TYPE_FI;
/* Ԥ��λ���� */
typedef struct
{
    u8                              byPresentNo; /// Ԥ��λ�� 1-255
    EGB_MANSCDP_PTZCMD_TYPE_PRESENT eAction;
}TGB_MANSCDP_PTZCMD_TYPE_PRESENT;
/* Ѳ������ */
typedef struct
{
    u8                              byPresentNo;    /// Ԥ��λ�� 1-255
    u8                              byCruiseNo;     /// Ѳ����� 1-255
    u16*                            pwArg;         /// Ѳ���ٶȻ�Ѳ��ͣ��ʱ�䣨�룩0-4095
    EGB_MANSCDP_PTZCMD_TYPE_CRUISE  eAction;
}TGB_MANSCDP_PTZCMD_TYPE_CRUISE;
/* ɨ����� */
typedef struct
{
    u8                              byScanNo;   /// Ԥ��λ�� 1-255
    u16*                            pwArg;     /// �Զ�ɨ���ٶ� 0-4095
    EGB_MANSCDP_PTZCMD_TYPE_SCAN    eAction;
}TGB_MANSCDP_PTZCMD_TYPE_SCAN;



/* PTZ���� */
typedef struct
{
    u16                     wAddr;     /// ����ͷ��ַ��
    EGB_MANSCDP_PTZCMD_TYPE eType;      /// PTZ��������
    PTGB_PTZCMD_STRUCT      ptStruct;   /// PTZ����ṹ��
}TGB_MANSCDP_PTZCMD;

ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TGB_MANSCDP_PTZCMD(HTOOL_MEMLIST hMemList, LPCSTR szIn, TGB_MANSCDP_PTZCMD* pvOut);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TGB_MANSCDP_PTZCMD2S(TGB_MANSCDP_PTZCMD* pvIn, LPSTR szOut, s32* pnLen);


 /*-------------------------------------------------------------------------------*
  | GB28181 DATE                                                                  |
  *-------------------------------------------------------------------------------*/
typedef struct
{
    s32 nYear;      /// ��
    s32 nMonth;     /// ��
    s32 nDay;       /// ��
    s32 nHour;      /// Сʱ
    s32 nMinute;    /// ����
    s32 nSecond;    /// ��
    s32 nMSecond;   /// ����
}TGB_DATETIME;


ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TDATETIME(LPCSTR szTime, TGB_DATETIME* ptDateTime);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TDATETIME2S(TGB_DATETIME* ptIn, LPSTR szOut, s32* pnLen,BOOL32 bUseMillisecond);//True:ת���������룬FALSE:ת���󲻴�����


 /*-------------------------------------------------------------------------------*
  | GB28181 SDP                                                                   |
  *-------------------------------------------------------------------------------*/
typedef enum
{
                GB_SDP_MEDIATYPE_VIDEO  = 1,    /// ��Ƶ
                GB_SDP_MEDIATYPE_AUDIO, /// 2   /// ��Ƶ
}EGB_SDP_MEDIATYPE;
typedef struct
{
                s32         nPayload;           /// payload
                LPSTR       szEncodingName;     /// ��������
                u32         dwClockRate;        /// ʱ��Ƶ��
}TGB_SDP_RTPMAP;

typedef struct
{
	LPSTR                      szStreamVkekVersion;   //vkek�İ汾��
	LPSTR                      szStreamVkek;       //vkek
}TGB_SDP_VKEKINFO;

typedef struct
{
                EGB_SDP_MEDIATYPE   eMediaType;         /// ý������
                s32                 nPort;              /// �˿�
                LPSTR               szProtocal;       /// Э��  �̶�ΪRTP/AVP
                s32*                pnPayloadList;      /// payload�б�
                s32                 nPayloadListSize;
                TGB_SDP_RTPMAP*     ptRTPMapList;       /// rtpmap�б�
                s32                 nRTPMapListSize;
                TGB_SDP_VKEKINFO*   ptVkekInfoList; //��������Ҫ�õ���vkek�б�
				s32                 nVkekInfoListSize;//vkekinfo����Ŀ
                u32                 dwDownloadSpeed;
                u32                 dwFileSize;
}TGB_SDP_MEDIA;

typedef enum
{
                GB_SDP_MEDIA_VIDEO_ENCODETYPE_MPEG4 = 1,
                GB_SDP_MEDIA_VIDEO_ENCODETYPE_H264, /// 2
                GB_SDP_MEDIA_VIDEO_ENCODETYPE_SVAC, /// 3
                GB_SDP_MEDIA_VIDEO_ENCODETYPE_3GP,  /// 4
}EGB_SDP_MEDIA_VIDEO_ENCODETYPE;
typedef enum
{
                GB_SDP_MEDIA_VIDEO_RESOLUTION_QCIF  = 1,
                GB_SDP_MEDIA_VIDEO_RESOLUTION_CIF,  /// 2
                GB_SDP_MEDIA_VIDEO_RESOLUTION_4CIF, /// 3
                GB_SDP_MEDIA_VIDEO_RESOLUTION_D1,   /// 4
                GB_SDP_MEDIA_VIDEO_RESOLUTION_720P, /// 5
                GB_SDP_MEDIA_VIDEO_RESOLUTION_1080P,/// 6
}EGB_SDP_MEDIA_VIDEO_RESOLUTION;
typedef enum
{
                GB_SDP_MEDIA_VIDEO_BITRATETYPE_CBR  = 1,    /// �̶�����
                GB_SDP_MEDIA_VIDEO_BITRATETYPE_VBR, /// 2   /// �ɱ�����
}EGB_SDP_MEDIA_VIDEO_BITRATETYPE;
typedef enum
{
                GB_SDP_MEDIA_ADUIO_ENCODETYPE_G711      = 1,    /// G.711
                GB_SDP_MEDIA_ADUIO_ENCODETYPE_G723,     /// 2   /// G.723.1
                GB_SDP_MEDIA_ADUIO_ENCODETYPE_G729,     /// 3   /// G.729
                GB_SDP_MEDIA_ADUIO_ENCODETYPE_G722,     /// 4   /// G.722.1
}EGB_SDP_MEDIA_AUDIO_ENCODETYPE;

typedef enum
{
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G723_5_3Kbps   = 1,    /// 5.3 kbps ��ע��G.723.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G723_6_3Kbps,  /// 2   /// 6.3 kbps ��ע��G.723.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G729_8Kbps,    /// 3   /// 8 kbps ��ע��G.729��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_16Kbps,   /// 4   /// 16 kbps ��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_24Kbps,   /// 5   /// 24 kbps ��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_32Kbps,   /// 6   /// 32 kbps ��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_48Kbps,   /// 7   /// 48 kbps ��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G711_64Kbps,   /// 8   /// 64 kbps ��ע��G.711��ʹ�ã�
}EGB_SDP_MEDIA_AUDIO_BITRATE;

typedef enum
{
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_1_8kHz            = 1,    /// 1-8 kHz ��ע��G.711/ G.723.1/ G.729��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_2_14kHz, /// 2   /// 2-14 kHz ��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_3_16kHz, /// 3   /// 3-16 kHz��ע��G.722.1��ʹ�ã�
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_4_32kHz, /// 4   /// 4-32 kHz��ע��G.722.1��ʹ�ã�
}EGB_SDP_MEDIA_AUDIO_SAMPLERATE;

typedef struct
{
                EGB_SDP_MEDIA_VIDEO_ENCODETYPE*     peVideoEncodeType;  /// ��Ƶ��������
                EGB_SDP_MEDIA_VIDEO_RESOLUTION*     peVideoResolution;  /// ��Ƶ�ֱ���
                u32*                                pdwVideoFrameRate;  /// ��Ƶ֡��
                EGB_SDP_MEDIA_VIDEO_BITRATETYPE*    peVideoBitRateType; /// ��Ƶ��������
                u32*                                pdwVideoBitRate;    /// ��Ƶ���� ��λkbps
                EGB_SDP_MEDIA_AUDIO_ENCODETYPE*     peAudioEncodeType;  /// ��Ƶ��������
                EGB_SDP_MEDIA_AUDIO_BITRATE*        peAudioBitRate;     /// ��Ƶ����
                EGB_SDP_MEDIA_AUDIO_SAMPLERATE*     peAudioSampleRate;  /// ��Ƶ������
}TGB_SDP_MEDIAINFO;

typedef struct
{
    LPSTR szIP;
    u16   wPort;
}TGB_SIP_SDP_ADDR;

//�ݲ�ʹ��
//typedef enum
//{
//	SDPCONNECTMODE_NOTSET    = 0,
//	SDPCONNECTMODE_SENDONLY = 145,       /* "sendonly",��radvison�����õ���ֵ */
//	SDPCONNECTMODE_RECVONLY,             /* "recvonly" */
//	SDPCONNECTMODE_SENDRECV             /* "sendrecv" */
//}ESDPConnectionMode;

typedef struct
{
                LPSTR                      szUsername_o;   /// ý�����������豸����
                LPSTR                      szSession_s;     /// �Ự����
                u32                        dwStartTime;    /// ��ʼʱ��
                u32                        dwEndTime;      /// ����ʱ��
                LPSTR                      szAddress_c;    ///
                TGB_SDP_MEDIA*             ptMediaList_m;
                s32                        nMediaListSize;
                s32                        nSSRC;         /// RTP�����SSRC
                BOOL32                     bSendOnly;
                LPSTR                      szUsername;     /// RTP��֤���û���
                LPSTR                      szPassword;     /// RTP��֤������
				LPSTR                      szSetup;         //��չ�ֶ�
				LPSTR                      szConnection;    //��չ�ֶ�
                /* u��Ӧ��д����Ƶ�ļ���URI����URIȡֵ�����ַ�ʽ����ݷ�ʽ����ͨ��ʽ��
                ��ݷ�ʽֱ�Ӳ��ò�������ʷý���ý��Դ����ĳ������ͷ�����豸ID��Ӧ����6.1.2�Ĺ涨���Լ���ز�����������"��"�ָ���
                    ��ͨ��ʽ����http://�洢�豸ID[/�ļ���]* /�ļ�����[/�ļ���]*Ϊ0��N���ļ��С�*/
                LPSTR                      szURI_u;
                TGB_SDP_MEDIAINFO*         ptMediaInfo;
                // kdm ext ���ַ
                s32                         nAddrListSize;
                TGB_SIP_SDP_ADDR*           ptAddrList;
}TGB_SDP;


typedef struct tagKdSdpKeyArg 
{
	LPCSTR szKey;  
	s32    nKeyType;//�㷨���ͣ�kdssl-ext_api.h����EKeyType���ö�������ֵ��
	s32    nKeyDataType;//�㷨��������,(kdssl-ext_api.h����EDataType���ö�������ֵ)
	LPCSTR szPassword;//˽Կ��˵�����룬���벻�Բ���ȥ��֤
	u32 dwEngineId;//Ӳ�����ܵ��豸id�����������0
}TKdSdpKeyArg;

ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TGB_SDP(HTOOL_MEMLIST hMemList, LPCSTR szIn,TGB_SDP* ptOut,LPCSTR szAlgorithm=NULL,TKdSdpKeyArg* ptKeyArg=NULL);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TGB_SDP2S(TGB_SDP* ptIn,LPSTR szOut, s32* pnLen, LPCSTR szAlgorithm=NULL,TKdSdpKeyArg* ptKeyArg=NULL);

typedef struct
{
    LPSTR szMediaSender;
    LPSTR szMediaSenderSN;
    LPSTR szMediaReceiver;
    LPSTR szMediaReceiverSN;
}TGB_SUBJECT;
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TGB_SUBJECT(HTOOL_MEMLIST hMemList, LPCSTR szIn, TGB_SUBJECT* ptOut);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TGB_SUBJECT2S(TGB_SUBJECT* ptIn,  LPSTR szOut, s32* pnLen);

 /*-------------------------------------------------------------------------------*
  | MANRTSP                                                                       |
  *-------------------------------------------------------------------------------*/
/* MANSRTSP����ö�� */
typedef enum
{
    GB_MANSRTSP_METHOD_NONE         = 0,
    GB_MANSRTSP_METHOD_UNKNOWN      = 12000,
    GB_MANSRTSP_METHOD_PLAY,        /// 12001
    GB_MANSRTSP_METHOD_PAUSE,       /// 12002
    GB_MANSRTSP_METHOD_TEARDOWN,    /// 12003
        //GB_MANSRTSP_METHOD_RESPONSE,    /// 12004
}EGB_MANSRTSP_METHOD;


/*
    NPT
    ��ͨ������ʱ�䣬��ʾ��Բ��ſ�ʼ��ʱ�䣨��ý�彨��ʱ�趨����ʼʱ�䣩�����ſ�ʼ��Ϊ0. NPT���е�ֵ��Ϊ������
    �� ý�彨��Լ�� ��13:00:00���ŵ�15:00:00
    npt=1:05:35.3-  ��14:05:35.3��ʼ���ţ�һֱ������(15:00:00)
    npt=now-14:50:00  �ӵ�ǰ���Ȳ��ţ�һֱ��14:50:00
    npt=-14:50:00  �ӿ�ʼʱ��(13:00:00)���ţ�һֱ��14:50:00

    SMPTE
    �������ý���ļ�(clip)��ʼ�����ʱ�䣬���Ծ�ȷ��֡��
    SMPTEĬ�ϵĸ�ʽ�� "SMPTE 30 DROP",�� 29.97֡/�룬
    �ھ�ȷ���������£����Խ�SMPTE��NPT��ͬ����
    �� ý�彨��Լ�� ���� A�ļ�
    smpte=1:07:33:-  ��A�ļ���1:07:33��ʼ���ţ�һֱ���ļ�����
 */

typedef enum
{
    GB_MANSRTSP_RANGETYPE_NONE      = 0,
        GB_MANSRTSP_RANGETYPE_NTP,      /// 1
        GB_MANSRTSP_RANGETYPE_SMPTE,    /// 2
}EGB_MANSRTSP_RANGETYPE;


typedef void* PTGB_MANSRTSP_RANGE;
typedef struct
{
    s32     nHour;
    s32     nMinute;
    s32     nSecond;
    s32     nMillionSecond;
    BOOL32  bIsNow;                 // �Ƿ�������
}TGB_MANSRTSP_NPT_RANGE;
typedef struct
{
    s32     nHour;
    s32     nMinute;
    s32     nSecond;
    s32     nFrames;
    s32     nSubFrame;
}TGB_MANSRTSP_SMPTE_RANGE;


/* 12001 GB_MANSRTSP_METHOD_PLAY */
typedef struct
{
    s32                     nCSeq;
    EGB_MANSRTSP_RANGETYPE  eRangeType;
    PTGB_MANSRTSP_RANGE     ptStartRange;
    PTGB_MANSRTSP_RANGE     ptEndRange;
    double*                 pdScale;
}TGB_MANSRTSP_METHOD_PLAY;

/* 12002 GB_MANSRTSP_METHOD_PAUSE */
typedef struct
{
    s32             nCSeq;
    s32             nPauseTime;
}TGB_MANSRTSP_METHOD_PAUSE;

/* 12003 GB_MANSRTSP_METHOD_TEARDOWN */
typedef struct
{
    s32             nCSeq;
}TGB_MANSRTSP_METHOD_TEARDOWN;



ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TMANSRTSP(HTOOL_MEMLIST hMemList, LPCSTR szIn, EGB_MANSRTSP_METHOD* peMethod, void** ppvOut);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TMANSRTSP2S(EGB_MANSRTSP_METHOD eMethod, void* ptIn, LPSTR szOut, s32* pnLen);







 /*-------------------------------------------------------------------------------*
  | MANSCDP                                                                       |
  *-------------------------------------------------------------------------------*/
// ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TMANSCDP(HTOOL_MEMLIST hMemList, LPCSTR szBody, EGB_MANSCDP_METHOD* peMethod, void** ppvOut);
// ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TMANSCDP(EGB_MANSCDP_METHOD eMethod, void* ptIn, EGB_MANSCDP_METHOD* peMethod, void** ppvOut);

#endif //_SIP_GB_TOOL_H_