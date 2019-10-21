#ifndef STREAM_DECENTRATION_ADAPTER_H
#define STREAM_DECENTRATION_ADAPTER_H
   
#include "cms/cms_errorcode.h"
#include "cms/ospsip/ospsip.h"
#include "sdk/redissdk/switch_struct.h"

class CRedisSdk;

struct TVtduResource
{
    TVtduResource()
    {
        err = CMS_SUCCESS;
        bCheckFlow = true;
    }

    std::string         resId;
    TSipURI             uri;   
    ECMS_ERRORCODE      err;
    TRedisSwitchType    switchType;
    TRedisSwitchAgent   tSrcAgent;
    TRedisSwitchAgent   tDstAgent;
    bool                bCheckFlow;
};

class CStreamDecentrationAdapter
{
public:
    CStreamDecentrationAdapter(){}
    ~CStreamDecentrationAdapter(){}

public:
    static bool FindPuiUri( TSipURI &tDstUri, const TSipURI &tDevUri,       const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode = NULL );
    static bool FindPuiUri( TSipURI &tDstUri, const std::string &strDevUri, const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode = NULL );
    static bool FindPuiUri( TSipURI &tDstUri, const std::string &strUser, const std::string &strDomain, const TSipURI &tDefaultUri, ECMS_ERRORCODE *peErrCode = NULL );

    static bool FindRcsUri( TSipURI &tDstUri, const string &strDomainName, bool bEnableRedisSelect = true );
        
    static bool FindCuiUri( TSipURI &tDstUri, const TSipURI &tDevUri,       const TSipURI &tDefaultUri );

    static bool SelectVtduResouce( TVtduResource &tVtduRes, const TSipURI &tDefaultUri );
    static bool ReleaseVtduResource( const std::string &resourceId ) ;

    static bool RegisterCu(    const std::string &strCuiId, const std::string &strSession, const std::string &strUserName,  
		const std::string &strAddress, const std::string &strCuiIp );
    static bool UnregisterCu(  const std::string &strCuiId, const std::string &strSession );

    static bool RegisterPu(    const std::string &strPuiId, const std::string &strDevId );
    static bool UnregisterPu(  const std::string &strPuiId, const std::string &strDevId );
    
    static std::string SdpSessionNameToRedisSwitchType( const string &strSessionName );

public:
    static void SetRedisSdk( CRedisSdk *pcSdk )
    {
        m_pcSdk = pcSdk;
    }

private:
    static CRedisSdk *m_pcSdk;
};


#endif