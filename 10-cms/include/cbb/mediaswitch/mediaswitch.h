/** 
* @file         mediaswitch.h
* @brief        转码模块
* @details      
* @author       顾肖康
* @date         2016/07/19
* @version      v1.0
* @par Copyright (c):
*    kedacom
* @par History:
*   version:V1.0  Copyright(C) 2003-2016 KDC, All rights reserved.
*/

#ifndef _MEDIA_SWITCH_H_
#define _MEDIA_SWITCH_H_

#include "kdvtype.h"
#include "kdcbbmedia.h"

#ifdef _USE_OSPSMALL_
#include "osp_small.h"
#else
//#include "osp.h"
#endif

//mediaswitch.h的头文件中不要添加__MRTC__宏，mrtc的数据结构与非mrtc版本公用，非mrtc版本不编译mrtc在c文件中的代码
#ifndef _LINUX_
//#if _MSC_VER <= 1200
#include "WS2tcpip.h"

#else

#include <arpa/inet.h>

// #include <in6addr.h>
// #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef s32 MSRESULT; ///< 错误码

/** 错误码 */
#define  MSRESULT_NO_ERROR                            (s32)(0)				///< 成功返回值
#define  MSRESULT_ERROR_BASE                          (s32)(20000)
#define  MSRESULT_ERROR_PARAM                         (MSRESULT_ERROR_BASE+1)	///< 参数错误
#define  MSRESULT_ERROR_NOCREATE                      (MSRESULT_ERROR_BASE+2)	///< 未创建
#define  MSRESULT_ERROR_MAXCREATE                     (MSRESULT_ERROR_BASE+3)	///< 达到最大创建数目
#define  MSRESULT_ERROR_MEM                           (MSRESULT_ERROR_BASE+4)	///< 内存错误
#define  MSRESULT_ERROR_UNSUPPORT                     (MSRESULT_ERROR_BASE+5)	///< 不支持
#define  MSRESULT_ERROR_UNKNOWN                       (MSRESULT_ERROR_BASE+6)	///< 未知错误
#define  MSRESULT_ERROR_SOCKET                        (MSRESULT_ERROR_BASE+7)	///< 套接字错误
#define  MSRESULT_ERROR_CONNECTTIMEOUT                (MSRESULT_ERROR_BASE+8)	///< 连接超时
#define  MSRESULT_ERROR_CONNECTIONCLOSE               (MSRESULT_ERROR_BASE+9)	///< 对端关闭连接
#define  MSRESULT_ERROR_AGAIN                         (MSRESULT_ERROR_BASE+10)	///< 需要重新发送
#define  MSRESULT_ERROR_CONNECT                       (MSRESULT_ERROR_BASE+11)	///< connect失败
#define  MSRESULT_ERROR_LISTEN                        (MSRESULT_ERROR_BASE+12)	///< 监听失败
#define  MSRESULT_ERROR_TASKCREATE                    (MSRESULT_ERROR_BASE+13)	///< 线程创建错误
#define  MSRESULT_ERROR_NOTRTP                        (MSRESULT_ERROR_BASE+14)	///< 非RTP包
#define  MSRESULT_ERROR_SEMCREATE                     (MSRESULT_ERROR_BASE+15)	///< 锁创建失败
#define  MSRESULT_ERROR_NOINIT                        (MSRESULT_ERROR_BASE+16)	///< 没有初始化
#define  MSRESULT_ERROR_BINDSOCKET                    (MSRESULT_ERROR_BASE+17)	///< 绑定端口失败
#define  MSRESULT_ERROR_FRAMEFULL                     (MSRESULT_ERROR_BASE+18)	///< 帧缓冲溢出
#define  MSRESULT_ERROR_PAYLOAD                       (MSRESULT_ERROR_BASE+19)	///< payload错误
#define  MSRESULT_ERROR_FRAMELIST                     (MSRESULT_ERROR_BASE+20)	///< 帧缓冲错误
#define  MSRESULT_ERROR_NOSTREAM                      (MSRESULT_ERROR_BASE+21)	///< 无码流
#define  MSRESULT_ERROR_GETAGAIN                      (MSRESULT_ERROR_BASE+22)	///< 读得太快，请稍后再次get数据
#define  MSRESULT_ERROR_READSLOW                      (MSRESULT_ERROR_BASE+23)	///< 读得太慢，请重新获取位置再get数据
#define  MSRESULT_ERROR_DATA                          (MSRESULT_ERROR_BASE+24)	///< 数据错误
#define  MSRESULT_ERROR_NOKEYFRAME                    (MSRESULT_ERROR_BASE+25)	///< 没有关键帧
#define  MSRESULT_ERROR_NOTSTART                      (MSRESULT_ERROR_BASE+26)	///< MSOut没有start
#define  MSRESULT_ERROR_FRAMELISTFULL                 (MSRESULT_ERROR_BASE+27)	///< 帧缓冲满
#define  MSRESULT_ERROR_CREATEAGAIN                   (MSRESULT_ERROR_BASE+28)	///< 多线程同时创建
#define  MSRESULT_ERROR_SPACEFULL                     (MSRESULT_ERROR_BASE+29)	///< 散列表空间已满
#define  MSRESULT_ERROR_FRAME_LIST_FULL               (MSRESULT_ERROR_BASE+30)	///< 帧链表满,输入数据慢一点.
#define  MSRESULT_ERROR_TCP_SEND                      (MSRESULT_ERROR_BASE+31)	///< TCP发送失败
#define  MSRESULT_ERROR_HASH_ERR                      (MSRESULT_ERROR_BASE+32)	///< 帧链表满,输入数据慢一点.
#define  MSRESULT_ERROR_BIND_FAILED                   (MSRESULT_ERROR_BASE+33)	///< 绑定失败
#define  MSRESULT_ERROR_INPROCESS                     (MSRESULT_ERROR_BASE+34)	///< tcp连接正在处理
#define  MSRESULT_ERROR_ACCEPTTIMEOUT                 (MSRESULT_ERROR_BASE+35)	///< 等待socket连接超时.
#define  MSRESULT_ERROR_BIND_INUSE                    (MSRESULT_ERROR_BASE+36)	///< 重复绑定
#define  MSRESULT_ERROR_UNSUPPORT_PSSTREAMID          (MSRESULT_ERROR_BASE+37)	///< 不支持的PS類型
#define  MSRESULT_ERROR_SEM_TIMEOUT                   (MSRESULT_ERROR_BASE+38)	///< 信号量超时

///MAXMSINLISTNUM MAXMSOUTLISTNUM MAXMSPIPLINELISTNUM 平台是10000，其他产品为1000，已经废弃，请不要再使用
#if 1
#define MAXMSINLISTNUM       (u32)(10000)
#define MAXMSOUTLISTNUM      (u32)(10000)
#define MAXMSPIPLINELISTNUM  (u32)(10000)
#endif

#define  MS_MAXENCNAMELEN (u32)(8)	///< 编码名称最大长度
#define  MS_MAXDESCRIPEINFOLEN (u32)(64)	///< 描述信息最大长度
#define  MS_MAX_VKEK_LEN            (u32)(32)    ///< VKEK字符串长度
#define  MS_MAX_VKEK_VERSION_LEN    (u32)(32)    ///< 描述信息最大长度
#define  MS_MAX_BIND_CORE_NUM    (u32)(256)    ///< 最大支持绑定的核个数
#define  MS_MAX_STREAM_NUM_IN_PROGRAM    (u32)(255)    ///< PS支持最大的流个数

#ifdef _LINUX_
	#define IN6_ADDR   struct in6_addr
	#define IN_ADDR    struct in_addr
#endif

//提供动态库需要
// for Microsoft c++
#ifdef _MSC_VER


#ifdef __cplusplus
#define M_API  extern "C" __declspec(dllexport)
#else
#define M_API  __declspec(dllexport)
#endif
#define MEDIASWITCH_API    __declspec(dllexport)

// for gcc
#else
#ifdef __cplusplus
#define M_API extern "C"
#else
#define M_API
#endif
#define MEDIASWITCH_API

#endif    // _MSC_VER

/** 时间戳计算模式 */
typedef enum tagMSTSCalMode
{
	E_MS_Ts_Local_Base = 0, ///< 默认模式,本地基准并矫正
	E_MS_Ts_Custom_Base,    ///< 自定义基准,不矫正
	E_MS_Ts_Rtcp_Ntp_Base_Priority,			///< RTCP优先基准.(如果rtcp中没有ntp,则按照E_MS_Ts_Local_Base执行)
    E_MS_Ts_Max,
}EMSTSCalMode;

/** 时间戳计算配置选项 */
typedef struct tagMSTSCalParam
{
	EMSTSCalMode m_eMode;
	u64 m_qwBaseUtc; ///<基准utc时间, E_MS_TS_CUSTOM_BASE需要设置改参数,其余模式不需要设置这个参数.
} TMSTSCalParam;

/** 加解密模式 */
typedef enum tagMSCipherMode
{
    MEDIANET_KDC_EXT_ENCRYPT,   ///<解密模式
    MEDIANET_KDC_EXT_DECRYPT,   ///<加密模式
    MEDIANET_KDC_EXT_INVALID,   ///<无效模式
} EMSCipherMode;

/** MediaCipher创建时参数 */
typedef struct tagMSCipherParam
{
    EMSCipherMode m_eMode;          ///<模式
    u32			m_dwEngineIdMd;     ///<散列算法加密用engine id
    u32			m_dwEngineIdCipher; ///<对称加密算法加密用engine id
    u32			m_dwEngineIdPkey;   ///<非对称加密算法加密用engine id
    u32			m_dwEngineIdRand;   ///<随机数生成用engine id
} TMSCipherParam;

/* VKEK结构体定义 */
typedef struct tagMSVkekInfo
{
    u8  m_abyVkek[MS_MAX_VKEK_LEN+1];   ///vkek明文/密文字符串
    u32 m_dwVkekLen;                    ///m_abyVkek中有效字符串长度	
    u8  m_abyVesion[MS_MAX_VKEK_VERSION_LEN+1]; ///vkek版本号字符串
    u32 m_dwVesionLen;                  ///m_abyVesion中有效字符串长度
    TMSCipherParam m_tCipherParam;      ///解密器参数
} TMSVkekInfo;

/** IP地址信息 */
typedef struct tagMSNetAddr
{
	u16 m_wIPType;           ///< IP类型 must be AF_INET or AF_INET6
	union
	{
		IN_ADDR m_tV4Addr;   ///< V4IP
		IN6_ADDR m_tV6Addr;  ///< V6IP
	};
}TMSNetAddr;

///扩展数据类型
typedef enum tagMSDataExtType
{
    MS_DATA_EXT_NTPTIME = 1,  ///<ntp时间
}EMSDataExtType;

///扩展数据
typedef struct tagMSDataExt
{
    EMSDataExtType m_eDataType; ///<扩展数据类型
    union
    {
        u64  m_qwNtpTime;       ///<NTP时间戳
    };
}TMSDataExt;

/** 连接模式 */
typedef enum tagMSBindMode
{
    E_MS_Bind_Connect_NotSharePort = 0, ///< 默认模式,主动对外连接,不与其他in与out共享端口.
    E_MS_Bind_Listen_NotSharePort, ///< 被动监听模式,不与其他in与out共享端口.
    E_MS_Bind_Listen_SharePort, ///< 被动监听模式,使用共享的端口.
}EMSBindMode;

/** tcp绑定参数 */
typedef struct tagMSTcpBindParam
{
    TMSNetAddr m_tLocal;    ///< 绑定的IP地址.
    EMSBindMode m_eBindMode;  ///< 绑定模式
} TMSTcpBindParam;

/** 网络参数 */
typedef struct tagMSRtpSession
{
	TMSNetAddr    m_tRTPAddr;  ///< RTP地址(网络序)
	u16           m_wRTPPort;  ///< RTP端口(本机序)
	TMSNetAddr    m_tRTCPAddr; ///< RTCP地址(网络序)
	u16           m_wRTCPPort; ///< RTCP端口(本机序)
}TMSRtpSession;

/** 编码名称以及payload */
typedef struct tagMSEncNameAndPayload
{
	s8       m_byEncName[MS_MAXENCNAMELEN]; ///< 编码名称
	u8       m_byPayload;	///< rtp中的payload值
}TMSEncNameAndPayload;

/** rtcp保活检测类型*/
typedef enum tagMSRtcpAliveChkType
{
    E_MS_RtcpAliveChkStop = 0,  ///< 不检测
    E_MS_RtcpAliveChkStart,     ///< 开启检测，规则1. 当MSIN在超时时间内没有收到SR包，判定为not alive；
                                ///<规则2. 当MSOUT在超时时间内没有收到RR包，判定为not alive；
    E_MS_RtcpAliveChkAuto,      ///< 自动检测，规则1.MSIN收到SR包后开启检测,在超时时间内没有再收到SR包，判定为not alive；
                                ///规则2. MSOUT收到RR包后开启检测,在超时时间内没有再收到RR包，判定为not alive；
}EMSRtcpAliveChkType;

/** rtcp 开启保活与断开时间 */
typedef struct tagMSRtcpAlive 
{
    EMSRtcpAliveChkType m_eChkType; ///<保活类型,默认E_MS_RtcpAliveChkAuto
	u32    m_dwDisconnectTime; ///< 多长时间未收到保活包则认为断链，单位（秒）
}TMSRtcpAlive;

/** 码流sps以及pps信息 */
typedef struct tagMSSpsPpsInfo 
{
	u8*      m_pbySPS;     ///< SPS
	u32      m_dwSPSSize;  ///< SPS长度
	u8*      m_pbyPPS;     ///< PPS
	u32      m_dwPPSSize;  ///< PPS长度
}TMSSpsPpsInfo;

/** 接收端重传参数 */
typedef struct tagMSRSParam 
{
	u32 m_dwFirstRSCheckTime;  ///< 第一个重传检测点
	u32 m_dwSecondRSCheckTime; ///< 第二个重传检测点
	u32 m_dwThirdRSCheckTime;  ///< 第三个重传检测点
	u32 m_dwRejectRSCheckTime; ///< 丢弃时间点
}TMSRSParam;

/** 码流类型PS ES */
typedef enum tagMSType
{
	E_MS_UnknowType = 0, ///< 未知类型
	E_MS_EsType,         ///< ES类型
	E_MS_PsType          ///< PS类型
}EMSType;

/** 设置输出时需要伪装的ip以及端口 */
typedef struct tagMSMapAddr
{
	TMSNetAddr    m_tAddr; ///< RTP地址(网络序)
	u16           m_wPort; ///< RTP端口(本机序)
}TMSMapAddr;

/** 设置输出时需要伪装的ip以及端口 */
typedef struct tagMSMap
{
    TMSMapAddr m_tRtpMap; ///<ip或者端口为0标示不伪装
    TMSMapAddr m_tRtcpMap;///<ip或者端口为0标示不伪装
}TMSMap;

/** 输出payload或者InterleavedId  */
typedef struct tagMSChangePayload
{
	s32          m_nInterleavedId; ///<  rtsp tcp模式使用InterleavedId，其他模式设置为0
	u8           m_byPayload;	   ///< 码流payload
}TMSChangePayload;

/** 接收端统计信息  */
typedef struct tagMSRcvStatistics
{
    u32    m_dwPackNum;       ///< 已接收的包数
    u32    m_dwPackLose;      ///< 丟包数
    u32    m_dwPackIndexError;///< 包乱序数
    u32    m_dwFrameNum;      ///< 已接收的帧数
}TMSRcvStatistics;

/** 流数 */
typedef struct tagStreamNumber
{
	u32 m_dwAudioStreamNum; ///<音频流数
	u32 m_dwVideoStreamNum; ///<视频流数
}TMSStreamNumber;

/** 接入InId/OutId描述信息  
* 目标id为Inid时：
*	格式：ip + 第n通道   
*	注意：放像时ip填充nvr ip
*	例如：172.16.64.67 第2通道
* 目标id为Outid时：
*	主要是功能描述：web预览+ip/gui预览/gui放像/gui录像  (其中一种)
*	注意：web预览时添加对方ip
*/
typedef struct tagTMSDescripeInfo
{
	s8   m_szMSDescripeInfo[MS_MAXDESCRIPEINFOLEN];    ///< 简短的描述信息

}TMSDescripeInfo;

typedef struct tagMSVideoInFo  
{
	u16 m_wVideoWidth;
	u16 m_wVideoHeight;
}TMSVideoInFo;

typedef enum tagMSMrtcChannelMode
{
    kInvalid = 0x0, ///< 无效信道
    kAudio = 0x01,  ///< 音频信道
    kVideo = 0x02,  ///< 视频信道
}TMSMrtcChannelMode;

///mrtc发送buf模式
typedef enum tagMSMrtcSndBufMode
{
    E_MS_MRTC_SndBufInvalid = 0x0, ///< 未配置，使用默认配置
    E_MS_MRTC_ExternalSndBuf = 0x01,  ///< 外部缓冲，使用netpacket缓冲
    E_MS_MRTC_InternalSndBuf = 0x02,  ///< 内部缓冲，使用mrtc缓冲，会多消耗内存
}EMSMrtcSndBufMode;

///通道创建配置
typedef struct tagMSMrtcChnCreateCfg
{
    TMSMrtcChannelMode  m_eChnMode; ///<通道音视频模式
    EMSMrtcSndBufMode   m_eSndBufMode; ///<通道buf模式，默认使用E_MS_MRTC_ExternalSndBuf
}TMSMrtcChnCreateCfg;

/** MRTC发送端配置       */
typedef struct tagMSMrtcSenderConfig
{
    u8 m_byRedEnable;   ///< 0: 禁止音频多倍发送, >0:开启音频多倍发送, 默认值0 (视频忽略)
    u8 m_byRedPayload;  ///< >0: 音频: 多倍发送使能时, 媒体包使用的媒体类型,默认值125; 视频: FEC使能时,媒体包使用的媒体类型, 默认值126
    u8 m_byFecEnable;   ///< 0: 禁止视频FEC, >0: 开启视频FEC功能, 默认值0 (音频忽略)
    u8 m_byFecPayload;  ///< >0: FEC使能时, FEC包使用的媒体类型, 默认值127 (音频忽略)
    u8 m_byFecMode;     ///< FEC使能时, FEC包生成规则的模式, 0: 突发丢包场景, >0: 随机丢包场景, 默认值0 (音频忽略)
    u8 m_byFecRate;     ///< FEC使能时, FEC包的生成比例, 取值1~255, 生成FEC包的个数=(媒体包的个数 * m_byFecRate) >> 8, 因cpu瓶颈, 比例不易太高, 默认值25(FEC包占比10%) (音频忽略)
    u8 m_byFecMaxFrames; ///< >0: FEC使能时, 开始生成FEC包的最大帧数, 默认值1 (音频忽略)
    u8 m_byCCEnable;    ///< 0: 禁止拥塞控制功能, >0: 开启拥塞控制功能, 会根据网络状态, 调整帧率/码率, 默认值0
    u32 m_dwCCStartRate; ///< >0: 起始发送速率, 默认值4Mbps
    u32 m_dwCCMinRate;  ///< >0: 最小发送速率, 拥塞控制调整的码率不会低于该值, 默认值10kbps
    u32 m_dwCCMaxRate;  ///< >0: 最大发送速率, 拥塞控制调整的码率不会高于该值, 默认值100Mbps
    u8 m_byPaceEnable;  ///< 0: 禁止平滑发送功能, >0: 开启平滑发送功能, 优先重传丢包, 有利于提高流畅性, 会消耗额外的CPU, 默认值0
    u8 m_byRtcpEnable;  ///< 是否允许发送RTCP包, >0: 允许, 0: 不允许, 默认值1
    u8 m_byRtxEnable;   ///< 0: 禁止丢包重传, >0: 允许丢包重传, 默认值1
    u8 m_byRtxMode;     ///< 重传包格式, 0: 重传包与原媒体包相同, Keda使用该模式, >0: 重传包遵守RFC4588, 与原媒体包不同, 有独立的PayloadType和SSRC, 默认值0
    u8 m_byRtxPayload;  ///< >0: @m_byRtxMode>0时有效, RFC4588重传包的媒体类型, 默认值0
    u32 m_dwRtxSSRC;    ///< >0: @m_byRtxMode>0时有效, RFC4588重传包的SSRC, 默认值0
    u32 m_dwRtxHistorySize; ///< >0: 发送历史记录中包的个数, 用于重传和平滑发送, 消耗内存MEM=m_dwRtxHistorySize*1500bytes, 默认值4000(1080P 4M@25fps约10s)
}TMSMrtcSenderConfig;

/** MRTC接收端配置       */
typedef struct tagMSMrtcReceiverConfig
{
    u8 m_byNackEnable;  ///< 是否发送重传请求, 0: 不发送, >0: 发送, 默认值1
    u8 m_byNackMode;    ///< 重传请求的格式, 0: 重传请求使用SDES NOTE包, kedacom旧的模式, >0: 重传请求使用RFC4585规定的NACK包, 默认值0 (MRTC自动协商是否切换为NACK包)
    u8 m_byRtxMode;     ///< 发送端的重传包格式, 0: 重传包与原媒体包相同, Keda使用该模式, >0: 重传包遵守RFC4588, 与原媒体包不同, 有独立的PayloadType和SSRC, 默认值0
    u8 m_byRtxPayload;  ///< 若发送端的重传包遵守RFC4588, 则必须指定与发送端重传包相同的媒体类型, 默认值0, Keda未使用该模式
    u32 m_dwRtxSSRC;    ///< 若发送端的重传包遵守RFC4588, 则必须指定与发送端重传包相同的SSRC, 默认值0, Keda未使用该模式
    u32 m_dwMinDelayMs; ///< 接收端开始组帧的最小延迟时间, 默认值200
    u32 m_dwMaxDelayMs; ///< 接收端组帧允许的最大延迟, 不小于@min_delay_ms, 超时会丢弃不全的帧, 默认值1000
    u8 m_byRtcpEnable;  ///< 是否允许发送RTCP包, >0: 允许, 0: 不允许, 默认值1 (开启依赖于RTCP的功能时,会自动使能,比如拥塞控制或丢包重传等)
    u8 m_byRembEnable;  ///< 是否发送REMB包(draft-alvestrand-rmcat-remb), >0: 发送, 0: 不发送, 默认值1
    u8 m_byRedPayload;  ///< 发送端的RED的媒体类型
    u8 m_byFecPayload;  ///< 发送端的FEC的媒体类型
    u8 m_byFramePacingMode;///< mrtc抛帧模式, 0: 即时抛帧模式(低延时低流畅性)(默认值) 1: 平滑抛帧模式(高延时高流畅性) 
    u8 m_byPerfDisable;   //mrtc接收通道高性能关闭,0：开启高性能接收，高的cpu消耗；1:关闭高性能接收，低的Cpu消耗，建议纯转发时，关闭高性能
}TMSMrtcReceiverConfig;

/** 网络状态反馈        */
typedef struct tagMSMrtcNetworkStatus
{
    u32 m_dwEstimatedRate;  ///< 估计的当前带宽
    u32 m_dwRtt;            ///< 往返时间,单位ms
    u8 m_byLossRate;        ///< 丢包率,取值0~255,换算百分比公式 ((loss * 100) >> 8)%
    u8 m_byRequestKeyFrame; ///< 是否请求关键帧, >0: 发送端应立即生成一个关键帧, 0: 无意义
}TMSMrtcNetworkStatus;

/** 接收端网络状态反馈 */
typedef struct tagMSMrtcReceiveState
{
    u32 m_dwRate;                   ///< 接收速率, 单位bps, 网络好时约大于码率(因包括RTP头), 网络不好时抖动较大, 更新间隔2s
    u32 m_dwExpectedDelay;          ///< 期望延时, 单位ms, 在[期望最小延时, 可容忍最大延时]之间波动
    u32 m_dwTotalFrames;            ///< 缓冲中还未上抛的帧数(不连续或不完整的帧需要等待重传, 不能上抛)
    u32 m_dwFirstTS;                ///< 缓冲中还未上抛的第一帧的时间戳
    u32 m_dwLastTS;                 ///< 缓冲中还未上抛的最后一帧的时间戳
    u32 m_dwAverageCompleteTimeMs;  ///< 每帧完整收全花费时间的平均值(归一化到每帧10包), 单位ms
}TMSMrtcReceiveState;

typedef struct tagMSStatisticsInfo
{
    u32 m_dwSendSrCount;     ///< sender report count, 发送sr包计数,out有效
    u32 m_dwSendRrCount;    ///< receiver report count,发送的rr包计数，in有效  
    u32 m_dwDiffFromAddPipeToSndOut; ///< add pipeline到发送第一包的时间差值.(毫秒)，out有效
    u32 m_dwPackNum;       ///< 包数，in接收的包数，或者out发送的包数
    u32 m_dwPackLose;      ///< 丟包数，in有效
    u32 m_dwPackIndexError;///< 包乱序数， in有效
    u32 m_dwFrameNum;      ///< 已接收的帧数，in有效
    u32 m_dwRtpBitrate;     ///<带RTP头的比特率统计，in与out有效
    u32 m_dwDataBitrate;    ///<编码数据的比特率统计(不包含rtp头)，in与out有效
	float  m_fFrameRate;     ///<实时帧率
}TMSStatisticsInfo;

typedef struct tagMSMux
{
    BOOL32 m_bEnable; ///< 混合使能
    u32 m_dwMuxId;       ///< muxid
}TMSMux;

///onvif放像扩展头，目前仅为过onvif工具使用，很多平台对该扩展头支持不好，所以正常情况不带扩展头，使用前提E_MS_OUT_Extence配置要为TRUE.
typedef struct tagMSOvfPlaybackExtHdr
{
    BOOL32  m_bEnable;  ///< 启用配置
    /*
    Cseq: 1 byte. This is the low-order byte of the Cseq value used in the RTSP PLAY
    command that was used to initiate transmission. When a client sends multiple,
    consecutive PLAY commands, this value may be used to determine where the data from
    each new PLAY command begins.
    */
    u8      m_byCSeq;   ///< rtsp play id
}TMSOvfPlaybackExtHdr;

/** 输入输出属性参数设置  */
typedef enum tagMSOpt
{
    /** 以下选项如果没有注明可动态修改，都是要在输入/输出start前配置 */
	/** 以下为输入/输出需要设置的参数 */
	E_MS_Type = 0, ///< EMSType 设置输入或者输出类型是ES还是PS(必须设置)
	E_MS_RtcpAlive = 1,///< TMSRtcpAlive  设置输出 rtcp保活模式, 默认保活关闭，保活开启状态下，默认90秒，可动态修改
	E_MS_DescripeInfo = 2, ///< TMSDescripeInfo in/out简短描述信息，
    E_MS_SockBuf  = 3, ///<u32 dwBufSize,设置socket发送接收缓冲,默认2MB.单位字节.
    E_MS_Statistics = 4, ///< TMSStatisticsInfo* ptStat, 统计信息. 只支持获取.
    E_MS_SharePort = 5, ///< BOOL32 bShared, rtp端口是否共享,默认FALSE,不共享,端口由每个In与out独占;如果端口共享，本地与远端的端口与地址须正确填写，否则数据包会错误处理
    //E_MS_ChkSrcInfo = 6, ///<BOOL32 bOn,设置udp源地址端口校验，默认为FALSE,关闭.可动态修改,废弃拆分成E_MS_UdpRtpChkSrcAddr与E_MS_UdpRtcpChkSrcAddr
    E_MS_RtpMux = 7, ///<TMSMux, RTP多路复用
    E_MS_RtcpMux = 8, ///<TMSMux, RTCP多路复用
    E_MS_MapIPPort = 9, ///<TMSMap, 设置输出时需要伪装的ip以及端口（伪装ip端口，对性能有消耗）
    E_MS_UdpPeerRtpParam = 10,///<TMSUdpPeerParam, 更新对端rtp参数, in在MSInStart调用后使用；out在MSOutStart调用后使用；
    E_MS_UdpPeerRtcpParam = 11, ///<TMSUdpPeerParam, 更新对端rctp参数,in在MSInStart调用后使用；out在MSOutStart调用后使用；
    E_MS_UdpRtpChkSrcAddr = 12, ///<<BOOL32 bEnable, 默认FALSE,rtp源地址校验，用于防串码流
    E_MS_UdpRtcpChkSrcAddr = 13, ///<<BOOL32 bEnable, 默认FALSE,rtcp源地址校验，用于防串流
    E_MS_RtcpSharePort = 14, ///< BOOL32 bShared, rtcp端口是否共享,默认FALSE,不共享,端口由每个In与out独占;如果端口共享，本地与远端的端口与地址须正确填写，否则数据包会错误处理，默认不能是TRUE的原因，IPC穿nat接入NVR需要依赖传输的第一个rtcp包来更新远端地址

	/** 以下为输入端需要设置的参数 */
	E_MS_IN_MaxINPacketNum = 20,///< u32 dwMaxInPacketNum, 设置输入最大包缓冲数，默认为2048,若设置的值不为2的指数幂，会向上取整至2的指数幂，最大不得超过32768
	E_MS_IN_MaxOutPacketNum = 21,///< u32 dwMaxOutPacketNum, 设置输出最大帧缓冲数，默认为8192,若设置的值不为2的指数幂，会向上取整至2的指数幂，最大不得超过32768
	E_MS_IN_MaxFrameNum = 22,///<u32 dwMaxFrameNum，设置最大帧缓冲数，默认为1，拉模式下，建议帧缓冲数不要设置为1
	E_MS_IN_RSParam = 23,///<TMSRSParam, 设置udp输入时的重传参数，建议该值分级别可配，级别一，40 80 120 200（512个包缓冲），级别二，80 200 480 720（2048个包缓冲），级别三， 100 280 600 1500（4096个包缓冲），默认级别二。
	E_MS_IN_EncNameAndPayload = 24,///<TMSEncNameAndPayload，设置onvif接入输入编码名称以及payload，若不设置则按照公司内部payload处理
	E_MS_IN_Sample = 25,///<u32 dwSample，设置onvif接入输入音视频采样率，aaclc格式必须设置
	E_MS_IN_AaclcChannel = 26,///<u8 byAaclcChannel，设置onvif接入输入音频aaclc声道数，用于aaclc格式音频，aaclc格式必须设置
	E_MS_IN_SpsPpsInfo = 27,///<TMSSpsPpsInfo, 设置输入sps，pps信息，用于onvif接入外厂商某些码流中不携带sps，pps信息，仅在rtsp信令中携带的情况
	E_MS_IN_ReSend = 28,///<BOOL32 bResend,设置udp输入时是否开启重传，默认为TRUE-开启重传, 可动态修改
	E_MS_IN_KedacomRtp = 29,///<BOOL32 bKedacomRtp, 设置是否为科达自己的RTP包，TRUE表示科达自己的rtp包，FALSE表示外厂商的RTP包，onvif接入外厂商时需要设置为FALSE，默认为TRUE
	E_MS_IN_FrameRate = 30,///<u8 byFrameRate,设置帧率，onvif接入外厂商码流时需要从rtsp信令中获取帧率信息设置，默认为25帧/秒
	E_MS_IN_StandardTS = 31,///<BOOL32 bStandardTS 设置输入是否为标准时间戳，TRUE表示为标准时间戳，FALSE表示为非标准时间戳，默认为TRUE
	//E_MS_IN_RcvStatics = 32,///<TMSRcvStatistics  仅支持get，获取输入的统计信息
	E_MS_IN_VideoInfo  = 33,///< TMSVideoInFo ,获取视频宽高信息，无效值为0，仅当有GUI浏览，录像时，即进入组帧逻辑，且收到关键帧之后才有宽高信息

	E_MS_IN_Ts_Calc_Mode = 34,///< TMSTSCalParam, 设置时间戳计算方法选项.
	E_MS_IN_Frame_List_Cover = 35,///<BOOL32 bCover 设置帧链表(帧缓冲)是否满覆盖.默认为TRUE.
	E_MS_IN_VKEK = 36,///< TMSVkekInfo, 设置vkek,vkek更新时需要设置vkek.可动态修改
    //E_MS_IN_FilterFunc = 37, ///<MSFilterFunc, 设置过滤函数,可以空,可以为有效的函数指针.可动态修改
    E_MS_IN_ReSendBuf  = 38, ///<BOOL32 bOn,设置udp重传缓冲是否开启，默认为FALSE,没有重传缓冲.可动态修改
    E_MS_IN_P2pReSend  = 39, ///<BOOL32 bOn,设置udp精确重传是否开启，默认为FALSE,精确重传关闭.可动态修改
    E_MS_IN_ChkStreamInfo = 40, ///<BOOL32 bChk,默认开启，设置接收码流包流属性检测，包括ps流音视频数目变化与宽高变化，es流的视频宽高变化，es流音频采样率声道数位宽变化.可动态修改
    
    /** 以下为输出端需要设置的参数 */
	E_MS_OUT_Payload = 100, ///<TMSChangePayload 设置输出payload（仅在需要修改payload时设置,rtsp 的tcp输出时需要填对应的m_nInterleavedId，其余情况下m_nInterleavedId填0）
	E_MS_OUT_Extence = 101,///<BOOL32 bExtence 设置输出是否带扩展头，TRUE表示带扩展头，FALSE表示不带扩展头，默认为TRUE
	E_MS_OUT_StandardTS = 102,///<BOOL32 bStandardTS 设置输出是否为标准时间戳，TRUE表示为标准时间戳，FALSE表示为非标准时间戳，默认为TRUE
	E_MS_OUT_SSRC = 103,///<u32 dwSSRC 设置输出SSRC，默认为不修改SSRC
	//E_MS_OUT_MapIPPort = 104,///<TMSMap,设置输出时需要伪装的ip以及端口（伪装ip端口，对性能有消耗）
	E_MS_OUT_UnStandardAaclc = 105,///<BOOL32 bStandardAaclc,设置发送非标准的aaclc码流（视频会议无法解析监控标准的aaclc码流，因为监控的aaclc带4字节头），暂时仅支持udp（因为与会议对通仅有udp）
	E_MS_OUT_SingleAudio = 106,///<BOOL32 bSingleAudio,设置输入是否是单音频
    E_MS_OUT_PackResExtHdrNum = 107, ///<u32 dwReservedExtHdrNum,设置切包时预留的扩展头字段个数，仅仅对H265，PS，svac生效
    E_MS_OUT_TrackID = 108, ///<TMSTrackID 设置码流的trackid，仅仅对PS输入有效
	E_MS_OUT_MultNetSet = 109, ///<TMSMultCardList 设置多网卡发送的网卡信息
	E_MS_OUT_PayloadEx = 110, ///< 区别于E_MS_OUT_Payload，E_MS_OUT_Payload修改所有包的payload，E_MS_OUT_PayloadEx仅修改符合条件的包的payload（音视频混合流的情况下，有需求需要只修改其中一路的payload）
    E_MS_OUT_OvfPlaybackExtHdr = 111, ///< TMSOvfPlaybackExtHdr onvif放像扩展头，目前仅为过onvif工具使用，很多平台对该扩展头支持不好，使用前提E_MS_OUT_Extence配置要为TRUE.
	
    E_MS_MrtcCreateChannel = 201, ///< TMSMrtcChnCreateCfg, 创建MRTC信道, 并指定是音频信道, 还是视频信道
    E_MS_IN_MrtcConfig = 231,     ///< TMSMrtcReceiverConfig, 获取或设置MRTC接收信道的配置 
    E_MS_OUT_MrtcConfig = 251,    ///< TMSMrtcSenderConfig, 获取或设置MRTC发送信道的配置
}EMSOpt;

/** udp传输参数 */
typedef struct tagMSUdpTransParam
{
	TMSRtpSession      m_tLocalNet;  ///< 本地网络参数
	TMSRtpSession      m_tRemoteNet; ///< 远端网络参数
}TMSUdpTransParam;

/** udp对端参数 */
typedef struct tagMSUdpPeerParam
{
	TMSNetAddr  m_tAddr;  ///< 地址(网络序)
	u16         m_wPort;  ///< 端口(本机序)
}TMSUdpPeerParam;

/** tcp传输参数 */
typedef struct tagMSTcpTransParam
{
	TMSRtpSession      m_tLocalNet;        ///< 本地网络参数
	TMSRtpSession      m_tRemoteNet;       ///< 远端网络参数
	u32                m_dwDisconnectTime; ///< 超时断链时间,0为异步接口，其他值同步接口，单位秒(同步接口将逐步废弃)
//	BOOL32             m_bListen;		   ///< 是否为监听端
}TMSTcpTransParam;

/** rtsp协议使用的interleavedid与trackid的对应 */
typedef struct tagMSInterleavedAndTrackId
{
	s32         m_nInterleavedId; ///< rtsp协议使用的interleavedid
	TKDTrackID  m_tTrackId;       ///< trackid
}TMSInterleavedAndTrackId;

/** fd传输参数 */
typedef struct tagMSRtspTransParam
{
	s32                         m_nFd; ///< fd
	TMSInterleavedAndTrackId	m_atInterleavedAndTrackId[KD_MAX_NUM_TRACK]; ///< rtsp协议使用的interleavedid及其与trackid对应关系
}TMSRtspTransParam;


/** 传输类型 */
typedef enum tagMSTransType
{
	E_Ms_UnknowTransType = 0,///< 未知传输类型
	E_MS_Udp,				///< TMSUdpTransParam udp传输参数
	E_MS_Tcp,				///< TMSTcpTransParam tcp传输参数
	E_MS_Rtsp,				///< TMSRtspTransParam rtsp传输参数
	E_MS_YushiGBTcp,		///< TMSTcpTransParam tcp传输参数
}EMSTransType;

/** 缓冲位置信息 */
typedef enum tagMSPosType
{
	E_MS_GetLastPos = 1, ///< 获取缓冲中最新的位置
	E_MS_GetFirstPos,    ///< 获取缓冲中最老的位置
	E_MS_GetBeforeTime,  ///< 获取时间戳前一个关键帧的位置
}EMSPosType;


/** PS流信息 */
typedef struct tagMSRtpMediaParam
{
	TMSEncNameAndPayload m_tEncNameAndPayload;
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

}TMSMediaParam;

/** PS流变更信息 */
typedef struct tagMSPsRtpMediaInfoChange
{
	TKDTrackID   m_tPSTrackID;              ///<变更流信息
	TMSMediaParam m_tMspsMediaParam;       ///<流信息
}TMSMediaInfoChange;

typedef struct tagKDTrackIDExt
{
    EKDTrackType    m_eTrackType;       ///<类型
    u32             m_dwTrackIndx;      ///<针对不同的TrackType,该索引值均从0开始
    u32             m_dwId;             ///<InId
}TKDTrackIDExt;

/** TrackID信息 */
typedef struct tagMSTrackID
{
    //TKDTrackID  m_atTrackId[KD_MAX_NUM_TRACK];  ///<TrackID列表
    TKDTrackIDExt  m_atTrackId[KD_MAX_NUM_TRACK];  ///<TrackID列表
    u16 m_wNum;   ///<TrackID个数
}TMSTrackID;

typedef struct tagMSNetCardInfo
{
	s8            m_szCardName[KD_MAX_LEN_NETCARD_NAME]; /// 网卡名称
	TMSMapAddr    m_tCardAddr;                  /// 使用的网卡地址及端口号（使用rawsocket进行发送）
}TMSNetCardInfo;

typedef struct tagMSMultCardList
{
	u16               m_wNum;                                   /// 网卡发送数量
	TMSNetCardInfo    m_atNetCardInfo[KD_MAX_NUM_NET_CARD];     /// 网卡名称 mark 注释
} TMSMultCardList;

/** 码流sps以及pps所在的buffer */
typedef struct tagMSFrameBuf 
{
	u8*      m_pbyframeBuf; ///< sps，pps所在的buffer
	u32      m_dwframeBuf;  ///< sps，pps所在的buffer长度
}TMSFrameBuf;

/** 网络事件类型 */
typedef enum tagMSEventType
{
	/** 正常消息事件回调 */
	E_MS_RecvRtcpRR      = 1,	///< 收到RR包
	E_MS_RecvRtcpBYE     = 2,	///< 收到BYE包
	E_MS_PSStreamSpsPps  = 5, ///< TMSFrameBuf   码流中的sps以及pps所在的buffer
	E_MS_ESStreamSps     = 6, ///< TMSSpsPpsInfo 码流中的sps
	E_MS_ESStreamPps     = 7, ///< TMSSpsPpsInfo 码流中的pps
	/** 错误事件回调 */
	E_MS_TcpDisconnect   = 100,	///< tcp断链
	E_MS_MallocFailed    = 101,	///< 内存分配失败
	E_MS_StreamChange    = 102, ///< 串码流
	E_MS_UnKnown_Payload = 103, ///< 未知码流载荷类型  pEventData 为载荷类型
	E_MS_RtcpDisconnect  = 104, ///< rtcp保活断链
	E_MS_TSError         = 105, ///< 设置的时间戳不准确，pEventData 为是否标准时间戳 bStandardTS
	E_MS_TcpSendError    = 106,	///< tcp发送失败
    E_MS_TcpDataErr      = 107, ///< 接收的tcp数据错误
    E_MS_MediaName = 108, ///< 接收的音视频编解码类型 使用结构体TMSMediaInfoChange
    E_MS_StreamNumber = 109, ///< in里面的码流个数
    E_MS_TcpConnectOk = 110, ///< tcp连接成功
    E_MS_TcpAcceptOk = 111, ///< tcp accept成功
	/** mrtc事件回调 */
    E_MS_NetworkChanged  = 201,     ///< 网络状态上报
    E_MS_ReceiveStateReport = 202,  ///< 接收端状态上报
}EMSEventType;

/** 核绑定参数 */
typedef struct tagMSThreadBindCoreParam
{
    u16  m_wBindCoreNum;		///< 绑定的核个数
    u16  m_awBindCoreList[MS_MAX_BIND_CORE_NUM]; ///<绑定的核列表,取值范围[0,256)，第一个核ID为0，以此类推
}TMSThreadBindCoreParam;

/** 初始化参数信息 */
typedef struct tagMSInitParam 
{
	u32  m_dwMaxInNum;		///< 最大输入数
	u32  m_dwMaxOutNum;		///< 最大输出数
	u32  m_dwMaxPiplineNum; ///< 最大连接数
    u16  m_wMsgStartPort;   ///< 模块内部信令通信起始端口.默认20400
    u16  m_wMsgEndPort;     ///< 模块内部信令通信终止端口.默认20406
    TMSThreadBindCoreParam   m_tRecvThreadBind; ///<接收线程绑定核参数
    TMSThreadBindCoreParam   m_tSendThreadBind; ///<发送线程绑定核参数
}TMSInitParam;

/** 帧内存分配及释放信息 */
typedef struct tagMSFrame
{
	TKDFrame  m_tFrame;			///< 帧结构体
	s32       m_nRefCount;		///< 引用计数
	void* m_hFrameSem;		///< 锁
// #ifdef _LINUX_
// 	atomic_t  m_tRefCount;
// #else
// #endif

	BOOL32 (*MSMallocFrame) (struct tagMSFrame*, u32 dwFrameSize); ///< 分配内存，内部用
	void   (*MSFreeFrame)   (struct tagMSFrame*); ///< 释放内存
	void   (*MSFrameAddRef) (struct tagMSFrame*); ///< 增加引用计数
	
	u32       m_dwRtpTimeStamp;          /*时间戳，接收的RTP协议中携带的时间戳*/
}TMSFrame;

/** nat prob */
typedef struct tagMSNatProbePack
{
	//NAT 探测包，buf 及长度 ，buf否中是否带usrdata，由上层填充。 
	u8         *m_pbyBuf;//探测包buffer 
	u16         m_wBufLen;//探测包长度,不超过1452	
	//nat 探测包对端地址
	TMSNetAddr  m_tPeerAddr;//RTCP回发地址(网络序)
	u16         m_wPeerPort;//RTCP回发端口（本机序）
}TMSNatProbePack;

typedef struct tagMSNatProbeProp 
{ 
	TMSNatProbePack m_tRtpNatProbePack; //rtp natpack struct
	TMSNatProbePack m_tRtcpNatProbePack; //rtcp natpack struct	
	u32 m_dwInterval;  //发送间隔，单位s。若为0，则表示不发送。
}TMSNatProbeProp;

typedef struct  tagMSExInfo
{
    u8*    m_pbyExBuf;//默认为NULL，用于额外信息， 例如智能信息，数字水印等，以减少拷贝
	u32    m_dwExBufLen;//默认为0，
    BOOL32 m_bSingleNalu;//是否为单nalu（默认为TRUE,单nalu,H264切包时P帧不去遍历查找0001，可降低CPU消耗,多nalu情况下bSingleNalu为TRUE请注意对通是否有问题）
}TMSExInfo;

typedef struct tagMSFilterFuncParam
{
    TMSNetAddr m_tRecvIP; ///<接收IP
    u16 m_wRecvPort;///<接收端口
    TMSNetAddr m_tSrcIP; ///<源IP
    u16 m_wSrcPort;///<源端口
    u8* m_pData;    ///<数据指针
    u32 m_dwLen;    ///<数据长度.
    u32 m_dwId;     ///<ID值
}TMSFilterFuncParam;

/** 
* @pam[in] void* TMSFilterFuncParam* ptParam 数据信息
* @return  	  TRUE:数据过滤掉，FALSE:数据不过滤
*/
typedef BOOL32 (*MSFilterFunc)(TMSFilterFuncParam* ptParam);

/** 
* @pam[out] void* pvContext  		上下文指针
* @note		  帧回调时pData为帧结构体TMSFrame，回调的数据使用完需要使用TMSFrame中的MSFreeFrame进行释放.包回调时pData为包结构体TKDPack.
*/
typedef   void (*PMSDATAPROC)(void* pData, void* pvContext); 

/** 
* @brief  事件回调函数
* @param[in]  u32 dwId	创建时返回的ID
* @param[in]  EMSEventType eEventType	事件类型
* @param[out] void* pEventData  		返回的data
* @param[out] u32 dwEventDataLen 		返回的data长度
* @param[out] void* pEventData  		上下文指针
*/
typedef   void (*PMSEVENTPROC)(u32 dwId, EMSEventType eEventType, void* pEventData, u32 dwEventDataLen, void* pvContext);

/** 
* @brief      初始化模块
* @param[in]  TMSInitParam* ptInitParam   	初始化参数  参见MAXMSINLISTNUM，MAXMSOUTLISTNUM，MAXMSPIPLINELISTNUM
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInit(const TMSInitParam* ptInitParam);

/** 
* @brief      设置切包长度
* @param[in]  u16 wPackLength   切包长度（不可超过IPC_MAX_RTPPACK_LENGTH-1384）
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSSetPackLength(u16 wPackLength);

/** 
* @brief      反初始化
* @note		  反初始化
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSUninit();

/**
*@brief 设置全局tcp端口范围
*@param[in]    u16 wStartPort最小端口
*@param[in]    u16 wEndPort最大端口
*@return       参见错误码定义
*@note 在MSInTcpBind前调用，只可调用一次。
*/
MEDIASWITCH_API MSRESULT MSSetTcpPortRange(u16 wStartPort, u16 wEndPort, u16 wSharePortNum);

/**
*@brief 设置过滤函数,过滤所有接收的包
*@param[in]    MSFilterFunc pFilter 过滤函数指针,pFilter为NULL时是清除回调，支持动态设置
*@return       参见错误码定义
*@note 对所有的IN有效
*/
MEDIASWITCH_API MSRESULT MSSetFilter(MSFilterFunc pFilter);

/** 
* @brief      创建
* @param[out]  PEVENTPROC pfEventCallBackProc 事件回调
* @param[out]  void* pvContext回调上下文
* @param[out]  u32* pdwId  返回对象id
* @note		  引用全局变量：无
*             不能在设置的回调函数中调用mediaswitch接口,可能造成死锁；不能再回调函数中执行耗时操作
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInCreate(PMSEVENTPROC pfEventCallBackProc, void* pvContext, u32* pdwId);


/** 
* @brief      设置参数，类似于setsockopt，通过不同的EMSOpt，设置不同的结构体参数
* @param[in]  u32 dwId 输入对象id
* @param[in]  EMSOpt eOpt  设置参数类型
* @param[in]  void* pvOptParam  参数结构体
* @param[in]  u32 dwOptParamLen 参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInSetOpt(u32 dwId, EMSOpt eOpt, const void* pvOptParam, u32 dwOptParamLen);


/** 
* @brief      获取参数，类似于getsockopt，通过不同的EMSOpt，设置不同的结构体参数
* @param[in]  u32 dwId 输入对象id
* @param[in]  EMSOpt eOpt  设置参数类型
* @param[out]  void* pvOptParam  参数结构体
* @param[out]  u32 dwOptParamLen 参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInGetOpt(u32 dwId, EMSOpt eOpt, void* pvOptParam, u32* dwOptParamLen);


/** 
* @brief      输入数据，根据EKDDataPackFormat的不同输入帧数据以及包数据，该接口与MSInSetTransParam不可同时调用
* @param[in]  u32 dwId 输入对象id
* @param[in]  EKDDataPackFormat eDataPackFormat  输入数据格式
* @param[in]  void* pData输入数据结构体指针  TKDPack或TKDFrame
* @param[in]  u32 dwDataLen  输入数据结构体大小
* @param[in]  void* pExData输入数据结构体指针  TMSExInfo,输入包数据时TMSDataExt类型指针，可以为NULL
* @param[in]  u32 dwExDataLen  输入数据结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInInputData(u32 dwId, EKDDataPackFormat eDataPackFormat, void* pData, u32 dwDataLen, void* pExData, u32 dwExDataLen, const void* pvContext);

/** 
* @brief      tcp绑定
* @param[in]  const TMSTcpBindParam* ptParam 绑定参数.
* @param[out] u16* pwBindPort 绑定的端口.
* @note		  引用全局变量：无,tcp使用.在调用MSInSetTransParam前调用.
* @return  	  参见错误码定义
*/
MEDIASWITCH_API  MSRESULT MSInTcpBind(u32 dwId, const TMSTcpBindParam* ptParam, u16* pwBindPort);

/** 
* @brief      设置输入网络参数，该接口与MSInInputData不可同时使用
* @param[in]  u32 dwId 输入对象id
* @param[in]  ETransType eTransType 传输参数类型
* @param[in]  void* pTransParam 传输参数结构体指针
* @param[in]   u32 dwParamSize 传输参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInSetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32 dwParamSize);


/** 
* @brief      获取输入网络参数
* @param[in]  u32 dwId 输入对象id
* @param[in]  ETransType eTransType 传输参数类型
* @param[out] void* pTransParam 传输参数结构体指针
* @param[out] u32 dwParamSize 传输参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInGetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32* pdwParamSize);


/** 
* @brief      开始输入码流
* @param[in]  u32 dwId 输入对象id
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInStart(u32 dwId);


/** 
* @brief      停止输入码流
* @param[in]  u32 dwId 输入对象id
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInStop(u32 dwId);

/** 
* @brief      设置nat探测包参数
* @param[in]  u32 dwId 输入对象id
* @param[in]  TMSNatProbeProp nat探测包结构体
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInSetNatProbeProp(u32 dwId, TMSNatProbeProp *ptNatProbeProp);

/** 
* @brief      释放
* @param[in]  u32 dwId 输入对象id
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInRelease(u32 dwId);

/** 
* @brief      清空缓冲(包括帧链表与未组帧的包)
* @param[in]  u32 dwId 输入对象id
* @note		  引用全局变量：无,使用MSInInputData输入数据的情况使用.
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSInClearBuf(u32 dwId);

/** 
* @brief      创建输出对象
* @param[out]  PEVENTPROC pfEventCallBackProc 事件回调
* @param[out]  void* pvContext 回调上下文
* @param[out]  u32* pdwId 返回输出对象id
* @note		  引用全局变量：无
*             不能在设置的回调函数中调用mediaswitch接口,可能造成死锁；不能再回调函数中执行耗时操作
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutCreate(PMSEVENTPROC pfEventCallBackProc, void* pvContext, u32* pdwId);

/** 
* @brief      设置输出参数，类似setsockopt
* @param[in]  u32 dwId 输出对象id
* @param[in]  EMSOpt eOpt  设置参数类型
* @param[in]  const void* pvOptParam  参数结构体
* @param[in]  u32 dwOptParamLen 参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutSetOpt(u32 dwId, EMSOpt eOpt, const void* pvOptParam, u32 dwOptParamLen);

/** 
* @brief      获取输出参数，类似getsockopt
* @param[in]  u32 dwId 输出对象id
* @param[in]  EMSOpt eOpt  设置参数类型
* @param[out] const void* pvOptParam  参数结构体
* @param[out] u32 dwOptParamLen 参数结构体大小
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutGetOpt(u32 dwId, EMSOpt eOpt, void* pvOptParam, u32* dwOptParamLen);

/** 
* @brief      设置数据回调
* @param[in]  u32 dwId 输出对象id
* @param[in]  const TKDTrackID*  ptTrackId 输出TrackId
* @param[out] EKDDataPackFormat eDataPackFormat 获取数据格式，包还是帧
* @param[out] PDATAPROC pfDataCallBackProc  数据回调函数
* @param[out] void* pvContext   回调上下文
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutSetDataCallback(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, PMSDATAPROC pfDataCallBackProc, void* pvContext);

/** 
* @brief      tcp绑定
* @param[in]  u32 dwId 输出对象id
* @param[in]  const TMSTcpBindParam* ptParam 绑定参数.
* @param[out] u16* pwBindPort 绑定的端口.
* @note		  引用全局变量：无,tcp使用.在调用MSOutSetTransParam前调用.
* @return  	  参见错误码定义
*/
MEDIASWITCH_API  MSRESULT MSOutTcpBind(u32 dwId, const TMSTcpBindParam* ptParam, u16* pwBindPort);

/** 
* @brief      设置输出传输参数
* @param[in]  u32 dwId 输出对象id
* @param[in]  EMSTransType eTransType 输入参数类型
* @param[in] const void* pTransParam 输入参数结构体
* @param[in] u32 dwParamSize 输入参数结构体大小
* @note		  1.引用全局变量：无  2.推模式下设置输出参数
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutSetTransParam(u32 dwId, EMSTransType eTransType, const void* pTransParam, u32 dwParamSize);

/** 
* @brief      获取传输参数
* @param[in]  u32 dwId 输出对象id
* @param[in]  EMSTransType eTransType 输入参数类型
* @param[out] const void* pTransParam 输入参数结构体
* @param[out] u32 dwParamSize 输入参数结构体大小
* @note		  1.引用全局变量：无  2.推模式下设置输出参数
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutGetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32* pdwParamSize);

/** 
* @brief      获取数据位置
* @param[in]  u32 dwId 输出对象id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in] EKDDataPackFormat eDataPackFormat 获取数据格式，包还是帧
* @param[in] EMSPosType ePosType 最老位置还是最新位置
* @param[out] u16 *pwPos  返回位置
* @note		  1.引用全局变量：无  2.拉模式下获取数据接口
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutGetDataPos(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, EMSPosType ePosType, u64 qwUtcTime, u16 *pwPos);

/** 
* @brief      获取数据
* @param[in]  u32 dwId 输出对象id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in]  EKDDataPackFormat eDataPackFormat 获取数据格式，包还是帧(★★★帧格式为TMSFrame,包格式为TKDPack)
*             帧数据用完之后使用(TMSFrame*)ptData->MSFreeFrame((TMSFrame*)ptData)释放
			  包数据用完之后使用MSFreeData释放（MSFreeData接口暂未实现帧数据的释放，实现后可统一使用该接口释放）
* @param[in]  u16 wSeq 获取位置
* @param[out] void* ptData 返回相应的数据结构体
* @note		  1.引用全局变量：无  2.拉模式下获取数据接口
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutGetData(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, u16 wPos, void* ptData);

/** 
* @brief      获取下一位置
* @param[in]  u32 dwId 输出对象id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in]  EKDDataPackFormat eDataPackFormat 获取数据格式，包还是帧
* @param[in]  u16 wSeq 参考位置
* @param[out] u16* pwNextPos 参考位置下一位置
* @note		  1.引用全局变量：无  2.拉模式下获取数据接口
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutGetNextPos(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, u16 wPos, u16* pwNextPos);

/** 
* @brief      发送rtcp bye
* @param[in]  u32 dwId 输出对象id
* @note		  调用一次，发送一次消息,仅用于udp方式
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutSendBYE(u32 dwId);


/** 
* @brief      开始输出
* @param[in]  u32 dwId 输出对象id
* @note		  引用全局变量：无  
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutStart(u32 dwId);

/** 
* @brief      停止输出
* @param[in]  u32 dwId 输出对象id
* @note		  引用全局变量：无  
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutStop(u32 dwId);

/** 
* @brief      设置nat探测包参数
* @param[in]  u32 dwId 输入对象id
* @param[in]  TMSNatProbeProp nat探测包结构体
* @note		  引用全局变量：无
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutSetNatProbeProp(u32 dwId, TMSNatProbeProp *ptNatProbeProp);

/** 
* @brief      释放输出对象
* @param[in]  u32 dwId 输出对象id
* @note		  引用全局变量：无  
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSOutRelease(u32 dwId);



/** 
* @brief      创建对象
* @param[out]  PEVENTPROC pfEventCallBackProc 事件回调
* @param[out]  void* pvContext 上下文指针
* @param[in]   u32 adwMSInId[KD_MAX_NUM_TRACK] 多个输入id对象，数组必须初始化清0之后再填，否则会报错
* @param[out]  u32 dwMSOutId 输出对象id
* @param[out]  u32* pdwId pipline对象id
* @note		  1.引用全局变量：无  
*             2.支持多输入对一输出（ES转PS）
*			  3.PS转ES情况请设置多个一输入对一输出(输入相同输出不同)，ES转ES请使用一输入对一输出
*             4.不能在设置的回调函数中调用mediaswitch接口,可能造成死锁；不能再回调函数中执行耗时操作
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSAddPipeline(PMSEVENTPROC pfEventCallBackProc, void* pvContext, const u32 adwMSInId[KD_MAX_NUM_TRACK], u32 dwMSOutId, u32* pdwId);


/** 
* @brief      释放对象
* @param[in]  u32 dwId 释放pipline对象id
* @note		  1.引用全局变量：无  
*             2.支持多输入对一输出（ES转PS）
*			  3.PS转ES情况请设置多个一输入对一输出(输入相同输出不同)，ES转ES请使用一输入对一输出
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSRemovePipeline(u32 dwId);

/** 
* @brief      释放包数据或帧数据
* @param[in]  void * pData帧数据或者包数据指针（暂未支持帧数据的释放，后续会支持）
* @note		  1.引用全局变量：无             
* @return  	  参见错误码定义
*/
MEDIASWITCH_API MSRESULT MSFreeData(void * pData, EKDDataPackFormat eDataPackFormat);

 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif//  
