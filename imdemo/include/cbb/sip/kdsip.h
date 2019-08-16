#ifndef _KDSIP_H_
#define _KDSIP_H_


#ifdef _MSC_VER
#include <windows.h>
#endif    // _MSC_VER
#include "kdvtype.h"
#ifndef NULL
#define NULL 0
#endif

typedef void* KDSIP_DIALOG_ID;                 // ����ͳһ��KDSIP�Ự��ʶ
typedef void* KDSIP_TRANS_ID;                  // KDSIP�����ʶ���������ֶԻ��ڵĲ�ͬ����

#define KDSIP_VERSION       "KDSIP_V2.2"  // kdsip�汾��
#define INVALID_DIALOG_ID   (KDSIP_DIALOG_ID)0 // ��ЧKDSIP��ʶ
#define INVALID_TRANS_ID    (KDSIP_TRANS_ID)0  // ��Ч�����ʶ
#define KDSIP_NO_PORT       (u16)-1            // ��Ч�˿�

// TKDSIP_INIT_CONFIG Ĭ��ֵ
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
// SIP�¼�����
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
// SIP Dialog ����
enum EKDSIP_DIALOG_TYPE
{
    KDSIP_DIALOG_TYPE_NONE = 0,
    KDSIP_DIALOG_TYPE_INVITE = 1,
    KDSIP_DIALOG_TYPE_REGISTER = 2,
    KDSIP_DIALOG_TYPE_SUBSCRIBE = 3,
};

// SIPӦ�������
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
    KDSIP_RESPONSE_401_UNAUTHORIZED         = 401, //401 ��Ȩ
    KDSIP_RESPONSE_403_FORBIDDEN        = 403,  // 403 ��ֹ����
    KDSIP_RESPONSE_404_NOT_FOUND        = 404,  // 404 �Ҳ���Ŀ��
	KDSIP_RESPONSE_405_NOT_ALLOWED        = 405,  // 405 ������Ĳ���
    KDSIP_RESPONSE_408_REQ_TIMEOUT      = 408,  // 408 ����ʱ
    KDSIP_RESPONSE_423_INTERV_TOO_BRIEF = 423,  // 423 �������Ƶ��
    KDSIP_RESPONSE_486_BUSY_HERE        = 486,  // 486 Busy Here
    // 5xx Server Error
    // 6xx Global Error
};

// KDSIP����ֵ
enum KDSIP_RETURN
{
	KDSIP_OK            = 0,    // OK
	KDSIP_NO_SOURCE     = 700,  // 700
	KDSIP_INVALID_ARG,          // ��Ч����
	KDSIP_FAIL,                 // ʧ��
	KDSIP_TIMEOUT,              // ��ʱ
	// ��������ֵ����
	//......
};

// Trans����
enum EKDSIP_TRANSPORT
{
	KDSIP_TRANSPORT_UDP   = 0,
	KDSIP_TRANSPORT_TCP,
    KDSIP_TRANSPORT_OTHER = 100,
};

// ��Ϣ��������
enum EKDSIP_CONTENT_TYPE
{
	KDSIP_CONTENT_UNKNOWN = 0, // λ������
	KDSIP_CONTENT_XML,         // xml����
	KDSIP_CONTENT_SDP,         // sdp����
    KDSIP_CONTENT_MANSCDP,     // GB28181 MANSCDP
    KDSIP_CONTENT_MANSRTSP,     // GB28181 MANRTSP
	KDSIP_CONTENT_KSLP,        //�Զ�������application/KSLP
	KDSIP_CONTENT_KSPTZ        //�Զ�������application/KSPTZ
};

// ��־���ͼ���
enum EKDSIP_LOG_EVENT
{
	KDSIP_LOG_NONE  = 0,           // �������־
    KDSIP_LOG_INFO  = 0x01,        // ֻ���INFO
	KDSIP_LOG_ERROR = 0x03,        // ���INFO ERROR
	KDSIP_LOG_EVENT = 0x07,        // ���INFO ERROR EVENT             Ĭ�ϼ���
    KDSIP_LOG_MSG   = 0x0f,        // ���INFO ERROR EVENT SIPMSG
    KDSIP_LOG_DEBUG = 0x17,        // ���INFO ERROR EVENT DEBUG
	KDSIP_LOG_ALL   = 0xff,        // �������DEBUG��Ϣ���ڵ�������־��Ϣ

	KDSIP_LOG_COUNT,
};

//��̬����ŵ���
//KD_API��kdvtype.h����
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
      ������    : CKDSipURITOOL
      ����      :
      �㷨ʵ��  : CKDSipURITOOL ���캯����ͨ���ַ��������� user domain �Լ� port
                  �ַ������� sip:sample_user@sample_domain:1234
      ����˵��  : LPCSTR szURI
      ����ֵ˵��:
    --------------------------------------------------------------------------------
      �޸ļ�¼:
      �� ��      �汾  �޸���  �߶���  �޸ļ�¼
      2011/10/21  1.0   ���              ����
    ==============================================================================*/
    CKDSipURITOOL(LPCSTR szURI);
    /*==============================================================================
      ������    : CKDSipURITOOL
      ����      :
      �㷨ʵ��  : CKDSipURITOOL ���캯����ͨ�� user domain �Լ� port����URI
      ����˵��  : LPCSTR szURI
      ����ֵ˵��:
    --------------------------------------------------------------------------------
      �޸ļ�¼:
      �� ��      �汾  �޸���  �߶���  �޸ļ�¼
      2011/10/21  1.0   ���              ����
    ==============================================================================*/
    CKDSipURITOOL(LPCSTR szUser, LPCSTR szDomain, u16 wPort);
    virtual ~CKDSipURITOOL();
    LPCSTR GetURI();
    LPCSTR GetUser();
    LPCSTR GetDomain();
    u16    GetPort();

protected:
    LPSTR m_szURI;    //uri
    LPSTR m_szUser;   //�û���
    LPSTR m_szDomain; //����
    u16   m_wPort;    //�˿�
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
// ��Ϣͷ��

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
    LPCSTR GetWWWAuthenticateRealm() const;                // �ͻ��˴����һ��ע���Ӧ�𣬻�ȡrealm
    LPCSTR GetCallID() const;                // ��ȡ��Ϣ��CallID

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
    KDSIP_RETURN SetAuthorizationInfo(LPCSTR szUsername, LPCSTR szPassword); //�ͻ��˷��͵ڶ���ע���ã����ü�Ȩ�û�������
    KDSIP_RETURN SetWWWAuthenticateRealm(LPCSTR szRealm); // ����˷��͵�һ��ע���ã�����realm
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
    LPSTR m_szCallID; // ���ڱ�ʶ��ͬ��ע��ػ�
protected:
    void FillCommonHeader(void* hMsg, BOOL32 bIsUa);
};

// ��Ϣ����
class KDSIP_API CKDSipMsgBody
{
public:
	CKDSipMsgBody();
	virtual ~CKDSipMsgBody();

	const u8*           GetMsgBodyContent(s32* pnMsgLen = NULL) const;//������\0���ڵĳ���
	KDSIP_RETURN        SetMsgBodyContent(const u8* pbyContent, s32 nMsgLen);
	EKDSIP_CONTENT_TYPE GetContentType() const;
	KDSIP_RETURN        SetContentType(EKDSIP_CONTENT_TYPE eType);

protected:
	EKDSIP_CONTENT_TYPE m_eContentType;         // ��Ϣ��type��XML or SDP
	u8*                 m_pbyContent;           // ��Ϣbuffer
	s32                 m_nContentLength;       // ��Ϣ��length
	CKDSipMsgBody*      m_pNext;                // ���ܴ��ڶ��BODY
};

// kdsip��Ϣ��
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
    EKDSIP_DIALOG_TYPE   m_eDialogType;   // KDSIP�Ự����
	KDSIP_DIALOG_ID      m_tDialogID;     // KDSIP�Ự��ʶ
	KDSIP_TRANS_ID       m_tTransID;      // KDSIP����ID���������ֶԻ��ڵĲ�ͬ����
    u32                  m_dwExpire;      // expire����ʱ��
    EKDSIP_TRANSPORT     m_eTransport;    // ����Э��
	EKDSIP_EVENT         m_eEvent;        // KDSIP�¼���Ϣ
    EKDSIP_RESPONSE_CODE m_eResponseCode; // Ӧ�������
	CKDSipMsgHead*       m_pcSipHead;     // KDSIP��Ϣͷ
	CKDSipMsgBody*       m_pcSipBody;     // KDSIP��Ϣ��
    u32                  m_dwSN;          // �¼���ˮ��

    s8*                  m_szSrcIP;
    u16                  m_wSrcPort;
    s8*                  m_szDstIP;
    u16                  m_wDstPort;

};

// Proxy��Ϣͷ
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
    void*   m_pvViaList;         // VIA�б�
    void*   m_pvRecordRouteList; // RecordRoute�б�
    void*   m_pvRouteList;       // Route�б�
    LPSTR   m_szRequestURI;      // RequestURI
};

// Proxy��Ϣ
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


// �ص��ӿ�
typedef void(*KDSIPLogCallback)(LPCSTR szLog);                            // ��־�ص�
typedef void(*KDSIPEventCallback)(CKDSipEvent* pcEvent, void* pvContext); // �¼��ص�


// KDSIP��ʼ������
struct KDSIP_API TKDSIP_INIT_CONFIG
{
    LPCSTR              szLogPath;           // ��־�ļ�·��
    LPCSTR              szLogFile;           // ��־�ļ� Ĭ����־����ΪKDSIP_LOG_EVENTENT
    u32                 dwLogFileMaxSize;    // ��־�ļ��������ֵ
    u32                 dwLogFileMaxFileNum; // ��־�ļ��������ֵ

    BOOL32              bProxyMode;          // �Ƿ�ΪProxy����ģʽ

    BOOL32              bTCPEnable;          // �Ƿ�����TCP
    LPCSTR              szLocalTCPIp;        // TCP IP
    u16                 wLocalTCPPort;       // TCP �˿�

    BOOL32              bUDPPortReuse;       // UDP�˿��Ƿ��� Ĭ��FALSE
    BOOL32              bTCPPortReuse;       // TCP�˿��Ƿ��� Ĭ��FALSE
    BOOL32              bUDPChecksumDisable; // ����UDPChecksum

    KDSIPLogCallback    pfLogCB;             // ��־�ص�
    KDSIPEventCallback  pfEventCB;           // ��Ϣ�ص�����ΪNULL��KDSipPollSipEvent������
    void*               pvEventCBContext;    // ��Ϣ�ص��û�������

    /*��ʼ�����Dialog�� ��dialog�����ڵȸ�ֵ��  hash���������  ��Ȼ�������ĳ�ʼ�Ի���, kdsip_pj����
    */
    s32                 nInitialDialogCap;   
    s32                 nMaxInviteDialog;    // ���Invite Dialog�� kdsip����
    s32                 nMaxRegisterDialog;  // ���Register Dialog�� kdsip����
    s32                 nMaxSubscribeDialog; // ���Subscribe Dialog�� kdsip����
	s32                 nNumOfNotifysPerSub; // ÿ���Ự��notify��     kdsip����
    /*��ʼ���������� �����������ڵȸ�ֵ�� hash��������� ��Ȼ�������, kdsip_pj����
    */
	s32                 nInitialTransCap;
    s32                 nMaxTrans;           // ��������� kdsip����
    s32                 nRetransmitTimer;    // �ش���ʱ ms(sip��T1��ʱ��)
    s32                 nTransLingerTimer;    // ����ȴ����ٶ�ʱ ms
    s32                 nMsgSizeMax;         // ��Ϣ��󳤶�

    s32                 nProcessingQueueSize; //kdsip���� �ײ���Ϣ���г��� 0Ĭ��nMaxInviteDialog + .nMaxSubscribeDialog + .nMaxRegisterDialog + .nMaxTrans
    s32                 nReadBufferNum;       //kdsip���� �ײ������������� 0 Ĭ�� nProcessingQueueSize/4
    s32                 nMaxTransmitter;      //kdsip���� �ײ�transmitter�������ֵ 0 Ĭ�� nMaxTrans+10
    s32                 nMaxMemPoolCapacity;  //kdsip_pj���� ����Э��ջ�ڴ�س��е��������ڴ�

	//kdsip_pj����
    //�Ի�����Ϣ������contact��Ϊ֮�����ϵ��ַ
	//����Ӧ�Դ�nat���� contact������������ַ ���ʺ���Ϊ֮�����ϵ��ַ ��Ӧ��ʹ��
	//socket�Ͻ�����Ϣ��ip�Ͷ˿���Ϊ֮�����ϵ��ַ
	//����sipЭ�飬contactӦ����Զ�˱�֤��ȷ��, �˿���Ӧ��Ϊtrue;
	//��nat�ĳ���Ϊ�˼���ipc������Э��ʵ�֣�Ҳ����contactЯ������������ַ,�˿�����Ϊfalse,
	//��viaͷ�е�rport=xx;received=xx;�е�ip�Ͷ˿���Ϊ��ϵ��ַ.
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
	u32 dwUsage;//��ǰռ�ø���	
    u32 dwMax;//����Ĺ��
}TKDSIP_DIALOG_RESOURCE;

/*====================================================================
������      : KDSipPollSipEvent
����        : ��ѯ��ȡKDSIP�¼�
�㷨ʵ��    : ����select��epoll�ķ�ʽ
����ȫ�ֱ���:
�������˵��:
              dwMillSec�� �Ժ���Ϊ��λ����ѯ��ʱ
����ֵ˵��  : ���ص�CKDSipEvent*ָ����Ҫ�ϲ����FreeSipEvent�ͷ�
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API CKDSipEvent* KDSipPollSipEvent(u32 dwMillSec);

/*====================================================================
������      : KDSipFreeSipEvent
����        : ����KDSipPollSipEvent���ص�KDSIP�¼�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              pCSipEvent: KDSipPollSipEvent���ص�ָ��
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API void KDSipFreeSipEvent(CKDSipEvent* pCSipEvent);

/*====================================================================
������      : KDSipInit
����        : KDSIP��ʼ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              szLocalUDPIp: sip UDP ������ַ
			  wLocalUDPPort: sip UDP�˿�
              ptInitConfig:������ʼ������
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInit(LPSTR szLocalUDPIp = NULL, u16 wLocalUDPPort = 5060,
	           const TKDSIP_INIT_CONFIG * ptInitConfig = NULL);

/*====================================================================
������      : KDSipQuit
����        : KDSIP�˳�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API void KDSipQuit();



/*====================================================================
������      : KDSipRegisterReq
����        : KDSIPע������ע��ʱ����nExpires = 0
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              cSipHead: KDSIP��Ϣͷ������KDSIP·����Ϣ
              nExpires: ��Чʱ�� ����Ϊ��λ
			  pcSipBody: KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
              EKDSIP_TRANSPORT ����Э��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterReq(const CKDSipMsgHead& cSipHead, s32 nExpires,
	const CKDSipMsgBody* pcSipBody, u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
������      : KDSipRegisterRefreshReq
����        : ˢ��ע��
              ���nExpires��дΪ0���ʾֹͣע�ᡣKDSIPЭ��ջ�����ٻỰ��KDSIP_DIALOG_ID������Ч��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              tDialogID: KDSIP�Ự��ʶ
			  nExpires: ��Чʱ�� ����Ϊ��λ ����Ϊ0��ʾȡ��ע��
			  pcSipBody: KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/20  0.1         ���
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterRefreshReq(KDSIP_DIALOG_ID tDialogID, s32 nExpires,
	const CKDSipMsgBody* pcSipBody, u32& dwSN);
KDSIP_API KDSIP_RETURN KDSipRegisterReqWithHeader(KDSIP_DIALOG_ID tDialogID, s32 nExpires, const CKDSipMsgHead* pcSipHeader,
    const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipRegisterRsp
����        : KDSIPע��Ӧ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: KDSIP�����ʶ
			 eResponseCode: KDSIP�������Ļ�Ӧ�� �ɹ�200 �ܾ�403
			 pcSipBody: KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipRegisterRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody);

KDSIP_API KDSIP_RETURN KDSipRegisterRspWithHeader(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,const CKDSipMsgHead* pcSipHeader,
    const CKDSipMsgBody* pcSipBody);

/*====================================================================
������      : KDSipInviteReq
����        : KDSIP��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              cSipHead: KDSIP��Ϣͷ������
			  pcSipBody:KDSIP��Ϣ����ָ��
              pcSipBody: KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
����ֵ˵��  :  �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
������      : KDSipInviteRsp
����        : KDSIP����Ӧ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: KDSIP�����ʶ
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipInviteACK
����        : KDSIP����ACK
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInviteACK(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipByeReq
����        : KDSIP���йҶ�����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
             pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipByeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipByeRsp
����        : KDSIP���йҶ�Ӧ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: ����ID
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipByeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipInfoReq
����        : ������Info����
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
             pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInfoReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipInfoRsp
����        : ������InfoӦ��
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tTransID: ����ID
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipInfoRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipMessageReq
����        : Message����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              cSipHead: KDSIP��Ϣͷ������KDSIP·����Ϣ
			  pcSipBody:KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
              eTransport ����Э��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
������      : KDSipMessageRsp
����        : MessageӦ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: KDSIP�����ʶ
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipMessageInDlgReq
����        : ������Message����
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
			 pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipMessageInDlgReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipOptionsReq
����        : Options����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              cSipHead: KDSIP��Ϣͷ������KDSIP·����Ϣ
			  pcSipBody:KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
              eTransport ����Э��
����ֵ˵��  : KDSIP�����ʶ������Ψһ�ԣ�INVALID_TRANS_IDΪ��Ч��ʶ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipOptionsReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

/*====================================================================
������      : KDSipOptionsRsp
����        : OptionsӦ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: KDSIP�����ʶ
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipOptionsRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipSubscribeReq
����        : ��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              cSipHead: KDSIP��Ϣͷ������KDSIP·����Ϣ
			  pcSipBody:KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
              eTransport ����Э��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
#define DEFAULT_EXPIRES     (3600)
KDSIP_API KDSIP_RETURN KDSipSubscribeReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
 u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP,u32 dwExpire = DEFAULT_EXPIRES,BOOL32 bAutoRefreshWithBody =FALSE);

/*====================================================================
������      : KDSipSubscribeRefreshReq
����        : ˢ�¶�������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
              tDialogID: KDSIP�Ự��ʶ, subscribe ���ص�KDSIP_DIALOG_ID
			  pcSipBody:KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/12/16  0.2         ���          ����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipSubscribeRefreshReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipSubscribeRsp
����        : ����Ӧ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: KDSIP�����ʶ
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
			 pcSipHeader:����ҵ����дContactURI
			 bActiveNotifyByUser:TRUE����ҵ���ͼ���notify��
								 FALSE��kdsip���Ͳ�����Ϣ��notify
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL, BOOL32 bActiveNotifyByUser = FALSE);

/*====================================================================
������      : SipUnSubscribeReq
����        : ȡ����������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
			  tDialogID: KDSIP�Ự��ʶ, subscribe ���ص�KDSIP_DIALOG_ID
			  pcSipBody:KDSIP��Ϣ����ָ��
              dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2011/10/13  0.3         ���           �޸ķ���ֵ����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipUnSubscribeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);

/*====================================================================
������      : KDSipUnSubscribeRsp
����        : ȡ������Ӧ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tTransID: ����ID
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipUnSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);


/*====================================================================
������      : KDSipNotifyReq
����        : ����֪ͨ����
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tDialogID: incoming subscribe��KDSIP��ʶ
			 pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipNotifyReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN);


/*====================================================================
������      : SipNotifyRsp
����        : ����֪ͨӦ��
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tTransID: ����ID
			 eResponseCode: KDSIP�������Ļ�Ӧ��
			 pcSipBody:KDSIP��Ϣ����ָ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_API KDSIP_RETURN KDSipNotifyRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL);

/*====================================================================
������      : KDSipTerminateInviteDialog
����        : ǿ�ƽ������ص�Invite Dialog, ���ᷢ��SIP���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tInviteDialogID: �ỰID
����ֵ˵��  : ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/20  0.2         ���
====================================================================*/
KDSIP_API void KDSipTerminateInviteDialog(KDSIP_DIALOG_ID tInviteDialogID);

/*====================================================================
������      : KDSipTerminateSubsDialog
����        : ǿ�ƽ������صĶ���Dialog, ���ᷢ��SIP���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tSubsDialogID: �ỰID
����ֵ˵��  : ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/20  0.2         ���
====================================================================*/
KDSIP_API void KDSipTerminateSubsDialog(KDSIP_DIALOG_ID tSubsDialogID);

/*====================================================================
������      : KDSipTerminateRegDialog
����        : ǿ�ƽ������ص�ע��Dialog, ���ᷢ��SIP���
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tRegDialogID: �ỰID
����ֵ˵��  : ��
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/09/20  0.2         ���
====================================================================*/
KDSIP_API void KDSipTerminateRegDialog(KDSIP_DIALOG_ID tRegDialogID);

/*====================================================================
������      : SipSetLogLev
����        : ����KDSIP��Ĵ�ӡ����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             eLogLev: ��ӡ����
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
====================================================================*/
KDSIP_API void KDSipSetLogLev(EKDSIP_LOG_EVENT eLogLev);

/*==========================================================
������: KDSipSetLogOnScreen
����:
�㷨ʵ��:
����˵��: BOOL32 bIsOnScreen
����ֵ˵��:
------------------------------------------------------------
�޸ļ�¼:
�� ��      �汾  �޸���  �߶���  �޸ļ�¼
2012/02/17  1.0  ���              ����
===========================================================*/
KDSIP_API void KDSipSetLogOnScreen(BOOL32 bIsOnScreen);

/*==========================================================
������: KDSipSetLogShowTime
����:
�㷨ʵ��:
����˵��: BOOL32 bIsShowTime
����ֵ˵��:
------------------------------------------------------------
�޸ļ�¼:
�� ��      �汾  �޸���  �߶���  �޸ļ�¼
2012/02/17  1.0  ���              ����
===========================================================*/
KDSIP_API void KDSipSetLogShowTime(BOOL32 bIsShowTime);

/**
  ����kdsip��־ʱ����Ƿ�ȷ��ms����
  TRUE:  ��ȷ��ms����
  FALSE: ��ȷ��s����
*/
KDSIP_API void KDSipSetLogTimeStampLevel(BOOL32 bMsLevel);


// ��״̬����ص�����.
typedef KDSIP_RETURN (*PFKDSipStatelessProxyCB) ( // ����ֵΪ0 �� KDSIP������.
	void*                pvUserData,			  // �Զ�������.
	CKDSipEventForProxy& cSipEvent ,		      // �յ�����Ϣ�¼��࣬������Ϣͷ����Ϣ��.
	BOOL32&              bStatelessMod,           // �Ƿ������״̬����
	BOOL32&              bRequestURIResolved      // �Ƿ������URI
	/*LPSTR szDstIp, u16& dstPort */);            // ʹ��SetProxyURI���
//
// 		Proxy2
//   Proxy1 		Proxy3
// A       B			c
// 1 A����INVITE��proxy
// 2 A����INVITE��B
// 3 A����INVITE��C
//
// ������������ص��Ĵ������
// 1 �ص�����statelessModΪFALSE��proxy��ʱ��ͬ��ua
// 2 �޸�requestURI������statelessModΪTRUE ����requesstURIResolvedΪTRUE
// 3 ����statelessModΪTRUE ����requesstURIResolvedΪFALSE������ SetProxyURI����û�е�����Э��ջ����404


/*==============================================================================
  ������    : KDSipSetProxyCallback
  ����      : ������״̬����ص�����
  �㷨ʵ��  :
  ����˵��  : PFKDSipStatelessProxyCB pf, void * pvUserData
  ����ֵ˵��:
--------------------------------------------------------------------------------
  �޸ļ�¼:
  �� ��      �汾  �޸���  �߶���  �޸ļ�¼
  2011/10/21  1.0   ���              ����
==============================================================================*/
KDSIP_API KDSIP_RETURN KDSipSetProxyCallback(PFKDSipStatelessProxyCB pf, void * pvUserData);

/*==============================================================================
  ������    : KDSipGetDialogResource
  ����      : ��ȡ�������͵ĻỰʹ�õ���Դ��
  �㷨ʵ��  :
  ����˵��  : eDialogType :        Ҫ��ȡ�ĻỰ����
              ptDialogResource:    ��ȡ��Դ�Ĵ��λ��
  ����ֵ˵��:
--------------------------------------------------------------------------------
  �޸ļ�¼:
  �� ��      �汾  �޸���  �߶���  �޸ļ�¼
  2014/10/27  1.0   �ܽ���              ����
==============================================================================*/
KDSIP_API KDSIP_RETURN KDSipGetDialogResource(EKDSIP_DIALOG_TYPE eDialogType,TKDSIP_DIALOG_RESOURCE * ptDialogResorce);

/*
  ��ȡ������Դռ�����
*/
KDSIP_API void KDSipGetTransResource(TKDSIP_DIALOG_RESOURCE* ptResc);

KDSIP_API void KDSIPVer();        // ��ӡ�汾��
KDSIP_API void KDSIPShow();       // ��ӡ��Դ���
KDSIP_API void KDSIPDialogShow(); // ��ӡ�Ự��Դ
KDSIP_API void KDSIPTransShow();  // ��ӡ������Դ
#ifndef _MSC_VER
KDSIP_API void KDSIPShowRouteTable();//��ӡroteTable�е����
#endif
KDSIP_API void KDSIPInviteDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPSubsDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPRegDialogShow(KDSIP_DIALOG_ID hInviteDialog);
KDSIP_API void KDSIPTransShow(KDSIP_TRANS_ID hInviteDialog);

#endif //#ifndef _KDSIP_H_
