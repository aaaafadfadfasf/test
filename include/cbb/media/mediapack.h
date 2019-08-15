/**
 * @file Mediapack.h  
 * @brief    �շ�UDP����֧��KEDACOM��RTP//RTCP����
 * @author    ���
 * @date 2016-6-16
 * @version  1.0 
 * @copyright V1.0  Copyright(C) 2016 Keda All rights reserved.												  
 */
#if !defined(AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_)
#define AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_

#include "osp.h"

namespace MPNameSpace {

#define		MP_ERR_BASE						7100
/** OK*/
#define		MP_ERROK						(u16)(0)
/** ��������*/
#define		MP_ERR_PARAM					(u16)(MP_ERR_BASE+ 1)
/** û��memory*/
#define		MP_ERR_NOMEM					(u16)(MP_ERR_BASE+ 2)
/** OspSem����ʧ��*/
#define		MP_ERR_SEM_CREATE				(u16)(MP_ERR_BASE+ 3)
/** SOCK����ʧ��*/
#define		MP_ERR_SOCKET_CREATE			(u16)(MP_ERR_BASE+ 4)
/** SOCCK bind ʧ��*/
#define		MP_ERR_SOCKET_BIND				(u16)(MP_ERR_BASE+ 5)
/** UNKNOWN ����*/
#define		MP_ERR_UNKNOWN					(u16)(MP_ERR_BASE+ 6)

#define		MP_ERR_CMD_MAGIC_WRONE			(u16)(MP_ERR_BASE+ 7)
/** watch socket recv cmd wrong*/
#define		MP_ERR_CMD_CMD_WRONE			(u16)(MP_ERR_BASE+ 8)
/** WATCH�׽��ֽ���ʧ��*/
#define		MP_ERR_WATCH_SOCKET				(u16)(MP_ERR_BASE+ 9)

/**  ���ͷ�ʧ��*/
#define		MP_ERR_RELEASE_FAIL				(u16)(MP_ERR_BASE+ 10)
/**  ���ʼ��ʧ��*/
#define		MP_ERR_INIT_FAIL				(u16)(MP_ERR_BASE+ 11)
/**  sendto fail�����ֽڲ���*/
#define		MP_ERR_SOCKET_SEND_FAIL			(u16)(MP_ERR_BASE+ 12)
/**  ����û�д���*/
#define		MP_ERR_OBJECT_NOT_NEW			(u16)(MP_ERR_BASE+ 13)
/**  free ����*/
#define		MP_ERR_MEM_RELEASE_FAIL			(u16)(MP_ERR_BASE+ 14)
#define		MP_ERR_PACKET_TYPE_UNKNOWN		(u16)(MP_ERR_BASE+ 15)
/**  SN������ ����Ҫ�ȴ�*/
#define		MP_ERR_TAKE_PACKET_WAIT			(u16)(MP_ERR_BASE+ 16)
#define		MP_ERR_PACKET_SN_ERR			(u16)(MP_ERR_BASE+ 17)
#define		MP_ERR_SOCKET_RECV				(u16)(MP_ERR_BASE+ 18)
/**  rtcp ���ķ���ʧ��*/
#define		MP_ERR_RTCP_PACKET				(u16)(MP_ERR_BASE+ 19)	
/**  send֮ǰûbind*/
#define		MP_ERR_SOCKET_NOT_BIND			(u16)(MP_ERR_BASE+ 20)	
/**  ����RTCP�ش�����֧��*/
#define		MP_ERR_RTCP_REQ_NOT_SUPPORT		(u16)(MP_ERR_BASE+ 21)	
/**  û���ҵ���PACKET*/
#define		MP_ERR_RESEND_PACKET_NOT_FOUND	(u16)(MP_ERR_BASE+ 22)
/**  epoll����ʧ��*/
#define		MP_ERR_EPOLL_FAIL				(u16)(MP_ERR_BASE+ 23)
/**  setsockopt fail*/
#define		MP_ERR_SOCKET_SET_OPT			(u16)(MP_ERR_BASE+ 24)	
/**  NOT SUPPORT type*/
#define		MP_ERR_NOT_SUPPORT				(u16)(MP_ERR_BASE+ 25)	

/**  ʵ��δִ��Create �������������*/
#define		MP_ERR_NOT_CREATE				(u16)(MP_ERR_BASE+ 26)
/**  NODE ALREADY ADD	*/
#define		MP_ERR_NODE_ALREADY_ADD			(u16)(MP_ERR_BASE+ 27)

#define		MP_ERR_OUT_OF_NODE				(u16)(MP_ERR_BASE+ 28)

#define		MP_ERR_NODE_NOT_IN				(u16)(MP_ERR_BASE+ 29)
/**  PACKET parse error*/
#define         MP_ERR_PACKET                   (u16)(MP_ERR_BASE +30)


#define		MP_PACKET_TYPE_KEDA				1
#define		MP_PACKET_TYPE_OTHER_NOTRTP  	100
#define		MP_PACKET_TYPE_OTHER_RTP    	104
#define		MIN_STATIS_TIME_INTERVAL		100
#define		MAX_STATIS_TIME_INTERVAL		1000
#define		DEFAUL_KEDA_PACKET_TIME_INTERVAL	10
#define		TIME_MAX_DIFFER						300
#define		MAX_TIME_TO_FORCE_ADJUST			1000

enum
{
	FRAME_TYPE_P		= 0,
	FRAME_TYPE_I		= 1,
	FRAME_TYPE_UNKNOW	= 2     ///< ��Ϊmediapack�յ��Ķ��ǰ����޷�������Ƿ�ؼ�֡����Ϊtype����FRAME_TYPE_UNKNOW
} ;

/** ����udp���ṹ*/
typedef struct 
{
	u64 m_qwTimeStamp;      ///< ʱ���
	u8	m_nStreamId;        ///< ����

	u8	m_byFrameBorder;	///< ֡�߽�
	u8  m_byPayload;        ///< �غ�����
	u8  m_byKeyFrame;       ///< �Ƿ�ؼ�֡
	u16 m_wSequence;        ///< ���ڸ�·�������
	u32 m_dwReserve ;        
	u32	m_dwSSRC;

	u32 m_nDataLen;           ///< ���ݳ���
	u8* m_pchData;            ///< ���ݰ�ָ��
} TUdpPack;

/** 
 * ������Ὣ�ش��������7����˲�������̫С������ʱ�������
 * �ش�����ĵ�λ�����кţ�������medianet������ʱ���
 */
typedef struct tagMpRSParam
{
	u16  m_wFirstTimeSpan;   ///< ��һ���ش�����   
	u16  m_wSecondTimeSpan;  ///< �ڶ����ش�����
	u16  m_wThirdTimeSpan;   ///< �������ش�����
	u16  m_wRejectTimeSpan;  ///< ���ڶ�����ʱ����
} TMpRSParam;

typedef struct tagMpCfgParam
{
    s32  m_nCPUaffinity;
} TMpCfgParam;

u16 MediaPacketInit(TMpCfgParam* ptParam = NULL);  ///< ʹ��֮ǰ������øú���
u16 MediaPacketRelease();///< ʹ����ϱ�����øú����������
u64 MpGetSysTime(BOOL32 bPrintTick = FALSE);

class CSend ;
class CRecv ;

/**
 *@brief  udp pack���ص�
 *@param[out]  TUdpPack & tPack �յ���udp���ص�
 *@param[out]  KD_PTR pvContext  ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
 *@return     void
 *@note    
 */
typedef void (* UdpPackCallback )(TUdpPack & tPack, KD_PTR pvContext);

class CMediaPackRecv  
{
	public:
		CMediaPackRecv();
		virtual ~CMediaPackRecv();

		/**
		 *@brief  clear ���ն������
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�   
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE );

		/**
		 *@brief   �������ն���
		 *@param[in]  u8 byPacketType MP_PACKET_TYPE_KEDA  MP_PACKET_TYPE_OTHER
		 *@param[in]  u32 dwBuffSize buff ��С
		 *@param[in]  UdpPackCallback pCallback ���ص�����
		 *@param[in]  KD_PTR pvContext ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 Create(u8 byPacketType, u32 dwBuffSize, UdpPackCallback pCallback, KD_PTR pvContext);

		/**
		 *@brief  SetRTPAddr ����rtp�����յ�ַ
		 *@param[in]  u32 dwIp  rtp�����յ�ַ
		 *@param[in]  u16 wPort rtp�����ն˿�
		 *@param[in]  u8 byStreamID ��ID��UdpPackCallback�ص���ID��Ӧ
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRTPAddr(u32 dwIp, u16 wPort, u8 byStreamID);

		/**
		 *@brief  ����rtcp���շ���ַ
		 *@param[in]  u32 dwIp rtcp�����յ�ַ
		 *@param[in]  u16 wPort rtcp�����ն˿�
		 *@param[in]  u32 dwDestIP ���͵�ַ
		 *@param[in]  u16 wDestPort ���Ͷ˿�
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRTCPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwDestIP, u16 wDestPort);

		/**
		 *@brief  ��ʼ����
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 StartRecv();
		/**
		 *@brief  ֹͣ����
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 StopRecv();

		/**
		 *@brief  �����ش�����
		 *@param[in] TMpRSParam tRsParam �ش�һЩ����
		 *@param[in] BOOL32 bReSend �ش����أ�Ĭ�ϴ�
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRSParam(TMpRSParam tRsParam, BOOL32 bReSend = TRUE);

		/**
		 *@brief  ��ӡ���н��ն���
		 *@retval NULL
		 *@note    
		 */
		void PrintInfo();

	private:

		CRecv 		*	m_pcRecv;
		SEMHANDLE		m_hSem ;

		BOOL32			m_bSemCreate ;
		BOOL32			m_bCreate ;

};


class CMediaPackSend  
{
	public:
		CMediaPackSend();
		virtual ~CMediaPackSend();
		/**
		 *@brief  clear ���Ͷ������
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�   
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE );

		/**
		 *@brief   �������Ͷ���
		 *@param[in]  u8 byPacketType MP_PACKET_TYPE_KEDA  MP_PACKET_TYPE_OTHER
		 *@param[in]  u32 dwBuffSize buf��С��������ٸ���
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 Create(u8 byPacketType, u32 dwBuffSize);

		/**
		 *@brief  SetRTPAddr ����rtp�����͵�ַ
		 *@param[in]  u32 dwLocalIp  rtp���ͱ���IP
		 *@param[in]  u16 wLocalPort ���ض˿�
		 *@param[in]  u32 dwDestIp   send Ŀ���ַ
		 *@param[in]  u16 wDestPort  sendĿ��˿�
		 *@param[in]  u32 dwSockType SOCK_DGRAM  SOCK_RAW
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRTPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwDestIp, u16 wDestPort, u32 dwSockType);

		/**
		 *@brief  ����SOCK_RAW ģʽ ��ַ
		 *@param[in]  u32 dwSrcIp  ԴIP
		 *@param[in]  u16 wSrcPort Դport
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRtpSrcAddr(u32 dwSrcIp, u16 wSrcPort);

		/**
		 *@brief  ����rtcp��ַ
		 *@param[in]  u32 dwIp rtcp�����յ�ַ
		 *@param[in]  u16 wPort rtcp�����ն˿�
		 *@param[in]  dwSockType SOCK_DGRAM  SOCK_RAW
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRTCPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwSockType);

		/**
		 *@brief  ����SOCK_RAW ģʽ rtcp ��ַ
		 *@param[in]  u32 dwSrcIp  ԴIP
		 *@param[in]  u16 wSrcPort Դport
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRtcpSrcAddr(u32 dwSrcIp, u16 wSrcPort);

		/**
		 *@brief  send ���ݰ�
		 *@param[in]  u8 * pbData ���ݰ�
		 *@param[in]  u16 wDataSize ����
		 *@param[in]  u32 dwDestIp 0������SetRTPAddr���õĵ�ַ
		 *@param[in]  u16 wDestPort 0������SetRTPAddr���õĶ˿�
		 *@param[in]  BOOL32 bCache ����һ����
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 Send(u8 * pbData, u16 wDataSize, u32 dwDestIp = 0, u16 wDestPort = 0, BOOL32 bCache = TRUE);

		/**
		 *@brief  ��ӡ���з��Ͷ���
		 *@retval NULL
		 *@note    
		 */
		void PrintInfo();
		/**
		 *@brief  �����ش�����
		 *@param[in] u16 wTimeSpan �ش�ʱ����
		 *@param[in] BOOL32 bReSend �ش����أ�Ĭ�ϴ�
		 *@retval    MP_ERROK �ɹ�
		 *@retval    MP_* ���������б�  
		 *@note    
		 */
		u16 SetRSParam(u16 wTimeSpan, BOOL32 bReSend = TRUE);
	private:

		CSend * m_pcSend ;

		SEMHANDLE		m_hSem ;

		BOOL32			m_bCreate ;
		BOOL32			m_bSemCreate ;
};


} ///<  namespace

#endif ///<  !defined(AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_)
