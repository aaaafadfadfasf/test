/**
* @file		bslib.h
* @brief		ʹ��kdstor�ƴ洢�洢�ļ��Ŀ�
* @author	�����
* @date		2016-06-14
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/

#ifndef _BLOB_STOR_H_
#define _BLOB_STOR_H_

#include <stdio.h>
#include "kdvtype.h"

/** bslib��֧�ֵ�IP��ַ��󳤶�*/
#define   BS_MAXLEN_IPADDR           64

/** bslib��֧�ֵĻ��������󳤶�*/
#define   BS_MAXLEN_MACHINEID        64

/** bslib��֧�ֵ�������󳤶�*/
#define   BS_MAXLEN_ZONENAME         32

/** bslib��֧�ֵ��û�����󳤶�*/
#define   BS_MAXLEN_USERNAME         32

/** bslib��֧�ֵ�������󳤶�*/
#define   BS_MAXLEN_PASSWD           16

/** bslib��֧�ֵ��ļ�����󳤶�*/
#define   BS_MAXLEN_FILENAME         255

/** bslib��֧�ֵ�ý�����������*/
#define   BS_MAXNUM_STREAM           3

/** bslib��֧�ֵı���������󳤶�*/
#define	 BS_MAXLEN_ENCODINGNAME     8

/** bslib��֧�ֵ��ļ���չ��Ϣ����󳤶�*/
#define   BS_MAXLEN_FILEEXTINFO           1024

/** bslib��Ĵ����붨��*/
typedef enum
{
	BS_Ok = 0,							///<�����ɹ�
	
	BS_Err_Invalid_Argument = 1000,			///<��Ч����
	BS_Err_File_Not_Exist,					///<�ļ�������
	BS_Err_File_Exist,                    ///<�ļ��Ѿ�����
	BS_Err_Mount,							///<�����ƴ洢ʧ��
	BS_Err_Not_Init,						///<��û�б���ʼ��
	BS_Err_NotBs,							///<�����ƴ洢
	BS_Err_IOErr,							///<IO����
	BS_Err_IOTimeOut,						///<IO������ʱ
	BS_Err_File_Reach_End,					///<�����ļ�ĩβ
	BS_Err_Invalid_Oper,					///<��Ч����
	BS_Err_Read_File,                     ///<���ļ�ʧ��
	BS_Err_Invalid_FileFormat,				///<��Ч���ļ���ʽ
	BS_Err_No_Mem,						///<���ڴ�
	BS_Err_Buffer_Too_Small,				///<��ȡ��ʱ������̫С���������һ������
	BS_Err_File_Open,						///<�ļ���ʧ��
	BS_Err_Pack_TimeStamp,					///<���ݰ���ʱ�������
	BS_Err_Data,							///<���ݰ�����
	BS_Err_Not_Empty,						///<����ǿ�
	BS_Err_Lost_Connection,				///<���ƴ洢����
	BS_Err_No_Space,						///<�ƴ洢�޿��ÿռ�
	BS_Err_OutOfPower,					///<�����ϵ�����
	BS_Err_No_Buffer,						///<�ƴ洢�޻���
	BS_Err_Bad_Index,						///<�ļ�������
	BS_Err_File_Locked,					///<�ļ�������
	BS_Err_Method_NotSupport,				///<������֧��

	BS_Err_Unknown,
}EBS_ErrType;

/** bslib��֧�ֵ�IP��ַЭ�����Ͷ���*/
typedef enum
{
	BS_IpType_IPv4 = 0,				///<IP��ַΪIPv4Э������
	BS_IpType_IPv6,					///<IP��ַΪIPv6Э������
}EBS_IpType;

/** bslib��֧�ֵ�ý�������Ͷ���*/
typedef enum
{
	BS_StreamType_Unknown,				///<δ֪��ý��������
	BS_StreamType_Audio,				///<��Ƶ��
	BS_StreamType_Video,				///<��Ƶ��
	BS_StreamType_Invalid,				///<��Ч��ý��������
}EBS_StreamType;

/** bslib��֧�ֵ����ݰ����Ͷ���*/
typedef enum
{
	BS_PackType_Keda = 1,				///<���ݰ�ΪKedacom�������������Ͱ�֧�������ؼ�֡����ߵķ���
	BS_PackType_Other_NotRTP,			///<���ݰ�Ϊ��RTP����ʽ�����ݰ������⳧�̱�����
	BS_PackType_Other_RTP,				///<���ݰ�ΪRTP����ʽ�����ݰ������⳧�̱�����
	BS_PackType_Unknown,				///<δ֪�����ݰ�����
}EBS_PackType;

/** IP��ַ�Ķ���*/
typedef struct  
{
	EBS_IpType  m_eIpType;							///<IP��ַЭ������
	s8          m_szIpAddr[BS_MAXLEN_IPADDR+1];		///<IP��ַ�ַ���
}TBS_IPAddr;

/** �����ַ�Ķ���*/
typedef struct  
{
	TBS_IPAddr   m_tIpAddr;						///<IP��ַ��Ϣ
	u16          m_wPort;							///<�˿���Ϣ
}TBS_NetAddr;

/** ��ĳ�ʼ�������ṹ��*/
typedef struct
{
	u16          m_wNetAddrNum;							///<��Ҫ���ӵ��Ʒ�������ַ����
	TBS_NetAddr  *m_ptNetAddr;								///<��Ҫ���ӵ��Ƶ�ַ��Ϣ
	s8           m_szMachineID[BS_MAXLEN_MACHINEID+1];		///<�Ʒ������Ļ���ID�� һ��ΪUUID        
	s8           m_szZoneName[BS_MAXLEN_ZONENAME+1];			///<�Ʒ�����������
	s8           m_szUserName[BS_MAXLEN_USERNAME+1];			///<�����Ʒ��������û���
	s8           m_szPasswd[BS_MAXLEN_PASSWD+1];			///<�����Ʒ�����������
	u32          m_dwTimeOut;								///<�����Ʒ������ĳ�ʱʱ�䣬������ʱ��û���������Ʒ����������ʧ�ܷ���
	s32          m_nDataChunk;							///<�ƴ洢ʹ�õ�Erasure code���ݱ��������е����ݿ�������Ŀǰû�����ֵ���ƣ����Ϊ0�����ڲ�Ĭ������Ϊ4
	s32          m_nParityChunk;							///<�ƴ洢ʹ�õ�Erasure code���ݱ��������е�У���������ȡֵ��Χ[1,2]�����Ϊ0�����ڲ�Ĭ������Ϊ1
}TBS_InitParam;

/** ��־�ļ�������Ϣ*/
typedef struct
{
	u16          m_wLogNum;                               ///<����������־�ļ�����*/
	u32          m_dwLogSize;                             ///<������־�ļ�������С����λ��bytes*/
	s8           m_szLogName[BS_MAXLEN_FILENAME+1];			///<��������־�ļ���ǰ׺�������Ǿ���·��+�ļ���ǰ׺
}TBS_LogCfg;

/** �ļ���Ϣ�ṹ�嶨��*/
typedef struct
{
	u64   m_qwStartTime;							///<�ļ���ʼʱ�䣬��λ��
	u64   m_qwEndTime;							///<�ļ�����ʱ�䣬��λ��
	u64   m_qwFileSize;							///<�ļ���С����λ���ֽ�
}TBS_Stat;

/** �ƴ洢�ļ�ϵͳ��Ϣ�ṹ�嶨��*/
typedef struct 
{
	u32 m_dwBlockSize;							///<�ײ���С����λ���ֽ�
	u64 m_qwFreeSize;								///<ʣ��ռ��С����λ���ֽ�
	u64 m_qwTotalSize;							///<�ܿռ��С����λ���ֽ�
	EBS_ErrType m_eFsStatus;						///<ϵͳ״̬�Ƿ�����,BS_Ok--����������ֵ--�쳣������
}TBS_Statfs;

/**
 *@brief BSLIB��ĳ�ʼ������
 *@param[in]	const TBS_InitParam* ptInitParam bslib��ĳ�ʼ������
 *@param[in]	TBS_LogCfg* ptLogCfg ��־�ļ�����
 *@return	BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note  ������ʹ��BsLib��������κνӿ�֮ǰ����
 */
EBS_ErrType BS_Init(TBS_InitParam* ptInitParam, TBS_LogCfg* ptLogCfg);

/**
 *@brief BsLib��ķ���ʼ������
 *@return	BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note  ���øýӿ�֮�󣬽�������ʹ��BsLib����κ���������
 */
EBS_ErrType BS_Unint();

/**
 *@brief �ж��Ƿ�Ϊ�ƴ洢�ϵ��ļ�
 *@param[in]		const s8* pszFile ������·�����ļ���
 *@return		BS_Ok:���ƴ洢�ļ� ������:�ж�ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_IsBS(const s8* pszFile);

/**
 *@brief ��ȡ�ƴ洢�ļ�ϵͳ��Ϣ
 *@param[in]		const s8* pszDir �ƴ洢·��
 *@param[out]		TBS_Statfs * ptStatfs �ƴ洢�ļ�ϵͳ��Ϣ
 *@return		BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Statfs(const s8* pszDir, TBS_Statfs * ptStatfs);

/**
 *@brief ��ȡ�ƴ洢�ϵ�ָ���ļ�����Ϣ
 *@param[in]		const s8* pszFileName �ƴ洢�ϵ��ļ�����������·��
 *@param[out]		TBS_Stat * ptStat �ɹ���ȡ���ļ���Ϣ
 *@return		BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Stat(const s8* pszFileName, TBS_Stat * ptStat);

/**
 *@brief �ж��ƴ洢���Ƿ������Ӧ��Ŀ¼�����ļ�
 *@param[in]		const s8* pszDir �ļ�������·����
 *@return		BS_Ok:�ļ���Ŀ¼���� ������:ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Access(const s8* pszDir);

/**
 *@brief ɾ���ƴ洢�ϵ�ָ���ļ������ļ�����ָ��Ŀ¼
 *@param[in]		const s8* pszFileName  �ļ�������Ŀ¼
 *@return		BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note	�����Ҫɾ������Ŀ¼��ֻ����Ŀ¼Ϊ��ʱ������ɾ���ɹ�
 */
EBS_ErrType BS_Remove(const s8* pszFileName);

/**
 *@brief �������߽����ƴ洢�ϵ��ļ�
 *@param[in]		const s8* pszFileName  ������·�����ļ���
 *@param[in]		BOOL32 bLock  TRUE��������FALSE������
 *@return		BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_LockFile(const s8* pszFileName, BOOL32 bLock);

/**
 *@brief �������ƴ洢д������ʱ�ĳ�ʱʱ��
 *@param[in]		const u32 dwTimeOutMs  ��Ҫ���õĳ�ʱʱ�䣬��λ������
 *@return		BS_Ok:�ɹ� ������:ʧ��
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_SetTimeOut(const u32 dwTimeOutMs);

/**
 *@brief ��ȡbslib�ڲ�����ĸ��ֶ��������������ռ�õ��ڴ����
 *@param[out]		u64& llTotalObjectSize   ���ж�����ܴ�С
 *@param[out]		u32& dwWriteObjectNum    ����д���������
 *@param[out]		u32& dwReadObjectNum     ���ж����������
 *@return
 *@ref
 *@see
 *@note	
 */
void  BS_GetObjectUsedInfo(u64& llTotalObjectSize, u32& dwWriteObjectNum, u32& dwReadObjectNum);

/**
 *@brief ��ȡbslib�ڲ�����Ļ����ܴ�С������������
 *@param[out]		u64& llTotalBuffSize   ��������л����С
 *@param[out]		u32& dwTotalBuffNum    ��������л�������
 *@return
 *@ref
 *@see
 *@note	
 */
void  BS_GetBufferUsedInfo(u64& llTotalBuffSize, u32& dwTotalBuffNum);

/**
 *@brief ����bslib�ڲ������ƴ洢д��key-value,����,attachment֮�����ʱʱ��
 *@param[in]		const u64 llKeyValueDelayTimeMs   д��key-value֮��sleep��ʱ�䣬��λ����
 *@param[in]		const u64 llDataDelayTimeMs	   д��data֮��sleep��ʱ�䣬��λ����
 *@param[in]		const u64 llAttachMentDelayTimeMs  д��attachment֮��sleep��ʱ�䣬��λ����
 *@return
 *@ref
 *@see
 *@note	�ýӿ�ֻ���ڲ���ʱ������ҵ����ʹ��bslib��ʱ��Ӧ��ʹ�øýӿ�
 */
void  BS_TestDelay(const u64 llKeyValueDelayTimeMs, const u64 llDataDelayTimeMs, const u64 llAttachMentDelayTimeMs);

/** ���ݰ���Ϣ����*/
typedef struct 
{
	u64 m_qwTimeStamp;		///<���ݰ���ʱ�������λ����
	u8  m_byPayLoad;			///<���ݰ���ý������
	u8	m_byPackFlag;			///<���ݰ���ʶ	�� 0 bit: 0-����֡�߽磬1-��֡�߽�
							///<				�� 1 bit: 0-��һ֡�ĵ�һ����1-����һ֡�ĵ�һ��
							///<				�� 2 �� 3 bit: 0-P֡��1-I֡��2-Unknonw
							///<				�� 4 �� 7 bit: ����
	u16 m_wSequence;			///<���ݰ������к�
	u16 m_wReserve;			///<�����ֶ�
	
	u32 m_dwDataLen;			///<�洢���ݰ����ݵĻ����С���ڻ�ȡ��ʵ�����ݰ����ݺ󣬻ᱻ�޸�
	u8 *m_pchData;           ///<�洢���ݰ����ݵĻ���ָ�룬���ϲ�ҵ�����
} TBS_PackInfo;

/** �ļ�������Ϣ*/
typedef struct
{
	u32     m_dwFileVer;						///<�ļ��汾��
	u32     m_dwFileDuration;                 ///<�ļ�ʱ������λ����
	u32     m_dwFileFirstPackTime;				///<�ļ��еĵ�һ�����ݰ���ʱ�������λ����
	u32     m_dwFileTotalPackCount;			///<�ļ��е����ݰ�����
	u64		m_qwFileTotalSize;				///<�ļ���С����λ�ֽ�
	u32     m_dwMaxPackSize;					///<�ļ����������ݰ���С
	u32     m_dwMaxFrameSize;					///<���֡���ݴ�С
	u16     m_wMaxFramePackCount;				///<�������֡���������ݰ���Ŀ
	u8		m_byStreamCount;					///<�ļ���ý��������Ŀ
	u8      m_byPackType;                     ///<�ļ��д洢�����ݰ�����
}TBS_FileProperty;

/** ��Ƶ��������Ϣ*/
typedef struct
{
	s32    m_nWidth;							///<ͼ���
	s32    m_nHeight;							///<ͼ���
	s32    m_nFrameRate;						///<֡��
	s32    m_nBitRate;						///<������
	s32    m_nKeyFrameIntvl;					///<�ؼ�֡�����������֡�����һ���ؼ�֡
	s32    m_nClockRate;						///<��Ƶ����ʱ�Ӳ����ʣ�ȡֵ��1000 or 90000
}TBS_VideoStreamProperty;

/** ��Ƶ��������Ϣ*/
typedef struct
{
	s32    m_nSampleRate;						///<������
	s32    m_nChannelsNum;					///<������
	s32    m_nClockRate;						///<��Ƶ����ʱ�Ӳ����ʣ�ȡֵ��1000 or ��ʵ�Ĳ�����
}TBS_AudioStreamProperty;

/** ý����������Ϣ*/
typedef struct
{
	EBS_StreamType   m_eStreamType;				///<ý�������ͣ���Ƶor��Ƶ
	u8   m_byPayloadType;						///<ý�������غ�����
	s8   m_szEncodingName[BS_MAXLEN_ENCODINGNAME+1];		///<ý�������غ����Ͷ�Ӧ�ı�������
	u16  m_wReserved;									///<�����ֶ�
	union
	{
		TBS_VideoStreamProperty  m_tVideoStreamProperty;		///<��Ƶ����ý��������Ϣ
		TBS_AudioStreamProperty  m_tAudioStreamProperty;		///<��Ƶ����ý��������Ϣ
	};
}TBS_StreamProperty;

/** �ļ���չ��Ϣ*/
typedef struct
{
	u32     m_dwFileExtInfoLen;					///<�ļ���չ��Ϣ��ʵ�ʳ���
	u8      m_byFileExtInfo[BS_MAXLEN_FILEEXTINFO];	///<�ļ���չ��Ϣ
}TBS_FileExtInfo;

/** ���ƴ洢д����ʱ�Ĳ���*/
typedef struct
{
	u32    m_dwDataFlushFrequence;			///<����ˢ�µ��Ʒ�������Ƶ�ʣ���λ:���룬��Χ:[1 -- �ļ�����ʱ��]*/
	u32    m_dwDataFlushUnitSize;			///<Ŀǰû��ʹ��
	u32    m_dwDataChunks;				///<�ƴ洢ʹ�õ�Erasure code���ݱ��������е����ݿ�������Ŀǰû�����ֵ���ƣ����Ϊ0�����ڲ�Ĭ������Ϊ4							
	u32    m_dwParityChunks;				///<�ƴ洢ʹ�õ�Erasure code���ݱ��������е�У���������ȡֵ��Χ[1,2]�����Ϊ0�����ڲ�Ĭ������Ϊ1
}TBS_FileWriteOptions;

class CBSReadImp;

/** ��ȡ�ƴ洢�ϵ��ļ����ඨ��*/
class CBSRead
{
public:
	CBSRead();
	virtual ~CBSRead();

	/**
	*@brief ���ݸ����������ļ������Ƿ񵹷ŵĲ��������ƴ洢�ϵ��ļ�
	*@param[in]		const s8* pszFileName   ��Ҫ�򿪽��ж�ȡ�������ļ���
	*@param[in]		BOOL32 bReverseRead     TRUE���ļ��򿪺���Ҫ�ɺ���ǰ��ȡ���ݰ���FALSE��������ǰ�����ȡ�ļ�
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType Open(const s8* pszFileName, BOOL32 bReverseRead = FALSE);

	/**
	*@brief ��ȡ�ļ��е����ݰ�
	*@param[out]		TBS_PackInfo * ptPack   �ɹ���ȡ�����ݰ���Ϣ�����е����ݰ������ֶλᱻ����Ϊʵ�ʵ����ݰ�����
	*@param[in]		BOOL32 bOnlyKeyFrame    TRUE��ֻ��ȡ���ڹؼ�֡�����ݰ���FALSE����ȡ����֡�����ݰ�
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ�����Open֮��ʹ��
	*/
	EBS_ErrType Read(TBS_PackInfo * ptPack, BOOL32 bOnlyKeyFrame = FALSE);

	/**
	*@brief ��λ���ļ��е�ĳ��λ�ý��ж�ȡ
	*@param[in]		const u64 llMSec   ��Ҫ��λ��ʱ��ƫ�ƣ�Ϊ���ʱ�䣬��λ�����룬
	*@param[in]		BOOL32 bSeekToKeyFrame    TRUE����λ��<=llMSecʱ�����һ���ؼ�֡��FALSE��ֱ�Ӷ�λ��������ʱ��λ��
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ�����Open֮��ʹ��
	*/
	EBS_ErrType Seek(const u64 llMSec, BOOL32 bSeekToKeyFrame = TRUE);

	/**
	*@brief ��ȡ�ļ���������Ϣ
	*@param[out]		TBS_FileProperty * ptFileProperty  �ɹ���ȡ�����ļ�������Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ�����Open֮��ʹ��
	*/
	EBS_ErrType GetFileProperty(TBS_FileProperty * ptFileProperty);

	/**
	*@brief ��ȡ�ļ��е�ý����������Ϣ
	*@param[in]		const u8 byStreamIndex  ��Ҫ��ȡ��ý�������
	*@param[out]		TBS_StreamProperty * ptStreamProperty  �ɹ���ȡ��ý����������Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ�����Open֮��ʹ��
	*/
	EBS_ErrType GetStreamProperty(const u8 byStreamIndex, TBS_StreamProperty * ptStreamProperty);

	/**
	*@brief ��ȡ�ļ��е���չ��Ϣ
	*@param[out]		TBS_FileExtInfo * ptFileExtInfo �ɹ���ȡ���ļ���չ��Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڵ�����Open֮��ʹ��
	*/
	EBS_ErrType GetFileExtInfo(TBS_FileExtInfo * ptFileExtInfo);

	/**
	*@brief �رմ򿪵��ļ�
	*@return	BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڵ���֮�����Ҫ�����ļ����������µ���open�ӿڴ��ļ�
	*/
	EBS_ErrType Close();

protected:
	CBSReadImp * m_pcBSRead;
};

class CBSWriteImp;

/** ���ƴ洢д�����ݵ��ඨ��*/
class CBSWrite
{
public:
	CBSWrite();
	virtual ~CBSWrite();

	/**
	*@brief ��д��ʽ���������ļ��ȴ�д������
	*@param[in]		const s8* pszFileName   ��Ҫ�򿪽���д�����ݵ������ļ���
	*@param[in]		const u8 byPackType     ���ݰ�����,�μ�EBS_PackType
	*@param[in]		TBS_FileWriteOptions * ptFileWriteOptions  �ļ�дѡ��
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	�ýӿڱ����ڸ����������ӿڱ�����ǰ�ɹ�����
	*/
	EBS_ErrType Open(const s8* pszFileName, const u8 byPackType, TBS_FileWriteOptions * ptFileWriteOptions = NULL);

	/**
	*@brief ���ļ���д�����ݰ�
	*@param[in]		TBS_PackInfo * ptPackInfo  ��Ҫд������ݰ�����Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType Write(TBS_PackInfo * ptPackInfo);

	/**
	*@brief ���ļ������ý����������Ϣ
	*@param[in]		const u8 byStreamIndex  ý������ţ���0��ʼ
	*@param[in]		TBS_StreamProperty * ptStreamProperty ý����������Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType AddStream(const u8 byStreamIndex, TBS_StreamProperty * ptStreamProperty);

	/**
	*@brief �����ļ���չ��Ϣ
	*@param[in]		const TBS_FileExtInfo * ptFileExtInfo �ļ���չ��Ϣ
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType SetFileExtInfo(const TBS_FileExtInfo * ptFileExtInfo);

	/**
	*@brief �ر��ļ�
	*@return			BS_Ok:�ɹ� ������:ʧ��
	*@ref
	*@see
	*@note	ʹ�øýӿڹر��ļ��󣬽����ܶ��ļ������κ��޸�
	*/
	EBS_ErrType Close();

protected:
	CBSWriteImp  * m_pcBSWrite;
};

#endif