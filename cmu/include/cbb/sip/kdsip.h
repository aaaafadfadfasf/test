#ifndef _KDSIP_H_
#define _KDSIP_H_


#ifdef _MSC_VER
#include <windows.h>
#endif    // _MSC_VER
#include "kdvtype.h"
#ifndef NULL
#define NULL 0
#endif

typedef void* KDSIP_DIALOG_ID;                 // 抽象统一的KDSIP会话标识
typedef void* KDSIP_TRANS_ID;                  // KDSIP事务标识，用于区分对话内的不同事务

#define KDSIP_VERSION       "KDSIP_V2.2"  // kdsip版本号
#define INVALID_DIALOG_ID   (KDSIP_DIALOG_ID)0 // 无效KDSIP标识
#define INVALID_TRANS_ID    (KDSIP_TRANS_ID)0  // 无效事务标识
#define KDSIP_NO_PORT       (u16)-1            // 无效端口

// TKDSIP_INIT_CONFIG 默认值
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_PATH         "./"
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE              "kdsip.log"
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_SIZE         (2 * 1024 * 1024)
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_NUM          5
#define KDSIP_INIT_CONFIG_DEFAULT_MAXIINVITEDIALOG      1024
#define KDSIP_INIT_CONFIG_DEFAULT_MAXREGISTERDIALOG     128
#define KDSIP_INIT_CONFIG_DEFAULT_MAXSUBSCRIBEDIALOG    1024

#define KDSIP_INIT_CONFIG_DEFAULT_DIALOG_CAP       100000
#define KDSIP_INIT_CONFIG_DEFAULT_TRANS_CAP        400000
#define KDSIP_INIT_CONFIG_DEFAULT_MEM_POOL_CAP     1<<20

#define KDSIP_INIT_CONFIG_DEFAULT_MAXTRANSCATION        1024
#define KDSIP_INIT_CONFIG_DEFAULT_RETRANSMITTIMER       500*8  //4s
#define KDSIP_INIT_CONFIG_DEFAULT_LINGERTIMER           32000  //32s
#define KDSIP_INIT_CONFIG_DEFAULT_MSG_SIZE_MAX          0xC000 //48k
#define KDSIP_INIT_CONFIG_DEFAULT_NUMOFNOTIFYPERSUB     5
// SIP事件类型
enum EKDSIP_EVENT
{
	KDSIP_EVENT_UNKNOWN               = 0,

	KDSIP_EVENT_REGISTER_REQ,
	KDSIP_EVENT_REGISTER_RSP,

	KDSIP_EVENT_UNREGISTER_REQ,
	KDSIP_EVENT_UNREGISTER_RSP,

	KDSIP_EVENT_INVITE_REQ,
	KDSIP_EVENT_INVITE_RSP,
	KDSIP_EVENT_INVITE_ACK,

	KDSIP_EVENT_INVITE_BYE_REQ,
	KDSIP_EVENT_INVITE_BYE_RSP,

	KDSIP_EVENT_INFO_REQ,
	KDSIP_EVENT_INFO_RSP,

	KDSIP_EVENT_OPTIONS_HB_REQ,
	KDSIP_EVENT_OPTIONS_HB_RSP,

	KDSIP_EVENT_MESSAGE_REQ,
	KDSIP_EVENT_MESSAGE_RSP,

	KDSIP_EVENT_MESSAGE_DIALOG_REQ,
	KDSIP_EVENT_MESSAGE_DIALOG_RSP,

	KDSIP_EVENT_SUBSCRIBE_REQ,
	KDSIP_EVENT_SUBSCRIBE_RSP,

	KDSIP_EVENT_UNSUBSCRIBE_REQ,
	KDSIP_EVENT_UNSUBSCRIBE_RSP,

	KDSIP_EVENT_NOTIFY_REQ,
	KDSIP_EVENT_NOTIFY_RSP,

    KDSIP_EVENT_DIALOG_TERMINATE,
    KDSIP_EVENT_TRANSACTION_TIMEOUT,

    KDSIP_EVENT_DIALOG_CREATED,
    KDSIP_EVENT_TRANSC_CREATED,

	KDSIP_EVENT_COUNT,
};
// SIP Dialog 类型
enum EKDSIP_DIALOG_TYPE
{
    KDSIP_DIALOG_TYPE_NONE = 0,
    KDSIP_DIALOG_TYPE_INVITE = 1,
    KDSIP_DIALOG_TYPE_REGISTER = 2,
    KDSIP_DIALOG_TYPE_SUBSCRIBE = 3,
};

// SIP应答错误码
enum EKDSIP_RESPONSE_CODE
{
    KDSIP_RESPONSE_NONE                 = 0,
    // 1xx Provisional
    KDSIP_RESPONSE_100_TRYING           = 100,  // 100 Trying
    KDSIP_RESPONSE_180_RINGING          = 180,  // 180 Ringing
    // 2xx Successful
    KDSIP_RESPONSE_200_OK               = 200,  // 200 OK
    KDSIP_RESPONSE_202_ACCEPTED         = 202,  // 202 Accepted
    // 3xx Redirection
    // 4xx Client Error
    KDSIP_RESPONSE_401_UNAUTHORIZED         = 401, //401 鉴权
    KDSIP_RESPONSE_403_FORBIDDEN        = 403,  // 403 禁止访问
    KDSIP_RESPONSE_404_NOT_FOUND        = 404,  // 404 找不到目标
	KDSIP_RESPONSE_405_NOT_ALLOWED        = 405,  // 405 不允许的操作
    KDSIP_RESPONSE_408_REQ_TIMEOUT      = 408,  // 408 请求超时
    KDSIP_RESPONSE_423_INTERV_TOO_BRIEF = 423,  // 423 请求过于频繁
    KDSIP_RESPONSE_486_BUSY_HERE        = 486,  // 486 Busy Here
    // 5xx Server Error
    // 6xx Global Error
};

// KDSIP返回值
enum KDSIP_RETURN
{
	KDSIP_OK            = 0,    // OK
	KDSIP_NO_SOURCE     = 700,  // 700
	KDSIP_INVALID_ARG,          // 无效参数
	KDSIP_FAIL,                 // 失败
	KDSIP_TIMEOUT,              // 超时
	// 其他返回值定义
	//......
};

// Trans类型
enum EKDSIP_TRANSPORT
{
	KDSIP_TRANSPORT_UDP   = 0,
	KDSIP_TRANSPORT_TCP,
    KDSIP_TRANSPORT_OTHER = 100,
};

// 消息内容类型
enum EKDSIP_CONTENT_TYPE
{
	KDSIP_CONTENT_UNKNOWN = 0, // 位置类型
	KDSIP_CONTENT_XML,         // xml类型
	KDSIP_CONTENT_SDP,         // sdp类型
    KDSIP_CONTENT_MANSCDP,     // GB28181 MANSCDP
    KDSIP_CONTENT_MANSRTSP,     // GB28181 MANRTSP
	KDSIP_CONTENT_KSLP,        //自定义类型application/KSLP
	KDSIP_CONTENT_KSPTZ        //自定义类型application/KSPTZ
};

// 日志类型级别
enum EKDSIP_LOG_EVENT
{
	KDSIP_LOG_NONE  = 0,           // 不输出日志
    KDSIP_LOG_INFO  = 0x01,        // 只输出INFO
	KDSIP_LOG_ERROR = 0x03,        // 输出INFO ERROR
	KDSIP_LOG_EVENT = 0x07,        // 输出INFO ERROR EVENT             默认级别
    KDSIP_LOG_MSG   = 0x0f,        // 输出INFO ERROR EVENT SIPMSG
    KDSIP_LOG_DEBUG = 0x17,        // 输出INFO ERROR EVENT DEBUG
	KDSIP_LOG_ALL   = 0xff,        // 输出包括DEBUG消息在内的所有日志消息

	KDSIP_LOG_COUNT,
};

//动态库符号导出
//KD_API在kdvtype.h定义
#ifndef KD_API
#define KD_API   
#endif
#ifndef KDSIP_API
#define KDSIP_API  KD_API 
#endif

class KDSIP_API CKDSipURITOOL
{
public:
    /*==============================================================================
      函数名    : CKDSipURITOOL
      功能      :
      算法实现  : CKDSipURITOOL 构造函数，通过字符串分析出 user domain 以及 port
                  字符串样例 sip:sample_user@sample_domain:1234
      参数说明  : LPCSTR szURI
      返回值说明:
    --------------------------------------------------------------------------------
      修改记录:
      日 期      版本  修改人  走读人  修改记录
      2011/10/21  1.0   吴彬              创建
    ==============================================================================*/
    CKDSipURITOOL(LPCSTR szURI);
    /*==============================================================================
      函数名    : CKDSipURITOOL
      功能      :
      算法实现  : CKDSipURITOOL 构造函数，通过 user domain 以及 port构造URI
      参数说明  : LPCSTR szURI
      返回值说明:
    --------------------------------------------------------------------------------
      修改记录:
      日 期      版本  修改人  走读人  修改记录
      2011/10/21  1.0   吴彬              创建
    ==============================================================================*/
    CKDSipURITOOL(LPCSTR szUser, LPCSTR szDomain, u16 wPort);
    virtual ~CKDSipURITOOL();
    LPCSTR GetURI();
    LPCSTR GetUser();
    LPCSTR GetDomain();
    u16    GetPort();

protected:
    LPSTR m_szURI;    //uri
    LPSTR m_szUser;   //用户名
    LPSTR m_szDomain; //域名
    u16   m_wPort;    //端口
};

class KDSIP_API CKDSipAUTHTOOL
{
public:
    CKDSipAUTHTOOL(LPCSTR szAuthorization, EKDSIP_EVENT eEvent);
    virtual ~CKDSipAUTHTOOL();
    LPCSTR GetUsername() const;
    BOOL32 CheckPassword(LPCSTR szPassword);
protected:
    void* m_pvAuthToolData;
};
// 消息头类

class KDSIP_API CKDSipMsgHead
{
public:
	CKDSipMsgHead();
	virtual ~CKDSipMsgHead();

	LPCSTR GetToURI() const;
	LPCSTR GetFromURI() const;
	LPCSTR GetProxyURI() const;
    LPCSTR GetContactURI() const;
	LPCSTR GetUserAgent() const;    // GB28181
    LPCSTR GetSubject() const;      // GB28181
    LPCSTR GetDate() const;         // GB28181
    LPCSTR GetAuthorization() const;                 // GB28181
    LPCSTR GetWWWAuthenticateRealm() const;                // 客户端处理第一条注册的应答，获取realm
    LPCSTR GetCallID() const;                // 获取消息的CallID

	KDSIP_RETURN SetToURI(LPCSTR szURI);
    KDSIP_RETURN SetToURI(LPCSTR szUser, LPCSTR szDomain, u16 wPort = KDSIP_NO_PORT);
	KDSIP_RETURN SetFromURI(LPCSTR szURI);
    KDSIP_RETURN SetFromURI(LPCSTR szUser, LPCSTR szDomain, u16 wPort = KDSIP_NO_PORT);
	KDSIP_RETURN SetProxyURI(LPCSTR szURI);
    KDSIP_RETURN SetProxyURI(LPCSTR szDomain, u16 wPort);
	KDSIP_RETURN SetContactURI(LPCSTR szURI);
    KDSIP_RETURN SetContactURI(LPCSTR szUser, LPCSTR szDomain, u16 wPort = KDSIP_NO_PORT);
    KDSIP_RETURN SetUserAgent(LPCSTR szUserAgent);  // GB28181
    KDSIP_RETURN SetSubject(LPCSTR szSubject);      // GB28181
    KDSIP_RETURN SetDate(LPCSTR szDate);            // GB28181
    KDSIP_RETURN SetAuthorizationInfo(LPCSTR szUsername, LPCSTR szPassword); //客户端发送第二条注册用，设置鉴权用户名密码
    KDSIP_RETURN SetWWWAuthenticateRealm(LPCSTR szRealm); // 服务端发送第一条注册用，设置realm
    LPSTR m_szUsername;
    LPSTR m_szPassword;
protected:
    LPSTR m_szTo;
    LPSTR m_szFrom;
    LPSTR m_szProxy;
    LPSTR m_szContact;
    LPSTR m_szUserAgent; // GB28181
    LPSTR m_szSubject;  // GB28181
    LPSTR m_szDate;     // GB28181
    LPSTR m_szRealm;
    LPSTR m_szAuthorization;
    LPSTR m_szCallID; // 用于标识不同的注册回话
protected:
    void FillCommonHeader(void* hMsg, BOOL32 bIsUa);
};

// 消息体类
class KDSIP_API CKDSipMsgBody
{
public:
	CKDSipMsgBody();
	virtual ~CKDSipMsgBody();

	const u8*           GetMsgBodyContent(s32* pnMsgLen = NULL) const;//不包括\0在内的长度
	KDSIP_RETURN        SetMsgBodyContent(const u8* pbyContent, s32 nMsgLen);
	EKDSIP_CONTENT_TYPE GetContentType() const;
	KDSIP_RETURN        SetContentType(EKDSIP_CONTENT_TYPE eType);

protected:
	EKDSIP_CONTENT_TYPE m_eContentType;         // 消息体type：XML or SDP
	u8*                 m_pbyContent;           // 消息buffer
	s32                 m_nContentLength;       // 消息体length
	CKDSipMsgBody*      m_pNext;                // 可能存在多个BODY
};

// kdsip消息类
class KDSIP_API CKDSipEvent
{
public:
	CKDSipEvent();
	virtual ~CKDSipEvent();

    EKDSIP_DIALOG_TYPE   GetDialogType() const;
    KDSIP_DIALOG_ID      GetDialogID() const;
    KDSIP_TRANS_ID       GetTransID() const;

    EKDSIP_TRANSPORT     GetTransportProtocal() const;
    EKDSIP_EVENT         GetEvent() const;
    EKDSIP_RESPONSE_CODE GetResponseCode() const;

    const CKDSipMsgHead* GetSipHead() const;
    const CKDSipMsgBody* GetSipBody() const;
    u32                  GetExpire() const;
    u32                  GetSN() const;
    const s8*            GetSrcIP() const;
    u16                  GetSrcPort() const;
    const s8*            GetDstIP() const;
    u16                  GetDstPort() const;

protected:
    EKDSIP_DIALOG_TYPE   m_eDialogType;   // KDSIP会话类型
	KDSIP_DIALOG_ID      m_tDialogID;     // KDSIP会话标识
	KDSIP_TRANS_ID       m_tTransID;      // KDSIP事务ID，用于区分对话内的不同事务
    u32                  m_dwExpire;      // expire过期时间
    EKDSIP_TRANSPORT     m_eTransport;    // 传输协议
	EKDSIP_EVENT         m_eEvent;        // KDSIP事件信息
    EKDSIP_RESPONSE_CODE m_eResponseCode; // 应答错误码
	CKDSipMsgHead*       m_pcSipHead;     // KDSIP消息头
	CKDSipMsgBody*       m_pcSipBody;     // KDSIP消息体
    u32                  m_dwSN;          // 事件流水号

    s8*                  m_szSrcIP;
    u16                  m_wSrcPort;
    s8*                  m_szDstIP;
    u16                  m_wDstPort;

};

// Proxy消息头
class KDSIP_API CKDSipMsgHeadForProxy:public CKDSipMsgHead
{
public:
    CKDSipMsgHeadForProxy();
    virtual ~CKDSipMsgHeadForProxy();

    //ViaURI: sip:domain[:port] eg: sip:proxy.kedacom.com | sip:172.16.64.100:5690
    KDSIP_RETURN    AddTopMostViaAddr(LPSTR szVia);
    KDSIP_RETURN    RemoveTopMostViaAddr();
    LPCSTR          GetTopMostViaAddr();

    //RecordRoute: sip:domain[:port][;lr] eg: sip:proxy.kedacom.com;lr
    KDSIP_RETURN    AddTopMostRecordRouteURI(LPSTR szRecordRoute);
    //KDSIP_RETURN    RemoveTopMostRecordRouteURI();
    LPCSTR          GetTopMostRecordRouteURI();
    KDSIP_RETURN   ReplaceRecordRouteURI(LPSTR szFindTargetURI, LPSTR szNewURI);

    //Route: sip:domain[:port] eg: sip:proxy.kedacom.com;lr
    //KDSIP_RETURN    AddTopMostRouteURI(LPSTR szRoute);
    KDSIP_RETURN    RemoveTopMostRouteURI();
    LPCSTR          GetTopMostRouteURI();

    //RequestURI: sip:[user@]domain[:port] eg: sip:ltm@kedacom.com:3221
    LPCSTR          GetRequestURI();
    KDSIP_RETURN    SetRequestURI(LPCSTR szRequestURI);
    KDSIP_RETURN    SetRequestURI(LPCSTR szUser, LPCSTR szDomain, u16 wPort = KDSIP_NO_PORT);

protected:
    void*   m_pvViaList;         // VIA列表
    void*   m_pvRecordRouteList; // RecordRoute列表
    void*   m_pvRouteList;       // Route列表
    LPSTR   m_szRequestURI;      // RequestURI
};

// Proxy消息
class KDSIP_API CKDSipEventForProxy:public CKDSipEvent
{
public:
    CKDSipEventForProxy();
    virtual ~CKDSipEventForProxy();
    CKDSipMsgHeadForProxy* GetSipHead() const;
    void SetTransportProtocal(EKDSIP_TRANSPORT eTransport);

protected:
    CKDSipMsgHeadForProxy* m_pcSipHead;
};


// 回调接口
typedef void(*KDSIPLogCallback)(LPCSTR szLog);                            // 日志回调
typedef void(*KDSIPEventCallback)(CKDSipEvent* pcEvent, void* pvContext); // 事件回调


// KDSIP初始化参数
struct KDSIP_API TKDSIP_INIT_CONFIG
{
    LPCSTR              szLogPath;           // 日志文件路径
    LPCSTR              szLogFile;           // 日志文件 默认日志级别为KDSIP_LOG_EVENTENT
    u32                 dwLogFileMaxSize;    // 日志文件长度最大值
    u32                 dwLogFileMaxFileNum; // 日志文件个数最大值

    BOOL32              bProxyMode;          // 是否为Proxy工作模式

    BOOL32              bTCPEnable;          // 是否启用TCP
    LPCSTR              szLocalTCPIp;        // TCP IP
    u16                 wLocalTCPPort;       // TCP 端口

    BOOL32              bUDPPortReuse;       // UDP端口是否复用 默认FALSE
    BOOL32              bTCPPortReuse;       // TCP端口是否复用 默认FALSE
    BOOL32              bUDPChecksumDisable; // 禁用UDPChecksum

    KDSIPLogCallback    pfLogCB;             // 日志回调
    KDSIPEventCallback  pfEventCB;           // 消息回调，若为NULL则KDSipPollSipEvent起作用
    void*               pvEventCBContext;    // 消息回调用户上下文

    /*初始允许的Dialog数 当dialog数大于等该值后  hash表进行扩张  仍然允许接入的初始对话数, kdsip_pj特有
    */
    s32                 nInitialDialogCap;   
    s32                 nMaxInviteDialog;    // 最大Invite Dialog数 kdsip特有
    s32                 nMaxRegisterDialog;  // 最大Register Dialog数 kdsip特有
    s32                 nMaxSubscribeDialog; // 最大Subscribe Dialog数 kdsip特有
	s32                 nNumOfNotifysPerSub; // 每个会话的notify数     kdsip特有
    /*初始允许事务数 当事务数大于等该值后 hash表进行扩张 仍然允许接入, kdsip_pj特有
    */
	s32                 nInitialTransCap;
    s32                 nMaxTrans;           // 最大事务数 kdsip特有
    s32                 nRetransmitTimer;    // 重传定时 ms(sip层T1定时器)
    s32                 nTransLingerTimer;    // 事务等待销毁定时 ms
    s32                 nMsgSizeMax;         // 消息最大长度

    s32                 nProcessingQueueSize; //kdsip特有 底层消息队列长度 0默认nMaxInviteDialog + .nMaxSubscribeDialog + .nMaxRegisterDialog + .nMaxTrans
    s32                 nReadBufferNum;       //kdsip特有 底层读缓冲队列数量 0 默认 nProcessingQueueSize/4
    s32                 nMaxTransmitter;      //kdsip特有 底层transmitter数量最大值 0 默认 nMaxTrans+10
    s32                 nMaxMemPoolCapacity;  //kdsip_pj特有 允许协议栈内存池持有的最大空闲内存

	//kdsip_pj特有
    //对话内消息允许以contact作为之后的联系地址
	//用于应对穿nat场景 contact可能是内网地址 不适合作为之后的联系地址 而应该使用
	//socket上接收消息的ip和端口作为之后的联系地址
	//按照sip协议，contact应该由远端保证正确性, 此开关应设为true;
	//穿nat的场景为了兼容ipc不按照协议实现，也就是contact携带的是内网地址,此开关设为false,
	//以via头中的rport=xx;received=xx;中的ip和端口作为联系地址.
    BOOL32              bAllowUseContactAsTarget;

    TKDSIP_INIT_CONFIG()
    {
        szLogPath           = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_PATH;
        szLogFile           = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE;
        dwLogFileMaxSize    = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_SIZE;
        dwLogFileMaxFileNum = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_NUM;
        bProxyMode          = FALSE;
        bTCPEnable          = FALSE;
        szLocalTCPIp        = NULL;
        wLocalTCPPort       = KDSIP_NO_PORT;
        bUDPPortReuse       = FALSE;
        bTCPPortReuse       = FALSE;
        bUDPChecksumDisable = FALSE;
        pfLogCB             = NULL;
        pfEventCB           = NULL;
        pvEventCBContext    = NULL;
        nMaxInviteDialog    = KDSIP_INIT_CONFIG_DEFAULT_MAXIINVITEDIALOG;
        nMaxRegisterDialog  = KDSIP_INIT_CONFIG_DEFAULT_MAXREGISTERDIALOG;
        nMaxSubscribeDialog = KDSIP_INIT_CONFIG_DEFAULT_MAXSUBSCRIBEDIALOG;
        nInitialDialogCap   = KDSIP_INIT_CONFIG_DEFAULT_DIALOG_CAP;
		nNumOfNotifysPerSub = KDSIP_INIT_CONFIG_DEFAULT_NUMOFNOTIFYPERSUB;
        nInitialTransCap    = KDSIP_INIT_CONFIG_DEFAULT_TRANS_CAP;
        nMaxTrans           = KDSIP_INIT_CONFIG_DEFAULT_MAXTRANSCATION;
        nRetransmitTimer    = KDSIP_INIT_CONFIG_DEFAULT_RETRANSMITTIMER;
        nTransLingerTimer   = KDSIP_INIT_CONFIG_DEFAULT_LINGERTIMER;
        nMsgSizeMax         = KDSIP_INIT_CONFIG_DEFAULT_MSG_SIZE_MAX;
        nProcessingQueueSize = 0;
        nReadBufferNum = 0;
        nMaxTransmitter = 0;
        nMaxMemPoolCapacity = KDSIP_INIT_CONFIG_DEFAULT_MEM_POOL_CAP;
        bAllowUseContactAsTarget = FALSE;
    }
};

//add by zhoujc 2014/10/24
typedef struct
{
	u32 dwUsage;//当前占用个数	
    u32 dwMax;//允许的规格
}TKDSIP_DIALOG_RESOURCE;

/*====================================================================
函数名      : KDSipPollSipEvent
功能        : 轮询获取KDSIP事件
算法实现    : 类似select或epoll的方式
引用全局变量:
输入参数说明:
              dwMillSec： 以毫秒为单位的轮询超时
返回值说明  : 返回的CKDSipEvent*指针需要上层调用FreeSipEvent释放
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API CKDSipEvent* KDSipPollSipEvent(u32 dwMillSec);

/*====================================================================
函数名      : KDSipFreeSipEvent
功能        : 销毁KDSipPollSipEvent返回的KDSIP事件
算法实现    :
引用全局变量:
输入参数说明:
              pCSipEvent: KDSipPollSipEvent返回的指针
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API void KDSipFreeSipEvent(CKDSipEvent* pCSipEvent);

/*====================================================================
函数名      : KDSipInit
功能        : KDSIP初始化
算法实现    :
引用全局变量:
输入参数说明:
              szLocalUDPIp: sip UDP 监听地址
			  wLocalUDPPort: sip UDP端口
              ptInitConfig:其他初始化配置
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInit(LPSTR szLocalUDPIp = NULL, u16 wLocalUDPPort = 5060,
	           const TKDSIP_INIT_CONFIG * ptInitConfig = NULL);

/*====================================================================
函数名      : KDSipQuit
功能        : KDSIP退出
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API void KDSipQuit();



/*====================================================================
函数名      : KDSipRegisterReq
功能        : KDSIP注册请求，注销时设置nExpires = 0
算法实现    :
引用全局变量:
输入参数说明:
              cSipHead: KDSIP消息头，包含KDSIP路由信息
              nExpires: 有效时间 以秒为单位
			  pcSipBody: KDSIP消息体类指针
              dwSN :流水号
              EKDSIP_TRANSPORT 传输协议
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterReq(const CKDSipMsgHead& cSipHead, s32 nExpires,
	const CKDSipMsgBody* pcSipBody, u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
函数名      : KDSipRegisterRefreshReq
功能        : 刷新注册
              如果nExpires填写为0则表示停止注册。KDSIP协议栈将销毁会话，KDSIP_DIALOG_ID不再有效。
算法实现    :
引用全局变量:
输入参数说明:
              tDialogID: KDSIP会话标识
			  nExpires: 有效时间 以秒为单位 若设为0表示取消注册
			  pcSipBody: KDSIP消息体类指针
              dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/20  0.1         吴彬
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterRefreshReq(KDSIP_DIALOG_ID tDialogID, s32 nExpires,
	const CKDSipMsgBody* pcSipBody, u32& dwSN);
KDSIP_API KDSIP_RETURN KDSipRegisterReqWithHeader(KDSIP_DIALOG_ID tDialogID, s32 nExpires, const CKDSipMsgHead* pcSipHeader,
    const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipRegisterRsp
功能        : KDSIP注册应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: KDSIP事务标识
			 eResponseCode: KDSIP处理结果的回应码 成功200 拒绝403
			 pcSipBody: KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody);

KDSIP_API KDSIP_RETURN KDSipRegisterRspWithHeader(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,const CKDSipMsgHead* pcSipHeader,
    const CKDSipMsgBody* pcSipBody);

/*====================================================================
函数名      : KDSipInviteReq
功能        : KDSIP呼叫请求
算法实现    :
引用全局变量:
输入参数说明:
              cSipHead: KDSIP消息头类引用
			  pcSipBody:KDSIP消息体类指针
              pcSipBody: KDSIP消息体类指针
              dwSN :流水号
返回值说明  :  成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
函数名      : KDSipInviteRsp
功能        : KDSIP呼叫应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: KDSIP事务标识
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
函数名      : KDSipInviteACK
功能        : KDSIP呼叫ACK
算法实现    :
引用全局变量:
输入参数说明:
             tDialogID: KDSIP会话标识, invite 返回的KDSIP_DIALOG_ID
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteACK(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipByeReq
功能        : KDSIP呼叫挂断请求
算法实现    :
引用全局变量:
输入参数说明:
             tDialogID: KDSIP会话标识, invite 返回的KDSIP_DIALOG_ID
             pcSipBody:KDSIP消息体类指针
             dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipByeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipByeRsp
功能        : KDSIP呼叫挂断应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: 事务ID
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipByeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipInfoReq
功能        : 呼叫内Info请求
算法实现    : 对话内事务
引用全局变量:
输入参数说明:
             tDialogID: KDSIP会话标识, invite 返回的KDSIP_DIALOG_ID
             pcSipBody:KDSIP消息体类指针
             dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInfoReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipInfoRsp
功能        : 呼叫内Info应答
算法实现    : 对话内事务
引用全局变量:
输入参数说明:
             tTransID: 事务ID
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInfoRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipMessageReq
功能        : Message请求
算法实现    :
引用全局变量:
输入参数说明:
              cSipHead: KDSIP消息头，包含KDSIP路由信息
			  pcSipBody:KDSIP消息体类指针
              dwSN :流水号
              eTransport 传输协议
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
函数名      : KDSipMessageRsp
功能        : Message应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: KDSIP事务标识
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipMessageInDlgReq
功能        : 呼叫内Message请求
算法实现    : 对话内事务
引用全局变量:
输入参数说明:
             tDialogID: KDSIP会话标识, invite 返回的KDSIP_DIALOG_ID
			 pcSipBody:KDSIP消息体类指针
             dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageInDlgReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipOptionsReq
功能        : Options请求
算法实现    :
引用全局变量:
输入参数说明:
              cSipHead: KDSIP消息头，包含KDSIP路由信息
			  pcSipBody:KDSIP消息体类指针
              dwSN :流水号
              eTransport 传输协议
返回值说明  : KDSIP事务标识，具有唯一性，INVALID_TRANS_ID为无效标识
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipOptionsReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
函数名      : KDSipOptionsRsp
功能        : Options应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: KDSIP事务标识
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipOptionsRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipSubscribeReq
功能        : 订阅请求
算法实现    :
引用全局变量:
输入参数说明:
              cSipHead: KDSIP消息头，包含KDSIP路由信息
			  pcSipBody:KDSIP消息体类指针
              dwSN :流水号
              eTransport 传输协议
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
#define DEFAULT_EXPIRES     (3600)
KDSIP_API KDSIP_RETURN KDSipSubscribeReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
 u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP,u32 dwExpire = DEFAULT_EXPIRES,BOOL32 bAutoRefreshWithBody =FALSE);

/*====================================================================
函数名      : KDSipSubscribeRefreshReq
功能        : 刷新订阅请求
算法实现    :
引用全局变量:
输入参数说明:
              tDialogID: KDSIP会话标识, subscribe 返回的KDSIP_DIALOG_ID
			  pcSipBody:KDSIP消息体类指针
              dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/12/16  0.2         吴彬          创建
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipSubscribeRefreshReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipSubscribeRsp
功能        : 订阅应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: KDSIP事务标识
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
			 pcSipHeader:用于业务填写ContactURI
			 bActiveNotifyByUser:TRUE，由业务发送激活notify，
								 FALSE：kdsip发送不带消息的notify
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL, BOOL32 bActiveNotifyByUser = FALSE);

/*====================================================================
函数名      : SipUnSubscribeReq
功能        : 取消订阅请求
算法实现    :
引用全局变量:
输入参数说明:
			  tDialogID: KDSIP会话标识, subscribe 返回的KDSIP_DIALOG_ID
			  pcSipBody:KDSIP消息体类指针
              dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2011/10/13  0.3         吴彬           修改返回值类型
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipUnSubscribeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
函数名      : KDSipUnSubscribeRsp
功能        : 取消订阅应答
算法实现    :
引用全局变量:
输入参数说明:
             tTransID: 事务ID
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipUnSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);


/*====================================================================
函数名      : KDSipNotifyReq
功能        : 订阅通知请求
算法实现    : 对话内事务
引用全局变量:
输入参数说明:
             tDialogID: incoming subscribe的KDSIP标识
			 pcSipBody:KDSIP消息体类指针
             dwSN :流水号
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipNotifyReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);


/*====================================================================
函数名      : SipNotifyRsp
功能        : 订阅通知应答
算法实现    : 对话内事务
引用全局变量:
输入参数说明:
             tTransID: 事务ID
			 eResponseCode: KDSIP处理结果的回应码
			 pcSipBody:KDSIP消息体类指针
返回值说明  : 成功返回KDSIP_OK，失败返回其他值
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
2011/09/20  0.2         吴彬           调整接口命名
2013/03/28  2.0         吴彬           调整接口参数
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipNotifyRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
函数名      : KDSipTerminateInviteDialog
功能        : 强制结束本地的Invite Dialog, 不会发送SIP信令。
算法实现    :
引用全局变量:
输入参数说明:
             tInviteDialogID: 会话ID
返回值说明  : 无
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/20  0.2         吴彬
====================================================================*/
KDSIP_API void KDSipTerminateInviteDialog(KDSIP_DIALOG_ID tInviteDialogID);

/*====================================================================
函数名      : KDSipTerminateSubsDialog
功能        : 强制结束本地的订阅Dialog, 不会发送SIP信令。
算法实现    :
引用全局变量:
输入参数说明:
             tSubsDialogID: 会话ID
返回值说明  : 无
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/20  0.2         吴彬
====================================================================*/
KDSIP_API void KDSipTerminateSubsDialog(KDSIP_DIALOG_ID tSubsDialogID);

/*====================================================================
函数名      : KDSipTerminateRegDialog
功能        : 强制结束本地的注册Dialog, 不会发送SIP信令。
算法实现    :
引用全局变量:
输入参数说明:
             tRegDialogID: 会话ID
返回值说明  : 无
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/09/20  0.2         吴彬
====================================================================*/
KDSIP_API void KDSipTerminateRegDialog(KDSIP_DIALOG_ID tRegDialogID);

/*====================================================================
函数名      : SipSetLogLev
功能        : 设置KDSIP库的打印级别
算法实现    :
引用全局变量:
输入参数说明:
             eLogLev: 打印级别
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/08/01  0.1         fanxg
====================================================================*/
KDSIP_API void KDSipSetLogLev(EKDSIP_LOG_EVENT eLogLev);

/*==========================================================
函数名: KDSipSetLogOnScreen
功能:
算法实现:
参数说明: BOOL32 bIsOnScreen
返回值说明:
------------------------------------------------------------
修改记录:
日 期      版本  修改人  走读人  修改记录
2012/02/17  1.0  吴彬              创建
===========================================================*/
KDSIP_API void KDSipSetLogOnScreen(BOOL32 bIsOnScreen);

/*==========================================================
函数名: KDSipSetLogShowTime
功能:
算法实现:
参数说明: BOOL32 bIsShowTime
返回值说明:
------------------------------------------------------------
修改记录:
日 期      版本  修改人  走读人  修改记录
2012/02/17  1.0  吴彬              创建
===========================================================*/
KDSIP_API void KDSipSetLogShowTime(BOOL32 bIsShowTime);

/**
  设置kdsip日志时间戳是否精确到ms级别
  TRUE:  精确到ms级别
  FALSE: 精确到s级别
*/
KDSIP_API void KDSipSetLogTimeStampLevel(BOOL32 bMsLevel);


// 无状态代理回调函数.
typedef KDSIP_RETURN (*PFKDSipStatelessProxyCB) ( // 返回值为0 或 KDSIP错误码.
	void*                pvUserData,			  // 自定义数据.
	CKDSipEventForProxy& cSipEvent ,		      // 收到的消息事件类，包含消息头和消息体.
	BOOL32&              bStatelessMod,           // 是否采用无状态代理
	BOOL32&              bRequestURIResolved      // 是否解析出URI
	/*LPSTR szDstIp, u16& dstPort */);            // 使用SetProxyURI替代
//
// 		Proxy2
//   Proxy1 		Proxy3
// A       B			c
// 1 A发送INVITE给proxy
// 2 A发送INVITE给B
// 3 A发送INVITE给C
//
// 以上三种情况回调的处理过程
// 1 回调设置statelessMod为FALSE，proxy此时等同于ua
// 2 修改requestURI，设置statelessMod为TRUE 设置requesstURIResolved为TRUE
// 3 设置statelessMod为TRUE 设置requesstURIResolved为FALSE并调用 SetProxyURI，若没有调用则协议栈返回404


/*==============================================================================
  函数名    : KDSipSetProxyCallback
  功能      : 设置无状态代理回调函数
  算法实现  :
  参数说明  : PFKDSipStatelessProxyCB pf, void * pvUserData
  返回值说明:
--------------------------------------------------------------------------------
  修改记录:
  日 期      版本  修改人  走读人  修改记录
  2011/10/21  1.0   吴彬              创建
==============================================================================*/
KDSIP_API KDSIP_RETURN KDSipSetProxyCallback(PFKDSipStatelessProxyCB pf, void * pvUserData);

/*==============================================================================
  函数名    : KDSipGetDialogResource
  功能      : 获取给定类型的会话使用的资源数
  算法实现  :
  参数说明  : eDialogType :        要获取的会话类型
              ptDialogResource:    获取资源的存放位置
  返回值说明:
--------------------------------------------------------------------------------
  修改记录:
  日 期      版本  修改人  走读人  修改记录
  2014/10/27  1.0   周建春              创建
==============================================================================*/
KDSIP_API KDSIP_RETURN KDSipGetDialogResource(EKDSIP_DIALOG_TYPE eDialogType,TKDSIP_DIALOG_RESOURCE * ptDialogResorce);

/*
  获取事务资源占用情况
*/
KDSIP_API void KDSipGetTransResource(TKDSIP_DIALOG_RESOURCE* ptResc);

KDSIP_API void KDSIPVer();        // 打印版本号
KDSIP_API void KDSIPShow();       // 打印资源情况
KDSIP_API void KDSIPDialogShow(); // 打印会话资源
KDSIP_API void KDSIPTransShow();  // 打印事务资源
#ifndef _MSC_VER
KDSIP_API void KDSIPShowRouteTable();//打印roteTable中的情况
#endif
KDSIP_API void KDSIPInviteDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPSubsDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPRegDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPTransShow(KDSIP_TRANS_ID hInviteDialog);

#endif //#ifndef _KDSIP_H_
