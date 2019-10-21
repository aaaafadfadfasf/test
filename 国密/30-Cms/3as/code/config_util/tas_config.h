#ifndef _TAS_CONFIG_H_
#define _TAS_CONFIG_H_
#include "inner_common.h"

class CTasConfig
{
public:
    static bool Init();
    static void Dump();

    static const s8* GetCfgPath()
    {
        return s_szCfgPath;
    }

    /* proxy相关配置 */
    static string GetTasURI();
    static string GetLocalIP4Proxy();
    static u16 GetLocalPort4Proxy();

    /* NMS的相关配置 */
    static string GetNMSIP();
    static u16 GetNMSPort();
    static string GetLocalIP4NMS();

    /* 数据库连接配置 */
    static s32 GetDbType();
    static string GetDbIP();
    static u16 GetDbPort();
    static string GetDbUserName();
    static string GetDbPassword();
    static string GetDbName();
    
    /* License SDK 服务器配置*/
    static void SetLsKeyType(u16 wLsKeyType);
    static u16 GetLsKeyType();
    static void SetLsIp(const string& strLsIp);
    static string GetLsIP();
    static void SetLsPort(u16 wLsPort);
    static u16 GetLsPort();

    /* 高级配置 */
    /* 消息队列大小配置 */
    static u32 GetCCtrlAppQueueSize();
    /* 获取DB任务池相关配置 */
    static u32 GetDbTaskItemMaxNum();
    static u32 GetDbTaskThreadNum();
    /* 会话任务相关配置 */
    static u32 GetSessionChangeNtfMaxNum(); // 单个会话任务允许缓存的通知任务的最大值
    /* 默认的日志级别 */
    static s32 GetDefaultLogLevel();
    /* license使用检测时间间隔，单位：秒 */
    static u32 GetLicenseUsageInterval();
    /* 行政区划分组最大层级是否可为5级 */
    static bool CanMaxCivilCodeLevUp2Five();

    static u32 GetManagePuMaxNum();
    static void SetManagePuMaxNum(u32 dwManagePuMaxNum);
	static string GetStartTime();
	static void SetStartTime(const string& val);

	/*CU相关配置*/
	static s32 GetPwdRemindTime();
	static s32 GetPwdLimitTime();
	static s32 GetPwdInputTimes();
	static s32 GetPwdLockedTime();
private:
    static bool GetString(const s8* pcSec, const s8* pcKey, string &strResult, const s8* pcDefault = "");
    static bool GetInt(const s8* pcSec, const s8* pcKey, s32 &nResult, s32 nDefault = 0);
    static bool GetInt(const s8* pcSec, const s8* pcKey, u32 &dwResult, u32 dwDefault = 0);
    static bool GetInt(const s8* pcSec, const s8* pcKey, u16 &wResult, u16 wDefault = 0);
private:
    static s8 s_szCfgPath[100];
	static string s_strStartTime;

    static string s_strModuleUri;
    static string s_strLocalIP4Proxy;
    static u16 s_wLocalPort4Proxy;

    static string s_strNMSIP;
    static u16 s_wNMSPort;
    static string s_strLocalIP4NMS;

    static s32 s_nDbType;
    static string s_strDbIP;
    static u16 s_wDbPort;
    static string s_strDbUsername;
    static string s_strDbPassword;
    static string s_strDbName;

    static u32 s_dwCCtrlAppQueueSize;

    static u32 s_dwDbTaskItemMaxNum;
    static u32 s_dwDbTaskThreadNum;

    static u32 s_dwSessionChangentfMaxNum;
    static s32 s_nDefaultLogLevel;

    static u32 s_dwLicenseUsageInterval;

    static u16 s_wLsKeyType;
    static string s_strLsIP;
    static u16 s_wLsPort;
    static u32 s_dwManagePuMaxNum;

    static bool s_bCanMaxCivilCodeLevUp2Five;    // 行政区划分组最大层级默认为4级，是否可为5级

	static s32 s_nPwdLimitTime;   //密码连续输入错误的限定时间，单位分钟，默认1分钟内输入3次错误被锁定5分钟
	static s32 s_nPwdInputTimes;  //密码连续输入次数
    static s32 s_nPwdLockedTime;  //密码被锁定的时间
};

#endif // _TAS_CONFIG_H_
