#ifndef _FAKE_CU_H_
#define _FAKE_CU_H_
#include "cms/ospext/ospext.h"

enum cusdk__EPlayMode
{
    cusdk__VideoAudioBoth,
    cusdk__VideoOnly,
    cusdk__AudioOnly
};

enum cusdk__ETransMode
{
    cusdk__UDP,
    cusdk__TCP
};

enum cusdk__EUserType
{
    //cusdk__UserTypeInvalid,
    cusdk__SuperAdmin,
    cusdk__Admin,
    cusdk__Operator,
};

enum cusdk__EUserPri
{
    cusdk__PriUserInvalid,              //无效的用户权限
    cusdk__PriUserCMSRecStart,			// 平台录像.
    cusdk__PriUserCMSRecPlay,			// 平台录像播放.
    cusdk__PriUserCMSRecDel,			// 平台录像删除.
    cusdk__PriUserCMSRecDownload,		// 平台录像下载.
    cusdk__PriUserLocalRec,				// 本地录像.

    cusdk__PriUserCMSRecPlayOtherDomain,// 跨域录像播放.
    cusdk__PriUserPUCall,				// 前端呼叫.		原em_Cms_Call_Ctrl
    cusdk__PriUserAlarmLog,				// 告警日志管理.	原em_Cms_Alarm_Manage
};

enum cusdk__EDevicePri
{
    cusdk__PriPUInvalid,        //无效的设备权限
    cusdk__PriPURecStart,		// 开始前端录像.
    cusdk__PriPURecPlay,		// 播放前端录像
    cusdk__PriPURecDel,		    // 删除前端录像.
    cusdk__PriPURecDownload,	// 前端录像下载.

    cusdk__PriPUConfig,	        // 前端参数配置. 针对视频源的部分怎么处理??
    cusdk__PriPUAlarmOutput,    // 前端开关量输出控制.
    cusdk__PriPUAlarmConfig,    // 布防配置.	
};

enum cusdk__EDeviceType
{
    cusdk__DevTypeUnknown,      // 非法设备类型
    cusdk__DevTypeEncoder,      // 编码设备，包含PUS，通过是否有设备存储能力标识
    cusdk__DevTypeDecoder,      // 解码设备
    cusdk__DevTypeCodecer,      // 编解码设备
    cusdk__DevTypeTVWall,       // 电视墙设备
    //放在B2版本考虑
    cusdk__DevTypeNVR,
    cusdk__DevTypeSVR,
    cusdk__DevAlarmHost,
};

enum cusdk__EPuSubscribeType
{
    cusdk__SubscribeOnlineStatus,		// 上下线.
    cusdk__SubscribeConfigStatus,		// 配置.
    cusdk__SubscribeServiceStatus,		// 录像/呼叫/解码/轮巡 等服务的状态
    cusdk__SubscribeAlarmStatus,		// 告警.
};

enum cusdk__EPtzCmd
{
    cusdk__PtzInvalidCmd,           //无效指令
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
};

enum cusdk__EUserOrUserGroup
{
    cusdk__TypeUser,
    cusdk__TypeUserGroup,
};

class cusdk__NetworkAddr
{
public:
    std::string ip;	/* required element of type xsd:string */
    int port;	/* required element of type xsd:int */
};

class cusdk__NatInfo
{
public:
    std::string natData;	/* required element of type xsd:string */
    cusdk__NetworkAddr localAddr;	/* required element of type cusdk:NetworkAddr */
    cusdk__NetworkAddr natAddr;	/* required element of type cusdk:NetworkAddr */
};

class cusdk__MediaNetInfo
{
public:
    cusdk__NatInfo videoRtpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo videoRtcpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo audioRtpNatInfo;	/* required element of type cusdk:NatInfo */
    cusdk__NatInfo audioRtcpNatInfo;	/* required element of type cusdk:NatInfo */
};



API void login();

API void logout();

API void SubscribeTvsStatus();

API void UnsubscribePuStatus();

API void GetNotify();

API void GetCUIList();

API void SetupStream();

API void PlayStream();

API void StopStream();

API void PtzCtrl();

API void PtzLock();

API void PtzUnlock();

API void SendTransData();

API void GetUserInfo();

API void GetUserGroupInfo();

API void GetUserData();

API void SetUserData();

API void GetUserDevGroupByGroup();

API void GetUserDevByGroup();

API void GetUserDevGroup();

API void GetDomainDevGroup();

API void GetUserDev();

API void GetUserDevByDomain();

API void GetDeviceGroupDeviceRelations();

API void CreateDeviceGroup();

API void DelDeviceGroup();

API void ModifyDeviceGroup();

API void DeviceGroupAddDevice();

API void DeviceGroupDelDevice();

API void GetUserList();

API void GetUserGroupList();

API void CreateUserGroup();

API void DelUserGroup();

API void ModifyUserGroup();

API void CreateUser();

API void DelUser();

API void ModifyUser();

API void UserGetDeviceByGroup();

API void UserAddDevice();

API void UserDelDevice();

API void UserModifyDevice();

API void GetAuthorizationDevice();

API void AddAuthorizationDevice();

API void DelAuthorizationDevice();

API void ModifyAuthorizationDevice();

API void GetDeviceBaseInfo();

//API void GetRCSSysParam();

API void GetAlarmLinkConfig();

API void SetAlarmLinkConfig();

API void QueryAlarmLinkConfig();

API void TVWallDelScheme();

API void TVWallQueryScheme();

API void TVWallSaveScheme();

API void TVWallLoadScheme();

API void TVWallStartPlay();

API void TVWallStopPlay();

API void TVWallWindowPollContrl();

API void CallPu();

API void QueryMap();

API void CreateMap();

API void ModifyMap();

API void DelMap();

API void CreateMapElement();

API void ModifyMapElement();

API void DelMapElement();

API void QueryMapElement();

#endif

