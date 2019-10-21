#include "osp/osp.h"
#include "teleapp.h"
#include "inner_common.h"
#ifdef _MSC_VER
#define snprintf _snprintf
#endif // _MSC_VER

API void login(s32 loginType, const s8* pszName, const s8* pszPwd, const s8* pszExtra)
{
    TLoginData tLoginData;
    tLoginData.loginType = loginType;
    snprintf(tLoginData.szName, sizeof(tLoginData.szName), "%s", pszName);
    snprintf(tLoginData.szPwd, sizeof(tLoginData.szPwd), "%s", pszPwd);
    snprintf(tLoginData.szExtra, sizeof(tLoginData.szExtra), "%s", pszExtra);
    s32 nRet = OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_LOGIN, &tLoginData, sizeof(tLoginData));
    TelePrint("OspPost ret:%d\n", nRet);
}

API void l()
{
    login(3, "admin", "888888", "3acuri");
}

API void q()
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_LOGOUT);
}

API void online(const s8* cstrName, const s8* cstrPwd)
{
    TOnlineInfo tInfo;
    strcpy(tInfo.szName, cstrName);
    strcpy(tInfo.szPwd, cstrPwd);
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_ONLINE, &tInfo, sizeof(tInfo));
}

API void offline(const s8* cstrCuSessid)
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_OFFLINE, cstrCuSessid, strlen(cstrCuSessid)+1);
}

API void devmodeladd(const s8* cstrName, const s8* cstrDomainId, u32 dwDevTypeid)
{
    TDevModelAdd tInfo;
    strcpy(tInfo.szName, cstrName);
    strcpy(tInfo.szDomainId, cstrDomainId);
    tInfo.dwDevType = dwDevTypeid;

    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVMODELADD, &tInfo, sizeof(tInfo));
}

API void devmodeldel(const s8* cstrId)
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVMODELDEL, cstrId, strlen(cstrId)+1);
}

API void devmodelqry()
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVMODELQRY);
}

API void devadd(const s8* pszName, const s8* pszDomainId, const s8* pszModelId)
{
    TDevAddInfo tAddInfo;
    strcpy(tAddInfo.szDevName, pszName);
    strcpy(tAddInfo.szDomainId, pszDomainId);
    strcpy(tAddInfo.szModelId, pszModelId);
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVADD, &tAddInfo, sizeof(tAddInfo));
}

API void devdel(const s8* pszId)
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVDEL, pszId, strlen(pszId)+1);
}

API void devmod(const s8* pszId, const s8* pszNewName)
{
    TDevModInfo tModInfo;
    strcpy(tModInfo.szDevId, pszId);
    strcpy(tModInfo.szDevNewName, pszNewName);
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVMOD, &tModInfo, sizeof(tModInfo));
}

API void devqry(s32 nStartIndex, s32 nCount)
{
    TDevQryInfo tQryInfo;
    tQryInfo.nStartIndex = nStartIndex;
    tQryInfo.nCount = nCount;
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_DEVQRY, &tQryInfo, sizeof(tQryInfo));
}

API void help()
{
    TelePrint("login(logintype(1-cmu, 2-cui, 3-tac), name, password, moduleuri)");
    TelePrint("q -- logout");
    TelePrint("autotest(xmlfile) -- read req message from xmlfile and send it");
    TelePrint("online(cuname, cupassword)");
    TelePrint("offline(cuSessid)");
    TelePrint("devmodeladd(name, domainid, devtypeid)");
    TelePrint("devmodeldel(modelid)");
    TelePrint("devmodelqry");
    TelePrint("devadd(devname, domainid, modelid)");
    TelePrint("devdel(devid)");
    TelePrint("devmod(devid, devnewname)");
    TelePrint("devqry(nstartindex, ncount)");

    TelePrint("");
}
API void quit()
{
    extern SEMHANDLE g_hTastestExitSem;
    OspSemGive(g_hTastestExitSem);
}

API void autotest(const s8* cstrfile)
{
    OspPost(MAKEIID(AID_TELEAPP, CInstance::DAEMON), INNER_AUTOTEST, cstrfile, strlen(cstrfile)+1);
}

void RegCmd()
{
#ifdef _LINUX_
    OspRegCommand("help", (void *)help, "help");
    OspRegCommand("quit", (void *)quit, "quit");
    OspRegCommand("login", (void *)login, "login(name, password, moduleuri)");
    OspRegCommand("l", (void*)l, "quick login");
    OspRegCommand("q", (void*)q, "logout");
    OspRegCommand("autotest", (void*)autotest, "autotest(xmlfile)");
    OspRegCommand("t", (void*)autotest, "quick autotest");
    OspRegCommand("online", (void*)online, "online(cuname, cupassword)");
    OspRegCommand("offline", (void*)offline, "offline(cuSessid)");
    OspRegCommand("devmodeladd", (void *)devmodeladd, "devmodeladd(name, domainid)");
    OspRegCommand("devmodeldel", (void *)devmodeldel, "devmodeldel(modelid)");
    OspRegCommand("devmodelqry", (void *)devmodelqry, "devmodelqry");
    OspRegCommand("devadd", (void *)devadd, "devadd(devname)");
    OspRegCommand("devdel", (void *)devdel, "devdel(devid)");
    OspRegCommand("devmod", (void *)devmod, "devmod(devid, devnewname)");
    OspRegCommand("devqry", (void *)devqry, "devqry(nstartindex, ncount)");
#endif // _LINUX_
}