/*========================================================================================
ģ����    ��uuid
�ļ���    ��uuid.h
����ļ�  ��
ʵ�ֹ���  ����ƽ̨��UUID��
����      ��liangli
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2011/07/21         V1.0              lianlgi                                �½��ļ�
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
#define MAX_CMS_UUID_LEN    32     //Ϊ�˼���ƽ̨1.0�汾��PU�Լ�����ģ�飬Ǩ��KDMNO���ȣ��ʽ�UUID�е��ĸ�'-'����ȥ��
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

    //�����ɵ�UUID�е�'-'����ȥ��
    char szUUIDTmp[MAX_UUID_LEN+1] = {0};
    strncpy(szUUIDTmp, (char *)szTmp, MAX_UUID_LEN);
    char *pCh = strstr(szTmp, UUID_SEPARATOR);
    while ( NULL != pCh )
    {
        u32 dwOffSet = pCh - szTmp;                 //szTmp�е�"-"����ƫ����
        char *pTmp = strstr(szUUIDTmp, UUID_SEPARATOR);
        CMS_ASSERT(NULL != pTmp);
        u32 dwTmpOffset = pTmp - szUUIDTmp;         //szUUIDTmp�е�һ��"-"����ƫ����
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
    void GenUUID()  //��������UUID
    {
        char szTmp[MAX_UUID_LEN+1] = {0};
        GenUUIDStr(szTmp);
        assign(szTmp);
    }
protected:
private:
};

typedef string UUIDString;  //�������ϴ����xml��Ҫ�ô˶���

#endif

