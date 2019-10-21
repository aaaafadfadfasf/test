/*****************************************************************************
  模块名      : 终端协议接口模块
  文件名      : MtCtrl.h
  相关文件    :
  文件实现功能:
  作者        : 卿晋
  版本        : V3.0  Copyright(C) 1997-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2005/6/14    3.0         卿晋      创建
******************************************************************************/
#ifndef MT_CTRL_H
#define MT_CTRL_H

#include "cbb/osp/osp.h"
#include "kdvtype.h"
#include "cms/pu/vsipdefine.h"
#include "cms/natagent/natagent.h"
//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
#include <map>
//#include <set>
//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化
#ifdef linux
#include <pthread.h>
#endif // _LINUX

using std::map;

//消息类型定义
#define OPERATION_RESULT_RESP				0x01 //操作结果应答[msgbody:TMTIOPResultResp]
#define EVENT_INPUTPIN_STATE_CHANGED		0x02 //并口告警输入状态通知[msgbody:TMTIEntityState]
#define EVENT_EN_MOTD_STATE_CHANGED			0x03 //编码移动侦测告警状态通知[msgbody:TMTIMotDetState]
#define EVENT_CONFIG_CHANGED				0x04 //参数更新通知[msgbody:TMTIConfigInfo]
#define EVENT_CONNECTION_LOST				0x05 //断链通知[mtc或mcu侧 msgbody:NULL]
#define EVENT_SERIAL_DATA					0x06 //终端串口数据[msgbody:TMTIDataSend]
#define EVENT_OUTPUTPIN_STATE_CHANGED       0x07 //并口告警输出状态通知[msgbody:TMTIEntityState]
#define EVENT_SYS_STATE_NOTIFY		        0x08 //系统状态通知[msgbody:u32]
#define EVENT_DEC_SERIAL_DATA				0x09 //解码器串口数据[msgbody:TMTISerialDataSend]
#define EVENT_VIDEO_IN_STATE_CHANGED	    0x0A //视频源丢失告警
#define EVENT_CHANNEL_STATE_NOTIFY			0x0B //通道状态通知
#define EVENT_VIDEO_IN_NOTIFY   			0x0C //视频参数通知
#define EVENT_REC_STATE_NOTIFY		        0x10 //录像异常通知[msgbody:u32 非零为错误码,零为恢复正常]
#define EVENT_DE_MOTD_STATE_CHANGED			0x13 //解码移动侦测告警状态通知[msgbody:TMTIMotDetState]
#define EVENT_VIENC_TYPE_CHANGED			0x14 //视频编码格式关于H264改变[msgbody:TMTIConfigInfo]
#define EVENT_PINENABLE_CHANGED				0x20	//body:TMTIInputPinState
//NAT穿透相关
#define EVENT_START_NAT_DETECT              0x40    //20100317 
#define EVENT_STOP_NAT_DETECT               0x41    
#define CMD_REBOOT_DEVICE					0x50 //重启设备
#define MSG_SET_SYS_TIME_REQ				0x51 //设置系统时间
#define MSG_SET_CONFIG_REQ					0x52 //设置参数请求
#define MSG_START_VIDEO_SEND_REQ			0x53 //开始视频发送
#define MSG_START_VIDEO_RECV_REQ			0x54 //开始视频接收
#define MSG_START_AUDIO_SEND_REQ			0x55 //开始音频发送
#define MSG_START_AUDIO_RECV_REQ			0x56 //开始音频接收
#define MSG_ADD_VIDEO_SEND_DEST_REQ			0x57 //增加视频发送目标
#define MSG_REMOVE_VIDEO_SEND_DEST_REQ		0x58 //删除视频发送目标
#define MSG_STOP_VIDEO_SEND_REQ				0x59 //停止视频发送
#define MSG_STOP_VIDEO_RECV_REQ				0x5A //停止视频接收
#define MSG_STOP_AUDIO_SEND_REQ				0x5B //停止音频发送
#define MSG_STOP_AUDIO_RECV_REQ				0x5C //停止音频接收
#define CMD_SEND_SERIAL_PORT_DATA			0x5D //发送串口数据
#define CMD_SEND_CAMERA_CTRL_DATA			0x5E //发送摄像头控制数据
#define CMD_SET_VIDEO_INPUT_PORT			0x5F //设置视频输入端口
#define CMD_SET_OUTPUT_PIN_STATE			0x60 //设置告警输出
#define CMD_START_ALARM_TIMER				0x61 //开始告警计时
#define CMD_STOP_ALARM_TIMER				0x62 //停止告警计时
#define CMD_HIDE_ALARM_TIMER				0x63 //隐藏告警计时
#define CMD_LOAD_DEFAULT_SETTINGS			0x64 //恢复默认参数
#define CMD_SAVE_SETTINGS					0x65 //保存参数
#define MSG_USER_CUSTOM						0x66 //用户自定义命令
#define MSG_ADD_AUDIO_SEND_DEST_REQ         0x67 //增加音频发送目标
#define MSG_REMOVE_AUDIO_SEND_DEST_REQ      0x68 //删除音频发送目标
//用户自定义消息内容类型宏定义
#define MSG_CONNECT_SUCCESS					0x69    //MCU与终端建链成功
#define CMD_SEND_KEY_FRAME					0x6A //发送关键帧
#define MSG_REAL_CONN_IP					0x6B //获取终端连接实际所用IP
#define CMD_START_SEND_COM_DATA				0x6C //向用户端发送摄像头返回串口数据
#define MSG_LOAD_FILE_OVER                  0x6D    //上传文件完成
#define MSG_SET_DEV_GUID                    0x6E //设置设备guid消息
//#define CMD_SET_VIDEO_DYN_VALUE				0x6F //设置解码器动态载荷值
#define EVENT_BITCURRENT_STATE_CHANGED      0x6F    //设置码流有无状态通知
//消息体为u8,内容为通道号
#define EVENT_START_REC_REQ                 0x70    //StartRec[msgbody:channelid(u8)]
////消息体为u8,内容为通道号
#define EVENT_STOP_REC_REQ                  0x71    //StopRec[msgbody:channelid(u8)]
//消息体为(u8)+(TimelyRecInfo),内容为通道号+定时参数
#define EVENT_SET_TIMELY_INFO_REQ           0x72    //SetTimelyRecInfo[msgbody:channelid(u8)+TimelyRecInfo]
//消息体为(u8)+(PeriodlyTimeInfo),内容为通道号+周期参数
#define EVENT_SET_PERIODLY_INFO_REQ         0x73    //SetPeriodlyRecInfo[msgbody:channelid(u8)+PeriodlyTimeInfo]
//消息体为(u8)+(RecChannelBasicInfo),内容为通道号+通道参数
#define EVENT_SET_CHANNEL_BASIC_INFO_REQ    0x74    // SetChannelRecBasicInfo[msgbody:channelid(u8)+RecChannelBasicInfo]
//消息体为(RecSysInfo),内容为录象系统
#define EVENT_SET_SYS_INFO_REQ              0x75    //SetRecSysInfo[msgbody:RecSysInfo]
//消息体为(u8+ RecStartEndTimeInfo+u8 +u16) 内容为通道号+开始结束时间+查询类型+查询数目
#define EVENT_QUIEY_REC_FILE_REQ            0x76    //QueryRecFile[MsgBody:TQueryParam]
//消息体为u32 ,内容是文件id
#define EVENT_START_EXPORT_REC_FILE_REQ     0x77    //MsgBody[u32 dwRecFileIndexId+u32 dwIpAddress+u16 wPort]
//消息体为u8+u8，内容为放像器id和操作类型
#define EVENT_REC_FILE_OPERATION_REQ        0x78    //RecFileOperation[byPlayerId(u8)+byOperationType(u8)]
//消息体为u32+u8，内容为文件或事件id，和类型
#define EVENT_DEL_REC_FILE_REQ              0x79    //DelRecFile[dwRecFileIndexId(u32+byType(u8)]
//消息体为u8+u32，内容为播放器id，拖拉时间
#define EVENT_SEEK_LOCATION_REQ             0x7A    //RecSeekLocation
//消息体为u32+u32+u16+u8, 内容为文件id号，ip地址，端口号,类型
#define EVENT_REC_FILE_START_PLAY_REQ       0x7B    //RecFileStartPlay
//消息体为u8，内容为类型
#define EVENT_DEL_REC_RECORD_REQ            0x7C    //DelRecRecord
//MCU或MTC
/*消息体QueryRecFileInfo，内容也为查询结果*/
#define EVENT_QUIRY_REC_FILE_ACK            0x7D      //查询录象文件应答[MsgBody:QueryRecFileInfo]
//消息体ProgressParaInfo，消息内容进度
#define EVENT_REC_PROGRESS_NOTIFY           0x7E      //进度通知[MsgBody:ProgressParaInfo]
/*消息体TMTIConfigInfo，其字段byConfigType：
PARAM_TYPE_TIMELY_INFO 对应定时录象信息
PARAM_TYPE_PERIODLY_INFO 对应周期录象信息
PARAM_TYPE_CHANNEL_BASIC_INFO 对应通道基本信息
PARAM_TYPE_REC_SYS_INFO    对应系统信息  */
#define EVENT_REC_INFO_CHANGED				   0x7F     //录象信息更新通知[MsgBody:TMTIConfigInfo]
#define EVENT_REC_STATUS_CHANGED               0x80     //录象器状态变化通知[MsgBody:u8+RecPlayerState]通道号+状态
#define EVENT_REC_FILE_START_PLAYERID_ACK      0X81     //放像器id通知[MsgBody:TPlayerRsp]
#define EVENT_REC_FILE_EXPORT_PROGRESS_NOTIFY  0X82     //导出文件信息通知[MsgBody:TRecFileExportInfo]
#define EVENT_STOP_EXPORT_REC_FILE_REQ         0x83     //停止导出文件通知[MsgBody:NULL]
#define EVENT_TIMER_DEAD                       0x84     //定时器僵死通知[MsgBody:NULL]
#define EVENT_SET_NMS_PARAM_REQ                0x85     //设置网管参数请求[msgbody:TNmsParam]
#define EVENT_VIDEO_SOURCE_STATE_NOTIFY        0x86     //终端视频源状态通知[MsgBody:channelId(u8)+state(u8)]
//mt
#define EVENT_QUIEY_REC_FILE_NUM_REQ           0x87     //查询满足条件的个数[MsgBody:TQueryParam]
#define EVENT_QUIEY_REC_FILE_NUM_ACK           0x88     //返回满足条件的个数(msgbody:u16)
#define EVENT_NEW_CONNECTION_COME              0x89     //新连接建立[msgbody:ip(32)+port(16)]
#define EVENT_CONNECTION_PING_ACK              0X90     //链路检测ping应答[msgbody:ip(32)+port(16)]
#define EVENT_MT_REGEDIT_REQ                   0x91     //终端注册请求(TDevRegeditInfo)
#define EVENT_MT_REGEDIT_RSP                   0x92     //终端注册应答(u16)
//mtc或mcu
#define EVENT_FILE_TRANS_ACK                   0x93     //TFileTransRspInfo
#define EVENT_START_FILE_TRANS_REQ             0x94     //TFileTransInfo
#define EVENT_STOP_FILE_TRANS_REQ              0x95     //NULL
#define EVENT_FILE_TRANS_DATA                  0x96     //TFileTransInfo
#define EVENT_TELECOM_ALARM_NOTIFY             0X97     // TMTIAlarmInfo
#define EVENT_TRANSPARENT_MESSAGE              0X98     // TMTITransparentInfo
//图片查询相关信令
#define EVENT_QUERY_PIC_FILE_NUM_REQ		   0x99	    //body:TMTIQueryPicFileParam
#define EVENT_QUERY_PIC_FILE_REQ			   0x9A	    //body:TMTIQueryPicFileParam
#define EVENT_DOWNLOAD_PIC_FILE_REQ			   0x9B	    //body:u16 windex+TMTIPicFileInfo
#define EVENT_DEL_PIC_FILE_REQ				   0x9C	    //body:TMTIQueryPicFileInfo
#define EVENT_DEL_PIC_FILEEX_REQ			   0x9D	    //body:TMTIPicFileInfo
#define EVENT_START_SNAPSHOT_REQ			   0x9E	    //body:u8
#define EVENT_STOP_SNAPSHOT_REQ				   0x9F	    //body:u8
#ifdef VSIP_MULTI_CHAN_SUPPORT
#define EVENT_SET_DEV_MULTIENCCHAN_PARAM_REQ	0xA0	//body:TMTISetConfig
#endif
#define MSG_START_VIDEO_RECV_EXREQ			   0xA0     //开始视频接收(扩展请求)
#define MSG_START_AUDIO_RECV_EXREQ	           0XA1     //开始音频接收(扩展请求)
//GPS相关
#define EVENT_SET_TRANSDATA_ACK                0xA3
#define EVENT_GET_TRANSDATA_ACK				   0xA4
#define EVENT_GET_TRANSCFG_ACK				   0xAE
#define EVENT_CREATE_SUBSCRIBE_ACK             0xAF
#define EVENT_QUERY_PIC_FILE_NACK              0xB0    //body:u16 add G.J. 20091222
#define EVENT_GET_SUBSCRIBE_ACK                0xB1
#define EVENT_TRANSPARENT_CHANNEL_TYPE_EX      0xB2
//hzc_Transparent_Chan_20110324{{
#define EVENT_TRANSPARENT_MESSAGE_RESP		   0xB3
//}}
#define EVENT_CU_OPERTION_RESP				   0xB4
#define EVENT_CU_OPERTION_NOTIFY			   0xB5
#define EVENT_PLAY_GPS_RESP					   0xB6
#define EVENT_PLAY_GPS_NOTIFY				   0xB7
#define EVENT_GET_DECODER_STYLE_RESP		   0xB8
#define EVENT_GET_CONFIG_RESP       		   0xB9    //add by Royan 20130131
/*消息体TMTIRecListQryRsp，内容为查询结果*/
#define EVENT_QUIRY_REC_LIST_RSP               0xBA      //查询录象日历应答[MsgBody:TMTIRecListQryRsp]
#define EVENT_GET_CUSTOM_EXT_CAPS_RESP  	   0xBB    //自定义扩展能力集
#define EVENT_STREAM_PROP_RSP                  0xBC    
#define EVENT_TRANSPARENT_QUERY_NOTIFY		   0xBE
//shenxin_rec_file_down_notify{{
#define EVENT_REC_FILE_DOWN_NOTIFY			   0xBF
//}}

#define EVENT_SET_OSD_VIDSRC_ACK			   0xC0
#define EVENT_GET_OSD_VIDSRC_ACK			   0xC1
#define EVENT_SET_OSD_CHN_ACK				   0xC2
#define EVENT_GET_OSD_CHN_ACK				   0xC3
#define EVENT_TRANSPARENT_SUBSCRIBE_RESP       0xD1
#define EVENT_TRANSPARENT_SUBSCRIBE_NTF        0xD2
#define EVENT_GET_MATRIX_ACK				   0xE0	//body:TMTIMatrixInfo
#define EVENT_SET_SNAPSHOTNAME_ACK			   0xF1	//body:u16 werror
#define EVENT_CHANGEPASSWORD_ACK			   0xF2	//zlq add 修改密码应答
#define EVENT_SETBMPOSDINFO_ACK				   0xF3	//zlq add
#define EVENT_TRANSPARENTCHAN_CFG_MSG		   0xF4	//zlq add
#define EVENT_TRANSPARENTCHAN_CTRL_MSG		   0xF5	//zlq add
#define EVENT_DIRECTTRANSCHAN_CHANGED		   0xF6	//zlq add	终端直传路数改变通知
#define EVENT_CAPBILITY_CHANGED				   0xF7	//zlq add 终端能力集改变通知
#define EVENT_QUERY_PIC_FILE_NUM_ACK		   0xF8	//body:u16 wnum
#define EVENT_QUERY_PIC_FILE_ACK			   0xF9	//body:TMTIQueryPicFileInfo
#define EVENT_DOWNLOAD_PIC_FILE_ACK			   0xFA	//body:TMTIDownloadPicFileInfo
#define EVENT_DEL_PIC_FILE_ACK				   0xFB	//body:u16 TMTISnapShotResult
#define EVENT_SET_SNAPSHOT_CONFIG_ACK		   0xFC	//body:u16 werror
#define EVENT_SET_MATRIX_CONFIG_ACK			   0xFD	//body:u16 werror
#define EVENT_START_SNAPSHOT_ACK			   0xFE	//body:u16 werror
#define EVENT_STOP_SNAPSHOT_ACK				   0xFF	//body:u16 werror

#define MSG_FILE_TRANS_BEGIN                    0
#define MSG_FILE_TRANS_END                      0xffff

#define MTI_AUDIO_TYPE 0
#define MTI_VIDEO_TYPE 1
#define MTI_BOTH_MEDIA_TYPE  2
#define MSG_SET_BMPOSDINFO						0xF0    //设置bmposd信息 zlq add
#define MSG_UPDATE_USERPSW                      0xF1    //更新用户密码 zlq add
#define MSG_TRANSPARENTCHAN_CFG					0xF2    //透明通道配置消息 zlq add
#define MSG_TRANSPARENTCHAN_CTRL				0xF3    //透明通道控制消息 zlq add
// 设备类型名称
#define MTI_DEV_KDM2401			(const char*)"KDM2401"
#define MTI_DEV_KDM2401S		(const char*)"KDM2401S"
#define MTI_DEV_KDM2401L		(const char*)"KDM2401L"
#define MTI_DEV_KDM2402			(const char*)"KDM2402"
#define MTI_DEV_KDM2402S		(const char*)"KDM2402S"
#define MTI_DEV_KDM2400			(const char*)"KDM2400"
#define MTI_DEV_KDM2500			(const char*)"KDM2500"
#define MTI_DEV_KDM2400P		(const char*)"KDM2400P"
#define MTI_DEV_KDM2500P		(const char*)"KDM2500P"
#define MTI_DEV_KDM2418			(const char*)"KDM2418"
#define MTI_DEV_KDM2518			(const char*)"KDM2518"
#define MTI_DEV_KDM2417			(const char*)"KDM2417"
#define MTI_DEV_KDM2428			(const char*)"KDM2404LS"
#define MTI_DEV_KDV8010A		(const char*)"KDV8010A"
#define MTI_DEV_KDV8010C		(const char*)"KDV8010C"
#define MTI_DEV_DEC5			(const char*)"DEC5"
#define MTI_DEV_KDM2560			(const char*)"KDM2560"
#define MTI_DEV_KDM2461			(const char*)"KDM2461"
#define MTI_DEV_KDM2461L		(const char*)"KDM2461L"
#define MTI_DEV_KDM2401LS		(const char*)"KDM2401LS"
#define MTI_DEV_KDM2402LS		(const char*)"KDM2402LS"
#define MTI_DEV_KDM2402L		(const char*)"KDM2402L"
#define MTI_DEV_KDM2440			(const char*)"KDM2440"
#define MTI_DEV_KDM2440P		(const char*)"KDM2440P"
#define MTI_DEV_KDM201_C04		(const char*)"KDM201-C04"
#define MTI_DEV_KDM201_C04P		(const char*)"KDM201-C04P"
#define MTI_DEV_KDM201_C04L		(const char*)"KDM201-C04L"
#define MTI_DEV_KDM201_D04		(const char*)"KDM201-D04"
#define MTI_DEV_KDM2100			(const char*)"KDM2100"
#define MTI_DEV_KDM2401ES		(const char*)"KDM2401ES"
#define MTI_DEV_KDM2404S		(const char*)"KDM2404S"
#define MTI_DEV_KDM2501			(const char*)"KDM2501"
#define MTI_DEV_KDM2462			(const char*)"KDM2462"
#define MTI_DEV_KDM2462L		(const char*)"KDM2462L"
#define MTI_DEV_KDM2462S		(const char*)"KDM2462S"
#define MTI_DEV_KDM2462LS		(const char*)"KDM2462LS"
#define MTI_DEV_KDM2461LS		(const char*)"KDM2461LS"
#define MTI_DEV_KDM2461S		(const char*)"KDM2461S"
//daiyi Bug00003637 2008-07-04 add start
#define MTI_DEV_KDM2110			(const char*)"KDM2110"
#define MTI_DEV_KDM2110E		(const char*)"KDM2110E"
#define MTI_DEV_KDM2121			(const char*)"KDM2121"
//daiyi Bug00003637 2008-07-04 add end
//2009/09/03 G.J. add
#define MTI_DEV_NVR2820         (const char*)"NVR2820"
#define MTI_DEV_NVR2820E        (const char*)"NVR2820E"
#define MTI_DEV_NVR2860         (const char*)"NVR2860"
#define MTI_DEV_KDM201S         (const char*)"KDM201S"
//end add
//add by shenxin 2011-6-20 添加支持SVR2810[E]设备入网
#define MTI_DEV_SVR2810			(const char*)"SVR2810"
#define MTI_DEV_SVR2810E		(const char*)"SVR2810E"
//end add

//能力定义
#define MTI_CAPABILITY_ENABLE	1
#define MTI_CAPABILITY_DISABLE	0
#define MTI_CAPOSDCOLOR_16		1
#define MTI_CAPOSDCOLOR_256		2
#define MTI_CAPOSDCOLOR_FONT	4	//字体自适应
#define MTI_CAPPRIVTENETWORK	1
#define MTI_CAPOPERATOR			2

//图像抓拍命名规则
#define SNAPSHOT_NAME_INVALID		0
#define SNAPSHOT_NAME_DATETIME		1	// 日期时间
#define SNAPSHOT_NAME_DEVALIAS		2	// 设备别名
#define SNAPSHOT_NAME_VIDSRCNAME	3	// 视频源名
#define SNAPSHOT_NAME_ALARMTYPE		4	// 告警类型
#define SNAPSHOT_NAME_USERDEFINE	5	// 用户自定义

//特殊设备类型
#define MTI_DEV_VIRMT			(const char *)"KDMVMT"//虚拟终端
#define MTI_DEV_SMARTSIGHT		(const char *)"S1600e"//smartsight设备

//链接类型
#define MTI_CONNECTION_NONE					0x00
#define MTI_CONNECTION_UNICAST				0x01
#define MTI_CONNECTION_MULTICAST			0x02
#define MTI_CONNECTION_BROADCAST			0x03
#define MTI_CONNECTION_TCPSERVER			0x04
#define MTI_CONNECTION_TCPCLIENT			0x05
#define MTI_CONNECTION_TCP					0x06
#define MTI_CONNECTION_SSL					0x07
#define MTI_CONNECTION_UDP                  0x08

//视频数据格式
#define MTI_VIDEO_TYPE_NONE					0x00
#define MTI_VIDEO_TYPE_MPEG4				0x01
#define MTI_VIDEO_TYPE_H264					0x02
#define MTI_VIDEO_TYPE_MJPEG                0x03
#define MTI_VIDEO_TYPE_H265					0x04
#define MTI_VIDEO_TYPE_SVAC					0x05
#define MTI_VIDEO_TYPE_SN4					0x11
#define MTI_VIDEO_TYPE_H261					0x12
#define MTI_VIDEO_TYPE_H263					0x13

//音频数据格式
#define MTI_AUDIO_TYPE_NONE					0x00
#define MTI_AUDIO_TYPE_PCMA					0x01
#define MTI_AUDIO_TYPE_PCMU					0x02
#define MTI_AUDIO_TYPE_GSM					0x03
#define MTI_AUDIO_TYPE_G729					0x04
#define MTI_AUDIO_TYPE_ADPCM                0x05
#define MTI_AUDIO_TYPE_G7221C               0x06
#define MTI_AUDIO_TYPE_G722                 0x07 
#define MTI_AUDIO_TYPE_AACLC                0x08 

//图象分辨率
#define MTI_VIDEO_RESOLUTION_NONE			0x00
#define MTI_VIDEO_RESOLUTION_AUTO			0x01
#define MTI_VIDEO_RESOLUTION_QCIF			0x02
#define MTI_VIDEO_RESOLUTION_CIF			0x03
#define MTI_VIDEO_RESOLUTION_2CIF			0x04
#define MTI_VIDEO_RESOLUTION_4CIF			0x05
#ifdef VSIP_ADD_NEW_RESOLUTION
#define MTI_VIDEO_RESOLUTION_QQCIF			0x06
#define MTI_VIDEO_RESOLUTION_VGA			0x07
#define MTI_VIDEO_RESOLUTION_QVGA			0x08 
//addbeg G.J. 20090901
#define MTI_VIDEO_RESOLUTION_720P           0x09
#define MTI_VIDEO_RESOLUTION_1080P          0x0A
//addend20090901
//add by shenxin
#define MTI_VIDEO_RESOLUTION_QXGA          0x0B
//end add
#define MTI_VIDEO_RESOLUTION_UXGA		   0x0C
#define MTI_VIDEO_RESOLUTION_960P	       0x0D
#endif

#define MTI_VIDEO_RESLUTION_VAL_72         72
#define MTI_VIDEO_RESLUTION_VAL_88         88
#define MTI_VIDEO_RESLUTION_VAL_144        144
#define MTI_VIDEO_RESLUTION_VAL_176        176
#define MTI_VIDEO_RESLUTION_VAL_240        240
#define MTI_VIDEO_RESLUTION_VAL_288        288
#define MTI_VIDEO_RESLUTION_VAL_320        320
#define MTI_VIDEO_RESLUTION_VAL_352        352
#define MTI_VIDEO_RESLUTION_VAL_480        480
#define MTI_VIDEO_RESLUTION_VAL_576        576
#define MTI_VIDEO_RESLUTION_VAL_640        640
#define MTI_VIDEO_RESLUTION_VAL_704        704
#define MTI_VIDEO_RESLUTION_VAL_720        720
#define MTI_VIDEO_RESLUTION_VAL_960        960
#define MTI_VIDEO_RESLUTION_VAL_1080       1080
#define MTI_VIDEO_RESLUTION_VAL_1280       1280
#define MTI_VIDEO_RESLUTION_VAL_1536       1536
#define MTI_VIDEO_RESLUTION_VAL_1920       1920
#define MTI_VIDEO_RESLUTION_VAL_2048       2048
#define MTI_VIDEO_RESLUTION_VAL_2160       2160
#define MTI_VIDEO_RESLUTION_VAL_3480       3480
#define MTI_VIDEO_RESLUTION_VAL_4096       4096

//图像质量
#define MTI_VIDEO_QUALITY_PRIOR             0x01 //质量优先
#define MTI_VIDEO_SPEED_PRIOR               0x02 //速度优先

//参数类型定义
#define PARAM_TYPE_SYS						0x00 //系统参数
#define PARAM_TYPE_SYS_PUB					0x01 //系统公共参数
#define PARAM_TYPE_VIDEO_ENC_NET			0x02 //视频编码网络参数
#define PARAM_TYPE_VIDEO_ENC				0x03 //视频编码参数
#define PARAM_TYPE_VIDEO_ENC_PUB			0x04 //视频编码公共参数
#define PARAM_TYPE_VIDEO_ENC_OSD			0x05 //视频编码字幕参数
#define PARAM_TYPE_VIDEO_ENC_MP				0x06 //视频编码多画面参数
#define PARAM_TYPE_VIDEO_ENC_INPUTPORT		0x07 //视频编码视频源端口参数
#define PARAM_TYPE_VIDEO_ENC_POLL			0x08 //视频编码轮询参数
#define PARAM_TYPE_VIDEO_DEC_NET			0x09 //视频解码网络参数
#define PARAM_TYPE_VIDEO_DEC				0x0A //视频解码参数
#define PARAM_TYPE_VIDEO_DEC_PUB			0x0B //视频解码公共参数
#define PARAM_TYPE_VIDEO_DEC_RETRANSFER		0x0C //视频解码重传参数
#define PARAM_TYPE_AUDIO_ENC_NET			0x0D //音频编码网络参数
#define PARAM_TYPE_AUDIO_ENC				0x0E //音频编码参数
#define PARAM_TYPE_AUDIO_DEC_NET			0x0F //音频解码网络参数
#define PARAM_TYPE_AUDIO_DEC				0x10 //音频解码参数
#define PARAM_TYPE_SERIAL_PORT				0x11 //串口参数
#define PARAM_TYPE_SERIAL_PORT_PUB			0x12 //串口公共参数
#define PARAM_TYPE_INPUT_PIN				0x13 //并口参数
#define PARAM_TYPE_VIDEO_INPUT				0x14 //视频源端口参数
#define PARAM_TYPE_EN_MOTION_DETECT			0x15 //编码移动侦测参数
#define PARAM_TYPE_DE_MOTION_DETECT			0x16 //解码移动侦测参数
#define PARAM_TYPE_PPPOE					0x17 //PPPOE拨号参数
//前端存储start
#define PARAM_TYPE_TIMELY_INFO              0x18 //定时录象信息
#define PARAM_TYPE_PERIODLY_INFO            0x19 //周期录象信息
#define PARAM_TYPE_CHANNEL_BASIC_INFO       0x20 //通道基本信息
#define PARAM_TYPE_REC_SYS_INFO             0x21 //系统信息
#define PARAM_NMS_INFO                      0x22 //网管参数
//前端存储end
#define PARAM_SHADE_INFO                    0x23 //图象屏蔽参数
#define PARAM_XIANTELECOM_ALARM_INFO        0X24 //告警参数
#define PARAM_XIANTELECOM_BANNER_INFO       0X25 //横幅参数
#define PARAM_PIC_CONFIG_INFO				0X26 //图片信息
#define PARAM_MATRIX_CONFIG_INFO			0x27 //矩阵信息
#define PARAM_PIC_NAMERULE_INFO				0x28 //抓拍信息
#define PARAM_PUNMS_INFO					0x29 //网管相关参数

//add by shenxin 
//设备控制台类型能力集
#define MTI_CTRLTYPE_IPCCtrl    0x00001000
#define MTI_CTRLTYPE_NVRStation 0x00002000 
#define MTI_CTRLTYPE_SVR		0x00003000 
#define MTI_CTRLTYPE_WebCtrl    0x00004000 //这个是用于DVS控制台的
#define MTI_CTRLTYPE_IPCWebCtrl 0x00008000 //这个是用于IPCv7控制台的

//设备类型能力集
#define MTI_DEVTYPE_IPC       0x00001000     
#define MTI_DEVTYPE_NVR       0x00002000 
#define MTI_DEVTYPE_SVR       0x00003000 
#define MTI_DEVTYPE_DVS       0x00004000   
//end

#ifdef VSIP_MULTI_CHAN_SUPPORT
#define PARAM_MULTI_ENCCHAN_INFO            0x30 //双流相关参数
#endif

#define PARAM_PIN_EXT_INFO					0x31 //并口扩展卡
#define PARAM_EXTINPUTPIN_ENABLE_STATUS     0x32 //并口使能状态

#define PARAM_TYPE_MAX_VALUE				0x40 //最大参数类型

#define PARAM_TYPE_VIDEO_DECCOMP			0xB5 //解码器风格改变通知

//多画面风格
#define VSIP_VIDEO_FORMAT_NONE			    0x00 //无多画面
#define VSIP_VIDEO_FORMAT_PIP_4			    0x01 //画中画(比例1:4)
#define VSIP_VIDEO_FORMAT_PIP_9			    0x02 //画中画(比例1:9)
#define VSIP_VIDEO_FORMAT_PIP_16		    0x03 //画中画(比例1:16)

//扩展消息类型通知
#define MTI_XIAN_ALARM_TYPE				    1
#define MTI_TRANSPARENT_CHANNEL_TYPE	    2//[2-30]
#define MTI_CAPABILITY_TYPE				    3
#define MTI_VMT_ALARM_TYPE				    4
#define MTI_TRANSPARENT_DEC2ENC_TYPE	    5
#define MTI_TRANSPARENT_TYPE			    6
#define MTI_TRANSPARENT_CHANNEL_TYPE_EX	    7
//{{{{{{{{{{{{{{{ add by Royan 20140515 智能告警主机
#define MTI_VMT_ALARM_TYPE_EX			    11//todo 待系统部确认分配
//}}}}}}}}}}}}}}} add by Royan 20140515 智能告警主机
#define MTI_PINENABLE_CHANGED				10	//并口使能状态的改变，扩展消息内容为各并口状态

//BEGIN
#define MTI_XIANTELCOM_TYPE  2
//END
//

//串口类型
#define VSIP_SERIAL_LINE_DRIVER_NONE        0x00
#define VSIP_SERIAL_LINE_DRIVER_RS232       0x01
#define VSIP_SERIAL_LINE_DRIVER_RS422       0x02
#define VSIP_SERIAL_LINE_DRIVER_RS485       0x03

//音频输入类型
#define VSIP_AUDIO_INPUTTYPE_LINEIN		    0x00
#define VSIP_AUDIO_INPUTTYPE_MIC		    0x01

//串口数据通道类型
#define MTI_SERIAL_STREAM_TYPE_NONE		    0x00 //不使用
#define MTI_SERIAL_STREAM_TYPE_UDP		    0x01 //UDP通道
#define MTI_SERIAL_STREAM_TYPE_TCP		    0x02 //TCP通道
#define MTI_SERIAL_STREAM_TYPE_VSIP		    0x03 //消息通道

//串口数据协议类型
#define MTI_SERIAL_DATA_TYPE_NONE		    0x00 //透明通道
#define MTI_SERIAL_DATA_TYPE_VSIP		    0x01 //VSIP协议

//码流传输类型
#define MTI_MEDIA_STREAM_TYPE_UDP		    0x00 //UDP通道
#define MTI_MEDIA_STREAM_TYPE_TCP		    0x01 //TCP通道

//解码无视频时的显示
#define VSIP_PLAYER_STAEVE_MODE_COLOR_BAR	0x00 //彩色条
#define VSIP_PLAYER_STAEVE_MODE_LAST_IMAGE	0x01 //屏幕冻结
#define VSIP_PLAYER_STAEVE_MODE_BLACK_SCR   0x02 //黑屏
#define VSIP_PLAYER_STAEVE_MODE_SHOW_OSD	0x03 //屏幕冻结并字幕提示

//视频输出端口类型
#define MTI_VIDEO_OUT_TYPE_C				0x01 //复合C端子
#define MTI_VIDEO_OUT_TYPE_VGA				0x02 //VGA输出

//告警检测状态
#define MTI_ALARM_DETECT_NONE				0x00 //不检测
#define MTI_ALARM_DETECT_ONCE				0x01 //检测一个周期
#define MTI_ALARM_DETECT_REPEAT				0x02 //循环检测
#define MTI_ALARM_DETECT_ALWAYS				0x03 //始终检测

//摄像头类型
#define MTI_CAMERA_TYPE_SONY				0x01 //SONY
#define MTI_CAMERA_TYPE_AD					0x02 //AD
#define MTI_CAMERA_TYPE_PELCO				0x03 //PELCO
#define MTI_CAMERA_TYPE_PIH					0x04 //PIH
#define MTI_CAMERA_TYPE_PELCO_P				0x05 //PELCO_P
#define MTI_CAMERA_TYPE_PARCO		        0x06
#define MTI_CAMERA_TYPE_AB					0x07 //
#define MTI_CAMERA_TYPE_YUNTAI			    0x08
#define MTI_CAMERA_TYPE_SAMSUNG				0x09 //SAMSUNG
#define MTI_CAMERA_TYPE_YAAN			    0x0A

#define MTI_CAMERA_TYPE_AD168				0x011// AD168
#define MTI_CAMERA_TYPE_KALATEL				0x012// 卡特拉
#define MTI_CAMERA_TYPE_JN					0x013
#define MTI_CAMERA_TYPE_WV_CS650			0x014// 松下
#define MTI_CAMERA_TYPE_WV_CS850			0x015// 松下
#define MTI_CAMERA_TYPE_ALEC				0x016// 艾立克
#define MTI_CAMERA_TYPE_PANASONIC_O         0x0E //PANASONIC-OLD
#define MTI_CAMERA_TYPE_PANASONIC_N         0x0F //PANASONIC-NEW

//VGA输出刷新频率
#define MTI_VGA_OUT_FREQUENCY_60			0x01 //刷新频率60
#define MTI_VGA_OUT_FREQUENCY_75			0x02 //刷新频率75
#define MTI_VGA_OUT_FREQUENCY_85			0x03 //刷新频率85

//设备编解码类型
#define MTI_NONE		                  0	//视频编解码都不支持
#define MTI_ENCODER		                  1	//只支持视频编码
#define MTI_DECODER		                  2	//只支持视频解码
#define MTI_BOTH		                  3	//视频编解码都支持
#define MTI_SMARTSIGHT_ENCODER            4 //SMARTSIGHT视频编码支持
#define MTI_SMARTSIGHT_DECODER            5 //SMARTSIGHT视频解码支持
#define MTI_SMARTSIGHT_BOTH               6 //SMARTSIGHT视频编解码都支持

//其它定义
#define MTI_DISABLE							0
#define MTI_ENABLE							1
#define MTI_FAIL							0
#define MTI_SUCCESS							1
#define MTI_ERR_NODISK						2    //无硬盘
#define MTI_ERR_RECORDING					3    //正在录象
#define MTI_ERR_RECPLAYING					4    //放像通道已满,不能再申请
#define MTI_ERR_FILE_NOT_EXIT				5    //文件不存在(该路径下无此文件)
#define MTI_ERR_RECLENGTH_ZERO				6    //文件大小为零(录象时无码流)
#define MTI_ERR_UNKNOWN_FAIL				7    //文件大小为零(录象时无码流)
#define MTI_ERR_NVR_NO_BUF_RECORD           8    //NVR录像缓存记录不存在,请重新搜索录像(前端放像时)
#define MTI_ERR_NVR_BITRATE_OVERFLOW        9    //比特率过大(NVR2820比特率只支持64 - 2048，其余NVR设备无此限制)
#define MTI_ERR_INVALID_PU_RECCHN           10   //NVR通道不可用,一般是由于NVR此通道上无视频源
#define MTI_ERR_NO_VALID_REC_DISK           11   //NVR无可用的录像磁盘分区
#define MTI_ERR_NOT_SUPPORT                 12   //不支持的功能
#define MTI_ERR_PIC_TOO_MUSH				13	 //NVR查询抓拍图片过多，需要缩小查询范围
#define MTI_ERR_PU_OVERLOAD                 1000 //前端超负载
#define MTI_ERR_CREATE_PATH_FAIL            2053 //目录创建失败(极可能是磁盘只读)
#define MTI_ERR_RECLOG_NOT_EXIST            2072 //reclog文件不存在
#define MTI_ERR_ASF_STATE_ERROR             15846//播放ASF文件状态不正确
#define MTI_ERR_DELETE_NVR_REC              13002//删除NVR前端录像失败(VS平台不支持删除NVR前端录像)
#define MTI_ERR_NO_GPSDATA					3000  //没有找到gps数据 
#define MTI_ERR_EXIST_PLAY_GPS				3001  //已经在播放gps数据
#define MTI_ERR_PLTFM_FLOWMAX               30001 //3g流量超过最大值

#define MTI_FTP_SERVER_PORT       1730
#define MTI_NAT_LOCAL_SOURCE_PORT 60000

//设备供电方式
#define MTI_DEVPOWERTYPE_INVALID			(u8)0	/*!< 无效供电方式 */
#define MTI_DEVPOWERTYPE_STABLE				(u8)1	/*!< 稳压电源供电 */
#define MTI_DEVPOWERTYPE_ETHNET				(u8)2	/*!< 以太网供电 */
#define MTI_DEVPOWERTYPE_UPS				(u8)3	/*!< UPS供电 */
#define MTI_DEVPOWERTYPE_BATTERY			(u8)4	/*!< 电池供电 */

//字幕
#define		MTI_OSD_LABEL0		0			// 视频源字
#define		MTI_OSD_LABEL1		1			// 视频源字
#define		MTI_OSD_LABEL2		2			// 视频源字
#define		MTI_OSD_LABEL3		3			// 视频源字


#define     MTI_ALARM_LABEL0    10           //告警字幕
#define     MTI_ALARM_LABEL1    11           //告警字幕
#define     MTI_ALARM_LABEL2    12           //告警字幕
#define     MTI_ALARM_LABEL3    13           //告警字幕

#define		MTI_ALARM_PIN		0			//并口告警
#define		MTI_ALARM_MOTION	1			//移动侦测告警

// 拒绝原因
#define		MTI_NACK_REASON_OK			0		// 传输成功
#define		MTI_NACK_REASON_LOSTPACK	1		// 丢包
#define		MTI_NACK_REASON_TIMEOUT		2		// 超时
#define		MTI_NACK_REASON_TYPEERR		3		// 类型不匹配
#define		MTI_NACK_REASON_LENERR		4		// 消息头长度出错
#define		MTI_NACK_REASON_PROTOCOL	6		// 协议编号出错
#define		MTI_NACK_REASON_RECVING		7		// 正在传输
#define		MTI_NACK_REASON_FILEERR		8		// 文件不正确
#define     MTI_NACK_REASON_FINISH      9

#define     MTI_MAX_ALARM_ALIAS_LENGTH  32     //最大告警别名大小
#define     MTI_MAX_ALARM_NUM           10      //最大告警个数
#define     MTI_MAX_SENSOR_NUM          4     //最大采集单元个数
#define		MTI_MAX_CU_OPERTION_NUM     8192   //cu和pu之间透明通道最大数据的长度
#define     MTI_MAX_STREAM_PROP_LENGTH  128    //流媒体最大数值长度

#define   MAXLEN_DEVICE_MANUFACTURER    (u16)32 //制造商信息的最大长度

#define MTI_RELATE_ENCID_DISABLE	0xFF    //告警关联视频编码通道无效值
#define MTI_OUTPUT_DISABLE			0xFF	//告警控制输出无效值
#define DAYS_OF_WEEK				7		//一周的天数
#define CONN_CHEK_INTERNAL			30000	//链路检测默认间隔时间(30s)
#define MAX_SERIALPORT_DATA_LEN		1024	//最串口数据长度
#define MAX_STRING_LEN				64		//最大字符串长度
#define MAX_VERSION_STRING_LEN      64      //最大版本字符串长度
#define MAX_USER_NUM                256     //最大用户数目
#define MAX_VIDEOIN_PORT_NAME_LEN	8		//最大视频输入端口名长度
#define MAX_VIDEOIN_PORT_LONGNAME_LEN   64		//最大视频名长度
#define MAX_VIDEOIN_PORT_NUM		32		//最大视频输入端口数量
#define MAX_PIC_REGION_NUM			16		//多画面最大画面数量
#define MAX_POLL_PORT_NUM			4		//轮询最大端口数
#define MAX_NET_ADAPTER_NUM         10      //支持的网卡数目
#define MAX_TIME_PRECISION          8       //时间精度
#define MAX_MATRIX_PORT_NUM			256		//最大矩阵端口数量
#define MAX_SEPARATOR_LEN			8		//最大分隔符长度
#ifdef WIN32
#define MAX_UNIT_NUM				1024	//最大设备数,windows
#else
#define MAX_UNIT_NUM                5000    //最大设备数,linux
#endif
//最大视频源数，扩展UNIT数 add by Royan 20130628
#define MAX_UNIT_NUM_EXTEND          512     //MAX_UNIT_NUM_EXTEND+g_cPuiConf.m_wMaxVidSrcNum=tMtiStackConf.wMaxPuNum

#define MAX_PATH_LEN				256		//最大路径长度
#define MAX_BMPINFOBUF_LEN			1024	//最大buf长度 zlq add
#define MAX_ENTITY_TYPE				16	    //最大能力类型
#define MAX_ENTITY_TOTAL_NUM		1800  	//最大能力总数modify G.J.   20091020 (512->1500) 20150127（1500->1800）
#define MAX_OSD_FONT_WIDTH          18      //最大字符宽度  2014.03.14 ywy
//nvr128
#define MAX_ENTITY_NUM				128      //最大通道数 modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_ENC_NUM			128      //最大视频编码通道数modify G.J. 20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_DEC_NUM			64	    //最大视频解码通道数 modify by Royan 20131031 16 -> 64
//nvr128
#define MAX_AUDIO_ENC_NUM			128      //最大音频编码通道数modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_AUDIO_DEC_NUM			128   	 //最大音频解码通道数modify G.J.  20090903 16 -> 64,20120705 64 -> 128
#define MAX_VIDEO_INPUT_PORT_NUM	128	    //最大视频输入端口数modify G.J.  20090903 16 -> 64, modify by Royan 20140912 64->128
#define MAX_INPUT_PIN_NUM			1024	//最大并口输入通道数modify G.J.  20091020 16 -> 1024
//#define MAX_ONE_CHN_INPUTPIN_NUM    16      //一个设备通道最大可以接入的并口输入数 add G.J. 20091020
#define MAX_OUTPUT_PIN_NUM			16	    //最大并口输出通道数
#define MAX_SERIAL_PORT_NUM			4	    //最大串口数
#define MAX_PLAYER_NUM              4       //最大放象器个数
#define MAX_CUSTOMCAP_NUM			256	    //最大自定义能力个数
#define MAX_BACKENDCAP_NUM          256     //最大后端能力集个数
#define MAX_STREAMPROP_NUM          256     //最大后端能力集个数


//前断录象协议接口宏定义start
#define MAX_REC_FILE_NUM            20    //最大录象文件数目
#define MTI_DISK_NUM                16    //最大磁盘分区数目 modify G.J.20090903 MAX_DISK_NUM -> MTI_DISK_NUM 避免与cmsconst.h重复定义
#define MAX_CHANNEL_NUM             128   //最大通道数目 modify by Royan 20140912 64->128
#define MTI_BUFFER_LENGTH           2048  //buffer大小
#define MAX_FILE_NAME_LENGTH        64    //最大文件长度
#define MAX_FILE_PATH_LENGTH        64    //最大文件路径长度
#define MAX_DISK_PARTITION_NAME     64    //分区名大小
#define MAX_THIRDPARTY_NAME_LEN     512   //第三方厂商名称列表长度 20100317
//文件覆盖策略
#define MTI_STRATEGY_STOP_REC                0        //停止录象
#define MTI_STRATEGY_COVER_ALL_REC           1        //覆盖所有记录
#define MTI_STRATEGY_COVER_REC_EXCEPT_ALARM  2        //覆盖除告警录象外的记录
//周期录象策略
#define MTI_NONE_PERIOD                      0           //不周期录象
#define MTI_PERIOD_DAYLY                     1           //每天录象
#define MTI_PERIOD_SELECTION                 2           //周期录象
//录象文件操作类型
#define MTI_OPERATION_NORMAL                 0         //正常播放
#define MTI_OPERATION_STOP                   1         //停止操作
#define MTI_OPERATION_PAUSE                  2         //暂停操作
#define MTI_OPERATION_RESUME                 3         //继续操作
#define MTI_OPERATION_2XFORWARD              4         //2倍速快进操作
#define MTI_OPERATION_4XFORWARD              5         //4倍速快进操作
#define MTI_OPERATION_2XBACKWARD             6         //2倍速快退操作
#define MTI_OPERATION_4XBACKWARD             7         //4倍速快退操作
#define MTI_OPERATION_8XFORWARD				 8		   //8倍速快进操作
#define MTI_OPERATION_8XBACKWARD			 9		   //8倍速快退操作
#define MTI_OPERATION_FRAME					10		   //单帧播放
#define MTI_OPERATION_16XFORWARD			11		   // 16快进
#define MTI_OPERATION_16XBACKWARD			12		   // 16快退

//录象文件/事件类型/图片事件
#define MTI_TYPE_H264					   0x10 //h264文件类型
#define MTI_QUIEY_ALL_EVENT                0    //查询所有事件
#define MTI_TYPE_PIN_ALARM                 1    //并口告警录象
#define MTI_TYPE_MOTION_DETECT_ALARM       2    //移动侦测告警录象
#define MTI_TYPE_MANUAL                    3    //手动录象
#define MTI_TYPE_TIMELY                    4    //定时录象
#define MTI_TYPE_PERIODLY                  5    //周期录象
#define MTI_QUIEY_FILE                     6    //按文件查询
//录象器状态
#define MTI_INITIALIZED                    0     //初始化状态置0
#define MTI_REC_NORUNNING                  1     //未录象
#define MTI_REC_RUNNING                    2     //在录象
#define MTI_REC_FINISHED                   3     //录象结束
//图片组合查询
#define MTI_PIC_CHAN_QUERY					1	//通道号查询
#define MTI_PIC_TIME_QUERY					2	//时间查询
#define MTI_PIC_EVENT_QUERY					4	//事件查询

//前断录象协议接口宏定义end

#define MAX_DET_AREA_NUM			16  //最大移动侦测区域数
#define MAX_SHADE_AREA_NUM			16  //最大图象屏蔽区域数
#define MAX_IP_LEN					15  //IP地址字符串长度

//分区方式
#define MTI_SINGLE_SUBAREA			0x01 //单分区
#define MTI_MULTI_SUBAREA			0x02 //多分区

//终端链路检测时间间隔
#define MT_CONNCTION_CHECK_INTERVAL         10*1000

//operation type
#define OPERATION_START_FILE_TRANS_REQ     0
#define OPERATION_STOP_FILE_TRANS_REQ      1
#define OPERATION_FILE_TRANS_DATA          2
#define OPERATION_START_FILE_TRANS_ACK     3
#define OPERATION_GET_GUID_REQ             4
#define OPERATION_GET_GUID_ACK             5
#define OPERATION_UDP_TRANSPARENT_DATA     6
#define OPERATION_START_RECV_EXTEND_REQ    7
#define OPERATION_INDENTIFY_USERPASSWORD_REQ   8
#define OPERATION_INDENTIFY_USERPASSWORD_ACK   9
//zlq add
#define OPERATION_CHANGE_PASSWORD_REQ   10
#define OPERATION_CHANGE_PASSWORD_ACK   11
#define OPERATION_SETBMPOSD_DATA_REQ	12
#define OPERATION_SETBMPOSD_DATA_ACK	13
#define OPERATION_TRANSPARENT_CFG_MSG	14
#define OPERATION_TRANSPARENT_CTRL_MSG	15
#define OPERATION_DIRECTTRANSCHAN_MSG	16
#define OPERATION_QUERY_PICFILE_REQ		17
#define OPERATION_QUERY_PICFILE_ACK		18
#define OPERATION_QUERY_PICFILE_NUM_REQ	19
#define OPERATION_QUERY_PICFILE_NUM_ACK	20
#define OPERATION_DOWNLOAD_PICFILE_REQ	21
#define OPERATION_DOWNLOAD_PICFILE_ACK	22
#define OPERATION_DEL_PICFILE_REQ		23
#define OPERATION_DEL_PICFILE_ACK		24
#define OPERATION_SET_SNAPSHOTCFG_REQ	25
#define OPERATION_SET_SNAPSHOTCFG_ACK	26
#define OPERATION_GET_SNAPSHOTCFG_REQ	27
#define OPERATION_GET_SNAPSHOTCFG_ACK	28
#define OPERATION_DEL_PICFILEEX_REQ		29
#define OPERATION_SET_MATRIX_REQ		30
#define OPERATION_SET_MATRIX_ACK		31
#define OPERATION_GET_MATRIX_REQ		32
#define OPERATION_GET_MATRIX_ACK		33
#define OPERATION_START_SNAPSHOT_REQ	34
#define OPERATION_START_SNAPSHOT_ACK	35
#define OPERATION_STOP_SNAPSHOT_REQ		36
#define OPERATION_STOP_SNAPSHOT_ACK		37
#define OPERATION_GET_SNAPSHOTNAME_REQ	38
#define OPERATION_GET_SNAPSHOTNAME_ACK	39
#define OPERATION_SET_SNAPSHOTNAME_REQ	40
#define OPERATION_SET_SNAPSHOTNAME_ACK	41
#define OPERATION_SET_PUNMS_REQ			42
#define OPERATION_SET_PUNMS_ACK			43
#define OPERATION_GET_PUNMS_REQ			44
#define OPERATION_GET_PUNMS_ACK			45
#define OPERATION_QUERY_PICFILE_NACK    73

//GPS
#define OPERATION_CREATE_SUBSCRIBE_REQ   67
#define OPERATION_CREATE_SUBSCRIBE_ACK   68
#define OPERATION_GET_SUBSCRIBE_REQ      69
#define OPERATION_GET_SUBSCRIBE_ACK      70
#define OPERATION_START_THIRDPARTY_RECV_REQ 80 //20100317

#define OPERATION_SET_SERIALBMP_DATA_REQ                     0xE1
#define OPERATION_SET_SERIALBMP_DATA_ACK                     0xE2

#define OPERATION_SET_OSD_VIDSRC_REQ	87
#define OPERATION_SET_OSD_VIDSRC_ACK	88
#define OPERATION_GET_OSD_VIDSRC_REQ	89
#define OPERATION_GET_OSD_VIDSRC_ACK	90
#define OPERATION_SET_OSD_CHANNEL_REQ	91
#define OPERATION_SET_OSD_CHANNEL_ACK	92
#define OPERATION_GET_OSD_CHANNEL_REQ	93
#define OPERATION_GET_OSD_CHANNEL_ACK	94

//透明通道类型
#define TRANSPARENT_TYPE_CFG			0
#define TRANSPARENT_TYPE_CTRL			1
//add end

//滚动速度
#define MTI_RUNSPEED_VERYSLOW    0
#define MTI_RUNSPEED_SLOW        1
#define MTI_RUNSPEED_NORMAL      2
#define MTI_RUNSPEED_FAST        3
#define MTI_RUNSPEED_VERYFAST    4
//滚动方式
#define MTI_RUNMODE_DOWN2UP      0
#define MTI_RUNMODE_LEFT2RIGHT   1
#define MTI_RUNMODE_STILL        2
#define MTI_RUNMODE_STILL_LALIGN    2     //左对齐静止
#define MTI_RUNMODE_STILL_RALIGN    3     //右对齐静止
#define MTI_RUNMODE_STILL_MIDDLE    4     //居中对齐静止


//对齐方式
#define MTI_ALIGN_TOLEFT         0
#define MTI_ALIGN_TORIGHT        1
#define MTI_ALIGN_TOCENTER       2

#define  MTI_PU_TRANS_DATA_LEN 2048

#define MAX_RES_NUM             32   //支持的分辨率个数 add by ywy
#define MAX_VID_FORMAT_NUM      16   //支持的最大视频文件格式数 add by ywy

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


//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
struct TVidResItem
{
public:
	TVidResItem()
	{
		Clear();
	}
	void Clear()
	{
		resolutionType = MTI_VIDEO_RESOLUTION_NONE;
		height = 0xffff;
		width = 0xffff;
	}
	void SetResolutionType(const u8 byType)
	{
		resolutionType = byType;
	}
	u8 GetResolutionType() const
	{
		return resolutionType;
	}
	void SetHeight(const u16 wHeight)
	{
		//ASSERT(resolutionType == 0);
		height = wHeight;
	}
	u16 GetHeight() const
	{
		return height;
	}
	void SetWidth(const u16 wWidth)
	{
		//ASSERT(resolutionType == 0);
		width = wWidth;
	}
	u16 GetWidth() const
	{
		return width;
	}
	//1.老方案，按照枚举值比大小
	//2.老方案均小于新方案
	//3.新方案比宽，宽相等时比高
	bool operator<(const TVidResItem&tItem) const
	{
		if (resolutionType == MTI_VIDEO_RESOLUTION_NONE && tItem.resolutionType == MTI_VIDEO_RESOLUTION_NONE)
		{
			return (width < tItem.width) || (width == tItem.width && height < tItem.height); 
		}
		if (resolutionType == MTI_VIDEO_RESOLUTION_NONE)
			return false;
		if (tItem.resolutionType == MTI_VIDEO_RESOLUTION_NONE)
			return true;
		return resolutionType < tItem.resolutionType;
	}

protected:
	//兼容以前方案，如果该值非 VIDEO_RESOLUTION_NONE ，则以该值为准。
	u8 resolutionType;		//分辨率类型，VIDEO_RESOLUTION_**
	u16 width;				//宽
	u16 height;				//高
}PACKED;


#ifdef _NEW_HW //新分辨率方案
typedef std::set<TVidResItem> TVidResSet;
typedef std::map<CVidType, TVidResSet> TVidTypeResMap;
//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化
#endif

typedef u8 CVidType;
struct TVidResFormatInfo
{
	CVidType byVidFormatType;
	TVidResItem abyVidResItem[MAX_RES_NUM];
	u8 byResItemNum;     //当前编码格式支持的分辨率个数，即abyVidResItem的有效元素个数
}PACKED;


//支持录像日历查询
/*
1、查询全年每月录像分布情况（比如，年=2013，月=0，日=0）
2、查询指定月份录像分布情况（比如，年=2013，月=1，日=0）
3、查询指定日期录像分布情况（比如，年=2013，月=1，日=3）
年为公元年份号，月取值范围为0~12，日取值范围为0~31。
*/
struct TMTIRecListQryReq
{
    u8 byChanId;       //编码通道
    u16 wYear;         //查询全年每月录像分布情况（比如，年=2013，月=0，日=0）
    u8 byMonth;        //查询指定月份录像分布情况（比如，年=2013，月=1，日=0）
    u8 byDay;          //查询指定日期录像分布情况（比如，年=2013，月=1，日=3）
    u8 byType;         //事件类型：MTI_QUIEY_ALL_EVENT等

    void SwapToNetwork()
    {
        wYear = htons(wYear);
    }
    void SwapFromNetwork()
    {
        wYear = ntohs(wYear);
    }
    BOOL PackedToBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( Buf, &byChanId, sizeof(byChanId));
        Buf += sizeof(byChanId);
        memcpy( Buf, &wYear, sizeof(wYear));
        Buf += sizeof(wYear);
        memcpy( Buf, &byMonth, sizeof(byMonth));
        Buf += sizeof(byMonth);
        memcpy( Buf, &byDay, sizeof(byDay));
        Buf += sizeof(byDay);
        memcpy( Buf, &byType,sizeof(byType));

        return TRUE;
    }

    BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( &byChanId, Buf, sizeof(byChanId));
        Buf += sizeof(byChanId);
        memcpy( &wYear, Buf, sizeof(wYear));
        Buf += sizeof(wYear);
        memcpy( &byMonth, Buf, sizeof(byMonth));
        Buf += sizeof(byMonth);
        memcpy( &byDay, Buf, sizeof(byDay));
        Buf += sizeof(byDay);
        memcpy( &byType,Buf, sizeof(byType));

        return TRUE;
    }
    unsigned short PackedLen()
    {
        return (sizeof(byChanId)+sizeof(wYear)+sizeof(byMonth)+sizeof(byDay)+sizeof(byType));
    }
}PACKED;

/*
对于查询1，每位代表1个月，第0位代表1月份，第1位代表2月份，以此类推（如果某位为0，代表无录像；否则，有录像）。其余位默认为0。
对于查询2，每位代表1天，第0位代表1日，第1位代表2日，以此类推（如果某位为0，代表无录像；否则，有录像）。其余位默认为0。
对于查询3，每位代表1小时，第0位代表1时，第1位代表2时，以此类推（如果某位为0，代表无录像；否则，有录像）。其余位默认为0。
*/
struct TMTIRecListQryRsp
{
    u32 dwQryResult;       //查询结果
    
    void SwapToNetwork()
    {
        dwQryResult = htonl(dwQryResult);
    }
    void SwapFromNetwork()
    {
        dwQryResult = ntohl(dwQryResult);
    }
    BOOL PackedToBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( Buf, &dwQryResult, sizeof(dwQryResult));

        return TRUE;
    }

    BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
    {
        if( PackedLen() > BufLen)
        {
            return FALSE;
        }

        memcpy( &dwQryResult, Buf, sizeof(dwQryResult));

        return TRUE;
    }
    unsigned short PackedLen()
    {
        return (sizeof(dwQryResult));
    }
}PACKED;

//modify by Royan 20140304 通道状态通知信息
typedef struct TMTIChnStatInfo
{
	u16		wEntityID;		//通道id，ID modify G.J. 20091020 u8->u16
	u32 	dwChnStat;		//通道状态，0：unused；1：online；2：offline
}PACKED TMTI_CHAN_STAT_INFO, *PTMTI_CHAN_STAT_INFO;

//支持natagent前端码流控制结果
#define   VSIP_MAX_MEDIA_ADDR_NUM       8
#define   VSIP_MAX_MEDIA_TYPE_NUM       2

struct TMTIMediaAddr
{
    u32     dwIp;
    u16     wPort;
}PACKED;

struct TMTIMediaChn
{
    TPunchKey       tPunchKey;                                 //打洞唯一标识
    u8				byAddrNum;                                 //地址数目
    TMTIMediaAddr	atAddrList[VSIP_MAX_MEDIA_ADDR_NUM];       //地址列表

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memset( Buf, VSIP_VALUE_TYPE_STRING, sizeof(VSIP_VALUE_TYPE_STRING) );
		Buf += sizeof(VSIP_VALUE_TYPE_STRING);
		memcpy( Buf, &(tPunchKey.m_wLen), sizeof(tPunchKey.m_wLen) );
		Buf += sizeof(tPunchKey.m_wLen);
		memcpy( Buf, &(tPunchKey.m_achKey), sizeof(tPunchKey.m_achKey) );
		Buf += sizeof(tPunchKey.m_achKey);
		memcpy( Buf, &(byAddrNum), sizeof(byAddrNum) );
		Buf += sizeof(byAddrNum);
		for ( s32 nAddrIndex=0; nAddrIndex<byAddrNum; nAddrIndex++ )
		{
			memcpy( Buf, &(atAddrList[nAddrIndex].dwIp), sizeof(atAddrList[nAddrIndex].dwIp) );
			Buf += sizeof(atAddrList[nAddrIndex].dwIp);
			memcpy( Buf, &(atAddrList[nAddrIndex].wPort), sizeof(atAddrList[nAddrIndex].wPort) );
			Buf += sizeof(atAddrList[nAddrIndex].wPort);
		}

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		//memcpy( &VSIP_VALUE_TYPE_STRING, Buf, sizeof(VSIP_VALUE_TYPE_STRING) );
		Buf += sizeof(VSIP_VALUE_TYPE_STRING);
		memcpy( &(tPunchKey.m_wLen), Buf, sizeof(tPunchKey.m_wLen) );
		Buf += sizeof(tPunchKey.m_wLen);
		memcpy( &(tPunchKey.m_achKey), Buf, sizeof(tPunchKey.m_achKey) );
		Buf += sizeof(tPunchKey.m_achKey);
		memcpy( &(byAddrNum), Buf, sizeof(byAddrNum) );
		Buf += sizeof(byAddrNum);
		for ( s32 nAddrIndex=0; nAddrIndex<byAddrNum; nAddrIndex++ )
		{
			memcpy( &(atAddrList[nAddrIndex].dwIp), Buf, sizeof(atAddrList[nAddrIndex].dwIp) );
			Buf += sizeof(atAddrList[nAddrIndex].dwIp);
			memcpy( &(atAddrList[nAddrIndex].wPort), Buf, sizeof(atAddrList[nAddrIndex].wPort) );
			Buf += sizeof(atAddrList[nAddrIndex].wPort);
		}

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(VSIP_VALUE_TYPE_STRING) + sizeof(tPunchKey.m_wLen) + tPunchKey.m_wLen
			+ sizeof(byAddrNum) + (sizeof(u32) + sizeof(u16))*byAddrNum;
		return wPacketLen;
	}
}PACKED;

struct TMTIMediaDesc
{
    TMTIMediaChn    tRtpAddrList;                              //rtp地址列表
    TMTIMediaChn    tRtcpAddrList;                             //rtcp地址列表

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tRtpAddrList.PackedToBuf(Buf, BufLen);
		Buf += tRtpAddrList.PackedLen();
		tRtcpAddrList.PackedToBuf(Buf, BufLen-tRtpAddrList.PackedLen());

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tRtpAddrList.UnpackedFromBuf(Buf, BufLen);
		Buf += tRtpAddrList.PackedLen();
		tRtcpAddrList.UnpackedFromBuf(Buf, BufLen-tRtpAddrList.PackedLen());

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = tRtpAddrList.PackedLen() + tRtcpAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIDevMediaParam
{
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息
}PACKED;

//媒体发送接口
struct TMTIStartSendMediaReq
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //传输类型:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartSendMediaRsp
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //传输类型:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP    
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartSendMediaAck
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

struct TMTIStopSendMediaReq
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

//typedef    TMTIOPResultResp   TMTIStopSendMediaRsp;            //使用通用应答

//媒体接收接口
struct TMTIStartRecvMediaReq
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    u8              byTransType;                               //传输类型:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);
		memcpy( Buf, &byTransType, sizeof(byTransType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);
		memcpy( &byTransType, Buf, sizeof(byTransType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType) + sizeof(byTransType);
		return wPacketLen;
	}
}PACKED;

struct TMTIStartRecvMediaRsp
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStartRecvMediaAck
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );
		Buf += sizeof(byMediaType);

		u16 wLen = BufLen - sizeof(wChnId) - sizeof(byMediaType);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIStopRecvMediaReq
{
    u16             wChnId;                                    //设备通道ID
    u8              byMediaType;                               //媒体类型:MTI_BOTH_MEDIA_TYPE/MTI_AUDIO_TYPE/MTI_VIDEO_TYPE

	void SwapToNetwork()
	{
		wChnId = htons(wChnId);
	}

	void SwapFromNetwork()
	{
		wChnId = ntohs(wChnId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wChnId, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( Buf, &byMediaType, sizeof(byMediaType) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wChnId, Buf, sizeof(wChnId) );
		Buf += sizeof(wChnId);
		memcpy( &byMediaType, Buf, sizeof(byMediaType) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(wChnId) + sizeof(byMediaType);
		return wPacketLen;
	}
}PACKED;

//typedef    TMTIOPResultResp   TMTIStopRecvMediaRsp;            //使用通用应答

//录像播放
struct TMTIRecStartPlayReq
{
    u32 dwIndexId;                                             //文件序号
    u8  byType;                                                //文件类型
    u32 dwStartTime;                                           //起始时间
    u32 dwEndTime;                                             //结束时间
    u8              byTransType;                               //传输类型:MTI_MEDIA_STREAM_TYPE_UDP/MTI_MEDIA_STREAM_TYPE_TCP
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
		dwStartTime = htonl(dwStartTime);
		dwEndTime = htonl(dwEndTime);
	}

	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
		dwStartTime = ntohl(dwStartTime);
		dwEndTime = ntohl(dwEndTime);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &byType, sizeof(byType) );
		Buf += sizeof(byType);
		memcpy( Buf, &dwStartTime, sizeof(dwStartTime) );
		Buf += sizeof(dwStartTime);
		memcpy( Buf, &dwEndTime, sizeof(dwEndTime) );
		Buf += sizeof(dwEndTime);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byType) - sizeof(dwStartTime) - sizeof(dwEndTime);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( &byType, Buf, sizeof(byType) );
		Buf += sizeof(byType);
		memcpy( &dwStartTime, Buf, sizeof(dwStartTime) );
		Buf += sizeof(dwStartTime);
		memcpy( &dwEndTime, Buf, sizeof(dwEndTime) );
		Buf += sizeof(dwEndTime);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byType) - sizeof(dwStartTime) - sizeof(dwEndTime);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwIndexId) + sizeof(byType) + sizeof(dwStartTime) + sizeof(dwEndTime)
			+ tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartPlayRsp
{
    u32             dwPlayId;                                  //放像ID
    TMTIMediaDesc	tVideoMediaDesc;                           //视频媒体信息
    TMTIMediaDesc	tAudioMediaDesc;                           //音频媒体信息

	void SwapToNetwork()
	{
		dwPlayId = htonl(dwPlayId);
	}

	void SwapFromNetwork()
	{
		dwPlayId = ntohl(dwPlayId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwPlayId, sizeof(dwPlayId) );
		Buf += sizeof(dwPlayId);

		u16 wLen = BufLen - sizeof(dwPlayId);
		tVideoMediaDesc.PackedToBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwPlayId, Buf, sizeof(dwPlayId) );
		Buf += sizeof(dwPlayId);

		u16 wLen = BufLen - sizeof(dwPlayId);
		tVideoMediaDesc.UnpackedFromBuf(Buf, wLen);
		Buf += tVideoMediaDesc.PackedLen();
		wLen -= tVideoMediaDesc.PackedLen();
		tAudioMediaDesc.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwPlayId) + tVideoMediaDesc.PackedLen() + tAudioMediaDesc.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartPlayAck
{
    u32             dwPlayId;                                  //放像ID

	void SwapToNetwork()
	{
		dwPlayId = htonl(dwPlayId);
	}

	void SwapFromNetwork()
	{
		dwPlayId = ntohl(dwPlayId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwPlayId, sizeof(dwPlayId) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwPlayId, Buf, sizeof(dwPlayId) );

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwPlayId);
		return wPacketLen;
	}
}PACKED;

//录像下载
struct TMTIRecStartExportReq
{
    u32 dwIndexId;                                             //文件序号
    u8  byConnectType;                                         //TCP/UDP
    TMTIMediaChn tAddrList;                                    //地址列表

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
	}

	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &byConnectType, sizeof(byConnectType) );
		Buf += sizeof(byConnectType);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byConnectType);
		tAddrList.PackedToBuf(Buf, wLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId) );
		Buf += sizeof(dwIndexId);
		memcpy( &byConnectType, Buf, sizeof(byConnectType) );
		Buf += sizeof(byConnectType);

		u16 wLen = BufLen - sizeof(dwIndexId) - sizeof(byConnectType);
		tAddrList.UnpackedFromBuf(Buf, wLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = sizeof(dwIndexId) + sizeof(byConnectType) + tAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;

struct TMTIRecStartExportRsp
{
    TMTIMediaChn tAddrList;                                    //地址列表

	void SwapToNetwork()
	{
	}

	void SwapFromNetwork()
	{
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tAddrList.PackedToBuf(Buf, BufLen);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		tAddrList.UnpackedFromBuf(Buf, BufLen);

		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 wPacketLen = tAddrList.PackedLen();
		return wPacketLen;
	}
}PACKED;


//系统时间参数结构
typedef struct TMTISysTime
{
	u16		wYear;		//年
	u8		byMonth;	//月 1-12
	u8		byDay;		//日 1-31
	u8		byHour;		//时 0-23
	u8		byMinute;	//分 0-59
	u8		bySecond;	//秒 0-59

	void SwapToNetwork()
	{
		wYear = htons(wYear);
	}

	void SwapFromNetwork()
	{
		wYear = ntohs(wYear);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wYear, sizeof(wYear) );
		Buf += sizeof(wYear);
		memcpy( Buf, &byMonth, sizeof(byMonth) );
		Buf += sizeof(byMonth);
		memcpy( Buf, &byDay, sizeof(byDay) );
		Buf += sizeof(byDay);
		memcpy( Buf, &byHour, sizeof(byHour) );
		Buf += sizeof(byHour);
		memcpy( Buf, &byMinute, sizeof(byMinute) );
		Buf += sizeof(byMinute);
		memcpy( Buf, &bySecond, sizeof(bySecond) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &wYear, Buf, sizeof(wYear) );
		Buf += sizeof(wYear);
		memcpy( &byMonth, Buf, sizeof(byMonth) );
		Buf += sizeof(byMonth);
		memcpy( &byDay, Buf, sizeof(byDay) );
		Buf += sizeof(byDay);
		memcpy( &byHour, Buf, sizeof(byHour) );
		Buf += sizeof(byHour);
		memcpy( &byMinute, Buf, sizeof(byMinute) );
		Buf += sizeof(byMinute);
        memcpy( &bySecond, Buf, sizeof(bySecond) );
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(u16)+5*sizeof(u8));
	}

}PACKED TMTI_SYS_TIME, *PTMTI_SYS_TIME;

//视频图象分辨率
typedef struct TVideoResolution
{
	u8 byAuto;
	u8 byQCIF;
	u8 byCIF;
	u8 by2CIF;
	u8 by4CIF;
#ifdef VSIP_ADD_NEW_RESOLUTION
	u8 byVGA;
	u8 byQQCIF;
	u8 byQVGA; 
	u8 by720P;
	u8 by1080P;
	u8 byQXGA;
	u8 byUXGA;
	u8 byQuadVGA;
#endif
}PACKED TVIDEO_RESOLUTION, *PTVIDEO_RESOLUTION;

//addbeg NAT穿透结构体 20100317
typedef struct TVsipStartNatDetectParam
{
    u32 dwSourceIP;
	u32 dwDestIP;
	u16 wDestPort;
    u16 dwSourcePort;
	u8  byDeviceType;
} PACKED TVSIP_STARTNATDETECTPARAM, *PTVSIP_STARTNATDETECTPARAM;
			
typedef struct TVsipStopNatDetectParam
{
    u32 dwIPAddr;
} PACKED TVSIP_STOPNATDETECTPARAM, *PTVSIP_STOPNATDETECTPARAM;
//addend 20100317

//视频压缩格式
typedef struct TVideoType
{
	u8 bySN4;
	u8 byMPEG4;
	u8 byH261;
	u8 byH263;
	u8 byH264;
	u8 byH265;
	u8 bySvac;
}PACKED TVIDEO_TYPE, *PTVIDEO_TYPE;

//视频图象分辨率
typedef struct TSPVideoResolution
{
	TVideoResolution tSN4;
	TVideoResolution tMEPG4;
	TVideoResolution tH261;
	TVideoResolution tH263;
	TVideoResolution tH264;
	TVideoResolution tH265;
	TVideoResolution tSvac;
}PACKED TSP_VIDEO_RESOLUTION, *PTSP_VIDEO_RESOLUTION;

//音频压缩格式
typedef struct TAudioType
{
	u8 byPCMA;
	u8 byPCMU;
	u8 byGSM;
	u8 byG729;
	u8 byADPCM;
	u8 byG7221C;
	u8 byG722;
	u8 byAACLC;
}PACKED TAUDIO_TYPE, *PTAUDIO_TYPE;

//串口类型
typedef struct TSerialType
{
	u8 byRS232;
	u8 byRS422;
	u8 byRS485;
}PACKED TSERIAL_TYPE, *PTSERIAL_TYPE;

//链接类型
typedef struct TConnType
{
	u8 byUdpUnicast;
	u8 byUdpMulticast;
	u8 byUdpBroadcast;
	u8 byTcpServer;
	u8 byTcpClient;
	u8 bySSL;
}PACKED TCONNECTION_TYPE, *PTCONNECTION_TYPE;

//搜索结果参数
typedef struct TMTIDiscoveryCBInfo
{
	TVsipGuid	tGuid;							//设备GUID值(只读)
	char	achName[MAX_STRING_LEN+1];			//设备名
	char	achTypeName[MAX_STRING_LEN+1];		//设备类型名(只读)
	u8		byDevType;							//设备类型(MTI_ENCODER等)
	u32		dwUnitIpAddr;						//设备IP地址
	u16		wVsipPort;							//VSIP端口号
}PACKED TMTI_DISCOVERY_CB_INFO, *PTMTI_DISCOVERY_CB_INFO;

//系统参数结构
typedef struct TMTISysParam
{
	char	achName[MAX_STRING_LEN+1];			//设备名
	char	achTypeName[MAX_STRING_LEN+1];		//设备类型名(只读)
	u8		byDevType;							//设备类型(MTI_ENCODER等)
}PACKED TMTI_SYS_PARAM, *PTMTI_SYS_PARAM;

//PPPOE参数结构
typedef struct TMTIPPPOEParam
{
	u8		byPPPOEState;						//PPPOE状态 MTI_DISABLE-不使用, MTI_ENABLE-使用
	char	achUsername[MAX_USER_NUM];			//拨号用户名
	char	achPassword[MAX_USER_NUM];			//拨号密码
	u8		byAutoDialState;					//自动拨号状态 MTI_DISABLE-不使用, MTI_ENABLE-使用
	u32		dwAutoDialWait;						//自动拨号等待时间(秒)
	u32		dwAutoDialTry;						//自动拨号重试次数
	u8		byDebugState;						//Debug状态 MTI_DISABLE-不使用, MTI_ENABLE-使用
    u16     wLcpEchoSendInterval;				//链接建立后，发送LCP-ECHO包的时间间隔
    u16     wLcpEchoRetryTimes;					//发送几次LCP-ECHO包没有得到相应则认为链接断开
}PACKED TMTI_PPPOE_PARAM, *PTMTI_PPPOE_PARAM;

//系统公共参数结构
typedef struct TMTISysPubParam
{
	u8      byDetPacketSend;                    //是否发送NAT探测包
	u32		dwUnitIpAddr;                       //设备IP地址
	u16		wVsipPort;                          //VSIP端口号
	u32		dwSubnetMask;                       //设备子网掩码(网络序)
	u32		dwGateWay;                          //网关地址(网络序)
	char	achVersion[MAX_VERSION_STRING_LEN+1];       //设备版本号(只读)
	u32		dwDiscoveryIpAddr;                  //搜索的注册地址
	u32		dwVsipMultiIpAddr;                  //搜索的组播地址
	char	achWebUsername[MAX_STRING_LEN+1];   //嵌入式Web用户名
	char	achWebPassword[MAX_STRING_LEN+1];   //嵌入式Web密码
	char	achWebGuestname[MAX_STRING_LEN+1];   //嵌入式Web操作员用户名	 zlq add
	char	achWebGuestPassword[MAX_STRING_LEN+1];   //嵌入式Web操作员密码	 zlq add
	u16		wAlarmContinuance;                  //告警恢复时间(秒)
	u8      bySubAreaType;                      //分区格式(只读 MTI_SINGLE_SUBAREA 单分区
	                                            //	       MTI_MULTI_SUBAREA多分区)
	u8      byMaxTranferNum;                    //最大直传路数
	char    abyPuid[MAX_VERSION_STRING_LEN];    //puid
	char    abyPassword[MAX_VERSION_STRING_LEN]; //验证密码
	u8      byIsDecCombined;                     //是否是解码合成图象
	u16		wAppType;							//应用类型
	u8		byMatrixValid;						//是否接入矩阵
}PACKED TMTI_SYS_PUB_PARAM, *PTMTI_SYS_PUB_PARAM;

typedef struct TMTIUserPassword
{
    char abyUserName[MAX_STRING_LEN];
	char abyPassWord[MAX_STRING_LEN];
}PACKED TMTI_USER_PASSWORD,*PTMTI_USER_PASSWORD;

//公共网络参数结构
typedef struct TMTICommonNetParam
{
	u8		byInitStreamState;		//码流初始化状态 MTI_DISABLE-不发送, MTI_ENABLE-发送
	u8		byCurStreamState;		//码流当前状态 MTI_DISABLE-未发送, MTI_ENABLE-发送
	TConnType	tSupportedConnType;	//支持的码流链接类型
	u8		byConnectionType;		//码流链接类型
	u32		dwStreamIpAddr;			//码流传输地址
	u16		wStreamPort;			//码流传输端口
}PACKED TMTI_COMMONNET_PARAM, *PTMTI_COMMONNET_PARAM;

//视频编码参数结构
typedef struct TMTIVideoEncParam
{
	TMTIVideoEncParam()
	{
		memset(this,0,sizeof(TMTIVideoEncParam));
	}
	TVideoType tSupportedVideoType;	//支持的视频格式(只读)
	TSPVideoResolution tSupportedVideoResolution;	//支持的视频分辨率(只读)
	u8		byVideoType;			//数据压缩格式
	u8		byVideoResolution;		//图象分辨率
	//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
#ifdef _ROYAN	
	TVidTypeResMap tVidTypeResMap;  // 支持的压缩格式和分辨率 //delete by yuwenying 2014.4.15
#endif
	TVidResFormatInfo abyVidResFormatInfo[MAX_VID_FORMAT_NUM];

	TVidResItem tVidResItem;        // 当前分辨率
	
	//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

	u8      bySupportVidFormatNum; //当前通道支持的编码格式数据量	
	u8		byFrameRate;			//帧率 1-25
	u8		byMaxFrameRate;			//最大帧率
	u8		byQuality;				//图象质量 1-2 (质量优先/速度优先)
	u32		dwBitRate;				//编码比特率 64K-4M
	u8		byMaxQuantization;		//最大量化因子 1-31
	u8		byMinQuantization;		//最小量化因子 1-31
	u16		wInterval;				//关键帧间隔 25-2500
	char	chBrightness;			//亮度 -128～127
	char	chContrast;				//对比度 -128～127
	char	chSaturation;			//饱和度 -128～127
	u8		byDnyValue;				//动态载荷值
    u8		bySharpness;			//锐度：0: not support; 1-4: available
	//{{{{{{{{{{{{{{{ add by Royan 20140528 高级编码参数-编码等级
    u8		byMpValue;			    //图像编码协议等级
	//}}}}}}}}}}}}}}} add by Royan 20140528 高级编码参数-编码等级
	u32     dwClockRate;            //视频采样率
}PACKED TMTI_VIDEOENC_PARAM, *PTMTI_VIDEOENC_PARAM;

//视频编码公共参数结构
typedef struct TMTIVideoEncPubParam
{
	u8		byMaxVideoEncNum;	//最大可用视频编码通道数量(只读)
	u8		byVideoEncUsed;		//启用的视频编码通道数
	u8		abyVideoEncCap[MAX_VIDEOIN_PORT_NAME_LEN];	//支持的视频编码能力(只读)
	u8		byMaxVideoEncCapNum;	//支持的编码能力个数(只读)
}PACKED TMTI_VIDEOENC_PUB_PARAM, *PTMTI_VIDEOENC_PUB_PARAM;

//视频编码字幕参数结构
typedef struct TMTIVideoEncOSDParam
{
	u8		byShowTimeState;		//是否显示时间 MTI_DISABLE-不显示, MTI_ENABLE-显示
	u8		byOSDState;				//编码字幕状态 MTI_DISABLE-无字幕, MTI_ENABLE-有字幕
}PACKED TMTI_VIDEOENC_OSD_PARAM, *PTMTI_VIDEOENC_OSD_PARAM;

//视频编码多画面参数结构
typedef struct TMTIVideoEncMPParam
{
	u8		abyEncodeMember[MAX_PIC_REGION_NUM]; //多画面对应的视频源端口号
	u8		byEncodeMemberNum; 		//多画面对应的视频源端口数量
	u8		byEncodeStyle;			//多画面编码风格类型
}PACKED TMTI_VIDEOENC_MP_PARAM, *PTMTI_VIDEOENC_MP_PARAM;

//视频编码视频源端口参数结构
typedef struct TMTIVideoEncInputPortParam
{
	u8		abyValidVideoInputPort[MAX_VIDEO_INPUT_PORT_NUM]; //可用的视频输入端口号(只读)
	u8		byValidVideoInputPortNum; 	//可用的视频输入端口数量(只读)
	u8		byVideoInputPort;		//当前使用的视频输入端口号
}PACKED TMTI_VIDEOENC_INPUT_PORT_PARAM, *PTMTI_VIDEOENC_INPUT_PORT_PARAM;

//视频编码轮询参数结构
typedef struct TMTIVideoEncPollParam
{
	u8		byPollState;		//轮询状态 MTI_DISABLE-不轮询, MTI_ENABLE-有效
	u8		byPollPortNum; 		//轮询端口数量
	u8		abyPollPort[MAX_POLL_PORT_NUM];	//轮询端口集
	u16		wPollInternal;	//轮询时间间隔 单位秒
}PACKED TMTI_VIDEOENC_POLL_PARAM, *PTMTI_VIDEOENC_POLL_PARAM;

//视频解码参数结构
typedef struct TMTIVideoDecParam
{
	TVideoType tSupportedVideoType;	//支持的视频格式(只读)
	u8		byPlayerBufferNum;		//播放缓冲区数 6-50
	u8		byPlayerOriginBufNum;	//播放起始缓冲区号 0-10
	u8		byVideoType;			//视频解码格式
	u8		byDecDealLev;			//图像后处理级别(0～3级,0表示没有)
}PACKED TMTI_VIDEODEC_PARAM, *PTMTI_VIDEODEC_PARAM;

//视频解码公共参数结构
typedef struct TMTIVideoDecPubParam
{
	u8		byVGAOutSupported;		//是否支持VGA输出(只读) MTI_DISABLE-不支持, MTI_ENABLE-支持
	u8		byVideoOutputType;		//视频输出端口类型 (MTI_VIDEO_OUT_TYPE_C 等)
	u8		byVGAFrequency;			//VGA输出刷新频率 (MTI_VGA_OUT_FREQUENCY_60 等)
	u8		byStarveMode;			//无视频的显示方式 (VSIP_PLAYER_STAEVE_MODE_COLOR_BAR 等)
}PACKED TMTI_VIDEODEC_PUB_PARAM, *PTMTI_VIDEODEC_PUB_PARAM;

//视频解码重传参数结构
typedef struct TMTIVideoDecRetransferParam
{
	u16		wRetransfer1stTimeSpan;//解码第1个重传检测点
	u16		wRetransfer2ndTimeSpan;//解码第2个重传检测点
	u16		wRetransfer3rdTimeSpan;//解码第3个重传检测点
	u16		wRejectTimeSpan;		//解码过期时间
	u8		byRetransferState;		//解码重传状态 MTI_DISABLE-不重传, MTI_ENABLE-重传
}PACKED TMTI_VIDEODEC_RETRANSFER_PARAM, *PTMTI_VIDEODEC_RETRANSFER_PARAM;

//音频编码参数结构
typedef struct TMTIAudioEncParam
{
	TAudioType	tSupportedAudioType;//支持的数据压缩格式(只读)
	u8		byAudioType;			//音频数据格式
	u8      byAudioTypeEx;          //扩展的音频格式
	u8		byAudioInputType;		//音频输入类型(VSIP_AUDIO_INPUTTYPE_LINEIN 等)
	u8		abyAudioInputType[MAX_VIDEOIN_PORT_NAME_LEN];	//支持的输入类型
	u8		byAudioMaxInputType;	//输入类型数量
	u8		byVolume;				//音量 0-25
	u8		byAECState;				//回声抵消状态 MTI_DISABLE-无效, MTI_ENABLE-有效
	u8		byDumbState;			//哑音状态 MTI_DISABLE-无效, MTI_ENABLE-有效

	u32		dwSmpRate;				//采样率
	u8		bySmpBits;				//采样位数
	u8		byChannel;				//声道
	u8		bySample;				//每帧样本数
	u32     dwClockRate;            //时钟频率
	u32     dwBitRate;              //音频码率
}PACKED TMTI_AUDIOENC_PARAM, *PTMTI_AUDIOENC_PARAM;


//音频解码参数结构
typedef struct TMTIAudioDecParam
{
	TAudioType	tSupportedAudioType;//支持的数据压缩格式(只读)
	u8		byVolume;				//音量 0-25
	u8		byMuteState;			//静音状态 MTI_DISABLE-无效, MTI_ENABLE-有效
}PACKED TMTI_AUDIODEC_PARAM, *PTMTI_AUDIODEC_PARAM;

//西安电信告警配置参数
typedef struct TMTIAlarmParam
{
    u8 byDetectEnable;  //检测状态 1-Enable 0-disable
	u8 byRelatedSerialId;//告警采集的串口通道（串口通道能力集）
    u8 byRelatedPinId;  //告警关联的并口告警通道（并口通道能力集）
    u8 byDisplayBannereEnable; //显示横幅 1-Enable 0-disable
    u8 byMaxTemperature; //温度上限（范围0～100度）
	u8 byMinTemperature; //温度下限（范围0～100度）
	u8 byMaxHumidity;    //湿度上限（范围0～100％）
	u8 byMinHumidity;    //湿度下限（范围0～100％）
    u16 wReportInterval; //温湿度上报时间间隔，默认30s（15～300s）
	char achAlarmAlias[MTI_MAX_ALARM_NUM*MTI_MAX_SENSOR_NUM][MTI_MAX_ALARM_ALIAS_LENGTH]; //告警开关别名数组(最大8个，别名长度32个字节，不能为空，默认"告警开关x" )	
	u8 byMaxUnitNum;     //采集单元个数,最大数量MTI_MAX_SENSOR_NUM
	char achUnitName[MTI_MAX_SENSOR_NUM][MTI_MAX_ALARM_ALIAS_LENGTH];  //采集单元的名称
	u8 byPinNum[MTI_MAX_ALARM_NUM*MTI_MAX_SENSOR_NUM];	//采集单元对应的并口号
}PACKED TMTI_ALARM_PARAM,*PTMTI_ALARM_PARAM;

typedef struct TMTIBackBGDColor
{
	u8 RColor;    //R背景
	u8 GColor;    //G背景
	u8 BColor;    //B背景
}PACKED TMTI_BackBGD_COLOR,*PTMTI_BackBGD_COLOR;

typedef struct TMTIAlarmInfo
{
	u16 wPinID;  //modify G.J. 20091020 u8->u16
	u8 byState;
	u16 wAlarmInfoLength;
	char *pAlarmInfoBuf;
}PACKED TMTI_ALARM_INFO,*PTMTI_ALARM_INFO;

typedef struct TMTITransparentInfo
{
    u8 byMessageType;
	u8 byNeedWriteToDB;
	u16 wTransparentInfoLength;
    char transparentInfoBuf[MTI_PU_TRANS_DATA_LEN];
}PACKED TMTI_TRANSPARENT_INFO,*PTMTI_TRANSPARENT_INFO;

typedef struct TMTIBannerParam
{
	u32 dwXPos;//显示区域的X坐标
    u32 dwYPos;//显示区域的Y坐标
    u32 dwWidth;//显示区域的宽
    u32 dwHeight;//显示区域的高
    u32 dwBMPWidth;//BMP图像宽
    u32 dwBMPHeight;//BMP图像高
    u32 dwBannerFontHeight;//字体高度－－主要用于上下滚动时，确定停顿的位置
	u32 dwBannerFontWidth;//字体宽度－－主要用于上下滚动时，确定停顿的位置
    TMTIBackBGDColor tBackBGDColor;//字体颜色
    u32 dwBGDClarity;//具体颜色透明度
    u32 dwPicClarity;//整幅图片透明度
    u32 dwRunMode;//滚动模式：上下or左右or静止（宏定义如上）
    u32 dwRunSpeed;//滚动速度 四个等级（宏定义如上）
    u32 dwRunTimes;//滚动次数 0为无限制滚动
	u32 dwRunForever;//无限制滚动
	u32 dwHaltTime;//停顿时间(秒)，上下滚动时，每滚完一行字的停顿间隔：0始终停留（与静止等同）
	u8  byAlignStyle;//对齐方式
	u8 byRelatedPinId;//关联的并口id
	u8  byCharBkColor;//字体背景颜色
}PACKED TMTI_BANNER_PARAM,*PTMTI_BANNER_PARAM;

//串口参数结构
typedef struct TMTISerialPortParam
{
	u32		dwBaudRate;				//串口波特率
	u8		byComPortType;			//串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
	TSerialType tSupportedType;		//支持的串口类型 (只读)
}PACKED TMTI_SERIALPORT_PARAM, *PTMTI_SERIALPORT_PARAM;

//串口公共参数结构
typedef struct TMTISerialPortPubParam
{
	u8		byInitStreamState;		//串口数据收发初始化状态 MTI_DISABLE-无效, MTI_ENABLE-有效
	u16		wStreamLocalPort;		//串口数据本地接收端口
	u32		dwStreamDestIpAddr;		//串口数据对端地址
	u16		wStreamDestPort;		//串口数据对端端口
	TConnType	tSupportedConnType;	//支持的码流链接类型
	u8		byConnectionType;		//码流链接类型
	u8		byProtocolType;			//协议类型 0-透明通道, 1-VSIP协议
	u8      bySerialId;             //对端的串口id
	u8		byDecCtrlEncEnable;		//解码器控制编码器使能
	u8		byDecCtrlChanId;		//解码器通道号
}PACKED TMTI_SERIALPORT_PUB_PARAM, *PTMTI_SERIALPORT_PUB_PARAM;

//并口输入参数结构
typedef struct TMTIInputPinParam
{
	u8		byAlarmDetectState;		//告警检测状态 0-不检测, 1-只检测一个周期, 2-每周循环检测, 3-始终检测
	u8		byAlarmInitState;		//定义告警和常态可选 zlq add
	TMTISysTime atAlarmDetectStartTime[DAYS_OF_WEEK]; //告警检测开始时间
	TMTISysTime atAlarmDetectDuration[DAYS_OF_WEEK];  //告警检测持续时间
	u8		byRelateEncId;			//告警关联的编码通道 (无关联编码通道则为 MTI_RELATE_ENCID_DISABLE)
	u8		byOutputPinID;			//告警自动控制输出ID (无控制输出则为 MTI_OUTPUT_DISABLE)
	u8		byAlarmOSDState;		//告警字幕状态 MTI_DISABLE-不显示, MTI_ENABLE-显示
	u16		wAlarmOSDLeftMargin;	//告警字幕左边距 1-200
	u16		wAlarmOSDTopMargin;		//告警字幕上边距 1-200
	char	achAlarmOSDName[MAX_STRING_LEN+1];//告警字幕内容
	u32		dwAlarmOSDColor;		//告警字幕颜色(RGB)
	u8		byEthAlarmState;			//是否发送ETH1网口告警
}PACKED TMTI_INPUTPIN_PARAM, *PTMTI_INPUTPIN_PARAM;

//并口输入状态结构 20091020
typedef struct TMTIInputPinState
{
	u8 adwInputPinState[128]; //适用于NVR64路设备每个通道对应一位

}PACKED TMTI_INPUTPIN_STATE, *PTMTI_INPUTPIN_STATE;

//视频源输入端口参数结构
typedef struct TMTIVideoInputParam
{
	u8		byCameraType;			//摄像头类型
	u8		byCameraID;				//摄像头地址码
	u8		bySerialPortID;			//对应的串口ID
	char	achVideoPortName[MAX_VIDEOIN_PORT_NAME_LEN+1];	//视频源端口名
	char	achVideoSrcName[MAX_VIDEOIN_PORT_LONGNAME_LEN+1];	//视频源名
	char	achVideoPortOSD[MAX_STRING_LEN+1];	//视频源字幕内容
	u16		wVideoPortOSDLeftMargin;	//视频源字幕左边距 1-200
	u16		wVideoPortOSDTopMargin;		//视频源字幕上边距 1-200
	u32		dwVideoPortOSDColor;		//视频源字幕颜色
}PACKED TMTI_VIDEO_INPUT_PARAM, *PTMTI_VIDEO_INPUT_PARAM;

//第三方解码add 20100317
typedef struct TMTIThirdPartyDecParam
{
	char achThirdPartyDecParam[MAX_THIRDPARTY_NAME_LEN+1];

}PACKED TMTI_THIRDPARTY_DEC_PARAM, *PTMTI_THIRDPARTY_DEC_PARAM;

//移动侦测区域结构体
typedef struct TMTIMotionDetectParam
{
	u8   byDetectEnable;			//侦测使能 MTI_ENABLE-有效 MTI_DISABLE-无效
	u32  dwXPos;					//侦测区域顶点横坐标
	u32  dwYPos;					//侦测区域顶点纵坐标
	u32  dwWidth;					//侦测区域宽度
	u32  dwHeigth;					//侦测区域高度
	u8   byAlarmRate;				//侦测区域告警产生百分比（大于此临界值告警,0-100）
	u8   byResumeRate;				//产生告警后运动范围百分比小于该值则恢复告警(该值小于byAlarmRate,0-100)
}PACKED TMTI_MOTION_DETECT_PARAM, *PTMTI_MOTION_DETECT_PARAM;

//add by Royan 20130201
typedef struct TMTIMotionDetectParamList
{
	u8                     byMotionDetectCount;	              		  //移动侦测数
	TMTIMotionDetectParam  atMtiMotionDetectParam[MAX_DET_AREA_NUM];  //移动侦测参数列表
}PACKED TMTI_MOTION_DETECT_PARAM_LIST, *PTMTI_MOTION_DETECT_PARAM_LIST;


//图像遮蔽控制结构体
typedef struct TMTIShadeParam
{
	u8   byShadeEnable;			//遮蔽使能 MTI_ENABLE-有效 MTI_DISABLE-无效
	u32  dwXPos;				//遮蔽区域顶点横坐标
	u32  dwYPos;				//遮蔽区域顶点纵坐标
	u32  dwWidth;				//遮蔽区域宽度
	u32  dwHeigth;				//遮蔽区域高度
}PACKED TMTI_SHADE_PARAM, *PTMTI_SHADE_PARAM;

//add by Royan 20130201
typedef struct TMTIShadeParamList
{
	u8              byShadeCount;		                	//图像遮蔽数
	TMTIShadeParam  atMtiShadeParam[MAX_SHADE_AREA_NUM];    //图像遮蔽参数列表
}PACKED TMTI_SHADE_PARAM_LIST, *PTMTI_SHADE_PARAM_LIST;

typedef struct TMTIMediaCapability
{
	u8 byVidEncNum; 				//视频编码通道数量
	u8 byVidDecNum; 				//视频解码通道数量
	u8 byAudEncNum; 				//音频编码通道数量
	u8 byAudDecNum; 				//音频解码通道数量
	u8 byVidAdpterNum; 				//视频转压通道数量
	u8 byVidCompNum; 				//图像合成通道数
	u8 byVidSrcPollNum;				//视频源轮询通道数量
	u8 byAudMixNum;					//音频混音通道数量
	u8 byOsdColorNum;				//台标字幕支持的颜色数
	u8 byShadeAreaNum;				//图像遮蔽区域数量
	u8 byMaxStreamSendNum;			//单个通道最大发送能力(一个通道包括视频和音频)
	u8 byMaxStreanDirSendNum;		//单个通道的最大直传路数(一个通道包括视频和音频)
	u8 byStreamEncrypt;				//是否码流加密
	u8 byMotionDetectAreaNum;		//移动侦测区域数量
	u8 bySnapShotChanNum;			//抓拍通道数
    u8 byCBRVBR;				    // 是否能够设置图像质量,1－支持，0－不支持
    u8 byMaxEncBitRate;				// 最大有效码率             
#ifdef VSIP_MULTI_CHAN_SUPPORT
    u8 bySupportMultiChan;		    //是否支持双流，1－支持，0－不支持
#endif
    u8 byMatrix;                            // 是否支持矩阵接入
    u8 byVideoEncSlaveChannelNum;   //视频编码辅流通道数 20090509
}PACKED TMTI_MEDIACAPABILITY, *PTMTI_MEDIACAPABILITY;

typedef struct TMTIRecCapability
{
	u8 byRecorderNum;				//录像通道数量
	u8 byPlayerNum;					//放像通道数量
	u8 byFileDownloadNum;			//录像文件下载通道数量
}PACKED TMTI_RECCAPABILITY, *PTMTI_RECCAPABILITY;

typedef struct TMTIDevCapability
{
	u8 byVGANum;					//视频输出VGA接口数量
	u8 byVidOutNum;					//视频输出接口数量
	u8 byVideoInputPortNum;			//视频源输入数量
	u8 bySerialPortNum;				//串口数量
	u16	wInputPinNum;			    //并口输入通道数量  modify G.J. 20091020 u8->u16
	u8 byOutputPinNum;				//并口输出通道数量
	u8 byPTZCtrlNum;				//可控制云台数量
	u8 byEtherNetnum;				//以太网口数量
	u8 byWirelessNum;				//无线网口数量
	u8 byXiAnTeleNum;				//西安电信告警数量
	u8 byPlatFormType;				//接入平台类型
	u8 byDevPowerType;				//供电类型
}PACKED TMTI_DEVCAPABILITY, *PTMTI_DEVCAPABILITY;

//设备能力集
typedef struct TMTIPUCapability
{
	TMTIMediaCapability		tMediaCap;								//多媒体能力集
	TMTIDevCapability		tDevCap;								//设备硬件能力集
	TMTIRecCapability		tRecCap;								//录放像能力集
}PACKED TMTI_PUCAPABILITY, *PTMTI_PUCAPABILITY;

// 平台侧基本能力集
typedef struct TMTICapability
{
	u8		byVideoEncNum;			//视频编码通道数量
	u8		byVideoDecNum;			//视频解码通道数量
	u8		byAudioEncNum;			//音频编码通道数量
	u8		byAudioDecNum;			//音频解码通道数量
	u8		bySerialPortNum;		//串口数量
	u16		wInputPinNum;			//并口输入数量  //modify G.J. 20091020 u8->u16
	u8		byOutputPinNum;			//并口输出数量
	u8		byVideoInputPortNum;	//视频输入端口数量
}PACKED TMTI_CAPABILITY,*pTMTI_CAPABILITY;

// 能力子项
typedef struct TMTICapSetItem
{
	u8 byCapItemId;		// 能力集类型ID
	u16 byCapItemValue;	    // 能力集类型值

	u8 GetCapItemId()
	{
		return byCapItemId;
	}

	void SetCapItemId(u8 byID)
	{
		byCapItemId = byID;
	}
	u16 GetCapItemValue()
	{
		return ntohs(byCapItemValue);
	}

	void SetCapItemValue(u16 wValue)
	{
		byCapItemValue = htons(wValue);
	}
}PACKED TMTICAPSETITEM;

//扩展能力集
typedef struct TMTICapSet
{
	u8				byCapItemNum;
	TMTICapSetItem	atCustomCap[MAX_CUSTOMCAP_NUM];
	//通过索引获取能力集类型
	u8 GetCapType(u8 byIndex)
	{
		if (byIndex>byCapItemNum)
		{
			return VSIP_INVALID_CAP_TYPE;
		}
		return atCustomCap[byIndex].byCapItemId;
	}
	//通过能力集类型获取能力集取值
	u16 GetCapValue(u8 byCapType)
	{
		u8 i = 0;
		while (i < byCapItemNum && atCustomCap[i].byCapItemId != byCapType)
		{
			i++;
		}
		if (i >= byCapItemNum)
		{
			return VSIP_INVALID_CAP_VALUE;
		}
		return atCustomCap[i].byCapItemValue;
	}

}PACKED TMTICAPSET;

//终端操作结果回复
typedef struct TMTIOPResultResp
{
	u16		wTransactionNum;	//操作流水号
	u16		wErrCode;			//操作结果(MTI_SUCCESS, MTI_FAIL)

	void SwapToNetwork()
	{
        wTransactionNum = htons(wTransactionNum);
		wErrCode = htons(wErrCode);
	}

	void SwapFromNetwork()
	{
		wTransactionNum = ntohs(wTransactionNum);
		wErrCode = ntohs(wErrCode);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wTransactionNum, sizeof(wTransactionNum));
		Buf += sizeof(wTransactionNum);
		memcpy( Buf, &wErrCode, sizeof(wErrCode));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(  &wTransactionNum, Buf, sizeof(wTransactionNum));
		Buf += sizeof(wTransactionNum);
		memcpy( &wErrCode, Buf, sizeof(wErrCode));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(wTransactionNum)+sizeof(wErrCode));
	}

}PACKED TMTI_OP_RESULT_RESP, *PTMTI_OP_RESULT_RESP;

typedef struct TMTIMediaSwitchAddr
{
	 u32 dwDestIpAddr;
	 u16 wDestRtpPort;
	 u16 wLocalPort;
	 u16 wDestRtcpPort;
	 u8  byConnectType;
	 u32 dwSSRC;
	 u16 wSrcRtpPort;
	 u16 wSrcRtcpPort;

	 void SwapToNetwork()
	 {
		 wDestRtpPort = htons(wDestRtpPort);
		 wDestRtcpPort = htons(wDestRtcpPort);
		 wLocalPort = htons(wLocalPort);
		 dwSSRC = htonl(dwSSRC);
		 wSrcRtpPort = htons(wSrcRtpPort);
		 wSrcRtcpPort = htons(wSrcRtcpPort);
	 }

	 void SwapFromNetwork()
	 {
		 wDestRtpPort = ntohs(wDestRtpPort);
		 wDestRtcpPort = ntohs(wDestRtcpPort);
		 wLocalPort = ntohs(wLocalPort);
		 dwSSRC = ntohl(dwSSRC);
		 wSrcRtpPort = ntohs(wSrcRtpPort);
		 wSrcRtcpPort = ntohs(wSrcRtcpPort);
	 }

	 void PackedToBuf(char* Buf)
	 {
		 memcpy(Buf,&dwDestIpAddr,sizeof(dwDestIpAddr));
		 Buf += sizeof(dwDestIpAddr);
		 memcpy(Buf,&wDestRtpPort,sizeof(wDestRtpPort));
		 Buf += sizeof(wDestRtpPort);
		 memcpy(Buf,&wDestRtpPort,sizeof(wDestRtpPort));   //本地端口，现不用，编码为目标协议端口
		 Buf += sizeof(wDestRtpPort);
		 memcpy(Buf,&byConnectType,sizeof(byConnectType));
		 Buf += sizeof(byConnectType);
		 memcpy(Buf,&dwSSRC,sizeof(dwSSRC));
		 Buf += sizeof(dwSSRC);
		 memcpy(Buf,&wSrcRtpPort,sizeof(wSrcRtpPort));
		 Buf += sizeof(wSrcRtpPort);
		 memcpy(Buf,&wSrcRtcpPort,sizeof(wSrcRtcpPort));
		 Buf += sizeof(wSrcRtcpPort);
		 memcpy(Buf,&wDestRtcpPort,sizeof(wDestRtcpPort));
	 }
}PACKED TMTI_MEDIA_SWITCH_ADDR,*PTMTI_MEDIA_SWITCH_ADDR;

typedef struct TFileTransInfo
{
	u16 wSegNumber;
	u8 byMessageType;
	u16 wLen;
	char abyBuf[SMALL_DATA_BUFSIZE];
}PACKED TMTI_FILE_TRANS_INFO,*PTMTI_FILE_TRANS_INFO;

typedef struct TFileTransRspInfo
{
	u16 wResultCode;
	u8 byOsdType;
}PACKED TFILE_TRANS_RSP_INFO,*PTFILE_TRANS_RSP_INFO;

//录象文件个数回复
typedef struct TQueryRecFileNum
{
	u16		wTransactionNum;	//操作流水号
	u16		wNum;			   //文件个数
	unsigned short PackedLen()
	{
		return (sizeof(wTransactionNum)+sizeof(wNum));
	}
}PACKED TQueryRecFileNum, *PTQueryRecFileNum;
//设置终端参数
typedef struct TMTISetConfig
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16 
	u8		byConfigType;	//结构体类型
	void *	pConfigData;	//结构体指针
}PACKED TMTI_SET_CONFIG, *PTMTI_SET_CONFIG;

//设置终端移动侦测参数
typedef struct TMTISetMotDetCfg
{
	u16     wAreaNum;		//区域数目
	void *	pConfigData;	//移动侦测参数结构体数组
}PACKED TMTI_SET_MOT_DET_CONFIG, *PTMTI_SET_MOT_DET_CONFIG;

//设置终端图象屏蔽参数
typedef struct TMTISetShadeCfg
{
	u16     wShadeAreaNum;		//区域数目
	void *	pConfigData;	//移动侦测参数结构体数组
}PACKED TMTI_SET_SHADE_CONFIG, *PTMTI_SET_SHADE_CONFIG;


//终端码流控制
typedef struct TMTIMediaCtrl
{
	u16		wEntityID;		//通道ID   modify G.J. 20091020 u8->u16
	u8		byStreamType;	//码流传输类型
	u16		wStreamPort;	//码流传输端口
	u32		dwStreamIpAddr;	//码流传输地址
}PACKED TMTI_MEDIA_CTRL, *PTMTI_MEDIA_CTRL;


//扩展终端码流控制(解码器)
typedef struct TMTIMediaCtrlEX
{
	u16		wEntityID;		              //通道ID modify G.J. 20091020 u8->u16 
	u8      byMediaCtrlType;              //video--MTI_VIDEO_TYPE,audio-- MTI_AUDIO_TYPE
	u8		byStreamType;	              //码流传输类型MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP
	u16		wLocalAcceptStreamPort;       //本地码流接收端口
	u32		dwLocalAcceptStreamIpAddr;	  //本地码流接收地址
   	u16		wRTCPStreamPort;              //对端的RTCP端口
	u32		dwRTCPStreamIpAddr;	          //对端的RTCP端口地址
}PACKED TMTI_MEDIA_CTRLEX, *PTMTI_MEDIA_CTRLEX;

//串口数据
typedef struct TMTISerialPortData
{
	u16		wEntityID;		    //串口号 modify G.J. 20091020 u8->u16 
	u16		wDataBufLen;		//数据长度
	u8 *	pbyDataBuf;			//数据指针
}PACKED TMTI_SERIAL_PORT_DATA, *PTMTI_SERIAL_PORT_DATA;

//摄像头控制数据
typedef struct TMTICameraCtrlData
{
	u16		wEntityID;		    //视频输入端口号 modify G.J. 20091020 u8->u16 
	u16		wDataBufLen;		//数据长度
	u8 *	pbyDataBuf;			//数据指针
}PACKED TMTI_CAMERA_CTRL_DATA, *PTMTI_CAMERA_CTRL_DATA;

//参数更新通知信息
typedef struct TMTIConfigInfo
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16 
	u8		byConfigType;	//参数类型
}PACKED TMTI_CONFIG_INFO, *PTMTI_CONFIG_INFO;

//透明数据应答
typedef struct TMTITransDataRsp
{
	u16     wMsgType;		    //消息类型
	u8      bIsHaveNtf;         //是否含有Ntf
	u32     dwReserved;         //保留字段
	u16	    wDataBufLen;	    //数据长度
	u8      abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //透明数据结构

	void SwapToNetwork()
	{
		wMsgType = htons(wMsgType);
		wDataBufLen = htons(wDataBufLen);
		dwReserved = htonl(dwReserved);
	}

	void SwapFromNetwork()
	{
		wMsgType = ntohs(wMsgType);
		wDataBufLen = ntohs(wDataBufLen);
		dwReserved = ntohl(dwReserved);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( Buf, &wMsgType, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( Buf, &bIsHaveNtf, sizeof(bIsHaveNtf));
		Buf += sizeof(bIsHaveNtf);
		memcpy(Buf,&dwReserved,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( Buf, &wDataBufLen, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( Buf, abyTransDataBuf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( &wMsgType, Buf, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( &bIsHaveNtf, Buf, sizeof(bIsHaveNtf));
		Buf += sizeof(bIsHaveNtf);
		memcpy(&dwReserved,Buf,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( &wDataBufLen, Buf, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( abyTransDataBuf, Buf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	unsigned short PackedLen() const
	{
		return (sizeof(bIsHaveNtf)+sizeof(wMsgType)+sizeof(dwReserved)+sizeof(wDataBufLen)+sizeof(abyTransDataBuf));
	}
}PACKED TMTI_TRANS_DATA_RSP, *PTMTI_TRANS_DATA_RSP;


typedef struct TMTIStreamProp
{
	u16 wStreamPropId;
	u8  byDataType;
	u16 wValueLength;
	u8  abyValue[MTI_MAX_STREAM_PROP_LENGTH];

	TMTIStreamProp()
	{
		memset(abyValue,0,sizeof(abyValue));
	}

	void SwapToNetwork()
	{
		wStreamPropId = htons(wStreamPropId);
		wValueLength = htons(wValueLength);
	}

	void SwapFromNetwork()
	{
		wStreamPropId = ntohs(wStreamPropId);
		wValueLength = ntohs(wValueLength);
	}

	u16 UnpackedFromBuf(char * Buf, unsigned short BufLen)
	{
		u16 wBufLen = 0;

		memcpy( &wStreamPropId ,Buf, sizeof(wStreamPropId));
		wBufLen += sizeof(wStreamPropId);

		memcpy(&byDataType,Buf+wBufLen,sizeof(byDataType));
		wBufLen += sizeof(byDataType);

		memcpy(&wValueLength,Buf+wBufLen,sizeof(wValueLength));
		wBufLen += sizeof(wValueLength);

		switch(byDataType)
		{
		case VSIP_VALUE_TYPE_CHAR:
			{
				memcpy(abyValue, Buf+wBufLen, sizeof(char));
				wBufLen += sizeof(char);
			}
			break;

		case VSIP_VALUE_TYPE_SHORT:
			{	
				memcpy( abyValue, Buf+wBufLen, sizeof(short));
				wBufLen += sizeof(short);
			}
			break;

		case VSIP_VALUE_TYPE_INT:
			{
				memcpy( abyValue, Buf+wBufLen, sizeof(int));
				wBufLen += sizeof(int);
			}
			break;

		case VSIP_VALUE_TYPE_UINT:
			{
				memcpy(abyValue, Buf+wBufLen, sizeof(u32));
				wBufLen += sizeof(u32);
			}
			break;
		case VSIP_VALUE_TYPE_STRING:
		default:
			{
				memcpy(abyValue,Buf+wBufLen,wValueLength);
				wBufLen += wValueLength;
			}
			break;
		}
		return wBufLen;
	}

	u16 PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 wBufLen = 0;

		memcpy(Buf+wBufLen,&wStreamPropId,sizeof(wStreamPropId));
		wBufLen += sizeof(wStreamPropId);

		memcpy(Buf+wBufLen,&byDataType,sizeof(byDataType));
		wBufLen += sizeof(byDataType);

		memcpy( Buf+wBufLen, &wValueLength, sizeof(wValueLength));
		wBufLen += sizeof(wValueLength);

		switch(byDataType)
		{
		case VSIP_VALUE_TYPE_CHAR:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(char));
				wBufLen += sizeof(char);
			}
			break;

		case VSIP_VALUE_TYPE_SHORT:
			{	
				memcpy( Buf+wBufLen, abyValue, sizeof(short));
				wBufLen += sizeof(short);
			}
			break;

		case VSIP_VALUE_TYPE_INT:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(int));
				wBufLen += sizeof(int);
			}
			break;

		case VSIP_VALUE_TYPE_UINT:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(u32));
				wBufLen += sizeof(u32);
			}
			break;
		case VSIP_VALUE_TYPE_STRING:
		default:
			{
				memcpy( Buf+wBufLen, abyValue, sizeof(wValueLength));
				wBufLen += wValueLength;
			}
			break;
		}
		return wBufLen;
	}

}PACKED TMTI_STREAM_PROP,*PTMTI_STREAM_PROP;

typedef struct TMTIStreamPropList
{
	u16 wErrCode;			//操作结果(MTI_SUCCESS, MTI_FAIL)
	u16 wStreamPropNum;
	TMTIStreamProp tStreamProp[MAX_STREAMPROP_NUM];

	void SwapToNetwork()
	{
		for (int i=0;i<wStreamPropNum;i++)
		{
			tStreamProp[i].SwapToNetwork();
		}
		wStreamPropNum = htons(wStreamPropNum);
	}

	void SwapFromNetwork()
	{
		for (int i=0;i<wStreamPropNum;i++)
		{
			tStreamProp[i].SwapFromNetwork();
		}
		wStreamPropNum = ntohs(wStreamPropNum);
	}

	u16 UnpackedFromBuf(char * Buf, unsigned short BufLen)
	{
		u16 wBufLen;
		for (int i=0;i<wStreamPropNum;i++)
		{
			wBufLen += tStreamProp[i].UnpackedFromBuf(Buf+wBufLen,BufLen);
		}
		return wBufLen;
	}

	u16 PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 wBufLen;
		for (int i=0;i<wStreamPropNum;i++)
		{
			wBufLen += tStreamProp[i].PackedToBuf(Buf+wBufLen,BufLen);
		}
		return wBufLen;
	}
}PACKED TMTI_STREAM_PROPLIST,*PTMTI_STREAM_PROPLIST;


typedef struct TMTIVideoAttribute
{
    u32     dwMediaType;      // 编码格式 VSIP_VIDEO_TYPE_, 
    u32     dwClockRate;      // 采样率 1000.90000，
    u16     wWidth;           // 宽
    u16     wHeigth;          // 高
    u8      byFramerate;      // 帧率
    u8      byStreamType;     // 1:固定码率，2:可变码率
    u32     dwBitRate;        // 码率
    u32     dwParam1;         //扩展参数
    u32     dwParam2;

	void SwapToNetwork()
	{
		dwMediaType = htonl(dwMediaType);
		dwClockRate = htonl(dwClockRate);
		wWidth = htons(wWidth);
		wHeigth = htons(wHeigth);
		dwBitRate = htonl(dwBitRate);
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		dwMediaType = ntohl(dwMediaType);
		dwClockRate = ntohl(dwClockRate);
		wWidth = ntohs(wWidth);
		wHeigth = ntohs(wHeigth);
		dwBitRate = ntohl(dwBitRate);
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}
    TMTIVideoAttribute()
    {
        memset(this, 0 , sizeof(TMTIVideoAttribute));
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\t\t视频编码参数------\n");
        OspPrintf(TRUE, FALSE, "\t\t编码格式:%d\n", dwMediaType);
        OspPrintf(TRUE, FALSE, "\t\t采样率:%d\n", dwClockRate);
        OspPrintf(TRUE, FALSE, "\t\t宽:%d\n", wWidth);
        OspPrintf(TRUE, FALSE, "\t\t高:%d\n", wHeigth);
        OspPrintf(TRUE, FALSE, "\t\t帧率:%d\n", byFramerate);
        OspPrintf(TRUE, FALSE, "\t\t1:固定码率，2:可变码率:%d\n", byStreamType);
        OspPrintf(TRUE, FALSE, "\t\t码率:%d\n", dwBitRate);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);
    }
}PACKED TMTI_VIDEO_ATTRI,*PTMTI_VIDEO_ATTRI;

typedef struct TMTIAudioAttribute
{  
    u32     dwAudioFormat;        // 编码格式 VISP_AUDIO_FORMAT_， 
    u32     dwSamplingRate;       // 采样率 160000,44100 
    u8      byChannels;           // 声道数 1=单声道，2=双声道
    u32     dwClockRate;          // 时钟频率 与m_dwSamplingRate相比，多了1000(代表1000ms的情况)
    u32     dwBitRate;            // 码率 48000表示48kps

/*   u8      m_bySamplingBits;       // 采样位数 8表示位宽为8，如此类推
    u32     m_dwSaples;             // 每帧样本数 160表示每帧包含160样本数
    u8      m_byFramHeadFlag;       // 仅AAC_LC使用，0=无，1=有
 */  
    u32     dwParam1;
    u32     dwParam2;

	void SwapToNetwork()
	{
		dwAudioFormat = htonl(dwAudioFormat);
		dwSamplingRate = htonl(dwSamplingRate);
		dwClockRate = htonl(dwClockRate);
		dwBitRate = htonl(dwBitRate);
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		dwAudioFormat = ntohl(dwAudioFormat);
		dwSamplingRate = ntohl(dwSamplingRate);
		dwClockRate = ntohl(dwClockRate);
		dwBitRate = ntohl(dwBitRate);
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}

    TMTIAudioAttribute()
    {
        memset(this, 0 , sizeof(TMTIAudioAttribute));
    }

    void PrintData()
    {
        OspPrintf(TRUE, FALSE, "\t\t音频编码参数------\n");
        OspPrintf(TRUE, FALSE, "\t\t编码格式:%d\n", dwAudioFormat);
        OspPrintf(TRUE, FALSE, "\t\t采样率:%d\n", dwSamplingRate);
        OspPrintf(TRUE, FALSE, "\t\t声道数:%d\n", byChannels);
        OspPrintf(TRUE, FALSE, "\t\t时钟频率:%d\n", dwClockRate);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);

    }
}PACKED TMTI_AUDIO_ATTRI,*PTMTI_AUDIO_ATTRI;

enum
{
    // 视频编码格式
    VIDEO_FORMAT=100,           //u32 同VSIP_VIDEO_TYPE_* 采样率 
    VIDEO_CLOCK_RATE=101,       // u32 比如，1000、90000等 宽 
    VIDEO_WIDTH=102,            // u16 单位为像素 高 
    VIDEO_HEIGHT=103,           //u16 单位为像素 帧率 
    VIDEO_FRAMERATE=104,        // u8 单位为fps 码率类型 
    VIDEO_BITRATETYPE=105,      //u8 1：固定码率（CBR）； 2：可变码率（VBR） 码率 
    VIDEO_BITRATE=106,          // u32 单位为Kbps

    //音频编码格式 
    AUDIO_FORMAT=200,           // u32 同VSIP_AUDIO_FORMAT_* 真实采样率 
    AUDIO_SAMPLING_RATE=201,    //u32 比如，16000、44100等 声道数 
    AUDIO_CHANNELS=202,         // u8 比如，1=单声道，2=双声道 时间戳采样率 
    AUDIO_CLOCK_RATE=203,       //u32 比如，与AUDIO_SAMPLING_RATE相比，取值范围多了1000（代表1000ms的情况） 码率 
    AUDIO_BITRATE=204,          //u32  单位为KBPS
};

typedef struct TMTIStreamAttribute
{
    enum AttributeType
    {
        Nty_Type_None = 0,	    // 两者都无
        Nty_Type_Video,     // 视频属性
        Nty_Type_Audio,		// 音频属性
        Nty_Type_Both,	    // 两者都有
    };
    AttributeType   eType;    // 属性类型
    TMTIVideoAttribute tMediaAttr;   // 视频属性
    TMTIAudioAttribute tAudioAttr;   // 音频属性
    s8              szManufacturer[MAXLEN_DEVICE_MANUFACTURER + 1]; //设备厂商信息
    u32             dwParam1;         //扩展字段1；
    u32             dwParam2;         //扩展字段2；

    TMTIStreamAttribute()
    {
        memset(this, 0, sizeof(TMTIStreamAttribute));
    }
	void SwapToNetwork()
	{
		tMediaAttr.SwapToNetwork();
		tAudioAttr.SwapToNetwork();
		dwParam1 = htonl(dwParam1);
		dwParam2 = htonl(dwParam2);
	}
	void SwapFromNetwork()
	{
		tMediaAttr.SwapFromNetwork();
		tAudioAttr.SwapFromNetwork();
		dwParam1 = ntohl(dwParam1);
		dwParam2 = ntohl(dwParam2);
	}
    void PrintData() 
    {
        if (Nty_Type_None == eType)
        {
            OspPrintf(TRUE, FALSE, "\t\t该通道未获取RTP流属性\n");
            return;
        }
        OspPrintf(TRUE, FALSE, "\t\t属性类型:%d(0-都无，1-视频，2-音频，3-两者都有)\n", eType);
        OspPrintf(TRUE, FALSE, "\t\t厂商:%s\n", szManufacturer);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam1:%d\n", dwParam1);
        OspPrintf(TRUE, FALSE, "\t\tm_dwParam2:%d\n", dwParam2);
        tMediaAttr.PrintData();
        tAudioAttr.PrintData();
    }
    u32 PackedToBuf(s8* pszBuff)
    {
        // 此处注意pszBuff长度是否会有安全隐患
        s16 wNum = 0;
        s16 wTypeID = 0;
        s8  szType = 0;
        s16 wDataLeng = 0;
        s16 wTotalLen = 0;

        if (NULL == pszBuff || Nty_Type_None == eType)
        {
            return wTotalLen;
        }

        //视频编码参数
        /*u32     m_dwMediaType;      // 编码格式 VSIP_VIDEO_TYPE_, 
        u32     m_dwClockRate;      // 采样率 1000.90000，
        u16     m_wWidth;           // 宽
        u16     m_wHeigth;          // 高
        u8      m_byFramerate;      // 帧率
        u8      m_byStreamType;     // 1:固定码率，2:可变码率
    u32     m_dwBitRate;        // 码率

  // 视频编码格式
  VIDEO_FORMAT=100,           //u32 同VSIP_VIDEO_TYPE_* 采样率 
  VIDEO_CLOCK_RATE=101,       // u32 比如，1000、90000等 宽 
  VIDEO_WIDTH=102,            // u16 单位为像素 高 
  VIDEO_HEIGHT=103,           //u16 单位为像素 帧率 
  VIDEO_FRAMERATE=104,        // u8 单位为fps 码率类型 
  VIDEO_BITRATETYPE=105,      //u8 1：固定码率（CBR）； 2：可变码率（VBR） 码率 
    VIDEO_BITRATE=106,          // u32 单位为Kbps*/

        /*流属性数量n(2)+n*(流属性id(2)+数据类型(1)+数据长度(2)+流属性数据)*/

        memcpy(pszBuff, &wNum, sizeof(s16)); //先填充个数，最后再修改
        s8* pszTmp = pszBuff; //保存BUFF的初始地址
        wTotalLen += sizeof(s16);

        if (0 != tMediaAttr.dwMediaType)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_FORMAT; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwMediaType);

			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);

            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);

			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);

            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwMediaType, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tMediaAttr.dwClockRate)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_CLOCK_RATE; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwClockRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwClockRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tMediaAttr.wWidth)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_WIDTH; //属性ID
            szType = VSIP_VALUE_TYPE_SHORT; 
            wDataLeng = sizeof(tMediaAttr.wWidth);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.wWidth, sizeof(u16));
            wTotalLen += sizeof(u16);
        }

        if (0 != tMediaAttr.wHeigth)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_HEIGHT; //属性ID
            szType = VSIP_VALUE_TYPE_SHORT;
            wDataLeng = sizeof(tMediaAttr.wHeigth);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.wHeigth, sizeof(u16));
            wTotalLen += sizeof(u16);
        }

        if (0 != tMediaAttr.byFramerate)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_FRAMERATE; //属性ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tMediaAttr.byFramerate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.byFramerate, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tMediaAttr.byStreamType)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_BITRATETYPE; //属性ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tMediaAttr.byStreamType);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.byStreamType, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tMediaAttr.dwBitRate)
        {
            wNum++; // 属性个数
            wTypeID = VIDEO_BITRATE; //属性ID
            szType =VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tMediaAttr.dwBitRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tMediaAttr.dwBitRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        /////////////////////////音频参数////////////////////////////////////////////////
        if (0 != tAudioAttr.dwSamplingRate)
        {
            wNum++; // 属性个数
            wTypeID = AUDIO_SAMPLING_RATE; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwSamplingRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwSamplingRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.dwAudioFormat)
        {
            wNum++; // 属性个数
            wTypeID = AUDIO_FORMAT; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwAudioFormat);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwAudioFormat, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.byChannels)
        {
            wNum++; // 属性个数
            wTypeID = AUDIO_CHANNELS; //属性ID
            szType = VSIP_VALUE_TYPE_CHAR;
            wDataLeng = sizeof(tAudioAttr.byChannels);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.byChannels, sizeof(u8));
            wTotalLen += sizeof(u8);
        }

        if (0 != tAudioAttr.dwClockRate)
        {
            wNum++; // 属性个数
            wTypeID = AUDIO_CLOCK_RATE; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwClockRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwClockRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

        if (0 != tAudioAttr.dwBitRate)
        {
            wNum++; // 属性个数
            wTypeID = AUDIO_BITRATE; //属性ID
            szType = VSIP_VALUE_TYPE_UINT;
            wDataLeng = sizeof(tAudioAttr.dwBitRate);
            
			wTypeID = htons(wTypeID);
            memcpy(pszBuff+wTotalLen, &wTypeID, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &szType, sizeof(s8));
            wTotalLen += sizeof(s8);
            
			wDataLeng = htons(wDataLeng);
            memcpy(pszBuff+wTotalLen, &wDataLeng, sizeof(s16));
            wTotalLen += sizeof(s16);
            
            memcpy(pszBuff+wTotalLen, &tAudioAttr.dwBitRate, sizeof(u32));
            wTotalLen += sizeof(u32);
        }

		wNum = htons(wNum);
        memcpy(pszTmp, &wNum, sizeof(s16)); //先填充个数，最后再修改
        return wTotalLen;
    }

    BOOL32 UnpackedFromBuf(char* pszMsg, const u32& dwMsgLen )
    {
        s16 wNum = 0;
        s16 wTypeID = 0;
        s8  szType = 0;
        s16 wDataLeng = 0;
        s8 szData[sizeof(TMTIStreamAttribute)] = {0};
        s16 wTotalLen = 0;

        char* pszTmp = pszMsg;
        memcpy(&wNum, pszTmp, sizeof(s16));
        wNum = ntohs(wNum);

        pszTmp += sizeof(s16);
        wTotalLen += sizeof(s16);

        BOOL32 bIsMedia = FALSE;
        BOOL32 bIsAudio = FALSE;

        while((u32)wTotalLen < dwMsgLen)
        {
            memcpy(&wTypeID, pszTmp, sizeof(s16));
			wTypeID = ntohs(wTypeID);
            pszTmp += sizeof(s16);
            wTotalLen += sizeof(s16);

            pszTmp += sizeof(s8);
            wTotalLen += sizeof(s8);

            memcpy(&wDataLeng, pszTmp, sizeof(s16));
			wDataLeng = ntohs(wDataLeng);

            pszTmp += sizeof(s16);
            wTotalLen += sizeof(s16);

            if (wDataLeng > sizeof(TMTIStreamAttribute))
            {
                return FALSE;
            }

            memcpy(&szData, pszTmp, wDataLeng);
            pszTmp +=wDataLeng;
            wTotalLen += wDataLeng;
            switch(wTypeID)
            {
            case VIDEO_FORMAT:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwMediaType, szData, wDataLeng);
					tMediaAttr.dwMediaType = ntohl(tMediaAttr.dwMediaType);
                    break;
                }
            case VIDEO_CLOCK_RATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwClockRate, szData, wDataLeng);
					tMediaAttr.dwClockRate = ntohl(tMediaAttr.dwClockRate);
                    break;
                }
            case VIDEO_WIDTH:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.wWidth, szData, wDataLeng);
					tMediaAttr.wWidth = ntohs(tMediaAttr.wWidth);
                    break;
                }
            case VIDEO_HEIGHT:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.wHeigth, szData, wDataLeng);
					tMediaAttr.wHeigth = ntohs(tMediaAttr.wHeigth);
                    break;
                }
            case VIDEO_FRAMERATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.byFramerate, szData, wDataLeng);

                    break;
                }
            case VIDEO_BITRATETYPE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.byStreamType, szData, wDataLeng);

                    break;
                }
            case VIDEO_BITRATE:
                {
                    bIsMedia = TRUE;
                    memcpy(&tMediaAttr.dwBitRate, szData, wDataLeng);
					tMediaAttr.dwBitRate = ntohl(tMediaAttr.dwBitRate);
                    break;
                }

                //音频编码格式 
            case AUDIO_FORMAT:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwAudioFormat, szData, wDataLeng);
					tAudioAttr.dwAudioFormat = ntohl(tAudioAttr.dwAudioFormat);
                    break;
                }
            case AUDIO_SAMPLING_RATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwSamplingRate, szData, wDataLeng);
					tAudioAttr.dwSamplingRate = ntohl(tAudioAttr.dwSamplingRate);
                    break;
                }
            case AUDIO_CHANNELS:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.byChannels, szData, wDataLeng);
                    break;
                }
            case AUDIO_CLOCK_RATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwClockRate, szData, wDataLeng);
					tAudioAttr.dwClockRate = ntohl(tAudioAttr.dwClockRate);
                    break;
                }
            case AUDIO_BITRATE:
                {
                    bIsAudio = TRUE;
                    memcpy(&tAudioAttr.dwBitRate, szData, wDataLeng);
					tAudioAttr.dwBitRate = ntohl(tAudioAttr.dwBitRate);
                    break;
                }
            }
        }
        if (TRUE == bIsMedia && TRUE == bIsAudio)
        {
            eType = Nty_Type_Both;
        }
        else if (TRUE == bIsMedia && FALSE == bIsAudio)
        {
            eType = Nty_Type_Video;
        }
        else if (FALSE == bIsMedia && TRUE == bIsAudio)
        {
            eType = Nty_Type_Audio;
        }
        else
        {
            eType = Nty_Type_None;
        }
		return TRUE;
    }
}PACKED TMTI_STREAM_ATTRI,*PTMTI_STREAM_ATTRI;

typedef struct TMTIStartDevRspEx
{
	u16 wErrCode;
	TMTIStreamAttribute tStreamAttri;

	void PrintData()
	{
		tStreamAttri.PrintData();
	}

	void SwapToNetwork()
	{
		wErrCode = htons(wErrCode);
		tStreamAttri.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		wErrCode = ntohs(wErrCode);
		tStreamAttri.SwapFromNetwork();
	}

	u32 PackedToBuf(s8 * Buf)
	{
		u32 dwBufLen = tStreamAttri.PackedToBuf(Buf);
		return dwBufLen;
	}

	BOOL32 UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		tStreamAttri.UnpackedFromBuf(Buf,BufLen);
		return TRUE;
	}
}PACKED TMTI_STARTDEV_RSPEX,*PTMTI_STARTDEV_RSPEX;

typedef struct TMTIRecFilePlayNtf
{
	u8 byPlayId;
	TMTIStreamAttribute tStreamAttri;

	void PrintData()
	{
		tStreamAttri.PrintData();
	}

	void SwapToNetwork()
	{
		tStreamAttri.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		tStreamAttri.SwapFromNetwork();
	}

	u32 PackedToBuf(s8 * Buf)
	{
		u32 dwBufLen = tStreamAttri.PackedToBuf(Buf);
		return dwBufLen;
	}

	BOOL32 UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		tStreamAttri.UnpackedFromBuf(Buf,BufLen);
		return TRUE;
	}
}PACKED TMTI_RECFILEPLAY_NTF,*PTMTI_RECFILEPLAY_NTF;

//透明数据通知
typedef struct TMTITransDataNtf
{
	u16     wMsgType;		    //消息类型
	u8      bIsLast;            //是否是最后一个Ntf
	u32     dwReserved;         //保留字段
	u16	    wDataBufLen;	    //数据长度
	u8      abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //透明数据结构

	void SwapToNetwork()
	{
		wMsgType = htons(wMsgType);
		wDataBufLen = htons(wDataBufLen);
		dwReserved = htonl(dwReserved);
	}

	void SwapFromNetwork()
	{
		wMsgType = ntohs(wMsgType);
		wDataBufLen = ntohs(wDataBufLen);
		dwReserved = ntohl(dwReserved);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( Buf, &wMsgType, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( Buf, &bIsLast, sizeof(bIsLast));
		Buf += sizeof(bIsLast);
		memcpy(Buf,&dwReserved,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( Buf, &wDataBufLen, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( Buf, abyTransDataBuf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		memcpy( &wMsgType, Buf, sizeof(wMsgType));
		Buf += sizeof(wMsgType);
		memcpy( &bIsLast, Buf, sizeof(bIsLast));
		Buf += sizeof(bIsLast);
		memcpy(&dwReserved,Buf,sizeof(dwReserved));
		Buf += sizeof(dwReserved);
		memcpy( &wDataBufLen, Buf, sizeof(wDataBufLen));
		Buf += sizeof(wDataBufLen);
		memcpy( abyTransDataBuf, Buf, sizeof(abyTransDataBuf));

		return TRUE;
	}

	unsigned short PackedLen() const
	{
		return (sizeof(bIsLast)+sizeof(wMsgType)+sizeof(dwReserved)+sizeof(wDataBufLen)+sizeof(abyTransDataBuf));
	}
}PACKED TMTI_TRANS_DATA_NTF, *PTMTI_TRANS_DATA_NTF;

//告警状态参数
typedef struct TMTIEntityState
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16
	BOOL	bIsEnable;		//状态 FALSE-无效; TRUE-有效
}PACKED TMTI_ENTITY_STATE, *PTMTI_ENTITY_STATE;

//移动侦测告警参数
typedef struct TMTIMotDetState
{
	u16		wEntityID;		//ID modify G.J. 20091020 u8->u16
	u16		wDetAreaNum;	//告警区域号
	BOOL	bIsEnable;		//状态 FALSE-无效;TRUE-有效
}PACKED TMTI_MOT_DET_STATE, *PTMTI_MOT_DET_STATE;

typedef struct TMTIDataSend
{
	u16 wDataLen;			//数据长度
	u8  abyDataBuf[DATA_BUFSIZE];			//数据Buffer
}PACKED TMTI_DATA_SEND, *PTMTI_DATA_SEND;

typedef struct TMTISerialDataSend
{
	u16 wDataLen;			//数据长度
	u8 *pbyDataBuf;			//数据指针
	u8	byChanId;			//编码通道
}PACKED TMTI_SERIAL_DATA_SEND, *PTMTI_SERIAL_DATA_SEND;

//视频源输入端口
typedef struct TMTIVideoInputPort
{
	u8		byVideoEncoderID;	//视频编码通道号
	u8		byVideoInputPortID;	//视频输入端口号
}PACKED TMTI_VIDEO_IN_PORT, *PTMTI_VIDEO_IN_PORT;

//开始告警计时
typedef struct TMTIStartAlarmTimer
{
	u16		wSeconds;			//告警计时显示时间
	u8		byVideoEncoderID;	//视频编码通道号
	u16		wAlarmInputId;		//告警通道号 	//zlq add  modify 20091020
	u8		byType;				//类型	MTI_ALARM_PIN:并口告警 MTI_ALARM_MOTION:移动侦测告警	//zlq add
}PACKED TMTI_START_ALARM_TIMER, *PTMTI_START_ALARM_TIMER;

//发送摄像头返回串口数据
typedef struct TMTISendComData
{
	u8		byVideoInputId;		//视频源端口号
	u8		bySendAble;			//发送使能(MTI_ENBALE-开始发送,MTI_DISABLE－停止发送)
}PACKED TMTI_SEND_COM_DATA, *PTMTI_SEND_COM_DATA;

//用户自定义消息
typedef struct TMTIUserCustomMsg
{
	u16		wMsgLen;			//消息长度
	u8	*	pbyMsgContent;		//参数内容指针
}PACKED TMTI_USER_CUSTOM_MSG, *PTMTI_USER_CUSTOM_MSG;

//回调消息句柄信息
typedef struct TMTIMsgHandle
{
	u8			byMsgType;
	VSIP_HANDLE	tVsipHandle;
}PACKED TMTI_MSG_HANDLE, *PTMTI_MSG_HANDLE;

//设备能力项信息
typedef struct TMTICapItemInfo
{
	u8			byEntityType;	 //能力类型
	u16			wEntityID;		 //能力ID    modify 20091020 u8->u16
	TVsipGuid	tEntityGuid;	 //能力GUID
	u16         wFabricant;      //厂商
}PACKED TMTI_CAP_ITEM_INFO, *PTMTI_CAP_ITEM_INFO;

//设备控制初始化信息
typedef struct TMTIInitInfo
{
	TVsipGuid	tUnitGuid;		//设备GUID
	TVsipGuid	tAppGuid;		//应用程序GUID
	u32			dwLocalIpAddr;	//设备IP地址
	u16			wVsipPort;		//设备VSIP端口
	char abyUserName[MAX_STRING_LEN]; //验证用户名
	char abyPassWord[MAX_STRING_LEN]; //验证用户密码
}PACKED TMTI_INIT_INFO, *PTMTI_INIT_INFO;

enum eDay
{
	MONDAY,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY,SATURDAY,SUNDAY
};

enum emOsdType  //add by ywy 2014.03.14
{
	LOGO = 0,
	ALARM_CAPTION,
	ALARM_PROMPT
};

//bmposd信息 zlq add
typedef struct TBmpOsdInfo
{
	u8		byVideoPort;		              //视频源端口
	u8		byType;			              	  //0:台标 1:告警字幕 2:告警提示
	u16		wWidth;				              //图像宽度
	u16		wHeigth;			              //图像高度
	u16		wTotalPacket;	                  //总包个数
	u16		wCurPacketIdx;	                  //当前包的索引号,从1开始
	u16		wCurLen;			              //当前长度
	u8      abyBuf[MAX_BMPINFOBUF_LEN];       //内容
	u8		byFontNum;			              //字符数量
	u8		byFontHeight;		              //字符高
	u8		abyFontWidth[MAX_OSD_FONT_WIDTH]; //字符宽度
	void SwapToNetWork()
	{
		wWidth = htons(wWidth);
		wHeigth = htons(wHeigth);
		wTotalPacket = htons(wTotalPacket);
		wCurPacketIdx = htons(wCurPacketIdx);
		wCurLen = htons(wCurLen);
	}
	void SwapFromNetwork()
	{
		wWidth = ntohs(wWidth);
		wHeigth = ntohs(wHeigth);
		wTotalPacket = ntohs(wTotalPacket);
		wCurPacketIdx = ntohs(wCurPacketIdx);
		wCurLen = ntohs(wCurLen);
	}
}PACKED TMT_BMPOSD_INFO, *PTMT_BMPOSD_INFO;

typedef struct TBmpOsdAckInfo
{
	u16		wTransactionNum;	//流水号
	u16		wErrorCode;			//操作结果 0:失败 1:成功
	u16		wCurPacketIdx;		//索引号
}PACKED TMT_BMPOSDACK_INFO, *PTMT_BMPOSDACK_INFO;
//add end

//矩阵配置 前端存储 zlq add
typedef struct TMTIMatrixInfo
{
	u8		byMatrixType;						        //矩阵类型
	u8		byMatrixInputPort;					        //矩阵起始输入端口
	u8		byMatrixOutputPort;					        //矩阵起始输出端口
	u8		byMatrixInputNum;					        //矩阵输入端口数
	u8		byMatrixOutputNum;					        //矩阵输出端口数
	u8		abyMatrixPort[MAX_VIDEO_INPUT_PORT_NUM];	//矩阵映射表
}PACKED PAKTMT_MATRIX_INFO,*PTMT_MATRIX_INFO;

typedef struct TRecStartPlayInfo
{
	u32 dwIndexId;
	u32 dwIpAddress;
	u16 wVideoPort;
	u16 wAudioPort;
	u8  byType;
	u32 dwStartTime;
	u32 dwEndTime;
	u32 dwSSRC;
	u16 wVidSrcRtpPort;
	u16 wAudSrcRtpPort;
	u16 wVidSrcRtcpPort;
	u16 wAudSrcRtcpPort;
	u16 wVidDestRtcpPort;
	u16 wAudDestRtcpPort;

	void SwapToNetwork()
	{
		dwIndexId = htonl(dwIndexId);
		wVideoPort = htons(wVideoPort);
		wAudioPort = htons(wAudioPort);
		dwStartTime = htonl(dwStartTime);
	    dwEndTime= htonl(dwEndTime);
		dwSSRC = htonl(dwSSRC);
		wVidSrcRtpPort = htons(wVidSrcRtpPort);
		wAudSrcRtpPort = htons(wAudSrcRtpPort);
		wVidSrcRtcpPort = htons(wVidSrcRtcpPort);
		wAudSrcRtcpPort = htons(wAudSrcRtcpPort);
		wVidDestRtcpPort = htons(wVidDestRtcpPort);
		wAudDestRtcpPort = htons(wAudDestRtcpPort);
	}
	void SwapFromNetwork()
	{
		dwIndexId = ntohl(dwIndexId);
		wVideoPort = ntohs(wVideoPort);
		wAudioPort = ntohs(wAudioPort);
		dwStartTime = ntohl(dwStartTime);
	    dwEndTime= ntohl(dwEndTime);
		dwSSRC = ntohl(dwSSRC);
		wVidSrcRtpPort = ntohs(wVidSrcRtpPort);
		wAudSrcRtpPort = ntohs(wAudSrcRtpPort);
		wVidSrcRtcpPort = ntohs(wVidSrcRtcpPort);
		wAudSrcRtcpPort = ntohs(wAudSrcRtcpPort);
		wVidDestRtcpPort = ntohs(wVidDestRtcpPort);
		wAudDestRtcpPort = ntohs(wAudDestRtcpPort);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwIndexId, sizeof(dwIndexId));
		Buf += sizeof(dwIndexId);
		memcpy( Buf, &dwIpAddress, sizeof(dwIpAddress));
		Buf += sizeof(dwIpAddress);
		memcpy( Buf, &wVideoPort,sizeof(wVideoPort));
		Buf += sizeof(wVideoPort);
		memcpy( Buf,&wAudioPort,sizeof(wAudioPort));
		Buf += sizeof(wAudioPort);
		memcpy( Buf, &byType, sizeof(byType));
		Buf += sizeof(byType);
		memcpy( Buf, &dwStartTime,sizeof(dwStartTime));
		Buf += sizeof(dwStartTime);
		memcpy( Buf,&dwEndTime,sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( Buf,&dwSSRC,sizeof(dwSSRC));
		Buf += sizeof(dwSSRC);
		memcpy( Buf,&wVidSrcRtpPort,sizeof(wVidSrcRtpPort));
		Buf += sizeof(wVidSrcRtpPort);
		memcpy( Buf,&wAudSrcRtpPort,sizeof(wAudSrcRtpPort));
		Buf += sizeof(wAudSrcRtpPort);
		memcpy( Buf,&wVidSrcRtcpPort,sizeof(wVidSrcRtcpPort));
		Buf += sizeof(wVidSrcRtcpPort);
		memcpy( Buf,&wAudSrcRtcpPort,sizeof(wAudSrcRtcpPort));
		Buf += sizeof(wAudSrcRtcpPort);
		memcpy( Buf,&wVidDestRtcpPort,sizeof(wVidDestRtcpPort));
		Buf += sizeof(wVidDestRtcpPort);
		memcpy( Buf,&wAudDestRtcpPort,sizeof(wAudDestRtcpPort));
		Buf += sizeof(wAudDestRtcpPort);

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwIndexId, Buf, sizeof(dwIndexId));
		Buf += sizeof(dwIndexId);
		memcpy( &dwIpAddress, Buf, sizeof(dwIpAddress));
		Buf += sizeof(dwIpAddress);
		memcpy( &wVideoPort,Buf, sizeof(wVideoPort));
		Buf += sizeof(wVideoPort);
		memcpy( &wAudioPort,Buf,sizeof(wAudioPort));
		Buf += sizeof(wAudioPort);
		memcpy( &byType, Buf, sizeof(byType));
		Buf += sizeof(byType);
		memcpy( &dwStartTime,Buf, sizeof(dwStartTime));
		Buf += sizeof(dwStartTime);
		memcpy( &dwEndTime,Buf,sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( &dwSSRC,Buf,sizeof(dwSSRC));
		Buf += sizeof(dwSSRC);
		memcpy( &wVidSrcRtpPort,Buf,sizeof(wVidSrcRtpPort));
		Buf += sizeof(wVidSrcRtpPort);
		memcpy( &wAudSrcRtpPort,Buf,sizeof(wAudSrcRtpPort));
		Buf += sizeof(wAudSrcRtpPort);
		memcpy(&wVidSrcRtcpPort,Buf,sizeof(wVidSrcRtcpPort));
		Buf += sizeof(wVidSrcRtcpPort);
		memcpy( &wAudSrcRtcpPort,Buf,sizeof(wAudSrcRtcpPort));
		Buf += sizeof(wAudSrcRtcpPort);
		memcpy( &wVidDestRtcpPort,Buf,sizeof(wVidDestRtcpPort));
		Buf += sizeof(wVidDestRtcpPort);
		memcpy( &wAudDestRtcpPort,Buf,sizeof(wAudDestRtcpPort));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(dwIndexId)+sizeof(dwIpAddress)+sizeof(wVideoPort)+
			sizeof(wAudioPort)+sizeof(byType)+sizeof(dwStartTime)+sizeof(dwEndTime)+
			sizeof(dwSSRC)+sizeof(wVidSrcRtpPort)+sizeof(wAudSrcRtpPort)+sizeof(wVidSrcRtcpPort)+
			sizeof(wAudSrcRtcpPort)+sizeof(wVidDestRtcpPort)+sizeof(wAudDestRtcpPort));
	}

}PACKED TREC_START_PLAY_INFO,*PTREC_START_PLAY_INFO;

//网管参数
typedef struct TNmsParam
{
	u8		byMemThreshold;        //系统内存使用阀值
	u8      byMaxVideoLoseRatio;   //视频丢包上报阀值
	u32     dwServerIP;            //网管服务器IP地址
	u8      byCpuUseRatio;         //cpu利用率

	void SwapToNetwork()
	{
		//dwServerIP = htonl(dwServerIP);
	}
	void SwapFromNetwork()
	{
		//dwServerIP = ntohl(dwServerIP);
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byMemThreshold, sizeof(byMemThreshold));
		Buf += sizeof(byMemThreshold);
		memcpy( Buf, &byMaxVideoLoseRatio, sizeof(byMaxVideoLoseRatio));
		Buf += sizeof(byMaxVideoLoseRatio);
		memcpy( Buf, &dwServerIP,sizeof(dwServerIP));
		Buf += sizeof(dwServerIP);
		memcpy( Buf,&byCpuUseRatio,sizeof(byCpuUseRatio));
		return TRUE;
	}
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &byMemThreshold, Buf, sizeof(byMemThreshold));
		Buf += sizeof(byMemThreshold);
		memcpy( &byMaxVideoLoseRatio, Buf, sizeof(byMaxVideoLoseRatio));
		Buf += sizeof(byMaxVideoLoseRatio);
		memcpy( &dwServerIP,Buf, sizeof(dwServerIP));
		Buf += sizeof(dwServerIP);
		memcpy( &byCpuUseRatio,Buf,sizeof(byCpuUseRatio));
		return TRUE;
	}
	unsigned short PackedLen()
	{
		return (sizeof(byMemThreshold)+sizeof(byMaxVideoLoseRatio)+sizeof(dwServerIP)+sizeof(byCpuUseRatio));
	}
}PACKED TNMS_PARAM,*PTNMS_PARAM;


//放象器应答
typedef struct TPlayerRsp
{
	u16		wTransactionNum;	//操作流水号
	u8		byPlayerId;			//player id

}PACKED TPLAYER_RSP, *PTPLAYER_RSP;

typedef struct TDevRegeditInfo
{
	u8  abyPuid[MAX_VERSION_STRING_LEN];
	u8  abyPassword[MAX_VERSION_STRING_LEN];
	u16 wPort;
	void SwapToNetwork()
	{
		wPort = htons(wPort);
	}

	void SwapFromNetwork()
	{
		wPort = ntohs(wPort);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, abyPuid, sizeof(abyPuid));
		Buf += sizeof(abyPuid);
		memcpy( Buf, abyPassword, sizeof(abyPassword));
		Buf += sizeof(abyPassword);
		memcpy( Buf, &wPort,sizeof(wPort));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( abyPuid, Buf, sizeof(abyPuid));
		Buf += sizeof(abyPuid);
		memcpy( abyPassword, Buf, sizeof(abyPassword));
		Buf += sizeof(abyPassword);
		memcpy( &wPort,Buf,sizeof(wPort));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(abyPuid)+sizeof(abyPassword)+sizeof(wPort));
	}

}PACKED TDEV_REGEDIT_INFO, *PTDEV_REGEDIT_INFO;



//开始停止录象时间设置结构
typedef struct RecStartEndTimeInfo
{
	TMTISysTime StartTime;     //开始录象时间
	TMTISysTime EndTime;       //停止录象时间

	void SwapToNetwork()
	{
		StartTime.SwapToNetwork();
		EndTime.SwapToNetwork();
	}

	void SwapFromNetwork()
	{
		StartTime.SwapFromNetwork();
		EndTime.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		StartTime.PackedToBuf(Buf,BufLen);
		Buf += StartTime.PackedLen();
		EndTime.PackedToBuf(Buf,BufLen-StartTime.PackedLen());
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
        StartTime.UnpackedFromBuf(Buf,BufLen);
		Buf += StartTime.PackedLen();
		EndTime.UnpackedFromBuf(Buf,BufLen-StartTime.PackedLen());
		return TRUE;
	}


	unsigned short PackedLen()
	{
		return (StartTime.PackedLen()+EndTime.PackedLen());
	}

}PACKED REC_START_END_TIME_INFO, *PREC_START_END_TIME_INFO;

typedef struct TQueryParam
{
	u8 byChanId;
	RecStartEndTimeInfo RStartEndTimeInfo;
	u8 byType;
	u32 dwFileIndex;
	u16 wNum;

	void SwapToNetwork()
	{
		RStartEndTimeInfo.SwapToNetwork();
		dwFileIndex = htonl(dwFileIndex);
		wNum = htons(wNum);
	}
	void SwapFromNetwork()
	{
		RStartEndTimeInfo.SwapFromNetwork();
		dwFileIndex = ntohl(dwFileIndex);
		wNum = ntohs(wNum);
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byChanId, sizeof(byChanId));
		Buf += sizeof(byChanId);

        RStartEndTimeInfo.PackedToBuf(Buf,BufLen-sizeof(byChanId));
		Buf += RStartEndTimeInfo.PackedLen();

		memcpy( Buf, &byType,sizeof(byType));
		Buf += sizeof(byType);
        memcpy( Buf, &dwFileIndex,sizeof(dwFileIndex));
		Buf += sizeof(dwFileIndex);
        memcpy( Buf, &wNum,sizeof(wNum));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &byChanId, Buf, sizeof(byChanId));
		Buf += sizeof(byChanId);

        RStartEndTimeInfo.UnpackedFromBuf(Buf,BufLen-sizeof(byChanId));
		Buf += RStartEndTimeInfo.PackedLen();

		memcpy( &byType,Buf, sizeof(byType));
		Buf += sizeof(byType);
        memcpy( &dwFileIndex,Buf, sizeof(dwFileIndex));
		Buf += sizeof(dwFileIndex);
        memcpy( &wNum,Buf, sizeof(wNum));
		return TRUE;
	}
	unsigned short PackedLen()
	{
		return (sizeof(byChanId)+RStartEndTimeInfo.PackedLen()+sizeof(dwFileIndex)+sizeof(byType)+sizeof(wNum));
	}
}PACKED TQueryParam,*PTQueryParam;

//定时间、录象信息
typedef struct TimelyRecInfo
{
    u8 byEnable;
	RecStartEndTimeInfo RecSETimeInfo;
	void SwapToNetwork()
	{
		RecSETimeInfo.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
		RecSETimeInfo.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byEnable, sizeof(byEnable));
		Buf += sizeof(byEnable);
		RecSETimeInfo.PackedToBuf(Buf,BufLen-sizeof(byEnable));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
	    memcpy( &byEnable, Buf, sizeof(byEnable));
		Buf += sizeof(byEnable);
		RecSETimeInfo.UnpackedFromBuf(Buf,BufLen-sizeof(byEnable));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(byEnable)+RecSETimeInfo.PackedLen());
	}

}PACKED TIMELY_REC_INFO, *PTIMELY_REC_INFO;

//录象文件/事件信息
typedef struct RecFileInfo
{
	u32 dwRecIndexId;                             //文件事件id号
	u8 byRecFileType;                             //录象文件/事件类型
	RecStartEndTimeInfo RecSETimeSetting;         //开始停止录象时间
	u8 abyFileName[MAX_FILE_NAME_LENGTH+1];       //文件别名
	u8 abyFilePath[MAX_FILE_PATH_LENGTH+1];       //最大文件路径长度

	void SetH264Type()
	{
		byRecFileType += MTI_TYPE_H264;
	}
	BOOL IsH264Type()
	{
		if (byRecFileType > MTI_TYPE_H264)
		{
			byRecFileType -= MTI_TYPE_H264;
			return TRUE;
		}
		return FALSE;
	}
	void SwapToNetwork()
	{
		dwRecIndexId = htonl(dwRecIndexId);
    	RecSETimeSetting.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
	   dwRecIndexId = ntohl(dwRecIndexId);
       RecSETimeSetting.SwapFromNetwork();
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwRecIndexId, sizeof(dwRecIndexId));
		Buf += sizeof(dwRecIndexId);
		memcpy( Buf,&byRecFileType,sizeof(byRecFileType));
		Buf += sizeof(byRecFileType);

        RecSETimeSetting.PackedToBuf(Buf,BufLen-sizeof(dwRecIndexId)-sizeof(byRecFileType));
		Buf += RecSETimeSetting.PackedLen();

		memcpy(Buf,abyFileName,sizeof(abyFileName));
		Buf += sizeof(abyFileName);
		memcpy(Buf,abyFilePath,sizeof(abyFilePath));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
	   	memcpy( &dwRecIndexId, Buf, sizeof(dwRecIndexId));
		Buf += sizeof(dwRecIndexId);
		memcpy( &byRecFileType,Buf, sizeof(byRecFileType));
		Buf += sizeof(byRecFileType);

        RecSETimeSetting.UnpackedFromBuf(Buf,BufLen-sizeof(dwRecIndexId)-sizeof(byRecFileType));
		Buf += RecSETimeSetting.PackedLen();
		memcpy(abyFileName,Buf,sizeof(abyFileName));
	    Buf += sizeof(abyFileName);
		memcpy(abyFilePath,Buf,sizeof(abyFilePath));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(dwRecIndexId)+sizeof(byRecFileType)
			+RecSETimeSetting.PackedLen()+sizeof(abyFileName)+sizeof(abyFilePath));
	}
}PACKED REC_FILE_INFO,* PREC_FILE_INFO;

//录像器状态信息
typedef struct RecPlayerState
{
	u8 bIsManualRec;		// 是否在手动录像
	u8 bIsMotiveAlarmRec;	// 是否在移动侦测告警录像
	u8 bIsPinAlarmRec;	    // 是否在并口告警录像
    u8 bIsTimerRec;	    	// 是否在定时录像
	u8 bIsWeekRec;		    // 是否在周期录像
	u8 bIsPreRec;			// 是否在预录

    void SwapToNetwork()
	{
	}
	void SwapFromNetwork()
	{
	}
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &bIsManualRec, sizeof(bIsManualRec));
		Buf += sizeof(bIsManualRec);
		memcpy(Buf, &bIsMotiveAlarmRec,sizeof(bIsMotiveAlarmRec));
		Buf += sizeof(bIsMotiveAlarmRec);
		memcpy( Buf, &bIsPinAlarmRec, sizeof(bIsPinAlarmRec));
        Buf += sizeof(bIsPinAlarmRec);
		memcpy( Buf, &bIsTimerRec, sizeof(bIsTimerRec));
		Buf += sizeof(bIsTimerRec);
		memcpy( Buf, &bIsWeekRec, sizeof(bIsWeekRec));
		Buf += sizeof(bIsWeekRec);
	    memcpy( Buf, &bIsPreRec, sizeof(bIsPreRec));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &bIsManualRec,Buf, sizeof(bIsManualRec));
		Buf += sizeof(bIsManualRec);
		memcpy(&bIsMotiveAlarmRec,Buf, sizeof(bIsMotiveAlarmRec));
		Buf += sizeof(bIsMotiveAlarmRec);
		memcpy( &bIsPinAlarmRec, Buf, sizeof(bIsPinAlarmRec));
        Buf += sizeof(bIsPinAlarmRec);
		memcpy( &bIsTimerRec, Buf, sizeof(bIsTimerRec));
		Buf += sizeof(bIsTimerRec);
		memcpy( &bIsWeekRec,Buf,  sizeof(bIsWeekRec));
		Buf += sizeof(bIsWeekRec);
	    memcpy( &bIsPreRec, Buf,sizeof(bIsPreRec));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (6*sizeof(u8));
	}


}PACKED REC_PLAYER_STATE,* PREC_PLAYER_STATE;

//进度参数信息
typedef struct ProgressParaInfo
{
	u8  abyCreateTime[MAX_TIME_PRECISION];      //录象文件创建时间,包括毫秒
	u32 dwTotalFileTime;                        //文件总时间
	u32 dwCurrentPlayTime;                      //当前播放时间
	u8 byPlayerId;                              //放像器id
	u8 byIsFinished;                            //是否播放结束
	u16 wTransactionNum;                        //操作流水号

    void SwapToNetwork()
	{
		dwTotalFileTime = htonl(dwTotalFileTime);
		dwCurrentPlayTime = htonl(dwCurrentPlayTime);
		wTransactionNum  = htons(wTransactionNum);
	}
	void SwapFromNetwork()
	{
        dwTotalFileTime = ntohl(dwTotalFileTime);
		dwCurrentPlayTime = ntohl(dwCurrentPlayTime);
		wTransactionNum = ntohs(wTransactionNum);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, abyCreateTime, sizeof(abyCreateTime));
		Buf += sizeof(abyCreateTime);
		memcpy(Buf, &dwTotalFileTime,sizeof(dwTotalFileTime));
		Buf += sizeof(dwTotalFileTime);
		memcpy( Buf, &dwCurrentPlayTime, sizeof(dwCurrentPlayTime));
        Buf += sizeof(dwCurrentPlayTime);
		memcpy( Buf, &byPlayerId, sizeof(byPlayerId));
		Buf += sizeof(byPlayerId);
		memcpy( Buf, &byIsFinished,sizeof(byIsFinished));
		Buf += sizeof(byIsFinished);
		memcpy( Buf,&wTransactionNum,sizeof(wTransactionNum));
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( abyCreateTime, Buf, sizeof(abyCreateTime));
		Buf += sizeof(abyCreateTime);
		memcpy( &dwTotalFileTime,Buf,sizeof(dwTotalFileTime));
		Buf += sizeof(dwTotalFileTime);
		memcpy( &dwCurrentPlayTime,Buf, sizeof(dwCurrentPlayTime));
		Buf += sizeof(dwCurrentPlayTime);
		memcpy( &byPlayerId, Buf,sizeof(byPlayerId));
		Buf += sizeof(byPlayerId);
		memcpy( &byIsFinished,Buf, sizeof(byIsFinished));
		Buf += sizeof(byIsFinished);
		memcpy( &wTransactionNum,Buf,sizeof(wTransactionNum));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(abyCreateTime)+sizeof(dwTotalFileTime)+sizeof(dwCurrentPlayTime)+sizeof(byPlayerId)+sizeof(byIsFinished)+sizeof(wTransactionNum));
	}

}PACKED PROGRESS_PARA_INFO,*PPROGRESS_PARA_INFO;

//查询文件信息表
typedef struct QueryRecFileInfo
{
	u16 wNum;                                   //录象文件数目
	u8 byType;                                  //录象文件/事件类型
	u8 bIsFinished;                             //是否是所有的录象文件,1为结束，0为未结束
	RecFileInfo aRFileInfo[MAX_REC_FILE_NUM];   //录象文件信息
	u16	wTransactionNum;                        //文件操作流水号

	void SwapToNetwork()
	{
		wNum = htons(wNum);
		for(int i=0;i<MAX_REC_FILE_NUM;++i)
		{
			aRFileInfo[i].SwapToNetwork();
		}
        wTransactionNum = htons(wTransactionNum);
	}
	void SwapFromNetwork()
	{
		wNum = ntohs(wNum);
		for(int i=0;i<MAX_REC_FILE_NUM;++i)
		{
			aRFileInfo[i].SwapFromNetwork();
		}
		wTransactionNum = ntohs(wTransactionNum);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		int i=0;
		int len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wNum, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy(Buf, &byType,sizeof(byType));
		Buf += sizeof(byType);
		memcpy( Buf, &bIsFinished, sizeof(bIsFinished));
		Buf += sizeof(bIsFinished);
		len = len - sizeof(wNum)-sizeof(byType)-sizeof(bIsFinished);

		for(i=0;i<MAX_REC_FILE_NUM;i++)
		{
		   aRFileInfo[i].PackedToBuf(Buf,len);
		   Buf += aRFileInfo[i].PackedLen();
           len = len - aRFileInfo[i].PackedLen();
		}
		memcpy(Buf,&wTransactionNum,sizeof(wTransactionNum));

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		int i=0;
		int len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &wNum, Buf, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy( &byType,Buf,sizeof(byType));
		Buf += sizeof(byType);
		memcpy( &bIsFinished,Buf, sizeof(bIsFinished));
		Buf += sizeof(bIsFinished);
		len = len - sizeof(wNum)-sizeof(byType)-sizeof(bIsFinished);

		for(i=0;i<MAX_REC_FILE_NUM;i++)
		{
		   aRFileInfo[i].UnpackedFromBuf(Buf,len);
		   Buf += aRFileInfo[i].PackedLen();
           len = len - aRFileInfo[i].PackedLen();
		}
		memcpy(&wTransactionNum, Buf, sizeof(wTransactionNum));
		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(wNum)+sizeof(byType)+sizeof(bIsFinished)+MAX_REC_FILE_NUM*aRFileInfo[0].PackedLen())
			+sizeof(wTransactionNum);
	}

}PACKED QUERY_REC_FILE_INFO,*PQUERY_REC_FILE_INFO;

//周期录象时间设置
typedef struct PeriodlyTimeInfo
{
	u8 byPeriodType;      //MTI_NONE_PERIOD,MTI_PERIOD_DAYLY,MTI_PERIOD_SELECTION
	RecStartEndTimeInfo aRTimeInfo[ 3*(DAYS_OF_WEEK+1)];

	void SwapToNetwork()
	{
		for(int i=0; i<3*(DAYS_OF_WEEK+1);++i)
		{
			aRTimeInfo[i].SwapToNetwork();
		}
	}

	void SwapFromNetwork()
	{
		for(int i=0; i<3*(DAYS_OF_WEEK+1); ++i)
		{
			aRTimeInfo[i].SwapFromNetwork();
		}
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		u16 len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( Buf, &byPeriodType, sizeof(byPeriodType));
		Buf += sizeof(byPeriodType);
		len = BufLen - sizeof(byPeriodType);
		for (int i = 0; i<3*(DAYS_OF_WEEK+1); i++)
		{
			aRTimeInfo[i].PackedToBuf(Buf,len);
			Buf += aRTimeInfo[i].PackedLen();
			len = BufLen - aRTimeInfo[i].PackedLen();

		}
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		u16 len = BufLen;
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &byPeriodType, Buf, sizeof(byPeriodType));
		Buf += sizeof(byPeriodType);
		len = BufLen - sizeof(byPeriodType);
		for (int i = 0; i<3*(DAYS_OF_WEEK+1); i++)
		{
			aRTimeInfo[i].UnpackedFromBuf(Buf,len);
			Buf += aRTimeInfo[i].PackedLen();
			len = BufLen - aRTimeInfo[i].PackedLen();

		}

		return TRUE;
	}

	unsigned short PackedLen()
	{
    	return (sizeof(byPeriodType)+3*(DAYS_OF_WEEK+1)*aRTimeInfo[0].PackedLen());
	}
}PACKED PERIODLY_TIME_INFO,*PPERIODLY_TIME_INFO;

typedef struct RecChannelBasicInfo
{
	u16 wAlarmPreRecTime;        //告警预录时间
	u16 wAlarmResumeRecTime;     //告警恢复后继续录制时间
	u8 byIsAbstractFrame;        //是否抽帧录象 1为是，0为否
	u8 byIsRecWhenAlarmOccur;    //告警是否录象,1为是,0为否
	u8 byAbstractRecInterval;    //抽帧录像间隔,指关键帧个数
	u8 byIsAudioRecorded;        //是否录音频
#ifdef VSIP_MULTI_CHAN_SUPPORT
    u8 byRecChanNum;             //0-录制主码流，其它－录制辅助码流号,默认为0
#endif

	void SwapToNetwork()
	{
		wAlarmPreRecTime = htons(wAlarmPreRecTime);
		wAlarmResumeRecTime = htons(wAlarmResumeRecTime);
	}

	void SwapFromNetwork()
	{
		wAlarmPreRecTime = ntohs(wAlarmPreRecTime);
		wAlarmResumeRecTime = ntohs(wAlarmResumeRecTime);
	}


	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &wAlarmPreRecTime, sizeof(wAlarmPreRecTime));
		Buf += sizeof(wAlarmPreRecTime);
		memcpy( Buf, &wAlarmResumeRecTime, sizeof(wAlarmResumeRecTime));
		Buf += sizeof(wAlarmResumeRecTime);
		memcpy( Buf, &byIsAbstractFrame, sizeof(byIsAbstractFrame));
		Buf += sizeof(byIsAbstractFrame);
		memcpy( Buf, &byIsRecWhenAlarmOccur, sizeof(byIsRecWhenAlarmOccur));
		Buf += sizeof(byIsRecWhenAlarmOccur);
		memcpy( Buf, &byAbstractRecInterval, sizeof(byAbstractRecInterval));
        Buf += sizeof(byAbstractRecInterval);
        memcpy( Buf ,&byIsAudioRecorded,sizeof(byIsAudioRecorded));
#ifdef VSIP_MULTI_CHAN_SUPPORT
        Buf += sizeof(byIsAudioRecorded);
        memcpy( Buf ,&byRecChanNum,sizeof(byRecChanNum));	
#endif
		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &wAlarmPreRecTime, Buf, sizeof(wAlarmPreRecTime));
		Buf += sizeof(wAlarmPreRecTime);
		memcpy( &wAlarmResumeRecTime, Buf, sizeof(wAlarmResumeRecTime));
		Buf += sizeof(wAlarmResumeRecTime);
		memcpy( &byIsAbstractFrame, Buf, sizeof(byIsAbstractFrame));
		Buf += sizeof(byIsAbstractFrame);
		memcpy(&byIsRecWhenAlarmOccur, Buf, sizeof(byIsRecWhenAlarmOccur));
		Buf += sizeof(byIsRecWhenAlarmOccur);
		memcpy(&byAbstractRecInterval, Buf, sizeof(byAbstractRecInterval));
		Buf += sizeof(byAbstractRecInterval);
        memcpy(&byIsAudioRecorded, Buf,sizeof(byIsAudioRecorded));
#ifdef VSIP_MULTI_CHAN_SUPPORT
        Buf += sizeof(byIsAudioRecorded);
        memcpy( &byRecChanNum, Buf, sizeof(byRecChanNum));
#endif
                
		return TRUE;
	}

	unsigned short PackedLen()
	{
		u16 len;
		len = sizeof(wAlarmPreRecTime)+sizeof(wAlarmResumeRecTime)+sizeof(byIsAbstractFrame)+
                        sizeof(byIsRecWhenAlarmOccur)+sizeof(byAbstractRecInterval)+sizeof(byIsAudioRecorded);
#ifdef VSIP_MULTI_CHAN_SUPPORT
		len = len + sizeof (byRecChanNum);
#endif
		return len;
	}
}PACKED REC_CHANNEL_BASIC_INFO,*PREC_CHANNEL_BASIC_INFO;

typedef struct RecSysInfo
{
	u8 byDiskFullStrategy;                                                 // 磁盘满录象策略
	u8 abyRecFileStorePath[MTI_DISK_NUM];                                  // 录象文件存放路径
	u32 dwDiskRemainSpace;                                                 // 磁盘剩余空间（M）
	u16 wLittleFileRecTime;                                                // 小文件录象时长
	u8 abyRecDiskPartitionInfo[MTI_DISK_NUM][MAX_DISK_PARTITION_NAME];     // 磁盘分区信息，用户不可设置
	u8 byDiskPartitionNum;                                                 // 文件分区数目,用户不可设置
	u8 byUsedPartitionNum;                                                 // 用户使用了的分区数目

    void SwapToNetwork()
	{
		dwDiskRemainSpace = htonl(dwDiskRemainSpace);
		wLittleFileRecTime = htons(wLittleFileRecTime);
	}

	void SwapFromNetwork()
	{
		dwDiskRemainSpace = ntohl(dwDiskRemainSpace);
		wLittleFileRecTime = ntohs(wLittleFileRecTime);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &byDiskFullStrategy, sizeof(byDiskFullStrategy));
		Buf += sizeof(byDiskFullStrategy);
		memcpy( Buf, abyRecFileStorePath, sizeof(abyRecFileStorePath));
		Buf += sizeof(abyRecFileStorePath);
		memcpy( Buf, &dwDiskRemainSpace, sizeof(dwDiskRemainSpace));
		Buf += sizeof(dwDiskRemainSpace);
		memcpy( Buf, &wLittleFileRecTime, sizeof(wLittleFileRecTime));
		Buf += sizeof(wLittleFileRecTime);
		memcpy(Buf,abyRecDiskPartitionInfo,sizeof(abyRecDiskPartitionInfo));
		Buf += sizeof(abyRecDiskPartitionInfo);
		memcpy(Buf,&byDiskPartitionNum,sizeof(byDiskPartitionNum));
        Buf += sizeof(byDiskPartitionNum);
		memcpy(Buf,&byUsedPartitionNum,sizeof(byUsedPartitionNum));


		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( &byDiskFullStrategy, Buf, sizeof(byDiskFullStrategy));
		Buf += sizeof(byDiskFullStrategy);
		memcpy( abyRecFileStorePath, Buf, sizeof(abyRecFileStorePath));
		Buf += sizeof(abyRecFileStorePath);
		memcpy( &dwDiskRemainSpace, Buf, sizeof(dwDiskRemainSpace));
		Buf += sizeof(dwDiskRemainSpace);
		memcpy( &wLittleFileRecTime, Buf, sizeof(wLittleFileRecTime));
		Buf += sizeof(wLittleFileRecTime);
		memcpy(abyRecDiskPartitionInfo,Buf,sizeof(abyRecDiskPartitionInfo));
		Buf += sizeof(abyRecDiskPartitionInfo);
		memcpy(&byDiskPartitionNum,Buf,sizeof(byDiskPartitionNum));
		 Buf += sizeof(byDiskPartitionNum);
		memcpy(&byUsedPartitionNum,Buf,sizeof(byUsedPartitionNum));

		return TRUE;
	}

	unsigned short PackedLen()
	{
		return (sizeof(byDiskFullStrategy)+sizeof(abyRecFileStorePath)+sizeof(dwDiskRemainSpace)+sizeof(wLittleFileRecTime)+
			sizeof(abyRecDiskPartitionInfo)+sizeof(byDiskPartitionNum)+sizeof(byUsedPartitionNum));
	}

}PACKED REC_SYS_INFO,*PREC_SYS_INFO;

//zlq add 抓拍功能设置
typedef struct TMTISnapShotConfig
{
	u8 byPinAlarmEnable;		//并口告警抓图使能 0关闭/1开启
	u8 byMotionDetectEnable;	//移动侦测抓图使能 0关闭/1开启
	u32	dwInterval;				//抓拍时间间隔(s)
	u32 dwMaxSnapNum;			//最大抓拍数量
}PACKED SNAP_SHOT_CONFIG,*PSNAP_SHOT_CONFIG;

typedef struct TMTISnapShotNamingRule
{
	// 字段顺序，从0开始，依次表示第i+1个字段
	u8 abyNameOrder[MAX_SEPARATOR_LEN];
	// 实际使用的字段个数
	u8 byNameNum;
	//  日期之间的分隔符(年月日)
	char achDateSeparator[MAX_SEPARATOR_LEN];
	// 时间之间的分隔符(时分秒)
	char achTimeSeparator[MAX_SEPARATOR_LEN];
	// 命名字段之间的分隔符
	char achNameSeparator[MAX_SEPARATOR_LEN];
	// 用户自定义字段内容
	char achUserDefine[MAX_STRING_LEN+1];
	// 是否远端存储
	BOOL bIsRemoteStore;
	// 服务器共享路径（格式：172.16.3.108:/usr/share)
	char achSvrSharePath[MAX_PATH_LEN + 1];
}PACKED TSNAPSHOT_NAMINGRULE,*PTSNAPSHOT_NAMINGRULE;

//zlq add 图片文件信息
typedef struct TMTIPicFileInfo
{
	u8 byType;			//事件类型
	u8 bychanId;		//通道号
	u8 byVideoResolution;//图像分辨率
	u8 byFormat;		//图片格式
	TMTISysTime tTime;	//截图时间
	u32 dwPicID;		//图片编号
	char achFileName[MAX_FILE_NAME_LENGTH+1];	//文件别名
	void SwapToNetwork()
	{
		tTime.SwapToNetwork();
		dwPicID = htonl(dwPicID);
	}

	void SwapFromNetwork()
	{
		tTime.SwapFromNetwork();
		dwPicID = ntohl(dwPicID);
	}
}PACKED TPIC_FILE_INFO,*PTPIC_FILE_INFO;

typedef struct TMTIQueryPicFileInfo
{
	u16 wMaxNum;		//当前查询文件总数量
	u8 byIndex;			//当前包索引号,从1开始
	u8 byNum;			//文件数量
	u8 byEnd;			//是否结束
	TMTIPicFileInfo aPicFileInfo[MAX_REC_FILE_NUM];	//图片文件列表
	void SwapToNetwork()
	{
		wMaxNum = htons(wMaxNum);
		for (int i = 0; i < MAX_REC_FILE_NUM; i++)
		{
			aPicFileInfo[i].SwapToNetwork();
		}
	}

	void SwapFromNetwork()
	{
		wMaxNum = ntohs(wMaxNum);
		for (int i = 0; i < MAX_REC_FILE_NUM; i++)
		{
			aPicFileInfo[i].SwapFromNetwork();
		}
	}
}PACKED QUERY_PIC_FILE_INFO,*PQUERY_PIC_FILE_INFO;

typedef struct TMTIQueryPicFileParam
{
	u8 byNum;			//文件数量
	u8 byType;			//查询类型
	u8 byChanId;		//通道号
	u8 byEvent;			//事件类型
	u16 wBeginNum;			//文件查询起始序号
	TMTISysTime tTimeStart;	//起始抓拍时间
	TMTISysTime tTimeEnd;	//结束抓拍时间
//	char achFileName[MAX_FILE_NAME_LENGTH+1];	//文件别名
	void SwapToNetwork()
	{
		wBeginNum = htons(wBeginNum);
		tTimeStart.SwapToNetwork();
		tTimeEnd.SwapToNetwork();
	}
	void SwapFromNetwork()
	{
		wBeginNum = ntohs(wBeginNum);
		tTimeStart.SwapFromNetwork();
		tTimeEnd.SwapFromNetwork();
	}
}PACKED QUERY_PIC_FILE_PARAM,*PQUERY_PIC_FILE_PARAM;

typedef struct TMTIDownloadPicFileInfo
{
	u16		wResult;			/*操作结果*/		
	u16		wTotalPacket;		/*总包个数*/
	u16		wCurPacketIdx;	    /*当前包的索引号,从1开始*/
	u16		wCurLen;			/*当前长度*/
	u32		dwPicID;			//图片编号
	u8      abyBuf[SMALL_DATA_BUFSIZE];       /*内容*/
	void SwapToNetwork()
	{
		wResult = htons(wResult);
		wTotalPacket = htons(wTotalPacket);
		wCurPacketIdx = htons(wCurPacketIdx);
		wCurLen = htons(wCurLen);
		dwPicID = htonl(dwPicID);

	}
	void SwapFromNetwork()
	{
		wResult = ntohs(wResult);
		wTotalPacket = ntohs(wTotalPacket);
		wCurPacketIdx = ntohs(wCurPacketIdx);
		wCurLen = ntohs(wCurLen);
		dwPicID = ntohl(dwPicID);

	}
}PACKED DOWNLOAD_PICFILE_INFO,*PDOWNLOAD_PICFILE_INFO;

//zlq add 图片操作结果
typedef struct TMTISnapShotResult
{
	u16 wError;		//结果
	u32 dwPicID;	//图片ID
	void SwapToNetwork()
	{
		wError = htons(wError);
		dwPicID = htonl(dwPicID);
	}
	void SwapFromNetwork()
	{
		wError = ntohs(wError);
		dwPicID = ntohl(dwPicID);
	}
}PACKED SNAP_SHOT_RESULT, *PSNAP_SHOT_RESULT;
#define SNAP_SHOT_RESULT_LEN  sizeof(TMTISnapShotResult)

//zlq add 图片操作结果
typedef struct TMTISnapShotResultEx
{
	u16 wError;		//结果
	u32 dwPicID;	//图片ID
	char achPicName[MAX_FILE_NAME_LENGTH + 1]; //该结构体与其他流不一致
	void SwapToNetwork()
	{
		wError = htons(wError);
		dwPicID = htonl(dwPicID);
	}

	void SwapFromNetwork()
	{
		wError = ntohs(wError);
		dwPicID = ntohl(dwPicID);
	}
}PACKED SNAP_SHOT_RESULT_EX, *PSNAP_SHOT_RESULT_EX;

//zlq add 网管相关配置
typedef struct TMTIPUNMSCFG
{
	u8	byRegType;								//连接类型:DNS,PUI
	u32	dwDNSIp;								//DNS的ip地址
	u16 wDNSPort;								//DNS端口
	u16 wPingCheckTimes;						//断链检测次数
	u16	wPingCheckInterval;						//断链检测时间间隔
	u32	dwVersionServerIp;						//版本服务器IP
	u16 wVersionServerPort;						//版本服务器端口
	u32 dwVersionCheckInterval;					//版本检测周期 单位:分钟
	s8	achVersionPath[MAX_PATH_LEN+1];			//下载版本文件信息
	s8	achVersionUserName[MAX_STRING_LEN+1];	//版本服务器用户名
	s8	achVersionUserPW[MAX_STRING_LEN+1];		//版本服务器密码
	u8	bySlotId;								//槽号
	s8	achBoxInfo[MAX_STRING_LEN+1];			//槽位信息
	s8	achLocationInfo[MAX_STRING_LEN+1];		//位置信息
	u16     DownloadStartPort;                  // 下载起始端口
	u16     BrowseStartPort;                    // 浏览起始端口
}PACKED PU_NMS_CFG,*PPU_NMS_CFG;

typedef struct TConnectionLostList
{
	u32 dwIpAddr;
	u16 wPort;
	u32 dwCheckTimerId;
	BOOL bEnable;
}PACKED TCONNECTION_LOST_LIST,*PTCONNECTION_LOST_LIST;

//录像文件下载通知
typedef struct TRecFileDownNty
{
	u16		wTransactionNum;	//操作流水号
	u16		wErrorCode;
	u16		wFtpPort;
}PACKED TRECFILEDOWNNTY;

#ifdef VSIP_MULTI_CHAN_SUPPORT
// 各流状态及通道间关系
typedef struct TMultiChanInfo
{
        u8 byChanType;                          //主流还是辅助流,1－主流，0－辅流
        u8 bySupportSubNum;             //支持辅助流路数，辅助流为0;如果是开启第二路码流，则填其码流号。
        u8 SubChanId[8];				//辅助流ID列表，根据辅助流路数，依次填写
        u8 byStatus;                            //当前辅助流是否开启（1－开启，0-未开启），仅主流有；如果是开启双流，则填1，如果停止双流则填0；
        u8 byParentEncId;                       //主流为0，辅助流为主流的编码通道号

        void SwapToNetwork()
        {
                //dwServerIP = htonl(dwServerIP);
        }
        void SwapFromNetwork()
        {
                //dwServerIP = ntohl(dwServerIP);
        }
        BOOL PackedToBuf( char * Buf, unsigned short BufLen )
        {
                if( PackedLen() > BufLen)
                {
                        return FALSE;
                }
                memcpy( Buf, &byChanType, sizeof(byChanType));
                Buf += sizeof(byChanType);
                memcpy( Buf, &bySupportSubNum, sizeof(bySupportSubNum));
                Buf += sizeof(bySupportSubNum);
                memcpy( Buf, &SubChanId, sizeof(SubChanId));
                Buf += sizeof(SubChanId);				
                memcpy( Buf, &byStatus,sizeof(byStatus));
                Buf += sizeof(byStatus);
                memcpy( Buf, &byParentEncId, sizeof(byParentEncId));
                return TRUE;
        }
        BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
        {
                if( PackedLen() > BufLen)
                {
                        return FALSE;
                }
                memcpy( &byChanType, Buf, sizeof(byChanType));
                Buf += sizeof(byChanType);
                memcpy( &bySupportSubNum, Buf, sizeof(bySupportSubNum));
                Buf += sizeof(bySupportSubNum);
                memcpy( &SubChanId, Buf, sizeof(SubChanId));
                Buf += sizeof(SubChanId);
                memcpy( &byStatus,Buf, sizeof(byStatus));
                Buf += sizeof(byStatus);
                memcpy( &byParentEncId,Buf,sizeof(byParentEncId));
                return TRUE;
        }
        unsigned short PackedLen()
        {
                return (sizeof(byChanType)+sizeof(bySupportSubNum)+sizeof(SubChanId)+sizeof(byStatus)+sizeof(byParentEncId));
        }
}PACKED TMULTICHANINFO, *PTMULTICHANINFO;

// 保存配置用，如不保存配置，则可以不用此结构
typedef struct TDevMultiChanInfo
{
        u8 byTotalNum;                          //视频编码通道数量
        TMultiChanInfo tMultiChanInfo[MAX_VIDEO_ENC_NUM];       //各流状态及通道间关系表
}PACKED TDEVMULTICHANINFO, *PTDEVMULTICHANINFO;

#endif

//cu透明通道消息结构体
typedef struct TMTICuOpertionMsg
{
private:
	u16    m_wBufLen;     //透明数据结构长度
	u8     m_abyTransDataBuf[MTI_MAX_CU_OPERTION_NUM];   //透明数据结构
	
	void SetBufLen(u16 wBufLen) 
	{
		m_wBufLen = htons(wBufLen);
	}
public:
	u16 GetBufLen() const
	{
		return ntohs(m_wBufLen);
	}
	
    void SetData(const u8 *pbyData, u16 wBufLen)
    {
        if(pbyData == NULL)
		{
			return;
		}
		
        memset(m_abyTransDataBuf, 0, sizeof(m_abyTransDataBuf));
        wBufLen = wBufLen > MTI_MAX_CU_OPERTION_NUM ? MTI_MAX_CU_OPERTION_NUM : wBufLen;
        memcpy(m_abyTransDataBuf, pbyData, wBufLen);
        SetBufLen(wBufLen);
    }
    const u8* GetData() const
    {
        return m_abyTransDataBuf;
    }
	
	u16 PackedLen() const
	{
		return sizeof(m_wBufLen)+GetBufLen();
	}
	
} PACKED TMTICuOpertionMsg, *PTMTICuOpertionMsg;

typedef struct TGpsStartPlayInfo
{
	u32  dwBeginTime; //开始时间,单位(秒） 
	u32  dwEndTime;   //结束时间，单位(秒) 
	u16  wPlayRate;   //播放速率,单位(秒) 
    u32  dwReserve1;  //保留字段 
	
	void SwapToNetwork()
	{
		dwBeginTime = htonl(dwBeginTime);
		dwEndTime = htonl(dwEndTime);
		wPlayRate = htons(wPlayRate);
		dwReserve1 = htonl(dwReserve1);
	}
	void SwapFromNetwork()
	{
		dwBeginTime = ntohl(dwBeginTime);
		dwEndTime = ntohl(dwEndTime);
		wPlayRate = ntohs(wPlayRate);
		dwReserve1 = ntohl(dwReserve1);
	}
	
	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( Buf, &dwBeginTime, sizeof(dwBeginTime));
		Buf += sizeof(dwBeginTime);
		memcpy( Buf, &dwEndTime, sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( Buf, &wPlayRate,sizeof(wPlayRate));
		Buf += sizeof(wPlayRate);
		memcpy( Buf,&dwReserve1,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		
		return TRUE;
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy( &dwBeginTime, Buf, sizeof(dwBeginTime));
		Buf += sizeof(dwBeginTime);
		memcpy( &dwEndTime, Buf, sizeof(dwEndTime));
		Buf += sizeof(dwEndTime);
		memcpy( &wPlayRate,Buf, sizeof(wPlayRate));
		Buf += sizeof(wPlayRate);
		memcpy( &dwReserve1,Buf,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(dwBeginTime)+sizeof(dwEndTime)+sizeof(wPlayRate)+sizeof(dwReserve1));
	}
	
}PACKED TGPS_START_PLAY_INFO,*PTGPS_START_PLAY_INFO;

typedef struct TGpsStartPlayRsp
{
public:
	u32 dwErrorCode;
	u8  byPlayId;
	u32 dwReserve;
}PACKED TGPS_START_PLAY_RSP,*PTGPS_START_PLAY_RSP;

typedef struct TPlayGpsNotify
{
	u8 byPlayerId;                              //放像器id 
	u8 byIsFinished;                            //是否播放结束 
	u32 dwErrorCode;                            //播放结束的错误码 
	u16 wNum;                                   //包含几条GpsData，后面接wNum个GpsData数据 
	u32 dwRecId;								//录像文件的唯一标识
    u32 dwReserve1;                             //保留字段
	u8  achGpsData[DATA_BUFSIZE];				//Gps数据部分

	void SwapFromNetwork()
	{
		dwErrorCode = ntohl(dwErrorCode);
		wNum = ntohs(wNum);
		dwReserve1 = ntohl(dwReserve1);
		dwRecId = ntohl(dwRecId);
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(&byPlayerId,Buf,sizeof(byPlayerId));
		Buf += 1;
		memcpy(&byIsFinished,Buf,sizeof(byIsFinished));
		Buf += 1;
		memcpy( &dwErrorCode, Buf, sizeof(dwErrorCode));
		Buf += sizeof(dwErrorCode);
		memcpy( &wNum,Buf, sizeof(wNum));
		Buf += sizeof(wNum);
		memcpy( &dwRecId, Buf, sizeof(dwRecId) );
		Buf += sizeof(dwRecId);
		memcpy( &dwReserve1,Buf,sizeof(dwReserve1));
		Buf += sizeof(dwReserve1);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(byPlayerId)+sizeof(byIsFinished)+sizeof(dwErrorCode)+sizeof(wNum)+sizeof(dwRecId)+sizeof(dwReserve1));
	}

}PACKED TPLAY_GPS_NOTIFY,*PTPLAY_GPS_NOTIFY;

typedef struct TTransSubsBase
{
private:
	u32 m_dwSubscribeType;
	u32 m_dwUserDefine;
	u32 m_dwReserved;
	u16 m_wBufLen;     //透明数据结构长度
	u8  m_abyTransDataBuf[DATA_BUFSIZE];   //透明数据结构 //DATA_BUFSIZE vsip最大消息长度
public:

	void SwapToNetwork()
	{
		m_dwSubscribeType = htonl(m_dwSubscribeType);
		m_dwUserDefine = htonl(m_dwUserDefine);
		m_dwReserved = htonl(m_dwReserved);
		m_wBufLen = htons(m_wBufLen);
	}
	void SwapFromNetwork()
	{
		m_dwSubscribeType = ntohl(m_dwSubscribeType);
		m_dwUserDefine = ntohl(m_dwUserDefine);
		m_dwReserved = ntohl(m_dwReserved);
		m_wBufLen = ntohs(m_wBufLen);
	}

	BOOL PackedToBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}

		memcpy( Buf, &m_dwSubscribeType, sizeof(m_dwSubscribeType) );
		Buf += sizeof(m_dwSubscribeType);
		memcpy( Buf, &m_dwUserDefine, sizeof(m_dwUserDefine) );
		Buf += sizeof(m_dwUserDefine);
		memcpy( Buf, &m_dwReserved, sizeof(m_dwReserved) );
		Buf += sizeof(m_dwReserved);	
		memcpy( Buf, &m_wBufLen, sizeof(m_wBufLen) );
		Buf += sizeof(m_wBufLen);

		memcpy( Buf, m_abyTransDataBuf, ntohs(m_wBufLen) );

		return TRUE;
	}

	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{

		memcpy( &m_dwSubscribeType, Buf, sizeof(m_dwSubscribeType) );
		Buf += sizeof(m_dwSubscribeType);
		memcpy( &m_dwUserDefine, Buf, sizeof(m_dwUserDefine) );
		Buf += sizeof(m_dwUserDefine);
		memcpy( &m_dwReserved, Buf, sizeof(m_dwReserved) );
		Buf += sizeof(m_dwReserved);
		memcpy( &m_wBufLen, Buf, sizeof(m_wBufLen) );
		Buf += sizeof(m_wBufLen);

		if( sizeof(m_abyTransDataBuf) < ntohs(m_wBufLen))
		{
			return FALSE;
		}

		memcpy( m_abyTransDataBuf, Buf, ntohs(m_wBufLen) );

		return TRUE;
	}



	u16 GetBufLen() const
	{
		return ntohs(m_wBufLen);
	}


	void SetBufLen(u16 wBufLen) 
	{
		m_wBufLen = htons(wBufLen);
	}

	u32 GetSubscribeType() const
	{

		return ntohl(m_dwSubscribeType);
	}

	void SetSubscribeType(u32 dwSubscribeType)
	{
		m_dwSubscribeType = htonl(dwSubscribeType);
	}

	void SetUserDefine(u32 dwUserDefine)
	{
		m_dwUserDefine = htonl(dwUserDefine);
	}

	u32 GetUserDefine() const
	{

		return ntohl(m_dwUserDefine);
	}

	void SetReserved(u32 dwReserved)
	{
		m_dwReserved = htonl(dwReserved);
	}

	u32 GetReserved() const
	{

		return ntohl(m_dwReserved);
	}


	void SetData(const u8 *pbyData, u16 wBufLen)
	{
		if(pbyData == NULL)
		{
			return;
		}

		memset(m_abyTransDataBuf, 0, sizeof(m_abyTransDataBuf));
		wBufLen = wBufLen > DATA_BUFSIZE ? DATA_BUFSIZE : wBufLen;
		memcpy(m_abyTransDataBuf, pbyData, wBufLen);
		SetBufLen(wBufLen);
	}
	const u8* GetData() const
	{
		return m_abyTransDataBuf;
	}

	u16 PackedLen() const
	{
		return sizeof(m_dwSubscribeType)+sizeof(m_dwUserDefine)+sizeof(m_dwReserved)+sizeof(m_wBufLen)+GetBufLen();
	}

	u16 StructLen() const
	{
		return sizeof(m_dwSubscribeType)+sizeof(m_dwUserDefine)+sizeof(m_dwReserved)+sizeof(m_wBufLen)+sizeof(m_abyTransDataBuf);
	}

} PACKED TTransSubsBase, *PTTransSubsBase;

typedef struct TTransSubsReq:public TTransSubsBase
{
	void SetExpire(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetExpire() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsReq, *PTTransSubsReq;

typedef struct TTransSubsRsp:public TTransSubsBase
{
	void SetErrorResult(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetErrorResult() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsRsp, *PTTransSubsRsp;

typedef struct TTransSubsNtf:public TTransSubsBase
{
	void SetSequenceNumber(u32 dwUserDefine)
	{
		SetUserDefine(dwUserDefine);
	}

	u32 GetSequenceNumber() const
	{

		return GetUserDefine();
	}

}PACKED TTransSubsNtf, *PTTransSubsNtf;

typedef struct TMtiDecStyleParam
{
	u16 wOutputId;
	u16 wDecStyle;
}PACKED TMTI_DEC_STYLE_PARAM,*PTMTI_DEC_STYLE_PARAM;

typedef struct TMtiCustomExtCapSet
{
	u8 byCapNum;
	u8 abyCapItem[MAX_CUSTOMCAP_NUM];
}PACKED TMTI_CUSTOM_EXT_CAP_SET,*PTMTI_CUSTOM_EXT_CAP_SET;

typedef struct TMtiGpsData 
{ 
	u32 dwTime;  //该GpsData的时间 
	char cNSIndicator; //南北纬 
	char cEWIndicator; //东西经 
	u8   byLatitudeHigh; //纬度高位值 
	u32  dwLatitudeLow; //纬度低位值 
	u8   byLongitudeHigh;    //经度高位值 
	u32  dwLongitudeLow;   //经度低位值 
	u16  wSpeedHigh;       //速度高位值 
	u16  wSpeedLow;        //速度低位值

	void SwapFromNetwork()
	{
		dwTime = ntohl(dwTime);
		dwLatitudeLow = ntohl(dwLatitudeLow);
		dwLongitudeLow = ntohl(dwLongitudeLow);
		wSpeedHigh = ntohs(wSpeedHigh);
		wSpeedLow = ntohs(wSpeedLow);
	}
	
	BOOL UnpackedFromBuf( char * Buf, unsigned short BufLen )
	{
		if( PackedLen() > BufLen)
		{
			return FALSE;
		}
		memcpy(&dwTime,Buf,sizeof(dwTime));
		Buf += sizeof(dwTime);
		memcpy(&cNSIndicator,Buf,sizeof(cNSIndicator));
		Buf += 1;
		memcpy(&cEWIndicator,Buf,sizeof(cEWIndicator));
		Buf += 1;
		memcpy(&byLatitudeHigh,Buf,sizeof(byLatitudeHigh));
		Buf += 1;
		memcpy( &dwLatitudeLow, Buf, sizeof(dwLatitudeLow));
		Buf += sizeof(dwLatitudeLow);
		memcpy(&byLongitudeHigh,Buf,sizeof(byLongitudeHigh));
		Buf += 1;
		memcpy( &dwLongitudeLow,Buf, sizeof(dwLongitudeLow));
		Buf += sizeof(dwLongitudeLow);
		memcpy( &wSpeedHigh,Buf,sizeof(wSpeedHigh));
		Buf += sizeof(wSpeedHigh);
		memcpy( &wSpeedLow,Buf,sizeof(wSpeedLow));
		Buf += sizeof(wSpeedLow);
		return TRUE;
	}
	
	unsigned short PackedLen()
	{
		return (sizeof(dwTime)+sizeof(cNSIndicator)+sizeof(cEWIndicator)+sizeof(byLatitudeHigh)+sizeof(dwLatitudeLow)+
			sizeof(byLongitudeHigh)+sizeof(dwLongitudeLow)+sizeof(wSpeedHigh)+sizeof(wSpeedLow));
	}

}PACKED TMTI_GPSDATA,*PTMTIGPSDATA;

//bmpserial信息
typedef struct TBmpSerialInfo
{
    u8      byVideoPort;          /*视频源端口*/
    u8      byType;               /*0:台标,1:告警字幕 2:告警提示 4:串口图片*/
    u16     wWidth;              //图像宽度
    u16     wHeigth;             //图像高度
    u16     wTotalPacket;         /*总包个数*/
    u16     wCurPacketIdx;        /*当前包的索引号,从1开始*/
    u16     wCurLen;              /*当前长度*/
    u8      abyBuf[MAX_BMPINFOBUF_LEN];       /*内容*/
    u8      byFontNum;            //字符数量
    u8      byFontHeight;         //字符高
    u8      abyFontWidth[18];     //字符宽度
    u16     wLeftMargin; //编码字幕左边距 1-200;
    u16     wTopMargin; //编码字幕上边距 1-200;
}PACKED TMT_BMPSERIAL_INFO, *PTMT_BMPSERIAL_INFO;


//后端上报给前端的能力集，目前仅上报是否支持采样率协商
typedef enum EnumVsipBackendEntity
{
	emVideoClockRateNego            = 1,    // MTI_ENABLE=支持采样率协商，MTI_DISABLE=不支持采样率协商 
} EVsipBackendEntity;

typedef struct TMTIBackendCap
{
	u16 wEntityId;                  // EVsipBackendEntity
	u32 dwEntityValue;
}PACKED TMTI_BACKEND_CAP,*PTMTI_BACKEND_CAP;

typedef struct TMTIBackendCapSets
{
	u16 wCapNum;                         
	TMTIBackendCap EntityValue[MAX_BACKENDCAP_NUM];
}PACKED TMTI_BACKEND_CAPSETS,*PTMTI_BACKEND_CAPSETS;

#ifdef _LINUX_ARM_D6446_	//zhuzhm modify 070917
typedef void (*pDISCOVERY_CB)( TMTIDiscoveryCBInfo *atDiscvInfo, u16 wUnitNum, void* pContext );
#else
typedef void (*pDISCOVERY_CB)( TMTIDiscoveryCBInfo atDiscvInfo[], u16 wUnitNum, void* pContext );
#endif
typedef void (*pMESSAGE_CB)( TMTIMsgHandle tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen, void* pContext );
typedef void (*pCONNECTED_CB)( u32 dwPuiIp, TVsipGuid tDevGuid, u32 dwUnitIpAddr, u16 wPort, void* pContext );
typedef void (*pCONNECTIONCHECK_CB)( TVsipGuid tDevGuid,  void* pContext);
typedef void (*pGetGuidByIp_CB)(TVsipGuid tDevGuid,u8 byLocalSerialId,u8 byTargetSerialId,void* pContext);
typedef void (*pTERMINATED_CB)(void);

typedef void (*pNEWDISCOVERY_CB)(TMTIDiscoveryCBInfo atDiscvInfo, void* pContext);

//前端设备分类 add G.J.20090903
enum
{
    DEVICE_PU_KDM = 0,      //普通PU设备
    DEVICE_PU_NVR,          //NVR设备
	DEVICE_PU_THIRD,        //外厂商设备
	DEVICE_PU_NVR128,       //nvr128
    DEVICE_PU_3G,           //无线设备
    DEVICE_PU_UNKNOWN,      //未知设备
	DEVICE_PU_G100,
};
//addend

//GPS透明数据结构
struct TMTISubscribeTransData
{
    u16 m_wType;                   //透明数据类型
    u32 m_dwSubType;                //透明数据子类型
    
    void SwapToNetwork()
    {
        m_wType = htons(m_wType);
        m_dwSubType = htonl(m_dwSubType);
    }
    
    void SwapFromNetwork()
    {
        m_wType = ntohs(m_wType);
        m_dwSubType = ntohl(m_dwSubType);
    }
}PACKED;

class CMTIUnitCtrl
{
public:
	CMTIUnitCtrl();
	~CMTIUnitCtrl();

	u16 SendUserOpertionMsg(u8 byChnId,char* pbyBuf,u16 wBufLen,BOOL32 bUseDevGuid = FALSE);//用户透明通道

	u16 GpsDataStartPlay(TGpsStartPlayInfo tPlayParam,u8 byVideoEncID);//开始播放Gps数据
	u16 GpsDataStopPlay(u8 byPlayId);//停止播放Gps数据

	u16 SendTransSubs(TTransSubsReq & tTransSubsReq);
	u16 SendTransSubsCancle(u16 wTransactionNum, TTransSubsReq & tTransSubsReq);

	u16 SetDecoderStyleParam(TMtiDecStyleParam tSetDecStyleParam);
	u16 GetDecoderStyleParam(TMtiDecStyleParam& tDecStyleParam);

	u16 SetBmpSerialData(const TMT_BMPSERIAL_INFO &tBmpData);

	u16 SetVidEncOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 GetVidEncOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 SetVidSrcOsdCfgEx(u16 wDataLen, char *achBuf);
	u16 GetVidSrcOsdCfgEx(u16 wDataLen, char *achBuf);

    //GPS相关
    u16 SetTransReq(u16 wType, const char* pBuf, u16 wBufLen);
    u16 GetTransReq(u16 wType);
    u16 SubscribeTransReq(u8 byOpetype, TMTISubscribeTransData tTans);
    u16 SubscribeTransReq(u8 byOpetype, const char* pBuf, u16 wBufLen);
    u16 InquiryTransReq(TMTISubscribeTransData tTans);
	u16 InquiryTransReq(const char* pBuf, u16 wBufLen);

    u16 FileTransData(u8 byOsdType,u16 wLen,char *abyBuf);
    u16 StartFileTransReq(u8 byOsdType);
    u16 StopFileTransReq(u8 byOsdType);
    u16 StartSmartSightCamera(u8 bySerialID,char *achBuf,u16 wLen);
    void SetConnectStatus(VSIP_HANDLE VsipHandle, eConnState byStatus);
	void SetConnectPui(VSIP_HANDLE VsipHandle, u8 *abyPuid);
    eConnState GetConnectStatus(VSIP_HANDLE VsipHandle);
	void PrintUnitInfo();


	//hzc_Transparent_Chan_20110324{{
	u16 SendUserTcpMsgEx(u8 byVideoEncID,char *pbyBuf, u16 wpbyBufLen);
	//}}

	//传输透明数据
	u16 SendUserTcpMsgEx2(u8 byVideeInID,u16 wMsgType,char *pbyBuf, u16 wpbyBufLen);

	/*==================================================================
	功能        : 扩展音视频的码流接收请求(对于音频或视频解码器)
	输入参数说明: 扩展媒体控制信息结构
	              s8* pName=NULL -- 第三方解码厂商名称默认为NULL
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
    u16 StartDecoderRecvEx(TMTIMediaCtrlEX tCfgParam, TMTIStreamAttribute tStreamAttri, s8* pName=NULL);
	/*==================================================================
	功能        : 设置告警参数(西安电信）
	输入参数说明: 告警参数
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 SetAlarmParam(TMTIAlarmParam tCfgParam);
	/*==================================================================
	功能        : 设置横幅参数(西安电信）
	输入参数说明: 横幅参数
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 SetBannerParam(TMTIBannerParam tCfgParam);
	/*==================================================================
	功能        : 获取告警参数(西安电信）
	输入参数说明: 告警参数
	返回值说明  : 成功返回MTI_SUCCESS，失败返回MTI_FAIL
	==================================================================*/
    u16 GetAlarmParam(TMTIAlarmParam &tCfgParam);
	/*==================================================================
	功能        : 获取横幅参数(西安电信）
	输入参数说明: 横幅参数
	返回值说明  : 成功返回MTI_SUCCESS，失败返回MTI_FAIL
	==================================================================*/
    u16 GetBannerParam(TMTIBannerParam &tCfgParam);
	/*======================前端录象业务接口start==========================*/

	/*==================================================================
	功能        : 打印录像信息
	输入参数说明: 通道id号
	返回值说明  : 无
	==================================================================*/
    void PrintRecInfo(u8 byChannel);

	/*==================================================================
	功能        : 指定某个终端开始录像
	输入参数说明: 通道id号
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
    u16 StartRec(u8 byChannel);
	/*==================================================================
	功能        : 指定某个终端停止录像
	输入参数说明: 通道id号
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
    u16 StopRec(u8 byChannel);
   /*==================================================================
	功能        : 设置某个终端定时录象参数
	输入参数说明: 通道id号
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 SetTimelyRecInfo(u8 byChannel, TimelyRecInfo TRecInfo);
	/*==================================================================
	功能        : 获取某个终端定时录象参数
	输入参数说明: 通道id号,定时录象参数
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetTimelyRecInfo(u8 byChannel, TimelyRecInfo & TRecInfo);
   /*==================================================================
	功能        : 设置某终端周期录象参数
	输入参数说明: 通道id号,周期类型，周期时间信息
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 SetPeriodlyRecInfo (u8 byChannel, PeriodlyTimeInfo PTimeInfo);
	/*==================================================================
	功能        : 获取某终端周期录象参数
	输入参数说明: 通道id号,周期时间信息
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetPeriodlyRecInfo (u8 byChannel,PeriodlyTimeInfo & PTimeInfo);
	/*==================================================================
	功能        : 设置通道录象基本参数
	输入参数说明: 通道id号,录象基本信息
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 SetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo RChannelBasicInfo);
	/*==================================================================
	功能        : 获取通道录象基本参数
	输入参数说明: 通道id号,录象基本信息
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo & RChannelBasicInfo);

	/*==================================================================
	功能        : 设置录象系统参数
	输入参数说明: 录象系统信息
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 SetRecSysInfo(RecSysInfo RSysInfo);

	/*==================================================================
	功能        : 获取录象系统参数
	输入参数说明: 录象系统信息
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetRecSysInfo(RecSysInfo & RSysInfo);
	/*==================================================================
	功能        : 查询通道的录象文件
	输入参数说明: 查询信息
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 QueryRecFile(TQueryParam QueryParam);
	/*==================================================================
	功能        : 查询满足条件的录象文件的个数
	输入参数说明: 查询信息
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 QueryRecFileNum(TQueryParam QueryParam);
	/*==================================================================
	功能        : 录象文件播放
	输入参数说明: (文件或事件)id号，ip地址，端口号,类型见MTI_QUIEY_ALL_EVENT
	返回值说明  :  成功返回操作流水号，失败返回MTI_FAIL
    附加说明：    操作结果通知
	==================================================================*/
	u16 RecFileStartPlay(TRecStartPlayInfo tCfgParam);
	/*==================================================================
	功能        : 录象文件操作
	输入参数说明: 播放器id，操作类型见MTI_OPERATION_NORMAL
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 RecFileOperation(u8 byPlayerId, u8 byOperationType);
	/*==================================================================
	功能        : 录像文件放像定位
	输入参数说明: 播放器id，拖拉时间
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
    附加说明：    操作结果通知
	==================================================================*/
	u16 RecSeekLocation(u8 byPlayerId,u32 dwDragTime);
	/*==================================================================
	功能        : 删除录象文件或事件，根据类型来判断，
                  若为文件，dwIndexId为文件id
				  若为事件，dwIndexId为事件id
	输入参数说明: id号，类型见MTI_QUIEY_ALL_EVENT
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 DelRecFile(u32 dwIndexId,u8 byType);
    /*==================================================================
	功能        : 删除所有录象记录(事件)如告警录象，手动录象
	              见MTI_QUIEY_ALL_EVENT
	输入参数说明: 文件id号
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
    u16 DelRecRecord(u8 byType);

	/*==================================================================
	功能        : 获取录象器状态
	输入参数说明: 通道id号，录象器状态
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 GetRecStatus(u8 byChannelId, RecPlayerState & byStatus);
	/*==================================================================
	功能        : 导出录象文件
	输入参数说明: 文件id号(MTI_CONNECTION_UDP,MTI_CONNECTION_TCP)
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 StartExportRecFile(u32 dwRecFileIndexId,u32 dwIpAddress,u16 wPort,u8 byConnectionType = MTI_CONNECTION_TCP);
	/*==================================================================
	功能        : 停止导出录象文件
	输入参数说明: 无
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    操作结果通知
	==================================================================*/
	u16 StopExportRecFile();
	/*==================================================================
	功能        : 设置网管参数
	输入参数说明: 网管参数
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	附加说明：    状态更新通知
	==================================================================*/
    u16 SetNmsParam(TNmsParam NmsParam);
	/*==================================================================
	功能        : 获取网管参数
	输入参数说明: 网管参数
	返回值说明  : 成功返回操作流水号，失败返回MTI_FAIL
	==================================================================*/
	u16 GetNmsParam(TNmsParam & NmsParam);
	/*======================前端录象业务接口end============================*/

   /*==================================================================
	功能        : 打印实体信息
	输入参数说明:
	返回值说明  :
	==================================================================*/
    void PrintEntityInfo();
	/*==================================================================
	功能        : 打印能力集信息
	输入参数说明:
	返回值说明  :
	==================================================================*/
    void PrintCapability();
	void PrintInfo();
	void PrintExtCapability();
	void PrintBasicParamInfo();
//#ifndef _LINUXEQUATOR_
	/*==================================================================
	功能        : 初始化设备控制
	输入参数说明: const TMTIInitInfo & tUnitInfo 设备初始参数
				  u8 byConnectionType 消息链路类型
				  u8 byIdentify 是否验证密码
				  (若验证则在TMTIInitInfo的结构体字段填写用户名密码字段）

	返回值说明  : 成功返回 TRUE; 失败返回 FALSE
	==================================================================*/
	BOOL Initialize( const TMTIInitInfo & tUnitInfo, u32 &dwConIP, u8 byConnectionType = MTI_CONNECTION_TCP,u8 byIdentify = 0);
	//zlq add
	BOOL InitializeEx( const TMTIInitInfo & tUnitInfo, u32 &dwConIP, u8 byConnectionType = MTI_CONNECTION_TCP,u8 byIdentify = 0);
	u16 SetBmpOsdData(TMT_BMPOSD_INFO &tBmpData);
	u16 QueryPicFile(TMTIQueryPicFileParam QueryParam);
	u16 QueryPicFileNum(TMTIQueryPicFileParam QueryParam);
	u16 DownLoadPicFile(TMTIPicFileInfo tPicInfo, u16 wIndex, u8 byHighSpeed  = 1);
	u16 DelPicFile(TMTIQueryPicFileInfo tPicFileInfo);//zlq add 删除图片文件 可以批量删
	u16 DelPicFileEx(TMTIPicFileInfo tPicFileInfo);//zlq add 07-01-27 //删除单个文件
	u16 SetSnapShotConfig(u8 byChannel, TMTISnapShotConfig tConfig);//抓图设置
	u16 GetSnapShotConfig(u8 byChannel, TMTISnapShotConfig &tConfig);
	u16 SetSnapShotNameRule(const TMTISnapShotNamingRule &tConfig);
	u16 GetSnapShotNameRule(TMTISnapShotNamingRule &tConfig);
	u16 StartSnapShot(u8 byChannel);
	u16 StopSnapShot(u8 byChannel);
	u16 SetMatrixParam(const TMTIMatrixInfo &tConfig);
    u16 GetMatrixParam(TMTIMatrixInfo &tConfig);  //mtc同步,pui异步
	u16 SetPuNmsParam(TMTIPUNMSCFG tConfig);
	u16 GetPuNmsParam(TMTIPUNMSCFG &tConfig);//mtc使用,同步方式
	u16 SendUserTcpMSg(u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
//#endif
	u16 ChangePassword( const TMTIUserPassword &tUserInfo);
	u16 SetVideoDynVal(u8 byChanId,u8 byVidFmt, u8 byDynVal);
	//add end
	/*==================================================================
	功能        : 设置消息回调函数
	输入参数说明: pMESSAGE_CB pFunc 回调函数指针
				  void* pContext 回调函数上下文
	返回值说明  : 成功返回 TRUE; 失败返回 FALSE
	==================================================================*/
	BOOL SetMessageCallBack( pMESSAGE_CB pFunc, void* pContext = NULL );

	/*==================================================================
	功能        : 获取设备能力集
	输入参数说明: TMTICapability & tCapability 待设置的能力集结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetCapabilities( TMTICapability & tCapability);
	u16 GetExtCapabilities(TMTICAPSET & tCapability);
	u16 GetCustomExtCapabilities();
	u16 GetAdpCapabilities(TMTICAPSET & tCapability,const char* achMtName);
	/*==================================================================
	功能        : 获取设备系统参数
	输入参数说明: TMTISysParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetSysCfg( TMTISysParam & tCfgParam);

	u16 GetSysCfgEx( TMTISysParam & tCfgParam);

	/*==================================================================
	功能        : 获取PPPOE参数
	输入参数说明: TMTIPPPOEParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetPPPOECfg( TMTIPPPOEParam & tCfgParam);

	/*==================================================================
	功能        : 获取系统公共参数
	输入参数说明: TMTISysPubParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetSysPubCfg( TMTISysPubParam & tCfgParam);

	/*==================================================================
	功能        : 获取视频编码网络参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTICommonNetParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncNetCfg( u16 wVideoEncID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 获取视频解码网络参数
	输入参数说明: u8 byVideoDecID 视频解码通道号(从0开始)
				  TMTICommonNetParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoDecNetCfg( u16 wVideoDecID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 获取音频编码网络参数
	输入参数说明: u8 byAudioEncID 音频编码通道号(从0开始)
				  TMTICommonNetParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetAudioEncNetCfg( u16 wAudioEncID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 获取音频解码网络参数
	输入参数说明: u8 byAudioDecID 音频解码通道号(从0开始)
				  TMTICommonNetParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetAudioDecNetCfg( u16 wAudioDecID, TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 获取视频编码参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIVideoEncParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncCfg( u8 byVideoEncID, TMTIVideoEncParam & tCfgParam, bool bSupportAdvancedEncParam = true );
	//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
	u16 GetVideoEncCfgEx( u8 byVideoEncID, TMTIVideoEncParam & tCfgParam );
	//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

	/*==================================================================
	功能        : 获取视频编码公共参数
	输入参数说明: TMTIVideoEncPubParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncPubCfg( TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频编码字幕参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIVideoEncOSDParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncOSDCfg( u16 wVideoEncID, TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频编码画面合成参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIVideoEncMPParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncMPCfg( u16 byVideoEncID, TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频编码视频源端口参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIVideoEncInputPortParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncInputPortCfg( u16 wVideoEncID, TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频编码轮询参数
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIVideoEncPollParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoEncPollCfg( u16 wVideoEncID, TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频解码参数
	输入参数说明: u8 byVideoDecID 视频解码通道号(从0开始)
				  TMTIVideoDecParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoDecCfg( u16 wVideoDecID, TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	功能        : 获取视频解码公共参数
	输入参数说明: u8 byVideoDecID 视频解码通道号(从0开始)
				  TMTIVideoDecPubParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoDecPubCfg( u16 wVideoDecID, TMTIVideoDecPubParam & tCfgParam, BOOL32 bExtend = FALSE );

	/*==================================================================
	功能        : 获取视频解码重传参数
	输入参数说明: u8 byVideoDecID 视频解码通道号(从0开始)
				  TMTIVideoDecRetransferParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoDecRetransferCfg( u16 wVideoDecID, TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	功能        : 获取音频编码参数 异步接口
	输入参数说明: u8 byAudioEncID 音频编码通道号(从0开始)
				  TMTIAudioEncParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetAudioEncCfg( u16 wAudioEncID, TMTIAudioEncParam & tCfgParam ); 

	/*==================================================================
	功能        : 获取音频解码参数
	输入参数说明: u8 byAudioDecID 音频解码通道号(从0开始)
				  TMTIAudioDecParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetAudioDecCfg( u16 wAudioDecID, TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	功能        : 获取串口参数
	输入参数说明: u8 bySerialPortID 串口号(从0开始)
				  TMTISerialPortParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetSerialPortCfg( u16 wSerialPortID, TMTISerialPortParam & tCfgParam );

	/*==================================================================
	功能        : 获取串口公共参数
	输入参数说明: u8 bySerialPortID 串口号(从0开始)
				  TMTISerialPortPubParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetSerialPortPubCfg( u16 wSerialPortID, TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	功能        : 获取并口输入参数
	输入参数说明: u8 bySerialPortID 并口输入端口号(从0开始)
				  TMTIInputPinParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetInputPinCfg( u16 wInputPinID, TMTIInputPinParam & tCfgParam );

	/*==================================================================
	功能        : 获取并口输入通道使能状态信息
	输入参数说明: TMTIInputPinParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetInputPinEnableStateCfg(TMTIInputPinState& tInputPinState); //20091020

	/*==========================================================================
	功能        : 设置并口输入使能状态
	输入参数说明: TMTIInputPinState& tEnableState 待设置的参数结构
	返回值说明  : void
	==========================================================================*/
	void SetInputPinEnableState(const TMTIInputPinState& tEnableState );

	/*==================================================================
	功能        : 获取视频源端口参数
	输入参数说明: u8 byVideoInputID 视频源端口号(从0开始)
				  TMTIVideoInputParam & tCfgParam 待设置的参数结构
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetVideoInputCfg( u16 wVideoInputID, TMTIVideoInputParam & tCfgParam, u32 dwTimeOut = 1000, bool bSendToPu = true, bool bPuReg = true);

	/*==================================================================
	功能        : 获取图象屏蔽参数
	输入参数说明: u16 wEntityID 视频源端口号
				  TMTIShadeParam *ptCfgParam 移动图象屏蔽参数
				  u16 & wDetAreaNum 返回图象屏蔽区域数
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
    u16 GetShadeCfg(u16 wEntityID, TMTIShadeParam *ptCfgParam, u16 & wShadeAreaNum );

	/*==================================================================
	功能        : 获取移动侦测参数
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
				  TMTIMotionDetectParamNew *ptCfgParam 移动侦测参数数组(必须为十六个元素数组)
				  u16 & wDetAreaNum 返回移动侦测区域数
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetMotionDetectCfg( u8 byType, u16 wEntityID, TMTIMotionDetectParam *ptCfgParam, u16 &wDetAreaNum );

	/*==================================================================
	功能        : 设置设备系统参数
	输入参数说明: const TMTISysParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetSysCfg( const TMTISysParam & tCfgParam );

	/*==================================================================
	功能        : 设置PPPOE参数
	输入参数说明: const TMTIPPPOEParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetPPPOECfg( const TMTIPPPOEParam & tCfgParam );

	/*==================================================================
	功能        : 设置系统公共参数
	输入参数说明: const TMTISysPubParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetSysPubCfg( const TMTISysPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码网络参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTICommonNetParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncNetCfg( u8 byVideoEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置视频解码网络参数
	输入参数说明: u8 byVideoDecID 视频解码通道ID(从0开始)
				  const TMTICommonNetParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoDecNetCfg( u8 byVideoDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置音频编码网络参数
	输入参数说明: u8 byAudioEncID 音频编码通道ID(从0开始)
				  const TMTICommonNetParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetAudioEncNetCfg( u8 byAudioEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置音频解码网络参数
	输入参数说明: u8 byAudioDecID 音频解码通道ID(从0开始)
				  const TMTICommonNetParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetAudioDecNetCfg( u8 byAudioDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置视频编码参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTIVideoEncParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncCfg( u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );
	//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
	u16 SetVideoEncCfgEx(u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );
	//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

	/*==================================================================
	功能        : 设置视频编码公共参数
	输入参数说明: const TMTIVideoEncPubParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncPubCfg( const TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码字幕参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTIVideoEncOSDParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncOSDCfg( u8 byVideoEncID, const TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码画面合成参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTIVideoEncMPParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncMPCfg( u8 byVideoEncID, const TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码视频源端口参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTIVideoEncInputPortParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncInputPortCfg( u8 byVideoEncID, const TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码轮询参数
	输入参数说明: u8 byVideoEncID 视频编码通道ID(从0开始)
				  const TMTIVideoEncPollParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoEncPollCfg( u8 byVideoEncID, const TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码参数
	输入参数说明: u8 byVideoDecID 视频解码通道ID(从0开始)
				  const TMTIVideoDecParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoDecCfg( u8 byVideoDecID, const TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码公共参数
	输入参数说明: u8 byVideoDecID 视频解码通道ID(从0开始)
				  const TMTIVideoDecPubParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoDecPubCfg( u8 byVideoDecID, const TMTIVideoDecPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码重传参数
	输入参数说明: u8 byVideoDecID 视频解码通道ID(从0开始)
				  const TMTIVideoDecRetransferParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoDecRetransferCfg( u8 byVideoDecID, const TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	功能        : 设置音频编码参数
	输入参数说明: u8 byAudioEncID 音频编码通道ID(从0开始)
				  const TMTIAudioEncParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetAudioEncCfg( u8 byAudioEncID, const TMTIAudioEncParam & tCfgParam );

	/*==================================================================
	功能        : 设置音频解码参数
	输入参数说明: u8 byAudioDecID 音频解码通道ID(从0开始)
				  const TMTIAudioDecParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetAudioDecCfg( u8 byAudioDecID, const TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	功能        : 设置串口参数
	输入参数说明: u8 bySerialPortID 串口ID(从0开始)
				  const TMTISerialPortParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetSerialPortCfg( u8 bySerialPortID, const TMTISerialPortParam & tCfgParam );
    	/*==================================================================
	功能        : 设置设备guid
	输入参数说明: u32 McuId
				  u32 MtId
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetDevGuidCfg(u32 McuId, u32 MtId);

	/*==================================================================
	功能        : 设置后端能力集
	输入参数说明: TMTIBackendCapSets
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetBackendCap(const TMTIBackendCapSets& tBackEndCap);

 	/*==================================================================
	功能        :  获取设备mcuid，也就是guid的data1字段
	输入参数说明:  空

	返回值说明  : 成功返回mcuid; 失败返回 MTI_FAIL
	==================================================================*/
	u32 GetDevMcuId();
	/*==================================================================
	功能        :  获取设备mtid，也就是guid的data2和data3合并字段
	输入参数说明:  空

	返回值说明  : 成功返回mtid; 失败返回 MTI_FAIL
	==================================================================*/
	u32 GetDevMtId();

	/*==================================================================
	功能        : 设置串口公共参数
	输入参数说明: u8 bySerialPortID 串口ID(从0开始)
				  const TMTISerialPortPubParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetSerialPortPubCfg( u8 bySerialPortID, const TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置并口输入参数
	输入参数说明: u16 wInputPinID 并口输入端口ID(从0开始)
				  const TMTIInputPinParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetInputPinCfg( u16 wInputPinID, const TMTIInputPinParam & tCfgParam );

	/*==================================================================
	功能        : GetThirdPartyVideoDecCfg
	输入参数说明: 获取支持第三方解码类型
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 GetThirdPartyVideoDecCfg(TMTIThirdPartyDecParam & tCfgParam );

	/*==================================================================
	功能        : 设置摄像头协议类型
	输入参数说明: u8 byVideoInputID 并口输入端口ID(从0开始)
				  const u8 byCameraType 摄像头协议类型
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	//{{{{{{{{{{{{{{{ modify by Royan 20140520 SetCameraType新增CameraId项
	//u16 SetCameraType( u8 byVideoInputID, const u8 byCameraType );
	u16 SetCameraType( u8 byVideoInputID, const u8 byCameraType, const u8 byCameraId );
	//}}}}}}}}}}}}}}} modify by Royan 20140520 SetCameraType新增CameraId项

	/*==================================================================
	功能        : 设置视频源端口参数
	输入参数说明: u8 byVideoInputID 并口输入端口ID(从0开始)
				  const TMTIVideoInputParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoInputCfg( u8 byVideoInputID, const TMTIVideoInputParam & tCfgParam );

	/*==================================================================
	功能        : 设置图象屏蔽参数
	输入参数说明: u16 wEntityID 视频源端口ID(从0开始)
				  const TMTIShadeParam & tCfgParam 设置的参数
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
    u16 SetShadeCfg( u16 wEntityID, const TMTIShadeParam * ptCfgParam, u16 wAreaNum );

	/*==================================================================
	功能        : 设置移动侦测参数
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
				  TMTIMotionDetectParamNew * ptCfgParam 移动侦测区域参数
				  u16 wAreaNum  移动侦测区域数
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetMotionDetectCfg( u8 byType, u16 wEntityID, const TMTIMotionDetectParam * ptCfgParam, u16 wAreaNum );

	/*==================================================================
	功能        : 开始视频发送
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  TMTIMediaSwitchAddr tMediaSwitchAddr 码流发送ip port等
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartVideoSend( u8 byVideoEncID,TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	功能        : 开始音频发送
	输入参数说明: u8 byAudioEncID 音频编码通道号(从0开始)
				  u32 dwDestIpAddr 发送目的IP地址
				  u16 wDestPort 发送目的端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartAudioSend( u8 byAudioEncID,TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	功能        : 开始视频接收
	输入参数说明: u8 byVideoDecID 视频解码通道号(从0开始)
				  u32 dwDestIpAddr 源IP地址
				  u16 wLocalPort 接收端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartVideoRecv( u8 byVideoDecID, TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	功能        : 开始音频接收
	输入参数说明: u8 byAudioDecID 音频解码通道号(从0开始)
				  u32 dwDestIpAddr 源IP地址
				  u16 wLocalPort 接收端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP,MTI_MEDIA_STREAM_TYPE_TCP)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartAudioRecv( u8 byAudioDecID, TMTIMediaSwitchAddr& tMediaSwitchAddr, u8 byType );

	/*==================================================================
	功能        : 增加视频发送目标
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  u32 dwDestIpAddr 发送目的IP地址
				  u16 wDestPort 发送目的端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP 等)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 AddVideoSendDest( u8 byVideoEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	功能        : 删除视频发送目标
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
				  u32 dwDestIpAddr 发送目的IP地址
				  u16 wDestPort 发送目的端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP 等)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 RemoveVideoSendDest( u8 byVideoEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	功能        : 增加音频发送目标
	输入参数说明: u8 byAEncID 音频编码通道号(从0开始)
				  u32 dwDestIpAddr 发送目的IP地址
				  u16 wDestPort 发送目的端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP 等)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 AddAudioSendDest( u8 byAEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	功能        : 删除音频发送目标
	输入参数说明: u8 byAEncID 音频编码通道号(从0开始)
				  u32 dwDestIpAddr 发送目的IP地址
				  u16 wDestPort 发送目的端口号
				  u8 byType 码流传输类型(MTI_MEDIA_STREAM_TYPE_UDP 等)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 RemoveAudioSendDest( u8 byAEncID, u32 dwDestIpAddr, u16 wDestPort, u8 byType );

	/*==================================================================
	功能        : 停止视频发送
	输入参数说明: u8 byVideoEncID 视频编码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopVideoSend( u8 byVideoEncID );

	/*==================================================================
	功能        : 停止音频发送
	输入参数说明: u8 byAudioEncID 音频编码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopAudioSend( u8 byAudioEncID );

	/*==================================================================
	功能        : 停止视频接收
	输入参数说明: u8 byVideoDecID 视频编码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopVideoRecv( u8 byVideoDecID );

	/*==================================================================
	功能        : 停止音频接收
	输入参数说明: u8 byAudioDecID 音频解码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopAudioRecv( u8 byAudioDecID );

	/*==================================================================
	功能        : 设置音频解码流属性
	输入参数说明: u8 byAudioDecID 音频解码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetMediaStreamProp(u8 byChnDecID,TMTIStreamAttribute& tStreamPropList);

	/*==================================================================
	功能        : 创建串口数据通道
	输入参数说明: u8 byComPortID 串口号(从0开始)
				  u8 byType 串口数据通道类型(MTI_SERIAL_STREAM_TYPE_UDP 等)
	返回值说明  : 成功返回 TRUE; 失败返回 FALSE
	==================================================================*/
	BOOL CreateSerialDataChannel( u8 byComPortID, u8 byType );

	/*==================================================================
	功能        : 删除串口数据通道
	输入参数说明: byComPortID 串口号(从0开始)
	返回值说明  : 成功返回 TRUE; 失败返回 FALSE
	==================================================================*/
	BOOL RemoveSerialDataChannel( u8 byComPortID );

	/*==================================================================
	功能        : 获取当前串口数据通道类型
	输入参数说明: u8 byComPortID 串口号(从0开始)
	返回值说明  : 串口数据通道类型(MTI_SERIAL_STREAM_TYPE_UDP 等)
	==================================================================*/
	u8  GetSerialDataChannelType( u8 byComPortID );

	/*==================================================================
	功能        : 发送串口数据
	输入参数说明: u8 byComPortID 串口号(从0开始)
				  u8* pbyDataBuf 数据指针
				  u16 wBufLen 数据长度
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SendSerialData( u8 byComPortID, u8* pbyDataBuf, u16 wBufLen );

	/*==================================================================
	功能        : 发送摄像头控制数据
	输入参数说明: u8 byVideoInputID 视频输入端口号(从0开始)
				  u8* pbyDataBuf 数据指针
				  u16 wBufLen 数据长度
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SendCameraCtrlData( u8 byVideoInputID, u8* pbyDataBuf, u16 wBufLen );

	/*==================================================================
	功能        : 设置并口输出状态
	输入参数说明: u16 wOutputPinID 输出端口号(从0开始)
				  BOOL bIsEnable 输出状态 TRUE有效, FALSE无效
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetOutputPinState( u16 wOutputPinID, BOOL bIsEnable );

	/*==================================================================
	功能        : 设置视频输入端口
	输入参数说明: u8 byVideoEncID 编码通道号(从0开始)
				  u8 byVideoInputPortID 视频输入端口号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetVideoInputPort( u8 byVideoEncID, u8 byVideoInputPortID );

	/*==================================================================
	功能        : 开始移动侦测
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartMotionDetect( u8 byType, u16 wEntityID );

	/*==================================================================
	功能        : 停止移动侦测
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopMotionDetect( u8 byType, u16 wEntityID );

	/*==================================================================
	功能        : 开始告警计时
	输入参数说明: u8 byVideoEncID 编码通道号(从0开始)
				  u16 wSeconds 告警计时时间
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StartAlarmTimer( TMTIStartAlarmTimer tShowAlarmTimer);

	/*==================================================================
	功能        : 停止告警计时
	输入参数说明: u8 byVideoEncID 编码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 StopAlarmTimer( TMTIStartAlarmTimer tShowAlarmTimer);

	/*==================================================================
	功能        : 隐藏告警计时
	输入参数说明: u8 byVideoEncID 编码通道号(从0开始)
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 HideAlarmTimer( u8 byVideoEncID);

	/*==================================================================
	功能        : 恢复默认参数设置
	输入参数说明: 无
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 LoadDefaultSettings();

	/*==================================================================
	功能        : 保存参数
	输入参数说明: 无
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SaveSettings();

	/*==================================================================
	功能        : 是否需要重启设备
	输入参数说明: 无
	返回值说明  : TRUE 需要重启设备; FALSE 不需要重启设备
	==================================================================*/
	BOOL IsRebootRequired( );

	/*==================================================================
	功能        : 重启设备
	输入参数说明: 无
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 Reboot();

	/*==================================================================
	功能        : 设置设备系统时间
	输入参数说明: const TMTISysTime & tSysTime 设置的时间
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SetDevSysTime( const TMTISysTime & tSysTime);

	/*==================================================================
	功能        : 发送自定义消息
	输入参数说明: u16 wMsgLen 消息长度
				  u8 * pbyMsgContent 消息内容
	返回值说明  : 成功返回 操作流水号; 失败返回 MTI_FAIL
	==================================================================*/
	u16 SendCustomMsg( u16 wMsgLen, u8 * pbyMsgContent);

	/*==================================================================
	功能        : 请求终端开始向目标IP发送NAT探测包
	输入参数说明: u32 dwMpIPAddr 交换板IP，dwMcuIPAddr－Mcu的IP
    			  wAudCallDetPort－音频呼叫端口，即发送音频解码探测包的目的端口
				  wRTPNATPort － RTP探测端口
	返回值说明  : 操作流水号
	==================================================================*/
	u16 StartNATDetect(u32 dwMcuIPAddr, u32 dwMpAddr,u16 wSourcePort, u16 wDesPort, u8 byDeviceType );

	/*==================================================================
	功能        : 请求终端停止向目标IP发送NAT探测包
	输入参数说明: dwMpIPAddr 交换板IP
	返回值说明  : 操作流水号
	==================================================================*/
	u16 StopNATDetect( u32 dwMpIPAddr);

	/*====================================================================
	功能		: 请求终端发送关键帧
	输入参数说明: u8 byVideoEncID 视频编码通道号
	返回值说明  : 成功返回操作流水号, 失败返回MTI_FAIL
	====================================================================*/
	u16 SendKeyFrame(u8 byVideoEncID);

	/*====================================================================
	功能		: 请求终端发送摄像头返回串口数据
	输入参数说明: u8 bySendAble 发送串口数据使能(MTI_ENABLE－开始发送,
								需指定视频源端口号;MTI_DISABLE－停止发送,
								不需指定视频源端口号)
				  u8 byVideoInpId 视频源端口号
	返回值说明  : 成功返回操作流水号, 失败返回MTI_FAIL
	====================================================================*/
	u16 StartSendComData(u8 bySendAble, u8 byVideoInpId = 0);

	/*====================================================================
	功能		: 发送设备注册应答
	输入参数说明: u16 错误码
				  u8 byVideoInpId 视频源端口号
	返回值说明  : 成功返回操作流水号, 失败返回MTI_FAIL
	====================================================================*/
	u16 SendDevRegeditRsp(u16 wResultRsp);

#ifdef VSIP_MULTI_CHAN_SUPPORT
	u16 SetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 GetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 SaveDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
#endif

#ifdef VSIP_PERFORM_IMPROVE
	BOOL ConnectionCheck(TVsipGuid tVsipGuid);
#endif

    //Natagent-PU 码流调度接口
	//发送码流：实时浏览
    u16 NAStartSendMediaReq(TMTIStartSendMediaReq& tReq);
    u16 NAStartSendMediaAck(TMTIStartSendMediaAck& tAck);
    u16 NAStopSendMediaReq(TMTIStopSendMediaReq& tReq);
	//接收码流：音频呼叫和前端解码
    u16 NAStartRecvMediaReq(TMTIStartRecvMediaReq& tReq);
    u16 NAStartRecvMediaAck(TMTIStartRecvMediaAck& tAck);
    u16 NAStopRecvMediaReq(TMTIStopRecvMediaReq& tReq);
	//录像播放
    u16 NARecStartPlayReq(TMTIRecStartPlayReq& tReq);
    u16 NARecStartPlayAck(TMTIRecStartPlayAck& tAck);
	//录像下载
    u16 NARecStartExportReq(TMTIRecStartExportReq& tReq);

    //录像日历查询
	u16 QueryRecList(TMTIRecListQryReq& tReq);
	
public :
	u8 m_byCurUserType;		//当前用户登录类型 zlq add
	u8 m_byIdentifyResult;//验证结果
	void ClearUnitCtrl(); //释放设备控制但不关闭连接
	void ReleaseUnitCtrl( ); //释放设备控制关闭连接
	BOOL GetEntityType( TVsipGuid tVsipGuid, u8 & byType);	//获取能力类型
	BOOL GetEntityId( TVsipGuid tVsipGuid, u16 * byID);		//获取能力ID
	BOOL GetEntityGuid( u8 byType, u16 wId, TVsipGuid & tVsipGuid );//获取能力GUID
	void PrintData();

	void SetConnectStatus(BOOL32 bStatus)
	{
		m_bIsConnected = bStatus;
	}
	
	BOOL32 IsIdenticalApp(u16 wInstanceId);

	void SetClientUnitHandle( VSIP_HANDLE tClientHandle)
	{
		m_bIsConnected = TRUE;
		m_tVsipComHandle = tClientHandle;
	}

	VSIP_HANDLE GetVsipHandle( )
	{
		return m_tVsipComHandle;
	}
    u8  GetDeviceType();        //获取设备类型addby 20090903 G.J.
	u16 SendMessageBack( TMTIMsgHandle tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen ); //发送消息给用户
	void GetInterConfig(TMTIConfigInfo tConfInfo);//zlq add 内部配置获取参数,解决pui设置参数时因为异步操作造成的错误.
	u8 GetCurConnetction(){return m_tVsipComHandle.ConnectionType;};//zlq add 获取当前连接类型
//	void SetCapability(TMTICapability tCap){m_tCapability = tCap;};//zlq add 修改能力集.解决西安电信动态改变并口数的需求.其他情况下禁用!
protected:

	void SetConfigState( u8 byConfigType, u16 wEntityId, BOOL bIsValid);

	BOOL GetConfigState( u8 byConfigType, u16 wEntityId);

	void SaveCapabilities( u16 CapListCount, VSIP_IC_CAPABILITIES *pCapList );	//保存能力集

	void SaveConfigData( TVsipGuid tVsipGuid, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存配置参数

	void SaveDeviceConfigData( u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	// 保存设备参数

	//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
	void SaveVideoEncConfigData_WH( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存视频编码参数
	//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

	void SaveVideoEncConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存视频编码参数

	void SaveVideoDecConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存视频解码参数

	void SaveAudioEncConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存音频编码参数

	void SaveAudioDecConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存音频解码参数

	void SaveSerialPortConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存串口参数

	void SaveInputPinConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存并口输入端口参数

	void SaveInputPinStateData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存并口输入状态使能参数

	void SaveVideoInputConfigData( u16 wEntityId, u16 ConfigItemCount,
		VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );	//保存视频输入端口参数

	u16 GetConfigID(u16 wChnID); //获取配置属性addby 20090903 G.J.

	//{{{{{{{{{{{{{{{ add by Royan 20140723 获取视频参数增加回调方式
	//void SavePropConfigData( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );//保存属性配置参数
	void SavePropConfigData( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo, bool& bNeedSendBack);//保存属性配置参数
	//}}}}}}}}}}}}}}} add by Royan 20140723 获取视频参数增加回调方式

	void SavePropNetworkConfig( u16 wConfigId, u8 * pConfData );//保存网络配置参数

	void SavePropVsipConfig( u16 wConfigId, u8 * pConfData );//保存VSIP配置参数

	void SavePropSerialPortConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropAudioConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropVideoEncConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropVideoDecConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData );

	void SavePropWebConfig( u16 wConfigId, u8 * pConfData );

	void SavePropVINConfig( u8 byEntityId, u16 wConfigId, u8 * pConfData, bool& bNeedSendBack );
	friend int DiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo,
							   TVsipGuid tVsipGuid,u16 Fabricant, u16 Product, u8 StatusChanged );

	friend int GetConfigRspCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount,
							   VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );

	friend int GetCapabilityRspCB( VSIP_HANDLE VsipHandle, u16 CapListCount,
								   VSIP_IC_CAPABILITIES *pCapList );

	friend int GetProprietaryRspCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
									 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int OperationRspCB( VSIP_HANDLE VsipHandle, u16 ErrorCode );

	friend int EventNotifyCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 EventType, u32 EventArg );

	friend int DisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );

	friend void ConnectionCheckCB(void * pArg);

	friend int CommandExReqCB(VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u8 CommandCode, u32 Arg1,
		u32 Arg2, u16 AdditionalArgCount, VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	//前端录象存储start
    friend int GetRecStatusRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannelId,RecPlayerState byRecStatus);
    friend int GetNmsParamRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,TNmsParam NmsParam);
	friend int GetTimelyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,TimelyRecInfo TRecInfo);
	friend int GetPeriodlyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
	friend int GetChannelRecBasicInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,RecChannelBasicInfo & RChanBasicInfo);
	friend int GetRecSysInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,RecSysInfo RSysInfo);
	friend int QueryRecFileAckCB(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
    friend int QueryRecListRspCB(VSIP_HANDLE VsipHandle, TMTIRecListQryRsp tRecListRsp);
	friend int QueryRecFileNumAckCB(VSIP_HANDLE VsipHandle, u16 wNum);
	friend int ProgressCallCB(VSIP_HANDLE VsipHandle, ProgressParaInfo ParaInfo);
	friend int RecStatusChangeCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,RecPlayerState byRecStatus);
	friend int RecFileStartPlayidCB(VSIP_HANDLE VsipHandle,TMTIRecFilePlayNtf tPlayNtf);
	friend int FileExportProgressCB(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
	friend int VideoSourceStateCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,u8 byState);
	friend int DevRegeditReqCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,TDevRegeditInfo DevRegInfoPara);
	friend int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

    //daiyi transdata start
    friend int SetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
    friend int GetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
    //daiyi transdata end
    
    //2009/08/28 fanxg gps
	friend int GetTransCfgAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);

	//hzc_Transparent_Chan_20110324{{
	friend int StackOperationRspExCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//}}

	friend int StackOperationRspEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataRsp tMTITransDataRsp);

	friend int StackOperationNtfEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataNtf tMTITransDataNtf);

	friend int GetCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	
	friend int NotifyCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);

	friend int DevStreamPropRsp(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTIStartDevRspEx tMTIStreamProps);

	//shenxin_rec_file_down_notify{{
	friend int RecFileDownNotifyCB(VSIP_HANDLE VsipHandle,u16 wErrorCode,u16 wFtpPort);
	//}}

	friend int PlayGpsDataCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwErrcode,u8 byPlayId,u32 dwReserve);
	friend int PlayGpsDataNotifyCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TPlayGpsNotify tPlayGpsDataNotify);

	friend int StackTransSubsRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsRsp &tReq);
	friend int StackTransSubsNtfCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsNtf &tNtf);

	//kedacom potocal begin
	friend int GetDecoderStyleCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	friend int GetCustomExtCapsCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//kedacom potocal end

	void SaveNmsParam(TVsipGuid guid,TNmsParam NmsParam);
	void SaveRecStatus(TVsipGuid guid,u8 byChannelId,RecPlayerState byRecStatus);
	void SaveTimelyRecInfo(TVsipGuid guid,u8 byChannelId,TimelyRecInfo TRecInfo);
	void SavePeriodlyRecInfo(TVsipGuid guid,u8 byChannelId,PeriodlyTimeInfo PTimeInfo);
	void SaveChannelRecBasicInfo(TVsipGuid guid,u8 byChannelId,RecChannelBasicInfo RChanBasicInfo);
	void SaveRecSysInfo(TVsipGuid guid,RecSysInfo RSysInfo);
	u16 SaveSnapShotConfig(u8 byChannel, TMTISnapShotConfig tConfig) {m_aSnapShotConfig[byChannel] = tConfig; return MTI_SUCCESS;};
	u16 SaveMatrixConfig(const TMTIMatrixInfo &tConfig){m_MatrixParam = tConfig; return MTI_SUCCESS;};
	u16 SaveSnapShotNameRule(TMTISnapShotNamingRule tConfig) { m_tSnapNameRule = tConfig; return MTI_SUCCESS;};
	u16 SavePuNmsParam(TMTIPUNMSCFG tConfig){m_tPuNmsParam = tConfig; return MTI_SUCCESS;};
	//前端录象存储end

#ifdef VSIP_MULTI_CHAN_SUPPORT
	friend void GetDevMultiEncchanParamRespCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, TMultiChanInfo &MultiChanInfo);
#endif

	//add by Royan 20130201
public:
	u16 GetEnMotionDetectAreaNum(u16 wEntityId);
	u16 GetShadeAreaNum(u16 wEntityId);
	TMTIMotionDetectParam* GetEnMotionDetectParam(u16 wEntityId, u16 wIndex);
	TMTIShadeParam* GetShadeParam(u16 wEntityId, u16 wIndex);
	TMTISerialPortParam* GetSerialPortParam(u16 wEntityId);
	TMTIAudioEncParam* GetAudioEncParam(u16 wEntityId);
	TMTISysParam* GetSysParam();
	TMTIVideoInputParam* GetVideoInputParam(u16 wEntityId);


#ifdef linux  
	void LockMutex();
	void UnlockMutex();
#endif

private:
#ifdef linux                     //add 20150124，防止pui和mti同时释放资源导致coredump
	pthread_mutex_t m_mutex;
#endif
	//前端存储start
    //TimelyRecInfo m_aTimelyRecInfo[MAX_CHANNEL_NUM];
    map<u16, TimelyRecInfo> m_aTimelyRecInfo;
	//PeriodlyTimeInfo m_aPeriodTimeInfo[MAX_CHANNEL_NUM];
    map<u16, PeriodlyTimeInfo> m_aPeriodTimeInfo;
    //RecChannelBasicInfo m_aRecChannelBasicInfo[MAX_CHANNEL_NUM];
    map<u16, RecChannelBasicInfo> m_aRecChannelBasicInfo;
    RecSysInfo m_RecSysInfo;
	//RecPlayerState m_abyRecStatus[MAX_CHANNEL_NUM];
    map<u16, RecPlayerState> m_abyRecStatus;
    TNmsParam m_NmsParam;
	TMTIAlarmParam m_tAlarmParam;
	TMTIBannerParam m_tBannerParam;
	//TMTISnapShotConfig m_aSnapShotConfig[MAX_CHANNEL_NUM];
    map<u16, TMTISnapShotConfig> m_aSnapShotConfig;
	TMTISnapShotNamingRule m_tSnapNameRule;
	//前端存储end

	pMESSAGE_CB					m_pfuncMessageCB;
	void					*	m_pCallBackContext;
	TMTIInitInfo				m_tUnitInfo;
	//TMTICapItemInfo				m_aptEntityInfo[MAX_ENTITY_TOTAL_NUM];
    map<u16, TMTICapItemInfo>				m_aptEntityInfo;
    u16                         m_wEntityCount; //modify G.J. 20090903 m_byEntityCount -> m_wEntityCount
	TMTICapability				m_tCapability;
	TMTICAPSET					m_tExtCapablity;
	BOOL						m_bIsRebootRequired;
	BOOL						m_bIsConnected;
	//NEW
	//u16							m_awMotEnDetAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, u16>							m_awMotEnDetAreaNum;
	//u16							m_awMotDeDetAreaNum[MAX_VIDEO_ENC_NUM];
    map<u16, u16>							m_awMotDeDetAreaNum;

	//u64							m_addConfigState[PARAM_TYPE_MAX_VALUE]; //modify  G.J. 20090903 u32 --> u64
    map<u16, u64>							m_addConfigState;
	//u64							m_addConfigStateNvr128[PARAM_TYPE_MAX_VALUE]; //nvr128//modify by shenxin,用于保存64-128路状态
    map<u16, u64>							m_addConfigStateNvr128;
	VSIP_HANDLE					m_tVsipComHandle;	//zhuzhm:重要成员
													//所有发送接收API，都通过此句柄
													//在GetUnitCtrl时，装载句柄
	VSIP_HANDLE					m_tSerialDataHandle;
	u8							m_bySerialDataChnType;

	TMTISysParam				m_tSysParam;
	TMTISysPubParam				m_tSysPubParam;
	TMTIPPPOEParam				m_tPPPOEParam;

	//TMTICommonNetParam			m_atVideoEncNetParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTICommonNetParam>			m_atVideoEncNetParam;
	//TMTIVideoEncParam			m_atVideoEncParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncParam>			m_atVideoEncParam;
	TMTIVideoEncPubParam		m_tVideoEncPubParam;
	//TMTIVideoEncOSDParam		m_atVideoEncOSDParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncOSDParam>		m_atVideoEncOSDParam;
	//TMTIVideoEncMPParam			m_atVideoEncMPParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncMPParam>			m_atVideoEncMPParam;
	//TMTIVideoEncInputPortParam	m_atVideoEncInputPortParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncInputPortParam>	m_atVideoEncInputPortParam;
	//TMTIVideoEncPollParam		m_atVideoEncPollParam[MAX_VIDEO_ENC_NUM];
    map<u16, TMTIVideoEncPollParam>		m_atVideoEncPollParam;

	//TMTICommonNetParam			m_atVideoDecNetParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTICommonNetParam>			m_atVideoDecNetParam;
	//TMTIVideoDecParam			m_atVideoDecParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecParam>			m_atVideoDecParam;
	//TMTIVideoDecPubParam		m_atVideoDecPubParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecPubParam>		m_atVideoDecPubParam;
	//TMTIVideoDecRetransferParam	m_atVideoDecRetransferParam[MAX_VIDEO_DEC_NUM];
    map<u16, TMTIVideoDecRetransferParam>	m_atVideoDecRetransferParam;
	//NEW
	//TMTIMotionDetectParam       m_atDeMotionDetParamNew[MAX_VIDEO_DEC_NUM][MAX_DET_AREA_NUM];

    map<u16, map<u16, TMTIMotionDetectParam> >      m_atDeMotionDetParamNew;

	//TMTICommonNetParam			m_atAudioEncNetParam[MAX_AUDIO_ENC_NUM];
    map<u16, TMTICommonNetParam>			m_atAudioEncNetParam;
	//TMTIAudioEncParam			m_atAudioEncParam[MAX_AUDIO_ENC_NUM];
    map<u16, TMTIAudioEncParam>			m_atAudioEncParam;
	//TMTICommonNetParam			m_atAudioDecNetParam[MAX_AUDIO_DEC_NUM];
    map<u16, TMTICommonNetParam>			m_atAudioDecNetParam;
	//TMTIAudioDecParam			m_atAudioDecParam[MAX_AUDIO_DEC_NUM];
    map<u16, TMTIAudioDecParam>			m_atAudioDecParam;

	//TMTISerialPortParam			m_atSerialPortParam[MAX_SERIAL_PORT_NUM];
    map<u16, TMTISerialPortParam>			m_atSerialPortParam;
	//TMTISerialPortPubParam		m_atSerialPortPubParam[MAX_SERIAL_PORT_NUM];
    map<u16, TMTISerialPortPubParam>		m_atSerialPortPubParam;

	//TMTIInputPinParam			m_atInputPinParam[MAX_INPUT_PIN_NUM];
    map<u16, TMTIInputPinParam>			m_atInputPinParam;
	TMTIInputPinState           m_tInputPinEnableState; //20091020
	//TMTIVideoInputParam			m_atVideoInputParam[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, TMTIVideoInputParam>			m_atVideoInputParam;
	//NEW
	//TMTIMotionDetectParam       m_atEnMotionDetParamNew[MAX_VIDEO_INPUT_PORT_NUM][MAX_DET_AREA_NUM];
    map<u16, map<u16, TMTIMotionDetectParam> > m_atEnMotionDetParamNew;
    //TMTIShadeParam              m_atShadeParam[MAX_VIDEO_INPUT_PORT_NUM][MAX_SHADE_AREA_NUM];
    map<u16, map<u16, TMTIShadeParam> > m_atShadeParam;
	//u16				            m_awShadeAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
    map<u16, u16>				            m_awShadeAreaNum;
	//矩阵 zlq add
	TMTIMatrixInfo				m_MatrixParam;
	//网管相关配置
	TMTIPUNMSCFG				m_tPuNmsParam;
	
	BOOL32                      m_bIsNewVideoParamVersion; //新版本前端视频编码参数上报规则变更, 为兼容新旧前端故加以判断addby G.J.20090901
	//{{{{{{{{{{{{{{{ add by Royan 20140328 分辨率数值化
	BOOL32                      m_bIsNewVideoParamVersionWH;    //20130904
	//}}}}}}}}}}}}}}} add by Royan 20140328 分辨率数值化

	BOOL32                      m_bIsRcvInputPinEntity; //是否已经接收到并口能力上报 20091020
	u16                         m_wInputPinEntityStartID; //并口告警超始GUID 20091020

#ifdef VSIP_MULTI_CHAN_SUPPORT
	TDevMultiChanInfo			m_tDevMultiChanInfo;
#endif
	TMTIThirdPartyDecParam      m_tThirdPartyDecParam; //add 20100317
};

class CMTIClientCtrl
{
	//链路检测信息
	typedef struct TMTIClientConnCheckInfo
	{
		u8				byCounter;		//断链计数
		SOCKHANDLE		SocketId;		//链路套接字
	}PACKED TMTI_CLIENT_CONN_CHECK_INF, *PTMTI_CLIENT_CONN_CHECK_INF;

public:
	CMTIClientCtrl();
	~CMTIClientCtrl();

    //线程检测
//#ifndef _LINUXEQUATOR_
	u16 StartFileTransAck(VSIP_HANDLE VsipHandle,TFileTransRspInfo tFileTransRspInfo);
//#endif
    /*==================================================================
	功能        : 获取消息接收线程计数
	输入参数说明: 无
	返回值说明  : 消息接收线程计数
	==================================================================*/
    u32 GetMsgRcvCheckCount();
   /*==================================================================
	功能        : 获取定时器线程计数
	输入参数说明: 无
	返回值说明  : 消息定时器线程计数
	==================================================================*/
    u32 GetTimerCheckCount();
	/*==================================================================
	功能        : 关闭链路
	输入参数说明: ip+端口
	返回值说明  : 无
	==================================================================*/
	void CloseConnection(u32 dwIpAddr, unsigned short wPort);
//#ifndef _LINUXEQUATOR_
	void PrintDebug(int selection,int index);
	void PrintConnectionInfo();
	void PrintUsedCapBuf();
	void PrintCapBuf(int index);
	void PrintStackConnection(int index);
	void PrintEntityBuf(int index);
//#endif//_LINUXEQUATOR_
	/*==================================================================
	功能        : linux系统下获取定时器pid
	输入参数说明: 无
	返回值说明  : 定时器pid
	==================================================================*/
	u32 GetTimerThreadId();
	/*==================================================================
	功能        : linux系统下获取消息接收线程pid
	输入参数说明: 无
	返回值说明  : 消息接收线程pid
	==================================================================*/
	u32 GetMsgRcvThreadId();

	/*==================================================================
	功能        : 链路检测
	输入参数说明: ip地址和端口
	返回值说明  : MTI_SUCCESS-则表示成功发送ping消息; MTI_FAIL-则表示该链路不存在

	==================================================================*/
    BOOL ConnectionCheck(u32 dwIpAddr, unsigned short wPort);

	/*==================================================================
	功能        : 判断指定ip是否在链路表中
	输入参数说明: ip地址
	返回值说明  : MTI_SUCCESS-则表示链路存在; MTI_FAIL-则表示该链路不存在

	==================================================================*/
	BOOL FindConnection(u32 dwIpAddr);

	/*====================================================================
	功能		: 请求设备注册
	输入参数说明: u32 (mcu地址网络序，注册码）
				  u8 byVideoInpId 视频源端口号
	返回值说明  : 成功返回TRUE, 失败返回FALSE
	====================================================================*/
	BOOL DevRegedit(u32 dwIpAddr,TDevRegeditInfo DevRegInfoPara);

	//前端存储接口start
	int SetNmsParam(TNmsParam NmsParam);
	/*==================================================================
	功能        : 设置录象器状态
	输入参数说明: 通道id，录象状态
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
//#ifndef _LINUXEQUATOR_
    u16 SetRecStatus(u8 byChannelId,RecPlayerState byRecStatus);
	/*==================================================================
	功能        : 设置定时录象信息状态
	输入参数说明: 通道id，定时录象信息
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	 u16 SetTimelyRecInfo(u8 byChannel, TimelyRecInfo TRecInfo);
	/*==================================================================
	功能        : 设置周期录象信息状态
	输入参数说明: 通道id，周期录象信息
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
     u16 SetPeriodlyRecInfo (u8 byChannel, PeriodlyTimeInfo PTimeInfo);
	 /*==================================================================
	功能        : 设置通道信息
	输入参数说明: 通道id，通道基本信息
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
     u16 SetChannelRecBasicInfo(u8 byChannel, RecChannelBasicInfo RChannelBasicInfo);
	 /*==================================================================
	功能        : 设置录象系统系统
	输入参数说明: 录象系统系统
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	 u16 SetRecSysInfo(RecSysInfo RSysInfo);
	 /*==================================================================
	功能        : 系统信息通知
	输入参数说明: 
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	 u16 SysStateNotify(u32 nSysArg);
	 /*==================================================================
	功能        : 发送查询信息
	输入参数说明: 查询参数
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
    int QueryAck(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
    /*==================================================================
	功能        : 发送查询到的个数信息
	输入参数说明: 个数
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	int QueryNumAck(VSIP_HANDLE VsipHandle,u16 wNum);    
	/*==================================================================
	功能        : 进度上报
	输入参数说明: 进度参数
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	int ProgressResult(VSIP_HANDLE VsipHandle,ProgressParaInfo ParaInfo);
	/*==================================================================
	功能        : 打印录象器信息
	输入参数说明: 通道号
	返回值说明  : 无
	==================================================================*/
	void PrintRecInfo(u8 byChannel);
	/*==================================================================
	功能        : 发送playerid
	输入参数说明: handle， id号
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
    int SendPlayerId(VSIP_HANDLE VsipHandle , u8 byPlayerId);
	/*==================================================================
	功能        : 发送mt端文件下载的ip地址和端口号
	输入参数说明: handle， id号
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
    int FileExportProgress(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
//#endif
	/*==================================================================
	功能        : 发送无视频源通知
	输入参数说明: 通道号
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	u16 SetVideoSoureState(u8 byChannel,u8 byState);
	//前端存储接口end
	/*==================================================================
	功能        : 初始化
	输入参数说明: 无
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	u16 Init(TMTISysPubParam tSysPubPara,u16 wMaxOnlineMtNum = 5,u16 wSendPort = 60000, u16 wFtpPort = 1730 );

	BOOL ForwordConnection(u32 dwIpAddr, u16 wPort);

	/*==================================================================
	功能        : 设置链路检测间隔时间间隔
	输入参数说明: u16 wCheckInterval 链路检测间隔时间间隔 单位ms
	返回值说明  : 无
	其它说明	: 在初始化成功后调用
	==================================================================*/
	void SetConnectionCheckInterval( u16 wCheckInterval = 30000 ){};

	/*==================================================================
	功能        : 设置消息回调函数
	输入参数说明: u32 dwLocalIpAddr 本机IP地址(网络序)
				  u32 dwMultiIpAddr VSIP组播地址(网络序)
				  u16 wVsipPort VSIP端口号
	返回值说明  : TRUE-成功; FALSE-失败
	==================================================================*/
	BOOL SetMessageCallBack( pMESSAGE_CB pFunc, void* pContext = NULL );

	/*==================================================================
	功能        : 设置设备能力集
	输入参数说明: const TMTICapability & tCapability 设备能力集
	返回值说明  : 无
	==================================================================*/
	void SetCapabilities( const TMTIPUCapability & tCapability, const TVsipGuid & tDeviceGuid);

	/*==================================================================
	功能        : 设置设备系统参数
	输入参数说明: const TMTISysParam & tCfgParam 设备系统参数
	返回值说明  : 无
	==================================================================*/
	void SetSysCfg( const TMTISysParam & tCfgParam);
//#ifndef _LINUXEQUATOR_
	/*==================================================================
	功能        : 设置告警配置参数(西安电信)
	输入参数说明: const TMTISysParam & tCfgParam 设备系统参数
	返回值说明  : 无
	==================================================================*/
	void SetAlarmParamCfg( const TMTIAlarmParam & tCfgParam);
	/*==================================================================
	功能        : 设置横幅参数(西安电信)
	输入参数说明: const TMTISysParam & tCfgParam 设备系统参数
	返回值说明  : 无
	==================================================================*/
	void SetBannerParamCfg( const TMTIBannerParam & tCfgParam);
//#endif //_LINUXEQUATOR_
	/*==================================================================
	功能        : 设置PPPOE参数
	输入参数说明: const TMTIPPPOEParam & tCfgParam 设备系统参数
	返回值说明  : 无
	==================================================================*/
	void SetPPPOECfg(const TMTIPPPOEParam & tCfgParam);

	/*==================================================================
	功能        : 设置系统公共参数
	输入参数说明: const TMTISysPubParam & tCfgParam 系统公共参数
	返回值说明  : 无
	==================================================================*/
	void SetSysPubCfg( const TMTISysPubParam & tCfgParam);

	/*==================================================================
	功能        : 设置视频编码网络参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTICommonNetParam & tCfgParam 视频编码网络参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncNetCfg( u8 byVideoEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置视频编码参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncParam & tCfgParam 视频编码参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncCfg( u8 byVideoEncID, const TMTIVideoEncParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码公共参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncPubParam & tCfgParam 视频编码公共参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncPubCfg( const TMTIVideoEncPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码字幕参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncOSDParam & tCfgParam 视频编码字幕参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncOSDCfg( u8 byVideoEncID, const TMTIVideoEncOSDParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码画面合成参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncMPParam & tCfgParam 视频编码画面合成参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncMPCfg( u8 byVideoEncID, const TMTIVideoEncMPParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码视频源端口参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncInputPortParam & tCfgParam 视频编码视频源端口参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncInputPortCfg( u8 byVideoEncID, const TMTIVideoEncInputPortParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频编码轮询参数
	输入参数说明: u8 byVideoEncID 视频编码通道号
				  const TMTIVideoEncPollParam & tCfgParam 视频编码轮询参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoEncPollCfg( u8 byVideoEncID, const TMTIVideoEncPollParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码网络参数
	输入参数说明: u8 byVideoEncID 视频解码通道号
				  const TMTICommonNetParam & tCfgParam 视频解码网络参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoDecNetCfg( u8 byVideoDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置视频解码参数
	输入参数说明: u8 byVideoEncID 视频解码通道号
				  const TMTIVideoDecParam & tCfgParam 视频解码参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoDecCfg( u8 byVideoDecID, const TMTIVideoDecParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码公共参数
	输入参数说明: u8 byVideoEncID 视频解码通道号
				  const TMTIVideoDecPubParam & tCfgParam 视频解码公共参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoDecPubCfg( u8 byVideoDecID, const TMTIVideoDecPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频解码重传参数
	输入参数说明: u8 byVideoEncID 视频解码通道号
				  const TMTIVideoDecRetransferParam & tCfgParam 视频解码重传参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoDecRetransferCfg( u8 byVideoDecID, const TMTIVideoDecRetransferParam & tCfgParam );

	/*==================================================================
	功能        : 设置音频编码网络参数
	输入参数说明: u8 byAudioEncID 音频编码通道号
				  const TMTICommonNetParam & tCfgParam 音频编码网络参数
	返回值说明  : 无
	==================================================================*/
	void SetAudioEncNetCfg( u8 byAudioEncID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置音频编码参数
	输入参数说明: u8 byAudioEncID 音频编码通道号
				  const TMTIAudioEncParam & tCfgParam 音频编码参数
	返回值说明  : 无
	==================================================================*/
	void SetAudioEncCfg( u8 byAudioEncID, const TMTIAudioEncParam & tCfgParam ); //设置音频编码参数

	/*==================================================================
	功能        : 设置音频解码网络参数
	输入参数说明: u8 byAudioEncID 音频编码通道号
				  const TMTICommonNetParam & tCfgParam 音频解码网络参数
	返回值说明  : 无
	==================================================================*/
	void SetAudioDecNetCfg( u8 byAudioDecID, const TMTICommonNetParam & tCfgParam);

	/*==================================================================
	功能        : 设置音频解码参数
	输入参数说明: u8 byAudioEncID 音频编码通道号
				  const TMTIAudioDecParam & tCfgParam 音频解码参数
	返回值说明  : 无
	==================================================================*/
	void SetAudioDecCfg( u8 byAudioDecID, const TMTIAudioDecParam & tCfgParam );

	/*==================================================================
	功能        : 设置串口参数
	输入参数说明: u8 bySerialPortID 串口号
				  const TMTISerialPortParam & tCfgParam 串口参数
	返回值说明  : 无
	==================================================================*/
	void SetSerialPortCfg( u8 bySerialPortID, const TMTISerialPortParam & tCfgParam );

	/*==================================================================
	功能        : 设置串口公共参数
	输入参数说明: u8 bySerialPortID 串口号
				  const TMTISerialPortPubParam & tCfgParam 串口公共参数
	返回值说明  : 无
	==================================================================*/
	void SetSerialPortPubCfg( u8 bySerialPortID, const TMTISerialPortPubParam & tCfgParam );

	/*==================================================================
	功能        : 设置并口输入参数
	输入参数说明: u16 wInputPinID 并口输入端口号
				  const TMTIInputPinParam & tCfgParam 并口输入参数
	返回值说明  : 无
	==================================================================*/
	void SetInputPinCfg( u16 wInputPinID, const TMTIInputPinParam & tCfgParam );

	/*==================================================================
	功能        : 设置视频源端口参数
	输入参数说明: u8 byVideoInputID 视频源端口号
				  const TMTIVideoInputParam & tCfgParam 视频源端口参数
	返回值说明  : 无
	==================================================================*/
	void SetVideoInputCfg( u8 byVideoInputID, const TMTIVideoInputParam & tCfgParam );

	/*==================================================================
	功能        : 设置移动侦测参数
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
				  TMTIMotionDetectParam * tCfgParam 移动侦测参数
				  u16 wDetAreaNum 侦测区域数
	返回值说明  : 成功返回 MTI_SUCCESS; 失败返回 MTI_FAIL
	==================================================================*/
	void SetMotionDetectCfg( u8 byType, u16 wEntityID, u16 wDetAreaNum, const TMTIMotionDetectParam *tCfgParam );


	/*==================================================================
	功能        : 设置图象屏蔽参数
	输入参数说明: u16 wEntityID 视频源端口ID(从0开始)
				  const TMTIShadeParam & tCfgParam 设置的参数
	返回值说明  : 无
	==================================================================*/
	void SetShadeCfg(u16 wEntityID, u16 wShadeAreaNum, const TMTIShadeParam *tCfgParam );
	/*==================================================================
	功能        : 设置设备是否需要重启
	输入参数说明: BOOL bIsRebootRequired TRUE-需要重启; FALSE-不需要重启
	返回值说明  : 无
	==================================================================*/
	void SetIsRebootRequired( BOOL bIsRebootRequired );

	/*==================================================================
	功能        : 发送并口输入状态
	输入参数说明: u16 wInputPinID 并口输入端口号
				  BOOL bIsEnable 并口输入状态
	返回值说明  : 无
	==================================================================*/
	void SendInputPinState( u16 wInputPinID, BOOL bIsEnable );

	void SendInputPinExtendInfo(u16 wInputPinID,u8 byEventType,u16 wLength,char *pabyBuf);

	u16 SendTransparentData(VSIP_HANDLE tVsipHandle,char *Buf,u16 wBufLen);

	/*==================================================================
	功能        : 发送并口输出状态
	输入参数说明: u16 wOutputPinID 并口输出端口号
				  BOOL bIsEnable 并口输出状态
	返回值说明  : 无
	==================================================================*/
	void SendOutputPinState( u16  wOutputPinID, BOOL bIsEnable );
	/*==================================================================
	功能        : 发送解码通道有无码流状态
	输入参数说明: u8 DecodeChannelId 解码通道端口号
				  BOOL bActive 有无码流
	返回值说明  : 无
	==================================================================*/
	void SendIsBitCurrentState(u8 DecodeChannelId, BOOL bActive);

	/*==================================================================
	功能        : 发送移动侦测告警状态
	输入参数说明: u8 byType 移动侦测类型(MTI_ENCODER-编码;MTI_DECODER-解码)
				  u16 wEntityID 移动侦测实体ID(编码为视频源端口号, 解码为解码器通道号)
				  u16 wAlarmAreaNum 移动侦测告警的区域号
				  BOOL bIsEnable 移动侦测状态
	返回值说明  : 无
	==================================================================*/
	void SendMotionDetectState( u8 byType, u16 wEntityID, u16 wAlarmAreaNum ,BOOL bIsEnable );

	/*==================================================================
	功能        : 返回操作结果
	输入参数说明: const TMTIMsgHandle & tMsgHandle 消息句柄
				  u16 wResult 操作结果(MTI_SUCCESS, MTI_FAIL)
	返回值说明  : 无
	==================================================================*/
	void SendOperationResult( const TMTIMsgHandle & tMsgHandle, u16 wResult );

	/*==================================================================
	功能        : 返回串口数据
	输入参数说明: u8 byVideoInputID 视频源端口号
				  u8 *pbySerData	串口数据
				  u16 wDataLen		数据长度
	返回值说明  : 无
	==================================================================*/
	void SendSerialDataNotify( u8 byVideoInputID, u8 *pbySerData, u16 wDataLen);
	void SendSerialDataNotifyEX( u8 byEntityId, u8 byEntityType, u8 *pbySerData, u16 wDataLen);
	/*==================================================================
	功能        : 是否主动建链
	输入参数说明:
	返回值说明  : 无
	==================================================================*/
	BOOL IsConnectForwardly();

	void CloseStack();

	u16 GetGuidReq(u32 dwIp,u8 byLocalSerialId,u8 byTargetSerialId);

	void SetGetGuidCallBack( pGetGuidByIp_CB pFunc);

	BOOL CreatTransparentChannel(u32 dwIp,TVsipGuid tGuid,u8 byConnectionType);

	u16 SendVideoDynVal(u8 byChanId,u8 byVidFmt, u8 byDynVal );

	u16 ChangePassWordAck(VSIP_HANDLE tVsipHandle,unsigned short ErrorCode);	//zlq add 修改密码
//#ifndef _LINUXEQUATOR_
	u16 SetBmpOsdAck(VSIP_HANDLE tVsipHandle,TMT_BMPOSDACK_INFO tBmpAckInfo);	//zlq add
	int QueryPicFileAck(VSIP_HANDLE tVsipHandle, TMTIQueryPicFileInfo QueryInfo); //zlq add
	int QueryPicFileNumAck(VSIP_HANDLE tVsipHandle, u16 wNum);
	int DownLoadPicFileAck(VSIP_HANDLE tVsipHandle, TMTIDownloadPicFileInfo tPicInfo);
	int DelPicFileAck(VSIP_HANDLE tVsipHandle, TMTISnapShotResult tCfg);
	int SetSnapShotConfigAck(VSIP_HANDLE tVsipHandle, u16 wError);
	int SetSnapShotConfig(u8 byChanId,TMTISnapShotConfig tConfig);
	int SetSnapShotNameRule(TMTISnapShotNamingRule tConfig);
	u16 StartSnapShotAck(VSIP_HANDLE tVsipHandle, u16 wError);
	u16 StopSnapShotAck(VSIP_HANDLE tVsipHandle, u16 wError);
	u16 SendUserTcpMSg(VSIP_HANDLE tVsipHandle, u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
	u16 DirectTransChanChangeMsg(VSIP_HANDLE tVsipHandle);
	u16 CapbilityChangeMsg(TMTICapability tCapability);
	u16 SetMatrixParam(const TMTIMatrixInfo &tConfig);
	u16 SetMatrixConfigAck(VSIP_HANDLE tVsipHandle, u16 wError);
	void SendRecStateAlarm(u8 ChanID, u16 ErrorCode);
	void SendVideoInputState(u8 ChanID, BOOL bError);
	VSIP_HANDLE GetForwordVsipHandle();//zlq add
//#endif //_LINUXEQUATOR_
	VSIP_HANDLE GetTransparentHandle();

#ifdef VSIP_MULTI_CHAN_SUPPORT
	u16 SetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
	u16 GetDevMultiEncchanParam (u8 byChannelId, TMultiChanInfo &MultiChanInfo);
#endif

protected:
	void SetConfigState( u8 byConfigType, u16 wEntityId, BOOL bIsValid);
	BOOL GetConfigState( u8 byConfigType, u16 wEntityId);

	u16 SendMessageBack( const TMTIMsgHandle & tMsgHandle, u8 * pbyMsgBuf, u16 wBufLen ); //发送消息给用户

	BOOL GetEntityType( TVsipGuid tVsipGuid, u8 & byType);	//获取能力类型
	BOOL GetEntityId( TVsipGuid tVsipGuid, u16 * wID);		//获取能力ID
	BOOL GetEntityGuid( u8 byType, u16 wId, TVsipGuid & tVsipGuid );//获取能力GUID

	//前端存储接口start
    u16 GetRecStatus(TVsipGuid tVsipGuid,u8 byChannelId,RecPlayerState & byRecStatus);
	u16 GetTimelyRecInfo(TVsipGuid tVsipGuid,u8 byChannelId,TimelyRecInfo & TRecInfo);
	u16 GetPeriodlyRecInfo(TVsipGuid tVsipGuid,u8 byChannelId,PeriodlyTimeInfo & PTimeInfo);
	u16 GetChannelRecBasicInfo(TVsipGuid tVsipGuid,u8 byChannelId,RecChannelBasicInfo & RChanBasicInfo);
	u16 GetRecSysInfo(TVsipGuid tVsipGuid,RecSysInfo & RSysInfo);
	u16 GetNmsParam(TVsipGuid tVsipGuid,TNmsParam & NmsParam);
	TMTISnapShotConfig GetSnapShotConfig(u8 byChanId){return m_aSnapShotConfig[byChanId];}; //zlq add
	TMTISnapShotNamingRule GetSnapShotNameRule(){return m_tSnapShotNameRule;};
	TMTIMatrixInfo GetMatrixConfig(){return m_MatrixParam;};
	//前端存储接口end

	u16 GetConfig( TVsipGuid tVsipGuid, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen); //获取设备参数

	u16 GetDeviceConfigData( u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取设备参数

	u16 GetVideoEncConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取视频编码参数

	u16 GetVideoDecConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
		   		  char achBuf[], u16 wBufLen );	//获取视频解码参数

	u16 GetAudioEncConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取音频编码参数

	u16 GetAudioDecConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取音频解码参数

	u16 GetSerialPortConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取串口参数

	u16 GetInputPinConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取并口输入端口参数

	u16 GetVideoInputConfigData( u16 wEntityId, u16 wConfigItemCount, u16 *pConfigItemId,
			  	  char achBuf[], u16 wBufLen );	//获取视频输入端口参数

	u16 GetPropConfig( u16 ConfigItemCount, VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG *pConfigItemId,
				   	  char achBuf[], u16 wBufLen); //获取设备属性参数

	u16 GetPropNetworkCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVsipCfg( u16 wConfId, char * pchBuf );

	u16 GetPropSysCfg( u16 wConfId, char * pchBuf );

	u16 GetPropSerialPortCfg( u16 wConfId, char * pchBuf );

	u16 GetPropAudioCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoEncCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoDecCfg( u16 wConfId, char * pchBuf );

	u16 GetPropOsdCfg( u16 wConfId, char * pchBuf );

	u16 GetPropWebCfg( u16 wConfId, char * pchBuf );

	u16 GetPropVideoInCfg( u16 wConfId, char * pchBuf );

	//设置设备参数
	u16 SetConfigReq( TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid, u16 wConfigItemCount,
					  VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo);

	BOOL SetDevConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVENConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetAENConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetADEConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetSPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetINPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetOUTPConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVINConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	BOOL SetVDEConfig( TMTIMsgHandle & tMsgHandle, u16 wEntityId, u16 wConfigItemCount, u8 * pbyConfigData);

	u16 SetDevGuidReq(TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid);

	BOOL SetDevGuidConfig(TMTIMsgHandle & tMsgHandle, TVsipGuid tVsipGuid);


	//设置设备属性参数
	u16 SetPropConfigReq( TMTIMsgHandle tMsgHandle, VSIP_GUID tVsipGuid, u16 wConfigItemCount,
						 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo);
	BOOL SetPropSysCfg( TMTIMsgHandle tMsgHandle, TMTISysPubParam &tSysPubParam, u16 wConfigId, u8 byConfigtype, u16 &wResult, u8 *pConfData);
	BOOL SetPropNetworkCfg( TMTISysPubParam & tSysPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVsipCfg( TMTISysPubParam &tSysPubParam, u16 wConfigId, u8 *pConfData);
	BOOL SetPropSerialPortCfg( TMTISerialPortPubParam & tSerialPortPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropAudioEncNetCfg( TMTICommonNetParam & tAudioEncNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropAudioDecNetCfg( TMTICommonNetParam & tAudioDecNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncNetCfg( TMTICommonNetParam & tVideoEncNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncPubCfg( TMTIVideoEncPubParam & tVideoEncPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncOSDCfg( TMTIVideoEncOSDParam & tVideoEncOSDParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncMPCfg( TMTIVideoEncMPParam & tVideoEncMPParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoEncPollCfg( TMTIVideoEncPollParam & tVideoEncPollParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecNetCfg( TMTICommonNetParam & tVideoDecNetParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecRTCfg( TMTIVideoDecRetransferParam & tVideoDecRetransferParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoDecPubCfg( TMTIVideoDecPubParam & tVideoDecPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropWebCfg( TMTISysPubParam & tSysPubParam, u16 wConfigId, u8 * pConfData );
	BOOL SetPropVideoInCfg( TMTIVideoInputParam & tVideoInputParam, u16 wConfigId, u8 * pConfData );


	u16 GetCapabilities( VSIP_IC_CAPABILITIES *pCapList, u16 wMaxCapCount); //获取设备能力集
	void GetCapability( VSIP_IC_CAPABILITIES *pCapList, u16 wCapCount, u8 byCapType ); //获取设备能力集

	//前端业务存储接口
	friend int StartRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int StopRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int SetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,TimelyRecInfo TRecInfo);
	friend int GetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int GetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int GetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int GetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
	friend int SetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
	friend int SetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,RecChannelBasicInfo RChannelBasicInfo);
	friend int SetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,RecSysInfo RSysInfo);
	friend int QueryRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
    friend int QueryRecFileNumReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
	friend int StartExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u32 dwIpAddress, u16 wPort,u8 byConnectionType);
	friend int StopExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int RecFileOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId,u8 byOperationType);
	friend int DelRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u8 byType);
	friend int DelRecRecordReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byType);
	friend int SeekLocationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId, u32 dwDragTime);
	friend int GetRecStatusReqCB( VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannelId);
	friend int RecFileStartPlayReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TRecStartPlayInfo tCfgParam);
	friend int SetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TNmsParam NmsParam);
	friend int GetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
	friend int DevRegeditRspCB(VSIP_HANDLE VsipHandle,u16 wResultRsp);
	friend int MtStackOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,
		u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);
	//前端存储end

	friend int AppContentTypeRspCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, u8 SwitchResult );
	friend int TimerDeadCB(VSIP_HANDLE VsipHandle);
    friend int SetDevGuidReqCB(VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid);
	friend int ClientDisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );
	friend int ClientReConnectCB( VSIP_HANDLE VsipHandle );
	friend int DiscoveryReqCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo );
	friend int ContentTypeReqCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, TVsipGuid DstGuid, u8 ContentType );
	friend int GetConfigReqCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount, u16 *pConfigItemId );
	friend int GetCapabilityReqCB( VSIP_HANDLE VsipHandle );
	friend int GetProprietaryReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
						VSIP_MEDIA_CONTROL_GETPROPRIETARY_CONFIG *pConfigItemId );
	friend int SetConfigReqCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 wConfigItemCount,
						VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );
	friend int SetProprietaryReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u16 wConfigItemCount,
						VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int StartDeviceReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u32 TxIpAddress,
								 u16 TxPort, u16 RxPort, u8 ConnectType );

	friend int StopDeviceReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid );

	friend int SendCommandReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u8 CommandCode,
								 u32 Arg1, u32 Arg2 );

	friend int SendCommandExReqCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u8 CommandCode,
								   u32 Arg1, u32 Arg2, u16 AdditionalArgCount,
								   VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	friend int SerialPortDataCB( VSIP_HANDLE VsipHandle, u8 * Buf, u16 BufLen );

	friend void CheckConnectionLostlist(void * pParam);

	friend int MtStackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int ClientDiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo, TVsipGuid tVsipGuid,
						  u16 Fabricant, u16 Product, u8 StatusChanged );

	void AddCapabilitiesInfo( u16 wEntityNum, u8 byEntityType);

#ifdef VSIP_MULTI_CHAN_SUPPORT
	friend void GetDevMultiEncchanParamReqCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid);
	friend void SetDevMultiEncchanParamReqCB (VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, TMultiChanInfo &MultiChanInfo);
#endif

public:
	u16 GetFtpPort() { return m_wFtpPort; }

private:

	u32                         m_dwRealConnIP; //终端与MCU等建链实际所用IP地址
	u16                         m_wPort;//终端与MCU等建链实际所用port
	u16							m_wFtpPort;//下载
	pMESSAGE_CB					m_pfuncMessageCB;
	pGetGuidByIp_CB             m_pfuncGetGuidCB;
	void					*	m_pCallBackContext;
	TVsipGuid					m_tDeviceGuid;
	VSIP_HANDLE					m_tAllClientHandle;
	VSIP_HANDLE					m_tClientHandle;
	TMTICapItemInfo			*	m_aptEntityInfo[MAX_ENTITY_TOTAL_NUM];
    u16						 	m_wEntityCount; //modify  G.J. 20090903 m_byEntityCount ->m_wEntityCount
	TMTIPUCapability			m_tCapability;
	BOOL						m_bIsRebootRequired;
	VSIP_HANDLE                 m_tTransparentHandle;

	BOOL                        m_bConnThreadOn;
	u16							m_awMotEnDetAreaNum[MAX_VIDEO_INPUT_PORT_NUM];
	u16							m_awMotDeDetAreaNum[MAX_VIDEO_ENC_NUM];

	u32							m_adwConfigState[PARAM_TYPE_MAX_VALUE];

	TMTISysParam				m_tSysParam;
	TMTISysPubParam				m_tSysPubParam;
	TMTIPPPOEParam				m_tPPPOEParam;

	TMTICommonNetParam			m_atVideoEncNetParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncParam			m_atVideoEncParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncPubParam		m_tVideoEncPubParam;
	TMTIVideoEncOSDParam		m_atVideoEncOSDParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncMPParam			m_atVideoEncMPParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncInputPortParam	m_atVideoEncInputPortParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoEncPollParam		m_atVideoEncPollParam[MAX_VIDEO_ENC_NUM];

	TMTICommonNetParam			m_atVideoDecNetParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecParam			m_atVideoDecParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecPubParam		m_atVideoDecPubParam[MAX_VIDEO_ENC_NUM];
	TMTIVideoDecRetransferParam	m_atVideoDecRetransferParam[MAX_VIDEO_ENC_NUM];

	TMTIMotionDetectParam    m_atDeMotionDetParamNew[MAX_VIDEO_DEC_NUM][MAX_DET_AREA_NUM];

	TMTICommonNetParam			m_atAudioEncNetParam[MAX_VIDEO_ENC_NUM];
	TMTIAudioEncParam			m_atAudioEncParam[MAX_VIDEO_ENC_NUM];
	TMTICommonNetParam			m_atAudioDecNetParam[MAX_VIDEO_ENC_NUM];
	TMTIAudioDecParam			m_atAudioDecParam[MAX_VIDEO_ENC_NUM];

	TMTISerialPortParam			m_atSerialPortParam[MAX_SERIAL_PORT_NUM];
	TMTISerialPortPubParam		m_atSerialPortPubParam[MAX_SERIAL_PORT_NUM];

	TMTIInputPinParam			m_atInputPinParam[MAX_INPUT_PIN_NUM];
	TMTIVideoInputParam			m_atVideoInputParam[MAX_VIDEO_INPUT_PORT_NUM];

	TMTIMotionDetectParam    m_atEnMotionDetParamNew[MAX_VIDEO_INPUT_PORT_NUM][MAX_DET_AREA_NUM];
    TMTIShadeParam           m_atShadeParam[MAX_VIDEO_INPUT_PORT_NUM][MAX_SHADE_AREA_NUM];
    u16				m_awShadeAreaNum[MAX_VIDEO_INPUT_PORT_NUM];

	u32 m_dwConnectionCheckTimerId;
	u32 m_dwCheckSyncTimerId;

	//前端存储start
    TimelyRecInfo m_aTimelyRecInfo[MAX_CHANNEL_NUM];
	PeriodlyTimeInfo m_aPeriodTimeInfo[MAX_CHANNEL_NUM];
    RecChannelBasicInfo m_aRecChannelBasicInfo[MAX_CHANNEL_NUM];
    RecSysInfo m_RecSysInfo;
	RecPlayerState m_abyRecStatus[MAX_CHANNEL_NUM];
	TNmsParam m_NmsParam;
	TConnectionLostList *m_ptConnetionLostList;
	TMTIAlarmParam m_tAlarmParam;
	TMTIBannerParam m_tBannerParam;
	TMTISnapShotConfig m_aSnapShotConfig[MAX_CHANNEL_NUM];
	TMTISnapShotNamingRule m_tSnapShotNameRule;
	//前端存储end
	TMTIMatrixInfo				m_MatrixParam;

#ifdef VSIP_MULTI_CHAN_SUPPORT
	TDevMultiChanInfo			m_tDevMultiChanInfo;
#endif
};


typedef struct TMTINetParam
{
	u32 dwIpAddr;	//IP地址(网络序)
	u16 wPort;		//端口号(1024-65535)
}PACKED TMTI_NET_PARAM, *PTMTI_NET_PARAM;

class CMTIUnitManager
{
	typedef struct TMTIUnitListInfo
	{
		eUnitState		emUnitState; //设备状态
		TMTIDiscoveryCBInfo	tDiscoveryInfo;	//设备系统参数
	}PACKED TMTI_UNIT_LIST_INF, *PTMTI_UNIT_LIST_INF;


	/*
	typedef struct TMTIClientUnitListInfo
	{ //client
		u16             wContentTime;   //建链后交换链路类型时间
		eConnState		emConnState;	//链接状态
		TVsipGuid		tDevGuid;		//设备GUID
		VSIP_HANDLE		VsipHandle;
	}PACKED TMTI_CLIENT_UNIT_LIST_INF, *PTMTI_CLIENT_UNIT_LIST_INF;
    */
	//链路检测信息
	typedef struct TMTIConnCheckInfo
	{
		u8				byCounter;		//断链计数
		TVsipGuid		tDevGuid;		//设备GUID
		SOCKHANDLE      ConnSock;       //链路套接字
	}PACKED TMTI_CONN_CHECK_INF, *PTMTI_CONN_CHECK_INF;

public:

	/*==================================================================
	功能        : 构造函数
	输入参数说明: 无
	返回值说明  : 无
	==================================================================*/
	CMTIUnitManager();

	/*==================================================================
	功能        : 析构函数
	输入参数说明: 无
	返回值说明  : 无
	==================================================================*/
	~CMTIUnitManager();

	/*==================================================================
	功能        : 初始化
	输入参数说明: u32 dwConnIpAddr 建立连接IP地址
	              u32 dwBindIPAddr 本机IP地址
				  u32 dwMultiIpAddr 搜索用组播地址
				  u16 wVsipPort VSIP端口
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	u16 Init( u32 dwConnIpAddr, u32 dwBindIPAddr, u32 dwMultiIpAddr = 0, u16 wVsipPort = DEFAULT_VSIP_PORT,u16 wMaxOnlineMtNum = MAX_UNIT_NUM);

	void Close();

	void Terminate();
    void PrintCapBuf(int i);

	void PrintUsedCapBuf();

    void PrintEntityBuf(int k);

    u32 GetMsgRcvCheckCount();

    u32 GetTimerCheckCount();

	/*==================================================================
	功能        : 开启消息发送线程
	输入参数说明: unsigned int dwMaxNum消息队列长度
				  time_t dwTimeout消息发送超时时间
				  void (*pfunCB)(const void *,const void *)有错误发生时的回调函数，
				                                           第一个指针指向TMTIMsgHandle 里面包括错误号和原消息发送句柄(即不包含消息内容)
														   第二个指针指向pContent
				  void *pContent回调时传给业务的指针
	返回值说明  : 无
	==================================================================*/
	void StartSendThread(unsigned int dwMaxNum,time_t dwTimeout,void (*pfunCB)(const void *,const void *),void *pContent);

	/*==================================================================
	功能        : 设置搜索结果回调
	输入参数说明: pDISCOVERY_CB pFunc 搜索结果回调函数
				  void* pContext 搜索回调上下文
	返回值说明  : 无
	==================================================================*/
	void SetDiscoveryCallBack( pDISCOVERY_CB pFunc, void* pContext = NULL );

	void SetNewDiscoveryCallBack(pNEWDISCOVERY_CB pFunc, void* pContext = NULL );

	/*==================================================================
	功能        : 注销关闭协议栈回调
	返回值说明  : 无
	==================================================================*/

    void SetTerminateMTCallBack(pTERMINATED_CB pFunc);
	/*==================================================================
	功能        : 设置终端建链回调
	输入参数说明: pCONNECTED_CB pFunc 终端建链回调函数
				  void* pContext 回调函数上下文
	返回值说明  : 无
	==================================================================*/
	void SetConnectedCallBack( pCONNECTED_CB pFunc, void* pContext = NULL );
    /*==================================================================
	功能        : 设置链路检测回调
	输入参数说明: pCONNECTIONCHECK_CB pFunc 链路检测回调函数
				  void* pContext 回调函数上下文
	返回值说明  : 无
	==================================================================*/
    void SetConnectionCheckCallBack( pCONNECTIONCHECK_CB pFunc, void* pContext = NULL );
    /*==================================================================
	功能        : 链路检测
	输入参数说明: guid

	返回值说明  : 成功返回MTI_SUCCESS,失败返回MTI_FAIL
	==================================================================*/
    BOOL ConnectionCheck(TVsipGuid tVsipGuid);

	BOOL ConnectionCheck(TVsipGuid tVsipGuid, u16 wInstanceId);

	/*==================================================================
	功能        : 设置链路检测间隔时间间隔
	输入参数说明: u32 wCheckInterval 链路检测间隔时间(单位ms)
				  u8 byDisconnectTime 断链确认次数
	返回值说明  : 无
	==================================================================*/
	void SetConnectionCheckInterval( u32 dwCheckInterval = 30000, u8 byDisconnectTime = 3 );

	/*==================================================================
	功能        : 搜索设备
	输入参数说明: TMTINetParam tDiscoveryDest[] 搜索目的IP地址表
				  u16 wDestCount 搜索目的IP地址数量
				  u16 wDiscoveryTimeout 搜索等待超时时间(毫秒)
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	u16 Discover( TMTINetParam tDiscoveryDest[], u16 wDestCount, u16 wDiscoveryTimeout = 5000 );

	u16 GetGuid(u32 dwIp);

	/*==================================================================
	功能        : 搜索全部设备
	输入参数说明: pDISCOVERY_CB pFuncCB 搜索结果回调函数
				  u16 wDiscoveryTimeout 搜索等待超时时间(毫秒)
	返回值说明  : MTI_SUCCESS-成功; MTI_FAIL-失败
	==================================================================*/
	u16 DiscoverAll( pDISCOVERY_CB pFuncCB, u16 wDiscoveryTimeout = 5000, void * pContext = NULL );

	/*==================================================================
	功能        : 检查设备是否在线
	输入参数说明: u32 dwUnitIpAddr 设备IP地址
	返回值说明  : TRUE-在线; FALSE-不在线
	==================================================================*/
	BOOL IsUnitOnline( u32 dwUnitIpAddr);

	/*==================================================================
	功能        : 获取设备
	输入参数说明: const TVsipGuid & tDevGuid 设备GUID
	返回值说明  : 返回设备控制指针, 失败返回NULL
	==================================================================*/
	CMTIUnitCtrl * GetUnitCtrl( const TVsipGuid & tDevGuid);
	//zlq add
	//CMTIUnitCtrl * GetUnitCtrl();
	//add end

	//daiyi add start for "app create connection"
	CMTIUnitCtrl * GetUnitCtrl(u32 dwIpAddr, u16 wPort);
	//daiyi add end for for "app create connection"
	/*==================================================================
	功能        : 删除设备
	输入参数说明: const TVsipGuid & tVsipGuid 设备GUID
	返回值说明  : TRUE-成功; FALSE-失败
	==================================================================*/
	BOOL DelUnitCtrl( const TVsipGuid & tVsipGuid);
	BOOL DelUnitCtrl( const TVsipGuid & tVsipGuid, u16 wInstId);
    BOOL ClearUnitCtrl( const TVsipGuid & tVsipGuid);
    /*==================================================================
	功能        : 判断某设备是否在设备控制表中，可能它已主动建链但未初始化
	输入参数说明: const TVsipGuid & tVsipGuid 设备GUID
	返回值说明  : TRUE-是; FALSE-否
	==================================================================*/
	BOOL DevIsInUnitCtrlInfo(const TVsipGuid & tVsipGuid);

	/*==================================================================
	功能        : 是否正在搜索
	输入参数说明: 无
	返回值说明  : TRUE-正在搜索; FALSE-未在搜索
	==================================================================*/
	BOOL IsDiscovering( ){ return m_bIsDiscovering;}

	/*=============================================================================
	函 数 名:  PrintDiscovered
	功    能:  打印业务搜索信息
	返 回 值:
	==============================================================================*/
	void PrintDiscovered();

	/*=============================================================================
	函 数 名:  PrintDiscoverAll
	功    能:  打印OS搜索信息
	返 回 值:
	==============================================================================*/
	void PrintDiscoverAll();

	/*=============================================================================
	函 数 名:  PrintUnitCapability
	功    能:  打印终端能力集信息
	返 回 值:
	==============================================================================*/
	void PrintUnitCapability();
	void PrintUnitCapability(const TVsipGuid tVsipGuid);
	/*=============================================================================
	函 数 名:  PrintUnitEntityinfo
	功    能:  打印终端能力集信息
	返 回 值:
	==============================================================================*/
	void PrintUnitEntityinfo();
	void PrintUnitEntityinfo(const TVsipGuid tVsipGuid);
	/*=============================================================================
	函 数 名:  PrintUnitCtrlInfo
	功    能:  打印设备控制句柄信息
	返 回 值:
	==============================================================================*/
	void PrintUnitCtrlInfo();
	void PrintStackConnection(int index);
	void PrintConnectionInfo();
	void PrintConListInfo(u16 wIndex=0, u16 wRange=0, u8 byLevel=0);
	void PrintUnitListInfo(u16 wIndex=0, u16 wRange=0, u8 byLevel=0);
	/*=============================================================================
	函 数 名:   PrintUnitInfo
	功    能:  打印设备搜索信息
	返 回 值:
	==============================================================================*/
	void PrintUnitInfo();


	/*=============================================================================
	函 数 名:  PrintClientInfo
	功    能:  打印主动建链设备信息
	返 回 值:
	==============================================================================*/
	void PrintClientUnitInfo();

	/*=============================================================================
	函 数 名:  ClearClientUnitInfo
	功    能:  消息接收线程接收错误断链时寻找并清楚主动建链终端信息
	返 回 值:
	==============================================================================*/
	void ClearClientUnitInfo(VSIP_HANDLE tVsipHandle);


	/*=============================================================================
    函 数 名:  GetExistUnitCtrl
    功    能:  获取已存在的设备控制指针
    返 回 值:
    ==========================================================================*/
#ifdef VSIP_PERFORM_IMPROVE
    CMTIUnitCtrl * GetExistUnitCtrl( const VSIP_HANDLE & VsipHandle);
#else
    CMTIUnitCtrl * GetExistUnitCtrl( const TVsipGuid & tGuid);
#endif
//#ifndef _LINUXEQUATOR_
	u16 SendUserUdpMSg(u32 dwDstIpaddr, u16 wDstPort, u8 byType, char *pbyBuf, u16 wpbyBufLen);
//#endif
	void SetIsPuiRegCmuOK(BOOL32 bOK);
protected:

	void AddClientUnit( const VSIP_HANDLE & VsipHandle );
	void SendDiscoveryResult( );
	void ClearDiscoveryResult( );

	BOOL AddDiscoveredUnit( TVsipGuid tVsipGuid, u32 dwUnitIpAddr, u16 wVsipPort, u16 wProduct = 0 );
	void CheckPingRespMsg( u8 * pBuf, u16 wBufLen);
	BOOL CheckConfigRespMsg( TVsipGuid tVsipGuid, u16 wConfigCount, VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );
	friend int DevDiscoveryRspCB( u8 DevTypeId, u32 DevIpAddr, void * pContext );
    friend void CheckSyncObjectCB(void * pParam);
	friend void DiscoveryTimeoutCB(void * pParam);
	friend void DiscoveryAidTimeoutCB(void * pParam);
	friend void DevDiscoveryTimeoutCB(void * pParam);
	friend void ConnectionCheckCB(void * pParam);
	friend int ClientConnectedCB( const VSIP_HANDLE & VsipHandle );

	friend int ClientContentTypeReqCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid,
									   TVsipGuid DstGuid, u8 ContentType );

	friend int ClientDiscoveryReqCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo );

	friend int DiscoveryRspCB( VSIP_HANDLE VsipHandle, VSIP_DISCOVERY_PING_INFO PingInfo,
							   TVsipGuid tVsipGuid, u16 Fabricant, u16 Product, u8 StatusChanged );

	friend int ContentTypeRspCB( VSIP_HANDLE VsipHandle, TVsipGuid SrcGuid, u8 SwitchResult );

	friend int GetCapabilityRspCB( VSIP_HANDLE VsipHandle, u16 CapListCount,
								   VSIP_IC_CAPABILITIES *pCapList );

	friend int GetConfigRspCB( VSIP_HANDLE VsipHandle, TVsipGuid tVsipGuid, u16 ConfigItemCount,
							   VSIP_MEDIA_CONTROL_CONFIG *pConfigItemInfo );

	friend int GetProprietaryRspCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 ConfigItemCount,
									 VSIP_MEDIA_CONTROL_SETPROPRIETARY_CONFIG *pConfigItemInfo );

	friend int OperationRspCB( VSIP_HANDLE VsipHandle, u16 ErrorCode );

	friend int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int EventNotifyCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid, u16 EventType, u32 EventArg );

	friend int EventNotifyExtendCB( VSIP_HANDLE VsipHandle, VSIP_GUID guid,unsigned short EventType,u16 wpbyBufLen,u8 *pbyBuf);

	friend int DisconnectedCB( VSIP_HANDLE VsipHandle, int nSockListNum );

	friend int CommandExReqCB(VSIP_HANDLE VsipHandle, VSIP_GUID tVsipGuid, u8 CommandCode, u32 Arg1,
		u32 Arg2, u16 AdditionalArgCount, VSIP_MEDIA_CONTROL_ADDITIONAL_ARG *pAddArg);

	//kedacom potocal begin
	friend int GetDecoderStyleCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	friend int GetCustomExtCapsCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
	//kedacom potocal end
private:

	BOOL bIsInited;
	u32					m_dwLocalIpAddr;//本机IP地址
	u16					m_wVsipPort;	//VSIP端口
	TMTIUnitListInfo	m_atUnitInfo[MAX_UNIT_NUM];	//设备信息

#ifndef VSIP_PERFORM_IMPROVE	//zhuzhm:链路检测放在UNITCTRL类中完成
	TMTIConnCheckInfo	m_atConnCheckInfo[MAX_UNIT_NUM];//链路检测信息
#endif

	TMTIDiscoveryCBInfo	m_atDevDiscvResult[MAX_UNIT_NUM];
	pDISCOVERY_CB		m_pfuncDevDiscvCB;
	void *				m_pDevDiscvCBContext;
	u32					m_dwDevDiscvCBTimerId;
	u32					m_dwDevDiscvCount;
	u32					m_dwLastDevDiscvCount;

	pDISCOVERY_CB	m_pfuncUnitListCallBack;
	void *			m_pUnitListCallBackContext;

	pNEWDISCOVERY_CB m_pNewDiscoveryCB;
	void *			 m_pNewDiscoveryContext;

	pCONNECTED_CB	m_pfuncUnitConnectedCallBack;
	void *			m_pUnitConnectedCallBackContext;

    pCONNECTIONCHECK_CB m_pfuncConnectionCheckCallBack;
    void *          m_pConnectionCheckContext;

	u32             m_dwMsgTimerId;
	u32				m_dwConnCheckCBTimerId;
	BOOL			m_bIsConnCheckCBEnable;
	u8				m_byDisconnectTime;

	BOOL			m_bIsDiscovering;
	BOOL			m_bIsDiscoverAllUnit;
	TMTINetParam	m_atDiscoveryDest[MAX_UNIT_NUM];
	u16				m_wDiscoveryDestCount;
	u32				m_dwDiscoveryCBTimerId;
	u16             m_wMaxOnlineMtNum;
	u32             m_dwDiscoveryCBAidTimerId;
	u32             m_wAidTimerout;
	u32             m_dwCheckSyncObjectTimerId;
};

//070813 zhuzhm add begin : for update
// 升级策略
#define VSIP_UPDATE_TYPE_INVALID		0   // 无效升级策略
#define VSIP_UPDATE_TYPE_FTP			1	// 标准ftp
#define VSIP_UPDATE_TYPE_USERDEF		2	// 自定义

#define MAX_UPDATE_FILE_NUM                 16
#define UPDATE_FILE_STRING_LEN              32

typedef struct VsipUpdateFileInfo
{
    char achName[UPDATE_FILE_STRING_LEN+1];  // 升级文件名
    char achPath[UPDATE_FILE_STRING_LEN+1];	 // 升级文件存放路径
}VSIP_UPDATE_FILE_INFO;

typedef struct VsipUpdateInfo
{
    u8 byPolice;     				// 升级策略
    u8 byFileNum;			// 升级文件个数（下面结构中有效项数）
    VSIP_UPDATE_FILE_INFO FileInfo[MAX_UPDATE_FILE_NUM];	// 对应升级文件信息
}VSIP_UPDATE_INFO;

s32 DeviceUpdateInfo (s8 *DeviceType, VSIP_UPDATE_INFO &Info);
// zhuzhm add end

//调试信息接口函数
API void mtiver();
API void mtihelp();
API void mtilogall();
API void mtilogon();
API void mtilogoff();
API void mtidebuglogon();
API void mtidebuglogoff();
API void mtisockdataon();
API void mtisockdataoff();
API void mtishowsockinfo();
API void mtishowcapability();
API void mtimiddlelogon();
API void mtimiddlelogoff();
API void mtitimeron();
API void mtitimeroff();
API void mtishowtimerstate();
API void vidparamoff();
API void vidparamon();

int GetRecStatusRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannelId,RecPlayerState byRecStatus);
int GetNmsParamRspCB( VSIP_HANDLE VsipHandle,VSIP_GUID guid,TNmsParam NmsParam);
int GetTimelyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,TimelyRecInfo TRecInfo);
int GetPeriodlyRecInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
int GetChannelRecBasicInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,u8 byChannel,RecChannelBasicInfo & RChanBasicInfo);
int GetRecSysInfoRspCB(VSIP_HANDLE VsipHandle, VSIP_GUID guid,RecSysInfo RSysInfo);
int QueryRecFileAckCB(VSIP_HANDLE VsipHandle, QueryRecFileInfo QRecFileinfo);
int QueryRecListRspCB(VSIP_HANDLE VsipHandle, TMTIRecListQryRsp tRecListRsp);
int QueryRecFileNumAckCB(VSIP_HANDLE VsipHandle, u16 wNum);
int ProgressCallCB(VSIP_HANDLE VsipHandle, ProgressParaInfo ParaInfo);
int RecStatusChangeCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,RecPlayerState byRecStatus);
int RecFileStartPlayidCB(VSIP_HANDLE VsipHandle,TMTIRecFilePlayNtf tPlayNtf);
int FileExportProgressCB(VSIP_HANDLE VsipHandle,ProgressParaInfo TProgress);
int VideoSourceStateCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,u8 byChannel,u8 byState);
int DevRegeditReqCB(VSIP_HANDLE VsipHandle,VSIP_GUID guid,TDevRegeditInfo DevRegInfoPara);
int StackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

int RecFileDownNotifyCB(VSIP_HANDLE VsipHandle,u16 wErrorCode,u16 wFtpPort);
int PlayGpsDataCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwErrcode,u8 byPlayId,u32 dwReserve);
int PlayGpsDataNotifyCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TPlayGpsNotify tPlayGpsDataNotify);

int StackTransSubsRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsRsp &tReq);
int StackTransSubsNtfCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TTransSubsNtf &tNtf);

int SetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int GetTransDataAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int GetTransCfgAckCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int StackOperationRspExCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int StackOperationRspEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataRsp tMTITransDataRsp);
int StackOperationNtfEx2CB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTITransDataNtf tMTITransDataNtf);
int GetCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int NotifyCuOperationCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u16 wpbyBufLen,u8 *pbyBuf);
int DevStreamPropRsp(VSIP_HANDLE VsipHandle,TVsipGuid guid,TMTIStartDevRspEx tMTIStreamProps);

//////////////////////////////////////////////////////////////////////////
//NA-PU
//实时浏览
int NAStartSendMediaRspCB(VSIP_HANDLE VsipHandle, TMTIStartSendMediaRsp& tRsp);
//音频呼叫和前端解码
int NAStartRecvMediaRspCB(VSIP_HANDLE VsipHandle, TMTIStartRecvMediaRsp& tRsp);
//录像播放
int NARecStartPlayRspCB(VSIP_HANDLE VsipHandle, TMTIRecStartPlayRsp& tRsp);
//录像下载
int NARecStartExportRspCB(VSIP_HANDLE VsipHandle, TMTIRecStartExportRsp& tRsp);

void ConnectionCheckCB(void * pParam);
void DiscoveryTimeoutCB(void * pParam);
void DiscoveryAidTimeoutCB(void * pParam);
void DevDiscoveryTimeoutCB(void * pParam);

void CheckConnectionLostlist(void * pParam);

int StartRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int StopRecReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int SetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,TimelyRecInfo TRecInfo);
int GetTimelyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int GetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int GetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int GetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel);
int SetPeriodlyRecInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,PeriodlyTimeInfo PTimeInfo);
int SetChannelRecBasicInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannel,RecChannelBasicInfo RChannelBasicInfo);
int SetRecSysInfoReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,RecSysInfo RSysInfo);
int QueryRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
int QueryRecFileNumReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TQueryParam QueryParam);
int StartExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u32 dwIpAddress, u16 wPort,u8 byConnectionType);
int StopExportRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int RecFileOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId,u8 byOperationType);
int DelRecFileReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u32 dwRecFileIndexId,u8 byType);
int DelRecRecordReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byType);
int SeekLocationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byPlayerId, u32 dwDragTime);
int GetRecStatusReqCB( VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byChannelId);
int RecFileStartPlayReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TRecStartPlayInfo tCfgParam);
int SetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,TNmsParam NmsParam);
int GetNmsParamReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid);
int DevRegeditRspCB(VSIP_HANDLE VsipHandle,u16 wResultRsp);
int MtStackOperationReqCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

int TimerDeadCB(VSIP_HANDLE VsipHandle);
int MtStackOperationRspCB(VSIP_HANDLE VsipHandle,TVsipGuid guid,u8 byRequestType,u16 wpbyBufLen,u8 *pbyBuf);

void SendNATOnce(void *pSendNATInfo);

#ifdef _LINUX_
void CheckContentRsp(void *pArg);
#endif

#ifdef WIN32
API int GetVmtNum();
#endif // WIN32


#ifdef WIN32
#pragma pack( pop )
#endif //WIN32
#endif //MT_CTRL_H


