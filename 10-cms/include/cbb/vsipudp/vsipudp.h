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

// 设备号
typedef struct
{
    u32    m_dwData1;
    u16    m_wData2;
    u16    m_wData3;
    u8     m_abyData4[8];
} TVsipGuid;

// 设备信息
typedef struct 
{
    TVsipGuid   m_tGuid;

    u32     m_dwIpAddress;
    u16     m_wPort;
    
    u8      m_byVerionID;                       // 版本ID
    BOOL32  m_bReachEnable;                     // 是否可达

    s8      m_achName[64+1];                    // 设备名
    s8      m_achTypeName[64+1];                // 设备类型名(只读)
    u8      m_byDevType;                        // 设备类型(MTI_ENCODER等) 
    s8      m_achSeriesName[16+1];              // 设备系列名

    u32     m_dwNetMask;
    u32     m_dwGateway;
} TVsipUnitInfo;


// 设备状态信息
typedef enum
{
    EVsipUdp_UnitInfoType_Up,             // 设备上线
    EVsipUdp_UnitInfoType_Down,           // 设备下线
    EVsipUdp_UnitInfoType_Update,         // 设备信息更新
    EVsipUdp_UnitInfoType_Delete,         // 设备删除

} EVsipUdp_UnitInfoType;

// 设备状态信息回调
typedef s32 (*FEvVsipUdp_UnitInfo)(TVsipUnitInfo* ptUnit, EVsipUdp_UnitInfoType eType, void* pContext);

typedef struct 
{
    u32                     m_dwMultiIp;            // 组播地址
    u16                     m_wMultiPort;           // 组播端口

    s32                     m_nUnitTTL;             // 设备存活检测间隔(s)

    FEvVsipUdp_UnitInfo     m_pfUnitInfo;           // 设备信息回调
    void*                   m_pUnitInfoContext;     // 回调上下文

} TVsipUdp_Config;


#define E_VSIPUDP_NO_ERROR                   ((s32)(0))
#define E_VSIPUDP_BASE_ERROR                 ((s32)(-3000))
#define E_VSIPUDP_PARAM_ERROR                ((s32)(E_VSIPUDP_BASE_ERROR-1))    //参数错误
#define E_VSIPUDP_HANDLE_ERROR               ((s32)(E_VSIPUDP_BASE_ERROR-2))    //句柄无效
#define E_VSIPUDP_NOT_CREATE                 ((s32)(E_VSIPUDP_BASE_ERROR-3))    //无创建
#define E_VSIPUDP_ALREADY_CREATE             ((s32)(E_VSIPUDP_BASE_ERROR-4))    //已创建

// 初时化，内部建立全局对象
s32 VsipUdp_Init(TVsipUdp_Config* ptConfig);

// 销毁。释放内部全局对象，释放所有内存
s32 VsipUdp_UnInit();

// 重置。清除设备队列并发送搜索
s32 VsipUdp_Reset();

// 重新上报所有设备信息
s32 VsipUdp_ReReport();

// 手工搜索一次
s32 VsipUdp_Search();

// 手工增加设备
s32 VsipUdp_ManualAdd(u32 dwIP, u16 wPort);

// 手工删除设备
s32 VsipUdp_ManualDel(u32 dwIP, u16 wPort);

// 更改IP
s32 VsipUdp_ChangeIp(TVsipGuid* ptGuid, u32 dwSrcIP,  
                 u32 dwNewIP, u32 dwNewMask, u32 dwNewGW);

// 设置打印级别
s32 VsipUdpSetPrint(u32 dwLevel);


//#ifdef __cplusplus
//}
//#endif


#endif // _VSIPUDP_H_
