#include "cms/cppunitlite/cppunitlite.h"
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include <iostream>
using std::cout;
using std::endl;
#include "transutility.h"
#include "cuiapp.h"
#include "cuiproto.h"
#include "cuidata.h"
#include <queue>
using std::queue;


CTaskNOQueue g_cTaskNOQue;

void SendOspSipMsg(COspSipMsg &cMsg)
{
    OspPost(cMsg.GetOspIID(), cMsg.GetOspEventID(), cMsg.GetMsgBuf(), cMsg.GetMsgLen());
}

void PopTaskNO(u32 &dwTaskNO)
{
    while (1)
    {
        OspTaskDelay(1000);
        if ( ! g_cTaskNOQue.empty() )
        {
            dwTaskNO = g_cTaskNOQue.front();
            g_cTaskNOQue.pop();
            break;
        }
    }
}

void InitTest()
{
    //通知上层业务注册proxy成功
    COspSipMsg cMsg;
    cMsg.SetOspEventID(OSP_PROXY_CONNECT);
    cMsg.SetOspIID(CUIAPP_DAEMON);
    cMsg.SetOspTaskNO(INVALID_TASKNO);
    cMsg.SetOspSeqNO(INVALID_SEQ_NO);
    SendOspSipMsg(cMsg);

//     g_cCuiConfig.m_tLocalIP = "172.16.64.95";
//     g_cCuiConfig.m_tLocalURI.SetURIString("cui@js");
//     g_cCuiConfig.m_tCmuURI.SetURIString("cmu@js");
}


void testCuLogin()
{
    CCuiCuLoginReq cCuInfo;
    cCuInfo.SetUserName("a1@js");

    CCuiMsgCuLoginReq cReq;
    cReq.tMsgBody = cCuInfo;
    TRecvNode * ptRecvNode = new TRecvNode();
    ptRecvNode->nMsgTotalLen = cReq.ToBuffer(ptRecvNode->szRecvBuf, MAX_RECV_MSG_LEN);
    ptRecvNode->nRecvedLen = ptRecvNode->nMsgTotalLen;
    time_t tCur;
    time(&tCur);
    ptRecvNode->nDeadLine = tCur + 3000;

    CMsgHead cMsgHead;
    cMsgHead.FromBuffer(ptRecvNode->szRecvBuf, ptRecvNode->nMsgTotalLen);

    OspPost(CUIAPP_DAEMON, (u16)(cMsgHead.nMsgType), &ptRecvNode, sizeof(ptRecvNode));

    u32 dwTaskNO = INVALID_TASKNO;
    PopTaskNO(dwTaskNO);
    //期望CUI向3as@js注册，发送注册应答
    COspSipMsg c3ASRsp;
    c3ASRsp.SetOspEventID(TAS_LOGIN_TAS_RSP);
    c3ASRsp.SetOspIID(CUIAPP_DAEMON);
    c3ASRsp.SetOspTaskNO(dwTaskNO);
    CLoginTasRsp cLog3asRsp;
    cLog3asRsp.SetSession("3as@js");
    string strXml = cLog3asRsp.ToXml();
    c3ASRsp.SetMsgBody(strXml.c_str(), strXml.length());
    SendOspSipMsg(c3ASRsp);

    PopTaskNO(dwTaskNO);
    c3ASRsp.SetOspEventID(TAS_CU_ONLINE_RSP);
    c3ASRsp.SetOspTaskNO(dwTaskNO);
    CCuOnlineRsp cCuOnlineRsp;
    cCuOnlineRsp.SetSession("3as@js");
    cCuOnlineRsp.SetCuSessionId("a1@js");
    strXml = cCuOnlineRsp.ToXml();
    c3ASRsp.SetMsgBody(strXml.c_str(), strXml.length());
    SendOspSipMsg(c3ASRsp);

    PopTaskNO(dwTaskNO);
    c3ASRsp.SetOspEventID(TAS_CUSER_QRY_RSP);
    c3ASRsp.SetOspTaskNO(dwTaskNO);
    CUserQryRsp cUserQryRsp;
    cUserQryRsp.SetSession("3as@js");
    CUserInfo cUserInfo;
    cUserInfo.SetUserID("a1");
    cUserInfo.SetUserName("a1@js");
    cUserQryRsp.SetUserInfo(cUserInfo);
    strXml = cUserQryRsp.ToXml();
    c3ASRsp.SetMsgBody(strXml.c_str(), strXml.length());
    SendOspSipMsg(c3ASRsp);

    PopTaskNO(dwTaskNO);
    c3ASRsp.SetOspEventID(CU_LOGIN_RSP);
    c3ASRsp.SetOspTaskNO(dwTaskNO);
    CCuLoginRsp cCuLoginRsp;
    cCuLoginRsp.SetSession("a11@js");
    strXml = cCuLoginRsp.ToXml();
    c3ASRsp.SetMsgBody(strXml.c_str(), strXml.length());
    SendOspSipMsg(c3ASRsp);

    PopTaskNO(dwTaskNO);
}


