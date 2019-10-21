/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : main.cpp
   相关文件    : 
   文件实现功能: Client的main
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#include "clientinst.h"
#include <iostream>
#include <fstream>

#define CLIENT_DAEMON (MAKEIID(CLIENT_AID, CInstance::DAEMON))

API void reg(char *pszClientName)
{
    if ( NULL == pszClientName )
    {
        OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
        OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
        OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
        OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
        OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
		OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
		OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
		OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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
		OspPrintf(TRUE, FALSE, "无效的客户名称\n");
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

s8 g_szUserName[256] = {0}; //注册到Proxy的UA名称

API void quit()
{
	g_MainSemLock.Give();
}

int main()
{
    //printf("please input your UserName: ");
    //scanf("%s", g_szUserName);
    memcpy(g_szUserName,"Client",sizeof("Client"));

    //初始化OSP
    OspInit(TRUE, 2500, "client");
    /*
    创建一个OspAppExt，带有Task管理功能，推荐全局只有一个OspAppExt
    */
    g_cClientApp.CreateOspApp("Client", CLIENT_AID, 80, 5000);

//     u32 adwIPList[20] = {0};
//     u16 wNum = 20;
//     OspAddrListGet(adwIPList, wNum);

    //初始化SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr.tIpAddr = LOCAL_IP;
    tOspSipConf.tLocalAddr.wPort = CLIENT_SIP_LOCAL_PORT;
    tOspSipConf.tProxyAddr.tIpAddr = PROXY_IP;
    tOspSipConf.tProxyAddr.wPort = PROXY_SIP_PORT;
    tOspSipConf.tLocalURI.SetNOString(CLIENT_URI);
    tOspSipConf.tLocalURI.SetUser(g_szUserName);
    /*
    dwDefaultOspIID的作用在于，当OspSip收到一个新请求事务时，由于该事务没有绑定任何AppIID，因此就会发送给一个默认的App处理
    同时对于Proxy注册成功/Proxy断链/UA断链等都会发送给默认App处理
    */
    tOspSipConf.dwDefaultOspIID = MAKEIID(CLIENT_AID, CInstance::DAEMON);

    TSipStackConf tSipStackConf;
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

    printf("OspSip初始化成功!\n");

    /*
    这里一定要初始化业务消息的描述，因为wEventId和strEvent相互转换是要初始化后才可以
    这也是GetSipXmlMsgType分析的基础，所以InitEventDesc函数要先调用
    */
    InitEventDesc();
    /*
    这里设置回调函数的作用：没收到一条sip消息，剥离出xml消息体后用此函数分析出EventID
    */
    //SetXmlCB(&GetSipXmlMsgType);

    /*
    OspSip初始化成功后就可以调用本函数开始注册Proxy，本函数是异步的
    如果Proxy不存在，OspSip底层会不断尝试注册
    如果Proxy注册成功后，上层的dwDefaultOspIID App会收到一条OSP_PROXY_CONNECT，此消息类似PowerUp
    如果Proxy断链，上层的dwDefaultOspIID App会收到一条OSP_PROXY_DISCONNECT，需要释放所有资源并返回初始状态
    等到Proxy再上线后，又会收到一条OSP_PROXY_CONNECT
    */
    TKDSIP_DIALOG_ID tRegId = OspSipRegProxy(240);
    if (tRegId == INVALID_DIALOG_ID)
    {
        printf("OspSipRegProxy[%s] fail!!!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
        return 0;
    }

    printf("OspSip注册proxy[%s]成功!\n", tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());

    g_MainSemLock.Lock();
    g_MainSemLock.Lock();
	OspSipQuit();
    OspQuit();
    return 0;
}