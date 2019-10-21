#ifndef _INTERNAL_H_
#define _INTERNAL_H_

#include "common.h"
#include "vsipudp.h"

DECLARE_HANDLE(HHash);

extern u32 g_dwVsipUdpPrtLevel;

#define PRT_DEBUG if ((g_dwVsipUdpPrtLevel&VSIPUDPDEBUG))   OspPrintf
#define PRT_WARN  if ((g_dwVsipUdpPrtLevel&VSIPUDPWARN))    OspPrintf
#define PRT_ERROR if ((g_dwVsipUdpPrtLevel&VSIPUDPERROR))   OspPrintf


typedef struct tagUnit TUnit;

typedef enum
{
    EUnitState_OffLine = 0,
    EUnitState_OnLine  = 1,
    EUnitState_Update  = 2,
    EUnitState_Delete  = 3,

} EUnitState;

typedef enum
{
    EUnicastCheckState_NOCHECK  = 0,
    EUnicastCheckState_CHECKED  = 1,

} EUnicastCheckState;

typedef enum
{
    EUnitSysInfoState_NOCHECK       = 0,
    EUnitSysInfoState_CHECKED_HALF  = 1,
    EUnitSysInfoState_CHECKED       = 2,

} EUnitSysInfoState;

struct tagUnit
{
    TUnit*          m_ptPrev;
    TUnit*          m_ptNext;

    TVsipUnitInfo   m_tUnit;
    BOOL32          m_bReported;
    EUnitState      m_eState;
    
    BOOL32          m_bValid;
    BOOL32          m_bManual;

    // SysInfo Check
    EUnitSysInfoState m_eSysInfoState;
    s32               m_nSysInfoCheckCount;

    // Unicast Check
    EUnicastCheckState m_eUnicastCheckState;
    s32                m_nUnicastCheckCount;

    // TTL Check
    BOOL32          m_bTTLCheck;
    s32             m_nTTL;
    
    // Request
    BOOL32          m_bWaitRequestAck;
    u16             m_wTransNum;
    s32             m_nWaitTime;
    s32             m_nResendTimes;
    
};

typedef struct tagManaulUnit TManualUnit;

struct tagManaulUnit
{
    TManualUnit*            m_ptPrev;
    TManualUnit*            m_ptNext;
    
    u32                     m_dwIP;
    u16                     m_wPort;
};


typedef struct 
{
    BOOL32                  m_bInit;
    BOOL32                  m_bReset;
    u32                     m_dwTaskState;

    // config
    u32                     m_dwMultiIp;
    u16                     m_wMultiPort;

    u32                     m_dwLocalIp;

    s32                     m_nUnitTTL;    //s
    s32                     m_nResendTimes;
    BOOL32                  m_bDelOffline;

    FEvVsipUdp_UnitInfo     m_pfUnitInfo;
    void*                   m_pUnitInfoContext;

    // message
    SOCKHANDLE              m_hMultiSocket; // INVALID_SOCKET;
    SOCKHANDLE              m_hSocket; 
    u16                     m_wUnicastPort;
    u16                     m_wTransNum;
    
    // unit list
    TUnit*                  m_ptFreeList;
    TUnit*                  m_ptUsedList;
    TUnit*                  m_ptLastCheck;

    HHash                   m_hAddrHash;
    s32                     m_nUnitNum;

    // manual unit list
    TManualUnit*            m_ptManualUnitArray;
    TManualUnit*            m_ptManualUnitUsedList;
    TManualUnit*            m_ptManualUnitFreeList;
    

    // Semphone info
    SEMHANDLE               m_hSem;
    const char*             m_pSemFile;
    s32                     m_nSemLine;

    TASKHANDLE              m_hSocketTask;
    TASKHANDLE              m_hMgrTask;

} TUnitMgr;


typedef struct 
{
    u8* m_pRawBuff;
    s32 m_nRawBuffLen;
} TRawMsg;

typedef struct tagVsipStructMsg
{
    s32 m_nType;
    u8* m_pStruct;
    s32 m_nStructLen;
    struct tagVsipStructMsg* m_pSub;
} TVsipStructMsg;


/************************************************************************/
/* Mgr                                                                  */
/************************************************************************/
s32 UnitMgrInit(TUnitMgr* ptMgr, TVsipUdp_Config* ptConfig);
s32 UnitMgrUnInit(TUnitMgr* ptMgr);

/*  Unit list manage */
s32 UnitMgrSearch(TUnitMgr* ptMgr);
s32 UnitMgrReset(TUnitMgr* ptMgr);
s32 UnitMgrReReport(TUnitMgr* ptMgr);
s32 UnitMgrManual_Add(TUnitMgr* ptMgr, u32 dwIP, u16 wPort);
s32 UnitMgrManual_Del(TUnitMgr* ptMgr, u32 dwIP, u16 wPort);
s32 UnitMgrSetIp(TUnitMgr* ptMgr, TVsipGuid* ptGuid, u32 dwTargetIP, 
                 u32 dwNewIP, u32 dwNewMask, u32 dwNewGW);

/*  task */
s32 UnitMgrMsgProcess(TUnitMgr* ptMgr, TVsipStructMsg* ptStruct, u32 dwPeerIp, u16 wPeerPort, BOOL32 bMulti);
s32 UnitMgrTimerProcess(TUnitMgr* ptMgr);

/*  message */
s32 MsgRecvProcess(TUnitMgr* ptMgr, s32 nWaitTime);
s32 VsipMsgParse(TRawMsg* ptRaw, TVsipStructMsg* ptStruct);
s32 VsipMsgBuild(TRawMsg* ptRaw, TVsipStructMsg* ptStruct);
s32 VsipMsgBuildAndSend(SOCKHANDLE hUdp, s32 nVsipMsgType, u16 wTransNum, u8* pStruct, s32 nStructLen, u32 dwPeerIp, u16 wPeerPort);

/*  debug */
s32 VsipMgrPrintInfo(TUnitMgr* ptMgr);


/************************************************************************/
/* Hash                                                                 */
/************************************************************************/

typedef u32 (*HashFunc)(void* pValue, void* hUserHandle);
typedef BOOL32 (*HashCheckSameFunc)(void* pValue1, void* pValue2, void* hUserHandle);

typedef struct 
{
    s32 m_nMaxHashNum;
    s32 m_nMaxTotalNum;
    HashFunc m_pfHash;
    HashCheckSameFunc m_pfCheckSame;
    void* m_hUserHandle;
} THashParam;

s32 HashCreate(HHash* phHash, THashParam* ptParam);
s32 HashAdd(HHash hHash, void* pValue);
s32 HashAdd_DelSameValue(HHash hHash, void* pValue);
s32 HashDel(HHash hHash, void* pValue);
void* HashFind(HHash hHash, void* pValue);
s32 HashClear(HHash hHash);
s32 HashFree(HHash* phHash);

s32 HashPrint(HHash hHash);

/************************************************************************/
/* Sem                                                                  */
/************************************************************************/
void iVsipSemTakeDebug(SEMHANDLE hSem, const char** ppFile, s32* pnLine, const char* pszFile, s32 nLine);
void iVsipSemGiveDebug(SEMHANDLE hSem, const char* pszFile, s32 nLine);


/************************************************************************/
/* UDP                                                                  */
/************************************************************************/
SOCKHANDLE UdpOpen(u32 dwIpAddr, u16 wPort, BOOL32 bRaw);

s32 UdpSetTtl(SOCKHANDLE hSocket, u8 byTTL);

s32 UdpSetMultiSend(SOCKHANDLE hSocket, u32 dwInterfaceIP);
s32 UdpSetMultiRecv(SOCKHANDLE hSocket, u32 dwMultiAddr, u32 dwInterfaceIP);
s32 UdpSetMultiRecvDrop(SOCKHANDLE hSocket, u32 dwMultiAddr, u32 dwInterfaceIP);
s32 UdpSetBroadCast(SOCKHANDLE hSocket);

s32 UdpSend(SOCKHANDLE hSocket, s8* achBuff, s32 nLen);
s32 UdpSendTo(SOCKHANDLE hSocket, s8* achBuff, s32 nLen, u32 dwDstIP, u16 wDstPort);

s32 UdpRecv(SOCKHANDLE hSocket, s8* achBuff, s32 nLen);
s32 UdpRecvFrom(SOCKHANDLE hSocket, s8* achBuff, s32 nBuffLen, u32* pdwPeerIp, u16* pwPeerPort);

s32 UdpClose(SOCKHANDLE hSocket);

s32 UdpNoBlock(SOCKHANDLE hSocket, BOOL32 bOn);

s32 UdpGetLocalAddr(SOCKHANDLE hSocket, u32* pdwLocalIP, u16* pwLocalPort);

s32 UdpSetRecvBuff(SOCKHANDLE hSocket, s32 nRecvBuffSize);
s32 UdpSetSendBuff(SOCKHANDLE hSocket, s32 nSendBuffSize);
s32 UdpGetRecvBuff(SOCKHANDLE hSocket);
s32 UdpGetSendBuff(SOCKHANDLE hSocket);

#endif // _INTERNAL_H_
