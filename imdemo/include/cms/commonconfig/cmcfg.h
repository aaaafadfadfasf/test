/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cmcfg.h
   相关文件    : 
   文件实现功能: commonconfig 提供公共配置的读写接口
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/14   1.0         liangli        创建
******************************************************************************/
#ifndef _COMMON_CONFIG_H_
#define _COMMON_CONFIG_H_
#include "../cms_const.h"
#include "../cms_struct.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

#ifdef _LINUX_
const s8 chDirSep = '/';
#else
const s8 chDirSep = '\\';
#endif

#ifndef LPCSTR
#define LPCSTR  const char *
#endif

struct CCommonNetNatInfo
{
    CCommonNetNatInfo()
    {
        platformPort = 0;
        platformNatPort = 0;  
		protocol = NAT_PROTOCOL_TCP;
    }
    string platformIP;     // Nat映射前
    int platformPort;
    string platformNatIP;  // Nat映射后
    int platformNatPort;
	string protocol;	   // 协议，TCP/UDP
};

//将公共配置结构提取到接口中，便于上层业务一次性获取所有配置
class CCommonConfig
{
public:
    CCommonConfig();

    bool ReadXmlConfig(const string &strXml);
    string GetXmlString();

    string GetDomainUUID();
    bool SetDomainUUID(const string &strUUID);

    string GetDomainName();
    bool SetDomainName(const string &strName);

    string GetParentDomainUUID();
    bool SetParentDomainUUID(const string &strUUID);

    string GetParentDomainName();
    bool SetParentDomainName(const string &strName);

    string GetParentPlatformProxyUrl();
    bool SetParentPlatformProxyUrl(const string &proxyUrl );

    string GetProxyIP();
    bool SetProxyIP(const string &strIP);

    unsigned short GetProxyPort();
    bool SetProxyPort(const unsigned short wPort);

    unsigned short GetHttpPort();
    bool SetHttpPort(unsigned short wPort);

    string GetStunIP();
    bool SetStunIP(const string &strIP);

    unsigned short GetStunPort();
    bool SetStunPort(const unsigned wPort);

    bool GetEnablePunch();
    bool SetEnablePunch(const bool &bEnbale);

    bool GetDefenseCrossStreamEnable();
    bool SetDefenseCrossStreamEnable(const bool &bEnbale);

    unsigned short GetNmsPort();
    bool SetNmsPort(const unsigned wPort);

    string GetLocalIP();
    bool SetLocalIP(const string &strIP);

    vector<CCommonNetNatInfo> GetNetNatInfo();
    bool SetNetNatInfo(const vector<CCommonNetNatInfo> &vecNatInfo);
    bool FindNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);
	bool AddNetNatInfo(const CCommonNetNatInfo &cNatInfo);
	bool DelNetNatInfo(const TNetAddr& tAddr, const string &strProtocol = NAT_PROTOCOL_UDP);
	bool ClearNetNatInfo(const string &strPlatIp);


    vector<CCommonNetNatInfo> GetOtherNetNatInfo();
    bool SetOtherNetNatInfo(const vector<CCommonNetNatInfo> &vecNatInfo);
    bool FindOtherNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);
	bool AddOtherNetNatInfo(const CCommonNetNatInfo &cNatInfo);
	bool DelOtherNetNatInfo(const TNetAddr& tAddr, const string &strProtocol = NAT_PROTOCOL_UDP);
	bool ClearOtherNetNatInfo(const string &strPlatIp);

    string GetCmsVer();
    bool SetCmsVer(const string &strVer);

	string GetCmsArch();
	bool SetCmsArch(const string &strArch);

    string GetCuUpdateUrl();
    bool SetCuUpdateUrl(const string &strUrl);

    string GetCuVer();
    bool SetCuVer(const string &strVer);

    string GetLogDir();
    bool SetLogDir(const string &strDir);

    string GetRedisIP();
    bool SetRedisIP(const string &strIP);
    unsigned short GetRedisPort();
    bool SetRedisPort(unsigned short wPort);
    string GetRedisPassword();
    bool SetRedisPassword(const string &strPwd);

    string GetMysqlUser();
    unsigned short GetMysqlPort();
    string GetMysqlPassword();
    bool GetMysqlPasswordSecure();

    bool IsMediaSwitchEnabled() const;
    void SetMediaswitch( bool bEnable );

private:
    string m_strDomainUUID;
    string m_strDomainName;
    string m_strParentDomainUUID;
    string m_strParentDomainName;

    string m_strProxyIP;
    unsigned short m_wProxyPort;
    unsigned short m_wHttpPort;

    string m_parentProxyUrl;	// 上级Proxy Url。

    string m_strStunIP;
    unsigned short m_wStunPort;

    bool    m_bEnablePunch;

    bool    m_bIsDefenseCrossStreamEnable;   //主动防御串码流

    unsigned short m_wNmsPort;

    string m_strLocalIP;
    vector<CCommonNetNatInfo> m_vecNatInfo;
    vector<CCommonNetNatInfo> m_vecOtherNatInfo;   //为从平台配置的端口映射

    string m_strCmsVer;
	string m_strCmsArch;
    string m_strCuVer;
    string m_strCuUrl;

    string m_strLogDir;


    string m_strRedisIP;
    unsigned short m_wRedisPort;
    string m_strRedisPassword;
    string m_strMysqlUser;
    string m_strMysqlPassword;
    bool m_bMysqlPasswordSecure;
    unsigned short m_wMysqlPort;
    bool m_bMediaSwitchEnabled;
};



/*====================================================================
函数名      : ReadCommonConfig
功能        : 在不重启的情况下，想要重新读取公共配置项，需要调用此接口
算法实现    :
             1. ReadCommonConfig（bool bIsChkDevMark = true）增加了默认参数，                标识是否检测DevMark的合法性。目前只有vtdu模块需要将该参数置为false。             2. 只有下面几个所有模块都用到的配置项读取时会检测是否读取成功：                1)本域UUID  2)本域域名   3)proxy地址   4)proxy端口  5)本地IP地址                这几项如果任何一个读取失败程序就会退出，其他项读取失败就使用默认值
引用全局变量:
输入参数说明:
             bIsChkDevMark:是否检测devMark，默认检测
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/21/             liangli
====================================================================*/
bool ReadCommonConfig(bool bIsChkDevMark = true);

/*====================================================================
函数名      : SaveCommonConfig
功能        : 修改公共配置项后，如果想保存到文件中，需要调用此接口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/21/             liangli
====================================================================*/
bool SaveCommonConfig();

/*====================================================================
函数名      : GetCommonConfig
功能        : 获取CommonConfig整个结构
算法实现    : 先调用ReadCommonConfig()后才能调用GetCommonConfig；返回的是对象，不是引用
引用全局变量:
输入参数说明:
             
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/05/13/             fanxg
====================================================================*/
CCommonConfig GetCommonConfig();

/*====================================================================
函数名      : GetGlobalDeviceUUID
功能        : 返回本域的域UUID
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
string GetDomainUUID();
bool SetDomainUUID(const string &strUUID);

/*====================================================================
函数名      : GetDomainName
功能        : 返回域名称字符串
              所有的域名字符串约定为全小写, 仅包含小写字母和数字
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
string GetDomainName();
bool SetDomainName(const string &strName);

/*====================================================================
函数名      : GetParentDomainUUID
功能        : 返回父域的域UUID
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/01/08/             huzhiyun
====================================================================*/
string GetParentDomainUUID();
bool SetParentDomainUUID(const string &strUUID);

/*====================================================================
函数名      : GetParentDomainName
功能        : 返回域名称字符串
              所有的域名字符串约定为全小写, 仅包含小写字母和数字
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/01/08/             huzhiyun
====================================================================*/
string GetParentDomainName();
bool SetParentDomainName(const string &strName);

/*====================================================================
函数名      : GetProxyIPAddress
功能        : 返回Proxy的地址
              一个域只有一个Proxy，一个3as和一个cmu
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
string GetProxyIPAddress();
bool SetProxyIPAddress(const string &strIpAddr);

/*====================================================================
函数名      : GetProxyPort
功能        : 返回Proxy的端口
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
unsigned short GetProxyPort();
bool SetProxyPort(const unsigned short wPort);

/*====================================================================
函数名      : GetParentPlatformProxyUrl / SetParentPlatformProxyUrl
功能        : 获得和修改上级平台的Proxy地址（URL形式 ip:port )
              一个域只有一个上级，同上级通信只需要Proxy地址。
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/03/06/             lizhixing
====================================================================*/
string GetParentPlatformProxyUrl();
bool SetParentPlatformProxyUrl(const string &proxyUrl );

/*====================================================================
函数名      : GetStunIPAddress
功能        : 获取STUN服务器地址
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/21/             liangli
====================================================================*/
string GetStunIPAddress();
bool SetStunIPAddress(const string &strIpAddr);

unsigned short GetStunPort();
bool SetStunPort(const unsigned short wPort);

/*====================================================================
函数名      : GetEnablePunch
功能        : 是否启用Punch
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/04/15            huangzhichun
====================================================================*/
bool GetEnablePunch();
bool SetEnablePunch(const bool &bEnbale);

/*====================================================================
函数名      : GetNmsPort
功能        : 获取网管服务器地址
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/04/01/             huzhiyun
====================================================================*/
unsigned short GetNmsPort();
bool SetNmsPort(const unsigned short wPort);

/*====================================================================
函数名      : GetLogDir
功能        : 目前2.0模块的日志比较大，所以在commonconfig里提供一个全局配置
             各个模块根据此配置将日志写到统一指定目录
             日志路径默认/var/log/(保证以/字符结尾，win下默认c:\kdm)

             各个模块在初始化 OspExtInit() 时
             string strLogDir = GetLogDir();
             TOspExtConf tOspExtConf;
             tOspExtConf.strErrLogDir = strLogDir + "cui/errlog/";
             tOspExtConf.strRunLogDir = strLogDir + "cui/runlog/"
             if ( ! OspExtInit(tOspExtConf) )

             在 OspSipInit()也做同样操作
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/3/21/             liangli
====================================================================*/
string GetLogDir();
bool SetLogDir(const string &strDir);


/*====================================================================
函数名      : GetLocalIPAddress
功能        : 获取本地用于业务通信的地址
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/10/10/            huangzhichun
====================================================================*/
string GetLocalIPAddress();
bool SetLocalIPAddress(const string &strIpAddr);

/*====================================================================
函数名      : GetCommonCmsVer
功能        : 获取平台的Cms版本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCmsVer();
bool SetCommonCmsVer(const string &strVer);

/*====================================================================
函数名      : GetCommonCmsVer
功能        : 获取平台的Cms的位数（X86为32位，X86_64为64位，参照cms_const)
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  :
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCmsArch();
bool SetCommonCmsArch(const string &strVer);

/*====================================================================
函数名      : GetCommonCuVer
功能        : 获取平台的cu版本信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCuVer();
bool SetCommonCuVer(const string &strVer);

/*====================================================================
函数名      : GetCommonCuUpdateUrl
功能        : 获取平台的cu url路径
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCuUpdateUrl();
bool SetCommonCuUpdateUrl(const string &strCuUpdateUrl);

/*====================================================================
函数名      : GetCommonNetNatInfo
功能        : 获取网络NAT映射信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/30/            huzhiyun
====================================================================*/
vector<CCommonNetNatInfo> GetCommonNetNatInfo();
bool SetCommonNetNatInfo(const vector<CCommonNetNatInfo> &vecNat);
bool AddCommonNetNatInfo(const CCommonNetNatInfo &cInfo);
bool DelCommonNetNatInfo(const TNetAddr &tAddr, const string &strProtocol);
bool ClearCommonNetNatInfo(const string &strPlatIp);
bool FindNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);

/*====================================================================
函数名      : GetCommonOtherNetNatInfo
功能        : 获取网络NAT映射信息（从平台的），为不破坏原有功能不分段。
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2016/10/17/           周旋
====================================================================*/
vector<CCommonNetNatInfo> GetCommonOtherNetNatInfo();
bool SetCommonOtherNetNatInfo(const vector<CCommonNetNatInfo> &vecNat);
bool AddCommonOtherNetNatInfo(const CCommonNetNatInfo &cInfo);
bool DelCommonOtherNetNatInfo(const TNetAddr &tAddr, const string &strProtocol);
bool ClearOtherCommonNetNatInfo(const string &strPlatIp);
bool FindOtherNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);


/*====================================================================
函数名      : GetRedisIP
功能        : 获取redis的ip,port,password信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2016/03/29/            huzhiyun
====================================================================*/
string GetRedisIP();
bool SetRedisIP(const string &strIpAddr);
unsigned short GetRedisPort();
bool SetRedisPort(unsigned short wPort);
string GetRedisPassword();


/*====================================================================
函数名      : GetMysql
功能        : 获取mysql的user,password,port,secure(是否加密)信息
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2017/08/21/            zhouxuan
====================================================================*/
string GetMysqlUser();
string GetMysqlPassword();
unsigned short GetMysqlPort();
bool GetMysqlSecure();
//////////////////////////////////////////////////////////////////////////
//给母卡制作工具使用

/*====================================================================
函数名      : CreateDeviceMark
功能        : 创建设备文件戳，供制作母卡或者工程施工时的小工具使用
算法实现    : 1，创建文件
             2，生成设备UUID
             3，写入文件
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/15/             liangli
====================================================================*/
bool CreateDeviceMark();

//////////////////////////////////////////////////////////////////////////
//一些公共接口

bool UnifyDir( string &strNewDir );

/*=============================================================================
函 数 名： IsDigital
功    能： check if the string is all digital
算法实现： 
全局变量： 
参    数： s8* pchAlias
返 回 值： inline BOOL32 
=============================================================================*/
bool IsDigital(const string &str);

/*==================================================================== 
    函数名  ：CreateDir
    功能    ：create directory
    引用全局变量：
    输入参数说明：s8 *pszNewDir  -- [in] new directory
    返回值说明：TRUE: create directory success
                FALSE: create directory failed
----------------------------------------------------------------------
    修改记录:
    日  期      版本		修改人		走读人      修改内容
====================================================================*/       
bool CreateDir( const string &strNewDir );

/*====================================================================
函数名      : GetModuleFullPathAndFileName
功能        : 
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 返回当前进程文件的绝对路径 和模块文件名
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
bool GetModuleFullPathAndFileName(string &strFullPath, string &strFileName);

/*====================================================================
函数名      : IsFileExist
功能        : 
算法实现    :
引用全局变量:
输入参数说明: const string strFile   可以是文件或者文件夹
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
bool IsFileExist(const string strFile);

/*==================================================================== 
函数名  ：DeleteNullDir
功能    ：delete null directory
引用全局变量：
输入参数说明：LPCSTR pszDir  -- [in] delete null directory
返回值说明：TRUE: delete directory success
FALSE: delete directory failed
----------------------------------------------------------------------
修改记录:
日  期      版本		修改人		走读人      修改内容
====================================================================*/   
bool DeleteNullDir(const string &strDir);

/*====================================================================
函数名      : CMSDeleteFile
功能        : 删除一个文件
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/14/             liangli
====================================================================*/
bool CMSDeleteFile(const string &strFile);

/*========================================================================
函数名      : IsAlreadyExist
功能        : 判断进程是否已经存在
算法实现    : 遍历/proc下的文件, 查找与进程同名,且PID不相同的存在
引用全局变量:
输入参数说明: char* pidName  -- 进程名
pid_t tCurPid  -- 当前进程号
输出参数说明:  
返回值说明  : TRUE 存在, FALSE 不存在
--------------------------------------------------------------------------
修改记录    :
日  期       版本        修改人        修改内容
2009/09/08   1.0         LIANGLI        CREATE
=========================================================================*/
#ifdef WIN32
typedef u32 pid_t;
//inline pid_t getpid()
//{
//    return 0;
//}
#endif

bool IsAlreadyExist( const string strPidName, pid_t tCurPid);

/*====================================================================
函数名      : ReadFileToString
功能        : 将文件内容载入到string对象中
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/15/             liangli
====================================================================*/
bool ReadFileToString(LPCSTR lpFile, string &strFileContent);

/*====================================================================
函数名      : WriteFileFromString
功能        : string内容写入文件
算法实现    :
引用全局变量:
输入参数说明:
返回值说明  : 成功:true，失败:false
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2011/11/15/             liangli
====================================================================*/
bool WriteFileFromString(LPCSTR lpFile, const string &strFileContent, bool bAllowEmpty=true);

/*====================================================================
函数名      : GetLanDesc
功能        : 获取语言描述
算法实现    :
引用全局变量:
输入参数说明: strDescID-语言描述ID，参见cms_landesc.h
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/11/23/             fanxg
====================================================================*/
string GetLanDesc(const string& strDescID);

/*====================================================================
函数名      : GetHttpPort SetHttpPort
功能        : 获取和设置业务Http端口
算法实现    :
引用全局变量:
输入参数说明: 
返回值说明  : 
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2014/11/26/             huzy
====================================================================*/
unsigned short GetHttpPort();
bool SetHttpPort(unsigned short wPort);


unsigned short GetExtendsMode();
bool SetExtensionMode(unsigned int mode);

#endif

