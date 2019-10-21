/**
* @file kdmfileinterface.h  
* @brief    �豸�������   http://172.16.6.123/svn/protocol_cbb/
* @author    
* @date 2016-06-16
* @version  1.0 
* @copyright V1.0  Copyright(C) 2016  All rights reserved.														  
*/

#ifndef _KDM_FILE_WRITER_H_
#define _KDM_FILE_WRITER_H_

#include "osp.h"
#include "kdvdef.h"
#include "kdmfileinterface_c.h"


/** ��ʾ�����Ĳ������������*/
#define OUT		

/** ��ʾ�����Ĳ������������*/
#define IN		

#ifdef _NVR_ARM_HISI_
#define OspPrintf(bScreen, bFile, szFormat, ...)
#endif  

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) |  \
    ((u32)(u8)(ch2) << 16) | ((u32)(u8)(ch3) << 24 ))
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P)          (P)
#endif

#ifndef _KDM_FILE_LOCAL_AUDIO_DEFINE_
#define _KDM_FILE_LOCAL_AUDIO_DEFINE_



#define KDM_FILE_AUDIO_G711A (u16)0x06       ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_G711A
#define KDM_FILE_AUDIO_G711U (u16)0x07	     ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_G711U
#define KDM_FILE_AUDIO_ADPCM (u16)0x11	     ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_ADPCM
#define KDM_FILE_AUDIO_G7221C  (u16)0x70	 ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_G7221C
#define KDM_FILE_AUDIO_AAC_LC (u16)0xA106	 ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_AAC_LC
#define KDM_FILE_STANDARD_AAC_LC (u16)0xFF   ///<Ŀǰ�ļ���֧�ֵ���Ƶ����_STANDARD_AAC_LC 
#define KDM_FILE_AUDIO_OPUS (u16)0x00F0	     ///<Ŀǰ�ļ���֧�ֵ���Ƶ���� Opus

#endif

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	��һ���ֽڵĽṹ���룬��ֹ��ͬ�ı��������´���
#endif	//WIN32
/** ֧��4K ����ؼ�֡���ķ����ڴ�*/
#define MAX_VIDEO_FRAME_SIZE_FOR4K   (2*1024*1024)  

/** ö����������asf��MP4*/
enum 
{
    ASF_File_Type  = 0,
    MP4_File_Type  = 1,    
};

/** ý������*/
typedef u16 ASF_MEDIA_TYPE;

/** д�ļ�����*/
class IKdmFileWriter
{
protected:
	
/**
 *@brief  д�ļ�����Constructors
 *@return     
 *@ref        
 *@see
 *@note       
 */
    IKdmFileWriter(void){};

/**
 *@brief  д�ļ�����destructor
 *@return     
 *@ref        
 *@see
 *@note       
 */
    virtual ~IKdmFileWriter(void){};
public:

/**
 *@brief  �����ļ�
 *@param[in]  pFileWriter
 *@param[in]  u32 type��type����ASF��MP4
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note       
 */
    static u16 CreateNew(IKdmFileWriter** pFileWriter, u32 type);  

/**
 *@brief  �ͷ�д����
 *@param[in]  pFileWriter
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note       
 */
    static u16 Release(IKdmFileWriter* pFileWriter);
	/**
 *@brief  �����ļ�
 *@param[in]  pFileWriter
 *@param[in]  LPCSTR pFilePath��pFilePath�ļ�·��
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  �����ļ��ַ�������3���ַ��ж�Ӧ�ô���ʲô���͵��ļ���ʽ������Ӧ 
 */
    static u16 CreateNew(IKdmFileWriter** pFileWriter, LPCSTR pFilePath);

/**
 *@brief  дASF�ļ��ĳ�ʼ������
 *@param[in] LPCSTR pFileName  �ļ���
 *@param[in] TKsjHeader * ptKsjHeader 
 *@param[in] u32 dwCreateTime ����ʱ��
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16	Open(IN LPCSTR pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0) = 0;
			
	/**
 *@brief  ���������
 *@param[in] pStreamProperty  �����Խṹ
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  AddStream(IN TStreamProperty * pStreamProperty) = 0; 
	virtual u16  SetMp4MdatSize(u32 dwMdatSize) {return 0;};
	virtual u16  SetMp4BufferSize(u32 dwBufferSize) {return 0;};
/**
 *@brief  ��ʼдASF�ļ������ݲ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  BeginWriting(void) = 0;

	/**
 *@brief  ���ղ�ͬ�������д֡����
 *@param[in] u8 byStreamNumber  �����
 *@param[in] u8 * pDataBuffer   ֡����ָ��
 *@param[in] u32 dwBufferSize   ֡���ݴ�С
 *@param[in] BOOL32 bIsKeyFrame �Ƿ��ǹؼ�֡
 *@param[in] u32 dwTimeStamp    ʱ���
 *@param[in] u16 wDuration      ����ʱ�䣬�ļ�����û���õ�
 *@param[out] u32* pdwFileSize = NULL
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
			        		 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
                             OUT u32* pdwFileSize = NULL) = 0;
	 
/**
 *@brief  ����д֡����
 *@return    �μ������붨�� 
 *@ref        
 *@see
 *@note  
 */
	virtual u16  EndWriting() = 0;
	
/**
 *@brief  �ر��ļ����ͷ���Դ�ռ�
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  Close() = 0;

   
/**
 *@brief  ����ASF�ļ���������Ϣ
 *@param[in] TASFFileProperty * pFileProperty    �����ļ�����
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  ������ASF
 */
	virtual u16	SetASFFileProperty(IN TASFFileProperty * pFileProperty){ return 0; }
	 
	/**
 *@brief  ����ASF�ļ������ݰ�Ȩ��Ϣ
 *@param[in]TContentDescriptionProperty * pContentDescriptionProperty   �����ļ��������Խṹ��ֵ
 *@return   �μ������붨��  
 *@ref        
 *@see
 *@note  ������ASF
 */
	virtual u16	SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty){ return 0; }

	
/**
 *@brief  �����Ƿ�������������
 *@param[in] BOOL32 isGenarate   
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  SetGenerateSimpleIndexObject(IN BOOL32 isGenarate = false){ return 0; }


	 
/**
 *@brief �����Ƿ���Ҫ����Ļ���������Ϣ
 *@param[in] u16 wObjectID   �������ID
 *@return     
 *@ref        
 *@see
 *@note  
 */
	virtual u16  SetDebugInfo(IN u16 wObjectID) { return 0; }
 
/**
 *@brief ���ݺ����ķ���ֵ��ȡ��ϸ�Ĵ�����Ϣ
 *@param[in] u16 wErrorIndex �������������ֵ  
 *@return     ������������Χ����������ΪNULL�����򷵻ش�����ϸ��Ϣ
 *@ref        
 *@see
 *@note  
 */
	virtual LPSTR GetErrorInfo(IN u16 wErrorIndex) { return 0; }
 
/**
 *@brief ר������ʾ���������ݡ���ÿ���ֽڵ�ֵ������ʮ�����Ƶ�������ʾ����
 *@param[in] u8 * pData  ֡����
 *@param[in] u32 dwDataLength  ֡����
 *@return     
 *@ref        
 *@see
 *@note  
 */
	virtual void ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength) {  }
 
/**
 *@brief ��ӡд�ļ��Ĳ�������
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16  PrintAllStep() { return 0; }
	
/**
 *@brief ��������ʱ��ʼʱ�䵽¼���һ֡ʱ��
 *@param[in] u32 DropFrameTime 
 *@return   �μ������붨��  
 *@ref        
 *@see
 *@note  
 */
	virtual u16 AddDropFrameTime(u32 DropFrameTime){return 0; }

/**
 *@brief ����moovˢ��Ƶ��
 *@param[in] u32 dwFrameInterval  ��Ƶ֡�����
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16 SetMoovFreq(IN u32 dwFrameInterval)  {return 0;}

/**
 *@brief ����ˢ����
 *@return     �μ������붨�� 
 *@ref        
 *@see
 *@note  
 */
	virtual u16 UpdateIndex() {return 0;}
	
/**
 *@brief �����ϲ��ӿ�
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note  
 */
	virtual u16 FixFile(IN const char * pFileName, IN const char * pMoovName)  {return 0;}
};

/** ��Ƶ������Ϣ*/
typedef struct tagAudioTypeInfo 
{
	u8 byAudioMediaType;///<��Ƶý������mp3,pcma,pcmu,g728,g729...
	u8 byAudioMode;///<��Ƶģʽ
}TAudioTypeInfo;

/** ��Ƶ������Ϣ*/
typedef union tagMediaDetailTypeIfo 
{
	u8 byVideoMediaType;///<��Ƶý������h261,h263,h264,mp4...
	TAudioTypeInfo tAudioTypeInfo;///<��Ƶģʽ
}UnionMediaDetailTypeInfo;

/**
 *@brief ��ҵ��Ļص�����
 *@param[in] u32 dwStatus  ����״̬
 *@param[in] KD_PTR pvUser
 *@param[in]u32 dwNextFrameTimeStamp ֡ʱ���
 *@param[in]TStreamDataPayload *piFrameData ֡���ݽṹ��
 *@return     
 *@ref        
 *@see
 *@note  
 */
typedef void (* ASFFrameProcCallBack)(KD_PTR pvUser, u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *piFrameData);

/** ���ű���*/
enum{
	NORMAL_SPEED		= 1,			///<�����ٶ�
	TWICE_SPEED			= 2,			///<������
	FOUR_TIMES_SPEED	= 4				///<�ı���
};



/**
 *@brief 
 *@return     
 *@ref        
 *@see
 *@note  reader�ӿڱص�
 */
void KdmFileStartUp();

/**
 *@brief 
 *@return     
 *@ref        
 *@see
 *@note  reader�ӿڱص�
 */
void KdmFileCleanUp();

/**
 *@brief �ƴ洢�ļ���ȡ����,�Ʒ��������ò�����������
 *@param[in] LPCSTR pszCloudConnectParamJsonForam  
 *@param[in] TKdmFile_CloudConnectParam *ptCloudConnectParam  
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note  
 */
u16 KdmFileCloudConnectParamParse(IN LPCSTR pszCloudConnectParamJsonForamt, TKdmFile_CloudConnectParam *ptCloudConnectParam);

/** ���ļ�*/
class IKdmFileReader
{
protected:

/**
 *@brief  ���ļ�����Constructors
 *@return     
 *@ref        
 *@see
 *@note       
 */
    IKdmFileReader(void) {};

		/**
 *@brief  ���ļ�����destructor
 *@return     
 *@ref        
 *@see
 *@note       
 */
    virtual ~IKdmFileReader(void) {};
    
public:    

/**
 *@brief  �����ļ�
 *@param[in]  pFileWriter
 *@param[in]  u32 type��type����ASF��MP4
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note       
 */
    static u16 CreateNew(IKdmFileReader** pFileReader, u32 type);

		/**
 *@brief  �ͷ�д����
 *@param[in]  pFileWriter
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note       
 */
    static u16 Release(IKdmFileReader* pFileReader);

		/**
 *@brief  �����ļ�
 *@param[in]  pFileWriter
 *@param[in]  LPCSTR pFilePath��pFilePath�ļ�·��
 *@return      �μ������붨��
 *@ref        
 *@see
 *@note  �����ļ��ַ�������3���ַ��ж�Ӧ�ô���ʲô���͵��ļ���ʽ������Ӧ 
 */
    static u16 CreateNew(IKdmFileReader** pFileReader, LPCSTR pFilePath);

/**
 *@brief  ��ASF/MP4�ļ��ĳ�ʼ��������ͬʱ���ݶ�ȡ�ƴ洢�ļ��ͱ����ļ�
 *@param[in] pYunConfPara:�Ʒ��������ò���
 *@param[in] pFileName���ļ�·����
 *@param[in] inCallback�����ݻص�����ָ��
 *@param[in]pvContext:�û�����
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   : 1.���ļ�·��ʾ����cloud://con_cate/con_name/blob_cate/blob_name��
            2.�����ļ��Ļ����贫����ȷ���Ʒ�����������ò���pYunConfPara�������ȡʧ�ܣ�
 */
	virtual u16	CloudOpen(IN TKdmFile_CloudConnectParam* ptCloudConnectParam, IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN void* pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

/**
 *@brief  дASF�ļ��ĳ�ʼ������
 *@param[in] LPCSTR pFileName  ������Ҫ������ASF�ļ���
 *@param[in] TKsjHeader * ptKsjHeader 
 *@param[in] u32 dwCreateTime ����ʱ��
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   ��ASF�ļ��ĳ�ʼ������������ж��Ƿ�ΪASF�ļ�������ȡASF�ļ���ͷ��Ϣ��ֻ֧�ִ򿪱����ļ�
 */
	virtual u16	Open(IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN KD_PTR pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

/**
 *@brief  ���ô���֡���ݵĻص�����
 *@param[in] KD_PTR pvUser  �û���ָ��
 *@param[in] ASFFrameProcCallBack inCallback  ���ûص��������û���ָ��
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	SetCallback(IN KD_PTR pvUser, IN ASFFrameProcCallBack inCallback) = 0;	
	
/**
 *@brief  ��ȡASF�ļ���������Ϣ
 *@param[out] TASFFileProperty * pFileProperty  �����ļ����Խṹ��ֵ
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty) = 0;
 
/**
 *@brief  ��ȡASF�ļ��������ĸ���
 *@param[out] TASFFileProperty * pFileProperty  �ļ�����
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetStreamTotal(OUT u16 *wStreamCount) = 0;


	/**
 *@brief  �ж��ļ��Ƿ�����˾��mp4�ļ�
 *@param[out] IsKedacomFile  
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	CopyRight(OUT  BOOL32 *IsKedacomFile) { return 0; };
	
	
/**
 *@brief  �õ���Ӧ�ֶεĳ���
 *@param[in]u8 byinStreamIndex  ����־
 *@param[out]u16 *poStreamTyp ������
 *@param[out]u32 *poErrorCorrectDataLength 
 *@param[out]u16 *poAudVidOthCodecSpecificLength
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength) = 0;

/**
 *@brief  ����������ȡASF�ļ���ÿ��������Ϣ
 *@param[in]u8 byinStreamIndex  ����־
 *@param[out]TStreamProperty * pioStreamProperty �����ļ������Խṹ��ֵ
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty) = 0;		
	
/**
 *@brief  ���ÿ�����źͱ���
 *@param[in]u16 wPlayMode  wPlayMode:����ģʽ�����ţ�����
 *@param[in]u32 wPlayRate  wPlayRate:�����ٶȣ���0��2��4
 *@return     ���ز���ģʽ
 *@ref        
 *@see
 *@note  
 */
    virtual u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate) = 0;

/**
 *@brief  �����������ʼʱ�䣬�첽��ȡASF�ļ���֡����
 *@param[in]bIsAutoCallBack �Ƿ��Զ��ص�
 *@param[in]u32 dwStartTime  ��ʼ����ʱ�䵥λ����
 *@param[in]BOOL32 bDragToKeyPoint �Ƿ�ӹؼ�λ�ÿ�ʼ�ص�
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   �Զ��ص��͵�֡�ص����л����������Զ��ص����ǵ�֡�ص������������ûص�������������ô˺�����
 */
	virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = TRUE) = 0;
	
/**
 *@brief  ��֡��ȡ������ÿ����һ��
 *@param[in]u32& dwStatus ����״̬
 *@param[in]TStreamDataPayload **ppoFrameData  ֡���ݽṹ��
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   ��֡�ص�ʱʹ�øú���
 */
	virtual u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData) = 0;

/**
 *@brief  ��ͣ�첽��ȡASF�ļ���֡����
 *@return      �μ������붨�� 
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Pause(void) = 0;

/**
 *@brief  ����ͣ��λ�������첽��ȡASF�ļ���֡����,���ɵ�֡�ص�ģʽ���ص��Զ��ص�ģʽ
 *@return    �μ������붨�� 
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Resume(void) = 0;

/**
 *@brief  ֹͣ�첽��ȡASF�ļ���֡����
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Stop(void) = 0;

/**
 *@brief  ֹͣ��ȡ���ݣ����ͷſռ�
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Close() = 0;	
    
/**
 *@brief  ��ȡASF�ļ�����ͷ����ĸ���
 *@param[out]u32 * dwHeadObjectCount ͷ����ĸ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetHeaderObjectTotal(OUT u32 * dwHeadObjectCount) { return 0; };
   
/**
 *@brief  ����ASF�ļ��Ļ�����Ƶ������Ч�����	
 *@param[in] BOOL32 isPlay 
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf
 */
    virtual u16	SetPlayAudioStream(IN BOOL32 isPlay = true ) { return 0; };

/**
 *@brief  �����Ƿ񲥷���Ƶ��
 *@param[in]BOOL32 isPlayFirstStream ��������������
 *@param[in]BOOL32 isPlaySecondStream ��������������
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   ��������������Ҫ����һ���������ô���ϵͳ�в���ǿ�Ʋ���һ������asf
 */
	virtual u16	SetPlayVideoStream(IN BOOL32 isPlayFirstStream = true, IN BOOL32 isPlaySecondStream = true ){ return 0; };
 
/**
 *@brief   ��ȡASF�ļ��ľ�������
 *@param[out]u16 *pErrorType ���ؾ������ݵ����� ERROR_CORRECTION_TYPE 
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf
 */
    virtual u16	GetErrorCorrectionType(OUT u16 *pErrorType) { return 0; };
	
/**
 *@brief   ��ȡASF�ļ��ľ������ݳ���
 *@param[out]u32 *pErrCorrDataLength ���ؾ������ݵĳ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetErrorCorrectionDataLength(OUT u32 *pErrCorrDataLength){ return 0; };
	 
/**
 *@brief    ��ȡASF�ļ��ľ�������
 *@param[out]u8 *pErrCorrData ��������
 *@param[in]u32 dwDataLength  ���ݳ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   ����������ݵĳ��ȣ����ؾ�������
 */
	virtual u16	GetErrorCorrectionData(OUT u8 *pErrCorrData, IN u32 dwDataLength){ return 0; };

/**
 *@brief    �����Ƿ���Ҫ����Ļ���������Ϣ
 *@param[in]u16 wObjectID �������������ֵ
 *@return     
 *@ref        
 *@see
 *@note   ������ΪASF_ObjectID_ALL_Objectʱ������Object��Ӧ��
               �ֲ���������Ϊtrue,����ֻ���ø�Object��Ӧ�ľֲ�����
 */
	virtual u16		SetDebugInfo(IN u16 wObjectID){ return 0; };

/**
 *@brief    �������ASF�ļ������ݶ�����������Ϣ��Table Of Content��
 *@return     wu
 *@ref        
 *@see
 *@note   
 */
	virtual u16		DumpObjectTOC(void){ return 0; };
	
/**
 *@brief    ���ݺ����ķ���ֵ��ȡ��ϸ�Ĵ�����Ϣ
 *@param[in]u16 wErrorIndex ������
 *@return     ������������Χ����������ΪNULL�����򷵻ش�����ϸ��Ϣ
 *@ref        
 *@see
 *@note   asf
 */
	virtual LPSTR GetErrorInfo(IN u16 wErrorIndex){ return 0; };

/**
 *@brief    ר������ʾ���������ݡ���ÿ���ֽڵ�ֵ������ʮ�����Ƶ�������ʾ������ÿ����ʾ16�����ݡ�ÿ���ֽ�һ����Ԫ��ÿ8���ֽ��зָ���
 *@param[in]u8 * pData  ���������ݵ�ָ��
 *@param[in]u32 dwDataLength  ���ݳ���
 *@return     wu 
 *@ref        
 *@see
 *@note   asf
 */
	virtual void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength){ };

/**
 *@brief    ���ݱ������ID���ó���Ӧ��ý������
 *@param[in]u8 byiStreamType  �����ͣ���Video_Media_Type��Audio_Media_Type
 *@param[in]u32 dwiCodecFormatID  �������id
 *@param[out]UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo����ľ���ý�����ͣ���h261,h263,pcma,pcmu,g728...
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf 
 */
	virtual u16	CodecFormatId2MediaType(IN u8 byiStreamType,IN u32 dwiCodecFormatID,OUT UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo){ return 0; };
 
/**
 *@brief    �õ���Ӧ�ֶεĳ���
 *@param[out]u16 *poTitleLength      
 *@param[out]u16 *poAuthorLength
 *@param[out]u16 *poCopyrightLength
 *@param[out]u16 *poDescriptionLength
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetContentDescriptionPropertyFieldLength(OUT u16 *poTitleLength,OUT u16 *poAuthorLength, OUT u16 *poCopyrightLength,OUT u16 *poDescriptionLength){ return 0; };
		
/**
 *@brief   ��ȡASF�ļ������ݰ�Ȩ��Ϣ
 *@param[out]TContentDescriptionProperty * pioContentDescriptionProperty �����ļ��������Խṹ��ֵ 
 *@return    �μ������붨��
 *@ref        
 *@see
 *@note   ���������úó��ȣ�������ڴ� asf
 */
	virtual u16	GetContentDescriptionProperty(IN OUT TContentDescriptionProperty * pioContentDescriptionProperty){ return 0; };

/**
 *@brief   ��ȡ��Ƶ���һ֡�ؼ�֡ʱ���
 *@param[out]s64 *poLastKeyFrameTime ���һ֡�ؼ�֡ʱ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note   mp4
 */
	virtual u16 GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime){return 0;};
	/**
 *@brief   ��ȡ��Ƶ���һ֡��Ƶ֡ʱ���
 *@param[out]s64 *poLastKeyFrameTime ���һ֡��Ƶ֡ʱ���
 *@return     �μ������붨��
 *@ref        
 *@see
 *@note  asf mp4
 */
	virtual u16 GetLastVideoFrameTime(OUT s64 *poLastVideoFrameTime){return 0;};
};

#if defined(__Win32__) || defined(WIN32)
#pragma pack(pop)
#endif	//WIN32

#endif
