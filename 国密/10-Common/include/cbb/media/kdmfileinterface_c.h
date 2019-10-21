
#ifndef _FILE_INTERFACE_DLL_H_
#define _FILE_INTERFACE_DLL_H_

#define OUT		// 表示函数的参数是输入参数
#define IN		// 表示函数的参数是输出参数

#include "kdvtype.h"
#include "kdvdef.h"

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	用一个字节的结构对齐，防止不同的编译器导致错误
#endif	//WIN32

#ifndef EVEREY_FILE_TYPE
#define EVEREY_FILE_TYPE	
//  流的类型
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
	TKdmFile_IPAddr   m_tSecondIpAddr;             /*可以不存在*/
	u16				m_wPort;
}TKdmFile_NetAddr;

typedef struct
{
	u16				m_wNetAddrNum;							/*需要连接的云地址数量*/
	TKdmFile_NetAddr* m_ptNetAddr;								/*数组，需要连接的云地址信息，由业务上层分配*/
	s8           m_szMachineID[KDMFILE_MAXLEN_MACHINEID+1];		/*机器ID， 即UUID*/        
	s8           m_szZoneName[KDMFILE_MAXLEN_ZONENAME+1];		/*域名*/
	s8           m_szUserName[KDMFILE_MAXLEN_USERNAME+1];		/*连接云的用户名*/
	s8           m_szPasswd[KDMFILE_MAXLEN_PASSWD+1];           /*连接云的密码*/
	u32          m_dwTimeOut;									/*连接超时时间*/
	s32          m_nDataChunk;								/*RAID的数据块数量，设置为0，表示使用云的默认配置*/
	s32          m_nParityChunk;								/*RAID的数据校验块数量，设置为0，表示使用云的默认配置*/
}TKdmFile_CloudConnectParam;

//  文件属性相关结构 
typedef struct tagASFFileProperty{
	u64		qwTotalFileSize;				// 文件大小
	u64		qwCreateDate;					// 文件创建时间，采用FILETIME格式
	u64		qwDataPacketsCount;				// 包含的数据包个数
	u64		qwPlayDuration;					// 文件播放的总时间长度(100nano-second)
	u64		qwSendDuration;					// 文件发送的总时间长度(100nano-second)
	u64		qwPreroll;					// 文件起始的偏移时间
	BOOL32		bIsBroadCast;					// 文件/现场流的标识
	u32		dwDataPacketSize;				// 包含数据包的大小
	u32		dwMaximumBitrate;				// 最大比特率 bits per second for entire file.
	u16		wAudioStreamCount;				// 音频流的数目
	u16		wVideoStreamCount;				// 视频流的数目
}TASFFileProperty;

//  内容描述属性结构 
typedef struct tagContentDescriptionProperty{
	u16		wTitleLength;					// 内容标题长度
	u16		wAuthorLength;				// 内容作者长度
	u16		wCopyrightLength;				// 内容版权长度
	u16		wDescriptionLength;			// 内容描述长度
	u16*		pwTitle;						// Unicode字符串表示的内容标题
	u16*		pwAuthor;					// Unicode字符串表示的内容作者
	u16*		pwCopyright;					// Unicode字符串表示的内容版权
	u16*		pwDescription;				// Unicode字符串表示的内容描述信息
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
	u32		dwFormatDataSize;				// 视频属性数据大小
	s32		dwImageWidth;					// 视频画面宽度
	s32		dwImageHeight;					// 视频画面高度
	u16		wReserved;						// 保留数据
	u16		wBitsPerPixelCount;				// 每象素的比特数
	u32		dwCompressionID;				// 视频压缩Codec标识
	u32		dwImageSize;					// 画面尺寸
	s32		nHorizontalPixelsPerMeter;		// 水平分辨率
	s32		nVerticalPixelsPerMeter;		// 垂直方向分辨率
	u32		dwColorsUsedCount;				// 调色板
	u32		dwImportantColorsCount;			// 
	u8*		pbyCodecSpecificData;			// Codec 扩展信息
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
	u16		wCodecID_FormatTag;				// 音频Codec标识
	u16		wNumberOfChannels;				// 音频通道数				
	u32		dwSamplesPerSecond;				// 采样率
	u32		dwAverageNumberOfBytesPerSecond;// 平均码率 Bytes / Second
	u16		wBlockAlignment;				// 块对齐字节数 
	u16		wBitsPerSample;					// 每个采样的比特数常为8或者16
	u16		wCodecSpecificDataSize;			// 音频Codec标识
	u16		wReserve;
	u8*		pbyCodecSpecificData;			// 音频Codec标识
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;							// 相当于 WAVEFORMATEX 结构
typedef struct tagAudioMediaInfo TAudioMediaInfo;
#endif

#ifndef VIDEOMEDIAINFO
#define VIDEOMEDIAINFO
struct tagVideoMediaInfo{
	u32		dwEncodedImageWidth;			// 视频画面宽度
	u32		dwEncodedImageHeight;			// 视频画面宽
	u16		wFormatDataSize;				// 视频属性数据大小
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
	u32		dwInfoLength;					// 其它媒体数据长度
	u8		*pbyInfoData;					// 其它媒体数据指针
} 
//#if defined(_VXWORKS_) || defined(_LINUX_)
//__attribute__ ( (packed) ) 
//#endif
TOtherMediaInfo;

#endif

#ifndef STREAMPROPERTY
#define STREAMPROPERTY
//  流属性结构 
struct tagStreamProperty{
	u16		wStreamType;					// 流类型
	u8		byStreamNumber;					// 流序号
	u8		byReserve;
	u16		wErrorCorrectType;				// 纠错数据类型	
	u16		wResever;
	u32		dwErrorCorrectDataLength;		// 纠错数据类型	
	u8		*pbyErrorCorrectData;			// 纠错数据类型	
	BOOL32	bIsEncryptedContent;			// 是否对内容采用了熵编码
	u32		dwAverageBitrate;				// 平均比特率  bits/s
    u32     dwFrameRate;
	u32     dwFrameNumbers;   
	union{
		TAudioMediaInfo	 tAudioMediaInfo;   // 音频数据属性
		TVideoMediaInfo	 tVideoMediaInfo;	// 视频数据属性
		TOtherMediaInfo	 tOtherMediaInfo;	// 其它媒体数据属性
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
	u8		byStreamNumber;				// 流序号			
	u32		dwMediaObjectNumber;			// 帧序号
	BOOL32	bIsKeyFrame;					// 是否为关键帧
	u32		dwPresentationTime;			// 帧时间信息（时间戳）
	u32		dwPayloadDataSize;			// 数据长度
	u8		*pbyPayloadData;				// 有效数据

}TStreamDataPayload;

//  函数返回的错误码
#ifndef __ASF_ERROR_CODE__
#define __ASF_ERROR_CODE__
enum
{
	ASF_Error_NoError				= 0,   // 没有错误，正确返回
	ASF_Error_FileNotFound			= 1,   // 文件没有找到
	ASF_Error_InvalidASFFile		= 2,	// 无效的ASF文件
	ASF_Error_ObjectNotFound 		= 3,	// 对象没有找到
	ASF_Error_BadIndex			= 4,	// 错误的对象索引
	ASF_Error_ValueNotFound		= 5,	// 没有找到相应的值
	ASF_Error_BadArgument			= 6,	// 错误的参数
	ASF_Error_ReadOnly			= 7,	// 文件只读错误
	ASF_Error_NotEnoughSpace		= 8,	// 存储空间不足错误
	ASF_Error_InternalError  		= 9,	// 无法申请内存的内部错误
	ASF_Error_ReadFileError  		= 10,	// 读文件内部错误
	ASF_Error_WriteFileError  		= 11,	// 写文件内部错误
	ASF_Error_SeekFileError  		= 12,	// 查找文件错误
	ASF_Error_CreateFileError  	= 13,	// 创建文件错误
	ASF_Error_CreateThreadError	= 14,	// 创建文件错误
	ASF_Error_ObjectNULLError		= 15,	// 参数指针为空的错误
	ASF_Error_TooMuchVideoStrream	= 16,	//
	ASF_Error_NotSupportedCodec	= 17,	//
	ASF_Error_FileEnd				= 18,	//
	ASF_Error_UnknownError			= 19,	// 无法确定的错误		
	ASF_Error_InvalidKsjFile		= 20,	// 不是ksj 文件
	ASF_Error_DiskFull			= 21,	//磁盘满错误
	ASF_Error_FrameCheckError		= 22	,//帧校验错误
	ASF_Error_ReadDiskError		= 23,	//读磁盘错误
	ASF_Error_AsfLibNotStartUp     = 24,   //Asflib库没有初始化
	ASF_Error_TimerInitError       = 25,   //定时器线
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

//播放模式，正常，快放，慢放
//05.7.12
enum {
	NORMAL_PLAY         = 0,
	FAST_PLAY           = 1,
	SLOW_PLAY           = 2,
	CANCEL_TIMER        = 3
};


//读或播放时的状态
enum{
	ASF_Read_Status_Wait			= 0,
	ASF_Read_Status_Playing			= 1,
	ASF_Read_Status_EOF				= 2,
	ASF_Read_Status_Closed			= 3,
	ASF_Read_Status_SINGLE_FRAME	= 4	//单帧调用05.8.11
};
typedef u16	ASF_READ_STATUS;

typedef void (* ASFFrameProcCallBack)(KD_PTR pvUser, u32 dwStatus, u32 dwNextFrameTimeStamp, TStreamDataPayload *piFrameData);

API u16	File_Open(IN const char * pFileName, TKsjHeader * ptKsjHeader, void **pWriter);
//API u16	File_OpenByType(IN const char * pFileName,u32 type, TKsjHeader * ptKsjHeader, void **pWriter);
API u16	KdmFile_Open(IN const char * pFileName, TKsjHeader * ptKsjHeader, u32 dwCreateTime, void **pWriter);
API u16	KdmFile_OpenByType(IN const char * pFileName, TKsjHeader * ptKsjHeader, u32 dwCreateTime, void **pWriter,u32 type);
//  设置ASF文件的属性信息
API u16 File_SetASFFileProperty(IN TASFFileProperty * pFileProperty, void *pWriter);
API u16  File_SetMp4MdatSize(u32 dwMdatSize ,void *pWriter) ;
//  设置ASF文件的内容版权信息
API u16 File_SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty, void *pWriter);

//  将设置好的流添加到文件头中
API u16 File_AddStream(IN TStreamProperty * pStreamProperty, void *pWriter);
API u16 File_SetMp4BufferSize(u32 dwBufferSize, void *pWriter) ;

//  开始写ASF文件的数据部分
API u16  File_BeginWriting(void *pWriter);

//  按照不同的流序号写帧数据
API u16  File_WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
						  IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, 
						  OUT u32* pdwFileSize, void *pWriter);
//  结束写帧数据
API u16  File_EndWriting(void *pWriter);

//  关闭文件，释放资源空间
API u16  File_Close(void *pWriter);

//DropFrameTime单位必须是秒，否则会导致时间错误
API u16 File_AddDropFrameTime(void *pWriter, u32 DropFrameTime);

//设置moov刷新频率，参数为视频帧间隔数
API u16 File_SetMoovFreq(void *pWriter, IN u32 dwFrameInterval);

//立即刷索引
API u16 File_UpdateIndex(void *pWriter);

//此接口仅供合并索引文件用
API u16	File_Create_FixFile(void **pWriter);

//索引合并接口
API u16 File_FixFile(void *pWriter, IN const char * pFileName, IN const char * pMoovName);

//此接口仅供合并索引文件用
API u16 File_Release_FixFile(void *pWriter);

//read
//reader接口必调
API void File_KdmFileStartUp();
API void File_KdmFileCleanUp();


API u16	File_Create_Read(IN const char * pFileName, void **pReader);
API u16 File_Create_ReadByType(IN u32 type, void **pReader);
//  读ASF文件的初始化操作，完成判断是否为ASF文件，并读取ASF文件的头信息
API u16 File_Open_Read(IN const char * pFileName, IN ASFFrameProcCallBack  inCallback, IN void* pvContext, OUT TKsjHeader * ptKsjHeader, void *pReader);

//  获取ASF文件的属性信息
API u16 File_GetASFFileProPerty(OUT TASFFileProperty * pFileProperty, void *pReader);

//  获取ASF文件的属性信息
API u16 File_CopyRight(OUT BOOL32 *IsKedacomFile, void *pReader);

//  获取ASF文件所有流的个数
API u16 File_GetStreamTotal(OUT u16 *wStreamCount, void *pReader);

//得到对应字段的长度
API u16 File_GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength ,void *pReader);

//  根据索引获取ASF文件的每个流的信息,参数需设置好长度，分配好内存
API u16 File_GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty, void *pReader);

//设置快放慢放和倍数
API u16 File_SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate, void *pReader);

API u16 File_Start(IN u32 dwStartTime , void *pReader , BOOL32 bIsAutoCallBack, BOOL32 bDragToKeyPoint);

//单帧获取函数，没调用一次
API u16 File_GetNextFrame( u32 *pdwStatus, TStreamDataPayload **ppoFrameData, void *pReader);

//获取视频最后一帧关键帧时间戳
API u16 File_GetLastKeyFrameTime(OUT s64 *poLastKeyFrameTime, void *pReader);

//获取视频最后一帧时间戳
API u16 File_GetLastVideoFrameTime(OUT s64 *poLastVideoFrameTime, void *pReader);

//  暂停异步读取ASF文件的帧数据
API u16 File_Pause(void *pReader);

//  由暂停的位置重新异步读取ASF文件的帧数据,或由单帧回调模式返回到自动回调模式
API u16 File_Resume(void *pReader);

//  停止异步读取ASF文件的帧数据
API u16 File_Stop(void *pReader);

//  关闭ASF文件读取，并释放资源
API u16 File_Close_Read(void *pReader);

API u16 File_Release_Read(void *pReader);


#if defined(__Win32__) || defined(WIN32)
#pragma pack(pop)
#endif	//WIN32


#endif //_FILE_INTERFACE_DLL_H_
