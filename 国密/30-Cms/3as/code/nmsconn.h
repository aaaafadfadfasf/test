/*****************************************************************************
   �ļ�ʵ�ֹ���: ����Ԫ��ݽ������ܷ������Ĺ���
   ����        : huzhiyun
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/11   1.0        huzhiyun        ����
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
