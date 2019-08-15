/**
* @file		kdslib.h
* @brief		kds�ļ���ʽͷ�ļ�
* @author	���
* @date		2007-04-11
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/

#if !defined(AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_)
#define AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_

#include "osp.h"

/** �����붨��*/
#define				KDS_ERR_BASE				7000								///<���������ʼֵ
#define				KDS_ERR_NOERR				(u16)(0)							///<�����ɹ�
#define				KDS_ERR_OPENFAIL			(u16)(KDS_ERR_BASE+		1)		///<���ļ�ʧ��
#define				KDS_ERR_INIT_FAIL			(u16)(KDS_ERR_BASE+		2)		///<���ʼ��ʧ��
#define				KDS_ERR_NOMEM				(u16)(KDS_ERR_BASE+		3)		///<û���ڴ�
#define				KDS_ERR_FILE_OP_ERR		(u16)(KDS_ERR_BASE+		4)		///<�ļ�����ʧ��
#define				KDS_ERR_PARAM				(u16)(KDS_ERR_BASE+		5)		///<��������
#define				KDS_ERR_NOT_PERMISSION_OP	(u16)(KDS_ERR_BASE+		6)		///<������Ĳ���
#define				KDS_ERR_PAYLOAD_CHANGE		(u16)(KDS_ERR_BASE+		7)		///<ý���غ����͸ı�
#define				KDS_ERR_NOT_KDS			(u16)(KDS_ERR_BASE+		8)		///<����kds�ļ�
#define				KDS_ERR_POSITION_ERR		(u16)(KDS_ERR_BASE+		9)		///<��λ��ʱ�����
#define				KDS_ERR_FILE_REACH_END		(u16)(KDS_ERR_BASE+		10)		///<�����ļ�β
#define				KDS_ERR_UNKNOWN			(u16)(KDS_ERR_BASE+		11)		///<δ֪����
#define				KDS_ERR_SMALL_BUFF		(u16)(KDS_ERR_BASE+		12)		///<�ϲ���Ļ���̫С
#define				KDS_ERR_PACK_TIME_ERR		(u16)(KDS_ERR_BASE+		13)		///<���ݰ���ʱ�������
#define				KDS_ERR_RESUME_FAIL	    (u16)(KDS_ERR_BASE+		14)		///<�ָ�ʧ�ܣ���ǰδ��
#define				KDS_ERR_FILE_NOT_OPEN		(u16)(KDS_ERR_BASE+		15)		///<�ļ�δ��
#define				KDS_ERR_PLAY_STATE_ERR		(u16)(KDS_ERR_BASE+		16)		///<����״̬����
#define				KDS_ERR_CALLBACK_NOT_SET	(u16)(KDS_ERR_BASE+		17)		///<�ص�����û������
#define				KDS_ERR_METHOD_NOTSUPPORT  (u16)(KDS_ERR_BASE+		18)		///<������֧��
#define				KDS_ERR_FILEVER_TOO_LOW    (u16)(KDS_ERR_BASE+       19)		///<�ļ��汾̫��
#define				KDS_ERR_TIMER_INITAL_ERR   (u16)(KDS_ERR_BASE+       20)		///<��ʱ����ʼ��ʧ��
#define				KDS_ERR_NOMORESPACE_FORINDEX  (u16)(KDS_ERR_BASE+     21)		///<�����ռ䲻����Ŀǰδʹ��

/** �ļ�֧�ֵ����������*/
#define				MAX_KDS_STREAM_NUM			10						

/** �����������С*/
#define				MAX_KDS_PACK_SIZE				5 * 1024

/** ý����չ��Ϣ��󳤶�*/
#define				MAXLEN_MEDIAINFO_NAME         1024

/** ����ı���������󳤶�*/
#define				KDS_MAXLEN_ENCODINGNAME     8

/** �ļ��е����ݰ����Ͷ���*/
#define				KDS_PACKET_TYPE_UNKNOWN			0			///<��Ч�İ�����
#define				KDS_PACKET_TYPE_KEDA				1			///<Keda�������ݰ�
#define				KDS_PACKET_TYPE_OTHERNOTRTP		100			///<��rtp���ݰ�
#define				KDS_PACKET_TYPE_OTHERRTP			104			///<�⳧�̵�rtp���ݰ�

/** ���ݰ�ͷ��С*/
#define				KDS_PACK_UNIT_SIZE				19

/** ����дģʽ����*/
#define				KDS_WRITE_MODE_WRITE			1				///<���̷���Ϊͨ���ļ�ϵͳ��ʹ��write��ʽд����
#define				KDS_WRITE_MODE_WRITEV			2				///<���̷���Ϊͨ���ļ�ϵͳ��ʹ��writev��ʽд����
#define				KDS_WRITE_MODE_VBFS           3				///<���̷���Ϊvbfs�ļ���ʽ

/** ����״̬����*/
enum
{
	KDS_PLAY_STATE_STOP				= 1,							///<ֹͣ״̬
	KDS_PLAY_STATE_START				= 2,							///<��ʼ״̬
	KDS_PLAY_STATE_PAUSE				= 3							///<��ͣ״̬
};

/** �ص��¼�����*/
enum
{
	KDS_CALLBACK_PLAYING			= 1,								///<��ʼ����
	KDS_CALLBACK_FILE_END			= 2								///<���ŵ��ļ�β
};

/** ����ģʽ����*/
enum EKdsPlayMode {
		KDS_NORMAL_PLAY         = 0,								///<��������
		KDS_FAST_PLAY           = 1,								///<���
		KDS_SLOW_PLAY           = 2								///<����
} ;

/** ���ʼ���ṹ��*/
typedef struct  
{
	BOOL32	m_bTakeSem ;					///<�Ƿ���������ļ�
} TKdsInit;

/**
 *@brief �ڴ��ͷ�
 *@param[in]		u32 mem  ��Ҫ�ͷŵ��ڴ��ַ
 *@param[in]		u32 context ������
 *@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note	
 */
typedef s16 (*KdsMemRelease)(u32 mem, u32 context);

/** �ڴ�ṹ�嶨��*/
typedef struct 
{
	KdsMemRelease	m_pfCB;						///<�ڴ��ͷź���
	u32				m_mem;					///<��Ҫ�ͷŵ��ڴ��ַ
	u32				m_context;				///<������
} TKdsMem;

/** �ļ���չý����Ϣ*/
typedef struct
{
	s8      m_achManuFacture[MAXLEN_MEDIAINFO_NAME+1];		///<��չý����Ϣ��Ŀǰ��Ҫ����������������Ϣ
}TKdsExtFileInfo;

/** ��Ƶý����������Ϣ�ṹ��*/
typedef struct
{
	s32    m_nWidth;						///<ͼ���
	s32    m_nHeight;					///<ͼ���
	s32    m_nFrameRate;					///<֡��
	s32    m_nBitRate;					///<������
	s32    m_nKeyFrameIntvl;				///<�ؼ�֡�����������֡����һ���ؼ�֡
	s32    m_nClockRate;					///<ʱ�Ӳ����ʣ�ȡֵ��Χ: 1000 or 90000
}TVideoStreamProperty;

/** ��Ƶý����������Ϣ�ṹ��*/
typedef struct
{
	s32    m_nSampleRate;                  ///<������
	s32    m_nClockRate;                   ///<ʱ�Ӳ����ʣ�ȡֵΪ 1000 or m_nSampleRate
	s32    m_nChannelsNum;                 ///<������
}TAudioStreamProperty;

/** ý����������Ϣ�ṹ��*/
typedef struct  
{
	u8   m_byPayloadType;								///<�غ�����
	s8   m_szEncodingName[KDS_MAXLEN_ENCODINGNAME+1];		///<��������
	union
	{
		TVideoStreamProperty  m_tVideoStreamProperty;	///<��Ƶ��������Ϣ
		TAudioStreamProperty  m_tAudioStreamProperty;	///<��Ƶ��������Ϣ
	};
}TKdsStreamProperty;

/** �ļ���Ϣ�ṹ��*/
typedef struct 
{
	u8      m_byFileVer;										///<�ļ��汾��
	u32     m_dwStartTime;									///<�ļ��е�һ�����ݰ�ʱ���,��λ����
	u32		m_dwRecTime ;										///<¼���ļ�����ʱ��,��λ��
	u32		m_dwMaxPackSize ;									///<¼���ļ���������С����λ�ֽ�
	u8		m_byMediaCount ;									///<¼���ļ�������Ƶ������
	u8		m_byPackType ;									///<¼���ļ��а�������,����KDS_PACKET_TYPE_KEDA
	TKdsStreamProperty m_atStreamProperty[MAX_KDS_STREAM_NUM];    ///<��������Ϣ
	TKdsExtFileInfo m_tKdsExtFileInfo;							///<�ļ�����չ��Ϣ
	
} TKdsFileInfo;

/** ����Ϣ�ṹ��*/
typedef struct 
{
    u64 m_qwTimeStamp;					///<���ݰ���ʱ���,��λ����
    u8	m_nStreamId;						///<���ݰ�������ý�������
	
	u8	m_byFrameBorder;					///<֡�߽��ʶ��0bit : 1-֡�߽磬0-��֡�߽�, 1bit : 1-����һ֡�ĵ�һ����, 0-��һ֡�ĵ�һ����
    u8  m_byPayload;						///<���ݰ����غ�����
    u8  m_byKeyFrame;						///<���ݰ��Ƿ����ڹؼ�֡
    u16 m_wSequence;						///<���ݰ������к�
	u16 m_wReserve ;						///<�����ֶ�

    u8* m_pchData;						///<���ݰ�ָ��
    u32 m_nDataLen;						///<���ݳ���

} TKdsPacketInfo;

/**
 *@brief	���ݰ��ص���������ʹ�ÿ���Զ�������ʱ����
 *@param[out]		KD_PTR pvContext  ������
 *@param[out]		u32 dwStatus      ����״̬
 *@param[out]		TKdsPacketInfo * ptPack  ��Ҫ���ŵ����ݰ���Ϣ
 *@return		
 *@ref
 *@see
 *@note	
 */
typedef void (* KdsProcCallBack)(KD_PTR pvContext, u32 dwStatus, TKdsPacketInfo * ptPack);


/**
 *@brief	��ĳ�ʼ������
 *@param[in]		TKdsInit * ptParam  ��ĳ�ʼ������
 *@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note	�ýӿڱ�����ʹ��kdslib��������κι���֮ǰ���ɹ�����
 */
u16 KdsInit(TKdsInit * ptParam);

/**
 *@brief	��ķ���ʼ������
 *@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note	������Ҫʹ��kdslib��ʱ��ʹ�øýӿ��ͷ������Դ
 */
u16 KdsRelease();

/**
 *@brief	�����ļ��汾��Ϣ
 *@param[in]		const u8 byVer  ��Ҫ�������ļ��汾��
 *@param[out]		u8& byMajor     �ļ������汾��
 *@param[out]		u8& byMinor     �ļ��Ĵΰ汾��
 *@return		
 *@ref
 *@see
 *@note	������Ҫʹ��kdslib��ʱ��ʹ�øýӿ��ͷ������Դ
 */
void KdsParseFileVer(const u8 byVer, u8& byMajor, u8& byMinor);

class CKdsLibFile ;

/** ��дkds�ļ����ඨ��*/
class CKdsLib  
{
public:
	CKdsLib();
	virtual ~CKdsLib();

	/**
	*@brief		��������kds�ļ�����Դ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ��ø����е������ӿ�֮ǰ������
	*/
	u16 Create();
	
	/**
	*@brief		��ָ�����ļ�׼��д��
	*@param[in]	s8 * pbFile  ��Ҫд��������ļ�������������·��
	*@param[in]	u8 byPackType  ���ݰ����ͣ�����KDS_PACKET_TYPE_KEDA
	*@param[in]	BOOL32 bNoBuff TRUE:���ݰ������棬ֱ��д���̣�
	*                            FALSE:�����ݰ����л��棬������ʱ��д�����
	*@param[in]	u32 dwFileStartAbsTime �ļ��Ŀ�ʼʱ�䣬����ʱ�䣬��λ��
	*@param[in]	u8 WriteMode  д���̷�����ģʽ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿ���Create()֮����ã������׼��Ҫд�����ݣ����һ��CKdsLibʵ������BeginRead����ͬʱ����
	*/
	u16 BeginWrite(s8 * pbFile, u8 byPackType , BOOL32 bNoBuff , u32 dwFileStartAbsTime = 0, u8 WriteMode = KDS_WRITE_MODE_WRITE);
	
	/**
	*@brief		��ָ�����ļ�׼����ȡ
	*@param[in]	s8 * pbFile  ��Ҫ��ȡ�������ļ�������������·��
	*@param[in]	BOOL32 bReverseRead �Ƿ��Ե��ŵķ�ʽ���ļ�
	*@param[in]	KdsProcCallBack inCallback  ���ݰ��ص�����
	*@param[in]	KD_PTR pvContext  �ص������е������Ĳ���
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿ���Create()֮����ã������׼��Ҫ��ȡ���ݰ������һ��CKdsLibʵ������BeginWrite����ͬʱ����
	*/
	u16 BeginRead(s8 * pbFile, BOOL32 bReverseRead = FALSE, KdsProcCallBack inCallback = NULL , KD_PTR pvContext = NULL);

	/**
	*@brief		��ʼ�Զ���ȡ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�����Զ���ȡ֮�����ݰ��ᶨʱͨ��BeginRead�ӿ������õ�KdsProcCallBack�ص��������͸��ϲ�ҵ��
	*             �ýӿ��ڵ�����BeginRead()֮���ʹ��              
	*/
	u16 StartAutoRead();

	/**
	*@brief		ֹͣ�Զ���ȡ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ��
	*/
	u16 StopAutoRead();

	/**
	*@brief		��ͣ�ļ���ȡ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ��
	*/
	u16 Pause();

	/**
	*@brief		�ָ��ļ���ȡ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����Pause()֮��ʹ��
	*/
	u16 Resume();
	
	/**
	*@brief		��ȡ�ļ���Ϣ
	*@param[out]	TKdsFileInfo * tKdsFileInfo �ɹ���ȡ���ļ���Ϣ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ��
	*/
	u16 GetFileInfo(TKdsFileInfo * tKdsFileInfo ) ;

	/**
	*@brief		�����ļ���չ��Ϣ
	*@param[in]	TKdsExtFileInfo * tKdsExtFileInfo ��Ҫ���浽�ļ��е��ļ���չ��Ϣ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginWrite()֮���ʹ��
	*/
	u16 SetExtFileInfo(TKdsExtFileInfo * tKdsExtFileInfo);   //add by linlifen 2010.12.03
	
	/**
	*@brief		��λ������ʱ���������ݶ�ȡ
	*@param[in]	u64 llMSec  ��Ҫ��λ��ʱ�䣬���ʱ�䣬��λ����
	*@param[in]	BOOL32 bSeekToKeyFrame  �Ƿ�λ���ؼ�֡
									 TRUE����λ��<=llMSec����ӽ�llMSec�Ĺؼ�֡
									 FALSE����ȷ��λ��������llMSec
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ��
	*/
	u16 SetReadPos(u64 llMSec, BOOL32 bSeekToKeyFrame = TRUE);

	/**
	*@brief		���ò�������
	*@param[in]	EKdsPlayMode mode  ����ģʽ
	*@param[in]	u8 rate            �������ʣ�ȡֵ >=1
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ��
	*/
	u16 SetPlayRate(EKdsPlayMode mode , u8 rate);

	/**
	*@brief		���ý��������
	*@param[in]	u8 byStreamId      ý�������
	*@param[in]	TKdsStreamProperty& tStreamProperty  ý��������
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginWrite()֮���ʹ��
	*/
	u16 AddStream(u8 byStreamId, TKdsStreamProperty& tStreamProperty);
	
	/**
	*@brief		д���ݰ�
	*@param[in]	TKdsPacketInfo * ptPack  ��Ҫд������ݰ���Ϣ
	*@param[in]	BOOL32 bFlush   TRUE:������ˢ�µ������ϣ�FALSE:�ȵ��ڲ����������Զ�ˢ��
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginWrite()֮���ʹ��
	*/
	u16 WritePacket(TKdsPacketInfo * ptPack, BOOL32 bFlush = FALSE , TKdsMem * ptMem = NULL );
	
	/**
	*@brief		��ȡ���ݰ�
	*@param[out]	TKdsPacketInfo * ptPack  �ɹ���ȡ�����ݰ���Ϣ�����е�m_pchData���ڴ����ϲ�ҵ���ṩ
	*                                     m_nDataLen�����ڳɹ���ȡ���ݰ��󣬻ᱻ�޸�Ϊʵ�ʵ����ݰ�����
	*@param[in]	BOOL32 bOnlyKeyFrame  TRUE��ֻ��ȡ�ؼ�֡�����ݰ���FALSE����ȡ���е����ݰ�
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		�ýӿ��ڵ�����BeginRead()֮���ʹ�ã���������Ѿ�������StartAutoRead()����Ӧ����ʹ�øýӿ�
	*/
	u16 ReadPacket(TKdsPacketInfo * ptPack, BOOL32 bOnlyKeyFrame = FALSE);
	
	/**
	*@brief		�����ļ���д��
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		
	*/
	u16 EndWrite();

	/**
	*@brief		�ͷŷ����ļ��������Դ
	*@return		KDS_ERR_NOERR:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note		
	*/
	u16 Close();
private:

	CKdsLibFile * m_pcKdsFile ;
};

#endif // !defined(AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_)

