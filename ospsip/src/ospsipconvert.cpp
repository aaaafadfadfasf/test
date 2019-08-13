#include "ospsipconvert.h"
#include "ospsiphb.h"
#include "ospsipconst.h"
#include "cms/cmu/cmu_proto.h"
#include "cbb/sip/sip_tool.h"
#include "ospsipgbnat.h"

extern PGetSipXmlMsgTypeCB g_pCBGetSipXmlMsgType;

extern s32 g_nRegProxyExpires;

extern TOspSipConf g_tOspSipConf;

COspSipConvertor g_cOspSipConvertor;

CSipCoreMsgQue g_cOspSipCoreMsgQue("SipCoreQue");

bool WriteCoreQue(const TOspSipCoreMsg& tMsg)
{
    bool bRet = g_cOspSipCoreMsgQue.PostMsg(tMsg);

    if (!bRet)
    {       
        if (tMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_SEND)
        {
            GLOBELOG(ERROR_LEV, "投递SipSendMsg失败\n");
            TOspSipSendMsg* ptSipSendMsg = (TOspSipSendMsg*)tMsg.pvMsg;
            delete ptSipSendMsg;
        }
        else if (tMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_RECV)
        {
            GLOBELOG(ERROR_LEV, "投递SipRecvMsg失败\n");
            CKDSipEvent* ptSipEvent = (CKDSipEvent*)tMsg.pvMsg;
            KDSipFreeSipEvent(ptSipEvent); 
        }
        else
        {
             GLOBELOG(ERROR_LEV, "投递未知SipMsg失败\n");
        }        
    }

    return bRet;
}

TOspSipCoreMsg ReadCoreQue(u32 dwMillSec)
{
    TOspSipCoreMsg tMsg;
    g_cOspSipCoreMsgQue.GetMsg(tMsg, dwMillSec);

    return tMsg;
}

bool SendMsgToOspApp(const COspSipMsg& cMsg)
{
    u8 byLogLev = EVENT_LEV;
    if (cMsg.GetSipEventID() == KDSIP_EVENT_OPTIONS_HB_RSP
        || cMsg.GetSipEventID() == KDSIP_EVENT_OPTIONS_HB_REQ)
    {
        byLogLev = TIMER_LEV;
    }

    GLOBELOG(OSPSIP, byLogLev, "send msg[%s-%d] size[%u bytes] to dst[app:%u,ins:%u,task:%u]\n",
        OspExtEventDesc(cMsg.GetOspEventID()).c_str(), cMsg.GetOspEventID(), cMsg.GetMsgLen(),
        GETAPP(cMsg.GetOspIID()), GETINS(cMsg.GetOspIID()), cMsg.GetOspTaskNO());

    if (cMsg.GetMsgLen() >= MAX_MSG_LEN)
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "send msg[%s-%d] size[%u bytes] to dst[app:%u,ins:%u,task:%u] fail, msg len overload[%u]\n",
            OspExtEventDesc(cMsg.GetOspEventID()).c_str(), cMsg.GetOspEventID(), cMsg.GetMsgLen(),
            GETAPP(cMsg.GetOspIID()), GETINS(cMsg.GetOspIID()), cMsg.GetOspTaskNO(), MAX_MSG_LEN);
        return false;
    }

    s32 nOspRet = ::OspPost(cMsg.GetOspIID(), cMsg.GetOspEventID(), cMsg.GetMsgBuf(), cMsg.GetMsgLen());
    if (nOspRet == OSP_OK)
    {      
        return true;
    }
    else
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "send msg[%s-%d] size[%u bytes] to dst[app:%u,ins:%u,task:%u] fail, errcode[%d]\n",
            OspExtEventDesc(cMsg.GetOspEventID()).c_str(), cMsg.GetOspEventID(), cMsg.GetMsgLen(),
            GETAPP(cMsg.GetOspIID()), GETINS(cMsg.GetOspIID()), cMsg.GetOspTaskNO(), nOspRet);
        return false;
    }
}

bool IsSipMsg(EKDSIP_EVENT eEvType)
{
    return (eEvType > KDSIP_EVENT_UNKNOWN && eEvType < KDSIP_EVENT_COUNT);
}

bool IsSipReqMsg(EKDSIP_EVENT eEvType)
{
    bool bIsReq = false;
    switch (eEvType)
    {
    case KDSIP_EVENT_REGISTER_REQ:
    case KDSIP_EVENT_UNREGISTER_REQ:
    case KDSIP_EVENT_INVITE_REQ:
    case KDSIP_EVENT_INVITE_BYE_REQ:
    case KDSIP_EVENT_INVITE_ACK:
    case KDSIP_EVENT_INFO_REQ:
    //case KDSIP_EVENT_OPTIONS_REQ:
    case KDSIP_EVENT_OPTIONS_HB_REQ:
    case KDSIP_EVENT_MESSAGE_REQ:
    case KDSIP_EVENT_MESSAGE_DIALOG_REQ:
    case KDSIP_EVENT_SUBSCRIBE_REQ:
    case KDSIP_EVENT_UNSUBSCRIBE_REQ:
    case KDSIP_EVENT_NOTIFY_REQ:
        {
            bIsReq = true;
        }
        break;

    default:
        break;
    }

    return bIsReq;
}

bool IsSipRspMsg(EKDSIP_EVENT eEvType)
{
    bool bIsRsp = false;

    switch (eEvType)
    {
    case KDSIP_EVENT_REGISTER_RSP:
    case KDSIP_EVENT_UNREGISTER_RSP:
    case KDSIP_EVENT_INVITE_RSP:
    case KDSIP_EVENT_INVITE_BYE_RSP:
    case KDSIP_EVENT_INFO_RSP:
    //case KDSIP_EVENT_OPTIONS_RSP:
    case KDSIP_EVENT_OPTIONS_HB_RSP:
    case KDSIP_EVENT_MESSAGE_RSP:
    case KDSIP_EVENT_MESSAGE_DIALOG_RSP:
    case KDSIP_EVENT_SUBSCRIBE_RSP:
    case KDSIP_EVENT_UNSUBSCRIBE_RSP:
    case KDSIP_EVENT_NOTIFY_RSP:
        {
            bIsRsp = true;
        }
        break;

    default:
        break;
    }

    return bIsRsp;    
}

bool IsSipCreateMsg(EKDSIP_EVENT eEvType)
{
    bool bIsCreate = false;

    switch (eEvType)
    {
    case KDSIP_EVENT_DIALOG_CREATED:
    case KDSIP_EVENT_TRANSC_CREATED:
        {
            bIsCreate = true;
        }
        break;

    default:
        break;
    }

    return bIsCreate; 
}

bool IsSipExceptMsg(EKDSIP_EVENT eEvType)
{
    bool bIsDestroy = false;

    switch (eEvType)
    {
    case KDSIP_EVENT_DIALOG_TERMINATE:
    case KDSIP_EVENT_TRANSACTION_TIMEOUT:
        {
            bIsDestroy = true;
        }
        break;

    default:
        break;
    }

    return bIsDestroy; 
}

bool IsKedacomSipMsg(const CKDSipMsgHead* pcSipHead)
{
    if (pcSipHead == NULL)
    {
        return false;
    }

    if (pcSipHead->GetUserAgent() != NULL
        && strcmp(USER_AGENT_KEDACOM, pcSipHead->GetUserAgent()) == 0)
    {
        return true;
    }

    return false;
}

#define ADD_SIP_EVENT_DESC(x) AddSipEventDesc(x, #x)
class CSipEventDesc
{
public:
    CSipEventDesc()
    {
        InitSipEventDesc();
    }

public:
    void AddSipEventDesc(size_t nval, LPCSTR szval)
    {
        m_tSipEventDesc.AddEventDesc(szval, (u16)nval);
    }

    void InitSipEventDesc()
    {
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_UNKNOWN);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_REGISTER_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_REGISTER_RSP);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_UNREGISTER_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_UNREGISTER_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INVITE_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INVITE_RSP);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INVITE_ACK);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INVITE_BYE_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INVITE_BYE_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INFO_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_INFO_RSP);

        //ADD_SIP_EVENT_DESC(KDSIP_EVENT_OPTIONS_REQ);
        //ADD_SIP_EVENT_DESC(KDSIP_EVENT_OPTIONS_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_OPTIONS_HB_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_OPTIONS_HB_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_MESSAGE_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_MESSAGE_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_MESSAGE_DIALOG_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_MESSAGE_DIALOG_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_SUBSCRIBE_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_SUBSCRIBE_RSP);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_UNSUBSCRIBE_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_UNSUBSCRIBE_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_NOTIFY_REQ);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_NOTIFY_RSP);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_DIALOG_TERMINATE);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_TRANSACTION_TIMEOUT);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_DIALOG_CREATED);
        ADD_SIP_EVENT_DESC(KDSIP_EVENT_TRANSC_CREATED);

        ADD_SIP_EVENT_DESC(KDSIP_EVENT_COUNT);
    }

    const string GetSipEventDesc(int nSipEvent)
    {
        if (nSipEvent < KDSIP_EVENT_COUNT && nSipEvent > KDSIP_EVENT_UNKNOWN)
        {
            return m_tSipEventDesc.GetEventDesc((u16)nSipEvent);
        }

        return m_tSipEventDesc.GetEventDesc(KDSIP_EVENT_UNKNOWN);
    }

private:
    CEventDescMap m_tSipEventDesc;
};

static CSipEventDesc g_tSipEventDesc;

const string SipEventDesc(int nSipEvent)
{
    return g_tSipEventDesc.GetSipEventDesc(nSipEvent);
}

COspSipConvertor::COspSipConvertor()
{
    m_dwDefaultOspIID = INVALID_IID;
}

/*====================================================================
函数名      : DoConvert
功能        : 处理kdsip回调给ospsip的消息
算法实现    : 消息分类：创建、销毁、请求、应答
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/04/09/             fanxg
====================================================================*/
bool COspSipConvertor::DoConvert(const CKDSipEvent* pSipEvent)
{
    if (pSipEvent == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sipevent指针为空\n");
        return false;
    }

    u8 byLogLev = EVENT_LEV;
    if (pSipEvent->GetEvent() == KDSIP_EVENT_OPTIONS_HB_RSP
        || pSipEvent->GetEvent() == KDSIP_EVENT_OPTIONS_HB_REQ)
    {
        byLogLev = TIMER_LEV;
    }

    u32 dwSN = pSipEvent->GetSN();
    if (dwSN != OSP_SIP_INVALID_KDSIP_SN)
    {
        TOspSipBind tBind = FindSNBind(dwSN);
        if (tBind.eSipEvent ==  KDSIP_EVENT_OPTIONS_HB_RSP
            || tBind.eSipEvent == KDSIP_EVENT_OPTIONS_HB_REQ)
        {
            byLogLev = TIMER_LEV;
        }     
    }

    //ospsip和kdsip基本稳定，降低一些日志的级别，避免日志文件被很快覆盖
    byLogLev = TRIVIAL_LEV;

    CLASSLOG(OSPSIP, byLogLev, "recv sip event[%s-%d][status code: %d], sip id[%u], transid[%u], SN[%lu]\n",
        SipEventDesc(pSipEvent->GetEvent()).c_str(),
        pSipEvent->GetEvent(),
        pSipEvent->GetResponseCode(), 
        pSipEvent->GetDialogID(),
        pSipEvent->GetTransID(),
        pSipEvent->GetSN());


    bool bSendToOspApp = false;
    COspSipMsg cMsg;   

    if (IsSipCreateMsg(pSipEvent->GetEvent()))
    {
        bSendToOspApp = ProcSipCreate(pSipEvent, cMsg);
    }
    else if (IsSipExceptMsg(pSipEvent->GetEvent()))
    {
        bSendToOspApp = ProcSipExcept(pSipEvent, cMsg);
    }
    else
    {
        if (!SetSipHeadAndBody(pSipEvent, cMsg))
        {
            return false;
        }
        
        if (IsSipReqMsg(pSipEvent->GetEvent()))
        {
            bSendToOspApp = ProcSipReq(pSipEvent, cMsg);
        }
        else if (IsSipRspMsg(pSipEvent->GetEvent()))
        {
            bSendToOspApp = ProcSipRsp(pSipEvent, cMsg);
        }
        else
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "未知SIP消息类型[%u]\n", pSipEvent->GetEvent());
        }
    }
    
    if (bSendToOspApp)
    {
        if (cMsg.GetOspIID() != INVALID_IID)
        {
            SendMsgToOspApp(cMsg);
        }
        else
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "sip消息[%s-%u]需要发送到上层，但osp IID无效，可能band已经超时删除\n",
                SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
            return false;
        }
    }

    return true;
}

bool COspSipConvertor::SetSipHeadAndBody(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg)
{
    u8 byLogLev = EVENT_LEV;
    if (pSipEvent->GetEvent() == KDSIP_EVENT_OPTIONS_HB_RSP
        || pSipEvent->GetEvent() == KDSIP_EVENT_OPTIONS_HB_REQ)
    {
        byLogLev = TIMER_LEV;
    }

    cMsg.SetSipEventID(pSipEvent->GetEvent());
    cMsg.SetSipDlgID(pSipEvent->GetDialogID());
    cMsg.SetSipTransID(pSipEvent->GetTransID());

    //每条SIP消息会携带对端和本端使用的网络地址信息

    //fanxg-20140428: kdsip目前实现对于notify消息是没有设置地址信息的，为了避免过多的错误打印，这里屏蔽掉
    if (pSipEvent->GetEvent() != KDSIP_EVENT_NOTIFY_REQ
        && pSipEvent->GetEvent() != KDSIP_EVENT_NOTIFY_RSP)
    {
        CLASSLOG(OSPSIP, TRIVIAL_LEV, "local[%s:%u], remote[%s:%u]\n",
            pSipEvent->GetDstIP(),
            pSipEvent->GetDstPort(),
            pSipEvent->GetSrcIP(),
            pSipEvent->GetSrcPort());

        cMsg.SetSipLocalIp(pSipEvent->GetDstIP());
        cMsg.SetSipLocalPort(pSipEvent->GetDstPort());
        cMsg.SetSipRemoteIp(pSipEvent->GetSrcIP());
        cMsg.SetSipRemotePort(pSipEvent->GetSrcPort());
    }
    

    //设置SIP消息头
    const CKDSipMsgHead* pSipHead = pSipEvent->GetSipHead();
    if (pSipHead != NULL)
    {     
        //call-id头域
        if (pSipHead->GetCallID() != NULL)
        {
            /*
               国标中使用单向心跳，下级向上级心跳请求，上级应答。上下级通过这种机制确认双方在线。
               当ua下线又立刻重启，需要通过callid来确认是新的注册还是刷新注册。
               说明：同一个注册会话内的刷新注册使用和第一个注册请求相同的callid。
            */
            CLASSLOG(OSPSIP, TRIVIAL_LEV, "SipHead.CallID = %s\n",
                pSipHead->GetCallID());

            cMsg.SetSipCallID(pSipHead->GetCallID());
        }

        //sip协议中from和to不可为空;
        if (pSipHead->GetFromURI() == NULL || pSipHead->GetToURI() == NULL)
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "FromURI() == NULL || ToURI() == NULL\n");
            return false;
        }

        CLASSLOG(OSPSIP, byLogLev, "SipHead.FromUri = %s, SipHead.ToUri = %s\n",
            pSipHead->GetFromURI(), pSipHead->GetToURI());

        string strFrom = GetSipUri(pSipHead->GetFromURI());
        cMsg.SetSipFromUri(strFrom.c_str());

        string strTo = GetSipUri(pSipHead->GetToURI());
        cMsg.SetSipToUri(strTo.c_str());

        //contact头域可能为空
        if (pSipHead->GetContactURI() != NULL)
        {
            CLASSLOG(OSPSIP, TRIVIAL_LEV, "SipHead.ContactUri = %s\n",
                pSipHead->GetContactURI());

            string strContact = GetSipUri(pSipHead->GetContactURI());
            cMsg.SetSipContactUri(strContact.c_str());
        }      

        //对于注册需要获取contact头域和auth头域
        if (pSipEvent->GetEvent() == KDSIP_EVENT_REGISTER_REQ
            || pSipEvent->GetEvent() == KDSIP_EVENT_UNREGISTER_REQ)
        {        
            //auth头域
            CLASSLOG(OSPSIP, byLogLev, "SipHead.Auth = %s\n",
                pSipHead->GetAuthorization());

            cMsg.SetSipAuthInfo(pSipHead->GetAuthorization());

            if (!IsKedacomSipMsg(pSipHead))
            {
                cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
            }

            cMsg.SetSipExpire(pSipEvent->GetExpire());

            //RequestUri头域：对于REGISTER方法，GetProxyURI()得到的就是RequestUri
            if (pSipHead->GetProxyURI() != NULL)
            {
                CLASSLOG(OSPSIP, byLogLev, "SipHead.RequestUri = %s\n",
                    pSipHead->GetProxyURI());

                string strRequest = GetSipUri(pSipHead->GetProxyURI());
                cMsg.SetSipRequestUri(strRequest.c_str());
            }    
        }

		//UserAgent字段
		if (pSipHead->GetUserAgent())
		{
			cMsg.SetUserAgent(pSipHead->GetUserAgent());
		}
    }
    else
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "error sip event[%s-%d][status code: %d], sip id[%u], transid[%u], SipHead == NULL!!!\n",
            SipEventDesc(pSipEvent->GetEvent()).c_str(),
            pSipEvent->GetEvent(),
            pSipEvent->GetResponseCode(), 
            pSipEvent->GetDialogID(),
            pSipEvent->GetTransID());

        return false;
    }

    //设置SIP消息体
    const s8* pchContent = NULL;
    s32 nLen = 0;

    string strMsgType;
    if (pSipEvent->GetSipBody())
    {
        cMsg.SetSipContentType(pSipEvent->GetSipBody()->GetContentType());

        pchContent = reinterpret_cast<const s8*>(pSipEvent->GetSipBody()->GetMsgBodyContent(&nLen));
        if (pchContent == NULL)
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "Sip Msg Body Content == NULL\n");
            return false;
        }

        if (pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_XML)
        {
            //这里是Keda的sip消息
            if (g_pCBGetSipXmlMsgType == NULL)
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "g_pCBGetSipXmlMsgType == NULL\n");
                return false;
            }
            strMsgType = g_pCBGetSipXmlMsgType(pchContent);

            u16 wMsgType = OspExtEventID(strMsgType.c_str());
            if (wMsgType != INVALID_OSP_EVENT)
            {
                CLASSLOG(OSPSIP, EVENT_LEV, "收到业务消息[%s-%d]\n", 
                    strMsgType.c_str(), wMsgType);
                cMsg.SetOspEventID(wMsgType);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "未知OSP消息类型[%s],请使用OSP_ADD_EVENT_DESC()注册消息\n", strMsgType.c_str());
                return false;
            }
        }
        else if (pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_SDP
            || pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_MANSCDP
            || pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_MANSRTSP)
        {
            //这里是GB28181的sip消息，国标消息使用统一的消息号投递给上层
            cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);

            //对于INVITE需要获取Subject头域
            if (pSipEvent->GetEvent() == KDSIP_EVENT_INVITE_REQ)
            {
                cMsg.SetSipSubject(pSipHead->GetSubject());
            }
        }
        else
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "Unknown Sip ContentType[%d]\n",
                int(pSipEvent->GetSipBody()->GetContentType()));
            return false;
        }

        if (!cMsg.SetMsgBody(pchContent, u16(nLen+1))) //+1: 表示将字符串结束符'\0'也返回给上层
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "设置sip消息体失败pchContent = %p, nLen = %d\n",
                pchContent, nLen+1);
            return false;
        }     
    }

    return true;
}

string COspSipConvertor::GetSipUri(LPCSTR szKdsipUri)
{
    string strUri;

    if (szKdsipUri == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "szKdsipUri == NULL\n");
        return strUri;
    }

    CKDSipURITOOL tKdsipUri(szKdsipUri);

    //sip协议中user(o)可以为空，domain(m)不可为空;
    if (tKdsipUri.GetDomain() == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sipUri非法，sipUri.User = %s, sipUri.Domain = %s\n",
            tKdsipUri.GetUser(), tKdsipUri.GetDomain());
        return strUri;
    }


    if (tKdsipUri.GetUser() != NULL)
    {
        strUri += tKdsipUri.GetUser();
    }
    else
    {
        CLASSLOG(OSPSIP, EVENT_LEV, "sipUri没有携带user，tKdsipUri.User = %s, tKdsipUri.Domain = %s, tKdsipUri.Port = %d\n",
            tKdsipUri.GetUser(), tKdsipUri.GetDomain(), tKdsipUri.GetPort());
    }
    
    strUri += "@";
    strUri += tKdsipUri.GetDomain();

    if (tKdsipUri.GetPort() != KDSIP_NO_PORT)
    {
        strUri += ":";
        char szPort[32] = {0};
        sprintf(szPort, "%hu", tKdsipUri.GetPort());
        strUri += szPort;
    }

    return strUri;
}

bool COspSipConvertor::ProcSipCreate(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg)
{
    bool bSendToOspApp = false;

    u32 dwSN = pSipEvent->GetSN();
    if(dwSN == OSP_SIP_INVALID_KDSIP_SN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "OSP_SIP_INVALID_KDSIP_SN\n");
        return false;
    }

    TOspSipBind tBind = FindSNBind(dwSN);
    if (tBind.eSipEvent == KDSIP_EVENT_UNKNOWN)
    {
        //如果找不到的是对话，需要将该对话release掉
        if (pSipEvent->GetEvent() == KDSIP_EVENT_DIALOG_CREATED)
        {
            //上层认为Dlg创建超时了，但kdsip层认为创建成功了，这里要释放被创建的超时Dlg。
            KDSIP_DIALOG_ID tSipDlgId = pSipEvent->GetDialogID();
            EKDSIP_DIALOG_TYPE tSipDlgType = pSipEvent->GetDialogType();

            CLASSLOG(OSPSIP, ERROR_LEV, "SN[%lu]没有对应的对话[DlgType:%d, DlgId:%lu]bind，可能对话创建超过[%ds]才回调\n", 
                dwSN, tSipDlgType, tSipDlgId, OSP_SIP_RESOURCE_CREATE_TIMEOUT);

            if (tSipDlgId != INVALID_DIALOG_ID)
            {
                if (tSipDlgType == KDSIP_DIALOG_TYPE_REGISTER)
                {
                    KDSipTerminateRegDialog(tSipDlgId);
                }
                else if (tSipDlgType == KDSIP_DIALOG_TYPE_INVITE)
                {
                    KDSipTerminateInviteDialog(tSipDlgId);
                }
                else if (tSipDlgType == KDSIP_DIALOG_TYPE_SUBSCRIBE)
                {
                    KDSipTerminateSubsDialog(tSipDlgId);
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "unknown DlgType[%d], DlgId[%lu]!!!!!!!\n", 
                        tSipDlgType, tSipDlgId);
                }
            }    
        }
        else
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "SN[%lu]没有对应的事务[TransId:%lu]bind，可能事务创建超过[%ds]才回调\n", 
                dwSN, pSipEvent->GetTransID(), OSP_SIP_RESOURCE_CREATE_TIMEOUT);
        }
        
        return false;
    }
    else
    {
        u8 byLogLev = EVENT_LEV;
        if (tBind.eSipEvent ==  KDSIP_EVENT_OPTIONS_HB_RSP
            || tBind.eSipEvent == KDSIP_EVENT_OPTIONS_HB_REQ)
        {
            byLogLev = TIMER_LEV;
        }

        //ospsip和kdsip基本稳定，降低一些日志的级别，避免日志文件被很快覆盖
        byLogLev = TRIVIAL_LEV;

        CLASSLOG(OSPSIP, byLogLev, "[SN = %lu][dlgId:%lu, tranId:%lu] find bind: sip event[%s-%d]-SN[%lu], osp event[%s], "
            "dst[%s], src[app:%u,ins:%u,task:%u]\n", 
            dwSN, pSipEvent->GetDialogID(), pSipEvent->GetTransID(), SipEventDesc(tBind.eSipEvent).c_str(), 
            tBind.eSipEvent, tBind.dwKdsipSN, tBind.strOspEvent.c_str(), tBind.tSipURI.GetURIString().c_str(), 
            GETAPP(tBind.tOspId.dwIID), GETINS(tBind.tOspId.dwIID), tBind.tOspId.dwTaskID);

        EraseSN(dwSN);
    }

    switch (pSipEvent->GetEvent())
    {  
    case KDSIP_EVENT_DIALOG_CREATED:
        {          
            KDSIP_DIALOG_ID tDlgId = pSipEvent->GetDialogID();
            if (tDlgId == INVALID_DIALOG_ID)
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "sip[%s-%d]对话创建失败，SN[%lu]\n", 
                    SipEventDesc(tBind.eSipEvent).c_str(), tBind.eSipEvent, dwSN);
                return false;
            }

            tBind.tSipId.tSipDlg = tDlgId;
            InsertDlg(tBind, tDlgId);         
        }
        break;

    case KDSIP_EVENT_TRANSC_CREATED:
        {
            KDSIP_TRANS_ID tTransId = pSipEvent->GetTransID();
            if (tTransId == INVALID_TRANS_ID)
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "sip[%s-%d]事务创建失败，SN[%lu]\n", 
                    SipEventDesc(tBind.eSipEvent).c_str(), tBind.eSipEvent, dwSN);

				if (KDSIP_EVENT_INVITE_BYE_REQ==tBind.eSipEvent)
				{
					CLASSLOG(OSPSIP, ERROR_LEV,"[%s-%d]事务创建失败,删除对应Dialog[%p]\n", 
						SipEventDesc(tBind.eSipEvent).c_str(), (s32)tBind.eSipEvent, tBind.tSipId.tSipDlg);
					EraseDlg(tBind.tSipId.tSipDlg);
				}

                return false;
            }

            tBind.tSipId.tSipTrans = tTransId;
            InsertOutTrans(tBind, tTransId); 
        }
        break;

    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip msg [%s-%u], won't be processed!!!\n", 
                SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
        }
        break;
    }

    return bSendToOspApp;
}

bool COspSipConvertor::ProcSipExcept(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg)
{
    bool bSendToOspApp = true;
    TOspTransID tOspTrans;

    switch (pSipEvent->GetEvent())
    {  
    case KDSIP_EVENT_DIALOG_TERMINATE:
        {
            //kdsip底层的dialog异常终止通知
            tOspTrans = FindDlg(pSipEvent->GetDialogID());
            EraseDlg(pSipEvent->GetDialogID());

            cMsg.SetOspEventID(OSP_SIP_DIALOG_TERMINATE);
        }
        break;

    case KDSIP_EVENT_TRANSACTION_TIMEOUT:
        {
            //请求发送超时，本地协议栈会发送事件KDSIP_EVENT_TRANSACTION_TIMEOUT给ospsip。
            //该事件的处理需要区别对话内请求和对话外请求，对话内请求还需要判断请求类型。

            //不管是对话内还是对话外都要先删除事务的绑定关系
            tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            cMsg.SetOspEventID(OSP_SIP_MSG_PROC_FAIL);
            cMsg.SetSipErrorCode(KDSIP_RESPONSE_408_REQ_TIMEOUT);

            //如果是对话内事务请求超时，需要特殊处理bye、unsubscribe、unregister，
            //确保对话绑定关系被删除。因为上层发送这些请求后，
            //Osp事务可能已经销毁，该超时消息发送上层也不会处理。
            if (pSipEvent->GetDialogID() != INVALID_DIALOG_ID)
            {
                TOspSipBind tBind = FindDlgBind(pSipEvent->GetDialogID());
                if (tBind.eSipEvent == KDSIP_EVENT_INVITE_BYE_REQ
                    || tBind.eSipEvent == KDSIP_EVENT_UNREGISTER_REQ
                    || tBind.eSipEvent == KDSIP_EVENT_UNSUBSCRIBE_REQ)
                {
                    CLASSLOG(OSPSIP, WARNING_LEV, "sipReqInDlg[%s-%u] timeout, need erase dialog bind!!!\n", 
                        SipEventDesc(tBind.eSipEvent).c_str(), tBind.eSipEvent);
                    EraseDlg(pSipEvent->GetDialogID());
                }
            }
        }
        break;

    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip msg [%s-%u], won't be processed!!!\n", 
                SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
            bSendToOspApp = false;
        }
        break;
    }

    cMsg.SetOspTransID(tOspTrans);

    return bSendToOspApp;
}

bool COspSipConvertor::ProcSipReq(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg)
{
    const CKDSipMsgHead* pSipHead = pSipEvent->GetSipHead();
    if (pSipHead == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "error sip event[%s-%d][status code: %d], sip id[%u], transid[%u], SipHead == NULL!!!\n",
            SipEventDesc(pSipEvent->GetEvent()).c_str(),
            pSipEvent->GetEvent(),
            pSipEvent->GetResponseCode(), 
            pSipEvent->GetDialogID(),
            pSipEvent->GetTransID());

        return false;
    }

    bool bSendToOspApp = true;
    TOspTransID tOspTrans;

    string strMsgType = OspExtEventDesc(cMsg.GetOspEventID());

    TSipTransaction tSipId;
    tSipId.tSipDlg = pSipEvent->GetDialogID();
    tSipId.tSipTrans = pSipEvent->GetTransID();

    switch (pSipEvent->GetEvent())
    {
    case KDSIP_EVENT_REGISTER_REQ:
    case KDSIP_EVENT_UNREGISTER_REQ:
        {
            //kdsip对于服务端的register方法没有使用会话进行管理，这里直接将事务投递到上层。
            //上层需要进行鉴权、续航、检时、路由的管理

            tOspTrans.dwIID = GetDefaultOspIID();

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());         
        }
        break;

    case KDSIP_EVENT_INVITE_REQ:
        {
            tOspTrans.dwIID = GetDefaultOspIID();

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_INVITE_ACK:
        {
            tOspTrans = FindDlg(pSipEvent->GetDialogID());

            if (IsGBDlg(pSipEvent->GetDialogID()))
            {
                cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
            }
        }
        break;

    case KDSIP_EVENT_INVITE_BYE_REQ:
        {
            tOspTrans = FindDlg(pSipEvent->GetDialogID());

            if (IsGBDlg(pSipEvent->GetDialogID()))
            {
                cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
            }
            else
            {
                cMsg.SetOspEventID(OSP_SIP_BYE_REQ);
            }  

            EraseDlg(pSipEvent->GetDialogID());

            /*
            只要收到BYE就直接删除DLG的绑定关系，不再等上层BYE_RSP到达时删除，
            因为此时上层的业务也可能发送了BYE将自己删除了，BYE_RSP不会到来。
            */

            //             TOspSipBind tOspSipBind;
            //             tOspSipBind.tSipId = tSipId;
            //             tOspSipBind.tOspId = tOspTrans;
            //             tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            //             InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());           
        }
        break;

    case KDSIP_EVENT_INFO_REQ:
        {
            tOspTrans = FindDlg(pSipEvent->GetDialogID());

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_OPTIONS_HB_REQ:
        {          
            //只有本UA添加过的心跳目的发来的心跳请求才会应答。
            //这样可以防止以下的错误：
            //UA1和UA2连接成功并开启心跳，当UA2掉线后又迅速重启，此时UA1并没有感知到UA2断链
            //UA1继续向UA2发送心跳，因为UA2现在已经启动，可以应答UA1的心跳，所以UA1一直不会
            //感知到UA2断链，UA2在UA1中的信息就不会清除，进而UA2向UA1新发起的连接就不会成功。
            //这明显是非常严重的错误逻辑。
			
            if (g_cHbNodeList.FindHbNode(cMsg.GetSipFromUri()) != NULL
				&& g_tOspSipConf.tLocalURI.GetUser() == cMsg.GetSipToUri().GetUser()) //mod by lyt 20171227,proxy不会再发出模块下线通知，因此这里需要增加对to字段的校验，防止本ua id变化重启后，依旧给对端心跳应答200的问题。
            {
                CLASSLOG(OSPSIP, TIMER_LEV, "收到src[%s]心跳请求，直接应答200 ok\n", 
                    cMsg.GetSipFromUri().GetURIString().c_str());

                OspSipHeartbeatRsp(pSipEvent->GetTransID());            
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "收到src[%s]心跳请求，但src不在本地心跳列表，可能本地UA刚启动，直接应答404\n",
                    cMsg.GetSipFromUri().GetURIString().c_str());

                OspSipHeartbeatRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_404_NOT_FOUND); 
            }

            bSendToOspApp = false;
        }
        break;

    case KDSIP_EVENT_MESSAGE_REQ:
        {
            tOspTrans.dwIID = GetDefaultOspIID();

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_MESSAGE_DIALOG_REQ:
        {
            tOspTrans = FindDlg(pSipEvent->GetDialogID());

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_SUBSCRIBE_REQ:
        {
            
            if (!ExistDlg(pSipEvent->GetDialogID()))
            {
                //新发起的订阅请求
                tOspTrans.dwIID = GetDefaultOspIID();
            }
            else
            {
                //刷新已存在的订阅请求
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
            }
            
            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_UNSUBSCRIBE_REQ:
        {
            //fanxg-20150715-临时应付一所会话外取消订阅测试---begin
            if (!ExistDlg(pSipEvent->GetDialogID()))
            {
                //新发起的订阅请求
                tOspTrans.dwIID = GetDefaultOspIID();
            }
            else
            {
                //刷新已存在的订阅请求
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
            }

            //tOspTrans = FindDlg(pSipEvent->GetDialogID());
            //fanxg-20150715-临时应付一所会话外取消订阅测试---end

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;

    case KDSIP_EVENT_NOTIFY_REQ:
        {
            tOspTrans = FindDlg(pSipEvent->GetDialogID());

            TOspSipBind tOspSipBind;
            tOspSipBind.tSipId = tSipId;
            tOspSipBind.tOspId = tOspTrans;
            tOspSipBind.eSipEvent = pSipEvent->GetEvent();
            tOspSipBind.strOspEvent = strMsgType;
            InsertIncomeTrans(tOspSipBind, pSipEvent->GetTransID());
        }
        break;
    
    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip msg [%s-%u], won't be processed!!!\n", 
                SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
            bSendToOspApp = false;
        }
        break;
    }

    cMsg.SetOspTransID(tOspTrans);

    return bSendToOspApp;
}

bool COspSipConvertor::ProcSipRsp(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg)
{
    bool bSendToOspApp = true;
    TOspTransID tOspTrans;

    //错误应答处理
    if (pSipEvent->GetResponseCode() != KDSIP_RESPONSE_200_OK)
    {
        /*
        本地发送请求时，对端协议栈或者对端业务层出错，
        返回了出错应答。OspSip根据这些失败Rsp向上层业务层发送OSP_SIP_MSG_PROC_FAIL。

        本地协议栈非发送超时的错误，由kdxxxReq()的返回值即时给出。
        发送超时错误本地协议栈会发送事件KDSIP_EVENT_TRANSACTION_TIMEOUT给ospsip。
       */

        CLASSLOG(OSPSIP, ERROR_LEV, "recv sip event[%s-%d][status code: %d]错误响应, dlgId[%u], transId[%u]\n",
            SipEventDesc(pSipEvent->GetEvent()).c_str(),
            pSipEvent->GetEvent(),
            pSipEvent->GetResponseCode(), 
            pSipEvent->GetDialogID(),
            pSipEvent->GetTransID());

        cMsg.SetOspEventID(OSP_SIP_MSG_PROC_FAIL);
        cMsg.SetSipErrorCode(pSipEvent->GetResponseCode());

        switch (pSipEvent->GetEvent())
        {
        case KDSIP_EVENT_REGISTER_RSP:
        case KDSIP_EVENT_UNREGISTER_RSP:
            {
                if (pSipEvent->GetResponseCode() == KDSIP_RESPONSE_401_UNAUTHORIZED)
                {
                    //国标认证
                    cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
                    cMsg.SetSipRealm(pSipEvent->GetSipHead()->GetWWWAuthenticateRealm());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                }
                else
                {
                    //本地发送建立会话请求，返回的是DlgID，所以用DlgID释放
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                    EraseDlg(pSipEvent->GetDialogID());
                }
            }
            break;

        case KDSIP_EVENT_INVITE_RSP:
        case KDSIP_EVENT_SUBSCRIBE_RSP:
            {
                //本地发送建立会话请求，返回的是DlgID，所以用DlgID释放
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
                EraseDlg(pSipEvent->GetDialogID());
            }
            break;


        case KDSIP_EVENT_INVITE_BYE_RSP:
        case KDSIP_EVENT_UNSUBSCRIBE_RSP:
            {
                //会话已经建立，返回的是TransID，所以用TransID来释放
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
                EraseDlg(pSipEvent->GetDialogID());
            }
            break;

        case KDSIP_EVENT_INFO_RSP:
        case KDSIP_EVENT_OPTIONS_HB_RSP:
        case KDSIP_EVENT_MESSAGE_RSP:
        case KDSIP_EVENT_MESSAGE_DIALOG_RSP:
        case KDSIP_EVENT_NOTIFY_RSP:
            {
                //会话内的事务，不需要释放Dlg
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            }
            break;

        default:
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "unknown rsp sip msg [%s-%u], won't be processed!!!\n", 
                    SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
                bSendToOspApp = false;
            }
            break;
        }
    }
    else
    {
        //正常应答处理
        bool bIsGbMsg = false;
        switch (pSipEvent->GetEvent())
        {
        case KDSIP_EVENT_REGISTER_RSP:
            {
                if (!ExistOutTrans(pSipEvent->GetTransID()))
                {
                    //注册
                    bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());     
                }
                else
                {
                    //刷新注册
                    bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                    tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());           
                }

                if (bIsGbMsg)
                {
                    //国标存在扩展Date头域
                    cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
                    cMsg.SetSipDate(pSipEvent->GetSipHead()->GetDate());
                }
                else
                {
                    cMsg.SetOspEventID(SIP_REG_PROXY_RSP);
                }          
            }
            break;

        case KDSIP_EVENT_UNREGISTER_RSP:
            {
                bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
                cMsg.SetOspEventID(SIP_REG_PROXY_RSP);
            }
            break;

        case KDSIP_EVENT_INVITE_RSP:
            {
                bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                tOspTrans = FindDlg(pSipEvent->GetDialogID());              
            }
            break;

        case KDSIP_EVENT_INVITE_BYE_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
                cMsg.SetOspEventID(OSP_SIP_BYE_RSP);
                EraseDlg(pSipEvent->GetDialogID());            
            }
            break;

        case KDSIP_EVENT_INFO_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            }
            break;

        case KDSIP_EVENT_OPTIONS_HB_RSP:
            {
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
                cMsg.SetOspEventID(SIP_HB_RSP);

                if (tOspTrans.dwIID == INVALID_IID)
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "心跳消息Trans[%u]找不到bind，可能已经超时删除\n", 
                        pSipEvent->GetTransID());
                }
                else
                {
                    //以下代码暂时规避kdsip分配相同transid的问题----->begin
                    if (GETAPP(tOspTrans.dwIID) != OSP_SIP_APP_ID)
                    {
                        CLASSLOG(OSPSIP, ERROR_LEV, "心跳消息Trans[%u]的目的不是ospapp，可能kdsip的事务号错乱，不回调给上层!!!\n",
                            pSipEvent->GetTransID());

                        bSendToOspApp = false;
                    }
                    //end<--------
                }     
            }
            break;

        case KDSIP_EVENT_MESSAGE_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            }
            break;

        case KDSIP_EVENT_MESSAGE_DIALOG_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            }
            break;

        case KDSIP_EVENT_SUBSCRIBE_RSP:
            {                    
                if (!ExistOutTrans(pSipEvent->GetTransID()))
                {
                    //订阅
                    bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                }
                else
                {
                    //更新订阅
                    bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                    tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
                }
            }
            break;

        case KDSIP_EVENT_UNSUBSCRIBE_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
                EraseDlg(pSipEvent->GetDialogID());
            }
            break;

        case KDSIP_EVENT_NOTIFY_RSP:
            {
                bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            }
            break;       

        default:
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip msg [%s-%u], won't be processed!!!\n", 
                    SipEventDesc(pSipEvent->GetEvent()).c_str(), pSipEvent->GetEvent());
                bSendToOspApp = false;
            }
            break;
        }

        //如果是国标消息，设置统一消息号
        if (bIsGbMsg)
        {
            cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
        }
    }  

    cMsg.SetOspTransID(tOspTrans);

    return bSendToOspApp;
}

bool COspSipConvertor::ExistOutTrans(TKDSIP_TRANS_ID tSipTransId)
{
    return m_cOutTransMap.Exist(tSipTransId);
}

TOspTransation COspSipConvertor::FindOspIdByOutTrans(TKDSIP_TRANS_ID tSipTransId)
{
    TOspTransation tOspTrans;

    if (tSipTransId == INVALID_TRANS_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "INVALID_TRANS_ID!!!\n");
        return tOspTrans;
    }

    TOspSipBind tOspSipBind;
    if (!m_cOutTransMap.Find(tSipTransId, tOspSipBind))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]没有映射osp trans\n", tSipTransId);
    }
    else
    {
        m_cOutTransMap.Erase(tSipTransId);
        tOspTrans = tOspSipBind.tOspId;
    }

    return tOspTrans;
}

void COspSipConvertor::InsertOutTrans(const TOspSipBind& tOspSipBind, TKDSIP_TRANS_ID tSipTransId)
{
    if (tSipTransId == INVALID_TRANS_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "INVALID_TRANS_ID!!!\n");
        return;
    }

    if (m_cOutTransMap.Exist(tSipTransId))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]已经映射osp trans\n", tSipTransId);
        return;
    }

    m_cOutTransMap[tSipTransId] = tOspSipBind;
}

void COspSipConvertor::EraseOutTrans(TKDSIP_TRANS_ID tSipTransId)
{
    if (!m_cOutTransMap.Exist(tSipTransId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]不存在osp trans，删除失败\n", tSipTransId);
        return;
    }

    m_cOutTransMap.Erase(tSipTransId);
}

bool COspSipConvertor::ExistIncomeTrans(TKDSIP_TRANS_ID tSipTransId)
{
    return m_cIncomeTransMap.Exist(tSipTransId);
}

bool COspSipConvertor::FindSipIdByIncomeTrans(TKDSIP_TRANS_ID tSipTransId, TSipTransaction& tSipId)
{
    if (tSipTransId == INVALID_TRANS_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "INVALID_TRANS_ID!!!\n");
        return false;
    }

    TOspSipBind tOspSipBind;

    if (m_cIncomeTransMap.Find(tSipTransId, tOspSipBind))
    {
        tSipId = tOspSipBind.tSipId;
        m_cIncomeTransMap.Erase(tSipTransId);
        return true;
    }

    return false;
}

void COspSipConvertor::InsertIncomeTrans(const TOspSipBind& tOspSipBind, TKDSIP_TRANS_ID tSipTransId)
{
    if (tSipTransId == INVALID_TRANS_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "INVALID_TRANS_ID!!!\n");
        return;
    }

    if (m_cIncomeTransMap.Exist(tSipTransId))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]已经映射osp trans\n", tSipTransId);
        return;
    }

    m_cIncomeTransMap[tSipTransId] = tOspSipBind;
}

void COspSipConvertor::EraseIncomeTrans(TKDSIP_TRANS_ID tSipTransId)
{
    if (!m_cIncomeTransMap.Exist(tSipTransId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]不存在osp trans，删除失败\n", tSipTransId);
        return;
    }

    m_cIncomeTransMap.Erase(tSipTransId);
}

bool COspSipConvertor::ExistDlg(TKDSIP_DIALOG_ID tSipDlgId)
{
    return m_cDlgMap.Exist(tSipDlgId);
}

TOspTransation COspSipConvertor::FindDlg(TKDSIP_DIALOG_ID tSipDlgId)
{
    TOspSipBind tBind;
    if (!m_cDlgMap.Find(tSipDlgId, tBind))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]没有映射osp trans\n", tSipDlgId);
    }

    return tBind.tOspId;
}

TOspSipBind COspSipConvertor::FindDlgBind(TKDSIP_DIALOG_ID tSipDlgId)
{
    TOspSipBind tBind;
    if (!m_cDlgMap.Find(tSipDlgId, tBind))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]没有映射osp trans\n", tSipDlgId);
    }

    return tBind;
}

void COspSipConvertor::InsertDlg(const TOspSipBind& tOspSipBind, TKDSIP_DIALOG_ID tSipDlgId)
{
    if (tSipDlgId == INVALID_DIALOG_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "INVALID_DIALOG_ID!!!\n");
        return ;
    }

    if (m_cDlgMap.Exist(tSipDlgId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]已经存在osp trans，插入失败\n", tSipDlgId);
        return;
    }

    m_cDlgMap[tSipDlgId] = tOspSipBind;
}

void COspSipConvertor::EraseDlg(TKDSIP_DIALOG_ID tSipDlgId)
{
    if (!m_cDlgMap.Exist(tSipDlgId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]不存在osp trans，可能已经删除\n", tSipDlgId);
        return;
    }

    m_cDlgMap.Erase(tSipDlgId);
}

bool COspSipConvertor::ExistSN(u32 dwSN)
{
    return m_cSNMap.Exist(dwSN);
}

TOspSipBind COspSipConvertor::FindSNBind(u32 dwSN)
{
    TOspSipBind tBind;
    if (!m_cSNMap.Find(dwSN, tBind))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip SN[%u] no bind\n", dwSN);
    }

    return tBind;
}

void COspSipConvertor::InsertSN(const TOspSipBind& tOspSipBind, u32 dwSN)
{
    if (dwSN == OSP_SIP_INVALID_KDSIP_SN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "OSP_SIP_INVALID_KDSIP_SN!!!\n");
        return ;
    }

    if (m_cSNMap.Exist(dwSN))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip SN[%u]已经存在bind，插入失败\n", dwSN);
        return;
    }

    m_cSNMap[dwSN] = tOspSipBind;
}

void COspSipConvertor::EraseSN(u32 dwSN)
{
    if (!m_cSNMap.Exist(dwSN))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip SN[%u]不存在bind，可能已经删除\n", dwSN);
        return;
    }

    m_cSNMap.Erase(dwSN);
}

TKDSIP_DIALOG_ID COspSipConvertor::GetRegProxyDlg() const
{
    Iterator pPos;
    while (!pPos.End())
    {
        TKDSIP_DIALOG_ID tSipDlg = INVALID_DIALOG_ID;
        TOspSipBind tOspSipBind;

        if (m_cDlgMap.Iterate(pPos, tSipDlg, tOspSipBind))
        {          
            if ((tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_RSP)
                && tOspSipBind.strOspEvent != OspExtEventDesc(OSP_SIP_GB28181_MSG)
                && tOspSipBind.tSipURI.GetUser() == UNIQUE_SIP_UA_PROXY)
            {
                return tSipDlg;
            }         
        }
    }

    return INVALID_DIALOG_ID;
}

bool COspSipConvertor::IsGBOutTrans(TKDSIP_TRANS_ID tSipTransId)
{
    TOspSipBind tOspSipBind;
    if (!m_cOutTransMap.Find(tSipTransId, tOspSipBind))
    {    
        return false;
    }

    if (tOspSipBind.strOspEvent == OspExtEventDesc(OSP_SIP_GB28181_MSG))
    {
        return true;
    }

    return false;
}

bool COspSipConvertor::IsGBIncomeTrans(TKDSIP_TRANS_ID tSipTransId)
{
    TOspSipBind tOspSipBind;
    if (!m_cIncomeTransMap.Find(tSipTransId, tOspSipBind))
    {    
        return false;
    }

    if (tOspSipBind.strOspEvent == OspExtEventDesc(OSP_SIP_GB28181_MSG))
    {
        return true;
    }

    return false;
}

bool COspSipConvertor::IsGBDlg(TKDSIP_DIALOG_ID tSipDlgId)
{
    TOspSipBind tOspSipBind;
    if (!m_cDlgMap.Find(tSipDlgId, tOspSipBind))
    {    
        return false;
    }

    if (tOspSipBind.strOspEvent == OspExtEventDesc(OSP_SIP_GB28181_MSG))
    {
        return true;
    }

    return false;
}

void COspSipConvertor::UpdateRouteInfo(const string& strMsgBuf)
{
    if (strMsgBuf.empty())
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "清空sip路由表\n");
        m_cRouteTable.Empty();
        return;
    }

    CUaRouterNtfReq cReq;
    cReq.FromXml(strMsgBuf);

    if (cReq.GetOpType() != NTF_OP_TYPE_ADD 
        && cReq.GetOpType() != NTF_OP_TYPE_MOD
        && cReq.GetOpType() != NTF_OP_TYPE_DEL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "无效的路由操作类型[%s]\n", cReq.GetOpType().c_str());
        return;
    }

    const CUaRouteTable& tRouteTable = cReq.GetUaRouteTable();
    CUaRouteTable::const_iterator iT = tRouteTable.begin();
    for (; iT != tRouteTable.end(); ++iT)
    {
        TSipURI tUri;
        tUri.SetURIString(iT->first);

        const TUaAddrPair& tAddrPair = iT->second;

        CLASSLOG(OSPSIP, CRITICAL_LEV, "路由from[%s-%s:%d]--->to[%s-%s:%d],opType[%s]\n", 
            g_tOspSipConf.tLocalURI.GetURIString().c_str(), 
            tAddrPair.GetFromAddr().GetNetIp().c_str(),
            tAddrPair.GetFromAddr().GetNetPort(),
            tUri.GetURIString().c_str(),
            tAddrPair.GetToAddr().GetNetIp().c_str(),
            tAddrPair.GetToAddr().GetNetPort(),
            cReq.GetOpType().c_str());

        if (cReq.GetOpType() == NTF_OP_TYPE_ADD || cReq.GetOpType() == NTF_OP_TYPE_MOD)
        {                 
            m_cRouteTable[tUri] = tAddrPair;
        }
        else
        {
            m_cRouteTable.Erase(tUri);
        }
    }
}


void COspSipConvertor::SetDefaultOspIID(u32 dwOspIID)
{
    CAutoLock cLock(m_cSemLock);
    m_dwDefaultOspIID = dwOspIID;
}

u32 COspSipConvertor::GetDefaultOspIID() const
{
    CAutoLock cLock(m_cSemLock);
    return m_dwDefaultOspIID;
}

void COspSipConvertor::CheckTimeout()
{
    //sip资源创建超时
    CheckSipCreateTimeout();

    //发出的事务kdsip底层会自动删除事务
    CheckTransTimeout(m_cOutTransMap);

    //收到的事务，如果上层一直没有发送应答，
    //那么由ospsip层发送超时终了应答，以便kdsip删除该事务
    CheckTransTimeout(m_cIncomeTransMap, true); 

    CheckDlgTimeout();
}

void COspSipConvertor::CheckTransTimeout(COspSipTransMap& cTransMap, bool bIsTimeoutRsp)
{
    Iterator pPos;
    while (!pPos.End())
    {
        TKDSIP_TRANS_ID tSipTrans;
        TOspSipBind tOspSipBind;

        if (cTransMap.Iterate(pPos, tSipTrans, tOspSipBind))
        {    
            if (tOspSipBind.IsTimeout())
            {                          
                TOspTransation tOspTrans;
                tOspTrans = tOspSipBind.tOspId;

                COspTime tBirthTime(tOspSipBind.tBirthTime);
                CLASSLOG(OSPSIP, ERROR_LEV,"删除超时事务[%s-%d][%s]trans[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                    SipEventDesc(tOspSipBind.eSipEvent).c_str(), (s32)tOspSipBind.eSipEvent, tOspSipBind.strOspEvent.c_str(),
                    tSipTrans, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                    tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());

                if (bIsTimeoutRsp)
                {
                    if (IsSipReqMsg(tOspSipBind.eSipEvent) && tSipTrans != INVALID_TRANS_ID)
                    {                       
                        TOspTransID tOspTrasnID;
                        tOspTrasnID.dwIID = OSP_SIP_DAEMON;
                        string strEmpty;

                        EKDSIP_EVENT eSipEventRsp = EKDSIP_EVENT(tOspSipBind.eSipEvent + 1);
                        OspSipPostRsp_Inner(eSipEventRsp, strEmpty,
                            tSipTrans, tOspTrasnID, KDSIP_RESPONSE_408_REQ_TIMEOUT);

                        CLASSLOG(OSPSIP, ERROR_LEV, "income事务[%s]超时，自动应答408\n", 
                            tOspSipBind.strOspEvent.c_str());
                    }
                }

                cTransMap.Erase(tSipTrans);
            }
        }
    }    
}

void COspSipConvertor::CheckSipCreateTimeout()
{
    Iterator pPos;
    while (!pPos.End())
    {
        u32 dwSN = OSP_SIP_INVALID_KDSIP_SN;
        TOspSipBind tOspSipBind;

        if (m_cSNMap.Iterate(pPos, dwSN, tOspSipBind))
        {    
            if (tOspSipBind.IsCreateTimeout())
            {                          
                TOspTransation tOspTrans;
                tOspTrans = tOspSipBind.tOspId;

                COspTime tBirthTime(tOspSipBind.tBirthTime);
                CLASSLOG(OSPSIP, ERROR_LEV,"删除创建超时事务[%s-%d][%s]SN[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                    SipEventDesc(tOspSipBind.eSipEvent).c_str(), (s32)tOspSipBind.eSipEvent, tOspSipBind.strOspEvent.c_str(),
                    dwSN, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                    tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());
                
				if (KDSIP_EVENT_INVITE_BYE_REQ==tOspSipBind.eSipEvent)
				{
					CLASSLOG(OSPSIP, ERROR_LEV,"[%s-%d]事务创建超时,删除对应Dialog[%p]\n", 
						SipEventDesc(tOspSipBind.eSipEvent).c_str(), (s32)tOspSipBind.eSipEvent, tOspSipBind.tSipId.tSipDlg);
					EraseDlg(tOspSipBind.tSipId.tSipDlg);
				}

                m_cSNMap.Erase(dwSN);
            }
        }
    }    
}

void COspSipConvertor::CheckDlgTimeout()
{

}

void COspSipConvertor::PrintTrans(u32 dwPrintFlag) const
{  
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    if ((dwPrintFlag&PRINT_OUT_TRANS) == PRINT_OUT_TRANS)
    {
        OspPrintf(TRUE, FALSE, "out事务绑定信息：[%u]个\n", m_cOutTransMap.GetSize());    
        u32 i = 0;
        Iterator pPos;
        while (!pPos.End())
        {
            TKDSIP_TRANS_ID tSipTrans;
            TOspSipBind tOspSipBind;

            if (m_cOutTransMap.Iterate(pPos, tSipTrans, tOspSipBind))
            {    
                TOspTransation tOspTrans;
                tOspTrans = tOspSipBind.tOspId;

                TSipTransaction tSipId;
                tSipId = tOspSipBind.tSipId;

                COspTime tBirthTime(tOspSipBind.tBirthTime);
                i++;
                OspPrintf(TRUE, FALSE, "  %4u.[%s][%s]:sip dlg[%u]-trans[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                    i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), tOspSipBind.strOspEvent.c_str(),
                    tSipId.tSipDlg, tSipTrans, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                    tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());
            }
        }    
        OspPrintf(TRUE, FALSE, "\nout事务绑定信息：[%u]个\n", m_cOutTransMap.GetSize());
    }
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    if ((dwPrintFlag&PRINT_IN_TRANS) == PRINT_IN_TRANS)
    {
        OspPrintf(TRUE, FALSE, "income事务信息：[%u]个\n", m_cIncomeTransMap.GetSize());    
        u32 i = 0;
        Iterator pPos;
        while (!pPos.End())
        {
            TKDSIP_TRANS_ID tSipTrans;
            TOspSipBind tOspSipBind;

            if (m_cIncomeTransMap.Iterate(pPos, tSipTrans, tOspSipBind))
            {    
                TOspTransation tOspTrans;
                tOspTrans = tOspSipBind.tOspId;

                TSipTransaction tSipId;
                tSipId = tOspSipBind.tSipId;

                COspTime tBirthTime(tOspSipBind.tBirthTime);
                i++;
                OspPrintf(TRUE, FALSE, "  %4u.[%s][%s]:sip dlg[%u]-trans[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                    i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), tOspSipBind.strOspEvent.c_str(),
                    tSipId.tSipDlg, tSipTrans, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                    tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());
            }
        }    
        OspPrintf(TRUE, FALSE, "\nincome事务信息：[%u]个\n", m_cIncomeTransMap.GetSize());
    }
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintDialog(u32 dwPrintFlag) const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    OspPrintf(TRUE, FALSE, "所有对话绑定信息：[%u]个\n", m_cDlgMap.GetSize());    
    u32 i = 0;
    u32 dwIvtNum = 0;
    u32 dwSsNum = 0;
    u32 dwRegNum = 0;
    Iterator pPos;
    pPos.SetToBegin();
    while (!pPos.End())
    {
        TKDSIP_DIALOG_ID tSipDlg;
        TOspSipBind tOspSipBind;

        if (m_cDlgMap.Iterate(pPos, tSipDlg, tOspSipBind))
        {
            if ((tOspSipBind.eSipEvent == KDSIP_EVENT_INVITE_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_INVITE_RSP))
            {
                dwIvtNum++;
                if ((dwPrintFlag&PRINT_IVT_DLG) == PRINT_IVT_DLG)
                {
                    i++;
                    TOspTransation& tOspTrans = tOspSipBind.tOspId;
                    COspTime tBirthTime(tOspSipBind.tBirthTime);
                    string& strOspEvent = tOspSipBind.strOspEvent;
                    TSipURI& tSipUri = tOspSipBind.tSipURI;
                    OspPrintf(TRUE, FALSE, "  %4u. [%s]sipDlg[%u] sipUri[%s]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)-event(%s)]---birth[%s]\n", 
                        i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), 
                        tSipDlg, tSipUri.GetURIString().c_str(), GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                        tOspTrans.dwTaskID, tOspTrans.dwSeqNO, strOspEvent.c_str(), tBirthTime.GetString().c_str());
                }

                continue;
            }

            if ((tOspSipBind.eSipEvent == KDSIP_EVENT_SUBSCRIBE_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_SUBSCRIBE_RSP))
            {
                dwSsNum++;
                if ((dwPrintFlag&PRINT_SS_DLG) == PRINT_SS_DLG)
                {
                    i++;
                    TOspTransation& tOspTrans = tOspSipBind.tOspId;
                    COspTime tBirthTime(tOspSipBind.tBirthTime);
                    string& strOspEvent = tOspSipBind.strOspEvent;
                    TSipURI& tSipUri = tOspSipBind.tSipURI;
                    OspPrintf(TRUE, FALSE, "  %4u. [%s]sipDlg[%u] sipUri[%s]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)-event(%s)]---birth[%s]\n", 
                        i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), 
                        tSipDlg, tSipUri.GetURIString().c_str(), GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                        tOspTrans.dwTaskID, tOspTrans.dwSeqNO, strOspEvent.c_str(), tBirthTime.GetString().c_str());
                }

                continue;
            }

            if ((tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_RSP))
            {
                dwRegNum++;
                if ((dwPrintFlag&PRINT_REG_DLG) == PRINT_REG_DLG)
                {
                    i++;
                    TOspTransation& tOspTrans = tOspSipBind.tOspId;
                    COspTime tBirthTime(tOspSipBind.tBirthTime);
                    string& strOspEvent = tOspSipBind.strOspEvent;
                    TSipURI& tSipUri = tOspSipBind.tSipURI;
                    OspPrintf(TRUE, FALSE, "  %4u. [%s]sipDlg[%u] sipUri[%s]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)-event(%s)]---birth[%s]\n", 
                        i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), 
                        tSipDlg, tSipUri.GetURIString().c_str(), GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                        tOspTrans.dwTaskID, tOspTrans.dwSeqNO, strOspEvent.c_str(), tBirthTime.GetString().c_str());
                }

                continue;
            }         
        }
    }    
    OspPrintf(TRUE, FALSE, "\n对话绑定信息：Total[%u]个, Ivt[%u], Ss[%u], Reg[%u]\n", 
        m_cDlgMap.GetSize(), dwIvtNum, dwSsNum, dwRegNum);

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintSN() const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    OspPrintf(TRUE, FALSE, "SN创建信息：[%u]个\n", m_cSNMap.GetSize());    
    u32 i = 0;
    Iterator pPos;
    while (!pPos.End())
    {
        u32 dwSN = OSP_SIP_INVALID_KDSIP_SN;
        TOspSipBind tOspSipBind;

        if (m_cSNMap.Iterate(pPos, dwSN, tOspSipBind))
        {    
            TOspTransation tOspTrans;
            tOspTrans = tOspSipBind.tOspId;

            COspTime tBirthTime(tOspSipBind.tBirthTime);
            i++;
            OspPrintf(TRUE, FALSE, "  %4u.[%s][%s]:sip SN[%lu]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                i, SipEventDesc(tOspSipBind.eSipEvent).c_str(), tOspSipBind.strOspEvent.c_str(),
                dwSN, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());
        }
    }    
    OspPrintf(TRUE, FALSE, "SN创建信息：[%u]个\n", m_cSNMap.GetSize()); 

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintRoute(const char* szUaName) const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    if (szUaName != NULL)
    {
        //打印指定UA的路由信息
        TSipURI tUri;
        tUri.SetUser(szUaName);
        tUri.SetDomain(g_tOspSipConf.tLocalURI.GetDomain());
        TUaAddrPair tAddrPair;
        if (m_cRouteTable.Find(tUri, tAddrPair))
        {
            OspPrintf(TRUE, FALSE, "  %4u.[%s-%s:%d]--->[%s-%s:%d]\n", 
                1, g_tOspSipConf.tLocalURI.GetURIString().c_str(), 
                tAddrPair.GetFromAddr().GetNetIp().c_str(),
                tAddrPair.GetFromAddr().GetNetPort(),
                tUri.GetURIString().c_str(),
                tAddrPair.GetToAddr().GetNetIp().c_str(),
                tAddrPair.GetToAddr().GetNetPort());
        }
        else
        {
            OspPrintf(TRUE, FALSE, "  [%s]不存在路由信息\n", 
                tUri.GetURIString().c_str());
        }      
    }
    else
    {
        OspPrintf(TRUE, FALSE, "路由表信息：[%u]个\n", m_cRouteTable.GetSize());

        //打印所有UA的路由信息
        u32 i = 0;
        Iterator pPos;
        while (!pPos.End())
        {
            TSipURI tUri;
            TUaAddrPair tAddrPair;

            if (m_cRouteTable.Iterate(pPos, tUri, tAddrPair))
            {    
                i++;
                OspPrintf(TRUE, FALSE, "  %4u.[%s-%s:%d]--->[%s-%s:%d]\n", 
                    i, g_tOspSipConf.tLocalURI.GetURIString().c_str(), 
                    tAddrPair.GetFromAddr().GetNetIp().c_str(),
                    tAddrPair.GetFromAddr().GetNetPort(),
                    tUri.GetURIString().c_str(),
                    tAddrPair.GetToAddr().GetNetIp().c_str(),
                    tAddrPair.GetToAddr().GetNetPort());
            }
        }

        OspPrintf(TRUE, FALSE, "路由表信息：[%u]个\n", m_cRouteTable.GetSize()); 
    }  

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
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
bool COspSipConvertor::OspSipPostReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, const string& strDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo)
{
    if (!IsSipReqMsg(eSipEvent))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] is not a sip request without dialog\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }

    if (!tDstURI.IsValidURI())
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "Invalid dst sip uri[%s], sip event[%s-%d]\n", 
            tDstURI.GetURIString().c_str(), SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }

    u8 byLogLev = EVENT_LEV;
    if (eSipEvent == KDSIP_EVENT_OPTIONS_HB_REQ)
    {
        byLogLev = TIMER_LEV;
    }

    CKDSipMsgHead tSipHead;

    char szTo[MAX_SIP_URI_LEN+1] = {0};
    sprintf(szTo, "sip:%s", tDstURI.GetNOString());

    char szFrom[MAX_SIP_URI_LEN+1] = {0};
    char szProxy[MAX_SIP_URI_LEN+1] = {0};
    if (!strDirectUAAddr.empty())
    {
        //UA和UA之间直接通信，这是上层直接指定对端地址，和U2U是两个机制
        CLASSLOG(OSPSIP, byLogLev, "sip event[%s-%d] directly send to UA[%s] without proxy\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent, strDirectUAAddr.c_str());

        sprintf(szProxy, "<sip:%s;lr>", strDirectUAAddr.c_str());

        if (eSipEvent == KDSIP_EVENT_INVITE_REQ || eSipEvent == KDSIP_EVENT_SUBSCRIBE_REQ)
        {
            //创建对话时，为了能使对端能正确发送对话内方法，这里需要将from的domain设置成user@LocalIp:port形式
            TSipURI tFromUri;  //user@LocalIp:port

            if (ptSipExtraInfo != NULL)
            {
				//国标
				if ( !ptSipExtraInfo->strFromUri.empty() )
				{//如果上层指定了From，则使用业务指定的From URI
					tFromUri.SetNOString(ptSipExtraInfo->strFromUri);
				}
				else
				{
                    // 20150724 宇视平台作为下级时出有问题，报我们From和To格式不一样；与HK，DH是没问题的。
					//tFromUri.SetUser(g_tOspSipConf.tLocalGbURI.GetUser());  
					//tFromUri.SetDomain(g_tOspSipConf.tLocalGbAddr.GetSipAddrStr()); 
                    tFromUri = g_tOspSipConf.tLocalGbURI;
				}
            }
            else
            {
                //非国标
                tFromUri.SetUser(g_tOspSipConf.tLocalURI.GetUser());
                tFromUri.SetDomain(g_tOspSipConf.tLocalAddr.GetSipAddrStr());
            }
            
            sprintf(szFrom, "sip:%s", tFromUri.GetNOString());
        }
        else
        {
            if (ptSipExtraInfo != NULL)
            {
                //国标
				if (ptSipExtraInfo->strFromUri.empty())
				{
					sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalGbURI.GetNOString());
				}
				else
				{
					sprintf(szFrom, "sip:%s", ptSipExtraInfo->strFromUri.c_str());
				}
                
            }
            else
            {
                //非国标
                sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());
            }        
        }
    }
    else if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
    {
        if (tDstURI.GetDomain() != OspSipGetLocalURI().GetDomain()) //liangli 建议先判断tDstURI是否proxy
        {
            //如果是发向异域的消息，直接投递给proxy
            sprintf(szProxy, "<sip:%s:%d;lr>", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), g_tOspSipConf.tProxyAddr.wPort);
            sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());
        }
        else
        {
            TUaAddrPair* ptDstRoute = m_cRouteTable.Find(tDstURI);
            if (ptDstRoute == NULL)
            {
               /*
               本域消息，但根据dstUri无法找到dstAddr，那也把消息先投递到proxy。
               (可能dstUA刚注册到proxy，proxy还没能发布，那这条消息proxy是可以中转的；
               也可能dstUA根本还没有向proxy注册，那这时proxy可以直接回404。)
               */
                sprintf(szProxy, "<sip:%s:%d;lr>", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), g_tOspSipConf.tProxyAddr.wPort);
                sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());

                //UA向proxy订阅路由
                if (tDstURI.GetUser() == UNIQUE_SIP_UA_PROXY && eSipEvent == KDSIP_EVENT_SUBSCRIBE_REQ)
                {
                    char szContact[MAX_SIP_URI_LEN+1] = {0};
                    sprintf(szContact, "sip:%s:%d", g_tOspSipConf.tLocalAddr.tIpAddr.c_str(), 
                        g_tOspSipConf.tLocalAddr.wPort);
                    tSipHead.SetContactURI(szContact);

                    CLASSLOG(OSPSIP, byLogLev, "route subscribe event[%s-%d] send to proxy[%s] with contact[%s]\n", 
                        SipEventDesc(eSipEvent).c_str(), eSipEvent, tDstURI.GetURIString().c_str(), szContact);
                }            
            }
            else
            {
                //本域消息，并且根据dstUri找到dstAddr，那就将消息投递到dstAddr。
                TSipAddr tDstAddr;
                tDstAddr.tIpAddr = ptDstRoute->GetToAddr().GetNetIp();
                tDstAddr.wPort = (u16)ptDstRoute->GetToAddr().GetNetPort();

                TSipAddr tSrcAddr;
                tSrcAddr.tIpAddr = ptDstRoute->GetFromAddr().GetNetIp();
                tSrcAddr.wPort = (u16)ptDstRoute->GetFromAddr().GetNetPort();
                
                CLASSLOG(OSPSIP, byLogLev, "sip event[%s-%d] directly send from [%s] to UA[%s(%s)] with U2U\n", 
                    SipEventDesc(eSipEvent).c_str(), eSipEvent, tSrcAddr.GetSipAddrStr().c_str(),
                    tDstURI.GetURIString().c_str(), tDstAddr.GetSipAddrStr().c_str());

                sprintf(szProxy, "<sip:%s;lr>", tDstAddr.GetSipAddrStr().c_str());
                sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());

                //U2U需要填写contact字段，保证对端的会话内请求能发过来
                char szContact[MAX_SIP_URI_LEN+1] = {0};
                sprintf(szContact, "sip:%s:%d", tSrcAddr.tIpAddr.c_str(), tSrcAddr.wPort);
                tSipHead.SetContactURI(szContact);
            }
        }       
    }
    else
    {
        //UA和UA之间通过proxy通信
        sprintf(szProxy, "<sip:%s:%d;lr>", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), g_tOspSipConf.tProxyAddr.wPort);
        sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());
    }
    
    tSipHead.SetToURI(szTo);
    tSipHead.SetFromURI(szFrom);
    tSipHead.SetProxyURI(szProxy);

    CKDSipMsgBody* ptSipBody = NULL;
    CKDSipMsgBody tSipBody;
    if (strMsgBuf.length() != 0)
    {
        u8* pBuf = const_cast<u8*>(reinterpret_cast<const u8*>(strMsgBuf.c_str()));  //pclint:1773
        tSipBody.SetMsgBodyContent(pBuf, s32(strMsgBuf.length()));
        if (ptSipExtraInfo == NULL)
        {
            tSipBody.SetContentType(KDSIP_CONTENT_XML);
        }
        else
        {
            tSipBody.SetContentType(ptSipExtraInfo->eSipContentType);
        }
        
        ptSipBody = &tSipBody;
    }

    KDSIP_RETURN tKdsipRet = KDSIP_FAIL;
    u32 dwKdsipSN = OSP_SIP_INVALID_KDSIP_SN;

    string strOspEvent;
    if (tSipBody.GetContentType() == KDSIP_CONTENT_XML)
    {
        if (!strMsgBuf.empty())
        {
            strOspEvent = g_pCBGetSipXmlMsgType(strMsgBuf.c_str());

            if (strOspEvent.empty())
            {
                strOspEvent = "unknown osp event";
            }
        }
        else
        {
            strOspEvent = "NullSipBody";
        }      
    }
    else
    {
        if (ptSipExtraInfo != NULL)
        {
            strOspEvent = OspExtEventDesc(OSP_SIP_GB28181_MSG);
        }
        else
        {
            strOspEvent = "NullSipBody";
        } 
    }
    
    TOspSipBind tOspSipBind;
    tOspSipBind.tOspId = tSrcOspTrans;
    tOspSipBind.tSipURI = tDstURI;
    tOspSipBind.eSipEvent = eSipEvent;
    tOspSipBind.strOspEvent = strOspEvent;

    switch (eSipEvent)
    {
    case KDSIP_EVENT_REGISTER_REQ:
        {
            if (ptSipExtraInfo == NULL)
            {
                //平台2.0注册proxy
                TKDSIP_DIALOG_ID tRegProxyID = GetRegProxyDlg();
                if (tRegProxyID != INVALID_DIALOG_ID)
                {
                    //刷新注册
                    tKdsipRet = KDSipRegisterRefreshReq(tRegProxyID, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                }
                else
                {
                    //新建注册
                    char szContact[MAX_SIP_URI_LEN+1] = {0};
                    sprintf(szContact, "sip:%s:%d", g_tOspSipConf.tLocalAddr.tIpAddr.c_str(), 
                        g_tOspSipConf.tLocalAddr.wPort);

                    char szRegistrar[MAX_SIP_URI_LEN+1] = {0};
                    sprintf(szRegistrar, "sip:%s:%d", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), 
                        g_tOspSipConf.tProxyAddr.wPort);

                    tSipHead.SetProxyURI(szRegistrar);
                    tSipHead.SetToURI(szFrom);
                    tSipHead.SetContactURI(szContact);

                    tKdsipRet = KDSipRegisterReq(tSipHead, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                    if (tKdsipRet != KDSIP_OK)
                    {
                        CLASSLOG(OSPSIP, ERROR_LEV, "KDSipRegisterReq is fail! errcode[%d]\n", (int)tKdsipRet);

                        CLASSLOG(OSPSIP, ERROR_LEV, "Register param[from:%s]-[to:%s]!\n",
                            tSipHead.GetFromURI(), tSipHead.GetToURI());
                        CLASSLOG(OSPSIP, ERROR_LEV, "Register param[proxy:%s]-[contact:%s]-[Expires:%d]!\n",
                            tSipHead.GetProxyURI(), tSipHead.GetContactURI(), g_nRegProxyExpires);

                    }
                    else
                    {
                        //tOspSipBind.tSipId.tSipDlg = tDlgId;
                        //InsertDlg(tOspSipBind, tDlgId);
                    }
                }    
            }
            else
            {
                //国标上级注册逻辑
                char szRegistrar[MAX_SIP_URI_LEN+1] = {0};
                sprintf(szRegistrar, "sip:%s@%s", tDstURI.GetUser().c_str(), strDirectUAAddr.c_str());

                tSipHead.SetProxyURI(szRegistrar);
                tSipHead.SetToURI(szFrom);
				SetContactUri(ptSipExtraInfo, tSipHead);

                tKdsipRet = KDSipRegisterReq(tSipHead, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                if (tKdsipRet != KDSIP_OK)
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "KDSipRegisterReq is fail! errcode[%d]\n", (int)tKdsipRet);

                    CLASSLOG(OSPSIP, ERROR_LEV, "Register param[from:%s]-[to:%s]!\n",
                        tSipHead.GetFromURI(), tSipHead.GetToURI());
                    CLASSLOG(OSPSIP, ERROR_LEV, "Register param[proxy:%s]-[contact:%s]-[Expires:%d]!\n",
                        tSipHead.GetProxyURI(), tSipHead.GetContactURI(), g_nRegProxyExpires);
                }                  
            }              
        }
        break;

    case KDSIP_EVENT_UNREGISTER_REQ:
        {
			if (ptSipExtraInfo == NULL)
			{
				//平台2.0注册proxy
				TKDSIP_DIALOG_ID tRegProxyID = GetRegProxyDlg();
				if (tRegProxyID != INVALID_DIALOG_ID)
				{
					//取消注册
					char szContact[MAX_SIP_URI_LEN+1] = {0};
					sprintf(szContact, "sip:%s:%d", g_tOspSipConf.tLocalAddr.tIpAddr.c_str(), 
						g_tOspSipConf.tLocalAddr.wPort);

					char szRegistrar[MAX_SIP_URI_LEN+1] = {0};
					sprintf(szRegistrar, "sip:%s:%d", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), 
						g_tOspSipConf.tProxyAddr.wPort);

					tSipHead.SetProxyURI(szRegistrar);
					tSipHead.SetToURI(szFrom);
					tSipHead.SetContactURI(szContact);

					tKdsipRet = KDSipRegisterReq(tSipHead, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
					if (tKdsipRet != KDSIP_OK)
					{
						CLASSLOG(OSPSIP, ERROR_LEV, "KDSipRegisterReq is fail! errcode[%d]\n", (int)tKdsipRet);

						CLASSLOG(OSPSIP, ERROR_LEV, "Register param[from:%s]-[to:%s]!\n",
							tSipHead.GetFromURI(), tSipHead.GetToURI());
						CLASSLOG(OSPSIP, ERROR_LEV, "Register param[proxy:%s]-[contact:%s]-[Expires:%d]!\n",
							tSipHead.GetProxyURI(), tSipHead.GetContactURI(), g_nRegProxyExpires);

					}
					else
					{
						//tOspSipBind.tSipId.tSipDlg = tDlgId;
						//InsertDlg(tOspSipBind, tDlgId);
					}
				}
				else
				{
					
				}    
			}
			else
			{
				//国标上级注册逻辑
				char szRegistrar[MAX_SIP_URI_LEN+1] = {0};
				sprintf(szRegistrar, "sip:%s@%s", tDstURI.GetUser().c_str(), strDirectUAAddr.c_str());

				tSipHead.SetProxyURI(szRegistrar);
				tSipHead.SetToURI(szFrom);
				SetContactUri(ptSipExtraInfo, tSipHead);

				tKdsipRet = KDSipRegisterReq(tSipHead, 0, ptSipBody, dwKdsipSN);
				if (tKdsipRet != KDSIP_OK)
				{
					CLASSLOG(OSPSIP, ERROR_LEV, "KDSipRegisterReq is fail! errcode[%d]\n", (int)tKdsipRet);

					CLASSLOG(OSPSIP, ERROR_LEV, "Register param[from:%s]-[to:%s]!\n",
						tSipHead.GetFromURI(), tSipHead.GetToURI());
					CLASSLOG(OSPSIP, ERROR_LEV, "Register param[proxy:%s]-[contact:%s]-[Expires:%d]!\n",
						tSipHead.GetProxyURI(), tSipHead.GetContactURI(), g_nRegProxyExpires);
				}                  
			}

        }
        break;

    case KDSIP_EVENT_INVITE_REQ:
        {
			if (ptSipExtraInfo != NULL)
			{
				tSipHead.SetSubject(ptSipExtraInfo->strSubject.c_str());
                CLASSLOG(OSPSIP, EVENT_LEV, "Subject[%s]\n", tSipHead.GetSubject());
			}

			if (ptSipExtraInfo != NULL)
			{
				// 修改contact字段，防止gbs会话内bye和其他消息无法发送 [9/8/2015 pangubing]
				SetContactUri(ptSipExtraInfo, tSipHead);
			}
			else
			{
				//todo,其他模块使用本版本ospsip发起订阅前，需先测试这个分支生成的contact字段是否有效
				char szContact[MAX_SIP_URI_LEN+1] = {0};
				sprintf(szContact, "sip:%s@%s",g_tOspSipConf.tLocalURI.GetUser().c_str(),g_tOspSipConf.tLocalAddr.GetSipAddrStr().c_str());
				tSipHead.SetContactURI(szContact);
			}

            tKdsipRet = KDSipInviteReq(tSipHead, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_OPTIONS_HB_REQ:
        {
            tKdsipRet = KDSipOptionsReq(tSipHead, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_MESSAGE_REQ:
        {
            tKdsipRet = KDSipMessageReq(tSipHead, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_SUBSCRIBE_REQ:
        {
			if (ptSipExtraInfo != NULL)
			{
				// 修改国标contact字段
				SetContactUri(ptSipExtraInfo, tSipHead);
				if(ptSipExtraInfo->dwRegExpire != 0) //GB订阅借用此字段把刷新订阅是否要消息体的信息从业务层带到ospsip
				{
					tKdsipRet = KDSipSubscribeReq(tSipHead, ptSipBody, dwKdsipSN, KDSIP_TRANSPORT_UDP, DEFAULT_EXPIRES, TRUE);
					break;
				}
			}
			else
			{
				//todo,其他模块使用本版本ospsip发起订阅前，需先测试这个分支生成的contact字段是否有效
				char szContact[MAX_SIP_URI_LEN+1] = {0};
				sprintf(szContact, "sip:%s@%s",g_tOspSipConf.tLocalURI.GetUser().c_str(),g_tOspSipConf.tLocalAddr.GetSipAddrStr().c_str());
				tSipHead.SetContactURI(szContact);
			}
			
            tKdsipRet = KDSipSubscribeReq(tSipHead, ptSipBody, dwKdsipSN);
        }
        break;
    case KDSIP_EVENT_UNSUBSCRIBE_REQ: // hzytodo 20150715 临时用于发送会话外取消订阅请求
        {
            tKdsipRet = KDSipSubscribeReq(tSipHead, ptSipBody, dwKdsipSN, KDSIP_TRANSPORT_UDP, 0); 
        }
        break;
    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip event[%s-%d]\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        }
        break;
    }

    if (tKdsipRet == KDSIP_FAIL
        || dwKdsipSN == OSP_SIP_INVALID_KDSIP_SN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] send to dst[%s] fail[KdsipRet = %d, dwKdsipSN = %u], event size[%u]bytes, osp event[%s], from[%s], to[%s]\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent, tDstURI.GetURIString().c_str(), 
            (int)tKdsipRet, dwKdsipSN, strMsgBuf.size(), strOspEvent.c_str(), szFrom, szTo);
        return false;
    }

    tOspSipBind.dwKdsipSN = dwKdsipSN;
    InsertSN(tOspSipBind, dwKdsipSN);

    CLASSLOG(OSPSIP, byLogLev, "send sip event[%s-%d] to dst[%s] success[dwKdsipSN = %u], event size[%u]bytes, osp event[%s], from[%s], to[%s]\n", 
        SipEventDesc(eSipEvent).c_str(), eSipEvent, tDstURI.GetURIString().c_str(), 
        dwKdsipSN, strMsgBuf.size(), strOspEvent.c_str(), szFrom, szTo);

    return true;
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
bool COspSipConvertor::OspSipPostInDlgReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, 
    const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo)
{
    if (tDstDlgId == INVALID_DIALOG_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] dialog id is invlid\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }

    if (!IsSipReqMsg(eSipEvent))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] is not a sip request with dialog\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }    

    CKDSipMsgBody* ptSipBody = NULL;
    CKDSipMsgBody tSipBody;
    if (strMsgBuf.length() != 0)
    {
        u8* pBuf = const_cast<u8*>(reinterpret_cast<const u8*>(strMsgBuf.c_str()));  //pclint:1773
        tSipBody.SetMsgBodyContent(pBuf, s32(strMsgBuf.length()));

        if (ptSipExtraInfo == NULL)
        {
            tSipBody.SetContentType(KDSIP_CONTENT_XML);
        }
        else
        {
            tSipBody.SetContentType(ptSipExtraInfo->eSipContentType);
        }
        
        ptSipBody = &tSipBody;
    }

    string strOspEvent;
    if (tSipBody.GetContentType() == KDSIP_CONTENT_XML)
    {
        if (!strMsgBuf.empty())
        {
            strOspEvent = g_pCBGetSipXmlMsgType(strMsgBuf.c_str());

            if (strOspEvent.empty())
            {
                strOspEvent = "unknown osp event";
            }
        }
        else
        {
            strOspEvent = "NullSipBody";
        }      
    }
    else
    {
        if (ptSipExtraInfo != NULL)
        {
            strOspEvent = OspExtEventDesc(OSP_SIP_GB28181_MSG);
        }
        else
        {
            strOspEvent = "NullSipBody";
        } 
    }

    KDSIP_RETURN tKdsipRet = KDSIP_FAIL;
    u32 dwKdsipSN = OSP_SIP_INVALID_KDSIP_SN;

    switch (eSipEvent)
    {
    case KDSIP_EVENT_REGISTER_REQ:
    case KDSIP_EVENT_UNREGISTER_REQ:
        {
            //国标上级注册认证请求
            if (ptSipExtraInfo == NULL)
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "ptSipExtraInfo == NULL, Auth[%s-%d]\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
                return false;
            }
            else
            {
                char szContact[MAX_SIP_URI_LEN+1] = {0};
                sprintf(szContact, "sip:%s:%d", g_tOspSipConf.tLocalAddr.tIpAddr.c_str(), 
                    g_tOspSipConf.tLocalAddr.wPort);

                if (!ptSipExtraInfo->strUserName.empty() && !ptSipExtraInfo->strPassword.empty())
                {
                    //注册认证
                    CKDSipMsgHead cSipHead;
                    cSipHead.SetAuthorizationInfo(ptSipExtraInfo->strUserName.c_str(), 
                        ptSipExtraInfo->strPassword.c_str());

                    tKdsipRet = KDSipRegisterReqWithHeader(tDstDlgId, ptSipExtraInfo->dwRegExpire, &cSipHead, NULL, dwKdsipSN);
                }
                else
                {
                    //刷新注册
                    tKdsipRet = KDSipRegisterRefreshReq(tDstDlgId, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                }
            }              
        }
        break;

    case KDSIP_EVENT_INVITE_ACK:
        {
            //ACK无应答，需要特殊处理
            if (KDSIP_OK != KDSipInviteACK(tDstDlgId, ptSipBody))
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] send fail\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
                return false;
            }
            else
            {
                CLASSLOG(OSPSIP, EVENT_LEV, "send sip event[%s-%d] to dialog[%p] success\n", 
                    SipEventDesc(eSipEvent).c_str(), eSipEvent, tDstDlgId);
                return true;
            }
        }
        break;

    case KDSIP_EVENT_INVITE_BYE_REQ:
        {
            tKdsipRet = KDSipByeReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_INFO_REQ:
        {
            tKdsipRet = KDSipInfoReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_MESSAGE_DIALOG_REQ:
        {
            tKdsipRet = KDSipMessageInDlgReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_SUBSCRIBE_REQ:
        {
            //订阅刷新,重用先前的对话ID，需要特殊处理
            tKdsipRet = KDSipSubscribeRefreshReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_UNSUBSCRIBE_REQ:
        {
            tKdsipRet = KDSipUnSubscribeReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    case KDSIP_EVENT_NOTIFY_REQ:
        {
            tKdsipRet = KDSipNotifyReq(tDstDlgId, ptSipBody, dwKdsipSN);
        }
        break;

    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip event[%s-%d]\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        }
        break;
    }

    if (tKdsipRet == KDSIP_FAIL
        || dwKdsipSN == OSP_SIP_INVALID_KDSIP_SN)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] to dialog[%p] send fail[KdsipRet = %d, dwKdsipSN = %u], event size[%u]bytes, osp event[%s]\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent, 
            tDstDlgId, (int)tKdsipRet, dwKdsipSN, strMsgBuf.size(), strOspEvent.c_str());

        return false;
    }

    //建立UAC端osp-sip的映射
    TOspSipBind tOspSipBind;
    tOspSipBind.tOspId = tSrcOspTrans;
    //tOspSipBind.tSipId.tSipTrans = tTransId;
    tOspSipBind.tSipId.tSipDlg = tDstDlgId;
    tOspSipBind.eSipEvent = eSipEvent;
    tOspSipBind.strOspEvent = strOspEvent;
    //InsertOutTrans(tOspSipBind, tTransId);

    tOspSipBind.dwKdsipSN = dwKdsipSN;
    InsertSN(tOspSipBind, dwKdsipSN);

    CLASSLOG(OSPSIP, EVENT_LEV, "send sip event[%s-%d] to dialog[%p] success[dwKdsipSN = %u], event size[%u]bytes, osp event[%s]\n", 
        SipEventDesc(eSipEvent).c_str(), eSipEvent, tDstDlgId, 
        dwKdsipSN, strMsgBuf.size(), strOspEvent.c_str());

    return true;
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
bool COspSipConvertor::OspSipPostRsp_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_TRANS_ID tDstTransId,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode, const TSipExtraInfo* ptSipExtraInfo)
{    
    TKDSIP_TRANS_ID tTransId = tDstTransId;

    if (tTransId == INVALID_TRANS_ID)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] trans id is invlid\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    }

    if (!IsSipRspMsg(eSipEvent))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] is not a sip response\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        return false;
    } 

    CKDSipMsgBody* ptSipBody = NULL;
    CKDSipMsgBody tSipBody;
    if (strMsgBuf.length() != 0)
    {
        u8* pBuf = const_cast<u8*>(reinterpret_cast<const u8*>(strMsgBuf.c_str()));  //pclint:1773
        tSipBody.SetMsgBodyContent(pBuf, s32(strMsgBuf.length()));

        if (ptSipExtraInfo == NULL)
        {
            tSipBody.SetContentType(KDSIP_CONTENT_XML);
        }
        else
        {
            tSipBody.SetContentType(ptSipExtraInfo->eSipContentType);
        }

        ptSipBody = &tSipBody;
    }

    KDSIP_RETURN eSipRet = KDSIP_FAIL;

    string strOspEvent;
    if (tSipBody.GetContentType() == KDSIP_CONTENT_XML)
    {
        if (!strMsgBuf.empty())
        {
            strOspEvent = g_pCBGetSipXmlMsgType(strMsgBuf.c_str());

            if (strOspEvent.empty())
            {
                strOspEvent = "unknown osp event";
            }
        }
        else
        {
            strOspEvent = "NullSipBody";
        }      
    }
    else
    {
        if (ptSipExtraInfo != NULL)
        {
            strOspEvent = OspExtEventDesc(OSP_SIP_GB28181_MSG);
        }
        else
        {
            strOspEvent = "NullSipBody";
        } 
    }

    TOspSipBind tOspSipBind;
    tOspSipBind.tOspId = tSrcOspTrans;
    tOspSipBind.tSipId.tSipTrans = tTransId;
    tOspSipBind.eSipEvent = eSipEvent;
    tOspSipBind.strOspEvent = strOspEvent;

    u8 byLogLev = EVENT_LEV;

    TSipTransaction tSipId;

    switch (eSipEvent)
    {
    case KDSIP_EVENT_REGISTER_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (ptSipExtraInfo != NULL)
                {
                    //GB28181会走进来
                    if (eRspCode == KDSIP_RESPONSE_401_UNAUTHORIZED)
                    {
                        CKDSipMsgHead cSipHeader_Rsp;
                        //cSipHeader_Rsp.SetAuthenticateInfo(ptSipExtraInfo->strRealm.c_str());    
                        cSipHeader_Rsp.SetWWWAuthenticateRealm(ptSipExtraInfo->strRealm.c_str());
                        if(!ptSipExtraInfo->strContactUri.empty())cSipHeader_Rsp.SetContactURI(ptSipExtraInfo->strContactUri.c_str());
                        eSipRet = KDSipRegisterRspWithHeader(tTransId, eRspCode, &cSipHeader_Rsp, ptSipBody);

                        CLASSLOG(OSPSIP, CRITICAL_LEV, "401认证，发送Realm[%s]\n", 
                            ptSipExtraInfo->strRealm.c_str());
                    }
                    else if (eRspCode == KDSIP_RESPONSE_200_OK)
                    {
                        CKDSipMsgHead cSipHeader_Rsp;
                        cSipHeader_Rsp.SetDate(ptSipExtraInfo->strDate.c_str());
                        if(!ptSipExtraInfo->strContactUri.empty())cSipHeader_Rsp.SetContactURI(ptSipExtraInfo->strContactUri.c_str());
                        eSipRet = KDSipRegisterRspWithHeader(tTransId, eRspCode, &cSipHeader_Rsp, ptSipBody);

                        CLASSLOG(OSPSIP, CRITICAL_LEV, "注册或续航成功，校时[%s]\n", 
                            ptSipExtraInfo->strDate.c_str());
                    }
                    else
                    {                       
                        eSipRet = KDSipRegisterRsp(tTransId, eRspCode, ptSipBody);

                        CLASSLOG(OSPSIP, ERROR_LEV, "注册失败，sip_errorCode[%d]\n", 
                            int(eRspCode));
                    }  
                }
                else
                {
                    eSipRet = KDSipRegisterRsp(tTransId, eRspCode, ptSipBody);
                }                         
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_UNREGISTER_RSP:
        {
            if (ptSipExtraInfo != NULL)
            {
                if (eRspCode == KDSIP_RESPONSE_401_UNAUTHORIZED )
                {
                    CKDSipMsgHead cSipHeader_Rsp;
                    //cSipHeader_Rsp.SetAuthenticateInfo(ptSipExtraInfo->strRealm.c_str());    
                    cSipHeader_Rsp.SetWWWAuthenticateRealm(ptSipExtraInfo->strRealm.c_str());
                    eSipRet = KDSipRegisterRspWithHeader(tTransId, eRspCode, &cSipHeader_Rsp, ptSipBody);

                    CLASSLOG(OSPSIP, CRITICAL_LEV, "401认证(注销)，发送Realm[%s]\n", 
                        ptSipExtraInfo->strRealm.c_str());
                }
                else
                {
                    eSipRet = KDSipRegisterRsp(tTransId, eRspCode, ptSipBody);
                }
            }
            else
            {
                eSipRet = KDSipRegisterRsp(tTransId, eRspCode, ptSipBody);
            }
        }
        break;

    case KDSIP_EVENT_INVITE_RSP:
        {
            //fanxg-20140813 为了支持公安一所对contact字段使用user@ip:port形式，上层构造正确的contact字段传到底层。
            if (ptSipExtraInfo != NULL)
            {
                //国标需要特殊的contact字段
                CKDSipMsgHead cSipHeader_Rsp;
                CKDSipMsgHead* pcSipHeader_Rsp = NULL;

                if (!ptSipExtraInfo->strSubject.empty())
                {
					if(!ptSipExtraInfo->strContactUri.empty())
					{
						cSipHeader_Rsp.SetContactURI(ptSipExtraInfo->strContactUri.c_str());
					}
					else
					{
						string strGbId;
						HTOOL_MEMLIST hMemList = SIP_TOOL_CreateMemList();
						TGB_SUBJECT tSub;
						memset(&tSub, 0, sizeof(tSub));
						u16 wRet = SIP_TOOL_CONVERT_S2TGB_SUBJECT(hMemList, ptSipExtraInfo->strSubject.c_str(), &tSub);
						if (wRet == SIP_TOOL_OK && tSub.szMediaSender != NULL)
						{
							strGbId = tSub.szMediaSender;
						}
						else
						{
							CLASSLOG(OSPSIP, ERROR_LEV, "subject转换失败[Ret = %d, szMediaSender = %s]\n", 
								wRet, tSub.szMediaSender);
						}
						SIP_TOOL_FreeMemList(hMemList);

						if (!strGbId.empty())
						{
                            string strExtranetIp;
                            u16 wExtranetPort;
                            OspSipConvert2ExtraNetAddr(ptSipExtraInfo, g_tOspSipConf.tLocalGbAddr.tIpAddr, g_tOspSipConf.tLocalGbAddr.wPort,
                                strExtranetIp, wExtranetPort);
							cSipHeader_Rsp.SetContactURI(strGbId.c_str(),
								strExtranetIp.c_str(), wExtranetPort);
						} 
					}
					pcSipHeader_Rsp = &cSipHeader_Rsp;
					CLASSLOG(OSPSIP, CRITICAL_LEV, "国标INVITE应答携带contact[%s]\n", 
						pcSipHeader_Rsp->GetContactURI());
                }
                
                eSipRet = KDSipInviteRsp(tTransId, eRspCode, ptSipBody, pcSipHeader_Rsp);
            }
            else
            {
                eSipRet = KDSipInviteRsp(tTransId, eRspCode, ptSipBody);
            }
            
            //当存在对话时，建立UAS端osp-sip的映射，这样UAC发送的对话内的新事务才能投递到osp事务
            if(KDSIP_OK == eSipRet && FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (tSipId.tSipDlg != INVALID_DIALOG_ID
                    && eRspCode == KDSIP_RESPONSE_200_OK)   //200 ok应答才需要添加到ospsip的对话列表
                {
                    tOspSipBind.tSipId.tSipDlg = tSipId.tSipDlg;
                    InsertDlg(tOspSipBind, tSipId.tSipDlg);
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_INVITE_BYE_RSP:
        {          
            eSipRet = KDSipByeRsp(tTransId, eRspCode, ptSipBody);

            //收到BYE时已经删除了对话的绑定关系，这里不需要再删除。
            //也就是说，以后上层可以不给BYE进行应答，当然应答了也没有关系

//             TSipTransaction tSipId;
//             if(FindSipIdByIncomeTrans(tTransId, tSipId))
//             {
//                 if (tSipId.tSipDlg != INVALID_DIALOG_ID)
//                 {
//                     EraseDlg(tSipId.tSipDlg);
//                 }
//                 else
//                 {
//                     CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
//                 }
//             }
//             else
//             {
//                 CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
//             }
        }
        break;

    case KDSIP_EVENT_INFO_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                eSipRet = KDSipInfoRsp(tTransId, eRspCode, ptSipBody);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }         
        }
        break;

    case KDSIP_EVENT_OPTIONS_HB_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                eSipRet = KDSipOptionsRsp(tTransId, eRspCode, ptSipBody);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }  
            
            byLogLev = TIMER_LEV;
        }
        break;

    case KDSIP_EVENT_MESSAGE_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                eSipRet = KDSipMessageRsp(tTransId, eRspCode, ptSipBody);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }         
        }
        break;

    case KDSIP_EVENT_MESSAGE_DIALOG_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                eSipRet = KDSipMessageRsp(tTransId, eRspCode, ptSipBody);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }           
        }
        break;

    case KDSIP_EVENT_SUBSCRIBE_RSP:
        {
            //fanxg-20150716 为了支持公安一所对contact字段使用user@ip:port形式，上层构造正确的contact字段传到底层。
            if (ptSipExtraInfo != NULL)
            {
                //国标需要特殊的contact字段
                CKDSipMsgHead cSipHeader_Rsp;
				SetContactUri(ptSipExtraInfo,cSipHeader_Rsp);

                CLASSLOG(OSPSIP, CRITICAL_LEV, "国标SUBSCRIBE应答携带contact[%s]\n", cSipHeader_Rsp.GetContactURI());
				if (ptSipExtraInfo->strSubject=="SelfSendActiveNtf") //与国标模块约定，此值意味着由业务层主动发出订阅会话激活Ntf
				{
					eSipRet = KDSipSubscribeRsp(tTransId, eRspCode, ptSipBody, &cSipHeader_Rsp,TRUE);
				}
				else
				{
					eSipRet = KDSipSubscribeRsp(tTransId, eRspCode, ptSipBody, &cSipHeader_Rsp);
				}
                
            }
            else
            {
                eSipRet = KDSipSubscribeRsp(tTransId, eRspCode, ptSipBody);
            }

            //当存在对话时，建立UAS端osp-sip的映射，这样UAC发送的对话内的新事务才能投递到osp事务
            if(KDSIP_OK == eSipRet && FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (tSipId.tSipDlg != INVALID_DIALOG_ID
                    && eRspCode == KDSIP_RESPONSE_200_OK)   //200 ok应答才需要添加到ospsip的对话列表
                {
                    if (ExistDlg(tSipId.tSipDlg))
                    {
                        CLASSLOG(OSPSIP, EVENT_LEV, "更新订阅请求应答\n");
                        //更新订阅不需要再次插入订阅对话
                    }
                    else
                    {
                        tOspSipBind.tSipId.tSipDlg = tSipId.tSipDlg;
                        InsertDlg(tOspSipBind, tSipId.tSipDlg);
                        CLASSLOG(OSPSIP, EVENT_LEV, "订阅请求应答\n");
                    }                
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_UNSUBSCRIBE_RSP:
        {
            //fanxg-20150716 为了支持公安一所对contact字段使用user@ip:port形式，上层构造正确的contact字段传到底层。
            if (ptSipExtraInfo != NULL)
            {
				//国标需要特殊的contact字段
				CKDSipMsgHead cSipHeader_Rsp;
				SetContactUri(ptSipExtraInfo,cSipHeader_Rsp);

                CLASSLOG(OSPSIP, CRITICAL_LEV, "国标SUBSCRIBE应答携带contact[%s]\n",cSipHeader_Rsp.GetContactURI());

                eSipRet = KDSipUnSubscribeRsp(tTransId, eRspCode, ptSipBody, &cSipHeader_Rsp);
            }
            else
            {
                eSipRet = KDSipUnSubscribeRsp(tTransId, eRspCode, ptSipBody);
            }

            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (tSipId.tSipDlg != INVALID_DIALOG_ID)
                {
                    EraseDlg(tSipId.tSipDlg);
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在对话\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_NOTIFY_RSP:
        {
            if(FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                eSipRet = KDSipNotifyRsp(tTransId, eRspCode, ptSipBody);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]不存在事务\n", tTransId);
            }           
        }
        break;

    default:
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "unknown sip event[%s-%d]\n", SipEventDesc(eSipEvent).c_str(), eSipEvent);
        }
        break;
    }

    if (eSipRet != KDSIP_OK)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] send fail, event size[%u]bytes, osp event[%s]\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent, 
            strMsgBuf.size(), strOspEvent.c_str());
        return false;
    }

    CLASSLOG(OSPSIP, byLogLev, "send sip event[%s-%d] to trans[%p] success, event size[%u]bytes, osp event[%s]\n", 
        SipEventDesc(eSipEvent).c_str(), eSipEvent, tTransId, strMsgBuf.size(), strOspEvent.c_str());

    return true;
}

void COspSipConvertor::ReleaseDialog_Inner(TKDSIP_DIALOG_ID tSipDlgId)
{
    if (tSipDlgId == INVALID_DIALOG_ID)
    {
        return;
    }

    TOspSipBind tOspSipBind = FindDlgBind(tSipDlgId);
    if (tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_REGISTER_RSP)
    {
        EraseDlg(tSipDlgId);
        KDSipTerminateRegDialog(tSipDlgId);
    }
    else if (tOspSipBind.eSipEvent == KDSIP_EVENT_INVITE_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_INVITE_RSP)
    {
        EraseDlg(tSipDlgId);
        KDSipTerminateInviteDialog(tSipDlgId);
    }
    else if (tOspSipBind.eSipEvent == KDSIP_EVENT_SUBSCRIBE_REQ || tOspSipBind.eSipEvent == KDSIP_EVENT_SUBSCRIBE_RSP)
    {
        EraseDlg(tSipDlgId);
        KDSipTerminateSubsDialog(tSipDlgId);
    }
    else
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip event[%s-%d] is not a dialog\n", 
            SipEventDesc(tOspSipBind.eSipEvent).c_str(), tOspSipBind.eSipEvent);
    }
}

void COspSipConvertor::SetContactUri( const TSipExtraInfo* ptSipExtraInfo, CKDSipMsgHead &tSipHead )
{
	if (NULL==ptSipExtraInfo)
	{
		return;
	}

	char szContact[MAX_SIP_URI_LEN+1] = {0};
	if (!ptSipExtraInfo->strContactUri.empty())
	{
		sprintf(szContact, "%s", ptSipExtraInfo->strContactUri.c_str());
	}
	else
	{
        string strExtranetIp;
        u16 wExtranetPort;
        OspSipConvert2ExtraNetAddr(ptSipExtraInfo, g_tOspSipConf.tLocalGbAddr.tIpAddr, g_tOspSipConf.tLocalGbAddr.wPort,
            strExtranetIp, wExtranetPort);
		sprintf(szContact, "sip:%s@%s:%d", g_tOspSipConf.tLocalGbURI.GetUser().c_str(),
			strExtranetIp.c_str(), wExtranetPort);
	}
	tSipHead.SetContactURI(szContact);
}
