/*****************************************************************************
   模块名      : DataDispatch
   文件名      : datadispatch.h
   相关文件    : 
   文件实现功能: DataDispatch模块接口函数定义
                与原有DataSwitch功能一致，内部结构优化，全部采用C++对象执行
                并且支持RTP缓冲，尽最大努力保证重传RTP包只发给需要的目的
                精简原有DataSwitch接口
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/02/28  1.0         梁 黎        创建
******************************************************************************/
/*****************************************************************************
2013.4.17 修改记录，修改人 梁黎
1，Vtdu先删除普通交换规则，最后删除Dump规则，
定位发现残留空的RecvGrp未处理，原因是 RemoveDump 未判断RecvGrp.IsEmpty()并删除之

2，VTDU仅第一次STUN能成功，接口调用方式是 先SetFilterFunc，再调用NatAgent.QueryStun()
原因：DD模块判断RecvGrp.IsEmpty()的条件是：1，普通交换存在；2，DUMP存在。以上二者皆为假
所以有可能第一次AddFilter后创建了RecvGrp，但是在清空普通交换后，RecvGrp就被删除了，导致Filter也被删除
原DataSwitch模块中把Filter和Dump等同处理

修改DD模块，AddFilter时添加DUMP规则，DelFilter时也会删除Dump规则
(实际上Dump规则可以不创建，只需要修改RecvGrp.IsEmpty()，增加Filter != NULL的判断)
即 AddDump和AddFilter等同，RemoveDump和DelFilter等同

3，为了防止DataDispatch在链接时与其他模块符号冲突，内部添加namespace NSDataDispatch
接口不变，也不感知该命名空间

2014.1.15 修改包统计，修改人 梁黎
原先只在RuleSet层面统计包，导致结果不准确，即使一个RuleSet内，不同交换的丢包情况是不一样的
现在针对Rule进行统计，每一路交换的丢包情况都正确统计
本次修改不影响原先逻辑，也不影响效率（内部对重传包的处理取巧了，出于效率考虑）

2014.1.16 增加三种运维告警，修改人 梁黎
增加 丢包，串码流，无码流告警

2014.05.29 增加对Rtcp报文的处理，修改人梁黎
增加逆向传输RTCP的SR/RR报文
增加自定义发送接口，以便vtdu能够定期发送保活报文

2014.8.19 修改人梁黎
Datadispatch支持过滤自己构造的SR/RR包，这种包在平台级联中是无意义的，vtdu需要在平台交换的边缘做保活
Datadispatch支持修改RTP包的SSRC，国标对接一所平台需要，Invite::sdp信令与码流SSRC一致

2014.11.28 修改人 范小钢
1. Datadispatch支持主动防御串码流，通过严格校验交换中的码流地址与码流实际发送地址是否匹配来实现。
   仅仅是u32和u16的两次比较，理论分析对转发性能几乎没有什么影响。该功能可以配置是否开启。
2. 完善dsrule命令的打印，按端口交换时打印出码流的源地址，而不再用*:*代替，便于问题排查。
3. 纯转发模式下增加丢包和重传包错误打印，便于云存储问题排查。
4. 增加dscfg调试命令，用于打印上层设置给ds的配置信息

2014.12.16 修改人 范小钢
rtcp重传序列号与rtp的映射关系采用hashmap实现，替代之前的取模算法，因为取模算法存在
循环问题，使rtcp序列号和实际的rtp包不一致，导致medianet重置缓存区，发起大量的重传请求，
码流的播放效果变差。

2015.02.04 修改人 范小钢
DS在接收数据包时，支持在某个端口代理收包，再交给真正的接收组处理，实现半源半端口的交换模式。

******************************************************************************/
#ifndef _DATA_DISPATCH_H_
#define _DATA_DISPATCH_H_

#include "cbb/osp/osp.h"

/*DataDispatch 返回值*/
#define DSOK    1
#define DSERROR 0

//设置DataDispatch的可靠级别
enum EReliableLevel
{
    RL_TRANS_DATA   = 0,
    RL_PARTIAL_BUFFERING,       //部分缓冲，保证DS和目的直接不广播重传包
    RL_FULL_BUFFERING,          //全缓冲，保证所有网段重传包负载最小，需要消耗2G以上内存
};

/*
0    0    0    0
d    a    v    r/c
第 0 bit上，0表示Rtp，1表示Rtcp，
第 1 bit上，1表示Video，0表示没有
第 2 bit上，1表示Audio，0表示没有
第 3 bit上，1表示Data，0表示没有
*/
enum ESwitchType
{
    ST_INVALID = 0,
    ST_VIDEO_RTP = 2,       //0010
    ST_VIDEO_RTCP = 3,      //0011
    ST_AUDIO_RTP = 4,       //0100
    ST_AUDIO_RTCP = 5,      //0101
    ST_DATA_RTP = 8,        //1000 //如果数据流是单向的，则只用RTP类型
    ST_DATA_RTCP = 9,       //1001
};

//日志级别类型
enum EDSLogLev
{
    emLog_None                  = 0,        //关闭所有打印
    emLog_Error                 = 1,        //错误级别
    emLog_Warning               = 2,        //警告级别
    emLog_Info                  = 3,        //信息（普通）级别
    emLog_All                   = (u32)(~0),//放开所有打印
};

enum EDSAlarmType
{
    emDSAlarm_NO_Alarm  =  0,           //无告警
    emDSAlarm_Lost_Pack =  1,           //丢包，消息体 u32 x 3 ExpectedSeqNO, RecvSeqNO, LostPackRate
    emDSAlarm_Confused_Pack = 2,        //串码流，消息体 u32 x 2N dwSrcIP, wSrcPort地址对
    emDSAlarm_NO_Pack   =  3,           //无码流，仅针对VideoRtp，消息体同第一个参数，vctSwitchSsnIDs
    emDSAlarm_Num,
};

enum EDSRspMsgType						// DS返回给上层的应答消息
{
	emDSRspMsgBegin   = 0,
	emDSGetPacketInfo					// 消息体 TPacks 
};

//RTCP报文类型
enum ERTCPType
{
    emRTCP_SR = 200,                  //Sender Report
    emRTCP_RR = 201,                  //Receiver Report
    emRTCP_SDES = 202,                //Source Describe
    emRTCP_BYE = 203,
    emRTCP_APP = 204,
    emRTCP_Num = 5
};

#define RULESET_MAX_WORK_DELAY  600000  //RuleSet从创建到收到第一个包的最大延长时间，单位ms，用于判断无码流

typedef unsigned int TSwitchSsnID;
#ifndef INVALID_SWITCH_SSN_ID
#define INVALID_SWITCH_SSN_ID (TSwitchSsnID)0
#endif
// 对于最大256kbyte的数据帧而言，小包数 <  96
#ifndef MAX_PACK_NUM_PER_FRAME
#define MAX_PACK_NUM_PER_FRAME 96
#endif

#define DEFAULT_SOCKET_BUFFERSIZE (128*1024*1024)

#ifdef _LINUX_
#define DOMAIN_SOCKET_PATH "/opt/kdm/data/unixsocket/%u-%d"
#endif

/*
一帧视频图像关键帧大约是9个包组成（以D1算）
以30 fps为例，每秒钟大约 30 * 9 = 270
按照目的端CU的重传最大等级来算，最多1840 ms
RingBuffer的最大容量 = 1.84 * 270 = 497 个包，再考虑1080P情况
将RingBuffer最终容量定为 700

一帧音频帧一包就是一帧，最后RingBuffer为100
*/
#define MAX_VIDEORTP_BUFFERING_PACK_NUM 700
#define MAX_AUDIORTP_BUFFERING_PACK_NUM 100

#define			DATASWITCH_PORT_START		21050
#define			DATASWITCH_PORT_END			21099

#define DD_INVALID_SSRC1    ((u32)0)
#define DD_INVALID_SSRC2    (~(u32)0)

struct TPacks
{
	TPacks()
	{
		dwRecv = 0;
		dwSend = 0;
		dwSendFailed = 0;
	}

	u32 dwRecv;             //收到多少包
	u32 dwSend;             //发送多少包
	u32 dwSendFailed;       //发送失败
};

struct TPacksWithTime
{
	TPacksWithTime()
	{
		tTime = 0;
	}

	time_t tTime;
	TPacks tPacks;
};

struct TRtpInfo
{
    TRtpInfo()
    {
        m_wSeqNO = 0;
        m_dwSSRC = 0;
    }
    u32 m_dwSSRC;
    u16 m_wSeqNO;
};


/*
RTCP包分为SR,RR,SDES,BYE,APP等类型的包
目前我们公司采用RTCP中的自定义SDES实现重传请求
因此PFGetRtcpInfo函数只解析我司的SDES类型，//其他类型直接返回，按照交换规则透传处理
2014.5.29 -- by liangli
RTCP交换规则，判断包来源，如果是源PU地址过来的，则逆向转发给所有目的CU
否则按照交换规则透传处理
只有其中SDES包需要记录重传请求
*/
struct TRtcpInfo
{
    TRtcpInfo()
    {
        m_dwSSRC = 0;
        m_byNum = 0;
        //memset(m_awNeedReSendSeqNO, 0, sizeof(m_awNeedReSendSeqNO));
        m_awNeedReSendSeqNO[0] = 0;
        m_emRtcpType = emRTCP_SDES;
        m_isKedaResendReq = 0;
    }
    u32 m_dwSSRC;
    u8  m_byNum;
    u16 m_awNeedReSendSeqNO[MAX_PACK_NUM_PER_FRAME];
    ERTCPType m_emRtcpType;
    u32 m_isKedaResendReq;  //是否科达的重传请求
};

//返回值 true表示是RTP包，需要处理；false是其他包，透传处理
typedef BOOL32 (*PFGetRtpInfo)(const void* pBuf, s32 nSize, TRtpInfo* pInfo);
//返回值 true表明该包是重传包，需要缓存后转发， false是其他RTCP包，透传处理
typedef BOOL32 (*PFGetRtcpInfo)(const void* pBuf, s32 nSize, TRtcpInfo* pInfo);

/*
规格说明：以下接口中，缺省dwIP为网络序，wPort为主机序
*/

/**
 * @func FilterFunc
 * @brief 接收过滤函数
 *
 * 每个接收结点有一个过滤函数，当此结点接收到UDP数据包时执行此函数。
 * 并根据函数的返回值，来动态地决定是否对此数据包转发。
 *
 * @param dwRecvIP        - 接收IP
 * @param wRecvPort      - 接收Port
 * @param dwSrcIP         - 源IP
 * @param SrcPort       - 源Port
 * @param pData          - [in, out]数据包；可以修改；
 * @param uLen           - [in, out]数据包长度；修改后的长度不可超过最大值
 * @return 0, 该数据包有效； 其他值，该数据包无效；
 */	
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, 
  u8* pData, u32 uLen);

/*
告警回调: 当任意一路交换发生丢包，串码流或者无码流（只针对Video RTP类型交换）
*/
typedef u32 (*PFAlarmCallBack)(u32 dwRuleNum, TSwitchSsnID *adwSsnID, ESwitchType eType, EDSAlarmType eAlarm, u32 dwParamNum, u32 *adwParams);

/*
 * 通用消息回调：
 */
typedef void (*PFMsgCallBack)(u32 dwTaskNO, u16 wEvent, void *pvContent, u32 dwLen);

/************************************************************
 * 函数名：dsInit 对应dsCreate
 *
 * 功能：  初始化DataDispatch模块
 *
 * 实现说明： 
 * DataDispatch不再支持“多用户”，去掉DSID
 * 
 * 参数说明：
 * DataDispatch模块设计成处理类RTP数据的框架，内置了分析标准RTP协议的函数，默认采用标准分析
 * 但是如果用户使用的是非标RTP协议，可设定相关的解析函数指针
 * DataDispatch支持以这种插件的形式，通用的处理非标RTP
 * 
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
struct TDsInitInfo
{
    TDsInitInfo()
    {
        eLevel = RL_TRANS_DATA;
        pfGetRtpInfo = NULL;
        pfGetRtcpInfo = NULL;
        //dwSockBuffSize = DEFAULT_SOCKET_BUFFERSIZE;
        dwRecvBuffSize = DEFAULT_SOCKET_BUFFERSIZE;;
        dwSendBuffSize = DEFAULT_SOCKET_BUFFERSIZE;;
        dwVideoRtpPackBuffSize = MAX_VIDEORTP_BUFFERING_PACK_NUM;
        dwAudioRtpPackBuffSize = MAX_AUDIORTP_BUFFERING_PACK_NUM;
        dwCPUAffinity = 1024;   //默认不绑定任何核
        bIsDefenseCrossStreamEnable = FALSE;
        pfAlarmCallBack = NULL;
		pfMsgCallBack   = NULL;
    }
    EReliableLevel eLevel;
    PFGetRtpInfo pfGetRtpInfo;
    PFGetRtcpInfo pfGetRtcpInfo;
    //u32 dwSockBuffSize;
    u32 dwRecvBuffSize;
    u32 dwSendBuffSize;
    u32 dwVideoRtpPackBuffSize;
    u32 dwAudioRtpPackBuffSize;
    u32 dwCPUAffinity;              //设置DataDispatch线程绑定CPU从0起第几个核，如果超过CPU核数则不绑定
    BOOL32 bIsDefenseCrossStreamEnable;//是否开启主动防御串码流
    PFAlarmCallBack pfAlarmCallBack;
	PFMsgCallBack   pfMsgCallBack;
};

API u32 dsInit(const TDsInitInfo &tInfo);

API u32 dsSetIsDefenseCrossStreamEnable(const BOOL32 bVar);

/************************************************************
 * 函数名：dsDestroy
 *
 * 功能： 终止DataDispatch的运行
 *
 * 实现说明：
 * 一次删除一个用户，直到所有用户都删除，所有资源才全释放。
 *
 * 参数说明：
 * 
 * 
 * 返回值：
 ************************************************************/
API void dsDestroy( );

/************************************************************
 * 函数名：dsAddDump
 *
 * 功能： 增加Dump规则
 * DUMP规则：从指定地址接收的数据包不被转发。
 * 如果一个接收地址只有DUMP规则，则接收的数据包不被转发；一般用于避免系统发送错误地址应答
 * 如果在Dump地址上添加其他转发规则，则按照其他规则转发。DUMP失效
 * Dump规则会被保留，需用户手动删除
 *
 * 实现说明：
 * DUMP规则允许被创建多次，因为它与其他规则是可以并存的，
 * 包括它自身；
 * 重复创建的DUMP规则会被覆盖
 *
 * 参数说明：
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP，如果本机有多个IP，可以分别指定，如果为0，表示指定ANYADDR
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsAddDump(u32 dwRecvIP, u16 wRecvPort);

/************************************************************
 * 函数名：dsRemoveDump
 *
 * 功能： 删除Dump规则
 *
 * 实现说明：
 * 
 * 参数说明：
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP，注意与dsAddDump对应
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsRemoveDump(u32 dwRecvIP, u16 wRecvPort);

/************************************************************
 * 函数名：dsAddP2P
 * 
 * 功能： 增加多对一的转发规则，对于平台业务就是P2P交换规则
 * 
 * 实现说明：
 *
 * 参数说明：
 * @param tSsnID        - 交换的会话标识
 * @param eSwithType    - 交换类型，DS需要交换类型来做RTCP重传缓冲
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP，同时也是网络接口
 * @param wRecvPort     - 接收Port，即接收数据包的目的Port
 * @param dwSrcIP       - 接收数据包的源IP
 * @param wSrcPort      - 接收数据包的源Port
 * @param dwSendtoIP    - 转发目的IP
 * @param wSendtoPort   - 转发目的Port
 * @param dwSendBindIP  - 发送绑定IP，采用RawSocket伪装，为0表示不伪装
 * @param wSendBindPort - 发送绑定Port，0表示不伪装
 - P2P类型的RTCP可能存在规则合并，而P2P的RTCPRule交换无法区分RTCP请求，（不关心SrcAddr）
 - 所以需要记录RTCP请求的源地址，用以判断RTCP请求具体来自哪个目的端
 - 添加RTP规则时，可以不填写该地址
 * @param dwRtcpSrcIP   - 目的端发送RTCP请求的地址，例如CU用本机IP + 60129发送Video RTCP请求
 * @param wRtcpSrcPort  - 地址需要是穿NAT之后的地址，等于是(M2P)交换地址匹配
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsAddP2P( TSwitchSsnID tSsnID,
                    ESwitchType eSwithType,
                    u32  dwRecvIP,
                    u16  wRecvPort,
                    u32  dwSrcIP ,
                    u16  wSrcPort,
                    u32  dwSendtoIP,
                    u16  wSendtoPort,
                    u32  dwSendBindIP,
                    u16  wSendBindPort,
                    u32  dwRtcpSrcIP,
                    u16  wRtcpSrcPort);

/************************************************************
 * 函数名：dsRemoveP2P
 *
 * 功能： 删除指定的多对一规则
 *
 * 实现说明：
 * 
 * 参数说明：
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 * @param dwSendtoIP    - 转发目的IP
 * @param wSendtoPort   - 转发目的Port
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsRemoveP2P( TSwitchSsnID tSsnID, ESwitchType eSwitchType );

/************************************************************
 * 函数名：dsAddM2P
 *
 * 功能： 增加按源转发的规则，在平台业务中M2P规则需要匹配源地址
 * 
 * 按源转发：使用此规则时，接收数据包的源地址必须与规则中的源
 * 地址相等。每次转发时，都要先根据数据包中的源地址查找转发规则
 * 如果找不到，则使用DUMP规则。
 * 注意：该接口支持多点到一点。如果dwSrcIP和wSrcPort都为零，
 * 它就完全等同于dsAddP2P
 *
 * 实现说明：
 *
 * 参数说明：
 * @param tSsnID        - 交换的会话标识
 * @param eSwithType    - 交换类型，DS需要交换类型来做RTCP重传缓冲
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP，同时也是网络接口
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 * @param dwSrcIP       - 接收数据包的源IP
 * @param wSrcPort      - 接收数据包的源Port
 * @param dwSendtoIP    - 转发目的IP
 * @param wSendtoPort   - 转发目的Port
 * @param dwSendBindIP  - 发送绑定IP，采用RawSocket伪装，为0表示不伪装
 * @param wSendBindPort - 发送绑定Port，0表示不伪装
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/      
API u32 dsAddM2P( TSwitchSsnID tSsnID,
                  ESwitchType eSwitchType,
                  u32  dwRecvIP,
                  u16  wRecvPort,
                  u32  dwSrcIP ,
                  u16  wSrcPort,
                  u32  dwSendtoIP,
                  u16  wSendtoPort,
                  u32  dwSendBindIP,
                  u16  wSendBindPort);

/************************************************************
 * 函数名：dsRemoveM2P
 * 
 * 功能： 删除指定的按源转发的规则
 *
 * 实现说明：
 * 
 * 参数说明：
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 * @param dwSrcIP       - 接收数据包的源IP
 * @param wSrcPort      - 接收数据包的源Port
 * @param dwSendtoIP    - 转发目的IP
 * @param wSendtoPort   - 转发目的Port
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/      
API u32 dsRemoveM2P( TSwitchSsnID tSsnID, ESwitchType eSwitchType );

/************************************************************
 * 函数名：dsSetRecvProxy
 * 
 * 功能： 为RecvGrp设置一个码流包接收代理，模拟出半源半端口的业务场景
 * 
 * 实现说明：
 *
 * 参数说明：
 * @param tSsnID        - 交换的会话标识
 * @param eSwithType    - 交换类型
 * @param dwSrcIP       - 接收数据包的源IP
 * @param wSrcPort      - 接收数据包的源Port
 * @param dwProxyRecvIP      - Proxy接收IP
 * @param wProxyRecvPort     - Proxy接收Port
 * @param dwRealRecvIP       - 实际接收IP
 * @param wRealRecvPort      - 实际接收Port

 - ProxyRecv地址先收包，然后投递给真实的接收组处理
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsSetRecvProxy( TSwitchSsnID tSsnID,
                    ESwitchType eSwithType,
                    u32  dwSrcIP ,
                    u16  wSrcPort,
                    u32  dwProxyRecvIP,
                    u16  wProxyRecvPort,
                    u32  dwRealRecvIP,
                    u16  wRealRecvPort
                    );

/************************************************************
 * 函数名：dsSetFilterFunc
 * 
 * 功能： 设置接收过滤函数
 *
 * 注意：函数指针可以为空，表示清除原有的设置；
 * 另外，由于是在DataDispatch的线程中调用此函数的，函数必须是全局的，并且线程安全
 * 可以在FilterFunc中修改回调包内容
 *
 * 实现说明：
 * 
 * 参数说明：
 * @param dwRecvIP      - 接收IP，即接收数据包的本机IP      //与规则的本机地址对应
 * @param wRecvPort     - 接收Port，即接收数据包的本机Port
 * @param ptFunc        - 过滤函数指针，如果为NULL表示删除过滤
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsSetFilterFunc( u32 dwRecvIP, 
                         u16 wRecvPort, 
                         FilterFunc ptFunc);

//目前暂不支持socks5用户数据
API u32 dsSetUserData(  u32  dwLocalIP,
                        u16  wLocalPort,
                        u32  dwSrcIP ,
                        u16  wSrcPort,
                        u32  dwDstIP,
                        u16  wDstPort,
                        BOOL32 bSend,
                        u8* pbyUserData,
                        s32 nUserLen);

/************************************************************
 * 函数名：dsRemoveBySsnID
 * 
 * 功能： 删除同一个交换会话内所有的转发规则，无论M2P或P2P
 * 
 * 实现说明：DataDispatch模块紧密贴合平台交换业务，内部记录了每条交换规则的会话ID
 * 该接口遍历所有交换规则，删除含带相同SsnID的交换规则
 * 
 * 参数说明：
 * @param tSsnID        - 交换会话ID，例如一次交换VideoRTP，VideoRTCP，AudioRTP，AudioRTCP四条规则拥有相同SsnID
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsRemoveBySsnID( TSwitchSsnID tSsnID );

/************************************************************
 * 函数名：dsRemoveAll
 * 
 * 功能： 删除所有的转发规则
 * 
 * 实现说明：
 * 
 * 参数说明：
 * @param bKeepDump     - 是否保留所有的dump规则，默认不保留
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsRemoveAll( BOOL32 bKeepDump = FALSE );


/************************************************************
 * 函数名：dsCheckRule
 * 
 * 功能： 检查所有规则
 * 
 * 实现说明：类似无码流这种状态，DataDispatch内部只会做一下标记而不会主动触发告警
 * 需要上层定期调用接口来轮询
 * 
 * 参数说明：
 * 
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsCheckRule();

/************************************************************
 * 函数名：dsGetPacketInfo
 * 
 * 功能： 获取指定交换的收发包统计信息
 * 
 * 实现说明：指定交换的SsnID，返回其当前最新的收发包数据信息
 * 
 * 参数说明：
 * 
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsGetPacketInfo(u32 dwTaskNO, TSwitchSsnID tSsnID, ESwitchType eType);

/************************************************************
 * 函数名：dsCreateRtcpPacket
 * 
 * 功能： 创建一个RTCP报文
 * 
 * 实现说明：目前只支持SR / RR保活报文，
 * 
 * 参数说明：
 * @ [IN]tSsnID RTCP交换的SwitchSsnID
 * @ [IN]eSwithType 交换类型，具体是ST_VIDEO_RTCP 还是 ST_AUDIO_RTCP ，每次只填写一种
 * @ [IN]emRtcpType 创建的保活包类型
 * @ [OUT]abyBuf 输出Buf
 * @ [IN]dwBufLen 输入Buff最大长度
 * 返回值：
 * @return 创建的BUF实际长度(保活报文一般不大于8字节)
 ************************************************************/
API u32 dsCreateRtcpPacket(TSwitchSsnID tSsnID,
                            ESwitchType eSwithType,
                            ERTCPType emRtcpType,   //目前只支持SR/RR保活包
                            u8 *abyBuf, u32 dwBufLen);

API BOOL32 dsIsRtcpPacketCreatedByDataDispatch(u32 *pdwBuf, u32 dwBufLen);

/************************************************************
 * 函数名：dsSendTo
 * 
 * 功能： 给上层提供某条规则的套接字发送接口，便于上层直接发送RTCP保活报文等功能
 * 
 * 实现说明：上层指定某一条具体的交换规则（tSsnID + eSwithType），指定发送的目的地址即可
 *          该接口赋予vtdu最大的灵活性，所有的信息基本上都由vtdu提供
 * 
 * 参数说明：略，备注：全部都是IN参数，如无特殊说明，所有IP都是网络序，所有Port都是主机序
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsSendTo(TSwitchSsnID tSsnID,
                ESwitchType eSwithType,
                u32 dwDstIP, u16 wDstPort,
                u8 *abyBuf, u32 dwBufLen);

/************************************************************
 * 函数名：dsSetSpecifiedSSRC
 *
 * 功能： 指定VideoRtp / AudioRtp的SSRC，如果传入非法值则关闭替换SSRC
 *
 * 实现说明：在发送RTP包之间修改包头SSRC
 * 
 * 参数说明：
 * @param tSsnID        - 交换流水号
 * @param eSwitchType   - 交换类型，与流水号一起唯一确定一条交换规则，只能是VideoRtp / AudioRtp
 * @param dwSSRC        - 指定的目标SSRC，主机序；0和FFFFFFFF是非法值，如果传入该值表明关掉替换功能
 *
 * 返回值：
 * @return DSOK: 表示成功 ; DSERROR: 表示失败 ;
 ************************************************************/
API u32 dsSetSpecifiedSSRC( TSwitchSsnID tSsnID, ESwitchType eSwitchType, u32 dwSSRC );

//////////////////////////////////////////////////////////////////////////
//调试接口
/************************************************************
 * 函数： dsinfo
 * 功能： 显示所有的转发规则，及所有正在监听的端口 
 * 输入：
 * 输出：
 * 返回：
 ************************************************************/
API void dsinfo();

/************************************************************
 * 函数： dsrule
 * 功能： 显示所有的转发规则，及所有正在监听的端口 
 * 输入：dwSpecifiedSwitchSsnID = 0 打印所有规则
        dwShowSwitchConnect = 1 打印指定的规则
        新方式会将同一个SwitchSession的交换放在一起打印，更加直观
 * 输出：
 * 返回：
 ************************************************************/
API void dsrule(u32 dwSpecifiedSwitchSsnID);

/************************************************************
 * 函数： dsruleset
 * 功能： 打印某个RecvGrp上的所有RuleSet信息
 * 输入： 
 * 输出：
 * 返回：
 ************************************************************/
API void dsruleset(u32 dwLocalPort);

/************************************************************
 * 函数： dsrp
 * 功能： 打印某个recvproxy上的相关信息
 * 输入： 
 * 输出：
 * 返回：
 ************************************************************/
API void dsrp(u32 dwLocalPort);

/************************************************************
 * 函数： dstrace
 * 功能： 跟踪打印某条交换
 * 输入： 
 * 输出：
 * 返回：
 ************************************************************/
API void dstrace(u32 dwSwitchSsnID, u32 dwSwitchType);

/************************************************************
 * 函数： dstraceoff
 * 功能： 关闭所有跟踪打印
 * 输入： 某些时候我们忘记了trace的交换SsnID，用此命令可以直接关闭所有trace
 * 输出：
 * 返回：
 ************************************************************/
API void dstraceoff();

/************************************************************
 * 函数： dsrtcp
 * 功能： 打印某个本机地址上的RTCP请求
 * 输入： 
 * 输出：
 * 返回：
 ************************************************************/
API void dsrtcp(u32 dwSwitchSsnID, u32 dwSwitchType);

/************************************************************
 * 函数： dsrel
 * 功能： 打印所有的TRuleRelevance列表，查看是否有资源泄露
 * 输入： 
 * 输出：
 * 返回：
 ************************************************************/
API void dsrel();

/************************************************************
 * 函数： dsloglev
 * 功能： 设置日志打印级别
 * 输入： dwLev 打印级别
 * 输出：
 * 返回：
 ************************************************************/
API void dsloglev(u32 dwLev);

/************************************************************
 * 函数： dsdetaillog
 * 功能： 在INFO级别下打印详细信息
 * 输入： bPrint TRUE表示打印
 * 输出：
 * 返回：
 ************************************************************/
API void dsdetaillog(u32 bPrint);

/************************************************************
 * 函数： dsver
 * 功能： 显示Dataswitch的版本信息
 * 输入：
 * 输出：
 * 返回：
 ************************************************************/      
API void dsver();

/************************************************************
 * 函数： dshelp
 * 功能： 显示Dataswitch所提供的命令的帮助信息
 * 输入：
 * 输出：
 * 返回：
 ************************************************************/      
API void dshelp();

/************************************************************
 * 函数： dscfg
 * 功能： 显示Dataswitch的基本配置
 * 输入：
 * 输出：
 * 返回：
 ************************************************************/      
API void dscfg();

#endif

//end of file
