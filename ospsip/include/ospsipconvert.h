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

//SIP协议栈事务存续时间是32s，这里设定事务绑定关系的超时略大于32s，为35s
#define OSP_SIP_BIND_TIMEOUT 35   //s

//sip资源创建超时，资源创建后会立刻回调，所以超时应该尽可能短
//fanxg-2014-07-22: 经过长时间观察，kdsip有可能资源创建时间超过2s，为了增强容错能力，超时改为20s
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
    函数名      : OspSipPostReq_Inner
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
    bool OspSipPostReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, const TSipURI& tDstURI, 
        const TOspTransID& tSrcOspTrans, const string& strDirectUAAddr, const TSipExtraInfo* ptSipExtraInfo);


    /*====================================================================
    函数名      : OspSipPostInDlgReq_Inner
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
    bool OspSipPostInDlgReq_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, 
        const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo);


    /*====================================================================
    函数名      : OspSipPostRsp_Inner
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
    bool OspSipPostRsp_Inner(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_TRANS_ID tDstTransId,
        const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

    /*====================================================================
    函数名      : ReleaseDialog_Inner
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
    void ReleaseDialog_Inner(TKDSIP_DIALOG_ID tSipDlgId);

    //更新其他UA的路由信息
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

        //注册Proxy对话是唯一，返回为空，重新注册，非空，刷新注册
        TKDSIP_DIALOG_ID GetRegProxyDlg() const;

        bool IsGBOutTrans(TKDSIP_TRANS_ID tSipTransId);
        bool IsGBIncomeTrans(TKDSIP_TRANS_ID tSipTransId);
        bool IsGBDlg(TKDSIP_DIALOG_ID tSipDlgId);

		// 设置contact [1/11/2016 pangubing]
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
    SIP_MSG_TYPE_UNKNOWN = 0,     //未知类型
    SIP_MSG_TYPE_REQ,             //对话外请求
    SIP_MSG_TYPE_REQ_INDLG,       //对话内请求
    SIP_MSG_TYPE_RSP,             //应答消息不区分对话内和对话外
    SIP_MSG_TYPE_CHECKER,         //检查超时
    SIP_MSG_TYPE_RELEASE_DLG,     //释放sip dlg
    SIP_MSG_UPDATE_ROUTE_INFO,    //更新其他UA的路由信息
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
    OSPSIP_CORE_MSG_TYPE_UNKNOWN = 0,     //未知类型
    OSPSIP_CORE_MSG_TYPE_SEND,            //发送消息
    OSPSIP_CORE_MSG_TYPE_RECV,            //接收消息
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