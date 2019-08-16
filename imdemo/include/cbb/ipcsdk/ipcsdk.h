/*****************************************************************************
ģ����  : ipcsdk�������ӿ�ģ��
�ļ���  : ipcsdk.h
����ļ���ipcsdk.h
ʵ�ֹ��ܣ�
����    ��Ǯ�S��
�汾    ��KDM-V5.0-R1.0 Copyright (C) 2009 - 2011 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����

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
��������: IPC_Init
��    ��: ��ʼ��IPCSDK
����˵��:
    wTelnetPort     Telnet�˿�(Ĭ��3000)
	bOpenTelnet     �Ƿ���Telnet(Ĭ�ϴ�)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_Init(unsigned short wTelnetPort = 3000, int bOpenTelnet = 1);

/*=================================================================
��������: IPC_Cleanup
��    ��: �ͷ�SDK�������Դ
����˵��: void
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_Cleanup();

/*=================================================================
��������: IPC_GetVersion
��    ��: ��ȡ����汾��
����˵��:
    pchVer [out]    ����汾��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_GetVersion(char *pchVer);

/*=================================================================
��������: IPC_SetSearchCB
��    ��: ָ�������ص�
����˵��:
    fSearchCB       ��������ص�����
    pContext        �ص�������
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_SetSearchCB(IPCSDK_SEARCH_CB fSearchCB, void *pContext);

/*=================================================================
��������: IPC_SearchPu
��    ��: ����ǰ��
����˵��: void
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SearchPu();

/*=================================================================
��������: IPC_Connect
��    ��: ����ǰ��
����˵��:
    dwConnectIP     ǰ��IP
	wPort           ���Ӷ˿�(ͨ����IPCSDK_VSIP_PORT)
    pchName         �û���
    pchPwd          ����
    pErrCd          ������
�� �� ֵ: �ɹ����ش���0��handleֵ, ʧ��pErrCd�з��ش�����
=================================================================*/
IPC_API unsigned long STDCALL IPC_Connect(unsigned long dwConnectIP, unsigned short wPort, const char *pchName, const char *pchPwd, int *pErrCd);

/*=================================================================
��������: IPC_IsConnected
��    ��: �Ƿ��Ѿ����ӵ�ǰ��
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: TRUE-������, FALSE-δ����
=================================================================*/
IPC_API int STDCALL IPC_IsConnected(unsigned long hHandle);

/*=================================================================
��������: IPC_Disconnect
��    ��: �Ͽ���ǰ�˵�����
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_Disconnect(unsigned long hHandle);

/*=================================================================
��������: IPC_GetLocalConnectIP
��    ��: ��ȡ��ǰ�˽������ӵı���IP
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: ����IP(������)
=================================================================*/
IPC_API unsigned long STDCALL IPC_GetLocalConnectIP(unsigned long hHandle);

/*=================================================================
��������: IPC_SetConnectionCheck
��    ��: ������·���ʱ����������ȷ�ϴ���
����˵��: 
    dwCheckInterval  ��·�����ʱ��(��λms,Ĭ��30000ms)
    byDisconnectTime ����ȷ�ϴ���(Ĭ��3��)
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_SetConnectionCheck(unsigned long dwCheckInterval = 30000, unsigned char byDisconnectTime = 3);

/*=================================================================
��������: IPC_SetMessageCallBack
��    ��: �����豸��Ϣ�ص�
����˵��:    
    fFuncCB         �ص�������
    pContext        �ص����������
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_SetMessageCallBack(IPCSDK_MSG_ACK_CB fFuncCB, void *pContext);

/*=================================================================
��������: IPC_GetPuInfo
��    ��: ��ȡǰ�˻�����Ϣ
����˵��:    
    hHandle         ǰ�˾��
    ptPuInfo [out]  �豸��Ϣ
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_GetPuInfo(unsigned long hHandle, TIPC_DISCOVERY_CB_INFO* ptPuInfo);

/*=================================================================
��������: IPC_GetVEncChanRelation
��    ��: ��ȡ��Ƶ����ͨ����ϵ������ṹ
����˵��: 
    hHandle          ǰ�˾��
    ptRelation [out] ��Ƶ����ͨ����ϵ������ṹ
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_GetVEncChanRelation(unsigned long hHandle, TIPC_VIDEO_ENC_CHAN_RELATION *ptRelation);

/*=================================================================
��������: IPC_RealPlay
��    ��: ��Ƶ���
����˵��: 
    hHandle         ǰ�˾��
    hPlayWnd        ���Ŵ���
	pErrCd          ������
    wVideoStartPort ��ʼ�˿�(Ĭ����IPCSDK_RTP_PORT)
    byCameraIndex   ��ƵԴͨ��(PLAY_MAINΪ������PLAY_SUBΪ����)
�� �� ֵ: �ɹ����ش���0�Ĳ��������ֵ��ʧ��pErrCd�з��ش�����
=================================================================*/
IPC_API unsigned long STDCALL IPC_RealPlay(unsigned long hHandle, unsigned long hPlayWnd, int *pErrCd, unsigned short wVideoStartPort = IPCSDK_RTP_PORT,
								           unsigned char byCameraIndex = PLAY_MAIN);

/*=================================================================
��������: IPC_PauseRealPlay
��    ��: ��ͣ��Ƶ���
����˵��:
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PauseRealPlay(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_ResumeRealPlay
��    ��: �ָ���Ƶ���
����˵��:
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_ResumeRealPlay(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_StopRealPlay
��    ��: ֹͣ��Ƶ���
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopRealPlay(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
��������: IPC_IsRealPlay
��    ��: �Ƿ����������Ƶ
����˵��:
    hPlayHandle     ���������
�� �� ֵ: TRUE-�������, FALSE-�������״̬
=================================================================*/
IPC_API int STDCALL IPC_IsRealPlay(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_ChangePlayWnd
��    ��: ��������������
����˵��: 
    hPlayHandle     ���������
    hNewPlayWnd     ������Ƶ�Ĵ��ھ��(�´���)
    bSaveLastFrame  ��Ƶ����ʱ�Ƿ���Ҫ�������һ֡ͼ��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_ChangePlayWnd(unsigned long hPlayHandle, unsigned long hNewPlayWnd, int bSaveLastFrame = 1);

/*=================================================================
��������: IPC_SetDrawCallBack
��    ��: ���û�ͼ����ص�
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    fDrawCB         ��ͼ����ص�
    dwContext       �ص����������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetDrawCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_DRAW_CB fDrawCB, unsigned long dwContext);

/*=================================================================
��������: IPC_SetFrameCallBack
��    ��: ������Ƶ֡�ص�
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    fFrameCB        ֡ͷ��Ϣ�ص�
    dwContext       �ص����������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetFrameCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_FRAME_CB fFrameCB, unsigned long dwContext); 

/*=================================================================
��������: IPC_SetFrameInfoCallBack
��    ��: ����YUV����������Ϣ�ص�
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    fFrameInfoCB    YUV����������Ϣ�ص�
    dwContext       �ص����������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetFrameInfoCallBack(unsigned long hHandle, unsigned long hPlayHandle, IPCSDK_FRAMEINFO_CB fFrameInfoCB, unsigned long dwContext);

/*===============================================================
��������: IPC_SetVerticalSyncMode
��    ��: ������رս���Ĵ�ֱͬ��
����˵��: 
    bVerSync        �Ƿ�����ֱͬ��
    hPlayHandle     �������������ָ��������н�������������
�� �� ֵ: void
================================================================*/
IPC_API void STDCALL IPC_SetVerticalSyncMode(int bVerSync, unsigned long hPlayHandle = 0);

/*===============================================================
��������: IPC_SetShowMargins
��    ��: ����ͼ��ü�
����˵��: 
    nLeft           �ü������Ե���
	nTop            �ü����ϱ�Ե���
	nRight          �ü����ұ�Ե���
	nBottom         �ü����±�Ե���
	bAuto           �Ƿ�������ͼ���С�ı�
�� �� ֵ: void
================================================================*/
IPC_API void STDCALL IPC_SetShowMargins(int nLeft, int nTop, int nRight, int nBottom, int bAuto = 1);

/*===============================================================
��������: IPC_SetSmoothness
��    ��: ������Ƶ������
          (������Ƶ���֮ǰʹ�ã��������ý�������ʼ������)
����˵��:
    nSmoothness     ��Ƶ������(Ĭ�Ϸ�Χ1~20)
�� �� ֵ: void
================================================================*/
IPC_API void STDCALL IPC_SetSmoothness(int nSmoothness);

/*=================================================================
��������: IPC_IsAudioMute
��    ��: ���ؼ����Ƿ���
����˵��: 
    hPlayHandle     ���������
�� �� ֵ: TRUE-����, FALSE-�Ǿ���
=================================================================*/
IPC_API int STDCALL IPC_IsAudioMute(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_SetMute
��    ��: ���ؼ��������Ƿ���
����˵��: 
    hPlayHandle     ���������
    bMute           1-����, 0-�Ǿ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetMute(unsigned long hPlayHandle, int bMute);

/*=================================================================
��������: IPC_SetVolume
��    ��: ���ñ��ؼ�������
����˵��: 
    hPlayHandle     ���������
    byVolume        ����ֵ 0-25(��0-10)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetVolume(unsigned long hPlayHandle, unsigned char byVolume);

/*=================================================================
��������: IPC_GetVolume
��    ��: ��ȡ���ؼ�������
����˵��: 
    hPlayHandle     ���������
    pbyVolume [out] ����ֵ
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_GetVolume(unsigned long hPlayHandle, unsigned char *pbyVolume);

/*=================================================================
��������: IPC_StartAudioCall
��    ��: ��ʼ��������
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
	wPort           ����RTCP�˿�
    byADecChn       ��Ƶ����ͨ����(��0��ʼ)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StartAudioCall(unsigned long hHandle, unsigned long hPlayHandle, unsigned short wPort = 0, char byADecChn = 0);

/*=================================================================
��������: IPC_StopAudioCall
��    ��: ֹͣ��������
����˵��: 
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    byADecChn       ��Ƶ����ͨ����(��0��ʼ)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopAudioCall(unsigned long hHandle, unsigned long hPlayHandle, unsigned char byADecChn = 0);

/*=================================================================
��������: IPC_IsAudioSending
��    ��: �Ƿ�����������״̬
����˵��:
    hPlayHandle     ���������
�� �� ֵ: TRUE-������, FALSE-û�з���
=================================================================*/
IPC_API int STDCALL IPC_IsAudioSending(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_IsCallMute
��    ��: ���������Ƿ�������״̬
����˵��:
    hPlayHandle     ���������
�� �� ֵ: TRUE-����, FALSE-������
=================================================================*/
IPC_API int STDCALL IPC_IsCallMute(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_SetCallMute
��    ��: ����������������״̬
����˵��:
    hPlayHandle     ���������
    bMute           1-����, 0-������
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_SetCallMute(unsigned long hPlayHandle, int bMute);

/*=================================================================
��������: IPC_SetCallVolume
��    ��: ����������������
����˵��:
    hPlayHandle     ���������
    byVolume        ����ֵ 0-25
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_SetCallVolume(unsigned long hPlayHandle, unsigned char byVolume);

/*=================================================================
��������: IPC_GetCallVolume
��    ��: ��ȡ������������
����˵��:
    hPlayHandle     ���������
�� �� ֵ: ��������ֵ
=================================================================*/
IPC_API unsigned char STDCALL IPC_GetCallVolume(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_PTZControl
��    ��: ִ������ͷ���ƹ���
����˵��:
    hHandle         ǰ�˾��
	byVideoSrc      ��ƵԴ���
    byMoveType      ����ͷ��������
    pcContent       ����ָ��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZControl(unsigned long hHandle, unsigned char byVideoSrc, ECamControl byMoveType, void *pcContent = 0);

/*=================================================================
��������: IPC_PTZControlEx
��    ��: ִ������ͷ���ƹ���(��չ)
����˵��:
    hHandle         ǰ�˾��
	byVideoSrc      ��ƵԴ���
    byCommandID     ����ͷ��������
    byParam1~4      ����1~4
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZControlEx(unsigned long hHandle, unsigned char byVideoSrc, unsigned char byCommandID, unsigned char byParam1 = 0,
									 unsigned char byParam2 = 0, unsigned char byParam3 = 0, unsigned char byParam4 = 0);

/*=================================================================
��������: IPC_PTZControlDirect
��    ��: ִ������ͷ���ƹ���(����Э������)
����˵��:
    hHandle         ǰ�˾��
	byVideoSrc      ��ƵԴ���
    pCtrlDate       ������������(Э������)
    dwSize          ���ݳ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZControlDirect(unsigned long hHandle, unsigned char byVideoSrc, void* pCtrlDate, unsigned long dwSize);

/*=================================================================
��������: IPC_PTZRebuildCoordinate
��    ��: ����Լ�
����˵��:
    hHandle         ǰ�˾��    
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZRebuildCoordinate(unsigned long hHandle);

/*=================================================================
��������: IPC_PTZGetCoordinate
��    ��: ��ȡ�����ǰ����ֵ(ˮƽ+��ֱ)
����˵��:
    hHandle              ǰ�˾��
	phCoordinate [out]   ˮƽ����
    pvCoordinate [out]   ��ֱ����     
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZGetCoordinate(unsigned long hHandle, float* phCoordinate, float* pvCoordinate);

/*=================================================================
��������: IPC_PTZSetCoordinate
��    ��: ���������ǰ����ֵ(ˮƽ+��ֱ)
����˵��:
    hHandle         ǰ�˾��
	hCoordinate     ˮƽ����
    vCoordinate     ��ֱ����    
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetCoordinate(unsigned long hHandle, float hCoordinate, float vCoordinate);

/*=================================================================
��������: IPC_PTZGetPanTiltZoom
��    ��: ��ȡ�����ǰPTZֵ(ˮƽ+��ֱ+�Ŵ���)
����˵��:
    hHandle              ǰ�˾��
	phCoordinate  [out]  ˮƽ����
    pvCoordinate  [out]  ��ֱ����
    pZoomPosition [out]  �Ŵ��� 
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZGetPanTiltZoom(unsigned long hHandle, float* phCoordinate, float* pvCoordinate, unsigned int* pZoomPosition);

/*=================================================================
��������: IPC_PTZSetPanTiltZoom
��    ��: ���������ǰPTZֵ(ˮƽ+��ֱ+�Ŵ���)
����˵��:
    hHandle         ǰ�˾��
	hCoordinate     ˮƽ����
    vCoordinate     ��ֱ����
    ZoomPosition    �Ŵ��� 
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetPanTiltZoom(unsigned long hHandle, float hCoordinate, float vCoordinate, unsigned int ZoomPosition);

/*=================================================================
��������: IPC_PTZSetSpeed
��    ��: ��������ٶ�(ˮƽ+��ֱ����)
����˵��:
    hHandle         ǰ�˾��
	hDirection      ˮƽ���� 0-��ʱ��, 1-˳ʱ��
    hSpeed          ˮƽ�ٶ� �ֱ���Ϊ0.01��/��
    vDirection      ��ֱ���� 0-ˮƽ����, 1-�ϼ�����
	vSpeed          ��ֱ�ٶ� �ֱ���Ϊ0.01��/��
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetSpeed(unsigned long hHandle, unsigned int hDirection, float hSpeed, unsigned int vDirection, float vSpeed);

/*=================================================================
��������: IPC_PTZSetFocusMode
��    ��: ��������۽�ģʽ
����˵��:
    hHandle         ǰ�˾��
	nMode           �۽�ģʽ 0-�Զ�, 1-�ֶ�    
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_PTZSetFocusMode(unsigned long hHandle, int nMode);

/*=================================================================
��������: IPC_PTZGetDomeModel
��    ��: ��ȡ����ͺ�
����˵��:
    hHandle         ǰ�˾��
	pchModel [out]  ����ͺ�
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZGetDomeModel(unsigned long hHandle, char *pchModel);

/*=================================================================
��������: IPC_PTZSetBufferOpt
��    ��: �������PTZ����ָ�����
����˵��:
    bUseBuffer      �Ƿ�����ָ����� 0-������, 1-����    
	dwMaxCount      ���������ָ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PTZSetBufferOpt(int bUseBuffer, unsigned long dwMaxCount = 0);

/*=================================================================
��������: IPC_GetPuCapVal
��    ��: ��ȡ�豸֧�ֶ�Ӧ���������
����˵��:
    hHandle         ǰ�˾��
    byType          ��������
�� �� ֵ: ���ش���0Ϊ֧�֣�����0Ϊ��֧��
=================================================================*/
IPC_API unsigned char STDCALL IPC_GetPuCapVal(unsigned long hHandle, ECapability byType);

/*=================================================================
��������: IPC_GetPuParam
��    ��: ��ȡ���ò���
����˵��:
    hHandle         ǰ�˾��
    wType           ��������
    pParam [out]    ��������ָ��
    dwSize          �������ݴ�С
	nWaitTime       �ȴ���ʱʱ��(Ĭ��10��)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_GetPuParam(unsigned long hHandle, unsigned short wType, void *pParam, unsigned long dwSize, int nWaitTime = 10000);

/*=================================================================
��������: IPC_SetPuParam
��    ��: �������ò���
����˵��:
    hHandle         ǰ�˾��
    wType           ��������
    pParam          ��������ָ��
    dwSize          �������ݴ�С
	nWaitTime       �ȴ���ʱʱ��(Ĭ��10��)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SetPuParam(unsigned long hHandle, unsigned short wType, void *pParam, unsigned long dwSize, int nWaitTime = 10000);

/*=================================================================
��������: IPC_SyncPuTime
��    ��: ͬ��ǰ���豸��PC������ʱ��
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SyncPuTime(unsigned long hHandle);

/*=================================================================
��������: IPC_RebootPu
��    ��: ����ǰ���豸
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_RebootPu(unsigned long hHandle);

/*=================================================================
��������: IPC_ResetFactoryDefaults
��    ��: �ָ��豸��������
����˵��: 
    hHandle         ǰ�˾��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_ResetFactoryDefaults(unsigned long hHandle);

/*=================================================================
��������: IPC_LoadCustomDefaultCfg
��    ��: �ָ�Ĭ������(��Ҫ����ͼ��λ�Ȳ���)
����˵��:
    hHandle         ǰ�˾��
    byVideoId       ��ƵԴID    
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LoadCustomDefaultCfg(unsigned long hHandle , unsigned char byVideoId);

/*=================================================================
��������: IPC_StartLocalRec
��    ��: ��ʼ����¼��
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    pchFileName     ������ļ���
    pchFilePath     ������ļ�·��
	wLocalRecPort   ¼��˿�
	fRecStatCB      ¼��״̬�ص�
	pContext        �û�������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StartLocalRec(unsigned long hHandle, unsigned long hPlayHandle, const char *pchFileName, const char *pchFilePath,
									  unsigned short wLocalRecPort = IPCSDK_LOCAL_REC_PORT, IPCSDK_REC_STATE_CB fRecStatCB = 0, void* pContext = 0);

/*=================================================================
��������: IPC_IsLocalRecording
��    ��: �Ƿ��ڱ���¼��״̬
����˵��:
    hPlayHandle     ���������
�� �� ֵ: TRUE-����¼����, FALSE-���ڱ���¼����
=================================================================*/
IPC_API int STDCALL IPC_IsLocalRecording(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_StopLocalRec
��    ��: ֹͣ����¼��
����˵��:
    hPlayHandle     ���������
    bStopTransfer   �Ƿ�ֹͣת������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopLocalRec(unsigned long hPlayHandle, int bStopTransfer);

/*=================================================================
��������: IPC_StartPuRec
��    ��: ��ʼǰ���ֶ�¼��
����˵��:
     hHandle        ǰ�˾��
     byChannel      ͨ��id��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StartPuRec(unsigned long hHandle, unsigned char byChannel);

/*=================================================================
��������: IPC_StopPuRec
��    ��: ֹͣǰ���ֶ�¼��
����˵��:
     hHandle        ǰ�˾��
     byChannel      ͨ��id��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopPuRec(unsigned long hHandle, unsigned char byChannel);

/*=================================================================
��������: IPC_LocalSnapshot
��    ��: ����ץ��
����˵��:
    hPlayHandle     ���������
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)
    byEncodeMode    ͼƬ����ģʽ(Ŀǰֻ֧��IPCSDK_PIC_ENC_JPEG)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalSnapshot(unsigned long hPlayHandle, const char *pchFileName, unsigned char byEncodeMode);

/*=================================================================
��������: IPC_PuSnapshot
��    ��: ǰ��ץ��
����˵��:
    hHandle         ǰ�˾��
    pchFileName     �����ͼƬ�ļ�ȫ·��(�ļ���+�ļ�·��)
    byVideoInId     ��ƵԴͨ����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PuSnapshot(unsigned long hHandle, const char *pchFileName, unsigned char byVideoInId);

/*=================================================================
��������: IPC_SearchPuPic
��    ��: ����ָ����ǰ��ץ��ͼƬ(�첽����)
          ���û��Զ�����豸��Ϣ�ص�IPCSDK_MSG_ACK_CB��
		  ����EVENT_NVR_QUERY_PIC_RSP��Ϣ
		  ��Ӧ�����ṹΪTIPC_QUERY_PIC_FILE_INFO
����˵��:
    hHandle         ǰ�˾��
    ptQueryParam    ͼƬ��ѯ�ṹ
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SearchPuPic(unsigned long hHandle, const TIPC_QUERY_PIC_FILE_PARAM *ptQueryParam);

/*=================================================================
��������: IPC_DownloadPuPic
��    ��: ����ǰ��ץ��ͼƬ(�첽����)
          ���û��Զ�����豸��Ϣ�ص�IPCSDK_MSG_ACK_CB��
          ����EVENT_DOWNLOAD_PIC_FILE_ACK��Ϣ
		  ��Ӧ�����ṹΪTIPC_DOWN_PIC_FILE_INFO
����˵��:
    hHandle         ǰ�˾��
    ptPicFileInfo   ͼƬ�ļ���Ϣ
    pchSavePath     ͼƬ�����ȫ·��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_DownloadPuPic(unsigned long hHandle, const TIPC_PIC_FILE_INFO *ptPicFileInfo, const char *pchSavePath);

/*=================================================================
��������: IPC_DeletePuPic
��    ��: ɾ��ָ����ǰ��ͼƬ
����˵��:
    hHandle         ǰ�˾��
    ptPicFileInfo   ͼƬ�ļ���Ϣ
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_DeletePuPic(unsigned long hHandle, const TIPC_PIC_FILE_INFO *ptPicFileInfo);

/*=================================================================
��������: IPC_SearchPuRec
��    ��: ����ָ��������ǰ��¼���ļ�(�첽����)
          ���û��Զ�����豸��Ϣ�ص�IPCSDK_MSG_ACK_CB��
		  ����EVENT_IPC_QUIRY_REC_FILE_RSP��Ϣ
          ��Ӧ�����ṹΪTIPC_QUERY_REC_INFO
����˵��:
     hHandle        ǰ�˾��
     ptQueryParam   ¼���ļ���ѯ�ṹ
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_SearchPuRec(unsigned long hHandle, const TIPC_QUERY_PARAM *ptQueryParam);

/*=================================================================
��������: IPC_StartPlayPuRec
��    ��: ��ʼ����ǰ��¼��
����˵��:
     hHandle        ǰ�˾��     
	 hPlayWnd       ���Ŵ���
     ptRecFileInfo  ¼���ļ���Ϣ
	 pErrCd         ������
�� �� ֵ: �ɹ����ش���0�Ĳ��������ֵ, ʧ��pErrCd�з��ش�����
=================================================================*/
IPC_API unsigned long STDCALL IPC_StartPlayPuRec(unsigned long hHandle, unsigned long hPlayWnd, const TIPC_REC_FILE_INFO_EX *ptRecFileInfo, int *pErrCd);

/*=================================================================
��������: IPC_IsPlayPuRec
��    ��: �Ƿ����ڲ���ǰ��¼��
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
�� �� ֵ: TRUE-������, FALSE-û�в���
=================================================================*/
IPC_API int STDCALL IPC_IsPlayPuRec(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
��������: IPC_StopPlayPuRec
��    ��: ֹͣ����ǰ��¼��
����˵��:
     hHandle        ǰ�˾��
     hPlayHandle    ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopPlayPuRec(unsigned long hHandle, unsigned long hPlayHandle);

/*=================================================================
��������: IPC_DownloadPuRec
��    ��: ����ǰ��¼���ļ�
����˵��:
    hHandle         ǰ�˾��
	hPlayHandle     ���������
    ptRecFileInfo   ¼���ļ���Ϣ
    pchSavePath     �����ļ���ȫ·��
	fDownRecCB      ¼�����ػص�
    pContext        �û�������
    bReStart        �Ƿ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_DownloadPuRec(unsigned long hHandle, unsigned long hPlayHandle, 
									  const TIPC_REC_FILE_INFO *ptRecFileInfo, const char *pchSavePath,
									  IPCSDK_DOWN_REC_CB fDownRecCB = 0, void* pContext = 0,
									  int bReStart = 1);

/*=================================================================
��������: IPC_StopDownloadPuRec
��    ��: ֹͣ����ǰ��¼��
����˵��:
    hHandle         ǰ�˾��
	hPlayHandle     ���������
    bEndWrite       �Ƿ���ֹд�ļ�
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopDownloadPuRec(unsigned long hHandle, unsigned long hPlayHandle, int bEndWrite);

/*=================================================================
��������: IPC_LocalPlayVideo
��    ��: ���ؽ��ղ���������
����˵��:
    hPlayWnd        ���Ŵ���
	wVideoRTPPort   ������Ƶ����RTP�˿�
	wVideoRTCPPort  ������Ƶ����RTCP�˿�
	wAudioRTPPort   ������Ƶ����RTP�˿�
	wAudioRTCPPort  ������Ƶ����RTCP�˿�
    pErrCd          ������    
�� �� ֵ: �ɹ����ش���0�Ĳ��������ֵ��ʧ��pErrCd�з��ش�����
=================================================================*/
IPC_API unsigned long STDCALL IPC_LocalPlayVideo(unsigned long hPlayWnd, unsigned short wVideoRTPPort, unsigned short wVideoRTCPPort, 
												 unsigned short wAudioRTPPort, unsigned short wAudioRTCPPort, int *pErrCd);

/*=================================================================
��������: IPC_LocalStopPlay
��    ��: ֹͣ�����������պͲ���
����˵��: 
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalStopPlay(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalOpenFile
��    ��: �򿪱��������ļ�
����˵��:
    hPlayWnd        ���Ŵ��� 
    ptParam         ���������ļ���Ϣ
	pErrCd          ������
�� �� ֵ: �ɹ����ش���0�Ĳ��������ֵ, ʧ��pErrCd�з��ش�����
=================================================================*/
IPC_API unsigned long STDCALL IPC_LocalOpenFile(unsigned long hPlayWnd, TIPC_PLAY_FILE_PARAM *ptParam, int *pErrCd);

/*=================================================================
��������: IPC_LocalCloseFile
��    ��: �رձ��������ļ�
����˵��: 
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalCloseFile(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalPlayFile
��    ��: ���ű��������ļ�
����˵��: 
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalPlayFile(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalStopFile
��    ��: ֹͣ���ű��������ļ�
����˵��: 
    hPlayHandle     ���������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalStopFile(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalPauseFile
��    ��: ��ͣ���ű��������ļ�
����˵��:
    hPlayHandle     ��������� 
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalPauseFile(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalResumeFile
��    ��: �ָ����ű��������ļ�
����˵��: 
    hPlayHandle     ��������� 
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalResumeFile(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalPlaySetPos
��    ��: ���ò��ű��������ļ��Ŀ�ʼλ��
����˵��: 
    hPlayHandle     ���������
    dwSec           ��ʼʱ���
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
 =================================================================*/
IPC_API int STDCALL IPC_LocalPlaySetPos(unsigned long hPlayHandle, unsigned long dwSec);

/*=================================================================
��������: IPC_LocalSingleFrame
��    ��: ��֡��ʽ���ű��������ļ�
          (����Ƶ��ͣ���������Ч)
����˵��: 
    hPlayHandle     ��������� 
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalSingleFrame(unsigned long hPlayHandle);

/*=================================================================
��������: IPC_LocalFastPlay
��    ��: �����ʽ���ű��������ļ�
����˵��: 
    hPlayHandle     ��������� 
    rate            ������� 2-2�� 4-4��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalFastPlay(unsigned long hPlayHandle, unsigned char rate);

/* =================================================================
��������: IPC_LocalSlowPlay
��    ��: ������ʽ���ű��������ļ�
����˵��:
    hPlayHandle     ��������� 
    rate            �������� 2-2�� 4-4�� 8-8�� 16-16��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_LocalSlowPlay(unsigned long hPlayHandle, unsigned char rate);

/*=================================================================
��������: IPC_LocalGetFileInfo
��    ��: ��ȡ���������ļ���Ϣ
����˵��: 
    hPlayHandle      ���������    
    ptFileInfo [out] �����ļ���Ϣ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
IPC_API int STDCALL IPC_LocalGetFileInfo(unsigned long hPlayHandle, TIPC_PLAY_FILE_INFO *ptFileInfo);

/*=================================================================
��������: IPC_PuRecPlayOperation
��    ��: ǰ��¼���ļ��طŲ���
          (����ǰ��¼��ط�ʱʹ��)
����˵��:
    hHandle         ǰ�˾��
	hPlayHandle     ���������
    byType          ¼��طŲ���(��ͣ����������������ŵ�)
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PuRecPlayOperation(unsigned long hHandle, unsigned long hPlayHandle, EPuRecPlayType byType);

/*=================================================================
��������: IPC_PuRecSeekLocation
��    ��: ǰ��¼�񲥷��϶�����
          (����ǰ��¼��ط�ʱʹ��)
����˵��:
    hHandle         ǰ�˾��
    hPlayHandle     ���������
    dwDragTime      �Ϸſ�ʼʱ��
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_PuRecSeekLocation(unsigned long hHandle, unsigned long hPlayHandle, unsigned long dwDragTime);

/*=================================================================
��������: IPC_GetDecoderStatis
��    ��: ��ý�����Ϣ
����˵��:
    hPlayHandle        ���������
    ptDecStatics [out] ����ͳ����Ϣ
�� �� ֵ: void
=================================================================*/
IPC_API void STDCALL IPC_GetDecoderStatis(unsigned long hPlayHandle, TIPC_DEC_STATICS *ptDecStatics);

/*=================================================================
��������: IPC_UploadTitlePic
��    ��: �ϴ�̨���ļ�
����˵��:
    hHandle         ǰ�˾��
    byMsgType       ̨����Ļ����    
    pchFilePath     ̨���ļ�ȫ·��(�ļ���+�ļ�·��)
    byAlarmPinId    ���ڸ澯ID
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_UploadTitlePic(unsigned long hHandle, unsigned char byMsgType, const char *pchFilePath, unsigned char byAlarmPinId = 0);

/*=================================================================
��������: IPC_GenerateOsdBmp
��    ��: ����OSD��Ļ�ļ�
����˵��:
    logfont         ��Ļ����
    pchOSD          ��Ļ����
    clrTxt          ������ɫ
    clrbg           ������ɫ
    pchBmpPath      �ļ�ȫ·��(�ļ���+�ļ�·��)
�� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
=================================================================*/
IPC_API int STDCALL IPC_GenerateOsdBmp(const LOGFONT logfont, const char *pchOSD, const COLORREF clrTxt, const COLORREF clrbg, const char *pchBmpPath);

/*=================================================================
��������: IPC_StartEncToDec
��    ��: �����������������͵���������
����˵��:
    hHandleEnc      ���������
    bySrcIdEnc      ��������ƵԴ��
    emPlayChanType  ������ͨ������
    hHandleDec      ���������
    byChanIdDec     ������ͨ����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StartEncToDec(unsigned long hHandleEnc, unsigned char bySrcIdEnc, EPlayChanType emPlayChanType,
								      unsigned long hHandleDec, unsigned char byChanIdDec);

/*=================================================================
��������: IPC_StopEncToDec
��    ��: ֹͣ�����������������͵���������
����˵��:
    hHandleEnc      ���������
    bySrcIdEnc      ��������ƵԴ��
    emPlayChanType  ������ͨ������
    hHandleDec      ���������
    byChanIdDec     ������ͨ����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_StopEncToDec(unsigned long hHandleEnc, unsigned char bySrcIdEnc, EPlayChanType emPlayChanType,
									 unsigned long hHandleDec, unsigned char byChanIdDec);

/*=================================================================
��������: IPC_AddSrcSwitch
��    ��: �����豸ͼ������Ƶ������Զ�ˣ�ת��ֻ�����ڱ�����
����˵��:
    hHandle         ǰ�˾��
    bySrcId         ǰ����ƵԴ���
    dwDestIp        �������յ�ַ
    wDestPort       ��Ƶ�������ն˿�(��Ƶ = ��Ƶ + 2��Ĭ��������)
    emPlayChanType  ��������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
          �˰汾���֧�����IPCSDK_MAX_TRANFER_NUM������ת��������
          IPCSDK_MAX_TRANFER_NUM·ת������ʧ��
=================================================================*/
IPC_API int STDCALL IPC_AddSrcSwitch(unsigned long hHandle, unsigned char bySrcId, unsigned long dwDestIp, unsigned short wDestPort, 
					                 EPlayChanType emPlayChanType);

/*=================================================================
��������: IPC_RemoveSrcSwitch
��    ��: �����豸ͼ��������Զ�ˣ����豸������ʱ�򣬽��Զ�ɾ������
          ת������
����˵��:
    hHandle         ǰ�˾��
    bySrcId         ǰ����ƵԴ���
    dwDestIp        �������յ�ַ
    wDestPort       ��Ƶ�������ն˿�(��Ƶ = ��Ƶ + 2��Ĭ��������)
    emPlayChanType  ��������
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_RemoveSrcSwitch(unsigned long hHandle, unsigned char bySrcId, unsigned long dwDestIp, unsigned short wDestPort, 
                                        EPlayChanType emPlayChanType);

/*=================================================================
��������: IPC_AddSrcSwitchEx
��    ��: �����豸ͼ�����������أ��ص����ϲ��û�
����˵��: 
	hHandle         ǰ�˾��
	bySrcId         ǰ����ƵԴ���
    pfCB            �����ص�����
	pContext        �ص�������
    emPlayChanType  �������� PLAY_MAIN-����, PLAY_SUB-����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_AddSrcSwitchEx(unsigned long hHandle, unsigned char bySrcId, IPCSDK_STREAMCB pfCB, void* pContext, EPlayChanType emPlayChanType);

/*=================================================================
��������: IPC_RemoveSrcSwitchEx
��    ��: ɾ�������豸ͼ�������ص�
����˵��: 
	hHandle         ǰ�˾��
	bySrcId         ǰ����ƵԴ���
    emPlayChanType  �������� PLAY_MAIN: ����; PLAY_SUB: ����
�� �� ֵ: �ɹ�����IPC_ERR_SUCCESS, ʧ�ܷ��ش�����
=================================================================*/
IPC_API int STDCALL IPC_RemoveSrcSwitchEx(unsigned long hHandle, unsigned char bySrcId, EPlayChanType emPlayChanType);

/*===============================================================
��������: IPC_DebugLevel
��    ��: �Զ�����Դ�ӡ����
����˵��: 
    byLevel 0-255֮�䣬������Ϊ0��ʱ�򣬲���ӡ��ӡ��Ϣ��������Ϊ255��ʱ��
	        ��ӡ���д�ӡ��Ϣ������Ĵ�ӡ��ϢΪ��ӡ�������µĴ�ӡ��Ϣ
			���磺��ӡ��������Ϊ100������Ĵ�ӡ��Ϣ����Ϊ0-100�Ĵ�ӡ
�� �� ֵ: void
================================================================*/
IPC_API void STDCALL IPC_DebugLevel(unsigned char byLevel);

/*===============================================================
��������: IPC_DebugOutput
��    ��: �Զ�����Դ�ӡ����
����˵��: 
    byLevel    ��ӡ���� ��0-255
	lpszFormat ��printf�еĲ���ʹ�÷�����ͬ��Ĭ���ڴ�ӡ����Ϣ��β����\n����
�� �� ֵ: void
================================================================*/
IPC_API void STDCALL IPC_DebugOutput( unsigned char byLevel, char* lpszFormat, ...);

/*=================================================================
��������: IPC_CreateTask
��    ��: �½�����
����˵��:
    pvTaskEntry     ��������ָ��
    szName          ��������
    byPriority      �������ȼ�
    dwStacksize     ջ��С
	dwParam         ����(������)
	wFlag           �����־	
�� �� ֵ: ������
=================================================================*/
IPC_API unsigned long STDCALL IPC_CreateTask(void* pvTaskEntry, char* szName, unsigned char byPriority, unsigned long dwStacksize, 
											 unsigned long dwParam, unsigned short wFlag);

/*=================================================================
��������: IPC_TerminateTask
��    ��: ��ֹ����
����˵��:
    dwTaskHandle    ������    	
�� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
=================================================================*/
IPC_API int STDCALL IPC_TerminateTask(unsigned long dwTaskHandle);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //