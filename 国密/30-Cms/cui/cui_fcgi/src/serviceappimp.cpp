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
#include "serviceappimp.h"
#include "transutility.h"
//#include "cms/cms_const.h"
//#include "cms/cms_errorcode.h"
#ifdef WITH_FASTCGI
#include "system/libfcgi/include/fcgi_stdio.h"
#endif

static SOCKHANDLE g_tSock = INVALID_SOCKET;        //cui.fcgi改为与cuiserver长连接，因此保存连接套接字
static TRecvNode *g_ptRecvNode = NULL;
extern string g_strLogFile;

/*====================================================================
函数名     ：CheckSocketConnected
功能       ：判断cuiserver是否已经断开连接，如果是，则将socket设置为INVALID
            以便重新连接
算法实现    ：由于cui.fcgi采用WebService请求方式驱动，通过额外线程心跳检查方式过于复杂
             所以直接采用recv返回值的方式判断tcp连接有效性
引用全局变量：
输入参数说明：
返回值说明：
====================================================================*/
void CheckSocketConnected(SOCKHANDLE &tSock)
{
    if ( INVALID_SOCKET == tSock )
    {
        return;
    }

    //设置为非阻塞模式, 以便非阻塞地检查tcp连接
    int nRet = SetSockNoBlock(tSock, 1);
    if( nRet == SOCKET_ERROR )
    {
        SockCloseAndClear(tSock);
        return;
    }

    char szTmpMsg[128] = {0};
    nRet = recv(tSock, szTmpMsg, 128, 0);
    if ( 0 == nRet )
    { //连接已经关闭
        SockCloseAndClear(tSock);
        return;
    }
    else if ( SOCKET_ERROR == nRet )
    {
        int nErrorCode = GetSockError();
        if ( !IsErrorcodeWouldBlock(nErrorCode) )
        {
            SockCloseAndClear(tSock);
            return;
        }
    }

    //重新设置为阻塞模式, 以便发送时阻塞
    nRet = SetSockNoBlock(tSock, 0);
    if( nRet == SOCKET_ERROR )
    {
        SockCloseAndClear(tSock);
    }
}

/*====================================================================
函数名     ：OperateImp
功能       ：cui.fcgi在测试过程中发现
            1，采用static模式的fastCGI最高效
            2，soap短链接存在服务端大量TIME_WAIT状态，导致系统拒绝服务
            因此将cui.fcgi改为与cuiserver长连接，一个cui.fcgi只有一个cuiserver连接
算法实现    ：判断全局连接套接字是否有效，如果无效重新连接
             连接失败则直接返回错误码，成功则保存套接字
             发送以及接收过程中，如果失败则关机套接字，等待下次再来请求
引用全局变量：
输入参数说明：
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
template<typename REQ, typename RSP, typename REQBuf, typename RSPBuf>
int OperateImp(REQ &cReq, RSP &cRsp)
{
    cRsp.SetErrorCode(ERR_CUI_CONNECT_CUI_FAIL);
    if ( NULL == g_ptRecvNode )
    {
        g_ptRecvNode = new TRecvNode;
    }
    else
    {
        g_ptRecvNode->Clear();
        CheckSocketConnected(g_tSock);
    }

    if ( INVALID_SOCKET == g_tSock )
    {
        u32 dwTimeout = GetCuiServerTimeout();
        if ( 0 == dwTimeout )
        {
            dwTimeout = MAX_OPR_TIMEOUT;
        }
        std::string strCuiIp = GetCuiServerIpStr();
        g_tSock = ConnectToCuiServer(strCuiIp, GetCuiServerTcpListenPort(), int(dwTimeout * 1000));
        if ( INVALID_SOCKET == g_tSock )
        {
            CuiWriteLog(g_strLogFile.c_str(), "connect CUI[%s-%hu] fail\n", 
                strCuiIp.c_str(), GetCuiServerTcpListenPort());
            cRsp.SetErrorCode(ERR_CUI_CONNECT_CUI_FAIL);
            return FALSE;
        }
		else
		{
			//读写也要设置超时时间
			u32 dwRWTimeout = GetCuiServerReadWriteTimeout();
			SetSendTimeOut(g_tSock, int(dwRWTimeout * 1000));
			SetRecvTimeOut(g_tSock, int(dwRWTimeout * 1000));
		}
    }

    REQBuf cReqBuf;
    cReqBuf.ptMsgBody = &cReq;
    s32 nRet = cReqBuf.ToBuffer(g_ptRecvNode->szRecvBuf, MAX_RECV_MSG_LEN);
    
    nRet = DoSend(g_tSock, g_ptRecvNode->szRecvBuf, (u32)nRet);
    if ( nRet <= 0 )
    {
        CuiWriteLog(g_strLogFile.c_str(), "send msg to cuiserver fail\n");
        cRsp.SetErrorCode(ERR_CUI_DISCONNECT_CUI);
        SockCloseAndClear(g_tSock);
        return FALSE;
    }

    g_ptRecvNode->tSock = INVALID_SOCKET;
    g_ptRecvNode->Clear();
    g_ptRecvNode->tSock = g_tSock;
    //ptRecvNode->nDeadLine = dwTimeout;

    nRet = SockRecv(g_ptRecvNode);
    if ( nRet <= 0 )
    {
        CuiWriteLog(g_strLogFile.c_str(), "recv msg from cuiserver fail\n");
        cRsp.SetErrorCode(ERR_CUI_TCP_TIMEOUT);
        SockCloseAndClear(g_tSock);
        return FALSE;
    }
    RSPBuf cRspBuf;
    cRspBuf.ptMsgBody = &cRsp;
    cRspBuf.FromBuffer(g_ptRecvNode->szRecvBuf, g_ptRecvNode->nMsgTotalLen);

    return TRUE;
}

/*====================================================================
函数名：CuLogin
功能：CU登录的验证
算法实现：
引用全局变量：
输入参数说明：CCuiCuLoginReq &cLoginReq 用户名和密码
             CCuiCuLoginRsp &cloginRsp 登录结果
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuLogin( CCuiCuLoginReq &cLoginReq, CCuiCuLoginRsp &cLoginRsp )
{
    return OperateImp<CCuiCuLoginReq, CCuiCuLoginRsp, CCuiMsgCuLoginReq, CCuiMsgCuLoginRsp>(cLoginReq, cLoginRsp);
}

/*====================================================================
函数名：CuLogout
功能：CU退出登录
算法实现：
引用全局变量：
输入参数说明：CCuLogoutReq &cLogoutReq, CCuLogoutRsp &cLogoutRsp
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuLogout( CCuLogoutReq &cLogoutReq, CCuLogoutRsp &cLogoutRsp )
{
    return OperateImp<CCuLogoutReq, CCuLogoutRsp, CCuiMsgCuLogoutReq, CCuiMsgCuLogoutRsp>(cLogoutReq, cLogoutRsp);
}

/*====================================================================
函数名：CuGetNotify
功能：作用有二：1，CU保活；2，获取平台推送通知消息
算法实现：
引用全局变量：
输入参数说明：CGetNotifyReq &cGetNotifyReq  用户的SessionID
             CGetNotifyRsp &cGetNotifyRsp  返回的推送消息，如果没有推送消息则为空结构体
返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
s32 CuGetNotify( CGetNotifyReq &cGetNotifyReq, CGetNotifyRsp &cGetNotifyRsp )
{
    return OperateImp<CGetNotifyReq, CGetNotifyRsp, CCuiMsgGetNotifyReq, CCuiMsgGetNotifyRsp>(cGetNotifyReq, cGetNotifyRsp);
}

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
s32 CuGetCuiList( CCuGetCuiListReq &cCuGetCuiListReq, CCuGetCuiListRsp &cCuGetCuiListRsp )
{
    return OperateImp<CCuGetCuiListReq, CCuGetCuiListRsp, CCuiMsgCuGetCuiListReq, CCuiMsgCuGetCuiListRsp>(cCuGetCuiListReq, cCuGetCuiListRsp);
}

s32 CuPtzCtrl( CPuPtzCtrlReq &cCuPtzCtrlReq, CPuPtzCtrlRsp &cCuPtzCtrlRsp )
{
    return OperateImp<CPuPtzCtrlReq, CPuPtzCtrlRsp, CCuiMsgPuPtzCtrlReq, CCuiMsgPuPtzCtrlRsp>(cCuPtzCtrlReq, cCuPtzCtrlRsp);
}

s32 CuPtzLock( CPuPtzLockReq &cCuPtzLockReq, CPuPtzLockRsp &cCuPtzLockRsp )
{
    return OperateImp<CPuPtzLockReq, CPuPtzLockRsp, CCuiMsgPuPtzLockReq, CCuiMsgPuPtzLockRsp>(cCuPtzLockReq, cCuPtzLockRsp);
}

s32 CuPtzUnlock( CPuPtzUnlockReq &cCuPtzUnlockReq, CPuPtzUnlockRsp &cCuPtzUnlockRsp )
{
    return OperateImp<CPuPtzUnlockReq, CPuPtzUnlockRsp, CCuiMsgPuPtzUnlockReq, CCuiMsgPuPtzUnlockRsp>(cCuPtzUnlockReq, cCuPtzUnlockRsp);
}

s32 CuPtzLock(CCuPtzLockAddReq &cCuPtzLockAddReq, CCuPtzLockAddRsp &cCuPtzLockAddRsp)
{
    return OperateImp<CCuPtzLockAddReq, CCuPtzLockAddRsp, CCuiMsgPtzLockAddReq, CCuiMsgPtzLockAddRsp>(cCuPtzLockAddReq, cCuPtzLockAddRsp);
}

s32 CuPtzUnlock(CCuPtzLockDelReq &cCuPtzLockDelReq, CCuPtzLockDelRsp &cCuPtzLockDelRsp )
{
    return OperateImp<CCuPtzLockDelReq, CCuPtzLockDelRsp, CCuiMsgPtzLockDelReq, CCuiMsgPtzLockDelRsp>(cCuPtzLockDelReq, cCuPtzLockDelRsp);
}

s32 CuSendTransData( CCuPuTransparentOperateReq &cSendDataReq, CCuPuTransparentOperateRsp &cSendDataRsp )
{
    return OperateImp<CCuPuTransparentOperateReq, CCuPuTransparentOperateRsp, CCuiMsgPuTransparentOperateReq, CCuiMsgPuTransparentOperateRsp>(cSendDataReq, cSendDataRsp);
}

s32 CuQueryTransData(CCuPuTransparentQryReq &cQryReq, CCuPuTransparentQryRsp &cQryRsp)
{
	return OperateImp<CCuPuTransparentQryReq,CCuPuTransparentQryRsp,CCuiMsgPuTransparentQryReq,CCuiMsgPuTransparentQryRsp>(cQryReq,cQryRsp);
}

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
s32 CuGetUserInfo( CCuQueryUserReq &cCuQueryUserReq, CCuQueryUserRsp &cCuQueryUserRsp )
{
    return OperateImp<CCuQueryUserReq, CCuQueryUserRsp, CCuiMsgCuQueryUserReq, CCuiMsgCuQueryUserRsp>(cCuQueryUserReq, cCuQueryUserRsp);
}

s32 CuGetUserGroupInfo( CCuUserGroupQryReq &cCuQueryUserGroupReq, CCuUserGroupQryRsp &cCuQueryUserGroupRsp )
{
    return OperateImp<CCuUserGroupQryReq, CCuUserGroupQryRsp, CCuiMsgCuUserGroupQryReq, CCuiMsgCuUserGroupQryRsp>(cCuQueryUserGroupReq, cCuQueryUserGroupRsp);
}

s32 CuGetUserData( CCuUserCustomDataQryReq &cReq, CCuUserCustomDataQryRsp &cRsp )
{
    return OperateImp<CCuUserCustomDataQryReq, CCuUserCustomDataQryRsp, CCuiMsgCuUserCustomDataQryReq, CCuiMsgCuUserCustomDataQryRsp>(cReq, cRsp);
}

s32 CuSetUserData( CCuUserCustomDataAddReq &cReq, CCuUserCustomDataAddRsp &cRsp )
{
    return OperateImp<CCuUserCustomDataAddReq, CCuUserCustomDataAddRsp, CCuiMsgCuUserCustomDataAddReq, CCuiMsgCuUserCustomDataAddRsp>(cReq, cRsp);
}

s32 CuDelUserData( CCuUserCustomDataDelReq &cReq, CCuUserCustomDataDelRsp &cRsp )
{
    return OperateImp<CCuUserCustomDataDelReq, CCuUserCustomDataDelRsp, CCuiMsgCuUserCustomDataDelReq, CCuiMsgCuUserCustomDataDelRsp>(cReq, cRsp);
}

s32 CuGetUserDevGroup( CCuUserDevGroupQryReq &cCuGetUserDevGroupReq, CCuUserDevGroupQryRsp &cCuGetUserDevGroupRsp )
{
    return OperateImp<CCuUserDevGroupQryReq, CCuUserDevGroupQryRsp, CCuiMsgCuUserDevGroupQryReq, CCuiMsgCuUserDevGroupQryRsp>(cCuGetUserDevGroupReq, cCuGetUserDevGroupRsp);
}

s32 CuGetUserDevFullInfo( CCuUserDevFullInfoQryReq &cCuGetUserDevFullInfoReq, CCuUserDevFullInfoQryRsp &cCuGetUserDevFullInfoRsp )
{
    return OperateImp<CCuUserDevFullInfoQryReq, CCuUserDevFullInfoQryRsp, CCuiMsgCuUserDevFullInfoQryReq, CCuiMsgCuUserDevFullInfoQryRsp>(cCuGetUserDevFullInfoReq, cCuGetUserDevFullInfoRsp);
}

s32 CuSubscribePuStatus( CCuSubscribeNotifyReq &cCuSubscribeNotifyReq, CCuSubscribeNotifyRsp &cCuSubscribeNotifyRsp )
{
    return OperateImp<CCuSubscribeNotifyReq, CCuSubscribeNotifyRsp, CCuiMsgCuSubscribeNotifyReq, CCuiMsgCuSubscribeNotifyRsp>(cCuSubscribeNotifyReq, cCuSubscribeNotifyRsp);
}

s32 CuUnsubscribePuStatus( CCuUnSubscribeNotifyReq &cCuUnSubscribePuStatusReq, CCuUnSubscribeNotifyRsp &cCuUnSubscribePuStatusRsp )
{
    return OperateImp<CCuUnSubscribeNotifyReq, CCuUnSubscribeNotifyRsp, CCuiMsgCuUnSubscribePuStatusReq, CCuiMsgCuUnSubscribePuStatusRsp>(cCuUnSubscribePuStatusReq, cCuUnSubscribePuStatusRsp);
}

s32 CuGetDeviceGroupOfDeviceInfo( CCuQryDevGroupOfDevInfoReq &cCuQryDevGroupOfDevInfoReq, CCuQryDevGroupOfDevInfoRsp &cCuQryDevGroupOfDevInfoRsp )
{
    return OperateImp<CCuQryDevGroupOfDevInfoReq, CCuQryDevGroupOfDevInfoRsp, CCuiMsgCuQryDevGroupOfDevInfoReq, CCuiMsgCuQryDevGroupOfDevInfoRsp>(cCuQryDevGroupOfDevInfoReq, cCuQryDevGroupOfDevInfoRsp);
}

s32 CuCreateDeviceGroup( CCuCreateDeviceGroupReq &cCuCreateDeviceGroupReq, CCuCreateDeviceGroupRsp &cCuCreateDeviceGroupRsp )
{
    return OperateImp<CCuCreateDeviceGroupReq, CCuCreateDeviceGroupRsp, CCuiMsgCuCreateDeviceGroupReq, CCuiMsgCuCreateDeviceGroupRsp>(cCuCreateDeviceGroupReq, cCuCreateDeviceGroupRsp);
}


s32 CuDeleteDeviceGroup( CCuDeleteDeviceGroupReq &cCuDeleteDeviceGroupReq, CCuDeleteDeviceGroupRsp &cCuDeleteDeviceGroupRsp )
{
    return OperateImp<CCuDeleteDeviceGroupReq, CCuDeleteDeviceGroupRsp, CCuiMsgCuDeleteDeviceGroupReq, CCuiMsgCuDeleteDeviceGroupRsp>(cCuDeleteDeviceGroupReq, cCuDeleteDeviceGroupRsp);
}

s32 CuModifyDeviceGroup( CCuModifyDeviceGroupReq &cCuModifyDeviceGroupReq, CCuModifyDeviceGroupRsp &cCuModifyDeviceGroupRsp )
{
    return OperateImp<CCuModifyDeviceGroupReq, CCuModifyDeviceGroupRsp, CCuiMsgCuModifyDeviceGroupReq, CCuiMsgCuModifyDeviceGroupRsp>(cCuModifyDeviceGroupReq, cCuModifyDeviceGroupRsp);
}

s32 CuDeviceGroupAddDevice( CCuDeviceGroupAddDeviceReq &cCuDeviceGroupAddDeviceReq, CCuDeviceGroupAddDeviceRsp &cCuDeviceGroupAddDeviceRsp )
{
    return OperateImp<CCuDeviceGroupAddDeviceReq, CCuDeviceGroupAddDeviceRsp, CCuiMsgCuDeviceGroupAddDeviceReq, CCuiMsgCuDeviceGroupAddDeviceRsp>(cCuDeviceGroupAddDeviceReq, cCuDeviceGroupAddDeviceRsp);
}

s32 CuDeviceGroupDelDevice( CCuDeviceGroupDelDeviceReq &cCuDeviceGroupDelDeviceReq, CCuDeviceGroupDelDeviceRsp &cCuDeviceGroupDelDeviceRsp )
{
    return OperateImp<CCuDeviceGroupDelDeviceReq, CCuDeviceGroupDelDeviceRsp, CCuiMsgCuDeviceGroupDelDeviceReq, CCuiMsgCuDeviceGroupDelDeviceRsp>(cCuDeviceGroupDelDeviceReq, cCuDeviceGroupDelDeviceRsp);
}

s32 CuDeviceGroupAddGroup(CCuDevTreeDevGrpAssignReq &cCuDevTreeDevGrpAssignReq, CCuDevTreeDevGrpAssignRsp &cCuDevTreeDevGrpAssignRsp)
{
	return OperateImp<CCuDevTreeDevGrpAssignReq, CCuDevTreeDevGrpAssignRsp, CCuiMsgCuDevTreeDevGrpAssignReq, CCuiMsgCuDevTreeDevGrpAssignRsp>(cCuDevTreeDevGrpAssignReq, cCuDevTreeDevGrpAssignRsp);
}

s32 CuCreateUserGroup( CCuCreateUserGroupReq &cCuCreateUserGroupReq, CCuCreateUserGroupRsp &cCuCreateUserGroupRsp )
{
    return OperateImp<CCuCreateUserGroupReq, CCuCreateUserGroupRsp, CCuiMsgCuCreateUserGroupReq, CCuiMsgCuCreateUserGroupRsp>(cCuCreateUserGroupReq, cCuCreateUserGroupRsp);
}

s32 CuDeleteUserGroup( CCuDeleteUserGroupReq &cCuDeleteUserGroupReq, CCuDeleteUserGroupRsp &cCuDeleteUserGroupRsp )
{
    return OperateImp<CCuDeleteUserGroupReq, CCuDeleteUserGroupRsp, CCuiMsgCuDeleteUserGroupReq, CCuiMsgCuDeleteUserGroupRsp>(cCuDeleteUserGroupReq, cCuDeleteUserGroupRsp);
}

s32 CuModifyUserGroup( CCuModifyUserGroupReq &cCuModifyUserGroupReq, CCuModifyUserGroupRsp &cCuModifyUserGroupRsp )
{
    return OperateImp<CCuModifyUserGroupReq, CCuModifyUserGroupRsp, CCuiMsgCuModifyUserGroupReq, CCuiMsgCuModifyUserGroupRsp>(cCuModifyUserGroupReq, cCuModifyUserGroupRsp);
}

s32 CuCreateUser( CCuCreateUserReq &cCuCreateUserReq, CCuCreateUserRsp &cCuCreateUserRsp )
{
    return OperateImp<CCuCreateUserReq, CCuCreateUserRsp, CCuiMsgCuCreateUserReq, CCuiMsgCuCreateUserRsp>(cCuCreateUserReq, cCuCreateUserRsp);
}

s32 CuDeleteUser( CCuDeleteUserReq &cCuDeleteUserReq, CCuDeleteUserRsp &cCuDeleteUserRsp )
{
    return OperateImp<CCuDeleteUserReq, CCuDeleteUserRsp, CCuiMsgCuDeleteUserReq, CCuiMsgCuDeleteUserRsp>(cCuDeleteUserReq, cCuDeleteUserRsp);
}

s32 CuModifyUser( CCuModifyUserReq &cCuModifyUserReq, CCuModifyUserRsp &cCuModifyUserRsp )
{
    return OperateImp<CCuModifyUserReq, CCuModifyUserRsp, CCuiMsgCuModifyUserReq, CCuiMsgCuModifyUserRsp>(cCuModifyUserReq, cCuModifyUserRsp);
}

s32 CuChangePassword(CCuChangePasswordReq &cCuChangePasswordReq, CCuChangePasswordRsp &cCuChangePasswordRsp)
{
    return OperateImp<CCuChangePasswordReq, CCuChangePasswordRsp, CCuiMsgCuChangePasswordReq, CCuiMsgCuModifyUserRsp>(cCuChangePasswordReq, cCuChangePasswordRsp);
}

s32 CuSendKeyFrame(CCuKeyFrameReq &cCuKeyFrameReq, CCuKeyFrameRsp &cCuKeyFrameRsp)
{
    return OperateImp<CCuKeyFrameReq, CCuKeyFrameRsp, CCuiMsgCuKeyFrameReq, CCuiMsgCuKeyFrameRsp>(cCuKeyFrameReq, cCuKeyFrameRsp);
}

s32 CuGetUserDevice( CCuGetUserDeviceReq &cCuGetUserDeviceReq, CCuGetUserDeviceRsp &cCuGetUserDeviceRsp )
{
    return OperateImp<CCuGetUserDeviceReq, CCuGetUserDeviceRsp, CCuiMsgCuGetUserDeviceReq, CCuiMsgCuGetUserDeviceRsp>(cCuGetUserDeviceReq, cCuGetUserDeviceRsp);
}

s32 CuGetUserGroupDevice( CCuGetUserGroupDeviceReq &cCuGetUserGroupDeviceReq, CCuGetUserGroupDeviceRsp &cCuGetUserGroupDeviceRsp )
{
    return OperateImp<CCuGetUserGroupDeviceReq, CCuGetUserGroupDeviceRsp, CCuiMsgCuGetUserGroupDeviceReq, CCuiMsgCuGetUserGroupDeviceRsp>(cCuGetUserGroupDeviceReq, cCuGetUserGroupDeviceRsp);
}

s32 CuUserAddDevice( CCuUserAddDeviceReq &cCuUserAddDeviceReq, CCuUserAddDeviceRsp &cCuUserAddDeviceRsp )
{
    return OperateImp<CCuUserAddDeviceReq, CCuUserAddDeviceRsp, CCuiMsgCuUserAddDeviceReq, CCuiMsgCuUserAddDeviceRsp>(cCuUserAddDeviceReq, cCuUserAddDeviceRsp);
}

s32 CuUserDelDevice( CCuUserDelDeviceReq &cCuUserDelDeviceReq, CCuUserDelDeviceRsp &cCuUserDelDeviceRsp )
{
    return OperateImp<CCuUserDelDeviceReq, CCuUserDelDeviceRsp, CCuiMsgCuUserDelDeviceReq, CCuiMsgCuUserDelDeviceRsp>(cCuUserDelDeviceReq, cCuUserDelDeviceRsp);
}

s32 CuUserModDevice( CCuUserModDeviceReq &cCuUserModDeviceReq, CCuUserModDeviceRsp &cCuUserModDeviceRsp )
{
    return OperateImp<CCuUserModDeviceReq, CCuUserModDeviceRsp, CCuiMsgCuUserModDeviceReq, CCuiMsgCuUserModDeviceRsp>(cCuUserModDeviceReq, cCuUserModDeviceRsp);
}

s32 CuUserGroupAddDevice( CCuUserGroupAddDeviceReq &cCuUserGroupAddDeviceReq, CCuUserGroupAddDeviceRsp &cCuUserGroupAddDeviceRsp )
{
    return OperateImp<CCuUserGroupAddDeviceReq, CCuUserGroupAddDeviceRsp, CCuiMsgCuUserGroupAddDeviceReq, CCuiMsgCuUserGroupAddDeviceRsp>(cCuUserGroupAddDeviceReq, cCuUserGroupAddDeviceRsp);
}

s32 CuUserGroupDelDevice( CCuUserGroupDelDeviceReq &cCuUserGroupDelDeviceReq, CCuUserGroupDelDeviceRsp &cCuUserGroupDelDeviceRsp )
{
    return OperateImp<CCuUserGroupDelDeviceReq, CCuUserGroupDelDeviceRsp, CCuiMsgCuUserGroupDelDeviceReq, CCuiMsgCuUserGroupDelDeviceRsp>(cCuUserGroupDelDeviceReq, cCuUserGroupDelDeviceRsp);
}

s32 CuUserGroupModDevice( CCuUserGroupModDeviceReq &cCuUserGroupModDeviceReq, CCuUserGroupModDeviceRsp &cCuUserGroupModDeviceRsp )
{
    return OperateImp<CCuUserGroupModDeviceReq, CCuUserGroupModDeviceRsp, CCuiMsgCuUserGroupModDeviceReq, CCuiMsgCuUserGroupModDeviceRsp>(cCuUserGroupModDeviceReq, cCuUserGroupModDeviceRsp);
}

s32 CuGetGetAuthorizationDevice( CCuGetAuthorizationDeviceReq &cCuGetAuthorizationDeviceReq, CCuGetAuthorizationDeviceRsp &cCuGetAuthorizationDeviceRsp )
{
    return OperateImp<CCuGetAuthorizationDeviceReq, CCuGetAuthorizationDeviceRsp, CCuiMsgCuGetAuthorizationDeviceReq, CCuiMsgCuGetAuthorizationDeviceRsp>(cCuGetAuthorizationDeviceReq, cCuGetAuthorizationDeviceRsp);
}

s32 CuAddAuthorizationDevice( CCuAddAuthorizationDeviceReq &cCuAddAuthorizationDeviceReq, CCuAddAuthorizationDeviceRsp &cCuAddAuthorizationDeviceRsp )
{
    return OperateImp<CCuAddAuthorizationDeviceReq, CCuAddAuthorizationDeviceRsp, CCuiMsgCuAddAuthorizationDeviceReq, CCuiMsgCuAddAuthorizationDeviceRsp>(cCuAddAuthorizationDeviceReq, cCuAddAuthorizationDeviceRsp);
}

s32 CuDelAuthorizationDevice( CCuDelAuthorizationDeviceReq &cCuDelAuthorizationDeviceReq, CCuDelAuthorizationDeviceRsp &cCuDelAuthorizationDeviceRsp )
{
    return OperateImp<CCuDelAuthorizationDeviceReq, CCuDelAuthorizationDeviceRsp, CCuiMsgCuDelAuthorizationDeviceReq, CCuiMsgCuDelAuthorizationDeviceRsp>(cCuDelAuthorizationDeviceReq, cCuDelAuthorizationDeviceRsp);
}

s32 CuModifyAuthorizationDevice( CCuModifyAuthorizationDeviceReq &cCuModifyAuthorizationDeviceReq, CCuModifyAuthorizationDeviceRsp &cCuModifyAuthorizationDeviceRsp )
{
    return OperateImp<CCuModifyAuthorizationDeviceReq, CCuModifyAuthorizationDeviceRsp, CCuiMsgCuModifyAuthorizationDeviceReq, CCuiMsgCuModifyAuthorizationDeviceRsp>(cCuModifyAuthorizationDeviceReq, cCuModifyAuthorizationDeviceRsp);
}

s32 CuGetDeviceBaseInfo( CCuGetDeviceBaseInfoReq &cCuGetDeviceBaseInfoReq, CCuGetDeviceBaseInfoRsp &cCuGetDeviceBaseInfoRsp )
{
    return OperateImp<CCuGetDeviceBaseInfoReq, CCuGetDeviceBaseInfoRsp, CCuiMsgCuGetDeviceBaseInfoReq, CCuiMsgCuGetDeviceBaseInfoRsp>(cCuGetDeviceBaseInfoReq, cCuGetDeviceBaseInfoRsp);
}

s32 CuSetupStream( CCuSetupStreamReq &cCuSetupStreamReq, CCuSetupStreamRsp &cCuSetupStreamRsp )
{
    return OperateImp<CCuSetupStreamReq, CCuSetupStreamRsp, CCuiMsgCuSetupStreamReq, CCuiMsgCuSetupStreamRsp>(cCuSetupStreamReq, cCuSetupStreamRsp);
}

s32 CuPlayStream( CCuPlayStreamReq &cCuPlayStreamReq, CCuPlayStreamRsp &cCuPlayStreamRsp )
{
    return OperateImp<CCuPlayStreamReq, CCuPlayStreamRsp, CCuiMsgCuPlayStreamReq, CCuiMsgCuPlayStreamRsp>(cCuPlayStreamReq, cCuPlayStreamRsp);
}

s32 CuStopStream( CCuStopStreamReq &cCuStopStreamReq, CCuStopInviteRsp &cCuStopStreamRsp )
{
    return OperateImp<CCuStopStreamReq, CCuStopInviteRsp, CCuiMsgCuStopStreamReq, CCuiMsgCuStopStreamRsp>(cCuStopStreamReq, cCuStopStreamRsp);
}

s32 CuGetRCSSysParam(CCuRcsGetSysParamReq &cCuRcsGetSysParamReq, CCuRcsGetSysParamRsp &cCuRcsGetSysParamRsp)
{
    return OperateImp<CCuRcsGetSysParamReq, CCuRcsGetSysParamRsp, CCuiMsgCuRcsGetSysParamReq, CCuiMsgCuRcsGetSysParamRsp>(cCuRcsGetSysParamReq, cCuRcsGetSysParamRsp);
}

s32 CuSetRCSSysParam(CCuRcsSetSysParamReq &cCuRcsSetSysParamReq, CCuRcsSetSysParamRsp &cCuRcsSetSysParamRsp)
{
    return OperateImp<CCuRcsSetSysParamReq, CCuRcsSetSysParamRsp, CCuiMsgCuRcsSetSysParamReq, CCuiMsgCuRcsSetSysParamRsp>(cCuRcsSetSysParamReq, cCuRcsSetSysParamRsp);
}

s32 CuGetNruList(CCuRcsQueryNruReq &cCuRcsQueryNruReq, CCuRcsQueryNruRsp &cCuRcsQueryNruRsp)
{
    return OperateImp<CCuRcsQueryNruReq, CCuRcsQueryNruRsp, CCuiMsgCuRcsQueryNruReq, CCuiMsgCuRcsQueryNruRsp>(cCuRcsQueryNruReq, cCuRcsQueryNruRsp);
}

s32 CuGetPuChnBinding(CCuRcsQueryPuChnBindingReq &cCuRcsQueryPuChnBindingReq, CCuRcsQueryPuChnBindingRsp &cCuRcsQueryPuChnBindingRsp)
{
    return OperateImp<CCuRcsQueryPuChnBindingReq, CCuRcsQueryPuChnBindingRsp, CCuiMsgCuRcsQueryPuChnBindingReq, CCuiMsgCuRcsQueryPuChnBindingRsp>(cCuRcsQueryPuChnBindingReq, cCuRcsQueryPuChnBindingRsp);
}

s32 CuBindPu(CCuRcsBindingPuChnReq &cCuRcsBindingPuChnReq, CCuRcsBindingPuChnRsp &cCuRcsBindingPuChnRsp)
{
    return OperateImp<CCuRcsBindingPuChnReq, CCuRcsBindingPuChnRsp, CCuiMsgRcsBindingPuChnReq, CCuiMsgRcsBindingPuChnRsp>(cCuRcsBindingPuChnReq,cCuRcsBindingPuChnRsp);
}

s32 CuUnBindPu(CCuRcsUnbindingPuChnReq &cCuRcsUnbindingPuChnReq, CCuRcsUnbindingPuChnRsp &cCuRcsUnbindingPuChnRsp)
{
    return OperateImp<CCuRcsUnbindingPuChnReq, CCuRcsUnbindingPuChnRsp, CCuiMsgRcsUnbindingPuChnReq, CCuiMsgRcsUnbindingPuChnRsp>(cCuRcsUnbindingPuChnReq, cCuRcsUnbindingPuChnRsp);
}

s32 CuGetPlatformRecordDays(CCuRcsQueryRecordDaysReq &cCuRcsQueryRecordDaysReq, CCuRcsQueryRecordDaysRsp &cCuRcsQueryRecordDaysRsp)
{
    return OperateImp<CCuRcsQueryRecordDaysReq, CCuRcsQueryRecordDaysRsp, CCuiMsgRcsQueryRecordDaysReq, CCuiMsgRcsQueryRecordDaysRsp>(cCuRcsQueryRecordDaysReq, cCuRcsQueryRecordDaysRsp);
}

s32 CuGetPuRecordDays(CCuPuRecDaysQryReq &cCuiPuRecDaysQryReq, CCuPuRecDaysQryRsp &cCuiPuRecDaysQryRsp)
{
    return OperateImp<CCuPuRecDaysQryReq, CCuPuRecDaysQryRsp, CCuiMsgPuRecDaysQryReq, CCuiMsgPuRecDaysQryRsp>(cCuiPuRecDaysQryReq, cCuiPuRecDaysQryRsp);
}

s32 CuGetPlatformRecord(CCuRcsQueryRecordReq &cCuRcsQueryRecordReq, CCuRcsQueryRecordRsp &cCuRcsQueryRecordRsp)
{
    return OperateImp<CCuRcsQueryRecordReq, CCuRcsQueryRecordRsp, CCuiMsgRcsQueryRecordReq, CCuiMsgRcsQueryRecordRsp>(cCuRcsQueryRecordReq, cCuRcsQueryRecordRsp);
}

s32 CuStartPlatformRecord(CCuRcsStartRecordTaskReq &cCuRcsStartRecordTaskReq, CCuRcsSetRecordTaskRsp &cCuRcsStartRecordTaskRsp)
{
    return OperateImp<CCuRcsStartRecordTaskReq, CCuRcsSetRecordTaskRsp, CCuiMsgRcsSetRecordTaskReq, CCuiMsgRcsSetRecordTaskRsp>(cCuRcsStartRecordTaskReq, cCuRcsStartRecordTaskRsp);
}

s32 CuStopPlatformRecord(CCuRcsStopRecordTaskReq &cCuRcsStopRecordTaskReq, CCuRcsStopRecordTaskRsp &cCuRcsStopRecordTaskRsp)
{
    return OperateImp<CCuRcsStopRecordTaskReq, CCuRcsStopRecordTaskRsp, CCuiMsgRcsSetRecordTaskReq, CCuiMsgRcsSetRecordTaskRsp>(cCuRcsStopRecordTaskReq, cCuRcsStopRecordTaskRsp);
}

s32 CuSetupPlayPlatformRecord(CCuSetupPlayPlatformRecordReq &cCuSetupPlayPlatformRecordReq, CCuSetupPlayPlatformRecordRsp &cCuSetupPlayPlatformRecordRsp)
{
    return OperateImp<CCuSetupPlayPlatformRecordReq, CCuSetupPlayPlatformRecordRsp, CCuiMsgSetupPlayPlatformRecordReq, CCuiMsgSetupPlayPlatformRecordRsp>(cCuSetupPlayPlatformRecordReq, cCuSetupPlayPlatformRecordRsp);
}

s32 CuPlayPlatformRecord(CCuPlayPlatformRecordReq &cCuPlayPlatformRecordReq, CCuPlayPlatformRecordRsp &cCuPlayPlatformRecordRsp)
{
    return OperateImp<CCuPlayPlatformRecordReq, CCuPlayPlatformRecordRsp, CCuiMsgPlayPlatformRecordReq, CCuiMsgPlayPlatformRecordRsp>(cCuPlayPlatformRecordReq, cCuPlayPlatformRecordRsp);
}

s32 CuVcrCtrl(CCuVcrCtrlReq &cCuVcrCtrlReq, CCuVcrCtrlRsp &cCuVcrCtrlRsp)
{
    return OperateImp<CCuVcrCtrlReq, CCuVcrCtrlRsp, CCuiMsgVcrCtrlReq, CCuiMsgVcrCtrlRsp>(cCuVcrCtrlReq, cCuVcrCtrlRsp);
}

s32 CuGetCurRecordPlayPosition(CCuPlaybackProgressQueryReq &cCuReplayProgressQueryReq, CCuPlaybackProgressQueryRsp &cCuReplayProgressQueryRsp)
{
    return OperateImp<CCuPlaybackProgressQueryReq, CCuPlaybackProgressQueryRsp, CCuiMsgPlaybackProgressQueryReq, CCuiMsgPlaybackProgressQueryRsp>(cCuReplayProgressQueryReq, cCuReplayProgressQueryRsp);
}

s32 CuStopPlayPlatformRecord(CCuStopPlatformRecordReq &cCuStopPlatformRecordReq, CCuStopPlatformRecordRsp &cCuStopPlatformRecordRsp)
{
    return OperateImp<CCuStopPlatformRecordReq, CCuStopPlatformRecordRsp, CCuiMsgStopPlatformRecordReq, CCuiMsgStopPlatformRecordRsp>(cCuStopPlatformRecordReq, cCuStopPlatformRecordRsp);
}

s32 CuSetupDownloadPlatformRecord(CCuDownloadPlatformRecordReq &cCuDownloadPlatformRecordReq, CCuDownloadPlatformRecordRsp &cCuDownloadPlatformRecordRsp )
{
    return OperateImp<CCuDownloadPlatformRecordReq, CCuDownloadPlatformRecordRsp, CCuiMsgDownloadPlatformRecordReq, CCuiMsgDownloadPlatformRecordRsp>(cCuDownloadPlatformRecordReq, cCuDownloadPlatformRecordRsp);
}

s32 CuStartDownloadPlatformRecord(CCuStartDownloadPlatformRecordReq &cCuStartDownloadPlatformRecordReq, CCuStartDownloadPlatformRecordRsp &cCuStartDownloadPlatformRecordRsp)
{
    return OperateImp<CCuStartDownloadPlatformRecordReq, CCuStartDownloadPlatformRecordRsp, CCuiMsgStartDownloadPlatformRecordReq, CCuiMsgStartDownloadPlatformRecordRsp>(cCuStartDownloadPlatformRecordReq, cCuStartDownloadPlatformRecordRsp);
}

s32 CuStopDownloadPlatformRecord(CCuStopDownloadPlatformRecordReq &cCuStopDownloadPlatformRecordReq, CCuStopDownloadPlatformRecordRsp &cCuStopDownloadPlatformRecordRsp)
{
    return OperateImp<CCuStopDownloadPlatformRecordReq, CCuStopDownloadPlatformRecordRsp, CCuiMsgStopDownloadPlatformRecordReq, CCuiMsgStopDownloadPlatformRecordRsp>(cCuStopDownloadPlatformRecordReq, cCuStopDownloadPlatformRecordRsp);
}

s32 CuGetDomainList(CCuDomainRelationQryReq &cCuDomainRelationQryReq, CCuDomainRelationQryRsp& cCuDomainRelationQryRsp)
{
    return OperateImp<CCuDomainRelationQryReq, CCuDomainRelationQryRsp, CCuiMsgDomainRelationQryReq, CCuiMsgDomainRelationQryRsp>(cCuDomainRelationQryReq, cCuDomainRelationQryRsp);
}

s32 CuGetRecordPlan(CCuRcsQueryRecordParamReq &cCuRcsQueryRecordParamReq, CCuRcsQueryRecordParamRsp &cCuRcsQueryRecordParamRsp)
{
    return OperateImp<CCuRcsQueryRecordParamReq, CCuRcsQueryRecordParamRsp, CCuiMsgRcsQueryRecordParamReq, CCuiMsgRcsQueryRecordParamRsp>(cCuRcsQueryRecordParamReq, cCuRcsQueryRecordParamRsp);
}

s32 CuSetRecordPlan(CCuRcsSetRecordParamReq &cCuRcsSetRecordParamReq, CCuRcsSetRecordParamRsp &cCuRcsSetRecordParamRsp)
{
    return OperateImp<CCuRcsSetRecordParamReq, CCuRcsSetRecordParamRsp, CCuiMsgRcsSetRecordParamReq, CCuiMsgRcsSetRecordParamRsp>(cCuRcsSetRecordParamReq, cCuRcsSetRecordParamRsp);
}

s32 CuQueryGpsTrece(CCuGpsTraceQryReq &cCuGpsTraceQryReq, CCuGpsTraceQryRsp &cCuGpsTraceQryRsp)
{
	return OperateImp<CCuGpsTraceQryReq,CCuGpsTraceQryRsp,CCuiMsgGpsTraceQryReq,CCuiMsgGpsTraceQryRsp>(cCuGpsTraceQryReq,cCuGpsTraceQryRsp);
}

s32 CuStartPuRecord(CCuPuRecordStartReq &cCuPuRecordStartReq, CCuPuRecordStartRsp &cCuPuRecordStartRsp)
{
	return OperateImp<CCuPuRecordStartReq,CCuPuRecordStartRsp,CCuiMsgPuRecordStartReq,CCuiMsgPuRecordStartRsp>(cCuPuRecordStartReq,cCuPuRecordStartRsp);
}

s32 CuStopPuRecord(CCuPuRecordStopReq &cCuPuRecordStopReq, CCuPuRecordStopRsp &cCuPuRecordStopRsp)
{
	return OperateImp<CCuPuRecordStopReq,CCuPuRecordStopRsp,CCuiMsgPuRecordStopReq,CCuiMsgPuRecordStopRsp>(cCuPuRecordStopReq,cCuPuRecordStopRsp);
}

s32 CuGetAllTVWall(CCuTvwallQryReq &cCuTvwallQryReq, CCuTvwallQryRsp &cCuTvwallQryRsp)
{
    return OperateImp<CCuTvwallQryReq, CCuTvwallQryRsp, CCuiMsgTvwallQryReq, CCuiMsgTvwallQryRsp>(cCuTvwallQryReq, cCuTvwallQryRsp);
}

s32 CCuCreatTVWall(CCuTvwallAddReq &cCuTvwallAddReq, CCuTvwallAddRsp &cCuTvwallAddRsp)
{
    return OperateImp<CCuTvwallAddReq, CCuTvwallAddRsp, CCuiMsgTvwallAddReq, CCuiMsgTvwallAddRsp>(cCuTvwallAddReq, cCuTvwallAddRsp);
}

s32 CuDelTVWall(CCuTvwallDelReq &cCuTvwallDelReq, CCuTvwallDelRsp &cCuTvwallDelRsp)
{
    return OperateImp<CCuTvwallDelReq, CCuTvwallDelRsp, CCuiMsgTvwallDelReq, CCuiMsgTvwallDelRsp>(cCuTvwallDelReq, cCuTvwallDelRsp);
}

s32 CuPlayRecordOnTVWall(CCuRecordOnTvReq &cCuRecOnTvReq, CCuRecordOnTvRsp &cCuRecOnTvRsp)
{
    return OperateImp<CCuRecordOnTvReq, CCuRecordOnTvRsp, CCuiMsgRecordOnTvReq, CCuiMsgRecordOnTvRsp>(cCuRecOnTvReq, cCuRecOnTvRsp);
}

s32 CuStopPlayRecordOnTVWall(CCuRecordOffTvReq &cCuRecOffTvReq, CCuRecordOffTvRsp &cCuRecOffTvRsp)
{
    return OperateImp<CCuRecordOffTvReq, CCuRecordOffTvRsp, CCuiMsgRecordOffTvReq, CCuiMsgRecordOffTvRsp>(cCuRecOffTvReq, cCuRecOffTvRsp);
}

s32 CuModifyTVWall(CCuTvwallModReq &cCuTvwallModReq, CCuTvwallModRsp &cCuTvwallModRsp)
{
    return OperateImp<CCuTvwallModReq, CCuTvwallModRsp, CCuiMsgTvwallModReq, CCuiMsgTvwallModRsp>(cCuTvwallModReq, cCuTvwallModRsp);
}

s32 CuTVWallStartPlay(CCuRealMediaOnTvReq &cCuRealMediaOnTvReq, CCuRealMediaOnTvRsp &cCuRealMediaOnTvRsp)
{
    return OperateImp<CCuRealMediaOnTvReq, CCuRealMediaOnTvRsp, CCuiMsgRealMediaOnTvReq, CCuiMsgRealMediaOnTvRsp>(cCuRealMediaOnTvReq, cCuRealMediaOnTvRsp);
}

s32 CuTVWallStopPlay(CCuRealMediaOffTvReq &cCuRealMediaOffTvReq, CCuRealMediaOffTvRsp &cCuRealMediaOffTvRsp)
{
    return OperateImp<CCuRealMediaOffTvReq, CCuRealMediaOffTvRsp, CCuiMsgRealMediaOffTvReq, CCuiMsgRealMediaOffTvRsp>(cCuRealMediaOffTvReq, cCuRealMediaOffTvRsp);
}

s32 CuTVWallWindowPollContrl(CCuTvPollOperateReq &cCuTvPollOperateReq, CCuTvPollOperateRsp &cCuTvPollOperateRsp)
{
    return OperateImp<CCuTvPollOperateReq, CCuTvPollOperateRsp, CCuiMsgTvPollOperateReq, CCuiMsgTvPollOperateRsp>(cCuTvPollOperateReq, cCuTvPollOperateRsp);
}

s32 CuTVWallSaveScheme(CCuTvWallSchemeSaveReq &cCuTvWallSchemeSaveReq, CCuTvWallSchemeSaveRsp &cCuTvWallSchemeSaveRsp)
{
    return OperateImp<CCuTvWallSchemeSaveReq,CCuTvWallSchemeSaveRsp, CCuiMsgTvWallSchemeSaveReq, CCuiMsgTvWallSchemeSaveRsp>(cCuTvWallSchemeSaveReq, cCuTvWallSchemeSaveRsp);
}

s32 CuTVWallDelScheme(CCuTvWallSchemeDelReq &cCuTvWallSchemeDelReq, CCuTvWallSchemeDelRsp &cCuTvWallSchemeDelRsp)
{
    return OperateImp<CCuTvWallSchemeDelReq, CCuTvWallSchemeDelRsp, CCuiMsgTvWallSchemeDelReq, CCuiMsgTvWallSchemeDelRsp>(cCuTvWallSchemeDelReq, cCuTvWallSchemeDelRsp);
}

s32 CuTVWallLoadScheme(CCuTvWallSchemeLoadReq &cCuTvWallSchemeLoadReq, CCuTvWallSchemeLoadRsp &cCuTvWallSchemeLoadRsp)
{
    return OperateImp<CCuTvWallSchemeLoadReq, CCuTvWallSchemeLoadRsp, CCuiMsgTvWallSchemeLoadReq, CCuiMsgTvWallSchemeLoadRsp>(cCuTvWallSchemeLoadReq, cCuTvWallSchemeLoadRsp);
}

s32 CuTVWallQueryScheme(CCuTvWallSchemeQueryReq &cCuTvWallSchemeQueryReq, CCuTvWallSchemeQueryRsp &cCuTvWallSchemeQueryRsp)
{
    return OperateImp<CCuTvWallSchemeQueryReq, CCuTvWallSchemeQueryRsp, CCuiMsgTvWallSchemeQueryReq, CCuiMsgTvWallSchemeQueryRsp>(cCuTvWallSchemeQueryReq, cCuTvWallSchemeQueryRsp);
}

s32 CuTVWallSetSchemePoll(CCuTvWallSchemeGroupSetReq &cCuTvWallSchemeGroupSetReq, CCuTvWallSchemeGroupSetRsp &cCuTvWallSchemeGroupSetRsp)
{
    return OperateImp<CCuTvWallSchemeGroupSetReq, CCuTvWallSchemeGroupSetRsp, CCuiMsgTvWallSchemeGroupSetReq, CCuiMsgTvWallSchemeGroupSetRsp>(cCuTvWallSchemeGroupSetReq, cCuTvWallSchemeGroupSetRsp);
}

s32 CuTVWallGetSchemePoll(CCuTvWallSchemeGroupGetReq &cCuTvWallSchemeGroupGetReq, CCuTvWallSchemeGroupGetRsp &cCuTvWallSchemeGroupGetRsp)
{
    return OperateImp<CCuTvWallSchemeGroupGetReq, CCuTvWallSchemeGroupGetRsp, CCuiMsgTvWallSchemeGroupGetReq, CCuiMsgTvWallSchemeGroupGetRsp>(cCuTvWallSchemeGroupGetReq, cCuTvWallSchemeGroupGetRsp);
}

s32 CuTVWallSchemePollControl(CCuTvWallSchemePollOperateReq &cCuTvWallSchemePollOperateReq, CCuTvWallSchemePollOperateRsp &cCuTvWallSchemePollOperateRsp)
{
    return OperateImp<CCuTvWallSchemePollOperateReq, CCuTvWallSchemePollOperateRsp, CCuiMsgTvWallSchemePollOperateReq, CCuiMsgTvWallSchemePollOperateRsp>(cCuTvWallSchemePollOperateReq, cCuTvWallSchemePollOperateRsp);
}

s32 CuCreateMap(CCuMapCreateReq &cCuMapCreateReq, CCuMapCreateRsp &cCuMapCreateRsp)
{
    return OperateImp<CCuMapCreateReq, CCuMapCreateRsp, CCuiMsgMapCreateReq, CCuiMsgMapCreateRsp>(cCuMapCreateReq, cCuMapCreateRsp);
}

s32 CuModifyMap(CCuMapModReq &cCuMapModReq, CCuMapModRsp &cCuMapModRsp)
{
    return OperateImp<CCuMapModReq, CCuMapModRsp, CCuiMsgMapModReq, CCuiMsgMapModRsp>(cCuMapModReq, cCuMapModRsp);
}

s32 CuDelMap(CCuMapDelReq &cCuMapDelReq, CCuMapDelRsp &cCuMapDelRsp)
{
    return OperateImp<CCuMapDelReq, CCuMapDelRsp, CCuiMsgMapDelReq, CCuiMsgMapDelRsp>(cCuMapDelReq, cCuMapDelRsp);
}

s32 CuQueryMap(CCuMapQryReq &cCuMapQryReq, CCuMapQryRsp &cCuMapQryRsp)
{
    return OperateImp<CCuMapQryReq, CCuMapQryRsp, CCuiMsgMapQryReq, CCuiMsgMapQryRsp>(cCuMapQryReq, cCuMapQryRsp);
}

s32 CuCreateMapElement(CCuMapElementCreateReq &cCuMapElementCreateReq, CCuMapElementCreateRsp &cCuMapElementCreateRsp)
{
    return OperateImp<CCuMapElementCreateReq, CCuMapElementCreateRsp, CCuiMsgMapElementCreateReq, CCuiMsgMapElementCreateRsp>(cCuMapElementCreateReq, cCuMapElementCreateRsp);
}

s32 CuModifyMapElement(CCuMapElementModReq &cCuMapElementModReq, CCuMapElementModRsp &cCuMapElementModRsp)
{
    return OperateImp<CCuMapElementModReq, CCuMapElementModRsp, CCuiMsgMapElementModReq, CCuiMsgMapElementModRsp>(cCuMapElementModReq, cCuMapElementModRsp);
}

s32 CuDelMapElement(CCuMapElementDelReq &cCuMapElementDelReq, CCuMapElementDelRsp &cCuMapElementDelRsp)
{
    return OperateImp<CCuMapElementDelReq, CCuMapElementDelRsp, CCuiMsgMapElementDelReq, CCuiMsgMapElementDelRsp>(cCuMapElementDelReq, cCuMapElementDelRsp);
}

s32 CuQueryMapElement(CCuMapElementQryReq &cCuMapElementQryReq, CCuMapElementQryRsp &cCuMapElementQryRsp)
{
    return OperateImp<CCuMapElementQryReq, CCuMapElementQryRsp, CCuiMsgMapElementQryReq, CCuiMsgMapElementQryRsp>(cCuMapElementQryReq, cCuMapElementQryRsp);
}

s32 CuSearchMapElement(CCuMapElementSearchReq &cCuMapElementSearchReq, CCuMapElementSearchRsp &cCuMapElementSearchRsp)
{
    return OperateImp<CCuMapElementSearchReq, CCuMapElementSearchRsp,
		CCuiMsgMapElementSearchReq, CCuiMsgMapElementSearchRsp>(cCuMapElementSearchReq, cCuMapElementSearchRsp);
}

s32 CuMapPositionMar2Earth(CCuPosCorrectMarsToEarthReq &cCuPosCorrectMarsToEarthReq, CCuPosCorrectMarsToEarthRsp &cCuPosCorrectMarsToEarthRsp)
{
    return OperateImp<CCuPosCorrectMarsToEarthReq, CCuPosCorrectMarsToEarthRsp, CCuiMsgPosCorrectMarsToEarthReq, CCuiMsgPosCorrectMarsToEarthRsp>(cCuPosCorrectMarsToEarthReq, cCuPosCorrectMarsToEarthRsp);
}

s32 CuMapPositionEarth2Mar(CCuPosCorrectEarthToMarsReq &cCuPosCorrectEarthToMarsReq, CCuPosCorrectEarthToMarsRsp &cCuPosCorrectEarthToMarsRsp)
{
    return OperateImp<CCuPosCorrectEarthToMarsReq, CCuPosCorrectEarthToMarsRsp, CCuiMsgPosCorrectEarthToMarsReq, CCuiMsgPosCorrectEarthToMarsRsp>(cCuPosCorrectEarthToMarsReq, cCuPosCorrectEarthToMarsRsp);
}

s32 CuQueryAlarmLinkConfig(CCuAlarmLinkQueryReq &cCuAlarmLinkQueryReq, CCuAlarmLinkQueryRsp &cCuAlarmLinkQueryRsp)
{
    return OperateImp<CCuAlarmLinkQueryReq, CCuAlarmLinkQueryRsp ,CCuiMsgAlarmLinkQueryReq, CCuiMsgAlarmLinkQueryRsp>(cCuAlarmLinkQueryReq, cCuAlarmLinkQueryRsp);
}

s32 CuGetAlarmLinkConfig(CCuAlarmLinkCfgGetReq &cCuAlarmLinkCfgGetReq, CCuAlarmLinkCfgGetRsp &cCuAlarmLinkCfgGetRsp)
{
    return OperateImp<CCuAlarmLinkCfgGetReq, CCuAlarmLinkCfgGetRsp, CCuiMsgAlarmLinkCfgGetReq, CCuiMsgAlarmLinkCfgGetRsp>(cCuAlarmLinkCfgGetReq, cCuAlarmLinkCfgGetRsp);
}

s32 CuSetAlarmLinkConfig(CCuAlarmLinkCfgSetReq &cCuAlarmLinkCfgSetReq, CCuAlarmLinkCfgSetRsp &cCuAlarmLinkCfgSetRsp)
{
    return OperateImp<CCuAlarmLinkCfgSetReq, CCuAlarmLinkCfgSetRsp, CCuiMsgAlarmLinkCfgSetReq, CCuiMsgAlarmLinkCfgSetRsp>(cCuAlarmLinkCfgSetReq, cCuAlarmLinkCfgSetRsp);
}

s32 CuQueryUserLog(CCuOperateLogQryReq &cCuOperateLogQryReq, CCuOperateLogQryRsp &cCuOperateLogQryRsp)
{
    return OperateImp<CCuOperateLogQryReq, CCuOperateLogQryRsp, CCuiMsgOperateLogQryReq, CCuiMsgOperateLogQryRsp>(cCuOperateLogQryReq, cCuOperateLogQryRsp);
}

s32 CuQueryDeviceLog(CCuDeviceLogQryReq &cCuDeviceLogQryReq, CCuDeviceLogQryRsp &cCuDeviceLogQryRsp)
{
    return OperateImp<CCuDeviceLogQryReq, CCuDeviceLogQryRsp, CCuiMsgDeviceLogQryReq, CCuiMsgDeviceLogQryRsp>(cCuDeviceLogQryReq, cCuDeviceLogQryRsp);
}

s32 CuQueryAlarmLog(CCuAlarmLogQryReq &cCuAlarmLogQryReq, CCuAlarmLogQryRsp &cCuAlarmLogQryRsp)
{
    return OperateImp<CCuAlarmLogQryReq, CCuAlarmLogQryRsp, CCuiMsgAlarmLogQryReq, CCuiMsgAlarmLogQryRsp>(cCuAlarmLogQryReq, cCuAlarmLogQryRsp);
}

s32 CuGetLogConfig(CCuConfigUasGetReq &cCuConfigUasGetReq, CCuConfigUasGetRsp &cCuConfigUasGetRsp)
{
    return OperateImp<CCuConfigUasGetReq, CCuConfigUasGetRsp, CCuiMsgConfigUasGetReq, CCuiMsgConfigUasGetRsp>(cCuConfigUasGetReq, cCuConfigUasGetRsp);
}

s32 CuSetLogConfig(CCuConfigUasSetReq &cCuConfigUasSetReq, CCuConfigUasSetRsp &cCuConfigUasSetRsp)
{
    return OperateImp<CCuConfigUasSetReq, CCuConfigUasSetRsp, CCuiMsgConfigUasSetReq, CCuiMsgConfigUasSetRsp>(cCuConfigUasSetReq, cCuConfigUasSetRsp);
}

s32 CuGetDeviceGBID(CCuGbsDevGbidGetReq &cCuGbsDevGbidGetReq, CCuGbsDevGbidGetRsp &cCuGbsDevGbidGetRsp)
{
	return OperateImp<CCuGbsDevGbidGetReq,CCuGbsDevGbidGetRsp,CCuiMsgGbsDevGbidGetReq,CCuiMsgGbsDevGbidGetRsp>(cCuGbsDevGbidGetReq,cCuGbsDevGbidGetRsp);
}

s32 CuCallPu(CCuStartThirdPartyCallReq &cCuStartThirdPartyCallReq, CCuStartThirdPartyCallRsp &cCuStartThirdPartyCallRsp )
{
    return OperateImp<CCuStartThirdPartyCallReq, CCuStartThirdPartyCallRsp, CCuiMsgStartThirdPartyCallReq, CCuiMsgStartThirdPartyCallRsp>(cCuStartThirdPartyCallReq, cCuStartThirdPartyCallRsp);
}

s32 CuHangUpPu(CCuStopThirdPartyCallReq &cCuStopThirdPartyCallReq, CCuStopThirdPartyCallRsp &cCuStopThirdPartyCallRsp)
{
    return OperateImp<CCuStopThirdPartyCallReq, CCuStopThirdPartyCallRsp, CCuiMsgStopThirdPartyCallReq, CCuiMsgStopThirdPartyCallRsp>(cCuStopThirdPartyCallReq, cCuStopThirdPartyCallRsp);
}

s32 CuGetDeviceVidEncoderParam(CCuPuVidEncParamGetReq &cPuVidEncParamGetReq, CCuPuVidEncParamGetRsp &cPuVidEncParamGetRsp)
{
    return OperateImp<CCuPuVidEncParamGetReq, CCuPuVidEncParamGetRsp, CCuiMsgPuVidEncParamGetReq, CCuiMsgPuVidEncParamGetRsp>(cPuVidEncParamGetReq, cPuVidEncParamGetRsp);
}

s32 CuGetDeviceAudEncoderParam(CCuPuAudEncParamGetReq &cPuAudEncParamGetReq, CCuPuAudEncParamGetRsp &cPuAudEncParamGetRsp)
{
	return OperateImp<CCuPuAudEncParamGetReq, CCuPuAudEncParamGetRsp, CCuiMsgPuAudEncParamGetReq, CCuiMsgPuAudEncParamGetRsp>(cPuAudEncParamGetReq, cPuAudEncParamGetRsp);
}

s32 CuSetDeviceEncoderParam(CCuPuVidEncParamSetReq &cCuPuVidEncParamSetReq, CCuPuVidEncParamSetRsp &cCuPuVidEncParamSetRsp)
{
    return OperateImp<CCuPuVidEncParamSetReq, CCuPuVidEncParamSetRsp, CCuiMsgPuVidEncParamSetReq, CCuiMsgPuVidEncParamSetRsp>(cCuPuVidEncParamSetReq, cCuPuVidEncParamSetRsp);
}

s32 CuGetDiskList(CCuRcsQueryDiskInfoReq &cCuRcsQueryDiskInfoReq, CCuRcsQueryDiskInfoRsp &cCuRcsQueryDiskInfoRsp)
{
    return OperateImp<CCuRcsQueryDiskInfoReq, CCuRcsQueryDiskInfoRsp, CCuiMsgRcsQueryDiskInfoReq, CCuiMsgRcsQueryDiskInfoRsp>(cCuRcsQueryDiskInfoReq, cCuRcsQueryDiskInfoRsp);
}

s32 CuSetDeviceName(CCuDeviceNameModReq &cCuDeviceNameModReq, CCuDeviceNameModRsp &cCuDeviceNameModRsp)
{
    return OperateImp<CCuDeviceNameModReq, CCuDeviceNameModRsp, CCuiMsgDeviceNameModReq, CCuiMsgDeviceNameModRsp>(cCuDeviceNameModReq, cCuDeviceNameModRsp);
}

s32 CuSetVideoSrcName(CCuDeviceCapIndexNameSetReq &cCuDeviceCapIndexNameSetReq, CCuDeviceCapIndexNameSetRsp &cCuDeviceCapIndexNameSetRsp)
{
    return OperateImp<CCuDeviceCapIndexNameSetReq, CCuDeviceCapIndexNameSetRsp, CCuiMsgDeviceCapIndexNameSetReq, CCuiMsgDeviceCapIndexNameSetRsp>(cCuDeviceCapIndexNameSetReq, cCuDeviceCapIndexNameSetRsp);
}

s32 CuGetMotionDetectConfig(CCuPuMotionDetectGetReq &cReq, CCuPuMotionDetectGetRsp &cRsp)
{
    return OperateImp<CCuPuMotionDetectGetReq, CCuPuMotionDetectGetRsp, CCuiMsgPuMotionDetectGetReq, CCuiMsgPuMotionDetectGetRsp>(cReq, cRsp);
}

s32 CuSetMotionDetectConfig(CCuPuMotionDetectSetReq &cReq, CCuPuMotionDetectSetRsp &cRsp)
{
    return OperateImp<CCuPuMotionDetectSetReq, CCuPuMotionDetectSetRsp, CCuiMsgPuMotionDetectSetReq, CCuiMsgPuMotionDetectSetRsp>(cReq, cRsp);
}

s32 CuGetShadeConfig(CCuPuAreaShadeGetReq &cReq, CCuPuAreaShadeGetRsp &cRsp)
{
    return OperateImp<CCuPuAreaShadeGetReq, CCuPuAreaShadeGetRsp, CCuiMsgPuAreaShadeGetReq, CCuiMsgPuAreaShadeGetRsp>(cReq, cRsp);
}

s32 CuSetShadeConfig(CCuPuAreaShadeSetReq& cReq, CCuPuAreaShadeSetRsp& cRsp)
{
    return OperateImp<CCuPuAreaShadeSetReq, CCuPuAreaShadeSetRsp, CCuiMsgPuAreaShadeSetReq, CCuiMsgPuAreaShadeSetRsp>(cReq, cRsp);
}

s32 CuSetTVDivNum(CTvWinStyleSetReq& cReq, CTvWinStyleSetRsp& cRsp)
{
    return OperateImp<CTvWinStyleSetReq, CTvWinStyleSetRsp, CCuiMsgTvWinStyleSetReq, CCuiMsgTvWinStyleSetRsp>(cReq, cRsp);
}

s32 CuGetDeviceChannelAlias(CCuDeviceCapIndexNameGetReq& cReq, CCuDeviceCapIndexNameGetRsp& cRsp)
{
    return OperateImp<CCuDeviceCapIndexNameGetReq, CCuDeviceCapIndexNameGetRsp, CCuiMsgDeviceCapIndexNameGetReq, CCuiMsgDeviceCapIndexNameGetRsp>(cReq, cRsp);
}

s32 CuSetDeviceChannelAlias(CCuDeviceCapIndexNameSetReq& cReq, CCuDeviceCapIndexNameSetRsp& cRsp)
{
    return OperateImp<CCuDeviceCapIndexNameSetReq, CCuDeviceCapIndexNameSetRsp , CCuiMsgDeviceCapIndexNameSetReq, CCuiMsgDeviceCapIndexNameSetRsp>(cReq, cRsp);
}

s32 CuGetSyncVideoSrcNameEnableAlias(CCuGetSyncVideoSrcNameEnableReq& cReq, CCuGetSyncVideoSrcNameEnableRsp& cRsp)
{
    return OperateImp<CCuGetSyncVideoSrcNameEnableReq, CCuGetSyncVideoSrcNameEnableRsp , CCuiMsgGetSyncVideoSrcNameEnableReq, CCuiMsgGetSyncVideoSrcNameEnableRsp>(cReq, cRsp);
}

s32 CuSetSyncVideoSrcNameEnableAlias(CCuSetSyncVideoSrcNameEnableReq& cReq, CCuSetSyncVideoSrcNameEnableRsp& cRsp)
{
	return OperateImp<CCuSetSyncVideoSrcNameEnableReq, CCuSetSyncVideoSrcNameEnableRsp , CCuiMsgSetSyncVideoSrcNameEnableReq, CCuiMsgSetSyncVideoSrcNameEnableRsp>(cReq, cRsp);
}

s32 CuGetSupportMapTypes(CCuGetSupportMapTypesReq& cReq, CCuGetSupportMapTypesRsp& cRsp)
{
    return OperateImp<CCuGetSupportMapTypesReq, CCuGetSupportMapTypesRsp, CCuiMsgGetSupportMapTypesReq, CCuiMsgGetSupportMapTypesRsp>(cReq, cRsp);
}

s32 CuQueryMapArea(CCuMapAreaQryReq& cReq, CCuMapAreaQryRsp& cRsp)
{
    return OperateImp<CCuMapAreaQryReq, CCuMapAreaQryRsp, CCuiMsgMapAreaQryReq, CCuiMsgMapAreaQryRsp>(cReq, cRsp);
}

s32 CuCreateMapArea(CCuMapAreaCreateReq& cReq, CCuMapAreaCreateRsp& cRsp)
{
    return OperateImp<CCuMapAreaCreateReq, CCuMapAreaCreateRsp, CCuiMsgMapAreaCreateReq, CCuiMsgMapAreaCreateRsp>(cReq, cRsp);
}

s32 CuModifyMapArea(CCuMapAreaModReq& cReq, CCuMapAreaModRsp& cRsp)
{
    return OperateImp<CCuMapAreaModReq, CCuMapAreaModRsp, CCuiMsgMapAreaModReq, CCuiMsgMapAreaModRsp>(cReq, cRsp);
}

s32 CuVsipChannelData(CCuPuVsipOperateReq& cReq, CCuPuVsipOperateRsp& cRsp)
{
    return OperateImp<CCuPuVsipOperateReq, CCuPuVsipOperateRsp, CCuiMsgPuVsipOperateReq, CCuiMsgPuVsipOperateRsp>(cReq, cRsp);
}

s32 CuGetMapImageUploadUrl(CCuMapUploadReq& cReq, CCuMapUploadRsp& cRsp)
{
    return OperateImp<CCuMapUploadReq, CCuMapUploadRsp, CCuiMsgMapUploadReq, CCuiMsgMapUploadRsp>(cReq, cRsp);
}

s32 CuGetMapElementImageUploadUrl(CCuElementIconUploadReq& cReq, CCuElementIconUploadRsp& cRsp)
{
    return OperateImp<CCuElementIconUploadReq, CCuElementIconUploadRsp, CCuiMsgElementIconUploadReq, CCuiMsgElementIconUploadRsp>(cReq, cRsp);
}

s32 CuQueryMapElementImageList(CCuElementImageQryReq& cReq, CCuElementImageQryRsp& cRsp)
{
    return OperateImp<CCuElementImageQryReq, CCuElementImageQryRsp, CCuiMsgElementImageQryReq, CCuiMsgElementImageQryRsp>(cReq, cRsp);
}

s32 CuSetCameraProtocolType(CCuPuCameraTypeSetReq& cReq, CCuPuCameraTypeSetRsp& cRsp)
{
    return OperateImp<CCuPuCameraTypeSetReq, CCuPuCameraTypeSetRsp, CCuiMsgPuCameraTypeSetReq, CCuiMsgPuCameraTypeSetRsp>(cReq, cRsp);
}

s32 CuResetAlarm(CCuPuAlarmResetReq& cReq, CCuPuAlarmResetRsp& cRsp)
{
	return OperateImp<CCuPuAlarmResetReq, CCuPuAlarmResetRsp, CCuiMsgPuAlarmResetReq, CCuiMsgPuAlarmResetRsp>(cReq, cRsp);
}

s32 CuDeviceRecordSvrQry(CCuDeviceRecordSvrQryReq& cReq, CCuDeviceRecordSvrQryRsp& cRsp)
{
	return OperateImp<CCuDeviceRecordSvrQryReq, CCuDeviceRecordSvrQryRsp, CCuiMsgDeviceRecordSvrQryReq, CCuiMsgDeviceRecordSvrQryRsp>(cReq, cRsp);
}

s32 CuPuRecord(CCuPuRecQryReq &cReq, CCuPuRecQryRsp &cRsp)
{
    return OperateImp<CCuPuRecQryReq, CCuPuRecQryRsp, CCuiMsgPuRecQryReq, CCuiMsgPuRecQryRsp>(cReq, cRsp);
}

s32 CuGetPtzPresetName(CCuVideosourcePresetcfgQryReq &cReq, CCuVideosourcePresetcfgQryRsp &cRsp)
{
    return OperateImp<CCuVideosourcePresetcfgQryReq, CCuVideosourcePresetcfgQryRsp, CCuiMsgVideosourcePresetcfgQryReq, CCuiMsgVideosourcePresetcfgQryRsp>(cReq, cRsp);
}

s32 CuSetPtzPresetName(CCuVideosourcePresetcfgAddReq &cReq, CCuVideosourcePresetcfgAddRsp &cRsp)
{
    return OperateImp<CCuVideosourcePresetcfgAddReq, CCuVideosourcePresetcfgAddRsp, CCuiMsgVideosourcePresetcfgAddReq, CCuiMsgVideosourcePresetcfgAddRsp>(cReq, cRsp);
}

s32 CuDelPtzPresetName(CCuVideosourcePresetcfgDelReq &cReq, CCuVideosourcePresetcfgDelRsp &cRsp)
{
    return OperateImp<CCuVideosourcePresetcfgDelReq, CCuVideosourcePresetcfgDelRsp, CCuiMsgVideosourcePresetcfgDelReq, CCuiMsgVideosourcePresetcfgDelRsp>(cReq, cRsp);
}

s32 CuStartExport(CCuFileExportReq &cReq, CCuFileExportRsp &cRsp)
{
    return OperateImp<CCuFileExportReq, CCuFileExportRsp, CCuiMsgFileExportReq, CCuiMsgFileExportRsp>(cReq, cRsp);
}
s32 CuGetExportProcess(CCuGetExportProcessReq &cReq, CCuGetExportProcessRsp &cRsp)
{
    return OperateImp<CCuGetExportProcessReq, CCuGetExportProcessRsp, CCuiMsgGetExportProcessReq, CCuiMsgGetExportProcessRsp>(cReq, cRsp);
}
s32 CuStopExport(CCuStopExportReq &cReq, CCuStopExportRsp &cRsp)
{
    return OperateImp<CCuStopExportReq, CCuStopExportRsp, CCuiMsgStopExportUserLogReq, CCuiMsgStopExportUserLogRsp>(cReq, cRsp);
}

s32 CuStartExportEx(CCuFileExportStartExReq &cReq, CCuFileExportStartExRsp &cRsp)
{
	return OperateImp<CCuFileExportStartExReq, CCuFileExportStartExRsp, CCuiMsgFileExportStartExReq, CCuiMsgFileExportStartExRsp>(cReq, cRsp);
}

s32 CuGetExportProcessEx(CCuFileExportGetProcessExReq &cReq, CCuFileExportGetProcessExRsp &cRsp)
{
	return OperateImp<CCuFileExportGetProcessExReq, CCuFileExportGetProcessExRsp, CCuiMsgFileExportGetProcessExReq, CCuiMsgFileExportGetProcessExRsp>(cReq, cRsp);
}

s32 CuStopExportEx(CCuFileExportStopExReq &cReq, CCuFileExportStopExRsp &cRsp)
{
	return OperateImp<CCuFileExportStopExReq, CCuFileExportStopExRsp, CCuiMsgFileExportStopExReq, CCuiMsgFileExportStopExRsp>(cReq, cRsp);
}

s32 CuStartImportEx(CCuFileImportStartExReq &cReq, CCuFileImportStartExRsp &cRsp)
{
	return OperateImp<CCuFileImportStartExReq, CCuFileImportStartExRsp, CCuiMsgFileImportStartExReq, CCuiMsgFileImportStartExRsp>(cReq, cRsp);
}

s32 CuSetImportProcessEx(CCuFileImportSetProcessExReq &cReq, CCuFileImportSetProcessExRsp &cRsp)
{
	return OperateImp<CCuFileImportSetProcessExReq, CCuFileImportSetProcessExRsp, CCuiMsgFileImportSetProcessExReq, CCuiMsgFileImportSetProcessExRsp>(cReq, cRsp);
}

s32 CuStopImportEx(CCuFileImportStopExReq &cReq, CCuFileImportStopExRsp &cRsp)
{
	return OperateImp<CCuFileImportStopExReq, CCuFileImportStopExRsp, CCuiMsgFileImportStopExReq, CCuiMsgFileImportStopExRsp>(cReq, cRsp);
}

s32 CuGetAlarmLinkDataVersion( CCuAlarmLinkVersionGetReq &cReq, CCuAlarmLinkVersionGetRsp &cRsp )
{
    return OperateImp<CCuAlarmLinkVersionGetReq, CCuAlarmLinkVersionGetRsp, CCuiAlarmLinkVersionGetReq, CCuiAlarmLinkVersionGetRsp>(cReq, cRsp);
}

s32 CuEMapDataVersion( CCuMpsDataVersionGetReq &cReq, CCuMpsDataVersionGetRsp &cRsp )
{
    return OperateImp<CCuMpsDataVersionGetReq, CCuMpsDataVersionGetRsp, CCuiMsgMpsDataVersionGetReq, CCuiMsgMpsDataVersionGetRsp>(
        cReq, cRsp);
}

s32 CuEGpsDataBackUp(CCuGpsDataBakeUpReq &cReq,CCuGpsDataBakeUpRsp &cRsp)
{
	return OperateImp<CCuGpsDataBakeUpReq,CCuGpsDataBakeUpRsp,CCuiMsgGpsDataBakeUpReq,CCuiMsgGpsDataBakeUpRsp>(cReq,cRsp);
}

s32 CuGpsStoreCfgGet(CCuMpsGpsStoreCfgGetReq &cReq,CCuMpsGpsStoreCfgGetRsp &cRsp)
{
	return OperateImp<CCuMpsGpsStoreCfgGetReq,CCuMpsGpsStoreCfgGetRsp,CCuiMsgMpsGpsStoreCfgGetReq,CCuiMsgMpsGpsStoreCfgGetRsp>(cReq,cRsp);
}

s32 CuGetDeviceSerialConfig( CCuPuSerialParamCfgGetReq &cReq, CCuPuSerialParamCfgGetRsp &cRsp )
{
    return OperateImp<CCuPuSerialParamCfgGetReq, CCuPuSerialParamCfgGetRsp, 
        CCuiTemplateMsg<CCuPuSerialParamCfgGetReq>, CCuiTemplateMsg<CCuPuSerialParamCfgGetRsp> >(cReq, cRsp);
}

s32 CuSetDeviceSerialConfig( CCuPuSerialParamCfgSetReq &cReq, CCuPuSerialParamCfgSetRsp &cRsp )
{
    return OperateImp<CCuPuSerialParamCfgSetReq, CCuPuSerialParamCfgSetRsp, 
        CCuiTemplateMsg<CCuPuSerialParamCfgSetReq>, CCuiTemplateMsg<CCuPuSerialParamCfgSetRsp> >(cReq, cRsp);
}

s32 CuGetCAPSConfig( CCuGetCAPSConfigReq cReq, CCuGetCAPSConfigRsp &cRsp )
{
	return OperateImp<CCuGetCAPSConfigReq, CCuGetCAPSConfigRsp, 
		CCuiTemplateMsg<CCuGetCAPSConfigReq>, CCuiTemplateMsg<CCuGetCAPSConfigRsp> >(cReq, cRsp);
}

s32 CuSetCAPSConfig( CCuSetCAPSConfigReq cReq, CCuSetCAPSConfigRsp &cRsp )
{
	return OperateImp<CCuSetCAPSConfigReq, CCuSetCAPSConfigRsp, 
		CCuiTemplateMsg<CCuSetCAPSConfigReq>, CCuiTemplateMsg<CCuSetCAPSConfigRsp> >(cReq, cRsp);
}

s32 CuQueryCAPSDeviceList( CCuQueryCAPSDeviceListReq cReq, CCuQueryCAPSDeviceListRsp &cRsp )
{
	return OperateImp<CCuQueryCAPSDeviceListReq, CCuQueryCAPSDeviceListRsp, 
		CCuiTemplateMsg<CCuQueryCAPSDeviceListReq>, CCuiTemplateMsg<CCuQueryCAPSDeviceListRsp> >(cReq, cRsp);
}

s32 CuCAPSAddDevice( CCuCAPSAddDeviceReq cReq, CCuCAPSAddDeviceRsp &cRsp )
{
	return OperateImp<CCuCAPSAddDeviceReq, CCuCAPSAddDeviceRsp, 
		CCuiTemplateMsg<CCuCAPSAddDeviceReq>, CCuiTemplateMsg<CCuCAPSAddDeviceRsp> >(cReq, cRsp);
}

s32 CuCAPSModifyDevice( CCuCAPSModifyDeviceReq cReq, CCuCAPSModifyDeviceRsp &cRsp )
{
	return OperateImp<CCuCAPSModifyDeviceReq, CCuCAPSModifyDeviceRsp, 
		CCuiTemplateMsg<CCuCAPSModifyDeviceReq>, CCuiTemplateMsg<CCuCAPSModifyDeviceRsp> >(cReq, cRsp);
}

s32 CuCAPSDelDevice( CCuCAPSDelDeviceReq cReq, CCuCAPSDelDeviceRsp &cRsp )
{
	return OperateImp<CCuCAPSDelDeviceReq, CCuCAPSDelDeviceRsp, 
		CCuiTemplateMsg<CCuCAPSDelDeviceReq>, CCuiTemplateMsg<CCuCAPSDelDeviceRsp> >(cReq, cRsp);
}

s32 CuUpdateCUProperty( CCuUpdateCUPropertyReq cReq, CCuUpdateCUPropertyRsp &cRsp )
{
	return OperateImp<CCuUpdateCUPropertyReq, CCuUpdateCUPropertyRsp, 
		CCuiTemplateMsg<CCuUpdateCUPropertyReq>, CCuiTemplateMsg<CCuUpdateCUPropertyRsp> >(cReq, cRsp);
}

s32 CuGetOfflineDeviceStat( CCuOffLineDevQryReq cReq, CCuOffLineDevQryRsp &cRsp )
{
	return OperateImp<CCuOffLineDevQryReq, CCuOffLineDevQryRsp, 
		CCuiTemplateMsg<CCuOffLineDevQryReq>, CCuiTemplateMsg<CCuOffLineDevQryRsp> >(cReq, cRsp);
}

s32 CuGbsGbDevTeleBoot(CCuGbsGbDevTeleBootReq cReq, CCuGbsGbDevTeleBootRsp &cRsp)
{
	return OperateImp< CCuGbsGbDevTeleBootReq, CCuGbsGbDevTeleBootRsp,
		CCuiMsgGbsGbDevTeleBootReq, CCuiMsgGbsGbDevTeleBootRsp>( cReq, cRsp );
}

s32 CuUserDeviceGroupAdd(CCuUserDeviceGroupAddReq cReq, CCuUserDeviceGroupAddRsp &cRsp)
{
	return OperateImp<CCuUserDeviceGroupAddReq, CCuUserDeviceGroupAddRsp,
		CCuiMsgUserDeviceGroupAddReq, CCuiMsgUserDeviceGroupAddRsp>(cReq, cRsp);
}

s32 CuUserDeviceGroupDel(CCuUserDeviceGroupDelReq cReq, CCuUserDeviceGroupDelRsp &cRsp)
{
	return OperateImp<CCuUserDeviceGroupDelReq, CCuUserDeviceGroupDelRsp,
		CCuiMsgUserDeviceGroupDelReq, CCuiMsgUserDeviceGroupDelRsp>(cReq, cRsp);
}

s32 CuUserGroupAddDeviceGroup(CCuUserGroupDeviceGroupAddReq cReq, CCuUserGroupDeviceGroupAddRsp &cRsp)
{
	return OperateImp<CCuUserGroupDeviceGroupAddReq, CCuUserGroupDeviceGroupAddRsp,
		CCuiMsgUserGroupDeviceGroupAddReq, CCuiMsgUserGroupDeviceGroupAddRsp>(cReq, cRsp);
}

s32 CuUserGroupDelDeviceGroup(CCuUserGroupDeviceGroupDelReq cReq, CCuUserGroupDeviceGroupDelRsp &cRsp)
{
	return OperateImp<CCuUserGroupDeviceGroupDelReq, CCuUserGroupDeviceGroupDelRsp,
		CCuiMsgUserGroupDeviceGroupDelReq, CCuiMsgUserGroupDeviceGroupDelRsp>(cReq, cRsp);
}

s32 CuUserSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserSetReq cReq, CCuDeviceAutoAssignToUserSetRsp &cRsp)
{
	return OperateImp<CCuDeviceAutoAssignToUserSetReq, CCuDeviceAutoAssignToUserSetRsp,
		CCuiMsgDeviceAutoAssignToUserSetReq, CCuiMsgDeviceAutoAssignToUserSetRsp>(cReq, cRsp);
}

s32 CuUserGroupSetAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupSetReq cReq, CCuDeviceAutoAssignToUserGroupSetRsp &cRsp)
{
	return OperateImp<CCuDeviceAutoAssignToUserGroupSetReq, CCuDeviceAutoAssignToUserGroupSetRsp,
		CCuiMsgDeviceAutoAssignToUserGroupSetReq, CCuiMsgDeviceAutoAssignToUserGroupSetRsp>(cReq, cRsp);
}

s32 CuUserQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserQryReq cReq, CCuDeviceAutoAssignToUserQryRsp &cRsp)
{
	return OperateImp<CCuDeviceAutoAssignToUserQryReq, CCuDeviceAutoAssignToUserQryRsp,
		CCuiMsgDeviceAutoAssignToUserQryReq, CCuiMsgDeviceAutoAssignToUserQryRsp>(cReq, cRsp);
}

s32 CuUserGroupQueryAutoSyncDeviceGroup(CCuDeviceAutoAssignToUserGroupQryReq cReq, CCuDeviceAutoAssignToUserGroupQryRsp &cRsp)
{
	return OperateImp<CCuDeviceAutoAssignToUserGroupQryReq, CCuDeviceAutoAssignToUserGroupQryRsp,
		CCuiMsgDeviceAutoAssignToUserGroupQryReq, CCuiMsgDeviceAutoAssignToUserGroupQryRsp>(cReq, cRsp);
}

s32 CuQueryOperationProgree(CCuTransactionProgressGetReq cReq, CCuTransactionProgressGetRsp &cRsp)
{
	return OperateImp<CCuTransactionProgressGetReq, CCuTransactionProgressGetRsp,
		CCuiMsgTransactionProgressGetReq, CCuiMsgTransactionProgressGetRsp>(cReq, cRsp);
}

s32 CuGetSuperGBDomainList(CCuGbsGbParentDomainGetReq cReq, CCuGbsGbParentDomainGetRsp &cRsp)
{
	return OperateImp<CCuGbsGbParentDomainGetReq, CCuGbsGbParentDomainGetRsp,
		CCuiMsgGbsGbParentDomainGetReq, CCuiMsgGbsGbParentDomainGetRsp>(cReq, cRsp);
}

s32 CuPushDevice(CCuGbsGbPDCatalogLoadReq cReq, CCuGbsGbPDCatalogLoadRsp &cRsp)
{
	return OperateImp<CCuGbsGbPDCatalogLoadReq, CCuGbsGbPDCatalogLoadRsp,
		CCuiMsgGbsGbPDCatalogLoadReq, CCuiMsgGbsGbPDCatalogLoadRsp>(cReq, cRsp);
}

s32 CuGetUserDataVersion(CCuUserDataVersionGetReq cReq, CCuUserDataVersionGetRsp &cRsp)
{
	return OperateImp<CCuUserDataVersionGetReq, CCuUserDataVersionGetRsp,
	CCuiMsgCuUserDataVersionGetReq, CCuiMsgCuUserDataVersionGetRsp>(cReq, cRsp);
}

s32 CuGetCivilcode(CCuGbsDevGrpCivilCodeGetReq cReq, CCuGbsDevGrpCivilCodeGetRsp &cRsp)
{
	return OperateImp<CCuGbsDevGrpCivilCodeGetReq, CCuGbsDevGrpCivilCodeGetRsp,
		CCuiMsgGbsDevGrpCivilCodeGetReq, CCuiMsgGbsDevGrpCivilCodeGetRsp>(cReq, cRsp);
}

s32 CuGetGbsDevKdid(CCuGbsDevKdidGetReq cReq, CCuGbsDevKdidGetRsp &cRsp)
{
	return OperateImp<CCuGbsDevKdidGetReq, CCuGbsDevKdidGetRsp,
		CCuiMsgGbsDevKdidGetReq, CCuiMsgGbsDevKdidGetRsp>(cReq, cRsp);
}

s32 CuCreateCustomTreeView(CCuDevTreeAddReq cReq, CCuDevTreeAddRsp &cRsp)
{
	return OperateImp<CCuDevTreeAddReq, CCuDevTreeAddRsp,
		CCuiMsgDevTreeAddReq, CCuiMsgDevTreeAddRsp>(cReq, cRsp);
}

s32 CuModifyCustomTreeViewName(CCuDevTreeModReq cReq, CCuDevTreeModRsp &cRsp)
{
	return OperateImp<CCuDevTreeModReq, CCuDevTreeModRsp,
		CCuiMsgDevTreeModReq, CCuiMsgDevTreeModRsp>(cReq, cRsp);
}

s32 CuDeleteCustomTreeView(CCuDevTreeDelReq cReq, CCuDevTreeDelRsp &cRsp)
{
	return OperateImp<CCuDevTreeDelReq, CCuDevTreeDelRsp,
		CCuiMsgDevTreeDelReq, CCuiMsgDevTreeDelRsp>(cReq, cRsp);
}

s32 CuGetCustomTreeViewList(CCuDevTreeQryReq cReq, CCuDevTreeQryRsp &cRsp)
{
	return OperateImp<CCuDevTreeQryReq, CCuDevTreeQryRsp,
		CCuiMsgDevTreeQryReq, CCuiMsgDevTreeQryRsp>(cReq, cRsp);
}

s32 CuGetDeviceGroupPath(CCuDeviceGrpPathQryReq cReq, CCuDeviceGrpPathQryRsp &cRsp)
{
	return OperateImp<CCuDeviceGrpPathQryReq, CCuDeviceGrpPathQryRsp,
		CCuiMsgDeviceGrpPathQryReq, CCuiMsgDeviceGrpPathQryRsp>(cReq, cRsp);
}

s32 CuSearchDevice(CCuDPSSearchReq cReq, CCuDPSSearchRsp &cRsp)
{
	return OperateImp<CCuDPSSearchReq, CCuDPSSearchRsp,
		CCuiMsgDPSSearchReq, CCuiMsgDPSSearchRsp>(cReq, cRsp);
}

s32 CuGetSearchResult(CCuDPSGetResultReq cReq, CCuDPSGetResultRsp &cRsp)
{
	return OperateImp<CCuDPSGetResultReq, CCuDPSGetResultRsp,
		CCuiMsgDPSGetResultReq, CCuiMsgDPSGetResultRsp>(cReq, cRsp);
}

s32 CuGetSearchResultByGroup(CCuDPSGetResultByGroupReq cReq, CCuDPSGetResultByGroupRsp &cRsp)
{
	return OperateImp<CCuDPSGetResultByGroupReq, CCuDPSGetResultByGroupRsp,
		CCuiMsgDPSGetResultByGroupReq, CCuiMsgDPSGetResultByGroupRsp>(cReq, cRsp);
}

s32 CuEndOperation(CCuDPSSearchStopReq cReq, CCuDPSSearchStopRsp &cRsp)
{
	return OperateImp<CCuDPSSearchStopReq, CCuDPSSearchStopRsp,
		CCuiMsgDPSSearchStopReq, CCuiMsgDPSSearchStopRsp>(cReq, cRsp);
}

s32 CuGetDeviceStatus(CCuDPSGetDeviceStatusReq cReq, CCuDPSGetDeviceStatusRsp &cRsp)
{
	return OperateImp<CCuDPSGetDeviceStatusReq, CCuDPSGetDeviceStatusRsp,
		CCuiMsgDPSGetDeviceStatusReq, CCuiMsgDPSGetDeviceStatusRsp>(cReq, cRsp);
}

s32 CuGetDevInfoAndStatus(CCuGetDevInfoAndStatusReq cReq, CCuGetDevInfoAndStatusRsp &cRsp)
{
	return OperateImp<CCuGetDevInfoAndStatusReq, CCuGetDevInfoAndStatusRsp,
		CCuiMsgGetDevInfoAndStatusReq, CCuiMsgGetDevInfoAndStatusRsp>(cReq, cRsp);
}

s32 CuGetUserDeviceInGroupRecursive(CCuUserDevGrpVidsrcQryReq cReq, CCuUserDevGrpVidsrcQryRsp &cRsp)
{
    return OperateImp<CCuUserDevGrpVidsrcQryReq, CCuUserDevGrpVidsrcQryRsp,
        CCuiTemplateMsg<CCuUserDevGrpVidsrcQryReq>, CCuiTemplateMsg<CCuUserDevGrpVidsrcQryRsp> >(cReq, cRsp);
}

s32 CuBatchOperation(CCuBatOptReq cReq, CCuBatOptRsp &cRsp)
{
	return OperateImp<CCuBatOptReq, CCuBatOptRsp,
		CCuiMsgBatOptReq, CCuiMsgBatOptRsp>(cReq, cRsp);
}

s32 CuGetDeviceGroupVideoSrcNum(CCuDevGrpVidsrcTotalnumGetReq cReq, CCuDevGrpVidsrcTotalnumGetRsp &cRsp)
{
	return OperateImp<CCuDevGrpVidsrcTotalnumGetReq, CCuDevGrpVidsrcTotalnumGetRsp,
		CCuiMsgDevGrpVidsrcTotalnumGetReq, CCuiMsgDevGrpVidsrcTotalnumGetRsp>(cReq, cRsp);
}

s32 CuGetVSOfflineInfo(CCuVidsrcInfoQryReq cReq, CCuVidsrcInfoQryRsp &cRsp)
{
	return OperateImp<CCuVidsrcInfoQryReq, CCuVidsrcInfoQryRsp,
		CCuiMsgVidsrcInfoQryReq, CCuiMsgVidsrcInfoQryRsp>(cReq, cRsp);
}

s32 CuGetAzimuth(CCuGetAzimuthReq cReq, CCuGetAzimuthRsp &cRsp)
{
	return OperateImp<CCuGetAzimuthReq, CCuGetAzimuthRsp,
		CCuiMsgGetAzimuthReq, CCuiMsgGetAzimuthRsp>(cReq, cRsp);
}

s32 CuGetDeviceGroupOfGBDeviceInfo(CCuDeviceGroupDeviceQry4GbReq &cReq, CCuDeviceGroupDeviceQry4GbRsp &cRsp)
{
	return OperateImp<CCuDeviceGroupDeviceQry4GbReq, CCuDeviceGroupDeviceQry4GbRsp,
		CCuiMsgDeviceGroupDeviceQry4GbReq, CCuiMsgDeviceGroupDeviceQry4GbRsp>(cReq, cRsp);
}

s32 CuDeviceMatchGbid(CCuDeviceMatchGbidReq cReq, CCuDeviceMatchGbidRsp &cRsp)
{
	return OperateImp<CCuDeviceMatchGbidReq, CCuDeviceMatchGbidRsp,
		CCuiMsgDeviceMatchGbidReq, CCuiMsgDeviceMatchGbidRsp>(cReq, cRsp);
}

s32 CuDeviceGrpDevSumNum(CCuDeviceGrpDevSumNumReq cReq, CCuDeviceGrpDevSumNumRsp &cRsp)
{
	return OperateImp<CCuDeviceGrpDevSumNumReq, CCuDeviceGrpDevSumNumRsp,
		CCuiMsgDeviceGrpDevSumNumReq, CCuiMsgDeviceGrpDevSumNumRsp>(cReq, cRsp);
}

s32 CuDeviceGroupDeviceMod4Gb(CDeviceGroupDeviceMod4GbReq cReq, CDeviceGroupDeviceMod4GbRsp &cRsp)
{
	return OperateImp<CDeviceGroupDeviceMod4GbReq, CDeviceGroupDeviceMod4GbRsp,
		CCuiMsgDeviceGroupDeviceMod4GbReq, CCuiMsgDeviceGroupDeviceMod4GbRsp>(cReq, cRsp);
}

s32 CuDeviceGbidErrGet(CCuDeviceGbidErrGetReq cReq, CCuDeviceGbidErrGetRsp &cRsp)
{
	return OperateImp<CCuDeviceGbidErrGetReq, CCuDeviceGbidErrGetRsp,
		CCuiMsgDeviceGbidErrGetReq, CCuiMsgDeviceGbidErrGetRsp>(cReq, cRsp);
}

s32 CuGbsCatalogSelectReportStart(CCuGbsCatalogSelectReportStartReq cReq, CCuGbsCatalogSelectReportStartRsp &cRsp)
{
	return OperateImp<CCuGbsCatalogSelectReportStartReq, CCuGbsCatalogSelectReportStartRsp,
		CCuiMsgGbsCatalogSelectReportStartReq, CCuiMsgGbsCatalogSelectReportStartRsp>(cReq, cRsp);
}

s32 CCuDeviceGbinfoQry(CCuDeviceGbinfoQryReq cReq, CCuDeviceGbinfoQryRsp &cRsp)
{
	return OperateImp<CCuDeviceGbinfoQryReq, CCuDeviceGbinfoQryRsp,
		CCuiMsgDeviceGbinfoQryReq, CCuiMsgDeviceGbinfoQryRsp>(cReq, cRsp);
}

s32 CCuDevgrpGbinfoQry(CCuDevgrpGbinfoQryReq cReq, CCuDevgrpGbinfoQryRsp &cRsp)
{
	return OperateImp<CCuDevgrpGbinfoQryReq, CCuDevgrpGbinfoQryRsp,
		CCuiMsgDevgrpGbinfoQryReq, CCuiMsgDevgrpGbinfoQryRsp>(cReq, cRsp);
}

s32 CCuSetPuVidSrcOSD(CCuPuVidSrcOsdSetReq cReq, CCuPuVidSrcOsdSetRsp &cRsp)
{
	return OperateImp<CCuPuVidSrcOsdSetReq, CCuPuVidSrcOsdSetRsp,
		CCuiMsgPuVidSrcOsdSetReq, CCuiMsgPuVidSrcOsdSetRsp>(cReq, cRsp);
}

s32 CCuGetPuVidSrcOSD(CCuPuVidSrcOsdGetReq cReq, CCuPuVidSrcOsdGetRsp &cRsp)
{
	return OperateImp<CCuPuVidSrcOsdGetReq, CCuPuVidSrcOsdGetRsp,
		CCuiMsgPuVidSrcOsdGetReq, CCuiMsgPuVidSrcOsdGetRsp>(cReq, cRsp);
}

s32 CuModifyGBDeviceGroup(CCuModifyGBDeviceGroupReq cReq, CCuModifyGBDeviceGroupRsp &cRsp)
{
	return OperateImp<CCuModifyGBDeviceGroupReq, CCuModifyGBDeviceGroupRsp,
		CCuiMsgModifyGBDeviceGroupReq, CCuiMsgModifyGBDeviceGroupRsp>(cReq, cRsp);
}

s32 CuPuLongOsdGet(CCuPuLongOsdGetReq cReq, CCuPuLongOsdGetRsp &cRsp)
{
	return OperateImp<CCuPuLongOsdGetReq, CCuPuLongOsdGetRsp,
		CCuiMsgPuLongOsdGetReq, CCuiMsgPuLongOsdGetRsp>(cReq, cRsp);
}

s32 CuPuLongOsdSet(CCuPuLongOsdSetReq cReq, CCuPuLongOsdSetRsp &cRsp)
{
	return OperateImp<CCuPuLongOsdSetReq, CCuPuLongOsdSetRsp,
		CCuiMsgPuLongOsdSetReq, CCuiMsgPuLongOsdSetRsp>(cReq, cRsp);
}



s32 CuAdjustVideoSrcOrder(CCuAdjustVideoSrcOrderReq cReq, CCuAdjustVideoSrcOrderRsp &cRsp)
{
	return OperateImp<CCuAdjustVideoSrcOrderReq, CCuAdjustVideoSrcOrderRsp ,
        CCuiMsgDeviceOrderDeviceGroupReq, CCuiMsgDeviceOrderDeviceGroupRsp >(cReq, cRsp);
}

s32 CuGetAlarmTypes(CCuGetAlarmTypesReq cReq, CCuGetAlarmTypesRsp &cRsp)
{
	return OperateImp<CCuGetAlarmTypesReq, CCuGetAlarmTypesRsp,
		CCuiMsgGetAlarmTypesReq, CCuiMsgGetAlarmTypesRsp>(cReq, cRsp);
}

s32 CuPlatformRecordLock(CCuRcsRecordLockOprReq cCuRcsRecordLockOprReq, CCuRcsRecordLockOprRsp &cCuRcsRecordLockOprRsp)
{
	return OperateImp<CCuRcsRecordLockOprReq, CCuRcsRecordLockOprRsp, 
		CCuiMsgRcsRecordLockOprReq, CCuiMsgRcsRecordLockOprRsp>(cCuRcsRecordLockOprReq, cCuRcsRecordLockOprRsp);
}

s32 CuPlatformRecordUnLock(CCuRcsRecordUnLockOprReq cCuRcsRecordUnLockOprReq, CCuRcsRecordUnLockOprRsp &cCuRcsRecordUnLockOprRsp)
{
	return OperateImp<CCuRcsRecordUnLockOprReq, CCuRcsRecordUnLockOprRsp, 
		CCuiMsgRcsRecordUnLockOprReq, CCuiMsgRcsRecordUnLockOprRsp>(cCuRcsRecordUnLockOprReq, cCuRcsRecordUnLockOprRsp);
}

s32 CuRealMediaSwitchOccurTypeReq(CCuRealMediaSwitchOccurTypeReq cReq, CCuRealMediaSwitchOccurTypeRsp &cRsp)
{
	return OperateImp<CCuRealMediaSwitchOccurTypeReq, CCuRealMediaSwitchOccurTypeRsp, 
		CCuiMsgRealMediaSwitchOccurTypeReq, CCuiMsgRealMediaSwitchOccurTypeRsp>(cReq, cRsp);
}

s32 CuGetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeGetReq cReq, CCuGbsGbdomainParentPrivilegeGetRsp &cRsp)
{
	return OperateImp<CCuGbsGbdomainParentPrivilegeGetReq, CCuGbsGbdomainParentPrivilegeGetRsp,
		CCuiMsgGbsGbdomainParentPrivilegeGetReq, CCuiMsgGbsGbdomainParentPrivilegeGetRsp>(cReq, cRsp);
}

s32 CuSetGBDomainPrivilege(CCuGbsGbdomainParentPrivilegeSetReq cReq, CCuGbsGbdomainParentPrivilegeSetRsp &cRsp)
{
	return OperateImp<CCuGbsGbdomainParentPrivilegeSetReq, CCuGbsGbdomainParentPrivilegeSetRsp,
		CCuiMsgGbsGbdomainParentPrivilegeSetReq, CCuiMsgGbsGbdomainParentPrivilegeSetRsp>(cReq, cRsp);
}

s32 CuGetGbsOptionConf(CCuGbsOptionConfGetReq cReq, CCuGbsOptionConfGetRsp &cRsp)
{
	return OperateImp<CCuGbsOptionConfGetReq, CCuGbsOptionConfGetRsp,
		CCuiMsgGbsOptionConfGetReq, CCuiMsgGbsOptionConfGetRsp>(cReq, cRsp);
}
s32 CuSetGbsOptionConf(CCuGbsOptionConfSetReq cReq, CCuGbsOptionConfSetRsp &cRsp)
{
	return OperateImp<CCuGbsOptionConfSetReq, CCuGbsOptionConfSetRsp,
		CCuiMsgGbsOptionConfSetReq, CCuiMsgGbsOptionConfSetRsp>(cReq, cRsp);
}
s32 CuUploadGbsCertificate(CCuGbsCertificateUploadReq cReq, CCuGbsCertificateUploadRsp &cRsp)
{
	return OperateImp<CCuGbsCertificateUploadReq, CCuGbsCertificateUploadRsp,
		CCuiMsgGbsCertificateUploadReq, CCuiMsgGbsCertificateUploadRsp>(cReq, cRsp);
}
s32 CuGetGbsCertificate(CCuGbsCertificateGetReq cReq, CCuGbsCertificateGetRsp &cRsp)
{
	return OperateImp<CCuGbsCertificateGetReq, CCuGbsCertificateGetRsp,
		CCuiMsgGbsCertificateGetReq, CCuiMsgGbsCertificateGetRsp>(cReq, cRsp);
}

s32 CuCertificateQry(CCuCertificateQryReq cReq, CCuCertificateQryRsp &cRsp)
{
	return OperateImp<CCuCertificateQryReq, CCuCertificateQryRsp,
		CCuiMsgCertificateQryReq, CCuiMsgCertificateQryRsp>(cReq, cRsp);
}

s32 CuDeviceSignatrueCtrl(CCuDeviceSignatrueCtrlReq cReq, CCuDeviceSignatrueCtrlRsp &cRsp)
{
	return OperateImp<CCuDeviceSignatrueCtrlReq, CCuDeviceSignatrueCtrlRsp,
		CCuiMsgDeviceSignatrueCtrlReq, CCuiMsgDeviceSignatrueCtrlRsp>(cReq, cRsp);
}

s32 CuDeviceEncryptionCtrl(CCuDeviceEncryptionCtrlReq cReq, CCuDeviceEncryptionCtrlRsp &cRsp)
{
	return OperateImp<CCuDeviceEncryptionCtrlReq, CCuDeviceEncryptionCtrlRsp,
		CCuiMsgDeviceEncryptionCtrlReq, CCuiMsgDeviceEncryptionCtrlRsp>(cReq, cRsp);
}

s32 CuSecurityAuthenticateFirstTime(CCuSecurityAuthorizationFirstReq  &cReq, CCuSecurityAuthorizationFirstRsp  &cRsp)
{
	return OperateImp<CCuSecurityAuthorizationFirstReq, CCuSecurityAuthorizationFirstRsp,
		CCuiMsgSecurityAuthorizationFirstReq, CCuiMsgSecurityAuthorizationFirstRsp>(cReq, cRsp);
}

s32 CuSecurityAuthenticateSecondTime(CCuSecurityAuthorizationSecondReq &cReq, CCuSecurityAuthorizationSecondRsp &cRsp)
{
	return OperateImp<CCuSecurityAuthorizationSecondReq, CCuSecurityAuthorizationSecondRsp,
		CCuiMsgSecurityAuthorizationSecondReq, CCuiMsgSecurityAuthorizationSecondRsp>(cReq, cRsp);
}

