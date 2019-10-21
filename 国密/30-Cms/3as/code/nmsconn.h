/*****************************************************************************
   文件实现功能: 以网元身份接入网管服务器的管理
   作者        : huzhiyun
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/11   1.0        huzhiyun        创建
******************************************************************************/
#ifndef _NMSCONN_H_
#define _NMSCONN_H_

#include "cbb/nms/nmclib.h"

extern CNmcLib g_cNmcLib;

void KeyDisconnectionAlarm(bool bAlarm=true);
void KeyExpireRemindAlarm(bool bAlarm=true);
void KeyValidateNotpass(bool bAlarm=true);
void KeyLiscenseTooManyAlarm(bool bAlarm=true);
void KeyLicenseGetFileValidateNotpass(bool bAlarm=true);
#endif // _NMSCONN_H_
