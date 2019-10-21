/** 
* @file         coedclib.h 
* @brief        mediasdk 
* @details  CODEC上层API封装函数 
* @author       胡昌威 魏治兵 
* @date     date 
* @version  A001 
* @par Copyright (c):  
*      kedacom 
* @par History:          
*   version: author, date, desc\n 
*/ 

#ifndef __CODECLIB_H_
#define __CODECLIB_H_

#include "KdvDef.h"
#include "codeccommon.h"
#include "kdvmedianet.h"
#include "kdmfileinterface.h"

/*=======================================常量、宏与结构的定义=================================*/

//模块版本信息和编译时间 及 依赖的库的版本和编译时间
API void kdvencoderver();
API void kdvdecoderver();

//模块帮助信息 及 依赖的库的帮助信息
API void kdvencoderhelp();
API void kdvdecoderhelp();

enum 
{
	FILE_PLAY_STOP = 0,///<文件播放状态——停止
	FILE_PLAY_START,   ///<文件播放状态——开始
	FILE_PLAY_PAUSE    ///<文件播放状态——暂停
}  ;


enum 
{
	FILE_TYPE_ASF = 0,///<文件类型（asfm,mp4文件统一用这个）
	FILE_TYPE_KSJ     ///<文件类型（ksj）
} ;

enum
{
	DECODER_TYPE_NET = 0 ,///<网络解码
	DECODER_TYPE_FILE     ///<文件解码
		
} ;


/**编码比特率(Kbps)*/
#define  BIT_RATE_ZERO           (u16)0 

/**I帧间最大P帧数目*/
#define  MAX_P_FARME_INTERNAL_I  (u8)75

/*量化参数(1-31)*/
#define  MAX_QUANT_PARAM         (u8)31   ///<最大量化参数
#define  MIN_QUANT_PARAM         (u8)1    ///<最小量化参数
#define  QUANT_PARAM_31          (u8)31   ///<最大量化参数
#define  QUANT_PARAM_1           (u8)1    ///<最小量化参数

/*图像大小*/
#define  VIDEO_WIDTH_720         (u16)720  ///<图像高度
#define  VIDEO_HEIGHT_576        (u16)576  ///<图像宽度
#define  VIDEO_WIDTH_352         (u16)352  ///<图像高度
#define  VIDEO_HEIGHT_288        (u16)288  ///<图像宽度
#define  VIDEO_WIDTH_DEFAULT     (u16)640  ///<图像默认高度
#define  VIDEO_HEIGHT_DEFAULT    (u16)480  ///<图像默认宽度

/*帧率(default:25)*/
#define  FRAME_RATE_ONE          (u8)1    ///<默认帧率
#define  FRAME_RATE_DEFAULT      (u8)25   ///<默认帧率

/*图象压缩质量(1-5)*/
#define  IMAGE_ENCODE_QUALITY_TWO  (u8)2   ///<压缩质量2
#define  IMAGE_ENCODE_QUALITY_FOUR (u8)4   ///<压缩质量4     

/*声音模式 mp3*/
#define	 AUDIO_MODE_WORST        (u8)0///<最差
#define	 AUDIO_MODE_BAD          (u8)1///<差 
#define	 AUDIO_MODE_NORMAL       (u8)2///<一般
#define	 AUDIO_MODE_FINE         (u8)3///<好 
#define	 AUDIO_MODE_BEST         (u8)4///<最好

//G.711
#define  AUDIO_MODE_PCMA         (u8)5///<a law
#define  AUDIO_MODE_PCMU         (u8)6///<u law 
//G.723
#define AUDIO_MODE_G723_6        (u8)7///<G723_6 
#define AUDIO_MODE_G723_5        (u8)8///<G723_5
//G.728
#define AUDIO_MODE_G728		 (u8)9 ///<G728
//G.722
#define AUDIO_MODE_G722		 (u8)10 ///<G722
//G.729
#define AUDIO_MODE_G729		 (u8)11 ///<G729
//G.7221
#define AUDIO_MODE_G7221		 (u8)12 ///<G7221
//adpcm
#define AUDIO_MODE_ADPCM		 (u8)13 ///<ADPCM

#define  MAX_LOCAL_IPADDR_LEN    (u8)30 ///<最大的ip地址长度

#ifndef  MAX_NETSND_DEST_NUM
#define  MAX_NETSND_DEST_NUM     (u8)5  ///<最大发送的目的地数
#endif

#define  CAP_AUDIO_ONLY          (u8)0x01 ///<音频            数据捕获
#define  CAP_CAMERA_ONLY         (u8)0x02 ///<      CAMERA视频数据捕获
#define  CAP_AC_BOTH             (u8)0x03 ///<音频＋CAMERA视频数据均进行捕获
#define  CAP_DESKTOP_ONLY        (u8)0x04 ///<	    DESKTOP桌面视频数据捕获
#define  CAP_AD_BOTH             (u8)0x05 ///<音频＋DESKTOP桌面视频数据均进行捕获
#define  CAP_FILE_AV_BOTH		 (u8)0x08 ///<以指定文件（目前支持kdc录像机产生的asf文件）的音频＋视频压缩流作为捕获源

#define  CTRL_AUDIO_ONLY          (u8)0x01 ///<音频(包括采集到的音频以及文件的音频流)
#define  CTRL_VIDEO_ONLY          (u8)0x02 ///<视频(包括采集到的视频以及文件的视频流)
#define  CTRL_AV_BOTH			  (u8)0x03 ///<（音频＋视频）

#define  MAX_DECODER_NUM		 (u8)96   ///<最大解码器数目


#define  CAP_FRAME_BMP           (u8)0 ///<RGB24位图;
#define  CAP_FRAME_UYVY          (u8)1 ///<yuv4:2:2格式
#define  CAP_FRAME_I420          (u8)2 ///<yuv4:2:0格式
#define  CAP_FRAME_YUY2          (u8)3 ///<yuy2格式
#define  SOFT_DEC_ONLY           (u8)0x01///<仅软解
#define  HARD_DEC_ONLY           (u8)0x02///<仅硬解
#define  SOFTHARD_DEC_BOTH       (u8)0x03///<同时软硬解 

#define  AUDIO_MODE_ONLYDEC      (u8)0x01///<音频模式仅解码
#define  AUDIO_MODE_ONLYRCV      (u8)0x02///<音频模式仅接收
#define  AUDIO_MODE_PLAY         (u8)0x00///<音频模式仅播放    
   
#define NAL_CUSTOM_TRANSDATA   30///<透明数据NAL类型
#define MAX_GPS_INFO_LEN     1024///<GPS信息最大长度

//Err Info
#define ERR_H264_ENC_HANDLE_NULL    8001///<H264编码句柄为空
#define ERR_H264_ENC_BUF_OVER_FLOW  8002///<H264编码缓冲溢出
#define ERR_H264_DEC_HANDLE_NULL    9001///<H264解码句柄为空
#define ERR_H264_DEC_BUF_OVER_FLOW  9002///<H264解码缓冲溢出

//MP4 ENC
#define MP4_CUSTOM_GPS             0xA0///<mpeg4 gps头
#define ERR_MP4_ENC_HANDLE_NULL    6001///<mpeg4编码句柄为空
#define ERR_MP4_DEC_HANDLE_NULL    7001///<mpeg4解码句柄为空

/** 码流结构*/
typedef struct
{
    void *pvStart;   ///<码流缓冲区起始位置
    u8 *pu8Buffer;   ///<码流缓冲区
    u32 u32Length;   ////<流长度
    u8 *pu8Tail;     ///<码流字节位置
}TBitStream;

struct TTransparentBuffer
{
    u16 wLength;    ///<长度包括wLength、wType、dwSubType和pBuf的总长度
	u16 wType;      ///<类型
	u32 dwSubType;  ///<根据byType来约定具体格式，不用时应置为0
	s8* pBuf;       ///<数据
};

/** 宽度、高度，请使用标准的视频格式。参见 videodefines.h 。最大为D1*/
struct TMixVidBgParam
{
	s32 nBgWidth;                             ///<背景图像的宽度
	s32 nBgHeight;                            ///<背景图像的高度
	s32 nDrawBoundaryFlag;                    ///<画面是否勾画边界线的标记（1表明画边界线，0表明不画边界线）
    s32 nBoundaryRGB;                         ///<画面边界线颜色RGB分量（格式为0x00RRGGBB）
    s32 nBoundaryWidth;                       ///<边界线统一宽度（须为2的倍数）
	s32 nDrawBackgroundFlag;                  ///<背景无图像处是否填充背景色
	s32 nBackgroundRGB;	                      ///<画面合成背景填充色RGB分量（格式为0x00RRGGBB）
};


#ifndef TNETSTRUCT
#define TNETSTRUCT
#define MAX_USERDATA_LEN    16   ///<最大用户数据长度

/** 网络参数*/
typedef struct tagNetSession
{
    tagNetSession(){m_dwRTPAddr=0; m_wRTPPort=0; m_dwRTCPAddr=0; m_wRTCPPort=0; \
                    m_dwRtpUserDataLen = 0; m_dwRtcpUserDataLen = 0;}
    
    u32   m_dwRTPAddr;  ///<RTP地址(网络序)
    u16   m_wRTPPort;   ///<RTP端口(本机序)
    u32   m_dwRTCPAddr; ///<RTCP地址(网络序)
    u16   m_wRTCPPort;  ///<RTCP端口(本机序)
    u32   m_dwRtpUserDataLen;   ///<Rtp用户数据长度,用于在代理情况下,需要在每个udp头前添加固定头数据的情况
    u8    m_abyRtpUserData[MAX_USERDATA_LEN]; ///<用户数据指针
    u32   m_dwRtcpUserDataLen;   ///<Rtcp用户数据长度,用于在代理情况下,需要在每个udp头前添加固定头数据的情况
    u8    m_abyRtcpUserData[MAX_USERDATA_LEN]; ///<用户数据指针
}TNetSession;

/** 本地网络参数*/
typedef struct tagLocalNetParam
{
	TNetSession  m_tLocalNet;   ///<本地网络地址
	u32        m_dwRtcpBackAddr;///<RTCP回发地址
	u16         m_wRtcpBackPort; ///<RTCP回发端口      
}TLocalNetParam;

/** 网络发送参数*/
typedef struct tagNetSndParam
{
	u8 m_byNum;                                     ///<发送地址对数
	TNetSession m_tLocalNet;                        ///<本地网络地址
	TNetSession m_tRemoteNet[MAX_NETSND_DEST_NUM];  ///<团段网络地址
}TNetSndParam;

#endif//TNETSTRUCT

/** 编码器参数*/
typedef struct Encoder
{  
	u32   m_dwCpuId;///<CPU标识
	s32   m_nPriority;///<优先级
	HWND    m_hwVideoPreview;///<视频预览句柄
}TEncoder;

/** 视频解码器参数*/
typedef struct VideoDecoder
{
	s32     m_nPriority;///<视频处理线程优先级
	u32     m_dwCpuId;  ///<视频处理的CPU标识
	s32     m_nVideoPlayBufNum;     ///<视频回放缓冲个数
	s32     m_nVideoStartPlayBufNum;///<视频开始回放缓冲个数
	HWND    m_hwPlayVideo;///<视频播放窗口句柄
	BOOL32  m_bOverlay;///<是否Overlay;
	u8      m_byMask;  ///<0x01软件解码,0x02硬件解码,0x03两者同时解码
    double  m_dbDecTimer;  ///<定时控制播放的时间间隔，单位ms；default 0，自适应，不定时。
	BOOL32  m_bNodeinterlacing;///<是否打开deinterlacing开关，default 0	 
	//硬件解码部分 
	s32 	m_nBsp15Freq;//MHZ
	s32 	m_nMemFreq;  //MHZ
	u8      m_byChipType;
	u8	    m_byBoardId;//index from 0 
	u8	    m_byBufNum; //queue length, default 5 
	CHAR	m_achDemoFilePath[MAX_PATH];	
	u32	  m_dwPostPrcLvl;  /*mpv4后处理的级别*/		 
}TVideoDecoder;

/** 音频解码器参数*/
typedef struct AudioDecoder
{
	s32   m_nPriority;       ///<音频处理线程优先级
	u32   m_dwCpuId;         ///<音频处理的CPU标识
	s32   m_nAudioPlayBufNum;///<音频播放缓冲数
	s32   m_nAudioStartPlayBufNum;///<音频开始播放缓冲数
    u8    m_byAudioMode; ///<音频模式
}TAudioDecoder;

#define FRMRATE_CAL_BY_FPS  0 ///<帧率采集模式FPS
#define FRMRATE_CAL_BY_SPF  1 ///<帧率采集模式SPF

/** 视频采集参数*/
typedef struct VideoCapParam
{
	u16	m_wVideoWidth; ///<视频捕获图像宽度(default:640)
	u16	m_wVideoHeight;///<视频捕获图像高度(default:480)
	u8  m_byFrmRateLittleThanOne; ///< 帧率是否小于1 ?  
	u8	m_byFrameRate; ///<视频捕获帧率(default:25)   若帧率>=1，则 m_byFrameRate ＝ fps, 若帧率<1，则 m_byFrameRate ＝ spf  
	u8  m_byFOURCC;    ///<视频捕获帧格式(default:CAP_FRAME_BMP)
}TVideoCapParam;

/** 编码器状态*/
typedef struct KdvEncStatus
{
	BOOL32 			m_bVideoSignal;	  ///<是否有视频信号
	BOOL32	        m_bVideoCapStart; ///<是否开始视频捕获
	BOOL32			m_bAudioCapStart; ///<是否开始音频捕获
	BOOL32			m_bVideoEncStart; ///<是否开始视频编码
	BOOL32			m_bAudioEncStart; ///<是否开始音频编码
	BOOL32			m_bNetSndVideoStart;///<是否开始视频发送
	BOOL32			m_bNetSndAudioStart;///<是否开始音频发送
	u32				m_dwVideoBufLen;  ///<视频缓冲长度
	u32				m_dwAudioBufLen;  ///<音频缓冲长度
	TVideoEncParam	m_tVideoEncParam; ///<视频编码参数
	u8				m_byAudioEncParam;///<音频编码模式
	u8				m_byAudEncMediaType; ///<音频编码媒体类型	
	TNetSndParam    m_tVideoSendAddr; ///<视频发送地址
	TNetSndParam    m_tAudioSendAddr; ///<音频发送地址
	u32				m_dwSetEncParam; ///<设置硬件编码参数次数
	u32				m_dwVideoSend;   ///<已发送的视频包数
	u32				m_dwAudioSend;   ///<已发送的音频包数
	u32				m_dwHardFrameNum;///<硬件回调帧数
	u32				m_dwPrevFrameNum;///<预览帧数	
}TKdvEncStatus;

/** 编码器统计信息*/
typedef struct KdvEncStatis
{
    u8   m_byVideoFrameRate;///<视频帧率
    u32  m_dwVideoBitRate;  ///<视频码流速度
    u32  m_dwAudioBitRate;  ///<语音码流速度
    u32  m_dwVideoPackLose; ///<视频丢帧数
    u32  m_dwVideoPackError;///<视频错帧数
    u32  m_dwAudioPackLose; ///<语音丢帧数
    u32  m_dwAudioPackError;///<语音错帧数
	u32  m_wAvrVideoBitRate;///<1分钟内视频编码平均码率
	u32	 m_wAvrAudioBitRate;///<1分钟内语音编码平均码率
}TKdvEncStatis;

/** 载荷表结构*/
typedef struct tagRtpPayload
{
    u8 m_byRmtActivePT;///<码流实际载荷
    u8 m_byRealPT;///<我们公司约定的载荷
}TRtpPayload;

/** 解码器状态*/
typedef struct KdvDecStatus
{
	BOOL32	m_bVideoDecStart;///<是否开始视频解码
	BOOL32	m_bAudioDecStart;///<是否开始音频解码
	BOOL32	m_bRcvVideoStart;///<是否开始视频接收
	BOOL32	m_bRcvAudioStart;///<是否开始音频接收
	u32	m_dwAudioBufLen; ///<音频播放缓冲长度
	u32	m_dwVideoBufLen; ///<视频播放缓冲长度
	TLocalNetParam   m_tAudioRcvAddr;///<音频当地地址
	TLocalNetParam   m_tVideoRcvAddr;///<视频当地地址
	BOOL32	m_bAudioOutThread;///<是否声音播放线程存活
	BOOL32	m_bMp4CodecThread;///<是否Mp4解码线程存活
	u32   m_dwAudioDecType;     ///<音频解码类型
	u32   m_dwVideoDecType;     ///<视频解码类型
}TKdvDecStatus;

/*解码器统计信息   remove to codeccommon.h*/

/*========================================类的声明与定义======================================*/

/*视频丢包处理策略*/
#define VID_DROP_PREFERRED_QUALITY      0    ///<质量优先，为等I帧策略
#define VID_DROP_PREFERRED_FAST_SPEED   1    ///<速度优先快档，自恢复策略中丢包后只解不播的帧数为1秒钟
#define VID_DROP_PREFERRED_MID_SPEED    2    ///<速度优先中档，自恢复策略中丢包后只解不播的帧数为2秒钟
#define VID_DROP_PREFERRED_SLOW_SPEED   3    ///<速度优先慢档，自恢复策略中丢包后只解不播的帧数为4秒钟

struct TEncoderRef;
struct TEncoderStatus;
class  CSoftEncoder;

typedef void (*PAUDIOCALLBACK)(u8 buAudioMode, u8 *pBuf, s32 nSize, KD_PTR pvContext);
typedef void (*PVOICECHECK)(CHAR cMode, KD_PTR pvContext);
typedef void (*PDRAWCALLBACK)(void* hAddDrawDC, KD_PTR pvParam);
typedef void (*PSTATICPICCALLBACK)(u8 *pBuf, u32 dwSize, BITMAPINFOHEADER* pHead, KD_PTR pvParam);
typedef void (*pfFileStatCB)(u8 byFileState, u32 dwSec, KD_PTR pvContext) ;
typedef void (*PFRAMEINFOCALLBACK)(u8 *pBitmapBuf, u32 dwWidth, u32 dwHeight, u32 dwbitcount, u32 dwTimeStamp, KD_PTR pvParam);//参数分别为位图缓冲指针、位图宽、高、位深。
typedef void (*PAUDIOFRAMEINFOCALLBACK)(u8 *pPcmBuf, u32 dwPcmLen, u32 dwTimeStamp, KD_PTR pvParam);//参数分别为位图缓冲指针、位图宽、高、位深。
typedef void (*PFRAMECALLBACK)(PFRAMEHDR pFrmHdr, KD_PTR pvParam);
//pDataBuf  - 数据缓冲区
//nDataSize - 数据内容长度
//dwContext - 上下文
typedef void (*PDATAINFOCALLBACK)(u8 *pDataBuf, s32 nDataSize, KD_PTR pvParam);
typedef void (*PDRAWTIMESTAMPSCALLBACK)(u32 dwTimestamps, KD_PTR pvParam);

/** 语音检测*/
typedef struct tagVoiceCheck
{
	s32			m_nBGTime;///<ime window of background voice sampling (ms is one unit)
	s32			m_nThreshold;///<阈值
	PVOICECHECK	m_pCallBack;///<回调函数
	void*       m_pvContext;///<上下文
}TVoiceCheck;

/** 文件解码器相关*/
typedef struct tagPlayFileParam
{
	u32				m_dwGranuity;///<文件播放粒度
	void*			m_pvContext;///<上下问
	pfFileStatCB	m_pfCB;///<回调函数
	u8				m_byFileType;///<文件类型
	s8				m_achFileName[512];///<文件名
} TPlayFileParam ;

/** 播放文件信息*/
typedef struct tagPlayFileInfo
{
	u64				m_qwTotalSize;///<文件总大小
	u64				m_qwCreateDate;///<文件创建时间
	u32				m_dwTotalTime;///<文件总时长
} TPlayFileInfo;

/** 文件播放流信息*/
typedef struct tagFileStreamInfo
{
	BOOL32			m_bAudio ;///<是否音频
	u16				m_wWidth;///<宽
	u16				m_wHeight ;///<高
	u8				m_byStreamId ;///<流id
	u8				m_byMediaType ;///<媒体格式payload
} TPlayFileStreamInfo ;

#define MAX_FILE_STREAM		3	///<最大文件流数目

/** 文件播放状态*/
typedef struct tagPlayFileStatus
{
	u32				m_dwCurrentPlayPos;///<当前播放位置
	u8				m_byPlayMode;///<播放模式
	s8				m_byPlayRate;///<播放速率
	u8				m_byPlayState;///<播放状态
	u8				m_byStreamNum ;///<流id
	u32				m_dwVFps;///<视频帧率
	u32				m_dwAFps;///<音频帧率
	TPlayFileStreamInfo		m_atFileStreamInfo[MAX_FILE_STREAM];///<流信息
} TPlayFileStatus;

/** 倒放文件信息*/
typedef struct tagTReserveFileInfo
{
	u32           m_dwFirstPackTimeStamps;///<文件中第一包的时间戳
	u32           m_dwBitRate;///<码率
	u32           m_dwKeyFrameInterval;///<关键帧间隔
	u8           m_byFrameRate;///<帧率
}TReserveFileInfo;

typedef void (*PGETAUDIOPOWER)(u8 byAudioPower, KD_PTR pvContext);

#define PIC_ENCODE_JPEG   (u8)1///<图片编码格式JPEG
#define PIC_ENCODE_BMP    (u8)2///<图片编码格式bmp

#define MAX_LOGO_NAME_LEN (u8)32///<最大台标名称长度

/** 台标融入码流的相关信息设置*/
typedef struct tagLogoSetting
{
	u32 m_dwXPos;    ///<台标位置
	u32 m_dwYPos;    ///<台标位置
	u8  m_byClarity; ///<台标透明度(0-255)
	u8  m_byLogoName[MAX_LOGO_NAME_LEN+3]; ///<终端别名
}TLogoSetting;

/** 倒放清缓冲类型*/
typedef enum
{
	PLAY_TO_RESERVE = 0,///<正放转倒放
	RESERVE_TO_PLAY,    ///<倒放转正放
	SEEKTO,             ///<跳转
}eResetBufType;

/** 文件播放信息*/
typedef struct tagKdmPlayFileInfo 
{
	u8   m_byVideoStreamNum;///<文件中视频流数目
	u8   m_byAudioStreamNum;///<文件中音频流数目
}TKdmPlayFileInfo;

class CKdvEncoder
{	
public:
    CKdvEncoder();
    virtual ~CKdvEncoder();
public:     
/** 
* Create 
* 创建编码器 
* @param[in]   HWND previewHwnd 预览窗口句柄 
* @param[in]   s32 nPriority    线程优先级
* @param[in]   u8 byBindCpuId   使用CPUID
* @param[in]   u8 byCapType     采集类型
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    Create( HWND previewHwnd, 
		           s32 nPriority = THREAD_PRIORITY_NORMAL,
		           u8 byBindCpuId = 0, u8 byCapType = CAP_AC_BOTH );

/** 
* Free 
* 销毁编码器 
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义 
* @par 其它 
*      释放资源函数，上层在程序退出之前必须调用！！！不可放在析构函数中调用！！！   
*/
    u16	   Free();

/** 
* SetVoiceCheck 
* 设置语音功率检测 
* @param[in]   BOOL32 bCheck                    是否检测语音
* @param[in]   const TVoiceCheck *ptVoiceCheck  语音检测参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
   u16    SetVoiceCheck(BOOL32 bCheck, const TVoiceCheck* ptVoiceCheck);
	
/** 
* StartCap 
* 开始采集
* @param[in]   u8 byCapType  采集类型
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    StartCap(u8 byCapType = CTRL_AV_BOTH);

/** 
* StopCap 
* 停止采集
* @param[in]   u8 byCapType  采集类型
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopCap(u8 byCapType =  CTRL_AV_BOTH);

/** 
* SetPreviewHwnd 
* 设置预览窗口
* @param[in]   HWND hPrevWnd  预览窗口句柄
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetPreviewHwnd( HWND hPrevWnd );

/** 
* SelectVideoSource 
* 选择视频源
* @param[in]   const char *szFileName 文件名
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SelectVideoSource(const char *szFileName = NULL);
	
/** 
* SetVideoIcon 
* 设置视频台标
* @param[in]   BOOL32 bEnable 是否设置，TRUE为设置，FALSE不设
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVideoIcon( BOOL32 bEnable );

/** 
* SetVideoCapParam 
* 设置视频采集参数
* @param[in]  const TVideoCapParam  *ptVideoCapParam 视频采集参数结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVideoCapParam(const TVideoCapParam  *ptVideoCapParam );

/** 
* SetAudioCapParam 
* 设置音频采集模式
* @param[in]  u8 byMode 
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioCapParam( u8 byMode );

/** 
* SaveAsPic 
* 保存图片
* @param[in]  LPCSTR lpszFileName 图片名称 
* @param[in]  u8 byEncodeMode     图片编码格式
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SaveAsPic(LPCSTR lpszFileName, u8 byEncodeMode);

/** 
* SetAudioCallBack 
* 设置音频回调
* @param[in]  PAUDIOCALLBACK pAudioCallBack 回调函数
* @param[in]  KD_PTR pvContext              回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAudioCallBack(PAUDIOCALLBACK pAudioCallBack, KD_PTR pvContext);

/** 
* SetPlaySpeed 
* 设置播放速率
* @param[in]  float fSpeed 播放倍速
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetPlaySpeed(float fSpeed);

/** 
* IsStepPlay 
* 是否单帧播放
* @param[in]  BOOL32 bIsStepPlay TRUE为单帧播放，FALSE恢复原来模式
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    IsStepPlay(BOOL32 bIsStepPlay);

/** 
* NextFrame 
* 播放下一帧
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16		NextFrame();

/** 
* SetInputAudioPowerCB 
* 设置获取输入音频功率的回调
* @param[in]  PGETAUDIOPOWER pGetInputAudioPowerCB 回调函数
* @param[in]  KD_PTR pvContext                     回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetInputAudioPowerCB(PGETAUDIOPOWER pGetInputAudioPowerCB, KD_PTR pvContext);

/** 
* RegAudioFrameCallBack 
* 音频编码后帧回调
* @param[in]  PFRAMECALLBACK pFrameCallback 回调函数
* @param[in]  KD_PTR pvParam                回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RegAudioFrameCallBack(PFRAMECALLBACK pFrameCallback, KD_PTR pvParam);

/** 
* SetAddLogoIntoEncStream 
* 设置 台标融入码流 的开关
* @param[in]  TLogoSetting *ptLogoSetting   设置台标参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAddLogoIntoEncStream(TLogoSetting *ptLogoSetting);
	
/** 
* SetTransData 
* 设置视频帧信息中携带的透明数据
* @param[in]  s8 *pszData   透明数据
* @param[in]  u32 dwDataLen 透明数据长度
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16 SetTransData(s8 *pszData, u32 dwDataLen);
	
/** 
* ShowPreview 
* 设置 编码器的本地预览 的开关
* @param[in]  BOOL32 bPreview    TRUE 打开预览，FALSE 关闭预览
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    ShowPreview(BOOL32 bPreview);

/** 
* GetAudioMute 
* 获得是否静音
* @param[out]   BOOL32& bMute    TRUE 静音，FALSE 非静音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioMute( BOOL32& bMute );     // 是否静音 [zhuyr][4/20/2005]

/** 
* GetEncoderStatus																																		
* 获取编码器状态
* @param[out]  TKdvEncStatus &tKdvEncStatus 编码器状态结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetEncoderStatus(TKdvEncStatus &tKdvEncStatus); 	
	
/** 
* GetEncoderStatis																																		
* 获取编码器的统计信息
* @param[out]  TKdvEncStatis &tKdvEncStatis 编码器统计信息结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetEncoderStatis(TKdvEncStatis &tKdvEncStatis );/*获取编码器的统计信息*/

/** 
* StartVideoCap																																		
* 开始采集图像
* @param[in]  u8 byCapType 采集模式
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartVideoCap(u8 byCapType = VIDCAP_CAPVIDEO);

/** 
* StopVideoCap																																		
* 停止采集图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopVideoCap(); /*停止采集图像*/

/** 
* SetVideoEncParam																																		
* 设置视频编码参数
* @param[in] const TVideoEncParam *ptVideoEncParam 视频编码参数结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVideoEncParam(const TVideoEncParam *ptVideoEncParam );

/** 
* GetVideoEncParam																																		
* 得到视频编码参数
* @param[out] TVideoEncParam &tVideoEncParam  视频编码参数结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetVideoEncParam(TVideoEncParam &tVideoEncParam );    
/** 
* StartVideoEnc																																		
*开始压缩图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartVideoEnc();

/** 
* StopVideoEnc																																		
* 停止压缩图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopVideoEnc(); 

/** 
* StartAudioCap																																		
*开始语音采集
* @param[in]  BOOL32 bAinMic 无意义，可不填
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartAudioCap(BOOL32 bAinMic = FALSE);

/** 
* StopAudioCap																																		
* 停止语音采集
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopAudioCap();

/** 
* SetAudioEncParam																																		
* 设置语音压缩参数
* @param[in]  u8 byAudioEncParam  音频模式
* @param[in]  u8 byMediaType      音频payload
* @param[in]  u16 wAudioDuration  音频帧时长
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioEncParam(u8 byAudioEncParam, 
                            u8 byMediaType=MEDIA_TYPE_MP3,
                            u16 wAudioDuration = 30); 
/** 
* GetAudioEncParam																																		
* 得到语音压缩参数
* @param[out]  u8 &byAudioEncParam  音频模式
* @param[out]  u8 *pbyMediaType   音频payload
* @param[out]  u16 *pwAudioDuration  音频帧时长
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioEncParam(u8 &byAudioEncParam, 
                            u8 *pbyMediaType = NULL,
                            u16 *pwAudioDuration = NULL);

/** 
* StartAudioEnc																																		
* 开始压缩语音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    StartAudioEnc();

/** 
* StopAudioEnc																																		
* 停止压缩语音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopAudioEnc(); 

/** 
* SetAudioMute																																		
* 设置是否静音
* @param[in]  BOOL32 bMute TRUE表示静音，FALSE表示不静音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioMute( BOOL32 bMute ); 

/** 
* SetAudioVolume																																		
* 设置采集音量
* @param[in]  u8 byVolume 音量 范围0~255
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioVolume(u8 byVolume ); 

/** 
* GetAudioVolume																																		
* 获取采集音量
* @param[out]  u8 &byVolume  音量
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioVolume(u8 &byVolume );

/** 
* SetNetFeedbackVideoParam																																		
* 设置图像的网络重传参数
* @param[in]  u16 wBufTimeSpan  缓冲时长
* @param[in] BOOL32 bRepeatSnd  是否开启重传
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetNetFeedbackVideoParam(u16 wBufTimeSpan, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetFeedbackAudioParam																																		
* 设置语音的网络重传参数
* @param[in]  u16 wBufTimeSpan  缓冲时长
* @param[in] BOOL32 bRepeatSnd  是否开启重传
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetNetFeedbackAudioParam(u16 wBufTimeSpan, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetSndVideoParam																																		
* 设置图像的网络传送参数
* @param[in]  const TNetSndParam *ptNetSndParam 网络发送参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetNetSndVideoParam(const TNetSndParam *ptNetSndParam);
	
/** 
* SetNetSndAudioParam																																		
* 设置语音的网络传送参数
* @param[in]  const TNetSndParam *ptNetSndParam 网络发送参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetNetSndAudioParam(const TNetSndParam *ptNetSndParam);
/** 
* GetNetSndVideoParam																																		
* 得到图像的网络传送参数
* @param[out]  TNetSndParam *ptNetSndParam 网络发送参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	BOOL32 GetNetSndVideoParam(TNetSndParam *ptNetSndParam);

/** 
* GetNetSndAudioParam																																		
* 得到语音的网络传送参数
* @param[out]  TNetSndParam *ptNetSndParam 网络发送参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    BOOL32 GetNetSndAudioParam(TNetSndParam *ptNetSndParam);

/** 
* StartSendVideo																																		
* 开始发送图像
* @param[in]  int  dwSSRC 发送的SSRC值，0表示不需要修改发送的SSRC值
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartSendVideo(int  dwSSRC = 0);
	
/** 
* StopSendVideo																																		
* 停止发送图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopSendVideo();

/** 
* StartSendAudio																																		
* 开始发送语音
* @param[in]  int  dwSSRC 发送的SSRC值，0表示不需要修改发送的SSRC值
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartSendAudio(int  dwSSRC = 0);

/** 
* StopSendAudio																																		
* 停止发送语音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopSendAudio();
	
/** 
* SetAudioDumb																																		
* 设置是否哑音
* @param[in]  BOOL32 bDumb TRUE表示哑音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16	   SetAudioDumb(BOOL32 bDumb); 
	
/** 
* SetFastUpata																																		
* 设置FastUpdata，MediaCtrl在75帧内编一个I帧发送
* @param[in]  BOOL32 bIsNeedProtect 是否需要强制编I帧
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetFastUpata(BOOL32 bIsNeedProtect = TRUE);
	
/** 
* SetVideoActivePT																																		
* 音频动态载荷的PT值的设定
* @param[in]  u8 byLocalActivePT 动态载荷值
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16 SetAudioActivePT(u8 byLocalActivePT);
	
/** 
* SetVideoActivePT																																		
* 视频动态载荷的PT值的设定
* @param[in]  u8 byLocalActivePT 动态载荷值
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16 SetVideoActivePT(u8 byLocalActivePT );
	
/** 
* SetAudEncryptKey																																		
* 设置音频编码加密key字串以及 加密模式 Aes or Des
* @param[in]  s8 *pszKeyBuf  key字符串
* @param[in]  u16 wKeySize   key字符串长度
* @param[in]  u8 byDecryptMode   加密模式 Aes or Des
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAudEncryptKey( s8 *pszKeyBuf, u16 wKeySize, u8 byEncryptMode );
	
/** 
* SetAudEncryptKey																																		
* 设置视频编码加密key字串以及 加密模式 Aes or Des
* @param[in]  s8 *pszKeyBuf  key字符串
* @param[in]  u16 wKeySize   key字符串长度
* @param[in]  u8 byDecryptMode   加密模式 Aes or Des
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetVidEncryptKey( s8 *pszKeyBuf, u16 wKeySize, u8 byEncryptMode ); 
	
/** 
* GetLastError 
* 获取最后个错误码
* @param[out]  u32 &dwErrorId  错误码   
*/
    VOID    GetLastError( u32 &dwErrorId );
	
private:
	s64    m_qwLastFastupdataTime;     //最近一次FastUpdata的时间

protected:
    CSoftEncoder     *m_pcKdvInterEnc;
    TEncoderRef      *m_ptRef;
    TEncoderStatus   *m_ptStatus;	
};

class CKdvVideoDec;
class CKdvAudioDec;
class CKdvASFFileReader;
struct TDecoderRef;
struct TDecoderStatus;

class CKdvDecoder
{
public:
    CKdvDecoder();
    virtual ~CKdvDecoder();
	
public:
/** 
* CreateDecoder 
* 创建解码器
* @param[in]  const TVideoDecoder *ptVideoDecoder 视频解码器参数
* @param[in]  const TAudioDecoder *ptAudioDecoder 音频解码器参数
* @param[in]  BOOL32 bSaveBuff                    是否缓冲数据，是否保存正放数据（倒放时需要，会影响性能）
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    CreateDecoder(const TVideoDecoder *ptVideoDecoder,const TAudioDecoder *ptAudioDecoder, BOOL32 bSaveBuff = FALSE);

/**  
* GetDecoderStatus																																		
* 获取解码器状态
* @param[out]  TKdvDecStatus &tKdvDecStatus 解码器状态结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetDecoderStatus(TKdvDecStatus &tKdvDecStatus );

/** 
* GetDecoderStatis																																		
* 获取解码器统计信息
* @param[out]  TKdvDecStatus &tKdvDecStatus 解码器统计信息结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetDecoderStatis(TKdvDecStatis &tKdvDecStatis );	

	
/** 
* StartVideoDec																																		
* 开始图像解码
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartVideoDec();

/** 
* StopVideoDec																																		
* 停止图像解码
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopVideoDec();

/** 
* StartAudioDec																																		
* 开始声音解码
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartAudioDec();

/** 
* StopAudioDec																																		
* 停止声音解码
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopAudioDec();

/** 
* SetAudioVolume																																		
* 设置输出声音音量
* @param[in]  u8 byVolume 音量大小 0最小，255最大
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioVolume(u8 byVolume );

/** 
* GetAudioVolume																																		
* 得到输出声音音量
* @param[out]  u8 &pbyVolume 音量大小 0最小，255最大
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioVolume(u8 &pbyVolume );	

/** 
* SetAudioMute																																		
* 设置静音
* @param[in]  BOOL32 bMute 是否静音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioMute(BOOL32 bMute);
	
/** 
* StartRcvVideo																																		
* 开始网络接收图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartRcvVideo();

/** 
* StopRcvVideo																																		
* 停止网络接收图像
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopRcvVideo();

/** 
* StartRcvAudio																																		
* 开始网络接收声音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StartRcvAudio();
	
/** 
* StopRcvAudio																																		
* 停止网络接收声音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    StopRcvAudio();

/** 
* SetVideoNetRcvParam																																		
* 设置图像的网络接收参数
* @param[in]  const TLocalNetParam *ptLocalNetParam 网络接收参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVideoNetRcvParam(const TLocalNetParam *ptLocalNetParam );
	
/** 
* SetAudioNetRcvParam																																		
* 设置语音的网络接收参数
* @param[in]  const TLocalNetParam *ptLocalNetParam 网络接收参数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioNetRcvParam(const TLocalNetParam *ptLocalNetParam );
	
/** 
* SetNetFeedbackVideoParam																																		
* 设置图像的网络重传参数
* @param[in]  TNetRSParam tNetRSParam 网络重传参数
* @param[in]  BOOL32 bRepeatSnd       是否开启重传
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetNetFeedbackVideoParam(TNetRSParam tNetRSParam, BOOL32 bRepeatSnd = FALSE);

/** 
* SetNetFeedbackAudioParam																																		
* 设置语音的网络重传参数
* @param[in]  TNetRSParam tNetRSParam 网络重传参数
* @param[in]  BOOL32 bRepeatSnd       是否开启重传
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetNetFeedbackAudioParam(TNetRSParam tNetRSParam, BOOL32 bRepeatSnd = FALSE);

/** 
* SetVidDecDblSend																																		
* 设置发送第二路视频
* @param[in]  const TNetSndParam *ptNetSndVideoParam 视频发送参数，参数NULL表示关闭
* @param[in]  u16 wBitRate  带宽
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVidDecDblSend(const TNetSndParam *ptNetSndVideoParam, u16 wBitRate = 4<<10);

/** 
* SetAudDecDblSend																																		
* 设置发送第二路音频
* @param[in]  const TNetSndParam *ptNetSndAudioParam 音频发送参数，参数NULL表示关闭
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudDecDblSend(const TNetSndParam *ptNetSndAudioParam); 

/** 
* ClearWindow																																		
* 清屏
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    ClearWindow(); 

/** 
* FreezeVideo																																		
* 视频冻结
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    FreezeVideo();

/** 
* UnFreezeVideo																																		
* 取消视频冻结
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    UnFreezeVideo();

/** 
* SetVideoActivePT																																		
* 设置视频动态载荷
* @param[in]  u8 byRmtActivePT  接收到的动态载荷的Playload值
* @param[in]  u8 byRealPT       该动态载荷实际代表的的Playload类型，等同于我们公司的Payload约定
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetVideoActivePT( u8 byRmtActivePT, u8 byRealPT );
	//设置视频动态载荷查表

/** 
* SetVideoPayloadTable																																		
* 设置查找视频动态载荷表
* @param[in]  TRtpPayload atPayloadTable[]   动态载荷表
* @param[in]  u32 dwPayloadNum               表项的个数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetVideoPayloadTable(TRtpPayload atPayloadTable[], u32 dwPayloadNum);

/** 
* SetVideoCallBackWithoutSPSPPS																																		
* 透传kdvmedainet 接口，控制kdvmedianet 是否在无SPSPPS的情况下也回调，默认值为false.不回调
* @param[in]  BOOL32 bNoPPSSPSStillCallback 是否回调
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16 SetVideoCallBackWithoutSPSPPS(BOOL32 bNoPPSSPSStillCallback);

/** 
* SetVidDecryptKey																																		
* 设置 视频解码key字串 以及 解密模式
* @param[in]  s8 *pszKeyBuf  key字符串
* @param[in]  u16 wKeySize   key字符串长度
* @param[in]  u8 byDecryptMode   解密模式 Aes or Des
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetVidDecryptKey(s8 *pszKeyBuf, u16 wKeySize, u8 byDecryptMode); 

/** 
* SetAudioActivePT																																		
* 设置音频动态载荷
* @param[in]  u8 byRmtActivePT  接收到的动态载荷的Playload值
* @param[in]  u8 byRealPT       该动态载荷实际代表的的Playload类型，等同于我们公司的Payload约定
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAudioActivePT( u8 byRmtActivePT, u8 byRealPT );

/** 
* SetAudioPayloadTable																																		
* 设置查找音频动态载荷表
* @param[in]  TRtpPayload atPayloadTable[]   动态载荷表
* @param[in]  u32 dwPayloadNum               表项的个数
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAudioPayloadTable(TRtpPayload atPayloadTable[], u32 dwPayloadNum);

/** 
* SetAudDecryptKey																																		
* 设置 音频解码key字串 以及 解密模式
* @param[in]  s8 *pszKeyBuf  key字符串
* @param[in]  u16 wKeySize   key字符串长度
* @param[in]  u8 byDecryptMode   解密模式 Aes or Des
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudDecryptKey(s8 *pszKeyBuf, u16 wKeySize, u8 byDecryptMode); 

/** 
* SetVidDropPolicy																																		
* 设置视频丢包处理策略
* @param[in]  u8 byDropPolicy  设置视频丢包处理策略
* @param[in]  u16 wKeySize   key字符串长度
VID_DROP_PREFERRED_QUALITY     质量优先，为等I帧策略
VID_DROP_PREFERRED_FAST_SPEED  速度优先快档，自恢复策略中丢包后只解不播的帧数为1秒钟
VID_DROP_PREFERRED_MID_SPEED   速度优先中档，自恢复策略中丢包后只解不播的帧数为2秒钟
VID_DROP_PREFERRED_SLOW_SPEED  速度优先慢档，自恢复策略中丢包后只解不播的帧数为4秒钟
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetVidDropPolicy(u8 byDropPolicy);

/** 
* SetVideoRtpPublicAddr																																		
* 设置视频接收的公网地址参数
* @param[in]  u32 dwRtpPublicIp  公网IP
* @param[in]  u16 wRtpPublicPort  公网端口
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16	   SetVideoRtpPublicAddr(u32 dwRtpPublicIp, u16 wRtpPublicPort);

/** 
* SetAudioRtpPublicAddr																																		
* 设置音频接收的公网地址参数
* @param[in]  u32 dwRtpPublicIp  公网IP
* @param[in]  u16 wRtpPublicPort  公网端口
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16	   SetAudioRtpPublicAddr(u32 dwRtpPublicIp, u16 wRtpPublicPort);
	
/** 
* SetDecoderMode																																		
* 设置解码模式
* @param[in]  BOOL32 bUserTimer TRUE表示定时解码，FALSE表示不定时解码
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetDecoderMode(BOOL32 bUserTimer);

/** 
* SetNoDeinterlacing																																		
* 设置deinterlacing开关
* @param[in]  BOOL32 bNoDeinterlacing TRUE表示deinterlacing关闭 FALSE表示打开
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetNoDeinterlacing(BOOL32 bNoDeinterlacing);

/** 
* SetWatchMoveRect																																		
* 设置移动侦测观察矩阵
* @param[in]  LPRECT pRect 矩阵参数
* @param[in]  u8 bySize    大小
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetWatchMoveRect(LPRECT pRect, u8 bySize);

/** 
* SetWatchColor																																		
* 设置观察颜色
* @param[in]  COLORREF crWatch  颜色
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetWatchColor(COLORREF crWatch);

/** 
* SetNetDithering																																		
* 否根据网络抖动来调整播放速度（定时播放有效）
* @param[in]  BOOL32 bUser 是否使用
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetNetDithering(BOOL32 bUser);

/** 
* RegStaticPicCallBack																																		
* 视频解码后帧信息回调
* @param[in]  PSTATICPICCALLBACK pCallBack 回调函数
* @param[in]  KD_PTR pvParam               回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    RegStaticPicCallBack(PSTATICPICCALLBACK pCallBack, KD_PTR pvParam);

/** 
* RegFrameInfoCallBack																																		
* 视频解码前帧信息回调
* @param[in]  PFRAMEINFOCALLBACK pFrameInfo 回调函数
* @param[in]  KD_PTR pvParam                回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RegFrameInfoCallBack(PFRAMEINFOCALLBACK pFrameInfo, KD_PTR pvParam);

/** 
* RegFrameCallBack																																		
* 视频网络组帧后帧信息回调（与RegFrameInfoCallBack回调位置不同，网络组帧后可能会经过播放控制处理，有可能会丢弃一部分帧数据，之后才会解码）
* @param[in]  PFRAMECALLBACK pFrame 回调函数
* @param[in]  KD_PTR pvParam        回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RegFrameCallBack(PFRAMECALLBACK pFrame, KD_PTR pvParam);

/** 
* RegAudioFrameInfoCallBack																																		
* 音频解码后帧信息回调
* @param[in]  PAUDIOFRAMEINFOCALLBACK pFrameInfo 回调函数
* @param[in]  KD_PTR pvParam                     回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RegAudioFrameInfoCallBack(PAUDIOFRAMEINFOCALLBACK pFrameInfo, KD_PTR pvParam);

/** 
* RegAudioFrameCallBack																																		
* 音频解码前帧信息回调
* @param[in]  PFRAMECALLBACK pFrame 回调函数
* @param[in]  KD_PTR pvParam        回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RegAudioFrameCallBack(PFRAMECALLBACK pFrame, KD_PTR pvParam);

/** 
* SetTransDataCallBack																																		
* 设置视频帧信息中携带的透明数据（GPS信息）回调函数，每帧回调
* @param[in]  PDATAINFOCALLBACK pFun 回调函数
* @param[in]  KD_PTR pvContext       回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16 SetTransDataCallBack(PDATAINFOCALLBACK pFun, KD_PTR pvContext);

/** 
* SetDrawCallBack																																		
* 设置画图回调
* @param[in]  PDRAWCALLBACK pFun   回调函数
* @param[in]  KD_PTR pvParam       回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetDrawCallBack(PDRAWCALLBACK pFun, KD_PTR pvParam);

/** 
* GetAudioMute																																		
* 得到静音状态
* @param[out]  BOOL32& bMute   是否静音
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioMute(BOOL32& bMute);

/** 
* SetAudioAdpcmDiscardStereo																																		
* 设置adpcm音频丢弃双声道
* @param[in]  BOOL32 bDiscard 是否丢弃
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetAudioAdpcmDiscardStereo(BOOL32 bDiscard);

/** 
* GetAudioAdpcmDiscardStereo																																		
* 获得adpcm音频是否丢弃双声道
* @param[out]  BOOL32 bDiscard 是否丢弃
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetAudioAdpcmDiscardStereo(BOOL32& bDiscard);

/** 
* GetWrapperVer																																		
* 得到Wrapper的版本和编译信息
* @param[out]  s8 *pszVerBuffer 版本信息字符串
* @param[out]  u8 bySize        字符串长度，bysize最好不小于128 
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    GetWrapperVer(s8 *pszVerBuffer, u8 bySize);
    
/** 
* SetShowMargins																																	
* 设置图像裁边
* @param[in]  s32 nLeft    左
* @param[in]  s32 nTop     上
* @param[in]  s32 nRight   右
* @param[in]  s32 nBottom  下
* @param[in]  BOOL32 bAuto bAuto为TRUE时，前四个参数表示四个方向上被裁的宽度是已CIF时的象素数,会随解码图象大小改变,bAuto为FALSe时，前四个参数设定不会随解码图象大小改变
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SetShowMargins(s32 nLeft, s32 nTop, s32 nRight, s32 nBottom, BOOL32 bAuto = TRUE);

/** 
* SetPlayHwnd																																		
* 设置播放窗口句柄
* @param[in]  HWND hPlayWnd         窗口句柄
* @param[in]  BOOL32 bSaveLastFrame 是否保存最后一帧
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetPlayHwnd( HWND hPlayWnd, BOOL32 bSaveLastFrame = FALSE );

/** 
* SetZoomWnd																																		
* 设置缩放窗口和区域
* @param[in]  HWND hPlayWnd         窗口句柄，NULL表示取消缩放窗口
* @param[in]  LPRECT pImgRect       窗口坐标，pImgRect对应的是原始图像的一个局部范围，而不是缩放窗口的局部范围,原始图像的宽高从函数GetDecoderStatis获取
*                                   设置矩形的区域，如果超过图像的返回，程序会进行截取，如果完全不在图像的范围内，则拒绝。
*                                   缩放窗口和主窗口的前后顺序要设定好，绘图时先绘制缩放，再绘制主窗口
*                                   如果发现闪烁，去MSDN查看WS_CLIPSIBLINGS和SetWindowPos.设置一下窗口属性和前后顺序。
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义   
* @par 修改日志 
*      wxh于2010-12-30创建  
*/
    BOOL32 SetZoomWnd(HWND hPlayWnd, LPRECT pImgRect);
    
/** 
* RedrawLastFrame																																		
* 窗口重画
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    RedrawLastFrame();
	
/** 
* SaveAsPic																																		
* 图像保存
* @param[in]  LPCSTR lpszFileName   文件名
* @param[in]  u8 byEncodeMode       编码模式
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
    u16    SaveAsPic(LPCSTR lpszFileName, u8 byEncodeMode);

/** 
* SetOnlyKeyFrame																																		
* 设置是否只解关键帧 
* @param[in]  BOOL32 bKeyFrame   TRUE为设置，FALSE为取消
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetOnlyKeyFrame(BOOL32 bKeyFrame);

/** 
* SetAudioCallBack																																		
* 设置音频网络组帧后回调 
* @param[in]  PAUDIOCALLBACK pAudioCallBack  回调函数
* @param[in]  KD_PTR pvContext               回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetAudioCallBack(PAUDIOCALLBACK pAudioCallBack, KD_PTR pvContext);

/** 
* SetOutputAudioPowerCB																																		
* 设置获取输出音频功率的回调 
* @param[in]  PGETAUDIOPOWER pGetOutputAudioPowerCB  回调函数
* @param[in]  KD_PTR pvContext                       回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	u16    SetOutputAudioPowerCB(PGETAUDIOPOWER pGetOutputAudioPowerCB, KD_PTR pvContext);

/** 
* SetWaitForVB																																		
* 设置垂直同步
* @param[in]  BOOL32 bEnable     是否垂直同步
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义    
*/
	VOID   SetWaitForVB(BOOL32 bEnable);

/** 
* SetZoomNum																																		
* 设置缩放次数
* @param[in]  s8 byZoomNum 缩放次数 （0~3）若不设置，内部采用自动缩放策略，设0，与修改前一样，缩放一次到位，设置大于3的值与3等效
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义 
* @par 修改日志 
*      gxk于2011-06-01创建    
*/
	VOID    SetZoomNum(s8 byZoomNum);

/** 
* InputFrame																																		
* 输入帧数据
* @param[in]  PFRAMEHDR pFrame  帧结构体
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16    InputFrame(PFRAMEHDR pFrame);

/** 
* SetVideoSample																																		
* 设置视频时间戳采样率
* @param[in]  u32 dwSample 时间戳采样率
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16 SetVideoSample(u32 dwSample);
	
/** 
* SetAudioSample																																		
* 设置音频时间戳采样率
* @param[in]  u32 dwSample 时间戳采样率
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16 SetAudioSample(u32 dwSample);

/** 
* GetLastError																																		
* 获取最新的错误码
* @param[out]  u32 &dwErrorId    错误码
*/
    VOID   GetLastError( u32 &dwErrorId );

/** 
* OpenFile																																		
* 打开文件
* @param[in]  TPlayFileParam *ptParam 文件参数
* @param[out]  TKdmPlayFileInfo* ptPlayFileInfo 文件播放信息
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		OpenFile(/*IN*/TPlayFileParam *ptParam, /*OUT*/ TKdmPlayFileInfo* ptPlayFileInfo);

/** 
* CloseFile																																		
* 关闭文件
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		CloseFile();

/** 
* PlayFile																																		
* 播放文件
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		PlayFile();

/** 
* StopFile																																		
* 停止播放文件
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		StopFile();

/** 
* SetPlayPos																																		
* 设置播放文件位置
* @param[in]  u32	dwSec  秒
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		SetPlayPos(u32	dwSec );

/** 
* PauseFile																																		
* 暂停播放文件
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		PauseFile();

/** 
* ResumeFile																																		
* 暂停后重新恢复播放文件
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		ResumeFile();

/** 
* SingleFrame																																		
* 单帧（向前）播放，只进视频帧，跳过音频帧
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		SingleFrame();

/** 
* SingleFrameForWmv																																		
* 单帧（向前）播放（给asf转wmv工具用，单帧向前播放，包括音频和视频帧）
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SingleFrameForWmv();

/** 
* GetStreamPropertyByIndex																																		
* 根据流序号获得流属性信息
* @param[in]  u8 byStreamIndex  流序号
* @param[out]  void * pioStreamProperty
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
    u16     GetStreamPropertyByIndex(u8 byStreamIndex, void * pioStreamProperty);//(TStreamProperty)

/** 
* FastPlay																																		
* 快放（目前不用）
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		FastPlay();

/** 
* FastPlay																																		
* 快放
* @param[in]  u8 rate 快放速率
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		FastPlay(u8 rate);

/** 
* SlowPlay																																		
* 慢放（目前不用）
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		SlowPlay();

/** 
* SlowPlay																																		
* 慢放
* @param[in]  u8 rate 慢放速率
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		SlowPlay(u8 rate);

/** 
* SetPlayAudioStreamNum																																		
* 设置播放音频流
* @param[in]  u8 byAudioStreamNum 默认播放第一路音频，byAudioStreamNum为1，byAudioStreamNum为2时表示播放第2路音频
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SetPlayAudioStreamNum(u8 byAudioStreamNum);

/** 
* GetFileInfo																																		
* 获取文件信息
* @param[out]  TPlayFileInfo *ptFileInfo 文件信息
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		GetFileInfo(TPlayFileInfo *ptFileInfo );
	
/** 
* GetPlayStatus																																		
* 获取播放状态
* @param[out]  TPlayFileStatus *ptPlayFileStatus  播放状态
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16		GetPlayStatus(TPlayFileStatus *ptPlayFileStatus);

/** 
* AsfReadCallBackProc																																		
* 读文件回调
* @param[in]  u32 dwStatus                  播放状态
* @param[in]  u32 dwNextFrameTimeStamp      下一帧时间戳
* @param[in]  TStreamDataPayload *ptPayload 回调数据信息
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	void	AsfReadCallBackProc(u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *ptPayload);

//倒放用接口
/** 
* ResetAllBuffers																																		
* 重置缓冲
* @param[in]  u8 byResetType 重置类型，参见eResetBufType
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     ResetAllBuffers(u8 byResetType);

/** 
* SetReversePlay																																		
* 设置倒放
* @param[in]  BOOL32 bReversePlay  是否倒放
* @param[out] u32* pdwTimestamps   返回当前播放时精确的时间戳
* @param[in]  BOOL32 bAllFrames    是否全帧率播放
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SetReversePlay(BOOL32 bReversePlay, u32* pdwTimestamps, BOOL32 bAllFrames = FALSE);

/** 
* SetDrawTimestampsCallback																																		
* 设置画图时间戳回调
* @param[in]  PDRAWTIMESTAMPSCALLBACK pDrawTimestampsCallBack 回调函数
* @param[in]  KD_PTR pvContext                                回调上下文
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SetDrawTimestampsCallback(PDRAWTIMESTAMPSCALLBACK pDrawTimestampsCallBack, KD_PTR pvContext);

/** 
* SeekTo																																		
* 倒放时跳转
* @param[in]  u32  dwSeekToTime   跳转时间
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SeekTo(u32  dwSeekToTime);

/** 
* PauseReversePlay																																		
* 暂停转倒放
* @param[in]  BOOL32 bPause 是否暂停
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     PauseReversePlay(BOOL32 bPause);

/** 
* SetPlayOver																																		
* 设置播放结束
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SetPlayOver();

/** 
* SetFileInfo																																		
* 倒放时设置播放文件信息
* @param[in]  TReserveFileInfo *ptFileInfo 播放文件信息
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     SetFileInfo(TReserveFileInfo *ptFileInfo);

/** 
* GetPlayTime																																		
* 获取播放的进度
* @param[out]  s32 * pnTimeStamps 进度，单位ms，负数表示未收到码流
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
	u16     GetPlayTime(s32 * pnTimeStamps);

protected:
public:
    CKdvVideoDec       *m_pcKdvVideoDec;//保留
    CKdvAudioDec       *m_pcKdvAudioDec;//保留
    TDecoderRef	       *m_ptRef;   //保留
    TDecoderStatus     *m_ptStatus;//保留


	TPlayFileParam		m_tFilePlayParam ;
	TPlayFileStatus		m_tFilePlayStatus ;
	TPlayFileInfo		m_tFilePlayInfo ;

	u32					m_dwVideoFrameNum ;
	u32					m_dwAudioFrameNum ;
	u32					m_dwVFrameNum;
	u32					m_dwAFrameNum;
	u32					m_dwCurTime;
	u32					m_dwLastTime;
	u8                  m_byAudioStreamNum;
	IKdmFileReader	*m_pcAsfReader ;
protected:

	BOOL32         m_bKeyFrame;
    u8		       m_byFlag;
	//	创建解码器保护
	SEMHANDLE			m_hSem ;
	BOOL32				m_bSemCreate ;
	BOOL32				m_bCreate;

	BOOL32				m_bDecoderMode ;
};


class CAVCapabilityList;
class CAVCapCheck  
{
public:
    CAVCapCheck();
    ~CAVCapCheck();
public:
	BOOL32 CheckAudInCap();
	BOOL32 CheckVidInCap();
	BOOL32 CheckAudOutCap();
	BOOL32 CheckVidOutCap();
	BOOL32 CheckAudEncCap(u8 byMediaType);
	BOOL32 CheckVidEncCap(u8 byMediaType);
	BOOL32 CheckAudDecCap(u8 byMediaType);
	BOOL32 CheckVidDecCap(u8 byMediaType);	

private:
	CAVCapabilityList *m_ptCAVCapList;
};

/** 
* SoftDecoderInit																																		
* 软解码器初始化
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
u16		SoftDecoderInit();

/** 
* SoftDecoderRelease																																		
* 软解码器反初始化
* @retval  CODEC_NO_ERROR  成功 
* @retval  参见错误码定义     
*/
u16		SoftDecoderRelease();

#endif /*__CODECLIB_H*/














