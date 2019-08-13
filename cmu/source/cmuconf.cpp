
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
        printf("�����ļ�·��Ϊ��\n");
        return FALSE;
    }

    //��ȡ���������ļ�
    if (!ReadCommonConfig())
    {
        printf("���������ļ���ʼ��ʧ��\n");
        return FALSE;
    }
    m_cCmCfg = GetCommonConfig();
    
    //�������ļ�
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

    //��������
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

    /*ֱ��ģʽ(ǰ����ֱ����������£�Ӱ���ֻ���Ƿ���ȱ���VTDU)��0--����ֱ����1--ʼ������ֱ��*/
    GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_DIRECT_TRANSFER_MODE, DIRECTTRANS_MODE_ONDEMAND, &nVal);
    m_nDirectTransMode = nVal;

    //GetRegKeyInt(szConfPath, SECTION_SWITCH, KEY_GEN_MEDIA_CHN_KEY, 0, &nVal);
    m_bEnablePunch = /*GetEnablePunch()*/false;

	//��ȡ�˿�ӳ���
	vector<CCommonNetNatInfo> vctMappingTableOld = GetCommonConfig().GetNetNatInfo();
	for (vector<CCommonNetNatInfo>::iterator it=vctMappingTableOld.begin(); it!=vctMappingTableOld.end(); it++)
	{
		//�޷�����vtdu��ʵ��ʹ�ö˿ڣ�ֱ�Ӷ�ȡ���е�udp�˿�ӳ���
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
		//�޷�����vtdu��ʵ��ʹ�ö˿ڣ�ֱ�Ӷ�ȡ���е�udp�˿�ӳ���
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
    OspPrintf(TRUE, FALSE, "\nCMU������Ϣ��\n");
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "  CMU����: [%s]\n", m_tLocalUri.GetDomain().c_str());
    OspPrintf(TRUE, FALSE, "  CMU��ַ��Ϣ: [%s]\n", m_tLocalAddr.GetSipAddrStr().c_str());
    OspPrintf(TRUE, FALSE, "  PROXY��ַ��Ϣ: [%s]\n", m_tProxyAddr.GetSipAddrStr().c_str()); 

    OspPrintf(TRUE, FALSE, "  ����ص���: [%d]\n", m_nMaxAcceptPu);
    OspPrintf(TRUE, FALSE, "  ���ͻ�����: [%d]\n", m_nMaxAcceptCu);
    OspPrintf(TRUE, FALSE, "  �澯��ֵ: [%d]\n", m_nOverloadThreshold);
    OspPrintf(TRUE, FALSE, "  ����redisͬ���豸����: [%u]\n", m_dwRedisDomainSyncNumOnce);
	OspPrintf(TRUE, FALSE, "  �Ƿ�����˽�м���redisֱ��: [%u]\n", m_bUseRedisDirectConnect);
    OspPrintf(TRUE, FALSE, "  ����redisͬ������ĳ�ʱʱ��: [%u]ms\n", m_nRedisDomainSyncCmdTimeout);
    OspPrintf(TRUE, FALSE, "  ����������Ӵ���: [%d]\n", m_wMaxDomainConnectTimes);
    OspPrintf(TRUE, FALSE, "  ��������ʧ��ʱ����֪ͨ�豸���ߵ�����: [%u]\n", m_dwDomainPuOfflineProcNumOnce);
    OspPrintf(TRUE, FALSE, "  ��������PU_ON_OFF������: [%d]\n", m_bDomainSsSkipPuStatus);
    OspPrintf(TRUE, FALSE, "  ��������PU_CONFIG������: [%d]\n", m_bDomainSsSkipPuConfig);
    OspPrintf(TRUE, FALSE, "  ��������PU_ALARM������: [%d]\n", m_bDomainSsSkipPuAlarm);
    OspPrintf(TRUE, FALSE, "  ��������PU_SERVICE������: [%d]\n", m_bDomainSsSkipPuService);
    OspPrintf(TRUE, FALSE, "  ��������PU_TRANS_DATA������: [%d]\n", m_bDomainSsSkipPuTransData);
    OspPrintf(TRUE, FALSE, "  ��������PU_GPS_DATA������: [%d]\n", m_bDomainSsSkipPuGpsData);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_ON_OFF������: [%d]\n", m_bAuthDomainSsSkipPuStatus);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_CONFIG������: [%d]\n", m_bAuthDomainSsSkipPuConfig);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_ALARM������: [%d]\n", m_bAuthDomainSsSkipPuAlarm);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_SERVICE������: [%d]\n", m_bAuthDomainSsSkipPuService);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_TRANS_DATA������: [%d]\n", m_bAuthDomainSsSkipPuTransData);
    OspPrintf(TRUE, FALSE, "  ���Կ�����Ȩ��PU_GPS_DATA������: [%d]\n", m_bAuthDomainSsSkipPuGpsData);

    OspPrintf(TRUE, FALSE, "  �Ƿ�㼶����: SwitchFollowTopo [%d]\n", m_bSwitchFollowTopo);
    OspPrintf(TRUE, FALSE, "  ֱ��ģʽ: DirectTransferMode [%d]\n", m_nDirectTransMode);
    OspPrintf(TRUE, FALSE, "  �Ƿ�����ý��ͨ����ֵȥPunch: EnablePunch [%d]\n", m_bEnablePunch);
	OspPrintf(TRUE, FALSE, "  �Ƿ�֧�ֺ��������Э��: SupportClockRate [%d]\n", m_bSupportClockRate);
	OspPrintf(TRUE, FALSE, "  �Ƿ����Զ�����������: EnableAutoDecoderAudioCall [%d]\n", m_bEnableAutoDecoderAudioCall);

	OspPrintf(TRUE, FALSE, "  �Ƿ���������: IsStreamKeepaliveEnable [%d]\n", m_bIsStreamKeepaliveEnable);
	OspPrintf(TRUE, FALSE, "  ����������: StreamKeepaliveInterval [%d]\n", m_wStreamKeepaliveInterval);
	OspPrintf(TRUE, FALSE, "  �Ƿ����öԶ���������: IsCheckSRRR [%d]\n", m_bIsCheckRecvSRRR);
	OspPrintf(TRUE, FALSE, "  �Զ��������ⳬʱ���: CheckSRRRTimeOut [%d]\n", m_wSRRRCheckTimeout);
	OspPrintf(TRUE, FALSE, "  �Ƿ����豸ע��ʱ���������޸��豸�ͺ�: ModDevModel [%d]\n", m_bModDevModel);

	OspPrintf(TRUE, FALSE, "  �Ƿ����ù����豸GPSʹ��: GbDevGpsEnable [%d]\n", m_bGbDevGpsEnable);

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
  
    OspPrintf(TRUE, FALSE, "\nCommonCfg������Ϣ��\n");

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "  ���ص�ַ: [%s]\n", m_cCmCfg.GetLocalIP().c_str());
    OspPrintf(TRUE, FALSE, "  ����UUID: [%s]\n", m_cCmCfg.GetDomainUUID().c_str());
    OspPrintf(TRUE, FALSE, "  ��������: [%s]\n", m_cCmCfg.GetDomainName().c_str());
    OspPrintf(TRUE, FALSE, "  ����UUID: [%s]\n", m_cCmCfg.GetParentDomainUUID().c_str());
    OspPrintf(TRUE, FALSE, "  ��������: [%s]\n", m_cCmCfg.GetParentDomainName().c_str());

    OspPrintf(TRUE, FALSE, "  Proxy��Ϣ: Local[%s:%d], Parent Uri[%s]\n", 
        m_cCmCfg.GetProxyIP().c_str(), m_cCmCfg.GetProxyPort(), m_cCmCfg.GetParentPlatformProxyUrl().c_str());

    OspPrintf(TRUE, FALSE, "  Stun��Ϣ: [%s:%d]\n", m_cCmCfg.GetStunIP().c_str(), m_cCmCfg.GetStunPort());

    OspPrintf(TRUE, FALSE, "  ����punch: [%d]\n", m_cCmCfg.GetEnablePunch());
    OspPrintf(TRUE, FALSE, "  Nms��Ϣ: [%d]\n", m_cCmCfg.GetNmsPort());

    OspPrintf(TRUE, FALSE, "  CmsVer: [%s], CuVer: [%s], CuUrl: [%s]\n", 
        m_cCmCfg.GetCmsVer().c_str(), m_cCmCfg.GetCuVer().c_str(), m_cCmCfg.GetCuUpdateUrl().c_str());

    OspPrintf(TRUE, FALSE, "  ��־Ŀ¼: [%s]\n", m_cCmCfg.GetLogDir().c_str());

	OspPrintf(TRUE, FALSE, "  UDP�˿�ӳ����������%d\n", g_tMappingTable.size());
	int nCount = 0;
	for (TMappingTable::const_iterator it=g_tMappingTable.begin(); it!=g_tMappingTable.end();it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "     [%d]��[%s][Local-%s:%d -> Mapping-%s:%d]\n", 
			nCount, it->second.GetProtocol().c_str(),
			it->second.GetLocalIP().c_str(), it->second.GetLocalPort(),
			it->second.GetMappingIP().c_str(), it->second.GetMappingPort());
	}
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

	OspPrintf(TRUE, FALSE, "\n˽�м�����Redis����IP������Ϣ��\n");

	OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
	OspPrintf(TRUE, FALSE, "  ˽�м�����Redis����IP��������%d\n",
		m_tDomainConnectIpList.size());
	nCount = 0;
	for (TDomainConnectIpList::const_iterator it=m_tDomainConnectIpList.begin(); it!=m_tDomainConnectIpList.end();it++)
	{
		nCount++;
		OspPrintf(TRUE, FALSE, "     [%d]��Domain[%s] IP[%s]\n", 
			nCount, it->first.c_str(), it->second.c_str());
	}
	OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
}

