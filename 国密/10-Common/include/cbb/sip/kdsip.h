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

#define KDSIP_VERSION       "KDSIP_V2.2"       // kdsip�汾��
#define INVALID_DIALOG_ID   (KDSIP_DIALOG_ID)0 // ��ЧKDSIP��ʶ
#define INVALID_TRANS_ID    (KDSIP_TRANS_ID)0  // ��Ч�����ʶ
#define KDSIP_NO_PORT       (u16)-1            // ��Ч�˿�

// TKDSIP_INIT_CONFIG Ĭ��ֵ
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_PATH         "./"
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE              "kdsip.log"
#define KDSIP_INIT_CONFIG_DEFAULT_SECURITY_CAPABILITY   "A:SM2;H:SM3;S:SM1/OFB/PKCS5,SM1/CBC/PKCS5,SM4/OFB/PKCS5,SM4/CBC/PKCS5;SI:SM3-SM2"
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_SIZE         (2 * 1024 * 1024)
#define KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_NUM          10
#define KDSIP_INIT_CONFIG_DEFAULT_MAXIINVITEDIALOG      1024
#define KDSIP_INIT_CONFIG_DEFAULT_MAXREGISTERDIALOG     128
#define KDSIP_INIT_CONFIG_DEFAULT_MAXSUBSCRIBEDIALOG    1024
#define KDSIP_INIT_CONFIG_DEFAULT_MAXTRANSCATION        1024
#define KDSIP_INIT_CONFIG_DEFAULT_RETRANSMITTIMER       500
#define KDSIP_INIT_CONFIG_DEFAULT_LINGERTIMER           32000
#define KDSIP_INIT_CONFIG_DEFAULT_MSG_SIZE_MAX          32 * 1024
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

	KDSIP_EVENT_DO_REQ,
	KDSIP_EVENT_DO_RSP,

	KDSIP_EVENT_SUBSCRIBE_REQ,
	KDSIP_EVENT_SUBSCRIBE_RSP,

	KDSIP_EVENT_UNSUBSCRIBE_REQ,
	KDSIP_EVENT_UNSUBSCRIBE_RSP,

	KDSIP_EVENT_NOTIFY_REQ,
	KDSIP_EVENT_NOTIFY_RSP,

	KDSIP_EVENT_NOTIFY_OUTDLG_REQ,
	KDSIP_EVENT_NOTIFY_OUTDLG_RSP,

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
	KDSIP_NONSUPPORT,           //�ӿڲ�֧��
	KDSIP_BUFFSIZE_SMALL,       //������buff��С
	KDSIP_DEVICEAUTHEN_FAIL,    //��Ϣ��֤ʧ��
	KDSIP_KEYCONSULT_FAIL,     //��ԿЭ��ʧ��
	KDSIP_CHECK_FAIL,          //checkʧ��
	KDSIP_CIPHER_FAIL,         //�ӽ���ʧ��
	KDSIP_NOSECCB,             //û�а�ȫ�㷨�ص�
	KDSIP_NOTFIND,             //û���ҵ���Դ
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
	KDSIP_CONTENT_KSPTZ,        //�Զ�������application/KSPTZ
	KDSIP_CONTENT_DDCP,         //�ر�����
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

class CKDSipURITOOL
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

class CKDSipAUTHTOOL
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
class CKDSipMsgHead
{
public:
	CKDSipMsgHead();
	virtual ~CKDSipMsgHead();

	LPCSTR GetToURI() const;
	LPCSTR GetWholeToHeader() const;
	LPCSTR GetFromURI() const;
	LPCSTR GetWholeFromHeader() const;
	LPCSTR GetProxyURI() const;
    LPCSTR GetContactURI() const;
	LPCSTR GetUserAgent() const;    // GB28181
    LPCSTR GetSubject() const;      // GB28181
    LPCSTR GetDate() const;         // GB28181
    LPCSTR GetAuthorization() const;                 // GB28181
    LPCSTR GetWWWAuthenticateRealm() const;                // �ͻ��˴����һ��ע���Ӧ�𣬻�ȡrealm
    LPCSTR GetCallID() const;                // ��ȡ��Ϣ��CallID

	LPCSTR GetWWWAuthenticate()const;
	LPCSTR GetSecurityInfo()const;
	LPCSTR GetMonitorUserIdentity()const;
	LPCSTR GetNote()const;
	LPCSTR GetNoteAlgorithm()const;
	u32    GetAlgorithmEngineId()const;
	LPCSTR GetNoteVkek()const;

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

	KDSIP_RETURN SetAuthorization(LPCSTR szAuthorization);
	KDSIP_RETURN SetWWWAuthenticate(LPCSTR szWWWAuthenticate);
	KDSIP_RETURN SetSecurityInfo(LPCSTR szSecurityInfo);
	KDSIP_RETURN SetMonitorUserIdentity(LPCSTR szMonitorUserIdentity);

	KDSIP_RETURN SetArgForNote(LPCSTR szNoteAlgorithm,u32 dwAlgorithmEngineId,LPCSTR szNoteVkek);

    LPSTR m_szUsername;
    LPSTR m_szPassword;
protected:
    LPSTR m_szTo;
	LPSTR m_szWholeTo;//����ȫ����Ϣ��to�ֶ�
    LPSTR m_szFrom;
	LPSTR m_szWholeFrom;//����ȫ����Ϣ��from�ֶ�(����from_tag)
    LPSTR m_szProxy;
    LPSTR m_szContact;
    LPSTR m_szUserAgent; // GB28181
    LPSTR m_szSubject;  // GB28181
    LPSTR m_szDate;     // GB28181
    LPSTR m_szRealm;
    LPSTR m_szAuthorization;
    LPSTR m_szCallID; // ���ڱ�ʶ��ͬ��ע��ػ�

	LPSTR m_szWWWAuthenticate;
	LPSTR m_szSecurityInfo;
	LPSTR m_szMonitorUserIdentity;
	LPSTR m_szNoteAlgorithm;//Ϊ���������м���note��ɢ���㷨
	u32   m_dwAlgorithmEngineId;//Ӳ�������ǵ�Ӳ��ģ��id,0��ʾ�������
	LPSTR m_szNoteVkek;
	LPSTR m_szNote;
};

// ��Ϣ����
class CKDSipMsgBody
{
public:
	CKDSipMsgBody();
	virtual ~CKDSipMsgBody();

	const u8*           GetMsgBodyContent(s32* pnMsgLen = NULL) const;
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
class CKDSipEvent
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
class CKDSipMsgHeadForProxy:public CKDSipMsgHead
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
class CKDSipEventForProxy:public CKDSipEvent
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

typedef struct tagKdSipKeyArg 
{
	LPCSTR szKey;  
	s32    nKeyType;//�㷨���ͣ�kdssl-ext_api.h����EKeyType���ö�������ֵ��
	s32    nKeyDataType;//�㷨��������,(kdssl-ext_api.h����EDataType���ö�������ֵ)
	LPCSTR szPassword;//˽Կ��˵�����룬���벻�Բ���ȥ��֤
	u32 dwEngineId;//Ӳ��������id,���������0
}TKdSipKeyArg;

enum EKDSIP_AUTH_VALUE{
	UNKNOWN_TYPE,
	DIGEST,
	CAPABILITY,
	UNIDIRECTION,
	BIDIRECTION
};

enum EKDSIP_CALLBACK_TYPE
{
    KDSIP_CALLBACK_ENCRYPT,
    KDSIP_CALLBACK_DECRYPT,
	KDSIP_CALLBACK_HASH,
    KDSIP_CALLBACK_SIGN,
    KDSIP_CALLBACK_VERIFY
};

/*********************************************************************************
KDGbGetVkekCallBack :��ȡָ��id��vkek
���� :
       szDeviceName���豸ID
	   szvkekbuffer : ���vkek��buffer
       szvkekBufferlen : ���vkek��buffer�ĳ���
	   EngineID:         ����ṩ�Ӵ������Ӳ��ģ��ID,0��ʾ���������
������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**********************************************************************************/
typedef KDSIP_RETURN (*KDGbGetVkekCallBack)(const char* szDeviceName,
	                                              char* szvkekbuffer,
												  s32 szvkekBufferlen,
												  u32* EngineID);


/*********************************************************************************
KDSipSecurityCalCallBack :�ӽ��ܡ�ɢ�С�ǩ����ǩ�ص�
���� :
       eCallBackType�����μ��������(���ܣ����ܣ��Ӵգ�ǩ������ǩ)
	   szAlgorithm : �����õ��㷨�ַ���
       szSrc : ������Ҫ�õ�����Դ
	   dwSrcLen:������Ҫ�õ�����Դ����
       szKey �������õ���Կ
	   nKeyType:�㷨���ͣ�kdssl-ext_api.h����EKeyType���ö�������ֵ��
	   szKeyPassword��˽Կ�Ļ���Ҫ������
	   dwEngineId: �Ӵ�������Ҫ���Ӧid��Ӳ��ģ��ID,0��ʾ���������
       szResultBuffer����ż�������buffer(���eCallBackType��KDSIP_CALLBACK_VERIFY����ǩ���������ŵ�����Ҫ�ȶԵĽ���ַ���)
	   ReSultBufferLen��*ReSultBufferLenc����ʱ�������szResultBuffer��С
						�����szResultBuffer��С������*ReSultBufferLen�������������buffer��С
						�ɹ������ʵ���ַ�������(����'\0')
������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**********************************************************************************/

typedef  KDSIP_RETURN (*KDSipSecurityCalcCallBack)(EKDSIP_CALLBACK_TYPE eCallBackType,
												   LPCSTR szAlgorithm,
											       LPCSTR szSrc,
												   u32    dwSrcLen,
												   TKdSipKeyArg* pKeyArg,
												   u32 dwEngineId,
											       LPSTR  szResultBuffer,
											       u32* ReSultBufferLen
											      );

// KDSIP��ʼ������
struct TKDSIP_INIT_CONFIG
{
    LPCSTR              szLogPath;           // ��־�ļ�·��
    LPCSTR              szLogFile;           // ��־�ļ� Ĭ����־����ΪKDSIP_LOG_EVENTENT
	LPCSTR              szSecCap;             // ���豸֧�ֵİ�ȫ�����㷨
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
	KDSipSecurityCalcCallBack pfSecCalcCB;   // ��ȫ�㷨����ص�
//	KDGbGetVkekCallBack pfGetVkekCB;         // ��ȡvkek�Ļص�
    void*               pvEventCBContext;    // ��Ϣ�ص��û�������

    s32                 nMaxInviteDialog;    // ���Invite Dialog��
    s32                 nMaxRegisterDialog;  // ���Register Dialog��
    s32                 nMaxSubscribeDialog; // ���Subscribe Dialog��
	s32                 nNumOfNotifysPerSub; // ÿ���Ự��notify��
    s32                 nMaxTrans;           // ���������
    s32                 nRetransmitTimer;    // �ش���ʱ ms
    s32                 nTransLingerTimer;    // ����ȴ����ٶ�ʱ ms
    s32                 nMsgSizeMax;         // ��Ϣ��󳤶�

    s32                 nProcessingQueueSize; //�ײ���Ϣ���г��� 0Ĭ��nMaxInviteDialog + .nMaxSubscribeDialog + .nMaxRegisterDialog + .nMaxTrans
    s32                 nReadBufferNum;       //�ײ������������� 0 Ĭ�� nProcessingQueueSize/4
    s32                 nMaxTransmitter;      //�ײ�transmitter�������ֵ 0 Ĭ�� nMaxTrans+10

    TKDSIP_INIT_CONFIG()
    {
        szLogPath           = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE_PATH;
        szLogFile           = KDSIP_INIT_CONFIG_DEFAULT_LOG_FILE;
		szSecCap            = KDSIP_INIT_CONFIG_DEFAULT_SECURITY_CAPABILITY;
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
		pfSecCalcCB         = NULL;
//		pfGetVkekCB         = NULL;
        pvEventCBContext    = NULL;
        nMaxInviteDialog    = KDSIP_INIT_CONFIG_DEFAULT_MAXIINVITEDIALOG;
        nMaxRegisterDialog  = KDSIP_INIT_CONFIG_DEFAULT_MAXREGISTERDIALOG;
        nMaxSubscribeDialog = KDSIP_INIT_CONFIG_DEFAULT_MAXSUBSCRIBEDIALOG;
		nNumOfNotifysPerSub = KDSIP_INIT_CONFIG_DEFAULT_NUMOFNOTIFYPERSUB;
        nMaxTrans           = KDSIP_INIT_CONFIG_DEFAULT_MAXTRANSCATION;
        nRetransmitTimer    = KDSIP_INIT_CONFIG_DEFAULT_RETRANSMITTIMER;
        nTransLingerTimer   = KDSIP_INIT_CONFIG_DEFAULT_LINGERTIMER;
        nMsgSizeMax         = KDSIP_INIT_CONFIG_DEFAULT_MSG_SIZE_MAX;
        nProcessingQueueSize = 0;
        nReadBufferNum = 0;
        nMaxTransmitter = 0;
    }
};

//add by zhoujc 2014/10/24
typedef struct
{
	s32 dwUsage;
    s32 dwMax;
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
CKDSipEvent* KDSipPollSipEvent(u32 dwMillSec);

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
void KDSipFreeSipEvent(CKDSipEvent* pCSipEvent);

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
KDSIP_RETURN KDSipInit(LPSTR szLocalUDPIp = NULL, u16 wLocalUDPPort = 5060,
	           const TKDSIP_INIT_CONFIG * ptInitConfig = NULL);


/*====================================================================
������      : KDSipUpdateSecCap
����        : ���°�ȫ�㷨������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
			szNewSecCap :��Ҫ���µİ�ȫ�㷨
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipUpdateSecCap(LPCSTR szNewSecCap);

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
void KDSipQuit();


 /*-------------------------------------------------------------------------------*
  | GB28181 Security  ��ؽӿ�                                                                 |
  *-------------------------------------------------------------------------------*/

class CKDSipAlgorithmTOOL
{
public:
	/*==============================================================================
      ������    : CKDSipAlgorithmTOOL
      ����      : �Ӹ�����ͷ���л�ȡȡЭ�̺���㷨(Э�̺�ÿ���㷨ֻ��һ�֡�
				 ������ӵ�һ�ε�ע�������л�ȡ�����㷨������ÿ���㷨�ж���)
      �㷨ʵ��  : CKDSipAlgorithmTOOL ���캯����ͨ���ַ����������ַ����к��Ӵ�
	  algorithm="A:SM2;H:SM3;S: SM1/OFB/PKCS5,SM1/CBC/PKCS5,SM4/OFB/PKCS5,SM4/CBC/PKCS5;SI:SM3-SM2"�е�
	  "A:SM2;H:SM3;S: SM1/OFB/PKCS5,SM1/CBC/PKCS5,SM4/OFB/PKCS5,SM4/CBC/PKCS5;SI:SM3-SM2"�ִ��������԰�������
	  ȡ�ö�Ӧ�ִ������û��ָ���ִ����ͣ����㷨�ַ���ֻ�������ȡ��
	  �����ֶ�
      ����˵��  : LPCSTR szURI
      ����ֵ˵��:
    --------------------------------------------------------------------------------
      �޸ļ�¼:
      �� ��      �汾  �޸���  �߶���  �޸ļ�¼
      2017/07/12  1.0   zhoujc              ����
	==============================================================================*/
    CKDSipAlgorithmTOOL(LPCSTR szInString);
    virtual ~CKDSipAlgorithmTOOL();
	LPCSTR GetWholeAlgorithmString();
    LPCSTR GetAsymmetricAlgorithm();
    LPCSTR GetHashAlgorithm();
    LPCSTR GetSymmetricAlgorithm();
    LPCSTR GetSignatureAlgorithm();

	CKDSipAlgorithmTOOL& operator=(const CKDSipAlgorithmTOOL& tother){
		CopyOtherStruct(tother);
		return *this;
	}
private:
	void FindWholeAlgorithm(LPCSTR szInString);
	void FindAsymmetricAlgorithm(LPCSTR szWholeAlgorithm);
	void FindHashAlgorithm(LPCSTR szWholeAlgorithm);
	void FindSymmetricAlgorithm(LPCSTR szWholeAlgorithm);
	void FindSignatureAlgorithm(LPCSTR szWholeAlgorithm);
	void CopyOtherStruct(const CKDSipAlgorithmTOOL& tother);

private:
	LPSTR m_szWholeAlgorithm;//�����㷨�ַ���(�磺"A:SM2;H:SM3;S: SM1/OFB/PKCS5,SM1/CBC/PKCS5,SM4/OFB/PKCS5,SM4/CBC/PKCS5;SI:SM3-SM2")
    LPSTR m_szAsymmetric;    //�ǶԳ��㷨 A�ֶ�(�磺SM2)
    LPSTR m_szHash;          //�Ӵ��㷨   H�ֶ�(�磺SM3)
    LPSTR m_szSymmetric;     //�Գ��㷨   S�ֶ�(�磺SM1/OFB/PKCS5)//Э�̺ú�ֻ��һ��
    LPSTR m_szSignature;     //ǩ���㷨   SI�ֶ�(�磺SM3-SM2)
};


EKDSIP_AUTH_VALUE KDSip_GetAuthType(LPCSTR szAuth);//szAuth ���յ���Authorization����WWW-Authenticateͷ


/***********************************************************************************************
*������    :KDSip_GetKeyVersion 
*����      :�ӵ�һ��ע�������л�ȡkeyversion�ַ���
*�㷨ʵ��  :
*           
*���� :
*        szAuthor           ---  ��һ��ע�������Author�ֶ�
*        szKeyVesionBuffer  ---  ���ڴ洢���ɵ�Author�ֶ�
*        pnBufferLen        ---  ���keyverySion��buffer��Сָ��
*������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**************************************************************************************************/
KDSIP_RETURN  KDSip_GetKeyVersion(LPCSTR szAuthor,LPSTR szKeyVesionBuffer,s32* pnBufferLen);

/***********************************************************************************************
*������    :KDSip_BuildAuthor 
*����      :����Authorizationͷ
*�㷨ʵ��  :����eAuthorValue���͹�����Ӧ���ֶ����ݣ������cability�������kdsipinit�����õ�����
*           ����һ���ֶγ�ȥ������keyversion��kdsipȡ��ǰʱ��������ȥ��
*           ��ֵ��UNIDIRECTION����BIDIRECETTIONʱ���հ�ȫ�ĵ������Ӧ�ֶ�
*           
*���� :
*        eAuthorValue         ---  Authorizationͷ�����ͣ�ȡֵCAPABILITY/UNIDIRECTION/BIDIRECTION
*                                  ��һ��ע������ʱ��CAPABILITY���ڶ���ע������ʱ��401Ӧ���л�ȡ
*		 szServerId           ---  ע��������ն˵�id�����ڽ��ն�У��
*		 szReceiveAuthen      ---  ��401�л�ȡ��www-Authenͷ �����ڻ�ȡ401���ص�random1
*		 szRandom2            ---  ע�ᷢ��˵�������ģ����������������������sign�ֶ�
*        szKeyVersion         ---  ��һ��ע��ʱЯ����keyversion�ֶΣ���NULLʱkdsip�Զ��ǰʱ��
*        ptUacPrivateKeyArg   ---  ע�ᷢ��˵�˽Կ�����ڼ���sign�ֶ�
*        szStorageAuthorBuff  ---  ���ڴ洢���ɵ�Author�ֶ�
*        dwAuthorBuffLen      ---  szStorageAuthorBuff�Ĵ�С
*������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**************************************************************************************************/
KDSIP_RETURN KDSip_BuildAuthor(EKDSIP_AUTH_VALUE eAuthorValue,
							    LPCSTR szServerId ,        
								LPCSTR szReceiveAuthen,    
								LPCSTR szRandom2,
								LPCSTR szKeyVersion,
								TKdSipKeyArg* ptUacPrivateKeyArg,                      
								LPSTR  szStorageAuthorBuff,    
								u32    dwAuthorBuffLen      
								);

/***********************************************************************************************
*������    :KDSip_BuildAuthen 
*����      :����WWW-Authenticateͷ
*�㷨ʵ��  :����eAuthorValue���͹�����Ӧ���ֶ����ݣ�ȡֵUNIDIRECTION/BIDIRECTION
*           
*���� :
*        eAuthorValue         ---  Authorizationͷ�����ͣ�ȡֵUNIDIRECTION/BIDIRECTION
*		 szReceiveAuthor      ---  �ӵ�һ��ע�������л�ȡ��Authorizationͷ�����ڻ�ȡ�㷨
*		 szRandom1            ---  ע����ն˵�������ģ�������random1�����
*        szStorageAuthenBuff  ---  ���ڴ洢���ɵ�WWW-Authenticateͷ��ֵ������ʹ���������ֵ����
*                                  CKDSipAlgorithmTOOL����Ӷ����Э�̺���㷨
*        dwAuthenBuffLen      ---  szStorageAuthenBuff�Ĵ�С
*������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**************************************************************************************************/
KDSIP_RETURN KDSip_BuildAuthen(EKDSIP_AUTH_VALUE eAuthorValue,  
								LPCSTR szReceiveAuthor,   
								LPCSTR szRandom1,         
								LPSTR  szStorageAuthenBuff,     
								u32    dwAuthenBuffLen         
							  );


/***********************************************************************************************
*������    :KDSip_CheckAuthor 
*����      :У��Authorizationͷ
*�㷨ʵ��  :����eAuthorValue���ͣ����հ�ȫ�ĵ���random2,random1,serviceid��ϳ�һ��buffer����ǩ�����Դ
*           ����ȡ��sign��base64_decode���㣬��������Ľ������ǩ�����Ԥ�ڽ����
*           
*���� :
*        ptUacPublicKeyArg    ---  ע�����󷽵Ĺ�Կ��������ǩ
*		 szServerId           ---  ע����ն��Լ���id������У���Ƿ��������д���idһ��
*        szReceiveAuthor      ---  �յ���ע�������е�Authorization�ֶ�
*������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**************************************************************************************************/
KDSIP_RETURN KDSip_CheckAuthor(TKdSipKeyArg* ptUacPublicKeyArg,              //uac��Կ����DIGESTʱuas����
							   LPCSTR szServerId,                //uas�Լ����豸id
							   LPCSTR szReceiveAuthor      //�������н��յ��Ļ�ȡ��Authorͷ����DIGESTʱuas����
							   );

/***********************************************************************************************
*������    :KDSip_BuildSecurityInfo 
*����      :����SecurityInfoͷ
*�㷨ʵ��  :����eAuthorValue���ͣ����հ�ȫ�ĵ�������ֶ���ϼ��㣬�õ�SecurityInfoͷ
*           
*���� :
*        eSecInfoValue        ---  ȡֵUNIDIRECTION/BIDIRECTION����Ӧ�ڵ���/˫����֤
*		 ptUacPublicKeyArg    ---  ע�����󷽵Ĺ�Կ�����ڼ���cryptkey���ֶ�
*		 ptUasPrivateKeyArg   ---  ע����ն��Լ���˽Կ����������sign�ֶ�
*        szReceiveAuthor      ---  �յ���ע�������е�Authorization�ֶ�
*������ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
**************************************************************************************************/
KDSIP_RETURN KDSip_BuildSecurityInfo(EKDSIP_AUTH_VALUE eSecInfoValue,
									TKdSipKeyArg* ptUacPublicKeyArg,//uac��Կ����DIGESTʱuas����
									TKdSipKeyArg* ptUasPrivateKeyArg,   //uas˽Կ������eAuthorValue��DIGESTʱuas����
									LPCSTR szVKEK,                  //VKEK
									LPCSTR szVKMK,                  //VKMK
									LPCSTR szVKMKVerSion,           //VKMK version
									LPCSTR szClientId,              //uac���豸id
									LPCSTR szReceiveAuthor,        //�������н��յ��Ļ�ȡ��Authorͷ����DIGESTʱuas����
									LPSTR  szStorageSecInfoBuff,    //���ڴ洢���ɵ�SecurityInfo�ֶ�
									u32    dwSecInfoLen             //���ڴ���洢�ռ��С
							       );

KDSIP_RETURN KDSip_CheckSecurityInfo(LPCSTR szClientId,                //uac�Լ����豸id
									 TKdSipKeyArg* ptUasPublicKeyArg,//uas��Կ����DIGESTʱuas����
									 TKdSipKeyArg* ptUacPrivateKeyArg,   //uac˽Կ������eAuthorValue��DIGESTʱuac����
									 LPCSTR szSecurityInfo,         //���յ���SecurityInfoͷ
									 LPSTR   szStorageVKEKBuff,      //����vkek��buff
									 LPSTR  szStorageVEMKBuff,      //
									 u32    dwStorageVKEKBuffLen,
									 u32    dwStorageVEMKBuffLen
                                    );
/***********************************************************************************************
*������    :KDSip_SetCallBackEventLog 
*����      :���üӽ��ܻص��Ĵ�ӡ��־
*�㷨ʵ��  :
*           
*���� :
*        dwEventLevel       ---- 0x0    ����ӡ
*                           ---  0x1    ��ӡKDSIP_CALLBACK_ENCRYPT�లȫ�����¼�������vkek�õ���
*                           ---  0x1<<1 ��ӡKDSIP_CALLBACK_DECRYPT�లȫ�����¼������ܻ��vkek�õ���
*                           ---- 0x1<<2 ��ӡKDSIP_CALLBACK_HASH�లȫ�����¼�   ������noteʱ�õ���
*                           -----0x1<<3 ��ӡKDSIP_CALLBACK_SIGN�లȫ�����¼�   ������ǩ��ʱ�õ���
*                           -----0x1<<4 ��ӡKDSIP_CALLBACK_VERIFY�లȫ�����¼� ��������ǩ���õ���
*                           �۲����¼�ʱ��������ֵΪ|�Ľ������
*        
*������������õ�ֵ
**************************************************************************************************/
u32  KDSip_SetCallBackEventLog(u32 dwEventLevel);

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
KDSIP_RETURN KDSipRegisterReq(const CKDSipMsgHead& cSipHead, s32 nExpires,
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
KDSIP_RETURN KDSipRegisterRefreshReq(KDSIP_DIALOG_ID tDialogID, s32 nExpires,
	const CKDSipMsgBody* pcSipBody, u32& dwSN);
KDSIP_RETURN KDSipRegisterReqWithHeader(KDSIP_DIALOG_ID tDialogID, s32 nExpires, const CKDSipMsgHead* pcSipHeader,
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
KDSIP_RETURN KDSipRegisterRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody);

KDSIP_RETURN KDSipRegisterRspWithHeader(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,const CKDSipMsgHead* pcSipHeader,
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
KDSIP_RETURN KDSipInviteReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport = KDSIP_TRANSPORT_UDP);

typedef struct
{
	EKDSIP_EVENT eKdsipEvent;
	LPCSTR szNote;
	LPCSTR szWholeFrom; //����ȫ����Ϣ��from�ֶ�
	LPCSTR szWholeTo;//����ȫ����Ϣ��to�ֶ�
	LPCSTR szCallID;
	LPCSTR szDate;
	LPCSTR szBody;
}TCHECKNOTEARG;
/*====================================================================
������      :KDSip_CheckNote
����        :У��noteֵ�Ƿ���ȷ
�������˵��:dwEngineId ������hash������豸ID��Ϊ0��ʾʹ���������

====================================================================*/
KDSIP_RETURN KDSip_CheckNote(const TCHECKNOTEARG *ptCheckNoteArg,u32 dwEngineId,LPCSTR szVKEK,BOOL32* bIsAuthMessage);
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
KDSIP_RETURN KDSipInviteRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipInviteACK
����        : KDSIP����ACK
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
			 pcSipBody:KDSIP��Ϣ����ָ��
			 pcSipHeader:��Ϣͷָ�룬������������NoteArg�����Ա��²�����note�ֶ�
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipInviteACK(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipByeReq
����        : KDSIP���йҶ�����
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, invite ���ص�KDSIP_DIALOG_ID
             pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
			 pcSipHeader:��Ϣͷָ�룬������������NoteArg�����Ա��²�����note�ֶ�
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
====================================================================*/
KDSIP_RETURN KDSipByeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipByeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipInfoReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipInfoRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipMessageReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
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
KDSIP_RETURN KDSipMessageRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipMessageInDlgReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipDoReq
����        : Do����
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
2014/06/04  2.0         �ܽ���         �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipDoReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport= KDSIP_TRANSPORT_UDP);

/*====================================================================
������      : KDSipDoRsp
����        : DoӦ��
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
2014/06/04  2.0         �ܽ���         �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipDoRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode, const CKDSipMsgBody* pcSipBody=NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipDoInDlgReq
����        : ������Do����
�㷨ʵ��    : �Ի�������
����ȫ�ֱ���:
�������˵��:
             tDialogID: KDSIP�Ự��ʶ, �Ự���ص�KDSIP_DIALOG_ID
			 pcSipBody:KDSIP��Ϣ����ָ��
             dwSN :��ˮ��
����ֵ˵��  : �ɹ�����KDSIP_OK��ʧ�ܷ�������ֵ
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/08/01  0.1         fanxg
2011/09/20  0.2         ���           �����ӿ�����
2013/03/28  2.0         ���           �����ӿڲ���
2014/06/04  2.0         �ܽ���         �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipDoInDlgReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipOptionsReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
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
KDSIP_RETURN KDSipOptionsRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipSubscribeReq(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
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
KDSIP_RETURN KDSipSubscribeRefreshReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL,BOOL32 bActiveNotifyByUser = FALSE);

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
KDSIP_RETURN KDSipUnSubscribeReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);

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
KDSIP_RETURN KDSipUnSubscribeRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
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
KDSIP_RETURN KDSipNotifyReq(KDSIP_DIALOG_ID tDialogID, const CKDSipMsgBody* pcSipBody, u32& dwSN,const CKDSipMsgHead* pcSipHeader = NULL);


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
KDSIP_RETURN KDSipNotifyRsp(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode,
	const CKDSipMsgBody* pcSipBody = NULL,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipNotifyReqOutDlg
����        : KDSipNotifyReqOutDlg����
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
2014/06/04  2.0         �ܽ���         �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipNotifyReqOutDlg(const CKDSipMsgHead& cSipHead, const CKDSipMsgBody* pcSipBody,
	u32& dwSN, EKDSIP_TRANSPORT eTransport= KDSIP_TRANSPORT_UDP,const CKDSipMsgHead* pcSipHeader = NULL);

/*====================================================================
������      : KDSipNotifyRspOutDlg
����        : KDSipNotifyRspOutDlgӦ��
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
2014/06/04  2.0         �ܽ���         �����ӿ�����
====================================================================*/
KDSIP_RETURN KDSipNotifyRspOutDlg(KDSIP_TRANS_ID tTransID, EKDSIP_RESPONSE_CODE eResponseCode, const CKDSipMsgBody* pcSipBody=NULL,const CKDSipMsgHead* pcSipHeader = NULL);

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
void KDSipTerminateInviteDialog(KDSIP_DIALOG_ID tInviteDialogID);

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
void KDSipTerminateSubsDialog(KDSIP_DIALOG_ID tSubsDialogID);

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
void KDSipTerminateRegDialog(KDSIP_DIALOG_ID tRegDialogID);

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
void KDSipSetLogLev(EKDSIP_LOG_EVENT eLogLev);

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
void KDSipSetLogOnScreen(BOOL32 bIsOnScreen);

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
void KDSipSetLogShowTime(BOOL32 bIsShowTime);

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
KDSIP_RETURN KDSipSetProxyCallback(PFKDSipStatelessProxyCB pf, void * pvUserData);

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
KDSIP_RETURN KDSipGetDialogResource(EKDSIP_DIALOG_TYPE eDialogType,TKDSIP_DIALOG_RESOURCE * ptDialogResorce);


/*==============================================================================
  ������    : KDSipBase64_encode
  ����      : ��������������base64�ı������
  �㷨ʵ��  :
  ����˵��  : pDstBuffer :        ��Ŵ�������Ľ����buffer
			  pDstBufferLen:      ��Ž����buffer���ȵĵ�ַ�������ǳ��ȣ�����Ǵ����ĳ���
			                     (��������ĳ��������Դ���ݳ��ȵ�4/3+3)			                      
              pSrcBuffer:         �����������buffer��ַ
			  nSrcBufferLen��     ����������ݳ���
  ����ֵ˵��: �ɹ�����0��ʧ�ܷ��ش����룬���������DstBuffer̫С��ϣ����dstbuffer���ȴ����pDstBufferLen��
--------------------------------------------------------------------------------
  �޸ļ�¼:
  �� ��      �汾  �޸���  �߶���  �޸ļ�¼
  2014/10/27  1.0   �ܽ���              ����
==============================================================================*/
KDSIP_RETURN  KDSipBase64_encode( unsigned char *pDstBuffer, int *pDstBufferLen,unsigned char *pSrcBuffer, int  nSrcBufferLen );

/*==============================================================================
  ������    : KDSipBase64_decode
  ����      : ��������������base64�Ľ������
  �㷨ʵ��  :
  ����˵��  : pDstBuffer :        ��Ŵ�������Ľ����buffer
			  pDstBufferLen:      ��������ĳ������Դ���ݵ�3/4
              pSrcBuffer:         �����������buffer��ַ
			  nSrcBufferLen��     ����������ݳ���
  ����ֵ˵��: �ɹ�����0��ʧ�ܷ��ش����룬���������DstBuffer̫С��ϣ����dstbuffer���ȴ����pDstBufferLen��
--------------------------------------------------------------------------------
  �޸ļ�¼:
  �� ��      �汾  �޸���  �߶���  �޸ļ�¼
  2014/10/27  1.0   �ܽ���              ����
==============================================================================*/
KDSIP_RETURN  KDSipBase64_decode( unsigned char *pDstBuffer, int *pDstBufferLen,unsigned char *pSrcBuffer, int  nSrcBufferLen );



void KDSIPVer();        // ��ӡ�汾��
void KDSIPShow();       // ��ӡ��Դ���
void KDSIPDialogShow(); // ��ӡ�Ự��Դ
void KDSIPTransShow();  // ��ӡ������Դ
#ifndef _MSC_VER
void KDSIPShowRouteTable();//��ӡroteTable�е����
#endif
void KDSIPInviteDialogShow(KDSIP_DIALOG_ID hInviteDialog);
void KDSIPSubsDialogShow(KDSIP_DIALOG_ID hInviteDialog);
void KDSIPRegDialogShow(KDSIP_DIALOG_ID hInviteDialog);
void KDSIPTransShow(KDSIP_TRANS_ID hInviteDialog);
void KDSipLog(u32 dwLogType, LPCSTR szFormat, ...);

#endif //#ifndef _KDSIP_H_
