/*=======================================================================
模块名      :SNMP适配模块
文件名      :snmpadp.h
相关文件    :snmpadp.cpp
文件实现功能 : 实现前端及平台设备与网管服务器之间的信息交互。

   结构字节序转化注意点（注：胡志云调查了库的实现，代理库内部并未调用转序的接口）
      1. 前端及平台业务不关心字节序的问题。所有的转化工作由代理库完成，上层业务
         全部使用主机序。
      2. 代理只在网络收发的接口处调用字节序转化接口，其他时间一律采用本机序

作者        : 李  博
版本        : V0.2  Copyright(C) 2005-2006 Suzhou Keda Technology Co.Ltd
                    All rights reserved.
--------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2005/12/22  0.1         李  博      创建
2006/02/23  0.2         李洪强      根据监控3.9的设计进行调整
2011/11/05  0.2         胡志云      解除对eventid.h的依赖(直接里面指定了AppID和EventID范围)
2011/11/11  0.2         胡志云      osp.h修改后我们需要显式包含cstdlib和ctime
2013/03/25  0.2         胡志云      平台2.0网管需求调整
=======================================================================*/
#ifndef _SNMPADP_H_
#define _SNMPADP_H_
/**
 * @defgroup SNMP适配模块
 * @{
 */
/** \file
*   设备端适配接口的头文件
*/
#include "cbb/osp/osp.h"
#include <cstdlib>
#include <ctime>

#ifdef WIN32
#pragma pack( push, 1 )
#define PACKED 
#endif //WIN32

#ifdef _VXWORKS_
#ifndef PACKED
#define PACKED __attribute__((__packed__))
#endif // PACKED
#endif //_VXWORKS_

#ifdef _LINUX_
#ifndef PACKED
#ifndef _EQUATOR_
#define PACKED  __attribute__((packed))
#else 
#define PACKED
#endif //EQUATOR_
#endif // PACKED 
#endif //_LINUX_

//代理AppId
#define AID_TNMS_BGN  11
#define AID_TNM_SNMPADP_CAGTSSN      (AID_TNMS_BGN + 6)
#define AID_TNM_SNMPADP_SAGTSSN      (AID_TNMS_BGN + 7)
/*设备代理与NMS接口消息（52001-53000）*/
#define EV_DEVAGT_NMS_BGN       52001
#define EV_DEVAGT_NMS_END       53000


//SNMP General Errors / Reasons Value Description
#define SNMP_SUCCESS              (u16)0    //Success Status
#define SNMP_FINISHED             (u16)1    //Too Big Data, Finished
#define SNMP_NOFINISHED           (u16)2    //Too Big Data, Not Finished
#define SNMP_GENERAL_ERROR        (u16)3    //General Error
#define SNMP_RESOURCE_UNAVAIL     (u16)4    //New or malloc Failed
#define SNMP_INTERNAL_ERROR       (u16)5    //Unexpected internal error
#define SNMP_UNSUPPORTED          (u16)6    //Unsupported function

//Callback Reasons
#define SNMP_TIMEOUT              (u16)10   //Outstanding request timed out
#define SNMP_NOTIFICATION         (u16)11   //Received notification (trap/inform)
#define SNMP_ASYNC_GETRESPONSE    (u16)12   //Received get-response
#define SNMP_ASYNC_SETRESPONSE    (u16)13   //Received set-response
#define SNMP_ASYNC_RESPONSE_ERROR (u16)14   //Received response error
#define SNMP_SESSION_DESTROYED    (u16)15   //Snmp Object destroyed
//Snmp Class Errors
#define SNMP_INVALID              (u16)20   //Snmp member function called on invalid instance
#define SNMP_INVALID_PDU          (u16)21   //Invalid Pdu passed to mf
#define SNMP_INVALID_TARGET       (u16)22   //Invalid target passed to mf
#define SNMP_INVALID_CALLBACK     (u16)23   //Invalid callback to mf
#define SNMP_INVALID_REQID        (u16)24   //Invalid request id to cancel
#define SNMP_INVALID_NOTIFYID     (u16)25   //Missing trap/inform Oid
#define SNMP_INVALID_OPERATION    (u16)26   //Snmp operation not allowed for specified target
#define SNMP_INVALID_OID          (u16)27   //Invalid Oid passed to mf
#define SNMP_INVALID_ADDRESS      (u16)28   //Invalid address passed to mf
#define SNMP_ERR_STATUS_SET       (u16)29   //Agent returned response Pdu with error status set
#define SNMP_TL_UNSUPPORTED       (u16)30   //Transport not supported
#define SNMP_TL_IN_USE            (u16)31   //Transport in use
#define SNMP_TL_FAILED            (u16)32   //Transport Failed

#define SNMP_ERR_TOO_BIG          (u16)50   //Pdu Too Big, see error index
#define SNMP_ERR_NO_SUCH_NAME     (u16)51   //No Such Variable Binding name, see returned error index
#define SNMP_ERR_BAD_VALUE        (u16)52   //Bad Variable Binding Value, see returned error index
#define SNMP_ERR_READ_ONLY        (u16)53   //Variable Binding is read only, see returned error index
#define SNMP_ERR_GENERAL_VB_ERR   (u16)54   //General Variable Binding error, see returned error index
#define SNMP_ERR_NO_ACCESS        (u16)55   //Operation Failure, No Access
#define SNMP_ERR_WRONG_TYPE       (u16)56   //Operation Failure, Bad Type
#define SNMP_ERR_WRONG_LENGTH     (u16)57   //Operation Failure, Bad Length
#define SNMP_ERR_WRONG_ENCODING   (u16)58   //Operation Failure, Wrong Encoding
#define SNMP_ERR_WRONG_VALUE      (u16)59   //Operation Failure, Wrong Value
#define SNMP_ERR_NO_CREATION      (u16)60   //Operation Failure, No Creation
#define SNMP_ERR_INCONSIST_VAL    (u16)61   //Operation Failure, Inconsistent Value
#define SNMP_ERR_RESOURCE_UNAVAIL (u16)62   //Operation Failure, Resource Unavailable
#define SNMP_ERR_COMITFAIL        (u16)63   //Operation Failure, Commit Failure
#define SNMP_ERR_UNDO_FAIL        (u16)64   //Operation Failure, Undo Failure
#define SNMP_ERR_AUTH_ERR         (u16)65   //Operation Failure, Authorization Error
#define SNMP_ERR_NOT_WRITEABLE    (u16)66   //Operation Failure, Not Writable
#define SNMP_ERR_INCONSIS_NAME    (u16)67   //Operation Failure, Inconsistent Name


#define TNM_DAYS_OF_WEEK                    7       //一周的天数
#define TNM_MAX_TIME_STRING_LEN             16      //最大时间字符串长度
#define TNM_MAX_SERIALPORT_DATA_LEN         1024    //最串口数据长度
#define TNM_MAX_STRING_LEN                  16      //最大字符串长度
#define TNM_MAX_VERSION_LEN                 32      //最大版本字符串长度
#define TNM_MAX_VERSION_PATH_LEN            128     //最大版本路径字符串长度
#define TNM_MAX_USER_NUM                    256     //最大用户数目
#define TNM_MAX_VIDEOIN_PORT_NAME_LEN       8       //最大视频输入端口名长度
#define TNM_MAX_VIDEOIN_PORT_NUM            32      //最大视频输入端口数量
#define TNM_MAX_PIC_REGION_NUM              16      //多画面最大画面数量
#define TNM_MAX_POLL_PORT_NUM               4       //轮询最大端口数
#define TNM_MAX_NET_ADAPTER_NUM             10      //支持的网卡数目
#define TNM_MAX_PANELLED_LEN                16      //最大面板灯个数
#define MAX_BOX_ID_LEN                      32      //最大机框标识符长度
#define TNM_MAX_DEVTYPE_NAME_LEN            31      //最大的设备名称长度
#define TNM_MAX_CAMERA_NAME_LEN             32      //最大的摄像头名

#define TNM_MAX_ETHERNET_NUM                5       //最大以太网口数
#define TNM_MAX_FAN_NUM                     5       //最大风扇数
#define TNM_MAX_POWER_NUM                   5       //最大电源数
#define TNM_MAX_VTDU_NUM                    16      //最大VPU板数
#define TNM_MAX_VPUDECCHN_NUM               16      //最大VPU解码通道数

#define TNM_MAX_CHAN_NUM                    16      //最大通道数
#define TNM_MAX_MAP_NUM                     5       //最大MAP数
#define TNM_MAX_VIDEOSRC_NUM                8       //最大视频数
#define TNM_MAX_LPTCHAN_NUM                 8       //最大并口数

#define TNM_MAX_RECFILEPATHNAME_LEN         128     //最大录像文件路径名长度
#define TNM_MAX_DBPROVIDER_LEN              64      //最大OLEDB 提供者名长度
#define TNM_MAX_DBNAME_LEN                  64      //最大数据库名长度

#define TNM_MAX_CU_LOG_NUM                  10      //从CU处理到的最多的日志条目数
#define TNM_MAX_CU_LOG_LEN                  199     //从CU处理到日志条目最大长度

#define TNM_MAX_COMMUNITY_LEN               32

#define TNM_MAX_FIELD_NAME_LEN              32      // 最大的域ID长度
#define TNM_MAX_ALIAS_NAME_LEN              32      // 最大的别名长度
#define TNM_MAX_DEVICE_ID_LEN               32      // 最大的设备ID长度
#define TNM_MAX_USER_ID_LEN                 32      // 最大的用户登录ID长度
#define TNM_MAX_PASSWORD_LEN                32      // 最大用户密码长度
#define TNM_CMS_ID_LEN                      18      // CMS的ID编号长度

#define TNM_MAX_FIELD_ID_LEN                32
#define TNM_MAX_USER_NAME_LEN               32

#define TNM_MAX_COMPILE_TIME_LEN            32      // 编译时间字串的最大长度

// 3A拓朴信息长度定义
#define TNM_MAX_DEV_NAME_LEN                32     // 设备名称
#define TNM_MAX_DEV_MODEINFO_LEN            16     // 设备型号
#define TNM_MAX_DEV_NETADDR_LEN             32     // 设备网络地址，IP或域名

// 子消息类型
#define SUBEV_TYPE_INVALID              0           // 无效值
#define SUBEV_TYPE_REQ                  1           // 请求类
#define SUBEV_TYPE_ACK                  2           // 确认类
#define SUBEV_TYPE_NACK                 3           // 拒绝类
#define SUBEV_TYPE_NOTIFY               4           // 通知类
#define SUBEV_TYPE_FINISH               5           // 批次消息完成确认类

#ifndef DEFAULT_WRITE_COMMUNITY
#define DEFAULT_WRITE_COMMUNITY             "private"
#endif
#ifndef DEFAULT_READ_COMMUNITY
#define DEFAULT_READ_COMMUNITY              "public"
#endif

#define TNM_DEFAULT_LISTENPORT              1727    //代理默认监听端口

//消息发送的协议类型
#define PROTO_TCP                           0x00000000
#define PROTO_UDP                           0xF0000000
//协议MASK
#define PROTO_TCP_MASK                      0xF0000000
#define PROTO_UDP_MASK                      0xF0000000

//链接类型
#define TNM_CONNECTION_NONE                 0x00
#define TNM_CONNECTION_UNICAST              0x01
#define TNM_CONNECTION_MULTICAST            0x02
#define TNM_CONNECTION_BROADCAST            0x03
#define TNM_CONNECTION_TCPSERVER            0x04
#define TNM_CONNECTION_TCPCLIENT            0x05
#define TNM_CONNECTION_TCP                  0x06
#define TNM_CONNECTION_SSL                  0x07

//视频数据格式
#define TNM_VIDEO_TYPE_NONE                 0x00
#define TNM_VIDEO_TYPE_SN4                  0x01
#define TNM_VIDEO_TYPE_MPEG4                0x02
#define TNM_VIDEO_TYPE_H261                 0x03
#define TNM_VIDEO_TYPE_H263                 0x04
#define TNM_VIDEO_TYPE_H264                 0x05
#define TNM_VIDEO_TYPE_H262             0x06    /*H.262 (MPEG-2)*/
#define TNM_VIDEO_TYPE_H263PLUS            0x07    /*H.263+*/

//音频数据格式
#define TNM_AUDIO_TYPE_NONE                 0x00
#define TNM_AUDIO_TYPE_PCMA                 0x01
#define TNM_AUDIO_TYPE_PCMU                 0x02
#define TNM_AUDIO_TYPE_GSM                  0x03
#define TNM_AUDIO_TYPE_G729                 0x04
#define TNM_AUDIO_TYPE_MP3                  0x05
#define TNM_AUDIO_TYPE_ADPCM                0x08

#define TNM_AUDIO_TYPE_G721                 0x09    /*G.721*/
#define TNM_AUDIO_TYPE_G722                 0x0A    /*G.722*/
#define TNM_AUDIO_TYPE_G7231                0x0B    /*G.7231*/
#define TNM_AUDIO_TYPE_G728                 0x0C    /*G.728*/
#define TNM_AUDIO_TYPE_G7221                0x0D    /*G.7221*/
#define TNM_AUDIO_TYPE_G7221C               0x0E    /*G.7221C*/

//图象分辨率
#define TNM_VIDEO_RESOLUTION_NONE           0x00
#define TNM_VIDEO_RESOLUTION_AUTO           0x01
#define TNM_VIDEO_RESOLUTION_QCIF           0x02
#define TNM_VIDEO_RESOLUTION_CIF            0x03
#define TNM_VIDEO_RESOLUTION_2CIF           0x04
#define TNM_VIDEO_RESOLUTION_4CIF           0x05

//图像质量
#define TNM_VIDEO_SET_NONE                  0x00 //不支持图象质量设置
#define TNM_VIDEO_QUALITY_PRIOR             0x01 //质量优先
#define TNM_VIDEO_SPEED_PRIOR               0x02 //速度优先

//图像后处理级别(0～3级,0表示没有)
#define TNM_VIDEO_DEALLEV_NONE              0x00
#define TNM_VIDEO_DEALLEV_ONE               0x01
#define TNM_VIDEO_DEALLEV_TWO               0x02
#define TNM_VIDEO_DEALLEV_THREE             0x03

//多画面风格
#define TNM_VIDEO_FORMAT_NONE               0x00 //无多画面
#define TNM_VIDEO_FORMAT_PIP_4              0x01 //画中画(比例1:4)
#define TNM_VIDEO_FORMAT_PIP_9              0x02 //画中画(比例1:9)
#define TNM_VIDEO_FORMAT_PIP_16             0x03 //画中画(比例1:16)

//串口类型
#define TNM_SERIAL_LINE_DRIVER_NONE         0x00
#define TNM_SERIAL_LINE_DRIVER_RS232        0x01
#define TNM_SERIAL_LINE_DRIVER_RS422        0x02
#define TNM_SERIAL_LINE_DRIVER_RS485        0x03

//音频输入类型
#define TNM_AUDIO_INPUTTYPE_LINEIN          0x00
#define TNM_AUDIO_INPUTTYPE_MIC             0x01
//音频输入能力
#define TNM_CAP_AUDIO_INPUT_LINEIN          0x01    //linein
#define TNM_CAP_AUDIO_INPUT_MIC             0x02    //mic

//串口数据通道类型
#define TNM_SERIAL_STREAM_TYPE_NONE         0x00 //不使用
#define TNM_SERIAL_STREAM_TYPE_UDP          0x01 //UDP通道
#define TNM_SERIAL_STREAM_TYPE_TCP          0x02 //TCP通道
#define TNM_SERIAL_STREAM_TYPE_VSIP         0x03 //消息通道

//串口数据协议类型
#define TNM_SERIAL_DATA_TYPE_NONE           0x00 //透明通道
#define TNM_SERIAL_DATA_TYPE_VSIP           0x01 //VSIP协议

//码流传输类型
#define TNM_MEDIA_STREAM_TYPE_UDP           0x00 //UDP通道
#define TNM_MEDIA_STREAM_TYPE_TCP           0x01 //TCP通道

//无视频时的显示
#define TNM_STARVE_MODE_COLOR_BAR           0x00 //彩色条
#define TNM_STARVE_MODE_LAST_IMAGE          0x01 //屏幕冻结
#define TNM_STARVE_MODE_BLACK_SCR           0x02 //黑屏
#define TNM_STARVE_MODE_SHOW_OSD            0x03 //屏幕冻结并字幕提示

//视频输出端口类型
#define TNM_VIDEO_OUT_TYPE_C                0x01 // 复合C端子
#define TNM_VIDEO_OUT_TYPE_S                0x02 // S端子
#define TNM_VIDEO_OUT_TYPE_VGA              0x03 // VGA输出

//告警检测状态
#define TNM_ALARM_DETECT_NONE               0x00 //不检测
#define TNM_ALARM_DETECT_ONCE               0x01 //检测一个周期
#define TNM_ALARM_DETECT_REPEAT             0x02 //循环检测
#define TNM_ALARM_DETECT_ALWAYS             0x03 //始终检测

//摄像头类型
#define TNM_CAMERA_TYPE_SONY                0x01    // 索尼（SONY）
#define TNM_CAMERA_TYPE_AD                  0x02    // AD
#define TNM_CAMERA_TYPE_PELCO               0x03    // PELCO
#define TNM_CAMERA_TYPE_PIH                 0x04    // PIH
#define TNM_CAMERA_TYPE_PELCO_P             0x05    // PELCO_P
#define TNM_CAMERA_TYPE_PARCO               0x06    // 柏高（PARCO）
#define TNM_CAMERA_TYPE_AB                  0x07    // 诶比（AB）
#define TNM_CAMERA_TYPE_YUNTAI              0x08
#define TNM_CAMERA_TYPE_SAMSUNG             0x09    // 三星（SAMSUNG）
#define TNM_CAMERA_TYPE_YAAN                0x010   // 亚安
#define TNM_CAMERA_TYPE_AD168               0x011   // AD168
#define TNM_CAMERA_TYPE_KALATEL             0x012   // 卡特拉（KALATEL）
#define TNM_CAMERA_TYPE_JN                  0x013 
#define TNM_CAMERA_TYPE_WV_CS650            0x014   // 松下（CS650）是850的老协议
#define TNM_CAMERA_TYPE_WV_CS850            0x015   // 松下（CS850）新协议
#define TNM_CAMERA_TYPE_ALEC                0x016   // 艾立克（ALEC）
#define TNM_CAMERA_TYPE_AB_P                0x017
#define TNM_CAMERA_TYPE_AB_D                0x018




//VGA输出刷新频率
#define TNM_VGA_OUT_FREQUENCY_60            0x01 //刷新频率60
#define TNM_VGA_OUT_FREQUENCY_75            0x02 //刷新频率75
#define TNM_VGA_OUT_FREQUENCY_85            0x03 //刷新频率85

//设备编解码类型
#define TNM_NONE                            0x0  //视频编解码都不支持
#define TNM_ENCODER                         0x1  //只支持视频编码
#define TNM_DECODER                         0x2  //只支持视频解码
#define TNM_BOTH                            0x3  //视频编解码都支持
#define TNM_SMARTSIGHT_ENCODER              0x4  //SMARTSIGHT视频编码支持
#define TNM_SMARTSIGHT_DECODER              0x5  //SMARTSIGHT视频解码支持
#define TNM_SMARTSIGHT_BOTH                 0x6  //SMARTSIGHT视频编解码都支持

//磁盘满的录像处理策略
#define TNM_DISK_POLICY_STOP_REC             0x0  // 停止录像
#define TNM_DISK_POLICY_REPLACE_ALL          0x1  // 覆盖所有文件
#define TNM_DISK_POLICY_KEEP_ALARMREC        0x2  // 覆盖除告警之外的文件

// 设备分区类型
#define TNM_SINGLE_DISK                     0x0     // 单分区
#define TNM_MULTI_DISK                      0x1     // 多分区

// 磁盘数量
#define TNM_MAX_NRU_DISK_NUM                (u8)255 // 最大磁盘数量
#define TNM_MAX_PU_DISK_NUM                 (u8)32  // 前端设备的最大磁盘数量

//vpu合成风格
#define TNM_VPU_STYLE_INVALID               0x0  //无效风格
#define TNM_VPU_STYLE_HTWO                  0x1  //两画面：左右分
#define TNM_VPU_STYLE_THREE                 0x2  //三画面
#define TNM_VPU_STYLE_FOUR                  0x3  //四画面
#define TNM_VPU_STYLE_SIX                   0x4  //六画面
#define TNM_VPU_STYLE_ONE                   0x5  //单画面（分成9份发送）
#define TNM_VPU_STYLE_VTWO                  0x6  //两画面： 一大一小（大画面分成9份发送）
#define TNM_VPU_STYLE_SPECFOUR              0x7  //特殊四画面：一大三小（大画面分成4份发送）
#define TNM_VPU_STYLE_SEVEN                 0x8  //七画面：三大四小（大画面分成4份发送）
#define TNM_VPU_STYLE_EIGHT                 0x9  //八画面
#define TNM_VPU_STYLE_NINE                  0xa //九画面
#define TNM_VPU_STYLE_TEN                   0xb //十画面：二大八小（大画面分成4份发送）
#define TNM_VPU_STYLE_THIRTEEN              0xc //十三画面
#define TNM_VPU_STYLE_SIXTEEN               0xd //十六画面

// 编解码码流收发状态
#define TNM_CODEC_STOP                      0x0 // 编解码通道空闲
#define TNM_ENCODING                        0x1 // 正在编码发送
#define TNM_DECODING                        0x2 // 正在解码发送

// 音视频方式
#define TNM_ONLY_VIDEO           0x1    // 只有视频
#define TNM_ONLY_AUDIO           0x2    // 只有音频
#define TNM_AUDIO_VIDEO          0x3    // 音视频

// 前端设备状态
#define TNM_PU_STATE_ONLINE      0x01 // 0: 是否在线：0:offline, 1:online
#define TNM_PU_STATE_MUTE        0x02 // 1: 是否远端静音：0:notmute, 1:mute
#define TNM_PU_STATE_SEND_AUDIO  0x04 // 2: 是否在发送音频：0:no, 1:yes
#define TNM_PU_STATE_PLAT_REC    0x08 // 3: 是否正在平台录像0:no, 1:yes
#define TNM_PU_STATE_PU_REC      0x10 // 4: 是否正在前端录像0:no, 1:yes
#define TNM_PU_STATE_ALARM       0x20 // 5: 是否存在业务告警 0:no,1:yes

#define TNM_STATE_OFFLINE                   0x00    // 设备离线
#define TNM_STATE_ONLINE                    0x01    // 设备在线

#define TNM_PU_GUID_LEN                     16      // GUID的长度

#define TNM_OSD_COLOR_FONT      (u8)1   //整字自适应
#define TNM_OSD_COLOR_16        (u8)2
#define TNM_OSD_COLOR_256       (u8)3

#define TNM_UPGRADE_MODE_NONE   (u8)0   //不能升级
#define TNM_UPGRADE_MODE_TCP    (u8)1   //TCP方式升级
#define TNM_UPGRADE_MODE_FTP    (u8)2   //FTP方式升级

//其它定义
#define TNM_DISABLE             0
#define TNM_ENABLE              1

#define TNM_INVALED_IP          0xFFFFFFFF      //无效的IP地址
#define TNM_INVALED_PORT        0xFFFF          //无效的端口
#define TNM_INVALID_CHANNEL_ID  0xFF            // 无效的通道号

#define TNM_REG_CMU   0              // 直接注册到平台
#define TNM_REG_DNS   1              // 通过负载均衡服务器获取平台地址

/************************************************************************/
/*                          告警码的定义(当前可用号码：10027)             */
/************************************************************************/
#define TNM_ALARM_CODE_INVALID             0            // 无效的告警码
#define TNM_ALARM_CODE_DEV_OFFLINE         10001        // 设备下线
#define TNM_ALARM_CODE_RECV_NOBITSTREAM    10002        // 接收无码流上报
#define TNM_ALARM_CODE_LOSEPACKET          10003        // 码流接收丢包超过指定阀值告警
#define TNM_ALARM_CODE_MAP_STATUS          10004        // MAPS异常告警
#define TNM_ALARM_CODE_MOTIONDETECT_STATUS 10005        // 移动侦测告警
#define TNM_ALARM_CODE_LPT_STATUS          10006        // 并口告警上报
#define TNM_ALARM_CODE_POWER_STATUS        10007        // 电源电压低于指定阀值告警
#define TNM_ALARM_CODE_CPU_STATUS          10008        // CPU使用率高于指定阀值告警
#define TNM_ALARM_CODE_MEMORY_STATUS       10009        // 内存使用超过指定阀值告警
#define TNM_ALARM_CODE_DISKFULL_STATUS     10010        // 磁盘满告警
#define TNM_ALARM_CODE_FAN_STATUS          10011        // 风扇转速过低告警
#define TNM_ALARM_CODE_ETHCARD_RESTORE     10012        // 以太网恢复告警
#define TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS 10013        // 前端设备视频源丢失告警
#define TNM_ALARM_CODE_LED_STATUS          10014        // 面板灯快闪、慢闪等状态发生改变
#define TNM_ALARM_CODE_SAME_REG_ID         10015        // 其他设备的ID号与本设备相同，同时向NMS注册
#define TNM_ALARM_CODE_REG_CMU             10016        // PU或其他模块注册CMU失败告警
#define TNM_ALARM_CODE_DISKDETECT_ERROR    10017        // 设备检测无磁盘或没有磁盘可工作告警


#define TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET        10021        //Cu通过CMU发送的CU业务告警
#define TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION        10022        //此告警是Cu通过CMU发送的CU业务告警
#define TNM_ALARM_CU_OVERLOAD              10023        // 登录CU数超过阀值
#define TNM_ALARM_PU_OVERLOAD              10024        // 登录PU数超过阀值
#define TNM_ALARM_VTDU_OVERLOAD            10025        // VTDU转发路数超过阀值


//  23  TNM_ALARM_CODE_NODISK_ERROR         无硬盘 - 按IDE编号(0-3)
//      u8(硬盘IDE编号) + u8(硬盘的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_NODISK_ERROR        10100       //无硬盘 - 按IDE编号(0-3)

//  24  TNM_ALARM_CODE_DISK_ERROR           硬盘分区坏 - 按分区编号(0-15)
//      u8(硬盘分区编号) + u8(硬盘分区的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_ERROR          10101       //硬盘分区坏 -按分区编号(0-15)

//  25  TNM_ALARM_CODE_DISK_NOFORMAT        硬盘分区未格式化 - 按分区编号(0-15)
//      u8(硬盘分区编号) + u8(硬盘分区的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_NOFORMAT       10102       //硬盘分区未格式化 - 按分区编号(0-15)

//  26  TNM_ALARM_CODE_STOPREC_LOGERR       前端停录 - 日志系统坏停录
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_STOPREC_LOGERR      10103       //前端停录 - 日志系统坏停录

//  27  TNM_ALARM_CODE_STOPREC_DISKERR      前端停录 - 硬盘坏停录
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_STOPREC_DISKERR     10104       //前端停录 - 硬盘坏停录


//  28  TNM_ALARM_CODE_DISCONNECT_3AS
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_3AS      11001      //未连接上3AS

//  29  TNM_ALARM_CODE_DISCONNECT_UAS
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_UAS      11002      //未连接上UAS

//  30  TNM_ALARM_CODE_DISCONNECT_PUI
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_PUI      11003      //未连接上PUI

//  31  TNM_ALARM_CODE_DISCONNECT_CUI
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_CUI      11004      //未连接上CUI

//  32  TNM_ALARM_CODE_DISCONNECT_NRU
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_NRU      11005      //未连接上NRU

//  33  TNM_ALARM_CODE_DISCONNECT_VTDU
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_VTDU     11006      //未连接上VTDU

//  34  TNM_ALARM_CODE_DISCONNECT_CMU
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_CMU      11007      //未连接上CMU

//  35  TNM_ALARM_CODE_DISCONNECT_DATABASE
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_DATABASE 11008      //未连接上DATABASE

//  36  TNM_ALARM_CODE_DISCONNECT_USBKEY
//      u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISCONNECT_USBKEY   11009     //未连接上USBKEY

//  37  TNM_ALARM_CODE_DISK_PART_R_ONLY   NRU磁盘分区只读告警
//      u8*(磁盘分区mount路径---最长不超过80)+u8(告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
#define TNM_ALARM_CODE_DISK_PART_R_ONLY			11010		//磁盘分区只读
#define TNM_ALARM_CODE_DISK_FS_ERROR			11011		//磁盘文件系统错误
#define TNM_ALARM_CODE_DISK_OFFLINE				11012		//磁盘下线
#define TNM_ALARM_CODE_STORAGE_ABNORMAL			11013		//磁盘有异常

#define TNM_ALARM_CODE_PLATFORM_IN_REPLACE_MODE 11014		//平台运行在备机替换模式

#define TNM_ALARM_CODE_RAID_DISK_OFFLINE		11015		 //阵列硬盘损坏或空缺
#define TNM_ALARM_CODE_RAID_DISK_HEALTH_BAD		11016		 //阵列硬盘健康状态异常
#define TNM_ALARM_CODE_RAID_DISKGROUP_STATUS_ABNORMAL	11017//阵列磁盘组状态不正常
#define TNM_ALARM_CODE_RAID_ISCSI_DROPPED		11018		 //阵列ISCSI断链
#define TNM_ALARM_CODE_RAID_ABNORMAL			11019		 //阵列异常

#define TNM_ALARM_CODE_DETECT_NULL_IP_NIC       11020        //探测到无IP的网卡

//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_SUPER_CMU		11021      //未连接上上级CMU
//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_SUB_CMU		11022		//下级CMU断开连接
//      s8[33](IPStr)+s8[33](TKDMNO)+u8(TNM_STATUS_ALARM/TNM_STATUS_NORMAL)
#define TNM_ALARM_CODE_DISCONNECT_BCMU_CMU		11023		//未连接上平级CMU

#define TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY       11024      //USB到期提醒
#define TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS   11026      //LICENSE校验不通过
//      u32(单个平台允许的最大录像磁盘数)+u32(当前连接的磁盘数)
#define TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD	11025		//平台的数据盘分区总数超过录像机支持的最大个数


/************************************************************************/
#define MAX_DEV_ALARM_TYPE                 36           // 单一设备最大可能上报的告警种类
#define TNM_ROOT_NODE_ID                    "99000000000000000050000000000000"  // 顶级平台的域编号

// ID编号长度
// 编号由三部分组成: CMS + NO类别码 + 剩余
#define   FIELD_LEN_CMS                     (u8)18      //  CMS编码段长度
#define   FIELD_LEN_ID_TYPE                 (u8)1       //  ID类别码 编码段长度
#define   FIELD_LEN_ELSE                    (u8)13      //  剩余     编码段长度

#define   FIELD_LEN_PU_TYPE                 (u8)1       // 前端设备类型   编码段长度, 具体类型enum TnmDevCategory
#define   FIELD_LEN_PU_SUBTYPE              (u8)1       // 前端子设备类型 编码段长度, 具体类型enum TnmDevCategory

#define   FIELD_LEN_PLAT_DEV_TYPE           (u8)1       //平台设备类型   编码段长度

//Hot Backup State
#define TNM_HB_STATE_NORMAL    0     //普通模式服务器
#define TNM_HB_STATE_ACTIVE    1     //活动热备服务器
#define TNM_HB_STATE_INACTIVE  2     //备用热备服务器

//CMU 各模块最大数
#define  TNM_MAX_CMU_MODULE_NUM    128

// 设备的告警状态
enum TnmStatusType
{
    TNM_STATUS_NONE = 0,        // 表示无效状态
    TNM_STATUS_ALARM,           // 表示各种有问题的告警状态(设备掉线等)
    TNM_STATUS_NORMAL,          // 表示各种正常的状态(设备上线等)
    TNM_STATUS_CLEAR,           // 问题仍然存在，但被管理员手动解除告警输出
    TNM_STATUS_NOCONFIRM = 4,   // 未确认
    TNM_STATUS_CONFIRM          // 已确认
};
#define MAX_MOUNTPATH_LEN       (s32)80 //存储管理部分::安装路径名最大长度

//PC平台设备的层槽号
#define PC_PLAT_LAYER 1
#define PC_PLAT_SLOT  1

// 网管设备的硬件类型
enum TnmUnitType
{
    // 未知设备
    TNM_UNIT_UNKNOW = 0,

    // 前端设备
    TNM_PU_KDM2400 = 1,
    TNM_PU_KDM2400_PLUS,
    TNM_PU_KDM2500,
    TNM_PU_KDM2500_PLUS,
    TNM_PU_KDM2417,
    TNM_PU_KDM2418,
    TNM_PU_KDM2401,
    TNM_PU_KDM2428,
    TNM_PU_KDM2518,
    TNM_PU_KDV8010A,
    TNM_PU_KDV8010C,
    TNM_PU_KDM2401L,
    TNM_PU_KDM2401S,
    TNM_PU_KDM2402S,
    TNM_PU_KDM2402,

    // 独立的平台设备
    TNM_CMU_KDM2100A = 16,

    // 业务单板
    TNM_BOARD_MPC = 21,
    TNM_BOARD_CRI,
    TNM_BOARD_DRI,
    TNM_BOARD_VPU,
    TNM_BOARD_VAS,
    TNM_BOARD_DEC5,
   
    // 台式计算机
    TNM_PC_UNIT = 31,
    
    //pu补充
    TNM_PU_KDM2461 = 55,
    TNM_PU_KDM2461L,
    TNM_PU_KDM2560,
    TNM_PU_KDM2401LS,
    TNM_PU_KDM2402L,
    TNM_PU_KDM2402LS,

    TNM_PU_KDM2440,
    TNM_PU_KDM2440P,
    TNM_PU_KDM201_ENC_CARD,
    TNM_PU_KDM201_DEC_CARD,
    TNM_PU_KDM2461LS,
    TNM_PU_KDM2462L,
    TNM_PU_KDM2462S,
    TNM_PU_KDM2462LS,
    TNM_PU_KDM2461S,
    TNM_PU_KDM2462,

    TNM_PU_HARDWARE_PU = 100        //前端设备硬件类型
};

// 业务类型定义
enum TnmServiceType
{
    // 未知业务类型
    TNM_SERVICE_UNKNOW = 0,

    // 前端业务
    TNM_PU_SERVICE_KDM2400 = 1,
    TNM_PU_SERVICE_KDM2400_PLUS,
    TNM_PU_SERVICE_KDM2500,
    TNM_PU_SERVICE_KDM2500_PLUS,
    TNM_PU_SERVICE_KDM2417,
    TNM_PU_SERVICE_KDM2418,
    TNM_PU_SERVICE_KDM2401,
    TNM_PU_SERVICE_KDM2428,
    TNM_PU_SERVICE_KDM2518,    
    TNM_PU_SERVICE_KDV8010A,
    TNM_PU_SERVICE_KDV8010C, 
    TNM_PU_SERVICE_KDMDEC5,
    TNM_PU_SERVICE_PU,                  // 前端业务类

    // 平台业务
    TNM_CMU_SERVICE_KDM2000 = 15,
    TNM_CMU_SERVICE_KDM2100A,

    // 单板业务
    TNM_CMU_SERVICE_PUI = 21,
    TNM_CMU_SERVICE_CUI,
    TNM_CMU_SERVICE_VTDU,
    TNM_CMU_SERVICE_VPU,
    TNM_CMU_SERVICE_DNS,

    // PC业务
    TNM_PC_SERVICE_SERVER = 35,  // 平台业务类
    TNM_PC_SERVICE_NRU = 31,
    TNM_PC_SERVICE_UAS,
    TNM_PC_SERVICE_3AS,
    TNM_PC_SERVICE_NMS,
    
    //pu补充
    TNM_PU_SERVICE_KDM2401L = 50,
    TNM_PU_SERVICE_KDM2401S,
    TNM_PU_SERVICE_KDM2402S,
    TNM_PU_SERVICE_KDM2402,
    TNM_PU_SERVICE_KDM2461,
    TNM_PU_SERVICE_KDM2461L,
    TNM_PU_SERVICE_KDM2560,
    TNM_PU_SERVICE_KDM2401LS,
    TNM_PU_SERVICE_KDM2402L,
    TNM_PU_SERVICE_KDM2402LS,

    TNM_PU_SERVICE_KDM2440,
    TNM_PU_SERVICE_KDM2440P,
    TNM_PU_SERVICE_KDM2461LS,
    TNM_PU_SERVICE_KDM2462L,
    TNM_PU_SERVICE_KDM2462S,
    TNM_PU_SERVICE_KDM2462LS,
    TNM_PU_SERVICE_KDM2461S,
    TNM_PU_SERVICE_KDM201_ENC_CARD,
    TNM_PU_SERVICE_KDM201_DEC_CARD,
    TNM_PU_SERVICE_KDM2462,
  
    //IAS,被注释掉了, 运营也不维护了
    //IMU
    TNM_CMU_SERVICE_IMU,

    //VMU
    TNM_CMU_SERVICE_VMU,

    //RMG
    TNM_CMU_SERVICE_RMG,

    //NVR
    TNM_NVR_SERVICE_TRASPARENT_CHANNEL,

    //VRS
    TNM_CMU_SERVICE_VRS,

    //VMTU
    TNM_CMU_SERVICE_VMTU,
    
    TNM_PC_SERVICE_ALL = 100,           // 所有业务类型
    TNM_KDV_SERVICE_ALL = 101,          // 所有业务类型  
};

// 编号类型
enum TnmDevIdType
{
    ID_TYPE_INVALID     = 0,    // 前端设备编号类型
    ID_TYPE_PU          = 1,    // 前端设备编号类型
    ID_TYPE_PLATFORM    = 2,    // 平台设备编号类型
    ID_TYPE_DOMAIN_CMU  = 5     // 域或CMU平台编号类型
};

// 设备的功能分类定义
enum TnmDevCategory
{
    TNM_CAT_INVALID = 0,

    TNM_CAT_CMU     = 1,        // CMU

    TNM_CAT_VTDU    = 11,       // 码流转发
    TNM_CAT_NRU     = 12,       // 网络录放像
    TNM_CAT_VPU     = 13,       // 画面复合
    TNM_CAT_UAS     = 14,       // CMU的个性化数据服务器
    TNM_CAT_PUI     = 15,       // PU接入
    TNM_CAT_CUI     = 16,       // CU接入
    TNM_CAT_DNS     = 17,       // 负载均衡服务器Lds

    TNM_CAT_DVR         = 21,   // DVR带存储的编码器（带本地存储的视频服务器）
    TNM_CAT_ENCODER     = 22,   // DV 编码器（不带本地存储的视频服务器）
    TNM_CAT_IPCAMERA    = 23,   // IP-Camera
    TNM_CAT_DECODER     = 24,   // 解码器
	TNM_CAT_MTU			= 26,	//转码服务器

    TNM_CAT_VIDICON = 31,       // 摄像机
    TNM_CAT_YT      = 32,       // 云台
    TNM_CAT_IOIN    = 33,       // 报警输入设备－开关量/模拟量，如红外、烟感、门磁等
    TNM_CAT_INOUT   = 34,       // 报警输出设备:如警灯、警铃
    TNM_CAT_AIN     = 35,       // 语音输入设备
    TNM_CAT_AOUT    = 36,       // 语音输出设备
    TNM_CAT_MATRIX  = 37        // 矩阵
};

enum TnmPlatDevType
{
    TNM_PLAT_VTDU   = 1,        // 码流转发
    TNM_PLAT_NRU    = 2,        // 网络录放像
    TNM_PLAT_VPU    = 3,        // 画面复合
    TNM_PLAT_UAS    = 5,        // CMU的个性化数据服务器
    TNM_PLAT_DNS    = 6,        // 负载均衡服务器
    TNM_PLAT_PUI    = 7,        // PU接入
    TNM_PLAT_CUI    = 8,         // CU接入
	TNM_PLAT_MTU    = 9,		//MTU
};

// PU设备类型
enum TnmPuDevType
{
    TNM_PU_TYPE_MAIN    = 1,    // 主设备：视频服务器等负责音视频编码并向网络传送的设备
    TNM_PU_TYPE_PERI    = 2     // 外围设备：主设备连接的摄像机、云台等设备
};

// PU主设备类型
enum TnmPuMainType
{
    TNM_PU_MAIN_DVR         = 1,    // DVR带存储的编码器（带本地存储的视频服务器）
    TNM_PU_MAIN_ENCODER     = 2,    // DV 编码器（不带本地存储的视频服务器）
    TNM_PU_MAIN_IPCAMERA    = 3,    // IP-Camera
    TNM_PU_MAIN_DECODER     = 4     //解码器
};

// PU子设备类型
enum TnmPuSubType
{
    TNM_PU_SUB_VIDICON      = 1,    //摄像机
    TNM_PU_SUB_YT           = 2,    //云台
    TNM_PU_SUB_IOIN         = 3,    //报警输入设备－开关量/模拟量，如红外、烟感、门磁等
    TNM_PU_SUB_INOUT        = 4,    //报警输出设备:如警灯、警铃
    TNM_PU_SUB_AIN          = 5,    //语音输入设备
    TNM_PU_SUB_AOUT         = 6,    //语音输出设备
    TNM_PU_SUB_MATRIX       = 7     //矩阵
};

//代理类型
enum AgentType
{
    EM_CENTERAGENT,     //中心代理
    EM_SUBAGENT         //子代理
};

enum CommunityType
{
    READ_COMMUNITY,
    WRITE_COMMUNITY
};


//面板闪烁状态
enum TnmLedStatus
{
    TNM_LED_NONE = 0,        // 无
    TNM_LED_ON,              // 亮
    TNM_LED_OFF,             // 灭
    TNM_LED_EVERY_0_25_SEC,  // 0.25秒切换一次亮灭状态
    TNM_LED_EVERY_0_5_SEC,   // 0.5秒切换一次亮灭状态
    TNM_LED_EVERY_1_0_SEC,   // 1秒切换一次亮灭状态
    TNM_LED_EVERY_2_0_SEC    // 2秒切换一次亮灭状态
};

//以太网口状态
enum TnmEnternetStatus
{
    TNM_ETHERNET_NONE = 0,
    TNM_ETHERNET_ON,
    TNM_ETHERNET_OFF
};

//风扇状态
enum TnmFanStatus
{
    TNM_FAN_NONE = 0,
    TNM_FAN_ON,
    TNM_FAN_OFF
};

//电源状态
enum TnmPowerStatus
{
    TNM_POWER_NONE = 0,
    TNM_POWER_ON,
    TNM_POWER_OFF
};

//录放像状态
enum TnmRpState 
{
    RP_STATE_NULL = 0xFF,
    RECORD_RUNNING = 0,     // 正在录像
//    RECORD_WAITING = 1,       // 定时待录像
    PRERECORDING = 2,       // 正在预录
    PLAY_RUNNING = 10,      // 正在播放
    PLAY_PAUSING = 11, 
    FAST_FORWARDING = 12, 
    SLOW_FORWARDING = 13, 

    RP_STATE_STOP = 20      // 录放像停止状态s
};

//放像速率
enum TnmPlayRate
{
    INVALID_RATE    = 0,    // 无效值
    ONE_TIMES   = 1,        // 1倍速
    TWO_TIMES   = 2,        // 2倍速
    THREE_TIMES = 3,        // 3倍速
    FOUR_TIMES  = 4,        // 4倍速

    ONE_OF_TWO      = 10,   // 1/2 倍速
    ONE_OF_THREE    = 11,   // 1/3 倍速
    ONE_OF_FOUR     = 12,   // 1/4 倍速
    ONE_OF_EIGHT    = 13,   // 1/8 倍速
};

enum TnmPowType
{
    emPowStabilizer,   // 稳压电源
    emPowEthernet,    // 以太网供电
    emPowUps,        // UPS供电
    emPowBattery     // 电池供电
};

// 设备初始化参数
typedef struct tagTnmAdpInitParam
{
    tagTnmAdpInitParam(void)
    {
        dwLocalIp   = TNM_INVALED_IP;
        wLocalPort  = TNM_INVALED_PORT;
        dwRemoteIp  = TNM_INVALED_IP;
        wRemotePort = TNM_INVALED_PORT;
    }
    u32  dwLocalIp;            //[网络序](NMS代理 )NMS监听绑定的IP地址
                               //        (中心代理)中心代理监听绑定的IP地址
    
    u32  dwRemoteIp;           //[网络序](中心代理)要连接的NMS的IP地址
                               //        (子代理  )要连接的中心代理的IP地址

    u16  wLocalPort;           //[网络序](NMS代理 )NMS的监听绑定的端口
                               //        (中心代理)中心代理的监听绑定的端口(默认：1727)
    u16  wRemotePort;          //[网络序](中心代理)要连接的NMS的端口
                               //        (子代理  )要连接的中心代理的端口
}PACKED TTnmAdpInitParam;


//建链成功上报3as业务层数据结构
typedef struct tagTnmLinkInfo
{   
    tagTnmLinkInfo()
    {
        dwLinkInsId = 0;
        dwNmsIp = 0;
        wNmsPort = 0;

    }
    u32 dwLinkInsId;    //[网络序]链路Id;
    u32 dwNmsIp;        //[网络序]所连接网管Ip;
    u16 wNmsPort;       //[网络序]网管端口;
    u8  byReserved1;
    u8  byReserved2;
}PACKED TnmLinkInfo;

#define READ_FLAG     0
#define WRITE_FLAG    1
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*=============================================================================
  功    能： Agent读写回调函数定义
  参    数： u32 dwNodeName --     [IN]读写节点名(NODE_SYSPUBCFGINFO等)
             u8 byRWFlag    --     [IN]读写标志(READ_FLAG WRITE_FLAG)
             void * pBuf    -- [IN/OUT]读写内容
             u16 * pwBufLen -- [IN/OUT]读写内容的长度
             u32 dwUserData --     [IN]已读内容的累积长度
                                       或写内容的结束标志(SNMP_FINISHED/SNMP_NOFINISHED)

  返 回 值： SNMP_SUCCESS - 成功
             SNMP_NOFINISHED - 数据太大读写还没有完成
             其它 - 失败
=============================================================================*/
typedef u16 (*TTnmAgentCallBack)(u32 dwNodeName, u8 byRWFlag, void * pBuf, u16 * pwBufLen, u32 dwUserData);

//renhouping 2006.08.16 for 3as
typedef u32 (*TTnm3asAgentCallBack)(u32 dwNodeName, u8 byRWFlag, void * pBuf, u16 * pwBufLen, u32 dwUserData, u32 dwInsId);

u16 SnmpAdp_AgentInit(u8 byAgentType, TTnmAdpInitParam * ptAdpInitParam, TTnmAgentCallBack tAgentCallBack);
u16 SnmpAdp_SendTrap(u32 dwTrapType, void * pBuf, u16 wBufLen);
u16 SnmpAdp_CloseAgent(void);

#ifdef __cplusplus
}
#endif

class CTnmSnmpAdapter;

/**
 * 设备端适配接口类.
 */
class CTnmAgentAdapter
{
public:
    CTnmAgentAdapter();
    virtual ~CTnmAgentAdapter();

    /**
     * 初始化设备端适配对象函数
     * @param byAgentType - 代理类型(enum AgentType)
     * @param ptAgentInitParam - 代理初始化参数指针
     * @param byAgentIndex - 代理索引值
     * @return SNMP_SUCCESS - 成功  其它 - 失败
     */
    u16  Initialize(u8 byAgentType, TTnmAdpInitParam *ptAgentInitParam = NULL, u8 byAgentIndex = 0);

    /**
     * 3AS专用的初始化设备端适配对象函数
     * @param byAgentType - 代理类型(enum AgentType)
     * @param ptAgentInitParam - 代理初始化参数指针
     * @param byParamNum - 参数数组元素个数
     * @return SNMP_SUCCESS - 成功  其它 - 失败
     */
    u16  Initialize_3asAdp(u8 byAgentType, TTnmAdpInitParam *ptAgentInitParam = NULL, u8 byParamNum = 1);

    /**
     * 设置读写共同体
     * @param pchCommunity --- 共同体指针
     * @param byCommunityType --- 共同体类型
     * @return SNMP_SUCCESS - 成功  其它 - 失败
     */
    u16  SetCommunity(s8 * pchCommunity, u8 byCommunityType);

    /*=============================================================================
      函 数 名： SetAgentCallBack
      功    能： 设置Agent端的读写回调函数
      参    数： TTnmAgentCallBack tAgentCallBack --- 回调函数
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SetAgentCallBack(TTnmAgentCallBack tAgentCallBack);
    u16  SetAgentCallBack_3asAdp(TTnm3asAgentCallBack tAgentCallBack);    

    /*=============================================================================
      函 数 名： SendTrap
      功    能： Agent端发主动向NMS发送信息的接口函数
      参    数： u32 dwTrapType --- 要发送的消息类型, NODE消息或TRAP消息
                 void * pBuf --- 告警内容
                 u16 wBufLen --- 告警长度
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SendTrap(u32 dwTrapType, void * pBuf, u16 wBufLen);

    /*=============================================================================
      函 数 名： SendTrap
      功    能： 3as Agent端发主动向NMS发送信息的接口函数
      参    数： u32 dwTrapType --- 要发送的消息类型, NODE消息或TRAP消息
                 void * pBuf --- 告警内容
                 u16 wBufLen --- 告警长度
                 u16 wInsId --- 接收消息的3as端代理的实例号
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  SendTrap_3asAdp(u32 dwTrapType, void * pBuf, u16 wBufLen, u32 dwInsId);

    ///断链通知
    u16  DisconnectNotify(void);

    /*=============================================================================
      函 数 名： CloseAgent
      功    能： 结束Agent的函数
      参    数： 无
      返 回 值： SNMP_SUCCESS - 成功  其它 - 失败
    =============================================================================*/
    u16  CloseAgent(void);

private:
    CTnmSnmpAdapter * m_pcSnmpAdp;
};

const s8* GetTimeStr(u32 dwTime);
const s8* GetIpStr(u32 dwIpAddr);
const s8 * GetStatusStr(u8 byState);
inline const s8* GetAlarmCodeDesc(u32 dwAlarmCode);

typedef struct tagTnmDeviceId
{
    enum { INVALID_ID_CHAR = '0' };
    // ID信息是否为有效
    BOOL32 IsValid(void) const;
    //2006/03/13 李洪强 是否为根节点
    BOOL32 IsRoot() const;
    // 2006/03/21 李洪强 将ID置为空
    void SetNull(void);
    // 2006/03/21 李洪强 重载等号
    BOOL32 operator == ( const tagTnmDeviceId &tDevId ) const;
    // 2006/03/30 李洪强 设置ID值
    void SetId(s8 *pbyId, s32 nLen);
    void SetId(const s8 *pbyId)
    {
        SetId(const_cast<s8*>(pbyId), strlen(pbyId));
    }
    // 2006/03/30 李洪强 得到设备ID的字串指针
    s8* GetId() const;
    // 2006/04/13 李洪强 解析设备ID，得到设备的分类
    u32 GetDevCategory() const;
    // 2006/04/13 李洪强 得到设备分类的描述
    const s8* GetDevCatDesc() const;
    // 2006/04/13 李洪强 得到设备ID是哪一大类的设备
    u8 GetIdType() const;
    // 2006/04/13 李洪强 得到前端设备的类型
    u8  GetPuType() const;
    // 2006/04/13 李洪强 得到前端主设备的类型
    u8  GetPuMainType() const;
    // 2006/04/13 李洪强 得到前端的外围设备类型
    u8  GetPuSubType() const;
    // 2006/04/13 李洪强 得到平台设备的类型
    u8  GetPlatType() const;
    // 2006/04/13 李洪强 ID字串转数字
    u32 StrToNum(const s8 *pchId, s32 nLen) const;
    // 2006/04/26 李洪强 是否为独立显示于网管界面上的设备
    BOOL32 IsIndependDev() const;

    bool operator < ( const tagTnmDeviceId& tId ) const;

    s8  achID[TNM_MAX_DEVICE_ID_LEN + 4];
}PACKED TTnmDeviceId;

/*========================================================================
函 数 名：IsValid
功    能：判断ID是否为有效值
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/

inline bool TTnmDeviceId::operator <( const tagTnmDeviceId& tId )const
{
    int nCmp = strcmp( achID, tId.achID );
    return nCmp < 0;
}

inline BOOL32 TTnmDeviceId::IsValid(void) const
{
    for (u16 wLoop = 0; wLoop < TNM_MAX_DEVICE_ID_LEN; wLoop++)
    {
        if (INVALID_ID_CHAR != achID[wLoop])
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*========================================================================
函 数 名：IsRoot
功    能：是否为根节点
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/13  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TTnmDeviceId::IsRoot() const
{   
    if (GetDevCategory() != TNM_CAT_CMU) 
    {
        // 不是平台设备
        return FALSE;
    }

    s32 nNum = StrToNum(achID, 2);
    if (nNum < 1) 
    {
        return FALSE;
    }

    for(s32 nIndex = 2; nIndex < FIELD_LEN_CMS; nIndex++)
    {
        s8 chNum = achID[nIndex];
        if (chNum != '0') 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*========================================================================
函 数 名：SetNull
功    能：将ID置为空
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/
inline void TTnmDeviceId::SetNull(void)
{
    memset(achID, INVALID_ID_CHAR, TNM_MAX_DEVICE_ID_LEN);
    achID[TNM_MAX_DEVICE_ID_LEN] = '\0';
}

/*========================================================================
函 数 名：operator == 
功    能：重载等号
参    数：const tagTnmDeviceId &tDevId          [in]要比较的ID
返 回 值: TRUE：    相同：  FALSE： 不相同
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TTnmDeviceId:: operator == ( const tagTnmDeviceId &tDevId ) const
{
    if (strncmp(achID, tDevId.achID, TNM_MAX_DEVICE_ID_LEN) == 0) 
    {
        return TRUE;
    }
    return FALSE;
}

/*========================================================================
函 数 名：SetId
功    能：设置ID值
参    数：s8 *pbyId                             [in]要设置的值
          s32 nLen                              [in]字串的长度
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/30  3.9     李洪强  创建
========================================================================*/
inline void TTnmDeviceId::SetId(s8 *pbyId, s32 nLen)
{
    assert(pbyId);
    assert(nLen > 0);
    assert(nLen <= TNM_MAX_USER_ID_LEN);

    if (pbyId == NULL || nLen < 1) 
    {
        return;
    }

    if (nLen > TNM_MAX_USER_ID_LEN)
    {
        nLen = TNM_MAX_USER_ID_LEN;
    }
    s32 nCopyLen = nLen;
    memcpy(achID, pbyId, nCopyLen);
    memset(achID + nCopyLen, 0, TNM_MAX_USER_ID_LEN + 1 - nCopyLen);
}
/*========================================================================
函 数 名：GetId
功    能：得到设备ID的字串指针
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/30  3.9     李洪强  创建
========================================================================*/
inline s8* TTnmDeviceId::GetId() const
{
    return (s8*)achID;
}

/*========================================================================
函 数 名：GetDevCategory
功    能：解析设备ID，得到设备的分类
参    数：
返 回 值：设备的分类enum TnmDevCategory
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u32 TTnmDeviceId::GetDevCategory() const
{
    u8 byType = GetIdType();
    switch(byType) 
    {
    case ID_TYPE_PU:
        {
            u8 byPuType = GetPuType();
            if (byPuType == TNM_PU_TYPE_MAIN) 
            {
                u8 byMainType = GetPuMainType();
                switch(byMainType) 
                {
                case TNM_PU_MAIN_DVR:
                    return TNM_CAT_DVR;
                case TNM_PU_MAIN_ENCODER:
                    return TNM_CAT_ENCODER;
                case TNM_PU_MAIN_IPCAMERA:
                    return TNM_CAT_IPCAMERA;
                case TNM_PU_MAIN_DECODER:
                    return TNM_CAT_DECODER;
                default:
                    break;
                }
                return TNM_CAT_INVALID;
            }
            else if (byPuType == TNM_PU_TYPE_PERI) 
            {
                u8 bySubType = GetPuSubType();
                switch(bySubType) 
                {
                case TNM_PU_SUB_VIDICON:
                    return TNM_CAT_VIDICON;
                case TNM_PU_SUB_YT:
                    return TNM_CAT_YT;
                case TNM_PU_SUB_IOIN:
                    return TNM_CAT_IOIN;
                case TNM_PU_SUB_INOUT:
                    return TNM_CAT_INOUT;
                case TNM_PU_SUB_AIN:
                    return TNM_CAT_AIN;
                case TNM_PU_SUB_AOUT:
                    return TNM_CAT_AOUT;
                case TNM_PU_SUB_MATRIX:
                    return TNM_CAT_MATRIX;
                default:
                    break;
                }
                return TNM_CAT_INVALID;
            }
        }
        break;
    case ID_TYPE_DOMAIN_CMU:
        {
            return TNM_CAT_CMU;
        }
        //break;
    case ID_TYPE_PLATFORM:
        {
            u8 byPuType = GetPlatType();
            switch(byPuType) 
            {
            case TNM_PLAT_VTDU:
                return TNM_CAT_VTDU;
            case TNM_PLAT_NRU:
                return TNM_CAT_NRU;
            case TNM_PLAT_VPU:
                return TNM_CAT_VPU;
            case TNM_PLAT_UAS:
                return TNM_CAT_UAS;
            case TNM_PLAT_PUI:
                return TNM_CAT_PUI;
            case TNM_PLAT_CUI:
                return TNM_CAT_CUI;
            case TNM_PLAT_DNS:
                return TNM_CAT_DNS;
			case TNM_PLAT_MTU:
				return TNM_CAT_MTU;
            default:
                break;
            }
            //return TNM_CAT_INVALID;
        }
        break;
    default:
        break;
        //return TNM_CAT_INVALID;
    }

    return TNM_CAT_INVALID;
}


/*========================================================================
函 数 名：GetIdType
功    能：得到设备ID是哪一大类的设备
参    数：
返 回 值：TnmDevIdType
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u8 TTnmDeviceId::GetIdType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS, FIELD_LEN_ID_TYPE);
}

/*========================================================================
函 数 名：StrToNum
功    能：ID字串转数字
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u32 TTnmDeviceId::StrToNum(const s8 *pchId, s32 nLen) const
{
    if (nLen < 0) 
    {
        return 0;
    }
    if (nLen > TNM_MAX_DEVICE_ID_LEN) 
    {
        nLen = TNM_MAX_DEVICE_ID_LEN;
    }
    s8 achId[TNM_MAX_DEVICE_ID_LEN + 1];
    memset(achId, 0, sizeof(achId));
    memcpy(achId, pchId, nLen);
    return atoi(achId);
}

/*========================================================================
函 数 名：GetPuType
功    能：得到前端设备的类型
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u8 TTnmDeviceId::GetPuType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE, FIELD_LEN_PU_TYPE);
}

/*========================================================================
函 数 名：GetPuMainType
功    能：得到前端主设备的类型
参    数：
返 回 值：enum TnmPuMainType
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u8 TTnmDeviceId::GetPuMainType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE + FIELD_LEN_PU_TYPE,
            FIELD_LEN_PU_SUBTYPE);
}

/*========================================================================
函 数 名：GetPuSubType
功    能：得到前端的外围设备类型
参    数：
返 回 值：enum TnmPuSubType
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline u8 TTnmDeviceId::GetPuSubType() const
{
    return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE + FIELD_LEN_PU_TYPE,
            FIELD_LEN_PU_SUBTYPE);  
}
// 2006/04/13 李洪强 得到平台设备的类型
inline u8 TTnmDeviceId::GetPlatType() const
{
    u8 byPlatType = GetIdType();
    if (byPlatType == ID_TYPE_PLATFORM) 
    {
        return (u8)StrToNum(achID + FIELD_LEN_CMS + FIELD_LEN_ID_TYPE,
                FIELD_LEN_PLAT_DEV_TYPE);
    }
    return TNM_CAT_INVALID;
}

/*========================================================================
函 数 名：IsIndependDev
功    能：判断设备是否为独立显示于网管界面上的设备
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/26  3.9     李洪强  创建
========================================================================*/
inline BOOL32 TTnmDeviceId::IsIndependDev() const
{
    u32 dwCategory = GetDevCategory();
    if (dwCategory == TNM_CAT_VTDU
        || dwCategory == TNM_CAT_VPU
        || dwCategory == TNM_CAT_PUI
        || dwCategory == TNM_CAT_CUI) 
    {
        // 各单板不直接显示在网管的拓朴树中
        return FALSE;
    }

    return TRUE;
}


typedef struct tagTnmUserId
{
    tagTnmUserId()
    {
        memset(achID, 0, sizeof(achID));
    }
    s8  achID[TNM_MAX_USER_ID_LEN + 1];
    u8  byReserved1;
    u8  byReserved2;
    u8  byReserved3;
}PACKED TTnmUserId;


/******************************************************************************/
/*                                    结构定义                                */
/*注意：以下所有结构里的成员变量，在模块间的接口调用或网络发送时都应该为网络序*/
/******************************************************************************/
/********************************************************************/
/* 1.   配置管理：配置参数的获取和设置                              */
/********************************************************************/
/*====================================================================
1.1.    公共配置参数
====================================================================*/
typedef struct tagTnmUnitType
{
    u8  byDeviceType;       // 设备的硬件型号（只读）(enum TnmUnitType)
    u8  byServiceType;      // 业务类型（只读）(enum TnmServiceType)
    //u8  byReqConfigFile;    // 设备在注册时请求上传配置文件 (TNM_DISABLE/TNM_ENABLE)
    //u8    byPowType;         // 供电方式  (TnmPowType)
    u16 wReserved;
}TTnmUnitType;

typedef struct tagTnmPubCfgInfo
{
    tagTnmPubCfgInfo()
    {
        memset(this, 0, sizeof(tagTnmPubCfgInfo));
    }
    u32 dwIpAddr;               // 设备IP地址
    u32 dwSubnetMask;           // 设备子网掩码
    u32 dwGateWay;              // 网关地址
    TTnmUnitType tUnitType;     // 设备类型
    u32 dwNmsIpAddr;            // 网管服务器IP地址（可读写）
    u16 wTrapPort;              // 网管服务器告警端口号（只读）
    u16 wGetSetPort;            // 代理端读写监听端口号（只读）
//    s8  achGetCommunity[TNM_MAX_COMMUNITY_LEN + 1]; //查询共同体名(可读写)
//    s8  achSetCommunity[TNM_MAX_COMMUNITY_LEN + 1]; //设置共同体名(可读写)
    u8  byMemThreshold;         // 系统内存使用阀值（可读写, 0-100）
    u8  byCpuThreshold;         // 系统CPU使用阀值（可读写, 0-100）
//  u8  byLostPackThreshold;    // 视频丢包告警阀值（可读写，0-100）
    u16 wAcceptBandwidth;           // 接入带宽 单位 kbsp
    /////u32 dwRealIpAddr;          // 设备真实IP地址(从服务器获取的地址,如PPPoE)

    const tagTnmPubCfgInfo * GetPubCfgPtr() const
    {
        return this;
    }
    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}TTnmPubCfgInfo;

typedef struct tagTnmServerSysCfgInfo
{
    tagTnmServerSysCfgInfo()
    {
        memset(this, 0, sizeof(tagTnmServerSysCfgInfo));
    }

    s8  m_achModuleTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1]; //模块类型
    TTnmDeviceId m_tModuleId;                              //模块编号
    s8  m_achModuleName[TNM_MAX_STRING_LEN + 4];           //模块名称
}TTnmServerSysCfgInfo;

inline void TTnmPubCfgInfo::Print(BOOL32 bNetToHost)
{
    if (bNetToHost) 
    {
        NetToHost();
    }

    OspPrintf(TRUE, FALSE, "IP=%s,", ::GetIpStr(dwIpAddr));
    OspPrintf(TRUE, FALSE, "SMask=%s, ", ::GetIpStr(dwSubnetMask));
    OspPrintf(TRUE, FALSE, "GW: %s;\n", ::GetIpStr(dwGateWay));
    OspPrintf(TRUE, FALSE, "\tDType: %d; SType: %d; NmsIP: %s; TrapPort: 0x%hx;\n",
            tUnitType.byDeviceType, tUnitType.byServiceType,
            ::GetIpStr(dwNmsIpAddr), wTrapPort);
    OspPrintf(TRUE, FALSE, "\tSetPort:0x%hx; MThreshold: %d; CThreshold: %d;\n", 
            wGetSetPort, byMemThreshold, byCpuThreshold);
    if (bNetToHost) 
    {
        HostToNet();
    }
}

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPubCfgInfo::NetToHost()
{
    dwIpAddr = ntohl(dwIpAddr);
    dwSubnetMask = ntohl(dwSubnetMask);
    dwGateWay = ntohl(dwGateWay);
    dwNmsIpAddr = ntohl(dwNmsIpAddr);
    wTrapPort = ntohs(wTrapPort);
    wGetSetPort = ntohs(wGetSetPort);
    wAcceptBandwidth = ntohs(wAcceptBandwidth);
    /////dwRealIpAddr = ntohl(dwRealIpAddr);
}
inline void TTnmPubCfgInfo::HostToNet()
{
    dwIpAddr = htonl(dwIpAddr);
    dwSubnetMask = htonl(dwSubnetMask);
    dwGateWay = htonl(dwGateWay);
    dwNmsIpAddr = htonl(dwNmsIpAddr);
    wTrapPort = htons(wTrapPort);
    wGetSetPort = htons(wGetSetPort);
    wAcceptBandwidth = htons(wAcceptBandwidth);
    /////dwRealIpAddr = htonl(dwRealIpAddr);
}
/*====================================================================
1.2.    前端设备配置参数
====================================================================*/
// 1.2.0  前端设备能力集
typedef struct tagTnmPuCapSet
{
    tagTnmPuCapSet()
    {
        memset(this, 0, sizeof(tagTnmPuCapSet));
    }
    u8  byVidEncChannelNum;     //视频编码通道数量
    u8  byVidDecChannelNum;     //视频解码通道数量
    u8  byAudEncChannelNum;     //音频编码通道数量
    u8  byAudDecChannelNum;     //音频解码通道数量
    u8  bySerialPortNum;        //串口数量
    u8  byLPortInputNum;        //并口输入通道数量
    u8  byLPortOutputNum;       //并口输出通道数量
    u8  byVideoSrcNum;          //视频源端口数量

    u8  byVidAdpterNum;         //视频转压通道数量
    u8  byVidCompNum;           //图像合成通道数量
    u8  byVidSrcPollNum;        //视频源轮询通道数量
    u8  byAudMixNum;            //音频混音通道数量
    u8  byPlayerNum;            //放像通道数量
    u8  byFileDwnldNum;         //文件下载数量
    u8  byRecorderNum;          //录像通道数量(同视频编码通道)
    u8  byMaxStreamSendNum;     //单个通道最大发送能力(一个通道包括视频和音频)
    u8  byMaxStreanDirSndNum;   //单个通道的最大直传路数(一个通道包括视频和音频)
    u8  byVGANum;               //视频输出VGA接口个数
    u8  byVidOutNum;            //视频输出接口个数

    u8  byOsdColorNum;          //台标颜色数(TNM_OSD_COLOR_16\TNM_OSD_COLOR_256)
    u8  byMoveMotionAreaNum;    //移动侦测区域最大个数
    u8  byShadeAreaNum;         //图像遮挡区域最大个数
    u8  byStreamEncrypt;        //码流加密
    u8  byEtherNetNum;          //以太网口数量
    u8  byWirelessNum;          //无线网口数量
    u8  byDevPowerType;         //设备供电方式

    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE);

}TTnmPuCapSet;

inline void TTnmPuCapSet::Print(BOOL32 bNetToHost)
{
    OspPrintf(TRUE, FALSE, "VECNun: %d; VDCNun: %d; AECNum: %d; ADCNum: %d\n",
                byVidEncChannelNum, byVidDecChannelNum, byAudEncChannelNum, byAudDecChannelNum);
    OspPrintf(TRUE, FALSE, "\tSPNum: %d; LCInNum: %d; LCOutNum: %d; VSrcNum: %d;\n",
                bySerialPortNum, byLPortInputNum, byLPortOutputNum, byVideoSrcNum);
    OspPrintf(TRUE, FALSE, "\tVidAdpterNum: %d, byVidCompNum: %d, byVidSrcPollNum: %d",
                byVidAdpterNum, byVidCompNum, byVidSrcPollNum);
    OspPrintf(TRUE, FALSE, "\tbyAudMixNum: %d, byPlayerNum: %d, byFileDwnldNum: %d\n",
                byAudMixNum, byPlayerNum, byFileDwnldNum);
    OspPrintf(TRUE, FALSE, "\tbyRecorderNum: %d, byMaxStreamSendNum: %d, byMaxStreanDirSndNum: %d",
                byRecorderNum, byMaxStreamSendNum, byMaxStreanDirSndNum);
    OspPrintf(TRUE, FALSE, "\tbyVGANum: %d, byVidOutNum: %d\n", byVGANum, byVidOutNum);

    OspPrintf(TRUE, FALSE, "\tbyOsdColorNum: %d, byMoveMotionAreaNum: %d, byShadeAreaNum: %d",
                byOsdColorNum, byMoveMotionAreaNum, byShadeAreaNum);
    OspPrintf(TRUE, FALSE, "\tbyStreamEncrypt: %d, byEtherNetNum: %d, byWirelessNum: %d, byDevPowerType: %d",
                byStreamEncrypt, byEtherNetNum, byWirelessNum, byDevPowerType);
}

/*====================================================================
1.2.1.  系统参数
====================================================================*/
//系统参数结构
typedef struct tagTnmPuSysCfgInfo : public TTnmPubCfgInfo
{
    tagTnmPuSysCfgInfo()
    {
        memset(achName, 0, sizeof(achName));
        dwRegCMUIpAddr = 0;
        dwSearchMultiIpAddr = 0;
        memset(achWebUsername, 0, sizeof(achWebUsername));
        memset(achWebPassword, 0, sizeof(achWebPassword));
        byNATPacketSend = TNM_DISABLE;
        wAlarmRecoverTime = 0;
        byDiskType = TNM_SINGLE_DISK;
        memset(achBoxID, 0, sizeof(achBoxID));
        memset(achDevTypeName, 0, sizeof(achDevTypeName));
    }
    TTnmDeviceId tDevId;                                //调备的唯一标识
    s8  achName[TNM_MAX_STRING_LEN + 4];                //设备名
    u32 dwRegCMUIpAddr;                                 //设备主动注册的平台地址（若非组播地址则主动
                                                        //向该地址注册，可读写）
    u32 dwSearchMultiIpAddr;                            //搜索的组播地址（可读写）
    s8  achWebUsername[TNM_MAX_USER_ID_LEN + 4];        //Web服务登陆用户名（可读写）
    s8  achWebPassword[TNM_MAX_PASSWORD_LEN + 4];       //Web服务登陆密码（可读写）
    u16 wAlarmRecoverTime;                              //告警恢复时间(秒,可读写)
    u8  byNATPacketSend;                                //是否发送NAT探测包(可读写) TNM_ENABLE/TNM_DISABLE
    u8  byDiskType;                                     //设备分区类型（可读写）
                                                        //TNM_SINGLE_DISK/TNM_MULTI_DISK
    u8  byLostPackThreshold;                            //视频丢包告警阀值（可读写，0-100）
    s8  achWebGuestname[TNM_MAX_USER_ID_LEN + 4];       //web guest name
    s8  achWebGuestPassword[TNM_MAX_PASSWORD_LEN + 4];  //web guest pwd
    u8  byRegCMUIpType;                                 //是直接注册到平台，还是通过负载均衡服务器请求平台地址。
                                                        //TNM_REG_CMU/TNM_REG_DNS
    u8  byHeartBeatSecond;                              //与平台的心跳，单位（秒）范围（1--60）
    u8  byHeartBeatLostNum;                             //丢失多少次心跳认为断链

    u32 dwVersionServerAddr;                            //版本服务器地址 （0：无效）
    u16 wVersionServerPort;                             //版本服务器端口
    u16 wReserved;
    s8  achVerServUsername[TNM_MAX_USER_ID_LEN + 4];    //版本服务器登陆用户名（可读写）
    s8  achVerServPassword[TNM_MAX_PASSWORD_LEN + 4];   //版本服务器登陆密码（可读写）
    s8  achVersionPath[TNM_MAX_VERSION_PATH_LEN + 4];   //版本文件所在的路径
    u16 wCheckVersionMinute;                            //版本检查的周期（单位：分钟  0：无效）
    u8  byStreamNum;                                    //最大码流连接数
    u8  byReserved;
    s8  achDeviceLocation[TNM_MAX_STRING_LEN + 4];      //设备所在的位置描述

    s8  achDevTypeName[TNM_MAX_DEVTYPE_NAME_LEN + 1];   //设备类型名(只读)

    u32 dwRealIpAddr;                                   //设备真实IP地址(从服务器获取的地址,如PPPoE)
    u8  byReqConfigFile;                                //设备在注册时请求上传配置文件 (TNM_DISABLE/TNM_ENABLE)
    u8  byPowType;                                      //供电方式  (TnmPowType)
    u16 wReserved1;

    s8  achBoxID[MAX_BOX_ID_LEN + 4];   //机框标识

    u8  abyReserved[28];

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tName=%s; RegIp=%s, ",  achName, ::GetIpStr(dwRegCMUIpAddr));
        OspPrintf(TRUE, FALSE, "MulIp: %s;\n", ::GetIpStr(dwSearchMultiIpAddr));
        OspPrintf(TRUE, FALSE, "\tWevUsrName: %s; WebPwd: %s; NAT: %d;\n", 
            achWebUsername, achWebPassword, byNATPacketSend);
        OspPrintf(TRUE, FALSE, "\tAResumTime: 0x%hx; DiskType: %d; \n",
            wAlarmRecoverTime, byDiskType);
        OspPrintf(TRUE, FALSE, "\tGuestWebName:%s; pwd:%s;\n", achWebGuestPassword, achWebGuestPassword);
        OspPrintf(TRUE, FALSE, "\tachBoxID:%s\n", achBoxID);
        OspPrintf(TRUE, FALSE, "\tachDevTypeName:%s\n", achDevTypeName);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuSysCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIpAddr = ntohl(dwRegCMUIpAddr);
    dwSearchMultiIpAddr = ntohl(dwSearchMultiIpAddr);
    wAlarmRecoverTime = ntohs(wAlarmRecoverTime);
    dwVersionServerAddr = ntohl(dwVersionServerAddr);
    wCheckVersionMinute = ntohs(wCheckVersionMinute);
    wVersionServerPort = ntohs(wVersionServerPort);
    dwRealIpAddr = ntohl(dwRealIpAddr);
}
inline void TTnmPuSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwRegCMUIpAddr = htonl(dwRegCMUIpAddr);
    dwSearchMultiIpAddr = htonl(dwSearchMultiIpAddr);
    wAlarmRecoverTime = htons(wAlarmRecoverTime);
    dwVersionServerAddr = htonl(dwVersionServerAddr);
    wCheckVersionMinute = htons(wCheckVersionMinute);
    wVersionServerPort = htons(wVersionServerPort);
    dwRealIpAddr = htonl(dwRealIpAddr);
}

// 前端存储系统参数, 只对2417/2428有效
typedef struct tagTnmPuStoreSysCfgInfo : public TTnmPuSysCfgInfo
{
    tagTnmPuStoreSysCfgInfo()
    {
        byDiskFullPolicy = TNM_DISK_POLICY_STOP_REC;
        wShortFileRecTime = 60;
        byMaxFreeDiskSpaceRate = 10;
    }

    u8  byDiskFullPolicy;                       // 磁盘满的覆盖策略（可读写）
                                                //  TNM_DISK_POLICY_STOP_REC
                                                //  TNM_DISK_POLICY_REPLACE_ALL
                                                //  TNM_DISK_POLICY_KEEP_ALARMREC
    u8  byMaxFreeDiskSpaceRate;                 // 磁盘最大剩余空间（可读写,100%）
    u16 wShortFileRecTime;                      // 小文件录制时长（可读写,分钟）
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPuSysCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tDiskFuPoli: %d; sFileRecTime: 0x%hx; MaxFreeSpace: %d%%;\n", 
                byDiskFullPolicy, ntohs(wShortFileRecTime), byMaxFreeDiskSpaceRate);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tDiskFuPoli: %d; sFileRecTime: 0x%hx; MaxFreeSpace: %d%%;\n", 
                byDiskFullPolicy, wShortFileRecTime, byMaxFreeDiskSpaceRate);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuStoreSysCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuStoreSysCfgInfo::NetToHost()
{
    TTnmPuSysCfgInfo::NetToHost();

    wShortFileRecTime = ntohs(wShortFileRecTime);
}
inline void TTnmPuStoreSysCfgInfo::HostToNet()
{
    TTnmPuSysCfgInfo::HostToNet();
    
    wShortFileRecTime = htons(wShortFileRecTime);
}

/*====================================================================
1.2.2.  PPPOE参数
====================================================================*/
//PPPOE参数结构
typedef struct tagTnmPPPOECfgInfo
{   
    tagTnmPPPOECfgInfo()
    {
        byPPPOEEnable = TNM_ENABLE;
        memset(achADSLUserName, 0, sizeof(achADSLUserName));
        memset(achADSLPassword, 0, sizeof(achADSLPassword));
        byAutoDialEnable = TNM_ENABLE;
        dwAutoDialWait = 60;
        dwAutoDialTry = 3;
        byDebugEnable = TNM_ENABLE;
        wLcpEchoSendInterval = 60;
        wLcpEchoRetryTimes = 3;
    }
    s8  achADSLUserName[TNM_MAX_USER_ID_LEN + 4];   // ADSL拨号用户名（可读写）
    s8  achADSLPassword[TNM_MAX_PASSWORD_LEN + 4];  // ADSL拨号密码（可读写）
    u8  byPPPOEEnable;              // PPPOE使能状态（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byAutoDialEnable;           // 自动拨号使能状态（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byDebugEnable;              // Debug使能状态（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    u32 dwAutoDialWait;             // 自动拨号等待时间（可读写，秒)
    u32 dwAutoDialTry;              // 自动拨号重试次数（可读写）
    u16 wLcpEchoSendInterval;       // 建链后，发送LCP-ECHO包的时间间隔（可读写，秒）
    u16 wLcpEchoRetryTimes;         // 发送几次LCP-ECHO包没有得到相应则认为链接断开（可读写）

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "En: %d; UsrName: %s; Pwd: %s;\n", 
            byPPPOEEnable, achADSLUserName, achADSLPassword);

        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tADialEn: %d; ADialWait: 0x%lx; DialTry: 0x%lx;\n",
            byAutoDialEnable, dwAutoDialWait, dwAutoDialTry);
        OspPrintf(TRUE, FALSE, "\tDebugEn: %d; Interval:0x%hx; Times:0x%hx;\n", 
            byDebugEnable, wLcpEchoSendInterval, wLcpEchoRetryTimes);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPPPOECfgInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPPPOECfgInfo::NetToHost()
{
    dwAutoDialWait = ntohl(dwAutoDialWait);
    dwAutoDialTry = ntohl(dwAutoDialTry);
    wLcpEchoSendInterval = ntohs(wLcpEchoSendInterval);
    wLcpEchoRetryTimes = ntohs(wLcpEchoRetryTimes);
}
inline void TTnmPPPOECfgInfo::HostToNet()
{
    dwAutoDialWait = htonl(dwAutoDialWait);
    dwAutoDialTry = htonl(dwAutoDialTry);
    wLcpEchoSendInterval = htons(wLcpEchoSendInterval);
    wLcpEchoRetryTimes = htons(wLcpEchoRetryTimes);
}

/*====================================================================
1.2.3.  视频编码参数表（按通道）
====================================================================*/
typedef struct tagTnmVideoTypeSet
{
    u8  bySN4;
    u8  byMPEG4;
    u8  byH261;
    u8  byH263;
    u8  byH264;
    u8  byReserved;
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tSN4=%d, MPEG4=%d, H261=%d, H263=%d, H264=%d\n",
                bySN4, byMPEG4, byH261, byH263, byH264);
    }
}PACKED TTnmVideoTypeSet;

typedef struct tagTnmVideoResolution
{
    u8  byAuto;
    u8  byQCIF;
    u8  byCIF;
    u8  by2CIF;
    u8  by4CIF;
    u8  byReserved;
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tAuto=%d, QCIF=%d, CIF=%d, 2CIF=%d, 4CIF=%d\n",
            byAuto, byQCIF, byCIF, by2CIF, by4CIF);
    }   
}PACKED TTnmVideoResolution;

//视频图象分辨率
typedef struct tagTnmVideoResolutionSet
{
    TTnmVideoResolution tSN4;
    TTnmVideoResolution tMEPG4;
    TTnmVideoResolution tH261;
    TTnmVideoResolution tH263;
    TTnmVideoResolution tH264;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "\tSN4:");
        tSN4.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tMEPG4:");
        tMEPG4.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH261:");
        tH261.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH263:");
        tH263.Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tH264:");
        tH264.Print(bNetToHost);
    }
}PACKED TTnmVideoResolutionSet;

typedef struct tagTnmVideoEncCfgInfo
{
    TTnmVideoTypeSet tVideoTypeSet; //设备支持的视频编码格式能力集（只读）
    TTnmVideoResolutionSet tVideoResolutionSet; //设备支持的视频分辨率能力集（只读）
    u8  byEncodeNo;         // 视频编码通道号
    u8  byVideoType;        // 视频编码格式（可读写）
                            // TNM_VIDEO_TYPE_NONE
                            // TNM_VIDEO_TYPE_SN4
                            // TNM_VIDEO_TYPE_MPEG4
                            // TNM_VIDEO_TYPE_H261
                            // TNM_VIDEO_TYPE_H263
                            // TNM_VIDEO_TYPE_H264
    u8  byVideoResolution;  // 视频分辨率（可读写）
                            // TNM_VIDEO_RESOLUTION_NONE
                            // TNM_VIDEO_RESOLUTION_AUTO
                            // TNM_VIDEO_RESOLUTION_QCIF
                            // TNM_VIDEO_RESOLUTION_CIF
                            // TNM_VIDEO_RESOLUTION_2CIF
                            // TNM_VIDEO_RESOLUTION_4CIF
    u8  byFrameRate;        // 帧率 1-25（可读写）
    u32 dwBitRate;          // 编码码率 64K-4M（可读写）kbps
    u8  byQuality;          // 图象质量 （可读写）TNM_VIDEO_QUALITY_PRIOR/TNM_VIDEO_SPEED_PRIOR
    u8  byMaxQuantization;  // 最大量化因子 1-31（可读写）
    u8  byMinQuantization;  // 最小量化因子 1-31（可读写）
    s8  chBrightness;       // 亮度 -127～128（可读写）
    u16 wInterval;          // 关键帧间隔 25-2500（可读写）
    s8  chContrast;         // 对比度 -127～128（可读写）
    s8  chSaturation;       // 饱和度 -127～128（可读写）
    s8  chColourGrading;      // 色度0～255（可读写）
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "EncNo: %d; VType: %d; VResol: %d; FRate: %d;\n",
            byEncodeNo, byVideoType, byVideoResolution, byFrameRate);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tQuty: %d; BRate: 0x%lx; MaxQu: %d; MinQu: %d;\n",
            byQuality, dwBitRate, byMaxQuantization, byMinQuantization);
        OspPrintf(TRUE, FALSE, "\tInterval: 0x%hx; Bright: %d; Contr: %d; Satur: %d;\n", 
            wInterval, chBrightness, chContrast, chSaturation);
        if (bNetToHost) 
        {
            HostToNet();
        }
        tVideoTypeSet.Print(bNetToHost);
        tVideoResolutionSet.Print(bNetToHost);

    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoEncCfgInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVideoEncCfgInfo::NetToHost()
{
    dwBitRate = ntohl(dwBitRate);
    wInterval = ntohs(wInterval);
}
inline void TTnmVideoEncCfgInfo::HostToNet()
{
    dwBitRate = htonl(dwBitRate);
    wInterval = htons(wInterval);
}

/*====================================================================
1.2.4.  视频解码参数表（按通道）
====================================================================*/
typedef struct tagTnmVideoDecCfgInfo
{
    TTnmVideoTypeSet tVideoTypeSet;     //设备支持的视频解码格式能力集（只读）
    u8  byDecodeNo;             // 视频解码通道号
    u8  byVideoType;            // 视频解码格式（可读写）TNM_VIDEO_TYPE_H263
    u8  byDecDealLev;           // 图像后处理级别(0～3级,0表示没有)（可读写）
    u8  byVGAOutEnable;         // VGA输出使能(只读) TNM_DISABLE, TNM_ENABLE
    u8  byVideoOutputType;      // 视频输出端口类型 （可读写）
                                //  TNM_VIDEO_OUT_TYPE_C
                                //  TNM_VIDEO_OUT_TYPE_S
                                //  TNM_VIDEO_OUT_TYPE_VGA
    u8  byVGAFrequency;         // VGA输出刷新频率（可读写）
                                // TNM_VGA_OUT_FREQUENCY_60
                                // TNM_VGA_OUT_FREQUENCY_75
                                // TNM_VGA_OUT_FREQUENCY_85
    u8  byStarveMode;           // 无视频的显示方式 (可读写)
                                //  TNM_STAEVE_MODE_COLOR_BAR
                                //  TNM_STARVE_MODE_LAST_IMAGE
                                //  TNM_STARVE_MODE_BLACK_SCR
                                //  TNM_STARVE_MODE_SHOW_OSD
    u8  byResendEnable;         // 解码重传使能状态 TNM_DISABLE/TNM_ENABLE
    u16 wResend1stTimeSpan;     // 解码第1个重传检测点
    u16 wResend2ndTimeSpan;     // 解码第2个重传检测点
    u16 wResend3rdTimeSpan;     // 解码第3个重传检测点
    u16 wRejectTimeSpan;        // 解码过期时间

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "DecNo: %d; VType: %d; DecLev: %d; VGAout: %d;\n",
            byDecodeNo, byVideoType, byDecDealLev, byVGAOutEnable);
        OspPrintf(TRUE, FALSE, "\tVOutType: %d; VGAFre: %d; Starve: %d; Resnd: %d;\n",
            byVideoOutputType, byVGAFrequency, byStarveMode, byResendEnable);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tResnd1:0x%hx; Resnd2:0x%hx; Resnd3: 0x%hx;Reject: 0x%hx;\n",
                ntohs(wResend1stTimeSpan), ntohs(wResend2ndTimeSpan), ntohs(wResend3rdTimeSpan),
                ntohs(wRejectTimeSpan));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tResnd1:0x%hx; Resnd2:0x%hx; Resnd3: 0x%hx;Reject: 0x%hx;\n",
                wResend1stTimeSpan, wResend2ndTimeSpan, wResend3rdTimeSpan, wRejectTimeSpan);
        }
        OspPrintf(TRUE, FALSE, "\t");
        tVideoTypeSet.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoDecCfgInfo;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVideoDecCfgInfo::NetToHost()
{
    wResend1stTimeSpan = ntohs(wResend1stTimeSpan);
    wResend2ndTimeSpan = ntohs(wResend2ndTimeSpan);
    wResend3rdTimeSpan = ntohs(wResend3rdTimeSpan);
    wRejectTimeSpan = ntohs(wRejectTimeSpan);
}
inline void TTnmVideoDecCfgInfo::HostToNet()
{
    wResend1stTimeSpan = htons(wResend1stTimeSpan);
    wResend2ndTimeSpan = htons(wResend2ndTimeSpan);
    wResend3rdTimeSpan = htons(wResend3rdTimeSpan);
    wRejectTimeSpan = htons(wRejectTimeSpan);
}

/*====================================================================
1.2.5.  音频编解码参数表（按通道）
====================================================================*/
//音频压缩格式
typedef struct tagTnmAudioTypeSet
{
    u8 byPCMA;
    u8 byPCMU;
    u8 byGSM;
    u8 byG729;
    u8 byADPCM;
    u8 byG7221C;
    u8 byReserved1;
    u8 byReserved2;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "PCMA: %d; PCMU: %d; GSM: %d; G729: %d; ADPCM: %d; G7221C: %d\n;",
            byPCMA, byPCMU, byGSM, byG729, byADPCM, byG7221C);
    }
}TTnmAudioTypeSet;

// 音频编码参数
typedef struct tagTnmAudioEncCfgInfo
{
    TTnmAudioTypeSet tEncTypeSet;   //设备支持的音频编码格式能力集（只读）
    u8  byChannelNo;                //  
    u8  byEncType;                  //音频编码格式（可读写）
                                    //  TNM_AUDIO_TYPE_PCMA
                                    //  TNM_AUDIO_TYPE_PCMU
                                    //  TNM_AUDIO_TYPE_GSM
                                    //  TNM_AUDIO_TYPE_G729
    u8  byInputType;                //音频输入类型（可读写）
                                    //  TNM_AUDIO_INPUTTYPE_LINEIN
                                    //  TNM_AUDIO_INPUTTYPE_MIC
    u8  byEncVolume;                //编码音量（可读写） 0-25
    u8  byAECEnable;                //回声抵消使能状态（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byDumbEnable;               //哑音使能状态（可读写） TNM_DISABLE/TNM_ENABLE
    u8  byCapInput;                 //音频输入能力-相与的结果为总能力
                                    //  TNM_CAP_AUDIO_INPUT_LINEIN | TNM_CAP_AUDIO_INPUT_MIC
    u8  byReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChNo: %d; EncT: %d; InT: %d; Vol: %d; AEC: %d; Dumb: %d; CapIn: 0x%x\n",
            byChannelNo, byEncType, byInputType, byEncVolume, byAECEnable, byDumbEnable, byCapInput);
        OspPrintf(TRUE, FALSE, "\t");
        tEncTypeSet.Print(bNetToHost);
    }
}TTnmAudioEncCfgInfo;

// 音频解码参数
typedef struct tagTnmAudioDecCfgInfo
{
    TTnmAudioTypeSet tDecTypeSet;   // 设备支持的音频解码格式能力集（只读）
    u8  byChannelNo;                // 音频解码通道号
    u8  byDecVolume;                // 解码音量（可读写） 0-25
    u8  byMuteEnable;               // 静音使能状态（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChNo: %d; Vol: %d; Mute: %d;\n",
            byChannelNo, byDecVolume, byMuteEnable);
        OspPrintf(TRUE, FALSE, "\t");
        tDecTypeSet.Print(bNetToHost);
    }
}PACKED TTnmAudioDecCfgInfo;

/*====================================================================
1.2.6.  视频源输入端口参数表（按端口）
====================================================================*/
typedef struct tagTnmVideoInputCfgInfo
{
    u8  byVidInputNo;               // 视频源输入端口号
    u8  byCameraType;               // 摄像头类型（可读写）
    u8  byCameAddrID;               // 摄像头地址码 1-255（可读写）
    u8  bySerialPortID;             // 摄像头对应串口ID（可读写，根据能力集）
    s8  achVideoPortName[TNM_MAX_VIDEOIN_PORT_NAME_LEN + 4]; //视频源端口名（可读写）
    s8  achVideoPortOSD[TNM_MAX_STRING_LEN + 4]; //视频源字幕内容（可读写）
    u16 wVideoPortOSDLeftMargin;    // 视频源字幕左边距 1-200（可读写）
    u16 wVideoPortOSDTopMargin;     // 视频源字幕上边距 1-200（可读写）
    u32 dwVideoPortOSDColor;        // 视频源字幕颜色（可读写）

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "VidNo: %d; CamType: %d; CamID: %d; SPortID: %d;\n",
            byVidInputNo, byCameraType, byCameAddrID, bySerialPortID);
        OspPrintf(TRUE, FALSE, "\tName: %s; OSD: %s;\n", achVideoPortName, achVideoPortOSD);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLMar: 0x%hx; TMar: 0x%hx; OSDClr: 0x%x;\n", 
                ntohs(wVideoPortOSDLeftMargin), ntohs(wVideoPortOSDTopMargin),
                ntohl(dwVideoPortOSDColor));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLMar: 0x%hx; TMar: 0x%hx; OSDClr: 0x%lx;\n", 
                wVideoPortOSDLeftMargin, wVideoPortOSDTopMargin, dwVideoPortOSDColor);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVideoInputCfgInfo;

//摄像头类型
typedef struct tagTnmCameraInfo
{ 
    u8  byCameraType;               // 摄像头类型
    u8  byReserved;
    u16 wReserved;
    s8  achCameraTypeName[TNM_MAX_CAMERA_NAME_LEN + 4]; //摄像头类型名
}TTnmCameraInfo;


/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVideoInputCfgInfo::NetToHost()
{
    wVideoPortOSDLeftMargin = ntohs(wVideoPortOSDLeftMargin);
    wVideoPortOSDTopMargin = ntohs(wVideoPortOSDTopMargin);
    dwVideoPortOSDColor = ntohl(dwVideoPortOSDColor);
}
inline void TTnmVideoInputCfgInfo::HostToNet()
{
    wVideoPortOSDLeftMargin = htons(wVideoPortOSDLeftMargin);
    wVideoPortOSDTopMargin = htons(wVideoPortOSDTopMargin);
    dwVideoPortOSDColor = htonl(dwVideoPortOSDColor);
}

/*====================================================================
1.2.7.  并口参数表（按端口）
====================================================================*/
//并口输入参数结构
typedef struct tagTnmLPTCfgInfo
{
    u8  bySerialNo;             // 端口号
    u8  byAlarmDetectState;     // 告警检测状态（可读写）
                                //  TNM_ALARM_DETECT_NONE
                                //  TNM_ALARM_DETECT_ONCE
                                //  TNM_ALARM_DETECT_REPEAT
                                //  TNM_ALARM_DETECT_ALWAYS
    u8  byRelateEncId;          // 告警关联的编码通道 （可读写）无关联编码通道则为 TNM_INVALID_CHANNEL_ID
    u8  byOutputLptId;          // 告警自动控制输出并口ID （可读写）无控制输出则为 TNM_INVALID_CHANNEL_ID
    u8  byAlarmOSDEnable;       // 告警字幕使能状态 TNM_DISABLE-不显示, TNM_ENABLE-显示
    u8  byReserved;
    u16 wReserved;
    u32 dwAlarmDetectStartTime[TNM_DAYS_OF_WEEK]; //告警检测开始时间（可读写）
    u32 dwAlarmDetectDuration[TNM_DAYS_OF_WEEK];  //告警检测持续时间（可读写）
    s8  achAlarmOSDName[TNM_MAX_STRING_LEN + 4];    //告警字幕内容（可读写）

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Sn: %d; EncId: %d; LptId: %d; OSD: %d\n",
            bySerialNo, byRelateEncId, byOutputLptId, byAlarmOSDEnable);
        OspPrintf(TRUE, FALSE, "\tDetState: %d; OsdName: %s;\n",
            byAlarmDetectState, achAlarmOSDName);
        struct tm tmStime;
        u32 dwTime = 0;
        u32 dwDay = 0;
        for (s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
        {
            dwTime = dwAlarmDetectStartTime[nIndex];
            dwDay = dwAlarmDetectDuration[nIndex];
            OspPrintf(TRUE, FALSE, "\tDay%d: ", nIndex+1);
            if (bNetToHost) 
            {
                dwTime = ntohl(dwTime);
                dwDay = ntohl(dwDay);
            }

            if (dwTime & 0x80000000)
            {
                OspPrintf(TRUE, FALSE, "\tDur: 0x%lx; 非法时间值: 0x%lx;\n", dwDay, dwTime);
            }
            else
            {
                tmStime = *localtime((time_t*)&dwTime);
                OspPrintf(TRUE, FALSE, "\tDur: 0x%lx; STime: %s\n", 
                    dwDay, asctime(&tmStime));
            }
        }   
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmLPTCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmLPTCfgInfo::NetToHost()
{
    for(s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
    {
        dwAlarmDetectStartTime[nIndex] = ntohl(dwAlarmDetectStartTime[nIndex]);
        dwAlarmDetectDuration[nIndex] = ntohl(dwAlarmDetectDuration[nIndex]);
    }
}
inline void TTnmLPTCfgInfo::HostToNet()
{
    for(s32 nIndex = 0; nIndex < TNM_DAYS_OF_WEEK; nIndex++)
    {
        dwAlarmDetectStartTime[nIndex] = htonl(dwAlarmDetectStartTime[nIndex]);
        dwAlarmDetectDuration[nIndex] = htonl(dwAlarmDetectDuration[nIndex]);
    }
}


/*====================================================================
1.2.8.  串口参数表（按串口）
====================================================================*/
//串口类型
typedef struct tagTnmSerialTypeSet
{
    u8 byRS232;
    u8 byRS422;
    u8 byRS485;
    u8 byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "RS232: %d; RS422: %d; RS485: %d;\n",
            byRS232, byRS422, byRS485);
    }
}PACKED TTnmSerialTypeSet;

typedef struct tagTnmSerialCfgInfo
{
    TTnmSerialTypeSet tSerialTypeSet;   // 串口类型能力集（只读）
    u32 dwBaudRate;                     // 串口波特率（可读写）
    u8  bySerialNo;                     // 串口号
    u8  byComPortType;                  // 串口类型（可读写）
                                        //  TNM_SERIAL_LINE_DRIVER_NONE
                                        //  TNM_SERIAL_LINE_DRIVER_RS232
                                        //  TNM_SERIAL_LINE_DRIVER_RS422
                                        //  TNM_SERIAL_LINE_DRIVER_RS485
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "SN: %d; Baud: 0x%x; Comtype: %d;\n",
                bySerialNo, ntohl(dwBaudRate), byComPortType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "SN: %d; Baud: 0x%lx; Comtype: %d;\n",
                bySerialNo, dwBaudRate, byComPortType);
        }
        OspPrintf(TRUE, FALSE, "\t");
        tSerialTypeSet.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmSerialCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmSerialCfgInfo::NetToHost()
{
    dwBaudRate = ntohl(dwBaudRate);
}
inline void TTnmSerialCfgInfo::HostToNet()
{
    dwBaudRate = htonl(dwBaudRate);
}
/*====================================================================
1.2.9.  前端存储参数表（按通道）2417/2428才有
====================================================================*/
typedef struct tagTnmStorageCfgInfo
{
    u8  byStorageChanNo;                // 存储通道号
    u8  byAlarmRecEnable;               // 告警录像使能（可读写）TNM_DISABLE/TNM_ENABLE
    u8  bySkipRecEnable;                // 抽帧录像使能（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byTimeRecEnable;                // 定时录像使能（可读写）TNM_DISABLE/TNM_ENABLE
    u32 dwAlarmPreRecTime;              // 告警预录时间（0：表示不预录，可读写）(秒)
    u32 dwRecTimeAfterRecover;          // 告警恢复后持续录像时间（可读写）(秒)
    u16 wSkipRecInterval;               // 抽帧录像关键帧间隔（帧的个数）（可读写）
    u8  byCycleRecEnable;               // 周期录像使能（可读写）TNM_DISABLE/TNM_ENABLE
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "ChNo: %d; RecEn: %d; PreRecT: 0x%x; RecT: 0x%x;\n",
                byStorageChanNo, byAlarmRecEnable, ntohl(dwAlarmPreRecTime), ntohl(dwRecTimeAfterRecover));
            OspPrintf(TRUE, FALSE, "\tSkipEn: %d; SkipInt: 0x%hx; TimeREn: %d; Cycle: %d;\n",
                bySkipRecEnable, ntohs(wSkipRecInterval), byTimeRecEnable, byCycleRecEnable);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "ChNo: %d; RecEn: %d; PreRecT: 0x%lx; RecT: 0x%lx;\n",
                byStorageChanNo, byAlarmRecEnable, dwAlarmPreRecTime, dwRecTimeAfterRecover);
            OspPrintf(TRUE, FALSE, "\tSkipEn: %d; SkipInt: 0x%hx; TimeREn: %d; Cycle: %d;\n",
                bySkipRecEnable, wSkipRecInterval, byTimeRecEnable, byCycleRecEnable);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmStorageCfgInfo;

typedef struct tagPeriodInterval
{
    u32 dwStartTime;        // 开始时间
    u32 dwEndTime;          // 结束时间
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwStime = dwStartTime;
        u32 dwEtime = dwEndTime;
        if (bNetToHost)
        {
            dwStime = ntohl(dwStime);
            dwEtime = ntohl(dwEtime);
        }        
        struct tm *ptm = NULL;
        ptm = localtime((time_t*)&dwStime);
        if (ptm ==NULL)
        {
            OspPrintf(TRUE, FALSE, "\t 1970.01.01.00.00\n");
        }
        OspPrintf(TRUE, FALSE, "\tstart:%s", asctime(ptm));

        ptm = localtime((time_t*)&dwEtime);
        if (ptm ==NULL)
        {
            OspPrintf(TRUE, FALSE, "\t 1970.01.01.00.00\n\n");
        }
        OspPrintf(TRUE, FALSE, "\tend:%s\n", asctime(ptm));
    }
    void NetToHost()
    {
        dwStartTime = ntohl(dwStartTime);
        dwEndTime = ntohl(dwEndTime);
    }
    void HostToNet()
    {
        dwStartTime = htonl(dwStartTime);
        dwEndTime = htonl(dwEndTime);
    }
}PACKED TPeriodInterval;


#define TTNM_PU_RECORD_TIMES_EACH_DAY   3   //每天录象的时段数目

#define PU_RECORD__EACHDAY    0   //每日录象
#define PU_RECORD__PEROID     1   //周期录象
typedef struct TTnmPuPeriodPolicy
{
    TPeriodInterval tPeriod[TNM_DAYS_OF_WEEK][TTNM_PU_RECORD_TIMES_EACH_DAY];//当byPolicy为PU_RECORD__EACHDAY时
                                                                         //tPeriod[0][0],tPeriod[0][1],tPeriod[0][2]有效
                                                                         //表示每日录象的3个时段
                                                                         //当byPolicy为PU_RECORD__PEROID时tPeriod各个元素
                                                                         //表示一周七天每日录象的3个时段
    u8 byChanNo;                                //通道号
    u8 byPolicy;                                //录象策略：周期录象(PU_RECORD__PEROID)/每日录象(PU_RECORD__EACHDAY)
    u8 byReserved;
    u8 byReserved1;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "ChanNo:%d; Policy:%d\n", byChanNo, byPolicy);
        OspPrintf(TRUE, FALSE, " Record Time:");
        s32 nIndex = 0;
        
        for (nIndex = 0; nIndex < TNM_DAYS_OF_WEEK;nIndex++)
        {
            OspPrintf(TRUE, FALSE, "\t%d:\n",nIndex);
            tPeriod[nIndex][0].Print(bNetToHost);
            tPeriod[nIndex][1].Print(bNetToHost);
            tPeriod[nIndex][2].Print(bNetToHost);
        }
    }
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuPeriodPolicy;

inline void TTnmPuPeriodPolicy::NetToHost()
{
    u32 dwIndex = 0;
    for (; dwIndex < TNM_DAYS_OF_WEEK; dwIndex++)
    {
        tPeriod[dwIndex][0].NetToHost();
        tPeriod[dwIndex][1].NetToHost();
        tPeriod[dwIndex][2].NetToHost();
    }
}

inline void TTnmPuPeriodPolicy::HostToNet()
{
    u32 dwIndex = 0;
    for (; dwIndex < TNM_DAYS_OF_WEEK; dwIndex++)
    {
        tPeriod[dwIndex][0].HostToNet();
        tPeriod[dwIndex][1].HostToNet();
        tPeriod[dwIndex][2].HostToNet();
    }
}
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmStorageCfgInfo::NetToHost()
{
    dwAlarmPreRecTime = ntohl(dwAlarmPreRecTime);
    dwRecTimeAfterRecover = ntohl(dwRecTimeAfterRecover);
    wSkipRecInterval = ntohs(wSkipRecInterval);
}
inline void TTnmStorageCfgInfo::HostToNet()
{
    dwAlarmPreRecTime = htonl(dwAlarmPreRecTime);
    dwRecTimeAfterRecover = htonl(dwRecTimeAfterRecover);
    wSkipRecInterval = htons(wSkipRecInterval);
}

/*====================================================================
1.3.    平台设备配置参数
====================================================================*/
/*====================================================================
1.3.1.  CMU配置参数
====================================================================*/
typedef struct tagTnmCmuCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId tDevId;    // CMU的ID（只读）
    u32 dw3asIp;            // 3AS的IP（可读写）
    u32 dwUasIp;            // UAS的IP（可读写）
    u32 dwParentCmuIp;      // 上级CMU的IP（只读）
    u16 wPuNatPort;         // PU穿越NAT的端口号：0表示不接收
    u16 wCuNatPort;         // PU穿越NAT的端口号：0表示不接收
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, 3as=%s, ", tDevId.achID, ::GetIpStr(dw3asIp));
        OspPrintf(TRUE, FALSE, "Uas=%s\n", ::GetIpStr(dwUasIp));
        OspPrintf(TRUE, FALSE, "\tPCmuIp: %s; PuPort: 0x%hx; CuPort: 0x%hx;\n",
            ::GetIpStr(dwParentCmuIp), wPuNatPort, wCuNatPort);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCmuCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dw3asIp = ntohl(dw3asIp);
    dwUasIp = ntohl(dwUasIp);
    dwParentCmuIp = ntohl(dwParentCmuIp);
    wPuNatPort = ntohs(wPuNatPort);
    wCuNatPort = ntohs(wCuNatPort);
}
inline void TTnmCmuCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dw3asIp = htonl(dw3asIp);
    dwUasIp = htonl(dwUasIp);
    dwParentCmuIp = htonl(dwParentCmuIp);
    wPuNatPort = htons(wPuNatPort);
    wCuNatPort = htons(wCuNatPort);
}

typedef struct tagTnmLadenCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId tDevId;    //CMU的ID（只读）
    u32 dwCmuIp;            //管理的CMU地址
    u16 dwCmuPort;          //管理的CMU端口号
    u16 wListenPort;        //侦听PU和CU连接的端口号
    u16 wCheckLinkInterval; //与CMU断链检测的时间间隔(秒)
    u8  byCheckLinkNum;     //与CMU断链检测的次数
    u8  byReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, CmuIp=%s, ", tDevId.achID, ::GetIpStr(dwCmuIp));
        OspPrintf(TRUE, FALSE, "CmuPort=%d ListenPort=%d\n", dwCmuPort, wListenPort);
        OspPrintf(TRUE, FALSE, "\tSynInterval=%d; CheckLinkNum=%d\n", wCheckLinkInterval, byCheckLinkNum);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }

    void NetToHost();
    void HostToNet();
}TTnmLadenCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmLadenCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    dwCmuPort = ntohs(dwCmuPort);
    wListenPort = ntohs(wListenPort);
    wCheckLinkInterval = ntohs(wCheckLinkInterval);
}
inline void TTnmLadenCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwCmuIp = htonl(dwCmuIp);
    dwCmuPort = htons(dwCmuPort);
    wListenPort = htons(wListenPort);
    wCheckLinkInterval = htons(wCheckLinkInterval);
}

/*====================================================================
1.3.3.  键盘配置参数
====================================================================*/
typedef struct tagTnmKeyboardCfgInfo
{
    u8  byIndex; //键盘索引编号（可读写）
    u8  byId;    //键盘设备ID（只读）
    u8  byPos;   //键盘的放置位置（可读写）
    u8  byReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; Pos: %d;\n",
            byId, byId, byPos);
    }
}PACKED TTnmKeyboardCfgInfo;

/*====================================================================
1.3.4.  NRU配置参数
====================================================================*/
typedef struct tagTnmNruCfgInfo : public TTnmPubCfgInfo
{
    TTnmDeviceId    tNruId;     // NRU编号（只读）
    u32 dwCmuIp;                // CMU侦听地址（可读写）
    s8  achRegCode[32];         // NRU注册码（可读写）?? to do
    u16 wCmuPort;               // CMU侦听端口（可读写）
    u16 dwNruListenIp;          // 定义重复，不再使用
    TTnmUserId  tUserId;        // 独享客户编号（可读写）
    u8  byMaxPreRecChanNum;     // 最大预录通道数量（可读写）
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tNruId: %s; UsrId: %s; RegCode: %s;\n",
            tNruId.achID, tUserId.achID, achRegCode);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tCIp: %s; CPort: 0x%hx; MaxChNum: %d;\n",
            ::GetIpStr(dwCmuIp), wCmuPort, byMaxPreRecChanNum);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmNruCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}

/*====================================================================
1.3.5.  多画面复合配置参数
====================================================================*/
typedef struct tagTnmVpuCfgInfo
{
    u16 wSwitchId;       //VPU使用的交换板索引号（可读写）
    u16 wSwitchPort;     //VPU接收码流的起始端口号（可读写）
    u8  byIndex;         //VPU索引编号（可读写）
    u8  byId;            //VPU设备ID（可读写）
    u16 wReserved;
    s8  achAlias[TNM_MAX_STRING_LEN + 4]; //VPU设备别名（可读写）

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; SwId: 0x%hx; SwPort: 0x%hx;\n",
                byIndex, byId, ntohs(wSwitchId), ntohs(wSwitchPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "Index: %d; Id: %d; SwId: 0x%hx; SwPort: 0x%hx;\n",
                byIndex, byId, wSwitchId, wSwitchPort);
        }
        OspPrintf(TRUE,FALSE, "\tAlias: %s;\n", achAlias);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVpuCfgInfo::NetToHost()
{
    wSwitchId = ntohs(wSwitchId);
    wSwitchPort = ntohs(wSwitchPort);
}
inline void TTnmVpuCfgInfo::HostToNet()
{
    wSwitchId = htons(wSwitchId);
    wSwitchPort = htons(wSwitchPort);
}
/*====================================================================
1.3.6.  视频矩阵配置参数
====================================================================*/
typedef struct tagTnmMatrixCfgInfo
{
    u8  byIndex;  //设备索引编号（可读写）
    u8  byId;     //设备ID（只读）
    u16 wReserved;
    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "Index: %d; Id: %d;\n", byIndex, byId);
    }
}PACKED TTnmMatrixCfgInfo;


/*====================================================================
1.4.    多画面复合器(VPU)配置参数
====================================================================*/
/*====================================================================
1.4.1.  系统配置参数
====================================================================*/
typedef struct tagTnmVpuSysCfgInfo : public TTnmPubCfgInfo
{
    tagTnmVpuSysCfgInfo()
    {
        tVpuId.SetNull();
        dwRegCMUIp = 0;
        wRegCMUPort = 0;
        byVpuStyle = 0;
        byEncodeType = 0;
        byMaxQualValue = 0;
        byMinQualValue = 0;
        dwEcdBitRate = 0;
        dwBandWidth = 0;
        wFrameRate = 0;
        byImageQuality = 0;
        wVideoWidth = 0;
        wVideoHeight = 0;
        byVideoOutput = 0;
        memset(achAlias, 0, sizeof(achAlias));
        
    }
    TTnmDeviceId  tVpuId;                       //设备全网唯一ID（可读写）
    u32 dwRegCMUIp;                             //注册的平台IP地址（可读写）
    u16 wRegCMUPort;                            //注册的平台端口号（可读写）
    u8  byVpuStyle;                             //多画面风格（可读写）TNM_VPU_STYLE_HTWO
    u8  byEncodeType;                           //VPU 编码类型
                                                //TNM_VIDEO_TYPE_MPEG4/TNM_VIDEO_TYPE_H261/TNM_VIDEO_TYPE_H263
    u8  byMaxQualValue;                         //最大量化参数(1-31)
    u8  byMinQualValue;                         //最小量化参数(1-31)
    u16 wIFramInterval;             //关键桢间隔
    u32 dwEcdBitRate;                           //编码比特率(kbps)
    u32 dwBandWidth;                            //网络发送带宽(kbps)
    u16 wFrameRate;                             //桢率
    u8  byImageQuality;                         //图象压缩质量(1-5)
    u16 wVideoWidth;                            //图象宽度
    u16 wVideoHeight;                           //图象高度
    s8  achAlias[TNM_MAX_ALIAS_NAME_LEN + 4];       //设备别名（可读写）
    u8  byVideoOutput;                          //是否背板输出 0不输出 /1输出
    u8  byReserved1;
    u8  byReserved2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tStyle: %d; RegIp: %s; RegPort: %d; EcdType: %d;\n",
                byVpuStyle, ::GetIpStr(ntohl(dwRegCMUIp)), ntohs(wRegCMUPort), byEncodeType);
            OspPrintf(TRUE, FALSE, "\tMaxQual: %d; MinQual: %d; EcdRate: %d; BandWidth: %d;\n", 
                byMaxQualValue, byMinQualValue, ntohl(dwEcdBitRate), ntohl(dwBandWidth));
            OspPrintf(TRUE, FALSE, "\tFrameRate:%d; ImageQual:%d; Vwidth: %d; Vheight:%d;\n",
                ntohs(wFrameRate), byImageQuality, ntohs(wVideoWidth), ntohs(wVideoHeight));

        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tStyle: %d; RegIp: %s; RegPort: %hu; EcdType: %d;\n",
                byVpuStyle, ::GetIpStr(dwRegCMUIp), wRegCMUPort, byEncodeType);
            OspPrintf(TRUE, FALSE, "\tMaxQual: %d; MinQual: %d; EcdRate: %lu; BandWidth: %lu;\n", 
                byMaxQualValue, byMinQualValue, dwEcdBitRate, dwBandWidth);
            OspPrintf(TRUE, FALSE, "\tFrameRate:%d; ImageQual:%d; Vwidth: %hu; Vheight:%hu;\n",
                wFrameRate, byImageQuality, wVideoWidth, wVideoHeight);
        }
        OspPrintf(TRUE, FALSE, "\tUnitNo: %s; Alias: %s; Videoout: %d\n", tVpuId.achID, achAlias, byVideoOutput);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuSysCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVpuSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIp = ntohl(dwRegCMUIp);
    wRegCMUPort = ntohs(wRegCMUPort);
    dwEcdBitRate = ntohl(dwEcdBitRate);
    dwBandWidth = ntohl(dwBandWidth);
    wFrameRate = ntohs(wFrameRate);
    wVideoWidth = ntohs(wVideoWidth);
    wVideoHeight = ntohs(wVideoHeight);
    wIFramInterval = ntohs(wIFramInterval);
}
inline void TTnmVpuSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwRegCMUIp = htonl(dwRegCMUIp);
    wRegCMUPort = htons(wRegCMUPort);
    dwEcdBitRate = htonl(dwEcdBitRate);
    dwBandWidth = htonl(dwBandWidth);
    wFrameRate = htons(wFrameRate);
    wVideoWidth = htons(wVideoWidth);
    wVideoHeight = htons(wVideoHeight);
    wIFramInterval = htons(wIFramInterval);
}
/*====================================================================
1.5.    视频矩阵（VAS）板配置参数
====================================================================*/
/*====================================================================
1.5.1.  系统配置参数
====================================================================*/
typedef struct tagTnmVasSysCfgInfo : public TTnmPubCfgInfo
{
    u32 dwRegCMUIp;                     //注册的平台IP地址（可读写）
    u16 wRegCMUPort;                    //注册的平台端口号（可读写）
    u16 wRegCMUNo;                      //注册的平台唯一ID（可读写）
    u8  byUnitNo;                       //设备全网唯一ID（可读写）
    u8  byMaxInputPort;                 //最大输入端口（可读写）
    u8  byMaxOutputPort;                //最大输出端口（可读写）
    u8  byReserved;
    s8  achAlias[TNM_MAX_STRING_LEN + 4]; //设备别名（可读写）

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tRegIp: %s; RegPort: 0x%hx; RegNo: 0x%hx; UniNo: %d;\n",
                ::GetIpStr(ntohl(dwRegCMUIp)), ntohs(wRegCMUPort), ntohs(wRegCMUNo), byUnitNo);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tRegIp: %s; RegPort: 0x%hx; RegNo: 0x%hx; UniNo: %d;\n",
                ::GetIpStr(dwRegCMUIp), wRegCMUPort, wRegCMUNo, byUnitNo);
        }
        OspPrintf(TRUE, FALSE, "\tMaxInPort: %d; OutPort: %d; Alias: %s;\n",
            byMaxInputPort, byMaxOutputPort, achAlias);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVasSysCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVasSysCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwRegCMUIp = ntohl(dwRegCMUIp);
    wRegCMUPort = ntohs(wRegCMUPort);
    wRegCMUNo = ntohs(wRegCMUNo);
}
inline void TTnmVasSysCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    dwRegCMUIp = htonl(dwRegCMUIp);
    wRegCMUPort = htons(wRegCMUPort);
    wRegCMUNo = htons(wRegCMUNo);
}

//1.6.  CUI配置参数
typedef struct tagTnmCuiCfgInfo : public TTnmPubCfgInfo
{
    u16 wListenPort;                            // 侦听端口
    u16 wMaxCuLinkNum;                          // 客户端最大连接数
    TTnmDeviceId tDevId;                        // CUI模块的ID
    TTnmDeviceId tCmuId;                        // 连接的CMU的ID
    u32 dwCmuIp;                                // 连接的CMU的IP
    u16 wCmuPort;                               // 连接的CMU的侦听端口
    u16 wReserved;
    s8  achCuiPwd[TNM_MAX_PASSWORD_LEN + 4];    // CUI向CMU注册的密码

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; LkNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                ntohs(wListenPort), ntohs(wMaxCuLinkNum), ::GetIpStr(ntohl(dwCmuIp)), ntohs(wCmuPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; LkNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                wListenPort, wMaxCuLinkNum, ::GetIpStr(dwCmuIp), wCmuPort);
        }
        OspPrintf(TRUE, FALSE, "\tDevId: %s; CmuId: %s; CuiPwd: %s;\n",
            tDevId.achID, tCmuId.achID, achCuiPwd);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCuiCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wListenPort = ntohs(wListenPort);
    wMaxCuLinkNum = ntohs(wMaxCuLinkNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmCuiCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wListenPort = htons(wListenPort);
    wMaxCuLinkNum = htons(wMaxCuLinkNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}

//1.7.  PUI配置参数
typedef struct tagTnmPuiCfgInfo : public TTnmPubCfgInfo
{
    u16 wListenPort;                         // 本地侦听端口
    u16 wMaxPuNum;                           // 最大PU接入数量
    u32 dwCmuIp;                             // 连接的CMU的IP地址
    u16 wCmuPort;                            // 连接CMU的端口号
    u16 wReserved;
    TTnmDeviceId tCmuId;                     // 连接的CMU的设备ID
    TTnmDeviceId tDevId;                     // PUI模块的ID
    s8  achPuiPwd[TNM_MAX_PASSWORD_LEN + 4]; // PUI向CMU注册的密码
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; PuNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                ntohs(wListenPort), ntohs(wMaxPuNum), ::GetIpStr(ntohl(dwCmuIp)), ntohs(wCmuPort));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tLnPort: 0x%hx; PuNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
                wListenPort, wMaxPuNum, ::GetIpStr(dwCmuIp), wCmuPort);
        }
        OspPrintf(TRUE, FALSE, "\tCmuId: %s; DevId: %s; PuiPwd: %s;\n",
            tCmuId.achID, tDevId.achID, achPuiPwd);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuiCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wListenPort = ntohs(wListenPort);
    wMaxPuNum = ntohs(wMaxPuNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmPuiCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wListenPort = htons(wListenPort);
    wMaxPuNum = htons(wMaxPuNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
}
// 1.8 VTDU配置参数
typedef struct tagTnmVtduCfgInfo : public TTnmPubCfgInfo
{
    u32 dwCmuIp;                                // 连接的CMU的IP地址
    u16 wMaxSwitchNum;                          // 最大交换码流数量
    u16 wCmuPort;                               // 连接CMU的端口号
    TTnmDeviceId tCmuId;                        // 连接的CMU的设备ID
    TTnmDeviceId tDevId;                        // VTDU的设备ID
    s8  achPwd[TNM_MAX_PASSWORD_LEN + 4];       // VTDU向CMU注册的密码
    u8  bySwitchNumAlarmRate;                   // 转发路数告警的百分比（1--99）
    u8  byReserved;
    u16 wReserved;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tSwNum: 0x%hx; CIp: %s; CPort: 0x%hx;\n",
            wMaxSwitchNum, ::GetIpStr(dwCmuIp), wCmuPort);

        OspPrintf(TRUE, FALSE, "\tCmuId=%s\n\tDevId=%s, Pwd=%s\n",
            tCmuId.achID, tDevId.achID, achPwd);
        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVtduCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    wMaxSwitchNum = ntohs(wMaxSwitchNum);
    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmVtduCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();

    wMaxSwitchNum = htons(wMaxSwitchNum);
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
} 
// 1.9 UAS配置参数
typedef struct tagTnmUasCfgInfo : public TTnmPubCfgInfo
{
    u32 dwCmuIp;                                // 连接的CMU的IP地址
    u16 wCmuPort;                               // 连接CMU的端口号
    u16 wReserved;
    TTnmDeviceId tCmuId;                        // 连接的CMU的设备ID
    TTnmDeviceId tDevId;                        // UAS的设备ID
    s8  achPwd[TNM_MAX_PASSWORD_LEN + 4];       // UAS向CMU注册的密码

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            NetToHost();
        }

        OspPrintf(TRUE, FALSE, "\tCmuIp=%s, CmuPort=0x%hx\n\tCmuId=%s\n",
            ::GetIpStr(dwCmuIp), wCmuPort, tCmuId.achID);
        
        OspPrintf(TRUE, FALSE, "\tDevId=%s, Pwd=%s\n",
            tDevId.achID, achPwd);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmUasCfgInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmUasCfgInfo::NetToHost()
{
    TTnmPubCfgInfo::NetToHost();

    dwCmuIp = ntohl(dwCmuIp);
    wCmuPort = ntohs(wCmuPort);
}
inline void TTnmUasCfgInfo::HostToNet()
{
    TTnmPubCfgInfo::HostToNet();
    
    dwCmuIp = htonl(dwCmuIp);
    wCmuPort = htons(wCmuPort);
} 


// 1.10 3AS配置参数
typedef struct tagTnm3asCfgInfo : public TTnmPubCfgInfo
{
    tagTnm3asCfgInfo()
    {
        memset(achAlias, 0, sizeof(achAlias));
    }
    s8  achAlias[TNM_MAX_ALIAS_NAME_LEN + 4];       // UAS向CMU注册的密码

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPubCfgInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tAlias:%s\n", achAlias);

        }
    }
    void TransByteOrder(BOOL32 bToHost = TRUE);
}PACKED TTnm3asCfgInfo;

/*========================================================================
函 数 名：TransByteOrder
功    能：转换字节序接口
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/13  3.9     李洪强  创建
========================================================================*/
inline void TTnm3asCfgInfo::TransByteOrder(BOOL32 bToHost)
{
    if (bToHost) 
    {
        TTnmPubCfgInfo::NetToHost();
    }
    else
    {
        TTnmPubCfgInfo::HostToNet();
    }
}


/********************************************************************/
/* 2. 性能管理：系统状态和性能的实时查询                            */
/********************************************************************/
/*====================================================================
2.1. 公共性能参数
====================================================================*/
// 基本的性能参数
typedef struct tagTnmBasePfmInfo
{
    u32 dwDeviceId;         // 该值为NMS为设备分配置的ID，前端设备不需要填写该项
    u32 dwSysTime;          // 系统时间
    
    u8  byCpuUseRate;       // CPU使用率(0-100)
    u8  byMemUseRate;       // 内存使用率(0-100)
    u8  byMemStatus;        // 内存使用状态(enum TnmStatusType)
    u8  bySysLoad;          // 系统负荷(0-100)
    s8  achHardwareVer[TNM_MAX_VERSION_LEN + 4];        // 设备版本号
    s8  achSoftwareVer[TNM_MAX_VERSION_LEN + 4];        // 设备软件版本号
    s8  achCompileTime[TNM_MAX_COMPILE_TIME_LEN + 4];   // 软件编译时间
    /////u8  byTemperature;     // 设备当前的温度（0--100）
    /////u8  byReserved;
    /////u16 wReserved;

    void Init()
    {
        dwDeviceId  = 0;
        byCpuUseRate = 0;
        byMemUseRate = 0;
        byMemStatus = TNM_STATUS_NONE;
        dwSysTime   = 0;
        bySysLoad   = 0;
        memset(achHardwareVer, 0, sizeof(achHardwareVer));
        memset(achSoftwareVer, 0, sizeof(achSoftwareVer));
        memset(achCompileTime, 0, sizeof(achCompileTime));
        /////byTemperature = 0;
    }
    const tagTnmBasePfmInfo* GetBasePfmInfoPtr() const
    {
        return this;
    }

    void AdjustSysTime(BOOL32 bDecTime = TRUE);

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            NetToHost();
        }
        OspPrintf(TRUE, FALSE, "\tDevId: 0x%lx; CpuRate: %d; SysTime: %s;\n", 
            dwDeviceId, byCpuUseRate, ::GetTimeStr(dwSysTime));
            
        OspPrintf(TRUE, FALSE, "\tMemRate: %d; MemStatus: %d; CompileT: %s\n", 
            byMemUseRate, byMemStatus, achCompileTime); 
        OspPrintf(TRUE, FALSE, "\tSysLoad: %d; SoftV: %s; HardV: %s;\n",
            bySysLoad, achSoftwareVer, achHardwareVer);

        if (bNetToHost) 
        {
            HostToNet();
        }
    }
    void NetToHost();
    void HostToNet();
}TTnmBasePfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmBasePfmInfo::NetToHost()
{
    dwSysTime = ntohl(dwSysTime);
    dwDeviceId = ntohl(dwDeviceId);
}
inline void TTnmBasePfmInfo::HostToNet()
{
    dwSysTime = htonl(dwSysTime);
    dwDeviceId = htonl(dwDeviceId);
} 


/*========================================================================
函 数 名：AdjustSysTime
功    能：由于不同操作系统间时间基准不一样，因此需要对系统时间进行调整
参    数：BOOL32 bDecTime = TRUE        [in]是否要将时间值减小
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/06/01  3.9     李洪强  创建
========================================================================*/
inline void TTnmBasePfmInfo::AdjustSysTime(BOOL32 bDecTime)
{
    if (bDecTime) 
    {
        dwSysTime -= 3600 * 8;
    }
    else
    {
        dwSysTime += 3600 * 8;
    }
}


typedef struct tagTnmNmsPfmInfo : public  TTnmBasePfmInfo
{
}TTnmNmsPfmInfo;

// 硬件设备的性能参数
typedef struct tagTnmPfmHwInfo : public TTnmBasePfmInfo
{
    u8  abyPanelLed[TNM_MAX_PANELLED_LEN];      // 设备面板灯的状态(一个u8代表一盏灯) enum TnmLedStatus
    u8  abyEnternetStatus[TNM_MAX_ETHERNET_NUM + 3];// 以太网口状态(一个u8代表一个网口) enum TnmEnternetStatus
    u8  abyMapStatus[TNM_MAX_MAP_NUM + 3];          // MAP状态(一个u8代表一块MAP)enum TnmStatusType
    u8  byWatchDogEnalbe;                       // 设备的硬件看门狗使能状态(TNM_DISABLE/TNM_ENABLE)
    u8  byReserved;
    u16 wReserved;

    /////u8  abyEthLostPkRate[TNM_MAX_ETHERNET_NUM + 3];// 以太网口丢包率（0--100）
    /////u32 abyEthBps[TNM_MAX_ETHERNET_NUM + 3];       // 以太网口流量（单位bps）

    void Init()
    {
        TTnmBasePfmInfo::Init();
        byWatchDogEnalbe    = TNM_DISABLE;
        s32 nIndex = 0;
		for(nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
		{
			abyPanelLed[nIndex] = TNM_LED_NONE;
		}
		for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
		{
			abyEnternetStatus[nIndex] = TNM_ETHERNET_NONE;
			/////abyEthLostPkRate[nIndex] = 0;
			//////abyEthBps[nIndex] = 0;
		}
		for(nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
		{
			abyMapStatus[nIndex] = TNM_STATUS_NONE;
		}
	}

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);

        s32 nIndex;

        OspPrintf(TRUE, FALSE, "\tWDog: %d; MapStatus: ", byWatchDogEnalbe);
        for (nIndex = 0; nIndex < TNM_MAX_MAP_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyMapStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLed: ");
        for (nIndex = 0; nIndex < TNM_MAX_PANELLED_LEN; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyPanelLed[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
		OspPrintf(TRUE, FALSE, "\tEthNet: ");
        for (nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyEnternetStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPfmHwInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPfmHwInfo::NetToHost()
{
    /////s32 nIndex = 0;

    TTnmBasePfmInfo::NetToHost();

    /////for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    /////{
        /////abyEthBps[nIndex] = ntohl(abyEthBps[nIndex]);
    /////}
}
inline void TTnmPfmHwInfo::HostToNet()
{
    /////s32 nIndex = 0;

    TTnmBasePfmInfo::HostToNet();

    /////for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    /////{
    /////    abyEthBps[nIndex] = htonl(abyEthBps[nIndex]);
    /////}
} 

typedef struct tagTnmLogonInfo
{
    tagTnmLogonInfo()
    {
        memset(this, 0, sizeof(tagTnmLogonInfo));
    }
    s8  achUserName[TNM_MAX_USER_NAME_LEN + 4];     // 登录用户名
    u32 dwCuIP;                                 // 登录IP地址
    u32 dwLoginTime;                            // 登录时间
    void Print(BOOL32 bNetToHost = FALSE)
    {       
        u32 dwIp = dwCuIP;
        u32 dwTime = dwLoginTime;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
            dwIp = ntohl(dwIp);
        }
        OspPrintf(TRUE, FALSE, "\tUsrname=%s, CuIp=%s, Time=%s\n", 
            achUserName, ::GetIpStr(dwIp), ::GetTimeStr(dwTime));
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmLogonInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmLogonInfo::NetToHost()
{
    dwCuIP = ntohl(dwCuIP);
    dwLoginTime = ntohl(dwLoginTime);
}
inline void TTnmLogonInfo::HostToNet()
{
    dwCuIP = htonl(dwCuIP);
    dwLoginTime = htonl(dwLoginTime);
}

/*====================================================================
2.2. 单板性能参数
====================================================================*/

typedef struct tagTnmBoardPfmInfo : public TTnmPfmHwInfo
{
    u8  byLayer;            //设备的层号
    u8  bySlot;             //设备的槽号

    u8  byReserved1;
    u8  byReserved2;

    void Init()
    {
        TTnmPfmHwInfo::Init();
        
        byLayer = 0;
        bySlot = 0;
    }
    /*========================================================================
    函 数 名：GetBoardPfmInfo
    功    能：得到单板的性能参数指针
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/21  3.9     李洪强  创建
    ========================================================================*/
    const tagTnmBoardPfmInfo * GetBoardPfmInfo() const;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tLayer: %d; Slot: %d;\n", byLayer, bySlot);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmBoardPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmBoardPfmInfo::NetToHost()
{
    TTnmPfmHwInfo::NetToHost();
}
inline void TTnmBoardPfmInfo::HostToNet()
{
    TTnmPfmHwInfo::HostToNet();
} 
/*========================================================================
函 数 名：GetBoardPfmInfo
功    能：得到单板的性能参数指针
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/
inline const TTnmBoardPfmInfo * TTnmBoardPfmInfo::GetBoardPfmInfo() const
{
    return this;
}


/*====================================================================
2.3. 前端设备性能参数
====================================================================*/
#define INVALID_DISK    (u8)0xFF
typedef struct tagTnmPuPfmInfo : public TTnmPfmHwInfo
{
    u8  abyRcvLosePacketRate[TNM_MAX_CHAN_NUM];     //接收丢包率（指最近1分钟之内的平均丢包率）查询
                                                    //一个u8代表一个通道，100%
                                                    //TNM_INVALID_CHANNEL_ID	无效值
    u8  abyCodecState[TNM_MAX_CHAN_NUM];            //编解码码流收发状态查询(一个u8代表一个通道)
                                                    //TNM_INVALID_CHANNEL_ID	无效值
                                                    //TNM_CODEC_STOP
                                                    //TNM_ENCODING
                                                    //TNM_DECODING
    u8  abyLPTInStatus[TNM_MAX_LPTCHAN_NUM];        //并口输入上报(一个u8代表一个通道)（只读）enum TnmStatusType
    u8  abyHardDiskUseRate[TNM_MAX_PU_DISK_NUM];    //有磁盘的前端各个磁盘的使用率(值小于100有效,值为INVALID_DISK说明该磁盘不存在)

    u8  abyLPTOutStatus[TNM_MAX_LPTCHAN_NUM];       //并口输出上报(一个u8代表一个通道)（只读）enum TnmStatusType
    u32 abyCodecRate[TNM_MAX_CHAN_NUM];             //通道编码码率(单位bps)
    u8  abyVideoSrcStatus[TNM_MAX_VIDEOSRC_NUM];    //视频源状态(一个u8代表一个视频源)enum TnmStatusType
    u8  abyVideoRecordState[TNM_MAX_CHAN_NUM];      //每个通道是否在录像(一个u8代表一个通道) enum TnmRpState
    u8  byVideoPlayState;                           //设备是否在放像 enum TnmRpState
    u8  byStreamNum;                                //当前视频流路数
    u8  byLostPacketRate;                           //业务层丢包率
    u8  byReserved;

    u8  abyEthLostPkRate[TNM_MAX_ETHERNET_NUM + 3]; //以太网口丢包率（0--100）
    u32 abyEthBps[TNM_MAX_ETHERNET_NUM + 3];        //以太网口流量（单位bps）

    u8  byLayer;                                    //设备的层号
    u8  bySlot;                                     //设备的槽号

    u8  abyReserved[30];
    
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        TTnmPfmHwInfo::Init();
        
        s32 nIndex = 0;
        for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            abyRcvLosePacketRate[nIndex] = 0;
            abyCodecState[nIndex] = TNM_INVALID_CHANNEL_ID;
            abyCodecRate[nIndex] = 0;
        }
        for(nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            abyLPTInStatus[nIndex] = TNM_STATUS_NONE;
            abyLPTOutStatus[nIndex] = TNM_STATUS_NONE;
        }
        for (nIndex = 0; nIndex < TNM_MAX_PU_DISK_NUM; nIndex++)
        {
            abyHardDiskUseRate[nIndex] = INVALID_DISK;
        }

        for (nIndex = 0; nIndex < TNM_MAX_VIDEOSRC_NUM; nIndex++)
        {
            abyVideoSrcStatus[nIndex] = TNM_STATUS_NONE;
        }
        
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            abyVideoRecordState[nIndex] = RP_STATE_NULL;
        }

        for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
        {
            abyEthBps[nIndex] = 0;
        }

        byVideoPlayState = RP_STATE_STOP;
        byStreamNum = 0;
        byLostPacketRate = 0;
        
        byLayer = 0;                                    //设备的层号
    	bySlot = 0;                                     //设备的槽号
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);

        s32 nIndex;
        OspPrintf(TRUE, FALSE, "\tLosRate: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyRcvLosePacketRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tCodeState: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyCodecState[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLPTStatus: ");
        for (nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyLPTInStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "DiskUseRate:");
        for (nIndex = 0; nIndex < TNM_MAX_PU_DISK_NUM; nIndex++)
        {
            if (abyHardDiskUseRate[nIndex] == INVALID_DISK)
            {
                break;
            }
            OspPrintf(TRUE, FALSE, "%d-", abyHardDiskUseRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuPfmInfo::NetToHost()
{
    s32 nIndex = 0;

    TTnmPfmHwInfo::NetToHost();

    for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
    {
        abyCodecRate[nIndex] = ntohl(abyCodecRate[nIndex]);
    }
    
    for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        abyEthBps[nIndex] = ntohl(abyEthBps[nIndex]);
    }
}
inline void TTnmPuPfmInfo::HostToNet()
{
    s32 nIndex = 0;

    TTnmPfmHwInfo::HostToNet();

    for(nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
    {
        abyCodecRate[nIndex] = htonl(abyCodecRate[nIndex]);
    }
    
    for(nIndex = 0; nIndex < TNM_MAX_ETHERNET_NUM; nIndex++)
    {
        abyEthBps[nIndex] = htonl(abyEthBps[nIndex]);
    }
}

/*====================================================================
2.x. Dec5性能参数
====================================================================*/
//基本性能参数
typedef struct tagTnmDec5PfmInfo : public TTnmBoardPfmInfo
{
    u8  abyRcvLosePacketRate[TNM_MAX_CHAN_NUM]; // 接收丢包率（指最近1分钟之内的平均丢包率）查询
                                                //  一个u8代表一个通道，100%
                                                //  TNM_INVALID_CHANNEL_ID  无效值
    u8  abyCodecState[TNM_MAX_CHAN_NUM];        // 编解码码流收发状态查询(一个u8代表一个通道)
                                                //  TNM_INVALID_CHANNEL_ID  无效值
                                                //  TNM_CODEC_STOP
                                                //  TNM_ENCODING
                                                //  TNM_DECODING
    u8  abyLPTStatus[TNM_MAX_LPTCHAN_NUM];      //并口告警上报(一个u8代表一个通道)（只读）enum TnmStatusType
    
    void Init()
    {
        TTnmBoardPfmInfo::Init();
        memset(abyRcvLosePacketRate, 0, sizeof(abyRcvLosePacketRate));
        memset(abyCodecState, 0, sizeof(abyCodecState));
        memset(abyLPTStatus, 0, sizeof(abyLPTStatus));
    }

    void Print(BOOL32 bNetToHost = FALSE )
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        
        s32 nIndex;
        OspPrintf(TRUE, FALSE, "\tLosRate: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyRcvLosePacketRate[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tCodeState: ");
        for (nIndex = 0; nIndex < TNM_MAX_CHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyCodecState[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
        OspPrintf(TRUE, FALSE, "\tLPTStatus: ");
        for (nIndex = 0; nIndex < TNM_MAX_LPTCHAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyLPTStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }

    void NetToHost()
    {
        TTnmBoardPfmInfo::NetToHost();
    }
    void HostToNet()
    {
        TTnmBoardPfmInfo::HostToNet();
    }

}PACKED TTnmDec5PfmInfo;

//////////////////////////////////////////////////////////////////////////
//  平台设备性能扩展参数
typedef struct tagTnmCmuExtendablePfmInfo : public TTnmBoardPfmInfo
{
    u32 dwRecTaskNum;           // 录像任务路数
    u32 dwRecBitRate;           //录像总码率
    u32 dwRecVodNum;            //录像点播路数
    u32 dwRecVodTotalBitRate;   // 录象点播总码率       
    u32 dwSwitchNum;            // 转发码流路数
    u32 dwTotalBitRate;         // 转发总码率
    u32 dwPreRecChanNum;        //预录路数

    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuExtendablePfmInfo;

inline void TTnmCmuExtendablePfmInfo::NetToHost()
{
    dwRecTaskNum = ntohl(dwRecTaskNum);           
    dwRecBitRate = ntohl(dwRecBitRate);           
    dwRecVodNum = ntohl(dwRecVodNum);            
    dwRecVodTotalBitRate = ntohl(dwRecVodTotalBitRate);       
    dwSwitchNum= ntohl(dwSwitchNum);            
    dwTotalBitRate = ntohl(dwTotalBitRate);         
    dwPreRecChanNum = ntohl(dwPreRecChanNum);        
}

inline void TTnmCmuExtendablePfmInfo::HostToNet()
{
    dwRecTaskNum = htonl(dwRecTaskNum);           
    dwRecBitRate = htonl(dwRecBitRate);           
    dwRecVodNum = htonl(dwRecVodNum);            
    dwRecVodTotalBitRate = htonl(dwRecVodTotalBitRate);       
    dwSwitchNum= htonl(dwSwitchNum);            
    dwTotalBitRate = htonl(dwTotalBitRate);         
    dwPreRecChanNum = htonl(dwPreRecChanNum);
}

//获取cmu的磁盘录像点数
typedef struct tagTnmCmuDiskPfmInfo : public TTnmBoardPfmInfo
{
    //挂载分区名
    u8 achMountPath[MAX_MOUNTPATH_LEN];
    
    //录像个数
    u32 dwRecNum;
    
    //区域号
    u32 dwAreaID;

    void NetToHost();
    void HostToNet();

}PACKED TTnmCmuDiskPfmInfo;

inline void TTnmCmuDiskPfmInfo::NetToHost()
{
   dwRecNum = ntohl(dwRecNum);
   dwAreaID = ntohl(dwAreaID);
}

inline void TTnmCmuDiskPfmInfo::HostToNet()
{
    dwRecNum = htonl(dwRecNum);
    dwAreaID = htonl(dwAreaID);
}

// 2.4. 平台设备性能参数
// 2.4.1 CMU基本性能参数
typedef struct tagTnmCmuPfmInfo : public TTnmBoardPfmInfo
{
    u16 wPuDevNum;          // 前端设备接入数
    u16 wCuNum;             // CU接入数
    u16 wVtduNum;           // VTDU连接数
    u16 wNruNum;            // NRU连接数
    u16 wBoardNum;          // 单板数量
    u16 wReserved;
    u8  abyFanStatus[TNM_MAX_FAN_NUM + 3];      // 风扇状态(一个u8代表一个风扇)enum TnmFanStatus
    u8  abyPowerStatus[TNM_MAX_POWER_NUM + 3];  // 电源状态(一个u8代表一个电源)enum TnmPowerStatus

    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        TTnmBoardPfmInfo::Init();
        wPuDevNum = 0;
        wCuNum = 0;
        wVtduNum = 0;
        wNruNum = 0;
        wBoardNum = 0;
        s32 nIndex = 0;
        for(nIndex = 0; nIndex < TNM_MAX_FAN_NUM; nIndex++)
        {
            abyFanStatus[nIndex] = TNM_FAN_NONE;
        }
        for(nIndex = 0; nIndex < TNM_MAX_POWER_NUM; nIndex++)
        {
            abyPowerStatus[nIndex] = TNM_POWER_NONE; 
        }
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);

        s32 nIndex;
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tPuNum: 0x%hx; CuNum: 0x%hx; NruNum: 0x%hx; \n",
                ntohs(wPuDevNum), ntohs(wCuNum), ntohs(wNruNum));
            OspPrintf(TRUE, FALSE, "\tVtduNum: 0x%hx; BoardNum: 0x%hx;\n",
                ntohs(wVtduNum), ntohs(wBoardNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tPuNum: 0x%hx; CuNum: 0x%hx; NruNum: 0x%hx; \n", wPuDevNum, wCuNum, wNruNum);
            OspPrintf(TRUE, FALSE, "\tVtduNum: 0x%hx; BoardNum: 0x%hx;\n", wVtduNum, wBoardNum);
        }
        OspPrintf(TRUE, FALSE, "\tFan: ");
        for (nIndex = 0; nIndex < TNM_MAX_FAN_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyFanStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "; Power: ");
        for (nIndex = 0; nIndex < TNM_MAX_POWER_NUM; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyPowerStatus[nIndex]);
        }
        OspPrintf(TRUE, FALSE, ";\n");
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCmuPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wPuDevNum = ntohs(wPuDevNum);
    wCuNum = ntohs(wCuNum);
    wVtduNum = ntohs(wVtduNum);
    wNruNum = ntohs(wNruNum);
    wBoardNum = ntohs(wBoardNum);
}
inline void TTnmCmuPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wPuDevNum = htons(wPuDevNum);
    wCuNum = htons(wCuNum);
    wVtduNum = htons(wVtduNum);
    wNruNum = htons(wNruNum);
    wBoardNum = htons(wBoardNum);
} 

//2.4.2 CMU扩展性能参数-CU登录情况

typedef struct tagTnmCuLogon 
{
    tagTnmCuLogon()
    {
        dwSessionId = 0;
    }
    u32 dwSessionId;        // 客户的用户Session
    TTnmLogonInfo   tCuLogonInfo;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSSId = dwSessionId;
        if (bNetToHost) 
        {
            dwSSId = ntohl(dwSSId);
        }
        OspPrintf(TRUE, FALSE, "\tSession=0x%lx\n", dwSSId);
        
        tCuLogonInfo.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuLogon;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCuLogon::NetToHost()
{
    tCuLogonInfo.NetToHost();
    dwSessionId = ntohl(dwSessionId);
}
inline void TTnmCuLogon::HostToNet()
{
    tCuLogonInfo.HostToNet();
    dwSessionId = htonl(dwSessionId);
} 

typedef struct tagTnmCuLogItem
{
    tagTnmCuLogItem()
    {
        dwSessionId = 0;
        memset(achRecentLog, 0, sizeof(achRecentLog));
    }
    u32 dwSessionId;        // 客户的用户Session
    s8  achRecentLog[TNM_MAX_CU_LOG_NUM][TNM_MAX_CU_LOG_LEN + 1];   // CU的最近的日志记录数

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "Session=0x%x\n", ntohl(dwSessionId));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "Session=0x%lx\n", dwSessionId);
        }
        s32 nIndex;
        for(nIndex = 0; nIndex < TNM_MAX_CU_LOG_NUM; nIndex++)
        {
            s8 *pchLog = achRecentLog[nIndex];
            if (strlen(pchLog) > 0) 
            {
                OspPrintf(TRUE, FALSE, "%s\n", pchLog);
            }
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuLogItem;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCuLogItem::NetToHost()
{
    dwSessionId = ntohl(dwSessionId);
}
inline void TTnmCuLogItem::HostToNet()
{
    dwSessionId = htonl(dwSessionId);
} 
//typedef struct tagTnmCmuCuInfo
//{
//  tagTnmCmuCuInfo()
//  {
//      dwSessionId = 0;
//  }
//    
//  u64 dwSessionId;        // 用户Session
//    TTnmLogonInfo tCuLogonInfo;
//    TTnmCuLogItem tCuLog; // CU的最近的日志记录数
//}PACKED TTnmCmuCuInfo;

//2.4.3 CMU扩展性能参数-PU连接情况
typedef struct tagTnmCmuPuInfo
{
    tagTnmCmuPuInfo()
    {
        dwDevIp = 0;
        byState = TNM_PU_STATE_ONLINE;
        tDevId.SetNull();
    }
    TTnmDeviceId tDevId;    // 前端设备ID
    u32 dwDevIp;            // 前端设备IP
    s8  byState;            // 设备状态
                            //  TNM_PU_STATE_ONLINE
                            //  TNM_PU_STATE_MUTE
                            //  TNM_PU_STATE_SEND_AUDIO
                            //  TNM_PU_STATE_PLAT_REC
                            //  TNM_PU_STATE_PU_REC
                            //  TNM_PU_STATE_ALARM
    u8  byReserved1;        // 2006/04/05 李洪强 四字节对齐
    u8  byReserved2;
    u8  byReserved3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; DevIp: %s; State: %d;\n", 
                tDevId.achID, ::GetIpStr(ntohl(dwDevIp)), byState);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; DevIp: %s; State: %d;\n", 
                tDevId.achID, ::GetIpStr(dwDevIp), byState);
        }
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCmuPuInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCmuPuInfo::NetToHost()
{
    dwDevIp = ntohl(dwDevIp);
}
inline void TTnmCmuPuInfo::HostToNet()
{
    dwDevIp = htonl(dwDevIp);
}


typedef struct tagTnmLaDenPfmInfo : public TTnmBasePfmInfo
{
    u16 wPuiNum;        //PUI数
    u16 wCuiNum;        //CUI数

    void Init()
    {
        TTnmBasePfmInfo::Init();
        wPuiNum = 0;
        wCuiNum = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);

        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tPuiNum: 0x%hx; wCuiNum: %d;\n", ntohs(wPuiNum), ntohs(wCuiNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tPuiNum: 0x%hx; wCuiNum: %d;\n", wPuiNum, wCuiNum);
        }
        OspPrintf(TRUE, FALSE, ";\n");
    }
    void NetToHost();
    void HostToNet();
}TTnmLaDenPfmInfo;

// 2.4.4 获取平台各模块连接状态
//  平台设备模块连接状态参数
typedef struct tagTnmCmuModuleStateInfo
{
	s8 achModuleIp[TNM_MAX_DEV_NETADDR_LEN + 1]; //模块ip地址
	s8 achModuleId[TNM_MAX_DEVICE_ID_LEN + 1];   //模块id
	u8 byModuleState;			                 //模块连接状态 0:表示未连接 1:表示已连接上
    u8 byModuleType;                             //模块类型
}PACKED TTnmCmuModuleStateInfo;


inline void TTnmLaDenPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    wPuiNum = ntohs(wPuiNum);
    wCuiNum = ntohs(wCuiNum);
}
inline void TTnmLaDenPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    wPuiNum = htons(wPuiNum);
    wCuiNum = htons(wCuiNum);
}

//Laden扩展性能参数-PUI列表
typedef struct tagTnmLadenPuiInfo
{
    tagTnmLadenPuiInfo()
    {
        tDevId.SetNull();
        dwPuNum = 0;
    }
    TTnmDeviceId tDevId;    //Pui设备ID
    u32 dwPuNum;            //前端设备个数

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwPuNum: %u;\n", tDevId.achID, ntohl(dwPuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwPuNum: %lu;\n", tDevId.achID, dwPuNum);
        }
    }

    void NetToHost();
    void HostToNet();

}TTnmLadenPuiInfo;

inline void TTnmLadenPuiInfo::NetToHost()
{
    dwPuNum = ntohl(dwPuNum);
}
inline void TTnmLadenPuiInfo::HostToNet()
{
    dwPuNum = htonl(dwPuNum);
}

//Laden扩展性能参数-CUI列表
typedef struct tagTnmLadenCuiInfo
{
    tagTnmLadenCuiInfo()
    {
        tDevId.SetNull();
        dwCuNum = 0;
    }
    TTnmDeviceId tDevId;    //Pui设备ID
    u32 dwCuNum;            //CU个数

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwCuNum: %u;\n", tDevId.achID, ntohl(dwCuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; dwCuNum: %lu;\n", tDevId.achID, dwCuNum);
        }
    }

    void NetToHost();
    void HostToNet();

}TTnmLadenCuiInfo;

inline void TTnmLadenCuiInfo::NetToHost()
{
    dwCuNum = ntohl(dwCuNum);
}

inline void TTnmLadenCuiInfo::HostToNet()
{
    dwCuNum = htonl(dwCuNum);
}

// 2.5 VPU
typedef struct tagTnmVpuEncState
{
    u8  byEncStatus;        //是否在编码(TNM_DISABLE TNM_ENABLE:在编码)
    u8  bySendFlag;         //是否在发送(TNM_DISABLE TNM_ENABLE:在发送)
    u8  byVideoResolution;  //图象分辨率
    u8  byVideoType;        //编码格式 TNM_VIDEO_TYPE_H263
    u8  byFrameRate;        //帧率 1-25
    u8  byVpuStyle;         //多画面风格
    u16 wReserved;          //保留2
    u32 dwBitRate;          //编码码率 64K-4M

    
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        byEncStatus = TNM_DISABLE;
        bySendFlag  = TNM_DISABLE;
        byVideoResolution   = TNM_VIDEO_RESOLUTION_NONE;
        byVideoType = TNM_VIDEO_TYPE_H263;
        byFrameRate = 25;
        byVpuStyle  = TNM_VPU_STYLE_INVALID;
        dwBitRate   = 64;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "EncStatus: %d; SFlag: %d; VResol: %d; BitRate: 0x%lx;\n", 
                byEncStatus, bySendFlag, byVideoResolution, dwBitRate);
        OspPrintf(TRUE, FALSE, "\tVType: %d; FramRate: %d; VpuStyle: %d;\n",
                byVideoType, byFrameRate, byVpuStyle);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuEncState;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVpuEncState::NetToHost()
{
    dwBitRate = ntohl(dwBitRate);
}
inline void TTnmVpuEncState::HostToNet()
{
    dwBitRate = htonl(dwBitRate);
}
typedef struct tagTnmVpuDecState
{
    u8  byDecStatus;        //是否在解码(TNM_DISABLE TNM_ENABLE:在解码)
    u8  byRcvFlag;          //是否在接收(TNM_DISABLE TNM_ENABLE:在接收)
    u8  byLosePkgRate;      //丢包率
    u8  byReserved1;        //保留1
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        byDecStatus = TNM_DISABLE;
        byRcvFlag   = TNM_DISABLE;
        byLosePkgRate   = 0;
        byReserved1 = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        OspPrintf(TRUE, FALSE, "DecStatus: %d; RFlag: %d; LoseRate: %d;\n",
                byDecStatus, byRcvFlag, byLosePkgRate);
    }
}PACKED TTnmVpuDecState;

typedef struct tagTnmVpuRcvChnStatus
{
    tagTnmVpuRcvChnStatus()
    {
        dwChnIp = 0;
        wChnPort = 0;
        dwRecvBitRate = 0;
        dwRecvLosePackNum = 0;
        dwRecvPackNum = 0;
    }
    u32     dwChnIp;                //所接收通道的Ip
    u16     wChnPort;               //所接收通道的Port
    u32     dwRecvBitRate;          //接收码率
    u32     dwRecvPackNum;          //收到的包数
    u32     dwRecvLosePackNum;      //网络接收缺少的包数
    u16     wReserved;
    void Print(BOOL32 bNetToHost = FALSE);
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuRcvChnStatus;

inline void TTnmVpuRcvChnStatus::Print(BOOL32 bNetToHost)
{
    if (bNetToHost)
    {
        OspPrintf(TRUE, FALSE, "\tChnIp:%s; ChnPort:%d; RcvRate: %d;\n", 
            ::GetIpStr(ntohl(dwChnIp)), ntohs(wChnPort), ntohl(dwRecvBitRate));
        OspPrintf(TRUE, FALSE, "\tRcvPackNum: %d; LosePackNum: %d;\n", 
            ntohl(dwRecvPackNum), ntohl(dwRecvLosePackNum));
    }
    else
    {        
        OspPrintf(TRUE, FALSE, "\tChnIp:%s; ChnPort:%hu; RcvRate: %lu;\n", 
            ::GetIpStr(dwChnIp), wChnPort, dwRecvBitRate);
        OspPrintf(TRUE, FALSE, "\tRcvPackNum: %lu; LosePackNum: %lu;\n", 
            dwRecvPackNum, dwRecvLosePackNum);
    }
}

inline void TTnmVpuRcvChnStatus::NetToHost()
{
    dwChnIp = ntohl(dwChnIp);
    wChnPort = ntohs(wChnPort);
    dwRecvBitRate = ntohl(dwRecvBitRate);
    dwRecvPackNum = ntohl(dwRecvPackNum);
    dwRecvLosePackNum = ntohl(dwRecvLosePackNum);
}

inline void TTnmVpuRcvChnStatus::HostToNet()
{
    dwChnIp = htonl(dwChnIp);
    wChnPort = htons(wChnPort);
    dwRecvBitRate = htonl(dwRecvBitRate);
    dwRecvPackNum = htonl(dwRecvPackNum);
    dwRecvLosePackNum = htonl(dwRecvLosePackNum);
}


typedef struct tagTnmVpuPfmInfo : public TTnmBoardPfmInfo
{   
    u8          byMergeStart;           //是否开始画面合成 0 否/1 是
    u8          byType;                 //画面合成类型    
    u8          byCurChnNum;            //当前参与合成的通道数
    u32         dwTotalMapNum;          //Map使用标志
    u32         dwNetSndIp;             //网络发送地址Ip
    u16         wNetSndPort;            //网络发送地址port
    u32         dwSendBitRate;          //发送码率
    u32         dwSendPackNum;          //发送的包数
    u32         dwSendDiscardPackNum;   //发送丢包数
    u8          byReserved;
    u16         wReserved2;
    // 2006/04/03 李洪强 增加初始化接口  2006.09.14 renhouping 修改
    void Init()
    {
        byMergeStart = TNM_DISABLE;
        byType = 0;
        byCurChnNum = 0;
        dwTotalMapNum = 0;
        dwNetSndIp = 0;
        wNetSndPort = 0;
        dwSendBitRate = 0;
        dwSendPackNum = 0;
        dwSendDiscardPackNum = 0;        
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost)
        {
            OspPrintf(TRUE, FALSE, "\tMapNum:%d; SndIp:0x%x; SndPort:%d;\n", 
                    ntohl(dwTotalMapNum), ntohl(dwNetSndIp), ntohs(wNetSndPort));
            OspPrintf(TRUE, FALSE, "\tSndRate:%d; SndPackNum:%d; SndLostNum:%d;\n", 
                    ntohl(dwSendBitRate), ntohl(dwSendPackNum), ntohl(dwSendDiscardPackNum));            
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tMapNum:%lu; SndIp:0x%lx; SndPort:%hu;\n", 
                    dwTotalMapNum, dwNetSndIp, wNetSndPort);
            OspPrintf(TRUE, FALSE, "\tSndRate:%lu; SndPackNum:%lu; SndLostNum:%lu;\n", 
                    dwSendBitRate, dwSendPackNum, dwSendDiscardPackNum);
        }
        OspPrintf(TRUE, FALSE, "\tMegStart: %d; Type: %d; ChnNum: %d; \n",
                byMergeStart, byType, byCurChnNum);

    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVpuPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVpuPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();
    dwTotalMapNum = ntohl(dwTotalMapNum);
    dwNetSndIp = ntohl(dwNetSndIp);
    wNetSndPort = ntohs(wNetSndPort);
    dwSendBitRate = ntohl(dwSendBitRate);
    dwSendPackNum = ntohl(dwSendPackNum);
    dwSendDiscardPackNum = ntohl(dwSendDiscardPackNum);
}
inline void TTnmVpuPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::NetToHost();
    dwTotalMapNum = htonl(dwTotalMapNum);
    dwNetSndIp = htonl(dwNetSndIp);
    wNetSndPort = htons(wNetSndPort);
    dwSendBitRate = htonl(dwSendBitRate);
    dwSendPackNum = htonl(dwSendPackNum);
    dwSendDiscardPackNum = htonl(dwSendDiscardPackNum);
}

/*====================================================================
2.6. NRU性能参数
====================================================================*/
#define MAX_REC_DIR_LEN             128     // 录像最大目录长度
#define MAX_REC_FILE_NAME_LEN       128      // 录像文件名最大长度
#define MAX_NRU_CUID_LEN            24      // 客户端ID最大长度
#define MAX_NRU_MAX_ADDR_NUM        10      // NRU最大IP地址数目
#define MAX_STREAM_PORT_NUM         4       // 媒体流最大端口数目

// 录像任务基本信息
typedef struct tagTnmRecTask
{
    tagTnmRecTask()
    {
        memset(achFileDir, 0, sizeof(achFileDir));
        memset(achFileName, 0, sizeof(achFileName));
        dwStartTime = 0;
        dwEndTime = 0;
    }
    s8  achFileDir[MAX_REC_DIR_LEN + 4];            // 录象目录
    s8  achFileName[MAX_REC_FILE_NAME_LEN + 4];     // 录象文件名
    u32 dwStartTime;                            // 录象开始时间
    u32 dwEndTime;                              // 录象结束时间
    TTnmUserId tUserId;                         // 客户或用户ID
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwStart = dwStartTime;
        u32 dwEnd = dwEndTime;
        if (bNetToHost)
        {
            dwStart = ntohl(dwStart);
            dwEnd = ntohl(dwEnd);
        }
        
        OspPrintf(TRUE, FALSE, "\tFDir=%s\n\tFName=%s\n", achFileDir, achFileName);
        OspPrintf(TRUE, FALSE, "\tSTime=%s\n", ::GetTimeStr(dwStart));
        OspPrintf(TRUE, FALSE, "\tETime=%s\n", ::GetTimeStr(dwEnd));
        OspPrintf(TRUE, FALSE, "\tUsrId=%s\n", tUserId.achID);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmRecTask;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmRecTask::NetToHost()
{
    dwStartTime = ntohl(dwStartTime);
    dwEndTime = ntohl(dwEndTime);
}
inline void TTnmRecTask::HostToNet()
{
    dwStartTime = htonl(dwStartTime);
    dwEndTime = htonl(dwEndTime);
}
// 录像媒体基本信息
typedef struct tagTnmNruMediaInfo
{
    tagTnmNruMediaInfo()
    {
        memset(this, 0, sizeof(tagTnmNruMediaInfo));
    }
    u32 dwListenIpAddr;                         // 侦听地址
    u16 awLocalListenPort[MAX_STREAM_PORT_NUM]; // 侦听端口
    u8  byStreamPortNum;                        // 码流端口数量
    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "PortNum: %d; LsnIp: %s; \n",
                byStreamPortNum, ::GetIpStr(ntohl(dwListenIpAddr)));
            OspPrintf(TRUE, FALSE, "\tLsnPort1: 0x%hx; LsnPort2: 0x%hx; LsnPort3: 0x%hx;\n",
                ntohs(awLocalListenPort[0]), ntohs(awLocalListenPort[1]), ntohs(awLocalListenPort[2]));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "PortNum: %d; LsnIp: %s; \n",
                byStreamPortNum, ::GetIpStr(dwListenIpAddr));
            OspPrintf(TRUE, FALSE, "\tLsnPort1: 0x%hx; LsnPort2: 0x%hx; LsnPort3: 0x%hx;\n",
                awLocalListenPort[0], awLocalListenPort[1], awLocalListenPort[2]);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruMediaInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruMediaInfo::NetToHost()
{
    s32 nIndex;
    dwListenIpAddr = ntohl(dwListenIpAddr);
    for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalListenPort[nIndex] = ntohs(awLocalListenPort[nIndex]);
    }
}
inline void TTnmNruMediaInfo::HostToNet()
{
    s32 nIndex;
    dwListenIpAddr = htonl(dwListenIpAddr);
    for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalListenPort[nIndex] = htons(awLocalListenPort[nIndex]);
    }
}

// 2.6.1 录像任务参数(扩展参数)
typedef struct tagTnmNruRecTask : public TTnmRecTask
{
    tagTnmNruRecTask()
    {
        tDevId.SetNull();
        dwDevChannelId = 0;
        byHasAudio = TNM_AUDIO_VIDEO;
        tDevId.SetNull();
        dwCurRecSpace = 0;
    }
    TTnmDeviceId    tDevId;                 // 设备 id
    u32             dwDevChannelId;         // 设备通道id
    u32             dwCurRecSpace;          // 当前录像任务的已录空间(M)
    TTnmNruMediaInfo tMediaInfo;            // 媒体信息
    u8              byHasAudio;             // 是否存在音频 TNM_ONLY_VIDEO/TNM_AUDIO_VIDEO

    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        
        u32 dwChnnlId = dwDevChannelId;
        u32 dwSpace = dwCurRecSpace;
        if (bNetToHost) 
        {
            dwChnnlId = ntohl(dwDevChannelId);
            dwSpace = ntohl(dwSpace);
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s\n\tChId=0x%lx, Space=0x%lx, Audio=%d\n",
            tDevId.achID, dwChnnlId, dwSpace, byHasAudio);
        OspPrintf(TRUE, FALSE, "\t");
        tMediaInfo.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruRecTask;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruRecTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwDevChannelId = ntohl(dwDevChannelId);
    dwCurRecSpace = ntohl(dwCurRecSpace);
    tMediaInfo.NetToHost();
}
inline void TTnmNruRecTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwDevChannelId = htonl(dwDevChannelId);
    dwCurRecSpace = htonl(dwCurRecSpace);
    tMediaInfo.HostToNet();
}
// 2.6.2 放象任务(扩展参数)
// 录放像进度
typedef struct tagTnmRecProg
{
    tagTnmRecProg()
    {
        dwCurProg = 0;
        dwTotalTime = 0;
    }
    u32 dwCurProg;                          // 当前进度（单位：秒）;
    u32 dwTotalTime;                        // 总长度，仅在放像时有效（单位：秒）;    
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwCur = dwCurProg;
        u32 dwTotal = dwTotalTime;
        if (bNetToHost) 
        {
            dwCur = ntohl(dwCurProg);
            dwTotal =  ntohl(dwTotalTime);
        }
        OspPrintf(TRUE, FALSE, "CurProg=0x%lx, TotalTime=0x%lx\n",dwCur, dwTotal);
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmRecProg;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmRecProg::NetToHost()
{
    dwCurProg = ntohl(dwCurProg);
    dwTotalTime = ntohl(dwTotalTime);
}
inline void TTnmRecProg::HostToNet()
{
    dwCurProg = htonl(dwCurProg);
    dwTotalTime = htonl(dwTotalTime);
}
typedef struct tagTnmNruPlayTask : public TTnmRecTask
{
    tagTnmNruPlayTask()
    {
        s32 nIndex;
        dwLocAddr   = 0;
        dwRemoteAddr    = 0;
        for(nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
        {
            awLocalPort[nIndex] = 0;
            awRemotePort[nIndex] = 0;
        }
        byPlayState = RP_STATE_NULL;
        byPlayRate  = ONE_TIMES;
        dwPlayChannel   = 0;
    }
    u32 dwLocAddr;                          // 本地地址
    u32 dwRemoteAddr;                       // 对端地址
    u16 awLocalPort[MAX_STREAM_PORT_NUM + 2];   // 本地端口列表
    u16 awRemotePort[MAX_STREAM_PORT_NUM + 2];  // 对端端口列表
    TTnmRecProg m_tRecProg;                 // 放像进度;
    u32 dwPlayChannel;                      // 放象通道;
    u8  byPlayState;                        // 放像状态 enum TnmRpState 
    u8  byPlayRate;                         // 放像速率;enum TnmPlayRate
    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        u32 dwLocalIp = dwLocAddr;
        u32 dwRIp = dwRemoteAddr;
        u32 dwChnnl = dwPlayChannel;
        if (bNetToHost) 
        {
            dwLocalIp = ntohl(dwLocAddr);
            dwRIp = ntohl(dwRemoteAddr);
            dwChnnl = ntohl(dwPlayChannel);
        }
        OspPrintf(TRUE, FALSE, "\tLIp=%s, ", ::GetIpStr(dwLocalIp));
        OspPrintf(TRUE, FALSE, "RIp=%s\n", ::GetIpStr(dwRIp));
        OspPrintf(TRUE, FALSE, "\tPlayState=%d, PlayRate=%d, PlayChannel=0x%lx;\n",
            byPlayState, byPlayRate, dwChnnl);
        OspPrintf(TRUE, FALSE, "\t");
        m_tRecProg.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPlayTask;

/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruPlayTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwLocAddr = ntohl(dwLocAddr);
    dwRemoteAddr = ntohl(dwRemoteAddr);
    for(s32 nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalPort[nIndex] = ntohs(awLocalPort[nIndex]);
        awRemotePort[nIndex] = ntohs(awRemotePort[nIndex]);
    }
    m_tRecProg.NetToHost();
    dwPlayChannel = ntohl(dwPlayChannel);
}
inline void TTnmNruPlayTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwLocAddr = htonl(dwLocAddr);
    dwRemoteAddr = htonl(dwRemoteAddr);
    for(s32 nIndex = 0; nIndex < MAX_STREAM_PORT_NUM; nIndex++)
    {
        awLocalPort[nIndex] = htons(awLocalPort[nIndex]);
        awRemotePort[nIndex] = htons(awRemotePort[nIndex]);
    }
    m_tRecProg.HostToNet();
    dwPlayChannel = htonl(dwPlayChannel);
}

// 2.6.3 下载任务(扩展参数)
typedef struct tagTnmNruDownLoadTask : public TTnmRecTask
{
    tagTnmNruDownLoadTask()
    {
        dwSendIP = 0;
        wSendPort = 0;
        dwRcvIP = 0;
        wRcvPort = 0;
    }

    u32 dwSendIP;                               // 发送端IP
    u32 dwRcvIP;                                // 接收端IP
    u16 wSendPort;                              // 发送端端口
    u16 wRcvPort;                               // 接收端端口

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmRecTask::Print(bNetToHost);
        
        u32 dwSend = dwSendIP;
        u32 dwRcv = dwRcvIP;
        u16 wSPort = wSendPort;
        u16 wRPort = wRcvPort;
        if (bNetToHost) 
        {
            dwSend =ntohl(dwSendIP);
            dwRcv = ntohl(dwRcvIP);
            wSPort = ntohs(wSendPort);
            wRPort = ntohs(wRcvPort);
        }
        OspPrintf(TRUE, FALSE, "\tSIp=%s, ", ::GetIpStr(dwSend));
        OspPrintf(TRUE, FALSE, "RIp=%s, SPort=0x%hx, RPort=0x%hx\n",
            ::GetIpStr(dwRcv), wSPort, wRPort);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruDownLoadTask;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruDownLoadTask::NetToHost()
{
    TTnmRecTask::NetToHost();

    dwSendIP = ntohl(dwSendIP);
    dwRcvIP = ntohl(dwRcvIP);
    wSendPort = ntohs(wSendPort);
    wRcvPort = ntohs(wRcvPort);
}
inline void TTnmNruDownLoadTask::HostToNet()
{
    TTnmRecTask::HostToNet();

    dwSendIP = htonl(dwSendIP);
    dwRcvIP = htonl(dwRcvIP);
    wSendPort = htons(wSendPort);
    wRcvPort = htons(wRcvPort);
}
// 2.6.4 预录任务(扩展参数)
typedef struct tagTnmNruPreRecTask 
{
    tagTnmNruPreRecTask()
    {
        tDevId.SetNull();
        dwDevChannelId  = 0;
        dwPreRecTime = 0;
        byHasAudio = TNM_AUDIO_VIDEO;
        tDevId.SetNull();
    }

    TTnmDeviceId tDevId;                        // 设备 id
    u32 dwDevChannelId;                         // 设备通道id
    u32 dwPreRecTime;                           // 预录时间长度

    TTnmNruMediaInfo tMediaInfo;                // 媒体信息
    TTnmUserId      tUserId;                    // 客户或用户ID
    u8  byHasAudio;                             // 是否存在音频
    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;
    u8  byReserve3;
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwChnnlId = dwDevChannelId;
        u32 dwRecTime = dwPreRecTime;
        if (bNetToHost) 
        {
            dwChnnlId = ntohl(dwDevChannelId);
            dwRecTime = ntohl(dwPreRecTime);
        }
        OspPrintf(TRUE, FALSE, "\tDevId=%s, ChId=0x%lx, PRecTime=0x%lx\n",
            tDevId.achID, dwChnnlId, dwRecTime);
        OspPrintf(TRUE, FALSE, "\tUsrId=%s, Audio=%d\n", tUserId.achID, byHasAudio);
        OspPrintf(TRUE, FALSE, "\t");
        tMediaInfo.Print(bNetToHost);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPreRecTask;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruPreRecTask::NetToHost()
{
    dwDevChannelId = ntohl(dwDevChannelId);
    dwPreRecTime = ntohl(dwPreRecTime);
    tMediaInfo.NetToHost();
}
inline void TTnmNruPreRecTask::HostToNet()
{
    dwDevChannelId = htonl(dwDevChannelId);
    dwPreRecTime = htonl(dwPreRecTime);
    tMediaInfo.HostToNet();
}
     
// 2.6.5 NRU基本性能参数
#define DISK_VALID      (u8)1
#define DISK_INVALID    (u8)0
typedef struct tagTnmNruPfmInfo : public TTnmPfmHwInfo
{
    u32 adwRemainDisk[TNM_MAX_NRU_DISK_NUM +1];   // 各磁盘剩余容量查询（只读）（M）
    u8  abyDiskState[TNM_MAX_NRU_DISK_NUM + 1];       // 各磁盘是否可用，和上面的数组一一对应DISK_VALID/DISK_INVALID 
    u32 dwRecTaskNum;       // 录像任务路数 
    u32 dwRecBitRate;       // 录像码率(bps), 暂时未提供
    u8  byCurDiskNum;       // 当前使用了多少磁盘
    u8  byCpuAlarm;         // CPU状态（只读） enum TnmStatusType
    u8  byDiskFullAlarm;    // 磁盘状态（只读）enum TnmStatusType
    
    u8  byReserve1;     //
    u32 adwFullDisk[TNM_MAX_NRU_DISK_NUM +1];   // 磁盘总容量（只读）（M）
    
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        byCurDiskNum    = 0;
        for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM+1; nIndex++)
        {
            adwRemainDisk[nIndex] = 0;
            adwFullDisk[nIndex] = 0;
            abyDiskState[nIndex] = 0;
        }

        byCpuAlarm  = TNM_STATUS_NONE;
        byDiskFullAlarm = TNM_STATUS_NONE;
        dwRecTaskNum    = 0;
        dwRecBitRate    = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmPfmHwInfo::Print(bNetToHost);
        OspPrintf(TRUE, FALSE, "\tDiskNum: %d; CpuAlarm: %d; DiskFull: %d;\n", 
            byCurDiskNum, byCpuAlarm, byDiskFullAlarm);
        OspPrintf(TRUE, FALSE, "\tDiskRemain: ");
        for (s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
        {
            u32 dwRemain = adwRemainDisk[nIndex];
            u32 dwFull = adwFullDisk[nIndex];
            if (bNetToHost) 
            {
                dwRemain = ntohl(dwRemain);
                dwFull = ntohl(dwFull);
            }
            if (dwRemain > 0) 
            {
                OspPrintf(TRUE, FALSE, "(%d, RemainDisk: 0x%lx), State: %d", nIndex, dwRemain, abyDiskState[nIndex]);
            }
            if (dwFull > 0)
            {
                OspPrintf(TRUE, FALSE, "(%d, FullDisk: 0x%lx), State: %d", nIndex, dwFull, abyDiskState[nIndex]);
            }
        }
        OspPrintf(TRUE, FALSE, "\n");
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tRecTaskNum: 0x%x; RecBitRate: 0x%x;\n",
                ntohl(dwRecTaskNum), ntohl(dwRecBitRate));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tRecTaskNum: 0x%lx; RecBitRate: 0x%lx;\n", dwRecTaskNum, dwRecBitRate);
        }
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmNruPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmNruPfmInfo::NetToHost()
{
    TTnmPfmHwInfo::NetToHost();

    for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
    {
        adwRemainDisk[nIndex] = ntohl(adwRemainDisk[nIndex]);
        adwFullDisk[nIndex] = ntohl(adwFullDisk[nIndex]);
    }
    dwRecTaskNum = ntohl(dwRecTaskNum);
    dwRecBitRate = ntohl(dwRecBitRate);
}
inline void TTnmNruPfmInfo::HostToNet()
{
    TTnmPfmHwInfo::HostToNet();

    for(s32 nIndex = 0; nIndex < TNM_MAX_NRU_DISK_NUM; nIndex++)
    {
        adwRemainDisk[nIndex] = htonl(adwRemainDisk[nIndex]);
        adwFullDisk[nIndex] = htonl(adwFullDisk[nIndex]);
    }
    dwRecTaskNum = htonl(dwRecTaskNum);
    dwRecBitRate = htonl(dwRecBitRate);
}

//获取Nru的磁盘录像点数
typedef struct tagTnmNruDiskPfmInfo
{
    tagTnmNruDiskPfmInfo()
    {
        memset(achMountPath, 0, sizeof(achMountPath));
        dwRecNum =0;
		dwTotalSize = 0;
		dwFreeSize = 0;
    }

    s8 achMountPath[MAX_MOUNTPATH_LEN];	//挂载分区名
    u32 dwRecNum;						//录像个数
	u32 dwTotalSize;					//分区总容量，M为单位
	u32 dwFreeSize;						//分区可用容量，M为单位

    
    void NetToHost();
    void HostToNet();
    
}PACKED TTnmNruDiskPfmInfo;

inline void tagTnmNruDiskPfmInfo::NetToHost()
{
    dwRecNum = ntohl(dwRecNum);
	dwTotalSize = ntohl(dwTotalSize);
	dwFreeSize = ntohl(dwFreeSize);
}

inline void tagTnmNruDiskPfmInfo::HostToNet()
{
    dwRecNum = htonl(dwRecNum);
	dwTotalSize = htonl(dwTotalSize);
	dwFreeSize = htonl(dwFreeSize);
}
/*====================================================================
2.7. 3AS性能参数
====================================================================*/
typedef struct tagTnm3asPfmInfo : public TTnmBasePfmInfo
{
    u32 dwUserLogonNum;     // 登录用户数
    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        dwUserLogonNum  = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tUsrLogonNum: 0x%x; \n", ntohl(dwUserLogonNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tUsrLogonNum: 0x%lx; \n", dwUserLogonNum);
        }
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnm3asPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnm3asPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    dwUserLogonNum = ntohl(dwUserLogonNum);
}
inline void TTnm3asPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    dwUserLogonNum = htonl(dwUserLogonNum);
}
/*====================================================================
2.8. UAS性能参数
====================================================================*/
typedef struct tagTnmUasPfmInfo : public TTnmBasePfmInfo
{
    u32 dwCmuLinkNum;       // CMU连接数
    u8  byNetUseRate;       // 网络资源使用率 100%
    u8  byDBStatus;         // 数据库连接状态 enum TnmStatusType

    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;

    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        byNetUseRate    = 0;
        byDBStatus  = TNM_STATUS_NONE;
        dwCmuLinkNum    = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBasePfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tNUseRate: %d; DbStatus: %d; LinkNum: 0x%x;\n",
                byNetUseRate, byDBStatus, ntohl(dwCmuLinkNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tNUseRate: %d; DbStatus: %d; LinkNum: 0x%lx;\n",
                byNetUseRate, byDBStatus, dwCmuLinkNum);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmUasPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmUasPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    dwCmuLinkNum = ntohl(dwCmuLinkNum);
}
inline void TTnmUasPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    dwCmuLinkNum = htonl(dwCmuLinkNum);
}

// 2.9 CUI 性能参数
// 2.9.1 CUI 基本性能参数
typedef struct tagTnmCuiPfmInfo : public TTnmBoardPfmInfo
{
    u16 wCuLinkNum;             // 当前正在连接的CU数目

    u8  byHotBackupState;       //活动热备服务器TNM_HB_STATE_ACTIVE
    u8  byReserve2;

    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        wCuLinkNum = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tCuLinkNum: 0x%hx;\n", ntohs(wCuLinkNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tCuLinkNum: 0x%hx;\n", wCuLinkNum);
        }
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCuiPfmInfo::NetToHost()
{
    TTnmBasePfmInfo::NetToHost();

    wCuLinkNum = ntohs(wCuLinkNum);
}
inline void TTnmCuiPfmInfo::HostToNet()
{
    TTnmBasePfmInfo::HostToNet();

    wCuLinkNum = htons(wCuLinkNum);
}

// 2.9.2 CUI 扩展性能参数
typedef struct tagTnmCuiPfmInfoEx
{
    tagTnmCuiPfmInfoEx()
    {
        dwCuIp = 0;
        dwLoginTime = 0;
    }
    TTnmUserId tUserId;         // CU用户登录名
    u32 dwCuIp;                 // CU的登录IP
    u32 dwLoginTime;            // CU登录的时间
    
    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwTime = dwLoginTime;
        u32 dwIp = dwCuIp;
        if (bNetToHost) 
        {
            dwTime = ntohl(dwTime);
            dwIp = ntohl(dwIp);
        }
        
        OspPrintf(TRUE, FALSE, "UsrId: %s; CuIp: %s;\n",
                          tUserId.achID, ::GetIpStr(dwIp)); 
        if (dwTime & 0x80000000) //如果最高位为 1 说明是非法时间变量
        {
            OspPrintf(TRUE, FALSE, "\t时间值非法: %lx;\n", dwTime);
        }
        else
        {
            struct tm tmTime = *localtime((time_t*)&dwTime);
            OspPrintf(TRUE, FALSE, "\tLoginTime: %s;\n", asctime(&tmTime));
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmCuiPfmInfoEx;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmCuiPfmInfoEx::NetToHost()
{
    dwCuIp = ntohl(dwCuIp);
    dwLoginTime = ntohl(dwLoginTime);
}
inline void TTnmCuiPfmInfoEx::HostToNet()
{
    dwCuIp = htonl(dwCuIp);
    dwLoginTime = htonl(dwLoginTime);
}

// 2.10 PUI 性能参数
// 2.10.1 PUI 基本性能参数
typedef struct tagTnmPuiPfmInfo : public TTnmBoardPfmInfo
{
    u16 wCurPuNum;              // 当前连接的PU数量
    
    u8  byHotBackupState; //活动热备服务器TNM_HB_STATE_ACTIVE
    u8  byReserve2;

    // 2006/04/03 李洪强 增加初始化接口
    void Init()
    {
        wCurPuNum   = 0;
    }
    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tCurPuNum: 0x%hx; \n", ntohs(wCurPuNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tCurPuNum: 0x%hx; \n", wCurPuNum);
        }
        
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuiPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wCurPuNum = ntohs(wCurPuNum);
}
inline void TTnmPuiPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wCurPuNum = htons(wCurPuNum);
}
// 2.10.2 PUI 扩展性能参数
typedef struct tagTnmPuiPfmInfoEx
{
    tagTnmPuiPfmInfoEx()
    {
        dwPuIp = 0;
        byServiceType = TNM_SERVICE_UNKNOW;
        memset(abyGuid, 0, sizeof(abyGuid));
        tPuId.SetNull();
    }
    u32 dwPuIp;                     // PU的IP地址
    TTnmDeviceId tPuId;             // PU的的设备ID
    u8  abyGuid[TNM_PU_GUID_LEN];   // 设备的GUID
    u8  byServiceType;              // 设备的业务类型enum TnmServiceType
    u8  byReserve1;     // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;
    u8  byReserve3;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        tPuId.achID[TNM_MAX_DEVICE_ID_LEN+3] = '\0';
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "PuIp: %s; PuId: %s; SType: %d;\n",
                ::GetIpStr(ntohl(dwPuIp)), tPuId.achID, byServiceType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "PuIp: %s; PuId: %s; SType: %d;\n",
                ::GetIpStr(dwPuIp), tPuId.achID, byServiceType);
        }
        OspPrintf(TRUE, FALSE, "\tGuid: ");
        for (s32 nIndex = 0; nIndex < TNM_PU_GUID_LEN; nIndex++)
        {
            OspPrintf(TRUE, FALSE, "%d-", abyGuid[nIndex]);
        }
        OspPrintf(TRUE, FALSE, "\n");
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmPuiPfmInfoEx;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmPuiPfmInfoEx::NetToHost()
{
    dwPuIp = ntohl(dwPuIp);
}
inline void TTnmPuiPfmInfoEx::HostToNet()
{
    dwPuIp = htonl(dwPuIp);
}

// 2.11 VTDU
// 2.11.1 VTDU基本性能参数
typedef struct tagTnmVtduPfmInfo : public TTnmBoardPfmInfo
{
    TTnmDeviceId tDevId;    // VTDU设备ID
    u16 wRcvStreamCurNum;                     // 接收视频流路数
    u16 wSndStreamCurNum;                     // 发送视频流路数
    u16 wRcvLostPackNum;    // 接收总丢包数
    u16 wTaskNum;           // 转发任务数
    u16 wRcvStreamMaxNum;                    // 最大接收视频流路数
    u16 wSndStreamMaxNum;                    // 最大发送视频流路数
    u32 dwRcvStreamMaxNumTime;              // 出现最大接收视频流路数的时间
    u32 dwSndStreamMaxNumTime;              // 出现最大发送视频流路数的时间
    void Init()
    {
        tDevId.SetNull();
        wRcvStreamCurNum    = 0;
        wSndStreamCurNum    = 0;
        wRcvLostPackNum = 0;
        wTaskNum        = 0;
        wRcvStreamMaxNum = 0;
        wSndStreamMaxNum = 0;
        dwRcvStreamMaxNumTime = 0;
        dwSndStreamMaxNumTime = 0;
    }

    void Print(BOOL32 bNetToHost = FALSE)
    {
        TTnmBoardPfmInfo::Print(bNetToHost);
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "\tDevId: %s; RcvNum: 0x%hx; SndNum: 0x%hx;\n",
                tDevId.achID, ntohs(wRcvStreamCurNum), ntohs(wSndStreamCurNum));
            OspPrintf(TRUE, FALSE, "\tLstNum: 0x%hx; TaskNum: 0x%hx;\n", 
                ntohs(wRcvLostPackNum), ntohs(wTaskNum));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "\tDevId: %s; RcvNum: 0x%hx; SndNum: 0x%hx;\n",
                tDevId.achID, wRcvStreamCurNum, wSndStreamCurNum);
            OspPrintf(TRUE, FALSE, "\tLstNum: 0x%hx; TaskNum: 0x%hx;\n",  wRcvLostPackNum, wTaskNum);
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduPfmInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVtduPfmInfo::NetToHost()
{
    TTnmBoardPfmInfo::NetToHost();

    wRcvStreamCurNum = ntohs(wRcvStreamCurNum);
    wSndStreamCurNum = ntohs(wSndStreamCurNum);
    wRcvLostPackNum = ntohs(wRcvLostPackNum);
    wTaskNum = ntohs(wTaskNum);
    wRcvStreamMaxNum = ntohs(wRcvStreamMaxNum);
    wSndStreamMaxNum = ntohs(wSndStreamMaxNum);
    dwRcvStreamMaxNumTime = ntohl(dwRcvStreamMaxNumTime);
    dwSndStreamMaxNumTime = ntohl(dwSndStreamMaxNumTime);
}
inline void TTnmVtduPfmInfo::HostToNet()
{
    TTnmBoardPfmInfo::HostToNet();

    wRcvStreamCurNum = htons(wRcvStreamCurNum);
    wSndStreamCurNum = htons(wSndStreamCurNum);
    wRcvLostPackNum = htons(wRcvLostPackNum);
    wTaskNum = htons(wTaskNum);
    wRcvStreamMaxNum = htons(wRcvStreamMaxNum);
    wSndStreamMaxNum = htons(wSndStreamMaxNum);
    dwRcvStreamMaxNumTime = htonl(dwRcvStreamMaxNumTime);
    dwSndStreamMaxNumTime = htonl(dwSndStreamMaxNumTime);
}
// 2.11.2 VTDU的扩展性能参数
typedef struct tagTnmVtduTask
{
    tagTnmVtduTask()
    {
        tDevId.SetNull();
        dwSrcDevIp      = 0;
        wSrcDevPort     = 0;
        dwRcvBitRate    = 0;
        dwRcvPackNum    = 0;
        dwRcvBytes      = 0;
        dwDestDevIp     = 0;
        wDestDevPort    = 0;
        dwSendBitRate   = 0;
        dwSendPacknum   = 0;
        dwSendBytes     = 0;
        byVideoType     = TNM_VIDEO_TYPE_NONE;
        byAudioType     = TNM_AUDIO_TYPE_NONE;
        byProtocolType  = TNM_MEDIA_STREAM_TYPE_TCP;
        byMediaType     = TNM_AUDIO_VIDEO;
    }
    TTnmDeviceId tDevId;    // VTDU设备ID
    u32 dwSrcDevIp;         // 源设备IP
    u32 dwRcvBitRate;       // 接收码率(kbps)
    u32 dwRcvPackNum;       // 接收包数
    u32 dwRcvBytes;         // 接收总字节数（byte）
    u32 dwDestDevIp;        // 目的设备IP
    u32 dwSendBitRate;      // 发送码流(kbps)
    u32 dwSendPacknum;      // 发送包数
    u32 dwSendBytes;        // 发送字节数(byte)
    u16 wSrcDevPort;        // 源设备端口
    u16 wDestDevPort;       // 目的设备端口
    u8  byVideoType;        // 视频码流格式
                            //  TNM_VIDEO_TYPE_SN4
                            //  TNM_VIDEO_TYPE_MPEG4
                            //  TNM_VIDEO_TYPE_H261
                            //  TNM_VIDEO_TYPE_H263
                            //  TNM_VIDEO_TYPE_H264
    u8  byAudioType;        // 音频码流格式
                            //  TNM_AUDIO_TYPE_PCMA
                            //  TNM_AUDIO_TYPE_PCMU
                            //  TNM_AUDIO_TYPE_GSM
                            //  TNM_AUDIO_TYPE_G729
                            //  TNM_AUDIO_TYPE_MP3
    u8 byProtocolType;      // 协议类型
                            //  TNM_MEDIA_STREAM_TYPE_UDP
                            //  TNM_MEDIA_STREAM_TYPE_TCP
    u8 byMediaType;         // 码流模块
                            //  TNM_ONLY_VIDEO
                            //  TNM_ONLY_AUDIO
                            //  TNM_AUDIO_VIDEO
    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; SrcIp: %s; SPort:0x%x; RcvRate:0x%x;\n",
                tDevId.achID, ::GetIpStr(ntohl(dwSrcDevIp)), ntohs(wSrcDevPort), ntohl(dwRcvBitRate));
            OspPrintf(TRUE, FALSE, "\tRcvNum: 0x%x; RcvByte: 0x%x; DstIp: %s; DstPort:0x%x;\n",
                ntohl(dwRcvPackNum), ntohl(dwRcvBytes), ::GetIpStr(ntohl(dwDestDevIp)), ntohs(wDestDevPort));
            OspPrintf(TRUE, FALSE, "\tSndRate: 0x%x; SndNum: 0x%x; SndByte:0x%x; VType:%d;\n",
                ntohl(dwSendBitRate), ntohl(dwSendPacknum), ntohl(dwSendBytes), byVideoType);
        }
        else
        {
            OspPrintf(TRUE, FALSE, "DevId: %s; SrcIp: %s; SPort:0x%hx; RcvRate:0x%lx;\n",
                tDevId.achID, ::GetIpStr(dwSrcDevIp), wSrcDevPort, dwRcvBitRate);
            OspPrintf(TRUE, FALSE, "\tRcvNum: 0x%lx; RcvByte: 0x%lx; DstIp: %s; DstPort:0x%hx;\n",
                dwRcvPackNum, dwRcvBytes, ::GetIpStr(dwDestDevIp), wDestDevPort);
            OspPrintf(TRUE, FALSE, "\tSndRate: 0x%lx; SndNum: 0x%lx; SndByte:0x%lx; VType:%d;\n",
                dwSendBitRate, dwSendPacknum, dwSendBytes, byVideoType);
        }
        OspPrintf(TRUE, FALSE, "\tAuType: %d; ProType: %d; MediaType: %d;\n",
            byAudioType, byProtocolType, byMediaType);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmVtduTask;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmVtduTask::NetToHost()
{
    dwSrcDevIp = ntohl(dwSrcDevIp);
    dwRcvBitRate = ntohl(dwRcvBitRate);
    dwRcvPackNum = ntohl(dwRcvPackNum);
    dwRcvBytes = ntohl(dwRcvBytes);
    dwDestDevIp = ntohl(dwDestDevIp);
    dwSendBitRate = ntohl(dwSendBitRate);
    dwSendPacknum = ntohl(dwSendPacknum);
    dwSendBytes = ntohl(dwSendBytes);
    wSrcDevPort = ntohs(wSrcDevPort);
    wDestDevPort = ntohs(wDestDevPort);
}
inline void TTnmVtduTask::HostToNet()
{
    dwSrcDevIp = htonl(dwSrcDevIp);
    dwRcvBitRate = htonl(dwRcvBitRate);
    dwRcvPackNum = htonl(dwRcvPackNum);
    dwRcvBytes = htonl(dwRcvBytes);
    dwDestDevIp = htonl(dwDestDevIp);
    dwSendBitRate = htonl(dwSendBitRate);
    dwSendPacknum = htonl(dwSendPacknum);
    dwSendBytes = htonl(dwSendBytes);
    wSrcDevPort = htons(wSrcDevPort);
    wDestDevPort = htons(wDestDevPort);
}
//////////////////////////////////////////////////////////////////////////
// 3.  设备拓朴图
// 3.1 设备拓朴条目
typedef struct tagTnmDevTopo
{
    tagTnmDevTopo()
    {
        tDevId.SetNull();
        memset(achDevName, 0, sizeof(achDevName));
        memset(achDevModeInfo, 0, sizeof(achDevModeInfo));
        memset(achNetAddress, 0, sizeof(achNetAddress));
    }

    TTnmDeviceId tDevId;                                // 设备的ID
    s8  achDevName[TNM_MAX_DEV_NAME_LEN + 4];           // 设备名称
    s8  achDevModeInfo[TNM_MAX_DEV_MODEINFO_LEN + 4];   // 设备型号
    s8  achNetAddress[TNM_MAX_DEV_NETADDR_LEN + 4];     // 设备网络地址，IP或域名(www.**)
}PACKED TTnmDevTopo;

// 4.设备告警
// 告警基本信息
typedef struct tagTnmDevAlarm
{
    tagTnmDevAlarm()
    {
        dwDevSerialNo = 0;
        dwAlarmCode = 0;
    }
    u32 dwDevSerialNo;          // 设备产生告警流水号，由设备动态生成。主要用于告警产生和恢复
                                // 的匹配。同一类告警在没有恢复之前，其上报的流水号是同一个
    u32 dwAlarmCode;            // 告警码，用于标识告警的种类,参见告警宏定义

    void Print(BOOL32 bNetToHost = FALSE)
    {
        u32 dwSn = dwDevSerialNo;
        u32 dwCode = dwAlarmCode;
        if (bNetToHost) 
        {
            dwSn = ntohl(dwDevSerialNo);
            dwCode = ntohl(dwAlarmCode);
        }
        
        OspPrintf(TRUE, FALSE, "Sn=0x%lx, code=%s(0x%lx), ", 
            dwSn, ::GetAlarmCodeDesc(dwCode), dwCode);
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmDevAlarm;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmDevAlarm::NetToHost()
{
    dwDevSerialNo = ntohl(dwDevSerialNo);
    dwAlarmCode = ntohl(dwAlarmCode);
}

inline void TTnmDevAlarm::HostToNet()
{
    dwDevSerialNo = htonl(dwDevSerialNo);
    dwAlarmCode = htonl(dwAlarmCode);
}

typedef struct tagTnmCuBrowseInfo
{
    tagTnmCuBrowseInfo()
    {
        m_startTime = 0;
        m_endTime = 0;
        memset(m_cuName, 0, sizeof(m_cuName));
        m_connectReqCount = 0;
        m_connectSuccessCount = 0;
        m_connectFailureCount = 0;
        m_releaseReqCount = 0;
        m_releaseSuccessCount = 0;
        m_releaseFailureCount = 0;
    }
    
    u32 m_startTime;
    u32 m_endTime;
    s8  m_cuName[TNM_MAX_STRING_LEN*2 + 4];
    u16 m_connectReqCount;
    u16 m_connectSuccessCount;
    u16 m_connectFailureCount;
    u16 m_releaseReqCount;
    u16 m_releaseSuccessCount;
    u16 m_releaseFailureCount;

    void NetToHost();
    void HostToNet();
}PACKED TTnmCuBrowseInfo;

inline void tagTnmCuBrowseInfo::NetToHost()
{
    m_startTime = ntohl(m_startTime);
    m_endTime = ntohl(m_endTime);
    m_connectReqCount = ntohs(m_connectReqCount);
    m_connectSuccessCount = ntohs(m_connectSuccessCount);
    m_connectFailureCount = ntohs(m_connectFailureCount);
    m_releaseReqCount = ntohs(m_releaseReqCount);
    m_releaseSuccessCount = ntohs(m_releaseSuccessCount);
    m_releaseFailureCount = ntohs(m_releaseFailureCount);
}

inline void tagTnmCuBrowseInfo::HostToNet()
{
    m_startTime = htonl(m_startTime);
    m_endTime = htonl(m_endTime);
    m_connectReqCount = htons(m_connectReqCount);
    m_connectSuccessCount = htons(m_connectSuccessCount);
    m_connectFailureCount = htons(m_connectFailureCount);
    m_releaseReqCount = htons(m_releaseReqCount);
    m_releaseSuccessCount = htons(m_releaseSuccessCount);
    m_releaseFailureCount = htons(m_releaseFailureCount);
}

//软件升级信息
typedef struct tagTnmVersionInfo
{
    u32 dwVersionServerAddr;                                //版本服务器地址
    u16 wVersionServerPort;                                 //版本服务器端口
    u16 wReserved;
    s8  achVerServUsername[TNM_MAX_USER_ID_LEN + 4];        //版本服务器登陆用户名
    s8  achVerServPassword[TNM_MAX_PASSWORD_LEN + 4];       //版本服务器登陆密码
    s8  achVersionFullPath[TNM_MAX_VERSION_PATH_LEN + 4];   //版本文件所在的全路径(包括文件名)
    void NetToHost();
    void HostToNet();
}TTnmVersionInfo;

inline void tagTnmVersionInfo::NetToHost()
{
    dwVersionServerAddr = ntohl(dwVersionServerAddr);
    wVersionServerPort = ntohs(wVersionServerPort);
}

inline void tagTnmVersionInfo::HostToNet()
{
    dwVersionServerAddr = htonl(dwVersionServerAddr);
    wVersionServerPort = htons(wVersionServerPort);
}

/************************************************************************/
/*                          软件更新相关                                */
/************************************************************************/
#define TNM_MAX_UPDATEFILE_LEN  32    //更新文件名最大长度
#define TNM_MAX_UPDATEPATH_LEN  128   //更新文件路径最大长度

#define TNM_MAX_DEVICE_BLOCKID_NUM      8       // 一个设备一次操作最多同时上传的模块个数

// 软件版本文件定义
#define BLOCK_ID_INVALID            (u8)0xFF    // 无效的内存块ID
#define BLOCK_ID_MDOS_Z             (u8)1       // mdos.z
#define BLOCK_ID_MEDIACTRLKDM       (u8)2       // Mediactrlkdm
#define BLOCK_ID_FONT               (u8)3       // FONT
#define BLOCK_ID_KDMVS_Z            (u8)4       // KDMVS.z
#define BLOCK_ID_KDM2417Z_IOS       (u8)5       // kdm2417z.ios
#define BLOCK_ID_MEDIACTRL          (u8)6       // mediactrl
#define BLOCK_ID_KERNEL_UPACK       (u8)7       // kernel.flashsei.upack
#define BLOCK_ID_KDMAPP_TGZ         (u8)8       // kdmapp.tgz
#define BLOCK_ID_MODULES_TGZ        (u8)9       // modules.tgz
#define BLOCK_ID_KDM2428Z_IOS       (u8)10      // kdm2428z.ios
#define BLOCK_ID_KDMVS_BIN          (u8)11      // KDMVS.bin

//  KDM2400/KDM2500：   mdos.z,Mediactrlkdm, FONT, KDMVS.z
//  KDM2417：           kdm2417z.ios,  KdmMediaCtrl, FONT, KDMVS.z
//  KDM2428：           kdm2428z.ios, KdmMediaCtrl, FONT, KDMVS.z
//  KDM2418/KDM2518：   kernel.flashsei.upack, kdmapp.tgz, modules.tgz


//cmu
#define BLOCK_ID_MCU_IOS            (u8)20    //mcu.ios
#define BLOCK_ID_MCU_Z              (u8)21    //mcu.z
//cri
#define BLOCK_ID_IOS                (u8)30    //ios
#define BLOCK_ID_KDVMPCRI_Z         (u8)31    //kdvmpcri.z
//2100a
#define BLOCK_ID_KDM2100A_IOS       (u8)40    //KDM2100A.ios
#define BLOCK_ID_KDMMEDIAGW_Z       (u8)41    //kdvmediagw.z

#define BLOCK_ID_MMP_Z              (u8)50    //mmp.z
//#define BLOCK_ID_KDMVS_Z          (u8)51    //kdmvs.z

#define BLOCK_ID_VASOS              (u8)53    //vasos
#define BLOCK_ID_VASCTRL_Z          (u8)54    //VasCtrl.z

#define BLOCK_ID_RPSERVER_EXE       (u8)60    //rpserver.exe

// rnu
#define BLOCK_ID_NRU_WIN32          (u8)70      // win32 -> nru.exe

// cui
#define BLOCK_ID_CUI_VXWORKS        (u8)71      // VxWorks -> cui
#define BLOCK_ID_CUI_LINUXS         (u8)72      // Linuxs -> cui
// pui
#define BLOCK_ID_PUI_VXWORKS        (u8)73      // VxWorks -> pui
#define BLOCK_ID_PUI_LINUXS         (u8)74      // Linuxs -> pui

// uas
#define BLOCK_ID_UAS_WIN32          (u8)75      // win32 -> uas.exe
#define BLOCK_ID_UAS_LINUXS         (u8)76      // Linuxs -> uas

// vtdu
#define BLOCK_ID_VTDU_VXWORKS       (u8)77      // VxWorks -> vtdu
#define BLOCK_ID_VTDU_LINUXS        (u8)78      // Linuxs -> vtdu

// cmu
#define BLOCK_ID_CMU_VXWORKS        (u8)80      // VxWorks -> cmu
#define BLOCK_ID_CMU_LINUXS         (u8)81      // Linuxs -> cmu
#define BLOCK_ID_CMU_WIN32          (u8)82      // Linuxs -> cmu.exe



// 传输内存块的类型
#define TNM_TRANS_FILE                      (u8)0x1     // 传输文件
#define TNM_TRANS_BUFFER                    (u8)0x2     // 传输一块内存

// 缺省内存块切片大小
#define TNM_DEF_SLICE_SIZE                  (16 * 1024) // 16K字节

// 内存块基本信息
typedef struct tagTnmBlockInfo
{
    tagTnmBlockInfo()
    {
        memset(this, 0, sizeof(tagTnmBlockInfo));
    }

    /*========================================================================
    函 数 名：GetSliceNum
    功    能：得到分片的数量
    参    数：
    返 回 值：
    --------------------------------------------------------------------------
    修改记录：
    日      期  版本    修改人  修改内容
    2006/03/21  3.9     李洪强  创建
    ========================================================================*/
    u32 GetSliceNum() const;
    
    u32 dwTotalSize;            // 内存块全部大小
    u32 dwSliceSize;            // 数据切片的大小, 纯有效数据

    u8  byTransType;        // 内存块类型，是buffer,还是文件
                            // TNM_TRANS_FILE
                            // TNM_TRANS_BUFFER
    u8  byBlockId;          // 标识文件或内存的内容，如软件版本文件的宏定义
    u8  byReserve1;         // 2006/04/05 李洪强 四字节对齐
    u8  byReserve2;

    void Print(BOOL32 bNetToHost = FALSE)
    {
        if (bNetToHost) 
        {
            OspPrintf(TRUE, FALSE, "TrsType: %d; BlkId: %d; TalSize: 0x%x;\n", 
                byTransType, byBlockId, ntohl(dwTotalSize));
            OspPrintf(TRUE, FALSE, "\tSlicSize: 0x%x; SlicNum: 0x%x;\n",
                ntohl(dwSliceSize), ntohl(GetSliceNum()));
        }
        else
        {
            OspPrintf(TRUE, FALSE, "TrsType: %d; BlkId: %d; TalSize: 0x%lx;\n", 
                byTransType, byBlockId, dwTotalSize);
            OspPrintf(TRUE, FALSE, "\tSlicSize: 0x%lx; SlicNum: 0x%lx;\n",
                dwSliceSize, GetSliceNum());
        }
    }
    // 2006/04/06 李洪强 增加字节序转化的接口
    void NetToHost();
    void HostToNet();
}PACKED TTnmBlockInfo;
/*========================================================================
函 数 名：NetToHost/HostToNet
功    能：字节序转化接口，网络收发消息时调用，前端上层业务在使用回调函数时要首先HostToNet转为网络序
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/04/06  3.9     李洪强  创建
========================================================================*/
inline void TTnmBlockInfo::NetToHost()
{
    dwTotalSize = ntohl(dwTotalSize);
    dwSliceSize = ntohl(dwSliceSize);
}

inline void TTnmBlockInfo::HostToNet()
{
    dwTotalSize = htonl(dwTotalSize);
    dwSliceSize = htonl(dwSliceSize);
}
/*========================================================================
函 数 名：GetSliceNum
功    能：得到分片的数量
参    数：
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/21  3.9     李洪强  创建
========================================================================*/
inline u32 TTnmBlockInfo::GetSliceNum() const
{
    assert(dwSliceSize > 0);
    if (dwSliceSize < 1) 
    {
        return 0;
    }
    u32 dwSliceNum = dwTotalSize/dwSliceSize;

    if (dwTotalSize%dwSliceSize != 0)
    {
        dwSliceNum++;
    }
    return dwSliceNum;
}

/********************************************************************/
/*                         配置信息消息定义                         */
/********************************************************************/
#define KDMCONF_BGN        5001
#define KDMCONF_END        KDMCONF_BGN + 999


//代理连接上管理端的通知
#define NODE_CONNECTED_NOTIFY           KDMCONF_BGN
//代理与管理端断开的通知
#define NODE_DISCONNECTED_NOTIFY        KDMCONF_BGN + 1

//3as与网管建链成功通知消息
//消息体：TnmLinkInfo
#define NODE_CONNECTED_NMS_NOTIFY       KDMCONF_BGN + 2

/********************************************************************/
/* 1.   配置管理：配置参数的获取和设置                              */
/********************************************************************/
/*====================================================================
1.1.    公共配置参数
====================================================================*/

// 软件版本升级
// 通知软件内容: TTnmVersionInfo
// nms -> agent(req):  TTnmVersionInfo
// nms <- agent(ack):  
// nms <- agent(nack): 
#define NODE_SOFTWARE_UPGRADE           KDMCONF_BGN + 3

// 发送软件版本信息
// 通知软件内容：TTnmBlockInfo
// nms -> agent(req): TTnmBlockInfo
// nms <- agent(ack): 
#define NODE_SOFTWARE_INFO              KDMCONF_BGN + 4
// 软件版本切分包
// nms -> agent(notify): u8(byBlockID) + u32(分片序号) + u32(有效数据长度) + 有效数据
// nms <- agent(ack): u8(byBlockID) + u32(分片序号)
// nms <- agent(nack):
// nms <- agent(finish):u8(byBlockID) + u32(分片序号)
#define NODE_SOFTWARE_PACKAGE           KDMCONF_BGN + 5
//所有软件版本上传完毕
//nms->agent:
#define NODE_SOFTWARE_UPLOAD_COMPLETE   KDMCONF_BGN + 6
        
//系统硬件复位
//查询结果或设置内容: 无
#define NODE_SYSTEM_RESET               KDMCONF_BGN + 7

//系统自检
//查询结果或设置内容: 无
#define NODE_SELF_TEST                  KDMCONF_BGN + 8

//重注册消息
//查询结果或设置内容: 无
#define NODE_REPEAT_REGISTER            KDMCONF_BGN + 10

//配置文件
//查询结果或设置内容: u8[65535]文件内容
#define NODE_SYSPUBCFG_FILE             KDMCONF_BGN + 11

//公共配置参数
//查询结果或设置内容: TTnmPubCfgInfo
#define NODE_SYSPUBCFGINFO              KDMCONF_BGN + 15

//平台设备系统参数：TTnmServerSysCfgInfo
#define NODE_SERVER_SYSCFGINFO          KDMCONF_BGN + 16

// 1.2. 前端设备配置参数
// 1.2.1.   系统参数
//查询结果或设置内容: TTnmPuCapSet
#define NODE_PU_CAPSETINFO              KDMCONF_BGN + 17

// 1.2.1.   前端设备和DEC5系统参数
//查询结果或设置内容: TTnmPuSysCfgInfo
#define NODE_PU_SYSCFGINFO             KDMCONF_BGN + 20
//查询结果或设置前端存储设备(2417/2428)的内容: TTnmPuStoreSysCfgInfo
#define NODE_PU_STORE_SYSCFGINFO       KDMCONF_BGN + 22

// 1.2.2.   PPPOE参数(pu & Dec5)
//查询结果或设置内容: TTnmPPPOECfgInfo
#define NODE_PU_PPPOECFGINFO           KDMCONF_BGN + 25

// 1.2.3.   视频编码参数表（按通道）
//查询结果[整个表]或设置内容[修改的条目]: u8(视频编码通道个数) + TTnmVideoEncCfgInfo数组
#define NODE_PU_VIDEOENCINFO            KDMCONF_BGN + 30

// 1.2.4.   视频解码参数表（按通道）(pu & dec5)
//查询结果[整个表]或设置内容[修改的条目]: u8(视频解码通道个数) + TTnmVideoDecCfgInfo数组
#define NODE_PU_VIDEODECINFO            KDMCONF_BGN + 35

// 1.2.5.   音频编解码参数表（按通道）
//  音频编码参数表(pu & dec5)
//查询结果[整个表]或设置内容[修改的条目]: u8(音频编码通道个数) + TTnmAudioEncCfgInfo数组
#define NODE_PU_AUDIO_ENC_INFO          KDMCONF_BGN + 40
//音频解码参数表
//查询结果[整个表]或设置内容[修改的条目]: u8(音频解码通道个数) + TTnmAudioDecCfgInfo数组
#define NODE_PU_AUDIO_DEC_INFO          KDMCONF_BGN + 41

// 1.2.6.   视频源输入端口参数表（按端口）
//查询结果: u8(视频源输入端口个数) + TTnmVideoInputCfgInfo数组 + u8(支持的摄像头个数) + TTnmCameraInfo数组
//设置内容: u8(视频源输入端口个数) + TTnmVideoInputCfgInfo数组
#define NODE_PU_VIDEOINPUTINFO          KDMCONF_BGN + 45

// 1.2.7.   并口参数表（按端口）
//查询结果[整个表]或设置内容[修改的条目]: u8(端口个数) + TTnmLPTCfgInfo数组
#define NODE_LPTCFGINFO                 KDMCONF_BGN + 50

// 1.2.8.   串口参数表（按串口）
//查询结果[整个表]或设置内容[修改的条目]: u8(串口个数) + TTnmSerialCfgInfo数组
#define NODE_SERIALCFGINFO              KDMCONF_BGN + 55

// 1.2.9.   前端存储参数表（按通道）
//查询结果[整个表]或设置内容[修改的条目]: u8(通道个数) + TTnmStorageCfgInfo数组
#define NODE_PU_STORAGECFGINFO          KDMCONF_BGN + 60

// 1.2.10  前端设备定时或每日录象策略
//查询结果[整个表]或设置内容[修改的条目]: u8(通道个数) + TTnmPuPeriodPolicy数组
#define NODE_PU_PERIOD_WOR_POLICY       KDMCONF_BGN + 62

// 1.3. 平台设备配置参数
// 1.3.1.   系统配置参数
//查询结果或设置内容: TTnmCmuCfgInfo
#define NODE_CMU_SYSCFGINFO             KDMCONF_BGN + 65

// 1.3.2.   网络配置参数
//查询结果或设置内容: TTnmCmuNetCfgInfo + u8(网卡个数) + TTnmEthernetCard数组 +
//                    u8(交换板个数) + TTnmSwitchBoard数组 +
//                    u8(交换板个数) + TTnmSubSwitchBoard数组
//#define NODE_CMU_NETCFGINFO             KDMCONF_BGN + 70


// 1.3.3.   键盘配置参数
//查询结果[整个表]或设置内容[修改的条目]: u8(键盘数) + TTnmKeyboardCfgInfo数组
#define NODE_CMU_KEYBOARDINFO           KDMCONF_BGN + 75

// 1.3.4.   NRU配置参数
//查询结果[整个表]或设置内容[修改的条目]: TTnmNruCfgInfo
#define NODE_CMU_NRUINFO                KDMCONF_BGN + 80

// 1.3.5.   多画面复合配置参数
//查询结果[整个表]或设置内容[修改的条目]: u8(VPU个数) + TTnmVpuCfgInfo数组
#define NODE_CMU_VPUINFO                KDMCONF_BGN + 85

// 1.3.6.   视频矩阵配置参数
//查询结果[整个表]或设置内容[修改的条目]: u8(视频矩阵个数) + TTnmMatrixCfgInfo数组
#define NODE_CMU_MATRIXINFO             KDMCONF_BGN + 90


// 1.4. 多画面复合器(VPU)配置参数
// 1.4.1.   系统配置参数
//查询结果或设置内容: TTnmVpuSysCfgInfo
#define NODE_VPU_SYSTEMINFO             KDMCONF_BGN + 95

// 1.5. 视频矩阵（VAS）板配置参数
// 1.5.1.   系统配置参数
//查询结果或设置内容: TTnmVasSysCfgInfo
#define NODE_VAS_SYSTEMINFO             KDMCONF_BGN + 100

// 1.6. 主控业务配置参数
// 1.6.1.   系统配置参数
//查询结果或设置内容: TTnmMainServSysCfgInfo
#define NODE_MP_SYSTEMINFO              KDMCONF_BGN + 105

//Laden配置参数
//查询结果或设置内容: TTnmLadenCfgInfo
#define NODE_LADEN_CFGINFO              KDMCONF_BGN + 107

// 1.7. CUI配置
//查询结果或设置内容: TTnmCuiCfgInfo
#define NODE_CUI_SYSTEMINFO             KDMCONF_BGN + 110

// 1.7.2.   PUI配置参数
//查询结果或设置内容: TTnmPuiCfgInfo
#define NODE_PUI_CFGINFO                KDMCONF_BGN + 115

// 1.8 VTDU配置参数
//查询结果或设置内容: TTnmVtduCfgInfo
#define NODE_VTDU_CFGINFO               KDMCONF_BGN + 117

// 1.9 UAS配置参数
//查询结果或设置内容: TTnmUasCfgInfo
#define NODE_UAS_CFGINFO                KDMCONF_BGN + 118

// 1.10 3AS配置参数
//查询结果或设置内容: TTnm3asCfgInfo
#define NODE_3AS_CFGINFO                KDMCONF_BGN + 119


/* 2. 性能管理：系统状态和性能的实时查询                            */
// 2.1. 公共性能参数
// 2.2 网管服务器性能参数  TTnmNmsPfmInfo
#define NODE_NMS_PFMINFO                KDMCONF_BGN + 120

// 2.3. 前端设备性能参数
//查询结果或设置内容: TTnmPuPfmInfo
#define NODE_PU_PFMINFO                 KDMCONF_BGN + 122

// 2.4.1. 平台设备基本性能参数
//查询结果或设置内容: TTnmCmuPfmInfo
#define NODE_CMU_PFMINFO                KDMCONF_BGN + 125

// 2.4.1.1 平台设备扩展性能参数
//查询结果或设置内容: TTnmCmuExtendablePfmInfo
#define NODE_CMU_EXTENDABLE_PFMINFO     KDMCONF_BGN + 126

// 2.4.1.2 平台设备扩展性能参数
//查询结果或设置内容: u16 分区个数+TTnmCmuDiskPfmInfo数组
#define NODE_CMU_DISK_PFMINFO           KDMCONF_BGN + 127

// 2.4.2 平台设备扩展性能参数 CU登录情况 
//查询结果或设置内容: u16 CU个数 + TTnmCuLogon数组
#define NODE_CMU_PFMINFO_CU_LIST        KDMCONF_BGN + 130
//nms->agt获取CU日志内容: u32(客户的用户Session的ID)
//nms<-agt通知CU日志内容: TTnmCuLogItem
#define NODE_CMU_PFMINFO_CU_LOG         KDMCONF_BGN + 131

// 2.4.3 查询平台上某个CU用户的视频浏览统计信息
#define NODE_CMU_CU_BROWSE_INFO         KDMCONF_BGN + 132

// 2.4.3 平台设备扩展性能参数 PU登录情况 
//查询结果或设置内容: u16 PU个数 + TTnmCmuPuInfo数组
#define NODE_CMU_PFMINFO_PU_LIST        KDMCONF_BGN + 133

// 2.4.4 获取平台各模块连接状态
// 查询结果或设置内容：u16 模块个数(TNM_MAX_CMU_MODULE_NUM = 128) ＋ TTnmCmuModuleState数组
#define NODE_CMU_MODULE_STATE           KDMCONF_BGN + 134

// 2.5. 平台设备VPU性能参数
//查询结果或设置内容: TTnmVpuPfmInfo
#define NODE_CMU_VPUPFMINFO             KDMCONF_BGN + 135
//查询结果或设置内容：u16(通道个数) + TTnmVpuRcvChnStatus数组
#define NODE_CMU_VPUPFMINFOEX             KDMCONF_BGN + 136


// 2.6. NRU性能参数
// 2.6.1 基本参数
//查询结果或设置内容: TTnmNruPfmInfo
#define NODE_NRU_PFMINFO                KDMCONF_BGN + 137
// 2.6.2 录像任务参数(扩展参数) u16(任务数) + TTnmNruRecTask
#define NODE_NRU_REC_TASK               KDMCONF_BGN + 138
// 2.6.3 放象任务参数(扩展参数) u16(任务数) + TTnmNruPlayTask
#define NODE_NRU_PLAY_TASK              KDMCONF_BGN + 139
// 2.6.4 下载任务参数(扩展参数) u16(任务数) + TTnmNruDownLoadTask
#define NODE_NRU_DOWNLOAD_TASK          KDMCONF_BGN + 140
// 2.6.5 预录任务参数(扩展参数) u16(任务数) + TTnmNruPreRecTask
#define NODE_NRU_PREREC_TASK            KDMCONF_BGN + 141
// 2.6.6 磁盘扩展参数  u16(分区数)+TTnmNruDiskPfmInfo数组
#define NODE_NRU_DISK_PFMINFO           KDMCONF_BGN + 142

// 2.7. 3AS性能参数
// 2.7.1 基本性能参数
//查询结果或设置内容: TTnm3asPfmInfo
#define NODE_3AS_PFMINFO                KDMCONF_BGN + 145
// 2.7.2 扩展性能参数
//查询结果或设置内容: u16(本消息的数组大小) + TTnmLogonInfo数组
#define NODE_3AS_PFMINFOEX              KDMCONF_BGN + 146

// 2.8. UAS性能参数
//查询结果或设置内容: TTnmUasPfmInfo
#define NODE_UAS_PFMINFO                KDMCONF_BGN + 150

// 2.9. CUI性能参数
// 基本性能参数  TTnmCuiPfmInfo
#define NODE_CUI_PFMINFO                KDMCONF_BGN + 155
// 扩展性能参数  u16(本消息的任务数组大小) + TTnmCuiPfmInfoEx
#define NODE_CUI_PFMINFOEX              KDMCONF_BGN + 160

// 2.10. PUI性能参数
// 基本性能参数  TTnmPuiPfmInfo
#define NODE_PUI_PFMINFO                KDMCONF_BGN + 165
// 扩展性能参数  u16(本消息的PU连接数组大小) + TTnmPuiPfmInfoEx
#define NODE_PUI_PFMINFOEX              KDMCONF_BGN + 170

// 2.11 VDTU性能参数
// 基本性能参数 TTnmVtduPfmInfo
#define NODE_VTDU_PFMINFO               KDMCONF_BGN + 175
// 扩展性能参数 u16(本消息的任务数组大小）+ TTnmVtduTask数组
#define NODE_VTDU_PFMINFOEX             KDMCONF_BGN + 180

//2.12 DEC5基本性能参数
//基本性能参数 TTnmDec5PfmInfo
#define NODE_DEC5_PFM_INFO              KDMCONF_BGN + 185


//landen基本性能参数
//查询结果或设置内容: TTnmLadenPfmInfo
#define NODE_LADEN_PFMINFO              KDMCONF_BGN + 190

//landen扩展性能参数 - PUI列表信息
//查询结果或设置内容: u16(PUI个数) + TTnmLadenPuiInfo数组
#define NODE_LADEN_PFMINFO_PUI_LIST     KDMCONF_BGN + 191

//landen扩展性能参数 - CUI列表信息
//查询结果或设置内容: u16(CUI个数) + TTnmLadenCuiInfo数组
#define NODE_LADEN_PFMINFO_CUI_LIST     KDMCONF_BGN + 192

// 3.1 设备拓朴信息
// agent -> 3as(req)查询信息的BUF内容为空
// 当为无效ID时查找根节点下的所有设备
// agent <- 3as(notify):
// 拓朴表: u32(全部节点的条目数) + u32(当前消息第一个节点的序号)
//          + u16(本条消息的条目数) + TTnmDevTopo数组
// agent <- 3as(finish);无
#define NODE_3AS_TOPO_INFO              KDMCONF_BGN + 200
// 3.2 增加拓朴设备
// agent <- 3as(notify)拓朴表: u16(条目数) + TTnmDevTopo
#define NODE_3AS_TOPO_ADD_DEV           KDMCONF_BGN + 205
// 3.3 删除拓朴设备
// agent <- 3as(notify)拓朴表: u16(条目数) + TTnmDevTopo
#define NODE_3AS_TOPO_DEL_DEV           KDMCONF_BGN + 210
// 3.3 更新拓朴设备
// agent <- 3as(notify)拓朴表: u16(条目数) + TTnmDevTopo
#define NODE_3AS_TOPO_UPDATE_DEV        KDMCONF_BGN + 215
// 批次消息发送完成时发送本消息，前面填消息号，后面填子消息类型
// agent <- 前端(notify):u16(NODE消息号) + u16(子消息类型SUBEV_TYPE_FINISH)
#define NODE_EVSTATUS_NOTIFY            KDMCONF_BGN + 220

/********************************************************************/
/*                        设  备  告  警                            */
/********************************************************************/
#define KDMTRAP_BGN             EV_DEVAGT_NMS_BGN + 1000
#define KDMTRAP_END             KDMTRAP_BGN + 999
#define KDMTRAP_MAX_IPADDR_LEN  32+1
#define KDMTRAP_MAX_DEV_NO      32+1
// 告警信息上报
// 消息体:TTnmDevAlarm + 具体的消息内容。根据不同种类的告警码，告警的内容
//      有所不同，评述如下
//  1. TNM_ALARM_CODE_DEV_OFFLINE  此告警由NMS产生，前端设备不关心
//      u8(告警状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  2.  TNM_ALARM_CODE_DEV_ONLINE
//  3.  TNM_ALARM_CODE_RECV_NOBITSTREAM
//      u8(通道号) + u8(通道的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  4.  TNM_ALARM_CODE_LOSEPACKET
//      u8(通道号) + u8(通道的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  5.  TNM_ALARM_CODE_MAP_STATUS
//      u8(MAP号) + u8(MAP的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  6.  TNM_ALARM_CODE_MOTIONDETECT_STATUS
//      u8(视频源号) + u8(视频源的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  7.  TNM_ALARM_CODE_LPT_STATUS
//      u8(通道号) + u8(通道的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  8.  TNM_ALARM_CODE_POWER_STATUS
//      u8(那个电源) + u8(电源的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  9.  TNM_ALARM_CODE_CPU_STATUS
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  10. TNM_ALARM_CODE_MEMORY_STATUS
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  11. TNM_ALARM_CODE_DISKFULL_STATUS
//      u8(状态：告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  12. TNM_ALARM_CODE_FAN_STATUS
//      u8(那个风扇) + u8(风扇状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  13. TNM_ALARM_CODE_ETHCARD_RESTORE
//      u8(以太网ID) + u8(以太网的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  14. TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS
//      u8(视频源号) + u8(视频源的状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  15. TNM_ALARM_CODE_LED_STATUS
//      u8(面板灯个数) + u8数组(enum TnmLedStatus)
//  16. TNM_ALARM_CODE_SAME_REG_ID  此告警由NMS产生，前端设备不关心
//      TTnmAgentRegReq + u8(告警: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  17. TNM_ALARM_CODE_REG_CMU  PU或其他模块注册CMU失败告警
//      u8(告警(TNM_STATUS_ALARM,注册失败) 恢复(TNM_STATUS_NORMAL，注册成功))
//  18. TNM_ALARM_CODE_DISKDETECT_ERROR    此告警由设备产生 设备检测无磁盘或没有磁盘可工作告警
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL))
//  19. TNM_ALARM_CODE_CMU_CU_ALARM_LOSEPACKET        此告警是Cu通过CMU发送的CU业务告警
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 告警描述
//  20. TNM_ALARM_CODE_CMU_CU_ALARM_REQUISTION        此告警是Cu通过CMU发送的CU业务告警
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 告警描述
//  21  TNM_ALARM_CU_OVERLOAD  登录到CMU上的CU数超过阀值
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 告警描述
//  22  TNM_ALARM_PU_OVERLOAD  登录到CMU上的PU数超过阀值
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 告警描述
//  23  TNM_ALARM_VTDU_OVERLOAD  VTDU的转发路数超过阀值
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 告警描述
//  24  TNM_ALARM_CODE_DISCONNECT_VTDU  VTDU未连接上
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 
//      s8 achSlaveIp[KDMTRAP_MAX_IPADDR_LEN](Vtdu Ip) +
//      s8 achSlaveId[KDMTRAP_MAX_DEV_NO](Vtdu Id)
//  25  TNM_ALARM_CODE_DISCONNECT_NRU  NRU未连接上
//      u8(状态: 告警(TNM_STATUS_ALARM) 恢复(TNM_STATUS_NORMAL)) + 
//      s8 achSlaveIp[KDMTRAP_MAX_IPADDR_LEN](Nru Ip) +
//      s8 achSlaveId[KDMTRAP_MAX_DEV_NO](Nru Id)

#define TRAP_TNM_DEV_ALARM                  KDMTRAP_BGN

//设备写完成Trap消息, 设备版本升级时写入设备完成后发给网管的消息
//      消息体：u8(0:写失败  1:成功) + u8(0:不重启  1:重启)
#define TRAP_SOFTWARE_COMPLETE              KDMTRAP_BGN+1

//网管配置冲突
#define TRAP_MASTER_NMS_EXIT_ALREADY        KDMTRAP_BGN+5

//最大告警描述信息长度
#define MAX_DESC_INFO_LEN  256

typedef struct tagAlarmTrapInfo
{
	tagAlarmTrapInfo()
	{	
		dwAgentID = 0;
		byParam = 0;
		byState = 0;
		wDescInfoLen = 0;
		wSubDevId = 0;
		memset(achDescInfo, 0, sizeof(achDescInfo));
	}
	void NetToHost();
	void HostToNet();
	TTnmDevAlarm tDevAlarm;   	//流水号、AlarmCode
	u32 dwAgentID;  			//用于查找到设备SN及相关设备信息
	u16 wSubDevId;
	u8  byParam;    			//并口号或电源号或电扇号或网卡号或视频源号或硬盘分区编号或录像通道号
	u8  byState;    			//告警状态
	u16 wDescInfoLen;			//告警描述信息长度
	s8 achDescInfo[MAX_DESC_INFO_LEN + 4];       //告警描述信息
}PACKED TAlarmTrapInfo;

inline void TAlarmTrapInfo::NetToHost()
{
	tDevAlarm.NetToHost();
	wDescInfoLen = ntohs(wDescInfoLen);
	dwAgentID = ntohl(dwAgentID);
	wSubDevId = ntohs(wSubDevId);
}

inline void TAlarmTrapInfo::HostToNet()
{
	tDevAlarm.HostToNet();
	wDescInfoLen = htons(wDescInfoLen);
	dwAgentID = htonl(dwAgentID);
	wSubDevId = htons(wSubDevId);
}


/*========================================================================
函 数 名：GetAlarmCodeDesc
功    能：调试接口：得到告警码的描述定义
参    数：u32 dwAlarmCode               [in]要得到描述的告警码
返 回 值：
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2006/03/30  3.9     李洪强  创建
========================================================================*/
inline const s8* GetAlarmCodeDesc(u32 dwAlarmCode)
{
    switch(dwAlarmCode)
    {
    case TNM_ALARM_CODE_DEV_OFFLINE:            return "DEV_OFFLINE";
    case TNM_ALARM_CODE_LOSEVIDEOSRC_STATUS:    return "LOSEVIDEOSRC_STATUS";
    case TNM_ALARM_CODE_RECV_NOBITSTREAM:       return "RECV_NOBITSTREAM";
    case TNM_ALARM_CODE_MAP_STATUS:             return "MAP_STATUS";
    case TNM_ALARM_CODE_CPU_STATUS:             return "CPU_STATUS";
    case TNM_ALARM_CODE_MEMORY_STATUS:          return "MEMORY_STATUS";
    case TNM_ALARM_CODE_DISKFULL_STATUS:        return "DISKFULL_STATUS";
    case TNM_ALARM_CODE_LOSEPACKET:             return "LOSEPACKET";
    case TNM_ALARM_CODE_LPT_STATUS:             return "LPT_STATUS";
    case TNM_ALARM_CODE_POWER_STATUS:           return "POWER_STATUS";
    case TNM_ALARM_CODE_FAN_STATUS:             return "FAN_STATUS";
    case TNM_ALARM_CODE_MOTIONDETECT_STATUS:    return "MOTIONDETECT_STATUS";
    case TNM_ALARM_CODE_ETHCARD_RESTORE:        return "ETHCARD_RESTORE";
    case TNM_ALARM_CODE_LED_STATUS:             return "LED_STATUS";
    case TNM_ALARM_CODE_SAME_REG_ID:            return "SAME_REG_ID";
    case TNM_ALARM_CODE_REG_CMU:                return "REG_CMU";
    case TNM_ALARM_CODE_DISKDETECT_ERROR:       return "DISK_DETECT";
    case TNM_ALARM_CU_OVERLOAD:                 return "CU_OVERLOAD";
    case TNM_ALARM_PU_OVERLOAD:                 return "PU_OVERLOAD";
    case TNM_ALARM_VTDU_OVERLOAD:               return "VTDU_OVERLOAD";
    case TNM_ALARM_CODE_NODISK_ERROR:			   return "NODISK_ERROR";
	case TNM_ALARM_CODE_DISK_ERROR:				   return "DISK_ERROR";
	case TNM_ALARM_CODE_DISK_NOFORMAT:			   return "DISK_NOFORMAT";
	case TNM_ALARM_CODE_STOPREC_LOGERR:			   return "STOPREC_LOGERR";
	case TNM_ALARM_CODE_STOPREC_DISKERR:        return "STOPREC_DISKERR";
    case TNM_ALARM_CODE_DISCONNECT_3AS:         return "DISCONNECT_3AS";
    case TNM_ALARM_CODE_DISCONNECT_UAS:         return "DISCONNECT_UAS";
    case TNM_ALARM_CODE_DISCONNECT_PUI:         return "DISCONNECT_PUI";
    case TNM_ALARM_CODE_DISCONNECT_CUI:         return "DISCONNECT_CUI";
    case TNM_ALARM_CODE_DISCONNECT_NRU:         return "DISCONNECT_NRU";
    case TNM_ALARM_CODE_DISCONNECT_VTDU:        return "DISCONNECT_VTDU";
    case TNM_ALARM_CODE_DISCONNECT_CMU:         return "DISCONNECT_CMU";
    case TNM_ALARM_CODE_DISCONNECT_DATABASE:    return "DISCONNECT_DATABASE";
    case TNM_ALARM_CODE_DISCONNECT_USBKEY:      return "DISCONNECT_USBKEY";
    case TNM_ALARM_CODE_DISK_PART_R_ONLY:       return "DISK_PART_R_ONLY";
	case TNM_ALARM_CODE_DISK_FS_ERROR:			return "DISK_FS_ERROR";
	case TNM_ALARM_CODE_DISK_OFFLINE:			return "DISK_OFFLINE";
	case TNM_ALARM_CODE_STORAGE_ABNORMAL:		return "STORAGE_ABNORMAL";

	case TNM_ALARM_CODE_PLATFORM_IN_REPLACE_MODE: return "PLATFORM_IN_REPLACE_MODE";

	case TNM_ALARM_CODE_RAID_DISK_OFFLINE:		return "RAID_DISK_OFFLINE";
	case TNM_ALARM_CODE_RAID_DISK_HEALTH_BAD:	return "RAID_DISK_HEALTH_BAD";
	case TNM_ALARM_CODE_RAID_DISKGROUP_STATUS_ABNORMAL:	return "RAID_DISKGROUP_STATUS_ABNORMAL";
	case TNM_ALARM_CODE_RAID_ISCSI_DROPPED:		return "RAID_ISCSI_DROPPED";
	case TNM_ALARM_CODE_RAID_ABNORMAL:			return "RAID_ABNORMAL";
	case TNM_ALARM_CODE_DETECT_NULL_IP_NIC:     return "NULL_IP_NIC";
	case TNM_ALARM_CODE_DISCONNECT_SUPER_CMU:	return "TNM_ALARM_CODE_DISCONNECT_SUPER_CMU";
	case TNM_ALARM_CODE_DISCONNECT_SUB_CMU:		return "TNM_ALARM_CODE_DISCONNECT_SUB_CMU";
	case TNM_ALARM_CODE_DISCONNECT_BCMU_CMU:	return "TNM_ALARM_CODE_DISCONNECT_BCMU_CMU";
	case TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD:	return "TNM_ALARM_CODE_PLATFORM_DISK_NUM_OVERLOAD";

    default:
    	OspPrintf(TRUE,FALSE,"unknow alarmcode %lu\n",dwAlarmCode);
        break;
    }
    return "TNM_ALARM_CODE_INVALID";
}

#ifdef WIN32
#pragma pack(pop)
#endif //WIN32
/**
 * @}
 */
#endif  //!_SNMP_AGENTADP_H_


