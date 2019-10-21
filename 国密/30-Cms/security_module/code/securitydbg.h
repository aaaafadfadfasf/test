#ifndef _SECURITY_DBG_H
#define _SECURITY_DBG_H


#include "mesg.h"
#include "cms/ospext/ospdbg.h"
#include "conf.h"
#include "cms/security_module/security_module_event.h"
#include "cms/security_module/security_module_proto.h"
#include "cbb/osp/osp.h"

extern SEMHANDLE g_hSecurityModuleExitSem;

API void securityhelp();


void RegSecurityCmd();

API void securityver();
API void securitycfg();
API void logpara();
API void loglev(u8 byLev);
API void logall(u8 byLev);
API void logoff();
API void tasklist();
API void task(u16 wTaskNO);
API void tracet(u16 wTaskNO);
API void tracent();
API void securitystat();
API void securityquit();

API void newvkek();

#endif