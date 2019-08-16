/*****************************************************************************
模块名  : pu前端二次开发包接口模块
文件名  : pusdkapi.h
相关文件：pusdkapi.h
实现功能：定义
作者    ：王永
版本    ：KDM-V5.0-R1.0 Copyright (C) 2009 - 2010 All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容

******************************************************************************/
#ifndef PUSDKAPI_H_
#define PUSDKAPI_H_

#include "pusdkdefine.h"
#include "pusdkstruct.h"

#ifdef __cplusplus
extern "C" {

#define KDM_API  extern "C" __declspec(dllexport)
#define STDCALL  __stdcall

#endif // __cplusplus

/*=================================================================
函数名称: KDM_InitSearch
功    能: 搜索前准备
参数说明: 
    dwLocalIp    本地IP地址，用户可以指定本地IP地址。如果是该参数是0，
	             则由底层指定当前默认的网卡地址，一般情况下该参数设置为0
    wSearchPort  本地搜索端口，默认设置值:PUSDK_VSIP_PORT
    dwMultiAddr  组播搜索地址 ，默认设置值:PUSDK_DEFAULT_MUL_ADDR
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_InitSearch( unsigned long dwLocalIp ,unsigned short  wSearchPort,unsigned long dwMultiAddr);

/*=================================================================
函数名称: KDM_AddSearchIp
功    能: 添加搜索地址和端口
参数说明: 
    dwIp   搜索地址
    wPort  搜索端口
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_AddSearchIp(unsigned long dwIp, unsigned short wPort);

/*=================================================================
函数名称: KDM_AddSearchIp
功    能: 删除搜索地址和端口
参数说明: 
    dwIp   搜索地址
    wPort  搜索端口
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DelSearchIp(unsigned long dwIp, unsigned short wPort);

/*=================================================================
函数名称: KDM_SearchPu
功    能: 搜索前端
参数说明: 
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SearchPu();

/*=================================================================
函数名称: KDM_SetSearchCB
功    能: 指定搜索回调
参数说明: 
    fCBMsg   搜索结果回调处理
    pContext 回调上下文
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetSearchCB( fPuSearch fCBMsg, void* pContext );

/*=================================================================
函数名称: KDM_GetAllPuNum
功    能: 获取搜索设备个数
参数说明: 
返 回 值: 成功 返回个数; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetAllPuNum();

/*=================================================================
函数名称: KDM_GetAllPuList
功    能: 获取搜索设备信息
参数说明: 
    ptDevInfo 设备信息数组首地址
    nNum      数组元素个数
返 回 值: 实际获得的设备数
=================================================================*/
KDM_API int STDCALL KDM_GetAllPuList( TPuUnitInfo *ptDevInfo, int nNum );

/*=================================================================
函数名称: KDM_SetPuIp
功    能: 跨网段修改IP
参数说明: 
    ptSetIpInfo 参数结构
返 回 值: int   成功 ERR_SUCCESS 或 >0 失败 ERR_INVALID、ERR_SIZE ...
=================================================================*/
KDM_API int STDCALL KDM_SetPuIp( TPuSetIpInfo *ptSetIpInfo );

/*=================================================================
函数名称: KDM_ReleaseSearch
功    能: 搜索结束释放
参数说明: 
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ReleaseSearch();

/*=================================================================
函数名称: KDM_SetMsgCB
功    能: 设置回调处理接口
参数说明: 
    fEvMsg     回调接口函数（输入）
    dwContext  回调上下文（输入）
返 回 值: void 
=================================================================*/
KDM_API void STDCALL KDM_SetMsgCB(FUNC_MSG_ACK_CB fEvMsg, void* pContext );

/*=================================================================
函数名称: KDM_InitPu
功    能: 连接设备前初始化
参数说明: 
    phHandle 输出参数，分配的设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_InitPu( OUT LPPUHANDLE phHandle );

/*=================================================================
函数名称: KDM_InitDecParam
功    能: 设置本地解码参数
参数说明: 
    wVidPlayBuf 视频播放缓存数 12-100
    wVidPlayBuf 音频播放缓存数 8-100
    wVidPlayBuf 丢包超时 0-30000 毫秒
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_InitDecParam( unsigned short wVidPlayBuf, unsigned short wAudPlayBuf, unsigned short wRejectTime );

/*=================================================================
函数名称: KDM_ReleasePu
功    能: 关闭设备连接，释放连接设备的相关资源，包括播放器等
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ReleasePu( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_ConnectPU
功    能: 连接前端,帐户和密码填NULL的时候，前端不做帐户和密码的认证，直接登陆
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    pUserName  用户名，以鉴权方式连接前端需提供，否则填空
    pPassword  密码，以鉴权方式连接前端需提供，否则填空
    dwIpAddr   前端地址
    wPort      前端连接端口，默认设置值:PUSDK_VSIP_PORT
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ConnectPU( PUHANDLE hHandle, const char* pUserName, const char* pPassword, 
                                        unsigned long dwIpAddr, unsigned short wPort );

/*=================================================================
函数名称: KDM_DisConnectPU
功    能: 断开前端连接
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DisConnectPU( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_SetProxy
功    能: 设置代理参数
参数说明: 
    hHandle     设备句柄（设备唯一标识）
    dwProxyIp   代理地址
    wProxyPort  代理端口
    strUserName 代理用户名
    wProxyPort  代理用户密码
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetProxy( PUHANDLE hHandle, unsigned long dwProxyIp, unsigned short wProxyPort,
                                       const char* pUserName, const char* pPassword );

/*=================================================================
函数名称: KDM_IamInNat
功    能: 开启穿NAT发送NAT探测包
参数说明: 
    hHandle        设备句柄（设备唯一标识）
    phPlayer       指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    hWnd           图像输出窗口句柄
    bySrcId        前端视频源编号
    emPlayType     该播放器将用于浏览码流还是播放录像
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_IamInNat( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, HWND hWnd, 
                                       unsigned char bySrcId, EPlayType emPlayType );

/*=================================================================
函数名称: KDM_SetTimeout
功    能: 设置同步超时时间
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    dwTimeOut  默认PUSDK_SYNCMD_TIMEOUT毫秒，不能为0
返 回 值: void 
=================================================================*/
KDM_API void STDCALL KDM_SetTimeout( PUHANDLE hHandle, unsigned long dwTimeOut );

/*=================================================================
函数名称: KDM_IsConnected
功    能: 是否已经连接前端
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 连接状态返回TRUE;否则返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsConnected( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_PlayVideo
功    能: 浏览视频
参数说明: 
    hHandle         设备句柄（设备唯一标识）
    phPlayer        指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    hWnd            图像输出窗口句柄
    bySrcId         前端视频源编号
    emPlayChanType  主流或辅流，默认emPlayMain 
    wVideoStartPort 码流起始端口,默认设置值60000
    pByChanId       输出视频源对应的视频源编码通道号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
          特别说明: 本接口返回值仅表示请求是否发送成功，PUSDK 使用通用回调消息告知操作结果。
          通用回调消息分别是 PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK 和 PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK
=================================================================*/
KDM_API int STDCALL KDM_PlayVideo( PUHANDLE hHandle, OUT LPPLAYERHANDLE phPlayer,
	HWND hWnd,
	unsigned char bySrcId, 
	EPlayChanType emPlayChanType /*= emPlayMain*/,					
	unsigned short wVideoStartPort /*= PUSDK_RTP_PORT*/, 
	OUT unsigned char *pByChanId /*= NULL*/);

/*=================================================================
函数名称: KDM_SwitchVideoSrc
功    能: 切换视频源，音频自动切换
参数说明: 
    hHandle     设备句柄（设备唯一标识）
    byChanId    通道号
    byVSrcId    视频源编号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SwitchVideoSrc( PUHANDLE hHandle,unsigned char byChanId, unsigned char byVSrcId );								

/*=================================================================
函数名称: KDM_StopPlay
功    能: 停止浏览音视频
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
特别说明: 本接口返回值仅表示请求是否发送成功，PUSDK 使用通用回调消息告知操作结果。
          通用回调消息分别是 PUSDK_MSG_CB_VIDEO_SEND_REMOVE_ACK 和 PUSDK_MSG_CB_AUDIO_SEND_REMOVE_ACK
=================================================================*/
KDM_API int STDCALL KDM_StopPlay( PUHANDLE hHandle, PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_PausePlay
功    能: 暂停音视频播放
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_PausePlay( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_ResumePlay
功    能: 恢复音视频浏览
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ResumePlay( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_LocalPlay
功    能: 本地接收码流播放
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    phPlayer 指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    wPort 本地接收起始端口
    hWnd 图像输出窗口句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_LocalPlay( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, unsigned short wPort, HWND hWnd );

/*=================================================================
函数名称: KDM_LocalPlayStop
功    能: 本地接收码流播放停止
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄 
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_LocalPlayStop( PUHANDLE hHandle, PLAYERHANDLE hPlayer );
	
/*=================================================================
函数名称: KDM_PlayState
功    能: 浏览状态
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: int 成功 返回 EPlayState 类型，失败 ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_PlayState( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_IsPlayVideo
功    能: 是否在浏览视频
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 正在播放返回TRUE;未播放返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsPlayVideo( PUHANDLE hHandle,PLAYERHANDLE hPlayer );


/*=================================================================
函数名称: KDM_ChangePlayWnd
功    能: 切换本地浏览窗口
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    hPlayer   播放器句柄
    hPlayWnd  本地新窗口句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ChangePlayWnd( PUHANDLE hHandle,PLAYERHANDLE hPlayer, HWND hPlayWnd ); 

/*=================================================================
函数名称: KDM_RedrawLastFrame
功    能: 窗口重画
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_RedrawLastFrame( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_StartDec
功    能: 开始解码
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_StopDec
功    能: 停止解码
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_IsDec
功    能: 是否正在解码
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 正在解码返回TRUE；否则返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_GetDecoderStatis
功    能: 获得解码信息
参数说明: 
    hHandle     设备句柄（设备唯一标识）
    hPlayer     播放器句柄
    ptDecStatis 统计结果数据输出
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetDecoderStatis( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                               TPuDecStatis* ptDecStatis );

/*=================================================================
函数名称: KDM_SetDrawCallBack
功    能: 设置解码回调
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    hPlayer  播放器句柄
    pFunc    回调处理函数
    pContext 回调相关上下文
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetDrawCallBack( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                         PUSDK_DRAW_CB pFunc,  unsigned long dwContext );
/*=================================================================
函数名称: KDM_SetFrameInfoCallBack
功    能: 设置解码回调
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
    pFunc   回调处理函数
    dwParam 回调相关上下文
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetFrameInfoCallBack( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
										      PUSDK_FRAMEINFO_CB pFunc, unsigned long dwContext );
/*=================================================================
函数名称: KDM_SetFrameCallBack
功    能: 设置解码回调
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
    pFunc   回调处理函数
    dwParam 回调相关上下文
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetFrameCallBack( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
										  PUSDK_FRAME_CB pFunc, unsigned long dwContext  );

/*=================================================================
函数名称: KDM_IsAudioMute
功    能: 是否本地静音
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 静音返回TRUE；否则返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsAudioMute( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_SetMute
功    能: 设置本地静音
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
    bMute   是否静音
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetMute( PUHANDLE hHandle,PLAYERHANDLE hPlayer, int bMute );

/*=================================================================
函数名称: KDM_SetVolume
功    能: 设置播放音量
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    hPlayer  播放器句柄
    byVolume 音量值 0-25
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetVolume( PUHANDLE hHandle,PLAYERHANDLE hPlayer, unsigned char byVolume );

/*=================================================================
函数名称: KDM_GetVolume
功    能: 获取播放音量
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
    pbyVolume  输出：音量值 0-25
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetVolume( PUHANDLE hHandle,PLAYERHANDLE hPlayer, OUT unsigned char* pbyVolume );

/*=================================================================
函数名称: KDM_SetCallVolume
功    能: 设置呼叫音量
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    hPlayer  播放器句柄
    byVolume 音量值 0-10
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetCallVolume( PUHANDLE hHandle, PLAYERHANDLE hPlayer, unsigned char byVolume );

/*=================================================================
函数名称: KDM_GetCallVolume
功    能: 获取呼叫音量
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
    pbyVolume  输出：音量值 0-10
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetCallVolume( PUHANDLE hHandle, PLAYERHANDLE hPlayer, OUT unsigned char* pbyVolume );

/*=================================================================
函数名称: KDM_InputAudPcmData
功    能: 输入PCM数据并进行编码
参数说明: 
	hHandle      设备句柄（设备唯一标识）
	phPlayer     播放器句柄
	pPcmData     数据指针；
	nDataLen     数据的长度（字节），最大支持2048字节；
	dwSampleRata 数据的采样率，只支持8000
	wBitLen      采样位宽，只支持16;
	byChnNum     声道数，只支持1
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_InputAudPcmData( PUHANDLE hHandle, PLAYERHANDLE hPlayer,
										void* pPcmData, int nDataLen,
										unsigned long dwSampleRate, 
										unsigned short wBitLen, 
										unsigned char byChnNum);

/*=================================================================
函数名称: KDM_StartPcmEnc
功    能: 输入PCM数据并进行编码
参数说明: 
	hHandle               设备句柄（设备唯一标识）
	phPlayer              播放器句柄
	pFnBitStreamCB        音频编码码流回调函数
	dwBitstreamCBContext  回调上下文
	pFnRtpPackCB          rtp包回调函数
	dwRtpPackCBContex     回调上下文
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartPcmEnc(PUHANDLE hHandle, PLAYERHANDLE hPlayer,  
									PUSDK_INPUT_PCM_BIT_STREAM_CB pFnBitStreamCB,  unsigned long dwBitstreamCBContext, 
									PUSDK_INPUT_PCM_RTP_PACK_CB pFnRtpPackCB,  unsigned long dwRtpPackCBContex );

/*=================================================================
函数名称: KDM_StopPcmEnc
功    能: 停止PCM编码
参数说明: 
	hHandle    设备句柄（设备唯一标识）
	phPlayer   播放器句柄 
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopPcmEnc(PUHANDLE hHandle, PLAYERHANDLE hPlayer);


/*=================================================================
函数名称: KDM_StartLocalRecVideo
功    能: 网络接收视频数据，在不解码的情况下进行本地录像
参数说明: 
    hHandle         设备句柄（设备唯一标识）
    phPlayer        指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    bySrcId         前端视频源编号
    emPlayChanType  主流或辅流，默认emPlayMain 
    wVideoStartPort 码流起始端口,默认设置值60000
    pByChanId       输出视频源对应的视频源编码通道号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
          特别说明: 本接口返回值仅表示请求是否发送成功，PUSDK 使用通用回调消息告知操作结果。
          通用回调消息分别是 PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK 和 PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK
=================================================================*/
KDM_API int STDCALL KDM_StartLocalRecVideo( PUHANDLE hHandle, OUT LPPLAYERHANDLE phPlayer,
	                                        unsigned char bySrcId, 
	                                        EPlayChanType emPlayChanType /*= emPlayMain*/,					
	                                        unsigned short wVideoStartPort /*= PUSDK_RTP_PORT*/, 
	                                        OUT unsigned char *pByChanId /*= NULL*/);

/*=================================================================
函数名称: KDM_StartLocalRec
功    能: 开始本地录像
参数说明: 
    hHandle       设备句柄（设备唯一标识）
    hPlayer       播放器句柄
    strSaveDir    录像保存目录
    strFileName   录像文件名称
    wLocalRecPort 暂未使用,默认设置值PUSDK_LOCAL_REC_PORT 
    fpProcCB      本地录像进度回调
    fpStateCB     本地录像状态回调
    dwContext     本地录像回调参数
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/

KDM_API int STDCALL KDM_StartLocalRec( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
            const char*             pSaveDir, const char* pFileName, 
			unsigned short          wLocalRecPort /*= PUSDK_LOCAL_REC_PORT*/,
            PUSDK_LOC_REC_PRO_CB    fProcCB /*= NULL*/,
            PUSDK_LOC_REC_STATE_CB  fStateCB /*= NULL*/,
            unsigned long           dwContext /*= NULL*/
            );

/*=================================================================
函数名称: KDM_IsLocalRecording
功    能: 是否在本地录像
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: TRUE:正在本地录像，FALSE:没有本地录像
=================================================================*/
KDM_API int STDCALL KDM_IsLocalRecording( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_StopLocalRec
功    能: 停止本地录像
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopLocalRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_LocalSnapshot
功    能: 本地抓拍
参数说明: 
    hHandle       设备句柄（设备唯一标识）
    hPlayer       播放器句柄
    pFilePathName 抓拍文件保存全路径，后缀名支持(.bmp|.jpeg)
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_LocalSnapshot( PUHANDLE hHandle, PLAYERHANDLE hPlayer, const char * pFilePathName );

/*=================================================================
函数名称: KDM_SyncPuTime
功    能: 同步时间
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    lSystemTime   PUSDK_SYNC_TIME_LOCAL 表示自动获取电脑当前系统时间，非0为指定的时间。
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SyncPuTime( PUHANDLE hHandle, time_t lSystemTime /*= PUSDK_SYNC_TIME_LOCAL*/ );

/*=================================================================
函数名称: KDM_RebootPu
功    能: 重启前端
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_RebootPu( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_SaveSettings
功    能: 保存前端参数
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SaveSettings( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_EnableCheckPuConnection
功    能: 断链检测
参数说明: 
    hHandle       设备句柄（设备唯一标识）
    bEnable       TRUE:开启断链检测，FALSE:关闭断链检测
    wCheckTime    断链检测间隔，默认设置值PUSDK_TEST_LINK 
    byChechCount  断链检测次数，默认设置值为PUSDK_TEST_LINK_COUNT
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_EnableCheckPuConnection( PUHANDLE hHandle, int bEnable, 
                                                      unsigned short wCheckTime /*= PUSDK_TEST_LINK*/, 
                                                      unsigned char byChechCount /*= PUSDK_TEST_LINK_COUNT*/ );

/*=================================================================
函数名称: KDM_UploadTitlePic
功    能: 上传图片，同步实现
参数说明: 
    hHandle       设备句柄（设备唯一标识）
    eType         图片类型
    strFilePath   文件路径
    byVideoSrc    视频源编号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_UploadTitlePic( PUHANDLE hHandle, EPuBmpType eType, const char* pFilePath, unsigned char byVideoSrc );

/*=================================================================
函数名称: KDM_GetPuRecNum
功    能: 获取前端录像数
参数说明: 
    hHandle            设备句柄（设备唯一标识）
    ptPuRecQueryNumReq 录像个数查询条件
    ptAck              输出：应答结果
返 回 值: int   成功，返回个数 失败 ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_GetPuRecNum( PUHANDLE hHandle, const TPuRecQueryNumReq* ptPuRecQueryNumReq, 
									OUT TPuRecQueryNumAck* ptAck );	

/*=================================================================
函数名称: KDM_SearchPuRec
功    能: 搜索前端录像文件信息
参数说明: 
    hHandle             设备句柄（设备唯一标识）
    ptPuRecQueryFileReq 搜索条件
    ptAck               输出：搜索结果应答
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SearchPuRec( PUHANDLE hHandle, const TPuRecQueryFileReq* ptPuRecQueryFileReq, 
                                          OUT TPuRecQueryFileAck* ptAck );

/*=================================================================
函数名称: KDM_StartPlayPuRec
功    能: 开始播放前端录像
参数说明: 
    hHandle          设备句柄（设备唯一标识）
    phPlayer         指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    hWnd             图像输出窗口句柄
    ptPuPlayStartReq 播放请求条件
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartPlayPuRec( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, HWND hWnd, 
                                        TPuPlayStartReq* ptPuPlayStartReq );

/*=================================================================
函数名称: KDM_SetDecoderMode
功    能: 设置解码模式 
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
    bUserTimer 设置解码模式 bUserTimer:是否使用定时解码模式 TRUE:是，FALSE:即时方式，即来一帧就解一帧
               在播放录像快进的时候，需要使用即时方式，浏览视频播放录像的时候，使用定时方式
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetDecoderMode( PUHANDLE hHandle,PLAYERHANDLE hPlayer, int bUserTimer );

/*=================================================================
函数名称: KDM_SetCoverDownload
功    能: 设置下载录像是否是覆盖下载 
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
    bCoverFlag TRUE:覆盖下载；FALSE:不覆盖下载
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetCoverDownload(PUHANDLE hHandle, PLAYERHANDLE hPlayer,int bCoverFlag );

/*=================================================================
函数名称: KDM_SetDecoderMode
功    能: 获取下载录像的下载粒度 
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
	pFile      下载文件录像名称
	pwRate     下载粒度
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetRecDownRate(PUHANDLE hHandle, PLAYERHANDLE hPlayer,const char * pFile, unsigned short* pwRate );

/*=================================================================
函数名称: KDM_IsPlayPuRec
功    能: 是否正在播放指定的前端录像 
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 正在前端录像返回TRUE；否则返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsPlayPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_StopPlayPuRec
功    能: 停止播放前端录像
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopPlayPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_PuRecPlayOperation
功    能: 前端录像控制操作
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    hPlayer    播放器句柄
    ptRecOpt   操作类型
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_PuRecPlayOperation( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                                 const TPuRecPlayOperation *ptRecOpt );

/*=================================================================
函数名称: KDM_PuRecSeekLocation
功    能: 前端录像播放拖动操作
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    byPlayId player id
    dwDragTime 拖放时间
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_PuRecSeekLocation( PUHANDLE hHandle,unsigned char byPlayId, 
                                                const unsigned long dwDragTime );

/*=================================================================
函数名称: KDM_DownloadPuRec
功    能: 下载录像文件
参数说明: 
    hHandle              设备句柄（设备唯一标识）
    phPlayer             指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    ptRecFileDownloadReq 下载条件
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DownloadPuRec( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, 
                                            const PUSDK_TPuRecFileDownloadReq* ptRecFileDownloadReq );

/*=================================================================
函数名称: KDM_StopDownloadPuRec
功    能: 停止下载前端录像
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopDownloadPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_DeletePuRec
功    能: 删除指定的前端录像
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    ptFileInfo 删除条件
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DeletePuRec( PUHANDLE hHandle, const TPuRecFileDelReq* ptFileInfo );

/*=================================================================
函数名称: KDM_StartPuRec
功    能: 开始前端手动录像
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    byChanId 编码通道
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartPuRec( PUHANDLE hHandle, const unsigned char byChanId );

/*=================================================================
函数名称: KDM_StopPuRec
功    能: 停止前端手动录像
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    byChanId 编码通道
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopPuRec( PUHANDLE hHandle, const unsigned char byChanId );
	
/*=================================================================
函数名称: KDM_SearchPuPic
功    能: 查询前端图片
参数说明: 
    hHandle             设备句柄（设备唯一标识）
    ptPuPicFileQueryReq 查询条件
    ptAck               查询应答
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SearchPuPic( PUHANDLE hHandle, const TPuPicFileQueryReq * ptPuPicFileQueryReq, 
                                          TPuPicFileQueryAck* ptAck );

/*=================================================================
函数名称: KDM_DownloadPuPic
功    能: 下载前端图片
参数说明: 
    hHandle           设备句柄（设备唯一标识）
    ptPicFileDownload 下载条件
    pSavePathName     保存图片名称及路径
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DownloadPuPic( PUHANDLE hHandle, const TPuPicFileDownloadReq* ptPicFileDownload,
                                            const char * pSavePathName );

/*=================================================================
函数名称: KDM_BrowsePuPic
功    能: 浏览图片，同步实现
参数说明: 
    hHandle             设备句柄（设备唯一标识）
    ptPicFileDownload   下载条件
    ptPicFile           输出：下载完的图片数据
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_BrowsePuPic( PUHANDLE hHandle, const TPuPicFileDownloadReq* ptPicFileDownload, 
                                          OUT TPuPicFileDownloadRes* ptPicFile );
	
/*=================================================================
函数名称: KDM_DeletePuPic
功    能: 删除前端图片 
参数说明: 
    hHandle          设备句柄（设备唯一标识）
    ptPuPicFileInfo  删除条件
    ptAck            删除结果
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DeletePuPic( PUHANDLE hHandle, const TPuPicFileDelExReq*  ptPuPicFileInfo, 
                                          TPuPicFileDelAck* ptAck );

/*=================================================================
函数名称: KDM_GetPuCapVal
功    能: 获取能力集
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    byType   能力类型，参见 ECapType
返 回 值: int   成功，返回能力值 失败 ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_GetPuCapVal( PUHANDLE hHandle, ECapType byType);

/*=================================================================
函数名称: KDM_GetPuParam
功    能: 获取配置参数
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    eCfgType     配置类型
    byChan       编码通道号
    pParam       输出：配置数据指针
    dwBufSize    配置数据大小
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetPuParam( PUHANDLE hHandle, EPuConfigType eCfgType, unsigned char byChanId, 
                                         OUT void* pParam, unsigned long dwBufSize );

/*=================================================================
函数名称: KDM_SetPuParam
功    能: 设置配置参数
参数说明: 
    hHandle      设备句柄（设备唯一标识）
    eCfgType     配置类型
    byChanId     编码通道号
    pParam       配置数据指针
    dwParamSize  配置数据大小
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_SetPuParam( PUHANDLE hHandle, EPuConfigType eCfgType, unsigned char byChanId, 
                                         const void* pParam, unsigned long dwParamSize );

/*=================================================================
函数名称: KDM_LoadDefaultSettings
功    能: 加载默认参数
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_LoadDefaultSettings( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_LoadDefaultVedioSettings
功    能: 加载默认视频参数
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    byChanId  编码通道号
    loadType  默认加载视频图像的亮度、对比度等值
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_LoadDefaultVedioSettings( PUHANDLE hHandle, unsigned char byChanId, EPuLoadDefualt eType );

/*=================================================================
函数名称: KDM_ControlCamera
功    能: 控制摄像头移动
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    byVideoSrc 视频源编号
    byMoveType 移动类型，参见 ECamControl
    byParam    移动幅度,默认为PUSDK_CAMERA_MOVE_RANGE
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ControlCamera( PUHANDLE hHandle, unsigned char byVideoSrc, 
                                       unsigned char byMoveType, unsigned char byParam);

/*=================================================================
函数名称: KDM_ControlCameraEx
功    能: 控制摄像头移动(4路IPC兼容版)
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    byVideoSrc 视频源编号
    byMoveType 控制类型，参见 ECamControl
    byParam(1~4)    参数1~4
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ControlCameraEx( PUHANDLE hHandle, BYTE byVideoSrc, BYTE byOperateType,
		BYTE byParam1, BYTE byParam2, BYTE byParam3, BYTE byParam4 );

/*=================================================================
函数名称: KDM_ControlCamera
功    能: 直接发送数据控制摄像头操作
参数说明:
	hHandle        设备句柄（设备唯一标识）
	byVideoSrc     视频源编号
	pCtrlDate      控制命令数据
	dwSize         数据长度
返 回 值: 返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
		  只要摄像头信令发送至前端，就认为成功，至于这个命令是否能生效
		  本地无法知道，所以如果本函数返回成功，但是没有实际的效果应该
		  检查发送的命名是否正确
=================================================================*/
KDM_API int STDCALL KDM_ControlCameraDirect(PUHANDLE hHandle, unsigned char byVideoSrc, 
										    void* pCtrlDate, unsigned long dwSize );

/*=================================================================
函数名称: KDM_GetCamCode
功    能: 获取摄像头回归操作的控制数据
参数说明: 
    hHandle    设备句柄（设备唯一标识）
	pCode      控制数据缓冲区
    nSize      控制数据缓冲区长度
	byCam      摄像头类型
    byAddrId   摄像头ID
    pbyCodeLen 返回的数据长度
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL  KDM_GetCamCode(PUHANDLE hHandle, char* pCode, int nSize, 
									unsigned char byCam, unsigned char byAddrId,unsigned char* pbyCodeLen);

/*=================================================================
函数名称: KDM_GetDiskPar
功    能: 获取磁盘信息
参数说明: 
    hHandle      设备句柄（设备唯一标识）
    nDiskId      磁盘号
    pDiskPartion 输出：系统分区信息
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetDiskPar( PUHANDLE hHandle, int nDiskId, OUT TPuDiskPartionParam* pDiskPartion );

/*=================================================================
函数名称: KDM_GetSysService
功    能: 获取系统服务
参数说明: 
    hHandle         设备句柄（设备唯一标识）
    ptSysService    输出：服务状态
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
// 特别说明: V4不支持
=================================================================*/
KDM_API int STDCALL KDM_GetSysService( PUHANDLE hHandle, OUT TPuSysService* ptSysService );

/*=================================================================
函数名称: KDM_GetSysInfo
功    能: 获取系统信息
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    ptSysInfo 输出：系统信息
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
// 特别说明: V4不支持
=================================================================*/
KDM_API int STDCALL KDM_GetSysInfo( PUHANDLE hHandle, OUT TPuSysInfo* ptSysInfo );

/*=================================================================
函数名称: KDM_GetInputPinState
功    能: 获取输入通道状态
参数说明: 
    hHandle  设备句柄（设备唯一标识）
    chanId   通道编号
    ptState  输出：通道输入状态
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
// 特别说明: V4不支持
=================================================================*/
KDM_API int STDCALL KDM_GetInputPinState( PUHANDLE hHandle, unsigned char byChanId, 
                                               OUT TPuInputPinState* ptState );

/*=================================================================
函数名称: KDM_GetOutputPinState
功    能: 获取输出通道状态
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    byChanId  通道编号，无用
    ptState   输出：通道输出状态
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
// 特别说明: V4不支持
=================================================================*/
KDM_API int STDCALL KDM_GetOutputPinState( PUHANDLE hHandle, unsigned char byChanId, 
                                                OUT TPuOutputPinState* ptState );

/*=================================================================
函数名称: KDM_GetChanState
功    能: 获取通道状态，所有通道状态
参数说明: 
    hHandle       设备句柄（设备唯一标识）
    ptChanReq     请求参数
    ptChanState   输出：通道状态数据
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
// 特别说明: V4不支持
=================================================================*/
KDM_API int STDCALL KDM_GetChanState( PUHANDLE hHandle, TPuChanStateReq* ptChanReq, 
                                           OUT TPuChanStateAck* ptChanState );

/*=================================================================
函数名称: KDM_ProductTest
功    能: 生产测试信令
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    nChanId 通道号
    pTestInfo 生产测试通讯数据
    byBufAck 输出：收到的应答信息
    nBufLen 应答信息缓存的长度
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_ProductTest( PUHANDLE hHandle, int nChanId,TProTestInfo *pTestInfo, 
									  IN unsigned char* byBufAck, IN int nBufLen );

/*=================================================================
函数名称: KDM_StartVideoRecv
功    能: 开启解码器视频接收
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    chanId  通道编号
    wPort   接收端口
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartVideoRecv( PUHANDLE hHandle, unsigned char byChanId, unsigned short wPort );

/*=================================================================
函数名称: KDM_StartAudioRecv
功    能: 开启解码器音频接收
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    byChanId  通道编号
    wPort     接收端口
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartAudioRecv( PUHANDLE hHandle, unsigned char byChanId, unsigned short wPort );

/*=================================================================
函数名称: KDM_StopVideoRecv
功    能: 关闭解码器视频接收
参数说明: 
    hHandle   设备句柄（设备唯一标识）
    byChanId  通道编号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopVideoRecv( PUHANDLE hHandle, unsigned char byChanId );

/*=================================================================
函数名称: KDM_StopAudioRecv
功    能: 关闭解码器音频接收
参数说明: 
    hHandle    设备句柄（设备唯一标识）
    byChanId   通道编号
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopAudioRecv( PUHANDLE hHandle, unsigned char byChanId);

/*=================================================================
函数名称: KDM_GetVEncChanRelation
功    能: 获取视频编码通道关系表参数结构
参数说明: 
    hHandle     设备句柄（设备唯一标识）
    ptRelation  输出：视频编码通道关系表参数结构
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetVEncChanRelation( PUHANDLE hHandle, 
                                                  OUT TPuVideoEncChannelRelation* ptRelation );

/*=================================================================
函数名称: KDM_GetVideoEncInputPortParam
功    能: 获取视频编码视频源端口参数结构
参数说明: 
    hHandle     设备句柄（设备唯一标识）
    ptRelation  输出：视频编码通道关系表参数结构
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetVideoEncInputPortParam( PUHANDLE hHandle, 
                                                  OUT TPuVideoEncInputPortParam atVideoEncInputPort[16] );

/*=================================================================
函数名称: KDM_GetPuInfo
功    能: 获取设备PING应答信息
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    ptInfo  设备PING应答信息
返 回 值: void
=================================================================*/
KDM_API void STDCALL KDM_GetPuInfo( PUHANDLE hHandle, TPuUnitInfo* ptInfo );

/*=================================================================
函数名称: KDM_GetBindIp
功    能: 获取连接前端绑定的本地IP地址
参数说明: 
    hHandle 设备句柄（设备唯一标识）
返 回 值: 成功返回本地通讯绑定的地址， 失败返回小于0的错误码.
=================================================================*/
KDM_API unsigned long STDCALL KDM_GetBindIp( PUHANDLE hHandle );

/*=================================================================
函数名称: KDM_StartAudioSend
功    能: 发送音频
参数说明: 
    hHandle      设备句柄（设备唯一标识）
    phPlayer     指向播放器句柄的指针，或输出由PUSDK自动指派的播放器句柄
    byDecChanId  音频解码通道编号
    wLocalPort   本地发送端口
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StartAudioSend( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, unsigned char byDecChanId, 
									    unsigned short wLocalPort);

/*=================================================================
函数名称: KDM_StopAudioSend
功    能: 停止发送音频
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄 
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_StopAudioSend( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
函数名称: KDM_IsAudioSend
功    能: 是否正在发送音频
参数说明: 
    hHandle 设备句柄（设备唯一标识）
    hPlayer 播放器句柄 
返 回 值: 返回TRUE正在发送音频，否则返回FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsAudioSend( PUHANDLE hHandle, PLAYERHANDLE hPlayer, BOOL& bAudioSend);

/*=================================================================
函数名称: GetFormatInfo
功    能: 获取磁盘格式化信息
参数说明: 
	hHandle          设备句柄（设备唯一标识）
    ptFormatDiskInfo 磁盘格式化信息 
返 回 值: 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_GetFormatInfo(PUHANDLE hHandle, TPuFormatGetDiskInfo* ptFormatDiskInfo );

/*=================================================================
函数名称: KDM_DiskFormat
功    能: 格式化磁盘
参数说明: 
	hHandle    设备句柄（设备唯一标识）
    ptDiskInfo 磁盘格式化信息 
返 回 值: 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
KDM_API int STDCALL KDM_DiskFormat(PUHANDLE hHandle,const  TPuFormatSetDiskInfo* ptDiskInfo );

/*=================================================================
函数名称: KDM_WriteComData
功    能: 写串口数据
参数说明: 
    hHandle  磁盘格式化信息 
	pComData 串口数据结构指针
返 回 值: 返回ERR_SUCCESS; 失败返回小于0的错误码 
备    注：上报信令PUSDK_COM_DATA_INFORM
=================================================================*/
KDM_API int STDCALL KDM_WriteComData(PUHANDLE hHandle,const  TPuComData* pComData );
	
/*===============================================================
函数名称: GetErrorSreing
功    能: 获取错误码描述字符串
参数说明: 
    nErrCode    错误码
	pErrString  错误码描述字符串缓冲区
	nSize       缓冲区长度
返 回 值: void
================================================================*/
KDM_API int STDCALL GetErrorSting(int nErrCode,char* pErrString,int nSize);

/*===============================================================
函数名称: KDM_DebugLevel
功    能: 自定义调试打印级别
参数说明: 
    byLevel 0-255之间，当设置为0的时候，不打印打印信息，当设置为255的时候
	        打印所有打印信息；输出的打印信息为打印级别以下的打印信息
			例如：打印级别设置为100，输出的打印信息级别为0-100的打印
返 回 值: void
================================================================*/
KDM_API void STDCALL KDM_DebugLevel( unsigned char byLevel );

/*===============================================================
函数名称: KDM_DebugOutput
功    能: 自定义调试打印级别
参数说明: 
    byLevel    打印级别 ；0-255
	lpszFormat 与printf中的参数使用方法相同，默认在打印的信息结尾加了\n分行
返 回 值: void
================================================================*/
KDM_API void STDCALL KDM_DebugOutput( unsigned char byLevel, char* lpszFormat, ...);

/*===============================================================
函数名称: KDM_GetCpuInfo
功    能: 取得当前的CPU信息
参数说明: 
    pIdlePercent  空闲的CPU百分比 	0-100
返 回 值: 成功返回ERR_SUCCESS; 失败返回小于0的错误码
================================================================*/
KDM_API int STDCALL KDM_GetCpuInfo( OUT int* pIdlePercent );

/*===============================================================
函数名称: KDM_SetVerticalSyncMode
功    能: 开启或关闭解码的垂直同步
参数说明: 
    bVerSync  是否开启垂直同步
返 回 值: 成功返回ERR_SUCCESS; 失败返回小于0的错误码
================================================================*/
KDM_API INT STDCALL KDM_SetVerticalSyncMode( BOOL bVerSync );

/*===============================================================
函数名称: KDM_SetShowMargins
功    能: 设置图像裁剪
参数说明: 
    nLeft   裁剪的左边缘宽度
	nTop    裁剪的上边缘宽度 
	nRight  裁剪的右边缘宽度 
	nBottom 裁剪的下边缘宽度
	bAuto   是否会随解码图象大小改变
返 回 值: 成功返回ERR_SUCCESS; 失败返回小于0的错误码
================================================================*/
KDM_API int STDCALL KDM_SetShowMargins(int nLeft, int nTop, int nRight, int nBottom, BOOL bAuto = TRUE);

/*===============================================================
函数名称: KDM_SetResetImage
功    能: 设置图像参数复位
参数说明: 
 
返 回 值: 成功返回ERR_SUCCESS; 失败返回小于0的错误码
================================================================*/
KDM_API int STDCALL KDM_SetResetImage(PUHANDLE hHandle, unsigned char byChanId, TPuLoadDefaultConfig* ptPuLoadDefaultConfig);

/*===============================================================
函数名称: KDM_SetResetImage
功    能: 设置图像参数复位
参数说明: 
 
返 回 值: 成功返回ERR_SUCCESS; 失败返回小于0的错误码
================================================================*/
//KDM_API int STDCALL KDM_StartPuSnap(PUHANDLE hHandle, const char* pszSavePath);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // 
