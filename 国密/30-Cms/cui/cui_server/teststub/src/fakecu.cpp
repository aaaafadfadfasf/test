#include "fakecu.h"
//#include "soapStub.h"
#include "cuiproto.h"
#include "serviceappimp.h"

string g_strUserName = "admin";
string g_strCuUserId = "acf9c72660df4574909dac20daa58af6";
string g_strDomainId = "6e503463ef9e4dd7b3f933f3888aaede";
string g_strDomainName = "china";
string g_strGroupId = "4f4d84d9ebbd4036ac19482679b66173";
string g_strDevURI = "d5a00519995e4ca7b60a0349a77781d2@china";
string g_strUserPswd = "888888";

int nType = 0;

extern string g_strCuInCmuSess;
extern bool g_bLogin;

string GetCuiServerIpStr()
{
    //std::string strCuiIp("172.16.251.6");
    std::string strCuiIp("127.0.0.1");
    return strCuiIp;
}

u16 GetCuiServerTcpListenPort()
{
    return CUI_TCP_DAT_RECV_PORT;
}
u32 GetCuiServerTimeout()
{
    return 90;
}

API void n()
{
    g_bLogin = !g_bLogin;
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
void login()
{
    CCuiCuLoginReq cLoginReq;
    cLoginReq.SetUserName(g_strUserName + "@" + g_strDomainName);
    cLoginReq.SetPassword(g_strUserPswd);
    cLoginReq.SetVersion("2012.2.29");

    CCuiCuLoginRsp cLoginRsp;
    CuLogin(cLoginReq, cLoginRsp);
    g_strCuInCmuSess = cLoginRsp.GetSession();
    if ( cLoginRsp.GetErrorCode() == CMS_SUCCESS )
    {
        g_bLogin = true;
        GetUserInfo();
    }
    else
    {
        OspPrintf(TRUE, FALSE, "登录失败\n");
    }

    return;
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
void logout(const char *pSess = NULL)
{
    CCuLogoutReq cCuLogoutReq;
    if ( NULL == pSess )
    {
        cCuLogoutReq.SetSession(g_strCuInCmuSess);
    }
    else
    {
        cCuLogoutReq.SetSession(pSess);
    }

    CCuLogoutRsp cCuLogoutRsp;
    CuLogout(cCuLogoutReq, cCuLogoutRsp);
    g_bLogin = false;
    return;
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
void SubscribePuStatus( )
{
    CCuSubscribePuStatusReq cCuSubscribePuStatusReq;
    cCuSubscribePuStatusReq.SetSession(g_strCuInCmuSess);

    CPuStatusSubscription cSub;
    cSub.SetAllSubscribeType();
    cSub.SetDeviceURI(g_strDevURI);
    cCuSubscribePuStatusReq.GetSubscribeList().push_back(cSub);

    CCuSubscribePuStatusRsp cCuSubscribePuStatusRsp;
    CuSubscribePuStatus(cCuSubscribePuStatusReq, cCuSubscribePuStatusRsp);
    return;
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
void UnsubscribePuStatus( )
{
    CCuUnSubscribePuStatusReq cCuUnSubscribePuStatusReq;
    cCuUnSubscribePuStatusReq.SetSession(g_strCuInCmuSess);

    CPuStatusSubscription cSub;
    cSub.SetAllSubscribeType();
    cSub.SetDeviceURI(g_strDevURI);
    cCuUnSubscribePuStatusReq.GetUnSubscribeList().push_back(cSub);

    CCuUnSubscribePuStatusRsp cCuUnSubscribePuStatusRsp;
    CuUnsubscribePuStatus(cCuUnSubscribePuStatusReq, cCuUnSubscribePuStatusRsp);
    return;
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
void GetNotify()
{
    CGetNotifyReq cGetNotifyReq;
    cGetNotifyReq.SetSession(g_strCuInCmuSess);

    CGetNotifyRsp cGetNotifyRsp;
    CuGetNotify(cGetNotifyReq, cGetNotifyRsp);

    list< CInviteStopNotify >::const_iterator pItem = cGetNotifyRsp.notifyPool.inviteStopNotify.begin();
    while ( pItem != cGetNotifyRsp.notifyPool.inviteStopNotify.end() )
    {
        OspPrintf(TRUE, FALSE, "收到平台的Invite通知，停止播放窗口 %d，原因是 %d\n",
            pItem->playId, pItem->errorCode);
        pItem++;
    }

    return;
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
void GetCUIList()
{
    CCuGetCuiListReq cCuGetCuiListReq;
    cCuGetCuiListReq.SetSession(g_strCuInCmuSess);

    CCuGetCuiListRsp cCuGetCuiListRsp;
    CuGetCuiList(cCuGetCuiListReq, cCuGetCuiListRsp);

    return;
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
void PtzCtrl()
{
    CPuPtzCtrlReq cCuPtzCtrlReq;
    cCuPtzCtrlReq.SetSession(g_strCuInCmuSess);
    TChannel tDevChn;
    tDevChn.SetDevUri(g_strDevURI);
    tDevChn.SetChnNO(1);
    cCuPtzCtrlReq.SetDevChn(tDevChn);
    TCamCtrl tCamCtrl;
    tCamCtrl.SetCommandId(1);
    tCamCtrl.GetParams().push_back(1);
    cCuPtzCtrlReq.SetCamCtrl(tCamCtrl);

    CPuPtzCtrlRsp cCuPtzCtrlRsp;
    CuPtzCtrl(cCuPtzCtrlReq, cCuPtzCtrlRsp);
    return;
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
void PtzLock()
{
    CPuPtzLockReq cCuPtzLockReq;
    cCuPtzLockReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cCuPtzLockReq.SetPuChn(tPuChn);
    cCuPtzLockReq.SetLockPassword("123");
    time_t tCurTime;
    time(&tCurTime);
    cCuPtzLockReq.SetBeginLockTime(tCurTime);
    cCuPtzLockReq.SetEndLockTime(tCurTime + 1000);

    CPuPtzLockRsp cCuPtzLockRsp;
    CuPtzLock(cCuPtzLockReq, cCuPtzLockRsp);


    return;
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
void PtzUnlock()
{
    CPuPtzUnlockReq cCuPtzUnlockReq;
    cCuPtzUnlockReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cCuPtzUnlockReq.SetPuChn(tPuChn);
    cCuPtzUnlockReq.SetUnlockPassword("123");

    CPuPtzUnlockRsp cCuPtzUnlockRsp;
    CuPtzUnlock(cCuPtzUnlockReq, cCuPtzUnlockRsp);

    return;
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
2012/2/1/             liangli
====================================================================*/
/// Web service operation 'SendTransData' (returns error code or SOAP_OK)
void SendTransData()
{
    CPuSendTransparentDataReq cDataReq;
    cDataReq.SetSession(g_strCuInCmuSess);
    TChannel tPuChn;
    tPuChn.SetDevUri(g_strDevURI);
    tPuChn.SetChnNO(1);
    cDataReq.SetDevChn(tPuChn);
    cDataReq.SetHexTransparentData("abc");

    CPuSendTransparentDataRsp cDataRsp;
    CuSendTransData(cDataReq, cDataRsp);
    return;
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
void GetUserInfo()
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(g_strCuInCmuSess);
    CUserInfoQueryCondition cCondition;
    string strUserName = GetNameWithoutDomainFromURI(g_strUserName); //查询CU本身信息, ligang@nanjing 转换为ligang
    cCondition.SetUserName(strUserName);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo
    if ( cCuQueryUserRsp.GetUserInfo().empty() )
    {
        OspPrintf(TRUE, FALSE, "警告，获取用户信息失败\n");
    }
    else
    {
        g_strCuUserId = cCuQueryUserRsp.GetUserInfo().at(0).GetUserID();
        g_strDomainId = cCuQueryUserRsp.GetUserInfo().at(0).GetDomainID();
        GetUserDev();
    }
    return;
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
void GetUserList()
{
    CCuQueryUserReq cCuQueryUserReq;
    cCuQueryUserReq.SetSession(g_strCuInCmuSess);
    cCuQueryUserReq.SetStartEntryIndex(0);
    cCuQueryUserReq.SetExpectEntryNum(1000);
    CUserInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuQueryUserReq.SetUserInfoQueryCondition(cCondition);

    CCuQueryUserRsp cCuQueryUserRsp;
    CuGetUserInfo(cCuQueryUserReq, cCuQueryUserRsp);  //CuGetSelfUserInfo
    printf("cCuQueryUserRsp.GetTotalEntryNum() = %d\n",
        cCuQueryUserRsp.GetTotalEntryNum());
    printf("cCuQueryUserRsp.GetUserInfo().size() = %d\n",
        cCuQueryUserRsp.GetUserInfo().size());
    printf("cCuQueryUserRsp.GetActualResultNum() = %d\n",
        cCuQueryUserRsp.GetActualResultNum());
    return;
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
void GetUserGroupInfo()
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetGroupNO("123");
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    return;
}

/*====================================================================
函数名      : GetUserGroupList
功能        : 获取当前登录CU的子用户组
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
void GetUserGroupList()
{
    CCuUserGroupQryReq cCuQueryUserGroupReq;
    cCuQueryUserGroupReq.SetSession(g_strCuInCmuSess);
    cCuQueryUserGroupReq.SetStartEntryIndex(0);
    cCuQueryUserGroupReq.SetExpectEntryNum(1);
    CUserGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuQueryUserGroupReq.SetUserGroupInfoQueryCondition(cCondition);

    CCuUserGroupQryRsp cCuQueryUserGroupRsp;
    CuGetUserGroupInfo(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);

    return;
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
void GetUserData()
{
    CCuGetUserDataReq cCuGetUserDataReq;
    cCuGetUserDataReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDataReq.SetKey("123");

    CCuGetUserDataRsp cCuGetUserDataRsp;
    CuGetUserData(cCuGetUserDataReq, cCuGetUserDataRsp);

    return;
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
void SetUserData()
{
    CCuSetUserDataReq cCuSetUserDataReq;
    cCuSetUserDataReq.SetSession(g_strCuInCmuSess);
    cCuSetUserDataReq.SetKey("123");
    cCuSetUserDataReq.SetUserData("123");

    CCuSetUserDataRsp cCuSetUserDataRsp;
    CuSetUserData(cCuSetUserDataReq, cCuSetUserDataRsp);

    return;
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
void GetUserDevGroupByGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCondition.SetDeviceParentGroupID("123");
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);
    return;
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
void GetUserDevGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);

    return;
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
void GetDomainDevGroup()
{
    CCuUserDevGroupQryReq cCuGetUserDevGroupReq;
    cCuGetUserDevGroupReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevGroupReq.SetExpectEntryNum(1);
    cCuGetUserDevGroupReq.SetStartEntryIndex(0);
    CDeviceGroupInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);
    cCondition.SetDomainUUID(g_strDomainId);
    cCuGetUserDevGroupReq.SetDeviceGroupInfoQueryCondition(cCondition);

    CCuUserDevGroupQryRsp cCuGetUserDevGroupRsp;
    CuGetUserDevGroup(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);
    return;
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
void GetUserDevByGroup()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDevicegroupId("123");
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    return;
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
void GetUserDev()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);
    if ( cCuGetUserDevFullInfoRsp.GetErrorCode() != CMS_SUCCESS )
    {
        OspPrintf(TRUE, FALSE, "获取用户设备失败，错误码[%d]\n", 
            cCuGetUserDevFullInfoRsp.GetErrorCode());
    }
    else
    {
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetTotalEntryNum() = %d\n",
            cCuGetUserDevFullInfoRsp.GetTotalEntryNum());
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetUserDevFullInfo().size() = %d\n",
            cCuGetUserDevFullInfoRsp.GetUserDevFullInfo().size());
        OspPrintf(TRUE, FALSE, "cCuGetUserDevFullInfoRsp.GetActualResultNum() = %d\n",
            cCuGetUserDevFullInfoRsp.GetActualResultNum());
    }

    return;
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
void GetUserDevByDomain()
{
    CCuUserDevFullInfoQryReq cCuGetUserDevFullInfoReq;
    cCuGetUserDevFullInfoReq.SetSession(g_strCuInCmuSess);
    cCuGetUserDevFullInfoReq.SetStartEntryIndex(0);
    cCuGetUserDevFullInfoReq.SetExpectEntryNum(1);
    cCuGetUserDevFullInfoReq.SetCuserId(g_strCuUserId);
    CCuserDevicefullinfoAndPrivilegeQueryCondition cCondition;
    cCondition.SetDomainId(g_strDomainId);
    cCuGetUserDevFullInfoReq.SetCondition(cCondition);

    CCuUserDevFullInfoQryRsp cCuGetUserDevFullInfoRsp;
    CuGetUserDevFullInfo(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);

    return;
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
void GetDeviceGroupDeviceRelations()
{
    CCuQryDevGroupOfDevInfoReq cCuQryDevGroupOfDevInfoReq;
    cCuQryDevGroupOfDevInfoReq.SetSession(g_strCuInCmuSess);
    cCuQryDevGroupOfDevInfoReq.SetExpectEntryNum(1);
    cCuQryDevGroupOfDevInfoReq.SetStartEntryIndex(0);
    CDeviceGroupDeviceInfoQueryCondition cCondition;
    cCondition.SetCuserID(g_strCuUserId);

    CCuQryDevGroupOfDevInfoRsp cCuQryDevGroupOfDevInfoRsp;
    CuGetDeviceGroupOfDeviceInfo(cCuQryDevGroupOfDevInfoReq, cCuQryDevGroupOfDevInfoRsp);

    return;
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
void CreateDeviceGroup()
{
    CCuCreateDeviceGroupReq cCuCreateDeviceGroupReq;
    cCuCreateDeviceGroupReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupInfo cDevGrpInfo;
    cDevGrpInfo.SetDeviceGroupName("devgroup");
    cDevGrpInfo.SetDeviceParentGroupID("123");
    cDevGrpInfo.SetDomainUUID(g_strDomainId);
    cDevGrpInfo.SetDeviceGroupDesc("desc");
    cCuCreateDeviceGroupReq.SetDeviceGroupInfo(cDevGrpInfo);

    CCuCreateDeviceGroupRsp cCuCreateDeviceGroupRsp;
    CuCreateDeviceGroup(cCuCreateDeviceGroupReq, cCuCreateDeviceGroupRsp);

    return;
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
void DelDeviceGroup()
{
    CCuDeleteDeviceGroupReq cCuDeleteDeviceGroupReq;
    cCuDeleteDeviceGroupReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupInfoKey cDevGroupId;
    cDevGroupId.SetDeviceGroupID(g_strGroupId);
    cCuDeleteDeviceGroupReq.SetDeviceGroupInfoKey(cDevGroupId);

    CCuDeleteDeviceGroupRsp cCuDeleteDeviceGroupRsp;
    CuDeleteDeviceGroup(cCuDeleteDeviceGroupReq, cCuDeleteDeviceGroupRsp);

    return;
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
void ModifyDeviceGroup()
{
    CCuModifyDeviceGroupReq cCuModifyDeviceGroupReq;
    cCuModifyDeviceGroupReq.SetSession(g_strCuInCmuSess);
    cCuModifyDeviceGroupReq.SetDeviceGroupID(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceGroupName(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceParentGroupID(g_strGroupId);
    cCuModifyDeviceGroupReq.SetDeviceGroupDesc(g_strGroupId);

    CCuModifyDeviceGroupRsp cCuModifyDeviceGroupRsp;
    CuModifyDeviceGroup(cCuModifyDeviceGroupReq, cCuModifyDeviceGroupRsp);
    return;
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
void DeviceGroupAddDevice()
{
    CCuDeviceGroupAddDeviceReq cCuDeviceGroupAddDeviceReq;
    cCuDeviceGroupAddDeviceReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupDeviceInfo cInfo;
    cInfo.SetDeviceGroupID(g_strGroupId);
    cInfo.SetDeviceID(g_strGroupId);
    cInfo.SetDomainId(g_strDomainId);
    TDeviceCapIndexs Indexs;
    Indexs.id = 1;
    Indexs.indexs.push_back(1);
    cInfo.GetDeviceCapIndexs().push_back(Indexs);
    cCuDeviceGroupAddDeviceReq.GetDeviceGroupDeviceInfo().push_back(cInfo);

    CCuDeviceGroupAddDeviceRsp cCuDeviceGroupAddDeviceRsp;
    CuDeviceGroupAddDevice( cCuDeviceGroupAddDeviceReq, cCuDeviceGroupAddDeviceRsp );

    return;
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
void DeviceGroupDelDevice()
{
    CCuDeviceGroupDelDeviceReq cCuDeviceGroupDelDeviceReq;
    cCuDeviceGroupDelDeviceReq.SetSession(g_strCuInCmuSess);
    CDeviceGroupDeviceInfo cInfo;
    cInfo.SetDeviceGroupID(g_strGroupId);
    cInfo.SetDeviceID(g_strGroupId);
    cInfo.SetDomainId(g_strDomainId);
    TDeviceCapIndexs Indexs;
    Indexs.id = 1;
    Indexs.indexs.push_back(1);
    cInfo.GetDeviceCapIndexs().push_back(Indexs);
    cCuDeviceGroupDelDeviceReq.GetDeviceGroupDeviceInfo().push_back(cInfo);

    CCuDeviceGroupDelDeviceRsp cCuDeviceGroupDelDeviceRsp;
    CuDeviceGroupDelDevice( cCuDeviceGroupDelDeviceReq, cCuDeviceGroupDelDeviceRsp );
    return;
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
void CreateUserGroup()
{
    CCuCreateUserGroupReq cCuCreateUserGroupReq;
    cCuCreateUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfo cUserGroupInfo;
    cUserGroupInfo.SetGroupName(g_strUserName);
    cUserGroupInfo.SetDescription(g_strUserName);
    cUserGroupInfo.SetUserType(1);
    CUserPrivilege cUserPri;
    cUserPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);
    cUserGroupInfo.SetUserPrivilege(cUserPri);
    cUserGroupInfo.SetCreateUser(g_strCuUserId);

    CCuCreateUserGroupRsp cCuCreateUserGroupRsp;
    CuCreateUserGroup(cCuCreateUserGroupReq, cCuCreateUserGroupRsp);

    return;
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
void DelUserGroup()
{
    CCuDeleteUserGroupReq cCuDeleteUserGroupReq;
    cCuDeleteUserGroupReq.SetSession(g_strCuInCmuSess);
    CUserGroupInfoKey cUserGroupInfoKey;
    cUserGroupInfoKey.SetGroupNO(g_strGroupId);
    cCuDeleteUserGroupReq.SetUserGroupInfoKey(cUserGroupInfoKey);

    CCuDeleteUserGroupRsp cCuDeleteUserGroupRsp;
    CuDeleteUserGroup(cCuDeleteUserGroupReq, cCuDeleteUserGroupRsp);

    return;
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
void ModifyUserGroup()
{
    CCuModifyUserGroupReq cCuModifyUserGroupReq;
    cCuModifyUserGroupReq.SetSession(g_strCuInCmuSess);
    cCuModifyUserGroupReq.SetGroupNO(g_strGroupId);
    cCuModifyUserGroupReq.SetGroupName(g_strGroupId);
    cCuModifyUserGroupReq.SetDescription(g_strGroupId);
    CUserPrivilege cUserPri;
    cUserPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);

    cCuModifyUserGroupReq.SetUserPrivilege(cUserPri);
    //不允许修改ID，创建日期，用户组类型和创建者ID，创建者ID只能后台由TAS自动修改

    CCuModifyUserGroupRsp cCuModifyUserGroupRsp;
    CuModifyUserGroup(cCuModifyUserGroupReq, cCuModifyUserGroupRsp);

    return;
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
void CreateUser()
{
    CCuCreateUserReq cCuCreateUserReq;
    cCuCreateUserReq.SetSession(g_strCuInCmuSess);
    CCuUserInfo cUserInfo;
	string strNewUser = "aaa";
    cUserInfo.SetUserName(strNewUser + "@" + g_strDomainName);
    CUserPrivilege cPri;
    cPri.SetPrivilege(CUserPrivilege::en_Call_Ctrl);
    cUserInfo.SetUserPrivilege(cPri);
    cCuCreateUserReq.SetUserInfo(cUserInfo);

    CCuCreateUserRsp cCuCreateUserRsp;
    CuCreateUser(cCuCreateUserReq, cCuCreateUserRsp);
    return;
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
void DelUser()
{
    CCuDeleteUserReq cCuDeleteUserReq;
    cCuDeleteUserReq.SetSession(g_strCuInCmuSess);
    cCuDeleteUserReq.SetUserID(g_strCuUserId);

    CCuDeleteUserRsp cCuDeleteUserRsp;
    CuDeleteUser(cCuDeleteUserReq, cCuDeleteUserRsp);

    return;
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
void ModifyUser()
{
    CCuModifyUserReq cCuModifyUserReq;
    cCuModifyUserReq.SetSession(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserID(g_strCuInCmuSess);
    cCuModifyUserReq.SetUsergrpID(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserName(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserPwd(g_strCuInCmuSess);
    cCuModifyUserReq.SetUserType(1);
    cCuModifyUserReq.SetMLoginFlag(1);
    cCuModifyUserReq.SetEnabledFlag(1);
    cCuModifyUserReq.SetDescription(g_strCuInCmuSess);
    time_t tCurTime;
    time(&tCurTime);
    CCMSTime cNameExpireData(tCurTime);
    cCuModifyUserReq.SetNameExpireDate(cNameExpireData.ToString());
    CCMSTime cPwdExpireData(tCurTime+1000);
    cCuModifyUserReq.SetPwdExpireDate(cPwdExpireData.ToString());
    CUserPrivilege cPri;
    cPri.SetPrivilege(CUserPrivilege::en_Alarm_Manage);
    cCuModifyUserReq.SetUserPrivilege(cPri);

    CCuModifyUserRsp cCuModifyUserRsp;
    CuModifyUser(cCuModifyUserReq, cCuModifyUserRsp);

    return;
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
void UserGetDeviceByGroup()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuGetUserDeviceReq cCuGetUserDeviceReq;
        cCuGetUserDeviceReq.SetSession(g_strCuInCmuSess);
        cCuGetUserDeviceReq.SetStartEntryIndex(0);
        cCuGetUserDeviceReq.SetExpectEntryNum(1);
        CUserDeviceInfoQueryCondition cCondition;
        cCondition.SetUserID(g_strCuUserId);
        cCondition.SetDevGrpID(g_strGroupId);
        cCuGetUserDeviceReq.SetUserDeviceInfoQueryCondition(cCondition);

        CCuGetUserDeviceRsp cCuGetUserDeviceRsp;
        CuGetUserDevice( cCuGetUserDeviceReq, cCuGetUserDeviceRsp );
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuGetUserGroupDeviceReq cCuGetUserGroupDeviceReq;
        cCuGetUserGroupDeviceReq.SetSession(g_strCuInCmuSess);
        cCuGetUserGroupDeviceReq.SetStartEntryIndex(0);
        cCuGetUserGroupDeviceReq.SetExpectEntryNum(1);
        CUserGroupDeviceInfoQueryCondition cCondition;
        cCondition.SetCuserGrpID(g_strCuUserId);
        cCondition.SetDevGrpID(g_strGroupId);
        cCuGetUserGroupDeviceReq.SetUserGroupDeviceInfoQueryCondition(cCondition);

        CCuGetUserGroupDeviceRsp cCuGetUserGroupDeviceRsp;
        CuGetUserGroupDevice( cCuGetUserGroupDeviceReq, cCuGetUserGroupDeviceRsp );
    }
    return;
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
void UserAddDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserAddDeviceReq cCuUserAddDeviceReq;
        cCuUserAddDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetUserID(g_strCuUserId);
        cCuUserAddDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);

        CCuUserAddDeviceRsp cCuUserAddDeviceRsp;
        CuUserAddDevice(cCuUserAddDeviceReq, cCuUserAddDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupAddDeviceReq cCuUserGroupAddDeviceReq;
        cCuUserGroupAddDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetDeviceID("123");
        cCuUserGroupAddDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupAddDeviceRsp cCuUserGroupAddDeviceRsp;
        CuUserGroupAddDevice(cCuUserGroupAddDeviceReq, cCuUserGroupAddDeviceRsp);
    }
    return;
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
void UserDelDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserDelDeviceReq cCuUserDelDeviceReq;
        cCuUserDelDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetDeviceID("123");
        cCuUserDelDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);

        CCuUserDelDeviceRsp cCuUserDelDeviceRsp;
        CuUserDelDevice(cCuUserDelDeviceReq, cCuUserDelDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupDelDeviceReq cCuUserGroupDelDeviceReq;
        cCuUserGroupDelDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetDeviceID("123");
        cCuUserGroupDelDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupDelDeviceRsp cCuUserGroupDelDeviceRsp;
        CuUserGroupDelDevice(cCuUserGroupDelDeviceReq, cCuUserGroupDelDeviceRsp);
    }
    return;
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
void UserModifyDevice()
{
    if ( nType == cusdk__TypeUser )
    {
        CCuUserModDeviceReq cCuUserModDeviceReq;
        cCuUserModDeviceReq.SetSession(g_strCuInCmuSess);
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetUserID(g_strCuUserId);
        cCuUserModDeviceReq.GetUserDeviceInfoList().push_back(cUserDevInfo);
        CCuUserModDeviceRsp cCuUserModDeviceRsp;
        CuUserModDevice(cCuUserModDeviceReq, cCuUserModDeviceRsp);
    }
    else if ( nType == cusdk__TypeUserGroup )
    {
        CCuUserGroupModDeviceReq cCuUserGroupModDeviceReq;
        cCuUserGroupModDeviceReq.SetSession(g_strCuInCmuSess);
        CUserGroupDeviceInfo cUserGroupDevInfo;
        cUserGroupDevInfo.SetGroupID(g_strCuUserId);
        cCuUserGroupModDeviceReq.GetUserGroupDeviceInfoList().push_back(cUserGroupDevInfo);

        CCuUserGroupModDeviceRsp cCuUserGroupModDeviceRsp;
        CuUserGroupModDevice(cCuUserGroupModDeviceReq, cCuUserGroupModDeviceRsp);
    }
    return;
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
void GetAuthorizationDevice()
{
    CCuGetAuthorizationDeviceReq cCuGetAuthorizationDeviceReq;
    cCuGetAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    cCuGetAuthorizationDeviceReq.SetStartEntryIndex(0);
    cCuGetAuthorizationDeviceReq.SetExpectEntryNum(1);

    CCuGetAuthorizationDeviceRsp cCuGetAuthorizationDeviceRsp;
    CuGetGetAuthorizationDevice(cCuGetAuthorizationDeviceReq, cCuGetAuthorizationDeviceRsp);

    return;
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
void AddAuthorizationDevice()
{
    CCuAddAuthorizationDeviceReq cCuAddAuthorizationDeviceReq;
    cCuAddAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    CTempAuthorizationInfo cTempAuthorizationInfo;
    cTempAuthorizationInfo.SetDeviceID("123");
    cCuAddAuthorizationDeviceReq.SetTempAuthorizationInfo(cTempAuthorizationInfo);

    CCuAddAuthorizationDeviceRsp cCuAddAuthorizationDeviceRsp;
    CuAddAuthorizationDevice(cCuAddAuthorizationDeviceReq, cCuAddAuthorizationDeviceRsp);
    return;
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
void DelAuthorizationDevice()
{
    CCuDelAuthorizationDeviceReq cCuDelAuthorizationDeviceReq;
    cCuDelAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    CTempAuthorizationInfoKey cTempAuthorizationInfoKey;
    cTempAuthorizationInfoKey.SetDeviceID("123");
    cTempAuthorizationInfoKey.SetUserName(GetNameWithoutDomainFromURI(g_strUserName));
    cTempAuthorizationInfoKey.SetDestinationDomain(GetDomainNameWithoutPreNameFromURI(g_strUserName));
    cCuDelAuthorizationDeviceReq.SetTempAuthorizationInfoKey(cTempAuthorizationInfoKey);

    CCuDelAuthorizationDeviceRsp cCuDelAuthorizationDeviceRsp;
    CuDelAuthorizationDevice(cCuDelAuthorizationDeviceReq, cCuDelAuthorizationDeviceRsp);
    return;
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
void ModifyAuthorizationDevice()
{
    CCuModifyAuthorizationDeviceReq cCuModifyAuthorizationDeviceReq;
    cCuModifyAuthorizationDeviceReq.SetSession(g_strCuInCmuSess);
    cCuModifyAuthorizationDeviceReq.SetDeviceID("123");
    cCuModifyAuthorizationDeviceReq.SetDesc("123");

    CCuModifyAuthorizationDeviceRsp cCuModifyAuthorizationDeviceRsp;
    CuModifyAuthorizationDevice(cCuModifyAuthorizationDeviceReq, cCuModifyAuthorizationDeviceRsp);
    return;
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
void GetDeviceBaseInfo()
{
    CCuGetDeviceBaseInfoReq cCuGetDeviceBaseInfoReq;
    cCuGetDeviceBaseInfoReq.SetSession(g_strCuInCmuSess);
    CDeviceInfoQueryCondition cCondition;
    cCondition.SetDeviceUUID(GetNameWithoutDomainFromURI(g_strDevURI));
    cCuGetDeviceBaseInfoReq.SetDeviceInfoQueryCondition(cCondition);

    CCuGetDeviceBaseInfoRsp cCuGetDeviceBaseInfoRsp;
    CuGetDeviceBaseInfo(cCuGetDeviceBaseInfoReq, cCuGetDeviceBaseInfoRsp);
    return;
}

void ConvertNetAddrFromCuToCui(const cusdk__NetworkAddr &tFrom, TNetAddr &tTo)
{
    tTo.SetNetIp(tFrom.ip);
    tTo.SetNetPort(tFrom.port);
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
void SetupStream()
{
    CCuSetupStreamReq cCuSetupStreamReq;
    cCuSetupStreamReq.SetSession(g_strCuInCmuSess);
    //源是PU
    TChannel tSrcChn;
    tSrcChn.SetDevUri(g_strDevURI);
    tSrcChn.SetChnNO(0);
    cCuSetupStreamReq.SetSrcChn(tSrcChn);
    //目的是CU
    TChannel tCuChn;
    tCuChn.SetDevUri(g_strCuInCmuSess);
    tCuChn.SetChnNO(1);
    cCuSetupStreamReq.SetDstChn(tCuChn);

    int playMode = cusdk__VideoAudioBoth;

    //遍历每个CU的IP
    cusdk__MediaNetInfo MediaNetInfo;
    cusdk__NetworkAddr NetWorkAddr;
    NetWorkAddr.ip = "172.16.64.95";
    NetWorkAddr.port = 12345;
    MediaNetInfo.videoRtpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.videoRtcpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.audioRtpNatInfo.localAddr = NetWorkAddr;
    NetWorkAddr.port += 1;
    MediaNetInfo.audioRtcpNatInfo.localAddr = NetWorkAddr;
    cusdk__MediaNetInfo *pCuMediaNetItem = &MediaNetInfo;
    if ( cusdk__VideoAudioBoth == playMode || cusdk__VideoOnly == playMode )
    {
        TMediaTransDesc tVideoMediaTransDesc;
        tVideoMediaTransDesc.SetMediaType(MEDIA_TYPE_VIDEO);
        //            TMediaTransProto tMediaTransProto;
        //            tMediaTransProto.SetTransport(MEDIA_TRANS_RTP);
        //            tVideoMediaTransDesc.SetMediaTransProto();

        TMediaTransChannel tVideoRtpChan;
        tVideoRtpChan.SetTransChnType(MEDIA_TRANS_CHN_RTP);
        tVideoRtpChan.SetMediaChannelKey(pCuMediaNetItem->videoRtpNatInfo.natData);
        TNetAddr tVideoRtpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtpNatInfo.localAddr, tVideoRtpAddr);
        tVideoRtpChan.GetAddrList().push_back(tVideoRtpAddr);
        if ( (! pCuMediaNetItem->videoRtpNatInfo.natAddr.ip.empty()) &&
            pCuMediaNetItem->videoRtpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtpNatInfo.natAddr, tVideoRtpAddr);
            tVideoRtpChan.GetAddrList().push_back(tVideoRtpAddr);
        }
        tVideoMediaTransDesc.GetMediaTransChnList().push_back(tVideoRtpChan);

        TMediaTransChannel tVideoRtcpChan;
        tVideoRtcpChan.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
        tVideoRtcpChan.SetMediaChannelKey(pCuMediaNetItem->videoRtcpNatInfo.natData);
        TNetAddr tVideoRtcpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtcpNatInfo.localAddr, tVideoRtcpAddr);
        tVideoRtcpChan.GetAddrList().push_back(tVideoRtcpAddr);
        if ( ( ! pCuMediaNetItem->videoRtcpNatInfo.natAddr.ip.empty()) &&
            pCuMediaNetItem->videoRtcpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->videoRtcpNatInfo.natAddr, tVideoRtcpAddr);
            tVideoRtcpChan.GetAddrList().push_back(tVideoRtcpAddr);
        }

        tVideoMediaTransDesc.GetMediaTransChnList().push_back(tVideoRtcpChan);
        cCuSetupStreamReq.AddDstMediaTransDesc(tVideoMediaTransDesc);
    }
    if ( cusdk__VideoAudioBoth == playMode || cusdk__AudioOnly == playMode )
    {
        TMediaTransDesc tAudioMediaTransDesc;
        tAudioMediaTransDesc.SetMediaType(MEDIA_TYPE_AUDIO);

        TMediaTransChannel tAudioRtpChan;
        tAudioRtpChan.SetTransChnType(MEDIA_TRANS_CHN_RTP);
        tAudioRtpChan.SetMediaChannelKey(pCuMediaNetItem->audioRtpNatInfo.natData);
        TNetAddr tAudioRtpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtpNatInfo.localAddr, tAudioRtpAddr);
        tAudioRtpChan.GetAddrList().push_back(tAudioRtpAddr);
        if ( ( ! pCuMediaNetItem->audioRtpNatInfo.natAddr.ip.empty() ) &&
            pCuMediaNetItem->audioRtpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtpNatInfo.natAddr, tAudioRtpAddr);
            tAudioRtpChan.GetAddrList().push_back(tAudioRtpAddr);
        }
        tAudioMediaTransDesc.GetMediaTransChnList().push_back(tAudioRtpChan);

        TMediaTransChannel tAudioRtcpChan;
        tAudioRtcpChan.SetTransChnType(MEDIA_TRANS_CHN_RTCP);
        tAudioRtcpChan.SetMediaChannelKey(pCuMediaNetItem->audioRtcpNatInfo.natData);
        TNetAddr tAudioRtcpAddr;
        ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtcpNatInfo.localAddr, tAudioRtcpAddr);
        tAudioRtcpChan.GetAddrList().push_back(tAudioRtcpAddr);
        if ( ( ! pCuMediaNetItem->audioRtcpNatInfo.natAddr.ip.empty() ) &&
            pCuMediaNetItem->audioRtcpNatInfo.natAddr.port != 0 )
        {
            ConvertNetAddrFromCuToCui(pCuMediaNetItem->audioRtcpNatInfo.natAddr, tAudioRtcpAddr);
            tAudioRtcpChan.GetAddrList().push_back(tAudioRtcpAddr);
        }
        tAudioMediaTransDesc.GetMediaTransChnList().push_back(tAudioRtcpChan);
        cCuSetupStreamReq.AddDstMediaTransDesc(tAudioMediaTransDesc);
    }

    CCuSetupStreamRsp cCuSetupStreamRsp;
    CuSetupStream( cCuSetupStreamReq, cCuSetupStreamRsp );
    if ( 0 == cCuSetupStreamRsp.GetErrorCode() )
    {
        //PlayStream();
        StopStream();
    }
    return;
}

void PlayStream()
{
    CCuPlayStreamReq cCuPlayStreamReq;
    cCuPlayStreamReq.SetSession(g_strCuInCmuSess);
    cCuPlayStreamReq.SetPlayId(1);

    CCuPlayStreamRsp cCuPlayStreamRsp;
    CuPlayStream(cCuPlayStreamReq, cCuPlayStreamRsp);
    return;
}

void StopStream()
{
    CCuStopStreamReq cCuStopStreamReq;
    cCuStopStreamReq.SetSession(g_strCuInCmuSess);
    cCuStopStreamReq.SetPlayId(1);

    CCuStopStreamRsp cCuStopStreamRsp;
    CuStopStream( cCuStopStreamReq, cCuStopStreamRsp );
    return;
}

