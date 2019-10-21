#ifndef _VSIPUDP_H_
#define _VSIPUDP_H_

#include "kdvtype.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

typedef enum
{
    // print level	
    VSIPUDPDEBUG        =    0x04,
    VSIPUDPWARN         =    0x02, 
    VSIPUDPERROR        =    0x01, 
} EVsipUdpLogLevel;

// �豸��
typedef struct
{
    u32    m_dwData1;
    u16    m_wData2;
    u16    m_wData3;
    u8     m_abyData4[8];
} TVsipGuid;

// �豸��Ϣ
typedef struct 
{
    TVsipGuid   m_tGuid;

    u32     m_dwIpAddress;
    u16     m_wPort;
    
    u8      m_byVerionID;                       // �汾ID
    BOOL32  m_bReachEnable;                     // �Ƿ�ɴ�

    s8      m_achName[64+1];                    // �豸��
    s8      m_achTypeName[64+1];                // �豸������(ֻ��)
    u8      m_byDevType;                        // �豸����(MTI_ENCODER��) 
    s8      m_achSeriesName[16+1];              // �豸ϵ����

    u32     m_dwNetMask;
    u32     m_dwGateway;
} TVsipUnitInfo;


// �豸״̬��Ϣ
typedef enum
{
    EVsipUdp_UnitInfoType_Up,             // �豸����
    EVsipUdp_UnitInfoType_Down,           // �豸����
    EVsipUdp_UnitInfoType_Update,         // �豸��Ϣ����
    EVsipUdp_UnitInfoType_Delete,         // �豸ɾ��

} EVsipUdp_UnitInfoType;

// �豸״̬��Ϣ�ص�
typedef s32 (*FEvVsipUdp_UnitInfo)(TVsipUnitInfo* ptUnit, EVsipUdp_UnitInfoType eType, void* pContext);

typedef struct 
{
    u32                     m_dwMultiIp;            // �鲥��ַ
    u16                     m_wMultiPort;           // �鲥�˿�

    s32                     m_nUnitTTL;             // �豸�������(s)

    FEvVsipUdp_UnitInfo     m_pfUnitInfo;           // �豸��Ϣ�ص�
    void*                   m_pUnitInfoContext;     // �ص�������

} TVsipUdp_Config;


#define E_VSIPUDP_NO_ERROR                   ((s32)(0))
#define E_VSIPUDP_BASE_ERROR                 ((s32)(-3000))
#define E_VSIPUDP_PARAM_ERROR                ((s32)(E_VSIPUDP_BASE_ERROR-1))    //��������
#define E_VSIPUDP_HANDLE_ERROR               ((s32)(E_VSIPUDP_BASE_ERROR-2))    //�����Ч
#define E_VSIPUDP_NOT_CREATE                 ((s32)(E_VSIPUDP_BASE_ERROR-3))    //�޴���
#define E_VSIPUDP_ALREADY_CREATE             ((s32)(E_VSIPUDP_BASE_ERROR-4))    //�Ѵ���

// ��ʱ�����ڲ�����ȫ�ֶ���
s32 VsipUdp_Init(TVsipUdp_Config* ptConfig);

// ���١��ͷ��ڲ�ȫ�ֶ����ͷ������ڴ�
s32 VsipUdp_UnInit();

// ���á�����豸���в���������
s32 VsipUdp_Reset();

// �����ϱ������豸��Ϣ
s32 VsipUdp_ReReport();

// �ֹ�����һ��
s32 VsipUdp_Search();

// �ֹ������豸
s32 VsipUdp_ManualAdd(u32 dwIP, u16 wPort);

// �ֹ�ɾ���豸
s32 VsipUdp_ManualDel(u32 dwIP, u16 wPort);

// ����IP
s32 VsipUdp_ChangeIp(TVsipGuid* ptGuid, u32 dwSrcIP,  
                 u32 dwNewIP, u32 dwNewMask, u32 dwNewGW);

// ���ô�ӡ����
s32 VsipUdpSetPrint(u32 dwLevel);


//#ifdef __cplusplus
//}
//#endif


#endif // _VSIPUDP_H_
