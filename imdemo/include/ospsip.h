
#ifndef _OSP_SIP_H_
#define _OSP_SIP_H_

#include "cms/ospext/ospext.h"
#include "cbb/sip/kdsip.h"
#include <string>
using std::string;
#include <vector>
using std::vector;

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

    //������Ϣͨ���¼�����
    OSP_SIP_GB28181_MSG = OSP_SIP_EVENT_BEGIN + 499,   //��Ϣ�壺���ֹ�����Ϣ��(�ַ���)��������Ϣ���ϲ����
    OSP_SIP_EV_END = OSP_SIP_EVENT_BEGIN + 500,
};

#define USER_AGENT_KEDACOM     ("kedacom")
#define DEFAULT_SIP_REG_EXPIRE (3600)

//����Ψһ�豸����
#define UNIQUE_SIP_UA_3AS       ("3as")
#define UNIQUE_SIP_UA_CMU       ("cmu")
#define UNIQUE_SIP_UA_PROXY     ("proxy")
#define UNIQUE_SIP_UA_RCS       ("rcs")
#define UNIQUE_SIP_UA_TVS       ("tvs")
#define UNIQUE_SIP_UA_ALS       ("als")
#define UNIQUE_SIP_UA_UAS       ("uas")
#define UNIQUE_SIP_UA_MPS       ("mps")
#define UNIQUE_SIP_UA_GBS       ("gbs")
#define UNIQUE_SIP_UA_DPS       ("dps")
#define UNIQUE_SIP_UA_NRU       ("nru")
#define UNIQUE_SIP_UA_IM		("im")

struct TSipAddr;
struct TSipExtraInfo;
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
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const T& tMsg, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostReq(eSipEvent, strBuf, tDstURI, tSrcOspTrans, szDirectUAAddr, ptSipExtraInfo);
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
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const T& tMsg, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostInDlgReq(eSipEvent, strBuf, tDstDlgId, tSrcOspTrans, ptSipExtraInfo);
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
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const T& tMsg,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostRsp(eSipEvent, tTransId, strBuf, tSrcOspTrans, eRspCode, ptSipExtraInfo);
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

/*====================================================================
������      : OspSipGetUserNameFromAuthInfo
����        : �Ӽ�Ȩ�ַ����л�ȡ�û���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              strAuthInfo: �����ļ�Ȩ�ַ���
              eSipEvent: ��Ҫ��Ȩ��SIP����
����ֵ˵��  : �û������ձ�ʾ��ȡʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/11/27  0.1         fanxg
====================================================================*/
string OspSipGetUserNameFromAuthInfo(const string& strAuthInfo, EKDSIP_EVENT eSipEvent = KDSIP_EVENT_REGISTER_REQ);

/*====================================================================
������      : OspSipAuthenticate
����        : ���ڷ�����SIPע��ļ�Ȩ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              strAuthInfo: �����ļ�Ȩ�ַ���
              strPassword: �����������ĳ���û�������(����)
              eSipEvent: ��Ҫ��Ȩ��SIP����
����ֵ˵��  : true:��Ȩ�ɹ�; false:��Ȩʧ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/11/27  0.1         fanxg
====================================================================*/
bool OspSipAuthenticate(const string& strAuthInfo, const string& strPassword, EKDSIP_EVENT eSipEvent = KDSIP_EVENT_REGISTER_REQ);


//ospsip���Դ�ӡ����

//ospsip����
API void ospsiphelp();

//ospsip�汾
API void ospsipver();

//ospsip����
API void ospsipcfg();

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

//��ʾ·����Ϣ
API void ospsiproute(const char* szUaName);

//����kdsip��ӡ����
API void kdsiplog(u8 byLogLev, u8 byIsKdSipOnTelnet = 0);

//��ӡkdsip�������Ϣ
API void kdsipshow();


//��ȡkdsip����Դ��Ϣ
bool OspSipGetKdsipResInfo(OUT TKDSIP_DIALOG_RESOURCE& tInfo, IN EKDSIP_DIALOG_TYPE& eType);

/*����NAT��͸�Ĺ��ܽӿ�*/
struct TOspSipNatData
{
    enum ProtoType
    {
        udp = 1,
        tcp = 2
    };
    TOspSipNatData()
    {
        proto = udp;
        intraNetPortBegin = 0;
        intraNetPortEnd = 0;
        extraNetPortBegin = 0;
        extraNetPortEnd = 0;
    }

    ProtoType proto;
    string intraNetIp;
    u16 intraNetPortBegin;
    u16 intraNetPortEnd;
    string extraNetIp;
    u16 extraNetPortBegin;
    u16 extraNetPortEnd;
    string desc;
};
void OspSipGbNatSetData(const string &strNatId, const vector<TOspSipNatData> &vecNatData); // vecNatDataΪ���ȫ������
bool OspSipGbNatDelData(const string &strNatId); // û���κ�һ��gbua����ʱ����ɾ��
bool OspSipGbNatSetGbua(const string &strGbua, const string &strNatId); 
void OspSipGbNatDelGbua(const string &strGbua);
void OspSipGbNatClearAllData();


struct TSipAddr
{
    TSipAddr()
    {
        wPort = 0;
    }

    void SetSipAddrStr(const char* szAddr)
    {
        if (szAddr == NULL)
        {
            return;
        }

        char szIp[64] = {0};
        int nPort = 0;
        sscanf(szAddr, "%[^:]:%d", szIp, &nPort);

        tIpAddr = szIp;
        wPort = u16(nPort);
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

struct TSipExtraInfo
{
    TSipExtraInfo()
    {
        eSipContentType = KDSIP_CONTENT_XML;
        dwRegExpire = DEFAULT_SIP_REG_EXPIRE;
        natProto = TOspSipNatData::udp;
    }

    EKDSIP_CONTENT_TYPE eSipContentType;   //sip��Ϣ������, gbs�����У������������ָ��
    string strDate;      //GB28181��չ�����ڻظ��¼�REGISTERӦ���е���չ��ʱ
    u32    dwRegExpire;  //SIPע�ᵽ��ʱ��(��λ����)
    string strRealm;     //����SIPע�᷽ѡ����ʵ��û��������룬Ӧ��401ʱ��ҪЯ��
    string strUserName;  //�������ϼ�GBע��ʱ��д���û���
    string strPassword;  //�������ϼ�GBע��ʱ��д������
    string strSubject;   //GB28181��չ������INVITE����ͷ���У���ʶ������Դ��Ŀ��
	string strFromUri;   //GB28181:����INVITEʱҵ��ָ����ӦFrom�ֶε�URI
    string strContactUri;//GB28181:����ע���Ӧָ����Ӧ��contact�ֶ�

    string strGbua;      //�Զ�UA�����ڹ���NAT��͸ʱ��ַת��
    TOspSipNatData::ProtoType natProto;//NATת��ʱʹ�õ�Э������ 
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
    TSipURI(const string& strUser, const string& strDomain)
    {
        m_strUser = strUser;
        m_strDomain = strDomain;
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
        //user��domain�в����ٰ���@
        if(m_strUser.find('@') != string::npos || m_strDomain.find('@') != string::npos)
        {
            return false;
        }

        //user��domain�в���Ϊ��
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
	TSipURI GetGbsURI() const
	{
		TSipURI tUri;
		tUri.m_strUser = UNIQUE_SIP_UA_GBS;
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

enum
{
    UA_ROUTE_MODE_U2U = 0,
    UA_ROUTE_MODE_UPU = 1,

};

typedef string TSipUAType;
#define UA_TYPE_INVALID          ("")
#define UA_TYPE_CMU              ("cmu")
#define UA_TYPE_VTDU             ("vtdu")
#define UA_TYPE_RCS              ("rcs")
#define UA_TYPE_CUI              ("cui")
#define UA_TYPE_PUI              ("pui")
#define UA_TYPE_NRU              ("nru")
#define UA_TYPE_TAS              ("tas")
#define UA_TYPE_ALS              ("als")
#define UA_TYPE_TVS              ("tvs")
#define UA_TYPE_UAS              ("uas")
struct TOspSipConf
{
    TOspSipConf() : tUAType(UA_TYPE_INVALID)
    {
        dwDefaultOspIID = INVALID_IID;
        nUaRouteMode = UA_ROUTE_MODE_U2U;
    }

    TSipUAType tUAType;    // ua������
    TSipURI tLocalURI;     //����CMS URI
    TSipAddr tLocalAddr;   //����CMS ��ַ
    TSipURI tLocalGbURI;   //���ع���URI�����ӹ���ʱ���Բ���
    TSipAddr tLocalGbAddr; //���ع����ַ�����ӹ���ʱ���Բ���
    TSipURI tProxyURI;
    TSipAddr tProxyAddr;
    u32 dwDefaultOspIID;
    s32 nUaRouteMode;
};

//��ȡkdsip��Ҫ��������
#define SECTION_KDSIP                  ( "KDSIP" )             //KDSIP������
#define KEY_KDSIP_MAX_TRANS            ( "MaxTrans" )          //kdsip�����������: MaxTrans>(MaxRegDlg+MaxInviteDlg+MaxSubscribeDlg)
#define KEY_KDSIP_MAX_REG_DLG          ( "MaxRegDlg" )         //kdsip���ע��Ի�����
#define KEY_KDSIP_MAX_INVITE_DLG       ( "MaxInviteDlg" )      //kdsip�����жԻ�����
#define KEY_KDSIP_MAX_SUBSCRIBE_DLG    ( "MaxSubscribeDlg" )   //kdsip����ĶԻ�����
#define KEY_KDSIP_MAX_MAX_NTF_PER_SUB  ( "MaxNtfPerSub" )      //ÿ�����ĻỰ��ntf���еĴ�С
#define KEY_KDSIP_DISABLE_CHECKSUM     ( "DisableCheckSum" )   //kdsipȡ�����IPͷУ���

#define KEY_KDSIP_ROUTE_MODE           ( "RouteMode" )         //sip��·��ģ��:U2U��UPU

#define DEFAULT_KDSIP_MAX_TRANS            (128)     
#define DEFAULT_KDSIP_MAX_REG_DLG          (1)        
#define DEFAULT_KDSIP_MAX_INVITE_DLG       (1)     
#define DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG    (1) 
#define DEFAULT_KDSIP_MAX_NTF_PER_SUB      (5)
#define DEFAULT_KDSIP_DISABLE_CHECKSUM     (FALSE) 
#define DEFAULT_KDSIP_ROUTE_MODE                   (UA_ROUTE_MODE_U2U)

struct TKdsipKeyCfg
{
    TKdsipKeyCfg()
    {
        nMaxTrans = DEFAULT_KDSIP_MAX_TRANS;
        nMaxRegDlg = DEFAULT_KDSIP_MAX_REG_DLG;
        nMaxInviteDlg = DEFAULT_KDSIP_MAX_INVITE_DLG;
        nMaxSubscribeDlg = DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG;
        nNumOfNotifysPerSub = DEFAULT_KDSIP_MAX_NTF_PER_SUB;
        bDisableCheckSum = DEFAULT_KDSIP_DISABLE_CHECKSUM;
        nSipRouteMode = UA_ROUTE_MODE_U2U;
    }

    s32 nMaxTrans;                //nMaxTrans>(nMaxRegDlg+nMaxInviteDlg+nMaxSubscribeDlg)
    s32 nMaxRegDlg;               //��СֵΪ1
    s32 nMaxInviteDlg;            //��СֵΪ1
    s32 nMaxSubscribeDlg;         //��СֵΪ1
    s32 nNumOfNotifysPerSub;      //��СֵΪ5
    BOOL32 bDisableCheckSum;      //Ĭ��Ϊ0��·��������������1
    u32 nSipRouteMode;            //sip��·��ģ��:U2U��UPU
};

struct TSipStackConf
{
    TSipStackConf() : tKdSipConfig(TKDSIP_INIT_CONFIG())
    {
        tKdSipConfig.nMaxTrans = DEFAULT_KDSIP_MAX_TRANS; 
        tKdSipConfig.nMaxRegisterDialog = DEFAULT_KDSIP_MAX_REG_DLG;
        tKdSipConfig.nMaxInviteDialog = DEFAULT_KDSIP_MAX_INVITE_DLG;
        tKdSipConfig.nMaxSubscribeDialog = DEFAULT_KDSIP_MAX_SUBSCRIBE_DLG;
        tKdSipConfig.nNumOfNotifysPerSub = DEFAULT_KDSIP_MAX_NTF_PER_SUB;
        tKdSipConfig.bUDPChecksumDisable = DEFAULT_KDSIP_DISABLE_CHECKSUM;
		tKdSipConfig.nMsgSizeMax = 48*1024; //Ĭ�����õײ���Ϣ����󳤶�Ϊ48K
		nSipRouteMode = UA_ROUTE_MODE_U2U;
    }

    //��ȡkeda��ini��ʽ�������ļ�ֱ��ʹ�øýӿڣ�����Ҫ�ٵ���SetKeyCfg()
    bool ReadKeyCfg(const s8* szConfPath);

    //��ȡ������ʽ�������ļ����ϲ����ж����ȡ������������Ϣ��ȡ��TKdsipKeyCfg�У��ٵ���SetKeyCfg()
    void SetKeyCfg(const TKdsipKeyCfg& tKeyCfg);

	TKDSIP_INIT_CONFIG tKdSipConfig;
    u32 nSipRouteMode;            //sip��·��ģ��:U2U��UPU
};


#define  MAX_SIP_URI_LEN        (256)
#define  MAX_SIP_AUTH_INFO_LEN  (512)
#define  MAX_SIP_SUBJECT_LEN    (512)
#define  MAX_SIP_DATE_LEN       (64)
#define  MAX_SIP_REALM_LEN      (64)
#define  MAX_SIP_CALLID_LEN     (128)
#define  MAX_SIP_IPADDR_LEN     (32)
#define  MAX_SIP_USERAGENT_LEN  (64)
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
        m_eSipContentType = KDSIP_CONTENT_UNKNOWN;
        memset(m_szRequestUri, 0 , MAX_SIP_URI_LEN+1);
		memset(m_szFromUri, 0 , MAX_SIP_URI_LEN+1);
		memset(m_szToUri, 0 , MAX_SIP_URI_LEN+1);
        memset(m_szContactUri, 0 , MAX_SIP_URI_LEN+1);
        memset(m_szSipAuthInfo, 0 , MAX_SIP_AUTH_INFO_LEN+1);
        memset(m_szSipRealm, 0 , MAX_SIP_REALM_LEN+1);
        memset(m_szSipSubject, 0 , MAX_SIP_SUBJECT_LEN+1);
        memset(m_szSipDate, 0 , MAX_SIP_DATE_LEN+1);
        m_dwSipExpire = DEFAULT_SIP_REG_EXPIRE;
        memset(m_szSipCallID, 0 , MAX_SIP_CALLID_LEN+1);
        memset(m_szSipLocalIp, 0 , MAX_SIP_IPADDR_LEN+1);
        m_wSipLocalPort = 0;
        memset(m_szSipRemoteIp, 0 , MAX_SIP_IPADDR_LEN+1);
        m_wSipRemotePort = 0;
		memset(m_szUserAgent, 0, MAX_SIP_USERAGENT_LEN+1);
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
    EKDSIP_CONTENT_TYPE  m_eSipContentType;

    char                 m_szRequestUri[MAX_SIP_URI_LEN+1];
	char                 m_szFromUri[MAX_SIP_URI_LEN+1];
	char                 m_szToUri[MAX_SIP_URI_LEN+1];
    char                 m_szContactUri[MAX_SIP_URI_LEN+1];
    char                 m_szSipAuthInfo[MAX_SIP_AUTH_INFO_LEN+1];
    char                 m_szSipRealm[MAX_SIP_REALM_LEN+1];
    char                 m_szSipSubject[MAX_SIP_SUBJECT_LEN+1];
    char                 m_szSipDate[MAX_SIP_DATE_LEN+1];
    u32                  m_dwSipExpire;
    char                 m_szSipCallID[MAX_SIP_CALLID_LEN+1];
    char                 m_szSipLocalIp[MAX_SIP_IPADDR_LEN+1];
    u16                  m_wSipLocalPort;
    char                 m_szSipRemoteIp[MAX_SIP_IPADDR_LEN+1];
    u16                  m_wSipRemotePort;
	char                 m_szUserAgent[MAX_SIP_USERAGENT_LEN+1];
    
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
    EKDSIP_CONTENT_TYPE GetSipContentType() const;
    void SetSipContentType(EKDSIP_CONTENT_TYPE eContentType);

    TSipURI GetSipRequestUri() const;
    void SetSipRequestUri(LPCSTR szSipUri);
	TSipURI GetSipFromUri() const;
	void SetSipFromUri(LPCSTR szSipUri);
	TSipURI GetSipToUri() const;
	void SetSipToUri(LPCSTR szSipUri);
    TSipURI GetSipContactUri() const;
    void SetSipContactUri(LPCSTR szSipUri);

    string GetSipAuthInfo() const;
    void SetSipAuthInfo(LPCSTR szSipAuInfo);
    string GetSipRealm() const;
    void SetSipRealm(LPCSTR szSipRealm);   
    string GetSipSubject() const;
    void SetSipSubject(LPCSTR szSipSubject);
    string GetSipDate() const;
    void SetSipDate(LPCSTR szSipDate);
    u32 GetSipExpire() const;
    void SetSipExpire(u32 dwSipExpire);

    string GetSipCallID() const; 
    void SetSipCallID(LPCSTR szSipCallID);

	string GetUserAgent() const;
	void SetUserAgent(LPCSTR szUserAgent);


    string GetSipLocalIp() const; 
    void SetSipLocalIp(LPCSTR szSipIp);
    u16 GetSipLocalPort() const;
    void SetSipLocalPort(u16 wSipPort);

    string GetSipRemoteIp() const; 
    void SetSipRemoteIp(LPCSTR szSipIp);
    u16 GetSipRemotePort() const;
    void SetSipRemotePort(u16 wSipPort);

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

public:
	//������Ҫ��д�ķ��������ȱʡ��ֹ����taskδ��д�÷��������е���Log�ӿ��������� --Add by Royan 20180530
	virtual const char* GetObjName() const { return "CSipTask"; }; //���븲�ǵķ���
	virtual const char* GetStateName(u32 dwState) const { return "CSipTaskUnknown"; }; //���븲�ǵķ���

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
    u32 PostReq(EKDSIP_EVENT eSIPMethod, const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();

        if (ptSipExtraInfo == NULL)
        {
            tOspTrans.dwSeqNO = cReq.GetSeqNum();
        }
        
        if ( !OspSipPostReq(eSIPMethod, cReq, tDstURI, tOspTrans, szDirectUAAddr, ptSipExtraInfo) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL);

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
    u32 PostInDlgReq(EKDSIP_EVENT eSIPMethod, const CReq &cReq, TKDSIP_DIALOG_ID tDstDlgId, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();

        if (ptSipExtraInfo == NULL)
        {
            tOspTrans.dwSeqNO = cReq.GetSeqNum();
        }       

        if ( !OspSipPostInDlgReq(eSIPMethod, cReq, tDstDlgId, tOspTrans, ptSipExtraInfo) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostInDlgReq(EKDSIP_EVENT eSIPMethod, const string &strMsgReq, TKDSIP_DIALOG_ID tDstDlgId, const TSipExtraInfo* ptSipExtraInfo = NULL);

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
    u32 PostRsp(EKDSIP_EVENT eSIPMethod, TKDSIP_TRANS_ID tTransId, const CRsp &cRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        TOspTransation tOspTrans;
        tOspTrans.dwIID = MAKEIID(GetInstance()->GetAppID(), GetInstance()->GetInsID());
        tOspTrans.dwTaskID = GetTaskNO();

        if (ptSipExtraInfo == NULL)
        {
            tOspTrans.dwSeqNO = cRsp.GetSeqNum();
        }       

        if ( !OspSipPostRsp(eSIPMethod, tTransId, cRsp, tOspTrans, eRspCode, ptSipExtraInfo) )
        {
            return PROCMSG_FAIL;
        }
        return PROCMSG_OK;
    }

    u32 PostRsp(EKDSIP_EVENT eSIPMethod, TKDSIP_TRANS_ID tTransId, const string &strMsgRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

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
    u32 PostMsgReq(const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        return PostReq<CReq>(KDSIP_EVENT_MESSAGE_REQ, cReq, tDstURI, szDirectUAAddr, ptSipExtraInfo);
    }

    u32 PostMsgReq(const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL);

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
    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const CRsp &cRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        return PostRsp<CRsp>(KDSIP_EVENT_MESSAGE_RSP, tTransId, cRsp, eRspCode, ptSipExtraInfo);
    }

    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const string &strMsgRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

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