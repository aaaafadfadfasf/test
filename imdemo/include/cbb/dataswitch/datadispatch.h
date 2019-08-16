/*****************************************************************************
   ģ����      : DataDispatch
   �ļ���      : datadispatch.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: DataDispatchģ��ӿں�������
                ��ԭ��DataSwitch����һ�£��ڲ��ṹ�Ż���ȫ������C++����ִ��
                ����֧��RTP���壬�����Ŭ����֤�ش�RTP��ֻ������Ҫ��Ŀ��
                ����ԭ��DataSwitch�ӿ�
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2013/02/28  1.0         �� ��        ����
******************************************************************************/
/*****************************************************************************
2013.4.17 �޸ļ�¼���޸��� ����
1��Vtdu��ɾ����ͨ�����������ɾ��Dump����
��λ���ֲ����յ�RecvGrpδ����ԭ���� RemoveDump δ�ж�RecvGrp.IsEmpty()��ɾ��֮

2��VTDU����һ��STUN�ܳɹ����ӿڵ��÷�ʽ�� ��SetFilterFunc���ٵ���NatAgent.QueryStun()
ԭ��DDģ���ж�RecvGrp.IsEmpty()�������ǣ�1����ͨ�������ڣ�2��DUMP���ڡ����϶��߽�Ϊ��
�����п��ܵ�һ��AddFilter�󴴽���RecvGrp�������������ͨ������RecvGrp�ͱ�ɾ���ˣ�����FilterҲ��ɾ��
ԭDataSwitchģ���а�Filter��Dump��ͬ����

�޸�DDģ�飬AddFilterʱ���DUMP����DelFilterʱҲ��ɾ��Dump����
(ʵ����Dump������Բ�������ֻ��Ҫ�޸�RecvGrp.IsEmpty()������Filter != NULL���ж�)
�� AddDump��AddFilter��ͬ��RemoveDump��DelFilter��ͬ

3��Ϊ�˷�ֹDataDispatch������ʱ������ģ����ų�ͻ���ڲ����namespace NSDataDispatch
�ӿڲ��䣬Ҳ����֪�������ռ�

2014.1.15 �޸İ�ͳ�ƣ��޸��� ����
ԭ��ֻ��RuleSet����ͳ�ư������½����׼ȷ����ʹһ��RuleSet�ڣ���ͬ�����Ķ�������ǲ�һ����
�������Rule����ͳ�ƣ�ÿһ·�����Ķ����������ȷͳ��
�����޸Ĳ�Ӱ��ԭ���߼���Ҳ��Ӱ��Ч�ʣ��ڲ����ش����Ĵ���ȡ���ˣ�����Ч�ʿ��ǣ�

2014.1.16 ����������ά�澯���޸��� ����
���� ���������������������澯

2014.05.29 ���Ӷ�Rtcp���ĵĴ����޸�������
����������RTCP��SR/RR����
�����Զ��巢�ͽӿڣ��Ա�vtdu�ܹ����ڷ��ͱ����

2014.8.19 �޸�������
Datadispatch֧�ֹ����Լ������SR/RR�������ְ���ƽ̨��������������ģ�vtdu��Ҫ��ƽ̨�����ı�Ե������
Datadispatch֧���޸�RTP����SSRC������Խ�һ��ƽ̨��Ҫ��Invite::sdp����������SSRCһ��

2014.11.28 �޸��� ��С��
1. Datadispatch֧������������������ͨ���ϸ�У�齻���е�������ַ������ʵ�ʷ��͵�ַ�Ƿ�ƥ����ʵ�֡�
   ������u32��u16�����αȽϣ����۷�����ת�����ܼ���û��ʲôӰ�졣�ù��ܿ��������Ƿ�����
2. ����dsrule����Ĵ�ӡ�����˿ڽ���ʱ��ӡ��������Դ��ַ����������*:*���棬���������Ų顣
3. ��ת��ģʽ�����Ӷ������ش��������ӡ�������ƴ洢�����Ų顣
4. ����dscfg����������ڴ�ӡ�ϲ����ø�ds��������Ϣ

2014.12.16 �޸��� ��С��
rtcp�ش����к���rtp��ӳ���ϵ����hashmapʵ�֣����֮ǰ��ȡģ�㷨����Ϊȡģ�㷨����
ѭ�����⣬ʹrtcp���кź�ʵ�ʵ�rtp����һ�£�����medianet���û�����������������ش�����
�����Ĳ���Ч����

2015.02.04 �޸��� ��С��
DS�ڽ������ݰ�ʱ��֧����ĳ���˿ڴ����հ����ٽ��������Ľ����鴦��ʵ�ְ�Դ��˿ڵĽ���ģʽ��

******************************************************************************/
#ifndef _DATA_DISPATCH_H_
#define _DATA_DISPATCH_H_

#include "cbb/osp/osp.h"

/*DataDispatch ����ֵ*/
#define DSOK    1
#define DSERROR 0

//����DataDispatch�Ŀɿ�����
enum EReliableLevel
{
    RL_TRANS_DATA   = 0,
    RL_PARTIAL_BUFFERING,       //���ֻ��壬��֤DS��Ŀ��ֱ�Ӳ��㲥�ش���
    RL_FULL_BUFFERING,          //ȫ���壬��֤���������ش���������С����Ҫ����2G�����ڴ�
};

/*
0    0    0    0
d    a    v    r/c
�� 0 bit�ϣ�0��ʾRtp��1��ʾRtcp��
�� 1 bit�ϣ�1��ʾVideo��0��ʾû��
�� 2 bit�ϣ�1��ʾAudio��0��ʾû��
�� 3 bit�ϣ�1��ʾData��0��ʾû��
*/
enum ESwitchType
{
    ST_INVALID = 0,
    ST_VIDEO_RTP = 2,       //0010
    ST_VIDEO_RTCP = 3,      //0011
    ST_AUDIO_RTP = 4,       //0100
    ST_AUDIO_RTCP = 5,      //0101
    ST_DATA_RTP = 8,        //1000 //����������ǵ���ģ���ֻ��RTP����
    ST_DATA_RTCP = 9,       //1001
};

//��־��������
enum EDSLogLev
{
    emLog_None                  = 0,        //�ر����д�ӡ
    emLog_Error                 = 1,        //���󼶱�
    emLog_Warning               = 2,        //���漶��
    emLog_Info                  = 3,        //��Ϣ����ͨ������
    emLog_All                   = (u32)(~0),//�ſ����д�ӡ
};

enum EDSAlarmType
{
    emDSAlarm_NO_Alarm  =  0,           //�޸澯
    emDSAlarm_Lost_Pack =  1,           //��������Ϣ�� u32 x 3 ExpectedSeqNO, RecvSeqNO, LostPackRate
    emDSAlarm_Confused_Pack = 2,        //����������Ϣ�� u32 x 2N dwSrcIP, wSrcPort��ַ��
    emDSAlarm_NO_Pack   =  3,           //�������������VideoRtp����Ϣ��ͬ��һ��������vctSwitchSsnIDs
    emDSAlarm_Num,
};

enum EDSRspMsgType						// DS���ظ��ϲ��Ӧ����Ϣ
{
	emDSRspMsgBegin   = 0,
	emDSGetPacketInfo					// ��Ϣ�� TPacks 
};

//RTCP��������
enum ERTCPType
{
    emRTCP_SR = 200,                  //Sender Report
    emRTCP_RR = 201,                  //Receiver Report
    emRTCP_SDES = 202,                //Source Describe
    emRTCP_BYE = 203,
    emRTCP_APP = 204,
    emRTCP_Num = 5
};

#define RULESET_MAX_WORK_DELAY  600000  //RuleSet�Ӵ������յ���һ����������ӳ�ʱ�䣬��λms�������ж�������

typedef unsigned int TSwitchSsnID;
#ifndef INVALID_SWITCH_SSN_ID
#define INVALID_SWITCH_SSN_ID (TSwitchSsnID)0
#endif
// �������256kbyte������֡���ԣ�С���� <  96
#ifndef MAX_PACK_NUM_PER_FRAME
#define MAX_PACK_NUM_PER_FRAME 96
#endif

#define DEFAULT_SOCKET_BUFFERSIZE (128*1024*1024)

#ifdef _LINUX_
#define DOMAIN_SOCKET_PATH "/opt/kdm/data/unixsocket/%u-%d"
#endif

/*
һ֡��Ƶͼ��ؼ�֡��Լ��9������ɣ���D1�㣩
��30 fpsΪ����ÿ���Ӵ�Լ 30 * 9 = 270
����Ŀ�Ķ�CU���ش����ȼ����㣬���1840 ms
RingBuffer��������� = 1.84 * 270 = 497 �������ٿ���1080P���
��RingBuffer����������Ϊ 700

һ֡��Ƶ֡һ������һ֡�����RingBufferΪ100
*/
#define MAX_VIDEORTP_BUFFERING_PACK_NUM 700
#define MAX_AUDIORTP_BUFFERING_PACK_NUM 100

#define			DATASWITCH_PORT_START		21050
#define			DATASWITCH_PORT_END			21099

#define DD_INVALID_SSRC1    ((u32)0)
#define DD_INVALID_SSRC2    (~(u32)0)

struct TPacks
{
	TPacks()
	{
		dwRecv = 0;
		dwSend = 0;
		dwSendFailed = 0;
	}

	u32 dwRecv;             //�յ����ٰ�
	u32 dwSend;             //���Ͷ��ٰ�
	u32 dwSendFailed;       //����ʧ��
};

struct TPacksWithTime
{
	TPacksWithTime()
	{
		tTime = 0;
	}

	time_t tTime;
	TPacks tPacks;
};

struct TRtpInfo
{
    TRtpInfo()
    {
        m_wSeqNO = 0;
        m_dwSSRC = 0;
    }
    u32 m_dwSSRC;
    u16 m_wSeqNO;
};


/*
RTCP����ΪSR,RR,SDES,BYE,APP�����͵İ�
Ŀǰ���ǹ�˾����RTCP�е��Զ���SDESʵ���ش�����
���PFGetRtcpInfo����ֻ������˾��SDES���ͣ�//��������ֱ�ӷ��أ����ս�������͸������
2014.5.29 -- by liangli
RTCP���������жϰ���Դ�������ԴPU��ַ�����ģ�������ת��������Ŀ��CU
�����ս�������͸������
ֻ������SDES����Ҫ��¼�ش�����
*/
struct TRtcpInfo
{
    TRtcpInfo()
    {
        m_dwSSRC = 0;
        m_byNum = 0;
        //memset(m_awNeedReSendSeqNO, 0, sizeof(m_awNeedReSendSeqNO));
        m_awNeedReSendSeqNO[0] = 0;
        m_emRtcpType = emRTCP_SDES;
        m_isKedaResendReq = 0;
    }
    u32 m_dwSSRC;
    u8  m_byNum;
    u16 m_awNeedReSendSeqNO[MAX_PACK_NUM_PER_FRAME];
    ERTCPType m_emRtcpType;
    u32 m_isKedaResendReq;  //�Ƿ�ƴ���ش�����
};

//����ֵ true��ʾ��RTP������Ҫ����false����������͸������
typedef BOOL32 (*PFGetRtpInfo)(const void* pBuf, s32 nSize, TRtpInfo* pInfo);
//����ֵ true�����ð����ش�������Ҫ�����ת���� false������RTCP����͸������
typedef BOOL32 (*PFGetRtcpInfo)(const void* pBuf, s32 nSize, TRtcpInfo* pInfo);

/*
���˵�������½ӿ��У�ȱʡdwIPΪ������wPortΪ������
*/

/**
 * @func FilterFunc
 * @brief ���չ��˺���
 *
 * ÿ�����ս����һ�����˺��������˽����յ�UDP���ݰ�ʱִ�д˺�����
 * �����ݺ����ķ���ֵ������̬�ؾ����Ƿ�Դ����ݰ�ת����
 *
 * @param dwRecvIP        - ����IP
 * @param wRecvPort      - ����Port
 * @param dwSrcIP         - ԴIP
 * @param SrcPort       - ԴPort
 * @param pData          - [in, out]���ݰ��������޸ģ�
 * @param uLen           - [in, out]���ݰ����ȣ��޸ĺ�ĳ��Ȳ��ɳ������ֵ
 * @return 0, �����ݰ���Ч�� ����ֵ�������ݰ���Ч��
 */	
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, 
  u8* pData, u32 uLen);

/*
�澯�ص�: ������һ·��������������������������������ֻ���Video RTP���ͽ�����
*/
typedef u32 (*PFAlarmCallBack)(u32 dwRuleNum, TSwitchSsnID *adwSsnID, ESwitchType eType, EDSAlarmType eAlarm, u32 dwParamNum, u32 *adwParams);

/*
 * ͨ����Ϣ�ص���
 */
typedef void (*PFMsgCallBack)(u32 dwTaskNO, u16 wEvent, void *pvContent, u32 dwLen);

/************************************************************
 * ��������dsInit ��ӦdsCreate
 *
 * ���ܣ�  ��ʼ��DataDispatchģ��
 *
 * ʵ��˵���� 
 * DataDispatch����֧�֡����û�����ȥ��DSID
 * 
 * ����˵����
 * DataDispatchģ����Ƴɴ�����RTP���ݵĿ�ܣ������˷�����׼RTPЭ��ĺ�����Ĭ�ϲ��ñ�׼����
 * ��������û�ʹ�õ��ǷǱ�RTPЭ�飬���趨��صĽ�������ָ��
 * DataDispatch֧�������ֲ������ʽ��ͨ�õĴ���Ǳ�RTP
 * 
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
struct TDsInitInfo
{
    TDsInitInfo()
    {
        eLevel = RL_TRANS_DATA;
        pfGetRtpInfo = NULL;
        pfGetRtcpInfo = NULL;
        //dwSockBuffSize = DEFAULT_SOCKET_BUFFERSIZE;
        dwRecvBuffSize = DEFAULT_SOCKET_BUFFERSIZE;;
        dwSendBuffSize = DEFAULT_SOCKET_BUFFERSIZE;;
        dwVideoRtpPackBuffSize = MAX_VIDEORTP_BUFFERING_PACK_NUM;
        dwAudioRtpPackBuffSize = MAX_AUDIORTP_BUFFERING_PACK_NUM;
        dwCPUAffinity = 1024;   //Ĭ�ϲ����κκ�
        bIsDefenseCrossStreamEnable = FALSE;
        pfAlarmCallBack = NULL;
		pfMsgCallBack   = NULL;
    }
    EReliableLevel eLevel;
    PFGetRtpInfo pfGetRtpInfo;
    PFGetRtcpInfo pfGetRtcpInfo;
    //u32 dwSockBuffSize;
    u32 dwRecvBuffSize;
    u32 dwSendBuffSize;
    u32 dwVideoRtpPackBuffSize;
    u32 dwAudioRtpPackBuffSize;
    u32 dwCPUAffinity;              //����DataDispatch�̰߳�CPU��0��ڼ����ˣ��������CPU�����򲻰�
    BOOL32 bIsDefenseCrossStreamEnable;//�Ƿ�����������������
    PFAlarmCallBack pfAlarmCallBack;
	PFMsgCallBack   pfMsgCallBack;
};

API u32 dsInit(const TDsInitInfo &tInfo);

API u32 dsSetIsDefenseCrossStreamEnable(const BOOL32 bVar);

/************************************************************
 * ��������dsDestroy
 *
 * ���ܣ� ��ֹDataDispatch������
 *
 * ʵ��˵����
 * һ��ɾ��һ���û���ֱ�������û���ɾ����������Դ��ȫ�ͷš�
 *
 * ����˵����
 * 
 * 
 * ����ֵ��
 ************************************************************/
API void dsDestroy( );

/************************************************************
 * ��������dsAddDump
 *
 * ���ܣ� ����Dump����
 * DUMP���򣺴�ָ����ַ���յ����ݰ�����ת����
 * ���һ�����յ�ַֻ��DUMP��������յ����ݰ�����ת����һ�����ڱ���ϵͳ���ʹ����ַӦ��
 * �����Dump��ַ���������ת������������������ת����DUMPʧЧ
 * Dump����ᱻ���������û��ֶ�ɾ��
 *
 * ʵ��˵����
 * DUMP��������������Σ���Ϊ�������������ǿ��Բ���ģ�
 * ����������
 * �ظ�������DUMP����ᱻ����
 *
 * ����˵����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP����������ж��IP�����Էֱ�ָ�������Ϊ0����ʾָ��ANYADDR
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsAddDump(u32 dwRecvIP, u16 wRecvPort);

/************************************************************
 * ��������dsRemoveDump
 *
 * ���ܣ� ɾ��Dump����
 *
 * ʵ��˵����
 * 
 * ����˵����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP��ע����dsAddDump��Ӧ
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsRemoveDump(u32 dwRecvIP, u16 wRecvPort);

/************************************************************
 * ��������dsAddP2P
 * 
 * ���ܣ� ���Ӷ��һ��ת�����򣬶���ƽ̨ҵ�����P2P��������
 * 
 * ʵ��˵����
 *
 * ����˵����
 * @param tSsnID        - �����ĻỰ��ʶ
 * @param eSwithType    - �������ͣ�DS��Ҫ������������RTCP�ش�����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP��ͬʱҲ������ӿ�
 * @param wRecvPort     - ����Port�����������ݰ���Ŀ��Port
 * @param dwSrcIP       - �������ݰ���ԴIP
 * @param wSrcPort      - �������ݰ���ԴPort
 * @param dwSendtoIP    - ת��Ŀ��IP
 * @param wSendtoPort   - ת��Ŀ��Port
 * @param dwSendBindIP  - ���Ͱ�IP������RawSocketαװ��Ϊ0��ʾ��αװ
 * @param wSendBindPort - ���Ͱ�Port��0��ʾ��αװ
 - P2P���͵�RTCP���ܴ��ڹ���ϲ�����P2P��RTCPRule�����޷�����RTCP���󣬣�������SrcAddr��
 - ������Ҫ��¼RTCP�����Դ��ַ�������ж�RTCP������������ĸ�Ŀ�Ķ�
 - ���RTP����ʱ�����Բ���д�õ�ַ
 * @param dwRtcpSrcIP   - Ŀ�Ķ˷���RTCP����ĵ�ַ������CU�ñ���IP + 60129����Video RTCP����
 * @param wRtcpSrcPort  - ��ַ��Ҫ�Ǵ�NAT֮��ĵ�ַ��������(M2P)������ַƥ��
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsAddP2P( TSwitchSsnID tSsnID,
                    ESwitchType eSwithType,
                    u32  dwRecvIP,
                    u16  wRecvPort,
                    u32  dwSrcIP ,
                    u16  wSrcPort,
                    u32  dwSendtoIP,
                    u16  wSendtoPort,
                    u32  dwSendBindIP,
                    u16  wSendBindPort,
                    u32  dwRtcpSrcIP,
                    u16  wRtcpSrcPort);

/************************************************************
 * ��������dsRemoveP2P
 *
 * ���ܣ� ɾ��ָ���Ķ��һ����
 *
 * ʵ��˵����
 * 
 * ����˵����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 * @param dwSendtoIP    - ת��Ŀ��IP
 * @param wSendtoPort   - ת��Ŀ��Port
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsRemoveP2P( TSwitchSsnID tSsnID, ESwitchType eSwitchType );

/************************************************************
 * ��������dsAddM2P
 *
 * ���ܣ� ���Ӱ�Դת���Ĺ�����ƽ̨ҵ����M2P������Ҫƥ��Դ��ַ
 * 
 * ��Դת����ʹ�ô˹���ʱ���������ݰ���Դ��ַ����������е�Դ
 * ��ַ��ȡ�ÿ��ת��ʱ����Ҫ�ȸ������ݰ��е�Դ��ַ����ת������
 * ����Ҳ�������ʹ��DUMP����
 * ע�⣺�ýӿ�֧�ֶ�㵽һ�㡣���dwSrcIP��wSrcPort��Ϊ�㣬
 * ������ȫ��ͬ��dsAddP2P
 *
 * ʵ��˵����
 *
 * ����˵����
 * @param tSsnID        - �����ĻỰ��ʶ
 * @param eSwithType    - �������ͣ�DS��Ҫ������������RTCP�ش�����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP��ͬʱҲ������ӿ�
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 * @param dwSrcIP       - �������ݰ���ԴIP
 * @param wSrcPort      - �������ݰ���ԴPort
 * @param dwSendtoIP    - ת��Ŀ��IP
 * @param wSendtoPort   - ת��Ŀ��Port
 * @param dwSendBindIP  - ���Ͱ�IP������RawSocketαװ��Ϊ0��ʾ��αװ
 * @param wSendBindPort - ���Ͱ�Port��0��ʾ��αװ
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/      
API u32 dsAddM2P( TSwitchSsnID tSsnID,
                  ESwitchType eSwitchType,
                  u32  dwRecvIP,
                  u16  wRecvPort,
                  u32  dwSrcIP ,
                  u16  wSrcPort,
                  u32  dwSendtoIP,
                  u16  wSendtoPort,
                  u32  dwSendBindIP,
                  u16  wSendBindPort);

/************************************************************
 * ��������dsRemoveM2P
 * 
 * ���ܣ� ɾ��ָ���İ�Դת���Ĺ���
 *
 * ʵ��˵����
 * 
 * ����˵����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 * @param dwSrcIP       - �������ݰ���ԴIP
 * @param wSrcPort      - �������ݰ���ԴPort
 * @param dwSendtoIP    - ת��Ŀ��IP
 * @param wSendtoPort   - ת��Ŀ��Port
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/      
API u32 dsRemoveM2P( TSwitchSsnID tSsnID, ESwitchType eSwitchType );

/************************************************************
 * ��������dsSetRecvProxy
 * 
 * ���ܣ� ΪRecvGrp����һ�����������մ���ģ�����Դ��˿ڵ�ҵ�񳡾�
 * 
 * ʵ��˵����
 *
 * ����˵����
 * @param tSsnID        - �����ĻỰ��ʶ
 * @param eSwithType    - ��������
 * @param dwSrcIP       - �������ݰ���ԴIP
 * @param wSrcPort      - �������ݰ���ԴPort
 * @param dwProxyRecvIP      - Proxy����IP
 * @param wProxyRecvPort     - Proxy����Port
 * @param dwRealRecvIP       - ʵ�ʽ���IP
 * @param wRealRecvPort      - ʵ�ʽ���Port

 - ProxyRecv��ַ���հ���Ȼ��Ͷ�ݸ���ʵ�Ľ����鴦��
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsSetRecvProxy( TSwitchSsnID tSsnID,
                    ESwitchType eSwithType,
                    u32  dwSrcIP ,
                    u16  wSrcPort,
                    u32  dwProxyRecvIP,
                    u16  wProxyRecvPort,
                    u32  dwRealRecvIP,
                    u16  wRealRecvPort
                    );

/************************************************************
 * ��������dsSetFilterFunc
 * 
 * ���ܣ� ���ý��չ��˺���
 *
 * ע�⣺����ָ�����Ϊ�գ���ʾ���ԭ�е����ã�
 * ���⣬��������DataDispatch���߳��е��ô˺����ģ�����������ȫ�ֵģ������̰߳�ȫ
 * ������FilterFunc���޸Ļص�������
 *
 * ʵ��˵����
 * 
 * ����˵����
 * @param dwRecvIP      - ����IP�����������ݰ��ı���IP      //�����ı�����ַ��Ӧ
 * @param wRecvPort     - ����Port�����������ݰ��ı���Port
 * @param ptFunc        - ���˺���ָ�룬���ΪNULL��ʾɾ������
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsSetFilterFunc( u32 dwRecvIP, 
                         u16 wRecvPort, 
                         FilterFunc ptFunc);

//Ŀǰ�ݲ�֧��socks5�û�����
API u32 dsSetUserData(  u32  dwLocalIP,
                        u16  wLocalPort,
                        u32  dwSrcIP ,
                        u16  wSrcPort,
                        u32  dwDstIP,
                        u16  wDstPort,
                        BOOL32 bSend,
                        u8* pbyUserData,
                        s32 nUserLen);

/************************************************************
 * ��������dsRemoveBySsnID
 * 
 * ���ܣ� ɾ��ͬһ�������Ự�����е�ת����������M2P��P2P
 * 
 * ʵ��˵����DataDispatchģ���������ƽ̨����ҵ���ڲ���¼��ÿ����������ĻỰID
 * �ýӿڱ������н�������ɾ��������ͬSsnID�Ľ�������
 * 
 * ����˵����
 * @param tSsnID        - �����ỰID������һ�ν���VideoRTP��VideoRTCP��AudioRTP��AudioRTCP��������ӵ����ͬSsnID
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsRemoveBySsnID( TSwitchSsnID tSsnID );

/************************************************************
 * ��������dsRemoveAll
 * 
 * ���ܣ� ɾ�����е�ת������
 * 
 * ʵ��˵����
 * 
 * ����˵����
 * @param bKeepDump     - �Ƿ������е�dump����Ĭ�ϲ�����
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsRemoveAll( BOOL32 bKeepDump = FALSE );


/************************************************************
 * ��������dsCheckRule
 * 
 * ���ܣ� ������й���
 * 
 * ʵ��˵������������������״̬��DataDispatch�ڲ�ֻ����һ�±�Ƕ��������������澯
 * ��Ҫ�ϲ㶨�ڵ��ýӿ�����ѯ
 * 
 * ����˵����
 * 
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsCheckRule();

/************************************************************
 * ��������dsGetPacketInfo
 * 
 * ���ܣ� ��ȡָ���������շ���ͳ����Ϣ
 * 
 * ʵ��˵����ָ��������SsnID�������䵱ǰ���µ��շ���������Ϣ
 * 
 * ����˵����
 * 
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsGetPacketInfo(u32 dwTaskNO, TSwitchSsnID tSsnID, ESwitchType eType);

/************************************************************
 * ��������dsCreateRtcpPacket
 * 
 * ���ܣ� ����һ��RTCP����
 * 
 * ʵ��˵����Ŀǰֻ֧��SR / RR����ģ�
 * 
 * ����˵����
 * @ [IN]tSsnID RTCP������SwitchSsnID
 * @ [IN]eSwithType �������ͣ�������ST_VIDEO_RTCP ���� ST_AUDIO_RTCP ��ÿ��ֻ��дһ��
 * @ [IN]emRtcpType �����ı��������
 * @ [OUT]abyBuf ���Buf
 * @ [IN]dwBufLen ����Buff��󳤶�
 * ����ֵ��
 * @return ������BUFʵ�ʳ���(�����һ�㲻����8�ֽ�)
 ************************************************************/
API u32 dsCreateRtcpPacket(TSwitchSsnID tSsnID,
                            ESwitchType eSwithType,
                            ERTCPType emRtcpType,   //Ŀǰֻ֧��SR/RR�����
                            u8 *abyBuf, u32 dwBufLen);

API BOOL32 dsIsRtcpPacketCreatedByDataDispatch(u32 *pdwBuf, u32 dwBufLen);

/************************************************************
 * ��������dsSendTo
 * 
 * ���ܣ� ���ϲ��ṩĳ��������׽��ַ��ͽӿڣ������ϲ�ֱ�ӷ���RTCP����ĵȹ���
 * 
 * ʵ��˵�����ϲ�ָ��ĳһ������Ľ�������tSsnID + eSwithType����ָ�����͵�Ŀ�ĵ�ַ����
 *          �ýӿڸ���vtdu��������ԣ����е���Ϣ�����϶���vtdu�ṩ
 * 
 * ����˵�����ԣ���ע��ȫ������IN��������������˵��������IP��������������Port����������
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsSendTo(TSwitchSsnID tSsnID,
                ESwitchType eSwithType,
                u32 dwDstIP, u16 wDstPort,
                u8 *abyBuf, u32 dwBufLen);

/************************************************************
 * ��������dsSetSpecifiedSSRC
 *
 * ���ܣ� ָ��VideoRtp / AudioRtp��SSRC���������Ƿ�ֵ��ر��滻SSRC
 *
 * ʵ��˵�����ڷ���RTP��֮���޸İ�ͷSSRC
 * 
 * ����˵����
 * @param tSsnID        - ������ˮ��
 * @param eSwitchType   - �������ͣ�����ˮ��һ��Ψһȷ��һ����������ֻ����VideoRtp / AudioRtp
 * @param dwSSRC        - ָ����Ŀ��SSRC��������0��FFFFFFFF�ǷǷ�ֵ����������ֵ�����ص��滻����
 *
 * ����ֵ��
 * @return DSOK: ��ʾ�ɹ� ; DSERROR: ��ʾʧ�� ;
 ************************************************************/
API u32 dsSetSpecifiedSSRC( TSwitchSsnID tSsnID, ESwitchType eSwitchType, u32 dwSSRC );

//////////////////////////////////////////////////////////////////////////
//���Խӿ�
/************************************************************
 * ������ dsinfo
 * ���ܣ� ��ʾ���е�ת�����򣬼��������ڼ����Ķ˿� 
 * ���룺
 * �����
 * ���أ�
 ************************************************************/
API void dsinfo();

/************************************************************
 * ������ dsrule
 * ���ܣ� ��ʾ���е�ת�����򣬼��������ڼ����Ķ˿� 
 * ���룺dwSpecifiedSwitchSsnID = 0 ��ӡ���й���
        dwShowSwitchConnect = 1 ��ӡָ���Ĺ���
        �·�ʽ�Ὣͬһ��SwitchSession�Ľ�������һ���ӡ������ֱ��
 * �����
 * ���أ�
 ************************************************************/
API void dsrule(u32 dwSpecifiedSwitchSsnID);

/************************************************************
 * ������ dsruleset
 * ���ܣ� ��ӡĳ��RecvGrp�ϵ�����RuleSet��Ϣ
 * ���룺 
 * �����
 * ���أ�
 ************************************************************/
API void dsruleset(u32 dwLocalPort);

/************************************************************
 * ������ dsrp
 * ���ܣ� ��ӡĳ��recvproxy�ϵ������Ϣ
 * ���룺 
 * �����
 * ���أ�
 ************************************************************/
API void dsrp(u32 dwLocalPort);

/************************************************************
 * ������ dstrace
 * ���ܣ� ���ٴ�ӡĳ������
 * ���룺 
 * �����
 * ���أ�
 ************************************************************/
API void dstrace(u32 dwSwitchSsnID, u32 dwSwitchType);

/************************************************************
 * ������ dstraceoff
 * ���ܣ� �ر����и��ٴ�ӡ
 * ���룺 ĳЩʱ������������trace�Ľ���SsnID���ô��������ֱ�ӹر�����trace
 * �����
 * ���أ�
 ************************************************************/
API void dstraceoff();

/************************************************************
 * ������ dsrtcp
 * ���ܣ� ��ӡĳ��������ַ�ϵ�RTCP����
 * ���룺 
 * �����
 * ���أ�
 ************************************************************/
API void dsrtcp(u32 dwSwitchSsnID, u32 dwSwitchType);

/************************************************************
 * ������ dsrel
 * ���ܣ� ��ӡ���е�TRuleRelevance�б��鿴�Ƿ�����Դй¶
 * ���룺 
 * �����
 * ���أ�
 ************************************************************/
API void dsrel();

/************************************************************
 * ������ dsloglev
 * ���ܣ� ������־��ӡ����
 * ���룺 dwLev ��ӡ����
 * �����
 * ���أ�
 ************************************************************/
API void dsloglev(u32 dwLev);

/************************************************************
 * ������ dsdetaillog
 * ���ܣ� ��INFO�����´�ӡ��ϸ��Ϣ
 * ���룺 bPrint TRUE��ʾ��ӡ
 * �����
 * ���أ�
 ************************************************************/
API void dsdetaillog(u32 bPrint);

/************************************************************
 * ������ dsver
 * ���ܣ� ��ʾDataswitch�İ汾��Ϣ
 * ���룺
 * �����
 * ���أ�
 ************************************************************/      
API void dsver();

/************************************************************
 * ������ dshelp
 * ���ܣ� ��ʾDataswitch���ṩ������İ�����Ϣ
 * ���룺
 * �����
 * ���أ�
 ************************************************************/      
API void dshelp();

/************************************************************
 * ������ dscfg
 * ���ܣ� ��ʾDataswitch�Ļ�������
 * ���룺
 * �����
 * ���أ�
 ************************************************************/      
API void dscfg();

#endif

//end of file
