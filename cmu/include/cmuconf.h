/*========================================================================================
模块名    ：cmu
文件名    ：cmuconf.h
相关文件  ：kdvtype.h ospsip.h
实现功能  ：cmu配置信息管理
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/11/15         V1.0              fanxg                                  新建文件
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
#define KEY_LOCAL_IP            ( "LocalIP" )         //本地SIP地址
#define KEY_LOCAL_PORT          ( "LocalPort" )       //本地SIP端口
#define KEY_MAX_ACCEPT_PU       ( "MaxAcceptPu" )     //cmu最大监控点接入数
#define KEY_MAX_ACCEPT_CU       ( "MaxAcceptCu" )     //cmu最大CU接入数
#define KEY_THRESHOLD_VALUE     ( "ThresholdValue" )  //告警阈值
#define KEY_MAX_PUREG_TIME      ( "MaxPuRegTime" )    //pu最大注册时间，超过此时间则强制其下线
#define KEY_VIDSRCNAME2PLAT_SWITCH ( "VidSrcName2PlatSwitch" ) //视频源名称同步平台开关
#define KEY_VIDSRCNAME2PU_SWITCH   ( "VidSrcName2PuSwitch" )   //视频源名称同步前端开关
#define KEY_DISPLAYAREAINFO_SWITCH ( "DisplayAreaInfoSwitch" ) //叠加辖区信息开关
#define KEY_PU_OFFLINE_RECORD_TIMERANGE  ("PuOfflineTimeRange")  // pu 变更记录的时间范围
#define KEY_PU_OFFLINE_RECORD_MAX_TIMES  ("PuMaxOfflineTimes")  // pu 最大变更次数，超出则通知CU
#define KEY_MAX_DOMAIN_CONNECT_TIMES  ("MaxDomainConnectTimes")  // 异域连接的最大次数，超过则给该域的所有在线设备做下线处理
#define KEY_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE  ("DomainPuOfflineProcNumOnce") // cmu无法连接到异域cmu时对该域进行下线处理，每次的处理数量
#define KEY_REDIS_DOMAIN_SYNC_NUM_ONCE ("RedisDomainSyncNumOnce")  // 私有级联时redis数据同步单次的设备数量
#define KEY_REDIS_DOMAIN_SYNC_CMD_TIMEOUT       ("RedisDomainSyncCmdTimeout")       // 私有级联时redis数据同步时redis命令的超时时间
#define KEY_MOD_DEV_MODEL       ("ModDevModel")       // 是否在设备注册时根据能力修改设备型号
#define KEY_REDIS_DIRECT_CONNECT ("RedisDirectConnect") // 是否私有级联时REDIS直连获取设备信息，缺省使用REDIS直连
#define KEY_AUTH_DOMAIN_REDIS_SYNC ("AuthDomainRedisSync") // 授权域（跨域授权）是否采用redis方式同步
#define KEY_GBDEV_GPS_ENABLE       ("GbDevGpsEnable")      // 是否启用国标设备GPS使能
#define KEY_DOMAIN_SS_SKIP_PU_STATUS      ("DomainSsSkipPuStatus")   // 私有级联时忽略PU_ON_OFF订阅项
#define KEY_DOMAIN_SS_SKIP_PU_CONFIG      ("DomainSsSkipPuConfig")   // 私有级联时忽略PU_CONFIG订阅项
#define KEY_DOMAIN_SS_SKIP_PU_ALARM       ("DomainSsSkipPuAlarm")    // 私有级联时忽略PU_ALARM订阅项
#define KEY_DOMAIN_SS_SKIP_PU_SERVICE     ("DomainSsSkipPuService")  // 私有级联时忽略PU_SERVICE订阅项
#define KEY_DOMAIN_SS_SKIP_PU_TRANS_DATA  ("DomainSsSkipPuTransData")// 私有级联时忽略PU_TRANSDATA订阅项
#define KEY_DOMAIN_SS_SKIP_PU_GPS_DATA    ("DomainSsSkipPuGpsData")  // 私有级联时忽略PU_GPS_DATA订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_STATUS      ("AuthDomainSsSkipPuStatus")   // 跨域授权时忽略PU_ON_OFF订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_CONFIG      ("AuthDomainSsSkipPuConfig")   // 跨域授权时忽略PU_CONFIG订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_ALARM       ("AuthDomainSsSkipPuAlarm")    // 跨域授权时忽略PU_ALARM订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_SERVICE     ("AuthDomainSsSkipPuService")  // 跨域授权时忽略PU_SERVICE订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_TRANS_DATA  ("AuthDomainSsSkipPuTransData")// 跨域授权时忽略PU_TRANSDATA订阅项
#define KEY_AUTH_DOMAIN_SS_SKIP_PU_GPS_DATA    ("AuthDomainSsSkipPuGpsData")  // 跨域授权时忽略PU_GPS_DATA订阅项
#define KEY_TCMALLOC_LEVEL ("tcmalloc_level")
#define KEY_TCMALLOC_RELEASE_INTERVAL "tcmalloc_release_interval"//tcmalloc主动释放内存的时间间隔，0表示不开启，单位秒（建议3600以上）

#define SECTION_DOMAINS         ( "DOMAINIPS" )

#define SECTION_SWITCH          ( "SWITCH" )
#define KEY_SWITCH_FOLLOW_TOPO  ( "SwitchFollowTopo" )  //按topo结构层级交换
#define KEY_SUPPORT_CLOCK_RATE  ( "SupportClockRate" )    //是否支持采样率协商
#define KEY_ENABLE_AUTO_DECODER_AUDIO_CALL  ( "EnableAutoDecoderAudioCall" )    //是否开启自动解码器呼叫
#define KEY_IS_STREAM_KEEPALIVE_ENABLE  ( "IsStreamKeepaliveEnable" )    //是否开启流保活，默认不开启0，开启为1
#define KEY_STREAM_KEEPALIVE_INTERVAL   ( "StreamKeepaliveInterval" )    //流保活发包间隔，默认5s，开启流保活后有效
#define KEY_IS_CHECK_RECV_SR_RR         ( "IsCheckSRRR" )				 //是否启用检测对端SR/RR包
#define KEY_CHECK_RECV_SR_RR_TIMEOUT    ( "CheckSRRRTimeOut" )		     //检测对端SR/RR包的超时时间秒
#define KEY_DIRECT_TRANSFER_MODE  ( "DirectTransferMode" )    //直传模式
//#define KEY_GEN_MEDIA_CHN_KEY  ( "GenMediaChnKey" )    //是否生成MediaChnKey

#define MAX_READ_BUF_LEN        (256)

#define DEFAULT_MAX_ACCEPT_PU     (20000)          //监控点
#define DEFAULT_MAX_ACCEPT_CU     (2000)           //cu数
#define DEFAULT_THRESHOLD_VALUE   (80)             //阈值
#define DEFAULT_MAX_PUREG_TIME    (60)             //pu最大注册时间
#define DEFAULT_PU_OFFLINE_RECORD_TIMERANGE      (3600)  // s(3600s=1h)
#define DEFAULT_PU_OFFLINE_RECORD_MAX_TIMES      (10)    // 次
#define DEFAULT_MAX_DOMAIN_CONNECT_TIMES         (5)     // 次
#define DEFAULT_DOMAIN_PU_OFFLINE_PROC_NUM_ONCE  (50)    // 个
#define DEFAULT_STREAM_KEEPALIVE_INTERVAL        (5)     // s秒
#define DEFAULT_SRRR_CHECK_TIMEOUT               (60)    // 检测对端SRRR包的超时时间，单位：秒。
#define DEFAULT_REDIS_DOMAIN_SYNC_NUM_ONCE       (50)
#define DEFAULT_REDIS_DIRECT_CONNECT       (1)
#define DEFAULT_REDIS_DOMAIN_SYNC_CMD_TIMEOUT       (5*1000) // 单位：毫秒

class CCmuConf
{
public:
    CCmuConf();
    BOOL32 ReadConf(const s8* szConfPath);
    void PrintData();
	void UpdateSyncVidSrcName2PlatSwitch(bool bVar);
	void UpdateDisplayAreaInfoSwitch(bool bVar);

    // 判断是否需要开启前端下线时间检测功能
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
	bool     m_bIsCheckRecvSRRR;			  // 是否开启对端流保活
	u16	     m_wSRRRCheckTimeout;			  // 对对端保活包的检测间隔超时时间
    s32      m_nDirectTransMode;
    bool     m_bEnablePunch;
    s32      m_nMaxAcceptPu;
    s32      m_nMaxAcceptCu;
    s32      m_nOverloadThreshold;
	s32      m_nMaxPuRegTime;
	bool     m_bSyncVidSrcName2Plat;          //同步视频源名称到平台
	bool     m_bSyncVidSrcName2Pu;            //同步视频源名称到PU
	bool     m_bDisplayAreaInfo;              //叠加辖区信息
	s32      m_nPuOfflineTimeRange;           // 设备下线统计时间间隔
	s32      m_nPuMaxOfflineTimes;            // 设备下线次数达到该值后通知CU
    u16      m_wMaxDomainConnectTimes;       // 最大域连接次数，超过该次数给该域的在线设备做下线处理
    u32      m_dwDomainPuOfflineProcNumOnce;         // cmu无法连接到异域cmu时对该域进行下线处理，每次的处理数量

    // redis域同步相关参数
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
	bool     m_bModDevModel;                  //是否在设备注册时根据能力修改设备型号

	//[DOMAINS]
	TDomainConnectIpList m_tDomainConnectIpList;

    bool     m_bAuthDomainRedisSync;
	bool     m_bGbDevGpsEnable;               //是否启用国标设备GPS使能

	s32 m_nTcmallocLevel;//tcmalloc内存释放频率等级
	s32 m_nTcmallocReleaseInterval;//tcmalloc自动释放内存定时器间隔
};

extern CCmuConf g_cCmuConf;

#endif  //#ifndef _VCMU_CONF_H