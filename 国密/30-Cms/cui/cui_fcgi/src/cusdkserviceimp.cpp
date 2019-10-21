/*****************************************************************************
模块名      : cui_fcgi 
文件名      : cusdkserviceimp.cpp
相关文件    : 
文件实现功能: cui_fcgi功能
作者        : 梁  黎
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/09/23  1.0         梁  黎        创建
******************************************************************************/
#include "kdvtype.h"
#include "serviceappimp.h"
#include "../soap/soapcusdkService.h"
#include "../soap/cusdk.nsmap"
//#include "cms/cms_const.h"
#include "cuiprotoconvertor.h"
#include "cbb/zlib/zlib.h"
#include "cms/utility/base64codec.h"
#include "cms/ospsip/ospsip.h"
#include "cms/utility/md5.h"
#include "alarmtypecommon.h"
extern string g_strDomainName;

#define CHECK_PARAM_VALID(_set, _invalid_param) do \
{ \
    if ( _set.count(_invalid_param) > 0 ) \
    { \
    rsp.errorCode = ERR_CUI_INVALID_PARAM; \
    return SOAP_OK; \
    } \
} while (0)


cusdk__MoveOperation GetMoveOp( int op )
{
    switch( op )
    {
    case cusdk__MoveInvalid: // 无效值
        //no break here
    default:
        return cusdk__MoveInvalid;
        break;

    case cusdk__InDevice_MoveAfter: //设备内移动
    case cusdk__InDevice_MoveTop: //设备内置顶
    case cusdk__InDevice_MoveBottom://设备内置底
    case cusdk__InGroup_MoveAfter: //分组内移动
    case cusdk__InGroup_MoveTop: //分组内置顶
    case cusdk__InGroup_MoveBottom: //分组内置底
        //no breaks here
        return cusdk__MoveOperation( op );
        break;
    }
}

//notice cusdk__MoveOperation should be valid
ENOrderType MoveOperationToOrderType( cusdk__MoveOperation op)
{
    switch( op )
    {
    case cusdk__InDevice_MoveAfter: //设备内移动
        return ENORDER_TYPE_INNER_DEVICE_MOVE;
        break;

    case cusdk__InDevice_MoveTop: //设备内置顶
        return ENORDER_TYPE_INNER_DEVICE_TOP;
        break;

    case cusdk__InDevice_MoveBottom://设备内置底
        return ENORDER_TYPE_INNER_DEVICE_BOTTOM;
        break;

    case cusdk__InGroup_MoveAfter: //分组内移动
        return ENORDER_TYPE_INNER_DEVGRP_MOVE;
        break;

    case cusdk__InGroup_MoveTop: //分组内置顶
        return ENORDER_TYPE_INNER_DEVGRP_TOP;
        break;

    case cusdk__InGroup_MoveBottom: //分组内置底
        return ENORDER_TYPE_INNER_DEVGRP_BOTTOM;
        break;
    }
}

/*====================================================================
函数名：DetectServer
功能：1、检查平台是否可用 2、获取平台域名
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
/// Web service operation 'DetectServer' (returns error code or SOAP_OK)
int cusdkService::DetectServer( cusdk__DetectServerReq detectReq, cusdk__DetectServerRsp &detectRsp )
{
	detectRsp.errorCode = 0;
	detectRsp.domainName = g_strDomainName;

    return SOAP_OK;
}

/*====================================================================
函数名：login
功能：
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
/// Web service operation 'login' (returns error code or SOAP_OK)
int cusdkService::Login(cusdk__LoginReq loginReq, cusdk__LoginRsp &loginRsp)
{
	//如果客户端已经用"SM3_Base64"加密过，则透传否则按老的md5加密
	std::string strEncPassword = loginReq.password;
	if (loginReq.passwordAlgorithm != "SM3_Base64")
	{
		MD5(loginReq.password, strEncPassword);
	}
    CCuiCuLoginReq cLoginReq;
    cLoginReq.SetUserName(loginReq.userName);
    cLoginReq.SetPassword(strEncPassword);
    cLoginReq.SetVersion(loginReq.cusdkVersion);

    cLoginReq.SetClientIp(getenv("REMOTE_ADDR"));
    cLoginReq.SetServiceIp(getenv("SERVER_ADDR"));
    cLoginReq.SetHostIp(getenv("HTTP_HOST"));
    cLoginReq.SetIsSupportNA(loginReq.isSupportNA);
    cLoginReq.SetClientType(loginReq.clientType);
	if(loginReq.webCUClientAddr.length()>0)
	{
		cLoginReq.SetWebCuIp(loginReq.webCUClientAddr);
	}
	

    CCuiCuLoginRsp cLoginRsp;
    CuLogin(cLoginReq, cLoginRsp);

    loginRsp.errorCode = cLoginRsp.GetErrorCode();
    loginRsp.session = cLoginRsp.GetSession();
    loginRsp.cmsVersion = cLoginRsp.GetCmsVer();
    loginRsp.cuVersion = cLoginRsp.GetCuVersion();
    loginRsp.cuUpdateUrl = cLoginRsp.GetUpdateUrl(); 
    loginRsp.stunInfo.ip = cLoginRsp.GetStunServerInfo().GetIp();
    loginRsp.stunInfo.port = cLoginRsp.GetStunServerInfo().GetPort();
    loginRsp.serverDataVersion = cLoginRsp.GetDataVersion();
	loginRsp.userLockRemainTime = cLoginRsp.GetUserLockRemainTime();
	loginRsp.passwordRemainTime = cLoginRsp.GetPasswordRemainTime();
	loginRsp.isSecureEnabled = cLoginRsp.IsSecureEnable();

    return SOAP_OK;
}

/*====================================================================
函数名：logout
功能：
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
/// Web service operation 'logout' (returns error code or SOAP_OK)
int cusdkService::Logout(cusdk__LogoutReq logoutReq, cusdk__LogoutRsp &logoutRsp)
{
    CCuLogoutReq cCuLogoutReq;
    cCuLogoutReq.SetSession(logoutReq.session);

    CCuLogoutRsp cCuLogoutRsp;
    CuLogout(cCuLogoutReq, cCuLogoutRsp);

    logoutRsp.session = logoutReq.session;
    logoutRsp.errorCode = cCuLogoutRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : SubscribePuStatus
功能        : 订阅某种类型的PU状态
算法实现    : 批量操作，如果有一个失败就认为是失败，返回部分操作失败的列表
如果都成功，就认为成功
这里返回的是未能订阅的列表
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/7/             liangli
====================================================================*/
/// Web service operation 'SubscribePuStatus' (returns error code or SOAP_OK)
//todo 当前默认发给cmu 后续需要修改，添加发送目标
int cusdkService::SubscribeNotify( cusdk__SubscribeNotifyReq subscribeNotifyReq, cusdk__SubscribeNotifyRsp &subscribeNotifyRsp )
{
    CCuSubscribeNotifyReq cCuSubscribeNotifyReq;
    cCuSubscribeNotifyReq.SetSession(subscribeNotifyReq.session);
    ConvertSubscriptionListFromCUToCUI(subscribeNotifyReq.subscriptionList, cCuSubscribeNotifyReq.GetSubscribeList());

    CCuSubscribeNotifyRsp cCuSubscribeNotifyRsp;
    CuSubscribePuStatus(cCuSubscribeNotifyReq, cCuSubscribeNotifyRsp);
    subscribeNotifyRsp.errorCode = cCuSubscribeNotifyRsp.GetErrorCode();
    subscribeNotifyRsp.session = subscribeNotifyReq.session;
    ConvertSubscriptionListFromCUIToCU(cCuSubscribeNotifyRsp.GetFailedSubscribeList(), subscribeNotifyRsp.failedList);
    return SOAP_OK;
}

/*====================================================================
函数名      : UnsubscribePuStatus
功能        : 取消某种状态的PU订阅
算法实现    : 批量操作，如果有一个失败就认为是失败，返回部分操作失败的列表
如果都成功，就认为成功
这里返回的是未能取消订阅的列表
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/7/             liangli
====================================================================*/
/// Web service operation 'UnsubscribePuStatus' (returns error code or SOAP_OK)
//todo 当前默认发给cmu 后续需要修改，添加发送目标
int cusdkService::UnsubscribeNotify( cusdk__UnsubscribeNotifyReq unsubscribeNotifyReq, cusdk__UnsubscribeNotifyRsp &unsubscribeNotifyRsp )
{
    CCuUnSubscribeNotifyReq cCuUnSubscribeNotifyReq;
    cCuUnSubscribeNotifyReq.SetSession(unsubscribeNotifyReq.session);
    ConvertSubscriptionListFromCUToCUI(unsubscribeNotifyReq.subscriptionList, cCuUnSubscribeNotifyReq.GetUnSubscribeList());

    CCuUnSubscribeNotifyRsp cCuUnSubscribeNotifyRsp;
    CuUnsubscribePuStatus(cCuUnSubscribeNotifyReq, cCuUnSubscribeNotifyRsp);
    unsubscribeNotifyRsp.errorCode = cCuUnSubscribeNotifyRsp.GetErrorCode();
    unsubscribeNotifyRsp.session = unsubscribeNotifyReq.session;
    ConvertSubscriptionListFromCUIToCU(cCuUnSubscribeNotifyRsp.GetFailedUnSubscribeList(), unsubscribeNotifyRsp.failedList);
    return SOAP_OK;
}


/*====================================================================
函数名：GetNotify
功能：作用有二：1，CU保活；2，获取平台推送通知消息
算法实现：
引用全局变量：
输入参数说明：std::string &session  用户的SessionID
kdcusdk__CNotifyList &rsp  返回的推送消息，如果没有推送消息则为空结构体
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
/// Web service operation 'GetNotify' (returns error code or SOAP_OK)
int cusdkService::GetNotify(cusdk__GetNotifyReq req, cusdk__GetNotifyRsp &rsp)
{
    CGetNotifyReq cGetNotifyReq;
    cGetNotifyReq.SetSession(req.session);
    cGetNotifyReq.SetTimeoutSeconds(req.timeoutMSec/1000+(req.timeoutMSec%1000>0? 1:0));

    CGetNotifyRsp cGetNotifyRsp;
    CuGetNotify(cGetNotifyReq, cGetNotifyRsp);

    rsp.session = req.session;
    //将平台的缓存通知转换为cusdk的通知
    ConvertCGetNotifyRspTocusdk__GetNotifyRsp(cGetNotifyRsp, rsp);
    return SOAP_OK;
}

/*====================================================================
函数名      : GetCUIList
功能        : 获取本域所有在线CUI列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/1/7/             liangli
====================================================================*/
/// Web service operation 'GetCUIList' (returns error code or SOAP_OK)
int cusdkService::GetCUIList( cusdk__GetCUIListReq req, cusdk__GetCUIListRsp &rsp )
{
    CCuGetCuiListReq cCuGetCuiListReq;
    cCuGetCuiListReq.SetSession(req.session);

    CCuGetCuiListRsp cCuGetCuiListRsp;
    CuGetCuiList(cCuGetCuiListReq, cCuGetCuiListRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetCuiListRsp.GetErrorCode();
    //将CUI地址列表转化为URL
    CIpAddrList &cCuiAddrList = cCuGetCuiListRsp.GetIpAddrList();
    CIpAddrList::iterator pItem = cCuiAddrList.begin();
    while ( pItem != cCuiAddrList.end() )
    {
        s8 szURL[NAME_MAX_LEN] = {0};
        s8 szPort[NAME_MAX_LEN] = {0};
//         if ( 80 != pItem->GetNetPort() )
//         {
            sprintf(szPort, ":%d", pItem->GetNetPort());
//        }
        sprintf(szURL, "http://%s%s/cui/cui.fcgi", pItem->GetNetIp().c_str(), szPort);
        rsp.cuiList.push_back(szURL);

        pItem++;
    }

    return SOAP_OK;
}

/*====================================================================
函数名      : PtzCtrl
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
/// Web service operation 'PtzCtrl' (returns error code or SOAP_OK)
int cusdkService::PtzCtrl( cusdk__PtzCtrlReq req, cusdk__PtzCtrlRsp &rsp )
{
    CPuPtzCtrlReq cCuPtzCtrlReq;
    cCuPtzCtrlReq.SetSession(req.session);
    TChannel tDevChn;
    tDevChn.SetDevUri(req.devId);
    tDevChn.SetChnNO(req.videoSrcId);
    cCuPtzCtrlReq.SetDevChn(tDevChn);
    TCamCtrl tCamCtrl;
    tCamCtrl.SetCommandId(req.ptzCmd);
    tCamCtrl.SetParams(req.ptzParam);
    cCuPtzCtrlReq.SetCamCtrl(tCamCtrl);

    CPuPtzCtrlRsp cCuPtzCtrlRsp;
    CuPtzCtrl(cCuPtzCtrlReq, cCuPtzCtrlRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuPtzCtrlRsp.GetErrorCode();

	if ( rsp.errorCode != CMS_SUCCESS )
	{
		rsp.userName = cCuPtzCtrlRsp.GetUsername();
		rsp.ptzLevel = cCuPtzCtrlRsp.GetPriPtzLevel();

		CCMSTime cmsBeginTime(cCuPtzCtrlRsp.GetBeginTime());
		rsp.beginTime = cmsBeginTime.GetTime();

		CCMSTime cmsEndTime(cCuPtzCtrlRsp.GetEndTime());
		rsp.endTime = cmsEndTime.GetTime();

		rsp.errorDesc = cCuPtzCtrlRsp.GetErrorDesc();
	}

    return SOAP_OK;
}

/*====================================================================
函数名      : PtzLock
功能        : 锁定前端摄像头
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'PtzLock' (returns error code or SOAP_OK)
int cusdkService::PtzLock( cusdk__PtzLockReq req, cusdk__PtzLockRsp &rsp )
{
	CCuPtzLockAddReq cCuPtzLockAddReq;
    cCuPtzLockAddReq.SetSession(req.session);

	CPtzLockInfo ptzLockInfo;
	ptzLockInfo.SetDeviceId(req.devId);
	ptzLockInfo.SetVideoSrcId(req.videoSrcId);
	ptzLockInfo.SetPassword(req.password);
	
	CCMSTime beginTime(req.beginTime);
	ptzLockInfo.SetBeginTime(beginTime.GetXSDDateTime());

	CCMSTime addEndTime(req.endTime);
	ptzLockInfo.SetEndTime(addEndTime.GetXSDDateTime());

	cCuPtzLockAddReq.SetCPtzLockInfo(ptzLockInfo);

	CCuPtzLockAddRsp cCuPtzLockAddRsp;
    CuPtzLock(cCuPtzLockAddReq, cCuPtzLockAddRsp);

	rsp.session   = req.session;
    rsp.errorCode = cCuPtzLockAddRsp.GetErrorCode();

	if (rsp.errorCode != CMS_SUCCESS)
	{
		rsp.userName = cCuPtzLockAddRsp.GetUsername();
		rsp.ptzLevel = cCuPtzLockAddRsp.GetPriPtzLevel();

		CCMSTime beginTime(cCuPtzLockAddRsp.GetBeginTime());
		rsp.beginTime = beginTime.GetTime();

		CCMSTime endTime(cCuPtzLockAddRsp.GetEndTime());
		rsp.endTime = endTime.GetTime();
	}

    return SOAP_OK;
}

/*====================================================================
函数名      : PtzUnlock
功能        : 对前端摄像头解锁
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'PtzUnlock' (returns error code or SOAP_OK)
int cusdkService::PtzUnlock( cusdk__PtzUnlockReq req, cusdk__PtzUnlockRsp &rsp )
{
	CCuPtzLockDelReq cCuPtzLockDelReq;
	cCuPtzLockDelReq.SetSession(req.session);
	cCuPtzLockDelReq.SetDeviceId(req.devId);
	cCuPtzLockDelReq.SetVideoSrcId(req.videoSrcId);
	cCuPtzLockDelReq.SetOperateType(PTZLOCK_UNLOCK);
	cCuPtzLockDelReq.SetPassword(req.password);

	CCuPtzLockDelRsp cCuPtzLockDelRsp;
    CuPtzUnlock(cCuPtzLockDelReq, cCuPtzLockDelRsp);

    rsp.session   = req.session;
	rsp.errorCode = cCuPtzLockDelRsp.GetErrorCode();
	if (rsp.errorCode != CMS_SUCCESS)
	{
		rsp.userName = cCuPtzLockDelRsp.GetUsername();
		rsp.ptzLevel = cCuPtzLockDelRsp.GetPriPtzLevel();

		CCMSTime beginTime(cCuPtzLockDelRsp.GetBeginTime());
		rsp.beginTime = beginTime.GetTime();

		CCMSTime endTime(cCuPtzLockDelRsp.GetEndTime());
		rsp.endTime = endTime.GetTime();
	}

    return SOAP_OK;
}

int cusdkService::PtzUnlockAll(cusdk__PtzUnlockAllReq req, cusdk__PtzUnlockAllRsp& rsp)
{
	CCuPtzLockDelReq cCuPtzLockDelReq;
	cCuPtzLockDelReq.SetSession(req.session);
	cCuPtzLockDelReq.SetOperateType(PTZLOCK_UNLOCKALL);

	CCuPtzLockDelRsp cCuPtzLockDelRsp;
	CuPtzUnlock(cCuPtzLockDelReq, cCuPtzLockDelRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuPtzLockDelRsp.GetErrorCode();

	return SOAP_OK;
}
/*====================================================================
函数名      : SendTransData
功能        : 发送透明串口数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/1/               liangli
2018/3/7                Royan         新增cusdk__TransChnUserDef处理
====================================================================*/
/// Web service operation 'SendTransData' (returns error code or SOAP_OK)
int cusdkService::SendTransData( cusdk__SendTransDataReq req, cusdk__SendTransDataRsp &rsp )
{
    CCuPuTransparentOperateReq cOptReq;
    cOptReq.SetSession(req.session);

	CPuTransparentQryReq cQueryReq;
	cQueryReq.SetSession(req.session);

	TChannel tPuChn;
	tPuChn.SetDevUri(req.devId);
	tPuChn.SetChnNO(req.channelId);

    switch(req.transChnType)
    {
    case cusdk__TransChnCOMPort:
        {
			cOptReq.SetData(req.base64TransDataReq);
			cOptReq.SetDevChn(tPuChn);
			cOptReq.SetMsgType(req.dataMsgType);
            cOptReq.SetSrvName(PU_TRANS_OPR_SEND_COMDATA);

			CCuPuTransparentOperateRsp cOptRsp;
			CuSendTransData(cOptReq, cOptRsp);

			rsp.session = req.session;
			rsp.errorCode = cOptRsp.GetErrorCode();
			rsp.base64TransDataRsp = cOptRsp.GetData();
			rsp.dataMsgType = cOptRsp.GetMsgType();
        }
        break;
    case cusdk__TransChnCameraTaskConfig:
        {
			cOptReq.SetData(req.base64TransDataReq);
			cOptReq.SetDevChn(tPuChn);
			cOptReq.SetMsgType(req.dataMsgType);
            cOptReq.SetSrvName(PU_TRANS_OPR_EXT_PTZ_CTRL);

			CCuPuTransparentOperateRsp cOptRsp;
			CuSendTransData(cOptReq, cOptRsp);

			rsp.session = req.session;
			rsp.errorCode = cOptRsp.GetErrorCode();
			rsp.base64TransDataRsp = cOptRsp.GetData();
			rsp.dataMsgType = cOptRsp.GetMsgType();

			if ( rsp.errorCode != CMS_SUCCESS )
			{
				rsp.userName = cOptRsp.GetUsername();
				rsp.ptzLevel = cOptRsp.GetPriPtzLevel();

				CCMSTime cmsBeginTime(cOptRsp.GetBeginTime());
				rsp.beginTime = cmsBeginTime.GetTime();

				CCMSTime cmsEndTime(cOptRsp.GetEndTime());
				rsp.endTime = cmsEndTime.GetTime();

				rsp.errorDesc = cOptRsp.GetErrorDesc();
			}
        }
        break;
	case cusdk__TransChnExtData:
		{
			cQueryReq.SetData(req.base64TransDataReq);
			cQueryReq.SetDevChn(tPuChn);
			cQueryReq.SetMsgType(req.dataMsgType);
			cQueryReq.SetSrvName(PU_TRANS_OPR_SEND_EXT_DATA);

			CCuPuTransparentQryRsp cQueryRsp;
			CuQueryTransData(cQueryReq,cQueryRsp);

			rsp.session = req.session;
			rsp.errorCode = cQueryRsp.GetErrorCode();
			rsp.base64TransDataRsp = cQueryRsp.GetData();
			rsp.dataMsgType = cQueryRsp.GetMsgType();
			if (0==cQueryRsp.GetTotalEntryNum())
			{
				rsp.hasNotify = false;
			}
			else
			{
				rsp.hasNotify = true;
			}
		}
		break;
	case cusdk__TransChnUserDef:
		{
			cOptReq.SetData(req.base64TransDataReq);
			cOptReq.SetDevChn(tPuChn);
			cOptReq.SetMsgType(req.dataMsgType);
			cOptReq.SetSrvName(req.dataMsgTypeUserDef);

			CCuPuTransparentOperateRsp cOptRsp;
			CuSendTransData(cOptReq, cOptRsp);

			rsp.session = req.session;
			rsp.errorCode = cOptRsp.GetErrorCode();
			rsp.base64TransDataRsp = cOptRsp.GetData();
			rsp.dataMsgType = cOptRsp.GetMsgType();
		}
		break;
    default:
        {

        }
        break;
    }

    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserInfo
功能        : 获取当前登录CU的用户信息, 仅用于CU自身登录用户
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/2/             liangli
====================================================================*/
/// Web service operation 'GetUserInfo' (returns error code or SOAP_OK)
int cusdkService::GetUserInfo( cusdk__GetUserInfoReq req, cusdk__GetUserInfoRsp &rsp )
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(req.session);
    CUserInfoQueryCondition cCondition;
    string strUserName = GetNameWithoutDomainFromURI(req.userName); //查询CU本身信息, ligang@nanjing 转换为ligang
    cCondition.SetUserName(strUserName);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo

    rsp.session   = req.session;
    rsp.errorCode = cCuQueryUserRsp.GetErrorCode();
    if ( cCuQueryUserRsp.GetActualResultNum() > 0 )
    {
        ConvertUserInfoFromCuiToCu(cCuQueryUserRsp.GetUserInfo().at(0), rsp.userInfo);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserList
功能        : 获取当前登录CU的子用户信息，用于用户管理
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
/// Web service operation 'GetUserList' (returns error code or SOAP_OK)
int cusdkService::GetUserList( cusdk__GetUserListReq req, cusdk__GetUserListRsp &rsp )
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(req.session);
    cCuQueryUserReq.SetStartEntryIndex(req.beginIndex);
    cCuQueryUserReq.SetExpectEntryNum(req.num);
    CUserInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo

    rsp.session   = req.session;
    rsp.errorCode = cCuQueryUserRsp.GetErrorCode();
    if ( cCuQueryUserRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuQueryUserRsp.GetTotalEntryNum();
        vector<CCuUserInfo>::const_iterator pUserInfoItem = cCuQueryUserRsp.GetUserInfo().begin();
        while ( pUserInfoItem != cCuQueryUserRsp.GetUserInfo().end() )
        {
            cusdk__UserInfo userInfo;
            ConvertUserInfoFromCuiToCu(*pUserInfoItem, userInfo);
            rsp.userInfoList.push_back(userInfo);
            pUserInfoItem++;
        }
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserGroupInfo
功能        : 获取当前登录CU的用户组信息, 仅用于CU自身登录用户
算法实现    : CU可通过此接口获取本域的DomainUUID
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/5/             liangli
====================================================================*/
/// Web service operation 'GetUserGroupInfo' (returns error code or SOAP_OK)
int cusdkService::GetUserGroupInfo( cusdk__GetUserGroupInfoReq req, cusdk__GetUserGroupInfoRsp &rsp )
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(req.session);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetGroupNO(req.userGroupId);
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuQueryUserGroupRsp.GetErrorCode();
    if ( cCuQueryUserGroupRsp.GetActualResultNum() > 0 )
    {
        ConvertUserGroupInfoFromCuiToCu(cCuQueryUserGroupRsp.GetUserGroupInfo().at(0), rsp.userGroupInfo);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserGroupList
功能        : 获取当前登录CU的拥有管理权限的子用户组列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
/// Web service operation 'GetUserGroupList' (returns error code or SOAP_OK)
int cusdkService::GetUserGroupList( cusdk__GetUserGroupListReq req, cusdk__GetUserGroupListRsp &rsp )
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(req.session);
    cCuQueryUserGroupReq.SetStartEntryIndex(req.beginIndex);
    cCuQueryUserGroupReq.SetExpectEntryNum(req.num);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuQueryUserGroupRsp.GetErrorCode();
    if ( cCuQueryUserGroupRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuQueryUserGroupRsp.GetTotalEntryNum();
        vector<CUserGroupInfo>::const_iterator pUserGroupInfoItem = cCuQueryUserGroupRsp.GetUserGroupInfo().begin();
        while ( pUserGroupInfoItem != cCuQueryUserGroupRsp.GetUserGroupInfo().end())
        {
            cusdk__UserGroup userGroup;
            ConvertUserGroupInfoFromCuiToCu(*pUserGroupInfoItem, userGroup);
            rsp.userGroupList.push_back(userGroup);
            pUserGroupInfoItem++;
        }

    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserData
功能        : 获取用户自定义数据
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/5/             liangli
====================================================================*/
/// Web service operation 'GetUserData' (returns error code or SOAP_OK)
int cusdkService::GetUserData( cusdk__GetUserDataReq req, cusdk__GetUserDataRsp &rsp )
{
    CCuUserCustomDataQryReq cReq;
    cReq.SetSession(req.session);
    cReq.GetCondition().SetDataId(req.key);
    cReq.GetCondition().SetDataType(req.dataType);
    cReq.GetCondition().SetFragNo(req.fragNo);

    CCuUserCustomDataQryRsp cRsp;
    CuGetUserData(cReq, cRsp);

    rsp.session   = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    rsp.allFragNum = cRsp.GetTotalEntryNum();
    if (!cRsp.GetInfos().empty())
    {
        const TUserCustomDataInfo &tInfo = cRsp.GetInfos().front();
        rsp.dataType = tInfo.dataType;
        rsp.fragNo = tInfo.currentFragNo;
        rsp.key = tInfo.dataId;
        rsp.value = tInfo.fragData;
    }

    return SOAP_OK;
}

/*====================================================================
函数名      : SetUserData
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
/// Web service operation 'SetUserData' (returns error code or SOAP_OK)
int cusdkService::SetUserData( cusdk__SetUserDataReq req, cusdk__SetUserDataRsp &rsp )
{
    CCuUserCustomDataAddReq cReq;
    cReq.SetSession(req.session);
    cReq.SetDataTag(req.session);
    cReq.SetLastFrag(req.fragNo + 1 == req.allFragNum);
    cReq.GetInfo().currentFragNo = req.fragNo;
    cReq.GetInfo().dataId = req.key;
    cReq.GetInfo().dataType = req.dataType;
    cReq.GetInfo().fragData = req.value;

    CCuUserCustomDataAddRsp cRsp;
    CuSetUserData(cReq, cRsp);

    rsp.session   = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::DelUserData( cusdk__DelUserDataReq req, cusdk__DelUserDataRsp &rsp )
{
    CCuUserCustomDataDelReq cReq;
    cReq.SetSession(req.session);
    cReq.GetKey().SetDataId(req.key);
    cReq.GetKey().SetDataType(req.dataType);

    CCuUserCustomDataDelRsp cRsp;
    CuDelUserData(cReq, cRsp);

    rsp.session   = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    return SOAP_OK;
}


int cusdkService::StartExportDeviceData( cusdk__StartExportDeviceDataReq req, cusdk__StartExportDeviceDataRsp &rsp )
{
	// 200W平台废弃，会导致3A断链
    //CCuFileExportReq cCuFileExportReq; 
    //cCuFileExportReq.SetSession(req.session);
    //cCuFileExportReq.SetSrvName(FILE_EXPORT_CUSER_DEVICEDATA);
    //cCuFileExportReq.SetFileSenderUri(DEV_TYPE_3AS);
    //cCuFileExportReq.SetFileReceiverUri(DEV_TYPE_CU);

    //CCuFileExportRsp cCuFileExportRsp;
    //CuStartExport(cCuFileExportReq, cCuFileExportRsp);

    rsp.session = req.session;
	rsp.errorCode = ERR_CMU_NOT_SUPPORT;
	//rsp.errorCode = cCuFileExportRsp.GetErrorCode();
	//rsp.exportFileUrl = cCuFileExportRsp.GetFileURL();

    return SOAP_OK;
}


int cusdkService::StopExportDeviceData( cusdk__StopExportDeviceDataReq req, cusdk__StopExportDeviceDataRsp &rsp )
{
    //CCuStopExportReq cCuStopExportReq;
    //cCuStopExportReq.SetSession(req.session);
    //CCuStopExportRsp cCuStopExportRsp;
    //CuStopExport(cCuStopExportReq, cCuStopExportRsp);
	rsp.session = req.session;
	rsp.errorCode = ERR_CMU_NOT_SUPPORT;
    //rsp.errorCode = cCuStopExportRsp.GetErrorCode();
    return SOAP_OK;
}


/*====================================================================
函数名      : GetUserDevGroupByGroup
功能        : 根据父设备组获取子一级设备组，一般CU构建设备树时用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/6/             liangli
====================================================================*/
/// Web service operation 'GetUserDevGroupByGroup' (returns error code or SOAP_OK)
int cusdkService::GetUserDevGroupByGroup( cusdk__GetUserDevGroupByGroupReq req, cusdk__GetUserDevGroupByGroupRsp &rsp )
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(req.session);
    cCuGetUserDevGroupReq.SetExpectEntryNum(req.num);
    cCuGetUserDevGroupReq.SetStartEntryIndex(req.beginIndex);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);
    cCondition.SetDeviceParentGroupID(req.parentDevGroupId);
	cCondition.SetDevTreeId(req.devTreeId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevGroupRsp.GetErrorCode();

	if ( rsp.errorCode ==  0)
	{
		if (cCuGetUserDevGroupRsp.GetActualResultNum() > 0)
		{
			ConvertUserDeviceGroupInfoFromCUIToCu(cCuGetUserDevGroupRsp.GetUserDevGroupInfo(), rsp.devGroupList);
		}
		rsp.totalNum = cCuGetUserDevGroupRsp.GetTotalEntryNum();
	}
	else
	{
		rsp.totalNum = 0;
	}
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserDevGroup
功能        : 获取以当前CU所在域为根节点的所有设备组，一般二次开发会用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDevGroup' (returns error code or SOAP_OK)
int cusdkService::GetUserDevGroup( cusdk__GetUserDevGroupReq req, cusdk__GetUserDevGroupRsp &rsp )
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(req.session);
    cCuGetUserDevGroupReq.SetExpectEntryNum(req.num);
    cCuGetUserDevGroupReq.SetStartEntryIndex(req.beginIndex);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevGroupRsp.GetErrorCode();
    if ( cCuGetUserDevGroupRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetUserDevGroupRsp.GetTotalEntryNum();
        ConvertUserDeviceGroupInfoFromCUIToCu(cCuGetUserDevGroupRsp.GetUserDevGroupInfo(), rsp.devGroupList);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetDomainDevGroup
功能        : 获取本域的所有设备组，一般二次开发会用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/9/             liangli
====================================================================*/
/// Web service operation 'GetDomainDevGroup' (returns error code or SOAP_OK)
int cusdkService::GetDomainDevGroup( cusdk__GetDomainDevGroupReq req, cusdk__GetDomainDevGroupRsp &rsp )
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(req.session);
    cCuGetUserDevGroupReq.SetExpectEntryNum(req.num);
    cCuGetUserDevGroupReq.SetStartEntryIndex(req.beginIndex);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);
    cCondition.SetDomainUUID(req.domainId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevGroupRsp.GetErrorCode();
    if ( cCuGetUserDevGroupRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetUserDevGroupRsp.GetTotalEntryNum();
        ConvertUserDeviceGroupInfoFromCUIToCu(cCuGetUserDevGroupRsp.GetUserDevGroupInfo(), rsp.devGroupList);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserDevByGroup
功能        : 根据设备组获取用户设备，一般CU用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/6/             liangli
====================================================================*/
/// Web service operation 'GetUserDevByGroup' (returns error code or SOAP_OK)
int cusdkService::GetUserDevByGroup( cusdk__GetUserDevByGroupReq req, cusdk__GetUserDevByGroupRsp &rsp )
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(req.session);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(req.beginIndex);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(req.num);
    cCuGetUserDevFullInfoReq.SetCuserId(req.userId);
	cCuGetUserDevFullInfoReq.SetQueryHint(req.beginQueryHint);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDevicegroupId(req.devGroupId);
	cCondition.SetDevTreeId(req.devTreeId);
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevFullInfoRsp.GetErrorCode();
    if ( cCuGetUserDevFullInfoRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetUserDevFullInfoRsp.GetTotalEntryNum();
        ConvertDevicefullinfoFromCuiToCu(cCuGetUserDevFullInfoRsp.GetUserDevFullInfo(), rsp.devList);
		rsp.nextQueryHint = cCuGetUserDevFullInfoRsp.GetQueryHint();
    }

    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserDev
功能        : 获取所有用户设备，一般二次开发用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDev' (returns error code or SOAP_OK)
int cusdkService::GetUserDev( cusdk__GetUserDevReq req, cusdk__GetUserDevRsp &rsp )
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(req.session);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(req.beginIndex);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(req.num);
    cCuGetUserDevFullInfoReq.SetCuserId(req.userId);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevFullInfoRsp.GetErrorCode();
    if ( cCuGetUserDevFullInfoRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetUserDevFullInfoRsp.GetTotalEntryNum();
        ConvertDevicefullinfoFromCuiToCu(cCuGetUserDevFullInfoRsp.GetUserDevFullInfo(), rsp.devList);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetUserDevByDomain
功能        : 根据当前域获取用户设备，一般二次开发用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetUserDevByDomain' (returns error code or SOAP_OK)
int cusdkService::GetUserDevByDomain( cusdk__GetDevByDomainReq req, cusdk__GetDevByDomainRsp &rsp )
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(req.session);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(req.beginIndex);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(req.num);
    cCuGetUserDevFullInfoReq.SetCuserId(req.userId);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDomainId(req.domainId);
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetUserDevFullInfoRsp.GetErrorCode();
    if ( cCuGetUserDevFullInfoRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetUserDevFullInfoRsp.GetTotalEntryNum();
        ConvertDevicefullinfoFromCuiToCu(cCuGetUserDevFullInfoRsp.GetUserDevFullInfo(), rsp.devList);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : GetDeviceGroupDeviceRelations
功能        : 获取设备与设备组的对应关系，一般二次开发用到此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/10/             liangli
====================================================================*/
/// Web service operation 'GetDeviceGroupDeviceRelations' (returns error code or SOAP_OK)
int cusdkService::GetDeviceGroupDeviceRelations( cusdk__GetDeviceGroupDeviceRelationsReq req, cusdk__GetDeviceGroupDeviceRelationsRsp &rsp )
{
    CCuQryDevGroupOfDevInfoReq cCuQryDevGroupOfDevInfoReq;
    cCuQryDevGroupOfDevInfoReq.SetSession(req.session);
    cCuQryDevGroupOfDevInfoReq.SetExpectEntryNum(req.num);
    cCuQryDevGroupOfDevInfoReq.SetStartEntryIndex(req.beginIndex);
    CDeviceGroupDeviceInfoQueryCondition cCondition;
    cCondition.SetCuserID(req.userId);

    CCuQryDevGroupOfDevInfoRsp cCuQryDevGroupOfDevInfoRsp;
    CuGetDeviceGroupOfDeviceInfo(cCuQryDevGroupOfDevInfoReq, cCuQryDevGroupOfDevInfoRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuQryDevGroupOfDevInfoRsp.GetErrorCode();
    if ( cCuQryDevGroupOfDevInfoRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuQryDevGroupOfDevInfoRsp.GetTotalEntryNum();
        ConvertDevGroupDevInfoFromCuiToCu(cCuQryDevGroupOfDevInfoRsp.GetDeviceGroupDeviceInfo(), rsp.devGroupRelationList);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : CreateDeviceGroup
功能        : 创建设备组，CU和二次开发都会用到此功能
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'CreateDeviceGroup' (returns error code or SOAP_OK)
int cusdkService::CreateDeviceGroup( cusdk__CreateDeviceGroupReq req, cusdk__CreateDeviceGroupRsp &rsp )
{
    CCuCreateDeviceGroupReq cCuCreateDeviceGroupReq;
    cCuCreateDeviceGroupReq.SetSession(req.session);

    CDeviceGroupInfo cDevGrpInfo;

    ConvertDevGrpInfoBaseFromCuToCui( &req.deviceGroupInfo, &cDevGrpInfo );

    cCuCreateDeviceGroupReq.SetDeviceGroupInfo(cDevGrpInfo);

	// cusdk接口没有新增
	//cCuCreateDeviceGroupReq.SetCanBeUsed4Gb(req.canUsedForGBPush);

    CCuCreateDeviceGroupRsp cCuCreateDeviceGroupRsp;
    CuCreateDeviceGroup(cCuCreateDeviceGroupReq, cCuCreateDeviceGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuCreateDeviceGroupRsp.GetErrorCode();
    rsp.deviceGroupId = cCuCreateDeviceGroupRsp.GetDeviceGroupInfoKey().GetDeviceGroupID();
    return SOAP_OK;
}

/*====================================================================
函数名      : DelDeviceGroup
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
/// Web service operation 'DelDeviceGroup' (returns error code or SOAP_OK)
int cusdkService::DelDeviceGroup( cusdk__DelDeviceGroupReq req, cusdk__DelDeviceGroupRsp &rsp )
{
    CCuDeleteDeviceGroupReq cCuDeleteDeviceGroupReq;
    cCuDeleteDeviceGroupReq.SetSession(req.session);
    CDeviceGroupInfoKey cDevGroupKey;
    cDevGroupKey.SetDeviceGroupID(req.deviceGroupId);
	cDevGroupKey.SetDevtreeId(req.devTreeId);
    cCuDeleteDeviceGroupReq.SetDeviceGroupInfoKey(cDevGroupKey);

    CCuDeleteDeviceGroupRsp cCuDeleteDeviceGroupRsp;
    CuDeleteDeviceGroup(cCuDeleteDeviceGroupReq, cCuDeleteDeviceGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuDeleteDeviceGroupRsp.GetErrorCode();
    rsp.conflictedNames = cCuDeleteDeviceGroupRsp.GetConflictedNames();
    return SOAP_OK;
}

/*====================================================================
函数名      : ModifyDeviceGroup
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
/// Web service operation 'ModifyDeviceGroup' (returns error code or SOAP_OK)
int cusdkService::ModifyDeviceGroup( cusdk__ModifyDeviceGroupReq req, cusdk__ModifyDeviceGroupRsp &rsp )
{
    CCuModifyDeviceGroupReq cCuModifyDeviceGroupReq;
    cCuModifyDeviceGroupReq.SetSession(req.session);
    cCuModifyDeviceGroupReq.SetDeviceGroupID(req.deviceGroupInfo.id);

	cCuModifyDeviceGroupReq.SetDeviceGroupName(req.deviceGroupInfo.name);

    cCuModifyDeviceGroupReq.SetDeviceParentGroupID(req.deviceGroupInfo.parentId);
    cCuModifyDeviceGroupReq.SetDeviceGroupDesc(req.deviceGroupInfo.description);
	cCuModifyDeviceGroupReq.SetGrpSeq(req.deviceGroupInfo.fOrderNumber);
	cCuModifyDeviceGroupReq.SetDevTreeId(req.deviceGroupInfo.devTreeId);
	
    CCuModifyDeviceGroupRsp cCuModifyDeviceGroupRsp;
    CuModifyDeviceGroup(cCuModifyDeviceGroupReq, cCuModifyDeviceGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuModifyDeviceGroupRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::ModifyGBDeviceGroup(cusdk__ModifyGBDeviceGroupReq req, cusdk__ModifyGBDeviceGroupRsp &rsp)
{
	CCuModifyGBDeviceGroupReq cCuModifyGBDeviceGroupReq;
	cCuModifyGBDeviceGroupReq.SetSession(req.session);
	cCuModifyGBDeviceGroupReq.SetDevTreeId(req.devTreeViewId);
	cCuModifyGBDeviceGroupReq.SetDevGrpId(req.devGroupId);
	cCuModifyGBDeviceGroupReq.SetDevGroupGbid(req.devGroupGBID);
	cCuModifyGBDeviceGroupReq.SetPrisec(req.devicePushChnSelectType);

	CCuModifyGBDeviceGroupRsp cCuModifyGBDeviceGroupRsp;
	CuModifyGBDeviceGroup(cCuModifyGBDeviceGroupReq, cCuModifyGBDeviceGroupRsp);

	rsp.operationId = cCuModifyGBDeviceGroupRsp.GetOperUuid();
	rsp.session = req.session;
	rsp.errorCode = cCuModifyGBDeviceGroupRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::AdjustVideoSrcOrder(cusdk__AdjustVideoSrcOrderReq req, cusdk__AdjustVideoSrcOrderRsp &rsp)
{
    cusdk__MoveOperation cuMoveOp = GetMoveOp( req.moveOp );
    if( cuMoveOp == cusdk__MoveInvalid )
    {
        return SOAP_ERR;
    }
    ENOrderType orderType = MoveOperationToOrderType( cuMoveOp );

    TSipURI groupUri;
    TSipURI movDevUri;
    TSipURI refDevUri;
    
    //剔除字符串中的@
    groupUri.SetURIString ( req.groupId           );
    movDevUri.SetURIString( req.vsMove.deviceId   );
    refDevUri.SetURIString( req.vsRefer.deviceId  );

    TVidSrcOrderInfo tSrcOrderInfo;    
    tSrcOrderInfo.SetDevGrpId        ( groupUri.GetUser()       );
    tSrcOrderInfo.SetDevMovedId      ( movDevUri.GetUser()      );
    tSrcOrderInfo.SetVidsrcIndexMoved( req.vsMove.videoSrcId    );
    tSrcOrderInfo.SetDevReferId      ( refDevUri.GetUser()      );
    tSrcOrderInfo.SetVidsrcIndexRefer( req.vsRefer.videoSrcId   );
    tSrcOrderInfo.SetOrderType       ( orderType                ); 

    CCuAdjustVideoSrcOrderReq cCuAdjustVideoSrcOrderReq;
    cCuAdjustVideoSrcOrderReq.SetSession        ( req.session   );
    cCuAdjustVideoSrcOrderReq.SetDevTreeId      ( req.treeId    );
    cCuAdjustVideoSrcOrderReq.SetVidsrcOrderInfo( tSrcOrderInfo );
    
    CCuAdjustVideoSrcOrderRsp cCuAdjustVideoSrcOrderRsp;
    CuAdjustVideoSrcOrder( cCuAdjustVideoSrcOrderReq, cCuAdjustVideoSrcOrderRsp );

	rsp.session   = req.session;
	rsp.errorCode = cCuAdjustVideoSrcOrderRsp.GetErrorCode();

    return SOAP_OK;
}


int cusdkService::GetAlarmTypes(cusdk__GetAlarmTypesReq req, cusdk__GetAlarmTypesRsp &rsp)
{
    CCuGetAlarmTypesReq cCuGetAlarmTypesReq;
	cCuGetAlarmTypesReq.SetSession(req.session);

	CCuGetAlarmTypesRsp cCuGetAlarmTypesRsp;
	CuGetAlarmTypes( cCuGetAlarmTypesReq, cCuGetAlarmTypesRsp );

	ConvertAlarmTypesInfoFromCuiToCu(cCuGetAlarmTypesRsp.GetAlarmTypeArray(), rsp.alarmTypes);

	rsp.session = req.session;
	rsp.errorCode = cCuGetAlarmTypesRsp.GetErrorCode();
    return SOAP_OK;
}


/*====================================================================
函数名      : DeviceGroupAddDevice
功能        : 设备组添加设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'DeviceGroupAddDevice' (returns error code or SOAP_OK)
int cusdkService::DeviceGroupAddDevice( cusdk__DeviceGroupAddDeviceReq req, cusdk__DeviceGroupAddDeviceRsp &rsp )
{
    CCuDeviceGroupAddDeviceReq cCuDeviceGroupAddDeviceReq;
    cCuDeviceGroupAddDeviceReq.SetSession(req.session);
	cCuDeviceGroupAddDeviceReq.SetSrcDevTreeId(req.srcDevTreeId);
	cCuDeviceGroupAddDeviceReq.SetDstDevTreeId(req.dstDevTreeId);

    ConvertDevGroupDevInfoFromCUToCUI(req.relationList, cCuDeviceGroupAddDeviceReq.GetDeviceGroupDeviceInfo());

    CCuDeviceGroupAddDeviceRsp cCuDeviceGroupAddDeviceRsp;
    CuDeviceGroupAddDevice( cCuDeviceGroupAddDeviceReq, cCuDeviceGroupAddDeviceRsp );

    rsp.session   = req.session;
    rsp.errorCode = cCuDeviceGroupAddDeviceRsp.GetErrorCode();
    return SOAP_OK;
}


int cusdkService::DeviceGroupAddGroup(cusdk__DeviceGroupAddGroupReq req, cusdk__DeviceGroupAddGroupRsp& rsp)
{
	CCuDevTreeDevGrpAssignReq cCuDevTreeDevGrpAssignReq;
	cCuDevTreeDevGrpAssignReq.SetSession(req.session);
	cCuDevTreeDevGrpAssignReq.SetSrcGroupId(req.srcGrpId);
	cCuDevTreeDevGrpAssignReq.SetSrcDevTreeId(req.srcDevTreeId);
	cCuDevTreeDevGrpAssignReq.SetDstDevTreeId(req.dstDevTreeId);
	cCuDevTreeDevGrpAssignReq.SetDstParentGrpId(req.dstGrpId);

	cCuDevTreeDevGrpAssignReq.SetOnlyOnline(req.filterOffline);
	cCuDevTreeDevGrpAssignReq.SetOnlyUnAssigned(req.filterExist);
        
    TDevGroupType tCuiDevGrpType = ConvertDevGrpTypeFromCuToCUI( (cusdk__EGBDeviceGroupType)req.GBDeviceGroupType );
    cCuDevTreeDevGrpAssignReq.SetGrpType( tCuiDevGrpType );

	CCuDevTreeDevGrpAssignRsp cCuDevTreeDevGrpAssignRsp;
	CuDeviceGroupAddGroup(cCuDevTreeDevGrpAssignReq, cCuDevTreeDevGrpAssignRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDevTreeDevGrpAssignRsp.GetErrorCode();
	rsp.operationId = cCuDevTreeDevGrpAssignRsp.GetOperUuid();

	return SOAP_OK;
}

/*====================================================================
函数名      : DeviceGroupDelDevice
功能        : 设备组删除设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/13/             liangli
====================================================================*/
/// Web service operation 'DeviceGroupDelDevice' (returns error code or SOAP_OK)
int cusdkService::DeviceGroupDelDevice( cusdk__DeviceGroupDelDeviceReq req, cusdk__DeviceGroupDelDeviceRsp &rsp )
{
    CCuDeviceGroupDelDeviceReq cCuDeviceGroupDelDeviceReq;
    cCuDeviceGroupDelDeviceReq.SetSession(req.session);
	cCuDeviceGroupDelDeviceReq.SetDevTreeId(req.devTreeId);
    ConvertDevGroupDevInfoFromCUToCUI(req.relationList, cCuDeviceGroupDelDeviceReq.GetDeviceGroupDeviceInfo());

    CCuDeviceGroupDelDeviceRsp cCuDeviceGroupDelDeviceRsp;
    CuDeviceGroupDelDevice( cCuDeviceGroupDelDeviceReq, cCuDeviceGroupDelDeviceRsp );

    rsp.session   = req.session;
    rsp.errorCode = cCuDeviceGroupDelDeviceRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : CreateUserGroup
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
int cusdkService::CreateUserGroup( cusdk__CreateUserGroupReq req, cusdk__CreateUserGroupRsp &rsp )
{
    CCuCreateUserGroupReq cCuCreateUserGroupReq;
    cCuCreateUserGroupReq.SetSession(req.session);
    CUserGroupInfo cUserGroupInfo;
    cUserGroupInfo.SetGroupName(req.userGroupInfo.name);
    cUserGroupInfo.SetDescription(req.userGroupInfo.description);
    cUserGroupInfo.SetUserType(static_cast<int>(req.userGroupInfo.userType));
    CUserPrivilege cUserPri;
    ConvertUserPriFromCuToCui(req.userGroupInfo.userPri, cUserPri);
    cUserGroupInfo.SetUserPrivilege(cUserPri);
    cUserGroupInfo.SetCreateUser(req.userGroupInfo.creator);

    MaxStreamCuToCui( req.userGroupInfo, cUserGroupInfo );
    WaterMarkCuToCui( req.userGroupInfo, cUserGroupInfo );

    cCuCreateUserGroupReq.SetUserGroupInfo(cUserGroupInfo);

    CCuCreateUserGroupRsp cCuCreateUserGroupRsp;
    CuCreateUserGroup(cCuCreateUserGroupReq, cCuCreateUserGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuCreateUserGroupRsp.GetErrorCode();
    rsp.userGroupId = cCuCreateUserGroupRsp.GetUserGroupInfoKey().GetGroupNO();
    return SOAP_OK;
}

/*====================================================================
函数名      : DelUserGroup
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
int cusdkService::DelUserGroup( cusdk__DelUserGroupReq req, cusdk__DelUserGroupRsp &rsp )
{
    CCuDeleteUserGroupReq cCuDeleteUserGroupReq;
    cCuDeleteUserGroupReq.SetSession(req.session);
    CUserGroupInfoKey cUserGroupInfoKey;
    cUserGroupInfoKey.SetGroupNO(req.userGroupId);
    cCuDeleteUserGroupReq.SetUserGroupInfoKey(cUserGroupInfoKey);

    CCuDeleteUserGroupRsp cCuDeleteUserGroupRsp;
    CuDeleteUserGroup(cCuDeleteUserGroupReq, cCuDeleteUserGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuDeleteUserGroupRsp.GetErrorCode();
	rsp.operationId = cCuDeleteUserGroupRsp.GetOperUuid();
    return SOAP_OK;
}

/*====================================================================
函数名      : ModifyUserGroup
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
int cusdkService::ModifyUserGroup( cusdk__ModifyUserGroupReq req, cusdk__ModifyUserGroupRsp &rsp )
{
    CCuModifyUserGroupReq cCuModifyUserGroupReq;
    cCuModifyUserGroupReq.SetSession(req.session);
    cCuModifyUserGroupReq.SetGroupNO(req.userGroupInfo.id);
    cCuModifyUserGroupReq.SetGroupName(req.userGroupInfo.name);
    cCuModifyUserGroupReq.SetDescription(req.userGroupInfo.description);

    MaxStreamCuToCui( req.userGroupInfo, cCuModifyUserGroupReq );
    WaterMarkCuToCui( req.userGroupInfo, cCuModifyUserGroupReq );

    CUserPrivilege cUserPri;
    ConvertUserPriFromCuToCui(req.userGroupInfo.userPri, cUserPri);

    cCuModifyUserGroupReq.SetUserPrivilege(cUserPri);
    //不允许修改ID，创建日期，用户组类型和创建者ID，创建者ID只能后台由TAS自动修改

    CCuModifyUserGroupRsp cCuModifyUserGroupRsp;
    CuModifyUserGroup(cCuModifyUserGroupReq, cCuModifyUserGroupRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuModifyUserGroupRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : CreateUser
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
int cusdkService::CreateUser( cusdk__CreateUserReq req, cusdk__CreateUserRsp &rsp )
{
	//如果客户端已经用"SM3_Base64"加密过，则透传否则按老的md5加密	
	if (req.userInfo.passwordAlgorithm != "SM3_Base64")
	{
		std::string strEncPassword = req.userInfo.userPassword;
		MD5(req.userInfo.userPassword, strEncPassword);
		req.userInfo.userPassword = strEncPassword;
	}
    CCuCreateUserReq cCuCreateUserReq;
    cCuCreateUserReq.SetSession(req.session);
    CCuUserInfo cUserInfo;
    ConvertUserInfoFromCuToCui(req.userInfo, cUserInfo);
    cCuCreateUserReq.SetUserInfo(cUserInfo);

    CCuCreateUserRsp cCuCreateUserRsp;
    CuCreateUser(cCuCreateUserReq, cCuCreateUserRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuCreateUserRsp.GetErrorCode();
    rsp.userId = cCuCreateUserRsp.GetUserID();
    return SOAP_OK;
}

/*====================================================================
函数名      : DelUser
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
int cusdkService::DelUser( cusdk__DelUserReq req, cusdk__DelUserRsp &rsp )
{
    CCuDeleteUserReq cCuDeleteUserReq;
    cCuDeleteUserReq.SetSession(req.session);
    cCuDeleteUserReq.SetUserID(req.userId);

    CCuDeleteUserRsp cCuDeleteUserRsp;
    CuDeleteUser(cCuDeleteUserReq, cCuDeleteUserRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuDeleteUserRsp.GetErrorCode();
	rsp.operationId = cCuDeleteUserRsp.GetOperUuid();
    return SOAP_OK;
}

/*====================================================================
函数名      : ModifyUser
功能        : 修改子用户基本信息，包括修改用户的组属性
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
int cusdkService::ModifyUser( cusdk__ModifyUserReq req, cusdk__ModifyUserRsp &rsp )
{
    CCuModifyUserReq cCuModifyUserReq;
    cCuModifyUserReq.SetSession(req.session);
    cCuModifyUserReq.SetUserID(req.userInfo.uuid);
    cCuModifyUserReq.SetUsergrpID(req.userInfo.userGroup);
    cCuModifyUserReq.SetUserName(req.userInfo.userName);
    if(!req.userInfo.userPassword.empty())
    {
		//如果客户端已经用"SM3_Base64"加密过，则透传否则按老的md5加密	
		if (req.userInfo.passwordAlgorithm != "SM3_Base64")
		{
			std::string strEncPassword = req.userInfo.userPassword;
			MD5(req.userInfo.userPassword, strEncPassword);
			req.userInfo.userPassword = strEncPassword;
		}

        cCuModifyUserReq.SetUserPwd(req.userInfo.userPassword);
    }  
    cCuModifyUserReq.SetUserType(static_cast<int>(req.userInfo.userType));
    cCuModifyUserReq.SetMLoginFlag(req.userInfo.allowMultilogin);
    cCuModifyUserReq.SetEnabledFlag(req.userInfo.enable);
    cCuModifyUserReq.SetDescription(req.userInfo.description);
    CCMSTime cNameExpireData(req.userInfo.accountOverdueTime);
    cCuModifyUserReq.SetNameExpireDate(cNameExpireData.ToString());
    CCMSTime cPwdExpireData(req.userInfo.passwordOverdueTime);
    cCuModifyUserReq.SetPwdExpireDate(cPwdExpireData.ToString());
	cCuModifyUserReq.SetPwdExpireEnabledFlag(req.userInfo.passwordExpireEnable);
	cCuModifyUserReq.SetPwdExpireTime(req.userInfo.passwordExpire);
	cCuModifyUserReq.SetLoginIpList(req.userInfo.bindingAddress);
	cCuModifyUserReq.SetIpListEnabledFlag(req.userInfo.bindIPEnable);
	cCuModifyUserReq.SetIpBindFlagForMobileClient(req.userInfo.IsIPAppliedToMobileEnd);
    MaxStreamCuToCui( req.userInfo, cCuModifyUserReq );
    WaterMarkCuToCui( req.userInfo, cCuModifyUserReq );

    CUserPrivilege cPri;
    ConvertUserPriFromCuToCui(req.userInfo.userPri, cPri);

    cCuModifyUserReq.SetUserPrivilege(cPri);

    CCuModifyUserRsp cCuModifyUserRsp;
    CuModifyUser(cCuModifyUserReq, cCuModifyUserRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuModifyUserRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : UserGetDeviceByGroup
功能        : 获取用户或者用户组的设备划归列表
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/14/             liangli
====================================================================*/
int cusdkService::UserGetDeviceByGroup( cusdk__UserGetDeviceByGroupReq req, cusdk__UserGetDeviceByGroupRsp &rsp )
{
    if ( req.type == cusdk__TypeUser )
    {
        CCuGetUserDeviceReq cCuGetUserDeviceReq;
        cCuGetUserDeviceReq.SetSession(req.session);
        cCuGetUserDeviceReq.SetStartEntryIndex(req.beginIndex);
        cCuGetUserDeviceReq.SetExpectEntryNum(req.num);
        CUserDeviceInfoQueryCondition cCondition;
        cCondition.SetUserID(req.id);
        cCondition.SetDevGrpID(req.deviceGroupId);
        cCuGetUserDeviceReq.SetUserDeviceInfoQueryCondition(cCondition);

        CCuGetUserDeviceRsp cCuGetUserDeviceRsp;
        CuGetUserDevice( cCuGetUserDeviceReq, cCuGetUserDeviceRsp );
        rsp.errorCode = cCuGetUserDeviceRsp.GetErrorCode();
        if ( cCuGetUserDeviceRsp.GetActualResultNum() > 0 )
        {
            rsp.totalNum = cCuGetUserDeviceRsp.GetTotalEntryNum();
            vector<CUserDeviceInfo>::const_iterator pUserDevInfoItem = cCuGetUserDeviceRsp.GetCuserDevInfo().begin();
            while ( pUserDevInfoItem != cCuGetUserDeviceRsp.GetCuserDevInfo().end() )
            {
                cusdk__AssignDeviceInfo cAssignDeviceInfo;
                ConvertUserDeviceInfoFromCuiToCu(*pUserDevInfoItem, cAssignDeviceInfo);
                rsp.deviceList.push_back(cAssignDeviceInfo);
                pUserDevInfoItem++;
            }
        }
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuGetUserGroupDeviceReq cCuGetUserGroupDeviceReq;
        cCuGetUserGroupDeviceReq.SetSession(req.session);
        cCuGetUserGroupDeviceReq.SetStartEntryIndex(req.beginIndex);
        cCuGetUserGroupDeviceReq.SetExpectEntryNum(req.num);
        CUserGroupDeviceInfoQueryCondition cCondition;
        cCondition.SetCuserGrpID(req.id);
        cCondition.SetDevGrpID(req.deviceGroupId);
        cCuGetUserGroupDeviceReq.SetUserGroupDeviceInfoQueryCondition(cCondition);

        CCuGetUserGroupDeviceRsp cCuGetUserGroupDeviceRsp;
        CuGetUserGroupDevice( cCuGetUserGroupDeviceReq, cCuGetUserGroupDeviceRsp );
        rsp.errorCode = cCuGetUserGroupDeviceRsp.GetErrorCode();
        if ( cCuGetUserGroupDeviceRsp.GetActualResultNum() > 0 )
        {
            rsp.totalNum = cCuGetUserGroupDeviceRsp.GetTotalEntryNum();
            vector<CUserGroupDeviceInfo>::const_iterator pUserGroupDevInfoItem = cCuGetUserGroupDeviceRsp.GetUserGroupDeviceInfo().begin();
            while ( pUserGroupDevInfoItem != cCuGetUserGroupDeviceRsp.GetUserGroupDeviceInfo().end() )
            {
                cusdk__AssignDeviceInfo cAssignDeviceInfo;
                ConvertUserGroupDeviceInfoFromCuiToCu(*pUserGroupDevInfoItem, cAssignDeviceInfo);
                rsp.deviceList.push_back(cAssignDeviceInfo);
                pUserGroupDevInfoItem++;
            }
        }
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

/*====================================================================
函数名      : UserAddDevice
功能        : 用户管理之 添加用户（组）设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
int cusdkService::UserAddDevice( cusdk__UserAddDeviceReq req, cusdk__UserAddDeviceRsp &rsp )
{
    if ( req.type == cusdk__TypeUser )
    {
        CCuUserAddDeviceReq cCuUserAddDeviceReq;
        cCuUserAddDeviceReq.SetSession(req.session);
        vector<cusdk__AssignDeviceInfo>::iterator pUserDevItem = req.deviceList.begin();
        while ( pUserDevItem != req.deviceList.end() )
        {
            CUserDeviceInfo cUserDevInfo;
            ConvertUserDeviceInfoFromCuToCui(*pUserDevItem, cUserDevInfo);
            //需要手工设置UserID
            cUserDevInfo.SetUserID(req.id);
            cCuUserAddDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
            pUserDevItem++;
        }

        CCuUserAddDeviceRsp cCuUserAddDeviceRsp;
        CuUserAddDevice(cCuUserAddDeviceReq, cCuUserAddDeviceRsp);
        rsp.errorCode = cCuUserAddDeviceRsp.GetErrorCode();
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuUserGroupAddDeviceReq cCuUserGroupAddDeviceReq;
        cCuUserGroupAddDeviceReq.SetSession(req.session);
        vector<cusdk__AssignDeviceInfo>::iterator pUserGroupDevItem = req.deviceList.begin();
        while ( pUserGroupDevItem != req.deviceList.end() )
        {
            CUserGroupDeviceInfo cUserGroupDevInfo;
            ConvertUserGroupDeviceInfoFromCuToCui(*pUserGroupDevItem, cUserGroupDevInfo);
            //需要手工设置UserGroupID
            cUserGroupDevInfo.SetGroupID(req.id);
            cCuUserGroupAddDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);
            pUserGroupDevItem++;
        }

        CCuUserGroupAddDeviceRsp cCuUserGroupAddDeviceRsp;
        CuUserGroupAddDevice(cCuUserGroupAddDeviceReq, cCuUserGroupAddDeviceRsp);
        rsp.errorCode = cCuUserGroupAddDeviceRsp.GetErrorCode();
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

/*====================================================================
函数名      : UserDelDevice
功能        : 用户管理之 删除用户（组）设备
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
int cusdkService::UserDelDevice( cusdk__UserDelDeviceReq req, cusdk__UserDelDeviceRsp &rsp )
{
    if ( req.type == cusdk__TypeUser )
    {
        CCuUserDelDeviceReq cCuUserDelDeviceReq;
        cCuUserDelDeviceReq.SetSession(req.session);
        vector<cusdk__AssignDeviceInfo>::iterator pUserDevItem = req.deviceList.begin();
        while ( pUserDevItem != req.deviceList.end() )
        {
            CUserDeviceInfo cUserDevInfo;
            ConvertUserDeviceInfoFromCuToCui(*pUserDevItem, cUserDevInfo);
            //需要手工设置UserID
            cUserDevInfo.SetUserID(req.id);
            cCuUserDelDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
            pUserDevItem++;
        }
        CCuUserDelDeviceRsp cCuUserDelDeviceRsp;
        CuUserDelDevice(cCuUserDelDeviceReq, cCuUserDelDeviceRsp);
        rsp.errorCode = cCuUserDelDeviceRsp.GetErrorCode();
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuUserGroupDelDeviceReq cCuUserGroupDelDeviceReq;
        cCuUserGroupDelDeviceReq.SetSession(req.session);
        vector<cusdk__AssignDeviceInfo>::iterator pUserGroupDevItem = req.deviceList.begin();
        while ( pUserGroupDevItem != req.deviceList.end() )
        {
            CUserGroupDeviceInfo cUserGroupDevInfo;
            ConvertUserGroupDeviceInfoFromCuToCui(*pUserGroupDevItem, cUserGroupDevInfo);
            //需要手工设置UserGroupID
            cUserGroupDevInfo.SetGroupID(req.id);
            cCuUserGroupDelDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);
            pUserGroupDevItem++;
        }

        CCuUserGroupDelDeviceRsp cCuUserGroupDelDeviceRsp;
        CuUserGroupDelDevice(cCuUserGroupDelDeviceReq, cCuUserGroupDelDeviceRsp);
        rsp.errorCode = cCuUserGroupDelDeviceRsp.GetErrorCode();
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

/*====================================================================
函数名      : UserModifyDevice
功能        : 用户管理之 修改用户（组）设备权限
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/15/             liangli
====================================================================*/
int cusdkService::UserModifyDevice( cusdk__UserModifyDeviceReq req, cusdk__UserModifyDeviceRsp &rsp )
{
    if ( req.type == cusdk__TypeUser )
    {
        CCuUserModDeviceReq cCuUserModDeviceReq;
        cCuUserModDeviceReq.SetSession(req.session);

        vector<cusdk__AssignDeviceInfo>::iterator pUserDevItem = req.deviceList.begin();
        while ( pUserDevItem != req.deviceList.end() )
        {
            CUserDeviceInfo cUserDevInfo;
            ConvertUserDeviceInfoFromCuToCui(*pUserDevItem, cUserDevInfo);
            cUserDevInfo.SetUserID(req.id);
            cCuUserModDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
            pUserDevItem++;
        }
        CCuUserModDeviceRsp cCuUserModDeviceRsp;
        CuUserModDevice(cCuUserModDeviceReq, cCuUserModDeviceRsp);
        rsp.errorCode = cCuUserModDeviceRsp.GetErrorCode();
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuUserGroupModDeviceReq cCuUserGroupModDeviceReq;
        cCuUserGroupModDeviceReq.SetSession(req.session);
        vector<cusdk__AssignDeviceInfo>::iterator pUserGroupDevItem = req.deviceList.begin();
        while ( pUserGroupDevItem != req.deviceList.end() )
        {
            CUserGroupDeviceInfo cUserGroupDevInfo;
            ConvertUserGroupDeviceInfoFromCuToCui(*pUserGroupDevItem, cUserGroupDevInfo);
            cUserGroupDevInfo.SetGroupID(req.id);
            cCuUserGroupModDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);
            pUserGroupDevItem++;
        }
        CCuUserGroupModDeviceRsp cCuUserGroupModDeviceRsp;
        CuUserGroupModDevice(cCuUserGroupModDeviceReq, cCuUserGroupModDeviceRsp);
        rsp.errorCode = cCuUserGroupModDeviceRsp.GetErrorCode();
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

/*====================================================================
函数名      : GetAuthorizationDevice
功能        : 临时授权功能只有超级管理员才能使用，所以临时授权是针对域，不针对其他用户
获取本域的临时授权设备列表接口普通CU用户也能使用，其他接口都只能超级管理员使用
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/16/             liangli
====================================================================*/
int cusdkService::GetAuthorizationDevice( cusdk__GetAuthorizationDeviceReq req, cusdk__GetAuthorizationDeviceRsp &rsp )
{
    CCuGetAuthorizationDeviceReq cCuGetAuthorizationDeviceReq;
    cCuGetAuthorizationDeviceReq.SetSession(req.session);
    cCuGetAuthorizationDeviceReq.SetStartEntryIndex(req.beginIndex);
    cCuGetAuthorizationDeviceReq.SetExpectEntryNum(req.num);

    CCuGetAuthorizationDeviceRsp cCuGetAuthorizationDeviceRsp;
    CuGetGetAuthorizationDevice(cCuGetAuthorizationDeviceReq, cCuGetAuthorizationDeviceRsp);
    rsp.errorCode = cCuGetAuthorizationDeviceRsp.GetErrorCode();
    if ( cCuGetAuthorizationDeviceRsp.GetActualResultNum() > 0 )
    {
        rsp.totalNum = cCuGetAuthorizationDeviceRsp.GetTotalEntryNum();
        ConvertTempAuthorizationInfoFromCuiToCu(cCuGetAuthorizationDeviceRsp.GetTempAuthorizationInfo(), rsp.authorizedDeviceList);
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

/*====================================================================
函数名      : AddAuthorizationDevice
功能        : CU向目标域指定用户临时授权某设备
算法实现    : 只能超级管理员使用
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/17/             liangli
====================================================================*/
int cusdkService::AddAuthorizationDevice( cusdk__AddAuthorizationDeviceReq req, cusdk__AddAuthorizationDeviceRsp &rsp )
{
    CCuAddAuthorizationDeviceReq cCuAddAuthorizationDeviceReq;
    cCuAddAuthorizationDeviceReq.SetSession(req.session);
    CTempAuthorizationInfo cTempAuthorizationInfo;
    ConvertTempAuthorizationInfoFromCuToCui(req.authorizedDev, cTempAuthorizationInfo);
    cCuAddAuthorizationDeviceReq.SetTempAuthorizationInfo(cTempAuthorizationInfo);

    CCuAddAuthorizationDeviceRsp cCuAddAuthorizationDeviceRsp;
    CuAddAuthorizationDevice(cCuAddAuthorizationDeviceReq, cCuAddAuthorizationDeviceRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuAddAuthorizationDeviceRsp.GetErrorCode();
    time(&(rsp.createTime));
    return SOAP_OK;
}

/*====================================================================
函数名      : DelAuthorizationDevice
功能        : 删除临时授权设备，仅超级管理员能使用
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/17/             liangli
====================================================================*/
int cusdkService::DelAuthorizationDevice( cusdk__DelAuthorizationDeviceReq req, cusdk__DelAuthorizationDeviceRsp &rsp )
{
    CCuDelAuthorizationDeviceReq cCuDelAuthorizationDeviceReq;
    cCuDelAuthorizationDeviceReq.SetSession(req.session);
    CTempAuthorizationInfoKey cTempAuthorizationInfoKey;
    cTempAuthorizationInfoKey.SetDeviceID(req.authorizedDev.devInfo.devId);
    vector<int > vidList;
    vector<cusdk__AssignVideoSrc >::const_iterator pVidItem = req.authorizedDev.devInfo.videoSrcIdList.begin();
    while ( pVidItem != req.authorizedDev.devInfo.videoSrcIdList.end() )
    {
        vidList.push_back(pVidItem->vieoSrcId);
        pVidItem++;
    }
    ConvertDeviceCapIndexsFromCuToCui(vidList, cTempAuthorizationInfoKey.GetDeviceCapIndexs());
    cTempAuthorizationInfoKey.SetUserName(GetNameWithoutDomainFromURI(req.authorizedDev.userName));
    cTempAuthorizationInfoKey.SetDestinationDomain(GetDomainNameWithoutPreNameFromURI(req.authorizedDev.userName));
    cCuDelAuthorizationDeviceReq.SetTempAuthorizationInfoKey(cTempAuthorizationInfoKey);

    CCuDelAuthorizationDeviceRsp cCuDelAuthorizationDeviceRsp;
    CuDelAuthorizationDevice(cCuDelAuthorizationDeviceReq, cCuDelAuthorizationDeviceRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuDelAuthorizationDeviceRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : ModifyAuthorizationDevice
功能        : 修改授权设备信息，仅超级管理员能使用，目前只能修改描述
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/17/             liangli
====================================================================*/
int cusdkService::ModifyAuthorizationDevice( cusdk__ModifyAuthorizationDeviceReq req, cusdk__ModifyAuthorizationDeviceRsp &rsp )
{
    CCuModifyAuthorizationDeviceReq cCuModifyAuthorizationDeviceReq;
    cCuModifyAuthorizationDeviceReq.SetSession(req.session);
    cCuModifyAuthorizationDeviceReq.SetDeviceID(req.authorizedDev.devInfo.devId);
    cCuModifyAuthorizationDeviceReq.SetDesc(req.authorizedDev.description);

    CCuModifyAuthorizationDeviceRsp cCuModifyAuthorizationDeviceRsp;
    CuModifyAuthorizationDevice(cCuModifyAuthorizationDeviceReq, cCuModifyAuthorizationDeviceRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuModifyAuthorizationDeviceRsp.GetErrorCode();
    return SOAP_OK;
}

/*====================================================================
函数名      : GetDeviceBaseInfo
功能        : 查询单个设备的基本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/17/             liangli
====================================================================*/
int cusdkService::GetDeviceBaseInfo( cusdk__GetDeviceBaseInfoReq req, cusdk__GetDeviceBaseInfoRsp &rsp )
{
    CCuGetDeviceBaseInfoReq cCuGetDeviceBaseInfoReq;
    cCuGetDeviceBaseInfoReq.SetSession(req.session);
    CDeviceInfoQueryCondition cCondition;
    cCondition.SetDeviceUUID(GetNameWithoutDomainFromURI(req.devId));
    cCuGetDeviceBaseInfoReq.SetDeviceInfoQueryCondition(cCondition);

    CCuGetDeviceBaseInfoRsp cCuGetDeviceBaseInfoRsp;
    CuGetDeviceBaseInfo(cCuGetDeviceBaseInfoReq, cCuGetDeviceBaseInfoRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuGetDeviceBaseInfoRsp.GetErrorCode();
    if ( cCuGetDeviceBaseInfoRsp.GetActualResultNum() > 0 )
    {
        ConvertDeviceBaseInfoFromCuiToCu(cCuGetDeviceBaseInfoRsp.GetDeviceInfo().at( 0 ), rsp.devInfo);
    }
    return SOAP_OK;
}

/*====================================================================
函数名      : SetupStream
功能        : Invite视频码流
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/2/22/             liangli
====================================================================*/
int cusdkService::SetupStream( cusdk__SetupStreamReq req, cusdk__SetupStreamRsp &rsp )
{
    CCuSetupStreamReq cCuSetupStreamReq;
    cCuSetupStreamReq.SetSession(req.session);
    //源是PU
    TChannel tSrcChn;
    tSrcChn.SetDevUri(req.devId);
    tSrcChn.SetChnNO(req.devChnId);
    cCuSetupStreamReq.SetSrcChn(tSrcChn);
    //目的是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(req.session);
    tCuChn.SetChnNO(req.playId);
    cCuSetupStreamReq.SetDstChn(tCuChn);

    TSdp tSdp;
    tSdp.SetOwner(tCuChn);
    tSdp.SetSessionName(SDP_SESSION_PLAY);

    if( req.directTrans)
    {
        tSdp.SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, TRUE_STR);
    }

	//设置MRTC使能
	if (cusdk__TransModeMRTC == req.transMode)
	{
		tSdp.SetMrtcEnable(true);
	}

	//设置平台直传
	if (req.directTransFromDeviceDomain)
	{
		tSdp.SetPlatDirectTransfer(true);
	}

    TSdpUri tSdpUri;
    //实时浏览时无需填写Token
    /*tSdpUri.SetRecToken(req.)*/
    //实时浏览时无需填写，因为直接和pu交互
    //tSdpUri.SetPuChn();
    //不存在录像文件的token，为实时浏览
    //tSdpUri.SetPlaybackType();
    tSdp.SetUri(tSdpUri);
    //浏览时无需填写
    //tSdp.SetTimeRange();
    //只有放像时填写
    //TSdp.SetPlaybackSessUri();

    //遍历每个CU的IP
    ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

    cCuSetupStreamReq.SetSdp(tSdp);

    CCuSetupStreamRsp cCuSetupStreamRsp;
    CuSetupStream( cCuSetupStreamReq, cCuSetupStreamRsp );

    rsp.session   = req.session;
    rsp.errorCode = cCuSetupStreamRsp.GetErrorCode();

    if(CMS_SUCCESS == rsp.errorCode)
    {
        ConvertTempMediaDescAndExtInfoCuiToCu(cCuSetupStreamRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
            cCuSetupStreamRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
    }

    return SOAP_OK;
}

int cusdkService::PlayStream( cusdk__PlayStreamReq req, cusdk__PlayStreamRsp &rsp )
{
    CCuPlayStreamReq cCuPlayStreamReq;
    cCuPlayStreamReq.SetSession(req.session);
    cCuPlayStreamReq.SetPlayId(req.playId);

    CCuPlayStreamRsp cCuPlayStreamRsp;
    CuPlayStream(cCuPlayStreamReq, cCuPlayStreamRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuPlayStreamRsp.GetErrorCode();
    return SOAP_OK;
}
int cusdkService::StopStream( cusdk__StopStreamReq req, cusdk__StopStreamRsp &rsp )
{
    CCuStopStreamReq cCuStopStreamReq;
    cCuStopStreamReq.SetSession(req.session);
    cCuStopStreamReq.SetPlayId(req.playId);

    CCuStopInviteRsp cCuStopStreamRsp;
    CuStopStream( cCuStopStreamReq, cCuStopStreamRsp );

    rsp.session   = req.session;
    rsp.errorCode = cCuStopStreamRsp.GetErrorCode();
    return SOAP_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int cusdkService::GetRCSSysParam(cusdk__GetRCSSysParamReq req, cusdk__GetRCSSysParamRsp &rsp)
{
    CCuRcsGetSysParamReq cCuRcsGetSysParamReq;
    cCuRcsGetSysParamReq.SetSession(req.session);

    CCuRcsGetSysParamRsp cCuRcsGetSysParamRsp;
    CuGetRCSSysParam(cCuRcsGetSysParamReq, cCuRcsGetSysParamRsp);

    ConvertTempCoverPolicyFromCuToCui(cCuRcsGetSysParamRsp.GetCoverPolicy(), rsp.coverPolicy); 

    rsp.session   = req.session;
    rsp.errorCode = cCuRcsGetSysParamRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetRCSSysParam(cusdk__SetRCSSysParamReq req, cusdk__SetRCSSysParamRsp &rsp)
{
    CCuRcsSetSysParamReq cCuRcsSetSysParamReq;
    cCuRcsSetSysParamReq.SetSession(req.session);

    ConvertTempCoverPolicyFromCuiToCu(req.coverPolicy, cCuRcsSetSysParamReq.GetCoverPolicy());

    CCuRcsSetSysParamRsp cCuRcsSetSysParamRsp;
    CuSetRCSSysParam(cCuRcsSetSysParamReq, cCuRcsSetSysParamRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuRcsSetSysParamRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetNruList(cusdk__GetNruListReq req, cusdk__GetNruListRsp &rsp)
{
    CCuRcsQueryNruReq cCuRcsQueryNruReq;
    cCuRcsQueryNruReq.SetSession(req.session);
    cCuRcsQueryNruReq.SetExpectEntryNum(req.num);
    cCuRcsQueryNruReq.SetStartEntryIndex(req.beginIndex);

    CCuRcsQueryNruRsp cCuRcsQueryNruRsp;
    CuGetNruList(cCuRcsQueryNruReq, cCuRcsQueryNruRsp);

    ConvertTempNruInfoFromCuToCui(cCuRcsQueryNruRsp.GetNruInfo(), rsp.nruList);

    rsp.session   =  req.session;
    rsp.errorCode =  cCuRcsQueryNruRsp.GetErrorCode();
    rsp.totalNum  =  cCuRcsQueryNruRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::GetPuChnBinding(cusdk__GetPuChnBindingReq req, cusdk__GetPuChnBindingRsp &rsp)
{
    CCuRcsQueryPuChnBindingReq cCuRcsQueryPuChnBindingReq;
    cCuRcsQueryPuChnBindingReq.SetSession(req.session);
    cCuRcsQueryPuChnBindingReq.SetDiskMountPath(req.diskMountPath);
    cCuRcsQueryPuChnBindingReq.SetNruID(req.nruId);
    cCuRcsQueryPuChnBindingReq.SetExpectEntryNum(req.num);
    cCuRcsQueryPuChnBindingReq.SetStartEntryIndex(req.beginIndex);

    CCuRcsQueryPuChnBindingRsp cCuRcsQueryPuChnBindingRsp;
    CuGetPuChnBinding(cCuRcsQueryPuChnBindingReq, cCuRcsQueryPuChnBindingRsp);

    ConverTempChnFromCuiToCu(cCuRcsQueryPuChnBindingRsp.GetChnList(), rsp.channelList);

    rsp.session = req.session;
    rsp.errorCode = cCuRcsQueryPuChnBindingRsp.GetErrorCode();
    rsp.totalNum  =  cCuRcsQueryPuChnBindingRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::BindPu(cusdk__BindPuReq req, cusdk__BindPuRsp &rsp)
{
    CCuRcsBindingPuChnReq cCuRcsBindingPuChnReq;
    cCuRcsBindingPuChnReq.SetSession(req.session);

    TPuChnBinding puChnBinding;
    puChnBinding.SetDiskMountPath(req.diskMountPath);
    puChnBinding.SetNruID(req.nruId);

    ConverTempChnFromCuToCui(req.channelList, puChnBinding.GetPuChnList());

    cCuRcsBindingPuChnReq.SetPuChnBinding(puChnBinding);

    CCuRcsBindingPuChnRsp cCuRcsBindingPuChnRsp;
    CuBindPu(cCuRcsBindingPuChnReq, cCuRcsBindingPuChnRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRcsBindingPuChnRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::UnbindPu(cusdk__UnbindPuReq req, cusdk__UnbindPuRsp &rsp)
{
    CCuRcsUnbindingPuChnReq cCuRcsUnbindingPuChnReq;
    cCuRcsUnbindingPuChnReq.SetSession(req.session);

    TPuChnBinding puChnBinding;
    puChnBinding.SetDiskMountPath(req.diskMountPath);
    puChnBinding.SetNruID(req.nruId);

    ConverTempChnFromCuToCui(req.channelList, puChnBinding.GetPuChnList());

    cCuRcsUnbindingPuChnReq.SetPuChnBinding(puChnBinding);

    CCuRcsUnbindingPuChnRsp cCuRcsUnbindingPuChnRsp;
    CuUnBindPu(cCuRcsUnbindingPuChnReq, cCuRcsUnbindingPuChnRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRcsUnbindingPuChnRsp.GetErrorCode();

    return SOAP_OK;
}

// int cusdkService::GetPlatformRecord(cusdk__GetPlatformRecordReq req, cusdk__GetPlatformRecordRsp &rsp)
// {
//     CCuRcsQueryRecordReq cCuRcsQueryRecordReq;
//     cCuRcsQueryRecordReq.SetSession(req.session);
//     CRcsQueryRecordReq cRcsQueryRecordReq;    
//     cRcsQueryRecordReq.SetSession(req.session);
// 
//     TChannel tChannel;
//     tChannel.SetChnNO(req.devChn.channelId);
//     tChannel.SetDevUri(req.devChn.deviceId);
//     cRcsQueryRecordReq.SetPuChn(tChannel);
// 
//     switch(req.recEventType)
//     {
//     case cusdk__RS_RECTYPE_ALL:
//         {
//             //此项不填写表示查询所有录像
//             //cCuRcsQueryRecordReq.SetRecType();
//         }
//         break;
//     case cusdk__RS_RECTYPE_ALARM:
//         {
//             cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_ALARM);
//         }
//         break;
//     case cusdk__RS_RECTYPE_HANDLE:
//         {
//             cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_HANDLE);
//         }
//         break;
//     case cusdk__RS_RECTYPE_PLAN:
//         {
//             cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_PLAN);
//         }
//         break;
//     case cusdk__RS_RECTYPE_UNKNOWN:
//         {
//             cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_UNKNOWN);
//         }
//         break;
//     default:
//         break;
//     }
// 
//     CCMSTime endTime(req.timeRange.endTime);
//     CCMSTime startTime(req.timeRange.startTime);
// 
//     TTimeRange tTimeRange;
//     tTimeRange.SetEndTime(endTime.ToString());
//     tTimeRange.SetStartTime(startTime.ToString());
//     cRcsQueryRecordReq.SetTimeRange(tTimeRange);
// 
//     cCuRcsQueryRecordReq.SetReq(cRcsQueryRecordReq.ToXml());
//     cCuRcsQueryRecordReq.SetRecordDomainName(req.recordDomainName);
// 
//     CCuRcsQueryRecordRsp cCuRcsQueryRecordRsp;
//     CuGetPlatformRecord(cCuRcsQueryRecordReq, cCuRcsQueryRecordRsp);
// 
// 
//     ConverTempRecordDurationFromCuiToCu(cCuRcsQueryRecordRsp.GetDurantionList(), rsp.recordDurationList, cCuRcsQueryRecordRsp.GetDomainName());
// 
//     rsp.errorCode = cCuRcsQueryRecordRsp.GetErrorCode();
//     rsp.session   = req.session;
//     rsp.devChn    = req.devChn;
// 
//     return SOAP_OK;
// }

//采用订阅形式
int cusdkService::StartPlatformRecord(cusdk__StartPlatformRecordReq req, cusdk__StartPlatformRecordRsp &rsp)
{
    CCuRcsStartRecordTaskReq cCuRcsStartRecordTaskReq;
    cCuRcsStartRecordTaskReq.SetIsStart(true);
    cCuRcsStartRecordTaskReq.SetRecType(RCS_RECTYPE_HANDLE);
    cCuRcsStartRecordTaskReq.SetSession(req.session);

    TChannel tChn;
    tChn.SetChnNO(req.devChn.channelId);
    tChn.SetDevUri(req.devChn.deviceId);
    cCuRcsStartRecordTaskReq.SetPuChn(tChn);

    CCuRcsStartRecordTaskRsp cCuRcsStartRecordTaskRsp;
    CuStartPlatformRecord(cCuRcsStartRecordTaskReq, cCuRcsStartRecordTaskRsp);

    rsp.errorCode = cCuRcsStartRecordTaskRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

//采用订阅形式
int cusdkService::StopPlatformRecord(cusdk__StopPlatformRecordReq req, cusdk__StopPlatformRecordRsp &rsp)
{
    CCuRcsStopRecordTaskReq cCuRcsStopRecordTaskReq;
    cCuRcsStopRecordTaskReq.SetIsStart(false);
    cCuRcsStopRecordTaskReq.SetRecType(RCS_RECTYPE_HANDLE);
    cCuRcsStopRecordTaskReq.SetSession(req.session);

    TChannel tChn;
    tChn.SetChnNO(req.devChn.channelId);
    tChn.SetDevUri(req.devChn.deviceId);
    cCuRcsStopRecordTaskReq.SetPuChn(tChn);

    CCuRcsStopRecordTaskRsp cCuRcsStopRecordTaskRsp;
    CuStopPlatformRecord(cCuRcsStopRecordTaskReq, cCuRcsStopRecordTaskRsp);

    rsp.errorCode = cCuRcsStopRecordTaskRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

// int cusdkService::SetupPlayPlatformRecord(cusdk__SetupPlayPlatformRecordReq req, cusdk__SetupPlayPlatformRecordRsp &rsp)
// {
//     CCuSetupPlayPlatformRecordReq cCuSetupPlayPlatformRecordReq;
//     cCuSetupPlayPlatformRecordReq.SetSession(req.session);
//     cCuSetupPlayPlatformRecordReq.SetDomainName(req.recordPlayDuration.recordDomainName);
//     CInviteReq cSetupStreamReq;
//     cSetupStreamReq.SetSession(req.session);
// 
//     //源是PU
//     TChannel tSrcChn;
//     tSrcChn.SetDevUri(req.recordPlayDuration.encoderChn.deviceId);
//     tSrcChn.SetChnNO(req.recordPlayDuration.encoderChn.channelId);
//     cSetupStreamReq.SetSrcChn(tSrcChn);
//     //目的是CU
//     TChannel tCuChn;
//     tCuChn.SetDevUri(req.session);
//     tCuChn.SetChnNO(req.playId);
//     cSetupStreamReq.SetDstChn(tCuChn);
// 
//     TSdp tSdp;
//     tSdp.SetOwner(tCuChn);
//     tSdp.SetSessionName(SDP_SESSION_PLAYBACK);
// 
//     TSdpUri tSdpUri;
//     tSdpUri.SetRecToken(req.recordPlayDuration.recordDurationId);
//     tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PLATFORM);
//     tSdpUri.SetPuChn(tSrcChn);
//     tSdp.SetUri(tSdpUri);
// 
//     TTimeRange tTimeRange;
//     CCMSTime begin(req.recordPlayDuration.durationTimeRange.startTime);
//     CCMSTime end(req.recordPlayDuration.durationTimeRange.endTime);
//     tTimeRange.SetStartTime(begin.ToString());
//     tTimeRange.SetEndTime(end.ToString());
//     tSdp.SetTimeRange(tTimeRange);
//     //rsp消息中的TSdp才携带
//     //TSdp.SetPlaybackSessUri(req.recordPlayDuration.);
// 
//     //遍历每个CU的IP
//     ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());
// 
//     cSetupStreamReq.SetSdp(tSdp);
//     cCuSetupPlayPlatformRecordReq.SetReq(cSetupStreamReq);
// 
//     CCuSetupPlayPlatformRecordRsp cCuSetupPlayPlatformRecordRsp;
//     CuSetupPlayPlatformRecord( cCuSetupPlayPlatformRecordReq, cCuSetupPlayPlatformRecordRsp );
// 
//     rsp.session   = req.session;
//     rsp.errorCode = cCuSetupPlayPlatformRecordRsp.GetErrorCode();
//     rsp.recordPlaySession = cCuSetupPlayPlatformRecordRsp.GetSdp().GetPlaybackSessUri();
//     if ( CMS_SUCCESS == cCuSetupPlayPlatformRecordRsp.GetErrorCode() )
//     {
//         ConverTempMediaDescListCuiToCu(cCuSetupPlayPlatformRecordRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList);
//     }
// 
//     return SOAP_OK;
// }
// 
// int cusdkService::PlayPlatformRecord(cusdk__PlayPlatformRecordReq req, cusdk__PlayPlatformRecordRsp &rsp)
// {
//     CCuPlayPlatformRecordReq cCuPlayPlatformRecordReq;
//     cCuPlayPlatformRecordReq.SetSession(req.session);
//     cCuPlayPlatformRecordReq.SetPlayId(req.playId);
// 
//     CCuPlayPlatformRecordRsp cCuPlayPlatformRecordRsp;
//     CuPlayPlatformRecord(cCuPlayPlatformRecordReq, cCuPlayPlatformRecordRsp);
// 
//     rsp.session = req.session;
//     rsp.errorCode = cCuPlayPlatformRecordRsp.GetErrorCode();
// 
//     return SOAP_OK;
// }

//会话内消息
int cusdkService::VcrCtrl(cusdk__VcrCtrlReq req, cusdk__VcrCtrlRsp &rsp)
{
    CCuVcrCtrlReq cCuVcrCtrlReq;
    cCuVcrCtrlReq.SetSession(req.session);
    cCuVcrCtrlReq.SetPlayRate(req.playSpeed);
    cCuVcrCtrlReq.SetSeekTime(req.seekTime);
  //  cCuVcrCtrlReq.SetPlaybackSessUri(req.recordPlaySession);
    cCuVcrCtrlReq.SetPlayId(req.playId);

    switch(req.vcrCmd)
    {
    case cusdk__VCRCMD_START:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_START);
        }
        break;
    case cusdk__VCRCMD_STOP:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_STOP);
        }
        break;
    case cusdk__VCRCMD_PAUSE:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_PAUSE);
        }
        break;
    case cusdk__VCRCMD_RESUME:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_RESUME);
        }
        break;
    case cusdk__VCRCMD_SEEK:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_SEEK);
        }
        break;
    case cusdk__VCRCMD_FASTPLAY:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_FASTPLAY);
        }
        break;
    case cusdk__VCRCMD_SLOWPLAY:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_SLOWPLAY);
        }
        break;
    case cusdk__VCRCMD_ONEFRAME:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_ONEFRAME);
        }
        break;
    case cusdk__VCRCMD_KEYFRAME:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_KEYFRAME);
        }
        break;
    case cusdk__VCRCMD_UNKNOWN:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_UNKNOWN);
        }
        break;
    case cusdk__VCRCMD_REVERSEPLAY:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_REVERSEPLAY);
        }
        break;
    case cusdk__VCRCMD_FORWARDPLAY:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_FORWARDPLAY);
        }
        break;
    case cusdk__VCRCMD_KEYSEEK:
        {
            cCuVcrCtrlReq.SetVcrCmd(VCRCMD_KEYSEEK);
        }
		break;
	case cusdk__VCRCMD_AUTO_KEYFRAME:
		{
			cCuVcrCtrlReq.SetVcrCmd(VCRCMD_AUTO_KEYFRAME);
		}
		break;
	case cusdk__VCRCMD_FORCE_KEYFRAME:
		{
			cCuVcrCtrlReq.SetVcrCmd(VCRCMD_FORCE_KEYFRAME);
		}
		break;
	case cusdk__VCRCMD_FORCE_ALLFRAME:
		{
			cCuVcrCtrlReq.SetVcrCmd(VCRCMD_FORCE_ALLFRAME);
		}
		break;
    case cusdk__VCRCMD_INVALID:
    default:
        break;
    }

    CCuVcrCtrlRsp cCuVcrCtrlRsp;
    CuVcrCtrl(cCuVcrCtrlReq, cCuVcrCtrlRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuVcrCtrlRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetCurRecordPlayPosition(cusdk__GetCurRecordPlayPositionReq req, cusdk__GetCurRecordPlayPositionRsp &rsp)
{
    CCuPlaybackProgressQueryReq cCuReplayProgressQueryReq;
    cCuReplayProgressQueryReq.SetSession(req.session);
//    cCuReplayProgressQueryReq.SetPlaybackSessUri(req.recordPlaySession);
    cCuReplayProgressQueryReq.SetPlayId(req.playId);

    CCuPlaybackProgressQueryRsp cCuReplayProgressQueryRsp;
    CuGetCurRecordPlayPosition(cCuReplayProgressQueryReq, cCuReplayProgressQueryRsp);

    rsp.session = req.session;
    rsp.playId = req.playId;
    rsp.errorCode = cCuReplayProgressQueryRsp.GetErrorCode();
 //   rsp.recordPlaySession = req.recordPlaySession;
    rsp.recordTimeRange.startTime = cCuReplayProgressQueryRsp.GetStartPlayTime();
    rsp.recordTimeRange.endTime = cCuReplayProgressQueryRsp.GetStartPlayTime() + cCuReplayProgressQueryRsp.GetTotalPlayTime();
    rsp.curTime = cCuReplayProgressQueryRsp.GetCurPlayTime();

    return SOAP_OK;
}

// int cusdkService::StopPlayPlatformRecord(cusdk__StopPlayPlatformRecordReq req, cusdk__StopPlayPlatformRecordRsp &rsp)
// {
//     CCuStopPlatformRecordReq cCuStopPlatformRecordReq;
//     cCuStopPlatformRecordReq.SetSession(req.session);
//     cCuStopPlatformRecordReq.SetPlayId(req.playId);
// 
//     CCuStopPlatformRecordRsp cCuStopPlatformRecordRsp;
//     CuStopPlayPlatformRecord(cCuStopPlatformRecordReq, cCuStopPlatformRecordRsp);
// 
//     rsp.session = req.session;
//     rsp.errorCode = cCuStopPlatformRecordRsp.GetErrorCode();
// 
//     return SOAP_OK;
// }

//int cusdkService::SetupDownloadPlatformRecord(cusdk__StopDownloadPlatformRecordReq req, cusdk__StopDownloadPlatformRecordRsp& rsp)
//{
//    CCuDownloadPlatformRecordReq cCuDownloadPlatformRecordReq;
//    cCuDownloadPlatformRecordReq.SetDomainName(req.recordPlayDuration.recordDomainName);
//    CInviteReq cSetupStreamReq;
//    cSetupStreamReq.SetSession(req.session);
//
//    //源是PU
//    TChannel tSrcChn;
//    tSrcChn.SetDevUri(req.recordPlayDuration.encoderChn.deviceId);
//    tSrcChn.SetChnNO(req.recordPlayDuration.encoderChn.channelId);
//    cSetupStreamReq.SetSrcChn(tSrcChn);
//    //目的是CU
//    TChannel tCuChn;
//    tCuChn.SetDevUri(req.session);
//    tCuChn.SetChnNO(req.playId);
//    cSetupStreamReq.SetDstChn(tCuChn);
//
//    TSdp tSdp;
//    tSdp.SetOwner(tCuChn);
//    tSdp.SetSessionName(SDP_SESSION_DOWNLOAD);
//
//    TSdpUri tSdpUri;
//    tSdpUri.SetRecToken(req.recordPlayDuration.recordDurationId);
//    //只有前端放像或者平台放像才要填写
//    tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PLATFORM);
//    tSdpUri.SetPuChn(tSrcChn);
//    tSdp.SetUri(tSdpUri);
//
//    TTimeRange tTimeRange;
//    CCMSTime begin(req.recordPlayDuration.durationTimeRange.startTime);
//    CCMSTime end(req.recordPlayDuration.durationTimeRange.endTime);
//    tTimeRange.SetStartTime(begin.ToString());
//    tTimeRange.SetEndTime(end.ToString());
//    tSdp.SetTimeRange(tTimeRange);
//    //rsp消息中的TSdp才携带
//    //TSdp.SetPlaybackSessUri(req.recordPlayDuration.);
//
//    //遍历每个CU的IP
//    ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());
//
//    cSetupStreamReq.SetSdp(tSdp);
//    cCuDownloadPlatformRecordReq.SetReq(cSetupStreamReq);
//
//    CCuDownloadPlatformRecordRsp cCuDownloadPlatformRecordRsp;
//    CuSetupDownloadPlatformRecord( cCuDownloadPlatformRecordReq, cCuDownloadPlatformRecordRsp );
//
//    rsp.session   = req.session;
//    rsp.errorCode = cCuDownloadPlatformRecordRsp.GetErrorCode();
//
//    if ( CMS_SUCCESS == cCuDownloadPlatformRecordRsp.GetErrorCode() )
//    {      
//        rsp.recordFilePath = cCuDownloadPlatformRecordRsp.GetSdp().GetDLFimePath();
//        rsp.recordFileName = cCuDownloadPlatformRecordRsp.GetSdp().GetDLFimeName();
//
//        ConverTempMediaDescListCuiToCu(cCuDownloadPlatformRecordRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList);
//  }
//
//    return SOAP_OK;
//}
//
//int cusdkService::StartDownloadPlatformRecord(cusdk__StartDownloadPlatformRecordReq req, cusdk__StartDownloadPlatformRecordRsp &rsp)
//{
//    CCuStartDownloadPlatformRecordReq cCuStartDownloadPlatformRecordReq;
//    cCuStartDownloadPlatformRecordReq.SetSession(req.session);
//    cCuStartDownloadPlatformRecordReq.SetPlayId(req.playId);
//
//    CCuStartDownloadPlatformRecordRsp cCuStartDownloadPlatformRecordRsp;
//    CuStartDownloadPlatformRecord(cCuStartDownloadPlatformRecordReq, cCuStartDownloadPlatformRecordRsp);
//
//    rsp.session = req.session;
//    rsp.errorCode = cCuStartDownloadPlatformRecordRsp.GetErrorCode();
//
//    return SOAP_OK;
//}
//
//int cusdkService::StopDownloadPlatformRecord(cusdk__StopDownloadPlatformRecordReq req, cusdk__StopDownloadPlatformRecordRsp &rsp)
//{
//    CCuStopDownloadPlatformRecordReq cCuStopDownloadPlatformRecordReq;
//    cCuStopDownloadPlatformRecordReq.SetSession(req.session);
//    cCuStopDownloadPlatformRecordReq.SetPlayId(req.playId);
//
//    CCuStopDownloadPlatformRecordRsp cCuStopDownloadPlatformRecordRsp;
//    CuStopDownloadPlatformRecord(cCuStopDownloadPlatformRecordReq, cCuStopDownloadPlatformRecordRsp);
//
//    rsp.errorCode = cCuStopDownloadPlatformRecordRsp.GetErrorCode();
//    rsp.session = req.session;
//
//    return SOAP_OK;
//}

//todo 李之兴,查询接口对应不上
int cusdkService::GetRecordPlan(cusdk__GetRecordPlanReq req, cusdk__GetRecordPlanRsp &rsp)
{
    //由于cu这边是查询一个，rcs是条件查询有偏差 
    CCuRcsQueryRecordParamReq cCuRcsQueryRecordParamReq;
    cCuRcsQueryRecordParamReq.SetSession(req.session);
    cCuRcsQueryRecordParamReq.SetExpectEntryNum(1);
    cCuRcsQueryRecordParamReq.SetStartEntryIndex(0);

    TChannel tchn;
    tchn.SetChnNO(req.devChn.channelId);
    tchn.SetDevUri(req.devChn.deviceId);
    cCuRcsQueryRecordParamReq.SetPuChn(tchn);

    CCuRcsQueryRecordParamRsp cCuRcsQueryRecordParamRsp;
    CuGetRecordPlan(cCuRcsQueryRecordParamReq, cCuRcsQueryRecordParamRsp);

    //CMS_ASSERT(cCuRcsQueryRecordParamRsp.GetRecordParamList().size() != 0);

    //由于cu这边是查询一个，rcs是条件查询有偏差 
    TRecordParamList::const_iterator item = cCuRcsQueryRecordParamRsp.GetRecordParamList().begin();
    while(item != cCuRcsQueryRecordParamRsp.GetRecordParamList().end())
    {
        rsp.devChn.channelId =  item->GetPuChn().GetChnNO();
        rsp.devChn.deviceId = item->GetPuChn().GetDevUri();


        if(item->GetRecMode() == RCS_RECMODE_ALL)
        {
            rsp.recordPlan.recordMode = cusdk__RS_RECMODE_ALL;
        }
        else if(item->GetRecMode() == RCS_RECMODE_ONLYVIDEO)
        {
            rsp.recordPlan.recordMode = cusdk__RS_RECMODE_ONLY_VIDEO;
        }
        else if(item->GetRecMode() == RCS_RECMODE_ONLYKEYFRAME)
        {
            rsp.recordPlan.recordMode = cusdk__RS_RECMODE_ONLY_KEYFRAME;
        }
        else if(item->GetRecMode() == RCS_RECMODE_UNKNOWN)
        {

        }

        rsp.recStreamType = item->GetRecStreamType();
        rsp.supportChangeStreamType = item->GetIsSupportChangeStreamType();

        rsp.recordPlan.supportSetRecDuration = item->GetIsSupportRecStorageDuration();
        rsp.recordPlan.recStorageDuration = item->GetRecStorageDuration();
        rsp.recordPlan.recDuraionEnabled = item->GetIsRecStorageDurationEnabled();

        rsp.recordPlan.recordDurationAfterAlarm = item->GetAlarmRecDelayStopTime();
        rsp.recordPlan.prerecordDuration = item->GetPreRecDuration();
        ConverTempCronTimeCuiToCu(item->GetPlanTimeList(), rsp.recordPlan.planedRecordCronTimeList);

        item++;
    }

    rsp.session = req.session;
    rsp.errorCode = cCuRcsQueryRecordParamRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetRecordPlan(cusdk__SetRecordPlanReq req, cusdk__SetRecordPlanRsp &rsp)
{
    CCuRcsSetRecordParamReq cCuRcsSetRecordParamReq;
    cCuRcsSetRecordParamReq.SetSession(req.session);
    TRecordParam recordParam;

    TChannel puChn;
    puChn.SetChnNO(req.devChn.channelId);
    puChn.SetDevUri(req.devChn.deviceId);
    recordParam.SetPuChn(puChn);

    if(req.recordPlan.recordMode == cusdk__RS_RECMODE_ALL)
    {
        recordParam.SetRecMode(RCS_RECMODE_ALL);
    }
    else if(req.recordPlan.recordMode == cusdk__RS_RECMODE_ONLY_VIDEO)
    {
        recordParam.SetRecMode(RCS_RECMODE_ONLYVIDEO);
    }
    else if(req.recordPlan.recordMode == cusdk__RS_RECMODE_ONLY_KEYFRAME)
    {
        recordParam.SetRecMode(RCS_RECMODE_ONLYKEYFRAME);
    }
    else
    {
        recordParam.SetRecMode(RCS_RECMODE_UNKNOWN);
    }

    recordParam.SetRecStreamType(req.recStreamType);
    recordParam.SetIsSupportChangeStreamType(req.supportChangeStreamType);

    recordParam.SetIsSupportRecStorageDuration(req.recordPlan.supportSetRecDuration);
    recordParam.SetRecStorageDuration(req.recordPlan.recStorageDuration);
    recordParam.SetIsRecStorageDurationEnabled(req.recordPlan.recDuraionEnabled);

    recordParam.SetAlarmRecDelayStopTime(req.recordPlan.recordDurationAfterAlarm);
    recordParam.SetPreRecDuration(req.recordPlan.prerecordDuration);
    //recordParam.SetIsHandleRecOn(false);
    recordParam.SetIsManualRecOn(false);
    ConverTempCronTimeCuToCui(req.recordPlan.planedRecordCronTimeList, recordParam.GetPlanTimeList());

    cCuRcsSetRecordParamReq.SetRecordParam(recordParam);

    CCuRcsSetRecordParamRsp cCuRcsSetRecordParamRsp;
    CuSetRecordPlan(cCuRcsSetRecordParamReq, cCuRcsSetRecordParamRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRcsSetRecordParamRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetTVWall(cusdk__GetTVWallReq req, cusdk__GetTVWallRsp &rsp)
{
    //接口暂无
    CCuTvwallQryReq cCuTvwallQryReq;
    cCuTvwallQryReq.SetSession(req.session);

    CTvwallInfoQueryCondition condition;
    condition.SetTvwallId(req.tvwallId);
    cCuTvwallQryReq.SetCondition(condition);

    CCuTvwallQryRsp cCuTvwallQryRsp;
    CuGetAllTVWall(cCuTvwallQryReq, cCuTvwallQryRsp);

    std::vector<cusdk__TVWall > tvWallList;
    ConverTempTvwallInfoFromCuiToCu(cCuTvwallQryRsp.GetTvwallInfo(), tvWallList);

    rsp.errorCode = cCuTvwallQryRsp.GetErrorCode();

    if( 0 != tvWallList.size())
    {
        rsp.tvWallInfo = tvWallList.front();
    }
	else
	{
		rsp.errorCode = ERR_TAS_RECORD_NOT_EXISTS;
	}
    
    rsp.session = req.session;

    return SOAP_OK;
}

//todo 创建时间由3as生成，3as并没有给，这边cui即时生成可能不准确
int cusdkService::CreateTVWall(cusdk__CreateTVWallReq req, cusdk__CreateTVWallRsp &rsp)
{
    CCuTvwallAddReq cCuTvwallAddReq;
    cCuTvwallAddReq.SetSession(req.session);
    CTvwallInfo tvwallInfo;

    tvwallInfo.SetDeviceUUID(req.TVWallInfo.tvwallId);
    tvwallInfo.SetDomainUUID(req.TVWallInfo.domainId);
    tvwallInfo.SetDeviceName(req.TVWallInfo.name);
    tvwallInfo.SetTvNum(req.TVWallInfo.tvNum);
    tvwallInfo.SetTvwallStyle(req.TVWallInfo.tvwallStyle);
    tvwallInfo.SetTvwallCustomStyleData(req.TVWallInfo.tvwallCustomStyleData);
    tvwallInfo.SetDescription(req.TVWallInfo.desc);
    CCMSTime tStartTime(req.TVWallInfo.createTime);
    tvwallInfo.SetCreateDate(tStartTime.ToString());


    CCMSTime tEndTime("2037-12-31T21:32:52");
    tvwallInfo.SetExpireDate(tEndTime.ToString());

    ConverTempTvDecoderBindersCuToCui(req.TVWallInfo.tvDecoderBind, tvwallInfo.GetTvDecoderBinders());
    cCuTvwallAddReq.SetTvwallInfo(tvwallInfo);

    CCuTvwallAddRsp cCuTvwallAddRsp;
    CCuCreatTVWall(cCuTvwallAddReq, cCuTvwallAddRsp);

    //todo 创建时间由3as生成，3as并没有给，这边cui即时生成可能不准确
    rsp.session = req.session;
    time_t currentTime = time(NULL);
    rsp.creatTime = currentTime;
    rsp.TVWallId = cCuTvwallAddRsp.GetTvwallId();
    rsp.errorCode = cCuTvwallAddRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::DelTVWall(cusdk__DelTVWallReq req, cusdk__DelTVWallRsp &rsp)
{
    CCuTvwallDelReq cCuTvwallDelReq;
    cCuTvwallDelReq.SetSession(req.session);
    cCuTvwallDelReq.SetTvwallId(req.TVWallId);

    CCuTvwallDelRsp cCuTvwallDelRsp;
    CuDelTVWall(cCuTvwallDelReq, cCuTvwallDelRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvwallDelRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::ModifyTVWall(cusdk__ModifyTVWallReq req, cusdk__ModifyTVWallRsp &rsp)
{
    CCuTvwallModReq cCuTvwallModReq;
    cCuTvwallModReq.SetSession(req.session);

    cCuTvwallModReq.SetId(req.TVWallInfo.tvwallId);
    cCuTvwallModReq.SetName(req.TVWallInfo.name);
    cCuTvwallModReq.SetTvNum(req.TVWallInfo.tvNum);
    cCuTvwallModReq.SetStyle(req.TVWallInfo.tvwallStyle);
    cCuTvwallModReq.SetCustomStyleData(req.TVWallInfo.tvwallCustomStyleData);
    cCuTvwallModReq.SetDesc(req.TVWallInfo.desc);

    vector<TvDecoderBinder> var;
    ConverTempTvDecoderBindersCuToCui(req.TVWallInfo.tvDecoderBind, var);
    cCuTvwallModReq.SetTvDecoderBinders(var);

    CCuTvwallModRsp cCuTvwallModRsp;
    CuModifyTVWall(cCuTvwallModReq, cCuTvwallModRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvwallModRsp.GetErrorCode();

    return SOAP_OK;
}

//todo 当前电视墙放像只是在当前放像，不跨域
int cusdkService::PlayRecordOnTVWall(cusdk__PlayRecordOnTVWallReq req, cusdk__PlayRecordOnTVWallRsp &rsp)
{   
    CCuRecordOnTvReq cCuRecOnTvReq;
    cCuRecOnTvReq.SetSession(req.session);

    TChannel encChn;
    encChn.SetChnNO(req.recordPlayDuration.encoderChn.channelId);
    encChn.SetDevUri(req.recordPlayDuration.encoderChn.deviceId);
    cCuRecOnTvReq.SetEncChn(encChn);

    TTvChannel tvChn;
    tvChn.SetTvDivId(req.tvDivId);
    tvChn.SetTvId(req.tvId);
    tvChn.SetTvWallId(req.TVWallId);
    cCuRecOnTvReq.SetTvChn(tvChn);

	TSdp tSdp;
	TSdpUri tSdpUri;
	tSdp.SetUri(tSdpUri);
	tSdp.CreateEmptyMediaList();
	ConverMediaAddrCuToCui(req,tSdp.GetMediaDescList());
	TStreamTag tStreamTag;
	tStreamTag.SetMediaStreamType(req.localMediaPortInfo.mediaStreamType);
	tStreamTag.SetManufacture(req.localMediaPortInfo.manufacture);
	tSdp.SetStreamTag(tStreamTag);
	cCuRecOnTvReq.SetSdp(tSdp);
    /*TTimeRange timeRange;
    CCMSTime begin(req.recordPlayDuration.durationTimeRange.startTime);
    timeRange.SetStartTime(begin.ToString());
    CCMSTime end(req.recordPlayDuration.durationTimeRange.endTime);
    timeRange.SetEndTime(end.ToString());
    cCuPlatRecOnTvReq.SetTimeRange(timeRange);
    cCuPlatRecOnTvReq.SetRecDurationToken(req.recordPlayDuration.recordDurationId);*/

    CCuRecordOnTvRsp cCuRecOnTvRsp;
    CuPlayRecordOnTVWall(cCuRecOnTvReq, cCuRecOnTvRsp);

	ConverDecoderSdpToCuAddr(cCuRecOnTvRsp.GetSdp(),rsp);
    rsp.session = req.session;
    rsp.errorCode = cCuRecOnTvRsp.GetErrorCode();

    return SOAP_OK;
}

s32 cusdkService::StopPlayRecordOnTVWall(cusdk__StopPlayRecordOnTVWallReq req, cusdk__StopPlayRecordOnTVWallRsp &rsp)
{
    CCuRecordOffTvReq cCuRecOffTvReq;
    cCuRecOffTvReq.SetSession(req.session);

    TTvChannel tvch;
    tvch.SetTvDivId(req.tvDivId);
    tvch.SetTvId(req.tvId);
    tvch.SetTvWallId(req.TVWallId);

    cCuRecOffTvReq.SetTvChn(tvch);

    CCuRecordOffTvRsp cCuRecOffTvRsp;
    CuStopPlayRecordOnTVWall(cCuRecOffTvReq, cCuRecOffTvRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRecOffTvRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetDomainList(cusdk__GetDomainListReq req,cusdk__GetDomainListRsp &rsp)
{
    CCuDomainRelationQryReq cCuDomainRelationQryReq;
    cCuDomainRelationQryReq.SetSession(req.session);

    CCuDomainRelationQryRsp cCuDomainRelationQryRsp;
    CuGetDomainList(cCuDomainRelationQryReq, cCuDomainRelationQryRsp);

    ConverTempDomainInfoFromCuiToCu(cCuDomainRelationQryRsp.GetDomainRelationInfo(), rsp.domainList);
    rsp.errorCode = cCuDomainRelationQryRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

int cusdkService::GetAllTVWall(cusdk__GetAllTVWallReq req,cusdk__GetAllTVWallRsp &rsp)
{
    CCuTvwallQryReq cCuTvwallQryReq;
    cCuTvwallQryReq.SetSession(req.session);
    cCuTvwallQryReq.SetExpectEntryNum(req.num);
    cCuTvwallQryReq.SetStartEntryIndex(req.beginIndex);

    CCuTvwallQryRsp cCuTvwallQryRsp;
    CuGetAllTVWall(cCuTvwallQryReq, cCuTvwallQryRsp);

    ConverTempTvwallInfoFromCuiToCu(cCuTvwallQryRsp.GetTvwallInfo(), rsp.tvWallList);

    rsp.session = req.session;
    rsp.errorCode = cCuTvwallQryRsp.GetErrorCode();
    rsp.totalNum = cCuTvwallQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::TVWallStartPlay(cusdk__TVWallStartPlayReq req,cusdk__TVWallStartPlayRsp &rsp)
{
    CCuRealMediaOnTvReq cCuRealMediaOnTvReq;
    cCuRealMediaOnTvReq.SetSession(req.session);

    TTvChannel tTvChn;
    tTvChn.SetTvDivId(req.tvDivId);
    tTvChn.SetTvId(req.tvId);
    tTvChn.SetTvWallId(req.TVWallId);
    cCuRealMediaOnTvReq.SetTvChn(tTvChn); 

    TChannel tEncChn;
    tEncChn.SetChnNO(req.encoderChn.channelId);
    tEncChn.SetDevUri(req.encoderChn.deviceId);
    cCuRealMediaOnTvReq.SetEncChn(tEncChn);

    //cu操作都是手动的
    cCuRealMediaOnTvReq.SetTvOccurType(TV_OCCUR_TYPE_HAND);

    CCuRealMediaOnTvRsp cCuRealMediaOnTvRsp;
    CuTVWallStartPlay(cCuRealMediaOnTvReq, cCuRealMediaOnTvRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRealMediaOnTvRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallStopPlay(cusdk__TVWallStopPlayReq req,cusdk__TVWallStopPlayRsp &rsp)
{
    CCuRealMediaOffTvReq cCuRealMediaOffTvReq;
    cCuRealMediaOffTvReq.SetSession(req.session);

    TChannel tDecChn;
    tDecChn.SetChnNO(req.encoderChn.channelId);
    tDecChn.SetDevUri(req.encoderChn.deviceId);
    cCuRealMediaOffTvReq.SetEncChn(tDecChn);

    TTvChannel tTvChn;
    tTvChn.SetTvDivId(req.tvDivId);
    tTvChn.SetTvId(req.tvId);
    tTvChn.SetTvWallId(req.TVWallId);
    cCuRealMediaOffTvReq.SetTvChn(tTvChn);

    //cu操作都是手动的
    cCuRealMediaOffTvReq.SetTvOccurType(TV_OCCUR_TYPE_HAND);

    CCuRealMediaOffTvRsp cCuRealMediaOffTvRsp;
    CuTVWallStopPlay(cCuRealMediaOffTvReq, cCuRealMediaOffTvRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuRealMediaOffTvRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallWindowPollContrl(cusdk__TVWallWindowPollContrlReq req, cusdk__TVWallWindowPollContrlRsp &rsp)
{
    CCuTvPollOperateReq cCuTvPollOperateReq;
    cCuTvPollOperateReq.SetSession(req.session);

    TTvChannel tTvChn;
    tTvChn.SetTvDivId(req.tvDivId);
    tTvChn.SetTvId(req.tvId);
    tTvChn.SetTvWallId(req.TVWallId);
    cCuTvPollOperateReq.SetTvChn(tTvChn);

    switch(req.pollCmd)
    {
    case cusdk__VCRCMD_START:
        {
            cCuTvPollOperateReq.SetOprType(POLL_OPERATE_START);
        }
        break;
    case cusdk__VCRCMD_STOP:
        {
            cCuTvPollOperateReq.SetOprType(POLL_OPERATE_STOP);
        }
        break;
    case cusdk__VCRCMD_PAUSE:   
        {
            cCuTvPollOperateReq.SetOprType(POLL_OPERATE_PAUSE);
        }
        break;
    case cusdk__VCRCMD_RESUME:
        {
            cCuTvPollOperateReq.SetOprType(POLL_OPERATE_RESUME);
        }
        break;
    case cusdk__VCRCMD_CONFIG:
        {
            cCuTvPollOperateReq.SetOprType(POLL_OPERATE_CONFIG);
        }
        break;
    case cusdk__VCRCMD_SEEK:
    case cusdk__VCRCMD_FASTPLAY:
    case cusdk__VCRCMD_SLOWPLAY:
    case cusdk__VCRCMD_ONEFRAME:
    case cusdk__VCRCMD_KEYFRAME:
    case cusdk__VCRCMD_REVERSEPLAY:
    case cusdk__VCRCMD_FORWARDPLAY:
    case cusdk__VCRCMD_KEYSEEK:
	case cusdk__VCRCMD_AUTO_KEYFRAME:
	case cusdk__VCRCMD_FORCE_KEYFRAME:
	case cusdk__VCRCMD_FORCE_ALLFRAME:
    case cusdk__VCRCMD_UNKNOWN:
    default:
        {

        }
        break;
    }

    ConverTempTvPollFromCuToCui(req.pollSteps, cCuTvPollOperateReq.GetPollList());

    CCuTvPollOperateRsp cCuTvPollOperateRsp;
    CuTVWallWindowPollContrl(cCuTvPollOperateReq, cCuTvPollOperateRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvPollOperateRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallSaveScheme(cusdk__TVWallSaveSchemeReq req,cusdk__TVWallSaveSchemeRsp &rsp)
{
    CCuTvWallSchemeSaveReq cCuTvWallSchemeSaveReq;
    cCuTvWallSchemeSaveReq.SetSession(req.session);

    TTvWallScheme         scheme;
//     switch(req.scheme.schemeType)
//     {
//     case cusdk__SchemePrivate:
//         {
//             scheme.SetOwnerType(TVWALL_SCHEME_OWNER_TYPE_PRIVATE);
//         }
//         break;
//     case cusdk__SchemePublic:
//         {
//             scheme.SetOwnerType(TVWALL_SCHEME_OWNER_TYPE_PUBLIC);
//         }
//         break;
//     }
// 
//     scheme.SetSchemeId(req.scheme.schemeId);
    scheme.SetSchemeName(req.scheme.schemeName);
    scheme.SetTvWallId(req.scheme.tvWallId);
    //todo 以下后续由cuiserver来填写
    //scheme.SetUserId();
    ConverTempTvWinFromCuToCui(req.scheme.tvSchemeList, scheme.GetTvWinList());

	string strSchema = scheme.ToXml();
	u8* abyBuff= NULL;
	uLong dwBuffLen = compressBound(strSchema.length()); 
	if((abyBuff = (u8*)malloc(sizeof(u8) * dwBuffLen)) == NULL)  
	{  
		rsp.session = req.session;
		rsp.errorCode = ERR_TVS_DB_OPR_FAIL;
		return SOAP_OK;  
	}  
	if (compress(abyBuff, &dwBuffLen,(u8*)strSchema.c_str(),strSchema.length()) != Z_OK)
	{
		free(abyBuff);
		rsp.session = req.session;
		rsp.errorCode = ERR_TVS_DB_OPR_FAIL;
		return SOAP_OK;
	}

	string strZipString((char*)abyBuff,dwBuffLen);
	strZipString = Base64Encode(strZipString);

    cCuTvWallSchemeSaveReq.SetScheme(strZipString);

    CCuTvWallSchemeSaveRsp cCuTvWallSchemeSaveRsp;
    CuTVWallSaveScheme(cCuTvWallSchemeSaveReq, cCuTvWallSchemeSaveRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvWallSchemeSaveRsp.GetErrorCode();
    //rsp.schemeId = cCuTvWallSchemeSaveRsp.GetSchemeId();

	free(abyBuff);
    return SOAP_OK;
}

int cusdkService::TVWallDelScheme(cusdk__TVWallDelSchemeReq req,cusdk__TVWallDelSchemeRsp &rsp)
{
    CCuTvWallSchemeDelReq cCuTvWallSchemeDelReq;
    cCuTvWallSchemeDelReq.SetSession(req.session);

    cCuTvWallSchemeDelReq.SetTvWallId(req.tvwallId);
    CTvWallSchemeNameList  schemeNameList;
    schemeNameList.push_back(req.schemeName);
    cCuTvWallSchemeDelReq.SetSchemeNameList(schemeNameList);

    CCuTvWallSchemeDelRsp cCuTvWallSchemeDelRsp;
    CuTVWallDelScheme(cCuTvWallSchemeDelReq, cCuTvWallSchemeDelRsp);

    rsp.session = req.session; 
    rsp.errorCode = cCuTvWallSchemeDelRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallLoadScheme(cusdk__TVWallLoadSchemeReq req,cusdk__TVWallLoadSchemeRsp &rsp)
{
    CCuTvWallSchemeLoadReq cCuTvWallSchemeLoadReq;
    cCuTvWallSchemeLoadReq.SetSchemeName(req.schemeName);
    cCuTvWallSchemeLoadReq.SetSession(req.session);
    cCuTvWallSchemeLoadReq.SetTvWallId(req.tvwallId);

    CCuTvWallSchemeLoadRsp cCuTvWallSchemeLoadRsp;
    CuTVWallLoadScheme(cCuTvWallSchemeLoadReq, cCuTvWallSchemeLoadRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvWallSchemeLoadRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallQueryScheme(cusdk__TVWallQuerySchemeReq req,cusdk__TVWallQuerySchemeRsp &rsp)
{
    CCuTvWallSchemeQueryReq cCuTvWallSchemeQueryReq;
    cCuTvWallSchemeQueryReq.SetSession(req.session);
    cCuTvWallSchemeQueryReq.SetTvWallId(req.tvWallId);
    //todo 后续由cuiserver填写
    //cCuTvWallSchemeQueryReq.SetUserId();

    CCuTvWallSchemeQueryRsp cCuTvWallSchemeQueryRsp;
    CuTVWallQueryScheme(cCuTvWallSchemeQueryReq, cCuTvWallSchemeQueryRsp);

    ConverTempTvWallSchemeFromCuiToCu(cCuTvWallSchemeQueryRsp.GetTvWallScheme(), rsp.schemeList);
    rsp.session = req.session;
    rsp.errorCode = cCuTvWallSchemeQueryRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallSetSchemePoll(cusdk__TVWallSetSchemePollReq req,cusdk__TVWallSetSchemePollRsp &rsp)
{
    CCuTvWallSchemeGroupSetReq cCuTvWallSchemeGroupSetReq;
    cCuTvWallSchemeGroupSetReq.SetSession(req.session);
    TTvWallSchemeGroup schemeGroup;
    schemeGroup.SetTvWallId(req.tvWallId);

    ConverTempvWallSchemePollCuToCui(req.pollSteps, schemeGroup.GetPollList());
    cCuTvWallSchemeGroupSetReq.SetSchemeGroup(schemeGroup);

    CCuTvWallSchemeGroupSetRsp cCuTvWallSchemeGroupSetRsp;
    CuTVWallSetSchemePoll(cCuTvWallSchemeGroupSetReq, cCuTvWallSchemeGroupSetRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvWallSchemeGroupSetRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallGetSchemePoll(cusdk__TVWallGetSchemePollReq req,cusdk__TVWallGetSchemePollRsp &rsp)
{
    CCuTvWallSchemeGroupGetReq cCuTvWallSchemeGroupGetReq;
    cCuTvWallSchemeGroupGetReq.SetSession(req.session);
    cCuTvWallSchemeGroupGetReq.SetTvWallId(req.tvWallId);

    CCuTvWallSchemeGroupGetRsp cCuTvWallSchemeGroupGetRsp;
    CuTVWallGetSchemePoll(cCuTvWallSchemeGroupGetReq, cCuTvWallSchemeGroupGetRsp);

    ConverTempTvWallSchemePollCuiToCu(cCuTvWallSchemeGroupGetRsp.GetSchemeGroup().GetPollList(), rsp.pollSteps);
    rsp.session = cCuTvWallSchemeGroupGetRsp.GetSession();
    rsp.errorCode = cCuTvWallSchemeGroupGetRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::TVWallSchemePollControl(cusdk__TVWallSchemePollControlReq req,cusdk__TVWallSchemePollControlRsp &rsp)
{
    CCuTvWallSchemePollOperateReq cCuTvWallSchemePollOperateReq;
    cCuTvWallSchemePollOperateReq.SetSession(req.session);
    cCuTvWallSchemePollOperateReq.SetTvWallId(req.tvWallId);

    switch(req.pollCmd)
    {
    case cusdk__VCRCMD_START:
        {
            cCuTvWallSchemePollOperateReq.SetOprType(POLL_OPERATE_START);
        }
        break;
    case cusdk__VCRCMD_STOP:
        {
            cCuTvWallSchemePollOperateReq.SetOprType(POLL_OPERATE_STOP);
        }
        break;
    case cusdk__VCRCMD_PAUSE:
        {
            cCuTvWallSchemePollOperateReq.SetOprType(POLL_OPERATE_PAUSE);
        }
        break;
    case cusdk__VCRCMD_RESUME:
        {
            cCuTvWallSchemePollOperateReq.SetOprType(POLL_OPERATE_RESUME);
        }
        break;
    case cusdk__VCRCMD_SEEK:
    case cusdk__VCRCMD_FASTPLAY:
    case cusdk__VCRCMD_SLOWPLAY:
    case cusdk__VCRCMD_ONEFRAME:
    case cusdk__VCRCMD_KEYFRAME:
    case cusdk__VCRCMD_REVERSEPLAY:
    case cusdk__VCRCMD_FORWARDPLAY:
	case cusdk__VCRCMD_KEYSEEK:
	case cusdk__VCRCMD_AUTO_KEYFRAME:
	case cusdk__VCRCMD_FORCE_KEYFRAME:
	case cusdk__VCRCMD_FORCE_ALLFRAME:
    case cusdk__VCRCMD_UNKNOWN:
    default:
        {

        }
        break;
    }

    CCuTvWallSchemePollOperateRsp cCuTvWallSchemePollOperateRsp;
    CuTVWallSchemePollControl(cCuTvWallSchemePollOperateReq, cCuTvWallSchemePollOperateRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuTvWallSchemePollOperateRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::CreateMap(cusdk__CreateMapReq req,cusdk__CreateMapRsp &rsp)
{
    CCuMapCreateReq cCuMapCreateReq;
    cCuMapCreateReq.SetSession(req.session);

    MapInfo mapInfo;
    mapInfo.SetMapId(req.mapInfo.mapId);
    mapInfo.SetMapName(req.mapInfo.mapName);
    switch(req.mapInfo.mapType)
    {
    case cusdk__MapTypeGoogle:
        {
            mapInfo.SetMapType(MAP_GOOGLE);
        }
        break;
    case cusdk__MapTypeJpeg:
        {
            mapInfo.SetMapType(MAP_JPEG);
        }
        break;
    default:
        {

        }
        break;
    }

    mapInfo.SetMapUrl(req.mapInfo.mapUrl);
    mapInfo.SetParentMapId(req.mapInfo.parentMapId);
    mapInfo.SetDescription(req.mapInfo.description);

    Bounds tBounds;
    MapPosition leftTop;
    leftTop.SetLatitude(req.mapInfo.minLatitude);
    leftTop.SetLongitude(req.mapInfo.minLongitude);
    MapPosition rightBottom;
    rightBottom.SetLatitude(req.mapInfo.maxLatitude);
    rightBottom.SetLongitude(req.mapInfo.maxLongitude);
    tBounds.SetLeftTop(leftTop);
    tBounds.SetRightBottom(rightBottom);
    mapInfo.SetBounds(tBounds);

    mapInfo.SetMaxScaleLevel(req.mapInfo.maxScaleLevel);
    mapInfo.SetMinScaleLevel(req.mapInfo.minScaleLevel);
    MapPosition tMapPosition;
    tMapPosition.SetLatitude(req.mapInfo.screenCenterLatitude);
    tMapPosition.SetLongitude(req.mapInfo.screenCenterLongitude);
    mapInfo.SetCenterPos(tMapPosition);
    mapInfo.SetDefaultScaleLevel(req.mapInfo.defaultScaleLevel);
    mapInfo.SetVersion(req.mapInfo.version);

    cCuMapCreateReq.SetMapInfo(mapInfo);

    CCuMapCreateRsp cCuMapCreateRsp;
    CuCreateMap(cCuMapCreateReq, cCuMapCreateRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapCreateRsp.GetErrorCode();
    rsp.mapId = cCuMapCreateRsp.GetMapId();

    return SOAP_OK;
}

int cusdkService::ModifyMap(cusdk__ModifyMapReq req,cusdk__ModifyMapRsp &rsp)
{
    CCuMapModReq cCuMapModReq;
    cCuMapModReq.SetSession(req.session);

    MapInfo mapInfo;
    mapInfo.SetMapId(req.mapInfo.mapId);
    mapInfo.SetMapName(req.mapInfo.mapName);
    switch(req.mapInfo.mapType)
    {
    case cusdk__MapTypeGoogle:
        {
            mapInfo.SetMapType(MAP_GOOGLE);
        }
        break;
    case cusdk__MapTypeJpeg:
        {
            mapInfo.SetMapType(MAP_JPEG);
        }
        break;
    default:
        {

        }
        break;
    }

    mapInfo.SetMapUrl(req.mapInfo.mapUrl);
    mapInfo.SetParentMapId(req.mapInfo.parentMapId);
    mapInfo.SetDescription(req.mapInfo.description);

    Bounds tBounds;
    MapPosition leftTop;
    leftTop.SetLatitude(req.mapInfo.minLatitude);
    leftTop.SetLongitude(req.mapInfo.minLongitude);
    MapPosition rightBottom;
    rightBottom.SetLatitude(req.mapInfo.maxLatitude);
    rightBottom.SetLongitude(req.mapInfo.maxLongitude);
    tBounds.SetLeftTop(leftTop);
    tBounds.SetRightBottom(rightBottom);

    mapInfo.SetBounds(tBounds);
    mapInfo.SetMaxScaleLevel(req.mapInfo.maxScaleLevel);
    mapInfo.SetMinScaleLevel(req.mapInfo.minScaleLevel);
    MapPosition tMapPosition;
    tMapPosition.SetLatitude(req.mapInfo.screenCenterLatitude);
    tMapPosition.SetLongitude(req.mapInfo.screenCenterLongitude);
    mapInfo.SetCenterPos(tMapPosition);
    mapInfo.SetDefaultScaleLevel(req.mapInfo.defaultScaleLevel);
    mapInfo.SetVersion(req.mapInfo.version);

    cCuMapModReq.SetMapInfo(mapInfo);

    CCuMapModRsp cCuMapModRsp;
    CuModifyMap(cCuMapModReq, cCuMapModRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapModRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::DelMap(cusdk__DelMapReq req,cusdk__DelMapRsp &rsp)
{
    CCuMapDelReq cCuMapDelReq;
    cCuMapDelReq.SetSession(req.session);
    cCuMapDelReq.SetMapId(req.mapId);

    CCuMapDelRsp cCuMapDelRsp;
    CuDelMap(cCuMapDelReq, cCuMapDelRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapDelRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::QueryMap(cusdk__QueryMapReq req,cusdk__QueryMapRsp &rsp)
{
    CCuMapQryReq cCuMapQryReq;
    cCuMapQryReq.SetSession(req.session);
    cCuMapQryReq.SetStartEntryIndex(req.beginIndex);
    cCuMapQryReq.SetExpectEntryNum(req.num);

    MapCondition condition;
    if(req.mapIdEnabled)
    {
        condition.SetMapId(req.mapId);
    }
    if(req.mapNameEnabled)
    {
        condition.SetMapName(req.mapName);
    }
    if (req.mapTypeEnabled)
    {
        EmMapType emMapType;
        if (cusdk__MapTypeGoogle == req.mapType)
        {
            emMapType = MAP_GOOGLE;
        }
        else if (cusdk__MapTypeJpeg == req.mapType)
        {
            emMapType = MAP_JPEG;
        }
        condition.SetMapType(emMapType);
    }
    if(req.parentMapIdEnabled)
    {
        condition.SetParentMapId(req.parentMapId);
    }

    cCuMapQryReq.SetCondition(condition);

    CCuMapQryRsp cCuMapQryRsp;
    CuQueryMap(cCuMapQryReq, cCuMapQryRsp);

    ConverTempMapCuiToCu(cCuMapQryRsp.GetMapInfos(), rsp.mapList);

    rsp.session = req.session;
    rsp.errorCode = cCuMapQryRsp.GetErrorCode();
    rsp.totalNum = cCuMapQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::CreateMapElement(cusdk__CreateMapElementReq req,cusdk__CreateMapElementRsp &rsp)
{
    CCuMapElementCreateReq cCuMapElementCreateReq;
    cCuMapElementCreateReq.SetSession(req.session);

    MapElement mapElement;
    mapElement.SetId(req.mapEleInfo.id);
    mapElement.SetName(req.mapEleInfo.name);
    mapElement.SetLayerId(req.mapEleInfo.layerId);
	mapElement.SetStoreTrace(req.mapEleInfo.storeGPSTrace);
    //mapElement.SetGroupId(req.mapEleInfo.elementGroupId);

    //todo 接口不一致，要找周昌鸿协商
    switch(req.mapEleInfo.type)
    {
    case cusdk__MapEleCommon:
        {
            mapElement.SetType(MAP_ElEMENT_COMMAN);
        }
        break;
    case cusdk__MapEleMove:
        {
            mapElement.SetType(MAP_ELEMENT_LIVE);
        }
        break;
//     case cusdk__MapEleMainMap:
//         {
//             /*mapElement.SetType();*/
//         }
//         break;
//     case cusdk__MapEleSubMap:
//         {
//             mapElement.SetType(MAP_ELEMENT_SUBMAP);
//         }
//         break;
    default:
        {

        }
        break;
    }

    mapElement.SetDeviceId(req.mapEleInfo.deviceId);
    //mapElement.SetId(req.mapEleInfo.id);
 //   mapElement.SetDeviceDomainId(req.mapEleInfo.deviceDomainId);
    mapElement.SetDeviceChannel(req.mapEleInfo.deviceChannel);

    //todo 接口不一致，要找周昌鸿协商
    switch(req.mapEleInfo.channelType)
    {
    case cusdk__DevChnTypeVideoSource:
        {
            mapElement.SetChannelType(MAP_DEVCHANNEL_VIDEO);
        }
        break;
    case cusdk__DevChnTypeAlarmInput:
        {
            mapElement.SetChannelType(MAP_DEVCHANNEL_ALARMIN);
        }
        break;
    default:
        {

        }
        break;
    }

    if( cusdk__MapCoordEarth == req.mapEleInfo.coordType)
    {
        mapElement.SetEarthCoord(true);
    }
    else if( cusdk__MapCoordMar == req.mapEleInfo.coordType)
    {
        mapElement.SetEarthCoord(false);
    }

    MapPosition tMapPosition;
    //tMapPosition.SetEarthCoord(req.mapEleInfo.position.earthCoord);
    tMapPosition.SetLatitude(req.mapEleInfo.position.latitude);
    tMapPosition.SetLongitude(req.mapEleInfo.position.longitude);
    mapElement.SetPosition(tMapPosition);

	MapPosition tMarsPosition;
	tMarsPosition.SetLatitude(req.mapEleInfo.marPosition.latitude);
	tMarsPosition.SetLongitude(req.mapEleInfo.marPosition.longitude);
	mapElement.SetMarsPosition(tMarsPosition);

    mapElement.SetMapId(req.mapEleInfo.mapId);
    mapElement.SetPicUrl(req.mapEleInfo.picUrl);
    mapElement.SetDescription(req.mapEleInfo.description);

    cCuMapElementCreateReq.SetMapElement(mapElement);

    CCuMapElementCreateRsp cCuMapElementCreateRsp;
    CuCreateMapElement(cCuMapElementCreateReq, cCuMapElementCreateRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapElementCreateRsp.GetErrorCode();
    rsp.mapEleId = cCuMapElementCreateRsp.GetElementId();

    return SOAP_OK;
}

int cusdkService::ModifyMapElement(cusdk__ModifyMapElementReq req,cusdk__ModifyMapElementRsp &rsp)
{
    CCuMapElementModReq cCuMapElementModReq;
    cCuMapElementModReq.SetSession(req.session);

    MapElement mapElement;
    mapElement.SetId(req.mapElementInfo.id);
    mapElement.SetName(req.mapElementInfo.name);
    mapElement.SetLayerId(req.mapElementInfo.layerId);
	mapElement.SetStoreTrace(req.mapElementInfo.storeGPSTrace);
    //mapElement.SetGroupId(req.mapElementInfo.elementGroupId);

    //todo 接口不一致，要找周昌鸿协商
    switch(req.mapElementInfo.type)
    {
    case cusdk__MapEleCommon:
        {
            mapElement.SetType(MAP_ElEMENT_COMMAN);
        }
        break;
    case cusdk__MapEleMove:
        {
            mapElement.SetType(MAP_ELEMENT_LIVE);
        }
        break;
//     case cusdk__MapEleMainMap:
//         {
// 
//         }
//         break;
//     case cusdk__MapEleSubMap:
//         {
//             mapElement.SetType(MAP_ELEMENT_SUBMAP);
//         }
//         break;
    default:
        {

        }
        break;
    }

    mapElement.SetDeviceId(req.mapElementInfo.deviceId);
//    mapElement.SetDeviceDomainId(req.mapElementInfo.deviceDomainId);
    mapElement.SetDeviceChannel(req.mapElementInfo.deviceChannel);

    //todo 接口不一致，要找周昌鸿协商
    switch(req.mapElementInfo.channelType)
    {
    case cusdk__DevChnTypeVideoSource:
        {
            mapElement.SetChannelType(MAP_DEVCHANNEL_VIDEO);
        }
        break;
    case cusdk__DevChnTypeAlarmInput:
        {
            mapElement.SetChannelType(MAP_DEVCHANNEL_ALARMIN);
        }
        break;
    default:
        {

        }
        break;
    }

    if( cusdk__MapCoordEarth == req.mapElementInfo.coordType)
    {
        mapElement.SetEarthCoord(true);
    }
    else if( cusdk__MapCoordMar == req.mapElementInfo.coordType ) 
    {
        mapElement.SetEarthCoord(false);
    }

    MapPosition tMapPosition;
    //tMapPosition.SetEarthCoord(req.mapElementInfo.position.earthCoord);
    tMapPosition.SetLatitude(req.mapElementInfo.position.latitude);
    tMapPosition.SetLongitude(req.mapElementInfo.position.longitude);
    mapElement.SetPosition(tMapPosition);
    //ConverTempMapPositionCuToCui(req.mapElementInfo.positions, mapElement.GetPositions());

	MapPosition tMarsPosition;
	tMarsPosition.SetLatitude(req.mapElementInfo.marPosition.latitude);
	tMarsPosition.SetLongitude(req.mapElementInfo.marPosition.longitude);
	mapElement.SetMarsPosition(tMarsPosition);

    mapElement.SetPicUrl(req.mapElementInfo.picUrl);
    mapElement.SetDescription(req.mapElementInfo.description);
    mapElement.SetMapId(req.mapElementInfo.mapId);
    cCuMapElementModReq.SetMapElement(mapElement);

    CCuMapElementModRsp cCuMapElementModRsp;
    CuModifyMapElement(cCuMapElementModReq, cCuMapElementModRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapElementModRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::DelMapElement(cusdk__DelMapElementReq req,cusdk__DelMapElementRsp &rsp)
{
    CCuMapElementDelReq cCuMapElementDelReq;
    cCuMapElementDelReq.SetSession(req.session);
    cCuMapElementDelReq.SetElementId(req.mapElementId);

    CCuMapElementDelRsp cCuMapElementDelRsp;
    CuDelMapElement(cCuMapElementDelReq, cCuMapElementDelRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapElementDelRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::QueryMapElement(cusdk__QueryMapElementReq req,cusdk__QueryMapElementRsp &rsp)
{
    CCuMapElementQryReq cCuMapElementQryReq;
    cCuMapElementQryReq.SetSession(req.session);
    cCuMapElementQryReq.SetStartEntryIndex(req.beginIndex);
    cCuMapElementQryReq.SetExpectEntryNum(req.num);

    MapElementCondition codition;
    
    if(req.mapIdEnabled)
    {
        codition.SetMapId(req.mapId);
    }  

    if(req.elementIdEnabled)
    {
        codition.SetId(req.elementId);
    }

    if(req.elementTypeEnabled)
    {
        switch(req.elementType)
        {
        case cusdk__MapEleCommon:
            {
                codition.SetType(MAP_ElEMENT_COMMAN);
            }
            break;
        case cusdk__MapEleMove:
            {
                codition.SetType(MAP_ELEMENT_LIVE);
            }
            break;
//         case cusdk__MapEleSubMap:
//             {
//                 codition.SetType(MAP_ELEMENT_SUBMAP);
//             }
//             break;
//         case cusdk__MapEleMainMap:
        case cusdk__MapElementTypeInvalid:
        default:
            {

            }
            break;
        }
    }

    if(req.deviceChannelEnabled)
    {
        codition.SetDeviceId(req.deviceId);
        codition.SetDeviceChannel(req.deviceChannel);

        switch(req.channelType)
        {
        case cusdk__DevChnTypeVideoSource:
            {
                codition.SetChannelType(MAP_DEVCHANNEL_VIDEO);
            }
            break;
        case cusdk__DevChnTypeAlarmInput:
            {
                codition.SetChannelType(MAP_DEVCHANNEL_ALARMIN);
            }
            break;
        case cusdk__DeviceChannelTypeInvalid:
        default:
            {

            }
            break;
        }
    }

    cCuMapElementQryReq.SetCondition(codition);

    CCuMapElementQryRsp cCuMapElementQryRsp;
    CuQueryMapElement(cCuMapElementQryReq, cCuMapElementQryRsp);

    ConverTempMapElementCuiToCu(cCuMapElementQryRsp.GetMapElement(), rsp.mapElementList);
    rsp.session = req.session;
    rsp.errorCode = cCuMapElementQryRsp.GetErrorCode();
    rsp.totalNum = cCuMapElementQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::SearchMapElement(cusdk__SearchMapElementReq req, cusdk__SearchMapElementRsp &rsp)
{
	rsp.session = req.session;
	rsp.errorCode = ERR_CUI_INVALID_PARAM;

	if (req.mapId.empty()
		|| req.searchKeyEnabled == req.searchPosEnable // 暂时不允许区域内搜索关键字
		|| req.beginIndex < 0
		|| req.num < 0)
	{
		return SOAP_OK;
	}

	CCuMapElementSearchReq cCuMapElementSearchReq;
	cCuMapElementSearchReq.SetSession(req.session);
	cCuMapElementSearchReq.SetStartEntryIndex(req.beginIndex);
	cCuMapElementSearchReq.SetExpectEntryNum(req.num);
	cCuMapElementSearchReq.SetMapId(req.mapId);

	if (req.searchKeyEnabled)
	{
		if (req.searchKey.empty())
		{
			return SOAP_OK;
		}
		cCuMapElementSearchReq.SetSearchKeyEnabled(true);
		cCuMapElementSearchReq.SetSearchKey(req.searchKey);
	}
	else if (req.searchPosEnable)
	{
		cCuMapElementSearchReq.SetSearchPosEnable(true);
	
		MapPosition position;
		position.SetLatitude(req.upperLeft.latitude);
		position.SetLongitude(req.upperLeft.longitude);
		cCuMapElementSearchReq.SetUpperLeft(position);

		position.SetLatitude(req.bottomRight.latitude);
		position.SetLongitude(req.bottomRight.longitude);
		cCuMapElementSearchReq.SetBottomRight(position);
	}
	else
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	CCuMapElementSearchRsp cCuMapElementSearchRsp;

	CuSearchMapElement(cCuMapElementSearchReq, cCuMapElementSearchRsp);

	ConverTempMapElementCuiToCu(cCuMapElementSearchRsp.GetMapElement(), rsp.mapElementList);

	rsp.session = req.session;
	rsp.errorCode = cCuMapElementSearchRsp.GetErrorCode();
	rsp.totalNum = cCuMapElementSearchRsp.GetTotalEntryNum();

	return SOAP_OK;
}
int cusdkService::MapPositionMar2Earth(cusdk__MapPositionMar2EarthReq req,cusdk__MapPositionMar2EarthRsp &rsp)
{
    CCuPosCorrectMarsToEarthReq cCuPosCorrectMarsToEarthReq;
    cCuPosCorrectMarsToEarthReq.SetSession(req.session);

    cCuPosCorrectMarsToEarthReq.SetMars_lat(req.marPos.latitude);
    cCuPosCorrectMarsToEarthReq.SetMars_lng(req.marPos.longitude);

    CCuPosCorrectMarsToEarthRsp cCuPosCorrectMarsToEarthRsp;
    CuMapPositionMar2Earth(cCuPosCorrectMarsToEarthReq, cCuPosCorrectMarsToEarthRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuPosCorrectMarsToEarthRsp.GetErrorCode();
    rsp.earthPos.latitude = cCuPosCorrectMarsToEarthRsp.GetEarth_lat();
    rsp.earthPos.longitude = cCuPosCorrectMarsToEarthRsp.GetEarth_lng();

    return SOAP_OK;
}

int cusdkService::MapPositionEarth2Mar(cusdk__MapPositionEarth2MarReq req,cusdk__MapPositionEarth2MarRsp &rsp)
{
    CCuPosCorrectEarthToMarsReq cCuPosCorrectEarthToMarsReq;
    cCuPosCorrectEarthToMarsReq.SetSession(req.session);

    MapPosition earthPos;
    earthPos.SetLatitude(req.earthPos.latitude);
    earthPos.SetLongitude(req.earthPos.longitude);

    CCuPosCorrectEarthToMarsRsp cCuPosCorrectEarthToMarsRsp;
    CuMapPositionEarth2Mar(cCuPosCorrectEarthToMarsReq, cCuPosCorrectEarthToMarsRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuPosCorrectEarthToMarsRsp.GetErrorCode();
    rsp.marPos.latitude = cCuPosCorrectEarthToMarsRsp.GetMars_lat();
    rsp.marPos.longitude = cCuPosCorrectEarthToMarsRsp.GetMars_lng();

    return SOAP_OK;
}


int cusdkService::GetEMapDataVersion( cusdk__GetEMapDataVersionReq req, cusdk__GetEMapDataVersionRsp &rsp )
{
    CCuMpsDataVersionGetReq cReq;
    cReq.SetSession(req.session);
    CCuMpsDataVersionGetRsp cRsp;

    CuEMapDataVersion(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    rsp.dataVersion.elementVersion = cRsp.GetVersion().elementVersion;

    return SOAP_OK;
}

int cusdkService::GetAlarmLinkDataVersion( cusdk__GetAlarmLinkDataVersionReq req, cusdk__GetAlarmLinkDataVersionRsp &rsp )
{
    CCuAlarmLinkVersionGetReq cReq;
    cReq.SetSession(req.session);

    CCuAlarmLinkVersionGetRsp cRsp;
    CuGetAlarmLinkDataVersion(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    rsp.dataVersion = cRsp.GetVersion();

    return SOAP_OK;
}


int cusdkService::QueryAlarmLinkConfig(cusdk__QueryAlarmLinkConfigReq req,cusdk__QueryAlarmLinkConfigRsp &rsp)
{
    //如果不添加查询条件则为所有告警联动查询
    CCuAlarmLinkQueryReq cCuAlarmLinkQueryReq;
    cCuAlarmLinkQueryReq.SetSession(req.session);
    cCuAlarmLinkQueryReq.SetStartEntryIndex(req.beginIndex);
    cCuAlarmLinkQueryReq.SetExpectEntryNum(req.num);

    CCuAlarmLinkQueryRsp cCuAlarmLinkQueryRsp;
    CuQueryAlarmLinkConfig(cCuAlarmLinkQueryReq, cCuAlarmLinkQueryRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuAlarmLinkQueryRsp.GetErrorCode();
    rsp.totalNum = cCuAlarmLinkQueryRsp.GetTotalEntryNum();
    ConverTempAlarmLinkageCuiToCu(cCuAlarmLinkQueryRsp.GetAlarmLink(), rsp.alarmLinkConfigList);

    return SOAP_OK;
}

int cusdkService::GetAlarmLinkConfig(cusdk__GetAlarmLinkConfigReq req,cusdk__GetAlarmLinkConfigRsp &rsp)
{
    CCuAlarmLinkCfgGetReq cCuAlarmLinkCfgGetReq;
    cCuAlarmLinkCfgGetReq.SetSession(req.session);
    TAlarmLinkSrc alarmSrc;
    alarmSrc.SetDevUri(req.alarmSource.alarmDeviceId);
    alarmSrc.SetInputId(req.alarmSource.alarmInputChnId);

    //转换到 cmsType
    {
        CMS_ALARM_TYPE cmsType = "";//ALARM_TYPE_PIN
        const CU_ALARM_TYPE &cuType = req.alarmSource.alarmType;
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {
            cmsType = pcAlmUtils->ConvertFromCuToCms( cuType ); //cusdk__AlarmInput
            alarmSrc.SetAlarmType(cmsType);
        }
    }

    cCuAlarmLinkCfgGetReq.SetAlarmSrc(alarmSrc);

    CCuAlarmLinkCfgGetRsp cCuAlarmLinkCfgGetRsp;
    CuGetAlarmLinkConfig(cCuAlarmLinkCfgGetReq, cCuAlarmLinkCfgGetRsp);

//    rsp.alarmLinkCoonfig.alarmName = cCuAlarmLinkCfgGetRsp.GetCfg().GetAlarmAlias();
    rsp.alarmLinkCoonfig.alarmSource.alarmDeviceId = cCuAlarmLinkCfgGetRsp.GetCfg().GetAlarmLinkSrc().GetDevUri();
    rsp.alarmLinkCoonfig.alarmSource.alarmInputChnId = cCuAlarmLinkCfgGetRsp.GetCfg().GetAlarmLinkSrc().GetInputId();
    rsp.alarmLinkCoonfig.enabled = cCuAlarmLinkCfgGetRsp.GetCfg().GetIsUsed();

    //转换回 cuType
    {
        const CMS_ALARM_TYPE &cmsType = cCuAlarmLinkCfgGetRsp.GetCfg().GetAlarmLinkSrc().GetAlarmType();//ALARM_TYPE_PIN
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {   
            //cusdk__AlarmInput
            rsp.alarmLinkCoonfig.alarmSource.alarmType = cusdk__EAlarmType( pcAlmUtils->ConvertFromCmsToCu( cmsType ) ); //cusdk__AlarmInput
        }
    }
    ConverTempCronTimeCuiToCu(cCuAlarmLinkCfgGetRsp.GetCfg().GetDefenceSched(), rsp.alarmLinkCoonfig.enabledCronTimeList);
    ConverTempAlarmLinkDstCuiToCu(cCuAlarmLinkCfgGetRsp.GetCfg().GetAlarmLinkDstList(), rsp.alarmLinkCoonfig.alarmLinkActionList);
    rsp.session = req.session;
    rsp.errorCode = cCuAlarmLinkCfgGetRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetAlarmLinkConfig(cusdk__SetAlarmLinkConfigReq req,cusdk__SetAlarmLinkConfigRsp &rsp)
{
    CCuAlarmLinkCfgSetReq cCuAlarmLinkCfgSetReq;
    cCuAlarmLinkCfgSetReq.SetSession(req.session);

    TAlarmLinkage cfg;
//    cfg.SetAlarmAlias(req.alarmLinkCoonfig.alarmName);
    cfg.SetIsUsed(req.alarmLinkCoonfig.enabled);

    TAlarmLinkSrc alarmSrc;
    alarmSrc.SetDevUri(req.alarmLinkCoonfig.alarmSource.alarmDeviceId);
    alarmSrc.SetInputId(req.alarmLinkCoonfig.alarmSource.alarmInputChnId);

    CMS_ALARM_TYPE cmsType = "";//ALARM_TYPE_PIN
    const CU_ALARM_TYPE &cuType = req.alarmLinkCoonfig.alarmSource.alarmType;
    const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
    if( pcAlmUtils != NULL )
    {
        cmsType = pcAlmUtils->ConvertFromCuToCms( cuType ); //cusdk__AlarmInput
        alarmSrc.SetAlarmType( cmsType );
    }


    cfg.SetAlarmLinkSrc(alarmSrc);

    ConverTempCronTimeCuToCui(req.alarmLinkCoonfig.enabledCronTimeList, cfg.GetDefenceSched());
    ConverTempAlarmLinkCuToCui(req.alarmLinkCoonfig.alarmLinkActionList, cfg.GetAlarmLinkDstList());
    cCuAlarmLinkCfgSetReq.SetCfg(cfg);

    CCuAlarmLinkCfgSetRsp cCuAlarmLinkCfgSetRsp;
    CuSetAlarmLinkConfig(cCuAlarmLinkCfgSetReq, cCuAlarmLinkCfgSetRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuAlarmLinkCfgSetRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::QueryUserLog(cusdk__QueryUserLogReq req,cusdk__QueryUserLogRsp &rsp)
{
    CCuOperateLogQryReq cCuOperateLogQryReq;
    cCuOperateLogQryReq.SetSession(req.session);
    cCuOperateLogQryReq.SetStartEntryIndex(req.beginIndex);
    cCuOperateLogQryReq.SetExpectEntryNum(req.num);

    COperateLogInfoQueryCondition condition;
    if(req.condition.userLoginSessionEnabled)
    {
        condition.SetLogTag(req.condition.userLoginSession);
    }
    
    if(req.condition.userIdEnabled)
    {
        condition.SetUserUri(req.condition.userId);
    }

    if(req.condition.domainNameEnabled)
    {
        condition.SetDomainName(req.condition.domainName);
    }

    if(req.condition.timeEnabled)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());
        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }

    if(req.condition.operateTypeEnabled)
    {
        condition.SetOperateType(req.condition.operateType);
    }

    cCuOperateLogQryReq.SetCondition(condition);

    CCuOperateLogQryRsp cCuOperateLogQryRsp;
    CuQueryUserLog(cCuOperateLogQryReq, cCuOperateLogQryRsp);

    ConverTempOperateLogCuiToCu(cCuOperateLogQryRsp.GetInfo(), rsp.userLogList);

    rsp.session = req.session;
    rsp.errorCode = cCuOperateLogQryRsp.GetErrorCode();
    rsp.totalNum = cCuOperateLogQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::QueryDeviceLog(cusdk__QueryDeviceLogReq req,cusdk__QueryDeviceLogRsp &rsp)
{
    CCuDeviceLogQryReq cCuDeviceLogQryReq;
    cCuDeviceLogQryReq.SetSession(req.session);

    cCuDeviceLogQryReq.SetExpectEntryNum(req.num);
    cCuDeviceLogQryReq.SetStartEntryIndex(req.beginIndex);

    CDeviceLogInfoQueryCondition condition;
    if(req.condition.deviceIdEnabled == true)
    {
        condition.SetDevUri(req.condition.deviceId);
    }

    if(req.condition.deviceNameEnabled == true)
    {
        condition.SetDevName(req.condition.deviceName);
    }

    if(req.condition.timeEnabled == true)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());

        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }

    if(req.condition.logTypeEnabled == true)
    {
        condition.SetLogType(req.condition.logType);
    }

    cCuDeviceLogQryReq.SetCondition(condition);

    CCuDeviceLogQryRsp cCuDeviceLogQryRsp;
    CuQueryDeviceLog(cCuDeviceLogQryReq, cCuDeviceLogQryRsp);

    ConverTempDeviceLogCuiToCu(cCuDeviceLogQryRsp.GetInfo(), rsp.deviceLogList);

    rsp.session = req.session;
    rsp.errorCode = cCuDeviceLogQryRsp.GetErrorCode();
    rsp.totalNum = cCuDeviceLogQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::QueryAlarmLog(cusdk__QueryAlarmLogReq req,cusdk__QueryAlarmLogRsp &rsp)
{
    CCuAlarmLogQryReq cCuAlarmLogQryReq;
    cCuAlarmLogQryReq.SetSession(req.session);
    cCuAlarmLogQryReq.SetStartEntryIndex(req.beginIndex);
    cCuAlarmLogQryReq.SetExpectEntryNum(req.num);

    CAlarmLogInfoQueryCondition condition;

    if(req.condition.deviceIdEnabled == true)
    {
        condition.SetDevUri(req.condition.deviceId);
    }

    if(req.condition.alarmNameEnabled == true)
    {
        condition.SetAlias(req.condition.alarmName);
    }

    if(req.condition.deviceDomainNameEnabled == true)
    {
        condition.SetDevDomainName(req.condition.deviceDomainName);
    }

    if(req.condition.alarmChannelIdEnabled == true)
    {
        condition.SetChannelId(req.condition.alarmChannelId);
    }

    if(req.condition.timeEnabled == true)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());

        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }

    if(req.condition.alarmTypeEnabled == true)
    {
        CMS_ALARM_TYPE cmsType = "";//ALARM_TYPE_PIN
        const CU_ALARM_TYPE &cuType = req.condition.alarmType;
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {
            cmsType = pcAlmUtils->ConvertFromCuToCms( cuType ); //cusdk__AlarmInput
            condition.SetType( cmsType );
        }
    }

    //todo flag标识cu这边不需要识别
    cCuAlarmLogQryReq.SetCondition(condition);

    CCuAlarmLogQryRsp cCuAlarmLogQryRsp;
    CuQueryAlarmLog(cCuAlarmLogQryReq, cCuAlarmLogQryRsp);

    ConverTempAlarmLogCuiToCu(cCuAlarmLogQryRsp.GetInfo(), rsp.alarmLogList);

    rsp.session = req.session;
    rsp.errorCode = cCuAlarmLogQryRsp.GetErrorCode();
    rsp.totalNum = cCuAlarmLogQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}

int cusdkService::GetLogConfig(cusdk__GetLogConfigReq req,cusdk__GetLogConfigRsp &rsp)
{
    CCuConfigUasGetReq cCuConfigUasGetReq;
    cCuConfigUasGetReq.SetSession(req.session);

    CCuConfigUasGetRsp cCuConfigUasGetRsp;
    CuGetLogConfig(cCuConfigUasGetReq, cCuConfigUasGetRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuConfigUasGetRsp.GetErrorCode();

    rsp.maxUserLogSavedDays = cCuConfigUasGetRsp.GetUserLogSavedMaxDays();
    rsp.maxUserLogSavedNumber = cCuConfigUasGetRsp.GetUserLogSavedMaxNumber();
    rsp.enableUserLog = cCuConfigUasGetRsp.GetUserLogEnabled();
    rsp.maxDeviceLogSavedNumber = cCuConfigUasGetRsp.GetDeviceLogSavedMaxNumber();
    rsp.maxDeviceLogSavedDays = cCuConfigUasGetRsp.GetDeviceLogSavedMaxDays();
    rsp.enableDeviceLog = cCuConfigUasGetRsp.GetDeviceLogEnabled();
    rsp.maxAlarmLogSavedNumber = cCuConfigUasGetRsp.GetAlarmLogSavedMaxNumber();
    rsp.maxAlarmLogSavedDays = cCuConfigUasGetRsp.GetAlarmLogSavedMaxDays();

    return SOAP_OK;
}

//获取国标id，用于移动客户端
int cusdkService::GetDeviceGBID(cusdk__GetDeviceGBIDReq req, cusdk__GetDeviceGBIDRsp &rsp)
{
	CCuGbsDevGbidGetReq cCuGbsDevGbidGetReq;
	cCuGbsDevGbidGetReq.SetSession(req.session);
	cCuGbsDevGbidGetReq.SetKdDevUri(req.deviceChn.deviceId);
	cCuGbsDevGbidGetReq.SetEncodeChnIndex(req.deviceChn.channelId);

	CCuGbsDevGbidGetRsp cCuGbsDevGbidGetRsp;
	CuGetDeviceGBID(cCuGbsDevGbidGetReq,cCuGbsDevGbidGetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuGbsDevGbidGetRsp.GetErrorCode();
	rsp.deviceGBID = cCuGbsDevGbidGetRsp.GetGbId();
	rsp.gbsID = cCuGbsDevGbidGetRsp.GetGbsId();

	return SOAP_OK;
}


int cusdkService::GetResInfo(cusdk__ResInfoReq cResInfoReq, cusdk__ResInfoRsp &cResInfoRsp)
{
	// mss接口
	return SOAP_OK;
}

int cusdkService::GetCAPSConfig(cusdk__GetCAPSConfigReq req, cusdk__GetCAPSConfigRsp &rsp)
{
	CCuGetCAPSConfigReq cCuGetCAPSConfigReq;
	cCuGetCAPSConfigReq.SetSession(req.session);
	
	CCuGetCAPSConfigRsp cCuGetCAPSConfigRsp;
	CuGetCAPSConfig(cCuGetCAPSConfigReq,cCuGetCAPSConfigRsp);

	rsp.session=req.session;
	rsp.errorCode=cCuGetCAPSConfigRsp.GetErrorCode();
	rsp.captureIntervalSecs=cCuGetCAPSConfigRsp.GetCaptureIntercal();
	rsp.wechatUrl=cCuGetCAPSConfigRsp.GetWechatUrl();   
	rsp.wechatTitle=cCuGetCAPSConfigRsp.GetWechatTitle();

	return SOAP_OK;
}

int cusdkService::SetCAPSConfig(cusdk__SetCAPSConfigReq req, cusdk__SetCAPSConfigRsp &rsp)
{
	CCuSetCAPSConfigReq cCuSetCAPSConfigReq;
	cCuSetCAPSConfigReq.SetSession(req.session);
	cCuSetCAPSConfigReq.SetCaptureInterval(req.captureIntervalSecs);
	cCuSetCAPSConfigReq.SetWechatTitle(req.wechatTitle);

	CCuSetCAPSConfigRsp cCuSetCAPSConfigRsp;
	CuSetCAPSConfig(cCuSetCAPSConfigReq,cCuSetCAPSConfigRsp);
	rsp.session=req.session;
	rsp.errorCode=cCuSetCAPSConfigRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::QueryCAPSDeviceList(cusdk__QueryCAPSDeviceListReq req, cusdk__QueryCAPSDeviceListRsp &rsp)
{
	CCuQueryCAPSDeviceListReq cCuQueryCAPSDeviceListReq;
	cCuQueryCAPSDeviceListReq.SetSession(req.session);
	cCuQueryCAPSDeviceListReq.SetStartEntryIndex(req.beginIndex);
	cCuQueryCAPSDeviceListReq.SetExpectEntryNum(req.num);

	CCuQueryCAPSDeviceListRsp cCuQueryCAPSDeviceListRsp;
	CuQueryCAPSDeviceList(cCuQueryCAPSDeviceListReq,cCuQueryCAPSDeviceListRsp);

	ConverCapsDevListCuiToCu(cCuQueryCAPSDeviceListRsp.GetDevList(),rsp.devList);
	rsp.session=req.session;
	rsp.errorCode=cCuQueryCAPSDeviceListRsp.GetErrorCode();
	rsp.totalNum=cCuQueryCAPSDeviceListRsp.GetTotalEntryNum();
	return SOAP_OK;
}

int cusdkService::CAPSAddDevice(cusdk__CAPSAddDeviceReq req, cusdk__CAPSAddDeviceRsp &rsp)
{
	CCuCAPSAddDeviceReq cCuCAPSAddDeviceReq;
	cCuCAPSAddDeviceReq.SetSession(req.session);

	CWechatDeviceList devList;
	ConverCapsDevListCuToCui(req.devList,devList);
	cCuCAPSAddDeviceReq.SetDevList(devList);

	CCuCAPSAddDeviceRsp cCuCAPSAddDeviceRsp;
	CuCAPSAddDevice(cCuCAPSAddDeviceReq,cCuCAPSAddDeviceRsp);

	rsp.session=req.session;
	rsp.errorCode=cCuCAPSAddDeviceRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::CAPSModifyDevice(cusdk__CAPSModifyDeviceReq req, cusdk__CAPSModifyDeviceRsp &rsp)
{
	CCuCAPSModifyDeviceReq cCuCAPSModifyDeviceReq;
	cCuCAPSModifyDeviceReq.SetSession(req.session);
	CWechatDevice cDevice;
	cDevice.SetDevUri(req.device.devId);
	cDevice.SetChnNO(req.device.chnId);
	cDevice.SetAlias(req.device.name);
	cDevice.SetGBId(req.device.devGBId);
	cDevice.SetGBDomainId(req.device.domainGBId);
	cCuCAPSModifyDeviceReq.SetDevice(cDevice);

	CCuCAPSModifyDeviceRsp cCuCAPSModifyDeviceRsp;
	CuCAPSModifyDevice(cCuCAPSModifyDeviceReq,cCuCAPSModifyDeviceRsp);

	rsp.session=req.session;
	rsp.errorCode=cCuCAPSModifyDeviceRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::CAPSDelDevice(cusdk__CAPSDelDeviceReq req, cusdk__CAPSDelDeviceRsp &rsp)
{
	CCuCAPSDelDeviceReq cCuCAPSDelDeviceReq;
	cCuCAPSDelDeviceReq.SetSession(req.session);
	cCuCAPSDelDeviceReq.SetDevId(req.device.deviceId);
	cCuCAPSDelDeviceReq.SetChanId(req.device.channelId);

	CCuCAPSDelDeviceRsp cCuCAPSDelDeviceRsp;
	CuCAPSDelDevice(cCuCAPSDelDeviceReq, cCuCAPSDelDeviceRsp);
	rsp.session=req.session;
	rsp.errorCode=cCuCAPSDelDeviceRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetOfflineDeviceStat(cusdk__GetOfflineDeviceStatReq req, cusdk__GetOfflineDeviceStatRsp &rsp)
{
	CCuOffLineDevQryReq cCuOffLineDevQryReq;
	cCuOffLineDevQryReq.SetSession(req.session);
	cCuOffLineDevQryReq.SetStartEntryIndex(req.beginIndex);
	cCuOffLineDevQryReq.SetExpectEntryNum(req.num);
	cCuOffLineDevQryReq.SetDomanName(req.domanName);
	cCuOffLineDevQryReq.SetCuQueryTag(req.queryTag);

	CCuOffLineDevQryRsp cCuOffLineDevQryRsp;

	if ( req.queryTag.empty() || // queryTag不允许为空
		req.domanName.empty() // domainName不允许为空
		)
	{
		rsp.session=req.session;
		rsp.errorCode = ERR_CUI_PARAMS_INVALID;
		rsp.onlineDeviceNum = 0;
		rsp.totalOfflineDeviceVideosrcNum = 0;
		rsp.idleDeviceNum = 0;
		rsp.queryTag = req.queryTag;
		rsp.domanName = req.domanName;
		return SOAP_OK;
	}

	CuGetOfflineDeviceStat(cCuOffLineDevQryReq,cCuOffLineDevQryRsp);

	ConvertDevListCuiToCu(cCuOffLineDevQryRsp.GetDevVideoSrcList(),rsp);

	rsp.session=req.session;
	rsp.errorCode=cCuOffLineDevQryRsp.GetErrorCode();
	rsp.onlineDeviceNum=cCuOffLineDevQryRsp.GetOnlineVideoSrcNum();
	rsp.totalOfflineDeviceVideosrcNum = cCuOffLineDevQryRsp.GetOfflineDevNum();
	rsp.idleDeviceNum = cCuOffLineDevQryRsp.GetIdleDevNum();
	rsp.queryTag = req.queryTag;
	rsp.domanName = req.domanName;
	return SOAP_OK;
}

int cusdkService::UpdateCUProperty( cusdk__UpdateCUPropertyReq req, cusdk__UpdateCUPropertyRsp &rsp )
{
	CCuUpdateCUPropertyReq cCuUpdateCUPropertyReq;
	cCuUpdateCUPropertyReq.SetSession(req.session);
	cCuUpdateCUPropertyReq.SetClientLocalIp(req.clientLocalIP);
	cCuUpdateCUPropertyReq.SetPlatNatMode(req.platNatMode);
	cCuUpdateCUPropertyReq.SetClientSupCuiTransPlatRec(req.clientSupportNatProbe);

	CCuUpdateCUPropertyRsp cCuUpdateCUPropertyRsp;
	CuUpdateCUProperty(cCuUpdateCUPropertyReq,cCuUpdateCUPropertyRsp);

	rsp.cuNatMode=(cusdk__ENATMode)cCuUpdateCUPropertyRsp.GetClientNat();
	rsp.platNatMode=(cusdk__ENATMode)cCuUpdateCUPropertyRsp.GetPlatNat();
	rsp.platNATIP=cCuUpdateCUPropertyRsp.GetCuPunchAddr().GetNetIp();
	rsp.platNATPort=cCuUpdateCUPropertyRsp.GetCuPunchAddr().GetNetPort();
	rsp.platSupportForward = cCuUpdateCUPropertyRsp.GetCuiSupTransPlatRec();

	rsp.session=req.session;
	rsp.errorCode=cCuUpdateCUPropertyRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::SetLogConfig(cusdk__SetLogConfigReq req,cusdk__SetLogConfigRsp &rsp)
{
    CCuConfigUasSetReq cCuConfigUasSetReq;
    cCuConfigUasSetReq.SetSession(req.session);

    cCuConfigUasSetReq.SetUserLogSavedMaxDays(req.maxUserLogSavedDays);
    cCuConfigUasSetReq.SetUserLogSavedMaxNumber(req.maxUserLogSavedNumber);
    cCuConfigUasSetReq.SetDeviceLogSavedMaxDays(req.maxDeviceLogSavedDays);
    cCuConfigUasSetReq.SetDeviceLogSavedMaxNumber(req.maxDeviceLogSavedNumber);
    cCuConfigUasSetReq.SetAlarmLogSavedMaxDays(req.maxAlarmLogSavedDays);
    cCuConfigUasSetReq.SetAlarmLogSavedMaxNumber(req.maxAlarmLogSavedNumber);
    cCuConfigUasSetReq.SetUserLogEnabled(req.enableUserLog);
    cCuConfigUasSetReq.SetDeviceLogEnabled(req.enableDeviceLog);

    CCuConfigUasSetRsp cCuConfigUasSetRsp;
    CuSetLogConfig(cCuConfigUasSetReq, cCuConfigUasSetRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuConfigUasSetRsp.GetErrorCode();

    return SOAP_OK;
}

//todo 黄志春没有给出修改后的接口
int cusdkService::CallPu(cusdk__CallPuReq req,cusdk__CallPuRsp &rsp)
{
    CCuStartThirdPartyCallReq cCuStartThirdPartyCallReq;
    cCuStartThirdPartyCallReq.SetSession(req.session);

    //源是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(req.session);
    tCuChn.SetChnNO(req.encodeChnId);
    cCuStartThirdPartyCallReq.SetSrcChn(tCuChn);

    //目的是PU
    TChannel tDstChn;
    tDstChn.SetDevUri(req.devId);
    tDstChn.SetChnNO(req.devDecChnId);
    cCuStartThirdPartyCallReq.SetDstChn(tDstChn);

    TSdp tSdp;
    tSdp.SetSessionName(SDP_SESSION_AUDIOCALL);

	//设置MRTC使能
	if (cusdk__TransModeMRTC == req.transMode)
	{
		tSdp.SetMrtcEnable(true);
	}

    //遍历每个CU的IP
    ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

//     vector<cusdk__MediaDescription >::const_iterator pCuMediaDescItem = req.mediaDescriptionList.begin();
//     while(pCuMediaDescItem != req.mediaDescriptionList.end())
//     {   
//         //todo 黄志春 格式参数如何填写？
//         TMediaDesc tMediaDesc;
//         if(cusdk__MediaTypeVideo == pCuMediaDescItem->mediaType)
//         {
//             tMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
//         }
//         else if(cusdk__MediaTypeAudio == pCuMediaDescItem->mediaType)
//         {
//             tMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
//         }
//         else if(cusdk__MediaTypeData == pCuMediaDescItem->mediaType)
//         {
//             tMediaDesc.SetMediaType(MEDIA_TYPE_DATA);
//         }
// 
//         std::vector<cusdk__MediaFormat >::const_iterator pCuMediaFormatItem = pCuMediaDescItem->supportedFormats.begin();
//         while(pCuMediaFormatItem != pCuMediaDescItem->supportedFormats.end())
//         {
//             TMediaFormat mediaFormat;
//             mediaFormat.SetEncodingName(pCuMediaFormatItem->encodingName);
//             mediaFormat.SetManufacture(pCuMediaFormatItem->manufacture);
//             mediaFormat.SetPayloadType(pCuMediaFormatItem->payloadType);
//             //todo 缺少详细的格式参数。根据实际需要设置，比如分辨率、关键帧间隔等，找李之兴协商
//             //mediaFormat.SetFormatParam();
//             tMediaDesc.SetMediaFormat(mediaFormat);
// 
//             pCuMediaFormatItem++;
//         }       
// 
//         TMediaTransProto mediaTransProto; 
//         if( cusdk__TransModeUDP == pCuMediaDescItem->transMode)
//         {
//             mediaTransProto.SetLowerTransport(LOWER_TRANS_UDP);
//         }
//         else if(cusdk__TransModeTCP == pCuMediaDescItem->transMode)
//         {
//             mediaTransProto.SetLowerTransport(LOWER_TRANS_TCP);
//         }
//         tMediaDesc.SetMediaTransProto(mediaTransProto);
// 
//         //tMediaDesc.SetMediaType(pCuMediaDescItem->supportedResolution.front());
// 
//         ConvertMediaTransChannelFromCuToCui( pCuMediaDescItem->transChannelList, tMediaDesc.GetMediaTransChnList());
// 
//         tSdp.GetMediaDescList().push_back(tMediaDesc);
// 
//         pCuMediaDescItem++;
//     }  
    cCuStartThirdPartyCallReq.SetSdp(tSdp);

    CCuStartThirdPartyCallRsp cCuStartThirdPartyCallRsp; 
    CuCallPu(cCuStartThirdPartyCallReq, cCuStartThirdPartyCallRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuStartThirdPartyCallRsp.GetErrorCode();

    if(CMS_SUCCESS == rsp.errorCode)
    {
        ConvertTempMediaDescAndExtInfoCuiToCu(cCuStartThirdPartyCallRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
            cCuStartThirdPartyCallRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
    }
//     TMediaDescList::const_iterator pCuiMediaDescItem = cCuStartThirdPartyCallRsp.GetSdp().GetMediaDescList().begin();
//     while(pCuiMediaDescItem != cCuStartThirdPartyCallRsp.GetSdp().GetMediaDescList().end())
//     {
//         cusdk__MediaDescription cusdkMediaDescription;
//         if( MEDIA_TYPE_VIDEO == pCuiMediaDescItem->GetMediaType())
//         {
//             cusdkMediaDescription.mediaType = cusdk__MediaTypeVideo;
//         }
//         else if( MEDIA_TYPE_AUDIO == pCuiMediaDescItem->GetMediaType())
//         {
//             cusdkMediaDescription.mediaType = cusdk__MediaTypeAudio;
//         }
//         else if( MEDIA_TYPE_DATA == pCuiMediaDescItem->GetMediaType())
//         {
//             cusdkMediaDescription.mediaType = cusdk__MediaTypeData;
//         }
//         else if(MEDIA_TYPE_UNKNOWN == pCuiMediaDescItem->GetMediaType())
//         {
//             cusdkMediaDescription.mediaType = cusdk__MediaTypeInvalid;
//         }
// 
//         cusdk__MediaFormat cuMediaFormat;
//         cuMediaFormat.encodingName = pCuiMediaDescItem->GetMediaFormat().GetEncodingName();
//         cuMediaFormat.manufacture = pCuiMediaDescItem->GetMediaFormat().GetManufacture();
//         cuMediaFormat.payloadType = pCuiMediaDescItem->GetMediaFormat().GetPayloadType();
//         cusdkMediaDescription.supportedFormats.push_back(cuMediaFormat);
// 
//         if( LOWER_TRANS_UDP == pCuiMediaDescItem->GetMediaTransProto().GetLowerTransport())
//         {
//             cusdkMediaDescription.transMode = cusdk__TransModeUDP;
//         }
//         else if(LOWER_TRANS_TCP == pCuiMediaDescItem->GetMediaTransProto().GetLowerTransport() )
//         {
//             cusdkMediaDescription.transMode = cusdk__TransModeTCP;
//         }
// 
//         ConvertMediaTransChannelFromCuiToCu(pCuiMediaDescItem->GetMediaTransChnList(), cusdkMediaDescription.transChannelList);
// 
//         rsp.mediaDescriptionList.push_back(cusdkMediaDescription);
// 
//         pCuiMediaDescItem++;
//     }

    return SOAP_OK;
}

//todo 黄志春没有给出确切的接口
int cusdkService::HangUpPu(cusdk__HangUpPuReq req,cusdk__HangUpPuRsp &rsp)
{
    CCuStopThirdPartyCallReq cCuStopThirdPartyCallReq;
    cCuStopThirdPartyCallReq.SetSession(req.session);

    //源是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(req.session);
    tCuChn.SetChnNO(req.encodeChnId);
    cCuStopThirdPartyCallReq.SetSrcChn(tCuChn);

    //目的是pu
    TChannel tChn;
    tChn.SetDevUri(req.devId);
    tChn.SetChnNO(req.devDecChnId);
    cCuStopThirdPartyCallReq.SetDstChn(tChn);

    CCuStopThirdPartyCallRsp cCuStopThirdPartyCallRsp;
    CuHangUpPu(cCuStopThirdPartyCallReq, cCuStopThirdPartyCallRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuStopThirdPartyCallRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetDeviceEncoderParam(cusdk__GetDeviceEncoderParamReq req,cusdk__GetDeviceEncoderParamRsp &rsp)
{
    CCuPuVidEncParamGetReq cCuPuVidEncParamGetReq;
    cCuPuVidEncParamGetReq.SetSession(req.session);
    TChannel tchn;
    tchn.SetChnNO(req.encodeChnId);
    tchn.SetDevUri(req.deviceId);
	cCuPuVidEncParamGetReq.SetDevChn(tchn);

    CCuPuVidEncParamGetRsp cCuPuVidEncParamGetRsp;
    CuGetDeviceVidEncoderParam(cCuPuVidEncParamGetReq, cCuPuVidEncParamGetRsp);

	ConvertVideoEncParamCuiToCu(cCuPuVidEncParamGetRsp.GetVidEncParam(), rsp.videoEncParam);
    ConverTempVidTypeCuiToCu(cCuPuVidEncParamGetRsp.GetSupportedMediaType().GetVidTypes(), rsp.suportedVideoModeList);

    rsp.session = req.session;
    rsp.errorCode = cCuPuVidEncParamGetRsp.GetErrorCode();

    return SOAP_OK;
}
int cusdkService::GetDeviceAudioEncoderParam(cusdk__GetDeviceAudioEncoderParamReq req, cusdk__GetDeviceAudioEncoderParamRsp &rsp)
{
	CCuPuAudEncParamGetReq cCuPuAudEncParamGetReq;
	cCuPuAudEncParamGetReq.SetSession(req.session);
	TChannel tchn;
	tchn.SetChnNO(req.encodeChnId);
	tchn.SetDevUri(req.deviceId);
	cCuPuAudEncParamGetReq.SetDevChn(tchn);
	
	CCuPuAudEncParamGetRsp cCuPuAudEncParamGetRsp;
	CuGetDeviceAudEncoderParam(cCuPuAudEncParamGetReq, cCuPuAudEncParamGetRsp);

	ConvertAudioEncParamCuiToCu(cCuPuAudEncParamGetRsp.GetParam(), rsp.audioEncParam);
	ConverTempAudTypeCuiToCu(cCuPuAudEncParamGetRsp.GetSurportedMediaType().GetAudTypes(), rsp.supportedAudioModeList);

	rsp.session = req.session;
	rsp.errorCode = cCuPuAudEncParamGetRsp.GetErrorCode();

	return SOAP_OK;
}
int cusdkService::SetPuVidSrcOSD(cusdk__SetPuVidSrcOSDReq req, cusdk__SetPuVidSrcOSDRsp &rsp)
{
	CCuPuVidSrcOsdSetReq cCuPuVidSrcOsdSetReq;
	cCuPuVidSrcOsdSetReq.SetSession(req.session);
	TChannel tChannl(req.deviceId, req.videoSrcId);

	cCuPuVidSrcOsdSetReq.SetPuChn(tChannl);
	ConvertTPuVidSrcOsdCfgFromCuToCui(req.vidSrcOSDList, cCuPuVidSrcOsdSetReq.GetPuVidSrcOsdCfg());

	CCuPuVidSrcOsdSetRsp cCuPuVidSrcOsdSetRsp;

	CCuSetPuVidSrcOSD(cCuPuVidSrcOsdSetReq, cCuPuVidSrcOsdSetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuPuVidSrcOsdSetRsp.GetErrorCode();
	rsp.OSDItemErrors = cCuPuVidSrcOsdSetRsp.GetPuVidSrcOsdRet();
	return SOAP_OK;
}

int cusdkService::GetPuVidSrcOSD(cusdk__GetPuVidSrcOSDReq req, cusdk__GetPuVidSrcOSDRsp &rsp)
{
	CCuPuVidSrcOsdGetReq cCuPuVidSrcOsdGetReq;
	cCuPuVidSrcOsdGetReq.SetSession(req.session);
	TChannel tChannl(req.deviceId, req.videoSrcId);

	cCuPuVidSrcOsdGetReq.SetPuChn(tChannl);

	CCuPuVidSrcOsdGetRsp cCuPuVidSrcOsdGetRsp;

	CCuGetPuVidSrcOSD(cCuPuVidSrcOsdGetReq, cCuPuVidSrcOsdGetRsp);

	ConvertTPuVidSrcOsdCfgFromCuiToCu(cCuPuVidSrcOsdGetRsp.GetPuVidSrcOsdCfg(), rsp.vidSrcOSDList);
	rsp.session = req.session;
	rsp.errorCode = cCuPuVidSrcOsdGetRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::SetDeviceEncoderParam(cusdk__SetDeviceEncoderParamReq req,cusdk__SetDeviceEncoderParamRsp &rsp)
{
    CCuPuVidEncParamSetReq cCuPuVidEncParamSetReq;
    cCuPuVidEncParamSetReq.SetSession(req.session);
    //由cuiserver填写
    //cCuPuVidEncParamSetReq.SetUserUri();
    TChannel tChn;        //设备通道
    tChn.SetChnNO(req.encodeChnId);
    tChn.SetDevUri(req.deviceId);
    cCuPuVidEncParamSetReq.SetDevChn(tChn);
    TVideoEncodeParam  param;         //视频编码参数   
    if(req.videoParam.videoMode.videoFormat == cusdk__VF_SN4)
    {
        param.SetVideoType(VID_FMT_SN4);
    }
    else if(req.videoParam.videoMode.videoFormat == cusdk__VF_MPEG4)
    {
        param.SetVideoType(VID_FMT_MPEG4);
    }
    else if(req.videoParam.videoMode.videoFormat == cusdk__VF_H261)
    {
        param.SetVideoType(VID_FMT_H261);
    }
    else if(req.videoParam.videoMode.videoFormat == cusdk__VF_H263)
    {
        param.SetVideoType(VID_FMT_H263);
    }
    else if(req.videoParam.videoMode.videoFormat == cusdk__VF_H264)
    {
        param.SetVideoType(VID_FMT_H264);
	}
	else if(req.videoParam.videoMode.videoFormat == cusdk__VF_H265)
	{
		param.SetVideoType(VID_FMT_H265);
	}
	else if(req.videoParam.videoMode.videoFormat == cusdk__VF_SVAC)
	{
		param.SetVideoType(VID_FMT_SVAC);
	}
	else if(req.videoParam.videoMode.videoFormat == cusdk__VF_MJPEG)
	{
		param.SetVideoType(VID_FMT_MJPEG);
	}


	if (req.videoParam.videoMode.videoResStr.empty())
	{
		if(req.videoParam.videoMode.videoRes == cusdk__VS_AUTO)
		{
			param.SetVideoRes(VID_RES_AUTO);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_QCIF)
		{
			param.SetVideoRes(VID_RES_QCIF);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_CIF)
		{
			param.SetVideoRes(VID_RES_CIF);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_2CIF)
		{
			param.SetVideoRes(VID_RES_2CIF);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_4CIF)
		{
			param.SetVideoRes(VID_RES_4CIF);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_QQCIF)
		{
			param.SetVideoRes(VID_RES_QQCIF);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_QVGA)
		{
			param.SetVideoRes(VID_RES_QVGA);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_VGA)
		{
			param.SetVideoRes(VID_RES_VGA);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_720P)
		{
			param.SetVideoRes(VID_RES_720P);
		}
		else if (req.videoParam.videoMode.videoRes == cusdk__VS_960P)
		{
			param.SetVideoRes(VID_RES_960P);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_1080A)
		{
			//当前平台不支持该类型
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_1080P)
		{
			param.SetVideoRes(VID_RES_1080P);
		}
		else if(req.videoParam.videoMode.videoRes == cusdk__VS_QXGA)
		{
			param.SetVideoRes(VID_RES_QXGA);
		}
	}
	else
	{
		param.SetVideoRes(req.videoParam.videoMode.videoResStr);
	}

    param.SetFrameRate(req.videoParam.frameRate);
    if(req.videoParam.quality == cusdk__VIDEO_QUALITY_PRIOR)
    {
        param.SetQuality(VID_QUALITY_PRIOR);
    }
    else if(req.videoParam.quality == cusdk__VIDEO_SPEED_PRIOR)
    {
        param.SetQuality(VID_SPEED_PRIOR);
    }

    param.SetBitRate(req.videoParam.bitRate);
    param.SetInterval(req.videoParam.interval);
    param.SetBright(req.videoParam.brightness);
    param.SetContrast(req.videoParam.contrast);
    param.SetSaturation(req.videoParam.saturation);
    param.SetSharpness(req.videoParam.sharpness);
    cCuPuVidEncParamSetReq.SetVidEncParam(param);

    CCuPuVidEncParamSetRsp cCuPuVidEncParamSetRsp;
    CuSetDeviceEncoderParam(cCuPuVidEncParamSetReq, cCuPuVidEncParamSetRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuPuVidEncParamSetRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetDiskList(cusdk__GetDiskListReq req, cusdk__GetDiskListRsp &rsp)
{
    CCuRcsQueryDiskInfoReq cCuRcsQueryDiskInfoReq;
    cCuRcsQueryDiskInfoReq.SetSession(req.session);
    cCuRcsQueryDiskInfoReq.SetNruID(req.nruId);
    cCuRcsQueryDiskInfoReq.SetStartEntryIndex(req.beginIndex);
    cCuRcsQueryDiskInfoReq.SetExpectEntryNum(req.num);

    CCuRcsQueryDiskInfoRsp cCuRcsQueryDiskInfoRsp;
    CuGetDiskList(cCuRcsQueryDiskInfoReq, cCuRcsQueryDiskInfoRsp);

    ConverTempDiskInfoCuiToCu(cCuRcsQueryDiskInfoRsp.GetDiskInfo(), rsp.diskList);

    rsp.session = req.session;
    rsp.totalNum = cCuRcsQueryDiskInfoRsp.GetTotalEntryNum();
    rsp.errorCode = cCuRcsQueryDiskInfoRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetDeviceName(cusdk__SetDeviceNameReq req, cusdk__SetDeviceNameRsp &rsp)
{
    CCuDeviceNameModReq cCuDeviceNameModReq;
    cCuDeviceNameModReq.SetSession(req.session);
    cCuDeviceNameModReq.SetDeviceUUID(req.deviceId);
    cCuDeviceNameModReq.SetDeviceName(req.deviceName);
    
    CCuDeviceNameModRsp cCuDeviceNameModRsp;
    CuSetDeviceName(cCuDeviceNameModReq, cCuDeviceNameModRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuDeviceNameModRsp.GetErrorCode();

    return SOAP_OK;
}

// int cusdkService::SetVideoSrcName(cusdk__SetVideoSrcNameReq req, cusdk__SetVideoSrcNameRsp &rsp)
// {
//     CCuDeviceCapIndexNameSetReq cCuDeviceCapIndexNameSetReq;
//     cCuDeviceCapIndexNameSetReq.SetSession(req.session);
// 
//     TDeviceCapName tDeviceCapName;
//     tDeviceCapName.deviceId = req.deviceId;
//     tDeviceCapName.capId = req.videoSrcId;
// 
//     TDeviceCapIndexName tDeviceCapIndexName;
//     tDeviceCapIndexName.index = req.videoSrcId;
//     tDeviceCapIndexName.name = req.videoSrcName;
//     tDeviceCapName.deviceCapIndexNames.push_back(tDeviceCapIndexName);
// 
//     cCuDeviceCapIndexNameSetReq.SetDeviceCapName(tDeviceCapName);
// 
//     CCuDeviceCapIndexNameSetRsp cCuDeviceCapIndexNameSetRsp;
//     CuSetVideoSrcName(cCuDeviceCapIndexNameSetReq, cCuDeviceCapIndexNameSetRsp);
// 
//     rsp.session = req.session;
//     rsp.errorCode = cCuDeviceCapIndexNameSetRsp.GetErrorCode();
// 
//     return SOAP_OK;
// }

int cusdkService::ChangePassword(cusdk__ChangePasswordReq req, cusdk__ChangePasswordRsp &rsp)
{
	//如果客户端已经用"SM3_Base64"加密过，则透传否则按老的md5加密
	std::string strEncNewPwd = req.newPassword;
	std::string strEncOldPwd = req.oldPassword;
	if (req.passwordAlgorithm != "SM3_Base64")
	{
		MD5(req.newPassword, strEncNewPwd);
		MD5(req.oldPassword, strEncOldPwd);
	}
    CCuChangePasswordReq cCuChangePasswordReq;
    cCuChangePasswordReq.SetSession(req.session);
    cCuChangePasswordReq.SetNewPassword(strEncNewPwd);
    cCuChangePasswordReq.SetOldPassword(strEncOldPwd);

    CCMSTime cPwdExpireData(req.passwordOverdueTime);
    cCuChangePasswordReq.SetPasswordOverdueTime(cPwdExpireData.ToString());

    CCuChangePasswordRsp cCuChangePasswordRsp;
    CuChangePassword(cCuChangePasswordReq, cCuChangePasswordRsp);

    rsp.session   = req.session;
    rsp.errorCode = cCuChangePasswordRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SendKeyFrame(cusdk__SendKeyFrameReq req, cusdk__SendKeyFrameRsp &rsp)
{
    CCuKeyFrameReq cCuKeyFrameReq;
    cCuKeyFrameReq.SetSession(req.session);
    cCuKeyFrameReq.GetDevChn().SetChnNO(req.encoderChannelId);
    cCuKeyFrameReq.GetDevChn().SetDevUri(req.devId);

    CCuKeyFrameRsp cCuKeyFrameRsp;
    CuSendKeyFrame(cCuKeyFrameReq, cCuKeyFrameRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuKeyFrameRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetMotionDetectConfig(cusdk__GetMotionDetectConfigReq req, cusdk__GetMotionDetectConfigRsp &rsp)
{
    CCuPuMotionDetectGetReq cReq;
    cReq.SetSession(req.session);
    //cReq.SetUserUri(Req.);
    cReq.GetDevChn().SetChnNO(req.videoSrcId);
    cReq.GetDevChn().SetDevUri(req.deviceId);

    CCuPuMotionDetectGetRsp cRsp;
    CuGetMotionDetectConfig(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    ConverTempMotionDetectInfoCuiToCu(cRsp.GetParam(), rsp.motionDetectAreaConfigList);

    return SOAP_OK;
}

int cusdkService::SetMotionDetectConfig(cusdk__SetMotionDetectConfigReq req, cusdk__SetMotionDetectConfigRsp &rsp)
{
    CCuPuMotionDetectSetReq cReq;
    cReq.SetSession(req.session);
    cReq.GetDevChn().SetChnNO(req.videoSrcId);
    cReq.GetDevChn().SetDevUri(req.deviceId);
    ConverTempMotionDetectInfoCuToCui(req.motionDetectAreaConfigList, cReq.GetParam());

    CCuPuMotionDetectSetRsp cRsp;
    CuSetMotionDetectConfig(cReq, cRsp);
    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetShadeConfig(cusdk__GetShadeConfigReq req, cusdk__GetShadeConfigRsp &rsp)
{
    CCuPuAreaShadeGetReq cReq;
    cReq.SetSession(req.session);
    cReq.GetDevChn().SetDevUri(req.deviceId);
    cReq.GetDevChn().SetChnNO(req.videoSrcId);

    CCuPuAreaShadeGetRsp cRsp;
    CuGetShadeConfig(cReq, cRsp);

    ConverTempAreaShadeParamCuiToCu(cRsp.GetParam(), rsp.shadeAreaConfigList);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetShadeConfig(cusdk__SetShadeConfigReq req, cusdk__SetShadeConfigRsp &rsp)
{
    CCuPuAreaShadeSetReq cReq;
    cReq.SetSession(req.session);
    cReq.GetDevChn().SetChnNO(req.videoSrcId);
    cReq.GetDevChn().SetDevUri(req.deviceId);
    ConverTempAreaShadeParamCuToCui(req.shadeAreaConfigList, cReq.GetParam());

    CCuPuAreaShadeSetRsp cRsp;
    CuSetShadeConfig(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetTVDivNum(cusdk__SetTVDivNumReq req, cusdk__SetTVDivNumRsp &rsp)
{
    CCuTvWinStyleSetReq cReq;
    cReq.SetSession(req.session);
    cReq.SetTvId(req.tvId);
    cReq.SetTvWallId(req.tvwallId);
    cReq.SetStyle((ETvWinStype)req.tvDivNum);

    CCuTvWinStyleSetRsp cRsp;
    CuSetTVDivNum(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetDeviceChannelName(cusdk__GetDeviceChannelNameReq req, cusdk__GetDeviceChannelNameRsp &rsp)
{
    CCuDeviceCapIndexNameGetReq cReq;
    cReq.SetSession(req.session);
    cReq.SetDeviceId(req.deviceId);
    if( cusdk__DevChnTypeVideoSource == req.chnType)
    {
        cReq.SetCapId(DEVICE_CAP_VIDEOSOURCE);
    }
    else if( cusdk__DevChnTypeAlarmInput == req.chnType)
    {
        cReq.SetCapId(DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT);
    }
	cReq.SetCapIndex(req.chnId);

    CCuDeviceCapIndexNameGetRsp cRsp;
    CuGetDeviceChannelAlias(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    if(CMS_SUCCESS == cRsp.GetErrorCode())
    {
        ConverTempChannelAliasCuiToCu(cRsp.GetDeviceCapName().deviceCapIndexNames, rsp.chnNameList);
    }

    return SOAP_OK;
}

int cusdkService::SetDeviceChannelName(cusdk__SetDeviceChannelNameReq req, cusdk__SetDeviceChannelNameRsp &rsp)
{
    CCuDeviceCapIndexNameSetReq cReq;
    cReq.SetSession(req.session);
    cReq.GetDeviceCapName().deviceId = req.deviceId;

    if( cusdk__DevChnTypeVideoSource == req.chnType)
    {
         cReq.GetDeviceCapName().capId = DEVICE_CAP_VIDEOSOURCE;
    }
    else if( cusdk__DevChnTypeAlarmInput == req.chnType)
    {
        cReq.GetDeviceCapName().capId = DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT;
    }

    TDeviceCapIndexName tDeviceCapIndexName;
    tDeviceCapIndexName.index = req.chnId;
    tDeviceCapIndexName.name = req.chnName;
	tDeviceCapIndexName.extData.SetLon(req.longitude);
	tDeviceCapIndexName.extData.SetLat(req.latitude);

    cReq.GetDeviceCapName().deviceCapIndexNames.push_back(tDeviceCapIndexName);

    CCuDeviceCapIndexNameSetRsp cRsp;
    CuSetDeviceChannelAlias(cReq, cRsp);
   
    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::GetSyncVideoSrcNameEnable(cusdk__GetSyncVideoSrcNameEnableReq req, cusdk__GetSyncVideoSrcNameEnableRsp &rsp)
{
	CCuGetSyncVideoSrcNameEnableReq cReq;
	cReq.SetSession( req.session );

	CCuGetSyncVideoSrcNameEnableRsp cRsp;
	CuGetSyncVideoSrcNameEnableAlias( cReq, cRsp );

	cRsp.GetSwichSets( SYNC_PUSRCNAME_TO_PU, rsp.enablePlatform2Pu );
	cRsp.GetSwichSets( SYNC_PUSRCNAME_TO_PLAT, rsp.enablePu2Platform );

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::SetSyncVideoSrcNameEnable(cusdk__SetSyncVideoSrcNameEnableReq req, cusdk__SetSyncVideoSrcNameEnableRsp &rsp)
{
	CCuSetSyncVideoSrcNameEnableReq cReq;
	cReq.SetSession( req.session );
	cReq.SetSwichSets( SYNC_PUSRCNAME_TO_PU, req.enablePlatform2Pu );
	cReq.SetSwichSets( SYNC_PUSRCNAME_TO_PLAT, req.enablePu2Platform );
	
	CCuSetSyncVideoSrcNameEnableRsp cRsp;
	CuSetSyncVideoSrcNameEnableAlias( cReq, cRsp );

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::UserAddTVWall(cusdk__UserAddTVWallReq req,cusdk__UserAddTVWallRsp &rsp)
{
    TDeviceCapPrivilege tDeviceCapPrivilege;
    tDeviceCapPrivilege.id = DEVICE_CAP_DUMMY;
    CBitsContainer cBitContainer;
    cBitContainer.ConvertFromInt(-1);
    tDeviceCapPrivilege.privileges[0] = cBitContainer;

    CTvwallPrivilege tvwallPrivilege;
    tvwallPrivilege.SetPrivilege(CTvwallPrivilege::em_Tvwall_Manage);

    if ( req.type == cusdk__TypeUser )
    {
        CCuUserAddDeviceReq cCuUserAddDeviceReq;
        cCuUserAddDeviceReq.SetSession(req.session);
        vector<std::string>::iterator pTvWallItem = req.tvwallList.begin();
        while ( pTvWallItem != req.tvwallList.end() )
        {
            CUserDeviceInfo cUserDevInfo;
            cUserDevInfo.SetDeviceID(GetNameWithoutDomainFromURI(*pTvWallItem));

            cUserDevInfo.GetDevicePrivilege().ConvertFromBitsContainer(tvwallPrivilege);
            cUserDevInfo.GetDeviceCapPrivilege().push_back(tDeviceCapPrivilege);

            //需要手工设置UserID
            cUserDevInfo.SetUserID(req.id);
            cCuUserAddDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
            pTvWallItem++;
        }

        CCuUserAddDeviceRsp cCuUserAddDeviceRsp;
        CuUserAddDevice(cCuUserAddDeviceReq, cCuUserAddDeviceRsp);
        rsp.errorCode = cCuUserAddDeviceRsp.GetErrorCode();
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuUserGroupAddDeviceReq cCuUserGroupAddDeviceReq;
        cCuUserGroupAddDeviceReq.SetSession(req.session);
        vector<std::string>::iterator pTvWallUserGroupItem = req.tvwallList.begin();
        while ( pTvWallUserGroupItem != req.tvwallList.end() )
        {
            CUserGroupDeviceInfo cUserGroupDevInfo;
            cUserGroupDevInfo.SetDeviceID(GetNameWithoutDomainFromURI(*pTvWallUserGroupItem));
            
            cUserGroupDevInfo.GetDevicePrivilege().ConvertFromBitsContainer(tvwallPrivilege);
            cUserGroupDevInfo.GetDeviceCapPrivilege().push_back(tDeviceCapPrivilege);
            //ConvertUserGroupDeviceInfoFromCuToCui(*pTvWallUserGroupItem, cUserGroupDevInfo);
            //需要手工设置UserGroupID
            cUserGroupDevInfo.SetGroupID(req.id);
            cCuUserGroupAddDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);
            pTvWallUserGroupItem++;
        }

        CCuUserGroupAddDeviceRsp cCuUserGroupAddDeviceRsp;
        CuUserGroupAddDevice(cCuUserGroupAddDeviceReq, cCuUserGroupAddDeviceRsp);
        rsp.errorCode = cCuUserGroupAddDeviceRsp.GetErrorCode();
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

int cusdkService::UserDelTVWall(cusdk__UserDelTVWallReq req, cusdk__UserDelTVWallRsp &rsp)
{
    TDeviceCapPrivilege tDeviceCapPrivilege;
    tDeviceCapPrivilege.id = DEVICE_CAP_DUMMY;
    CBitsContainer cBitContainer;
    cBitContainer.ConvertFromInt(-1);
    tDeviceCapPrivilege.privileges[0] = cBitContainer;


    CTvwallPrivilege tvwallPrivilege;
    tvwallPrivilege.SetPrivilege(CTvwallPrivilege::em_Tvwall_Manage);

    if ( req.type == cusdk__TypeUser )
    {
        CCuUserDelDeviceReq cCuUserDelDeviceReq;
        cCuUserDelDeviceReq.SetSession(req.session);
//         vector<cusdk__AssignDeviceInfo>::iterator pUserDevItem = req.deviceList.begin();
//         while ( pUserDevItem != req.deviceList.end() )
//         {
//             CUserDeviceInfo cUserDevInfo;
//             ConvertUserDeviceInfoFromCuToCui(*pUserDevItem, cUserDevInfo);
//             //需要手工设置UserID
//             cUserDevInfo.SetUserID(req.id);
//             cCuUserDelDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
//             pUserDevItem++;
//         }

        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetDeviceID(GetNameWithoutDomainFromURI(req.tvwallId));
        cUserDevInfo.SetUserID(req.id);

        cUserDevInfo.GetDevicePrivilege().ConvertFromBitsContainer(tvwallPrivilege);
        cUserDevInfo.GetDeviceCapPrivilege().push_back(tDeviceCapPrivilege);
        cCuUserDelDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);

        CCuUserDelDeviceRsp cCuUserDelDeviceRsp;
        CuUserDelDevice(cCuUserDelDeviceReq, cCuUserDelDeviceRsp);
        rsp.errorCode = cCuUserDelDeviceRsp.GetErrorCode();
    }
    else if ( req.type == cusdk__TypeUserGroup )
    {
        CCuUserGroupDelDeviceReq cCuUserGroupDelDeviceReq;
        cCuUserGroupDelDeviceReq.SetSession(req.session);

        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetGroupID(req.id);
        cUserGroupDevInfo.SetDeviceID(GetNameWithoutDomainFromURI(req.tvwallId));

        cUserGroupDevInfo.GetDevicePrivilege().ConvertFromBitsContainer(tvwallPrivilege);
        cUserGroupDevInfo.GetDeviceCapPrivilege().push_back(tDeviceCapPrivilege);
        cCuUserGroupDelDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupDelDeviceRsp cCuUserGroupDelDeviceRsp;
        CuUserGroupDelDevice(cCuUserGroupDelDeviceReq, cCuUserGroupDelDeviceRsp);

        rsp.errorCode = cCuUserGroupDelDeviceRsp.GetErrorCode();
    }
    rsp.session   = req.session;
    return SOAP_OK;
}

int cusdkService::UserGetTVWall(cusdk__UserGetTVWallReq req, cusdk__UserGetTVWallRsp &rsp)
{
    CCuTvwallQryReq cCuTvwallQryReq;
    cCuTvwallQryReq.SetSession(req.session);

    CTvwallInfoQueryCondition condition;
    if( cusdk__TypeUser == req.type)
    {
        condition.SetCuserId(req.id);
    }
    else if(cusdk__TypeUserGroup == req.type)
    {
        condition.SetCuserGrpId(req.id);
    }
    cCuTvwallQryReq.SetExpectEntryNum(req.num);
    cCuTvwallQryReq.SetStartEntryIndex(req.beginIndex);
    cCuTvwallQryReq.SetCondition(condition);

    CCuTvwallQryRsp cCuTvwallQryRsp;
    CuGetAllTVWall(cCuTvwallQryReq, cCuTvwallQryRsp);

    std::vector<cusdk__TVWall > tvWallList;
    ConverTempTvwallIdFromCuiToCu(cCuTvwallQryRsp.GetTvwallInfo(), rsp.tvWallIdList);
    rsp.totalNum = cCuTvwallQryRsp.GetTotalEntryNum();

    rsp.errorCode = cCuTvwallQryRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

int cusdkService::GetSupportMapTypes(cusdk__GetSupportMapTypesReq req, cusdk__GetSupportMapTypesRsp &rsp)
{
    CCuGetSupportMapTypesReq cCuGetSupportMapTypesReq;
    cCuGetSupportMapTypesReq.SetSession(req.session);

    CCuGetSupportMapTypesRsp cCuGetSupportMapTypesRsp;
    CuGetSupportMapTypes(cCuGetSupportMapTypesReq, cCuGetSupportMapTypesRsp);

    ConverTempMapTypeInfoFromCuiToCu(cCuGetSupportMapTypesRsp.GetSupportTypeList(), rsp.supportMapTypes);

    rsp.errorCode = cCuGetSupportMapTypesRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}
int cusdkService::QueryMapArea( cusdk__QueryMapAreaReq req, cusdk__QueryMapAreaRsp &rsp)
{
    CCuMapAreaQryReq cCuMapAreaQryReq;
    cCuMapAreaQryReq.SetSession(req.session);
    cCuMapAreaQryReq.SetExpectEntryNum(req.num);
    cCuMapAreaQryReq.SetStartEntryIndex(req.beginIndex);

    MapAreaCondition condition;

    if(req.areaIdEnabled)
    {
        condition.SetAreaId(req.areaId);
    }

    if(req.parentMapIdEnabled)
    {
        condition.SetOwnId(req.parentMapId);
    }

    if(req.refMapIdEnabled)
    {
        condition.SetRefMapId(req.refMapId);
    }

    cCuMapAreaQryReq.SetCondition(condition);

    CCuMapAreaQryRsp cCuMapAreaQryRsp;
    CuQueryMapArea(cCuMapAreaQryReq, cCuMapAreaQryRsp);

    ConverTempMapAreaFromCuiToCu(cCuMapAreaQryRsp.GetResults(), rsp.areaList);

    rsp.session = req.session;
    rsp.errorCode = cCuMapAreaQryRsp.GetErrorCode();
    rsp.totalNum = cCuMapAreaQryRsp.GetTotalEntryNum();

    return SOAP_OK;
}
int cusdkService::CreateMapArea( cusdk__CreateMapAreaReq req, cusdk__CreateMapAreaRsp &rsp)
{
    CCuMapAreaCreateReq cCuMapAreaCreateReq;
    cCuMapAreaCreateReq.SetSession(req.session);
    
    cCuMapAreaCreateReq.GetArea().SetOwnId(req.mapAreaInfo.parentMapId);
    cCuMapAreaCreateReq.GetArea().SetCustomStyle(req.mapAreaInfo.customStyle);
    cCuMapAreaCreateReq.GetArea().SetLayerId(req.mapAreaInfo.layerId);
    cCuMapAreaCreateReq.GetArea().SetOpacity(req.mapAreaInfo.opacity);
    cCuMapAreaCreateReq.GetArea().SetUuid(req.mapAreaInfo.areaId);
    cCuMapAreaCreateReq.GetArea().SetRefMapId(req.mapAreaInfo.refMapId);
    cCuMapAreaCreateReq.GetArea().SetColor(req.mapAreaInfo.layerColor);

    ConverTempMapPositionCuToCui(req.mapAreaInfo.trace, cCuMapAreaCreateReq.GetArea().GetTrace());

    CCuMapAreaCreateRsp cCuMapAreaCreateRsp;
    CuCreateMapArea(cCuMapAreaCreateReq, cCuMapAreaCreateRsp);

    rsp.mapAreaId = cCuMapAreaCreateRsp.GetAreaId();
    rsp.session = req.session;
    rsp.errorCode = cCuMapAreaCreateRsp.GetErrorCode();
    
    return SOAP_OK;
}
int cusdkService::ModifyMapArea( cusdk__ModifyMapAreaReq req, cusdk__ModifyMapAreaRsp &rsp)
{
    CCuMapAreaModReq cCuMapAreaModReq;
    cCuMapAreaModReq.SetSession(req.session);

    cCuMapAreaModReq.GetArea().SetOwnId(req.mapAreaInfo.parentMapId);
    cCuMapAreaModReq.GetArea().SetCustomStyle(req.mapAreaInfo.customStyle);
    cCuMapAreaModReq.GetArea().SetLayerId(req.mapAreaInfo.layerId);
    cCuMapAreaModReq.GetArea().SetOpacity(req.mapAreaInfo.opacity);
    cCuMapAreaModReq.GetArea().SetUuid(req.mapAreaInfo.areaId);
    cCuMapAreaModReq.GetArea().SetRefMapId(req.mapAreaInfo.refMapId);
    cCuMapAreaModReq.GetArea().SetColor(req.mapAreaInfo.layerColor);

    ConverTempMapPositionCuToCui(req.mapAreaInfo.trace, cCuMapAreaModReq.GetArea().GetTrace());

    CCuMapAreaModRsp cCuMapAreaModRsp;
    CuModifyMapArea(cCuMapAreaModReq, cCuMapAreaModRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapAreaModRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::VsipChannelData( cusdk__VsipChannelDataReq req, cusdk__VsipChannelDataRsp &rsp)
{
    CCuPuVsipOperateReq cReq;
    cReq.SetSession(req.session);

    cReq.SetDevUri(req.deviceId);
    cReq.SetSubDevNo(req.deviceIndex);
    cReq.SetVsipData(req.vsipBase64DataReq);

    CCuPuVsipOperateRsp cRsp;
    CuVsipChannelData(cReq, cRsp);

    rsp.vsipBase64DataRsp = cRsp.GetVsipData();
    rsp.errorCode = cRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

int cusdkService::SetTransDataParam( cusdk__SetTransDataParamReq req, cusdk__SetTransDataParamRsp &rsp )
{
    // hzytodo
    return SOAP_OK;
}

int cusdkService::GetTransDataParam( cusdk__GetTransDataParamReq req, cusdk__GetTransDataParamRsp &rsp )
{
    // hzytodo
    return SOAP_OK;
}


int cusdkService::QueryGPSTrace( cusdk__QueryGPSTraceReq req, cusdk__QueryGPSTraceRsp &rsp )
{
	CCuGpsTraceQryReq cReq;
	cReq.SetSession(req.session);

	TTimeRange timeRange;
	CCMSTime begin(req.timeRange.startTime);
	timeRange.SetStartTime(begin.ToString());
	CCMSTime end(req.timeRange.endTime);
	timeRange.SetEndTime(end.ToString());

	TraceCondition	condition;
	condition.SetDevUri(req.deviceId);
	condition.SetTimeRange(timeRange);

	cReq.SetCondition(condition);
	cReq.SetStartEntryIndex(req.beginIndex);
	cReq.SetExpectEntryNum(req.num);

	CCuGpsTraceQryRsp cRsp;
	CuQueryGpsTrece(cReq,cRsp);

	ConverTempGpsTraceCuiToCu(cRsp.GetInfo(),rsp.gpsInfoList);
	rsp.errorCode = cRsp.GetErrorCode();
	rsp.session = req.session;
	rsp.totalNum = cRsp.GetTotalEntryNum();

    return SOAP_OK;
}

//此功能暂时不用
int cusdkService::GetMapConfig(cusdk__GetMapConfigReq req, cusdk__GetMapConfigRsp &rsp)
{
	/*CCuMpsGpsStoreCfgGetReq cReq;
	cReq.SetSession(req.session);
	CCuMpsGpsStoreCfgGetRsp cRsp;

	CuGpsStoreCfgGet(cReq,cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();
	rsp.maxGPSInfoSavedDays = cRsp.GetStoreCfg().GetMaxGpsDayCount();
	//rsp.maxGPSInfoSavedDeviceNumber = cRsp.GetStoreCfg().GetGpsSamplingCoff();
	*/
	return SOAP_OK; // hzytodo
}

int cusdkService::SetMapConfig(cusdk__SetMapConfigReq req, cusdk__SetMapConfigRsp &rsp)
{
	return SOAP_OK; // hzytodo
}


int cusdkService::GetDeviceSerialConfig( cusdk__GetDeviceSerialConfigReq req, cusdk__GetDeviceSerialConfigRsp &rsp )
{
    CCuPuSerialParamCfgGetReq cReq;
    cReq.SetSession(req.session);
    cReq.SetDevUri(req.deviceId);
    cReq.GetPuSerPortParam().SetSerialPortID(req.serialPortID);

    CCuPuSerialParamCfgGetRsp cRsp;
    CuGetDeviceSerialConfig(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();
    rsp.deviceId = cRsp.GetDevUri();
    rsp.serialPortID = cRsp.GetPuSerPortParam().GetSerialPortID();
    rsp.baudRate = cRsp.GetPuSerPortParam().GetBaudRate();
    rsp.comPortType = cRsp.GetPuSerPortParam().GetComPortType();
    rsp.rS232Enable = cRsp.GetPuSerPortParam().GetRS232Enable();
    rsp.rS422Enable = cRsp.GetPuSerPortParam().GetRS422Enable();
    rsp.rS485Enable = cRsp.GetPuSerPortParam().GetRS485Enable();

    return SOAP_OK;
}

int cusdkService::SetDeviceSerialConfig( cusdk__SetDeviceSerialConfigReq req, cusdk__SetDeviceSerialConfigRsp &rsp )
{
    CCuPuSerialParamCfgSetReq cReq;
    cReq.SetSession(req.session);
    cReq.SetDevUri(req.deviceId);
    cReq.GetPuSerPortParam().SetSerialPortID(req.serialPortID);
    cReq.GetPuSerPortParam().SetBaudRate(req.baudRate);
    cReq.GetPuSerPortParam().SetComPortType(req.comPortType);

    CCuPuSerialParamCfgSetRsp cRsp;
    CuSetDeviceSerialConfig(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}


int cusdkService::StartPuRecord(cusdk__StartPuRecordReq req, cusdk__StartPuRecordRsp &rsp)
{
	CCuPuRecordStartReq cCuPuRecordStartReq;
	cCuPuRecordStartReq.SetSession(req.session);

	TChannel tChn;
	tChn.SetChnNO(req.devChn.channelId);
	tChn.SetDevUri(req.devChn.deviceId);
	cCuPuRecordStartReq.SetDevChn(tChn);

	CCuPuRecordStartRsp cCuPuRecordStartRsp;
	CuStartPuRecord(cCuPuRecordStartReq, cCuPuRecordStartRsp);

	rsp.errorCode = cCuPuRecordStartRsp.GetErrorCode();
	rsp.session = req.session;

	return SOAP_OK;
}

int cusdkService::StopPuRecord(cusdk__StopPuRecordReq req, cusdk__StopPuRecordRsp &rsp)
{
	CCuPuRecordStopReq cCuPuRecordStopReq;
	cCuPuRecordStopReq.SetSession(req.session);

	TChannel tChn;
	tChn.SetChnNO(req.devChn.channelId);
	tChn.SetDevUri(req.devChn.deviceId);
	cCuPuRecordStopReq.SetDevChn(tChn);

	CCuPuRecordStopRsp cCuPuRecordStopRsp;
	CuStopPuRecord(cCuPuRecordStopReq, cCuPuRecordStopRsp);

	rsp.errorCode = cCuPuRecordStopRsp.GetErrorCode();
	rsp.session = req.session;

	return SOAP_OK;
}

int cusdkService::GetMapImageUploadUrl(cusdk__GetMapImageUploadUrlReq req, cusdk__GetMapImageUploadUrlRsp &rsp)
{
    CCuMapUploadReq cCuMapUploadReq;
    cCuMapUploadReq.SetSession(req.session);
    cCuMapUploadReq.SetParentMapId(req.parentMapId);
    //cCuMapUploadReq.SetMapType(req.);

    CCuMapUploadRsp cCuMapUploadRsp;
    CuGetMapImageUploadUrl(cCuMapUploadReq, cCuMapUploadRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuMapUploadRsp.GetErrorCode();
    rsp.imageUploadUrl = cCuMapUploadRsp.GetUploadUrl();

    return SOAP_OK;
}

int cusdkService::GetMapElementImageUploadUrl(cusdk__GetMapElementImageUploadUrlReq req, cusdk__GetMapElementImageUploadUrlRsp &rsp)
{
    CCuElementIconUploadReq cCuElementIconUploadReq;
    cCuElementIconUploadReq.SetSession(req.session);
    cCuElementIconUploadReq.SetIconName(req.imageName);

    CCuElementIconUploadRsp cCuElementIconUploadRsp;
    CuGetMapElementImageUploadUrl(cCuElementIconUploadReq, cCuElementIconUploadRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuElementIconUploadRsp.GetErrorCode();
    rsp.imageUploadUrl = cCuElementIconUploadRsp.GetUploadUrl();

    return SOAP_OK;
}

int cusdkService::QueryMapElementImageList(cusdk__QueryMapElementImageListReq req, cusdk__QueryMapElementImageListRsp &rsp)
{
    CCuElementImageQryReq cCuElementImageQryReq;
    cCuElementImageQryReq.SetSession(req.session);

    CCuElementImageQryRsp cCuElementImageQryRsp;
    CuQueryMapElementImageList(cCuElementImageQryReq, cCuElementImageQryRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuElementImageQryRsp.GetErrorCode();
    rsp.mapElementImageUrlList = cCuElementImageQryRsp.GetImageList();

    return SOAP_OK;
}

int cusdkService::QueryAlarmLinkArea(cusdk__QueryAlarmLinkAreaReq req, cusdk__QueryAlarmLinkAreaRsp &rsp)
{
    return SOAP_OK;
}

int cusdkService::CreateAlarmLinkArea(cusdk__CreateAlarmLinkAreaReq req, cusdk__CreateAlarmLinkAreaRsp &rsp)
{
    return SOAP_OK;
}

int cusdkService::DelAlarmLinkArea(cusdk__DelAlarmLinkAreaReq req, cusdk__DelAlarmLinkAreaRsp &rsp)
{
    return SOAP_OK;
}

int cusdkService::ModifyAlarmLinkArea(cusdk__ModifyAlarmLinkAreaReq req, cusdk__ModifyAlarmLinkAreaRsp &rsp)
{
    return SOAP_OK;
}

int cusdkService::GetCameraProtocolType(cusdk__GetCameraProtocolTypeReq req, cusdk__GetCameraProtocolTypeRsp &rsp)
{
    return SOAP_OK;
}

int cusdkService::SetCameraProtocolType(cusdk__SetCameraProtocolTypeReq req, cusdk__SetCameraProtocolTypeRsp &rsp)
{
    CCuPuCameraTypeSetReq cReq;
    cReq.SetSession(req.session);
    cReq.GetDevChn().SetDevUri(req.deviceId);
    cReq.GetDevChn().SetChnNO(req.videoSrcId);
    cReq.SetCameraType(req.cameraProtocolType);
	cReq.SetCameraId(req.cameraId);

    CCuPuCameraTypeSetRsp cRsp;
    CuSetCameraProtocolType(cReq, cRsp);

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::EliminateAlarm(cusdk__EliminateAlarmReq req, cusdk__EliminateAlarmRsp &rsp)
{
	CCuPuAlarmResetReq cReq;
	cReq.SetSession(req.session);
	cReq.GetDevChn().SetDevUri(req.deviceId);
	cReq.GetDevChn().SetChnNO(req.vsId);

	CCuPuAlarmResetRsp cRsp;
	CuResetAlarm(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetDeviceRecordsServerUrl(cusdk__GetDeviceRecordsServerUrlReq req, cusdk__GetDeviceRecordsServerUrlRsp &rsp)
{
    CDeviceRecordSvrQryReq cReq;
    cReq.SetSession( req.session );
    CDeviceRecordSvrQryRsp cRsp;

    CuDeviceRecordSvrQry( cReq, cRsp );
    const CDevRecordSvrUriList &uriLst = cRsp.GetUriList();
    for ( size_t i=0; i!=uriLst.size(); ++i )
    {
        rsp.deviceRecordsServerUrls.push_back( uriLst[i] );
    }
    rsp.session  = cRsp.GetSession();
    rsp.errorCode= cRsp.GetErrorCode();

    return SOAP_OK;
}


int cusdkService::QueryRecord(cusdk__QueryRecordReq req, cusdk__QueryRecordRsp &rsp)
{
    CCMSTime endTime(req.timeRange.endTime);
    CCMSTime startTime(req.timeRange.startTime);

    TTimeRange tTimeRange;
    tTimeRange.SetEndTime(endTime.ToString());
    tTimeRange.SetStartTime(startTime.ToString());

    TChannel tChannel;
    tChannel.SetChnNO(req.devChn.channelId);
    tChannel.SetDevUri(req.devChn.deviceId);

    //cusdk__ERecordTypePlatformRecord = 1, cusdk__ERecordTypePURecord = 2
    if(req.recordType == cusdk__ERecordTypePlatformRecord)
    {
        CCuRcsQueryRecordReq cCuRcsQueryRecordReq;
        cCuRcsQueryRecordReq.SetSession(req.session);

        CRcsQueryRecordReq cRcsQueryRecordReq;    
        cRcsQueryRecordReq.SetSession(req.session);

        cRcsQueryRecordReq.SetPuChn(tChannel);

        switch(req.recEventType)
        {
        case cusdk__RS_RECTYPE_ALL:
            {
                //此项不填写表示查询所有录像
                //cCuRcsQueryRecordReq.SetRecType();
            }
            break;
        case cusdk__RS_RECTYPE_ALARM:
            {
                cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_ALARM);
            }
            break;
        case cusdk__RS_RECTYPE_HANDLE:
            {
                cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_HANDLE);
            }
            break;
        case cusdk__RS_RECTYPE_PLAN:
            {
                cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_PLAN);
            }
            break;
        case cusdk__RS_RECTYPE_UNKNOWN:
            {
                cRcsQueryRecordReq.SetRecType(RCS_RECTYPE_UNKNOWN);
            }
            break;
        default:
            break;
        }

        cRcsQueryRecordReq.SetTimeRange(tTimeRange);

        cCuRcsQueryRecordReq.SetReq(cRcsQueryRecordReq.ToXml());
        cCuRcsQueryRecordReq.SetRecordDomainName(req.recordDomainName);

        CCuRcsQueryRecordRsp cCuRcsQueryRecordRsp;
        CuGetPlatformRecord(cCuRcsQueryRecordReq, cCuRcsQueryRecordRsp);

        ConverTempRecordDurationFromCuiToCu(cCuRcsQueryRecordRsp.GetDurantionList(), rsp.recordDurationList, cCuRcsQueryRecordRsp.GetDomainName());

        rsp.errorCode = cCuRcsQueryRecordRsp.GetErrorCode();
    }
    else if(req.recordType == cusdk__ERecordTypePURecord)
    {
        CCuPuRecQryReq cCuPuRecQryReq;
        cCuPuRecQryReq.SetSession(req.session);
        cCuPuRecQryReq.SetTimeRange(tTimeRange);
        cCuPuRecQryReq.SetDevChn(tChannel);
        cCuPuRecQryReq.SetRecType(PU_EVENT_TYPE_ALL);
        cCuPuRecQryReq.SetQueryHandle(req.queryHandle);
        cCuPuRecQryReq.SetGMTLabel(req.useVSIPTimeAsGMTTime);
      
        CCuPuRecQryRsp cCuPuRecQryRsp;
        CuPuRecord(cCuPuRecQryReq, cCuPuRecQryRsp);
        rsp.queryHandle = cCuPuRecQryRsp.GetQueryHandle();
        rsp.hasLeft = cCuPuRecQryRsp.GetHasLeft();
        ConverTempPuRecInfoFromCuiToCu(cCuPuRecQryRsp.GetRecInfoList(), rsp.recordDurationList);

        rsp.errorCode = cCuPuRecQryRsp.GetErrorCode();
		rsp.useVSIPTimeAsGMTTime = cCuPuRecQryRsp.GetIsUseVSIPTimeAsGMTTime();
    }
    
    rsp.session   = req.session;
    rsp.devChn    = req.devChn;

    return SOAP_OK;
}

int cusdkService::SetupPlayRecord(cusdk__SetupPlayRecordReq req, cusdk__SetupPlayRecordRsp &rsp)
{

    TChannel tSrcChn;
    tSrcChn.SetDevUri(req.recordPlayDuration.encoderChn.deviceId);
    tSrcChn.SetChnNO(req.recordPlayDuration.encoderChn.channelId);

    //目的是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(req.session);
    tCuChn.SetChnNO(req.playId);

    TSdp tSdp;
    tSdp.SetOwner(tCuChn);

    tSdp.SetSessionName(SDP_SESSION_PLAYBACK);

    TSdpUri tSdpUri;
    tSdpUri.SetRecToken(req.recordPlayDuration.recordDurationId);
    tSdpUri.SetPuChn(tSrcChn);

    TTimeRange tTimeRange;
    CCMSTime begin(req.recordPlayDuration.durationTimeRange.startTime);
    CCMSTime end(req.recordPlayDuration.durationTimeRange.endTime);
    tTimeRange.SetStartTime(begin.ToString());
    tTimeRange.SetEndTime(end.ToString());
    tSdp.SetTimeRange(tTimeRange);

	//设置MRTC使能
	if (cusdk__TransModeMRTC == req.transMode)
	{
		tSdp.SetMrtcEnable(true);
	}

    if(req.recordPlayDuration.recordType == cusdk__ERecordTypePlatformRecord)
    {
        CCuSetupPlayPlatformRecordReq cCuSetupPlayPlatformRecordReq;
        cCuSetupPlayPlatformRecordReq.SetSession(req.session);
        cCuSetupPlayPlatformRecordReq.SetDomainName(req.recordPlayDuration.recordDomainName);
        CInviteReq cSetupStreamReq;
        cSetupStreamReq.SetSession(req.session);

        cSetupStreamReq.SetSrcChn(tSrcChn);
        cSetupStreamReq.SetDstChn(tCuChn);
        tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PLATFORM);

        if( req.directTrans)
        {
            tSdp.SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, TRUE_STR);
        }

        tSdp.SetUri(tSdpUri);

        //遍历每个CU的IP
        ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

        cSetupStreamReq.SetSdp(tSdp);
        cCuSetupPlayPlatformRecordReq.SetReq(cSetupStreamReq);

        CCuSetupPlayPlatformRecordRsp cCuSetupPlayPlatformRecordRsp;
        CuSetupPlayPlatformRecord( cCuSetupPlayPlatformRecordReq, cCuSetupPlayPlatformRecordRsp );

        rsp.errorCode = cCuSetupPlayPlatformRecordRsp.GetErrorCode();
        rsp.recordPlaySession = cCuSetupPlayPlatformRecordRsp.GetSdp().GetPlaybackSessUri();
        if ( CMS_SUCCESS == cCuSetupPlayPlatformRecordRsp.GetErrorCode() )
        {
            ConvertTempMediaDescAndExtInfoCuiToCu(cCuSetupPlayPlatformRecordRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
                cCuSetupPlayPlatformRecordRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
        }
    }
    else if(req.recordPlayDuration.recordType == cusdk__ERecordTypePURecord)
    {
        CCuSetupStreamReq cCuSetupStreamReq;
        cCuSetupStreamReq.SetEvent(CU_CUI_INVITE_PLAY_PURECORD_REQ);
        cCuSetupStreamReq.SetSession(req.session);

        cCuSetupStreamReq.SetSrcChn(tSrcChn);
        cCuSetupStreamReq.SetDstChn(tCuChn);
        tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PU);

        if( req.directTrans)
        {
            tSdp.SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, TRUE_STR);
        }

		//设置平台直传
		if (req.directTransFromDeviceDomain)
		{
			tSdp.SetPlatDirectTransfer(true);
		}

        tSdp.SetUri(tSdpUri);

        //遍历每个CU的IP
        ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

        cCuSetupStreamReq.SetSdp(tSdp);

        CCuSetupStreamRsp cCuSetupStreamRsp;
        CuSetupStream( cCuSetupStreamReq, cCuSetupStreamRsp );

        rsp.errorCode = cCuSetupStreamRsp.GetErrorCode();
        rsp.recordPlaySession = cCuSetupStreamRsp.GetSdp().GetPlaybackSessUri();
        if ( CMS_SUCCESS == cCuSetupStreamRsp.GetErrorCode() )
        {
            ConvertTempMediaDescAndExtInfoCuiToCu(cCuSetupStreamRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
                cCuSetupStreamRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
        }
    }

    rsp.session   = req.session;
    
    return SOAP_OK;
}

int cusdkService::PlayRecord(cusdk__PlayRecordReq req, cusdk__PlayRecordRsp &rsp)
{
    CCuPlayPlatformRecordReq cCuPlayPlatformRecordReq;
    cCuPlayPlatformRecordReq.SetSession(req.session);
    cCuPlayPlatformRecordReq.SetPlayId(req.playId);

    CCuPlayPlatformRecordRsp cCuPlayPlatformRecordRsp;
    CuPlayPlatformRecord(cCuPlayPlatformRecordReq, cCuPlayPlatformRecordRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuPlayPlatformRecordRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::StopPlayRecord(cusdk__StopPlayRecordReq req, cusdk__StopPlayRecordRsp &rsp)
{
    CCuStopPlatformRecordReq cCuStopPlatformRecordReq;
    cCuStopPlatformRecordReq.SetSession(req.session);
    cCuStopPlatformRecordReq.SetPlayId(req.playId);

    CCuStopPlatformRecordRsp cCuStopPlatformRecordRsp;
    CuStopPlayPlatformRecord(cCuStopPlatformRecordReq, cCuStopPlatformRecordRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuStopPlatformRecordRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::SetupDownloadRecord(cusdk__SetupDownloadRecordReq req, cusdk__SetupDownloadRecordRsp &rsp)
{
    //源是PU
    TChannel tSrcChn;
    tSrcChn.SetDevUri(req.recordPlayDuration.encoderChn.deviceId);
    tSrcChn.SetChnNO(req.recordPlayDuration.encoderChn.channelId);

    //目的是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(req.session);
    tCuChn.SetChnNO(req.playId);

    TSdp tSdp;
    tSdp.SetOwner(tCuChn);
    tSdp.SetSessionName(SDP_SESSION_DOWNLOAD);

    TSdpUri tSdpUri;
    tSdpUri.SetRecToken(req.recordPlayDuration.recordDurationId);
    tSdpUri.SetPuChn(tSrcChn);

    TTimeRange tTimeRange;
    CCMSTime begin(req.recordPlayDuration.durationTimeRange.startTime);
    CCMSTime end(req.recordPlayDuration.durationTimeRange.endTime);
    tTimeRange.SetStartTime(begin.ToString());
    tTimeRange.SetEndTime(end.ToString());
    tSdp.SetTimeRange(tTimeRange);

	//设置MRTC使能
	if (cusdk__TransModeMRTC == req.transMode)
	{
		tSdp.SetMrtcEnable(true);
	}

    if(req.recordPlayDuration.recordType == cusdk__ERecordTypePlatformRecord)
    {
        CCuDownloadPlatformRecordReq cCuDownloadPlatformRecordReq;
        cCuDownloadPlatformRecordReq.SetDomainName(req.recordPlayDuration.recordDomainName);
        CInviteReq cSetupStreamReq;
        cSetupStreamReq.SetSession(req.session);

        cSetupStreamReq.SetSrcChn(tSrcChn);
        cSetupStreamReq.SetDstChn(tCuChn);

        
        //只有前端放像或者平台放像才要填写
        tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PLATFORM);
        if( req.directTrans)
        {
            tSdp.SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, TRUE_STR);
        }
       
        tSdp.SetUri(tSdpUri);
        //rsp消息中的TSdp才携带
        //TSdp.SetPlaybackSessUri(req.recordPlayDuration.);

        //遍历每个CU的IP
        ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

        cSetupStreamReq.SetSdp(tSdp);
        cCuDownloadPlatformRecordReq.SetReq(cSetupStreamReq);

        CCuDownloadPlatformRecordRsp cCuDownloadPlatformRecordRsp;
        CuSetupDownloadPlatformRecord( cCuDownloadPlatformRecordReq, cCuDownloadPlatformRecordRsp );

        if ( CMS_SUCCESS == cCuDownloadPlatformRecordRsp.GetErrorCode() )
        {      
            rsp.recordFilePath = cCuDownloadPlatformRecordRsp.GetSdp().GetDLFimePath();
            rsp.recordFileName = cCuDownloadPlatformRecordRsp.GetSdp().GetDLFimeName();

            ConvertTempMediaDescAndExtInfoCuiToCu(cCuDownloadPlatformRecordRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
                cCuDownloadPlatformRecordRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
        }
        rsp.errorCode = cCuDownloadPlatformRecordRsp.GetErrorCode();
    }
    else if(req.recordPlayDuration.recordType == cusdk__ERecordTypePURecord)
    {
        CCuSetupStreamReq cCuSetupStreamReq;
        cCuSetupStreamReq.SetEvent(CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ);
        cCuSetupStreamReq.SetSession(req.session);

        cCuSetupStreamReq.SetSrcChn(tSrcChn);
        cCuSetupStreamReq.SetDstChn(tCuChn);
        tSdpUri.SetPlaybackType(PLAYBACK_TYPE_PU);
        if( req.directTrans)
        {
            tSdp.SetExtInfo(SDP_EXT_INFO_DIRECT_TRANSFER, TRUE_STR);
        }

		//设置平台直传
		if (req.directTransFromDeviceDomain)
		{
			tSdp.SetPlatDirectTransfer(true);
		}
        
        tSdp.SetUri(tSdpUri);

        //遍历每个CU的IP
        ConverTempMediaDescListCuToCui(req.mediaDescriptionList, tSdp.GetMediaDescList());

        cCuSetupStreamReq.SetSdp(tSdp);

        CCuSetupStreamRsp cCuSetupStreamRsp;
        CuSetupStream( cCuSetupStreamReq, cCuSetupStreamRsp );

        rsp.errorCode = cCuSetupStreamRsp.GetErrorCode();
        if ( CMS_SUCCESS == cCuSetupStreamRsp.GetErrorCode() )
        {
            rsp.recordFilePath = cCuSetupStreamRsp.GetSdp().GetDLFimePath();
            rsp.recordFileName = cCuSetupStreamRsp.GetSdp().GetDLFimeName();

            ConvertTempMediaDescAndExtInfoCuiToCu(cCuSetupStreamRsp.GetSdp().GetMediaDescList(), rsp.mediaDescriptionList,
                cCuSetupStreamRsp.GetSdp().GetExtInfo(), rsp.mediaExtInfo);
        }
    }

    rsp.session   = req.session;

    return SOAP_OK;
}

int cusdkService::StartDownloadRecord( cusdk__StartDownloadRecordReq req, cusdk__StartDownloadRecordRsp &rsp)
{
    CCuStartDownloadPlatformRecordReq cCuStartDownloadPlatformRecordReq;
    cCuStartDownloadPlatformRecordReq.SetSession(req.session);
    cCuStartDownloadPlatformRecordReq.SetPlayId(req.playId);

    CCuStartDownloadPlatformRecordRsp cCuStartDownloadPlatformRecordRsp;
    CuStartDownloadPlatformRecord(cCuStartDownloadPlatformRecordReq, cCuStartDownloadPlatformRecordRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuStartDownloadPlatformRecordRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::StopDownloadRecord( cusdk__StopDownloadRecordReq req, cusdk__StopDownloadRecordRsp &rsp)
{
    CCuStopDownloadPlatformRecordReq cCuStopDownloadPlatformRecordReq;
    cCuStopDownloadPlatformRecordReq.SetSession(req.session);
    cCuStopDownloadPlatformRecordReq.SetPlayId(req.playId);

    CCuStopDownloadPlatformRecordRsp cCuStopDownloadPlatformRecordRsp;
    CuStopDownloadPlatformRecord(cCuStopDownloadPlatformRecordReq, cCuStopDownloadPlatformRecordRsp);

    rsp.errorCode = cCuStopDownloadPlatformRecordRsp.GetErrorCode();
    rsp.session = req.session;

    return SOAP_OK;
}

int cusdkService::GetPtzPresetName( cusdk__GetPtzPresetNameReq req, cusdk__GetPtzPresetNameRsp &rsp)
{
    CCuVideosourcePresetcfgQryReq cCuVideosourcePresetcfgQryReq;
    cCuVideosourcePresetcfgQryReq.SetSession(req.session);
    cCuVideosourcePresetcfgQryReq.GetCondition().SetDevUri(req.deviceId);
    cCuVideosourcePresetcfgQryReq.GetCondition().SetVideosourceId(req.vieoSrcId);
    //cCuVideosourcePresetcfgQryReq.GetCondition().SetPositionId(req.);

    CCuVideosourcePresetcfgQryRsp cCuVideosourcePresetcfgQryRsp;
    CuGetPtzPresetName(cCuVideosourcePresetcfgQryReq, cCuVideosourcePresetcfgQryRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuVideosourcePresetcfgQryRsp.GetErrorCode();
    ConverTempVideosourcePresetcfgListCuiToCu(cCuVideosourcePresetcfgQryRsp.GetInfos(), rsp.presetNameList);

    return SOAP_OK;
}

int cusdkService::SetPtzPresetName( cusdk__SetPtzPresetNameReq req, cusdk__SetPtzPresetNameRsp &rsp)
{
    CCuVideosourcePresetcfgAddReq cCuVideosourcePresetcfgAddReq;   
    cCuVideosourcePresetcfgAddReq.SetSession(req.session);
    cCuVideosourcePresetcfgAddReq.GetInfo().devUri = req.deviceId;
    cCuVideosourcePresetcfgAddReq.GetInfo().videosourceId = req.vieoSrcId;
    TPresetPositionName tPresetPositionName;
    tPresetPositionName.posId = req.presetId;
    tPresetPositionName.posName = req.presetName;
    cCuVideosourcePresetcfgAddReq.GetInfo().posNames.push_back(tPresetPositionName);

    CCuVideosourcePresetcfgAddRsp cCuVideosourcePresetcfgAddRsp;
    CuSetPtzPresetName(cCuVideosourcePresetcfgAddReq, cCuVideosourcePresetcfgAddRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuVideosourcePresetcfgAddRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::DelPtzPresetName( cusdk__DelPtzPresetNameReq req, cusdk__DelPtzPresetNameRsp &rsp)
{
    CCuVideosourcePresetcfgDelReq cCuVideosourcePresetcfgDelReq;
    cCuVideosourcePresetcfgDelReq.SetSession(req.session);
    cCuVideosourcePresetcfgDelReq.GetKey().SetDevUri(req.deviceId);
    cCuVideosourcePresetcfgDelReq.GetKey().SetPositionId(req.presetId);
    cCuVideosourcePresetcfgDelReq.GetKey().SetVideosourceId(req.vieoSrcId);

    CCuVideosourcePresetcfgDelRsp cCuVideosourcePresetcfgDelRsp;
    CuDelPtzPresetName(cCuVideosourcePresetcfgDelReq, cCuVideosourcePresetcfgDelRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuVideosourcePresetcfgDelRsp.GetErrorCode();

    return SOAP_OK;
}

int cusdkService::QueryRecordDays(cusdk__QueryRecordDaysReq req,cusdk__QueryRecordDaysRsp &rsp)
{
    if(cusdk__ERecordTypePlatformRecord == req.recordType)
    {
        CCuRcsQueryRecordDaysReq cCuRcsQueryRecordDaysReq;
        cCuRcsQueryRecordDaysReq.SetSession(req.session);

        CRcsQueryRecordDaysReq cRcsQueryRecordDaysReq;
        cRcsQueryRecordDaysReq.SetSession(req.session);
        CCMSTime startTime(req.timeRange.startTime);
        CCMSTime endTime(req.timeRange.endTime);
        TTimeRange timeRange;
        timeRange.SetStartTime(startTime.ToString());
        timeRange.SetEndTime(endTime.ToString());
        cRcsQueryRecordDaysReq.SetTimeRange(timeRange);
        ConverTempChnFromCuToCui(req.devChnList, cRcsQueryRecordDaysReq.GetPuChnList());

        string strReq = cRcsQueryRecordDaysReq.ToXml();
        cCuRcsQueryRecordDaysReq.SetReq(strReq);
        cCuRcsQueryRecordDaysReq.SetRecordDomainName(req.recordDomainName);

        CCuRcsQueryRecordDaysRsp cCuRcsQueryRecordDaysRsp;
        CuGetPlatformRecordDays(cCuRcsQueryRecordDaysReq, cCuRcsQueryRecordDaysRsp);

        ConverTempRecordDayFromCuiToCu(cCuRcsQueryRecordDaysRsp.GetPuChnRecDaysList(), rsp.recordStateInDaysList, req.timeRange);
        rsp.errorCode = cCuRcsQueryRecordDaysRsp.GetErrorCode();
    }   
    else if(cusdk__ERecordTypePURecord == req.recordType)
    {
        CCuPuRecDaysQryReq cCuPuRecDaysQryReq;
        cCuPuRecDaysQryReq.SetSession(req.session);
        cusdk__DevChannel& cDevChn = *req.devChnList.begin();

        cCuPuRecDaysQryReq.GetDevChn().SetDevUri(cDevChn.deviceId);
        cCuPuRecDaysQryReq.GetDevChn().SetChnNO(cDevChn.channelId);

        CCMSTime startTime(req.timeRange.startTime);
        cCuPuRecDaysQryReq.SetTimeCond(startTime.ToString());

        CCuPuRecDaysQryRsp cCuPuRecDaysQryRsp;
        CuGetPuRecordDays(cCuPuRecDaysQryReq, cCuPuRecDaysQryRsp);

        ConverTempPuRecordDayFromCuiToCu(cCuPuRecDaysQryRsp.GetResultBitMask(), rsp.recordStateInDaysList, req.timeRange, cDevChn);
        //ConverTempRecordDayFromCuiToCu(cCuPuRecDaysQryRsp.GetResultBitMask(), rsp.recordStateInDaysList);
        rsp.errorCode = cCuPuRecDaysQryRsp.GetErrorCode();
    }

    rsp.session = req.session;
    

    return SOAP_OK;
}

int cusdkService::StartExportUserLog(cusdk__StartExportUserLogReq req,cusdk__StartExportUserLogRsp &rsp)
{
    CCuFileExportReq cCuFileExportReq; 
    cCuFileExportReq.SetSession(req.session);
    cCuFileExportReq.SetSrvName(FILE_EXPORT_CUSEROPERATELOG);
    cCuFileExportReq.SetFileSenderUri(DEV_TYPE_UAS);
    cCuFileExportReq.SetFileReceiverUri(DEV_TYPE_CU);
    cCuFileExportReq.SetFileCharacterset(req.characterSet);
    cCuFileExportReq.SetFileTimeZone(req.timeZone);

    COperateLogInfoQueryCondition condition;
    if(req.condition.userIdEnabled)
    {
        condition.SetUserUri(req.condition.userId);
    }

    if(req.condition.userLoginSessionEnabled)
    {
        condition.SetLogTag(req.condition.userLoginSession);
    }

    if(req.condition.domainNameEnabled)
    {
        condition.SetDomainName(req.condition.domainName);
    }

    if(req.condition.timeEnabled)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());
        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }

    if(req.condition.operateTypeEnabled)
    {
        condition.SetOperateType(req.condition.operateType);
    }

    cCuFileExportReq.SetFileToken(condition.ToXml());

    CCuFileExportRsp cCuFileExportRsp;
    CuStartExport(cCuFileExportReq, cCuFileExportRsp);

    rsp.session = req.session;
    rsp.errorCode = cCuFileExportRsp.GetErrorCode();
    rsp.exportFileUrl = cCuFileExportRsp.GetFileURL();

    return SOAP_OK;
}

int cusdkService::GetExportUserLogProcess(cusdk__GetExportUserLogProcessReq req, cusdk__GetExportUserLogProcessRsp &rsp)
{
    CCuGetExportProcessReq cCuGetExportUserLogProcessReq;
    cCuGetExportUserLogProcessReq.SetSession(req.session);
    cCuGetExportUserLogProcessReq.SetExpoetFileUrl(req.exportFileUrl);
    CCuGetExportProcessRsp cCuGetExportUserLogProcessRsp;
    CuGetExportProcess(cCuGetExportUserLogProcessReq, cCuGetExportUserLogProcessRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuGetExportUserLogProcessRsp.GetErrorCode();
    rsp.percentage = cCuGetExportUserLogProcessRsp.GetPercent();
    return SOAP_OK;
}

int cusdkService::StopExportUserLog(cusdk__StopExportUserLogReq req, cusdk__StopExportUserLogRsp &rsp)
{
    CCuStopExportReq cCuStopExportUserLogReq;
    cCuStopExportUserLogReq.SetSession(req.session);
    cCuStopExportUserLogReq.SetExportFileUrl(req.exportFileUrl);
    CCuStopExportRsp cCuStopExportUserLogRsp;
    CuStopExport(cCuStopExportUserLogReq, cCuStopExportUserLogRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuStopExportUserLogRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::StartExportDeviceLog( cusdk__StartExportDeviceLogReq req, cusdk__StartExportDeviceLogRsp &rsp)
{
    CCuFileExportReq cCuFileExportReq; 
    cCuFileExportReq.SetSession(req.session);
    cCuFileExportReq.SetSrvName(FILE_EXPORT_DEVICELOG);
    cCuFileExportReq.SetFileSenderUri(DEV_TYPE_UAS);
    cCuFileExportReq.SetFileReceiverUri(DEV_TYPE_CU);
    cCuFileExportReq.SetFileCharacterset(req.characterSet);
    cCuFileExportReq.SetFileTimeZone(req.timeZone);

    CDeviceLogInfoQueryCondition condition;
    if(req.condition.deviceIdEnabled)
    {
        condition.SetDevUri(req.condition.deviceId);
    }
    if(req.condition.deviceNameEnabled)
    {
        condition.SetDevName(req.condition.deviceName);
    }
    if(req.condition.timeEnabled)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());
        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }
    if(req.condition.logTypeEnabled)
    {
        condition.SetLogType(req.condition.logType);
    }
    cCuFileExportReq.SetFileToken(condition.ToXml());
    CCuFileExportRsp cCuFileExportRsp;
    CuStartExport(cCuFileExportReq, cCuFileExportRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuFileExportRsp.GetErrorCode();
    rsp.exportFileUrl = cCuFileExportRsp.GetFileURL();
    return SOAP_OK;
}

int cusdkService::GetExportDeviceLogProcess(cusdk__GetExportDeviceLogProcessReq req, cusdk__GetExportDeviceLogProcessRsp &rsp)
{
    CCuGetExportProcessReq cCuGetExportDeviceLogProcessReq;
    cCuGetExportDeviceLogProcessReq.SetSession(req.session);
    cCuGetExportDeviceLogProcessReq.SetExpoetFileUrl(req.exportFileUrl);
    CCuGetExportProcessRsp cCuGetExportDeviceLogProcessRsp;
    CuGetExportProcess(cCuGetExportDeviceLogProcessReq, cCuGetExportDeviceLogProcessRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuGetExportDeviceLogProcessRsp.GetErrorCode();
    rsp.percentage = cCuGetExportDeviceLogProcessRsp.GetPercent();
    return SOAP_OK;
}

int cusdkService::StopExportDeviceLog( cusdk__StopExportDeviceLogReq req, cusdk__StopExportDeviceLogRsp &rsp)
{
    CCuStopExportReq cCuStopExportDeviceLogReq;
    cCuStopExportDeviceLogReq.SetSession(req.session);
    cCuStopExportDeviceLogReq.SetExportFileUrl(req.exportFileUrl);
    CCuStopExportRsp cCuStopExportDeviceLogRsp;
    CuStopExport(cCuStopExportDeviceLogReq, cCuStopExportDeviceLogRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuStopExportDeviceLogRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::StartExportAlarmLog( cusdk__StartExportAlarmLogReq req, cusdk__StartExportAlarmLogRsp &rsp)
{
    CCuFileExportReq cCuFileExportReq; 
    cCuFileExportReq.SetSession(req.session);
    cCuFileExportReq.SetSrvName(FILE_EXPORT_ALARMLOG);
    cCuFileExportReq.SetFileSenderUri(DEV_TYPE_UAS);
    cCuFileExportReq.SetFileReceiverUri(DEV_TYPE_CU);
    cCuFileExportReq.SetFileCharacterset(req.characterSet);
    cCuFileExportReq.SetFileTimeZone(req.timeZone);

    CAlarmLogInfoQueryCondition condition;
    if(req.condition.deviceIdEnabled)
    {
        condition.SetDevUri(req.condition.deviceId);
    }
    if(req.condition.alarmChannelIdEnabled)
    {
        condition.SetChannelId(req.condition.alarmChannelId);
    }
    if(req.condition.alarmNameEnabled)
    {
        condition.SetAlias(req.condition.alarmName);
    }
    if(req.condition.deviceDomainNameEnabled)
    {
        condition.SetDevDomainName(req.condition.deviceDomainName);
    }
    if(req.condition.timeEnabled)
    {
        CCMSTime begin(req.condition.timeBegin);
        condition.SetHappenedTimeBegin(begin.ToString());
        CCMSTime end(req.condition.timeEnd);
        condition.SetHappenedTimeEnd(end.ToString());
    }
    if(req.condition.alarmTypeEnabled)
    {
        CMS_ALARM_TYPE cmsType = "";//ALARM_TYPE_PIN
        const CU_ALARM_TYPE &cuType = req.condition.alarmType;
        const CAlarmTypeCommon *pcAlmUtils = GetCommonType();
        if( pcAlmUtils != NULL )
        {
            cmsType = pcAlmUtils->ConvertFromCuToCms( cuType ); //cusdk__AlarmInput
            condition.SetType( cmsType );
        }   
    }
    cCuFileExportReq.SetFileToken(condition.ToXml());
    CCuFileExportRsp cCuFileExportRsp;
    CuStartExport(cCuFileExportReq, cCuFileExportRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuFileExportRsp.GetErrorCode();
    rsp.exportFileUrl = cCuFileExportRsp.GetFileURL();
    return SOAP_OK;
}

int cusdkService::GetExportAlarmLogProcess( cusdk__GetExportAlarmLogProcessReq req, cusdk__GetExportAlarmLogProcessRsp &rsp)
{
    CCuGetExportProcessReq cCuGetExportAlarmLogProcessReq;
    cCuGetExportAlarmLogProcessReq.SetSession(req.session);
    cCuGetExportAlarmLogProcessReq.SetExpoetFileUrl(req.exportFileUrl);
    CCuGetExportProcessRsp cCuGetExportAlarmLogProcessRsp;
    CuGetExportProcess(cCuGetExportAlarmLogProcessReq, cCuGetExportAlarmLogProcessRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuGetExportAlarmLogProcessRsp.GetErrorCode();
    rsp.percentage = cCuGetExportAlarmLogProcessRsp.GetPercent();
    return SOAP_OK;
}

int cusdkService::StopExportAlarmLog( cusdk__StopExportAlarmLogReq req, cusdk__StopExportAlarmLogRsp &rsp)
{
    CCuStopExportReq cCuStopExportAlarmLogReq;
    cCuStopExportAlarmLogReq.SetSession(req.session);
    cCuStopExportAlarmLogReq.SetExportFileUrl(req.exportFileUrl);
    CCuStopExportRsp cCuStopExportAlarmLogRsp;
    CuStopExport(cCuStopExportAlarmLogReq, cCuStopExportAlarmLogRsp);
    rsp.session = req.session;
    rsp.errorCode = cCuStopExportAlarmLogRsp.GetErrorCode();
    return SOAP_OK;
}

int cusdkService::RestartDevice(cusdk__RestartDeviceReq req, cusdk__RestartDeviceRsp &rsp)
{
	CCuGbsGbDevTeleBootReq cCuGbsGbDevTeleBootReq;
	
	cCuGbsGbDevTeleBootReq.SetSession(req.session);
	cCuGbsGbDevTeleBootReq.SetKdDevUri(req.deviceId);

	CCuGbsGbDevTeleBootRsp cCuGbsGbDevTeleBootRsp;
	
	CuGbsGbDevTeleBoot(cCuGbsGbDevTeleBootReq, cCuGbsGbDevTeleBootRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuGbsGbDevTeleBootRsp.GetErrorCode();

	return SOAP_OK;
}

// 划归一个设备组给用户
int cusdkService::UserAddDeviceGroup(cusdk__UserAddDeviceGroupReq req, cusdk__UserAddDeviceGroupRsp &rsp)
{
	CCuUserDeviceGroupAddReq cCuUserDeviceGroupAddReq;

	cCuUserDeviceGroupAddReq.SetSession(req.session);
	cCuUserDeviceGroupAddReq.SetUserId(req.userId);
	cCuUserDeviceGroupAddReq.SetDevGrpId(req.devgrpid);

	cCuUserDeviceGroupAddReq.SetAssignPriv(req.usePri);

	if ( req.usePri )
	{
		CDevicePrivilege cDevicePrivilege;
		ConvertDevicePrivilegeFromCuToCui(req.devPriSet, cDevicePrivilege);
		cCuUserDeviceGroupAddReq.SetDevicePrivilege(cDevicePrivilege.GetEncoderPrivilege());

		CVideosourcePrivilege cVideosourcePrivilege;
		ConvertVideosourcePrivilegeFromCuToCui(req.videoSrcPriSet, cVideosourcePrivilege);
		cVideosourcePrivilege.SetPTZCtrl(req.ptzLevel);
		cCuUserDeviceGroupAddReq.SetDeviceCapPrivilege(cVideosourcePrivilege);
	}

	CCuUserDeviceGroupAddRsp cCuUserDeviceGroupAddRsp;

	CuUserDeviceGroupAdd(cCuUserDeviceGroupAddReq, cCuUserDeviceGroupAddRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuUserDeviceGroupAddRsp.GetErrorCode();

	rsp.operationId = cCuUserDeviceGroupAddRsp.GetOperUuid();

	return SOAP_OK;
}

// 取消划归一个用户的设备组
int cusdkService::UserDelDeviceGroup(cusdk__UserDelDeviceGroupReq req, cusdk__UserDelDeviceGroupRsp &rsp)
{
	CCuUserDeviceGroupDelReq cCuUserDeviceGroupDelReq;

	cCuUserDeviceGroupDelReq.SetSession(req.session);
	cCuUserDeviceGroupDelReq.SetUserId(req.userId);
	cCuUserDeviceGroupDelReq.SetDevGrpId(req.devgrpid);

	CCuUserDeviceGroupDelRsp cCuUserDeviceGroupDelRsp;

	CuUserDeviceGroupDel(cCuUserDeviceGroupDelReq, cCuUserDeviceGroupDelRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuUserDeviceGroupDelRsp.GetErrorCode();
	rsp.operationId = cCuUserDeviceGroupDelRsp.GetOperUuid();

	return SOAP_OK;
}

// 划归一个设备组给用户组
int cusdkService::UserGroupAddDeviceGroup(cusdk__UserGroupAddDeviceGroupReq req, cusdk__UserGroupAddDeviceGroupRsp &rsp)
{
	CCuUserGroupDeviceGroupAddReq cCuUserGroupDeviceGroupAddReq;

	cCuUserGroupDeviceGroupAddReq.SetSession(req.session);
	cCuUserGroupDeviceGroupAddReq.SetUserGroupId(req.usergrpId);
	cCuUserGroupDeviceGroupAddReq.SetDevGrpId(req.devgrpid);

	cCuUserGroupDeviceGroupAddReq.SetAssignPriv(req.usePri);

	if (req.usePri)
	{
		CDevicePrivilege cDevicePrivilege;
		ConvertDevicePrivilegeFromCuToCui(req.devPriSet, cDevicePrivilege);
		cCuUserGroupDeviceGroupAddReq.SetDevicePrivilege(cDevicePrivilege.GetEncoderPrivilege());

		CVideosourcePrivilege cVideosourcePrivilege;
		ConvertVideosourcePrivilegeFromCuToCui(req.videoSrcPriSet, cVideosourcePrivilege);
		cVideosourcePrivilege.SetPTZCtrl(req.ptzLevel);
		cCuUserGroupDeviceGroupAddReq.SetDeviceCapPrivilege(cVideosourcePrivilege);
	}

	CCuUserGroupDeviceGroupAddRsp cCuUserGroupDeviceGroupAddRsp;
	
	CuUserGroupAddDeviceGroup(cCuUserGroupDeviceGroupAddReq, cCuUserGroupDeviceGroupAddRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuUserGroupDeviceGroupAddRsp.GetErrorCode();
	rsp.operationId = cCuUserGroupDeviceGroupAddRsp.GetOperUuid();

	return SOAP_OK;
}

// 取消划归一个用户组的设备组
int cusdkService::UserGroupDelDeviceGroup(cusdk__UserGroupDelDeviceGroupReq req, cusdk__UserGroupDelDeviceGroupRsp &rsp)
{
	CCuUserGroupDeviceGroupDelReq cCuUserGroupDeviceGroupDelReq;

	cCuUserGroupDeviceGroupDelReq.SetSession(req.session);
	cCuUserGroupDeviceGroupDelReq.SetUserGroupId(req.usergrpId);
	cCuUserGroupDeviceGroupDelReq.SetDevGrpId(req.devgrpid);

	CCuUserGroupDeviceGroupDelRsp cCuUserGroupDeviceGroupDelRsp;

	CuUserGroupDelDeviceGroup(cCuUserGroupDeviceGroupDelReq, cCuUserGroupDeviceGroupDelRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuUserGroupDeviceGroupDelRsp.GetErrorCode();
	rsp.operationId = cCuUserGroupDeviceGroupDelRsp.GetOperUuid();

	return SOAP_OK;
}

// 自动同步/取消同步一个用户的设备组
int cusdkService::UserSetAutoSyncDeviceGroup(cusdk__UserSetAutoSyncDeviceGroupReq req, cusdk__UserSetAutoSyncDeviceGroupRsp &rsp)
{
	CCuDeviceAutoAssignToUserSetReq cCuDeviceAutoAssignToUserSetReq;

	cCuDeviceAutoAssignToUserSetReq.SetSession(req.session);
	cCuDeviceAutoAssignToUserSetReq.SetUserId(req.userId);
	cCuDeviceAutoAssignToUserSetReq.SetDevGrpId(req.devgrpid);
	cCuDeviceAutoAssignToUserSetReq.SetAutoAssign(req.bSync);

	CCuDeviceAutoAssignToUserSetRsp cCuDeviceAutoAssignToUserSetRsp;

	CuUserSetAutoSyncDeviceGroup(cCuDeviceAutoAssignToUserSetReq, cCuDeviceAutoAssignToUserSetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDeviceAutoAssignToUserSetRsp.GetErrorCode();
	rsp.operationId = cCuDeviceAutoAssignToUserSetRsp.GetOperUuid();

	return SOAP_OK;
}

// 自动同步/取消同步一个用户组的设备组
int cusdkService::UserGroupSetAutoSyncDeviceGroup(cusdk__UserGroupSetAutoSyncDeviceGroupReq req, cusdk__UserGroupSetAutoSyncDeviceGroupRsp &rsp)
{
	CCuDeviceAutoAssignToUserGroupSetReq cCuDeviceAutoAssignToUserGroupSetReq;

	cCuDeviceAutoAssignToUserGroupSetReq.SetSession(req.session);
	cCuDeviceAutoAssignToUserGroupSetReq.SetUserGroupId(req.usergrpId);
	cCuDeviceAutoAssignToUserGroupSetReq.SetDevGrpId(req.devgrpid);
	cCuDeviceAutoAssignToUserGroupSetReq.SetAutoAssign(req.bSync);

	CCuDeviceAutoAssignToUserGroupSetRsp cCuDeviceAutoAssignToUserGroupSetRsp;

	CuUserGroupSetAutoSyncDeviceGroup(cCuDeviceAutoAssignToUserGroupSetReq, cCuDeviceAutoAssignToUserGroupSetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDeviceAutoAssignToUserGroupSetRsp.GetErrorCode();
	rsp.operationId = cCuDeviceAutoAssignToUserGroupSetRsp.GetOperUuid();

	return SOAP_OK;
}

// 分页获取用户所有已同步设备组列表
int cusdkService::UserQueryAutoSyncDeviceGroup(cusdk__UserQueryAutoSyncDeviceGroupReq req, cusdk__UserQueryAutoSyncDeviceGroupRsp &rsp)
{
	CCuDeviceAutoAssignToUserQryReq cCuDeviceAutoAssignToUserQryReq;

	cCuDeviceAutoAssignToUserQryReq.SetSession(req.session);
	cCuDeviceAutoAssignToUserQryReq.SetUserId(req.userId);
	cCuDeviceAutoAssignToUserQryReq.SetStartEntryIndex(req.beginIndex);
	cCuDeviceAutoAssignToUserQryReq.SetExpectEntryNum(req.num);
	
	CCuDeviceAutoAssignToUserQryRsp cCuDeviceAutoAssignToUserQryRsp;

	CuUserQueryAutoSyncDeviceGroup(cCuDeviceAutoAssignToUserQryReq, cCuDeviceAutoAssignToUserQryRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDeviceAutoAssignToUserQryRsp.GetErrorCode();
	rsp.totalnum = cCuDeviceAutoAssignToUserQryRsp.GetTotalEntryNum();

	if ( cCuDeviceAutoAssignToUserQryRsp.GetActualResultNum() > 0)
	{
		rsp.totalnum = cCuDeviceAutoAssignToUserQryRsp.GetTotalEntryNum();

		vector<CDeviceAutoAssignInfo>::const_iterator pDeviceGroupIdItem = cCuDeviceAutoAssignToUserQryRsp.GetDeviceAutoAssignInfo().begin();
		
		while ( pDeviceGroupIdItem != cCuDeviceAutoAssignToUserQryRsp.GetDeviceAutoAssignInfo().end())
		{
			rsp.devgroupIds.push_back(pDeviceGroupIdItem->GetDevGrpId()); 
			pDeviceGroupIdItem++;
		}
	}
	
	return SOAP_OK;
}

// 分页获取用户组所有已同步设备组列表
int cusdkService::UserGroupQueryAutoSyncDeviceGroup(cusdk__UserGroupQueryAutoSyncDeviceGroupReq req, cusdk__UserGroupQueryAutoSyncDeviceGroupRsp &rsp)
{
	CCuDeviceAutoAssignToUserGroupQryReq cCuDeviceAutoAssignToUserGroupQryReq;

	cCuDeviceAutoAssignToUserGroupQryReq.SetSession(req.session);
	cCuDeviceAutoAssignToUserGroupQryReq.SetUserGroupId(req.usergrpId);
	cCuDeviceAutoAssignToUserGroupQryReq.SetStartEntryIndex(req.beginIndex);
	cCuDeviceAutoAssignToUserGroupQryReq.SetExpectEntryNum(req.num);

	CCuDeviceAutoAssignToUserGroupQryRsp cCuDeviceAutoAssignToUserGroupQryRsp;

	CuUserGroupQueryAutoSyncDeviceGroup(cCuDeviceAutoAssignToUserGroupQryReq, cCuDeviceAutoAssignToUserGroupQryRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDeviceAutoAssignToUserGroupQryRsp.GetErrorCode();
	rsp.totalnum = cCuDeviceAutoAssignToUserGroupQryRsp.GetTotalEntryNum();

	if ( cCuDeviceAutoAssignToUserGroupQryRsp.GetActualResultNum() > 0)
	{
		rsp.totalnum = cCuDeviceAutoAssignToUserGroupQryRsp.GetTotalEntryNum();

		vector<CDeviceAutoAssignInfo>::const_iterator pDeviceGroupIdItem = cCuDeviceAutoAssignToUserGroupQryRsp.GetDeviceAutoAssignInfo().begin();

		while ( pDeviceGroupIdItem != cCuDeviceAutoAssignToUserGroupQryRsp.GetDeviceAutoAssignInfo().end())
		{
			rsp.devgroupIds.push_back(pDeviceGroupIdItem->GetDevGrpId());
			pDeviceGroupIdItem++;
		}
	}

	return SOAP_OK;
}

// 查询指定操作Id的当前执行进度
int cusdkService::QueryOperationProgress(cusdk__QueryOperationProgreeReq req, cusdk__QueryOperationProgreeRsp &rsp)
{
	CCuTransactionProgressGetReq cCuTransactionProgressGetReq;

	cCuTransactionProgressGetReq.SetSession(req.session);
	cCuTransactionProgressGetReq.SetOperUuid(req.operationId);

	CCuTransactionProgressGetRsp cCuCTransactionProgressGetRsp;

	CuQueryOperationProgree(cCuTransactionProgressGetReq, cCuCTransactionProgressGetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuCTransactionProgressGetRsp.GetErrorCode();
	rsp.progress = cCuCTransactionProgressGetRsp.GetProgress().finishNum;
	rsp.total = cCuCTransactionProgressGetRsp.GetProgress().totalNum;

	return SOAP_OK;
}

int cusdkService::GetSuperGBDomainList(cusdk__GetGBDomainListReq req, cusdk__GetGBDomainListRsp &rsp)
{
	CCuGbsGbParentDomainGetReq cCuGbsGbParentDomainGetReq;

	cCuGbsGbParentDomainGetReq.SetSession(req.session);

	CCuGbsGbParentDomainGetRsp cCuGbsGbParentDomainGetRsp;

	CuGetSuperGBDomainList(cCuGbsGbParentDomainGetReq, cCuGbsGbParentDomainGetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuGbsGbParentDomainGetRsp.GetErrorCode();

	vector< TGbsGbParentDomainInfo >& TDomainInfoList = cCuGbsGbParentDomainGetRsp.GetGbParentDomainList();

	if ( TDomainInfoList.size() > 0 )
	{
		for (size_t nFori = 0; nFori < TDomainInfoList.size(); nFori++ )
		{
			cusdk__GBDomainInfo sdkDomainInfo;
			sdkDomainInfo.gbDomainId = TDomainInfoList[nFori].GetGbid();
			sdkDomainInfo.domainName = TDomainInfoList[nFori].GetName();
			sdkDomainInfo.devTreeVersion = TDomainInfoList[nFori].GetDevTreeVer();
			sdkDomainInfo.devTreeViewId = TDomainInfoList[nFori].GetDevTreeId();
			rsp.gbDomainList.push_back(sdkDomainInfo);
		}
	}
		
	return SOAP_OK;
}

int cusdkService::PushDevice(cusdk__DevicePushReq req, cusdk__DevicePushRsp &rsp)
{
	CCuGbsGbPDCatalogLoadReq cCuGbsGbPDCatalogLoadReq;

	cCuGbsGbPDCatalogLoadReq.SetSession(req.session);
	cCuGbsGbPDCatalogLoadReq.SetUser(req.userName);
	cCuGbsGbPDCatalogLoadReq.SetGbId(req.gbDomainId);

	// 增加设置cusession，当gbs notify通知给cui时才能找到对应的cu
	cCuGbsGbPDCatalogLoadReq.SetCuSession(req.session);

	CCuGbsGbPDCatalogLoadRsp cCuGbsGbPDCatalogLoadRsp;

	CuPushDevice(cCuGbsGbPDCatalogLoadReq, cCuGbsGbPDCatalogLoadRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuGbsGbPDCatalogLoadRsp.GetErrorCode();

	return SOAP_OK;
}


int cusdkService::GetDataVersion(cusdk__GetDataVersionReq req, cusdk__GetDataVersionRsp& rsp)
{
	if ( req.dataType == cusdk__DataTypeDevice ) // 获取设备数据 详见cusdk__EDataType
	{
		CCuUserDataVersionGetReq cCuUserDataVersionGetReq;

		cCuUserDataVersionGetReq.SetSession(req.session);

		CCuUserDataVersionGetRsp cCuUserDataVersionGetRsp;

		CuGetUserDataVersion(cCuUserDataVersionGetReq, cCuUserDataVersionGetRsp);

		rsp.dataVersion = cCuUserDataVersionGetRsp.GetDataVersion();
		rsp.errorCode = cCuUserDataVersionGetRsp.GetErrorCode();
	}
	else
	{
		rsp.errorCode = ERR_CUI_PARAMS_INVALID; // "非法的参数"
	}
	
	rsp.session = req.session;

	return SOAP_OK;
}

int cusdkService::GetCivilcode(cusdk__GetCivilcodeReq req, cusdk__GetCivilcodeRsp &rsp)
{
	CCuGbsDevGrpCivilCodeGetReq cCuGbsDevGrpCivilCodeGetReq;

	cCuGbsDevGrpCivilCodeGetReq.SetSession(req.session);
	cCuGbsDevGrpCivilCodeGetReq.SetGbDevGrpId(req.deviceGroupId);

	CCuGbsDevGrpCivilCodeGetRsp cCuGbsDevGrpCivilCodeGetRsp;

	CuGetCivilcode(cCuGbsDevGrpCivilCodeGetReq, cCuGbsDevGrpCivilCodeGetRsp);

	rsp.civilcode = cCuGbsDevGrpCivilCodeGetRsp.GetCivilcode();
	rsp.session = req.session;
	rsp.errorCode = cCuGbsDevGrpCivilCodeGetRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetDeviceChnByGBID(cusdk__GetDeviceChnByGBIDReq req, cusdk__GetDeviceChnByGBIDRsp &rsp)
{
	CCuGbsDevKdidGetReq cCuGbsDevKdidGetReq;

	cCuGbsDevKdidGetReq.SetSession(req.session);
	cCuGbsDevKdidGetReq.SetDevGbid(req.deviceGBID);

	CCuGbsDevKdidGetRsp cCuGbsDevKdidGetRsp;

	CuGetGbsDevKdid(cCuGbsDevKdidGetReq, cCuGbsDevKdidGetRsp);

	rsp.deviceID = cCuGbsDevKdidGetRsp.GetKdDevUri();
	rsp.chnId = cCuGbsDevKdidGetRsp.GetEncodeChnIndex();
	rsp.session = req.session;
	rsp.errorCode = cCuGbsDevKdidGetRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::StartDataFileExport(cusdk__StartDataFileExportReq req, cusdk__StartDataFileExportRsp &rsp)
{
	CCuFileExportStartExReq cCuFileExportStartReqEx;
	cCuFileExportStartReqEx.SetSession(req.session);

	cusdk__EDataType dataType = (cusdk__EDataType)req.exportedDataType;

	rsp.errorCode = CMS_SUCCESS;
	rsp.session = req.session;

	switch (dataType)
	{
	case cusdk__DataTypeGBSCatalog:
		{
			cCuFileExportStartReqEx.SetSrvName(FILE_EXPORT_GB_CATALOG);
			cCuFileExportStartReqEx.SetFileSenderUri(DEV_TYPE_GBS);
			cCuFileExportStartReqEx.SetFileReceiverUri(DEV_TYPE_CU);
		}
		break;
	case cusdk__DataTypeGBSDevicePush:
		{
			cCuFileExportStartReqEx.SetSrvName(FILE_EXPORT_GB_CATALOG_REPORT_CFG);
			cCuFileExportStartReqEx.SetFileSenderUri(DEV_TYPE_GBS);
			cCuFileExportStartReqEx.SetFileReceiverUri(DEV_TYPE_CU);
			if ( req.params.empty() )
			{
				rsp.errorCode = ERR_CUI_INVALID_PARAM;
			}
			else
			{
				cCuFileExportStartReqEx.SetFileToken(req.params.front());
			}
		}
		break;
	case cusdk__DataDeviceStatistics:
		{
			cCuFileExportStartReqEx.SetSrvName(FILE_EXPORT_DEVICE_SATATISTICS);
			cCuFileExportStartReqEx.SetFileSenderUri(DEV_TYPE_DPS);
			cCuFileExportStartReqEx.SetFileReceiverUri(DEV_TYPE_CU);
			if ( req.params.empty() )
			{
				rsp.errorCode = ERR_CUI_INVALID_PARAM;
			}
			else
			{
				cCuFileExportStartReqEx.GetCustomParams() = req.params;
			}
		}
		break;
	default:
		{
			rsp.errorCode = ERR_CUI_INVALID_PARAM;
		}
		break;
	}
	
	if ( rsp.errorCode != CMS_SUCCESS )
	{
		return SOAP_OK;
	}

	CCuFileExportStartExRsp cCuFileExportStartExRsp;
	CuStartExportEx(cCuFileExportStartReqEx, cCuFileExportStartExRsp);

	ostringstream osOptrionId;
	osOptrionId << cCuFileExportStartExRsp.GetTaskNo() << "_" << cCuFileExportStartExRsp.GetTaskType();
	rsp.operationId = osOptrionId.str();
	
	rsp.fileUrl = cCuFileExportStartExRsp.GetFileURL();
	rsp.errorCode = cCuFileExportStartExRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetOperationProgress(cusdk__GetOperationProgreeReq req, cusdk__GetOperationProgreeRsp &rsp)
{
	CCuFileExportGetProcessExReq cCuFileExportGetProcessExReq;
	cCuFileExportGetProcessExReq.SetSession(req.session);

	rsp.session = req.session;

	int nPos = req.operationId.find_first_of("_");
	if (nPos == string::npos)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	string strTaskNo = req.operationId.substr(0, nPos);
	string strTaskType = req.operationId.substr(nPos+1);

	u32 taskNo = atol(strTaskNo.c_str());
	if ( taskNo == INVALID_TASKNO || strTaskType.empty() )
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	cCuFileExportGetProcessExReq.SetTaskNo(taskNo);
	cCuFileExportGetProcessExReq.SetTaskType(strTaskType);

	CCuFileExportGetProcessExRsp cCuFileExportGetProcessExRsp;

	CuGetExportProcessEx(cCuFileExportGetProcessExReq, cCuFileExportGetProcessExRsp);

	rsp.errorCode = cCuFileExportGetProcessExRsp.GetErrorCode();
	rsp.progress = cCuFileExportGetProcessExRsp.GetPercent();
	rsp.total = 100;

	return SOAP_OK;
}

int cusdkService::EndDataFileExport(cusdk__EndDataFileExportReq req, cusdk__EndDataFileExportRsp &rsp)
{
	CCuFileExportStopExReq cCuFileExportStopExReq;
	cCuFileExportStopExReq.SetSession(req.session);
	
	rsp.session = req.session;

	int nPos = req.operationId.find_first_of("_");
	if (nPos == string::npos)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	string strTaskNo = req.operationId.substr(0, nPos);
	string strTaskType = req.operationId.substr(nPos + 1);

	u32 taskNo = atol(strTaskNo.c_str());
	if (taskNo == INVALID_TASKNO /*|| strTaskType.empty()*/)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	cCuFileExportStopExReq.SetTaskNo(taskNo);

	CCuFileExportStopExRsp cCuFileExportStopExRsp;

	CuStopExportEx(cCuFileExportStopExReq, cCuFileExportStopExRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuFileExportStopExRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::StartDataFileImport(cusdk__StartDataFileImportReq req, cusdk__StartDataFileImportRsp &rsp)
{
	CCuFileImportStartExReq cCuFileImportStartExReq;
	cCuFileImportStartExReq.SetSession(req.session);

	cusdk__EDataType dataType = (cusdk__EDataType)req.importedDataType;

	rsp.errorCode = CMS_SUCCESS;
	rsp.session = req.session;

	switch (dataType)
	{
	case cusdk__DataTypeGBSDevicePush:
		{
			cCuFileImportStartExReq.SetSrvName(FILE_IMPORT_GB_CATALOG_REPORT_CFG);
			cCuFileImportStartExReq.SetFileSenderUri(DEV_TYPE_CU);
			cCuFileImportStartExReq.SetFileReceiverUri(DEV_TYPE_GBS);
			if ( req.params.empty() )
			{
				rsp.errorCode = ERR_CUI_INVALID_PARAM;
			}
			else
			{
				cCuFileImportStartExReq.SetFileToken(req.params.front());
			}
		}
		break;
	default:
		{
			rsp.errorCode = ERR_CUI_INVALID_PARAM;
		}
		break;
	}

	if ( rsp.errorCode != CMS_SUCCESS )
	{
		return SOAP_OK;
	}

	CCuFileImportStartExRsp cCuFileImportStartExRsp;
	CuStartImportEx(cCuFileImportStartExReq, cCuFileImportStartExRsp);

	ostringstream osTaskNo;
	osTaskNo << cCuFileImportStartExRsp.GetTaskNo();
	rsp.operationId = osTaskNo.str();

	rsp.fileUrl = cCuFileImportStartExRsp.GetDirURL();
	rsp.errorCode = cCuFileImportStartExRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::UpdateOperationProgress(cusdk__UpdateOperationProgreeReq req, cusdk__UpdateOperationProgreeRsp &rsp)
{
	rsp.session = req.session;

	if ( req.progress < 0 || req.total < 0 )
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	u32 taskNo = atol(req.operationId.c_str());

	CCuFileImportSetProcessExReq cCuFileImportSetProcessExReq;
	cCuFileImportSetProcessExReq.SetSession(req.session);
	cCuFileImportSetProcessExReq.SetProgress(req.progress);
	cCuFileImportSetProcessExReq.SetTotal(req.total);

	cCuFileImportSetProcessExReq.SetTaskNo(taskNo);

	CCuFileImportSetProcessExRsp cCuFileImportSetProcessExRsp;

	CuSetImportProcessEx(cCuFileImportSetProcessExReq, cCuFileImportSetProcessExRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuFileImportSetProcessExRsp.GetErrorCode();

	return SOAP_OK;
}


int cusdkService::EndDataFileImport(cusdk__EndDataFileImportReq req, cusdk__EndDataFileImportRsp &rsp)
{
	CCuFileImportStopExReq cCuFileImportStopExReq;
	cCuFileImportStopExReq.SetSession(req.session);

	rsp.session = req.session;

	u32 taskNo = atol(req.operationId.c_str());

	cCuFileImportStopExReq.SetTaskNo(taskNo);
	cCuFileImportStopExReq.SetCancelImport(req.cancelOperation);

	// 是否支持取消，这个CU还没有确定参数，CCuStopImportReq中预留该参数，默认是完成

	CCuFileImportStopExRsp cCuFileImportStopExRsp;

	CuStopImportEx(cCuFileImportStopExReq, cCuFileImportStopExRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuFileImportStopExRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::CreateCustomTreeView(cusdk__CreateCustomTreeViewReq req, cusdk__CreateCustomTreeViewRsp &rsp)
{
	CCuDevTreeAddReq cCuDevTreeAddReq;
	cCuDevTreeAddReq.SetSession(req.session);
	cCuDevTreeAddReq.SetDevtreeName(req.name);
	cCuDevTreeAddReq.SetCanBeUsed4Gb(req.canUsedForGBPush);
	cCuDevTreeAddReq.SetCopyFromOtherTree(req.cloneFromAnotherTreeView);
	cCuDevTreeAddReq.SetSrcTreeId(req.cloneSrcTreeViewId);
	
	CCuDevTreeAddRsp cCuDevTreeAddRsp;

	CuCreateCustomTreeView(cCuDevTreeAddReq, cCuDevTreeAddRsp);

	rsp.id = cCuDevTreeAddRsp.GetDevtreeId();
	rsp.operationId = cCuDevTreeAddRsp.GetOperUuid();
	rsp.session = req.session;
	rsp.errorCode = cCuDevTreeAddRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::ModifyCustomTreeViewName(cusdk__ModifyCustomTreeViewNameReq req, cusdk__ModifyCustomTreeViewNameRsp &rsp)
{
	CCuDevTreeModReq cCuDevTreeModReq;
	cCuDevTreeModReq.SetSession(req.session);
	cCuDevTreeModReq.SetDevtreeId(req.id);
	cCuDevTreeModReq.SetDevtreeName(req.name);
	
	CCuDevTreeModRsp cCuDevTreeModRsp;

	CuModifyCustomTreeViewName(cCuDevTreeModReq, cCuDevTreeModRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDevTreeModRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::DeleteCustomTreeView(cusdk__DeleteCustomTreeViewReq req, cusdk__DeleteCustomTreeViewRsp &rsp)
{
	CCuDevTreeDelReq cCuDevTreeDelReq;
	cCuDevTreeDelReq.SetSession(req.session);
	cCuDevTreeDelReq.SetDevtreeId(req.id);

	CCuDevTreeDelRsp cCuDevTreeDelRsp;

	CuDeleteCustomTreeView(cCuDevTreeDelReq, cCuDevTreeDelRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDevTreeDelRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetCustomTreeViewList(cusdk__GetCustomTreeViewListReq req, cusdk__GetCustomTreeViewListRsp &rsp)
{
	CCuDevTreeQryReq cCuDevTreeQryReq;
	cCuDevTreeQryReq.SetSession(req.session);
	cCuDevTreeQryReq.SetIncludeFavorites(req.includeFavorite);

	CCuDevTreeQryRsp cCuDevTreeQryRsp;

	CuGetCustomTreeViewList(cCuDevTreeQryReq, cCuDevTreeQryRsp);

	ConvertDevTreeInfoFromCUIToCu(cCuDevTreeQryRsp.GetDevTreeInfo(), rsp.treeViewInfoList);

	rsp.session = req.session;
	rsp.errorCode = cCuDevTreeQryRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetDeviceGroupPath(cusdk__GetDeviceGroupPathReq req, cusdk__GetDeviceGroupPathRsp &rsp)
{
	CCuDeviceGrpPathQryReq cDeviceGrpPathQryReq;
	cDeviceGrpPathQryReq.SetSession(req.session);
	cDeviceGrpPathQryReq.SetDevTreeId(req.devTreeId);
	cDeviceGrpPathQryReq.SetDeviceId(req.devId);
	cDeviceGrpPathQryReq.SetVideoSrcId(req.videoSrcId);

	CCuDeviceGrpPathQryRsp cDeviceGrpPathQryRsp;

	CuGetDeviceGroupPath(cDeviceGrpPathQryReq, cDeviceGrpPathQryRsp);

	ConvertDevGroupInfoFromCUIToCu(cDeviceGrpPathQryRsp.GetDeviceGroupInfo(), rsp.devGroupPath);

	rsp.session = req.session;
	rsp.errorCode = cDeviceGrpPathQryRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::SearchDevice(cusdk__SearchDeviceReq req, cusdk__SearchDeviceRsp &rsp)
{
	CCuDPSSearchReq cCuDPSSearchReq;
	cCuDPSSearchReq.SetSession(req.session);
	cCuDPSSearchReq.SetKeywords(req.keywords);
	cCuDPSSearchReq.SetMaxResultItemNum(req.maxResultItemNum);
	cCuDPSSearchReq.SetMaxSearchTimeMSec(req.maxSearchTimeMSec);
	
	CCuDPSSearchRsp cCuDPSSearchRsp;

	CuSearchDevice(cCuDPSSearchReq, cCuDPSSearchRsp);
	
	ostringstream osOptrionId;
	osOptrionId << cCuDPSSearchRsp.GetTaskNo() << "_" << cCuDPSSearchRsp.GetTaskType();
	rsp.operationId = osOptrionId.str();

	rsp.session = req.session;
	rsp.errorCode = cCuDPSSearchRsp.GetErrorCode();

	return SOAP_OK;
}


int cusdkService::GetSearchResult(cusdk__GetSearchResultReq req, cusdk__GetSearchResultRsp &rsp)
{
	CCuDPSGetResultReq cCuDPSGetResultReq;

	rsp.session = req.session;
	int nPos = req.operationId.find_first_of("_");
	if (nPos == string::npos)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	string strTaskNo = req.operationId.substr(0, nPos);
	string strTaskType = req.operationId.substr(nPos + 1);

	u32 taskNo = atol(strTaskNo.c_str());
	if (taskNo == INVALID_TASKNO /*|| strTaskType.empty()*/)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	cCuDPSGetResultReq.SetSession(req.session);
	cCuDPSGetResultReq.SetTaskNo(taskNo);
	cCuDPSGetResultReq.SetStartEntryIndex(req.beginIndex);
	cCuDPSGetResultReq.SetExpectEntryNum(req.num);

	CCuDPSGetResultRsp cCuDPSGetResultRsp;

	CuGetSearchResult(cCuDPSGetResultReq, cCuDPSGetResultRsp);

	rsp.lastPage = cCuDPSGetResultRsp.IsLastPage();
	rsp.totalNum = cCuDPSGetResultRsp.GetTotalEntryNum();
	
	ConverSearchDPSResultCuiToCu(cCuDPSGetResultRsp.GetSearchResultList(), rsp.result);

	rsp.session = req.session;
	rsp.errorCode = cCuDPSGetResultRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetSearchResultByGroup(cusdk__GetSearchResultByGroupReq req, cusdk__GetSearchResultByGroupRsp& rsp)
{
	CCuDPSGetResultByGroupReq cCuDPSGetResultByGroupReq;
	cCuDPSGetResultByGroupReq.SetSession(req.session);

	rsp.session = req.session;

	int nPos = req.operationId.find_first_of("_");
	if (nPos == string::npos)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	string strTaskNo = req.operationId.substr(0, nPos);
	string strTaskType = req.operationId.substr(nPos + 1);

	u32 taskNo = atol(strTaskNo.c_str());
	if (taskNo == INVALID_TASKNO /*|| strTaskType.empty()*/)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	cCuDPSGetResultByGroupReq.SetTaskNo(taskNo);
	cCuDPSGetResultByGroupReq.SetStartEntryIndex(req.beginIndex);
	cCuDPSGetResultByGroupReq.SetExpectEntryNum(req.num);

	cCuDPSGetResultByGroupReq.SetDevGroupId(req.deviceGroupId);
	cCuDPSGetResultByGroupReq.SetType(req.type);

	CCuDPSGetResultByGroupRsp cCuDPSGetResultByGroupRsp;

	CuGetSearchResultByGroup(cCuDPSGetResultByGroupReq, cCuDPSGetResultByGroupRsp);

	ConverSearchDPSResultCuiToCu(cCuDPSGetResultByGroupRsp.GetSearchResultList(), rsp.result);
	
	rsp.lastPage = cCuDPSGetResultByGroupRsp.IsLastPage();
	rsp.allVideoSrcNum = cCuDPSGetResultByGroupRsp.GetVidSrcNum();
	rsp.onlineVideoSrcNum = cCuDPSGetResultByGroupRsp.GetOnlineVidSrcNum();
	rsp.errorCode = cCuDPSGetResultByGroupRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::EndOperation(cusdk__EndOperationReq req, cusdk__EndOperationRsp &rsp)
{
	CCuDPSSearchStopReq cCuDPSSearchStopReq;
	cCuDPSSearchStopReq.SetSession(req.session);

	int nPos = req.operationId.find_first_of("_");
	if (nPos == string::npos)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}

	string strTaskNo = req.operationId.substr(0, nPos);
	string strTaskType = req.operationId.substr(nPos + 1);

	u32 taskNo = atol(strTaskNo.c_str());
	if (taskNo == INVALID_TASKNO /*|| strTaskType.empty()*/)
	{
		rsp.errorCode = ERR_CUI_INVALID_PARAM;
		return SOAP_OK;
	}
	
	cCuDPSSearchStopReq.SetTaskNo(taskNo);

	CCuDPSSearchStopRsp cCuDPSSearchStopRsp;

	CuEndOperation(cCuDPSSearchStopReq, cCuDPSSearchStopRsp);

	rsp.errorCode = cCuDPSSearchStopRsp.GetErrorCode();
	rsp.session = req.session;

	return SOAP_OK;
}


int cusdkService::GetDeviceGroup(cusdk__GetDeviceGroupReq req, cusdk__GetDeviceGroupRsp &rsp)
{
	CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
	cCuGetUserDevGroupReq.SetSession(req.session);
	cCuGetUserDevGroupReq.SetExpectEntryNum(1);
	cCuGetUserDevGroupReq.SetStartEntryIndex(0);
	CDeviceGroupInfoQueryCondition cCondition;
	//cCondition.SetCuserID(req.userId);
	//cCondition.SetDeviceParentGroupID(req.parentDevGroupId);
	//cCondition.SetDevTreeId(req.devTreeId);
	cCondition.SetDeviceGroupID(req.id);
	cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

	CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
	CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

	rsp.session   = req.session;
	rsp.errorCode = cCuGetUserDevGroupRsp.GetErrorCode();

	if ( rsp.errorCode ==  0)
	{
		if (cCuGetUserDevGroupRsp.GetActualResultNum() > 0)
		{
			vector<cusdk__DeviceGroup> devGroupList;
			ConvertUserDeviceGroupInfoFromCUIToCu(cCuGetUserDevGroupRsp.GetUserDevGroupInfo(), devGroupList);
			if ( !devGroupList.empty() )
			{
				rsp.group = devGroupList.front();
			}
		}
	}

	return SOAP_OK;
}


int cusdkService::GetDevice(cusdk__GetDeviceReq req, cusdk__GetDeviceRsp &rsp)
{
	CCuGetDevInfoAndStatusReq cCuGetDevInfoAndStatusReq;
	cCuGetDevInfoAndStatusReq.SetSession(req.session);
	cCuGetDevInfoAndStatusReq.GetDevIdList().push_back(req.id);

	CCuGetDevInfoAndStatusRsp cCuGetDevInfoAndStatusRsp;
	rsp.session = req.session;

	if (req.id.empty())
	{
		rsp.errorCode = ERR_CUI_PARAMS_INVALID;
		return SOAP_OK;
	}

	CuGetDevInfoAndStatus(cCuGetDevInfoAndStatusReq, cCuGetDevInfoAndStatusRsp);

	// 当前只单个设备查询，但CUI支持批量设备查询，如果采用批量，必须限制一下数量，否则容易超时;
	if (cCuGetDevInfoAndStatusRsp.GetErrorCode() != CMS_SUCCESS)
	{
		rsp.errorCode = cCuGetDevInfoAndStatusRsp.GetErrorCode();
		return SOAP_OK;
	}


    if( cCuGetDevInfoAndStatusRsp.GetDevInfoList().empty() )
    {
		rsp.errorCode = ERR_CUI_GET_REDIS_DATA_FAILED;
		return SOAP_OK;
    }

    // 目前只取一个设备CU没有批量
	TDeviceInfoAndStatus &devInfoAndStruct = cCuGetDevInfoAndStatusRsp.GetDevInfoList().front();

	if ( !devInfoAndStruct.isDevInfoValid )
	{
		rsp.errorCode = ERR_CUI_GET_REDIS_DATA_FAILED;
		return SOAP_OK;
	}

	std::vector<TDevicefullinfoAndPrivilege> tDevicefullinfoAndPrivilegeList;
	tDevicefullinfoAndPrivilegeList.push_back(devInfoAndStruct.devInfo);
	std::vector<cusdk__DeviceInfo> cuDeviceInfoList;
	ConvertDevicefullinfoFromCuiToCu(tDevicefullinfoAndPrivilegeList, cuDeviceInfoList);

	if (!cuDeviceInfoList.empty())
	{
		rsp.devInfo = cuDeviceInfoList.front();
	}

	rsp.online.deviceId = req.id;
	rsp.online.online = devInfoAndStruct.isOnline;

	ConvertDevConfigFromCuiToCu(devInfoAndStruct.devConfig, rsp.config);
    ConvertServiceStatusFromCuiToCu(req.id, devInfoAndStruct.serviceStatus, rsp.service);
    ConvertAlarmStatusFromCuiToCu(devInfoAndStruct.alarmStatus, rsp.alarm);

	return SOAP_OK;
}

int cusdkService::GetUserDeviceInGroup(cusdk__GetUserDeviceInGroupReq req, cusdk__GetUserDeviceInGroupRsp &rsp)
{
    TSipURI tUri;
    CCuUserDevGrpVidsrcQryReq cReq;
    cReq.SetSession(req.session);
    cReq.SetDevtreeId(req.treeId);
    cReq.SetCuserId(req.userId);
    if (tUri.SetNOString(req.userId))
    {
        cReq.SetCuserId(tUri.GetUser());
    }
    else
    {
        cReq.SetCuserId(req.userId);
    }
    if (tUri.SetNOString(req.groupId))
    {
        cReq.SetDevGrpId(tUri.GetUser());
    }
    else
    {
        cReq.SetDevGrpId(req.groupId);
    }
    cReq.SetQueryTag2(req.queryTag);
    cReq.SetRecursive(req.recursive);

    CCuUserDevGrpVidsrcQryRsp cRsp;

    CuGetUserDeviceInGroupRecursive(cReq, cRsp);

    rsp.queryTag = cRsp.GetQueryTag();
    rsp.lastPage = cRsp.GetLastNtf();
    cusdk__SimpleUserDevice userDev;
    
    for (vector<CUserDevGrpVidSrcInfo>::const_iterator it = cRsp.GetInfos().begin();
        it != cRsp.GetInfos().end(); ++it)
    {
        tUri.SetUser(it->GetDevId());
        tUri.SetDomain(it->GetDevDomainID()); // cuiserver完成了转换的
        userDev.devId = tUri.GetURIString();
        userDev.videoSrcIdList.clear();

        for( size_t i = 0; i!= it->GetVidSrcIndex().size(); ++i )
        {
            userDev.videoSrcIdList.push_back( it->GetVidSrcIndex().at(i) ); 
        }
        //for (map<int, CBitsContainer>::const_iterator it2 = it->GetDevCapPri().privileges.begin();
        //    it2 != it->GetDevCapPri().privileges.end(); ++it2)
        //{
        //    userDev.videoSrcIdList.push_back(it2->first);
        //}
        rsp.devList.push_back(userDev);
    }

    rsp.session = req.session;
    rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::BatchOperation(cusdk__BatchOperationReq req, cusdk__BatchOperationRsp &rsp)
{
	CCuBatOptReq cCuBatOptReq;
	cCuBatOptReq.SetSession(req.session);
	cCuBatOptReq.SetTreeId(req.treeId);
	cCuBatOptReq.SetGroupId(req.groupId);
	cCuBatOptReq.SetOptType(req.operationType);
	cCuBatOptReq.SetParamList(req.paramList);
	cCuBatOptReq.SetRecursive(req.recursive);
		
	CCuBatOptRsp cCuBatOptRsp;
	CuBatchOperation(cCuBatOptReq, cCuBatOptRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuBatOptRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetDeviceGroupVideoSrcNum(cusdk__GetDeviceGroupVideoSrcNumReq req, cusdk__GetDeviceGroupVideoSrcNumRsp &rsp)
{
	CCuDevGrpVidsrcTotalnumGetReq cCuDevGrpVidsrcTotalnumGetReq;
	cCuDevGrpVidsrcTotalnumGetReq.SetSession(req.session);
	cCuDevGrpVidsrcTotalnumGetReq.SetDevGrpIds(req.groupIdList);

	CCuDevGrpVidsrcTotalnumGetRsp cCuDevGrpVidsrcTotalnumGetRsp;

	CuGetDeviceGroupVideoSrcNum(cCuDevGrpVidsrcTotalnumGetReq, cCuDevGrpVidsrcTotalnumGetRsp);

	ConverDevGroupVsNums(cCuDevGrpVidsrcTotalnumGetReq, cCuDevGrpVidsrcTotalnumGetRsp.GetDevGrpVidsrcNum(), rsp.grpvsNums);

	rsp.session = req.session;
	rsp.errorCode = cCuDevGrpVidsrcTotalnumGetRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetVSOfflineInfo(cusdk__GetVSOfflineInfoReq req, cusdk__GetVSOfflineInfoRsp &rsp)
{
	CCuVidsrcInfoQryReq cCuVidsrcInfoQryReq;
	cCuVidsrcInfoQryReq.SetSession(req.session);

	std::map<string, string> mapUUIDtoName;
	ConverDevVsList(req.videosrclist, cCuVidsrcInfoQryReq.GetDevVidsrcs(), mapUUIDtoName);

	CCuVidsrcInfoQryRsp cCuVidsrcInfoQryRsp;
	
	CuGetVSOfflineInfo(cCuVidsrcInfoQryReq, cCuVidsrcInfoQryRsp);

	ConverDevVsOfflineInfo(mapUUIDtoName, cCuVidsrcInfoQryRsp.GetSearchResultList(), rsp.vsOfflineInfo);

	if (cCuVidsrcInfoQryRsp.GetSearchResultList().empty())
	{
		rsp.errorCode = ERR_CUI_PARAMS_INVALID;
	}
	else
	{
		rsp.errorCode = cCuVidsrcInfoQryRsp.GetErrorCode();
	}

	rsp.session = req.session;

	return SOAP_OK;
}

int cusdkService::GetDevAzimuthalAngle(cusdk__GetDevAzimuthalAngleReq req, cusdk__GetDevAzimuthalAngleRsp &rsp)
{
	// 方位角
	CCuGetAzimuthReq cCuGetAzimuthReq;
	cCuGetAzimuthReq.SetSession(req.session);
	cCuGetAzimuthReq.SetVidSrcChn(TChannel(req.deviceId, req.videosrcId));

	CCuGetAzimuthRsp cCuGetAzimuthRsp;

	CuGetAzimuth(cCuGetAzimuthReq, cCuGetAzimuthRsp);

	rsp.azimuthalAngleData = cCuGetAzimuthRsp.GetAzimuthInfo();
	rsp.errorCode = cCuGetAzimuthRsp.GetErrorCode();

	rsp.session = req.session;

	return SOAP_OK;
}
//// 尚未提供
//int cusdkService::GetDeviceGBIDBatch(cusdk__GetDeviceGBIDBatchReq req, cusdk__GetDeviceGBIDBatchRsp &rsp)
//{
//	rsp.errorCode = CMS_SUCCESS;
//	rsp.session = req.session;
//	return SOAP_OK;
//}
// 尚未提供
int cusdkService::GetChildrenGBDomainList(cusdk__GetChildrenGBDomainListReq req, cusdk__GetChildrenGBDomainListRsp &rsp)
{
	rsp.errorCode = CMS_SUCCESS;
	rsp.session = req.session;
	return SOAP_OK;
}
// 尚未提供
int cusdkService::GetGBDevciePushDomain(cusdk__GetGBDevicePushDomainReq req, cusdk__GetGBDevicePushDomainRsp &rsp)
{
	rsp.errorCode = CMS_SUCCESS;
	rsp.session = req.session;
	return SOAP_OK;
}
// 删除录像
int cusdkService::DeleteRecord(cusdk__DeleteRecordReq req, cusdk__DeleteRecordRsp &rsp)
{
	rsp.errorCode = CMS_SUCCESS;
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::AutoMatchGBId(cusdk__AutoMatchGBIdReq req, cusdk__AutoMatchGBIdRsp &rsp)
{
	CCuDeviceMatchGbidReq cCuDeviceMatchGbidReq;
	cCuDeviceMatchGbidReq.SetSession(req.session);
	cCuDeviceMatchGbidReq.SetDevTreeId(req.devTreeViewId);
	cCuDeviceMatchGbidReq.SetDevGrpId(req.devGroupId);
	cCuDeviceMatchGbidReq.SetDevId(req.deviceId);
	cCuDeviceMatchGbidReq.SetDevVidIndex(req.devVidIndex);
	cCuDeviceMatchGbidReq.SetDevMatchType(req.devMatchType);
	cCuDeviceMatchGbidReq.SetMatchLastBits(req.matchLastBits);

	CCuDeviceMatchGbidRsp cCuDeviceMatchGbidRsp;

	CuDeviceMatchGbid(cCuDeviceMatchGbidReq, cCuDeviceMatchGbidRsp);
	
	rsp.gbid = cCuDeviceMatchGbidRsp.GetGbid();
	rsp.operationId = cCuDeviceMatchGbidRsp.GetOperId();
	rsp.errorCode = cCuDeviceMatchGbidRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::GetGroupDevSumNum(cusdk__GetGroupDevSumNumReq req, cusdk__GetGroupDevSumNumRsp &rsp)
{
	CCuDeviceGrpDevSumNumReq cCuDeviceGrpDevSumNumReq;
	cCuDeviceGrpDevSumNumReq.SetSession(req.session);
	cCuDeviceGrpDevSumNumReq.SetDevTreeId(req.devTreeViewId);
	cCuDeviceGrpDevSumNumReq.SetDevGrpId(req.devGroupId);

	CCuDeviceGrpDevSumNumRsp cCuDeviceGrpDevSumNumRsp;

	CuDeviceGrpDevSumNum(cCuDeviceGrpDevSumNumReq, cCuDeviceGrpDevSumNumRsp);

	rsp.chnNum = cCuDeviceGrpDevSumNumRsp.GetTotalNum();
	rsp.errorCode = cCuDeviceGrpDevSumNumRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::ModifyGBDevice(cusdk__ModifyGBDeviceReq req, cusdk__ModifyGBDeviceRsp &rsp)
{
	CCuDeviceGroupDeviceMod4GbReq cCuDeviceGroupDeviceMod4GbReq;
	cCuDeviceGroupDeviceMod4GbReq.SetSession(req.session);
	cCuDeviceGroupDeviceMod4GbReq.SetDevTreeId(req.devTreeViewId);
	cCuDeviceGroupDeviceMod4GbReq.SetDeviceGroupID(req.devGroupId);
	cCuDeviceGroupDeviceMod4GbReq.SetDeviceId(req.deviceId);
	cCuDeviceGroupDeviceMod4GbReq.SetVidIndex(req.videoSrcId);
	cCuDeviceGroupDeviceMod4GbReq.SetOperatetype(req.operationType);
	cCuDeviceGroupDeviceMod4GbReq.SetGbid(req.gbID);

	CCuDeviceGroupDeviceMod4GbRsp cCuDeviceGroupDeviceMod4GbRsp;

	CuDeviceGroupDeviceMod4Gb(cCuDeviceGroupDeviceMod4GbReq, cCuDeviceGroupDeviceMod4GbRsp);

	rsp.devtreeId = cCuDeviceGroupDeviceMod4GbRsp.GetDevtreeId();
	rsp.devgroupId = cCuDeviceGroupDeviceMod4GbRsp.GetDevgroupId();
	rsp.deviceId = cCuDeviceGroupDeviceMod4GbRsp.GetDeviceId();
	rsp.vidIndex = cCuDeviceGroupDeviceMod4GbRsp.GetVidIndex();
	rsp.primaryOriginalGBId = cCuDeviceGroupDeviceMod4GbRsp.GetGbid().GetOldprimarygbid();
	rsp.secondOriginalGBId = cCuDeviceGroupDeviceMod4GbRsp.GetGbid().GetOldsecondarygbid();
	rsp.primaryNewGBId = cCuDeviceGroupDeviceMod4GbRsp.GetGbid().GetNewprimarygbid();
	rsp.secondNewGBId = cCuDeviceGroupDeviceMod4GbRsp.GetGbid().GetNewsecondarygbid();
	
	rsp.errorCode = cCuDeviceGroupDeviceMod4GbRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::StartDevicePush(cusdk__StartDevicePushReq req, cusdk__StartDevicePushRsp &rsp)
{
	CCuGbsCatalogSelectReportStartReq cCuGbsCatalogSelectReportStartReq;
	cCuGbsCatalogSelectReportStartReq.SetSession(req.session);
	cCuGbsCatalogSelectReportStartReq.SetParentGbDomainId(req.parentDomainId);
	cCuGbsCatalogSelectReportStartReq.SetDevTreeId(req.devTreeViewId);

	CCuGbsCatalogSelectReportStartRsp cCuGbsCatalogSelectReportStartRsp;
	CuGbsCatalogSelectReportStart(cCuGbsCatalogSelectReportStartReq, cCuGbsCatalogSelectReportStartRsp);

	rsp.errorCode = cCuGbsCatalogSelectReportStartRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::GetGBDeviceByGroup(cusdk__GetGBDeviceByGroupReq req, cusdk__GetGBDeviceByGroupRsp &rsp)
{
	CCuDeviceGroupDeviceQry4GbReq cCuDeviceGroupDeviceQry4GbReq;
	cCuDeviceGroupDeviceQry4GbReq.SetSession(req.session);
	cCuDeviceGroupDeviceQry4GbReq.SetExpectEntryNum(req.num);
	cCuDeviceGroupDeviceQry4GbReq.SetStartEntryIndex(req.beginIndex);
	
	cCuDeviceGroupDeviceQry4GbReq.GetCondition().SetDevicegroupId(req.groupId);
	cCuDeviceGroupDeviceQry4GbReq.GetCondition().SetDevTreeId(req.treeId);
	
	CCuDeviceGroupDeviceQry4GbRsp cCuDeviceGroupDeviceQry4GbRsp;
	CuGetDeviceGroupOfGBDeviceInfo(cCuDeviceGroupDeviceQry4GbReq, cCuDeviceGroupDeviceQry4GbRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuDeviceGroupDeviceQry4GbRsp.GetErrorCode();

	if (cCuDeviceGroupDeviceQry4GbRsp.GetActualResultNum() > 0)
	{
		rsp.totalNum = cCuDeviceGroupDeviceQry4GbRsp.GetTotalEntryNum();

		std::vector<CDeviceGroupDeviceInfo4Gb> &devInfoList = cCuDeviceGroupDeviceQry4GbRsp.GetDeviceGroupGBDeviceInfo();
		for (size_t nIndex = 0; nIndex < devInfoList.size(); nIndex++)
		{
			cusdk__CDeviceInfoGB cuDevInfo;
			ConvertDevGroupGBDevInfoFromCuiToCu(devInfoList[nIndex], cuDevInfo);
			rsp.devList.push_back(cuDevInfo);
		}
	}
	return SOAP_OK;
}

int cusdkService::GetNextDeviceGBIDError(cusdk__GetNextDeviceGBIDErrorReq req, cusdk__GetNextDeviceGBIDErrorRsp &rsp)
{
	CCuDeviceGbidErrGetReq cCuDeviceGbidErrGetReq;
	cCuDeviceGbidErrGetReq.SetSession(req.session);
	cCuDeviceGbidErrGetReq.SetDevTreeId(req.devTreeViewId);
	cCuDeviceGbidErrGetReq.SetGroupId(req.groupId);
	cCuDeviceGbidErrGetReq.SetErrIndex(req.errIndex);

	CCuDeviceGbidErrGetRsp cCuDeviceGbidErrGetRsp;

	CuDeviceGbidErrGet(cCuDeviceGbidErrGetReq, cCuDeviceGbidErrGetRsp);

	if (cCuDeviceGbidErrGetRsp.GetExistErr() == true)
	{
		cusdk__DevicePushErrorInfo errInfo;
		ConvertDevicePushErrorInfoFromCUIToCu(cCuDeviceGbidErrGetRsp.GetErrItem(), errInfo);
		rsp.nextError.push_back(errInfo);
	}

	rsp.errorCode = cCuDeviceGbidErrGetRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::SearchGBDevice(cusdk__SearchGBDeviceReq req, cusdk__SearchGBDeviceRsp &rsp)
{
	CCuDeviceGbinfoQryReq cCuDeviceGbinfoQryReq;
	cCuDeviceGbinfoQryReq.SetSession(req.session);
	cCuDeviceGbinfoQryReq.SetDevTreeId(req.deviceTreeViewId);
	cCuDeviceGbinfoQryReq.SetGbid(req.deviceGBID);
	cCuDeviceGbinfoQryReq.SetName(req.deviceName);
	cCuDeviceGbinfoQryReq.SetStartEntryIndex(req.beginIndex);
	cCuDeviceGbinfoQryReq.SetExpectEntryNum(req.requestNum);

	CCuDeviceGbinfoQryRsp cCuDeviceGbinfoQryRsp;

	CCuDeviceGbinfoQry(cCuDeviceGbinfoQryReq, cCuDeviceGbinfoQryRsp);

	ConvertDeviceFullGbInfoListFromCuiToCu(cCuDeviceGbinfoQryRsp.GetInfos(), rsp.searchResult);
	
	rsp.totalNum = cCuDeviceGbinfoQryRsp.GetTotalEntryNum();
	rsp.errorCode = cCuDeviceGbinfoQryRsp.GetErrorCode();
	rsp.session = req.session;
	return SOAP_OK;
}

int cusdkService::SearchGBDeviceGroup(cusdk__SearchGBDeviceGroupReq req, cusdk__SearchGBDeviceGroupRsp &rsp)
{
	CCuDevgrpGbinfoQryReq cCuDevgrpGbinfoQryReq;
	cCuDevgrpGbinfoQryReq.SetSession(req.session);
	cCuDevgrpGbinfoQryReq.SetDevTreeId(req.deviceTreeViewId);
	cCuDevgrpGbinfoQryReq.SetCivilCode(req.civilcode);
	cCuDevgrpGbinfoQryReq.SetName(req.groupName);
	cCuDevgrpGbinfoQryReq.SetStartEntryIndex(req.beginIndex);
	cCuDevgrpGbinfoQryReq.SetExpectEntryNum(req.requestNum);

	CCuDevgrpGbinfoQryRsp cCuDevgrpGbinfoQryRsp;

	CCuDevgrpGbinfoQry(cCuDevgrpGbinfoQryReq, cCuDevgrpGbinfoQryRsp);
	
	ConvertDevGrpFullGbInfoListFromCuiToCu(cCuDevgrpGbinfoQryRsp.GetInfos(), rsp.searchResult);

	rsp.errorCode = cCuDevgrpGbinfoQryRsp.GetErrorCode();
	rsp.session = req.session;
	rsp.totalNum = cCuDevgrpGbinfoQryRsp.GetTotalEntryNum();
	return SOAP_OK;
}


int cusdkService::GetServerConfig(cusdk__GetServerConfigReq req, cusdk__GetServerConfigRsp &rsp)
{
	CCuGetSyncVideoSrcNameEnableReq cReq;
	cReq.SetSession(req.session);

	CCuGetSyncVideoSrcNameEnableRsp cRsp;
	CuGetSyncVideoSrcNameEnableAlias(cReq, cRsp);

	cRsp.GetSwichSets(DISPLAY_AREA_INFO, rsp.enableDecoderShowAreaInfo);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
};

int cusdkService::SetServerConfig(cusdk__SetServerConfigReq req, cusdk__SetServerConfigRsp &rsp)
{
	CCuSetSyncVideoSrcNameEnableReq cReq;
	cReq.SetSession(req.session);
	cReq.SetSwichSets(DISPLAY_AREA_INFO, req.enableDecoderShowAreaInfo);

	CCuSetSyncVideoSrcNameEnableRsp cRsp;
	CuSetSyncVideoSrcNameEnableAlias(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
};


int cusdkService::GetPULongOSD(cusdk__GetPULongOSDReq req, cusdk__GetPULongOSDRsp &rsp)
{
	CCuPuLongOsdGetReq cReq;
	cReq.SetSession(req.session);

	TChannel tChn;
	tChn.SetDevUri(req.deviceId);
	tChn.SetChnNO(req.videoSrcId);

	cReq.SetDevChn(tChn);

	CCuPuLongOsdGetRsp cRsp;
	CuPuLongOsdGet(cReq, cRsp);

	ConvertCPuLongOsdParamFromCuiToCu(cRsp.GetParam(), rsp.longOSDParam);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::SetPULongOSD(cusdk__SetPULongOSDReq req, cusdk__SetPULongOSDRsp &rsp)
{
	CCuPuLongOsdSetReq cReq;
	cReq.SetSession(req.session);

	TChannel tChn;
	tChn.SetDevUri(req.deviceId);
	tChn.SetChnNO(req.videoSrcId);

	TLongOsdParam tParam;
	tParam.SetColor(req.longOSDParam.color);
	tParam.SetLeftMargin(req.longOSDParam.leftMargin);
	switch (req.longOSDParam.state)
	{
	case cusdk__OSDStateDisable:
		tParam.SetState(OSD_DISABLE);
		break;
	case cusdk__OSDStateText:
		tParam.SetState(OSD_TITLE);
		break;
	case cusdk__OSDStateInvalid:
		tParam.SetState(OSD_DISABLE);
		break;
	default:
		tParam.SetState(OSD_DISABLE);
		break;
	}
	tParam.SetText(req.longOSDParam.text);
	tParam.SetTopMargin(req.longOSDParam.topMargin);

	cReq.SetDevChn(tChn);
	cReq.SetParam(tParam);

	CCuPuLongOsdSetRsp cRsp;
	CuPuLongOsdSet(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::LockRecord(cusdk__LockRecordReq req, cusdk__LockRecordRsp &rsp)
{
	vector<cusdk__RecDuration>::iterator it = req.recordDurationList.begin();
	if (it != req.recordDurationList.end())
	{
		CCuRcsRecordLockOprReq cCuRcsRecordLockOprReq;

		cCuRcsRecordLockOprReq.SetRecToken((*it).recordDurationId);
		cCuRcsRecordLockOprReq.SetSession(req.session);

		CCuRcsRecordLockOprRsp cCuRcsRecordLockOprRsp;
		CuPlatformRecordLock(cCuRcsRecordLockOprReq, cCuRcsRecordLockOprRsp);

		rsp.errorCode = cCuRcsRecordLockOprRsp.GetErrorCode();
		rsp.session = req.session;
		return SOAP_OK;
	}

	return SOAP_ERR;	
}

int cusdkService::UnlockRecord(cusdk__UnlockRecordReq req, cusdk__UnlockRecordRsp &rsp)
{
	vector<cusdk__RecDuration>::iterator it = req.recordDurationList.begin();
	if (it != req.recordDurationList.end())
	{
		CCuRcsRecordUnLockOprReq cReq;
		TRecLockOprType tRecLockOprType = RCS_REC_UNLOCK;

		cReq.SetOprType(tRecLockOprType);
		cReq.SetRecToken((*it).recordDurationId);
		cReq.SetSession(req.session);

		CCuRcsRecordUnLockOprRsp cRsp;
		CuPlatformRecordUnLock(cReq, cRsp);

		rsp.errorCode = cRsp.GetErrorCode();
		rsp.session = req.session;
		return SOAP_OK;
	}

	return SOAP_OK;
}

int cusdkService::TVWallClearAlarm(cusdk__TVWallClearAlarmReq req, cusdk__TVWallClearAlarmRsp &rsp)
{
	CCuRealMediaSwitchOccurTypeReq cReq;
	cReq.SetSession(req.session);

	TTvChannel  tvChn;
	tvChn.SetTvWallId(req.TVWallId);
	tvChn.SetTvId(req.tvId);
	tvChn.SetTvDivId(req.tvDivId);

	cReq.SetTvChn(tvChn);
	cReq.SetDstOccurType(TV_OCCUR_TYPE_HAND);

	CCuRealMediaSwitchOccurTypeRsp cRsp;
	CuRealMediaSwitchOccurTypeReq(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();

	return SOAP_OK;
	//add by haoruijie 20170531
}

/// Web service operation 'LockRecordBatch' (returns error code or SOAP_OK)
int cusdkService::LockRecordBatch(cusdk__LockRecordBatchReq req, cusdk__LockRecordBatchRsp &rsp)
{
	return SOAP_OK;
}


/// Web service operation 'UnlockRecordBatch' (returns error code or SOAP_OK)
int cusdkService::UnlockRecordBatch(cusdk__UnlockRecordBatchReq req, cusdk__UnlockRecordBatchRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'QueryLockedRecord' (returns error code or SOAP_OK)
int cusdkService::QueryLockedRecord(cusdk__QueryLockedRecordReq req, cusdk__QueryLockedRecordRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'GetStreamPreDispatchConfig' (returns error code or SOAP_OK)
int cusdkService::GetStreamPreDispatchConfig(cusdk__GetStreamPreDispatchConfigReq req, cusdk__GetStreamPreDispatchConfigRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'SetStreamPreDispatchConfig' (returns error code or SOAP_OK)
int cusdkService::SetStreamPreDispatchConfig(cusdk__SetStreamPreDispatchConfigReq req, cusdk__SetStreamPreDispatchConfigRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'QueryStreamPreDispatchDevice' (returns error code or SOAP_OK)
int cusdkService::QueryStreamPreDispatchDevice(cusdk__QueryStreamPreDispatchDeviceReq req, cusdk__QueryStreamPreDispatchDeviceRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'StreamPreDispatchAddDevice' (returns error code or SOAP_OK)
int cusdkService::StreamPreDispatchAddDevice(cusdk__StreamPreDispatchAddDeviceReq req, cusdk__StreamPreDispatchAddDeviceRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'StreamPreDispatchDelDevice' (returns error code or SOAP_OK)
int cusdkService::StreamPreDispatchDelDevice(cusdk__StreamPreDispatchDelDeviceReq req, cusdk__StreamPreDispatchDelDeviceRsp &rsp)
{
	return SOAP_OK;
}

/// Web service operation 'GetEncChnRes' (returns error code or SOAP_OK)
int cusdkService::GetEncChnRes(cusdk__GetEncChnResReq req, cusdk__GetEncChnResRsp& rsp)
{
	return SOAP_OK;
}

/// Web service operation 'AllocEncChnRes' (returns error code or SOAP_OK)
int cusdkService::AllocEncChnRes(cusdk__AllocEncChnResReq req, cusdk__AllocEncChnResRsp& rsp)
{
	return SOAP_OK;
}

/// Web service operation 'ReleaseEncChnRes' (returns error code or SOAP_OK)
int cusdkService::ReleaseEncChnRes(cusdk__ReleaseEncChnResReq req, cusdk__ReleaseEncChnResRsp& rsp)
{
	return SOAP_OK;
}

/// Web service operation 'GBPuTransdataSsSet' (returns error code or SOAP_OK)
int cusdkService::GBPuTransdataSsSet( cusdk__GBPuTransdataSsSetReq req, cusdk__GBPuTransdataSsSetRsp& rsp )
{
	return SOAP_OK;
}

int cusdkService::GetGBDomainPrivilege(cusdk__GetGBDomainPrivilegeReq req, cusdk__GetGBDomainPrivilegeRsp& rsp)
{
	CCuGbsGbdomainParentPrivilegeGetReq cCuGbsGbdomainParentPrivilegeGetReq;
	cCuGbsGbdomainParentPrivilegeGetReq.SetSession(req.session);
	cCuGbsGbdomainParentPrivilegeGetReq.SetParentid(req.domainId);

	CCuGbsGbdomainParentPrivilegeGetRsp cCuGbsGbdomainParentPrivilegeGetRsp;
	CuGetGBDomainPrivilege(cCuGbsGbdomainParentPrivilegeGetReq, cCuGbsGbdomainParentPrivilegeGetRsp);

	rsp.domainId = cCuGbsGbdomainParentPrivilegeGetRsp.GetParentid();
	rsp.priRecordPlay = cCuGbsGbdomainParentPrivilegeGetRsp.GetRecplay();
	rsp.priRecordDownload = cCuGbsGbdomainParentPrivilegeGetRsp.GetRecdownload();
	rsp.priPTZ = cCuGbsGbdomainParentPrivilegeGetRsp.GetPtz();
	rsp.session = req.session;
	rsp.errorCode = cCuGbsGbdomainParentPrivilegeGetRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::SetGBDomainPrivilege(cusdk__SetGBDomainPrivilegeReq req, cusdk__SetGBDomainPrivilegeRsp& rsp)
{
	CCuGbsGbdomainParentPrivilegeSetReq cCuGbsGbdomainParentPrivilegeSetReq;
	cCuGbsGbdomainParentPrivilegeSetReq.SetSession(req.session);
	cCuGbsGbdomainParentPrivilegeSetReq.SetParentid(req.domainId);
	cCuGbsGbdomainParentPrivilegeSetReq.SetRecplay(req.priRecordPlay);
	cCuGbsGbdomainParentPrivilegeSetReq.SetRecdownload(req.priRecordDownload);
	cCuGbsGbdomainParentPrivilegeSetReq.SetPtz(req.priPTZ);

	CCuGbsGbdomainParentPrivilegeSetRsp cCuGbsGbdomainParentPrivilegeSetRsp;
	CuSetGBDomainPrivilege(cCuGbsGbdomainParentPrivilegeSetReq, cCuGbsGbdomainParentPrivilegeSetRsp);

	rsp.session = req.session;
	rsp.errorCode = cCuGbsGbdomainParentPrivilegeSetRsp.GetErrorCode();

	return SOAP_OK;
}

int cusdkService::GetGBConfig(cusdk__GetGBConfigReq req, cusdk__GetGBConfigRsp &rsp)
{
	CCuGbsOptionConfGetReq gbsReq;
	gbsReq.SetSession(req.session);
	CCuGbsOptionConfGetRsp gbsRsp;
	CuGetGbsOptionConf(gbsReq, gbsRsp);
	rsp.session = req.session;
	rsp.syncLatitudeAndLongitudeFromDevice = gbsRsp.GetConf().syncGbpuLongLati;
	rsp.errorCode = gbsRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::SetGBConfig(cusdk__SetGBConfigReq req, cusdk__SetGBConfigRsp &rsp)
{
	CCuGbsOptionConfSetReq gbsReq;
	gbsReq.SetSession(req.session);
	gbsReq.SetSyncGbpuLongLati(req.syncLatitudeAndLongitudeFromDevice);
	CCuGbsOptionConfSetRsp gbsRsp;
	CuSetGbsOptionConf(gbsReq, gbsRsp);
	rsp.session = req.session;	
	rsp.errorCode = gbsRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::SetCertificate(cusdk__SetCertificateReq req, cusdk__SetCertificateRsp &rsp)
{
	CCuGbsCertificateUploadReq cReq;
	cReq.SetSession(req.session);
	cReq.SetCert4encry(req.cert4encry);
	cReq.SetCert4sign(req.cert4sign);
	cReq.SetType(req.type);
	cReq.SetOwnerid(req.ownerid);
	cReq.SetValidField(req.validField);
	CCuGbsCertificateUploadRsp cRsp;
	CuUploadGbsCertificate(cReq, cRsp);
	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::GetCertificate(cusdk__GetCertificateReq req, cusdk__GetCertificateRsp &rsp)
{
	CCuGbsCertificateGetReq cReq;
	cReq.SetSession(req.session);
	cReq.SetOwnerid(req.ownerid);
	CCuGbsCertificateGetRsp cRsp;
	CuGetGbsCertificate(cReq, cRsp);
	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();
	rsp.cert4encry = cRsp.GetCert4encry();
	rsp.cert4sign = cRsp.GetCert4sign();
	rsp.ownerid = cRsp.GetOwnerid();
	rsp.type = cRsp.GetType();
	rsp.validField = cRsp.GetValidField();
	return SOAP_OK;
}

int cusdkService::GetDeviceCertificate(cusdk__GetDeviceCertificateReq req, cusdk__GetDeviceCertificateRsp &rsp)
{

	//CuiWriteLog( "/tmp/cuifcgi.log", "cusdkService::GetDeviceCertificate begin \n" );
	CCuCertificateQryReq cReq;
	cReq.SetSession(req.session);
	cReq.SetId(req.deviceId);
	cReq.SetModuleId(req.session);

	CCuCertificateQryRsp cRsp;
	CuCertificateQry(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();
	rsp.encryptCert = cRsp.GetEncryptCert();
	rsp.signatureCert = cRsp.GetSignatureCert();

	//CuiWriteLog( "/tmp/cuifcgi.log", "cusdkService::GetDeviceCertificate end \n" );

	return SOAP_OK;
}

int cusdkService::DeviceSignatrueCtrl(cusdk__DeviceSignatrueCtrlReq req, cusdk__DeviceSignatrueCtrlRsp &rsp)
{
	CCuDeviceSignatrueCtrlReq cReq;
	cReq.SetDevUri(req.deviceId);
	cReq.SetEnabled(req.enabled);
	cReq.SetSession(req.session);

	CCuDeviceSignatrueCtrlRsp cRsp;
	CuDeviceSignatrueCtrl(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();
	return SOAP_OK;
}

int cusdkService::DeviceEncryptionCtrl(cusdk__DeviceEncryptionCtrlReq req, cusdk__DeviceEncryptionCtrlRsp &rsp)
{
	CCuDeviceEncryptionCtrlReq cReq;
	cReq.SetDevUri(req.deviceId);
	cReq.SetEnabled(req.enabled);
	cReq.SetSession(req.session);

	CCuDeviceEncryptionCtrlRsp cRsp;
	CuDeviceEncryptionCtrl(cReq, cRsp);

	rsp.session = req.session;
	rsp.errorCode = cRsp.GetErrorCode();


	return SOAP_OK;
}

int cusdkService::BidirectionalAuth1(cusdk__BidirectionalAuth1Req req, cusdk__BidirectionalAuth1Rsp &rsp)
{
	CCuSecurityAuthorizationFirstReq cReq;
	cReq.SetSession(req.session);
	cReq.SetAlgorithm(req.algorithm);

	CCuSecurityAuthorizationFirstRsp cRsp;

	CuSecurityAuthenticateFirstTime(cReq, cRsp);

	rsp.errorCode = cRsp.GetErrorCode();
	rsp.session = cRsp.GetSession();
	rsp.random1 = cRsp.GetRandom1();
	rsp.algorithm = cRsp.GetAlgorithm();

	return SOAP_OK;
}

int cusdkService::BidirectionalAuth2(cusdk__BidirectionalAuth2Req req, cusdk__BidirectionalAuth2Rsp &rsp)
{

	CCuSecurityAuthorizationSecondReq cReq;
	cReq.SetSession(req.session);
	cReq.SetServerId(req.serverId);
	cReq.SetAlgorithm(req.algorithm);
	cReq.SetRandom1(req.random1);
	cReq.SetRandom2(req.random2);
	cReq.SetSign1(req.sign1);

	CCuSecurityAuthorizationSecondRsp cRsp;

	CuSecurityAuthenticateSecondTime(cReq, cRsp);
	rsp.errorCode = cRsp.GetErrorCode();
	rsp.session = cRsp.GetSession();
	rsp.random1 = cRsp.GetRandom1();
	rsp.random2 = cRsp.GetRandom2();
	rsp.cryptkey = cRsp.GetCrptKey();
	rsp.sign2 = cRsp.GetSign2();
	rsp.algorithm = cRsp.GetAlgorithm();

	return SOAP_OK;
}
