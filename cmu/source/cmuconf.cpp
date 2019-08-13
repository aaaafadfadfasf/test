
#include "cms/ospext/osplog.h"
#include "cms/commonconfig/cmcfg.h"
#include "kdvsys.h"
#include "cmuconf.h"
#include "cms/cmu/cmu_struct.h"

CCmuConf g_cCmuConf;
extern TMappingTable g_tMappingTable;

CCmuConf::CCmuConf()
{
    m_bSwitchFollowTopo = false;
    m_nDirectTransMode = 0;
    m_bEnablePunch = true;
    m_nMaxAcceptPu = DEFAULT_MAX_ACCEPT_PU;
    m_nMaxAcceptCu = DEFAULT_MAX_ACCEPT_CU;
    m_nOverloadThreshold = DEFAULT_THRESHOLD_VALUE;
	m_bSyncVidSrcName2Plat = false;
	m_bSyncVidSrcName2Pu = false;
	m_bDisplayAreaInfo = false;
	m_nPuOfflineTimeRange = 0;
	m_nPuMaxOfflineTimes = 0;
    m_wMaxDomainConnectTimes = 0;
	m_bSupportClockRate = false;
	m_bEnableAutoDecoderAudioCall = false;
	m_bIsStreamKeepaliveEnable = false;
	m_wStreamKeepaliveInterval = DEFAULT_STREAM_KEEPALIVE_INTERVAL;
	m_bIsCheckRecvSRRR = false;
	m_wSRRRCheckTimeout = DEFAULT_SRRR_CHECK_TIMEOUT;

    m_dwRedisDomainSyncNumOnce = DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE;
	m_bUseRedisDirectConnect = true;
    m_nRedisDomainSyncCmdTimeout = DEFAULT_REDIS_DOMAIN_SYNC_CMD_TIMEOUT;
	m_bModDevModel = false;

	m_bAuthDomainRedisSync = false;
	m_bGbDevGpsEnable = false;

    m_bDomainSsSkipPuStatus    = false;
    m_bDomainSsSkipPuConfig    = false;
    m_bDomainSsSkipPuAlarm     = false;
    m_bDomainSsSkipPuService   = false;
    m_bDomainSsSkipPuTransData = false;
    m_bDomainSsSkipPuGpsData   = false;

    m_bAuthDomainSsSkipPuStatus    = false;
    m_bAuthDomainSsSkipPuConfig    = false;
    m_bAuthDomainSsSkipPuAlarm     = false;
    m_bAuthDomainSsSkipPuService   = false;
    m_bAuthDomainSsSkipPuTransData = false;
    m_bAuthDomainSsSkipPuGpsData   = false;
	m_nTcmallocLevel = 1;
	m_nTcmallocReleaseInterval = 0;
}

BOOL32 CCmuConf::ReadConf(const s8* szConfPath)
{
    if (szConfPath == NULL)
    {
        printf("配置文件路径为空\n");
        return FALSE;
    }

    //读取公共配置文件
    if (!ReadCommonConfig())
    {
        printf("公共配置文件初始化失败\n");
        return FALSE;
    }
    m_cCmCfg = GetCommonConfig();
    
    //读配置文件
    s8 szStr[MAX_READ_BUF_LEN] = {0};
    int nVal = 0;

    m_tLocalUri.SetDomain(GetDomainName());
    m_tLocalUri.SetUser(UNIQUE_SIP_UA_CMU);

    m_t3asUri.SetDomain(GetDomainName());
    m_t3asUri.SetUser(UNIQUE_SIP_UA_3AS);
    
    m_tProxyUri.SetDomain(GetDomainName());
    m_tProxyUri.SetUser(UNIQUE_SIP_UA_PROXY);
    
    m_tLocalAddr.tIpAddr = GetLocalIPAddress();

    GetRegKeyInt(szConfPath, SECTION_CMU, KEY_LOCAL_PORT, CMU_SIP_UDP_PORT, &nVal);
    m_tLocalAddr.wPort = (u16)nVal;

    m_tProxyAddr.tIpAddr = GetProxyIPAddress();

    m_tProxyAddr.wPort = GetProxyPort();

    //接入能力
    GetRegKeyInt(szConfPath, SECTION_CMU, KEY_MAX_ACCEPT_PU, DEFAULT_MAX_ACCEPT_PU, &nVal);
    m_nMaxAcceptPu = nVal;
    GetRegKeyInt(szConfPath, SECTION_CMU, KEY_MAX_ACCEPT_CU, DEFAULT_MAX_ACCEPT_CU, &nVal);
    m_nMaxAcceptCu = nVal;
    GetRegKeyInt(szConfPath, SECTION_CMU, KEY_THRESHOLD_VALUE, DEFAULT_THRESHOLD_VALUE, &nVal);
    m_nOverloadThreshold = nVal;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_MAX_PUREG_TIME,DEFAULT_MAX_PUREG_TIME,&nVal);
	m_nMaxPuRegTime = nVal;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_VIDSRCNAME2PLAT_SWITCH,0,&nVal);
	m_bSyncVidSrcName2Plat = nVal ? true : false;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_VIDSRCNAME2PU_SWITCH,0,&nVal);
	m_bSyncVidSrcName2Pu = nVal ? true : false;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DISPLAYAREAINFO_SWITCH,0,&nVal);
	m_bDisplayAreaInfo = nVal ? true : false;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_PU_OFFLINE_RECORD_TIMERANGE,DEFAULT_PU_OFFLINE_RECORD_TIMERANGE,&nVal);
	m_nPuOfflineTimeRange = nVal;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_PU_OFFLINE_RECORD_MAX_TIMES,DEFAULT_PU_OFFLINE_RECORD_MAX_TIMES,&nVal);
	m_nPuMaxOfflineTimes = nVal;
    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_MAX_DOMAIN_CONNECT_TIMES,DEFAULT_MAX_DOMAIN_CONNECT_TIMES,&nVal);
    m_wMaxDomainConnectTimes = (u16)nVal;
    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE,DEFAULT_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE,&nVal);
    m_dwDomainPuOfflineProcNumOnce = (u32)nVal;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_REDIS_DOMAIN_SYNC_NUM_ONCE,DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE,&nVal);
    m_dwRedisDomainSyncNumOnce = nVal;
	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_REDIS_DIRECT_CONNECT,DEFAULT_REDIS_DIRECT_CONNECT,&nVal);
	m_bUseRedisDirectConnect = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_REDIS_DOMAIN_SYNC_CMD_TIMEOUT,DEFAULT_REDIS_DOMAIN_SYNC_CMD_TIMEOUT,&nVal);
    m_nRedisDomainSyncCmdTimeout = nVal;

	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_MOD_DEV_MODEL,0,&nVal);
	m_bModDevModel = nVal ? true : false;

	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_REDIS_SYNC,0,&nVal);
	m_bAuthDomainRedisSync = nVal ? true : false;

	GetRegKeyInt(szConfPath,SECTION_CMU,KEY_GBDEV_GPS_ENABLE,0,&nVal);
	m_bGbDevGpsEnable = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_STATUS,0,&nVal);
    m_bDomainSsSkipPuStatus = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_CONFIG,0,&nVal);
    m_bDomainSsSkipPuConfig = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_ALARM,0,&nVal);
    m_bDomainSsSkipPuAlarm = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_SERVICE,0,&nVal);
    m_bDomainSsSkipPuService = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_TRANS_DATA,0,&nVal);
    m_bDomainSsSkipPuTransData = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_DOMAIN_SS_SKIP_PU_GPS_DATA,0,&nVal);
    m_bDomainSsSkipPuGpsData = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_STATUS,0,&nVal);
    m_bAuthDomainSsSkipPuStatus = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_CONFIG,0,&nVal);
    m_bAuthDomainSsSkipPuConfig = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_ALARM,0,&nVal);
    m_bAuthDomainSsSkipPuAlarm = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_SERVICE,0,&nVal);
    m_bAuthDomainSsSkipPuService = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_TRANS_DATA,0,&nVal);
    m_bAuthDomainSsSkipPuTransData = nVal ? true : false;

    GetRegKeyInt(szConfPath,SECTION_CMU,KEY_AUTH_DOMAIN_SS_SKIP_PU_GPS_DATA,0,&nVal);
    m_bAuthDomainSsSkipPuGpsData = nVal ? true : false;

    ///////////////////////////////////////////////////////////////////////////
    //SECTION_SWITCH
    GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_SWITCH_FOLLOW_TOPO, 0, &nVal);
    m_bSwitchFollowTopo = nVal ? true : false;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_SUPPORT_CLOCK_RATE, 0, &nVal);
	m_bSupportClockRate = nVal ? true : false;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_ENABLE_AUTO_DECODER_AUDIO_CALL, 0, &nVal);
	m_bEnableAutoDecoderAudioCall = nVal ? true : false;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_IS_STREAM_KEEPALIVE_ENABLE, 0, &nVal);
	m_bIsStreamKeepaliveEnable = nVal ? true : false;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_STREAM_KEEPALIVE_INTERVAL, DEFAULT_STREAM_KEEPALIVE_INTERVAL, &nVal);
	m_wStreamKeepaliveInterval = (nVal>DEFAULT_STREAM_KEEPALIVE_INTERVAL) ? nVal:DEFAULT_STREAM_KEEPALIVE_INTERVAL;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_IS_CHECK_RECV_SR_RR, 0, &nVal);
	m_bIsCheckRecvSRRR = nVal ? true : false;

	//SECTION_SWITCH
	GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_CHECK_RECV_SR_RR_TIMEOUT, DEFAULT_SRRR_CHECK_TIMEOUT, &nVal);
	m_wSRRRCheckTimeout = (nVal>DEFAULT_SRRR_CHECK_TIMEOUT) ? nVal:DEFAULT_SRRR_CHECK_TIMEOUT;

    /*直传模式(前端有直传能力情况下，影响的只是是否调度本域VTDU)：0--按需直传，1--始终优先直传*/
    GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_DIRECT_TRANSFER_MODE, DIRECTTRANS_MODE_ONDEMAND, &nVal);
    m_nDirectTransMode = nVal;

    //GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_GEN_MEDIA_CHN_KEY, 0, &nVal);
    m_bEnablePunch = /*GetEnablePunch()*/false;

	//读取端口映射表
	vector<CCommonNetNatInfo> vctMappingTableOld = GetCommonConfig().GetNetNatInfo();
	for (vector<CCommonNetNatInfo>::iterator it=vctMappingTableOld.begin(); it!=vctMappingTableOld.end(); it++)
	{
		//无法区分vtdu的实际使用端口，直接读取所有的udp端口映射表
		if (it->protocol==NAT_PROTOCOL_UDP)
		{
			TMappingItem tItem;
			tItem.SetLocalIP(it->platformIP);
			tItem.SetLocalPort(it->platformPort);
			tItem.SetMappingIP(it->platformNatIP);
			tItem.SetMappingPort(it->platformNatPort);
			tItem.SetProtocol(it->protocol);
			std::stringstream ssKey;
			ssKey << it->protocol << "_" << it->platformIP << "_" << it->platformPort;
			g_tMappingTable[ssKey.str()] = tItem;
		}
	}
	vector<CCommonNetNatInfo> vctMappingTable = GetCommonOtherNetNatInfo();
	for (vector<CCommonNetNatInfo>::iterator it=vctMappingTable.begin(); it!=vctMappingTable.end(); it++)
	{
		//无法区分vtdu的实际使用端口，直接读取所有的udp端口映射表
		if (it->protocol==NAT_PROTOCOL_UDP)
		{
			TMappingItem tItem;
			tItem.SetLocalIP(it->platformIP);
			tItem.SetLocalPort(it->platformPort);
			tItem.SetMappingIP(it->platformNatIP);
			tItem.SetMappingPort(it->platformNatPort);
			tItem.SetProtocol(it->protocol);
			std::stringstream ssKey;
			ssKey << it->protocol << "_" << it->platformIP << "_" << it->platformPort;
			g_tMappingTable[ssKey.str()] = tItem;
		}
	}

	TDomainEntryTable tDomainEntryTable;
	tDomainEntryTable.Init(szConfPath, SECTION_DOMAINS);
	tDomainEntryTable.LoadEntry();
	vector<TDomainEntry>& tDomainEntryList = tDomainEntryTable.GetEntryList();
	for (vector<TDomainEntry>::const_iterator it=tDomainEntryList.begin(); it!=tDomainEntryList.end(); ++it)
	{
		if (!(it->strDomainName.empty() || it->strConnectIp.empty()))
		{
			m_tDomainConnectIpList[it->strDomainName] = it->strConnectIp;
		}
	}

    UNREFERENCED_PARAMETER(szStr);


	GetRegKeyInt(szConfPath, SECTION_CMU, KEY_TCMALLOC_LEVEL, m_nTcmallocLevel, &nVal);
	m_nTcmallocLevel = nVal;
	
	GetRegKeyInt(szConfPath, SECTION_CMU, KEY_TCMALLOC_RELEASE_INTERVAL, m_nTcmallocReleaseInterval, &nVal);
	m_nTcmallocReleaseInterval = nVal;

    return TRUE;
}

void CCmuConf::UpdateSyncVidSrcName2PlatSwitch(bool bVar)
{
	int nVal = 0;
	m_bSyncVidSrcName2Plat = bVar;
	nVal = (int)m_bSyncVidSrcName2Plat;
	SetRegKeyInt(cmu_config,SECTION_CMU,KEY_VIDSRCNAME2PLAT_SWITCH,nVal);
}

void CCmuConf::UpdateDisplayAreaInfoSwitch(bool bVar)
{
	int nVal = 0;
	m_bDisplayAreaInfo = bVar;
	nVal = (int)m_bDisplayAreaInfo;
	SetRegKeyInt(cmu_config,SECTION_CMU,KEY_DISPLAYAREAINFO_SWITCH,nVal);
}

bool CCmuConf::IsEnableCheckPuOfflineTime()
{
    return (m_nPuMaxOfflineTimes > 0 && m_nPuOfflineTimeRange > 0);
}

s32 CCmuConf::GetTcmallocLevel() const
{
	return m_nTcmallocLevel;
}
s32 CCmuConf::GetTcmallocReleaseInterval() const
{
	return m_nTcmallocReleaseInterval;
}
void CCmuConf::SetTcmallocReleaseInterval(int nInterval)
{
	m_nTcmallocReleaseInterval = nInterval;
	int nVal = (int)m_nTcmallocReleaseInterval;
	SetRegKeyInt(cmu_config, SECTION_CMU, KEY_TCMALLOC_RELEASE_INTERVAL, nVal);
}

void CCmuConf::PrintData()
{
    OspPrintf(TRUE, FALSE, "\nCMU配置信息：\n");
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "  CMU域名: [%s]\n", m_tLocalUri.GetDomain().c_str());
    OspPrintf(TRUE, FALSE, "  CMU地址信息: [%s]\n", m_tLocalAddr.GetSipAddrStr().c_str());
    OspPrintf(TRUE, FALSE, "  PROXY地址信息: [%s]\n", m_tProxyAddr.GetSipAddrStr().c_str()); 

    OspPrintf(TRUE, FALSE, "  最大监控点数: [%d]\n", m_nMaxAcceptPu);
    OspPrintf(TRUE, FALSE, "  最大客户端数: [%d]\n", m_nMaxAcceptCu);
    OspPrintf(TRUE, FALSE, "  告警阈值: [%d]\n", m_nOverloadThreshold);
    OspPrintf(TRUE, FALSE, "  单次redis同步设备数量: [%u]\n", m_dwRedisDomainSyncNumOnce);
	OspPrintf(TRUE, FALSE, "  是否启用私有级联redis直连: [%u]\n", m_bUseRedisDirectConnect);
    OspPrintf(TRUE, FALSE, "  单次redis同步命令的超时时间: [%u]ms\n", m_nRedisDomainSyncCmdTimeout);
    OspPrintf(TRUE, FALSE, "  最大异域连接次数: [%d]\n", m_wMaxDomainConnectTimes);
    OspPrintf(TRUE, FALSE, "  异域连接失败时单次通知设备下线的数量: [%u]\n", m_dwDomainPuOfflineProcNumOnce);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_ON_OFF订阅项: [%d]\n", m_bDomainSsSkipPuStatus);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_CONFIG订阅项: [%d]\n", m_bDomainSsSkipPuConfig);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_ALARM订阅项: [%d]\n", m_bDomainSsSkipPuAlarm);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_SERVICE订阅项: [%d]\n", m_bDomainSsSkipPuService);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_TRANS_DATA订阅项: [%d]\n", m_bDomainSsSkipPuTransData);
    OspPrintf(TRUE, FALSE, "  忽略异域PU_GPS_DATA订阅项: [%d]\n", m_bDomainSsSkipPuGpsData);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_ON_OFF订阅项: [%d]\n", m_bAuthDomainSsSkipPuStatus);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_CONFIG订阅项: [%d]\n", m_bAuthDomainSsSkipPuConfig);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_ALARM订阅项: [%d]\n", m_bAuthDomainSsSkipPuAlarm);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_SERVICE订阅项: [%d]\n", m_bAuthDomainSsSkipPuService);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_TRANS_DATA订阅项: [%d]\n", m_bAuthDomainSsSkipPuTransData);
    OspPrintf(TRUE, FALSE, "  忽略跨域授权域PU_GPS_DATA订阅项: [%d]\n", m_bAuthDomainSsSkipPuGpsData);

    OspPrintf(TRUE, FALSE, "  是否层级交换: SwitchFollowTopo [%d]\n", m_bSwitchFollowTopo);
    OspPrintf(TRUE, FALSE, "  直传模式: DirectTransferMode [%d]\n", m_nDirectTransMode);
    OspPrintf(TRUE, FALSE, "  是否生成媒体通道健值去Punch: EnablePunch [%d]\n", m_bEnablePunch);
	OspPrintf(TRUE, FALSE, "  是否支持后端能力集协商: SupportClockRate [%d]\n", m_bSupportClockRate);
	OspPrintf(TRUE, FALSE, "  是否开启自动解码器呼叫: EnableAutoDecoderAudioCall [%d]\n", m_bEnableAutoDecoderAudioCall);

	OspPrintf(TRUE, FALSE, "  是否开启流保活: IsStreamKeepaliveEnable [%d]\n", m_bIsStreamKeepaliveEnable);
	OspPrintf(TRUE, FALSE, "  流保活发包间隔: StreamKeepaliveInterval [%d]\n", m_wStreamKeepaliveInterval);
	OspPrintf(TRUE, FALSE, "  是否启用对端流保活检测: IsCheckSRRR [%d]\n", m_bIsCheckRecvSRRR);
	OspPrintf(TRUE, FALSE, "  对端流保活检测超时间隔: CheckSRRRTimeOut [%d]\n", m_wSRRRCheckTimeout);
	OspPrintf(TRUE, FALSE, "  是否在设备注册时根据能力修改设备型号: ModDevModel [%d]\n", m_bModDevModel);

	OspPrintf(TRUE, FALSE, "  是否启用国标设备GPS使能: GbDevGpsEnable [%d]\n", m_bGbDevGpsEnable);

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
  
    OspPrintf(TRUE, FALSE, "\nCommonCfg配置信息：\n");

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "  本地地址: [%s]\n", m_cCmCfg.GetLocalIP().c_str());
    OspPrintf(TRUE, FALSE, "  本域UUID: [%s]\n", m_cCmCfg.GetDomainUUID().c_str());
    OspPrintf(TRUE, FALSE, "  本域域名: [%s]\n", m_cCmCfg.GetDomainName().c_str());
    OspPrintf(TRUE, FALSE, "  父域UUID: [%s]\n", m_cCmCfg.GetParentDomainUUID().c_str());
    OspPrintf(TRUE, FALSE, "  父域域名: [%s]\n", m_cCmCfg.GetParentDomainName().c_str());

    OspPrintf(TRUE, FALSE, "  Proxy信息: Local[%s:%d], Parent Uri[%s]\n", 
        m_cCmCfg.GetProxyIP().c_str(), m_cCmCfg.GetProxyPort(), m_cCmCfg.GetParentPlatformProxyUrl().c_str());

    OspPrintf(TRUE, FALSE, "  Stun信息: [%s:%d]\n", m_cCmCfg.GetStunIP().c_str(), m_cCmCfg.GetStunPort());

    OspPrintf(TRUE, FALSE, "  开启punch: [%d]\n", m_cCmCfg.GetEnablePunch());
    OspPrintf(TRUE, FALSE, "  Nms信息: [%d]\n", m_cCmCfg.GetNmsPort());

    OspPrintf(TRUE, FALSE, "  CmsVer: [%s], CuVer: [%s], CuUrl: [%s]\n", 
        m_cCmCfg.GetCmsVer().c_str(), m_cCmCfg.GetCuVer().c_str(), m_cCmCfg.GetCuUpdateUrl().c_str());

    OspPrintf(TRUE, FALSE, "  日志目录: [%s]\n", m_cCmCfg.GetLogDir().c_str());

	OspPrintf(TRUE, FALSE, "  UDP端口映射配置数：%d\n", g_tMappingTable.size());
	int nCount = 0;
	for (TMappingTable::const_iterator it=g_tMappingTable.begin(); it!=g_tMappingTable.end();it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "     [%d]：[%s][Local-%s:%d -> Mapping-%s:%d]\n", 
			nCount, it->second.GetProtocol().c_str(),
			it->second.GetLocalIP().c_str(), it->second.GetLocalPort(),
			it->second.GetMappingIP().c_str(), it->second.GetMappingPort());
	}
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

	OspPrintf(TRUE, FALSE, "\n私有级联域Redis连接IP配置信息：\n");

	OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
	OspPrintf(TRUE, FALSE, "  私有级联域Redis连接IP配置数：%d\n",
		m_tDomainConnectIpList.size());
	nCount = 0;
	for (TDomainConnectIpList::const_iterator it=m_tDomainConnectIpList.begin(); it!=m_tDomainConnectIpList.end();it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "     [%d]：Domain[%s] IP[%s]\n", 
			nCount, it->first.c_str(), it->second.c_str());
	}
	OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
}

