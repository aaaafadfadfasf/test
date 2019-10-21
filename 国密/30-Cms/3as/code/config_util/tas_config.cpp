#include "tas_config.h"
#include "inner_common.h"
#include "dbopr/dbengine.h"
#include "kdvsys.h"
#include "cms_const.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/db_password_codec.h"
/* 配置SECTION常量定义 */
static const s8* SEC_PROXY = "PROXYINFO";
static const s8* SEC_NMS = "NMSINFO";
static const s8* SEC_DB = "DATABASEINFO";
static const s8* SEC_ADVANCED = "ADVANCEDINFO";
static const s8* SEC_LSSDK = "LICENSESDKINFO";
static const s8* SEC_CU = "CUINFO";

/* 配置KEY常量定义 */
static const s8* KEY_PROYX_SELFURI = "SELFURI";
static const s8* KEY_PROYX_LOCALIP = "LOCALIP";
static const s8* KEY_PROYX_LOCALPORT = "LOCALPORT";

static const s8* KEY_NMS_IP = "IP";
static const s8* KEY_NMS_PORT = "PORT";
static const s8* KEY_NMS_LOCALIP = "LOCALIP";

static const s8* KEY_DB_TYPE = "DBTYPE";
static const s8* KEY_DB_IP = "IP";
static const s8* KEY_DB_PORT = "PORT";
static const s8* KEY_DB_USER = "USER";
static const s8* KEY_DB_PASSWORD = "PASSWORD";
static const s8* KEY_DB_DATABASE = "DATABASE";
static const s8* KEY_DB_SECURE = "SECURE";

static const s8* KEY_ADVANCED_CCTRL_APP_QUEUESIZE = "CCTRL_APP_QUEUESIZE";

static const s8* KEY_ADVANCED_DATABASE_TASK_ITEM_MAX_NUM = "DATABASE_TASK_ITEM_MAX_NUM";
static const s8* KEY_ADVANCED_DATABASE_THREAD_NUM = "DATABASE_THREAD_NUM";

static const s8* KEY_ADVANCED_CHANGE_NTF_MAX_NUM = "SESSION_CHANGE_NTF_MAX_NUM";
static const s8* KEY_ADVANCED_DEFAULT_LOG_LEVEL = "DEFAULT_LOG_LEVEL";
static const s8* KEY_ADVANCED_LICENSE_USAGE_CHECK_INTERVAL = "LICENSE_USAGE_CHECK_INTERVAL";
static const s8* KEY_ADVANCED_MANAGE_PU_MAX_NUM = "MANAGE_PU_MAX_NUM";

static const s8* KEY_ADVANCED_MAX_CIVILCODE_LEV_UP_TO_FIVE = "MAX_CIVILCODE_LEV_UP_TO_FIVE";

static const s8* KEY_LSSDK_KEY_TYPE = "LICENSE_KEY_TYPE"; 
static const s8* KEY_LSSDK_IP = "IP";
static const s8* KEY_LSSDK_PORT = "PORT";


static const s8* KEY_CU_PWD_LIMIT_TIME = "PWD_LIMIT_TIME";
static const s8* KEY_CU_PWD_INPUT_TIMES = "PWD_INPUT_TIMES";
static const s8* KEY_CU_PWD_LOCKED_TIME = "PWD_LOCKED_TIME";

s8 CTasConfig::s_szCfgPath[100];
string CTasConfig::s_strStartTime;

string CTasConfig::s_strModuleUri;
string CTasConfig::s_strLocalIP4Proxy;
u16 CTasConfig::s_wLocalPort4Proxy;

string CTasConfig::s_strNMSIP;
u16 CTasConfig::s_wNMSPort;
string CTasConfig::s_strLocalIP4NMS;

s32 CTasConfig::s_nDbType;
string CTasConfig::s_strDbIP;
u16 CTasConfig::s_wDbPort;
string CTasConfig::s_strDbUsername;
string CTasConfig::s_strDbPassword;
string CTasConfig::s_strDbName;

u32 CTasConfig::s_dwCCtrlAppQueueSize;
u32 CTasConfig::s_dwDbTaskItemMaxNum;
u32 CTasConfig::s_dwDbTaskThreadNum;

u32 CTasConfig::s_dwSessionChangentfMaxNum;
s32 CTasConfig::s_nDefaultLogLevel = CRITICAL_LEV;
u32 CTasConfig::s_dwLicenseUsageInterval = 3600;

u16 CTasConfig::s_wLsKeyType;
string CTasConfig::s_strLsIP;
u16 CTasConfig::s_wLsPort;
u32 CTasConfig::s_dwManagePuMaxNum;

bool CTasConfig::s_bCanMaxCivilCodeLevUp2Five = false;

s32 CTasConfig::s_nPwdLimitTime;
s32 CTasConfig::s_nPwdInputTimes;
s32 CTasConfig::s_nPwdLockedTime;

#define PrintAndReturn(desc) {printf(desc); return false;}
inline bool ValidIntRange(u32 dwValue, u32 dwMin, u32 dwMax)
{
    return ((dwMin<= dwValue) && (dwValue <= dwMax));
}
inline bool ValidIP(const string &strIP)
{
    u32 dwIP = inet_addr(strIP.c_str());
    return !((0 == dwIP) || (0xffffffff == dwIP));
}

bool CTasConfig::Init()
{
    sprintf(s_szCfgPath, "conf/aaacfg.ini");

    GetString(SEC_PROXY, KEY_PROYX_SELFURI, s_strModuleUri, UNIQUE_SIP_UA_3AS);
    s_strModuleUri.append("@");
    s_strModuleUri.append(CurrentDomainName());

    GetString(SEC_PROXY, KEY_PROYX_LOCALIP, s_strLocalIP4Proxy, GetLocalIPAddress().c_str());
    if (!ValidIP(s_strLocalIP4Proxy))
    {
        PrintAndReturn("read sip local ip failed");
    }
    GetInt(SEC_PROXY, KEY_PROYX_LOCALPORT, s_wLocalPort4Proxy, TAS_SIP_UDP_PORT);

    if (!GetString(SEC_NMS, KEY_NMS_IP, s_strNMSIP))
    {
        PrintAndReturn("read nms ip failed");
    }
    if (!GetInt(SEC_NMS, KEY_NMS_PORT, s_wNMSPort))
    {
        PrintAndReturn("read nms port failed");
    }
    if (!GetString(SEC_NMS, KEY_NMS_LOCALIP, s_strLocalIP4NMS))
    {
        PrintAndReturn("read nms local ip failed");
    }

    if (!GetInt(SEC_DB, KEY_DB_TYPE, s_nDbType) || !ValidIntRange(s_nDbType, 1, 3))
    {
        PrintAndReturn("read 3as database type failed");
    }
    if (!GetString(SEC_DB, KEY_DB_IP, s_strDbIP) || !ValidIP(s_strDbIP))
    {
        PrintAndReturn("read 3as database ip failed");
    }
    if (!GetInt(SEC_DB, KEY_DB_PORT, s_wDbPort))
    {
        PrintAndReturn("read 3as database port failed");
    }
    if (!GetString(SEC_DB, KEY_DB_USER, s_strDbUsername) || s_strDbUsername == "")
    {
        PrintAndReturn("read 3as database username failed");
    }
    u16 wIsDbPwdSecure;
    if (!GetInt(SEC_DB, KEY_DB_SECURE, wIsDbPwdSecure))
    {
        wIsDbPwdSecure = 0;
    }
    string strTmpDbPassword;
    if (!GetString(SEC_DB, KEY_DB_PASSWORD, strTmpDbPassword))
    {
        PrintAndReturn("read 3as database password failed");
    }
    if (wIsDbPwdSecure == 0)
    {
        s_strDbPassword = strTmpDbPassword;
    }
    else
    {
        s_strDbPassword = DecryptMySqlPassword(strTmpDbPassword);
    }
    if (!GetString(SEC_DB, KEY_DB_DATABASE, s_strDbName) || s_strDbName == "")
    {
        PrintAndReturn("read 3as database name failed");
    }

    if (!GetInt(SEC_ADVANCED, KEY_ADVANCED_CCTRL_APP_QUEUESIZE, s_dwCCtrlAppQueueSize, 15000) 
        || !ValidIntRange(s_dwCCtrlAppQueueSize, 10000, 1000000))
    {
        PrintAndReturn("read cctrl app queue size failed");
    }
    if (!GetInt(SEC_ADVANCED, KEY_ADVANCED_DATABASE_TASK_ITEM_MAX_NUM, s_dwDbTaskItemMaxNum) 
        || !ValidIntRange(s_dwDbTaskItemMaxNum, 1000, 1000000))
    {
        PrintAndReturn("read database_task_item_max_num failed");
    }
    if (!GetInt(SEC_ADVANCED, KEY_ADVANCED_DATABASE_THREAD_NUM, s_dwDbTaskThreadNum) 
        || !ValidIntRange(s_dwDbTaskThreadNum, 3, 100))
    {
        PrintAndReturn("read database_thread_num failed");
    }

    GetInt(SEC_ADVANCED, KEY_ADVANCED_CHANGE_NTF_MAX_NUM, s_dwSessionChangentfMaxNum, 50000);
    if (!ValidIntRange(s_dwSessionChangentfMaxNum, 1000, 10000000))
    {
        PrintAndReturn("read change ntf max num failed");
    }
    GetInt(SEC_ADVANCED, KEY_ADVANCED_DEFAULT_LOG_LEVEL, s_nDefaultLogLevel, CRITICAL_LEV);
    GetInt(SEC_ADVANCED, KEY_ADVANCED_LICENSE_USAGE_CHECK_INTERVAL, s_dwLicenseUsageInterval, s_dwLicenseUsageInterval);
    if (!ValidIntRange(s_dwLicenseUsageInterval, 60, 3600*24*7))
    {
        PrintAndReturn("read LicenseUsageInterval failed");
    }
    GetInt(SEC_ADVANCED, KEY_ADVANCED_MANAGE_PU_MAX_NUM, s_dwManagePuMaxNum, 0);

    u32 dwIsMaxCivilCodeLevUp2Five = 0;
    GetInt(SEC_ADVANCED, KEY_ADVANCED_MAX_CIVILCODE_LEV_UP_TO_FIVE, dwIsMaxCivilCodeLevUp2Five, 0);
    s_bCanMaxCivilCodeLevUp2Five = (dwIsMaxCivilCodeLevUp2Five != 0);

    GetInt(SEC_LSSDK, KEY_LSSDK_KEY_TYPE, s_wLsKeyType, 0);
    GetString(SEC_LSSDK, KEY_LSSDK_IP, s_strLsIP, "0.0.0.0");
    GetInt(SEC_LSSDK, KEY_LSSDK_PORT, s_wLsPort, 10010);

	
	GetInt(SEC_CU, KEY_CU_PWD_LIMIT_TIME, s_nPwdLimitTime, 1);
	GetInt(SEC_CU, KEY_CU_PWD_INPUT_TIMES, s_nPwdInputTimes, 3);
	GetInt(SEC_CU, KEY_CU_PWD_LOCKED_TIME, s_nPwdLockedTime, 5);
	
    return true;
}

void CTasConfig::Dump()
{
    TelePrint(" 配置信息===========================================================");
    TelePrint(" 配置文件           : %s", s_szCfgPath);

    TelePrint(" proxy配置----------------------------------------------------------");
    TelePrint(" TAS自身的URI       : %s", s_strModuleUri.c_str());
    TelePrint(" 连接proxy时的本地IP: %s", s_strLocalIP4Proxy.c_str());
    TelePrint(" 连接proxy时的本地端口:%u", static_cast<u32>(s_wLocalPort4Proxy));
    TelePrint(" 网管配置-----------------------------------------------------------");
    TelePrint(" 网管服务器IP         : %s", s_strNMSIP.c_str());
    TelePrint(" 网管服务器端口       : %u", static_cast<u32>(s_wNMSPort));
    TelePrint(" 连接网管时的本地IP   : %s", s_strLocalIP4NMS.c_str());
    TelePrint(" 数据库配置---------------------------------------------------------");
    TelePrint(" 数据库类型           : %u", static_cast<u32>(s_nDbType));
    TelePrint(" 数据库服务器IP       : %s", s_strDbIP.c_str());
    TelePrint(" 数据库服务器端口     : %u", static_cast<u32>(s_wDbPort));
    TelePrint(" 数据库登录用户       : %s", s_strDbUsername.c_str());
    TelePrint(" 数据库登录密码       : %s", s_strDbPassword.c_str());
    TelePrint(" 工作数据库名称       : %s", s_strDbName.c_str());
    TelePrint(" 高级配置---------------------------------------------------------");
    TelePrint(" 数据库任务缓存数最大值  : %u", s_dwDbTaskItemMaxNum);
    TelePrint(" 数据库工作线程数        : %u", s_dwDbTaskThreadNum);
    TelePrint(" 会话变更通知缓存数最大值: %u", s_dwSessionChangentfMaxNum);
    TelePrint(" 默认的日志级别数值      : %d", s_nDefaultLogLevel);
    TelePrint(" LICENSE使用情况检测间隔 : %u秒", s_dwLicenseUsageInterval);
    TelePrint(" 行政区划分组最大层级可为5级: %s", s_bCanMaxCivilCodeLevUp2Five ? "Yes" : "No");
    TelePrint(" LicenseSDK--------------------------------------------------------");
    TelePrint(" 授权获取方式(本地/网络) : %u", s_wLsKeyType);
    TelePrint(" License服务器 授权产品名: %s", LICENSESDK_PRODUCT_NAME);
    TelePrint(" License服务器 Freetime  : %u秒", LICENSESDK_FREETIME);
    TelePrint(" License服务器 IP        : %s", s_strLsIP.c_str());
    TelePrint(" License服务器 端口      : %u", s_wLsPort);
}

string CTasConfig::GetNMSIP()
{
    return s_strNMSIP;
}

u16 CTasConfig::GetNMSPort()
{
    return s_wNMSPort;
}

string CTasConfig::GetLocalIP4NMS()
{
    return s_strLocalIP4NMS;
}

u32 CTasConfig::GetDbTaskItemMaxNum()
{
    return s_dwDbTaskItemMaxNum;
}

u32 CTasConfig::GetDbTaskThreadNum()
{
    return s_dwDbTaskThreadNum;
}

s32 CTasConfig::GetDbType()
{
    return s_nDbType;
}

string CTasConfig::GetDbIP()
{
    return s_strDbIP;
}

u16 CTasConfig::GetDbPort()
{
    return s_wDbPort;
}

string CTasConfig::GetDbUserName()
{
    return s_strDbUsername;
}

string CTasConfig::GetDbPassword()
{
    return s_strDbPassword;
}

string CTasConfig::GetDbName()
{
    return s_strDbName;
}

string CTasConfig::GetTasURI()
{
    return s_strModuleUri;
}

string CTasConfig::GetLocalIP4Proxy()
{
    return s_strLocalIP4Proxy;
}

u16 CTasConfig::GetLocalPort4Proxy()
{
    return s_wLocalPort4Proxy;
}

bool CTasConfig::GetString( const s8* pcSec, const s8* pcKey, string &strResult, const s8* pcDefault /*= ""*/ )
{
    s8 szBuf[1024];

    if (GetRegKeyString(s_szCfgPath, pcSec, pcKey, pcDefault, szBuf, sizeof(szBuf)))
    {
        szBuf[sizeof(szBuf)-1] = '\0';
        strResult = szBuf;
        return true;
    }
    else
    {
        szBuf[sizeof(szBuf)-1] = '\0';
        strResult = szBuf;
        return false;
    }
}

bool CTasConfig::GetInt( const s8* pcSec, const s8* pcKey, s32 &nResult, s32 nDefault )
{
    if (GetRegKeyInt(s_szCfgPath, pcSec, pcKey, nDefault, &nResult))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CTasConfig::GetInt( const s8* pcSec, const s8* pcKey, u16 &wResult, u16 wDefault )
{
    s32 nResult;

    if (GetInt(pcSec, pcKey, nResult, (s32)wDefault))
    {
        wResult = (u16)nResult;
        return true;
    }
    else
    {
        wResult = (u16)nResult;
        return false;
    }
}

bool CTasConfig::GetInt( const s8* pcSec, const s8* pcKey, u32 &dwResult, u32 dwDefault /*= 0*/ )
{
    s32 nResult;

    if (GetInt(pcSec, pcKey, nResult, (s32)dwDefault))
    {
        dwResult = (u32)nResult;
        return true;
    }
    else
    {
        dwResult = (u32)nResult;
        return false;
    }
}

u32 CTasConfig::GetSessionChangeNtfMaxNum()
{
    return s_dwSessionChangentfMaxNum;
}

u32 CTasConfig::GetCCtrlAppQueueSize()
{
    return s_dwCCtrlAppQueueSize;
}

s32 CTasConfig::GetDefaultLogLevel()
{
    return s_nDefaultLogLevel;
}

u32 CTasConfig::GetLicenseUsageInterval()
{
    return s_dwLicenseUsageInterval;
}

u16 CTasConfig::GetLsKeyType()
{
    return s_wLsKeyType;
}
string CTasConfig::GetLsIP()
{
    return s_strLsIP;
}

u16 CTasConfig::GetLsPort()
{
    return s_wLsPort;
}

void CTasConfig::SetLsKeyType(u16 wLsKeyType)
{
    s_wLsKeyType = wLsKeyType;
    SetRegKeyInt(s_szCfgPath, SEC_LSSDK, KEY_LSSDK_KEY_TYPE,  s_wLsKeyType);
}

void CTasConfig::SetLsIp(const string& strLsIp)
{
    s_strLsIP = strLsIp; 
    SetRegKeyString(s_szCfgPath, SEC_LSSDK, KEY_LSSDK_IP,  s_strLsIP.c_str()); 
}

void CTasConfig::SetLsPort(u16 wLsPort)
{
    s_wLsPort = wLsPort; 
    SetRegKeyInt(s_szCfgPath, SEC_LSSDK, KEY_LSSDK_PORT, s_wLsPort); 
}

string CTasConfig::GetStartTime()
{
	 return s_strStartTime;
}

void CTasConfig::SetStartTime(const string& val )
{
	 s_strStartTime = val;
}
u32 CTasConfig::GetManagePuMaxNum()
{
    return s_dwManagePuMaxNum;
}
void CTasConfig::SetManagePuMaxNum(u32 dwManagePuMaxNum)
{
    s_dwManagePuMaxNum = dwManagePuMaxNum;
    SetRegKeyInt(s_szCfgPath, SEC_ADVANCED, KEY_ADVANCED_MANAGE_PU_MAX_NUM, s_dwManagePuMaxNum);
}

bool CTasConfig::CanMaxCivilCodeLevUp2Five()
{
    return s_bCanMaxCivilCodeLevUp2Five;
}

s32 CTasConfig::GetPwdLimitTime()
{
	return s_nPwdLimitTime;
}

s32 CTasConfig::GetPwdInputTimes()
{
	return s_nPwdInputTimes;
}

s32 CTasConfig::GetPwdLockedTime()
{
	return s_nPwdLockedTime;
}
