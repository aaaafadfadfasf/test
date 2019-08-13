#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/natagent/natagent.h"
#include "cmucoreapp.h"
#include "cmuconf.h"
#include "cmudbg.h"
#include "nmstask.h"
#include "cmudata.h"
#include "system/tcmalloc/malloc_extension.h"

CXSemLock g_cMainSemLock;

void NALogCB(EmLogLev emLogLev, const s8 *pchLog)
{
	u8 byLogLevel = EVENT_LEV;
	switch(emLogLev)
	{
	case emError:
		byLogLevel = ERROR_LEV;
		break;
	case emWarning:
		byLogLevel = WARNING_LEV;
		break;
	case emInfo:
		byLogLevel = CRITICAL_LEV;
		break;
	default:break;
	}

	GLOBELOG(byLogLevel, "%s", pchLog);
}

void NAQryAddrCB(EmResult emResult,
    u32                 dwStunResultNum,
    TStunResult        *ptStunResultList,
    u32                 dwSeqNum,
    void               *context)
{
    //do nothing
}

void NAPunchResultCB(EmResult emResult,
    u32                 dwPunchResultNum,
    TPunchResult       *ptPunchResultList,
    u32                 dwSeqNum,
    void               *context)
{
    //do nothing
}

int main(int argc, char* argv[])
{
    //��ȡcmu�����ļ�
    if (g_cCmuConf.ReadConf(cmu_config) == FALSE)
    {
        printf("��ȡ�����ļ�ʧ��\n");
        return 0;
    }
    printf("��ȡ�����ļ��ɹ�!\n");
	int nTMLevel = g_cCmuConf.GetTcmallocLevel();
	printf("tcmalloc level = %d\n", nTMLevel);
	MallocExtension::instance()->SetMemoryReleaseRate(nTMLevel);

    // ���ݳ�ʼ��
    CRedisPipe::SetRedisPipeLogFunc(cbRedisLog);
    CCommonConfig cConfig = GetCommonConfig();
    if (!CCmuData::GetInstance()->Initialize(cConfig.GetDomainUUID(), cConfig.GetDomainName(), 
        cConfig.GetRedisIP(), cConfig.GetRedisPort(), cConfig.GetRedisPassword()))
    {
        printf("���ݳ�ʼ��ʧ��!\n");
        return 0;
    }

    string strLogDir = GetLogDir();
    TOspExtConf tOspExtConf;
#ifdef _LINUX_
    tOspExtConf.strErrLogDir = strLogDir + "cmu/errlog/";
    tOspExtConf.strRunLogDir = strLogDir + "cmu/runlog/";
#else
    tOspExtConf.strErrLogDir = strLogDir + "cmu\\errlog\\";
    tOspExtConf.strRunLogDir = strLogDir + "cmu\\runlog\\";
#endif

    //��ʼ��OSPEXT
	tOspExtConf.ReadKeyCfg(cmu_config);
    if (FALSE == OspExtInit(tOspExtConf))
    {
        printf("ospext��ʼ��ʧ��\n");
        return 0;
    }
    printf("ospext��ʼ���ɹ�!\n");

    //linux��ע��telnet��������
#ifdef _LINUX_
    RegCmuCmd();
#endif

    //��ʼ����Ϣ����
    InitCmuMsgDesc();

    //��ʼ��cmu�ڸ�ģ����־����
    InitCmuModuleLog();

    //����OSP APP
    if (g_cCmuCoreApp.CreateOspApp("cmucore", CMU_CORE_APP_ID, 80, CMU_CORE_APP_QUEUE_SIZE) == FALSE)
    {
        printf("CreateOspApp fail!!!\n");
        return 0;
    }

    //��ʼ��SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr = g_cCmuConf.m_tLocalAddr;
    tOspSipConf.tProxyAddr = g_cCmuConf.m_tProxyAddr;
    tOspSipConf.tLocalURI = g_cCmuConf.m_tLocalUri;
    tOspSipConf.dwDefaultOspIID = CMU_CORE_DAEMON;
    tOspSipConf.tUAType = UA_TYPE_CMU;

    TSipStackConf tSipStackConf;
    tSipStackConf.ReadKeyCfg(cmu_config);
    //tSipStackConf.tKdSipConfig.nReceiveBufferSize = (35*1024);
#ifdef _LINUX_
    string strSipLogPath = GetLogDir() + "cmu/";   //ֻҪ���õ�ģ�鼴�ɣ�kdsip���Լ�������Ŀ¼
#else
    string strSipLogPath = GetLogDir() + "cmu\\";
#endif
    tSipStackConf.tKdSipConfig.szLogPath = (LPSTR)strSipLogPath.c_str();
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }
    printf("OspSip��ʼ���ɹ�!\n");

    //��ʼ�����ܿ�
    if(!g_cNmsLib.Init(g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalUri.GetUser(),
                   g_cCmuConf.m_tLocalAddr.GetIpStr(),
                   GetNmsPort(),
                   g_cCmuConf.m_tLocalAddr.GetIpStr()))
    {
        printf("InitNmsLib fail!!!\n");
        return 0;
    }
    printf("NmsLib��ʼ���ɹ�!!!\n");

    //��ʼ��natagent
    TInitInfo tInitInfo;
    NASetInitInfoToNULL(&tInitInfo);

    strcpy(tInitInfo.m_tSTUNAddr.m_achIP, ::GetStunIPAddress().c_str());
    tInitInfo.m_tSTUNAddr.m_wPort = ::GetStunPort();

    tInitInfo.m_fLogPrinter = &NALogCB;
    tInitInfo.m_fQueryMappedAddr = &NAQryAddrCB;
    tInitInfo.m_fPunchResult = &NAPunchResultCB;

    if (NATAGENT_OK != NAInitLib(&tInitInfo))
    {
        printf("Failed to NAInitLib!\n");
        return 0;
    }
    printf("NatagentLib��ʼ���ɹ�!!!\n");

    //����coreApp
    ::OspPost(CMU_CORE_DAEMON, CMU_START);

    printf("cmu�����ɹ�!\n");
    GLOBELOG(CRITICAL_LEV, "cmu�����ɹ�!\n");

	//����tcmalloc�Զ��ͷ��ڴ�task
	int nReleaseInterval = g_cCmuConf.GetTcmallocReleaseInterval();
	GLOBELOG(CRITICAL_LEV, "tcmalloc release interval = %d\n", nReleaseInterval);

    //�������߳�
    g_cMainSemLock.Take();
    g_cMainSemLock.Take();

    //coreApp�˳�
    ::OspPost(CMU_CORE_DAEMON, CMU_QUIT);
    g_cMainSemLock.Take();

    //�˳�NA
    NAQuitLib();

    //�˳�����
    g_cNmsLib.Quit();
    
    //�˳�SIP
    OspSipQuit();

    //�˳�OSPEXT
    OspExtQuit();

    return 0;
}







