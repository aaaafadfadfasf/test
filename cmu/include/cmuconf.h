/*========================================================================================
ģ����    ��cmu
�ļ���    ��cmuconf.h
����ļ�  ��kdvtype.h ospsip.h
ʵ�ֹ���  ��cmu������Ϣ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/11/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _CMU_CONF_H
#define _CMU_CONF_H

#include "kdvtype.h"
#include "cms/ospsip/ospsip.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/cfgfile.h"

#define cmu_config  (LPCSTR)"./conf/cmu.ini"

struct TDomainEntry
{
public:
	string strDomainName;
	string strConnectIp;

public:
	TDomainEntry()
	{
	}

	string ToString() const
	{
		std::ostringstream oss;
		oss<<strDomainName << "\t" << strConnectIp;
		return oss.str();
	}

	bool Parse(LPCSTR lpszEntryContent)
	{
		s8 achDomainName[256] = {0};
		s8 achConnectIp[16] = {0};

		if(NULL == lpszEntryContent)
		{
			return false;
		}

		if(2 == sscanf(lpszEntryContent, "%s%s", 
			achDomainName,
			achConnectIp))
		{
			strDomainName = achDomainName;
			strConnectIp = achConnectIp;

			return true;
		}

		return false;
	}

	bool MatchInKey(const TDomainEntry& tEntry) const
	{
		if(tEntry.strDomainName == strDomainName)
		{
			return true;
		}

		return false;
	}
};
typedef CEntryTable<TDomainEntry>  TDomainEntryTable;
typedef string TStrDomainName,TStrConnectIp;
typedef map<TStrDomainName, TStrConnectIp> TDomainConnectIpList;

#define SECTION_CMU             ( "CMU" )
#define KEY_LOCAL_IP            ( "LocalIP" )         //����SIP��ַ
#define KEY_LOCAL_PORT          ( "LocalPort" )       //����SIP�˿�
#define KEY_MAX_ACCEPT_PU       ( "MaxAcceptPu" )     //cmu����ص������
#define KEY_MAX_ACCEPT_CU       ( "MaxAcceptCu" )     //cmu���CU������
#define KEY_THRESHOLD_VALUE     ( "ThresholdValue" )  //�澯��ֵ
#define KEY_MAX_PUREG_TIME      ( "MaxPuRegTime" )    //pu���ע��ʱ�䣬������ʱ����ǿ��������
#define KEY_VIDSRCNAME2PLAT_SWITCH ( "VidSrcName2PlatSwitch" ) //��ƵԴ����ͬ��ƽ̨����
#define KEY_VIDSRCNAME2PU_SWITCH   ( "VidSrcName2PuSwitch" )   //��ƵԴ����ͬ��ǰ�˿���
#define KEY_DISPLAYAREAINFO_SWITCH ( "DisplayAreaInfoSwitch" ) //����Ͻ����Ϣ����
#define KEY_PU_OFFLINE_RECORD_TIMERANGE  ("PuOfflineTimeRange")  // pu �����¼��ʱ�䷶Χ
#define KEY_PU_OFFLINE_RECORD_MAX_TIMES  ("PuMaxOfflineTimes")  // pu �����������������֪ͨCU
#define KEY_MAX_DOMAIN_CONNECT_TIMES  ("MaxDomainConnectTimes")  // �������ӵ������������������������������豸�����ߴ���
#define KEY_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE  ("DomainPuOfflineProcNumOnce") // cmu�޷����ӵ�����cmuʱ�Ը���������ߴ���ÿ�εĴ�������
#define KEY_REDIS_DOMAIN_SYNC_NUM_ONCE ("RedisDomainSyncNumOnce")  // ˽�м���ʱredis����ͬ�����ε��豸����
#define KEY_REDIS_DOMAIN_SYNC_CMD_TIMEOUT       ("RedisDomainSyncCmdTimeout")       // ˽�м���ʱredis����ͬ��ʱredis����ĳ�ʱʱ��
#define KEY_MOD_DEV_MODEL       ("ModDevModel")       // �Ƿ����豸ע��ʱ���������޸��豸�ͺ�
#define KEY_REDIS_DIRECT_CONNECT ("RedisDirectConnect") // �Ƿ�˽�м���ʱREDISֱ����ȡ�豸��Ϣ��ȱʡʹ��REDISֱ��
#define KEY_AUTH_DOMAIN_REDIS_SYNC ("AuthDomainRedisSync") // ��Ȩ�򣨿�����Ȩ���Ƿ����redis��ʽͬ��
#define KEY_GBDEV_GPS_ENABLE       ("GbDevGpsEnable")      // �Ƿ����ù����豸GPSʹ��
#define KEY_DOMAIN_SS_SKIP_PU_STATUS      ("DomainSsSkipPuStatus")   // ˽�м���ʱ����PU_ON_OFF������
#define KEY_DOMAIN_SS_SKIP_PU_CONFIG      ("DomainSsSkipPuConfig")   // ˽�м���ʱ����PU_CONFIG������
#define KEY_DOMAIN_SS_SKIP_PU_ALARM       ("DomainSsSkipPuAlarm")    // ˽�м���ʱ����PU_ALARM������
#define KEY_DOMAIN_SS_SKIP_PU_SERVICE     ("DomainSsSkipPuService")  // ˽�м���ʱ����PU_SERVICE������
#define KEY_DOMAIN_SS_SKIP_PU_TRANS_DATA  ("DomainSsSkipPuTransData")// ˽�м���ʱ����PU_TRANSDATA������
#define KEY_DOMAIN_SS_SKIP_PU_GPS_DATA    ("DomainSsSkipPuGpsData")  // ˽�м���ʱ����PU_GPS_DATA������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_STATUS      ("AuthDomainSsSkipPuStatus")   // ������Ȩʱ����PU_ON_OFF������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_CONFIG      ("AuthDomainSsSkipPuConfig")   // ������Ȩʱ����PU_CONFIG������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_ALARM       ("AuthDomainSsSkipPuAlarm")    // ������Ȩʱ����PU_ALARM������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_SERVICE     ("AuthDomainSsSkipPuService")  // ������Ȩʱ����PU_SERVICE������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_TRANS_DATA  ("AuthDomainSsSkipPuTransData")// ������Ȩʱ����PU_TRANSDATA������
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_GPS_DATA    ("AuthDomainSsSkipPuGpsData")  // ������Ȩʱ����PU_GPS_DATA������
#define KEY_TCMALLOC_LEVEL ("tcmalloc_level")
#define KEY_TCMALLOC_RELEASE_INTERVAL "tcmalloc_release_interval"//tcmalloc�����ͷ��ڴ��ʱ������0��ʾ����������λ�루����3600���ϣ�

#define SECTION_DOMAINS         ( "DOMAINIPS" )

#define SECTION_SWITCH          ( "SWITCH" )
#define KEY_SWITCH_FOLLOW_TOPO  ( "SwitchFollowTopo" )  //��topo�ṹ�㼶����
#define KEY_SUPPORT_CLOCK_RATE  ( "SupportClockRate" )    //�Ƿ�֧�ֲ�����Э��
#define KEY_ENABLE_AUTO_DECODER_AUDIO_CALL  ( "EnableAutoDecoderAudioCall" )    //�Ƿ����Զ�����������
#define KEY_IS_STREAM_KEEPALIVE_ENABLE  ( "IsStreamKeepaliveEnable" )    //�Ƿ��������Ĭ�ϲ�����0������Ϊ1
#define KEY_STREAM_KEEPALIVE_INTERVAL   ( "StreamKeepaliveInterval" )    //������������Ĭ��5s���������������Ч
#define KEY_IS_CHECK_RECV_SR_RR         ( "IsCheckSRRR" )				 //�Ƿ����ü��Զ�SR/RR��
#define KEY_CHECK_RECV_SR_RR_TIMEOUT    ( "CheckSRRRTimeOut" )		     //���Զ�SR/RR���ĳ�ʱʱ����
#define KEY_DIRECT_TRANSFER_MODE  ( "DirectTransferMode" )    //ֱ��ģʽ
//#define KEY_GEN_MEDIA_CHN_KEY  ( "GenMediaChnKey" )    //�Ƿ�����MediaChnKey

#define MAX_READ_BUF_LEN        (256)

#define DEFAULT_MAX_ACCEPT_PU     (20000)          //��ص�
#define DEFAULT_MAX_ACCEPT_CU     (2000)           //cu��
#define DEFAULT_THRESHOLD_VALUE   (80)             //��ֵ
#define DEFAULT_MAX_PUREG_TIME    (60)             //pu���ע��ʱ��
#define DEFAULT_PU_OFFLINE_RECORD_TIMERANGE      (3600)  // s(3600s=1h)
#define DEFAULT_PU_OFFLINE_RECORD_MAX_TIMES      (10)    // ��
#define DEFAULT_MAX_DOMAIN_CONNECT_TIMES         (5)     // ��
#define DEFAULT_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE  (50)    // ��
#define DEFAULT_STREAM_KEEPALIVE_INTERVAL        (5)     // s��
#define DEFAULT_SRRR_CHECK_TIMEOUT               (60)    // ���Զ�SRRR���ĳ�ʱʱ�䣬��λ���롣
#define DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE       (50)
#define DEFAULT_REDIS_DIRECT_CONNECT       (1)
#define DEFAULT_REDIS_DOMAIN_SYNC_CMD_TIMEOUT       (5*1000) // ��λ������

class CCmuConf
{
public:
    CCmuConf();
    BOOL32 ReadConf(const s8* szConfPath);
    void PrintData();
	void UpdateSyncVidSrcName2PlatSwitch(bool bVar);
	void UpdateDisplayAreaInfoSwitch(bool bVar);

    // �ж��Ƿ���Ҫ����ǰ������ʱ���⹦��
    bool IsEnableCheckPuOfflineTime();

	s32 GetTcmallocLevel() const;
	s32 GetTcmallocReleaseInterval() const;
	void SetTcmallocReleaseInterval(int nInterval);
public:
    TSipURI  m_tLocalUri;
    TSipURI  m_t3asUri;
    TSipURI  m_tProxyUri;
    TSipAddr m_tLocalAddr;
    TSipAddr m_tProxyAddr;
    bool     m_bSwitchFollowTopo;
	bool     m_bSupportClockRate;
	bool     m_bEnableAutoDecoderAudioCall;
	bool     m_bIsStreamKeepaliveEnable;
	u16      m_wStreamKeepaliveInterval;
	bool     m_bIsCheckRecvSRRR;			  // �Ƿ����Զ�������
	u16	     m_wSRRRCheckTimeout;			  // �ԶԶ˱�����ļ������ʱʱ��
    s32      m_nDirectTransMode;
    bool     m_bEnablePunch;
    s32      m_nMaxAcceptPu;
    s32      m_nMaxAcceptCu;
    s32      m_nOverloadThreshold;
	s32      m_nMaxPuRegTime;
	bool     m_bSyncVidSrcName2Plat;          //ͬ����ƵԴ���Ƶ�ƽ̨
	bool     m_bSyncVidSrcName2Pu;            //ͬ����ƵԴ���Ƶ�PU
	bool     m_bDisplayAreaInfo;              //����Ͻ����Ϣ
	s32      m_nPuOfflineTimeRange;           // �豸����ͳ��ʱ����
	s32      m_nPuMaxOfflineTimes;            // �豸���ߴ����ﵽ��ֵ��֪ͨCU
    u16      m_wMaxDomainConnectTimes;       // ��������Ӵ����������ô���������������豸�����ߴ���
    u32      m_dwDomainPuOfflineProcNumOnce;         // cmu�޷����ӵ�����cmuʱ�Ը���������ߴ���ÿ�εĴ�������

    // redis��ͬ����ز���
    u32      m_dwRedisDomainSyncNumOnce;
	u32      m_bUseRedisDirectConnect;
    s32      m_nRedisDomainSyncCmdTimeout;

    bool     m_bDomainSsSkipPuStatus;
    bool     m_bDomainSsSkipPuConfig;
    bool     m_bDomainSsSkipPuAlarm;
    bool     m_bDomainSsSkipPuService;
    bool     m_bDomainSsSkipPuTransData;
    bool     m_bDomainSsSkipPuGpsData;
    bool     m_bAuthDomainSsSkipPuStatus;
    bool     m_bAuthDomainSsSkipPuConfig;
    bool     m_bAuthDomainSsSkipPuAlarm;
    bool     m_bAuthDomainSsSkipPuService;
    bool     m_bAuthDomainSsSkipPuTransData;
    bool     m_bAuthDomainSsSkipPuGpsData;

    CCommonConfig m_cCmCfg;
	bool     m_bModDevModel;                  //�Ƿ����豸ע��ʱ���������޸��豸�ͺ�

	//[DOMAINS]
	TDomainConnectIpList m_tDomainConnectIpList;

    bool     m_bAuthDomainRedisSync;
	bool     m_bGbDevGpsEnable;               //�Ƿ����ù����豸GPSʹ��

	s32 m_nTcmallocLevel;//tcmalloc�ڴ��ͷ�Ƶ�ʵȼ�
	s32 m_nTcmallocReleaseInterval;//tcmalloc�Զ��ͷ��ڴ涨ʱ�����
};

extern CCmuConf g_cCmuConf;

#endif  //#ifndef _VCMU_CONF_H