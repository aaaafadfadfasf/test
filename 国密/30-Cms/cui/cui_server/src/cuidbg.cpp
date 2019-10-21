#include "cuidbg.h"
#include "cuiapp.h"
#include "sdk/redissdk/redissdk.h"

#define PrintFunGroupDesc(x) OspPrintf(TRUE, FALSE, x"\n")
#define PrintFunDesc(fun, funDesc) OspPrintf(TRUE, FALSE, "\t%2d "#fun":\t"funDesc"\n", wIndex++)

extern CRedisSdk  g_cCuiRedisSdk;

API void cuihelp()
{
    cuiver();
    u16 wIndex = 1;
    PrintFunGroupDesc("\nCUI��������");
    PrintFunDesc(cuihelp(), "CUI��������");
    PrintFunDesc(cuiver(), "��ӡCUI�汾��Ϣ�ͱ�����Ϣ");
    PrintFunDesc(cuicfg(), "��ӡCUI������Ϣ");
    PrintFunDesc(cuistat(), "��ӡCUI��״̬");
    PrintFunDesc(cuiquit(), "�˳�CUI");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    PrintFunDesc(logpara(),"��ʾlog������Ϣ\n");
    PrintFunDesc(logall(u32 bLogAll), "������ģ������еȼ�����־����־����μ�logparam");
    PrintFunDesc(logoff(), "�ر�����ģ������еȼ�����־");
    PrintFunDesc(cuilogvc(u32 bLogErr), "��APPģ�����־, 0 ��ʾEVENT�ȼ���1 ��ʾERROR�ȼ�");
    PrintFunDesc(cuilogerr(), "��������ģ��Ĵ��󼶱����־");
    PrintFunDesc(cuilogtask(u32 bLogErr), "��LOGIN_TASKģ�����־, 0 ��ʾEVENT�ȼ���1 ��ʾERROR�ȼ�");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    //�����������
	PrintFunDesc(vtdunatclear(LPCSTR szUserSess), "�����ǰ�û�����NAT̽�����Ϣ,cuSessionΪ�����������nat̽���");
	PrintFunDesc(getvslist(), "��ӡCUI�������ƵԴ״̬�б�");
    PrintFunDesc(tasklist(LPCSTR szTaskType), "��ӡ��ǰ��������");
    PrintFunDesc(task(u32 dwTaskNO), "����taskNO��ӡָ������");
    PrintFunDesc(taskson(u32 dwTaskNO), "����taskNO��ӡָ�������������б�");
    PrintFunDesc(IWillKillZombieTask(u32 dwTaskNO), "killָ����ʬ����");
    PrintFunDesc(tracet(u16 wTaskNO), "traceָ������");
    PrintFunDesc(tracent(), "trace��һ���½�����");
    PrintFunDesc(cuitaskshow(), "��ӡ����Task��Ϣ�����ts()");
    PrintFunDesc(taslist(u32 bPrintUserDevList), "��ӡ����TAS��Ϣ��Ĭ�� 0 ��ʾ����ӡ�û��豸�б�1 ��ʾ��ӡ�û��豸�б�");
    PrintFunDesc(ds(), "��ӡ����SIP Dialog����Ϣ");
    PrintFunDesc(hb(), "��ӡ����SIP��HeartBeat�б�");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    PrintFunDesc(culist(u32 bPrintSubTask), "��ӡ����CU�б�Ĭ�� 0 ��ʾ����ӡ�������б�1 ��ʾ��ӡ���������ϸ��Ϣ");
    PrintFunDesc(domain(), "��ӡ���б���Ϣ");
    PrintFunDesc(taskmap(), "��ӡTaskӳ���Ŀǰ�������б�");
    PrintFunDesc(mailsize(), "��ӡ�����е���Ϣ����");
    PrintFunDesc(pe(), "��ӡ��ǰ���ӵ�����ģ��");
    PrintFunDesc(userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri), "��ӡ�û��豸�����Ϣ���������������:�û�ID���豸ID");
    PrintFunDesc(pudata(LPCSTR szDevUrl), "�鿴��ȡ�����豸���ƣ���ƵԴ����, ָ���豸�Ķ������");
    PrintFunDesc(pcq(LPCSTR szCuSession), "��ӡָ��cu�Ự��������Ϣ����,�Ự��Ϊ��ʱ��ʾ����ӡ");
    PrintFunDesc(toptime(), "�鿴�ȽϺ�ʱ���¼�");
    PrintFunDesc(settoptime(u32 dwNum), "��Ϊ0ʱ���toptime������Ϣ,Ϊ��0ʱ���ö��д�С");
	PrintFunDesc(getptzlock(LPCSTR szDevId), "�鿴�豸������PTZ����ƵԴ��devIdΪ��ʱ��ӡ������PTZ��������ƵԴ");
	PrintFunDesc(getbatopt(), "�鿴CU�������������");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
	OspPrintf(TRUE, FALSE, "���� CUI������Ϣ\n");
	PrintFunDesc(setcuntfsize(s32 nCuNtfMaxSize), "����CU֪ͨ��Ϣ����������ƣ�Ĭ��Ϊ0(int ���ֵ2147483647), ��Сֵ1000");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
    OspPrintf(TRUE, FALSE, "%-20s: osp��������\n","  osphelp()");
    OspPrintf(TRUE, FALSE, "%-20s: ospext��������\n","  ospexthelp()");
    OspPrintf(TRUE, FALSE, "%-20s: ospsip��������\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "%-20s: vtdu��������\n","  vtduhelp()");
    OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");
}

void CuiRegHelpCmd()
{
#ifdef _LINUX_
	OspRegCommandEx("cuihelp", (void*)cuihelp, "", "");
	OspRegCommandEx("cuiver", (void*)cuiver, "", "");
	OspRegCommandEx("cuicfg", (void*)cuicfg, "", "");
	OspRegCommandEx("cuistat", (void*)cuistat, "", "");
	OspRegCommandEx("cuiquit", (void*)cuiquit, "", "i");
	OspRegCommandEx("logpara", (void*)logpara, "", "");                   
	OspRegCommandEx("logall", (void*)logall, "", "i");
	OspRegCommandEx("logoff", (void*)logoff, "", "");
	OspRegCommandEx("cuilogvc", (void*)cuilogvc, "", "i");
	OspRegCommandEx("cuilogerr", (void*)cuilogerr, "", "");
	OspRegCommandEx("cuilogtask", (void*)cuilogtask, "", "i");
	OspRegCommandEx("vtdunatclear", (void*)vtdunatclear, "", "s");
	OspRegCommandEx("getptzlock", (void*)getptzlock, "", "s");
	OspRegCommandEx("getvslist", (void*)getvslist, "", "");
	OspRegCommandEx("tasklist", (void*)tasklist, "", "s");
	OspRegCommandEx("task", (void*)task, "", "i");
	OspRegCommandEx("taskson", (void*)taskson, "", "i");
	OspRegCommandEx("IWillKillZombieTask", (void*)IWillKillZombieTask, "", "i");
	OspRegCommandEx("tracet", (void*)tracet, "", "i");
	OspRegCommandEx("tracent", (void*)tracent, "", "");
	OspRegCommandEx("cuitaskshow", (void*)cuitaskshow, "", "");
	OspRegCommandEx("ts", (void*)ts, "", "");
	OspRegCommandEx("ds", (void*)ds, "", "");
	OspRegCommandEx("hb", (void*)hb, "", "");
	OspRegCommandEx("culist", (void*)culist, "", "i");
	OspRegCommandEx("taslist", (void*)taslist, "", "i");
	OspRegCommandEx("domain", (void*)domain, "", "");
	OspRegCommandEx("taskmap", (void*)taskmap, "", "");
	OspRegCommandEx("mailsize", (void*)mailsize, "", "");
	OspRegCommandEx("pe", (void*)pe, "", "");
	OspRegCommandEx("userdevinfo", (void*)userdevinfo, "", "s");
	OspRegCommandEx("pudata", (void*)pudata, "", "s");
	OspRegCommandEx("pcq", (void*)pcq, "", "s");
	OspRegCommandEx("toptime", (void*)toptime, "", "");
	OspRegCommandEx("settoptime", (void*)settoptime, "", "i");
	OspRegCommandEx("logparam", (void*)logparam, "", "");
	OspRegCommandEx("pt", (void*)pt, "", "i");
	OspRegCommandEx("tt", (void*)tt, "", "i");
	OspRegCommandEx("emulate_sipmsg", (void*)emulate_sipmsg, "", "iiis");
	OspRegCommandEx("setcuntfsize", (void*)setcuntfsize, "", "i");
	OspRegCommandEx("getbatopt", (void*)getbatopt, "", "");
	OspRegCommandEx("redissdk", (void*)GetRedis, "", "");

#endif
}

API void GetRedis()
{
    g_cCuiRedisSdk.PrintData();
}

API void logpara()
{
    OspShowLogParam();
}

API void logall(u8 bLogAll)
{
//     u8 byLogLev = EVENT_LEV;
//     if ( bLogAll )
//     {
//         byLogLev = ALL_LEV;
//     }
    liblog(bLogAll);

    OspSetLogLev(bLogAll, CUIAPP_AID);
    OSP_SET_MOD_LOG(CUI_APP, bLogAll);
    OSP_SET_MOD_LOG(CUI_DATA, bLogAll);
    OSP_SET_MOD_LOG(LOGIN_TASK, bLogAll);
    OSP_SET_MOD_LOG(NORMAL_TASK, bLogAll);
	OSP_SET_MOD_LOG(TCP_THREAD, bLogAll);
	OSP_SET_MOD_LOG(REDIS_LOG, bLogAll);
}

void liblog(u8 byLog)
{
    ospsiplog(byLog);
    ospextlog(byLog);
    OSP_SET_MOD_LOG(OSPEXT, byLog);
    OSP_SET_MOD_LOG(OSPSIP, byLog);
}

API void logoff()
{
    ospsiplog(NOLOG_LEV);
    ospextlog(NOLOG_LEV);
    OspSetLogLev(NOLOG_LEV, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, NOLOG_LEV);
    OSP_SET_MOD_LOG(CUI_DATA, NOLOG_LEV);
    OSP_SET_MOD_LOG(LOGIN_TASK, NOLOG_LEV);
    OSP_SET_MOD_LOG(NORMAL_TASK, NOLOG_LEV);
    OSP_SET_MOD_LOG(TCP_THREAD, NOLOG_LEV);
}

API void cuilogvc( u32 bLogErr )
{
    u8 byLogLev  = ERROR_LEV;
    if ( bLogErr == 0 )
    {
        byLogLev  = EVENT_LEV;
    }
    ospsiplog(byLogLev);
    ospextlog(byLogLev);
    OspSetLogLev(byLogLev, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, byLogLev);
}

API void cuilogerr()
{
    ospsiplog(ERROR_LEV);
    ospextlog(ERROR_LEV);
    OspSetLogLev(ERROR_LEV, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(CUI_APP, ERROR_LEV);
    OSP_SET_MOD_LOG(CUI_DATA, ERROR_LEV);
    OSP_SET_MOD_LOG(LOGIN_TASK, ERROR_LEV);
    OSP_SET_MOD_LOG(NORMAL_TASK, ERROR_LEV);
    OSP_SET_MOD_LOG(TCP_THREAD, ERROR_LEV);
}

API void cuilogtask( u32 bLogErr )
{
    u8 byLogLev  = ERROR_LEV;
    if ( bLogErr == 0 )
    {
        byLogLev  = EVENT_LEV;
    }
    ospsiplog(byLogLev);
    ospextlog(byLogLev);
    OspSetLogLev(byLogLev, CUIAPP_AID, 0);
    OSP_SET_MOD_LOG(LOGIN_TASK, byLogLev);
    OSP_SET_MOD_LOG(NORMAL_TASK, byLogLev);
}

//��ӡ��������
API void tasklist(LPCSTR szTaskType)
{
    OspPrintTask(CUIAPP_AID, 0, szTaskType);
}

//��ӡָ��������Ϣ
API void task(u32 dwTaskNO)
{
    OspPrintOneTask(dwTaskNO, CUIAPP_AID, 0);
}

//��������Ŵ�ӡָ�������������
API void taskson(u32 dwTaskNO)
{
    culist(dwTaskNO);
}

//ɱ��ָ����ʬ�������ڹ��ϻָ����߲��ԣ�����ʹ�ö��ĵ����Ʊ����û��������
API void IWillKillZombieTask(u32 dwTaskNO)
{
    OspKillTask(dwTaskNO, CUIAPP_AID, 0);
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

API void culist(u32 bPrintSubTask)
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_CU_LIST, &bPrintSubTask, sizeof(bPrintSubTask));
}

API void cuitaskshow()
{
    OspPrintTask(CUIAPP_AID);
}

API void ts()
{
    cuitaskshow();
}


API void taslist(u32 bPrintUserDevList)
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_TAS_LIST, &bPrintUserDevList, sizeof(bPrintUserDevList));
}

API void domain()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_DOMAIN_LIST);
}

API void ds()
{
    OspPrintf(TRUE, FALSE, "-----------------------OSP SIP��Dialog�б�-----------------------\n");
    ospsipdlg();
    OspPrintf(TRUE, FALSE, "-----------------------OSP SIP��Transaction�б�------------------\n");
    ospsiptrans();
}

API void hb()
{
    ospsiphblist();
}

API void cuiver()
{
    OspPrintf(TRUE, FALSE, CUI_VER);
}

API void cuicfg()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_CONFIG);
}

API void logparam()
{
    OspShowLogParam();
}

API void pt( u32 dwTaskNO )
{
    OspPrintOneTask(dwTaskNO, CUIAPP_AID, 0);
}

API void tt( u32 dwTaskNO )
{
    OspTraceTask(dwTaskNO);
}

API void cuistat()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_STATUS);
}

API void taskmap()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_TASK_MAP);
}

API void mailsize()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_MAIL_SIZE);
}

API void pe()
{
    OspPost(CUIAPP_DAEMON, CUI_PRINT_PE_AGENT);
}

API void userdevinfo(LPCSTR szUserSess, LPCSTR szDevUri/* = NULL*/)
{
	TUserDevData tUserDevData;
    if(szUserSess == NULL)
    {
        OspPrintf(TRUE, FALSE, "szUserSess����Ϊ��\n");
        return;
    }

	string strUserUUid(szUserSess);

	TSipURI tSipUri;
	if (tSipUri.SetURIString(strUserUUid))
	{
		strUserUUid = tSipUri.GetUser();
	}
	strncpy(tUserDevData.szDevUUid, strUserUUid.c_str(), sizeof(s8) * 32);

	if (szDevUri != NULL)
	{
		string strDevUUid(szDevUri);

		if ( tSipUri.SetURIString(strDevUUid) )
		{
			strDevUUid = tSipUri.GetUser();
		}
		
		strncpy(tUserDevData.szDevUUid, strDevUUid.c_str(), sizeof(s8)*32);
	}

    ::OspPost(CUIAPP_DAEMON, CUI_PRINT_USER_DEVINFO, &tUserDevData, sizeof(TUserDevData));
}

API void pudata(LPCSTR szDevUrl)
{
    if(szDevUrl == NULL)
	{
		OspPrintf(TRUE, FALSE, "szDevUrl����Ϊ��\n");
        return;
    }
    ::OspPost(CUIAPP_DAEMON, CUI_PRINT_PUDATA, szDevUrl, (u16)(strlen(szDevUrl)+1));
}

API void toptime()
{
    g_cElapsedTimeList.PrintData();
}

API void settoptime(u32 dwNum)
{
    if(0 == dwNum)
    {
        g_cElapsedTimeList.Clear();
    }
    else
    {
        g_cElapsedTimeList.SetMaxNum(dwNum);
    }
}

API void pcq(LPCSTR szCuSession)
{
    if (NULL == szCuSession)
    {
        ::OspPost(CUIAPP_DAEMON, CUI_PRINT_CUREQ);
    }
    else
    {
        ::OspPost(CUIAPP_DAEMON, CUI_PRINT_CUREQ, szCuSession, (u16)(strlen(szCuSession)+1));
    }
}

API void emulate_sipmsg(u32 dwTaskNo, s32 nSipError, s32 nEvent, LPCSTR szEventContent)
{
    TCuiEmulateSipMsg tMsg;
    tMsg.dwDstTaskno = dwTaskNo;
    tMsg.nSipErrorno = nSipError;
    tMsg.nEvent = nEvent;
    if (NULL != szEventContent)
    {
        strncpy(tMsg.szMsgBody, szEventContent, sizeof(tMsg.szMsgBody));
        tMsg.szMsgBody[sizeof(tMsg.szMsgBody)-1] = '\0';
    }
    
    ::OspPost(CUIAPP_DAEMON, CUI_EMULATE_SIPMSG, &tMsg, sizeof(tMsg));
}

API void vtdunatclear(LPCSTR szUserSess) //����û�NAT̽���
{
	u16 len = 0;
	if ( szUserSess == NULL )
	{
		::OspPrintf(TRUE,FALSE,"ClearAllVtduNatData\n");
	}
	else
	{
		len=(u16)(strlen(szUserSess)+1);
	}
	
	::OspPost(CUIAPP_DAEMON,CUI_CLEAR_VTDUNATPACKET,szUserSess, len);
}

API void getptzlock(LPCSTR szDevId)
{
	u16 len = 0;
	if (szDevId == NULL)
	{
		::OspPrintf(TRUE, FALSE, "��ӡ���б�PTZ�������豸��ƵԴ\n");
	}
	else
	{
		len = (u16)(strlen(szDevId) + 1);
	}

	::OspPost(CUIAPP_DAEMON, CUI_PRINT_PTZ_LOCK_DEV_VS_LIST, szDevId, len);
}

API void getvslist()
{
	OspPost(CUIAPP_DAEMON, CUI_PRINT_VS_STATUS_LIST);
}

API void setcuntfsize(s32 nCuNtfMaxSize /*= 0*/)
{
	OspPost(CUIAPP_DAEMON, CUI_SET_CU_NTF_MAX_SIZE, &nCuNtfMaxSize, sizeof(nCuNtfMaxSize));
}

API void getbatopt()
{
	OspPost(CUIAPP_DAEMON, CUI_PRINT_CU_BAT_OPT_LIST);
}
