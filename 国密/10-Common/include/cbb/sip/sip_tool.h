#ifndef _SIP_TOOL_H_
#define _SIP_TOOL_H_
#include "kdvtype.h"

//国家标准GB/T 28181‐2011《安全防范视频监控联网系统信息传输交换控制技术要求》修改补充文件 2014
//a字段增加的两个属性
//倍速参数 用于文件下载时控制下载速度 取值为整形（a=downloadspeed:下载倍速）
#define GB28181_ATTIBUTE_NAME_DOWNLOAD_SPEED "downloadspeed"
//文件大小 用于文件下载时进度计算 单位byte  （a=filesize:文件大小）
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
/* PTZ命令结构体 */
typedef void*   PTGB_PTZCMD_STRUCT;
/* PTZ命令类型 */
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ      = 1,    /// PTZ操作
    GB_MANSCDP_PTZCMD_TYPE_FI,      /// 2   /// FI操作
    GB_MANSCDP_PTZCMD_TYPE_PRESENT, /// 3   /// 预置位操作
    GB_MANSCDP_PTZCMD_TYPE_CRUISE,  /// 4   /// 巡航操作
    GB_MANSCDP_PTZCMD_TYPE_SCAN,    /// 5   /// 扫描操作
        //                 GB_MANSCDP_PTZCMD_TYPE_STOP,    /// 6   /// 停止
}EGB_MANSCDP_PTZCMD_TYPE;

typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_PAN_LEFT = 1,    /// 左
        GB_MANSCDP_PTZCMD_TYPE_PTZ_PAN_RIGHT,   /// 2   /// 右
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_PAN;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_TILT_UP = 1,    /// 上
        GB_MANSCDP_PTZCMD_TYPE_PTZ_TILT_DOWN,   /// 2   /// 下
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_TILT;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM_IN   = 1,    /// 放大
        GB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM_OUT,    /// 2   /// 缩小
}EGB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_FI_FOCUS_NEAR    = 1,    /// 近
        GB_MANSCDP_PTZCMD_TYPE_FI_FOCUS_FAR,    /// 2   /// 远
}EGB_MANSCDP_PTZCMD_TYPE_FI_FOCUS;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_FI_IRIS_REDUCE   = 1,    /// 缩小
        GB_MANSCDP_PTZCMD_TYPE_FI_IRIS_IMPROVE, /// 2   /// 放大
}EGB_MANSCDP_PTZCMD_TYPE_FI_IRIS;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_PRESENT_SET      = 1,    /// 设置预置位
        GB_MANSCDP_PTZCMD_TYPE_PRESENT_GOTO,    /// 2   /// 调用预置位
        GB_MANSCDP_PTZCMD_TYPE_PRESENT_REMOVE,  /// 3   /// 删除预置位
}EGB_MANSCDP_PTZCMD_TYPE_PRESENT;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_CRUISE_SET       = 1,    /// 加入巡航点
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_REMOVE,   /// 2   /// 删除巡航点
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_SETSPEED, /// 3   /// 设置巡航速度
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_SETPAUSE, /// 4   /// 设置巡航停留时间
        GB_MANSCDP_PTZCMD_TYPE_CRUISE_START,    /// 5   /// 开始巡航
}EGB_MANSCDP_PTZCMD_TYPE_CRUISE;
typedef enum
{
    GB_MANSCDP_PTZCMD_TYPE_SCAN_START       = 1,    /// 开始自动扫描
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETLEFT,    /// 2   /// 设置自动扫描左边界
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETRIGHT,   /// 3   /// 设置自动扫描右边界
        GB_MANSCDP_PTZCMD_TYPE_SCAN_SETSPEED,   /// 4   /// 设置自动扫描速度
}EGB_MANSCDP_PTZCMD_TYPE_SCAN;


/* PTZ操作 空表示停止响应操作*/
typedef struct
{
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_PAN*    pePan;
    u8*                                 pbyPanSpeed;    /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_TILT*   peTilt;
    u8*                                 pbyTiltSpeed;   /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_PTZ_ZOOM*   peZoom;
    u8*                                 pbyZoomSpeed;   /// 0-15
}TGB_MANSCDP_PTZCMD_TYPE_PTZ;
/* FI操作 空表示停止响应操作*/
typedef struct
{
    EGB_MANSCDP_PTZCMD_TYPE_FI_FOCUS*   peFocus;
    u8*                                 pbyFocusSpeed;   /// 0-255
    EGB_MANSCDP_PTZCMD_TYPE_FI_IRIS*    peIris;
    u8*                                 pbyIrisSpeed;    /// 0-255
}TGB_MANSCDP_PTZCMD_TYPE_FI;
/* 预置位操作 */
typedef struct
{
    u8                              byPresentNo; /// 预置位号 1-255
    EGB_MANSCDP_PTZCMD_TYPE_PRESENT eAction;
}TGB_MANSCDP_PTZCMD_TYPE_PRESENT;
/* 巡航操作 */
typedef struct
{
    u8                              byPresentNo;    /// 预置位号 1-255
    u8                              byCruiseNo;     /// 巡航组号 1-255
    u16*                            pwArg;         /// 巡航速度或巡航停留时间（秒）0-4095
    EGB_MANSCDP_PTZCMD_TYPE_CRUISE  eAction;
}TGB_MANSCDP_PTZCMD_TYPE_CRUISE;
/* 扫描操作 */
typedef struct
{
    u8                              byScanNo;   /// 预置位号 1-255
    u16*                            pwArg;     /// 自动扫描速度 0-4095
    EGB_MANSCDP_PTZCMD_TYPE_SCAN    eAction;
}TGB_MANSCDP_PTZCMD_TYPE_SCAN;



/* PTZ命令 */
typedef struct
{
    u16                     wAddr;     /// 摄像头地址码
    EGB_MANSCDP_PTZCMD_TYPE eType;      /// PTZ命令类型
    PTGB_PTZCMD_STRUCT      ptStruct;   /// PTZ命令结构体
}TGB_MANSCDP_PTZCMD;

ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TGB_MANSCDP_PTZCMD(HTOOL_MEMLIST hMemList, LPCSTR szIn, TGB_MANSCDP_PTZCMD* pvOut);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TGB_MANSCDP_PTZCMD2S(TGB_MANSCDP_PTZCMD* pvIn, LPSTR szOut, s32* pnLen);


 /*-------------------------------------------------------------------------------*
  | GB28181 DATE                                                                  |
  *-------------------------------------------------------------------------------*/
typedef struct
{
    s32 nYear;      /// 年
    s32 nMonth;     /// 月
    s32 nDay;       /// 日
    s32 nHour;      /// 小时
    s32 nMinute;    /// 分钟
    s32 nSecond;    /// 秒
    s32 nMSecond;   /// 毫秒
}TGB_DATETIME;


ESIP_TOOL_RESULT SIP_TOOL_CONVERT_S2TDATETIME(LPCSTR szTime, TGB_DATETIME* ptDateTime);
ESIP_TOOL_RESULT SIP_TOOL_CONVERT_TDATETIME2S(TGB_DATETIME* ptIn, LPSTR szOut, s32* pnLen,BOOL32 bUseMillisecond);//True:转换出带毫秒，FALSE:转换后不带毫秒


 /*-------------------------------------------------------------------------------*
  | GB28181 SDP                                                                   |
  *-------------------------------------------------------------------------------*/
typedef enum
{
                GB_SDP_MEDIATYPE_VIDEO  = 1,    /// 视频
                GB_SDP_MEDIATYPE_AUDIO, /// 2   /// 音频
}EGB_SDP_MEDIATYPE;
typedef struct
{
                s32         nPayload;           /// payload
                LPSTR       szEncodingName;     /// 编码名称
                u32         dwClockRate;        /// 时钟频率
}TGB_SDP_RTPMAP;

typedef struct
{
	LPSTR                      szStreamVkekVersion;   //vkek的版本号
	LPSTR                      szStreamVkek;       //vkek
}TGB_SDP_VKEKINFO;

typedef struct
{
                EGB_SDP_MEDIATYPE   eMediaType;         /// 媒体类型
                s32                 nPort;              /// 端口
                LPSTR               szProtocal;       /// 协议  固定为RTP/AVP
                s32*                pnPayloadList;      /// payload列表
                s32                 nPayloadListSize;
                TGB_SDP_RTPMAP*     ptRTPMapList;       /// rtpmap列表
                s32                 nRTPMapListSize;
                TGB_SDP_VKEKINFO*   ptVkekInfoList; //码流中需要用到的vkek列表
				s32                 nVkekInfoListSize;//vkekinfo的数目
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
                GB_SDP_MEDIA_VIDEO_BITRATETYPE_CBR  = 1,    /// 固定码率
                GB_SDP_MEDIA_VIDEO_BITRATETYPE_VBR, /// 2   /// 可变码率
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
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G723_5_3Kbps   = 1,    /// 5.3 kbps （注：G.723.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G723_6_3Kbps,  /// 2   /// 6.3 kbps （注：G.723.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G729_8Kbps,    /// 3   /// 8 kbps （注：G.729中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_16Kbps,   /// 4   /// 16 kbps （注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_24Kbps,   /// 5   /// 24 kbps （注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_32Kbps,   /// 6   /// 32 kbps （注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G722_48Kbps,   /// 7   /// 48 kbps （注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_BITRATE_FOR_G711_64Kbps,   /// 8   /// 64 kbps （注：G.711中使用）
}EGB_SDP_MEDIA_AUDIO_BITRATE;

typedef enum
{
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_1_8kHz            = 1,    /// 1-8 kHz （注：G.711/ G.723.1/ G.729中使用）
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_2_14kHz, /// 2   /// 2-14 kHz （注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_3_16kHz, /// 3   /// 3-16 kHz（注：G.722.1中使用）
                GB_SDP_MEDIA_AUDIO_SAMPLERATE_FOR_G722_4_32kHz, /// 4   /// 4-32 kHz（注：G.722.1中使用）
}EGB_SDP_MEDIA_AUDIO_SAMPLERATE;

typedef struct
{
                EGB_SDP_MEDIA_VIDEO_ENCODETYPE*     peVideoEncodeType;  /// 视频编码类型
                EGB_SDP_MEDIA_VIDEO_RESOLUTION*     peVideoResolution;  /// 视频分辨率
                u32*                                pdwVideoFrameRate;  /// 视频帧率
                EGB_SDP_MEDIA_VIDEO_BITRATETYPE*    peVideoBitRateType; /// 视频码率类型
                u32*                                pdwVideoBitRate;    /// 视频码率 单位kbps
                EGB_SDP_MEDIA_AUDIO_ENCODETYPE*     peAudioEncodeType;  /// 音频编码类型
                EGB_SDP_MEDIA_AUDIO_BITRATE*        peAudioBitRate;     /// 音频码率
                EGB_SDP_MEDIA_AUDIO_SAMPLERATE*     peAudioSampleRate;  /// 音频采样率
}TGB_SDP_MEDIAINFO;

typedef struct
{
    LPSTR szIP;
    u16   wPort;
}TGB_SIP_SDP_ADDR;

//暂不使用
//typedef enum
//{
//	SDPCONNECTMODE_NOTSET    = 0,
//	SDPCONNECTMODE_SENDONLY = 145,       /* "sendonly",据radvison层设置的数值 */
//	SDPCONNECTMODE_RECVONLY,             /* "recvonly" */
//	SDPCONNECTMODE_SENDRECV             /* "sendrecv" */
//}ESDPConnectionMode;

typedef struct
{
                LPSTR                      szUsername_o;   /// 媒体流接收者设备编码
                LPSTR                      szSession_s;     /// 会话类型
                u32                        dwStartTime;    /// 开始时间
                u32                        dwEndTime;      /// 结束时间
                LPSTR                      szAddress_c;    ///
                TGB_SDP_MEDIA*             ptMediaList_m;
                s32                        nMediaListSize;
                s32                        nSSRC;         /// RTP所需的SSRC
                BOOL32                     bSendOnly;
                LPSTR                      szUsername;     /// RTP验证用用户名
                LPSTR                      szPassword;     /// RTP验证用密码
				LPSTR                      szSetup;         //扩展字段
				LPSTR                      szConnection;    //扩展字段
                /* u行应填写视音频文件的URI。该URI取值有两种方式：简捷方式和普通方式。
                简捷方式直接采用产生该历史媒体的媒体源（如某个摄像头）的设备ID（应符合6.1.2的规定）以及相关参数，参数用"："分隔；
                    普通方式采用http://存储设备ID[/文件夹]* /文件名，[/文件夹]*为0－N级文件夹。*/
                LPSTR                      szURI_u;
                TGB_SDP_MEDIAINFO*         ptMediaInfo;
                // kdm ext 多地址
                s32                         nAddrListSize;
                TGB_SIP_SDP_ADDR*           ptAddrList;
}TGB_SDP;


typedef struct tagKdSdpKeyArg 
{
	LPCSTR szKey;  
	s32    nKeyType;//算法类型（kdssl-ext_api.h里面EKeyType这个枚举里面的值）
	s32    nKeyDataType;//算法数据类型,(kdssl-ext_api.h里面EDataType这个枚举里面的值)
	LPCSTR szPassword;//私钥据说有密码，密码不对不会去验证
	u32 dwEngineId;//硬件加密的设备id，软件加密填0
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
/* MANSRTSP方法枚举 */
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
    是通常播放时间，表示相对播放开始的时间（即媒体建链时设定的起始时间），播放开始记为0. NPT所有的值均为正数。
    例 媒体建链约定 从13:00:00播放到15:00:00
    npt=1:05:35.3-  从14:05:35.3开始播放，一直到结束(15:00:00)
    npt=now-14:50:00  从当前进度播放，一直到14:50:00
    npt=-14:50:00  从开始时间(13:00:00)播放，一直到14:50:00

    SMPTE
    是相对于媒体文件(clip)开始的相对时间，可以精确到帧。
    SMPTE默认的格式是 "SMPTE 30 DROP",即 29.97帧/秒，
    在精确到秒的情况下，可以将SMPTE与NPT等同处理。
    例 媒体建链约定 播放 A文件
    smpte=1:07:33:-  从A文件的1:07:33开始播放，一直到文件结束
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
    BOOL32  bIsNow;                 // 是否是现在
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