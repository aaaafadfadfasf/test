/**
* @file		bslib.h
* @brief		使用kdstor云存储存储文件的库
* @author	林莉芬
* @date		2016-06-14
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/

#ifndef _BLOB_STOR_H_
#define _BLOB_STOR_H_

#include <stdio.h>
#include "kdvtype.h"

/** bslib库支持的IP地址最大长度*/
#define   BS_MAXLEN_IPADDR           64

/** bslib库支持的机器编号最大长度*/
#define   BS_MAXLEN_MACHINEID        64

/** bslib库支持的域名最大长度*/
#define   BS_MAXLEN_ZONENAME         32

/** bslib库支持的用户名最大长度*/
#define   BS_MAXLEN_USERNAME         32

/** bslib库支持的密码最大长度*/
#define   BS_MAXLEN_PASSWD           16

/** bslib库支持的文件名最大长度*/
#define   BS_MAXLEN_FILENAME         255

/** bslib库支持的媒体流最大数量*/
#define   BS_MAXNUM_STREAM           3

/** bslib库支持的编码名称最大长度*/
#define	 BS_MAXLEN_ENCODINGNAME     8

/** bslib库支持的文件扩展信息的最大长度*/
#define   BS_MAXLEN_FILEEXTINFO           1024

/** bslib库的错误码定义*/
typedef enum
{
	BS_Ok = 0,							///<操作成功
	
	BS_Err_Invalid_Argument = 1000,			///<无效参数
	BS_Err_File_Not_Exist,					///<文件不存在
	BS_Err_File_Exist,                    ///<文件已经存在
	BS_Err_Mount,							///<连接云存储失败
	BS_Err_Not_Init,						///<库没有被初始化
	BS_Err_NotBs,							///<不是云存储
	BS_Err_IOErr,							///<IO错误
	BS_Err_IOTimeOut,						///<IO操作超时
	BS_Err_File_Reach_End,					///<读到文件末尾
	BS_Err_Invalid_Oper,					///<无效操作
	BS_Err_Read_File,                     ///<读文件失败
	BS_Err_Invalid_FileFormat,				///<无效的文件格式
	BS_Err_No_Mem,						///<无内存
	BS_Err_Buffer_Too_Small,				///<读取包时，缓存太小，不够存放一包数据
	BS_Err_File_Open,						///<文件打开失败
	BS_Err_Pack_TimeStamp,					///<数据包的时间戳错误
	BS_Err_Data,							///<数据包错误
	BS_Err_Not_Empty,						///<对象非空
	BS_Err_Lost_Connection,				///<与云存储断链
	BS_Err_No_Space,						///<云存储无可用空间
	BS_Err_OutOfPower,					///<机器断电运行
	BS_Err_No_Buffer,						///<云存储无缓存
	BS_Err_Bad_Index,						///<文件索引损坏
	BS_Err_File_Locked,					///<文件被锁定
	BS_Err_Method_NotSupport,				///<方法不支持

	BS_Err_Unknown,
}EBS_ErrType;

/** bslib库支持的IP地址协议类型定义*/
typedef enum
{
	BS_IpType_IPv4 = 0,				///<IP地址为IPv4协议类型
	BS_IpType_IPv6,					///<IP地址为IPv6协议类型
}EBS_IpType;

/** bslib库支持的媒体流类型定义*/
typedef enum
{
	BS_StreamType_Unknown,				///<未知的媒体流类型
	BS_StreamType_Audio,				///<音频流
	BS_StreamType_Video,				///<视频流
	BS_StreamType_Invalid,				///<无效的媒体流类型
}EBS_StreamType;

/** bslib库支持的数据包类型定义*/
typedef enum
{
	BS_PackType_Keda = 1,				///<数据包为Kedacom码流包，该类型包支持码流关键帧、宽高的分析
	BS_PackType_Other_NotRTP,			///<数据包为非RTP包格式，数据包来自外厂商编码器
	BS_PackType_Other_RTP,				///<数据包为RTP包格式，数据包来自外厂商编码器
	BS_PackType_Unknown,				///<未知的数据包类型
}EBS_PackType;

/** IP地址的定义*/
typedef struct  
{
	EBS_IpType  m_eIpType;							///<IP地址协议类型
	s8          m_szIpAddr[BS_MAXLEN_IPADDR+1];		///<IP地址字符串
}TBS_IPAddr;

/** 网络地址的定义*/
typedef struct  
{
	TBS_IPAddr   m_tIpAddr;						///<IP地址信息
	u16          m_wPort;							///<端口信息
}TBS_NetAddr;

/** 库的初始化参数结构体*/
typedef struct
{
	u16          m_wNetAddrNum;							///<需要连接的云服务器地址数量
	TBS_NetAddr  *m_ptNetAddr;								///<需要连接的云地址信息
	s8           m_szMachineID[BS_MAXLEN_MACHINEID+1];		///<云服务器的机器ID， 一般为UUID        
	s8           m_szZoneName[BS_MAXLEN_ZONENAME+1];			///<云服务器的域名
	s8           m_szUserName[BS_MAXLEN_USERNAME+1];			///<连接云服务器的用户名
	s8           m_szPasswd[BS_MAXLEN_PASSWD+1];			///<连接云服务器的密码
	u32          m_dwTimeOut;								///<连接云服务器的超时时间，超过该时间没有连接上云服务器，则会失败返回
	s32          m_nDataChunk;							///<云存储使用的Erasure code数据保护级别中的数据块数量，目前没有最大值限制，如果为0，则内部默认设置为4
	s32          m_nParityChunk;							///<云存储使用的Erasure code数据保护级别中的校验块数量，取值范围[1,2]，如果为0，则内部默认设置为1
}TBS_InitParam;

/** 日志文件配置信息*/
typedef struct
{
	u16          m_wLogNum;                               ///<允许的最大日志文件数量*/
	u32          m_dwLogSize;                             ///<单个日志文件的最大大小，单位：bytes*/
	s8           m_szLogName[BS_MAXLEN_FILENAME+1];			///<完整的日志文件名前缀，必须是绝对路径+文件名前缀
}TBS_LogCfg;

/** 文件信息结构体定义*/
typedef struct
{
	u64   m_qwStartTime;							///<文件起始时间，单位秒
	u64   m_qwEndTime;							///<文件结束时间，单位秒
	u64   m_qwFileSize;							///<文件大小，单位：字节
}TBS_Stat;

/** 云存储文件系统信息结构体定义*/
typedef struct 
{
	u32 m_dwBlockSize;							///<底层块大小，单位：字节
	u64 m_qwFreeSize;								///<剩余空间大小，单位：字节
	u64 m_qwTotalSize;							///<总空间大小，单位：字节
	EBS_ErrType m_eFsStatus;						///<系统状态是否正常,BS_Ok--正常，其他值--异常错误码
}TBS_Statfs;

/**
 *@brief BSLIB库的初始化函数
 *@param[in]	const TBS_InitParam* ptInitParam bslib库的初始化参数
 *@param[in]	TBS_LogCfg* ptLogCfg 日志文件配置
 *@return	BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note  必须在使用BsLib库的其它任何接口之前调用
 */
EBS_ErrType BS_Init(TBS_InitParam* ptInitParam, TBS_LogCfg* ptLogCfg);

/**
 *@brief BsLib库的反初始化函数
 *@return	BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note  调用该接口之后，将不能再使用BsLib库的任何其它功能
 */
EBS_ErrType BS_Unint();

/**
 *@brief 判断是否为云存储上的文件
 *@param[in]		const s8* pszFile 带绝对路径的文件名
 *@return		BS_Ok:是云存储文件 错误码:判断失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_IsBS(const s8* pszFile);

/**
 *@brief 获取云存储文件系统信息
 *@param[in]		const s8* pszDir 云存储路径
 *@param[out]		TBS_Statfs * ptStatfs 云存储文件系统信息
 *@return		BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Statfs(const s8* pszDir, TBS_Statfs * ptStatfs);

/**
 *@brief 获取云存储上的指定文件的信息
 *@param[in]		const s8* pszFileName 云存储上的文件名，带绝对路径
 *@param[out]		TBS_Stat * ptStat 成功获取的文件信息
 *@return		BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Stat(const s8* pszFileName, TBS_Stat * ptStat);

/**
 *@brief 判断云存储上是否存在相应的目录或者文件
 *@param[in]		const s8* pszDir 文件名或者路径名
 *@return		BS_Ok:文件或目录存在 错误码:失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_Access(const s8* pszDir);

/**
 *@brief 删除云存储上的指定文件名的文件或者指定目录
 *@param[in]		const s8* pszFileName  文件名或者目录
 *@return		BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note	如果需要删除的是目录，只有在目录为空时，才能删除成功
 */
EBS_ErrType BS_Remove(const s8* pszFileName);

/**
 *@brief 锁定或者解锁云存储上的文件
 *@param[in]		const s8* pszFileName  带绝对路径的文件名
 *@param[in]		BOOL32 bLock  TRUE：锁定，FALSE：解锁
 *@return		BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_LockFile(const s8* pszFileName, BOOL32 bLock);

/**
 *@brief 设置向云存储写入数据时的超时时间
 *@param[in]		const u32 dwTimeOutMs  需要设置的超时时间，单位：毫秒
 *@return		BS_Ok:成功 错误码:失败
 *@ref
 *@see
 *@note
 */
EBS_ErrType BS_SetTimeOut(const u32 dwTimeOutMs);

/**
 *@brief 获取bslib内部分配的各种对象的数量及对象占用的内存情况
 *@param[out]		u64& llTotalObjectSize   所有对象的总大小
 *@param[out]		u32& dwWriteObjectNum    所有写对象的数量
 *@param[out]		u32& dwReadObjectNum     所有读对象的数量
 *@return
 *@ref
 *@see
 *@note	
 */
void  BS_GetObjectUsedInfo(u64& llTotalObjectSize, u32& dwWriteObjectNum, u32& dwReadObjectNum);

/**
 *@brief 获取bslib内部分配的缓冲总大小及缓冲总数量
 *@param[out]		u64& llTotalBuffSize   分配的所有缓冲大小
 *@param[out]		u32& dwTotalBuffNum    分配的所有缓冲数量
 *@return
 *@ref
 *@see
 *@note	
 */
void  BS_GetBufferUsedInfo(u64& llTotalBuffSize, u32& dwTotalBuffNum);

/**
 *@brief 设置bslib内部在向云存储写入key-value,数据,attachment之后的延时时间
 *@param[in]		const u64 llKeyValueDelayTimeMs   写入key-value之后sleep的时间，单位毫秒
 *@param[in]		const u64 llDataDelayTimeMs	   写入data之后sleep的时间，单位毫秒
 *@param[in]		const u64 llAttachMentDelayTimeMs  写入attachment之后sleep的时间，单位毫秒
 *@return
 *@ref
 *@see
 *@note	该接口只用于测试时，正常业务在使用bslib库时不应该使用该接口
 */
void  BS_TestDelay(const u64 llKeyValueDelayTimeMs, const u64 llDataDelayTimeMs, const u64 llAttachMentDelayTimeMs);

/** 数据包信息定义*/
typedef struct 
{
	u64 m_qwTimeStamp;		///<数据包的时间戳，单位毫秒
	u8  m_byPayLoad;			///<数据包的媒体类型
	u8	m_byPackFlag;			///<数据包标识	第 0 bit: 0-不是帧边界，1-是帧边界
							///<				第 1 bit: 0-是一帧的第一包，1-不是一帧的第一包
							///<				第 2 至 3 bit: 0-P帧，1-I帧，2-Unknonw
							///<				第 4 至 7 bit: 流号
	u16 m_wSequence;			///<数据包的序列号
	u16 m_wReserve;			///<保留字段
	
	u32 m_dwDataLen;			///<存储数据包内容的缓冲大小，在获取到实际数据包内容后，会被修改
	u8 *m_pchData;           ///<存储数据包内容的缓冲指针，由上层业务分配
} TBS_PackInfo;

/** 文件属性信息*/
typedef struct
{
	u32     m_dwFileVer;						///<文件版本号
	u32     m_dwFileDuration;                 ///<文件时长，单位毫秒
	u32     m_dwFileFirstPackTime;				///<文件中的第一个数据包的时间戳，单位毫秒
	u32     m_dwFileTotalPackCount;			///<文件中的数据包总数
	u64		m_qwFileTotalSize;				///<文件大小，单位字节
	u32     m_dwMaxPackSize;					///<文件中最大的数据包大小
	u32     m_dwMaxFrameSize;					///<最大帧数据大小
	u16     m_wMaxFramePackCount;				///<最大数据帧包含的数据包数目
	u8		m_byStreamCount;					///<文件中媒体流的数目
	u8      m_byPackType;                     ///<文件中存储的数据包类型
}TBS_FileProperty;

/** 视频流属性信息*/
typedef struct
{
	s32    m_nWidth;							///<图像宽
	s32    m_nHeight;							///<图像高
	s32    m_nFrameRate;						///<帧率
	s32    m_nBitRate;						///<比特率
	s32    m_nKeyFrameIntvl;					///<关键帧间隔，即多少帧会出现一个关键帧
	s32    m_nClockRate;						///<视频流的时钟采样率，取值：1000 or 90000
}TBS_VideoStreamProperty;

/** 音频流属性信息*/
typedef struct
{
	s32    m_nSampleRate;						///<采样率
	s32    m_nChannelsNum;					///<声道数
	s32    m_nClockRate;						///<音频流的时钟采样率，取值：1000 or 真实的采样率
}TBS_AudioStreamProperty;

/** 媒体流属性信息*/
typedef struct
{
	EBS_StreamType   m_eStreamType;				///<媒体流类型：音频or视频
	u8   m_byPayloadType;						///<媒体流的载荷类型
	s8   m_szEncodingName[BS_MAXLEN_ENCODINGNAME+1];		///<媒体流的载荷类型对应的编码名称
	u16  m_wReserved;									///<保留字段
	union
	{
		TBS_VideoStreamProperty  m_tVideoStreamProperty;		///<视频流的媒体属性信息
		TBS_AudioStreamProperty  m_tAudioStreamProperty;		///<音频流的媒体属性信息
	};
}TBS_StreamProperty;

/** 文件扩展信息*/
typedef struct
{
	u32     m_dwFileExtInfoLen;					///<文件扩展信息的实际长度
	u8      m_byFileExtInfo[BS_MAXLEN_FILEEXTINFO];	///<文件扩展信息
}TBS_FileExtInfo;

/** 向云存储写数据时的参数*/
typedef struct
{
	u32    m_dwDataFlushFrequence;			///<数据刷新到云服务器的频率，单位:毫秒，范围:[1 -- 文件结束时间]*/
	u32    m_dwDataFlushUnitSize;			///<目前没有使用
	u32    m_dwDataChunks;				///<云存储使用的Erasure code数据保护级别中的数据块数量，目前没有最大值限制，如果为0，则内部默认设置为4							
	u32    m_dwParityChunks;				///<云存储使用的Erasure code数据保护级别中的校验块数量，取值范围[1,2]，如果为0，则内部默认设置为1
}TBS_FileWriteOptions;

class CBSReadImp;

/** 读取云存储上的文件的类定义*/
class CBSRead
{
public:
	CBSRead();
	virtual ~CBSRead();

	/**
	*@brief 根据给定的完整文件名及是否倒放的参数，打开云存储上的文件
	*@param[in]		const s8* pszFileName   需要打开进行读取的完整文件名
	*@param[in]		BOOL32 bReverseRead     TRUE：文件打开后需要由后往前读取数据包，FALSE：正常从前往后读取文件
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType Open(const s8* pszFileName, BOOL32 bReverseRead = FALSE);

	/**
	*@brief 读取文件中的数据包
	*@param[out]		TBS_PackInfo * ptPack   成功读取的数据包信息，其中的数据包长度字段会被设置为实际的数据包长度
	*@param[in]		BOOL32 bOnlyKeyFrame    TRUE：只读取属于关键帧的数据包，FALSE：读取所有帧的数据包
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用了Open之后使用
	*/
	EBS_ErrType Read(TBS_PackInfo * ptPack, BOOL32 bOnlyKeyFrame = FALSE);

	/**
	*@brief 定位到文件中的某个位置进行读取
	*@param[in]		const u64 llMSec   需要定位的时间偏移，为相对时间，单位：毫秒，
	*@param[in]		BOOL32 bSeekToKeyFrame    TRUE：定位到<=llMSec时间最近一个关键帧，FALSE：直接定位到给定的时间位置
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用了Open之后使用
	*/
	EBS_ErrType Seek(const u64 llMSec, BOOL32 bSeekToKeyFrame = TRUE);

	/**
	*@brief 获取文件的属性信息
	*@param[out]		TBS_FileProperty * ptFileProperty  成功获取到的文件属性信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用了Open之后使用
	*/
	EBS_ErrType GetFileProperty(TBS_FileProperty * ptFileProperty);

	/**
	*@brief 获取文件中的媒体流属性信息
	*@param[in]		const u8 byStreamIndex  需要获取的媒体流编号
	*@param[out]		TBS_StreamProperty * ptStreamProperty  成功获取的媒体流属性信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用了Open之后使用
	*/
	EBS_ErrType GetStreamProperty(const u8 byStreamIndex, TBS_StreamProperty * ptStreamProperty);

	/**
	*@brief 获取文件中的扩展信息
	*@param[out]		TBS_FileExtInfo * ptFileExtInfo 成功获取的文件扩展信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用了Open之后使用
	*/
	EBS_ErrType GetFileExtInfo(TBS_FileExtInfo * ptFileExtInfo);

	/**
	*@brief 关闭打开的文件
	*@return	BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口调用之后，如果要访问文件，必须重新调用open接口打开文件
	*/
	EBS_ErrType Close();

protected:
	CBSReadImp * m_pcBSRead;
};

class CBSWriteImp;

/** 向云存储写入数据的类定义*/
class CBSWrite
{
public:
	CBSWrite();
	virtual ~CBSWrite();

	/**
	*@brief 以写方式创建并打开文件等待写入数据
	*@param[in]		const s8* pszFileName   需要打开进行写入数据的完整文件名
	*@param[in]		const u8 byPackType     数据包类型,参见EBS_PackType
	*@param[in]		TBS_FileWriteOptions * ptFileWriteOptions  文件写选项
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在该类中其他接口被调用前成功调用
	*/
	EBS_ErrType Open(const s8* pszFileName, const u8 byPackType, TBS_FileWriteOptions * ptFileWriteOptions = NULL);

	/**
	*@brief 向文件中写入数据包
	*@param[in]		TBS_PackInfo * ptPackInfo  需要写入的数据包的信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType Write(TBS_PackInfo * ptPackInfo);

	/**
	*@brief 向文件中添加媒体流属性信息
	*@param[in]		const u8 byStreamIndex  媒体流编号，从0开始
	*@param[in]		TBS_StreamProperty * ptStreamProperty 媒体流属性信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType AddStream(const u8 byStreamIndex, TBS_StreamProperty * ptStreamProperty);

	/**
	*@brief 设置文件扩展信息
	*@param[in]		const TBS_FileExtInfo * ptFileExtInfo 文件扩展信息
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note
	*/
	EBS_ErrType SetFileExtInfo(const TBS_FileExtInfo * ptFileExtInfo);

	/**
	*@brief 关闭文件
	*@return			BS_Ok:成功 错误码:失败
	*@ref
	*@see
	*@note	使用该接口关闭文件后，将不能对文件再做任何修改
	*/
	EBS_ErrType Close();

protected:
	CBSWriteImp  * m_pcBSWrite;
};

#endif