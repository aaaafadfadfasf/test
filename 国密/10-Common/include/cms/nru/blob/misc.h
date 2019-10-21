#ifndef MISC_H_
#define MISC_H_

#ifdef _WIN32
#if _MSC_VER < 1500	

#define COM_VC6 1

#include "kdvtype.h"
typedef s8 int8;
typedef s16 int16;
typedef s32 int32;
typedef s64 int64;

typedef u8 uint8;
typedef u16 uint16;
typedef u32 uint32;
typedef u64 uint64;
#endif
#endif


#ifndef COM_VC6
#include <stdint.h>

typedef int8_t int8;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

#include <string>
#include <vector>

#endif /* MISC_H_ */
