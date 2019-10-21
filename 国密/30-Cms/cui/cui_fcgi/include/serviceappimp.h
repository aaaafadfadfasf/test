/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : serviceappimp.
   相关文件    : 
   文件实现功能: cui_fcgi功能
   作者        : 梁  黎
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/09/23  1.0         梁  黎        创建
******************************************************************************/
#ifndef _SERVICE_APP_IMP_H_
#define _SERVICE_APP_IMP_H_

#include <string>
#include "kdvtype.h"
#include "../../common/cuiproto.h"

string GetCuiServerIpStr();
u16 GetCuiServerTcpListenPort();
u32 GetCuiServerTimeout();  //单位秒
u32 GetCuiServerReadWriteTimeout();
/*====================================================================
函数名：CuLogin
功能：CU登录的验证
算法实现：
引用全局变量：
输入参数说明：CCuLoginMsg &cLoginReq 用户名和密码
             CLoginRsp &cloginRsp 登录结果
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuLogin(CCuiCuLoginReq &cLoginReq, CCuiCuLoginRsp &cLoginRsp);

/*====================================================================
函数名：CuLogout
功能：CU退出登录
算法实现：
引用全局变量：
输入参数说明：CLogoutRsp &logoutRsp 退出结果
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuLogout(CCuLogoutReq &cLogoutReq, CCuLogoutRsp &cLogoutRsp);

/*====================================================================
函数名：CuGetNotify
功能：作用有二：1，CU保活；2，获取平台推送通知消息
算法实现：
引用全局变量：
输入参数说明：CGetNotifyReq &cGetNotifyReq  用户的SessionID
             CGetNotifyRsp &cGetNotifyRsp  返回的推送消息，如果没有推送消息则为空结构体
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuGetNotify(CGetNotifyReq &cGetNotifyReq, CGetNotifyRsp &cGetNotifyRsp);

/*====================================================================
函数名      : CuGetCuiList
功能        : 获取本域在线CUI列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/1/7/             liangli
====================================================================*/
s32 CuGetCuiList(CCuGetCuiListReq &cCuGetCuiListReq, CCuGetCuiListRsp &cCuGetCuiListRsp);

/*====================================================================
函数名      : CuPtzCtrl
功能        : 对前端摄像头进行控制
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/1/31/             liangli
====================================================================*/
s32 CuPtzCtrl(CPuPtzCtrlReq &cCuPtzCtrlReq, CPuPtzCtrlRsp &cCuPtzCtrlRsp);

/*====================================================================
函数名      : CuPtzLock
功能        : 对前端摄像头进行加锁
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
s32 CuPtzLock(CPuPtzLockReq &cCuPtzLockReq, CPuPtzLockRsp &cCuPtzLockRsp);				// for cmu
s32 CuPtzLock(CCuPtzLockAddReq &cCuPtzLockAddReq, CCuPtzLockAddRsp &cCuPtzLockAddRsp);	// for tas

/*====================================================================
函数名      : CuPtzUnlock
功能        : 对前端摄像头进行解锁
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
s32 CuPtzUnlock(CPuPtzUnlockReq &cCuPtzUnlockReq, CPuPtzUnlockRsp &cCuPtzUnlockRsp);		// for cmu
s32 CuPtzUnlock(CCuPtzLockDelReq &cCuPtzLockDelReq, CCuPtzLockDelRsp &cCuPtzLockDelRsp);	// for tas

/*====================================================================
函数名      : CuSendTransData
功能        : 发送透明串口数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
s32 CuSendTransData(CCuPuTransparentOperateReq &cSendDataReq, CCuPuTransparentOperateRsp &cSendDataRsp);

/*====================================================================
函数名      : CuQueryTransData
功能        : query透明串口数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
s32 CuQueryTransData(CCuPuTransparentQryReq &cQryReq, CCuPuTransparentQryRsp &cQryRsp);

/*====================================================================
函数名      : CuGetSelfUserInfo
功能        : CU查询自身的用户信息
算法实现    : CU查询自身用户信息，以及查询有权限的其他用户信息，对3AS都是同一条信令
             所以不能由CuiServer来区分是那种查询，只能由cui_fcgi来区分
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/2/             liangli
====================================================================*/
s32 CuGetUserInfo(CCuQueryUserReq &cCuQueryUserReq, CCuQueryUserRsp &cCuQueryUserRsp);

/*====================================================================
函数名      : CuGetSelfUserGroupInfo
功能        : CU查询自身的用户组信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/5/             liangli
====================================================================*/
s32 CuGetUserGroupInfo(CCuUserGroupQryReq &cCuQueryUserGroupReq, CCuUserGroupQryRsp &cCuQueryUserGroupRsp);

/*====================================================================
函数名      : CuGetUserData
功能        : 获取CU自定义数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/5/             liangli
====================================================================*/
s32 CuGetUserData(CCuUserCustomDataQryReq &cReq, CCuUserCustomDataQryRsp &cRsp);

/*====================================================================
函数名      : CuSetUserData
功能        : 设置CU自定义数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/5/             liangli
====================================================================*/
s32 CuSetUserData(CCuUserCustomDataAddReq &cReq, CCuUserCustomDataAddRsp &cRsp);

s32 CuDelUserData(CCuUserCustomDataDelReq &cReq, CCuUserCustomDataDelRsp &cRsp);

/*====================================================================
函数名      : CuGetUserDevGroup
功能        : 获取用户拥有的设备组，即只获取用户设备树中的树节点，不包含叶子节点
              获取的设备组用有“hasDev”字段，标识是否空组
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/6/             liangli
====================================================================*/
s32 CuGetUserDevGroup(CCuUserDevGroupQryReq &cCuGetUserDevGroupReq, CCuUserDevGroupQryRsp &cCuGetUserDevGroupRsp);

/*====================================================================
函数名      : CuGetUserDevFullInfo
功能        : 获取用户设备完整信息列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/6/             liangli
====================================================================*/
s32 CuGetUserDevFullInfo(CCuUserDevFullInfoQryReq &cCuGetUserDevFullInfoReq, CCuUserDevFullInfoQryRsp &cCuGetUserDevFullInfoRsp);

/*====================================================================
函数名      : CuSubscribePuStatus
功能        : 订阅pu状态，这里可能订阅pu的不同状态
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/7/             liangli
====================================================================*/
s32 CuSubscribePuStatus(CCuSubscribeNotifyReq &cCuSubscribeNotifyReq, CCuSubscribeNotifyRsp &cCuSubscribeNotifyRsp);

/*====================================================================
函数名      : CuUnsubscribePuStatus
功能        : 取消PU状态订阅
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/7/             liangli
====================================================================*/
s32 CuUnsubscribePuStatus(CCuUnSubscribeNotifyReq &cCuUnSubscribePuStatusReq, CCuUnSubscribeNotifyRsp &cCuUnSubscribePuStatusRsp);

/*====================================================================
函数名      : CuGetDeviceGroupOfDeviceInfo
功能        : 获取设备和设备组的对应关系
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuGetDeviceGroupOfDeviceInfo(CCuQryDevGroupOfDevInfoReq &cCuQryDevGroupOfDevInfoReq, CCuQryDevGroupOfDevInfoRsp &cCuQryDevGroupOfDevInfoRsp);

/*====================================================================
函数名      : CuCreateDeviceGroup
功能        : 创建设备组
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuCreateDeviceGroup(CCuCreateDeviceGroupReq &cCuCreateDeviceGroupReq, CCuCreateDeviceGroupRsp &cCuCreateDeviceGroupRsp);

/*====================================================================
函数名      : CuDeleteDeviceGroup
功能        : 删除设备组
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuDeleteDeviceGroup(CCuDeleteDeviceGroupReq &cCuDeleteDeviceGroupReq, CCuDeleteDeviceGroupRsp &cCuDeleteDeviceGroupRsp);

/*====================================================================
函数名      : CuModifyDeviceGroup
功能        : 修改设备组信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuModifyDeviceGroup(CCuModifyDeviceGroupReq &cCuModifyDeviceGroupReq, CCuModifyDeviceGroupRsp &cCuModifyDeviceGroupRsp);

/*====================================================================
函数名      : CuDeviceGroupAddDevice
功能        : 设备组中添加设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuDeviceGroupAddDevice(CCuDeviceGroupAddDeviceReq &cCuDeviceGroupAddDeviceReq, CCuDeviceGroupAddDeviceRsp &cCuDeviceGroupAddDeviceRsp);

s32 CuDeviceGroupAddGroup(CCuDevTreeDevGrpAssignReq &cCuDevTreeDevGrpAssignReq, CCuDevTreeDevGrpAssignRsp &cCuDevTreeDevGrpAssignRsp);
/*====================================================================
函数名      : CuDeviceGroupDelDevice
功能        : 设备组中添加设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
s32 CuDeviceGroupDelDevice(CCuDeviceGroupDelDeviceReq &cCuDeviceGroupDelDeviceReq, CCuDeviceGroupDelDeviceRsp &cCuDeviceGroupDelDeviceRsp);

/*====================================================================
函数名      : CuCreateUserGroup
功能        : 创建新的用户组
算法实现    : 用户组没有树的层级结构，该用户创建的用户组只被该用户看到
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuCreateUserGroup(CCuCreateUserGroupReq &cCuCreateUserGroupReq, CCuCreateUserGroupRsp &cCuCreateUserGroupRsp);

/*====================================================================
函数名      : CuDeleteUserGroup
功能        : 删除用户组
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuDeleteUserGroup(CCuDeleteUserGroupReq &cCuDeleteUserGroupReq, CCuDeleteUserGroupRsp &cCuDeleteUserGroupRsp);

/*====================================================================
函数名      : CuModifyUserGroup
功能        : 修改用户组基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuModifyUserGroup(CCuModifyUserGroupReq &cCuModifyUserGroupReq, CCuModifyUserGroupRsp &cCuModifyUserGroupRsp);

/*====================================================================
函数名      : CuCreateUser
功能        : 创建子用户
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuCreateUser(CCuCreateUserReq &cCuCreateUserReq, CCuCreateUserRsp &cCuCreateUserRsp);

/*====================================================================
函数名      : CuDeleteUser
功能        : 删除子用户
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuDeleteUser(CCuDeleteUserReq &cCuDeleteUserReq, CCuDeleteUserRsp &cCuDeleteUserRsp);

/*====================================================================
函数名      : CuModifyUser
功能        : 修改子用户基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuModifyUser(CCuModifyUserReq &cCuModifyUserReq, CCuModifyUserRsp &cCuModifyUserRsp);

/*====================================================================
函数名      : CuChangePassword
功能        : 修改子用户基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuChangePassword(CCuChangePasswordReq &cCuChangePasswordReq, CCuChangePasswordRsp &cCuChangePasswordRsp);

/*====================================================================
函数名      : CuSendKeyFrame
功能        : 修改子用户基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuSendKeyFrame(CCuKeyFrameReq &cCuKeyFrameReq, CCuKeyFrameRsp &CCuKeyFrameRsp);

/*====================================================================
函数名      : CuGetUserDevice
功能        : 获取用户的设备划归列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuGetUserDevice(CCuGetUserDeviceReq &cCuGetUserDeviceReq, CCuGetUserDeviceRsp &cCuGetUserDeviceRsp);

/*====================================================================
函数名      : CuGetUserGroupDevice
功能        : 获取用户组的设备划归列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
s32 CuGetUserGroupDevice(CCuGetUserGroupDeviceReq &cCuGetUserGroupDeviceReq, CCuGetUserGroupDeviceRsp &cCuGetUserGroupDeviceRsp);

/*====================================================================
函数名      : CuUserAddDevice
功能        : 用户管理之用户添加设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserAddDevice(CCuUserAddDeviceReq &cCuUserAddDeviceReq, CCuUserAddDeviceRsp &cCuUserAddDeviceRsp);

/*====================================================================
函数名      : CuUserDelDevice
功能        : 用户管理之用户删除设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserDelDevice(CCuUserDelDeviceReq &cCuUserDelDeviceReq, CCuUserDelDeviceRsp &cCuUserDelDeviceRsp);

/*====================================================================
函数名      : CuUserModDevice
功能        : 用户管理之用户修改设备权限
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserModDevice(CCuUserModDeviceReq &cCuUserModDeviceReq, CCuUserModDeviceRsp &cCuUserModDeviceRsp);

/*====================================================================
函数名      : CuUserGroupAddDevice
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupAddDevice(CCuUserGroupAddDeviceReq &cCuUserGroupAddDeviceReq, CCuUserGroupAddDeviceRsp &cCuUserGroupAddDeviceRsp);

/*====================================================================
函数名      : CuUserGroupDelDevice
功能        : 用户组删除设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupDelDevice(CCuUserGroupDelDeviceReq &cCuUserGroupDelDeviceReq, CCuUserGroupDelDeviceRsp &cCuUserGroupDelDeviceRsp);

/*====================================================================
函数名      : CuUserGroupModDevice
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
s32 CuUserGroupModDevice(CCuUserGroupModDeviceReq &cCuUserGroupModDeviceReq, CCuUserGroupModDeviceRsp &cCuUserGroupModDeviceRsp);

/*====================================================================
函数名      : CuGetGetAuthorizationDevice
功能        : 获取本域临时授权设备列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
s32 CuGetGetAuthorizationDevice(CCuGetAuthorizationDeviceReq &cCuGetAuthorizationDeviceReq, CCuGetAuthorizationDeviceRsp &cCuGetAuthorizationDeviceRsp);

/*====================================================================
函数名      : CuAddAuthorizationDevice
功能        : 向目标域的指定用户添加临时授权设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
s32 CuAddAuthorizationDevice(CCuAddAuthorizationDeviceReq &cCuAddAuthorizationDeviceReq, CCuAddAuthorizationDeviceRsp &cCuAddAuthorizationDeviceRsp);

/*====================================================================
函数名      : CuDelAuthorizationDevice
功能        : 删除临时授权设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
s32 CuDelAuthorizationDevice(CCuDelAuthorizationDeviceReq &cCuDelAuthorizationDeviceReq, CCuDelAuthorizationDeviceRsp &cCuDelAuthorizationDeviceRsp);

/*====================================================================
函数名      : CuModifyAuthorizationDevice
功能        : 修改临时授权设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
s32 CuModifyAuthorizationDevice(CCuModifyAuthorizationDeviceReq &cCuModifyAuthorizationDeviceReq, CCuModifyAuthorizationDeviceRsp &cCuModifyAuthorizationDeviceRsp);

/*====================================================================
函数名      : CuGetDeviceBaseInfo
功能        : 获取指定设备基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
s32 CuGetDeviceBaseInfo(CCuGetDeviceBaseInfoReq &cCuGetDeviceBaseInfoReq, CCuGetDeviceBaseInfoRsp &cCuGetDeviceBaseInfoRsp);

/*====================================================================
函数名      : CuSetupStream
功能        : Cu发起Invite请求
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetupStream(CCuSetupStreamReq &cCuSetupStreamReq, CCuSetupStreamRsp &cCuSetupStreamRsp);

/*====================================================================
函数名      : CuPlayStream
功能        : CU发起InviteAck请求
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuPlayStream(CCuPlayStreamReq &cCuPlayStreamReq, CCuPlayStreamRsp &cCuPlayStreamRsp);

/*====================================================================
函数名      : CuStopStream
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStopStream(CCuStopStreamReq &cCuStopStreamReq, CCuStopInviteRsp &cCuStopStreamRsp);

/*====================================================================
函数名      : CuGetRCSSysParam
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetRCSSysParam(CCuRcsGetSysParamReq &cCuRcsGetSysParamReq, CCuRcsGetSysParamRsp &cCuRcsGetSysParamRsp);

/*====================================================================
函数名      : CuGetRCSSysParam
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetRCSSysParam(CCuRcsSetSysParamReq &cCuRcsSetSysParamReq, CCuRcsSetSysParamRsp &cCuRcsSetSysParamRsp);

/*====================================================================
函数名      : CuGetNruList
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetNruList(CCuRcsQueryNruReq &cCuRcsQueryNruReq, CCuRcsQueryNruRsp &cCuRcsQueryNruRsp);

/*====================================================================
函数名      : CuGetPuChnBinding
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetPuChnBinding(CCuRcsQueryPuChnBindingReq &cCuRcsQueryPuChnBindingReq, CCuRcsQueryPuChnBindingRsp &cCuRcsQueryPuChnBindingRsp);

/*====================================================================
函数名      : CuBindPu
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuBindPu(CCuRcsBindingPuChnReq &cCuRcsBindingPuChnReq, CCuRcsBindingPuChnRsp &cCuRcsBindingPuChnRsp);

/*====================================================================
函数名      : CuUnBindPu
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuUnBindPu(CCuRcsUnbindingPuChnReq &cCuRcsUnbindingPuChnReq, CCuRcsUnbindingPuChnRsp &CCuRcsUnbindingPuChnRsp);

/*====================================================================
函数名      : CuGetPlatformRecordDays
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetPlatformRecordDays(CCuRcsQueryRecordDaysReq &cCuRcsQueryRecordDaysReq, CCuRcsQueryRecordDaysRsp &CCuRcsQueryRecordDaysRsp);

s32 CuGetPuRecordDays(CCuPuRecDaysQryReq &cCuiPuRecDaysQryReq, CCuPuRecDaysQryRsp &cCuiPuRecDaysQryRsp);

s32 CuStartExport(CCuFileExportReq &cCuFileExportReq, CCuFileExportRsp &cCuFileExportRsp);
s32 CuGetExportProcess(CCuGetExportProcessReq &cCuGetExportUserLogProcessReq, CCuGetExportProcessRsp &cCuGetExportUserLogProcessRsp);
s32 CuStopExport(CCuStopExportReq &cCuStopExportUserLogReq, CCuStopExportRsp &cCuStopExportUserLogRsp);

s32 CuStartExportEx(CCuFileExportStartExReq &cReq, CCuFileExportStartExRsp &cRsp);
s32 CuGetExportProcessEx(CCuFileExportGetProcessExReq &cReq, CCuFileExportGetProcessExRsp &cRsp);
s32 CuStopExportEx(CCuFileExportStopExReq &cReq, CCuFileExportStopExRsp &cRsp);

s32 CuStartImportEx(CCuFileImportStartExReq &cReq, CCuFileImportStartExRsp &cRsp);
s32 CuSetImportProcessEx(CCuFileImportSetProcessExReq &cReq, CCuFileImportSetProcessExRsp &cRsp);
s32 CuStopImportEx(CCuFileImportStopExReq &cReq, CCuFileImportStopExRsp &cRsp);

/*====================================================================
函数名      : CuGetPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetPlatformRecord(CCuRcsQueryRecordReq &cCuRcsQueryRecordReq, CCuRcsQueryRecordRsp &cCuRcsQueryRecordRsp);



/*====================================================================
函数名      : CuStartPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStartPlatformRecord(CCuRcsSetRecordTaskReq &cCuRcsSetRecordTaskReq, CCuRcsSetRecordTaskRsp &cCuRcsSetRecordTaskRsp);

/*====================================================================
函数名      : CuStartPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlatformRecord(CCuRcsStopRecordTaskReq &cCuRcsStopRecordTaskReq, CCuRcsStopRecordTaskRsp &cCuRcsStopRecordTaskRsp);

/*====================================================================
函数名      : CuSetupPlayPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetupPlayPlatformRecord(CCuSetupPlayPlatformRecordReq &cCuSetupPlayPlatformRecordReq, CCuSetupPlayPlatformRecordRsp &cCuSetupPlayPlatformRecordRsp);

/*====================================================================
函数名      : CuPlayPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuPlayPlatformRecord(CCuPlayPlatformRecordReq &cCuPlayPlatformRecordReq, CCuPlayPlatformRecordRsp &cCuPlayPlatformRecordRsp);

/*====================================================================
函数名      : CuVcrCtrl
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuVcrCtrl(CCuVcrCtrlReq &cCuVcrCtrlReq, CCuVcrCtrlRsp &cCuVcrCtrlRsp);

/*====================================================================
函数名      : CuGetCurRecordPlayPosition
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetCurRecordPlayPosition(CCuPlaybackProgressQueryReq &cCuReplayProgressQueryReq, CCuPlaybackProgressQueryRsp &cCuReplayProgressQueryRsp);

/*====================================================================
函数名      : CuStopPlayPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlayPlatformRecord(CCuStopPlatformRecordReq &cCuStopPlatformRecordReq, CCuStopPlatformRecordRsp &cCuStopPlatformRecordRsp);

/*====================================================================
函数名      : CuSetupDownloadPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetupDownloadPlatformRecord(CCuDownloadPlatformRecordReq &cCuDownloadPlatformRecordReq, CCuDownloadPlatformRecordRsp &cCuDownloadPlatformRecordRsp );

/*====================================================================
函数名      : CuStartDownloadPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStartDownloadPlatformRecord(CCuStartDownloadPlatformRecordReq &cCuStartDownloadPlatformRecordReq, CCuStartDownloadPlatformRecordRsp &cCuStartDownloadPlatformRecordRsp);

/*====================================================================
函数名      : CuStopDownloadPlatformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStopDownloadPlatformRecord(CCuStopDownloadPlatformRecordReq &cCuStopDownloadPlatformRecordReq, CCuStopDownloadPlatformRecordRsp &cCuStopDownloadPlatformRecordRsp);

/*====================================================================
函数名      : CuQueryGpsTrece
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/5/20/             qiqianqian
====================================================================*/
s32 CuQueryGpsTrece(CCuGpsTraceQryReq &cCuGpsTraceQryReq, CCuGpsTraceQryRsp &cCuGpsTraceQryRsp);

/*====================================================================
函数名      : CuStartPuformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/5/19/             qiqianqian
====================================================================*/
s32 CuStartPuRecord(CCuPuRecordStartReq &cCuPuRecordStartReq, CCuPuRecordStartRsp &cCuPuRecordStartRsp);


s32 CuGetDeviceSerialConfig(CCuPuSerialParamCfgGetReq &cReq, CCuPuSerialParamCfgGetRsp &cRsp);
s32 CuSetDeviceSerialConfig(CCuPuSerialParamCfgSetReq &cReq, CCuPuSerialParamCfgSetRsp &cRsp);

/*====================================================================
函数名      : CuStopPuformRecord
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/5/19/             qiqianqian
====================================================================*/
s32 CuStopPuRecord(CCuPuRecordStopReq &cCuPuRecordStopReq, CCuPuRecordStopRsp &cCuPuRecordStopRsp);

/*====================================================================
函数名      : CuGetDomainList
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetDomainList(CCuDomainRelationQryReq &cCuDomainRelationQryReq, CCuDomainRelationQryRsp& cCuDomainRelationQryRsp);

/*====================================================================
函数名      : CuGetRecordPlan
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetRecordPlan(CCuRcsQueryRecordParamReq &cCuRcsQueryRecordParamReq, CCuRcsQueryRecordParamRsp &cCuRcsQueryRecordParamRsp);

/*====================================================================
函数名      : CuSetRecordPlan
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetRecordPlan(CCuRcsSetRecordParamReq &cCuRcsSetRecordParamReq, CCuRcsSetRecordParamRsp &cCuRcsSetRecordParamRsp);

/*====================================================================
函数名      : CuGetAllTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetAllTVWall(CCuTvwallQryReq &cCuTvwallQryReq, CCuTvwallQryRsp &cCuTvwallQryRsp);

/*====================================================================
函数名      : CCuCreatTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CCuCreatTVWall(CCuTvwallAddReq &cCuTvwallAddReq, CCuTvwallAddRsp &cCuTvwallAddRsp);

/*====================================================================
函数名      : CuDelTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuDelTVWall(CCuTvwallDelReq &cCuTvwallDelReq, CCuTvwallDelRsp &cCuTvwallDelRsp);

/*====================================================================
函数名      : CuModifyTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuModifyTVWall(CCuTvwallModReq &cCuTvwallModReq, CCuTvwallModRsp &cCuTvwallModRsp);

/*====================================================================
函数名      : CuPlayRecordOnTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuPlayRecordOnTVWall(CCuRecordOnTvReq &cCuPlatRecOnTvReq, CCuRecordOnTvRsp &cCuPlatRecOnTvRsp);

/*====================================================================
函数名      : CuStopPlayRecordOnTVWall
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuStopPlayRecordOnTVWall(CCuRecordOffTvReq &cCuPlatRecOffTvReq, CCuRecordOffTvRsp &cCuPlatRecOffTvRsp);
/*====================================================================
函数名      : CuTVWallStartPlay
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallStartPlay(CCuRealMediaOnTvReq &cCuRealMediaOnTvReq, CCuRealMediaOnTvRsp &cCuRealMediaOnTvRsp);

/*====================================================================
函数名      : CuTVWallStopPlay
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallStopPlay(CCuRealMediaOffTvReq &cCuRealMediaOffTvReq, CCuRealMediaOffTvRsp &cCuRealMediaOffTvRsp);

/*====================================================================
函数名      : CuTVWallWindowPollContrl
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallWindowPollContrl(CCuTvPollOperateReq &cCuTvPollOperateReq, CCuTvPollOperateRsp &cCuTvPollOperateRsp);

/*====================================================================
函数名      : CuTVWallSaveScheme
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSaveScheme(CCuTvWallSchemeSaveReq &cCuTvWallSchemeSaveReq, CCuTvWallSchemeSaveRsp &cCuTvWallSchemeSaveRsp);

/*====================================================================
函数名      : CuTVWallDelScheme
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallDelScheme(CCuTvWallSchemeDelReq &cCuTvWallSchemeDelReq, CCuTvWallSchemeDelRsp &cCuTvWallSchemeDelRsp);

/*====================================================================
函数名      : CuTVWallLoadScheme
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallLoadScheme(CCuTvWallSchemeLoadReq &cCuTvWallSchemeLoadReq, CCuTvWallSchemeLoadRsp &cCuTvWallSchemeLoadRsp);

/*====================================================================
函数名      : CuTVWallQueryScheme
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallQueryScheme(CCuTvWallSchemeQueryReq &cCuTvWallSchemeQueryReq, CCuTvWallSchemeQueryRsp &cCuTvWallSchemeQueryRsp);

/*====================================================================
函数名      : CuTVWallSetSchemePoll
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSetSchemePoll(CCuTvWallSchemeGroupSetReq &cCuTvWallSchemeGroupSetReq, CCuTvWallSchemeGroupSetRsp &cCuTvWallSchemeGroupSetRsp);

/*====================================================================
函数名      : CuTVWallGetSchemePoll
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallGetSchemePoll(CCuTvWallSchemeGroupGetReq &cCuTvWallSchemeGroupGetReq, CCuTvWallSchemeGroupGetRsp &cCuTvWallSchemeGroupGetRsp);

/*====================================================================
函数名      : CuTVWallSchemePollControl
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuTVWallSchemePollControl(CCuTvWallSchemePollOperateReq &cCuTvWallSchemePollOperateReq, CCuTvWallSchemePollOperateRsp &cCuTvWallSchemePollOperateRsp);

/*====================================================================
函数名      : CuCreateMap
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuCreateMap(CCuMapCreateReq &cCuMapCreateReq, CCuMapCreateRsp &cCuMapCreateRsp);

/*====================================================================
函数名      : CuModifyMap
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuModifyMap(CCuMapModReq &cCuMapModReq, CCuMapModRsp &cCuMapModRsp);

/*====================================================================
函数名      : CuDelMap
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuDelMap(CCuMapDelReq &cCuMapDelReq, CCuMapDelRsp &cCuMapDelRsp);

/*====================================================================
函数名      : CuQueryMap
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryMap(CCuMapQryReq &cCuMapQryReq, CCuMapQryRsp &cCuMapQryRsp);

/*====================================================================
函数名      : CreateMapElement
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuCreateMapElement(CCuMapElementCreateReq &cCuMapElementCreateReq, CCuMapElementCreateRsp &cCuMapElementCreateRsp);

/*====================================================================
函数名      : CuModifyMapElement
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuModifyMapElement(CCuMapElementModReq &cCuMapElementModReq, CCuMapElementModRsp &cCuMapElementModRsp);

/*====================================================================
函数名      : CuDelMapElement
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuDelMapElement(CCuMapElementDelReq &cCuMapElementDelReq, CCuMapElementDelRsp &cCuMapElementDelRsp);

/*====================================================================
函数名      : CuQueryMapElement
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryMapElement(CCuMapElementQryReq &cCuMapElementQryReq, CCuMapElementQryRsp &cCuMapElementQryRsp);

s32 CuSearchMapElement(CCuMapElementSearchReq &cCuMapElementSearchReq, CCuMapElementSearchRsp &cCuMapElementSearchRsp);
/*====================================================================
函数名      : CuMapPositionMar2Earth
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuMapPositionMar2Earth(CCuPosCorrectMarsToEarthReq &cCuPosCorrectMarsToEarthReq, CCuPosCorrectMarsToEarthRsp &cCuPosCorrectMarsToEarthRsp);

/*====================================================================
函数名      : CuMapPositionMar2Earth
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuMapPositionEarth2Mar(CCuPosCorrectEarthToMarsReq &cCuPosCorrectEarthToMarsReq, CCuPosCorrectEarthToMarsRsp &cCuPosCorrectEarthToMarsRsp);

s32 CuEMapDataVersion(CCuMpsDataVersionGetReq &cReq, CCuMpsDataVersionGetRsp &cRsp);

s32 CuEGpsDataBackUp(CCuGpsDataBakeUpReq &cReq,CCuGpsDataBakeUpRsp &cRsp);

s32 CuGpsStoreCfgGet(CCuMpsGpsStoreCfgGetReq &cReq,CCuMpsGpsStoreCfgGetRsp &cRsp);
/*====================================================================
函数名      : CuQueryAlarmLinkConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryAlarmLinkConfig(CCuAlarmLinkQueryReq &cCuAlarmLinkQueryReq, CCuAlarmLinkQueryRsp &cCuAlarmLinkQueryRsp);

s32 CuGetAlarmLinkDataVersion(CCuAlarmLinkVersionGetReq &cCuAlarmLinkQueryReq, CCuAlarmLinkVersionGetRsp &cCuAlarmLinkQueryRsp);

/*====================================================================
函数名      : CuGetAlarmLinkConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetAlarmLinkConfig(CCuAlarmLinkCfgGetReq &cCuAlarmLinkCfgGetReq, CCuAlarmLinkCfgGetRsp &cCuAlarmLinkCfgGetRsp);

/*====================================================================
函数名      : CuSetAlarmLinkConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetAlarmLinkConfig(CCuAlarmLinkCfgSetReq &cCuAlarmLinkCfgSetReq, CCuAlarmLinkCfgSetRsp &cCuAlarmLinkCfgSetRsp);

/*====================================================================
函数名      : CuQueryUserLog
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryUserLog(CCuOperateLogQryReq &cCuOperateLogQryReq, CCuOperateLogQryRsp &cCuOperateLogQryRsp);

/*====================================================================
函数名      : CuQueryDeviceLog
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryDeviceLog(CCuDeviceLogQryReq &cCuDeviceLogQryReq, CCuDeviceLogQryRsp &cCuDeviceLogQryRsp);

/*====================================================================
函数名      : CuQueryAlarmLog
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuQueryAlarmLog(CCuAlarmLogQryReq &cCuAlarmLogQryReq, CCuAlarmLogQryRsp &cCuAlarmLogQryRsp);

/*====================================================================
函数名      : CuGetLogConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetLogConfig(CCuConfigUasGetReq &cCuConfigUasGetReq, CCuConfigUasGetRsp &cCuConfigUasGetRsp);

/*====================================================================
函数名      : CuSetLogConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetLogConfig(CCuConfigUasSetReq &cCuConfigUasSetReq, CCuConfigUasSetRsp &cCuConfigUasSetRsp);

/*====================================================================
函数名      : CuGetDeviceGBID
功能        : 获取设备国标id
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/08/04/             qiqianqian
====================================================================*/
s32 CuGetDeviceGBID(CCuGbsDevGbidGetReq &cCuGbsDevGbidGetReq, CCuGbsDevGbidGetRsp &cCuGbsDevGbidGetRsp);

/*====================================================================
函数名      : CuCallPu
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuCallPu(CCuStartThirdPartyCallReq &cCuStartThirdPartyCallReq, CCuStartThirdPartyCallRsp &cCuStartThirdPartyCallRsp );

/*====================================================================
函数名      : CuHangUpPu
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuHangUpPu(CCuStopThirdPartyCallReq &cCuStopThirdPartyCallReq, CCuStopThirdPartyCallRsp &cCuStopThirdPartyCallRsp);
/*====================================================================
函数名      : CuGetDeviceEncoderParam
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetDeviceVidEncoderParam(CCuPuVidEncParamGetReq &cPuVidEncParamGetReq, CCuPuVidEncParamGetRsp &cPuVidEncParamGetRsp);

s32 CuGetDeviceAudEncoderParam(CCuPuAudEncParamGetReq &cPuAudEncParamGetReq, CCuPuAudEncParamGetRsp &cPuAudEncParamGetRsp);
/*====================================================================
函数名      : CuSetDeviceEncoderParam
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetDeviceEncoderParam(CCuPuVidEncParamSetReq &cCuPuVidEncParamSetReq, CCuPuVidEncParamSetRsp &cCuPuVidEncParamSetRsp);

/*====================================================================
函数名      : CuGetDiskList
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetDiskList(CCuRcsQueryDiskInfoReq &cCuRcsQueryDiskInfoReq, CCuRcsQueryDiskInfoRsp &cCuRcsQueryDiskInfoRsp);

/*====================================================================
函数名      : CuSetDeviceName
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetDeviceName(CCuDeviceNameModReq &cCuDeviceNameModReq, CCuDeviceNameModRsp &cCuDeviceNameModRsp);

/*====================================================================
函数名      : CuSetVideoSrcName
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuSetVideoSrcName(CCuDeviceCapIndexNameSetReq &cCuDeviceCapIndexNameSetReq, CCuDeviceCapIndexNameSetRsp &cCuDeviceCapIndexNameSetRsp);

/*====================================================================
函数名      : CuGetMotionDetectConfig
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
s32 CuGetMotionDetectConfig(CCuPuMotionDetectGetReq &cReq, CCuPuMotionDetectGetRsp &cRsp);

s32 CuSetMotionDetectConfig(CCuPuMotionDetectSetReq &cReq, CCuPuMotionDetectSetRsp &cRsp);

s32 CuGetShadeConfig(CCuPuAreaShadeGetReq &cReq, CCuPuAreaShadeGetRsp &cRsp);

s32 CuSetShadeConfig(CCuPuAreaShadeSetReq& cReq, CCuPuAreaShadeSetRsp& cRsp);

s32 CuSetTVDivNum(CTvWinStyleSetReq& cReq, CTvWinStyleSetRsp& cRsp);

s32 CuGetDeviceChannelAlias(CCuDeviceCapIndexNameGetReq& cReq, CCuDeviceCapIndexNameGetRsp& cRsp);

s32 CuSetDeviceChannelAlias(CCuDeviceCapIndexNameSetReq& cReq, CCuDeviceCapIndexNameSetRsp& cRsp);

s32 CuGetSyncVideoSrcNameEnableAlias(CCuGetSyncVideoSrcNameEnableReq& cReq, CCuGetSyncVideoSrcNameEnableRsp& cRsp);

s32 CuSetSyncVideoSrcNameEnableAlias(CCuSetSyncVideoSrcNameEnableReq& cReq, CCuSetSyncVideoSrcNameEnableRsp& cRsp);

s32 CuGetSupportMapTypes(CCuGetSupportMapTypesReq& cReq, CCuGetSupportMapTypesRsp& cRsp);

s32 CuQueryMapArea(CCuMapAreaQryReq& cReq, CCuMapAreaQryRsp& cRsp);

s32 CuCreateMapArea(CCuMapAreaCreateReq& cReq, CCuMapAreaCreateRsp& cRsp);

s32 CuModifyMapArea(CCuMapAreaModReq& cReq, CCuMapAreaModRsp& cRsp);

s32 CuVsipChannelData(CCuPuVsipOperateReq& cReq, CCuPuVsipOperateRsp& cRsp);

s32 CuGetMapImageUploadUrl(CCuMapUploadReq& cReq, CCuMapUploadRsp& cRsp);

s32 CuGetMapElementImageUploadUrl(CCuElementIconUploadReq& cReq, CCuElementIconUploadRsp& cRsp);

s32 CuQueryMapElementImageList(CCuElementImageQryReq& cReq, CCuElementImageQryRsp& cRsp);

s32 CuSetCameraProtocolType(CCuPuCameraTypeSetReq& cReq, CCuPuCameraTypeSetRsp& cRsp);

s32 CuResetAlarm(CCuPuAlarmResetReq& cReq, CCuPuAlarmResetRsp& cRsp);

s32 CuDeviceRecordSvrQry(CCuDeviceRecordSvrQryReq& cReq, CCuDeviceRecordSvrQryRsp& cRsp);

s32 CuPuRecord(CCuPuRecQryReq &cReq, CCuPuRecQryRsp &cRsp);

s32 CuGetPtzPresetName(CCuVideosourcePresetcfgQryReq &cReq, CCuVideosourcePresetcfgQryRsp &cRsp);

s32 CuSetPtzPresetName(CCuVideosourcePresetcfgAddReq &cReq, CCuVideosourcePresetcfgAddRsp &cRsp);

s32 CuDelPtzPresetName(CCuVideosourcePresetcfgDelReq &cReq, CCuVideosourcePresetcfgDelRsp &cRsp);

s32 CuGetCAPSConfig(CCuGetCAPSConfigReq cReq, CCuGetCAPSConfigRsp &cRsp);

s32 CuSetCAPSConfig(CCuSetCAPSConfigReq cReq, CCuSetCAPSConfigRsp &cRsp);

s32 CuQueryCAPSDeviceList(CCuQueryCAPSDeviceListReq cReq, CCuQueryCAPSDeviceListRsp &cRsp);

s32 CuCAPSAddDevice(CCuCAPSAddDeviceReq cReq, CCuCAPSAddDeviceRsp &cRsp);

s32 CuCAPSModifyDevice(CCuCAPSModifyDeviceReq cReq, CCuCAPSModifyDeviceRsp &cRsp);

s32 CuCAPSDelDevice(CCuCAPSDelDeviceReq cReq, CCuCAPSDelDeviceRsp &cRsp);

s32 CuGetOfflineDeviceStat(CCuOffLineDevQryReq cReq, CCuOffLineDevQryRsp &cRsp);

s32 CuUpdateCUProperty(CCuUpdateCUPropertyReq cReq, CCuUpdateCUPropertyRsp &cRsp);

s32 CuGbsGbDevTeleBoot( CCuGbsGbDevTeleBootReq cReq, CCuGbsGbDevTeleBootRsp &cRsp );

s32 CuUserDeviceGroupAdd(CCuUserDeviceGroupAddReq cReq, CCuUserDeviceGroupAddRsp &cRsp);

s32 CuUserDeviceGroupDel(CCuUserDeviceGroupDelReq cReq, CCuUserDeviceGroupDelRsp &cRsp);

s32 CuUserGroupAddDeviceGroup(CCuUserGroupDeviceGroupAddReq cReq, CCuUserGroupDeviceGroupAddRsp &cRsp);

s32 CuUserGroupDelDeviceGroup(CCuUserGroupDeviceGroupDelReq cReq, CCuUserGroupDeviceGroupDelRsp &cRsp);

s32 CuUserSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserSetReq cReq, CCuDeviceAutoAssignToUserSetRsp &cRsp);

s32 CuUserGroupSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupSetReq cReq, CCuDeviceAutoAssignToUserGroupSetRsp &cRsp);

s32 CuUserQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserQryReq cReq, CCuDeviceAutoAssignToUserQryRsp &cRsp);

s32 CuUserGroupQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupQryReq cReq, CCuDeviceAutoAssignToUserGroupQryRsp &cRsp);

s32 CuQueryOperationProgree(CCuTransactionProgressGetReq cReq, CCuTransactionProgressGetRsp &cRsp);

s32 CuGetSuperGBDomainList(CCuGbsGbParentDomainGetReq cReq, CCuGbsGbParentDomainGetRsp &cRsp);

s32 CuPushDevice(CCuGbsGbPDCatalogLoadReq cReq, CCuGbsGbPDCatalogLoadRsp &cRsp);

s32 CuGetUserDataVersion(CCuUserDataVersionGetReq cReq, CCuUserDataVersionGetRsp &cRsp);

s32 CuGetCivilcode(CCuGbsDevGrpCivilCodeGetReq cReq, CCuGbsDevGrpCivilCodeGetRsp &cRsp);

s32 CuGetGbsDevKdid(CCuGbsDevKdidGetReq cReq, CCuGbsDevKdidGetRsp &cRsp);

s32 CuCreateCustomTreeView(CCuDevTreeAddReq cReq, CCuDevTreeAddRsp &cRsp);

s32 CuModifyCustomTreeViewName(CCuDevTreeModReq cReq, CCuDevTreeModRsp &cRsp);

s32 CuDeleteCustomTreeView(CCuDevTreeDelReq cReq, CCuDevTreeDelRsp &cRsp);

s32 CuGetCustomTreeViewList(CCuDevTreeQryReq cReq, CCuDevTreeQryRsp &cRsp);

s32 CuGetDeviceGroupPath(CCuDeviceGrpPathQryReq cReq, CCuDeviceGrpPathQryRsp &cRsp);

s32 CuSearchDevice(CCuDPSSearchReq cReq, CCuDPSSearchRsp &cRsp);

s32 CuGetSearchResult(CCuDPSGetResultReq cReq, CCuDPSGetResultRsp &cRsp);

s32 CuGetSearchResultByGroup(CCuDPSGetResultByGroupReq cReq, CCuDPSGetResultByGroupRsp &cRsp);

s32 CuEndOperation(CCuDPSSearchStopReq cReq, CCuDPSSearchStopRsp &cRsp);

s32 CuGetDeviceStatus(CCuDPSGetDeviceStatusReq cReq, CCuDPSGetDeviceStatusRsp &cRsp);

s32 CuGetDevInfoAndStatus(CCuGetDevInfoAndStatusReq cReq, CCuGetDevInfoAndStatusRsp &cRsp);

s32 CuGetUserDeviceInGroupRecursive(CCuUserDevGrpVidsrcQryReq cReq, CCuUserDevGrpVidsrcQryRsp &cRsp);

s32 CuBatchOperation(CCuBatOptReq cReq, CCuBatOptRsp &cRsp);

s32 CuGetDeviceGroupVideoSrcNum(CCuDevGrpVidsrcTotalnumGetReq cReq, CCuDevGrpVidsrcTotalnumGetRsp &cRsp);

s32 CuGetVSOfflineInfo(CCuVidsrcInfoQryReq cReq, CCuVidsrcInfoQryRsp &cRsp);

s32 CuGetAzimuth(CCuGetAzimuthReq cReq, CCuGetAzimuthRsp &cRsp);

s32 CuGetDeviceGroupOfGBDeviceInfo(CCuDeviceGroupDeviceQry4GbReq &cReq, CCuDeviceGroupDeviceQry4GbRsp &cRsp);

s32 CuDeviceMatchGbid(CCuDeviceMatchGbidReq cReq, CCuDeviceMatchGbidRsp &cRsp);

s32 CuDeviceGrpDevSumNum(CCuDeviceGrpDevSumNumReq cReq, CCuDeviceGrpDevSumNumRsp &cRsp);

s32 CuDeviceGroupDeviceMod4Gb(CDeviceGroupDeviceMod4GbReq cReq, CDeviceGroupDeviceMod4GbRsp &cRsp);

s32 CuDeviceGbidErrGet(CCuDeviceGbidErrGetReq cReq, CCuDeviceGbidErrGetRsp &cRsp);

s32 CuGbsCatalogSelectReportStart(CCuGbsCatalogSelectReportStartReq cReq, CCuGbsCatalogSelectReportStartRsp &cRsp);

s32 CCuDeviceGbinfoQry(CCuDeviceGbinfoQryReq cReq, CCuDeviceGbinfoQryRsp &cRsp);

s32 CCuDevgrpGbinfoQry(CCuDevgrpGbinfoQryReq cReq, CCuDevgrpGbinfoQryRsp &cRsp);

s32 CCuSetPuVidSrcOSD(CCuPuVidSrcOsdSetReq cReq, CCuPuVidSrcOsdSetRsp &cRsp);

s32 CCuGetPuVidSrcOSD(CCuPuVidSrcOsdGetReq cReq, CCuPuVidSrcOsdGetRsp &cRsp);

s32 CuModifyGBDeviceGroup(CCuModifyGBDeviceGroupReq cReq, CCuModifyGBDeviceGroupRsp &cRsp);

s32 CuPuLongOsdGet(CCuPuLongOsdGetReq cReq, CCuPuLongOsdGetRsp &cRsp);

s32 CuPuLongOsdSet(CCuPuLongOsdSetReq cReq, CCuPuLongOsdSetRsp &cRsp);

s32 CuAdjustVideoSrcOrder(CCuAdjustVideoSrcOrderReq cReq, CCuAdjustVideoSrcOrderRsp &cRsp);

s32 CuGetAlarmTypes(CCuGetAlarmTypesReq cReq, CCuGetAlarmTypesRsp &cRsp);

s32 CuPlatformRecordLock(CCuRcsRecordLockOprReq cCuRcsRecordLockOprReq, CCuRcsRecordLockOprRsp &cCuRcsRecordLockOprRsp);

s32 CuPlatformRecordUnLock(CCuRcsRecordUnLockOprReq cCuRcsRecordUnLockOprReq, CCuRcsRecordUnLockOprRsp &cCuRcsRecordUnLockOprRsp);

s32 CuRealMediaSwitchOccurTypeReq(CCuRealMediaSwitchOccurTypeReq cReq, CCuRealMediaSwitchOccurTypeRsp &cRsp);

s32 CuGetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeGetReq cReq, CCuGbsGbdomainParentPrivilegeGetRsp &cRsp);
s32 CuSetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeSetReq cReq, CCuGbsGbdomainParentPrivilegeSetRsp &cRsp);

s32 CuGetGbsOptionConf(CCuGbsOptionConfGetReq cReq, CCuGbsOptionConfGetRsp &cRsp);
s32 CuSetGbsOptionConf(CCuGbsOptionConfSetReq cReq, CCuGbsOptionConfSetRsp &cRsp);
//证书上传下载
s32 CuUploadGbsCertificate(CCuGbsCertificateUploadReq cReq, CCuGbsCertificateUploadRsp &cRsp);
s32 CuGetGbsCertificate(CCuGbsCertificateGetReq cReq, CCuGbsCertificateGetRsp &cRsp);

s32 CuCertificateQry(CCuCertificateQryReq cReq, CCuCertificateQryRsp &cRsp);
s32 CuDeviceSignatrueCtrl(CCuDeviceSignatrueCtrlReq cReq, CCuDeviceSignatrueCtrlRsp &cRsp);
s32 CuDeviceEncryptionCtrl(CCuDeviceEncryptionCtrlReq cReq, CCuDeviceEncryptionCtrlRsp &cRsp);
s32 CuSecurityAuthenticateFirstTime(CCuSecurityAuthorizationFirstReq  &cReq, CCuSecurityAuthorizationFirstRsp  &cRsp);
s32 CuSecurityAuthenticateSecondTime(CCuSecurityAuthorizationSecondReq &cReq, CCuSecurityAuthorizationSecondRsp &cRsp);

#endif


