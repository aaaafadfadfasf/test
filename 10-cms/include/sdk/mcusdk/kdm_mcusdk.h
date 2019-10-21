#ifndef _KDM_MCUSDK_INCLUDE_
#define _KDM_MCUSDK_INCLUDE_

#include "McuSdkType.h"

#ifdef KDM_MCUSDK_EXPORTS
#define KDM_MCUSDK_API __declspec(dllexport)
#else
#define KDM_MCUSDK_API __declspec(dllimport)
#endif

#if defined (_MAC_IOS_) ||defined (_KDM_LINUX_) || defined (_ANDROID_)
#define STDCALL 
#define KDM_MCUSDK_API 
#else
#define STDCALL __stdcall
#endif

/**********************************************************************************
                                 KEDACOM
/**********************************************************************************

��ʹ�ýӿڵĹ�������Ҫע�⼸����
һ���豸ID����ID���豸ID��ָ�豸��ƽ̨�е�Ψһ�ı������У���ƽ̨�������ɣ��ͻ��˼�
SDKֻ��ȥʹ�ø��豸ID���в���������ID��ָƽ̨�����ʾ�ı������У��������ֲ�ͬ��ƽ̨

�����豸����ƵԴ�ͱ���ͨ������ƵԴ������Ƶ����Դ����һ��NVR�豸�Ͻӵ����ĸ�IPC����
����˵��NVR�豸���ĸ���ƵԴ�ֱ��Ӧ4��IPC������NVR�����а˸�����ͨ�����ֱ���Ա��
��ͬ��ʽ����������ÿ��IPC���Գ�1080P��720P�������������������������������ˣ����Կ�
��˵��NVR�豸��4����ƵԴ8������ͨ��

����Ŀǰ������Ȳ����У���ƵԴ��ֵ��ͨ����ֵͨ������ȵ�ֵ
/**********************************************************************************
**********************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
    
    /******************************************************************************
    �� �� ����  Kdm_CreateMcuSdk
    ��    �ܣ�  ����McuSDKʵ����Ҫ������SDK�ӿ������ȱ����ã������ڳ��˽ӿ������
                �����ӿ��У���Ҫ�õ��ýӿڵķ���ֵ
    ��    ����  ��
    �� �� ֵ��  �ǿ�ָ�룬ָ��sdkʵ��:�ɹ���NULL:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API void* STDCALL Kdm_CreateMcuSdk();



    /******************************************************************************
    �� �� ����  Kdm_DestroyMcuSdk
    ��    �ܣ�  ����McuSDKʵ����Ҫ�����в��������Ժ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
    �� �� ֵ��  ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_DestroyMcuSdk(IN void *pMcuHandle);
    


    /******************************************************************************
    �� �� ����  Kdm_Init
    ��    �ܣ�  ��ʼ��McuSDKʵ��������SDK��������Ҫ���øýӿڶ�SDKʵ�����г�ʼ������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Init(IN void *pMcuHandle);



    /******************************************************************************
    �� �� ����  Kdm_Cleanup
    ��    �ܣ�  ����McuSDKʵ����������SDK֮ǰ��Ҫ���øýӿڽ�����ص��������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Cleanup(IN void *pMcuHandle);
    


    /******************************************************************************
    �� �� ����  Kdm_PlatTypeDetect
    ��    �ܣ�  ����½ƽ̨�����ͣ���½֮ǰ���Ե��øýӿڼ��ƽ̨����
                ����Ѿ��ܹ�ȷ��ƽ̨�����ͣ����Բ����øýӿ��Լӿ��ٶ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                strnsAddr     ƽ̨��ip��ַ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  emBussinessModUnable:    ����֪��ƽ̨���ͣ�ͨ����ζ��Ҫ��ֹ����
                emPlat1BS:    ƽ̨1.0
                emPlat2BS:    ƽ̨2.0
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API EBussinessMod STDCALL Kdm_PlatTypeDetect(IN void *pMcuHandle,
      IN const char *strnsAddr, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_ModualSelect
    ��    �ܣ�  ѡ��SDK������ƽ̨����ģ�顢����ģ�顢����ģ��
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                emBuss      ƽ̨���ͣ�����Kdm_PlatTypeDetect�ӿڵķ���ֵ
                emStream    ����ʹ��ģ�飬��ֵΪö������EStreamMod������μ�
                            EStreamMod�Ķ��壬ͨ��ȡemG900
                emDec       �Ƿ���ҪSDK����ģ�飬��ֵΪö������EDecoderMod�������
                            ��EDecoderMod�Ķ��壬ͨ��ȡemBaseDec
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ModualSelect(IN void *pMcuHandle, 
      IN EBussinessMod emBuss, IN EStreamMod emStream, IN EDecoderMod emDec);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_SetSaveLogFile/Kdm_SetSaveLogFile_V2
    ��    �ܣ�  ���ÿ�����رձ���SDK����־�ļ�
    ��    ����  pMcuHandle     ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwLogFlag      ����Kdm_SetSaveLogFile���Ƿ񱣴���־�ļ��ı�־��1��ʾ
                               ���棬0��ʾ������
                               ����Kdm_SetSaveLogFile_V2������eLogLevel�ж���ֵ���
                               ����ϣ�emLevAll��ʾ�������д�ӡ����
                SaveLogFileDirectName    ������־�ļ���·����ע���ֵ��·�����ƣ���
                                         �����ļ����ƣ��硰C:\\LOG��
                SaveLogFileName          ������־�ļ������ƣ�NULLʱĬ��Ϊ������
                dwMaxSizeKB              ������־�ļ��Ĵ�С��0ʱĬ��Ϊ50MB
                dwMaxFiles               ��־�ļ������������0ʱĬ��Ϊ32����
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ע����ͬһ��������ʹ�õ���־�ӿ�Ӧ����һ�£����ɻ��á�dwMaxSizeKBֵ
                ������mcusdk��urlplaysdkģ�飬dwMaxFilesֵ������mcusdk��urlplaysdk
                ��uniplayģ�顣
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     yss                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSaveLogFile(IN void *pMcuHandle,
      IN u32 dwLogFlag, IN const char* SaveLogFileDirectName );

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSaveLogFile_V2(IN void *pMcuHandle,
      IN u32 dwLogFlag, IN const char* SaveLogFileDirectName, 
      IN const char* SaveLogFileName, IN u32 dwMaxSizeKB, IN u32 dwMaxFiles, 
      OUT u32 *pErrorCode );



    /******************************************************************************
    �� �� ����  Kdm_SetScreenShowLog
    ��    �ܣ�  ���ÿ�����ر���Ļ��ӡ
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwShowLogLev  д����Ļ�ϵ���־�ȼ�������eLogLevel�ж���ֵ��������
                              ��emLevAll��ʾ�������д�ӡ����
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ֧��printf��__android_log_print��telnet
                telnet�˿�һ����3010��Ҳ���ܻᱻ�޸�
                telnet���� on����telnet��ӡ��off�ر�telnet��ӡ��help�鿴��������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     yss                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetScreenShowLog(IN void *pMcuHandle,
      IN u32 dwShowLogLev);



    /******************************************************************************
    �� �� ����  Kdm_GetSdkVersion_V2
    ��    �ܣ�  ��ȡSDK�İ汾��Ϣ������CU��url��ַ
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                versionInfo   SDK�İ汾��Ϣ�ṹ��ָ�룬�ⲿ�����ڴ�
                nBufSize      �����ⲿ�����ṹ��Ĵ�С
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSdkVersion_V2(IN void *pMcuHandle, 
      OUT McusdkVersionInfo *versionInfo, IN int nBufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetPlatDomainInfo
    ��    �ܣ�  ��ȡ��ǰ��¼ƽ̨��DomainID����Ϣ��DomainID����Ϣ�������ʱʹ��
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tPlatDomain  ƽ̨��DomainID����Ϣ��ͨ����һ��32λ���ַ���
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPlatDomainInfo(IN void* pMcuHandle,
      OUT DomainID& tPlatDomain, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDomainListTotal
    ��    �ܣ�  ��ȡƽ̨˽������Ϣ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDomainNum    �������Ŀ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDomainListTotal(IN void *pMcuHandle,
      OUT int& nDomainNum, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDomainList
    ��    �ܣ�  �����������Ż�ȡƽ̨˽������Ϣ����Ҫ�ȵ���Kdm_GetDomainListTotal
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                DomainIdx   �������ţ�0 - (tDomainNum-1)
                tDomainIfno ����Ϣ��������ID�������ơ��ϼ�ƽ̨��ID
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDomainList(IN void *pMcuHandle,
      IN int nDomainIdx, OUT TDomainInfo& tDomainInfo, OUT u32 *pErrorCode);
    


	/******************************************************************************
    �� �� ����  Kdm_GetChildrenGBDomainListTotal
    ��    �ܣ�  ��ȡƽ̨����������Ϣ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tGBDomainNum  �������Ŀ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetChildrenGBDomainListTotal(
      IN void *pMcuHandle, OUT int& nGBDomainNum, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetChildrenGBDomainList
    ��    �ܣ�  �����������Ż�ȡƽ̨����������Ϣ����Ҫ��
                ����Kdm_GetChildrenGBDomainListTotal
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                nGBDomainIdx  �������ţ�0 - (tGBDomainNum-1)
                tGBDomainIfno ����Ϣ��������ID�������ơ���״̬��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetChildrenGBDomainList(IN void *pMcuHandle,
      IN int nGBDomainIdx, OUT TChildrenGBDomainInfo& tGBDomainInfo, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetSDKEventCallback/Kdm_SetSDKEventCallback_V2
    ��    �ܣ�  �����¼��ص��������ڸ��ֲ���ʱ��ͨ�����ж�Ӧ���¼��ص������������
                �ɹ�������ʧ�ܵ�
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                EventCBFunc ��ʹ�����ṩʵ�ֵĻص�������SDK���¼�����ʱ���øú�����
                            ������Ӧʵ����ʹ����������
                dwUserData  �û��ص����Զ�������
    �� �� ֵ��  1:�ɹ���0:ʧ��
    ��    ע:   ֧��64bit�汾ʹ��Kdm_SetSDKEventCallback_V2
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetSDKEventCallback(IN void *pMcuHandle, 
      IN McuSdkEvent_Callback EventCBFunc, IN u32 dwUserData);

    KDM_MCUSDK_API u32 STDCALL Kdm_SetSDKEventCallback_V2(IN void *pMcuHandle,
      IN McuSdkEvent_Callback EventCBFunc, IN KDM_PARAM dwUserData);
    


    /******************************************************************************
    �� �� ����  Kdm_Login
    ��    �ܣ�  ��¼ƽ̨����ȡ�豸����������صȲ�������Ҫ�ȵ�½ƽ̨��ÿ��SDKʵ����
                ֻ�ܵ�½һ��ƽ̨
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                strUser       �û���
                strPassword   �û�����
                strnsAddr     ƽ̨��ַ��ͨ��ֻ��ip���޸���ƽ̨��½�˿�ʱ������д
                              "ip:port"����ʽ
                strClientType �ͻ������ͣ���"Windows"��"WindowsOCX"��"Linux"��
                              "IOS"��"Android" ��������
                pErrorCode    ����ʧ��ʱ�����Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Login(IN void *pMcuHandle, 
      IN const char *strUser, IN const char *strPassword, 
      IN const char *strnsAddr, IN const char *strClientType, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetSession
    ��    �ܣ�  ���ڻ�ȡ��½ƽ̨���õ�seesion��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pSesinInfo    ���ڽ���Session��Ϣ���Ѿ�����ռ��SessionInfoָ��
                pErrorCode    ����ʧ��ʱ�����Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/04/17  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSession(IN void *pMcuHandle,
      OUT SessionInfo *pSessionInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_LoginWithSession
    ��    �ܣ�  ���ڴ���session�����SDK��¼����ȡ�豸����������صȲ�������Ҫ��
                ��½ƽ̨
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                SessionInfo ��½ƽ̨��ȡ����seesion��
                bUseInsideHeartbeat     �Ƿ�ʹ��SDK�ڲ���������ͨ��ΪTRUE
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LoginWithSession(IN void *pMcuHandle, 
      IN SessionInfo *pSesinInfo, IN BOOL32 bUseInsideHeartbeat,
      OUT u32 *pErrorCode);
    


    /******************************************************************************
    �� �� ����  Kdm_Logout
    ��    �ܣ�  �ǳ�ƽ̨
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Logout(IN void *pMcuHandle, OUT u32 *pErrorCode);
    


    /******************************************************************************
    �� �� ����  Kdm_GetGroupByGroup
    ��    �ܣ�  ��ȡƽ̨�豸�ķ�����Ϣ����Kdm_GetGroupNext֮ǰ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                groupid       �����groupid���ɸ�groupid��������ȡ������Ϣ������
                              �������groupidΪ�գ���""��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ػ�ȡ���������id������Kdm_GetGroupNext�ӿڣ���Kdm_GetGroupNext
                �ӿڻ�ȡ����Ժ��ڲ����ٸ�����id
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetGroupByGroup(IN void *pMcuHandle, 
      IN GroupID groupid, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetGroupNext
    ��    �ܣ�  ѭ����ȡƽ̨���豸������Ϣ��������ʧ�ܵ�ʱ���ʾ������Ϣ��ȡ��ɣ�
                ע���ӷ���Ļ�ȡ�������������ӷ��黹��ҪǶ�׻�ȡ�ӷ������Ϣ��
                ��ȡ�ӷ���ͬ��Ҫ��������Kdm_GetGroupByGroup���л�ȡ�����������е�
                groupid��Ϊ��������Ϣ�е�groupid��groupInfo->groupID
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID   ��ȡ���������id������Kdm_GetGroupByGroup�ӿڷ���
                groupInfo  �������Ϣ�����嶨��μ�GROUPINFO�ṹ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGroupNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT GROUPINFO* groupInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDeviceByGroup
    ��    �ܣ�  ��ȡ����������豸����Kdm_GetDeviceNext֮ǰ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                gourpid       �����groupid����ʾҪ��ȡ���ĸ������µ��豸
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ػ�ȡ�������豸������id������Kdm_GetDeviceNext�ӿڣ�
                ��Kdm_GetDeviceNext�ӿڻ�ȡ����Ժ��ڲ����ٸ�����id
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetDeviceByGroup(IN void *pMcuHandle,
      IN GroupID gourpid, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDeviceNext
    ��    �ܣ�  ѭ����ȡ�����µ��豸����Kdm_GetDeviceByIndex��Kdm_GetDeviceByGroup
                �ӿڵ��ú���ã�����ʧ�ܱ�ʾ��ȡ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID      ��ȡ�豸���̵�����id���ɽӿ�Kdm_GetDeviceByIndex
                              ���߽ӿ�Kdm_GetDeviceByGroup���÷���
                deviceInfo    �������豸��Ϣ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT DEVICEINFO* deviceInfo, OUT u32 *pErrorCode);
        


    /******************************************************************************
    �� �� ����  Kdm_GetDeviceByIndex
    ��    �ܣ�  ����Ż�ȡ����������豸�����Kdm_GetDeviceNextһ��ʹ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                gourpid    �����groupid����ʾҪ��ȡ���ĸ������µ��豸
                INdex      ��ȡ���豸�ڷ����е���ţ���ʾ�Ӹ��豸��ĵ�index���豸
                           ��ʼ��ȡ����index��ֵΪ0ʱ�ýӿ���Kdm_GetDeviceByGroup
                           �ӿڵĹ�����ȫһ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ػ�ȡ�������豸������id��ʹ����Kdm_GetDeviceNext�ӿڣ���
                Kdm_GetDeviceNext�ӿڻ�ȡ����Ժ��ڲ����ٸ�����id
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetDeviceByIndex(IN void *pMcuHandle, 
      IN GroupID gourpid, IN u32 index, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetMultiViewDeviceTreeList/Kdm_GetMultiViewDeviceTreeList_V2
    ��    �ܣ�  ��ȡ����ͼ�豸���б�
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pTreeList  �豸�Ķ���ͼ���б�������ͼ������������ͼ��Ϣ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   Kdm_GetMultiViewDeviceTreeList_V2֧�ֻ�ȡ�û��ղؼУ�����ṹ�嶨��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetMultiViewDeviceTreeList(IN void *pMcuHandle,
      OUT MultiViewDeviceTreeList *pTreeList, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetMultiViewDeviceTreeList_V2(IN void *pMcuHandle,
      OUT MultiViewDeviceTreeList_V2 *pTreeList, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SelectDeviceTreeID
    ��    �ܣ�  ѡ��ǰ�������豸��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                treeId     �豸��ID
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SelectDeviceTreeID(IN void *pMcuHandle, 
      IN const char *treeId, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_SetDevStatusCallback/Kdm_SetDevStatusCallback_V2
    ��    �ܣ�  �����豸״̬�ص����������Խ��ն����豸״̬��SDK�ķ�����Ϣ����Ҫ����
                �øûص�������Ȼ���ٶ����豸״̬
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                cbFunc     ����SDK�����豸״̬��Ϣ�Ļص�����
                deUserData �û��Զ�������ݣ��ڻص�������ʹ��
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   ֧��64bit�汾ʹ��Kdm_SetDevStatusCallback_V2
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDevStatusCallback(IN void *pMcuHandle,
      IN DevStatus_Callback cbFunc, IN u32 deUserData);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDevStatusCallback_V2(IN void *pMcuHandle,
      IN DevStatus_Callback cbFunc, IN KDM_PARAM deUserData);



    /******************************************************************************
    �� �� ����  Kdm_SubscriptDeviceStatus
    ��    �ܣ�  �����豸��״̬��Ϣ��ֻ�ж���֮��ƽ̨�ŻὫ���豸��״̬��Ϣͨ���ص�
                �������ظ�ʹ���ߣ����Զ�����������Զ��Ĳ�ͬ�豸��״̬��Ϣ��һ�ε�
                ������ܹ�����MAX_REQ_DEV_SUBS_NUM���豸����Ϣ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                vctDeviceID TSUBSDEVS���ͽṹ���ڲ�����Ҫ���ĵ��豸�ĸ�����
                            �����豸���豸ID���ýṹ�����ֻ�ܰ���
                            MAX_REQ_DEV_SUBS_NUM���豸
                emSbs       ���ĵ��豸״̬�����ͣ����嶨��μ�ö��ESubscriptInfo
                            ����Ҫע����ǵ���ǽ״̬��Ҫ�������ļ������Ի�|����
                            ��ʽȥ���ĵ���ǽ��״̬��Ϣ
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SubscriptDeviceStatus(IN void *pMcuHandle, 
      IN TSUBSDEVS vctDeviceID, IN ESubscriptInfo emSbs, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_UnSubscriptDeviceStatus
    ��    �ܣ�  ȡ�������豸��״̬��Ϣ�����Զ������������ȡ�����Ĳ�ͬ�豸��״̬
                ��Ϣ��һ�ε�������ܹ�ȡ������MAX_REQ_DEV_SUBS_NUM���豸����Ϣ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                vctDeviceID TSUBSDEVS���ͽṹ���ڲ�����Ҫȡ�����ĵ��豸�ĸ�����
                            �����豸���豸ID���ýṹ�����ֻ�ܰ���
                            MAX_REQ_DEV_SUBS_NUM���豸
                emSbs       ȡ�����ĵ��豸״̬�����ͣ�����μ�ö��ESubscriptInfo
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnSubscriptDeviceStatus(IN void *pMcuHandle,
      IN TSUBSDEVS vctDeviceID, IN ESubscriptInfo emSbs, OUT u32 *pErrorCode);
    
    

    /******************************************************************************
    �� �� ����  Kdm_GetDevSrcStatus
    ��    �ܣ�  ��ȡ�豸����ƵԴ״̬��ƽ̨2.0���յ��豸��״̬�ص�����ΪemDevConfig
                ʱ���ã�ƽ̨1.0���յ�״̬�ص�����ΪemDevOnline����
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevSrc    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                srcStatus  �豸����ƵԴ״̬��Ϣ�������Ƿ����á��Ƿ�����
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevSrcStatus(IN void *pMcuHandle, 
      IN DEVCHN tDevSrc, OUT DEVSRC_ST* srcStatus, OUT u32 *pErrorCode);

    
    
    /******************************************************************************
    �� �� ����  Kdm_GetDeviceGBID
    ��    �ܣ�  ���豸ID��ȡ�豸�Ĺ���ID����Kdm_GetDeviceIDByGBID�ӿڹ����෴
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tDevGbID   ��ȡ���豸�Ĺ���id
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceGBID(IN void *pMcuHandle, 
      IN const DEVCHN tDevChn, OUT DEVCHN &tDevGbID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDeviceIDByGBID/Kdm_GetDeviceChnByGBID
    ��    �ܣ�  ���豸����ID��ȡ�豸ID����Kdm_GetDeviceGBID�ӿڹ����෴
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevGbID   �豸�Ĺ���ID
                tDevUUID/tDevChn  ��ȡ���豸��id/��ȡ���豸id����ƵԴͨ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceIDByGBID(IN void *pMcuHandle, 
      IN const DeviceID tDevGbID, OUT DeviceID &tDevUUID, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceChnByGBID(IN void *pMcuHandle,
      IN const DeviceID tDevGbID, OUT DEVCHN &tDevChn, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDevicekdmNO
    ��    �ܣ�  ��ȡ�豸��kdmNo�����ƴ��ţ��ñ��ͬ����Ψһ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tDevkdmNo  ��ȡ���豸�ƴ���
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevicekdmNO(IN void *pMcuHandle, 
      IN const DEVCHN tDevChn, OUT DeviceKDMNO& tDevkdmNo, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_GetDeviceChannelName
    ��    �ܣ�  ��ȡ�豸������ͨ��������
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDeviceId   ������豸ID
                tDevAllChnIdName  ���صĸýṹ�а����豸����ͨ��������
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceChannelName(IN void *pMcuHandle, 
      IN const DeviceID tDeviceId, OUT TDevAllChnIdName& tDevAllChnIdName, 
      OUT u32 *pErrorCode);
    


    /******************************************************************************
    �� �� ����  Kdm_SetStreamPattern
    ��    �ܣ�  ������������ķ�ʽ
    ��    ����  pMcuHandle        ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                eStreamPattern    �����Ĵ��䷽ʽ�����Ͳμ�ö��EStreamFlowPattern�Ķ���
                pErrorCode        ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetStreamPattern(IN void *pMcuHandle,
      IN EStreamFlowPattern eStreamPattern, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartRealPlay_V2/Kdm_StartRealPlay_V3
    ��    �ܣ�  ��ʼʵʱ�������������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn      �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tStreamParam ������������ԣ���õ�������ʾ�����ֶ�m_pDrawWnd��
                             V2�ӿڲμ��ṹ��SPARAM��V3�ӿڲμӽṹ��RealplayParam
                tCbSdkMedia  �ص������ṹ�弰�û����ݣ����������ص���YUV���ݻص���
                             URL�ص�����������Ӧ�Ļص������������ɹ�����յ���Ӧ
                             �����ݣ�ע��URL�ص������ṩ���û�ѡ�������ĸ�ʽ������
                             ��������Щ�ص�������ʹ��SDK��Ĭ�ϵĴ���ʽ
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ųɹ��󷵻ص�һ����0��ʼ�ķ�65535��playID:�ɹ���65535:����ʧ��
    ��    ע:   Kdm_StartRealPlay_V3�ӿھ���tStreamParam�ṹ���ֶΣ�ͬʱ֧����������
                ֱ�������ض���UDP����������ṹ�嶨��
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  3.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN SPARAM tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
      OUT u32 *pErrorCode);
      
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V3(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN RealplayParam tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartRealPlayByGBID
    ��    �ܣ�  ͨ���豸����ID����ʵʱ���
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChnGBID  �豸ͨ���Ĺ���ID��GBID���뵽DEVCHN::deviceID��Ա
                tStreamParam ������������ԣ���õ�������ʾ�����ֶ�m_pDrawWnd��
                             �������ݲμ��ṹ��SPARAM
                tCbSdkMedia  �ص������ṹ�弰�û����ݣ����������ص���YUV���ݻص���
                             URL�ص�����������Ӧ�Ļص������������ɹ�����յ���Ӧ
                             �����ݣ�ע��URL�ص������ṩ���û�ѡ�������ĸ�ʽ������
                             ��������Щ�ص�������ʹ��SDK��Ĭ�ϵĴ���ʽ
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ųɹ��󷵻ص�һ����0��ʼ�ķ�65535��playID:�ɹ���65535:����ʧ��
    ��    ע��  �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/24  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlayByGBID(IN void *pMcuHandle,
      IN DEVCHN tDevChnGBID, IN SPARAM tStreamParam, 
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopRealPlay
    ��    �ܣ�  ����dwPlayID��ֹͣ��Ӧ��ʵʱ����Ĳ���
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID    ����Kdm_StartRealPlay���صĲ���ID
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:ֹͣ���ųɹ���FALSE:ֹͣ����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRealPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzControl
    ��    �ܣ�  ��֧��PTZ�����ģ��豸����PTZ����ָ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tPtzCmd    PTZ���������ͽṹ�������������͡������ִ���ٶȡ������
                           �ȼ�������ĳ���ʱ�䣬����μ��ṹPTZCMD���������Ͳμ�
                           ö��ePtzCmd����
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN PTZCMD tPtzCmd, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPointMoveControl
    ��    �ܣ�  ��֧��PTZ�����ģ��豸���Ͷ����ƶ�ָ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tPointCmd  �����ƶ��Ĳ�����Ϣ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPointMoveControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN POINTMOVE tPointCmd, OUT u32 *pErrorCode );



    /******************************************************************************
    �� �� ����  Kdm_SendPtzPreSet/Kdm_LoadPtzPreSet
    ��    �ܣ�  ���豸����Ԥ��λ����ָ������ӿڹ�����ͬ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn     �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tPreSetPosition              Ԥ��λ���
                tPtzAuthority/pPtzAuthority  ������Ϣ���ٶȡ��ȼ�������ʱ�䣬�ɴ���
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY tPtzAuthority, 
      OUT u32 *pErrCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LoadPtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY *pPtzAuthority, 
      OUT u32 *pErrCode);    



    /******************************************************************************
    �� �� ����  Kdm_SavePtzPreSet
    ��    �ܣ�  �����豸��Ԥ��λ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn     �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tPreSetPosition  Ԥ��λ���
                pPtzAuthority    ������Ϣ�������ٶȡ��ȼ�������ʱ�䣬�ɴ���
                pErrorCode  ����ʧ��ʱ�����Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SavePtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY *pPtzAuthority, 
      OUT u32 *pErrCode);



    /******************************************************************************
    �� �� ����  Kdm_TaskWatcher
    ��    �ܣ�  ������������
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tWatchTask ������������Ϣ�������Ƿ����á���ʱʱ�䡢����ʱ���
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TaskWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TWatchTask tWatchTask, OUT u32 *pErrCode);



    /******************************************************************************
    �� �� ����  Kdm_PrePosWatcher
    ��    �ܣ�  Ԥ��λѲ��
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn      �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tWatchPrePos Ѳ�ӵ������Ϣ�������Ƿ����á��ӳ�ʱ�䡢Ԥ��λ��ŵ�
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PrePosWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TWatchPrePos tWatchPrePos, OUT u32 *pErrCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzBrightnessUp
    ��    �ܣ�  �����豸�����ȣ������Kdm_SendPtzBrightnessStopʹ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessUp(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzBrightnessDown
    ��    �ܣ�  ��С�豸�����ȣ������Kdm_SendPtzBrightnessStopʹ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessDown(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzBrightnessStop
    ��    �ܣ�  ֹͣ���ӻ��С�豸�����ȵ�����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessStop(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);
    


    /******************************************************************************
    �� �� ����  Kdm_SendPtzFocusFar
    ��    �ܣ�  �����豸��Զ�۽��������Kdm_SendPtzFocusStopʹ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusFar(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzFocusNear
    ��    �ܣ�  �����豸�Ľ��۽��������Kdm_SendPtzFocusStopʹ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusNear(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzFocusAuto
    ��    �ܣ�  �����豸���Զ��۽�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusAuto(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SendPtzFocusStop
    ��    �ܣ�  ֹͣ�豸�ľ۽�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pPtzAuthority ������Ϣ�������ٶȡ����𡢳���ʱ�䳤�ȣ��ɴ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusStop(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);    



    /******************************************************************************
    �� �� ����  Kdm_SetUpStreamTrans
    ��    �ܣ�  ��������ת����������ԣ���ƽ̨�豸����ֱ�Ӵ���ĳ��IP��ַ�˿�
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn         �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tStrTransLoc    �������ԣ���������Ƶ��RTP��RTCP�˿ڡ�����������IP��ַ
                tStrTransRemote Զ�˵�������ԣ�����μ�TStreamTransRemote�ṹ����
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���óɹ��󷵻ص�һ����0��ʼ�ķ�65535��ֱ��ID:�ɹ���65535:����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetUpStreamTrans(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TStreamTransLocal tStrTransLoc,
      OUT TStreamTransRemote& tStrTransRemote, OUT u32 *pErrorCode);


    
    /******************************************************************************
    �� �� ����  Kdm_StartStreamTrans
    ��    �ܣ�  ����ֱ��StrTransId��ʼ������ֱ������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                StrTransId    ����Kdm_SetUpStreamTrans�ӿڷ��ص�ֱ��ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartStreamTrans(IN void *pMcuHandle,
      IN u32 StrTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopStreamTrans
    ��    �ܣ�  ����ֱ��StrTransIdֹͣ������ֱ������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                StrTransId    ����Kdm_SetUpStreamTrans�ӿڷ��ص�ֱ��ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopStreamTrans(IN void *pMcuHandle,
      IN u32 StrTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_CreateRecordTask/Kdm_CreateRecordTask_V2
    ��    �ܣ�  ����¼�����񣬷���TaskID���ڲ�ѯ¼��¼��طš�¼�����صȲ���
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                domainName V2�ӿ���������������ʾ��ѯ���豸��ָ�����ϵ�¼�񣬶�����
                           ��ѯ��ǰ�û���¼���ϵ�¼��ע��domainName������ID������
                           �������豸ID"@"������ǲ���
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ط�-1��TaskIDֵ:�ɹ���-1:ʧ��
    ��    ע��  Kdm_CreateRecordTask_V2֧�ְ��򴴽�¼���������ڲ�ѯ���豸��ָ����
                �ϵ�¼��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTask(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTask_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN const char *domainName, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_CreateRecordTaskByGBID
    ��    �ܣ�  ����ͨ���豸����ID����¼�����񣬷���TaskID���ڲ�ѯ¼��¼��طš�
                ¼�����صȲ���
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChnGBID �豸�Ĺ���ID
                domainName  �豸�������������
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ط�-1��TaskIDֵ:�ɹ���-1:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/24  1.0        zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTaskByGBID(IN void *pMcuHandle,
      IN DEVCHN tDevChnGBID, IN const char *domainName, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_PreLoadRecord
    ��    �ܣ�  Ԥ����¼���¼�����ڲ�ѯ¼���¼����Kdm_GetRecordNextǰ����
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID     ����Kdm_CreateRecordTask���ص�����ID
                tPreLoadTime Ԥ����¼�����ʼʱ���
                ERecType     ¼�����ͣ���ƽ̨¼����ǰ��¼�񣬾���μ�eRecordType
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PreLoadRecord(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TPeriod tPreLoadTime, IN eRecordType ERecType,
      OUT u32 *pErrorCode);   

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_PreLoadRecord_V2(IN void *pMcuHandle,
		IN u32 dwTaskID, IN TPeriod tPreLoadTime, IN eRecordType ERecType, IN emRecordEventType ERecEvent,
		OUT u32 *pErrorCode);
		
		
    
    /******************************************************************************
    �� �� ����  Kdm_GetRecordNext
    ��    �ܣ�  ѭ����ȡ¼���¼
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID    ����Kdm_CreateRecordTask���ص�����ID
                dwSeekTime  ��ȡ¼���¼����ʼʱ�䣬������һ��¼���¼
                pRecordInfo ��ȡ��¼����Ϣ������μ�TRecordInfo
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN u32 dwSeekTime, OUT TRecordInfo *pRecordInfo, 
      OUT u32 *pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordNext_V2(IN void *pMcuHandle,
		IN u32 dwTaskID, IN u32 dwSeekTime, OUT TRecordInfo_V2 *pRecordInfo,
		OUT u32 *pErrorCode);


    /******************************************************************************
    �� �� ����  Kdm_DestroyRecordTask
    ��    �ܣ�  ����¼������
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID    ��Ҫ���ٵ�¼������ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroyRecordTask(IN void *pMcuHandle, 
      IN u32 dwTaskID);

    
    
    /******************************************************************************
    �� �� ����  Kdm_QueryRecordInDays/Kdm_QueryRecordInDays_V2
    ��    �ܣ�  ��ѯ¼��������������¼��ı�ǣ�
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                recType    ¼�����ͣ�ǰ��¼�����ƽ̨¼��
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                domainName V2�ӿ��������Ʋ�������ѯ�豸�ڸ����ϵ�¼�������������ǵ�
                           ǰ�û���¼�򣬲�����ID���������������豸ID"@"������ǲ���
                period     ��Ҫ��ѯ��ʱ�����򣬴��ݵ�ʱ�������뵽�յı߽�(���ʱ��)
                           ��ʱ������Ϊһ���¼��
                bHasRecFlags  ���ڽ���ÿ��¼���ǵ�����
                flagCnt    ������Խ��ձ�ǵĸ���
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  Kdm_QueryRecordInDays_V2֧�ְ����ѯ¼������
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryRecordInDays(IN void *pMcuHandle, 
      IN eRecordType recType, IN const DEVCHN &tDevChn, IN const TPeriod &period, 
      OUT bool *bHasRecFlags, IN int flagCnt, OUT u32* pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryRecordInDays_V2(IN void *pMcuHandle,
      IN eRecordType recType, IN const DEVCHN &tDevChn, IN const char *domainName, 
      IN const TPeriod &period, OUT bool *bHasRecFlags, IN int flagCnt, 
      OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartRecordPlay/Kdm_StartRecordPlay_V2
    ��    �ܣ�  ��ʼ¼��ط�
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID     ����Kdm_CreateRecordTask�ӿڷ��ص�����ID
                dwPlayTime   ����¼��Ŀ�ʼʱ��
                stSparam     ������������ԣ���õ�������ʾ�����ֶ�m_pDrawWnd��
                             �������ݲμ��ṹ��SPARAM
                tCbSdkMedia  �ص������ṹ�弰�û����ݣ����������ص���YUV���ݻص���
                             ��������Ӧ�Ļص������������ɹ�����յ���Ӧ�����ݣ�
                             ¼��ط�URL�ص���Ч�����
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ųɹ��󷵻ص�һ����0��ʼ�ķ�65535��playID:�ɹ���65535:����ʧ��
    ��    ע:   ֧��64bit�汾ʹ��Kdm_StartRecordPlay_V2
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN u32 dwPlayTime, IN SPARAM stSparam,
      IN CB_SDKMEDIA tCbSdkMedia, OUT u32 *pErrorCode);  

    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay_V2(IN void *pMcuHandle,
      IN u32 dwTaskID, IN u32 dwPlayTime, IN SPARAM stSparam,
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);

	//ע�� Kdm_StartRecordPlay_V3 ֻ������ƽ̨2.0��ƽ̨1.0����ƽ̨����Ļ��Ʋ�֧������ӿ�
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay_V3(IN void *pMcuHandle,
		IN TRecordInfo * recInfo, IN u32 dwPlayTime, IN RecordplayParam *ptSparam,
		IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_RecordPlayBySETime/Kdm_RecordPlayBySETime_V2
    ��    �ܣ�  ����ʱ���������¼��
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID    ����Kdm_CreateRecordTask�ӿڷ��ص�����ID
                tRecordPlayInfo  ¼���¼�������Ϣ��������ʼ����ʱ�估��Ⱦ���ڵ�
                tCbSdkMedia �ص������ṹ�弰�û����ݣ����������ص���YUV���ݻص���
                            ��������Ӧ�Ļص������������ɹ�����յ���Ӧ�����ݣ�
                            ¼��ط�URL�ص���Ч�����
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���ųɹ��󷵻ص�һ����0��ʼ�ķ�65535��playID:�ɹ���65535:����ʧ��
    ��    ע:   ֧��64bit�汾ʹ��Kdm_RecordPlayBySETime_V2
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_RecordPlayBySETime(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TRecordPlayInfo tRecordPlayInfo, 
      IN CB_SDKMEDIA tCbSdkMedia, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_RecordPlayBySETime_V2(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TRecordPlayInfo tRecordPlayInfo, 
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopRecordPlay
    ��    �ܣ�  ֹͣ¼��ط�
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ����Kdm_StartRecordPlay/Kdm_RecordPlayBySETime�ӿڷ���
                           �Ĳ���ID
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode); 



    /******************************************************************************
    �� �� ����  Kdm_SetUpRecordStreamTrans
    ��    �ܣ�  ����¼������ת����������ԣ���ƽ̨�豸¼������ֱ�Ӵ���ĳ��IP��ַ�˿�
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pRecordInfo     ¼����Ϣ������μ�TRecordInfo
                tStrTransLoc    �������ԣ���������Ƶ��RTP��RTCP�˿ڡ�����������IP��ַ
                tStrTransRemote Զ�˵�������ԣ�����μ�TStreamTransRemote�ṹ����
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ���óɹ��󷵻ص�һ����0��ʼ�ķ�65535��ֱ��ID:�ɹ���65535:����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetUpRecordStreamTrans(IN void *pMcuHandle,
      IN const TRecordInfo &tRecordInfo, IN const TStreamTransLocal &tStrTransLoc, 
	  OUT TStreamTransRemote& tStrTransRemote, OUT u32 *pErrorCode);


    
    /******************************************************************************
    �� �� ����  Kdm_StartRecordStreamTrans
    ��    �ܣ�  ����ֱ��dwTransId��ʼ¼��������ֱ������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTransId     ����Kdm_SetUpRecordStreamTrans�ӿڷ��ص�ֱ��ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/28  1.0     yss                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartRecordStreamTrans(IN void *pMcuHandle,
      IN u32 dwTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopRecordStreamTrans
    ��    �ܣ�  ����ֱ��dwTransIdֹͣ¼��������ֱ������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTransId     ����Kdm_SetUpRecordStreamTrans�ӿڷ��ص�ֱ��ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordStreamTrans(IN void *pMcuHandle,
      IN u32 dwTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_RecordPlayCtrl
    ��    �ܣ�  ¼�񲥷Ź����е�VCR����
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                stRecPlyInfo VCR���Ƶ����ͼ���Ӧ������ֵ,����μ�TREPCTLINFO����
                             �����ֶ����͵�ȡֵ�μ�ö��RecordPlayCtrlType�Ķ���
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   Kdm_RecordPlayCtrl_V2֧�����ÿ��ʱ�Ƿ����ÿ�⣬���TREPCTLINFO_V2
                �ṹ�嶨��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordPlayCtrl(IN void *pMcuHandle,
      IN TREPCTLINFO stRecPlyInfo, OUT u32 *pErrorCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordPlayCtrl_V2(IN void *pMcuHandle,
      IN TREPCTLINFO_V2 stRecPlyInfo, OUT u32 *pErrorCode);  
   


    /******************************************************************************
    �� �� ����  Kdm_StartRecordDownLoad/Kdm_StartRecordDownLoad_V2
    ��    �ܣ�  ��ʼ¼������
    ��    ����  pMcuHandle     ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID       ����Kdm_CreateRecordTask�ӿڷ��ص�����ID
                tDownloadInfo  ¼�����������Ϣ������¼���ļ����ơ���ʼʱ�䡢¼��
                               ���͡����ر��ٵ�
                pErrorCode     ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ��ʼ�ɹ��󷵻ص�һ����0��ʼ�ķ�65535������ID:�ɹ���65535:����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad(IN void *pMcuHandle,
      IN u32 dwTaskID, IN TRecordDownloadInfo tDownloadInfo, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad_V2(IN void *pMcuHandle,
      IN u32 dwTaskID, IN TRecordDownloadInfo_V2 tDownloadInfo,
	  OUT u32 *pErrorCode);

	

    /******************************************************************************
    �� �� ����  Kdm_StopRecordDownLoad
    ��    �ܣ�  ֹͣ¼������
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ����Kdm_StartRecordDownLoad/Kdm_StartRecordDownLoad_V2
                           ���ص�����ID
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordDownLoad(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartLocalRecord/Kdm_StartLocalRecord_V2
    ��    �ܣ�  ��������¼��¼���ļ������ڱ��أ��˽ӿڵ���ǰ�豣֤�����������
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID    ʵʱ�����¼��ط�ʱ���صĲ���ID
                strFileName ������·�����ļ���׺��¼���ļ�����
                emFileType  ¼������ͣ�����mp4��asf������μ�ELocalRecType
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartLocalRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN const char *strFileName, IN ELocalRecType emFileType);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartLocalRecord_V2(IN void *pMcuHandle,
      IN u32 dwPlayID, IN const char *strFileName, IN ELocalRecType emFileType, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopLocalRecord/Kdm_StopLocalRecord_V2
    ��    �ܣ�  ֹͣ����¼��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  2.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopLocalRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopLocalRecord_V2(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_SaveSnapshot
    ��    �ܣ�  ͼƬץ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
                strPicName ������·�����ļ���׺��ͼƬ����
                emPicType  ץ��ͼƬ����ĸ�ʽ������μ�ö��EPictureType
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   ץ�Ľ��ͨ���¼��ص�emSnapshot���������أ�0Ϊ�ɹ�����0Ϊ������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveSnapshot(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN const char *strPicName, IN EPictureType emPicType);
    
    

    /******************************************************************************
    �� �� ����  Kdm_RefreshPlayWnd
    ��    �ܣ�  ˢ�²��Ŵ���
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RefreshPlayWnd(IN void *pMcuHandle,
      IN u32 dwPlayID);



    /******************************************************************************
    �� �� ����  Kdm_DrawOneFrame
    ��    �ܣ�  ����ÿ֡ͼ��ӿڣ���IOS��ʹ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
#ifdef _MAC_IOS_
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DrawOneFrame(IN void *pMcuHandle,
      IN u32 dwPlayID);
#endif
    
    
    /******************************************************************************
    �� �� ����  Kdm_SetAudioEnable
    ��    �ܣ�  ������Ƶ��Ĭ�������SDK�����Ǿ���������øýӿڿ�����Ƶ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioEnable(IN void *pMcuHandle,
      IN u32 dwPlayID);



    /******************************************************************************
    �� �� ����  Kdm_SetAudioDisable
    ��    �ܣ�  �ر���Ƶ����Kdm_SetAudioEnable�����෴
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioDisable(IN void *pMcuHandle, 
      IN u32 dwPlayID);
    
    

    /******************************************************************************
    �� �� ����  Kdm_SetAudioVolume
    ��    �ܣ�  ��������ֵ����Χ��0--100
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
                nVolume    Ҫ���õ�����ֵ��С
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioVolume(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN s32 nVolume);
    


    /******************************************************************************
    �� �� ����  Kdm_GetAudioVolume
    ��    �ܣ�  ��ȡ����ֵ����Χ��0--100
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ʵʱ�����¼��ط�ʱ���صĲ���ID
    �� �� ֵ��  0--100:�ɹ���-1:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API s32 STDCALL Kdm_GetAudioVolume(IN void *pMcuHandle, 
      IN u32 dwPlayID);



    /******************************************************************************
    �� �� ����  Kdm_DropAudio
    ��    �ܣ�  ���������������Ƶ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ��ʼʵʱ�������¼��ط�ʱ���ص�ID
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/21  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DropAudio(IN void *pMcuHandle, IN u32 dwPlayID, 
      IN BOOL32 bDrop, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetAudCapVolume/Kdm_GetAudCapVolume
    ��    �ܣ�  ����/��ȡ�ɼ�����������ֻ��һ·�ɼ�������ֱ����������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwVolume/pdwVolume ����ֵ 0 - 100
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:���óɹ���FALSE:����ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/21  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudCapVolume(IN void *pMcuHandle,
      IN int dwVolume, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudCapVolume(IN void *pMcuHandle, 
      OUT int* pdwVolume, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetAudCapMute/Kdm_GetAudCapMute
    ��    �ܣ�  ����/��ȡ�ɼ������������
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                bMute/pbMute 1��������0������
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudCapMute(IN void *pMcuHandle, 
      IN int bMute, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudCapMute(IN void *pMcuHandle, 
      OUT int* pbMute, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetInputAudioPowerCB
    ��    �ܣ�  ������������ʱ����ȡ��Ƶ���ʵĻص�����          
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pGetInputAudioPowerCB  ��Ƶ���ʻص�����
                pvContext    �û��Զ������ݣ����ڻص������д��������� 
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  Kdm_Init�ɹ�֮�󣬿�����������֮ǰ����
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������    
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/08  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetInputAudioPowerCB(IN void *pMcuHandle, 
      IN PMCUSDK_GETAUDIOPOWER pGetInputAudioPowerCB, IN void* pvContext);

    

    /******************************************************************************
    �� �� ����  Kdm_SetVideoOSD
    ��    �ܣ�  ������Ļ��Ϣ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tInfo      ��Ļ��Ϣ���������ݼ�����ֵ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetVideoOSD(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TVidOSD tInfo, OUT u32* pErrorCode);


    
    /******************************************************************************
    �� �� ����  Kdm_GetVideoOSD
    ��    �ܣ�  ��ȡ��Ļ��Ϣ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                ptInfo     Ԥ�ȷ���ÿռ�� TVidOSD �ṹ�壬�������շ�����Ϣ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0        zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoOSD(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TVidOSD *ptInfo, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartPlatRec
    ��    �ܣ�  ����ƽ̨¼��¼���ļ��洢��ƽ̨��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPlatRec(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopPlatRec
    ��    �ܣ�  ֹͣƽ̨¼��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlatRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);
    
    

    /******************************************************************************
    �� �� ����  Kdm_StartPuRec
    ��    �ܣ�  ����ǰ��¼��¼���ļ��洢��ǰ���豸��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPuRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopPuRec
    ��    �ܣ�  ֹͣǰ��¼��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPuRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDevRecordStatus
    ��    �ܣ�  ��ѯ�豸¼��״̬���豸��Ҫ�ȶ���emRecStatus���豸״̬�ص�����Ϊ
                emDevRecStatusʱ���øýӿ�
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn      �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tDevSrcStat  ¼��ľ�����Ϣ���������ͼ�״̬���μ�TDevRecordStatus
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevRecordStatus(IN void *pMcuHandle, 
      IN DeviceID tDevChn, OUT TDevRecordStatus& tDevSrcStat, 
      OUT u32* pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_SendKeyFrame
    ��    �ܣ�  ǿ�Ʒ��͹ؼ�֡����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendKeyFrame(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32 *pErrorCode);

    
    
    /******************************************************************************
    �� �� ����  Kdm_SendTransData_V3
    ��    �ܣ�  ����͸������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pTransData    ͸��������Ϣ���������ݳ��ȵ���Ϣ���μ�TTransChnData_V3
                              �ṹ�嶨��
                pTransDataRsp ͸������Ӧ����Ϣ���μ�TTransChnDataRsp�ṹ�嶨��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ͳһʹ��Kdm_SendTransData_V3�ӿڣ�����֧��ƽ̨�����buf���ȣ�ͬʱͬ
                ������ǰ��Ӧ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  3.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData_V3(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData_V3 *pTransData, 
      OUT TTransChnDataRsp *pTransDataRsp, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTransData_V2
    ��    �ܣ�  ��ȡǰ���豸��͸�����ݣ��豸��Ҫ�ȶ���emRecvTransData�����豸״̬
                �ص������лص�����ΪemDevTransDataNtf��ʱ����øýӿ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tTransData    ͸��������Ϣ���������ݳ��ȵ���Ϣ���μ�TTransChnData_V2
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTransData_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TTransChnData_V2& tTransData, OUT u32* pErrorCode);

    
    
	/******************************************************************************
    �� �� ����  Kdm_SendTransDataForGB
    ��    �ܣ�  ������豸����͸�����ݣ���������ƽ̨�伶��ȫΪ����Э��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pTransData    ͸�����ݷ�����Ϣ���������ݼ����ȵ���Ϣ
                pTransDataRsp ͸������ͬ��Ӧ����Ϣ���μ�TTransChnDataRspGB�ṹ�嶨��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/05  1.0     yss                  ����
    ******************************************************************************/
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransDataForGB(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnDataGB *pTransData,
      OUT TTransChnDataRspGB *pTransDataRsp, OUT u32 *pErrorCode);



	/******************************************************************************
    �� �� ����  Kdm_GetTransDataForGB
    ��    �ܣ�  ��ȡǰ�˹����豸��͸�����ݣ��豸��Ҫ�ȶ���emRecvTransData�����豸״̬
                �ص������лص�����ΪemDevTransDataNtfGB��ʱ����øýӿ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tTransData    ͸������֪ͨ��Ϣ���������ݼ����ȵ���Ϣ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTransDataForGB(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TTransChnDataGB& tTransData, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetPuEncoderParam_V2
    ��    �ܣ�  ��ȡǰ���豸�ı������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pEncoderParamTotal    ��������������Ϣ���μ�TEncoderParamTotal_V2
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuEncoderParam_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TEncoderParamTotal_V2& pEncoderParamTotal,
      OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetPuEncoderParam
    ��    �ܣ�  ����ǰ���豸�ı������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pEncoderParam ��������������Ϣ���μ�TEncoderParam
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPuEncoderParam(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TEncoderParam *pEncoderParam,OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetPuAudioParams
    ��    �ܣ�  ��ȡǰ���豸��Ƶ�Ĳ���
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tAudParam  ��Ƶ�����������Ϣ���μ�TAudioParams
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuAudioParams(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TAudioParams& tAudParam, OUT u32* pErrorCode);

    

    /******************************************************************************
    �� �� ����  Kdm_GetDevGpsInfo_V2
    ��    �ܣ�  ��ȡ�豸��GPS��Ϣ����Ҫ�ȶ����豸��GPS״̬��Ȼ�����豸״̬�ص�����
                ���յ�����ΪemDevGpsInfoʱ���ô˽ӿڻ�ȡGPS��Ϣ
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tDevGpsInfo   �豸GPS��Ϣ��������ƫǰ����ƫ��ľ�γ����Ϣ�ȣ��μ�
                              DeviceGPSInfo_V2�ṹ�嶨��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGpsInfo_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT DeviceGPSInfo_V2& tDevGpsInfo, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDevLatiLongTude
    ��    �ܣ�  ��ȡ�豸��γ����Ϣ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                tDevLatiLongtude  �豸��γ����Ϣ������μ�TDevLatiLongTude�Ķ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevLatiLongTude(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TDevLatiLongTude& tDevLatiLongtude, 
      OUT u32* pErrorCode);
    
    
    
    /******************************************************************************
    �� �� ����  Kdm_StartVoiceCall
    ��    �ܣ�  ��·�������У��ڲ��ɼ����ͱ�����Ƶ���ݣ�ֹͣ����ʹ��Kdm_StopVoiceCall
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                temDevchn     �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                temAudioEncType    ǰ����Ƶ�������ͣ�����μ�ö��EAudioEncType�Ķ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ��Ƶ����ID 0-(MAX_AUDIO_PLAYID_NUM-1):�ɹ���-1����65535:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartVoiceCall(IN void *pMcuHandle,
      IN DEVCHN temDevchn, IN EAudioEncType temAudioEncType, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopVoiceCall
    ��    �ܣ�  ������Ƶ����IDֹͣ�������У�����Kdm_StartVoiceCall��������
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                VoiceCallId ����Kdm_StartVoiceCall�ӿڷ��ص���Ƶ���е�ID
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopVoiceCall(IN void *pMcuHandle,
      IN u32 VoiceCallId, OUT u32 *pErrorCode);
      
      

    /******************************************************************************
    �� �� ����  Kdm_StartVoiceCall_V2
    ��    �ܣ�  ֧��ͬʱ���ж���豸���������нӿڣ�ֹͣ����ʹ��Kdm_StopVoiceCall_V2
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                temDevchn     �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                temAudioEncType    ������Ч����ΪemInvailed
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   �ýӿ�Ϊ��֧�ֶ���豸���У��ڲ�û�л�����Ϣ�����ؽ��ΪTRUE��FALSE
                ��ʼ���к�ֹͣ���л����ڲ���������ʱ�����ǰ���id������ɵļ�����һ��
                ֹͣʱ��Ӧ����StopVoiceCall_V2�ӿ�
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/12  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartVoiceCall_V2(IN void *pMcuHandle, 
      IN DEVCHN temDevchn, IN EAudioEncType temAudioEncType, OUT u32 *pErrorCode);

   
   
    /******************************************************************************
    �� �� ����  Kdm_StopVoiceCall_V2
    ��    �ܣ�  �����豸��ź�ͨ��ֹͣ�������У�����Kdm_StartVoiceCall_V2��������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                temDevchn     �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/12  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopVoiceCall_V2(IN void *pMcuHandle, 
      IN DEVCHN temDevchn, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StartPuCall
    ��    �ܣ�  �������ز��ɼ�������������������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                senderAddr    �ⲿ��Ҫ���͸�ƽ̨��Ƶ�����ķ����ߵĵ�ַ��Ϣ
                puCallInfo    ���ص�ƽ̨������������Ϣ����senderAddr�ĸ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPuCall(IN void *pMcuHandle, 
      IN const DEVCHN &tDevChn, IN const TMediaAddrInfo &senderAddr, 
      OUT TPuCallInfo &puCallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopPuCall
    ��    �ܣ�  ֹͣ���ز��ɼ�������������������
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn       �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPuCall(IN void *pMcuHandle,
      IN const DEVCHN &tDevChn, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTVWallTotal
    ��    �ܣ�  ��ȡ����ǽ������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallTotal  ���صĵ���ǽ������Ŀ
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWallTotal(IN void *pMcuHandle,
      OUT int& tvWallTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTVWall
    ��    �ܣ�  ���ݵ���ǽ����ֵ����ȡ����ǽ����Ϣ����Ҫ�ȵ���Kdm_GetTVWallTotal
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallIdx    ����ǽ��Ϣ������ֵ��0-(tvWallTotal-1)
                tTvWallInfo  ���صĵ���ǽ��Ϣ������μ�TTvWallInfoData�Ķ���
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWall(IN void *pMcuHandle,
      IN u32 tvWallIdx, OUT TTvWallInfoData& tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_CreateTVWall
    ��    �ܣ�  ��������ǽ
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                CreatTvWallReq  �½��ĵ���ǽ��Ϣ������μ�TTvWallInfoData
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ����ǽID:�ɹ���NULL:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API char* STDCALL Kdm_CreateTVWall(IN void *pMcuHandle,
      IN TTvWallInfoData CreatTvWallReq, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_ModifyTVWall
    ��    �ܣ�  �޸ĵ���ǽ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tTvWallInfo Ҫ�޸ĵ���Ϣ������μ�TTvWallInfoData
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ModifyTVWall(IN void *pMcuHandle,
      IN TTvWallInfoData tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_DelTVWall
    ��    �ܣ�  ɾ������ǽ
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId      Ҫɾ���ĵ���ǽID
                tvWallName    Ҫɾ���ĵ���ǽ������
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DelTVWall(IN void *pMcuHandle,
      IN char* tvWallId, IN char* tvWallName, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTVWallById
    ��    �ܣ�  ���ݵ���ǽID��ȡ����ǽ����Ϣ
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId      ����ǽID
                tTvWallInfo   ��ȡ�ĵ���ǽ�������Ϣ������μ�TTvWallInfoData
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWallById(IN void *pMcuHandle,
      IN char* tvWallId, OUT TTvWallInfoData& tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTVDivNum
    ��    �ܣ�  ���ݵ���ǽID�����ӻ���Ż�ȡ����ǽ�е��ӻ��Ļ�����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId      ����ǽID
                tvId          ����ǽ�е��ӻ������
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ����0����Ȼ��:�ɹ���0:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetTVDivNum(IN void *pMcuHandle,
      IN char* tvWallId, IN int tvId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetTVDivNum
    ��    �ܣ�  ���õ���ǽ��ĳ���ӻ��Ļ�����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvSetDivNumData   ����ǽID�����ӻ�ID�����������μ��ṹTTvDivNumData
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetTVDivNum(IN void *pMcuHandle,
      IN TTvDivNumData tvSetDivNumData, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetTVDivState
    ��    �ܣ�  ��ȡ���ӻ��ķֻ���״̬
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tTvWallDivInfo  ����ǽID�����ӻ�ID���ֻ���ID���μ�TTvWallCommonData
                ptTWDivState  ���صķֻ���״̬���μ��ṹTTVWallTVDivStateNtf
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע:   ��Ҫ�ȶ��ĵ���ǽ״̬�����յ�emTvWallStaChgNtf״̬֪ͨʱ��ѭ������
                �����Ӧ�ֻ����״̬
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/03/20  1.0     zm                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVDivState(IN void *pMcuHandle,
        IN TTvWallCommonData tTvWallDivInfo,OUT TTVWallTVDivStateNtf* ptTWDivState,
        OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallStartPlay
    ��    �ܣ�  ʵʱ���ϵ���ǽ
    ��    ����  pMcuHandle            ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallStartPlyData    ����ǽ��Ϣ������豸��Ϣ���μ�TTvWallPlayData
                pErrorCode            ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú��
                                      �����ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStartPlay(IN void *pMcuHandle,
      IN TTvWallPlayData tvWallStartPlyData, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallStopPlay
    ��    �ܣ�  ֹͣʵʱ���ϵ���ǽ
    ��    ����  pMcuHandle            ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallStopPlyData     ����ǽ��Ϣ��ǰ���豸��Ϣ���μ�TTvWallPlayData
                pErrorCode            ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú��
                                      �����ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStopPlay(IN void *pMcuHandle,
      IN TTvWallPlayData tvWallStopPlyData, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallStartPlayRecord
    ��    �ܣ�  ¼��ط��ϵ���ǽ����ǽǰ�����Ƚ���¼��طŲ���
    ��    ����  pMcuHandle       ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID         ¼��ط�ʱ�Ĳ���ID
                tvWallComData    ����ǽID�����ӻ�ID�����ӻ��ֻ���ID
                pErrorCode       ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStartPlayRecord(IN void *pMcuHandle,
      IN u32 dwPlayID, IN TTvWallCommonData tvWallComData, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallStopPlayRecord
    ��    �ܣ�  ֹͣ¼��ط��ϵ���ǽ
    ��    ����  pMcuHandle       ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID         ¼��ط�ʱ�Ĳ���ID
                tvWallComData    ����ǽID�����ӻ�ID�����ӻ��ֻ���ID
                pErrorCode       ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStopPlayRecord(IN void *pMcuHandle,
      IN u32 dwPlayID, IN TTvWallCommonData tvWallComData, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallQuerySchemeTotal
    ��    �ܣ�  ��ѯ����ǽԤ������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId     ����ǽID
                tTvWallSchemeTotal  Ԥ��������Ŀ
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������
                             ��ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallQuerySchemeTotal(IN void *pMcuHandle,
      IN char* tvWallId, OUT int& tTvWallSchemeTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallQueryScheme
    ��    �ܣ�  ����Ԥ�������Ż�ȡԤ����Ϣ����Ҫ�ȵ���Kdm_TVWallQuerySchemeTotal
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                SchIndx       Ԥ�������ţ�0-(tTvWallSchemeTotal-1)
                tvWallScheme  Ԥ����Ϣ������μ��ṹTTvWallScheme�Ķ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallQueryScheme(IN void *pMcuHandle,
      IN int SchIndx, OUT TTvWallScheme& tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallCreatScheme
    ��    �ܣ�  ����Ԥ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallScheme  Ԥ����Ϣ������μ��ṹTTvWallScheme�Ķ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallCreatScheme(IN void *pMcuHandle,
      IN TTvWallScheme tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallModifyScheme
    ��    �ܣ�  �޸�Ԥ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallScheme  Ԥ����Ϣ������μ��ṹTTvWallScheme�Ķ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallModifyScheme(IN void *pMcuHandle,
      IN TTvWallScheme tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallDelScheme
    ��    �ܣ�  ɾ��Ԥ��
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId    ����ǽID
                SchemeName  Ԥ������
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallDelScheme(IN void *pMcuHandle,
      IN char* tvWallId, IN char* SchemeName, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallLoadScheme
    ��    �ܣ�  ����Ԥ��
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId      ����ǽID
                SchemeName    Ԥ������
                bLoadWithSave �Ƿ񱣴������Ԥ����TRUE��ʾ���棬FALSE��ʾ������
                              Ŀǰ�ò���δʹ�ã����Ժ���
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallLoadScheme(IN void *pMcuHandle,
      IN char* tvWallId,IN char* SchemeName, IN BOOL32 bLoadWithSave, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallGetSchemePollTotal
    ��    �ܣ�  ��ȡ����ǽԤ����Ѳ����
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tSchPollStepReq ����ǽԤ����Ѳ��Ϣ��ƽ̨2.0ֻ��Ҫ��д����ǽID��ƽ̨
                                1.0ֻ��Ҫ��д������ͨ��������μ�TSchPollStepReq
                SchemePollTotal ��Ѳ����
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallGetSchemePollTotal(IN void *pMcuHandle,
      IN TSchPollStepReq tSchPollStepReq, OUT int& SchemePollTotal, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallGetSchemePoll
    ��    �ܣ�  ����Ԥ����Ѳ�����Ż�ȡ����ǽԤ����Ѳ��Ϣ����Ҫ�ȵ���
                Kdm_TVWallGetSchemePollTotal
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                SchemePollIndx  Ԥ����Ѳ�����ţ�0-(SchemePollTotal-1)
                tvWallSchePoll  ��Ѳ��Ϣ������μ�tagTVWallSchemePollStep�Ķ���
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallGetSchemePoll(IN void *pMcuHandle,
      IN int SchemePollIndx,OUT tagTVWallSchemePollStep& tvWallSchePoll, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallSetSchemePoll
    ��    �ܣ�  ����Ԥ����Ѳ��Ϣ
    ��    ����  pMcuHandle          ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tSchPollStepReq     Ԥ�������Ϣ��ƽ̨2.0 ֻ��Ҫ��д����ǽID
                                    ƽ̨1.0ֻ��Ҫ��д������ͨ��
                tvWallSchePollTotal ���е���Ѳ��Ϣ��Ԥ������Ŀ
                pErrorCode          ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú��
                                    �����ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallSetSchemePoll(IN void *pMcuHandle, 
      IN TSchPollStepReq tSchPollStepReq, 
      IN TTvWallSchemePollStepTotal tvWallSchePollTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallSchemePollControl
    ��    �ܣ�  Ԥ����Ѳ����
    ��    ����  pMcuHandle       ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tSchPollStepReq  Ԥ�������Ϣ��ƽ̨2.0ֻ��Ҫ��д����ǽID
                                 ƽ̨1.0ֻ��Ҫ��д������ͨ��
                etvWSPSCmd       ����ָ�����μ�emETVWallSCHEPOLLSTEPCMD�Ķ���
                tvWallSchePoll   ���е���Ѳ��Ϣ��Ԥ������Ŀ��ƽ̨1.0������
                pErrorCode       ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallSchemePollControl(IN void *pMcuHandle,
      IN TSchPollStepReq tSchPollStepReq, IN emETVWallSCHEPOLLSTEPCMD etvWSPSCmd, 
      IN TTvWallSchemePollStepTotal& tvWallSchePoll, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_TVWallClearAlarm
    ��    �ܣ�  ����ǽ�ֻ�������
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tvWallId        ����ǽID    
                tvId            ���ӻ�ID
                tvDivId         ���ӻ��ϵķֻ���ID
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/04/25  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallClearAlarm(IN void *pMcuHandle,
      IN char *tvWallId, IN int tvId, IN int tvDivId, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetNruListTotal
    ��    �ܣ�  ��ȡNRU��Ϣ����
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                beginIdx   ��ʼ���
                num        �����ȡ����Ŀ����
                tNruNum    ʵ�ʻ�ȡ����Ŀ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNruListTotal(IN void *pMcuHandle,
      IN int beginIdx, IN int num, OUT int& tNruNum, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetNruList
    ��    �ܣ�  ����NRU�����Ż�ȡNRU��Ϣ����Ҫ�ȵ���Kdm_GetNruListTotal
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                NruIdx     NRU������
                tNruIfno   NRU��Ϣ�������豸��Ϣ���豸������Ϣ������μ�TCNruInfo
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNruList(IN void *pMcuHandle,
      IN int NruIdx, OUT TCNruInfo& tNruIfno, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetRCSSysParam
    ��    �ܣ�  ���ô��̸��ǲ���
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                temRCSSParam ���̵ĸ��ǲ��ԣ�����μ�ö��emCoverPolicy�Ķ���
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRCSSysParam(IN void *pMcuHandle,
      IN emCoverPolicy temRCSSParam, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetRCSSysParam
    ��    �ܣ�  ��ȡ���̸��ǲ���
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                temRCSSParam    ���̵ĸ��ǲ��ԣ�����μ�ö��emCoverPolicy�Ķ���
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRCSSysParam(IN void *pMcuHandle,
      OUT emCoverPolicy& temRCSSParam, OUT u32 *pErrorCode);


		
    /******************************************************************************
    �� �� ����  Kdm_SetTimeSyncServer
    ��    �ܣ�  ͬ��������ʱ�䣬���ú��ͨ���¼��ص���ʹ���ߣ��ص�����ΪemSyncTime
                m_dwReserve1 ΪUTCʱ���32λ��m_dwReserve2 ΪUTCʱ���32λ
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                bSynctimeServer    �Ƿ�ͬ��������ʱ��
                bSendCmd     �Ƿ��ڷ��غ�ֱ��ͬ��
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetTimeSyncServer(IN void *pMcuHandle, 
      IN BOOL32 bSynctimeServer, IN BOOL32 bSendCmd, OUT u32* pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetDirectTransMode
    ��    �ܣ�  ��������ֱ��ģʽ
    ��    ����  pMcuHandle        ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                bIsDirectTrans    �Ƿ�����ֱ��ģʽ��true��ʾ�ǣ�false��ʾ����
    �� �� ֵ��  ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_SetDirectTransMode(IN void *pMcuHandle,
      IN bool bIsDirectTrans);



    /******************************************************************************
    �� �� ����  Kdm_SetRecordFileMaxSizeLimit
    ��    �ܣ�  ����¼�����ص����ļ��Ĵ�С�������˴�С��¼���ļ��ᱻ�Զ��з�,
                ��λ��GB(��windows��Ч),Ĭ����3,ȡֵ��Χ[1,0x40000000)
                ������������һ���ļ�filename.mp4���з�֮���ļ�����Ϊfilename-x.mp4
                x�Ǵ�1���ۼ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                nFileSizeGB   ���ƵĴ�Сֵ����λΪGB
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordFileMaxSizeLimit(IN void *pMcuHandle, 
      IN int nFileSizeGB);
    


    /******************************************************************************
    �� �� ����  Kdm_ChangePassword
    ��    �ܣ�  �޸�ƽ̨�ĵ�¼����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                pOldPWD       ������
                pNewPWD       ������
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20    1.0            xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ChangePassword(IN void *pMcuHandle,
      IN const char *pOldPWD, IN const char *pNewPWD, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetCivilcode
    ��    �ܣ�  ��ȡ�豸��������������루Civilcode��
    ��    ����  pMcuHandle     ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                deviceGroupId  �豸��ID, ��ʽ uuid@����
                pCivilcode     ָ����civilcode���ڴ�
                bufLen         ���civilcode���ڴ泤��
                pErrorCode     ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetCivilcode( IN void* pMcuHandle, 
      IN const GroupID deviceGroupId, OUT char* pCivilcode, IN int bufLen, 
      OUT u32* pErrorCode );            



    /******************************************************************************
    �� �� ����  Kdm_StartPlay/Kdm_StartPlay_V2
    ��    �ܣ�  ʹ��url���в���, Ŀǰ֧��TCP��UDP��rtsp���ļ� 
                �ļ�����ʱurl����:["file://D:/recfile/part1/3.mp4?aa=xx&bb=yy"]
    ��    ����  pMcuHandle     ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                utf8_url       ����ֱ�Ӳ���ʹ�õ�URL
                tStreamParam   ������������ԣ���õ�������ʾ�����ֶ�m_pDrawWnd��
                               �������ݲμ��ṹ��SPARAM
                tCbSdkMedia    �ص������ṹ�弰�û����ݣ����������ص���YUV���ݻص���
                               URL�ص������ã�����Ϊ��
                pErrorCode     ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ����ID 129-65534:�ɹ���65535:ʧ��
    ��    ע:   ֧��64bit�汾ʹ��Kdm_StartPlay_V2
                �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartPlay(IN void *pMcuHandle, 
      IN const char *utf8_url, IN SPARAM tStreamParam, 
      IN CB_SDKMEDIA CbSdkMedia, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_StartPlay_V2(IN void *pMcuHandle,
      IN const char *utf8_url, IN SPARAM tStreamParam,
      IN CB_SDKMEDIA_V2 CbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_StopPlay
    ��    �ܣ�  ֹͣ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID      �ɹ�����ʱ���صĲ���ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetRecordFileTimeLen
    ��    �ܣ�  ��ȡ¼���ļ�����ʱ������λ���룩������Kdm_StartPlay����¼���ļ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID      �ɹ�����ʱ���صĲ���ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ����0����Ȼ��:�ɹ���0:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetRecordFileTimeLen(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetRecordPlayedTime
    ��    �ܣ�  ��ȡ¼���ļ���ǰ�Ѿ����ŵ�ʱ������λ���룩������Kdm_StartPlay����
                ¼���ļ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID      �ɹ�����ʱ���صĲ���ID
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  ����0����Ȼ��:�ɹ���0:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetRecordPlayedTime(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetRecordPlayedTime
    ��    �ܣ�  ����¼���ļ���ǰ�Ѿ����ŵ�ʱ������λ���룩��Ҳ����¼���ļ���Seek��
                �ܣ�����Kdm_StartPlay����¼���ļ�
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID      �ɹ�����ʱ���صĲ���ID
                dwPlayedTime  ���õĲ���ʱ��ֵ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordPlayedTime(IN void *pMcuHandle,
      IN u32 dwPlayID, IN u32 dwPlayedTime, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_PauseRecordPlay
    ��    �ܣ�  ��ͣ��ָ�¼�񲥷ţ�����Kdm_StartPlay����¼���ļ�
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   �ɹ�����ʱ���صĲ���ID
                bPause     �Ƿ���ͣ��true��ʾ��ͣ��false��ʾ����
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PauseRecordPlay(IN void *pMcuHandle,
      IN u32 dwPlayID, IN bool bPause, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDeviceStreamUrl
    ��    �ܣ�  ��ѯָ���豸��rtsp�����͵����ӵ�ַ��Ϣ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tDevChn    �豸����ص���Ϣ��������id���豸id��ͨ���ż���ƵԴ��
                urlType    URL�����ͣ�����μ�ö��emUrlType�Ķ���
                urlList    ���ص�URL�б��ں���URL��������ÿ��URL����Ϣ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceStreamUrl(IN void *pMcuHandle, 
      IN const DEVCHN &tDevChn, IN emUrlType urlType, 
      OUT Stream_UrlList &urlList, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_CreateSearchTask
    ��    �ܣ�  �����豸�������񣬷��������豸������ID
    ��    ����  pMcuHandle     ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                queryKey       ��������
                maxResultItem  ���Ľ������Ŀ
                timeoutMSec    �����ĳ�ʱʱ��
                bNeedNotify    �Ƿ���������֪ͨ������Ϊtrue����̨������ɺ�����¼�
                               �ص�������ص�emSearchResult���͵�֪ͨ������Ϊfalse��
                               û���¼�֪ͨ,��ʱ����Kdm_IsSearchFinished��ѯ��������
                pErrorCode     ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  ����0����Ȼ��:�ɹ���0:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateSearchTask(IN void *pMcuHandle, 
      IN const char* queryKey,IN u32 maxResultItem, IN u32 timeoutMSec, 
      IN BOOL32 bNotifyResult, OUT u32 *pErrorCode);

    

    /******************************************************************************
    �� �� ����  Kdm_IsSearchFinished
    ��    �ܣ�  ʵʱ�ж������Ƿ��Ѿ����
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID      Kdm_CreateSearchTask ��������������ID
                curPercent    ���ص�ǰ�Ѿ���ɵĽ���ֵ
                totalPercent  �����ܽ���ֵ
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú������Ϊ0
    �� �� ֵ��  ����TRUE��ʾ�����Ѿ���ɡ�����FALSE�����pErrorCode������Ϊ0���ʾ
                ������δ��ɣ����pErrorCode�����벻Ϊ0�����ʾ�����������ʧ����
    ��    ע��  ���ӿ��ǹ��û�ʵʱ��ѯ�����Ƿ�����õģ��������ǵ��¼��ص���Ҳ����
                ר�ŵ��¼��첽֪ͨ�ϲ������Ƿ����
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/24  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_IsSearchFinished(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT int *curPercent, OUT int *totalPercent, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetSearchResult
    ��    �ܣ�  ������Ż�ȡ�������豸��Ϣ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID   ����ID
                startIndex ��ʼ���
                maxNum     �����õ�����Ŀ
                item       ��ŷ���ֵ������,��С��С��maxNum
                curNum     ���λ�ȡ���ص���Ŀ
                totalNum   �������(����δ���ʱ�ɱ�)
                lastPage   �Ƿ������һҳ
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSearchResult(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN const u32 startIndex, IN const u32 maxNum, 
      OUT tagTSearchResultItem* item, OUT u32 *curNum, OUT u32 *totalNum,    
      OUT BOOL32* lastPage, OUT u32 *pErrorCode );



    /******************************************************************************
    �� �� ����  Kdm_DestroySearchTask
    ��    �ܣ�  ���������豸������
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwTaskID   ����ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroySearchTask(IN void *pMcuHandle,
      IN u32 dwTaskID);



    /******************************************************************************
    �� �� ����  Kdm_IsCapbilityEnable
    ��    �ܣ�  �ж�ƽ̨��SDK�Ƿ�֧��ĳ��������ĳЩ�ӿ���Ҫƽ̨��SDK���������������
                ����ʹ��
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                capType    ��Ҫ��ѯ����������
    �� �� ֵ��  TRUE:֧�֣�FALSE:��֧��
    ��    ע��  �ýӿڱ����ڳɹ���¼ƽ̨���ٵ��ã�����ֻ�᷵��FALSE
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/05/20  1.0     xw                  ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_IsCapbilityEnable(IN void *pMcuHandle, 
      IN eCapType capType);



    /******************************************************************************
    �� �� ����  Kdm_GetDeviceExtraInfoInt/Kdm_GetDeviceExtraInfoString
    ��    �ܣ�  �����ͻ�ȡ�豸������Ϣ����ֵ�Ͳ���/�ַ����Ͳ���
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                grpId        �豸����ID���ɴ���
                devId        �豸ID
                type         ��ȡ�豸������Ϣ������
                pValue/pBuf  ������ֵ/�ַ�����Ԥ�ȷ�����ڴ�ָ��
                bufSize      ��ӦpBuf�Ĵ�С
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  �ýӿ���ֱ����ƽ̨��ȡ�豸������Ϣ������ȡ������Ϣ����Ӧ�Բ���ȡ��
                ��ֱ�Ӵ��豸ID���еĲ���
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/06/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DeviceID devId, IN EDeviceExtraInfoType type, 
      OUT int *pValue, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DeviceID devId, IN EDeviceExtraInfoType type, 
      OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDevGroupExtraInfoInt/Kdm_GetDevGroupExtraInfoString
    ��    �ܣ�  �����ͻ�ȡ�豸�������Ϣ����ֵ�Ͳ���/�ַ����Ͳ�����
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                grpId        �豸����ID
                type         ��ȡ�豸�������Ϣ������
                pValue/pBuf  ������ֵ/�ַ�����Ԥ�ȷ�����ڴ�ָ��
                bufSize      ��ӦpBuf�Ĵ�С
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGroupExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN EDevGroupExtraInfoType type, OUT int *pValue, 
      OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGroupExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN EDevGroupExtraInfoType type, OUT char *pBuf, 
      OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetVideoSrcExtraInfoInt/Kdm_GetVideoSrcExtraInfoString
    ��    �ܣ�  �����ͻ�ȡ�豸��ƵԴ������Ϣ����ֵ�Ͳ���/�ַ����Ͳ�����
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                grpId        ��ƵԴ��ID���ɴ���
                devSrc       �豸��ƵԴ��Ϣ
                type         ��ȡ��ƵԴ������Ϣ������
                pValue/pBuf  ������ֵ/�ַ�����Ԥ�ȷ�����ڴ�ָ��
                bufSize      ��ӦpBuf�Ĵ�С
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoSrcExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DEVCHN devSrc, 
      IN EVideoSrcExtraInfoType type, OUT int *pValue, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoSrcExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DEVCHN devSrc, IN EVideoSrcExtraInfoType type, 
      OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_GetDevAzimuthalAngle
    ��    �ܣ�  ��ȡ�豸�ķ�λ����Ϣ
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dev        �豸�����Ϣ
                pBuf       ������ŷ�λ����Ϣ�Ļ���
                bufSize    ָ��pBuf�Ĵ�С�Լ�����ʵ�����pBuf�Ĵ�С
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ����������������صķ�λ����Ϣ�����ݣ������ɵ������Լ����С��˻���
                ��Ϣ��ǰ���豸���壬SDKֻ͸�����ϲ㡣���ǵ�ǰ���豸�Ĳ�Ʒ�����Դ˶�
                ���Ӱ�죬��������ʱ����ǰ���豸Э��ȷ����
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevAzimuthalAngle(IN void *pMcuHandle, 
      IN const DEVCHN dev, OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetDecodeMode
    ��    �ܣ�  �����Ƿ���Ӳ�����룬ȫ�����ã����ǰ����
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                decMode    ����Ӳ�������ģʽ    
    �� �� ֵ��  ��
    ��    ע��  Ĭ��������룬����Ӳ�����豸֧�֣�Ӳ�⿪��ֻ�����ú�����������Ч
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_SetDecodeMode(IN void *pMcuHandle, 
      IN EDecodeMode decMode);



    /******************************************************************************
    �� �� ����  Kdm_GetStreamSampleRate
    ��    �ܣ�  ��ȡ����������
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ������ųɹ�ʱ�Ĳ���ID
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ֻ֧��UDPģʽ
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/23  1.0     zsy                 ����
    ******************************************************************************/    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetStreamSampleRate(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT int *pVideoSampleRate, OUT int *pAudioSampleRate, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SaveVideoBufToFile
    ��    �ܣ�  ��yuv���ݱ���Ϊ���ص�bmp�ļ�����jpg�ļ�
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                nFileType   ������ļ����ͣ�����ʽEPictureType��0Ϊbmp32��1Ϊjpg����
                            ����ʽ�ݲ�֧��
                pBuf        ������Ƶyuv���ݵ�ָ��
                nSize       buf�ĳ���
                nWidth      yuvͼ��Ŀ�
                nHeight     yuvͼ��ĸ�
                nVideoType  pBuf�������Ƶ���ݵ����ͣ�����ʽMCUSDK_FRAME_DATA_FORMAT��
                            Ŀǰֻ֧��NV12��I420��ʹ��SDK��YUV�ص���ʽ��I420
                sFileName   Ŀ���ļ�·��
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/09/20  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveVideoBufToFile(IN void *pMcuHandle, 
      IN int nFileType, IN u8* pBuf, IN int nSize, IN int nWidth, IN int nHeight, 
      IN long nVideoType, IN char* sFileName, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_ChangePlayWnd
    ��    �ܣ�  �ı�ͼ��Ĳ��Ŵ���        
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID    ����Ĳ���ID
                nRegionNum  ����������Ϣ,ID=0��������ȫ������ʾ����
                pSrcRect    ��Ƶ������,���������ľ���λ�ã����ڸı䴰�ڴ�С��������
                            Ӱ�����������Ч������NULL������ȫ��,��ʾ��߱������0
                hDestWnd    ������Ƶ����ʾ���
                bEnable     �Ƿ���������Ϊfalse��������ھ����������ʾͼ��
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  nRegionNum������ָ����������������ĳһ·ͼ����ʾ�����hwnd��ʱ��
                ���ֵ�Ż����0һ��ı�ͼ������������޸�ͼ�����ʾ�������
                nRegionNum����Ϊ0����
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/09/20  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ChangePlayWnd(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN u32 nRegionNum, IN MCUSDK_KDRECT* pSrcRect, 
      IN void* hDestWnd, IN BOOL32 bEnable, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetLocalIp
    ��    �ܣ�  ���ñ���IP��ַ         
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                strLocalIp  ���ص�ip��ַ���ַ����Ϊ128byte
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  �������һ�㲻��Ҫ���ã��ṩ��CUʹ�ã������õ�����£�sdk�ڲ����Զ�
                ��ȡ����ip��ַ����Kdm_Init֮��Kdm_Login֮ǰ����
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/09/27  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetLocalIp(IN void *pMcuHandle, 
      IN const char* strLocalIp, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_SetRecvRsParam
    ��    �ܣ�  �����ش��������Ƿ����ش�         
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tRsParam    �ش�����
                bRsFlag     �Ƿ����ش�
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ֻ֧��UDPģʽ
                һ�������£�sdk������һ���ش�����Ĭ���ǿ����ش���
                SDK����CU�ͻ��˵ļ�������level-1 level-2 level-3,Ĭ��Ϊlevel-2
                TRSParam tRsPara = {80, 120, 160, 200};//level-1
                TRSParam tRsPara = {80, 200, 480, 720};//level-2
                TRSParam tRsPara = {80, 280, 600, 920};//level-3
                Kdm_Init֮��UDP����֮ǰ����
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/09/27  1.0     wxh                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecvRsParam(IN void *pMcuHandle, 
      IN MCUSDK_TRSParam tRsParam, IN BOOL32 bRsFlag, OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_EnableNATDetection
    ��    �ܣ�  ������رշ���NAT̽���
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                nEnableFlag  ������رձ�ǣ�0��ʾ��SDK�ڲ������Ƿ�����NAT̽�����
                             1��ʾǿ������NAT̽�⣬2��ʾǿ�Ʋ�����NAT̽��
                nInterval    ��ʾ����NAT̽��ʱ̽������͵�ʱ��������λΪ��
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ���ͻ��������ƽ̨����NAT�ڣ�����Ҫ���UDP����ʱ������Ҫ����NAT̽�⣬
                �Ա�UDP�����ܹ�����ͻ��ˡ����ӿڽ����ÿ�����ر�״̬��̽������ں�
                ̨��ʱ���͡����ͳɹ����������״�������ϵͳ��ǰ��¼�û���Ȩ�޵ȶ�
                �������ؾ���
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/10/11  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableNATDetection(IN void *pMcuHandle, 
      IN int nEnableFlag, IN int nInterval);



    /******************************************************************************
    �� �� ����  Kdm_SetPlatNATMode
    ��    �ܣ�  ָ��ƽ̨��NAT����
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle    
                nNATMode    ָ��ƽ̨��NAT����,δ֪��ѡ��Ĭ���Զ� cusdk__NATModeAuto
                            cusdk__NATModeAuto = 0, cusdk__NATModeInner = 1, 
                            cusdk__NATModeOutside = 2
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  һ��ֻ�ڷǳ����������²���Ҫ��������ӿ�,������Ҫ�ڵ�¼ƽ̨֮ǰ��
                ����������²���Ҫ���ô˽ӿ�
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2016/11/9   1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPlatNATMode(IN void *pMcuHandle, 
      IN int nNATMode);



    /******************************************************************************
    �� �� ����  Kdm_SetSignalTimeout
    ��    �ܣ�  �������ʱʱ�亯��
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                nRcvTimeout �������ݵĳ�ʱʱ�䣬��λΪ��
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  �˽ӿ�Ӧ����Kdm_Init֮��Kdm_Login֮ǰ����
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/01/30  1.0     yss                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSignalTimeout(IN void *pMcuHandle, 
      IN int nRcvTimeout);



    /******************************************************************************
    �� �� ����  Kdm_SetDrawCallBack
    ��    �ܣ�  ���ò�������ʱ�Ļ�ͼ�ص�����
    ��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   �ɹ����ŷ��صĲ���ID
                pfunc      �ص�����
                userdata   �ص���������û�����
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  �ص������ڲ�Ҫ����Mcusdk�������ӿڣ�Ҳ��Ҫ����ʱ�������
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/03/24  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDrawCallBack(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN McusdkDraw_Callback pfunc, IN KDM_PARAM userdata, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    �� �� ����  Kdm_QueryCurrentUserInfoInt/Kdm_QueryCurrentUserInfoString
    ��    �ܣ�  ��ѯ��ǰ�û�����Ϣ�����Ȩ�ޣ���ֵ�Ͳ���/�ַ����Ͳ���
    ��    ����  pMcuHandle    ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle    
                nQueryType    �ο�ECurrUserInfoType
                pValue/pBuf   ������ֵ/�ַ�����Ԥ�ȷ�����ڴ�ָ��
                bufSize       ��ӦpBuf�Ĵ�С
                pErrorCode    ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/04/17  1.0     wxh                 ����
    ******************************************************************************/    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryCurrentUserInfoInt(IN void *pMcuHandle, 
      IN ECurrUserInfoType nQeuryType, OUT int* pValue, OUT u32 *pErrorCode);
      
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryCurrentUserInfoString(IN void *pMcuHandle, 
      IN ECurrUserInfoType nQeuryType, OUT char *pBuf, OUT int *bufSize, 
      OUT u32 *pErrorCode);
      
      
      
    /******************************************************************************
    �� �� ����  Kdm_SetAudioEncParam/Kdm_GetAudioEncParam
    ��    �ܣ�  ����/��ȡ����������Ƶ�������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                param        ��Ƶ����������μ�McusdkAudioEncodParam�ṹ��
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioEncParam(IN void *pMcuHandle, 
      IN McusdkAudioEncodParam param, OUT u32 *pErrorCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudioEncParam(IN void *pMcuHandle, 
      OUT McusdkAudioEncodParam* param, OUT u32 *pErrorCode);
    


    /******************************************************************************
    �� �� ����  Kdm_QueryLockedRecord
    ��    �ܣ�  ��ѯ������ƽ̨¼��
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tQryLockedRec��ѯ������һ�β�ѯ��෵��MAX_REQ_DEV_SUBS_NUM�����
                ptQryResult  ��ѯ���Ľ��
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryLockedRecord(IN void *pMcuHandle, 
	  IN McusdkQueryLockedRecord tQryLockedRec, OUT McusdkQryLockedRecResult* ptQryResult,
	  OUT u32 *pErrorCode);

    

	/******************************************************************************
    �� �� ����  Kdm_LockRecord
    ��    �ܣ�  ����¼����Ϣ���������ƽ̨¼�񣬿�ͨ��Kdm_PreLoadRecord/Kdm_GetRecordNext
                ��ȡƽ̨¼����Ϣ
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                ptMcuLockRec ¼����Ϣ��һ��������MAX_REQ_DEV_SUBS_NUM��¼����Ϣ
                bLock        ����TRUEʱΪ����������FALSEʱΪ����
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockRecord(IN void *pMcuHandle, 
	  IN McusdkLockRecord* ptMcuLockRec, IN BOOL32 bLock, OUT u32 *pErrorCode);



	/******************************************************************************
    �� �� ����  Kdm_LockRecordBatch
    ��    �ܣ�  ����ʱ�������ƽ̨¼��, ��֤¼���¼��������
    ��    ����  pMcuHandle   ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                devChn       ����¼��ļ�ص�
                tRange       ����ʱ���
                pErrorCode   ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockRecordBatch(IN void *pMcuHandle, 
	  IN DEVCHN devChn, IN TPeriod tRange, OUT u32 *pErrorCode);



	/******************************************************************************
    �� �� ����  Kdm_UnLockRecordBatch
    ��    �ܣ�  ����ʱ��ν���ƽ̨¼��
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                devChn          ����¼��ļ�ص�. �����豸ID����Ϊ��, ��ʾ������������¼��
                tRange          ����ʱ���
                enableTimeRange �Ƿ�ʹ��ʱ��β���. �����õĻ���������ʱ��ε�����¼��
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnLockRecordBatch(IN void *pMcuHandle, 
	  IN DEVCHN devChn, IN TPeriod tRange, IN BOOL32 enableTimeRange, OUT u32 *pErrorCode);



	/******************************************************************************
    �� �� ����  Kdm_BatchOperate
    ��    �ܣ�  ��������ӿڣ�֧��ƽ̨¼��ǰ��¼����ˢ��ֹͣ�������С�
                ǰ������Ԥ��λ������ƽ̨¼��
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                ptBatchOperate  �������������Լ���ز���
                pErrorCode      ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/04/17  1.0     wxh                 ����   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_BatchOperate(IN void *pMcuHandle, 
	  IN McusdkBatchOperation* ptBatchOperate, OUT u32 *pErrorCode);
    


	/******************************************************************************
	�� �� ����  Kdm_SelectAudioChannel
	��    �ܣ�  ѡ�����û�ͣ��˫·��Ƶ�е�ĳһ·
	��    ����  pMcuHandle ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwPlayID   ��ʼʵʱ���ʱ���صĲ���ID
                audioChnID ѡ�����Ƶͨ��ID
                bEnable    �Ƿ�����
                pErrorCode ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
	�� �� ֵ��  TRUE : �ɹ���FALSE : ʧ��
	��    ע��  ���������֧��˫·��Ƶ�������Ƿ����˫·��Ƶ��ͨ���¼��ص�֪ͨ����
	            emDoubleAudioChannel�ϱ���֮��ɽ���ѡ��
	֧��ƽ̨��  ƽ̨2.0
	-------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
	2017/08/17  1.0     zsy                 ����
	******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SelectAudioChannel(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN u32 audioChnID, IN BOOL32 bEnable, OUT u32 *pErrorCode);


	
    /******************************************************************************
    �� �� ����  Kdm_GetDeviceInfobyDeviceId
    ��    �ܣ�  ��ȡ�����豸������Ϣ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                devId       �豸ID
                pDeviceInfo Ԥ�������ڴ洢���ص��豸������Ϣ
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ����ƽ̨�汾��֧�ָù���
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/08/17  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceInfobyDeviceId(IN void *pMcuHandle, 
      IN const DeviceID devId, OUT DEVICEINFO* pDeviceInfo, OUT u32 *pErrorCode);
	  
	  
	  
	/******************************************************************************
    �� �� ����  Kdm_GetGroupInfo
    ��    �ܣ�  ��ȡ������������Ϣ
    ��    ����  pMcuHandle  ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                tGroupID    ��ID
                tGroupInfo  �洢���ص���������Ϣ
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ����ƽ̨�汾��֧�ָù���
    ֧��ƽ̨��  ƽ̨2.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2017/08/17  1.0     zsy                 ����
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGroupInfo(IN void *pMcuHandle,
      IN GroupID tGroupID, OUT GROUPINFO &tGroupInfo, OUT u32* pErrorCode); 



    /******************************************************************************
    �� �� ����  Kdm_SetLogLevel
    ��    �ܣ�  ��Kdm_SetSaveLogFile_V2�ɹ�����SDK��־�󣬿��Ե��ô˽ӿ�����SDK�ڸ�
                ��ģ�����־�ȼ�����־�ȼ�����eLogLevel�ж���ֵ�������ϣ�emLevAll
                ��ʾ�������д�ӡ����
    ��    ����  pMcuHandle      ����Kdm_CreateMcuSdk�ӿڷ��صķǿ�handle
                dwMcusdkLev     ����mcusdkģ�����־�ȼ�
                dwUrlplaysdkLev ����urlplaysdkģ�����־�ȼ�
                dwUniplayLev    ����uniplayģ�����־�ȼ�
                pErrorCode  ����ʧ��ʱ�������Ĵ����룬ͨ���ɹ����ú�������ֵΪ0
    �� �� ֵ��  TRUE:�ɹ���FALSE:ʧ��
    ��    ע��  ��
    ֧��ƽ̨��  ƽ̨2.0��ƽ̨1.0
    -------------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��      �汾    �޸���    �߶���    �޸�����
    2018/05/2   2.0     yss                 ����
    ******************************************************************************/
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetLogLevel(IN void *pMcuHandle,
		IN u32 dwMcusdkLev, IN u32 dwUrlplaysdkLev, IN u32 dwUniplayLev, 
		OUT u32 *pErrorCode );
    


    /******************************************************************************
          ���½ӿڽ����ڲ�CU2.0ʹ�ã��ӿ��ĵ����޽ӿ�˵�����ⲿ�ͻ�������ʹ��
    ******************************************************************************/

    /*����ƽ̨�ڲ�ʹ��
    ��ʼ��ֹͣ��Ƶ���뷢�ͣ�CU���յ�ƽ̨notify֮�������Ӧ�ӿ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartAudioEncSend(IN void *pMcuHandle, 
      IN VoiceCallNatEx tCallEx, IN EAudioEncType temAudioEncType, 
      OUT u32 *pErrorCode);
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopAudioEncSend(IN void *pMcuHandle, 
      OUT u32 *pErrorCode);
    
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ����ͳ����Ϣ*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDecStatis(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT DecStatis *pDecStatis, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ����YUV��Ϣ�ص�����YUV���ݻص����������ڷ��ؽṹ�����buf�ǿյ�
    ֻ��YUV��Ϣ,û��YUV����*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetYUVInfoCallBack(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN StreamYuv_Callback pfunc, IN void *userdata, 
      OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    �������ʱ�Ƿ�֧�ִ�֡,Ĭ�ϲ�֧��,���ú�֧�ִ���512KB��֡,�������ĸ����ڴ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSupportLargeFrame(IN void *pMcuHandle, 
      IN BOOL32 bSupport, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    �ɴ����������PTZ��������, ����ֵ��Ĭ�Ϻ�ƽ̨2.0��PTZ���������һ��
    ��������������, �� pPtzParam �� nParamNum����,ÿ����������һ��int��*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PtzControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN EMcusdkPtzCmd ptzCmd, IN int *pPtzParam, 
      IN int nParamNum, OUT McusdkPtzLockInfo *pLockInfo, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ��ͣ����*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PauseDecode(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bPause, OUT u32 *pErrorCode);

	/*����ƽ̨�ڲ�ʹ��
    CUָ��InternalPlayID�Ĳ��Žӿ�*/
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V4(IN void *pMcuHandle,
		IN DEVCHN tDevChn, IN RealplayParam tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
		IN int nInternalPlayID, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ��ȡһ·���ŵ��ڲ�����ID*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetInternalPlayID(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT int *pnInternalPlayID, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ���ֽ����쳣�˳�ʱʹ���ڲ�����ID�Ͽ�ƽ̨����
    ���������·���ŵĽ��̻����ڣ���ֹʹ�øýӿ����Ͽ�����*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlayByInternalPlayID(IN void *pMcuHandle, 
      IN int nInternalPlayID, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ��ȡ��·ʵʱ������������ϸ������Ϣ��ӡ�ı�������ʵ������Ҫ���ֽ���*/
    KDM_MCUSDK_API int STDCALL Kdm_GetStreamDebugString(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT char *pDebugString, IN int nBufSize, OUT u32 *pErrorCode);
    
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ��������*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTaskWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TWatchTask *ptWatchTask, OUT u32 *pErrCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ��ȡԤ��λѲ��*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPrePosWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TWatchPrePos *ptWatchPrePos, OUT u32 *pErrCode);

    /*����ƽ̨�ڲ�ʹ��
    ��ȡԤ��λ����
    itemCount��ʾԤ��λ�������������,presetNamesԤ��λ�������飨ʹ��UTF8���룩
    ����ֵΪʵ������presetNames�ı���������-1��ʾ����*/
    KDM_MCUSDK_API int STDCALL Kdm_GetPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, OUT McusdkPresetName *presetNames, IN int itemCount, 
      OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ����Ԥ��λ������presetName Ԥ��λ������ʹ��UTF8���룩*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, IN McusdkPresetName presetName, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ɾ��Ԥ��λ������nPresetIdԤ��λId��*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DelPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, IN int nPresetId, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ����ʵʱ����GPS��Ϣ�ص�������������ȡ�����е�GPS���ݣ���ƽ̨�����ȡ*/	    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetGPSDataCallBack(IN void *pMcuHandle, 
      IN PMCUSDK_GPS_CALLBACK pfCB, IN void *pUserData, OUT u32 *pErrorCode);
    
    /*����ƽ̨�ڲ�ʹ��
    ������ѯ�澯��־������*/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateQueryAlarmLogTask(IN void *pMcuHandle, 
      IN McusdkAlarmlogReq tAlarmReq, OUT u32* pdwTotal, OUT u32 *pErrorCode);
    
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ�澯��־����*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmLogNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT McusdkAlarmLogItem* ptLogItem, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ���ٲ�ѯ�澯��־������*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroyQueryAlarmLogTask(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ������ʱ�ڴ�¼��
    nReserve �����ֶ����������ڴ�¼��ʱ������λ�룩��Ŀǰ��ʱ��0���ڲ�Ĭ��30��
    �ڴ�¼����ں�̨һֱ���У��ڴ�¼�����Ǵ�ʵʱ����ĵ�ǰʱ������30�������*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN int nReserve, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ���ڴ�¼�����
    outFullFileName����ǰ���ڴ�¼����̳�һ������¼���ļ����ļ���
    ��������ڴ�¼���ʱ�仹û�дﵽ�򳬹�30�룬��ô�����������ڴ�¼��Ҳ������30��
    ����Ѵﵽ�򾭳���30���ˣ���ô���������ڴ�¼�񽫻��ǵ�ǰʱ������30���¼������
    ��������ڴ�¼��󣬺�̨���ڴ�¼���ǻ�������еģ������´��ٴν��ڴ�¼�����*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN char *outFullFileName, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ֹͣ��ʱ�ڴ�¼��ӿڣ����ڵײ�Ⲣδ�ṩ�ýӿڣ�����Ŀǰ���ô˽ӿ���Ч*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);
      
   	/*����ƽ̨�ڲ�ʹ��
    �����ڴ�¼���Ѿ��������Ƶ֡��,��ֵ�ͣ���ʾ�Ѿ��������Ƶ֡��������-1��ʾ����*/
    KDM_MCUSDK_API int STDCALL Kdm_GetMemoryRecordBufferSizeInFrames(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    ����ADPCM�����������ͻ��˾��������������ݣ����ǲ�Ӱ�챾��¼����Ļ�������*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DropADPCMAudio(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bDrop, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ�ƶ��������Ĳ���汾��, szVersionҪ���С����Ϊ32�ֽ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDecoderVersion(IN void *pMcuHandle, 
      IN char *manufactor, OUT char *szVersion, IN int nBufSize, OUT u32 *pErrorCode);

    /*����ƽ̨�ڲ�ʹ��
    PTZ�����ͽ����ӿ�
    �����ӿ����ƽ̨����ʧ�ܣ�ʧ����ͨ�� McusdkPTZLockRsp���أ����������Ƿ��سɹ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockPTZ(IN void *pMcuHandle, IN McusdkPTZLockReq req, 
      OUT McusdkPTZLockRsp *pRsp, OUT u32 *pErrorCode);
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnLockPTZ(IN void *pMcuHandle, 
      IN McusdkPTZUnLockReq req, OUT McusdkPTZUnLockRsp *pRsp, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    �������봹ֱͬ��*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableVerticalSync(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bEnable, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ�澯������Ϣ*/  
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmTypesInfo(IN void *pMcuHandle, 
      OUT McusdkAlarmTypesInfo* pAlarmInfo, OUT u32 *pErrorCode);
      
    /*����ƽ̨�ڲ�ʹ��
    ��ȡ�澯����������Ϣ*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmLinkConfig(IN void *pMcuHandle,
      IN McusdkAlarmSource tAlarmsource, OUT McusdkAlarmLinkConfig* ptAlarmLinkConfig, 
      OUT u32 *pErrorCode);
	  
	//�����ֹ� ����/ֹͣ ƽ̨¼��
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartGroupPlatRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopGroupPlatRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	//�����ֹ� ����/ֹͣ ǰ��¼��
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartGroupPuRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopGroupPuRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);

	//��ȡ/����ǰ��¼��ƻ�����
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordPlan(IN void *pMcuHandle, IN DEVCHN devChn, 
	  OUT McusdkRecordPlan *plan, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordPlan(IN void *pMcuHandle, IN DEVCHN devChn, 
	  IN McusdkRecordPlan *plan, OUT u32* pErrorCode);
	
	//��ȡNRU�б�
	/*�ο�
	Kdm_GetNruList
	Kdm_GetNruListTotal
	*/
	//��ȡNRU�����б�
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDiskListTotal(IN void *pMcuHandle, IN const char *nruId, 
	  OUT int *pOutNum, OUT u32* pErrorCode);
	//pnNum ������Ҫ��ȡ������������ʵ��д��ppNruInfo������
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDiskList(IN void *pMcuHandle, IN const char *nruId, 
	  IN McusdkDIskInfo *ppDiskInfo, IN int nStartIndex, IN OUT int *pnNum, OUT u32* pErrorCode);
	
	//���ɾ�������󶨹�ϵ
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceDiskBindInfoList(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid�ʹ��̹���·��
		IN int nStartIndex,		//��ҳ��ѯ����ʼ����
		//pnNum:�����ʾ���β�ѯ�ķ�ҳ��ͬʱҲ��ʾppDevChnsָ���DEVCHN����
		//�����ʾʵ������ppDevChns��DEVCHN����
		IN OUT int *pnNum,  OUT DEVCHN *ppDevChns,	
		
		OUT int *pTotalNum,		//����
		OUT u32* pErrorCode);

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_BindDeviceDisk(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid�ʹ��̹���·��		
		IN DEVCHN *ppDevChns,	IN int nChnNum,		//��Ҫ�󶨵��������·����ͨ���б�
		OUT u32* pErrorCode);

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnBindDeviceDisk(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid�ʹ��̹���·��		
		IN DEVCHN *ppDevChns, IN int nChnNum,		//��Ҫ�󶨵��������·����ͨ���б�
		OUT u32* pErrorCode);

	//����ʱ��������ʱ��ͬ��¼���ѯ�ӿڡ�����Ҫ��cuʹ�ã�	
	//pNum,�룺��ʾrecords���ѷ����������������ʾʵ�ʲ�ѯ����������
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordQuery(IN void *pMcuHandle, 
	  IN McusdkQueryRecordReq *recQueryReq, OUT TRecordInfo *records, 
	  IN OUT int *pNum, OUT u32* pErrorCode);

	//���ñ�����������ˮӡ�ӿ�
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetWaterMark(IN void *pMcuHandle, IN u32 dwPlayID, 
	  IN McusdkWaterMark *pWaterMarkInfo, OUT u32* pErrorCode);

	//��ȡ��ǰ�û�������ˮӡ������������ȡ�Ƿ�����ˮӡ����
	//Ҳ������ö�� em_UserWaterMarkEnable ���� Kdm_QueryCurrentUserInfoInt �ӿڲ�ѯ
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetUserWaterMark(IN void *pMcuHandle, 
	  OUT McusdkUserWaterMark *pWaterMarkInfo, OUT u32* pErrorCode);

	//֧��RPCTRL���أ��ù��ܽ�֧�ֿƴ��Լ���¼��
	//��rpctrl�������ó�ʱ������
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRpctrlTimeOutParam(IN void *pMcuHandle, 
	  IN u32 dwInterval, IN u32 dwRepeatCount, OUT u32* pErrorCode);
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRpctrlDownLoad(IN void *pMcuHandle, 
	  IN TRecordInfo *recInfo, IN const char *localfile, IN RpctrlParam *ptParam, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRpctrlDownLoad(IN void *pMcuHandle, 
	  IN u32 dwPlayID, OUT u32* pErrorCode);
	//��ȡ���ؽ��ȣ�����pCurTime�Ѿ����ص�¼��ʱ����������pTotalTime������������¼���ʱ��������
	//��pCurTime == pTotalTimeʱ��ʾ�������
	//��������˴��󣬺�������FALSE�� �������뿴pErrorCode
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRpctrlDownLoadProgress(IN void *pMcuHandle, 
	  IN u32 dwPlayID, OUT int *pCurTime, OUT int *pTotalTime, OUT u32* pErrorCode);

	//����¼�񲥷������Ƿ�ؼ�֡����
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableKeyFramePlay(IN void *pMcuHandle, IN u32 dwPlayID, 
	  IN BOOL32 bEnable, OUT u32* pErrorCode);
	//����¼���ļ�����ȡ��Ƶ�����
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudioTrackNum(IN void *pMcuHandle, IN u32 dwPlayID, 
	  OUT int *pTrackNum, OUT u32* pErrorCode);
	
	//����ssl��ȫģʽ, 
	//bSafeSignalָ�Ƿ������������
	//ptSafeInfoָ���Ƿ������������ܣ�Ϊ����Ԥ����Ҫ����˫����֤��ʱ��������
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableSafeMode(IN void *pMcuHandle, IN BOOL32 bSafeSignal, IN McusdkSafeModeInfo *ptSafeInfo, OUT u32 *pErrorCode);
	//������ֹͣǰ���豸������ǩ������.
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableDeviceSignatrue(IN void *pMcuHandle, IN const DeviceID devId, IN bool bEnable, OUT u32 *pErrorCode);
	//������ֹͣǰ���豸���������ܿ���.
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableDeviceEncryption(IN void *pMcuHandle, IN const DeviceID devId, IN bool bEnable, OUT u32 *pErrorCode);

	
	//��ȡ��ǰ����¼���ʱ����Χ����¼�߷ŵ�ʱ����������¼���ļ���Χ�ã�
	//����ǵ��ţ��벻Ҫ���øýӿڣ��ýӿڲ�֧�ֵ���
	//���������Ȼǿ�Ƶ��ô˽ӿڻ�ȡ��������ô�����ĺ��������޷�������
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordRange(IN void *pMcuHandle, IN u32 dwPlayID, 
	  OUT TPeriod *prange, OUT u32 *pcurTime, OUT u32 *pErrorCode);
	  
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRCSSysParam_V2(IN void *pMcuHandle,
      IN McusdkRCSSysParam tRcsParam, OUT u32 *pErrorCode);
	  
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRCSSysParam_V2(IN void *pMcuHandle,
      OUT McusdkRCSSysParam* pRcsParam, OUT u32 *pErrorCode);  
    
    //ֱ��ָ��¼���ļ���������, nSpeed��ʾ���ر��٣�0��ʾĬ�ϣ�������ȡֵ2,4,8,16,
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad_V3(IN void *pMcuHandle,
      IN TRecordInfo *recInfo, IN TPeriod *downloadRange,  IN const char *localfile,
      IN int nSpeed, OUT u32 *pErrorCode);

	//�ϲ����¼��һ��¼�񣬸ýӿڲ�֧�ֶ�ʵ��Ҳ��֧�ֶ��̣߳�ͬһ����ͬһʱ��ֻ����һ��
	//��windows����Ч
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_CombineRecordFiles(IN void *pMcuHandle,
		IN const char **inputFileList, IN int nFileNum, IN const char *outputFile, 
		IN Mcusdk_Combine_CallBack pfn, IN void *userData, OUT u32 *pErrorCode);

	KDM_MCUSDK_API void STDCALL Kdm_CancelCombineRecordFiles(IN void *pMcuHandle);	
      
	//������ȡ�豸�Ĺ���ID
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceGBIDBatch(IN void *pMcuHandle, IN OUT DeviceID2GBIDItem *devIds, IN int devNum/*<=100*/,  OUT u32 *pErrorCode);

    /******************************************************************************
                         ���½ӿڷ�������������Ҫ����ʹ��
    ******************************************************************************/

    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetHighDefinitionValue(IN void *pMcuHandle);

    /*�ýӿڷ��� ��ʹ������ӿ��е�m_wHighDefinition�ֶλ�URL�ص�*/
    KDM_MCUSDK_API void STDCALL Kdm_SetHighDefinitionValue(IN void *pMcuHandle,
      IN BOOL32 temHighDefinition);

    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetFeatureCode(IN void *pMcuHandle, 
      IN const char* pchFeatureCode);

    /*�ýӿڷ�������ʹ���������ӿ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SearchDvcReq(IN void *pMcuHandle, 
      IN const char* SearchDvcName,IN int SearchResLimit, OUT u32* pErrorCode);

    /*�ýӿڷ�������ʹ���������ӿ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSearchDvcInfo(IN void *pMcuHandle,
      OUT StSearchDvcRspInfo &temSearchDvcInfo,OUT u32* pErrorCode);

    /*�ýӿڷ�������ȡ����Ƶ������ʹ��Kdm_GetStreamSampleRate�ӿ�*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPlayingChnInfo(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT TTPlyingChnAVInfo& tPlayingChnInfo, OUT u32* pErrorCode);

    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDecodeColor(IN void *pMcuHandle,
      IN u32 dwPlayID, IN int nRegionNum, IN int nBrightness,
      IN int nContrast, IN int nSaturation, IN int nHue, OUT u32 *pErrorCode); 
      
    /*�ýӿڷ���  ��ʹ��Kdm_StartRealPlay_V2 Kdm_StartRealPlay_V3*/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN SPARAM tStreamParam, IN CB_SDKMEDIA tCbSdkMedia, 
      OUT u32 *pErrorCode);   
    
    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGbidByGbid(IN void *pMcuHandle,  
      IN DEVCHN tGbid, OUT DEVCHN* ptOldGbid, OUT u32 *pErrorCode);

    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNewGbidByOldGbid(IN void *pMcuHandle,  
        IN DEVCHN tOldGbid, OUT DEVCHN* ptNewGbid, OUT u32 *pErrorCode);
    
    /*�ýӿڷ���*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DeleteRecord(IN void *pMcuHandle, 
      IN const char *domainName, IN const DEVCHN chn, IN const TPeriod timeRange, 
      OUT u32 *pErrorCode);
      
    /*�ýӿڷ���  ��ʹ��Kdm_GetSdkVersion_V2*/  
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSdkVersion(IN void *pMcuHandle, 
      OUT char* pchVersion, OUT char *pchUpdateurl);
    
    /*�ýӿڷ���  ��ʹ��Kdm_SendTransData_V3*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData *pTransData, OUT u32 *pErrorCode);
    
    /*�ýӿڷ���  ��ʹ��Kdm_SendTransData_V3*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData_V2 *pTransData, OUT u32 *pErrorCode);
    
    /*�ýӿڷ���  ��ʹ��Kdm_GetPuEncoderParam_V2*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuEncoderParam(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TEncoderParamTotal& pEncoderParamTotal,
      OUT u32* pErrorCode);
    
    /*�ýӿڷ���  ��ʹ��Kdm_GetDevGpsInfo_V2*/ 
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGpsInfo(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT DeviceGPSInfo& tDevGpsInfo, OUT u32* pErrorCode);
      

#ifdef __cplusplus
}
#endif



#endif
