/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : main.cpp
   相关文件    : 
   文件实现功能: server的main
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#include "serverinst.h"
#include <iostream>

CServerApp g_cServerApp;
CXSemLock g_MainSemLock;

API void bc(char *szMsg)
{
    CSubject *pcSubject = g_cServerApp.GetAppData().GetSubject();
    CObsMessage cMsg(szMsg);
    pcSubject->NotifyObserver(cMsg);
}

API void quit()
{
	g_MainSemLock.Give();
}

int main()
{
    //初始化OSP
    OspInit(TRUE, 2500, "server");
    g_cServerApp.CreateOspApp("Server", SERVER_AID, 80);

//     u32 adwIPList[20] = {0};
//     u16 wNum = 20;
//     OspAddrListGet(adwIPList, wNum);

    //初始化SIP
    TOspSipConf tOspSipConf;
    tOspSipConf.tLocalAddr.tIpAddr = LOCAL_IP;
    tOspSipConf.tLocalAddr.wPort = SERVER_SIP_LOCAL_PORT;
    tOspSipConf.tProxyAddr.tIpAddr = PROXY_IP;
    tOspSipConf.tProxyAddr.wPort = PROXY_SIP_PORT;
    tOspSipConf.tLocalURI.SetNOString(SERVER_URI);
    tOspSipConf.dwDefaultOspIID = MAKEIID(SERVER_AID, CInstance::DAEMON);

    TSipStackConf tSipStackConf;
    if(!OspSipInit(tOspSipConf, tSipStackConf))
    {
        printf("InitOspSip fail!!!\n");
        return 0;
    }

    printf("OspSip初始化成功!\n");
    //SetXmlCB(&GetSipXmlMsgType);
    InitEventDesc();

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