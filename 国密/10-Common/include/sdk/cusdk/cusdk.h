
//gsoap cusdk service name:   cusdk
//gsoap cusdk service style:  document
//gsoap cusdk service encoding: literal
//gsoap cusdk service namespace: http://www.kedacom.com/cusdk.wsdl
//gsoap cusdk service location:  http://127.0.0.1

//gsoap cusdk schema namespace:	urn:cusdk

/**	cusdk webservice 接口.
*	Created by lizhixing@20111117 
*/

/*************************************************
公共结构体定义
**************************************************/

#import "stlvector.h"
#import "stlset.h"
#import "kdtype.gsoap"

// 
// enum cusdk__EPlayMode
// {
// 	cusdk__PlayModeInvalid,		// 无效值
// 	cusdk__VideoAudioBoth,
// 	cusdk__VideoOnly,
// 	cusdk__AudioOnly
// };

enum cusdk__ETransMode
{
	cusdk__TransModeInvalid,	// 无效值.
	cusdk__TransModeUDP,
	cusdk__TransModeMRTC,
	//cusdk__TransModeTCP
};


enum cusdk__EUserType
{
	// cusdk__UserTypeInvalid, 3A中定义0是超级管理员. 这里必须和它保持一致.
	cusdk__SuperAdmin,
	cusdk__Admin,
	cusdk__Operator,
    cusdk__SecurityAudit,           // 安全审计
    cusdk__SecurityManage,          // 安全管理
    cusdk__SecurityOperator,        // 安全操作
};

enum cusdk__EUserPri
{
    cusdk__PriUserInvalid,              // 无效的用户权限
	cusdk__PriUserCMSRecStart,			// 平台录像.
	cusdk__PriUserCMSRecPlay,			// 平台录像播放.
	cusdk__PriUserCMSRecDel,			// 平台录像删除.
	cusdk__PriUserCMSRecDownload,		// 平台录像下载.
	cusdk__PriUserLocalRec,				// 本地录像.

	cusdk__PriUserCMSRecPlayOtherDomain,// 跨域录像播放.
	cusdk__PriUserPUCall,				// 前端呼叫.		原em_Cms_Call_Ctrl
	cusdk__PriUserAlarmLog,				// 告警日志管理.	原em_Cms_Alarm_Manage
	cusdk__PriUserWaterMark,			// 水印权限
};

enum cusdk__EDevicePri
{
    cusdk__PriPUInvalid,        //无效的设备权限
    cusdk__PriPURecStart,		// 开始前端录像.		设备权限
	cusdk__PriPURecPlay,		// 播放前端录像			设备权限
	cusdk__PriPURecDel,		    // 删除前端录像.		设备权限
	cusdk__PriPURecDownload,	// 前端录像下载.		设备权限

	cusdk__PriPUConfig,	        // 前端参数配置.		设备权限
	cusdk__PriPUAlarmOutput,    // 前端开关量输出控制.	设备权限
	cusdk__PriPUAlarmConfig,    // 布防配置.			设备权限
	cusdk__PriPuAudioCtrl,      // 音频控制.			视频源权限.
	cusdk__PriPuSavePreset,		// 预置位保存权限.		视频源权限.
	cusdk__PriPuPtzLock,		// 云台锁定权限.		视频源权限.
};

enum cusdk__EDeviceType
{
	cusdk__DevTypeUnknown,      // 非法设备类型
	cusdk__DevTypeEncoder,      // 编码设备，包含PUS，通过是否有设备存储能力标识
    cusdk__DevTypeDecoder,      // 解码设备
	cusdk__DevTypeCodecer,      // 编解码设备
	cusdk__DevTypeTVWall,       // 电视墙设备
    //放在B2版本考虑
	//cusdk__DevTypeNVR,
	//cusdk__DevTypeSVR,
	//cusdk__DevAlarmHost,
};

// 通知订阅的类型.
enum cusdk__ESubscribeNotifyType
{
	cusdk__SubscribeNotifyInvalid,		// 无效值.
	cusdk__SubscribePuStatus,			// 设备状态.
	cusdk__SubscribeTVWallStatus,		// 电视墙状态.
	cusdk__SubscribeNRUStatus,			// 录像机/磁盘 状态.
	// cusdk__SubscribeAlarmLinkConfig,	// 告警配置. 平台没有支持这个..
	cusdk__SubscribeSeviceStatus,		// 平台上的服务状态.

	cusdk__SubscribeDeviceGroupStat,			// 设备组统计信息(非递归).
	cusdk__SubscribeDeviceGroupRecursiveStat,	// 设备组统计(递归设备组)信息.

};

// 设备状态订阅的子类型.
enum cusdk__EPuSubscribeType
{
	cusdk__PuSubscribeTypeInvalid,		// 无效值.
	cusdk__PUSubscribeOnlineStatus,		// 上下线.
    cusdk__PUSubscribeConfigStatus,		// 配置.
    cusdk__PUSubscribeServiceStatus,	// 录像/呼叫/解码/轮巡 等服务的状态
	cusdk__PUSubscribeAlarmStatus,		// 告警.
	cusdk__PUSubscribeGPS,				// GPS信息订阅.
	cusdk__PUSubscribeTransdata,		// 前端透明数据.
};

enum cusdk__EPtzCmd
{
    cusdk__PtzCmdInvalid,           //无效指令
	cusdk__PtzMoveUp,			    // 1	//向上移动
	cusdk__PtzMoveDown,	    	    // 2	//向下移动
	cusdk__PtzMoveLeft,			    // 3	//向左移动
	cusdk__PtzMoveRight,			// 4	//向右移动
	cusdk__PtzMoveHome,			    // 5	//回归
	cusdk__PtzMoveStop,			    // 6	//停止移动
	cusdk__PtzZoomIn,				// 7	//拉近摄像头
	cusdk__PtzZoomOut,				// 8	//拉远摄像头
	cusdk__PtzZoomStop,			    // 9	//视野调节停止
	cusdk__PtzFocusFar,		        // 10	//将焦距调远
	cusdk__PtzFocusNear,			// 11	//将焦距调近
	cusdk__PtzFocusAuto,			// 12	//自动调焦
	cusdk__PtzFocusStop,			// 13	//调焦停止
	cusdk__PtzPresetSave,	    	// 14	//摄象头预存
	cusdk__PtzPresetCall,			// 15	//调摄象头预存
	cusdk__PtzCameraReset,		    // 16	// 初始化摄像头
	cusdk__PtzBrightUp,			    // 17	//画面调亮
	cusdk__PtzBrightDown,			// 18	//画面调暗
	cusdk__PtzBrightStop,			// 19	//停止调亮
	cusdk__PtzCommandNew,			// 20	//新命令	  神马东西??	 1.0里没有用到.
	cusdk__PtzCommandAppend,		// 21	//附加命令 神马东西?? 1.0里没有用到.
	cusdk__PtzGotoPoint,			// 22	//中心定位
	cusdk__PtzZoomArea,			    // 23	//局部放大
	
	cusdk__PtzAlecPanSpeed,	        // 24	//艾立克水平移动速度设置命令，（内部使用） 没用到??
	cusdk__PtzAlecTiltSpeed,	    // 25	//艾立克垂直移动速度设置命令，（内部使用） 没用到??
	cusdk__PtzSonySetBright,	    // 26	//sony亮度调节启动命令（内部使用）		没用到??

	cusdk__PtzMatrixSwitch,		    // 27	//切换视频
	cusdk__PtzMatrixVideoMix4,		// 28	//画面合成2×2
	cusdk__PtzMatrixVideoMix9,		// 29	//画面合成3×3
	cusdk__PtzMatrixVideoMix16,	    // 30	//画面合成4×4
	cusdk__PtzMatrixVideoMixPP,	    // 31	//画面合成画中画

	cusdk__PtzZoomAreaStop,		    // 32    // 取消局部放大 

	////菜单//////////////////////////////////////////////////////////////////////	
	cusdk__PtzMenuUp,				// 33	//菜单上
	cusdk__PtzMenuDown,			    // 34	//菜单下
	cusdk__PtzMenuLeft,			    // 35	//菜单左
	cusdk__PtzMenuRight,			// 36	//菜单右

	cusdk__PtzMenuShow,			    // 37	//菜单显示
	cusdk__PtzMenuHide,			    // 38	//菜单隐藏

	cusdk__PtzMenuEnter,			// 39	//菜单确定
	cusdk__PtzMenuCancel,		    // 40	//菜单取消

	cusdk__PtzPresetDel,			// 41	//删除摄象头预存 (没用到?)
	////矩阵切换//////////////////////////////////////////////////////////////////////
	cusdk__PtzMatrixSwitchASC,	    // 42	//正向切换
	cusdk__PtzMatrixSwitchESC,	    // 43	//反向切换
	cusdk__PtzMatrixHoldImage,	    // 44	//图像保持

	cusdk__PtzMoveLeftUp,			//(u8)45		//向左上移动
	cusdk__PtzMoveLeftDown,	        //(u8)46		//向左下移动
	cusdk__PtzMoveRightUp,		    //(u8)47		//向右上移动
	cusdk__PtzMoveRightDown,		//(u8)48		//向右下移动

	////巡航//////////////////////////////////////////////////////////////////////	
	cusdk__PtzAutoScanStart=128,    // 128	//开始自动巡航
	cusdk__PtzAutoScanStop,         // 129	//停止自动巡航	
	cusdk__PtzPresetScanStart,		// 130	//开始巡检(预置位轮巡?)
	cusdk__PtzPresetScanStop,       // 131	//停止巡检(预置位轮巡?)
	cusdk__PtzScanSpeedSet,			// (u8)132	//水平扫描速度（影响自动扫描速度）bySpeed(1-40s)	

	// 雨刷
	cusdk__PtzStartWiper = 160,		//(u8)160	//开始雨刷，第一个参数为雨刷速度，值为0-15，其他参数置零
	cusdk__PtzStopWiper,			//(u8)161	//停止雨刷

	// +by lizhixing@2014-11-04
	cusdk__PtzLightOn,            //(u8)162   //开启灯光
	cusdk__PtzLightOff,           //(u8)163   //关闭灯光
	cusdk__PtzStartExt1,           //(u8)164   //扩展功能一（可能是开启喷水）
	cusdk__PtzStopExt1,           //(u8)165   //关闭喷水扩展功能一
	cusdk__PtzStartExt2,          //(u8)166   //扩展功能二（可能是开启除霜）
	cusdk__PtzStopExt2,			//(u8)167   //关闭除霜扩展功能二

	// +by maonanjing@2016-04-06
	cusdk_PtzLock,				//(u8)168	//云解锁权限
};


enum cusdk__EVideoFormat
{
	cusdk__VF_Invalid,	// 无效值.
	cusdk__VF_SN4,
	cusdk__VF_MPEG4,
	cusdk__VF_H261,
	cusdk__VF_H263,
	cusdk__VF_H264,
	cusdk__VF_H265,
	cusdk__VF_SVAC,
	cusdk__VF_MJPEG,
};

enum cusdk__EAudioFormat
{
	cusdk__AF_Invalid,	// 无效值.
	cusdk__AF_PCMA,
	cusdk__AF_PCMU,
	cusdk__AF_GSM,
	cusdk__AF_G729,
	cusdk__AF_ADPCM,
	cusdk__AF_G7221C,
	cusdk__AF_G722,
	cusdk__AF_AACLC,
	cusdk__AF_G711,
	cusdk__AF_G726,
	cusdk__AF_AAC,
};

enum cusdk__EVideoResolution
{
	cusdk__VS_Invalid,	// 无效值.
	cusdk__VS_AUTO,
	cusdk__VS_QCIF,
	cusdk__VS_CIF,
	cusdk__VS_2CIF,
	cusdk__VS_4CIF,
	cusdk__VS_QQCIF,

	cusdk__VS_QVGA,
	cusdk__VS_VGA,

	cusdk__VS_720P,
	cusdk__VS_1080A,
	cusdk__VS_1080P,
	cusdk__VS_QXGA,

	cusdk__VS_960P,
};

enum cusdk__EUserOrUserGroup
{
	cusdk__TypeInvalid,		// 无效值.
	cusdk__TypeUser,
	cusdk__TypeUserGroup,
};


enum cusdk__ERS_CoverPolicy
{
	cusdk__RS_COVERPOLICY_INVALID,			// 无效值.
	cusdk__RS_COVERPOLICY_STOPRECORD,          /*通道空间满停录像*/
	cusdk__RS_COVERPOLICY_COVERALL,            /*通道空间满覆盖所有录像*/
	cusdk__RS_COVERPOLICY_COVERNORMAL,         /*通道空间满覆盖普通录像*/
	cusdk__RS_COVERPOLICY_UNKNOWN,
};
// enum cusdk__ERS_PartFormat
// {
// 	cusdk__RS_PARTFORMAT_INVALID,	// 无效值.
// 	cusdk__RS_PARTFORMAT_EXT3,
// 	cusdk__RS_PARTFORMAT_VBFS,
// 	cusdk__RS_PARTFORMAT_UNKNOWN,
// };

// 磁盘状态.
enum cusdk__ERS_DiskState
{
	cusdk__RS_DISKSTATE_INVALID,
	cusdk__RS_DISKSTATE_ONLINE,
	cusdk__RS_DISKSTATE_OFFLINE,
	cusdk__RS_DISKSTATE_UNKNOWN,
};


// 分区状态.
enum cusdk__ERS_PartState
{
	cusdk__RS_PARTSTATE_INVALID,		// 无效值.
	cusdk__RS_PARTSTATE_OFFLINE,		// 离线
	cusdk__RS_PARTSTATE_UNFORMATTED,	// 没有格式化
	cusdk__RS_PARTSTATE_FORMATTED,		// 格式化了.没挂载.
	cusdk__RS_PARTSTATE_MOUNTED,		// 已挂载, 正常状态.
	cusdk__RS_PARTSTATE_UMOUNTEDD,		// 未挂载.
	cusdk__RS_PARTSTATE_UNKNOWN,		// 未知状态.
};

enum cusdk__ERecordType
{
	cusdk__ERecordTypeInvalid,
	cusdk__ERecordTypePlatformRecord,	// 平台录像回放.
	cusdk__ERecordTypePURecord,			// 前端录像回放
};

enum cusdk__ERecordEventType
{
	cusdk__RS_RECTYPE_INVALID,		// 无效值.
	cusdk__RS_RECTYPE_ALL,				// 所有类型的录像.
	cusdk__RS_RECTYPE_ALARM,            /*告警录像*/
	cusdk__RS_RECTYPE_HANDLE,               /*手动录像*/
	cusdk__RS_RECTYPE_PLAN,                 /*计划录像，包括定时录像和周期录像*/
	cusdk__RS_RECTYPE_UNKNOWN,              
};

enum cusdk__ERS_RecMode
{
	cusdk__RS_RECMODE_INVALID,		// 无效值.
	cusdk__RS_RECMODE_ALL,			// 正常普通录像.
	cusdk__RS_RECMODE_ONLY_VIDEO,		// 只录视频.
	cusdk__RS_RECMODE_ONLY_KEYFRAME,	// 只录关键帧.
};


enum cusdk__EVCRCmd
{
	cusdk__VCRCMD_INVALID,		// 无效值.
	cusdk__VCRCMD_CONFIG,		// 配置.
	cusdk__VCRCMD_START,        /*开始*/
	cusdk__VCRCMD_STOP,         /*停止*/ 
	cusdk__VCRCMD_PAUSE,        /*暂停*/
	cusdk__VCRCMD_RESUME,       /*恢复*/
	cusdk__VCRCMD_SEEK,         /*时间定位*/
	cusdk__VCRCMD_FASTPLAY,     /*快放 参数？*/		
	cusdk__VCRCMD_SLOWPLAY,     /*慢放 参数？ */
	cusdk__VCRCMD_ONEFRAME,     /*单帧播放*/
	cusdk__VCRCMD_KEYFRAME,     /*只放关键帧*/
	cusdk__VCRCMD_REVERSEPLAY,	/*倒放*/
	cusdk__VCRCMD_FORWARDPLAY,	/*正放*/
	cusdk__VCRCMD_KEYSEEK,		/*定位到关键帧. */
	cusdk__VCRCMD_AUTO_KEYFRAME,          /*自动模式，是否抽帧由NRU确定：低倍速全帧，高倍速只放关键帧*/
	cusdk__VCRCMD_FORCE_KEYFRAME,         /*强制只放关键帧*/
	cusdk__VCRCMD_FORCE_ALLFRAME,         /*强制回放全帧*/
	cusdk__VCRCMD_UNKNOWN,          
};

enum cusdk__EDeviceChannelType
{
	cusdk__DeviceChannelTypeInvalid,	// 无效值.
	cusdk__DevChnTypeVideoSource,	// 视频源.
	cusdk__DevChnTypeAlarmInput,	// 告警输入.

};

// 设备能力标签.
enum cusdk__EDeviceCapLabel
{
	cusdk__DEVICE_CAP_INVALID = 0,				// 无效值.
	cusdk__DEVICE_CAP_LOCAL_STORAGE = 2,			 // 设备能力标签-存储,可以前端录像.
	cusdk__DEVICE_CAP_LABEL_ENCODE_NVR = 7,          // 设备能力标签-NVR
	cusdk__DEVICE_CAP_LABEL_ENCODE_SVR = 8,          // 设备能力标签-SVR
	cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_BULLET = 9,   // 设备能力标签-IPC枪机
	cusdk__DEVICE_CAP_LABEL_ENCODE_IPC_DOME = 10,    // 设备能力标签-IPC球机
	cusdk__DEVICE_CAP_LABEL_ENCODE_GENERAL = 11,     // 设备能力标签-普通编码器
	cusdk__DEVICE_CAP_LABEL_ENCODE_WIRELESS = 12,    // 设备能力标签-无线编码器
	cusdk__DEVICE_CAP_LABEL_GBDEVICE = 14,           // 设备能力标签-国标设备
	cusdk__DEVICE_CAP_LABEL_CONFDEVICE = 15,        // 设备能力标签-会议终端
	cusdk__DEVICE_CAP_LABEL_GBRAWSTREAM = 16,       // 设备能力标签-国标原始码流
};

// 设备创建者的类型.
enum cusdk__EDeviceCreatorType
{
	cusdk__DevCreatorTypePlat = 0,					// 由平台内部创建
	cusdk__DevCreatorTypeG300,						// 由G300创建
	cusdk__DevCreatorTypeGBS,						// 由gbs创建，只限本域设备
	cusdk__DevCreatorTypeGBSForeignDomain,			// 由gbs创建，只限异域设备
};

enum cusdk__ERecordStatus
{
	cusdk__RecordStatusInvalid,	// 无效值.
	cusdk__RecordStatusIdel,	// 当前没有录像.
	cusdk__RecordStatusOk,		// 当前正在正常录像.
	cusdk__RecordStatusTry,		// 当前正在尝试开启录像.
	cusdk__RecordStatusStopping,// 当前正在停止录像.
};

enum cusdk__EAlarmType
{
	cusdk__AlarmTypeInvalid,	// 无效的告警类型.
	cusdk__AlarmMove,			// 移动侦测
	cusdk__AlarmInput,	// 告警输入.
	cusdk__AlarmDiskfull,		// 磁盘满.
	cusdk__AlarmVideoLost,		// 视频源丢失.
	cusdk__AlarmIntelligent,	// 智能告警.
	cusdk__AlarmVideo,			// 视质轮巡告警。

	// 以下是新的告警,只在 AlarmTypeV2 中存在.
	cusdk__AlarmExceptionEvent,		//异常事件消息
	cusdk__AlarmDefocus,				//虚焦检测
	cusdk__AlarmSceneChange,			//场景变换
	cusdk__AlarmRegionInvasion,		//区域入侵
	cusdk__AlarmRegionLeaving,			//区域离开
	cusdk__AlarmObjectTaken,			//物品拿取
	cusdk__AlarmObjectLeft,			//物品遗留
	cusdk__AlarmPeopleGather,			//人员聚集
	cusdk__AlarmCorgon,				//警戒线
	cusdk__AlarmAudioAbnormal,			//声音异常
	cusdk__AlarmLumaChange,			//亮度变化
	//R4B1支持国标2016报警类型 补充的
	cusdk__AlarmAntiDismantle,			//设备防拆报警
	cusdk__AlarmHighTemperature,		//设备高温报警
	cusdk__AlarmLowTemperature,		//设备低温报警
	cusdk__AlarmPersonVideo,           //人工视频
	cusdk__AlarmSportGoalCheck,        //运动目标检测
	cusdk__AlarmTripwireCheck,         //绊线监测
	cusdk__AlarmHoverCheck,             //  徘徊检测报警
	cusdk__AlarmTrafficStatisticsCheck, //  流量统计报警
	cusdk__AlarmDensityCheck,           //密度检测
	cusdk__AlarmVideoException,         //视频异常
	cusdk__AlarmMoveFast,               //快速移动
	cusdk__AlarmRetrogradeCheck,        //逆行检测
	cusdk__AlarmDiskFault,              //存储设备磁盘故障报警
	cusdk__AlarmFanFault,              // 存储设备风扇故障报警
	cusdk__AlarmGPSOverSpeed,           //gps 超速
	cusdk__AlarmSIMDataTrafficExceeded,        //sim 数据流量超标
	
};

enum cusdk__EAlarmStatus
{
	cusdk__AlarmStatusInvalid,	// 无效值.
	cusdk__AlarmOccur,		// 告警产生.
	cusdk__AlarmRestore,	// 告警恢复.
};

// 轮巡状态
enum cusdk__EPollStatus
{
	cusdk__PollStatusInvalid,	// 无效值.
	cusdk__PollStarted,	// 轮巡已经开始.
	cusdk__PollStoped,	// 轮巡已经停止.
	cusdk__PollPaused,	// 轮巡暂停.
};

// 电视墙窗口/解码器 画面合成模式/风格. 
enum cusdk__ETVDivStyle
{
	cusdk__TVDIV_Invalid = 0,	// 无效值.
	cusdk__TVDIV_1 = 1,
	cusdk__TVDIV_2 = 2,
	cusdk__TVDIV_3 = 3,
	cusdk__TVDIV_4 = 4,
	cusdk__TVDIV_6 = 6,
	cusdk__TVDIV_8 = 8,
	cusdk__TVDIV_9 = 9,
	cusdk__TVDIV_16 = 16,
	cusdk__TVDIV_6_EQUAL = 106,	// 新增对6等分的支持 +by lzx@2017-07-11
};


// 电视墙的播放模式.
enum cusdk__ETVWallPlayMode
{
	cusdk__TVWallPlayModeInvalid,	// 无效值.
	cusdk__TVPlayModeLiveStream,
	cusdk__TVPlayModePlatformRecord,
	cusdk__TVPlayModePuRecord,
};

enum cusdk__ECameraProtocolType{
	cusdk__CAM_TYPE_Invalid = 0,
	cusdk__CAM_TYPE_SONY,	        //	SONY
	cusdk__CAM_TYPE_AD,			//	AD
	cusdk__CAM_TYPE_PELCO,			//	PELCO-D
	cusdk__CAM_TYPE_PIH,			//	利凌
	cusdk__CAM_TYPE_PELCO_P,		//	PELCO-P
	cusdk__CAM_TYPE_PARCO,			//	PARCO柏高
	cusdk__CAM_TYPE_AB,			//	AB
	cusdk__CAM_TYPE_YUNTAI,		//	独立云台
	cusdk__CAM_TYPE_Samsung,		//	三星
	cusdk__CAM_TYPE_YAAN,			//	亚安
	cusdk__CAM_TYPE_AD168,			//	AD168类型
	cusdk__CAM_TYPE_KALATEL,		//	卡拉特
	cusdk__CAM_TYPE_JN,			//	JN伟华
	cusdk__CAM_TYPE_WV_CS650,		//	松下,是850的老协议
	cusdk__CAM_TYPE_WV_CS850,		//	松下,是850的新协议
	cusdk__CAM_TYPE_ALEC,		    //	艾立克
	cusdk__CAM_TYPE_AB_P,          //	AB-P
	cusdk__CAM_TYPE_AB_D,          //	AB-D
	cusdk__CAM_TYPE_PELCO_D_JEC,   //	JEC PELCO-D
	cusdk__CAM_TYPE_PELCO_P_JEC,   //	JEC PELCO-P
	cusdk__CAM_TYPE_TIANDY,        //  天地伟业
	cusdk__CAM_TYPE_G100,          //  G100
	cusdk__CAM_TYPE_368ZT,         //  368转台插件
	cusdk__CAM_TYPE_GaoDeTI,       //  GaoDeTI红外插件
	cusdk__CAM_TYPE_DaliTI,        //  DaliTI红外插件
	cusdk__CAM_TYPE_2130_PELCO_P,  //  非标准PELCO_P协议
	cusdk__CAM_TYPE_K_PELCO_D,
	cusdk__CAM_TYPE_JVCTKC676,
	cusdk__CAM_TYPE_INFINOVA_PELCO_D,
	cusdk__CAM_TYPE_PEARMAIN_PELCO_D,
	cusdk__CAM_TYPE_PEARMAIN_PELCO_P,
	cusdk__CAM_TYPE_YAAN_PELCO_D,
	cusdk__CAM_TYPE_LANGKE,			// 朗科.
	cusdk__CAM_TYPE_CAMERA_NUM,

	// 	cusdk__CAM_TYPE_MATRIX=128,	//	矩阵协议
	// 	cusdk__MATRIX_TYPE_AB=128,		//  AB 矩阵
	// 	cusdk__MATRIX_TYPE_ROBOT,			//  ROBOT 画面合成矩阵
	// 	cusdk__MATRIX_TYPE_SUNNHO,		//  SUNNHO	矩阵
	// 	cusdk__MATRIX_TYPE_PEARMAIN,   //  红苹果  矩阵
	// 	cusdk__MATRIX_TYPE_AD,         //  AD 矩阵
	// 	cusdk__MATRIX_TYPE_INFORNOVA,
	// 	cusdk__MATRIX_TYPE_PECLO,      
	// 	cusdk__MATRIX_TYPE_HONEWELL,  //霍尼韦尔
	// 	cusdk__MATRIX_TYPE_TIANDI,
	// 	cusdk__MATRIX_TYPE_HUATONG,//华通矩阵
	// 	cusdk__MATRIX_TYPE_TONGCHUANG,//同创矩阵
	// 	cusdk__MATRIX_TYPE_BOCOM,     //博康矩阵
	// 	cusdk__MATRIX_TYPE_EXTRON,    //爱思创
	// 	cusdk__MATRIX_TYPE_ST_MS650,  //金三立
	// 	cusdk__MATRIX_TYPE_LIKEN,     //利肯
	// 	cusdk__MATRIX_TYPE_VINET,     //微耐特
	// 	cusdk__MATRIX_TYPE_WEPUS,     //威普士
	// 	cusdk__MATRIX_TYPE_ADKEYBOARD, //AD键盘控制
	// 	cusdk__MATRIX_TYPE_XINTAI,    //新泰矩阵
	// 	cusdk__MATRIX_TYPE_JINCHAN,   //金灿矩阵
	// 	cusdk__CAM_TYPE_MATRIX_NUM
};

// 地图配置信息. Jpeg 和 Google 地图.
// 地图配置信息就是地图在初始化的时候需要的一些参数. 两种地图都允许创建.
// 地图信息, Jpeg地图允许用户创建新地图.
// Google地图的创建, 只允许获取地图信息.

enum cusdk__EMapType
{
	cusdk__MapTypeInvalid,	// 无效值.
	cusdk__MapTypeGoogle,
	cusdk__MapTypeJpeg,
};

// 图元增删改
enum cusdk__EMapElementType
{
	cusdk__MapElementTypeInvalid,	// 无效值.
	// 这个枚举内部有哪几种待定..

	cusdk__MapEleCommon,	// 并口???
	cusdk__MapEleMove,		// ????
// 
// 
// 	cusdk__MapEleMainMap,	// ??是否需要? 主地图.
// 	cusdk__MapEleSubMap,	// 子地图. ?是否需要??

};

enum cusdk__EMapCoordType
{
	cusdk__MapCoordTypeInvalid,	// 无效值.
	cusdk__MapCoordEarth,	// 地球坐标.
	cusdk__MapCoordMar,		// 加偏的火星坐标
};




// enum cusdk__EUserOperateType
// {
// 	cusdk__UserOpTypeInvalid,	
// 	cusdk__UserOpTypeLogin,					// 登录.1
// 	cusdk__UserOpTypeLogout,					// 登出.2
// 	cusdk__UserOpTypeLiveStreamPlay,			// 浏览实时码流.3
// 	cusdk__UserOpTypePlatformRecordStart,		// 开始手动平台录像.4
// 	cusdk__UserOpTypePlatformRecordStop,		// 停止手动平台录像.5
// 	cusdk__UserOpTypePlatformRecordPlay,		// 平台录像播放.6
// 	cusdk__UserOpTypePlatformRecordDownload,	// 平台录像下载.7
// 	cusdk__UserOpTypePlatformRecordDelete,		// 平台录像删除.8
// 	cusdk__UserOpTypePuRecordStart,		// 前端手动录像开始.9
// 	cusdk__UserOpTypePuRecordStop,		// 前端手动录像停止.10
// 	cusdk__UserOpTypePuRecordPlay,		// 前端录像播放.11
// 	cusdk__UserOpTypePuRecordDownload,	// 前端录像下载.12
// 	cusdk__UserOpTypePuRecordDelete,	// 前端录像删除.13
// 
// 	cusdk__UserOpTypeTVWall,			// 电视墙控制,配置.14
// 
// 	cusdk__UserOpTypeCall,				// 前端呼叫.15
// 	cusdk__UserOpTypeHangUpCall,		// 前端呼叫.16
// 
// 
// 	cusdk__UserOpTypeUserManage,			// 用户管理17
// 	cusdk__UserOpTypeLogManage,				// 日志管理18
// 	cusdk__UserOpTypeRecordManage,			// 录像机管理类.19
// 	cusdk__UserOpTypeMapConfig,				// 地图配置.20
// 	cusdk__UserOpTypePuConfig,				// 前端配置.21
// 	cusdk__UserOpTypePtzCtrl,				// ptz控制 22
// 	cusdk__UserOpTypeAlarmConfig,			// 告警配置 23
// 	cusdk__UserOpTypeSysParamConfig,		// 系统参数配置 24
// 
// 	
// 	cusdk__UserOpTypeOther = 25
// };

enum cusdk__EUserOperateType
{
	cusdk__UserOpTypeInvalid,	
	cusdk__UserOpTypeLogin,				// 登录/登出. 1
	cusdk__UserOpTypeLiveStream,		// 浏览实时码流. 2
	cusdk__UserOpTypePlatformRecord,	// 平台录像操作. 3
	cusdk__UserOpTypePuRecord,			// 前端录像	4
	cusdk__UserOpTypeTVWall,			// 电视墙控制,配置.5
	cusdk__UserOpTypeCall,				// 前端呼叫.6
	cusdk__UserOpTypeUserManage,		// 用户管理7
	cusdk__UserOpTypeNRUManage,			// 录像机管理类. 8
	cusdk__UserOpTypeMapConfig,			// 地图配置. 9
	cusdk__UserOpTypePuConfig,			// 前端配置.10
	cusdk__UserOpTypePtzCtrl,			// ptz控制 11
	cusdk__UserOpTypeAlarmConfig,		// 告警配置 12
	cusdk__UserOpTypeSysParamConfig,	// 系统参数配置 13
	cusdk__UserOpTypeOther,				// 其它 14

	cusdk__UserOpTypeRecordStart,		// 开启录像 15
	cusdk__UserOpTypeRecordStop,		// 停止录像 16
	cusdk__UserOpTypeRecordPlayback,	// 录像回放 17
	cusdk__UserOpTypeRecordDownload,	// 录像下载 18
	cusdk__UserOpTypeCuLocalRecord,		// CU本地录像 19

	// cusdk__UserOpTypeLiveStreamPlay,		// 实时浏览(enUserOpTypeVideo Play) 20

};

enum cusdk__EVideoQuality
{
	cusdk__VideoQualityInvalid,		// 无效值.
	cusdk__VIDEO_QUALITY_PRIOR = 1, //质量优先
	cusdk__VIDEO_SPEED_PRIOR = 2,	//速度优先
};

enum cusdk__EDeviceLogType
{
	cusdk__DeviceLogTypeInvalid,	// 无效值.
	cusdk__DeviceLogTypeOnline,		// 注册上线.
	cusdk__DeviceLogTypeOffline,	// 下线.
	cusdk__DeviceLogTypePuException,		// 设备异常. 1.0和需求中区分平台设备异常和前端设备异常. 2.0里没有平台设备了..
	cusdk__DeviceLogTypePlatformRecordException,	// 平台录像异常.
	cusdk__DeviceLogTypePuRecordToPlatform,	// 平台录像转存.
	cusdk__DeviceLogTypeOther,
};

enum cusdk__EAlarmLinkActionType
{
	cusdk__AlarmLinkActionTypeInvalid,		// 无效值.
	cusdk__AlarmLinkActionPlatformRecord,	// 平台录像.
	cusdk__AlarmLinkActionPlayOnTV,		// 图像上电视墙.
	cusdk__AlarmLinkActionReadSecond,	// 告警读秒.
	cusdk__AlarmLinkActionIOOutput,		// 告警输出
	cusdk__AlarmLinkCameraPrePos,		// 告警摄像头转到预置位.
};

enum cusdk__EMediaType
{
	cusdk__MediaTypeInvalid,	// 无效值.
	cusdk__MediaTypeVideo,	// 视频.
	cusdk__MediaTypeAudio,	// 音频.
	cusdk__MediaTypeData,	// 数据.
};

enum cusdk__ETransChannelType
{
	cusdk__TransChannelTypeInvalid,	//无效值.
	cusdk__TransChannelTypeRTP,
	cusdk__TransChannelTypeRTCP,
	cusdk__TransChannelTypeData,
};

enum cusdk__EDeviceEntity
{
	cusdk__VSIP_ENTITY_DEVICE					=0x00,
    cusdk__VSIP_ENTITY_VIDEODECODER             =0x01,
    cusdk__VSIP_ENTITY_VIDEOENCODER             =0x02,
    cusdk__VSIP_ENTITY_AUDIODECODER             =0x03,
    cusdk__VSIP_ENTITY_AUDIOENCODER             =0x04,
    cusdk__VSIP_ENTITY_SERIALPORT               =0x05,
    cusdk__VSIP_ENTITY_INPUTPIN                 =0x06,
    cusdk__VSIP_ENTITY_OUTPUTPIN                =0x07,
    cusdk__VSIP_ENTITY_PTZMOTOR                 =0x08,
    cusdk__VSIP_ENTITY_PTZCONTROL               =0x09,
    cusdk__VSIP_ENTITY_ARCHIVER                 =0x0A,
    cusdk__VSIP_ENTITY_VIDEOIN		            =0x0C,
    cusdk__VSIP_ENTITY_ETHERNET					=0x0D,  //0x01~0x0C为基本能力集  Ox0D~0x2F是扩展能力集
    cusdk__VSIP_ENTITY_WIRELESS		            =0x0E,
    cusdk__VSIP_ENTITY_XIANTELE					=0x0F,
    cusdk__VSIP_ENTITY_VIDIOADPTER	            =0x10,
    cusdk__VSIP_ENTITY_VIDIOCOMP	            =0x11,
    cusdk__VSIP_ENTITY_VIDEOSRCPOLL             =0x12,
    cusdk__VSIP_ENTITY_AUDIOMIX                 =0x13,
    cusdk__VSIP_ENTITY_SHADEAREANUM             =0x14,
    cusdk__VSIP_ENTITY_DEVPOWERTYPE             =0x15,
    cusdk__VSIP_ENTITY_MAXSTREAMSEND            =0x16,
    cusdk__VSIP_ENTITY_MAXSTREAMDIRSEND         =0x17,
    cusdk__VSIP_ENTITY_ENCRYPT		            =0x18,
    cusdk__VSIP_ENTITY_RECORDER					=0x19,
    cusdk__VSIP_ENTITY_PLAYER			        =0x1A,
    cusdk__VSIP_ENTITY_FILEDOWNLOAD	            =0x1B,
    cusdk__VSIP_ENTITY_VGAOUT			        =0x1C,
    cusdk__VSIP_ENTITY_VIDEOOUT		            =0x1D, 
    cusdk__VSIP_ENTITY_OSDCOLOR					=0x1E,
    cusdk__VSIP_ENTITY_PLATFORMTYPE				=0x1F,
    cusdk__VSIP_ENTITY_MOTIONDETECTAREANUM		=0x20,
    cusdk__VSIP_ENTITY_SNAPSHOTCHANNUM			=0x21,
    cusdk__VSIP_ENTITY_SLOTINFONUM				=0x22,
    cusdk__VSIP_ENTITY_CBRVBR					=0x23,       // 是否能够设置图像质量
    cusdk__VSIP_ENTITY_MAXENCBITRATE			=0x24,		// 最大有效码率
    cusdk__VSIP_ENTITY_MULTICHAN                =0x25,       // 是否支持双流
    cusdk__VSIP_ENTITY_MATRIX                   =0x26,       // 是否支持矩阵接入
    cusdk__VSIP_ENTITY_VIDEOENCSLAVECHANNELNUM  =0x2F,       // 视频编码辅流通道数  20090509
    cusdk__VSIP_ENTITY_TRANSPORTDATA			=0x29,       // 是否使用透明数据传输
    cusdk__VSIP_ENTITY_GPS         				=0x35,       // GPS功能
    cusdk__VSIP_ENTITY_EXTINPUTPIN      		=0x36,       // 扩展并口能力(只有NVR使用)add G.J. 20091020

    cusdk__VSIP_ENTITY_DECODEEXT                =0x3B,       //是否支持解码器第三方扩展  //add GJ 20100317

	cusdk__VSIP_ENTITY_SUPPORT_RESOLUTION_WH = 0x3D,		 // 是否支持字符串形式的上报分辨率. 

	cusdk__VSIP_ENTITY_EXT_OSD					=0xF0,       //是否支持多行字幕
	cusdk__VSIP_ENTITY_SUPPORT_TOTAL_PICTURE    =0xF2,       //是否支持整体图元 
	cusdk__VSIP_ENTITY_MAX_FRAMERATE			=0xF4,       //支持的最大编码帧率
	cusdk__VSIP_ENTITY_DEVTYPE                  =0xF5,	   //设备类型能力集
	cusdk__VSIP_ENTITY_CTRLTYPE                 =0xF6,	   //控制台类型能力集
	cusdk__VSIP_ENTITY_MAX_ENC_CHANNEL			=0xF7,	   //最大编码通道数能力集
	cusdk__VSIP_ENTITY_CHANNEL_CAPS				=0xF8,       //设备具有通道能力

	cusdk__VSIP_ENTITY_RECLIST_QRY				=0x50,		// 是否支持录像的日历查询.
    cusdk__VSIP_ENTITY_CHANNL_STATE_REPORT		=0x51,		// 是否支持通道状态上报
	cusdk__VSIPDEF_ENTITY_REPLAY_MAXSPEED       =0x52,      //最大放像倍速
	cusdk__VSIPDEF_ENTITY_REPLAY_MAXSPEED_ALLFRAME	=0x53,      //最大满帧放像速度

	cusdk__VSIP_ENTITY_HMDI_COUNT				=0x60,    //HMDI总数
	cusdk__VSIP_ENTITY_HMDI_1_DEC_CHAN_COUNT	=0x61,    //HMDI_1的解码通道数
	cusdk__VSIP_ENTITY_HMDI_2_DEC_CHAN_COUNT	=0x62,    //HMDI_2的解码通道数
	cusdk__VSIP_ENTITY_HMDI_3_DEC_CHAN_COUNT	=0x63,    //HMDI_3的解码通道数
	cusdk__VSIP_ENTITY_HMDI_4_DEC_CHAN_COUNT	=0x64,    //HMDI_4的解码通道数

	cusdk__VsipEntitySmartTab					= 0x97,		// 设备支持拼接(图像缩放)功能.
	cusdk__VSIP_ENTITY_SUPPORT_BD_TRACK_MODE    = 0xD7,     //支持智能枪球联动
    cusdk__VSIP_ENTITY_G100_NOT_SUPPORT_VIDEONAME_SYNC =0x65,   //不支持视频源名称同步能力集

};

enum cusdk__EDeviceTransparentChannelType
{
	cusdk__TransChnCOMPort,			// 设备COM口透明数据
	cusdk__TransChnCameraTaskConfig,// 摄像机任务配置数据.
	cusdk__TransChnExtData,			// 透明数据，支持请求应答通知三态模型 2014-07-29添加。
	// cusdk__TransChnVSIP,			// vsip 协议栈通道.
	// cusdk__TransChnReqRspNtf,		// 透明数据, 请求应答通知三态模型. 2014年8月14日 添加.
	cusdk__TransChnUserDef,        // 透明数据，用户自定义透明数据类型,当前仅国标接入模块支持接收并处理
};

enum cusdk__EChnStatus
{
	cusdk__CHNSTATUS_IDEL = 0,      
	cusdk__CHNSTATUS_ONLINE,
	cusdk__CHNSTATUS_OFFLINE ,
};

//域状态 
enum cusdk__EDomainStatus 
{ 
    cusdk__DomainStatusInvalid,   // 无效值. 
    cusdk__DomainOnline,          // 在线. 
    cusdk__DomainOffline,         // 离线. 
};

class cusdk__UserGroup
{
	xsd__string id;			            // id.
	xsd__string name;		            // 用户组名称.
    xsd__string description;            // 描述
	xsd__string creator;	            // 创建者.
	enum cusdk__EUserType  userType;				// 用户类型. 操作员,管理员,超级管理员.
	std::set< enum cusdk__EUserPri > userPri;		// 用户权限. cusdk__PriUserAlarmLog及之前的权限可以放到这里.cusdk__PriUserWaterMark及之后的权限放到 userPriV2.
	std::set< int > userPriV2;						// 用户权限. +by lzx@2018-01-30 V2R4B1


	xsd__boolean maxStreamNumEnable;	// 是否启用 maxStreamNum, true 表示限制, false表示无限制. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int maxStreamNum;				// 用户允许的最大码流数目. +by lzx@2017-12-14 V2R2B3SP1
	// 视频水印功能. +by lzx@2017-12-20 V2R4B1
	xsd__boolean waterMarkSupported;	// 是否支持水印功能. 兼容性字段, 只有启用水印功能,以下字段才有效.
	xsd__boolean waterMarkEnabled;		// 是否启用水印.
	xsd__string waterMarkFont;			// 水印字体
	xsd__int waterMarkFontSize;		// 字体大小.	定义见 cusdk__EWaterMarkFontSize.
	xsd__int waterMarkRotationAngle;	// 旋转角度
	xsd__int waterMarkColor;			// 颜色.
	xsd__int waterMarkTransparent;		// 透明度
	xsd__boolean waterMarkTiled;		// 是否平铺.
	// 视频水印功能结束. +by lzx@2017-12-20 V2R4B1
};

enum cusdk__EWaterMarkFontSize			// 视频水印的字体定义.
{
	cusdk__WaterMarkFontSizeInvalid,	// 无效
	cusdk__WaterMarkFontSizeSmall,		// 小字体
	cusdk__WaterMarkFontSizeNormal,	// 正常字体
	cusdk__WaterMarkFontSizeLarge,		// 大字体.
};

class cusdk__UserInfo
{
	xsd__string uuid;				// 用户uuid.
    xsd__string userName;			// 登录时用的用户名. lee@kedacom 
	xsd__string userPassword;
	xsd__string domainID;			// 用户所在域ID.
	enum cusdk__EUserType  userType;				// 用户类型. 操作员,管理员,超级管理员.
	std::set< enum cusdk__EUserPri > userPri;		// 用户权限. cusdk__PriUserAlarmLog及之前的权限可以放到这里.cusdk__PriUserWaterMark及之后的权限放到 userPriV2.
	std::set< int > userPriV2;						// 用户权限. +by lzx@2018-01-30 V2R4B1
	xsd__string creator;			// 创建者Id.
	xsd__string userGroup;			// 用户组Id.
	xsd__string description;		// 描述.
	time_t accountOverdueTime;	// 账户截止日期.
	time_t passwordOverdueTime;	// 密码截至日期.	
	xsd__boolean enable;					// 启用.	
	xsd__boolean allowMultilogin;			// 是否允许多次登录.
	xsd__boolean passwordExpireEnable;			//是否启用密码有效期
	xsd__int passwordExpire;						//密码有效期，单位为天	
	//std::vector < xsd__string > bindIpList;		//用户绑定Ip的列表
	xsd__boolean  bindIPEnable;             //是否启用绑定IP地址
	xsd__string bindingAddress; // 用户绑定的IP地址, 为空表示不启用或老版本CU. +by lzx@2017-07-03
	xsd__boolean IsIPAppliedToMobileEnd; //绑定的IP使用应用于移动端 +by lzx@2017-08-24

	xsd__boolean maxStreamNumEnable;	// 是否启用 maxStreamNum, true 表示限制, false表示无限制. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int maxStreamNum;				// 用户允许的最大码流数目. +by lzx@2017-12-14 V2R2B3SP1

	// 视频水印功能. +by lzx@2017-12-20 V2R4B1
	xsd__boolean waterMarkSupported;	// 是否支持水印功能. 兼容性字段, 只有启用水印功能,以下字段才有效.
	xsd__boolean waterMarkEnabled;		// 是否启用水印.
	xsd__string waterMarkFont;			// 水印字体
	xsd__int waterMarkFontSize;		// 字体大小.	定义见 cusdk__EWaterMarkFontSize.
	xsd__int waterMarkRotationAngle;	// 旋转角度
	xsd__int waterMarkColor;			// 颜色.
	xsd__int waterMarkTransparent;		// 透明度
	xsd__boolean waterMarkTiled;		// 是否平铺.
	// 视频水印功能结束. +by lzx@2017-12-20 V2R4B1

	xsd__string passwordAlgorithm;	// 密码使用的加密算法. 空 为明文, "SM3_Base64" 表示SM3取散列值后Base64编码为字符串.
};
enum cusdk__EGBDeviceGroupType
{
	cusdk__GBDeviceGroupTypeInvalid,	// 无效.
	cusdk__GBDeviceGroupTypeCivilcode,	// 行政区划.
	cusdk__GBDeviceGroupTypeVitual,		// 虚拟分组.(215/216)
};

// 设备组基本信息.
class cusdk__DeviceGroupInfo
{
	xsd__string id;			// 设备组Id.
	xsd__string parentId;	// 上级设备组Id
	xsd__string domainId;   // 设备组所在域 UUID.
	xsd__string name;		// 显示的设备组名.
	xsd__string description;// 描述.
	double      fOrderNumber; //排序序号
	xsd__string devTreeId;  //设备树根分组Id

	xsd__string originalGBId;		// 设备组的国标ID(行政区划)
	xsd__string newGBID;			// 设备组的

	xsd__int GBDeviceGroupType;		// 国标推送时分组的类型, 定义见 cusdk__EGBDeviceGroupType. +by lzx@2017-12-14 V2R2B3SP1
	xsd__int CreatorType;           // 创建者类型  枚举为 cusdk__EDeviceCreatorType
};

// 设备组完整信息.
class cusdk__DeviceGroup : public cusdk__DeviceGroupInfo
{
	xsd__boolean hasDevice;	// 设备组本级下是否有用户拥有的设备

	xsd__boolean recursiveEmpty;	// 设备组递归的所有下级是否为空.
};

class cusdk__EncoderChn
{
	xsd__int id;
//	xsd__string name;
//	enum cusdk__EVideoFormat videoFormat;			// 视频格式.
//	enum cusdk__EVideoResolution videoSolution;		// 分辨率.
};

class cusdk__DevChannel
{
    xsd__string deviceId;     //设备的唯一标识 pu uuid@domain
    xsd__int    channelId;      //通道有可能是编码通道，有可能是视频源，还有可能是放像通道, 要根据协议具体应用场景判断
};

class cusdk__VideoSrc	// 视频源.
{
	xsd__int id;									// 视频源索引，从0开始
	xsd__string name;								// 视频源别名
	//std::set< xsd__int > chnIdList;					// 视频源通道列表(主辅流)
	int ptzLevel;									// ptz控制等级.
	std::set< enum cusdk__EDevicePri > priList;	    // 权限. cusdk__EDevicePri
	std::set< int > priListV2;					// 新的权限 定义见 cusdk__EDevicePri

	// 2015-01-22 添加设备GPS信息的支持. by lizhixing@2015-01-22
	double				longitude;				// 经度
	double				latitude;				// 纬度
};

// class cusdk__AlarmInputChn	// 告警输入(并口)通道.
// {
// 	xsd__int id;		// 并口输入通道id.
// //	xsd__string name;	// 别名
// };                                                   



class cusdk__DeviceInfo	
{
	xsd__string deviceId;							// uuid.
    xsd__string domainId;							// 设备所在域.
	xsd__string name;								// 设备名称
	xsd__string deviceAlias;                        // 设备别名, 短编号.
	//考虑设备组除了按设备分外, 还可以按照视频源/告警通道等划归单位划分, 所以注释掉所属分组ID
//	xsd__string devGroupId;							// 所属设备组. 

	xsd__string deviceIP;							// 设备的IP地址. +by lizhixing@2013-10-21

	enum cusdk__EDeviceType deviceType;				// 设备类型. 编码器, 解码器, 编解码器 等.

	xsd__string deviceModel;						// 设备详细型号. 如 IPC112 等.
	std::vector< enum cusdk__EDeviceCapLabel > deviceCapLabelList;	// 设备的能力标签列表.

	xsd__string manufacturer;						// 制造商.

	int creatorType;								// 设备的创建类型 枚举为 cusdk__EDeviceCreatorType

    xsd__string description;                        // 设备描述
	std::vector< cusdk__VideoSrc > videoSrcList;	// 视频源列表.
	std::set< enum cusdk__EDevicePri > priList;	    // 设备自身的权限. cusdk__EDevicePri
	std::set< int > priListV2;					// 新的权限 定义见 cusdk__EDevicePri
	//std::vector< cusdk__AlarmInputChn > alarmInputChnList;	// 告警输入通道列表. 3A用户查询不到告警输入通道.
    xsd__int videoSrcNum;                           //视频源总个数
    xsd__int encodeChnNum;                          //编码通道个数
    xsd__int decodeChnNum;                          //解码通道个数
    xsd__int alarmInputNum;                         //并口输入总数

	xsd__int videoOutputChnNum;						// 视频输出通道数目. 

	xsd__string deviceExtInfo;						// 设备扩展信息.
};

// 设备组/设备 从属关系. ?? 有点复杂...... 不如采用靠设备组获取 设备划归信息 的方式.
class cusdk__DeviceGroupRelation
{
	xsd__string devGroupId;					// 设备组id.
	xsd__string devId;						// 设备id.
	std::vector< int > videoSrcIdList;		// 视频源列表.
	//std::vector< int > alarmInputChnList;	// 告警输入通道. 用户查不到告警输入通道.
};

// 划归用的视频源.
class cusdk__AssignVideoSrc
{
	xsd__int vieoSrcId;	// id
	std::set<  int > devPriSet;	// 视频源权限列表. 定义见 cusdk__EDevicePri
	int ptzLevel;		// ptz控制等级.
};

// 划归用的告警输入通道. 不再支持划归.
//typedef int cusdk__AssignAlarminputChn;

// 划归时使用的设备信息.
class cusdk__AssignDeviceInfo
{
	xsd__string devId;
	std::set<  int >		devPriSet;		// 设备权限列表. 定义见 cusdk__EDevicePri
	std::vector< cusdk__AssignVideoSrc >		videoSrcIdList;	// 视频源列表.
//	std::vector< cusdk__AssignAlarminputChn > alarmInputChnList;	// 告警输入通道. 不支持划归
};

// 授权设备/临时授权
class cusdk__AuthorizationDeviceInfo
{
	xsd__string userName;						// 被授权的目标用户名, 登录用的用户名.例如ligang@suzhou
	cusdk__AssignDeviceInfo devInfo;		    // 被授权的设备信息.
	xsd__string description;					// 描述.
	time_t	createTime;							// 创建时间.
};

class cusdk__DeviceChannelNameChangeNotify
{
    xsd__string deviceId;					// 设备Id.
	enum cusdk__EDeviceChannelType chnType;	// 通道类型.
	int chnId;								// 通道Id.
	xsd__string chnName;					// 通道名.
	double	longitude;						// 经度
	double	latitude;						// 纬度
};

class cusdk__DevOnlineNotify
{
    xsd__string deviceId;  //pu uuid@domain
    xsd__boolean online;
};

class cusdk__PuBaseCapSet
{
    xsd__int videoEncNum;               //视频编码通道数量;
    xsd__int videoDecNum;               //视频解码通道数量;
    xsd__int audioEncNum;               //音频编码通道数量;
    xsd__int audioDecNum;               //音频解码通道数量;
    xsd__int serialPortNum;             //串口数量;
    xsd__int alarmInputChnNum;               //并口输入数量;
    xsd__int alarmOutputChnNum;              //并口输出数量;
    xsd__int videoInPortNum;            //视频输入端口数量;
};

//为了模板批量处理map转vector，以下vector的容器类型都是id + value
class cusdk__PuExtCap
{
    xsd__int id;            // 能力集类型ID 值在cusdk__EDeviceEntity
    xsd__int value;         // 能力集类型值
};

class cusdk__AlarmInputChnEnable
{
    xsd__int id;            //并口通道
    xsd__boolean value;     //是否使能
};

class cusdk__ChannelVideoFormat
{
    xsd__int id;            //编码通道
    xsd__string value;      //编码视频格式
};

class cusdk__ChannelVidDynValue
{
    xsd__int id;            //编码通道
    xsd__int value;         //视频载荷
};

class cusdk__VideoSrcEncoderSet
{
    xsd__int id;            //视频源
    std::set<xsd__int> value;     //当前视频源正在使用的编码通道列表
};



class cusdk__EncoderAvailableVidSrcSet
{
    xsd__int id;            //编码通道
    std::set<xsd__int> value;       //编码通道对应的可用视频源
};

class cusdk__VideoSrcCamType
{
    xsd__int id;            // 视频源Id
    enum cusdk__ECameraProtocolType camType;         //摄像头类型
};

class cusdk__VideoSrcCamID
{
    xsd__int id;            //视频源
    xsd__int value;         //摄像头地址码
};

class cusdk__EncoderBitrate
{
    xsd__int id;            //编码通道
    xsd__int value;         //编码比特率
};

class cusdk__EncoderChnStatus
{
	xsd__int id;            //编码通道
	xsd__int status;        //编码通道状态 cusdk__EChnStatus
};

class cusdk__EncoderVideoRes	// 通道分辨率.
{
	xsd__int id;		// 通道id
	xsd__int width;		// 视频宽度.
	xsd__int height;	// 视频高度.
};

class cusdk__DevConfigNotify
{
    xsd__string         deviceId;             //pu uuid@domain
    xsd__string         devIp;              //前端设备IP
    xsd__string         devType;            //设备类型，编码器或者解码器
    xsd__string         devMode;            //设备型号，CU会使用它判断一些老设备，平台不会使用

    cusdk__PuBaseCapSet                         baseCapSet;     //基本能力集
    std::vector<cusdk__PuExtCap >               extCapSet;      //扩展能力集
    std::vector<cusdk__AlarmInputChnEnable>     alarmInputChnEnable; //并口输入通道使能配置
    std::vector<cusdk__ChannelVideoFormat>      chnVidFormat;   //通道当前的视频格式
    std::vector<cusdk__ChannelVidDynValue>      chnVidDynValue; //如果通道视频格式是H264，设置该通道的动态载荷
    std::vector<cusdk__VideoSrcEncoderSet >     vidSrcChnSet;   //视频源当前正在使用的通道列表
    std::vector<cusdk__EncoderAvailableVidSrcSet > chnVidSrcSet;   //通道可用的视频源列表
    std::vector<cusdk__VideoSrcCamType>         camType;        //视频源的摄像头类型
    std::vector<cusdk__VideoSrcCamID>           camId;          //视频源的摄像头地址码
    std::vector<cusdk__EncoderBitrate>          bitRate;        //通道的比特率 

	std::vector<cusdk__EncoderChnStatus>		chnStatus;		// 通道状态.
	std::vector<cusdk__EncoderVideoRes>			chnVideoRes;	// 通道的视频分辨率.

    xsd__int                            freeLicenceNum; //免费的licence点数
    xsd__int                            directTransNum; //设备支持的直传路数
    xsd__boolean                        isDecCombined;  //解码器是否合成码流
};

class cusdk__VidDecoderToPuChn
{
    xsd__int id;                //解码器视频解码通道
    cusdk__DevChannel puChn;    //播放的编码通道
};

class cusdk__AudDecoderToCuChn
{
    xsd__int id;                //编码器音频解码通道
    cusdk__DevChannel cuChn;    //音频呼叫者
};

class cusdk__EncoderStatusInt
{
    xsd__int id;                //编码通道
    xsd__boolean value;         //bool通道状态
};


class cusdk__RecordStatus
{
	xsd__int encoderChannelId;
	enum cusdk__ERecordStatus recordStatus;

	int errorCode;				// 当前录像任务最后的错误.
};

class cusdk__EncoderStatusStr
{
    xsd__int id;                //编码通道
    xsd__string value;          //string通道状态
};

class cusdk__DecoderStatusStr
{
    xsd__int id;                //解码器的解码通道
    xsd__string value;          //string通道状态
};

class cusdk__DevServiceStatusNotify
{
    xsd__string deviceId;             //pu uuid@domain
    // std::vector<cusdk__VidDecoderToPuChn>      decChnVidSrc;    //解码器播放的视频源通道（通常是PU）
    std::vector<cusdk__AudDecoderToCuChn>   audioCallSrc;    //音频呼叫的源（通常是CU）
    std::vector<cusdk__EncoderStatusInt>    isPlatPreRec;    //是否在平台预录
    std::vector<cusdk__RecordStatus>		platRecStatus;   // 平台录像状态
    std::vector<cusdk__EncoderStatusInt>    isPuPreRec;      //是否在前端预录
    std::vector<cusdk__EncoderStatusInt>    isPuRec;         //是否在前端录像
    std::vector<cusdk__DecoderStatusStr>    decPoll;         //解码器轮询状态
};

// class cusdk__DevAlarmData
// {
//     xsd__boolean    isAlarm;      //是否告警
//     xsd__string     time;         //产生时间
//     xsd__string     desc;         //告警描述
// };

// class cusdk__AlarmChannelToAlarmData
// {
//     xsd__int id;        //告警通道
//     cusdk__DevAlarmData data;
// };



// 预案类型.
// enum cusdk__ESchemeType
// {
// 	cusdk__SchemePrivate,	// 私有预案.
// 	cusdk__SchemePublic,	// 公共预案.
// };

class cusdk__DevAlarmNotify
{
    xsd__string deviceId;                     //pu uuid@domain
	enum cusdk__EAlarmType alarmType;       //告警类型, 只有
	int alarmTypeV2;						// 告警类型具体定义见 cusdk__EAlarmType, 新告警在这里.
	int  alarmChnId;						//告警输入通道
	enum cusdk__EAlarmStatus alarmStatus;	//告警标志: ALARM_STATUS_OCCUR, ALARM_STATUS_RESTORE
	int  detAreaId;							//移动侦测表示告警区域号，移动监测时有效
	time_t time;							//告警通知事件发生的时间
	xsd__string alarmData;					//告警数据

	xsd__string alarmName;					// 告警名称.

//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmVidMove;    //移动检测告警状态
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmOutPin;     //并口输出告警状态
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmInPin;      //并口输入告警状态
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmVidLost;    //视频源丢失告警状态
//     std::vector<cusdk__AlarmChannelToAlarmData>      alarmInte;       //智能告警状态
//     xsd__boolean                                     alarmDiskFull;   //磁盘满告警
};

class cusdk__StopPlayNotify
{
    xsd__int playId;        //Invite的CU播放窗口
    xsd__int errorCode;     //错误码
};



// 网络地址.
class cusdk__NetworkAddr
{
	xsd__string ip;
	xsd__int port;
};
class cusdk__NatInfo
{
	xsd__string natData;	// NAT包发送的内容.
	cusdk__NetworkAddr localAddr;	// 本机地址端口.
	cusdk__NetworkAddr natAddr;		// NAT映射后的地址端口.
};
class cusdk__MediaNetInfo
{
	cusdk__NatInfo videoRtpNatInfo;
	cusdk__NatInfo videoRtcpNatInfo;
	cusdk__NatInfo audioRtpNatInfo;
	cusdk__NatInfo audioRtcpNatInfo;
};
class cusdk__MediaFormat
{
    xsd__int payloadType;       // 编码格式ID，具体定义见kdvdef.h
	xsd__int encodingType;		// 编码格式, 具体定义见?
    xsd__string encodingName;   // 编码格式名，对应于payloadType的名称
    xsd__string manufacture;    // 媒体流厂商
	xsd__string mediaStreamType;// 媒体流格式. "ES", "PS", "TS", "RTP_ES", "RTP_PS", "RTP_TS" 等.
	
	xsd__int bitRateType;       // 码率类型, 可变或者固定码率.1：固定码率（CBR）；2：可变码率（VBR）
	xsd__int bitRate;			// 码率 单位 Kbps

	xsd__int keyframeInterval;	// 关键帧间隔, 帧的个数.
	xsd__int frameRate;			// 帧率. 单位为 fps
	xsd__int videoWidth;		// 分辨率宽度.
	xsd__int videoHeight;		// 分辨率高度.

	xsd__int clockRate;			// 时间戳采样率.

	xsd__int samplingRate;                      // 真实采样率	比如，16000、44100等
	xsd__int audioChannels;                     // 声道数 比如，1=单声道，2=双声道
};

class cusdk__OrderMediaFormat
{
	bool needTransMediaFormat;        //是否指定转码格式, FALSE表示不转码，TRUE表示转码
	//目标默认为kedacom的编码格式, 不支持转为外厂商格式
	enum cusdk__EVideoFormat dstVideoFormat; //目标视频格式，H264 or MPEG4
	enum cusdk__EVideoResolution dstSolution;    //目标分辨率，默认D1
	xsd__int dstFrameRate;  //目标视频帧率，默认25 fps
	xsd__int dstBitRate;    //目标码率，默认800 kbps
	enum cusdk__EAudioFormat dstAudioFormat; //目前暂不支持
};

class cusdk__TranscodeMediaFormat
{
	bool needTranscode;        //是否需要转码.

	//目标默认为kedacom的编码格式, 不支持转为外厂商格式
	enum cusdk__EVideoFormat dstVideoFormat;	//目标视频格式
	enum cusdk__EVideoResolution dstSolution;   //目标分辨率，默认D1
	xsd__int dstFrameRate;  //目标视频帧率
	xsd__int dstBitRate;    //目标码率

	enum cusdk__EAudioFormat dstAudioFormat; //目前暂不支持音频转码.
};

// 分区信息。
class cusdk__PartitionInfo
{
	int diskId;				// 所属磁盘Id.
	int partitionId;		// 分区Id.
	xsd__string partName;	// 分区名.
	int totalSize;			// 总空间, 单位 MB
	int freeSize;			// 空闲空间, 单位 MB.
	enum cusdk__ERS_PartState partState;	// 分区状态.
	xsd__string partType;	// 分区格式( vbfs, ext3 )
	int abnNum;				// 异常次数.
	xsd__string mountPath;	// 挂在路径.
	int start;				// 起始块位置.
	int end;				// 结束块位置
	int recordNum;			// 录像数.
};

class cusdk__DiskInfo
{
	int diskId;				// 磁盘Id.
	xsd__string diskName;	// 磁盘名.
	int diskSize;			// 容量. 单位MB.
	int diskAbnNum;			// 异常次数.
	enum cusdk__ERS_DiskState diskState;	// 磁盘状态.
	xsd__string diskSource;	// 来源.
	int cylinders;			// 柱面数.
	int heads;				// 磁头数.
	int sectors;			// 扇区数目

	std::vector< cusdk__PartitionInfo > partitionList;	// 分区列表.
};

// nru信息。
class cusdk__NruInfo
{
	xsd__string nruId;			// nru id。
	xsd__string devName;		//设备名
	xsd__string devType;		//设备类型:VS200,KDM2801E等
	xsd__string manuFactory;	//设备厂商

	xsd__string ifName;		// 网卡名.
	xsd__string ipAddr;		// IP地址.
	xsd__string netMask;	// 子网掩码.
	xsd__string gateway;	// 网关.

	xsd__string RMSAddr;	// 注册RMS服务器地址.
	int maxBandWidthKb;		// NRU的最大数据带宽, 单位 Kb/S.
	bool online;			// 是否在线.
	bool used;				// 是否启用.
};

// 时间段。
class cusdk__TimeRange
{
	time_t         startTime;
	time_t         endTime;
};

// 录像时间段
class cusdk__RecTimeRange
{
	enum cusdk__ERecordEventType        recEventType;   /*录像事件类型：手动录像、计划录像 or 告警录像*/
	cusdk__TimeRange      timeRange;             /*录像时间范围*/
};

// 录像结果时间段
class cusdk__RecDuration
{
	xsd__string recordDurationId;		// 录像标识(Token)。
	cusdk__TimeRange durationTimeRange;	// 录像总的时间段
	std::vector< cusdk__RecTimeRange > subTimeRangeList;	// 子录像（连续录像中的不同的录像段的类型）
	xsd__string recordDomainName;		// 录像所在域名，填空为本域。

	cusdk__DevChannel encoderChn;		// 录像的编码器通道.

	xsd__boolean locked;				// 录像是否被锁定. +by lzx@2017-09-18
	time_t	lockExpireTime;				// 录像锁定的失效时间. +by lzx@2017-09-18
};

// 录像播放/下载时的参数.
class cusdk__RecordPlayDuration
{
	xsd__string recordDurationId;		// 录像标识(Token)。
	enum cusdk__ERecordType recordType;	// 前端录像/平台录像.

	cusdk__TimeRange durationTimeRange;	// 录像总的时间段	
	xsd__string recordDomainName;		// 录像所在域名，填空为本域。 

	cusdk__DevChannel encoderChn;		// 录像的编码器通道.
};


class cusdk__OnePuChnRecDays
{
	cusdk__DevChannel     puChn;
	cusdk__TimeRange	  timeRange;								// 时间段。
	std::vector<bool>     recordStateInDays;                 // 从时间段里的第一天开始， 有录像的天置为true
};

// class cusdk__CronScedule
// {
// 	xsd__string cronStart;	// 开始的Cron 时间。
// 	int duration;					// 持续多长时间，单位秒。
// };

class cusdk__RecordPlan
{	
	int prerecordDuration;	// 预录时长. 0 表示不预录.
	enum cusdk__ERS_RecMode recordMode;		// 录像模式.
	std::vector< xsd__string > planedRecordCronTimeList;	// 录像计划的开启时间点。

	int recordDurationAfterAlarm;	// 告警恢复后平台录像时间, 单位秒.
	int recStorageDuration; //录像存储时长（>=1天），0-系统默认
	bool recDuraionEnabled; //当前是否启用录像存储时长设置
	bool supportSetRecDuration; //是否支持设置录像存储时长
};

class cusdk__TVDecoderBind
{
	int tvId;		// 电视序号
	xsd__string decoderId;	// 解码器设备id.

	int decoderOutputId;	// 解码器设备视频输出通道Id.
};

class cusdk__TVWall
{
	xsd__string tvwallId;			// 电视墙编号
	xsd__string domainId;		// 电视墙所属域编号
	xsd__string name;				// 电视墙名称
	int tvNum;						// 电视机数目
	int tvwallStyle;					// 电视墙风格, 不使用枚举，因为TAS不关心具体的内容，否则到时风格类型添加时TAS也要改了
	xsd__string tvwallCustomStyleData; // 电视墙自定义风格数据
	xsd__string desc;          // 电视墙描述
	time_t createTime;		// 入网日期

	std::vector< cusdk__TVDecoderBind > tvDecoderBind; // 电视与解码器绑定关系
};

// 窗口轮巡的一步
class cusdk__TVWallWindowPollStep
{
	cusdk__DevChannel encoderChn;	// 编码器/IPC 等视频源通道.
	int duration;					// 该视频源的码流持续时间.
};

// 电视上的单个子窗口(画面合成)的预案.
class cusdk__TVWALLTVDivScheme
{
	int divId;	// 解码器通道id.
	std::vector< cusdk__TVWallWindowPollStep > pollSteps;	// 轮巡的各个步骤. 如果只有一个轮巡, 则是普通浏览.
};

// 单个电视的预案.
class cusdk__TVWallTVScheme
{
	int tvId;			// 电视Id
	enum cusdk__ETVDivStyle divStyle;	// 窗口的画面合成分画面风格.	
	std::vector< cusdk__TVWALLTVDivScheme > tvWallDivList;	// 电视的画面合成的小窗口.	
};


// 电视墙的预案.
class cusdk__TVWallScheme
{
//	xsd__string schemeId;	// 预案Id.
	xsd__string schemeName;	// 预案名称.
	xsd__string tvWallId;	// 预案所属电视墙Id.
//	enum cusdk__ESchemeType schemeType;	// 公共预案还是私有预案.
	std::vector< cusdk__TVWallTVScheme > tvSchemeList;	// 电视墙上所有电视的预案列表.
};


// 预案轮巡.
class cusdk__TVWallSchemePollStep
{
	xsd__string tvWallSchemeName;	// 预案名
	int duration;					// 预案切换前的保持时间.
};


// 电视墙的一个画面合成子窗口的状态.
class cusdk__TVWallTVDivState
{
	//	int tvId;	// 电视Id.
	int divId;	// 画面合成子窗口(通道)Id.

	enum cusdk__ETVWallPlayMode divPlayMode;	// 电视墙分画面的播放模式. ( 实时浏览, 平台录像, 前端录像 ).
	xsd__string recordPlaySession;	// 录像播放的Session.
	cusdk__RecordPlayDuration recordPlayDuration;	// 当前播放的录像时间段.

	enum cusdk__EPollStatus pollState;	// 窗口轮巡的状态.	

	cusdk__DevChannel encoderChn;	// 当前浏览的编码器通道.
	xsd__int errorCode;		// 当前浏览请求出现的错误, 错误码.
};

// 电视墙上的一个电视的状态.
class cusdk__TVWallTVState
{
	int tvId;	// 电视Id.
	std::vector< cusdk__TVWallTVDivState > divStateList;	// 画面合成子画面状态. 没有画面合成的此项只有一项.
};

class cusdk__TVWallStateNotify
{
	xsd__string tvWallId;	// 电视墙Id.

	enum cusdk__EPollStatus schemePollState;				// 预案轮巡的状态.
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// 电视墙预案轮巡的步骤.

	cusdk__TVWallScheme curSchemeInfo;						// 当前电视墙的临时预案.(可能是保存过的, 也可能是未保存的临时预案)

	std::vector< cusdk__TVWallTVState > tvStateList;		// 电视墙上的电视的状态.
};

class cusdk__DomainInfo
{
	xsd__string domainId;	// 域Id
	xsd__string domainName;	// 域名
	xsd__string parentDomainId;	// 上级域Id.
    xsd__int    status;         // 域状态cusdk__EDomainStatus
};


class cusdk__TransChannel
{
	enum cusdk__ETransChannelType transChannelType;	// 传输通道的类型.

	xsd__string natData;	// NAT包发送的内容.( 只在UDP模式下有效 )

	std::vector< cusdk__NetworkAddr > netAddrList;	// 网络地址列表, 内网地址在前, 公网(NAT)地址在后.

	//2013-11-01 平台未实现，暂不修改// 2013-09-10 后平台需要将本机地址和NAT映射地址分开.
// 	std::vector< cusdk__NetworkAddr > localAddrList;	// 本机地址端口.
// 	std::vector< cusdk__NetworkAddr > natAddrList;		// NAT映射后的地址端口.
};

class cusdk__MediaDescription
{
	enum cusdk__ETransMode transMode;	// TCP/UDP
	enum cusdk__EMediaType mediaType;  //媒体类型：VIDEO，AUDIO，DATA

	std::vector< cusdk__TransChannel > transChannelList;  // 媒体传输通道。UDP传输时，通常是RTP与RTCP通道；数据传输或TCP传输时，单通道就可以	

	std::vector< cusdk__MediaFormat > supportedFormats;	// 能支持的视频编码格式. 还需要扩展. 分辨率, 码率等必要的编码参数信息, 参考标准.
	std::vector< enum cusdk__EVideoResolution > supportedResolution;	// 能支持的视频分辨率. 只在视频时有效.

};

// 媒体扩展信息.
class cusdk__MediaExtInfo
{
	xsd__string extInfoName;	
	xsd__string extInfoValue;
};

class cusdk__BaseReq
{
	xsd__string session;
};
//
//class cusdk__BaseRsp
//{
//	xsd__string session;	// 冗余 , 为了查问题方便过滤.
//	long errorCode;
//};




/*************************************************
具体方法定义
**************************************************/
class cusdk__DetectServerReq
{
};

class cusdk__DetectServerRsp /* : public cusdk__BaseRsp */
{
	xsd__string domainName;		// 平台域名
	
	xsd__string cuNatIp;        // CU的Nat地址.
	xsd__int    cuNatPort;		// CU的Nat端口.
	
	xsd__string session;		// 冗余 , 为了查问题方便过滤.
	long errorCode;

};

//gsoap cusdk service method-documentation: 探测服务器是否正常提供服务. .
int cusdk__DetectServer( cusdk__DetectServerReq detectReq, cusdk__DetectServerRsp &detectRsp );

class cusdk__STUNServerInfo
{
    xsd__string ip;        //整个系统配置的全局唯一的STUN服务器
    xsd__int    port;
};

enum cusdk__ENATMode
{
	cusdk__NATModeAuto,		// 自动模式,由服务端自动检测
	cusdk__NATModeInner,	// 位于NAT里面,经过地址映射.
	cusdk__NATModeOutside,	// 位于NAT外面,无需地址映射.
};

class cusdk__LoginReq
{
	xsd__string userName;
	xsd__string password;	
	xsd__string cusdkVersion;	// 客户端使用sdk的版本.

	xsd__string clientType;		// 客户端类型,(选填,默认为CU).

	xsd__boolean isSupportNA;	// 是否支持NA库.

	xsd__string webCUClientAddr;	// WebCU客户端地址(WebCU通过平台服务器本机的WebCU服务端登录CUI,网页地址需要WebCU服务端提供.)

	xsd__boolean isSupportSecure;	// CU是否支持安全模式.

	xsd__string passwordAlgorithm;	// 密码使用的加密算法. 空 为明文, "SM3_Base64" 表示SM3取散列值后Base64编码为字符串.
};

class cusdk__LoginRsp /* : public cusdk__BaseRsp */
{
//	xsd__string session;
	xsd__string cmsVersion;     // 平台版本, 用于CU判断升级
	xsd__string cuVersion;		// 平台上的客户端版本.
	xsd__string cuUpdateUrl;	// CU升级包的URL.
	cusdk__STUNServerInfo stunInfo;
	xsd__string serverDataVersion; // 服务端的数据版本, 用于本地数据缓存优化.
    xsd__int    userLockRemainTime;    //用户锁定剩余时间，单位为秒
	xsd__int    passwordRemainTime;   //密码距离到期还剩余的时间，单位为秒。为0和负值时，表示已经过期

	xsd__boolean isSecureEnabled;	// 平台是否启用的安全模式.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};

//gsoap cusdk service method-documentation: 登录 .
int cusdk__Login( cusdk__LoginReq loginReq, cusdk__LoginRsp &loginRsp );

class cusdk__LogoutReq : public cusdk__BaseReq
{
};
class cusdk__LogoutRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};

//gsoap cusdk service method-documentation: 登出 .
int cusdk__Logout( cusdk__LogoutReq logoutReq, cusdk__LogoutRsp& logoutRsp );


class cusdk__NotifySubscription
{
	enum cusdk__ESubscribeNotifyType subscribeNotifyType;	// 通知订阅的类型.

	xsd__string deviceId;									// 订阅的相关设备URI.
	std::set<enum cusdk__EPuSubscribeType> subscribeTypes;	// 设备状态订阅的子类型.

	xsd__string deviceTreeId;								// 订阅的设备树ID, 只在订阅类型是cusdk__SubscribeDeviceGroupStat时有效.
	xsd__string deviceGroupId;								// 订阅的设备组ID, 只在订阅类型是cusdk__SubscribeDeviceGroupStat时有效.
};

class cusdk__SubscribeNotifyReq : public cusdk__BaseReq
{
	std::vector< cusdk__NotifySubscription > subscriptionList;
};
class cusdk__SubscribeNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__NotifySubscription > failedList;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};


//gsoap cusdk service method-documentation: 通知订阅 .
int cusdk__SubscribeNotify( cusdk__SubscribeNotifyReq req, cusdk__SubscribeNotifyRsp& rsp );

class cusdk__UnsubscribeNotifyReq : public cusdk__BaseReq
{
	std::vector< cusdk__NotifySubscription > subscriptionList;
};
class cusdk__UnsubscribeNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__NotifySubscription > failedList;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};

//gsoap cusdk service method-documentation: 取消状态订阅 .
int cusdk__UnsubscribeNotify( cusdk__UnsubscribeNotifyReq req, cusdk__UnsubscribeNotifyRsp& rsp );

class cusdk__DeviceFullInfo : public cusdk__DeviceInfo
{
    std::vector< cusdk__DeviceGroupRelation > devGroupToChannel;    //设备组和通道的对应关系，其中devId为空
};

// 告警联动动作.
class cusdk__AlarmLinkAction
{
	std::vector< enum cusdk__EAlarmLinkActionType > actionType;	// 联动动作的类型, 可以多项.
	cusdk__DevChannel encoderChn;	// 告警联动的编码通道.

	//int recordDurationAfterAlarm;	// 告警恢复后平台录像时间.

	int camPrePos;	// 摄像头预置位

	// 电视墙
	xsd__string tvWallId;	// 电视墙Id.
	int tvId;				// 电视序号.
	int tvDivId;			// 电视分画面序号.

	// 告警输出.
	int alarmIOOutputChnId;	// 告警输出通道Id.

	// 
	// 	std::vector< xsd__string > SMSPhoneNumList;		// 短信电话号码列表.
	// 	std::vector< xsd__string > voicePhoneNumList;	// 声讯通知号码列表.
	// 	std::vector< xsd__string > mailList;			// 邮箱地址列表.
};

// 告警源.
class cusdk__AlarmSource
{
	xsd__string alarmDeviceId;			// 告警设备Id.
	int alarmInputChnId;				// 产生告警的通道Id. 不同告警类型下的含义不同.
	enum cusdk__EAlarmType alarmType;	// 告警类型.
};

// 告警联动配置
class cusdk__AlarmLinkConfig
{
	cusdk__AlarmSource alarmSource;	// 告警源

	//	xsd__string alarmName;			// 告警别名.

	bool enabled;	// 是否启用.
	std::vector< xsd__string > enabledCronTimeList;	// 启用告警布防的时间段.

	std::vector< cusdk__AlarmLinkAction > alarmLinkActionList;	// 告警联动动作列表.	
};

// 导出失败通知.
class cusdk__ExportFail
{
	xsd__string exportFileUrl;	// 导出的Url.
	int errorCode;				// 导出的失败原因.
};

// 服务类型
enum cusdk__EServiceType
{
	/*录像功能（RCS）：告警功能（ALS）：电视墙功能（TVS）：电子地图功能（MPS）：
	日志功能（UAS）：平台配置存储服务（UAS 包括用户自定义数据和前端配置数据）*/
	cusdk__ServiceRecord,
	cusdk__ServiceAlarmLink,
	cusdk__ServiceTVWall,
	cusdk__ServiceMap,
	cusdk__ServiceLog,
	cusdk__ServiceConfig,
};
// 服务状态.
class cusdk__ServiceStatus
{
	enum cusdk__EServiceType serviceType;
	bool serviceOk;
};

// GPS信息
class cusdk__GPSInfo	// 参考大华的.
{
	time_t				revTime;				// 定位时间
	xsd__string			deviceId;				// 设备序列号
	double				longitude;				// 经度
	double				latitude;				// 纬度

	double				marLongitude;			// 加偏后的经度.
	double				marLatitude;			// 加偏后的纬度.

	double				height;					// 高度(米)
	double				angle;					// 方向角(正北方向为原点，顺时针为正)
	double				speed;					// 速度(公里/小时)
//	int					starCount;				// 定位星数
//	xsd__boolean		antennaState;			// 天线状态(true 好，false 坏)
//	xsd__boolean		orientationState;		// 定位状态(true 定位，false 不定位)
};

// 设备透明数据通知.
class cusdk__TransDataNotify
{
	xsd__string deviceId;	// 设备Id.
	xsd__int chnId;			// 设备通道ID.

	xsd__int transChnType;	// 透明通道类型. 见enum cusdk__EDeviceTransparentChannelType

	xsd__int dataMsgType;	// 透明通道协议中具体规定.
	xsd__boolean isEnd;				// 是否是最后一条通知.
	xsd__string transDataBase64;	// 数据,base64编码.
    xsd__string dataMsgTypeUserDef;    //类型，transChnType为cusdk_TransChnUserDef时，实际类型在这里定义

};

//设备推送进度通知
class cusdk__DevicePushProgressNotify
{
	xsd__int pos;              //当前进度，正常进度是0~100，返回-1代表出错 
	xsd__string errorDsc;      //错误描述            
};

// 平台通知消息.
class cusdk__CMSMessageNotify
{
	time_t messageTime;			// 事件发生时间.
	xsd__string messageType;	// 通知消息类型
	xsd__string messageContent;	// 通知消息内容
};

// 开始编码传输通知.
class cusdk__StartEncoderTransNotify
{
	xsd__int encodeChnId;	// 客户端的音视频编码通道Id. 

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 协商后的媒体信息描述. 如果为空就表示不需要创建码流.
	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// 媒体扩展信息.
};

// 设备组统计信息通知.
class cusdk__DeviceGroupStatNotify
{
	xsd__string treeId;		// 设备树Id
	xsd__string groupId;	// 设备组Id
	xsd__boolean recursive;	// 是否递归统计.

	int totalNum;			// 总数
	int onlineNum;			// 在线数目
	int errorCode;			// 统计过程中出错的错误码
};

class cusdk__GetNotifyReq : public cusdk__BaseReq
{
	xsd__int timeoutMSec;	// 超时时间, 单位毫秒. 为0表示立即返回, 否则会在有通知消息后或超时后返回.
};
class cusdk__GetNotifyRsp /* : public cusdk__BaseRsp */
{
    std::vector< cusdk__DeviceFullInfo > deviceAddNotify;       //1 设备添加通知.
    std::vector< cusdk__AssignDeviceInfo > deviceDelNotify;     //2 设备 或 监控点删除通知. 
    std::vector< cusdk__DeviceFullInfo > deviceModNotify;       //3 设备修改通知.

    std::vector< cusdk__DeviceChannelNameChangeNotify> deviceChannelNameChangeNotify;   //4 设备通道别名修改通知.

    std::vector< cusdk__DeviceGroupInfo > deviceGroupAddNotify;     //5 设备组添加通知.
    std::vector< xsd__string > deviceGroupDelNotify;            //6 设备组删除通知.
    std::vector< cusdk__DeviceGroupInfo > deviceGroupModNotify;     //7 设备组修改通知.

	std::vector< cusdk__DevOnlineNotify > devOnlineNotify;      //8 设备上下线通知.
    std::vector< cusdk__DevConfigNotify > devConfigNotify;      //9 设备配置修改通知.
    std::vector< cusdk__DevServiceStatusNotify > devServiceStatusNotify;	//10 设备 平台录像/前端录像/呼叫/轮巡(解码器)等服务状态.
    std::vector< cusdk__DevAlarmNotify > devAlarmNotify;        //11 设备告警通知.

    std::vector< cusdk__StopPlayNotify > stopPlayNotify;        //12 码流异常中断通知.

	std::vector< cusdk__TVWallStateNotify > tvWallStateNotify;	//13 电视墙状态通知.

	std::vector< cusdk__UserInfo > userAddNotify;			//14 添加用户通知.
	std::vector< xsd__string > userDelNotify;				//15 删除用户通知.
	std::vector< cusdk__UserInfo > userModNotify;			//16 修改用户通知.

	std::vector< cusdk__UserGroup > userGroupAddNotify;     //17 添加用户组通知.
	std::vector< xsd__string > userGroupDelNotify;          //18 删除用户组通知.
	std::vector< cusdk__UserGroup > userGroupModNotify;     //19 修改用户组通知.

	std::vector< cusdk__TVWall > tvWallAddNotify;     //20 添加电视墙通知.
	std::vector< xsd__string > tvWallDelNotify;       //21 删除电视墙通知.
	std::vector< cusdk__TVWall > tvWallModNotify;     //22 修改电视墙通知.

	std::vector< cusdk__DeviceGroupRelation > deviceGroupAddDeviceNotify;     //23 设备组中划归设备通知.
	std::vector< cusdk__DeviceGroupRelation > deviceGroupDelDeviceNotify;     //24 设备组中删除设备通知.

	std::vector< cusdk__AlarmLinkConfig > alarmLinkConfigChangeNotify;		// 25 告警联动配置更新通知.

	std::vector< cusdk__STUNServerInfo > stunChangedNotify;		// 26 stun地址配置更新通知.
	std::vector< cusdk__ExportFail > exportFailNotify;		// 27 导出失败通知.

	std::vector< cusdk__ServiceStatus > serviceStatusNotify;	// 28 平台服务.

	std::vector< cusdk__GPSInfo > gpsNotify;	// 29 GPS信息通知.

	std::vector< cusdk__TransDataNotify > transDataNotify;	// 30 透明数据通知.

	std::vector< cusdk__DevicePushProgressNotify > devPushProgressNotify;  // 31设备推送通知

	std::vector< cusdk__CMSMessageNotify > cmsMessageNotify;	// 32 平台服务器通知消息.

	std::vector< cusdk__StartEncoderTransNotify > startEncoderTransNotify;	// 33 平台要求CU开始编码传输(呼叫).

	std::vector< cusdk__DeviceGroupStatNotify > deviceGroupStatNotify;		// 34 设备组统计通知.

    std::vector< xsd__int > seqMarks;   //通知消息的序列标记，标记从1开始，标识数据成员出现的位置
    xsd__boolean hasDataLeft;           //是否还有剩余通知数据

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取通知. 兼作心跳保活功能.
int cusdk__GetNotify( cusdk__GetNotifyReq req, cusdk__GetNotifyRsp& rsp );


class cusdk__GetCUIListReq : public cusdk__BaseReq
{
};
class cusdk__GetCUIListRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > cuiList;	    //返回每个CUI的URL, 如果端口不为80, 还要带上端口信息

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取所有CUI地址列表, 用于多路抗毁和负载均衡.
int cusdk__GetCUIList( cusdk__GetCUIListReq req, cusdk__GetCUIListRsp& rsp );


class cusdk__GetDomainListReq : public cusdk__BaseReq
{
};
class cusdk__GetDomainListRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DomainInfo > domainList;	    // 返回域列表

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取所有域.
int cusdk__GetDomainList( cusdk__GetDomainListReq req, cusdk__GetDomainListRsp& rsp );


class cusdk__SetupStreamReq : public cusdk__BaseReq
{
	xsd__int playId;

	xsd__string devId;
	xsd__int devChnId;

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 浏览请求方的媒体信息描述.

	cusdk__TranscodeMediaFormat transcodeMeidaFormat;		// 要求平台转码

	xsd__boolean directTrans;								// 是否直传, 由前端直接发出.
	xsd__boolean directTransFromDeviceDomain;				// 直传, 由设备所在域发出码流.

// 	enum cusdk__EPlayMode playMode;	        // Video/Audio/Both
 	int transMode;		// 定义参见 cusdk__ETransMode
// 	std::vector<cusdk__MediaNetInfo> localNetInfoList;  //之所以用vector, 考虑CU多IP情况
// 	cusdk__OrderMediaFormat mediaInfo;	// 要求平台转码.
};
class cusdk__SetupStreamRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 服务器方的媒体信息描述.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// 媒体扩展信息.
// 	cusdk__MediaFormat videoFormat;
// 	cusdk__MediaFormat audioFormat;
// 	std::vector<cusdk__MediaNetInfo> remoteNetInfoList; //平台也多IP情况

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 申请码流资源, 创建交换.
int cusdk__SetupStream( cusdk__SetupStreamReq req, cusdk__SetupStreamRsp& rsp );

class cusdk__PlayStreamReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__PlayStreamRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始发送码流.
int cusdk__PlayStream( cusdk__PlayStreamReq req, cusdk__PlayStreamRsp& rsp );

class cusdk__StopStreamReq : public cusdk__BaseReq
{
	xsd__int playId;	
};
class cusdk__StopStreamRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 停止码流.
int cusdk__StopStream( cusdk__StopStreamReq req, cusdk__StopStreamRsp& rsp );

class cusdk__SendKeyFrameReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int encoderChannelId;
};
class cusdk__SendKeyFrameRsp /* : public cusdk__BaseRsp */
{
	xsd__string errorDesc;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 强制前端设备发送关键帧.
int cusdk__SendKeyFrame( cusdk__SendKeyFrameReq req, cusdk__SendKeyFrameRsp& rsp );

class cusdk__PtzCtrlReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;
	enum cusdk__EPtzCmd ptzCmd;
	std::vector< int > ptzParam;
};
class cusdk__PtzCtrlRsp /* : public cusdk__BaseRsp */
{
    xsd__string errorDesc;

	xsd__string userName;	//锁定PTZ的用户名（锁定成功值为空）
	xsd__int	ptzLevel;	//锁定PTZ的用户ptz控制等级（锁定成功为空）
	time_t		beginTime;	//锁定开始时间
	time_t		endTime;	//自动解锁时间

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZ控制.
int cusdk__PtzCtrl( cusdk__PtzCtrlReq req, cusdk__PtzCtrlRsp& rsp );

class cusdk__PtzLockReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;
	time_t beginTime;		// 锁定开始时间.
	time_t endTime;		    // 自动解锁时间.
	xsd__string password;	// 手动解锁密码.
};
class cusdk__PtzLockRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	xsd__string userName;	//锁定PTZ的用户名（锁定成功值为空）
	xsd__int	ptzLevel;	//锁定PTZ的用户ptz控制等级（锁定成功为空）
	time_t		beginTime;	//锁定开始时间
	time_t		endTime;	//自动解锁时间
	long errorCode;			//错误码

};
//gsoap cusdk service method-documentation: PTZ锁定.
int cusdk__PtzLock( cusdk__PtzLockReq req, cusdk__PtzLockRsp& rsp );

class cusdk__PtzUnlockReq : public cusdk__BaseReq
{
	xsd__string devId;
	xsd__int videoSrcId;	
	xsd__string password;	// 解锁密码.
};
class cusdk__PtzUnlockRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	xsd__string	userName;	//锁定PTZ的用户名（解锁成功值为空）
	xsd__int	ptzLevel;	//锁定PTZ的用户ptz控制等级（解锁成功值为空）
	time_t		beginTime;	//锁定开始时间
	time_t		endTime;	//锁定解锁时间
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZ锁定.
int cusdk__PtzUnlock( cusdk__PtzUnlockReq req, cusdk__PtzUnlockRsp& rsp );

class cusdk__PtzUnlockAllReq : public cusdk__BaseReq
{
};
class cusdk__PtzUnlockAllRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: PTZ全部解锁.
int cusdk__PtzUnlockAll( cusdk__PtzUnlockAllReq req, cusdk__PtzUnlockAllRsp& rsp );

class cusdk__SendTransDataReq : public cusdk__BaseReq
{
	enum cusdk__EDeviceTransparentChannelType transChnType;	// 透明通道类型.

	xsd__string devId;			// 设备Id.
	xsd__int channelId;			// 设备通道Id.

	xsd__int dataMsgType;			// 数据类型。
	xsd__string base64TransDataReq;	// 使用base64标准编码的透明数据.
    xsd__string dataMsgTypeUserDef;    //用户自定义类型实际携带的数据类型，dataMsgType为cusdk__TransChnUserDef时，使用该字段传输

};
class cusdk__SendTransDataRsp /* : public cusdk__BaseRsp */
{
	xsd__int dataMsgType;			// 返回的数据类型。
	xsd__string base64TransDataRsp;	// 返回的base64标准编码的透明数据.

	xsd__boolean hasNotify;			// 是否有Notify通知. cusdk__TransDataNotify.

	// PTZ锁定原因造成的失败.
	xsd__string errorDesc;

	xsd__string userName;	//锁定PTZ的用户名（锁定成功值为空）
	xsd__int	ptzLevel;	//锁定PTZ的用户ptz控制等级（锁定成功为空）
	time_t		beginTime;	//锁定开始时间
	time_t		endTime;	//自动解锁时间

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
    xsd__string dataMsgTypeUserDef;    //用户自定义类型实际携带的数据类型，dataMsgType为cusdk__TransChnUserDef时，使用该字段传输


};
//gsoap cusdk service method-documentation: 发送透明数据.
int cusdk__SendTransData( cusdk__SendTransDataReq req, cusdk__SendTransDataRsp& rsp );


class cusdk__GetUserInfoReq : public cusdk__BaseReq
{
	xsd__string userName;
};
class cusdk__GetUserInfoRsp /* : public cusdk__BaseRsp */
{
	cusdk__UserInfo userInfo;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取当前登录CU的用户信息, 仅用于CU自身登录用户.
int cusdk__GetUserInfo( cusdk__GetUserInfoReq req, cusdk__GetUserInfoRsp& rsp );


class cusdk__GetUserGroupInfoReq : public cusdk__BaseReq
{
	xsd__string userGroupId;	// 用户组id.
};
class cusdk__GetUserGroupInfoRsp /* : public cusdk__BaseRsp */
{
	cusdk__UserGroup userGroupInfo;	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取当前登录CU的用户组信息, 仅用于CU自身登录用户.
int cusdk__GetUserGroupInfo( cusdk__GetUserGroupInfoReq req, cusdk__GetUserGroupInfoRsp& rsp );

class cusdk__GetUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// 自定义数据键值 用户id + 此key值作为唯一标识
	int dataType;		// 数据类型.
	int fragNo;			// 分块号码. ( 因为数据可能超过一个包的最大大小, 所以会分包 )
};
class cusdk__GetUserDataRsp /* : public cusdk__BaseRsp */
{
	xsd__string key;	// 自定义数据键值 用户id + 此key值作为唯一标识 冗余数据,查问题方便
	int dataType;		// 数据类型.
	int fragNo;			// 分块号码. ( 因为数据可能超过一个包的最大大小, 所以会分包  冗余数据,查问题方便 )

	int allFragNum;		// 分片总数.

	xsd__string value;	// 自定义数据. 这个可能很大, 比如自定义设备树

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取用户自定义数据.
int cusdk__GetUserData( cusdk__GetUserDataReq req, cusdk__GetUserDataRsp& rsp );

class cusdk__SetUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// 自定义数据键值
	int dataType;		// 数据类型.

	int fragNo;			// 分块号码. ( 因为数据可能超过一个包的最大大小, 所以会分包
	int allFragNum;		// 分片总数.

	xsd__string value;	// 自定义数据.
};
class cusdk__SetUserDataRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置用户自定义数据.
int cusdk__SetUserData( cusdk__SetUserDataReq req, cusdk__SetUserDataRsp& rsp );

class cusdk__DelUserDataReq : public cusdk__BaseReq
{
	xsd__string key;	// 自定义数据键值
	int dataType;		// 数据类型.
};
class cusdk__DelUserDataRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除用户自定义数据.
int cusdk__DelUserData( cusdk__DelUserDataReq req, cusdk__DelUserDataRsp& rsp );

// 设备组/设备数据的文件导出接口.
class cusdk__StartExportDeviceDataReq : public cusdk__BaseReq
{	
};

class cusdk__StartExportDeviceDataRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// 导出操作的Url.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导出用户的域/设备组/设备 数据.
int cusdk__StartExportDeviceData( cusdk__StartExportDeviceDataReq req, cusdk__StartExportDeviceDataRsp& rsp );

class cusdk__StopExportDeviceDataReq : public cusdk__BaseReq
{	
};

class cusdk__StopExportDeviceDataRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消数据导出,删除文件.
int cusdk__StopExportDeviceData( cusdk__StopExportDeviceDataReq req, cusdk__StopExportDeviceDataRsp& rsp );



class cusdk__GetUserDevGroupByGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// 用户Id. 当此ID不为空时，查询的设备组信息中hasDev是该用户的, 否则是设备组的
	xsd__string parentDevGroupId;// 父设备组Id. 当此ID为空时, 查询根节点下的子设备组，不为空时查询指定父组下的设备组，每次仅查询一级
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
	xsd__string devTreeId;  //设备树根分组Id
};
class cusdk__GetUserDevGroupByGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // 设备列表.
	int totalNum;										// 满足条件的设备组的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 按照用户/设备组的组合来获取子一级设备组列表.为CU界面上展开设备树才构建设备子树的树节点时用到此接口.若二次开发不关心设备树,不用调用
int cusdk__GetUserDevGroupByGroup( cusdk__GetUserDevGroupByGroupReq req, cusdk__GetUserDevGroupByGroupRsp& rsp );


class cusdk__GetUserDevByGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// 用户Id.
	xsd__string devGroupId;	// 设备组Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;		// 本次请求要的总数.
	xsd__string devTreeId; //设备树Id

	xsd__string beginQueryHint;	// 查询的起始点, 查询标志, 用于分页查询. 当beginToken为空时, 按数量和序号分页. 

};
class cusdk__GetUserDevByGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // 设备列表.
	int totalNum;								        // 满足条件的设备的总数.

	xsd__string nextQueryHint;	// 查询标志, 用于分页查询. 当nextQueryHint为空时, 按数量和序号分页. 

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 按照用户/设备组的组合来获取设备列表.当CU界面上需要点开某个设备组之后显示设备时，调用该接口
int cusdk__GetUserDevByGroup( cusdk__GetUserDevByGroupReq req, cusdk__GetUserDevByGroupRsp& rsp );


class cusdk__GetUserDevGroupReq : public cusdk__BaseReq
{
	xsd__string userId;		// 用户Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;		    // 本次请求要的数目.
	xsd__string devTreeId;  //设备树根分组Id
};
class cusdk__GetUserDevGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // 设备组列表.
	int totalNum;										// 满足条件设备组的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 一次性获取用户的所有设备组列表.
int cusdk__GetUserDevGroup( cusdk__GetUserDevGroupReq req, cusdk__GetUserDevGroupRsp& rsp );

class cusdk__GetDomainDevGroupReq : public cusdk__BaseReq
{
    xsd__string userId;		// 用户Id.
	xsd__string domainId;	// 域Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;		    // 本次请求要的数目.
};
class cusdk__GetDomainDevGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroup > devGroupList;	    // 设备列表.
	int totalNum;								        // 满足条件的设备的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取本域内所有的设备组列表, 不包括同步过来的设备组.. 
int cusdk__GetDomainDevGroup( cusdk__GetDomainDevGroupReq req, cusdk__GetDomainDevGroupRsp& rsp );

class cusdk__GetUserDevReq : public cusdk__BaseReq
{
	xsd__string userId;		// 用户Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;		    // 本次请求要的数目.
	xsd__string devTreeId; //设备树Id
};
class cusdk__GetUserDevRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // 设备列表.
	int totalNum;										// 满足条件的设备的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取用户所有的设备列表. 二次开发时使用, 用超级管理员用户可以获取到本平台有权限的所有设备.
int cusdk__GetUserDev( cusdk__GetUserDevReq req, cusdk__GetUserDevRsp& rsp );


class cusdk__GetDevByDomainReq : public cusdk__BaseReq
{
	xsd__string domainId;	// 域Id
    xsd__string userId;		// 用户Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;		    // 本次请求要的总数.
};
class cusdk__GetDevByDomainRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceInfo > devList;	        // 设备列表.
	int totalNum;								        // 满足条件的设备的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 按照域来获取用户设备列表.
int cusdk__GetUserDevByDomain( cusdk__GetDevByDomainReq req, cusdk__GetDevByDomainRsp& rsp );

// 查询 设备组 - 设备(视频源/告警通道) 的关系 二次开发直接获取设备列表的时候需要这个才能搞到设备组信息..
class cusdk__GetDeviceGroupDeviceRelationsReq : public cusdk__BaseReq
{
    xsd__string userId;		// 用户Id.
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__GetDeviceGroupDeviceRelationsRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__DeviceGroupRelation > devGroupRelationList;	// 设备关系列表.
	int totalNum;									// 满足条件的设备组设备关系的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取所有设备组/设备关系. 二次开发直接获取设备列表后如果还需要设备树拓扑, 那他们需要这个.
int cusdk__GetDeviceGroupDeviceRelations( cusdk__GetDeviceGroupDeviceRelationsReq req, cusdk__GetDeviceGroupDeviceRelationsRsp& rsp );

// 设备树管理部分.
class cusdk__CreateDeviceGroupReq : public cusdk__BaseReq
{
	cusdk__DeviceGroupInfo deviceGroupInfo;	// id不需要填, 由服务器生成.
};

class cusdk__CreateDeviceGroupRsp /* : public cusdk__BaseRsp */
{
	xsd__string deviceGroupId;	        // 设备组ID

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建设备组, .
int cusdk__CreateDeviceGroup( cusdk__CreateDeviceGroupReq req, cusdk__CreateDeviceGroupRsp& rsp );

class cusdk__DelDeviceGroupReq : public cusdk__BaseReq
{
	xsd__string deviceGroupId;//待删除的分组Id
	xsd__string devTreeId; //设备树Id
};
class cusdk__DelDeviceGroupRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > conflictedNames;        //删除设备组时将导致重名的组名列表, 最多提示10个. 错误码为ERR_TAS_EXIST_SAMENAME_RECORD时有效

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除设备组.
int cusdk__DelDeviceGroup( cusdk__DelDeviceGroupReq req, cusdk__DelDeviceGroupRsp& rsp );

class cusdk__ModifyDeviceGroupReq : public cusdk__BaseReq
{
	cusdk__DeviceGroupInfo deviceGroupInfo;
};
class cusdk__ModifyDeviceGroupRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改设备组, 包括设备组的拓扑结构.
int cusdk__ModifyDeviceGroup( cusdk__ModifyDeviceGroupReq req, cusdk__ModifyDeviceGroupRsp& rsp );

class cusdk__DeviceGroupAddDeviceReq : public cusdk__BaseReq
{
	std::vector<cusdk__DeviceGroupRelation> relationList;
	xsd__string srcDevTreeId; //标识添加之前，视频源所属的设备树Id
	xsd__string dstDevTreeId; //标识添加之后，视频源所属的设备树Id
};
class cusdk__DeviceGroupAddDeviceRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设备组添加设备/视频源/告警输入通道.
int cusdk__DeviceGroupAddDevice( cusdk__DeviceGroupAddDeviceReq req, cusdk__DeviceGroupAddDeviceRsp& rsp );

class cusdk__DeviceGroupAddGroupReq : public cusdk__BaseReq
{
	xsd__string srcGrpId; //标识添加之前,视频源所属设备分组
	xsd__string dstGrpId; //标识添加之后,视频源所属设备分组
	xsd__string srcDevTreeId; //标识添加之前，视频源所属的设备树Id
	xsd__string dstDevTreeId; //标识添加之后，视频源所属的设备树Id

	bool filterOffline;		// 是否滤除不在线设备.
	bool filterExist;		// 是否滤除已经在目的树上存在的设备.

	xsd__int GBDeviceGroupType;		// 国标推送时分组的类型, 定义见 cusdk__EGBDeviceGroupType. +by lzx@2017-12-14 V2R2B3SP1
};

class cusdk__DeviceGroupAddGroupRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};

//以分组的形式，向其他分组添加视频源
int cusdk__DeviceGroupAddGroup( cusdk__DeviceGroupAddGroupReq req, cusdk__DeviceGroupAddGroupRsp& rsp );

class cusdk__DeviceGroupDelDeviceReq : public cusdk__BaseReq
{
	std::vector<cusdk__DeviceGroupRelation> relationList;
	xsd__string devTreeId; //视频源所属的设备树Id
};
class cusdk__DeviceGroupDelDeviceRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设备组删除设备/视频源/告警输入通道.
int cusdk__DeviceGroupDelDevice( cusdk__DeviceGroupDelDeviceReq req, cusdk__DeviceGroupDelDeviceRsp& rsp );


// 用户管理部分..


class cusdk__GetUserListReq : public cusdk__BaseReq
{
    xsd__string userId;		// 用户Id.    由于此接口与cusdk__GetUserInfo对应TAS同一个查询用户接口，所以必须由CU指定查询条件
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__GetUserListRsp /* : public cusdk__BaseRsp */
{
	std::vector<cusdk__UserInfo> userInfoList;
	int totalNum;										// 满足条件的用户的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取当前用户拥有管理权限的子用户列表.用于用户管理
int cusdk__GetUserList( cusdk__GetUserListReq req, cusdk__GetUserListRsp& rsp );

class cusdk__GetUserGroupListReq : public cusdk__BaseReq
{
    xsd__string userId;		// 用户Id.    由于此接口与cusdk__GetUserGroupInfo对应TAS同一个查询用户接口，所以必须由CU指定查询条件
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__GetUserGroupListRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__UserGroup> userGroupList;
	int totalNum;										// 满足条件的用户组的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取当前用户拥有管理权限的用户组列表.
int cusdk__GetUserGroupList( cusdk__GetUserGroupListReq req, cusdk__GetUserGroupListRsp& rsp );

// 用户组.
class cusdk__CreateUserGroupReq : public cusdk__BaseReq
{
	cusdk__UserGroup userGroupInfo;	// id不需要填, 由服务器生成.
};
class cusdk__CreateUserGroupRsp/* : public cusdk__BaseRsp*/
{
	xsd__string userGroupId;	    // 用户组信息

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建用户组, .
int cusdk__CreateUserGroup( cusdk__CreateUserGroupReq req, cusdk__CreateUserGroupRsp& rsp );

class cusdk__DelUserGroupReq : public cusdk__BaseReq
{
	xsd__string userGroupId;	
};
class cusdk__DelUserGroupRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况,对应进度查询接口：QueryOperationProgress
};
//gsoap cusdk service method-documentation: 删除用户组, .
int cusdk__DelUserGroup( cusdk__DelUserGroupReq req, cusdk__DelUserGroupRsp& rsp );

class cusdk__ModifyUserGroupReq : public cusdk__BaseReq
{
	cusdk__UserGroup userGroupInfo;
};
class cusdk__ModifyUserGroupRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改用户组信息 .
int cusdk__ModifyUserGroup( cusdk__ModifyUserGroupReq req, cusdk__ModifyUserGroupRsp& rsp );

// 用户
class cusdk__CreateUserReq : public cusdk__BaseReq
{
	cusdk__UserInfo userInfo;	// id不需要填, 由服务器生成.
};
class cusdk__CreateUserRsp/* : public cusdk__BaseRsp*/
{
	xsd__string userId;		    // 完整信息

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建用户 .
int cusdk__CreateUser( cusdk__CreateUserReq req, cusdk__CreateUserRsp& rsp );

class cusdk__DelUserReq : public cusdk__BaseReq
{
	xsd__string userId;	
};
class cusdk__DelUserRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况，对应进度查询接口：QueryOperationProgress
};
//gsoap cusdk service method-documentation: 删除用户 .
int cusdk__DelUser( cusdk__DelUserReq req, cusdk__DelUserRsp& rsp );

class cusdk__ModifyUserReq : public cusdk__BaseReq
{
	cusdk__UserInfo userInfo;	
};
class cusdk__ModifyUserRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改用户信息 .
int cusdk__ModifyUser( cusdk__ModifyUserReq req, cusdk__ModifyUserRsp& rsp );

class cusdk__ChangePasswordReq : public cusdk__BaseReq
{
	xsd__string oldPassword;		// 旧密码.
	xsd__string newPassword;		// 新密码.
	time_t passwordOverdueTime;		// 密码截止时间.

	xsd__string passwordAlgorithm;	// 密码使用的加密算法. 空 为明文, "SM3_Base64" 表示SM3取散列值后Base64编码为字符串.
};
class cusdk__ChangePasswordRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改用户密码 .
int cusdk__ChangePassword( cusdk__ChangePasswordReq req, cusdk__ChangePasswordRsp& rsp );

// 用户组/用户 的设备划归.
// 这里是当前用户对其他用户/用户组进行操作，为了区分操作对象是用户还是组，所以有cusdk__EUserOrUserGroup
class cusdk__UserGetDeviceByGroupReq : public cusdk__BaseReq
{
	xsd__string id;						// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	xsd__string deviceGroupId;			// 设备组Id.
	xsd__int beginIndex;				// 起始序号.
	xsd__int num;						// 本次请求要的总数.
};
class cusdk__UserGetDeviceByGroupRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__AssignDeviceInfo> deviceList;	// 设备划归信息.
	int totalNum;										// 满足条件的设备划归信息的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取用户/用户组划归的设备/视频源/告警输入通道.仅当用户管理功能的用户设备划归时使用
int cusdk__UserGetDeviceByGroup( cusdk__UserGetDeviceByGroupReq req, cusdk__UserGetDeviceByGroupRsp& rsp );

class cusdk__UserAddDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserAddDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 用户/用户组划归设备/视频源/告警输入通道.
int cusdk__UserAddDevice( cusdk__UserAddDeviceReq req, cusdk__UserAddDeviceRsp& rsp );

class cusdk__UserDelDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserDelDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 用户/用户组删除设备/视频源/告警输入通道.
int cusdk__UserDelDevice( cusdk__UserDelDeviceReq req, cusdk__UserDelDeviceRsp& rsp );

class cusdk__UserGetTVWallReq : public cusdk__BaseReq
{
	xsd__string id;						// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	xsd__int beginIndex;				// 起始序号.
	xsd__int num;						// 本次请求要的总数.
};
class cusdk__UserGetTVWallRsp/* : public cusdk__BaseRsp*/
{
	std::vector<xsd__string> tvWallIdList;	// 已经划归了的电视墙Id.
	int totalNum;										// 满足条件的电视墙划归的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取用户/用户组划归的电视墙.仅当用户管理功能的用户设备划归时使用
int cusdk__UserGetTVWall( cusdk__UserGetTVWallReq req, cusdk__UserGetTVWallRsp& rsp );

class cusdk__UserAddTVWallReq : public cusdk__BaseReq
{
	xsd__string id;		// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	std::vector<xsd__string> tvwallList;
};
class cusdk__UserAddTVWallRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 用户/用户组划归设备/视频源/告警输入通道.
int cusdk__UserAddTVWall( cusdk__UserAddTVWallReq req, cusdk__UserAddTVWallRsp& rsp );

class cusdk__UserDelTVWallReq : public cusdk__BaseReq
{
	xsd__string id;		// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	xsd__string tvwallId;
};
class cusdk__UserDelTVWallRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 用户/用户组删除设备/视频源/告警输入通道.
int cusdk__UserDelTVWall( cusdk__UserDelTVWallReq req, cusdk__UserDelTVWallRsp& rsp );

class cusdk__UserModifyDeviceReq : public cusdk__BaseReq
{
	xsd__string id;		// 用户或用户组的Id. 如果用户和用户组的Id不会重复, 那可以只用一个接口.
	enum cusdk__EUserOrUserGroup type;	// 操作的目标是用户还是用户组.
	std::vector<cusdk__AssignDeviceInfo> deviceList;
};
class cusdk__UserModifyDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 用户/用户组修改设备/视频源/告警输入通道的权限.
int cusdk__UserModifyDevice( cusdk__UserModifyDeviceReq req, cusdk__UserModifyDeviceRsp& rsp );

//以下临时授权功能只有超级管理员才能使用，所以临时授权是针对域，不针对其他用户
class cusdk__GetAuthorizationDeviceReq : public cusdk__BaseReq
{
	xsd__int beginIndex;				// 起始序号.
	xsd__int num;						// 本次请求要的总数.
};
class cusdk__GetAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__AuthorizationDeviceInfo> authorizedDeviceList;		// 被授权设备列表
	int totalNum;															// 满足条件的设备组的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取临时授权设备列表.
int cusdk__GetAuthorizationDevice( cusdk__GetAuthorizationDeviceReq req, cusdk__GetAuthorizationDeviceRsp& rsp );

class cusdk__AddAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// 被授权设备. 其中的创建时间由平台生成
};
class cusdk__AddAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{
    time_t	createTime;							        // 创建时间.由TAS生成

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 添加临时授权设备.
int cusdk__AddAuthorizationDevice( cusdk__AddAuthorizationDeviceReq req, cusdk__AddAuthorizationDeviceRsp& rsp );

class cusdk__DelAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// 被授权设备.
};
class cusdk__DelAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除临时授权设备.
int cusdk__DelAuthorizationDevice( cusdk__DelAuthorizationDeviceReq req, cusdk__DelAuthorizationDeviceRsp& rsp );

class cusdk__ModifyAuthorizationDeviceReq : public cusdk__BaseReq
{
	cusdk__AuthorizationDeviceInfo authorizedDev;		// 被授权设备.
};
class cusdk__ModifyAuthorizationDeviceRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改临时授权设备.
int cusdk__ModifyAuthorizationDevice( cusdk__ModifyAuthorizationDeviceReq req, cusdk__ModifyAuthorizationDeviceRsp& rsp );


class cusdk__GetDeviceBaseInfoReq : public cusdk__BaseReq
{
	xsd__string devId;
};
class cusdk__GetDeviceBaseInfoRsp/* : public cusdk__BaseRsp*/
{
	cusdk__DeviceInfo devInfo;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取单个设备的信息. 无权限信息.
int cusdk__GetDeviceBaseInfo( cusdk__GetDeviceBaseInfoReq req, cusdk__GetDeviceBaseInfoRsp& rsp );



class cusdk__GetRCSSysParamReq : public cusdk__BaseReq
{	
};
class cusdk__GetRCSSysParamRsp/* : public cusdk__BaseRsp*/
{
	enum cusdk__ERS_CoverPolicy coverPolicy;		// 录像覆盖策略。

	bool maxLockedRecordDiskSpaceCapacitySupported;	// 是否支持 maxLockedRecordDiskSpaceCapacity 参数, 用于兼容性. +by lzx@2018-03-06 V2R4B1
	int maxLockedRecordDiskSpaceCapacity;			// 锁定录像在磁盘中的最大占比. +by lzx@2018-03-06 V2R4B1

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取覆盖策略.
int cusdk__GetRCSSysParam( cusdk__GetRCSSysParamReq req, cusdk__GetRCSSysParamRsp& rsp );

class cusdk__SetRCSSysParamReq : public cusdk__BaseReq
{
	enum cusdk__ERS_CoverPolicy coverPolicy;

	//bool maxLockedRecordDiskSpaceCapacitySupported;	// 是否支持 maxLockedRecordDiskSpaceCapacity 参数, 用于兼容性. +by lzx@2018-03-06 V2R4B1
	//int maxLockedRecordDiskSpaceCapacity;			// 锁定录像在磁盘中的最大占比. +by lzx@2018-03-06 V2R4B1
};
class cusdk__SetRCSSysParamRsp/* : public cusdk__BaseRsp*/
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取覆盖策略.
int cusdk__SetRCSSysParam( cusdk__SetRCSSysParamReq req, cusdk__SetRCSSysParamRsp& rsp );

class cusdk__GetNruListReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__GetNruListRsp/* : public cusdk__BaseRsp*/
{
	int totalNum;										// 满足条件的Nru的总数.
	std::vector<cusdk__NruInfo> nruList;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 分页获取NRU/分区列表.
int cusdk__GetNruList( cusdk__GetNruListReq req, cusdk__GetNruListRsp& rsp );

class cusdk__GetDiskListReq : public cusdk__BaseReq
{
	xsd__string nruId;		// 磁盘所属NRU Id.

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__GetDiskListRsp/* : public cusdk__BaseRsp*/
{
	std::vector<cusdk__DiskInfo> diskList;	// 磁盘列表.
	int totalNum;							// 满足条件的磁盘的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 分页获取磁盘列表.
int cusdk__GetDiskList( cusdk__GetDiskListReq req, cusdk__GetDiskListRsp& rsp );

class cusdk__GetPuChnBindingReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU的UUID
	xsd__string diskMountPath;	// 分区的mount路径。

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__GetPuChnBindingRsp/* : public cusdk__BaseRsp*/
{	
	xsd__string nruId;	// NRU的UUID	冗余
	xsd__string diskMountPath;	// 分区的mount路径。冗余

	int totalNum;	// 分区上绑定的设备总数.
	std::vector<cusdk__DevChannel> channelList;	// 绑定的通道列表。	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取绑定关系列表.
int cusdk__GetPuChnBinding( cusdk__GetPuChnBindingReq req, cusdk__GetPuChnBindingRsp& rsp );

class cusdk__BindPuReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU的UUID
	xsd__string diskMountPath;	// 分区的mount路径。
	std::vector<cusdk__DevChannel> channelList;	// 绑定的通道列表。
};
class cusdk__BindPuRsp/* : public cusdk__BaseRsp*/
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  添加绑定关系
int cusdk__BindPu( cusdk__BindPuReq req, cusdk__BindPuRsp& rsp );

class cusdk__UnbindPuReq : public cusdk__BaseReq
{
	xsd__string nruId;	// NRU的UUID
	xsd__string diskMountPath;	// 分区的mount路径。
	std::vector<cusdk__DevChannel> channelList;	// 解除绑定的通道列表。
};
class cusdk__UnbindPuRsp/* : public cusdk__BaseRsp*/
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  删除绑定关系
int cusdk__UnbindPu( cusdk__UnbindPuReq req, cusdk__UnbindPuRsp& rsp );

class cusdk__QueryRecordDaysReq : public cusdk__BaseReq
{
	std::vector<cusdk__DevChannel> devChnList;		// 设备通道
	enum cusdk__ERecordType recordType;		// 回放类型, 平台/前端 录像.
	cusdk__TimeRange timeRange;						// 查询时间段。
	xsd__string recordDomainName;					// 录像所在域名，填空为本域。
};
class cusdk__QueryRecordDaysRsp /* : public cusdk__BaseRsp */
{		
	std::vector<cusdk__OnePuChnRecDays> recordStateInDaysList;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  查询一段时间里哪天有平台录像
int cusdk__QueryRecordDays( cusdk__QueryRecordDaysReq req, cusdk__QueryRecordDaysRsp& rsp );

class cusdk__QueryRecordReq : public cusdk__BaseReq
{
	enum cusdk__ERecordType recordType;		// 回放类型, 平台/前端 录像.
	cusdk__DevChannel devChn;				// 设备通道
	enum cusdk__ERecordEventType recEventType;		// 录像事件类型
	cusdk__TimeRange timeRange;				// 查询时间段。

	xsd__string queryHandle;				// 查询句柄.为空表示新查询

	xsd__string recordDomainName;			// 只有平台录像有效. 录像所在域的域名，填空为本域。


	bool useVSIPTimeAsGMTTime;				// VSIP前端录像查询时间标志, true则要求和VSIP交互的时间当作GMT时间.
};
class cusdk__QueryRecordRsp /* : public cusdk__BaseRsp */
{		
	cusdk__DevChannel devChn;								// 设备通道(冗余，但方便抓包查问题。）
	std::vector< cusdk__RecDuration > recordDurationList;	// 结果。

	xsd__string queryHandle;				// 查询句柄.
	xsd__boolean hasLeft;					// 是否还有其它记录.

	bool useVSIPTimeAsGMTTime;				// VSIP前端录像查询时间标志, true则要求和VSIP交互的时间当作GMT时间.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  查询录像
int cusdk__QueryRecord( cusdk__QueryRecordReq req, cusdk__QueryRecordRsp& rsp );

class cusdk__StartPlatformRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StartPlatformRecordRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  开始手动平台录像
int cusdk__StartPlatformRecord( cusdk__StartPlatformRecordReq req, cusdk__StartPlatformRecordRsp& rsp );

class cusdk__StopPlatformRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StopPlatformRecordRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  停止手动平台录像
int cusdk__StopPlatformRecord( cusdk__StopPlatformRecordReq req, cusdk__StopPlatformRecordRsp& rsp );

class cusdk__StartPuRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StartPuRecordRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  开始手动前端录像
int cusdk__StartPuRecord( cusdk__StartPuRecordReq req, cusdk__StartPuRecordRsp& rsp );

class cusdk__StopPuRecordReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	
};
class cusdk__StopPuRecordRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation:  停止手动前端录像
int cusdk__StopPuRecord( cusdk__StopPuRecordReq req, cusdk__StopPuRecordRsp& rsp );


class cusdk__SetupPlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// 客户端的播放器id。
	cusdk__RecordPlayDuration recordPlayDuration;	// 要播放的录像段.

	xsd__boolean directTrans;								// 是否直传, 由前端直接发出.
	xsd__boolean directTransFromDeviceDomain;				// 直传, 由设备所在域发出码流.

	int transMode;		// 定义参见 cusdk__ETransMode

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 浏览请求方的媒体信息描述.
	cusdk__TranscodeMediaFormat transcodeMeidaFormat;		// 要求平台转码

//	cusdk__RecDuration recordFileDuration;			// 要播放的录像文件信息. +@2014-08-20
};
class cusdk__SetupPlayRecordRsp /* : public cusdk__BaseRsp */
{
	xsd__string recordPlaySession;				// 录像播放session, 系统全局。
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 服务器方的媒体信息描述.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// 媒体扩展信息.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 请求开始播放录像, 申请码流资源, 创建交换.
int cusdk__SetupPlayRecord( cusdk__SetupPlayRecordReq req, cusdk__SetupPlayRecordRsp& rsp );


class cusdk__PlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// 客户端的播放器id。
};
class cusdk__PlayRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始发送录像的码流
int cusdk__PlayRecord( cusdk__PlayRecordReq req, cusdk__PlayRecordRsp& rsp );

class cusdk__VcrCtrlReq : public cusdk__BaseReq
{
	xsd__int playId;				// 客户端的播放器id。
//	xsd__string recordPlaySession;	// 录像播放session。

	enum cusdk__EVCRCmd vcrCmd;		// vcr 命令。
	int seekTime;					// 要定位的时间点，相对时间, 单位秒。
	int playSpeed;					// 播放速度。
};
class cusdk__VcrCtrlRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 录像播放VCR控制
int cusdk__VcrCtrl( cusdk__VcrCtrlReq req, cusdk__VcrCtrlRsp& rsp );

class cusdk__GetCurRecordPlayPositionReq : public cusdk__BaseReq
{
	xsd__int playId;							// 客户端的播放器id。
//	xsd__string recordPlaySession;				// 录像播放session。
};
class cusdk__GetCurRecordPlayPositionRsp /* : public cusdk__BaseRsp */
{
	xsd__int playId;								// 客户端的播放器id。
//	xsd__string recordPlaySession;				// 录像播放session。冗余，为了查问题方便。

	time_t curTime;			// 录像播放进度的绝对时间。
	cusdk__TimeRange      recordTimeRange;             /*录像时间范围*/

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询录像播放的当前进度
int cusdk__GetCurRecordPlayPosition( cusdk__GetCurRecordPlayPositionReq req, cusdk__GetCurRecordPlayPositionRsp& rsp );

class cusdk__StopPlayRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// 客户端的播放器id。

	//xsd__string recordPlaySession;				// 录像播放session。
};
class cusdk__StopPlayRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 停止播放录像
int cusdk__StopPlayRecord( cusdk__StopPlayRecordReq req, cusdk__StopPlayRecordRsp& rsp );

class cusdk__SetupDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;								// 客户端的播放器id。

	cusdk__RecordPlayDuration recordPlayDuration;	// 要下载的录像段.

	xsd__boolean directTrans;						// 是否直传, 由前端直接发出.
	xsd__boolean directTransFromDeviceDomain;				// 直传, 由设备所在域发出码流.

	int transMode;		// 定义参见 cusdk__ETransMode

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 下载方的媒体信息描述.	
};
class cusdk__SetupDownloadRecordRsp /* : public cusdk__BaseRsp */
{	
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 服务器方的媒体信息描述.

	xsd__string recordFilePath;	// rpctrl需要的播放文件路径.
	xsd__string recordFileName;	// rpctrl需要的播放文件名.

	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// 媒体扩展信息.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 请求请求开始录像下载, 申请码流资源, 创建交换.
int cusdk__SetupDownloadRecord( cusdk__SetupDownloadRecordReq req, cusdk__SetupDownloadRecordRsp& rsp );

class cusdk__StartDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__StartDownloadRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始发送录像的码流
int cusdk__StartDownloadRecord( cusdk__StartDownloadRecordReq req, cusdk__StartDownloadRecordRsp& rsp );

class cusdk__StopDownloadRecordReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__StopDownloadRecordRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 停止录像下载
int cusdk__StopDownloadRecord( cusdk__StopDownloadRecordReq req, cusdk__StopDownloadRecordRsp& rsp );

class cusdk__GetRecordPlanReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;			// 设备通道。
};
class cusdk__GetRecordPlanRsp /* : public cusdk__BaseRsp */
{
	cusdk__DevChannel devChn;			// 设备通道。冗余，为了查问题方便。
	cusdk__RecordPlan recordPlan;		// 录像计划。
	int recStreamType;                  // 录像码流类型。0-主流，1-辅流1，2-辅流2，3-辅流3
	bool supportChangeStreamType;       // 支持切换录像码流类型
	xsd__string session;	            // 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取录像计划
int cusdk__GetRecordPlan( cusdk__GetRecordPlanReq req, cusdk__GetRecordPlanRsp& rsp );

class cusdk__SetRecordPlanReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;			// 设备视频源。
	int recStreamType;                  // 录像码流类型。0-主流，1-辅流1，2-辅流2，3-辅流3
	bool supportChangeStreamType;       // 支持切换录像码流类型
	cusdk__RecordPlan recordPlan;		// 录像计划。
};
class cusdk__SetRecordPlanRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置录像计划
int cusdk__SetRecordPlan( cusdk__SetRecordPlanReq req, cusdk__SetRecordPlanRsp& rsp );



// 查询电视墙列表
class cusdk__GetAllTVWallReq : public cusdk__BaseReq
{	
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__GetAllTVWallRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWall > tvWallList;	// 电视墙列表.
	int totalNum;											// 电视墙的总数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取所有电视墙.
int cusdk__GetAllTVWall( cusdk__GetAllTVWallReq req, cusdk__GetAllTVWallRsp& rsp );

class cusdk__GetTVWallReq : public cusdk__BaseReq
{	
	xsd__string tvwallId;			// 电视墙Id
};

class cusdk__GetTVWallRsp /* : public cusdk__BaseRsp */
{
	cusdk__TVWall tvWallInfo;	// 电视墙列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取电视墙详细信息.
int cusdk__GetTVWall( cusdk__GetTVWallReq req, cusdk__GetTVWallRsp& rsp );

class cusdk__CreateTVWallReq : public cusdk__BaseReq
{
	cusdk__TVWall TVWallInfo;	// id不需要填, 由服务器生成.
};

class cusdk__CreateTVWallRsp /* : public cusdk__BaseRsp */
{
	xsd__string TVWallId;	        // 电视墙ID
	time_t creatTime;					// 创建时间.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建电视墙, .
int cusdk__CreateTVWall( cusdk__CreateTVWallReq req, cusdk__CreateTVWallRsp& rsp );

class cusdk__DelTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;
};
class cusdk__DelTVWallRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除电视墙.
int cusdk__DelTVWall( cusdk__DelTVWallReq req, cusdk__DelTVWallRsp& rsp );

class cusdk__ModifyTVWallReq : public cusdk__BaseReq
{
	cusdk__TVWall TVWallInfo;
};
class cusdk__ModifyTVWallRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改电视墙.
int cusdk__ModifyTVWall( cusdk__ModifyTVWallReq req, cusdk__ModifyTVWallRsp& rsp );


// 放像.
class cusdk__TVWallStartPlayReq : public cusdk__BaseReq
{	
	//cusdk__DevChannel decoderChn;	// 解码器 解码通道.
	xsd__string TVWallId;			// 电视墙Id.
	int tvId;						// 电视机Id.
	int tvDivId;					// 电视机分画面Id.
	cusdk__DevChannel encoderChn;	// 编码器/IPC 等视频源通道.	
};

class cusdk__TVWallStartPlayRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	
};
//gsoap cusdk service method-documentation: 电视墙/解码器 上播放图像.
int cusdk__TVWallStartPlay( cusdk__TVWallStartPlayReq req, cusdk__TVWallStartPlayRsp& rsp );

class cusdk__TVWallStopPlayReq : public cusdk__BaseReq
{		
	cusdk__DevChannel encoderChn;	// 当前编码通道.
	xsd__string TVWallId;			// 电视墙Id.
	int tvId;						// 电视机Id.
	int tvDivId;					// 电视机分画面Id.
};

class cusdk__TVWallStopPlayRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 电视墙/解码器 上播放图像.
int cusdk__TVWallStopPlay( cusdk__TVWallStopPlayReq req, cusdk__TVWallStopPlayRsp& rsp );

class cusdk__TVWallWindowPollContrlReq : public cusdk__BaseReq
{	
	//cusdk__DevChannel encoderChn;	// 编码器/IPC 等视频源通道.	

	xsd__string TVWallId;			// 电视墙Id.
	int tvId;						// 电视机Id.
	int tvDivId;					// 电视机分画面Id.

	std::vector< cusdk__TVWallWindowPollStep > pollSteps;	// 轮巡的各个步骤.

	enum cusdk__EVCRCmd pollCmd;	// 轮巡控制命令. 支持 开始/停止/暂停/恢复.  是否需要新建一个枚举还是重用录像的?
};

class cusdk__TVWallWindowPollContrlRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 电视墙/解码器 窗口轮巡控制.
int cusdk__TVWallWindowPollContrl( cusdk__TVWallWindowPollContrlReq req, cusdk__TVWallWindowPollContrlRsp& rsp );

class cusdk__TVWallSaveSchemeReq : public cusdk__BaseReq
{
	cusdk__TVWallScheme scheme;	// 要保存的预案. id不需要填, 由服务器生成.
};

class cusdk__TVWallSaveSchemeRsp /* : public cusdk__BaseRsp */
{
//	xsd__string schemeId;	        // 预案Id

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 保存预案. 同名预案覆盖.
int cusdk__TVWallSaveScheme( cusdk__TVWallSaveSchemeReq req, cusdk__TVWallSaveSchemeRsp& rsp );

class cusdk__TVWallDelSchemeReq : public cusdk__BaseReq
{
	xsd__string tvwallId;		// 所属电视墙Id.
	xsd__string schemeName;		// 预案名称.
//	xsd__string schemeId;	        // 预案Id
};

class cusdk__TVWallDelSchemeRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除预案.
int cusdk__TVWallDelScheme( cusdk__TVWallDelSchemeReq req, cusdk__TVWallDelSchemeRsp& rsp );

class cusdk__TVWallLoadSchemeReq : public cusdk__BaseReq
{
	xsd__string tvwallId;		// 所属电视墙Id.
	xsd__string schemeName;		// 预案名称.
//	xsd__string schemeId;	        // 预案Id
};

class cusdk__TVWallLoadSchemeRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 载入预案.
int cusdk__TVWallLoadScheme( cusdk__TVWallLoadSchemeReq req, cusdk__TVWallLoadSchemeRsp& rsp );

class cusdk__TVWallQuerySchemeReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// 电视墙Id.
};

class cusdk__TVWallQuerySchemeRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWallScheme > schemeList;	// 电视墙所有的预案列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取电视墙的预案..
int cusdk__TVWallQueryScheme( cusdk__TVWallQuerySchemeReq req, cusdk__TVWallQuerySchemeRsp& rsp );

class cusdk__TVWallSetSchemePollReq : public cusdk__BaseReq
{
	xsd__string tvWallId;
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// 电视墙预案轮巡的步骤.
};

class cusdk__TVWallSetSchemePollRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置电视墙预案轮巡的步骤..
int cusdk__TVWallSetSchemePoll( cusdk__TVWallSetSchemePollReq req, cusdk__TVWallSetSchemePollRsp& rsp );

class cusdk__TVWallGetSchemePollReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// 电视墙Id.
};

class cusdk__TVWallGetSchemePollRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__TVWallSchemePollStep > pollSteps;	// 电视墙预案轮巡的步骤.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取电视墙的预案轮巡配置..
int cusdk__TVWallGetSchemePoll( cusdk__TVWallGetSchemePollReq req, cusdk__TVWallGetSchemePollRsp& rsp );

class cusdk__TVWallSchemePollControlReq : public cusdk__BaseReq
{
	xsd__string tvWallId;	// 电视墙Id.
	enum cusdk__EVCRCmd pollCmd;	// 轮巡控制命令. 支持 开始/停止/暂停/恢复.  是否需要新建一个枚举还是重用录像的?
};

class cusdk__TVWallSchemePollControlRsp /* : public cusdk__BaseRsp */
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 电视墙的预案轮巡控制.
int cusdk__TVWallSchemePollControl( cusdk__TVWallSchemePollControlReq req, cusdk__TVWallSchemePollControlRsp& rsp );

class cusdk__SetTVDivNumReq : public cusdk__BaseReq
{	
	xsd__string tvwallId;	// 电视墙Id
	int tvId;		// 电视序号.
	int tvDivNum;	// 电视分画面数目	定义见 cusdk__ETVDivStyle
};

class cusdk__SetTVDivNumRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置设备通道别名.
int cusdk__SetTVDivNum( cusdk__SetTVDivNumReq req, cusdk__SetTVDivNumRsp& rsp );

/*解码器码流接收端口信息*/
class cusdk__DecMediaPortInfo
{
	xsd__string deviceIP;      //设备IP（解码器或者CU的码流接收地址）
	int rtpVideoPort;          //解码器RTP视频接收端口
	int rtpAudioPort;          //解码器RTP音频接收端口
	int rtcpVideoPort;         //解码器RTCP视频发送端口
	int rtcpAudioPort;         //解码器RTCP音频发送端口
};
/*本地的码流接收端口信息*/
class cusdk__LocalMediaPortInfo : public cusdk__DecMediaPortInfo
{
	xsd__string mediaStreamType; //码流类型（ES、PS、TS等等）
	xsd__string manufacture;	// 厂商.
};
class cusdk__PlayRecordOnTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;			// 电视墙Id.
	int tvId;						// 电视机Id.
	int tvDivId;					// 电视机分画面Id.

	cusdk__RecordPlayDuration recordPlayDuration;	// 要播放的录像段列表.
	cusdk__LocalMediaPortInfo localMediaPortInfo;   //本地码流接收端口信息
};
class cusdk__PlayRecordOnTVWallRsp /* : public cusdk__BaseRsp */
{
	
	xsd__string session;	// 冗余 , 为了查问题方便过滤.

	cusdk__DecMediaPortInfo decMediaPortInfo;   //解码器码流接收端口信息

	long errorCode;
};
//gsoap cusdk service method-documentation: 播放录像到电视墙.
int cusdk__PlayRecordOnTVWall( cusdk__PlayRecordOnTVWallReq req, cusdk__PlayRecordOnTVWallRsp& rsp );


class cusdk__StopPlayRecordOnTVWallReq : public cusdk__BaseReq
{
	xsd__string TVWallId;			// 电视墙Id.
	int tvId;						// 电视机Id.
	int tvDivId;					// 电视机分画面Id.
};
class cusdk__StopPlayRecordOnTVWallRsp /* : public cusdk__BaseRsp */
{
	
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 停止电视墙上播放的录像.
int cusdk__StopPlayRecordOnTVWall( cusdk__StopPlayRecordOnTVWallReq req, cusdk__StopPlayRecordOnTVWallRsp& rsp );

// class cusdk__GetPlayRecordOnTVWallStateReq : public cusdk__BaseReq
// {
// 	xsd__string TVWallId;			// 电视墙Id.
// 	int tvId;						// 电视机Id.
// 	int tvDivId;					// 电视机分画面Id.
// 
// 	//cusdk__DevChannel decoderChn;														// 要获取的解码器通道.
// };
// class cusdk__GetPlayRecordOnTVWallStateRsp /* : public cusdk__BaseRsp */
// {
// 	std::vector< cusdk__RecordPlayDuration > recordPlayDurationList;	// 计划播放的录像段列表.
// 	cusdk__RecordPlayDuration curRecordDuration;						// 当前正在播放的录像段.
// 	xsd__string recordPlaySession;				// 当前的录像播放session。可以用 cusdk__GetCurRecordPlayPosition 获取进度.
//
//xsd__string session;	// 冗余 , 为了查问题方便过滤.
//long errorCode;
// };
// //gsoap cusdk service method-documentation: 获取电视墙的录像播放状态. 这个也可以是通知, 这里写成查询..
// int cusdk__GetPlayRecordOnTVWallState( cusdk__GetPlayRecordOnTVWallStateReq req, cusdk__GetPlayRecordOnTVWallStateRsp& rsp );

// 
// class cusdk__PlayRecordOnTVWallVCRCtrlReq : public cusdk__BaseReq
// {
// 	xsd__string recordPlaySession;				// 录像播放session。
// 
// 	enum cusdk__EVCRCmd vcrCmd;					// vcr 命令。
// 	int seekTime;								// 要定位的时间点，相对时间, 单位秒。
// 	int playSpeed;								// 播放速度。这个不一定支持, 待确定.
// };
// class cusdk__PlayRecordOnTVWallVCRCtrlRsp /* : public cusdk__BaseRsp */
// {
//
//xsd__string session;	// 冗余 , 为了查问题方便过滤.
//long errorCode;
// };
// //gsoap cusdk service method-documentation: 播放录像到电视墙.
// int cusdk__PlayRecordOnTVWallVCRCtrl( cusdk__PlayRecordOnTVWallVCRCtrlReq req, cusdk__PlayRecordOnTVWallVCRCtrlRsp& rsp );

class cusdk__MapModuleAttrs
{
	xsd__string key;
	xsd__string value;
};
class cusdk__MapTypeInfo
{
	enum cusdk__EMapType mapType;	// 地图类型.
	xsd__string mapVersion;			// 版本.
	xsd__string accessUrl;			// 访问服务地址
	std::vector< cusdk__MapModuleAttrs > moduleAttrs;	// 地图模块属性
};
class cusdk__GetSupportMapTypesReq : public cusdk__BaseReq
{
};
class cusdk__GetSupportMapTypesRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MapTypeInfo > supportMapTypes;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取当前服务器支持的地图类型.
int cusdk__GetSupportMapTypes( cusdk__GetSupportMapTypesReq req, cusdk__GetSupportMapTypesRsp& rsp );

class cusdk__Map
{
	xsd__string mapId;		// 地图Id
	xsd__string mapName;	// 地图名称.

	enum cusdk__EMapType mapType;	// 地图类型.

	xsd__string mapUrl;		// 地图图片文件url. ( 由地图插件定义格式 ) jpeg地图要先上传图片.

	xsd__string parentMapId;	// 父地图Id.

	xsd__string description;	// 描述

	// 地图边界.
	double minLongitude;	
	double maxLongitude;
	double minLatitude;
	double maxLatitude;

	// 缩放范围.
	int minScaleLevel;
	int maxScaleLevel;

	double screenCenterLongitude;		// 屏幕(地图控件)中间点显示的默认经度
	double screenCenterLatitude;		// 屏幕(地图控件)中间点显示的默认维度.
	int defaultScaleLevel;				// 默认的缩放级别

	enum cusdk__EMapCoordType coordType;// 坐标类型.
	xsd__string version;	// 地图版本.?

	// 地图类型?? 是否需要?? Google地图是否允许创建??
};

class cusdk__GetMapImageUploadUrlReq : public cusdk__BaseReq
{
	xsd__string parentMapId;		// 上级地图Id.
};
class cusdk__GetMapImageUploadUrlRsp /* : public cusdk__BaseRsp */
{
	xsd__string imageUploadUrl;		// 地图图片上传URL

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取上传地图图片的url.
int cusdk__GetMapImageUploadUrl( cusdk__GetMapImageUploadUrlReq req, cusdk__GetMapImageUploadUrlRsp& rsp );

class cusdk__CreateMapReq : public cusdk__BaseReq
{
	cusdk__Map mapInfo;		// 要创建的地图信息.
};
class cusdk__CreateMapRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapId;		// 新创建的地图的id.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建地图. Google地图并不允许创建?. Jpeg地图要先上传图片, 然后创建. 
int cusdk__CreateMap( cusdk__CreateMapReq req, cusdk__CreateMapRsp& rsp );

class cusdk__ModifyMapReq : public cusdk__BaseReq
{
	cusdk__Map mapInfo;		// 要修改的地图信息.
};
class cusdk__ModifyMapRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改地图.
int cusdk__ModifyMap( cusdk__ModifyMapReq req, cusdk__ModifyMapRsp& rsp );

class cusdk__DelMapReq : public cusdk__BaseReq
{
	xsd__string mapId;
};
class cusdk__DelMapRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除地图.
int cusdk__DelMap( cusdk__DelMapReq req, cusdk__DelMapRsp& rsp );

class cusdk__QueryMapReq : public cusdk__BaseReq
{
	xsd__string mapId;	// 指定地图Id.
	bool mapIdEnabled;	// 是否有效.

	xsd__string parentMapId;	// 父地图
	bool parentMapIdEnabled;	// 是否按父地图查询.

	enum cusdk__EMapType mapType;	// 地图类型.
	bool mapTypeEnabled;

	xsd__string mapName;	// 地图名称
	bool mapNameEnabled;	// 是否按地图名称查询.

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryMapRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;										// 图元数目.
	std::vector< cusdk__Map > mapList;	// 所有地图列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询地图列表
int cusdk__QueryMap( cusdk__QueryMapReq req, cusdk__QueryMapRsp& rsp );

class cusdk__MapPosition
{
	double longitude;	// 经度.
	double latitude;	// 纬度.
};

class cusdk__MapArea
{
	xsd__string areaId;	             	//区域ID
	xsd__string parentMapId;	        //区域所属地图ID
	xsd__string refMapId;	            //区域对应的子地图ID
	xsd__string layerId;	            //地图图层ID 预留字段.
	xsd__string layerColor;				// 区域的颜色.
	//int styleId;		             	//区域的样式
	xsd__string customStyle;	        //区域自定义样式
	double opacity;		             	//区域显示透明度
	std::vector<cusdk__MapPosition> trace;	//区域各顶点坐标
};

class cusdk__QueryMapAreaReq : public cusdk__BaseReq
{
	xsd__string areaId;	             	//区域ID
	bool areaIdEnabled;

	xsd__string parentMapId;
	bool parentMapIdEnabled;

	xsd__string refMapId;
	bool refMapIdEnabled;


	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryMapAreaRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;							// 图元数目.
	std::vector< cusdk__MapArea > areaList;	// 所有地图列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询地图列表
int cusdk__QueryMapArea( cusdk__QueryMapAreaReq req, cusdk__QueryMapAreaRsp& rsp );

class cusdk__CreateMapAreaReq : public cusdk__BaseReq
{
	cusdk__MapArea mapAreaInfo;		// 要创建的区域信息.
};
class cusdk__CreateMapAreaRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapAreaId;		// 新创建的地图区域的id.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建地图区域
int cusdk__CreateMapArea( cusdk__CreateMapAreaReq req, cusdk__CreateMapAreaRsp& rsp );

class cusdk__ModifyMapAreaReq : public cusdk__BaseReq
{
	cusdk__MapArea mapAreaInfo;		// 要修改的地图区域信息.
};
class cusdk__ModifyMapAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改地图区域信息
int cusdk__ModifyMapArea( cusdk__ModifyMapAreaReq req, cusdk__ModifyMapAreaRsp& rsp );

class cusdk__MapElement
{
	xsd__string id;
	xsd__string name;						// 图元名称.
	xsd__string mapId;						// 所属的地图的Id.
//	xsd__string elementGroupId;				//所属图元组id.(图元组功能不一定实现.)
	xsd__string layerId;					//地图图层ID 预留字段.


	enum cusdk__EMapElementType type;		// 图元类型.

	xsd__string deviceId;					// 图元对应设备Id.
	int deviceChannel;						// 图元对应设备通道.
	enum cusdk__EDeviceChannelType channelType;	// 图元对应的设备通道的类型.
	
	cusdk__MapPosition position;			// 图元坐标.
	enum cusdk__EMapCoordType coordType;	// 坐标类型.
	
	xsd__string picUrl;						// 图元图片路径(或者图片文件名?),不一定需要..

	xsd__string description;				// 描述.

	bool storeGPSTrace;						// 是否存储轨迹信息.

	cusdk__MapPosition marPosition;			// 火星坐标的位置. (创建/修改的时候无效)
};

class cusdk__MpsDataVersion
{
	int elementVersion;                //图元数据版本号
};

class cusdk__CreateMapElementReq : public cusdk__BaseReq
{
	cusdk__MapElement mapEleInfo;		// 要创建的图元信息.
};

class cusdk__CreateMapElementRsp /* : public cusdk__BaseRsp */
{
	xsd__string mapEleId;		// 新创建的地图图元的id.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建地图图元
int cusdk__CreateMapElement( cusdk__CreateMapElementReq req, cusdk__CreateMapElementRsp& rsp );

class cusdk__ModifyMapElementReq : public cusdk__BaseReq
{
	cusdk__MapElement mapElementInfo;		// 要修改的地图图元信息.
};
class cusdk__ModifyMapElementRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改地图图元.
int cusdk__ModifyMapElement( cusdk__ModifyMapElementReq req, cusdk__ModifyMapElementRsp& rsp );

class cusdk__DelMapElementReq : public cusdk__BaseReq
{
	xsd__string mapElementId;
};
class cusdk__DelMapElementRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除地图图元.
int cusdk__DelMapElement( cusdk__DelMapElementReq req, cusdk__DelMapElementRsp& rsp );

class cusdk__QueryMapElementReq : public cusdk__BaseReq
{
	xsd__string mapId;		// 图元所属地图Id.
	bool mapIdEnabled;


 	xsd__string elementId;	// 图元Id
 	bool elementIdEnabled;
 
	enum cusdk__EMapElementType elementType;		// 图元类型.
	bool elementTypeEnabled;

	xsd__string deviceId;					// 图元对应设备Id.
	int deviceChannel;						// 图元对应设备通道.
	enum cusdk__EDeviceChannelType channelType;	// 图元对应的设备通道的类型.
	bool deviceChannelEnabled;	// 是否按上面三项查询.

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryMapElementRsp /* : public cusdk__BaseRsp */
{
	int totalNum;										// 图元数目.
	std::vector< cusdk__MapElement > mapElementList;	// 所有地图列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询地图图元列表
int cusdk__QueryMapElement( cusdk__QueryMapElementReq req, cusdk__QueryMapElementRsp& rsp );


class cusdk__SearchMapElementReq : public cusdk__BaseReq
{
	xsd__string mapId;		// 图元所属地图Id. 暂不支持jpeg地图

	// searchPosEnable 和 searchPosEnable不可同时为true;

	cusdk__MapPosition upperLeft;	// 左上坐标.
	cusdk__MapPosition bottomRight;	// 右下坐标.
	bool searchPosEnable;

	xsd__string searchKey;	// 搜索关键字 模糊匹配
	bool searchKeyEnabled;

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__SearchMapElementRsp /* : public cusdk__BaseRsp */
{
	int totalNum;										// 图元数目.
	std::vector< cusdk__MapElement > mapElementList;	// 所有地图列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 区域查询图元/搜索图元关键字
int cusdk__SearchMapElement( cusdk__SearchMapElementReq req, cusdk__SearchMapElementRsp& rsp );


class cusdk__GetMapElementImageUploadUrlReq : public cusdk__BaseReq
{
	xsd__string imageName;		// 图片名字
};
class cusdk__GetMapElementImageUploadUrlRsp /* : public cusdk__BaseRsp */
{
	xsd__string imageUploadUrl;		// 地图图片上传URL

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取上传地图图片的url.
int cusdk__GetMapElementImageUploadUrl( cusdk__GetMapElementImageUploadUrlReq req, cusdk__GetMapElementImageUploadUrlRsp& rsp );

class cusdk__QueryMapElementImageListReq : public cusdk__BaseReq
{
	
};
class cusdk__QueryMapElementImageListRsp /* : public cusdk__BaseRsp */
{
	std::vector< xsd__string > mapElementImageUrlList;	// 所有地图图元图片url列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询地图图元列表
int cusdk__QueryMapElementImageList( cusdk__QueryMapElementImageListReq req, cusdk__QueryMapElementImageListRsp& rsp );

// 纠偏

class cusdk__MapPositionMar2EarthReq : public cusdk__BaseReq
{
	cusdk__MapPosition marPos;	// 火星坐标.
};
class cusdk__MapPositionMar2EarthRsp /* : public cusdk__BaseRsp */
{
	cusdk__MapPosition earthPos;	// 地球坐标.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 加偏后的坐标转换为争取坐标.
int cusdk__MapPositionMar2Earth( cusdk__MapPositionMar2EarthReq req, cusdk__MapPositionMar2EarthRsp& rsp );

class cusdk__MapPositionEarth2MarReq : public cusdk__BaseReq
{
	cusdk__MapPosition earthPos;	// 地球坐标.
};
class cusdk__MapPositionEarth2MarRsp /* : public cusdk__BaseRsp */
{
	cusdk__MapPosition marPos;	// 火星坐标.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 正确坐标转换为加偏坐标.
int cusdk__MapPositionEarth2Mar( cusdk__MapPositionEarth2MarReq req, cusdk__MapPositionEarth2MarRsp& rsp );

// 获取告警联动数据版本.
class cusdk__GetEMapDataVersionReq : public cusdk__BaseReq
{
};
class cusdk__GetEMapDataVersionRsp /* : public cusdk__BaseRsp */
{
	cusdk__MpsDataVersion dataVersion;	// 图元数据版本.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询电子地图数据版本
int cusdk__GetEMapDataVersion( cusdk__GetEMapDataVersionReq req, cusdk__GetEMapDataVersionRsp& rsp );

// 获取告警联动数据版本.
class cusdk__GetAlarmLinkDataVersionReq : public cusdk__BaseReq
{
};
class cusdk__GetAlarmLinkDataVersionRsp /* : public cusdk__BaseRsp */
{
	xsd__string dataVersion;	// 当前的数据版本.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询告警联动数据版本
int cusdk__GetAlarmLinkDataVersion( cusdk__GetAlarmLinkDataVersionReq req, cusdk__GetAlarmLinkDataVersionRsp& rsp );

// 获取全部联动配置.
class cusdk__QueryAlarmLinkConfigReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{
	int totalNum;												// 布防配置数目.
	std::vector< cusdk__AlarmLinkConfig > alarmLinkConfigList;	// 布防配置列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询布防列表
int cusdk__QueryAlarmLinkConfig( cusdk__QueryAlarmLinkConfigReq req, cusdk__QueryAlarmLinkConfigRsp& rsp );

// 获取联动配置
class cusdk__GetAlarmLinkConfigReq : public cusdk__BaseReq
{
	cusdk__AlarmSource alarmSource;	// 告警源
};
class cusdk__GetAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{												
	cusdk__AlarmLinkConfig alarmLinkCoonfig;	// 布防配置.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取某个告警源的布防配置.
int cusdk__GetAlarmLinkConfig( cusdk__GetAlarmLinkConfigReq req, cusdk__GetAlarmLinkConfigRsp& rsp );

// 设置联动配置.
class cusdk__SetAlarmLinkConfigReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkConfig alarmLinkCoonfig;	// 布防配置.
};
class cusdk__SetAlarmLinkConfigRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置布防配置
int cusdk__SetAlarmLinkConfig( cusdk__SetAlarmLinkConfigReq req, cusdk__SetAlarmLinkConfigRsp& rsp );

// 区域布防.
class cusdk__AlarmLinkArea
{
	xsd__string id;	// 区域布防id.
	xsd__string name;	// 区域名.
	std::vector< cusdk__AlarmSource > alarmLinkSourceList;	// 区域中的告警源列表.
};

// 获取全部区域布防列表.
class cusdk__QueryAlarmLinkAreaReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{
	int totalNum;											// 布防区域数目.
	std::vector< cusdk__AlarmLinkArea > alarmLinkAreaList;	// 布防区域列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询布防区域的列表
int cusdk__QueryAlarmLinkArea( cusdk__QueryAlarmLinkAreaReq req, cusdk__QueryAlarmLinkAreaRsp& rsp );

class cusdk__CreateAlarmLinkAreaReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkArea alarmLinkAreaInfo;	// id不需要填, 由服务器生成.
};

class cusdk__CreateAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{
	xsd__string alarmLinkAreaId;	    // 联动区域ID

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 创建联动区域, .
int cusdk__CreateAlarmLinkArea( cusdk__CreateAlarmLinkAreaReq req, cusdk__CreateAlarmLinkAreaRsp& rsp );

class cusdk__DelAlarmLinkAreaReq : public cusdk__BaseReq
{
	xsd__string alarmLinkAreaId;
};
class cusdk__DelAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 删除联动区域.
int cusdk__DelAlarmLinkArea( cusdk__DelAlarmLinkAreaReq req, cusdk__DelAlarmLinkAreaRsp& rsp );

class cusdk__ModifyAlarmLinkAreaReq : public cusdk__BaseReq
{
	cusdk__AlarmLinkArea alarmLinkAreaInfo;
};
class cusdk__ModifyAlarmLinkAreaRsp /* : public cusdk__BaseRsp */
{

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改联动区域.
int cusdk__ModifyAlarmLinkArea( cusdk__ModifyAlarmLinkAreaReq req, cusdk__ModifyAlarmLinkAreaRsp& rsp );


// 用户日志


class cusdk__UserLog
{
	xsd__string logId;			// 日志编号
	xsd__string loginSession;	// 登录Session.
	xsd__string userId;			// 用户编号
	xsd__string domainName;		// 用户域名
	xsd__string userName;		// 用户名
	xsd__string ipAddr;			// ip地址. ? uas上没有.
	time_t happenedTime;	// 发生时间
	enum cusdk__EUserOperateType operateType;	// 操作类型. 因为兼容性考虑, 只支持 cusdk__UserOpTypeCuLocalRecord 及之前的日志类型, 之后的日志类型使用 operatorTypeV2.
	int operateTypeV2;			// 操作类型, 定义见 cusdk__UserOpTypeCuLocalRecord. 
	int operateResult;			// 操作结果(错误码?)
	xsd__string operateDesc;	// 操作描述
};



class cusdk__QueryUserLogCondition
{	
	xsd__string userId;
	bool userIdEnabled;

	// 	xsd__string userName;
	// 	bool userNameEnabled;

	xsd__string userLoginSession;
	bool userLoginSessionEnabled;

	xsd__string domainName;
	bool domainNameEnabled;

	time_t timeBegin;
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EUserOperateType operateType;	// 操作类型. 因为兼容性考虑, 只支持 cusdk__UserOpTypeCuLocalRecord 及之前的日志类型, 之后的日志类型使用 operatorTypeV2.
	int operateTypeV2;			// 操作类型, 定义见 cusdk__UserOpTypeCuLocalRecord.
	bool operateTypeEnabled;
};

class cusdk__QueryUserLogReq : public cusdk__BaseReq
{	
	cusdk__QueryUserLogCondition condition;

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__QueryUserLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// 日志数目.

	std::vector< cusdk__UserLog > userLogList;	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询用户日志列表.
int cusdk__QueryUserLog( cusdk__QueryUserLogReq req, cusdk__QueryUserLogRsp& rsp );

class cusdk__StartExportUserLogReq : public cusdk__BaseReq
{	
	cusdk__QueryUserLogCondition condition;
	xsd__string characterSet;				// 字符集.

	xsd__string timeZone;			// 时区, 示例: 东八区可以是 8,  +8 如果半小时, 8.5 +8.5
};

class cusdk__StartExportUserLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// 导出操作的Url.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导出用户日志列表.
int cusdk__StartExportUserLog( cusdk__StartExportUserLogReq req, cusdk__StartExportUserLogRsp& rsp );

class cusdk__GetExportUserLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__GetExportUserLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// 导出进度百分比 1-100%

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询用户日志导出的进度.
int cusdk__GetExportUserLogProcess( cusdk__GetExportUserLogProcessReq req, cusdk__GetExportUserLogProcessRsp& rsp );

class cusdk__StopExportUserLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__StopExportUserLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消用户日志导出,删除文件.
int cusdk__StopExportUserLog( cusdk__StopExportUserLogReq req, cusdk__StopExportUserLogRsp& rsp );

class cusdk__DeviceLog
{
	xsd__string logId;						// 日志编号
	xsd__string deviceId;						// 设备编号
//	xsd__string deviceDomainName;				// 设备所属域名
	xsd__string deviceName;					// 设备名称
	xsd__string deviceIp;					// 设备IP地址.
	xsd__string deviceAlias;					// 设备别名
	time_t happenedTime;					// 发生时间
	enum cusdk__EDeviceLogType logType;		// 日志类型
	xsd__string logDesc;					// 日志描述
};

class cusdk__QueryDeviceLogCondition
{	
	xsd__string deviceId;		// 设备Id.
	bool deviceIdEnabled;

	xsd__string deviceName;		// 设备名称.
	bool deviceNameEnabled;

	time_t timeBegin;			// 时间段.
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EDeviceLogType logType;	// 日志类型.
	bool logTypeEnabled;
};

class cusdk__QueryDeviceLogReq : public cusdk__BaseReq
{	
	cusdk__QueryDeviceLogCondition condition;

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__QueryDeviceLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// 布防配置数目.

	std::vector< cusdk__DeviceLog > deviceLogList;	// 设备日志列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询设备日志.
int cusdk__QueryDeviceLog( cusdk__QueryDeviceLogReq req, cusdk__QueryDeviceLogRsp& rsp );

class cusdk__StartExportDeviceLogReq : public cusdk__BaseReq
{	
	cusdk__QueryDeviceLogCondition condition;
	xsd__string characterSet;				// 字符集.

	xsd__string timeZone;			// 时区, 示例: 东八区可以是 8,  +8 如果半小时, 8.5 +8.5
};

class cusdk__StartExportDeviceLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// 导出操作的Url.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导出设备日志列表.
int cusdk__StartExportDeviceLog( cusdk__StartExportDeviceLogReq req, cusdk__StartExportDeviceLogRsp& rsp );

class cusdk__GetExportDeviceLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__GetExportDeviceLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// 导出进度百分比 1-100%

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询设备日志导出的进度.
int cusdk__GetExportDeviceLogProcess( cusdk__GetExportDeviceLogProcessReq req, cusdk__GetExportDeviceLogProcessRsp& rsp );

class cusdk__StopExportDeviceLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__StopExportDeviceLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消设备日志导出,删除文件.
int cusdk__StopExportDeviceLog( cusdk__StopExportDeviceLogReq req, cusdk__StopExportDeviceLogRsp& rsp );

//class cusdk__DeleteDeviceLogReq : public cusdk__BaseReq
//{	
//	std::vector< xsd__string > alarmLogIdList;	// 要删除的日志Id列表.
//};
//
//class cusdk__DeleteDeviceLogRsp /* : public cusdk__BaseRsp */
//{	
//
//xsd__string session;	// 冗余 , 为了查问题方便过滤.
//long errorCode;
//};
////gsoap cusdk service method-documentation: 删除设备日志日志.
//int cusdk__DeleteDeviceLog( cusdk__DeleteDeviceLogReq req, cusdk__DeleteDeviceLogRsp& rsp );

class cusdk__AlarmLog
{
	xsd__string logId;			// 日志编号
	xsd__string deviceId;       // 设备编号
	xsd__string deviceDomainName; // 设备所属域编号
	xsd__string deviceName;         // 设备名称
	xsd__string deviceAlias;        // 设备别名
	int alarmChannelId;            // 告警通道
	enum cusdk__EAlarmType alarmType;               // 告警类型 
	enum cusdk__EAlarmStatus alarmStatus;           // 告警状态
	time_t occurTime;       // 告警时间
	time_t resumeTime;      // 恢复时间
	xsd__string alarmName;           // 告警别名
	xsd__string description;          // 告警描述
};

class cusdk__QueryAlarmLogCondition
{	
	xsd__string deviceId;
	bool deviceIdEnabled;

	int alarmChannelId;
	bool alarmChannelIdEnabled;

	xsd__string alarmName;
	bool alarmNameEnabled;

	xsd__string deviceDomainName;
	bool deviceDomainNameEnabled;

	time_t timeBegin;
	time_t timeEnd;
	bool timeEnabled;

	enum cusdk__EAlarmType alarmType;
	bool alarmTypeEnabled;
};

class cusdk__QueryAlarmLogReq : public cusdk__BaseReq
{	
	cusdk__QueryAlarmLogCondition condition;

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};

class cusdk__QueryAlarmLogRsp /* : public cusdk__BaseRsp */
{	
	int totalNum;			// 告警日志数目.

	std::vector< cusdk__AlarmLog > alarmLogList;	// 设备日志列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询告警日志.
int cusdk__QueryAlarmLog( cusdk__QueryAlarmLogReq req, cusdk__QueryAlarmLogRsp& rsp );

class cusdk__StartExportAlarmLogReq : public cusdk__BaseReq
{	
	cusdk__QueryAlarmLogCondition condition;
	xsd__string characterSet;				// 字符集.

	xsd__string timeZone;			// 时区, 示例: 东八区可以是 8,  +8 如果半小时, 8.5 +8.5
};

class cusdk__StartExportAlarmLogRsp /* : public cusdk__BaseRsp */
{	
	xsd__string exportFileUrl;	// 导出操作的Url.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导出告警日志.
int cusdk__StartExportAlarmLog( cusdk__StartExportAlarmLogReq req, cusdk__StartExportAlarmLogRsp& rsp );

class cusdk__GetExportAlarmLogProcessReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__GetExportAlarmLogProcessRsp /* : public cusdk__BaseRsp */
{	
	int percentage;		// 导出进度百分比 1-100%

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询告警日志导出的进度.
int cusdk__GetExportAlarmLogProcess( cusdk__GetExportAlarmLogProcessReq req, cusdk__GetExportAlarmLogProcessRsp& rsp );

class cusdk__StopExportAlarmLogReq : public cusdk__BaseReq
{	
	xsd__string exportFileUrl;	// 导出操作的Url.
};

class cusdk__StopExportAlarmLogRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消告警日志导出,删除日志文件.
int cusdk__StopExportAlarmLog( cusdk__StopExportAlarmLogReq req, cusdk__StopExportAlarmLogRsp& rsp );


//class cusdk__DeleteAlarmLogReq : public cusdk__BaseReq
//{	
//	std::vector< xsd__string > alarmLogIdList;	// 要删除的日志Id列表.
//};
//
//class cusdk__DeleteAlarmLogRsp /* : public cusdk__BaseRsp */
//{	
//
//xsd__string session;	// 冗余 , 为了查问题方便过滤.
//long errorCode;
//};
////gsoap cusdk service method-documentation: 删除告警日志.
//int cusdk__DeleteAlarmLog( cusdk__DeleteAlarmLogReq req, cusdk__DeleteAlarmLogRsp& rsp );

class cusdk__GetLogConfigReq : public cusdk__BaseReq
{		
};

class cusdk__GetLogConfigRsp /* : public cusdk__BaseRsp */
{	
	int maxUserLogSavedDays;	// 最大的用户日志保存天数.
	int maxUserLogSavedNumber;	// 最大的用户日志保存条数.
	bool enableUserLog;			// 是否启用用户日志.

	int maxDeviceLogSavedNumber;// 最大的设备日志保存条数.
	int maxDeviceLogSavedDays;	// 最大的设备日志保存天数.
	bool enableDeviceLog;		// 是否启用设备日志.

	int maxAlarmLogSavedNumber;	// 最大的告警日志保存条数.
	int maxAlarmLogSavedDays;	// 最大的告警日志保存天数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取日志配置.
int cusdk__GetLogConfig( cusdk__GetLogConfigReq req, cusdk__GetLogConfigRsp& rsp );

class cusdk__SetLogConfigReq : public cusdk__BaseReq
{	
	int maxUserLogSavedDays;	// 最大的用户日志保存天数.
	int maxUserLogSavedNumber;	// 最大的用户日志保存条数.
	bool enableUserLog;			// 是否启用用户日志.

	int maxDeviceLogSavedNumber;// 最大的设备日志保存条数.
	int maxDeviceLogSavedDays;	// 最大的设备日志保存天数.
	bool enableDeviceLog;		// 是否启用设备日志.

	int maxAlarmLogSavedNumber;	// 最大的告警日志保存条数.
	int maxAlarmLogSavedDays;	// 最大的告警日志保存天数.
};
class cusdk__SetLogConfigRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置日志配置.
int cusdk__SetLogConfig( cusdk__SetLogConfigReq req, cusdk__SetLogConfigRsp& rsp );




// class cusdk__MediaNetInfo
// {
// 	cusdk__NatInfo videoRtpNatInfo;
// 	cusdk__NatInfo videoRtcpNatInfo;
// 	cusdk__NatInfo audioRtpNatInfo;
// 	cusdk__NatInfo audioRtcpNatInfo;
// };

class cusdk__CallPuReq : public cusdk__BaseReq
{	
	xsd__int encodeChnId;	// 客户端的音视频编码通道Id. 


	xsd__string devId;		// 要呼叫的设备Id.
	xsd__int devDecChnId;	// 要呼叫的设备解码通道Id.

// 	xsd__int devVideoDecChnId;	// 要呼叫的设备视频解码通道Id.
// 	xsd__int devAudioDecChnId;	// 要呼叫的设备通道Id.
		
	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 呼叫方的媒体信息描述.
	
	int transMode;		// 定义参见 cusdk__ETransMode
// 	
// 	std::vector< enum cusdk__EPlayMode > supportedCallMode;		// 能支持的呼叫模式. 把最期望的模式放在前面
// 
// 	std::vector< cusdk__MediaFormat > supportedVideoFormat;		// 能支持的视频编码格式. 还需要扩展. 分辨率, 码率等必要的编码参数信息, 参考标准.
// 	std::vector< cusdk__MediaFormat > supportedAudioFormat;		// 能支持的音频编码格式.
// 	std::vector<cusdk__MediaNetInfo> localNetInfoList;  // 音频源的多IP情况	
};

class cusdk__CallPuRsp /* : public cusdk__BaseRsp */
{		
	//std::vector<cusdk__MediaNetInfo> remoteNetInfoList; //平台也多IP情况

	std::vector< cusdk__MediaDescription > mediaDescriptionList;	// 协商后的媒体信息描述. 如果为空就表示不需要创建码流.
	std::vector< cusdk__MediaExtInfo > mediaExtInfo;	// 媒体扩展信息.


// 	enum cusdk__EPlayMode callMode;	// 协商后确定的音视频呼叫模式. 音视频呼叫 or 音频呼叫 or 视频呼叫?
// 	cusdk__MediaFormat supportedVideoFormat;		// 协商后确定的视频编码格式. 还需要扩展. 分辨率, 码率等必要的编码参数信息, 应该参考标准.
// 	cusdk__MediaFormat supportedAudioFormat;		// 协商后确定的音频编码格式.
// 	bool needCreateNewVideoSwitch;		// 是否需要发送新的视频.
// 	bool needCreateNewAudioSwitch;		// 是否需要发送音频.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 音视频呼叫 音频呼叫前端, 或者音视频呼叫 CU.
int cusdk__CallPu( cusdk__CallPuReq req, cusdk__CallPuRsp& rsp );

class cusdk__HangUpPuReq : public cusdk__BaseReq
{	
	xsd__int encodeChnId;		// 客户端的音频编码通道Id. 
	
	xsd__string devId;			// 呼叫的设备Id.
	xsd__int devDecChnId;	// 要呼叫的设备解码通道Id.

// 	xsd__int devVideoDecChnId;	// 要呼叫的设备视频解码通道Id.
// 	xsd__int devAudioDecChnId;	// 呼叫的设备通道Id.

//	enum cusdk__EPlayMode callMode;		// 要挂断的是音视频 or 音频 or 视频? 或者通道Id用无效值来表示无效??
};

class cusdk__HangUpPuRsp /* : public cusdk__BaseRsp */
{		
	//int remainAudioSwitch;	// 该音频通道剩余的音频交换数. 如果为0, 则停止音频编码和发送.
// 	bool needStopVideoSwitch;	// 是否该停止发送视频.
// 	bool needStopAudioSwitch;	// 是否该停止发送音频.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 挂断音视频呼叫
int cusdk__HangUpPu( cusdk__HangUpPuReq req, cusdk__HangUpPuRsp& rsp );



class cusdk__VideoMode
{
	enum cusdk__EVideoFormat videoFormat;	// 视频编码格式. 只允许设置 cusdk__VF_H265 之前的值.
	int videoFormatV2;						// 视频编码格式. 为了兼容性,改为int, 值的定义参见 cusdk__EVideoFormat.
	enum cusdk__EVideoResolution videoRes;	//图象分辨率;
	xsd__string videoResStr;				//字符表示的分辨率. 如果#define VSIP_ENTITY_SUPPORT_RESOLUTION_WH 0x3D 这个能力集存在,就使用字符串的分辨率. +by lizhixing@2014年3月28日
};

class cusdk__VideoEncodeParam
{
	cusdk__VideoMode videoMode;	// 编码格式和分辨率
	int frameRate;			//帧率 1-25;
	enum cusdk__EVideoQuality quality;		//图象质量; <见pu\MtCtrl.h>
	int bitRate;			//编码比特率 64K-4M;
	int interval;			//关键帧间隔 25-2500;
	int brightness;			//亮度 -127～127
	int contrast;			//对比度 -127～127
	int saturation;			//饱和度 -127～127
	int sharpness;			// 锐度. -127-127
};

class cusdk__AudioMode
{
	int audioFormat;	// 音频编码格式,定义参见 cusdk__EAudioFormat
};

class cusdk__AudioEncoderParam
{
	int audioForamt;	// 音频编码格式. 定义见 cusdk__EAudioFormat
	int volume;			// 音量,范围 0-25
	bool AECState;		// 回声抵消是否开启
	int sampleRate;		// 采样率.
	int sampleBits;		// 采样位数
	int channleNum;		// 声道数目.
	int samplePerFrame;	// 每帧采样样本数
};

class cusdk__GetDeviceEncoderParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int	encodeChnId;		// 编码通道Id.
};

class cusdk__GetDeviceEncoderParamRsp /* : public cusdk__BaseRsp */
{		
	cusdk__VideoEncodeParam  videoEncParam;   //当前视频编码参数
	std::vector< cusdk__VideoMode >  suportedVideoModeList;  //所有支持的视频模式

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备编码参数.
int cusdk__GetDeviceEncoderParam( cusdk__GetDeviceEncoderParamReq req, cusdk__GetDeviceEncoderParamRsp& rsp );


class cusdk__GetDeviceAudioEncoderParamReq : public cusdk__BaseReq
{
	xsd__string deviceId;	// 设备Id
	int	encodeChnId;		// 编码通道Id.
};

class cusdk__GetDeviceAudioEncoderParamRsp /* : public cusdk__BaseRsp */
{
	cusdk__AudioEncoderParam audioEncParam;	// 当前的音频编码参数.
	std::vector< cusdk__AudioMode > supportedAudioModeList;	// 支持的音频格式.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备编码参数.
int cusdk__GetDeviceAudioEncoderParam(cusdk__GetDeviceAudioEncoderParamReq req, cusdk__GetDeviceAudioEncoderParamRsp& rsp);


class cusdk__SetDeviceEncoderParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int	encodeChnId;		// 编码通道Id.

	cusdk__VideoEncodeParam  videoParam;      //当前视频编码参数
};

class cusdk__SetDeviceEncoderParamRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置设备编码参数.
int cusdk__SetDeviceEncoderParam( cusdk__SetDeviceEncoderParamReq req, cusdk__SetDeviceEncoderParamRsp& rsp );

class cusdk__PuVidSrcOSDItem
{
	int			type;			//字幕类型，参见emPuVidSrcOsdCfgType
	int			id;				//字幕编号，从0开始，当字幕类型为台标和横幅时，标识台标或横幅编号；若为并口告警字幕时，标识并口号
	bool		enabled;		//字幕使能
	xsd__string content;		//字幕内容，最大长度128
};
typedef std::vector< cusdk__PuVidSrcOSDItem > cusdk__PuVidSrcOSDList;

class cusdk__SetPuVidSrcOSDReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.

	cusdk__PuVidSrcOSDList vidSrcOSDList;	// OSD 配置列表.
};

class cusdk__SetPuVidSrcOSDRsp /* : public cusdk__BaseRsp */
{		
	std::vector< int > OSDItemErrors;	// 每个 cusdk__PuVidSrcOSDItem 设置时的错误.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置设备OSD信息.
int cusdk__SetPuVidSrcOSD( cusdk__SetPuVidSrcOSDReq req, cusdk__SetPuVidSrcOSDRsp& rsp );

class cusdk__GetPuVidSrcOSDReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.
};

class cusdk__GetPuVidSrcOSDRsp /* : public cusdk__BaseRsp */
{	
	cusdk__PuVidSrcOSDList vidSrcOSDList;	// OSD 配置列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备OSD信息.
int cusdk__GetPuVidSrcOSD( cusdk__GetPuVidSrcOSDReq req, cusdk__GetPuVidSrcOSDRsp& rsp );

class cusdk__SetDeviceNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	
	xsd__string deviceName;	// 新设别名.
};

class cusdk__SetDeviceNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改设备的名称
int cusdk__SetDeviceName( cusdk__SetDeviceNameReq req, cusdk__SetDeviceNameRsp& rsp );

class cusdk__GetCameraProtocolTypeReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.
};

class cusdk__GetCameraProtocolTypeRsp /* : public cusdk__BaseRsp */
{		
	enum cusdk__ECameraProtocolType cameraProtocolType;	// 摄像头协议类型.
	xsd__int cameraId;	// 摄像头地址码/ID

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取视频源的摄像头协议类型
int cusdk__GetCameraProtocolType( cusdk__GetCameraProtocolTypeReq req, cusdk__GetCameraProtocolTypeRsp& rsp );

class cusdk__SetCameraProtocolTypeReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.

	enum cusdk__ECameraProtocolType cameraProtocolType;	// 摄像头协议类型.
	xsd__int cameraId;	// 摄像头地址码/ID
};

class cusdk__SetCameraProtocolTypeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 修改视频源的名称
int cusdk__SetCameraProtocolType( cusdk__SetCameraProtocolTypeReq req, cusdk__SetCameraProtocolTypeRsp& rsp );

// 区域.
class cusdk__Area
{
	int x;
	int y;
	int width;
	int height;
};

// 移动侦测参数
class cusdk__MotionDetectAreaConfig
{
	bool enable;			// 是否启用.
	cusdk__Area detectArea;	// 侦测的区域.
	int alarmRate;			// 1-100 告警产生灵敏度.
	int restoreRate;		// 1-100 恢复灵敏度.
};

class cusdk__GetMotionDetectConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.
};

class cusdk__GetMotionDetectConfigRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__MotionDetectAreaConfig > motionDetectAreaConfigList;	// 所有的侦测区域列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取移动侦测配置.
int cusdk__GetMotionDetectConfig( cusdk__GetMotionDetectConfigReq req, cusdk__GetMotionDetectConfigRsp& rsp );

class cusdk__SetMotionDetectConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.

	std::vector< cusdk__MotionDetectAreaConfig > motionDetectAreaConfigList;	// 所有的侦测区域列表.
};

class cusdk__SetMotionDetectConfigRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置移动侦测配置.
int cusdk__SetMotionDetectConfig( cusdk__SetMotionDetectConfigReq req, cusdk__SetMotionDetectConfigRsp& rsp );

// 图像遮蔽参数
class cusdk__ShadeAreaConfig
{
	bool enable;			// 是否启用.
	cusdk__Area shadeArea;	// 遮蔽的区域.
};

class cusdk__GetShadeConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.
};

class cusdk__GetShadeConfigRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__ShadeAreaConfig > shadeAreaConfigList;	// 所有的遮蔽区域列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取移动侦测配置.
int cusdk__GetShadeConfig( cusdk__GetShadeConfigReq req, cusdk__GetShadeConfigRsp& rsp );

class cusdk__SetShadeConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int videoSrcId;			// 视频源Id.

	std::vector< cusdk__ShadeAreaConfig > shadeAreaConfigList;	// 所有的侦测区域列表.
};

class cusdk__SetShadeConfigRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置移动侦测配置.
int cusdk__SetShadeConfig( cusdk__SetShadeConfigReq req, cusdk__SetShadeConfigRsp& rsp );

class cusdk__GetDeviceChannelNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	enum cusdk__EDeviceChannelType chnType;	// 通道类型.
	int chnId;	// 通道Id.	// -1 代表查所有通道的别名, 否则是具体某一个通道.
};

class cusdk__ChannelName
{
	int chnId;				// 通道Id.
	xsd__string chnName;	// 通道名称.
	double longitude;		//视频源经度
	double latitude;		//视频源纬度
};

class cusdk__GetDeviceChannelNameRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__ChannelName > chnNameList;	// 设备的所有该类型

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备通道别名.
int cusdk__GetDeviceChannelName( cusdk__GetDeviceChannelNameReq req, cusdk__GetDeviceChannelNameRsp& rsp );


class cusdk__SetDeviceChannelNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	enum cusdk__EDeviceChannelType chnType;	// 通道类型.
	int chnId;				// 通道Id.
	xsd__string chnName;	// 通道名称.
	double longitude;		//视频源经度
	double latitude;		//视频源纬度
};

class cusdk__SetDeviceChannelNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	
};
//gsoap cusdk service method-documentation: 设置设备通道别名.
int cusdk__SetDeviceChannelName( cusdk__SetDeviceChannelNameReq req, cusdk__SetDeviceChannelNameRsp& rsp );

//同步视频源别名
class cusdk__GetSyncVideoSrcNameEnableReq
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
};

class cusdk__GetSyncVideoSrcNameEnableRsp
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	bool enablePu2Platform; //前端同步到平台
	bool enablePlatform2Pu; //平台同步到前端
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取视频源名称同步使能.
int cusdk__GetSyncVideoSrcNameEnable(cusdk__GetSyncVideoSrcNameEnableReq req, cusdk__GetSyncVideoSrcNameEnableRsp& rsp);

class cusdk__SetSyncVideoSrcNameEnableReq
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	bool enablePu2Platform; //前端同步到平台
	bool enablePlatform2Pu; //平台同步到前端
};

class cusdk__SetSyncVideoSrcNameEnableRsp
{
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置视频源名称同步使能.
int cusdk__SetSyncVideoSrcNameEnable(cusdk__SetSyncVideoSrcNameEnableReq req, cusdk__SetSyncVideoSrcNameEnableRsp& rsp);

class cusdk__GetPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int vieoSrcId;			// 视频源Id.
};

class cusdk__PresetName
{
	int presetId;			// 预置位Id.
	xsd__string presetName;	// 预置位名称.
};

class cusdk__GetPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		
	std::vector< cusdk__PresetName > presetNameList;	// 视频源的所有设过的预置位别名.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取摄像头预置位别名.
int cusdk__GetPtzPresetName( cusdk__GetPtzPresetNameReq req, cusdk__GetPtzPresetNameRsp& rsp );


class cusdk__SetPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int vieoSrcId;			// 视频源Id.
	int presetId;			// 预置位Id.

	xsd__string presetName;	// 预置位名称.
};

class cusdk__SetPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置视频源别名.
int cusdk__SetPtzPresetName( cusdk__SetPtzPresetNameReq req, cusdk__SetPtzPresetNameRsp& rsp );

class cusdk__DelPtzPresetNameReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int vieoSrcId;			// 视频源Id.
	int presetId;			// 预置位Id.
};

class cusdk__DelPtzPresetNameRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置视频源别名.
int cusdk__DelPtzPresetName( cusdk__DelPtzPresetNameReq req, cusdk__DelPtzPresetNameRsp& rsp );


class cusdk__VsipChannelDataReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int deviceIndex;		// 设备序号. 当NVRSVR设备的时候有效, 普通设备都是0.
	xsd__string vsipBase64DataReq;	// vsip数据包.
};

class cusdk__VsipChannelDataRsp /* : public cusdk__BaseRsp */
{		
	xsd__string vsipBase64DataRsp;	// vsip数据包.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: VSIP透明数据通道
int cusdk__VsipChannelData( cusdk__VsipChannelDataReq req, cusdk__VsipChannelDataRsp& rsp );

// 设备 GPS 配置.
class cusdk__TransDataParam
{
	bool       enable;             	                   //是否订阅
	xsd__string     gpsParam;                          //gps订阅参数
};

class cusdk__SetTransDataParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int        type;                                   //透明数据类型
	int        subType;                                //透明数据子类型
	cusdk__TransDataParam param;	// 配置.
};

class cusdk__SetTransDataParamRsp /* : public cusdk__BaseRsp */
{		

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置设备透明通道参数.
int cusdk__SetTransDataParam( cusdk__SetTransDataParamReq req, cusdk__SetTransDataParamRsp& rsp );

class cusdk__GetTransDataParamReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id
	int        type;                                   //透明数据类型
	int        subType;                                //透明数据子类型
};

class cusdk__GetTransDataParamRsp /* : public cusdk__BaseRsp */
{	
	cusdk__TransDataParam param;	// 配置.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备透明通道参数.
int cusdk__GetTransDataParam( cusdk__GetTransDataParamReq req, cusdk__GetTransDataParamRsp& rsp );

// 轨迹信息 查询
class cusdk__QueryGPSTraceReq : public cusdk__BaseReq
{
	xsd__string deviceId;	// 设备Id.

	cusdk__TimeRange timeRange;	// 时间跨度.

	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryGPSTraceRsp /* : public cusdk__BaseRsp */
{
	int totalNum;								// 总数.
	std::vector< cusdk__GPSInfo > gpsInfoList;	// GPS设备列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询设备的GPS轨迹.
int cusdk__QueryGPSTrace( cusdk__QueryGPSTraceReq req, cusdk__QueryGPSTraceRsp& rsp );
// 

class cusdk__GetMapConfigReq : public cusdk__BaseReq
{	
};

class cusdk__GetMapConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__int maxGPSInfoSavedDays;	// GPS信息最大存储天数
	xsd__int maxGPSInfoSavedSize;	// 最大的GPS存储占用空间,单位(M).
	xsd__int maxGPSInfoSavedDeviceNumber;	// 最大的GPS存储路数.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取电子地图的配置信息.
int cusdk__GetMapConfig( cusdk__GetMapConfigReq req, cusdk__GetMapConfigRsp& rsp );


class cusdk__SetMapConfigReq : public cusdk__BaseReq
{	
	xsd__int maxGPSInfoSavedDays;	// GPS信息最大存储天数
	xsd__int maxGPSInfoSavedSize;	// 最大的GPS存储占用空间,单位(M).
	xsd__int maxGPSInfoSavedDeviceNumber;	// 最大的GPS存储路数.

};

class cusdk__SetMapConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置电子地图的配置信息.
int cusdk__SetMapConfig( cusdk__SetMapConfigReq req, cusdk__SetMapConfigRsp& rsp );

class cusdk__GetDeviceSerialConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id.
	xsd__int serialPortID; //串口号
};

class cusdk__GetDeviceSerialConfigRsp 
{		
	xsd__string deviceId;	// 设备Id.
	xsd__int serialPortID; //串口号

	xsd__int baudRate; //串口波特率	
	xsd__int comPortType; //串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485

	xsd__boolean rS232Enable;
	xsd__boolean rS422Enable;
	xsd__boolean rS485Enable;

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备的串口配置信息.
int cusdk__GetDeviceSerialConfig( cusdk__GetDeviceSerialConfigReq req, cusdk__GetDeviceSerialConfigRsp& rsp );


class cusdk__SetDeviceSerialConfigReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	// 设备Id.
	xsd__int serialPortID; //串口号

	xsd__int baudRate; //串口波特率
	xsd__int comPortType; //串口类型 0:不使用, 1:RS232, 2:RS422, 3:RS485
};

class cusdk__SetDeviceSerialConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置设备的的串口配置信息.
int cusdk__SetDeviceSerialConfig( cusdk__SetDeviceSerialConfigReq req, cusdk__SetDeviceSerialConfigRsp& rsp );

class cusdk__GetDeviceGBIDReq : public cusdk__BaseReq
{	
	cusdk__DevChannel deviceChn;	// 设备的编码通道。
};

class cusdk__GetDeviceGBIDRsp /* : public cusdk__BaseRsp */
{		
	xsd__string deviceGBID;	// 该通道对应的GB（国标） ID。
	xsd__string gbsID;		// 通信的GBS编号  也即系统目录编号

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取一个设备通道的国标ID
int cusdk__GetDeviceGBID( cusdk__GetDeviceGBIDReq req, cusdk__GetDeviceGBIDRsp& rsp );

// class cusdk__GetDeviceGBIDBatchReq : public cusdk__BaseReq
// {	
// 	std::vector<cusdk__DevChannel> deviceChns;	// 设备的编码通道。
// };
// 
// class cusdk__DeviceChnGBIDInfo
// {
// 	cusdk__DevChannel deviceChn;	// 设备通道信息
// 	xsd__string deviceGBID;			// 通道国标ID
// 	xsd__string gbsID;				// 通道的GBS编码,也即系统目录编号
// };
// 
// class cusdk__GetDeviceGBIDBatchRsp /* : public cusdk__BaseRsp */
// {		
// 	std::vector< cusdk__DeviceChnGBIDInfo > deviceGBIDInfos;
// 
// 	xsd__string session;	// 冗余 , 为了查问题方便过滤.
// 	long errorCode;
// };
// 
// //gsoap cusdk service method-documentation: 批量获取设备通道的国标ID
// int cusdk__GetDeviceGBIDBatch( cusdk__GetDeviceGBIDBatchReq req, cusdk__GetDeviceGBIDBatchRsp& rsp );

class cusdk__GetCivilcodeReq : public cusdk__BaseReq
{	
	xsd__string deviceGroupId;	// 设备组的ID。
};

class cusdk__GetCivilcodeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string civilcode;	// 设备组对应的行政区划civilcode.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取一个设备组的行政区划编号
int cusdk__GetCivilcode( cusdk__GetCivilcodeReq req, cusdk__GetCivilcodeRsp& rsp );

class cusdk__GetDeviceChnByGBIDReq : public cusdk__BaseReq
{	
	xsd__string deviceGBID;	// 设备的国标编码。
};

class cusdk__GetDeviceChnByGBIDRsp /* : public cusdk__BaseRsp */
{		
	xsd__string deviceID;	// 设备的ID,形式是 uuid@域名 
	xsd__int chnId;			// 设备的通道id.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取设备的GBID获取设备的ID和通道.
int cusdk__GetDeviceChnByGBID( cusdk__GetDeviceChnByGBIDReq req, cusdk__GetDeviceChnByGBIDRsp& rsp );


class cusdk__GetCAPSConfigReq : public cusdk__BaseReq
{	
};

class cusdk__GetCAPSConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__int captureIntervalSecs;	// 抓拍时间间隔,单位秒.
	xsd__string wechatUrl;			// 微信平台网页URL.
	xsd__string wechatTitle;		// 微信平台的标题.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 获取抓拍配置
int cusdk__GetCAPSConfig( cusdk__GetCAPSConfigReq req, cusdk__GetCAPSConfigRsp& rsp );

class cusdk__SetCAPSConfigReq : public cusdk__BaseReq
{	
	xsd__int captureIntervalSecs;	// 抓拍时间间隔,单位秒.
	xsd__string wechatTitle;		// 微信平台的标题.
};

class cusdk__SetCAPSConfigRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 设置抓拍配置
int cusdk__SetCAPSConfig( cusdk__SetCAPSConfigReq req, cusdk__SetCAPSConfigRsp& rsp );

class cusdk__CAPSDevice
{
	xsd__string devId;		// 设备原始ID.
	xsd__int chnId;			// 设备通道.
	xsd__string name;		// 名称.

	xsd__string devGBId;	// 设备的国标.
	xsd__string domainGBId;	// 域的国标ID.

	//xsd__int videosrcId;		// 视频源ID. ?
};

class cusdk__QueryCAPSDeviceListReq : public cusdk__BaseReq
{
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.
};
class cusdk__QueryCAPSDeviceListRsp /* : public cusdk__BaseRsp */
{
	int totalNum;								// 总数.
	std::vector<cusdk__CAPSDevice> devList;		// 设备列表.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 查询划归的设备.
int cusdk__QueryCAPSDeviceList( cusdk__QueryCAPSDeviceListReq req, cusdk__QueryCAPSDeviceListRsp& rsp );

class cusdk__CAPSAddDeviceReq : public cusdk__BaseReq
{	
	std::vector<cusdk__CAPSDevice> devList;		// 设备列表.
};

class cusdk__CAPSAddDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 划归一个设备
int cusdk__CAPSAddDevice( cusdk__CAPSAddDeviceReq req, cusdk__CAPSAddDeviceRsp& rsp );

// gsoap cusdk service method-documentation: 修改一个设备.
class cusdk__CAPSModifyDeviceReq : public cusdk__BaseReq
{	
	cusdk__CAPSDevice device;	// 修改的设备.
};

class cusdk__CAPSModifyDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 划归一个设备
int cusdk__CAPSModifyDevice( cusdk__CAPSModifyDeviceReq req, cusdk__CAPSModifyDeviceRsp& rsp );

class cusdk__CAPSDelDeviceReq : public cusdk__BaseReq
{	
	cusdk__DevChannel device;	// 要删除的设备.
};

class cusdk__CAPSDelDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 删除一个设备.
int cusdk__CAPSDelDevice( cusdk__CAPSDelDeviceReq req, cusdk__CAPSDelDeviceRsp& rsp );


class cusdk__GetOfflineDeviceStatReq : public cusdk__BaseReq
{	
	xsd__int beginIndex;	// 起始序号.
	xsd__int num;			// 本次请求要的总数.

	xsd__string domanName;	// 要统计的域.

	xsd__string queryTag;	// 一次查询请求的标记，用来区分不同的查询。
};

class cusdk__DeviceVideosrcId
{
	xsd__string deviceId;	// 设备ID.
	xsd__int videoSrcId;	// 视频源ID.
	xsd__boolean deviceOnline;      // 视频源所在的设备是否在线
};

class cusdk__GetOfflineDeviceStatRsp /* : public cusdk__BaseRsp */
{		
//	vector< xsd__string > offlineDeviceList;	// 按设备来统计.

	std::vector< cusdk__DeviceVideosrcId > offlineDeviceVideosrcList;	// 不在线的视频源列表.

	std::vector< cusdk__DeviceVideosrcId > idleStatVideoSrcList;    //处于Idle状态的视频源列表

	xsd__int totalOfflineDeviceVideosrcNum;	// 总数,用于分页.

	xsd__int onlineDeviceNum;

	xsd__int idleDeviceNum; //idle状态

	xsd__string queryTag;	// 冗余 , 为了查问题方便
	xsd__string domanName;	// 冗余 , 为了查问题方便

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 获取不在线设备信息
int cusdk__GetOfflineDeviceStat( cusdk__GetOfflineDeviceStatReq req, cusdk__GetOfflineDeviceStatRsp& rsp );

class cusdk__UpdateCUPropertyReq : public cusdk__BaseReq
{	
	xsd__string clientLocalIP;	// CU的本地IP.
	enum cusdk__ENATMode platNatMode;	// 手工指定平台的NAT模式.
	xsd__boolean clientSupportNatProbe;  //标识客户端是否支持NAT穿透
};

class cusdk__UpdateCUPropertyRsp /* : public cusdk__BaseRsp */
{		
	enum cusdk__ENATMode cuNatMode;		// 检测到的CU的NAT模式.
	enum cusdk__ENATMode platNatMode;	// 检测到的平台的NAT模式.
	xsd__string platNATIP;		// 平台接收NAT包的IP.
	xsd__int platNATPort;		// 平台接收的NAT包的端口
	xsd__boolean platSupportForward; //平台是否支持代理转发码流(主要用于对称NAT环境下，平台录像的播放及下载)

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 获取不在线设备信息
int cusdk__UpdateCUProperty( cusdk__UpdateCUPropertyReq req, cusdk__UpdateCUPropertyRsp& rsp );

class cusdk__RestartDeviceReq : public cusdk__BaseReq
{	
	xsd__string deviceId;	 // 设备ID
};

class cusdk__RestartDeviceRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 重启设备(目前只支持国标设备)
int cusdk__RestartDevice( cusdk__RestartDeviceReq req, cusdk__RestartDeviceRsp& rsp );


////////////////////////按组给用户划归或取消划归设备///////////////////////////////////
class cusdk__UserAddDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//用户id
	xsd__string devgrpid;		//设备组id	

	bool usePri;						// 是否使用以下指定设备权限.
	std::set<  int > devPriSet;			// 设备权限列表. 定义见 cusdk__EDevicePri
	std::set<  int > videoSrcPriSet;	// 视频源权限列表. 定义见 cusdk__EDevicePri
	int ptzLevel;						// ptz控制等级.
};

class cusdk__UserAddDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 划归一个设备组给用户
int cusdk__UserAddDeviceGroup( cusdk__UserAddDeviceGroupReq req, cusdk__UserAddDeviceGroupRsp& rsp );

class cusdk__UserDelDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//用户id
	xsd__string devgrpid;		//设备组id	
};

class cusdk__UserDelDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 取消划归一个用户的设备组
int cusdk__UserDelDeviceGroup( cusdk__UserDelDeviceGroupReq req, cusdk__UserDelDeviceGroupRsp& rsp );
////////////////////////按组给用户组划归或取消划归设备///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserGroupAddDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//用户组id
	xsd__string devgrpid;		//设备组id	

	bool usePri;						// 是否使用以下指定设备权限.
	std::set<  int > devPriSet;			// 设备权限列表. 定义见 cusdk__EDevicePri
	std::set<  int > videoSrcPriSet;	// 视频源权限列表. 定义见 cusdk__EDevicePri
	int ptzLevel;						// ptz控制等级.
};

class cusdk__UserGroupAddDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 划归一个设备组给用户组
int cusdk__UserGroupAddDeviceGroup( cusdk__UserGroupAddDeviceGroupReq req, cusdk__UserGroupAddDeviceGroupRsp& rsp );

class cusdk__UserGroupDelDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//用户组id
	xsd__string devgrpid;		//设备组id	
};

class cusdk__UserGroupDelDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 取消划归一个用户组的设备组
int cusdk__UserGroupDelDeviceGroup( cusdk__UserGroupDelDeviceGroupReq req, cusdk__UserGroupDelDeviceGroupRsp& rsp );

////////////////////////设置取消用户或用户的设备组自动同步标记///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserSetAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//用户id
	xsd__string devgrpid;		//设备组id
	xsd__boolean bSync;			//设置或取消自动同步标记	
};

class cusdk__UserSetAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 自动同步/取消同步一个用户的设备组
int cusdk__UserSetAutoSyncDeviceGroup( cusdk__UserSetAutoSyncDeviceGroupReq req, cusdk__UserSetAutoSyncDeviceGroupRsp& rsp );


class cusdk__UserGroupSetAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//用户组id
	xsd__string devgrpid;		//设备组id
	xsd__boolean bSync;			//设置或取消自动同步标记	
};

class cusdk__UserGroupSetAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__string	operationId;	//扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况
};
// gsoap cusdk service method-documentation: 自动同步/取消同步一个用户组的设备组
int cusdk__UserGroupSetAutoSyncDeviceGroup( cusdk__UserGroupSetAutoSyncDeviceGroupReq req, cusdk__UserGroupSetAutoSyncDeviceGroupRsp& rsp );
////////////////////////查询用户或用户的设备组自动同步标记///////////////////////////////////


//查询指定操作Id(operationId)的当前操作进度(这个接口因为cui实现的原因,只用于设备划归操作的进度查询!)
class cusdk__QueryOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//操作Id，由平台返回的那个	
};

class cusdk__QueryOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__int progress;		//进度值
	xsd__int total;			//总数

};
// gsoap cusdk service method-documentation: 查询指定操作Id的当前执行进度
int cusdk__QueryOperationProgress( cusdk__QueryOperationProgreeReq req, cusdk__QueryOperationProgreeRsp& rsp );

//////////////////////////////////////////////////////////////////////////////////////////////
class cusdk__UserQueryAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string userId;			//用户id
	xsd__int beginIndex;		// 起始序号.
	xsd__int num;		    	// 本次请求要的数目.
};

class cusdk__UserQueryAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	long totalnum;		//总数
	std::vector< xsd__string > devgroupIds;
};
// gsoap cusdk service method-documentation: 分页获取用户所有已同步设备组列表
int cusdk__UserQueryAutoSyncDeviceGroup( cusdk__UserQueryAutoSyncDeviceGroupReq req, cusdk__UserQueryAutoSyncDeviceGroupRsp& rsp );


class cusdk__UserGroupQueryAutoSyncDeviceGroupReq : public cusdk__BaseReq
{	
	xsd__string usergrpId;		//用户组id
	xsd__int beginIndex;		// 起始序号.
	xsd__int num;		    	// 本次请求要的数目.
};

class cusdk__UserGroupQueryAutoSyncDeviceGroupRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__int totalnum;		//总数
	std::vector< xsd__string > devgroupIds;
};
// gsoap cusdk service method-documentation: 分页获取用户组所有已同步设备组列表
int cusdk__UserGroupQueryAutoSyncDeviceGroup( cusdk__UserGroupQueryAutoSyncDeviceGroupReq req, cusdk__UserGroupQueryAutoSyncDeviceGroupRsp& rsp );



class cusdk__GetGBDomainListReq : public cusdk__BaseReq
{
};

class cusdk__GBDomainInfo
{
	xsd__string gbDomainId;          //国标域ID
	xsd__string domainName;	         // 域名

//	xsd__string userName;            //推送的用户名（推送哪个用户的设备）

	xsd__string devTreeViewId;		// 推送的设备视图Id
	xsd__string devTreeVersion;		// 推送的设备视图版本号.
};

class cusdk__GetGBDomainListRsp
{
	xsd__string session;
	std::vector< cusdk__GBDomainInfo > gbDomainList;
	long errorCode;
};
// gsoap cusdk service method-documentation: 获取上级国标域列表，附带有往上级推送的状态
int cusdk__GetSuperGBDomainList(cusdk__GetGBDomainListReq req, cusdk__GetGBDomainListRsp& rsp);



class cusdk__DevicePushReq : public cusdk__BaseReq
{
	xsd__string userName;      //要推送哪个用户的设备
	xsd__string gbDomainId;    //要推送到的国标域
};

class cusdk__DevicePushRsp
{
	xsd__string session;
	long errorCode;
};
// gsoap cusdk service method-documentation: 推送设备到上级国标域
int cusdk__PushDevice(cusdk__DevicePushReq req, cusdk__DevicePushRsp& rsp);

enum cusdk__EDataType
{
	cusdk__DataTypeDevice,	// 设备数据.
	cusdk__DataTypeEMap,	// 电子地图,图元信息.
	cusdk__DataTypeAlarmLink,	// 告警联动.

	cusdk__DataTypeGBSCatalog,		// 导出GBS中的设备信息/状态. 无参数.
	cusdk__DataTypeGBSDevicePush,	// 导出/导入 GBS对上级域的推送设备. 参数1: 上级域的国标ID. 

	cusdk__DataDeviceStatistics,	// 导出设备统计信息csv. 参数1: 设备查询条件.
};

class cusdk__GetDataVersionReq : public cusdk__BaseReq
{
	xsd__int dataType;	// 数据类型. 类型见 cusdk__EDataType
};

class cusdk__GetDataVersionRsp
{
	xsd__string session;
	long errorCode;

	xsd__string dataVersion;
};
// gsoap cusdk service method-documentation: 获取平台的数据版本.
int cusdk__GetDataVersion(cusdk__GetDataVersionReq req, cusdk__GetDataVersionRsp& rsp);


// 通用的文件导出接口.
class cusdk__StartDataFileExportReq : public cusdk__BaseReq
{	
	int exportedDataType;	// 要传输的数据类型. 具体定义参见 cusdk__EDataType

	std::vector< xsd__string > params;	// 参数, 每种导出不同, 具体参见 cusdk__EDataType 的描述..
};

class cusdk__StartDataFileExportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string fileUrl;	// 导出/导入操作的Url.

	xsd__string operationId;	// 异步查询进度用的操作ID. 用于 cusdk__QueryOperationProgress 接口查询进度.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导出数据,异步,需要调用 cusdk__QueryOperationProgress 获取进度.
int cusdk__StartDataFileExport( cusdk__StartDataFileExportReq req, cusdk__StartDataFileExportRsp& rsp );

class cusdk__EndDataFileExportReq : public cusdk__BaseReq
{	
	xsd__string operationId;	// 返回的操作ID.
	std::vector< xsd__string > params;	// 参数, 每种导出不同, 具体参见 cusdk__EDataType 的描述..
};

class cusdk__EndDataFileExportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消数据导出.
int cusdk__EndDataFileExport( cusdk__EndDataFileExportReq req, cusdk__EndDataFileExportRsp& rsp );


// 通用的文件导入接口.
class cusdk__StartDataFileImportReq : public cusdk__BaseReq
{	
	int importedDataType;	// 要传输的数据类型. 具体定义参见 cusdk__EDataType

	std::vector< xsd__string > params;	// 参数, 每种数据不同, 具体参见 cusdk__EDataType 的描述..
};

class cusdk__StartDataFileImportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string fileUrl;	// 导出/导入操作的Url.

	xsd__string operationId;	// 异步查询进度用的操作ID. 用于后续操作, cusdk__QueryOperationProgress 接口查询进度.

	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 开始导入 数据.
int cusdk__StartDataFileImport( cusdk__StartDataFileImportReq req, cusdk__StartDataFileImportRsp& rsp );

class cusdk__EndDataFileImportReq : public cusdk__BaseReq
{	
	xsd__string operationId;	// 返回的操作ID.

	xsd__boolean cancelOperation;	// 是否取消操作.

	std::vector< xsd__string > params;	// 参数, 每种数据不同, 具体参见 cusdk__EDataType 的描述..
};

class cusdk__EndDataFileImportRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
//gsoap cusdk service method-documentation: 已经完成或取消数据导入.
int cusdk__EndDataFileImport( cusdk__EndDataFileImportReq req, cusdk__EndDataFileImportRsp& rsp );

//查询指定操作Id(operationId)的当前操作进度
class cusdk__GetOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//操作Id，由平台返回的那个	
};

class cusdk__GetOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
	xsd__int progress;		//进度值
	xsd__int total;		//总数

};
// gsoap cusdk service method-documentation: 查询指定操作Id的当前执行进度
int cusdk__GetOperationProgress( cusdk__GetOperationProgreeReq req, cusdk__GetOperationProgreeRsp& rsp );


// 子域信息
class cusdk__ChildrenGBDomainInfo
{
	xsd__string gbDomainId;          //国标域ID
	xsd__string domainName;	         // 域名
    xsd__int    status;              // 域状态cusdk__EDomainStatus
};

// 获取所有的子域信息
class cusdk__GetChildrenGBDomainListReq : public cusdk__BaseReq
{
};

class cusdk__GetChildrenGBDomainListRsp
{
 std::vector< cusdk__ChildrenGBDomainInfo > domainIdList;	// 所有子域列表.
 xsd__string session;
 long errorCode;
};

// 获取当前设置的域列表.
class cusdk__GetGBDevicePushDomainReq : public cusdk__BaseReq
{
 xsd__string superDomainId;	// 获取的上级国标域ID
};

class cusdk__GetGBDevicePushDomainRsp
{
 std::vector< cusdk__ChildrenGBDomainInfo > domainIdList;	// 设置的子域列表.
 xsd__string session;
 long errorCode;
};


// 设置当前的域列表.
class cusdk__SetGBDevicePushDomainReq : public cusdk__BaseReq
{
 std::vector< xsd__string > domainIdList;	// 设置的子域列表.
 xsd__string superDomainId;	// 设置的上级国标域ID
};

class cusdk__SetGBDevicePushDomainRsp
{
 xsd__string session;
 long errorCode;
};



// 获取所有域列表
int cusdk__GetChildrenGBDomainList( cusdk__GetChildrenGBDomainListReq req, cusdk__GetChildrenGBDomainListRsp& rsp);

// 获取当前推送的域列表
int cusdk__GetGBDevciePushDomain(cusdk__GetGBDevicePushDomainReq req, cusdk__GetGBDevicePushDomainRsp& rsp);

// 设置当前推送的域列表.

// 设置指定操作Id(operationId)的当前操作进度
class cusdk__UpdateOperationProgreeReq : public cusdk__BaseReq
{	
	xsd__string operationId;	//操作Id，由平台返回的那个	

	xsd__int progress;		//进度值
	xsd__int total;		//总数
};

class cusdk__UpdateOperationProgreeRsp /* : public cusdk__BaseRsp */
{		
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;
};
// gsoap cusdk service method-documentation: 更新指定操作Id的当前执行进度
int cusdk__UpdateOperationProgress( cusdk__UpdateOperationProgreeReq req, cusdk__UpdateOperationProgreeRsp& rsp );

 
 class cusdk__ResInfoReq
 {
 	//播放类型，0osp，1rtsp，2rtmp
 	xsd__int     PlayMode;
 	//请求服务端IP，如果为nat外给nat外url，nat内给nat内url，不填则给所有网段，nat内外url
 	xsd__int     ServerIp;
 	//无用
 	xsd__int     ServerPort;
 	//平台域ID，20位国标ID形式或者KDMNO
 	xsd__string  DomainId;
 	//设备通道ID，20位国标ID形式或者KDMNO
 	xsd__string  DeviceId;
 	//兼容KDMNO使用，如果使用国标ID则无用,使用KDMNO为通道号
 	xsd__int     ChannelId;
 	//无用
 	xsd__int     ScreenWidth;
 	//无用
 	xsd__int     ScreenHeight;
 	//无用
 	xsd__int     Quality;
 	//无用
 	xsd__string  Url;
 	//上下文，相同的上下文会选择同一台从机去浏览，主要用于osp浏览
 	xsd__string  Context;
 	//录像开始时间，不填认为是直播
 	xsd__int	 StartTime;
 	//录像结束时间，不填认为是直播
 	xsd__int	 EndTime;
 	//录像类型，1为平台录像，2为前端录像
 	xsd__int	 VodType;
 };
 
 class cusdk__ResInfo
 {
 	xsd__string  Url;
 	//分辨率
 	xsd__int     Width;
 	xsd__int     Height;
 	xsd__int     FrameRate;
 	//OriginalStream为原始码流，TransCodeHidefition为转码清晰，TransCodeFluent为转码流畅
 	xsd__string  Desc;	
 	//厂商名，请根据是否有转码能力选择URL	
 	xsd__string  ManuFactory;
 	//osp模式下为tcpsendport，其他模式暂时无用
 	xsd__string  Reserve;
 	//视频格式
 	xsd__string  VideoType;
 	//视频采样率
 	xsd__int	 VideoSmpRate;
 	//音频格式
 	xsd__string  AudioType;
 	//音频采样率
 	xsd__int	 AudioSmpRate;
 	//音频通道数
 	xsd__int	 AudioChnlNum;
 };
 
 class cusdk__ResInfoRsp
 {
 	std::vector<cusdk__ResInfo> ResList;	
 };
 int cusdk__GetResInfo(cusdk__ResInfoReq cResInfoReq, cusdk__ResInfoRsp &cResInfoRsp);

 // 设备搜索/统计 搜索条件关键字支持如下, 所有条件之间都是 AND 关系.
 // userid		指定要搜索的用户id, 未指定时搜索当前登录用户.
 // key			搜索关键字. 模糊匹配. 拼音首字母?
 // state=online/offline	设备状态, 在线/不在线, idle状态总是忽略掉的.
 // tree		指定要搜索的树id(视图), 如果未指定,搜索系统默认设备树.
 // rootgroup	指定根设备组, 未指定时为树的根组.
 // devid		指定要搜索的设备Id.
 // filterEmptyGroup	指定设备组中是否有设备,默认只返回有设备的设备组.
 // type=group/device/videosrc		指定要搜索的类型是设备组, 设备,视频源. 可以多选, 多个条件用 / 分隔.
 // location=yes/no 指定视频源是否有经纬度坐标, yes有, no没有. 不填时忽略此条件.
 // devOnTree=id	设备同时在另外一棵树(自定义视图)上, 为空时忽略此参数. 
 // devOnTree!=id	设备不在另外一棵树(自定义视图)上, 为空时忽略此参数.	
 // parentgroup		父组,不递归.
 // statVS=yes/no		是否统计设备总数和在线设备数目. 默认不统计. 启用统计功能, 则 GetSearchResultByGroup 可以获取到设备总数和在线数.
 class cusdk__SearchDeviceReq : public cusdk__BaseReq
 {
	 xsd__string keywords;	// 搜索关键字. 
	 xsd__int maxResultItemNum;	// 最大的结果集大小,超过的忽略. -1 表示无限制.
	 xsd__int maxSearchTimeMSec;// 搜索最大耗时,超过这个时间后停止搜索.
 };

 enum cusdk__SearchResultType
 {
	 cusdk__SearchResultInvalid,		// 无意义.
	 cusdk__SearchResultDeviceGroup,	// 分组.
	 cusdk__SearchResultDevice,			// 设备.
	 cusdk__SearchResultVideosrc,		// Videosrc.
 };

 class cusdk__SearchDeviceRsp
 {
	 xsd__string operationId;	// 操作Id, 用于获取进度. cusdk__GetOperationProgress

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;		// 错误码.
 };

 // 设备搜索/统计.
 int cusdk__SearchDevice( cusdk__SearchDeviceReq req, cusdk__SearchDeviceRsp& rsp );

 // 获取搜索结果.
 class cusdk__GetSearchResultReq : public cusdk__BaseReq
 {
	 xsd__string operationId;

	 xsd__int beginIndex;		// 起始序号.
	 xsd__int num;		    	// 本次请求要的数目.
 };

 class cusdk__DeviceGroupPathItem
 {
	 xsd__string groupId;
	 xsd__string groupName;
 };

 class cusdk__SearchResultItem
 {
	 xsd__int type;		// 类型, 定义参见cusdk__SearchResultType, 设备组/设备/视频源
	 xsd__string id;	// Id
	 xsd__int vsId;		// 视频源id. 结果为设备组/设备时无意义.
	 xsd__string name;	// 名称

	 std::vector<cusdk__DeviceGroupPathItem> fullDeviceGroupPath;	// 结果节点之上的所有设备组节点Id列表. 

	 xsd__boolean online;	// 是否在线.
	 std::vector< int > deviceCapLabelList;	// 设备的能力标签列表. (球机/枪机) cusdk__EDeviceCapLabel

	 xsd__string domainId;	// 设备所属域Id.
	 xsd__string manufacturer;						// 制造商.
 };

 class cusdk__GetSearchResultRsp
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;		// 错误码.

	 xsd__boolean lastPage;	// 是否是最后一页了.

	 xsd__int totalNum;		// 结果总数.
	 std::vector< cusdk__SearchResultItem > result;	// 结果列表.
 };

 int cusdk__GetSearchResult( cusdk__GetSearchResultReq req, cusdk__GetSearchResultRsp& rsp );

 class cusdk__GetSearchResultByGroupReq : public cusdk__BaseReq
 {
	 xsd__string operationId;

	 xsd__int beginIndex;		// 起始序号.
	 xsd__int num;		    	// 本次请求要的数目.

	 xsd__string deviceGroupId;
	 xsd__int type;		// 类型, 定义参见cusdk__SearchResultType, 设备组/设备/视频源
 };

 class cusdk__GetSearchResultByGroupRsp
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;		// 错误码.

	 xsd__boolean lastPage;	// 是否是最后一页了.
	 std::vector< cusdk__SearchResultItem > result;	// 结果列表.

	 xsd__int allVideoSrcNum;		// 结果总数.  statVS=yes 时启用此字段, 否则返回0.
	 xsd__int onlineVideoSrcNum;	// 在线视频源数目. statVS=yes 时启用此字段, 否则返回0.
 };

 int cusdk__GetSearchResultByGroup(cusdk__GetSearchResultByGroupReq req, cusdk__GetSearchResultByGroupRsp& rsp);
 

 // 结束操作.
 class cusdk__EndOperationReq : public cusdk__BaseReq
 {
	 xsd__string operationId;	// 返回的操作ID.

	 std::vector< xsd__string > params;	// 参数, 
 };


 class cusdk__EndOperationRsp /* : public cusdk__BaseRsp */
 {	
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 结束一个操作.
 int cusdk__EndOperation( cusdk__EndOperationReq req, cusdk__EndOperationRsp& rsp );


 // 通过ID获取一个设备组的信息.
 class cusdk__GetDeviceGroupReq : public cusdk__BaseReq
 {
	 xsd__string id;	// 设备组id.
 };

 class cusdk__GetDeviceGroupRsp
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;		// 错误码.

	 cusdk__DeviceGroup group;

	 // std::vector<xsd__string> fullDeviceGroupPath;	// 搜索节点之上的所有设备组节点Id列表. 
 };

 int cusdk__GetDeviceGroup( cusdk__GetDeviceGroupReq req, cusdk__GetDeviceGroupRsp& rsp );

 // 通过ID获取一个设备的信息.
 class cusdk__GetDeviceReq : public cusdk__BaseReq
 {
	 xsd__string id;		// 设备Id.
 };

 class cusdk__GetDeviceRsp 
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;		// 错误码.

	 cusdk__DeviceInfo devInfo;		// 设备信息.
	 //cusdk__DeviceStatus devStatus;	// 设备状态

	// std::vector<cusdk__DeviceGroupPathItem> fullDeviceGroupPath;	// 结果节点之上的所有设备组节点Id列表. 只返回第一个分组.

	 cusdk__DevOnlineNotify online;
	 cusdk__DevConfigNotify config;
	 cusdk__DevServiceStatusNotify service;
	 std::vector< cusdk__DevAlarmNotify > alarm;        // 设备告警通知.

 };

 int cusdk__GetDevice( cusdk__GetDeviceReq req, cusdk__GetDeviceRsp& rsp );

 class cusdk__GetDeviceGroupPathReq : public cusdk__BaseReq
 {
     xsd__string devTreeId; //设备树根分组Id，因同一视频源可能被添加至不同的视图
     xsd__string devId;   //设备Id
	 xsd__int  videoSrcId; //视频源Id
 };

 class cusdk__GetDeviceGroupPathRsp
 {
	 std::vector<cusdk__DeviceGroupInfo> devGroupPath;  //分组路径，依次由上层分组到下层分组排列
	 xsd__string session;
	 long errorCode;
 };

 //根据视频源信息，获取其所属分组路径
 int cusdk__GetDeviceGroupPath(cusdk__GetDeviceGroupPathReq req, cusdk__GetDeviceGroupPathRsp& rsp);


 class cusdk__CreateCustomTreeViewReq : public cusdk__BaseReq
 {
	 xsd__string name; //自定义视图名称

	 bool cloneFromAnotherTreeView;		// 是否从另外一棵树clone.
	 xsd__string cloneSrcTreeViewId;	// 从另外一棵树上clone, 源树id, 为空为系统设备树.

	 bool canUsedForGBPush;			// 是否支持作为国标选择性推送的推送设备树
 };

 class cusdk__CreateCustomTreeViewRsp
 {
	 xsd__string id; //标识新创建的视图id

	 xsd__string operationId;	// 操作ID, 用于查询进度. (cusdk__QueryOperationProgress).

	 xsd__string session;
	 long errorCode;
 };
 
 //创建自定义设备树视图
 int cusdk__CreateCustomTreeView(cusdk__CreateCustomTreeViewReq req, cusdk__CreateCustomTreeViewRsp& rsp);

 class cusdk__ModifyCustomTreeViewNameReq : public cusdk__BaseReq
 {
	 xsd__string id; //视图id，标识要修改的视图
	 xsd__string name; //新的视图名称
 };


 class cusdk__ModifyCustomTreeViewNameRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //修改自定义设备树视图名称
 int cusdk__ModifyCustomTreeViewName(cusdk__ModifyCustomTreeViewNameReq req, cusdk__ModifyCustomTreeViewNameRsp& rsp);


 class cusdk__DeleteCustomTreeViewReq : public cusdk__BaseReq
 {
	 xsd__string id; //视图id
 };

 class cusdk__DeleteCustomTreeViewRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //删除自定义设备树视图
 int cusdk__DeleteCustomTreeView(cusdk__DeleteCustomTreeViewReq req, cusdk__DeleteCustomTreeViewRsp& rsp);


 enum cusdk__TreeViewType
 {
	 cusdk__TreeViewType_System = 0, //系统级的视图，多用户共享
	 cusdk__TreeViewType_User, //用户级的视图，用户独占
 };

 //自定义设备树视图信息
 class cusdk__CustomTreeViewInfo
 {
	 xsd__string id; //视图id
	 xsd__string name; //视图名称
	 xsd__string dataVersion; //数据版本[只读]

	 bool canUsedForGBPush;	// 是否支持作为国标选择性推送的推送设备树
	 int  treeViewType; //定义见cusdk__TreeViewType
 };

 class cusdk__GetCustomTreeViewListReq : public cusdk__BaseReq
 {
	  bool includeFavorite; // 请求结果中是否要包含用户私有的设备树（即收藏夹）
 };

 class cusdk__GetCustomTreeViewListRsp
 {
	 std::vector<cusdk__CustomTreeViewInfo> treeViewInfoList;
	 xsd__string session;
	 long errorCode;
 };

 //获取自定义设备树视图列表
 int cusdk__GetCustomTreeViewList(cusdk__GetCustomTreeViewListReq req, cusdk__GetCustomTreeViewListRsp& rsp);

 // 递归查询分组下的设备.
 class cusdk__GetUserDeviceInGroupReq : public cusdk__BaseReq
 {
	 xsd__string userId;	// 用户ID, 为空时为当前操作者用户.
	 xsd__string treeId;	// 树ID, 为空则为系统默认设备树.
	 xsd__string groupId;	// 设备组Id.
	 xsd__boolean recursive;// 是否对下级分组做递归操作.
	 xsd__string queryTag;	// 查询Id, 用于分页查询.

	 xsd__int num;			// 期望查询的数量.
 };

 class cusdk__SimpleUserDevice
 {
	 xsd__string devId;
	 std::vector<int> videoSrcIdList;
 };

 class cusdk__GetUserDeviceInGroupRsp
 {
	 xsd__string queryTag;	// 查询Id, 用于分页查询.
	 xsd__boolean lastPage;	// 是否最后一页.

	 std::vector< cusdk__SimpleUserDevice > devList;

	 xsd__string session;
	 long errorCode;
 };

 int cusdk__GetUserDeviceInGroup(cusdk__GetUserDeviceInGroupReq req, cusdk__GetUserDeviceInGroupRsp& rsp);

 // 批量操作接口
 enum cusdk__EOperationType
 {
	 cusdk__OpStartPlatRecord,	// 开始平台录像
	 cusdk__OpStopPlatRecord,	// 停止平台录像
	 cusdk__OpStartPuRecord,	// 开始前端录像
	 cusdk__OpStopPuRecrod,		// 停止前端录像
	 cusdk__OpStartWiper,		// 开始雨刷
	 cusdk__OpStopWiper,		// 停止雨刷
	 cusdk__OpStopPuCall,		// 停止语音呼叫, 参数1:用户呼叫的PlayId
	 cusdk__OpLoadPreset,		// 前端载入预置位 参数1: 预置位ID.

	 cusdk__OpLockRecord,		// 锁定平台录像. 参数1: 录像起始时间. 参数2: 录像结束时间. 时间格式都为标准时间字符串. +by lzx@2018-03-06 V2R4B1
 };
 class cusdk__BatchOperationReq : public cusdk__BaseReq
 {
	 xsd__string treeId;	// 树ID, 为空则为系统默认设备树.
	 xsd__string groupId;	// 组ID.
	 xsd__boolean recursive;// 是否对下级分组做递归操作.
	 xsd__int operationType;// 操作类型, 定义参见 cusdk__EOperationType.

	 std::vector< xsd__string > paramList;	// 参数列表, 根据操作类型不同,需要不同的参数.
 };

 class cusdk__BatchOperationRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 int cusdk__BatchOperation(cusdk__BatchOperationReq req, cusdk__BatchOperationRsp& rsp);

// class cusdk__GetSearchDvcReq
// {
// 	xsd__string			m_SearchDevName;
// 	xsd__string			m_FeatureCode;//
// 	xsd__int			m_ReceivedNum;		//?channel count
// 	xsd__int 	       m_SerchResLimit;
// 	xsd__int 				m_dwserver;
// };
// 
// class cusdk_SearchVidSrc
// {
// 	xsd__int			m_nSn;				// Video Source id(No.)
// 	xsd__string			m_szSrcName;		// video source alias
// 	xsd__int			m_nPtzLevel;		// video source ptz level
// 	xsd__int			m_IsSearchShow;		// 0 no show 1 show
// };
// 
// class cusdk__SearchDvcInfo
// {
// 	//devices
// 	xsd__int			m_nSn;
// 	xsd__string			m_szdeviceID;
// 	xsd__string			m_szdomainID;
// 	xsd__int			m_nDevSrcNum;
// 	xsd__int			m_nEncoderChnNum;
// 	xsd__string			m_szDevSrcAlias;
// 	xsd__int			m_nDevType;
// 	xsd__int			m_nDevCap;
// 	xsd__int			m_nCallType;
// 	xsd__string			m_szManufacturer;
// 	cusdk_SearchVidSrc	m_aDevSrcChn[64];	// device video source infomation
// 
// 	//group
// 	xsd__string			m_szSelfGroupId;
// 	xsd__string			m_szGroupName;
// 	xsd__int			m_bHashDevice;
// 
// 	//
// 	xsd__string			m_szParentId;	
// 	xsd__int			m_SearDvcType;		//1:group;2:devices;
// 
// };
// 
// class cusdk__GetSearchDvcRsp
// {
// 	xsd__int			m_totalChannelNum;			//?channel count
// 	xsd__int			m_hasReceiveChannelNum;		//?channel count
// 	xsd__int			m_CurrentDvcNum;			//?Dvc count
// 	std::vector<cusdk__SearchDvcInfo> SearchDvcResult;
// };
// int cusdk__GetSearchDvcRes(cusdk__GetSearchDvcReq cSearchDvcReq, cusdk__GetSearchDvcRsp &cSearchDvcRsp);
// 
// 

class cusdk__GroupVSNum
{
	xsd__string groupId;			//设备组ID
    xsd__int num;                //视频源总数
};

 class cusdk__GetDeviceGroupVideoSrcNumReq : public cusdk__BaseReq
 {
	 xsd__string treeId;						// 树ID, 为空则为系统默认设备树.（由于目前这个接口只在离线统计用，这个值，只会填空）
	 std::vector< xsd__string > groupIdList;	// 需要查询视频源总数的设备组ID列表
 };

 class cusdk__GetDeviceGroupVideoSrcNumRsp
 {
	 std::vector< cusdk__GroupVSNum > grpvsNums;	//设备组的视频源总数
	 xsd__string session;
	 long errorCode;
 };
//批量查询指定设备组下的所有视频源总数
 int cusdk__GetDeviceGroupVideoSrcNum(cusdk__GetDeviceGroupVideoSrcNumReq req, cusdk__GetDeviceGroupVideoSrcNumRsp& rsp);

 
 class cusdk__VideoSrcId
 {
	 xsd__string deviceId;
	 xsd__int vsId;
 };
 
 class cusdk__VSOfflineInfo
 {
	cusdk__VideoSrcId videosrcId;	//视频源ID
	xsd__string groupId;		//所在的父组组ID，设备树仅使用系统设备树
	xsd__string devName;	//所在设备的设备别名
	xsd__string name;		//别名
	xsd__string deviceIP;	//ip地址
	double longitude;		// 经度
	double latitude;		// 纬度	
 };
 
 class cusdk__GetVSOfflineInfoReq : public cusdk__BaseReq
 {	 
 //设备树仅使用系统设备树
	 std::vector< cusdk__VideoSrcId > videosrclist;	// 需要查询视频源总数的设备组ID列表
 };

 class cusdk__GetVSOfflineInfoRsp
 {
	 std::vector< cusdk__VSOfflineInfo > vsOfflineInfo;	//离线视频源信息
	 xsd__string session;
	 long errorCode;
 };
//批量查询离线视频源的相关信息
 int cusdk__GetVSOfflineInfo(cusdk__GetVSOfflineInfoReq req, cusdk__GetVSOfflineInfoRsp& rsp);

// 2016-07-18 临时版本功能, 开发者: 汪辉,王斌. 
// int cusdk__GetChildrenGBDomainList(cusdk__GetGBDomainListReq req, cusdk__GetGBDomainListRsp& rsp);

//  class cusdk__GetGBDevicePushDomainReq : public cusdk__BaseReq
//  {
//  };
// 
//  class cusdk__GetGBDevicePushDomainRsp
//  {
// 	 std::vector< xsd__string > domainIdList;	// 选择的域列表.
// 
// 	 xsd__string session;
// 	 long errorCode;
//  };
// 
//  // 获取当前推送的域列表.
//  int cusdk__GetGBDevciePushDomain(cusdk__GetGBDevicePushDomainReq req, cusdk__GetGBDevicePushDomainRsp& rsp);
// 
//  class cusdk__SetGBDevicePushDomainReq : public cusdk__BaseReq
//  {
// 	 std::vector< xsd__string > domainIdList;	// 选择的域列表.
//  };
// 
//  class cusdk__SetGBDevicePushDomainRsp
//  {
// 	 xsd__string session;
// 	 long errorCode;
//  };
// 
//  // 获取当前推送的域列表.
//  int cusdk__SetGBDevciePushDomain(cusdk__SetGBDevicePushDomainReq req, cusdk__SetGBDevicePushDomainRsp& rsp);
// 

 class cusdk__GetDevAzimuthalAngleReq : public cusdk__BaseReq
 {
	 xsd__string deviceId;	// 设备Id
	 int videosrcId;		// 视频源Id
 };

 class cusdk__GetDevAzimuthalAngleRsp
 {
	 xsd__string azimuthalAngleData;

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 获取视频源的方位角
 int cusdk__GetDevAzimuthalAngle(cusdk__GetDevAzimuthalAngleReq req, cusdk__GetDevAzimuthalAngleRsp& rsp);

 class cusdk__DeleteRecordReq : public cusdk__BaseReq
 {
	 //xsd__string recordId;	// 录像Id.
	 xsd__string deviceId;	// 设备ID.
	 int		 chnId;		// 设备通道ID.
	 cusdk__TimeRange timeRange;	// 时间段.
	 //xsd__string domainName;	// 录像所在域Id. ?
 };

 class cusdk__DeleteRecordRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 删除录像文件
 int cusdk__DeleteRecord(cusdk__DeleteRecordReq req, cusdk__DeleteRecordRsp& rsp);



 // 自动匹配GBID
 class cusdk__AutoMatchGBIdReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// 设备视图Id
	 xsd__string devGroupId;	// 设备组Id, 递归自动匹配设备组下面设备的GBId.
	 //xsd__string devGrpgbid;    // 分组gbid

	 xsd__string deviceId;		// 设备Id,可以为空,为空时自动匹配分组下所有设备的国标Id.
	 int devVidIndex;
	 int devMatchType;
	 bool matchLastBits;        // 是否匹配后 5 位
	 //xsd__string devGbid;
 };

 class cusdk__AutoMatchGBIdRsp
 {
	 xsd__string gbid;
	 xsd__string session;
	 long errorCode;
	 xsd__string operationId; //扩展字段由客户端生成的一个唯一Id，后续可以通过这个Id查询本次请求的处理情况，对应进度查询接口：QueryOperationProgress
 };

 //gsoap cusdk service method-documentation: 自动匹配GBID
 int cusdk__AutoMatchGBId( cusdk__AutoMatchGBIdReq req, cusdk__AutoMatchGBIdRsp& rsp );

 // 查询分组下的推送通道数目.
 class cusdk__GetGroupDevSumNumReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// 设备树视图ID.
	 xsd__string devGroupId;	// 设备组ID.
 };

 class cusdk__GetGroupDevSumNumRsp
 {
	 int chnNum;			// 设备组下推送的通道总数.

	xsd__string session;
	long errorCode;
 };

 //gsoap cusdk service method-documentation: 统计推送的通道总数
 int cusdk__GetGroupDevSumNum( cusdk__GetGroupDevSumNumReq req, cusdk__GetGroupDevSumNumRsp& rsp );

 enum cusdk__EDevicePushChnSelectType
 {
	 cusdk__ModifyDeviceGroupGBID = 0,
	 cusdk__ModifyDeviceGroupPrimary = 1,
	 cusdk__ModifyDeviceGroupSecond = 2,
	 cusdk__ModifyDeviceGroupBoth = 3,
 };

 // CDeviceGroupModReq 修改 priSec 部分
 class cusdk__ModifyGBDeviceGroupReq : public cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// 设备树视图ID.
	 xsd__string devGroupId;	// 设备组ID.

	 xsd__string devGroupGBID; // 修改分组 gbid
	 int devicePushChnSelectType;	// 主辅流选择类型, 定义见 cusdk__EDevicePushChnSelectType. 
 };

 class cusdk__ModifyGBDeviceGroupRsp
 {
	 xsd__string operationId;	// 操作ID, 用于查询进度. QueryOperationProgress.
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 一个设备组的推送配置(只推送主流, 只推送辅流, 主辅流都推送)
 int cusdk__ModifyGBDeviceGroup( cusdk__ModifyGBDeviceGroupReq req, cusdk__ModifyGBDeviceGroupRsp& rsp );

 enum cusdk__EDevicePushChnModifyType
 {
	 cusdk__DevicePushChnModifyTypeNil = 0,
	 cusdk__DevicePushChnModifyTypeAddprimary,
	 cusdk__DevicePushChnModifyTypeAddsecond,
	 cusdk__DevicePushChnModifyTypeDelprimary,
	 cusdk__DevicePushChnModifyTypeDelsecond,
	 cusdk__DevicePushChnModifyTypeModifyprimary,
	 cusdk__DevicePushChnModifyTypeModifysecond
 };
 // 修改通道GBID ( CDeviceGroupDeviceMod4GbReq )
 class cusdk__ModifyGBDeviceReq : cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// 设备树视图ID.
	 xsd__string devGroupId;	// 设备组ID.

	 xsd__string deviceId;		// 设备ID, 
	 int videoSrcId;			// 视频源ID

	 int operationType;			// cusdk__EDevicePushChnModifyType.
	 xsd__string gbID;			// 要修改的国标ID.
 };

 class cusdk__ModifyGBDeviceRsp
 {
	 xsd__string devtreeId;               
	 xsd__string devgroupId;               
	 xsd__string deviceId;               
	 int vidIndex; 

	 // CDeviceGroupDeviceMod4GbNtfReq 中 gbid
	 xsd__string primaryOriginalGBId;	// 通道0(主流)原GBId.
	 xsd__string secondOriginalGBId;	// 通道1(辅流)原GBId.
	 xsd__string primaryNewGBId;		// 通道0(主流)新GBId.
	 xsd__string secondNewGBId;		// 通道1(辅流)新GBId.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 修改通道GBID
 int cusdk__ModifyGBDevice(cusdk__ModifyGBDeviceReq req, cusdk__ModifyGBDeviceRsp& rsp);


 // 推送一棵树的新选择性推送.
 class cusdk__StartDevicePushReq : public cusdk__BaseReq
 {
	 xsd__string parentDomainId;	// 要推送设备的域ID
	 xsd__string devTreeViewId;		// 要推送的设备视图ID. 当为空时,为取消选择性推送,使用全推送(推送系统默认树).
 };

 class cusdk__StartDevicePushRsp
 {
	 xsd__string session;
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 推送一棵树的新选择性推送
 int cusdk__StartDevicePush( cusdk__StartDevicePushReq req, cusdk__StartDevicePushRsp& rsp );


 // 国标选择性推送设备信息. 
 class cusdk__CVideoSrcInfoGB		// (TDevGrpFullGbInfos)
 {
	 int videoSrcId;				// 视频源Id
	 xsd__string deviceId;			// 所属设备Id
	 xsd__string deviceName;		// 所属设备名
	 xsd__string videoSrcName;		// 视频源名

	 xsd__string primaryOriginalGBId;	// 通道0(主流)原GBId.
	 xsd__string secondOriginalGBId;	// 通道1(辅流)原GBId.
	 xsd__string primaryNewGBId;		// 通道0(主流)新GBId.
	 xsd__string secondNewGBId;		// 通道1(辅流)新GBId.

 };

 class cusdk__CVideoSrcInfoGBInGroup : public cusdk__CVideoSrcInfoGB
 {
	 std::vector< cusdk__DeviceGroupInfo > deviceGroupPath;	// 设备组路径.
 };

 class cusdk__CDeviceInfoGB			// (CDeviceGroupDeviceInfo4Gb)
 {
	 xsd__string deviceTreeViewId;	// 设备树Id
	 xsd__string deviceGroupId;		// 设备组Id
	 xsd__string deviceName;		// 设备名
	 xsd__string deviceId;			// 设备Id(KedaID)
	 std::vector< cusdk__CVideoSrcInfoGB > videoSrcList;	// 视频源信息.(TDeviceInfo4Gb)
 };

 // 获取设备列表. CDeviceGroupDeviceQry4GbReq
 class cusdk__GetGBDeviceByGroupReq : cusdk__BaseReq
 {
	 //xsd__string userId;	// 用户ID, 为空时为当前操作者用户.
	 xsd__string treeId;	// 树ID, 为空则为系统默认设备树.
	 xsd__string groupId;	// 设备组Id.

	 xsd__int beginIndex;	// 起始序号.
	 xsd__int num;			// 本次请求要的总数.
 };

 class cusdk__GetGBDeviceByGroupRsp
 {
	 int totalNum;										// 满足条件的设备组的总数.

	 std::vector< cusdk__CDeviceInfoGB > devList;		// 设备列表.

	 xsd__string session;
	 long errorCode;
 };
 int cusdk__GetGBDeviceByGroup(cusdk__GetGBDeviceByGroupReq req, cusdk__GetGBDeviceByGroupRsp& rsp);


 // 按照GBID搜索设备.
 //gsoap cusdk service method-documentation: 按照GBID搜索设备
 class cusdk__SearchGBDeviceReq : cusdk__BaseReq
 {
	 xsd__string deviceTreeViewId;	// 所属设备树ID.
	 xsd__string deviceGBID;		// 要搜索的GBID（只支持精确搜索）.
	 xsd__string deviceName;        // 要搜索的设备名称(支持模糊搜索).
	 xsd__int    beginIndex;        // 起始索引
	 xsd__int    requestNum;        // 请求返回的结果数量
 };

 class cusdk__SearchGBDeviceRsp
 {
	 std::vector< cusdk__CVideoSrcInfoGBInGroup > searchResult;	// 搜索结果.
	 xsd__int totalNum; //结果总数
	 xsd__string session;
	 long errorCode;
 };

 int cusdk__SearchGBDevice(cusdk__SearchGBDeviceReq req, cusdk__SearchGBDeviceRsp& rsp);

class cusdk__SearchGBDeviceGroupInfo
{
	std::vector< cusdk__DeviceGroupInfo > groupInfo;
};
 // 按照Cvilcode搜索设备分组
 //gsoap cusdk service method-documentation: 按照Cvilcode搜索设备分组
 class cusdk__SearchGBDeviceGroupReq : cusdk__BaseReq
 {
	 xsd__string deviceTreeViewId;	// 所属设备树ID.
	 xsd__string civilcode;			// 设备组行政区划.
	 xsd__string groupName;         // 要搜索的设备名称(支持模糊搜索).
	 xsd__int    beginIndex;        // 起始索引
	 xsd__int    requestNum;        // 请求返回的结果数量
 };

 class cusdk__SearchGBDeviceGroupRsp
 {
	 std::vector< cusdk__SearchGBDeviceGroupInfo > searchResult;	// 搜索结果.
	 xsd__int totalNum; //结果总数
	 xsd__string session;
	 long errorCode;
 };
 int cusdk__SearchGBDeviceGroup(cusdk__SearchGBDeviceGroupReq req, cusdk__SearchGBDeviceGroupRsp& rsp);


 // 选择性推送错误类型
 enum cusdk__DevicePushErrorType
 {
	 DevicePushErrorOk,				// 无错误
	 DevicePushErrorPrimaryGBIdRepeat,		// 主流国标ID重复
	 DevicePushErrorSecondGBIdRepeat,		// 辅流国标ID重复
 };
 class cusdk__DevicePushErrorInfo
 {
	 int errorType;					// 错误类型, 定义见 cusdk__DevicePushErrorType

// 	 xsd__string domainId;			// 设备所属域Id
// 	 xsd__string devId;				// 设备Id.
// 	 xsd__string devName;			// 设备名
// 	 xsd__string videoSrcName;		// 视频源名
// 	 int videoSrcId;				// 视频源Id
// 	 xsd__string primaryOriginalGBId;	// 通道0(主流)原GBId.
// 	 xsd__string secondOriginalGBId;	// 通道1(辅流)原GBId.
// 	 xsd__string primaryNewGBId;		// 通道0(主流)新GBId.
// 	 xsd__string secondNewGBId;		// 通道1(辅流)新GBId.

	 cusdk__CVideoSrcInfoGBInGroup videoSrc;	// 有错误的视频源信息.

	 xsd__string deviceGroupId;			// 错误所在的设备组.
	 int indexInDeviceGroup;			// 视频源所在设备组的序号.
	 int errIndex;                         // 要查找的错误编号
 };

 class cusdk__GetNextDeviceGBIDErrorReq : cusdk__BaseReq
 {
	 xsd__string devTreeViewId;	// 设备视图Id
	 xsd__string groupId;		// 设备组Id, 为空时查询整棵树.
	 int errIndex;                   // 上次查找的错误编号
 };

 class cusdk__GetNextDeviceGBIDErrorRsp
 {
	 std::vector< cusdk__DevicePushErrorInfo > nextError;	// 下一个错误, 为空表示没有错误.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 获取选择性推送的错误列表
 int cusdk__GetNextDeviceGBIDError(cusdk__GetNextDeviceGBIDErrorReq req, cusdk__GetNextDeviceGBIDErrorRsp& rsp);

 class cusdk__GetServerConfigReq : cusdk__BaseReq
 {
	 
 };

 class cusdk__GetServerConfigRsp
 {
	 xsd__boolean enableDecoderShowAreaInfo;	// 解码器是否显示设备辖区信息.

	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 获取平台服务端配置信息
 int cusdk__GetServerConfig(cusdk__GetServerConfigReq req, cusdk__GetServerConfigRsp& rsp);


 class cusdk__SetServerConfigReq : cusdk__BaseReq
 {
	 xsd__boolean enableDecoderShowAreaInfo;	// 解码器是否显示设备辖区信息.
 };

 class cusdk__SetServerConfigRsp
 {
	 xsd__string session;
	 long errorCode;
 };

 //gsoap cusdk service method-documentation: 设置平台服务端配置信息
 int cusdk__SetServerConfig(cusdk__SetServerConfigReq req, cusdk__SetServerConfigRsp& rsp);

 class cusdk__TVWallClearAlarmReq : public cusdk__BaseReq
 {	
	 //cusdk__DevChannel decoderChn;	// 解码器 解码通道.
	 xsd__string TVWallId;			// 电视墙Id.
	 int tvId;						// 电视机Id.
	 int tvDivId;					// 电视机分画面Id.

 };

 class cusdk__TVWallClearAlarmRsp /* : public cusdk__BaseRsp */
 {

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: 电视墙消除告警上墙的画面, 恢复到原画面.
 int cusdk__TVWallClearAlarm( cusdk__TVWallClearAlarmReq req, cusdk__TVWallClearAlarmRsp& rsp );

 // 锁定平台录像
 class cusdk__LockRecordReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__RecDuration > recordDurationList;	// 需要锁定的录像。
 };

 class cusdk__LockRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: 锁定录像, 保证录像记录不被覆盖..
 int cusdk__LockRecord( cusdk__LockRecordReq req, cusdk__LockRecordRsp& rsp );

 // 解锁锁定的平台录像
 class cusdk__UnlockRecordReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__RecDuration > recordDurationList;	// 需要解锁的录像。
 };

 class cusdk__UnlockRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: 解锁锁定的录像, 释放录像占用的空间
 int cusdk__UnlockRecord( cusdk__UnlockRecordReq req, cusdk__UnlockRecordRsp& rsp );


 // 字幕获取
 enum cusdk__EOSDState
 {
	 cusdk__OSDStateInvalid,	// 0 无效值
	 cusdk__OSDStateDisable,	// 1 禁用
	 cusdk__OSDStateText,		// 2 文本模式的字幕.
 };

 class cusdk__LongOSDParam
 {
	 int state;                         //	编码字幕状态 cusdk__EOSDState
	 int leftMargin;                    //	编码字幕左边距 1-200;
	 int topMargin;                     //	编码字幕上边距 1-200;
	 int color;                         //	视频源字幕颜色;
	 xsd__string text;                  //	视频源字幕内容, UTF8编码
 };

 class cusdk__GetPULongOSDReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// 设备ID
	 int videoSrcId;		// 视频源ID
 };

 class cusdk__GetPULongOSDRsp /* : public cusdk__BaseRsp */
 {
	 cusdk__LongOSDParam longOSDParam;	//	OSD字幕参数.

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 字幕获取.
 int cusdk__GetPULongOSD( cusdk__GetPULongOSDReq req, cusdk__GetPULongOSDRsp& rsp );

 // 字幕设置
 class cusdk__SetPULongOSDReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// 设备ID
	 int videoSrcId;		// 视频源ID

	 cusdk__LongOSDParam longOSDParam;	//	OSD字幕参数.
 };

 class cusdk__SetPULongOSDRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 字幕设置
 int cusdk__SetPULongOSD( cusdk__SetPULongOSDReq req, cusdk__SetPULongOSDRsp& rsp );


 // 获取设备的证书.
 class cusdk__GetDeviceCertificateReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// 设备ID	
 };

 class cusdk__GetDeviceCertificateRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string signatureCert;	// 签名证书 
	 xsd__string encryptCert;	// 加密证书

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 获取设备的证书.
 int cusdk__GetDeviceCertificate( cusdk__GetDeviceCertificateReq req, cusdk__GetDeviceCertificateRsp& rsp );
 
 // 启用设备码流签名控制
 class cusdk__DeviceSignatrueCtrlReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// 设备ID	
	 xsd__boolean enabled;	// 是否启用签名.
 };
 class cusdk__DeviceSignatrueCtrlRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 启用设备码流签名控制.
 int cusdk__DeviceSignatrueCtrl( cusdk__DeviceSignatrueCtrlReq req, cusdk__DeviceSignatrueCtrlRsp& rsp );

 // 启用设备码流加密控制.
 class cusdk__DeviceEncryptionCtrlReq : public cusdk__BaseReq
 {	
	 xsd__string deviceId;	// 设备ID	
	 xsd__boolean enabled;	// 是否启用码流加密
 };
 class cusdk__DeviceEncryptionCtrlRsp /* : public cusdk__BaseRsp */
 {
	
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 启用设备码流加密控制.
 int cusdk__DeviceEncryptionCtrl( cusdk__DeviceEncryptionCtrlReq req, cusdk__DeviceEncryptionCtrlRsp& rsp );


 // 双向认证第一次握手.
 class cusdk__BidirectionalAuth1Req : public cusdk__BaseReq
 {	
	 xsd__string algorithm;
 };
 class cusdk__BidirectionalAuth1Rsp /* : public cusdk__BaseRsp */
 {
	 xsd__string algorithm;
	 xsd__string random1;

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 双向认证第一次握手.
 int cusdk__BidirectionalAuth1( cusdk__BidirectionalAuth1Req req, cusdk__BidirectionalAuth1Rsp& rsp );


 // 双向认证第二次握手.
 class cusdk__BidirectionalAuth2Req : public cusdk__BaseReq
 {	
	 xsd__string random1;
	 xsd__string random2;
	 xsd__string serverId;
	 xsd__string sign1;

	 xsd__string algorithm;
 };
 class cusdk__BidirectionalAuth2Rsp /* : public cusdk__BaseRsp */
 {
	 xsd__string random1;
	 xsd__string random2;
	 xsd__string cryptkey;
	 xsd__string sign2;

	 xsd__string algorithm;

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 双向认证第二次握手.
 int cusdk__BidirectionalAuth2( cusdk__BidirectionalAuth2Req req, cusdk__BidirectionalAuth2Rsp& rsp );

 //顺序调整操作
 enum cusdk__MoveOperation
 {
	 cusdk__MoveInvalid = 0, // 无效值
	 cusdk__InDevice_MoveAfter, //设备内移动
	 cusdk__InDevice_MoveTop, //设备内置顶
	 cusdk__InDevice_MoveBottom,//设备内置底
	 cusdk__InGroup_MoveAfter, //分组内移动
	 cusdk__InGroup_MoveTop, //分组内置顶
	 cusdk__InGroup_MoveBottom, //分组内置底
 };

 class cusdk_DeviceVideoSrc
 {   
	 xsd__string deviceId; //设备Id
	 int videoSrcId; //视频源Id，-1时表示整个设备
 };

 //调整视频源顺序请求
 class cusdk__AdjustVideoSrcOrderReq : public cusdk__BaseReq
 {
	 xsd__string treeId; //设备树Id
	 xsd__string groupId; //所属分组Id
	 int moveOp; //调整操作 cusdk__MoveOperation
	 cusdk_DeviceVideoSrc vsMove; //需要调整的视频源
	 cusdk_DeviceVideoSrc vsRefer; /*供参考基准的视频源。当操作为cusdk__InDevice_MoveAfter及cusdk__InGroup_MoveAfter时，
	 需要填写此vsRefer字段，表示将vsMoved移动到vsRefer之后。置顶、置底时此字段无效*/
 };

 //调整视频源顺序应答
 class cusdk__AdjustVideoSrcOrderRsp
 {   
	 xsd__string session;
	 long errorCode;
 };

 //调整视频源顺序
 int cusdk__AdjustVideoSrcOrder( cusdk__AdjustVideoSrcOrderReq req, cusdk__AdjustVideoSrcOrderRsp& rsp );

 // 获取告警类型和其定义 
 class cusdk__AlarmType
 {
	 int alarmType;				// 告警类型定义. 参见 cusdk__EAlarmType
	 xsd__string alarmTypeName;	// 告警类型名称.
 };
 class cusdk__GetAlarmTypesReq : public cusdk__BaseReq
 {	
 };

 class cusdk__GetAlarmTypesRsp /* : public cusdk__BaseRsp */
 {
	 std::vector< cusdk__AlarmType > alarmTypes;	// 平台支持的所有告警类型及其名称.

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 字幕设置
 int cusdk__GetAlarmTypes( cusdk__GetAlarmTypesReq req, cusdk__GetAlarmTypesRsp& rsp );

 class cusdk__EliminateAlarmReq : public cusdk__BaseReq
 {	 
	 xsd__string deviceId;	// 设备ID.
	 int vsId;				// 视频源ID, 当vsId为-1时, 表示消除整个设备的告警.
 };

 class cusdk__EliminateAlarmRsp
 {
	 xsd__string session;
	 long errorCode;
 };
 // 消除设备告警
 int cusdk__EliminateAlarm(cusdk__EliminateAlarmReq req, cusdk__EliminateAlarmRsp& rsp);

 // 获取一机一档的服务器地址
 class cusdk__GetDeviceRecordsServerUrlReq : public cusdk__BaseReq
 {	 
	 
 };

 class cusdk__GetDeviceRecordsServerUrlRsp
 {
	 std::vector< xsd__string > deviceRecordsServerUrls;

	 xsd__string session;
	 long errorCode;
 };
 // 获取一机一档的服务器地址
 int cusdk__GetDeviceRecordsServerUrl(cusdk__GetDeviceRecordsServerUrlReq req, cusdk__GetDeviceRecordsServerUrlRsp& rsp);


 // 锁定平台录像  +by lzx@2018-03-06 V2R4B1
 class cusdk__LockRecordBatchReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// 锁定录像的监控点. 其中设备ID不能为空.
	 cusdk__TimeRange timeRange;						// 锁定时间段。
 };

 class cusdk__LockRecordBatchRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 锁定录像, 保证录像记录不被覆盖..  +by lzx@2018-03-06 V2R4B1
 int cusdk__LockRecordBatch( cusdk__LockRecordBatchReq req, cusdk__LockRecordBatchRsp& rsp );

 // 解锁平台录像.  +by lzx@2018-03-06 V2R4B1
 class cusdk__UnlockRecordBatchReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// 锁定录像的监控点. 其中设备ID可以为空, 表示搜索所有锁定录像.

	 bool enableTimeRange;								// 是否使用时间段参数. 不启用的话搜索所有时间段的锁定录像.
	 cusdk__TimeRange timeRange;						// 锁定时间段。
 };

 class cusdk__UnlockRecordBatchRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

 };
 //gsoap cusdk service method-documentation: 解锁锁定的录像,   +by lzx@2018-03-06 V2R4B1
 int cusdk__UnlockRecordBatch( cusdk__UnlockRecordBatchReq req, cusdk__UnlockRecordBatchRsp& rsp );


 // 查询锁定的平台录像.  +by lzx@2018-03-06 V2R4B1
 class cusdk__QueryLockedRecordReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId devVideoSrc;					// 锁定录像的监控点. 其中设备ID可以为空, 表示搜索所有锁定录像.
	 bool enableTimeRange;								// 是否使用时间段参数. 不启用的话搜索所有时间段的锁定录像.
	 cusdk__TimeRange timeRange;						// 锁定时间段。

	 int maxResultNum;									// 返回的最大结果数.
 };

 class cusdk__QueryLockedRecordRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

	 std::vector< cusdk__RecDuration > recordDurationList;	// 结果。
	 bool hasMoreResult;									// 有超出最大结果数的录像记录未返回.
 };
 //gsoap cusdk service method-documentation: 查询锁定的平台录像  +by lzx@2018-03-06 V2R4B1
 int cusdk__QueryLockedRecord( cusdk__QueryLockedRecordReq req, cusdk__QueryLockedRecordRsp& rsp );


 // 码流预调配置获取.  +by lzx@2018-03-06 V2R4B1
 class cusdk__GetStreamPreDispatchConfigReq : public cusdk__BaseReq
 {	
	 
 };

 class cusdk__GetStreamPreDispatchConfigRsp /* : public cusdk__BaseRsp */
 {
	xsd__string session;	// 冗余 , 为了查问题方便过滤.
	long errorCode;

	bool enabled;			// 是否启用码流预调.
	int streamType;         // 录像码流类型。0-主流，1-辅流1，2-辅流2，3-辅流3
 };
 //gsoap cusdk service method-documentation: 获取码流预调的配置  +by lzx@2018-03-06 V2R4B1
 int cusdk__GetStreamPreDispatchConfig( cusdk__GetStreamPreDispatchConfigReq req, cusdk__GetStreamPreDispatchConfigRsp& rsp );


 // 码流预调配置设置.  +by lzx@2018-03-06 V2R4B1
 class cusdk__SetStreamPreDispatchConfigReq : public cusdk__BaseReq
 {	
	 bool enabled;			// 是否启用码流预调.
	 int streamType;         // 录像码流类型。0-主流，1-辅流1，2-辅流2，3-辅流3
 };

 class cusdk__SetStreamPreDispatchConfigRsp /* : public cusdk__BaseRsp */
 {
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 设置码流预调的配置  +by lzx@2018-03-06 V2R4B1
 int cusdk__SetStreamPreDispatchConfig( cusdk__SetStreamPreDispatchConfigReq req, cusdk__SetStreamPreDispatchConfigRsp& rsp );

 class cusdk__QueryStreamPreDispatchDeviceReq : public cusdk__BaseReq
 {
	 xsd__int beginIndex;	// 起始序号.
	 xsd__int num;			// 本次请求要的总数.
 };
 class cusdk__QueryStreamPreDispatchDeviceRsp /* : public cusdk__BaseRsp */
 {
	 int totalNum;								// 总数.
	 std::vector< cusdk__VideoSrcId > videoSrcList;		// 码流被预调的视频源列表.

	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 //gsoap cusdk service method-documentation: 查询码流预调的设备.  +by lzx@2018-03-06 V2R4B1
 int cusdk__QueryStreamPreDispatchDevice( cusdk__QueryStreamPreDispatchDeviceReq req, cusdk__QueryStreamPreDispatchDeviceRsp& rsp );

 class cusdk__StreamPreDispatchAddDeviceReq : public cusdk__BaseReq
 {	
	 std::vector< cusdk__VideoSrcId > videoSrcList;		// 码流被预调的视频源列表.
 };

 class cusdk__StreamPreDispatchAddDeviceRsp /* : public cusdk__BaseRsp */
 {		
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;

	 std::vector< cusdk__VideoSrcId > failedVideoSrcList;		// 添加失败的码流被预调的视频源列表.
 };
 // gsoap cusdk service method-documentation: 码流预调添加一个设备  +by lzx@2018-03-06 V2R4B1
 int cusdk__StreamPreDispatchAddDevice( cusdk__StreamPreDispatchAddDeviceReq req, cusdk__StreamPreDispatchAddDeviceRsp& rsp );

 class cusdk__StreamPreDispatchDelDeviceReq : public cusdk__BaseReq
 {	
	 cusdk__VideoSrcId deviceVideoSrc;	// 要删除的设备视频源.
 };

 class cusdk__StreamPreDispatchDelDeviceRsp /* : public cusdk__BaseRsp */
 {		
	 xsd__string session;	// 冗余 , 为了查问题方便过滤.
	 long errorCode;
 };
 // gsoap cusdk service method-documentation: 码流预调删除一个设备.  +by lzx@2018-03-06 V2R4B1
 int cusdk__StreamPreDispatchDelDevice( cusdk__StreamPreDispatchDelDeviceReq req, cusdk__StreamPreDispatchDelDeviceRsp& rsp );

/*
  编码器编码通道资源信息生成规则：
  1.提供设备视频源对应编码通道基础信息；
  2.大于1080P分辨率（宽高任意一项超过）额外生成一套1080P的资源，编码通道为原通道；
  3.外厂商额外生成一套科达的资源编码通道为原通道。
*/
//编码器编码通道资源信息
class cusdk__EncChnRes
{
	cusdk__DevChannel                    encChn;         //编码器编码通道

	xsd__string                          manuFactory;    //厂商名
	xsd__int                             videoWidth;     //视频宽度
	xsd__int                             videoHeight;    //视频高度
	xsd__string                          videoFormat;    //视频格式

	xsd__string                          desc;           //描述信息
	xsd__string                          reserve;        //预留
};

class cusdk__GetEncChnResReq : public cusdk__BaseReq
{	
	cusdk__DevChannel                    vidSrcChn;      //编码器视频源通道
};
class cusdk__GetEncChnResRsp /* : public cusdk__BaseRsp */
{
	std::vector<cusdk__EncChnRes>        encChnResList;  //编码器编码通道资源信息列表
	xsd__string                          session;	     //冗余 , 为了查问题方便过滤.
	long                                 errorCode;      //错误码
};
// gsoap cusdk service method-documentation: 获取编码器视频源通道资源信息.  +by Royan@2018-03-13 V2R4B1
int cusdk__GetEncChnRes( cusdk__GetEncChnResReq req, cusdk__GetEncChnResRsp& rsp );

class cusdk__AllocEncChnResReq : public cusdk__BaseReq
{
	xsd__int playId;

	cusdk__EncChnRes                     encChnRes;      //编码器编码通道资源信息

	std::vector< cusdk__MediaDescription > mediaDescriptionList; //浏览请求方的媒体信息描述.
};
class cusdk__AllocEncChnResRsp /* : public cusdk__BaseRsp */
{
	std::vector< cusdk__MediaDescription > mediaDescriptionList; //服务器方的媒体信息描述.
	std::vector< cusdk__MediaExtInfo >     mediaExtInfo;	     //媒体扩展信息.
	xsd__string                            session;	             //冗余 , 为了查问题方便过滤.
	long                                   errorCode;            //错误码
};
//gsoap cusdk service method-documentation: 申请码流资源, 创建交换.  +by Royan@2018-03-13 V2R4B1
int cusdk__AllocEncChnRes( cusdk__AllocEncChnResReq req, cusdk__AllocEncChnResRsp& rsp );

class cusdk__ReleaseEncChnResReq : public cusdk__BaseReq
{
	xsd__int playId;
};
class cusdk__ReleaseEncChnResRsp /* : public cusdk__BaseRsp */
{	

	xsd__string session;   //冗余 , 为了查问题方便过滤.
	long        errorCode; //错误码
};
//gsoap cusdk service method-documentation: 停止码流.  +by Royan@2018-03-13 V2R4B1
int cusdk__ReleaseEncChnRes( cusdk__ReleaseEncChnResReq req, cusdk__ReleaseEncChnResRsp& rsp );


class cusdk__GBPuTransdataSsSetReq : public cusdk__BaseReq
{
	cusdk__DevChannel devChn;	// 设备通道.
};
class cusdk__GBPuTransdataSsSetRsp /* : public cusdk__BaseRsp */
{	
	xsd__string session;   //冗余 , 为了查问题方便过滤.
	long        errorCode; //错误码
};
//gsoap cusdk service method-documentation: 停止码流.  +by Royan@2018-03-13 V2R4B1
int cusdk__GBPuTransdataSsSet( cusdk__GBPuTransdataSsSetReq req, cusdk__GBPuTransdataSsSetRsp& rsp );

// 获取国标推送的权限
class cusdk__GetGBDomainPrivilegeReq : public cusdk__BaseReq
{	 
	xsd__string domainId;		// GB域ID.
};

class cusdk__GetGBDomainPrivilegeRsp
{
	xsd__string domainId;		// 带回,方便抓包过滤.

	bool priRecordPlay;		// 是否有录像播放权限
	bool priRecordDownload;	// 是否有录像下载权限.
	bool priPTZ;				// 是否有PTZ权限.

	xsd__string session;
	long errorCode;
};
// 获取国标推送的权限
int cusdk__GetGBDomainPrivilege(cusdk__GetGBDomainPrivilegeReq req, cusdk__GetGBDomainPrivilegeRsp& rsp);

// 设置国标推送的权限
class cusdk__SetGBDomainPrivilegeReq : public cusdk__BaseReq
{	 
	xsd__string domainId;

	bool priRecordPlay;		// 是否有录像播放权限
	bool priRecordDownload;	// 是否有录像下载权限.
	bool priPTZ;				// 是否有PTZ权限.
};

class cusdk__SetGBDomainPrivilegeRsp
{
	xsd__string session;
	long errorCode;
};
// 设置国标推送的权限
int cusdk__SetGBDomainPrivilege(cusdk__SetGBDomainPrivilegeReq req, cusdk__SetGBDomainPrivilegeRsp& rsp);

// 获取国标的配置
class cusdk__GetGBConfigReq : public cusdk__BaseReq
{	 
};

class cusdk__GetGBConfigRsp
{
	xsd__boolean syncLatitudeAndLongitudeFromDevice;			// 是否从国标前端同步经纬度坐标

	xsd__string session;
	long errorCode;
};
// 获取国标的配置
int cusdk__GetGBConfig(cusdk__GetGBConfigReq req, cusdk__GetGBConfigRsp& rsp);

// 设置国标的配置
class cusdk__SetGBConfigReq : public cusdk__BaseReq
{	 
	xsd__boolean syncLatitudeAndLongitudeFromDevice;			// 是否从国标前端同步经纬度坐标
};

class cusdk__SetGBConfigRsp
{
	xsd__string session;
	long errorCode;
};
// 设置国标的配置
int cusdk__SetGBConfig(cusdk__SetGBConfigReq req, cusdk__SetGBConfigRsp& rsp);

// 上传证书
class cusdk__SetCertificateReq : public cusdk__BaseReq
{
	int type; // 1:设备证书 2：用户证书 其它值：非法                
	xsd__string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号                
	xsd__string validField; // 校验字段  设备证书时为密码模块id，用户证书时为用户身份证号码                

	xsd__string cert4sign;  // 签名证书PEM格式内容                
	xsd__string cert4encry; // 加密证书PEM格式内容    
};

class cusdk__SetCertificateRsp
{
	xsd__string session;
	long errorCode;
};

int cusdk__SetCertificate(cusdk__SetCertificateReq req, cusdk__SetCertificateRsp& rsp);

// 获取证书
class cusdk__GetCertificateReq : public cusdk__BaseReq
{
	xsd__string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号                
};

class cusdk__GetCertificateRsp
{
	int type; // 1:设备证书 2：用户证书 其它值：非法                
	xsd__string ownerid; // 证书拥有者id  设备证书时为设备编号，用户证书时为用户编号                
	xsd__string validField; // 校验字段  设备证书时为密码模块id，用户证书时为用户身份证号码                

	xsd__string cert4sign;  // 签名证书PEM格式内容                
	xsd__string cert4encry; // 加密证书PEM格式内容    

	xsd__string session;
	long errorCode;
};

int cusdk__GetCertificate(cusdk__GetCertificateReq req, cusdk__GetCertificateRsp& rsp);
