/*****************************************************************************
   �ļ�ʵ�ֹ���: ϵͳ��ںͳ���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
******************************************************************************/
#include "osp/osp.h"
#include "inner_common.h"
#include <cstdio>
#include "cctrl.h"
#include "license_mng.h"
#include "nmsconn.h"
#include "log_system.h"
#include "sync_analyzer_mng.h"
#include "config_util/tas_config.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/cmu/cmu_proto.h"
#include "gbs/gbs_proto.h"
#include "redis/fastdbmng.h"
#include "gbs/gbs_event.h"


extern void RegisterDebugAPI();

SEMHANDLE g_hTasExitSem  = NULL;       // �����˳�ͬ�������ź���

#define PrintAndBreak(desc) {printf("%s\n", desc); break;}

void CBTasTerminate(int nSig)
{
    printf("recv terminate signal, signum=%d\n", nSig);
    static bool s_bHandledTerminateSig = false;
    if (!s_bHandledTerminateSig)
    {
        OspSemGive(g_hTasExitSem);
        s_bHandledTerminateSig = true;
    }
}

int main()
{
    s32 nRet = -1;
    bool bNeedDelTasExitSem = false;
    do 
    {
        // CreateDeviceMark();
        if (!ReadCommonConfig())
        {
            PrintAndBreak("ReadCommonConfig failed");
        }

        string strLogDir = GetLogDir();
        if (strLogDir.empty())
        {
            PrintAndBreak("log dir is empty in commonconfig");
        }
        TOspExtConf tOspExtConf;
        tOspExtConf.dwMaxNodeNum+=10; // �����ܿ�ʹ��
#ifdef _LINUX_
        tOspExtConf.strErrLogDir = strLogDir + "tas/errlog/";
        tOspExtConf.strRunLogDir = strLogDir + "tas/runlog/";
#else
        tOspExtConf.strErrLogDir = strLogDir + "tas\\errlog\\";
        tOspExtConf.strRunLogDir = strLogDir + "tas\\runlog\\";
#endif
		tOspExtConf.ReadKeyCfg(CTasConfig::GetCfgPath());
        if (!OspExtInit(tOspExtConf))
        {
            PrintAndBreak("OspInit failed");
        }
        RegisterDebugAPI();

        if (!CTasConfig::Init())
        {
            PrintAndBreak("CTasConfig Init failed");
        }
        
        if (!g_cLicenseApp.CreateOspApp("LicenseApp", AID_License, 90, 1000))
        {
            PrintAndBreak("LicenseApp create failed");
        }
        if (!g_cSyncAnalyzerApp.CreateOspApp("SyncAnalyzerApp", AID_SyncAnalyze, 100, 10000))
        {
            PrintAndBreak("SyncAnalyzerApp create failed");
        }

        if (!g_cCtrlApp.CreateOspApp("CCtrlApp", AID_CCtrl, 80, (u16)CTasConfig::GetCCtrlAppQueueSize()))
        {
            PrintAndBreak("CCtrlApp create failed");
        }
        
        if (!g_cNmcLib.Init(UNIQUE_SIP_UA_3AS, NMC_SUBTYPE_TAS, NMC_SUBTYPE_TAS, 
                       GetLocalIPAddress(), GetNmsPort(), "0"))
        {
            PrintAndBreak("NmcLib init failed");
        }

        InitTacTasEventDesc();
        InitTasInnerEventDesc();
		InitGbsEventDesc();
        InitCmsEventDesc();
		InitGbsEventDesc();

        TOspSipConf tOspSipConf;
        tOspSipConf.tLocalAddr.tIpAddr = CTasConfig::GetLocalIP4Proxy();
        tOspSipConf.tLocalAddr.wPort = CTasConfig::GetLocalPort4Proxy();
        tOspSipConf.tProxyAddr.tIpAddr = GetProxyIPAddress();
        tOspSipConf.tProxyAddr.wPort = GetProxyPort();
        tOspSipConf.tLocalURI.SetNOString(CTasConfig::GetTasURI());
        tOspSipConf.dwDefaultOspIID = MAKEIID(AID_CCtrl, CInstance::DAEMON);
        TSipStackConf tSipStackConf;
        tSipStackConf.ReadKeyCfg(CTasConfig::GetCfgPath());
#ifdef _LINUX_
        string strSipLogPath = strLogDir + "tas/";   //ֻҪ���õ�ģ�鼴�ɣ�kdsip���Լ�������Ŀ¼
#else
        string strSipLogPath = strLogDir + "tas\\";   //ֻҪ���õ�ģ�鼴�ɣ�kdsip���Լ�������Ŀ¼
#endif 
        tSipStackConf.tKdSipConfig.szLogPath = const_cast<LPSTR>(strSipLogPath.c_str());

        if(!OspSipInit(tOspSipConf, tSipStackConf))
        {
            PrintAndBreak("InitOspSip failed");
        }

        taslog(enLogSrcAll, CTasConfig::GetDefaultLogLevel());

		CTasConfig::SetStartTime(COspTime::GetCurrStrTime());
		if(!FastDbInit())
		{
			PrintAndBreak("FastDbInit fail,quit ... \n");
		}
        if (!RedisSetTasDbInfo(CTasConfig::GetDbIP(), CTasConfig::GetDbPort(), CTasConfig::GetDbPassword())) 
        {
            printf("Pub tas dbinfo to redis failed! dps will not work properly\n");
        }

        // ��������App
        OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), OSP_POWERON);

        if (FALSE == OspSemBCreate(&g_hTasExitSem))
        {
            PrintAndBreak("TasExitSem create failed");
        }
#ifdef _LINUX_
        OspRegQuitFunc(CBTasTerminate);
#endif // _LINUX_
        bNeedDelTasExitSem = true;
        OspSemTake(g_hTasExitSem);
        OspSemTake(g_hTasExitSem); // �ȴ��˳��ź�

        g_cNmcLib.Quit();

        printf("tas exitting step1...\n");
        OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgTasExitCmd);
        OspSemTakeByTime(g_hTasExitSem, 1000*10); // �ȴ�ҵ���ͷ���Դ
        printf("tas exitting step2...\n");

        nRet = 0;
    } while (false);

    printf("tas exitting step3...\n");
    OspSipQuit();
    printf("tas exitting step4...\n");
    OspExtQuit();
    if (bNeedDelTasExitSem) OspSemDelete(g_hTasExitSem);
    printf("tas exitted...\n");
    return nRet;
}
