#include "log_system.h"
#include <stdarg.h>
#include "inner_common.h"
#include "ospext/ospdbg.h"

API void logpara()
{
    OspShowLogParam();
}

API void taslog( ENLogSrc enSrc, u32 dwLogLevel )
{
    u8 byLogLevel = static_cast<u8>(dwLogLevel);
    u8 byModId;
    bool bAll = (enSrc == enLogSrcAll);

    if (bAll || OSPEXT == (s32)enSrc)
    {
        byModId = static_cast<u8>(OSPEXT);
        OspSetModLogLev(byModId, byLogLevel, "OSPEXT");
    }
    if (bAll || OSPSIP == (s32)enSrc)
    {
        byModId = static_cast<u8>(OSPSIP);
        OspSetModLogLev(byModId, byLogLevel, "OSPSIP");
        kdsiplog(byLogLevel);
    }
    if (bAll || enLogSrcSystem == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcSystem);
        OspSetModLogLev(byModId, byLogLevel, "Global");
    }
    if (bAll || enLogSrcNMSConn == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcNMSConn);
        OspSetModLogLev(byModId, byLogLevel, "NMSConnApp");
    }
    if (bAll || enLogSrcDbTaskPool == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcDbTaskPool);
        OspSetModLogLev(byModId, byLogLevel, "DbTaskPool");
    }

    if (bAll || enLogSrcCCtrl == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcCCtrl);
        OspSetLogLev(byLogLevel, AID_CCtrl, 0);
        OspSetModLogLev(byModId, byLogLevel, "CCtrlApp");
    }
    if (bAll || enLogSrcBackground == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcBackground);
        OspSetModLogLev(byModId, byLogLevel, "BackgroundApp");
    }
    if (bAll || enLogSrcLicense == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcLicense);
        OspSetModLogLev(byModId, byLogLevel, "LicenseApp");
    }
    if (bAll || enLogSrcSyncAnalyzer == enSrc)
    {
        byModId = static_cast<u8>(enLogSrcSyncAnalyzer);
        OspSetModLogLev(byModId, byLogLevel, "SyncAnalyzer");
    }
}

API void tla()
{
    taslog(enLogSrcAll, ALL_LEV);
}

API void tle()
{
    taslog(enLogSrcAll, ERROR_LEV);
}

API void logall( u32 dwLogLevel )
{
    taslog(enLogSrcAll, dwLogLevel);
}

API void logoff()
{
    taslog(enLogSrcAll, NOLOG_LEV);
}

API void cctrllog( u32 dwLevel )
{
    taslog(enLogSrcCCtrl, dwLevel);
}

API void dblog( u32 dwLevel )
{
    taslog(enLogSrcDbTaskPool, dwLevel);
}

API void nmslog( u32 dwLevel )
{
    taslog(enLogSrcNMSConn, dwLevel);
}

API void licenselog( u32 dwLevel )
{
    taslog(enLogSrcLicense, dwLevel);
}
