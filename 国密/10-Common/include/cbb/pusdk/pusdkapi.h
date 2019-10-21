/*****************************************************************************
ģ����  : puǰ�˶��ο������ӿ�ģ��
�ļ���  : pusdkapi.h
����ļ���pusdkapi.h
ʵ�ֹ��ܣ�����
����    ������
�汾    ��KDM-V5.0-R1.0 Copyright (C) 2009 - 2010 All Rights Reserved
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��        �汾        �޸���        �߶���    �޸�����

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
��������: KDM_InitSearch
��    ��: ����ǰ׼��
����˵��: 
    dwLocalIp    ����IP��ַ���û�����ָ������IP��ַ������Ǹò�����0��
	             ���ɵײ�ָ����ǰĬ�ϵ�������ַ��һ������¸ò�������Ϊ0
    wSearchPort  ���������˿ڣ�Ĭ������ֵ:PUSDK_VSIP_PORT
    dwMultiAddr  �鲥������ַ ��Ĭ������ֵ:PUSDK_DEFAULT_MUL_ADDR
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_InitSearch( unsigned long dwLocalIp ,unsigned short  wSearchPort,unsigned long dwMultiAddr);

/*=================================================================
��������: KDM_AddSearchIp
��    ��: ���������ַ�Ͷ˿�
����˵��: 
    dwIp   ������ַ
    wPort  �����˿�
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_AddSearchIp(unsigned long dwIp, unsigned short wPort);

/*=================================================================
��������: KDM_AddSearchIp
��    ��: ɾ��������ַ�Ͷ˿�
����˵��: 
    dwIp   ������ַ
    wPort  �����˿�
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DelSearchIp(unsigned long dwIp, unsigned short wPort);

/*=================================================================
��������: KDM_SearchPu
��    ��: ����ǰ��
����˵��: 
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SearchPu();

/*=================================================================
��������: KDM_SetSearchCB
��    ��: ָ�������ص�
����˵��: 
    fCBMsg   ��������ص�����
    pContext �ص�������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetSearchCB( fPuSearch fCBMsg, void* pContext );

/*=================================================================
��������: KDM_GetAllPuNum
��    ��: ��ȡ�����豸����
����˵��: 
�� �� ֵ: �ɹ� ���ظ���; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetAllPuNum();

/*=================================================================
��������: KDM_GetAllPuList
��    ��: ��ȡ�����豸��Ϣ
����˵��: 
    ptDevInfo �豸��Ϣ�����׵�ַ
    nNum      ����Ԫ�ظ���
�� �� ֵ: ʵ�ʻ�õ��豸��
=================================================================*/
KDM_API int STDCALL KDM_GetAllPuList( TPuUnitInfo *ptDevInfo, int nNum );

/*=================================================================
��������: KDM_SetPuIp
��    ��: �������޸�IP
����˵��: 
    ptSetIpInfo �����ṹ
�� �� ֵ: int   �ɹ� ERR_SUCCESS �� >0 ʧ�� ERR_INVALID��ERR_SIZE ...
=================================================================*/
KDM_API int STDCALL KDM_SetPuIp( TPuSetIpInfo *ptSetIpInfo );

/*=================================================================
��������: KDM_ReleaseSearch
��    ��: ���������ͷ�
����˵��: 
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ReleaseSearch();

/*=================================================================
��������: KDM_SetMsgCB
��    ��: ���ûص�����ӿ�
����˵��: 
    fEvMsg     �ص��ӿں��������룩
    dwContext  �ص������ģ����룩
�� �� ֵ: void 
=================================================================*/
KDM_API void STDCALL KDM_SetMsgCB(FUNC_MSG_ACK_CB fEvMsg, void* pContext );

/*=================================================================
��������: KDM_InitPu
��    ��: �����豸ǰ��ʼ��
����˵��: 
    phHandle ���������������豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_InitPu( OUT LPPUHANDLE phHandle );

/*=================================================================
��������: KDM_InitDecParam
��    ��: ���ñ��ؽ������
����˵��: 
    wVidPlayBuf ��Ƶ���Ż����� 12-100
    wVidPlayBuf ��Ƶ���Ż����� 8-100
    wVidPlayBuf ������ʱ 0-30000 ����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_InitDecParam( unsigned short wVidPlayBuf, unsigned short wAudPlayBuf, unsigned short wRejectTime );

/*=================================================================
��������: KDM_ReleasePu
��    ��: �ر��豸���ӣ��ͷ������豸�������Դ��������������
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ReleasePu( PUHANDLE hHandle );

/*=================================================================
��������: KDM_ConnectPU
��    ��: ����ǰ��,�ʻ���������NULL��ʱ��ǰ�˲����ʻ����������֤��ֱ�ӵ�½
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    pUserName  �û������Լ�Ȩ��ʽ����ǰ�����ṩ���������
    pPassword  ���룬�Լ�Ȩ��ʽ����ǰ�����ṩ���������
    dwIpAddr   ǰ�˵�ַ
    wPort      ǰ�����Ӷ˿ڣ�Ĭ������ֵ:PUSDK_VSIP_PORT
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ConnectPU( PUHANDLE hHandle, const char* pUserName, const char* pPassword, 
                                        unsigned long dwIpAddr, unsigned short wPort );

/*=================================================================
��������: KDM_DisConnectPU
��    ��: �Ͽ�ǰ������
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DisConnectPU( PUHANDLE hHandle );

/*=================================================================
��������: KDM_SetProxy
��    ��: ���ô������
����˵��: 
    hHandle     �豸������豸Ψһ��ʶ��
    dwProxyIp   �����ַ
    wProxyPort  ����˿�
    strUserName �����û���
    wProxyPort  �����û�����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetProxy( PUHANDLE hHandle, unsigned long dwProxyIp, unsigned short wProxyPort,
                                       const char* pUserName, const char* pPassword );

/*=================================================================
��������: KDM_IamInNat
��    ��: ������NAT����NAT̽���
����˵��: 
    hHandle        �豸������豸Ψһ��ʶ��
    phPlayer       ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    hWnd           ͼ��������ھ��
    bySrcId        ǰ����ƵԴ���
    emPlayType     �ò���������������������ǲ���¼��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_IamInNat( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, HWND hWnd, 
                                       unsigned char bySrcId, EPlayType emPlayType );

/*=================================================================
��������: KDM_SetTimeout
��    ��: ����ͬ����ʱʱ��
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    dwTimeOut  Ĭ��PUSDK_SYNCMD_TIMEOUT���룬����Ϊ0
�� �� ֵ: void 
=================================================================*/
KDM_API void STDCALL KDM_SetTimeout( PUHANDLE hHandle, unsigned long dwTimeOut );

/*=================================================================
��������: KDM_IsConnected
��    ��: �Ƿ��Ѿ�����ǰ��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: ����״̬����TRUE;���򷵻�FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsConnected( PUHANDLE hHandle );

/*=================================================================
��������: KDM_PlayVideo
��    ��: �����Ƶ
����˵��: 
    hHandle         �豸������豸Ψһ��ʶ��
    phPlayer        ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    hWnd            ͼ��������ھ��
    bySrcId         ǰ����ƵԴ���
    emPlayChanType  ����������Ĭ��emPlayMain 
    wVideoStartPort ������ʼ�˿�,Ĭ������ֵ60000
    pByChanId       �����ƵԴ��Ӧ����ƵԴ����ͨ����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
          �ر�˵��: ���ӿڷ���ֵ����ʾ�����Ƿ��ͳɹ���PUSDK ʹ��ͨ�ûص���Ϣ��֪���������
          ͨ�ûص���Ϣ�ֱ��� PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK �� PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK
=================================================================*/
KDM_API int STDCALL KDM_PlayVideo( PUHANDLE hHandle, OUT LPPLAYERHANDLE phPlayer,
	HWND hWnd,
	unsigned char bySrcId, 
	EPlayChanType emPlayChanType /*= emPlayMain*/,					
	unsigned short wVideoStartPort /*= PUSDK_RTP_PORT*/, 
	OUT unsigned char *pByChanId /*= NULL*/);

/*=================================================================
��������: KDM_SwitchVideoSrc
��    ��: �л���ƵԴ����Ƶ�Զ��л�
����˵��: 
    hHandle     �豸������豸Ψһ��ʶ��
    byChanId    ͨ����
    byVSrcId    ��ƵԴ���
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SwitchVideoSrc( PUHANDLE hHandle,unsigned char byChanId, unsigned char byVSrcId );								

/*=================================================================
��������: KDM_StopPlay
��    ��: ֹͣ�������Ƶ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
�ر�˵��: ���ӿڷ���ֵ����ʾ�����Ƿ��ͳɹ���PUSDK ʹ��ͨ�ûص���Ϣ��֪���������
          ͨ�ûص���Ϣ�ֱ��� PUSDK_MSG_CB_VIDEO_SEND_REMOVE_ACK �� PUSDK_MSG_CB_AUDIO_SEND_REMOVE_ACK
=================================================================*/
KDM_API int STDCALL KDM_StopPlay( PUHANDLE hHandle, PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_PausePlay
��    ��: ��ͣ����Ƶ����
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_PausePlay( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_ResumePlay
��    ��: �ָ�����Ƶ���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ResumePlay( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_LocalPlay
��    ��: ���ؽ�����������
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    phPlayer ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    wPort ���ؽ�����ʼ�˿�
    hWnd ͼ��������ھ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_LocalPlay( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, unsigned short wPort, HWND hWnd );

/*=================================================================
��������: KDM_LocalPlayStop
��    ��: ���ؽ�����������ֹͣ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ��������� 
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_LocalPlayStop( PUHANDLE hHandle, PLAYERHANDLE hPlayer );
	
/*=================================================================
��������: KDM_PlayState
��    ��: ���״̬
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: int �ɹ� ���� EPlayState ���ͣ�ʧ�� ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_PlayState( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_IsPlayVideo
��    ��: �Ƿ��������Ƶ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: ���ڲ��ŷ���TRUE;δ���ŷ���FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsPlayVideo( PUHANDLE hHandle,PLAYERHANDLE hPlayer );


/*=================================================================
��������: KDM_ChangePlayWnd
��    ��: �л������������
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    hPlayer   ���������
    hPlayWnd  �����´��ھ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ChangePlayWnd( PUHANDLE hHandle,PLAYERHANDLE hPlayer, HWND hPlayWnd ); 

/*=================================================================
��������: KDM_RedrawLastFrame
��    ��: �����ػ�
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_RedrawLastFrame( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_StartDec
��    ��: ��ʼ����
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_StopDec
��    ��: ֹͣ����
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_IsDec
��    ��: �Ƿ����ڽ���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: ���ڽ��뷵��TRUE�����򷵻�FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsDec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_GetDecoderStatis
��    ��: ��ý�����Ϣ
����˵��: 
    hHandle     �豸������豸Ψһ��ʶ��
    hPlayer     ���������
    ptDecStatis ͳ�ƽ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetDecoderStatis( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                               TPuDecStatis* ptDecStatis );

/*=================================================================
��������: KDM_SetDrawCallBack
��    ��: ���ý���ص�
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    hPlayer  ���������
    pFunc    �ص�������
    pContext �ص����������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetDrawCallBack( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                         PUSDK_DRAW_CB pFunc,  unsigned long dwContext );
/*=================================================================
��������: KDM_SetFrameInfoCallBack
��    ��: ���ý���ص�
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
    pFunc   �ص�������
    dwParam �ص����������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetFrameInfoCallBack( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
										      PUSDK_FRAMEINFO_CB pFunc, unsigned long dwContext );
/*=================================================================
��������: KDM_SetFrameCallBack
��    ��: ���ý���ص�
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
    pFunc   �ص�������
    dwParam �ص����������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetFrameCallBack( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
										  PUSDK_FRAME_CB pFunc, unsigned long dwContext  );

/*=================================================================
��������: KDM_IsAudioMute
��    ��: �Ƿ񱾵ؾ���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: ��������TRUE�����򷵻�FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsAudioMute( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_SetMute
��    ��: ���ñ��ؾ���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
    bMute   �Ƿ���
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetMute( PUHANDLE hHandle,PLAYERHANDLE hPlayer, int bMute );

/*=================================================================
��������: KDM_SetVolume
��    ��: ���ò�������
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    hPlayer  ���������
    byVolume ����ֵ 0-25
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetVolume( PUHANDLE hHandle,PLAYERHANDLE hPlayer, unsigned char byVolume );

/*=================================================================
��������: KDM_GetVolume
��    ��: ��ȡ��������
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
    pbyVolume  ���������ֵ 0-25
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetVolume( PUHANDLE hHandle,PLAYERHANDLE hPlayer, OUT unsigned char* pbyVolume );

/*=================================================================
��������: KDM_SetCallVolume
��    ��: ���ú�������
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    hPlayer  ���������
    byVolume ����ֵ 0-10
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetCallVolume( PUHANDLE hHandle, PLAYERHANDLE hPlayer, unsigned char byVolume );

/*=================================================================
��������: KDM_GetCallVolume
��    ��: ��ȡ��������
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
    pbyVolume  ���������ֵ 0-10
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetCallVolume( PUHANDLE hHandle, PLAYERHANDLE hPlayer, OUT unsigned char* pbyVolume );

/*=================================================================
��������: KDM_InputAudPcmData
��    ��: ����PCM���ݲ����б���
����˵��: 
	hHandle      �豸������豸Ψһ��ʶ��
	phPlayer     ���������
	pPcmData     ����ָ�룻
	nDataLen     ���ݵĳ��ȣ��ֽڣ������֧��2048�ֽڣ�
	dwSampleRata ���ݵĲ����ʣ�ֻ֧��8000
	wBitLen      ����λ��ֻ֧��16;
	byChnNum     ��������ֻ֧��1
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_InputAudPcmData( PUHANDLE hHandle, PLAYERHANDLE hPlayer,
										void* pPcmData, int nDataLen,
										unsigned long dwSampleRate, 
										unsigned short wBitLen, 
										unsigned char byChnNum);

/*=================================================================
��������: KDM_StartPcmEnc
��    ��: ����PCM���ݲ����б���
����˵��: 
	hHandle               �豸������豸Ψһ��ʶ��
	phPlayer              ���������
	pFnBitStreamCB        ��Ƶ���������ص�����
	dwBitstreamCBContext  �ص�������
	pFnRtpPackCB          rtp���ص�����
	dwRtpPackCBContex     �ص�������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartPcmEnc(PUHANDLE hHandle, PLAYERHANDLE hPlayer,  
									PUSDK_INPUT_PCM_BIT_STREAM_CB pFnBitStreamCB,  unsigned long dwBitstreamCBContext, 
									PUSDK_INPUT_PCM_RTP_PACK_CB pFnRtpPackCB,  unsigned long dwRtpPackCBContex );

/*=================================================================
��������: KDM_StopPcmEnc
��    ��: ֹͣPCM����
����˵��: 
	hHandle    �豸������豸Ψһ��ʶ��
	phPlayer   ��������� 
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopPcmEnc(PUHANDLE hHandle, PLAYERHANDLE hPlayer);


/*=================================================================
��������: KDM_StartLocalRecVideo
��    ��: ���������Ƶ���ݣ��ڲ����������½��б���¼��
����˵��: 
    hHandle         �豸������豸Ψһ��ʶ��
    phPlayer        ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    bySrcId         ǰ����ƵԴ���
    emPlayChanType  ����������Ĭ��emPlayMain 
    wVideoStartPort ������ʼ�˿�,Ĭ������ֵ60000
    pByChanId       �����ƵԴ��Ӧ����ƵԴ����ͨ����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
          �ر�˵��: ���ӿڷ���ֵ����ʾ�����Ƿ��ͳɹ���PUSDK ʹ��ͨ�ûص���Ϣ��֪���������
          ͨ�ûص���Ϣ�ֱ��� PUSDK_MSG_CB_VIDEO_SEND_ADD_ACK �� PUSDK_MSG_CB_AUDIO_SEND_ADD_ACK
=================================================================*/
KDM_API int STDCALL KDM_StartLocalRecVideo( PUHANDLE hHandle, OUT LPPLAYERHANDLE phPlayer,
	                                        unsigned char bySrcId, 
	                                        EPlayChanType emPlayChanType /*= emPlayMain*/,					
	                                        unsigned short wVideoStartPort /*= PUSDK_RTP_PORT*/, 
	                                        OUT unsigned char *pByChanId /*= NULL*/);

/*=================================================================
��������: KDM_StartLocalRec
��    ��: ��ʼ����¼��
����˵��: 
    hHandle       �豸������豸Ψһ��ʶ��
    hPlayer       ���������
    strSaveDir    ¼�񱣴�Ŀ¼
    strFileName   ¼���ļ�����
    wLocalRecPort ��δʹ��,Ĭ������ֵPUSDK_LOCAL_REC_PORT 
    fpProcCB      ����¼����Ȼص�
    fpStateCB     ����¼��״̬�ص�
    dwContext     ����¼��ص�����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/

KDM_API int STDCALL KDM_StartLocalRec( PUHANDLE hHandle, PLAYERHANDLE hPlayer, 
            const char*             pSaveDir, const char* pFileName, 
			unsigned short          wLocalRecPort /*= PUSDK_LOCAL_REC_PORT*/,
            PUSDK_LOC_REC_PRO_CB    fProcCB /*= NULL*/,
            PUSDK_LOC_REC_STATE_CB  fStateCB /*= NULL*/,
            unsigned long           dwContext /*= NULL*/
            );

/*=================================================================
��������: KDM_IsLocalRecording
��    ��: �Ƿ��ڱ���¼��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: TRUE:���ڱ���¼��FALSE:û�б���¼��
=================================================================*/
KDM_API int STDCALL KDM_IsLocalRecording( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_StopLocalRec
��    ��: ֹͣ����¼��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopLocalRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_LocalSnapshot
��    ��: ����ץ��
����˵��: 
    hHandle       �豸������豸Ψһ��ʶ��
    hPlayer       ���������
    pFilePathName ץ���ļ�����ȫ·������׺��֧��(.bmp|.jpeg)
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_LocalSnapshot( PUHANDLE hHandle, PLAYERHANDLE hPlayer, const char * pFilePathName );

/*=================================================================
��������: KDM_SyncPuTime
��    ��: ͬ��ʱ��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    lSystemTime   PUSDK_SYNC_TIME_LOCAL ��ʾ�Զ���ȡ���Ե�ǰϵͳʱ�䣬��0Ϊָ����ʱ�䡣
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SyncPuTime( PUHANDLE hHandle, time_t lSystemTime /*= PUSDK_SYNC_TIME_LOCAL*/ );

/*=================================================================
��������: KDM_RebootPu
��    ��: ����ǰ��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_RebootPu( PUHANDLE hHandle );

/*=================================================================
��������: KDM_SaveSettings
��    ��: ����ǰ�˲���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SaveSettings( PUHANDLE hHandle );

/*=================================================================
��������: KDM_EnableCheckPuConnection
��    ��: �������
����˵��: 
    hHandle       �豸������豸Ψһ��ʶ��
    bEnable       TRUE:����������⣬FALSE:�رն������
    wCheckTime    �����������Ĭ������ֵPUSDK_TEST_LINK 
    byChechCount  ������������Ĭ������ֵΪPUSDK_TEST_LINK_COUNT
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_EnableCheckPuConnection( PUHANDLE hHandle, int bEnable, 
                                                      unsigned short wCheckTime /*= PUSDK_TEST_LINK*/, 
                                                      unsigned char byChechCount /*= PUSDK_TEST_LINK_COUNT*/ );

/*=================================================================
��������: KDM_UploadTitlePic
��    ��: �ϴ�ͼƬ��ͬ��ʵ��
����˵��: 
    hHandle       �豸������豸Ψһ��ʶ��
    eType         ͼƬ����
    strFilePath   �ļ�·��
    byVideoSrc    ��ƵԴ���
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_UploadTitlePic( PUHANDLE hHandle, EPuBmpType eType, const char* pFilePath, unsigned char byVideoSrc );

/*=================================================================
��������: KDM_GetPuRecNum
��    ��: ��ȡǰ��¼����
����˵��: 
    hHandle            �豸������豸Ψһ��ʶ��
    ptPuRecQueryNumReq ¼�������ѯ����
    ptAck              �����Ӧ����
�� �� ֵ: int   �ɹ������ظ��� ʧ�� ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_GetPuRecNum( PUHANDLE hHandle, const TPuRecQueryNumReq* ptPuRecQueryNumReq, 
									OUT TPuRecQueryNumAck* ptAck );	

/*=================================================================
��������: KDM_SearchPuRec
��    ��: ����ǰ��¼���ļ���Ϣ
����˵��: 
    hHandle             �豸������豸Ψһ��ʶ��
    ptPuRecQueryFileReq ��������
    ptAck               ������������Ӧ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SearchPuRec( PUHANDLE hHandle, const TPuRecQueryFileReq* ptPuRecQueryFileReq, 
                                          OUT TPuRecQueryFileAck* ptAck );

/*=================================================================
��������: KDM_StartPlayPuRec
��    ��: ��ʼ����ǰ��¼��
����˵��: 
    hHandle          �豸������豸Ψһ��ʶ��
    phPlayer         ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    hWnd             ͼ��������ھ��
    ptPuPlayStartReq ������������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartPlayPuRec( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, HWND hWnd, 
                                        TPuPlayStartReq* ptPuPlayStartReq );

/*=================================================================
��������: KDM_SetDecoderMode
��    ��: ���ý���ģʽ 
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
    bUserTimer ���ý���ģʽ bUserTimer:�Ƿ�ʹ�ö�ʱ����ģʽ TRUE:�ǣ�FALSE:��ʱ��ʽ������һ֡�ͽ�һ֡
               �ڲ���¼������ʱ����Ҫʹ�ü�ʱ��ʽ�������Ƶ����¼���ʱ��ʹ�ö�ʱ��ʽ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetDecoderMode( PUHANDLE hHandle,PLAYERHANDLE hPlayer, int bUserTimer );

/*=================================================================
��������: KDM_SetCoverDownload
��    ��: ��������¼���Ƿ��Ǹ������� 
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
    bCoverFlag TRUE:�������أ�FALSE:����������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetCoverDownload(PUHANDLE hHandle, PLAYERHANDLE hPlayer,int bCoverFlag );

/*=================================================================
��������: KDM_SetDecoderMode
��    ��: ��ȡ����¼����������� 
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
	pFile      �����ļ�¼������
	pwRate     ��������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetRecDownRate(PUHANDLE hHandle, PLAYERHANDLE hPlayer,const char * pFile, unsigned short* pwRate );

/*=================================================================
��������: KDM_IsPlayPuRec
��    ��: �Ƿ����ڲ���ָ����ǰ��¼�� 
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: ����ǰ��¼�񷵻�TRUE�����򷵻�FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsPlayPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_StopPlayPuRec
��    ��: ֹͣ����ǰ��¼��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopPlayPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_PuRecPlayOperation
��    ��: ǰ��¼����Ʋ���
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    hPlayer    ���������
    ptRecOpt   ��������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_PuRecPlayOperation( PUHANDLE hHandle,PLAYERHANDLE hPlayer, 
                                                 const TPuRecPlayOperation *ptRecOpt );

/*=================================================================
��������: KDM_PuRecSeekLocation
��    ��: ǰ��¼�񲥷��϶�����
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    byPlayId player id
    dwDragTime �Ϸ�ʱ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_PuRecSeekLocation( PUHANDLE hHandle,unsigned char byPlayId, 
                                                const unsigned long dwDragTime );

/*=================================================================
��������: KDM_DownloadPuRec
��    ��: ����¼���ļ�
����˵��: 
    hHandle              �豸������豸Ψһ��ʶ��
    phPlayer             ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    ptRecFileDownloadReq ��������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DownloadPuRec( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, 
                                            const PUSDK_TPuRecFileDownloadReq* ptRecFileDownloadReq );

/*=================================================================
��������: KDM_StopDownloadPuRec
��    ��: ֹͣ����ǰ��¼��
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ���������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopDownloadPuRec( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_DeletePuRec
��    ��: ɾ��ָ����ǰ��¼��
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    ptFileInfo ɾ������
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DeletePuRec( PUHANDLE hHandle, const TPuRecFileDelReq* ptFileInfo );

/*=================================================================
��������: KDM_StartPuRec
��    ��: ��ʼǰ���ֶ�¼��
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    byChanId ����ͨ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartPuRec( PUHANDLE hHandle, const unsigned char byChanId );

/*=================================================================
��������: KDM_StopPuRec
��    ��: ֹͣǰ���ֶ�¼��
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    byChanId ����ͨ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopPuRec( PUHANDLE hHandle, const unsigned char byChanId );
	
/*=================================================================
��������: KDM_SearchPuPic
��    ��: ��ѯǰ��ͼƬ
����˵��: 
    hHandle             �豸������豸Ψһ��ʶ��
    ptPuPicFileQueryReq ��ѯ����
    ptAck               ��ѯӦ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SearchPuPic( PUHANDLE hHandle, const TPuPicFileQueryReq * ptPuPicFileQueryReq, 
                                          TPuPicFileQueryAck* ptAck );

/*=================================================================
��������: KDM_DownloadPuPic
��    ��: ����ǰ��ͼƬ
����˵��: 
    hHandle           �豸������豸Ψһ��ʶ��
    ptPicFileDownload ��������
    pSavePathName     ����ͼƬ���Ƽ�·��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DownloadPuPic( PUHANDLE hHandle, const TPuPicFileDownloadReq* ptPicFileDownload,
                                            const char * pSavePathName );

/*=================================================================
��������: KDM_BrowsePuPic
��    ��: ���ͼƬ��ͬ��ʵ��
����˵��: 
    hHandle             �豸������豸Ψһ��ʶ��
    ptPicFileDownload   ��������
    ptPicFile           ������������ͼƬ����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_BrowsePuPic( PUHANDLE hHandle, const TPuPicFileDownloadReq* ptPicFileDownload, 
                                          OUT TPuPicFileDownloadRes* ptPicFile );
	
/*=================================================================
��������: KDM_DeletePuPic
��    ��: ɾ��ǰ��ͼƬ 
����˵��: 
    hHandle          �豸������豸Ψһ��ʶ��
    ptPuPicFileInfo  ɾ������
    ptAck            ɾ�����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DeletePuPic( PUHANDLE hHandle, const TPuPicFileDelExReq*  ptPuPicFileInfo, 
                                          TPuPicFileDelAck* ptAck );

/*=================================================================
��������: KDM_GetPuCapVal
��    ��: ��ȡ������
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    byType   �������ͣ��μ� ECapType
�� �� ֵ: int   �ɹ�����������ֵ ʧ�� ERR_INVALID ...
=================================================================*/
KDM_API int STDCALL KDM_GetPuCapVal( PUHANDLE hHandle, ECapType byType);

/*=================================================================
��������: KDM_GetPuParam
��    ��: ��ȡ���ò���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    eCfgType     ��������
    byChan       ����ͨ����
    pParam       �������������ָ��
    dwBufSize    �������ݴ�С
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetPuParam( PUHANDLE hHandle, EPuConfigType eCfgType, unsigned char byChanId, 
                                         OUT void* pParam, unsigned long dwBufSize );

/*=================================================================
��������: KDM_SetPuParam
��    ��: �������ò���
����˵��: 
    hHandle      �豸������豸Ψһ��ʶ��
    eCfgType     ��������
    byChanId     ����ͨ����
    pParam       ��������ָ��
    dwParamSize  �������ݴ�С
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_SetPuParam( PUHANDLE hHandle, EPuConfigType eCfgType, unsigned char byChanId, 
                                         const void* pParam, unsigned long dwParamSize );

/*=================================================================
��������: KDM_LoadDefaultSettings
��    ��: ����Ĭ�ϲ���
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_LoadDefaultSettings( PUHANDLE hHandle );

/*=================================================================
��������: KDM_LoadDefaultVedioSettings
��    ��: ����Ĭ����Ƶ����
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    byChanId  ����ͨ����
    loadType  Ĭ�ϼ�����Ƶͼ������ȡ��Աȶȵ�ֵ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_LoadDefaultVedioSettings( PUHANDLE hHandle, unsigned char byChanId, EPuLoadDefualt eType );

/*=================================================================
��������: KDM_ControlCamera
��    ��: ��������ͷ�ƶ�
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    byVideoSrc ��ƵԴ���
    byMoveType �ƶ����ͣ��μ� ECamControl
    byParam    �ƶ�����,Ĭ��ΪPUSDK_CAMERA_MOVE_RANGE
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ControlCamera( PUHANDLE hHandle, unsigned char byVideoSrc, 
                                       unsigned char byMoveType, unsigned char byParam);

/*=================================================================
��������: KDM_ControlCameraEx
��    ��: ��������ͷ�ƶ�(4·IPC���ݰ�)
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    byVideoSrc ��ƵԴ���
    byMoveType �������ͣ��μ� ECamControl
    byParam(1~4)    ����1~4
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ControlCameraEx( PUHANDLE hHandle, BYTE byVideoSrc, BYTE byOperateType,
		BYTE byParam1, BYTE byParam2, BYTE byParam3, BYTE byParam4 );

/*=================================================================
��������: KDM_ControlCamera
��    ��: ֱ�ӷ������ݿ�������ͷ����
����˵��:
	hHandle        �豸������豸Ψһ��ʶ��
	byVideoSrc     ��ƵԴ���
	pCtrlDate      ������������
	dwSize         ���ݳ���
�� �� ֵ: �� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
		  ֻҪ����ͷ�������ǰ�ˣ�����Ϊ�ɹ���������������Ƿ�����Ч
		  �����޷�֪��������������������سɹ�������û��ʵ�ʵ�Ч��Ӧ��
		  ��鷢�͵������Ƿ���ȷ
=================================================================*/
KDM_API int STDCALL KDM_ControlCameraDirect(PUHANDLE hHandle, unsigned char byVideoSrc, 
										    void* pCtrlDate, unsigned long dwSize );

/*=================================================================
��������: KDM_GetCamCode
��    ��: ��ȡ����ͷ�ع�����Ŀ�������
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
	pCode      �������ݻ�����
    nSize      �������ݻ���������
	byCam      ����ͷ����
    byAddrId   ����ͷID
    pbyCodeLen ���ص����ݳ���
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL  KDM_GetCamCode(PUHANDLE hHandle, char* pCode, int nSize, 
									unsigned char byCam, unsigned char byAddrId,unsigned char* pbyCodeLen);

/*=================================================================
��������: KDM_GetDiskPar
��    ��: ��ȡ������Ϣ
����˵��: 
    hHandle      �豸������豸Ψһ��ʶ��
    nDiskId      ���̺�
    pDiskPartion �����ϵͳ������Ϣ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetDiskPar( PUHANDLE hHandle, int nDiskId, OUT TPuDiskPartionParam* pDiskPartion );

/*=================================================================
��������: KDM_GetSysService
��    ��: ��ȡϵͳ����
����˵��: 
    hHandle         �豸������豸Ψһ��ʶ��
    ptSysService    ���������״̬
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
// �ر�˵��: V4��֧��
=================================================================*/
KDM_API int STDCALL KDM_GetSysService( PUHANDLE hHandle, OUT TPuSysService* ptSysService );

/*=================================================================
��������: KDM_GetSysInfo
��    ��: ��ȡϵͳ��Ϣ
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    ptSysInfo �����ϵͳ��Ϣ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
// �ر�˵��: V4��֧��
=================================================================*/
KDM_API int STDCALL KDM_GetSysInfo( PUHANDLE hHandle, OUT TPuSysInfo* ptSysInfo );

/*=================================================================
��������: KDM_GetInputPinState
��    ��: ��ȡ����ͨ��״̬
����˵��: 
    hHandle  �豸������豸Ψһ��ʶ��
    chanId   ͨ�����
    ptState  �����ͨ������״̬
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
// �ر�˵��: V4��֧��
=================================================================*/
KDM_API int STDCALL KDM_GetInputPinState( PUHANDLE hHandle, unsigned char byChanId, 
                                               OUT TPuInputPinState* ptState );

/*=================================================================
��������: KDM_GetOutputPinState
��    ��: ��ȡ���ͨ��״̬
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    byChanId  ͨ����ţ�����
    ptState   �����ͨ�����״̬
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
// �ر�˵��: V4��֧��
=================================================================*/
KDM_API int STDCALL KDM_GetOutputPinState( PUHANDLE hHandle, unsigned char byChanId, 
                                                OUT TPuOutputPinState* ptState );

/*=================================================================
��������: KDM_GetChanState
��    ��: ��ȡͨ��״̬������ͨ��״̬
����˵��: 
    hHandle       �豸������豸Ψһ��ʶ��
    ptChanReq     �������
    ptChanState   �����ͨ��״̬����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
// �ر�˵��: V4��֧��
=================================================================*/
KDM_API int STDCALL KDM_GetChanState( PUHANDLE hHandle, TPuChanStateReq* ptChanReq, 
                                           OUT TPuChanStateAck* ptChanState );

/*=================================================================
��������: KDM_ProductTest
��    ��: ������������
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    nChanId ͨ����
    pTestInfo ��������ͨѶ����
    byBufAck ������յ���Ӧ����Ϣ
    nBufLen Ӧ����Ϣ����ĳ���
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_ProductTest( PUHANDLE hHandle, int nChanId,TProTestInfo *pTestInfo, 
									  IN unsigned char* byBufAck, IN int nBufLen );

/*=================================================================
��������: KDM_StartVideoRecv
��    ��: ������������Ƶ����
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    chanId  ͨ�����
    wPort   ���ն˿�
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartVideoRecv( PUHANDLE hHandle, unsigned char byChanId, unsigned short wPort );

/*=================================================================
��������: KDM_StartAudioRecv
��    ��: ������������Ƶ����
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    byChanId  ͨ�����
    wPort     ���ն˿�
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartAudioRecv( PUHANDLE hHandle, unsigned char byChanId, unsigned short wPort );

/*=================================================================
��������: KDM_StopVideoRecv
��    ��: �رս�������Ƶ����
����˵��: 
    hHandle   �豸������豸Ψһ��ʶ��
    byChanId  ͨ�����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopVideoRecv( PUHANDLE hHandle, unsigned char byChanId );

/*=================================================================
��������: KDM_StopAudioRecv
��    ��: �رս�������Ƶ����
����˵��: 
    hHandle    �豸������豸Ψһ��ʶ��
    byChanId   ͨ�����
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopAudioRecv( PUHANDLE hHandle, unsigned char byChanId);

/*=================================================================
��������: KDM_GetVEncChanRelation
��    ��: ��ȡ��Ƶ����ͨ����ϵ������ṹ
����˵��: 
    hHandle     �豸������豸Ψһ��ʶ��
    ptRelation  �������Ƶ����ͨ����ϵ������ṹ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetVEncChanRelation( PUHANDLE hHandle, 
                                                  OUT TPuVideoEncChannelRelation* ptRelation );

/*=================================================================
��������: KDM_GetVideoEncInputPortParam
��    ��: ��ȡ��Ƶ������ƵԴ�˿ڲ����ṹ
����˵��: 
    hHandle     �豸������豸Ψһ��ʶ��
    ptRelation  �������Ƶ����ͨ����ϵ������ṹ
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetVideoEncInputPortParam( PUHANDLE hHandle, 
                                                  OUT TPuVideoEncInputPortParam atVideoEncInputPort[16] );

/*=================================================================
��������: KDM_GetPuInfo
��    ��: ��ȡ�豸PINGӦ����Ϣ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    ptInfo  �豸PINGӦ����Ϣ
�� �� ֵ: void
=================================================================*/
KDM_API void STDCALL KDM_GetPuInfo( PUHANDLE hHandle, TPuUnitInfo* ptInfo );

/*=================================================================
��������: KDM_GetBindIp
��    ��: ��ȡ����ǰ�˰󶨵ı���IP��ַ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
�� �� ֵ: �ɹ����ر���ͨѶ�󶨵ĵ�ַ�� ʧ�ܷ���С��0�Ĵ�����.
=================================================================*/
KDM_API unsigned long STDCALL KDM_GetBindIp( PUHANDLE hHandle );

/*=================================================================
��������: KDM_StartAudioSend
��    ��: ������Ƶ
����˵��: 
    hHandle      �豸������豸Ψһ��ʶ��
    phPlayer     ָ�򲥷��������ָ�룬�������PUSDK�Զ�ָ�ɵĲ��������
    byDecChanId  ��Ƶ����ͨ�����
    wLocalPort   ���ط��Ͷ˿�
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StartAudioSend( PUHANDLE hHandle, LPPLAYERHANDLE phPlayer, unsigned char byDecChanId, 
									    unsigned short wLocalPort);

/*=================================================================
��������: KDM_StopAudioSend
��    ��: ֹͣ������Ƶ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ��������� 
�� �� ֵ: �ɹ� ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_StopAudioSend( PUHANDLE hHandle,PLAYERHANDLE hPlayer );

/*=================================================================
��������: KDM_IsAudioSend
��    ��: �Ƿ����ڷ�����Ƶ
����˵��: 
    hHandle �豸������豸Ψһ��ʶ��
    hPlayer ��������� 
�� �� ֵ: ����TRUE���ڷ�����Ƶ�����򷵻�FALSE
=================================================================*/
KDM_API int STDCALL KDM_IsAudioSend( PUHANDLE hHandle, PLAYERHANDLE hPlayer, BOOL& bAudioSend);

/*=================================================================
��������: GetFormatInfo
��    ��: ��ȡ���̸�ʽ����Ϣ
����˵��: 
	hHandle          �豸������豸Ψһ��ʶ��
    ptFormatDiskInfo ���̸�ʽ����Ϣ 
�� �� ֵ: ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_GetFormatInfo(PUHANDLE hHandle, TPuFormatGetDiskInfo* ptFormatDiskInfo );

/*=================================================================
��������: KDM_DiskFormat
��    ��: ��ʽ������
����˵��: 
	hHandle    �豸������豸Ψһ��ʶ��
    ptDiskInfo ���̸�ʽ����Ϣ 
�� �� ֵ: ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
=================================================================*/
KDM_API int STDCALL KDM_DiskFormat(PUHANDLE hHandle,const  TPuFormatSetDiskInfo* ptDiskInfo );

/*=================================================================
��������: KDM_WriteComData
��    ��: д��������
����˵��: 
    hHandle  ���̸�ʽ����Ϣ 
	pComData �������ݽṹָ��
�� �� ֵ: ����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ����� 
��    ע���ϱ�����PUSDK_COM_DATA_INFORM
=================================================================*/
KDM_API int STDCALL KDM_WriteComData(PUHANDLE hHandle,const  TPuComData* pComData );
	
/*===============================================================
��������: GetErrorSreing
��    ��: ��ȡ�����������ַ���
����˵��: 
    nErrCode    ������
	pErrString  �����������ַ���������
	nSize       ����������
�� �� ֵ: void
================================================================*/
KDM_API int STDCALL GetErrorSting(int nErrCode,char* pErrString,int nSize);

/*===============================================================
��������: KDM_DebugLevel
��    ��: �Զ�����Դ�ӡ����
����˵��: 
    byLevel 0-255֮�䣬������Ϊ0��ʱ�򣬲���ӡ��ӡ��Ϣ��������Ϊ255��ʱ��
	        ��ӡ���д�ӡ��Ϣ������Ĵ�ӡ��ϢΪ��ӡ�������µĴ�ӡ��Ϣ
			���磺��ӡ��������Ϊ100������Ĵ�ӡ��Ϣ����Ϊ0-100�Ĵ�ӡ
�� �� ֵ: void
================================================================*/
KDM_API void STDCALL KDM_DebugLevel( unsigned char byLevel );

/*===============================================================
��������: KDM_DebugOutput
��    ��: �Զ�����Դ�ӡ����
����˵��: 
    byLevel    ��ӡ���� ��0-255
	lpszFormat ��printf�еĲ���ʹ�÷�����ͬ��Ĭ���ڴ�ӡ����Ϣ��β����\n����
�� �� ֵ: void
================================================================*/
KDM_API void STDCALL KDM_DebugOutput( unsigned char byLevel, char* lpszFormat, ...);

/*===============================================================
��������: KDM_GetCpuInfo
��    ��: ȡ�õ�ǰ��CPU��Ϣ
����˵��: 
    pIdlePercent  ���е�CPU�ٷֱ� 	0-100
�� �� ֵ: �ɹ�����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
================================================================*/
KDM_API int STDCALL KDM_GetCpuInfo( OUT int* pIdlePercent );

/*===============================================================
��������: KDM_SetVerticalSyncMode
��    ��: ������رս���Ĵ�ֱͬ��
����˵��: 
    bVerSync  �Ƿ�����ֱͬ��
�� �� ֵ: �ɹ�����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
================================================================*/
KDM_API INT STDCALL KDM_SetVerticalSyncMode( BOOL bVerSync );

/*===============================================================
��������: KDM_SetShowMargins
��    ��: ����ͼ��ü�
����˵��: 
    nLeft   �ü������Ե���
	nTop    �ü����ϱ�Ե��� 
	nRight  �ü����ұ�Ե��� 
	nBottom �ü����±�Ե���
	bAuto   �Ƿ�������ͼ���С�ı�
�� �� ֵ: �ɹ�����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
================================================================*/
KDM_API int STDCALL KDM_SetShowMargins(int nLeft, int nTop, int nRight, int nBottom, BOOL bAuto = TRUE);

/*===============================================================
��������: KDM_SetResetImage
��    ��: ����ͼ�������λ
����˵��: 
 
�� �� ֵ: �ɹ�����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
================================================================*/
KDM_API int STDCALL KDM_SetResetImage(PUHANDLE hHandle, unsigned char byChanId, TPuLoadDefaultConfig* ptPuLoadDefaultConfig);

/*===============================================================
��������: KDM_SetResetImage
��    ��: ����ͼ�������λ
����˵��: 
 
�� �� ֵ: �ɹ�����ERR_SUCCESS; ʧ�ܷ���С��0�Ĵ�����
================================================================*/
//KDM_API int STDCALL KDM_StartPuSnap(PUHANDLE hHandle, const char* pszSavePath);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // 
