/*****************************************************************************
  模块名      : 终端协议接口模块
  文件名      : VSIPDefine.h
  相关文件    : 
  文件实现功能: 
  作者        : 卿晋
  版本        : V3.0  Copyright(C) 1997-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2005/6/14    3.0         卿晋        创建
******************************************************************************/
#ifndef VSIP_DEFINES_H
#define VSIP_DEFINES_H

//add by Royan 20160415
#define VSIP_SUBS_TYPE_AZIMUTH                         (0x20010001)
#define VSIP_SUBS_TYPE_INTELLIGENT_ALARM               (0x20030001)

//zhuzhm: 070906 add
//#define VSIP_MULTI_CHAN_SUPPORT

//zhuzhm:070907 add for epoll support 5000 connect
#define VSIP_PERFORM_IMPROVE

//zhuzhm:080129 add MPEG4_VGA, MPEG4_QQCIF resolution
#define VSIP_ADD_NEW_RESOLUTION
//zhuzhm:070913 for my debug
//#define ZHUZHM_DEBUG

#define VSIP_HEADER_LEN		7

#define VSIP_PROTOCOL_VERSION						   0x0100
#define VSIP_PUID_LENGTH                               32
#define VSIP_DOMAIN_LENGTH                             16 
// --------------------------------------------------------------------------
// ****************           VSIP Message types             ****************
// --------------------------------------------------------------------------
#define VSIP_MSG_TYPE_NULL                             0x00
#define VSIP_MSG_TYPE_D_PING_REQ                       0x01
#define VSIP_MSG_TYPE_D_PING_RESP                      0x02
#define VSIP_MSG_TYPE_IC_CAPABILITIES_REQ              0x03
#define VSIP_MSG_TYPE_IC_CAPABILITIES_RESP             0x04
#define VSIP_MSG_TYPE_MC_STARTDEVICE_REQ               0x05
#define VSIP_MSG_TYPE_MC_STOPDEVICE_REQ                0x06
#define VSIP_MSG_TYPE_MC_SETCONFIG_REQ                 0x07
#define VSIP_MSG_TYPE_MC_GETCONFIG_REQ                 0x08
#define VSIP_MSG_TYPE_MC_GETCONFIG_RESP                0x09
#define VSIP_MSG_TYPE_MC_SENDCOMMAND_REQ               0x0A
#define VSIP_MSG_TYPE_E_NOTIFY                         0x0B
#define VSIP_MSG_TYPE_E_SUBSCRIBE_REQ                  0x0C
#define VSIP_MSG_TYPE_ER_ERROR                         0x0D
#define VSIP_MSG_TYPE_MC_SETPROPCONFIG_REQ             0x0E
#define VSIP_MSG_TYPE_MC_GETPROPCONFIG_REQ             0x0F
#define VSIP_MSG_TYPE_MC_GETPROPCONFIG_RESP            0x10
#define VSIP_MSG_TYPE_MC_REPORT_REQ                    0x11
#define VSIP_MSG_TYPE_MC_REPORT_NOTIFY                 0x12
#define VSIP_MSG_TYPE_D_CONTENT_TYPE_SWITCH_REQ        0x13
#define VSIP_MSG_TYPE_D_CONTENT_TYPE_SWITCH_RESP       0x14
#define VSIP_MSG_TYPE_MC_STARTDEVICEEX_REQ             0x15
#define VSIP_MSG_TYPE_MC_STOPDEVICEEX_REQ              0x16
#define VSIP_MSG_TYPE_E_SUBSCRIBE_EX_REQ               0x17
#define VSIP_MSG_TYPE_MC_SENDCOMMANDEX_REQ             0x18
#define VSIP_MSG_TYPE_SET_DEV_GUID_REQ                 0x19


#define VSIP_MSG_TYPE_START_REC_REQ                    0x21
#define VSIP_MSG_TYPE_STOP_REC_REQ                     0x22  
#define VSIP_MSG_TYPE_SET_TIMELY_REC_INFO_REQ          0x23
#define VSIP_MSG_TYPE_SET_PERIODLY_REC_INFO_REQ        0x24
#define VSIP_MSG_TYPE_SET_CHANNEL_REC_BASIC_INFO_REQ   0x25
#define VSIP_MSG_TYPE_SET_REC_SYS_INFO_REQ             0x26
#define VSIP_MSG_TYPE_QUERY_REC_FILE_REQ               0x27
#define VSIP_MSG_TYPE_START_EXPORT_REC_FILE_REQ        0x28
#define VSIP_MSG_TYPE_REC_FILE_OPERATION_REQ           0x29
#define VSIP_MSG_TYPE_DEL_REC_FILE_REQ                 0x2A
#define VSIP_MSG_TYPE_REC_SEEK_LOCATION_REQ            0x2B
#define VSIP_MSG_TYPE_GET_REC_STATUS_REQ               0x2C
#define VSIP_MSG_TYPE_REC_FILE_START_PLAY_REQ          0x2D
#define VSIP_MSG_TYPE_GET_TIMELY_REC_INFO_REQ          0x2E
#define VSIP_MSG_TYPE_GET_REC_STATUS_RSP               0x2F

#define VSIP_MSG_TYPE_GET_TIMELY_REC_INFO_RSP          0x30
#define VSIP_MSG_TYPE_GET_PERIODLY_REC_INFO_REQ        0x31
#define VSIP_MSG_TYPE_GET_PERIODLY_REC_INFO_RSP        0x32
#define VSIP_MSG_TYPE_GET_CHANNEL_REC_BASIC_INFO_REQ   0x33
#define VSIP_MSG_TYPE_GET_CHANNEL_REC_BASIC_INFO_RSP   0x34
#define VSIP_MSG_TYPE_GET_REC_SYS_INFO_REQ             0X35
#define VSIP_MSG_TYPE_GET_REC_SYS_INFO_RSP             0X36
#define VSIP_MSG_TYPE_DEL_REC_RECORD_REQ               0x37
#define VSIP_MSG_TYPE_QUERY_REC_FILE_RSP               0x38
#define VSIP_MSG_TYPE_PROGRESS_CALL                    0x39    
#define VSIP_MSG_TYPE_REC_STATUS_CHANGE_NOTIFY         0x3A   
#define VSIP_MSG_TYPE_REC_FILE_START_PLAYID_NOTIFY     0x3B 
#define VSIP_MSG_TYPE_QUERY_REC_FILE_ACK               0x3C  
#define VSIP_MSG_TYPE_FILE_EXORT_PROGRESS_NOTIFY       0X3D  
#define VSIP_MSG_TYPE_STOP_EXPORT_REC_FILE_REQ         0x3E
#define VSIP_MSG_TYPE_SET_NMS_PARAM_REQ                0x3F
#define VSIP_MSG_TYPE_GET_NMS_PARAM_REQ                0x40
#define VSIP_MSG_TYPE_GET_NMS_PARAM_RSP                0x41
#define VSIP_MSG_TYPE_VIDEO_SOURCE_STATE_NOTIFY        0x42
#define VSIP_MSG_TYPE_QUERY_REC_FILE_NUM_REQ           0x43 
#define VSIP_MSG_TYPE_QUERY_REC_FILE_NUM_ACK           0x44
#define VSIP_MSG_TYPE_DEV_REGEDIT_REQ                  0x45
#define VSIP_MSG_TYPE_DEV_REGEDIT_RSP                  0x46

#define VSIP_MSG_TYPE_MC_OPEARTION_REQ                 0x50
#define VSIP_MSG_TYPE_MC_OPEARTION_RSP                 0x51 
#define VSIP_MSG_TYPE_E_NOTIFY_EXTEND                  0x52

#ifdef VSIP_MULTI_CHAN_SUPPORT
#define VSIP_MSG_GET_DEV_MULTIENCCHAN_PARAM_REQ        0x60
#define VSIP_MSG_GET_DEV_MULTIENCCHAN_PARAM_RESP       0x61
#define VSIP_MSG_SET_DEV_MULTIENCCHAN_PARAM_REQ        0x62
#endif

//hzc_Transparent_Chan_20110324{{
#define VSIP_MSG_TYPE_MC_OPEARTION_REQ_EX              0x6A
#define VSIP_MSG_TYPE_MC_OPEARTION_RSP_EX              0x6B
//}}
#define VSIP_MSG_TYPE_CU_OPEARTION_REQ_EX              0x6C 
#define VSIP_MSG_TYPE_CU_OPEARTION_RSP_EX              0x6D
#define VSIP_MSG_TYPE_CU_OPEARTION_NOTIFY              0x6E

//GPS相关
#define VSIP_MSG_TYPE_SET_CFG_REQ                      0x72
#define VSIP_MSG_TYPE_GET_CFG_REQ               	   0x73
#define VSIP_MSG_TYPE_GET_CFG_ACK               	   0x74

#define VSIP_MSG_TYPE_MC_OPEARTION_REQ_EX2             0x77
#define VSIP_MSG_TYPE_MC_OPEARTION_RSP_EX2             0x78
#define VSIP_MSG_TYPE_USER_MSG_DIALOG_INFO             0x79
#define VSIP_MSG_TYPE_MC_SET_BACKENDCAPABILITIES_REQ   0x7A //设置后端能力
#define VSIP_MSG_TYPE_MC_STARTDEVICE_RSP               0x7B //开始传输音视频码流应答
#define VSIP_MSG_TYPE_MC_SET_STREAMPROPERTY_REQ        0x7C //设置流媒体属性

#define VSIP_MSG_TYPE_REC_FILE_DOWN_NOTIFY			   0x80
#define VSIP_MSG_TYPE_START_PLAY_GPSDATA_REQ           0x81 
#define VSIP_MSG_TYPE_START_PLAY_GPSDATA_RESP          0x82   
#define VSIP_MSG_TYPE_STOP_PLAY_GPSDATA_REQ            0x83 
#define VSIP_MSG_TYPE_PLAY_GPSDATA_NOTIFY              0x84

#define VSIP_MSG_TYPE_USER_MSG_SUBSCRIBE_REQ           0x85   
#define VSIP_MSG_TYPE_USER_MSG_SUBSCRIBE_RSP           0x86 
#define VSIP_MSG_TYPE_USER_MSG_SUBSCRIBE_NOTIFY        0x87

//daiyi transdata start
#define VSIP_MSG_SET_TRANSDATA_REQ                 (u8)0x96
#define VSIP_MSG_SET_TRANSDATA_RSP                 (u8)0x97
#define VSIP_MSG_GET_TRANSDATA_REQ                 (u8)0x98
#define VSIP_MSG_GET_TRANSDATA_RSP                 (u8)0x99

//支持录像日历查询
#define VSIP_MSG_TYPE_QUERY_REC_LIST_REQ           (u8)0x9A
#define VSIP_MSG_TYPE_QUERY_REC_LIST_RSP           (u8)0x9B


//支持natagent的pu码流调度信令
//实时码流
#define VSIP_MSG_NA_START_SEND_MEDIA_REQ                  (u8)0xB0
#define VSIP_MSG_NA_START_SEND_MEDIA_RSP                  (u8)0xB1
#define VSIP_MSG_NA_START_SEND_MEDIA_ACK                  (u8)0xB2
#define VSIP_MSG_NA_STOP_SEND_MEDIA_REQ                   (u8)0xB3
#define VSIP_MSG_NA_STOP_SEND_MEDIA_RSP                   VSIP_MSG_TYPE_ER_ERROR

#define VSIP_MSG_NA_START_RECV_MEDIA_REQ                  (u8)0xB4
#define VSIP_MSG_NA_START_RECV_MEDIA_RSP                  (u8)0xB5
#define VSIP_MSG_NA_START_RECV_MEDIA_ACK                  (u8)0xB6
#define VSIP_MSG_NA_STOP_RECV_MEDIA_REQ                   (u8)0xB7
#define VSIP_MSG_NA_STOP_RECV_MEDIA_RSP                   VSIP_MSG_TYPE_ER_ERROR

//历史码流播放
#define VSIP_MSG_NA_REC_PLAY_START_REQ                    (u8)0xB8
#define VSIP_MSG_NA_REC_PLAY_START_RSP                    (u8)0xB9
#define VSIP_MSG_NA_REC_PLAY_START_ACK                    (u8)0xBA
#define VSIP_MSG_NA_REC_PLAY_PROGRESS_NTF                 VSIP_MSG_TYPE_PROGRESS_CALL    //播放进度通知
#define VSIP_MSG_NA_REC_PLAY_OPERATION_REQ                VSIP_MSG_TYPE_REC_FILE_OPERATION_REQ //Play控制
#define VSIP_MSG_NA_REC_PLAY_OPERATION_RSP                VSIP_MSG_TYPE_ER_ERROR
#define VSIP_MSG_NA_REC_PLAY_SEEK_REQ                     VSIP_MSG_TYPE_REC_SEEK_LOCATION_REQ  //Seek操作
#define VSIP_MSG_NA_REC_PLAY_SEEK_RSP                     VSIP_MSG_TYPE_ER_ERROR


//历史码流下载
#define VSIP_MSG_NA_REC_EXPORT_START_REQ                  (u8)0xBB
#define VSIP_MSG_NA_REC_EXPORT_START_RSP                  (u8)0xBC
#define VSIP_MSG_NA_REC_EXPORT_START_ACK                  (u8)0xBD



// --------------------------------------------------------------------------
// ****************              Connection Types            ****************
// --------------------------------------------------------------------------
#define VSIP_CONNECTION_NULL                        0x00
#define VSIP_CONNECTION_UNICAST                     0x01
#define VSIP_CONNECTION_MULTICAST                   0x02
#define VSIP_CONNECTION_TCPSERVER                   0x04
#define VSIP_CONNECTION_TCPCLIENT                   0x08
#define VSIP_CONNECTION_TCP                         0x0C   // 2 bits are used because Server and client are the same
#define VSIP_CONNECTION_BROADCAST                   0x10
#define VSIP_CONNECTION_SSL                         0x20

// --------------------------------------------------------------------------
// ****************           VSIP Error Codes               ****************
// --------------------------------------------------------------------------
#define VSIP_ERROR_CODE_SUCCESS                     0x00    // Success
#define VSIP_ERROR_CODE_FAIL                        0x01    // Failure
#define VSIP_ERROR_CODE_UNKNOWNCAPABILITY           0x02    // Unknown Capability
#define VSIP_ERROR_CODE_REQUIRE_SERVER              0x03    // We want to be a server
#define VSIP_ERROR_CODE_REQUIRE_CLIENT              0x04    // We want to be a client
#define VSIP_ERROR_CODE_NOT_SUPPORT                 0x05    // 不支持的功能

// --------------------------------------------------------------------------
// ****************         VSIP Config Value Types          ****************
// --------------------------------------------------------------------------
#define VSIP_VALUE_TYPE_INVALID                     0x00
#define VSIP_VALUE_TYPE_CHAR                        0x01
#define VSIP_VALUE_TYPE_SHORT                       0x02
#define VSIP_VALUE_TYPE_INT                         0x03
#define VSIP_VALUE_TYPE_STRING                      0x04
#define VSIP_VALUE_TYPE_UINT                        0x05

// --------------------------------------------------------------------------
// ****************           Content Type Value             ****************
// --------------------------------------------------------------------------
#define VSIP_CONTENT_TYPE_NONE                      0x00
#define VSIP_CONTENT_TYPE_VSIP                      0x01
#define VSIP_CONTENT_TYPE_STREAM                    0x02
#define VSIP_CONTENT_TYPE_VSIP_SSL_CLIENT           0x03
#define VSIP_CONTENT_TYPE_VSIP_SSL_SERVER           0x04
#define VSIP_CONTENT_TYPE_STREAM_SSL_CLIENT         0x05
#define VSIP_CONTENT_TYPE_STREAM_SSL_SERVER         0x06

// --------------------------------------------------------------------------
// ****************              Capabilities                ****************
// --------------------------------------------------------------------------
#define VSIP_INVALID_CAP_TYPE						0xFF
#define VSIP_INVALID_CAP_VALUE						0xFF
#define VSIP_CAP_ENABLE								0x01
#define VSIP_CAP_DISABLE							0x00

// for VSIP_ENTITY_MAXENCBITRATE
#define VSIP_BITRATE_128							0x01
#define VSIP_BITRATE_256							0x02
#define VSIP_BITRATE_512							0x03
#define VSIP_BITRATE_768							0x04
#define VSIP_BITRATE_896							0x05
#define VSIP_BITRATE_1024							0x06
#define VSIP_BITRATE_2048							0x07
#define VSIP_BITRATE_3072							0x08
#define VSIP_BITRATE_4096							0x09
#define VSIP_BITRATE_8192							0x0A

//0x01~0x0C为基本能力集
#define VSIP_ENTITY_DEVICE		                    0x00
#define VSIP_ENTITY_VIDEODECODER                    0x01
#define VSIP_ENTITY_VIDEOENCODER                    0x02
#define VSIP_ENTITY_AUDIODECODER                    0x03
#define VSIP_ENTITY_AUDIOENCODER                    0x04
#define VSIP_ENTITY_SERIALPORT                      0x05
#define VSIP_ENTITY_INPUTPIN                        0x06
#define VSIP_ENTITY_OUTPUTPIN                       0x07
#define VSIP_ENTITY_PTZMOTOR                        0x08
#define VSIP_ENTITY_PTZCONTROL                      0x09
#define VSIP_ENTITY_ARCHIVER                        0x0A
#define VSIP_ENTITY_VIDEOIN		                    0x0C
//Ox0D~0xFF是扩展能力集
#define VSIP_ENTITY_ETHERNET	                    0x0D   
#define VSIP_ENTITY_WIRELESS	                    0x0E
#define VSIP_ENTITY_XIANTELE	                    0x0F
#define VSIP_ENTITY_VIDIOADPTER	                    0x10
#define VSIP_ENTITY_VIDIOCOMP	                    0x11
#define VSIP_ENTITY_VIDEOSRCPOLL                    0x12
#define VSIP_ENTITY_AUDIOMIX                        0x13
#define VSIP_ENTITY_SHADEAREANUM                    0x14
#define VSIP_ENTITY_DEVPOWERTYPE                    0x15
#define VSIP_ENTITY_MAXSTREAMSEND                   0x16
#define VSIP_ENTITY_MAXSTREAMDIRSEND                0x17
#define VSIP_ENTITY_ENCRYPT		                    0x18
#define VSIP_ENTITY_RECORDER	                    0x19
#define VSIP_ENTITY_PLAYER			                0x1A
#define VSIP_ENTITY_FILEDOWNLOAD	                0x1B
#define VSIP_ENTITY_VGAOUT			                0x1C
#define VSIP_ENTITY_VIDEOOUT		                0x1D  
#define VSIP_ENTITY_OSDCOLOR						0x1E
#define VSIP_ENTITY_PLATFORMTYPE					0x1F
#define VSIP_ENTITY_MOTIONDETECTAREANUM				0x20
#define VSIP_ENTITY_SNAPSHOTCHANNUM					0x21
#define VSIP_ENTITY_SLOTINFONUM						0x22
#define VSIP_ENTITY_CBRVBR							0x23        // 是否能够设置图像质量
#define VSIP_ENTITY_MAXENCBITRATE					0x24		// 最大有效码率
#ifdef VSIP_MULTI_CHAN_SUPPORT
#define VSIP_ENTITY_MULTICHAN                       0x25        // 是否支持双流
#endif
#define VSIP_ENTITY_MATRIX                          0x26        // 是否支持矩阵接入
#define VSIP_ENTITY_VIDEOENCSLAVECHANNELNUM         0x2F        // 视频编码辅流通道数  20090509
#define VSIP_ENTITY_TRANSPORTDATA					0x29        // 是否使用透明数据传输
#define VSIP_ENTITY_GPS         					0x35        // GPS功能
#define VSIP_ENTITY_EXTINPUTPIN      				0x36        // 扩展并口能力(只有NVR使用)add G.J. 20091020
#define VSIP_ENTITY_3G                              0x37        // 3G，(该扩展能表示是无线前端)fanxg-20131118

#define VSIP_ENTITY_DECODEEXT                       0x3B        // 是否支持解码器第三方扩展  //add GJ 20100317
//{{{{{{{{{{{{{{{ add by Royan 20140327 分辨率数值化
#define VSIP_ENTITY_SUPPORT_RESOLUTION_WH           0x3D
//}}}}}}}}}}}}}}} add by Royan 20140327 分辨率数值化

#define VSIP_ENTITY_NEW_OSD                         0x47        // 新OSD
//{{{{{{{{{{{{{{{ add by Royan 20140801 自定义扩展能力集 
#define VSIP_ENTITY_SUPPORT_CUSTOM_EXT_CAPS         0x48        // 是否支持自定义扩展能力集
//}}}}}}}}}}}}}}} add by Royan 20140801 自定义扩展能力集

#define KDM_ENTITY_VIDEOIN_NAMESYNC                 0x49		//视频源名称同步能力集 


#define VSIP_ENTITY_RECLIST_QRY                     0x50        // 是否支持录像日历查询
//modify by Royan 20140304 通道状态通知信息
#define VSIP_ENTITY_CHANNL_STATE_REPORT             0x51	    // 是否支持通道状态上报
#define VSIP_ENTITY_REPLAY_MAXSPEED                 0x52        // 最大放像倍速
#define VSIP_ENTITY_REPLAY_MAXSPEED_ALLFRAME        0x53        // 最大满帧放像倍速

#define VSIPDEF_ENTITY_INTELLIGENT_NOTIFY           0x55        // 支持基础智能告警（订阅类透明通道方式）

//D04F解码器HMDI相关能力集类型定义如下：
#define VSIP_ENTITY_HMDI_COUNT                      0x60        // HMDI总数
#define VSIP_ENTITY_HMDI_1_DEC_CHAN_COUNT           0x61        // HMDI_1的解码通道数
#define VSIP_ENTITY_HMDI_2_DEC_CHAN_COUNT           0x62        // HMDI_2的解码通道数
#define VSIP_ENTITY_HMDI_3_DEC_CHAN_COUNT           0x63        // HMDI_3的解码通道数
#define VSIP_ENTITY_HMDI_4_DEC_CHAN_COUNT           0x64        // HMDI_4的解码通道数

#define VSIP_ENTITY_SMART_TAB                       0x97        // 是否支持智能拼接
#define VSIP_ENTITY_SUPPORT_BD_TRACK_MODE           0xD7        // 是否支持智能枪球联动

#define VSIP_ENTITY_AUDIO_EX                        0xDB        // 音频扩展，为了区分新旧前端，特引入一个新的能力集
#define VSIP_ENTITY_VIDEO_CLOCK_RATE                0xDC        // 是否支持视频采样率属性
#define VSIP_ENTITY_SUPPORT_BACKEND_CAPABILITIES    0xDD        // 是否支持后端能力

#define VSIP_ENTITY_DECODER_AUDIO_CALL              0xED        // 是否支持解码器呼叫

#define VSIP_ENTITY_EXT_OSD						    0xF0        // 是否支持多行字幕
#define VSIP_ENTITY_SUPPORT_TOTAL_PICTURE           0xF2        // 是否支持整体图元 
#define VSIP_ENTITY_MAX_FRAMERATE				    0xF4        // 支持的最大编码帧率
#define VSIP_ENTITY_DEVTYPE                         0xF5	    // 设备类型能力集
#define VSIP_ENTITY_CTRLTYPE                        0xF6	    // 控制台类型能力集
#define VSIP_ENTITY_MAX_ENC_CHANNEL					0xF7	    // 最大编码通道数能力集//nvr128

#define VSIP_ENTITY_CHARSET_ENCODING                0xFC        // 支持多字符集编码

// --------------------------------------------------------------------------
// ****************              平台能力                    ****************
// --------------------------------------------------------------------------
#define VSIP_CAP_SUPPORT_NVR128		0x00000001                  //nvr128

// --------------------------------------------------------------------------
// ****************                  Commands                ****************
// --------------------------------------------------------------------------
// Video commands (VSIP_ENTITY_VIDEOENCODER)
#define VSIP_COMMAND_SEND_KEYFRAME						0x01  // Have no argument 
// Other commands
#define VSIP_COMMAND_PROPRIETARY						0x02  // For proprietary commands
// Set TCP connection as VITAL
//#define VSIP_COMMAND_SET_CONNECTION_VITAL				0x03  // A broken VITAL connection will cause all streams to be terminated
// Reset the wireless passkey (VSIP_ENTITY_WIRELESS_INTERFACE)
//#define VSIP_COMMAND_RESET_WLS_PASS_KEY				0x04  // Reset the wireless passkey (802.11 or SPCF)
#define VSIP_COMMAND_SHOW_ALARM_TIMER					0xFF

#define VSIP_COMMAND_START_NAT_DETECT					0x05  //request mt start nat detect
#define VSIP_COMMAND_STOP_NAT_DETECT					0x06  //request mt stop nat detect
#define VSIP_COMMAND_SEND_SER_DATA						0x07
#define VSIP_COMMAND_SEND_COM_DATA						0x08  //request mt send serial data of camera
#define VSIP_COMMAND_SEND_SER_DATA_DEC					0x09
// --------------------------------------------------------------------------
// ****************          Command    Arguments            ****************
// --------------------------------------------------------------------------
//VSIP_COMMAND_PROPRIETARY
#define VSIP_COMMAND_PROPRIETARY_REBOOT					0x01
#define VSIP_COMMAND_PROPRIETARY_SAVE_SETTINGS			0x02
#define VSIP_COMMAND_PROPRIETARY_LOAD_SETTINGS			0x03

//VSIP_COMMAND_SHOW_ALARM_TIMER
#define VSIP_COMMAND_SHOW_ALARM_TIMER_START				0x01
#define VSIP_COMMAND_SHOW_ALARM_TIMER_STOP				0x02
#define VSIP_COMMAND_SHOW_ALARM_TIMER_HIDE				0x03



// --------------------------------------------------------------------------
// ****************                   Events                 ****************
// --------------------------------------------------------------------------
#define VSIP_EVENT_TYPE_INPUTPIN_STATECHANGE		0x01
#define VSIP_EVENT_TYPE_VIDEOINPUT_STATECHANGE		0x02
#define VSIP_EVENT_TYPE_OUTPUTPIN_STATECHANGE       0xAF
#define VSIP_EVENT_TYPE_EN_MOTD_ALERTCHANGE			0xFD
#define VSIP_EVENT_TYPE_DE_MOTD_ALERTCHANGE			0xEE
#define VSIP_EVENT_TYPE_CONFIG_CHANGED				0xFF
#define VSIP_EVENT_TYPE_VIENC_TYPE_CHANGED		    0xEA
#define VSIP_EVENT_TYPE_BITCURRENT_STATECHANGED     0xEB
#define VSIP_EVENT_TYPE_REC_INFO_CHANGED            0xEC
#define VSIP_EVENT_TYPE_SYS_STATE_NOTIFY            0xED
#define VSIP_EVENT_TYPE_REC_STATE_NOTIFY            0xFC
#ifdef VSIP_MULTI_CHAN_SUPPORT
#define VSIP_EVENT_TYPE_MULTIENCCHAN_STATECHANGE 0x03
#endif
//modify by Royan 20140304 通道状态通知信息
#define VSIP_EVENT_TYPE_CHANNL_STATE_NOTIFY         0xEF


// --------------------------------------------------------------------------
// ****************              Event Arguments             ****************
// --------------------------------------------------------------------------
//state
#define	STATE_ON									0x00
#define STATE_OFF									0x01



// --------------------------------------------------------------------------
// ****************              Network Config              ****************
// --------------------------------------------------------------------------
#define CONFIG_NETWORK_PORT                         0x0001
#define CONFIG_NETWORK_CONNECTION_TYPE              0x0002
#define CONFIG_NETWORK_SUPPORTED_CONNECTION_TYPE    0x0003

// --------------------------------------------------------------------------
// ****************               Video Config               ****************
// --------------------------------------------------------------------------
#define CONFIG_VIDEO_ATTRIBUTE_BRIGHTNESS			0x1001
#define CONFIG_VIDEO_ATTRIBUTE_CONTRAST				0x1002
#define CONFIG_VIDEO_ATTRIBUTE_SATURATION			0x1007
#define CONFIG_VIDEO_ATTRIBUTE_SHARPNESS		    0xc400   //锐度配置

#define CONFIG_VIDEO_INFO_SUPPORTED_DATA_FORMAT     0x1022
#define CONFIG_VIDEO_COMPRESSION_DATA_FORMAT        0x1011
#define CONFIG_VIDEO_COMPRESSION_FRAME_RATE         0x1013
#define CONFIG_VIDEO_MAX_FRAME_RATE			        0xc402 //20150710 Royan 最大帧率
#define CONFIG_VIDEO_COMPRESSION_BIT_RATE           0x1016
#define CONFIG_VIDEO_COMPRESSION_INTRA_INTERVAL     0x1017
//{{{{{{{{{{{{{{{ add by Royan 20140327 分辨率数值化
#define CONFIG_VIDEO_COMPRESSION_DATA_FORMAT_WH     0x101A 
#define CONFIG_VIDEO_COMPRESSION_DATA_RESOLUTION_WH 0x101B 
#define CONFIG_VIDEO_INFO_SUPPORTED_DATA_FORMAT_WH  0x1020 
#define CONFIG_VIDEO_COMPRESSION_CURRENT_DATA_RESOLUTION_WH 0x1021 //todo 待确定????
//}}}}}}}}}}}}}}} add by Royan 20140327 分辨率数值化
#define CONFIG_VIDEO_COMPRESSION_MAX_QUANTIZATION	0x10FF
#define CONFIG_VIDEO_COMPRESSION_MIN_QUANTIZATION	0x10FE
#define CONFIG_VIDEO_COMPRESSION_IMG_QUALITY		0x10FD
//{{{{{{{{{{{{{{{ add by Royan 20140528 高级编码参数-编码等级
#define CONFIG_VIDEO_COMPRESSION_MP_VALUE			0xC403
//}}}}}}}}}}}}}}} add by Royan 20140528 高级编码参数-编码等级

#define CONFIG_VIDEO_INFO_SOURCE_INPUT              0x1023
#define CONFIG_VIDEO_INFO_SOURCE_INPUT_PORTS        0x10FC

#define CONFIG_VIDEO_CLOCK_RATE         			0x1025  // 视频采样率

#define CONFIG_VIDEO_PLAYER_BUFFER_NUM				0x10F9
#define CONFIG_VIDEO_PLAYER_ORIGIN_BUFFER_NUM		0x10F8
#define CONFIG_VIDEO_PLAYER_DATA_FORMAT             0x10FA
#define CONFIG_VIDEO_PLAYER_DEC_DEAL_LEV			0x10FB

#define CONFIG_VIDEO_SHADE_NUM                      0x10EF
#define CONFIG_VIDEO_SHADE_STATE                    0x10EE
#define CONFIG_VIDEO_SHADE_X_POS                    0x10ED
#define CONFIG_VIDEO_SHADE_Y_POS                    0x10EC
#define CONFIG_VIDEO_SHADE_WIDTH                    0x10EB  
#define CONFIG_VIDEO_SHADE_HEIGTH                   0x10EA


//add G.J. 20100225
// --------------------------------------------------------------------------
// ****************            GPS config            ****************
// --------------------------------------------------------------------------
#define CONFIG_TYPE_GPS                             1

#define CONFIG_GPS_ENABLE                           1
#define CONFIG_GPS_COLLECT_INTERVAL                 2
#define CONFIG_GPS_NOTIFY_INTERVAL                  3


// --------------------------------------------------------------------------
// ****************               Audio Config               ****************
// --------------------------------------------------------------------------
#define CONFIG_AUDIO_ATTRIBUT_VOLUME                0x2002
#define CONFIG_AUDIO_COMPRESSION_INPUT_TYPE	        0x2010
#define CONFIG_AUDIO_COMPRESSION_DATA_FORMAT        0x2011
#define CONFIG_AUDIO_INFO_SUPPORTED_DATA_FORMAT     0x2021
#define CONFIG_AUDIO_PLAYER_AEC_ENABLE			    0x20FF // 回声抵消状态，u8，MTI_DISABLE-无效, MTI_ENABLE-有效
#define CONFIG_AUDIO_ATTRIBUT_MUTE					0x20FE
#define CONFIG_AUDIO_ATTRIBUT_DUMB					0x20FD // 哑音状态，u8，MTI_DISABLE-无效, MTI_ENABLE-有效
#define CONFIG_AUDIO_VALID_INPUT_TYPE				0x20FC
#define CONFIG_AUDIO_ATTRIBUT_MIX                   0x20F0 // 混音状态，u8，MTI_DISABLE-无效, MTI_ENABLE-有效

#define CONFIG_AUDIO_COMPRESSION_SAMPLING_RATE      0x2012  // 采样率，u32，单位为bps，比如32kbps为32000
#define CONFIG_AUDIO_COMPRESSION_SAMPLE_BITS        0x2015  //采样位数，u8，比如8位、16位、32位等
#define CONFIG_AUDIO_COMPRESSION_CHANNEL            0x2013  //单双声道，u8，1-单声道，2-双声道
#define CONFIG_AUDIO_COMPRESSION_SAMPLES            0x2016  //每帧样本数，u32
#define CONFIG_AUDIO_COMPRESSION_DATA_FORMAT2       0x2017  //当前采用的音频数据格式，u32，继承0x2011的全部取值，并扩展为最多32种
#define CONFIG_AUDIO_COMPRESSION_BIT_RATE           0x2018  //音频码率，u32，单位为bps，比如32kbps为32000
#define CONFIG_AUDIO_CLOCK_RATE                     0x2019  //音频时间戳采样率，u32，单位为HZ，可选取值为1000和真实采样率（比如，8000、16000、44100等）（若不支持该参数，默认为1000）



// --------------------------------------------------------------------------
// ****************              Serial Port Config          ****************
// --------------------------------------------------------------------------
#define CONFIG_SERIAL_PORT_BAUD_RATE                0x3001
#define CONFIG_SERIAL_PORT_LINE_DRIVER              0x3009
#define CONFIG_SERIAL_PORT_DATA_INPUT				0x30FF
#define CONFIG_SERIAL_PORT_SUPPORTED_TYPE			0x30FE

// --------------------------------------------------------------------------
// ****************              IO Pin Config               ****************
// --------------------------------------------------------------------------
#define CONFIG_IO_PIN_STATE                         0x4001
#define CONFIG_IO_PIN_INPUT_OUTPUT					0x40FF
#define CONFIG_IO_PIN_ALARM_DETECT_STATE			0x40FE
#define CONFIG_IO_PIN_ALARM_DETECT_START_TIME		0x40FD
#define CONFIG_IO_PIN_ALARM_DETECT_DURATION			0x40FC
#define CONFIG_IO_PIN_ALARM_RELATE_ENCID			0x40F6
#define CONFIG_IO_PIN_ALARM_OSD_STATE				0x40FB
#define CONFIG_IO_PIN_ALARM_OSD_LEFT_MARGIN			0x40FA
#define CONFIG_IO_PIN_ALARM_OSD_TOP_MARGIN			0x40F9
#define CONFIG_IO_PIN_ALARM_OSD_NAME				0x40F8
#define CONFIG_IO_PIN_ALARM_OSD_COLOR				0x40F7
#define CONFIG_IO_PIN_ALARM_INIT_STATE				0x40F5
#define CONFIG_IO_PIN_ALARM_ETH_STATE				0x40F4
#define CONFIG_IO_PIN_INPUT_ENABLE_STATE			0x40F3 //(1024并口通道使用) add G.J. 20091020 

// --------------------------------------------------------------------------
// ****************              System Config               ****************
// --------------------------------------------------------------------------
#define CONFIG_SYSTEM_REBOOTREQUIRED                0x5001 
#define CONFIG_SYSTEM_DEVICE_NAME                   0x5002 
#define CONFIG_SYSTEM_TYPE_NAME		                0x5003 
#define CONFIG_SYSTEM_DEVICE_TYPE                   0x50FE 
#define CONFIG_SYSTEM_DEVICE_TIME                   0x50FF 

// --------------------------------------------------------------------------
// ****************              PPPOE Config                ****************
// --------------------------------------------------------------------------
#define CONFIG_PPPOE_STATE							0x50FD
#define CONFIG_PPPOE_USERNAME						0x50FC
#define CONFIG_PPPOE_PASSWORD						0x50FB
#define CONFIG_PPPOE_AUTODIAL_STATE					0x50FA
#define CONFIG_PPPOE_AUTODIAL_WAIT					0x50F9
#define CONFIG_PPPOE_AUTODIAL_TRY					0x50F8
#define CONFIG_PPPOE_DEBUG_STATE					0x50F7
#define CONFIG_PPPOE_LCPECHO_SEND_INTERVAL			0x50F6
#define CONFIG_PPPOE_LCPECHO_RETRY_TIMES			0x50F5

// --------------------------------------------------------------------------
// ****************   Xian Telecom Pin and Banner Config     ****************
// --------------------------------------------------------------------------
#define CONFIG_XIANTELECOM_DETECT_ENABLE            0X50EF
#define CONFIG_XIANTELECOM_RELATED_SERIALID         0X50EE
#define CONFIG_XIANTELECOM_RELATED_PINID            0X50ED
#define CONFIG_XIANTELECOM_DISPALY_BANNER_ENABLE    0X50EC
#define CONFIG_XIANTELECOM_MAX_TEMPERATURE          0X50EB
#define CONFIG_XIANTELECOM_MIN_TEMPERATURE          0X50EA
#define CONFIG_XIANTELECOM_MAX_HUMIDITY             0X50E9
#define CONFIG_XIANTELECOM_MIN_HUMIDITY             0X50E8
#define CONFIG_XIANTELECOM_REPORT_INTERVAL          0X50E7
#define CONFIG_XIANTELECOM_MAXUNITNUM		        0X50E6
#define CONFIG_XIANTELECOM_ALARM_ALIAS0             0X50D0
#define CONFIG_XIANTELECOM_ALARM_ALIAS1             0X50D1
#define CONFIG_XIANTELECOM_ALARM_ALIAS2             0X50D2
#define CONFIG_XIANTELECOM_ALARM_ALIAS3             0X50D3
#define CONFIG_XIANTELECOM_ALARM_ALIAS4             0X50D4
#define CONFIG_XIANTELECOM_ALARM_ALIAS5             0X50D5
#define CONFIG_XIANTELECOM_ALARM_ALIAS6             0X50D6
#define CONFIG_XIANTELECOM_ALARM_ALIAS7             0X50D7
#define CONFIG_XIANTELECOM_ALARM_ALIAS8             0X50D8
#define CONFIG_XIANTELECOM_ALARM_ALIAS9             0X50D9
#define CONFIG_XIANTELECOM_UNITNAME0		        0X50DA
#define CONFIG_XIANTELECOM_UNITNAME1		        0X50DB
#define CONFIG_XIANTELECOM_UNITNAME2		        0X50DC
#define CONFIG_XIANTELECOM_UNITNAME3		        0X50DD
#define CONFIG_XIANTELECOM_SENSORNUM0				0X0000//zlq add 用来和别名叠加
#define CONFIG_XIANTELECOM_SENSORNUM1				0X0100
#define CONFIG_XIANTELECOM_SENSORNUM2				0X0200
#define CONFIG_XIANTELECOM_SENSORNUM3				0X0300
#define CONFIG_XIANTELECOM_PIN_NUM0		            0X50A0//zlq add
#define CONFIG_XIANTELECOM_PIN_NUM1		            0X50A1
#define CONFIG_XIANTELECOM_PIN_NUM2		            0X50A2
#define CONFIG_XIANTELECOM_PIN_NUM3		            0X50A3
#define CONFIG_XIANTELECOM_PIN_NUM4		            0X50A4
#define CONFIG_XIANTELECOM_PIN_NUM5		            0X50A5
#define CONFIG_XIANTELECOM_PIN_NUM6		            0X50A6
#define CONFIG_XIANTELECOM_PIN_NUM7		            0X50A7
#define CONFIG_XIANTELECOM_PIN_NUM8		            0X50A8
#define CONFIG_XIANTELECOM_PIN_NUM9		            0X50A9

#define CONFIG_XIANTELECOM_BANNER_XPOS              0X50CF
#define CONFIG_XIANTELECOM_BANNER_YPOS              0X50CE
#define CONFIG_XIANTELECOM_BANNER_WIDTH             0X50CD
#define CONFIG_XIANTELECOM_BANNER_HEIGHT            0X50CC
#define CONFIG_XIANTELECOM_BANNER_BMPWIDTH          0X50CB
#define CONFIG_XIANTELECOM_BANNER_BMPHEIGHT         0X50CA
#define CONFIG_XIANTELECOM_BANNER_FONTWIDTH         0X50C9
#define CONFIG_XIANTELECOM_BANNER_FONTHEIGHT        0X50C8
#define CONFIG_XIANTELECOM_BANNER_BACKCOLORRED      0X50C7
#define CONFIG_XIANTELECOM_BANNER_BACKCOLORBLUE     0X50C6
#define CONFIG_XIANTELECOM_BANNER_BACKCOLORGREEN    0X50C5
#define CONFIG_XIANTELECOM_BANNER_BGDCLARITY        0X50C4
#define CONFIG_XIANTELECOM_BANNER_PICCLARITY        0X50C3
#define CONFIG_XIANTELECOM_BANNER_RUNMODE           0X50C2
#define CONFIG_XIANTELECOM_BANNER_RUNSPEED          0X50C1
#define CONFIG_XIANTELECOM_BANNER_RUNTIMES          0X50C0
#define CONFIG_XIANTELECOM_BANNER_RUNFOREVER        0X50BF
#define CONFIG_XIANTELECOM_BANNER_HALTTIME          0X50BE
#define CONFIG_XIANTELECOM_BANNER_ALIGHSTYLE        0X50BD
#define CONFIG_XIANTELECOM_BANNER_RELATEDPINID      0X50BC
#define CONFIG_XIANTELECOM_BANNER_CHARAC_COLOR      0X50BB
#define CONFIG_XIANTELECOM_SENSORNUM_MASK			0X50FF //zlq add 屏蔽采集单元特殊处理


// --------------------------------------------------------------------------
// ****************              Motion Detect Config        ****************
// --------------------------------------------------------------------------
//#define CONFIG_MOTD_UPPER_THRESHOLD 	            0xA001
//#define CONFIG_MOTD_LOWER_THRESHOLD		            0xA002
//#define CONFIG_MOTD_MASK_STRING 		            0xA009
#define CONFIG_MOTD_STATE 				            0xA00B
//#define CONFIG_MOTD_SENSITIVITY 		            0xA0FF

//NEW
#define CONFIG_MOTD_DETAREA_NUM						0xA00A
#define CONFIG_MOTD_X_POS							0xA003
#define CONFIG_MOTD_Y_POS							0xA004
#define CONFIG_MOTD_WIDTH							0xA005
#define CONFIG_MOTD_HEIGTH							0xA006
#define CONFIG_MOTD_ALARM_RATE						0xA007
#define CONFIG_MOTD_RESUME_RATE						0xA008
//***************************************************************************
//                           Proprietory Configurations                                
//***************************************************************************

// --------------------------------------------------------------------------
// ****************               Network config             ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_NETWORK					0x3005
#define PROP_CONFIG_ID_NETWORK_IPADDR		            0x0001
#define PROP_CONFIG_ID_NETWORK_SUBNET		            0x0002
#define PROP_CONFIG_ID_NETWORK_GATEWAY		            0x0003
#define PROP_CONFIG_ID_NETWORK_DETPACKET				0x0004

// --------------------------------------------------------------------------
// ****************                VSIP config               ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_VSIP						0x300F
#define PROP_CONFIG_ID_VSIP_PORT						0x0001
#define PROP_CONFIG_ID_VSIP_MULTI_IPADDR		        0x0002
#define PROP_CONFIG_ID_VSIP_DISCOVERY_IPADDR		    0x0003

// --------------------------------------------------------------------------
// ****************               System config              ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_SYS						0x3001
#define PROP_CONFIG_ID_SYS_VERSION						0x00FF
#define PROP_CONFIG_ID_SYS_CUSTOM_MSG					0x00FE
#define PROP_CONFIG_ID_SYS_ALARM_CONTINUE				0x00FD
#define PROP_CONFIG_ID_SYS_SUBAREA_TYPE					0x00FC
#define PROP_CONFIG_ID_SYS_MAX_TRANSFER_NUM             0x00FB
#define PROP_CONFIG_ID_SYS_PUID                         0x00FA
#define PROP_CONFIG_ID_SYS_PUID_PASSWORD                0x00F9
#define PROP_CONFIG_ID_SYS_DEC_ISCOMBINED               0X00F8
#define PROP_CONFIG_ID_SYS_APPTYPE						0X00F7
#define PROP_CONFIG_ID_SYS_MATRIXVALID					0X00F6
//支持的字符编码类型
#define PROP_CONFIG_ID_SYS_SUPPORTED_CHARSET_ENCODING   0x0306
//当前的字符编码类型
#define PROP_CONFIG_ID_SYS_ACTIVE_CHARSET_ENCODING      0x0307

// --------------------------------------------------------------------------
// ****************              Serial config               ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_SERIAL					0x3002
#define PROP_CONFIG_ID_SERIAL_INIT_STATE				0x000C
#define PROP_CONFIG_ID_SERIAL_DEST_IPADDR				0x00FF
#define PROP_CONFIG_ID_SERIAL_DEST_PORT					0x00FE
#define PROP_CONFIG_ID_SERIAL_RECV_PORT					0x00FD
#define PROP_CONFIG_ID_SERIAL_CONN_TYPE					0x0019
#define PROP_CONFIG_ID_SERIAL_PROTO_TYPE				0x00FC
#define PROP_CONFIG_ID_SERIAL_SP_CONN_TYPE				0x00FB
#define PROP_CONFIG_ID_SERIAL_CONNECTION_ID             0x00FA
#define PROP_CONFIG_ID_SERIAL_DECCTRL_STATE             0x00F9
#define PROP_CONFIG_ID_SERIAL_DECCTRL_ID				0x00F8

// --------------------------------------------------------------------------
// ****************                Audio config              ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_AUDIO					0x3007
#define PROP_CONFIG_ID_AUDIOENC_INIT_STATE				0x0008
#define PROP_CONFIG_ID_AUDIOENC_CUR_STATE				0x00E8
#define PROP_CONFIG_ID_AUDIOENC_DEST_IPADDR				0x00FF
#define PROP_CONFIG_ID_AUDIOENC_DEST_PORT				0x00FE
#define PROP_CONFIG_ID_AUDIOENC_CONN_TYPE				0x00ED
#define PROP_CONFIG_ID_AUDIOENC_SP_CONN_TYPE			0x00FD
#define PROP_CONFIG_ID_AUDIODEC_INIT_STATE				0x000A
#define PROP_CONFIG_ID_AUDIODEC_CUR_STATE				0x00EA
#define PROP_CONFIG_ID_AUDIODEC_DEST_IPADDR				0x00FC
#define PROP_CONFIG_ID_AUDIODEC_DEST_PORT				0x00FB
#define PROP_CONFIG_ID_AUDIODEC_CONN_TYPE				0x00F9
#define PROP_CONFIG_ID_AUDIODEC_SP_CONN_TYPE			0x00FA
#define PROP_CONFIG_ID_AUTIOENC_ADD_DEST                0x00EB
#define PROP_CONFIG_ID_AUDIOENC_REMOVE_DEST				0X00EC

// --------------------------------------------------------------------------
// ****************            Video Encoder config          ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_VIDEOENC					0x3004
#define PROP_CONFIG_ID_VIDEOENC_INIT_STATE				0x000A
#define PROP_CONFIG_ID_VIDEOENC_CUR_STATE				0x0009
#define PROP_CONFIG_ID_VIDEOENC_DEST_IPADDR				0x0006
#define PROP_CONFIG_ID_VIDEOENC_DEST_PORT				0x00FF
#define PROP_CONFIG_ID_VIDEOENC_SP_CONN_TYPE			0x00EF
#define PROP_CONFIG_ID_VIDEOENC_CONN_TYPE				0x0010
#define PROP_CONFIG_ID_VIDEOENC_ENCODER_NUM				0x00FE
#define PROP_CONFIG_ID_VIDEOENC_ENCODER_USED			0x00EE
#define PROP_CONFIG_ID_VIDEOENC_CAP_VALID				0x00ED
#define PROP_CONFIG_ID_VIDEOENC_OSD_TIME_STATE			0x00FD
#define PROP_CONFIG_ID_VIDEOENC_OSD_STATE				0x00FC
#define PROP_CONFIG_ID_VIDEOENC_MP_VIDEO_IN_PORTS		0x00F9
#define PROP_CONFIG_ID_VIDEOENC_MP_STYLE				0x00F8
#define PROP_CONFIG_ID_VIDEOENC_POLL_STATE				0x00F5
#define PROP_CONFIG_ID_VIDEOENC_POLL_VIDEO_IN_PORT		0x00F4
#define PROP_CONFIG_ID_VIDEOENC_POLL_INTERVAL			0x00F3
#define PROP_CONFIG_ID_VIDEOENC_ADD_DEST				0x0025
#define PROP_CONFIG_ID_VIDEOENC_REMOVE_DEST				0x0026
#define PROP_CONFIG_SECTION_ID_VID_DEST					0x5004

// --------------------------------------------------------------------------
// ****************            Video Decoder config          ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_VIDEODEC					0x3012
#define PROP_CONFIG_ID_VIDEODEC_INIT_STATE				0x000A
#define PROP_CONFIG_ID_VIDEODEC_CUR_STATE				0x0009
#define PROP_CONFIG_ID_VIDEODEC_CONN_TYPE				0x0010
#define PROP_CONFIG_ID_VIDEODEC_DEST_IPADDR				0x0006
#define PROP_CONFIG_ID_VIDEODEC_DEST_PORT				0x00FF
#define PROP_CONFIG_ID_VIDEODEC_SP_CONN_TYPE			0x00EF
#define PROP_CONFIG_ID_VIDEODEC_RETRANS_STATE			0x00FE
#define PROP_CONFIG_ID_VIDEODEC_RETRANS_1ST_SPAN		0x00FD
#define PROP_CONFIG_ID_VIDEODEC_RETRANS_2EN_SPAN		0x00FC
#define PROP_CONFIG_ID_VIDEODEC_RETRANS_3RD_SPAN		0x00FB
#define PROP_CONFIG_ID_VIDEODEC_RETRANS_REJ_SPAN		0x00FA
#define PROP_CONFIG_ID_VIDEODEC_VIDEO_OUT_TYPE			0x00F9
#define PROP_CONFIG_ID_VIDEODEC_VGA_OUT_STATE			0x00F8
#define PROP_CONFIG_ID_VIDEODEC_VGA_OUT_FREQ			0x00F7
#define PROP_CONFIG_ID_VIDEODEC_THIRDPARTY              0x0080  //add GJ 20100317

// --------------------------------------------------------------------------
// ****************                OSD config                ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_OSD						0x301A
#define PROP_CONFIG_ID_OSD_STARVE_MODE					0x0003

// --------------------------------------------------------------------------
// ****************                Web config                ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_WEB						0x30FF
#define PROP_CONFIG_ID_WEB_USERNAME						0x0001
#define PROP_CONFIG_ID_WEB_PASSWORD						0x0002
#define PROP_CONFIG_ID_WEB_GUESTNAME					0x0003
#define PROP_CONFIG_ID_WEB_GUESTPW						0x0004

// --------------------------------------------------------------------------
// ****************            Video Input config            ****************
// --------------------------------------------------------------------------
#define PROP_CONFIG_SECTION_ID_VIN						0x30FE
#define PROP_CONFIG_ID_VIN_CAMERA_TYPE					0x0001
#define PROP_CONFIG_ID_VIN_CAMERA_ID					0x0002
#define PROP_CONFIG_ID_VIN_SERIAL_ID					0x0003
#define PROP_CONFIG_ID_VIN_LONGNAME						0x0005 
#define PROP_CONFIG_ID_VIN_NAME							0x0011
#define PROP_CONFIG_ID_VIN_OSD_CONTENT					0x0012
#define PROP_CONFIG_ID_VIN_OSD_LEFT_MERGIN				0x0013
#define PROP_CONFIG_ID_VIN_OSD_TOP_MERGIN				0x0014
#define PROP_CONFIG_ID_VIN_OSD_COLOR					0x0015
#define PROP_CONFIG_ID_VIN_CAMERA_DATA					0x00FF





//******************************************************************************************//
//                                  Parameter Values                                          
//******************************************************************************************//

// --------------------------------------------------------------------------
// ****************                Video Analog Formats      ****************
// --------------------------------------------------------------------------
#define VSIP_VIDEO_ANALOG_FORMAT_NONE               0x00
#define VSIP_VIDEO_ANALOG_FORMAT_NTSC               0x01
#define VSIP_VIDEO_ANALOG_FORMAT_PAL                0x02

// --------------------------------------------------------------------------
// ****************                Video Data Formats      ****************
// --------------------------------------------------------------------------
#define VSIP_VIDEO_FORMAT_NONE 			0x00
#define VSIP_VIDEO_FORMAT_H261_CIF 		0x01
#define VSIP_VIDEO_FORMAT_H261_QCIF		0x02
#define VSIP_VIDEO_FORMAT_SN4_CIF		0x04
#define VSIP_VIDEO_FORMAT_SN4_QCIF		0x08
#define VSIP_VIDEO_FORMAT_SN4_CUSTOM	0x10
#define VSIP_VIDEO_FORMAT_MPEG4			0x20
#define VSIP_VIDEO_FORMAT_SN4_2CIF		0x40
#define VSIP_VIDEO_FORMAT_SN4_4CIF		0x80
#ifdef VSIP_ADD_NEW_RESOLUTION
#define VSIP_VIDEO_FORMAT_MPEG4_VGA		0x100
#define VSIP_VIDEO_FORMAT_MPEG4_QQCIF	0x200
#else
#define VSIP_VIDEO_FORMAT_SN4_HALFD1	0x100
#define VSIP_VIDEO_FORMAT_SN4_2CIFH		0x200
#endif
#define VSIP_VIDEO_FORMAT_MPEG4_CIF		0x400
#define VSIP_VIDEO_FORMAT_MPEG4_QCIF	0x800
#define VSIP_VIDEO_FORMAT_MPEG4_2CIF	0x1000
#define VSIP_VIDEO_FORMAT_MPEG4_4CIF	0x2000
#define VSIP_VIDEO_FORMAT_H263			0x4000
#define VSIP_VIDEO_FORMAT_H264_CIF		0x8000
#define VSIP_VIDEO_FORMAT_H264_QCIF		0x00010000
#define VSIP_VIDEO_FORMAT_H264_2CIF		0x00020000
#define VSIP_VIDEO_FORMAT_H264_4CIF		0x00040000
#define VSIP_VIDEO_FORMAT_H264_QQCIF    0x00100000 //add 20090612
#define VSIP_VIDEO_FORMAT_MPEG4_QVGA    0x00200000 //add 20090819
#define VSIP_VIDEO_FORMAT_H264_QVGA     0x00400000 //add 20090819
#define VSIP_VIDEO_FORMAT_H264_720P     0x00800000 //add by shenxin
#define VSIP_VIDEO_FORMAT_H264_1080P    0x01000000 //add by shenxin

//新版本前端视频编码规则addbeg G .J.20090901
#define VSIP_VIDEO_TYPE_BIT_NUM         4    //编码类型占4位
#define VSIP_VIDEO_RESOLUTION_BIT_NUM   28   //每一种分辩率占1位 最多28个
#define VSIP_VIDEO_FORMAT_COUNT         4    //最多支持的编码格式数

#define VSIP_VIDEO_TYPE_MPEG4           0x1
#define VSIP_VIDEO_TYPE_H264            0x2
#define VSIP_VIDEO_TYPE_MJPEG           0x3
#define VSIP_VIDEO_TYPE_H265            0x4
#define VSIP_VIDEO_TYPE_SVAC            0x5

#define VSIP_VIDEO_RESOLUTION_NONE      0x00 
#define VSIP_VIDEO_RESOLUTION_QQCIF     0x01
#define VSIP_VIDEO_RESOLUTION_QCIF      0x02
#define VSIP_VIDEO_RESOLUTION_CIF       0x04	 
#define VSIP_VIDEO_RESOLUTION_2CIF      0x08
#define VSIP_VIDEO_RESOLUTION_4CIF      0x10
#define VSIP_VIDEO_RESOLUTION_VGA       0x20   	
#define VSIP_VIDEO_RESOLUTION_QVGA      0X40
#define VSIP_VIDEO_RESOLUTION_720P      0x80
#define VSIP_VIDEO_RESOLUTION_1080P     0x100
#define VSIP_VIDEO_RESOLUTION_QXGA		0x400
#define VSIP_VIDEO_RESOLUTION_UXGA      0x800
#define VSIP_VIDEO_RESOLUTION_QuadVGA   0x00001000
//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
#define VSIP_VIDEO_RESOLUTION_AUTO		0x00000200
//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化


// 画面合成风格
#define VSIPDEF_VIDCOMP_1STYLE                                      0
#define VSIPDEF_VIDCOMP_4STYLE                                      1		
#define VSIPDEF_VIDCOMP_9STYLE                                      2			
#define VSIPDEF_VIDCOMP_16STYLE                                     3
#define VSIPDEF_VIDCOMP_2STYLE                                      4
#define VSIPDEF_VIDCOMP_3STYLE                                      5
#define VSIPDEF_VIDCOMP_6STYLE                                      6
#define VSIPDEF_VIDCOMP_8STYLE                                      7
#define VSIPDEF_VIDCOMP_6STYLE_EQUAL                                8   //20170725 add by Royan Support6EqualStyle
#define VSIPDEF_VIDCOMP_INVALID_STYLE                               0xFF


//addend 20090901

// --------------------------------------------------------------------------
// ****************                Audio Input Types         ****************
// --------------------------------------------------------------------------
#define VSIP_AUDIO_INPUTTYPE_LINEIN                 0x00
#define VSIP_AUDIO_INPUTTYPE_MIC                    0x01

// --------------------------------------------------------------------------
// ****************                Audio Data Formats             ****************
// --------------------------------------------------------------------------
#define VSIP_AUDIO_FORMAT_NONE                      0x00
#define VSIP_AUDIO_FORMAT_PCM                       0x01
#define VSIP_AUDIO_FORMAT_ULAW                      0x02
#define VSIP_AUDIO_FORMAT_GSM                       0x04
#define VSIP_AUDIO_FORMAT_G729						0x08
#define VSIP_AUDIO_FORMAT_ADPCM                     0x10
#define VSIP_AUDIO_FORMAT_G7221C                    0x20
#define VSIP_AUDIO_FORMAT_G722                      0x40
#define VSIP_AUDIO_FORMAT_AACLC                     0x80

//  Typedefs
typedef unsigned char eMsgType;
typedef unsigned char eValueType;



//***************************************************
//              相关结构定义
//***************************************************

#define DEFAULT_VSIP_PORT	5510
//nvr128
#define DATA_BUFSIZE	    16384//modify G.J. 20090903 3074 -> 8192 注:目前64路NVR设备测试峰值为7350Byte, 计算得出64路设备能力集上报时最大值接近15K
#define SMALL_DATA_BUFSIZE  7168    //7K(7*1024)
#ifdef WIN32
#define MAX_SOCKET_HANDLE	1024
#define MAX_CLINET_NUM		1024
#else
#define MAX_SOCKET_HANDLE	256
#define MAX_CLINET_NUM		256
#endif
/*
#ifdef MTENABLE
#undef MAX_SOCKET_HANDLE
#undef  MAX_CLINET_NUM
#define MAX_SOCKET_HANDLE	5
#define MAX_CLINET_NUM		5
#endif*/

#define MAX_TIMERID_NUM     32		//zlq add nat探测包数量超过了16个,现改为32个.
#define MAX_NAT_DET_NUM     32		//zlq add nat探测包数量超过了16个,现改为32个.
#define NAT_PORT_CALC		1000   //码流源端口与目的端口偏移量

#define IP_OF_MP			0x01
#define IP_OF_MCU			0x02

#ifdef WIN32
#pragma pack( push, 1 )
#define PACKED 
#endif //WIN32
#ifndef PACKED
#ifdef _VXWORKS_
#define PACKED __attribute__((__packed__))
#endif //_VXWORKS_
#ifdef _LINUX_
#ifndef _LINUXEQUATOR_
#define PACKED __attribute__((packed))
#endif //_LINUXEQUATOR_
#endif //_LINUX_
#ifdef _LINUXEQUATOR_
#define PACKED
#endif //_LINUXEQUATOR_
#endif //#ifndef PACKED

//GUID结构
typedef struct TVsipGuid
{
	unsigned int	Data1;//对于设备GUID,Data1必须为0
   	unsigned short	Data2;
	unsigned short	Data3;
   	unsigned char	Data4[8];

    BOOL Decode(char* pBuf, u16 wBufLen)
    {
        if (!UnpackedFromBuf(pBuf, wBufLen))
            return FALSE;
        
        SwapFromNetwork();
        return TRUE;
	}
	
	void SwapToNetwork()
	{
		Data1 = htonl(Data1);
		Data2 = htons(Data2);
		Data3 = htons(Data3);
	}
	
	void SwapFromNetwork()
	{
		Data1 = ntohl(Data1);
		Data2 = ntohs(Data2);
		Data3 = ntohs(Data3);
	}
	
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

#ifdef _VXWORKS_
	    *Buf = (Data1 >>24) & 0xff ;
		*(Buf+1) = (Data1 >>16) & 0xff ;
		*(Buf+2) = (Data1 >>8) & 0xff ;
		*(Buf+3) = Data1 & 0xff;
		
		*(Buf+4) = (Data2 >>8) & 0xff ;
		*(Buf+5) = Data2 & 0xff;
		*(Buf+6) = (Data3 >>8) & 0xff ;
		*(Buf+7) = Data3 & 0xff;

		*(Buf+8) = Data4[0];
		*(Buf+9) = Data4[1];
		*(Buf+10) = Data4[2];
		*(Buf+11) = Data4[3];
		*(Buf+12) = Data4[4];
		*(Buf+13) = Data4[5];
		*(Buf+14) = Data4[6];
		*(Buf+15) = Data4[7];

#else
		memcpy( Buf, &Data1, sizeof(Data1) );
		Buf += sizeof(Data1);
		memcpy( Buf, &Data2, sizeof(Data2) );
		Buf += sizeof(Data2);
		memcpy( Buf, &Data3, sizeof(Data2) );
		Buf += sizeof(Data3);
		memcpy( Buf, Data4, sizeof(Data4) );
		
#endif
		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
#ifdef _VXWORKS_
       Data1 = *((u32*)Buf);
	   Buf += sizeof(Data1);
	   Data2 =*((u16*)Buf);
       Buf += sizeof(Data2);		
       Data3 = *((u16*)Buf);
	   Buf += sizeof(Data3);

		Data4[0]=*(Buf) ;
		Data4[1]=*(Buf+1) ;
		Data4[2]=*(Buf+2) ;
		Data4[3]=*(Buf+3) ;
		Data4[4]=*(Buf+4) ;
		Data4[5]=*(Buf+5) ;
		Data4[6]=*(Buf+6) ;
		Data4[7]=*(Buf+7) ;
#else
	    memcpy( &Data1, Buf, sizeof(Data1) );
		Buf += sizeof(Data1);
		memcpy( &Data2, Buf, sizeof(Data2) );
		Buf += sizeof(Data2);
		memcpy( &Data3, Buf, sizeof(Data3) );
		Buf += sizeof(Data3);
		memcpy( Data4, Buf, sizeof(Data4) );
#endif

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(Data1)+sizeof(Data2)+sizeof(Data3)+sizeof(Data4) );
	}

	BOOL operator == ( const TVsipGuid & Guid)
	{
		return ( memcmp( this, &Guid, sizeof(TVsipGuid)) == 0 );
	}

	BOOL IsIdenticalUnitGuid( const TVsipGuid & Guid)
	{
		return ( memcmp( &this->Data2, &Guid.Data2, sizeof(TVsipGuid)-sizeof(Data1)) == 0 );
	}
}PACKED VSIP_GUID, *PVSIP_GUID;

// 协议句柄
typedef struct tagVsipHandle
{
	tagVsipHandle()
	{
		memset( &DevGuid, 0, sizeof(DevGuid));
		ConnectionType = VSIP_CONNECTION_NULL;
		ContentType = VSIP_CONTENT_TYPE_NONE;
		SrcMsgType = 0;
		SocketId = INVALID_SOCKET;
		IpAddr = 0;
		Port = 0;
		TransactionNum = 0;
		bIsSend2All = FALSE;
		pContext = NULL;
		nSendTimeouts = 0;
	}
	TVsipGuid		DevGuid;	//对端设备GUID
	unsigned char	ConnectionType;
	unsigned char	ContentType;
	unsigned char	SrcMsgType;
	SOCKHANDLE		SocketId;	
	u32         	IpAddr;		//设备IP地址
	unsigned short  Port;		//设备VSIP通信端口
	unsigned short	TransactionNum;
	BOOL			bIsSend2All;
	void *			pContext;
    u8              abyPuid[64];
	s32             nSendTimeouts;
#ifdef VSIP_PERFORM_IMPROVE
	u32 ListId;		//zhuzhm: 保存对应链路表ID，避免循环搜索
					// 只在创建链路时设置
	u32 LocalIp;		//本地IP地址
	u16 LocalPort;	//本地VSIP通信端口
					// zhuzhm:用于链路检测，在GetUnitCtrl时设置
#endif			
}PACKED VSIP_HANDLE, *PVSIP_HANDLE;

enum eConnState
{ 
	CONN_STATE_NULL, CONN_STATE_CONNECTED, CONN_STATE_READY
};
enum eUnitState
{ 
	STATE_NULL, STATE_DISCOVERED, STATE_CONFIGURED, STATE_INITED
};

// 消息头
typedef struct _VSIP_HEADER
{   
	unsigned short ProtocolVersion;		// 版本号   
	char MsgType;						// 消息类型(如VSIP_MSG_TYPE_D_PING_REQ)
	unsigned short TransactionNumber;	// 传输序列号
	unsigned short Length;				// 消息总大小

	
	void SwapToNetwork()
	{
		ProtocolVersion = htons(ProtocolVersion);
		TransactionNumber = htons(TransactionNumber);
		Length = htons(Length);
	}
	
	void SwapFromNetwork()
	{
		TransactionNumber = ntohs(TransactionNumber);
		ProtocolVersion = ntohs(ProtocolVersion);
		Length = ntohs(Length);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
#ifdef _VXWORKS_
		*Buf = (ProtocolVersion >>8) & 0xff;
		*(Buf+1) = ProtocolVersion & 0xff;
		*(Buf+2) = MsgType ;
		*(Buf+3) = (TransactionNumber >>8) & 0xff;
		*(Buf+4) = TransactionNumber & 0xff;
		*(Buf+5) = (Length >>8) & 0xff ;
		*(Buf+6) = Length & 0xff;
#else
	    memcpy( Buf, &ProtocolVersion, sizeof(ProtocolVersion) );
		Buf += sizeof(ProtocolVersion);
		memcpy( Buf, &MsgType, sizeof(MsgType) );
		Buf += sizeof(MsgType);
		memcpy( Buf, &TransactionNumber, sizeof(TransactionNumber) );
		Buf += sizeof(TransactionNumber);
		memcpy( Buf, &Length, sizeof(Length) );
#endif

		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
#ifdef _VXWORKS_		
	   ProtocolVersion = *((u16*)Buf);
	   Buf += sizeof(ProtocolVersion);
	   MsgType =*((u8*)Buf);
       Buf += sizeof(MsgType);		
       TransactionNumber = *((u16*)Buf);
	   Buf += sizeof(TransactionNumber);
	   Length = *((u16*)Buf);

#else
        memcpy( &ProtocolVersion, Buf, sizeof(ProtocolVersion) );
		Buf += sizeof(ProtocolVersion);
		memcpy( &MsgType, Buf, sizeof(MsgType) );
		Buf += sizeof(MsgType);
		memcpy( &TransactionNumber, Buf, sizeof(TransactionNumber) );
		Buf += sizeof(TransactionNumber);
		memcpy( &Length, Buf, sizeof(Length) );
	   
#endif
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(ProtocolVersion)+sizeof(MsgType)+sizeof(TransactionNumber)+sizeof(Length) );
	}
}PACKED VSIP_HEADER, *PVSIP_HEADER;



// 发现请求信息体
typedef struct _VSIP_DISCOVERY_PING_INFO
{   
	u32            IpAddress;		// VSIP应用IP
	unsigned short Port;				// VSIP应用端口号
	unsigned char  ConnectionType;	// 连接类型(如VSIP_CONNECTION_UNICAST)
	
	
	void SwapToNetwork()
	{
		Port = htons(Port);
	}
	
	void SwapFromNetwork()
	{
		Port = ntohs(Port);
	}
	
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
#ifdef _VXWORKS_
		*Buf = (IpAddress >>24) & 0xff ;
		*(Buf+1) = (IpAddress >>16) & 0xff ;
		*(Buf+2) = (IpAddress >>8) & 0xff ;
		*(Buf+3) = IpAddress & 0xff;
		
		*(Buf+4) = (Port >>8) & 0xff ;
		*(Buf+5) = Port & 0xff;
		*(Buf+6) = ConnectionType;
		
#else

	    memcpy( Buf, &IpAddress, sizeof(IpAddress) );
		Buf += sizeof(IpAddress);
		memcpy( Buf, &Port, sizeof(Port) );
		Buf += sizeof(Port);
		memcpy( Buf, &ConnectionType, sizeof(ConnectionType) );

#endif
	

		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

#ifdef _VXWORKS_
       IpAddress = *((u32*)Buf);
	   Buf += sizeof(IpAddress);
	   Port =*((u16*)Buf);
       Buf += sizeof(Port);		
       ConnectionType = *((u8*)Buf);
#else
	   	memcpy( &IpAddress, Buf, sizeof(IpAddress) );
		Buf += sizeof(IpAddress);
		memcpy( &Port, Buf, sizeof(Port) );
		Buf += sizeof(Port);
		memcpy( &ConnectionType, Buf, sizeof(ConnectionType) );
#endif

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(IpAddress)+sizeof(Port)+sizeof(ConnectionType) );
	}

}PACKED VSIP_DISCOVERY_PING_INFO, *PVSIP_DISCOVERY_PING_INFO;

// 能力集信息结构
typedef struct _VSIP_IC_CAPABILITIES
{
	unsigned char     EntityType;		// 能力集类型(如VSIP_ENTITY_VIDEODECODER)
	VSIP_GUID         guid;				// 能力集唯一ID
	unsigned short    Fabricant;		// 能力集开发商标识(如VSIP_FABRICANT_ID_SMARTSIGHT)
	unsigned short    Version;			// 能力集版本号
	
	
	void SwapToNetwork()
	{
		guid.SwapToNetwork();
		Fabricant = htons(Fabricant);
		Version = htons(Version);
	}
	
	void SwapFromNetwork()
	{
		guid.SwapFromNetwork();
		Fabricant = ntohs(Fabricant);
		Version = ntohs(Version);
	}
	
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &EntityType, sizeof(EntityType) );
		Buf += sizeof(EntityType);
		guid.PackedToBuf( Buf, BufLen);
		Buf += guid.PackedLen();
		memcpy( Buf, &Fabricant, sizeof(Fabricant) );
		Buf += sizeof(Fabricant);
		memcpy( Buf, &Version, sizeof(Version) );

		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &EntityType, Buf, sizeof(EntityType) );
		Buf += sizeof(EntityType);
		guid.UnpackedFromBuf( Buf, BufLen);
		Buf += guid.PackedLen();
		memcpy( &Fabricant, Buf, sizeof(Fabricant) );
		Buf += sizeof(Fabricant);
		memcpy( &Version, Buf, sizeof(Version) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(EntityType)+guid.PackedLen()+sizeof(Fabricant)+sizeof(Version) );
	}
}PACKED VSIP_IC_CAPABILITIES, *PVSIP_IC_CAPABILITIES;

// 字符串配置项结构
typedef struct _CONFIG_DATA_STRING
{
	unsigned short    ValueSize;	// 字符串长度
	unsigned char     *pszValue;	// 字符串内容
	
}PACKED CONFIG_DATA_STRING, *PCONFIG_DATA_STRING;   

// 配置项类型
union CONFIG_DATA
{
	unsigned char      cValue;			// char,1byte
	unsigned short     sValue;			// short,2bytes
	u32                uiValue;			// uint,4bytes
	int                iValue;			// int,4bytes
	CONFIG_DATA_STRING szValue;			// string
};

// 配置信息结构
typedef struct _VSIP_MEDIA_CONTROL_CONFIG
{
	unsigned short    Id;			// 配置项ID
	eValueType        ValueType;	// 配置项类型
	void *            ConfigData;   // 配置项值,指向CONFIG_DATA的指针
	
}PACKED VSIP_MEDIA_CONTROL_CONFIG, *PVSIP_MEDIA_CONTROL_CONFIG;

// 
typedef struct _VSIP_BACKEND_CAP
{
	u16 wEntityId;
	u32 dwEntityValue;
}PACKED VSIP_BACKEND_CAP, *PVSIP_BACKEND_CAP;

// 设置属性配置信息结构
typedef struct _VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG
{
	unsigned short	Id;				// 配置项ID
	eValueType		ValueType;		// 配置项类型
	void *			ConfigData;		// 配置项值,指向CONFIG_DATA的指针
	unsigned short	SectionId;		// 配置项字段ID
	unsigned short	NameSize;		// 配置项名称长度
	unsigned char*	pszName;		// 配置项名称内容
	unsigned short	HelpStringSize;	// 配置项帮助信息长度
	unsigned char*	pszHelpString;	// 配置项帮助信息内容
	u32				Reserved;		// 保留项
	
}PACKED VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG, *PVSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG;

// 获取属性配置信息结构
typedef struct _VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG
{
	unsigned short Id;				// 配置项ID
	unsigned short SectionId;		// 配置项字段ID
}PACKED VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG, *PVSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG;

// 特别参数信息结构
typedef struct _VSIP_MEDIA_CONTROL_ADDITIONAL_ARG
{
	eValueType        ValueType;	// 配置项类型
	void *            ConfigData;   // 配置项值,指向CONFIG_DATA的指针

}PACKED VSIP_MEDIA_CONTROL_ADDITIONAL_ARG, *PVSIP_MEDIA_CONTROL_ADDITIONAL_ARG;

//NAT探测包消息定义
typedef struct _VSIP_NAT_DET_PACK_INFO
{
protected:
	u32 m_dwSrcIP;			//探测包源地址
	u16 m_wSrcPort;			//探测包源端口
//	
//	TVsipGuid  m_tGuid;     //设备Guid
	char  m_abyPuid[VSIP_PUID_LENGTH-VSIP_DOMAIN_LENGTH+1]; //puid
    char m_byReserved;		//保留
	u32 m_dwSessID;
public:
	u32 GetSrcIP(void) const  
	{ 
		return(m_dwSrcIP ); 
	}
	u16 GetSrcPort(void) const 
	{ 
		return (ntohs( m_wSrcPort ) ); 
	}
	void SetSrcIP(u32 dwSrcIP) 
	{
		m_dwSrcIP= dwSrcIP;
	}
	void SetSrcPort(u16 wSrcPort) 
	{
		m_wSrcPort= (htons(wSrcPort));
	}
	u32 GetmSessID()
	{
		return (ntohl(m_dwSessID));
	}
	void SetSessID(u32 dwSessId)
	{
		m_dwSessID =(htonl(dwSessId));
	}
	void SetPuid(char abyPuid[])
	{
		strncpy(m_abyPuid,abyPuid+VSIP_DOMAIN_LENGTH,sizeof(m_abyPuid)-1);
        m_abyPuid[sizeof(m_abyPuid)-1]='\0';
	}
	void GetPuid(char abyPuid[])
	{
		strncpy(abyPuid,m_abyPuid,sizeof(abyPuid)-1);
        abyPuid[sizeof(abyPuid)-1]='\0';
	}
	
}PACKED VSIP_NAT_DET_PACK_INFO, *PVSIPTMTI_NAT_DET_PACK_INFO;

//发送NAT探测包参数
typedef struct _VSIP_SEND_NAT_INFO 
{
	u32            dwDstIpAddr;				//发送探测包目的地址
	unsigned short wSrcPort;			    //发送RTP探测包源端口
	unsigned short wDstPort;				//发送探测包目的端口
	VSIP_NAT_DET_PACK_INFO tNATDetPack;		//NAT探测包参数
}PACKED VSIP_SEND_NAT_INFO, *PVSIP_SEND_NAT_INFO;

//发送NAT探测包定时器ID
typedef struct _VSIP_SEND_NAT_TIMER_ID 
{
	BOOL			bIsValid;			//该项是否已存储了正进行的定时器ID
	u32         	dwMpIpAddr;         //Mp IP
	u32             dwMcuIpAddr;		//MCU IP
	u32             dwSrcIpAddr;		//发送探测包源IP，即终端IP
	unsigned short	wCodeDstPort;       //码流目的端口
	unsigned short  wNATDetNum;			//发送探测包的路数
	VSIP_SEND_NAT_INFO tSendNatInfo[MAX_NAT_DET_NUM];	//发送NAT探测包参数
	u32             dwBufLen;			//内容长度，即探测包长度
	u32         	dwTimerID;			//定时器ID
}PACKED VSIP_SEND_NAT_TIMER_ID, *PVSIP_SEND_NAT_TIMER_ID;

#ifdef WIN32
#pragma pack( pop )
#endif //WIN32


#endif //VSIP_DEFINES_H

