
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

    //国标消息通用事件类型
    OSP_SIP_GB28181_MSG = OSP_SIP_EVENT_BEGIN + 499,   //消息体：各种国标消息体(字符串)，具体消息由上层解析
    OSP_SIP_EV_END = OSP_SIP_EVENT_BEGIN + 500,
};

#define USER_AGENT_KEDACOM     ("kedacom")
#define DEFAULT_SIP_REG_EXPIRE (3600)

//域内唯一设备名称
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
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostReq(EKDSIP_EVENT eSipEvent, const T& tMsg, const TSipURI& tDstURI, 
    const TOspTransID& tSrcOspTrans, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostReq(eSipEvent, strBuf, tDstURI, tSrcOspTrans, szDirectUAAddr, ptSipExtraInfo);
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
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const string& strMsgBuf, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostInDlgReq(EKDSIP_EVENT eSipEvent, const T& tMsg, TKDSIP_DIALOG_ID tDstDlgId, const TOspTransID& tSrcOspTrans, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostInDlgReq(eSipEvent, strBuf, tDstDlgId, tSrcOspTrans, ptSipExtraInfo);
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
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

template<class T>
bool OspSipPostRsp(EKDSIP_EVENT eSipEvent, TKDSIP_TRANS_ID tTransId, const T& tMsg,
    const TOspTransID& tSrcOspTrans, EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL)
{
    string strBuf = tMsg.ToXml();
    return OspSipPostRsp(eSipEvent, tTransId, strBuf, tSrcOspTrans, eRspCode, ptSipExtraInfo);
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
string OspSipGetUserNameFromAuthInfo(const string& strAuthInfo, EKDSIP_EVENT eSipEvent = KDSIP_EVENT_REGISTER_REQ);

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
bool OspSipAuthenticate(const string& strAuthInfo, const string& strPassword, EKDSIP_EVENT eSipEvent = KDSIP_EVENT_REGISTER_REQ);


//ospsip调试打印命令

//ospsip帮助
API void ospsiphelp();

//ospsip版本
API void ospsipver();

//ospsip配置
API void ospsipcfg();

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

//显示路由信息
API void ospsiproute(const char* szUaName);

//设置kdsip打印级别
API void kdsiplog(u8 byLogLev, u8 byIsKdSipOnTelnet = 0);

//打印kdsip库基本信息
API void kdsipshow();


//获取kdsip的资源信息
bool OspSipGetKdsipResInfo(OUT TKDSIP_DIALOG_RESOURCE& tInfo, IN EKDSIP_DIALOG_TYPE& eType);

/*国标NAT穿透的功能接口*/
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
void OspSipGbNatSetData(const string &strNatId, const vector<TOspSipNatData> &vecNatData); // vecNatData为表的全部数据
bool OspSipGbNatDelData(const string &strNatId); // 没有任何一个gbua引用时才能删除
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

    EKDSIP_CONTENT_TYPE eSipContentType;   //sip消息体类型, gbs开发中，这个变量必须指明
    string strDate;      //GB28181扩展：用于回复下级REGISTER应答中的扩展检时
    u32    dwRegExpire;  //SIP注册到期时间(单位：秒)
    string strRealm;     //用于SIP注册方选择合适的用户名和密码，应答401时需要携带
    string strUserName;  //用于向上级GB注册时填写的用户名
    string strPassword;  //用于向上级GB注册时填写的密码
    string strSubject;   //GB28181扩展：用于INVITE请求头域中，标识码流的源和目的
	string strFromUri;   //GB28181:用于INVITE时业务指定相应From字段的URI
    string strContactUri;//GB28181:用于注册答应指定相应的contact字段

    string strGbua;      //对端UA，用于国标NAT穿透时地址转换
    TOspSipNatData::ProtoType natProto;//NAT转换时使用的协议类型 
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
        //user和domain中不能再包含@
        if(m_strUser.find('@') != string::npos || m_strDomain.find('@') != string::npos)
        {
            return false;
        }

        //user和domain中不能为空
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

    TSipUAType tUAType;    // ua的类型
    TSipURI tLocalURI;     //本地CMS URI
    TSipAddr tLocalAddr;   //本地CMS 地址
    TSipURI tLocalGbURI;   //本地国标URI，不接国标时可以不填
    TSipAddr tLocalGbAddr; //本地国标地址，不接国标时可以不填
    TSipURI tProxyURI;
    TSipAddr tProxyAddr;
    u32 dwDefaultOspIID;
    s32 nUaRouteMode;
};

//读取kdsip重要的配置项
#define SECTION_KDSIP                  ( "KDSIP" )             //KDSIP配置项
#define KEY_KDSIP_MAX_TRANS            ( "MaxTrans" )          //kdsip最大事务数量: MaxTrans>(MaxRegDlg+MaxInviteDlg+MaxSubscribeDlg)
#define KEY_KDSIP_MAX_REG_DLG          ( "MaxRegDlg" )         //kdsip最大注册对话数量
#define KEY_KDSIP_MAX_INVITE_DLG       ( "MaxInviteDlg" )      //kdsip最大呼叫对话数量
#define KEY_KDSIP_MAX_SUBSCRIBE_DLG    ( "MaxSubscribeDlg" )   //kdsip最大订阅对话数量
#define KEY_KDSIP_MAX_MAX_NTF_PER_SUB  ( "MaxNtfPerSub" )      //每个订阅会话中ntf队列的大小
#define KEY_KDSIP_DISABLE_CHECKSUM     ( "DisableCheckSum" )   //kdsip取消检测IP头校验和

#define KEY_KDSIP_ROUTE_MODE           ( "RouteMode" )         //sip的路由模型:U2U或UPU

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
    s32 nMaxRegDlg;               //最小值为1
    s32 nMaxInviteDlg;            //最小值为1
    s32 nMaxSubscribeDlg;         //最小值为1
    s32 nNumOfNotifysPerSub;      //最小值为5
    BOOL32 bDisableCheckSum;      //默认为0，路由器有问题是填1
    u32 nSipRouteMode;            //sip的路由模型:U2U或UPU
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
		tKdSipConfig.nMsgSizeMax = 48*1024; //默认设置底层消息体最大长度为48K
		nSipRouteMode = UA_ROUTE_MODE_U2U;
    }

    //读取keda的ini格式的配置文件直接使用该接口，不需要再调用SetKeyCfg()
    bool ReadKeyCfg(const s8* szConfPath);

    //读取其他格式的配置文件，上层自行定义读取方法将配置信息读取到TKdsipKeyCfg中，再调用SetKeyCfg()
    void SetKeyCfg(const TKdsipKeyCfg& tKeyCfg);

	TKDSIP_INIT_CONFIG tKdSipConfig;
    u32 nSipRouteMode;            //sip的路由模型:U2U或UPU
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

public:
	//子类需要重写的方法；添加缺省防止子类task未重写该方法析构中调用Log接口引发崩溃 --Add by Royan 20180530
	virtual const char* GetObjName() const { return "CSipTask"; }; //必须覆盖的方法
	virtual const char* GetStateName(u32 dwState) const { return "CSipTaskUnknown"; }; //必须覆盖的方法

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
    u32 PostMsgReq(const CReq &cReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        return PostReq<CReq>(KDSIP_EVENT_MESSAGE_REQ, cReq, tDstURI, szDirectUAAddr, ptSipExtraInfo);
    }

    u32 PostMsgReq(const string &strMsgReq, const TSipURI &tDstURI, LPCSTR szDirectUAAddr = NULL, const TSipExtraInfo* ptSipExtraInfo = NULL);

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
    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const CRsp &cRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL)
    {
        return PostRsp<CRsp>(KDSIP_EVENT_MESSAGE_RSP, tTransId, cRsp, eRspCode, ptSipExtraInfo);
    }

    u32 PostMsgRsp(TKDSIP_TRANS_ID tTransId, const string &strMsgRsp, 
        EKDSIP_RESPONSE_CODE eRspCode = KDSIP_RESPONSE_200_OK, const TSipExtraInfo* ptSipExtraInfo = NULL);

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