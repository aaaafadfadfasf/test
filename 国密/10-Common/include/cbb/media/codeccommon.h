/** 
* @file         CodecCommon.h 
* @brief        mediasdk 
* @details      通用定义 
* @author       李雪峰
* @date     2004/02/07
* @version  3.0  
* @par Copyright (c):  
*      kedacom 
* @par History:          
*   version: 李雪峰, 2004/02/07, 通用定义\n 
*/ 
#ifndef __COMMON_H_
#define __COMMON_H_

/** 
 * 背景颜色 
 */ 
typedef struct BackBGDColor
{
	u8 RColor;    ///<R背景
	u8 GColor;    ///<G背景
	u8 BColor;    ///<B背景	
}TBackBGDColor;

/** 
 * 重传参数 
 */ 
typedef struct tagNetRSParam
{
    u16  m_wFirstTimeSpan;	  ///<第一个重传检测点
	u16  m_wSecondTimeSpan;  ///<第二个重传检测点
	u16  m_wThirdTimeSpan;   ///<第三个重传检测点
	u16  m_wRejectTimeSpan;  ///<过期丢弃的时间跨度
} TNetRSParam;

/*==================================错误码=======================*/

#ifndef DES_ENCRYPT_MODE
#define DES_ENCRYPT_MODE         (u8)0      ///<DES加密模式
#define AES_ENCRYPT_MODE         (u8)1      ///<AES加密模式
#define ENCRYPT_KEY_SIZE         (u8)32     ///<密钥长度 取其中的较大值
#define DES_ENCRYPT_KEY_SIZE     (u8)8      ///<DES密钥长度
#define AES_ENCRYPT_KEY_SIZE_MODE_A (u8)16  ///<AES Mode-A 密钥长度
#define AES_ENCRYPT_KEY_SIZE_MODE_B (u8)24  ///<AES Mode-B 密钥长度
#define AES_ENCRYPT_KEY_SIZE_MODE_C (u8)32  ///<AES Mode-C 密钥长度
#endif

#define CODEC_NO_ERROR              (u16)0///<无错
#define CODEC_ERROR_BASE            (u16)10000///<错误码基值
#define ERROR_CODEC_UNKNOWN	        (u16)10000///<不知名的编解码器错误 
#define ERROR_CODEC_PARAM           (u16)(CODEC_ERROR_BASE + 1)///<参数错误;  
#define ERROR_DEC_DRAW_HDC	        (u16)(CODEC_ERROR_BASE + 2)///<解码器画图窗口DC错误
#define ERROR_DEC_DRAW_DIBOPEN      (u16)(CODEC_ERROR_BASE + 3)///<解码器画图对象打开错误
#define ERROR_DEC_DRAW_DIBBEGIN     (u16)(CODEC_ERROR_BASE + 4)///<解码器画图格式错误
#define ERROR_DEC_DRAW_CLEARWINDOW  (u16)(CODEC_ERROR_BASE + 5)///<解码器清屏错误
#define ERROR_DEC_ICOPEN            (u16)(CODEC_ERROR_BASE + 6)///<解码器打开解码驱动错误
#define ERROR_DEC_DECOMPRESSBEGIN   (u16)(CODEC_ERROR_BASE + 7)///<解码器设置解码驱动格式错误
#define ERROR_DEC_SETTIMER          (u16)(CODEC_ERROR_BASE + 8)///<解码器设置解码定时器错误
#define ERROR_DEC_NO_VIDEO          (u16)(CODEC_ERROR_BASE + 9)///<解码器没有创建视频
#define ERROR_DEC_SOCKET_CALLBACK   (u16)(CODEC_ERROR_BASE + 10)///<解码器设置网络事件回调错误
#define ERROR_DEC_SOCKET_CREATE     (u16)(CODEC_ERROR_BASE + 11)///<解码器创建网络套接字错误
#define ERROR_DEC_NO_AUDIO          (u16)(CODEC_ERROR_BASE + 12)///<解码器没有创建音频 
#define ERROR_DEC_WAVEOPEN          (u16)(CODEC_ERROR_BASE + 13)///<解码器不能打开声卡*
#define ERROR_DEC_MIXEROPEN         (u16)(CODEC_ERROR_BASE + 14)///<解码器不能打开声卡音量控制
#define ERROR_DEC_WAVEPREHEADER     (u16)(CODEC_ERROR_BASE + 15)///<解码器不能设置音频播放头
#define ERROR_DEC_WAVEWRITE         (u16)(CODEC_ERROR_BASE + 16)///<解码器不能向声卡写音频数据
#define ERROR_DEC_WAVE_ENCOPEN      (u16)(CODEC_ERROR_BASE + 17)///<解码器不能打开音频解码器*
#define ERROR_DEC_WAVE_ENCPREHEADER (u16)(CODEC_ERROR_BASE + 18)///<解码器不能设置音频解码头
#define ERROR_DEC_WAVE_ENCCONVERT   (u16)(CODEC_ERROR_BASE + 19)///<解码器音频解码错误
#define ERROR_DEC_WAVESTART         (u16)(CODEC_ERROR_BASE + 20)///<解码器开始音频播放错误
#define ERROR_DEC_WAVESTOP          (u16)(CODEC_ERROR_BASE + 21)///<解码器停止音频播放错误
#define ERROR_DEC_MIXER_SETVOLUME   (u16)(CODEC_ERROR_BASE + 22)///<解码器设置音量出错
#define ERROR_DEC_MIXER_GETVOLUME   (u16)(CODEC_ERROR_BASE + 23)///<解码器获取音量出错
#define ERROR_DEC_MIXERMUTE         (u16)(CODEC_ERROR_BASE + 24)///<解码器设置静音出错
#define ERROR_DEC_AUDIOOUT_EXIT     (u16)(CODEC_ERROR_BASE + 25)///<解码器音频线程退出出错
#define ERROR_DEC_HARDCREATE        (u16)(CODEC_ERROR_BASE + 26)///<解码器创建硬件解码器失败
#define ERROR_DEC_CREATE            (u16)(CODEC_ERROR_BASE + 27)///<解码器创建出错 
#define ERROR_DEC_NEW               (u16)(CODEC_ERROR_BASE + 28)///<对象生成出错
#define ERROR_DEC_SETAUDIO_ADDR     (u16)(CODEC_ERROR_BASE + 29)///<解码器设置音频地址错误  
#define ERROR_DEC_AUDIO_LEN         (u16)(CODEC_ERROR_BASE + 30)///<音频编解码的数据长度不正确
#define ERROR_DEC_NO_MASK           (u16)(CODEC_ERROR_BASE + 31)///<既不是软件解码也不是硬件解码
#define ERROR_SOCKOBJ_NULL          (u16)(CODEC_ERROR_BASE + 32)///<网络接收对象不存在
#define ERROR_DEC_NET_INVALIDPARAM  (u16)(CODEC_ERROR_BASE + 33)///<无效的网络接收参数 
#define EROOR_DEC_NO_IMPLEMENT      (u16)(CODEC_ERROR_BASE + 34)///<此功能还不支持 
#define ERROR_DEC_SETSTATE          (u16)(CODEC_ERROR_BASE + 35)///<解码器设置解码驱动状态错误
#define	ERROR_DEC_SEMCREATE			(u16)(CODEC_ERROR_BASE + 36)///<创建信号量错
#define	ERROR_DEC_NOTCREATE			(u16)(CODEC_ERROR_BASE + 37)///<解码器没有创建
#define	ERROR_DEC_FILEPLAYSTATUS	(u16)(CODEC_ERROR_BASE + 38)///<文件播放状态错误
#define ERROR_DEC_FILEEND           (u16)(CODEC_ERROR_BASE + 39)///<文件播放结束

#define ERROR_ENC_CAPWIN            (u16)(CODEC_ERROR_BASE + 100)///<编码器创建捕获窗口出错
#define ERROR_ENC_CAPDRIVER         (u16)(CODEC_ERROR_BASE + 101)///<编码器没有发现捕获卡
#define ERROR_ENC_DIBOPEN           (u16)(CODEC_ERROR_BASE + 102)///<编码器画图对象打开错误
#define ERROR_ENC_DRAWHDC           (u16)(CODEC_ERROR_BASE + 103)///<编码器画图窗口打不开
#define ERROR_ENC_ICOPEN            (u16)(CODEC_ERROR_BASE + 104)///<编码器编码对象打开出错
#define ERROR_ENC_DRAWPOS           (u16)(CODEC_ERROR_BASE + 105)///<编码器得到画图位置出错
#define ERROR_ENC_DIBBEGIN          (u16)(CODEC_ERROR_BASE + 106)///<编码器设置画图格式出错
#define ERROR_ENC_GET_VIDEOBMP      (u16)(CODEC_ERROR_BASE + 107)///<编码器得到图像格式出错
#define ERROR_ENC_SET_VIDEOBMP      (u16)(CODEC_ERROR_BASE + 108)///<编码器设置图像格式出错
#define ERROR_ENC_GET_CAPSET        (u16)(CODEC_ERROR_BASE + 109)///<编码器得到捕获设置出错
#define ERROR_ENC_SET_CAPSET        (u16)(CODEC_ERROR_BASE + 110)///<编码器设置捕获设置出错
#define ERROR_ENC_SET_AUDIOFORMATE  (u16)(CODEC_ERROR_BASE + 111)///<编码器设置音频捕获格式出错
#define ERROR_ENC_STREAMOPEN        (u16)(CODEC_ERROR_BASE + 112)///<编码器打开音频编码对象出错
#define ERROR_ENC_PREHEADER         (u16)(CODEC_ERROR_BASE + 113)///<编码器准备音频编码头出错
#define ERROR_ENC_MIXEROPEN         (u16)(CODEC_ERROR_BASE + 114)///<编码器创建控制音量对象出错
#define ERROR_ENC_MIXER_SETVOLUME   (u16)(CODEC_ERROR_BASE + 115)///<编码器设置音量出错
#define ERROR_ENC_MIXER_GETVOLUME   (u16)(CODEC_ERROR_BASE + 116)///<编码器得到音量出错
#define ERROR_ENC_ALREADY           (u16)(CODEC_ERROR_BASE + 117)///<编码器状态已经存在
#define ERROR_ENC_STOPCAP           (u16)(CODEC_ERROR_BASE + 118)///<编码器无法停止捕获
#define ERROR_ENC_SETSTATE          (u16)(CODEC_ERROR_BASE + 119)///<编码器设置编码参数出错
#define ERROR_ENC_ENCBEGIN          (u16)(CODEC_ERROR_BASE + 120)///<编码器设置编码帧格式
#define ERROR_ENC_SETTIMER          (u16)(CODEC_ERROR_BASE + 121)///<编码器设置定时器出错
#define ERROR_ENC_STOPVIDEOENC      (u16)(CODEC_ERROR_BASE + 122)///<编码起停止视频编码出错
#define ERROR_ENC_NO_SETPARAM       (u16)(CODEC_ERROR_BASE + 123)///<编码器没有设置参数
#define ERROR_ENC_STOPAUDIOENC      (u16)(CODEC_ERROR_BASE + 124)///<编码器音频编码出错
#define ERROR_ENC_CREATESOCK        (u16)(CODEC_ERROR_BASE + 125)///<编码器设置当地地址出错
#define ERROR_ENC_STOPSENDVIDEO     (u16)(CODEC_ERROR_BASE + 126)///<编码器停止发送视频出错
#define ERROR_ENC_STOPSENDAUDIO     (u16)(CODEC_ERROR_BASE + 127)///<编码器停止发送音频出错
#define ERROR_ENC_AUDIOENC          (u16)(CODEC_ERROR_BASE + 128)///<编码器音频转换错误
#define ERROR_ENC_NO_CREATE         (u16)(CODEC_ERROR_BASE + 129)///<编码器创建不成功  
#define ERROR_ENC_SOCK_INIT         (u16)(CODEC_ERROR_BASE + 130)///<编码器网络套接字不是2.2版
#define ERROR_ENC_STARTCAP          (u16)(CODEC_ERROR_BASE + 131)///<编码器无法开始捕获
#define ERROR_ENC_CAP_WIDTH         (u16)(CODEC_ERROR_BASE + 132)///<编码器捕获帧太宽
#define ERROR_DRAW_FORMATE          (u16)(CODEC_ERROR_BASE + 133)///<画图格式不支持
#define ERROR_HARDENC_FORMATE       (u16)(CODEC_ERROR_BASE + 134)///<硬件不支持的画图格式
#define ERROR_ENC_CAP_FORMATE       (u16)(CODEC_ERROR_BASE + 135)///<采集格式和编码格式不匹配
#define ERROR_ENC_NEW               (u16)(CODEC_ERROR_BASE + 136)///<对象生成出错
#define ERROR_ENC_INVALID_PARAM     (u16)(CODEC_ERROR_BASE + 137)///<无效的参数
#define ERROR_ENC_CREATE_EVENT      (u16)(CODEC_ERROR_BASE + 138)///<不能创建事件 
#define EROOR_ENC_NO_IMPLEMENT      (u16)(CODEC_ERROR_BASE + 139)///<此功能还不支持 
#define ERROR_ENC_GETSTATE          (u16)(CODEC_ERROR_BASE + 140)///<编码器得到编码参数出错
#define ERROR_DECENC_CREATE_THREAD  (u16)(CODEC_ERROR_BASE + 141)///</不能创建线程

#define ERROR_ENC_CAP_DESKTOP_FORMAT    (u16)(CODEC_ERROR_BASE + 142)///<要求的编码器桌面捕获格式还不支持
#define ERROR_ENC_CREATE_DESKTOP_MEMDC  (u16)(CODEC_ERROR_BASE + 143)///<创建桌面内存dc失败
#define ERROR_ENC_CAP_DESKTOP_WIDTH     (u16)(CODEC_ERROR_BASE + 144)///<编码器捕获桌面分辨率高于当前桌面实际分辨率
#define ERROR_ENC_CAP_DESKTOP_BITCOUNT  (u16)(CODEC_ERROR_BASE + 145)///<编码器捕获桌面位深高于当前桌面实际位深
#define ERROR_ENC_INVILAD_FILE          (u16)(CODEC_ERROR_BASE + 146)///<提供的文件无效
#define ERROR_ENC_OPERATE_FILE          (u16)(CODEC_ERROR_BASE + 147)///<文件编码器对指定的文件操作失败
#define ERROR_DEC_SNAP_NO_VIDEO         (u16)(CODEC_ERROR_BASE + 148)///<解码器抓拍无图像
#define ERROR_DEC_SNAP_NOSPACE          (u16)(CODEC_ERROR_BASE + 149) ///<解码器抓拍磁盘无剩余空间
#define ERROR_ENC_NO_AUDIO_INPUTDEV     (u16)(CODEC_ERROR_BASE + 150) ///<没有音频输入设备

#define  VIDCAP_CAPVIDEO			(u8)1	//采集video
#define  VIDCAP_CAPVGA				(u8)2	//采集vga
//====================结构定义=====================================//
/*图像压缩码率控制参数*/
#define	 RC_MODE_CONST_BITRATE	 (u8)0x0 ///<恒速率
#define  RC_MODE_VAR_BITRATE	 (u8)0x1 ///<变速率

/** 
 * 视频编码参数 
 */ 
typedef struct VideoEncParam
{
	u8  m_byEncType;   ///<图像编码类型
    u8  m_byRcMode;    ///<图像压缩码率控制参数
    u16  m_byMaxKeyFrameInterval;///<I帧间最大P帧数目
    u8  m_byMaxQuant;  ///<最大量化参数(1-31)
    u8  m_byMinQuant;  ///<最小量化参数(1-31)
    u8  m_byReserved1; ///<保留
    u8  m_byReserved2; ///<保留
    u8  m_byReserved3; ///<保留
    u16  m_wBitRate;    ///<编码比特率(Kbps)
    u16  m_wReserved4;  ///<保留		
	u32 m_dwSndNetBand;///<网络发送带宽(单位:Kbps,1K=1024)   
    u32 m_dwReserved5; ///<保留
	u8  m_byQcMode; ///<图像质量码率控制参数,质量优先或者速度优先
    u8  m_byQI; ///< QI值
    u8  m_byQP;///< QP值

	u8  m_byFrameFmt; ///< 分辨率 0:不固定类型, 1: half cif, 2: 1 cif, 3: 2 cif,  4: 4 cif,  5: 用于图像合成编码  
	u8	m_byFrmRateCanSet; ///<帧率是否由外界设定 ?
	u8  m_byFrmRateLittleThanOne; ///< 帧率是否小于1 ?
}TVideoEncParam;

/** 
 * 解码器统计信息 
 */ 
typedef struct KdvDecStatis
{
	u8	  m_byVideoFrameRate;///<视频解码帧率
	u16	  m_wVideoBitRate;   ///<视频解码码率
	u16	  m_wAudioBitRate;   ///<语音解码码率
	u32   m_dwVideoRecvFrame;///<收到的视频帧数
	u32   m_dwAudioRecvFrame;///<收到的语音帧数
	u32   m_dwVideoLoseFrame;///<丢失的视频帧数
	u32   m_dwAudioLoseFrame;///<丢失的语音帧数
    u16   m_wVideoLoseRatio;///<视频丢失率,单位是% 	
	u16   m_wAudioLoseRatio;///<音频丢失率,单位是% 	
	u32   m_dwPackError;///<乱帧数
	u32   m_dwIndexLose;///<序号丢帧
	u32   m_dwSizeLose; ///<大小丢帧
	u32   m_dwFullLose; ///<满丢帧	
	u32   m_wAvrVideoBitRate;   ///<1分钟内视频解码平均码率
	u32	  m_wAvrAudioBitRate;   ///<1分钟内语音解码平均码率
	BOOL32 m_bVidCompellingIFrm;  ///<视频强制请求I帧							  
	u32   m_dwDecdWidth;	///<码流的宽
	u32   m_dwDecdHeight;   ///<码流的高
}TKdvDecStatis; 



#endif





















