/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : main.cpp
   相关文件    : 
   文件实现功能: cui_fcgi功能
   作者        : 梁  黎
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/09/23  1.0         梁  黎        创建
******************************************************************************/
#include <fstream>
#include "../../common/cuiproto.h"
#ifdef WITH_FASTCGI
#include "system/libfcgi/include/fcgi_config.h"
#include "system/libfcgi/include/fcgi_stdio.h"
#endif
#include "../soap/soapcusdkService.h"

#ifdef WIN32
#define CONFIG_FILE_NAME ".\\conf\\cuicfg.xml"
#define COMMON_CONFIG_FILE_NAME "c:\\kdm\\commonconfig.ini"
#define PATH_SPLIT "\\"
#else
#define CONFIG_FILE_NAME "./conf/cuicfg.xml"
#define COMMON_CONFIG_FILE_NAME "/etc/kdm/commonconfig.ini"
#define PATH_SPLIT "/"
#endif
#define KEY_TCP_LISTEN_IP_BEGIN "<tcp_listen_ip>"
#define KEY_TCP_LISTEN_IP_END "</tcp_listen_ip>"
#define KEY_TCP_LISTEN_PORT_BEGIN "<tcp_listen_port>"
#define KEY_TCP_LISTEN_PORT_END "</tcp_listen_port>"
#define KEY_TCP_TIMEOUT_BEGIN "<tcp_timeout>"
#define KEY_TCP_TIMEOUT_END "</tcp_timeout>"

#define KEY_TCP_RW_TIMEOUT_BEGIN "<tcp_rwtimeout>"
#define KEY_TCP_RW_TIMEOUT_END "</tcp_rwtimeout>"

#define KEY_DOMAIN_BEGIN "<domain>"
#define KEY_DOMAIN_END "</domain>"
#define KEY_NAME_BEGIN "<name>"
#define KEY_NAME_END "</name>"

#define DEF_DOMAIN_NAME "kedacom"

// #ifdef WIN32
// #include "system/libfcgi/include/fcgimisc.h"
// #else
// extern char **environ;
// #endif

// char **savedEnviron = NULL;
// char **requestEnviron = NULL;

string g_strLogFile;
u16 g_wCuiServerTcpListenPort = 0;
u32 g_dwCuiServerTimeout = 0;
u32 g_dwCuiServerReadWriteTimeout = 0;

string g_strCuiServerTcpIp;
string g_strDomainName;

string GetXmlKeyText(const string &strXml, LPCSTR lpKeyBegin, LPCSTR lpEnd)
{
    string strRet;
    if ( NULL == lpKeyBegin || NULL == lpEnd )
    {
        return strRet;
    }
    int nKeyLen = (int)strlen(lpKeyBegin);
    if ( 0 == nKeyLen || 0 == strlen(lpEnd) )
    {
        return strRet;
    }

    string::size_type pos1 = strXml.find(lpKeyBegin);
    if ( pos1 == string::npos )
    {
        return strRet;
    }
    pos1 += (u32)nKeyLen;
    string::size_type pos2 = strXml.find(lpEnd);
    if ( pos2 == string::npos )
    {
        return strRet;
    }
    strRet.assign(strXml, pos1, pos2-pos1);
    return strRet;
}

bool ReadFileToString(LPCSTR lpFile, string &strFileContent)
{
    std::ifstream file(lpFile, std::ios_base::in | std::ios_base::binary);
    if ( !file )
    {
        return false;
    }
    std::istreambuf_iterator<char> dataBegin(file);
    std::istreambuf_iterator<char> dataEnd;
    strFileContent.assign(dataBegin, dataEnd);
    file.close();
    return true;
}

string GetCuiServerIpStr()
{
    if(g_strCuiServerTcpIp.empty())
    {
        std::string strCuiIp("127.0.0.1");
        return strCuiIp;
    }
    return g_strCuiServerTcpIp;
}

u16 GetCuiServerTcpListenPort()
{
    return g_wCuiServerTcpListenPort;
}

//单位秒，这个值实际仅用于连接cui的时候的超时
u32 GetCuiServerTimeout()
{
    return g_dwCuiServerTimeout;
}

//与cui的读写超时，单位秒
u32 GetCuiServerReadWriteTimeout()
{
	return g_dwCuiServerReadWriteTimeout;
}

bool GetDomainName( string &domainName )
{
	domainName.clear();

	string strXmlContent;
	if (!ReadFileToString(COMMON_CONFIG_FILE_NAME, strXmlContent))
	{
		CuiWriteLog(g_strLogFile.c_str(), "读取公共配置文件失败\n");
		return false;
	}

	string strDomain = GetXmlKeyText(strXmlContent, KEY_DOMAIN_BEGIN, KEY_DOMAIN_END);
	if ( strDomain.empty() )
	{
		CuiWriteLog(g_strLogFile.c_str(), "读取配置文件失败 key:domain\n");
		return false;
	}

	string strDomainName = GetXmlKeyText(strDomain, KEY_NAME_BEGIN, KEY_NAME_END);
	if ( strDomainName.empty() )
	{
		CuiWriteLog(g_strLogFile.c_str(), "读取配置文件失败 domain key name:\n");
		return false;
	}

	domainName = strDomainName;

	return true;
}
/** The length of post data. */
// int GetInputLen()
// {
//     char *contentLength = FCGX_GetParam( "CONTENT_LENGTH", requestEnviron );
//     int inputLen = 0;
//     if (contentLength != NULL)
//     {
//         inputLen = strtol(contentLength, NULL, 10);
//     }
//     return inputLen;
// }

/*
产生HTTP响应头部
*/
// void GeneratRspHeader()
// {
//     time_t now;
//     time(&now);
//     int nRadom = rand();
//     // update the time to avoid cache.
//     printf( "Last-Modified: %I64u %d GMT\r\n", now, nRadom );
//     // return xml.
//     printf("Content-type: text/xml\r\n");
//     printf( "\r\n" );
// }

/** 线程安全的Accept. */
// int FCGI_Accept_ThreadSafe( FCGX_ParamArray *fcgiEnv )
// {
//      savedEnviron = environ;
//      int acceptStatus = FCGI_Accept();
//      requestEnviron = environ;
//      environ = savedEnviron;
// 
//      CuiWriteLog(g_strLogFile.c_str(), g_strLogFile.c_str(), "acceptStatus = %d\n", acceptStatus);
// 
// 	 // 传出..
// 	 *fcgiEnv = requestEnviron;
// 
//      if(acceptStatus >= 0 && !FCGX_IsCGI()) {
//          /*
//           * requestEnviron指向FCGI_Accept分配的内存中的名-值对。
//           * 可以读取，不可保留指针 -- 改为获得拷贝。
//           */
//      }
// 
// 	return acceptStatus;
// }

/*====================================================================
函数名：SockInit
功能：套接口库初始化。封装windows的WSAStartup，vxWorks下返回TRUE.
算法实现：（可选项）
引用全局变量：
输入参数说明：

返回值说明：成功返回TRUE，失败返回FALSE
====================================================================*/
// int SockInit(void)
// {
// #ifdef _MSC_VER
//     WSADATA wsaData;
//     u32 err;
// 	static int flag = 0;
// 
// 	if (flag == 1)				/* 只初始化一次*/
// 		return TRUE;
// 
//     err = WSAStartup(MAKEWORD(2, 2), &wsaData);
// 	if(err != 0)
// 	{
// 		return FALSE;
// 	}
// 
// 	flag = 1;
// #endif
// 
// 	return TRUE;
// }


int main ( int argc, char* argv[] )
{
    //读取cui运行路径以便生成日志路径
    g_strLogFile = argv[0];
    string::size_type pos = g_strLogFile.rfind(PATH_SPLIT);
    if ( pos == string::npos )
    {
        return -1;
    }
    g_strLogFile.replace(pos+1, string::npos, "cuilog.log");

    //读取CuiServer的配置文件，cui.fcgi和cuiserver共用一个配置文件
    string strCuiCfgXml;
    ReadFileToString(CONFIG_FILE_NAME, strCuiCfgXml);
    string strPort = GetXmlKeyText(strCuiCfgXml, KEY_TCP_LISTEN_PORT_BEGIN, KEY_TCP_LISTEN_PORT_END);
    if ( strPort.empty() )
    {
        CuiWriteLog(g_strLogFile.c_str(), "读取CUI配置文件失败\n");
        return 0;
    }
    g_wCuiServerTcpListenPort = (u16)atol(strPort.c_str());

    g_strCuiServerTcpIp = GetXmlKeyText(strCuiCfgXml, KEY_TCP_LISTEN_IP_BEGIN, KEY_TCP_LISTEN_IP_END);
    if(g_strCuiServerTcpIp.empty())
    {
        CuiWriteLog(g_strLogFile.c_str(), "读取CUI配置文件失败\n");
        g_strCuiServerTcpIp = "127.0.0.1";
    }
	//连接cui的超时时间
    string strTimeout = GetXmlKeyText(strCuiCfgXml, KEY_TCP_TIMEOUT_BEGIN, KEY_TCP_TIMEOUT_END);
    if ( strTimeout.empty() )
    {
        g_dwCuiServerTimeout = MAX_OPR_TIMEOUT;
    }
    else
    {
        g_dwCuiServerTimeout = (u32)atol(strTimeout.c_str());
    }

	//与cui交互的超时时间
	strTimeout = GetXmlKeyText(strCuiCfgXml, KEY_TCP_RW_TIMEOUT_BEGIN, KEY_TCP_RW_TIMEOUT_END);
	if (strTimeout.empty())
	{
		g_dwCuiServerReadWriteTimeout = 25;
	}
	else
	{
		g_dwCuiServerReadWriteTimeout = (u32)atol(strTimeout.c_str());
	}

	

    CuiWriteLog(g_strLogFile.c_str(), "cuiserver ip = %s, listen port = %hu, cui.fcgi operator timeout = %u, fcgi w/r timeout=%u \n", 
				GetCuiServerIpStr().c_str(), g_wCuiServerTcpListenPort, g_dwCuiServerTimeout, g_dwCuiServerReadWriteTimeout);

	// 读取公共配置信息中的域名
	if ( !GetDomainName( g_strDomainName ) )
	{
		CuiWriteLog(g_strLogFile.c_str(), "获取域名失败\n");

		g_strDomainName = DEF_DOMAIN_NAME;
	}

    SockInit();

    //初始化所有业务消息
    InitCuiEventDesc();
    InitCmsEventDesc();
    InitTacTasEventDesc();
    InitMpsEventDesc();
    InitRcsEventDesc();
    InitUasEventDesc();
	InitGbsEventDesc();
	InitCapsEventDesc();
	InitDPSEvent();

#ifndef WITH_FASTCGI
    if ( argc != 2 )
    {
        printf("Usage: cui.fcgi LocalPort\n");
        return 0;
    }
    int nLocalPort = atoi(argv[1]);
    cusdkService soapProxy(SOAP_C_UTFSTRING|SOAP_IO_KEEPALIVE);
    soapProxy.run(nLocalPort);
#else
    cusdkService soapProxy(SOAP_C_UTFSTRING|SOAP_IO_KEEPALIVE);
    soapProxy.serve();
#endif

	return 0;
}

