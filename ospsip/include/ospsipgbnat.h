#ifndef _OSPSIPGBNAT_H_
#define _OSPSIPGBNAT_H_
/*功能：国标NAT穿透相关功能 */

#include "cms/ospsip/ospsip.h"
#include "ospsipconvert.h"

void OspSipConvert2ExtraNetAddr(const TSipExtraInfo* ptSipExtraInfo, const string &strIntranetIp, u16 wIntranetPort, 
    string &strExtranetIp, u16 &wExtranetPort);

void OspSipConvert2ExtraNetAddr(TOspSipSendMsg *ptSipSendMsg, bool bConvertFrom, bool bConvertContact, bool bConvertSdp);

void OspSipGbNatDump();

#endif // _OSPSIPGBNAT_H_
