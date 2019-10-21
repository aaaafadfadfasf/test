/**
 * @file udp2asf.h
 * @brief    udp -> asf �ݲ�֧��˫��
 * @author    ���
 * @date 2007/08/25
 * @version  1.0
 * @copyright V1.0  Copyright(C) 2016 Keda All rights reserved.
 */
#if !defined(AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_)
#define AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_

#include "osp.h"
#include "kdvdef.h"
#include "kdmfileinterface.h"
#include "kdvmedianet.h"
#ifdef _WIN32
#pragma comment(lib, "kdmfileinterface.lib")
#endif

#define				UA_ERR_BASE						7300
/**udp2asf���� OK*/
#define				UA_ERROK						0
/**udp2asf���� û�д���*/
#define				UA_ERR_NOT_CREATE				UA_ERR_BASE + 1
/**udp2asf���� ��������*/
#define				UA_ERR_PARAM					UA_ERR_BASE + 2
/**udp2asf���� û��ʼ��*/
#define				UA_ERR_NOT_INIT					UA_ERR_BASE + 3
/**udp2asf���� ��ʼ��ʧ��*/
#define				UA_ERR_INIT_FAIL				UA_ERR_BASE + 4
/**udp2asf���� ������ʧ��*/
#define				UA_ERR_SEM_CREATE				UA_ERR_BASE + 5
/**udp2asf���� �ڴ�Խ��*/
#define				UA_ERR_OUT_OF_MEM				UA_ERR_BASE + 6
/**udp2asf���� ý�����Ͳ�֧��*/
#define				UA_ERR_MEDIATYPE_UNSUPPORT		UA_ERR_BASE + 7
/**udp2asf���� �����ı�*/
#define				UA_ERR_FORMAT_CHANGED			UA_ERR_BASE + 8
/**udp2asf���� �������ע���� 256*/
#define				UA_ERR_REACH_MAX_REG			UA_ERR_BASE + 9
/**udp2asf���� û���ҵ�ƥ���ע��ip��port*/
#define				UA_ERR_NOT_FIND_REG				UA_ERR_BASE + 10
/**udp2asf���� �ص�����NULL*/
#define				UA_ERR_NOT_FIND_CB				UA_ERR_BASE + 11
/**udp2asf���� port Ϊ0*/
#define				UA_ERR_OUT_OF_PORT				UA_ERR_BASE + 12
/**udp2asf���� ��frame*/
#define				UA_ERR_WAIT_FRAME				UA_ERR_BASE + 13
/**udp2asf���� �洢�ļ���ʽ��֧��*/
#define             UA_ERR_FILETYPE_UNSUPPORT       UA_ERR_BASE + 14

/** �����ַ�ṹ*/
typedef  struct  tagTU2ANetAddr 
{
	u32  m_dwIp;	///< ��ַ
	u16  m_wPort;	///< �˿ں�
}TU2ANetAddr;

enum
{
	FileType_ByFileName = 0,///< �����ļ������ж��ļ�����
	FileType_Asf,///< ����⴫����ļ���������asf�ļ�
	FileType_Mp4,///< ����mp4�ļ�
};

/**
 *@brief  �����ջص�����
 *@param[out]  u8* pPackBuf ������
 *@param[out]  u16 wPackLen ����
 *@param[out]  TU2ANetAddr* ptDstAddr ���͵�ַ
 *@param[out]  TU2ANetAddr* ptSrcAddr NULL
 *@param[out]  u64 qwTimeStamp 0
 *@param[out]  KD_PTR pContext  ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
 *@return     
 *@note       
 */
typedef void* (*U2AUdpPackRecvCallback)(u8* pPackBuf, u16 wPackLen, TU2ANetAddr* ptDstAddr, TU2ANetAddr* ptSrcAddr, u64 qwTimeStamp, KD_PTR pContext);

/**
 *@brief  ��֡�ص�
 *@param[out]  PFRAMEHDR pFrame �ص�֡��Ϣ
 *@param[out]  KD_PTR pContext  ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
 *@return     void
 *@ref        
 *@see
 */
typedef void* (*U2AFrameCB)(PFRAMEHDR pFrame, KD_PTR pContext);

/**
 *@brief  ע��pack���ջص�
 *@param[in] void* pRcvAddr ���յ�ַ
 *@param[in] U2AUdpPackRecvCallback pUdpCallBack �����ջص�
 *@param[in] KD_PTR pContext  ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
 *@retval    UA_ERROK �ɹ�
 *@retval    !UA_ERROK ʧ�ܣ��ο����������
 */
u16 U2ARegSndChannel(void* pRcvAddr, U2AUdpPackRecvCallback pUdpCallBack, KD_PTR pContext);

/**
 *@brief ɾ�����ջص�
 *@param[in] void* pRcvAddr ���յ�ַ
 *@retval    UA_ERROK �ɹ�
 *@retval    !UA_ERROK ʧ�ܣ��ο����������
 */
u16 U2AUnRegSndChannel(void* pRcvAddr) ;

/**
 *@brief  udp2asf��ʼ��
 *@retval    UA_ERROK �ɹ�
 *@retval    !UA_ERROK ʧ�ܣ��ο����������
 *@note
 */
u16 U2AInit();

/**
 *@brief  udp2asf�ͷ�
 *@retval    UA_ERROK �ɹ�
 *@retval    !UA_ERROK ʧ�ܣ��ο����������
 *@note    
 */
u16 U2ARelease();

class CGenAsf ;

class CUdp2Asf  
{
	public:
		CUdp2Asf();
		virtual ~CUdp2Asf();
		/**
		 *@brief  clear ����
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο����������  
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE);

		/**
		 *@brief   �����洢�ļ�
		 *@param[in]  s8 * pchFileName �ļ���
		 *@param[in]  u8 byFileType �ļ����� FileType_ByFileName���ݺ�׺ȷ��
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο���������� 
		 *@note    
		 */
	u16 Create(s8 * pchFileName, u8 byFileType = FileType_ByFileName, u32 qwCreateTime = 0);

		/**
		 *@brief   ����֡�ص��ӿ�
		 *@param[in]  U2AFrameCB pfCB֡�ص�����
		 *@param[in]  KD_PTR pContext ���ݸ��ݲ�ͬ���ͣ����ݲ�ͬ
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο���������� 
		 *@note    
		 */
		u16 Create(U2AFrameCB pfCB, KD_PTR pContext);

		/**
		 *@brief   ֹͣ¼��
		 *@param[in]  NULL
		 *@retval   void
		 *@note    
		 */
		u16 EndWrite();

		/**
		 *@brief   ���buf
		 *@param[in]  NULL
		 *@retval   void
		 *@note    
		 */
		u16 ResetBuf();


		/**
		 *@brief   ���� ��̬�غɵ� Playloadֵ
		 *@param[in]  u8 byStreamId ��ID
		 *@param[in]  u8 byRmtActivePT �ⳡ�Զ���payload
		 *@param[in]  u8 byRealPT ���ǹ�˾��payload
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο����������  
		 *@note    
		 */
		u16 SetActivePT(u8 byStreamId, u8 byRmtActivePT, u8 byRealPT);

		/**
		 *@brief   �����Ƿ����4k����,Ĭ��Ϊfalse��
		 *@param[in]  u8 byStreamId ��ID
		 *@param[in]  BOOL32 bis4k �Ƿ�4K
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο����������
		 *@note    
		 */
		u16 SetIs4k(u8 byStreamId, BOOL32 bis4k = false);

		/**
		 *@brief   input ����,ҵ����յ����ݺ�ֱ�Ӹ�����
		 *@param[in]  u8 * pbData ���ݰ�
		 *@param[in]  u16 wDataSize ����
		 *@param[in]  u64 qwTime ʱ���
		 *@param[in]  u8 byStreamId ��ID
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο����������
		 *@note    
		 */
		  u16 SetStreamSave(u8 byStreamId, u8 byAudioStreamNo,BOOL32 bIsSave);
		u16 InputUdp(u8 * pbData, u16 wDataSize, u64 qwTime, u8 byStreamId);

		/**
		 *@brief   ����ʱ��������ʡ�
		 *@param[in]  u8 byStreamId ��ID
		 *@param[in]  u32 dwSample ������
		 *@retval    UA_ERROK �ɹ�
		 *@retval    !UA_ERROK ʧ�ܣ��ο���������� 
		 *@note    
		 */
		u16 SetTimestampSample(u8 byStreamId, u32 dwSample);
	u16 SetUserData(u8* pDataBuffer, u32 dwBufferSize);
	u16 FileSetUserData(LPCSTR pFilePath, u8* pDataBuffer, u32 dwBufferSize);
	public:

	private:

		SEMHANDLE	m_hSem;

		BOOL32		m_bSemCreate;
		BOOL32		m_bCreate;

		CGenAsf *	m_pcGenAsf ;


};

#endif // !defined(AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_)
