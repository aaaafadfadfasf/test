
#ifndef _OSP_SIP_H_
#define _OSP_SIP_H_

#include "cms/ospext/ospext.h"
#include "cbb/sip/kdsip2.h"
#include <string>
using std::string;

typedef KDSIP_DIALOG_ID TKDSIP_DIALOG_ID;
typedef KDSIP_TRANS_ID  TKDSIP_TRANS_ID;
typedef std::string     TIpAddress;

enum EOSP_SIP_EVENT
{
    OSP_SIP_EV_BEGIN = OSP_SIP_EVENT_BEGIN,

    //SIP����֪ͨ
    OSP_SIP_DISCONNECT,      //��Ϣ��: szSipUri(char* �ַ���, �����Զ˵�sip uri)

    //Proxy����֪ͨ
    OSP_PROXY_DISCONNECT,    //��Ϣ��: ��

	//Proxyע��ɹ�֪ͨ
	OSP_PROXY_CONNECT,       //��Ϣ��: ��

    //SIP BYE����
    OSP_SIP_BYE_REQ,    //��Ϣ��: ͸��BYE�����е���Ϣ��

    //SIP BYEӦ��
    OSP_SIP_BYE_RSP,       //��Ϣ��: ��

    //SIP�㴦�����
    OSP_SIP_MSG_PROC_FAIL,   //��Ϣ��: ��

	//SIP��Dialog�쳣��ֹ֪ͨ
	OSP_SIP_DIALOG_TERMINATE,//��Ϣ��: ��

    //ospsip֪ͨ�ϲ����¼��������ļ�
    OSP_SIP_SIGNAL_RELOAD_CFG,//��Ϣ��: ��

    OSP_SIP_EV_END = OSP_SIP_EVENT_BEGIN + 500,
};

//����Ψһ�豸����
#define UNIQUE_SIP_UA_3AS       ("3as")
#define UNIQUE_SIP_UA_CMU       ("cmu")
#define UNIQUE_SIP_UA_PROXY     ("proxy")
#define UNIQUE_SIP_UA_RCS       ("rcs")
#define UNIQUE_SIP_UA_TVS       ("tvs")
#define UNIQUE_SIP_UA_ALS       ("als")
#define UNIQUE_SIP_UA_UAS       ("uas")
#define UNIQUE_SIP_UA_MPS       ("mps")

struct TSipAddr;
struct TSipURI;
struct TOspSipConf;
struct TSipStackConf;
struct TOspTransaction;
class COspSipMsg;

typedef TOspTransaction TOspTransID;

/*====================================================================
������      : InitOspSip
����        : OspSip��ʼ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              tOspSipConf: OspSip��ص�������Ϣ
              tSipStackConf: SipЭ��ջ��ص�����
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipInit(const TOspSipConf& tOspSipConf, const TSipStackConf& tSipStackConf);


/*====================================================================
������      : QuitOspSip
����        : OspSip
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
void OspSipQuit();


/*====================================================================
������      : OspSipRegProxy
����        : ע�ᵽSIP PROXY
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             dwExpire: The expires value for registration(��λ����)
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipRegProxy(s32 nExpires);


/*====================================================================
������      : OspSipSetHBParam
����        : ����OSPSIP��������
�㷨ʵ��    : ����ʧ�ܣ����ϲ㷢�� OSP_SIP_DISCONNECT ��Ϣ
����ȫ�ֱ���:
�������˵��:
             tHbDst: ����Ŀ��
             tHbSrc: ����������
             wHbTime: ���������ʱ��(����Ϊ��λ),
             wHbCnt: ����������
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipSetHBParam(const TSipURI& tHbDst, const TOspTransID& tHbSrc, LPCSTR szDirectUAAddr = NULL, u16 wHbTime = 10, u16 wHbCnt = 3);


/*====================================================================
������      : OspSipStopHB
����        : ֹͣOSPSIP����
�㷨ʵ��    : ֹͣ�������������ϲ㷢�� OSP_SIP_DISCONNECT ��Ϣ
����ȫ�ֱ���:
�������˵��:
             tHbDst: ����Ŀ��
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/12/27  0.1         fanxg
====================================================================*/
bool OspSipStopHB(const TSipURI& tHbDst);


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
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL);

template<class T>
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const T& tMsg, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostReq(eSipEvent, strBuf, tDstURI, tSrcOspTrans, szDirectUAAddr);
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
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans);

template<class T>
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const T& tMsg, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostInDlgReq(eSipEvent, strBuf, tDstDlgId, tSrcOspTrans);
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
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const string& strMsgBuf,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK);

template<class T>
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const T& tMsg,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostRsp(eSipEvent, tTransId, strBuf, tSrcOspTrans, eRspCode);
}

/*====================================================================
������      : OspSipReleaseDlg
����        : �ͷŶԻ���Դ�����ᷢ��SIP��Ϣ
              ���ڴ��������Dialog����Դ����
�㷨ʵ��    : 
����ȫ�ֱ���:
�������˵��:
              tDlgId: �Ի�ID
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
void OspSipReleaseDlg(TKDSIP_DIALOG_ID tDlgId);


/*====================================================================
������      : OspSipGetLocalURI
����        : ����UA��Proxyע��ʱ��д��LocalURI

              ��ΪSIPЭ��ջ����UDPЭ�飬�������Ҫ֪���ͻ��˵�URI������ͻ��˷�����
              �ͻ���������ע��ʱ��ҪЯ������URI��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : ����UA��Proxyע��ʱ��д��LocalURI
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/29  0.1         fanxg
====================================================================*/
const TSipURI OspSipGetLocalURI();

void OspSipSignalHandleCB(int nSig);


//ospsip���Դ�ӡ����

//ospsip����
API void ospsiphelp();

//ospsip�汾
API void ospsipver();

//����ospsip��ӡ����
API void ospsiplog(u8 byLogLev);

//��ʾospsip�����б�
API void ospsiphblist();

//��ʾSN��Ϣ
API void ospsipsn();

//��ʾ������Ϣ
API void ospsiptrans();

//��ʾincome������Ϣ
API void ospsipin();

//��ʾout������Ϣ
API void ospsipout();

//��ʾ�Ի���Ϣ
API void ospsipdlg();

//��ʾinvite�Ի���Ϣ
API void ospsipivt();

//��ʾsubscribe�Ի���Ϣ
API void ospsipss();

//��ʾregister�Ի���Ϣ
API void ospsipreg();

//��ʾ���Ͷ�����Ϣ
API void ospsipque();

//����kdsip��ӡ����
API void kdsiplog(u8 byLogLev, u8 byIsKdSipOnTelnet = 0);

//��ӡkdsip�������Ϣ
API void kdsipshow();

struct TSipAddr
{
    TSipAddr()
    {
        wPort = 0;
    }

    string GetSipAddrStr() const
    {
        char szPort[16] = {0};
        sprintf(szPort, "%u", wPort);

        string strAddr = tIpAddr;
        strAddr += ":";
        strAddr += szPort;

        return strAddr;
    }

    string GetIpStr() const
    {
        return tIpAddr;
    }

    string GetPortStr() const
    {
        char szPort[16] = {0};
        sprintf(szPort, "%u", wPort);
        return string(szPort);
    }

    TIpAddress tIpAddr;
    u16 wPort;
};

struct TSipURI
{
public:
    TSipURI()
    {
        Clear();
    }

    TSipURI(const string& strURI)
    {
        Clear();
        SetNOString(strURI);
    }

    ~TSipURI()
    {
        Clear();
    }

    void Clear()
    {
        m_strUser.clear();
        m_strDomain.clear();
    }

    void SetNull()
    {
        Clear();
    }

    bool IsNull() const
    {
        return (m_strUser.empty() || m_strDomain.empty());
    }

    bool operator == ( const TSipURI& tOther ) const
    {
        return (m_strUser == tOther.m_strUser && m_strDomain == tOther.m_strDomain);
    }

    bool operator != ( const TSipURI& tOther ) const
    {
        return !(operator ==(tOther));
    }

    bool IsValidURI() const
    {
        return (!m_strUser.empty() && !m_strDomain.empty());
    }

    //Ŀǰ������Ψһ��URI�������ﶨ��ͻ�ȡ�ƺ���������Ϊ��ᵼ��ospsip�������ϲ��ҵ��ģ�鶨�壬��������
    TSipURI GetCmuURI() const
    {
        TSipURI tCmuNO;
        tCmuNO.m_strUser = UNIQUE_SIP_UA_CMU;
        tCmuNO.m_strDomain = m_strDomain;

        return tCmuNO;
    }
    TSipURI Get3ASURI() const
    {
        TSipURI t3asNO;
        t3asNO.m_strUser = UNIQUE_SIP_UA_3AS;
        t3asNO.m_strDomain = m_strDomain;

        return t3asNO;
    }
	TSipURI GetProxyURI() const
	{
		TSipURI tProxyNO;
		tProxyNO.m_strUser = UNIQUE_SIP_UA_PROXY;
		tProxyNO.m_strDomain = m_strDomain;

		return tProxyNO;
	}
    TSipURI GetAlsURI() const
    {
        TSipURI tUri;
        tUri.m_strUser = UNIQUE_SIP_UA_ALS;
        tUri.m_strDomain = m_strDomain;

        return tUri;
    }
    TSipURI GetTvsURI() const
    {
        TSipURI tUri;
        tUri.m_strUser = UNIQUE_SIP_UA_TVS;
        tUri.m_strDomain = m_strDomain;

        return tUri;
    }
    TSipURI GetRcsURI() const
    {
        TSipURI tUri;
        tUri.m_strUser = UNIQUE_SIP_UA_RCS;
        tUri.m_strDomain = m_strDomain;

        return tUri;
    }
    TSipURI GetUasURI() const
    {
        TSipURI tUri;
        tUri.m_strUser = UNIQUE_SIP_UA_UAS;
        tUri.m_strDomain = m_strDomain;

        return tUri;
    }
    TSipURI GetMpsURI() const
    {
        TSipURI tUri;
        tUri.m_strUser = UNIQUE_SIP_UA_MPS;
        tUri.m_strDomain = m_strDomain;

        return tUri;
    }

    const string& GetUser() const
    {
        return m_strUser;
    }

    string& GetUser()
    {
        return m_strUser;
    }

    void SetUser(string strUser)
    {
        m_strUser = strUser;
    }

    const string& GetDomain() const
    {
        return m_strDomain;
    }

    string& GetDomain()
    {
        return m_strDomain;
    }

    void SetDomain(string strDomain)
    {
        m_strDomain = strDomain;
    }

    string GetURIString() const
    {
        return (m_strUser+"@"+m_strDomain);
    }

#define GetNOString() GetURIString().c_str()

    bool SetNOString(const char* szURI)
    {
        if (szURI == NULL)
        {
            SetNull();
            return true;
        }

        return SetNOString(string(szURI));
    }

    bool SetNOString(const string& strURI)
    {
        if (strURI.empty())
        {
            SetNull();
            return true;
        }

        string::size_type strPos = strURI.find('@');

        if (strPos == string::npos)
        {
            return false;
        }

        m_strUser.assign(strURI, 0, strPos);
        m_strDomain.assign(strURI, strPos+1, strURI.size()-strPos-1);

        return true;
    }

    bool SetURIString(const string& strURI)
    {
        return SetNOString(strURI);
    }

private:
    string m_strUser;
    string m_strDomain;
};

template <> struct CHashFunctor<TSipURI>
{
    size_t operator()(const TSipURI& key) const
    {
        string strKey = key.GetUser() + key.GetDomain();
        return hash_ms(strKey.c_str(), 0);
    }
};

struct TOspSipConf
{
    TOspSipConf()
    {
        dwDefaultOspIID = INVALID_IID;
    }

    TSipURI tLocalURI;
    TSipAddr tLocalAddr;
    TSipURI tProxyURI;
    TSipAddr tProxyAddr;
    u32 dwDefaultOspIID;
};

//��ȡkdsip��Ҫ��������
#define SECTION_KDSIP                  ( "KDSIP" )             //KDSIP������
#define KEY_KDSIP_MAX_TRANS            ( "MaxTrans" )          //kdsip�����������: MaxTrans>(MaxRegDlg+MaxInviteDlg+MaxSubscribeDlg)
#define KEY_KDSIP_MAX_REG_DLG          ( "MaxRegDlg" )         //kdsip���ע��Ի�����
#define KEY_KDSIP_MAX_INVITE_DLG       ( "MaxInviteDlg" )      //kdsip�����жԻ�����
#define KEY_KDSIP_MAX_SUBSCRIBE_DLG    ( "MaxSubscribeDlg" )   //kdsip����ĶԻ�����
#define KEY_KDSIP_DISABLE_CHECKSUM     ( "DisableCheckSum" )   //kdsipȡ�����IPͷУ���

#define DEFAULT_KDSIP_MAX_TRANS            (128)     
#define DEFAULT_KDSIP_MAX_REG_DLG          (1)        
#define DEFAULT_KDSIP_MAX_INVITE_DLG       (1)     
#define DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG    (1) 
#define DEFAULT_KDSIP_DISABLE_CHECKSUM     (FALSE) 

struct TKdsipKeyCfg
{
    TKdsipKeyCfg()
    {
        nMaxTrans = DEFAULT_KDSIP_MAX_TRANS;
        nMaxRegDlg = DEFAULT_KDSIP_MAX_REG_DLG;
        nMaxInviteDlg = DEFAULT_KDSIP_MAX_INVITE_DLG;
        nMaxSubscribeDlg = DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG;
        bDisableCheckSum = DEFAULT_KDSIP_DISABLE_CHECKSUM;
    }

    s32 nMaxTrans;
    s32 nMaxRegDlg;
    s32 nMaxInviteDlg;
    s32 nMaxSubscribeDlg;
    BOOL32 bDisableCheckSum;
};

struct TSipStackConf
{
    TSipStackConf()
    {
        tKdSipConfig.nMaxTrans = DEFAULT_KDSIP_MAX_TRANS; 
        tKdSipConfig.nMaxRegisterDialog = DEFAULT_KDSIP_MAX_REG_DLG;
        tKdSipConfig.nMaxInviteDialog = DEFAULT_KDSIP_MAX_INVITE_DLG;
        tKdSipConfig.nMaxSubscribeDialog = DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG;
        //tKdSipConfig.bDisableCheckSum = DEFAULT_KDSIP_DISABLE_CHECKSUM;
    }

    //��ȡkeda��ini��ʽ�������ļ�ֱ��ʹ�øýӿڣ�����Ҫ�ٵ���SetKeyCfg()
    bool ReadKeyCfg(const s8* szConfPath);

    //��ȡ������ʽ�������ļ����ϲ����ж����ȡ������������Ϣ��ȡ��TKdsipKeyCfg�У��ٵ���SetKeyCfg()
    void SetKeyCfg(const TKdsipKeyCfg& tKeyCfg);

	TKDSIP_INIT_CONFIG tKdSipConfig;
};


#define  MAX_SIP_URI_LEN   (255)
struct TOspSipMsgHead
{
public:
    TOspSipMsgHead()
    {
        Clear();
    }

    ~TOspSipMsgHead()
    {
        Clear();
    }

    void Clear()
    {
        //��Ϣ�峤��
        m_wMsgBodyLen = 0;

        //OSP ��Ϣ
        m_wOspEventId = INVALID_OSP_EVENT;
        m_dwOspIID = INVALID_IID;
        m_dwOspTaskNO = INVALID_TASKNO;
        m_dwOspSeqNO = INVALID_SEQ_NO;

        //SIP ��Ϣ
        m_eSipEvent = KDSIP_EVENT_UNKNOWN;
        m_tSipDlgId = INVALID_DIALOG_ID;
        m_tSipTransId = INVALID_TRANS_ID;
        m_eSipErrorCode = KDSIP_RESPONSE_200_OK;
		memset(m_szFromUri, 0 , MAX_SIP_URI_LEN+1);
		memset(m_szToUri, 0 , MAX_SIP_URI_LEN+1);
    }

public:

    //��Ϣ�峤��
    u16  m_wMsgBodyLen;

    //OSP ��Ϣ
    u16  m_wOspEventId;
    u32  m_dwOspIID;
    u32  m_dwOspTaskNO;
    u32  m_dwOspSeqNO;

    //SIP ��Ϣ
    EKDSIP_EVENT         m_eSipEvent;
    TKDSIP_DIALOG_ID     m_tSipDlgId;
    TKDSIP_TRANS_ID      m_tSipTransId;
    EKDSIP_RESPONSE_CODE m_eSipErrorCode;
	char                 m_szFromUri[MAX_SIP_URI_LEN+1];
	char                 m_szToUri[MAX_SIP_URI_LEN+1];
};

#define OSP_SIP_MSG_MAX_LEN (u16)(0xFFFF)

#define OSP_SIP_MSG_BODY_OFFSET sizeof(TOspSipMsgHead)

#define OSP_SIP_MSG_BODY_MAX_LEN (OSP_SIP_MSG_MAX_LEN - OSP_SIP_MSG_BODY_OFFSET)

class COspSipMsg
{
public:
    COspSipMsg();
    COspSipMsg(u8 * const pbyMsg, u16 wMsgLen);
    COspSipMsg(const COspSipMsg& cMsg);
    COspSipMsg& operator= (const COspSipMsg& cMsg);

    u16 GetOspEventID() const;
    void SetOspEventID(u16 wEventID);
    void SetOspIID(u32 dwOspIID);
    u32 GetOspIID() const;
    u32 GetOspTaskNO() const;
    void SetOspTaskNO(u32 dwTaskNO);
    u32 GetOspSeqNO() const;
    void SetOspSeqNO(u32 dwSeqNO);
    TOspTransID GetOspTransID() const;
    void SetOspTransID(const TOspTransID& tOspTransID);

    EKDSIP_EVENT GetSipEventID() const;
    const string GetSipEventStr() const;
    void SetSipEventID(EKDSIP_EVENT eEventID);
    EKDSIP_EVENT GetSipReq() const;
    EKDSIP_EVENT GetSipRsp() const;
    void SetSipDlgID(TKDSIP_DIALOG_ID tSipId);
    TKDSIP_DIALOG_ID GetSipDlgID() const;
    void SetSipTransID(TKDSIP_TRANS_ID tSipTransId);
    TKDSIP_TRANS_ID GetSipTransID() const;
    EKDSIP_RESPONSE_CODE GetSipErrorCode() const;
    void SetSipErrorCode(EKDSIP_RESPONSE_CODE eErrorCode);

	TSipURI GetSipFromUri() const;
	void SetSipFromUri(LPCSTR szSipUri);
	TSipURI GetSipToUri() const;
	void SetSipToUri(LPCSTR szSipUri);

    void ClearMsgBody(void);
    LPCSTR GetMsgBody() const;
    u16 GetMsgBodyLen() const;
    bool SetMsgBody(const void* pMsgBody, u16 wBodyLen);
    u16 GetMsgLen() const;
    const u8* GetMsgBuf() const; //��ȡ������Ϣָ�룬�û������ṩBUFFER

    template<class CMsg>
    void GetMsgBody(CMsg& rMsg) const
    {
        LPCSTR szXml = GetMsgBody();
        rMsg.FromXml(szXml);
    }  

protected:
	TOspSipMsgHead* GetMsgHead();
	const TOspSipMsgHead* GetMsgHead() const;

protected:
    u8   m_abyBuffer[OSP_SIP_MSG_MAX_LEN];        //��ϢBuffer��28K
};

class CSipTask : public CTask
{
public:
	CSipTask(CInstExt *pcInst) : CTask(pcInst){};
	virtual ~CSipTask(){};

    virtual u32 ProcMsg(CMessage *const pcMsg)
    {
        TASKLOG(OSPSIP, EVENT_LEV, "Recv Sip Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        CStateProc cProc;
        if ( !GetStateProc(GetState(), cProc) )
        {
            CMS_ASSERT( 0 && "[CTask::ProcMsg]��ȡCStateProcʧ�ܣ��Ƿ����Ǹ���InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.ProcMsg && "[CTask::ProcMsg]ProcMsg����ָ��Ϊ�գ��Ƿ����Ǹ���InitStateMachine" );
        CMS_ASSERT( cProc.ProcErrMsg && "[CTask::ProcMsg]ProcErrMsg����ָ��Ϊ�գ��Ƿ����Ǹ���InitStateMachine" );

        PFProcMsg pfProcMsg = NULL;

        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL || pcMsg->event == OSP_SIP_DIALOG_TERMINATE)
        {
            pfProcMsg = (PFProcMsg)cProc.ProcErrMsg;
        }
        else
        {
            pfProcMsg = (PFProcMsg)cProc.ProcMsg;
        }

        if (pfProcMsg == NULL)
        {
            TASKLOG(ERROR_LEV, "event[%s-%hu]������ָ��Ϊ��\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }

        return (this->*pfProcMsg)(pcMsg);
    }

public:
	/*====================================================================
    ������      : PostReq
    ����        : ��װ��OspSipPostReq�ӿڣ��û��������tOspTrans
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:EKDSIP_EVENT eSIPMethod   ʹ�õ�SIP����������KDSIP_EVENT_MESSAGE_REQ
                CReq &cReq                Ҫ���͵���Ϣ��
                const TSipURI &tDstURI    Ŀ��URI����Ϊ�ǵ�һ�η��ͣ�����Ҫָ��Ŀ��URI
                LPCSTR szDirectUAAddr     UA��UAֱ��ͨ������Ҫ��ò���, ��ʽ  ip:port
    ����ֵ˵��  : �ɹ�:PROCMSG_OK = 0��ʧ��:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    template <class CReq>
    u32 PostReq(EKDSIP_EVENT eSIPMethod, const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();
        tOspTrans.dwSeqNO = cReq.GetSeqNum();

        if ( !OspSipPostReq(eSIPMethod, cReq, tDstURI, tOspTrans, szDirectUAAddr) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL);

    /*====================================================================
    ������      : PostInDlgReq
    ����        : ��װ��OspSipPostInDlgReq�ӿڣ��û��������tOspTrans
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:EKDSIP_EVENT eSIPMethod   ʹ�õ�SIP����������KDSIP_EVENT_UNSUBSCRIBE_REQ
                CReq &cReq                Ҫ���͵���Ϣ��
                TKDSIP_DIALOG_ID tDstDlgId    ����ֻ�ܷ��ͻỰ������tDstDlgId�ɻỰRSP��Ϣ�ṩ
    ����ֵ˵��  : �ɹ�:PROCMSG_OK = 0��ʧ��:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    template <class CReq>
    u32 PostInDlgReq(EKDSIP_EVENT eSIPMethod, const CReq &cReq, TKDSIP_DIALOG_ID tDstDlgId)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();
        tOspTrans.dwSeqNO = cReq.GetSeqNum();

        if ( !OspSipPostInDlgReq(eSIPMethod, cReq, tDstDlgId, tOspTrans) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostInDlgReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, TKDSIP_DIALOG_ID tDstDlgId);

    /*====================================================================
    ������      : PostRsp
    ����        : ��װ��OspSipPostRsp�ӿڣ��û��������tOspTrans
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:EKDSIP_EVENT eSIPMethod   ʹ�õ�SIP����������KDSIP_EVENT_MESSAGE_RSP
                TKDSIP_TRANS_ID tTransId  �ظ�������Ҫ�����Ƿ��ڻỰ��, ֻ�������Żظ�
                CRsp &cRsp                Ҫ���͵���Ϣ��
    ����ֵ˵��  : �ɹ�:PROCMSG_OK = 0��ʧ��:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    template <class CRsp>
    u32 PostRsp(EKDSIP_EVENT eSIPMethod, TKDSIP_TRANS_ID tTransId, const CRsp &cRsp)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();
        tOspTrans.dwSeqNO = cRsp.GetSeqNum();

        if ( !OspSipPostRsp(eSIPMethod, tTransId, cRsp, tOspTrans) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostRsp(EKDSIP_EVENT eSIPMethod, TKDSIP_TRANS_ID tTransId, const string &strMsgRsp);

    /*====================================================================
    ������      : PostMsgReq / PostMsgRsp
    ����        : ��װ��OspSipPostReq�ӿڣ��û��������tOspTrans
                  �󲿷���Ϣ����ͨ��Message��SIP�������ͣ�����ֱ�ӷ�װ�ã�����Ҫ�û��ٹ���
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:CReq &cReq                Ҫ���͵���Ϣ��
                const TSipURI &tDstURI    Ŀ��URI����Ϊ�ǵ�һ�η��ͣ�����Ҫָ��Ŀ��URI
                LPCSTR szDirectUAAddr     UA��UAֱ��ͨ������Ҫ��ò���, ��ʽ  ip:port
    ����ֵ˵��  : �ɹ�:PROCMSG_OK = 0��ʧ��:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    template <class CReq>
    u32 PostMsgReq(const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL)
    {
        return PostReq<CReq>(KDSIP_EVENT_MESSAGE_REQ, cReq, tDstURI, szDirectUAAddr);
    }

    u32 PostMsgReq(const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL);

    /*====================================================================
    ������      : PostMsgRsp
    ����        : ��װ��OspSipPostRsp�ӿڣ��û��������tOspTrans
                  �󲿷���Ϣ����ͨ��Message��SIP�������ͣ�����ֱ�ӷ�װ�ã�����Ҫ�û��ٹ���
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:TKDSIP_TRANS_ID tTransId  �ظ�������Ҫ�����Ƿ��ڻỰ��, ֻ�������Żظ�
                CRsp &cRsp                Ҫ���͵���Ϣ��
    ����ֵ˵��  : �ɹ�:PROCMSG_OK = 0��ʧ��:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    template <class CRsp>
    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const CRsp &cRsp)
    {
        return PostRsp<CRsp>(KDSIP_EVENT_MESSAGE_RSP, tTransId, cRsp);
    }

    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const string &strMsgRsp);

    /*====================================================================
    ������      : SetHBParam
    ����        : ��װ��OspSipSetHBParam�ӿڣ��û��������tOspTrans
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    bool SetHBParam(const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, u16 wHbTime = 10, u16 wHbCnt = 3);

    /*====================================================================
    ������      : StopHB
    ����        : ��װ��OspSipStopHB�ӿ�
    ����ȫ�ֱ���:
    �������˵��:
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
    2011/10/28/             liangli
    ====================================================================*/
    bool StopHB(const TSipURI& tHbDst)
    {
        return OspSipStopHB(tHbDst);
    }
};

#endif  //#ifndef _OSP_SIP_H_