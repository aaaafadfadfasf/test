/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : main.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: Client��main
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
******************************************************************************/
#include "clientinst.h"
#include <iostream>
#include <fstream>

#define CLIENT_DAEMON (MAKEIID(CLIENT_AID, CInstance::DAEMON))

API void reg(char *pszClientName)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
        return;
    }
    char szBuf[512] = {0};
    char *pTmp = szBuf;
    strncpy(pTmp, pszClientName, 511);
    OspPost(CLIENT_DAEMON, EV_START_REG, szBuf, sizeof(szBuf));
}

API void unreg(char *pszClientName)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
        return;
    }
    char szBuf[512] = {0};
    char *pTmp = szBuf;
    strncpy(pTmp, pszClientName, 511);
    OspPost(CLIENT_DAEMON, EV_STOP_REG, szBuf, sizeof(szBuf));
}

API void msg(char *pszClientName, const char *pMsg)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
        return;
    }
    char szBuf[1024] = {0};
    char *pTmp = szBuf;
    strncpy(pTmp, pszClientName, 511);
    pTmp += 512;
    strncpy(pTmp, pMsg, 511);
    OspPost(CLIENT_DAEMON, EV_SEND_MSG, szBuf, sizeof(szBuf));
}

API void ss(char *pszClientName)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
        return;
    }
    char szBuf[512] = {0};
    char *pTmp = szBuf;
    strncpy(pTmp, pszClientName, 511);
    OspPost(CLIENT_DAEMON, EV_START_SS, szBuf, sizeof(szBuf));
}

API void ns(char *pszClientName)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
        return;
    }
    char szBuf[512] = {0};
    char *pTmp = szBuf;
    strncpy(pTmp, pszClientName, 511);
    OspPost(CLIENT_DAEMON, EV_STOP_SS, szBuf, sizeof(szBuf));
}

API void in(char *pszClientName)
{
	if ( NULL == pszClientName )
	{
		OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
		return;
	}
	char szBuf[512] = {0};
	char *pTmp = szBuf;
	strncpy(pTmp, pszClientName, 511);
	OspPost(CLIENT_DAEMON, EV_START_IN, szBuf, sizeof(szBuf));
}

API void ni(char *pszClientName)
{
	if ( NULL == pszClientName )
	{
		OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
		return;
	}
	char szBuf[512] = {0};
	char *pTmp = szBuf;
	strncpy(pTmp, pszClientName, 511);
	OspPost(CLIENT_DAEMON, EV_STOP_IN, szBuf, sizeof(szBuf));
}



API void sendinfo(char *pszClientName)
{
	if ( NULL == pszClientName )
	{
		OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
		return;
	}
	char szBuf[512] = {0};
	char *pTmp = szBuf;
	strncpy(pTmp, pszClientName, 511);
	OspPost(CLIENT_DAEMON, EV_SEND_INFO, szBuf, sizeof(szBuf));
}

API void sendinmessage(char *pszClientName)
{
	if ( NULL == pszClientName )
	{
		OspPrintf(TRUE, FALSE, "��Ч�Ŀͻ�����\n");
		return;
	}
	char szBuf[512] = {0};
	char *pTmp = szBuf;
	strncpy(pTmp, pszClientName, 511);
	OspPost(CLIENT_DAEMON, EN_SEND_DLG_MSG, szBuf, sizeof(szBuf));
}

inline void InitInnerEventDesc()
{
    OSP_ADD_EVENT_DESC(EV_START_REG);
    OSP_ADD_EVENT_DESC(EV_STOP_REG);
    OSP_ADD_EVENT_DESC(EV_SEND_MSG);
    OSP_ADD_EVENT_DESC(EV_START_SS);
    OSP_ADD_EVENT_DESC(EV_STOP_SS);
	OSP_ADD_EVENT_DESC(EV_START_IN);
	OSP_ADD_EVENT_DESC(EV_STOP_IN);
    OSP_ADD_EVENT_DESC(EV_SEND_INFO);
    OSP_ADD_EVENT_DESC(EN_SEND_DLG_MSG);
}

const int _TEST_NUM = 10000;
API void testreg()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		reg(sztemp);
		Sleep(20);
	}
}

API void testunreg()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		unreg(sztemp);
		Sleep(30);
	}
}

API void testss()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		ss(sztemp);
		Sleep(140);
	}
}

API void testns()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		ns(sztemp);
		Sleep(150);
	}
}

API void testin()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		in(sztemp);
		Sleep(30);
	}
}

API void testni()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		ni(sztemp);
		Sleep(40);
	}
}

API void testsendinfo()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		sendinfo(sztemp);
		Sleep(20);
	}
}

API void testsendinmessage()
{
	for(int i=0;i<_TEST_NUM;i++)
	{
		char sztemp[20] = {0};
		sprintf(sztemp, "%d", i);
		sendinmessage(sztemp);
		Sleep(20);
	}
}

typedef COspApp<CClientInst, 64> CClientApp;

CClientApp g_cClientApp;
CXSemLock g_MainSemLock;

s8 g_szUserName[256] = {0}; //ע�ᵽProxy��UA����

API void quit()
{
	g_MainSemLock.Give();
}

int main()
{
    //printf("please input your UserName: ");
    //scanf("%s", g_szUserName);
    memcpy(g_szUserName,"Client",sizeof("Client"));

    //��ʼ��OSP
    OspInit(TRUE, 2500, "client");
    /*
    ����һ��OspAppExt������Task�����ܣ��Ƽ�ȫ��ֻ��һ��OspAppExt
    */
    g_cClientApp.CreateOspApp("Client", CLIENT_AID, 80, 5000);

//     u32 adwIPList[20] = {0};
//     u16 wNum = 20;
//     OspAddrListGet(adwIPList, wNum);

    //��ʼ��SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr.tIpAddr = LOCAL_IP;
    tOspSipConf.tLocalAddr.wPort = CLIENT_SIP_LOCAL_PORT;
    tOspSipConf.tProxyAddr.tIpAddr = PROXY_IP;
    tOspSipConf.tProxyAddr.wPort = PROXY_SIP_PORT;
    tOspSipConf.tLocalURI.SetNOString(CLIENT_URI);
    tOspSipConf.tLocalURI.SetUser(g_szUserName);
    /*
    dwDefaultOspIID���������ڣ���OspSip�յ�һ������������ʱ�����ڸ�����û�а��κ�AppIID����˾ͻᷢ�͸�һ��Ĭ�ϵ�App����
    ͬʱ����Proxyע��ɹ�/Proxy����/UA�����ȶ��ᷢ�͸�Ĭ��App����
    */
    tOspSipConf.dwDefaultOspIID = MAKEIID(CLIENT_AID, CInstance::DAEMON);

    TSipStackConf tSipStackConf;
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

    printf("OspSip��ʼ���ɹ�!\n");

    /*
    ����һ��Ҫ��ʼ��ҵ����Ϣ����������ΪwEventId��strEvent�໥ת����Ҫ��ʼ����ſ���
    ��Ҳ��GetSipXmlMsgType�����Ļ���������InitEventDesc����Ҫ�ȵ���
    */
    InitEventDesc();
    /*
    �������ûص����������ã�û�յ�һ��sip��Ϣ�������xml��Ϣ����ô˺���������EventID
    */
    //SetXmlCB(&GetSipXmlMsgType);

    /*
    OspSip��ʼ���ɹ���Ϳ��Ե��ñ�������ʼע��Proxy�����������첽��
    ���Proxy�����ڣ�OspSip�ײ�᲻�ϳ���ע��
    ���Proxyע��ɹ����ϲ��dwDefaultOspIID App���յ�һ��OSP_PROXY_CONNECT������Ϣ����PowerUp
    ���Proxy�������ϲ��dwDefaultOspIID App���յ�һ��OSP_PROXY_DISCONNECT����Ҫ�ͷ�������Դ�����س�ʼ״̬
    �ȵ�Proxy�����ߺ��ֻ��յ�һ��OSP_PROXY_CONNECT
    */
    TKDSIP_DIALOG_ID tRegId = OspSipRegProxy(240);
    if (tRegId == INVALID_DIALOG_ID)
    {
        printf("OspSipRegProxy[%s] fail!!!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
        return 0;
    }

    printf("OspSipע��proxy[%s]�ɹ�!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());

    g_MainSemLock.Lock();
    g_MainSemLock.Lock();
	OspSipQuit();
    OspQuit();
    return 0;
}