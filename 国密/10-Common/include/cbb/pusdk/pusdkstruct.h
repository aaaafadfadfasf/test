#ifndef PUSDKSTRUCT_H_
#define PUSDKSTRUCT_H_

#include "pusdkdefine.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// ͨ����ṹ����

// �豸GUID�ṹ
typedef struct
{
    unsigned long    m_dwData1;
    unsigned short   m_wData2;
    unsigned short   m_wData3;
    unsigned char    m_abyData4[8];
} TPuGuid;

// �豸��Ϣ�ṹ
typedef struct
{
    TPuGuid        m_tGuid;                                 // �豸GUIDֵ(ֻ��)
    char           m_achName[PUSDK_MAX_STRING_LEN64+1];     // �豸��
    char           m_achTypeName[PUSDK_MAX_STRING_LEN64+1]; // �豸������(ֻ��)
    char           m_achSeriesName[PUSDK_MAX_STRING_LEN16]; // �豸ϵ����
    unsigned char  m_byDevType;                             // �豸����(EPuDeviceType)
    unsigned long  m_dwUnitIpAddr;                          // �豸IP��ַ
    unsigned short m_wVsipPort;                             // VSIP�˿ں�
} TPuUnitInfo;

// �豸�����ص������ⲿΨһ����ʹ�õĶ��壬ֻ�ܷ������
typedef int (*fPuSearch)(TPuUnitInfo *ptPuInfo, void *pContext);

// �����豸����
typedef struct
{
    unsigned long  dwIpAddr;                // ������ַ
    unsigned short wPort;                   // �����˿�
} TPuAddressInfo;

//////////////////////////////////////////////////////////////////////////
// �豸״̬
//////////////////////////////////////////////////////////////////////////

// ������ͳ����Ϣ
typedef struct
{
	unsigned long   m_dwDecSSRC;       /*SSRC100608yuleiadd*/
    unsigned char   m_byVideoFrameRate;//��Ƶ����֡��
    unsigned short  m_wVideoBitRate;   //��Ƶ��������
    unsigned short  m_wAudioBitRate;   //������������
    unsigned long   m_dwVideoRecvFrame;//�յ�����Ƶ֡��
    unsigned long   m_dwAudioRecvFrame;//�յ�������֡��
    unsigned long   m_dwVideoLoseFrame;//��ʧ����Ƶ֡��
    unsigned long   m_dwAudioLoseFrame;//��ʧ������֡��
    unsigned short  m_wVideoLoseRatio;//��Ƶ��ʧ��,��λ��%     
    unsigned short  m_wAudioLoseRatio;//��Ƶ��ʧ��,��λ��%     
    unsigned long   m_dwPackError;     //��֡�� 
    unsigned long   m_dwIndexLose;//��Ŷ�֡
    unsigned long   m_dwSizeLose; //��С��֡
    unsigned long   m_dwFullLose; //����֡    
    unsigned long   m_wAvrVideoBitRate;   //1��������Ƶ����ƽ������
    unsigned long   m_wAvrAudioBitRate;   //1��������������ƽ������
    int             m_bVidCompellingIFrm;  //��Ƶǿ������I֡                                  
    unsigned long   m_dwDecdWidth;    //�����Ŀ�
    unsigned long   m_dwDecdHeight;   //�����ĸ�
} TPuDecStatis;

// ϵͳ����״̬
typedef struct 
{
    unsigned char      byNatSend;              // ̽�������   
    unsigned char      byTimerSyn;             // ʱ��ͬ������ 
    unsigned char      bySocks5Agent;          // socks5����   
    unsigned char      byDns;                  // DNS����      
    unsigned char      byPppoe;                // pppoe����    
    unsigned char      bySnmpAgent;            // ���ܴ���     
    unsigned char      bySysGuard;             // ϵͳ����     
    unsigned char      byDhcp;                 // DHCP         
    unsigned char      byDdns;                 // DDNS         
    unsigned char      byWifi;                 // wifi         
    unsigned char      by3g;                   // 3G           
    unsigned char      byGps;                  // GPS          
    unsigned char      byPWRState;             // ��Դ״̬ 0-�� 1-AC���� 2-���    
    unsigned char      byBatteryState;         // ��ص��� 0-�� 1-�� 2-���� 3-�� 4-�쳣 
} TPuSysService;

// ��Ƶ����ͨ��״̬    
typedef struct
{        
    unsigned char        bySendEnable;         // �Ƿ���   
    unsigned char        byEnFormat;           // �����ʽ   
    unsigned char        byEnRes;              // �ֱ���     
    unsigned short       wEnRtBitRate;         // ʵʱ����   
    unsigned short       wEnAvrBitRate;        // ƽ������   
    unsigned long       dwPackLose;           // ��֡��     
    unsigned long       dwPackError;          // ��֡��         
} TPuVidEncState;

// ��Ƶ����ͨ��״̬
typedef struct
{ 
    unsigned char        byRecvEnable;         // �Ƿ����   
    unsigned char        byDecFormat;          // �����ʽ   
    unsigned char        byDecRes;             // �ֱ���     
    unsigned short       wDecRtBitRate;        // ʵʱ����   
    unsigned short       wDecAvrBitRate;       // ƽ������   
    unsigned long       dwRcvFrmNum;          // �յ�֡��   
    unsigned char        byLoseFrmRatio;       // ��֡��     
} TPuVidDecState;

// ��Ƶ����״̬
typedef struct
{        
    unsigned char        bySendEnable;        // �Ƿ��� 
    unsigned char        byEnFormat;          // �����ʽ 
    unsigned char        byAecEnable;         // �Ƿ�������� 
    unsigned char        byDumbEnable;        // �Ƿ����� 
    unsigned char        byEnVolume;          // ���� 
} TPuAudEncState;

// ��Ƶ����״̬
typedef struct
{
    unsigned char        byRecvEnable;        // �Ƿ���� 
    unsigned char        byDecFormat;         // �����ʽ 
    unsigned char        byMuteEnable;        // �Ƿ��� 
    unsigned char        byDecVolume;         // ���� 
} TPuAudDecState;

// ¼��״̬
typedef struct 
{
    unsigned char  byIsManualRec;          // �Ƿ����ֶ�¼��
    unsigned char  byIsMotiveAlarmRec;     // �Ƿ����ƶ����澯¼��
    unsigned char  byIsPinAlarmRec;        // �Ƿ��ڲ��ڸ澯¼��
    unsigned char  byIsTimelyRec;          // �Ƿ��ڶ�ʱ¼��
    unsigned char  byIsWeekRec;            // �Ƿ�������¼��
    unsigned char  byIsPreRec;             // �Ƿ���Ԥ¼
} TPuRecState;

// ����״̬
typedef struct 
{
    unsigned char  byIsPlaying;             // �Ƿ��ڷ���
} TPuPlayState;

// ͨ����������
typedef struct 
{
    unsigned char  byChanType;               // ͨ������ EPuChanType
    unsigned char  byChanId;                 // ͨ�����
} TPuChanId;

// ͨ��״̬��ѯ��������
typedef struct 
{
    unsigned char  byChanNum;   // ����ѯͨ������
    TPuChanId      atChanId[64]; // ����ѯͨ���������ݼ���
} TPuChanStateReq;

// ͨ��״̬����
typedef struct 
{
    TPuChanId tChanId;           // ͨ����������
    
    union
    {
        TPuAudEncState tAudEncState; // ��Ƶ����״̬
        TPuAudDecState tAudDecState; // ��Ƶ����״̬
        TPuVidEncState tVidEncState; // ��Ƶ����״̬
        TPuVidDecState tVidDecState; // ��Ƶ����״̬
        TPuRecState    tRecState;    // ¼��״̬
        TPuPlayState   tPlayState;   // ����״̬
    }u;
} TPuChanState;

// ͨ��״̬��ѯӦ������
typedef struct 
{
    unsigned char    byChanNum;          // Ӧ��ͨ������
    TPuChanState     atChanState[64];    // Ӧ��ͨ��״̬���ݼ���
} TPuChanStateAck;

// ǰ���豸ϵͳ��Ϣ
typedef struct 
{
    char   achDevType[PUSDK_MAX_STRING_LEN64 + 1];     // �豸����
    char   achDevName[PUSDK_MAX_STRING_LEN64 + 1];     // �豸����
    char   achDevIP  [PUSDK_MAX_STRING_LEN32 + 1];     // IP��ַ
    char   achDevMac [PUSDK_MAX_STRING_LEN32 + 1];     // MAC��ַ
    char   achSoftVer[PUSDK_MAX_STRING_LEN64 + 1];     // ����汾
    char   achHardVer[PUSDK_MAX_STRING_LEN32 + 1];     // Ӳ���汾
    char   achConnStatus[PUSDK_MAX_STRING_LEN32 + 1];  // ����״̬
} TPuSysInfo;

// ���̷�����Ϣ
typedef struct 
{
    unsigned char  m_byDiskId;                              // ����ID
    unsigned char  m_byPartId;                              // ����ID
    unsigned char  m_byState;                               // ����״̬��EPuDiskState
    unsigned long  m_dwHealth;                              // ���̽���״̬��EPuDiskHealth
    unsigned long  m_dwMBSize;                              // ������С
    unsigned long  m_dwFreeMBSize;                          // ʣ��ռ�
    char           m_achName[PUSDK_MAX_FILEPATH_LEN + 1];   // ����mount�������
    char           m_achPhyName[PUSDK_MAX_FILEPATH_LEN + 1];// ��������������
} TPuDiskPartion;

// ϵͳ������Ϣ
typedef struct 
{
    unsigned char     m_byDiskPartionNum;                          // ϵͳ��������(ֻ��)
    TPuDiskPartion    m_atDiskPartionInfo[PUSDK_MAX_DISKPART_NUM]; // ϵͳ�����б�(ֻ��)
} TPuDiskPartionParam;



//! ��ʽ��״̬
typedef struct 
{
    unsigned char byCurState;        // ��ʽ��״̬(EPuFormatState) 
    unsigned char byErrorNum;        // ʧ�ܴ��� 
}TPuFormatState;

//! ������Ϣ
typedef struct
{
    char achDiskName[PUSDK_MAX_STRING_LEN32 +1];		// �������� 
    unsigned char byDiskType;   		                // ��������(EPuDiskType)
    unsigned char byDiskSubareaNum;					    // ���̰����ķ������� 
	unsigned long dwDiskSizeMB;                         // ����������С(MB) 
    //ע: ÿ�������м�������,����abySubareaType��ǰ����Ԫ�������÷�������,������С��ͬ
    char abyFileSystemType[PUSDK_MAX_SUBEREA_NUM];   	// �ļ�ϵͳ����(EPuFileSystemType)
    char abySubareaUseType[PUSDK_MAX_SUBEREA_NUM];		// ��������(EPuSubareaUseType) 
    char abySubareaSize[PUSDK_MAX_SUBEREA_NUM];		    //������С(�ٷֱ�,��10%��Ϊ10,ע��:�Ǳ�����̵İٷֱ�) 
} TPuDiskDetailInfo;

// ���̸�ʽ����Ϣ(GET)
typedef struct 
{
    unsigned char       byErrorCode;                    // 0 ���� �ɹ�����0 ���� ʧ��
    unsigned char       byDiskNum;                      // ���̸��� 
    TPuFormatState      tFormatState;                   // ���̸�ʽ��״̬ 
    TPuDiskDetailInfo	atDiskInfo[PUSDK_MAX_DISK_NUM]; // ������Ϣ 
} TPuFormatGetDiskInfo;

// ���̸�ʽ����Ϣ(SET)
typedef struct 
{
    unsigned char     byDiskNum;                       // ���̸��� 
    TPuDiskDetailInfo atDiskInfo[PUSDK_MAX_DISK_NUM];  // ������Ϣ 
} TPuFormatSetDiskInfo;

typedef struct
{
    unsigned char		byErrCode;		// ������,ע:��������Ϊnoerrʱ��ʹ�ø�ʽ������ 
    unsigned char		byMkfsPercent;	// ��ǰ���̸�ʽ������(�ٷֱ�,��10%��Ϊ10) 
} TPuDiskFormatProgressInfo;

// �������״̬
typedef struct 
{
    unsigned char      byUsedNum;                  // ʹ����������ǰOutputPin����
    unsigned char      abyState[64];               // ״̬ EEventType
} TPuOutputPinState;

// ��������״̬
typedef struct 
{
    unsigned char      byUsedNum;     // ʹ����������ǰInputPin����
    unsigned char      abyState[64];  // ״̬ EEventType
} TPuInputPinState;

//////////////////////////////////////////////////////////////////////////
// ¼����Ϣ�����ṹ

//ϵͳʱ������ṹ
typedef struct
{
    unsigned short   wYear;    
    unsigned char    byMonth;   //�� 1-12
    unsigned char    byDay;     //�� 1-31    
    unsigned char    byHour;    //ʱ 0-23    
    unsigned char    byMinute;  //�� 0-59
    unsigned char    bySecond;  //�� 0-59
} TPuSysTime;

// ��ѯ¼���ļ���Ŀ����
typedef struct
{
    unsigned char  byChannelId;
    unsigned char  byType;          // EPuRecQueryType
    TPuSysTime     tStartTime;
    TPuSysTime     tEndTime;
} TPuRecQueryNumReq;

// ��ѯ¼���ļ���ĿӦ��
typedef struct
{
    unsigned short         wNum;
} TPuRecQueryNumAck;

// ����ǰ��¼��
typedef struct
{
    unsigned long  dwIndexId;     // ¼���ļ�������
    unsigned char  byType;        // EPuRecQueryType ¼������
    TPuSysTime     tStartTime;    // ��ʼʱ��
    TPuSysTime     tEndTime;      // ����ʱ��
    unsigned short wIdentifier;   // ¼���ʶ����ʹ�ã�    
} TPuPlayStartReq;

typedef struct 
{
    unsigned char     byPuPlayerId;                             // ������id
    unsigned long     dwCreateTime;                           // ¼���ļ�����ʱ��(s)
    unsigned long     dwTotalFileTime;                        // �ļ���ʱ��
    unsigned long     dwCurrentPlayTime;                      // ��ǰ����ʱ��
    unsigned char     byIsFinished;                           // �Ƿ񲥷Ž���
    unsigned short    wTransactionNum;                        // ������ˮ�ţ��Ͳ��������е�һ�£�
} TPuPlayProgress;

// ��ѯ¼���ļ���Ϣ����
typedef struct
{
    unsigned char     byChannelId;     // ͨ����
    unsigned char     byType;          // ǰ��¼������ EPuRecQueryType
    TPuSysTime        tStartTime;      // ��ʼʱ��
    TPuSysTime        tEndTime;        // ����ʱ��
    unsigned long     dwIndex;         // ¼���ļ�������
    unsigned short    wNum;            // ��ǰ��ѯ��Ŀ (1-20,��0��ʱ�򲻷�������)
    unsigned char     byQueryMode;     // ��ѯģʽ EPuRecQueryFileMode
} TPuRecQueryFileReq;

// ����¼���ļ���Ϣ
typedef struct 
{
    unsigned long   dwRecIndexId;      // �ļ��¼�id��
    unsigned char   byRecFileType;     // ¼���ļ�/�¼����� EPuRecQueryType
    TPuSysTime      tStartTime;        // ��ʼʱ��
    TPuSysTime      tEndTime;          // ����ʱ��
    unsigned char   abyFileName[65];   // �ļ�����
    unsigned char   abyFilePath[65];   // ����ļ�·������
    
} TPuRecFileInfo;

// ��ѯ¼���ļ���ϢӦ��
typedef struct
{
    unsigned short     wNum;               // ¼���ļ���Ŀ
    unsigned char      byType;             // ¼���ļ�/�¼����� EPuRecQueryType
    unsigned char      byIsFinished;       // �Ƿ������е�¼���ļ�,1Ϊ������0Ϊδ����
    TPuRecFileInfo     atFileInfo[20];     // ¼���ļ���Ϣ
    unsigned short     wTransactionNum;    // �ļ�������ˮ��
} TPuRecQueryFileAck;

//�����������
typedef struct 
{
    EPuRecPlayType      byOperationType;              // EPuRecPlayType
} TPuRecPlayOperation;

// ɾ��ǰ��¼���ļ�
typedef struct 
{
    unsigned char     byType;                       // �ļ����� EPuRecQueryType
    unsigned long     dwRecFileIndexId;             // �ļ�ID
} TPuRecFileDelReq;

// ץ��ͼƬ�ļ���ѯ����
typedef struct 
{
    unsigned char   byNum;            // �ļ�����
    unsigned char   byType;           // ��ѯ���� EPuPicFileQueryType
    unsigned char   byChannelId;      // ͨ����
    unsigned char   byEvent;          // �¼����� EPuRecQueryType
    unsigned short  wBeginNum;        // �ļ���ѯ��ʼ���
    TPuSysTime      tTimeStart;       // ��ʼץ��ʱ��
    TPuSysTime      tTimeEnd;         // ����ץ��ʱ��
} TPuPicFileQueryReq;

// ץ���ļ���Ϣ
typedef struct 
{
    unsigned char    byType;                // �¼�����
    unsigned char    byChannelId;           // ͨ����
    unsigned char    byVideoResolution;     // ͼ��ֱ��� EPuVideoRes
    unsigned char    byFormat;              // ͼƬ��ʽ
    TPuSysTime       tTime;                 // ��ͼʱ��
    unsigned long    dwPicID;               // ͼƬ���
    char             achFileName[PUSDK_MAX_STRING_LEN64+1];    // �ļ�����
} TPuPicFileInfo;

// ץ��ͼƬ�ļ���ѯӦ��
typedef struct 
{
    unsigned short   wMaxNum;            // ��ǰ��ѯ�ļ�������
    int              nBeginNum;          // �ļ���ѯ��ʼ���, < 0 Invalid, V5 
    unsigned char    byIndex;            // ��ǰ��������,��1��ʼ
    unsigned char    byNum;              // �ļ�����
    unsigned char    byEnd;              // �Ƿ����
    TPuPicFileInfo   aPicFileInfo[20];   // ͼƬ�ļ��б�
    unsigned short   wTransactionNum;    // ������ˮ��
} TPuPicFileQueryAck;

// ץ���ļ�����
typedef struct 
{
    //unsigned short wIndex;                // Ĭ��1
    TPuPicFileInfo tPicFileInfo;
} TPuPicFileDownloadReq;

// ͼƬ���ؽ��
typedef struct
{
    unsigned long dwPicID;    // ͼƬ���
    int           res;        // �������
    void*         pPicDate;   // ͼƬ����
    int           nLenDate;   // ͼƬ���ݳ���
} TPuPicFileDownloadRes;

// ɾ��ץ���ļ�
typedef struct 
{
    TPuPicFileInfo tPicFileInfo;
} TPuPicFileDelExReq;

// ץ���ļ�ɾ�����
typedef struct 
{
    unsigned short    wError;         // ���
    unsigned long     dwPicID;        // ͼƬID
} TPuPicFileDelAck;

//////////////////////////////////////////////////////////////////////////
// ������ṹ����

//////////////////////////////////////////////////////////////////////////
// from vsip

// �������޸�IP
typedef struct 
{
    unsigned long  dwTargetIP; // Ŀ��IP��ַ
    unsigned long  dwNewIP;    // ��IP��ַ
    unsigned long  dwNewMask;  // ����������
    unsigned long  dwNewGW;    // �����ص�ַ
    TPuGuid        tGUID;      // GUID
} TPuSetIpInfo;

//ϵͳ�����ṹ
typedef struct
{
    unsigned char    abyName[PUSDK_MAX_STRING_LEN64+1];        // �豸��
    unsigned char    abyTypeName[PUSDK_MAX_STRING_LEN64+1];    // �豸������(ֻ��)
    unsigned char    byDevType;                                // �豸����(EPuDeviceType) 
    unsigned char    abySeriesName[PUSDK_MAX_MTALIAS_LEN+1];   // �豸ϵ����
} TPuSysParam;

//ϵͳ���������ṹ
typedef struct
{
    unsigned char      byIndustryType;                             // ��ҵ����(Ĭ��:PUSDK_INDUSTRY_TELECOM)
    unsigned char      abyVersion[PUSDK_MAX_VERSION_STRING_LEN+1]; // �豸�汾��(ֻ��)
    
    unsigned char      abyMacAddr[6];                          // MAC
    unsigned long      dwUnitIpAddr;                           // �豸IP��ַ
    
    unsigned long      dwSubnetMask;                           // �豸��������(������)
    unsigned long      dwGateWay;                              // ���ص�ַ(������)
    
    unsigned char      byAddrMode;                             // ��ַʹ�÷�ʽ���̶�IP����DHCP   
    unsigned char      byDhcpEthId;                            // ��ַDHCP�����ں�
    unsigned char      byUseDhcpGateway;                       // �Ƿ�ʹ��DHCP������
    
    unsigned short     wVsipPort;                              // VSIP�˿ں�,�豸�����˿�
    unsigned long      dwRegPlatformIpAddr;                    // ע��ƽ̨��ַ
    unsigned short     wRegPlatformPort;                       // ע��ƽ̨�˿ں�
        
    unsigned short     wVsipMultiPort;                         // �����˿�        
    unsigned long      dwVsipMultiIpAddr;                      // �������鲥��ַ        
    
    unsigned char      abyWebUsername[PUSDK_MAX_MTALIAS_LEN+1];    // Ƕ��ʽWeb�û���
    unsigned char      abyWebPassword[PUSDK_MAX_MTALIAS_LEN+1];    // Ƕ��ʽWeb����
    unsigned char      abyGeustUsername[PUSDK_MAX_MTALIAS_LEN+1];  // Ƕ��ʽWeb�û���
    unsigned char      abyGeustPassword[PUSDK_MAX_MTALIAS_LEN+1];  // Ƕ��ʽWeb����
    
    unsigned char      byDetPacketSend;                        // �Ƿ���NAT̽���
    unsigned char      byDetPacketInterval;                    // NAT̽������(Ĭ��:PUSDK_MAX_NATSEND_INT)
    
    unsigned char      abyPuid[PUSDK_MAX_PUID_LEN+1];                 // puid
    unsigned char      abyPassword[PUSDK_MAX_STRING_LEN64+1];   // ��֤����
    
    unsigned short     wAlarmContinuance;                      // �澯�ָ�ʱ��(��)
    unsigned char      bySubAreaType;                          // ������ʽ(ֻ��)
    unsigned char      byMaxTranferNum;                        // ���ֱ��·��
    unsigned char      byMatrixValid;                          // �Ƿ�������
    char               chSysTimeZone;                          // ϵͳʱ����Ϣ(0:UTC,+1~+12:��ʱ��,-1~-12:��ʱ��,Ĭ��:PUSDK_DEFCFG_TIMEZONE)
    char               byIsDecCombined;                        // �Ƿ���ϳ����(��������) 
    
    unsigned char      byDoubleFlowEnable;                     // �Ƿ���˫����0-��������1-����
    unsigned char      byVideoEncCurNum;                       // ��Ƶ���뵱ǰʹ��ͨ����
    unsigned char      byEncPref;                              // �������ȷ�ʽ���ٶ�����/ͼ���������ȣ�
	char               abySupportCharsetEncode[PUSDK_MAX_STRING_LEN64 + 1];  //֧�ֵ��ַ���
    char               abyActiveCharsetEncode[PUSDK_MAX_STRING_LEN64 + 1];    //��ǰ���ַ���
} TPuSysPubParam;

//PPPOE�����ṹ
typedef struct
{
    unsigned char  m_byPPPOEState;                 // PPPOE״̬ 0-��ʹ��, 1-ʹ��
    char           m_achUsername[PUSDK_MAX_USER_NUM];    // �����û���
    char           m_achPassword[PUSDK_MAX_USER_NUM];    // ��������
    unsigned char  m_byAutoDialState;              // �Զ�����״̬ 0-��ʹ��, 1-ʹ��
    unsigned long  m_dwAutoDialWait;               // �Զ����ŵȴ�ʱ��(Ĭ��60��)
    unsigned long  m_dwAutoDialTry;                // �Զ��������Դ���(Ĭ��6��)
    unsigned char  m_byDebugState;                 // Debug״̬ 0-��ʹ��, 1-ʹ��
    unsigned short m_wLcpEchoSendInterval;         // ���ӽ����󣬷���LCP-ECHO����ʱ����(Ĭ��30��)
    unsigned short m_wLcpEchoRetryTimes;           // ���ͼ���LCP-ECHO��û�еõ���Ӧ����Ϊ���ӶϿ�(Ĭ��6��)
    unsigned char  m_byDefaultRouteEnable;         // ·��ʹ��, 0-��ʹ��, 1-ʹ��
} TPuPppoeParam;

// ����������
typedef struct
{
    unsigned char        m_byVideoEncNum;              // ��Ƶ����ͨ����������ӳƽ̨Ӧ����Ҫ��ͨ������
    unsigned char        m_byVideoDecNum;              // ��Ƶ����ͨ������
    unsigned char        m_byAudioEncNum;              // ��Ƶ����ͨ������
    unsigned char        m_byAudioDecNum;              // ��Ƶ����ͨ������
    
    unsigned char        m_bySerialPortNum;            // ��������
    unsigned char        m_byInputPinNum;              // ������������
    unsigned char        m_byOutputPinNum;             // �����������
    unsigned char        m_byVideoInputPortNum;        // ��Ƶ����˿�����
    
} TPuBaseCap;

//������������չ������
typedef struct
{
    int     m_nVGANum;                      // ��Ƶ���VGA�ӿ�����
    int     m_nVidOutNum;                   // ��Ƶ����ӿ�����
    int     m_nPTZCtrlNum;                  // �ɿ�����̨����
    int     m_nEtherNetNum;                 // ��̫��������
    int     m_nWirelessNum;                 // ������������
    int     m_nXiAnTeleNum;                 // �������Ÿ澯����
    int     m_nPlatFormType;                // ����ƽ̨����
    int     m_nDevPowerType;                // ��������
    int     m_nUseSd;                       // �Ƿ�ʹ��SD��
    unsigned char m_bySlotInfoNum;          // ��λ��Ϣ����(������)
    
    int     m_nVidAdpterNum;                // ��Ƶתѹͨ������
    int     m_nVidCompNum;                  // ͼ��ϳ�ͨ����
    int     m_nVidSrcPollNum;               // ��ƵԴ��ѯͨ������
    int     m_nAudMixNum;                   // ��Ƶ����ͨ������
    int     m_nOsdColorNum;                 // ̨����Ļ֧�ֵ���ɫ��
    int     m_nShadeAreaNum;                // ͼ���ڱ���������
    int     m_nMaxStreamSendNum;            // ����ͨ�����������(һ��ͨ��������Ƶ����Ƶ)
    int     m_nMaxStreamDirSendNum;         // ����ͨ�������ֱ��·��(һ��ͨ��������Ƶ����Ƶ)
    int     m_nStreamEncrypt;               // �Ƿ���������
    int     m_nMotionDetectAreaNum;         // �ƶ������������
    int     m_nSnapShotChanNum;             // ץ��ͨ����
    int     m_nCBRVBR;                      // �Ƿ�֧�����ʿ���,1��֧�֣�0����֧��
    int     m_nMaxEncBitRate;               // �����Ч����             
    int     m_nSupportMultiChan;            // �Ƿ�֧��˫����1��֧�֣�0����֧��
    int     m_nMatrix;                      // �Ƿ�֧�־������
    
    int     m_nRecorderNum;                 // ¼��ͨ������
    int     m_nPlayerNum;                   // ����ͨ������
    int     m_nFileDownloadNum;             // ¼���ļ�����ͨ������

    int     m_nShadeAreaSize;               // ���ͼ���ڱ���� 
    int     m_nSupportCompStyle;            // ����ϳ����� 1,2,4,8λ�ֱ����1,4,9,16���������
    int     m_nImageDisplaySetting;         // ֧�ֵı���ͼ����ʾ������������(ÿ��bit��ʾһ��������) EPuImageSetting
    unsigned char      m_bySupportImageDisPlay;        // ֧�ֱ���ͼ�����
    unsigned char      m_byRecTypeSupport;             // ֧�ֵ�¼������
    unsigned char      m_bySupportSnapShot;            // ֧��ץ������
    unsigned char      m_bySupportP2P;                 // ֧��P2P��1-֧��, 0-��֧��
    unsigned char      m_bySelfCamera;                 // �Դ�����ͷ��1-֧��, 0-��֧��
    unsigned char      m_byOtherAlarmNum;              // ��չ�澯������
    unsigned char      m_byPuVer;                      // ǰ�˰汾��ָ0(v3.9);1(v4.0);2(v5.0)
    unsigned char      m_byVideoEncSlaveChannelNum;    // ��Ƶ���븨��ͨ����

    unsigned char      m_bySupportAEC;                 // ֧�ֻ�������
    unsigned char      m_bySupportOsdScroll;           // ֧�ֺ��
    unsigned char      m_bySupportOsdExtension;        // ֧����չ

    unsigned char      m_bySupportGps;                 // ֧��GPS
    unsigned char      m_bySupportWifi;                // ֧��WiFi
    unsigned char      m_bySupport3g;                  // ֧��3G
    
    unsigned char      m_byEncQualityOptimize;         // ֧�ֱ��������Ż�
    unsigned char      m_bySupportBncOut;              // ֧�ֱ���BNC���
    
    int                m_adwVideoAudeoEnc[16];         // ������Ƶ����Ƶ�Ķ�Ӧ��ϵ
    unsigned char      m_byVideoEncPhysicalNum;        // ��Ƶ��������ͨ������
    unsigned char      m_bySupportEmergentButton;      // ֧�ֽ����澯��ť
} TPuMiscCap;

// �豸����������
typedef struct 
{
    TPuBaseCap		  m_tBaseCap;
    TPuMiscCap        m_tExtCap;
} TPuCapInfo;

//Video Type
typedef struct
{
    unsigned char bySN4;
    unsigned char byMPEG4;
    unsigned char byH261;
    unsigned char byH263;
    unsigned char byH264;
} TPuVideoType;

//Video Resolution
typedef struct
{
    unsigned char byAuto;
    unsigned char byQCIF;
    unsigned char byCIF;
    unsigned char by2CIF;
    unsigned char by4CIF;
    unsigned char byVGA;
    unsigned char byQQCIF;
    unsigned char byQVGA;
    unsigned char byXVGA;
    unsigned char by720P;
    unsigned char by1080P;
} TPuVideoResolution;

//��Ƶͼ��ֱ���
typedef struct
{
    TPuVideoResolution tSN4;
    TPuVideoResolution tMEPG4;
    TPuVideoResolution tH261;
    TPuVideoResolution tH263;
    TPuVideoResolution tH264;
} TPuSpVideoResolution;

//��Ƶ����ͨ�������ṹ
typedef struct
{  
    TPuVideoType         tSupportedVideoType;       // ֧�ֵ���Ƶ��ʽ(ֻ��)
    TPuSpVideoResolution tSupportedVideoResolution; // ֧�ֵ���Ƶ�ֱ���(ֻ��)
    unsigned char        byVideoType;             // ����ѹ����ʽ���μ� EPuVideoType
    unsigned char        byVideoResolution;       // ͼ��ֱ��ʣ��μ� EPuVideoResolution
    unsigned char        byFrameRate;             // ֡�� 1-25
    
    unsigned long        dwBitRate;               // ��������� 64K-4M
    unsigned char        byMaxQuantization;       // ����������� 1-31
    unsigned char        byMinQuantization;       // ��С�������� 1-31
    unsigned short       wInterval;               // �ؼ�֡��� 25-2500
    char                 byBrightness;            // ���� -128��127
    char                 byContrast;              // �Աȶ� -128��127
    char                 bySaturation;            // ���Ͷ� -128��127
    char                 byHue;                   // ɫ�� -128��127
    char                 chSharpness;             // ��� -128��127
    unsigned char        byBitRateType;           // 0:������, 1:������
    
    unsigned char        bySendRetrans;           // �Ƿ����ش�(Ĭ��:1-��)
    unsigned short       wSendBufTimeSpan;        // �ش�ʱ����(Ĭ��:200(1~1500����)
    int                  bOpenMultiChan;          // ˫���Ƿ���(��ʹ��)
    unsigned char        byImageQuality;          // ͼ������(Ĭ��:1-��������, 2-�ٶ�����)
    unsigned char        bSlaveChannel;           // �Ƿ��Ǹ���ͨ��(Ĭ��:0-����)
    
    unsigned char        byH264FrameMode;         // 0: FieldMode, 1:FrameMode
} TPuVideoEncParam;

//��Ƶ���빫�������ṹ
typedef struct 
{
    unsigned char        byMaxVideoEncNum;      //��������Ƶ����ͨ������(ֻ��)
    unsigned char        byVideoEncUsed;        //���õ���Ƶ����ͨ����
} TPuVideoEncPubParam;

// ������������
typedef struct
{
    unsigned char byUdpUnicast;                // �鲥��ʽ
    unsigned char byUdpMulticast;              // �ಥ��ʽ
    unsigned char byUdpBroadcast;              // �㲥��ʽ
    unsigned char byTcpServer;                 // TCP�����
    unsigned char byTcpClient;                 // TCP�ͻ���
    unsigned char bySSL;                       // SSL��ʽ
} TPuConnType;

//������������ṹ
typedef struct
{
    unsigned char        byInitStreamState;      // ������ʼ��״̬ 0-������, 1-����
    unsigned char        byCurStreamState;       // ������ǰ״̬ 0-δ����, 1-����
    TPuConnType          tSupportedConnType;     // ֧�ֵ�������������
    unsigned char        byConnectionType;       // ������������
    unsigned long        dwStreamIpAddr;         // ���������ַ
    unsigned short       wStreamPort;            // ��������˿�
} TPuCommonNetParam;

//��Ƶ������Ļ�����ṹ
typedef struct 
{
    unsigned char        byShowTimeState;      // �Ƿ���ʾʱ�� 0-����ʾ, 1-��ʾ
    unsigned char        byOSDState;           // ������Ļ״̬ 0-����Ļ, 1-����Ļ
} TPuVideoEncOsdParam;

//��Ƶ����໭������ṹ
typedef struct 
{
    unsigned char abyEncodeMember[PUSDK_MAX_PIC_REGION_NUM];// �໭���Ӧ����ƵԴ�˿ں�
    unsigned char byEncodeMemberNum;                        // �໭���Ӧ����ƵԴ�˿�����
    unsigned char byEncodeStyle;                            // �໭�����������(0-���ϳɣ�1-�Ļ���ϳ�)
} TPuVideoEncMpParam;

//��Ƶ������ƵԴ�˿ڲ����ṹ
typedef struct 
{    
    unsigned char   abyValidVideoInputPort[PUSDK_MAX_VIDEOIN_PORT_NUM];// ���õ���Ƶ����˿ں�(ֻ��)
    unsigned char   byValidVideoInputPortNum;                          // ���õ���Ƶ����˿�����(ֻ��)
    unsigned char   byVideoCurrentInputPort;                           // ��ǰʹ�õ���Ƶ����˿ں�
} TPuVideoEncInputPortParam;

// ��Ƶ����ͨ����ϵ����
typedef struct
{
    unsigned char  byChannelId;        // ����ͨ��Id
    unsigned char  byMainChannelId;    // ��Ӧ������ͨ��Id
} TPuVideoEncChannelRelation_Item;

// ��Ƶ����ͨ����ϵ������ṹ
typedef struct 
{
    unsigned char                    byChannelNum;   // ͨ����
    TPuVideoEncChannelRelation_Item  atChannel[32];  // ͨ����Ϣ    
} TPuVideoEncChannelRelation;

//��Ƶ������ѯ�����ṹ
typedef struct 
{
    unsigned char        byPollState;                          // ��ѯ״̬ 0-����ѯ, 1-��Ч
    unsigned char        byPollPortNum;                        // ��ѯ�˿�����
    unsigned char        abyPollPort[PUSDK_MAX_POLL_PORT_NUM]; // ��ѯ�˿ڼ�
    unsigned short       wPollInternal;                        // ��ѯʱ���� ��λ��
} TPuVideoEncPollParam;

typedef struct 
{
    unsigned char  byChanType;                 // �������Ǹ�����, 0:����,1:����
    unsigned char  bySupportSubNum;            // ֧�ָ�����·������������Ч;
    unsigned char  SubChanId[8];               // ������ID�б����ݸ�����·����������д,��������Ч;
    unsigned char  byStatus;                   // ��ǰ�������Ƿ�����1��������0-δ���������������У�����ǿ���˫��������1�����ֹͣ˫������0��
    unsigned char  byParentEncId;              // �����ı���ͨ����,����������Ч
    unsigned long  m_dwReserved;               // for align
} TPuMultiEncChanParam;

//��Ƶ��������ṹ
typedef struct 
{
    TPuVideoType   tSupportedVideoType;      // ֧�ֵ���Ƶ��ʽ(ֻ��)
    unsigned char  byPlayerBufferNum;        // ���Ż������� 6-50
    unsigned char  byPlayerOriginBufNum;     // ������ʼ�������� 0-10
    unsigned char  byVideoType;              // ��Ƶ�����ʽ���μ�EPuVideoType
    unsigned char  byDecDealLevel;           // ͼ�������(0��3��,0��ʾû��)
    unsigned char  byImageQuality;           // ͼ������(Ĭ��:1-��������, 2-�ٶ�����)
    unsigned char  byPerpenSync;             // ��ֱͬ��(0-�رգ�1-����)
} TPuVideoDecParam;

//��Ƶ���빫�������ṹ
typedef struct 
{    
    unsigned char  byVGAOutSupported;        //�Ƿ�֧��VGA���(ֻ��) 0-��֧��, 1-֧��
    unsigned char  byVideoOutputType;        //��Ƶ����˿����� (�μ� EPuVideoOutType )
    unsigned char  byVGAFrequency;           //VGA���ˢ��Ƶ�� (60/75/85)
    unsigned char  byStarveMode;             //����Ƶ����ʾ��ʽ���μ� EPuPlayerStarveMode
} TPuVideoDecPubParam;

//��Ƶ�����ش������ṹ
typedef struct 
{
    unsigned short wRetransfer1stTimeSpan;  // �����1���ش�����(Ĭ��40)
    unsigned short wRetransfer2ndTimeSpan;  // �����2���ش�����(Ĭ��80)
    unsigned short wRetransfer3rdTimeSpan;  // �����3���ش�����(Ĭ��160)
    unsigned short wRejectTimeSpan;         // �������ʱ��(Ĭ��200)
    unsigned char  byRetransferState;       // �����ش�״̬ 0-���ش�, 1-�ش�
} TPuVideoDecRetransferParam;

//��Ƶѹ����ʽ
typedef struct
{
    unsigned char byPCMA;
    unsigned char byPCMU;
    unsigned char byGSM;
    unsigned char byG729;
    unsigned char byADPCM;
    unsigned char byG7221C;
} TPuAudioType;

//��Ƶ��������
typedef struct 
{
    unsigned char      byLineIn;
    unsigned char      byMic;
} TPuAudioInMode;

//��Ƶ��������ṹ
typedef struct 
{
    TPuAudioType   tSupportedAudioType; // ֧�ֵ�����ѹ����ʽ(ֻ��)
    TPuAudioInMode tSupportedAudioIn;   // ֧�ֵ���Ƶ���뷽ʽ
    
    unsigned char  byAudioType;         // ��Ƶ���ݸ�ʽ EPuAudioType
    unsigned char  byAudioInputType;    // ��Ƶ�������ͣ�1-Line In��2-mic
    unsigned char  byVolume;            // ���� 0-25
    unsigned char  byAECState;          // ��������״̬ 0-��Ч, 1-��Ч
    unsigned char  byDumbState;         // ����״̬ 0-��Ч, 1-��Ч
    unsigned char  byIsMix;             // �Ƿ����
} TPuAudioEncParam;

//��Ƶ��������ṹ
typedef struct 
{
    TPuAudioType   tSupportedAudioType; // ֧�ֵ�����ѹ����ʽ(ֻ��)
    unsigned char  byVolume;            // ���� 0-25
    unsigned char  byMuteState;         // ����״̬ 0-��Ч, 1-��Ч
} TPuAudioDecParam; 

//��������
typedef struct
{
    unsigned char byRS232;
    unsigned char byRS422;
    unsigned char byRS485;
} TPuSerialType;

//���ڲ����ṹ
typedef struct 
{
    unsigned long   dwBaudRate;       // ���ڲ�����
    unsigned char   byComPortType;    // �������� 0:��ʹ��, 1:RS232, 2:RS422, 3:RS485
    TPuSerialType   tSupportedType;   // ֧�ֵĴ������� (ֻ��)
    unsigned char   byStop;           // ֹͣλ��Ĭ��1
    unsigned char   byDig;            // ����λ��Ĭ��8
    unsigned char   byParityCheck;    // ��żУ�� 0-��У�飬1-żУ�飬2-��У��
    unsigned char   byStart;          // ��ʼλ��Ĭ��1
    unsigned long   dwTimeOutMS;      // ���������ʱʱ��(ms)��Ĭ��200
    unsigned long   dwMaxReadByte;    // ����������ֽ�����Ĭ��8
    unsigned long   dwWriteDelayTime; // д������ʱʱ��(ms)
    unsigned char   byIsSlaveMode;    // �����Ƿ��Ǵ�ģʽ
} TPuSerialPortParam;

//���ڹ��������ṹ
typedef struct 
{
    unsigned char  byInitStreamState;         // ���������շ���ʼ��״̬ 0-��Ч, 1-��Ч
    unsigned short wStreamLocalPort;          // �������ݱ��ؽ��ն˿�
    unsigned long  dwStreamDestIpAddr;        // �������ݶԶ˵�ַ
    unsigned short wStreamDestPort;           // �������ݶԶ˶˿�
    TPuConnType    tSupportedConnType;        // ֧�ֵ�������������
    unsigned char  byConnectionType;          // ������������
    unsigned char  byProtocolType;            // Э������ 0-͸��ͨ��, 1-VSIPЭ��
} TPuSerialPortPubParam;

//��ƵԴ����˿ڲ����ṹ
typedef struct 
{
    unsigned char  byCameraType;                 // ����ͷ���ͣ��μ� EPuCameraType
    unsigned char  byCameraID;                   // ����ͷ��ַ��
    unsigned char  bySerialPortID;               // ��Ӧ�Ĵ���ID
    unsigned char  abyVideoPortName[PUSDK_MAX_STRING_LEN16+1];    // ��ƵԴ�˿���
    unsigned char  abyVideoPortOSD[PUSDK_MAX_STRING_LEN64+1];    // ��ƵԴ��Ļ����
    unsigned short wVideoPortOSDLeftMargin;      // ��ƵԴ��Ļ��߾�
    unsigned short wVideoPortOSDTopMargin;       // ��ƵԴ��Ļ�ϱ߾�
    unsigned long  dwVideoPortOSDColor;          // ��ƵԴ��Ļ��ɫ��RGBֵ
    
    unsigned char  byOsdFontMode;                // ����ģʽ EPuOsdFontMode
    unsigned char  abyOsdCustomFontName[32+1];   // �Զ�����������
    unsigned long  dwOsdCustomFontSize;          // �Զ��������С
    unsigned long  dwOsdCustomFontStyle;         // �Զ��������� (EPuCustomFontStyle)
    
    char           byLighteness;                 // ����-127~128 (Ĭ��:0)
    char           byContrast;                   // �Աȶ�-127~128 (Ĭ��:0)
    char           bySaturation;                 // ���Ͷ�-127~128 (Ĭ��:0)
    char           byHue;                        // ɫ��-127~128 (Ĭ��:0)
    
    // add
    unsigned char  byAutoResetEnable;            // �Ƿ��Զ���λ             
    char           szResetCmd[PUSDK_MAX_STRING_LEN64+1]; // �Զ���λ�ַ���
    unsigned short wAutoResetTime;                // �Զ���λʱ�䣬Ĭ��60��
    
    unsigned short byNoVideoAlarmEnable;         // �Ƿ��ϱ���ƵԴ��ʧ�澯

    unsigned short wTimeOsdLeftMargin;           // ʱ����߾�
    unsigned short wTimeOsdTopMargin;            // ʱ���ϱ߾�
    
    unsigned short wAlarmTimeOsdLeftMargin;      // �澯��ʱ��߾�
    unsigned short wAlarmTimeOsdTopMargin;       // �澯��ʱ�ϱ߾�
    
    unsigned short wAlarmOsdLeftMargin;          // �澯��Ļ��߾�
    unsigned short wAlarmOsdTopMargin;           // �澯��Ļ�ϱ߾�
} TPuVideoInputParam;

//������Ƶͼ���������
typedef struct 
{
    unsigned char byOutputChanId;                         // ͼ�����ͨ��ID
    unsigned char byImageOutputEnable;                    // ͼ�񱾵����ʹ��
    unsigned char byCompStyle;                            // ����ϳɱ����� (0-1����,1-4����)
    unsigned char byCompMemberNum;                        // ����ϳ���ƵԴ�˿�����
    unsigned char abyCompMember[PUSDK_MAX_PIC_REGION_NUM];// ����ϳ���ƵԴ�˿ں�
} TPuLocalImageOutputParam;

//��������ṹ
typedef struct {
    unsigned long       m_dwBackgroundColor;  // ������ɫ ��0:͸��,1:��͸��,2:��͸����
    unsigned long       m_dwFontColor;        // ������ɫ��Ĭ�ϣ�0xFFFFFF��ɫ��RGB(r,g,b)
    unsigned long       m_dwHaltTime;         // ͣ��ʱ��
    unsigned short      m_wLeftMargin;        // X����
    unsigned short      m_wTopMargin;         // Y����
    unsigned short      m_wOnelineHigh;       // ��Ļ���и߶�
    unsigned short      m_wScrollTimes;       // ��������(Ĭ��:1)
    unsigned char       m_byFontSize;         // �����С(Ĭ��:28)
    unsigned char       m_byArrangeMode;      // ����ģʽ(0-����� 1-�ж��� 2-�Ҷ���)
    unsigned char       m_byScrollMode;       // ����ģʽ(0-���� 1-���� 2-���� 3-���� 4-����)
    unsigned char       m_byScrollSpeed;      // �����ٶ�(0-�� 1-�� 2-��)
    unsigned char       m_byScrollType;       // ������ʽ(0ʼ�գ�1ָ������)
    unsigned char       m_byScrollLineNum;    // �������(Ĭ��:1)
    unsigned char       m_byTransparency;     // �������(0-͸����1-��͸����2-��͸��)
    unsigned char       m_byOsdScrollEnable;  // �������ʹ��(0-��Ч, 1-��Ч)
    char                m_achFontType[16+1];  // ��������(Ĭ�ϣ�����)
    char                m_achContent[256+1];  // ���� ������Ļ��ʱ���á��ָ��������ĻAAA��BBB
} TPuOsdScroll;

//ͼ���ڱο�������
typedef struct 
{
    unsigned char  byShadeEnable;         // �ڱ�ʹ�� 1-��Ч 0-��Ч
    unsigned long  dwXPos;                // �ڱ����򶥵������
    unsigned long  dwYPos;                // �ڱ����򶥵�������
    unsigned long  dwWidth;               // �ڱ������ȣ���Χ 0-720
    unsigned long  dwHeigth;              // �ڱ�����߶ȣ���Χ 0-576
} TPuVideoShadeArea;

//ͼ���ڱο��ƽṹ��
typedef struct 
{
    unsigned short     wAreaNum;                                    // ������Ŀ
    TPuVideoShadeArea  atShadeArea[PUSDK_MAX_VIDEO_SHADE_AREA_NUM]; // ͼ���ڱο�������
} TPuVideoShadeParam;

// �澯��������
typedef struct 
{
    // ��ƵԴ���
    unsigned char        byVidSrcId;            // ������ƵԴid(����)(��Ч��ƵԴid�б�)
    unsigned char        byVidSrcSwtEnable;     // ��ƵԴ�л�ʹ��(Ĭ��:��)
    unsigned char        byCamPosSwtEnable;     // ��ƵԴԤ��λ�л�(Ĭ��:��)(����̨������ʾ)
    unsigned long        dwCamPosId;            // ����ͷԤ��λid(Ĭ��:0)(����̨������ʾ)    
    unsigned char        bySnapShotEnable;      // ͼ��ץ��(Ĭ��0:��)
    unsigned char        byLocalRecEnable;      // ����¼��ʹ��(Ĭ��0:��)
    unsigned char        byAlarmTimeEnable;     // �澯��ʱʹ��(Ĭ��0:��)
    unsigned char        bySoundAlarm;          // �澯����ʹ��(Ĭ��0:��)
    unsigned char        byOsdShowEnable;       // ��Ļ��ʾ(Ĭ��0:��)
    unsigned char        byOsdUseType;          // ��Ļ����(Ĭ��:0)
    unsigned char        byColorId;             // ��ɫ
    char                 achOsdName[PUSDK_MAX_STRING_LEN64 + 1];     // ��Ļ����(Ĭ��:alarm+id)
    
    // �������
    unsigned char        byPinOutputEnable;     // �������ͨ��ʹ��(Ĭ��0:��)
    unsigned char        byOutputPinId;         // �����ĸ澯���ͨ��(��Ч�������ͨ��id�б�)
    unsigned short       wLinkFuncSupport;      // �澯����֧�ֲ���������PUSDK_LINKSET_VIDSRCSWT_SUPPORT �ȶ�ѡ(��λ��)
} TPuAlarmLinkInfo;

//���ܲ���
typedef struct 
{    
    unsigned char  byMemThreshold;             // ϵͳ�ڴ�ʹ�÷�ֵ
    unsigned char  byMaxVideoLoseRatio;        // ��Ƶ�����ϱ���ֵ
    unsigned long  dwServerIP;                 // ���ܷ�����IP��ַ
    unsigned short wServerPort;                // ���ܷ�����Port
    unsigned char  byCpuUseRatio;              // cpu������
    
    char           achGetCommunity[ 32 + 1 ];  // ��ѯ��ͬ����(Ĭ��:public)
    char           achSetCommunity[ 32 + 1 ];  // ���ù�ͬ����(Ĭ��:private)
    unsigned char  byTrapServerNum;            // trap����������(Ĭ��:1��)
    unsigned short wLocalPort;                 // SNMP���ض˿ںţ�Ĭ��161
} TPuNmsParam;

// ǰ��ץ�Ĳ���
typedef struct 
{
    unsigned char  byPinAlarmEnable;        // ���ڸ澯ץͼʹ�� 0�ر�/1����
    unsigned char  byMotionDetectEnable;    // �ƶ����ץͼʹ�� 0�ر�/1����
    unsigned long  dwInterval;              // ץ��ʱ����(s)
    unsigned long  dwMaxSnapNum;            // ���ץ������
} TPuSnapshotConfig;

// ��������
typedef struct 
{
    unsigned char abyNameOrder[4];                 // �ֶ�˳�򣬴�0��ʼ�����α�ʾ��i+1���ֶ�
    unsigned char byNameNum;                       // ʵ��ʹ�õ��ֶθ���
    char          achDateSeparator[8];             // ����֮��ķָ���(������)(��Ч�ɴ�ӡ�ַ�)
    char          achTimeSeparator[8];             // ʱ��֮��ķָ���(ʱ����)(��Ч�ɴ�ӡ�ַ�)
    char          achNameSeparator[8];             // �����ֶ�֮��ķָ���(��Ч�ɴ�ӡ�ַ�)
    char          achUserDefine[64+1];             // �û��Զ����ֶ�����
} TPuNamingRule;

// FTP����
typedef struct 
{
    unsigned long   m_dwFtpSvrIpAddr;                           // FTP��������ַ
    unsigned short  m_wFtpSvrPort;                              // FTP�������˿�
    char            m_achFtpSvrPath[PUSDK_MAX_FILEPATH_LEN+1];  // FTP���������·��
    char            m_achUserName[64 + 1];                      // �û���
    char            m_achUserPassWord[64 + 1];                  // �û�����
} TPuFtpServer;

// �ļ��洢����
typedef struct 
{
    unsigned char byStoreType;      // �洢���ͣ�0-���洢,2-���ش洢
    TPuFtpServer  tPuFtpCfg;        // FTP����
    
    // ���ش洢����
    unsigned char              byUsedDiskPartionhNum;                          // �û�ʹ�õķ�������
    unsigned char              abyUsedDiskPartionId[ PUSDK_MAX_DISKPART_NUM ]; // �û�ʹ�õķ���·���б�(����Ӳ��ID)
} TPuFileStoreParam;

// ǰ��ץ��ͼƬ�洢����
typedef struct 
{
    TPuNamingRule      tNamingRule;       // ץ��ͼƬ��������
    TPuFileStoreParam  tFileStoreCfg;     // �洢����
} TPuSnapshotStoreConfig;

// ϵͳ��չ����
typedef struct
{
    unsigned char  byLinkChkInterval;                      // ��·�����(��λ:��,Ĭ��:10��(0~60s)
    unsigned char  byLinkChkDisNum;                        // ��·�������Դ���(��λ:��,Ĭ��:3��(1~255��)
    
    // ʱ��ͬ��
    unsigned char  byTimeSynMode;                          // ʱ��ͬ����ʽ 
    unsigned long  dwSynTimeServerIp;                      // ʱ��ͬ��������,0��ʾ��ͬ��(Ĭ��:ip=0)
    unsigned short wSynTimeServerPort;                     // (Ĭ��port=123)
    unsigned long  dwSynTimeInterval;                      // ͬ��ʱ����1~72*60����(Ĭ��:1)
    
    // socks5
    unsigned long  dwSocks5ServerIp;                       // SOCKS5 ���������,0��ʾ��ʹ��(Ĭ��:ip=0)
    unsigned short wSocks5ServerPort;                      // (Ĭ��port=1080)
    char           achSocks5UserName[ PUSDK_MAX_MTALIAS_LEN + 1 ];    // �û���
    char           achSocks5Password[ PUSDK_MAX_MTALIAS_LEN + 1 ];    // �û�����
    
    // Domain
    unsigned char  byUseDomain;                            // �Ƿ�ʹ������
    char           achPlatDomain[PUSDK_MAX_STRING_LEN64+1];// ƽ̨����
    unsigned char  byLdsMode;                              // LDSʹ��ģʽ��0��no use��1��IP�� 2��domain
    char           achLdsDomain[PUSDK_MAX_STRING_LEN64+1]; // LDS����
    unsigned long  dwLdsServerIp;                          // LDS ��ַ
    unsigned short wLdsServerPort;                         // LDS �˿�
    
    // other
    unsigned char  byStreamMaxSendNum;             // ��ǰ����ͨ���������·��(Ĭ��:6)
    
    int            bUseIndiLight;                  // �Ƿ���ָʾ��
    int            bSupportBuildNet;               // �Ƿ�֧������ TURE:֧��
    
    unsigned short wStartPort;                     // ���ط��ͻ������ʼ�˿�(Ĭ��: 60000)
    char           achDeviceIPosInfo[32+1];        // ǰ��λ����Ϣ
    unsigned long  dwDnsAddr;                      // ��ҪDNS
    unsigned long  dwDnsBackupAddr;                // ��ҪDNS
    
    unsigned char  byCameraInfoNotifyPlantformEnable;// ����ͷ�����ϱ�ƽ̨ʹ��
    
} TPuSysExtParam;

// ǰ�˵�Ե���Ϣ
typedef struct 
{
    int            bOpenP2PConnect;            // �Ƿ�����Ե�
    unsigned long  dwPuDestAddr;               // ��Ե�Զ�IP
    unsigned short wPuDestPort;                // ��Ե�Զ�Port
    int            bOpenRmtSerial;             // ��Ե��Ƿ�Զ�˶�����
    unsigned char  byDestEncChan;              // ��Ե�Զ˱���ͨ��
    unsigned char  bySendAud;                  // �Ƿ�����Ƶ
} TPuVideoDecPpParam;

// ���춨���ʱ���
typedef struct 
{
    unsigned long dwStartTime;                // ��ʼʱ��, hh*3600+mm*60+ss
    unsigned long dwEndTime;                  // ����ʱ��, hh*3600+mm*60+ss
} TPuDayPeriodTime;

// ������ʱ�䶨���ʱ���, 1970��ʼ
typedef struct 
{
    unsigned long dwStartTime;                // ��ʼʱ��, 1970��ʼ
    unsigned long dwEndTime;                  // ����ʱ��, 1970��ʼ
} TPuFixPeriodTime;

// ����ʱ���
typedef struct 
{
    unsigned char     m_byFixUsedNum;              // �̶�ʱ�����ʹ�õĸ���
    unsigned char     m_byDayUsedNum;              // ��ʱ�����ʹ�õĸ���
    unsigned char     m_byWeekUsedNum;             // ��ʱ�����ʹ�õĸ���
    
    TPuFixPeriodTime  m_atFixTimerTable[6];        // ��ʱ
    TPuDayPeriodTime  m_atDayTimerlyTable[6];      // ÿ�춨ʱ
    TPuDayPeriodTime  m_atWeekTimerlyTable[7*6];   // ÿ������˳��Ϊ[7*loop+week]
} TPuPeriodTable;

// ǰ��¼��ͨ������
typedef struct 
{
    unsigned char      byChanId;                   // ¼��ͨ��
    unsigned short     wAlarmPreRecTime;           // �澯Ԥ¼ʱ�䣬Ĭ��0��
    unsigned short     wAlarmResumeRecTime;        // �澯�ָ������¼��ʱ�䣬Ĭ��5��
    unsigned char      byIsAbstractFrame;          // �Ƿ��֡¼�� 1Ϊ�ǣ�0Ϊ��
    unsigned char      byIsRecWhenAlarmOccur;      // �澯�Ƿ�¼��,1Ϊ��,0Ϊ��
    unsigned char      byAbstractRecInterval;      // ��֡¼����,ָ�ؼ�֡������Ĭ��5
    unsigned char      byIsAudioRecorded;          // �Ƿ�¼��Ƶ��Ĭ��1����
    unsigned char      byRecChanNum;               // 0-¼��������������-¼�Ƹ���������,Ĭ��Ϊ0
    
    unsigned char      m_byRecTimeMode;            // ¼��ʱ��ģʽ��EPuRunTimeMode
    TPuPeriodTable     tPeriodTable;               // ����ʱ���
    
} TPuRecChannel;

// ǰ��¼�����ò���
typedef struct 
{
    unsigned char      byDiskFullStrategy;                 // ������¼����� 0-ֹͣ 1-ȫ���� 2-�����Ǹ澯¼��
    unsigned char      abyRecFileStorePath[16];            // ¼���ļ����·��
    unsigned long      dwDiskRemainSpace;                  // ����ʣ��ռ�(50MB)
    unsigned short     wLittleFileRecTime;                 // С�ļ�¼��ʱ��(2MB)
    unsigned char      byDiskPartitionNum;                 // �ļ�������Ŀ,�û���������
    unsigned char      abyRecDiskPartitionInfo[16][64];    // ���̷�����Ϣ���û���������
    unsigned char      byUsedPartitionNum;                 // �û�ʹ���˵ķ�����Ŀ,Ĭ��1
    unsigned long      dwDownloadSpeedLimit;               // ¼�������ٶ�KB(Ĭ��:1024)     
    unsigned char      byIsPltRecWhenRecErr;               // ¼�����ʱ�Ƿ�ƽ̨ת¼(Ĭ��0������)
    unsigned char      byIsAutoFixWhenRecErr;              // ¼������Ƿ��Զ��޸�(Ĭ��1����)
    unsigned char      byRecOsdState;                      // ¼��ͼ��״̬(Ĭ��1����)
    unsigned char      byPuDisConnRecEnable;               // ����ǰ��ת¼��0-��������1-����
    unsigned char      byIsAlarmWhenDiskErr;               // �����쳣�澯�ϱ�
} TPuRecSys;

// ǰ��¼���������
typedef struct 
{
    unsigned char      byChannelId;
	unsigned short     wAlarmPreRecTime;          // �澯Ԥ¼ʱ��
	unsigned short     wAlarmResumeRecTime;       // �澯�ָ������¼��ʱ��
	unsigned char      byIsAbstractFrame;         // �Ƿ��֡¼�� 1Ϊ�ǣ�0Ϊ��
	unsigned char      byIsRecWhenAlarmOccur;     // �澯�Ƿ�¼��,1Ϊ��,0Ϊ��
	unsigned char      byAbstractRecInterval;     // ��֡¼����,ָ�ؼ�֡����
	unsigned char      byIsAudioRecorded;         // �Ƿ�¼��Ƶ
    unsigned char      byRecChanNum;              // 0-¼����������������¼�Ƹ���������,Ĭ��Ϊ0
} TPuRecChanBasicInfo;

// ǰ��¼����չ����
typedef struct 
{
    unsigned long         dwDownloadSpeedLimit;      // (����)
    TPuFileStoreParam     tRecFileStoreCfg;          // ¼��洢��Ϣ
    
} TPuRecSysEx;

// ����ͷԤ��λ
typedef struct 
{ 
    unsigned char byUsePreset;                // Ԥ��λʹ��    
    char          achPresetAlias[16];         // Ԥ��λ����
} TPuCameraPresetInfo;

// ����ͷ����
typedef struct 
{
    unsigned char      byCurPresetNo;   // ��ǰԤ��λ
    unsigned char      byInverseMode;   // ��ǰ��ת����
    unsigned char      byUseAutoBack;   // �Ƿ��Զ���λ
    unsigned short     wAutoBackTime;   // �Զ���λʱ��
    unsigned char      byScanMode;      // �Զ�ɨ�跽ʽ
    
    unsigned char      byScanTimeMode;  // ɨ��ʱ��ģʽ, EPuRunTimeMode
    TPuPeriodTable     tScanPeriodTalbe;// �Զ�ɨ��ʱ���
    
    TPuCameraPresetInfo  tPresetInfo[8];   // Ԥ��λ��Ϣ
} TPuCameraCfg;


//������������ṹ
typedef struct 
{
    unsigned char    byAlarmDetectState;    // EPuRunTimeMode
    
    unsigned char    byRelateEncId;         // �澯�����ı���ͨ�� Ĭ��0
    unsigned char    byAlarmCheckState;     // �澯״̬ 0-��Ч 1-�͵�ƽ 2-�ߵ�ƽ 3-�պ� 4-��
    char             achChanAlais[32 + 1];  // ����
    TPuAlarmLinkInfo tAlarmLinkInfo;        // �澯��������
    TPuPeriodTable   tCheckPeriodTable;     // ���ʱ���
	
} TPuInputPinParam;

// �������ͨ������
typedef struct 
{
    unsigned char m_byAlarmMode;                      // �澯ģʽ(Ĭ��:3-�պ� 4-��)
    char          m_achOutputPinChanAlais[32 + 1 ];   // �������ͨ������(Ĭ��:�������ͨ��+id)
} TPuOutputPinParam;

// DNS����
typedef struct 
{
    unsigned long dwDnsAddr;          // DNS��ַ
    unsigned long dwDnsBackupAddr;    // DNS���ݵ�ַ
} TPuDnsParam ;
//! DDNS����
typedef struct 
{
    unsigned char   byState;                              // ע��״̬
    unsigned char   byDomainNameNum;                      // ���ص���������
    char            abyDomainName[32][64+1];              // ��������
    unsigned long   dwDonator;                            // �ʻ����ͣ���ѣ��շ�
    TPuDnsParam     tDnsCfg;
} TPuDomainName;

typedef struct 
{
    unsigned char byUseDdns;                                 // �Ƿ�ʹ��DDNS
    unsigned char byServiceNum;                              // ֧�ַ������
    unsigned char abyService[2];                             // �������ͣ�3322��dyndns
    unsigned char bySrvType;                                 // ��ǰ��������
    char          achDnsServer[16];                          // dns server ip��ַ
    char          achUsername[128];                          // ��֤�û��� 
    char          achPassword[128];                          // ��֤���� 
    char          achHostname[128];                          // ����
    char          achInterface[16];                          // �������ڵ�ַ
    TPuDomainName tPuDomain;                                 // ���ص�������Ϣ
} TPuDdnsParam;

//�ƶ��������ṹ��
typedef struct 
{
    unsigned long  dwXPos;                 // ������򶥵������
    unsigned long  dwYPos;                 // ������򶥵�������
    unsigned long  dwWidth;                // ��������ȣ���Χ 0-720
    unsigned long  dwHeigth;               // �������߶ȣ���Χ 0-576
    
    unsigned char   byDetectEnable;         // ���ʹ�� 1-��Ч 0-��Ч
    unsigned char   byAlarmRate;            // �������澯�����ٷֱȣ����ڴ��ٽ�ֵ�澯,0-100��
} TPuMotionDetectArea;

//�����ն��ƶ�������
typedef struct 
{
    unsigned short      wAreaNum;         //������Ŀ
    TPuMotionDetectArea atDetectArea[PUSDK_MAX_MOTION_DETECT_AREAM_NUM];      //�ƶ��������ṹ������
    
    unsigned char       byDetectTimeMode;      // ���ʱ��ģʽ, EPuRunTimeMode
    TPuPeriodTable      tCheckPeriodTable;     // ���ʱ�����ڱ�
    
    unsigned char       byDetectSense;        // ֡��������(Ĭ��:60,��Χ:0~99%)
    TPuAlarmLinkInfo    tAlarmLinkInfo;       // �澯����
} TPuMotionDetectParam;

// ��չ������
typedef struct 
{
    unsigned char        byRelateSerialId;   // ��չ����������Id��         
    unsigned char        byPinExtSerialAddr; // ��չ���മ�ڵ�ַ�� 
} TPuExtCard;

// ǰ����չ������
typedef struct 
{
    unsigned char  byExtCardNum;
    TPuExtCard     atExtCard[8];
} TPuExtCardParam;

//��Ƶ����ͼ��ֱ���
typedef struct 
{
    unsigned short wWidth;      // ͼ���
    unsigned short wHeight;     // ͼ���
    unsigned short wRefreshRate;// ˢ����
} TPuResolution;

//��Ƶ����ͼ����ʾ�����ṹ
typedef struct 
{
    TPuResolution atSupportResType[64];  // Ŀǰ���֧��64��
    unsigned char bySupportResTypeNum;   // ֧�ֵķֱ�����������
    unsigned char byCurrentResType;      // ��ǰʹ�õķֱ���
    unsigned char bySupportOutputMode;   // ֧�ֵ����ģʽ(ֻ��) ��λ 0-�� 1-BNC 2-VGA 4-HDMI 8-BNC+VGA 16-BNC+HDMI
    unsigned char byCurrentOutputMode;   // ��ǰʹ�õ����ģʽ 
} TPuImageDisplayParam;

//��Ƶ����໭������ṹ
typedef struct 
{
    unsigned char byCompStyle;       // �ϳɷ�� 1-1���� 2-4���� 4-9���� 8-16����
    unsigned char byCompMemberNum;   // �ϳ�����
    char achCompMember[64];          // ��Ա�б�
} TPuVideoDecCompParam;

// ------------------------------------------------------- //
// �ⲿ���ݲɼ��豸
typedef struct
{
    unsigned char  byOsdVideoEncId;       //�����ı���ͨ��
    char           achDeviceCode[32 + 1]; //�豸�루�豸��ʶ����MN�ֶΣ�
    unsigned long  dwOsdColor;            //������ɫ(RGB)
    unsigned short wOsdXpos;              //X����
    unsigned short wOsdYpos;              //Y����
    unsigned long  dwReserved;            //�����ֶ�
} TPuDAQDevice;

typedef struct 
{
    unsigned char  byState;        //���ڱ�ʶ�Ƿ�����
    unsigned char  byProtocolId;   //Э��ID��Э�����ͣ�����ǰ���豸ֻ��һ������Э��
    unsigned char  byNum;          //��Ч����
    TPuDAQDevice   atDevice[8];    //�豸�б�
} TPuDAQDeviceParam;

//////////////////////////////////////////////////////////////////////////
// �ļ���������� //
#ifdef WIN32
typedef struct 
{
    unsigned __int64        m_qwTotalSize;		// ��С
    unsigned __int64        m_qwCreateDate;     // ����ʱ��
    unsigned long           m_dwTotalTime; 		// ����ʱ��
} TPuPlayFileInfo;
#endif
typedef struct 
{
    int            m_bAudio ;           // �Ƿ�����Ƶ��
    unsigned short m_wWidth;            // ͼ����
    unsigned short m_wHeight ;          // ͼ��߶�
    unsigned char  m_byStreamId ;       // �������
    unsigned char  m_byMediaType ;      // �����ʽ
} TPuPlayFileStreamInfo ;

typedef struct 
{
    unsigned long m_dwCurrentPlayPos;   // ���Ž���
    unsigned char m_byPlayMode;         // ���ŷ�ʽ
    char          m_byPlayRate;         // �����ٶ�
    unsigned char m_byPlayState;        // ����״̬ 1-��ʼ 0-ֹͣ
    unsigned char m_byStreamNum ;       // ������
    unsigned long m_dwVFps;             // ��Ƶ֡��
    unsigned long m_dwAFps;             // ��Ƶ֡��
    TPuPlayFileStreamInfo  m_atFileStreamInfo[3]; // ������Ϣ
} TPuPlayFileStatus;

//////////////////////////////////////////////////////////////////////////
// Ӧ����Ϣ

// PING��Ӧ��
typedef struct 
{
    unsigned long      m_dwIpAddress;
    unsigned short     m_wPort;
    
    unsigned long      m_dwVersionID;
    unsigned char      m_abyName[PUSDK_MAX_STRING_LEN64+1];       // �豸��
    unsigned char      m_abyTypeName[PUSDK_MAX_STRING_LEN64+1];   // �豸������(ֻ��)
    unsigned char      m_byDevType;                               // �豸����(EPuDeviceType) 
    unsigned char      m_abySeriesName[PUSDK_MAX_MTALIAS_LEN+1];  // �豸ϵ����
} TPuPingAck;

// ����ԭ��
typedef struct
{
    EPuDropType m_eType;
} TPuDropReason;

// �첽��ϢӦ��
typedef struct 
{
    unsigned short     m_wErrorCode;
} TPuResult;

// ������Ϣ
typedef struct 
{
    unsigned char   byRebootType;                    // ��������
    unsigned long   dwRebootTime;                    // ����ʱ��
    char            achRebootHint[128];              // ������ʾ
} TPuRebootInfo;

// �����������ݽṹ
// AP����Ϣ
typedef struct
{
    unsigned char  byIsActive;       // ��AP���Ƿ��ǻ�� 
    unsigned char  byHiddenAP;       // �Ƿ�Ϊ���ؽڵ� 
    unsigned char  byChannelId;      // Ƶ���� 
    unsigned char  byBSSType;        // ������ʽ:EVsipWifiApInfo_byBSSType 
    unsigned char  byVerifyTpye;     // �����֤��ʽ: EVsipWifiApInfo_byVerifyTpye
    unsigned char  byArithmetic;     // �����㷨:EVsipWifiApInfo_byArithmetic 
    int            dwMaxRate;        // AP����������ٶ�, ��λ: kbps 
    unsigned char  byRSSI;           // �ź�ǿ��:0��100 
    unsigned char  abyBSSIDMac[6];   // AP���MAC��ַ 
    char           achSSID[32+1];    // AP������� 
    unsigned char  abyPwdsValid[4];  // ��Կ��Ч�� WEP_KEY_NUM = 4
    char           achPasswd[4][64]; // ��Կ�� 
    unsigned char  byEncryMode;      // ���ڱ�ʾ����ģʽ: EVsipWifiApInfo_byEncryMode 
} TPuWifiApInfo;

// ��ȡ����Ӧ��
typedef struct
{
    unsigned char  byAPNum;            // ɨ�赽��AP����� 
} TPuWifiCfgGetResp;

// ����/�Ͽ�����
typedef struct
{
    unsigned char  byConnect;          // ���ӻ�Ͽ� 
    char           achSSID[32+1];      // AP������� 
    unsigned char  byChannelId;        // Ƶ���� 
} TPuWifiConnectionReqParam;

// �ϱ�WIFI����״̬
typedef struct
{
    unsigned char  byWifiState;        // Wifi״̬; ���ӡ��Ͽ������豸��
    unsigned char  byBSSType;          // Infra����Ad-Hoc 
    unsigned char  abyBSSIDMac[6];     // AP���MAC��ַ 
    char           achSSID[32+1];      // AP������� 
    unsigned char  byChannelId;        // Ƶ���� 
    unsigned char  byRSSI;             // �ź�ǿ�ȣ�0��100 
    unsigned long  dwMaxRate;          // AP�����ٶ�, ��λ: kbps 
} TPuWifiConnectionStatus;

typedef struct
{
    unsigned char byAddrMode;    // ��ַʹ�÷�ʽ���̶�IP����DHCP 
    unsigned long dwUnitIpAddr;  // �豸IP��ַ
    unsigned long dwSubnetMask;  // �豸��������(������)
    unsigned long dwGateWay;     // ���ص�ַ(������)
    unsigned long dwDNSaddr;     // DNS��ַ(������)
} TPuWifiNetParam;

// 3G���ýṹ
typedef struct
{ 
    unsigned char  byAutoDisConn;      // �Զ��Ͽ����� 
    unsigned char  byPhoneListNum;     // �绰���������� 
    char           achPhoneNum[5][32]; // �绰���� 
    unsigned char  byMsgListNum;       // ��Ϣ���������� 
    char           achMsgNum[5][32];   // ��Ϣ������ 
    unsigned long  dwRmgIp;
    unsigned short wRmgPort;
} TPu3gCfgParam;

// 3G״̬
typedef struct
{
    unsigned short wLinkState;        // ����״̬��ʹ��unsigned short�����룬0-���� 
    unsigned long dwSignalStrength;   // �ź�ǿ����ǿΪ100������Ϊ0 
    unsigned long dwOnLineTime;       // ����ʱ������λΪ���� 
    unsigned long dwInDataCount;      // ����������������λΪKb 
    unsigned long dwOutDataCount;     // ����������������λΪKb 
    unsigned long dwLocalIpAddr;      // ����IP 
    unsigned long dwDns1Addr;         // DNS 1 
    unsigned long dwDns2Addr;         // DNS 2 
    unsigned long dwNetType;          // �������ͣ�1->EVDO, 2->WCDMA, 3->TD-WCDMA
    char          achVendor[16 + 1];  // 3GоƬ�ṩ�̣�HUAWEI, QUALCOMM
    char          achProduct[16 + 1]; // 3GоƬ����: EM660, MI600
} TPu3gStatus;

// ������ʽ
typedef struct
{
       unsigned char byWirelessMode;     // ���߷�ʽ 
} TPuWirelessModeParam;

// GPS������������/��ѯӦ��
typedef struct
{
    unsigned char  byEnabled;      // 1��������0��ȡ��
    unsigned short wType;          // ͸����������
    unsigned long  dwSubType;      // ͸������������
} TPuGpsSubScribeParam;

// GPS��������Ӧ��
typedef struct
{
    unsigned short wError;    //�����1-�ɹ� 0-ʧ��
} TPuGpsSetSubScribeAckParam;

// GPS���Ĳ�ѯ����
typedef struct
{
    unsigned short wType;              //͸����������
    unsigned long  dwSubType;          //͸������������
} TPuGpsGetSubScribeParam;

// GPS����
typedef struct
{
    unsigned char  byEnabled;        //�Ƿ�ʹ��
    unsigned short wCollectInterval; //�ɼ����
    unsigned short wNotifyInterval;  //�ϱ����
} TPuGpsCfgParam;

// GPS֪ͨ��Ϣ
typedef struct
{
    unsigned long  dwTime;                 //UTCʱ��
    unsigned char  byAvailability;         //��Ч��
    char           chNSIndicator;          //�ϱ�γ��ָʾ
    unsigned char  byLatitudeHigh;         //γ�ȣ���
    unsigned long  dwLatitudeLow;          //γ�ȣ������֮һ��
    char           chEWIndicator;          //������ָʾ
    unsigned char  byLongitudeHigh;        //���ȣ���
    unsigned long  dwLongitudeLow;         //���ȣ������֮һ��
    unsigned short wSpeed;                 //�ٶȣ�����/Сʱ
    unsigned short wSpeedLow;              //�ٶȣ�ǧ��֮һ����/Сʱ
    unsigned short wDirectionHigh;         //���򣬶�
    unsigned short wDirectionLow;          //����ǧ��֮һ��
    unsigned long  dwAltitudeHigh;         //���Σ���
    unsigned char  byAltitudeLow;          //���Σ�����
    unsigned char  bySatelliteNum;         //���ڽ���λ�õ���������
} TPuGpsNotifyParam;


// ������Ƶ�ļ�����
typedef struct 
{
    unsigned long      m_dwGranuity;
    unsigned long      m_dwContext;
    PUSDK_FILE_STAT_CB m_pfCB;
    unsigned char      m_byFileType;   // PUSDK_FILE_TYPE_ASF
    char               m_achFileName[512];
}PUSDK_TPuPlayFileParam ;

// ǰ��¼����������
typedef struct  
{    
    unsigned long      dwRecFileIndexId;          // ǰ��¼���ļ�����ֵ
    TPuSysTime         tStartTime;                // �����ļ���ʼʱ��
    TPuSysTime         tEndTime;                  // �����ļ�����ʱ��
    char               achRecFilePath[MAX_PATH];  // �����ļ�·��
    char               achSavePath[MAX_PATH];     // ���ر���·��
    PUSDK_DOWN_REC_CB  fRcvCallBack;              // ¼�����ؽ��Ȼص�����
    void*              pCallBackContext;          // �ص����������Ĳ���
    unsigned short     wGran;                     // ¼�����ؽ��Ȼص�����(��λ:��)
    PUSDK_DOWN_REC_DATA_CB fDownDataCB;           // ¼���������ݻص�����
    void*              pDownDataCBContext;        // ¼���������ݻص�����
}PUSDK_TPuRecFileDownloadReq;


// ǰ��ע��ص�����ָ��
typedef void (*FUNC_CONNECTED_ACK)( PUHANDLE hHandle, TPuUnitInfo *ptPuInfo, void *pContext );


// ��������ͨѶ
typedef struct
{
    unsigned short wType;       // ��������
    unsigned short wLen;        // ͨѶ���ݳ���
    char           abyBuf[64+1];// ͨѶ����
}TProTestInfo;

// �汾���� 
typedef struct 
{   
    unsigned char    byFileType;                         // ���������ļ����� PUSDK_UPDATE_TYPE_ALL
    char             achFileInfo[64 +1];                 // ���������ļ�·�����ļ���
    unsigned long    dwFtpIp;                            // FTP��������ַ
    unsigned short   wFtpPort;                           // FTP�������˿� PUSDK_DEFAULT_FTP_PORT
    char             achFtpName[ 256 + 1 ];              // FTP�û���
    char             achFtpPassWord[ 256 + 1 ];          // FTP�û�����
} TPuVerUpdateInfo;

typedef struct
{ 
	char achCaption[MAX_CAPTION_NUM+1]; // ��Ļ����
}TBmpCaption;

// ��Ļ������
typedef struct  
{
	int  nNum;                                 // ��Ļ����
	int  nMaxLen;                              // ��Ļ����ÿ�����ʹ�ó���
	TBmpCaption tCaption[MAX_BMP_CAPTION_NUM]; // ��Ļ����
}TOsdBmpCaption, *PTOsdBmpCaption;

// ��Ȩ���ؽ�� 
typedef struct 
{
    unsigned short wResult; // �����붨��:EPuLoginPuError
} TPuUserPasswordIdentifyAck;


// ֡�ص��ṹ
typedef struct 
{
	unsigned char    m_byMediaType;   // ý������
    unsigned char*   m_pbyData;       // ���ݻ���
    unsigned long    m_dwPreBufSize;  // m_pData����ǰԤ���˶��ٿռ䣬���ڼ�
	// RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12
	// (FIXED HEADER + Extence option + Extence bit)
    unsigned long    m_dwDataSize;    // m_pDataָ���ʵ�ʻ����С�����С
    unsigned char    m_byFrameRate;   // ����֡��,���ڽ��ն�
    unsigned long    m_dwFrameID;     // ֡��ʶ�����ڽ��ն�
    unsigned long    m_dwTimeStamp;   // ʱ���, ���ڽ��ն�
    unsigned long    m_dwSSRC;        // ͬ��Դ, ���ڽ��ն�
    union
    {	
        struct{
			int              m_bKeyFrame;    // Ƶ֡���ͣ�I or P��
			unsigned short   m_wVideoWidth;  // ��Ƶ֡��
			unsigned short   m_wVideoHeight; // ��Ƶ֡��
		}m_tVideoParam;
        unsigned char        m_byAudioMode;  // ��Ƶģʽ
    }x;
}TFrameHeadInfo, *pTFrameHeadInfo;
// �ص��յ���һ֡���ݡ�
// pFrmHdr��ָ��һ��FRAMEHDR�ṹ�壬�ýṹ������������֡��������ߣ��Ƿ�ؼ�֡���Լ�ָ��֡���ݵ�ָ���
// dwParam���û��������Ĳ���
typedef void (*PUSDK_FRAME_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

//CMU �汾�����ݻص�
typedef void (*PUSDKCMU_FRAME_CB)( PUHANDLE hHandle, unsigned char byChanId, TFrameHeadInfo* pFrmHdr, void* dwContext );

//����YUV���ݻص�
typedef void (*PUSDK_YUV_DATA_CB)( PUHANDLE hHandle, PLAYERHANDLE hPlayer, unsigned char* pBuf, unsigned long nBufSize, 
								   int nWidth, int nHeight, TFrameHeadInfo* pFrmHdr, unsigned long dwContext );

// ��ƵԴ״̬
typedef struct 
{
    EEventType eState; // ״̬
}TPuVideoInState;

// �ƶ�����ϱ�
typedef struct 
{
	unsigned short   wAlarmAreaNum; // ��������(��Ч)
    EEventType       eState;        // ״̬
} TPuMotionDetAlarm;

// Ӳ���쳣�ϱ�
typedef struct 
{
    unsigned short    wRecError; // ������:EPuRecError
} TPuRecErrorNotify;

// ���������ϱ�
typedef struct 
{
    EEventType   eState; // ״̬
} TPuInputPinAlarm;

// �����ϱ��ṹ
typedef struct 
{
    unsigned char     byComId;              // ����ͨ���ţ�һ��ֻ��һ�����ڣ���Ϊ0
    int               dwReserved;           // ��������������Ϊ�������ݱ�ʶ
    unsigned short    wMsgLen;              // ��Ϣ����
    char              abyMsgBuff[256];
} TPuComData;

typedef struct
{
    EPu3gNetType e3gNetType;                  // 3G
    char achUsername[32];     // �û���
    char achPassword[32];     // ����
    char achPhoneNumber[16];  // ���Ӻ���
} TPu3gNetParam;

typedef struct
{
    unsigned char	by3gNetNum;         // 3G�������
    TPu3gNetParam	at3gNetParam[5];
    EPu3gNetType	eCurUsedNetType;    // ��ǰʹ�õ���������
} TPu3gNetCfg;

// for ipc
typedef struct
{
    unsigned short	wReciprocal;            // ��ǰ�Ŀ��Ų�������
    unsigned short	wSupportListNum;        // ֧�ֵĿ��Ų����б���(ֻ��)
    unsigned short	awSupportList[MAX_RECIPROCAL_LIST_NUM]; // (ֻ��)
    unsigned long	dwReserved1;
    unsigned long	dwReserved2;
} TPuSnapConfig;

// for ipc
typedef struct
{
    unsigned short  byCmdType;
    unsigned long   dwValue;
} TPuCameraCtrl;

typedef struct 
{
  unsigned char      byConfigType;   // EVsipLoadDefualt
} TPuLoadDefaultConfig;

#endif

