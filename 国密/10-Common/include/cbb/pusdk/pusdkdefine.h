//////////////////////////////////////////////////////////////////////////
// pusdkdefine.h 
#ifndef PUSDKDEFINE_H_
#define PUSDKDEFINE_H_

#define PUSDK_DEFCFG_TIMEZONE                8          // 系统时区
#define PUSDK_MAX_DISKPART_NUM              16          // 分区最大个数
#define PUSDK_MAX_FILEPATH_LEN              64          // 文件路径最大长度
#define PUSDK_MAX_MOTION_DETECT_AREAM_NUM   16
#define PUSDK_MAX_MTALIAS_LEN               16
#define PUSDK_MAX_NATSEND_INT               30         // nat包发送间隔(秒) 
#define PUSDK_MAX_PIC_REGION_NUM            6
#define PUSDK_MAX_POLL_PORT_NUM             5
#define PUSDK_MAX_PUID_LEN                  32
#define PUSDK_MAX_STRING_LEN16              16
#define PUSDK_MAX_STRING_LEN32              32
#define PUSDK_MAX_STRING_LEN64              64
#define PUSDK_MAX_USER_NUM                  256
#define PUSDK_MAX_VERSION_STRING_LEN        64
#define PUSDK_MAX_VIDEO_SHADE_AREA_NUM      16
#define PUSDK_MAX_VIDEOIN_PORT_NUM          16          // 最大视频输入端口数
#define PUSDK_SYNCMD_TIMEOUT                (5*1000)    // 同步操作超时时间
#define PUSDK_SEARCH_TIMEOUT                (5*1000)    // 搜索超时时间
#define PUSDK_SYNC_TIME_LOCAL               (-1)        // 同步时间设置的本地同步时间
#define PUSDK_MAX_DISK_NUM                  4           // 最大磁盘信息数
#define PUSDK_MAX_SUBEREA_NUM               4           // 每个磁盘的子分区数
#define MAX_RECIPROCAL_LIST_NUM				30


#ifndef MAX_PATH
#define MAX_PATH                            260
#endif


// 摄像头控制函数移动幅度
#define PUSDK_CAMERA_MOVE_RANGE              2

#define PUSDK_MAX_TRANFER_NUM                3           // 最大直传路数

// 端口定义
#define PUSDK_CMU_PORT                       (unsigned short)1722      // CMU 侦听端口
#define PUSDK_VSIP_PORT                      (unsigned short)5510      // VSIP端口
#define PUSDK_TEST_LINK                      (unsigned short)10000     // 断链检测超时，毫秒
#define PUSDK_TEST_LINK_COUNT                (unsigned long)3        // 断链检测超时次数
#define PUSDK_RTP_PORT                       (unsigned short)60000     // 视频浏览端口
#define PUSDK_LOCAL_REC_PORT                 (unsigned short)10086     // 本地录像起始端口
#define PUSDK_DEFAULT_MUL_ADDR               (unsigned long)0x012010E0 // 默认的组播ip

//////////////////////////////////////////////////////////////////////////
// 摄像头别名
#define PUSDK_CAM_NAME_SONY                  "SONY"
#define PUSDK_CAM_NAME_AD                    "AD"
#define PUSDK_CAM_NAME_PELCO                 "PELCO-D"
#define PUSDK_CAM_NAME_PIH                   "利凌"
#define PUSDK_CAM_NAME_PELCO_P               "PELCO-P"
#define PUSDK_CAM_NAME_PARCO                 "PARCO柏高"
#define PUSDK_CAM_NAME_AB                    "AB"
#define PUSDK_CAM_NAME_YUNTAI                "独立云台"
#define PUSDK_CAM_NAME_Samsung               "Samsung三星"
#define PUSDK_CAM_NAME_YAAN                  "YUNTAI亚安"
#define PUSDK_CAM_NAME_AD168                 "AD168"
#define PUSDK_CAM_NAME_KALATEL               "KALATEL卡拉特"
#define PUSDK_CAM_NAME_JN                    "JN伟华"
#define PUSDK_CAM_NAME_WV_CS650              "松下CS650"
#define PUSDK_CAM_NAME_WV_CS850              "松下CS850"
#define PUSDK_CAM_NAME_ALEC                  "ALEC艾立克"
#define PUSDK_CAM_NAME_AB_P                  "AB-P"
#define PUSDK_CAM_NAME_AB_D                  "AB-D"
#define PUSDK_CAM_NAME_PELCO_D_JEC           "JEC PELCO-D"
#define PUSDK_CAM_NAME_PELCO_P_JEC           "JEC PELCO-P"
#define PUSDK_CAM_NAME_TIANDY                 "TIANDY"
#define PUSDK_CAM_NAME_MATRIX                "通用矩阵协议"
#define PUSDK_CAM_NAME_ROBOT                 "ROBOT 画面合成矩阵"

// 设备行业类型
#define PUSDK_INDUSTRY_INVALID               (unsigned char)0  // 无效类型 
#define PUSDK_INDUSTRY_TELECOM               (unsigned char)1  // 中国电信 
#define PUSDK_INDUSTRY_NETCOM                (unsigned char)2  // 中国网通 
#define PUSDK_INDUSTRY_UNICOM                (unsigned char)3  // 中国联通 
#define PUSDK_INDUSTRY_RAILCOM               (unsigned char)4  // 中国铁通 
#define PUSDK_INDUSTRY_MOBILE                (unsigned char)5  // 中国移动 
#define PUSDK_INDUSTRY_ENTERPRISE            (unsigned char)6  // 企业专网 

// 设备行业类型

//name 告警联动参数
#define PUSDK_LINKSET_VIDSRCSWT_SUPPORT   (unsigned short)1   // 支持视频源切换 
#define PUSDK_LINKSET_CAMPOSSWT_SUPPORT   (unsigned short)2   // 支持摄像头预置位切换 
#define PUSDK_LINKSET_SNAPSHOT_SUPPORT    (unsigned short)4   // 支持抓拍 
#define PUSDK_LINKSET_LOCALREC_SUPPORT    (unsigned short)8   // 支持前端录像 
#define PUSDK_LINKSET_ALARMTIME_SUPPORT   (unsigned short)16  // 支持告警计时 
#define PUSDK_LINKSET_OSDSHOW_SUPPORT     (unsigned short)32  // 支持台标显示 
#define PUSDK_LINKSET_PINOUTPUT_SUPPORT   (unsigned short)64  // 支持并口输出 
#define PUSDK_LINKSET_AUDIOOUT_SUPPORT    (unsigned short)128 // 支持告警音频输出 
#define PUSDK_LINKSET_CONVIDSRC_SUPPORT   (unsigned short)256 // 支持关联视频源 
// 告警联动参数

// 前端句柄
#define PUHANDLE      long
#define LPPUHANDLE    long*

// 播放器句柄
#define PLAYERHANDLE long
#define LPPLAYERHANDLE    long*

// 本地定义错误码
#define ERR_SUCCESS                  (0)     // 成功
#define ERR_INVALID                  (-1)    // 内部动态创建对象错误,没有初始化对象,或者普通空指针返回
#define ERR_SIZE                     (-2)    // 大小不正确     
#define ERR_TYPE                     (-3)    // 类型不正确
#define ERR_RANGE                    (-4)    // 范围不正确
#define ERR_HANDLE                   (-5)    // 无效的handle
#define ERR_OVERCAP                  (-6)    // 设备能力不支持
#define ERR_TIMEOUT                  (-7)    // 超时错误
#define ERR_STATE                    (-8)    // 状态错误，前置条件不满足
#define ERR_SPACE                    (-9)    // 空间不足
#define ERR_PATH                     (-10)   // 错误的路径
#define ERR_LACK_RESOURECE           (-11)   // 资源不足
#define ERR_MSG_SEQUENCE             (-12)   // 消息队列错误
#define ERR_DLL_LINK                 (-13)   // dll链接错误
#define ERR_LOCALFILE_OPEN           (-14)   // 没有打开本地文件
#define ERR_LOCALFILE_PLAY           (-15)   // 没有播放本地文件
#define ERR_PU_ERR                   (-16)   // 前端返回错误
#define ERR_NO_DLL_CAMERA            (-17)   // 没有camera.dll库
#define ERR_CAMERA_TYPE              (-18)   // 摄像头类型不正确
#define ERR_GET_CAMERA_OPT_CODE      (-19)   // 获取摄像头操作码失败
#define ERR_FILE_OPT                 (-20)   // 文件操作失败
#define ERR_PLAYER_HANDLE            (-21)   // handle句柄和palyer句柄不对应
#define ERR_PALYER_INVALID           (-22)   // player无效
#define ERR_PALYER_REC               (-23)   // player正在录像
#define ERR_PALYER_PLAY              (-24)   // player正在播放视频
#define ERR_SEARCH                   (-25)   // 搜索错误
#define ERR_CREATE_BMP               (-26)   // 创建位图失败
#define ERR_CONNECT_EXIST            (-27)   // 连接已经存在
#define ERR_CONNECT_PU               (-28)   // 连接设备失败
#define ERR_REQ_RECV_AUDIO           (-29)   // 请求前端接收音频失败

#define ERR_OSP                      (-30)   // Osp组建错误
#define ERR_VSIP                     (-31)   // 协议栈组建错误
#define ERR_SWITCH                   (-32)   // 转发错误
#define ERR_ENC                      (-40)   // 编码器错误
#define ERR_DEC                      (-41)   // 解码器错误
#define ERR_RPPLAY                   (-50)   // 录像库错误
#define ERR_RPDOWNLOAD               (-51)   // 录像下载库错误
#define ERR_CAMERA                   (-60)   // 抓拍库错误
#define ERR_PICISDOWN                (-61)   // 当前正在下载图片
#define ERR_PICISUPLOAD              (-62)   // 当前正在上传图片
#define ERR_DEC_ICOPEN               (-100)  // 本地解码库打开异常

// 与前端交互的错误
#define ERR_SRART                    (-100)

#define ERR_LOGIN_OTHER              (ERR_SRART+11) // 登陆其他错误
#define ERR_LOGIN_TIMEOUT            (ERR_SRART+12) // 登陆超时
#define ERR_LOGIN_PASSWORD           (ERR_SRART+13) // 用户名或密码不正确
#define ERR_LOGIN_ADMINLOGIN         (ERR_SRART+14) // 管理员已经登录
#define ERR_LOGIN_MAX_USER           (ERR_SRART+15) // USER用户登录已到最大值
#define ERR_LOGIN_MAX_LOGIN          (ERR_SRART+16) // 用户登录总数已到最大值
#define ERR_LOGIN_NOT_CONN           (ERR_SRART+17) // 没有连接

// 设备软件版本类型 CAP_VERSION
#define     PUSDK_V39_VERSIONID       0 // 3.9版本
#define     PUSDK_V40_VERSIONID       1 // 4.0版本
#define     PUSDK_V50_VERSIONID       2 // 5.0版本

#define     PUSDK_FILE_TYPE_ASF       0 // asf文件类型
//#define 	PUSDK_FILE_TYPE_KSJ       1
//#define 	PUSDK_FILE_TYPE_KDS       2

//! 无线模式;新的模式添加，注意使按位的方式；
#define PUSDK_WIRELESS_MODE_INVALID   (unsigned char)0   // 无效模式 
#define PUSDK_WIRELESS_MODE_WIFI      (unsigned char)1   // WiFi模式 
#define PUSDK_WIRELESS_MODE_3G        (unsigned char)2   // 3G模式 

// 字幕相关设置
#define MAX_CAPTION_NUM               100   // 每行字幕最大长度
#define MAX_BMP_CAPTION_NUM           12    // 最大字幕个数
#define MAX_ROW_SPACE                 1024  // 最大行间距
#define MAX_SCREEN_OSD_X              720   // 字幕最大宽度
#define BLUR_COLOR_TYPE_1             1     // 沟边沟一个像素
#define BLUR_COLOR_TYPE_2             2     // 沟边沟两个像素

// 版本升级 文件类型
#define PUSDK_UPDATE_TYPE_UBOOT	    1
#define PUSDK_UPDATE_TYPE_IOS		2
#define PUSDK_UPDATE_TYPE_APP		3
#define PUSDK_UPDATE_TYPE_SYS		4
#define PUSDK_UPDATE_TYPE_ALL		5

// 版本升级 默认FTP端口
#define PUSDK_DEFAULT_FTP_PORT		54321


// 浏览视频主辅流类型
typedef enum
{
    emPlayMain = 0,
    emPlaySub  = 1,
}EPlayChanType;

// 播放状态
typedef enum
{
    emPlaying,
    emStop,
    emPause,
}EPlayState;

// 播放类型
typedef enum 
{
    emPlayVideo = 1,
    emPlayRec   = 2
}EPlayType;

// 台标类型 //图片类型
typedef enum
{       
    emPuBmpType_Lable               = 0,  // 台标
    emPuBmpType_MMAlarm,                  // 移动侦测告警
    emPuBmpType_PinAlarm,                 // 并口告警
    emPuBmpType_Banner,                   // 横幅
} EPuBmpType;
 
// 设备编解码类型
typedef enum
{
    PUSDK_DEVICE_NONE                = 0, // 视频编解码都不支持
    PUSDK_DEVICE_ENCODER             = 1, // 只支持视频编码
    PUSDK_DEVICE_DECODER             = 2, // 只支持视频解码
    PUSDK_DEVICE_BOTH                = 3, // 视频编解码都支持
    PUSDK_DEVICE_SMARTSIGHT_ENCODER  = 4, // SMARTSIGHT视频编码支持
    PUSDK_DEVICE_SMARTSIGHT_DECODER  = 5, // SMARTSIGHT视频解码支持
    PUSDK_DEVICE_SMARTSIGHT_BOTH     = 6, // SMARTSIGHT视频编解码都支持                
} EPuDeviceType;

//视频数据格式
typedef enum
{
    PUSDK_VIDEO_TYPE_NONE            = 0,
    PUSDK_VIDEO_TYPE_SN4             = 1,//废弃
    PUSDK_VIDEO_TYPE_MPEG4           = 97,
    PUSDK_VIDEO_TYPE_H261            = 31,
    PUSDK_VIDEO_TYPE_H263            = 34,
    PUSDK_VIDEO_TYPE_H264            = 106,            
} EPuVideoType;

//音频数据格式
typedef enum
{
    PUSDK_AUDIO_TYPE_PCMA            = 8,
    PUSDK_AUDIO_TYPE_PCMU            = 0,
    PUSDK_AUDIO_TYPE_ADPCM           = 5,
    PUSDK_AUDIO_TYPE_G7221C          = 98,               
} EPuAudioType;

//图象分辨率
typedef enum
{
    PUSDK_VIDEO_RESOLUTION_AUTO      = 0,
    PUSDK_VIDEO_RESOLUTION_QQCIF     = 0x08,
    PUSDK_VIDEO_RESOLUTION_QCIF      = 0x20,
    PUSDK_VIDEO_RESOLUTION_CIF       = 0x40,
    PUSDK_VIDEO_RESOLUTION_QVGA      = 0x42,   
    PUSDK_VIDEO_RESOLUTION_D1        = 0x82,
    PUSDK_VIDEO_RESOLUTION_VGA       = 0x83,   
    PUSDK_VIDEO_RESOLUTION_2CIF      = 0x60,
    PUSDK_VIDEO_RESOLUTION_XVGA      = 0xA3,   
    PUSDK_VIDEO_RESOLUTION_720P      = 0xD0,   
    PUSDK_VIDEO_RESOLUTION_1080P     = 0xE0,   
} EPuVideoResolution;

//解码无视频时的显示
typedef enum
{
    PUSDK_PLAYER_STARVE_MODE_INVALID     = 0, //无效(关闭)
    PUSDK_PLAYER_STARVE_MODE_COLOR_BAR   = 1, //彩色条
    PUSDK_PLAYER_STARVE_MODE_LAST_IMAGE  = 2, //屏幕冻结
    PUSDK_PLAYER_STARVE_MODE_BLUE_SCR    = 3, //蓝屏
    PUSDK_PLAYER_STARVE_MODE_SHOW_OSD    = 4, //屏幕冻结并字幕提示                
} EPuPlayerStarveMode;

//视频输出端口类型
typedef enum
{
    PUSDK_VIDEO_OUT_TYPE_INVALID         = 0, // 无效类型
    PUSDK_VIDEO_OUT_TYPE_C               = 1, // 复合C端子
    PUSDK_VIDEO_OUT_TYPE_S               = 2, // S端子
    PUSDK_VIDEO_OUT_TYPE_VGA             = 3, // VGA输出
} EPuVideoOutType;

//台标字体模式
typedef enum
{
    EPuOsdFontMode_DEFAULT      = 0,
    EPuOsdFontMode_BIG          = 1,
    EPuOsdFontMode_MEDIUM       = 2,
    EPuOsdFontMode_SMALL        = 3,
    EPuOsdFontMode_CUSTOM       = 4,
} EPuOsdFontMode;

//台标字体自定义风格
typedef enum
{
    EPuCustomFontStyle_Normal   = 0x0,
    EPuCustomFontStyle_Bold     = 0x1,
    EPuCustomFontStyle_Italic   = 0x2,
} EPuCustomFontStyle;

// 磁盘状态
typedef enum
{
    EPuDiskState_NODISK         = 0,      // 无硬盘
    EPuDiskState_NOPARTION      = 1,      // 未分区
    EPuDiskState_NOFORMAT       = 2,      // 未格式化
    EPuDiskState_NORMAL         = 3,      // 正常
    EPuDiskState_READONLY       = 4,      // 只读
    EPuDiskState_INVALID        = 5,      // 不可用
} EPuDiskState;

// 磁盘健康状态
typedef enum
{
    EPuDiskHeath_Ok             = 0x0,    // 正常
    EPuDiskHeath_NeedRepair     = 0x01,   // 建议强制修复
    EPuDiskHeath_NeedFormat     = 0x02,   // 建议格式化
    EPuDiskHeath_NeedChangeDisk = 0x04,   // 更换硬盘
    EPuDiskHeath_NeedReboot     = 0x08,   // 重新启动        
} EPuDiskHealth;

//Event Arguments 
typedef enum
{
    PUSDK_STATE_ON   = 0x00,
    PUSDK_STATE_OFF  = 0x01,
} EEventType;

// 字幕字体(宋体)表
typedef  enum
{
	eTextHeightDefault, // eTextHeightNo5
	eTextHeightNo0,     // 56
	eTextHeightNo1,     // 48
	eTextHeightNo2,     // 34
	eTextHeightNo3,     // 32
	eTextHeightNo4,     // 28
	eTextHeightNo5,     // 24
	eTextHeightNo6,     // 22
	eTextHeightNo7      // 20 
}EFontTable;

/************************************************************************/
/* Message interface, message type                                      */
/************************************************************************/

// 前端参数结构类型
typedef enum 
{
    emPuConfig_Invalid                 = 0,
    emPuConfig_Sys                     = 1,    // TPuSysParam
    emPuConfig_SysPub                  = 2,    // TPuSysPubParam
//  emPuConfig_SysTime                 = 3,    // Dummy, not set by user
    emPuConfig_PPPOE                   = 4,    // TPuPppoeParam V4不支持
//  emPuConfig_Cap                     = 5,    // TPuCapInfo
    
    emPuConfig_VideoEnc                = 6,    // TPuVideoEncParam
    emPuConfig_VideoEncPub             = 7,    // TPuVideoEncPubParam
    emPuConfig_VideoEncNet             = 8,    // TPuCommonNetParam  
    emPuConfig_VideoEncOSD             = 9,    // TPuVideoEncOsdParam
    emPuConfig_VideoEncMP              = 10,   // TPuVideoEncMpParam
    emPuConfig_VideoEncInputPort       = 11,   // TPuVideoEncInputPortParam
    emPuConfig_VideoEncPoll            = 12,   // TPuVideoEncPollParam
    emPuConfig_VideoEncMultiEncChan    = 13,   // TPuMultiEncChanParam
    
    emPuConfig_VideoDec                = 14,   // TPuVideoDecParam
    emPuConfig_VideoDecPub             = 15,   // TPuVideoDecPubParam
    emPuConfig_VideoDecNet             = 16,   // TPuCommonNetParam
    emPuConfig_VideoDecRetransfer      = 17,   // TPuVideoDecRetransferParam
    
    emPuConfig_AudioEnc                = 18,   // TPuAudioEncParam V4不支持
    emPuConfig_AudioEncNet             = 19,   // TPuCommonNetParam
    
    emPuConfig_AudioDec                = 20,   // TPuAudioDecParam V4不支持
    emPuConfig_AudioDecNet             = 21,   // TPuCommonNetParam
    
    emPuConfig_SerialPort              = 22,   // TPuSerialPortParam
    emPuConfig_SerialPortPub           = 23,   // TPuSerialPortPubParam
    
    emPuConfig_VideoInput              = 24,   // TPuVideoInputParam
    emPuConfig_OsdScroll               = 25,   // TPuOsdScroll
    
    emPuConfig_VideoShade              = 26,   // TPuVideoShadeParam
    
//  emPuConfig_AlarmLink               = 27,   // TPuAlarmLinkInfo
    
    emPuConfig_Nms                     = 28,   // TPuNmsParam
    
//  emPuConfig_VideoDestAdd            = 29,   // Dummy, not set by user
//  emPuConfig_VideoDestRemove         = 30,   // Dummy, not set by user
//  emPuConfig_AudioDestAdd            = 31,   // Dummy, not set by user
//  emPuConfig_AudioDestRemove         = 32,   // Dummy, not set by user
    
//  emPuConfig_CamCtrl                 = 33,
//  emPuConfig_PuNms                   = 34,   // TPuNms
//  emPuConfig_OutputPinState          = 35,   // Dummy, not set by user
    emPuConfig_Snapshot                = 36,   // TPuSnapshotConfig
    emPuConfig_PuSnapConf              = 39,   // TPuSnapConfig 

    //5.0 cfg
    emPuConfig_SysExt                  = 51,   // TPuSysExtParam V4不支持
    emPuConfig_P2p                     = 52,   // TPuVideoDecPpParam    
    emPuConfig_RecChan                 = 53,   // TPuRecChannel V4不支持
    emPuConfig_RecSys                  = 54,   // TPuRecSys V4不支持
	emPuConfig_RecChanBase             = 40,   // TPuRecChanBasicInfo(平台)
	emPuConfig_RecChanPeriodly         = 41,   // TPuRecChannel(平台)
	emPuConfig_RecChanTimely           = 42,   // TVsipRecChannel(平台)
	emPuConfig_RecSysInfo              = 43,   // TVsipRecSys(平台)
    emPuConfig_RecSysEx                = 55,   // TPuRecSysEx V4不支持
    emPuConfig_SnapshotStore           = 56,   // TPuSnapshotStoreConfig
    emPuConfig_CamCfg                  = 57,   // TPuCameraCfg
    emPuConfig_InputPin                = 59,   // TPuInputPinParam V4不支持
    emPuConfig_MotionDet               = 60,   // TPuMotionDetectParam V4不支持
    emPuConfig_OutputPin               = 61,   // TPuOutputPinParam V4不支持
	emPuConfig_Ddns                    = 62,   // TPuDdnsParam(保留)
    emPuConfig_ExtCard                 = 63,   // TPuExtCardParam V4不支持
    
    emPuConfig_OsdExtension            = 65,   // TPuDAQDeviceParam
    emPuConfig_WirelessMode            = 67,   // TPuWirelessModeParam (保留)
    emPuConfig_VerUpdateInfo           = 73,   // TPuVerUpdateInfo (保留)

    emPuConfig_WifiApInfo              = 74,   // TPuWifiApInfo 添加AP点(保留) 
    emPuConfig_WifiCfgGetResp          = 75,   // TPuWifiCfgGetResp 搜索WIFI热点(保留)
    emPuConfig_WifiConnectionReqParam  = 76,   // TPuWifiConnectionReqParam 连接一个热点(保留)
    //emPuConfig_WifiConnectionStatus    = 77,   // TPuWifiConnectionStatus (保留)
    emPuConfig_WifiNetParam            = 78,   // TPuWifiNetParam 网络查询设置(保留)

    emPuConfig_3gCfgParam              = 79,   // TPu3gCfgParam (保留)
    emPuConfig_3gStatus                = 80,   // TPu3gStatus (保留)
	emPuConfig_3gNetCfg                = 81,   // TPu3gNetCfg
    emPuConfig_GpsCfgParam             = 82,   // TPuGpsCfgParam (保留)
    emPuConfig_LocalImageOutput        = 83,   // TPuLocalImageOutputParam
    
    emPuConfig_ImageDisplayParam       = 84,   // TPuImageDisplayParam
    emPuConfig_VideoDecCompParam       = 85,   // TPuVideoDecCompParam


    emPuConfig_End,
} EPuConfigType;

//摄像头类型
typedef enum
{
    PUSDK_CAMERA_TYPE_SONY=1,        //  SONY
    PUSDK_CAMERA_TYPE_AD,            //  AD
    PUSDK_CAMERA_TYPE_PELCO,         //  PELCO-D
    PUSDK_CAMERA_TYPE_PIH,           //  利凌
    PUSDK_CAMERA_TYPE_PELCO_P,       //  PELCO-P
    PUSDK_CAMERA_TYPE_PARCO,         //  PARCO柏高
    PUSDK_CAMERA_TYPE_AB,            //  AB
    PUSDK_CAMERA_TYPE_YUNTAI,        //  独立云台
    PUSDK_CAMERA_TYPE_SAMSUNG,       //  三星
    PUSDK_CAMERA_TYPE_YAAN,          //  亚安
    PUSDK_CAMERA_TYPE_AD168,         //  AD168类型
    PUSDK_CAMERA_TYPE_KALATEL,       //  卡拉特
    PUSDK_CAMERA_TYPE_JN,            //  JN伟华
    PUSDK_CAMERA_TYPE_WV_CS650,      //  松下,是850的老协议
    PUSDK_CAMERA_TYPE_WV_CS850,      //  松下,是850的新协议
    PUSDK_CAMERA_TYPE_ALEC,          //  艾立克
    PUSDK_CAMERA_TYPE_AB_P,          //  AB-P
    PUSDK_CAMERA_TYPE_AB_D,          //  AB-D
    PUSDK_CAMERA_TYPE_PELCO_D_JEC,   //  JEC PELCO-D
    PUSDK_CAMERA_TYPE_PELCO_P_JEC,   //  JEC PELCO-P
    PUSDK_CAMERA_TYPE_TIANDY,        //  天地伟业
    // PUSDK_CAMERA_TYPE_SUNNHO,     //  SUNNHO    矩阵
    // PUSDK_CAMERA_TYPE_PEARMAIN,   //  红苹果  矩阵
	PUSDK_CAMERA_TYPE_PELCO_D_KEDA = 26, // PELCO_D_K
    PUSDK_CAMERA_TYPE_CAMERA_NUM,
    
    PUSDK_CAMERA_TYPE_MATRIX=128,    //  矩阵协议
    PUSDK_MATRIX_TYPE_AB=128,        //  AB 矩阵
    PUSDK_CAMERA_TYPE_ROBOT,         //  ROBOT 画面合成矩阵
    PUSDK_MATRIX_TYPE_SUNNHO,        //  SUNNHO    矩阵
    PUSDK_MATRIX_TYPE_PEARMAIN,      //  红苹果  矩阵
    PUSDK_CAMERA_TYPE_MATRIX_NUM,
} EPuCameraType;

// 告警检测状态
typedef enum
{
    emPuRunTimeMode_None   = 0,
    emPuRunTimeMode_Always = 0x01,
    emPuRunTimeMode_Fix    = 0x02,
    emPuRunTimeMode_Day    = 0x04,
    emPuRunTimeMode_Week   = 0x08,
} EPuRunTimeMode;


//////////////////////////////////////////////////////////////////////////
// 前端录像播放操作类型
typedef enum 
{
    RECPLAY_NORMAL    = 0,      // 正常播放
    RECPLAY_STOP,               // 停止操作
    RECPLAY_PAUSE,              // 暂停操作
    RECPLAY_RESUME,             // 继续操作
    
    RECPLAY_2XFORWARD,          // 2倍速快进操作
    RECPLAY_4XFORWARD,          // 4倍速快进操作
    
    RECPLAY_2XFORWARD_SLOW,     // 1/2倍速慢速播放
    RECPLAY_4XFORWARD_SLOW,     // 1/4倍速慢速播放
}EPuRecPlayType;

// 查询录像文件方式
typedef enum
{
    TPuRecQueryFileMode_FILEINDEX      = 0,   //按文件序号查询
    TPuRecQueryFileMode_QUERYINDEX     = 1,   //按查询序号查询
} EPuRecQueryFileMode;

// 查询录像文件类型
typedef enum
{
    EPuRecQueryType_ALL                =   0,
    EPuRecQueryType_PIN_ALARM          =   1,
    EPuRecQueryType_MOTIONDETECT_ALARM =   2,
    EPuRecQueryType_MANUAL             =   3,
    EPuRecQueryType_TIMELY             =   4,
    EPuRecQueryType_PERIODLY           =   5,
    EPuRecQueryType_FILE               =   6,
} EPuRecQueryType; 

// 查询图片文件类型
typedef enum
{
    EPuPicFileRecQueryType_ALL      =   0,  // 所有
    EPuPicFileRecQueryType_CHAN     =   1,  // 通道号查询
    EPuPicFileRecQueryType_TIME     =   2,  // 时间查询
    EPuPicFileRecQueryType_EVENT    =   4,  // 事件查询
} EPuPicFileQueryType;

// 支持的本地图像显示参数配置能力
// 在使用时使用"|"进行能力叠加即可,例如同时支持画面合成配置,分辨率配置 
// emPuImageSetting_PicComp | emPuImageSetting_Resolution 
//
typedef enum EnumPuImageSetting
{
    emPuImageSetting_PicComp       = 0x01,  // 画面合成配置
    emPuImageSetting_Resolution    = 0x02,  // 分辨率配置
    emPuImageSetting_RefreshRate   = 0x04,  // 刷新率配置
    emPuImageSetting_OutputMode    = 0x08,  // 输出模式配置
} EPuImageSetting;

// 前端能力级类型
typedef enum
{
    CAP_VIDEODECODER = 0x01,     // 视频解码通道数量
    CAP_VIDEOENCODER,            // 视频编码通道数量
    CAP_AUDIODECODER,            // 音频解码通道数量
    CAP_AUDIOENCODER,            // 音频编码通道数量
    CAP_SERIALPORT,              // 串口数量
    CAP_INPUTPIN,                // 并口输入数量
    CAP_OUTPUTPIN,               // 并口输出数量
    CAP_PTZMOTOR,                // null
    CAP_PTZCONTROL,              // 可控制云台数量
    CAP_ARCHIVER,                // null
    CAP_VIDEOIN    = 0x0C,       // 视频源数量
    CAP_ETHERNET,                // 以太网口数量
    CAP_WIRELESS,                // 无线网口数量
    CAP_XIANTELE,                // 西安电信告警数量
    CAP_VIDIOADPTER,             // null
    CAP_VIDIOCOMP,               // 图像合成通道数
    CAP_VIDEOSRCPOLL,            // 视频源轮询通道数量
    CAP_AUDIOMIX,                // 音频混音通道数量
    CAP_SHADEAREANUM,            // 图像遮挡区域数量
    CAP_DEVPOWERTYPE,            // 供电类型
    CAP_MAXSTREAMSEND,           // 单个通道最大发送能力(一个通道包括视频和音频)
    CAP_MAXSTREAMDIRSEND,        // 单个通道的最大直传路数(一个通道包括视频和音频)
    CAP_ENCRYPT,                 // 是否码流加密
    //    CAP_RECORDER,          // 
    CAP_PLAYER,                  // 放像通道数量
    CAP_FILEDOWNLOAD,            // 录像文件下载通道数量
    CAP_VGAOUT,                  // 视频输出VGA接口数量
    CAP_VIDEOOUT,                // 视频输出接口数量
    CAP_OSDCOLOR,                // 台标字幕支持的颜色数
    CAP_PLATFORMTYPE,            // 接入平台类型
    CAP_MOTIONDETECTAREANUM,     // 移动侦测区域数量
    CAP_SNAPSHOTCHANNUM,         // 抓拍通道数
    CAP_SLOTINFONUM,             // 槽位信息数量(网管用)
    CAP_CBRVBR,                  // 是否能够设置图像质量
    CAP_MAXENCBITRATE,           // 最大有效码率
    CAP_MULTI_CHANNEL,           // 是否支持双流
    CAP_MATRIX,                  // 是否支持矩阵接入
    CAP_AUDIODECODETYPE,         // null
    CAP_SD_CARD,                 // 是否使用SD卡
    CAP_VERSION,                 // 前端版本号
    CAP_TS = 0x30,               // null
    // 前端抓拍使能
    CAP_SNAPSHOT_UNSUPPORTED,    // 前端是否支持抓拍
    CAP_SNAPSHOT_DISKSTORAGE,    // 前端是否支持本地硬盘抓拍
    CAP_SNAPSHOT_BUFSENTBACK,    // 前端是否支持数据回传抓拍
    CAP_SNAPSHOT_FTPSENTBACK,    // 前端是否支持FTP回传抓拍
    // 前端录像使能
    CAP_RECORDER,                // 前端是否支持录像 
    CAP_PUREC_MANUAL,            // 前端是否支持手动录像
    CAP_PUREC_ALARM,             // 前端是否支持告警录像
    CAP_PUREC_TIMELY,            // 前端是否支持定时录像
    CAP_PUREC_WEEKLY,            // 前端是否支持周期录像
    CAP_SUPPORT_P2P,             // 前端是否支持P2P
    CAP_SYS_LOG,                 // 前端是否支持日志输出  
    CAP_MAX_SHADE_AREA_SIZE,     // 最大图像遮蔽面积

    CAP_COMPSTYLE,               // 支持的画面合成风格能力
    CAP_IMAGE_DISPLAY_SETTING,   // 支持的本地图像显示参数配置能力(每个bit表示一种配置项) EPuImageSetting
    CAP_SUPPORT_IMAGE_DISPLAY,   // 支持本地图像输出

    CAP_AEC,                     // 是否支持回声抵消
    CAP_OSD_SCROLL,              // 是否支持横幅
    CAP_OSD_EXTENSION,           // 是否支持动态字幕

    CAP_GPS,                     // 是否支持GPS
    CAP_WIFI,                    // 是否支持WiFi
    CAP_3G,                      // 是否支持3G

	CAP_LICENSE,                 // 支持平台许可
    CAP_WEBFORMAT,               // 支持WEB格式化

	CAP_WHITE_BALANCE_ONE,       // 白平衡1
	CAP_WHITE_BALANCE_TWO,       // 白平衡2

	CAP_CHAR_SET_ENCODE,          // 是否支持设置字符集

}ECapType; 

// 摄像头控制类型
typedef enum 
{
    CAM_FOCUSAUTO = 1,                           // 聚焦-自动
    CAM_FOCUSFAR,                                // 聚焦-手动-拉远
    CAM_FOCUSNEAR,                               // 聚焦-手动-拉近
    CAM_FOCUSSTOP,                               // 聚焦-手动-停止
    CAM_MOVEUP,                                  // 向上
    CAM_MOVEDOWN,                                // 向下
    CAM_MOVEHOME,                                // 复位
    CAM_MOVELEFT,                                // 向左
    CAM_MOVERIGHT,                               // 向右
    CAM_MOVESTOP,                                // 停止运动
    CAM_ZOOMWIDE,                                // 变倍-视野放大
    CAM_ZOOMTELE,                                // 变倍-视野缩小
    CAM_ZOOMSTOP,                                // 变倍-停止
    CAM_PRESETSET,                               // 添加预置位
    CAM_BRIGHTUP,                                // 画面调亮
    CAM_BRIGHTDOWN,                              // 画面调暗
    CAM_BRIGHTSTOP,                              // 停止画面调节
    CAM_PRESETCALL,                              // 读取预置位
    
	CAM_PRESETREMOVE                   = 59,     // 删除预置位
	CAM_TOURSTART                      = 48,     // 开始预置位巡视
	CAM_TOUREND                        = 109,    // 结束预置位巡视
	CAM_STARTAUTOSCAN                  = 24,     // 开始水平巡航
	CAM_STOPAUTOSCAN                   = 25,     // 停止水平巡航
	CAM_SCANSPEED_SET                  = 72,     // 巡航速度
	CAM_COMMAND_GOTOPT                 = 22,     // 中心定位
	CAM_COMMAND_ZOOMPT                 = 23,     // 局部放大
	CAM_COMMAND_ZMWHOLE                = 34,     // 局部缩小
	CAM_DEPTHRATESPEED_ON              = 70,     // 开启景深比例速度
	CAM_DEPTHRATESPEED_OFF             = 71,     // 关闭景深比例速度
	CAM_WIPER_ON                       = 105,    // 雨刷开启
	CAM_WIPER_OFF                      = 106,    // 雨刷关闭
	CAM_IR_ON                          = 107,    // 红外开启
	CAM_IR_OFF                         = 108,    // 红外关闭
	CAM_DEFROST_ON                     = 111,    // 除霜开启
	CAM_DEFROST_OFF                    = 112,    // 除霜关闭
	CAM_LASER_ON                       = 113,    // 激光开启
	CAM_LASER_OFF                      = 114,    // 激光关闭
	CAM_RESTORE_FACTORY_SETTINGS       = 110,    // 恢复出厂设置

	CAM_DAYNIGHTMOD_ON                 = 52,     // 日夜模式开
	CAM_DAYNIGHTMOD_OFF                = 53,     // 日夜模式关
	CAM_DAYNIGHTMOD_AUTO               = 54,     // 日夜模式自动
	CAM_WIDEDYNAMIC_ON                 = 50,     // 宽动态开
	CAM_WIDEDYNAMIC_OFF                = 51,     // 宽动态关
	CAM_BACKLIGHT_ON                   = 87,     // 背光补偿开
	CAM_BACKLIGHT_OFF                  = 88,     // 背光补偿关
	CAM_WHITEBALANCEMODE_AUTO          = 102,    // 白平衡模式自动
	CAM_WHITEBALANCEMODE_MANUAL        = 103,    // 白平衡模式手动
	CAM_ALL_AUTO                       = 104,    // 机芯全自动
	CAM_SHUTTERMODE_AUTO               = 98,     // 快门模式自动
	CAM_SHUTTERMODE_MANUAL             = 99,     // 快门模式手动
	CAM_PLUSMODE_AUTO                  = 100,    // 增益模式自动
	CAM_PLUSMODE_MANUAL                = 101,    // 增益模式手动
	CAM_APERTUREMODE_AUTO              = 94,     // 光圈模式自动
	CAM_APERTUREMODE_MANUAL_HIGHSPEED  = 95,     // 光圈模式手动高速(大)
	CAM_APERTUREMODE_MANUAL_LOWSPEED   = 97,     // 光圈模式手动低速(小)

	CAM_NOOP                                     // 无操作
}ECamControl;

//下载状态,录像下载回调输出
typedef enum
{
    DWL_STATUS_START = 1,             // 1开始下载
    DWL_STATUS_ADD_STREAM_ERROR,      // 2写流错误
    DWL_STASTU_ADD_FRAME_ERROR,       // 3写帧错误
    DWL_STATUS_SND_REQ,               // 4发送下载请求
    DWL_STATUS_WAIT_STREAM_PROPERTY,  // 5等待流属性
    DWL_STATUS_SND_PROPERTY_ACK,      // 6发送流属性回馈
    DWL_STATUS_WAIT_FRAME_DATA,       // 7等待帧数据
    DWL_STATUS_SND_FRAME_DATA_ACK,    // 8发送帧回馈
    DWL_STATUS_TIMEOUT,               // 9超时
    DWL_STATUS_CLOSE,                 // 10关闭
    DWL_STATUS_RCV_END,               // 11下载结束
    DWL_STATUS_REQ_INVALID,           // 12检查下载参数(发送方认为下载请求参数可能不合法)
    DWL_STATUS_PROBE,                 //
    DWL_DATA_TYPE,
    DWL_EXCEED_NUM
}EDownloadPuRecStatus;

// 断链原因
typedef enum
{
    EPuDropType_PeerDrop,             // 前端主动断开
    EPuDropType_NetDrop,              // 网络无效
} EPuDropType;

// 抓拍图片分辨率
typedef enum
{
    EPuVideoRes_NONE      =   0x00,
    EPuVideoRes_AUTO      =   0x01,
    EPuVideoRes_QCIF      =   0x02,
    EPuVideoRes_CIF       =   0x03,
    EPuVideoRes_2CIF      =   0x04,
    EPuVideoRes_4CIF      =   0x05,
    EPuVideoRes_QQCIF     =   0x06,
    EPuVideoRes_VGA       =   0x07,
    
    EPuVideoRes_QVGA      =   0x08,
    EPuVideoRes_XVGA      =   0x09,
} EPuVideoRes;

// 前端通道类型
typedef enum
{
    EPuChanType_AudioEnc = 1,
    EPuChanType_AudioDec = 2,
    EPuChanType_VideoEnc = 3,
    EPuChanType_VideoDec = 4,
    EPuChanType_Rec      = 5,
    EPuChanType_Play     = 6,
        
} EPuChanType;

typedef enum
{
    emPuFormatState_IDLE       = 1,
	emPuFormatState_IN_PROCESS = 2,
	emPuFormatState_END        = 3,
} EPuFormatState;

typedef enum
{
    emPuDiskType_HARD_DISK = 1,
	emPuDiskType_SCSI_DISK = 2,
	emPuDiskType_SD_DISK   = 3,
} EPuDiskType;

typedef enum
{
    emPuSubareaUseType_SnapShot = 1,
	emPuSubareaUseType_Record   = 2
} EPuSubareaUseType;

typedef enum
{
    emPuFileSystemType_EXT3 = 1,
	emPuFileSystemType_KFS  = 2,
} EPuFileSystemType;


typedef enum 
{
    EPuLoadDefualt_VideoImageValue = 1,       // 视频图像的亮度、对比度等值
} EPuLoadDefualt;

// PUSDK 内部使用结构与定义
// 格式化操作类型
typedef enum
{
    EPuDiskFormatType_Invalid          = 0,
	EPuDiskFormatType_FormatAllDisk    = 1, //分区并格式化全部磁盘
	EPuDiskFormatType_FormatDisk       = 2, //分区并格式化某个磁盘
	EPuDiskFormatType_FormatPartition  = 3, //格式化分区
	EPuDiskFormatType_RepairDisk       = 4, //磁盘修复
	EPuDiskFormatType_Defragment       = 5, //分区碎片整理		
} EPuDiskFormatType;

// 硬盘异常上报
typedef enum
{
	EPuRecErrNo                      = 0,       // 无错误
	EPuRecErrWrite		             = 18319,   // 写入错误
	EPuRecErrRead				     = 18320,   // 读错误
	EPuRecErrFind				     = 18321,   // 查找失败
	EPuRecErrRename			         = 18322,   // 换名失败
	EPuRecErrNoformat			     = 18323,   // 分区未格式化
	EPuRecErrReadonly			     = 18324,   // 文件系统只读
	EPuRecErrPartinvalid		     = 18325    // 分区不可用
	// 如果错误码不在此范围则为未知错误
}EPuRecError;

typedef enum
{
	EPuLoginPuErrNo                  = 0,
	EPuLoginPuErrLoginFailed		 = 18457,   // 用户名或密码不正确 
	EPuLoginPuErrNoRight			 = 18458,	// 没有权限 
	EPuLoginPuErrNoLogin			 = 18459,	// 用户未登录 
	EPuLoginPuErrUserlogNumOverload	 = 18460,	// USER用户登录已到最大值 
	EPuLoginPuErrAdminLogin			 = 18461,	// 管理员已经登录 
	EPuLoginPuErrTimeout			 = 18463,	// 超时 
	// 如果错误码不在此范围则为未知错误
}EPuLoginPuError;


typedef enum 
{
    emPuWifiApInfo_byEncryMode_NONE       = 0,
	emPuWifiApInfo_byEncryMode_WEP        = 1,
	emPuWifiApInfo_byEncryMode_WPA_PSK    = 2,
	emPuWifiApInfo_byEncryMode_WPA2_PSK   = 3,
} EPuWifiApInfo_byEncryMode;

typedef enum 
{
	emPuWifiApInfo_byArithmetic_WPM_Share     = 1,
	emPuWifiApInfo_byArithmetic_WPM_Open      = 2,
	emPuWifiApInfo_byArithmetic_WPM_Auto      = 3,
	emPuWifiApInfo_byArithmetic_WPM_Invalid   = 4,
	emPuWifiApInfo_byArithmetic_AR_None       = 5,
	emPuWifiApInfo_byArithmetic_AR_TKIP       = 6,
	emPuWifiApInfo_byArithmetic_AR_CCMP       = 7,
	emPuWifiApInfo_byArithmetic_AR_WRAP       = 8,
	emPuWifiApInfo_byArithmetic_AR_Invalid    = 9,

} EPuWifiApInfo_byArithmetic;

typedef enum 
{
    emPuWifiApInfo_byVerifyTpye_Invalid       = 0,
	emPuWifiApInfo_byVerifyTpye_PSK           = 1,
	emPuWifiApInfo_byVerifyTpye_Enterprise    = 2,
} EPuWifiApInfo_byVerifyTpye;

typedef enum 
{
    emPuWifiApInfo_byBSSType_Infra    = 1,
	emPuWifiApInfo_byBSSType_Ad_Hoc   = 2,
} EPuWifiApInfo_byBSSType;

typedef enum
{
    emPu3gNetType_EVDO                = 0x01,
	emPu3gNetType_WCDMA               = 0x02,
	emPu3gNetType_TDSCDMA             = 0x04,
} EPu3gNetType;

// 本地录像进度回调
typedef void (*PUSDK_LOC_REC_PRO_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwCurrentRectime, void* pContext );

// 本地录像状态回调；对应的回调类型和结构如上所示    
typedef void (*PUSDK_LOC_REC_STATE_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwCBType, void* pData, int nDataLen, void* pContext );

// 本地文件播放回调
typedef void (*PUSDK_FILE_STAT_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char byFileState, unsigned long dwSec, unsigned long dwContext) ;

// PUSDK全局通用回调
typedef void (*FUNC_MSG_ACK_CB)( PUHANDLE hHandle, int nMsgType, unsigned char byChanId, 
                                    void* pData, int nSize, void* pContext );

// 音频解码回调
typedef void (*PUSDK_AUDIO_PCM_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char* pBuf, unsigned long dwPcmLen, 
                                    unsigned long dwSamplerate, unsigned long dwBitWidth, 
                                    unsigned long dwContext );

// 视频解码回调，回调YUV420数据给用户,参数分别为:数据、宽、高；pBuf的长度=nWidth*nHeight*3/2
typedef void (*PUSDK_DRAW_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                               unsigned char* pBuf, int nWidth, int nHeight, unsigned long dwContext);

// 回调出解码后的YUV420数据。
// 参数分别为:位图缓冲指针、位图宽、高、位深、YUV数据每个像素占多少位、用户的上下文参数
// pBitmapBuf 的数据长度=dwWidth*dwHeight*dwbitcount/8
typedef void (*PUSDK_FRAMEINFO_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned char *pBitmapBuf, unsigned long dwWidth, unsigned long dwHeight, 
                                    unsigned long dwbitcount,  unsigned long dwContext);

// 定义在pusdkstruct.h 中 
// typedef void (*PUSDK_FRAME_CB)( PUHANDLE hHandle,PLAYERHANDLE hPlayer, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

// 将用户输入的PCM数据编码后的回调,参数分别为:指向码流数据、码流数据长度、调用该回调的用户上下文参数 
typedef void (*PUSDK_INPUT_PCM_BIT_STREAM_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    void* pbyBitStream, unsigned long dwLen, unsigned long dwContext);

// 将用户输入的PCM数据编码后，打成RTP包后的回调,参数分别为:指向RTP数据、流数据长度、调用该回调的用户上下文参数
typedef void (*PUSDK_INPUT_PCM_RTP_PACK_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    void* pbyRtpPack, unsigned long dwLen, unsigned long dwContext);

// 录像下载回调
typedef void (*PUSDK_DOWN_REC_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                    unsigned long dwStatusCode, /* EDownloadPuRecStatus */
                                    unsigned long dwCurTime, void* pContext);
// 录像下载数据回调
typedef void (*PUSDK_DOWN_REC_DATA_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
                                        unsigned char* pBuf, unsigned long dwLen, void* pContext );

// CMU 版本码流转发回调
typedef void (*PUSDK_STREAMCB)( PUHANDLE hHandle, unsigned char byChanId, unsigned char* pBuffer,
							    long lLength, void* pContext );

///////////////////////////////////////////////////////////////
// PUSDK 通用回调通知类型 （ FUNC_MSG_ACK_CB 中的 nMsgType ）

// 类型基准值
#define PUSDK_MSG_CB_BASE                       2000

// 浏览前端图片处理回调，返回图片下载信息 
#define PUSDK_MSG_CB_BROWSE_PUPIC               (PUSDK_MSG_CB_BASE + 1)  // TPuPicFileDownloadRes

// 断链消息
#define PUSDK_MSG_CB_DISCONNECTED               2  // TPuUnitInfo

// 发送消息等待应答超时通知
#define PUSDK_MSG_CB_WAITACK_TIMEOUT            5

// 上传前端图片处理回调，返回图片上传信息
#define PUSDK_MSG_CB_UPLOAD_PUPIC               728  // TPuBmpOsdAck

// 下载前端图片处理回调，返回图片下载信息 
#define PUSDK_MSG_CB_DOWNLOAD_PUPIC             754  // TPuPicFileDownloadAck

// 请求添加视频码流应答
#define PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK         451  // TPuResult

// 请求添加音频码流应答
#define PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK         453  // TPuResult

// 请求删除视频码流应答
#define PUSDK_MSG_CB_VIDEO_SEND_REMOVE_ACK      452  // TPuResult

// 请求删除音频码流应答
#define PUSDK_MSG_CB_AUDIO_SEND_REMOVE_ACK      454 // TPuResult

// 前端重启消息
#define PUSDK_INFORM_REBOOT                    1000 //TPuRebootInfo

// 配置相关
#define PUSDK_5_0_START                        900 
//
#define PUSDK_GET_SYS_ACK                      201         // TPuSysParam
#define PUSDK_SET_SYS_ACK                      401         // TPuResult
#define PUSDK_CONFIG_INFORM_SYS                501
//
#define PUSDK_GET_SYSPUB_ACK                   202         // TPuSysPubParam
#define PUSDK_SET_SYSPUB_ACK                   402
#define PUSDK_CONFIG_INFORM_SYSPUB             502
// 
#define PUSDK_GET_PPPOE_ACK                    204         // TPuPppoeParam
#define PUSDK_SET_PPPOE_ACK                    404 
#define PUSDK_CONFIG_INFORM_PPPOE              504
//
#define PUSDK_GET_VIDEOENC_ACK                 211         // TPuVideoEncParam                
#define PUSDK_SET_VIDEOENC_ACK                 411
#define PUSDK_CONFIG_INFORM_VIDEOENC           511
//
#define PUSDK_GET_VIDEOENC_PUB_ACK             212         // TPuVideoEncPubParam
#define PUSDK_SET_VIDEOENC_PUB_ACK             412
#define PUSDK_CONFIG_INFORM_VIDEOENC_PUB       512
//
#define PUSDK_GET_VIDEOENC_NET_ACK             213         // TPuCommonNetParam
#define PUSDK_SET_VIDEOENC_NET_ACK             413
#define PUSDK_CONFIG_INFORM_VIDEOENC_NET       513
//
#define PUSDK_GET_VIDEOENC_OSD_ACK             214         // TPuVideoEncOsdParam
#define PUSDK_SET_VIDEOENC_OSD_ACK             414
#define PUSDK_CONFIG_INFORM_VIDEOENC_OSD       514
//
#define PUSDK_GET_VIDEOENC_MP_ACK              215         // TPuVideoEncMpParam
#define PUSDK_SET_VIDEOENC_MP_ACK              415
#define PUSDK_CONFIG__CONFIG_INFORM_VIDEOENC_MP  515
//切换视频源返回此信令
#define PUSDK_GET_VIDEOENC_INPUTPORT_ACK       216         // TPuVideoEncInputPortParam
#define PUSDK_SET_VIDEOENC_INPUTPORT_ACK       416
#define PUSDK_CONFIG_INFORM_VIDEOENC_INPUTPORT 516
//
#define PUSDK_GET_VIDEOENC_POLL_ACK            217         // TPuVideoEncPollParam
#define PUSDK_SET_VIDEOENC_POLL_ACK            417
#define PUSDK__CONFIG_INFORM_VIDEOENC_POLL     517
//
#define PUSDK_GET_VIDEOENC_MULTIENCCHAN_ACK    218          // TPuMultiEncChanParam
#define PUSDK_SET_VIDEOENC_MULTIENCCHAN_ACK    418
#define PUSDK_CONFIG_INFORM_MULTIENCCHAN       518
//
#define PUSDK_GET_VIDEODEC_ACK                 220          // TPuVideoDecParam
#define PUSDK_SET_VIDEODEC_ACK                 421
#define PUSDK_CONFIG_INFORM_VIDEODEC           521
//
#define PUSDK_GET_VIDEODEC_PUB_ACK             221          // TPuVideoDecPubParam
#define PUSDK_SET_VIDEODEC_PUB_ACK             422
#define PUSDK_CONFIG_INFORM_VIDEODEC_PUB       522 
// 
#define PUSDK_GET_VIDEODEC_NET_ACK             222         // TPuCommonNetParam
#define PUSDK_SET_VIDEODEC_NET_ACK             423
#define PUSDK_CONFIG_INFORM_VIDEODEC_NET       523
//
#define PUSDK_GET_VIDEODEC_RETRANSFER_ACK      223         // TPuVideoDecRetransferParam 
#define PUSDK_SET_VIDEODEC_RETRANSFER_ACK      424
#define PUSDK_CONFIG_INFORM_VIDEODEC_RETRANSFER 524
//
#define PUSDK_GET_AUDIOENC_ACK                 231         // TPuAudioEncParam
#define PUSDK_SET_AUDIOENC_ACK                 431
#define PUSDK_CONFIG_INFORM_AUDIOENC           531
//
#define PUSDK_GET_AUDIOENC_NET_ACK             232         // TPuCommonNetParam
#define PUSDK_SET_AUDIOENC_NET_ACK             432
#define PUSDK_CONFIG_INFORM_AUDIOENC_NET       532
//
#define PUSDK_GET_AUDIODEC_ACK                 233         // TPuAudioDecParam
#define PUSDK_SET_AUDIODEC_ACK                 433
#define PUSDK_CONFIG_INFORM_AUDIODEC           533
//
#define PUSDK_GET_AUDIODEC_NET_ACK             234        // TPuCommonNetParam
#define PUSDK_SET_AUDIODEC_NET_ACK             434
#define PUSDK_CONFIG_INFORM_AUDIODEC_NET       534
//
#define PUSDK_GET_SERIALPORT_ACK               241         // TPuSerialPortParam
#define PUSDK_SET_SERIALPORT_ACK               441
#define PUSDK_CONFIG_INFORM_SERIALPORT         541
//
#define PUSDK_GET_SERIALPORT_PUB_ACK          242        // TPuSerialPortPubParam
#define PUSDK_SET_SERIALPORT_PUB_ACK          442
#define PUSDK_CONFIG_INFORM_SERIALPORT_PUB    542
//
#define PUSDK_GET_VIDEOINPUT_ACK              245        // TPuVideoInputParam
#define PUSDK_SET_VIDEOINPUT_ACK              445
#define PUSDK_CONFIG_INFORM_INPUTPIN          543
//
#define PUSDK_GET_OSD_SCROLL_ACK              248        // TPuOsdScroll
#define PUSDK_SET_OSD_SCROLL_ACK              461
#define PUSDK_CONFIG_INFORM_OSDSCROLL         561
//
#define PUSDK_GET_VIDEOSHADE_ACK              247        // TPuVideoShadeParam 
#define PUSDK_SET_VIDEOSHADE_ACK              447
#define PUSDK_CONFIG_INFORM_SHADE             547
//
#define PUSDK_GET_NMS_ACK                     42         // TPuNmsParam
#define PUSDK_SET_NMS_ACK                     44
#define PUSDK_CONFIG_INFORM_NMS               548
//
#define PUSDK_GET_SNAPSHOT_CFG_ACK            716        // TPuSnapshotConfig
#define PUSDK_SET_SNAPSHOT_CFG_ACK            718
#define PUSDK_CONFIG_INFORM_SNAPSHOT          549
//
#define PUSDK_GET_SYSEXT_ACK                  (2  + PUSDK_5_0_START) // TPuSysExtParam
#define PUSDK_SET_SYSEXT_ACK                  (4  + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_SYSEXT            (5  + PUSDK_5_0_START)
//
#define PUSDK_GET_P2P_INFO_ACK                (7  + PUSDK_5_0_START) // TPuVideoDecPpParam
#define PUSDK_SET_P2P_INFO_ACK                (9  + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_P2P               (10 + PUSDK_5_0_START)
//
#define PUSDK_GET_RECCHAN_ACK                 (12 + PUSDK_5_0_START) // TPuRecChannel
#define PUSDK_SET_RECCHAN_ACK                 (14 + PUSDK_5_0_START) 
#define PUSDK_CONFIG_INFORM_RECCHAN           (15 + PUSDK_5_0_START)
//
#define PUSDK_GET_RECSYS_ACK                  (17 + PUSDK_5_0_START) // TPuRecSys
#define PUSDK_SET_RECSYS_ACK                  (19 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_RECSYS            (20 + PUSDK_5_0_START)

// 下面的是平台使用 begin
#define PUSDK_RECCHAN_BASICINFO_GET_ACK       46 // TPuRecChanBasicInfo
// 此信令Set的时候使用的数据结构是TPuRecChannel,配置类型emPuConfig_RecChan
#define PUSDK_RECCHAN_BASICINFO_SET_ACK       48 // TPuResult

#define PUSDK_RECCHAN_TIMELYINFO_GET_ACK      50 // TPuRecChannel
#define PUSDK_RECCHAN_TIMELYINFO_SET_ACK      52 // TPuResult
 
#define PUSDK_RECSYS_INFO_GET_ACK             58 // TPuRecSys
#define PUSDK_RECSYS_INFO_SET_ACK             60 // TPuResult

// 
#define PUSDK__RECCHAN_PERIODLYINFO_GET_ACK   54 // TPuRecChannel
#define PUSDK__RECCHAN_PERIODLYINFO_SET_ACK   56 // TPuResult
// end

//
#define PUSDK_GET_RECSYSEX_ACK                (22 + PUSDK_5_0_START) // TPuRecSysEx
#define PUSDK_SET_RECSYSEX_ACK                (24 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_RECSYSEX          (25 + PUSDK_5_0_START)
//
#define PUSDK_GET_SNAPSHOTSTORE_CFG_ACK       (112+ PUSDK_5_0_START) // TPuSnapshotStoreConfig
#define PUSDK_SET_SNAPSHOTSTORE_CFG_ACK       (114+ PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_SNAPSHOTSTORE_CFG  550
//
#define PUSDK_GET_CAM_CFG_ACK                 (32 + PUSDK_5_0_START) // TPuCameraCfg
#define PUSDK_SET_CAM_CFG_ACK                 (34 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_CAM_CFG            35
//
#define PUSDK_GET_INPUTPIN_ACK                (42 + PUSDK_5_0_START) // TPuInputPinParam
#define PUSDK_SET_INPUTPIN_ACK                (44 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_INPUTPIN           543
//
#define PUSDK_GET_MOTIONDETECT_ACK            (47 + PUSDK_5_0_START) // TPuMotionDetectParam
#define PUSDK_SET_MOTIONDETECT_ACK            (49 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_MOTIONDETECT       546
//   
#define PUSDK_GET_OUTPUTPIN_ACK               (52 + PUSDK_5_0_START) // TPuOutputPinParam
#define PUSDK_SET_OUTPUTPIN_ACK               (54 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_OUTPUTPIN          544

#define PUSDK_GET_DDNS_ACK                    (57 + PUSDK_5_0_START) // TPuDdnsParam
#define PUSDK_SET_DDNS_ACK                    (59 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_DDNS               60
//
#define PUSDK_GET_EXTCARD_ACK                 (62 + PUSDK_5_0_START) // TPuExtCardParam
#define PUSDK_SET_EXTCARD_ACK                 (64 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_EXTCARD            65
//
#define PUSDK_GET_DAQDEV_ACK                  (123+ PUSDK_5_0_START)        // TPuDAQDeviceParam
#define PUSDK_SET_DAQDEV_ACK                  (125+ PUSDK_5_0_START)        // TPuResult
#define PUSDK_CONFIG_INFORM_DAQDEV             563
//
#define PUSDK_GET_WIRELESS_MODE_ACK           249    // TPuWirelessModeParam
#define PUSDK_SET_WIRELESS_MODE_ACK           462    // TPuResult
#define PUSDK_CONFIG_INFORM_WIRELESS_MODE     562
//
#define PUSDK_GET_VERUPDATEINFO_ACK           (96+ PUSDK_5_0_START) // TPuVerUpdateInfo
#define PUSDK_SET_VERUPDATEINFO_ACK           (98+ PUSDK_5_0_START) // TPuResult

// 搜索热点响应，返回搜索到的热点个数
#define PUSDK_GET_WIFI_CFG_RESP                (2 + 1100) // TPuWifiCfgGetResp
// 搜索热点信息上报 
#define PUSDK_REPORT_WIFI_AP_CFG               (3 + 1100) // TPuWifiApInfo 
// 添加AP点返回结果
#define PUSDK_ADD_WIFI_AP_RESP                 (5 + 1100) // TPuResult
// 删除AP点返回结果
#define PUSDK_REMOVE_WIFI_AP_RESP              (7 + 1100) // TPuResult
// 连接返回结果
#define PUSDK_SET_WIFI_CONNECTION_ACK          (9 + 1100) // TPuResult
// 连接返回状态信息上报
#define PUSDK_REPORT_WIFI_CONNECTION_STATE     (10 + 1100) // TPuWifiConnectionStatus
// 获取WIFI网络设置结果
#define PUSDK_GET_WIFI_NET_CFG_RESP            (12 + 1100) // TPuWifiNetParam
// 设置WIFI网络设置结果
#define PUSDK_SET_WIFI_NET_CFG_RESP            (14 + 1100) // TPuResult
// WIFI配置改变上报
#define PUSDK_CONFIG_INFORM_WIFI_NET_CFG        567

//
#define PUSDK_GET_3G_CFG_RESP                 (2 + 1150)     // TPu3gCfgParam
#define PUSDK_SET_3G_CFG_RESP                 (4 + 1150)     // TPuResult
#define PUSDK_CONFIG_INFORM_3G_CFG             578
// 
#define PUSDK_GET_3G_NET_CFG_RESP             (7 + 1150)  // TPu3gNetCfg
#define PUSDK_SET_3G_NET_CFG_RESP             (9 + 1150)  // TPuResult
#define PUSDK_CONFIG_INFORM_3G_NET_CFG        568

//
#define PUSDK_GET_GPS_CFG_ACK                 (2 + 1200)     // TPuGpsCfgParam
#define PUSDK_SET_GPS_CFG_ACK                 (4 + 1200)     // TPuResult
#define PUSDK_CONFIG_INFORM_GPS_CFG            579
//
#define PUSDK_GET_LOCALIMAGE_OUTPUT_PARAM_ACK (136 + PUSDK_5_0_START) // TPuLocalImageOutputParam
#define PUSDK_SET_LOCALIMAGE_OUTPUT_PARAM_ACK (138 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_LOCALIMAGE_OUTPUT  564
//
#define PUSDK_GET_IMAGE_DISPLAY_PARAM_ACK     (140 + PUSDK_5_0_START) // TPuImageDisplayParam
#define PUSDK_SET_IMAGE_DISPLAY_PARAM_ACK     (142 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_IMAGE_DISPLAY      565
//
#define PUSDK_GET_VIDEO_DECCOMP_PARAM_ACK     (144 + PUSDK_5_0_START) // TPuVideoDecCompParam
#define PUSDK_SET_VIDEO_DECCOMP_PARAM         (146 + PUSDK_5_0_START) // TPuResult
#define PUSDK_CONFIG_INFORM_VIDEO_DECCOMP      566
// end 配置

// 获取前端录像数
#define  PUSDK_REC_QUERY_NUM_ACK                 82      // TPuRecQueryNumAck

//录像文件搜索 KDM_SearchPuRec
#define  PUSDK_REC_QUERY_FILE_ACK                84      // TPuRecQueryFileAck

// 播放录像的时候，前端返回的录像机ID pleyerid
#define  PUSDK_PLAY_PLAYERID_INFORM              93       // BYTE

// 播放前端录像应答 KDM_StartPlayPuRec
#define  PUSDK_PLAY_START_ACK                    92       // TpuResult

// 前端录像播放进度消息 KDM_StartPlayPuRec
#define PUSDK_MSG_CB_PLAY_PRG                    94       // TPuPlayProgress

// 前端录像控制操作应答 KDM_PuRecPlayOperation 和StopPlayPuRec
#define PUSDK_PLAY_OPERATION_ACK                 98       // TPuResult    

//前端录像播放拖动操作应答 KDM_PuRecSeekLocation
#define PUSDK_PLAY_SEEK_ACK                      96       // TPuResult

// 获取系统信息应答 KDM_GetSysInfo
#define PUSDK_GET_SYSINFO_ACK                    1010     // TPuSysInfo

// 时间同步应答 KDM_SyncPuTime
#define PUSDK_SET_SYSTIME_ACK                    403      // TPuResult

// 查询前端图片应答 KDM_SearchPuPic
#define PUSDK_PICFILE_QUERY_ACK                  742      // TPuPicFileQueryAck

// 删除前端图片应答 KDM_DeletePuPic
#define PUSDK_PICFILE_DEL_ACK                    745      // TPuPicFileDelAck

// 前端图片下载应答 KDM_DownloadPuPic
#define PUSDK_PICFILE_DOWNLOAD_ACK              20000     // TPuResult

// 上传图片应答 UploadTitlePic
#define PUSDK_UPLOAD_TITLE_PIC_ACK              20010     // TPuResult

// 获取前端状态 KDM_GetChanState
#define PUSDK_GET_CHANSTATE_ACK                 1008      // TPuChanStateAck

// 开始前端手动录像应答 KDM_StartPuRec
#define PUSDK_REC_START_ACK                     74       // TPuResult

// 停止前端手动录像应答 KDM_StopPuRec
#define PUSDK_REC_STOP_ACK                      76       // TPuResult

// 鉴权返回信令KDM_LoginPu
#define PUSDK_IDENTIFY_USERPASSWORD_ACK         702      // TPuUserPasswordIdentifyAck

// 启用 GPS 订阅ACK KDM_GpsSubScribe
#define PUSDK_CREATE_SUBSCRIBE_ACK              782      // TPuResult

// 视频丢失上报
#define PUSDK_VIDEO_IN_STATE_CHANGED            649      // TPuVideoInState

// 移动侦测上报
#define PUSDK_EN_MOTD_STATE_CHANGED             642      // TPuMotionDetAlarm

// 硬盘异常上报 
#define PUSDK_REC_ERROR_NOTIFY                  653      // TPuRecErrorNotify

// 报警输入 
#define PUSDK_INPUTPIN_STATE_CHANGED            641      // TPuInputPinAlarm

// 串口数据上报
#define PUSDK_COM_DATA_INFORM                   693      // TPuComData

// GPS 上报数据
#define PUSDK_GPS_DATA_NOTIFY_EX                655  // TPuGpsNotifyParam

#endif



