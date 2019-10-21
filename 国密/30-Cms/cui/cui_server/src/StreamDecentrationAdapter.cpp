#include "StreamDecentrationAdapter.h"
#include "sdk/redissdk/redissdk.h"
#include "cms/commonconfig/cmcfg.h"
//允许使用CRedisSdk 或子类实例
CRedisSdk* CStreamDecentrationAdapter::m_pcSdk = NULL;


bool CStreamDecentrationAdapter::FindPuiUri( TSipURI &tDstUri, const TSipURI &tDevUri, const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode )
{
    if( m_pcSdk==NULL )
    {
        return false;
    }
    //modified by zzc @ 2017-07-07
    //出于码流去中心化的需要，信令直接选一个pui    
    std::string strPuiSvrId;
    std::string strPuiSvrDomain;
    ECMS_ERRORCODE errResult = m_pcSdk->SelectPui( tDevUri.GetUser(), strPuiSvrId, strPuiSvrDomain );
    
    TSipURI tPuiUri(strPuiSvrId, strPuiSvrDomain );
    
    bool bFound =   errResult == CMS_SUCCESS 
                && tPuiUri.IsValidURI();

    tDstUri = bFound ? tPuiUri : tDefaultUri;

    if( peErrCode != NULL )
    {
        *peErrCode = errResult;
    }

    GLOBELOG( EVENT_LEV, "SelectPui [%s], errcode [%d], uri [%s], return [%s] now \n",
        errResult == CMS_SUCCESS ? "successful": "failed", 
        int(errResult), tPuiUri.GetURIString().c_str(), tDstUri.GetURIString().c_str() );

    return bFound;
}

bool CStreamDecentrationAdapter::FindPuiUri( TSipURI &tDstUri, const std::string &strDevUri, const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode )
{
    TSipURI tPuDevUri( strDevUri );
    return FindPuiUri( tDstUri, tPuDevUri, tDefaultUri, peErrCode );
}

bool CStreamDecentrationAdapter::FindPuiUri( TSipURI &tDstUri, const std::string &strUser, const std::string &strDomain, const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode )
{
    TSipURI tPuDevUri( strUser, strDomain );
    return FindPuiUri( tDstUri, tPuDevUri, tDefaultUri, peErrCode );
}

bool CStreamDecentrationAdapter::SelectVtduResouce( TVtduResource &tVtduRes, const TSipURI &tDefaultUri )
{
    if( m_pcSdk==NULL )
    {
        tVtduRes.uri = tDefaultUri;
        return false;
    }

    if( tVtduRes.bCheckFlow )
    {
        TRedisSwitchFlow tFlow = m_pcSdk->GetSwitchFlow( tVtduRes.switchType, tVtduRes.tSrcAgent, tVtduRes.tDstAgent );
        if( tFlow == RedisSwitchFlow_Cmu )
        {
            return false;
        }
    }
    std::string strUser;
    std::string strDomain;

    //GLOBELOG( EVENT_LEV, "select vtdu, moduleType:[%s], strId:[%s], channel: [%d] \n",  
    //    tSwitchAgent.tId.tModuleType.c_str(), tSwitchAgent.tId.strId.c_str(), tSwitchAgent.nChannel );
    tVtduRes.err = m_pcSdk->SelectVtdu( tVtduRes.switchType, tVtduRes.tSrcAgent, tVtduRes.tDstAgent, strUser, strDomain, tVtduRes.resId );
    tVtduRes.uri.SetUser( strUser );
    tVtduRes.uri.SetDomain( strDomain );

    bool bSuccessful = tVtduRes.err == CMS_SUCCESS ;

    if( !bSuccessful )
    {
        GLOBELOG( EVENT_LEV, "select vtdu [%s]: errCode [%d], uri [%s], resId [%s], return uri [%s]  \n",
            tVtduRes.err == CMS_SUCCESS ? "successful": "failed" ,
            tVtduRes.err, tVtduRes.uri.GetURIString().c_str(), tVtduRes.resId.c_str(), tDefaultUri.GetURIString().c_str() );
        tVtduRes.uri = tDefaultUri;
    }
    else
    {
        GLOBELOG( EVENT_LEV, "select vtdu [%s]: errCode [%d], uri [%s], resId [%s],  \n", 
            tVtduRes.err == CMS_SUCCESS ? "successful": "failed" ,
            tVtduRes.err, tVtduRes.uri.GetURIString().c_str(), tVtduRes.resId.c_str() );
    }

    return tVtduRes.err == CMS_SUCCESS;
}


bool CStreamDecentrationAdapter::FindRcsUri( TSipURI &tDstUri, const string &strDomainName, bool bEnableRedisSelect )
{
    bool bSuccessful = true;

    string strDstDomain = strDomainName.empty() ? GetDomainName() : strDomainName;

    string strId;
    string strDomain;
    
    bool bIsDstLocalDomain = strDstDomain == GetDomainName();
    bool bRedisSel = bEnableRedisSelect && bIsDstLocalDomain;
    //非本域直接 发送不再选
    //if(    !bRedisSel
    //    || m_pcSdk == NULL
    //    || CMS_SUCCESS != m_pcSdk->SelectRcs( strId, strDomain ) )
    {
        strId       = DEV_TYPE_RCS;
        strDomain   = strDstDomain;
        bSuccessful = !bRedisSel;
    }
    tDstUri.SetUser  ( strId     );
    tDstUri.SetDomain( strDomain );

    GLOBELOG(EVENT_LEV, "查找rcs uri, 是否采用redis[%d], 结果[%d], uri[%s] \n", 
        bRedisSel, bSuccessful, tDstUri.GetURIString().c_str() );

    return bSuccessful;
}


bool CStreamDecentrationAdapter::ReleaseVtduResource( const std::string &resourceId ) 
{
    if( m_pcSdk==NULL )
    {
        return false;
    }
    ECMS_ERRORCODE err = m_pcSdk->ReleaseVtduResource( resourceId );
    if( err != CMS_SUCCESS )
    {
        return false;
    }

    return true;
}



bool CStreamDecentrationAdapter::RegisterCu(    const std::string &strCuiId,  
                                                const std::string &strSession, 
                                                const std::string &strUserName, 
                                                const std::string &strAddress,
												const std::string &strCuiIp )
{
    if( m_pcSdk==NULL )
    {
        return false;
    }

    TRedisCuAgent tCuAgent;
    tCuAgent.strCuiId = strCuiId;
    tCuAgent.strSession = strSession;
    tCuAgent.strAddress = strAddress;
	tCuAgent.strUserName = strUserName;
	tCuAgent.strCuiIp = strCuiIp;

    ECMS_ERRORCODE err = m_pcSdk->CuOnline( strCuiId, tCuAgent );
    if( err != CMS_SUCCESS )
    {
        return false;
    }

    return true;
}


bool CStreamDecentrationAdapter::UnregisterCu(const std::string &strCuiId, const std::string &strSession )
{
    if( m_pcSdk==NULL )
    {
        return false;
    }

    ECMS_ERRORCODE err = m_pcSdk->CuOffline( strCuiId, strSession );
    if( err != CMS_SUCCESS )
    {
        return false;
    }
}


bool CStreamDecentrationAdapter::RegisterPu(  const std::string &strPuiId, const std::string &strDevId )
{
    if( m_pcSdk==NULL )
    {
        return false;
    }
    
    return  CMS_SUCCESS == m_pcSdk->PuOnline( strPuiId, strDevId );
}


bool CStreamDecentrationAdapter::UnregisterPu(  const std::string &strPuiId, const std::string &strDevId )
{
    if( m_pcSdk==NULL )
    {
        return false;
    }
    
    return CMS_SUCCESS == m_pcSdk->PuOffline( strPuiId, strDevId );
}

bool CStreamDecentrationAdapter::FindCuiUri( TSipURI &tDstUri, const TSipURI &tCuUri, const TSipURI &tDefaultUri )
{
    if( m_pcSdk == NULL )
    {
        return false;
    }
    string strCuiId;
    string strDomain;
    ECMS_ERRORCODE err = m_pcSdk->SelectCui( tCuUri.GetUser(), strCuiId, strDomain);
    if( err!=CMS_SUCCESS)
    {
        return false;
    }
    tDstUri.SetUser  ( strCuiId );
    tDstUri.SetDomain( strDomain );
    return true;
}


std::string CStreamDecentrationAdapter::SdpSessionNameToRedisSwitchType( const string &strSessionName )
{
    if( strSessionName == SDP_SESSION_PLAY )
    {
        return RedisSwitchType_Play;
    }
    if( strSessionName == SDP_SESSION_PLAYBACK )
    {
        return RedisSwitchType_Playback;
    }
    if( strSessionName == SDP_SESSION_AUDIOCALL )
    {
        return RedisSwitchType_Audiocall;
    }
    if( strSessionName == SDP_SESSION_DOWNLOAD )
    {
        return RedisSwitchType_Download;
    }
    
    return "";
}



