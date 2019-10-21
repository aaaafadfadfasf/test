#ifndef _BASIC_TYPES_H_
#define _BASIC_TYPES_H_
#include "system/config/osconfig.h"
// define some basic types
typedef unsigned char  UInt8;
typedef unsigned short UInt16;
typedef unsigned int   UInt32;
#if defined( WIN32 )
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif

#ifdef WIN32
typedef SOCKET Socket;
typedef unsigned int FDSizeType;
typedef char BufType;
#else
typedef int Socket;
typedef int FDSizeType;
typedef unsigned char BufType;
#endif

#endif

//end of file


