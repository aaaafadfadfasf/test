/**
 * @file Mediapack.h  
 * @brief    收发UDP包，支持KEDACOM的RTP//RTCP解析
 * @author    白昊
 * @date 2016-6-16
 * @version  1.0 
 * @copyright V1.0  Copyright(C) 2016 Keda All rights reserved.												  
 */
#if !defined(AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_)
#define AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_

#include "osp.h"

namespace MPNameSpace {

#define		MP_ERR_BASE						7100
/** OK*/
#define		MP_ERROK						(u16)(0)
/** 参数错误*/
#define		MP_ERR_PARAM					(u16)(MP_ERR_BASE+ 1)
/** 没有memory*/
#define		MP_ERR_NOMEM					(u16)(MP_ERR_BASE+ 2)
/** OspSem创建失败*/
#define		MP_ERR_SEM_CREATE				(u16)(MP_ERR_BASE+ 3)
/** SOCK创建失败*/
#define		MP_ERR_SOCKET_CREATE			(u16)(MP_ERR_BASE+ 4)
/** SOCCK bind 失败*/
#define		MP_ERR_SOCKET_BIND				(u16)(MP_ERR_BASE+ 5)
/** UNKNOWN 类型*/
#define		MP_ERR_UNKNOWN					(u16)(MP_ERR_BASE+ 6)

#define		MP_ERR_CMD_MAGIC_WRONE			(u16)(MP_ERR_BASE+ 7)
/** watch socket recv cmd wrong*/
#define		MP_ERR_CMD_CMD_WRONE			(u16)(MP_ERR_BASE+ 8)
/** WATCH套接字接收失败*/
#define		MP_ERR_WATCH_SOCKET				(u16)(MP_ERR_BASE+ 9)

/**  库释放失败*/
#define		MP_ERR_RELEASE_FAIL				(u16)(MP_ERR_BASE+ 10)
/**  库初始化失败*/
#define		MP_ERR_INIT_FAIL				(u16)(MP_ERR_BASE+ 11)
/**  sendto fail返回字节不对*/
#define		MP_ERR_SOCKET_SEND_FAIL			(u16)(MP_ERR_BASE+ 12)
/**  对象没有创建*/
#define		MP_ERR_OBJECT_NOT_NEW			(u16)(MP_ERR_BASE+ 13)
/**  free 错误*/
#define		MP_ERR_MEM_RELEASE_FAIL			(u16)(MP_ERR_BASE+ 14)
#define		MP_ERR_PACKET_TYPE_UNKNOWN		(u16)(MP_ERR_BASE+ 15)
/**  SN不连续 ，需要等待*/
#define		MP_ERR_TAKE_PACKET_WAIT			(u16)(MP_ERR_BASE+ 16)
#define		MP_ERR_PACKET_SN_ERR			(u16)(MP_ERR_BASE+ 17)
#define		MP_ERR_SOCKET_RECV				(u16)(MP_ERR_BASE+ 18)
/**  rtcp 报文分析失败*/
#define		MP_ERR_RTCP_PACKET				(u16)(MP_ERR_BASE+ 19)	
/**  send之前没bind*/
#define		MP_ERR_SOCKET_NOT_BIND			(u16)(MP_ERR_BASE+ 20)	
/**  此种RTCP重传请求不支持*/
#define		MP_ERR_RTCP_REQ_NOT_SUPPORT		(u16)(MP_ERR_BASE+ 21)	
/**  没有找到此PACKET*/
#define		MP_ERR_RESEND_PACKET_NOT_FOUND	(u16)(MP_ERR_BASE+ 22)
/**  epoll创建失败*/
#define		MP_ERR_EPOLL_FAIL				(u16)(MP_ERR_BASE+ 23)
/**  setsockopt fail*/
#define		MP_ERR_SOCKET_SET_OPT			(u16)(MP_ERR_BASE+ 24)	
/**  NOT SUPPORT type*/
#define		MP_ERR_NOT_SUPPORT				(u16)(MP_ERR_BASE+ 25)	

/**  实例未执行Create 便进行其他动作*/
#define		MP_ERR_NOT_CREATE				(u16)(MP_ERR_BASE+ 26)
/**  NODE ALREADY ADD	*/
#define		MP_ERR_NODE_ALREADY_ADD			(u16)(MP_ERR_BASE+ 27)

#define		MP_ERR_OUT_OF_NODE				(u16)(MP_ERR_BASE+ 28)

#define		MP_ERR_NODE_NOT_IN				(u16)(MP_ERR_BASE+ 29)
/**  PACKET parse error*/
#define         MP_ERR_PACKET                   (u16)(MP_ERR_BASE +30)


#define		MP_PACKET_TYPE_KEDA				1
#define		MP_PACKET_TYPE_OTHER_NOTRTP  	100
#define		MP_PACKET_TYPE_OTHER_RTP    	104
#define		MIN_STATIS_TIME_INTERVAL		100
#define		MAX_STATIS_TIME_INTERVAL		1000
#define		DEFAUL_KEDA_PACKET_TIME_INTERVAL	10
#define		TIME_MAX_DIFFER						300
#define		MAX_TIME_TO_FORCE_ADJUST			1000

enum
{
	FRAME_TYPE_P		= 0,
	FRAME_TYPE_I		= 1,
	FRAME_TYPE_UNKNOW	= 2     ///< 因为mediapack收到的都是包，无法区别出是否关键帧，因为type都是FRAME_TYPE_UNKNOW
} ;

/** 接收udp包结构*/
typedef struct 
{
	u64 m_qwTimeStamp;      ///< 时间戳
	u8	m_nStreamId;        ///< 流号

	u8	m_byFrameBorder;	///< 帧边界
	u8  m_byPayload;        ///< 载荷类型
	u8  m_byKeyFrame;       ///< 是否关键帧
	u16 m_wSequence;        ///< 对于该路流的序号
	u32 m_dwReserve ;        
	u32	m_dwSSRC;

	u32 m_nDataLen;           ///< 数据长度
	u8* m_pchData;            ///< 数据包指针
} TUdpPack;

/** 
 * 代码里会将重传检测点除以7，因此不可设置太小，丢弃时间点无用
 * 重传检测点的单位是序列号，而不像medianet那样是时间戳
 */
typedef struct tagMpRSParam
{
	u16  m_wFirstTimeSpan;   ///< 第一个重传检测点   
	u16  m_wSecondTimeSpan;  ///< 第二个重传检测点
	u16  m_wThirdTimeSpan;   ///< 第三个重传检测点
	u16  m_wRejectTimeSpan;  ///< 过期丢弃的时间跨度
} TMpRSParam;

typedef struct tagMpCfgParam
{
    s32  m_nCPUaffinity;
} TMpCfgParam;

u16 MediaPacketInit(TMpCfgParam* ptParam = NULL);  ///< 使用之前必须调用该函数
u16 MediaPacketRelease();///< 使用完毕必须调用该函数清空数据
u64 MpGetSysTime(BOOL32 bPrintTick = FALSE);

class CSend ;
class CRecv ;

/**
 *@brief  udp pack包回调
 *@param[out]  TUdpPack & tPack 收到的udp包回调
 *@param[out]  KD_PTR pvContext  内容根据不同类型，内容不同
 *@return     void
 *@note    
 */
typedef void (* UdpPackCallback )(TUdpPack & tPack, KD_PTR pvContext);

class CMediaPackRecv  
{
	public:
		CMediaPackRecv();
		virtual ~CMediaPackRecv();

		/**
		 *@brief  clear 接收对象和锁
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表   
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE );

		/**
		 *@brief   创建接收对象
		 *@param[in]  u8 byPacketType MP_PACKET_TYPE_KEDA  MP_PACKET_TYPE_OTHER
		 *@param[in]  u32 dwBuffSize buff 大小
		 *@param[in]  UdpPackCallback pCallback 包回调函数
		 *@param[in]  KD_PTR pvContext 内容根据不同类型，内容不同
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 Create(u8 byPacketType, u32 dwBuffSize, UdpPackCallback pCallback, KD_PTR pvContext);

		/**
		 *@brief  SetRTPAddr 设置rtp包接收地址
		 *@param[in]  u32 dwIp  rtp包接收地址
		 *@param[in]  u16 wPort rtp包接收端口
		 *@param[in]  u8 byStreamID 流ID和UdpPackCallback回调包ID对应
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRTPAddr(u32 dwIp, u16 wPort, u8 byStreamID);

		/**
		 *@brief  设置rtcp包收发地址
		 *@param[in]  u32 dwIp rtcp包接收地址
		 *@param[in]  u16 wPort rtcp包接收端口
		 *@param[in]  u32 dwDestIP 发送地址
		 *@param[in]  u16 wDestPort 发送端口
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRTCPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwDestIP, u16 wDestPort);

		/**
		 *@brief  开始接收
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 StartRecv();
		/**
		 *@brief  停止接收
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 StopRecv();

		/**
		 *@brief  设置重传参数
		 *@param[in] TMpRSParam tRsParam 重传一些参数
		 *@param[in] BOOL32 bReSend 重传开关，默认打开
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRSParam(TMpRSParam tRsParam, BOOL32 bReSend = TRUE);

		/**
		 *@brief  打印所有接收对象
		 *@retval NULL
		 *@note    
		 */
		void PrintInfo();

	private:

		CRecv 		*	m_pcRecv;
		SEMHANDLE		m_hSem ;

		BOOL32			m_bSemCreate ;
		BOOL32			m_bCreate ;

};


class CMediaPackSend  
{
	public:
		CMediaPackSend();
		virtual ~CMediaPackSend();
		/**
		 *@brief  clear 发送对象和锁
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表   
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE );

		/**
		 *@brief   创建发送对象
		 *@param[in]  u8 byPacketType MP_PACKET_TYPE_KEDA  MP_PACKET_TYPE_OTHER
		 *@param[in]  u32 dwBuffSize buf大小，分配多少个包
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 Create(u8 byPacketType, u32 dwBuffSize);

		/**
		 *@brief  SetRTPAddr 设置rtp包发送地址
		 *@param[in]  u32 dwLocalIp  rtp发送本地IP
		 *@param[in]  u16 wLocalPort 本地端口
		 *@param[in]  u32 dwDestIp   send 目标地址
		 *@param[in]  u16 wDestPort  send目标端口
		 *@param[in]  u32 dwSockType SOCK_DGRAM  SOCK_RAW
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRTPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwDestIp, u16 wDestPort, u32 dwSockType);

		/**
		 *@brief  设置SOCK_RAW 模式 地址
		 *@param[in]  u32 dwSrcIp  源IP
		 *@param[in]  u16 wSrcPort 源port
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRtpSrcAddr(u32 dwSrcIp, u16 wSrcPort);

		/**
		 *@brief  设置rtcp地址
		 *@param[in]  u32 dwIp rtcp包接收地址
		 *@param[in]  u16 wPort rtcp包接收端口
		 *@param[in]  dwSockType SOCK_DGRAM  SOCK_RAW
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRTCPAddr(u32 dwLocalIp, u16 wLocalPort, u32 dwSockType);

		/**
		 *@brief  设置SOCK_RAW 模式 rtcp 地址
		 *@param[in]  u32 dwSrcIp  源IP
		 *@param[in]  u16 wSrcPort 源port
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRtcpSrcAddr(u32 dwSrcIp, u16 wSrcPort);

		/**
		 *@brief  send 数据包
		 *@param[in]  u8 * pbData 数据包
		 *@param[in]  u16 wDataSize 包长
		 *@param[in]  u32 dwDestIp 0就是用SetRTPAddr设置的地址
		 *@param[in]  u16 wDestPort 0就是用SetRTPAddr设置的端口
		 *@param[in]  BOOL32 bCache 缓存一定包
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 Send(u8 * pbData, u16 wDataSize, u32 dwDestIp = 0, u16 wDestPort = 0, BOOL32 bCache = TRUE);

		/**
		 *@brief  打印所有发送对象
		 *@retval NULL
		 *@note    
		 */
		void PrintInfo();
		/**
		 *@brief  设置重传参数
		 *@param[in] u16 wTimeSpan 重传时间间隔
		 *@param[in] BOOL32 bReSend 重传开关，默认打开
		 *@retval    MP_ERROK 成功
		 *@retval    MP_* 上述错误列表  
		 *@note    
		 */
		u16 SetRSParam(u16 wTimeSpan, BOOL32 bReSend = TRUE);
	private:

		CSend * m_pcSend ;

		SEMHANDLE		m_hSem ;

		BOOL32			m_bCreate ;
		BOOL32			m_bSemCreate ;
};


} ///<  namespace

#endif ///<  !defined(AFX_MEDIAPACK_H__D116DFC2_ABD5_4B49_81C3_77A3F3B3316C__INCLUDED_)
