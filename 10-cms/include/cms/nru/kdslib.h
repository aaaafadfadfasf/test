/**
* @file		kdslib.h
* @brief		kds文件格式头文件
* @author	白昊
* @date		2007-04-11
* @version	1.0
* @copyright V1.0  Copyright(C) 2016 CBB All rights reserved.
*/

#if !defined(AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_)
#define AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_

#include "osp.h"

/** 错误码定义*/
#define				KDS_ERR_BASE				7000								///<错误码的起始值
#define				KDS_ERR_NOERR				(u16)(0)							///<操作成功
#define				KDS_ERR_OPENFAIL			(u16)(KDS_ERR_BASE+		1)		///<打开文件失败
#define				KDS_ERR_INIT_FAIL			(u16)(KDS_ERR_BASE+		2)		///<库初始化失败
#define				KDS_ERR_NOMEM				(u16)(KDS_ERR_BASE+		3)		///<没有内存
#define				KDS_ERR_FILE_OP_ERR		(u16)(KDS_ERR_BASE+		4)		///<文件操作失败
#define				KDS_ERR_PARAM				(u16)(KDS_ERR_BASE+		5)		///<参数错误
#define				KDS_ERR_NOT_PERMISSION_OP	(u16)(KDS_ERR_BASE+		6)		///<不允许的操作
#define				KDS_ERR_PAYLOAD_CHANGE		(u16)(KDS_ERR_BASE+		7)		///<媒体载荷类型改变
#define				KDS_ERR_NOT_KDS			(u16)(KDS_ERR_BASE+		8)		///<不是kds文件
#define				KDS_ERR_POSITION_ERR		(u16)(KDS_ERR_BASE+		9)		///<定位的时间错误
#define				KDS_ERR_FILE_REACH_END		(u16)(KDS_ERR_BASE+		10)		///<读到文件尾
#define				KDS_ERR_UNKNOWN			(u16)(KDS_ERR_BASE+		11)		///<未知错误
#define				KDS_ERR_SMALL_BUFF		(u16)(KDS_ERR_BASE+		12)		///<上层给的缓冲太小
#define				KDS_ERR_PACK_TIME_ERR		(u16)(KDS_ERR_BASE+		13)		///<数据包的时间戳错误
#define				KDS_ERR_RESUME_FAIL	    (u16)(KDS_ERR_BASE+		14)		///<恢复失败，当前未用
#define				KDS_ERR_FILE_NOT_OPEN		(u16)(KDS_ERR_BASE+		15)		///<文件未打开
#define				KDS_ERR_PLAY_STATE_ERR		(u16)(KDS_ERR_BASE+		16)		///<播放状态错误
#define				KDS_ERR_CALLBACK_NOT_SET	(u16)(KDS_ERR_BASE+		17)		///<回调函数没有设置
#define				KDS_ERR_METHOD_NOTSUPPORT  (u16)(KDS_ERR_BASE+		18)		///<方法不支持
#define				KDS_ERR_FILEVER_TOO_LOW    (u16)(KDS_ERR_BASE+       19)		///<文件版本太低
#define				KDS_ERR_TIMER_INITAL_ERR   (u16)(KDS_ERR_BASE+       20)		///<定时器初始化失败
#define				KDS_ERR_NOMORESPACE_FORINDEX  (u16)(KDS_ERR_BASE+     21)		///<索引空间不够，目前未使用

/** 文件支持的最大流数量*/
#define				MAX_KDS_STREAM_NUM			10						

/** 允许的最大包大小*/
#define				MAX_KDS_PACK_SIZE				5 * 1024

/** 媒体扩展信息最大长度*/
#define				MAXLEN_MEDIAINFO_NAME         1024

/** 允许的编码名称最大长度*/
#define				KDS_MAXLEN_ENCODINGNAME     8

/** 文件中的数据包类型定义*/
#define				KDS_PACKET_TYPE_UNKNOWN			0			///<无效的包类型
#define				KDS_PACKET_TYPE_KEDA				1			///<Keda码流数据包
#define				KDS_PACKET_TYPE_OTHERNOTRTP		100			///<非rtp数据包
#define				KDS_PACKET_TYPE_OTHERRTP			104			///<外厂商的rtp数据包

/** 数据包头大小*/
#define				KDS_PACK_UNIT_SIZE				19

/** 数据写模式定义*/
#define				KDS_WRITE_MODE_WRITE			1				///<磁盘分区为通用文件系统且使用write方式写磁盘
#define				KDS_WRITE_MODE_WRITEV			2				///<磁盘分区为通用文件系统且使用writev方式写磁盘
#define				KDS_WRITE_MODE_VBFS           3				///<磁盘分区为vbfs文件格式

/** 播放状态定义*/
enum
{
	KDS_PLAY_STATE_STOP				= 1,							///<停止状态
	KDS_PLAY_STATE_START				= 2,							///<开始状态
	KDS_PLAY_STATE_PAUSE				= 3							///<暂停状态
};

/** 回调事件定义*/
enum
{
	KDS_CALLBACK_PLAYING			= 1,								///<开始播放
	KDS_CALLBACK_FILE_END			= 2								///<播放到文件尾
};

/** 放像模式定义*/
enum EKdsPlayMode {
		KDS_NORMAL_PLAY         = 0,								///<正常播放
		KDS_FAST_PLAY           = 1,								///<快放
		KDS_SLOW_PLAY           = 2								///<慢放
} ;

/** 库初始化结构体*/
typedef struct  
{
	BOOL32	m_bTakeSem ;					///<是否加锁访问文件
} TKdsInit;

/**
 *@brief 内存释放
 *@param[in]		u32 mem  需要释放的内存地址
 *@param[in]		u32 context 上下文
 *@return		KDS_ERR_NOERR:成功 错误码:失败
 *@ref
 *@see
 *@note	
 */
typedef s16 (*KdsMemRelease)(u32 mem, u32 context);

/** 内存结构体定义*/
typedef struct 
{
	KdsMemRelease	m_pfCB;						///<内存释放函数
	u32				m_mem;					///<需要释放的内存地址
	u32				m_context;				///<上下文
} TKdsMem;

/** 文件扩展媒体信息*/
typedef struct
{
	s8      m_achManuFacture[MAXLEN_MEDIAINFO_NAME+1];		///<扩展媒体信息，目前主要用来存码流厂商信息
}TKdsExtFileInfo;

/** 视频媒体流属性信息结构体*/
typedef struct
{
	s32    m_nWidth;						///<图像宽
	s32    m_nHeight;					///<图像高
	s32    m_nFrameRate;					///<帧率
	s32    m_nBitRate;					///<比特率
	s32    m_nKeyFrameIntvl;				///<关键帧间隔，即多少帧会有一个关键帧
	s32    m_nClockRate;					///<时钟采样率，取值范围: 1000 or 90000
}TVideoStreamProperty;

/** 音频媒体流属性信息结构体*/
typedef struct
{
	s32    m_nSampleRate;                  ///<采样率
	s32    m_nClockRate;                   ///<时钟采样率，取值为 1000 or m_nSampleRate
	s32    m_nChannelsNum;                 ///<声道数
}TAudioStreamProperty;

/** 媒体流属性信息结构体*/
typedef struct  
{
	u8   m_byPayloadType;								///<载荷类型
	s8   m_szEncodingName[KDS_MAXLEN_ENCODINGNAME+1];		///<编码名称
	union
	{
		TVideoStreamProperty  m_tVideoStreamProperty;	///<视频流属性信息
		TAudioStreamProperty  m_tAudioStreamProperty;	///<音频流属性信息
	};
}TKdsStreamProperty;

/** 文件信息结构体*/
typedef struct 
{
	u8      m_byFileVer;										///<文件版本号
	u32     m_dwStartTime;									///<文件中第一个数据包时间戳,单位毫秒
	u32		m_dwRecTime ;										///<录像文件的总时长,单位秒
	u32		m_dwMaxPackSize ;									///<录像文件中最大包大小，单位字节
	u8		m_byMediaCount ;									///<录像文件中音视频流总数
	u8		m_byPackType ;									///<录像文件中包的类型,比如KDS_PACKET_TYPE_KEDA
	TKdsStreamProperty m_atStreamProperty[MAX_KDS_STREAM_NUM];    ///<流属性信息
	TKdsExtFileInfo m_tKdsExtFileInfo;							///<文件的扩展信息
	
} TKdsFileInfo;

/** 包信息结构体*/
typedef struct 
{
    u64 m_qwTimeStamp;					///<数据包的时间戳,单位毫秒
    u8	m_nStreamId;						///<数据包所属的媒体流编号
	
	u8	m_byFrameBorder;					///<帧边界标识，0bit : 1-帧边界，0-非帧边界, 1bit : 1-不是一帧的第一个包, 0-是一帧的第一个包
    u8  m_byPayload;						///<数据包的载荷类型
    u8  m_byKeyFrame;						///<数据包是否属于关键帧
    u16 m_wSequence;						///<数据包的序列号
	u16 m_wReserve ;						///<保留字段

    u8* m_pchData;						///<数据包指针
    u32 m_nDataLen;						///<数据长度

} TKdsPacketInfo;

/**
 *@brief	数据包回调函数，在使用库的自动放像功能时设置
 *@param[out]		KD_PTR pvContext  上下文
 *@param[out]		u32 dwStatus      播放状态
 *@param[out]		TKdsPacketInfo * ptPack  需要播放的数据包信息
 *@return		
 *@ref
 *@see
 *@note	
 */
typedef void (* KdsProcCallBack)(KD_PTR pvContext, u32 dwStatus, TKdsPacketInfo * ptPack);


/**
 *@brief	库的初始化函数
 *@param[in]		TKdsInit * ptParam  库的初始化参数
 *@return		KDS_ERR_NOERR:成功 错误码:失败
 *@ref
 *@see
 *@note	该接口必须在使用kdslib库的其他任何功能之前被成功调用
 */
u16 KdsInit(TKdsInit * ptParam);

/**
 *@brief	库的反初始化函数
 *@return		KDS_ERR_NOERR:成功 错误码:失败
 *@ref
 *@see
 *@note	不再需要使用kdslib库时，使用该接口释放相关资源
 */
u16 KdsRelease();

/**
 *@brief	解析文件版本信息
 *@param[in]		const u8 byVer  需要解析的文件版本号
 *@param[out]		u8& byMajor     文件的主版本号
 *@param[out]		u8& byMinor     文件的次版本号
 *@return		
 *@ref
 *@see
 *@note	不再需要使用kdslib库时，使用该接口释放相关资源
 */
void KdsParseFileVer(const u8 byVer, u8& byMajor, u8& byMinor);

class CKdsLibFile ;

/** 读写kds文件的类定义*/
class CKdsLib  
{
public:
	CKdsLib();
	virtual ~CKdsLib();

	/**
	*@brief		创建访问kds文件的资源
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口必须在调用该类中的其他接口之前被调用
	*/
	u16 Create();
	
	/**
	*@brief		打开指定的文件准备写入
	*@param[in]	s8 * pbFile  需要写入的完整文件名，即带绝对路径
	*@param[in]	u8 byPackType  数据包类型，比如KDS_PACKET_TYPE_KEDA
	*@param[in]	BOOL32 bNoBuff TRUE:数据包不缓存，直接写磁盘，
	*                            FALSE:对数据包进行缓存，缓存满时再写入磁盘
	*@param[in]	u32 dwFileStartAbsTime 文件的开始时间，绝对时间，单位秒
	*@param[in]	u8 WriteMode  写磁盘分区的模式
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口在Create()之后调用，则表明准备要写入数据，针对一个CKdsLib实例，与BeginRead不能同时调用
	*/
	u16 BeginWrite(s8 * pbFile, u8 byPackType , BOOL32 bNoBuff , u32 dwFileStartAbsTime = 0, u8 WriteMode = KDS_WRITE_MODE_WRITE);
	
	/**
	*@brief		打开指定的文件准备读取
	*@param[in]	s8 * pbFile  需要读取的完整文件名，即带绝对路径
	*@param[in]	BOOL32 bReverseRead 是否以倒放的方式打开文件
	*@param[in]	KdsProcCallBack inCallback  数据包回调函数
	*@param[in]	KD_PTR pvContext  回调函数中的上下文参数
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note	该接口在Create()之后调用，则表明准备要读取数据包，针对一个CKdsLib实例，与BeginWrite不能同时调用
	*/
	u16 BeginRead(s8 * pbFile, BOOL32 bReverseRead = FALSE, KdsProcCallBack inCallback = NULL , KD_PTR pvContext = NULL);

	/**
	*@brief		开始自动读取
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		开启自动读取之后，数据包会定时通过BeginRead接口中设置的KdsProcCallBack回调函数推送给上层业务
	*             该接口在调用了BeginRead()之后可使用              
	*/
	u16 StartAutoRead();

	/**
	*@brief		停止自动读取
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用
	*/
	u16 StopAutoRead();

	/**
	*@brief		暂停文件读取
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用
	*/
	u16 Pause();

	/**
	*@brief		恢复文件读取
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了Pause()之后使用
	*/
	u16 Resume();
	
	/**
	*@brief		获取文件信息
	*@param[out]	TKdsFileInfo * tKdsFileInfo 成功获取的文件信息
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用
	*/
	u16 GetFileInfo(TKdsFileInfo * tKdsFileInfo ) ;

	/**
	*@brief		设置文件扩展信息
	*@param[in]	TKdsExtFileInfo * tKdsExtFileInfo 需要保存到文件中的文件扩展信息
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginWrite()之后可使用
	*/
	u16 SetExtFileInfo(TKdsExtFileInfo * tKdsExtFileInfo);   //add by linlifen 2010.12.03
	
	/**
	*@brief		定位到给定时间点进行数据读取
	*@param[in]	u64 llMSec  需要定位的时间，相对时间，单位毫秒
	*@param[in]	BOOL32 bSeekToKeyFrame  是否定位到关键帧
									 TRUE：定位到<=llMSec的最接近llMSec的关键帧
									 FALSE：精确定位到给定的llMSec
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用
	*/
	u16 SetReadPos(u64 llMSec, BOOL32 bSeekToKeyFrame = TRUE);

	/**
	*@brief		设置播放速率
	*@param[in]	EKdsPlayMode mode  放像模式
	*@param[in]	u8 rate            放像速率，取值 >=1
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用
	*/
	u16 SetPlayRate(EKdsPlayMode mode , u8 rate);

	/**
	*@brief		添加媒体流属性
	*@param[in]	u8 byStreamId      媒体流编号
	*@param[in]	TKdsStreamProperty& tStreamProperty  媒体流属性
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginWrite()之后可使用
	*/
	u16 AddStream(u8 byStreamId, TKdsStreamProperty& tStreamProperty);
	
	/**
	*@brief		写数据包
	*@param[in]	TKdsPacketInfo * ptPack  需要写入的数据包信息
	*@param[in]	BOOL32 bFlush   TRUE:将数据刷新到磁盘上，FALSE:等到内部缓冲满后自动刷新
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginWrite()之后可使用
	*/
	u16 WritePacket(TKdsPacketInfo * ptPack, BOOL32 bFlush = FALSE , TKdsMem * ptMem = NULL );
	
	/**
	*@brief		读取数据包
	*@param[out]	TKdsPacketInfo * ptPack  成功读取的数据包信息，其中的m_pchData的内存由上层业务提供
	*                                     m_nDataLen参数在成功读取数据包后，会被修改为实际的数据包长度
	*@param[in]	BOOL32 bOnlyKeyFrame  TRUE：只读取关键帧的数据包，FALSE：读取所有的数据包
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		该接口在调用了BeginRead()之后可使用，但是如果已经调用了StartAutoRead()，则不应该再使用该接口
	*/
	u16 ReadPacket(TKdsPacketInfo * ptPack, BOOL32 bOnlyKeyFrame = FALSE);
	
	/**
	*@brief		结束文件的写入
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		
	*/
	u16 EndWrite();

	/**
	*@brief		释放访问文件的相关资源
	*@return		KDS_ERR_NOERR:成功 错误码:失败
	*@ref
	*@see
	*@note		
	*/
	u16 Close();
private:

	CKdsLibFile * m_pcKdsFile ;
};

#endif // !defined(AFX_KDSLIB_H__5EE31847_9BA7_4BAA_88ED_14120BB979B9__INCLUDED_)

