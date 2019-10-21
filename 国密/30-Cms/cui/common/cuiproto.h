/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cuiproto.h
   相关文件    : 
   文件实现功能: cui_fcgi功能
   作者        : 梁  黎
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/09/23  1.0         梁  黎        创建
******************************************************************************/
#ifndef _CUI_PROTO_H_
#define _CUI_PROTO_H_

#include "cuistruct.h"
#ifdef WITH_FASTCGI
#include "system/libfcgi/include/fcgi_stdio.h"
#endif

using namespace std;

const s32 CUI_MAGIC_NUM = (s32)0x20120930;
const s32 CUI_PROTOCOL_VER = (s32)0x01;
const s32 MAX_OPR_TIMEOUT = 10;   //操作最大超时时间，单位秒


#define CUI_MAX_LOG_BUF_LEN 1024

inline int CuiWriteLog(LPCSTR lpFile, const char* szFormat, ...)
{
    char szMsg[CUI_MAX_LOG_BUF_LEN] = {0};
    va_list pvlist;
    va_start(pvlist, szFormat);
    int nLen = vsprintf((char *)szMsg, szFormat, pvlist);
    if( nLen <= 0 || nLen >= sizeof(szMsg) ) szMsg[sizeof(szMsg)-1] = '\0';
    va_end(pvlist);

#ifndef _CUISERVER_
    FCGI_fprintf( FCGI_stderr, "%s", szMsg );
#endif

#ifdef DEBUG
    int nWriteLen = strlen(szMsg);
    std::ofstream file(lpFile, std::ios_base::out | std::ios_base::binary | std::ios_base::app );
    if ( !file )
    {
        return -1;
    }
    file.write(szMsg, nWriteLen);
    file.close();
    return nWriteLen;
#else
    return 0;
#endif
}

class CMsgHead
{
public:
    s32 nMsgVer;    //消息版本号
    s32 nMagicNum;  //幻数，防伪
    s32 nMsgType;   //消息号
    s32 nMsgLen;    //消息长度, 不包括消息头
public:
    CMsgHead()
    {
        nMsgVer = CUI_PROTOCOL_VER;
        nMagicNum = CUI_MAGIC_NUM;
        nMsgType = 0;
        nMsgLen = 0;
    }

    s32 ToBuffer(s8* szBuf, s32 nBufLen)
    {
        CMS_ASSERT(NULL != szBuf);
        CMS_ASSERT((u32)nBufLen >= sizeof(CMsgHead));

        s32 nTmp = (s32)htonl((u32)nMsgVer);
        s8 *pTmp = szBuf;
        
        memcpy(pTmp, &nTmp, sizeof(nTmp));
        pTmp += sizeof(nTmp);
        
        nTmp = (s32)htonl((u32)nMagicNum);
        memcpy(pTmp, &nTmp, sizeof(nTmp));
        pTmp += sizeof(nTmp);
        
        nTmp = (s32)htonl((u32)nMsgType);
        memcpy(pTmp, &nTmp, sizeof(nTmp));
        pTmp += sizeof(nTmp);
        
        nTmp = (s32)htonl((u32)nMsgLen);
        memcpy(pTmp, &nTmp, sizeof(nTmp));
        pTmp += sizeof(nTmp);

        return sizeof(CMsgHead);
    }
    s32 FromBuffer(s8* szBuf, s32 nBufLen)
    {
        CMS_ASSERT(NULL != szBuf);
        CMS_ASSERT((u32)nBufLen >= sizeof(CMsgHead));

        s32 *pTmp = (s32 *)szBuf;
        s32 nTmp = (s32)ntohl((u32)*(pTmp + 0));
        if ( nTmp != CUI_PROTOCOL_VER )
        {
            return -1;
        }
        nTmp = (s32)ntohl((u32)*(pTmp + 1));
        if ( nTmp != CUI_MAGIC_NUM )
        {
            return -1;
        }

        nTmp = (s32)ntohl((u32)*(pTmp + 2));
        nMsgType = nTmp;
        nTmp = (s32)ntohl((u32)*(pTmp + 3));
        nMsgLen = nTmp;
        return sizeof(CMsgHead);
    }
};

template <typename T>
class CCuiTemplateMsg
{
public:
    CMsgHead tMsgHead;
    T    *ptMsgBody;
public:
    CCuiTemplateMsg()
    {
        ptMsgBody = NULL;
    }
    s32 ToBuffer(s8* szBuf, s32 nBufLen)  //返回整个消息长度, 包括消息头
    {
        CMS_ASSERT(NULL != szBuf);
        CMS_ASSERT(nBufLen > 0);
        
        std::string strMsgBody = ptMsgBody->ToXml();
        if (nBufLen < (s32)(strMsgBody.length() + 1 + sizeof(CMsgHead)))
        {
            return -1;
        }

        tMsgHead.nMsgType = CU_CUI_WEBSERVICE_REQ;
        tMsgHead.nMsgLen = strMsgBody.length() + 1;
        s32 nRet = tMsgHead.ToBuffer(szBuf, nBufLen);
        CMS_ASSERT(nRet > 0);

        CMS_ASSERT(strMsgBody.length() + 1 + sizeof(CMsgHead) < (u32)nBufLen);
        strncpy(szBuf+sizeof(CMsgHead), strMsgBody.c_str(), strMsgBody.length());
        szBuf[sizeof(CMsgHead) + strMsgBody.length()] = '\0';
        return (strMsgBody.length() + 1 + sizeof(CMsgHead));
    }
    s32 FromBuffer(s8* szBuf, s32 nBufLen)
    {
        CMS_ASSERT(NULL != szBuf);
        CMS_ASSERT((u32)nBufLen >= sizeof(CMsgHead));
        
        if ( (u32)nBufLen < sizeof(CMsgHead) )
        {
            return -1;
        }
        s32 nRet = tMsgHead.FromBuffer(szBuf, nBufLen);
        CMS_ASSERT(nRet > 0);
        std::string strMsgBody(szBuf + sizeof(CMsgHead), tMsgHead.nMsgLen);
//        strMsgBody.push_back('\0');  //保险起见末尾加个终结符

//        const int dwErrorCode = (const int)atol(GetSipErrorcode(strMsgBody.c_str()).c_str());
        ptMsgBody->FromXml(strMsgBody);   //修改之后子类可以直接解析基类的xml
        //如果是失败Rsp, 可能消息体是CEventRsp
//         if ( CMS_SUCCESS != dwErrorCode )
//         {
//             tMsgBody.SetSeqNum((const int)atol(GetSipSeqNO(strMsgBody.c_str()).c_str()));
//             tMsgBody.SetSession(GetSipSessionID(strMsgBody.c_str()));
//             tMsgBody.SetErrorCode(dwErrorCode);
//         }
//         else
//         {
//             tMsgBody.FromXml(strMsgBody);
//         }
        return (tMsgHead.nMsgLen + sizeof(CMsgHead));
    }
};

//////////////////////////////////////////////////////////////////////////
//各种请求的消息体
typedef CCuiTemplateMsg<CCuiCuLoginReq>         CCuiMsgCuLoginReq;
typedef CCuiTemplateMsg<CCuiCuLoginRsp>         CCuiMsgCuLoginRsp;

typedef CCuiTemplateMsg<CCuLogoutReq>           CCuiMsgCuLogoutReq;
typedef CCuiTemplateMsg<CCuLogoutRsp>           CCuiMsgCuLogoutRsp;

typedef CCuiTemplateMsg<CGetNotifyReq>          CCuiMsgGetNotifyReq;
typedef CCuiTemplateMsg<CGetNotifyRsp>          CCuiMsgGetNotifyRsp;

typedef CCuiTemplateMsg<CCuGetCuiListReq>       CCuiMsgCuGetCuiListReq;
typedef CCuiTemplateMsg<CCuGetCuiListRsp>       CCuiMsgCuGetCuiListRsp;

typedef CCuiTemplateMsg<CPuPtzCtrlReq>          CCuiMsgPuPtzCtrlReq;
typedef CCuiTemplateMsg<CPuPtzCtrlRsp>          CCuiMsgPuPtzCtrlRsp;

typedef CCuiTemplateMsg<CPuPtzLockReq>          CCuiMsgPuPtzLockReq;
typedef CCuiTemplateMsg<CPuPtzLockRsp>          CCuiMsgPuPtzLockRsp;

typedef CCuiTemplateMsg<CPuPtzUnlockReq>        CCuiMsgPuPtzUnlockReq;
typedef CCuiTemplateMsg<CPuPtzUnlockRsp>        CCuiMsgPuPtzUnlockRsp;

typedef CCuiTemplateMsg<CCuPtzLockAddReq>          CCuiMsgPtzLockAddReq;
typedef CCuiTemplateMsg<CCuPtzLockAddRsp>          CCuiMsgPtzLockAddRsp;

typedef CCuiTemplateMsg<CCuPtzLockDelReq>        CCuiMsgPtzLockDelReq;
typedef CCuiTemplateMsg<CCuPtzLockDelRsp>        CCuiMsgPtzLockDelRsp;

typedef CCuiTemplateMsg<CCuPuTransparentOperateReq>  CCuiMsgPuTransparentOperateReq;
typedef CCuiTemplateMsg<CCuPuTransparentOperateRsp>  CCuiMsgPuTransparentOperateRsp;

typedef CCuiTemplateMsg<CCuPuTransparentQryReq>  CCuiMsgPuTransparentQryReq;
typedef CCuiTemplateMsg<CCuPuTransparentQryRsp>  CCuiMsgPuTransparentQryRsp;

typedef CCuiTemplateMsg<CCuQueryUserReq>            CCuiMsgCuQueryUserReq;
typedef CCuiTemplateMsg<CCuQueryUserRsp>            CCuiMsgCuQueryUserRsp;

typedef CCuiTemplateMsg<CCuUserGroupQryReq>         CCuiMsgCuUserGroupQryReq;
typedef CCuiTemplateMsg<CCuUserGroupQryRsp>         CCuiMsgCuUserGroupQryRsp;

typedef CCuiTemplateMsg<CCuUserCustomDataQryReq>    CCuiMsgCuUserCustomDataQryReq;
typedef CCuiTemplateMsg<CCuUserCustomDataQryRsp>    CCuiMsgCuUserCustomDataQryRsp;
 
typedef CCuiTemplateMsg<CCuUserCustomDataAddReq>    CCuiMsgCuUserCustomDataAddReq;
typedef CCuiTemplateMsg<CCuUserCustomDataAddRsp>    CCuiMsgCuUserCustomDataAddRsp;

typedef CCuiTemplateMsg<CCuUserCustomDataDelReq>    CCuiMsgCuUserCustomDataDelReq;
typedef CCuiTemplateMsg<CCuUserCustomDataDelRsp>    CCuiMsgCuUserCustomDataDelRsp;

typedef CCuiTemplateMsg<CCuUserDevGroupQryReq>      CCuiMsgCuUserDevGroupQryReq;
typedef CCuiTemplateMsg<CCuUserDevGroupQryRsp>      CCuiMsgCuUserDevGroupQryRsp;

typedef CCuiTemplateMsg<CCuUserDevFullInfoQryReq>   CCuiMsgCuUserDevFullInfoQryReq;
typedef CCuiTemplateMsg<CCuUserDevFullInfoQryRsp>   CCuiMsgCuUserDevFullInfoQryRsp;

typedef CCuiTemplateMsg<CCuSubscribeNotifyReq>    CCuiMsgCuSubscribeNotifyReq;
typedef CCuiTemplateMsg<CCuSubscribeNotifyRsp>    CCuiMsgCuSubscribeNotifyRsp;

typedef CCuiTemplateMsg<CCuUnSubscribeNotifyReq>  CCuiMsgCuUnSubscribePuStatusReq;
typedef CCuiTemplateMsg<CCuUnSubscribeNotifyRsp>  CCuiMsgCuUnSubscribePuStatusRsp;

typedef CCuiTemplateMsg<CCuQryDevGroupOfDevInfoReq> CCuiMsgCuQryDevGroupOfDevInfoReq;
typedef CCuiTemplateMsg<CCuQryDevGroupOfDevInfoRsp> CCuiMsgCuQryDevGroupOfDevInfoRsp;

typedef CCuiTemplateMsg<CCuCreateDeviceGroupReq>    CCuiMsgCuCreateDeviceGroupReq;
typedef CCuiTemplateMsg<CCuCreateDeviceGroupRsp>    CCuiMsgCuCreateDeviceGroupRsp;

typedef CCuiTemplateMsg<CCuDeleteDeviceGroupReq>    CCuiMsgCuDeleteDeviceGroupReq;
typedef CCuiTemplateMsg<CCuDeleteDeviceGroupRsp>    CCuiMsgCuDeleteDeviceGroupRsp;

typedef CCuiTemplateMsg<CCuModifyDeviceGroupReq>    CCuiMsgCuModifyDeviceGroupReq;
typedef CCuiTemplateMsg<CCuModifyDeviceGroupRsp>    CCuiMsgCuModifyDeviceGroupRsp;

typedef CCuiTemplateMsg<CCuDeviceGroupAddDeviceReq> CCuiMsgCuDeviceGroupAddDeviceReq;
typedef CCuiTemplateMsg<CCuDeviceGroupAddDeviceRsp> CCuiMsgCuDeviceGroupAddDeviceRsp;

typedef CCuiTemplateMsg<CCuDeviceGroupDelDeviceReq> CCuiMsgCuDeviceGroupDelDeviceReq;
typedef CCuiTemplateMsg<CCuDeviceGroupDelDeviceRsp> CCuiMsgCuDeviceGroupDelDeviceRsp;

typedef CCuiTemplateMsg<CCuDevTreeDevGrpAssignReq> CCuiMsgCuDevTreeDevGrpAssignReq;
typedef CCuiTemplateMsg<CCuDevTreeDevGrpAssignRsp> CCuiMsgCuDevTreeDevGrpAssignRsp;

typedef CCuiTemplateMsg<CCuCreateUserGroupReq>      CCuiMsgCuCreateUserGroupReq;
typedef CCuiTemplateMsg<CCuCreateUserGroupRsp>      CCuiMsgCuCreateUserGroupRsp;

typedef CCuiTemplateMsg<CCuDeleteUserGroupReq>      CCuiMsgCuDeleteUserGroupReq;
typedef CCuiTemplateMsg<CCuDeleteUserGroupRsp>      CCuiMsgCuDeleteUserGroupRsp;

typedef CCuiTemplateMsg<CCuModifyUserGroupReq>      CCuiMsgCuModifyUserGroupReq;
typedef CCuiTemplateMsg<CCuModifyUserGroupRsp>      CCuiMsgCuModifyUserGroupRsp;

typedef CCuiTemplateMsg<CCuCreateUserReq>           CCuiMsgCuCreateUserReq;
typedef CCuiTemplateMsg<CCuCreateUserRsp>           CCuiMsgCuCreateUserRsp;

typedef CCuiTemplateMsg<CCuDeleteUserReq>           CCuiMsgCuDeleteUserReq;
typedef CCuiTemplateMsg<CCuDeleteUserRsp>           CCuiMsgCuDeleteUserRsp;

typedef CCuiTemplateMsg<CCuModifyUserReq>           CCuiMsgCuModifyUserReq;
typedef CCuiTemplateMsg<CCuModifyUserRsp>           CCuiMsgCuModifyUserRsp;

typedef CCuiTemplateMsg<CCuChangePasswordReq>       CCuiMsgCuChangePasswordReq;

typedef CCuiTemplateMsg<CCuKeyFrameReq> CCuiMsgCuKeyFrameReq;
typedef CCuiTemplateMsg<CCuKeyFrameRsp> CCuiMsgCuKeyFrameRsp;

typedef CCuiTemplateMsg<CCuGetUserDeviceReq>        CCuiMsgCuGetUserDeviceReq;
typedef CCuiTemplateMsg<CCuGetUserDeviceRsp>        CCuiMsgCuGetUserDeviceRsp;

typedef CCuiTemplateMsg<CCuGetUserGroupDeviceReq>   CCuiMsgCuGetUserGroupDeviceReq;
typedef CCuiTemplateMsg<CCuGetUserGroupDeviceRsp>   CCuiMsgCuGetUserGroupDeviceRsp;

typedef CCuiTemplateMsg<CCuUserAddDeviceReq>        CCuiMsgCuUserAddDeviceReq;
typedef CCuiTemplateMsg<CCuUserAddDeviceRsp>        CCuiMsgCuUserAddDeviceRsp;

typedef CCuiTemplateMsg<CCuUserDelDeviceReq>        CCuiMsgCuUserDelDeviceReq;
typedef CCuiTemplateMsg<CCuUserDelDeviceRsp>        CCuiMsgCuUserDelDeviceRsp;

typedef CCuiTemplateMsg<CCuUserModDeviceReq>        CCuiMsgCuUserModDeviceReq;
typedef CCuiTemplateMsg<CCuUserModDeviceRsp>        CCuiMsgCuUserModDeviceRsp;

typedef CCuiTemplateMsg<CCuUserGroupAddDeviceReq>   CCuiMsgCuUserGroupAddDeviceReq;
typedef CCuiTemplateMsg<CCuUserGroupAddDeviceRsp>   CCuiMsgCuUserGroupAddDeviceRsp;

typedef CCuiTemplateMsg<CCuUserGroupDelDeviceReq>   CCuiMsgCuUserGroupDelDeviceReq;
typedef CCuiTemplateMsg<CCuUserGroupDelDeviceRsp>   CCuiMsgCuUserGroupDelDeviceRsp;

typedef CCuiTemplateMsg<CCuUserGroupModDeviceReq>   CCuiMsgCuUserGroupModDeviceReq;
typedef CCuiTemplateMsg<CCuUserGroupModDeviceRsp>   CCuiMsgCuUserGroupModDeviceRsp;

typedef CCuiTemplateMsg<CCuGetAuthorizationDeviceReq> CCuiMsgCuGetAuthorizationDeviceReq;
typedef CCuiTemplateMsg<CCuGetAuthorizationDeviceRsp> CCuiMsgCuGetAuthorizationDeviceRsp;

typedef CCuiTemplateMsg<CCuAddAuthorizationDeviceReq> CCuiMsgCuAddAuthorizationDeviceReq;
typedef CCuiTemplateMsg<CCuAddAuthorizationDeviceRsp> CCuiMsgCuAddAuthorizationDeviceRsp;

typedef CCuiTemplateMsg<CCuDelAuthorizationDeviceReq> CCuiMsgCuDelAuthorizationDeviceReq;
typedef CCuiTemplateMsg<CCuDelAuthorizationDeviceRsp> CCuiMsgCuDelAuthorizationDeviceRsp;

typedef CCuiTemplateMsg<CCuModifyAuthorizationDeviceReq> CCuiMsgCuModifyAuthorizationDeviceReq;
typedef CCuiTemplateMsg<CCuModifyAuthorizationDeviceRsp> CCuiMsgCuModifyAuthorizationDeviceRsp;

typedef CCuiTemplateMsg<CCuGetDeviceBaseInfoReq>    CCuiMsgCuGetDeviceBaseInfoReq;
typedef CCuiTemplateMsg<CCuGetDeviceBaseInfoRsp>    CCuiMsgCuGetDeviceBaseInfoRsp;

typedef CCuiTemplateMsg<CCuSetupStreamReq>          CCuiMsgCuSetupStreamReq;
typedef CCuiTemplateMsg<CCuSetupStreamRsp>          CCuiMsgCuSetupStreamRsp;

typedef CCuiTemplateMsg<CCuPlayStreamReq>           CCuiMsgCuPlayStreamReq;
typedef CCuiTemplateMsg<CCuPlayStreamRsp>           CCuiMsgCuPlayStreamRsp;

typedef CCuiTemplateMsg<CCuStopStreamReq>           CCuiMsgCuStopStreamReq;
typedef CCuiTemplateMsg<CCuStopInviteRsp>           CCuiMsgCuStopStreamRsp;

typedef CCuiTemplateMsg<CCuRcsGetSysParamReq>       CCuiMsgCuRcsGetSysParamReq;
typedef CCuiTemplateMsg<CCuRcsGetSysParamRsp>       CCuiMsgCuRcsGetSysParamRsp;

typedef CCuiTemplateMsg<CCuRcsSetSysParamReq>       CCuiMsgCuRcsSetSysParamReq;
typedef CCuiTemplateMsg<CCuRcsSetSysParamRsp>       CCuiMsgCuRcsSetSysParamRsp; 

typedef CCuiTemplateMsg<CCuRcsQueryNruReq>       CCuiMsgCuRcsQueryNruReq;
typedef CCuiTemplateMsg<CCuRcsQueryNruRsp>       CCuiMsgCuRcsQueryNruRsp;

typedef CCuiTemplateMsg<CCuRcsQueryPuChnBindingReq>       CCuiMsgCuRcsQueryPuChnBindingReq;
typedef CCuiTemplateMsg<CCuRcsQueryPuChnBindingRsp>       CCuiMsgCuRcsQueryPuChnBindingRsp;

typedef CCuiTemplateMsg<CCuRcsBindingPuChnReq>  CCuiMsgRcsBindingPuChnReq;
typedef CCuiTemplateMsg<CCuRcsBindingPuChnRsp>  CCuiMsgRcsBindingPuChnRsp;

typedef CCuiTemplateMsg<CCuRcsUnbindingPuChnReq> CCuiMsgRcsUnbindingPuChnReq;
typedef CCuiTemplateMsg<CCuRcsUnbindingPuChnRsp> CCuiMsgRcsUnbindingPuChnRsp;

typedef CCuiTemplateMsg<CCuRcsQueryRecordDaysReq> CCuiMsgRcsQueryRecordDaysReq;
typedef CCuiTemplateMsg<CCuRcsQueryRecordDaysRsp> CCuiMsgRcsQueryRecordDaysRsp;

typedef CCuiTemplateMsg<CCuPuRecDaysQryReq> CCuiMsgPuRecDaysQryReq;
typedef CCuiTemplateMsg<CCuPuRecDaysQryRsp> CCuiMsgPuRecDaysQryRsp;

typedef CCuiTemplateMsg<CCuRcsQueryRecordReq> CCuiMsgRcsQueryRecordReq;
typedef CCuiTemplateMsg<CCuRcsQueryRecordRsp> CCuiMsgRcsQueryRecordRsp;

typedef CCuiTemplateMsg<CCuRcsSetRecordTaskReq> CCuiMsgRcsSetRecordTaskReq;
typedef CCuiTemplateMsg<CCuRcsSetRecordTaskRsp> CCuiMsgRcsSetRecordTaskRsp;

typedef CCuiTemplateMsg<CCuSetupPlayPlatformRecordReq> CCuiMsgSetupPlayPlatformRecordReq;
typedef CCuiTemplateMsg<CCuSetupPlayPlatformRecordRsp> CCuiMsgSetupPlayPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuPlayPlatformRecordReq> CCuiMsgPlayPlatformRecordReq;
typedef CCuiTemplateMsg<CCuPlayPlatformRecordRsp> CCuiMsgPlayPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuVcrCtrlReq> CCuiMsgVcrCtrlReq;
typedef CCuiTemplateMsg<CCuVcrCtrlRsp> CCuiMsgVcrCtrlRsp;

typedef CCuiTemplateMsg<CCuPlaybackProgressQueryReq> CCuiMsgPlaybackProgressQueryReq;
typedef CCuiTemplateMsg<CCuPlaybackProgressQueryRsp> CCuiMsgPlaybackProgressQueryRsp;

typedef CCuiTemplateMsg<CCuStopPlatformRecordReq> CCuiMsgStopPlatformRecordReq;
typedef CCuiTemplateMsg<CCuStopPlatformRecordRsp> CCuiMsgStopPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuDownloadPlatformRecordReq> CCuiMsgDownloadPlatformRecordReq;
typedef CCuiTemplateMsg<CCuDownloadPlatformRecordRsp> CCuiMsgDownloadPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuStartDownloadPlatformRecordReq> CCuiMsgStartDownloadPlatformRecordReq;
typedef CCuiTemplateMsg<CCuStartDownloadPlatformRecordRsp> CCuiMsgStartDownloadPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuStopDownloadPlatformRecordReq> CCuiMsgStopDownloadPlatformRecordReq;
typedef CCuiTemplateMsg<CCuStopDownloadPlatformRecordRsp> CCuiMsgStopDownloadPlatformRecordRsp;

typedef CCuiTemplateMsg<CCuDomainRelationQryReq> CCuiMsgDomainRelationQryReq;
typedef CCuiTemplateMsg<CCuDomainRelationQryRsp> CCuiMsgDomainRelationQryRsp;

typedef CCuiTemplateMsg<CCuRcsQueryRecordParamReq> CCuiMsgRcsQueryRecordParamReq;
typedef CCuiTemplateMsg<CCuRcsQueryRecordParamRsp> CCuiMsgRcsQueryRecordParamRsp;

typedef CCuiTemplateMsg<CCuRcsSetRecordParamReq> CCuiMsgRcsSetRecordParamReq;
typedef CCuiTemplateMsg<CCuRcsSetRecordParamRsp> CCuiMsgRcsSetRecordParamRsp;

typedef CCuiTemplateMsg<CCuPuRecordStartReq> CCuiMsgPuRecordStartReq;
typedef CCuiTemplateMsg<CCuPuRecordStartRsp> CCuiMsgPuRecordStartRsp;

typedef CCuiTemplateMsg<CCuPuRecordStopReq> CCuiMsgPuRecordStopReq;
typedef CCuiTemplateMsg<CCuPuRecordStopRsp> CCuiMsgPuRecordStopRsp;

typedef CCuiTemplateMsg<CCuTvwallQryReq> CCuiMsgTvwallQryReq;
typedef CCuiTemplateMsg<CCuTvwallQryRsp> CCuiMsgTvwallQryRsp;

typedef CCuiTemplateMsg<CCuTvwallAddReq> CCuiMsgTvwallAddReq;
typedef CCuiTemplateMsg<CCuTvwallAddRsp> CCuiMsgTvwallAddRsp;

typedef CCuiTemplateMsg<CCuTvwallDelReq> CCuiMsgTvwallDelReq;
typedef CCuiTemplateMsg<CCuTvwallDelRsp> CCuiMsgTvwallDelRsp;

typedef CCuiTemplateMsg<CCuPlatRecOnTvReq> CCuiMsgPlatRecOnTvReq;
typedef CCuiTemplateMsg<CCuPlatRecOnTvRsp> CCuiMsgPlatRecOnTvRsp;

typedef CCuiTemplateMsg<CCuPlatRecOffTvReq> CCuiMsgPlatRecOffTvReq;
typedef CCuiTemplateMsg<CCuPlatRecOffTvRsp> CCuiMsgPlatRecOffTvRsp;

typedef CCuiTemplateMsg<CCuRecordOnTvReq> CCuiMsgRecordOnTvReq;
typedef CCuiTemplateMsg<CCuRecordOnTvRsp> CCuiMsgRecordOnTvRsp;

typedef CCuiTemplateMsg<CCuRecordOffTvReq> CCuiMsgRecordOffTvReq;
typedef CCuiTemplateMsg<CCuRecordOffTvRsp> CCuiMsgRecordOffTvRsp;

typedef CCuiTemplateMsg<CCuTvwallModReq> CCuiMsgTvwallModReq;
typedef CCuiTemplateMsg<CCuTvwallModRsp> CCuiMsgTvwallModRsp;

typedef CCuiTemplateMsg<CCuRealMediaOnTvReq> CCuiMsgRealMediaOnTvReq;
typedef CCuiTemplateMsg<CCuRealMediaOnTvRsp> CCuiMsgRealMediaOnTvRsp;

typedef CCuiTemplateMsg<CCuRealMediaOffTvReq> CCuiMsgRealMediaOffTvReq;
typedef CCuiTemplateMsg<CCuRealMediaOffTvRsp> CCuiMsgRealMediaOffTvRsp;

typedef CCuiTemplateMsg<CCuTvPollOperateReq> CCuiMsgTvPollOperateReq;
typedef CCuiTemplateMsg<CCuTvPollOperateRsp> CCuiMsgTvPollOperateRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeSaveReq> CCuiMsgTvWallSchemeSaveReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeSaveRsp> CCuiMsgTvWallSchemeSaveRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeDelReq> CCuiMsgTvWallSchemeDelReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeDelRsp> CCuiMsgTvWallSchemeDelRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeLoadReq> CCuiMsgTvWallSchemeLoadReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeLoadRsp> CCuiMsgTvWallSchemeLoadRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeQueryReq> CCuiMsgTvWallSchemeQueryReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeQueryRsp> CCuiMsgTvWallSchemeQueryRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeGroupSetReq> CCuiMsgTvWallSchemeGroupSetReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeGroupSetRsp> CCuiMsgTvWallSchemeGroupSetRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemeGroupGetReq> CCuiMsgTvWallSchemeGroupGetReq;
typedef CCuiTemplateMsg<CCuTvWallSchemeGroupGetRsp> CCuiMsgTvWallSchemeGroupGetRsp;

typedef CCuiTemplateMsg<CCuTvWallSchemePollOperateReq> CCuiMsgTvWallSchemePollOperateReq;
typedef CCuiTemplateMsg<CCuTvWallSchemePollOperateRsp> CCuiMsgTvWallSchemePollOperateRsp;

typedef CCuiTemplateMsg<CCuMapCreateReq> CCuiMsgMapCreateReq;
typedef CCuiTemplateMsg<CCuMapCreateRsp> CCuiMsgMapCreateRsp;

typedef CCuiTemplateMsg<CCuMapModReq> CCuiMsgMapModReq;
typedef CCuiTemplateMsg<CCuMapModRsp> CCuiMsgMapModRsp;

typedef CCuiTemplateMsg<CCuMapDelReq> CCuiMsgMapDelReq;
typedef CCuiTemplateMsg<CCuMapDelRsp> CCuiMsgMapDelRsp;

typedef CCuiTemplateMsg<CCuMapQryReq> CCuiMsgMapQryReq;
typedef CCuiTemplateMsg<CCuMapQryRsp> CCuiMsgMapQryRsp;

typedef CCuiTemplateMsg<CCuMapElementCreateReq> CCuiMsgMapElementCreateReq;
typedef CCuiTemplateMsg<CCuMapElementCreateRsp> CCuiMsgMapElementCreateRsp;

typedef CCuiTemplateMsg<CCuMapElementModReq> CCuiMsgMapElementModReq;
typedef CCuiTemplateMsg<CCuMapElementModRsp> CCuiMsgMapElementModRsp;

typedef CCuiTemplateMsg<CCuMapElementDelReq> CCuiMsgMapElementDelReq;
typedef CCuiTemplateMsg<CCuMapElementDelRsp> CCuiMsgMapElementDelRsp;

typedef CCuiTemplateMsg<CCuMapElementQryReq> CCuiMsgMapElementQryReq;
typedef CCuiTemplateMsg<CCuMapElementQryRsp> CCuiMsgMapElementQryRsp;

typedef CCuiTemplateMsg<CCuMapElementSearchReq> CCuiMsgMapElementSearchReq;
typedef CCuiTemplateMsg<CCuMapElementSearchRsp> CCuiMsgMapElementSearchRsp;

typedef CCuiTemplateMsg<CCuPosCorrectMarsToEarthReq> CCuiMsgPosCorrectMarsToEarthReq;
typedef CCuiTemplateMsg<CCuPosCorrectMarsToEarthRsp> CCuiMsgPosCorrectMarsToEarthRsp;

typedef CCuiTemplateMsg<CCuPosCorrectEarthToMarsReq> CCuiMsgPosCorrectEarthToMarsReq;
typedef CCuiTemplateMsg<CCuPosCorrectEarthToMarsRsp> CCuiMsgPosCorrectEarthToMarsRsp;

typedef CCuiTemplateMsg<CCuMpsDataVersionGetReq> CCuiMsgMpsDataVersionGetReq;
typedef CCuiTemplateMsg<CCuMpsDataVersionGetRsp> CCuiMsgMpsDataVersionGetRsp;

typedef CCuiTemplateMsg<CCuGpsDataBakeUpReq> CCuiMsgGpsDataBakeUpReq;
typedef CCuiTemplateMsg<CCuGpsDataBakeUpRsp> CCuiMsgGpsDataBakeUpRsp;

typedef CCuiTemplateMsg<CMpsGpsStoreCfgGetReq> CCuiMsgMpsGpsStoreCfgGetReq;
typedef CCuiTemplateMsg<CMpsGpsStoreCfgGetRsp> CCuiMsgMpsGpsStoreCfgGetRsp;

typedef CCuiTemplateMsg<CCuGpsTraceQryReq> CCuiMsgGpsTraceQryReq;
typedef CCuiTemplateMsg<CCuGpsTraceQryRsp> CCuiMsgGpsTraceQryRsp;

typedef CCuiTemplateMsg<CCuAlarmLinkVersionGetReq> CCuiAlarmLinkVersionGetReq;
typedef CCuiTemplateMsg<CCuAlarmLinkVersionGetRsp> CCuiAlarmLinkVersionGetRsp;

typedef CCuiTemplateMsg<CCuAlarmLinkQueryReq> CCuiMsgAlarmLinkQueryReq;
typedef CCuiTemplateMsg<CCuAlarmLinkQueryRsp> CCuiMsgAlarmLinkQueryRsp;

typedef CCuiTemplateMsg<CCuAlarmLinkCfgGetReq> CCuiMsgAlarmLinkCfgGetReq;
typedef CCuiTemplateMsg<CCuAlarmLinkCfgGetRsp> CCuiMsgAlarmLinkCfgGetRsp;

typedef CCuiTemplateMsg<CCuAlarmLinkCfgSetReq> CCuiMsgAlarmLinkCfgSetReq;
typedef CCuiTemplateMsg<CCuAlarmLinkCfgSetRsp> CCuiMsgAlarmLinkCfgSetRsp;

typedef CCuiTemplateMsg<CCuOperateLogQryReq> CCuiMsgOperateLogQryReq;
typedef CCuiTemplateMsg<CCuOperateLogQryRsp> CCuiMsgOperateLogQryRsp;

typedef CCuiTemplateMsg<CCuDeviceLogQryReq> CCuiMsgDeviceLogQryReq;
typedef CCuiTemplateMsg<CCuDeviceLogQryRsp> CCuiMsgDeviceLogQryRsp;

typedef CCuiTemplateMsg<CCuAlarmLogQryReq> CCuiMsgAlarmLogQryReq;
typedef CCuiTemplateMsg<CCuAlarmLogQryRsp> CCuiMsgAlarmLogQryRsp;

typedef CCuiTemplateMsg<CCuConfigUasGetReq> CCuiMsgConfigUasGetReq;
typedef CCuiTemplateMsg<CCuConfigUasGetRsp> CCuiMsgConfigUasGetRsp;

typedef CCuiTemplateMsg<CCuConfigUasSetReq> CCuiMsgConfigUasSetReq;
typedef CCuiTemplateMsg<CCuConfigUasSetRsp> CCuiMsgConfigUasSetRsp;

typedef CCuiTemplateMsg<CCuGbsDevGbidGetReq> CCuiMsgGbsDevGbidGetReq;
typedef CCuiTemplateMsg<CCuGbsDevGbidGetRsp> CCuiMsgGbsDevGbidGetRsp;

typedef CCuiTemplateMsg<CCuStartThirdPartyCallReq> CCuiMsgStartThirdPartyCallReq;
typedef CCuiTemplateMsg<CCuStartThirdPartyCallRsp> CCuiMsgStartThirdPartyCallRsp;

typedef CCuiTemplateMsg<CCuStopThirdPartyCallReq> CCuiMsgStopThirdPartyCallReq;
typedef CCuiTemplateMsg<CCuStopThirdPartyCallRsp> CCuiMsgStopThirdPartyCallRsp;

typedef CCuiTemplateMsg<CCuPuVidEncParamGetReq> CCuiMsgPuVidEncParamGetReq;
typedef CCuiTemplateMsg<CCuPuVidEncParamGetRsp> CCuiMsgPuVidEncParamGetRsp;

typedef CCuiTemplateMsg<CCuPuAudEncParamGetReq> CCuiMsgPuAudEncParamGetReq;
typedef CCuiTemplateMsg<CCuPuAudEncParamGetRsp> CCuiMsgPuAudEncParamGetRsp;

typedef CCuiTemplateMsg<CCuPuVidEncParamSetReq> CCuiMsgPuVidEncParamSetReq;
typedef CCuiTemplateMsg<CCuPuVidEncParamSetRsp> CCuiMsgPuVidEncParamSetRsp;

typedef CCuiTemplateMsg<CCuRcsQueryDiskInfoReq> CCuiMsgRcsQueryDiskInfoReq;
typedef CCuiTemplateMsg<CCuRcsQueryDiskInfoRsp> CCuiMsgRcsQueryDiskInfoRsp;

typedef CCuiTemplateMsg<CCuDeviceNameModReq> CCuiMsgDeviceNameModReq;
typedef CCuiTemplateMsg<CCuDeviceNameModRsp> CCuiMsgDeviceNameModRsp;

typedef CCuiTemplateMsg<CCuDeviceCapIndexNameSetReq> CCuiMsgDeviceCapIndexNameSetReq;
typedef CCuiTemplateMsg<CCuDeviceCapIndexNameSetRsp> CCuiMsgDeviceCapIndexNameSetRsp;

typedef CCuiTemplateMsg<CCuGetSyncVideoSrcNameEnableReq> CCuiMsgGetSyncVideoSrcNameEnableReq;
typedef CCuiTemplateMsg<CCuGetSyncVideoSrcNameEnableRsp> CCuiMsgGetSyncVideoSrcNameEnableRsp;

typedef CCuiTemplateMsg<CCuSetSyncVideoSrcNameEnableReq> CCuiMsgSetSyncVideoSrcNameEnableReq;
typedef CCuiTemplateMsg<CCuSetSyncVideoSrcNameEnableRsp> CCuiMsgSetSyncVideoSrcNameEnableRsp;

typedef CCuiTemplateMsg<CCuPuMotionDetectGetReq> CCuiMsgPuMotionDetectGetReq;
typedef CCuiTemplateMsg<CCuPuMotionDetectGetRsp> CCuiMsgPuMotionDetectGetRsp;

typedef CCuiTemplateMsg<CCuPuMotionDetectSetReq> CCuiMsgPuMotionDetectSetReq;
typedef CCuiTemplateMsg<CCuPuMotionDetectSetRsp> CCuiMsgPuMotionDetectSetRsp;

typedef CCuiTemplateMsg<CCuPuAreaShadeGetReq> CCuiMsgPuAreaShadeGetReq;
typedef CCuiTemplateMsg<CCuPuAreaShadeGetRsp> CCuiMsgPuAreaShadeGetRsp;

typedef CCuiTemplateMsg<CCuPuAreaShadeSetReq> CCuiMsgPuAreaShadeSetReq;
typedef CCuiTemplateMsg<CCuPuAreaShadeSetRsp> CCuiMsgPuAreaShadeSetRsp;

typedef CCuiTemplateMsg<CTvWinStyleSetReq> CCuiMsgTvWinStyleSetReq;
typedef CCuiTemplateMsg<CTvWinStyleSetRsp> CCuiMsgTvWinStyleSetRsp;

typedef CCuiTemplateMsg<CCuDeviceCapIndexNameGetReq>CCuiMsgDeviceCapIndexNameGetReq;
typedef CCuiTemplateMsg<CCuDeviceCapIndexNameGetRsp>CCuiMsgDeviceCapIndexNameGetRsp;

typedef CCuiTemplateMsg<CCuGetSupportMapTypesReq>CCuiMsgGetSupportMapTypesReq;
typedef CCuiTemplateMsg<CCuGetSupportMapTypesRsp>CCuiMsgGetSupportMapTypesRsp;

typedef CCuiTemplateMsg<CCuMapAreaQryReq>CCuiMsgMapAreaQryReq;
typedef CCuiTemplateMsg<CCuMapAreaQryRsp>CCuiMsgMapAreaQryRsp;

typedef CCuiTemplateMsg<CCuMapAreaCreateReq>CCuiMsgMapAreaCreateReq;
typedef CCuiTemplateMsg<CCuMapAreaCreateRsp>CCuiMsgMapAreaCreateRsp;

typedef CCuiTemplateMsg<CCuMapAreaModReq>CCuiMsgMapAreaModReq;
typedef CCuiTemplateMsg<CCuMapAreaModRsp>CCuiMsgMapAreaModRsp;

typedef CCuiTemplateMsg<CCuPuVsipOperateReq>CCuiMsgPuVsipOperateReq;
typedef CCuiTemplateMsg<CCuPuVsipOperateRsp>CCuiMsgPuVsipOperateRsp;

typedef CCuiTemplateMsg<CCuMapUploadReq>CCuiMsgMapUploadReq;
typedef CCuiTemplateMsg<CCuMapUploadRsp>CCuiMsgMapUploadRsp;

typedef CCuiTemplateMsg<CCuElementIconUploadReq>CCuiMsgElementIconUploadReq;
typedef CCuiTemplateMsg<CCuElementIconUploadRsp>CCuiMsgElementIconUploadRsp;

typedef CCuiTemplateMsg<CCuElementImageQryReq>CCuiMsgElementImageQryReq;
typedef CCuiTemplateMsg<CCuElementImageQryRsp>CCuiMsgElementImageQryRsp;

typedef CCuiTemplateMsg<CCuPuCameraTypeSetReq>CCuiMsgPuCameraTypeSetReq;
typedef CCuiTemplateMsg<CCuPuCameraTypeSetRsp>CCuiMsgPuCameraTypeSetRsp;

typedef CCuiTemplateMsg<CCuPuAlarmResetReq>CCuiMsgPuAlarmResetReq;
typedef CCuiTemplateMsg<CCuPuAlarmResetRsp>CCuiMsgPuAlarmResetRsp;

typedef CCuiTemplateMsg<CCuDeviceRecordSvrQryReq>CCuiMsgDeviceRecordSvrQryReq;
typedef CCuiTemplateMsg<CCuDeviceRecordSvrQryRsp>CCuiMsgDeviceRecordSvrQryRsp;

typedef CCuiTemplateMsg<CCuPuRecQryReq> CCuiMsgPuRecQryReq;
typedef CCuiTemplateMsg<CCuPuRecQryRsp> CCuiMsgPuRecQryRsp;

typedef CCuiTemplateMsg<CCuVideosourcePresetcfgQryReq> CCuiMsgVideosourcePresetcfgQryReq;
typedef CCuiTemplateMsg<CCuVideosourcePresetcfgQryRsp> CCuiMsgVideosourcePresetcfgQryRsp;

typedef CCuiTemplateMsg<CCuVideosourcePresetcfgAddReq> CCuiMsgVideosourcePresetcfgAddReq;
typedef CCuiTemplateMsg<CCuVideosourcePresetcfgAddRsp> CCuiMsgVideosourcePresetcfgAddRsp;

typedef CCuiTemplateMsg<CCuVideosourcePresetcfgDelReq> CCuiMsgVideosourcePresetcfgDelReq;
typedef CCuiTemplateMsg<CCuVideosourcePresetcfgDelRsp> CCuiMsgVideosourcePresetcfgDelRsp;

typedef CCuiTemplateMsg<CCuFileExportReq> CCuiMsgFileExportReq;
typedef CCuiTemplateMsg<CCuFileExportRsp> CCuiMsgFileExportRsp;

typedef CCuiTemplateMsg<CCuGetExportProcessReq> CCuiMsgGetExportProcessReq;
typedef CCuiTemplateMsg<CCuGetExportProcessRsp> CCuiMsgGetExportProcessRsp;
typedef CCuiTemplateMsg<CCuStopExportReq> CCuiMsgStopExportUserLogReq;
typedef CCuiTemplateMsg<CCuStopExportRsp> CCuiMsgStopExportUserLogRsp;

typedef CCuiTemplateMsg<CCuFileExportStartExReq> CCuiMsgFileExportStartExReq;
typedef CCuiTemplateMsg<CCuFileExportStartExRsp> CCuiMsgFileExportStartExRsp;
typedef CCuiTemplateMsg<CCuFileExportGetProcessExReq> CCuiMsgFileExportGetProcessExReq;
typedef CCuiTemplateMsg<CCuFileExportGetProcessExRsp> CCuiMsgFileExportGetProcessExRsp;
typedef CCuiTemplateMsg<CCuFileExportStopExReq> CCuiMsgFileExportStopExReq;
typedef CCuiTemplateMsg<CCuFileExportStopExRsp> CCuiMsgFileExportStopExRsp;

typedef CCuiTemplateMsg<CCuFileImportStartExReq> CCuiMsgFileImportStartExReq;
typedef CCuiTemplateMsg<CCuFileImportStartExRsp> CCuiMsgFileImportStartExRsp;
typedef CCuiTemplateMsg<CCuFileImportSetProcessExReq> CCuiMsgFileImportSetProcessExReq;
typedef CCuiTemplateMsg<CCuFileImportSetProcessExRsp> CCuiMsgFileImportSetProcessExRsp;
typedef CCuiTemplateMsg<CCuFileImportStopExReq> CCuiMsgFileImportStopExReq;
typedef CCuiTemplateMsg<CCuFileImportStopExRsp> CCuiMsgFileImportStopExRsp;

typedef CCuiTemplateMsg<CCuGbsGbDevTeleBootReq> CCuiMsgGbsGbDevTeleBootReq;
typedef CCuiTemplateMsg<CCuGbsGbDevTeleBootRsp> CCuiMsgGbsGbDevTeleBootRsp;

typedef CCuiTemplateMsg<CCuUserDeviceGroupAddReq> CCuiMsgUserDeviceGroupAddReq;
typedef CCuiTemplateMsg<CCuUserDeviceGroupAddRsp> CCuiMsgUserDeviceGroupAddRsp;

typedef CCuiTemplateMsg<CCuUserDeviceGroupDelReq> CCuiMsgUserDeviceGroupDelReq;
typedef CCuiTemplateMsg<CCuUserDeviceGroupDelRsp> CCuiMsgUserDeviceGroupDelRsp;

typedef CCuiTemplateMsg<CCuUserGroupDeviceGroupAddReq> CCuiMsgUserGroupDeviceGroupAddReq;
typedef CCuiTemplateMsg<CCuUserGroupDeviceGroupAddRsp> CCuiMsgUserGroupDeviceGroupAddRsp;

typedef CCuiTemplateMsg<CCuUserGroupDeviceGroupDelReq> CCuiMsgUserGroupDeviceGroupDelReq;
typedef CCuiTemplateMsg<CCuUserGroupDeviceGroupDelRsp> CCuiMsgUserGroupDeviceGroupDelRsp;

typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserSetReq> CCuiMsgDeviceAutoAssignToUserSetReq;
typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserSetRsp> CCuiMsgDeviceAutoAssignToUserSetRsp;

typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserGroupSetReq> CCuiMsgDeviceAutoAssignToUserGroupSetReq;
typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserGroupSetRsp> CCuiMsgDeviceAutoAssignToUserGroupSetRsp;

typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserQryReq> CCuiMsgDeviceAutoAssignToUserQryReq;
typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserQryRsp> CCuiMsgDeviceAutoAssignToUserQryRsp;

typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserGroupQryReq> CCuiMsgDeviceAutoAssignToUserGroupQryReq;
typedef CCuiTemplateMsg<CCuDeviceAutoAssignToUserGroupQryRsp> CCuiMsgDeviceAutoAssignToUserGroupQryRsp;

typedef CCuiTemplateMsg<CCuTransactionProgressGetReq> CCuiMsgTransactionProgressGetReq;
typedef CCuiTemplateMsg<CCuTransactionProgressGetRsp> CCuiMsgTransactionProgressGetRsp;

typedef CCuiTemplateMsg<CCuGbsGbParentDomainGetReq> CCuiMsgGbsGbParentDomainGetReq;
typedef CCuiTemplateMsg<CCuGbsGbParentDomainGetRsp> CCuiMsgGbsGbParentDomainGetRsp;

typedef CCuiTemplateMsg<CCuGbsGbPDCatalogLoadReq> CCuiMsgGbsGbPDCatalogLoadReq;
typedef CCuiTemplateMsg<CCuGbsGbPDCatalogLoadRsp> CCuiMsgGbsGbPDCatalogLoadRsp;

typedef CCuiTemplateMsg<CCuUserDataVersionGetReq> CCuiMsgCuUserDataVersionGetReq;
typedef CCuiTemplateMsg<CCuUserDataVersionGetRsp> CCuiMsgCuUserDataVersionGetRsp;

typedef CCuiTemplateMsg<CCuGbsDevGrpCivilCodeGetReq> CCuiMsgGbsDevGrpCivilCodeGetReq;
typedef CCuiTemplateMsg<CCuGbsDevGrpCivilCodeGetRsp> CCuiMsgGbsDevGrpCivilCodeGetRsp;

typedef CCuiTemplateMsg<CCuGbsDevKdidGetReq> CCuiMsgGbsDevKdidGetReq;
typedef CCuiTemplateMsg<CCuGbsDevKdidGetRsp> CCuiMsgGbsDevKdidGetRsp;

typedef CCuiTemplateMsg<CCuDevTreeAddReq> CCuiMsgDevTreeAddReq;
typedef CCuiTemplateMsg<CCuDevTreeAddRsp> CCuiMsgDevTreeAddRsp;

typedef CCuiTemplateMsg<CCuDevTreeModReq> CCuiMsgDevTreeModReq;
typedef CCuiTemplateMsg<CCuDevTreeModRsp> CCuiMsgDevTreeModRsp;

typedef CCuiTemplateMsg<CCuDevTreeDelReq> CCuiMsgDevTreeDelReq;
typedef CCuiTemplateMsg<CCuDevTreeDelRsp> CCuiMsgDevTreeDelRsp;

typedef CCuiTemplateMsg<CCuDevTreeQryReq> CCuiMsgDevTreeQryReq;
typedef CCuiTemplateMsg<CCuDevTreeQryRsp> CCuiMsgDevTreeQryRsp;

typedef CCuiTemplateMsg<CCuDeviceGrpPathQryReq> CCuiMsgDeviceGrpPathQryReq;
typedef CCuiTemplateMsg<CCuDeviceGrpPathQryRsp> CCuiMsgDeviceGrpPathQryRsp;

typedef CCuiTemplateMsg<CCuDPSSearchReq> CCuiMsgDPSSearchReq;
typedef CCuiTemplateMsg<CCuDPSSearchRsp> CCuiMsgDPSSearchRsp;

typedef CCuiTemplateMsg<CCuDPSGetResultReq> CCuiMsgDPSGetResultReq;
typedef CCuiTemplateMsg<CCuDPSGetResultRsp> CCuiMsgDPSGetResultRsp;

typedef CCuiTemplateMsg<CCuDPSGetResultByGroupReq> CCuiMsgDPSGetResultByGroupReq;
typedef CCuiTemplateMsg<CCuDPSGetResultByGroupRsp> CCuiMsgDPSGetResultByGroupRsp;

typedef CCuiTemplateMsg<CCuDPSSearchStopReq> CCuiMsgDPSSearchStopReq;
typedef CCuiTemplateMsg<CCuDPSSearchStopRsp> CCuiMsgDPSSearchStopRsp;

typedef CCuiTemplateMsg<CCuDPSGetDeviceStatusReq> CCuiMsgDPSGetDeviceStatusReq;
typedef CCuiTemplateMsg<CCuDPSGetDeviceStatusRsp> CCuiMsgDPSGetDeviceStatusRsp;

typedef CCuiTemplateMsg<CCuGetDevInfoAndStatusReq> CCuiMsgGetDevInfoAndStatusReq;
typedef CCuiTemplateMsg<CCuGetDevInfoAndStatusRsp> CCuiMsgGetDevInfoAndStatusRsp;

typedef CCuiTemplateMsg<CCuBatOptReq> CCuiMsgBatOptReq;
typedef CCuiTemplateMsg<CCuBatOptRsp> CCuiMsgBatOptRsp;

typedef CCuiTemplateMsg<CCuDevGrpVidsrcTotalnumGetReq> CCuiMsgDevGrpVidsrcTotalnumGetReq;
typedef CCuiTemplateMsg<CCuDevGrpVidsrcTotalnumGetRsp> CCuiMsgDevGrpVidsrcTotalnumGetRsp;

typedef CCuiTemplateMsg<CCuVidsrcInfoQryReq> CCuiMsgVidsrcInfoQryReq;
typedef CCuiTemplateMsg<CCuVidsrcInfoQryRsp> CCuiMsgVidsrcInfoQryRsp;

typedef CCuiTemplateMsg<CCuGetAzimuthReq> CCuiMsgGetAzimuthReq;
typedef CCuiTemplateMsg<CCuGetAzimuthRsp> CCuiMsgGetAzimuthRsp;

typedef CCuiTemplateMsg<CCuDeviceGroupDeviceQry4GbReq> CCuiMsgDeviceGroupDeviceQry4GbReq;
typedef CCuiTemplateMsg<CCuDeviceGroupDeviceQry4GbRsp> CCuiMsgDeviceGroupDeviceQry4GbRsp;

typedef CCuiTemplateMsg<CCuDeviceMatchGbidReq> CCuiMsgDeviceMatchGbidReq;
typedef CCuiTemplateMsg<CCuDeviceMatchGbidRsp> CCuiMsgDeviceMatchGbidRsp;

typedef CCuiTemplateMsg<CCuDeviceGrpDevSumNumReq> CCuiMsgDeviceGrpDevSumNumReq;
typedef CCuiTemplateMsg<CCuDeviceGrpDevSumNumRsp> CCuiMsgDeviceGrpDevSumNumRsp;


typedef CCuiTemplateMsg<CDeviceGroupDeviceMod4GbReq> CCuiMsgDeviceGroupDeviceMod4GbReq;
typedef CCuiTemplateMsg<CDeviceGroupDeviceMod4GbRsp> CCuiMsgDeviceGroupDeviceMod4GbRsp;

typedef CCuiTemplateMsg<CCuDeviceGbidErrGetReq> CCuiMsgDeviceGbidErrGetReq;
typedef CCuiTemplateMsg<CCuDeviceGbidErrGetRsp> CCuiMsgDeviceGbidErrGetRsp;

typedef CCuiTemplateMsg<CCuGbsCatalogSelectReportStartReq> CCuiMsgGbsCatalogSelectReportStartReq;
typedef CCuiTemplateMsg<CCuGbsCatalogSelectReportStartRsp> CCuiMsgGbsCatalogSelectReportStartRsp;

typedef CCuiTemplateMsg<CCuDeviceGbinfoQryReq> CCuiMsgDeviceGbinfoQryReq;
typedef CCuiTemplateMsg<CCuDeviceGbinfoQryRsp> CCuiMsgDeviceGbinfoQryRsp;

typedef CCuiTemplateMsg<CCuDevgrpGbinfoQryReq> CCuiMsgDevgrpGbinfoQryReq;
typedef CCuiTemplateMsg<CCuDevgrpGbinfoQryRsp> CCuiMsgDevgrpGbinfoQryRsp;

typedef CCuiTemplateMsg<CCuPuVidSrcOsdSetReq> CCuiMsgPuVidSrcOsdSetReq;
typedef CCuiTemplateMsg<CCuPuVidSrcOsdSetRsp> CCuiMsgPuVidSrcOsdSetRsp;

typedef CCuiTemplateMsg<CCuPuVidSrcOsdGetReq> CCuiMsgPuVidSrcOsdGetReq;
typedef CCuiTemplateMsg<CCuPuVidSrcOsdGetRsp> CCuiMsgPuVidSrcOsdGetRsp;

typedef CCuiTemplateMsg<CCuModifyGBDeviceGroupReq> CCuiMsgModifyGBDeviceGroupReq;
typedef CCuiTemplateMsg<CCuModifyGBDeviceGroupRsp> CCuiMsgModifyGBDeviceGroupRsp;

typedef CCuiTemplateMsg<CCuPuLongOsdGetReq> CCuiMsgPuLongOsdGetReq;
typedef CCuiTemplateMsg<CCuPuLongOsdGetRsp> CCuiMsgPuLongOsdGetRsp;

typedef CCuiTemplateMsg<CCuPuLongOsdSetReq> CCuiMsgPuLongOsdSetReq;
typedef CCuiTemplateMsg<CCuPuLongOsdSetRsp> CCuiMsgPuLongOsdSetRsp;

typedef CCuiTemplateMsg<CCuAdjustVideoSrcOrderReq> CCuiMsgDeviceOrderDeviceGroupReq;
typedef CCuiTemplateMsg<CCuAdjustVideoSrcOrderRsp> CCuiMsgDeviceOrderDeviceGroupRsp;

typedef CCuiTemplateMsg<CCuRcsRecordLockOprReq> CCuiMsgRcsRecordLockOprReq;
typedef CCuiTemplateMsg<CCuRcsRecordLockOprRsp> CCuiMsgRcsRecordLockOprRsp;

typedef CCuiTemplateMsg<CCuRcsRecordUnLockOprReq> CCuiMsgRcsRecordUnLockOprReq;
typedef CCuiTemplateMsg<CCuRcsRecordUnLockOprRsp> CCuiMsgRcsRecordUnLockOprRsp;

typedef CCuiTemplateMsg<CCuRealMediaSwitchOccurTypeReq> CCuiMsgRealMediaSwitchOccurTypeReq;
typedef CCuiTemplateMsg<CCuRealMediaSwitchOccurTypeRsp> CCuiMsgRealMediaSwitchOccurTypeRsp;

typedef CCuiTemplateMsg<CCuGetAlarmTypesReq> CCuiMsgGetAlarmTypesReq;
typedef CCuiTemplateMsg<CCuGetAlarmTypesRsp> CCuiMsgGetAlarmTypesRsp;

typedef CCuiTemplateMsg<CCuGbsGbdomainParentPrivilegeGetReq> CCuiMsgGbsGbdomainParentPrivilegeGetReq;
typedef CCuiTemplateMsg<CCuGbsGbdomainParentPrivilegeGetRsp> CCuiMsgGbsGbdomainParentPrivilegeGetRsp;
typedef CCuiTemplateMsg<CCuGbsGbdomainParentPrivilegeSetReq> CCuiMsgGbsGbdomainParentPrivilegeSetReq;
typedef CCuiTemplateMsg<CCuGbsGbdomainParentPrivilegeSetRsp> CCuiMsgGbsGbdomainParentPrivilegeSetRsp;

typedef CCuiTemplateMsg<CCuGbsOptionConfGetReq> CCuiMsgGbsOptionConfGetReq;
typedef CCuiTemplateMsg<CCuGbsOptionConfGetRsp> CCuiMsgGbsOptionConfGetRsp;
typedef CCuiTemplateMsg<CCuGbsOptionConfSetReq> CCuiMsgGbsOptionConfSetReq;
typedef CCuiTemplateMsg<CCuGbsOptionConfSetRsp> CCuiMsgGbsOptionConfSetRsp;

//证书上传下载
typedef CCuiTemplateMsg<CCuGbsCertificateUploadReq> CCuiMsgGbsCertificateUploadReq;
typedef CCuiTemplateMsg<CCuGbsCertificateUploadRsp> CCuiMsgGbsCertificateUploadRsp;
typedef CCuiTemplateMsg<CCuGbsCertificateGetReq> CCuiMsgGbsCertificateGetReq;
typedef CCuiTemplateMsg<CCuGbsCertificateGetRsp> CCuiMsgGbsCertificateGetRsp;


typedef CCuiTemplateMsg<CCuCertificateQryReq> CCuiMsgCertificateQryReq;
typedef CCuiTemplateMsg<CCuCertificateQryRsp> CCuiMsgCertificateQryRsp;

typedef CCuiTemplateMsg<CCuDeviceSignatrueCtrlReq> CCuiMsgDeviceSignatrueCtrlReq;
typedef CCuiTemplateMsg<CCuDeviceSignatrueCtrlRsp> CCuiMsgDeviceSignatrueCtrlRsp;

typedef CCuiTemplateMsg<CCuDeviceEncryptionCtrlReq> CCuiMsgDeviceEncryptionCtrlReq;
typedef CCuiTemplateMsg<CCuDeviceEncryptionCtrlRsp> CCuiMsgDeviceEncryptionCtrlRsp;

typedef CCuiTemplateMsg<CCuSecurityAuthorizationFirstReq> CCuiMsgSecurityAuthorizationFirstReq;
typedef CCuiTemplateMsg<CCuSecurityAuthorizationFirstRsp> CCuiMsgSecurityAuthorizationFirstRsp;

typedef CCuiTemplateMsg<CCuSecurityAuthorizationSecondReq> CCuiMsgSecurityAuthorizationSecondReq;
typedef CCuiTemplateMsg<CCuSecurityAuthorizationSecondRsp> CCuiMsgSecurityAuthorizationSecondRsp;

#endif

