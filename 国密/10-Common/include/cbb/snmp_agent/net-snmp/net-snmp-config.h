#ifndef NET_SNMP_CONFIG_PRE_H
#define NET_SNMP_CONFIG_PRE_H

#ifdef _MSC_VER
#include "../win_conf/net-snmp/net-snmp-config-win.h"
#else
#include "../linux_conf/net-snmp/net-snmp-config-linux.h"	
#endif

#endif  //end NET_SNMP_CONFIG_PRE_H