#ifndef _GB_NA_NAT_AGENT_H_
#define _GB_NA_NAT_AGENT_H_

/*************************************************************************
������
�Ͻ���NatAgent�Ļص������е���NA�ӿڣ��Լ���һЩ�ӳٲ������ᵼ��NA������Punch��ʱ

�޸ļ�¼��
2013.4.24 by ����
1���޸�TInitInfo��ʼ���ӿں��壬�û���TInitInfo��ָ�������ų�IP�б�Ĭ��Ϊ�գ������ǿ�������IP�б�
   ��ԭ�ȳ�ʼ����������IP�б��Ϊ��NA�Լ��ж�����IP�б�������ˢ��
2������NA�߳����ƣ����ڶ�λ��������ĸ��߳�
3���ṩ����IP�����б�ӿ� NAGetOnlineIPList
4���޸�querystun�ص��߼����޸�Ϊ�ص����ʼ�ճɹ������û��ж�Addr�Ƿ���Ч��NA�ṩ�жϽӿڣ�
   ��Ф�������ʹQueryStunʧ�ܣ����ǿ��Լ���Punch
   QueryStun������Ϊ�ǻ�ȡ���õ�ַ�б�����ʧ��

2013.4.26 by ����
1���ع����룬����������STUN�����ɴ�����£��޸�SendTask�޷����ٵ�Bug
2������SendThread/RecvTask�ġ��������ԡ�������CPU����
   ԭ�ȶ���select��ʱʱ��Ϊ20ms����Ҫ�ǿ���Task��3�볬ʱ�������ܹ�Ѹ�ٻص��ϲ�
   �������Task�����Ϊ�գ�û��Ҫ��ô�����ѯ
3����SelfBindģʽ�£�����NA�ڲ�RecvSock�˿ڸ��ã���ֹҵ����ڻص���bindʧ��
   ע�⣺���ݶ˿ڸ��ù���ҵ����socketҲ��Ҫ���ö˿ڸ��ã�������Ȼbindʧ��

2013.5.2 by ����
1��������һ��ͬ����ȡna����ӿڣ�����ҵ����ܹ����̵߳ĵ���NA�ӿڲ����querystun����punch����������ÿ��Ƕ��̻߳ص�������
   ����ҵ����ڵ���NAPunch�󣬽����ŵ���NAGetResult��ͬ���ȴ��󷵻غͻص�һ���Ľ�����ݣ������˶��̱߳��
   ͬ��ģʽ�ͻص�ģʽ���ܲ��棬ͬ��ģʽ�ᵼ��ҵ�������������ͻ���ʹ�ã�����������Ҫ������������󣬲�����ʹ��
2���޸�������־��ӡ����

2013.5.5 by liangli
1����NA�Ļص��̣߳�sendthread�����̶߳�ջ����Ϊ1 MB����ֹ�ص��߳�ջ���
   NRU���ֹ��ڻص������ж���CMonMsg(27K)����ջ������⣬����ҵ��㲻Ҫ�ڻص������д���ҵ��

2013.5.7 by Royan
1�����NAStunServerStat�ӿڣ�����StunServer�Ƿ����ͨ��
2��ʹ��ticks�����ش��ͳ�ʱʱ�䣬ȡ��������ѭ��������ʽ��

2013.5.8 by Royan
1������Req-Rsp-Ack��ʽ����Punch�жϣ�UDP����������֡�
   �յ��κ�һ��Req���ظ�һ��Rsp�������յ���һ��Rspʱ�ظ�Ack���յ�Ack���ظ���
   ��ǰNA�汾������Req/Rsp����ѶԶ�Req��SrcAddr��ΪPunchӦ����������·�������punch�������
   ���絥�����󶨶�IP��172,10����RawSocket���ͿɴSrcAddr=10��DstAddr=172������Ŀ�ĵ�ַ��һ��·����ͨ�����������򲻿ɴ�
   �°汾NAֻ����Rsp/Ack������punch���
2�����һЩ��Ҫ�ĵ��Դ�ӡ��

2013.5.10 by Royan
1��ֻ�󶨶Զ˵�ַ����Punch����RawSocket�Զ���䱾��IP��������ν��Punch����
2������NAQueryMappedAddr & NAPunch�ӿڣ�����UsedIP��
   ���ҵ�񲻴����Լ�ָ��ʹ�õı���IP���������һ����ЧIP��NA�ڲ���ʹ���������߿��õ�IP�б�����
   ���ҵ�����ʹ��ָ���ı���IP���������Ч�ı���IP��ΪUsedIP��NA�ڲ�ʹ�ø�UsedIP������
   ���Թ�����������
   A��192.A & 10.A���� B��192.B & 10.B����ʱ������������������Punch�ɹ�������ȴ���ֲ�ͬ��Punchͨ����
   |10.A  -> 10.B Req  |
   |192.A <- 192.B Req |
   |10.A  <- 10.B Rsp  |
   |192.A -> 192.B Rsp |

2013.5.13 by Royan
1������time_t����ÿ��Punch��ʱ������
2������win/linux�µ�u64��ӡ��win��%I64u��linux��%llu��
3��u64����ͳһΪddw��

2013.5.16 by Royan
1���ḻ��ӡ���������ͺ�״̬��Ϊstr��ӡ��
2������NACmd��ͳһ����������ýӿڡ����NAHelp��ӡ�е�CmdNumʹ�á�ҵ�����ֻ��Ҫע�����������NAHelp & NACmd�����ɡ�
3���ص�ʱ��ӡPunch��ʼʱ�䡣

2013.5.28 by Royan
1��NAInitLibʱ�������ж�StunServerIP�Ƿ���Ч����Ч������IP��ͨstun���λԤ��ΪTRUE����Ч����Ԥ����λΪFALSE����ʾStunServer����ͨ��
2��NAUpdateStunAddrʱ���ж�StunAddr�Ƿ��б仯���ޱ仯ֱ��return NATAGENT_OK���б仯������Stun��ַ��
   �� �����ж�StunServerIP�Ƿ���Ч����Ч������IP��ͨstun���λԤ��ΪTRUE����Ч����Ԥ����λΪFALSE����ʾStunServer����ͨ��
3��127.0.0.1Ҳ��ӵ���ЧIP�жϣ���natagent�϶�127.0.0.1Ϊ��ЧIP��

2013.5.30 by Royan
1������ƽ̨�����������ܵ���vtdu����NAInitLib��ȡ��������IP�����޸�NAInitLibѭ����ȡ����IP��ÿ�μ��10s������ȡ6�Σ���ȡ�ɹ���������ѭ����

2013.7.3
1������CUÿһ��kedaplayer�����װһ��na������һ��na�ͻ��3���ڲ��˿ڡ���ˣ�ԭ�������3500-3599��100���ڲ��˿ڶξͲ�����ʹ���ˣ����ڴ˻����ϣ�
   �Ѷ˿ڶηŴ�3500-4499��1000���˿ڡ�
2���޸�na�Ĵ�ӡ��Stun/Punch��ʼ����������ʱʱ����ӡ��u64ǿת�������ӡ��������⡣
3����stunָ��UsedIPʱ��stun�ص�û�й��˵�����ȥ������IP���ڴˣ�stun�ص������ˣ������ĵ�ַ����Ϊ0.0.0.0:0��

2013.8.12 by Royan
1���޸�2������˿ڡ�
2��linux�¸���epollģʽ��
3������LogLev��emDebug��Debug���Լ�����־��������NADebugLogOn & NADebugLogOff�򿪹ر�Debug������־��

2013.9.10 by Royan
1���޸�STUN̽����Ϊ1���ӣ�ԭ10���ӣ���
2���ڼ������IP��Ϊ0ʱ��STUN̽������Ϊ5�룻��Ϊ0ʱ��STUN̽����Ϊ1���ӡ�

2014.9.29 by Royan
1���޸��ش��������50ms�𣬱���������ֱ����ʱ��
2��stun��ʱ3s��punch��ʱ�ӳ�Ϊ10s��
3���������Χ�ڴ�ӡ��Ϊerror����
4�����ΰ�����ip������

2016.11.9 by Royan
1����������󶨡�


�������⣺
1�������������������ʱ�������STUN��ַֻ������һ�����ε�
   ��Ȼ������һ������querystunһ����ʱ
   ���õ�ǰ��NAT����ʹ����Ӫ�����磩Ӧ�ñ�Ȼ����IP����ĳ��stun��ַ����ͨ
   �������ж�ʱ����ʹquerystun��ʱҲ���Լ���punch
   �¸��汾���Դ���stun��ַ�б�ͬʱstunserver��anyaddr
2����NAInitLib��NAUpdateStunAddr�����Stun��ַΪһ����Ч��ַ������IP��û������StunServerʱ����Ҫ��̽���̳߳�ʱ�Ż�����̽����λΪFALSE��
   ������̺�ʱһ����ʱʱ�䣨3�룩��������������Ż���
**************************************************************************/

/*
�粻������˵�������ӿ������ж˿ڶ�Ϊ��������ڣ�ip��Ϊ�ַ�������
*/

#include "kdvtype.h"

#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif


#ifdef WIN32
#ifdef __cplusplus
#define NATAGENT_API               extern "C" __declspec(dllexport)
#else
#define NATAGENT_API               __declspec(dllexport)
#endif //__cplusplus
#else
#ifdef __cplusplus
#define NATAGENT_API               extern "C" DLL_PUBLIC
#else
#define NATAGENT_API               DLL_PUBLIC
#endif //__cplusplus
#endif

/************************************************************************/
/*          �궨��                                                      */
/************************************************************************/
#define NATAGENT_MAX_IP_LEN            (32)               //IP��󳤶�Ϊ32
#define NATAGENT_MAX_NETSEG            (4)                //���������������һ�����������4��IP
#define NATAGENT_MAX_IP_NUM            (20)               //�������IP����(�ο�OSP����)
#define NATAGENT_MAX_KEY_LEN           (32+1)             //Punch Key��󳤶�Ϊ32���ټ����ս��
#define NATAGENT_MAX_PORT_NUM          (4)                //���֧��һ��4���˿�
#define NATAGENT_MAX_ID_LEN            (128)
/*
ע�⣺����ϵͳ��inet_addr("")�ķ���ֵ��һ�£������벻Ҫ����մ���ΪIP����
Ϊ��NatAgent���ṩ����ؽṹ��ĳ�ʼ������ NASetNatAddrToInvalid
*/
#define NATAGENT_INVALID_IP_STR        ("0.0.0.0")        //��ЧIP��ַ"0.0.0.0"
#define NATAGENT_INVALID_IP_ANY        ((unsigned long)0x00000000)
#define NATAGENT_INVALID_IP_NONE       (0xffffffff)

#define NA_OSP_ETHERNET_ADAPTER_NAME_LENGTH (64)
#define NA_OSP_ETHERNET_ADAPTER_MAC_LENGTH  (6)
#define NA_OSP_ETHERNET_ADAPTER_MAX_IP      (16)
#define NA_OSP_ETHERNET_ADAPTER_MAX_NUM     (32)

//����״̬,��Ӧ��TOSPEthernetAdapterInfo �ṹ��nState�ֶ�
#define NA_OSP_ETHERNET_ADAPTER_UNKNOWN (0)
#define NA_OSP_ETHERNET_ADAPTER_UP      (1)
#define NA_OSP_ETHERNET_ADAPTER_DOWN    (2)

/* ������Ϣ */
typedef struct {
	s8  achName[NA_OSP_ETHERNET_ADAPTER_NAME_LENGTH];/*������*/
	s8  achIp[NATAGENT_MAX_IP_LEN];        /*��ַ*/
	s8  achSubNetMask[NATAGENT_MAX_IP_LEN];/*��������*/
}TNANicInfo;

/* ���������Ϣ */
typedef struct {
	u16 wNum; 											/*��������*/
	TNANicInfo atNicInfo[NA_OSP_ETHERNET_ADAPTER_MAX_NUM];/*������Ϣ*/
}TNANicInfoAll;

/************************************************************************/
/*          ö�ٶ���                                                    */
/************************************************************************/
//ִ�н������
typedef enum
{
    NATAGENT_OK             = 0,        //�����ɹ�
    NATAGENT_FAIL           = 1         //����ʧ��
}ENATAGENT_OPR_RESULT;

//��־��������
typedef enum 
{
	emError                 = 0,        //���󼶱�
	emWarning               = 1,        //���漶��
	emInfo                  = 2,        //��Ϣ����ͨ������
	emDebug                 = 3         //DEBUG����
}EmLogLev;

//�������
typedef enum
{
    emStopped               = 0,        //���û�������ֹ��ʱ
    emOK                    = 1,        //�ɹ�
    emTimeout               = 2,        //��ʱ
    emSysErr                = 3         //�׽��ִ���
}EmResult;

//��������
typedef enum
{
    emSelfBind              = 0,        //��NatAgent���󶨲������˿�
    emDataSwitchBind        = 1         //���û���DataSwitch�����ù��˺���
}EmNetWorkType;

/************************************************************************/
/*          �ṹ��ͻص��ӿڶ���                                         */
/************************************************************************/
//nat��ַ�ṹ
typedef struct tagNatAddr
{
    s8                   m_achIP[NATAGENT_MAX_IP_LEN];      //IP��ַ
    u16                  m_wPort;                           //������Port
}TNatAddr;

//��������ַ��
typedef struct tagNatAddrPair
{
	TNatAddr             m_tLocalAddr;                      //������ַ
	TNatAddr             m_tMappedAddr;                     //������ַ��������ַ
}TNatAddrPair;

typedef enum
{
    NA_KDNA_MODE = 0,
    NA_KDGB_MODE = 1,
} ePunchMode;

typedef struct tagGBIdInfomation
{
    s8                   m_achLocalID[NATAGENT_MAX_ID_LEN];               //����
    s8                   m_achRemoteID[NATAGENT_MAX_ID_LEN];               //����
    s8                   m_achSessionID[NATAGENT_MAX_ID_LEN];		//����
    TNatAddrPair         m_tReq;                                    //���
    TNatAddrPair         m_tRsp;								    //���
} TGBInfo;

//Punch Key
typedef struct tagPunchKey
{
    ePunchMode           m_eMode;                           //
    s8                   m_achKey[NATAGENT_MAX_KEY_LEN];    //Key
    u16                  m_wLen;                            //ʵ�ʳ���
    TGBInfo              m_tGBInfo;                         //����Punchģʽ��ʹ��
}TPunchKey;

/*
Punch��Ӧ��Ϣ�ṹ��
���ǵ���Ҫһ�δ��������ʴ���һ��struct����ʾһ�����ض˿ڶ�Ӧ������Ҫ�Ĵ���Ϣ��
@Param  m_wLocalPort  ���ض˿ڡ�
@Param  m_tPunchKey  key��
@Param  m_wRemoteAddrsNum  Զ�˵�ַ��������Զ����������
@Param  m_ptRemoteAddrList  Զ�˵�ַ�б�
*/
typedef struct tagPunchInfo
{
	u16                  m_wLocalPort;                      //������
	TPunchKey            m_tPunchKey;                       //key
	u16                  m_wRemoteAddrsNum;                 //Զ�˵�ַ��
	TNatAddr            *m_ptRemoteAddrList;                //Զ�˵�ַ�б�
}TPunchInfo;

/*
Stun��ѯ������ַ�����
ÿһ�����ض˿ڶ�Ӧһ�����ؽ����ÿ���������һ����������ַ���б�������ַ�Ķ˿�Ψһ��
@Param  m_wAddrPairNum    ��ַ����Ŀ��
@Param  m_ptAddrPairList  ��ַ���б�
*/
typedef struct tagStunResult
{
	u32                  m_dwAddrPairNum;                    //��ַ����Ŀ
	TNatAddrPair         m_atAddrPair[NATAGENT_MAX_NETSEG];  //��ַ���б�
}TStunResult;

/*
Punch�����
ÿһ��Key��ÿһ�����ض˿ڣ���Ӧһ���ͨ�ĵ�ַ�ԡ�
@Param m_tPunchKey ����key��һ�����ض˿ڶ�Ӧһ��key��
@Param m_tLocalAddr ����ip��port
@Param m_tRemoteAddr Զ��ip��port
m_tLocalAddr��m_tRemoteAddr��m_tPunchKey��Ӧ��ͨ�ĵ�һ�Ե�ַ�ԡ�
*/
typedef struct tagPunchResult
{
	TPunchKey            m_tPunchKey;                       //key
	TNatAddr             m_tLocalAddr;                      //���ص�ַ
	TNatAddr             m_tRemoteAddr;                     //Զ�˵�ַ
}TPunchResult;

/******************************************************************************
����д��־�Ļص�������
@Param emLogLev ��־����
@Param pchLog   ��־���ݡ�
******************************************************************************/
typedef void (*CBLogPrinter)(EmLogLev emLogLev, const s8 *pchLog);

/******************************************************************************
�����ȡ�Զ˵�ַ�ԵĻص�������
@Param emResult         ��ѯ������п��ܳ�ʱ��
@Param dwStunResultNum  stun�ص����������ͬ�ڱ��ض˿��������NATAGENT_MAX_PORT_NUM��
@Param ptStunResultList ����б�
@Param dwSeqNum         ���кţ���֤����һ���ԣ����Ӧ��NAQueryMappedAddr���к���ȡ�
@Param context          �����ġ�
******************************************************************************/
typedef void (*CBQueryMappedAddr)(
	EmResult            emResult,
	u32                 dwStunResultNum,
	TStunResult        *ptStunResultList,
	u32                 dwSeqNum,
	void               *context);

/******************************************************************************
���崦��򶴽���Ļص�������
@Param emResult          Punch�����ʧ��������������Ч��
@Param dwPunchResultNum  Punch�ص����������ͬ�ڴ���Ŀ�����NATAGENT_MAX_PORT_NUM��
@Param ptPunchResultList �򶴽����Ϣ�б�(����)������Ԫ����Ϣ������key����ͨ�ı��غ�Զ�˵�ַ�ԡ�
@Param dwSeqNum          ���кţ���֤����һ���ԣ����Ӧ��NAPunch���к���ȡ�
@Param context           �����ġ�
******************************************************************************/
typedef void (*CBPunchResult)(
	EmResult            emResult,
	u32                 dwPunchResultNum,
	TPunchResult       *ptPunchResultList,
	u32                 dwSeqNum,
	void               *context);

/*���߿��õ�����IP�б�*/
/*
ʵ���㷨��
NatAgent�ڲ����ȡ�������е�ַ�б�Ȼ�����ж���Щ��������
���ų�ָ����IP������ҵ�����õ���������IP�б�
ÿ�ε���NAGetOnlineIPList�Ľ�����ܲ�һ�������߲�Σ������鼴�ü�ȡ
*/
typedef struct tagOnlineIPList
{
    u32                 m_dwOnlineIPNum;                                         //����IP��Ŀ
    s8                  m_achOnlineIP[NATAGENT_MAX_NETSEG][NATAGENT_MAX_IP_LEN]; //����IP�б�
}TOnlineIPList;

/*
��ʼ����Ϣ�ṹ���û�����
1��������IP�б�
�ض�Ӧ�ó�����ҵ����ѱ���ĳЩ��ַ����Ϊ����������ַ
���m_achUnusedIP�б���Ը���NA�ų���Щ��ַ������Ϊ����
NA������stun��punch������ʹ����Щ���Σ���ʹIP����
Ĭ���û����Բ�����дm_achUnusedIP�б�
2��STUN��ַ
��ʹstun��ַ��Ч��NAҲ���Գ�ʼ���ɹ�
��ΪNA����2������ɣ�querystun��punch
�п����û��ھ������ڣ�����NAT��������stun������Ҫ��punch
������Ҫ�޸�ΪSTUN��ַ�б�
3��Stun�ص����
NAQueryMappedAddr�ӿ�һ�������Բ�ѯ4���˿�
��ϱ������4������IP���ټ���LocalAddr��MappedAddr��һ����32��Addr
    //ֻҪ��һ��Addr�յ�STUNServerӦ�𣬻ص����ΪemOK
    //������е�ַ����STUNServer�޷�pingͨ���ص����ΪemTimeout
�����Ѿ��޸�Ϊ������ζ��ɹ������صĶ��Ǳ�������IP�б�
4��Punch�ص����
NAPunch�ӿ�һ������ѯ4���˿ڣ��ֱ�󶨱���4��Addr�����Զ����32����ַ���Զ�Ҳ��4 x 4 x 2������punch��
ֻ�е�����4���˿�ȫ���ɹ������ص����ΪemOK
����ص����ΪemTimeout
5���Ͻ��ڻص������е���NA�ӿڣ��ᵼ������������Lock(g_hCBMutex);��
6�����m_bSyncMode = TRUE��NA���ᴥ���ص����Ὣ�ص����ͨ��NAGetResult�ӿ��׸�ҵ���
*/
typedef struct tagInitInfo
{
    u32                 m_dwUnusedIPNum;               //������IP��Ŀ
    s8                  m_achUnusedIP[NATAGENT_MAX_NETSEG][NATAGENT_MAX_IP_LEN]; //������IP�б�
    TNatAddr            m_tSTUNAddr;                   //Stun Server��ַ�������� 
	CBLogPrinter        m_fLogPrinter;                 //��־�ص��ӿ�
    CBQueryMappedAddr   m_fQueryMappedAddr;            //��ȡ������ַ����ص��ӿ�
    CBPunchResult       m_fPunchResult;                //�򶴽���ص��ӿ�
    BOOL32              m_bSyncMode;                   //ͬ����ȡ�ص����ģʽ
}TInitInfo;

/*
Ϊ�˱����û�ֱ��memset(ptInitInfo, 0, sizeof(TInitInfo))������Ӵ˺���
�ú����������������ڴ棬ֻ�ǰ����û���ʼ���������ݳ�Ա
@Param  ����Ҫ��ʼ���Ľṹ���ַ
@Return ��Ȼ���ظýṹ���ַ
*/
NATAGENT_API TInitInfo* NASetInitInfoToNULL(TInitInfo *ptInitInfo);
/* ͬ�� */
NATAGENT_API TNatAddr* NASetNatAddrToInvalid(TNatAddr *ptNatAddr);

/*
�ж�IP�Ƿ�Ϸ�
�Ƿ�IP�ַ��������������
1, a.b.c.d �κ�һ�γ���255
2��0.0.0.0
3��NULL
*/
NATAGENT_API BOOL32 NAIsIpAddrInvalid(u32 dwIp);
NATAGENT_API BOOL32 NAIsIpStrInvalid(const char *lpIPStr);

/************************************************************************/
/*          �ӿڶ���                                                    */
/************************************************************************/
/*
�жϴ�����հ����������ݰ�����ֵ��DataSwitchģʽ�����˺�����ΪFilter�������հ���
�������������NatAgent���ƴ����淶��Լ��������IP��ַΪ�����򣬶˿�Ϊ����������Ϊ������
����InvalidPacket��ʾ����punch or stun����
����StunPacket��ʾ��stun��������PunchPacket��ʾ��punch��
ע��NAOnAck������ĳ�NAOnRecv�����ǽӿ��޸ĵ�Ӱ�죬��ֻ�ڴ˱�עһ�£�����
*/
NATAGENT_API u32 NAOnAck(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 dwLen);

//NatAgent��ʼ��
NATAGENT_API ENATAGENT_OPR_RESULT NAInitLib(TInitInfo *ptInitInfo);

/******************************************************************************
����Stun Server��ַ��
@Param ptStunAddr      Stun Server��ַ��
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAUpdateStunAddr(TNatAddr *ptStunAddr);

/******************************************************************************
��ȡ�������ε�ַ�б�
�������ε�ַ�б��Ǳ�����ַ�б��У��������ߵ���ЩIP�������ų��û�ָ��������IP
@Param ptOnlineIPList  [OUT]����IP��ַ�б�
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAGetOnlineIPList(TOnlineIPList *ptOnlineIPList);

/******************************************************************************
��ѯ������ַ��
@Param eNetworkType    [IN]�������͡�
@Param wLocalPortNum   [IN]�˿���Ŀ�����ô���NATAGENT_MAX_PORT_NUM��
@Param pwLocalPortList [IN]�˿��б�(����)��Ԫ����Ŀ����wLocalPortNum��
@Param achUsedIP       [IN]ָ������ʹ��IP��IN����
                       ���ҵ�񲻴����Լ�ָ��ʹ�õı���IP���������һ����ЧIP��NA�ڲ���ʹ���������߿��õ�IP�б�����
                       ���ҵ�����ʹ��ָ���ı���IP���������Ч�ı���IP��ΪUsedIP��NA�ڲ�ʹ�ø�UsedIP������
@Param dwSeqNum        [OUT]���кţ���֤����һ���ԣ����Ӧ��CBQueryMappedAddr���к���ȡ�
@Param context         [IN]������ָ�롣
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAQueryMappedAddr(
	EmNetWorkType        eNetworkType,
	u32                  dwLocalPortNum,
	const u16           *pwLocalPortList,
	s8                   achUsedIP[NATAGENT_MAX_IP_LEN],
	u32                 *pdwSeqNum,
	void                *context);

/******************************************************************************
�򶴡�
@Param eNetworkType    [IN]�������͡�
@Param dwPunchInfoNum  [IN]Punch��Ϣ�������ڱ��ض˿��������NATAGENT_MAX_PORT_NUM��
@Param ptPunchInfoList [IN]Punch��Ϣ�б�(����)��Ԫ����Ŀ����dwPunchInfoNum��
                       ÿһ������Ϣ�����������ض˿ڣ�Զ�˵������ַ��������ǰ�����ں󣩡�
@Param achUsedIP       [IN]ָ������ʹ��IP��IN����
					   ���ҵ�񲻴����Լ�ָ��ʹ�õı���IP���������һ����ЧIP��NA�ڲ���ʹ���������߿��õ�IP�б�����
					   ���ҵ�����ʹ��ָ���ı���IP���������Ч�ı���IP��ΪUsedIP��NA�ڲ�ʹ�ø�UsedIP������
@Param dwSeqNum        [OUT]���кţ���֤����һ���ԣ����Ӧ��CBQueryMappedAddr���к���ȡ�
@Param context         [IN]������ָ�롣
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAPunch(
	EmNetWorkType        eNetworkType,
	u32                  dwPunchInfoNum,
	const TPunchInfo    *ptPunchInfoList,
	s8                   achUsedIP[NATAGENT_MAX_IP_LEN],
	u32                 *pdwSeqNum,
	void                *context);

/******************************************************************************
����ͬ����ȡ�ص��������
@Param dwSeqNum     [IN]�ص������к�
@Param pemType      [OUT]�ص��Ĳ�������
@Param pemResult    [OUT]�ص����
@Param pdwResultNum [OUT]�������
@Param ptResultList [OUT]�ص���Ϣ���飨ע�⣬���ݾ���Ӧ�ó�����������TStunResult *���� TPunchResult *��
@Param ppContent    [OUT]�ص�������ָ�룬ע�⣬����Ĳ�����ָ���ָ��
@Param dwTimeout    [IN]�ӿڳ�ʱʱ�䣬��λms���Ƽ� 3500
����ֵ������ɹ���ȡ�ص���Ϣ����NATAGENT_OK�����򷵻�NATAGENT_FAIL
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAGetResult(u32 dwSeqNum, EmResult *pemResult, u32 *pdwResultNum, 
    void *ptResultList, void **ppContent, u32 dwTimeout);

/******************************************************************************
����UDP�����
@Param ptLocalNatAddr      [IN]UDP����Ӧ�ı��ص�ַ��
@Param ptRemoteNatAddr     [IN]UDP����Ӧ��Զ�˵�ַ��
@Param dwKeepAliveInterval [IN]����������λ���룬�Ƽ� 20��
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAStartKeepAlive(TPunchKey tPunchKey,
	const TNatAddr      *ptLocalNatAddr,
	const TNatAddr      *ptRemoteNatAddr,
	u32                  dwKeepAliveInterval);

/******************************************************************************
ֹͣUDP�����
@Param ptLocalNatAddr  [IN]UDP����Ӧ�ı��ص�ַ��
@Param ptRemoteNatAddr [IN]UDP����Ӧ��Զ�˵�ַ��
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAStopKeepAlive(
	const TNatAddr      *ptLocalNatAddr,
	const TNatAddr      *ptRemoteNatAddr);

//�˳�NatAgentLib
NATAGENT_API ENATAGENT_OPR_RESULT NAQuitLib();

//����StunServer��ͨ״̬��TRUE��ͨ��FALSE����ͨ��
NATAGENT_API BOOL32 NAStunServerStat();

/************************************************************************/
/*          ���Խӿڶ���                                                 */
/* NatAgent�ⱻ�趨��������OSP�⣬���������Ҫ��telnetִ�У����ϲ��û�ע��  */
/* ����ע��OSP����ʱ���������ַ�������Сд                                 */
/************************************************************************/
//������Ϣ
NATAGENT_API void NAHelp();

//��ӡȫ����Ϣ
NATAGENT_API void NAGlobalInfo();

//��ӡ�ص���������
NATAGENT_API void NACBList();

//��ӡ������������
NATAGENT_API void NARecvList();

//��ӡ������������
NATAGENT_API void NASendList();

//��ӡ������������
NATAGENT_API void NAKAList();

//��Debug������־
NATAGENT_API void NADebugLogOn();

//�ر�Debug������־
NATAGENT_API void NADebugLogOff();

//NA������û��ܽӿ�
NATAGENT_API void NACmd(u16 wCmdNum);

//for test
NATAGENT_API u32 NAGetKAListSize();
NATAGENT_API u32 NAGetCBListSize();
NATAGENT_API u32 NAGetSendListSize();
NATAGENT_API u32 NAGetRecvListSize();

NATAGENT_API BOOL32 NAGetOnlineNicList(TNANicInfoAll* ptOnlineNicList);
NATAGENT_API BOOL32 NAGetAllNicList(TNANicInfoAll* ptAllNicList);

NATAGENT_API u32 ConstructGBPunchReq(u8 byPunchType,u8* pbyPunchReq, const TPunchKey *ptPunchKey, s8* pchLocalIp, u16 wLocalPort, s8* pchRemoteIp, u16 wRemotePort);


#endif /*_GB_NA_NAT_AGENT_H_*/


