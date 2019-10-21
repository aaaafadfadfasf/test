#ifndef _KDMSEARCH_H_
#define _KDMSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int m_dwTargetIP;
    unsigned int m_dwNewIP;
    unsigned int m_dwNewMask;
    unsigned int m_dwNewGW;
} TSetNewIpInfo;

// 修改IP回调函数结构，0 -- 成功，非0 -- 失败
typedef int (*FEvSetDevIp)(void* pUserHandle, TSetNewIpInfo tNewIpInfo);

// 重启回调
typedef void (*FEvReboot)(void* pUserHandle, int nResult);

/************************************************************************/
/* 函数功能：启动搜索模块                                               */
/* 参数说明：pfunSetIp —— 修改IP地址回调指针                          */
/*           pchConfigPath —— 配置文件路径                            */
/*                              如：/usr/config/                        */
/*                              或：c:\d0h\usr\config\                  */
/* 函数返回值：0 —— 成功；非0 —— 失败                               */
/* 函数说明：搜索模块启动一个搜索线程                                   */
/************************************************************************/
int KdmSearchStartup(void* pUserHandle, FEvSetDevIp pfunSetIp, FEvReboot pfunReboot, char* pchConfigPath);

/************************************************************************/
/* 函数功能：停止搜索模块                                               */
/* 参数说明：无                                                         */
/* 函数返回值：无                                                       */
/* 函数说明：                                                           */
/************************************************************************/
void KdmSearchCleanup(void);

#ifdef __cplusplus
}
#endif

#endif // ifndef _KDMSEARCH_H_

/************************************************************************/
/* _280X_ 配置文件格式                                                  */
/* 配置文件名：mdcfg.ini                                                */
/* #开头的行为注释，注释请单独用一行                                    */
/************************************************************************/
/*

#网络配置信息
[NETINFO]
EthNum = 0
#格式
#EthN = ip Netmask Gateway MacAddr
#当没配网关时，就用0.0.0.0代替


#设备信息
[DEVINFO] 
DeviceType = 80
DeviceName = G200
TypeName = G200
SeriesName = G_Serial
MulticastIp = 224.16.32.1
MulitcastPort = 5510

[DEBUGOPTION]
#调试开关，1为打开调试打印，0为关闭。修改后需要重启模块
DebugPrint = 1
#此开关暂时无效
DebugLog = 0

*/

/************************************************************************/
/* _280X_ 版本信息文件                                                  */
/* 文件名：kdm-search.ver                                               */
/************************************************************************/
/*

Kdm search version: KDM SEARCH 2.20100612, compile time: Jun 27 2010 00:14:34.

*/

/**************************** FILE END *******************************/

