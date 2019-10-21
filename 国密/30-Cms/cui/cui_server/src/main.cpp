/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : main.cpp
   相关文件    : 
   文件实现功能: cui_server main念书
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/7   1.0         liangli        创建
******************************************************************************/
#include <iostream>
#include "cms/cms_const.h"
#include "cms/ospext/ospext.h"
#include "cms/cmu/cmu_event.h"
#include "cms/tas/tas_event.h"
#include "cuiproto.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/cmu/vtdulib.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/dps/dps_event.h"
#include "redisdb.h"
#include "sdk/redissdk/redissdk.h"

#define CUI_LOG_DIR "cui"

CCuiApp g_cCuiApp;
CConfig g_cCuiConfig;
CCuiDataMgr g_cCuiDataMgr;
CXSemLock g_MainSemLock;
bool    g_bSockRecvThreadRun = true;
string g_strLogFile;
CElapsedTimeList g_cElapsedTimeList;

CMapCuVideoSrcStatList g_mapDevVideoSrcStatList;
CCuAgent *g_pCuAgentToCMU = NULL;

TMappingTable g_tMappingTable;
extern CRedisDBInterface g_cRedisDb;
extern CRedisSdk g_cCuiRedisSdk;

//redis断链通知或其他可能的状态通知回调函数定义(这个是应胡志云要求加的）
void CuiRedisStatusCallback(CFastDBInterface *pInter, EFDB_StatusCode eSC, void *reserve, void *userdata)
{
	if (SC_DISCONNECTED == eSC)
	{
		GLOBELOG(ERROR_LEV, "Redis Svr is disconnected, CUI quit!\n");
		::OspPost(CUIAPP_DAEMON, CUI_REDIS_DISC);
	}

	GLOBELOG(WARNING_LEV, "Redis Svr Status(%d), ignore...\n", eSC);
}

void CuiProcRedisSdkCallback(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3);

bool InitRedisSdk4Cui();

int main ( int argc, char* argv[] )
{
#if 0
    CreateDeviceMark();
#endif
    if ( ! ReadCommonConfig() )
    {
        printf("初始化公共配置文件失败, 直接退出\n");
        return -1;
    }

	g_cCuiConfig.m_cCmCfg = GetCommonConfig();

    if ( ! g_cCuiConfig.ReadConfig(CONFIG_FILE_NAME) )
    {
        printf("读取CUI配置文件失败，直接退出\n");
        return -1;
    }

    string strLogDir = GetLogDir();
    
    TOspExtConf tOspExtConf;
    if( IsFileExist( EXT_CFG_FILE_NAME ) )
    {
        tOspExtConf.ReadKeyCfg( EXT_CFG_FILE_NAME );
    }
#ifdef _LINUX_
    tOspExtConf.strErrLogDir = strLogDir + CUI_LOG_DIR + "/errlog/";
    tOspExtConf.strRunLogDir = strLogDir + CUI_LOG_DIR + "/runlog/";
    g_strLogFile = tOspExtConf.strErrLogDir + "cuilog.log";
#else
    tOspExtConf.strErrLogDir = strLogDir + CUI_LOG_DIR + "\\errlog\\";
    tOspExtConf.strRunLogDir = strLogDir + CUI_LOG_DIR + "\\runlog\\";
    g_strLogFile = tOspExtConf.strErrLogDir + "cuilog.log";
#endif
    if ( ! OspExtInit(tOspExtConf) )
    {
        printf("OspExtLib初始化失败\n");
        return -1;
    }

#ifdef _LINUX_
        CuiRegHelpCmd();
        OspRegQuitFunc(cuiquit);
#endif

    //初始化所有业务消息
    InitCuiEventDesc();
    InitCmsEventDesc();
    InitTacTasEventDesc();
    InitMpsEventDesc();
    InitRcsEventDesc();
    InitUasEventDesc();
	InitGbsEventDesc();
	InitCapsEventDesc();
	InitDPSEvent();
	InitSecurityModuleEventDesc();

    if( !InitRedisSdk4Cui() )
    {
        return 0;
    }

    TRecvThreadInfo tInfo;
    tInfo.dwListenIpAddr = INADDR_ANY;
    tInfo.wListenPort = g_cCuiConfig.GetTcpListenPort();
#ifdef USE_EPOLL
    //TASKHANDLE hRecvThread = OspTaskCreate( EpollReceivingThread, "EpollRecvTask", 100, 1<<20, (u32)&tInfo, 0);
    OspTaskCreate( EpollReceivingThread, "EpollRecvTask", 100, 1<<20, (KD_PTR)&tInfo, 0);
#else
    //TASKHANDLE hRecvThread = OspTaskCreate( SelectReceivingThread, "SelectRecvTask", 100, 1<<20, (u32)&tInfo, 0);
    OspTaskCreate( SelectReceivingThread, "SelectRecvTask", 100, 1<<20, (KD_PTR)&tInfo, 0);
#endif
    g_cCuiApp.CreateOspApp("CuiApp", (u16)CUIAPP_AID, (u8)80, (u16)g_cCuiConfig.GetAppQueueSize());

    //初始化SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tUAType = UA_TYPE_CUI;
    //tOspSipConf.tLocalAddr.tIpAddr = g_cCuiConfig.GetLocalIP();
    tOspSipConf.tLocalAddr.tIpAddr = GetLocalIPAddress();
    tOspSipConf.tLocalAddr.wPort = g_cCuiConfig.GetLocalPort();
    tOspSipConf.tProxyAddr.tIpAddr = GetProxyIPAddress();
    tOspSipConf.tProxyAddr.wPort = GetProxyPort();
    tOspSipConf.tLocalURI = g_cCuiConfig.GetLocalURI();
    tOspSipConf.tProxyURI = g_cCuiConfig.GetLocalURI().GetProxyURI();
    tOspSipConf.nUaRouteMode = g_cCuiConfig.GetRouteMode();
    tOspSipConf.dwDefaultOspIID = CUIAPP_DAEMON;

    TSipStackConf tSipStackConf;
#ifdef _LINUX_
    string strSipLogPath = GetLogDir() + CUI_LOG_DIR + "/";   //只要设置到模块即可，kdsip会自己创建子目录
#else
    string strSipLogPath = GetLogDir() + CUI_LOG_DIR + "\\";   //只要设置到模块即可，kdsip会自己创建子目录
#endif
    tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();
	tSipStackConf.tKdSipConfig.nMsgSizeMax = KDSIP_INIT_CONFIG_DEFAULT_MSG_SIZE_MAX*2; //放大到64K

    TKdsipKeyCfg tKeyCfg ;// = ReadKeyCfg_xml(cui_config);
    g_cCuiConfig.SetSipKeyCfg(tKeyCfg);
    tSipStackConf.SetKeyCfg(tKeyCfg);

    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

	//初始化vtdulib
	TVtduLibConf tVtduLibConf;
	tVtduLibConf.m_emRunMode = RUN_MODE_ATTACH;
	tVtduLibConf.m_dwDefaultOspIID = CUIAPP_DAEMON;
	tVtduLibConf.m_fpNatDetectCB = NatDetect_CB;

	if (VtduLibStart(tVtduLibConf) == FALSE)
	{
		printf("VtduLibStart fail!!!\n");
		return 0;
	}
	printf("VtduLibStart初始化成功!\n");
	GLOBELOG(CRITICAL_LEV, "VtduLibStart初始化成功!\n");

	//更新nat映射
	TVtduCfg tVtduCfg = GetVtduCfg();

	//vtdu NATDTECT探测接收端口
	u16& wNatPort = g_cCuiConfig.GetNatPort();
	wNatPort = tVtduCfg.m_nMediaStartPort;
	if (wNatPort <= 0)
	{
		printf("cuicfg.xml-NatRecvPort(%d) invalid!!!\n", wNatPort);
		return 0;
	}
	g_cCuiConfig.UpdateNatMap(wNatPort);

	if (AddNatRcvPort(wNatPort) == FALSE)
	{
		printf("AddNatRcvPort(%d) fail!!!\n", wNatPort);
		return 0;
	}
	printf("AddNatRcvPort(%d)调用成功!\n",wNatPort);
	GLOBELOG(CRITICAL_LEV, "AddNatRcvPort(%d)调用成功!\n", wNatPort); 


    if (!OspSipRegProxy(3600))
    {
        printf("OspSipRegProxy[%s] fail!!!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
        return 0;
    }

    //设置打印级别
    OspSetLogTimeLev(1);
    OspLogOspPrefix(0);
    OspSetTimeMonitor(0);
    logall(2);
    liblog(2);

	SetLogCallBack(cbRedisLog, NULL);

    g_cCuiDataMgr.SetCuiBeginRunTime();
    g_cCuiDataMgr.SetCuiStateHappenTime();

    g_MainSemLock.Lock();
    g_MainSemLock.Lock();

    OspPost(CUIAPP_DAEMON, CUI_QUIT);
    g_MainSemLock.Lock(5000);

    OspSipQuit();
    OspExtQuit();

    return 0;
}

bool InitRedisSdk4Cui()
{
    string redisIp = g_cCuiConfig.m_cCmCfg.GetRedisIP();
	if (redisIp.empty())
	{
		redisIp = g_cCuiConfig.GetLocalIp();
	}
	int redisPort = g_cCuiConfig.m_cCmCfg.GetRedisPort();
	if (redisPort == 0)
	{
		redisPort = REDIS_DEF_PORT;
	}
	string strRedisPwd = g_cCuiConfig.m_cCmCfg.GetRedisPassword();
	FDB_ConnInfo connectInfo;
	string strClientName = string(DEV_TYPE_CUI) + ":" + g_cCuiConfig.GetLocalURI().GetUser();
	connectInfo.clientname = strClientName;
	connectInfo.bAutoReconn = false;
	connectInfo.password = strRedisPwd;
	connectInfo.pfStatusCallBack = CuiRedisStatusCallback;
	EFDB_ErrorCode er = g_cRedisDb.Connect(redisIp, redisPort, connectInfo);
	if (er != EC_OK)
	{
		printf("connect redis failed ip[%s] port[%d] pwd[%s] result[%d]\n", redisIp.c_str(), redisPort, strRedisPwd.c_str(), er);
		return false;
	}

	ECMS_ERRORCODE eErrCode = g_cCuiRedisSdk.Initialize(strClientName, redisIp, redisPort, strRedisPwd, CuiProcRedisSdkCallback, NULL );
	if (eErrCode != CMS_SUCCESS)
	{
		printf("connect redis failed ip[%s] port[%d] pwd[%s] result[%d]\n", redisIp.c_str(), redisPort, strRedisPwd.c_str(), eErrCode);
		return false;
	}
    
    TRedisModuleId tModuleId;
    tModuleId.tModuleType = RedisModuleType_Cui;
    tModuleId.strId       = g_cCuiConfig.GetLocalURI().GetUser();
    ECMS_ERRORCODE errCode = g_cCuiRedisSdk.CleanModulePrivateData( tModuleId );
    return true;
}

API void cuiquit(int nSignal)
{
    printf("[MAIN]Recv singal %d, quit cui\n", nSignal);
    g_bSockRecvThreadRun = false;
    g_MainSemLock.UnLock();
}

void CuiProcRedisSdkCallback(RedisSdk_Msg emMsg, const void *pvParam1, const void *pvParam2, const void *pvParam3)
{
    switch (emMsg)
    {
    case RedisSdk_Msg_Redis_Disconnect:
        {
            TRedisSdkDiscCB *ptCB = new TRedisSdkDiscCB();
            if(NULL != ptCB)
            {
                ptCB->emCode = *(reinterpret_cast<const EFDB_StatusCode *>(pvParam2));

                s32 nRet = ::OspPost(CUIAPP_DAEMON, CUI_REDISSDK_DISC_CB, &ptCB, sizeof(ptCB));
                if (0 != nRet)
                {
                    delete ptCB;
                    ptCB = NULL;
                }
            }

        }
        break;
    case RedisSdk_Msg_Subscribe_Notify:
        {
            TRedisSdkSubCB *ptCB = new TRedisSdkSubCB();
            if(NULL != ptCB)
            {
                ptCB->cSsChn = *(reinterpret_cast<const CRedisSsChannel *>(pvParam2));
                ptCB->strContent  = *(reinterpret_cast<const string *>(pvParam3));

                s32 nRet = ::OspPost(CUIAPP_DAEMON, CUI_REDISSDK_SUB_CB, &ptCB, sizeof(ptCB));
                if (0 != nRet)
                {
                    delete ptCB;
                    ptCB = NULL;
                }
            }
        }
        break;
    default:
        {
            printf("未知的RedisSdk回调[%d]，pvParam1[%p], pvParam2[%p], pvParam2[%p]！\n", 
                        emMsg, pvParam1, pvParam2, pvParam3);
                    return;
        }
        break;
    }
}
