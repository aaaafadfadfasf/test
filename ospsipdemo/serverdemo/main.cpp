/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : main.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: server��main
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/10/10   1.0         liangli        ����
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
    //��ʼ��OSP
    OspInit(TRUE, 2500, "server");
    g_cServerApp.CreateOspApp("Server", SERVER_AID, 80);

//     u32 adwIPList[20] = {0};
//     u16 wNum = 20;
//     OspAddrListGet(adwIPList, wNum);

    //��ʼ��SIP
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

    printf("OspSip��ʼ���ɹ�!\n");
    //SetXmlCB(&GetSipXmlMsgType);
    InitEventDesc();

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