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
            GLOBELOG(ERROR_LEV, "Ͷ��SipSendMsgʧ��\n");
            TOspSipSendMsg* ptSipSendMsg = (TOspSipSendMsg*)tMsg.pvMsg;
            delete ptSipSendMsg;
        }
        else if (tMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_RECV)
        {
            GLOBELOG(ERROR_LEV, "Ͷ��SipRecvMsgʧ��\n");
            CKDSipEvent* ptSipEvent = (CKDSipEvent*)tMsg.pvMsg;
            KDSipFreeSipEvent(ptSipEvent); 
        }
        else
        {
             GLOBELOG(ERROR_LEV, "Ͷ��δ֪SipMsgʧ��\n");
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
������      : DoConvert
����        : ����kdsip�ص���ospsip����Ϣ
�㷨ʵ��    : ��Ϣ���ࣺ���������١�����Ӧ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/04/09/             fanxg
====================================================================*/
bool COspSipConvertor::DoConvert(const CKDSipEvent* pSipEvent)
{
    if (pSipEvent == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sipeventָ��Ϊ��\n");
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

    //ospsip��kdsip�����ȶ�������һЩ��־�ļ��𣬱�����־�ļ����ܿ츲��
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
            CLASSLOG(OSPSIP, ERROR_LEV, "δ֪SIP��Ϣ����[%u]\n", pSipEvent->GetEvent());
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
            CLASSLOG(OSPSIP, ERROR_LEV, "sip��Ϣ[%s-%u]��Ҫ���͵��ϲ㣬��osp IID��Ч������band�Ѿ���ʱɾ��\n",
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

    //ÿ��SIP��Ϣ��Я���Զ˺ͱ���ʹ�õ������ַ��Ϣ

    //fanxg-20140428: kdsipĿǰʵ�ֶ���notify��Ϣ��û�����õ�ַ��Ϣ�ģ�Ϊ�˱������Ĵ����ӡ���������ε�
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
    

    //����SIP��Ϣͷ
    const CKDSipMsgHead* pSipHead = pSipEvent->GetSipHead();
    if (pSipHead != NULL)
    {     
        //call-idͷ��
        if (pSipHead->GetCallID() != NULL)
        {
            /*
               ������ʹ�õ����������¼����ϼ����������ϼ�Ӧ�����¼�ͨ�����ֻ���ȷ��˫�����ߡ�
               ��ua������������������Ҫͨ��callid��ȷ�����µ�ע�ỹ��ˢ��ע�ᡣ
               ˵����ͬһ��ע��Ự�ڵ�ˢ��ע��ʹ�ú͵�һ��ע��������ͬ��callid��
            */
            CLASSLOG(OSPSIP, TRIVIAL_LEV, "SipHead.CallID = %s\n",
                pSipHead->GetCallID());

            cMsg.SetSipCallID(pSipHead->GetCallID());
        }

        //sipЭ����from��to����Ϊ��;
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

        //contactͷ�����Ϊ��
        if (pSipHead->GetContactURI() != NULL)
        {
            CLASSLOG(OSPSIP, TRIVIAL_LEV, "SipHead.ContactUri = %s\n",
                pSipHead->GetContactURI());

            string strContact = GetSipUri(pSipHead->GetContactURI());
            cMsg.SetSipContactUri(strContact.c_str());
        }      

        //����ע����Ҫ��ȡcontactͷ���authͷ��
        if (pSipEvent->GetEvent() == KDSIP_EVENT_REGISTER_REQ
            || pSipEvent->GetEvent() == KDSIP_EVENT_UNREGISTER_REQ)
        {        
            //authͷ��
            CLASSLOG(OSPSIP, byLogLev, "SipHead.Auth = %s\n",
                pSipHead->GetAuthorization());

            cMsg.SetSipAuthInfo(pSipHead->GetAuthorization());

            if (!IsKedacomSipMsg(pSipHead))
            {
                cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
            }

            cMsg.SetSipExpire(pSipEvent->GetExpire());

            //RequestUriͷ�򣺶���REGISTER������GetProxyURI()�õ��ľ���RequestUri
            if (pSipHead->GetProxyURI() != NULL)
            {
                CLASSLOG(OSPSIP, byLogLev, "SipHead.RequestUri = %s\n",
                    pSipHead->GetProxyURI());

                string strRequest = GetSipUri(pSipHead->GetProxyURI());
                cMsg.SetSipRequestUri(strRequest.c_str());
            }    
        }

		//UserAgent�ֶ�
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

    //����SIP��Ϣ��
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
            //������Keda��sip��Ϣ
            if (g_pCBGetSipXmlMsgType == NULL)
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "g_pCBGetSipXmlMsgType == NULL\n");
                return false;
            }
            strMsgType = g_pCBGetSipXmlMsgType(pchContent);

            u16 wMsgType = OspExtEventID(strMsgType.c_str());
            if (wMsgType != INVALID_OSP_EVENT)
            {
                CLASSLOG(OSPSIP, EVENT_LEV, "�յ�ҵ����Ϣ[%s-%d]\n", 
                    strMsgType.c_str(), wMsgType);
                cMsg.SetOspEventID(wMsgType);
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "δ֪OSP��Ϣ����[%s],��ʹ��OSP_ADD_EVENT_DESC()ע����Ϣ\n", strMsgType.c_str());
                return false;
            }
        }
        else if (pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_SDP
            || pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_MANSCDP
            || pSipEvent->GetSipBody()->GetContentType() == KDSIP_CONTENT_MANSRTSP)
        {
            //������GB28181��sip��Ϣ��������Ϣʹ��ͳһ����Ϣ��Ͷ�ݸ��ϲ�
            cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);

            //����INVITE��Ҫ��ȡSubjectͷ��
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

        if (!cMsg.SetMsgBody(pchContent, u16(nLen+1))) //+1: ��ʾ���ַ���������'\0'Ҳ���ظ��ϲ�
        {
            CLASSLOG(OSPSIP, ERROR_LEV, "����sip��Ϣ��ʧ��pchContent = %p, nLen = %d\n",
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

    //sipЭ����user(o)����Ϊ�գ�domain(m)����Ϊ��;
    if (tKdsipUri.GetDomain() == NULL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sipUri�Ƿ���sipUri.User = %s, sipUri.Domain = %s\n",
            tKdsipUri.GetUser(), tKdsipUri.GetDomain());
        return strUri;
    }


    if (tKdsipUri.GetUser() != NULL)
    {
        strUri += tKdsipUri.GetUser();
    }
    else
    {
        CLASSLOG(OSPSIP, EVENT_LEV, "sipUriû��Я��user��tKdsipUri.User = %s, tKdsipUri.Domain = %s, tKdsipUri.Port = %d\n",
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
        //����Ҳ������ǶԻ�����Ҫ���öԻ�release��
        if (pSipEvent->GetEvent() == KDSIP_EVENT_DIALOG_CREATED)
        {
            //�ϲ���ΪDlg������ʱ�ˣ���kdsip����Ϊ�����ɹ��ˣ�����Ҫ�ͷű������ĳ�ʱDlg��
            KDSIP_DIALOG_ID tSipDlgId = pSipEvent->GetDialogID();
            EKDSIP_DIALOG_TYPE tSipDlgType = pSipEvent->GetDialogType();

            CLASSLOG(OSPSIP, ERROR_LEV, "SN[%lu]û�ж�Ӧ�ĶԻ�[DlgType:%d, DlgId:%lu]bind�����ܶԻ���������[%ds]�Żص�\n", 
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
            CLASSLOG(OSPSIP, ERROR_LEV, "SN[%lu]û�ж�Ӧ������[TransId:%lu]bind���������񴴽�����[%ds]�Żص�\n", 
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

        //ospsip��kdsip�����ȶ�������һЩ��־�ļ��𣬱�����־�ļ����ܿ츲��
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
                CLASSLOG(OSPSIP, ERROR_LEV, "sip[%s-%d]�Ի�����ʧ�ܣ�SN[%lu]\n", 
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
                CLASSLOG(OSPSIP, ERROR_LEV, "sip[%s-%d]���񴴽�ʧ�ܣ�SN[%lu]\n", 
                    SipEventDesc(tBind.eSipEvent).c_str(), tBind.eSipEvent, dwSN);

				if (KDSIP_EVENT_INVITE_BYE_REQ==tBind.eSipEvent)
				{
					CLASSLOG(OSPSIP, ERROR_LEV,"[%s-%d]���񴴽�ʧ��,ɾ����ӦDialog[%p]\n", 
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
            //kdsip�ײ��dialog�쳣��ֹ֪ͨ
            tOspTrans = FindDlg(pSipEvent->GetDialogID());
            EraseDlg(pSipEvent->GetDialogID());

            cMsg.SetOspEventID(OSP_SIP_DIALOG_TERMINATE);
        }
        break;

    case KDSIP_EVENT_TRANSACTION_TIMEOUT:
        {
            //�����ͳ�ʱ������Э��ջ�ᷢ���¼�KDSIP_EVENT_TRANSACTION_TIMEOUT��ospsip��
            //���¼��Ĵ�����Ҫ����Ի�������ͶԻ������󣬶Ի���������Ҫ�ж��������͡�

            //�����ǶԻ��ڻ��ǶԻ��ⶼҪ��ɾ������İ󶨹�ϵ
            tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());
            cMsg.SetOspEventID(OSP_SIP_MSG_PROC_FAIL);
            cMsg.SetSipErrorCode(KDSIP_RESPONSE_408_REQ_TIMEOUT);

            //����ǶԻ�����������ʱ����Ҫ���⴦��bye��unsubscribe��unregister��
            //ȷ���Ի��󶨹�ϵ��ɾ������Ϊ�ϲ㷢����Щ�����
            //Osp��������Ѿ����٣��ó�ʱ��Ϣ�����ϲ�Ҳ���ᴦ��
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
            //kdsip���ڷ���˵�register����û��ʹ�ûỰ���й�������ֱ�ӽ�����Ͷ�ݵ��ϲ㡣
            //�ϲ���Ҫ���м�Ȩ����������ʱ��·�ɵĹ���

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
            ֻҪ�յ�BYE��ֱ��ɾ��DLG�İ󶨹�ϵ�����ٵ��ϲ�BYE_RSP����ʱɾ����
            ��Ϊ��ʱ�ϲ��ҵ��Ҳ���ܷ�����BYE���Լ�ɾ���ˣ�BYE_RSP���ᵽ����
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
            //ֻ�б�UA��ӹ�������Ŀ�ķ�������������Ż�Ӧ��
            //�������Է�ֹ���µĴ���
            //UA1��UA2���ӳɹ���������������UA2���ߺ���Ѹ����������ʱUA1��û�и�֪��UA2����
            //UA1������UA2������������ΪUA2�����Ѿ�����������Ӧ��UA1������������UA1һֱ����
            //��֪��UA2������UA2��UA1�е���Ϣ�Ͳ������������UA2��UA1�·�������ӾͲ���ɹ���
            //�������Ƿǳ����صĴ����߼���
			
            if (g_cHbNodeList.FindHbNode(cMsg.GetSipFromUri()) != NULL
				&& g_tOspSipConf.tLocalURI.GetUser() == cMsg.GetSipToUri().GetUser()) //mod by lyt 20171227,proxy�����ٷ���ģ������֪ͨ�����������Ҫ���Ӷ�to�ֶε�У�飬��ֹ��ua id�仯���������ɸ��Զ�����Ӧ��200�����⡣
            {
                CLASSLOG(OSPSIP, TIMER_LEV, "�յ�src[%s]��������ֱ��Ӧ��200 ok\n", 
                    cMsg.GetSipFromUri().GetURIString().c_str());

                OspSipHeartbeatRsp(pSipEvent->GetTransID());            
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "�յ�src[%s]�������󣬵�src���ڱ��������б����ܱ���UA��������ֱ��Ӧ��404\n",
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
                //�·���Ķ�������
                tOspTrans.dwIID = GetDefaultOspIID();
            }
            else
            {
                //ˢ���Ѵ��ڵĶ�������
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
            //fanxg-20150715-��ʱӦ��һ���Ự��ȡ�����Ĳ���---begin
            if (!ExistDlg(pSipEvent->GetDialogID()))
            {
                //�·���Ķ�������
                tOspTrans.dwIID = GetDefaultOspIID();
            }
            else
            {
                //ˢ���Ѵ��ڵĶ�������
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
            }

            //tOspTrans = FindDlg(pSipEvent->GetDialogID());
            //fanxg-20150715-��ʱӦ��һ���Ự��ȡ�����Ĳ���---end

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

    //����Ӧ����
    if (pSipEvent->GetResponseCode() != KDSIP_RESPONSE_200_OK)
    {
        /*
        ���ط�������ʱ���Զ�Э��ջ���߶Զ�ҵ������
        �����˳���Ӧ��OspSip������Щʧ��Rsp���ϲ�ҵ��㷢��OSP_SIP_MSG_PROC_FAIL��

        ����Э��ջ�Ƿ��ͳ�ʱ�Ĵ�����kdxxxReq()�ķ���ֵ��ʱ������
        ���ͳ�ʱ���󱾵�Э��ջ�ᷢ���¼�KDSIP_EVENT_TRANSACTION_TIMEOUT��ospsip��
       */

        CLASSLOG(OSPSIP, ERROR_LEV, "recv sip event[%s-%d][status code: %d]������Ӧ, dlgId[%u], transId[%u]\n",
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
                    //������֤
                    cMsg.SetOspEventID(OSP_SIP_GB28181_MSG);
                    cMsg.SetSipRealm(pSipEvent->GetSipHead()->GetWWWAuthenticateRealm());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                }
                else
                {
                    //���ط��ͽ����Ự���󣬷��ص���DlgID��������DlgID�ͷ�
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                    EraseDlg(pSipEvent->GetDialogID());
                }
            }
            break;

        case KDSIP_EVENT_INVITE_RSP:
        case KDSIP_EVENT_SUBSCRIBE_RSP:
            {
                //���ط��ͽ����Ự���󣬷��ص���DlgID��������DlgID�ͷ�
                tOspTrans = FindDlg(pSipEvent->GetDialogID());
                EraseDlg(pSipEvent->GetDialogID());
            }
            break;


        case KDSIP_EVENT_INVITE_BYE_RSP:
        case KDSIP_EVENT_UNSUBSCRIBE_RSP:
            {
                //�Ự�Ѿ����������ص���TransID��������TransID���ͷ�
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
                //�Ự�ڵ����񣬲���Ҫ�ͷ�Dlg
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
        //����Ӧ����
        bool bIsGbMsg = false;
        switch (pSipEvent->GetEvent())
        {
        case KDSIP_EVENT_REGISTER_RSP:
            {
                if (!ExistOutTrans(pSipEvent->GetTransID()))
                {
                    //ע��
                    bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());     
                }
                else
                {
                    //ˢ��ע��
                    bIsGbMsg = IsGBOutTrans(pSipEvent->GetTransID());
                    tOspTrans = FindOspIdByOutTrans(pSipEvent->GetTransID());           
                }

                if (bIsGbMsg)
                {
                    //���������չDateͷ��
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
                    CLASSLOG(OSPSIP, ERROR_LEV, "������ϢTrans[%u]�Ҳ���bind�������Ѿ���ʱɾ��\n", 
                        pSipEvent->GetTransID());
                }
                else
                {
                    //���´�����ʱ���kdsip������ͬtransid������----->begin
                    if (GETAPP(tOspTrans.dwIID) != OSP_SIP_APP_ID)
                    {
                        CLASSLOG(OSPSIP, ERROR_LEV, "������ϢTrans[%u]��Ŀ�Ĳ���ospapp������kdsip������Ŵ��ң����ص����ϲ�!!!\n",
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
                    //����
                    bIsGbMsg = IsGBDlg(pSipEvent->GetDialogID());
                    tOspTrans = FindDlg(pSipEvent->GetDialogID());
                }
                else
                {
                    //���¶���
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

        //����ǹ�����Ϣ������ͳһ��Ϣ��
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]û��ӳ��osp trans\n", tSipTransId);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]�Ѿ�ӳ��osp trans\n", tSipTransId);
        return;
    }

    m_cOutTransMap[tSipTransId] = tOspSipBind;
}

void COspSipConvertor::EraseOutTrans(TKDSIP_TRANS_ID tSipTransId)
{
    if (!m_cOutTransMap.Exist(tSipTransId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]������osp trans��ɾ��ʧ��\n", tSipTransId);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]�Ѿ�ӳ��osp trans\n", tSipTransId);
        return;
    }

    m_cIncomeTransMap[tSipTransId] = tOspSipBind;
}

void COspSipConvertor::EraseIncomeTrans(TKDSIP_TRANS_ID tSipTransId)
{
    if (!m_cIncomeTransMap.Exist(tSipTransId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip trans[%u]������osp trans��ɾ��ʧ��\n", tSipTransId);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]û��ӳ��osp trans\n", tSipDlgId);
    }

    return tBind.tOspId;
}

TOspSipBind COspSipConvertor::FindDlgBind(TKDSIP_DIALOG_ID tSipDlgId)
{
    TOspSipBind tBind;
    if (!m_cDlgMap.Find(tSipDlgId, tBind))
    {    
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]û��ӳ��osp trans\n", tSipDlgId);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]�Ѿ�����osp trans������ʧ��\n", tSipDlgId);
        return;
    }

    m_cDlgMap[tSipDlgId] = tOspSipBind;
}

void COspSipConvertor::EraseDlg(TKDSIP_DIALOG_ID tSipDlgId)
{
    if (!m_cDlgMap.Exist(tSipDlgId))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip dlg[%u]������osp trans�������Ѿ�ɾ��\n", tSipDlgId);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "sip SN[%u]�Ѿ�����bind������ʧ��\n", dwSN);
        return;
    }

    m_cSNMap[dwSN] = tOspSipBind;
}

void COspSipConvertor::EraseSN(u32 dwSN)
{
    if (!m_cSNMap.Exist(dwSN))
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "sip SN[%u]������bind�������Ѿ�ɾ��\n", dwSN);
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
        CLASSLOG(OSPSIP, ERROR_LEV, "���sip·�ɱ�\n");
        m_cRouteTable.Empty();
        return;
    }

    CUaRouterNtfReq cReq;
    cReq.FromXml(strMsgBuf);

    if (cReq.GetOpType() != NTF_OP_TYPE_ADD 
        && cReq.GetOpType() != NTF_OP_TYPE_MOD
        && cReq.GetOpType() != NTF_OP_TYPE_DEL)
    {
        CLASSLOG(OSPSIP, ERROR_LEV, "��Ч��·�ɲ�������[%s]\n", cReq.GetOpType().c_str());
        return;
    }

    const CUaRouteTable& tRouteTable = cReq.GetUaRouteTable();
    CUaRouteTable::const_iterator iT = tRouteTable.begin();
    for (; iT != tRouteTable.end(); ++iT)
    {
        TSipURI tUri;
        tUri.SetURIString(iT->first);

        const TUaAddrPair& tAddrPair = iT->second;

        CLASSLOG(OSPSIP, CRITICAL_LEV, "·��from[%s-%s:%d]--->to[%s-%s:%d],opType[%s]\n", 
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
    //sip��Դ������ʱ
    CheckSipCreateTimeout();

    //����������kdsip�ײ���Զ�ɾ������
    CheckTransTimeout(m_cOutTransMap);

    //�յ�����������ϲ�һֱû�з���Ӧ��
    //��ô��ospsip�㷢�ͳ�ʱ����Ӧ���Ա�kdsipɾ��������
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
                CLASSLOG(OSPSIP, ERROR_LEV,"ɾ����ʱ����[%s-%d][%s]trans[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
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

                        CLASSLOG(OSPSIP, ERROR_LEV, "income����[%s]��ʱ���Զ�Ӧ��408\n", 
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
                CLASSLOG(OSPSIP, ERROR_LEV,"ɾ��������ʱ����[%s-%d][%s]SN[%u]---osp[app(%u)-ins(%u)-task(%u)-seq(%u)]---birth[%s]\n", 
                    SipEventDesc(tOspSipBind.eSipEvent).c_str(), (s32)tOspSipBind.eSipEvent, tOspSipBind.strOspEvent.c_str(),
                    dwSN, GETAPP(tOspTrans.dwIID), GETINS(tOspTrans.dwIID),
                    tOspTrans.dwTaskID, tOspTrans.dwSeqNO, tBirthTime.GetString().c_str());
                
				if (KDSIP_EVENT_INVITE_BYE_REQ==tOspSipBind.eSipEvent)
				{
					CLASSLOG(OSPSIP, ERROR_LEV,"[%s-%d]���񴴽���ʱ,ɾ����ӦDialog[%p]\n", 
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
        OspPrintf(TRUE, FALSE, "out�������Ϣ��[%u]��\n", m_cOutTransMap.GetSize());    
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
        OspPrintf(TRUE, FALSE, "\nout�������Ϣ��[%u]��\n", m_cOutTransMap.GetSize());
    }
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    if ((dwPrintFlag&PRINT_IN_TRANS) == PRINT_IN_TRANS)
    {
        OspPrintf(TRUE, FALSE, "income������Ϣ��[%u]��\n", m_cIncomeTransMap.GetSize());    
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
        OspPrintf(TRUE, FALSE, "\nincome������Ϣ��[%u]��\n", m_cIncomeTransMap.GetSize());
    }
    
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintDialog(u32 dwPrintFlag) const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    OspPrintf(TRUE, FALSE, "���жԻ�����Ϣ��[%u]��\n", m_cDlgMap.GetSize());    
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
    OspPrintf(TRUE, FALSE, "\n�Ի�����Ϣ��Total[%u]��, Ivt[%u], Ss[%u], Reg[%u]\n", 
        m_cDlgMap.GetSize(), dwIvtNum, dwSsNum, dwRegNum);

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintSN() const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    OspPrintf(TRUE, FALSE, "SN������Ϣ��[%u]��\n", m_cSNMap.GetSize());    
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
    OspPrintf(TRUE, FALSE, "SN������Ϣ��[%u]��\n", m_cSNMap.GetSize()); 

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}

void COspSipConvertor::PrintRoute(const char* szUaName) const
{
    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");

    if (szUaName != NULL)
    {
        //��ӡָ��UA��·����Ϣ
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
            OspPrintf(TRUE, FALSE, "  [%s]������·����Ϣ\n", 
                tUri.GetURIString().c_str());
        }      
    }
    else
    {
        OspPrintf(TRUE, FALSE, "·�ɱ���Ϣ��[%u]��\n", m_cRouteTable.GetSize());

        //��ӡ����UA��·����Ϣ
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

        OspPrintf(TRUE, FALSE, "·�ɱ���Ϣ��[%u]��\n", m_cRouteTable.GetSize()); 
    }  

    OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n\n");
}


/*====================================================================
������      : OspSipPostReq
����        : ����SIP�Ի���������Ϣ������OSP�����SIP�����ӳ���ϵ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              eSipEvent: sip��ϢID
              strMsgBuf: sip��Ϣ��
              tDstURI: ��ϢĿ��URI
              tSrcOspTrans: osp����ID
����ֵ˵��  : �ɹ�:true��ʧ��:false
��ע        : �ɷ���SIP�¼�����:

              KDSIP_EVENT_REGISTER_REQ,
              KDSIP_EVENT_UNREGISTER_REQ,
              KDSIP_EVENT_INVITE_REQ,
              KDSIP_EVENT_OPTIONS_HB_REQ,
              KDSIP_EVENT_MESSAGE_REQ,
              KDSIP_EVENT_SUBSCRIBE_REQ,
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
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
        //UA��UA֮��ֱ��ͨ�ţ������ϲ�ֱ��ָ���Զ˵�ַ����U2U����������
        CLASSLOG(OSPSIP, byLogLev, "sip event[%s-%d] directly send to UA[%s] without proxy\n", 
            SipEventDesc(eSipEvent).c_str(), eSipEvent, strDirectUAAddr.c_str());

        sprintf(szProxy, "<sip:%s;lr>", strDirectUAAddr.c_str());

        if (eSipEvent == KDSIP_EVENT_INVITE_REQ || eSipEvent == KDSIP_EVENT_SUBSCRIBE_REQ)
        {
            //�����Ի�ʱ��Ϊ����ʹ�Զ�����ȷ���ͶԻ��ڷ�����������Ҫ��from��domain���ó�user@LocalIp:port��ʽ
            TSipURI tFromUri;  //user@LocalIp:port

            if (ptSipExtraInfo != NULL)
            {
				//����
				if ( !ptSipExtraInfo->strFromUri.empty() )
				{//����ϲ�ָ����From����ʹ��ҵ��ָ����From URI
					tFromUri.SetNOString(ptSipExtraInfo->strFromUri);
				}
				else
				{
                    // 20150724 ����ƽ̨��Ϊ�¼�ʱ�������⣬������From��To��ʽ��һ������HK��DH��û����ġ�
					//tFromUri.SetUser(g_tOspSipConf.tLocalGbURI.GetUser());  
					//tFromUri.SetDomain(g_tOspSipConf.tLocalGbAddr.GetSipAddrStr()); 
                    tFromUri = g_tOspSipConf.tLocalGbURI;
				}
            }
            else
            {
                //�ǹ���
                tFromUri.SetUser(g_tOspSipConf.tLocalURI.GetUser());
                tFromUri.SetDomain(g_tOspSipConf.tLocalAddr.GetSipAddrStr());
            }
            
            sprintf(szFrom, "sip:%s", tFromUri.GetNOString());
        }
        else
        {
            if (ptSipExtraInfo != NULL)
            {
                //����
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
                //�ǹ���
                sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());
            }        
        }
    }
    else if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
    {
        if (tDstURI.GetDomain() != OspSipGetLocalURI().GetDomain()) //liangli �������ж�tDstURI�Ƿ�proxy
        {
            //����Ƿ����������Ϣ��ֱ��Ͷ�ݸ�proxy
            sprintf(szProxy, "<sip:%s:%d;lr>", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), g_tOspSipConf.tProxyAddr.wPort);
            sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());
        }
        else
        {
            TUaAddrPair* ptDstRoute = m_cRouteTable.Find(tDstURI);
            if (ptDstRoute == NULL)
            {
               /*
               ������Ϣ��������dstUri�޷��ҵ�dstAddr����Ҳ����Ϣ��Ͷ�ݵ�proxy��
               (����dstUA��ע�ᵽproxy��proxy��û�ܷ�������������Ϣproxy�ǿ�����ת�ģ�
               Ҳ����dstUA������û����proxyע�ᣬ����ʱproxy����ֱ�ӻ�404��)
               */
                sprintf(szProxy, "<sip:%s:%d;lr>", g_tOspSipConf.tProxyAddr.tIpAddr.c_str(), g_tOspSipConf.tProxyAddr.wPort);
                sprintf(szFrom, "sip:%s", g_tOspSipConf.tLocalURI.GetNOString());

                //UA��proxy����·��
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
                //������Ϣ�����Ҹ���dstUri�ҵ�dstAddr���Ǿͽ���ϢͶ�ݵ�dstAddr��
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

                //U2U��Ҫ��дcontact�ֶΣ���֤�Զ˵ĻỰ�������ܷ�����
                char szContact[MAX_SIP_URI_LEN+1] = {0};
                sprintf(szContact, "sip:%s:%d", tSrcAddr.tIpAddr.c_str(), tSrcAddr.wPort);
                tSipHead.SetContactURI(szContact);
            }
        }       
    }
    else
    {
        //UA��UA֮��ͨ��proxyͨ��
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
                //ƽ̨2.0ע��proxy
                TKDSIP_DIALOG_ID tRegProxyID = GetRegProxyDlg();
                if (tRegProxyID != INVALID_DIALOG_ID)
                {
                    //ˢ��ע��
                    tKdsipRet = KDSipRegisterRefreshReq(tRegProxyID, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                }
                else
                {
                    //�½�ע��
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
                //�����ϼ�ע���߼�
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
				//ƽ̨2.0ע��proxy
				TKDSIP_DIALOG_ID tRegProxyID = GetRegProxyDlg();
				if (tRegProxyID != INVALID_DIALOG_ID)
				{
					//ȡ��ע��
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
				//�����ϼ�ע���߼�
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
				// �޸�contact�ֶΣ���ֹgbs�Ự��bye��������Ϣ�޷����� [9/8/2015 pangubing]
				SetContactUri(ptSipExtraInfo, tSipHead);
			}
			else
			{
				//todo,����ģ��ʹ�ñ��汾ospsip������ǰ�����Ȳ��������֧���ɵ�contact�ֶ��Ƿ���Ч
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
				// �޸Ĺ���contact�ֶ�
				SetContactUri(ptSipExtraInfo, tSipHead);
				if(ptSipExtraInfo->dwRegExpire != 0) //GB���Ľ��ô��ֶΰ�ˢ�¶����Ƿ�Ҫ��Ϣ�����Ϣ��ҵ������ospsip
				{
					tKdsipRet = KDSipSubscribeReq(tSipHead, ptSipBody, dwKdsipSN, KDSIP_TRANSPORT_UDP, DEFAULT_EXPIRES, TRUE);
					break;
				}
			}
			else
			{
				//todo,����ģ��ʹ�ñ��汾ospsip������ǰ�����Ȳ��������֧���ɵ�contact�ֶ��Ƿ���Ч
				char szContact[MAX_SIP_URI_LEN+1] = {0};
				sprintf(szContact, "sip:%s@%s",g_tOspSipConf.tLocalURI.GetUser().c_str(),g_tOspSipConf.tLocalAddr.GetSipAddrStr().c_str());
				tSipHead.SetContactURI(szContact);
			}
			
            tKdsipRet = KDSipSubscribeReq(tSipHead, ptSipBody, dwKdsipSN);
        }
        break;
    case KDSIP_EVENT_UNSUBSCRIBE_REQ: // hzytodo 20150715 ��ʱ���ڷ��ͻỰ��ȡ����������
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
������      : OspSipPostInDlgReq
����        : ����SIP�Ի���������Ϣ������OSP�����SIP�����ӳ���ϵ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              eSipEvent: sip��ϢID
              strMsgBuf: sip��Ϣ��
              tDstDlgId: sip�Ի�ID
              tSrcOspTrans: osp����ID
����ֵ˵��  : �ɹ�:true��ʧ��:false
��ע        : �ɷ���SIP�¼�����:

              KDSIP_EVENT_INVITE_ACK,
              KDSIP_EVENT_INVITE_BYE_REQ,
              KDSIP_EVENT_INFO_REQ,
              KDSIP_EVENT_MESSAGE_DIALOG_REQ,
              KDSIP_EVENT_SUBSCRIBE_REQ,
              KDSIP_EVENT_UNSUBSCRIBE_REQ,
              KDSIP_EVENT_NOTIFY_REQ,
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
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
            //�����ϼ�ע����֤����
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
                    //ע����֤
                    CKDSipMsgHead cSipHead;
                    cSipHead.SetAuthorizationInfo(ptSipExtraInfo->strUserName.c_str(), 
                        ptSipExtraInfo->strPassword.c_str());

                    tKdsipRet = KDSipRegisterReqWithHeader(tDstDlgId, ptSipExtraInfo->dwRegExpire, &cSipHead, NULL, dwKdsipSN);
                }
                else
                {
                    //ˢ��ע��
                    tKdsipRet = KDSipRegisterRefreshReq(tDstDlgId, g_nRegProxyExpires, ptSipBody, dwKdsipSN);
                }
            }              
        }
        break;

    case KDSIP_EVENT_INVITE_ACK:
        {
            //ACK��Ӧ����Ҫ���⴦��
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
            //����ˢ��,������ǰ�ĶԻ�ID����Ҫ���⴦��
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

    //����UAC��osp-sip��ӳ��
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
������      : OspSipPostRsp
����        : ����SIPӦ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              eSipEvent: sip��ϢID
              tTransId: SIP����ID
              strMsgBuf: sip��Ϣ��
              tSrcOspTrans: ����osp����ID
              eRspCode: SIP״̬��
����ֵ˵��  : �ɹ�:true��ʧ��:false
��ע        : �ɷ���SIP�¼�����:

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
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
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
                    //GB28181���߽���
                    if (eRspCode == KDSIP_RESPONSE_401_UNAUTHORIZED)
                    {
                        CKDSipMsgHead cSipHeader_Rsp;
                        //cSipHeader_Rsp.SetAuthenticateInfo(ptSipExtraInfo->strRealm.c_str());    
                        cSipHeader_Rsp.SetWWWAuthenticateRealm(ptSipExtraInfo->strRealm.c_str());
                        if(!ptSipExtraInfo->strContactUri.empty())cSipHeader_Rsp.SetContactURI(ptSipExtraInfo->strContactUri.c_str());
                        eSipRet = KDSipRegisterRspWithHeader(tTransId, eRspCode, &cSipHeader_Rsp, ptSipBody);

                        CLASSLOG(OSPSIP, CRITICAL_LEV, "401��֤������Realm[%s]\n", 
                            ptSipExtraInfo->strRealm.c_str());
                    }
                    else if (eRspCode == KDSIP_RESPONSE_200_OK)
                    {
                        CKDSipMsgHead cSipHeader_Rsp;
                        cSipHeader_Rsp.SetDate(ptSipExtraInfo->strDate.c_str());
                        if(!ptSipExtraInfo->strContactUri.empty())cSipHeader_Rsp.SetContactURI(ptSipExtraInfo->strContactUri.c_str());
                        eSipRet = KDSipRegisterRspWithHeader(tTransId, eRspCode, &cSipHeader_Rsp, ptSipBody);

                        CLASSLOG(OSPSIP, CRITICAL_LEV, "ע��������ɹ���Уʱ[%s]\n", 
                            ptSipExtraInfo->strDate.c_str());
                    }
                    else
                    {                       
                        eSipRet = KDSipRegisterRsp(tTransId, eRspCode, ptSipBody);

                        CLASSLOG(OSPSIP, ERROR_LEV, "ע��ʧ�ܣ�sip_errorCode[%d]\n", 
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
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

                    CLASSLOG(OSPSIP, CRITICAL_LEV, "401��֤(ע��)������Realm[%s]\n", 
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
            //fanxg-20140813 Ϊ��֧�ֹ���һ����contact�ֶ�ʹ��user@ip:port��ʽ���ϲ㹹����ȷ��contact�ֶδ����ײ㡣
            if (ptSipExtraInfo != NULL)
            {
                //������Ҫ�����contact�ֶ�
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
							CLASSLOG(OSPSIP, ERROR_LEV, "subjectת��ʧ��[Ret = %d, szMediaSender = %s]\n", 
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
					CLASSLOG(OSPSIP, CRITICAL_LEV, "����INVITEӦ��Я��contact[%s]\n", 
						pcSipHeader_Rsp->GetContactURI());
                }
                
                eSipRet = KDSipInviteRsp(tTransId, eRspCode, ptSipBody, pcSipHeader_Rsp);
            }
            else
            {
                eSipRet = KDSipInviteRsp(tTransId, eRspCode, ptSipBody);
            }
            
            //�����ڶԻ�ʱ������UAS��osp-sip��ӳ�䣬����UAC���͵ĶԻ��ڵ����������Ͷ�ݵ�osp����
            if(KDSIP_OK == eSipRet && FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (tSipId.tSipDlg != INVALID_DIALOG_ID
                    && eRspCode == KDSIP_RESPONSE_200_OK)   //200 okӦ�����Ҫ��ӵ�ospsip�ĶԻ��б�
                {
                    tOspSipBind.tSipId.tSipDlg = tSipId.tSipDlg;
                    InsertDlg(tOspSipBind, tSipId.tSipDlg);
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_INVITE_BYE_RSP:
        {          
            eSipRet = KDSipByeRsp(tTransId, eRspCode, ptSipBody);

            //�յ�BYEʱ�Ѿ�ɾ���˶Ի��İ󶨹�ϵ�����ﲻ��Ҫ��ɾ����
            //Ҳ����˵���Ժ��ϲ���Բ���BYE����Ӧ�𣬵�ȻӦ����Ҳû�й�ϵ

//             TSipTransaction tSipId;
//             if(FindSipIdByIncomeTrans(tTransId, tSipId))
//             {
//                 if (tSipId.tSipDlg != INVALID_DIALOG_ID)
//                 {
//                     EraseDlg(tSipId.tSipDlg);
//                 }
//                 else
//                 {
//                     CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
//                 }
//             }
//             else
//             {
//                 CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
            }           
        }
        break;

    case KDSIP_EVENT_SUBSCRIBE_RSP:
        {
            //fanxg-20150716 Ϊ��֧�ֹ���һ����contact�ֶ�ʹ��user@ip:port��ʽ���ϲ㹹����ȷ��contact�ֶδ����ײ㡣
            if (ptSipExtraInfo != NULL)
            {
                //������Ҫ�����contact�ֶ�
                CKDSipMsgHead cSipHeader_Rsp;
				SetContactUri(ptSipExtraInfo,cSipHeader_Rsp);

                CLASSLOG(OSPSIP, CRITICAL_LEV, "����SUBSCRIBEӦ��Я��contact[%s]\n", cSipHeader_Rsp.GetContactURI());
				if (ptSipExtraInfo->strSubject=="SelfSendActiveNtf") //�����ģ��Լ������ֵ��ζ����ҵ��������������ĻỰ����Ntf
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

            //�����ڶԻ�ʱ������UAS��osp-sip��ӳ�䣬����UAC���͵ĶԻ��ڵ����������Ͷ�ݵ�osp����
            if(KDSIP_OK == eSipRet && FindSipIdByIncomeTrans(tTransId, tSipId))
            {
                if (tSipId.tSipDlg != INVALID_DIALOG_ID
                    && eRspCode == KDSIP_RESPONSE_200_OK)   //200 okӦ�����Ҫ��ӵ�ospsip�ĶԻ��б�
                {
                    if (ExistDlg(tSipId.tSipDlg))
                    {
                        CLASSLOG(OSPSIP, EVENT_LEV, "���¶�������Ӧ��\n");
                        //���¶��Ĳ���Ҫ�ٴβ��붩�ĶԻ�
                    }
                    else
                    {
                        tOspSipBind.tSipId.tSipDlg = tSipId.tSipDlg;
                        InsertDlg(tOspSipBind, tSipId.tSipDlg);
                        CLASSLOG(OSPSIP, EVENT_LEV, "��������Ӧ��\n");
                    }                
                }
                else
                {
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
            }
        }
        break;

    case KDSIP_EVENT_UNSUBSCRIBE_RSP:
        {
            //fanxg-20150716 Ϊ��֧�ֹ���һ����contact�ֶ�ʹ��user@ip:port��ʽ���ϲ㹹����ȷ��contact�ֶδ����ײ㡣
            if (ptSipExtraInfo != NULL)
            {
				//������Ҫ�����contact�ֶ�
				CKDSipMsgHead cSipHeader_Rsp;
				SetContactUri(ptSipExtraInfo,cSipHeader_Rsp);

                CLASSLOG(OSPSIP, CRITICAL_LEV, "����SUBSCRIBEӦ��Я��contact[%s]\n",cSipHeader_Rsp.GetContactURI());

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
                    CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
                }
            }
            else
            {
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]�����ڶԻ�\n", tTransId);
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
                CLASSLOG(OSPSIP, ERROR_LEV, "transid[%u]����������\n", tTransId);
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
