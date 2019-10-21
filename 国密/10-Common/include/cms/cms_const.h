/*****************************************************************************
   模块名      : cms_const 
   文件名      : cms_const.h
   相关文件    : 
   文件实现功能: cms_const.h 定义整个CMS系统的常量
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/22   1.0         liangli        创建
******************************************************************************/

#ifndef _CMS_CONST_H_
#define _CMS_CONST_H_

#include "kdvtype.h"
#include "cms/cms_landesc.h"
#include <string>


enum EAlarmType
{
	cu__AlarmTypeInvalid,	        // 无效的告警类型.
	cu__AlarmMove,			        // 移动侦测
	cu__AlarmInput,	                // 告警输入.
	cu__AlarmDiskfull,		        // 磁盘满.
	cu__AlarmVideoLost,		        // 视频源丢失.
	cu__AlarmIntelligent,	        // 智能告警.
	cu__AlarmVideo,			        // 视质轮巡告警。
	cu__AlarmExceptionEvent,		//异常事件消息
	cu__AlarmDefocus,				//虚焦检测
	cu__AlarmSceneChange,			//场景变换
	cu__AlarmRegionInvasion,		//区域入侵
	cu__AlarmRegionLeaving,			//区域离开
	cu__AlarmObjectTaken,			//物品拿取
	cu__AlarmObjectLeft,			//物品遗留
	cu__AlarmPeopleGather,			//人员聚集
	cu__AlarmCorgon,				//警戒线
	cu__AlarmAudioAbnormal,			//声音异常
	cu__AlarmLumaChange,			//亮度变化
};

/* 公共常量定义，业务无特殊要求请使用它们的常量值 */
const u32 UUID_STRING_LEN = 32;
const u32 USER_NAME_MAX_LEN = 256;
const u32 USER_PASSWORD_MAX_LEN = 256;
const u32 TMP_BUFFER_LEN = 256;

const u32 NAME_MAX_LEN = 256;
const u32 NAME_SHORT_MAX_LEN = 256;
const u32 COMPANY_NAME_MAX_LEN = 256;
// 用户名请参见USER_NAME_MAX_LEN
const u32 DOMAIN_NAME_MAX_LEN = 256;
const u32 MAXLEN_DESC = 255;    //描述的最大长度

const u32 DESCRIPTION_MAX_LEN = 512;
const u32 DEPARTMENT_NUM_MAXLEN = 256;
const u32 TELEPHONE_MAX_LEN = 256;
const u32 EMAIL_MAX_LEN = 256;

#define NTF_BUF_MAX_SIZE            (25*1024)   //25K
#define MAX_NET_NUM	4   // 平台最多支持的网段
#define TOTAL_STREAMS_IN_EVERY_SWITCH_CHANNEL        ( 4 ) // 每个交换通道包含码流数
#define CMS_MSG_TIMEOUT             (35)        //35秒
#define CMS_PTZ_MAX_PRI_LEV         (10)        //最大PTZ控制权限：10
#define CMS_PTZ_DEFAULT_HOLD_TIME   (60)        //PTZ控制默认把持时间：60秒

typedef std::string TNatProtocolType;
#define NAT_PROTOCOL_TCP            ("TCP")
#define NAT_PROTOCOL_UDP            ("UDP")

/* 公配文件中用于标识32位还是64位的字符 */
#define CMS_ARCH_X86_32BIT          ("X86")
#define CMS_ARCH_X86_64BIT          ("X86_64")
#define CMS_ARCH_X86_32BIT_ONDISP   ("32bit")
#define CMS_ARCH_X86_64BIT_ONDISP   ("64bit")


/* 设备常量 */
const u32 DEVICE_NAME_MAX_LEN = NAME_MAX_LEN;
const u32 DEVICE_ALIAS_MAX_LEN = NAME_SHORT_MAX_LEN;
const s32 DEVICE_TYPE_INVALID = 0; // 非法设备类型  设备类型定义 ?还有哪些类型
const s32 DEVICE_TYPE_ENCODER = 1; // 编码设备
const s32 DEVICE_TYPE_DECODER = 2; // 解码设备
const s32 DEVICE_TYPE_CODECER = 3; // 编解设备
const s32 DEVICE_TYPE_TV_WALL = 4; // 电视墙设备
const s32 DEVICE_CAP_INVALID = 0;                   // 非法设备能力类型
// 伪能力用途: a. 表示不关注具体设备能力的设备类型的设备能力， b. 表示以设备为单位进行划归的设备类型的划归能力
const s32 DEVICE_CAP_DUMMY = 6;                     // 设备能力-伪能力(能力数值固定为1)
const s32 DEVICE_CAP_VIDEOSOURCE = 1;               // 设备能力-视频源
const s32 DEVICE_CAP_LOCAL_STORAGE = 2;             // 设备能力-本地存储(比如一些编码器可能具有存储能力而不一些不具有)
const s32 DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT = 3; // 设备能力-并口告警
const s32 DEVICE_CAP_ENCODE_CHANNEL = 4;            // 设备能力-编码通道
const s32 DEVICE_CAP_DECODE_CHANNEL = 5;            // 设备能力-解码通道
const s32 DEVICE_CAP_DECODE_OUTPUTCHANNEL = 13;     // 设备能力-解码输出通道 比如公司的D04F设备,就具有多输出通道
const s32 DEVICE_CAP_LABEL_ENCODE_NVR = 7;          // 设备能力标签-NVR
const s32 DEVICE_CAP_LABEL_ENCODE_SVR = 8;          // 设备能力标签-SVR
const s32 DEVICE_CAP_LABEL_ENCODE_IPC_BULLET = 9;   // 设备能力标签-IPC枪机
const s32 DEVICE_CAP_LABEL_ENCODE_IPC_DOME = 10;    // 设备能力标签-IPC球机
const s32 DEVICE_CAP_LABEL_ENCODE_GENERAL = 11;     // 设备能力标签-普通编码器
const s32 DEVICE_CAP_LABEL_ENCODE_WIRELESS = 12;    // 设备能力标签-无线编码器
const s32 DEVICE_CAP_LABEL_GBDEVICE = 14;           // 设备能力标签-国标设备
const s32 DEVICE_CAP_LABEL_CONFDEVICE = 15;         // 设备能力标签-会议终端
const s32 DEVICE_CAP_LABEL_GBRAWSTREAM = 16;        // 设备能力标签-国标原始码流

const int DEVICE_LINEMODE_INTRANET = 0;    //CMU与PU在同一网络
const int DEVICE_LINEMODE_EXTRANET = 1;    //CMU与PU在不同网络
const int DEVICE_LINEMODE_CMU_OUTNAT_PU_OUTNAT = 0;    //CMU在NAT外，PU在NAT外标记
const int DEVICE_LINEMODE_CMU_OUTNAT_PU_INNAT = 1;     //CMU在NAT外，PU在NAT内标记
const int DEVICE_LINEMODE_CMU_INNAT_PU_OUTNAT = 2;     //CMU在NAT内，PU在NAT外标记
const int DEVICE_LINEMODE_CMU_INNAT_PU_INNAT = 4;      //CMU在NAT内，PU在NAT内标记

const u32 DEVICE_NETADDRESS_MAX_LEN = 40;
const u32 DEVICE_FIXADDRESS_MAX_LEN = 128;
const u32 DEVICE_MANUFACTURE_NAME_MAX_LEN = 64;
const u32 DEVICE_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* 设备组常量 */
const u32 DEVICEGROUP_NAME_MAX_LEN = NAME_MAX_LEN;
const u32 DEVICEGROUP_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* CU用户常量 */
const u32 CUSER_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 CUSER_PASSWORD_MAX_LEN = USER_PASSWORD_MAX_LEN;
const u32 CUSER_TYPE_SUPERADMIN   = 0;   // 超级管理员
const u32 CUSER_TYPE_COMMONADMIN  = 1;   // 管理员
const u32 CUSER_TYPE_OPERATOR     = 2;   // 操作员 CU用户类型
const u32 CUSER_COMPANY_NAME_MAX_LEN = COMPANY_NAME_MAX_LEN;
const u32 CUSER_DEPARTMENT_NUM_MAXLEN = DEPARTMENT_NUM_MAXLEN;
const u32 CUSER_TELEPHONE_MAX_LEN = TELEPHONE_MAX_LEN;
const u32 CUSER_EMAIL_MAX_LEN = EMAIL_MAX_LEN;
const u32 CUSER_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* CU用户组常量 */
const u32 CUSERGROUP_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 CUSERGROUP_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/* Tas用户常量 */
const u32 TASUSER_NAME_MAX_LEN = USER_NAME_MAX_LEN;
const u32 TASUSER_PASSWORD_MAX_LEN = USER_PASSWORD_MAX_LEN;
const u32 TASUSER_COMPANY_NAME_MAX_LEN = COMPANY_NAME_MAX_LEN;
const u32 TASUSER_DEPARTMENT_NUM_MAXLEN = DEPARTMENT_NUM_MAXLEN;
const u32 TASUSER_TELEPHONE_MAX_LEN = TELEPHONE_MAX_LEN;
const u32 TASUSER_EMAIL_MAX_LEN = EMAIL_MAX_LEN;
const u32 TASUSER_DESCRIPTION_MAX_LEN = DESCRIPTION_MAX_LEN;

/*解码器能力常量*/
const u32 DECODER_MAX_DEC_CAP_PER_HDMI = 16;   //解码器的每个HDMI的最大解码能力

//用户认证信息定义
#define AUTH_DEFAULT_AES_KEY        (s8 *)"kedacom.shanghai"
#define AUTH_DEFAULT_AES_MODE       (2)
#define AUTH_DEFAULT_AES_INITKEY    (s8 *)"2003091919190900"

//{{码流交换相关，huangzhichun
typedef std::string TMidaType;
#define MEDIA_TYPE_VIDEO      "VIDEO"
#define MEDIA_TYPE_AUDIO      "AUDIO"
#define MEDIA_TYPE_DATA       "DATA"
#define MEDIA_TYPE_VIDEO_LOW  "video"
#define MEDIA_TYPE_AUDIO_LOW  "audio"
#define MEDIA_TYPE_DATA_LOW   "data"
#define MEDIA_TYPE_UNKNOWN    "UNKNOWN"


//{{TMediaTransProto
typedef std::string TMediaTransport;
#define MEDIA_TRANS_RTP       "RTP"         //实时传输协议,  MediaType[VIDEO] TransProto[RTP] TransChannelList[RTP,RTCP]
                                            //              MediaType[AUDIO] TransProto[RTP] TransChannelList[RTP,RTCP]
#define MEDIA_TRANS_RTSP      "RTSP"
#define MEDIA_TRANS_DATA      "DATA"        //单通道数据传输协议，MediaType[DATA] TransProto[DATA] TransChannelList[DATA]
#define MEDIA_TRANS_DATARTP   "DATARTP"     //双通道数据传输协议，MediaType[DATA] TransProto[DATARTP] TransChannelList[RTP,RTCP]

typedef std::string TLowerTransport;
#define LOWER_TRANS_UDP    "UDP"
#define LOWER_TRANS_TCP    "TCP"
#define LOWER_TRANS_UNKNOWN    "UNKNOWN"

typedef std::string TTransCastMode;
#define TRANS_CAST_MODE_UNICAST   "unicast"
#define TRANS_CAST_MODE_MULTICAST "multicast"
#define TRANS_CAST_MODE_UNKNOWN   "unknown"
//}}TMediaTransProto

typedef std::string TTransChannel;
#define MEDIA_TRANS_CHN_RTP       "RTP"
#define MEDIA_TRANS_CHN_RTCP      "RTCP"
#define MEDIA_TRANS_CHN_DATA      "DATA"
#define MEDIA_TRANS_CHN_UNKNOWN   "UNKNOWN"

//交换类型
typedef std::string TSwitchType;
#define  SWITCH_TYPE_M2P    "M2P"
#define  SWITCH_TYPE_P2P    "P2P"

//交换方式
typedef std::string TSwitchMode;
#define  SWITCH_MODE_SNG    "SNG"   //单播,非直传
#define  SWITCH_MODE_MUL    "MUL"   //组播
#define  SWITCH_MODE_DRT    "DRT"   //单播,直传

//直传模式[设备有直传能力情况下],目前只对浏览等实时交换有效
typedef int TDirectTransMode;
#define  DIRECTTRANS_MODE_ONDEMAND     0   //按需直传
#define  DIRECTTRANS_MODE_ALWAYS       1   //始终直传

typedef std::string TChannelType;
#define  CHANNEL_TYPE_RECVONLY    "recvonly"
#define  CHANNEL_TYPE_SENDONLY    "sendonly"
//#define  CHANNEL_TYPE_SENDRECV    "sendrecv"
//#define  CHANNEL_TYPE_INACTIVE    "inactive"
//#define  CHANNEL_TYPE_UNDEFINED   "undefined"

#define MEDIA_MANUFACTURE_KEDACOM "kedacom"

typedef std::string TMediaStreamType;
#define MEDIA_STREAM_TYPE_ES    "ES"
#define MEDIA_STREAM_TYPE_PS    "PS"
#define MEDIA_STREAM_TYPE_TS    "TS"
#define MEDIA_STREAM_TYPE_RTP_ES "RTP_ES"
#define MEDIA_STREAM_TYPE_RTP_PS "RTP_PS"
#define MEDIA_STREAM_TYPE_RTP_TS "RTP_TS"
//}}码流交换相关

#define INVALID_CHN_NO  0xFFFFFFFF
#define INVALID_NETSEG_NO 0xFFFFFFFF

//告警相关常量
#define    ALARM_RESUME_TIME_INVALIDVAL  "2038-01-01T10:15:14+08:00"  // 告警恢复时间非法值，等于它时表示告警恢复时间无效

//告警标志
#define    ALARM_FLAG_OCCUR         "occur"           //产生
#define    ALARM_FLAG_RESTORE       "restore"         //恢复

//告警类型
#define    ALARM_TYPE_PIN             "pin"          //并口
#define    ALARM_TYPE_MOVE            "move"         //移动监测
#define    ALARM_TYPE_VIDLOST         "vid lost"     //视频源丢失
#define    ALARM_TYPE_DISKFULL        "disk full"    //磁盘满
//#define    ALARM_TYPE_CHN_UNAVAIL     "chn unavail"  //通道不可用
#define    ALARM_TYPE_INTELLIGENT     "intelligent"  //智能
#define    ALARM_TYPE_VIDEOPOLL       "video poll"  //视质轮询
//添加基础智能告警子类型
#define    ALARM_TYPE_EXCEPTION_EVENT "exception event" //异常事件消息
#define    ALARM_TYPE_DEFOCUS         "defocus"         //虚焦检测
#define    ALARM_TYPE_SCENE_CHANGE    "sceme change"    //场景变换
#define    ALARM_TYPE_REGION_INVASION "region invasion" //区域入侵
#define    ALARM_TYPE_REGION_LEAVING  "region leaving"  //区域离开
#define    ALARM_TYPE_OBJECT_TAKEN    "object taken"    //物品拿取
#define    ALARM_TYPE_OBJECT_LEFT     "object left"     //物品遗留
#define    ALARM_TYPE_PEOPLE_GATHER   "people gather"   //人员聚集
#define    ALARM_TYPE_CORGON          "corgon"          //警戒线
#define    ALARM_TYPE_AUDIO_ABNORMAL  "audio abnormal"  //声音异常
#define    ALARM_TYPE_LUMA_CHANGE     "luma change"     //亮度变化

#define    ALARM_TYPE_GPS_OVERSPEED                "gps overspeed"      //gps 超速
#define    ALARM_TYPE_SIM_DATA_SERVICE_OVERUSED    "sim 数据流量 超出"    //数据流量超标
//国标2016
#define    ALARM_TYPE_ILLEGAL_DEV_DISASSEMBLY       "illegal device dissassembly" //设备非法拆解
#define    ALARM_TYPE_DEVICE_OVER_HEAT              "device over heat"            //设备过热
#define    ALARM_TYPE_DEVICE_OVER_COLD              "device over cold"            //设备过冷
#define    ALARM_TYPE_ARTIFICIAL_SCENE              "artificial scene"            //人工视频
#define    ALARM_TYPE_OBJECT_MOTION                 "object motion"               //移动检测
#define    ALARM_TYPE_TRIP_WIRE                     "trip wire"                   //绊线
#define    ALARM_TYPE_MOVING_AROUND                 "moving around"               //徘徊检测
#define    ALARM_TYPE_INDIVIDUAL_FLOW_STATISTICS    "individual flow statistcs"    //流量检测
#define    ALARM_TYPE_INDIVIDUAL_THICKNESS_STATISTICS "individual thickness detect" //密度统计
#define    ALARM_TYPE_VIDEO_EXCEPTION               "video exception"              //视频异常
#define    ALARM_TYPE_MOVING_SWIFTLY                "moving swiftly"               //快速移动
#define    ALARM_TYPE_STORAGE_DEV_DISK_FAULT        "storage device disk fault"    //存储设备磁盘故障
#define    ALARM_TYPE_STORAGE_DEV_FAN_FAULT         "storage device fan fault"    //存储设备风扇故障
#define    ALARM_TYPE_GO_AGAINST_TRAFFIC            "go against traffic"            //逆向行驶


//进程间信号常量定义
//用户自定义信号起始值
#ifndef USER_SIGRTMIN
#define USER_SIGRTMIN  34
#endif

//用户自定义信号结束值
#ifndef USER_SIGRTMAX
#define USER_SIGRTMAX  64
#endif
//cms使用CMS_SIGNAL_START(44)到CMS_SIGNAL_END(64)范围内的自定义常量信号
#define    CMS_SIGNAL_START           USER_SIGRTMIN+10    //34-44可能被系统库使用
#define    CMS_SIGNAL_END             USER_SIGRTMAX       //
#define    CMS_SIGNAL_RELAOD_CFG      CMS_SIGNAL_START    //通知重新加载配置文件

//定义一个无效经纬度，便于选择性生成xml
#define INVALID_LONGITUDE         65533.0
#define INVALID_LATITUDE          65533.0


//获取告警类型名称对应值接口
static u32 ConverAlarmTypeStrToInt( const std::string& AlarmType )
{
	if (ALARM_TYPE_PIN == AlarmType)
	{
		return cu__AlarmInput;
	}
	else if (ALARM_TYPE_MOVE == AlarmType)
	{
		return cu__AlarmMove;
	}
	else if (ALARM_TYPE_VIDLOST == AlarmType)
	{
		return cu__AlarmVideoLost;
	}
	else if (ALARM_TYPE_DISKFULL == AlarmType)
	{
		return cu__AlarmDiskfull;
	}
	else if (ALARM_TYPE_INTELLIGENT == AlarmType)
	{
		return cu__AlarmIntelligent;
	}
	else if (ALARM_TYPE_VIDEOPOLL == AlarmType)
	{
		return cu__AlarmVideo;
	}
	else if (ALARM_TYPE_EXCEPTION_EVENT == AlarmType)
	{
		return cu__AlarmExceptionEvent;
	}
	else if (ALARM_TYPE_DEFOCUS == AlarmType)
	{
		return cu__AlarmDefocus;
	}
	else if (ALARM_TYPE_SCENE_CHANGE == AlarmType)
	{
		return cu__AlarmSceneChange;
	}
	else if (ALARM_TYPE_REGION_INVASION == AlarmType)
	{
		return cu__AlarmRegionInvasion;
	}
	else if (ALARM_TYPE_REGION_LEAVING == AlarmType)
	{
		return cu__AlarmRegionLeaving;
	}
	else if (ALARM_TYPE_OBJECT_TAKEN == AlarmType)
	{
		return cu__AlarmObjectTaken;
	}
	else if (ALARM_TYPE_OBJECT_LEFT == AlarmType)
	{
		return cu__AlarmObjectLeft;
	}
	else if (ALARM_TYPE_PEOPLE_GATHER == AlarmType)
	{
		return cu__AlarmPeopleGather;
	}
	else if (ALARM_TYPE_CORGON == AlarmType)
	{
		return cu__AlarmCorgon;
	}
	else if (ALARM_TYPE_AUDIO_ABNORMAL == AlarmType)
	{
		return cu__AlarmAudioAbnormal;
	}
	else if (ALARM_TYPE_LUMA_CHANGE == AlarmType)
	{
		return cu__AlarmLumaChange;
	}
	else 
	{
		return cu__AlarmTypeInvalid;
	}
}

//平台2.0端口使用规划
//STUN (UDP)
const u32 DEFAULT_STUN_SVR_PORT = 3478;

//Natagent内部socket端口
//一个kedaplayer就要封装一个nalib（3个端口），现分配1000个端口给NATAGENT使用
//const u32 NATAGENT_INNER_SOCK_PORT_START = 3500;
//const u32 NATAGENT_INNER_SOCK_PORT_END   = 4499;

//PROXY
const u32 PROXY_SIP_UDP_PORT = 5060;
const u32 PROXY_SIP_TCP_PORT = 5060;

//3AS
const u32 TAS_SIP_UDP_PORT = 5061;
const u32 TAS_SIP_TCP_PORT = 5061;

//PMS
const u32 PMS_SIP_UDP_PORT = 5062;
const u32 PMS_SIP_TCP_PORT = 5062;
const u32 PMS_PU_LISTEN_PORT = 5510;
const u32 PMS_REC_LISTEN_PORT = 10086;
const u32 PMS_NMS_LISTEN_PORT = 1727;

//PMCS
const u32 PMCS_SIP_UDP_PORT = 5063;
const u32 PMCS_SIP_TCP_PORT = 5063;

//CMSSDK_GW
const u32 CMSSDK_SIP_UDP_PORT = 5069;
const u32 CMSSDK_SIP_TCP_PORT = 5069;

//CUI 1.0
const u32 CUI_1_OSP_TCP_PORT = 1722;

//CMU
const u32 CMU_SIP_UDP_PORT = 5070;
const u32 CMU_SIP_TCP_PORT = 5070;

//ALS
const u32 ALS_SIP_UDP_PORT = 5071;
const u32 ALS_SIP_TCP_PORT = 5071;

//TVS
const u32 TVS_SIP_UDP_PORT = 5072;
const u32 TVS_SIP_TCP_PORT = 5072;

//UAS
const u32 UAS_SIP_UDP_PORT = 5073;
const u32 UAS_SIP_TCP_PORT = 5073;

//RCS
const u32 RCS_SIP_UDP_PORT = 5074;
const u32 RCS_SIP_TCP_PORT = 5074;

//MPS
const u32 MPS_SIP_UDP_PORT = 5075;
const u32 MPS_SIP_TCP_PORT = 5075;

//NRU
const u32 NRU_SIP_UDP_PORT = 5076;
const u32 NRU_SIP_TCP_PORT = 5076;

//RMS
const u32 RMS_OSP_TCP_PORT = 5080;

// CAPS 图片抓拍/微信平台 服务器.
const u32 CAPS_SIP_UDP_PORT = 5090;

// DPS
const u32 DPS_SIP_UDP_PORT = 5095;
const u32 DPS_SIP_CMSSDK_PORT = 5096;
#define  DPS_ROUTER_TCP_URL "tcp://*:5097"
#define  DPS_DEALER_TCP_URL "tcp://*:5098"



//GBS
const u32 GBS_SIP_UDP_PORT = 5511;
const u32 GBS_SIP_TCP_PORT = 5511;
const u32 GBS_SIP_UDP_PORT2 = 5082;
const u32 GBS_SIP_TCP_PORT2 = 5082;

//SECURITY_MODULE
const u32 SECURITY_MODULE_SIP_UDP_PORT = 5083;


//G800 监听端口
const u32 G800_MMU_PORT = 5600; // Mmu的MmuPort 当前与PUI的范围存在冲突风险，但是由于一个机器上不会布置那么多PUI故不会有问题
const u32 G800_HTTP_PORT = 5084;  // Mtu的HttpServicePort
const u32 G800_MTU_PORT = 6600; // Mtu的MtuPort

//VTDU SIP端口范围：5101-5200
const u32 VTDU_SIP_UDP_PORT = 5101;
const u32 VTDU_SIP_TCP_PORT = 5101;
const u32 VTDU_SIP_PORT_END = 5200;
const u32 VTDU_MEDIA_UDP_START_PORT = 40000;
const u32 VTDU_MEDIA_TCP_START_PORT = 40000;
const u32 VTDU_MEDIA_UDP_M2P_PORT = 8002;
const u32 VTDU_MEDIA_TCP_M2P_PORT = 8002;

//CUI SIP端口范围：5201-5300
const u32 CUI_SIP_UDP_PORT = 5201;
const u32 CUI_SIP_TCP_PORT = 5201;
const u32 CUI_TCP_DAT_RECV_PORT = 5300;
const u32 CUI_SIP_PORT_END = 5300;
const u32 CUI_WS_PORT = 9080;
const u32 CUI_CU_MAX_NUM = 100;

//PUI SIP端口范围：5301-5400  5511与5600已经分别被GBS和G800占用了
const u32 PUI_SIP_UDP_PORT = 5301;
const u32 PUI_SIP_TCP_PORT = 5301;
const u32 PUI_SIP_PORT_END = 5400;
const u32 PUI_VSIP_LISTEN_PORT = 5510;
const u32 PUI_VSIP_PORT_END = 5600;
const u32 PUI_MEDIA_UDP_PORT = 7000;
const u32 PUI_MEDIA_TCP_PORT = 7000;

const u32 OVF_SIP_UDP_PORT = 5401;
const u32 OVF_SIP_TCP_PORT = 5401;
const u32 PUI_OVFUDP_LISTEN_PORT = 3702;  //onvif设备的udp端口，默认3702
const u32 OVF_MEDIA_UDP_PORT = 7100;
const u32 OVF_MEDIA_TCP_PORT = 7100;

//PUI_GB SIP端口范围：5800-5899  
const u32 PUIGB_SIP_UDP_PORT = 5800;
const u32 PUIGB_SIP_TCP_PORT = 5800;

//PMC_fcgi SIP端口范围：5601-5700
const u32 PMC_FCGI_SIP_UDP_PORT = 5601;
const u32 PMC_FCGI_SIP_TCP_PORT = 5601;
const u32 PMC_FCGI_SIP_PORT_END = 5700;


//CBB底层模块端口使用情况。范围: 10001-39999,上层业务不能使用该范围内的端口
const u32 KDM_CBB_PORT_START = 10001;

const u32 KD_DEV_FINDER_UDP_PORT = 10001;//录像子系统中的组播搜索端口（原5690，现改为10001）
const u32 SCS_OSP_TCP_PORT = 10002;

const u32 KDM_CBB_PORT_END = 39999;



/*
CBB底层模块目前所绑定的端口与cms1.0保持不变。

#define NSMNRUPORT      9005    //NRU与NSM通信端口
#define NSMVTDUPORT     9006    //VTDU与NSM通信端口

osp内部socket端口：20010~20200

medianet 20400~20599

mediapack 11000~12000

rpctrl   20600~20799


之前随机绑定的端口改为使用固定端口来绑定。

asf定时器：20800~20849

kds定时器：20850~20899

kdmfile定时器：20900~20949

rpctrl定时器：20950~20999

dataswitch内部使用端口由随机绑定改为使用固定分配的端口

起始端口：21050

结束端口：21099


nru下载端口：24000

nru码流起始端口：25000

放像端口为录像端口加10000，单台nru留有2500路的余地。

无线前端运行在平台上的模块端口范围: 12000 ~ 12500
proxyserver 接收前端码流端口 12000（ps 起来后一直监听该端口，启动脚本可以修改该端口）
连接 gmss 端口 12001（ps 起来后一直监听该端口，内部采用逻辑：前端码流端口+1）
iperf 测速端口  12500（界面写死  该端口只在测速时候占用，测速结束后不占用该端口）

当前设备连接 ProxyServer 时，ProxyServer 为前端设备分配50个端口（无论
> 设备是什么样的能力）
> 
> 例如：第n个前端连接至 ProxyServer 后，ProxyServer 将 35000 + ( n -
> 1 ) * 50 至 35000 + n * 50 端口分配给该前端。（35000 是 ProxyServer 的起始
> 端口）
> 
> 注：前端设备定义表中的最大编码通道数为16，前端起始端口60000，音频呼叫
> 通道号为0（音频呼叫只有一个通道），录像和视频通道请查看各个设备能力集。
>                                                                                                                                        
>           |               前端端口计算               |  ProxyServer端口计算                                                            
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   实时视  |       起始端口 + 4 * 通道号 - 1000       |   ProxyServer 根据前端起始端口号，和前端设备能力和左边的计算方式，每计算出前端  
>           |                                          |                                                                                 
>     频    |                                          |                 一个端口，则在50个端口内，分配一个（顺序分配）                  
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   实时音  |           实时视频 主卡Port + 2          |                                                                                 
>           |                                          |                                                                                 
>     频    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   音频呼  |     起始端口 + 4 * 16 + 4 *通道号 + 2    |                                                                                 
>           |                                          |                                                                                 
>     叫    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   录像视  |  起始端口 + 4 * (最大编码通道+1 +通道号) |                                                                                 
>           |                                          |                                                                                 
>     频    |                  - 1000                  |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>           |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>   录像音  |             录像视频Port + 2             |                                                                                 
>           |                                          |                                                                                 
>     频    |                                          |                                                                                 
> ----------+------------------------------------------+-------------------------------------------------------------------------------- 
>           |                                          |                                                                                 
>                                                                                                                                        

*/

///////////////////////////////////////////////////////////////////////////////
//huangzhichun:工具宏

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#define CMS_MIN(x,y) ((x)<(y)?(x):(y))
#define CMS_MAX(x,y) ((x)>(y)?(x):(y))

#ifndef TRUE_STR
#define TRUE_STR "true"
#endif

#ifndef FALSE_STR
#define FALSE_STR "false"
#endif

#define BoolStr(bVar) (bVar? TRUE_STR : FALSE_STR)
#define BOOL32ToBool(bBool32) (bBool32 ? true:false)
#define BoolToBOOL32(bBool) (bBool ? TRUE:FALSE)

#define VERIFY_AND_BREAK(bResult) \
    if(!(bResult)) \
{ \
    break; \
}

#define VERIFY_AND_RETURN_RET(bResult, nRtnError) \
    if(!(bResult)) \
{ \
    return nRtnError; \
}

#define SAFE_DEL_PTR(ptr) do{ delete (ptr); (ptr) = NULL; } while(false)
#define SAFE_DEL_ARRAY_PTR(ptr) do{ delete[] (ptr); (ptr) = NULL; } while(false)
#endif // _CMS_CONST_H_

#define ARRAY_SIZE(ar) sizeof(ar)/sizeof(ar[0])
//end of file
