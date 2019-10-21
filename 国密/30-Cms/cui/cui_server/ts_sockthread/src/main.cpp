#include "cms/cppunitlite/cppunitlite.h"
#include "cms/cppunitlite/testresultxml.h"
#include "cms/ospext/ospext.h"
#include "cuiproto.h"
#include "transthread.h"
#include "serviceappimp.h"

CXSemLock g_MainSemLock;
bool    g_bSockRecvThreadRun = true;
//#define _UNIT_TEST_

string GetCuiServerIpStr()
{
    string strIp("127.0.0.1");
    return strIp;
}

u16 GetCuiServerTcpListenPort()
{
    return CUI_TCP_DAT_RECV_PORT;
}
u32 GetCuiServerTimeout()
{
    return 10;
}

void PostMsgToSvrApp(u16 wEvent, const void* pvContent, u16 wLength)
{
    printf("recv PostMsgToSvrApp\n");
    PTRecvNode ptRecvNode = *(PTRecvNode*)(pvContent);

    CMsgHead cHead;
    s32 nRet = cHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);
    CMS_ASSERT(nRet > 0);
    char *pXmlMsgContent = ptRecvNode->szRecvBuf + nRet;
    string strEvent = GetSipXmlMsgType(pXmlMsgContent);
    CMS_ASSERT(strEvent.length() > 0);
    u16 wEvent1 = OspExtEventID( strEvent.c_str() );
    CMS_ASSERT(INVALID_OSP_EVENT != wEvent1);

    CEventRsp cRsp;
    cRsp.SetEvent(wEvent1+1);
    string strSeqNO = GetSipSeqNO(pXmlMsgContent);
    cRsp.SetSeqNum((const int)atol(strSeqNO.c_str()));
    cRsp.SetSession(GetSipSessionID(pXmlMsgContent));
    cRsp.SetErrorCode(ERR_CUI_NOT_IN_SERVICE);
    string strRspXml = cRsp.ToXml();
    SockSend(ptRecvNode->tSock, CUI_CU_WEBSERVICE_RSP, strRspXml.c_str(), strRspXml.length());
    FreeRecvNode(ptRecvNode);
}

API void login()
{
    CCuiCuLoginReq cLoginReq;
    cLoginReq.SetUserName("liangli");
    cLoginReq.SetPassword("12345");
    CCuiCuLoginRsp cLoginRsp;
    CuLogin(cLoginReq, cLoginRsp);
}

int main(int argc,char** argv)
{
    //初始化Osp
    //SockInit();
    OspInit(TRUE);

    {
        //在这里编写测试桩
        TRecvThreadInfo tInfo;
        tInfo.dwListenIpAddr = INADDR_ANY;
        tInfo.wListenPort = (unsigned short)CUI_TCP_DAT_RECV_PORT;
#ifdef _LINUX_
        TASKHANDLE hRecvThread = OspTaskCreate( EpollReceivingThread, "EpollRecvTask", 100, 1<<20, (u32)&tInfo, 0);
#else
        TASKHANDLE hRecvThread = OspTaskCreate( SelectReceivingThread, "SelectRecvTask", 100, 1<<20, (u32)&tInfo, 0);
#endif
        
    }
    //初始化所有业务消息
    InitCmsEventDesc();
    InitTacTasEventDesc();

    printf("begin\n");
    OspDelay(1000);
    login();
    g_MainSemLock.Lock();
    g_MainSemLock.Lock();

//     char achModName[256] = {0};
//     strncpy(achModName, argv[0], sizeof(achModName)-1);
// 
//     std::string szFilePath(achModName);
//     szFilePath += ".xml";
//     TestResultXml tr(szFilePath);

    //TestRegistry::runAllTests(tr);

    return 9;
    
    //return tr.wasSucessful() ? 0 : 1;
}

