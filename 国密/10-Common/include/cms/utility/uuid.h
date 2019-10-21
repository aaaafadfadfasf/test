/*========================================================================================
模块名    ：uuid
文件名    ：uuid.h
相关文件  ：
实现功能  ：跨平台的UUID类
作者      ：liangli
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/07/21         V1.0              lianlgi                                新建文件
=========================================================================================*/
#ifndef _COMMON_UUID_H_
#define _COMMON_UUID_H_

#include <string>
using std::string;
#include "cms/cmsassert.h"

#ifdef _LINUX_
#include <uuid/uuid.h>
#include <string.h>
#else
#include <rpc.h>
//#include <rpcdce.h>
#endif

#define MAX_UUID_LEN        36
#define MAX_CMS_UUID_LEN    32     //为了兼容平台1.0版本的PU以及网管模块，迁就KDMNO长度，故将UUID中的四个'-'符号去掉
#define UUID_SEPARATOR      ("-")

inline const char *GenUUIDStr(char *szTmp)
{
#ifdef _LINUX_
    uuid_t uu;
    uuid_generate(uu);
    uuid_unparse(uu, szTmp);
#else
    UUID guid;
    UuidCreate(&guid);
    RPC_CSTR lpGuid = NULL;
    UuidToStringA(&guid, &lpGuid);
    strncpy(szTmp, (char *)lpGuid, MAX_UUID_LEN);
    RpcStringFreeA(&lpGuid);
#endif

    //将生成的UUID中的'-'符号去掉
    char szUUIDTmp[MAX_UUID_LEN+1] = {0};
    strncpy(szUUIDTmp, (char *)szTmp, MAX_UUID_LEN);
    char *pCh = strstr(szTmp, UUID_SEPARATOR);
    while ( NULL != pCh )
    {
        u32 dwOffSet = pCh - szTmp;                 //szTmp中的"-"符号偏移量
        char *pTmp = strstr(szUUIDTmp, UUID_SEPARATOR);
        CMS_ASSERT(NULL != pTmp);
        u32 dwTmpOffset = pTmp - szUUIDTmp;         //szUUIDTmp中第一个"-"符号偏移量
        strncpy(szUUIDTmp+dwTmpOffset, szTmp+dwOffSet+1, MAX_UUID_LEN-dwOffSet);
        pCh = strstr(szTmp+dwOffSet+1, UUID_SEPARATOR);
    }
    strncpy(szTmp, szUUIDTmp, MAX_UUID_LEN);
    return szTmp;
}

class CUUID : public string
{
public:
    CUUID()
    {
        GenUUID();
    }
    CUUID(const CUUID &cObj)
    {
        string::operator =(cObj);
    }
    CUUID(const string &cObj)
    {
        string::operator =(cObj);
    }
    void GenUUID()  //重新生成UUID
    {
        char szTmp[MAX_UUID_LEN+1] = {0};
        GenUUIDStr(szTmp);
        assign(szTmp);
    }
protected:
private:
};

typedef string UUIDString;  //在网络上传输的xml需要用此定义

#endif

