#include "configfile.h"
#include "kdvsys.h"

static bool _GetString(const s8* pcKey, string &strResult, const s8* pcDefault="" )
{
    s8 szBuf[1024];

    if (GetRegKeyString("tas_test.conf", "conf", pcKey, pcDefault, szBuf, sizeof(szBuf)))
    {
        szBuf[sizeof(szBuf)-1] = '\0';
        strResult = szBuf;
        printf("string conf value:%s\n", szBuf);
        return true;
    }
    else
    {
        return false;
    }
}

static bool _GetInt( const s8* pcKey, s32 &nResult, s32 nDefault=0 )
{
    if (GetRegKeyInt("tas_test.conf", "conf", pcKey, nDefault, &nResult))
    {
        return true;
    }
    else
    {
        return false;
    }
}

string GetProxyIP_()
{
    string str;
    _GetString("proxy_ip", str, "172.16.64.99");
    return str;
}

u16 GetProxyPort_()
{
    s32 n;
    _GetInt("proxy_port", n, 5060);
    return n;
}

string GetTasUri()
{
    string str;
    _GetString("tas_uri", str, "3as@china");
    return str;
}

string GetSelfUri()
{
    string str;
    _GetString("self_uri", str, "tas_test@china");
    return str;
}

string GetLocalIP()
{
    string str;
    _GetString("localip", str, "172.16.251.57");
    return str;
}

u16 GetLocalPort()
{
    s32 n;
    _GetInt("localport", n, 33333);
    return n;
}

s32 GetSipStackSize()
{
    s32 n;
    _GetInt("sipstack_size", n, 1024*2);
    return n;
}
