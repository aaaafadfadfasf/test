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
    INNER_LOGIN = 10000, // ��Ϣ�� TLoginData
    INNER_LOGOUT,        // ��Ϣ�� ��
    INNER_ONLINE,        // ��Ϣ�� TOnlineInfo
    INNER_OFFLINE,       // ��Ϣ�� cuSessid
    INNER_DEVMODELADD,   // ��Ϣ�� TDevModelAdd
    INNER_DEVMODELDEL,   // ��Ϣ�� devmodelid
    INNER_DEVMODELQRY,   // ��Ϣ�� ��
    INNER_DEVADD,        // ��Ϣ�� TDevAddInfo
    INNER_DEVDEL,        // ��Ϣ�� devId
    INNER_DEVMOD,        // ��Ϣ�� TDevModInfo
    INNER_DEVQRY,        // ��Ϣ�� TDevQryInfo
    INNER_CUSERADD,      // ��Ϣ�� TCuserAddInfo
    INNER_CUSERDEL,      // ��Ϣ�� cuserid
    INNER_CUSERMOD,      // ��Ϣ�� TCuserModInfo
    INNER_CUSERQRY,      // ��Ϣ�� TCuserQryInfo
    INNER_AUTOTEST,      // ��Ϣ�� filename�����ܣ����ļ��ж�ȡ������ϢȻ���Զ����з���
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
