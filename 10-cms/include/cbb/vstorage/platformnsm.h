/*****************************************************************************
    ģ���� ��NSM
    �ļ��� ��
    ����ļ� ��
    �ļ�ʵ�ֹ��ܣ�
    ����        ����Ф��
    �汾        ��V1.0  Copyright(C) 2007-2008 KDC, All rights reserved.
------------------------------------------------------------------------------
    �޸ļ�¼:
    ��  ��		�汾		�޸���		�߶���      �޸�����
    2010/12/24  1.0         ��Ф��                     ����
******************************************************************************/
#ifndef __PLAT_NSM__H
#define __PLAT_NSM__H
#include "kdvtype.h"


#define NruStatus_Unconnected    1   /* NRU �Ѿ����У�����δ���ӵ�ƽ̨ */
#define NruStatus_Connected      2   /* NRU �Ѿ����У��������ӵ�ƽ̨ */

#define VtduStatus_Unconnected   1   /* VTDU �Ѿ����У�����δ���ӵ�ƽ̨ */
#define VtduStatus_Connected     2   /* VTDU �Ѿ����У��������ӵ�ƽ̨ */

#define NSM_MAGIC     0x1234    //magic number
#define NSMVERSION    0x0001    //�汾��
#define MAX_NSMMSG_SIZE 1200    //�����Ϣ��������
#define NSMNRUPORT      9005    //NRU��NSMͨ�Ŷ˿�
#define NSMVTDUPORT     9006    //VTDU��NSMͨ�Ŷ˿�

#define NsmDisk_FS_Raw          0   /*����δ��ʽ��*/
#define NsmDisk_FS_Ext3         1   /*���̸�ʽ��Ϊext3*/
#define NsmDisk_FS_Vbfs         2   /*���̸�ʽ��Ϊvbfs*/

typedef struct 
{
	u32  dwNruStatus;//NRU״̬
	s8   szVersion[64]; /* �汾�ַ��� */
}TNruToNsmStatus;

typedef struct 
{
	u32  dwVtduStatus;//VTDU״̬
}TVtduToNsmStatus;

typedef union tagLargeInteger
{
    struct
    {
        unsigned int    dwLowPart;
        unsigned int    dwHighPart;
    };
#ifdef _WIN32
    unsigned __int64    qwQuadPart;
#else
    unsigned long long  qwQuadPart;
#endif /* _WIN32 */
} TLargeInteger;


//NRU���̽ṹ
typedef struct
{
    s8                     m_szuuid[64];        /* ����Ψһ��ʶ */
    s8                     m_szName[128];        /* �������� */
    TLargeInteger          m_qwTotalCapacity;   /* ����������in MB */
    TLargeInteger          m_qwFreeCapacity;    /* ����������in MB */
    u32                    m_dwError;           /* �쳣���� */
    u32                    m_dwTotalRecords;    /* ¼���� */
    u32                    m_dwIoRatio;         /* IO������ */
    u32                    m_dwState;           /* ״̬ */
    u32                    m_dwFileSystemType;  /* �ļ�ϵͳ��ʽ */
    s8                     m_szMountPoint[64];  /* ����·�� */
    u32                    m_dwReserved[6];
} TNruNsmDisk;

typedef enum
{
	E_DBStatus_OK = 0,
	E_DBStatus_TableDestroyed = 1,
	E_DBStatus_LoseConnection = 2,
}ENruDBStatus;

typedef struct 
{
	u32           m_dwDBStatus;     //ȡֵΪENruDBStatus
	TLargeInteger m_llDBDestroyedOrRepairedTime; //���ݿ��𻵻����޸�ʱ��
	u8      m_bReserved[12];  //�����ֶ�
}TNruNsmDBStatus;

enum NruNsmOpCode
{
    NruNsmOpCode_Begin = 4000,
	//ע��4001
	NruNsmOpCode_Register,
    //��ѯNRU״̬4002
    NruNsmOpCode_NruStatus,
    //��ȡ������Ϣ4003
    NruNsmOpCode_GetDiskInfo,

	//1.0 nru��ȡnru ID, 2.0 nruδʹ��
    NruNsmOpCode_GetNruID,
    
    //���ݿ�״̬֪ͨ 4005
    NruNsmOpCode_NotifyDBState,
    
    NruNsmOpCode_End,
};

enum VtduNsmOpCode
{
    VtduNsmOpCode_Begin = 5000,
	//ע��5001
	VtduNsmOpCode_Register,
    //��ѯVTDU״̬5002
    VtduNsmOpCode_VtduStatus,

    VtduNsmOpCode_End,
};

#endif /* __PLAT_NSM__H */


