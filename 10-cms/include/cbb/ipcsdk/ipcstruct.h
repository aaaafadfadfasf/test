//////////////////////////////////////////////////////////////////////////
// ipcstruct.h 
#ifndef IPCSTRUCT_H_
#define IPCSTRUCT_H_

#include "ipcdefine.h"

/*************************************************
/	                  搜索前端
/*************************************************/
// 网络参数
typedef struct tagIPCNetParam
{
	unsigned long   dwIpAddr;	                                      // IP地址(网络序)
	unsigned short  wPort;		                                      // 端口号(1024-65535)
} TIPC_NET_PARAM, *PTIPC_NET_PARAM;

//GUID结构
typedef struct tagIPCVsipGuid
{
	unsigned long	Data1;                                            // 对于设备GUID,Data1必须为0
   	unsigned short	Data2;
	unsigned short	Data3;
   	unsigned char	Data4[8];
} TIPC_VSIP_GUID, *PTIPC_VSIP_GUID;

// 搜索结果参数
typedef struct tagIPCDiscoveryCBInfo
{
	TIPC_VSIP_GUID	tGuid;							                  // 设备GUID值(只读)
	char	        achName[MAX_STR_LEN_64+1];			              // 设备名
	char	        achTypeName[MAX_STR_LEN_64+1];		              // 设备类型名(只读)
	unsigned char   byDevType;							              // 设备类型(IPC_ENCODER等)
	unsigned long	dwUnitIpAddr;						              // 设备IP地址
	unsigned short	wVsipPort;							              // VSIP端口号
} TIPC_DISCOVERY_CB_INFO, *PTIPC_DISCOVERY_CB_INFO;

/* 搜索结果回调 */
typedef void (*IPCSDK_SEARCH_CB)(TIPC_DISCOVERY_CB_INFO atUnitParam[], unsigned short wUnitNum, void *pContext);

// 链接类型
typedef struct tagIPCConnType
{
	unsigned char byUdpUnicast;                                       // 支持为1，不支持为0，下同
	unsigned char byUdpMulticast;
	unsigned char byUdpBroadcast;
	unsigned char byTcpServer;
	unsigned char byTcpClient;
	unsigned char bySSL;
} TIPC_CON_TYPE, *PTIPC_CON_TYPE;

/*************************************************
/	                  参数配置
/*************************************************/
// 前端基本参数结构
typedef struct tagIPCSysParam
{
	char		   achName[MAX_STR_LEN_64+1];			              // 设备名
	char		   achTypeName[MAX_STR_LEN_64+1];		              // 设备类型名(只读)
	unsigned char  byDevType;							              // 设备类型(IPC_ENCODER等)
	char 		   abySeriesName[16+1];					              // 设备系列名
} TIPC_PU_BASE_PARAM, *PTIPC_PU_BASE_PARAM;

// 系统公共参数结构
typedef struct tagIPCSysPubParam
{
	unsigned char   byDetPacketSend;                                  // 是否发送NAT探测包
	unsigned long	dwUnitIpAddr;                                     // 设备IP地址
	unsigned short	wVsipPort;                                        // VSIP端口号
	unsigned long	dwSubnetMask;                                     // 设备子网掩码(网络序)
	unsigned long	dwGateWay;                                        // 网关地址(网络序)
	char	        achVersion[MAX_VER_STRING_LEN+1];                 // 设备版本号(只读)
	unsigned long	dwDiscoveryIpAddr;                                // 搜索的注册地址
	unsigned long	dwVsipMultiIpAddr;                                // 搜索的组播地址
	char	        achWebUsername[MAX_STR_LEN_64+1];                 // 嵌入式Web用户名
	char	        achWebPassword[MAX_STR_LEN_64+1];                 // 嵌入式Web密码
	char	        achWebGuestname[MAX_STR_LEN_64+1];                // 嵌入式Web操作员用户名
	char	        achWebGuestPassword[MAX_STR_LEN_64+1];            // 嵌入式Web操作员密码
	unsigned short	wAlarmContinuance;                                // 告警恢复时间(秒)
	unsigned char   bySubAreaType;                                    // 分区格式(只读 IPCSDK_SINGLE_SUBAREA 单分区
	                                                                  // IPCSDK_MULTI_SUBAREA 多分区)
	unsigned char   byMaxTranferNum;                                  // 最大直传路数
	char            abyPuid[MAX_VER_STRING_LEN];                      // 前端ID
	char            abyPassword[MAX_VER_STRING_LEN];                  // 验证密码
	unsigned char   byIsDecCombined;                                  // 是否是解码合成图象
	unsigned short  wAppType;							              // 应用类型 0-静态分配  1-DHCP
	unsigned char   byMatrixValid;						              // 是否接入矩阵
	unsigned char   byDecMPStyle;		                              // 画面解码模式
    char            achSoftVer[MAX_STR_LEN_64 + 1];
    char            achHardVer[MAX_STR_LEN + 1];
	char            achPtzSoftVer[MAX_STR_LEN_64  + 1];
} TIPC_SYS_PUB_PARAM, *PTIPC_SYS_PUB_PARAM;

#ifdef WIN32
#pragma pack(push, 1)
#endif
// 域名信息参数结构
typedef struct tagIPCDomainNameInfo
{
	unsigned char   byState;                                          // 注册状态: 0-离线; 1-在线; 2-正在更新
	unsigned char   byDomainNameNum;                                  // 返回的域名数量
	char            abyDomainName[MAX_STR_LEN][MAX_STR_LEN_64+1];     // 域名名称
	unsigned long   dwDonator;                                        // 帐户类型：免费，收费
} TIPC_DOMAIN_NM_INFO, *PTIPC_DOMAIN_NM_INFO;

// 动态域名服务参数结构
typedef struct tagIPCDDNSCfg
{
	unsigned char byServiceNum;                                       // 支持服务个数
	unsigned char abyService[2];                                      // 服务类型，ex:3322，花生壳，dyndns
	unsigned char bySrvType;                                          // 当前服务类型
	char achDnsServer[16];                                            // dns server ip地址
	char achUsername[128];                                            // 认证用户名
	char achPassword[128];                                            // 认证密码
	char achHostname[128];                                            // 域名
	char achInterface[16];                                            // 本地网口地址
	TIPC_DOMAIN_NM_INFO  tPuDomain;                                   // 域名信息
} TIPC_DDNS_CFG, *PTIPC_DDNS_CFG;

// DNS配置
typedef struct tagIPCDnsCfg
{
	unsigned long dwDnsAddr;                                          // DNS地址
	unsigned long dwDnsBackupAddr;                                    // DNS备份地址
} TIPC_DNS_CFG, *PTIPC_DNS_CFG;

// 本地网络配置参数
typedef struct tagIPCLocalNetCfg
{
	unsigned char   byAddrMode;                                       // 地址使用方式，固定IP或者DHCP
	unsigned char   byDhcpEthId;                                      // 地址DHCP的网口号
	unsigned char   byUseDhcpGateway;                                 // 是否使用DHCP的网关
	unsigned long	dwIpAddr;				                          // 设备IP地址(默认:IPCSDK_DEFCFG_IPADDR)
	unsigned long	dwSubnetMask;			                          // 设备子网掩码(默认:IPCSDK_DEFCFG_NETMASK)
	unsigned long   dwGateWay;				                          // 网关地址(默认:IPCSDK_DEFCFG_GATEWAY)
	unsigned char	abyMacAddr[6];			                          // mac地址(内部使用)
	unsigned char   byUseDdns;                                        // 是否使用DDNS
	TIPC_DDNS_CFG   tDdnsCfg;                                         // ddns信息
	unsigned char   byChangeMask;			                          // 是否改变mask,目前用于整个结构是否修改(配置模块)
	TIPC_DNS_CFG	tDnsCfg;                                          // DNS信息
} TIPC_LOCAL_NET_CFG, *PTIPC_LOCAL_NET_CFG;
#ifdef WIN32
#pragma pack(pop)
#endif

/*编码通道主从关系*/
typedef struct tagIPCVideoEncChanRelationItem
{
    unsigned char       byChannelId;                                  // 编码通道Id
    unsigned char       byMainChannelId;                              // 对应主编码通道Id
} TIPC_VIDEO_ENC_CHAN_RELATION_ITEM, *PTIPC_VIDEO_ENC_CHAN_RELATION_ITEM;

typedef struct tagIPCVideoEncChanRelation
{
    unsigned char                       byChannelNum;                 // 通道数
    TIPC_VIDEO_ENC_CHAN_RELATION_ITEM   atChannel[32];                // 通道信息	
} TIPC_VIDEO_ENC_CHAN_RELATION, *PTIPC_VIDEO_ENC_CHAN_RELATION;

// 视频编码网络参数
typedef struct tagIPCVideoEncNetParam
{
	unsigned char			byEncID;				                  // 视频编码通道号
	unsigned char			byInitStreamState;		                  // 码流初始化状态 0-不发送, 1-发送
	unsigned char			byCurStreamState;		                  // 码流当前状态 0-未发送, 1-已发送
	TIPC_CON_TYPE	        tSupportedConnType;		                  // 支持的码流链接类型
	unsigned char			byConnectionType;		                  // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned long			dwStreamIpAddr;			                  // 码流传输地址
	unsigned short			wStreamPort;			                  // 码流传输端口
} TIPC_VIDEC_ENC_NET_PARAM, *PTIPC_VIDEC_ENC_NET_PARAM;

// 视频压缩格式
typedef struct tagIPCVideoType
{
	unsigned char bySN4;                                              // 支持为1，不支持为0，下同
	unsigned char byMPEG4;
	unsigned char byH261;
	unsigned char byH263;
	unsigned char byH264;
} TIPC_VIDEO_TYPE, *PTIPC_VIDEO_TYPE;

// 视频图象分辨率
typedef struct tagIPCVideoResolution
{
	unsigned char byAuto;                                             // 支持为1，不支持为0，下同
	unsigned char byQCIF;
	unsigned char byCIF;
	unsigned char by2CIF;
	unsigned char by4CIF;
	unsigned char byVGA;
	unsigned char byQQCIF;
	unsigned char byQVGA;
	unsigned char by720P;
	unsigned char by1080P;
	unsigned char byQXGA;
} TVIDEO_IMG_RESOLUTION, *PTVIDEO_IMG_RESOLUTION;

// 视频分辨率格式
typedef struct tagIPCSPVideoResolution
{
	TVIDEO_IMG_RESOLUTION tSN4;
	TVIDEO_IMG_RESOLUTION tMEPG4;
	TVIDEO_IMG_RESOLUTION tH261;
	TVIDEO_IMG_RESOLUTION tH263;
	TVIDEO_IMG_RESOLUTION tH264;
} TIPC_SP_VIDEO_RESOLUTION, *PTIPC_SP_VIDEO_RESOLUTION;

// 视频编码参数结构
typedef struct tagIPCVideoEncParam
{
	unsigned char					 byEncID;				          // 视频通道号
	TIPC_VIDEO_TYPE			         tSupportedVideoType;	          // 支持的视频格式(只读)
	TIPC_SP_VIDEO_RESOLUTION         tSupportedVideoResolution;       // 支持的视频分辨率(只读)
	unsigned char					 byVideoType;			          // 视频解码格式(IPCSDK_VIDEO_TYPE_SN4等)
	unsigned char					 byVideoResolution;		          // 图象分辨率(IPCSDK_VIDEO_RESOLUTION_AUTO等)
	unsigned char					 byFrameRate;			          // 帧率 1-25
	unsigned char					 byQuality;				          // 图象质量 1-2 (质量优先/速度优先)
	unsigned long					 dwBitRate;				          // 编码比特率 64K-4M
	unsigned char					 byMaxQuantization;		          // 最大量化因子 1-31
	unsigned char					 byMinQuantization;		          // 最小量化因子 1-31
	unsigned short					 wInterval;				          // 关键帧间隔 25-2500
	char					         chBrightness;			          // 亮度 -128～127
	char					         chContrast;				      // 对比度 -128～127
	char					         chSaturation;			          // 饱和度 -128～127
    unsigned char			         byBitRateType;                   // 0:定码率, 1:变码率
	unsigned char		             byStreamType;				      // 0:RTP, 1:TS
	unsigned char		             bySharpness;				      // 锐度 0: not support, 1~4: available
	unsigned char                    byVideoFrame;                    // 场帧
	unsigned char                    byMaxFrameRate;                  // 支持的最大帧率(只读)
	unsigned char 	                 byEncRankId;                     // 图像编码协议等级
	unsigned char                    byEncFormat;                     // 图像编码格式
} TIPC_VIDEO_ENC_PARAM, *PTIPC_VIDEO_ENC_PARAM;

// 视频编码公共参数结构
typedef struct tagIPCVideoEncPubParam
{
	unsigned char		byMaxVideoEncNum;	                          // 最大可用视频编码通道数量(只读)
	unsigned char		byVideoEncUsed;		                          // 启用的视频编码通道数
	unsigned char		abyVideoEncCap[MAX_VIDEOIN_PORT_NM_LEN];	  // 支持的视频编码能力(只读)
	unsigned char		byMaxVideoEncCapNum;	                      // 支持的编码能力个数(只读)
} TIPC_VIDEO_ENC_PUB_PARAM, *PTIPC_VIDEO_ENC_PUB_PARAM;

// 视频编码字幕参数结构
typedef struct tagIPCVideoEncOSDParam
{
	unsigned char		byEncID;				                      // 编码通道号
	unsigned char		byShowTimeState;		                      // 是否显示时间 0-不显示, 1-显示
	unsigned char		byOSDState;				                      // 编码字幕状态 0-无字幕, 1-有字幕
} TIPC_VIDEO_ENC_OSD_PARAM, *PTIPC_VIDEO_ENC_OSD_PARAM;

// 视频编码视频源端口参数结构
typedef struct tagIPCVideoEncInputPortParam
{	
	unsigned char	byEncID;				                          // 编码通道
															          // 有几个视频输入端口数组里就有几个项可用，而它的值则表示这个视频输入ID;
															          // 通常视频输入端口N对应视频源ID也是N
	unsigned char	abyValidVideoInputPort[MAX_VIDEOIN_PORT_NUMBER];  // 可用的视频输入端口号(只读)	
	unsigned char	byValidVideoInputPortNum; 	                      // 可用的视频输入端口数量(只读)
	unsigned char	byVideoInputPort;		                          // 当前使用的视频输入端口号
} TIPC_VIDEO_ENC_INPUT_PT_PARAM, *PTIPC_VIDEO_ENC_INPUT_PT_PARAM;

// 视频解码网络参数
typedef struct tagIPCVideoDecNetParam
{
	unsigned char			byDecID;				                  // 解码通道号
	unsigned char			byInitStreamState;		                  // 码流初始化状态 0-不发送, 1-发送
	unsigned char			byCurStreamState;		                  // 码流当前状态 0-未发送, 1-已发送
	TIPC_CON_TYPE	        tSupportedConnType;		                  // 支持的码流链接类型
	unsigned char			byConnectionType;		                  // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned long			dwStreamIpAddr;			                  // 码流传输地址
	unsigned short			wStreamPort;			                  // 码流传输端口
} TIPC_VIDEO_DEC_NET_PARAM, *PTIPC_VIDEO_DEC_NET_PARAM;

// 视频解码参数结构
typedef struct tagIPCVideoDecParam
{
	unsigned char			byDecID;				                  // 解码通道
	TIPC_VIDEO_TYPE	        tSupportedVideoType;	                  // 支持的视频格式(只读)
	unsigned char			byPlayerBufferNum;		                  // 播放缓冲区数 6-50
	unsigned char			byPlayerOriginBufNum;	                  // 播放起始缓冲区号 0-10
	unsigned char			byVideoType;			                  // 视频解码格式(IPCSDK_VIDEO_TYPE_SN4等)
	unsigned char			byDecDealLev;			                  // 图像后处理级别(0～3级,0表示没有)
	unsigned char		    bySharpFactor; 
} TIPC_VIDEO_DEC_PARAM, *PTIPC_VIDEO_DEC_PARAM;

// 视频解码公共参数结构
typedef struct tagIPCVideoDecPubParam
{	
	unsigned char		byDecID;					                  // 视频解码通道
	unsigned char		byVGAOutSupported;			                  // 是否支持VGA输出(只读) 0-不支持, 1-支持
	unsigned char		byVideoOutputType;			                  // 视频输出端口类型(IPC_VIDEO_OUT_TYPE_C等)
	unsigned char		byVGAFrequency;				                  // VGA输出刷新频率(IPC_VGA_OUT_FREQUENCY_60等)
	unsigned char		byStarveMode;				                  // 无视频的显示方式
} TIPC_VIDEO_DEC_PUB_PARAM, *PTIPC_VIDEO_DEC_PUB_PARAM;

// 音频编码网络参数
typedef struct tagIPCAudioEncNetParam
{
	unsigned char			byAEncID;				                  // 音频编码通道号
	unsigned char			byInitStreamState;		                  // 码流初始化状态 0-不发送, 1-发送
	unsigned char			byCurStreamState;		                  // 码流当前状态 0-未发送, 1-已发送
	TIPC_CON_TYPE	        tSupportedConnType;		                  // 支持的码流链接类型
	unsigned char			byConnectionType;		                  // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned long			dwStreamIpAddr;			                  // 码流传输地址
	unsigned short			wStreamPort;			                  // 码流传输端口
} TIPC_AUDIO_ENC_NET_PARAM, *PTIPC_AUDIO_ENC_NET_PARAM;

// 音频压缩格式
typedef struct tagIPCAudioType
{
	unsigned char byPCMA;                                             // 支持为1，不支持为0，下同
	unsigned char byPCMU;
	unsigned char byGSM;
	unsigned char byG729;
	unsigned char byADPCM;
	unsigned char byG722;
    unsigned char byG7221C;
	unsigned char byAACLC;
} TIPC_AUDIO_TYPE, *PTIPC_AUDIO_TYPE;

// 音频编码参数结构
typedef struct tagIPCAudioEncParam
{
	unsigned char		byAEncID;				                      // 音频编码通道
	TIPC_AUDIO_TYPE	    tSupportedAudioType;	                      // 支持的数据压缩格式(只读)
	unsigned char		byAudioType;			                      // 音频数据格式(IPCSDK_AUDIO_TYPE_PCMA等)
	unsigned char		byAudioInputType;		                      // 音频输入类型(IPCSDK_AUDIO_INPUTTYPE_LINEIN等)
	unsigned char		abyAudioInputType[MAX_VIDEOIN_PORT_NM_LEN];   // 支持的输入类型
	unsigned char		byAudioMaxInputType;	                      // 输入类型数量
	unsigned char		byVolume;				                      // 音量 0-25
	unsigned char		byAECState;				                      // 回声抵消状态 0-无效, 1-有效
	unsigned char		byDumbState;			                      // 哑音状态 0-无效, 1-有效
} TIPC_AUDIO_ENC_PARAM, *PTIPC_AUDIO_ENC_PARAM;

// 音频解码网络参数
typedef struct tagIPCAudioDecNetParam
{
	unsigned char			byADecID;				                  // 音频解码通道号
	unsigned char			byInitStreamState;		                  // 码流初始化状态 0-不发送, 1-发送
	unsigned char			byCurStreamState;		                  // 码流当前状态 0-未发送, 1-已发送
	TIPC_CON_TYPE	        tSupportedConnType;		                  // 支持的码流链接类型
	unsigned char			byConnectionType;		                  // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned long			dwStreamIpAddr;			                  // 码流传输地址
	unsigned short			wStreamPort;			                  // 码流传输端口
} TIPC_AUDIO_DEC_NET_PARAM, *PTIPC_AUDIO_DEC_NET_PARAM;

// 音频解码参数结构
typedef struct tagIPCAudioDecParam
{
	unsigned char			byADecID;				                  // 音频解码通道
	TIPC_AUDIO_TYPE	        tSupportedAudioType;	                  // 支持的数据压缩格式(只读)
	unsigned char			byVolume;				                  // 音量 0-25
	unsigned char			byMuteState;			                  // 静音状态 0-无效, 1-有效
	unsigned char	        byMixRecord;			                  // 混音录像参数
} TIPC_AUDIO_DEC_PARAM, *PTIPC_AUDIO_DEC_PARAM;

// 串口类型
typedef struct tagIPCSerialType
{
	unsigned char byRS232;                                            // 支持为1，不支持为0，下同
	unsigned char byRS422;
	unsigned char byRS485;
} TIPC_SERIAL_TYPE, *PTIPC_SERIAL_TYPE;

// 串口参数结构
typedef struct tagIPCSerialPortParam
{
	unsigned char			 bySerialPortID;			              // 串口ID
	unsigned long			 dwBaudRate;				              // 当前使用的串口波特率
	unsigned char			 byComPortType;			                  // 当前使用的串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
	TIPC_SERIAL_TYPE         tSupportedType;		                  // 支持的串口类型 (只读)
	unsigned char			 byStop;					              // 停止位
	unsigned char			 byDig;					                  // 数据位
	unsigned char			 byParityCheck;			                  // 奇偶校验
	unsigned char			 byStart;				                  // 起始位
} TIPC_SERIAL_PT_PARAM, *PTIPC_SERIAL_PT_PARAM;

// 串口公共参数结构
typedef struct tagIPCSerialPortPubParam
{
	unsigned char			bySerialPortID;			                  // 操作的串口ID
	unsigned char			byInitStreamState;		                  // 串口数据收发初始化状态 0-无效, 1-有效
	unsigned short			wStreamLocalPort;		                  // 串口数据本地接收端口
	unsigned long			dwStreamDestIpAddr;		                  // 串口数据对端地址
	unsigned short			wStreamDestPort;		                  // 串口数据对端端口
	TIPC_CON_TYPE	        tSupportedConnType;		                  // 支持的码流链接类型
	unsigned char			byConnectionType;		                  // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned char			byProtocolType;			                  // 协议类型 0-透明通道, 1-VSIP协议
	unsigned char			bySerialId;                               // 对端的串口id
} TIPC_SERIAL_PT_PUB_PARAM, *PTIPC_SERIAL_PT_PUB_PARAM;

// 系统时间参数结构
typedef struct tagIPCSysTime
{
	unsigned short		wYear;		                                  // 年
	unsigned char		byMonth;	                                  // 月 1-12
	unsigned char		byDay;		                                  // 日 1-31
	unsigned char		byHour;		                                  // 时 0-23
	unsigned char		byMinute;	                                  // 分 0-59
	unsigned char		bySecond;	                                  // 秒 0-59
} TIPC_SYS_TIME, *PTIPC_SYS_TIME;

// 并口输入参数结构
typedef struct tagIPCInputPinParam
{
	unsigned char		  byPinID;					                  // 并口ID
	unsigned char		  byAlarmInitState;		                      // 定义告警和常态可选
	unsigned char		  byAlarmDetectState;			              // 告警检测状态 0-不检测, 1-只检测一个周期, 2-每周循环检测, 3-始终检测
	TIPC_SYS_TIME         atAlarmDetectStartTime[MAX_WEEKLYDAY_NUM];  // 告警检测开始时间,一周７天
	TIPC_SYS_TIME         atAlarmDetectDuration[MAX_WEEKLYDAY_NUM];	  // 告警检测持续时间,一周７天
	unsigned char		  byRelateEncId;				              // 告警关联的编码通道 (无关联编码通道则为 IPCSDK_RELATE_ENCID_DISABLE)
	unsigned char		  byOutputPinID;				              // 告警自动控制输出ID (无控制输出则为 IPCSDK_OUTPUT_DISABLE)
	unsigned char		  byAlarmOSDState;			                  // 告警字幕状态 0-不显示, 1-显示
	unsigned short		  wAlarmOSDLeftMargin;		                  // 告警字幕左边距 1-200
	unsigned short		  wAlarmOSDTopMargin;			              // 告警字幕上边距 1-200
	char		          achAlarmOSDName[MAX_STR_LEN_64+1];          // 告警字幕内容
	unsigned long		  dwAlarmOSDColor;			                  // 告警字幕颜色(RGB)
	unsigned char		  byEthAlarmState;			                  // 是否发送ETH1网口告警
} TIPC_INPUT_PIN_PARAM, *PTIPC_INPUT_PIN_PARAM;

// 视频源输入端口参数结构
typedef struct tagIPCVideoInputParam
{
	unsigned char		byVideoInputID;				                  // 视频源端口号
	unsigned char		byCameraType;				                  // 摄像头类型(IPC_CAMERA_TYPE_SONY等)
	unsigned char		byCameraID;					                  // 摄像头地址码
	unsigned char		bySerialPortID;				                  // 对应的串口ID
	char		        achVideoPortName[MAX_VIDEOIN_PORT_NM_LEN+1];  // 视频源端口名
	char		        achVideoPortOSD[MAX_STRING_OSD_LEN+1];        // 视频源字幕内容
	unsigned short		wVideoPortOSDLeftMargin;	                  // 视频源字幕左边距 1-200
	unsigned short		wVideoPortOSDTopMargin;		                  // 视频源字幕上边距 1-200
	unsigned long		dwVideoPortOSDColor;		                  // 视频源字幕颜色
	unsigned char       byInputType;	
	unsigned short      wVideoPortTimeLeftMargin;                     // 视频源字幕左边距 1-200
    unsigned short      wVideoPortTimeTopMargin;                      // 视频源字幕上边距 1-200
    unsigned short      wVideoPortAlarmLeftMargin;                    // 视频源告警字幕左边距 1-200
    unsigned short      wVideoPortAlarmTopMargin;                     // 视频源告警字幕上边距 1-200
} TIPC_VIDEO_INPUT_PARAM, *PTIPC_VIDEO_INPUT_PARAM;

// 网管参数
typedef struct tagIPCNmsParam
{
	unsigned char	 byMemThreshold;			                      // 系统内存使用阀值
	unsigned char    byMaxVideoLoseRatio;	                          // 视频丢包上报阀值
	unsigned char    byCpuUseRatio;			                          // cpu利用率
	unsigned long	 dwServerIP;				                      // 网管服务器IP地址
} TIPC_NMS_PARAM, *PTIPC_NMS_PARAM;

// 网管相关配置
typedef struct TIPCPuNmsConfig
{
	unsigned char	byRegType;								          // 连接类型:DNS,PUI
	unsigned long	dwDNSIp;								          // DNS的ip地址
	unsigned short  wDNSPort;								          // DNS端口
	unsigned short  wPingCheckTimes;					 	          // 断链检测次数
	unsigned short 	wPingCheckInterval;						          // 断链检测时间间隔
	unsigned long	dwVersionServerIp;						          // 版本服务器IP
	unsigned short  wVersionServerPort;						          // 版本服务器端口
	unsigned long   dwVersionCheckInterval;					          // 版本检测周期 单位:分钟
	char	        achVersionPath[MAX_PATH_LENGTH+1];			      // 下载版本文件信息
	char	        achVersionUserName[MAX_STR_LEN_64+1];	          // 版本服务器用户名
	char	        achVersionUserPW[MAX_STR_LEN_64+1];		          // 版本服务器密码
	unsigned char	bySlotId;							         	  // 槽号
	char	        achBoxInfo[MAX_STR_LEN_64+1];			          // 槽位信息
	char	        achLocationInfo[MAX_STR_LEN_64+1];		          // 位置信息
	unsigned short  DownloadStartPort;                                // 下载起始端口
	unsigned short  BrowseStartPort;                                  // 浏览起始端口
} TIPC_PU_NMS_CONFIG,*PTIPC_PU_NMS_CONFIG;

// PPPOE参数结构
typedef struct tagIPCPPPOEParam
{
	unsigned char		byPPPOEState;				                  // PPPOE状态 0-不使用, 1-使用
	char		        achUsername[MAX_USER_NUMBER];	              // 拨号用户名
	char		        achPassword[MAX_USER_NUMBER];	              // 拨号密码
	unsigned char		byAutoDialState;			                  // 自动拨号状态 0-不使用, 1-使用
	unsigned long		dwAutoDialWait;				                  // 自动拨号等待时间(秒)
	unsigned long		dwAutoDialTry;				                  // 自动拨号重试次数
	unsigned char		byDebugState;				                  // Debug状态 0-不使用, 1-使用
    unsigned short		wLcpEchoSendInterval;		                  // 链接建立后，发送LCP-ECHO包的时间间隔
    unsigned short		wLcpEchoRetryTimes;			                  // 发送几次LCP-ECHO包没有得到相应则认为链接断开
} TIPC_PPPOE_PARAM, *PTIPC_PPPOE_PARAM;

// 视频解码重传参数结构
typedef struct tagIPCVideoDecRetransferParam
{
	unsigned char		byDecID;					                  // 视频解码通道
	unsigned short		wRetransfer1stTimeSpan;		                  // 解码第1个重传检测点
	unsigned short		wRetransfer2ndTimeSpan;		                  // 解码第2个重传检测点
	unsigned short		wRetransfer3rdTimeSpan;		                  // 解码第3个重传检测点
	unsigned short		wRejectTimeSpan;			                  // 解码过期时间
	unsigned char		byRetransferState;			                  // 解码重传状态 0-不重传, 1-重传
} TIPC_VIDEO_DEC_RETRANSFER_PARAM, *PTIPC_VIDEO_DEC_RETRANSFER_PARAM;

// 录像系统参数
typedef struct tagIPCRecSysInfo
{
	unsigned char	byDiskFullStrategy;                               // 磁盘满录象策略
	unsigned char	abyRecFileStorePath[MAX_DISK_NUM];                // 录象文件存放路径
	unsigned long	dwDiskRemainSpace;                                // 磁盘剩余空间(MB)
	unsigned short	wLittleFileRecTime;                               // 小文件录象时长
	unsigned char	abyRecDiskPartitionInfo[MAX_DISK_NUM][MAX_DISK_PARTITION_NAME]; // 磁盘分区信息，用户不可设置
	unsigned char	byDiskPartitionNum;                               // 文件分区数目,用户不可设置
	unsigned char	byUsedPartitionNum;                               // 用户使用了的分区数目
} TIPC_REC_SYS_INFO, *PTIPC_REC_SYS_INFO;

// 录像基本参数
typedef struct tagIPCRecChannelBasicInfo
{
	unsigned char	byChannel;
	unsigned short	wAlarmPreRecTime;				                  // 告警预录时间
	unsigned short	wAlarmResumeRecTime;			                  // 告警恢复后继续录制时间
	unsigned char	byIsAbstractFrame;				                  // 是否抽帧录象 1为是，0为否
	unsigned char	byIsRecWhenAlarmOccur;			                  // 告警是否录象,1为是,0为否
	unsigned char	byAbstractRecInterval;			                  // 抽帧录像间隔,指关键帧个数
	unsigned char	byIsAudioRecorded;				                  // 是否录音频
	unsigned char   byRecChanNum;                                     // 0-录制主码流，其它－录制辅助码流号,默认为0
} TIPC_REC_CHANNEL_BASIC_INFO, *PTIPC_REC_CHANNEL_BASIC_INFO;

// 开始停止录象时间设置结构
typedef struct tagIPCRecStartEndTimeInfo
{
	TIPC_SYS_TIME StartTime;				                          // 开始录象时间
	TIPC_SYS_TIME EndTime;					                          // 停止录象时间
} TIPC_REC_START_END_TIME_INFO, *PTIPC_REC_START_END_TIME_INFO;

// 定时录像
typedef struct tagIPCTimelyRecInfo
{
	unsigned char	byChannel;
    unsigned char	byEnable;
	TIPC_REC_START_END_TIME_INFO RecSETimeInfo;
} TIPC_TIMELY_REC_INFO, *PTIPC_TIMELY_REC_INFO;

// 周期录像
typedef struct tagIPCPeriodlyTimeInfo
{
	unsigned char	byChannel;
	unsigned char	byPeriodType;					                  // 周期录象策略(IPCSDK_PERIOD_DAYLY等)
	TIPC_REC_START_END_TIME_INFO aRTimeInfo[3*(MAX_WEEKLYDAY_NUM+1)];
} TIPC_PERIODLY_TIME_INFO, *PTIPC_PERIODLY_TIME_INFO;

// 抓拍基本参数
typedef struct tagIPCSnapShotConfig
{
	unsigned char	byChannel;
	unsigned char	byPinAlarmEnable;				                  // 并口告警抓图使能 0关闭/1开启
	unsigned char	byMotionDetectEnable;			                  // 移动侦测抓图使能 0关闭/1开启
	unsigned long	dwInterval;						                  // 抓拍时间间隔(s)
	unsigned long	dwMaxSnapNum;					                  // 最大抓拍数量
} TIPC_SNAPSHOT_CONFIG,*PTIPC_SNAPSHOT_CONFIG;

// 抓拍命名规则
typedef struct tagIPCSnapShotNamingRule
{
	unsigned char abyNameOrder[MAX_SEPA_LEN];                         // 字段顺序，从0开始，依次表示第i+1个字段
	unsigned char byNameNum;                                          // 实际使用的字段个数	
	char          achDateSeparator[MAX_SEPA_LEN];                     // 日期之间的分隔符(年月日)	
	char          achTimeSeparator[MAX_SEPA_LEN];                     // 时间之间的分隔符(时分秒)	
	char          achNameSeparator[MAX_SEPA_LEN];                     // 命名字段之间的分隔符 	
	char          achUserDefine[MAX_STR_LEN_64+1];                    // 用户自定义字段内容	
	int           bIsRemoteStore;                                     // 是否远端存储	
	char          achSvrSharePath[MAX_PATH_LENGTH + 1];               // 服务器共享路径（格式：172.16.3.108:/usr/share) 
} TIPC_SNAPSHOT_NAMING_RULE,*PTIPC_SNAPSHOT_NAMING_RULE;

// 快门参数
typedef struct tagIPCSnapShotExtCfg
{
	unsigned char  byChannel;                                         // 通道号 
	unsigned short wReciprocal;	                                      // 当前的快门参数倒数
	unsigned short wSupportListNum; 	                              // 支持的快门参数列表数(只读)
	unsigned short awSupportList[MAX_RECIPROCAL_LIST_SIZE];	          // 支持的快门参数列表(只读)	
} TIPC_SNAPSHOT_EXT_CFG, *PTIPC_SNAPSHOT_EXT_CFG;

// 白平衡参数
typedef struct tagIPCWhiteBalance
{
	unsigned short wBluePlus;                                         // 蓝增益
	unsigned short wRedPlus;                                          // 红增益
} TIPC_WHITE_BALANCE, *PTIPC_WHITE_BALANCE;

// 双流参数
typedef struct tagIPCMultiChanInfo
{
	unsigned char byChanType;                                         // 主流还是辅助流,1－主流，0－辅流
	unsigned char bySupportSubNum;                                    // 支持辅助流路数，辅助流为0;如果是开启第二路码流，则填其码流号。
	unsigned char SubChanId[8];				                          // 辅助流ID列表，根据辅助流路数，依次填写
	unsigned char byStatus;                                           // 当前辅助流是否开启（1－开启，0-未开启），仅主流有；如果是开启双流，则填1，如果停止双流则填0；
    unsigned char byParentEncId;                                      // 主流为0，辅助流为主流的编码通道号
} TIPC_MULTI_CHAN_INFO, *PTIPC_MULTI_CHAN_INFO;

// 云台控制相关参数
typedef struct tagIPCPtzCtrlInfo
{
	unsigned char  bySupportIMageFlip;                                 // 是否支持图像翻转 0-不支持 1-支持
	unsigned char  bySupportDevFlip;                                   // 是否支持云台翻转 0-不支持 1-支持
	unsigned char  bySupportBackLight;                                 // 是否支持背光补偿 0-不支持 1-支持
	unsigned char  bySupportDayNight;                                  // 是否支持日夜模式 0-不支持 1-支持
	unsigned char  bySupportWdr;                                       // 是否支持宽动态   0-不支持 1-支持
	unsigned char  bySupportWB;                                        // 是否支持白平衡   0-不支持 1-支持
	unsigned char  bySupportShutter;                                   // 是否支持快门     0-不支持 1-支持
	unsigned char  bySupportGain;                                      // 是否支持增益     0-不支持 1-支持
	unsigned char  bySupportImgScale;                                  // 是否支持景深比例 0-不支持 1-支持
	unsigned char  bySupportIR;                                        // 是否支持红外     0-不支持 1-支持
	unsigned char  bySupportLaser;                                     // 是否支持激光     0-不支持 1-支持
	unsigned char  bySupportWiper;                                     // 是否支持雨刷     0-不支持 1-支持
	unsigned char  bySupportDefrost;                                   // 是否支持除霜     0-不支持 1-支持
	unsigned char  bySupportFocus;                                     // 是否支持聚焦     0-不支持 1-支持 

	unsigned char  byDayNightSetMode;                                  // 日夜模式设定方式 0-自动   1-手动
	unsigned char  byWBSetMode;                                        // 白平衡设定方式   0-自动   1-手动
	unsigned char  byShutterSetMode;                                   // 快门设定方式     0-自动   1-手动
	unsigned char  byGainSetMode;                                      // 增益设定方式     0-自动   1-手动
	unsigned char  byFocusMode;                                        // 聚焦设定方式     0-自动   1-手动

	unsigned char  byBackLightState;                                   // 背光补偿状态     0-关     1-开
	unsigned char  byDayNightState;                                    // 日夜模式状态     0-日模式 1-夜模式
	unsigned char  byWdrState;                                         // 宽动态状态       0-关     1-开
	unsigned char  byWBValueMode;                                      // 白平衡值模式     0-普通   1-高级(红蓝增益)
	unsigned char  byWBValue;                                          // 白平衡-普通值
	unsigned short wWBRedPlus;                                         // 白平衡-红增益值
	unsigned short wWBBluePlus;                                        // 白平衡-蓝增益值
	unsigned long  dwShutterValue;                                     // 快门取值(分母值)
	unsigned long  dwGainValue;                                        // 增益取值(现为6的倍数)
	unsigned long  dwMaxGainVal;                                       // 增益最大值(现为6的倍数)
	unsigned long  dwFocusVal;                                         // 聚焦值(手动模式)
} TIPC_PTZ_CTRL_INFO, *pTIPC_PTZ_CTRL_INFO;

/*************************************************
/	                   OSD
/*************************************************/
// OSD台标
typedef struct tagIPCAdjustPicInfo
{
	unsigned short wStartX;				                              // 图片小块起始坐标X轴
	unsigned short wStartY;                                           // 图片小块起始坐标Y轴
	unsigned char  byRegionHeight;		                              // 图片小块的高（每个小块都相同)
	unsigned char  byRegionWidthSize;		                          // 下面这个结构的项数
	unsigned char  abyRegionWidth[16];	                              // 图片各个小块的宽
} TIPC_ADJUST_PIC_INFO, *PTIPC_ADJUST_PIC_INFO;

// 台标字幕上传
typedef struct tagTitleUpload
{
	unsigned long dwPackInd;                                          // 文件数据包ID
	unsigned char byMsgType;                                          // 台标字幕类型
	unsigned char *pTitleData;                                        // 文件数据
	unsigned long dwDataSize;                                         // 文件大小
	unsigned char byAlarmPinId;                                       // 并口告警ID
} TIPC_TITLE_UPLOAD, *PTIPC_TITLE_UPLOAD;

/*************************************************
/	                 移动侦测
/*************************************************/
// 移动侦测区域结构体
typedef struct tagIPCMotionDetectParam
{
	unsigned char   byDetectEnable;			                          // 侦测使能 1-有效 0-无效
	unsigned long   dwXPos;					                          // 侦测区域顶点横坐标
	unsigned long   dwYPos;					                          // 侦测区域顶点纵坐标
	unsigned long   dwWidth;					                      // 侦测区域宽度
	unsigned long   dwHeigth;			                              // 侦测区域高度
	unsigned char   byAlarmRate;				                      // 侦测区域告警产生百分比（大于此临界值告警,0-100）
	unsigned char   byResumeRate;			                          // 产生告警后运动范围百分比小于该值则恢复告警(该值小于byAlarmRate,0-100)
	short           swDetSense;				                          // 灵敏度 -10~10
} TIPC_MOTION_DETECT_PARAM, *PTIPC_MOTION_DETECT_PARAM;

typedef struct tagIPCMotionDetect
{
	unsigned char            byType;							      // 移动侦测类型(IPC_ENCODER-编码;IPC_DECODER-解码)
	unsigned char            byEntityID;						      // 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
	TIPC_MOTION_DETECT_PARAM aMotionDetectParam[MAX_MOTION_DETECT];   // 移动侦测参数数组(必须为十六个元素数组)
	unsigned short           wDetAreaNum;						      // 返回移动侦测区域数
} TIPC_MOTION_DETECT, *PTIPC_MOTION_DETECT;

/*************************************************
/	                 图像遮蔽
/*************************************************/
// 图像遮蔽控制结构体
typedef struct tagIPCShadeParam
{
	unsigned char          byShadeEnable;			                  // 遮蔽使能 1-有效 0-无效
	unsigned long          dwXPos;				                      // 遮蔽区域顶点横坐标
	unsigned long          dwYPos;				                      // 遮蔽区域顶点纵坐标
	unsigned long          dwWidth;				                      // 遮蔽区域宽度
	unsigned long          dwHeigth;				                  // 遮蔽区域高度
} TIPC_SHADE_PARAM, *PTIPC_SHADE_PARAM;

// 图像遮蔽控制载体
typedef struct tagIPCVideoShadeParam
{
	unsigned char       byEntityID;                                   // 视频源端口ID
	TIPC_SHADE_PARAM    atSDArea[MAX_MOTION_DETECT];                  // 图像遮蔽控制区域
	unsigned short      wAreaNum;                                     // 区域数目
} TIPC_VIDEO_SHADE_PARAM, *PTIPC_VIDEO_SHADE_PARAM;

/*************************************************
/	                 视频浏览
/*************************************************/
// 解码器初始化参数结构
typedef struct tagIPCDecWrapInitParam
{
	unsigned long		dwConnectIP;
	unsigned short		wStartPort;		                              // 起始端口
		   					                                          // 各解码器根据自己的ID,使用在起始端口后面的第(wStartPort+ID*4)个端口
	unsigned long		dwChnID;
	unsigned long		dwVideoInputPort;
	unsigned long		dwRecFileID;
    unsigned char       byRecPlayId;
	unsigned short		wDecID;
	unsigned long    	lWnd;
} TIPC_DEC_WRAP_INIT_PARAM, *PTIPC_DEC_WRAP_INIT_PARAM;

// 公共网络参数结构
typedef struct tagIPCCommonNetParam
{
	unsigned char   byInitStreamState;		                          // 码流初始化状态 0-不发送, 1-发送
	unsigned char	byCurStreamState;		                          // 码流当前状态 0-未发送, 1-发送
	TIPC_CON_TYPE	tSupportedConnType;	                              // 支持的码流链接类型
	unsigned char	byConnectionType;		                          // 码流链接类型(IPCSDK_CONNECTION_UNICAST等)
	unsigned long	dwStreamIpAddr;			                          // 码流传输地址
	unsigned short	wStreamPort;			                          // 码流传输端口
} TIPC_COMMON_NET_PARAM, *PTIPC_COMMON_NET_PARAM;

// 解码器统计信息
typedef struct tagIPCDecStatics
{
	unsigned char	m_byVideoFrameRate;                               // 视频解码帧率
	unsigned short	m_wVideoBitRate;                                  // 视频解码码率
	unsigned short	m_wAudioBitRate;                                  // 语音解码码率
	unsigned long   m_dwVideoRecvFrame;                               // 收到的视频帧数
	unsigned long   m_dwAudioRecvFrame;                               // 收到的语音帧数
	unsigned long   m_dwVideoLoseFrame;                               // 丢失的视频帧数
	unsigned long   m_dwAudioLoseFrame;                               // 丢失的语音帧数
    unsigned short	m_wVideoLoseRatio;                                // 视频丢失率,单位是% 	
	unsigned short	m_wAudioLoseRatio;                                // 音频丢失率,单位是% 	
	unsigned long   m_dwPackError;                                    // 乱帧数
	unsigned long   m_dwIndexLose;                                    // 序号丢帧
	unsigned long   m_dwSizeLose;                                     // 大小丢帧
	unsigned long   m_dwFullLose;                                     // 满丢帧
	unsigned long   m_wAvrVideoBitRate;                               // 1分钟内视频解码平均码率
	unsigned long	m_wAvrAudioBitRate;                               // 1分钟内语音解码平均码率
	int	            m_bVidCompellingIFrm;                             // 视频强制请求I帧
	unsigned long   m_dwDecdWidth;	                                  // 码流的宽
	unsigned long   m_dwDecdHeight;                                   // 码流的高
} TIPC_DEC_STATICS, *PTIPC_DEC_STATICS;

// 视频帧头结构
typedef struct tagIPCFrameHdr
{
    unsigned char    byMediaType;                                     // 媒体类型
    unsigned char    *pData;                                          // 数据缓冲
    unsigned long    dwPreBufSize;                                    // pData缓冲前预留了多少空间，用于加
	                                                                  // RTP option的时候偏移指针一般为12+4+12
	                                                                  // (FIXED HEADER + Extence option + Extence bit)
    unsigned long    dwDataSize;                                      // pData指向的实际缓冲大小缓冲大小
    unsigned char    byFrameRate;                                     // 发送帧率,用于接收端
    unsigned long    dwFrameID;                                       // 帧标识，用于接收端
    unsigned long    dwTimeStamp;                                     // 时间戳, 用于接收端
    unsigned long    dwSSRC;                                          // 同步源, 用于接收端
    union
    {
        struct{
			int               bKeyFrame;                              // 频帧类型(I帧 or P帧)
			unsigned short    wVideoWidth;                            // 视频帧宽
			unsigned short    wVideoHeight;                           // 视频帧高
		}tVideoParam;
        unsigned char    byAudioMode;                                 // 音频模式
    };
} TIPC_FRAMEHDR,*PTIPC_FRAMEHDR;

// 视频帧回调
typedef void (*IPCSDK_FRAME_CB)(unsigned long hHandle, unsigned long hPlayHandle, PTIPC_FRAMEHDR pFrmHdr, unsigned long dwParam);

// 码流转发帧数据回调
typedef void (*IPCSDK_DATA_SWITCH_FRM_CB)(unsigned long hHandle, unsigned char byChanId, PTIPC_FRAMEHDR *pFrmHdr, void* pContext);

// YUV解码码流信息回调
// 参数分别为位图缓冲指针、位图宽、高、位深
typedef void (*IPCSDK_FRAMEINFO_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned char *pBitmapBuf, 
									unsigned long dwWidth, unsigned long dwHeight, unsigned long dwbitcount, unsigned long dwParam);

/*************************************************
/	                 呼叫和监听
/*************************************************/
// 扩展终端码流控制(解码器)
typedef struct tagIPCMediaCtrlEX
{
	unsigned char	  byEntityID;		                              // 通道ID
	unsigned char     byMediaCtrlType;                                // Video--IPCSDK_VIDEO_TYPE_XXX, Audio--IPCSDK_AUDIO_TYPE_XXX
	unsigned char     byStreamType;	                                  // 码流传输类型IPCSDK_MEDIA_STREAM_TYPE_UDP, IPCSDK_MEDIA_STREAM_TYPE_TCP
	unsigned short    wLocalAcceptStreamPort;                         // 本地码流接收端口
	unsigned long  	  dwLocalAcceptStreamIpAddr;                      // 本地码流接收地址
   	unsigned short	  wRTCPStreamPort;                                // 对端的RTCP端口
	unsigned long	  dwRTCPStreamIpAddr;	                          // 对端的RTCP端口地址
} TIPC_MEDIA_CTRL_EX, *PTIPC_MEDIA_CTRL_EX;

/*************************************************
/	              图像抓拍和下载
/*************************************************/
// 图片查询结构
typedef struct tagIPCQueryPicFileParam39
{
	unsigned char	      byNum;									  // 文件数量
	unsigned char	      byType;									  // 查询类型
	unsigned char	      byChanId;								      // 通道号
	unsigned char	      byEvent;								      // 事件类型
	unsigned short	      wBeginNum;								  // 文件查询起始序号
	TIPC_SYS_TIME         tTimeStart;						          // 起始抓拍时间
	TIPC_SYS_TIME         tTimeEnd;							          // 结束抓拍时间
} TIPC_QUERY_PIC_FILE_PARAM, *PTIPC_QUERY_PIC_FILE_PARAM;

// 查询文件条件
typedef struct tagIPCPicFileInfo
{
	unsigned char	byType;									          // 事件类型(IPCSDK_QUIEY_ALL_EVENT等)
	unsigned char	bychanId;								          // 通道号
	unsigned char	byVideoResolution;						          // 图像分辨率
	unsigned char	byFormat;								          // 图片格式
	TIPC_SYS_TIME   tTime;							                  // 截图时间
	unsigned long	dwPicID;								          // 图片编号
	char	        achFileName[MAX_FILE_NAME_LENGTH+1];	          // 文件别名
} TIPC_PIC_FILE_INFO, *PTIPC_PIC_FILE_INFO;

typedef struct tagIPCPicFileAck
{
	unsigned char byType;			                                  // 事件类型(IPCSDK_QUIEY_ALL_EVENT等)
	unsigned char bychanId;		                                      // 通道号
	unsigned char byVideoResolution;                                  // 图像分辨率
	unsigned char byFormat;		                                      // 图片格式
	unsigned long dwTime;			                                  // 截图时间
	unsigned long dwPicID;		                                      // 图片编号
	unsigned long dwSize;			                                  // 文件大小
} TIPC_PIC_FILE_ACK, *PTIPC_PIC_FILE_ACK;

typedef struct tagIPCPicFileAckEx
{
	unsigned char byType;			                                  // 事件类型(IPCSDK_QUIEY_ALL_EVENT等)
	unsigned char bychanId;		                                      // 通道号
	unsigned char byVideoResolution;                                  // 图像分辨率
	unsigned char byFormat;		                                      // 图片格式
	TIPC_SYS_TIME tTime;	                                          // 截图时间
	unsigned long dwPicID;		                                      // 图片编号
	char          achFileName[MAX_FILE_NAME_LENGTH+1];	              // 文件别名
} TIPC_PIC_FILE_ACK_EX, *PTIPC_PIC_FILE_ACK_EX;

// 图片查询结果
typedef struct tagIPCQueryPicFileInfo
{
    unsigned short wMaxNum;                                           // 当前查询文件总数量
    unsigned short wIndex;                                            // 当前包索引号,从1开始
    unsigned char byNum;                                              // 文件数量
    unsigned char byEnd;                                              // 是否结束
    TIPC_PIC_FILE_ACK aPicFileInfo[MAX_PIC_FILE_NUM];                 // 图片文件列表
} TIPC_QUERY_PIC_FILE_INFO, *PTIPC_QUERY_PIC_FILE_INFO;

typedef struct tagIPCDownloadPicFileInfo
{
	unsigned short		wResult;			                          // 操作结果
	unsigned short		wTotalPacket;		                          // 总包个数
	unsigned short		wCurPacketIdx;	                              // 当前包的索引号,从1开始
	unsigned short		wCurLen;			                          // 当前长度
	unsigned long		dwPicID;			                          // 图片编号
	unsigned char       abyBuf[SM_DATA_BUFSIZE];                      // 内容
} TIPC_DOWN_PIC_FILE_INFO, *PTIPC_DOWN_PIC_FILE_INFO;

// 前端图片下载相关
typedef struct tagIPCDownPuPic
{
	unsigned long 		dwCurPkgNum;			                      // 当前包数量
	unsigned char		*pPicFileBuf;			                      // 图片文件缓冲区
	unsigned long		dwBufRecvedBytes;		                      // 已收到的大小
	int					nDownloadPicRet;		                      // -1:下载失败 0:还未结束 1:下载成功
	TIPC_PIC_FILE_ACK_EX tPicFileInfo;			                      // 待下载的图片文件信息
	char				szSavePath[MAX_PATH_LENGTH];                  // 保存路径
} TIPC_DOWN_PU_PIC, *PTIPC_DOWN_PU_PIC;

/*************************************************
/	               前端录像相关
/*************************************************/
// 录像文件查询结构
typedef struct tagIPCQueryParam
{
	unsigned char                byChanId;
	TIPC_REC_START_END_TIME_INFO RStartEndTimeInfo;
	unsigned char                byType;
	unsigned long                dwFileIndex;
	unsigned short               wNum;
} TIPC_QUERY_PARAM, *PTIPC_QUERY_PARAM;

// 录像文件信息
typedef struct tagIPCRecFileInfo
{
	unsigned char   byChanID;			                              // 通道ID
	unsigned long   dwStartTime;		                              // 开始时间
	unsigned long   dwEndTime;		                                  // 结束时间
	unsigned long   dwRecSize;		                                  // 文件大小
	unsigned char   byRecFileType;		                              // 录像文件类型(EPuRecQueryType类型)
	unsigned long   dwFileID;
} TIPC_REC_FILE_INFO, *PTIPC_REC_FILE_INFO;

// 录象文件/事件信息
typedef struct tagIPCRecFileInfoEx
{
	unsigned long	dwRecIndexId;							          // 文件事件id号
	unsigned char	byRecFileType;                                    // 录象文件/事件类型(EPuRecQueryType类型)
	TIPC_REC_START_END_TIME_INFO RecSETimeSetting;                    // 开始停止录象时间
	unsigned char	abyFileName[MAX_FILE_NAME_LENGTH+1];              // 文件别名
	unsigned char	abyFilePath[MAX_FILE_PATH_LENGTH+1];              // 最大文件路径长度
} TIPC_REC_FILE_INFO_EX, *PTIPC_REC_FILE_INFO_EX;

// 查询文件信息表
typedef struct tagIPCQueryRecFileInfo
{
	unsigned short wNum;                                              // 录象文件数目
	unsigned char  byType;                                            // 录象文件/事件类型
	unsigned char  bIsFinished;                                       // 是否是所有的录象文件,1为结束，0为未结束
	TIPC_REC_FILE_INFO_EX aRFileInfo[MAX_REC_FILE_NUM];               // 录象文件信息
	unsigned short wTransactionNum;                                   // 文件操作流水号
} TIPC_QUERY_REC_INFO, *PTIPC_QUERY_REC_INFO;

// 进度参数信息
typedef struct tagIPCProgressInfo
{
	unsigned char  abyCreateTime[MAX_TM_PRECISION];                   // 录象文件创建时间,包括毫秒
	unsigned long  dwTotalFileTime;                                   // 文件总时间
	unsigned long  dwCurrentPlayTime;                                 // 当前播放时间
	unsigned char  byPlayerId;                                        // 放像器id
	unsigned char  byIsFinished;                                      // 是否播放结束
	unsigned short wTransactionNum;                                   // 操作流水号
} TIPC_PROGRESS_INFO, *PTIPC_PROGRESS_INFO;

/*************************************************
/	               录像文件回放
/*************************************************/
// 文件解码器相关
typedef struct tagIPCPlayFileParam
{
    unsigned long        dwGranuity;
    unsigned long        dwContext;
    IPCSDK_FILE_STAT_CB  pfCB;
    unsigned char        byFileType;
    char                 achFileName[512];
} TIPC_PLAY_FILE_PARAM, *PTIPC_PLAY_FILE_PARAM;

// 回放文件的信息
typedef struct tagIPCPlayFileInfo
{
    unsigned __int64   qwTotalSize;
    unsigned __int64   qwCreateDate;
    unsigned long      dwTotalTime;
} TIPC_PLAY_FILE_INFO, *PTIPC_PLAY_FILE_INFO;

// 回放文件的流信息
typedef struct tagIPCPlayFileStreamInfo
{
    unsigned long          dwAudio;                                   // 是否是音频流
    unsigned short         wWidth;                                    // 图像宽度
    unsigned short         wHeight;                                   // 图像高度
    unsigned char          byStreamId;                                // 码流编号
    unsigned char          byMediaType;                               // 编码格式
} TIPC_PLAY_FILE_STREAM_INFO, *PTIPC_PLAY_FILE_STREAM_INFO;

// 回放文件的状态
typedef struct tagIPCPlayFileStatus
{
    unsigned long                      dwCurrentPlayPos;
    unsigned char                      byPlayMode;
    unsigned char                      byPlayRate;
    unsigned char                      byPlayState;
    unsigned char                      byStreamNum;
    unsigned long                      dwVFps;
    unsigned long                      dwAFps;
    TIPC_PLAY_FILE_STREAM_INFO atFileStreamInfo[MAX_FILE_STREAM_NUM];
} TIPC_PLAY_FILE_STATUS, *PTIPC_PLAY_FILE_STATUS;

/*************************************************
/	             透明通道扩展参数
/*************************************************/
// 自动增益参数(最大值)
typedef struct tagIPCAgcInfo
{
	unsigned long dwAgcMaxHigh;                                       // 自动增益最大值上限
	unsigned long dwAgcMaxLow;                                        // 自动增益最大值下限 
	unsigned long dwAgcMaxCurVal;                                     // 自动增益最大值当前值 
} TIPC_AGC_INFO, *PTIPC_AGC_INFO;

// 自动快门参数(最值)
typedef struct tagIPCAutoShutterInfo
{
	unsigned long dwAutoShutterMax;                                   // 自动快门范围最大值
	unsigned long dwAutoShutterMin;                                   // 自动快门范围最小值 
	unsigned long dwAutoShutterCurMaxVal;                             // 自动快门当前可设定的最大值
	unsigned long dwAutoShutterCurMinVal;                             // 自动快门当前可设定的最小值
} TIPC_AUTOSHUT_INFO, *PTIPC_AUTOSHUT_INFO;

// 本地回显参数
typedef struct tagIPCBncInfo
{
	int bSupportBNC;                                                  // 是否支持本地回显
	int nBNCStatus;                                                   // 本地回显状态 0-关闭 1-开启
} TIPC_BNC_INFO, *PTIPC_BNC_INFO;

/*************************************************
/	                  前端升级
/*************************************************/
#ifdef WIN32
#pragma pack(push, 1)
#endif
// 基本地址信息
typedef struct tagIPCAddrInfo
{
	unsigned long   dwIp;				                              // ip地址(网络序)
	unsigned short	wPort;				                              // 端口号
} TIPC_ADDR_INFO,*PTIPC_ADDR_INFO;

// 用户信息
typedef struct tagIPCUserInfo
{
	char	achName[ MAX_USER_NUMBER + 1 ];		                      // 用户名
	char	achPassWord[ MAX_USER_NUMBER + 1 ];	                      // 用户密码
} TIPC_USER_INFO,*PTIPC_USER_INFO;

// 版本升级
typedef struct tagIPCVerUpdateInfo
{
	unsigned char	 byFileType;									  // 需升级的文件类型
	char			 achFileInfo[MAX_STR_LEN_64+1];			          // 需升级的文件路径及文件名
	TIPC_ADDR_INFO   tFtpAddr;									      // FTP服务器地址及端口
	TIPC_USER_INFO	 tUserInfo;								          // 用户登录信息
	    
} TIPC_VER_UPD_INFO, *PTIPC_VER_UPD_INFO;

/*************************************************
/	               系统信息(全)
/*************************************************/
// FTP配置
typedef struct tagIPCFtpSvrCfg
{
	unsigned long    dwFtpSvrIpAddr;                                  // FTP服务器地址
	unsigned short   wFtpSvrPort;                                     // FTP服务器端口
	char             achFtpSvrPath[MAX_STR_LEN_64 + 1];               // FTP服务器存放路径
	TIPC_USER_INFO   tFtpUserInfo;                                    // FTP登陆用户信息
} TIPC_FTP_SVR_CFG, *PTIPC_FTP_SVR_CFG;

// 文件存储配置
typedef struct tagIPCFileStoreCfg
{	
	int		         bIsRemoteStore;							      // 是否远端存储
	unsigned long	 dwNfsSvrIpAddr;								  // NFS服务器地址
	char			 achSvrSharePath[MAX_STR_LEN_64 + 1];	          // 服务器共享路径	
	int              bIsFtpUpLoad;                                    // 是否FTP存储
	TIPC_FTP_SVR_CFG tPuFtpCfg;                                       // FTP配置	
	unsigned char 	 abyUsedDiskPartionId[ MAX_DISK_NUM ];            // 用户使用的分区路径列表(分区硬件ID)
	unsigned char 	 byUsedDiskPartionhNum;						      // 用户使用的分区个数	
} TIPC_FILE_STORE_CFG, *PTIPC_FILE_STORE_CFG;

// 命名规则
typedef struct tagIPCNamingRule
{
	unsigned char abyNameOrder[MAX_NAMERULE_NUM];		              // 字段顺序，从0开始，依次表示第i+1个字段
	unsigned char byNameNum;								          // 实际使用的字段个数
	char achDateSeparator[MAX_SEPA_LEN];		                      // 日期之间的分隔符(年月日)(有效可打印字符)
	char achTimeSeparator[MAX_SEPA_LEN];		                      // 时间之间的分隔符(时分秒)(有效可打印字符)
	char achNameSeparator[MAX_SEPA_LEN];		                      // 命名字段之间的分隔符(有效可打印字符)
	char achUserDefine[MAX_STR_LEN_64 + 1];		                      // 用户自定义字段内容	
} TIPC_NAMING_RULE, *PTIPC_NAMING_RULE;

// 抓拍配置
typedef struct tagIPCSnapShotCfg
{
	unsigned short		    wSnapInterval;			                  // 图像抓拍时间间隔(单位:秒,默认5秒,范围:5~300s)
	unsigned char			byOneAlmPicMaxNum;		                  // 单次告警抓拍图片个数(单位:张,默认5张,范围:5~50张)
	unsigned long			dwUseSize;				                  // 抓拍使用空间大小(单位:MB,默认50MB(50M~硬盘分区最大空间))
	TIPC_NAMING_RULE		tNamingRule;			                  // 抓拍图片命名规则
	TIPC_FILE_STORE_CFG		tFileStoreCfg;			                  // 存储配置	
} TIPC_SNAPSHOT_CFG, *PTIPC_SNAPSHOT_CFG;

// 分区信息
typedef struct tagIPCDiskPartionInfo
{
	int                     bFormated;								  // 是否格式化
	int                     bValid;								      // 是否有效
	unsigned long           dwMBSize;								  // 分区大小
	unsigned long           dwFreeMBSize;							  // 剩余空间
	char                    achName[ MAX_STR_LEN_64 + 1 ];		      // 分区mount后的名称
	char                    achPhyName[ MAX_STR_LEN_64 + 1 ];         // 分区的物理名称
	unsigned long           dwLogicId;					              // 逻辑编号				
} TIPC_DISK_PARTION_INFO, *PTIPC_DISK_PARTION_INFO;

// socks5排除地址信息
typedef struct tagIPCFilterAddr
{
	unsigned long		    dwIpAddr;		                          // ip地址
	unsigned long		    dwMask;			                          // 子网掩码	
} TIPC_FILTER_ADDR, *PTIPC_FILTER_ADDR;

// 系统参数(全部)
typedef struct tagPuSysCfg
{
	unsigned char				byStackType;						  // 协议类型(默认:IPC_STACK_VSIP)
	TIPC_ADDR_INFO		        tDnsServer;							  // DNS服务器地址,0表示不启用
	unsigned long 			    dwRegPlatformIp;					  // 注册的平台IP(默认:IPCSDK_DEFCFG_REGPFIP)
	unsigned short				wRegPlatformPort;					  // 注册的平台PORT(默认:IPCSDK_DEFCFG_REGPFPORT)
	unsigned long				dwVsipMultiIpAddr;					  // 搜索的组播地址(默认:IPCSDK_DEFCFG_VSIPMULIP)
	unsigned short				wVsipPort;							  // 搜索的端口号
	unsigned char				byLinkChkInterval;					  // 链路检测间隔(单位:秒,默认:10秒(0~60s)
	unsigned char				byLinkChkDisNum;					  // 链路检测最大尝试次数(单位:次,默认:3次(1~255次)
	
	int			                bNatPacketSend;						  // 是否发送NAT探测包(默认:否)
	unsigned char				byNatSndInterval;					  // NAT探测包间隔(默认:5)
	unsigned char				byCurStrSendNum;					  // 当前单个通道的最大发送路数(默认:5)
	unsigned char				byCurDirSendNum;					  // 当前单个通道的最大直传路数(默认:0路)
		
	unsigned char				abyVidEncCapSet[MAX_CHANNEL_NUM];	  // 支持的视频编码能力(只读)
	unsigned char				byVidEncCapSetNum;					  // 支持的编码能力个数(只读)
	unsigned char				byMaxVidEncNum;						  // 当前最大视频编码能力(默认同当前设备最小能力)
	int			                bIsVidCompOutput;					  // 是否画面合成输出(仅解码器)
	unsigned char               byTimeSynMode;                        // 时钟同步方式
	TIPC_ADDR_INFO		        tSynTimeServer;						  // 时钟同步服务器,0表示不同步
	unsigned long				dwSynTimeInt;						  // 同步时间间隔(默认:IPCSDK_DEFCFG_SYNTIMEINT(1~72*60分钟)
	TIPC_USER_INFO		        tAdminUserInfo;						  // 管理员信息(默认:admin,admin)
	TIPC_USER_INFO		        tNormalUserInfo;					  // 普通用户信息(默认:user,user)
	int			                bGuardEnable;						  // 系统守卫使能(内部使用,默认:启用)
	TIPC_SNAPSHOT_CFG	        tSnapShotCfg;						  // 图像抓拍配置
	
	// 系统分区信息
	TIPC_DISK_PARTION_INFO 	    atDiskPartionInfo[MAX_DISK_NUM];	  // 系统分区列表(只读)
	unsigned char				byDiskPartionNum;					  // 系统分区个数(只读)
	
	TIPC_ADDR_INFO		        tSocks5Agent;						  // SOCKS5 代理服务器,0表示不使用
	TIPC_USER_INFO		        tSocks5AuthInfo;					  // SOCKS5认证信息(默认:默认:admin,admin)
	TIPC_FILTER_ADDR	        atSocks5Filter[MAX_FILTERADDR_NUM];	  // SOCKS5过滤地址列表(默认:无)
	
	unsigned short              wAlarmResumeTime;                     // 为了兼容3.9新增告警恢复时间字段，只给vsipstack用
	int                         bUseIndiLight;                        // 是否开启指示灯
	unsigned char				byChangeMask;						  // 是否改变mask,目前用于整个结构是否修改(配置模块)
	unsigned char               byUseDomain;                          // 是否使用域名
	char                        achPlatDomain[MAX_STR_LEN_64+1];      // 平台域名
	int                         bSupportBuildNet;                     // 是否支持组网 TURE:支持
} TIPC_SYS_CFG_PARAM, *PTIPC_SYS_CFG_PARAM;

/*************************************************
/	               SD卡相关参数
/*************************************************/
// 前端SD卡容量信息
typedef struct tagIPCSDCardInfo
{
	unsigned char bySdCardExist;                                      // sd卡是否存在，1:存在；0:不存在
	unsigned long dwTotalMBSize;                                      // 总容量
	unsigned long dwTotalRecMBSize;                                   // 总的录像空间
	unsigned long dwFreeRecMBSize;                                    // 剩余录像空间
} TIPC_SD_CARD_INFO, *PTIPC_SD_CARD_INFO;

// SD卡格式化参数
typedef struct tagIPCFormatSDCard
{
	int           bFormatted;                                         // 是否已经格式化
} TIPC_FORMAT_SD_CARD, *PTIPC_FORMAT_SD_CARD;
#ifdef WIN32
#pragma pack(pop)
#endif

#endif
