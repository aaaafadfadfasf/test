//////////////////////////////////////////////////////////////////////////
// ipcdefine.h 
#ifndef IPCDEFINE_H_
#define IPCDEFINE_H_

/* 错误码 */
#define IPC_ERR_SUCCESS                     (0)    // 成功返回
#define IPC_ERR_INIT_OSP                    (-1)   // OSP通信模块初始化失败
#define IPC_ERR_GET_LOC_IP                  (-2)   // 获取本机IP失败
#define IPC_ERR_CREATE_VSIP                 (-3)   // 协议栈管理对象创建失败
#define IPC_ERR_INIT_VSIP                   (-4)   // 协议栈对象初始化失败
#define IPC_ERR_GET_ADAPTER_INFO            (-5)   // 获取本机网卡信息失败
#define IPC_ERR_INIT_DATA_SWITCH            (-6)   // 码流转发模块初始化失败
#define IPC_ERR_INVALID_PU_HANDLE           (-10)  // 无效的前端句柄
#define IPC_ERR_GET_PU_CTRL                 (-11)  // 获取前端控制对象失败
#define IPC_ERR_INIT_PU_CTRL                (-12)  // 初始化设备控制失败(连接时)
#define IPC_ERR_INVALID_CON_LOC_IP          (-13)  // 连接成功后，协议栈返回的本地IP为0
#define IPC_ERR_GET_ORIGIN_CAP              (-14)  // 获取原始能力集失败
#define IPC_ERR_GET_EXT_CAP                 (-15)  // 获取扩展能力集失败
#define IPC_ERR_GET_ADAPTER_CAP             (-16)  // 获取适配器能力集失败
#define IPC_ERR_GET_PU_BASE_INFO            (-17)  // 获取前端基本信息失败
#define IPC_ERR_INVALID_USER_PWD            (-18)  // 连接时的用户名密码错误
#define IPC_ERR_GET_PLAYER                  (-30)  // 获取播放器对象失败
#define IPC_ERR_INVALID_VD_SRC_ID           (-31)  // 无效的视频源通道号
#define IPC_ERR_INIT_DECODER                (-32)  // 解码器初始化失败
#define IPC_ERR_START_DECODE                (-33)  // 视音频解码失败
#define IPC_ERR_GET_VD_ENC_PT_CFG           (-34)  // 获取视频编码视频源端口参数失败
#define IPC_ERR_INVALID_PLAY_WND            (-35)  // 无效的播放窗口句柄
#define IPC_ERR_GET_AD_DEC_NET_CFG          (-36)  // 获取音频解码网络参数失败
#define IPC_ERR_START_DEC_RECV_EX           (-37)  // 扩展音视频的码流接收请求失败
#define IPC_ERR_INIT_ENCODER                (-38)  // 编码器初始化失败
#define IPC_ERR_AD_CALL_UNSUPPORTED         (-39)  // 前端不支持语音呼叫
#define IPC_ERR_PU_STOP_AD_RECV             (-40)  // 停止前端音频接收失败
#define IPC_ERR_START_SEND_AUDIO            (-41)  // 音频编码发送失败
#define IPC_ERR_STOP_ENCODE                 (-42)  // 编码器停止音频编码失败
#define IPC_ERR_REC_WITH_NO_VD_PLAY         (-43)  // 在没有视频播放的情况下进行本地录像
#define IPC_ERR_INIT_LOCAL_RECORDER         (-44)  // 本地录像机初始化失败
#define IPC_ERR_START_LOCAL_REC             (-45)  // 开始本地录像失败
#define IPC_ERR_STOP_LOCAL_REC              (-46)  // 停止本地录像失败
#define IPC_ERR_SHOT_WITH_NO_VD_PLAY        (-47)  // 在没有视频播放的情况下进行本地抓拍
#define IPC_ERR_LOCAL_SNAPSHOT              (-48)  // 本地抓拍失败
#define IPC_ERR_LOCAL_RECV_PU_REC           (-49)  // 本地接收前端录像失败
#define IPC_ERR_INVALID_REC_FILE_NAME       (-50)  // 本地码流文件名为空
#define IPC_ERR_LOCAL_OPEN_FILE             (-51)  // 本地解码器打开文件失败
#define IPC_ERR_LOCAL_PLAY_FILE             (-52)  // 本地解码器播放文件失败
#define IPC_ERR_UPLOAD_TITLE_PIC            (-53)  // 上传台标文件失败
#define IPC_ERR_ADD_VIDEO_SEND_DEST         (-54)  // 前端编码器能力不够
#define IPC_ERR_STREAM_EXCEED_LIMIT         (-55)  // 直传路数不够(视频浏览受限)
#define IPC_ERR_PU_OVER_CAP                 (-56)  // 前端能力不支持 
#define IPC_ERR_CAMCTRL_UNSUPPORTED         (-60)  // 前端设备不支持此摄像头协议
#define IPC_ERR_SEND_CAMCTRL                (-61)  // 摄像头控制命令发送失败
#define IPC_ERR_PARSE_CAM_PROTOCOL          (-62)  // 摄像头协议解析失败
#define IPC_ERR_INVALID_PORT_NUM            (-63)  // 无效的端口号或已被其他应用占用
#define IPC_ERR_GET_PU_PARAM                (-80)  // 获取前端参数配置失败
#define IPC_ERR_GET_PU_PARAM_EX             (-81)  // 获取透明通道参数配置失败
#define IPC_ERR_INVALID_PARAM               (-82)  // 无效的配置参数
#define IPC_ERR_SET_PU_PARAM                (-83)  // 设置前端参数配置失败
#define IPC_ERR_SET_PU_PARAM_EX             (-84)  // 设置透明通道参数配置失败
#define IPC_ERR_SYNC_PU_TIME                (-85)  // 同步前端设备时间失败
#define IPC_ERR_REBOOT_PU                   (-86)  // 前端设备重启失败
#define IPC_ERR_RESET_DEFAULTS              (-87)  // 前端设备恢复出厂失败
#define IPC_ERR_LOAD_DEFAULT_CFG            (-88)  // 恢复默认设置失败(主要用于图像复位操作)
#define IPC_ERR_START_PU_REC                (-89)  // 开始前端录像失败
#define IPC_ERR_STOP_PU_REC                 (-90)  // 停止前端录像失败
#define IPC_ERR_PU_SNAPSHOT                 (-91)  // 前端抓拍失败
#define IPC_ERR_PU_SNAPSHOT_TIME_OUT        (-92)  // 前端抓拍超时
#define IPC_ERR_SEARCH_PU_PIC               (-93)  // 前端搜索图片失败
#define IPC_ERR_DOWNLOAD_PU_PIC             (-94)  // 下载前端图片失败
#define IPC_ERR_DELETE_PU_PIC               (-95)  // 前端删除图片失败
#define IPC_ERR_SEARCH_PU_REC               (-96)  // 前端搜索录像文件失败
#define IPC_ERR_INVALID_PU_REC_INFO         (-97)  // 指定的前端录像信息为空
#define IPC_ERR_START_PLAY_PU_REC           (-98)  // 前端播放录像文件失败
#define IPC_ERR_PU_EXPORT_REC_FILE          (-99)  // 前端导出录像文件失败
#define IPC_ERR_PU_STOP_EXPORT_REC_FILE     (-100) // 前端停止导出录像文件失败
#define IPC_ERR_PU_DELETE_REC_FILE          (-101) // 前端删除录像文件失败
#define IPC_ERR_PU_REC_PLAY_OPERATION       (-102) // 前端录像文件回放操作失败
#define IPC_ERR_PU_REC_SEEK_LOCATION        (-103) // 前端录像播放拖动操作失败
#define IPC_ERR_ADD_DATA_SWITCH             (-104) // 增加码流按源转发失败
#define IPC_ERR_REMOVE_ALL_DATA_SWITCH      (-105) // 删除指定源的按源转发失败
#define IPC_ERR_REMOVE_DATA_SWITCH          (-106) // 删除指定目的的按源转发失败

/* 一般常量 */
#define MAX_STR_LEN                  32   // 最大各类名称长度(32位)
#define MAX_STR_LEN_64               64   // 最大各类名称长度(64位)
#define MAX_VER_STRING_LEN           64   // 最大版本号、密码长度
#define MAX_STRING_OSD_LEN           64   // 最大字幕内容长度
#define MAX_USER_NUMBER             256   // 最大拨号用户名/密码长度
#define MAX_SEPA_LEN                  8   // 最大分隔符长度
#define MAX_NAMERULE_NUM              4   // 最大命名长度
#define MAX_PATH_LENGTH             256   // 最大文件路径长度
#define MAX_FILTERADDR_NUM		      5	  // 最大过滤地址个数
#define MAX_VIDEOIN_PORT_NUMBER      32   // 最大视频输入端口数
#define MAX_VIDEOIN_PORT_NM_LEN       8   // 最大视频源端口名长度
#define MAX_FILE_STREAM_NUM           3   // 最大回放文件流数目
#define MAX_MOTION_DETECT            16   // 移动侦测最多数量
#define SM_DATA_BUFSIZE            1024   // 图片内容字节长度
#define MAX_TM_PRECISION              8   // 录像时间精度
#define MAX_REC_FILE_NUM             20   // 最大录象文件数目
#define MAX_PIC_FILE_NUM             20   // 最大抓拍文件数目
#define MAX_DISK_NUM                 16   // 最大磁盘分区数目
#define MAX_CHANNEL_NUM               4   // 最大通道数目
#define MAX_VIDCHAN_NUM		         16	  // 最大编解码通道个数
#define MAX_FILE_NAME_LENGTH         64   // 最大文件长度
#define MAX_FILE_PATH_LENGTH         64   // 最大文件路径长度
#define MAX_DISK_PARTITION_NAME      64   // 分区名大小
#define MAX_CONNECTTED_PU			 16   // 最多同时放像的PU数量
#define MAX_RECIPROCAL_LIST_SIZE	 30   // 支持的快门参数列表
#define MAX_DAILYTIME_NUM             6   // 每天最大时间段个数
#define MAX_WEEKLYDAY_NUM             7   // 每周天数

/* 视频数据格式 */
#define IPCSDK_VIDEO_TYPE_NONE					0x00
#define IPCSDK_VIDEO_TYPE_SN4					0x01
#define IPCSDK_VIDEO_TYPE_MPEG4					0x02
#define IPCSDK_VIDEO_TYPE_H261					0x03
#define IPCSDK_VIDEO_TYPE_H263					0x04
#define IPCSDK_VIDEO_TYPE_H264					0x05

/* 音频数据格式 */
#define IPCSDK_AUDIO_TYPE_NONE					0x00
#define IPCSDK_AUDIO_TYPE_PCMA					0x01
#define IPCSDK_AUDIO_TYPE_PCMU					0x02
#define IPCSDK_AUDIO_TYPE_GSM					0x03
#define IPCSDK_AUDIO_TYPE_G729					0x04
#define IPCSDK_AUDIO_TYPE_ADPCM					0x05
#define IPCSDK_AUDIO_TYPE_G7221C				0x06
#define IPCSDK_AUDIO_TYPE_G722                  0x07
#define IPCSDK_AUDIO_TYPE_AACLC                 0x08

/* 图象分辨率 */
#define IPCSDK_VIDEO_RESOLUTION_NONE			0x00
#define IPCSDK_VIDEO_RESOLUTION_AUTO			0x01
#define IPCSDK_VIDEO_RESOLUTION_QCIF			0x02
#define IPCSDK_VIDEO_RESOLUTION_CIF			    0x03
#define IPCSDK_VIDEO_RESOLUTION_2CIF			0x04
#define IPCSDK_VIDEO_RESOLUTION_4CIF			0x05
#define IPCSDK_VIDEO_RESOLUTION_QQCIF			0x06
#define IPCSDK_VIDEO_RESOLUTION_VGA			    0x07
#define IPCSDK_VIDEO_RESOLUTION_QVGA			0x08
#define IPCSDK_VIDEO_RESOLUTION_720P			0x09
#define IPCSDK_VIDEO_RESOLUTION_1080P			0x0A
#define IPCSDK_VIDEO_RESOLUTION_QXGA            0x0B

/* 有效码率 */
#define IPCSDK_BITRATE_128						0x01
#define IPCSDK_BITRATE_256						0x02
#define IPCSDK_BITRATE_512						0x03
#define IPCSDK_BITRATE_768						0x04
#define IPCSDK_BITRATE_896						0x05
#define IPCSDK_BITRATE_1024						0x06
#define IPCSDK_BITRATE_2048						0x07
#define IPCSDK_BITRATE_3072						0x08
#define IPCSDK_BITRATE_4096						0x09
#define IPCSDK_BITRATE_8192						0x0A

/* 图像质量 */
#define IPCSDK_VIDEO_QUALITY_PRIOR				0x01 // 质量优先
#define IPCSDK_VIDEO_SPEED_PRIOR				0x02 // 速度优先

/* 图片编码类型 */
#define IPCSDK_PIC_ENC_JPEG                        1 // jpeg格式
#define IPCSDK_PIC_ENC_BMP                         2 // bmp格式(暂不支持)

/* 分区方式 */
#define IPCSDK_SINGLE_SUBAREA			        0x01 // 单分区
#define IPCSDK_MULTI_SUBAREA			        0x02 // 多分区

/* 音频输入类型 */
#define IPCSDK_AUDIO_INPUTTYPE_LINEIN			0x00
#define IPCSDK_AUDIO_INPUTTYPE_MIC				0x01

/* 录象文件/事件类型/图片事件 */
#define IPCSDK_QUIEY_ALL_EVENT					0    // 查询所有事件
#define IPCSDK_TYPE_PIN_ALARM					1    // 并口告警录象
#define IPCSDK_TYPE_MOTION_DETECT_ALARM			2    // 移动侦测告警录象
#define IPCSDK_TYPE_MANUAL						3    // 手动录象
#define IPCSDK_TYPE_TIMELY						4    // 定时录象
#define IPCSDK_TYPE_PERIODLY					5    // 周期录象
#define IPCSDK_QUIEY_FILE						6    // 按文件查询

/* 播放文件类型 */
#define IPCSDK_FILE_TYPE_ASF                    0    // asf文件

/* 图片组合查询 */
#define IPCSDK_PIC_CHAN_QUERY					1	 // 通道号查询
#define IPCSDK_PIC_TIME_QUERY					2	 // 时间查询
#define IPCSDK_PIC_EVENT_QUERY					4	 // 事件查询

/* 链接类型 */
#define IPCSDK_CONNECTION_NONE					0x00
#define IPCSDK_CONNECTION_UNICAST				0x01
#define IPCSDK_CONNECTION_MULTICAST				0x02
#define IPCSDK_CONNECTION_BROADCAST				0x03
#define IPCSDK_CONNECTION_TCPSERVER				0x04
#define IPCSDK_CONNECTION_TCPCLIENT				0x05
#define IPCSDK_CONNECTION_TCP					0x06
#define IPCSDK_CONNECTION_SSL					0x07
#define IPCSDK_CONNECTION_UDP					0x08

/* 串口类型 */
#define IPCSDK_SERIAL_LINE_DRIVER_NONE			0x00
#define IPCSDK_SERIAL_LINE_DRIVER_RS232			0x01
#define IPCSDK_SERIAL_LINE_DRIVER_RS422			0x02
#define IPCSDK_SERIAL_LINE_DRIVER_RS485			0x03

/* 音频输入类型 */
#define IPCSDK_AUDIO_INPUTTYPE_LINEIN		    0x00
#define IPCSDK_AUDIO_INPUTTYPE_MIC		        0x01

/* 串口数据通道类型 */
#define IPCSDK_SERIAL_STREAM_TYPE_NONE		    0x00 // 不使用
#define IPCSDK_SERIAL_STREAM_TYPE_UDP		    0x01 // UDP通道
#define IPCSDK_SERIAL_STREAM_TYPE_TCP		    0x02 // TCP通道
#define IPCSDK_SERIAL_STREAM_TYPE_VSIP		    0x03 // 消息通道

/* 串口数据协议类型 */
#define IPCSDK_SERIAL_DATA_TYPE_NONE		    0x00 // 透明通道
#define IPCSDK_SERIAL_DATA_TYPE_VSIP		    0x01 // VSIP协议

/* 码流传输类型 */
#define IPCSDK_MEDIA_STREAM_TYPE_UDP		    0x00 // UDP通道
#define IPCSDK_MEDIA_STREAM_TYPE_TCP		    0x01 // TCP通道

#define IPCSDK_RELATE_ENCID_DISABLE				0xFF // 告警关联视频编码通道无效值
#define IPCSDK_OUTPUT_DISABLE					0xFF // 告警控制输出无效值

/* 告警检测策略 */
#define IPCSDK_PINALMCHK_NONE			  0	 // 不检测
#define	IPCSDK_PINALMCHK_TIMER			  1	 // 定时检测
#define IPCSDK_PINALMCHK_WEEKLY			  2	 // 周期检测
#define IPCSDK_PINALMCHK_ALL			  3	 // 始终检测

/* 并口告警模式 */
#define IPCSDK_PINALARM_INVALID			  0	 // 无效模式
#define IPCSDK_PINALARM_HIGHVOL			  1	 // 高电平模式(输入)
#define IPCSDK_PINALARM_LOWVOL			  2  // 低电平模式(输入)
#define IPCSDK_PINALARM_CLOSE			  3	 // 闭合模式(输出)
#define IPCSDK_PINALARM_OPEN			  4	 // 打开模式(输出)

/* 告警联动参数 */
#define IPCSDK_LNK_VIDSRCSWT_SUPPORT      1  // 支持视频源切换
#define IPCSDK_LNK_CAMPOSSWT_SUPPORT      2  // 支持摄像头预置位切换
#define IPCSDK_LNK_SNAPSHOT_SUPPORT       4  // 支持抓拍
#define IPCSDK_LNK_LOCALREC_SUPPORT       8  // 支持前端录像
#define IPCSDK_LNK_ALARMTIME_SUPPORT     16  // 支持告警计时
#define IPCSDK_LNK_OSDSHOW_SUPPORT       32  // 支持台标显示
#define IPCSDK_LNK_PINOUTPUT_SUPPORT     64  // 支持并口输出
#define IPCSDK_LNK_AUDIOOUT_SUPPORT     128  // 支持告警音频输出
#define IPCSDK_LNK_CONVIDSRC_SUPPORT    256  // 支持关联视频源

/* 周期录象策略 */
#define IPCSDK_NONE_PERIOD                      0    // 不周期录象
#define IPCSDK_PERIOD_DAYLY                     1    // 每天录象
#define IPCSDK_PERIOD_SELECTION                 2    // 周期录象

/* 文件覆盖策略 */
#define IPCSDK_STRATEGY_STOP_REC                0    // 停止录象
#define IPCSDK_STRATEGY_COVER_ALL_REC           1    // 覆盖所有记录
#define IPCSDK_STRATEGY_COVER_REC_EXCEPT_ALARM  2    // 覆盖除告警录象外的记录

/* 命名规则 */
#define SNAPSHOT_NAME_INVALID		0
#define SNAPSHOT_NAME_DATETIME		1	// 日期时间
#define SNAPSHOT_NAME_DEVALIAS		2	// 设备别名
#define SNAPSHOT_NAME_VIDSRCNAME	3	// 视频源名
#define SNAPSHOT_NAME_ALARMTYPE		4	// 告警类型
#define SNAPSHOT_NAME_USERDEFINE	5	// 用户自定义

/* 字幕字体 */
#define IPCSDK_CAPOSDCOLOR_16		1
#define IPCSDK_CAPOSDCOLOR_256		2
#define IPCSDK_CAPOSDCOLOR_FONT		4	// 字体自适应

/* 字幕相关定义 */
#define IPCSDK_OSDUSE_DEFAULT		0	  // 默认方式
#define IPCSDK_OSDUSE_USERDEF		1	  // 用户自定义方式
#define IPCSDK_OSDPICTYPE_INVALID	0	  // 无效类型
#define IPCSDK_OSDPICTYPE_LABEL		1	  // 用于台标显示
#define IPCSDK_OSDPICTYPE_PINALARM	2	  // 用于并口告警显示
#define IPCSDK_OSDPICTYPE_NORECV	3	  // 用于无接收码流字幕显示
#define IPCSDK_OSDPICTYPE_ALMHINT	4	  // 用于告警提示显示
#define IPCSDK_OSDPICTYPE_MMALARM	5	  // 移动侦测告警
#define IPCSDK_OSDPICTYPE_ALMTIME	6	  // 用于告警计时显示
#define IPCSDK_OSDPICTYPE_EXALARM	7	  // 外部告警显示
#define IPCSDK_OSDPICTYPE_RECORD    8     // 录像显示

/* 台标字幕类型 */
#define	IPCSDK_OSD_LABEL0		0		// 视频源字
#define	IPCSDK_OSD_LABEL1		1		// 视频源字
#define	IPCSDK_OSD_LABEL2		2		// 视频源字
#define	IPCSDK_OSD_LABEL3		3		// 视频源字

#define IPCSDK_ALARM_LABEL0    10       //告警字幕
#define IPCSDK_ALARM_LABEL1    11       //告警字幕
#define IPCSDK_ALARM_LABEL2    12       //告警字幕
#define IPCSDK_ALARM_LABEL3    13       //告警字幕

/* 图片格式 */
#define IPCSDK_PIC_DAT		0
#define IPCSDK_PIC_JPG		10

/* 前端存储start */
#define PARAM_TYPE_TIMELY_INFO              0x18 // 定时录象信息
#define PARAM_TYPE_PERIODLY_INFO            0x19 // 周期录象信息
#define PARAM_TYPE_CHANNEL_BASIC_INFO       0x20 // 通道基本信息
#define PARAM_TYPE_REC_SYS_INFO             0x21 // 系统信息
#define PARAM_NMS_INFO                      0x22 // 网管参数

/* 前端存储end */
#define PARAM_SHADE_INFO                    0x23 // 图象屏蔽参数
#define PARAM_XIANTELECOM_ALARM_INFO        0X24 // 告警参数
#define PARAM_XIANTELECOM_BANNER_INFO       0X25 // 横幅参数
#define PARAM_PIC_CONFIG_INFO				0X26 // 图片信息
#define PARAM_MATRIX_CONFIG_INFO			0x27 // 矩阵信息
#define PARAM_PIC_NAMERULE_INFO				0x28 // 抓拍信息
#define PARAM_PUNMS_INFO					0x29 // 网管相关参数
#define PARAM_PINEXT_INFO					0x31 // 并口扩展卡参数

/* 摄像头类型 */
#define IPC_CAMERA_TYPE_SONY				1   // SONY
#define IPC_CAMERA_TYPE_AD					2   // AD
#define IPC_CAMERA_TYPE_PELCO				3   // PELCO
#define IPC_CAMERA_TYPE_PIH					4   // 利凌
#define IPC_CAMERA_TYPE_PELCO_P				5   // PELCO-P
#define IPC_CAMERA_TYPE_PARCO		        6   // PARCO柏高
#define IPC_CAMERA_TYPE_AB					7	// AB 
#define IPC_CAMERA_TYPE_YUNTAI			    8   // 独立云台
#define IPC_CAMERA_TYPE_SAMSUNG				9   // Samsung三星
#define IPC_CAMERA_TYPE_YAAN			    10  // YUNTAI亚安
#define IPC_CAMERA_TYPE_AD168				11	// AD168
#define IPC_CAMERA_TYPE_KALATEL				12	// KALATEL卡拉特
#define IPC_CAMERA_TYPE_JN					13  // JN伟华
#define IPC_CAMERA_TYPE_WV_CS650			14	// 松下CS650
#define IPC_CAMERA_TYPE_WV_CS850			15	// 松下CS850
#define IPC_CAMERA_TYPE_ALEC				16	// ALEC艾立克
#define IPC_CAMERA_TYPE_AB_P				17	// AB-P
#define IPC_CAMERA_TYPE_AB_D				18	// AB-D
#define IPC_CAMERA_TYPE_PELCO_D_JEC			19	// JEC PELCO-D
#define IPC_CAMERA_TYPE_PELCO_P_JEC			20	// JEC PELCO-P
#define IPC_CAMERA_TYPE_ROBOT				21  // ROBOT
#define IPC_CAMERA_TYPE_KDM2130_PELCO_P		22  // KDM2130摄像头控制协议
#define IPC_CAMEAR_TYPE_PELCO_D_K			26  // PELCO_D_K

/* 摄像头别名定义 */
#define IPC_CAM_NAME_SONY              	"SONY"
#define IPC_CAM_NAME_AD                	"AD"
#define IPC_CAM_NAME_PELCO             	"PELCO-D"
#define IPC_CAM_NAME_PIH                "利凌"
#define IPC_CAM_NAME_PELCO_P           	"PELCO-P"
#define IPC_CAM_NAME_PARCO             	"PARCO柏高"
#define IPC_CAM_NAME_AB                	"AB"
#define IPC_CAM_NAME_YUNTAI           	"独立云台"
#define IPC_CAM_NAME_Samsung           	"Samsung三星"
#define IPC_CAM_NAME_YAAN             	"YUNTAI亚安"
#define IPC_CAM_NAME_AD168             	"AD168"
#define IPC_CAM_NAME_KALATEL          	"KALATEL卡拉特"
#define IPC_CAM_NAME_JN                 "JN伟华"
#define IPC_CAM_NAME_WV_CS650         	"松下CS650"
#define IPC_CAM_NAME_WV_CS850         	"松下CS850"
#define IPC_CAM_NAME_ALEC              	"ALEC艾立克"d
#define IPC_CAM_NAME_AB_P              	"AB-P"
#define IPC_CAM_NAME_AB_D              	"AB-D"
#define IPC_CAM_NAME_PELCO_D_JEC      	"JEC PELCO-D"
#define IPC_CAM_NAME_PELCO_P_JEC      	"JEC PELCO-P"
#define IPC_CAM_NAME_TIANDY		   	    "TIANDY"
#define IPC_CAM_NAME_MATRIX           	"通用矩阵协议"
#define IPC_CAM_NAME_ROBOT            	"ROBOT 画面合成矩阵"

/* 视频输出端口类型 */
#define IPC_VIDEO_OUT_TYPE_NONE			    0x00 // 无
#define IPC_VIDEO_OUT_TYPE_C			    0x01 // 复合C端子
#define IPC_VIDEO_OUT_TYPE_VGA			    0x02 // VGA输出
#define IPC_VIDEO_OUT_TYPE_HDMI			    0x04 // HDMI
#define IPC_VIDEO_OUT_TYPE_BNCandVGA	    0X08 // C adn VGA
#define IPC_VIDEO_OUT_TYPE_BNCandHDMI	    0x10 // C and HDMI

/* VGA输出刷新频率 */
#define IPC_VGA_OUT_FREQUENCY_60			0x01 // 刷新频率60
#define IPC_VGA_OUT_FREQUENCY_75			0x02 // 刷新频率75
#define IPC_VGA_OUT_FREQUENCY_85			0x03 // 刷新频率85

/* 设备编解码类型 */
#define IPC_NONE		                  0	// 视频编解码都不支持
#define IPC_ENCODER		                  1	// 只支持视频编码
#define IPC_DECODER		                  2	// 只支持视频解码
#define IPC_BOTH		                  3	// 视频编解码都支持
#define IPC_SMARTSIGHT_ENCODER            4 // SMARTSIGHT视频编码支持
#define IPC_SMARTSIGHT_DECODER            5 // SMARTSIGHT视频解码支持
#define IPC_SMARTSIGHT_BOTH               6 // SMARTSIGHT视频编解码都支持

/* 协议类型 */
#define IPC_STACK_INVALID				(BYTE)0 // 无效协议类型
#define IPC_STACK_VSIP					(BYTE)1 // VSIP协议
#define IPC_STACK_GEP					(BYTE)2 // 全球眼协议
#define IPC_STACK_OSP					(BYTE)3 // OSP协议

/* 出厂默认参数配置 */
#define IPCSDK_DEFCFG_PUID					(LPCSTR)"009900000000000000000100000000  "	// puid or pupwd
#define IPCSDK_DEFCFG_IPADDR				(unsigned long)(inet_addr("192.168.0.99"))	// 出厂IP
#define IPCSDK_DEFCFG_NETMASK				(unsigned long)(inet_addr("255.255.255.0"))	// 出厂子网掩码
#define IPCSDK_DEFCFG_GATEWAY				0								            // 出厂网关
#define IPCSDK_DEFCFG_STARTPORT				60000							            // 起始端口
#define IPCSDK_DEFCFG_TIMEZONE				(char)8								        // 系统时区
#define IPCSDK_DEFCFG_REGPFIP				(LPCSTR)"224.1.1.1"					        // 注册平台地址
#define IPCSDK_DEFCFG_REGPFPORT				5510							            // 注册平台端口
#define IPCSDK_DEFCFG_VSIPMULIP				(LPCSTR)"224.16.32.1"				        // 协议栈搜索多播地址
#define IPCSDK_DEFCFG_PPPOE_NAME            (LPCSTR)"root"                              // PPPoE用户名
#define	IPCSDK_DEFCFG_PPPOE_PASSWORD        (LPCSTR)"root"                              // PPPoE密码
#define IPCSDK_DEFCFG_PPPOE_SERVERNAME      (LPCSTR)"SERVER_NAME"                       // PPPoE服务器名         
#define IPCSDK_DEFCFG_SYNTIMEINT			10		                                    // 同步时间间隔(分钟)
#define IPCSDK_DEFCFG_AUTODIALINT			1		                                    // 自动拨号间隔(秒)
#define IPCSDK_DEFCFG_DIALRETRYTIMES		3		                                    // 拨号重试次数(次)
#define IPCSDK_DEFCFG_LESNDINT				30		                                    // LP间隔(秒)
#define IPCSDK_DEFCFG_LERETRYTIMES			3		                                    // LP重试次数(次)
#define IPCSDK_DEFCFG_CPUTHD				95		                                    // cpu报警阀值
#define IPCSDK_DEFCFG_MEMORYTHD				95		                                    // 内存报警阀值
#define IPCSDK_DEFCFG_VIDLOSTTHD			95		                                    // 视频接收丢包报警阀值
#define IPCSDK_DEFCFG_SNMPNATINT			10		                                    // 网管探测包发送间隔
#define IPCSDK_DEFCFG_TRAPRESNDNUM			10		                                    // trap重发次数
#define IPCSDK_DEFCFG_TRAPRESNDINT			10		                                    // trap重发间隔
#define IPCSDK_DEFCFG_VIDBRT				120		                                    // 亮度
#define IPCSDK_DEFCFG_VIDCTS				120		                                    // 对比度
#define IPCSDK_DEFCFG_VIDSTT				150		                                    // 饱和度
#define IPCSDK_DEFCFG_VIDRESNDTIME			200		                                    // 视频发送重传间隔(毫秒)
#define IPCSDK_DEFCFG_VIDSRCPOLLINT			10		                                    // 视频源轮询时间间隔(秒)
#define IPCSDK_DEFCFG_VIDBITRATE			800	                                        // 视频发送码率
#define IPCSDK_DEFCFG_VIDMAXKFINT			75		                                    // 视频最大关键帧间隔
#define IPCSDK_DEFCFG_VIDMP4MAXQ			31		                                    // mp4最大量化参数
#define IPCSDK_DEFCFG_VIDMP4MINQ			3		                                    // mp4最小量化参数
#define IPCSDK_DEFCFG_VIDH264MAXQ			45		                                    // h264最大量化参数
#define IPCSDK_DEFCFG_VIDH264MINQ			20		                                    // h264最小量化参数
#define IPCSDK_DEFCFG_VIDFRMRATE			25		                                    // 视频帧间隔
#define IPCSDK_DEFCFG_VIDRS1TS				40		                                    // 视频接收第一重传点
#define IPCSDK_DEFCFG_VIDRS2TS				80		                                    // 视频接收第二重传点
#define IPCSDK_DEFCFG_VIDRS3TS				160	                                        // 视频接收第三重传点
#define IPCSDK_DEFCFG_VIDRSRTS				200	                                        // 视频接收过期时间
#define IPCSDK_DEFCFG_AUDIOVOL				12		                                    // 音量
#define IPCSDK_DEFCFG_OSDXPOS				50		                                    // osd起始位置x
#define IPCSDK_DEFCFG_OSDYPOS				50		                                    // osd起始位置y
#define IPCSDK_DEFCFG_RECSFILE				2		                                    // 录像小文件时间间隔(分钟)
#define IPCSDK_DEFCFG_RECFREESIZE			50		                                    // 磁盘剩余空间MB
#define IPCSDK_DEFCFG_RECSYSFREESIZE		350	                                        // 系统分区剩余空间MB
#define IPCSDK_DEFCFG_RECPARTMINSIZE		50		                                    // 能录像的最小分区大小MB
#define IPCSDK_DEFCFG_DLSPEEDLIMIT 		    1024	                                    // 录像下载速度KB
#define IPCSDK_DEFCFG_ALMRESUMETIME			5		                                    // 告警恢复时间
#define IPCSDK_DEFCFG_DETSENSE				80		                                    // 移动帧测灵敏度(%)
#define IPCSDK_DEFCFG_DETALMRATE			10		                                    // 移动帧测告警产生阀值(%)
#define IPCSDK_DEFCFG_BAUDRATE				9600	                                    // 串口波特率
#define IPCSDK_DEFCFG_RECIKEYINT			5		                                    // 录像关键帧间隔(个)
#define IPCSDK_DEFCFG_OSDCLRID				145		                                    // 默认颜色ID -->白色*/
#define IPCSDK_DEFCFG_TESTSERVERIP			(LPCSTR)"2.1.1.1"				            // 测试服务器IP地址
#define IPCSDK_DEFCFG_DOWNLOADPORT			1730	                                    // 录像下载服务端口
#define IPCSDK_DEFCFG_DEFAULTPOSINFO        (LPCSTR)"设备位置信息"                      // 设备默认位置信息
#define IPCSDK_DEFCFG_CFGNULL               (LPCSTR)"NuLL"                              // 配置为空时的替代值

/* 摄像头控制 */
typedef enum ECamControl
{	
	CAM_FOCUSAUTO = 1,                      // 自动对焦
	CAM_FOCUSFAR,                           // 手动对焦(拉远)
	CAM_FOCUSNEAR,                          // 手动对焦(推进)
  	CAM_FOCUSSTOP,                          // 停止对焦
	CAM_MOVEUP,                             // 向上
	CAM_MOVEDOWN,                           // 向下
	CAM_MOVEHOME,                           // 复位
	CAM_MOVELEFT,	                        // 左移
	CAM_MOVERIGHT,                          // 右移
	CAM_MOVESTOP,	                        // 停止
	CAM_ZOOMWIDE,                           // 视野放大
	CAM_ZOOMTELE,                           // 视野缩小
	CAM_ZOOMSTOP,	                        // 停止缩放
	CAM_PRESETSET,	                        // 设置预置位
	CAM_PRESETCALL,                         // 读取预置位
	CAM_PRESETREMOVE,                       // 清除预置位
	CAM_BRIGHTUP,                           // 画面调亮(光圈)
	CAM_BRIGHTDOWN,                         // 画面调暗(光圈)
	CAM_BRIGHTSTOP,                         // 停止调亮(光圈)
	CAM_TOURSTART,                          // 开始预置位巡视  
	CAM_TOUREND,                            // 结束预置位巡视
	CAM_STARTAUTOSCAN,                      // 开始水平巡航
	CAM_STOPAUTOSCAN,                       // 停止水平巡航
	CAM_SCANSPEED_SET,                      // 设置水平扫描速度
	CAM_GOTOPT,                             // 中心定位
	CAM_ZOOMPT,                             // 局部放大
	CAM_ZMWHOLE,                            // 局部缩小
	CAM_DEPTHRATESPEED_ON,                  // 开启景深比例速度
	CAM_DEPTHRATESPEED_OFF,                 // 关闭景深比例速度
	CAM_WIPER_ON,                           // 雨刷开启
	CAM_WIPER_OFF,                          // 雨刷关闭   
	CAM_IR_ON,                              // 红外开启 
	CAM_IR_OFF,                             // 红外关闭
	CAM_DEFROST_ON,                         // 除霜开启                          
	CAM_DEFROST_OFF,                        // 除霜关闭
	CAM_LASER_ON,                           // 激光开启
	CAM_LASER_OFF,                          // 激光关闭
	CAM_RESTORE_FACTORY_SETTINGS,           // 恢复出厂设置
	CAM_DAYNIGHTMOD_ON,                     // 日夜模式开
	CAM_DAYNIGHTMOD_OFF,                    // 日夜模式关
	CAM_DAYNIGHTMOD_AUTO,                   // 日夜模式自动
	CAM_WIDEDYNAMIC_ON,                     // 宽动态开
	CAM_WIDEDYNAMIC_OFF,                    // 宽动态关
	CAM_BACKLIGHT_ON,                       // 背光补偿开
	CAM_BACKLIGHT_OFF,                      // 背光补偿关
	CAM_WHITEBALANCEMODE_AUTO,              // 白平衡模式自动
	CAM_WHITEBALANCEMODE_MANUAL,            // 白平衡模式手动
	CAM_ALL_AUTO,                           // 机芯全自动
	CAM_SHUTTERMODE_AUTO,                   // 快门模式自动
	CAM_SHUTTERMODE_MANUAL,                 // 快门模式手动
	CAM_PLUSMODE_AUTO,                      // 增益模式自动
	CAM_PLUSMODE_MANUAL,                    // 增益模式手动
	CAM_APERTUREMODE_AUTO,                  // 光圈模式自动     
	CAM_APERTUREMODE_MANUAL_HIGHSPEED,      // 光圈模式手动高速(大)
	CAM_APERTUREMODE_MANUAL_LOWSPEED,       // 光圈模式手动低速(小)
	CAM_MENU_SHOW,		                    // 菜单显示
	CAM_MENU_HIDE,		                    // 菜单隐藏
	CAM_MENU_ENTER,		                    // 菜单确定
	CAM_MENU_EXIT,		                    // 菜单取消
	CAM_IMAGE_SHOW,		                    // 图像正显示
	CAM_IMAGE_ANTISHOW,	                    // 图像反显示
	CAM_MACHINE_ZERO,                       // 零位校正 
	CAM_PANPOSION_QUERY,                    // 上位机查询水平位置
	CAM_TILTPOSION_QUERY,                   // 上位机查询俯仰位置
	CAM_ZOOMPOSION_QUERY,                   // 上位机查询变倍值
	CAM_PANPOSION_SET,                      // 上位机设置水平位置
	CAM_TILTPOSION_SET,                     // 上位机设置俯仰位置
	CAM_ZOOMPOSION_SET,                     // 上位机设置变倍值
    CAM_FOCUS_SWITCH_TO_MANUAL,             // 停止自动对焦，切换为手动对焦
	CAM_MOVE_LEFTUP,                        // 向左上
	CAM_MOVE_LEFTDOWN,                      // 向左下
	CAM_MOVE_RIGHTUP,	                    // 向右上
	CAM_MOVE_RIGHTDOWN,                     // 向右下
	CAM_NOOP			                    // 无操作
};

/* 参数配置类型及对应结构 */
typedef enum EParamType
{
	IPCSDK_PARAM_PU_BASE = 0,			// 前端基本参数           TIPC_PU_BASE_PARAM
	IPCSDK_PARAM_SYS_PUB,				// 系统公共参数           TIPC_SYS_PUB_PARAM
	IPCSDK_PARAM_SYS_ALL,               // 系统参数(全部)         TIPC_SYS_CFG_PARAM
	IPCSDK_PARAM_LOCAL_NET_CFG,         // 本地网络配置参数       TIPC_LOCAL_NET_CFG
	IPCSDK_PARAM_VIDEO_ENC_NET,			// 视频编码网络参数       TIPC_VIDEC_ENC_NET_PARAM
	IPCSDK_PARAM_VIDEO_ENC,				// 视频编码参数	          TIPC_VIDEO_ENC_PARAM
	IPCSDK_PARAM_VIDEO_ENC_PUB,			// 视频编码公共参数       TIPC_VIDEO_ENC_PUB_PARAM
	IPCSDK_PARAM_VIDEO_ENC_OSD,			// 视频编码字幕参数       TIPC_VIDEO_ENC_OSD_PARAM
	IPCSDK_PARAM_VIDEO_ENC_INPUTPORT,	// 视频编码视频源端口参数 TIPC_VIDEO_ENC_INPUT_PT_PARAM
	IPCSDK_PARAM_VIDEO_DEC_NET,			// 视频解码网络参数       TIPC_VIDEO_DEC_NET_PARAM
	IPCSDK_PARAM_VIDEO_DEC,				// 视频解码参数　         TIPC_VIDEO_DEC_PARAM
	IPCSDK_PARAM_VIDEO_DEC_PUB,			// 视频解码公共参数       TIPC_VIDEO_DEC_PUB_PARAM
	IPCSDK_PARAM_AUDIO_ENC_NET,			// 音频编码网络参数       TIPC_AUDIO_ENC_NET_PARAM
	IPCSDK_PARAM_AUDIO_ENC,				// 音频编码参数           TIPC_AUDIO_ENC_PARAM
	IPCSDK_PARAM_AUDIO_DEC_NET,			// 音频解码网络参数       TIPC_AUDIO_DEC_NET_PARAM
	IPCSDK_PARAM_AUDIO_DEC,				// 音频解码参数           TIPC_AUDIO_DEC_PARAM
	IPCSDK_PARAM_SERIAL_PORT,			// 串口参数               TIPC_SERIAL_PT_PARAM
	IPCSDK_PARAM_SERIAL_PORT_PUB,		// 串口公共参数           TIPC_SERIAL_PT_PUB_PARAM
	IPCSDK_PARAM_INPUT_PIN,				// 并口输入参数           TIPC_INPUT_PIN_PARAM
	IPCSDK_PARAM_VIDEO_INPUT,			// 视频源端口参数         TIPC_VIDEO_INPUT_PARAM
	IPCSDK_PARAM_NMS_INFO,				// 网管参数               TIPC_NMS_PARAM
	IPCSDK_PARAM_PU_NMS_CFG,			// 网管相关配置参数       TIPC_PU_NMS_CONFIG
	IPCSDK_PARAM_EN_MOTION_DETECT,		// 编码移动侦测参数       TIPC_MOTION_DETECT
	IPCSDK_PARAM_VIDEO_SHADE,           // 图像遮蔽控制参数       TIPC_VIDEO_SHADE_PARAM
	IPCSDK_PARAM_PPPOE,				    // PPPOE拨号参数          TIPC_PPPOE_PARAM
	IPCSDK_PARAM_VIDEO_DEC_RETRANSFER,	// 视频解码重传参数       TIPC_VIDEO_DEC_RETRANSFER_PARAM
	IPCSDK_PARAM_REC_SYS_INFO,			// 录像系统参数           TIPC_REC_SYS_INFO
	IPCSDK_PARAM_REC_BASIC_INFO,		// 录像基本参数           TIPC_REC_CHANNEL_BASIC_INFO
	IPCSDK_PARAM_TIMELY_REC,			// 定时录象信息           TIPC_TIMELY_REC_INFO
	IPCSDK_PARAM_PERIOD_REC,			// 周期录象信息           TIPC_PERIODLY_TIME_INFO
	IPCSDK_PARAM_SNAPSHOT_BASIC_INFO,	// 抓拍基本参数           TIPC_SNAPSHOT_CONFIG
	IPCSDK_PARAM_SNAPSHOT_NAME_RULE,	// 抓拍命名规则           TIPC_SNAPSHOT_NAMING_RULE
	IPCSDK_PARAM_SNAPSHOT_EXT_CFG,      // 快门参数               TIPC_SNAPSHOT_EXT_CFG
	IPCSDK_PARAM_AGC_INFO,              // 自动增益参数(最大值)   TIPC_AGC_INFO
	IPCSDK_PARAM_AUTOSHUT_INFO,         // 自动快门参数(最值)     TIPC_AUTOSHUT_INFO
	IPCSDK_PARAM_BNC_INFO,              // 本地回显参数           TIPC_BNC_INFO
	IPCSDK_PARAM_WHITE_BALANCE,         // 白平衡参数             TIPC_WHITE_BALANCE
	IPCSDK_PARAM_SD_CARD_INFO,          // SD卡容量参数           TIPC_SD_CARD_INFO
	IPCSDK_PARAM_FORMAT_SD_CARD,        // 格式化SD卡参数         TIPC_FORMAT_SD_CARD
	IPCSDK_PARAM_MULTI_CHAN_INFO,       // 双流参数               TIPC_MULTI_CHAN_INFO
	IPCSDK_PARAM_PTZCTRL_INFO           // 云台控制相关参数       TIPC_PTZ_CTRL_INFO
};

/* 浏览视频主辅流类型 */
typedef enum EPlayChanType
{
    PLAY_MAIN = 0,
	PLAY_SUB  = 1,
};

/* 本地录像状态(录像回调中使用) */
typedef enum ElLocRecordingStat
{
    LOCAL_REC_START = 1,
	LOCAL_REC_STOP,
	LOCAL_REC_FILE_START ,
	LOCAL_REC_START_TIME,
	LOCAL_PREREC_START,
	LOCAL_PREREC_STOP,
	LOCAL_STREAM_STOP,        
};

/* 本地录像回放状态 */
typedef enum ElLocRecPlayStat
{
	LOCAL_PLAYBACK_STOP  = 0,
	LOCAL_PLAYBACK_START = 1,
};

/* 前端录像状态 */
typedef enum EPuRecordingStat
{
	eAll,				    // 获取所有状态
	eManualRec,		        // 手动录像
	eMotiveAlarmRec,	    // 移动侦测告警录像
	ePinAlarmRec,	        // 并口告警录像
	eTimerRec,	    	    // 定时录像
	eWeekRec,		        // 周期录像
	ePreRec,			    // 预录
};

/* 录像回放控制 */
typedef enum EPuRecPlayType
{
	RECPLAY_NORMAL	= 0,        // 正常播放
	RECPLAY_STOP,				// 停止操作
	RECPLAY_PAUSE,		        // 暂停操作
	RECPLAY_RESUME,		        // 继续操作

	RECPLAY_2XFORWARD,          // 2倍速快进操作
	RECPLAY_4XFORWARD,          // 4倍速快进操作
	RECPLAY_8XFORWARD,          // 8倍速快进操作  (暂不支持)
	RECPLAY_16XFORWARD,         // 16倍速快进操作 (暂不支持)

	RECPLAY_2XFORWARD_SLOW,     // 1/2倍速慢速播放
	RECPLAY_4XFORWARD_SLOW,     // 1/4倍速慢速播放
	RECPLAY_8XFORWARD_SLOW,     // 1/8倍速慢速播放
	RECPLAY_16XFORWARD_SLOW,    // 1/16倍速慢速播放

	RECPLAY_FORWARD_FRAME,      // 单帧播放操作 (暂不支持)
	
	RECPLAY_2XBACKWARD,         // 2倍速快退操作 (暂不支持)
	RECPLAY_4XBACKWARD,         // 4倍速快退操作 (暂不支持)
	RECPLAY_8XBACKWARD,         // 8倍速快退操作 (暂不支持)
	RECPLAY_16XBACKWARD,        // 16倍速快退操作 (暂不支持)

	RECPLAY_2XBACKWARD_SLOW,    // 1/2倍速慢退操作 (暂不支持)
	RECPLAY_4XBACKWARD_SLOW,    // 1/4倍速慢退操作 (暂不支持)

	RECPLAY_BACKWARD_FRAME,     // 单帧播放操作 (暂不支持)
};

/* 前端录像回放结果 */
typedef enum EPuRecPlayRet
{
	RECRET_FAIL,				// 失败
	RECRET_SUCCESS,				// 成功
	RECRET_ERR_NODISK,			// 无硬盘
	RECRET_ERR_RECORDING,		// 正在录象
	RECRET_ERR_RECPLAYING,		// 放像通道已满,不能再申请
	RECRET_ERR_FILE_NOT_EXIT,	// 文件不存在(该路径下无此文件)
	RECRET_ERR_RECLENGTH_ZERO,	// 文件长度为0 
};

// 查询录像文件方式
typedef enum EPuRecQueryFileMode
{
    PuRecQueryFileMode_FILEINDEX      = 0,   //按文件序号查询
	PuRecQueryFileMode_QUERYINDEX     = 1,   //按查询序号查询
};

// 查询录像文件类型
typedef enum EPuRecQueryType
{
    PuRecQueryType_ALL                =   0,  // 所有
	PuRecQueryType_PIN_ALARM          =   1,  // 并口告警
	PuRecQueryType_MOTIONDETECT_ALARM =   2,  // 移动侦测告警
	PuRecQueryType_MANUAL             =   3,  // 手动录像
	PuRecQueryType_TIMELY             =   4,  // 定时录像
	PuRecQueryType_PERIODLY           =   5,  // 周期录像
	PuRecQueryType_FILE               =   6,  // 录像文件
};

// 查询图片文件类型
typedef enum EPuPicFileQueryType
{
    PuPicFileRecQueryType_ALL         =   0,  // 所有
	PuPicFileRecQueryType_CHAN        =   1,  // 通道号查询
	PuPicFileRecQueryType_TIME        =   2,  // 时间查询
	PuPicFileRecQueryType_EVENT       =   4,  // 事件查询
};

// 录像文件下载状态
typedef enum EDwRecStatus
{
	RECV_STATUS_START = 1,					  // 开始下载
	RECV_STATUS_ADD_STREAM_ERROR,			  // 写流错误
	RECV_STASTU_ADD_FRAME_ERROR,			  // 写帧错误
	RECV_STATUS_SND_REQ,					  // 发送下载请求
	RECV_STATUS_WAIT_STREAM_PROPERTY,		  // 等待流属性
	RECV_STATUS_SND_PROPERTY_ACK,			  // 发送流属性回馈
	RECV_STATUS_WAIT_FRAME_DATA,			  // 等待帧数据 
	RECV_STATUS_SND_FRAME_DATA_ACK,			  // 发送帧回馈
	RECV_STATUS_TIMEOUT,					  // 超时
	RECV_STATUS_CLOSE,						  // 关闭
	RECV_STATUS_RCV_END,					  // 下载结束
	RECV_STATUS_REQ_INVALID,				  // 检查下载参数(发送方认为下载请求参数可能不合法)
	RECV_STATUS_PROBE,						
	RECV_DATA_TYPE, 
	RECV_STATUS_SERVER_REFUSE                 //连接被服务器拒绝
};

// 台标图片类型
typedef enum EPuBmpType
{       
    emPuBmpType_Lable = 0,       // 台标
    emPuBmpType_MMAlarm,         // 移动侦测告警
    emPuBmpType_PinAlarm,        // 并口告警
    emPuBmpType_Banner,          // 横幅
};

// 台标字体模式
typedef enum EPuOsdFontMode
{
    PuOsdFontMode_DEFAULT             =   0,
	PuOsdFontMode_BIG                 =   1,
	PuOsdFontMode_MEDIUM              =   2,
	PuOsdFontMode_SMALL               =   3,
	PuOsdFontMode_CUSTOM              =   4,
};

//台标字体自定义风格
typedef enum EPuCustomFontStyle
{
    PuCustomFontStyle_Normal          = 0x0,
	PuCustomFontStyle_Bold            = 0x1,
	PuCustomFontStyle_Italic          = 0x2,
};

/* 前端能力集类型 */
typedef enum ECapability
{
	CAP_VIDEODECODER = 0x01,            // 0x01 视频解码通道数量
	CAP_VIDEOENCODER,                   // 0x02 视频编码通道数量
	CAP_AUDIODECODER,                   // 0x03 音频解码通道数量
	CAP_AUDIOENCODER,	                // 0x04 音频编码通道数量
	CAP_SERIALPORT,                     // 0x05 串口数量
	CAP_INPUTPIN,	                    // 0x06 并口输入数量
	CAP_OUTPUTPIN,	                    // 0x07 并口输出数量 
	CAP_PTZMOTOR,                       // 0x08 null
	CAP_PTZCONTROL,                     // 0x09 可控制云台数量
	CAP_ARCHIVER,                       // 0x0A null
	CAP_VIDEOIN	= 0x0C,		            // 0x0C 视频源数量
	CAP_ETHERNET,	                    // 0x0D 以太网口数量
	CAP_WIRELESS,	                    // 0x0E 无线网口数量
	CAP_XIANTELE,	                    // 0x0F 西安电信告警数量
	CAP_VIDIOADPTER,	                // 0x10 null
	CAP_VIDIOCOMP,						// 0x11 图像合成通道数
	CAP_VIDEOSRCPOLL,                   // 0x12 视频源轮询通道数量
	CAP_AUDIOMIX,                       // 0x13 音频混音通道数量
	CAP_SHADEAREANUM,                   // 0x14 图像遮挡区域数量
	CAP_DEVPOWERTYPE,                   // 0x15 供电类型
	CAP_MAXSTREAMSEND,                  // 0x16 单个通道最大发送能力(一个通道包括视频和音频)
	CAP_MAXSTREAMDIRSEND,               // 0x17 单个通道的最大直传路数(一个通道包括视频和音频)
	CAP_ENCRYPT,		                // 0x18 是否码流加密
	CAP_RECORDER,	                    // 0x19 录像通道数量
	CAP_PLAYER,			                // 0x1A 放像通道数量
	CAP_FILEDOWNLOAD,	                // 0x1B 录像文件下载通道数量
	CAP_VGAOUT,			                // 0x1C 视频输出VGA接口数量
	CAP_VIDEOOUT,		                // 0x1D 视频输出接口数量
	CAP_OSDCOLOR,						// 0x1E 台标字幕支持的颜色数
	CAP_PLATFORMTYPE,					// 0x1F 接入平台类型
	CAP_MOTIONDETECTAREANUM,			// 0x20 移动侦测区域数量
	CAP_SNAPSHOTCHANNUM,				// 0x21 抓拍通道数
	CAP_SLOTINFONUM,					// 0x22 槽位信息数量(网管用)
	CAP_CBRVBR,							// 0x23 是否能够设置图像质量
	CAP_MAXENCBITRATE,					// 0x24	最大有效码率
	CAP_MULTI_CHANNEL,					// 0x25 是否支持双流
	CAP_MATRIX,							// 0x26 是否支持矩阵接入
	CAP_AUDIODECODETYPE,				// 0x27 音频解码类型
	CAP_SD_CARD,						// 0x28 是否使用SD卡
	CAP_VERSION,						// 0x29 前端版本号
	CAP_TS = 0x30,                      // 0x30 null
	CAP_PUSNAPSHOT,                     // 0x31 前端是否支持抓拍
	CAP_WIFI = 0xE0,					// 0xE0 是否支持WIFI
	CAP_PUSNAPSHOT_SHUTTER,             // VSIP_ENTITY_SNAPSHOT_FLASH
	CAP_VIDEO_FRAME_MODE,               // VSIP_ENTITY_VIDEO_FRAM_MODE
	CAP_NEED_REBOOT,
	CAP_SUPPORT_RTSP,
	CAP_AUDIOENCMIC,
	CAP_CAMERATASK,
	CAP_UPGRADEYUNTAI,
	CAP_WATCHONTIME,
	CAP_AUDIOENCAEC,
	CAP_VIDEOENCRANK,
	CAP_VIDEOD1RES,
    CAP_WHITEBALANCELOW,
	CAP_WHITEBALANCEHIGH,
	CAP_DEVTYPE = 0xEE,
    CAP_TIME_ALARM_OSD
};

/* ACK消息类型定义(用户消息回调) */
#define EVENT_CONNECTION_LOST				    0x05       // 断链通知
#define EVENT_CONNECTION_PING_ACK               0X90       // 链路检测ping应答
#define EVENT_DDNS_UPDATE						0xA8       // DDNS更新通知
#define EVENT_WIFI_SCAN_ACK						0xAE	   // 前端wifi扫描请求应答
#define EVENT_WIFI_CONNECT_ACK					0xAF	   // 前端连接/断开wifi网络结果
#define EVENT_WIFI_SET_AP_ACK					0xB7       // 设置前端保存的AP列表结果
#define EVENT_WIFI_REP_STATE					0xBA       // 前端WIFI网络状态上报
#define EVENT_WIFI_SET_NET_ACK					0xB5       // 设置前端WIFI网络参数结果
#define EVENT_TRANSPARENTCHAN_CFG_MSG			0xF4	   // 通过发送UDP消息得到的版本信息
#define EVENT_TRANSPARENTCHAN_CTRL_MSG			0xF5	   // 通过发送UDP消息得到的版本信息
#define EVENT_IPC_QUERY_MONTH_REC_RSP			0xAB	   // 查询录像月历信息
#define EVENT_QUIEY_REC_FILE_NUM_ACK            0x88       // 返回满足条件的个数
#define EVENT_QUIRY_REC_FILE_ACK                0x7D       // 查询录象文件应答
#define EVENT_IPC_QUIRY_REC_FILE_RSP			0xA5       // 查询IPC录像结果回调
#define EVENT_REC_FILE_START_PLAYERID_ACK       0X81       // 放像器id通知
#define EVENT_REC_PROGRESS_NOTIFY               0x7E       // 进度通知
#define EVENT_RECFILE_UPDATE					0xAC       // 录像播放时新增录像记录通知
#define EVENT_IPC_QUERY_PIC_RSP					0xA7       // 图片查询结果内容
#define EVENT_DOWNLOAD_PIC_FILE_ACK				0xFA	   // 图片下载回应
#define EVENT_DEL_PIC_FILEEX_ACK				0xA9       // 删除图片回应
#define EVENT_SETBMPOSDINFO_ACK					0xF3	   // 发送台标图片文件包回调
#define EVENT_EN_MOTD_STATE_CHANGED			    0x03       // 编码移动侦测告警状态通知
#define EVENT_INPUTPIN_STATE_CHANGED		    0x02       // 并口告警输入状态通知
#define EVENT_OUTPUTPIN_STATE_CHANGED           0x07       // 并口告警输出状态通知
#define EVENT_TRANSPARENT_MESSAGE_RESP		    0xB0	   // 通明通道buffer
#define EVENT_CONFIG_CHANGED				    0x04       // 参数更新通知
#define EVENT_PUSTARTSNAPSHOT_ACK				0xA2	   // 开始前端抓拍应答
#define EVENT_SERIAL_DATA					    0x06       // 终端串口数据
#define EVENT_YUNTAI_UPGRADE_ACK                0x77       // 云台升级完毕应答

/* 端口定义 */
#define IPCSDK_CMU_PORT                       (unsigned short)1722      // CMU 侦听端口
#define IPCSDK_VSIP_PORT                      (unsigned short)5510      // VSIP端口
#define IPCSDK_TEST_LINK                      (unsigned short)10000     // 断链检测超时，毫秒
#define IPCSDK_TEST_LINK_COUNT                (unsigned long)3          // 断链检测超时次数
#define IPCSDK_RTP_PORT                       (unsigned short)60000     // 视频浏览端口
#define IPCSDK_LOCAL_REC_PORT                 (unsigned short)10086     // 本地录像起始端口
#define IPCSDK_DEFAULT_MUL_ADDR               (unsigned long)0x012010E0 // 默认的组播ip
#define IPCSDK_DOWNLOAD_PORT				  (unsigned short)10000     // 下载本地接收端口
#define IPCSDK_DOWNLOAD_PUPORT				  (unsigned short)1730	    // 前端下载发送端口
#define IPCSDK_RECPLAY_STARTPORT			  (unsigned short)65066     // 放像端口

/*************************************************
/	            IPCSDK回调原型定义
/*************************************************/
// 设备消息回调
typedef void (*IPCSDK_MSG_ACK_CB)(unsigned long hHandle, unsigned char uMsgType, void* pData, int nSize, void* pContext);

// 视频解码回调(YUV420数据)
typedef void (*IPCSDK_DRAW_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwPaintDC, unsigned long dwParam);

// 录像状态回调
typedef void (*IPCSDK_REC_STATE_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwRecorderID, unsigned long dwCBType,
									void* pData, int nDataLen, void* pContext);

// 录像下载回调
typedef void (*IPCSDK_DOWN_REC_CB)(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwStatusCode, unsigned long dwCurTime,
								   void* pContext);

// 本地文件播放回调
typedef void (*IPCSDK_FILE_STAT_CB)(unsigned long hPlayHandle, unsigned char byFileState, unsigned long dwSec, unsigned long dwContext);

// 码流转发回调
typedef void (*IPCSDK_STREAMCB)(unsigned long hHandle, unsigned char byChanId, unsigned char* pBuffer, long lLength, void* pContext);

#endif