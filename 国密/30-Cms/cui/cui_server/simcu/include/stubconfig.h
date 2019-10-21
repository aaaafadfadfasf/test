#ifndef _STUB_CONFIG_H_
#define _STUB_CONFIG_H_


#include "cms/container/xmap.h"
#include "cms/ospext/ospext.h"

typedef CXMap<int, string> CEvToMsgMap;

extern CEvToMsgMap g_cEventToMsgMap;

void LoadMsg(const string &strDir);


#endif
