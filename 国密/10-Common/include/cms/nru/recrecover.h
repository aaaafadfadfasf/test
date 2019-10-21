#ifndef _REC_RECOVER_H_
#define _REC_RECOVER_H_

#include "kdvtype.h"

/************************************************************************/
/*          �����붨��                                                  */
/************************************************************************/
typedef enum
{
	RRErrCode_OK = 0,
	RRErrCode_Error,
	RRErrCode_ERR_InvalidParam,
	RRErrCode_ERR_LoadDisk,
	RRErrCode_ERR_ReadDisk,
	RRErrCode_ERR_NotVbfs,
	RRErrCode_ERR_FsDestroyed,
	RRErrCode_ERR_NoMemeory,
}ERRErrCode;

/************************************************************************/
/*          �궨��                                                      */
/************************************************************************/
#define RR_MAXLEN_DISKNAME          64
#define RR_MAXLEN_FILEPATH          128
#define RR_MAXLEN_FILENAME          128
#define RR_MAXLEN_STREAMFORMAT      32
#define RR_MAXNUM_STREAM            2
#define RR_MAXLEN_IPADDR            64
#define RR_MAXLEN_MACHINEID			64
#define RR_MAXLEN_ZONENAME          32
#define RR_MAXLEN_USERNAME          32
#define RR_MAXLEN_PASSWD            16
#define RR_MAXLEN_MEDIAINFO         1024

/************************************************************************/
/*          �ṹ�嶨��                                                  */
/************************************************************************/

typedef enum
{
	E_StorType_Normal = 1,
	E_StorType_Vbfs,   
	E_StorType_Cloud,
}ERR_StorType;

typedef enum
{
	E_DType_File = 1,         /*�ļ�*/
	E_Dtype_Dir,              /*·��*/
}ERR_DType;

typedef enum
{
	E_RR_StreamType_Audio = 1,
	E_RR_StreamType_Video,
}ERR_StreamType;

typedef enum
{
	E_IpType_IPv4 = 1,
	E_IpType_IPv6,
}ERR_IpType;

typedef struct tagRRDirect
{
	ERR_DType  m_eRRDtype;
	s8         m_szDirect[RR_MAXLEN_FILEPATH+1];
}TRR_Direct;

typedef struct tagRRStreamInfo
{
	ERR_StreamType  m_eStreamType;
	u8              m_byStreamFormat;                            /*�����ı����ʽ*/
	s8              m_szFormatName[RR_MAXLEN_STREAMFORMAT+1];    /*�����ı����ʽ����*/
}TRR_StreamInfo;

typedef struct tagRRExtInfo
{
	u16   m_wExtInfoLen;
	u8    m_byExtInfo[RR_MAXLEN_MEDIAINFO];
}TRR_ExtInfo;

typedef struct tagRRFileInfo
{
	u32        m_dwStartTime;           /*��λ����*/
	u32        m_dwEndTime;             /*��λ����*/
	u64        m_llFileSize;            /*��λ���ֽ�*/
	BOOL32     m_bLocked;               /*¼���ļ��Ƿ�����*/
	u8         m_byStreamCount;         /*¼���ļ�����������*/
	TRR_StreamInfo m_atStreamInfo[RR_MAXNUM_STREAM];
	TRR_ExtInfo    m_tExtInfo;
}TRR_FileInfo;

typedef struct tagRRIPAddr 
{
	ERR_IpType  m_eIpType;
	s8          m_szIpAddr[RR_MAXLEN_IPADDR+1];
}TRR_IPAddr;

typedef struct tagRRNetAddr
{
	TRR_IPAddr   m_tIpAddr;
	u16          m_wPort;
}TRR_NetAddr;

typedef struct tagCloudStorLoadParam
{
	u16          m_wNetAddrNum;                            /*��Ҫ���ӵ��Ƶ�ַ����*/
	TRR_NetAddr  *m_ptNetAddr;                             /*���飬��Ҫ���ӵ��Ƶ�ַ��Ϣ*/
	s8           m_szMachineID[RR_MAXLEN_MACHINEID+1];     /*����ID�� ��UUID*/        
	s8           m_szZoneName[RR_MAXLEN_ZONENAME+1];       /*����*/
	s8           m_szUserName[RR_MAXLEN_USERNAME+1];       /*�����Ƶ��û���*/
	s8           m_szPasswd[RR_MAXLEN_PASSWD+1];           /*�����Ƶ�����*/
	u32          m_dwTimeOut;                              /*���ӳ�ʱʱ��*/
}TRR_CloudStorLoadParam;

typedef struct tagOtherStorLoadParam
{
	s8           m_szDiskName[RR_MAXLEN_DISKNAME+1];
}TRR_OtherStorLoadParam;

/*====================================================================
������      : RR_LoadStor
����        : ���ش洢
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN] ERR_StorType eStorType -- ��Ҫ���صĴ洢���ͣ�ext3, vbfs or cloud
			   [IN] void* pvLoadParam -- ���ز�����
					�ƴ洢���ز����ṹ�� -- TRR_CloudStorLoadParam
					�����洢���ز����ṹ�� -- TRR_OtherStorLoadParam
����ֵ˵��  :  0���ɹ��� other��ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2015/01/29             linlifen         ����
====================================================================*/
ERRErrCode RR_LoadStor(const ERR_StorType eStorType, void* pvLoadParam);

/*====================================================================
������      : RR_UnloadStor
����        : ж�ش洢
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��: ��
����ֵ˵��  : 0���ɹ��� other��ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2015/01/29             linlifen         ����
====================================================================*/
ERRErrCode RR_UnloadStor();

/*====================================================================
������      : RR_ScanDir
����        : ɨ��·����ע�⣬�ýӿڲ����е���ɨ�裬�����ɨ��·��ΪA,
			  ��·��A�������·��B����·��B������������·������ֻ�᷵��
			  ·��B�����Ҫ���·��B���������·���������ٴε��øýӿ�
			  ��·��B����ɨ�裬��������
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN] const s8* pszScanDirName -- ��Ҫɨ���·��
			   [OUT] TRR_Direct ***DirList -- ɨ�赽���ļ���ȡ·���б�
			   [OUT] s32& nDirNum  -- ɨ�赽���ļ�����·��������
����ֵ˵��  :  0���ɹ��� other��ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2015/01/29             linlifen         ����
====================================================================*/
ERRErrCode RR_ScanDir(const s8* pszScanDirName, TRR_Direct ***ppptDirList, s32& nDirNum);

/*====================================================================
������      : RR_GetFileInfo
����        : ��ȡ�ļ�����Ϣ
�㷨ʵ��    : 
����ȫ�ֱ���: 
�������˵��:  [IN] const s8* pszFileName -- �ļ���
			   [OUT] TRR_FileInfo* ptFileInfo -- �ļ���Ϣ
����ֵ˵��  :  0���ɹ��� other��ʧ��
----------------------------------------------------------------------
�޸ļ�¼    ��
��  ��      �汾        �޸���        �޸�����
2015/01/29             linlifen         ����
====================================================================*/
ERRErrCode RR_GetFileInfo(const s8* pszFileName, TRR_FileInfo* ptFileInfo);

void RR_ReleaseDirect(TRR_Direct ***ppptDirList, const s32 nDirNum);

#endif

