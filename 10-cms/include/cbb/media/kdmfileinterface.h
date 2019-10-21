/**
* @file kdmfileinterface.h  
* @brief    设备接入管理   http://172.16.6.123/svn/protocol_cbb/
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


/** 表示函数的参数是输入参数*/
#define OUT		

/** 表示函数的参数是输出参数*/
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



#define KDM_FILE_AUDIO_G711A (u16)0x06       ///<目前文件库支持的音频类型_G711A
#define KDM_FILE_AUDIO_G711U (u16)0x07	     ///<目前文件库支持的音频类型_G711U
#define KDM_FILE_AUDIO_ADPCM (u16)0x11	     ///<目前文件库支持的音频类型_ADPCM
#define KDM_FILE_AUDIO_G7221C  (u16)0x70	 ///<目前文件库支持的音频类型_G7221C
#define KDM_FILE_AUDIO_AAC_LC (u16)0xA106	 ///<目前文件库支持的音频类型_AAC_LC
#define KDM_FILE_STANDARD_AAC_LC (u16)0xFF   ///<目前文件库支持的音频类型_STANDARD_AAC_LC 
#define KDM_FILE_AUDIO_OPUS (u16)0x00F0	     ///<目前文件库支持的音频类型 Opus

#endif

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	用一个字节的结构对齐，防止不同的编译器导致错误
#endif	//WIN32
/** 支持4K 定义关键帧最大的分配内存*/
#define MAX_VIDEO_FRAME_SIZE_FOR4K   (2*1024*1024)  

/** 枚举类型区分asf和MP4*/
enum 
{
    ASF_File_Type  = 0,
    MP4_File_Type  = 1,    
};

/** 媒体类型*/
typedef u16 ASF_MEDIA_TYPE;

/** 写文件操作*/
class IKdmFileWriter
{
protected:
	
/**
 *@brief  写文件操作Constructors
 *@return     
 *@ref        
 *@see
 *@note       
 */
    IKdmFileWriter(void){};

/**
 *@brief  写文件操作destructor
 *@return     
 *@ref        
 *@see
 *@note       
 */
    virtual ~IKdmFileWriter(void){};
public:

/**
 *@brief  创建文件
 *@param[in]  pFileWriter
 *@param[in]  u32 type，type区分ASF和MP4
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note       
 */
    static u16 CreateNew(IKdmFileWriter** pFileWriter, u32 type);  

/**
 *@brief  释放写对象
 *@param[in]  pFileWriter
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note       
 */
    static u16 Release(IKdmFileWriter* pFileWriter);
	/**
 *@brief  创建文件
 *@param[in]  pFileWriter
 *@param[in]  LPCSTR pFilePath，pFilePath文件路径
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  根据文件字符串最后的3个字符判断应该创建什么类型的文件格式，自适应 
 */
    static u16 CreateNew(IKdmFileWriter** pFileWriter, LPCSTR pFilePath);

/**
 *@brief  写ASF文件的初始化操作
 *@param[in] LPCSTR pFileName  文件名
 *@param[in] TKsjHeader * ptKsjHeader 
 *@param[in] u32 dwCreateTime 创建时间
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16	Open(IN LPCSTR pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0) = 0;
			
	/**
 *@brief  添加流属性
 *@param[in] pStreamProperty  流属性结构
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  AddStream(IN TStreamProperty * pStreamProperty) = 0; 
	virtual u16  SetMp4MdatSize(u32 dwMdatSize) {return 0;};
	virtual u16  SetMp4BufferSize(u32 dwBufferSize) {return 0;};
/**
 *@brief  开始写ASF文件的数据部分
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  BeginWriting(void) = 0;

	/**
 *@brief  按照不同的流序号写帧数据
 *@param[in] u8 byStreamNumber  流序号
 *@param[in] u8 * pDataBuffer   帧数据指针
 *@param[in] u32 dwBufferSize   帧数据大小
 *@param[in] BOOL32 bIsKeyFrame 是否是关键帧
 *@param[in] u32 dwTimeStamp    时间戳
 *@param[in] u16 wDuration      持续时间，文件库里没有用到
 *@param[out] u32* pdwFileSize = NULL
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
			        		 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
                             OUT u32* pdwFileSize = NULL) = 0;
	 
/**
 *@brief  结束写帧数据
 *@return    参见错误码定义 
 *@ref        
 *@see
 *@note  
 */
	virtual u16  EndWriting() = 0;
	
/**
 *@brief  关闭文件，释放资源空间
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  Close() = 0;

   
/**
 *@brief  设置ASF文件的属性信息
 *@param[in] TASFFileProperty * pFileProperty    输入文件属性
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  适用于ASF
 */
	virtual u16	SetASFFileProperty(IN TASFFileProperty * pFileProperty){ return 0; }
	 
	/**
 *@brief  设置ASF文件的内容版权信息
 *@param[in]TContentDescriptionProperty * pContentDescriptionProperty   输入文件内容属性结构的值
 *@return   参见错误码定义  
 *@ref        
 *@see
 *@note  适用于ASF
 */
	virtual u16	SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty){ return 0; }

	
/**
 *@brief  设置是否生成索引对象
 *@param[in] BOOL32 isGenarate   
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  SetGenerateSimpleIndexObject(IN BOOL32 isGenarate = false){ return 0; }


	 
/**
 *@brief 设置是否需要在屏幕输出调试信息
 *@param[in] u16 wObjectID   输入对象ID
 *@return     
 *@ref        
 *@see
 *@note  
 */
	virtual u16  SetDebugInfo(IN u16 wObjectID) { return 0; }
 
/**
 *@brief 根据函数的返回值获取详细的错误信息
 *@param[in] u16 wErrorIndex 输入错误码索引值  
 *@return     若索引超出范围，函数返回为NULL，否则返回错误详细信息
 *@ref        
 *@see
 *@note  
 */
	virtual LPSTR GetErrorInfo(IN u16 wErrorIndex) { return 0; }
 
/**
 *@brief 专用于显示二进制数据、将每个字节的值用两个十六进制的数字显示出来
 *@param[in] u8 * pData  帧数据
 *@param[in] u32 dwDataLength  帧长度
 *@return     
 *@ref        
 *@see
 *@note  
 */
	virtual void ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength) {  }
 
/**
 *@brief 打印写文件的操作步骤
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16  PrintAllStep() { return 0; }
	
/**
 *@brief 增加下载时开始时间到录像第一帧时间
 *@param[in] u32 DropFrameTime 
 *@return   参见错误码定义  
 *@ref        
 *@see
 *@note  
 */
	virtual u16 AddDropFrameTime(u32 DropFrameTime){return 0; }

/**
 *@brief 设置moov刷新频率
 *@param[in] u32 dwFrameInterval  视频帧间隔数
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16 SetMoovFreq(IN u32 dwFrameInterval)  {return 0;}

/**
 *@brief 立即刷索引
 *@return     参见错误码定义 
 *@ref        
 *@see
 *@note  
 */
	virtual u16 UpdateIndex() {return 0;}
	
/**
 *@brief 索引合并接口
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note  
 */
	virtual u16 FixFile(IN const char * pFileName, IN const char * pMoovName)  {return 0;}
};

/** 音频类型信息*/
typedef struct tagAudioTypeInfo 
{
	u8 byAudioMediaType;///<音频媒体类型mp3,pcma,pcmu,g728,g729...
	u8 byAudioMode;///<音频模式
}TAudioTypeInfo;

/** 视频类型信息*/
typedef union tagMediaDetailTypeIfo 
{
	u8 byVideoMediaType;///<视频媒体类型h261,h263,h264,mp4...
	TAudioTypeInfo tAudioTypeInfo;///<音频模式
}UnionMediaDetailTypeInfo;

/**
 *@brief 给业务的回调函数
 *@param[in] u32 dwStatus  数据状态
 *@param[in] KD_PTR pvUser
 *@param[in]u32 dwNextFrameTimeStamp 帧时间戳
 *@param[in]TStreamDataPayload *piFrameData 帧数据结构体
 *@return     
 *@ref        
 *@see
 *@note  
 */
typedef void (* ASFFrameProcCallBack)(KD_PTR pvUser, u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *piFrameData);

/** 播放倍数*/
enum{
	NORMAL_SPEED		= 1,			///<正常速度
	TWICE_SPEED			= 2,			///<两倍速
	FOUR_TIMES_SPEED	= 4				///<四倍速
};



/**
 *@brief 
 *@return     
 *@ref        
 *@see
 *@note  reader接口必调
 */
void KdmFileStartUp();

/**
 *@brief 
 *@return     
 *@ref        
 *@see
 *@note  reader接口必调
 */
void KdmFileCleanUp();

/**
 *@brief 云存储文件读取功能,云服务器配置参数解析函数
 *@param[in] LPCSTR pszCloudConnectParamJsonForam  
 *@param[in] TKdmFile_CloudConnectParam *ptCloudConnectParam  
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note  
 */
u16 KdmFileCloudConnectParamParse(IN LPCSTR pszCloudConnectParamJsonForamt, TKdmFile_CloudConnectParam *ptCloudConnectParam);

/** 读文件*/
class IKdmFileReader
{
protected:

/**
 *@brief  读文件操作Constructors
 *@return     
 *@ref        
 *@see
 *@note       
 */
    IKdmFileReader(void) {};

		/**
 *@brief  读文件操作destructor
 *@return     
 *@ref        
 *@see
 *@note       
 */
    virtual ~IKdmFileReader(void) {};
    
public:    

/**
 *@brief  创建文件
 *@param[in]  pFileWriter
 *@param[in]  u32 type，type区分ASF和MP4
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note       
 */
    static u16 CreateNew(IKdmFileReader** pFileReader, u32 type);

		/**
 *@brief  释放写对象
 *@param[in]  pFileWriter
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note       
 */
    static u16 Release(IKdmFileReader* pFileReader);

		/**
 *@brief  创建文件
 *@param[in]  pFileWriter
 *@param[in]  LPCSTR pFilePath，pFilePath文件路径
 *@return      参见错误码定义
 *@ref        
 *@see
 *@note  根据文件字符串最后的3个字符判断应该创建什么类型的文件格式，自适应 
 */
    static u16 CreateNew(IKdmFileReader** pFileReader, LPCSTR pFilePath);

/**
 *@brief  读ASF/MP4文件的初始化操作，同时兼容读取云存储文件和本地文件
 *@param[in] pYunConfPara:云服务器配置参数
 *@param[in] pFileName：文件路径；
 *@param[in] inCallback：数据回调函数指针
 *@param[in]pvContext:用户数据
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   : 1.云文件路径示例：cloud://con_cate/con_name/blob_cate/blob_name；
            2.读云文件的话，需传入正确的云服务器相关配置参数pYunConfPara，否则读取失败；
 */
	virtual u16	CloudOpen(IN TKdmFile_CloudConnectParam* ptCloudConnectParam, IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN void* pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

/**
 *@brief  写ASF文件的初始化操作
 *@param[in] LPCSTR pFileName  输入需要创建的ASF文件名
 *@param[in] TKsjHeader * ptKsjHeader 
 *@param[in] u32 dwCreateTime 创建时间
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   读ASF文件的初始化操作，完成判断是否为ASF文件，并读取ASF文件的头信息，只支持打开本地文件
 */
	virtual u16	Open(IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN KD_PTR pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL) = 0;

/**
 *@brief  设置处理帧数据的回调函数
 *@param[in] KD_PTR pvUser  用户空指针
 *@param[in] ASFFrameProcCallBack inCallback  设置回调函数和用户空指针
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	SetCallback(IN KD_PTR pvUser, IN ASFFrameProcCallBack inCallback) = 0;	
	
/**
 *@brief  获取ASF文件的属性信息
 *@param[out] TASFFileProperty * pFileProperty  返回文件属性结构的值
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty) = 0;
 
/**
 *@brief  获取ASF文件所有流的个数
 *@param[out] TASFFileProperty * pFileProperty  文件属性
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetStreamTotal(OUT u16 *wStreamCount) = 0;


	/**
 *@brief  判断文件是否是我司的mp4文件
 *@param[out] IsKedacomFile  
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	CopyRight(OUT  BOOL32 *IsKedacomFile) { return 0; };
	
	
/**
 *@brief  得到对应字段的长度
 *@param[in]u8 byinStreamIndex  流标志
 *@param[out]u16 *poStreamTyp 流类型
 *@param[out]u32 *poErrorCorrectDataLength 
 *@param[out]u16 *poAudVidOthCodecSpecificLength
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength) = 0;

/**
 *@brief  根据索引获取ASF文件的每个流的信息
 *@param[in]u8 byinStreamIndex  流标志
 *@param[out]TStreamProperty * pioStreamProperty 返回文件流属性结构的值
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty) = 0;		
	
/**
 *@brief  设置快放慢放和倍数
 *@param[in]u16 wPlayMode  wPlayMode:播放模式，如快放，慢放
 *@param[in]u32 wPlayRate  wPlayRate:播放速度，如0，2，4
 *@return     返回播放模式
 *@ref        
 *@see
 *@note  
 */
    virtual u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate) = 0;

/**
 *@brief  根据输入的起始时间，异步读取ASF文件的帧数据
 *@param[in]bIsAutoCallBack 是否自动回调
 *@param[in]u32 dwStartTime  开始播放时间单位毫秒
 *@param[in]BOOL32 bDragToKeyPoint 是否从关键位置开始回调
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   自动回调和单帧回调可切换，不管是自动回调还是单帧回调，都必须设置回调函数，必须调用此函数。
 */
	virtual u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = TRUE) = 0;
	
/**
 *@brief  单帧获取函数，每调用一次
 *@param[in]u32& dwStatus 播放状态
 *@param[in]TStreamDataPayload **ppoFrameData  帧数据结构体
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   单帧回调时使用该函数
 */
	virtual u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData) = 0;

/**
 *@brief  暂停异步读取ASF文件的帧数据
 *@return      参见错误码定义 
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Pause(void) = 0;

/**
 *@brief  由暂停的位置重新异步读取ASF文件的帧数据,或由单帧回调模式返回到自动回调模式
 *@return    参见错误码定义 
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Resume(void) = 0;

/**
 *@brief  停止异步读取ASF文件的帧数据
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Stop(void) = 0;

/**
 *@brief  停止读取数据，并释放空间
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   
 */
	virtual u16	Close() = 0;	
    
/**
 *@brief  获取ASF文件所有头对象的个数
 *@param[out]u32 * dwHeadObjectCount 头对象的个数
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetHeaderObjectTotal(OUT u32 * dwHeadObjectCount) { return 0; };
   
/**
 *@brief  设置ASF文件的互斥音频流的有效流序号	
 *@param[in] BOOL32 isPlay 
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf
 */
    virtual u16	SetPlayAudioStream(IN BOOL32 isPlay = true ) { return 0; };

/**
 *@brief  设置是否播放视频流
 *@param[in]BOOL32 isPlayFirstStream 输入给定的流序号
 *@param[in]BOOL32 isPlaySecondStream 输入给定的流序号
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   （所有流中至少要播放一个流，设置错误系统有策略强制播放一个流）asf
 */
	virtual u16	SetPlayVideoStream(IN BOOL32 isPlayFirstStream = true, IN BOOL32 isPlaySecondStream = true ){ return 0; };
 
/**
 *@brief   获取ASF文件的纠错类型
 *@param[out]u16 *pErrorType 返回纠错数据的类型 ERROR_CORRECTION_TYPE 
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf
 */
    virtual u16	GetErrorCorrectionType(OUT u16 *pErrorType) { return 0; };
	
/**
 *@brief   获取ASF文件的纠错数据长度
 *@param[out]u32 *pErrCorrDataLength 返回纠错数据的长度
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetErrorCorrectionDataLength(OUT u32 *pErrCorrDataLength){ return 0; };
	 
/**
 *@brief    获取ASF文件的纠错数据
 *@param[out]u8 *pErrCorrData 纠错数据
 *@param[in]u32 dwDataLength  数据长度
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   输入纠错数据的长度，返回纠错数据
 */
	virtual u16	GetErrorCorrectionData(OUT u8 *pErrCorrData, IN u32 dwDataLength){ return 0; };

/**
 *@brief    设置是否需要在屏幕输出调试信息
 *@param[in]u16 wObjectID 输入错误码索引值
 *@return     
 *@ref        
 *@see
 *@note   当输入为ASF_ObjectID_ALL_Object时，所有Object对应的
               局部变量设置为true,否则只设置该Object对应的局部变量
 */
	virtual u16		SetDebugInfo(IN u16 wObjectID){ return 0; };

/**
 *@brief    输出整个ASF文件的内容对象索引表信息（Table Of Content）
 *@return     wu
 *@ref        
 *@see
 *@note   
 */
	virtual u16		DumpObjectTOC(void){ return 0; };
	
/**
 *@brief    根据函数的返回值获取详细的错误信息
 *@param[in]u16 wErrorIndex 错误码
 *@return     若索引超出范围，函数返回为NULL，否则返回错误详细信息
 *@ref        
 *@see
 *@note   asf
 */
	virtual LPSTR GetErrorInfo(IN u16 wErrorIndex){ return 0; };

/**
 *@brief    专用于显示二进制数据、将每个字节的值用两个十六进制的数字显示出来，每行显示16个数据、每个字节一个单元，每8个字节有分隔符
 *@param[in]u8 * pData  二进制数据的指针
 *@param[in]u32 dwDataLength  数据长度
 *@return     wu 
 *@ref        
 *@see
 *@note   asf
 */
	virtual void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength){ };

/**
 *@brief    根据编解码器ID，得出相应的媒体类型
 *@param[in]u8 byiStreamType  流类型，如Video_Media_Type，Audio_Media_Type
 *@param[in]u32 dwiCodecFormatID  编解码器id
 *@param[out]UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo输出的具体媒体类型，如h261,h263,pcma,pcmu,g728...
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf 
 */
	virtual u16	CodecFormatId2MediaType(IN u8 byiStreamType,IN u32 dwiCodecFormatID,OUT UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo){ return 0; };
 
/**
 *@brief    得到对应字段的长度
 *@param[out]u16 *poTitleLength      
 *@param[out]u16 *poAuthorLength
 *@param[out]u16 *poCopyrightLength
 *@param[out]u16 *poDescriptionLength
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   asf
 */
	virtual u16	GetContentDescriptionPropertyFieldLength(OUT u16 *poTitleLength,OUT u16 *poAuthorLength, OUT u16 *poCopyrightLength,OUT u16 *poDescriptionLength){ return 0; };
		
/**
 *@brief   获取ASF文件的内容版权信息
 *@param[out]TContentDescriptionProperty * pioContentDescriptionProperty 返回文件内容属性结构的值 
 *@return    参见错误码定义
 *@ref        
 *@see
 *@note   参数需设置好长度，分配好内存 asf
 */
	virtual u16	GetContentDescriptionProperty(IN OUT TContentDescriptionProperty * pioContentDescriptionProperty){ return 0; };

/**
 *@brief   获取视频最后一帧关键帧时间戳
 *@param[out]s64 *poLastKeyFrameTime 最后一帧关键帧时间戳
 *@return     参见错误码定义
 *@ref        
 *@see
 *@note   mp4
 */
	virtual u16 GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime){return 0;};
	/**
 *@brief   获取视频最后一帧视频帧时间戳
 *@param[out]s64 *poLastKeyFrameTime 最后一帧视频帧时间戳
 *@return     参见错误码定义
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
