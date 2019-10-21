/*****************************************************************************
    模块名 ：NSM
    文件名 ：
    相关文件 ：
    文件实现功能：
    作者        ：顾肖康
    版本        ：V1.0  Copyright(C) 2007-2008 KDC, All rights reserved.
------------------------------------------------------------------------------
    修改记录:
    日  期		版本		修改人		走读人      修改内容
    2010/12/24  1.0         顾肖康                     创建
******************************************************************************/
#ifndef __PLAT_NSM__H
#define __PLAT_NSM__H
#include "kdvtype.h"


#define NruStatus_Unconnected    1   /* NRU 已经运行，但还未连接到平台 */
#define NruStatus_Connected      2   /* NRU 已经运行，并已连接到平台 */

#define VtduStatus_Unconnected   1   /* VTDU 已经运行，但还未连接到平台 */
#define VtduStatus_Connected     2   /* VTDU 已经运行，并已连接到平台 */

#define NSM_MAGIC     0x1234    //magic number
#define NSMVERSION    0x0001    //版本号
#define MAX_NSMMSG_SIZE 1200    //最大消息附带长度
#define NSMNRUPORT      9005    //NRU与NSM通信端口
#define NSMVTDUPORT     9006    //VTDU与NSM通信端口

#define NsmDisk_FS_Raw          0   /*磁盘未格式化*/
#define NsmDisk_FS_Ext3         1   /*磁盘格式化为ext3*/
#define NsmDisk_FS_Vbfs         2   /*磁盘格式化为vbfs*/

typedef struct 
{
	u32  dwNruStatus;//NRU状态
	s8   szVersion[64]; /* 版本字符串 */
}TNruToNsmStatus;

typedef struct 
{
	u32  dwVtduStatus;//VTDU状态
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


//NRU磁盘结构
typedef struct
{
    s8                     m_szuuid[64];        /* 磁盘唯一标识 */
    s8                     m_szName[128];        /* 磁盘名称 */
    TLargeInteger          m_qwTotalCapacity;   /* 磁盘容量，in MB */
    TLargeInteger          m_qwFreeCapacity;    /* 空闲容量，in MB */
    u32                    m_dwError;           /* 异常次数 */
    u32                    m_dwTotalRecords;    /* 录像数 */
    u32                    m_dwIoRatio;         /* IO利用率 */
    u32                    m_dwState;           /* 状态 */
    u32                    m_dwFileSystemType;  /* 文件系统格式 */
    s8                     m_szMountPoint[64];  /* 挂载路径 */
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
	u32           m_dwDBStatus;     //取值为ENruDBStatus
	TLargeInteger m_llDBDestroyedOrRepairedTime; //数据库损坏或者修复时间
	u8      m_bReserved[12];  //保留字段
}TNruNsmDBStatus;

enum NruNsmOpCode
{
    NruNsmOpCode_Begin = 4000,
	//注册4001
	NruNsmOpCode_Register,
    //查询NRU状态4002
    NruNsmOpCode_NruStatus,
    //获取磁盘信息4003
    NruNsmOpCode_GetDiskInfo,

	//1.0 nru获取nru ID, 2.0 nru未使用
    NruNsmOpCode_GetNruID,
    
    //数据库状态通知 4005
    NruNsmOpCode_NotifyDBState,
    
    NruNsmOpCode_End,
};

enum VtduNsmOpCode
{
    VtduNsmOpCode_Begin = 5000,
	//注册5001
	VtduNsmOpCode_Register,
    //查询VTDU状态5002
    VtduNsmOpCode_VtduStatus,

    VtduNsmOpCode_End,
};

#endif /* __PLAT_NSM__H */


