#include "cms/cmu/cmu_event.h"
#include "cms/tas/tas_event.h"
#include "cms/uas/uas_event.h"
#include "cms/ospsip/ospsip.h"

#include "cmudbg.h"
#include "cmuconst.h"
#include "cms/natagent/natagent.h"

#include "cmuconf.h"
#include "system/tcmalloc/malloc_extension.h"

extern TDomainTable g_tDomainTable;
extern CCmuConf g_cCmuConf;
extern CXSemLock g_cMainSemLock;
bool g_bDebugOn = false;

//cmu��������
API void cmuhelp()
{
    //ģ���������
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "%-45s: cmu��������\n","  cmuhelp()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu�汾��Ϣ\n","  cmuver()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu������Ϣ\n","  cmucfg()");
    OspPrintf(TRUE, FALSE, "%-45s: redissdk�����Ϣ\n","  redissdk()");
    OspPrintf(TRUE, FALSE, "%-45s: cmu״̬��Ϣ\n","  cmustat()");
    OspPrintf(TRUE, FALSE, "%-45s: �˳�����\n","  cmuquit()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //��־�����������
    OspPrintf(TRUE, FALSE, "%-45s: ��ʾlog������Ϣ\n","  logpara()");
    OspPrintf(TRUE, FALSE, "%-45s: ��������ģ���ӡ����\n","  logall(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: �ر����д�ӡ\n","  logoff()");
    OspPrintf(TRUE, FALSE, "%-45s: �򿪵���ģʽ��Taskʧ��ʱ���ӡ������Ϣ��\n","  debug(bool bVal)");
    //OspPrintf(TRUE, FALSE, "%-45s: ��������App��ӡ����\n","  loglev(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����core inst��ӡ����\n","  corelog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����pui�Ự��ӡ����\n","  puilog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����cui�Ự��ӡ����\n","  cuilog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����pu�Ự��ӡ����\n","  pulog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����cu�Ự��ӡ����\n","  culog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����tas�Ự��ӡ����\n","  taslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����uas�Ự��ӡ����\n","  uaslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����nms�Ự��ӡ����\n","  nmslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ��������cmu�Ự��ӡ����\n","  domainlog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����vtdu�Ự��ӡ����\n","  vtdulog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ���ö��������ӡ����\n","  sslog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ���ý��������ӡ����\n","  switchlog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-45s: ����redis��־��ӡ����\n","  redislog(u8 byLev)");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //�����������
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡ��ǰ��������szTaskName:Ϊ��ʱ��ӡ����Task�������ӡָ������Task��\n","  tasklist(LPCSTR szTaskName)");
    OspPrintf(TRUE, FALSE, "%-45s: ����taskNO��ӡָ������\n","  task(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: ����taskNO��ӡָ�������������б�\n","  taskson(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: killָ����ʬ����\n","  IWillKillZombieTask(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: ����taskNO��ӡָ�������������б�\n","  pnst(u32 dwTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: ����SessID��ӡָ�������������б�\n","  psst(LPCSTR szSession)");
    OspPrintf(TRUE, FALSE, "%-45s: traceָ������\n","  tracet(u16 wTaskNO)");
    OspPrintf(TRUE, FALSE, "%-45s: trace��һ���½�����\n","  tracent()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //ҵ����������
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ������豸ͳ����Ϣ\n","  sumpu(LPCSTR szDomainName)");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ������豸�б�byState��0-����״̬��1-���ߣ�2-����ע�ᣩ\n","  pulist(LPCSTR szDomainName, u8 byState)");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ��ǰ�˵���ϸ��Ϣ\n","  pu(LPCSTR szDevUri)");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡ���б�\n","  domainlist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ�������Ϣ\n","  domain(LPCSTR szDomainName)");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡPuId��Uriӳ���б�\n","  map12list()");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡ����cmu�������б�\n","  cmulist()");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡvtdu�б�\n","  vtdulist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡpui�б�\n","  puilist(LPCSTR szPuiType)");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡcui�б�\n","  cuilist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡcu�б�\n","  culist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡ����ģ���б�\n","  pelist()");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡsip���������б�\n","  sslist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡsip�����������б�\n","  dsslist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡredis���������б�\n","  rsslist()");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ���豸�Ķ������б�\n","  sserlist(LPCSTR szDevUri)");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡ�����б�szDevChn��ǰ�˺�ͨ����Ϊ�ձ�ʾ���У���ʽ��DevUri + \"_\" + ͨ���ţ��磺abcd@keda_1��\n","  sl/switchlist(szDevChn)");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ��ǰ�˵Ľ�����Ϣ��szDevChn��ǰ�˺�ͨ������ʽ��DevUri + \"_\" + ͨ���ţ��磺abcd@keda_1��\n","  pusl(LPCSTR szDevChn)");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡָ��cu�Ľ�����Ϣ��szSession��cu��Session��ͨ��culist���Ի�ȡ����\n","  cusl(LPCSTR szSession)");

    OspPrintf(TRUE, FALSE, "\n");
    OspPrintf(TRUE, FALSE, "%-45s: ��ӡ�����澯�б�\n","  nmsdalist()");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡ���ظ澯�б�\n","  nmsoalist()");
	OspPrintf(TRUE, FALSE, "%-45s: NA�����������\n","  nacmd(u16 wCmdNum)");
	OspPrintf(TRUE, FALSE, "%-45s: ��ӡ���б���Ϣ\n","  pdt()");
    
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

    //����ģ���help����
    OspPrintf(TRUE, FALSE, "%-45s: osp��������\n","  osphelp()");
	OspPrintf(TRUE, FALSE, "%-45s: ospext��������\n","  ospexthelp()");
	OspPrintf(TRUE, FALSE, "%-45s: ospsip��������\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "%-45s: natagent��������\n","  nahelp()");

    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
}

#ifdef _LINUX_
void RegCmuCmd()
{
    //ģ���������
    OspRegCommandEx("cmuhelp", (void *)cmuhelp, "", "");
    OspRegCommandEx("cmuver", (void *)cmuver, "", "");
    OspRegCommandEx("cmucfg", (void *)cmucfg, "", "");
    OspRegCommandEx("redissdk", (void *)redissdk, "", "");
    OspRegCommandEx("cmustat", (void *)cmustat, "", "");
    OspRegCommandEx("cmuquit", (void *)cmuquit,"", "");

    //��־�����������
    OspRegCommandEx("logpara", (void *)logpara, "", "");
    //OspRegCommandEx("loglev", (void *)loglev, "", "i");
    OspRegCommandEx("corelog", (void *)corelog, "", "i");
    OspRegCommandEx("puilog", (void *)puilog, "", "i");
    OspRegCommandEx("cuilog", (void *)cuilog, "", "i");
    OspRegCommandEx("pulog", (void *)pulog, "", "i");
    OspRegCommandEx("culog", (void *)culog, "", "i");
    OspRegCommandEx("taslog", (void *)taslog, "", "ii");
    OspRegCommandEx("uaslog", (void *)uaslog, "", "i");
    OspRegCommandEx("nmslog", (void *)nmslog, "", "i");
    OspRegCommandEx("domainlog", (void *)domainlog, "", "i");
    OspRegCommandEx("sslog", (void *)sslog, "", "i");
    OspRegCommandEx("vtdulog", (void *)vtdulog, "", "i");
    OspRegCommandEx("switchlog", (void *)switchlog, "", "i");
    OspRegCommandEx("redislog", (void *)redislog, "", "i");
    OspRegCommandEx("logall", (void *)logall, "", "i");
    OspRegCommandEx("logoff", (void *)logoff, "", "");
    OspRegCommandEx("debug", (void *)debug, "", "i");

    //�����������
    OspRegCommandEx("tasklist", (void *)tasklist, "", "s");
    OspRegCommandEx("task", (void *)task, "", "i");
    OspRegCommandEx("taskson", (void *)taskson, "", "i");
    OspRegCommandEx("IWillKillZombieTask", (void *)IWillKillZombieTask, "", "i");
    OspRegCommandEx("pnst", (void *)pnst, "", "i");
    OspRegCommandEx("psst", (void *)psst, "", "s");
    OspRegCommandEx("tracet", (void *)tracet, "", "i");
    OspRegCommandEx("tracent", (void *)tracent, "", "");

    //ҵ����������
	OspRegCommandEx("sumpu", (void *)sumpu,"", "s");
    OspRegCommandEx("pulist", (void *)pulist,"", "si");
    OspRegCommandEx("pu", (void *)pu,"", "s");
    OspRegCommandEx("domainlist", (void *)domainlist,"", "");
    OspRegCommandEx("syncparam", (void *)syncparam,"", "si");
	OspRegCommandEx("map12list", (void *)map12list,"", "");

    OspRegCommandEx("domain", (void *)domain,"", "s");
    OspRegCommandEx("cmulist", (void *)cmulist,"", "");
	OspRegCommandEx("vtdulist", (void *)vtdulist,"", "");
    OspRegCommandEx("puilist", (void *)puilist,"", "s");
    OspRegCommandEx("cuilist", (void *)cuilist,"", "");
    OspRegCommandEx("culist", (void *)culist,"", "");
    OspRegCommandEx("pelist", (void *)pelist,"", "");

    OspRegCommandEx("sslist", (void *)sslist,"", "");
    OspRegCommandEx("dsslist", (void *)dsslist,"", "");
    OspRegCommandEx("rsslist", (void *)rsslist,"", "");
    OspRegCommandEx("sserlist", (void *)sserlist,"", "s");

    OspRegCommandEx("switchlist", (void *)switchlist,"", "s");
    OspRegCommandEx("sl", (void *)switchlist,"", "s");
	OspRegCommandEx("pusl", (void *)pusl,"", "s");
    OspRegCommandEx("cusl", (void *)cusl,"", "s"); 

	OspRegCommandEx("adt", (void *)adt,"", "ssss");
	OspRegCommandEx("pdt", (void *)pdt,"", "");
	OspRegCommandEx("area", (void *)area,"", "i");

	OspRegCommandEx("nmsdalist", (void *)nmsdalist,"", "");
	OspRegCommandEx("nmsoalist", (void *)nmsoalist,"", "");

	//NA��������
	OspRegCommandEx("nahelp", (void *)nahelp,"", "");
	OspRegCommandEx("nacmd", (void *)nacmd,"", "i");

	OspRegCommandEx("SetMemoryReleaseRate", (void *)SetMemoryReleaseRate, "", "i");
	OspRegCommandEx("GetMemoryReleaseRate", (void *)GetMemoryReleaseRate, "", "");
	OspRegCommandEx("ReleaseFreeMemory", (void *)ReleaseFreeMemory, "", "");
	OspRegCommandEx("SetMemroyReleaseTimer", (void *)SetMemroyReleaseTimer, "", "i");
	
}
#endif

#define CMU_VER FORMAT_VER("cmu")

//cmu�汾��Ϣ
API void cmuver()
{
    OspPrintf(TRUE, FALSE, CMU_VER);
}

//cmu������Ϣ
API void cmucfg()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CONF);
}

//redissdk��Ϣ
API void redissdk()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_REDISSDK);
}

//cmu״̬��Ϣ
API void cmustat()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_STAT);
}

//��ʾlog������Ϣ
API void logpara()
{
    OspShowLogParam();
}

//����log����
API void loglev(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
}

//����corelog����
API void corelog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_INS, byLev);
}

//����puilog����
API void puilog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_PUI, byLev);
}

//����cuilog����
API void cuilog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CUI, byLev);
}

//����pulog����
API void pulog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_PU, byLev);
}

//����culog����
API void culog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CU, byLev);
}

//����3aslog����
API void taslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_3AS, byLev);
}

//����uaslog����
API void uaslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_UAS, byLev);
}

//����nmslog����
API void nmslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_NMS, byLev);
}

//��������cmulog����
API void domainlog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_CMU, byLev);
}

//���ö���log����
API void sslog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_SS, byLev);
}

//����vtdulog����
API void vtdulog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_VTDU, byLev);
}

//���ý���log����
API void switchlog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_SWITCH, byLev);
}

//���ý���log����
API void redislog(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);
    OspSetModLogLev(CMU_MOD_REDIS, byLev);
}

//ͬʱ��������ģ��log����
API void logall(u8 byLev)
{
    OspSetLogLev(byLev, CMU_CORE_APP_ID);

    OspSetModLogLev(OSPEXT, byLev);
    OspSetModLogLev(OSPSIP, byLev);

    OspSetModLogLev(CMU_MOD_INS, byLev);
    OspSetModLogLev(CMU_MOD_PUI, byLev);
    OspSetModLogLev(CMU_MOD_CUI, byLev);
    OspSetModLogLev(CMU_MOD_PU, byLev);
    OspSetModLogLev(CMU_MOD_BATCH, byLev);
    OspSetModLogLev(CMU_MOD_CU, byLev);
    OspSetModLogLev(CMU_MOD_3AS, byLev);
    OspSetModLogLev(CMU_MOD_UAS, byLev);
    OspSetModLogLev(CMU_MOD_NMS, byLev);
    OspSetModLogLev(CMU_MOD_CMU, byLev);
    OspSetModLogLev(CMU_MOD_VTDU, byLev);
    OspSetModLogLev(CMU_MOD_SS, byLev);
    OspSetModLogLev(CMU_MOD_SWITCH, byLev);
    OspSetModLogLev(CMU_MOD_REDIS, byLev);

    ospsiplog(byLev);
    ospextlog(byLev);
}

//�ر�����ģ��log����
API void logoff()
{
    logall(0);
}

//�򿪵���ģʽ
API void debug(bool bVal)
{
    g_bDebugOn = bVal;
}

//��ӡ��������
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CMU_CORE_APP_ID, 0, szTaskType);
}

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CMU_CORE_APP_ID, 0);
}

//��������Ŵ�ӡָ������������񣬵�ͬ��pnst
API void taskson(u32 dwTaskNO)
{
    pnst(dwTaskNO);
}

//ɱ��ָ����ʬ�������ڹ��ϻָ����߲��ԣ�����ʹ�ö��ĵ����Ʊ����û��������
API void IWillKillZombieTask(u32 dwTaskNO)
{
    OspKillTask(dwTaskNO, CMU_CORE_APP_ID, 0);
}

//��������Ŵ�ӡָ�������������
API void pnst(u32 dwTaskNO)
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SON_TASK_LIST_BY_NO, &dwTaskNO, sizeof(dwTaskNO));
}

//����sesskon�Ŵ�ӡָ�������������
API void psst(LPCSTR szSession)
{
    if (szSession == NULL)
    {
        GLOBELOG(ERROR_LEV, "szSession == NULL\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SON_TASK_LIST_BY_SESS, szSession, (u16)strlen(szSession)+1);
}

//trace ָ������
API void tracet(u32 dwTaskNO)
{
    OspTraceTask(dwTaskNO);
}

//trace ��һ���½�������
API void tracent()
{
    OspTraceNextTask();
}

//���ߵ�pui�б�
API void puilist(LPCSTR szPuiType)
{
    if (NULL != szPuiType)
    {
        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUI_LIST, szPuiType, (u16)strlen(szPuiType) + 1);
    }
    else
    {
        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUI_LIST);
    }
}

//pu�б�
API void sumpu(LPCSTR szDomainName)
{
    TPrintDevParam tParam;

    if (NULL != szDomainName)
    {
        strncpy(tParam.m_achDomainName, szDomainName, sizeof(tParam.m_achDomainName) - 1);
    }

	::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SUM_PU, &tParam, sizeof(tParam));
}

API void pulist(LPCSTR szDomainName, u8 byState)
{
    TPrintDevParam tParam;
    tParam.m_byState = byState;

    if (NULL != szDomainName)
    {
        strncpy(tParam.m_achDomainName, szDomainName, sizeof(tParam.m_achDomainName) - 1);
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DEV_LIST, &tParam, sizeof(tParam));
}

//��ӡָ��pu��Ϣ
API void pu(LPCSTR szDevUri)
{
    if (szDevUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "puid����Ϊ��\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PU_INFO, szDevUri, (u16)strlen(szDevUri)+1);
}

//domain��Ϣ�б�
API void domainlist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_LIST);
}

//��ͬ��Task
API void syncparam(LPCSTR szDomainName, u32 dwSyncNumOnce)
{
    if (NULL != szDomainName)
    {
        TDomainSyncParam tParam;
        strncpy(tParam.szDomainName, szDomainName, sizeof(tParam.szDomainName) - 1);
        tParam.dwSyncNumOnce = dwSyncNumOnce;

        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_SYNC_PARAM, &tParam, sizeof(tParam));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "invalid param!\n");
    }
}

//{{{{{{{{{{{{{{{ add by Royan 20140513 ���ܸ澯����
//1.0puid��2.0puUriӳ���
API void map12list()
{
	::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PUID_URI_MAP_LIST);
}
//}}}}}}}}}}}}}}} add by Royan 20140513 ���ܸ澯����

//domain����
API void domain(LPCSTR szDomainName)
{
    if (szDomainName == NULL)
    {
        OspPrintf(TRUE, FALSE, "szDomainName����Ϊ��\n");
        return;
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DOMAIN_DATA, szDomainName, (u16)strlen(szDomainName)+1);
}

//���ӵ�cmu�б�
API void cmulist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CMU_LIST);
}

//���ӵ�vtdu�б�
API void vtdulist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_VTDU_LIST);
}

//���ߵ�cui�б�
API void cuilist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CUI_LIST);
}

//���ߵ�cu�б�
API void culist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CU_LIST);
}

//���ߵ������б�
API void pelist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_PE_LIST);
}

//�����豸���������б�
API void sslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SS_LIST);
}

//�����豸���������б�
API void dsslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_DSS_LIST);
}

//redis���������б�
API void rsslist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_RSS_LIST);
}

//�����豸�Ķ������б�
API void sserlist(LPCSTR szDevUri)
{
    if (szDevUri == NULL)
    {
        OspPrintf(TRUE, FALSE, "szUserSess����Ϊ��\n");
        return;
    }
	else
	{
		::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SSER_LIST, szDevUri, (u16)strlen(szDevUri)+1);
	}
}

//SwitchNode�б�,DevChn��ʽ��"7d53960277f341858f00b043fb8636a3@china_0"
API void switchlist(const char* szDevChn)
{
    TPrintSwitchParam tParam;

    if (szDevChn)
    {
		string strDevChn(szDevChn);
		string strDevUri = strDevChn.substr(0, strDevChn.find_first_of("_"));
		string strChnNO  = strDevChn.substr(strDevChn.find_first_of("_") + 1);
		strncpy(tParam.m_achDevUri, strDevUri.c_str(), sizeof(tParam.m_achDevUri) - 1);
        tParam.m_byChn = (u8)atoi(strChnNO.c_str());
    }

    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_SWITCH_NODE_LIST, &tParam, sizeof(tParam));
}

API void pusl(const char* szDevChn)
{
	if (NULL != szDevChn)
	{
		string strDevChn(szDevChn);

		TPrintSwitchParam tParam;
		string strDevUri = strDevChn.substr(0, strDevChn.find_first_of("_"));
		string strChnNO  = strDevChn.substr(strDevChn.find_first_of("_") + 1);
		strncpy(tParam.m_achDevUri, strDevUri.c_str(), sizeof(tParam.m_achDevUri) - 1);
		tParam.m_byChn = (u8)atoi(strChnNO.c_str());

		::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CERTAIN_PU_SWITCH_INFO, &tParam, sizeof(tParam));
	}
	else
	{
		OspPrintf(TRUE, FALSE, "szDevChnΪ��!!!\n");
	}
}

API void cusl(const char* szSession)
{
    if (NULL != szSession)
    {
        TPrintSwitchParam tParam;
        strncpy(tParam.m_achDevUri, szSession, sizeof(tParam.m_achDevUri) - 1);

        ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_CERTAIN_CU_SWITCH_INFO, &tParam, sizeof(tParam));
    }
    else
    {
        OspPrintf(TRUE, FALSE, "szSessionΪ��!!!\n");
    }
}

//add domain table
API void adt(LPCSTR szPeerDomainName, LPCSTR szPeerIP, LPCSTR szLocalIP, LPCSTR szOpType)
{
	TDomainItem tItem;
	tItem.SetPeerDomainName(szPeerDomainName);
	tItem.SetPeerIP(szPeerIP);
	tItem.SetLocalIP(szLocalIP);
	tItem.SetOperateType(DOMAIN_OP_TYPE_ADD);

	if (string(DOMAIN_OP_TYPE_ADD) == string(szOpType))
	{
		g_tDomainTable[szPeerDomainName] = tItem;
		OspPrintf(TRUE, FALSE, "Add Domain Table, Domain[%s]\n", szPeerDomainName);
	}
	else
	{
		g_tDomainTable.erase(szPeerDomainName);
		OspPrintf(TRUE, FALSE, "Del Domain Table, Domain[%s]\n", szPeerDomainName);
	}
}
//print domain table
API void pdt()
{
	for (TDomainTable::const_iterator it=g_tDomainTable.begin(); it!=g_tDomainTable.end(); it++)
	{
		OspPrintf(TRUE, FALSE, "Domain[%s], PeerIP[%s], LocalIP[%s], OpType[%s]\n",
			it->second.GetPeerDomainName().c_str(),
			it->second.GetPeerIP().c_str(),
			it->second.GetLocalIP().c_str(),
			it->second.GetOperateType().c_str());
	}

	OspPrintf(TRUE, FALSE, "DomainNum[%d]\n", g_tDomainTable.size());
}
//set display area info
API void area(u8 byCfg)
{
	
	bool bCfg = (0 == byCfg) ? false : true;
	g_cCmuConf.UpdateDisplayAreaInfoSwitch(bCfg);
}


//��ӡ�����澯�б�
API void nmsdalist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_NMS_DIS_ALARM_LIST);    
}

//��ӡ���ظ澯�б�
API void nmsoalist()
{
    ::OspPost(CMU_CORE_DAEMON, CMU_PRINT_NMS_OVERLOAD_ALARM_LIST);
}

//Natagent����
API void nahelp()
{
	NAHelp();
}

//Natagent�����������
API void nacmd(u16 wCmdNum)
{
	NACmd(wCmdNum);
}

//�˳�cmu
API void cmuquit()
{
    g_cMainSemLock.Give();
}


API void SetMemoryReleaseRate(int nRange)
{
	double range = nRange;
	OspPrintf(TRUE, FALSE, "Set Memory Release Rate = %g\n", range);
	MallocExtension::instance()->SetMemoryReleaseRate(range);
}
API void GetMemoryReleaseRate()
{
	double range = MallocExtension::instance()->GetMemoryReleaseRate();
	OspPrintf(TRUE, FALSE, "Memory Release Rate = %g\n", range);
}
API void ReleaseFreeMemory()
{
	MallocExtension::instance()->ReleaseFreeMemory();
}
API void SetMemroyReleaseTimer(int nSeconds)
{	
	OspPrintf(TRUE, FALSE, "Memory Release Timer from %d change to %d sec\n", g_cCmuConf.GetTcmallocReleaseInterval(), nSeconds);
	g_cCmuConf.SetTcmallocReleaseInterval(nSeconds);	
}
//��ʼ����Ϣ����
void InitCmuMsgDesc()
{
    //�ڲ���Ϣ
    OSP_ADD_EVENT_DESC(CMU_START);
    OSP_ADD_EVENT_DESC(CMU_QUIT);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CONF);
    OSP_ADD_EVENT_DESC(CMU_PRINT_REDISSDK);
    OSP_ADD_EVENT_DESC(CMU_PRINT_STAT);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PUI_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PU_INFO);
	OSP_ADD_EVENT_DESC(CMU_PRINT_DOMAIN_LIST);
	OSP_ADD_EVENT_DESC(CMU_PRINT_PUID_URI_MAP_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_DOMAIN_DATA);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CUI_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_VTDU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CMU_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_PE_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_DSS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_RSS_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SSER_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SWITCH_NODE_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_NMS_DIS_ALARM_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_NMS_OVERLOAD_ALARM_LIST);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SON_TASK_LIST_BY_NO);
    OSP_ADD_EVENT_DESC(CMU_PRINT_SON_TASK_LIST_BY_SESS);
    OSP_ADD_EVENT_DESC(CMU_CMU_DELETE);
    OSP_ADD_EVENT_DESC(CMU_CMU_DISCONNECT);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_SUCCESS);
    OSP_ADD_EVENT_DESC(CMU_CMU_CONNECT_FAIL);
    OSP_ADD_EVENT_DESC(CMU_PU_STATUS_UPDATE);
    OSP_ADD_EVENT_DESC(CMU_SWITCH_FINISH_CB);
    OSP_ADD_EVENT_DESC(CMU_DEV_ONLINE_NTF);
    OSP_ADD_EVENT_DESC(CMU_DEV_OFFLINE_NTF);
	OSP_ADD_EVENT_DESC(CMU_PRINT_CERTAIN_PU_SWITCH_INFO);
    OSP_ADD_EVENT_DESC(CMU_PRINT_CERTAIN_CU_SWITCH_INFO);

    OSP_ADD_EVENT_DESC(CMU_FASTDB_SUB_CB);
    OSP_ADD_EVENT_DESC(CMU_REDISSDK_DISC_CB);
    OSP_ADD_EVENT_DESC(CMU_REDISSDK_SUB_CB);
    OSP_ADD_EVENT_DESC(CMU_DOMAIN_REDIS_SYNC);
    OSP_ADD_EVENT_DESC(CMU_SLIDING_WINDOW_PROC);

    //������Ϣ
    InitCmsEventDesc();
    InitTacTasEventDesc();
    InitUasEventDesc();
}

//��ʼ��cmu�ڸ�ģ����־����
void InitCmuModuleLog()
{

#ifdef DEBUG
    u8 byCmuLogLev = EVENT_LEV;
#else
    u8 byCmuLogLev = ERROR_LEV;
#endif

    OSP_SET_MOD_LOG(CMU_MOD_INS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CUI, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_PUI, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_PU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_BATCH, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_3AS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_UAS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_NMS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_CMU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_VTDU, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_SWITCH, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_SS, byCmuLogLev);
    OSP_SET_MOD_LOG(CMU_MOD_REDIS, byCmuLogLev);
}