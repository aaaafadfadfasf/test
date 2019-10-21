/************************************************************
//  �� �� ����  asflib.h
//  ����ļ���  asflib.cpp
//	��    �ߣ�	�Ժ���
//	����ʱ�䣺	2005-08-29
//	�޸�ʱ�䣺	
************************************************************/

#ifndef __ASF_LIB__
#define __ASF_LIB__


/************************************************************
*************************************************************

//����ASF�ļ���д�ӿڵ����ݽṹ

*************************************************************
*************************************************************/
#include "osp.h"

#ifdef _SUPPORT_KCS_CLOUD_
#include "kdmfilecloud.h"
#else
#include "kdmfileinterface.h"
#endif

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	��һ���ֽڵĽṹ���룬��ֹ��ͬ�ı��������´���
#endif	//WIN32



#define MAX_ASF_READER					400		//���ɲ���·��
#define MAX_ASF_PATH_LENTGTH			128		//asf�ļ�·����󳤶�,������,�ϲ㲻�ÿ���
// �������Ч�Եļ��
#define CHECKSTREAMNUMBER(a)	(((a) >= 1 && (a) <= 63) ? TRUE : FALSE) //֧���������128����Ƶ��2

#define COPY_DWORD(achByte, dwInt)          do{*(achByte) =   (u8)(((u32)dwInt)& 0xff) ; \
                                               *(achByte+1) = (u8)(((u32)dwInt >> 8)& 0xff) ; \
                                               *(achByte+2) = (u8)(((u32)dwInt >> 16)& 0xff); \
                                               *(achByte+3) = (u8)(((u32)dwInt >> 24)& 0xff); \
                                               } while(0);

#define COPY_WORD(achByte, wInt)             do{*(achByte) =   (u8)(((u16)wInt)& 0xff) ; \
                                                *(achByte+1) = (u8)(((u16)wInt >> 8)& 0xff); \
                                               } while(0);

typedef void* HTimer;

    
// ���������������ṹ
struct TStreamListEntry {	
	TStreamProperty		tStreamProperty;	// �����������
	TStreamListEntry	*pNextStream;		// ��һ���������ָ��
	u8 					byStreamID;			// �������˳���
	u8					byAlignedField;		//�����ֽ�,������
	u16					wAlignedField;		//�����ֽ�,������
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;


//  ��־����
typedef struct tagMarkers{
	u64		qwOffset;						// ��־���ļ��е�λ��ƫ��
	u64		qwPresentationTime;				// ��־��ʾ��ʱ��
	u32		dwMarkerDescriptionLength;		// ��־����������
	u8		*pbyMarkerDescription;			// ��־��������Ϣ
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
TMarkerStruct;



//  ����ű�����
typedef struct tagCommands{
	u32		dwPresentationTime;				// �ű�ִ�е�ʱ��
	u16		wCommandsTypeLength;			// �ű������ͳ���
	u16		*pwCommandsType;				// �ű�������
	u16		wCommandNameLength;				// �ű������Ƴ���
	u16		*pwCommandName;				// �ű�������
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
TCommandStruct;


//  ������������ 
enum {
		No_Error_Correction_Type 		= 0,
		Audio_Spread_Type 				= 1,
		Unknown_Error_Type 				= 2
};
typedef u16 ERROR_CORRECTION_TYPE;


// �ļ������ID�������õ�����Ϣ�Ͷ����ʶ
// ��ʶ����3λ�������
// 1 �� ��  ���ļ�ͷ��Ϣ��ص�ͷ������
// 2 �� ��  �����ݶ�����صĶ��� 
// 3 �� ��  ��������Ϣ��صĶ���
// 4 �� ��  �ۺ��ԵĶ���
// 9 �� ��  ��Ч�Ķ���
enum
{
		// ASF TOP-LEVEL HEADER OBJECT
		ASF_ObjectID_Header_Object                       = 100,
		ASF_ObjectID_File_Properties_Object              = 101,
		ASF_ObjectID_Stream_Properties_Object            = 102,
		ASF_ObjectID_Header_Extension_Object             = 103,
		ASF_ObjectID_Codec_List_Object                   = 104,
		ASF_ObjectID_Script_Command_Object               = 105,
		ASF_ObjectID_Marker_Object                       = 106,
		ASF_ObjectID_Bitrate_Mutual_Exclusion_Object     = 107,
		ASF_ObjectID_Error_Correction_Object             = 108,
		ASF_ObjectID_Content_Description_Object          = 109,
		ASF_ObjectID_Extended_Content_Description_Object = 110,
		ASF_ObjectID_Content_Branding_Object             = 111,
		ASF_ObjectID_Stream_Bitrate_Properties_Object    = 112,
		ASF_ObjectID_Content_Encryption_Object           = 113,
		ASF_ObjectID_Extended_Content_Encryption_Object  = 114,
		ASF_ObjectID_Alt_Extended_Content_Encryption_Object = 115,
		ASF_ObjectID_Digital_Signature_Object            = 116,
		ASF_ObjectID_Padding_Object                      = 117, 
		ASF_ObjectID_UserInfo_Object                      = 118, 

		// ASF TOP-LEVEL DATA OBJECT
		ASF_ObjectID_Data_Object						= 200,
		ASF_ObjectID_Packet_Object						= 201,        
		ASF_ObjectID_DataErrorCorrection_Object			= 202,
		ASF_ObjectID_DataParseInfomation_Object			= 203,
		ASF_ObjectID_DataPadding_Object					= 204,
		ASF_ObjectID_DataPayload_Object					= 205,
		
		// ASF TOP-LEVEL INDEX OBJECTS       
		ASF_ObjectID_Simple_Index_Object                 = 300,
		ASF_ObjectID_Index_Object                        = 301,
		ASF_ObjectID_Media_Object_Index_Object           = 302,
		ASF_ObjectID_Timecode_Index_Object               = 303,
		
		// ASF ALL OBJECT		
		ASF_ObjectID_ALL_Object							 = 400,
		ASF_ObjectID_ALL_Header_Object					 = 401,
		ASF_ObjectID_ALL_Frame_Object					 = 402,

		// Present Nothing 
		ASF_ObjectID_Nothing_Object						 = 900
		
};
typedef u16 ASFObjectID;


class CKdvASFFile;
/*********************************************************
**********************************************************

����ASF�ļ���д�ӿ���

**********************************************************
**********************************************************/


class CKdvASFFileWriter : public IKdmFileWriter
{
public:
	//
	// Constructors and destructor.
	CKdvASFFileWriter(void);
	~CKdvASFFileWriter(void);

	//  дASF�ļ��ĳ�ʼ������
		virtual u16	Open(IN const char * pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0) ;

	//  ����ASF�ļ���������Ϣ
	u16	SetASFFileProperty(IN TASFFileProperty * pFileProperty);
	
	//  ����ASF�ļ������ݰ�Ȩ��Ϣ
	u16	SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty);

	//�����Ƿ�������������
	u16  SetGenerateSimpleIndexObject(IN BOOL32 isGenarate = false);
		
	//  �����úõ�����ӵ��ļ�ͷ��
	u16  AddStream(IN TStreamProperty * pStreamProperty);

	//  ��ʼдASF�ļ������ݲ���
	u16  BeginWriting(void);

	//  ���ղ�ͬ�������д֡����
	u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
					 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, OUT u32* pdwFileSize = NULL);
	
	//  ����д֡����
	u16  EndWriting(void);
	
	//  �ر��ļ����ͷ���Դ�ռ�
	u16  Close(void);


	//  �����Ƿ���Ҫ����Ļ���������Ϣ
	u16  SetDebugInfo(IN u16 wObjectID);
	//  ���ݺ����ķ���ֵ��ȡ��ϸ�Ĵ�����Ϣ
	LPSTR GetErrorInfo(IN u16 wErrorIndex);
	//  ר������ʾ���������ݡ���ÿ���ֽڵ�ֵ������ʮ�����Ƶ�������ʾ����
	void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength);	
	u16 PrintAllStep();
	
protected:

	SEMHANDLE			m_hSem;

	BOOL32				m_bSemCreate;
	BOOL32				m_bOpen;

	CKdvASFFile			*m_pASFFile;				//  ASF�ļ��������ָ��
	
	u16					m_wObjectIDForDebug;		//	��ʾҪ���Ե�Ŀ�����ID
	
};



/********************************************************
*********************************************************

�ļ�����ASF�ļ��Ķ��ӿ���

*********************************************************
*********************************************************/


class CKdvASFFileReader : public IKdmFileReader
{
public:
	//
	// Constructors and destructor.
	CKdvASFFileReader(void);
	~CKdvASFFileReader(void);
	u16	GetASFStreamList(TStreamListEntry *&ptStreamListEntry);
	//  ��ASF/MP4�ļ��ĳ�ʼ��������ͬʱ���ݶ�ȡ�ƴ洢�ļ����߱����ļ�
	//  ���ļ�·��ʾ����cloud://con_cate/con_name/blob_cate/blob_name
	//  �����ļ��Ļ����贫���Ʒ�����������ò����������ȡʧ��
	u16	CloudOpen(IN TKdmFile_CloudConnectParam* ptCloudConnectParam, IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN void* pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);

	//  ��ASF�ļ��ĳ�ʼ������������ж��Ƿ�ΪASF�ļ�������ȡASF�ļ���ͷ��Ϣ
	u16	Open(IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN KD_PTR pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);

	//  ���ô���֡���ݵĻص�����
	u16	SetCallback(IN KD_PTR pvUser, IN ASFFrameProcCallBack inCallback);

	//  ��ȡASF�ļ�����ͷ����ĸ���
	u16	GetHeaderObjectTotal(OUT u32 * dwHeadObjectCount);
	
	//  ��ȡASF�ļ���������Ϣ
	u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty);

	//05.7.26�õ���Ӧ�ֶεĳ���
	u16	GetContentDescriptionPropertyFieldLength(OUT u16 *poTitleLength,OUT u16 *poAuthorLength, OUT u16 *poCopyrightLength,OUT u16 *poDescriptionLength);
	
	//  ��ȡASF�ļ������ݰ�Ȩ��Ϣ,���������úó��ȣ�������ڴ�
	u16	GetContentDescriptionProperty(IN OUT TContentDescriptionProperty * pioContentDescriptionProperty);

	//  ��ȡASF�ļ��������ĸ���
	u16	GetStreamTotal(OUT u16 *wStreamCount);
	
	//�õ���Ӧ�ֶεĳ���
	u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength);

	//  ����������ȡASF�ļ���ÿ��������Ϣ,���������úó��ȣ�������ڴ�
	u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty);
	
	//  ��ȡASF�ļ��ľ�������
	u16	GetErrorCorrectionType(OUT u16 *pErrorType);
	//  ��ȡASF�ļ��ľ������ݳ���
	u16	GetErrorCorrectionDataLength(OUT u32 *pErrCorrDataLength);
	//  ��ȡASF�ļ��ľ�������
	u16	GetErrorCorrectionData(OUT u8 *pErrCorrData, IN u32 dwDataLength);	
	
	//  �����Ƿ񲥷���Ƶ��
	u16	SetPlayAudioStream(IN BOOL32 isPlayFirstStream = true, BOOL32 isPlaySecondStream = true);
	//  �����Ƿ񲥷���Ƶ����������������Ҫ����һ���������ô���ϵͳ�в���ǿ�Ʋ���һ������
	u16	SetPlayVideoStream(IN BOOL32 isPlayFirstStream = true, IN BOOL32 isPlaySecondStream = true );//05.7.26

	//���ÿ�����źͱ���
    u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate);

	//���ݱ������ID���ó���Ӧ��ý������
	u16	CodecFormatId2MediaType(IN u8 byiStreamType,IN u32 dwiCodecFormatID,OUT UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo);

	//  �����������ʼʱ�䣬�첽��ȡASF�ļ���֡����
	//bIsAutoCallBack���Ƿ��Զ��ص����Զ��ص��͵�֡�ص����л�
	//�������Զ��ص����ǵ�֡�ص������������ûص�������������ô˺�����
	u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = TRUE);
	
	//��֡��ȡ������û����һ��
	u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData);
	//������һ֡��Ƶ֡��Ӧ֡ʱ���
	u16 GetLastVideoFrameTime(OUT s64 *poLastKeyFrameTime);
	//  ��ͣ�첽��ȡASF�ļ���֡����
	u16	Pause(void);
	//  ����ͣ��λ�������첽��ȡASF�ļ���֡����,���ɵ�֡�ص�ģʽ���ص��Զ��ص�ģʽ
	u16	Resume(void);
	//  ֹͣ�첽��ȡASF�ļ���֡����
	u16	Stop(void);
	//  �ر�ASF�ļ���ȡ�����ͷ���Դ
	u16	Close(void);	


	//  �����Ƿ���Ҫ����Ļ���������Ϣ
	u16		SetDebugInfo(IN u16 wObjectID);
	//  �������ASF�ļ������ݶ�����������Ϣ��Table Of Content��
	u16		DumpObjectTOC(void);
	//  ���ݺ����ķ���ֵ��ȡ��ϸ�Ĵ�����Ϣ
	LPSTR GetErrorInfo(IN u16 wErrorIndex);
	//  ר������ʾ���������ݡ���ÿ���ֽڵ�ֵ������ʮ�����Ƶ�������ʾ����
	void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength);	
	
#ifdef _SUPPORT_KCS_CLOUD_
public:
	cdb::ReadOnlyStreamBlob * m_pcReadBlob;
#endif

private:
	CKdvASFFile*	m_pASFFile;				//  ASF�ļ��������ָ��

	u16				m_wObjectIDForDebug;		//	��ʾҪ���Ե�Ŀ�����ID

	SEMHANDLE			m_hSem;
	
	BOOL32				m_bSemCreate;
};


/***************************************************************
****************************************************************

ȫ�ֺ�������

****************************************************************
****************************************************************/

#ifndef API
#define API extern "C"
#endif


/*==============================================================
 AsfStartUp():	��ʱ���̴߳�������,��AsfCleanUp�����ԳƵ��ã��ڵ��ö��ӿ�֮ǰ����
				���ֻ�õ�д�ӿڣ��������������Բ����á��ڵ��ö��ӿ�֮ǰ��Ҫ����AsfStartUp(),
				����AsfStartUp����֮�����new���asfreader�����ʵ����	
				�������涨ʱ���Ĺ�ϵ,Ŀǰ��һ�������������֧��new 32��asfreader����,
				asfwriter�������ơ�	Delete���� asfreader֮�����AsfCleanUp()��
 ===============================================================*/
void AsfStartUp();

/*===============================================================
AsfCleanUp(): Asflib�Ķ�ʱ���߳��˳�����,��AsfStartUp�����ԳƵ���
================================================================*/
void AsfCleanUp();

/*================================================================
AsfWriteCleanUp:��ֻ�õ�asfwriter�������,���û�е���asfstartup��asfcleanup,
�����˳�ʱҪ������������ͷ��ڴ�,�������ڴ�й©.
���������asfstartup��asfcleanup,���õ����������
=================================================================*/
void AsfWriteCleanUp();

/*================================================================
 asflibversion(): asflib�汾��Ϣ
=================================================================*/
API void asflibversion();

/*================================================================
 asflibhelp(): asflib��������
=================================================================*/
API void asflibhelp();

/*================================================================
 asffileinfo()���ӿڴ򿪵��ļ������ź��ļ�����Ϣ
=================================================================*/
API void asffileinfo();

/*================================================================
 sfdebuginfo()��asflibʵʱ��Ϣ��ӡ
=================================================================*/
API void asfdebuginfo(s32 nIndex, s32 nLevel);

#if defined(__Win32__) || defined(WIN32)
#pragma pack(pop)
#endif	//WIN32

#endif  //__ASF_LIB__
