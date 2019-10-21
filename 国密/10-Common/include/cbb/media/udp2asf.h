/**
 * @file udp2asf.h
 * @brief    udp -> asf 暂不支持双流
 * @author    白昊
 * @date 2007/08/25
 * @version  1.0
 * @copyright V1.0  Copyright(C) 2016 Keda All rights reserved.
 */
#if !defined(AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_)
#define AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_

#include "osp.h"
#include "kdvdef.h"
#include "kdmfileinterface.h"
#include "kdvmedianet.h"
#ifdef _WIN32
#pragma comment(lib, "kdmfileinterface.lib")
#endif

#define				UA_ERR_BASE						7300
/**udp2asf返回 OK*/
#define				UA_ERROK						0
/**udp2asf返回 没有创建*/
#define				UA_ERR_NOT_CREATE				UA_ERR_BASE + 1
/**udp2asf返回 参数错误*/
#define				UA_ERR_PARAM					UA_ERR_BASE + 2
/**udp2asf返回 没初始化*/
#define				UA_ERR_NOT_INIT					UA_ERR_BASE + 3
/**udp2asf返回 初始化失败*/
#define				UA_ERR_INIT_FAIL				UA_ERR_BASE + 4
/**udp2asf返回 锁创建失败*/
#define				UA_ERR_SEM_CREATE				UA_ERR_BASE + 5
/**udp2asf返回 内存越界*/
#define				UA_ERR_OUT_OF_MEM				UA_ERR_BASE + 6
/**udp2asf返回 媒体类型不支持*/
#define				UA_ERR_MEDIATYPE_UNSUPPORT		UA_ERR_BASE + 7
/**udp2asf返回 参数改变*/
#define				UA_ERR_FORMAT_CHANGED			UA_ERR_BASE + 8
/**udp2asf返回 超过最大注册数 256*/
#define				UA_ERR_REACH_MAX_REG			UA_ERR_BASE + 9
/**udp2asf返回 没有找到匹配的注册ip和port*/
#define				UA_ERR_NOT_FIND_REG				UA_ERR_BASE + 10
/**udp2asf返回 回调函数NULL*/
#define				UA_ERR_NOT_FIND_CB				UA_ERR_BASE + 11
/**udp2asf返回 port 为0*/
#define				UA_ERR_OUT_OF_PORT				UA_ERR_BASE + 12
/**udp2asf返回 等frame*/
#define				UA_ERR_WAIT_FRAME				UA_ERR_BASE + 13
/**udp2asf返回 存储文件格式不支持*/
#define             UA_ERR_FILETYPE_UNSUPPORT       UA_ERR_BASE + 14

/** 网络地址结构*/
typedef  struct  tagTU2ANetAddr 
{
	u32  m_dwIp;	///< 地址
	u16  m_wPort;	///< 端口号
}TU2ANetAddr;

enum
{
	FileType_ByFileName = 0,///< 根据文件名来判断文件类型
	FileType_Asf,///< 不检测传入的文件名，创建asf文件
	FileType_Mp4,///< 创建mp4文件
};

/**
 *@brief  包接收回调函数
 *@param[out]  u8* pPackBuf 包数据
 *@param[out]  u16 wPackLen 包长
 *@param[out]  TU2ANetAddr* ptDstAddr 发送地址
 *@param[out]  TU2ANetAddr* ptSrcAddr NULL
 *@param[out]  u64 qwTimeStamp 0
 *@param[out]  KD_PTR pContext  内容根据不同类型，内容不同
 *@return     
 *@note       
 */
typedef void* (*U2AUdpPackRecvCallback)(u8* pPackBuf, u16 wPackLen, TU2ANetAddr* ptDstAddr, TU2ANetAddr* ptSrcAddr, u64 qwTimeStamp, KD_PTR pContext);

/**
 *@brief  组帧回调
 *@param[out]  PFRAMEHDR pFrame 回调帧信息
 *@param[out]  KD_PTR pContext  内容根据不同类型，内容不同
 *@return     void
 *@ref        
 *@see
 */
typedef void* (*U2AFrameCB)(PFRAMEHDR pFrame, KD_PTR pContext);

/**
 *@brief  注册pack接收回调
 *@param[in] void* pRcvAddr 接收地址
 *@param[in] U2AUdpPackRecvCallback pUdpCallBack 包接收回调
 *@param[in] KD_PTR pContext  内容根据不同类型，内容不同
 *@retval    UA_ERROK 成功
 *@retval    !UA_ERROK 失败，参考具体错误码
 */
u16 U2ARegSndChannel(void* pRcvAddr, U2AUdpPackRecvCallback pUdpCallBack, KD_PTR pContext);

/**
 *@brief 删除接收回调
 *@param[in] void* pRcvAddr 接收地址
 *@retval    UA_ERROK 成功
 *@retval    !UA_ERROK 失败，参考具体错误码
 */
u16 U2AUnRegSndChannel(void* pRcvAddr) ;

/**
 *@brief  udp2asf初始化
 *@retval    UA_ERROK 成功
 *@retval    !UA_ERROK 失败，参考具体错误码
 *@note
 */
u16 U2AInit();

/**
 *@brief  udp2asf释放
 *@retval    UA_ERROK 成功
 *@retval    !UA_ERROK 失败，参考具体错误码
 *@note    
 */
u16 U2ARelease();

class CGenAsf ;

class CUdp2Asf  
{
	public:
		CUdp2Asf();
		virtual ~CUdp2Asf();
		/**
		 *@brief  clear 对象
		 *@param[in]  BOOL32 bSem TRUE 
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码  
		 *@note    
		 */
		u16 Clear(BOOL32 bSem = TRUE);

		/**
		 *@brief   创建存储文件
		 *@param[in]  s8 * pchFileName 文件名
		 *@param[in]  u8 byFileType 文件类型 FileType_ByFileName根据后缀确定
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码 
		 *@note    
		 */
	u16 Create(s8 * pchFileName, u8 byFileType = FileType_ByFileName, u32 qwCreateTime = 0);

		/**
		 *@brief   创建帧回调接口
		 *@param[in]  U2AFrameCB pfCB帧回调函数
		 *@param[in]  KD_PTR pContext 内容根据不同类型，内容不同
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码 
		 *@note    
		 */
		u16 Create(U2AFrameCB pfCB, KD_PTR pContext);

		/**
		 *@brief   停止录像
		 *@param[in]  NULL
		 *@retval   void
		 *@note    
		 */
		u16 EndWrite();

		/**
		 *@brief   清楚buf
		 *@param[in]  NULL
		 *@retval   void
		 *@note    
		 */
		u16 ResetBuf();


		/**
		 *@brief   设置 动态载荷的 Playload值
		 *@param[in]  u8 byStreamId 流ID
		 *@param[in]  u8 byRmtActivePT 外场自定义payload
		 *@param[in]  u8 byRealPT 我们公司的payload
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码  
		 *@note    
		 */
		u16 SetActivePT(u8 byStreamId, u8 byRmtActivePT, u8 byRealPT);

		/**
		 *@brief   设置是否接收4k码流,默认为false。
		 *@param[in]  u8 byStreamId 流ID
		 *@param[in]  BOOL32 bis4k 是否4K
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码
		 *@note    
		 */
		u16 SetIs4k(u8 byStreamId, BOOL32 bis4k = false);

		/**
		 *@brief   input 数据,业务接收到数据后直接给我们
		 *@param[in]  u8 * pbData 数据包
		 *@param[in]  u16 wDataSize 长度
		 *@param[in]  u64 qwTime 时间戳
		 *@param[in]  u8 byStreamId 流ID
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码
		 *@note    
		 */
		  u16 SetStreamSave(u8 byStreamId, u8 byAudioStreamNo,BOOL32 bIsSave);
		u16 InputUdp(u8 * pbData, u16 wDataSize, u64 qwTime, u8 byStreamId);

		/**
		 *@brief   设置时间戳采样率。
		 *@param[in]  u8 byStreamId 流ID
		 *@param[in]  u32 dwSample 采样率
		 *@retval    UA_ERROK 成功
		 *@retval    !UA_ERROK 失败，参考具体错误码 
		 *@note    
		 */
		u16 SetTimestampSample(u8 byStreamId, u32 dwSample);
	u16 SetUserData(u8* pDataBuffer, u32 dwBufferSize);
	u16 FileSetUserData(LPCSTR pFilePath, u8* pDataBuffer, u32 dwBufferSize);
	public:

	private:

		SEMHANDLE	m_hSem;

		BOOL32		m_bSemCreate;
		BOOL32		m_bCreate;

		CGenAsf *	m_pcGenAsf ;


};

#endif // !defined(AFX_UDP2ASF_H__0B8FD9F0_00C1_4BAC_9894_168119DB218E__INCLUDED_)
