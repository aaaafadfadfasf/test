
#include "cms/ospsip/ospsip.h"
#include "system/tinyxml/xml.h"
#include "kdvsys.h"
#include "ospsipconst.h"
#include "ospsipapp.h"
#include "ospsiphb.h"
#include "ospsipevtpoll.h"
#include "ospsipconvert.h"
#include "ospsipgbnat.h"


#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

TOspSipConf g_tOspSipConf;

TKDSIP_INIT_CONFIG g_tKdSipConfig;

PGetSipXmlMsgTypeCB g_pCBGetSipXmlMsgType = NULL;

void SetXmlCB(PGetSipXmlMsgTypeCB pCbXml)
{
    g_pCBGetSipXmlMsgType = pCbXml;
}

//KDSIP回调函数
void KDSIPEventCB(CKDSipEvent* pcEvent, void* pvContext)
{
    UNREFERENCED_PARAMETER(pvContext);

    if (pcEvent == NULL)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "kdsip回调事件指针为空\n");
        return;
    }
    
    //回调消息非常频繁，日志改为TIMER_LEV级别
    GLOBELOG(OSPSIP, TIMER_LEV, "Kdsip Callback Event[%s-%d], SipSN[%lu]\n", 
        SipEventDesc(pcEvent->GetEvent()).c_str(), pcEvent->GetEvent(), pcEvent->GetSN());

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_RECV;
    tCoreMsg.pvMsg = pcEvent;

    WriteCoreQue(tCoreMsg);
}

void OspSipSignalHandleCB(int nSig)
{
    u16 wSignalCB = OSP_SIP_EV_BEGIN;
    switch (nSig)
    {
    case CMS_SIGNAL_RELAOD_CFG:
        {
            //信号CMS_SIGNAL_RELAOD_CFG表示重新加载配置文件
            GLOBELOG(OSPSIP, CRITICAL_LEV, "receive signal[%d], reload config file\n", nSig);
            wSignalCB = OSP_SIP_SIGNAL_RELOAD_CFG;
        }
        break;

    default:
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "receive unknown signal[%d], omit it\n", nSig);
        }
        break;
    }

    if (wSignalCB != OSP_SIP_EV_BEGIN)
    {
        COspSipMsg cMsg;
        TOspTransID tOspTrans;
        tOspTrans.dwIID = g_tOspSipConf.dwDefaultOspIID;

        cMsg.SetOspEventID(wSignalCB);
        cMsg.SetOspTransID(tOspTrans);
        SendMsgToOspApp(cMsg);
    }   
}


//给kdsip库实现打印回调函数，使kdsip支持ospprintf输出
u8 g_byIsKdSipOnTelnet = 0;

void KdSipPrintf(LPCSTR szLog)
{
    if (g_byIsKdSipOnTelnet == 0)
    {
        return;
    }

    if (szLog == NULL)
    {
        return;
    }

    //osp内部的最大日志长度是6000
    #define OSP_MAX_LOG_MSG_LEN          4096   //#define MAX_LOG_MSG_LEN          6000         //每条日志的最大长度
    size_t nLogLen = strlen(szLog);

    //分批打印kdsip的长日志
    size_t nPrintCount = nLogLen/OSP_MAX_LOG_MSG_LEN;
    size_t nMod = nLogLen%OSP_MAX_LOG_MSG_LEN;
    if (nMod != 0)
    {
        nPrintCount++;
    }

    for (size_t i=0; i<nPrintCount; i++)
    {
        s8 szLogUnit[OSP_MAX_LOG_MSG_LEN+1] = {0};
        if (i == nPrintCount -1)
        {
            strcpy(szLogUnit, szLog+i*OSP_MAX_LOG_MSG_LEN);
        }
        else
        {
            strncpy(szLogUnit, szLog+i*OSP_MAX_LOG_MSG_LEN, OSP_MAX_LOG_MSG_LEN);
        }

        OspPrintf(TRUE, FALSE, "%s", szLogUnit);
    }   
}

/*====================================================================
函数名      : InitOspSip
功能        : OspSip初始化
算法实现    : 
引用全局变量:             
输入参数说明: 
              tOspSipConf: OspSip相关的配置信息
              tSipStackConf: Sip协议栈相关的配置
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    : 
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipInit(const TOspSipConf& tOspSipConf, const TSipStackConf& tSipStackConf)
{
    g_tOspSipConf = tOspSipConf;
    g_tOspSipConf.nUaRouteMode = tSipStackConf.nSipRouteMode;
    g_tKdSipConfig = tSipStackConf.tKdSipConfig;
    TKDSIP_INIT_CONFIG* pKdSipConf = const_cast<TKDSIP_INIT_CONFIG*>(&tSipStackConf.tKdSipConfig);

    pKdSipConf->pfEventCB = &KDSIPEventCB;
    pKdSipConf->pfLogCB = &KdSipPrintf;
    pKdSipConf->dwLogFileMaxFileNum = 5;   //kdsip每个日志文件大小为2M，5个文件循环覆盖，一共占用10M左右
    pKdSipConf->nMsgSizeMax = tSipStackConf.tKdSipConfig.nMsgSizeMax;
    pKdSipConf->nRetransmitTimer = 4*1000; //重传间隔设置为4s, kdsip已经修改，确保重传次数不超过3次

    g_cOspSipConvertor.SetDefaultOspIID(g_tOspSipConf.dwDefaultOspIID);
    SetXmlCB(&GetSipXmlMsgType);

    //设置信号回调
    OspExtSetSignalProcCB(&OspSipSignalHandleCB);

#ifdef DEBUG
    OSP_SET_MOD_LOG(OSPSIP, EVENT_LEV);
#else
    OSP_SET_MOD_LOG(OSPSIP, CRITICAL_LEV);
#endif
    
    if (KDSIP_OK != KDSipInit(NULL/*const_cast<LPSTR>(tOspSipConf.tLocalAddr.tIpAddr.c_str())*/,
        tOspSipConf.tLocalAddr.wPort, pKdSipConf))
    {
        printf("Kdsiplib init fail!!!\n");
        GLOBELOG(OSPSIP, ERROR_LEV, "Kdsiplib init fail!!!\n");
        return false;
    }

    printf("Kdsiplib init success!!!\n");
    GLOBELOG(OSPSIP, CRITICAL_LEV, "Kdsiplib init success!!!\n");
    
#ifdef DEBUG
    KDSipSetLogLev(KDSIP_LOG_DEBUG);
#else
    KDSipSetLogLev(KDSIP_LOG_ERROR);
#endif

    KDSipSetLogOnScreen(FALSE);

    TASKHANDLE hSipEvtThread = NULL;
    hSipEvtThread = OspTaskCreate( SipEventPollThread, "SipEventPollThread", 100, 1<<20, 0, 0);
    if( hSipEvtThread == NULL )
    {
        printf("create SipEventPollThread fail!!!\n");
        GLOBELOG(OSPSIP, ERROR_LEV, "create SipEventPollThread fail!!!\n");
        return false;
    }

    printf("create SipEventPollThread success!!!\n");
    GLOBELOG(OSPSIP, CRITICAL_LEV, "create SipEventPollThread success!!!\n");

    InitOspSipMsgDesc();

    g_cOspSipApp.CreateOspApp("ospsipapp", OSP_SIP_APP_ID, 80,8000);

#ifdef _LINUX_
    RegOspSipCmd();
#endif

    return true;
}

/*====================================================================
函数名      : QuitOspSip
功能        : OspSip
算法实现    : 
引用全局变量: 
输入参数说明: 
返回值说明  :
----------------------------------------------------------------------
修改记录    : 
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
void OspSipQuit()
{
    g_bSipPollRun = false;
    KDSipQuit();
}

/*====================================================================
函数名      : OspSipPostReq
功能        : 发送SIP对话外请求消息，并绑定OSP事务和SIP事务的映射关系
算法实现    :
引用全局变量:
输入参数说明:
              eSipEvent: sip消息ID
              strMsgBuf: sip消息体
              tDstURI: 消息目的URI
              tSrcOspTrans: osp事务ID
返回值说明  : 成功:true，失败:false
备注        : 可发送SIP事件包括:

              KDSIP_EVENT_REGISTER_REQ,
              KDSIP_EVENT_UNREGISTER_REQ,
              KDSIP_EVENT_INVITE_REQ,
              KDSIP_EVENT_OPTIONS_HB_REQ,
              KDSIP_EVENT_MESSAGE_REQ,
              KDSIP_EVENT_SUBSCRIBE_REQ,
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo)
{
    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_TYPE_REQ;
    ptSipSendMsg->eSipEvent = eSipEvent;
    ptSipSendMsg->strMsgBuf = strMsgBuf;
    ptSipSendMsg->tDstURI = tDstURI;
    ptSipSendMsg->tSrcOspTrans = tSrcOspTrans;

    if (szDirectUAAddr != NULL)
    {
        ptSipSendMsg->strDirectUAAddr = szDirectUAAddr;
    }

    if (ptSipExtraInfo != NULL)
    {
        ptSipSendMsg->ptSipExtraInfo = new TSipExtraInfo(*ptSipExtraInfo);
    }

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;
    OspSipConvert2ExtraNetAddr(ptSipSendMsg, true, true, true);

    return WriteCoreQue(tCoreMsg);
}

/*====================================================================
函数名      : OspSipPostInDlgReq
功能        : 发送SIP对话内请求消息，并绑定OSP事务和SIP事务的映射关系
算法实现    :
引用全局变量:
输入参数说明:
              eSipEvent: sip消息ID
              strMsgBuf: sip消息体
              tDstDlgId: sip对话ID
              tSrcOspTrans: osp事务ID
返回值说明  : 成功:true，失败:false
备注        : 可发送SIP事件包括:

              KDSIP_EVENT_INVITE_ACK,
              KDSIP_EVENT_INVITE_BYE_REQ,
              KDSIP_EVENT_INFO_REQ,
              KDSIP_EVENT_MESSAGE_DIALOG_REQ,
              KDSIP_EVENT_SUBSCRIBE_REQ,
              KDSIP_EVENT_UNSUBSCRIBE_REQ,
              KDSIP_EVENT_NOTIFY_REQ,
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, 
    const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo)
{
    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_TYPE_REQ_INDLG;
    ptSipSendMsg->eSipEvent = eSipEvent;
    ptSipSendMsg->strMsgBuf = strMsgBuf;
    ptSipSendMsg->tDstDlgId = tDstDlgId;
    ptSipSendMsg->tSrcOspTrans = tSrcOspTrans;

    if (ptSipExtraInfo != NULL)
    {
        ptSipSendMsg->ptSipExtraInfo = new TSipExtraInfo(*ptSipExtraInfo);
    }

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;
    OspSipConvert2ExtraNetAddr(ptSipSendMsg, false, true, false);

    return WriteCoreQue(tCoreMsg);
}

/*====================================================================
函数名      : OspSipPostRsp
功能        : 发送SIP应答
算法实现    :
引用全局变量:
输入参数说明:
              eSipEvent: sip消息ID
              tTransId: SIP事务ID
              strMsgBuf: sip消息体
              tSrcOspTrans: 本地osp事务ID
              eRspCode: SIP状态码
返回值说明  : 成功:true，失败:false
备注        : 可发送SIP事件包括:

              KDSIP_EVENT_REGISTER_RSP,
              KDSIP_EVENT_UNREGISTER_RSP,
              KDSIP_EVENT_INVITE_RSP,
              KDSIP_EVENT_INVITE_BYE_RSP,
              KDSIP_EVENT_INFO_RSP,
              KDSIP_EVENT_OPTIONS_HB_RSP, 
              KDSIP_EVENT_MESSAGE_RSP,
              KDSIP_EVENT_MESSAGE_DIALOG_RSP,
              KDSIP_EVENT_SUBSCRIBE_RSP,
              KDSIP_EVENT_UNSUBSCRIBE_RSP,
              KDSIP_EVENT_NOTIFY_RSP,
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const string& strMsgBuf, 
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode, const TSipExtraInfo* ptSipExtraInfo)
{    
    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_TYPE_RSP;
    ptSipSendMsg->eSipEvent = eSipEvent;
    ptSipSendMsg->strMsgBuf = strMsgBuf;
    ptSipSendMsg->tDstTransId = tTransId;
    ptSipSendMsg->tSrcOspTrans = tSrcOspTrans;
    ptSipSendMsg->eRspCode = eRspCode;

    if (ptSipExtraInfo != NULL)
    {
        ptSipSendMsg->ptSipExtraInfo = new TSipExtraInfo(*ptSipExtraInfo);
    }

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;
    OspSipConvert2ExtraNetAddr(ptSipSendMsg, false, true, true);

    return WriteCoreQue(tCoreMsg);
}

/*====================================================================
函数名      : OspSipReleaseDlg
功能        : 释放对话资源，不会发送SIP消息
              用于处理断链的Dialog的资源回收
算法实现    :
引用全局变量:
输入参数说明:
              tDlgId: 对话ID
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
void OspSipReleaseDlg(TKDSIP_DIALOG_ID tDlgId)
{
    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_TYPE_RELEASE_DLG;
    ptSipSendMsg->tDstDlgId = tDlgId;

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;

    WriteCoreQue(tCoreMsg);
}


/*====================================================================
函数名      : OspSipRegProxy
功能        : 注册到SIP PROXY
算法实现     : 原先采用同步模式，后来采用异步模式，只要发送注册请求成功就认为成功
引用全局变量:             
输入参数说明: 
             dwExpire: The expires value for registration(单位：秒)
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    : 
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipRegProxy(s32 nExpires)
{
    //判断是否注册成功过，或者正在注册。

    if(OSP_OK != OspPost(OSP_SIP_DAEMON, SIP_REG_PROXY_REQ, &nExpires, sizeof(nExpires)))
    {
        GLOBELOG(OSPSIP, WARNING_LEV, "OspPost() 调用失败\n");
        return false;
    }

    return true;
}

/*====================================================================
函数名      : OspSipRegProxy
功能        : 注册到SIP PROXY
算法实现     : 目前该接口只在ospsip内部使用
引用全局变量:             
输入参数说明: 
             dwExpire: The expires value for registration
             tOspTransId: Osp事务ID
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    : 
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
// TKDSIP_DIALOG_ID OspSipRegProxy(s32 nExpires, const TOspTransID& tOspTransId)
// {
//     char szFrom[MAX_SIP_URI_LEN+1] = {0};
//     sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetURIString().c_str());
// 
//     char szRegistrar[MAX_SIP_URI_LEN+1] = {0};
//     sprintf(szRegistrar, "sip:%s:%d", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), 
//         g_tOspSipConf.tProxyAddr.wPort);
// 
//     char szContact[MAX_SIP_URI_LEN+1] = {0};
//     sprintf(szContact, "sip:%s:%d", g_tOspSipConf.tLocalAddr.tIpAddr.c_str(), 
//         g_tOspSipConf.tLocalAddr.wPort);
// 
//     CKDSipMsgHead tSipHead;
//     tSipHead.SetFromURI(szFrom);
//     tSipHead.SetToURI(szFrom);
//     tSipHead.SetProxyURI(szRegistrar);
//     tSipHead.SetContactURI(szContact);
// 
//     if (g_tRegProxyID != INVALID_DIALOG_ID)
//     {
//         OspSipReleaseDlg(g_tRegProxyID);
//     }
// 
//     TKDSIP_DIALOG_ID tSipRegId = KDSipRegisterReq(tSipHead, nExpires);
//     if (tSipRegId == INVALID_DIALOG_ID)
//     {
//         GLOBELOG(OSPSIP, ERROR_LEV, "KDSipRegisterReq is fail!\n");
// 
//         GLOBELOG(OSPSIP, ERROR_LEV, "Register param[from:%s]-[to:%s]!\n",
//             tSipHead.GetFromURI(), tSipHead.GetToURI());
//         GLOBELOG(OSPSIP, ERROR_LEV, "Register param[proxy:%s]-[contact:%s]-[Expires:%d]!\n",
//             tSipHead.GetProxyURI(), tSipHead.GetContactURI(), nExpires);
// 
//         return tSipRegId;
//     }
// 
//     TOspSipBind tOspSipBind;
//     tOspSipBind.tOspId = tOspTransId;
//     tOspSipBind.tSipId.tSipDlg = tSipRegId;
//     tOspSipBind.eSipEvent = KDSIP_EVENT_REGISTER_REQ;
// 
//     g_cOspSipConvertor.InsertDlg(tOspSipBind, tSipRegId);
// 
//     g_tRegProxyID = tSipRegId;
// 
//     return tSipRegId;
// }

/*====================================================================
函数名      : OspSipSetHBParam
功能        : 设置OSPSIP心跳参数
算法实现    : 心跳失败，向上层发送 SIP_DISCONNECT 消息
引用全局变量:             
输入参数说明: 
             tHbDst: 心跳目的
             tHbSrc: 心跳发起者
             wHbTime: 心跳检测时间(以秒为单位),
             wHbCnt: 心跳检测次数
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    : 
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipSetHBParam(const TSipURI& tHbDst, const TOspTransID& tHbSrc, LPCSTR szDirectUAAddr, u16 wHbTime, u16 wHbCnt)
{
    if(!tHbDst.IsValidURI())
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "无效的心跳URI\n");
        return false;
    }

    if (tHbSrc.dwIID == INVALID_IID)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "无效的OSPIID\n");
        return false;
    }

    if (g_cHbNodeList.GetSize() >= SIP_HB_NODE_MAX)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "心跳结点数超过最大值[%d]\n", SIP_HB_NODE_MAX);
        return false;
    }

    //todo 检测是否已经设置过心跳了，如果是则更新node
    //todo 为什么不在OspSipApp中添加node？避免同步锁
    
    u32 dwDstIID = OSP_SIP_PENDING;

    TOspSipHbNode* pHbNode = g_cHbNodeList.FindHbNode(tHbDst);
    if (pHbNode == NULL)
    {
        //添加
        TOspSipHbNode tHbNode;
        tHbNode.tHbDst = tHbDst;
        tHbNode.tHbSrc = tHbSrc;
        tHbNode.wHbTime = wHbTime;
        tHbNode.wHbCnt = wHbCnt;
        if (szDirectUAAddr != NULL)
        {
            tHbNode.strDirectUAAddr = szDirectUAAddr;
        }
        
        if (!g_cHbNodeList.InsertHbNode(tHbNode))
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "dst[%s]插入心跳列表失败\n",
                tHbNode.tHbDst.GetURIString().c_str());
            return false;
        }
    }
    else
    {
        //更新
        pHbNode->tHbSrc = tHbSrc;
        pHbNode->wHbTime = wHbTime;
        pHbNode->wHbCnt = wHbCnt;
        if (szDirectUAAddr != NULL)
        {
            pHbNode->strDirectUAAddr = szDirectUAAddr;
        }

        if (pHbNode->wHbWorkInst != CInstance::INVALID)
        {
            dwDstIID = MAKEIID(OSP_SIP_APP_ID, pHbNode->wHbWorkInst);
        }
    }

    string strURI = tHbDst.GetURIString();

    return (OSP_OK == OspPost(dwDstIID, SIP_HB_ADD_NODE, strURI.c_str(), u16(strURI.length()+1)));
}

/*====================================================================
函数名      : OspSipStopHB
功能        : 停止OSPSIP心跳
算法实现    : 停止心跳，不会向上层发送 OSP_SIP_DISCONNECT 消息
引用全局变量:
输入参数说明:
             tHbDst: 心跳目的
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/12/27  0.1         fanxg
====================================================================*/
bool OspSipStopHB(const TSipURI& tHbDst)
{
    TOspSipHbNode* pHbNode = g_cHbNodeList.FindHbNode(tHbDst);
    if (pHbNode == NULL)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "目的心跳不存在[%s]\n", tHbDst.GetURIString().c_str());
        return false;
    }

    if (pHbNode->wHbWorkInst == CInstance::INVALID)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "无效心跳结点[%s]\n", tHbDst.GetURIString().c_str());
        return false;
    }

    u32 dwDstIID = MAKEIID(OSP_SIP_APP_ID, pHbNode->wHbWorkInst);
    string strURI = tHbDst.GetURIString();

    return (OSP_OK == OspPost(dwDstIID, SIP_HB_DEL_NODE, strURI.c_str(), u16(strURI.length()+1)));
}


/*====================================================================
函数名      : OspSipGetLocalURI
功能        : 返回UA向Proxy注册时填写的LocalURI

              因为SIP协议栈采用UDP协议，服务端需要知道客户端的URI才能向客户端发请求
              客户端向服务端注册时需要携带自身URI信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 返回UA向Proxy注册时填写的LocalURI
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
const TSipURI OspSipGetLocalURI()
{
    return g_tOspSipConf.tLocalURI;
}

/*====================================================================
函数名      : OspSipAuthenticate
功能        : 用于服务器SIP注册的鉴权
算法实现    :
引用全局变量:
输入参数说明:
              strAuthInfo: 完整的鉴权字符串
              strPassword: 服务器分配给某个用户的密码(明文)
              eSipEvent: 需要鉴权的SIP方法
返回值说明  : true:鉴权成功; false:鉴权失败
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/11/27  0.1         fanxg
====================================================================*/
bool OspSipAuthenticate(const string& strAuthInfo, const string& strPassword, EKDSIP_EVENT eSipEvent)
{
    CKDSipAUTHTOOL cAuthTool(strAuthInfo.c_str(), eSipEvent);
    LPCSTR szUserName = cAuthTool.GetUsername();

    if (szUserName == NULL)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "用户名为空，[%s-%d]鉴权失败\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }
    else
    {
        GLOBELOG(OSPSIP, CRITICAL_LEV, "[%s-%d]携带鉴权信息[%s],userName[%s]-password[%s]，开始鉴权\n",
            SipEventDesc(eSipEvent).c_str(), eSipEvent, strAuthInfo.c_str(), szUserName, strPassword.c_str());

        if (TRUE == (cAuthTool.CheckPassword(strPassword.c_str())))
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "userName[%s]鉴权成功\n", szUserName);
            return true;
        }
        else
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "userName[%s]鉴权失败\n", szUserName);
           return false;
        }
    }   
}

/*====================================================================
函数名      : OspSipGetUserNameFromAuthInfo
功能        : 从鉴权字符串中获取用户名
算法实现    :
引用全局变量:
输入参数说明:
              strAuthInfo: 完整的鉴权字符串
              eSipEvent: 需要鉴权的SIP方法
返回值说明  : 用户名，空表示获取失败
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/11/27  0.1         fanxg
====================================================================*/
string OspSipGetUserNameFromAuthInfo(const string& strAuthInfo, EKDSIP_EVENT eSipEvent)
{
    string strUserName;

    CKDSipAUTHTOOL cAuthTool(strAuthInfo.c_str(), eSipEvent);
    LPCSTR szUserName = cAuthTool.GetUsername();
    if (szUserName != NULL)
    {
        strUserName = szUserName;
    }
    else
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "AuthInfo[%s]中用户名为空\n", strAuthInfo.c_str());
    }

    return strUserName;
}

//ospsip调试打印命令

//ospsip帮助
API void ospsiphelp()
{
    OspPrintf(TRUE, FALSE, "%-20s: ospsip帮助提示\n","  ospsiphelp()");
    OspPrintf(TRUE, FALSE, "%-20s: ospsip版本信息\n","  ospsipver()");
    OspPrintf(TRUE, FALSE, "%-20s: ospsip配置信息\n","  ospsipcfg()");
    OspPrintf(TRUE, FALSE, "%-20s: 设置ospsip打印级别\n","  ospsiplog(u8 byLev)");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip心跳列表\n","  ospsiphblist()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip SN信息\n","  ospsipsn()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip事务信息\n","  ospsiptrans()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip对话信息\n","  ospsipdlg()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip发送队列\n","  ospsipque()");
    OspPrintf(TRUE, FALSE, "%-20s: 显示ospsip路由表信息：参数可指定UA\n","  ospsiproute(LPCSTR szUaName)");
    OspPrintf(TRUE, FALSE, "%-20s: 设置kdsip打印级别:byLev:级别；byIsOnTelnet:telnet输出\n","  kdsiplog(u8 byLev, u8 byIsOnTelnet)");
    OspPrintf(TRUE, FALSE, "%-20s: 打印kdsip库基本信息\n","  kdsipshow()");
}

#ifdef _LINUX_
void RegOspSipCmd()
{
    OspRegCommandEx("ospsiphelp", (void *)ospsiphelp, "", "");
    OspRegCommandEx("ospsipver", (void *)ospsipver, "", "");
    OspRegCommandEx("ospsipcfg", (void *)ospsipcfg, "", "");
    OspRegCommandEx("ospsiplog", (void *)ospsiplog, "", "i");    
    OspRegCommandEx("ospsiphblist", (void *)ospsiphblist, "", "");
    OspRegCommandEx("ospsipsn", (void *)ospsipsn, "", "");
    OspRegCommandEx("ospsiptrans", (void *)ospsiptrans, "", "");
    OspRegCommandEx("ospsipin", (void *)ospsipin, "", "");
    OspRegCommandEx("ospsipout", (void *)ospsipout, "", "");
    OspRegCommandEx("ospsipdlg", (void *)ospsipdlg, "", "");    
    OspRegCommandEx("ospsipivt", (void *)ospsipivt, "", "");
    OspRegCommandEx("ospsipreg", (void *)ospsipreg, "", "");
    OspRegCommandEx("ospsipss", (void *)ospsipss, "", "");
    OspRegCommandEx("ospsipque", (void *)ospsipque, "", "");
    OspRegCommandEx("ospsiproute", (void *)ospsiproute, "", "s");
    OspRegCommandEx("kdsiplog", (void *)kdsiplog, "", "ii");
    OspRegCommandEx("kdsipshow", (void *)kdsipshow, "", "");
}
#endif


#define OSP_SIP_VER "ospsip 1.0 Compile Time: %s, %s\r\n", __DATE__, __TIME__
//ospsip版本
API void ospsipver()
{
    OspPrintf(TRUE, FALSE, OSP_SIP_VER);
}

extern void OspSipGbNatDump();
API void ospsipcfg()
{
    OspPrintf(TRUE, FALSE, "RouteMode=%d, MaxTrans=%d, MaxReg=%d, MaxInvite=%d, MaxSs=%d, MaxNtfPerSs=%d, DisableCheckSum=%d\n", 
        g_tOspSipConf.nUaRouteMode,
        g_tKdSipConfig.nMaxTrans, g_tKdSipConfig.nMaxRegisterDialog,
        g_tKdSipConfig.nMaxInviteDialog, g_tKdSipConfig.nMaxSubscribeDialog, g_tKdSipConfig.nNumOfNotifysPerSub,
        g_tKdSipConfig.bUDPChecksumDisable);

    OspPrintf(TRUE, FALSE, "\n");
    OspSipGbNatDump();
}

//设置ospsip打印级别
API void ospsiplog(u8 byLogLev)
{
    OspSetLogLev(byLogLev, OSP_SIP_APP_ID);
    OspSetModLogLev(OSPSIP, byLogLev);
}

//显示ospsip心跳列表
API void ospsiphblist()
{
    g_cHbNodeList.PrintHbList();
}

//显示SN信息
API void ospsipsn()
{
    g_cOspSipConvertor.PrintSN();
}

//显示事务信息
API void ospsiptrans()
{
    g_cOspSipConvertor.PrintTrans();
}

//显示income事务信息
API void ospsipin()
{
    g_cOspSipConvertor.PrintTrans(PRINT_IN_TRANS);
}

//显示out事务信息
API void ospsipout()
{
    g_cOspSipConvertor.PrintTrans(PRINT_OUT_TRANS);
}

//显示对话信息
API void ospsipdlg()
{
    g_cOspSipConvertor.PrintDialog();
}

//显示invite对话信息
API void ospsipivt()
{
    g_cOspSipConvertor.PrintDialog(PRINT_IVT_DLG);
}

//显示subscribe对话信息
API void ospsipss()
{
    g_cOspSipConvertor.PrintDialog(PRINT_SS_DLG);
}

//显示register对话信息
API void ospsipreg()
{
    g_cOspSipConvertor.PrintDialog(PRINT_REG_DLG);
}

//显示发送队列信息
API void ospsipque()
{
    g_cOspSipCoreMsgQue.Print();
}

//显示UA的路由信息
API void ospsiproute(const char* szUaName)
{
    g_cOspSipConvertor.PrintRoute(szUaName);
}

//设置kdsip打印级别
API void kdsiplog(u8 byLogLev, u8 byIsKdSipOnTelnet)
{
    KDSipSetLogLev(EKDSIP_LOG_EVENT(byLogLev));
    g_byIsKdSipOnTelnet = byIsKdSipOnTelnet;
}

//打印kdsip库基本信息
API void kdsipshow()
{
    u8 byIsKdSipOnTelnet = g_byIsKdSipOnTelnet;
    if (byIsKdSipOnTelnet == 0)
    {
        g_byIsKdSipOnTelnet = 1;
    }

    KDSIPShow();
    g_byIsKdSipOnTelnet = byIsKdSipOnTelnet;
}

//获取kdsip的资源信息
bool OspSipGetKdsipResInfo(OUT TKDSIP_DIALOG_RESOURCE& tInfo, IN EKDSIP_DIALOG_TYPE& eType)
{
	tInfo.dwUsage = 0;
	tInfo.dwMax   = 0;

	//获取kdsip资源
	if(KDSIP_OK == KDSipGetDialogResource(eType, &tInfo))
	{
		return true;
	}

	return false;
}

COspSipMsg::COspSipMsg()
{
    memset(m_abyBuffer, 0, OSP_SIP_MSG_MAX_LEN);

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->Clear();
}

COspSipMsg& COspSipMsg::operator= (const COspSipMsg& cMsg)
{
    if(&cMsg == this)
    {
        return *this;
    }
    memcpy(m_abyBuffer, cMsg.m_abyBuffer, cMsg.GetMsgLen());
    return (*this);
}

COspSipMsg::COspSipMsg(const COspSipMsg& cMsg)
{
    this->operator=(cMsg);
}

COspSipMsg::COspSipMsg(u8 * const pbyMsg, u16 wMsgLen)
{
    memset(m_abyBuffer, 0, OSP_SIP_MSG_MAX_LEN);

    if( wMsgLen < OSP_SIP_MSG_BODY_OFFSET || pbyMsg == NULL )
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "无效数据:wMsgLen = %hu, pbyMsg = %p\n",
            wMsgLen, pbyMsg);

        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    if (wMsgLen >= sizeof( m_abyBuffer ))
    {
        wMsgLen = sizeof( m_abyBuffer );
    }
    memcpy( m_abyBuffer, pbyMsg, wMsgLen );
    ptMsgHead->m_wMsgBodyLen = (wMsgLen - OSP_SIP_MSG_BODY_OFFSET);
}

void COspSipMsg::SetOspIID(u32 dwOspIID)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_dwOspIID = dwOspIID;
}

u32 COspSipMsg::GetOspIID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_dwOspIID;
}

u32 COspSipMsg::GetOspTaskNO() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_dwOspTaskNO;
}

void COspSipMsg::SetOspTaskNO(u32 dwTaskNO)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_dwOspTaskNO = dwTaskNO;
}

u32 COspSipMsg::GetOspSeqNO() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_dwOspSeqNO;
}

void COspSipMsg::SetOspSeqNO(u32 dwSeqNO)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_dwOspSeqNO = dwSeqNO;
}

TOspTransID COspSipMsg::GetOspTransID() const
{
    TOspTransID tOspTransID;
    tOspTransID.dwIID = GetOspIID();
    tOspTransID.dwTaskID = GetOspTaskNO();
    tOspTransID.dwSeqNO = GetOspSeqNO();
    return tOspTransID;
}

void COspSipMsg::SetOspTransID(const TOspTransID& tOspTransID)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_dwOspIID = tOspTransID.dwIID;
    ptMsgHead->m_dwOspTaskNO = tOspTransID.dwTaskID;
    ptMsgHead->m_dwOspSeqNO = tOspTransID.dwSeqNO;
}

EKDSIP_EVENT COspSipMsg::GetSipEventID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_eSipEvent;
}


const string COspSipMsg::GetSipEventStr() const
{
    return SipEventDesc(GetSipEventID());
}

void COspSipMsg::SetSipEventID(EKDSIP_EVENT eEventID)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_eSipEvent = eEventID;
}


EKDSIP_EVENT COspSipMsg::GetSipReq() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();

    if(IsSipReqMsg(ptMsgHead->m_eSipEvent))
    {
        return ptMsgHead->m_eSipEvent;
    }
    else if(IsSipRspMsg(ptMsgHead->m_eSipEvent))
    {
        return EKDSIP_EVENT(ptMsgHead->m_eSipEvent - 1);
    }
    else
    {
        return KDSIP_EVENT_UNKNOWN;
    }
}

EKDSIP_EVENT COspSipMsg::GetSipRsp() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();

    if(IsSipReqMsg(ptMsgHead->m_eSipEvent))
    {
        return EKDSIP_EVENT(ptMsgHead->m_eSipEvent + 1);
    }
    else
    {
        return KDSIP_EVENT_UNKNOWN;
    }
}

void COspSipMsg::SetSipDlgID(TKDSIP_DIALOG_ID tSipId)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_tSipDlgId = tSipId;
}

TKDSIP_DIALOG_ID COspSipMsg::GetSipDlgID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_tSipDlgId;
}

void COspSipMsg::SetSipTransID(TKDSIP_TRANS_ID tSipTransId)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_tSipTransId = tSipTransId;
}


TKDSIP_TRANS_ID COspSipMsg::GetSipTransID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_tSipTransId;
}

EKDSIP_RESPONSE_CODE COspSipMsg::GetSipErrorCode() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_eSipErrorCode;
}

void COspSipMsg::SetSipErrorCode(EKDSIP_RESPONSE_CODE eErrorCode)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_eSipErrorCode = eErrorCode;
}

EKDSIP_CONTENT_TYPE COspSipMsg::GetSipContentType() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_eSipContentType;
}
void COspSipMsg::SetSipContentType(EKDSIP_CONTENT_TYPE eContentType)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_eSipContentType = eContentType;
}

TSipURI COspSipMsg::GetSipRequestUri() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    TSipURI tSipUri;
    tSipUri.SetNOString(ptMsgHead->m_szRequestUri);
    return tSipUri;
}

void COspSipMsg::SetSipRequestUri(LPCSTR szSipUri)
{
    if (szSipUri == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri == null\n");
        return;
    }

    if (strlen(szSipUri) > MAX_SIP_URI_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri长度大于[%d]\n", MAX_SIP_URI_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szRequestUri, szSipUri);

}

TSipURI COspSipMsg::GetSipFromUri() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    TSipURI tSipUri;
    tSipUri.SetNOString(ptMsgHead->m_szFromUri);
    return tSipUri;
}

void COspSipMsg::SetSipFromUri(LPCSTR szSipUri)
{
    if (szSipUri == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri == null\n");
        return;
    }

    if (strlen(szSipUri) > MAX_SIP_URI_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri长度大于[%d]\n", MAX_SIP_URI_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szFromUri, szSipUri);

}

TSipURI COspSipMsg::GetSipToUri() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    TSipURI tSipUri;
    tSipUri.SetNOString(ptMsgHead->m_szToUri);
    return tSipUri;
}

void COspSipMsg::SetSipToUri(LPCSTR szSipUri)
{
    if (szSipUri == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri == null\n");
        return;
    }

    if (strlen(szSipUri) > MAX_SIP_URI_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri长度大于[%d]\n", MAX_SIP_URI_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szToUri, szSipUri);
}

TSipURI COspSipMsg::GetSipContactUri() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    TSipURI tSipUri;
    tSipUri.SetNOString(ptMsgHead->m_szContactUri);
    return tSipUri;
}
void COspSipMsg::SetSipContactUri(LPCSTR szSipUri)
{
    if (szSipUri == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri == null\n");
        return;
    }

    if (strlen(szSipUri) > MAX_SIP_URI_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipUri长度大于[%d]\n", MAX_SIP_URI_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szContactUri, szSipUri);
}

string COspSipMsg::GetSipAuthInfo() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipAuthInfo);
}
void COspSipMsg::SetSipAuthInfo(LPCSTR szAuthInfo)
{
    if (szAuthInfo == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szAuthInfo == null\n");
        return;
    }

    if (strlen(szAuthInfo) > MAX_SIP_AUTH_INFO_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szAuthInfo长度大于[%d]\n", MAX_SIP_AUTH_INFO_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipAuthInfo, szAuthInfo);
}

string COspSipMsg::GetSipRealm() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipRealm);
}
void COspSipMsg::SetSipRealm(LPCSTR szSipRealm)
{
    if (szSipRealm == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipRealm == null\n");
        return;
    }

    if (strlen(szSipRealm) > MAX_SIP_REALM_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipRealm长度大于[%d]\n", MAX_SIP_REALM_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipRealm, szSipRealm);
}

string COspSipMsg::GetSipSubject() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipSubject);
}
void COspSipMsg::SetSipSubject(LPCSTR szSipSubject)
{
    if (szSipSubject == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipSubject == null\n");
        return;
    }

    if (strlen(szSipSubject) > MAX_SIP_SUBJECT_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipSubject长度大于[%d]\n", MAX_SIP_SUBJECT_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipSubject, szSipSubject);
}

string COspSipMsg::GetSipDate() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipDate);
}
void COspSipMsg::SetSipDate(LPCSTR szSipDate)
{
    if (szSipDate == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipDate == null\n");
        return;
    }

    if (strlen(szSipDate) > MAX_SIP_DATE_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipDate长度大于[%d]\n", MAX_SIP_DATE_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipDate, szSipDate);
}

u32 COspSipMsg::GetSipExpire() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_dwSipExpire;
}
void COspSipMsg::SetSipExpire(u32 dwSipExpire)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_dwSipExpire = dwSipExpire;
}

string COspSipMsg::GetSipCallID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipCallID);
}
void COspSipMsg::SetSipCallID(LPCSTR szSipCallID)
{
    if (szSipCallID == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipCallID == null\n");
        return;
    }

    if (strlen(szSipCallID) > MAX_SIP_CALLID_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipCallID长度大于[%d]\n", MAX_SIP_CALLID_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipCallID, szSipCallID);
}

string COspSipMsg::GetSipLocalIp() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipLocalIp);
}
void COspSipMsg::SetSipLocalIp(LPCSTR szSipIp)
{
    if (szSipIp == NULL)
    {
        //会话内消息kdsip可能没有
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipIp == null\n");
        return;
    }

    if (strlen(szSipIp) > MAX_SIP_IPADDR_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipIp长度大于[%d]\n", MAX_SIP_IPADDR_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipLocalIp, szSipIp);
}

u16 COspSipMsg::GetSipLocalPort() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_wSipLocalPort;
}
void COspSipMsg::SetSipLocalPort(u16 wSipPort)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_wSipLocalPort = wSipPort;
}


string COspSipMsg::GetSipRemoteIp() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return string(ptMsgHead->m_szSipRemoteIp);
}
void COspSipMsg::SetSipRemoteIp(LPCSTR szSipIp)
{
    if (szSipIp == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipIp == null\n");
        return;
    }

    if (strlen(szSipIp) > MAX_SIP_IPADDR_LEN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szSipIp长度大于[%d]\n", MAX_SIP_IPADDR_LEN);
        return;
    }

    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    strcpy(ptMsgHead->m_szSipRemoteIp, szSipIp);
}

u16 COspSipMsg::GetSipRemotePort() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_wSipRemotePort;
}
void COspSipMsg::SetSipRemotePort(u16 wSipPort)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_wSipRemotePort = wSipPort;
}

TOspSipMsgHead* COspSipMsg::GetMsgHead()
{
    return (TOspSipMsgHead*)m_abyBuffer;
}

const TOspSipMsgHead* COspSipMsg::GetMsgHead() const
{
    return (const TOspSipMsgHead*)m_abyBuffer;
}

void COspSipMsg::ClearMsgBody(void)                //清空有效消息体
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();

    u16 wBodyLen = ptMsgHead->m_wMsgBodyLen;
    if (wBodyLen != 0)
    {//warning: memset used with constant zero length parameter;
        //不要使用 0 长度 去 memset
        memset(&m_abyBuffer[OSP_SIP_MSG_BODY_OFFSET], 0, wBodyLen);
    }        

    ptMsgHead->m_wMsgBodyLen = 0;
}

u16 COspSipMsg::GetOspEventID() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_wOspEventId;
}

void COspSipMsg::SetOspEventID(u16 wEventID)
{
    TOspSipMsgHead* ptMsgHead = GetMsgHead();
    ptMsgHead->m_wOspEventId = wEventID;
}


bool COspSipMsg::SetMsgBody(const void* pMsgBody, u16 wBodyLen)
{
    if (0 == wBodyLen)
    {
        this->ClearMsgBody();
        return true;
    }
    if(pMsgBody != NULL && wBodyLen > 0)
    {            
        if(wBodyLen > OSP_SIP_MSG_BODY_MAX_LEN)
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "SIP消息体长度大于最大缓冲区[%hu]，达到[%hu]，设置失败\n", 
                OSP_SIP_MSG_BODY_MAX_LEN, wBodyLen);
            return false;
        }
        TOspSipMsgHead* ptMsgHead = GetMsgHead();
        if(ptMsgHead == NULL)
            return false;;

        memcpy(&m_abyBuffer[OSP_SIP_MSG_BODY_OFFSET], pMsgBody, wBodyLen);
        ptMsgHead->m_wMsgBodyLen = wBodyLen;

        return true;
    }

    CLASSLOG(OSPSIP, ERROR_LEV, "无效数据:wBodyLen = %hu, pMsgBody = %p\n",
        wBodyLen, pMsgBody);

    return false;
}

LPCSTR COspSipMsg::GetMsgBody() const
{
    return (const char*)(&m_abyBuffer[OSP_SIP_MSG_BODY_OFFSET]);
}

u16 COspSipMsg::GetMsgBodyLen() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return ptMsgHead->m_wMsgBodyLen;
}

u16 COspSipMsg::GetMsgLen() const
{
    const TOspSipMsgHead* ptMsgHead = GetMsgHead();
    return (ptMsgHead->m_wMsgBodyLen + OSP_SIP_MSG_BODY_OFFSET);
}

const u8* COspSipMsg::GetMsgBuf() const//获取整个消息指针，用户不需提供BUFFER
{
    return (const u8 *)m_abyBuffer ;
}

string COspSipMsg::GetUserAgent() const
{
	const TOspSipMsgHead* ptMsgHead = GetMsgHead();
	return string(ptMsgHead->m_szUserAgent);
}

void COspSipMsg::SetUserAgent( LPCSTR szUserAgent )
{
	if (szUserAgent == NULL)
	{
		CLASSLOG(OSPSIP, ERROR_LEV, "szUserAgent == null\n");
		return;
	}

	if (strlen(szUserAgent) > MAX_SIP_USERAGENT_LEN)
	{
		CLASSLOG(OSPSIP, ERROR_LEV, "szUserAgent长度大于[%d]\n", MAX_SIP_USERAGENT_LEN);
		return;
	}

	TOspSipMsgHead* ptMsgHead = GetMsgHead();
	strcpy(ptMsgHead->m_szUserAgent, szUserAgent);
}


int GetXmlNodeInt(const string& strXml, const string& strNodeName)
{
    string strInt = GetXmlNodeText(strXml, strNodeName);
    return atoi(strInt.c_str());
}

//siptask 实现，方便sip接口在task中的调用
u32 CSipTask::PostReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo)
{
    TOspTransation tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
    tOspTrans.dwTaskID = GetTaskNO();

    if (ptSipExtraInfo == NULL)
    {
        tOspTrans.dwSeqNO = (u32)GetXmlNodeInt(strMsgReq, XML_SEQ_NUM_NODE);
    }
    
    if ( !OspSipPostReq(eSIPMethod, strMsgReq, tDstURI, tOspTrans, szDirectUAAddr, ptSipExtraInfo) )
    {
        return PROCMSG_FAIL;
    }
    return PROCMSG_OK;
}

u32 CSipTask::PostInDlgReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, TKDSIP_DIALOG_ID tDstDlgId, const TSipExtraInfo* ptSipExtraInfo)
{
    TOspTransation tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
    tOspTrans.dwTaskID = GetTaskNO();

    if (ptSipExtraInfo == NULL)
    {
        tOspTrans.dwSeqNO = (u32)GetXmlNodeInt(strMsgReq, XML_SEQ_NUM_NODE);
    }

    if ( !OspSipPostInDlgReq(eSIPMethod, strMsgReq, tDstDlgId, tOspTrans, ptSipExtraInfo) )
    {
        return PROCMSG_FAIL;
    }
    return PROCMSG_OK;
}

u32 CSipTask::PostRsp(EKDSIP_EVENT eSIPMethod, TKDSIP_TRANS_ID tTransId, const string &strMsgRsp, 
    EKDSIP_RESPONSE_CODE eRspCode, const TSipExtraInfo* ptSipExtraInfo)
{
    TOspTransation tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
    tOspTrans.dwTaskID = GetTaskNO();

    if (ptSipExtraInfo == NULL)
    {
        tOspTrans.dwSeqNO = (u32)GetXmlNodeInt(strMsgRsp, XML_SEQ_NUM_NODE);
    }

    if ( !OspSipPostRsp(eSIPMethod, tTransId, strMsgRsp, tOspTrans, eRspCode, ptSipExtraInfo) )
    {
        return PROCMSG_FAIL;
    }
    return PROCMSG_OK;
}

u32 CSipTask::PostMsgReq(const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo)
{
    return PostReq(KDSIP_EVENT_MESSAGE_REQ, strMsgReq, tDstURI, szDirectUAAddr, ptSipExtraInfo);
}

u32 CSipTask::PostMsgRsp(TKDSIP_TRANS_ID tTransId, const string &strMsgRsp, 
    EKDSIP_RESPONSE_CODE eRspCode, const TSipExtraInfo* ptSipExtraInfo)
{
    return PostRsp(KDSIP_EVENT_MESSAGE_RSP, tTransId, strMsgRsp, eRspCode, ptSipExtraInfo);
}

bool CSipTask::SetHBParam(const TSipURI &tDstURI, LPCSTR szDirectUAAddr, u16 wHbTime, u16 wHbCnt)
{
    TOspTransID tOspTransId;
    tOspTransId.dwIID = MAKEIID(GetAppID(), GetInsID());
    tOspTransId.dwTaskID = GetTaskNO();
    tOspTransId.dwSeqNO = INVALID_SEQ_NO;
    return OspSipSetHBParam(tDstURI, tOspTransId, szDirectUAAddr, wHbTime, wHbCnt);
}

bool TSipStackConf::ReadKeyCfg(const s8* szConfPath)
{
    if (szConfPath == NULL)
    {
        printf("kdsip配置文件路径为空\n");
        return false;
    }

    TKdsipKeyCfg tKeyCfg;

    s32 nVal = 0;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_MAX_TRANS, DEFAULT_KDSIP_MAX_TRANS, &nVal);
    tKeyCfg.nMaxTrans = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_MAX_REG_DLG, DEFAULT_KDSIP_MAX_REG_DLG, &nVal);
    tKeyCfg.nMaxRegDlg = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_MAX_INVITE_DLG, DEFAULT_KDSIP_MAX_INVITE_DLG, &nVal);
    tKeyCfg.nMaxInviteDlg = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_MAX_SUBSCRIBE_DLG, DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG, &nVal);
    tKeyCfg.nMaxSubscribeDlg = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_MAX_MAX_NTF_PER_SUB, DEFAULT_KDSIP_MAX_NTF_PER_SUB, &nVal);
    tKeyCfg.nNumOfNotifysPerSub = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_DISABLE_CHECKSUM, DEFAULT_KDSIP_DISABLE_CHECKSUM, &nVal);
    tKeyCfg.bDisableCheckSum = nVal;

    GetRegKeyInt(szConfPath, SECTION_KDSIP, KEY_KDSIP_ROUTE_MODE, DEFAULT_KDSIP_ROUTE_MODE, &nVal);
    tKeyCfg.nSipRouteMode = nVal;

    SetKeyCfg(tKeyCfg);

    return true;
}

void TSipStackConf::SetKeyCfg(const TKdsipKeyCfg& tKeyCfg)
{
    tKdSipConfig.nMaxTrans = (tKeyCfg.nMaxTrans<=0) ? 1 : tKeyCfg.nMaxTrans; 
    tKdSipConfig.nMaxRegisterDialog = (tKeyCfg.nMaxRegDlg<=0) ? 1 : tKeyCfg.nMaxRegDlg;
    tKdSipConfig.nMaxInviteDialog = (tKeyCfg.nMaxInviteDlg<=0) ? 1 : tKeyCfg.nMaxInviteDlg;
    tKdSipConfig.nMaxSubscribeDialog = (tKeyCfg.nMaxSubscribeDlg<=0) ? 1 : tKeyCfg.nMaxSubscribeDlg;
    tKdSipConfig.nNumOfNotifysPerSub = (tKeyCfg.nNumOfNotifysPerSub<=5) ?  5 : tKeyCfg.nNumOfNotifysPerSub;
    tKdSipConfig.bUDPChecksumDisable = tKeyCfg.bDisableCheckSum;
    nSipRouteMode = tKeyCfg.nSipRouteMode; 
}

