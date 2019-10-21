/************************************************************
//  文 件 名：  asflib.h
//  相关文件：  asflib.cpp
//	作    者：	赵红显
//	创建时间：	2005-08-29
//	修改时间：	
************************************************************/

#ifndef __ASF_LIB__
#define __ASF_LIB__


/************************************************************
*************************************************************

//定义ASF文件读写接口的数据结构

*************************************************************
*************************************************************/
#include "osp.h"

#ifdef _SUPPORT_KCS_CLOUD_
#include "kdmfilecloud.h"
#else
#include "kdmfileinterface.h"
#endif

#if defined(__Win32__) || defined(WIN32)
#pragma pack(push, 1)				//	用一个字节的结构对齐，防止不同的编译器导致错误
#endif	//WIN32



#define MAX_ASF_READER					400		//最多可播放路数
#define MAX_ASF_PATH_LENTGTH			128		//asf文件路径最大长度,调试用,上层不用考虑
// 流序号有效性的检查
#define CHECKSTREAMNUMBER(a)	(((a) >= 1 && (a) <= 63) ? TRUE : FALSE) //支持最大流数128音视频除2

#define COPY_DWORD(achByte, dwInt)          do{*(achByte) =   (u8)(((u32)dwInt)& 0xff) ; \
                                               *(achByte+1) = (u8)(((u32)dwInt >> 8)& 0xff) ; \
                                               *(achByte+2) = (u8)(((u32)dwInt >> 16)& 0xff); \
                                               *(achByte+3) = (u8)(((u32)dwInt >> 24)& 0xff); \
                                               } while(0);

#define COPY_WORD(achByte, wInt)             do{*(achByte) =   (u8)(((u16)wInt)& 0xff) ; \
                                                *(achByte+1) = (u8)(((u16)wInt >> 8)& 0xff); \
                                               } while(0);

typedef void* HTimer;

    
// 流对象的属性链表结构
struct TStreamListEntry {	
	TStreamProperty		tStreamProperty;	// 流对象的属性
	TStreamListEntry	*pNextStream;		// 下一个流对象的指针
	u8 					byStreamID;			// 流对象的顺序号
	u8					byAlignedField;		//对齐字节,无意义
	u16					wAlignedField;		//对齐字节,无意义
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
;


//  标志数据
typedef struct tagMarkers{
	u64		qwOffset;						// 标志在文件中的位置偏移
	u64		qwPresentationTime;				// 标志显示的时间
	u32		dwMarkerDescriptionLength;		// 标志的描述长度
	u8		*pbyMarkerDescription;			// 标志的描述信息
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
TMarkerStruct;



//  命令脚本数据
typedef struct tagCommands{
	u32		dwPresentationTime;				// 脚本执行的时间
	u16		wCommandsTypeLength;			// 脚本的类型长度
	u16		*pwCommandsType;				// 脚本的类型
	u16		wCommandNameLength;				// 脚本的名称长度
	u16		*pwCommandName;				// 脚本的名称
} 
#if defined(_VXWORKS_) || defined(_LINUX_)
__attribute__ ( (packed) ) 
#endif
TCommandStruct;


//  纠错数据类型 
enum {
		No_Error_Correction_Type 		= 0,
		Audio_Spread_Type 				= 1,
		Unknown_Error_Type 				= 2
};
typedef u16 ERROR_CORRECTION_TYPE;


// 文件对象的ID，供设置调试信息和对象标识
// 标识码由3位数字组成
// 1 × ×  与文件头信息相关的头部对象
// 2 × ×  与数据对象相关的对象 
// 3 × ×  与索引信息相关的对象
// 4 × ×  综合性的对象
// 9 × ×  无效的对象
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

定义ASF文件的写接口类

**********************************************************
**********************************************************/


class CKdvASFFileWriter : public IKdmFileWriter
{
public:
	//
	// Constructors and destructor.
	CKdvASFFileWriter(void);
	~CKdvASFFileWriter(void);

	//  写ASF文件的初始化操作
		virtual u16	Open(IN const char * pFileName, TKsjHeader * ptKsjHeader = NULL, u32 dwCreateTime = 0) ;

	//  设置ASF文件的属性信息
	u16	SetASFFileProperty(IN TASFFileProperty * pFileProperty);
	
	//  设置ASF文件的内容版权信息
	u16	SetContentProperty(IN TContentDescriptionProperty * pContentDescriptionProperty);

	//设置是否生成索引对象
	u16  SetGenerateSimpleIndexObject(IN BOOL32 isGenarate = false);
		
	//  将设置好的流添加到文件头中
	u16  AddStream(IN TStreamProperty * pStreamProperty);

	//  开始写ASF文件的数据部分
	u16  BeginWriting(void);

	//  按照不同的流序号写帧数据
	u16  WriteSample(IN u8 byStreamNumber, IN u8 * pDataBuffer, IN u32 dwBufferSize,
					 IN BOOL32 bIsKeyFrame, IN u32 dwTimeStamp, IN u16 wDuration, OUT u32* pdwFileSize = NULL);
	
	//  结束写帧数据
	u16  EndWriting(void);
	
	//  关闭文件，释放资源空间
	u16  Close(void);


	//  设置是否需要在屏幕输出调试信息
	u16  SetDebugInfo(IN u16 wObjectID);
	//  根据函数的返回值获取详细的错误信息
	LPSTR GetErrorInfo(IN u16 wErrorIndex);
	//  专用于显示二进制数据、将每个字节的值用两个十六进制的数字显示出来
	void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength);	
	u16 PrintAllStep();
	
protected:

	SEMHANDLE			m_hSem;

	BOOL32				m_bSemCreate;
	BOOL32				m_bOpen;

	CKdvASFFile			*m_pASFFile;				//  ASF文件的类变量指针
	
	u16					m_wObjectIDForDebug;		//	表示要调试的目标对象ID
	
};



/********************************************************
*********************************************************

文件定义ASF文件的读接口类

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
	//  读ASF/MP4文件的初始化操作，同时兼容读取云存储文件或者本地文件
	//  云文件路径示例：cloud://con_cate/con_name/blob_cate/blob_name
	//  读云文件的话，需传入云服务器相关配置参数，否则读取失败
	u16	CloudOpen(IN TKdmFile_CloudConnectParam* ptCloudConnectParam, IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN void* pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);

	//  读ASF文件的初始化操作，完成判断是否为ASF文件，并读取ASF文件的头信息
	u16	Open(IN LPCSTR pFileName, IN ASFFrameProcCallBack inCallback = NULL , IN KD_PTR pvContext = NULL, OUT TKsjHeader * ptKsjHeader = NULL);

	//  设置处理帧数据的回调函数
	u16	SetCallback(IN KD_PTR pvUser, IN ASFFrameProcCallBack inCallback);

	//  获取ASF文件所有头对象的个数
	u16	GetHeaderObjectTotal(OUT u32 * dwHeadObjectCount);
	
	//  获取ASF文件的属性信息
	u16	GetASFFileProPerty(OUT TASFFileProperty * pFileProperty);

	//05.7.26得到对应字段的长度
	u16	GetContentDescriptionPropertyFieldLength(OUT u16 *poTitleLength,OUT u16 *poAuthorLength, OUT u16 *poCopyrightLength,OUT u16 *poDescriptionLength);
	
	//  获取ASF文件的内容版权信息,参数需设置好长度，分配好内存
	u16	GetContentDescriptionProperty(IN OUT TContentDescriptionProperty * pioContentDescriptionProperty);

	//  获取ASF文件所有流的个数
	u16	GetStreamTotal(OUT u16 *wStreamCount);
	
	//得到对应字段的长度
	u16 GetStreamPropertyRelativeFieldLenth(IN u8 byinStreamIndex, OUT u16 *poStreamType, OUT u32 *poErrorCorrectDataLength, OUT u16 *poAudVidOthCodecSpecificLength);

	//  根据索引获取ASF文件的每个流的信息,参数需设置好长度，分配好内存
	u16	GetStreamPropertyByIndex(IN u8 byStreamIndex,IN OUT TStreamProperty * pioStreamProperty);
	
	//  获取ASF文件的纠错类型
	u16	GetErrorCorrectionType(OUT u16 *pErrorType);
	//  获取ASF文件的纠错数据长度
	u16	GetErrorCorrectionDataLength(OUT u32 *pErrCorrDataLength);
	//  获取ASF文件的纠错数据
	u16	GetErrorCorrectionData(OUT u8 *pErrCorrData, IN u32 dwDataLength);	
	
	//  设置是否播放音频流
	u16	SetPlayAudioStream(IN BOOL32 isPlayFirstStream = true, BOOL32 isPlaySecondStream = true);
	//  设置是否播放视频流（所有流中至少要播放一个流，设置错误系统有策略强制播放一个流）
	u16	SetPlayVideoStream(IN BOOL32 isPlayFirstStream = true, IN BOOL32 isPlaySecondStream = true );//05.7.26

	//设置快放慢放和倍数
    u16 SetPlayRate(IN u16 wPlayMode, IN u32 wPlayRate);

	//根据编解码器ID，得出相应的媒体类型
	u16	CodecFormatId2MediaType(IN u8 byiStreamType,IN u32 dwiCodecFormatID,OUT UnionMediaDetailTypeInfo& uoMediaDetailTypeInfo);

	//  根据输入的起始时间，异步读取ASF文件的帧数据
	//bIsAutoCallBack，是否自动回调，自动回调和单帧回调可切换
	//不管是自动回调还是单帧回调，都必须设置回调函数，必须调用此函数。
	u16	Start(IN u32 dwStartTime = 0, BOOL32 bIsAutoCallBack = true, BOOL32 bDragToKeyPoint = TRUE);
	
	//单帧获取函数，没调用一次
	u16	GetNextFrame( u32& dwStatus, TStreamDataPayload **ppoFrameData);
	//获得最后一帧视频帧对应帧时间戳
	u16 GetLastVideoFrameTime(OUT s64 *poLastKeyFrameTime);
	//  暂停异步读取ASF文件的帧数据
	u16	Pause(void);
	//  由暂停的位置重新异步读取ASF文件的帧数据,或由单帧回调模式返回到自动回调模式
	u16	Resume(void);
	//  停止异步读取ASF文件的帧数据
	u16	Stop(void);
	//  关闭ASF文件读取，并释放资源
	u16	Close(void);	


	//  设置是否需要在屏幕输出调试信息
	u16		SetDebugInfo(IN u16 wObjectID);
	//  输出整个ASF文件的内容对象索引表信息（Table Of Content）
	u16		DumpObjectTOC(void);
	//  根据函数的返回值获取详细的错误信息
	LPSTR GetErrorInfo(IN u16 wErrorIndex);
	//  专用于显示二进制数据、将每个字节的值用两个十六进制的数字显示出来
	void	ShowDataWithHex(IN u8 * pData, IN u32 dwDataLength);	
	
#ifdef _SUPPORT_KCS_CLOUD_
public:
	cdb::ReadOnlyStreamBlob * m_pcReadBlob;
#endif

private:
	CKdvASFFile*	m_pASFFile;				//  ASF文件的类变量指针

	u16				m_wObjectIDForDebug;		//	表示要调试的目标对象ID

	SEMHANDLE			m_hSem;
	
	BOOL32				m_bSemCreate;
};


/***************************************************************
****************************************************************

全局函数定义

****************************************************************
****************************************************************/

#ifndef API
#define API extern "C"
#endif


/*==============================================================
 AsfStartUp():	定时器线程创建函数,与AsfCleanUp（）对称调用，在调用读接口之前调用
				如果只用到写接口，这两个函数可以不调用。在调用读接口之前需要调用AsfStartUp(),
				调用AsfStartUp（）之后可以new多个asfreader对象或实例，	
				由于里面定时器的关系,目前在一个进程里面最多支持new 32个asfreader对象,
				asfwriter不受限制。	Delete所有 asfreader之后调用AsfCleanUp()。
 ===============================================================*/
void AsfStartUp();

/*===============================================================
AsfCleanUp(): Asflib的定时器线程退出函数,与AsfStartUp（）对称调用
================================================================*/
void AsfCleanUp();

/*================================================================
AsfWriteCleanUp:在只用到asfwriter的情况下,如果没有调用asfstartup和asfcleanup,
则在退出时要调用这个函数释放内存,否则有内存泄漏.
如果调用了asfstartup和asfcleanup,则不用调用这个函数
=================================================================*/
void AsfWriteCleanUp();

/*================================================================
 asflibversion(): asflib版本信息
=================================================================*/
API void asflibversion();

/*================================================================
 asflibhelp(): asflib帮助函数
=================================================================*/
API void asflibhelp();

/*================================================================
 asffileinfo()：接口打开的文件索引号和文件名信息
=================================================================*/
API void asffileinfo();

/*================================================================
 sfdebuginfo()：asflib实时信息打印
=================================================================*/
API void asfdebuginfo(s32 nIndex, s32 nLevel);

#if defined(__Win32__) || defined(WIN32)
#pragma pack(pop)
#endif	//WIN32

#endif  //__ASF_LIB__
