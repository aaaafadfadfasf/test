#include "inner_common.h"
#include "cms/commonconfig/cmcfg.h"
#include <cstdio>

void printhelp()
{
    printf("support these options: -domainid -domainname -newuuid.\n");
    printf("-domainid(get current domainid)\n-domainname(get current domainname)\n-newuuid(generate a new uuid)\n");
}
SEMHANDLE g_hTastestExitSem  = NULL;       // 程序退出同步访问信号量
int main(int argc,char* argv[])
{
    if (argc == 1)
    {
        printhelp();
        return -1;
    }
    if (!InitCommonConfig())
    {
        printf("InitCommonConfig failed");
        return -1;
    }
    if (0 == strcmp(argv[1], "-domainid"))
    {
        printf("%s", GetDomainUUID().c_str());
        return 0;
    }
    if (0 == strcmp(argv[1], "-domainname"))
    {
        printf("%s", GetDomainName().c_str());
        return 0;
    }
    if (0 == strcmp(argv[1], "-newuuid"))
    {
        CUUID cuuid;
        printf("%s", cuuid.c_str());
        return 0;
    }

    printhelp();
    return -1;
}
