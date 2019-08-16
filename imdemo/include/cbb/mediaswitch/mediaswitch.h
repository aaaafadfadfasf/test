/** 
* @file         mediaswitch.h
* @brief        ת��ģ��
* @details      
* @author       ��Ф��
* @date         2016/07/19
* @version      v1.0
* @par Copyright (c):
*    kedacom
* @par History:
*   version:V1.0  Copyright(C) 2003-2016 KDC, All rights reserved.
*/

#ifndef _MEDIA_SWITCH_H_
#define _MEDIA_SWITCH_H_

#include "kdvtype.h"
#include "kdcbbmedia.h"

#ifdef _USE_OSPSMALL_
#include "osp_small.h"
#else
//#include "osp.h"
#endif

//mediaswitch.h��ͷ�ļ��в�Ҫ���__MRTC__�꣬mrtc�����ݽṹ���mrtc�汾���ã���mrtc�汾������mrtc��c�ļ��еĴ���
#ifndef _LINUX_
//#if _MSC_VER <= 1200
#include "WS2tcpip.h"

#else

#include <arpa/inet.h>

// #include <in6addr.h>
// #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef s32 MSRESULT; ///< ������

/** ������ */
#define  MSRESULT_NO_ERROR                            (s32)(0)				///< �ɹ�����ֵ
#define  MSRESULT_ERROR_BASE                          (s32)(20000)
#define  MSRESULT_ERROR_PARAM                         (MSRESULT_ERROR_BASE+1)	///< ��������
#define  MSRESULT_ERROR_NOCREATE                      (MSRESULT_ERROR_BASE+2)	///< δ����
#define  MSRESULT_ERROR_MAXCREATE                     (MSRESULT_ERROR_BASE+3)	///< �ﵽ��󴴽���Ŀ
#define  MSRESULT_ERROR_MEM                           (MSRESULT_ERROR_BASE+4)	///< �ڴ����
#define  MSRESULT_ERROR_UNSUPPORT                     (MSRESULT_ERROR_BASE+5)	///< ��֧��
#define  MSRESULT_ERROR_UNKNOWN                       (MSRESULT_ERROR_BASE+6)	///< δ֪����
#define  MSRESULT_ERROR_SOCKET                        (MSRESULT_ERROR_BASE+7)	///< �׽��ִ���
#define  MSRESULT_ERROR_CONNECTTIMEOUT                (MSRESULT_ERROR_BASE+8)	///< ���ӳ�ʱ
#define  MSRESULT_ERROR_CONNECTIONCLOSE               (MSRESULT_ERROR_BASE+9)	///< �Զ˹ر�����
#define  MSRESULT_ERROR_AGAIN                         (MSRESULT_ERROR_BASE+10)	///< ��Ҫ���·���
#define  MSRESULT_ERROR_CONNECT                       (MSRESULT_ERROR_BASE+11)	///< connectʧ��
#define  MSRESULT_ERROR_LISTEN                        (MSRESULT_ERROR_BASE+12)	///< ����ʧ��
#define  MSRESULT_ERROR_TASKCREATE                    (MSRESULT_ERROR_BASE+13)	///< �̴߳�������
#define  MSRESULT_ERROR_NOTRTP                        (MSRESULT_ERROR_BASE+14)	///< ��RTP��
#define  MSRESULT_ERROR_SEMCREATE                     (MSRESULT_ERROR_BASE+15)	///< ������ʧ��
#define  MSRESULT_ERROR_NOINIT                        (MSRESULT_ERROR_BASE+16)	///< û�г�ʼ��
#define  MSRESULT_ERROR_BINDSOCKET                    (MSRESULT_ERROR_BASE+17)	///< �󶨶˿�ʧ��
#define  MSRESULT_ERROR_FRAMEFULL                     (MSRESULT_ERROR_BASE+18)	///< ֡�������
#define  MSRESULT_ERROR_PAYLOAD                       (MSRESULT_ERROR_BASE+19)	///< payload����
#define  MSRESULT_ERROR_FRAMELIST                     (MSRESULT_ERROR_BASE+20)	///< ֡�������
#define  MSRESULT_ERROR_NOSTREAM                      (MSRESULT_ERROR_BASE+21)	///< ������
#define  MSRESULT_ERROR_GETAGAIN                      (MSRESULT_ERROR_BASE+22)	///< ����̫�죬���Ժ��ٴ�get����
#define  MSRESULT_ERROR_READSLOW                      (MSRESULT_ERROR_BASE+23)	///< ����̫���������»�ȡλ����get����
#define  MSRESULT_ERROR_DATA                          (MSRESULT_ERROR_BASE+24)	///< ���ݴ���
#define  MSRESULT_ERROR_NOKEYFRAME                    (MSRESULT_ERROR_BASE+25)	///< û�йؼ�֡
#define  MSRESULT_ERROR_NOTSTART                      (MSRESULT_ERROR_BASE+26)	///< MSOutû��start
#define  MSRESULT_ERROR_FRAMELISTFULL                 (MSRESULT_ERROR_BASE+27)	///< ֡������
#define  MSRESULT_ERROR_CREATEAGAIN                   (MSRESULT_ERROR_BASE+28)	///< ���߳�ͬʱ����
#define  MSRESULT_ERROR_SPACEFULL                     (MSRESULT_ERROR_BASE+29)	///< ɢ�б�ռ�����
#define  MSRESULT_ERROR_FRAME_LIST_FULL               (MSRESULT_ERROR_BASE+30)	///< ֡������,����������һ��.
#define  MSRESULT_ERROR_TCP_SEND                      (MSRESULT_ERROR_BASE+31)	///< TCP����ʧ��
#define  MSRESULT_ERROR_HASH_ERR                      (MSRESULT_ERROR_BASE+32)	///< ֡������,����������һ��.
#define  MSRESULT_ERROR_BIND_FAILED                   (MSRESULT_ERROR_BASE+33)	///< ��ʧ��
#define  MSRESULT_ERROR_INPROCESS                     (MSRESULT_ERROR_BASE+34)	///< tcp�������ڴ���
#define  MSRESULT_ERROR_ACCEPTTIMEOUT                 (MSRESULT_ERROR_BASE+35)	///< �ȴ�socket���ӳ�ʱ.
#define  MSRESULT_ERROR_BIND_INUSE                    (MSRESULT_ERROR_BASE+36)	///< �ظ���
#define  MSRESULT_ERROR_UNSUPPORT_PSSTREAMID          (MSRESULT_ERROR_BASE+37)	///< ��֧�ֵ�PS���
#define  MSRESULT_ERROR_SEM_TIMEOUT                   (MSRESULT_ERROR_BASE+38)	///< �ź�����ʱ

///MAXMSINLISTNUM MAXMSOUTLISTNUM MAXMSPIPLINELISTNUM ƽ̨��10000��������ƷΪ1000���Ѿ��������벻Ҫ��ʹ��
#if 1
#define MAXMSINLISTNUM       (u32)(10000)
#define MAXMSOUTLISTNUM      (u32)(10000)
#define MAXMSPIPLINELISTNUM  (u32)(10000)
#endif

#define  MS_MAXENCNAMELEN (u32)(8)	///< ����������󳤶�
#define  MS_MAXDESCRIPEINFOLEN (u32)(64)	///< ������Ϣ��󳤶�
#define  MS_MAX_VKEK_LEN            (u32)(32)    ///< VKEK�ַ�������
#define  MS_MAX_VKEK_VERSION_LEN    (u32)(32)    ///< ������Ϣ��󳤶�
#define  MS_MAX_BIND_CORE_NUM    (u32)(256)    ///< ���֧�ְ󶨵ĺ˸���
#define  MS_MAX_STREAM_NUM_IN_PROGRAM    (u32)(255)    ///< PS֧������������

#ifdef _LINUX_
	#define IN6_ADDR   struct in6_addr
	#define IN_ADDR    struct in_addr
#endif

//�ṩ��̬����Ҫ
// for Microsoft c++
#ifdef _MSC_VER


#ifdef __cplusplus
#define M_API  extern "C" __declspec(dllexport)
#else
#define M_API  __declspec(dllexport)
#endif
#define MEDIASWITCH_API    __declspec(dllexport)

// for gcc
#else
#ifdef __cplusplus
#define M_API extern "C"
#else
#define M_API
#endif
#define MEDIASWITCH_API

#endif    // _MSC_VER

/** ʱ�������ģʽ */
typedef enum tagMSTSCalMode
{
	E_MS_Ts_Local_Base = 0, ///< Ĭ��ģʽ,���ػ�׼������
	E_MS_Ts_Custom_Base,    ///< �Զ����׼,������
	E_MS_Ts_Rtcp_Ntp_Base_Priority,			///< RTCP���Ȼ�׼.(���rtcp��û��ntp,����E_MS_Ts_Local_Baseִ��)
    E_MS_Ts_Max,
}EMSTSCalMode;

/** ʱ�����������ѡ�� */
typedef struct tagMSTSCalParam
{
	EMSTSCalMode m_eMode;
	u64 m_qwBaseUtc; ///<��׼utcʱ��, E_MS_TS_CUSTOM_BASE��Ҫ���øĲ���,����ģʽ����Ҫ�����������.
} TMSTSCalParam;

/** �ӽ���ģʽ */
typedef enum tagMSCipherMode
{
    MEDIANET_KDC_EXT_ENCRYPT,   ///<����ģʽ
    MEDIANET_KDC_EXT_DECRYPT,   ///<����ģʽ
    MEDIANET_KDC_EXT_INVALID,   ///<��Чģʽ
} EMSCipherMode;

/** MediaCipher����ʱ���� */
typedef struct tagMSCipherParam
{
    EMSCipherMode m_eMode;          ///<ģʽ
    u32			m_dwEngineIdMd;     ///<ɢ���㷨������engine id
    u32			m_dwEngineIdCipher; ///<�ԳƼ����㷨������engine id
    u32			m_dwEngineIdPkey;   ///<�ǶԳƼ����㷨������engine id
    u32			m_dwEngineIdRand;   ///<�����������engine id
} TMSCipherParam;

/* VKEK�ṹ�嶨�� */
typedef struct tagMSVkekInfo
{
    u8  m_abyVkek[MS_MAX_VKEK_LEN+1];   ///vkek����/�����ַ���
    u32 m_dwVkekLen;                    ///m_abyVkek����Ч�ַ�������	
    u8  m_abyVesion[MS_MAX_VKEK_VERSION_LEN+1]; ///vkek�汾���ַ���
    u32 m_dwVesionLen;                  ///m_abyVesion����Ч�ַ�������
    TMSCipherParam m_tCipherParam;      ///����������
} TMSVkekInfo;

/** IP��ַ��Ϣ */
typedef struct tagMSNetAddr
{
	u16 m_wIPType;           ///< IP���� must be AF_INET or AF_INET6
	union
	{
		IN_ADDR m_tV4Addr;   ///< V4IP
		IN6_ADDR m_tV6Addr;  ///< V6IP
	};
}TMSNetAddr;

///��չ��������
typedef enum tagMSDataExtType
{
    MS_DATA_EXT_NTPTIME = 1,  ///<ntpʱ��
}EMSDataExtType;

///��չ����
typedef struct tagMSDataExt
{
    EMSDataExtType m_eDataType; ///<��չ��������
    union
    {
        u64  m_qwNtpTime;       ///<NTPʱ���
    };
}TMSDataExt;

/** ����ģʽ */
typedef enum tagMSBindMode
{
    E_MS_Bind_Connect_NotSharePort = 0, ///< Ĭ��ģʽ,������������,��������in��out����˿�.
    E_MS_Bind_Listen_NotSharePort, ///< ��������ģʽ,��������in��out����˿�.
    E_MS_Bind_Listen_SharePort, ///< ��������ģʽ,ʹ�ù���Ķ˿�.
}EMSBindMode;

/** tcp�󶨲��� */
typedef struct tagMSTcpBindParam
{
    TMSNetAddr m_tLocal;    ///< �󶨵�IP��ַ.
    EMSBindMode m_eBindMode;  ///< ��ģʽ
} TMSTcpBindParam;

/** ������� */
typedef struct tagMSRtpSession
{
	TMSNetAddr    m_tRTPAddr;  ///< RTP��ַ(������)
	u16           m_wRTPPort;  ///< RTP�˿�(������)
	TMSNetAddr    m_tRTCPAddr; ///< RTCP��ַ(������)
	u16           m_wRTCPPort; ///< RTCP�˿�(������)
}TMSRtpSession;

/** ���������Լ�payload */
typedef struct tagMSEncNameAndPayload
{
	s8       m_byEncName[MS_MAXENCNAMELEN]; ///< ��������
	u8       m_byPayload;	///< rtp�е�payloadֵ
}TMSEncNameAndPayload;

/** rtcp����������*/
typedef enum tagMSRtcpAliveChkType
{
    E_MS_RtcpAliveChkStop = 0,  ///< �����
    E_MS_RtcpAliveChkStart,     ///< ������⣬����1. ��MSIN�ڳ�ʱʱ����û���յ�SR�����ж�Ϊnot alive��
                                ///<����2. ��MSOUT�ڳ�ʱʱ����û���յ�RR�����ж�Ϊnot alive��
    E_MS_RtcpAliveChkAuto,      ///< �Զ���⣬����1.MSIN�յ�SR���������,�ڳ�ʱʱ����û�����յ�SR�����ж�Ϊnot alive��
                                ///����2. MSOUT�յ�RR���������,�ڳ�ʱʱ����û�����յ�RR�����ж�Ϊnot alive��
}EMSRtcpAliveChkType;

/** rtcp ����������Ͽ�ʱ�� */
typedef struct tagMSRtcpAlive 
{
    EMSRtcpAliveChkType m_eChkType; ///<��������,Ĭ��E_MS_RtcpAliveChkAuto
	u32    m_dwDisconnectTime; ///< �೤ʱ��δ�յ����������Ϊ��������λ���룩
}TMSRtcpAlive;

/** ����sps�Լ�pps��Ϣ */
typedef struct tagMSSpsPpsInfo 
{
	u8*      m_pbySPS;     ///< SPS
	u32      m_dwSPSSize;  ///< SPS����
	u8*      m_pbyPPS;     ///< PPS
	u32      m_dwPPSSize;  ///< PPS����
}TMSSpsPpsInfo;

/** ���ն��ش����� */
typedef struct tagMSRSParam 
{
	u32 m_dwFirstRSCheckTime;  ///< ��һ���ش�����
	u32 m_dwSecondRSCheckTime; ///< �ڶ����ش�����
	u32 m_dwThirdRSCheckTime;  ///< �������ش�����
	u32 m_dwRejectRSCheckTime; ///< ����ʱ���
}TMSRSParam;

/** ��������PS ES */
typedef enum tagMSType
{
	E_MS_UnknowType = 0, ///< δ֪����
	E_MS_EsType,         ///< ES����
	E_MS_PsType          ///< PS����
}EMSType;

/** �������ʱ��Ҫαװ��ip�Լ��˿� */
typedef struct tagMSMapAddr
{
	TMSNetAddr    m_tAddr; ///< RTP��ַ(������)
	u16           m_wPort; ///< RTP�˿�(������)
}TMSMapAddr;

/** �������ʱ��Ҫαװ��ip�Լ��˿� */
typedef struct tagMSMap
{
    TMSMapAddr m_tRtpMap; ///<ip���߶˿�Ϊ0��ʾ��αװ
    TMSMapAddr m_tRtcpMap;///<ip���߶˿�Ϊ0��ʾ��αװ
}TMSMap;

/** ���payload����InterleavedId  */
typedef struct tagMSChangePayload
{
	s32          m_nInterleavedId; ///<  rtsp tcpģʽʹ��InterleavedId������ģʽ����Ϊ0
	u8           m_byPayload;	   ///< ����payload
}TMSChangePayload;

/** ���ն�ͳ����Ϣ  */
typedef struct tagMSRcvStatistics
{
    u32    m_dwPackNum;       ///< �ѽ��յİ���
    u32    m_dwPackLose;      ///< �G����
    u32    m_dwPackIndexError;///< ��������
    u32    m_dwFrameNum;      ///< �ѽ��յ�֡��
}TMSRcvStatistics;

/** ���� */
typedef struct tagStreamNumber
{
	u32 m_dwAudioStreamNum; ///<��Ƶ����
	u32 m_dwVideoStreamNum; ///<��Ƶ����
}TMSStreamNumber;

/** ����InId/OutId������Ϣ  
* Ŀ��idΪInidʱ��
*	��ʽ��ip + ��nͨ��   
*	ע�⣺����ʱip���nvr ip
*	���磺172.16.64.67 ��2ͨ��
* Ŀ��idΪOutidʱ��
*	��Ҫ�ǹ���������webԤ��+ip/guiԤ��/gui����/gui¼��  (����һ��)
*	ע�⣺webԤ��ʱ��ӶԷ�ip
*/
typedef struct tagTMSDescripeInfo
{
	s8   m_szMSDescripeInfo[MS_MAXDESCRIPEINFOLEN];    ///< ��̵�������Ϣ

}TMSDescripeInfo;

typedef struct tagMSVideoInFo  
{
	u16 m_wVideoWidth;
	u16 m_wVideoHeight;
}TMSVideoInFo;

typedef enum tagMSMrtcChannelMode
{
    kInvalid = 0x0, ///< ��Ч�ŵ�
    kAudio = 0x01,  ///< ��Ƶ�ŵ�
    kVideo = 0x02,  ///< ��Ƶ�ŵ�
}TMSMrtcChannelMode;

///mrtc����bufģʽ
typedef enum tagMSMrtcSndBufMode
{
    E_MS_MRTC_SndBufInvalid = 0x0, ///< δ���ã�ʹ��Ĭ������
    E_MS_MRTC_ExternalSndBuf = 0x01,  ///< �ⲿ���壬ʹ��netpacket����
    E_MS_MRTC_InternalSndBuf = 0x02,  ///< �ڲ����壬ʹ��mrtc���壬��������ڴ�
}EMSMrtcSndBufMode;

///ͨ����������
typedef struct tagMSMrtcChnCreateCfg
{
    TMSMrtcChannelMode  m_eChnMode; ///<ͨ������Ƶģʽ
    EMSMrtcSndBufMode   m_eSndBufMode; ///<ͨ��bufģʽ��Ĭ��ʹ��E_MS_MRTC_ExternalSndBuf
}TMSMrtcChnCreateCfg;

/** MRTC���Ͷ�����       */
typedef struct tagMSMrtcSenderConfig
{
    u8 m_byRedEnable;   ///< 0: ��ֹ��Ƶ�౶����, >0:������Ƶ�౶����, Ĭ��ֵ0 (��Ƶ����)
    u8 m_byRedPayload;  ///< >0: ��Ƶ: �౶����ʹ��ʱ, ý���ʹ�õ�ý������,Ĭ��ֵ125; ��Ƶ: FECʹ��ʱ,ý���ʹ�õ�ý������, Ĭ��ֵ126
    u8 m_byFecEnable;   ///< 0: ��ֹ��ƵFEC, >0: ������ƵFEC����, Ĭ��ֵ0 (��Ƶ����)
    u8 m_byFecPayload;  ///< >0: FECʹ��ʱ, FEC��ʹ�õ�ý������, Ĭ��ֵ127 (��Ƶ����)
    u8 m_byFecMode;     ///< FECʹ��ʱ, FEC�����ɹ����ģʽ, 0: ͻ����������, >0: �����������, Ĭ��ֵ0 (��Ƶ����)
    u8 m_byFecRate;     ///< FECʹ��ʱ, FEC�������ɱ���, ȡֵ1~255, ����FEC���ĸ���=(ý����ĸ��� * m_byFecRate) >> 8, ��cpuƿ��, ��������̫��, Ĭ��ֵ25(FEC��ռ��10%) (��Ƶ����)
    u8 m_byFecMaxFrames; ///< >0: FECʹ��ʱ, ��ʼ����FEC�������֡��, Ĭ��ֵ1 (��Ƶ����)
    u8 m_byCCEnable;    ///< 0: ��ֹӵ�����ƹ���, >0: ����ӵ�����ƹ���, ���������״̬, ����֡��/����, Ĭ��ֵ0
    u32 m_dwCCStartRate; ///< >0: ��ʼ��������, Ĭ��ֵ4Mbps
    u32 m_dwCCMinRate;  ///< >0: ��С��������, ӵ�����Ƶ��������ʲ�����ڸ�ֵ, Ĭ��ֵ10kbps
    u32 m_dwCCMaxRate;  ///< >0: ���������, ӵ�����Ƶ��������ʲ�����ڸ�ֵ, Ĭ��ֵ100Mbps
    u8 m_byPaceEnable;  ///< 0: ��ֹƽ�����͹���, >0: ����ƽ�����͹���, �����ش�����, ���������������, �����Ķ����CPU, Ĭ��ֵ0
    u8 m_byRtcpEnable;  ///< �Ƿ�������RTCP��, >0: ����, 0: ������, Ĭ��ֵ1
    u8 m_byRtxEnable;   ///< 0: ��ֹ�����ش�, >0: �������ش�, Ĭ��ֵ1
    u8 m_byRtxMode;     ///< �ش�����ʽ, 0: �ش�����ԭý�����ͬ, Kedaʹ�ø�ģʽ, >0: �ش�������RFC4588, ��ԭý�����ͬ, �ж�����PayloadType��SSRC, Ĭ��ֵ0
    u8 m_byRtxPayload;  ///< >0: @m_byRtxMode>0ʱ��Ч, RFC4588�ش�����ý������, Ĭ��ֵ0
    u32 m_dwRtxSSRC;    ///< >0: @m_byRtxMode>0ʱ��Ч, RFC4588�ش�����SSRC, Ĭ��ֵ0
    u32 m_dwRtxHistorySize; ///< >0: ������ʷ��¼�а��ĸ���, �����ش���ƽ������, �����ڴ�MEM=m_dwRtxHistorySize*1500bytes, Ĭ��ֵ4000(1080P 4M@25fpsԼ10s)
}TMSMrtcSenderConfig;

/** MRTC���ն�����       */
typedef struct tagMSMrtcReceiverConfig
{
    u8 m_byNackEnable;  ///< �Ƿ����ش�����, 0: ������, >0: ����, Ĭ��ֵ1
    u8 m_byNackMode;    ///< �ش�����ĸ�ʽ, 0: �ش�����ʹ��SDES NOTE��, kedacom�ɵ�ģʽ, >0: �ش�����ʹ��RFC4585�涨��NACK��, Ĭ��ֵ0 (MRTC�Զ�Э���Ƿ��л�ΪNACK��)
    u8 m_byRtxMode;     ///< ���Ͷ˵��ش�����ʽ, 0: �ش�����ԭý�����ͬ, Kedaʹ�ø�ģʽ, >0: �ش�������RFC4588, ��ԭý�����ͬ, �ж�����PayloadType��SSRC, Ĭ��ֵ0
    u8 m_byRtxPayload;  ///< �����Ͷ˵��ش�������RFC4588, �����ָ���뷢�Ͷ��ش�����ͬ��ý������, Ĭ��ֵ0, Kedaδʹ�ø�ģʽ
    u32 m_dwRtxSSRC;    ///< �����Ͷ˵��ش�������RFC4588, �����ָ���뷢�Ͷ��ش�����ͬ��SSRC, Ĭ��ֵ0, Kedaδʹ�ø�ģʽ
    u32 m_dwMinDelayMs; ///< ���ն˿�ʼ��֡����С�ӳ�ʱ��, Ĭ��ֵ200
    u32 m_dwMaxDelayMs; ///< ���ն���֡���������ӳ�, ��С��@min_delay_ms, ��ʱ�ᶪ����ȫ��֡, Ĭ��ֵ1000
    u8 m_byRtcpEnable;  ///< �Ƿ�������RTCP��, >0: ����, 0: ������, Ĭ��ֵ1 (����������RTCP�Ĺ���ʱ,���Զ�ʹ��,����ӵ�����ƻ򶪰��ش���)
    u8 m_byRembEnable;  ///< �Ƿ���REMB��(draft-alvestrand-rmcat-remb), >0: ����, 0: ������, Ĭ��ֵ1
    u8 m_byRedPayload;  ///< ���Ͷ˵�RED��ý������
    u8 m_byFecPayload;  ///< ���Ͷ˵�FEC��ý������
    u8 m_byFramePacingMode;///< mrtc��֡ģʽ, 0: ��ʱ��֡ģʽ(����ʱ��������)(Ĭ��ֵ) 1: ƽ����֡ģʽ(����ʱ��������) 
    u8 m_byPerfDisable;   //mrtc����ͨ�������ܹر�,0�����������ܽ��գ��ߵ�cpu���ģ�1:�رո����ܽ��գ��͵�Cpu���ģ����鴿ת��ʱ���رո�����
}TMSMrtcReceiverConfig;

/** ����״̬����        */
typedef struct tagMSMrtcNetworkStatus
{
    u32 m_dwEstimatedRate;  ///< ���Ƶĵ�ǰ����
    u32 m_dwRtt;            ///< ����ʱ��,��λms
    u8 m_byLossRate;        ///< ������,ȡֵ0~255,����ٷֱȹ�ʽ ((loss * 100) >> 8)%
    u8 m_byRequestKeyFrame; ///< �Ƿ�����ؼ�֡, >0: ���Ͷ�Ӧ��������һ���ؼ�֡, 0: ������
}TMSMrtcNetworkStatus;

/** ���ն�����״̬���� */
typedef struct tagMSMrtcReceiveState
{
    u32 m_dwRate;                   ///< ��������, ��λbps, �����ʱԼ��������(�����RTPͷ), ���粻��ʱ�����ϴ�, ���¼��2s
    u32 m_dwExpectedDelay;          ///< ������ʱ, ��λms, ��[������С��ʱ, �����������ʱ]֮�䲨��
    u32 m_dwTotalFrames;            ///< �����л�δ���׵�֡��(��������������֡��Ҫ�ȴ��ش�, ��������)
    u32 m_dwFirstTS;                ///< �����л�δ���׵ĵ�һ֡��ʱ���
    u32 m_dwLastTS;                 ///< �����л�δ���׵����һ֡��ʱ���
    u32 m_dwAverageCompleteTimeMs;  ///< ÿ֡������ȫ����ʱ���ƽ��ֵ(��һ����ÿ֡10��), ��λms
}TMSMrtcReceiveState;

typedef struct tagMSStatisticsInfo
{
    u32 m_dwSendSrCount;     ///< sender report count, ����sr������,out��Ч
    u32 m_dwSendRrCount;    ///< receiver report count,���͵�rr��������in��Ч  
    u32 m_dwDiffFromAddPipeToSndOut; ///< add pipeline�����͵�һ����ʱ���ֵ.(����)��out��Ч
    u32 m_dwPackNum;       ///< ������in���յİ���������out���͵İ���
    u32 m_dwPackLose;      ///< �G������in��Ч
    u32 m_dwPackIndexError;///< ���������� in��Ч
    u32 m_dwFrameNum;      ///< �ѽ��յ�֡����in��Ч
    u32 m_dwRtpBitrate;     ///<��RTPͷ�ı�����ͳ�ƣ�in��out��Ч
    u32 m_dwDataBitrate;    ///<�������ݵı�����ͳ��(������rtpͷ)��in��out��Ч
	float  m_fFrameRate;     ///<ʵʱ֡��
}TMSStatisticsInfo;

typedef struct tagMSMux
{
    BOOL32 m_bEnable; ///< ���ʹ��
    u32 m_dwMuxId;       ///< muxid
}TMSMux;

///onvif������չͷ��Ŀǰ��Ϊ��onvif����ʹ�ã��ܶ�ƽ̨�Ը���չͷ֧�ֲ��ã������������������չͷ��ʹ��ǰ��E_MS_OUT_Extence����ҪΪTRUE.
typedef struct tagMSOvfPlaybackExtHdr
{
    BOOL32  m_bEnable;  ///< ��������
    /*
    Cseq: 1 byte. This is the low-order byte of the Cseq value used in the RTSP PLAY
    command that was used to initiate transmission. When a client sends multiple,
    consecutive PLAY commands, this value may be used to determine where the data from
    each new PLAY command begins.
    */
    u8      m_byCSeq;   ///< rtsp play id
}TMSOvfPlaybackExtHdr;

/** ����������Բ�������  */
typedef enum tagMSOpt
{
    /** ����ѡ�����û��ע���ɶ�̬�޸ģ�����Ҫ������/���startǰ���� */
	/** ����Ϊ����/�����Ҫ���õĲ��� */
	E_MS_Type = 0, ///< EMSType ��������������������ES����PS(��������)
	E_MS_RtcpAlive = 1,///< TMSRtcpAlive  ������� rtcp����ģʽ, Ĭ�ϱ���رգ������״̬�£�Ĭ��90�룬�ɶ�̬�޸�
	E_MS_DescripeInfo = 2, ///< TMSDescripeInfo in/out���������Ϣ��
    E_MS_SockBuf  = 3, ///<u32 dwBufSize,����socket���ͽ��ջ���,Ĭ��2MB.��λ�ֽ�.
    E_MS_Statistics = 4, ///< TMSStatisticsInfo* ptStat, ͳ����Ϣ. ֻ֧�ֻ�ȡ.
    E_MS_SharePort = 5, ///< BOOL32 bShared, rtp�˿��Ƿ���,Ĭ��FALSE,������,�˿���ÿ��In��out��ռ;����˿ڹ���������Զ�˵Ķ˿����ַ����ȷ��д���������ݰ��������
    //E_MS_ChkSrcInfo = 6, ///<BOOL32 bOn,����udpԴ��ַ�˿�У�飬Ĭ��ΪFALSE,�ر�.�ɶ�̬�޸�,������ֳ�E_MS_UdpRtpChkSrcAddr��E_MS_UdpRtcpChkSrcAddr
    E_MS_RtpMux = 7, ///<TMSMux, RTP��·����
    E_MS_RtcpMux = 8, ///<TMSMux, RTCP��·����
    E_MS_MapIPPort = 9, ///<TMSMap, �������ʱ��Ҫαװ��ip�Լ��˿ڣ�αװip�˿ڣ������������ģ�
    E_MS_UdpPeerRtpParam = 10,///<TMSUdpPeerParam, ���¶Զ�rtp����, in��MSInStart���ú�ʹ�ã�out��MSOutStart���ú�ʹ�ã�
    E_MS_UdpPeerRtcpParam = 11, ///<TMSUdpPeerParam, ���¶Զ�rctp����,in��MSInStart���ú�ʹ�ã�out��MSOutStart���ú�ʹ�ã�
    E_MS_UdpRtpChkSrcAddr = 12, ///<<BOOL32 bEnable, Ĭ��FALSE,rtpԴ��ַУ�飬���ڷ�������
    E_MS_UdpRtcpChkSrcAddr = 13, ///<<BOOL32 bEnable, Ĭ��FALSE,rtcpԴ��ַУ�飬���ڷ�����
    E_MS_RtcpSharePort = 14, ///< BOOL32 bShared, rtcp�˿��Ƿ���,Ĭ��FALSE,������,�˿���ÿ��In��out��ռ;����˿ڹ���������Զ�˵Ķ˿����ַ����ȷ��д���������ݰ��������Ĭ�ϲ�����TRUE��ԭ��IPC��nat����NVR��Ҫ��������ĵ�һ��rtcp��������Զ�˵�ַ

	/** ����Ϊ�������Ҫ���õĲ��� */
	E_MS_IN_MaxINPacketNum = 20,///< u32 dwMaxInPacketNum, ��������������������Ĭ��Ϊ2048,�����õ�ֵ��Ϊ2��ָ���ݣ�������ȡ����2��ָ���ݣ���󲻵ó���32768
	E_MS_IN_MaxOutPacketNum = 21,///< u32 dwMaxOutPacketNum, ����������֡��������Ĭ��Ϊ8192,�����õ�ֵ��Ϊ2��ָ���ݣ�������ȡ����2��ָ���ݣ���󲻵ó���32768
	E_MS_IN_MaxFrameNum = 22,///<u32 dwMaxFrameNum���������֡��������Ĭ��Ϊ1����ģʽ�£�����֡��������Ҫ����Ϊ1
	E_MS_IN_RSParam = 23,///<TMSRSParam, ����udp����ʱ���ش������������ֵ�ּ�����䣬����һ��40 80 120 200��512�������壩���������80 200 480 720��2048�������壩���������� 100 280 600 1500��4096�������壩��Ĭ�ϼ������
	E_MS_IN_EncNameAndPayload = 24,///<TMSEncNameAndPayload������onvif����������������Լ�payload�������������չ�˾�ڲ�payload����
	E_MS_IN_Sample = 25,///<u32 dwSample������onvif������������Ƶ�����ʣ�aaclc��ʽ��������
	E_MS_IN_AaclcChannel = 26,///<u8 byAaclcChannel������onvif����������Ƶaaclc������������aaclc��ʽ��Ƶ��aaclc��ʽ��������
	E_MS_IN_SpsPpsInfo = 27,///<TMSSpsPpsInfo, ��������sps��pps��Ϣ������onvif�����⳧��ĳЩ�����в�Я��sps��pps��Ϣ������rtsp������Я�������
	E_MS_IN_ReSend = 28,///<BOOL32 bResend,����udp����ʱ�Ƿ����ش���Ĭ��ΪTRUE-�����ش�, �ɶ�̬�޸�
	E_MS_IN_KedacomRtp = 29,///<BOOL32 bKedacomRtp, �����Ƿ�Ϊ�ƴ��Լ���RTP����TRUE��ʾ�ƴ��Լ���rtp����FALSE��ʾ�⳧�̵�RTP����onvif�����⳧��ʱ��Ҫ����ΪFALSE��Ĭ��ΪTRUE
	E_MS_IN_FrameRate = 30,///<u8 byFrameRate,����֡�ʣ�onvif�����⳧������ʱ��Ҫ��rtsp�����л�ȡ֡����Ϣ���ã�Ĭ��Ϊ25֡/��
	E_MS_IN_StandardTS = 31,///<BOOL32 bStandardTS ���������Ƿ�Ϊ��׼ʱ�����TRUE��ʾΪ��׼ʱ�����FALSE��ʾΪ�Ǳ�׼ʱ�����Ĭ��ΪTRUE
	//E_MS_IN_RcvStatics = 32,///<TMSRcvStatistics  ��֧��get����ȡ�����ͳ����Ϣ
	E_MS_IN_VideoInfo  = 33,///< TMSVideoInFo ,��ȡ��Ƶ�����Ϣ����ЧֵΪ0��������GUI�����¼��ʱ����������֡�߼������յ��ؼ�֮֡����п����Ϣ

	E_MS_IN_Ts_Calc_Mode = 34,///< TMSTSCalParam, ����ʱ������㷽��ѡ��.
	E_MS_IN_Frame_List_Cover = 35,///<BOOL32 bCover ����֡����(֡����)�Ƿ�������.Ĭ��ΪTRUE.
	E_MS_IN_VKEK = 36,///< TMSVkekInfo, ����vkek,vkek����ʱ��Ҫ����vkek.�ɶ�̬�޸�
    //E_MS_IN_FilterFunc = 37, ///<MSFilterFunc, ���ù��˺���,���Կ�,����Ϊ��Ч�ĺ���ָ��.�ɶ�̬�޸�
    E_MS_IN_ReSendBuf  = 38, ///<BOOL32 bOn,����udp�ش������Ƿ�����Ĭ��ΪFALSE,û���ش�����.�ɶ�̬�޸�
    E_MS_IN_P2pReSend  = 39, ///<BOOL32 bOn,����udp��ȷ�ش��Ƿ�����Ĭ��ΪFALSE,��ȷ�ش��ر�.�ɶ�̬�޸�
    E_MS_IN_ChkStreamInfo = 40, ///<BOOL32 bChk,Ĭ�Ͽ��������ý��������������Լ�⣬����ps������Ƶ��Ŀ�仯���߱仯��es������Ƶ��߱仯��es����Ƶ������������λ��仯.�ɶ�̬�޸�
    
    /** ����Ϊ�������Ҫ���õĲ��� */
	E_MS_OUT_Payload = 100, ///<TMSChangePayload �������payload��������Ҫ�޸�payloadʱ����,rtsp ��tcp���ʱ��Ҫ���Ӧ��m_nInterleavedId�����������m_nInterleavedId��0��
	E_MS_OUT_Extence = 101,///<BOOL32 bExtence ��������Ƿ����չͷ��TRUE��ʾ����չͷ��FALSE��ʾ������չͷ��Ĭ��ΪTRUE
	E_MS_OUT_StandardTS = 102,///<BOOL32 bStandardTS ��������Ƿ�Ϊ��׼ʱ�����TRUE��ʾΪ��׼ʱ�����FALSE��ʾΪ�Ǳ�׼ʱ�����Ĭ��ΪTRUE
	E_MS_OUT_SSRC = 103,///<u32 dwSSRC �������SSRC��Ĭ��Ϊ���޸�SSRC
	//E_MS_OUT_MapIPPort = 104,///<TMSMap,�������ʱ��Ҫαװ��ip�Լ��˿ڣ�αװip�˿ڣ������������ģ�
	E_MS_OUT_UnStandardAaclc = 105,///<BOOL32 bStandardAaclc,���÷��ͷǱ�׼��aaclc��������Ƶ�����޷�������ر�׼��aaclc��������Ϊ��ص�aaclc��4�ֽ�ͷ������ʱ��֧��udp����Ϊ������ͨ����udp��
	E_MS_OUT_SingleAudio = 106,///<BOOL32 bSingleAudio,���������Ƿ��ǵ���Ƶ
    E_MS_OUT_PackResExtHdrNum = 107, ///<u32 dwReservedExtHdrNum,�����а�ʱԤ������չͷ�ֶθ�����������H265��PS��svac��Ч
    E_MS_OUT_TrackID = 108, ///<TMSTrackID ����������trackid��������PS������Ч
	E_MS_OUT_MultNetSet = 109, ///<TMSMultCardList ���ö��������͵�������Ϣ
	E_MS_OUT_PayloadEx = 110, ///< ������E_MS_OUT_Payload��E_MS_OUT_Payload�޸����а���payload��E_MS_OUT_PayloadEx���޸ķ��������İ���payload������Ƶ�����������£���������Ҫֻ�޸�����һ·��payload��
    E_MS_OUT_OvfPlaybackExtHdr = 111, ///< TMSOvfPlaybackExtHdr onvif������չͷ��Ŀǰ��Ϊ��onvif����ʹ�ã��ܶ�ƽ̨�Ը���չͷ֧�ֲ��ã�ʹ��ǰ��E_MS_OUT_Extence����ҪΪTRUE.
	
    E_MS_MrtcCreateChannel = 201, ///< TMSMrtcChnCreateCfg, ����MRTC�ŵ�, ��ָ������Ƶ�ŵ�, ������Ƶ�ŵ�
    E_MS_IN_MrtcConfig = 231,     ///< TMSMrtcReceiverConfig, ��ȡ������MRTC�����ŵ������� 
    E_MS_OUT_MrtcConfig = 251,    ///< TMSMrtcSenderConfig, ��ȡ������MRTC�����ŵ�������
}EMSOpt;

/** udp������� */
typedef struct tagMSUdpTransParam
{
	TMSRtpSession      m_tLocalNet;  ///< �����������
	TMSRtpSession      m_tRemoteNet; ///< Զ���������
}TMSUdpTransParam;

/** udp�Զ˲��� */
typedef struct tagMSUdpPeerParam
{
	TMSNetAddr  m_tAddr;  ///< ��ַ(������)
	u16         m_wPort;  ///< �˿�(������)
}TMSUdpPeerParam;

/** tcp������� */
typedef struct tagMSTcpTransParam
{
	TMSRtpSession      m_tLocalNet;        ///< �����������
	TMSRtpSession      m_tRemoteNet;       ///< Զ���������
	u32                m_dwDisconnectTime; ///< ��ʱ����ʱ��,0Ϊ�첽�ӿڣ�����ֵͬ���ӿڣ���λ��(ͬ���ӿڽ��𲽷���)
//	BOOL32             m_bListen;		   ///< �Ƿ�Ϊ������
}TMSTcpTransParam;

/** rtspЭ��ʹ�õ�interleavedid��trackid�Ķ�Ӧ */
typedef struct tagMSInterleavedAndTrackId
{
	s32         m_nInterleavedId; ///< rtspЭ��ʹ�õ�interleavedid
	TKDTrackID  m_tTrackId;       ///< trackid
}TMSInterleavedAndTrackId;

/** fd������� */
typedef struct tagMSRtspTransParam
{
	s32                         m_nFd; ///< fd
	TMSInterleavedAndTrackId	m_atInterleavedAndTrackId[KD_MAX_NUM_TRACK]; ///< rtspЭ��ʹ�õ�interleavedid������trackid��Ӧ��ϵ
}TMSRtspTransParam;


/** �������� */
typedef enum tagMSTransType
{
	E_Ms_UnknowTransType = 0,///< δ֪��������
	E_MS_Udp,				///< TMSUdpTransParam udp�������
	E_MS_Tcp,				///< TMSTcpTransParam tcp�������
	E_MS_Rtsp,				///< TMSRtspTransParam rtsp�������
	E_MS_YushiGBTcp,		///< TMSTcpTransParam tcp�������
}EMSTransType;

/** ����λ����Ϣ */
typedef enum tagMSPosType
{
	E_MS_GetLastPos = 1, ///< ��ȡ���������µ�λ��
	E_MS_GetFirstPos,    ///< ��ȡ���������ϵ�λ��
	E_MS_GetBeforeTime,  ///< ��ȡʱ���ǰһ���ؼ�֡��λ��
}EMSPosType;


/** PS����Ϣ */
typedef struct tagMSRtpMediaParam
{
	TMSEncNameAndPayload m_tEncNameAndPayload;
	union
	{
		struct
		{
			BOOL32    m_bKeyFrame;    /*Ƶ֡���ͣ�I or P��*/
			u16       m_wVideoWidth;  /*��Ƶ֡��*/
			u16       m_wVideoHeight; /*��Ƶ֡��*/
		}m_tVideoParam;
		struct
		{
			u32       m_dwSample;           /*��Ƶ������*/
			u16       m_wChannel;           /*������*/
			u16       m_wBitsPerSample;     /*λ��*/
		}m_tAudioParam;
	}x;

}TMSMediaParam;

/** PS�������Ϣ */
typedef struct tagMSPsRtpMediaInfoChange
{
	TKDTrackID   m_tPSTrackID;              ///<�������Ϣ
	TMSMediaParam m_tMspsMediaParam;       ///<����Ϣ
}TMSMediaInfoChange;

typedef struct tagKDTrackIDExt
{
    EKDTrackType    m_eTrackType;       ///<����
    u32             m_dwTrackIndx;      ///<��Բ�ͬ��TrackType,������ֵ����0��ʼ
    u32             m_dwId;             ///<InId
}TKDTrackIDExt;

/** TrackID��Ϣ */
typedef struct tagMSTrackID
{
    //TKDTrackID  m_atTrackId[KD_MAX_NUM_TRACK];  ///<TrackID�б�
    TKDTrackIDExt  m_atTrackId[KD_MAX_NUM_TRACK];  ///<TrackID�б�
    u16 m_wNum;   ///<TrackID����
}TMSTrackID;

typedef struct tagMSNetCardInfo
{
	s8            m_szCardName[KD_MAX_LEN_NETCARD_NAME]; /// ��������
	TMSMapAddr    m_tCardAddr;                  /// ʹ�õ�������ַ���˿ںţ�ʹ��rawsocket���з��ͣ�
}TMSNetCardInfo;

typedef struct tagMSMultCardList
{
	u16               m_wNum;                                   /// ������������
	TMSNetCardInfo    m_atNetCardInfo[KD_MAX_NUM_NET_CARD];     /// �������� mark ע��
} TMSMultCardList;

/** ����sps�Լ�pps���ڵ�buffer */
typedef struct tagMSFrameBuf 
{
	u8*      m_pbyframeBuf; ///< sps��pps���ڵ�buffer
	u32      m_dwframeBuf;  ///< sps��pps���ڵ�buffer����
}TMSFrameBuf;

/** �����¼����� */
typedef enum tagMSEventType
{
	/** ������Ϣ�¼��ص� */
	E_MS_RecvRtcpRR      = 1,	///< �յ�RR��
	E_MS_RecvRtcpBYE     = 2,	///< �յ�BYE��
	E_MS_PSStreamSpsPps  = 5, ///< TMSFrameBuf   �����е�sps�Լ�pps���ڵ�buffer
	E_MS_ESStreamSps     = 6, ///< TMSSpsPpsInfo �����е�sps
	E_MS_ESStreamPps     = 7, ///< TMSSpsPpsInfo �����е�pps
	/** �����¼��ص� */
	E_MS_TcpDisconnect   = 100,	///< tcp����
	E_MS_MallocFailed    = 101,	///< �ڴ����ʧ��
	E_MS_StreamChange    = 102, ///< ������
	E_MS_UnKnown_Payload = 103, ///< δ֪�����غ�����  pEventData Ϊ�غ�����
	E_MS_RtcpDisconnect  = 104, ///< rtcp�������
	E_MS_TSError         = 105, ///< ���õ�ʱ�����׼ȷ��pEventData Ϊ�Ƿ��׼ʱ��� bStandardTS
	E_MS_TcpSendError    = 106,	///< tcp����ʧ��
    E_MS_TcpDataErr      = 107, ///< ���յ�tcp���ݴ���
    E_MS_MediaName = 108, ///< ���յ�����Ƶ��������� ʹ�ýṹ��TMSMediaInfoChange
    E_MS_StreamNumber = 109, ///< in�������������
    E_MS_TcpConnectOk = 110, ///< tcp���ӳɹ�
    E_MS_TcpAcceptOk = 111, ///< tcp accept�ɹ�
	/** mrtc�¼��ص� */
    E_MS_NetworkChanged  = 201,     ///< ����״̬�ϱ�
    E_MS_ReceiveStateReport = 202,  ///< ���ն�״̬�ϱ�
}EMSEventType;

/** �˰󶨲��� */
typedef struct tagMSThreadBindCoreParam
{
    u16  m_wBindCoreNum;		///< �󶨵ĺ˸���
    u16  m_awBindCoreList[MS_MAX_BIND_CORE_NUM]; ///<�󶨵ĺ��б�,ȡֵ��Χ[0,256)����һ����IDΪ0���Դ�����
}TMSThreadBindCoreParam;

/** ��ʼ��������Ϣ */
typedef struct tagMSInitParam 
{
	u32  m_dwMaxInNum;		///< ���������
	u32  m_dwMaxOutNum;		///< ��������
	u32  m_dwMaxPiplineNum; ///< ���������
    u16  m_wMsgStartPort;   ///< ģ���ڲ�����ͨ����ʼ�˿�.Ĭ��20400
    u16  m_wMsgEndPort;     ///< ģ���ڲ�����ͨ����ֹ�˿�.Ĭ��20406
    TMSThreadBindCoreParam   m_tRecvThreadBind; ///<�����̰߳󶨺˲���
    TMSThreadBindCoreParam   m_tSendThreadBind; ///<�����̰߳󶨺˲���
}TMSInitParam;

/** ֡�ڴ���估�ͷ���Ϣ */
typedef struct tagMSFrame
{
	TKDFrame  m_tFrame;			///< ֡�ṹ��
	s32       m_nRefCount;		///< ���ü���
	void* m_hFrameSem;		///< ��
// #ifdef _LINUX_
// 	atomic_t  m_tRefCount;
// #else
// #endif

	BOOL32 (*MSMallocFrame) (struct tagMSFrame*, u32 dwFrameSize); ///< �����ڴ棬�ڲ���
	void   (*MSFreeFrame)   (struct tagMSFrame*); ///< �ͷ��ڴ�
	void   (*MSFrameAddRef) (struct tagMSFrame*); ///< �������ü���
	
	u32       m_dwRtpTimeStamp;          /*ʱ��������յ�RTPЭ����Я����ʱ���*/
}TMSFrame;

/** nat prob */
typedef struct tagMSNatProbePack
{
	//NAT ̽�����buf ������ ��buf�����Ƿ��usrdata�����ϲ���䡣 
	u8         *m_pbyBuf;//̽���buffer 
	u16         m_wBufLen;//̽�������,������1452	
	//nat ̽����Զ˵�ַ
	TMSNetAddr  m_tPeerAddr;//RTCP�ط���ַ(������)
	u16         m_wPeerPort;//RTCP�ط��˿ڣ�������
}TMSNatProbePack;

typedef struct tagMSNatProbeProp 
{ 
	TMSNatProbePack m_tRtpNatProbePack; //rtp natpack struct
	TMSNatProbePack m_tRtcpNatProbePack; //rtcp natpack struct	
	u32 m_dwInterval;  //���ͼ������λs����Ϊ0�����ʾ�����͡�
}TMSNatProbeProp;

typedef struct  tagMSExInfo
{
    u8*    m_pbyExBuf;//Ĭ��ΪNULL�����ڶ�����Ϣ�� ����������Ϣ������ˮӡ�ȣ��Լ��ٿ���
	u32    m_dwExBufLen;//Ĭ��Ϊ0��
    BOOL32 m_bSingleNalu;//�Ƿ�Ϊ��nalu��Ĭ��ΪTRUE,��nalu,H264�а�ʱP֡��ȥ��������0001���ɽ���CPU����,��nalu�����bSingleNaluΪTRUE��ע���ͨ�Ƿ������⣩
}TMSExInfo;

typedef struct tagMSFilterFuncParam
{
    TMSNetAddr m_tRecvIP; ///<����IP
    u16 m_wRecvPort;///<���ն˿�
    TMSNetAddr m_tSrcIP; ///<ԴIP
    u16 m_wSrcPort;///<Դ�˿�
    u8* m_pData;    ///<����ָ��
    u32 m_dwLen;    ///<���ݳ���.
    u32 m_dwId;     ///<IDֵ
}TMSFilterFuncParam;

/** 
* @pam[in] void* TMSFilterFuncParam* ptParam ������Ϣ
* @return  	  TRUE:���ݹ��˵���FALSE:���ݲ�����
*/
typedef BOOL32 (*MSFilterFunc)(TMSFilterFuncParam* ptParam);

/** 
* @pam[out] void* pvContext  		������ָ��
* @note		  ֡�ص�ʱpDataΪ֡�ṹ��TMSFrame���ص�������ʹ������Ҫʹ��TMSFrame�е�MSFreeFrame�����ͷ�.���ص�ʱpDataΪ���ṹ��TKDPack.
*/
typedef   void (*PMSDATAPROC)(void* pData, void* pvContext); 

/** 
* @brief  �¼��ص�����
* @param[in]  u32 dwId	����ʱ���ص�ID
* @param[in]  EMSEventType eEventType	�¼�����
* @param[out] void* pEventData  		���ص�data
* @param[out] u32 dwEventDataLen 		���ص�data����
* @param[out] void* pEventData  		������ָ��
*/
typedef   void (*PMSEVENTPROC)(u32 dwId, EMSEventType eEventType, void* pEventData, u32 dwEventDataLen, void* pvContext);

/** 
* @brief      ��ʼ��ģ��
* @param[in]  TMSInitParam* ptInitParam   	��ʼ������  �μ�MAXMSINLISTNUM��MAXMSOUTLISTNUM��MAXMSPIPLINELISTNUM
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInit(const TMSInitParam* ptInitParam);

/** 
* @brief      �����а�����
* @param[in]  u16 wPackLength   �а����ȣ����ɳ���IPC_MAX_RTPPACK_LENGTH-1384��
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSSetPackLength(u16 wPackLength);

/** 
* @brief      ����ʼ��
* @note		  ����ʼ��
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSUninit();

/**
*@brief ����ȫ��tcp�˿ڷ�Χ
*@param[in]    u16 wStartPort��С�˿�
*@param[in]    u16 wEndPort���˿�
*@return       �μ������붨��
*@note ��MSInTcpBindǰ���ã�ֻ�ɵ���һ�Ρ�
*/
MEDIASWITCH_API MSRESULT MSSetTcpPortRange(u16 wStartPort, u16 wEndPort, u16 wSharePortNum);

/**
*@brief ���ù��˺���,�������н��յİ�
*@param[in]    MSFilterFunc pFilter ���˺���ָ��,pFilterΪNULLʱ������ص���֧�ֶ�̬����
*@return       �μ������붨��
*@note �����е�IN��Ч
*/
MEDIASWITCH_API MSRESULT MSSetFilter(MSFilterFunc pFilter);

/** 
* @brief      ����
* @param[out]  PEVENTPROC pfEventCallBackProc �¼��ص�
* @param[out]  void* pvContext�ص�������
* @param[out]  u32* pdwId  ���ض���id
* @note		  ����ȫ�ֱ�������
*             ���������õĻص������е���mediaswitch�ӿ�,������������������ٻص�������ִ�к�ʱ����
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInCreate(PMSEVENTPROC pfEventCallBackProc, void* pvContext, u32* pdwId);


/** 
* @brief      ���ò�����������setsockopt��ͨ����ͬ��EMSOpt�����ò�ͬ�Ľṹ�����
* @param[in]  u32 dwId �������id
* @param[in]  EMSOpt eOpt  ���ò�������
* @param[in]  void* pvOptParam  �����ṹ��
* @param[in]  u32 dwOptParamLen �����ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInSetOpt(u32 dwId, EMSOpt eOpt, const void* pvOptParam, u32 dwOptParamLen);


/** 
* @brief      ��ȡ������������getsockopt��ͨ����ͬ��EMSOpt�����ò�ͬ�Ľṹ�����
* @param[in]  u32 dwId �������id
* @param[in]  EMSOpt eOpt  ���ò�������
* @param[out]  void* pvOptParam  �����ṹ��
* @param[out]  u32 dwOptParamLen �����ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInGetOpt(u32 dwId, EMSOpt eOpt, void* pvOptParam, u32* dwOptParamLen);


/** 
* @brief      �������ݣ�����EKDDataPackFormat�Ĳ�ͬ����֡�����Լ������ݣ��ýӿ���MSInSetTransParam����ͬʱ����
* @param[in]  u32 dwId �������id
* @param[in]  EKDDataPackFormat eDataPackFormat  �������ݸ�ʽ
* @param[in]  void* pData�������ݽṹ��ָ��  TKDPack��TKDFrame
* @param[in]  u32 dwDataLen  �������ݽṹ���С
* @param[in]  void* pExData�������ݽṹ��ָ��  TMSExInfo,���������ʱTMSDataExt����ָ�룬����ΪNULL
* @param[in]  u32 dwExDataLen  �������ݽṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInInputData(u32 dwId, EKDDataPackFormat eDataPackFormat, void* pData, u32 dwDataLen, void* pExData, u32 dwExDataLen, const void* pvContext);

/** 
* @brief      tcp��
* @param[in]  const TMSTcpBindParam* ptParam �󶨲���.
* @param[out] u16* pwBindPort �󶨵Ķ˿�.
* @note		  ����ȫ�ֱ�������,tcpʹ��.�ڵ���MSInSetTransParamǰ����.
* @return  	  �μ������붨��
*/
MEDIASWITCH_API  MSRESULT MSInTcpBind(u32 dwId, const TMSTcpBindParam* ptParam, u16* pwBindPort);

/** 
* @brief      ������������������ýӿ���MSInInputData����ͬʱʹ��
* @param[in]  u32 dwId �������id
* @param[in]  ETransType eTransType �����������
* @param[in]  void* pTransParam ��������ṹ��ָ��
* @param[in]   u32 dwParamSize ��������ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInSetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32 dwParamSize);


/** 
* @brief      ��ȡ�����������
* @param[in]  u32 dwId �������id
* @param[in]  ETransType eTransType �����������
* @param[out] void* pTransParam ��������ṹ��ָ��
* @param[out] u32 dwParamSize ��������ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInGetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32* pdwParamSize);


/** 
* @brief      ��ʼ��������
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInStart(u32 dwId);


/** 
* @brief      ֹͣ��������
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInStop(u32 dwId);

/** 
* @brief      ����nat̽�������
* @param[in]  u32 dwId �������id
* @param[in]  TMSNatProbeProp nat̽����ṹ��
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInSetNatProbeProp(u32 dwId, TMSNatProbeProp *ptNatProbeProp);

/** 
* @brief      �ͷ�
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInRelease(u32 dwId);

/** 
* @brief      ��ջ���(����֡������δ��֡�İ�)
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������,ʹ��MSInInputData�������ݵ����ʹ��.
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSInClearBuf(u32 dwId);

/** 
* @brief      �����������
* @param[out]  PEVENTPROC pfEventCallBackProc �¼��ص�
* @param[out]  void* pvContext �ص�������
* @param[out]  u32* pdwId �����������id
* @note		  ����ȫ�ֱ�������
*             ���������õĻص������е���mediaswitch�ӿ�,������������������ٻص�������ִ�к�ʱ����
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutCreate(PMSEVENTPROC pfEventCallBackProc, void* pvContext, u32* pdwId);

/** 
* @brief      �����������������setsockopt
* @param[in]  u32 dwId �������id
* @param[in]  EMSOpt eOpt  ���ò�������
* @param[in]  const void* pvOptParam  �����ṹ��
* @param[in]  u32 dwOptParamLen �����ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutSetOpt(u32 dwId, EMSOpt eOpt, const void* pvOptParam, u32 dwOptParamLen);

/** 
* @brief      ��ȡ�������������getsockopt
* @param[in]  u32 dwId �������id
* @param[in]  EMSOpt eOpt  ���ò�������
* @param[out] const void* pvOptParam  �����ṹ��
* @param[out] u32 dwOptParamLen �����ṹ���С
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutGetOpt(u32 dwId, EMSOpt eOpt, void* pvOptParam, u32* dwOptParamLen);

/** 
* @brief      �������ݻص�
* @param[in]  u32 dwId �������id
* @param[in]  const TKDTrackID*  ptTrackId ���TrackId
* @param[out] EKDDataPackFormat eDataPackFormat ��ȡ���ݸ�ʽ��������֡
* @param[out] PDATAPROC pfDataCallBackProc  ���ݻص�����
* @param[out] void* pvContext   �ص�������
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutSetDataCallback(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, PMSDATAPROC pfDataCallBackProc, void* pvContext);

/** 
* @brief      tcp��
* @param[in]  u32 dwId �������id
* @param[in]  const TMSTcpBindParam* ptParam �󶨲���.
* @param[out] u16* pwBindPort �󶨵Ķ˿�.
* @note		  ����ȫ�ֱ�������,tcpʹ��.�ڵ���MSOutSetTransParamǰ����.
* @return  	  �μ������붨��
*/
MEDIASWITCH_API  MSRESULT MSOutTcpBind(u32 dwId, const TMSTcpBindParam* ptParam, u16* pwBindPort);

/** 
* @brief      ��������������
* @param[in]  u32 dwId �������id
* @param[in]  EMSTransType eTransType �����������
* @param[in] const void* pTransParam ��������ṹ��
* @param[in] u32 dwParamSize ��������ṹ���С
* @note		  1.����ȫ�ֱ�������  2.��ģʽ�������������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutSetTransParam(u32 dwId, EMSTransType eTransType, const void* pTransParam, u32 dwParamSize);

/** 
* @brief      ��ȡ�������
* @param[in]  u32 dwId �������id
* @param[in]  EMSTransType eTransType �����������
* @param[out] const void* pTransParam ��������ṹ��
* @param[out] u32 dwParamSize ��������ṹ���С
* @note		  1.����ȫ�ֱ�������  2.��ģʽ�������������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutGetTransParam(u32 dwId, EMSTransType eTransType, void* pTransParam, u32* pdwParamSize);

/** 
* @brief      ��ȡ����λ��
* @param[in]  u32 dwId �������id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in] EKDDataPackFormat eDataPackFormat ��ȡ���ݸ�ʽ��������֡
* @param[in] EMSPosType ePosType ����λ�û�������λ��
* @param[out] u16 *pwPos  ����λ��
* @note		  1.����ȫ�ֱ�������  2.��ģʽ�»�ȡ���ݽӿ�
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutGetDataPos(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, EMSPosType ePosType, u64 qwUtcTime, u16 *pwPos);

/** 
* @brief      ��ȡ����
* @param[in]  u32 dwId �������id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in]  EKDDataPackFormat eDataPackFormat ��ȡ���ݸ�ʽ��������֡(����֡��ʽΪTMSFrame,����ʽΪTKDPack)
*             ֡��������֮��ʹ��(TMSFrame*)ptData->MSFreeFrame((TMSFrame*)ptData)�ͷ�
			  ����������֮��ʹ��MSFreeData�ͷţ�MSFreeData�ӿ���δʵ��֡���ݵ��ͷţ�ʵ�ֺ��ͳһʹ�øýӿ��ͷţ�
* @param[in]  u16 wSeq ��ȡλ��
* @param[out] void* ptData ������Ӧ�����ݽṹ��
* @note		  1.����ȫ�ֱ�������  2.��ģʽ�»�ȡ���ݽӿ�
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutGetData(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, u16 wPos, void* ptData);

/** 
* @brief      ��ȡ��һλ��
* @param[in]  u32 dwId �������id
* @param[in]  const TKDTrackID*  ptTrackId
* @param[in]  EKDDataPackFormat eDataPackFormat ��ȡ���ݸ�ʽ��������֡
* @param[in]  u16 wSeq �ο�λ��
* @param[out] u16* pwNextPos �ο�λ����һλ��
* @note		  1.����ȫ�ֱ�������  2.��ģʽ�»�ȡ���ݽӿ�
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutGetNextPos(u32 dwId, const TKDTrackID*  ptTrackId, EKDDataPackFormat eDataPackFormat, u16 wPos, u16* pwNextPos);

/** 
* @brief      ����rtcp bye
* @param[in]  u32 dwId �������id
* @note		  ����һ�Σ�����һ����Ϣ,������udp��ʽ
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutSendBYE(u32 dwId);


/** 
* @brief      ��ʼ���
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������  
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutStart(u32 dwId);

/** 
* @brief      ֹͣ���
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������  
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutStop(u32 dwId);

/** 
* @brief      ����nat̽�������
* @param[in]  u32 dwId �������id
* @param[in]  TMSNatProbeProp nat̽����ṹ��
* @note		  ����ȫ�ֱ�������
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutSetNatProbeProp(u32 dwId, TMSNatProbeProp *ptNatProbeProp);

/** 
* @brief      �ͷ��������
* @param[in]  u32 dwId �������id
* @note		  ����ȫ�ֱ�������  
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSOutRelease(u32 dwId);



/** 
* @brief      ��������
* @param[out]  PEVENTPROC pfEventCallBackProc �¼��ص�
* @param[out]  void* pvContext ������ָ��
* @param[in]   u32 adwMSInId[KD_MAX_NUM_TRACK] �������id������������ʼ����0֮���������ᱨ��
* @param[out]  u32 dwMSOutId �������id
* @param[out]  u32* pdwId pipline����id
* @note		  1.����ȫ�ֱ�������  
*             2.֧�ֶ������һ�����ESתPS��
*			  3.PSתES��������ö��һ�����һ���(������ͬ�����ͬ)��ESתES��ʹ��һ�����һ���
*             4.���������õĻص������е���mediaswitch�ӿ�,������������������ٻص�������ִ�к�ʱ����
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSAddPipeline(PMSEVENTPROC pfEventCallBackProc, void* pvContext, const u32 adwMSInId[KD_MAX_NUM_TRACK], u32 dwMSOutId, u32* pdwId);


/** 
* @brief      �ͷŶ���
* @param[in]  u32 dwId �ͷ�pipline����id
* @note		  1.����ȫ�ֱ�������  
*             2.֧�ֶ������һ�����ESתPS��
*			  3.PSתES��������ö��һ�����һ���(������ͬ�����ͬ)��ESתES��ʹ��һ�����һ���
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSRemovePipeline(u32 dwId);

/** 
* @brief      �ͷŰ����ݻ�֡����
* @param[in]  void * pData֡���ݻ��߰�����ָ�루��δ֧��֡���ݵ��ͷţ�������֧�֣�
* @note		  1.����ȫ�ֱ�������             
* @return  	  �μ������붨��
*/
MEDIASWITCH_API MSRESULT MSFreeData(void * pData, EKDDataPackFormat eDataPackFormat);

 
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif//  
