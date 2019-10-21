
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

    //SIP断链通知
    OSP_SIP_DISCONNECT,      //消息体: szSipUri(char* 字符串, 心跳对端的sip uri)

    //Proxy断链通知
    OSP_PROXY_DISCONNECT,    //消息体: 无

	//Proxy注册成功通知
	OSP_PROXY_CONNECT,       //消息体: 无

    //SIP BYE请求
    OSP_SIP_BYE_REQ,    //消息体: 透传BYE方法中的消息体

    //SIP BYE应答
    OSP_SIP_BYE_RSP,       //消息体: 无

    //SIP层处理错误
    OSP_SIP_MSG_PROC_FAIL,   //消息体: 无

	//SIP层Dialog异常终止通知
	OSP_SIP_DIALOG_TERMINATE,//消息体: 无

    //ospsip通知上层重新加载配置文件
    OSP_SIP_SIGNAL_RELOAD_CFG,//消息体: 无

    OSP_SIP_EV_END = OSP_SIP_EVENT_BEGIN + 500,
};

//域内唯一设备名称
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
bool OspSipInit(const TOspSipConf& tOspSipConf, const TSipStackConf& tSipStackConf);


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
void OspSipQuit();


/*====================================================================
函数名      : OspSipRegProxy
功能        : 注册到SIP PROXY
算法实现    :
引用全局变量:
输入参数说明:
             dwExpire: The expires value for registration(单位：秒)
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipRegProxy(s32 nExpires);


/*====================================================================
函数名      : OspSipSetHBParam
功能        : 设置OSPSIP心跳参数
算法实现    : 心跳失败，向上层发送 OSP_SIP_DISCONNECT 消息
引用全局变量:
输入参数说明:
             tHbDst: 心跳目的
             tHbSrc: 心跳发起者
             wHbTime: 心跳检测间隔时间(以秒为单位),
             wHbCnt: 心跳检测次数
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/29  0.1         fanxg
====================================================================*/
bool OspSipSetHBParam(const TSipURI& tHbDst, const TOspTransID& tHbSrc, LPCSTR szDirectUAAddr = NULL, u16 wHbTime = 10, u16 wHbCnt = 3);


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
bool OspSipStopHB(const TSipURI& tHbDst);


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
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL);

template<class T>
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const T& tMsg, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostReq(eSipEvent, strBuf, tDstURI, tSrcOspTrans, szDirectUAAddr);
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
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans);

template<class T>
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const T& tMsg, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostInDlgReq(eSipEvent, strBuf, tDstDlgId, tSrcOspTrans);
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
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK);

template<class T>
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const T& tMsg,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostRsp(eSipEvent, tTransId, strBuf, tSrcOspTrans, eRspCode);
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
void OspSipReleaseDlg(TKDSIP_DIALOG_ID tDlgId);


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
const TSipURI OspSipGetLocalURI();

void OspSipSignalHandleCB(int nSig);


//ospsip调试打印命令

//ospsip帮助
API void ospsiphelp();

//ospsip版本
API void ospsipver();

//设置ospsip打印级别
API void ospsiplog(u8 byLogLev);

//显示ospsip心跳列表
API void ospsiphblist();

//显示SN信息
API void ospsipsn();

//显示事务信息
API void ospsiptrans();

//显示income事务信息
API void ospsipin();

//显示out事务信息
API void ospsipout();

//显示对话信息
API void ospsipdlg();

//显示invite对话信息
API void ospsipivt();

//显示subscribe对话信息
API void ospsipss();

//显示register对话信息
API void ospsipreg();

//显示发送队列信息
API void ospsipque();

//设置kdsip打印级别
API void kdsiplog(u8 byLogLev, u8 byIsKdSipOnTelnet = 0);

//打印kdsip库基本信息
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

    //目前来看，唯一性URI放在这里定义和获取似乎不合理，因为这会导致ospsip依赖于上层的业务模块定义，先这样吧
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

//读取kdsip重要的配置项
#define SECTION_KDSIP                  ( "KDSIP" )             //KDSIP配置项
#define KEY_KDSIP_MAX_TRANS            ( "MaxTrans" )          //kdsip最大事务数量: MaxTrans>(MaxRegDlg+MaxInviteDlg+MaxSubscribeDlg)
#define KEY_KDSIP_MAX_REG_DLG          ( "MaxRegDlg" )         //kdsip最大注册对话数量
#define KEY_KDSIP_MAX_INVITE_DLG       ( "MaxInviteDlg" )      //kdsip最大呼叫对话数量
#define KEY_KDSIP_MAX_SUBSCRIBE_DLG    ( "MaxSubscribeDlg" )   //kdsip最大订阅对话数量
#define KEY_KDSIP_DISABLE_CHECKSUM     ( "DisableCheckSum" )   //kdsip取消检测IP头校验和

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

    //读取keda的ini格式的配置文件直接使用该接口，不需要再调用SetKeyCfg()
    bool ReadKeyCfg(const s8* szConfPath);

    //读取其他格式的配置文件，上层自行定义读取方法将配置信息读取到TKdsipKeyCfg中，再调用SetKeyCfg()
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
        //消息体长度
        m_wMsgBodyLen = 0;

        //OSP 信息
        m_wOspEventId = INVALID_OSP_EVENT;
        m_dwOspIID = INVALID_IID;
        m_dwOspTaskNO = INVALID_TASKNO;
        m_dwOspSeqNO = INVALID_SEQ_NO;

        //SIP 信息
        m_eSipEvent = KDSIP_EVENT_UNKNOWN;
        m_tSipDlgId = INVALID_DIALOG_ID;
        m_tSipTransId = INVALID_TRANS_ID;
        m_eSipErrorCode = KDSIP_RESPONSE_200_OK;
		memset(m_szFromUri, 0 , MAX_SIP_URI_LEN+1);
		memset(m_szToUri, 0 , MAX_SIP_URI_LEN+1);
    }

public:

    //消息体长度
    u16  m_wMsgBodyLen;

    //OSP 信息
    u16  m_wOspEventId;
    u32  m_dwOspIID;
    u32  m_dwOspTaskNO;
    u32  m_dwOspSeqNO;

    //SIP 信息
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
    const u8* GetMsgBuf() const; //获取整个消息指针，用户不需提供BUFFER

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
    u8   m_abyBuffer[OSP_SIP_MSG_MAX_LEN];        //消息Buffer，28K
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
            CMS_ASSERT( 0 && "[CTask::ProcMsg]获取CStateProc失败，是否忘记覆盖InitStateMachine" );
            return PROCMSG_FAIL;
        }
        CMS_ASSERT( cProc.ProcMsg && "[CTask::ProcMsg]ProcMsg函数指针为空，是否忘记覆盖InitStateMachine" );
        CMS_ASSERT( cProc.ProcErrMsg && "[CTask::ProcMsg]ProcErrMsg函数指针为空，是否忘记覆盖InitStateMachine" );

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
            TASKLOG(ERROR_LEV, "event[%s-%hu]处理函数指针为空\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            return PROCMSG_FAIL;
        }

        return (this->*pfProcMsg)(pcMsg);
    }

public:
	/*====================================================================
    函数名      : PostReq
    功能        : 封装了OspSipPostReq接口，用户无需关心tOspTrans
    算法实现    :
    引用全局变量:
    输入参数说明:EKDSIP_EVENT eSIPMethod   使用的SIP方法，例如KDSIP_EVENT_MESSAGE_REQ
                CReq &cReq                要发送的消息体
                const TSipURI &tDstURI    目标URI，因为是第一次发送，所以要指定目标URI
                LPCSTR szDirectUAAddr     UA和UA直接通信是需要填该参数, 格式  ip:port
    返回值说明  : 成功:PROCMSG_OK = 0，失败:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
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
    函数名      : PostInDlgReq
    功能        : 封装了OspSipPostInDlgReq接口，用户无需关心tOspTrans
    算法实现    :
    引用全局变量:
    输入参数说明:EKDSIP_EVENT eSIPMethod   使用的SIP方法，例如KDSIP_EVENT_UNSUBSCRIBE_REQ
                CReq &cReq                要发送的消息体
                TKDSIP_DIALOG_ID tDstDlgId    这里只能发送会话内请求，tDstDlgId由会话RSP消息提供
    返回值说明  : 成功:PROCMSG_OK = 0，失败:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
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
    函数名      : PostRsp
    功能        : 封装了OspSipPostRsp接口，用户无需关心tOspTrans
    算法实现    :
    引用全局变量:
    输入参数说明:EKDSIP_EVENT eSIPMethod   使用的SIP方法，例如KDSIP_EVENT_MESSAGE_RSP
                TKDSIP_TRANS_ID tTransId  回复事务不需要关心是否在会话内, 只针对事务号回复
                CRsp &cRsp                要发送的消息体
    返回值说明  : 成功:PROCMSG_OK = 0，失败:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
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
    函数名      : PostMsgReq / PostMsgRsp
    功能        : 封装了OspSipPostReq接口，用户无需关心tOspTrans
                  大部分消息都是通过Message的SIP方法发送，这里直接封装好，不需要用户再关心
    算法实现    :
    引用全局变量:
    输入参数说明:CReq &cReq                要发送的消息体
                const TSipURI &tDstURI    目标URI，因为是第一次发送，所以要指定目标URI
                LPCSTR szDirectUAAddr     UA和UA直接通信是需要填该参数, 格式  ip:port
    返回值说明  : 成功:PROCMSG_OK = 0，失败:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/10/28/             liangli
    ====================================================================*/
    template <class CReq>
    u32 PostMsgReq(const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL)
    {
        return PostReq<CReq>(KDSIP_EVENT_MESSAGE_REQ, cReq, tDstURI, szDirectUAAddr);
    }

    u32 PostMsgReq(const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL);

    /*====================================================================
    函数名      : PostMsgRsp
    功能        : 封装了OspSipPostRsp接口，用户无需关心tOspTrans
                  大部分消息都是通过Message的SIP方法发送，这里直接封装好，不需要用户再关心
    算法实现    :
    引用全局变量:
    输入参数说明:TKDSIP_TRANS_ID tTransId  回复事务不需要关心是否在会话内, 只针对事务号回复
                CRsp &cRsp                要发送的消息体
    返回值说明  : 成功:PROCMSG_OK = 0，失败:PROCMSG_FAIL = 1
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/10/28/             liangli
    ====================================================================*/
    template <class CRsp>
    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const CRsp &cRsp)
    {
        return PostRsp<CRsp>(KDSIP_EVENT_MESSAGE_RSP, tTransId, cRsp);
    }

    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const string &strMsgRsp);

    /*====================================================================
    函数名      : SetHBParam
    功能        : 封装了OspSipSetHBParam接口，用户无需关心tOspTrans
    算法实现    :
    引用全局变量:
    输入参数说明:
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/10/28/             liangli
    ====================================================================*/
    bool SetHBParam(const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, u16 wHbTime = 10, u16 wHbCnt = 3);

    /*====================================================================
    函数名      : StopHB
    功能        : 封装了OspSipStopHB接口
    引用全局变量:
    输入参数说明:
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/10/28/             liangli
    ====================================================================*/
    bool StopHB(const TSipURI& tHbDst)
    {
        return OspSipStopHB(tHbDst);
    }
};

#endif  //#ifndef _OSP_SIP_H_