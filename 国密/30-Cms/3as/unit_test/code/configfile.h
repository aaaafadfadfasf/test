#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_
#include <string>
using std::string;
#include "inner_common.h"

string GetProxyIP_();
u16 GetProxyPort_();

string GetTasUri();
string GetSelfUri();
string GetLocalIP();
u16 GetLocalPort();
s32 GetSipStackSize();

#endif // _CONFIGFILE_H_
