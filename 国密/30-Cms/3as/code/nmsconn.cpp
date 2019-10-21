#include "nmsconn.h"

CNmcLib g_cNmcLib;
void SendAlarm(u32 dwAlarmCode, u32 dwAlarmSerialNo, bool bAlarm)
{
    u8 byAlarm = bAlarm? TNM_STATUS_ALARM: TNM_STATUS_NORMAL;
    TTnmDevAlarm tAlarm;
    tAlarm.dwAlarmCode = dwAlarmCode;
    tAlarm.dwDevSerialNo = dwAlarmSerialNo;
    tAlarm.HostToNet();

    u8 abyBuf[sizeof(tAlarm)+sizeof(byAlarm)];
    memcpy(abyBuf, &tAlarm, sizeof(tAlarm));
    memcpy(abyBuf+sizeof(tAlarm), &byAlarm, sizeof(byAlarm));
    g_cNmcLib.SendMsg(TRAP_TNM_DEV_ALARM, abyBuf, sizeof(abyBuf));
}


void KeyDisconnectionAlarm( bool bAlarm/*=true*/ )
{
    SendAlarm(TNM_ALARM_CODE_DISCONNECT_USBKEY, 1, bAlarm);
}

void KeyExpireRemindAlarm( bool bAlarm/*=true*/ )
{
    SendAlarm(TNM_ALARM_CODE_AT_TERM_REMIND_USBKEY, 2, bAlarm);
}

void KeyValidateNotpass( bool bAlarm/*=true*/ )
{
    SendAlarm(TNM_ALARM_CODE_LICENSE_VALIDATE_NOT_PASS, 3, bAlarm);
}
void KeyLiscenseTooManyAlarm( bool bAlarm/*=true*/ )
{
	SendAlarm(TNM_ALARM_CODE_MANAGE_LICENSE_TOO_MUCH, 4, bAlarm); 
}
void KeyLicenseGetFileValidateNotpass(bool bAlarm/*=true*/)
{
    SendAlarm(TNM_ALARM_CODE_TAS_LICENSE_GETFILE_VALIDATE_NOT_PASS,5,bAlarm);

}
