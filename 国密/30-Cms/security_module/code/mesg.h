#ifndef _MESG_H_
#define _MESG_H_
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_eventid.h"
#include "cms/ospext/ospext.h"


const u32 AID_COREAPP = 400;
const u32 COREAPPIID = MAKEIID(AID_COREAPP, CInstance::DAEMON);

enum INNERMSG
{
    ModulePowon=USER_INNER_MSG_BEGIN,
    ModulePowoff,
	ModulePrintStat,


};








#endif // _MESG_H_
