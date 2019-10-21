/*�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
  �UKDDevFinder.h                                                               �U
  �U  �������ܣ�                                                                �U
  �U     * ͨ���鲥���������豸                                                 �U
  �U     * ���������豸�����ַ                                                 �U
  �U     * ���������豸����״̬                                                 �U
  �U     * ���������豸��Ϣ                                                     �U
  �U     * ���������豸��չ����                                                 �U
  �U                                                                            �U
  �d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g
  �U�޶���¼                                                                    �U
  �U1.0      ����1.0�汾                                    ���      2011-07-19�U
  �U         ����Э��ջ�ӿ�                                 ���      2011-07-27�U
  �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a*/
#ifndef _KDDEVFINDER_H_
#define _KDDEVFINDER_H_
#include "kdvtype.h"


#ifdef WIN32

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib,"Ws2_32.lib")

    #ifdef __cplusplus
        #define KDDEVFINDER_API               extern "C" __declspec(dllexport)
    #else
        #define KDDEVFINDER_API               __declspec(dllexport)
    #endif //__cplusplus
#elif defined _LINUX_
    #ifdef __cplusplus
        #define KDDEVFINDER_API               extern "C"
    #else
        #define KDDEVFINDER_API
    #endif //__cplusplus
#endif


/*�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
  �U��������                                                                    �U
  �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a*/
  
#define KDDEVFINDER_VERSION                 "v1.0"
#define KDDEVFINDER_DEFAULT_FIND_ADDRESS    "239.255.255.200"
#define KDDEVFINDER_DEFAULT_FIND_PORT       21000
#define KDDEVFINDER_DEFAULT_UUID_LEN        32
#define KDDEVFINDER_DEFAULT_STRING_LEN      256
#define KDDEVFINDER_DEFAULT_LONG_STRING_LEN 2048
#define KDDEVFINDER_DEFAULT_TIMEOUT         32
#define MAX_SUPPORT_INTERFACE_NUM 32


typedef enum
{
    EKDDEVFINDER_OK             = 0,        // �ɹ�
    EKDDEVFINDER_INVALID_ARG    = 1,        // ��������
    EKDDEVFINDER_OUT_OF_MEMORY  = 2,        // �ڴ����
    EKDDEVFINDER_TIMEOUT        = 3,        // ��ʱ
    EKDDEVFINDER_REFUSE         = 4,        // �ܾ��������ڻص�����ʾ�ܾ���Ӧ��
    EKDDEVFINDER_SYSFAIL        = 5,        // ϵͳ����ʧ��
}   EKDDEVFINDER_RESULT;				// ����ֵ

typedef enum
{
    EKDDEVFINDER_EVENT_SEARCH               = 10000,        // ����
    EKDDEVFINDER_EVENT_SEARCH_ACK           = 10001,        // ������Ӧ
    EKDDEVFINDER_EVENT_SEARCH_FINISH        = 10003,        // ������Ӧ
    EKDDEVFINDER_EVENT_TIMEOUT              = 10009,        // ��ʱ
    EKDDEVFINDER_EVENT_CHANGE_IP            = 10010,        // �޸�IP
    EKDDEVFINDER_EVENT_CHANGE_IP_ACK        = 10011,        // �޸�IP��Ӧ
    EKDDEVFINDER_EVENT_CHANGE_INFO          = 10020,        // �޸��豸��Ϣ
    EKDDEVFINDER_EVENT_CHANGE_INFO_ACK      = 10021,        // �޸��豸��Ϣ��Ӧ
    EKDDEVFINDER_EVENT_CHANGE_CHECKIN       = 10030,        // �豸����
    EKDDEVFINDER_EVENT_CHANGE_CHECKIN_ACK   = 10031,        // �豸������Ӧ
    EKDDEVFINDER_EVENT_CHANGE_CHECKOUT      = 10032,        // �豸����
    EKDDEVFINDER_EVENT_CHANGE_CHECKOUT_ACK  = 10033,        // �豸������Ӧ
    EKDDEVFINDER_EVENT_CHANGE_DATA          = 10040,        // �޸��豸����
    EKDDEVFINDER_EVENT_CHANGE_DATA_ACK      = 10041,        // �޸��豸���ݻ�Ӧ
}   EKDDEVFINDER_EVENT;									// �¼�

typedef struct
{
    s8 achUUID[KDDEVFINDER_DEFAULT_UUID_LEN + 1];               // �豸Ψһ��� 32λUUID
    s8 achName[KDDEVFINDER_DEFAULT_STRING_LEN];             // �豸����
    s8 achType[KDDEVFINDER_DEFAULT_STRING_LEN];             // �豸����
    s8 achManufacturer[KDDEVFINDER_DEFAULT_STRING_LEN];     // �豸������Ϣ
}   TKDDEVFINDER_DEVICE_INFO;                           // �豸������Ϣ

typedef struct
{
    s8 achInterface[KDDEVFINDER_DEFAULT_STRING_LEN];    // �����ӿ���
    s8 achIP[KDDEVFINDER_DEFAULT_STRING_LEN];           // IP��ַ
    s32 nSubmaskLen;                                    // �������볤��
    s8 achGateway[KDDEVFINDER_DEFAULT_STRING_LEN];      // ����
}   TKDDEVFINDER_DEVICE_IP;                         // �豸IP��Ϣ


typedef struct
{
    s8 achName[KDDEVFINDER_DEFAULT_STRING_LEN];         // ��չ��������
    s8 achValue[KDDEVFINDER_DEFAULT_LONG_STRING_LEN];   // ��չ����ֵ
}   TKDDEVFINDER_DEVICE_EXT_DATA;                   // ��չ����

typedef struct
{
    TKDDEVFINDER_DEVICE_EXT_DATA*   ptExtDataList;                                      // ��չ�����б�                         
    s32                             nExtDataListSize;                                   // ��չ�����б���
}   TKDDEVFINDER_DEVICE_DATA;                                                   // �豸����

typedef struct
{
    s8                              achCheckinServerIP[KDDEVFINDER_DEFAULT_STRING_LEN]; // ����������IP��ַ
    s32                             nCheckinServerSubmaskLen;                           // �����������������볤��
}   TKDDEVFINDER_DEVICE_CHECKIN;                                                   // �豸������Ϣ

typedef struct
{
    TKDDEVFINDER_DEVICE_INFO*       ptDeviceInfo;   // �豸������Ϣ
    TKDDEVFINDER_DEVICE_IP*         ptDeviceIP;     // �豸IP
    TKDDEVFINDER_DEVICE_CHECKIN*    ptCheckin;      // �豸������Ϣ
    TKDDEVFINDER_DEVICE_DATA*       ptDeviceData;   // �豸����
    BOOL32                          bEnable;        // �Ƿ�����
    u32                             dwDeviceID;     // �豸��� �Է������Ч
}   TKDDEVFINDER_DEVICE;                     // �豸����

typedef struct
{
    u32                         dwSN;                               // �¼���ˮ��
    EKDDEVFINDER_RESULT         eSuccess;                           // ACK�ɹ���ʶ������ACK��Ч
    u32                         dwInterfaceIp;
    u32                         dwRemoteIp;
    union
    {
        TKDDEVFINDER_DEVICE_INFO*   ptSearch;           // EKDDEVFINDER_EVENT_SEARCH            ��������
        TKDDEVFINDER_DEVICE*        ptSearchACK;        // EKDDEVFINDER_EVENT_SEARCH_ACK        �������
        s32                         nSearchSum;         // EKDDEVFINDER_EVENT_SEARCH_FINISH     ��������
        TKDDEVFINDER_DEVICE*        ptChange;           // EKDDEVFINDER_EVENT_CHANGE_xxx        �޸��豸
        TKDDEVFINDER_DEVICE*        ptChangeACK;        // EKDDEVFINDER_EVENT_CHANGE_xxx_ACK    �޸��豸���
    };
}   TKDDEVFINDER_EVENT_ARG;                     // �¼�����

// �¼�����ص���������
typedef EKDDEVFINDER_RESULT (*KDDEVFIDER_EVENT_HANDLER)(EKDDEVFINDER_EVENT eEvent, TKDDEVFINDER_EVENT_ARG* ptArg, void* pvContext);

/*�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
  �UЭ��ջ                                                                      �U
  �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a*/

/* ==========================================================
   ������: KDDEVFIDER_InitStack
   ����:  ��ʼ��Э��ջ��Դ
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/27  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_InitStack();

/* ==========================================================
   ������: KDDEVFIDER_UninitStack
   ����:  �ͷ�Э��ջ��Դ
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/27  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_UninitStack();

/*�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
  �U�����                                                                      �U
  �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a*/

/* ==========================================================
   ������: KDDEVFIDER_StartServer
   ����: ��ʼ��������
   �㷨ʵ��: 
   ����˵��: KDDEVFIDER_EVENT_HANDLER pfHandler,  [IN] �¼��������ص�
             TKDDEVFINDER_DEVICE* ptDevice        [IN] �豸��ʼ��Ϣ
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   2012/07/23  1.0  ���              ���pvContext
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_StartServer(KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext , const TKDDEVFINDER_DEVICE* ptDevice);


/* ==========================================================
   ������: KDDEVFIDER_GetCurrentDevice
   ����:  ��ȡ��ǰ�豸����ĸ���
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice [OUT] �豸���󸱱�
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_GetCurrentDevice(TKDDEVFINDER_DEVICE* ptDevice);

/* ==========================================================
   ������: KDDEVFIDER_FreeCurrentDevice
   ����:  ����KDDEVFIDER_GetCurrentDevice��õ��豸���󸱱�
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice [IN] �豸���󸱱�
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_FreeCurrentDevice(TKDDEVFINDER_DEVICE* ptDevice);

/* ==========================================================
   ������: KDDEVFIDER_UpdateCurrentDevice
   ����: ���������豸����
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice �豸����
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_UpdateDevice(TKDDEVFINDER_DEVICE* ptDevice);


/* ==========================================================
   ������: KDDEVFIDER_StopServer
   ����:  ֹͣ��������
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_StopServer();


/*�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
  �U�ͻ���                                                                      �U
  �^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a*/
/* ==========================================================
   ������: KDDEVFIDER_StartClient
   ����: ��ʼ��������
   �㷨ʵ��: 
   ����˵��: KDDEVFIDER_EVENT_HANDLER pfHandler,  [IN] �¼��������ص�
             TKDDEVFINDER_DEVICE* ptDevice        [IN] �豸��ʼ��Ϣ
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   2012/07/23  1.0  ���              ���pvContext
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_StartClient(u32 dwInterfaceNum, u32* pdwInterfaceList);


/* ==========================================================
   ������: KDDEVFIDER_Search
   ����:  �����豸 ���ͨ���첽�ص�
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE_INFO* ptDeviceInfo, [IN] �豸����������Ϊ�������� NULL��ʾ������
             KDDEVFIDER_EVENT_HANDLER pfHandler,     [IN] �������ص�
             void* pvContext,                        [IN] �ص�������
             u32 dwTimeoutS                          [IN] ������ʱ ��λ��
             u32* pdwSN                              [OUT]��ˮ��
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Search(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE_INFO* ptDeviceInfo, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   ������: KDDEVFIDER_ChangeIP
   ����:  �޸��豸IP
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice,         [IN] �豸������Ҫ��д�豸ID �Լ��豸IP
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] �������ص�
             void* pvContext,                       [IN] �ص�������
             u32 dwTimeoutS                         [IN] ������ʱ ��λ��
             u32* pdwSN                             [OUT]��ˮ��
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeIP(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   ������: KDDEVFIDER_ChangeInfo
   ����:  �޸��豸������Ϣ
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice,         [IN] �豸������Ҫ��д�豸ID �Լ��豸������Ϣ
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] �������ص�
             void* pvContext,                       [IN] �ص�������
             u32 dwTimeoutS                         [IN] ������ʱ ��λ��
             u32* pdwSN                             [OUT]��ˮ�� 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeInfo(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   ������: KDDEVFIDER_Checkin
   ����:  �豸����
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice,         [IN] �豸������Ҫ��д�豸ID �Լ�������Ϣ
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] �������ص�
             void* pvContext,                       [IN] �ص�������
             u32 dwTimeoutS                         [IN] ������ʱ ��λ��
             u32* pdwSN                             [OUT]��ˮ�� 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Checkin(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   ������: KDDEVFIDER_Checkout
   ����: �豸����
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice,         [IN] �豸������Ҫ��д�豸ID
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] �������ص�
             void* pvContext,                       [IN] �ص�������
             u32 dwTimeoutS                         [IN] ������ʱ ��λ��
             u32* pdwSN                             [OUT]��ˮ�� 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Checkout(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   ������: KDDEVFIDER_ChangeData
   ����: �豸�����޸�
   �㷨ʵ��: 
   ����˵��: TKDDEVFINDER_DEVICE* ptDevice,         [IN] �豸������Ҫ��д�豸ID�Լ���չ����
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] �������ص�
             void* pvContext,                       [IN] �ص�������
             u32 dwTimeoutS                         [IN] ������ʱ ��λ��
             u32* pdwSN                             [OUT]��ˮ��
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeData(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);


/* ==========================================================
   ������: KDDEVFIDER_CleanSearch
   ����:  ����ϴ������������Դ������ҵ�����֮ǰ����
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_CleanSearch();

/* ==========================================================
   ������: KDDEVFIDER_StopClient
   ����:  ֹͣ��������
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_StopClient();

/* ==========================================================
   ������: KDDEVFIDER_SetLogCB
   ����:  ������־�ص�
   �㷨ʵ��: 
   ����˵��: 
   ����ֵ˵��: 
   ------------------------------------------------------------
   �޸ļ�¼:
   �� ��      �汾  �޸���  �߶���  �޸ļ�¼
   2012/07/19  1.0  ���              ����
   ===========================================================*/
typedef void (*KDDEVFINDER_LOG)(const char* szLog);
KDDEVFINDER_API void KDDEVFIDER_SetLogCB(KDDEVFINDER_LOG pfLogCB);

#endif //_KDDEVFINDER_H_  

