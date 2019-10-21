/*****************************************************************************
ģ����      : netpacket
�ļ���      : netpacket.h
����ļ�    : netpacket.c
�ļ�ʵ�ֹ���: netpacketģ��ӿ�����
����        : ��Ф�� 
�汾        : V1.0  Copyright(C) 2003-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2014/03/01  1.0         ��Ф��       Create
******************************************************************************/
#ifndef _NETPACKET_
#define _NETPACKET_

#ifdef _USE_OSPSMALL_
#include "osp_small.h"
#else
#include "osp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** ������ */
#define  NETPACKET_NO_ERROR                            (s32)(0)					///< �ɹ�����ֵ 
#define  NETPACKET_ERROR_BASE                          (s32)(16000)				///< ��������ʼ�ο�ֵ
#define  NETPACKET_ERROR_PARAM                         (NETPACKET_ERROR_BASE+1)	///< ��������
#define  NETPACKET_ERROR_NOCREATE                      (NETPACKET_ERROR_BASE+2)	///< δ����
#define  NETPACKET_ERROR_MAXCREATE                     (NETPACKET_ERROR_BASE+3)	///< �ﵽ��󴴽���Ŀ
#define  NETPACKET_ERROR_MEM                           (NETPACKET_ERROR_BASE+4)	///< �ڴ����
#define  NETPACKET_ERROR_NOKEYFRAME                    (NETPACKET_ERROR_BASE+5)	///< û�йؼ�֡
#define  NETPACKET_ERROR_UNSUPPORT                     (NETPACKET_ERROR_BASE+6)	///< ��֧��
#define  NETPACKET_ERROR_AGAIN                         (NETPACKET_ERROR_BASE+7)	///< ��Ҫ���»�ȡ�ð����ô����벢�Ǳ�ʾ������ʾ���ù��죩
#define  NETPACKET_ERROR_READSLOW                      (NETPACKET_ERROR_BASE+8)	///< ��Ҫ���¶�λ�ؼ�֡����ʾ���ù�����
#define  NETPACKET_ERROR_UNKNOW                        (NETPACKET_ERROR_BASE+9)	///< δ֪����
#define  NETPACKET_ERROR_SEMCREATE                     (NETPACKET_ERROR_BASE+10)///<������ʧ��
#define  NETPACKET_ERROR_BUF_FULL                      (NETPACKET_ERROR_BASE+11)///< ��ƬBUF����

#define IPC_MAX_RTPPACK_LENGTH     (1384)	///< һ��rtp����������󳤶ȣ�����RTP��չͷ�м���8�ֽڵ�NTPʱ������а�������Ӧ��8����1392��Ϊ1384��

typedef s32 (*PFMESSAGEPROC)(u32 dwId, u32 dwFrameID, void* pContext);

//֡�ṹ��
typedef struct tagNewFrameHdr
{
	u8     m_byMediaType; /*ý������*/
	u8    *m_pData;       /*���ݻ���*/
	u32    m_dwPreBufSize;/*m_pData����ǰԤ���˶��ٿռ䣬���ڼ�*/
	/* RTP option��ʱ��ƫ��ָ��һ��Ϊ12+4+12*/
	/* (FIXED HEADER + Extence option + Extence bit)*/
	u32    m_dwDataSize;  /*m_pDataָ���ʵ�ʻ����С�����С*/
	u8     m_byFrameRate; /*����֡��,���ڽ��ն�*/
	u8      m_byPadSize;/*������ݳ���*/
	u32    m_dwFrameID;   /*֡��ʶ�����ڽ��ն�*/
	u32    m_dwTimeStamp; /*ʱ���, ���ڽ��ն�*/
	u32    m_dwSSRC;      /*ͬ��Դ, ���ڽ��ն�*/
	u64    m_llNTPTime;   /*NTPʱ���*/
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
			u32       m_dwSample;/*��Ƶ������*/
			u16       m_wChannel;/*������*/
			u16       m_wBitsPerSample;/*λ��*/
		}m_tAudioParam;
	}x;
	u8   m_byStreamID;//PES���������Ƶ 0-��Ƶ��0 1-��Ƶ��1
}NEWFRAMEHDR,*PNEWFRAMEHDR;

typedef struct tagPackInfo
{
	u32    m_dwTS;				//netpacket
	u32    m_dwSSRC;    		
	u16    m_wSn;       		//netpacket
	u8     m_byMediaType;		//netpacket
	u8     m_byPrever;
	u8*    m_pbyBuffer;			//netpacket
	u16    m_wRtpPackSize;		//netpacket
	BOOL32 m_bFirstPackOfFrame;
	BOOL32 m_bLastPackOfFrame;
	u16    m_wPackNum;           //netpacket//�˰����ڵ�֡��Ӧ��RTP�����ܸ�����
	u64    m_llTimeStamps;       //netpacket//UTCʱ�䣬RTP����NVR�󣬽���ģ���ȡ����ϵͳʱ�䣻��λMS
	union
	{
		struct
		{
			BOOL32    m_bKeyFrame;    /*Ƶ֡���ͣ�I or P��*/
			u16       m_wVideoWidth;  /*��Ƶ֡��*/
			u16       m_wVideoHeight; /*��Ƶ֡��*/
		}m_tVideoParam;          //netpacket
		struct
		{
			u32       m_dwSample;           /*��Ƶ������*/
			u16       m_wChannel;           /*������*/
			u16       m_wBitsPerSample;     /*λ��*/
		}m_tAudioParam;
	}x;
}TPackInfo;

typedef struct tagNewPackInfo
{
	TPackInfo m_ptPackInfo;
	//netpacket
	BOOL32 m_bUsed;           
	BOOL32 m_bMark;           
	u8    m_byAudioMode;      /*��Ƶģʽ*/ 
	u8     m_byFrameRate;     
	s32    m_nNextKeyFramePos;
	u32    m_dwFrameID;       
	u32    m_dwUnStandardTS;  
	u64    m_llTimeStamps;  
	s32    m_nRefCount;		  ///< ���ü���
	void*  m_hPackSem;		      ///< ��
	void   (*NPFreePack)   (struct tagNewPackInfo*); ///< �ͷ��ڴ�
	void   (*NPPackAddRef) (struct tagNewPackInfo*); ///< �������ü���
	void*  m_pNewPackInfoAddr;

}TNewPackInfo;

typedef enum ENetPacketPSAudioMode
{
	NoAudio = 0x00,
	FirstAudio = 0x01,
	SecondAudio = 0x02,
	TwoAudio = 0x03,
}ENetPacketPSAudioMode;

//������ʽΪ����PS������
//MEDIA_TYPE_NULL��ʾ����Ƶ��������Ƶ
typedef struct tagNPPSInfo 
{
	u8   m_byVideoType;//��Ƶpayload
	u8   m_abyAudioType[2];//��Ƶpayload
}TNPPSInfo;
	  
typedef struct tagFrameExInfo
{
	u8*    m_pbyExBuf;//Ĭ��ΪNULL�����ڶ�����Ϣ�� ����������Ϣ������ˮӡ�ȣ��Լ��ٿ���
	u32    m_dwExBufLen;//Ĭ��Ϊ0��
	u8     m_byStreamId;//��id��Ĭ��Ϊ0����2·��Ƶ�Ļ�idΪ1
	BOOL32 m_bSingleNalu;//�Ƿ�Ϊ��nalu��Ĭ��ΪTRUE,��nalu,H264�а�ʱP֡��ȥ��������0001���ɽ���CPU����,��nalu�����bSingleNaluΪTRUE��ע���ͨ�Ƿ������⣩
}TFrameExInfo;

/*=============================================================================
	������		��NetPacketInit
	����		����ʼ��
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketInit();

/*=============================================================================
	������		��NetPacketUnInit
	����		������ʼ��
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketUnInit();

/*=============================================================================
	������		��NetPacketCreate
	����		����������
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwBufferSize    ���仺���С����λ���ֽڣ�
				                       ʵ���ڲ����仺������dwBufferSize���ڲ������dwBufferSize���϶��룬�����а���С����2��ָ���ݸ�����
				   TPSInfo* ptPSInfo   NULL����ʾ�����ʽΪES����ΪNULL����ʾ�����ʽΪPS
				   u32* pdwId          ����Id

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketCreate(u32 dwBufferSize, TNPPSInfo* ptPSInfo, u32* pdwId);


/**  
* @brief 		���û���ģʽ 
* @detail 		
* @param[in]    u32 dwId,    Id��ʶ
* @param[in]    BOOL32 bCoverMode ����ģʽ TRUE:�����а����壬������� 
                                           FALSE:�����а�ǰ���ʣ�໺�������Ƿ��㹻�����岻��ʱNetPacketInputFrame�����ش���
* @retval  		�μ������붨��
*/
s32 NetPacketSetMode(u32 dwId, BOOL32 bCoverMode);

/**  
* @brief 		�����а�ʱԤ������չͷ�ֶθ��� 
* @detail 		
* @param[in]    u32 dwId,    Id��ʶ
* @param[in]    u32 dwResExtHdrNum �а�ʱԤ������չͷ�ֶθ���
* @retval  		�μ������붨��
*/
s32 NetPacketSetResExtHdrNum(u32 dwId, u32 dwResExtHdrNum);

/**  
* @brief 		����һ֡���� 
* @detail 		���ӿ�Ϊý�����ã����������һ֡���ݣ�payload�����ߡ��Ƿ�ؼ�֡��ʱ�����֡�ʵȲ��������д
* @param[in]    u32 dwId        Id��ʶ
* @param[in]    PFRAMEHDR pFrame   ֡����
* @param[in]    TFrameExInfo*  pFrameExInfo  ֡������Ϣ                                        
* @retval  		�μ������붨��
*/
s32 NetPacketInputFrame(u32 dwId, PNEWFRAMEHDR pFrame, TFrameExInfo*  pFrameExInfo);

/*=============================================================================
	������		NetPacketGetFirstKeyFramePos
	����		����ȡ����������Ĺؼ�֡�İ����
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId           Id��ʶ
				   u16* pwSeq         ���ذ����				   
                   �ýӿ�Ϊ¼�����ã����ڻ�ȡ���������ϵĹؼ�֡��һ���İ���ţ���¼��ģ�����
	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetFirstKeyFramePos(u32 dwId, u16* pwSeq);

/*=============================================================================
	������		NetPacketGetLastKeyFramePos
	����		����ȡ���������µĹؼ�֡�İ����
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId           Id��ʶ
				   u16* pwSeq         ���ذ����				   
                   �ýӿ�Ϊ��������ã����ڻ�ȡ���������µĹؼ�֡��һ���İ���ţ������緢��ģ�����
	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetLastKeyFramePos(u32 dwId, u16* pwSeq);

/*=============================================================================
	������		NetPacketGetLastPackPosEx
	����		����ȡ���������µİ����
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId           Id��ʶ
				   u16* pwSeq         ���ذ����				   

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetLastPackPosEx(u32 dwId, u16* pwSeq);

/*=============================================================================
	������		NetPacketGetPacket
	����		����ȡһ������
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId              Id��ʶ
				   u16 wSeq              �����
				   TNewPackInfo* ptPackInfo ���ذ���Ϣ

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetPacket(u32 dwId, u16 wSeq, TNewPackInfo** pptPackInfo, u32* pdwFrameID);

/*=============================================================================
	������		NetPacketGetPacket
	����		����ȡһ������
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId              Id��ʶ
				   u16 wSeq              �����
				   TNewPackInfo* ptPackInfo ���ذ���Ϣ

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetPacketExt(u32 dwId, u16 wSeq, TNewPackInfo** ptPackInfo, u32* pdwFrameID);

/*=============================================================================
	������		NetPacketRelease
	����		���ͷ���Դ
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId      Id��ʶ				   

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketRelease(u32 dwId);

			   

/**
* @brief 		��ȡ�а�����״̬���������а���������ȡ�� 
* @detail 		���ӿ����������
* @param[in]    u32 dwId    Id��ʶ
* @param[out]   u8* byProgress    ���ͽ��Ȱٷֱ�0--�������� 100--�����Ѿ�ȫ������
* @retval  		�Ƿ������ TRUE ����ɣ� FALSE����δ������ɡ�
*/
u32 NetPacketCheckSendProgress(u32 dwId, u8* pbyProgress);

/*=============================================================================
	������		NetPacketSetPackLength
	����		�������а�����
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u16 wPackLength      �а����ȣ����ɳ���IPC_MAX_RTPPACK_LENGTH-1384��	

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetPackLength(u16 wPackLength);

/*=============================================================================
	������		NetPacketSetNTPExtence
	����		�������Ƿ����NTPʱ�����RTP��չͷ�У�Ŀǰ����H264��������ӣ����ڼ���8���ֽڵ���չͷ���а�������1392��Ϊ1384��
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId                Id��ʶ	
	               BOOL32 bAddNTPExtence   �Ƿ����				   

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetNTPExtence(u32 dwId, BOOL32 bAddNTPExtence);

/*=============================================================================
	������		NetPacketSetCallback
	����		������һ֡�����������Ϣ�ص�
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId                Id��ʶ	
	               PFMESSAGEPROC pMessageCallback   �ص�
				   void* pvContext         �ص�������

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetCallback(u32 dwId, PFMESSAGEPROC pMessageCallback, void* pvContext);

/*=============================================================================
	������		NetPacketSetPSCut
	����		������PS����Ƿ���
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId                Id��ʶ	
	               BOOL32 bStart           �Ƿ���

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetPSCut(u32 dwId, BOOL32 bStart);

/*=============================================================================
	������		NetPacketGetAudioSampleAndChannelInfo
	����		����ȡ��Ƶ�������Լ�������
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u8 byPayload	
	               u8 byAudioMode  
				   u32 *pdwSample  ���ز�����
				   u8* pbyChannel  ����������

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetAudioSampleAndChannelInfo(u8 byPayload, u8 byAudioMode, u32 *pdwSample, u8* pbyChannel);

/*=============================================================================
	������		NetPacketSetNoExtence
	����		�����ò�����չͷ
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               BOOL32 bNoExtence �Ƿ����չͷ(bNoExtenceΪTRUE��ʾ������չͷ)

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetNoExtence(BOOL32 bNoExtence);

/*=============================================================================
	������		NetPacketSetPSAudioMode
	����		������PS�������Ƶģʽ��Ĭ��ֻ�����1·��Ƶ
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
				   u32 dwId                Id��ʶ
	               ENetPacketPSAudioMode ePSAudioMode �����Ƶģʽ

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketSetPSAudioMode(u32 dwId, ENetPacketPSAudioMode ePSAudioMode);


/*=============================================================================
	������		NetPacketGetPacketData
	����		����ȡһ������
	�㷨ʵ��	������ѡ�
	����ȫ�ֱ�������
	�������˵����
	               u32 dwId              Id��ʶ
				   u16 wSeq              �����
				   TNewPackInfo* ptPackInfo ���ذ���Ϣ

	����ֵ˵�����μ������붨��
=============================================================================*/
s32 NetPacketGetPacketData(u32 dwId, u16 wSeq, TNewPackInfo** pptPackInfo, u32* pdwFrameID, u8* pbySendBuf, u16* bySendBufSize, u8 byTransHead);

#ifdef __cplusplus
}
#endif

#endif //_NETPACKET_
