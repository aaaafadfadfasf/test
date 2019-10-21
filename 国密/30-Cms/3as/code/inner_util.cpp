#include "inner_util.h"
#include "cms/tas/tas_event.h"
#include "cms/gbs/gbs_event.h"
#include "ospsip/ospsip.h"
#include "ospext/ospext.h"
#include "cms/cmu/cmu_event.h"
#include "inner_common.h"
#include "gbs/gbs_event.h"
#include "redis/fastdbmng.h"

void MakeLower( string& str )
{
    for ( string::iterator it = str.begin(); it != str.end(); ++it )
    {
        if ( *it >= 'A' && *it <= 'Z' )
        {
            *it += ( 'a' - 'A' );
        }
    }
}

void MakeUper( string& str )
{
    for ( string::iterator it = str.begin(); it != str.end(); ++it )
    {
        if ( *it >= 'a' && *it <= 'z' )
        {
            *it += ( 'A' - 'a' );
        }
    }
}

void _replace_str(string& str, const string& strSrc, const string& strDst) 
{
    string::size_type pos = 0;
    string::size_type srcLen = strSrc.size();
    string::size_type dstLen = strDst.size();
    while((pos = str.find(strSrc, pos)) != string::npos)
    {         
        str.replace(pos, srcLen, strDst);                
        pos += dstLen;        
    }
}

void TasReboot()
{
    sleep(3);
    exit(-3);
}

bool IsNtfReqType( u16 wMsgId )
{
    switch (wMsgId)
    {
    case TAS_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_DEL_NTF_REQ:
    case TAS_DEVICE_MOD_NTF_REQ:
    case TAS_DEVICE_QRY_NTF_REQ:
    case TAS_TVWALL_ADD_NTF_REQ:
    case TAS_TVWALL_DEL_NTF_REQ:
    case TAS_TVWALL_MOD_NTF_REQ:
    case TAS_TVWALL_QRY_NTF_REQ:
    case TAS_DEVICE_MODEL_ADD_NTF_REQ:
    case TAS_DEVICE_MODEL_DEL_NTF_REQ:
    case TAS_DEVICE_MODEL_QRY_NTF_REQ:
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ:
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_REQ:
    case TAS_DEVICE_MANUFACTURER_QRY_NTF_REQ:
    case TAS_CUSER_ADD_NTF_REQ:
    case TAS_CUSER_DEL_NTF_REQ:
    case TAS_CUSER_MOD_NTF_REQ:
    case TAS_CUSER_QRY_NTF_REQ:
    case TAS_CUSER_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_DEVICE_MOD_NTF_REQ:
    case TAS_CUSER_DEVICE_QRY_NTF_REQ:
    case TAS_DEVICE_GROUP_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEL_NTF_REQ:
    case TAS_DEVICE_GROUP_MOD_NTF_REQ:
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ:
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ:
    case TAS_CUSER_GROUP_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_MOD_NTF_REQ:
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
    case TAS_DOMAIN_RELATION_QRY_NTF_REQ:
    case TAS_DOMAIN_RELATION_ADD_NTF_REQ:
    case TAS_DOMAIN_RELATION_DEL_NTF_REQ:
    case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
    case TAS_TACUSER_ADD_NTF_REQ:
    case TAS_TACUSER_DEL_NTF_REQ:
    case TAS_TACUSER_MOD_NTF_REQ:
    case TAS_TACUSER_QRY_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_ADD_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_DEL_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_MOD_NTF_REQ:
    case TAS_SYNCDSTDOMAIN_QRY_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ:
    case FILE_COLLECT_PROGRESS_NTF_REQ:
	case TAS_DEVTREE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_REQ:
	case TAS_PTZLOCK_QRY_NTF_REQ:
	case TAS_PTZLOCK_ADD_NTF_REQ:
	case TAS_PTZLOCK_DEL_NTF_REQ:
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
    case TAS_LICENSE_UPDATE_NTF_REQ:
	case TAS_DEVGRP_GBINFO_QRY_NTF_REQ:
	case TAS_DEVICE_GBINFO_QRY_NTF_REQ:
        return true;
    default:
        return false;
    }
}

bool IsNtfRspType( u16 wMsgId )
{
    switch (wMsgId)
    {
    case TAS_DEVICE_ADD_NTF_RSP:
    case TAS_DEVICE_DEL_NTF_RSP:
    case TAS_DEVICE_MOD_NTF_RSP:
    case TAS_DEVICE_QRY_NTF_RSP:
    case TAS_TVWALL_ADD_NTF_RSP:
    case TAS_TVWALL_DEL_NTF_RSP:
    case TAS_TVWALL_MOD_NTF_RSP:
    case TAS_TVWALL_QRY_NTF_RSP:
    case TAS_DEVICE_MODEL_ADD_NTF_RSP:
    case TAS_DEVICE_MODEL_DEL_NTF_RSP:
    case TAS_DEVICE_MODEL_QRY_NTF_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP:
    case TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP:
    case TAS_CUSER_ADD_NTF_RSP:
    case TAS_CUSER_DEL_NTF_RSP:
    case TAS_CUSER_MOD_NTF_RSP:
    case TAS_CUSER_QRY_NTF_RSP:
    case TAS_CUSER_DEVICE_ADD_NTF_RSP:
    case TAS_CUSER_DEVICE_DEL_NTF_RSP:
    case TAS_CUSER_DEVICE_MOD_NTF_RSP:
    case TAS_CUSER_DEVICE_QRY_NTF_RSP:
    case TAS_DEVICE_GROUP_ADD_NTF_RSP:
    case TAS_DEVICE_GROUP_DEL_NTF_RSP:
    case TAS_DEVICE_GROUP_MOD_NTF_RSP:
    case TAS_DEVICE_GROUP_QRY_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP:
    case TAS_CUSER_GROUP_ADD_NTF_RSP:
    case TAS_CUSER_GROUP_DEL_NTF_RSP:
    case TAS_CUSER_GROUP_MOD_NTF_RSP:
    case TAS_CUSER_GROUP_QRY_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP:
    case TAS_DOMAIN_RELATION_QRY_NTF_RSP:
    case TAS_DOMAIN_RELATION_ADD_NTF_RSP:
    case TAS_DOMAIN_RELATION_DEL_NTF_RSP:
    case TAS_DOMAIN_RELATION_MOD_NTF_RSP:
    case TAS_TACUSER_ADD_NTF_RSP:
    case TAS_TACUSER_DEL_NTF_RSP:
    case TAS_TACUSER_MOD_NTF_RSP:
    case TAS_TACUSER_QRY_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP:
    case FILE_COLLECT_PROGRESS_NTF_RSP:
	case TAS_DEVTREE_QRY_NTF_RSP:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_RSP:
	case TAS_PTZLOCK_QRY_NTF_RSP:
	case TAS_PTZLOCK_ADD_NTF_RSP:
	case TAS_PTZLOCK_DEL_NTF_RSP:
	case TAS_VIDSRC_INFO_QRY_NTF_RSP:
    case TAS_LICENSE_UPDATE_NTF_RSP:
	case TAS_DEVGRP_GBINFO_QRY_NTF_RSP:
	case TAS_DEVICE_GBINFO_QRY_NTF_RSP:
        return true;
    default:
        return false;
    }
}

bool IsRspType( u16 wMsgId )
{
    switch (wMsgId)
    {
    case TAS_LOGIN_RSP:
    case TAS_LOGOUT_RSP:
    case TAS_CU_ONLINE_RSP:
    case TAS_CU_OFFLINE_RSP:
    case TAS_DEVICE_ADD_RSP:
    case TAS_DEVICE_ADD_NTF_RSP:
    case TAS_DEVICE_DEL_RSP:
    case TAS_DEVICE_DEL_NTF_RSP:
    case TAS_DEVICE_MOD_RSP:
    case TAS_DEVICE_MOD_NTF_RSP:
    case TAS_DEVICE_QRY_RSP:
    case TAS_DEVICE_QRY_NTF_RSP:
    case TAS_TVWALL_ADD_RSP:
    case TAS_TVWALL_ADD_NTF_RSP:
    case TAS_TVWALL_DEL_RSP:
    case TAS_TVWALL_DEL_NTF_RSP:
    case TAS_TVWALL_MOD_RSP:
    case TAS_TVWALL_MOD_NTF_RSP:
    case TAS_TVWALL_QRY_RSP:
    case TAS_TVWALL_QRY_NTF_RSP:
    case TAS_DEVICE_MODEL_ADD_RSP:
    case TAS_DEVICE_MODEL_ADD_NTF_RSP:
    case TAS_DEVICE_MODEL_DEL_RSP:
    case TAS_DEVICE_MODEL_DEL_NTF_RSP:
    case TAS_DEVICE_MODEL_QRY_RSP:
    case TAS_DEVICE_MODEL_QRY_NTF_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_GET_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_RSP:
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_NTF_RSP:
    case TAS_DEVICE_MANUFACTURER_ADD_RSP:
    case TAS_DEVICE_MANUFACTURER_DEL_RSP:
    case TAS_DEVICE_MANUFACTURER_QRY_RSP:
    case TAS_DEVICE_MANUFACTURER_QRY_NTF_RSP:
    case TAS_CUSER_ADD_RSP:
    case TAS_CUSER_ADD_NTF_RSP:
    case TAS_CUSER_DEL_RSP:
    case TAS_CUSER_DEL_NTF_RSP:
    case TAS_CUSER_MOD_RSP:
    case TAS_CUSER_MOD_NTF_RSP:
    case TAS_CUSER_QRY_RSP:
    case TAS_CUSER_QRY_NTF_RSP:
    case TAS_CUSER_DEVICE_ADD_RSP:
    case TAS_CUSER_DEVICE_ADD_NTF_RSP:
    case TAS_CUSER_DEVICE_DEL_RSP:
    case TAS_CUSER_DEVICE_DEL_NTF_RSP:
    case TAS_CUSER_DEVICE_MOD_RSP:
    case TAS_CUSER_DEVICE_MOD_NTF_RSP:
    case TAS_CUSER_DEVICE_QRY_RSP:
    case TAS_CUSER_DEVICE_QRY_NTF_RSP:
    case TAS_DEVICE_GROUP_ADD_RSP:
    case TAS_DEVICE_GROUP_ADD_NTF_RSP:
    case TAS_DEVICE_GROUP_DEL_RSP:
    case TAS_DEVICE_GROUP_DEL_NTF_RSP:
    case TAS_DEVICE_GROUP_MOD_RSP:
    case TAS_DEVICE_GROUP_MOD_NTF_RSP:
    case TAS_DEVICE_GROUP_QRY_RSP:
    case TAS_DEVICE_GROUP_QRY_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_ADD_RSP:
    case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_DEL_RSP:
    case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_RSP:
    case TAS_DEVICE_GROUP_DEVICE_QRY_RSP:
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_RSP:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_RSP:
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP:
	case TAS_DEVICE_GROUP_MOD_4GB_RSP:
    case TAS_CUSER_GROUP_ADD_RSP:
    case TAS_CUSER_GROUP_ADD_NTF_RSP:
    case TAS_CUSER_GROUP_DEL_RSP:
    case TAS_CUSER_GROUP_DEL_NTF_RSP:
    case TAS_CUSER_GROUP_MOD_RSP:
    case TAS_CUSER_GROUP_MOD_NTF_RSP:
    case TAS_CUSER_GROUP_QRY_RSP:
    case TAS_CUSER_GROUP_QRY_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_ADD_RSP:
    case TAS_CUSER_GROUP_DEVICE_ADD_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_DEL_RSP:
    case TAS_CUSER_GROUP_DEVICE_DEL_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_MOD_RSP:
    case TAS_CUSER_GROUP_DEVICE_MOD_NTF_RSP:
    case TAS_CUSER_GROUP_DEVICE_QRY_RSP:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_RSP:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP:
    case TAS_LICENSE_UPDATE_RSP:
    case TAS_LICENSE_GET_RSP:
    case TAS_DOMAIN_RELATION_QRY_RSP:
    case TAS_DOMAIN_RELATION_QRY_NTF_RSP:
    case TAS_DOMAIN_RELATION_ADD_NTF_RSP:
    case TAS_DOMAIN_RELATION_DEL_NTF_RSP:
    case TAS_DOMAIN_RELATION_MOD_NTF_RSP:
    case TAS_DOMAIN_NAME_GET_RSP:
    case TAS_CONFIG_TAS_SET_RSP:
    case TAS_CONFIG_TAS_GET_RSP:
    case TAS_CTRL_TAS_RSP:
    case TAS_TACUSER_ADD_RSP:
    case TAS_TACUSER_ADD_NTF_RSP:
    case TAS_TACUSER_DEL_RSP:
    case TAS_TACUSER_DEL_NTF_RSP:
    case TAS_TACUSER_MOD_RSP:
    case TAS_TACUSER_MOD_NTF_RSP:
    case TAS_TACUSER_QRY_RSP:
    case TAS_TACUSER_QRY_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_ADD_RSP:
    case TAS_TEMP_AUTHORIZATION_ADD_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_DEL_RSP:
    case TAS_TEMP_AUTHORIZATION_DEL_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_MOD_RSP:
    case TAS_TEMP_AUTHORIZATION_MOD_NTF_RSP:
    case TAS_TEMP_AUTHORIZATION_QRY_RSP:
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_RSP:
    case TAS_SYNC_TAS_DATA_RSP:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_RSP:
    case FILE_EXPORT_RSP:
    case FILE_EXPORT_ACK:
    case FILE_COLLECT_PROGRESS_NTF_RSP:
    case OSP_SIP_BYE_REQ:
    case OSP_SIP_BYE_RSP:
	case TAS_CUSER_DEVICE_GROUP_ADD_RSP:
	case TAS_CUSER_DEVICE_GROUP_DEL_RSP:
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP:
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_RSP:
	case TAS_TRANSACTION_PROGRESS_GET_RSP:
    case TAS_CUSER_DATA_VERSION_GET_RSP:
	case TAS_DEVTREE_ADD_RSP: 
	case TAS_DEVTREE_MOD_RSP: 
	case TAS_DEVTREE_DEL_RSP: 
	case TAS_DEVTREE_QRY_RSP: 
	case TAS_DEVTREE_QRY_NTF_RSP:
	case TAS_DEVTREE_DEVGROUP_ASSIGN_RSP:
	case TAS_DEVICE_GROUPPATH_QRY_RSP:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_RSP:
	case TAS_DEV_CAPINFO_MOD_RSP: 
	case TAS_PTZLOCK_ADD_RSP: 
	case TAS_PTZLOCK_DEL_RSP:
	case TAS_PTZLOCK_QRY_RSP: 
	case TAS_PTZLOCK_QRY_NTF_RSP: 
	case TAS_PTZLOCK_ADD_NTF_RSP:
	case TAS_PTZLOCK_DEL_NTF_RSP:
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP:
	case TAS_VIDSRC_INFO_QRY_RSP:
	case TAS_VIDSRC_INFO_QRY_NTF_RSP:
    case TAS_LICENSE_UPDATE_NTF_RSP:
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP:
	case TAS_DEVICE_MATCH_GBID_RSP:
	case TAS_DEVGRP_GBINFO_QRY_RSP:
	case TAS_DEVICE_GBINFO_QRY_RSP:
	case TAS_DEVGRP_GBINFO_QRY_NTF_RSP:
	case TAS_DEVICE_GBINFO_QRY_NTF_RSP:
	case TAS_DEVICE_GBID_ERROR_GET_RSP:
	case GBS_DEV_GBID_BATCH_GET_RSP:
	case TAS_DEVICE_GROUP_DEVICE_ORDER_RSP:
        return true;
    default:
        return false;
    }
    return true;
}

bool IsQryReqType( u16 wMsgId )
{
    switch (wMsgId)
    {
    case TAS_DEVICE_QRY_REQ:
    case TAS_TVWALL_QRY_REQ:
    case TAS_DEVICE_MODEL_QRY_REQ:
    case TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ:
    case TAS_DEVICE_MANUFACTURER_QRY_REQ:
    case TAS_CUSER_QRY_REQ:
    case TAS_CUSER_DEVICE_QRY_REQ:
    case TAS_DEVICE_GROUP_QRY_REQ:
    case TAS_DEVICE_GROUP_DEVICE_QRY_REQ:
    case TAS_CUSER_GROUP_QRY_REQ:
    case TAS_CUSER_GROUP_DEVICE_QRY_REQ:
    case TAS_LICENSE_GET_REQ:
    case TAS_DOMAIN_RELATION_QRY_REQ:
    case TAS_TACUSER_QRY_REQ:
    case TAS_TEMP_AUTHORIZATION_QRY_REQ:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ:
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ:
	case TAS_DEVTREE_QRY_REQ:
	case TAS_DEVICE_GROUPPATH_QRY_REQ:
	case TAS_PTZLOCK_QRY_REQ:
	case TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ:
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ:
	case TAS_VIDSRC_INFO_QRY_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ:
	case TAS_DEVGRP_GBINFO_QRY_REQ:
	case TAS_DEVICE_GBINFO_QRY_REQ:
        return true;
    default:
        return false;
    }
}


void TelePrint( const s8* cstrFormat, ... )
{
    va_list parmList;
    va_start(parmList, cstrFormat);

    s8 szBuff[1024];
    s32 nWriteBytes;
#ifdef _LINUX_
    nWriteBytes = vsnprintf(szBuff, sizeof(szBuff)-1, cstrFormat, parmList);
#else
    nWriteBytes = vsprintf(szBuff, cstrFormat, parmList);
#endif // _LINUX_
    va_end(parmList);

    if ((0 <= nWriteBytes) && ((u32)nWriteBytes+1 < sizeof(szBuff)))
    {
        OspPrintf(TRUE, FALSE, "%s\n", szBuff);
    }
    else if (-1 == nWriteBytes)
    {
        OspPrintf(TRUE, FALSE, "%s\n", "vsprintf error in TelePrint...");
    }
    else
    {
        OspPrintf(TRUE, FALSE, "%s\n", "the print content is too long in TelePrint...");
    }
}

bool ValidateMsg( CMessage * const pcMsg )
{
    // 校验TAS外部消息（外部消息是不可信的）
    if (TAS_EXTERN_MSG_BEGIN <= pcMsg->event && pcMsg->event <= TAS_EXTERN_MSG_END)
    {
        // TAS的外部消息都有消息体
        if (NULL == pcMsg->content) 
        {
            return false;
        }

        return true;
    }

    // 校验TAS内部消息(可信的)
    if (USER_INNER_MSG_BEGIN <= pcMsg->event)
    {
        return true;
    }

    // OspSipEvent消息(可信的)
    if (OSP_SIP_EV_BEGIN <= pcMsg->event && pcMsg->event <= OSP_SIP_EV_END)
    {
        return true;
    }
    
    // Osp库的消息(可信的)
    if (pcMsg->event < OSP_USEREVENT_BASE)
    {
        return true;
    }
    // OspSip库的消息(可信的)
    if (OSP_SIP_EV_BEGIN <= pcMsg->event && pcMsg->event <= OSP_SIP_EV_END)
    {
        return true;
    }

    // 公共消息
    if (CMS_EVENT_BEGIN <= pcMsg->event && pcMsg->event <= CMS_EVENT_END)
    {
        return true;
    }

    // 共用协议，比如通用的文件下载协议
    if (CMS_EVENT_BEGIN <= pcMsg->event && pcMsg->event <= CMS_EVENT_END)
    {
        return true;
    }
	if (GBS_EVENT_BEGIN <=pcMsg->event && pcMsg->event <= GBS_EVENT_END)
	{
		return true;
	}

	//GBS部分协议需要支持
	if (GBS_EXTERN_MSG_BEGIN<=pcMsg->event && pcMsg->event <= GBS_EXTERN_MSG_END)
	{
		return true;
	}

    // 未知消息
    return false;
}

void FindSubdomainList( const vector<CDomainRelationInfo> &cInput, const string &strParentDomainId, vector<CDomainRelationInfo> &cOutput )
{
    cOutput.clear();

    // 从cInput中查找子域，存放于cOutput中
    u32 i;
    for (i = 0; i < cInput.size(); ++i)
    {
        if (cInput[i].GetParentDomainID() == strParentDomainId)
        {
            cOutput.push_back(cInput[i]);
        }
    }

    // 以cOutput中的项目为父域从cInput中查找子域存储于cOutput中
    for (i = 0; i < cOutput.size(); ++i)
    {
        for (u32 j = 0; j < cInput.size(); ++j)
        {
            if (cOutput[i].GetDomainID() == cInput[j].GetParentDomainID())
            {
                cOutput.push_back(cInput[j]);
            }
        }
    }
}

void GenerateEncodeCapPrivilegeInfo( const CDeviceModelInfo &cModelInfo, const vector<TDeviceCapPrivilege> &cPrivIn, vector<TDeviceCapPrivilege> &cPrivOut )
{
    CMS_ASSERT(cModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER);

    u32 dwVideosourceNum = 0;
    u32 dwEncodeChnNum = 0;

    for (u32 i = 0; i < cModelInfo.GetDeviceCapInfo().size(); ++i)
    {
        if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_VIDEOSOURCE)
        {
            dwVideosourceNum = cModelInfo.GetDeviceCapInfo()[i].num;
        }
        else if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_ENCODE_CHANNEL)
        {
            dwEncodeChnNum = cModelInfo.GetDeviceCapInfo()[i].num;
        }
    }

    cPrivOut = cPrivIn;

    TDeviceCapPrivilege tCapPrivi;
    tCapPrivi.id = DEVICE_CAP_ENCODE_CHANNEL;
    map<int, CBitsContainer> privilege;

    for (u32 i = 0; i < cPrivIn.size(); ++i)
    {
        if (DEVICE_CAP_VIDEOSOURCE == cPrivIn[i].id)
        {
            /* 自动构造编码通道权限(包括辅流) */
            const map<int, CBitsContainer> &privileges = cPrivIn[i].privileges;

            for (map<int, CBitsContainer>::const_iterator it = privileges.begin(); it != privileges.end(); ++it)
            {
                privilege.clear();
                privilege.insert(map<int, CBitsContainer>::value_type(it->first, it->second));

                if (it->first + dwVideosourceNum < dwEncodeChnNum)
                {
                    privilege.insert(map<int, CBitsContainer>::value_type(it->first + dwVideosourceNum, it->second));
                }

				tCapPrivi.privileges = privilege;
				cPrivOut.push_back(tCapPrivi);
            }

			break;
        }
    }
}

void GenerateEncodeCapIndexInfo( const CDeviceModelInfo &cModelInfo, const vector<TDeviceCapIndexs> &cPrivIn, vector<TDeviceCapIndexs> &cPrivOut )
{
    CMS_ASSERT(cModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER);

    u32 dwVideosourceNum = 0;
    u32 dwEncodeChnNum = 0;

    for (u32 i = 0; i < cModelInfo.GetDeviceCapInfo().size(); ++i)
    {
        if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_VIDEOSOURCE)
        {
            dwVideosourceNum = cModelInfo.GetDeviceCapInfo()[i].num;
        }
        else if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_ENCODE_CHANNEL)
        {
            dwEncodeChnNum = cModelInfo.GetDeviceCapInfo()[i].num;
        }
    }

    cPrivOut = cPrivIn;

    TDeviceCapIndexs tCapIndex;
    tCapIndex.id = DEVICE_CAP_ENCODE_CHANNEL;

    for (u32 i = 0; i < cPrivIn.size(); ++i)
    {
        if (DEVICE_CAP_VIDEOSOURCE == cPrivIn[i].id)
        {
            const vector<int> &indexs = cPrivIn[i].indexs;

            for (vector<int>::const_iterator it = indexs.begin(); it != indexs.end(); ++it)
            {
                tCapIndex.indexs.clear();
                tCapIndex.indexs.push_back(*it);

                if (*it + dwVideosourceNum < dwEncodeChnNum)
                {
                    tCapIndex.indexs.push_back(*it + dwVideosourceNum);
                }
            }

			cPrivOut.push_back(tCapIndex);

			break;
        }
    }
}

void AddEncodeIndexInfo(const CDeviceModelInfo &cModelInfo, vector<TDeviceCapIndexs> &cIndexs)
{
	CMS_ASSERT(cModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER);

	for (vector<TDeviceCapIndexs>::const_iterator it = cIndexs.begin(); cIndexs.end() != it; ++it)
	{
		if (DEVICE_CAP_ENCODE_CHANNEL == it->id && !it->indexs.empty())	// 已经包含编码通道信息，无需再次添加
		{
			GLOBELOG(PROGRESS_LEV, "【AddEncodeIndexInfo】原设备能力中已经包含编码通道信息，无需再次添加!\n");
			return;
		}
	}

	u32 dwVideosourceNum = 0;
	u32 dwEncodeChnNum = 0;

	for (u32 i = 0; i < cModelInfo.GetDeviceCapInfo().size(); ++i)
	{
		if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_VIDEOSOURCE)
		{
			dwVideosourceNum = cModelInfo.GetDeviceCapInfo()[i].num;
		}
		else if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_ENCODE_CHANNEL)
		{
			dwEncodeChnNum = cModelInfo.GetDeviceCapInfo()[i].num;
		}
	}

	TDeviceCapIndexs tCapIndex;
	tCapIndex.id = DEVICE_CAP_ENCODE_CHANNEL;

	for (u32 i = 0; i < cIndexs.size(); ++i)
	{
		if (DEVICE_CAP_VIDEOSOURCE == cIndexs[i].id)
		{
			tCapIndex.indexs.clear();

			const vector<int> &indexs = cIndexs[i].indexs;
			for (vector<int>::const_iterator it = indexs.begin(); it != indexs.end(); ++it)
			{
				tCapIndex.indexs.push_back(*it);

				if (*it + dwVideosourceNum < dwEncodeChnNum)
				{
					tCapIndex.indexs.push_back(*it + dwVideosourceNum);
				}
			}

			cIndexs.push_back(tCapIndex);

			break;
		}
	}
}

void AddEncodePrivilegeInfo(const CDeviceModelInfo &cModelInfo, vector<TDeviceCapPrivilege> &cPri)
{
	CMS_ASSERT(cModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER);

	for (vector<TDeviceCapPrivilege>::const_iterator it = cPri.begin(); cPri.end() != it; ++it)
	{
		if (DEVICE_CAP_ENCODE_CHANNEL == it->id && !it->privileges.empty())	// 已经包含编码通道信息，无需再次添加
		{
			GLOBELOG(PROGRESS_LEV, "【AddEncodePrivilegeInfo】原设备能力中已经包含编码通道信息，无需再次添加!\n");
			return;
		}
	}

	u32 dwVideosourceNum = 0;
	u32 dwEncodeChnNum = 0;

	for (u32 i = 0; i < cModelInfo.GetDeviceCapInfo().size(); ++i)
	{
		if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_VIDEOSOURCE)
		{
			dwVideosourceNum = cModelInfo.GetDeviceCapInfo()[i].num;
		}
		else if (cModelInfo.GetDeviceCapInfo()[i].type == DEVICE_CAP_ENCODE_CHANNEL)
		{
			dwEncodeChnNum = cModelInfo.GetDeviceCapInfo()[i].num;
		}
	}

	TDeviceCapPrivilege tCapPri;
	tCapPri.id = DEVICE_CAP_ENCODE_CHANNEL;

	for (u32 i = 0; i < cPri.size(); ++i)
	{
		if (DEVICE_CAP_VIDEOSOURCE == cPri[i].id)
		{
			tCapPri.privileges.clear();

			const map<int, CBitsContainer> &cBits = cPri[i].privileges;
			for (map<int, CBitsContainer>::const_iterator it = cBits.begin(); it != cBits.end(); ++it)
			{
				tCapPri.privileges.insert(map<int, CBitsContainer>::value_type(it->first, it->second));

				if (it->first + dwVideosourceNum < dwEncodeChnNum)
				{
					tCapPri.privileges.insert(map<int, CBitsContainer>::value_type(it->first + dwVideosourceNum, it->second));
				}
			}

			cPri.push_back(tCapPri);

			break;
		}
	}
}

bool RelaceSessionId( string &strMsgContent, const string &strNewSessId )
{
    string::size_type pos1, pos2;

    pos1 = strMsgContent.find("<session>");
    if (string::npos != pos1)
    {
		pos1 += 9; // strlen("<session>");

		pos2 = strMsgContent.find("</session>", pos1);
		if (string::npos == pos2)
		{
			CMS_ASSERT(false && "</session> not found");
			return false;
		}

		strMsgContent.replace(pos1, pos2-pos1, strNewSessId.c_str());
		return true; 
    }
	else
	{
		pos1 = strMsgContent.find("</seqNum>");
		if (string::npos != pos1)
		{
			pos1 += 9; // strlen("</seqNum>");

			string strSession = "<session>" + strNewSessId + "</session>";

			strMsgContent.insert(pos1, strSession);

			return true;
		}
		else
		{
			CMS_ASSERT(false && "</seqNum> not found");
			return false;
		}
	}
}

bool ValidateDomainRelationInfo4Add( const vector<CDomainRelationInfo> &cOrigin, const CDomainRelationInfo &tChangeRelation )
{
    for (vector<CDomainRelationInfo>::const_iterator it = cOrigin.begin(); it != cOrigin.end(); ++it)
    {
        if (it->GetDomainID() == tChangeRelation.GetDomainID())
        {
            return false;
        }
    }
    vector<CDomainRelationInfo> cDomainRelations = cOrigin;
    cDomainRelations.push_back(tChangeRelation);

    return ValidateDomainRelationInfos(cDomainRelations);
}

bool ValidateDomainRelationInfo4Mod( const vector<CDomainRelationInfo> &cOrigin, const CDomainRelationInfo &tChangeRelation )
{
    vector<CDomainRelationInfo> cDomainRelations = cOrigin;

    vector<CDomainRelationInfo>::iterator it;
    for (it = cDomainRelations.begin(); it != cDomainRelations.end(); ++it)
    {
        if (it->GetDomainID() == tChangeRelation.GetDomainID())
        {
            *it = tChangeRelation;
            break;
        }
    }
    if (it == cOrigin.end())
    {
        return false;
    }

    return ValidateDomainRelationInfos(cDomainRelations);
}

bool ValidateDomainRelationInfos( const vector<CDomainRelationInfo> &cDomainRelations )
{
    // 判断同名
    for (vector<CDomainRelationInfo>::const_iterator it = cDomainRelations.begin(); 
        it != cDomainRelations.end(); ++it)
    {
        for (vector<CDomainRelationInfo>::const_iterator it2 = it+1; it2 != cDomainRelations.end(); ++it2)
        {
            if (it->GetDomainName() == it2->GetDomainName())
            {
                return false;
            }
        }
    }

    // 判断回环 算法：查找当前条目的父亲以及父亲的父亲一直找下去，如果查过过程中出现了重复条目即回环了
    vector<vector<CDomainRelationInfo>::const_iterator> vecTemp;
    bool bFoundParent;
    for (vector<CDomainRelationInfo>::const_iterator it = cDomainRelations.begin(); 
        it != cDomainRelations.end(); ++it)
    {
        vecTemp.clear();
        vecTemp.push_back(it);
        
        do 
        {
            bFoundParent = false;

            for (vector<CDomainRelationInfo>::const_iterator it2 = cDomainRelations.begin(); 
                it2 != cDomainRelations.end(); ++it2)
            {
                if (vecTemp.back()->GetParentDomainID() == it2->GetDomainID())
                {
                    for (vector<vector<CDomainRelationInfo>::const_iterator>::const_iterator itTmp = vecTemp.begin();
                        itTmp != vecTemp.end(); ++itTmp)
                    {
                        if (*itTmp == it2)
                        {
                            return false;
                        }
                    }

                    vecTemp.push_back(it2);
                    bFoundParent = true;
                    break;
                }
            }
        } while (bFoundParent);
    }

    return true;
}

bool IsDomainIdExists(const vector<CDomainRelationInfo> &cInput, const string &strDomainId)
{
    for (vector<CDomainRelationInfo>::const_iterator it = cInput.begin(); it != cInput.end(); ++it)
    {
        if (it->GetDomainID() == strDomainId)
        {
            return true;
        }
    }

    return false;
}

bool IsAllDigit( const string& str )
{
	for (int i=0;i<str.length();i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}

	return true;
}

string CutDevGrpGbId(const string &strOrgName )
{
	/*条件：
	 带国标编码的格式：上海市-3121000000
	 1、长度大于等于12
	 2、倒数第11位为‘-’
	 3、最后10位为数字
	*/
	string newName=strOrgName;

	size_t nStrlen=strOrgName.length();
	if (nStrlen>=12)
	{
		if (strOrgName.substr(nStrlen-11,1)=="-")
		{
				newName=strOrgName.substr(0,nStrlen-11);
				GLOBELOG( EVENT_LEV, "CutDevGrpGbId:[%s] after:[%s]!\n",strOrgName.c_str(),newName.c_str());
		}
	}

	return newName;
}


bool GetDirFileList(char *dir,vector<string>& vecFileNameList)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        return false;
    }

    char szBuf[1024];
    size_t dirLen=strlen(dir);
    bool bNeedAppendSplit = (*(dir+dirLen-1)=='/')?false:true;

    while((entry = readdir(dp)) != NULL) {
        if (strlen(entry->d_name)+dirLen+3 >= sizeof(szBuf))
        {
            closedir(dp);
            return false;
        }

        szBuf[0]='\0';
        strcat(szBuf, dir);
        if (bNeedAppendSplit)
        {
            strcat(szBuf, "/");
        }
        strcat(szBuf, entry->d_name);
        lstat(szBuf,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) 
        {
            continue;
        }
        else
        {
            vecFileNameList.push_back(entry->d_name);
        }
    }
    closedir(dp);
    return true;
}


string GetMachineTypeByPUMaxNum(int dwPuMaxNum ) 
{
    string strMachine = "";
    if (dwPuMaxNum >= MACHINE_TYPE_2901_G2_S_MAX_PU_NUM && dwPuMaxNum < MACHINE_TYPE_2901_G2_M_MAX_PU_NUM)
    {
        strMachine = MACHINE_TYPE_2901_G2_S;
    }
    else if (dwPuMaxNum >= MACHINE_TYPE_2901_G2_M_MAX_PU_NUM && dwPuMaxNum < MACHINE_TYPE_2901_G2_L_MAX_PU_NUM)
    {
        strMachine = MACHINE_TYPE_2901_G2_M;
    }
    else if (dwPuMaxNum >= MACHINE_TYPE_2901_G2_L_MAX_PU_NUM && dwPuMaxNum < MACHINE_TYPE_2901_G2_H_MAX_PU_NUM)
    {
        strMachine = MACHINE_TYPE_2901_G2_L;
    }
    else if (dwPuMaxNum >= MACHINE_TYPE_2901_G2_H_MAX_PU_NUM)
    {
        strMachine = MACHINE_TYPE_2901_G2_H;
    }
    return strMachine;

}

bool WriteTableDataToPushXmlFile(const string& strTreeId,const TPushXmlTreeInfo& tPushXmlTreeInfo)
{
	string filepath="/dev/shm/tas/"+strTreeId +".xml";
	remove(filepath.c_str());
	string strPushXml;
	s8 achTreeBuf[256];
	sprintf(achTreeBuf,"  <treeid>%s</treeid>\r\n",tPushXmlTreeInfo.treeid.c_str());
	strPushXml +=  string(achTreeBuf);
	sprintf(achTreeBuf,"  <treever>%s</treever>\r\n",tPushXmlTreeInfo.treever.c_str());
	strPushXml += string(achTreeBuf);
	u32 dwMaxCivilcodeLen = 2*GetMaxCivilcodeLev();
	s8 achGrpBuf[1024];
	s8 achGrpDevBuf[1024];
	for (vector<TPushXmlGrpInfo>::const_iterator it = tPushXmlTreeInfo.grpinfo.begin();it != tPushXmlTreeInfo.grpinfo.end();it++)
	{
		if (it->groupGbid.size() <= dwMaxCivilcodeLen) //行政区划分组
		{
			sprintf(achGrpBuf,
				"  <devgrp>\r\n"
				"    <kdid>%s</kdid>\r\n"
				"    <name>%s</name>\r\n"
				"    <gbid>%s</gbid>\r\n"
				"    <parentid>%s</parentid>\r\n"
				"  </devgrp>\r\n",
				it->groupId.c_str(),
				it->groupName.c_str(),
				it->groupGbid.c_str(),
				it->parentId.c_str());
		}
		else //215、216
		{
			sprintf(achGrpBuf,
				"  <devgrp2>\r\n"
				"    <kdid>%s</kdid>\r\n"
				"    <name>%s</name>\r\n"
				"    <gbid>%s</gbid>\r\n"
				"    <parentid>%s</parentid>\r\n"
				"  </devgrp2>\r\n",
				it->groupId.c_str(),
				it->groupName.c_str(),
				it->groupGbid.c_str(),
				it->parentId.c_str());
		}
		
		strPushXml += string(achGrpBuf);
	}
	for (vector<TPushXmlGrpDevInfo>::const_iterator it = tPushXmlTreeInfo.grpdevinfo.begin();it != tPushXmlTreeInfo.grpdevinfo.end();it++)
	{
		sprintf(achGrpDevBuf,
			"  <dev>\r\n"
			"    <kdid>%s</kdid>\r\n"
			"    <name>%s</name>\r\n"
			"    <oldgbid>%s</oldgbid>\r\n"
			"    <gbid>%s</gbid>\r\n"
			"    <chnid>%d</chnid>\r\n"
			"    <primary>%d</primary>\r\n"
			"    <civilcode>%s</civilcode>\r\n"
			"    <parentid>%s</parentid>\r\n"
			"  </dev>\r\n",
			it->devId.c_str(),
			it->devName.c_str(),
			it->devOldGbid.c_str(),
			it->devNewGbid.c_str(),
			it->chnid,
			it->primary,
			it->civilcode.c_str(),
			it->parentId.c_str());
		strPushXml += string(achGrpDevBuf);
	}

	ofstream outfile;
	outfile.open(filepath.c_str(),ios::app);
	if (!outfile.is_open())
	{
		GLOBELOG(CRITICAL_LEV, "open file[%s] fail, can't write xml to the file !!!\n", filepath.c_str());
	}
	else
	{
		outfile<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n";
		outfile<<"<cfg>\r\n";
		outfile<<"  <enable>1</enable>\r\n";
		outfile<<strPushXml;
		outfile<<"</cfg>\r\n";
		outfile.close();
		GLOBELOG(EVENT_LEV, "pushxml write successed! start mv to[%s]\n", filepath.c_str());
	}

	return true;
}