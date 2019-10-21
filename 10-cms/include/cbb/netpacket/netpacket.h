/*****************************************************************************
模块名      : netpacket
文件名      : netpacket.h
相关文件    : netpacket.c
文件实现功能: netpacket模块接口声明
作者        : 顾肖康 
版本        : V1.0  Copyright(C) 2003-2005 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2014/03/01  1.0         顾肖康       Create
******************************************************************************/
#ifndef _NETPACKET_
#define _NETPACKET_

#ifdef _USE_OSPSMALL_
#include "osp_small.h"
#else
#include "osp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** 错误码 */
#define  NETPACKET_NO_ERROR                            (s32)(0)					///< 成功返回值 
#define  NETPACKET_ERROR_BASE                          (s32)(16000)				///< 错误码起始参考值
#define  NETPACKET_ERROR_PARAM                         (NETPACKET_ERROR_BASE+1)	///< 参数错误
#define  NETPACKET_ERROR_NOCREATE                      (NETPACKET_ERROR_BASE+2)	///< 未创建
#define  NETPACKET_ERROR_MAXCREATE                     (NETPACKET_ERROR_BASE+3)	///< 达到最大创建数目
#define  NETPACKET_ERROR_MEM                           (NETPACKET_ERROR_BASE+4)	///< 内存错误
#define  NETPACKET_ERROR_NOKEYFRAME                    (NETPACKET_ERROR_BASE+5)	///< 没有关键帧
#define  NETPACKET_ERROR_UNSUPPORT                     (NETPACKET_ERROR_BASE+6)	///< 不支持
#define  NETPACKET_ERROR_AGAIN                         (NETPACKET_ERROR_BASE+7)	///< 需要重新获取该包（该错误码并非表示出错，表示读得过快）
#define  NETPACKET_ERROR_READSLOW                      (NETPACKET_ERROR_BASE+8)	///< 需要重新定位关键帧（表示读得过慢）
#define  NETPACKET_ERROR_UNKNOW                        (NETPACKET_ERROR_BASE+9)	///< 未知错误
#define  NETPACKET_ERROR_SEMCREATE                     (NETPACKET_ERROR_BASE+10)///<锁创建失败
#define  NETPACKET_ERROR_BUF_FULL                      (NETPACKET_ERROR_BASE+11)///< 分片BUF已满

#define IPC_MAX_RTPPACK_LENGTH     (1384)	///< 一个rtp包的数据最大长度（由于RTP扩展头中加入8字节的NTP时间戳，切包长度相应减8，由1392改为1384）

typedef s32 (*PFMESSAGEPROC)(u32 dwId, u32 dwFrameID, void* pContext);

//帧结构体
typedef struct tagNewFrameHdr
{
	u8     m_byMediaType; /*媒体类型*/
	u8    *m_pData;       /*数据缓冲*/
	u32    m_dwPreBufSize;/*m_pData缓冲前预留了多少空间，用于加*/
	/* RTP option的时候偏移指针一般为12+4+12*/
	/* (FIXED HEADER + Extence option + Extence bit)*/
	u32    m_dwDataSize;  /*m_pData指向的实际缓冲大小缓冲大小*/
	u8     m_byFrameRate; /*发送帧率,用于接收端*/
	u8      m_byPadSize;/*填充数据长度*/
	u32    m_dwFrameID;   /*帧标识，用于接收端*/
	u32    m_dwTimeStamp; /*时间戳, 用于接收端*/
	u32    m_dwSSRC;      /*同步源, 用于接收端*/
	u64    m_llNTPTime;   /*NTP时间戳*/
	union
	{
		struct
		{
			BOOL32    m_bKeyFrame;    /*频帧类型（I or P）*/
			u16       m_wVideoWidth;  /*视频帧宽*/
			u16       m_wVideoHeight; /*视频帧宽*/
		}m_tVideoParam;
		struct
		{
			u32       m_dwSample;/*音频采样率*/
			u16       m_wChannel;/*声道数*/
			u16       m_wBitsPerSample;/*位宽*/
		}m_tAudioParam;
	}x;
	u8   m_byStreamID;//PES包里面的音频 0-音频流0 1-音频流1
}NEWFRAMEHDR,*PNEWFRAMEHDR;

typedef struct tagPackInfo
{
	u32    m_dwTS;				//netpacket
	u32    m_dwSSRC;    		
	u16    m_wSn;       		//netpacket
	u8     m_byMediaType;		//netpacket
	u8     m_byPrever;
	u8*    m_pbyBuffer;			//netpacket
	u16    m_wRtpPackSize;		//netpacket
	BOOL32 m_bFirstPackOfFrame;
	BOOL32 m_bLastPackOfFrame;
	u16    m_wPackNum;           //netpacket//此包所在的帧对应的RTP包的总个数；
	u64    m_llTimeStamps;       //netpacket//UTC时间，RTP包到NVR后，接收模块获取到的系统时间；单位MS
	union
	{
		struct
		{
			BOOL32    m_bKeyFrame;    /*频帧类型（I or P）*/
			u16       m_wVideoWidth;  /*视频帧宽*/
			u16       m_wVideoHeight; /*视频帧宽*/
		}m_tVideoParam;          //netpacket
		struct
		{
			u32       m_dwSample;           /*音频采样率*/
			u16       m_wChannel;           /*声道数*/
			u16       m_wBitsPerSample;     /*位宽*/
		}m_tAudioParam;
	}x;
}TPackInfo;

typedef struct tagNewPackInfo
{
	TPackInfo m_ptPackInfo;
	//netpacket
	BOOL32 m_bUsed;           
	BOOL32 m_bMark;           
	u8    m_byAudioMode;      /*音频模式*/ 
	u8     m_byFrameRate;     
	s32    m_nNextKeyFramePos;
	u32    m_dwFrameID;       
	u32    m_dwUnStandardTS;  
	u64    m_llTimeStamps;  
	s32    m_nRefCount;		  ///< 引用计数
	void*  m_hPackSem;		      ///< 锁
	void   (*NPFreePack)   (struct tagNewPackInfo*); ///< 释放内存
	void   (*NPPackAddRef) (struct tagNewPackInfo*); ///< 增加引用计数
	void*  m_pNewPackInfoAddr;

}TNewPackInfo;

typedef enum ENetPacketPSAudioMode
{
	NoAudio = 0x00,
	FirstAudio = 0x01,
	SecondAudio = 0x02,
	TwoAudio = 0x03,
}ENetPacketPSAudioMode;

//如打包方式为国标PS流必填
//MEDIA_TYPE_NULL表示无视频或者无音频
typedef struct tagNPPSInfo 
{
	u8   m_byVideoType;//视频payload
	u8   m_abyAudioType[2];//音频payload
}TNPPSInfo;
	  
typedef struct tagFrameExInfo
{
	u8*    m_pbyExBuf;//默认为NULL，用于额外信息， 例如智能信息，数字水印等，以减少拷贝
	u32    m_dwExBufLen;//默认为0，
	u8     m_byStreamId;//流id，默认为0，第2路音频的话id为1
	BOOL32 m_bSingleNalu;//是否为单nalu（默认为TRUE,单nalu,H264切包时P帧不去遍历查找0001，可降低CPU消耗,多nalu情况下bSingleNalu为TRUE请注意对通是否有问题）
}TFrameExInfo;

/*=============================================================================
	函数名		：NetPacketInit
	功能		：初始化
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketInit();

/*=============================================================================
	函数名		：NetPacketUnInit
	功能		：反初始化
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketUnInit();

/*=============================================================================
	函数名		：NetPacketCreate
	功能		：创建缓冲
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwBufferSize    分配缓冲大小，单位（字节）
				                       实际内部分配缓冲会大于dwBufferSize，内部会根据dwBufferSize向上对齐，按照切包大小分配2的指数幂个缓冲
				   TPSInfo* ptPSInfo   NULL，表示打包方式为ES，不为NULL，表示打包方式为PS
				   u32* pdwId          返回Id

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketCreate(u32 dwBufferSize, TNPPSInfo* ptPSInfo, u32* pdwId);


/**  
* @brief 		设置缓冲模式 
* @detail 		
* @param[in]    u32 dwId,    Id标识
* @param[in]    BOOL32 bCoverMode 缓冲模式 TRUE:正常切包缓冲，不做监测 
                                           FALSE:缓冲切包前监测剩余缓冲数量是否足够，缓冲不足时NetPacketInputFrame将返回错误
* @retval  		参见错误码定义
*/
s32 NetPacketSetMode(u32 dwId, BOOL32 bCoverMode);

/**  
* @brief 		设置切包时预留的扩展头字段个数 
* @detail 		
* @param[in]    u32 dwId,    Id标识
* @param[in]    u32 dwResExtHdrNum 切包时预留的扩展头字段个数
* @retval  		参见错误码定义
*/
s32 NetPacketSetResExtHdrNum(u32 dwId, u32 dwResExtHdrNum);

/**  
* @brief 		输入一帧数据 
* @detail 		本接口为媒体库调用，传入编码后的一帧数据，payload，宽、高、是否关键帧、时间戳、帧率等参数务必填写
* @param[in]    u32 dwId        Id标识
* @param[in]    PFRAMEHDR pFrame   帧数据
* @param[in]    TFrameExInfo*  pFrameExInfo  帧额外信息                                        
* @retval  		参见错误码定义
*/
s32 NetPacketInputFrame(u32 dwId, PNEWFRAMEHDR pFrame, TFrameExInfo*  pFrameExInfo);

/*=============================================================================
	函数名		NetPacketGetFirstKeyFramePos
	功能		：获取缓冲中最早的关键帧的包序号
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId           Id标识
				   u16* pwSeq         返回包序号				   
                   该接口为录像库调用，用于获取缓冲中最老的关键帧第一包的包序号，由录像模块调用
	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetFirstKeyFramePos(u32 dwId, u16* pwSeq);

/*=============================================================================
	函数名		NetPacketGetLastKeyFramePos
	功能		：获取缓冲中最新的关键帧的包序号
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId           Id标识
				   u16* pwSeq         返回包序号				   
                   该接口为网传库调用，用于获取缓冲中最新的关键帧第一包的包序号，由网络发送模块调用
	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetLastKeyFramePos(u32 dwId, u16* pwSeq);

/*=============================================================================
	函数名		NetPacketGetLastPackPosEx
	功能		：获取缓冲中最新的包序号
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId           Id标识
				   u16* pwSeq         返回包序号				   

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetLastPackPosEx(u32 dwId, u16* pwSeq);

/*=============================================================================
	函数名		NetPacketGetPacket
	功能		：获取一包数据
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId              Id标识
				   u16 wSeq              包序号
				   TNewPackInfo* ptPackInfo 返回包信息

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetPacket(u32 dwId, u16 wSeq, TNewPackInfo** pptPackInfo, u32* pdwFrameID);

/*=============================================================================
	函数名		NetPacketGetPacket
	功能		：获取一包数据
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId              Id标识
				   u16 wSeq              包序号
				   TNewPackInfo* ptPackInfo 返回包信息

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetPacketExt(u32 dwId, u16 wSeq, TNewPackInfo** ptPackInfo, u32* pdwFrameID);

/*=============================================================================
	函数名		NetPacketRelease
	功能		：释放资源
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId      Id标识				   

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketRelease(u32 dwId);

			   

/**
* @brief 		获取切包缓存状态，否是所有包都被发送取走 
* @detail 		本接口网传库调用
* @param[in]    u32 dwId    Id标识
* @param[out]   u8* byProgress    发送进度百分比0--缓存满， 100--缓存已经全部发出
* @retval  		是否发送完成 TRUE ：完成， FALSE：尚未发送完成。
*/
u32 NetPacketCheckSendProgress(u32 dwId, u8* pbyProgress);

/*=============================================================================
	函数名		NetPacketSetPackLength
	功能		：设置切包长度
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u16 wPackLength      切包长度（不可超过IPC_MAX_RTPPACK_LENGTH-1384）	

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetPackLength(u16 wPackLength);

/*=============================================================================
	函数名		NetPacketSetNTPExtence
	功能		：设置是否添加NTP时间戳到RTP扩展头中（目前仅在H264码流中添加，由于加了8个字节的扩展头，切包长度由1392改为1384）
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId                Id标识	
	               BOOL32 bAddNTPExtence   是否添加				   

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetNTPExtence(u32 dwId, BOOL32 bAddNTPExtence);

/*=============================================================================
	函数名		NetPacketSetCallback
	功能		：设置一帧打包结束的消息回调
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId                Id标识	
	               PFMESSAGEPROC pMessageCallback   回调
				   void* pvContext         回调上下文

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetCallback(u32 dwId, PFMESSAGEPROC pMessageCallback, void* pvContext);

/*=============================================================================
	函数名		NetPacketSetPSCut
	功能		：设置PS打包是否开启
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId                Id标识	
	               BOOL32 bStart           是否开启

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetPSCut(u32 dwId, BOOL32 bStart);

/*=============================================================================
	函数名		NetPacketGetAudioSampleAndChannelInfo
	功能		：获取音频采样率以及声道数
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u8 byPayload	
	               u8 byAudioMode  
				   u32 *pdwSample  返回采样率
				   u8* pbyChannel  返回声道数

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetAudioSampleAndChannelInfo(u8 byPayload, u8 byAudioMode, u32 *pdwSample, u8* pbyChannel);

/*=============================================================================
	函数名		NetPacketSetNoExtence
	功能		：设置不带扩展头
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               BOOL32 bNoExtence 是否带扩展头(bNoExtence为TRUE表示不带扩展头)

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetNoExtence(BOOL32 bNoExtence);

/*=============================================================================
	函数名		NetPacketSetPSAudioMode
	功能		：设置PS流打包音频模式，默认只打包第1路音频
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
				   u32 dwId                Id标识
	               ENetPacketPSAudioMode ePSAudioMode 打包音频模式

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketSetPSAudioMode(u32 dwId, ENetPacketPSAudioMode ePSAudioMode);


/*=============================================================================
	函数名		NetPacketGetPacketData
	功能		：获取一包数据
	算法实现	：（可选项）
	引用全局变量：无
	输入参数说明：
	               u32 dwId              Id标识
				   u16 wSeq              包序号
				   TNewPackInfo* ptPackInfo 返回包信息

	返回值说明：参见错误码定义
=============================================================================*/
s32 NetPacketGetPacketData(u32 dwId, u16 wSeq, TNewPackInfo** pptPackInfo, u32* pdwFrameID, u8* pbySendBuf, u16* bySendBufSize, u8 byTransHead);

#ifdef __cplusplus
}
#endif

#endif //_NETPACKET_
