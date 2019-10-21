/*****************************************************************************
模块名  : ipcsdk开发包接口模块
文件名  : ipcsdk.h
相关文件：ipcsdk.h
实现功能：
作者    ：钱赟辰
版本    ：KDM-V5.0-R1.0 Copyright (C) 2009 - 2011 All Rights Reserved
-----------------------------------------------------------------------------
修改记录:
日  期        版本        修改人        走读人    修改内容

******************************************************************************/
#ifndef IPCSDK_H_
#define IPCSDK_H_

#include "ipcdefine.h"
#include "ipcstruct.h"

#ifdef __cplusplus
extern "C" {

#ifdef WIN32
	#define IPC_API  extern "C" __declspec(dllexport)
	#define STDCALL  __stdcall
#else
	#define IPC_API
	#define STDCALL	
#endif

#endif // __cplusplus

/*=================================================================
函数名称: IPC_Init
功    能: 初始化IPCSDK
参数说明:
    wTelnetPort     Telnet端口(默认3000)
	bOpenTelnet     是否开启Telnet(默认打开)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_Init(unsigned short wTelnetPort = 3000, int bOpenTelnet = 1);

/*=================================================================
函数名称: IPC_Cleanup
功    能: 释放SDK的相关资源
参数说明: void
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_Cleanup();

/*=================================================================
函数名称: IPC_GetVersion
功    能: 获取软件版本号
参数说明:
    pchVer [out]    软件版本号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_GetVersion(char *pchVer);

/*=================================================================
函数名称: IPC_SetSearchCB
功    能: 指定搜索回调
参数说明:
    fSearchCB       搜索结果回调处理
    pContext        回调上下文
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_SetSearchCB(IPCSDK_SEARCH_CB fSearchCB, void *pContext);

/*=================================================================
函数名称: IPC_SearchPu
功    能: 搜索前端
参数说明: void
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SearchPu();

/*=================================================================
函数名称: IPC_Connect
功    能: 连接前端
参数说明:
    dwConnectIP     前端IP
	wPort           连接端口(通常是IPCSDK_VSIP_PORT)
    pchName         用户名
    pchPwd          密码
    pErrCd          错误码
返 回 值: 成功返回大于0的handle值, 失败pErrCd中返回错误码
=================================================================*/
IPC_API unsigned long STDCALL IPC_Connect(unsigned long dwConnectIP, unsigned short wPort, const char *pchName, const char *pchPwd, int *pErrCd);

/*=================================================================
函数名称: IPC_IsConnected
功    能: 是否已经连接到前端
参数说明: 
    hHandle         前端句柄
返 回 值: TRUE-已连接, FALSE-未连接
=================================================================*/
IPC_API int STDCALL IPC_IsConnected(unsigned long hHandle);

/*=================================================================
函数名称: IPC_Disconnect
功    能: 断开与前端的连接
参数说明: 
    hHandle         前端句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_Disconnect(unsigned long hHandle);

/*=================================================================
函数名称: IPC_GetLocalConnectIP
功    能: 获取与前端进行连接的本地IP
参数说明: 
    hHandle         前端句柄
返 回 值: 本地IP(主机序)
=================================================================*/
IPC_API unsigned long STDCALL IPC_GetLocalConnectIP(unsigned long hHandle);

/*=================================================================
函数名称: IPC_SetConnectionCheck
功    能: 设置链路检测时间间隔及断链确认次数
参数说明: 
    dwCheckInterval  链路检测间隔时间(单位ms,默认30000ms)
    byDisconnectTime 断链确认次数(默认3次)
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_SetConnectionCheck(unsigned long dwCheckInterval = 30000, unsigned char byDisconnectTime = 3);

/*=================================================================
函数名称: IPC_SetMessageCallBack
功    能: 设置设备消息回调
参数说明:    
    fFuncCB         回调处理函数
    pContext        回调相关上下文
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_SetMessageCallBack(IPCSDK_MSG_ACK_CB fFuncCB, void *pContext);

/*=================================================================
函数名称: IPC_GetPuInfo
功    能: 获取前端基本信息
参数说明:    
    hHandle         前端句柄
    ptPuInfo [out]  设备信息
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_GetPuInfo(unsigned long hHandle, TIPC_DISCOVERY_CB_INFO* ptPuInfo);

/*=================================================================
函数名称: IPC_GetVEncChanRelation
功    能: 获取视频编码通道关系表参数结构
参数说明: 
    hHandle          前端句柄
    ptRelation [out] 视频编码通道关系表参数结构
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_GetVEncChanRelation(unsigned long hHandle, TIPC_VIDEO_ENC_CHAN_RELATION *ptRelation);

/*=================================================================
函数名称: IPC_RealPlay
功    能: 视频浏览
参数说明: 
    hHandle         前端句柄
    hPlayWnd        播放窗口
	pErrCd          错误码
    wVideoStartPort 起始端口(默认是IPCSDK_RTP_PORT)
    byCameraIndex   视频源通道(PLAY_MAIN为主流，PLAY_SUB为辅流)
返 回 值: 成功返回大于0的播放器句柄值，失败pErrCd中返回错误码
=================================================================*/
IPC_API unsigned long STDCALL IPC_RealPlay(unsigned long hHandle, unsigned long hPlayWnd, int *pErrCd, unsigned short wVideoStartPort = IPCSDK_RTP_PORT,
								           unsigned char byCameraIndex = PLAY_MAIN);

/*=================================================================
函数名称: IPC_PauseRealPlay
功    能: 暂停视频浏览
参数说明:
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PauseRealPlay(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_ResumeRealPlay
功    能: 恢复视频浏览
参数说明:
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_ResumeRealPlay(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_StopRealPlay
功    能: 停止视频浏览
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopRealPlay(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_IsRealPlay
功    能: 是否正在浏览视频
参数说明:
    hPlayHandle     播放器句柄
返 回 值: TRUE-正在浏览, FALSE-不在浏览状态
=================================================================*/
IPC_API int STDCALL IPC_IsRealPlay(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_ChangePlayWnd
功    能: 变更码流输出窗口
参数说明: 
    hPlayHandle     播放器句柄
    hNewPlayWnd     播放视频的窗口句柄(新窗口)
    bSaveLastFrame  视频冻结时是否需要保存最后一帧图像
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_ChangePlayWnd(unsigned long hPlayHandle, unsigned long hNewPlayWnd, int bSaveLastFrame = 1);

/*=================================================================
函数名称: IPC_SetDrawCallBack
功    能: 设置绘图解码回调
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    fDrawCB         绘图解码回调
    dwContext       回调相关上下文
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetDrawCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_DRAW_CB fDrawCB, unsigned long dwContext);

/*=================================================================
函数名称: IPC_SetFrameCallBack
功    能: 设置视频帧回调
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    fFrameCB        帧头信息回调
    dwContext       回调相关上下文
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetFrameCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_FRAME_CB fFrameCB, unsigned long dwContext); 

/*=================================================================
函数名称: IPC_SetFrameInfoCallBack
功    能: 设置YUV解码码流信息回调
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    fFrameInfoCB    YUV解码码流信息回调
    dwContext       回调相关上下文
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetFrameInfoCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_FRAMEINFO_CB fFrameInfoCB, unsigned long dwContext);

/*===============================================================
函数名称: IPC_SetVerticalSyncMode
功    能: 开启或关闭解码的垂直同步
参数说明: 
    bVerSync        是否开启垂直同步
    hPlayHandle     播放器句柄，不指定则对所有解码器进行设置
返 回 值: void
================================================================*/
IPC_API void STDCALL IPC_SetVerticalSyncMode(int bVerSync, unsigned long hPlayHandle = 0);

/*===============================================================
函数名称: IPC_SetShowMargins
功    能: 设置图像裁剪
参数说明: 
    nLeft           裁剪的左边缘宽度
	nTop            裁剪的上边缘宽度
	nRight          裁剪的右边缘宽度
	nBottom         裁剪的下边缘宽度
	bAuto           是否会随解码图象大小改变
返 回 值: void
================================================================*/
IPC_API void STDCALL IPC_SetShowMargins(int nLeft, int nTop, int nRight, int nBottom, int bAuto = 1);

/*===============================================================
函数名称: IPC_SetSmoothness
功    能: 设置视频流畅度
          (须在视频浏览之前使用，用于设置解码器初始化参数)
参数说明:
    nSmoothness     视频流畅度(默认范围1~20)
返 回 值: void
================================================================*/
IPC_API void STDCALL IPC_SetSmoothness(int nSmoothness);

/*=================================================================
函数名称: IPC_IsAudioMute
功    能: 本地监听是否静音
参数说明: 
    hPlayHandle     播放器句柄
返 回 值: TRUE-静音, FALSE-非静音
=================================================================*/
IPC_API int STDCALL IPC_IsAudioMute(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_SetMute
功    能: 本地监听设置是否静音
参数说明: 
    hPlayHandle     播放器句柄
    bMute           1-静音, 0-非静音
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetMute(unsigned long hPlayHandle, int bMute);

/*=================================================================
函数名称: IPC_SetVolume
功    能: 设置本地监听音量
参数说明: 
    hPlayHandle     播放器句柄
    byVolume        音量值 0-25(或0-10)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetVolume(unsigned long hPlayHandle, unsigned char byVolume);

/*=================================================================
函数名称: IPC_GetVolume
功    能: 获取本地监听音量
参数说明: 
    hPlayHandle     播放器句柄
    pbyVolume [out] 音量值
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_GetVolume(unsigned long hPlayHandle, unsigned char *pbyVolume);

/*=================================================================
函数名称: IPC_StartAudioCall
功    能: 开始语音呼叫
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
	wPort           本地RTCP端口
    byADecChn       音频解码通道号(从0开始)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StartAudioCall(unsigned long hHandle, unsigned long hPlayHandle, unsigned short wPort = 0, char byADecChn = 0);

/*=================================================================
函数名称: IPC_StopAudioCall
功    能: 停止语音呼叫
参数说明: 
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    byADecChn       音频解码通道号(从0开始)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopAudioCall(unsigned long hHandle, unsigned long hPlayHandle, unsigned char byADecChn = 0);

/*=================================================================
函数名称: IPC_IsAudioSending
功    能: 是否处于语音发送状态
参数说明:
    hPlayHandle     播放器句柄
返 回 值: TRUE-发送中, FALSE-没有发送
=================================================================*/
IPC_API int STDCALL IPC_IsAudioSending(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_IsCallMute
功    能: 语音呼叫是否是哑音状态
参数说明:
    hPlayHandle     播放器句柄
返 回 值: TRUE-哑音, FALSE-非哑音
=================================================================*/
IPC_API int STDCALL IPC_IsCallMute(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_SetCallMute
功    能: 设置语音呼叫哑音状态
参数说明:
    hPlayHandle     播放器句柄
    bMute           1-哑音, 0-非哑音
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_SetCallMute(unsigned long hPlayHandle, int bMute);

/*=================================================================
函数名称: IPC_SetCallVolume
功    能: 设置语音呼叫音量
参数说明:
    hPlayHandle     播放器句柄
    byVolume        音量值 0-25
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_SetCallVolume(unsigned long hPlayHandle, unsigned char byVolume);

/*=================================================================
函数名称: IPC_GetCallVolume
功    能: 获取语音呼叫音量
参数说明:
    hPlayHandle     播放器句柄
返 回 值: 返回音量值
=================================================================*/
IPC_API unsigned char STDCALL IPC_GetCallVolume(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_PTZControl
功    能: 执行摄像头控制功能
参数说明:
    hHandle         前端句柄
	byVideoSrc      视频源编号
    byMoveType      摄像头控制类型
    pcContent       数据指针
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZControl(unsigned long hHandle, unsigned char byVideoSrc, ECamControl byMoveType, void *pcContent = 0);

/*=================================================================
函数名称: IPC_PTZControlEx
功    能: 执行摄像头控制功能(扩展)
参数说明:
    hHandle         前端句柄
	byVideoSrc      视频源编号
    byCommandID     摄像头控制类型
    byParam1~4      参数1~4
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZControlEx(unsigned long hHandle, unsigned char byVideoSrc, unsigned char byCommandID, unsigned char byParam1 = 0,
									 unsigned char byParam2 = 0, unsigned char byParam3 = 0, unsigned char byParam4 = 0);

/*=================================================================
函数名称: IPC_PTZControlDirect
功    能: 执行摄像头控制功能(发送协议数据)
参数说明:
    hHandle         前端句柄
	byVideoSrc      视频源编号
    pCtrlDate       控制命令数据(协议数据)
    dwSize          数据长度
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZControlDirect(unsigned long hHandle, unsigned char byVideoSrc, void* pCtrlDate, unsigned long dwSize);

/*=================================================================
函数名称: IPC_PTZRebuildCoordinate
功    能: 球机自检
参数说明:
    hHandle         前端句柄    
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZRebuildCoordinate(unsigned long hHandle);

/*=================================================================
函数名称: IPC_PTZGetCoordinate
功    能: 获取球机当前坐标值(水平+垂直)
参数说明:
    hHandle              前端句柄
	phCoordinate [out]   水平坐标
    pvCoordinate [out]   垂直坐标     
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZGetCoordinate(unsigned long hHandle, float* phCoordinate, float* pvCoordinate);

/*=================================================================
函数名称: IPC_PTZSetCoordinate
功    能: 设置球机当前坐标值(水平+垂直)
参数说明:
    hHandle         前端句柄
	hCoordinate     水平坐标
    vCoordinate     垂直坐标    
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetCoordinate(unsigned long hHandle, float hCoordinate, float vCoordinate);

/*=================================================================
函数名称: IPC_PTZGetPanTiltZoom
功    能: 获取球机当前PTZ值(水平+垂直+放大倍数)
参数说明:
    hHandle              前端句柄
	phCoordinate  [out]  水平坐标
    pvCoordinate  [out]  垂直坐标
    pZoomPosition [out]  放大倍数 
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZGetPanTiltZoom(unsigned long hHandle, float* phCoordinate, float* pvCoordinate, unsigned int* pZoomPosition);

/*=================================================================
函数名称: IPC_PTZSetPanTiltZoom
功    能: 设置球机当前PTZ值(水平+垂直+放大倍数)
参数说明:
    hHandle         前端句柄
	hCoordinate     水平坐标
    vCoordinate     垂直坐标
    ZoomPosition    放大倍数 
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetPanTiltZoom(unsigned long hHandle, float hCoordinate, float vCoordinate, unsigned int ZoomPosition);

/*=================================================================
函数名称: IPC_PTZSetSpeed
功    能: 设置球机速度(水平+垂直方向)
参数说明:
    hHandle         前端句柄
	hDirection      水平方向 0-逆时针, 1-顺时针
    hSpeed          水平速度 分辨率为0.01度/秒
    vDirection      垂直方向 0-水平方向, 1-南极方向
	vSpeed          垂直速度 分辨率为0.01度/秒
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetSpeed(unsigned long hHandle, unsigned int hDirection, float hSpeed, unsigned int vDirection, float vSpeed);

/*=================================================================
函数名称: IPC_PTZSetFocusMode
功    能: 设置球机聚焦模式
参数说明:
    hHandle         前端句柄
	nMode           聚焦模式 0-自动, 1-手动    
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetFocusMode(unsigned long hHandle, int nMode);

/*=================================================================
函数名称: IPC_PTZGetDomeModel
功    能: 获取球机型号
参数说明:
    hHandle         前端句柄
	pchModel [out]  球机型号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZGetDomeModel(unsigned long hHandle, char *pchModel);

/*=================================================================
函数名称: IPC_PTZSetBufferOpt
功    能: 设置球机PTZ操作指令缓冲区
参数说明:
    bUseBuffer      是否启用指令缓冲区 0-不启用, 1-启用    
	dwMaxCount      缓冲区最大指令集数
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PTZSetBufferOpt(int bUseBuffer, unsigned long dwMaxCount = 0);

/*=================================================================
函数名称: IPC_GetPuCapVal
功    能: 获取设备支持对应能力的情况
参数说明:
    hHandle         前端句柄
    byType          能力类型
返 回 值: 返回大于0为支持，返回0为不支持
=================================================================*/
IPC_API unsigned char STDCALL IPC_GetPuCapVal(unsigned long hHandle, ECapability byType);

/*=================================================================
函数名称: IPC_GetPuParam
功    能: 获取配置参数
参数说明:
    hHandle         前端句柄
    wType           配置类型
    pParam [out]    配置数据指针
    dwSize          配置数据大小
	nWaitTime       等待超时时间(默认10秒)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_GetPuParam(unsigned long hHandle, unsigned short wType, void *pParam, unsigned long dwSize, int nWaitTime = 10000);

/*=================================================================
函数名称: IPC_SetPuParam
功    能: 设置配置参数
参数说明:
    hHandle         前端句柄
    wType           配置类型
    pParam          配置数据指针
    dwSize          配置数据大小
	nWaitTime       等待超时时间(默认10秒)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SetPuParam(unsigned long hHandle, unsigned short wType, void *pParam, unsigned long dwSize, int nWaitTime = 10000);

/*=================================================================
函数名称: IPC_SyncPuTime
功    能: 同步前端设备与PC主机的时间
参数说明: 
    hHandle         前端句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SyncPuTime(unsigned long hHandle);

/*=================================================================
函数名称: IPC_RebootPu
功    能: 重启前端设备
参数说明: 
    hHandle         前端句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_RebootPu(unsigned long hHandle);

/*=================================================================
函数名称: IPC_ResetFactoryDefaults
功    能: 恢复设备出厂设置
参数说明: 
    hHandle         前端句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_ResetFactoryDefaults(unsigned long hHandle);

/*=================================================================
函数名称: IPC_LoadCustomDefaultCfg
功    能: 恢复默认设置(主要用于图像复位等操作)
参数说明:
    hHandle         前端句柄
    byVideoId       视频源ID    
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LoadCustomDefaultCfg(unsigned long hHandle , unsigned char byVideoId);

/*=================================================================
函数名称: IPC_StartLocalRec
功    能: 开始本地录像
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    pchFileName     保存的文件名
    pchFilePath     保存的文件路径
	wLocalRecPort   录像端口
	fRecStatCB      录像状态回调
	pContext        用户上下文
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StartLocalRec(unsigned long hHandle, unsigned long hPlayHandle, const char *pchFileName, const char *pchFilePath,
									  unsigned short wLocalRecPort = IPCSDK_LOCAL_REC_PORT, IPCSDK_REC_STATE_CB fRecStatCB = 0, void* pContext = 0);

/*=================================================================
函数名称: IPC_IsLocalRecording
功    能: 是否处于本地录像状态
参数说明:
    hPlayHandle     播放器句柄
返 回 值: TRUE-本地录像中, FALSE-不在本地录像中
=================================================================*/
IPC_API int STDCALL IPC_IsLocalRecording(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_StopLocalRec
功    能: 停止本地录像
参数说明:
    hPlayHandle     播放器句柄
    bStopTransfer   是否停止转发码流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopLocalRec(unsigned long hPlayHandle, int bStopTransfer);

/*=================================================================
函数名称: IPC_StartPuRec
功    能: 开始前端手动录像
参数说明:
     hHandle        前端句柄
     byChannel      通道id号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StartPuRec(unsigned long hHandle, unsigned char byChannel);

/*=================================================================
函数名称: IPC_StopPuRec
功    能: 停止前端手动录像
参数说明:
     hHandle        前端句柄
     byChannel      通道id号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopPuRec(unsigned long hHandle, unsigned char byChannel);

/*=================================================================
函数名称: IPC_LocalSnapshot
功    能: 本地抓拍
参数说明:
    hPlayHandle     播放器句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)
    byEncodeMode    图片保存模式(目前只支持IPCSDK_PIC_ENC_JPEG)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalSnapshot(unsigned long hPlayHandle, const char *pchFileName, unsigned char byEncodeMode);

/*=================================================================
函数名称: IPC_PuSnapshot
功    能: 前端抓拍
参数说明:
    hHandle         前端句柄
    pchFileName     保存的图片文件全路径(文件名+文件路径)
    byVideoInId     视频源通道号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PuSnapshot(unsigned long hHandle, const char *pchFileName, unsigned char byVideoInId);

/*=================================================================
函数名称: IPC_SearchPuPic
功    能: 搜索指定的前端抓拍图片(异步方法)
          在用户自定义的设备消息回调IPCSDK_MSG_ACK_CB中
		  捕获EVENT_NVR_QUERY_PIC_RSP消息
		  对应参数结构为TIPC_QUERY_PIC_FILE_INFO
参数说明:
    hHandle         前端句柄
    ptQueryParam    图片查询结构
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SearchPuPic(unsigned long hHandle, const TIPC_QUERY_PIC_FILE_PARAM *ptQueryParam);

/*=================================================================
函数名称: IPC_DownloadPuPic
功    能: 下载前端抓拍图片(异步方法)
          在用户自定义的设备消息回调IPCSDK_MSG_ACK_CB中
          捕获EVENT_DOWNLOAD_PIC_FILE_ACK消息
		  对应参数结构为TIPC_DOWN_PIC_FILE_INFO
参数说明:
    hHandle         前端句柄
    ptPicFileInfo   图片文件信息
    pchSavePath     图片保存的全路径
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_DownloadPuPic(unsigned long hHandle, const TIPC_PIC_FILE_INFO *ptPicFileInfo, const char *pchSavePath);

/*=================================================================
函数名称: IPC_DeletePuPic
功    能: 删除指定的前端图片
参数说明:
    hHandle         前端句柄
    ptPicFileInfo   图片文件信息
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_DeletePuPic(unsigned long hHandle, const TIPC_PIC_FILE_INFO *ptPicFileInfo);

/*=================================================================
函数名称: IPC_SearchPuRec
功    能: 搜索指定条件的前端录像文件(异步方法)
          在用户自定义的设备消息回调IPCSDK_MSG_ACK_CB中
		  捕获EVENT_IPC_QUIRY_REC_FILE_RSP消息
          对应参数结构为TIPC_QUERY_REC_INFO
参数说明:
     hHandle        前端句柄
     ptQueryParam   录像文件查询结构
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_SearchPuRec(unsigned long hHandle, const TIPC_QUERY_PARAM *ptQueryParam);

/*=================================================================
函数名称: IPC_StartPlayPuRec
功    能: 开始播放前端录像
参数说明:
     hHandle        前端句柄     
	 hPlayWnd       播放窗口
     ptRecFileInfo  录像文件信息
	 pErrCd         错误码
返 回 值: 成功返回大于0的播放器句柄值, 失败pErrCd中返回错误码
=================================================================*/
IPC_API unsigned long STDCALL IPC_StartPlayPuRec(unsigned long hHandle, unsigned long hPlayWnd, const TIPC_REC_FILE_INFO_EX *ptRecFileInfo, int *pErrCd);

/*=================================================================
函数名称: IPC_IsPlayPuRec
功    能: 是否正在播放前端录像
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
返 回 值: TRUE-播放中, FALSE-没有播放
=================================================================*/
IPC_API int STDCALL IPC_IsPlayPuRec(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_StopPlayPuRec
功    能: 停止播放前端录像
参数说明:
     hHandle        前端句柄
     hPlayHandle    播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopPlayPuRec(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_DownloadPuRec
功    能: 下载前端录像文件
参数说明:
    hHandle         前端句柄
	hPlayHandle     播放器句柄
    ptRecFileInfo   录像文件信息
    pchSavePath     下载文件的全路径
	fDownRecCB      录像下载回调
    pContext        用户上下文
    bReStart        是否重新下载
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_DownloadPuRec(unsigned long hHandle, unsigned long hPlayHandle, 
									  const TIPC_REC_FILE_INFO *ptRecFileInfo, const char *pchSavePath,
									  IPCSDK_DOWN_REC_CB fDownRecCB = 0, void* pContext = 0,
									  int bReStart = 1);

/*=================================================================
函数名称: IPC_StopDownloadPuRec
功    能: 停止下载前端录像
参数说明:
    hHandle         前端句柄
	hPlayHandle     播放器句柄
    bEndWrite       是否终止写文件
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopDownloadPuRec(unsigned long hHandle, unsigned long hPlayHandle, int bEndWrite);

/*=================================================================
函数名称: IPC_LocalPlayVideo
功    能: 本地接收并播放码流
参数说明:
    hPlayWnd        播放窗口
	wVideoRTPPort   本地视频接收RTP端口
	wVideoRTCPPort  本地视频接收RTCP端口
	wAudioRTPPort   本地音频接收RTP端口
	wAudioRTCPPort  本地音频接收RTCP端口
    pErrCd          错误码    
返 回 值: 成功返回大于0的播放器句柄值，失败pErrCd中返回错误码
=================================================================*/
IPC_API unsigned long STDCALL IPC_LocalPlayVideo(unsigned long hPlayWnd, unsigned short wVideoRTPPort, unsigned short wVideoRTCPPort, 
												 unsigned short wAudioRTPPort, unsigned short wAudioRTCPPort, int *pErrCd);

/*=================================================================
函数名称: IPC_LocalStopPlay
功    能: 停止本地码流接收和播放
参数说明: 
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalStopPlay(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalOpenFile
功    能: 打开本地码流文件
参数说明:
    hPlayWnd        播放窗口 
    ptParam         本地码流文件信息
	pErrCd          错误码
返 回 值: 成功返回大于0的播放器句柄值, 失败pErrCd中返回错误码
=================================================================*/
IPC_API unsigned long STDCALL IPC_LocalOpenFile(unsigned long hPlayWnd, TIPC_PLAY_FILE_PARAM *ptParam, int *pErrCd);

/*=================================================================
函数名称: IPC_LocalCloseFile
功    能: 关闭本地码流文件
参数说明: 
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalCloseFile(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalPlayFile
功    能: 播放本地码流文件
参数说明: 
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalPlayFile(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalStopFile
功    能: 停止播放本地码流文件
参数说明: 
    hPlayHandle     播放器句柄
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalStopFile(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalPauseFile
功    能: 暂停播放本地码流文件
参数说明:
    hPlayHandle     播放器句柄 
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalPauseFile(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalResumeFile
功    能: 恢复播放本地码流文件
参数说明: 
    hPlayHandle     播放器句柄 
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalResumeFile(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalPlaySetPos
功    能: 设置播放本地码流文件的开始位置
参数说明: 
    hPlayHandle     播放器句柄
    dwSec           开始时间点
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
 =================================================================*/
IPC_API int STDCALL IPC_LocalPlaySetPos(unsigned long hPlayHandle, unsigned long dwSec);

/*=================================================================
函数名称: IPC_LocalSingleFrame
功    能: 单帧方式播放本地码流文件
          (在视频暂停的情况下有效)
参数说明: 
    hPlayHandle     播放器句柄 
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalSingleFrame(unsigned long hPlayHandle);

/*=================================================================
函数名称: IPC_LocalFastPlay
功    能: 快进方式播放本地码流文件
参数说明: 
    hPlayHandle     播放器句柄 
    rate            快放速数 2-2倍 4-4倍
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalFastPlay(unsigned long hPlayHandle, unsigned char rate);

/* =================================================================
函数名称: IPC_LocalSlowPlay
功    能: 慢进方式播放本地码流文件
参数说明:
    hPlayHandle     播放器句柄 
    rate            慢放速数 2-2倍 4-4倍 8-8倍 16-16倍
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalSlowPlay(unsigned long hPlayHandle, unsigned char rate);

/*=================================================================
函数名称: IPC_LocalGetFileInfo
功    能: 获取本地码流文件信息
参数说明: 
    hPlayHandle      播放器句柄    
    ptFileInfo [out] 本地文件信息
返 回 值: 成功 返回ERR_SUCCESS; 失败返回小于0的错误码
=================================================================*/
IPC_API int STDCALL IPC_LocalGetFileInfo(unsigned long hPlayHandle, TIPC_PLAY_FILE_INFO *ptFileInfo);

/*=================================================================
函数名称: IPC_PuRecPlayOperation
功    能: 前端录像文件回放操作
          (须在前端录像回放时使用)
参数说明:
    hHandle         前端句柄
	hPlayHandle     播放器句柄
    byType          录像回放操作(暂停、继续、快进、慢放等)
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PuRecPlayOperation(unsigned long hHandle, unsigned long hPlayHandle, EPuRecPlayType byType);

/*=================================================================
函数名称: IPC_PuRecSeekLocation
功    能: 前端录像播放拖动操作
          (须在前端录像回放时使用)
参数说明:
    hHandle         前端句柄
    hPlayHandle     播放器句柄
    dwDragTime      拖放开始时间
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_PuRecSeekLocation(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwDragTime);

/*=================================================================
函数名称: IPC_GetDecoderStatis
功    能: 获得解码信息
参数说明:
    hPlayHandle        播放器句柄
    ptDecStatics [out] 解码统计信息
返 回 值: void
=================================================================*/
IPC_API void STDCALL IPC_GetDecoderStatis(unsigned long hPlayHandle, TIPC_DEC_STATICS *ptDecStatics);

/*=================================================================
函数名称: IPC_UploadTitlePic
功    能: 上传台标文件
参数说明:
    hHandle         前端句柄
    byMsgType       台标字幕类型    
    pchFilePath     台标文件全路径(文件名+文件路径)
    byAlarmPinId    并口告警ID
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_UploadTitlePic(unsigned long hHandle, unsigned char byMsgType, const char *pchFilePath, unsigned char byAlarmPinId = 0);

/*=================================================================
函数名称: IPC_GenerateOsdBmp
功    能: 生成OSD字幕文件
参数说明:
    logfont         字幕字体
    pchOSD          字幕内容
    clrTxt          字体颜色
    clrbg           背景颜色
    pchBmpPath      文件全路径(文件名+文件路径)
返 回 值: 成功返回TRUE, 失败返回FALSE
=================================================================*/
IPC_API int STDCALL IPC_GenerateOsdBmp(const LOGFONT logfont, const char *pchOSD, const COLORREF clrTxt, const COLORREF clrbg, const char *pchBmpPath);

/*=================================================================
函数名称: IPC_StartEncToDec
功    能: 将编码器的码流发送到解码器上
参数说明:
    hHandleEnc      编码器句柄
    bySrcIdEnc      编码器视频源号
    emPlayChanType  编码器通道类型
    hHandleDec      解码器句柄
    byChanIdDec     解码器通道号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StartEncToDec(unsigned long hHandleEnc, unsigned char bySrcIdEnc, EPlayChanType emPlayChanType,
								      unsigned long hHandleDec, unsigned char byChanIdDec);

/*=================================================================
函数名称: IPC_StopEncToDec
功    能: 停止将编码器的码流发送到解码器上
参数说明:
    hHandleEnc      编码器句柄
    bySrcIdEnc      编码器视频源号
    emPlayChanType  编码器通道类型
    hHandleDec      解码器句柄
    byChanIdDec     解码器通道号
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_StopEncToDec(unsigned long hHandleEnc, unsigned char bySrcIdEnc, EPlayChanType emPlayChanType,
									 unsigned long hHandleDec, unsigned char byChanIdDec);

/*=================================================================
函数名称: IPC_AddSrcSwitch
功    能: 发送设备图像音视频码流到远端；转发只适用于编码器
参数说明:
    hHandle         前端句柄
    bySrcId         前端视频源编号
    dwDestIp        码流接收地址
    wDestPort       视频码流接收端口(音频 = 视频 + 2，默认已请求)
    emPlayChanType  主流或辅流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
          此版本最大支持添加IPCSDK_MAX_TRANFER_NUM流码流转发，大于
          IPCSDK_MAX_TRANFER_NUM路转发，则失败
=================================================================*/
IPC_API int STDCALL IPC_AddSrcSwitch(unsigned long hHandle, unsigned char bySrcId, unsigned long dwDestIp, unsigned short wDestPort, 
					                 EPlayChanType emPlayChanType);

/*=================================================================
函数名称: IPC_RemoveSrcSwitch
功    能: 发送设备图像码流到远端，当设备断链的时候，将自动删除所有
          转发码流
参数说明:
    hHandle         前端句柄
    bySrcId         前端视频源编号
    dwDestIp        码流接收地址
    wDestPort       视频码流接收端口(音频 = 视频 + 2，默认已请求)
    emPlayChanType  主流或辅流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_RemoveSrcSwitch(unsigned long hHandle, unsigned char bySrcId, unsigned long dwDestIp, unsigned short wDestPort, 
                                        EPlayChanType emPlayChanType);

/*=================================================================
函数名称: IPC_AddSrcSwitchEx
功    能: 请求设备图像码流到本地，回调给上层用户
参数说明: 
	hHandle         前端句柄
	bySrcId         前端视频源编号
    pfCB            码流回调函数
	pContext        回调上下文
    emPlayChanType  主流或辅流 PLAY_MAIN-主流, PLAY_SUB-辅流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_AddSrcSwitchEx(unsigned long hHandle, unsigned char bySrcId, IPCSDK_STREAMCB pfCB, void* pContext, EPlayChanType emPlayChanType);

/*=================================================================
函数名称: IPC_RemoveSrcSwitchEx
功    能: 删除请求设备图像码流回调
参数说明: 
	hHandle         前端句柄
	bySrcId         前端视频源编号
    emPlayChanType  主流或辅流 PLAY_MAIN: 主流; PLAY_SUB: 辅流
返 回 值: 成功返回IPC_ERR_SUCCESS, 失败返回错误码
=================================================================*/
IPC_API int STDCALL IPC_RemoveSrcSwitchEx(unsigned long hHandle, unsigned char bySrcId, EPlayChanType emPlayChanType);

/*===============================================================
函数名称: IPC_DebugLevel
功    能: 自定义调试打印级别
参数说明: 
    byLevel 0-255之间，当设置为0的时候，不打印打印信息，当设置为255的时候
	        打印所有打印信息；输出的打印信息为打印级别以下的打印信息
			例如：打印级别设置为100，输出的打印信息级别为0-100的打印
返 回 值: void
================================================================*/
IPC_API void STDCALL IPC_DebugLevel(unsigned char byLevel);

/*===============================================================
函数名称: IPC_DebugOutput
功    能: 自定义调试打印级别
参数说明: 
    byLevel    打印级别 ；0-255
	lpszFormat 与printf中的参数使用方法相同，默认在打印的信息结尾加了\n分行
返 回 值: void
================================================================*/
IPC_API void STDCALL IPC_DebugOutput( unsigned char byLevel, char* lpszFormat, ...);

/*=================================================================
函数名称: IPC_CreateTask
功    能: 新建任务
参数说明:
    pvTaskEntry     任务处理函数指针
    szName          任务名称
    byPriority      任务优先级
    dwStacksize     栈大小
	dwParam         参数(上下文)
	wFlag           任务标志	
返 回 值: 任务句柄
=================================================================*/
IPC_API unsigned long STDCALL IPC_CreateTask(void* pvTaskEntry, char* szName, unsigned char byPriority, unsigned long dwStacksize, 
											 unsigned long dwParam, unsigned short wFlag);

/*=================================================================
函数名称: IPC_TerminateTask
功    能: 终止任务
参数说明:
    dwTaskHandle    任务句柄    	
返 回 值: 成功返回TRUE, 失败返回FALSE
=================================================================*/
IPC_API int STDCALL IPC_TerminateTask(unsigned long dwTaskHandle);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //