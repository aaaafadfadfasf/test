#ifndef _INNER_COMMON_H_
#define _INNER_COMMON_H_

#include "kdvtype.h"
#include "ospsip/ospsip.h"
#include "ospext/ospext.h"
#include "cmsassert.h"
#include <string>
using std::string;
#include "tas/tas_event.h"
#include "tas/tas_proto.h"
#include "tas/tas_struct.h"
#include "cms/cms_errorcode.h"


const u16 AID_TELEAPP = 3;

enum
{
    INNER_LOGIN = 10000, // 消息体 TLoginData
    INNER_LOGOUT,        // 消息体 空
    INNER_ONLINE,        // 消息体 TOnlineInfo
    INNER_OFFLINE,       // 消息体 cuSessid
    INNER_DEVMODELADD,   // 消息体 TDevModelAdd
    INNER_DEVMODELDEL,   // 消息体 devmodelid
    INNER_DEVMODELQRY,   // 消息体 空
    INNER_DEVADD,        // 消息体 TDevAddInfo
    INNER_DEVDEL,        // 消息体 devId
    INNER_DEVMOD,        // 消息体 TDevModInfo
    INNER_DEVQRY,        // 消息体 TDevQryInfo
    INNER_CUSERADD,      // 消息体 TCuserAddInfo
    INNER_CUSERDEL,      // 消息体 cuserid
    INNER_CUSERMOD,      // 消息体 TCuserModInfo
    INNER_CUSERQRY,      // 消息体 TCuserQryInfo
    INNER_AUTOTEST,      // 消息体 filename，功能：从文件中读取请求消息然后自动进行发送
};

struct TLoginData
{
    s32 loginType;
    s8 szName[20];
    s8 szPwd[20];
    s8 szExtra[20];
};

struct TOnlineInfo
{
    s8 szName[20];
    s8 szPwd[20];
};

struct TDevModelAdd
{
    s8 szName[32];
    s8 szDomainId[36];
    u32 dwDevType;
};

struct TDevAddInfo
{
    s8 szDevName[36];
    s8 szDomainId[36];
    s8 szModelId[36];
};
struct TDevModInfo
{
    s8 szDevId[36];
    s8 szDevNewName[36];
};

struct TDevQryInfo
{
    s32 nStartIndex;
    s32 nCount;
};

struct TCuserAddInfo
{
    s8 szUsername[36];
    s8 szCreatorId[36];
};

struct TCuserModInfo
{
    s8 szUserId[36];
    s8 szUserNewName[36];
};

struct TCuserQryInfo
{
    s32 nStartIndex;
    s32 nCount;
};

inline void TelePrint(const s8* cstrFormat, ...)
{
    va_list parmList;
    va_start(parmList, cstrFormat);

    s8 szBuff[1024*64];
    s32 nWriteBytes;
#ifdef _LINUX_
    nWriteBytes = vsnprintf(szBuff, sizeof(szBuff)-1, cstrFormat, parmList);
#else
    nWriteBytes = vsprintf(szBuff, cstrFormat, parmList);
#endif // _LINUX_
    va_end(parmList);

    if ((0 <= nWriteBytes) && ((u32)nWriteBytes+1 < sizeof(szBuff)))
    {
        OspPrintf(TRUE, FALSE, "%s\n", szBuff);
    }
    else if (-1 == nWriteBytes)
    {
        OspPrintf(TRUE, FALSE, "%s\n", "vsprintf error in TelePrint...");
    }
    else
    {
        OspPrintf(TRUE, FALSE, "%s\n", "the print content is too long in TelePrint...");
    }
}

bool IsRspType(u16 wMsgId);
bool IsQryNtfReqType(u16 wMsgId);
bool IsNtfReqType(u16 wMsgId);

string ReplaceSessid(const s8* cstrRawdata, const string &cstrNewSessid);
string ReplaceSeqno(const s8* cstrRawdata, u32 dwNewSeqno);

#endif // _INNER_COMMON_H_
