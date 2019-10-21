#include "securitydbg.h"
#include "cms/ospext/osptool.h"
#include "cms/cms_proto.h"
#include "cms/ospsip/ospsip.h"
using namespace ::std;

API void securityhelp()
{
	OspPrintf(TRUE, FALSE, "%-20s: security��������\n","  securityhelp");
	OspPrintf(TRUE, FALSE, "%-20s: security�汾��Ϣ\n","  securityver");
	OspPrintf(TRUE, FALSE, "%-20s: security������Ϣ\n","  securitycfg()");
	OspPrintf(TRUE, FALSE, "%-20s: ��ӡsecurity��״̬\n","  securitystat()");
	OspPrintf(TRUE, FALSE, "%-20s: �˳�����\n","  securityquit()");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");


	OspPrintf(TRUE, FALSE, "%-20s   : ��ʾlog������Ϣ\n","  logpara()");
	OspPrintf(TRUE, FALSE, "%-20s   : ��log������Ϣ\n","  logall(u8 byLev)");
	OspPrintf(TRUE, FALSE, "%-20s   : �ر�log������Ϣ\n","  logoff()");


	OspPrintf(TRUE, FALSE, "\n  ����ģ��ʹ�õ���־�����б�:\n");
	OspPrintf(TRUE, FALSE, "  NOLOG_LEV------0            //��log���:\n");
	OspPrintf(TRUE, FALSE, "  ERROR_LEV------1            //���󼶱�:\n");
	OspPrintf(TRUE, FALSE, "  WARNING_LEV------2          //���漶��:\n");
	OspPrintf(TRUE, FALSE, "  CRITICAL_LEV------3         //�ؼ����𣬱����豸��¼�ǳ���¼�����ر�:\n");
	OspPrintf(TRUE, FALSE, "  EVENT_LEV------4            //��ͨ���𣬱����յ�һ����Ϣ��ӡһ��:\n");
	OspPrintf(TRUE, FALSE, "  PROGRESS_LEV------5         //����֪ͨ���𣬱���ǰ�˷������֪ͨ����������ѯ֪ͨ:\n");
	OspPrintf(TRUE, FALSE, "  TIMER_LEV------6            //Ƶ���Ķ�ʱ�����𣬱��綨ʱ������:\n");
	OspPrintf(TRUE, FALSE, "  ALL_LEV------7              //���м���ſ�:\n");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

	OspPrintf(TRUE, FALSE, "%-20s: ��ӡ��ǰ��������\n","  tasklist(LPCSTR szTaskType = NULL)");
	OspPrintf(TRUE, FALSE, "%-20s: ����taskNO��ӡָ������\n","  task(u32 dwTaskNO)");
	OspPrintf(TRUE, FALSE, "%-20s: traceָ������\n","  tracet(u16 wTaskNO)");
	OspPrintf(TRUE, FALSE, "%-20s: trace��һ���½�����\n","  tracent()");
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

	//����ģ���help����
	OspPrintf(TRUE, FALSE, "%-20s: osp��������\n","  osphelp()");
	OspPrintf(TRUE, FALSE, "%-20s: ospext��������\n","  ospexthelp()");
	OspPrintf(TRUE, FALSE, "%-20s: ospsip��������\n","  ospsiphelp()"); 
	OspPrintf(TRUE, FALSE, "\n------------------------------------------------------------------\n");

}



void RegSecurityCmd()
{
	OspRegCommand("securityhelp", (void *)securityhelp, "");
	OspRegCommand("securityver", (void *)securityver, "");
	OspRegCommand("securitycfg", (void *)securitycfg, "");
	OspRegCommand("securitystat", (void*)securitystat, "");
	OspRegCommand("securityquit", (void *)securityquit,"");

	OspRegCommand("logpara", (void *)logpara, "");
	OspRegCommand("loglev", (void *)loglev, "");
	OspRegCommand("logall", (void *)logall, "");
	OspRegCommand("logoff", (void *)logoff, "");


	OspRegCommand("tasklist", (void *)tasklist, "");
	OspRegCommand("task", (void *)task, "");
	OspRegCommand("tracet", (void *)tracet, "");
	OspRegCommand("tracent", (void *)tracent, "");

	OspRegCommand("newvkek", (void *)newvkek, "");
}


API void newvkek()
{
	CMessage *const pcMsg = new CMessage();
	COspSipMsg *pcOspSipMsg = new COspSipMsg();
	CSecurityModuleNewVkekReq *cReq = new CSecurityModuleNewVkekReq();
	cReq->SetModuleId("123456");
	cReq->SetVkekVersion("1.0");
	cReq->SetWithId("9999");
	//pcOspSipMsg->SetMsgBody(cReq, (cReq->ToXml()).size());
	pcMsg->content = (u8 *)pcOspSipMsg;
	pcMsg->event = SECURITY_MODULE_NEW_VKEK_REQ;
	::OspPost(COREAPPIID, SECURITY_MODULE_NEW_VKEK_REQ, (void *)pcMsg, sizeof(CMessage));
}


#define SECURITY_VER FORMAT_VER("security")


API void securityver()
{
	OspPrintf(TRUE, FALSE, SECURITY_VER);
}


//security������Ϣ
API void securitycfg()
{
	//g_cPmcsCfg.PrintData();
}


//��ʾlog������Ϣ
API void logpara()
{
	OspShowLogParam();
}


//����log����
API void loglev(u8 byLev)
{
	OspSetLogLev(byLev, AID_COREAPP);
}


API void logall(u8 byLev)
{
	OspSetLogLev(byLev, AID_COREAPP);

	OspSetModLogLev(OSPEXT, byLev);
	OspSetModLogLev(OSPSIP, byLev);

	ospsiplog(byLev);
	ospextlog(byLev);
}

API void logoff()
{
	logall(0);
}


//��ӡ��������
API void tasklist()
{
	OspPrintTask(AID_COREAPP, 0);
}


//��ӡָ��������Ϣ
API void task(u16 wTaskNO)
{
	OspPrintOneTask(wTaskNO, AID_COREAPP, 0);
}


//trace ָ������
API void tracet(u16 wTaskNO)
{
	OspTraceTask(wTaskNO);
}

//trace ��һ���½�������
API void tracent()
{
	OspTraceNextTask();
}


API void securitystat()
{
	OspPost(COREAPPIID, ModulePrintStat);
}

//�˳�cmu
API void securityquit()
{
	OspSemGive(g_hSecurityModuleExitSem);
}