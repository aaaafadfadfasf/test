
#ifndef _FILE_INTERFACE_DLL_H_
#define _FILE_INTERFACE_DLL_H_

#define OUT		// ��ʾ�����Ĳ������������
#define IN		// ��ʾ�����Ĳ������������

#include "kdvtype.h"
#include "kdvdef.h"

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	��һ���ֽڵĽṹ���룬��ֹ��ͬ�ı��������´���
#endif	//WIN32

#ifndef EVEREY_FILE_TYPE
#define EVEREY_FILE_TYPE	
//  ��������
enum {
		Audio_Media_Type				= 0,
		Video_Media_Type				= 1,
		Command_Media_Type			= 2,
		JFIF_Media_Type				= 3,
		Degradable_JPEG_Media_Type		= 4,
		File_Transfer_Media_Type		= 5,
		Binary_Media_Type				= 6,
		Unknown_Media_Type			= 7
};
#endif


#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) |  \
	((u32)(u8)(ch2) << 16) | ((u32)(u8)(ch3) << 24 ))
#endif

#define   KDMFILE_MAXLEN_IPADDR           64
#define   KDMFILE_MAXLEN_MACHINEID        64
#define   KDMFILE_MAXLEN_ZONENAME         32
#define   KDMFILE_MAXLEN_USERNAME         32
#define   KDMFILE_MAXLEN_PASSWD           16
#define   KDMFILE_MAXLEN_FILENAME         255

typedef enum
{
	KdmFIle_IpType_IPv4 = 0,
	KdmFIle_IpType_IPv6,
}EKdmFile_IpType;

typedef struct  
{
	EKdmFile_IpType  m_eIpType;
	s8          m_szIpAddr[KDMFILE_MAXLEN_IPADDR+1];
}TKdmFile_IPAddr;

typedef struct  
{
	TKdmFile_IPAddr   m_tPrimaryIpAddr;
	TKdmFile_IPAddr   m_tSecondIpAddr;             /*���Բ�����*/
	u16				m_wPort;
}TKdmFile_NetAddr;

typedef struct
{
	u16				m_wNetAddrNum;							/*��Ҫ���ӵ��Ƶ�ַ����*/
	TKdmFile_NetAddr* m_ptNetAddr;								/*���飬��Ҫ���ӵ��Ƶ�ַ��Ϣ����ҵ���ϲ����*/
	s8           m_szMachineID[KDMFILE_MAXLEN_MACHINEID+1];		/*����ID�� ��UUID*/        
	s8           m_szZoneName[KDMFILE_MAXLEN_ZONENAME+1];		/*����*/
	s8           m_szUserName[KDMFILE_MAXLEN_USERNAME+1];		/*�����Ƶ��û���*/
	s8           m_szPasswd[KDMFILE_MAXLEN_PASSWD+1];           /*�����Ƶ�����*/
	u32          m_dwTimeOut;									/*���ӳ�ʱʱ��*/
	s32          m_nDataChunk;								/*RAID�����ݿ�����������Ϊ0����ʾʹ���Ƶ�Ĭ������*/
	s32          m_nParityChunk;								/*RAID������У�������������Ϊ0����ʾʹ���Ƶ�Ĭ������*/
}TKdmFile_CloudConnectParam;

//  �ļ�������ؽṹ 
typedef struct tagASFFileProperty{
	u64		qwTotalFileSize;				// �ļ���С
	u64		qwCreateDate;					// �ļ�����ʱ�䣬����FILETIME��ʽ
	u64		qwDataPacketsCount;				// ���������ݰ�����
	u64		qwPlayDuration;					// �ļ����ŵ���ʱ�䳤��(100nano-second)
	u64		qwSendDuration;					// �ļ����͵���ʱ�䳤��(100nano-second)
	u64		qwPreroll;					// �ļ���ʼ��ƫ��ʱ��
	BOOL32		bIsBroadCast;					// �ļ�/�ֳ����ı�ʶ
	u32		dwDataPacketSize;				// �������ݰ��Ĵ�С
	u32		dwMaximumBitrate;				// �������� bits per second for entire file.
	u16		wAudioStreamCount;				// ��Ƶ������Ŀ
	u16		wVideoStreamCount;				// ��Ƶ������Ŀ
}TASFFileProperty;

//  �����������Խṹ 
typedef struct tagContentDescriptionProperty{
	u16		wTitleLength;					// ���ݱ��ⳤ��
	u16		wAuthorLength;				// �������߳���
	u16		wCopyrightLength;				// ���ݰ�Ȩ����
	u16		wDescriptionLength;			// ������������
	u16*		pwTitle;						// Unicode�ַ�����ʾ�����ݱ���
	u16*		pwAuthor;					// Unicode�ַ�����ʾ����������
	u16*		pwCopyright;					// Unicode�ַ�����ʾ�����ݰ�Ȩ
	u16*		pwDescription;				// Unicode�ַ�����ʾ������������Ϣ
}TContentDescriptionProperty;

#ifndef KSJHEADER
#define KSJHEADER
typedef struct tagKsjHeader 
{
	u32 m_dwFileMarker;
	u16 m_wVersion;
	u16 m_wHeaderLength;
	u32	m_dwDecoderId;
	u16	m_wExtDataLength;
	u16 m_wReserve;
	u8  m_pbyExtData[1024] ;
	
} TKsjHeader ;
#endif

#ifndef FORMATDATASTRUCT
#define FORMATDATASTRUCT
struct tagFormatDataStruct{
	u32		dwFormatDataSize;				// ��Ƶ�������ݴ�С
	s32		dwImageWidth;					// ��Ƶ������
	s32		dwImageHeight;					// ��Ƶ����߶�
	u16		wReserved;						// ��������
	u16		wBitsPerPixelCount;				// ÿ���صı�����
	u32		dwCompressionID;				// ��Ƶѹ��Codec��ʶ
	u32		dwImageSize;					// ����ߴ�
	s32		nHorizontalPixelsPerMeter;		// ˮƽ�ֱ���
	s32		nVerticalPixelsPerMeter;		// ��ֱ����ֱ���
	u32		dwColorsUsedCount;				// ��ɫ��
	u32		dwImportantColorsCount;			// 
	u8*		pbyCodecSpecificData;			// Codec ��չ��Ϣ
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;	
typedef  struct tagFormatDataStruct TFormatData;
#endif

#ifndef AUDIOMEDIAINFO
#define AUDIOMEDIAINFO
// ASF Audio Media Type
struct tagAudioMediaInfo{
	u16		wCodecID_FormatTag;				// ��ƵCodec��ʶ
	u16		wNumberOfChannels;				// ��Ƶͨ����				
	u32		dwSamplesPerSecond;				// ������
	u32		dwAverageNumberOfBytesPerSecond;// ƽ������ Bytes / Second
	u16		wBlockAlignment;				// ������ֽ��� 
	u16		wBitsPerSample;					// ÿ�������ı�������Ϊ8����16
	u16		wCodecSpecificDataSize;			// ��ƵCodec��ʶ
	u16		wReserve;
	u8*		pbyCodecSpecificData;			// ��ƵCodec��ʶ
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;							// �൱�� WAVEFORMATEX �ṹ
typedef struct tagAudioMediaInfo TAudioMediaInfo;
#endif

#ifndef VIDEOMEDIAINFO
#define VIDEOMEDIAINFO
struct tagVideoMediaInfo{
	u32		dwEncodedImageWidth;			// ��Ƶ������
	u32		dwEncodedImageHeight;			// ��Ƶ�����
	u16		wFormatDataSize;				// ��Ƶ�������ݴ�С
	u16		wReserve;
	TFormatData 	tFormatData;			
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;
typedef  struct tagVideoMediaInfo TVideoMediaInfo;
#endif

#ifndef OTHERMEDIAINFO
#define OTHERMEDIAINFO
typedef struct tarOtherMediaInfo{
	u32		dwInfoLength;					// ����ý�����ݳ���
	u8		*pbyInfoData;					// ����ý������ָ��
} 
//#if defined(_VXWORKS_) || defined(_LINUX_)
//__attribute__ ( (packed) ) 
//#endif
TOtherMediaInfo;

#endif

#ifndef STREAMPROPERTY
#define STREAMPROPERTY
//  �����Խṹ 
struct tagStreamProperty{
	u16		wStreamType;					// ������
	u8		byStreamNumber;					// �����
	u8		byReserve;
	u16		wErrorCorrectType;				// ������������	
	u16		wResever;
	u32		dwErrorCorrectDataLength;		// ������������	
	u8		*pbyErrorCorrectData;			// ������������	
	BOOL32	bIsEncryptedContent;			// �Ƿ�����ݲ������ر���
	u32		dwAverageBitrate;				// ƽ��������  bits/s
    u32     dwFrameRate;
	u32     dwFrameNumbers;   
	union{
		TAudioMediaInfo	 tAudioMediaInfo;   // ��Ƶ��������
		TVideoMediaInfo	 tVideoMediaInfo;	// ��Ƶ��������
		TOtherMediaInfo	 tOtherMediaInfo;	// ����ý����������
	};
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;
typedef struct tagStreamProperty TStreamProperty;
#endif

typedef struct tagStreamDataPayload
{
	u8		byStreamNumber;				// �����			
	u32		dwMediaObjectNumber;			// ֡���
	BOOL32	bIsKeyFrame;					// �Ƿ�Ϊ�ؼ�֡
	u32		dwPresentationTime;			// ֡ʱ����Ϣ��ʱ�����
	u32		dwPayloadDataSize;			// ���ݳ���
	u8		*pbyPayloadData;				// ��Ч����

}TStreamDataPayload;

//  �������صĴ�����
#ifndef __ASF_ERROR_CODE__
#define __ASF_ERROR_CODE__
enum
{
	ASF_Error_NoError				= 0,   // û�д�����ȷ����
	ASF_Error_FileNotFound			= 1,   // �ļ�û���ҵ�
	ASF_Error_InvalidASFFile		= 2,	// ��Ч��ASF�ļ�
	ASF_Error_ObjectNotFound 		= 3,	// ����û���ҵ�
	ASF_Error_BadIndex			= 4,	// ����Ķ�������
	ASF_Error_ValueNotFound		= 5,	// û���ҵ���Ӧ��ֵ
	ASF_Error_BadArgument			= 6,	// ����Ĳ���
	ASF_Error_ReadOnly			= 7,	// �ļ�ֻ������
	ASF_Error_NotEnoughSpace		= 8,	// �洢�ռ䲻�����
	ASF_Error_InternalError  		= 9,	// �޷������ڴ���ڲ�����
	ASF_Error_ReadFileError  		= 10,	// ���ļ��ڲ�����
	ASF_Error_WriteFileError  		= 11,	// д�ļ��ڲ�����
	ASF_Error_SeekFileError  		= 12,	// �����ļ�����
	ASF_Error_CreateFileError  	= 13,	// �����ļ�����
	ASF_Error_CreateThreadError	= 14,	// �����ļ�����
	ASF_Error_ObjectNULLError		= 15,	// ����ָ��Ϊ�յĴ���
	ASF_Error_TooMuchVideoStrream	= 16,	//
	ASF_Error_NotSupportedCodec	= 17,	//
	ASF_Error_FileEnd				= 18,	//
	ASF_Error_UnknownError			= 19,	// �޷�ȷ���Ĵ���		
	ASF_Error_InvalidKsjFile		= 20,	// ����ksj �ļ�
	ASF_Error_DiskFull			= 21,	//����������
	ASF_Error_FrameCheckError		= 22	,//֡У�����
	ASF_Error_ReadDiskError		= 23,	//�����̴���
	ASF_Error_AsfLibNotStartUp     = 24,   //Asflib��û�г�ʼ��
	ASF_Error_TimerInitError       = 25,   //��ʱ����
};

typedef u16 Error_Code;
static char * sASF_ErrorInfo[] = 
{
	/* 0  */ "No Error, return OK!" ,
	/* 1  */ "Open the file FAILED for ASF file does not exist or the file cannot be open with \"rb\" mode!" ,
	/* 2  */ "The opened file is invalid, it maybe not keep to ASF Specification, or occur some Error when parsing the file !"  ,
	/* 3  */ "Cannot find Object by the given index!"   ,
	/* 4  */ "It is a wrong index of Object!"  ,
	/* 5  */ "Cannot get value by the given index!" ,
	/* 6  */ "The input parameter is illegal,for example pointer is NULL or parameter is not in the range!"  ,
	/* 7  */ "The file is only for reading!" ,
	/* 8  */ "There is not enough space for write or new!" ,
	/* 9  */ "Failed in newing space for pointer!" ,
	/* 10 */ "Read file ERROR for the get bytes is not equal to the one that should get!"  ,
	/* 11 */ "Write file ERROR for the put bytes is not equal to the one that should put!"  ,
	/* 12 */ "Seek file ERROR !" ,
	/* 13 */ "Create file ERROR !" ,
	/* 14 */ "Create thread ERROR!" ,
	/* 15 */ "Input pointer is NULL!" ,
	/* 16 */ "there is too much stream or video stream!",
	/* 17 */ "not supported codec format id, cannot play the asf file!",
	/* 18 */ "to file end!",
	/* 19 */ "There is an unknown Error!"
	/* 20 */ "Invalid Ksj File"
	/* 21 */ "Disk Full"
	/* 22 */ "Frame Check Error"
	/* 23 */ "Read Disk Error"
	/* 24 */ "Asflib not start up"
	/* 25 */ "timer thread init failed"
};

#endif

//����ģʽ����������ţ�����
//05.7.12
enum {
	NORMAL_PLAY         = 0,
	FAST_PLAY           = 1,
	SLOW_PLAY           = 2,
	CANCEL_TIMER        = 3
};


//���򲥷�ʱ��״̬
enum{
	ASF_Read_Status_Wait			= 0,
	ASF_Read_Status_Playing			= 1,
	ASF_Read_Status_EOF				= 2,
	ASF_Read_Status_Closed			= 3,
	ASF_Read_Status_SINGLE_FRAME	= 4	//��֡����05.8.11
};
typedef u16	ASF_READ_STATUS;

typedef void (* ASFFrameProcCallBack)(KD_PTR pvUser, u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *piFrameData);

API u16	File_Open(IN const char * pFileName, TKsjHeader * ptKsjHeader, void **pWriter);
//API u16	File_OpenByType(IN const char * pFileName,u32 type, TKsjHeader * ptKsjHeader, void **pWriter);
API u16	KdmFile_Open(IN const char * pFileName, TKsjHeader * ptKsjHeader, u32 dwCreateTime, void **pWriter);
API u16	KdmFile_OpenByType(IN const char * pFileName, TKsjHeader * ptKsjHeader, u32 dwCreateTime, void **pWriter,u32 type);
//  ����ASF�ļ���������Ϣ
API u16 File_SetASFFileProperty(IN TASFFileProperty * pFileProperty, void *pWriter);
API u16  File_SetMp4MdatSize(u32 dwMdatSize ,void *pWriter) ;
//  ����ASF�ļ������ݰ�Ȩ��Ϣ
API u16 File_SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty, void *pWriter);

//  �����úõ�����ӵ��ļ�ͷ��
API u16 File_AddStream(IN TStreamProperty * pStreamProperty, void *pWriter);
API u16 File_SetMp4BufferSize(u32 dwBufferSize, void *pWriter) ;

//  ��ʼдASF�ļ������ݲ���
API u16  File_BeginWriting(void *pWriter);

//  ���ղ�ͬ�������д֡����
API u16  File_WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
						  IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
						  OUT u32* pdwFileSize, void *pWriter);
//  ����д֡����
API u16  File_EndWriting(void *pWriter);

//  �ر��ļ����ͷ���Դ�ռ�
API u16  File_Close(void *pWriter);

//DropFrameTime��λ�������룬����ᵼ��ʱ�����
API u16 File_AddDropFrameTime(void *pWriter, u32 DropFrameTime);

//����moovˢ��Ƶ�ʣ�����Ϊ��Ƶ֡�����
API u16 File_SetMoovFreq(void *pWriter, IN u32 dwFrameInterval);

//����ˢ����
API u16 File_UpdateIndex(void *pWriter);

//�˽ӿڽ����ϲ������ļ���
API u16	File_Create_FixFile(void **pWriter);

//�����ϲ��ӿ�
API u16 File_FixFile(void *pWriter, IN const char * pFileName, IN const char * pMoovName);

//�˽ӿڽ����ϲ������ļ���
API u16 File_Release_FixFile(void *pWriter);

//read
//reader�ӿڱص�
API void File_KdmFileStartUp();
API void File_KdmFileCleanUp();


API u16	File_Create_Read(IN const char * pFileName, void **pReader);
API u16 File_Create_ReadByType(IN u32 type, void **pReader);
//  ��ASF�ļ��ĳ�ʼ������������ж��Ƿ�ΪASF�ļ�������ȡASF�ļ���ͷ��Ϣ
API u16 File_Open_Read(IN const char * pFileName, IN ASFFrameProcCallBack  inCallback, IN void* pvContext, OUT TKsjHeader * ptKsjHeader, void *pReader);

//  ��ȡASF�ļ���������Ϣ
API u16 File_GetASFFileProPerty(OUT TASFFileProperty * pFileProperty, void *pReader);

//  ��ȡASF�ļ���������Ϣ
API u16 File_CopyRight(OUT BOOL32 *IsKedacomFile, void *pReader);

//  ��ȡASF�ļ��������ĸ���
API u16 File_GetStreamTotal(OUT u16 *wStreamCount, void *pReader);

//�õ���Ӧ�ֶεĳ���
API u16 File_GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength ,void *pReader);

//  ����������ȡASF�ļ���ÿ��������Ϣ,���������úó��ȣ�������ڴ�
API u16 File_GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty, void *pReader);

//���ÿ�����źͱ���
API u16 File_SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate, void *pReader);

API u16 File_Start(IN u32 dwStartTime , void *pReader , BOOL32 bIsAutoCallBack, BOOL32 bDragToKeyPoint);

//��֡��ȡ������û����һ��
API u16 File_GetNextFrame( u32 *pdwStatus, TStreamDataPayload **ppoFrameData, void *pReader);

//��ȡ��Ƶ���һ֡�ؼ�֡ʱ���
API u16 File_GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime, void *pReader);

//��ȡ��Ƶ���һ֡ʱ���
API u16 File_GetLastVideoFrameTime(OUT s64 *poLastVideoFrameTime, void *pReader);

//  ��ͣ�첽��ȡASF�ļ���֡����
API u16 File_Pause(void *pReader);

//  ����ͣ��λ�������첽��ȡASF�ļ���֡����,���ɵ�֡�ص�ģʽ���ص��Զ��ص�ģʽ
API u16 File_Resume(void *pReader);

//  ֹͣ�첽��ȡASF�ļ���֡����
API u16 File_Stop(void *pReader);

//  �ر�ASF�ļ���ȡ�����ͷ���Դ
API u16 File_Close_Read(void *pReader);

API u16 File_Release_Read(void *pReader);


#if defined(__Win32__) || defined(WIN32)
#pragma pack(pop)
#endif	//WIN32


#endif //_FILE_INTERFACE_DLL_H_
