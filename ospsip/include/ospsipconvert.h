#ifndef _OSP_SIP_CONVERT_H_
#define _OSP_SIP_CONVERT_H_

#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospmailbox.h"
#include "cms/cms_struct.h"


typedef string (*PGetSipXmlMsgTypeCB)(const char* szXmlBody);

bool SendMsgToOspApp(const COspSipMsg& cMsg);

bool IsSipMsg(EKDSIP_EVENT eEvType);

bool IsSipReqMsg(EKDSIP_EVENT eEvType);

bool IsSipRspMsg(EKDSIP_EVENT eEvType);

bool IsSipCreateMsg(EKDSIP_EVENT eEvType);

bool IsSipExceptMsg(EKDSIP_EVENT eEvType);

bool IsKedacomSipMsg(const CKDSipMsgHead* pcSipHead);

const string SipEventDesc(s32 nSipEvent);

struct TSipTransaction
{
    TSipTransaction()
    {
        tSipDlg = INVALID_DIALOG_ID;
        tSipTrans = INVALID_TRANS_ID;
    }

    TKDSIP_DIALOG_ID tSipDlg;
    TKDSIP_TRANS_ID  tSipTrans;
};

//SIPЭ��ջ�������ʱ����32s�������趨����󶨹�ϵ�ĳ�ʱ�Դ���32s��Ϊ35s
#define OSP_SIP_BIND_TIMEOUT 35   //s

//sip��Դ������ʱ����Դ����������̻ص������Գ�ʱӦ�þ����ܶ�
//fanxg-2014-07-22: ������ʱ��۲죬kdsip�п�����Դ����ʱ�䳬��2s��Ϊ����ǿ�ݴ���������ʱ��Ϊ20s
#define OSP_SIP_RESOURCE_CREATE_TIMEOUT 20   //s

#define OSP_SIP_INVALID_KDSIP_SN 0

struct TOspSipBind
{
    TOspSipBind()
    {
        time(&tBirthTime);
        eSipEvent = KDSIP_EVENT_UNKNOWN;
        dwKdsipSN = OSP_SIP_INVALID_KDSIP_SN;
    }

    bool IsTimeout() const
    {
        time_t tCurrTime;
        time(&tCurrTime);

        return (tCurrTime - tBirthTime > OSP_SIP_BIND_TIMEOUT);
    }

    bool IsCreateTimeout() const
    {
        time_t tCurrTime;
        time(&tCurrTime);

        return (tCurrTime - tBirthTime > OSP_SIP_RESOURCE_CREATE_TIMEOUT);
    }

    TOspTransaction tOspId;
    TSipTransaction tSipId;
    TSipURI tSipURI;
    time_t tBirthTime;
    EKDSIP_EVENT eSipEvent;
    string strOspEvent;
    u32 dwKdsipSN;
};

typedef CXMap<TKDSIP_TRANS_ID, TOspSipBind> COspSipTransMap;
typedef CXMap<TKDSIP_DIALOG_ID, TOspSipBind> COspSipDlgMap;

typedef CXMap<u32, TOspSipBind> COspSipSNMap;

typedef CXMap<TSipURI, TUaAddrPair> COspSipRouteTable;

class COspSipConvertor
{
public:
    COspSipConvertor();

    bool DoConvert(const CKDSipEvent* pSipEvent);   
    bool SetSipHeadAndBody(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg);
    string GetSipUri(LPCSTR szKdsipUri);
    bool ProcSipReq(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg);
    bool ProcSipRsp(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg);
    bool ProcSipCreate(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg);
    bool ProcSipExcept(const CKDSipEvent* pSipEvent, COspSipMsg& cMsg);

    void SetDefaultOspIID(u32 dwOspIID);
    u32 GetDefaultOspIID() const;

    void CheckTimeout();
    void CheckTransTimeout(COspSipTransMap& cTransMap, bool bIsTimeoutRsp = false);
    void CheckDlgTimeout();
    void CheckSipCreateTimeout();

#define     PRINT_IN_TRANS      (1<<0)
#define     PRINT_OUT_TRANS     (1<<1)
#define     PRINT_ALL_TRANS     (PRINT_IN_TRANS+PRINT_OUT_TRANS)
    void PrintTrans(u32 dwPrintFlag = PRINT_ALL_TRANS) const;

#define     PRINT_IVT_DLG     (1<<0)
#define     PRINT_SS_DLG      (1<<1)
#define     PRINT_REG_DLG     (1<<2)
#define     PRINT_ALL_DLG     (PRINT_IVT_DLG+PRINT_SS_DLG+PRINT_REG_DLG)
    void PrintDialog(u32 dwPrintFlag = PRINT_ALL_DLG) const;

    void PrintSN() const;

    void PrintRoute(const char* szUaName) const;


    /*====================================================================
    ������      : OspSipPostReq_Inner
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
    bool OspSipPostReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, const TSipURI& tDstURI, 
        const TOspTransID& tSrcOspTrans, const string& strDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo);


    /*====================================================================
    ������      : OspSipPostInDlgReq_Inner
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
    bool OspSipPostInDlgReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, 
        const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo);


    /*====================================================================
    ������      : OspSipPostRsp_Inner
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
    bool OspSipPostRsp_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_TRANS_ID tDstTransId,
        const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

    /*====================================================================
    ������      : ReleaseDialog_Inner
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
    void ReleaseDialog_Inner(TKDSIP_DIALOG_ID tSipDlgId);

    //��������UA��·����Ϣ
    void UpdateRouteInfo(const string& strMsgBuf);
    
protected:
        bool ExistOutTrans(TKDSIP_TRANS_ID tSipTransId);
        TOspTransation FindOspIdByOutTrans(TKDSIP_TRANS_ID tSipTransId);
        void InsertOutTrans(const TOspSipBind& tOspSipBind, TKDSIP_TRANS_ID tSipTransId);
        void EraseOutTrans(TKDSIP_TRANS_ID tSipTransId);

        bool ExistIncomeTrans(TKDSIP_TRANS_ID tSipTransId);
        bool FindSipIdByIncomeTrans(TKDSIP_TRANS_ID tSipTransId, TSipTransaction& tSipId);
        void InsertIncomeTrans(const TOspSipBind& tOspSipBind, TKDSIP_TRANS_ID tSipTransId);
        void EraseIncomeTrans(TKDSIP_TRANS_ID tSipTransId);

        bool ExistDlg(TKDSIP_DIALOG_ID tSipDlgId);
        TOspTransation FindDlg(TKDSIP_DIALOG_ID tSipDlgId);
        TOspSipBind FindDlgBind(TKDSIP_DIALOG_ID tSipDlgId);
        void InsertDlg(const TOspSipBind& tOspSipBind, TKDSIP_DIALOG_ID tSipDlgId);
        void EraseDlg(TKDSIP_DIALOG_ID tSipDlgId);

        bool ExistSN(u32 dwSN);
        TOspSipBind FindSNBind(u32 dwSN);
        void InsertSN(const TOspSipBind& tOspSipBind, u32 dwSN);
        void EraseSN(u32 dwSN);

        //ע��Proxy�Ի���Ψһ������Ϊ�գ�����ע�ᣬ�ǿգ�ˢ��ע��
        TKDSIP_DIALOG_ID GetRegProxyDlg() const;

        bool IsGBOutTrans(TKDSIP_TRANS_ID tSipTransId);
        bool IsGBIncomeTrans(TKDSIP_TRANS_ID tSipTransId);
        bool IsGBDlg(TKDSIP_DIALOG_ID tSipDlgId);

		// ����contact [1/11/2016 pangubing]
		void SetContactUri( const TSipExtraInfo* ptSipExtraInfo, CKDSipMsgHead &tSipHead );
private:
    COspSipTransMap m_cOutTransMap;
    COspSipTransMap m_cIncomeTransMap;
    COspSipDlgMap m_cDlgMap;

    COspSipSNMap m_cSNMap;

    COspSipRouteTable m_cRouteTable;

    u32 m_dwDefaultOspIID;

    mutable CXSemLock m_cSemLock;
};

extern COspSipConvertor g_cOspSipConvertor;

enum  ESIP_MSG_TYPE
{
    SIP_MSG_TYPE_UNKNOWN = 0,     //δ֪����
    SIP_MSG_TYPE_REQ,             //�Ի�������
    SIP_MSG_TYPE_REQ_INDLG,       //�Ի�������
    SIP_MSG_TYPE_RSP,             //Ӧ����Ϣ�����ֶԻ��ںͶԻ���
    SIP_MSG_TYPE_CHECKER,         //��鳬ʱ
    SIP_MSG_TYPE_RELEASE_DLG,     //�ͷ�sip dlg
    SIP_MSG_UPDATE_ROUTE_INFO,    //��������UA��·����Ϣ
};

struct TOspSipSendMsg
{
    TOspSipSendMsg()
    {
        eSipMsgType = SIP_MSG_TYPE_UNKNOWN;
        eSipEvent = KDSIP_EVENT_UNKNOWN;
        tDstDlgId = INVALID_DIALOG_ID;
        tDstTransId = INVALID_TRANS_ID;
        eRspCode = KDSIP_RESPONSE_NONE;
        ptSipExtraInfo = NULL;
    }
    ~TOspSipSendMsg()
    {
        if (ptSipExtraInfo != NULL)
        {
            delete ptSipExtraInfo;
            ptSipExtraInfo = NULL;
        }
    }

    ESIP_MSG_TYPE         eSipMsgType;
    EKDSIP_EVENT          eSipEvent;
    TKDSIP_DIALOG_ID      tDstDlgId;
    TKDSIP_TRANS_ID       tDstTransId;
    TSipURI               tDstURI;
    TOspTransID           tSrcOspTrans;
    string                strMsgBuf;
    EKDSIP_RESPONSE_CODE  eRspCode;

    string                 strDirectUAAddr;
    TSipExtraInfo*         ptSipExtraInfo;
};

enum  EOSPSIP_CORE_MSG_TYPE
{
    OSPSIP_CORE_MSG_TYPE_UNKNOWN = 0,     //δ֪����
    OSPSIP_CORE_MSG_TYPE_SEND,            //������Ϣ
    OSPSIP_CORE_MSG_TYPE_RECV,            //������Ϣ
};

struct TOspSipCoreMsg
{
    TOspSipCoreMsg()
    {
        eCoreMsgType = OSPSIP_CORE_MSG_TYPE_UNKNOWN;
        pvMsg = NULL;
    }

    EOSPSIP_CORE_MSG_TYPE         eCoreMsgType;
    void*                         pvMsg;
};

typedef CXMailbox<TOspSipCoreMsg> CSipCoreMsgQue;
extern CSipCoreMsgQue g_cOspSipCoreMsgQue;
bool WriteCoreQue(const TOspSipCoreMsg& tMsg);
TOspSipCoreMsg ReadCoreQue(u32 dwMillSec = INFINITE);


#endif  //#ifndef _OSP_SIP_CONVERT_H_