#include "osp.h"
#include "kdvtype.h"

#ifndef _IDSHELP_H_
#define _IDSHELP_H_

#ifndef IDSAPI
#define IDSAPI extern "C"
#endif


#ifdef WIN32
#pragma comment(lib, "iphlpapi.lib")
#endif


//获取IP地址(字符串)
IDSAPI BOOL32 IdsGetIPStr(u32 dwIP, s8 achIPStr[], u32 dwLen);

//osp打印
IDSAPI BOOL32 IdsPrint(const s8 *szFormat, ...);

#define MAX_IDS_WCHAR_LEN	(1024)
#define MAX_IDS_VERSTR_LEN	(128)
#define MAX_IDS_PATH_LEN	(256)  //最大路径长度
#define MAX_IDS_IPADDR_LEN  (16)   //ip地址长度 123.456.789.012
#define MAX_IDS_BASESTR_LEN	 (32)
#define MAX_IDS_TIMESTR_LEN	 MAX_IDS_BASESTR_LEN
#define MAX_IDS_MACADDR_LEN	 MAX_IDS_BASESTR_LEN
#define MAX_IDS_NETNAME_LEN	(MAX_IDS_BASESTR_LEN*2)
#define MAX_IDS_NETINF_NUM	(4)

#define IDS_FORMAT_VER(mn, ver) "%s Version: %s (Compile Time: %s, %s)\r\n", #mn, #ver, __TIME__, __DATE__

#define IDS_HARDWARE_LEN   (u32)(50) //硬件版本字符串长度

#define PRODUCT_STR        "PRODUCT: "
#define HARDWARE_VER_STR   "Hardware Ver: "



enum enCheckStatus
{
	Status_Occur,
	Status_Resume,
};

enum enIdsErrorCode
{
	IDS_ERR_NO = 0,
	IDS_ERR_TOO_MANY_LINKS = 1, 
	IDS_ERR_CREATEDIR_FAIL = 2, 	
};

enum enIdsState
{
	IDS_STATE_IDLE = 0,
	IDS_STATE_SERVICE = 1,
};

enum enProductType
{
	IDS_PRODUCT_HARDWARE = 0,
	IDS_PRODUCT_SOFTWARE,
};

struct TSysNetInfo
{
public:
	TSysNetInfo(){
		memset(this, 0, sizeof(TSysNetInfo));
	}
	s8 m_szNetName[MAX_IDS_NETNAME_LEN];
	s8 m_szNetDesc[MAX_IDS_NETNAME_LEN];
	u32 m_dwNetIP;
	u32 m_dwNetMask;
	u32 m_dwGateWay;
void Print(){
		s8 szIP[MAX_IDS_IPADDR_LEN] = {0};
		IdsGetIPStr(m_dwNetIP, szIP, sizeof(szIP));
		IdsPrint("IP:%s  ", szIP);
		IdsGetIPStr(m_dwNetMask, szIP, sizeof(szIP));
		IdsPrint("Mask:%s  ", szIP);
		IdsGetIPStr(m_dwGateWay, szIP, sizeof(szIP));
		IdsPrint("Gateway:%s\n", szIP);
		IdsPrint("Name: %s, Desc:%s\n", m_szNetName, m_szNetDesc);
	}
};

struct TIdsNetInfo
{
public:
	TIdsNetInfo()
	{
		memset(this, 0, sizeof(TIdsNetInfo));
	}
	TSysNetInfo m_tNetList[MAX_IDS_NETINF_NUM];
	u8     m_byNetNum;
	u32	   m_dwDftGateway;
	void Print(){
		s8 szIP[MAX_IDS_IPADDR_LEN] = {0};
		IdsGetIPStr(m_dwDftGateway, szIP, sizeof(szIP));
		IdsPrint("DefaultGateway: %s, NetNum:%d\n", szIP, m_byNetNum);
		for (u8 i = 0; i < m_byNetNum; i++){
			m_tNetList[i].Print();
		}
	}
};
struct TIdsMacInfo
{
public:
	TIdsMacInfo(){memset(this, 0, sizeof(TIdsMacInfo));	}
	void SetMacAddr(LPCSTR lpszMacAddr)	{
		strncpy(m_szMacAddr, lpszMacAddr, sizeof(m_szMacAddr)-1);
		m_szMacAddr[sizeof(m_szMacAddr)-1] = 0;
	}
	s8* GetMacAddr(void){return m_szMacAddr;}
	void SetMacBinary(u8 abyMacAddr[])	{
		for(u8 byIndex = 0; byIndex < MAX_IDS_MACADDR_LEN; byIndex++)
		{m_achMacBinary[byIndex]=abyMacAddr[byIndex];}
	}
	u8* GetMacBinary(void){return m_achMacBinary;}
	void SetDevName(LPCSTR lpszDevName){
		strncpy(m_szDevName, lpszDevName, sizeof(m_szDevName)-1);
		m_szDevName[sizeof(m_szDevName)-1] = 0;
	}
	s8* GetDevName(void){return m_szDevName;}
protected:
	s8 m_szMacAddr[MAX_IDS_MACADDR_LEN]; //字符串形式
	u8 m_achMacBinary[MAX_IDS_MACADDR_LEN]; //二进制形式
	s8 m_szDevName[MAX_IDS_MACADDR_LEN];
};

struct TIdsCpuInfo
{
public:
	TIdsCpuInfo()
	{
		memset(this, 0, sizeof(TIdsCpuInfo));
	}
	u8 m_byUsePercent;	//使用百分比 0-100
};

struct TIdsMemInfo
{
public:
	TIdsMemInfo()
	{
		memset(this, 0, sizeof(TIdsMemInfo));
	}
	u8 m_byUsePercent;	//使用百分比 0-100	
	u32 m_dwPhysicsSize; /* 物理内存大小(KByte) */	
	u32 m_dwAllocSize; /* 已使用内存(KBytes) */	
};

typedef void (*CpuCheckCB)(enCheckStatus enStat, void *pContext);
typedef void (*MemCheckCB)(enCheckStatus enStat, void *pContext);
typedef void (*NetCheckCB)(enCheckStatus enStat, u8 byEthNo, void *pContext);

//初始化
IDSAPI BOOL32 IdsInitHelpLib();

IDSAPI BOOL32 IdsSetCpuCheck(CpuCheckCB pCheckCB, u8 byAlarmPercent, void *pContext);

IDSAPI BOOL32 IdsSetMemCheck(MemCheckCB pCheckCB, u8 byAlarmPercent, void *pContext);

IDSAPI BOOL32 IdsSetNetCheck(NetCheckCB pCheckCB, void *pContext);

IDSAPI s32 IdsExecCmd(s8* szOutBuf, u16 wOutBufLen, const s8* szCmdFormat, ...);


//文件是否存在
IDSAPI BOOL32 IdsExistFile(const s8 *pszFileName);

//目录是否存在
IDSAPI BOOL32 IdsExistDir(const s8 *pszDirName);

//创建目录，强烈建议不使用该函数，使用IdsCreateDirEx函数
IDSAPI BOOL32 IdsCreateDir(const s8 *pszDirName, u32 dwDirLen);

//创建目录
IDSAPI BOOL32 IdsCreateDirEx(const s8 *pszDirName, u32 dwDirLen, u16& wErrorCode);

//获取IP地址(网络序)
IDSAPI u32 IdsGetIPAddr(const s8* pszIpAddress);

//获取"127.0.0.1"的IP地址
IDSAPI u32 IdsGetLoopBackIP();


//创建软链接
IDSAPI BOOL32 IdsCreateLink(const s8* pszSrcFile, const s8* pszDstFile);

//删除文件
IDSAPI BOOL32 IdsRemoveFile(const s8* pszFileName);

//删除单层目录文件
IDSAPI BOOL32 IdsRemoveFiles(const s8* pszDirName);

// 获取系统IP地址
IDSAPI BOOL32 IdsGetNetInfo(TIdsNetInfo* ptNetInfo);
//以下两函数不再使用
IDSAPI u16 IdsGetIPInfo(TSysNetInfo atNetINfo[], u16 wMaxNetNum);
IDSAPI u32 IdsGetGateway(void);

// 设置系统IP地址
IDSAPI BOOL32 IdsSetNetInfo(TIdsNetInfo* ptNetInfo);

//设置单独网卡IP地址，提供kdmsearch服务
IDSAPI BOOL32 IdsSetIndivialNetInfo(u32 dwOriginalIP, TSysNetInfo tNewInfo);

//获取参数配置数据
IDSAPI BOOL32 IdsGetParamValue(const s8 *const s, const s8 *const szParam, s8 *szValue);

// 字符串str2是否在字符串str1中，找不到则返回空
// 找到则返回str1中str2开始的字符串
IDSAPI s8* IdsStrInStr(const s8* str1, const s8* str2);

// 字符查找, 查找一个字符c在另一个字符串strSrc中末次出现的位置
IDSAPI s8* IdsStrrch(s8* strSrc, const char c); // 2014.6.5 modify by xjb for Bug00183701

//获取e2prom版本信息(字符串)
IDSAPI BOOL32 IdsGetHardwareInfo(s8 achHardware[], u32 dwLen);

//获取e2prom版本信息(字符串)
IDSAPI BOOL32 IdsGetProductHardWare(s8 achHardware[], u32 dwLen, s32& nProductType, 
				const s8* pszDefaultProduct = NULL, const s8* pszDefaultPath = NULL);

//字符串转换为小写
IDSAPI BOOL32 IdsToLower(const s8 achIn[],s8 achLower[]);

//获取MAC地址
IDSAPI BOOL32 IdsGetMacAddr(const s8 *achDevName, TIdsMacInfo &tIdsMacInfo);

//获取当前时间
IDSAPI BOOL32 IdsGetCurTimeStr(s8* pachString, u32 dwStringLen);

//获取apache配置的相关信息，目前是web监听端口
IDSAPI BOOL32 IdsGetApacheCfgInfo(const s8* pszConfPath, u16 &wPort); // 2014.5.21 add by xjb for Bug00182018 

/*====================================================================
函数名    ：IdsGetVersionStr  功能：获取产品的版本信息
参数说明  ：pszModuleName  模块名(eg: tvm)，或者产品名(eg: KDM201-TVM)
			pszModuleVer: 版本号(eg: v1r1fix3)
			pszModuleType: 产品型号，比如L, W, L64, W64
			pszOutVerStr: 输出串
			dwOutVerStrlen：输出串的长度
返回值说明：成功或失败
----------------------------------------------------------------------
修改记录  ：
日  期      版本        修改人            走读人          修改记录
2014/6/3   V1.0        包增辉             
====================================================================*/
IDSAPI BOOL32 IdsGetModuleVersion(const s8* pszModuleName, const s8* pszModuleVer, const s8 *pszModuleType,
							   s8 *pszOutVerStr, u32 dwOutVerStrlen);

/*====================================================================
函数名    ：IdsGetExeDir，功能 ：获取程序执行目录 例: /opt/kdm/mts/mtu/
参数说明  ：pszMainStr: 程序执行路径字符串指针
			dwOutVerStrlen：程序执行字符串长度，不得少于MAX_IDS_PATH_LEN(256)
返回值说明： 成功或失败         
====================================================================*/
IDSAPI BOOL32 IdsGetExeDir(s8 *pszMainStr, u32 dwMainStrlen);

/*====================================================================
函数名      IdsGetProductDir  ，功能：获取产品执行目录 /opt/kdm/mts/
参数说明  ：pszMainStr: 程序执行路径字符串指针 
			dwOutVerStrlen：程序执行字符串长度，不得少于MAX_IDS_PATH_LEN(256)
返回值说明： 成功或失败          
====================================================================*/
IDSAPI BOOL32 IdsGetProductDir(s8 *pszMainStr, u32 dwMainStrlen);

/*====================================================================
函数名      IdsGetAbsFilePath ，功能：拼凑文件完成路径
参数说明  ：pszFileDir: 文件路径 /opt/kdm/mts/
			pszFileName：文件名  mtscfg.ini
			pszFileAbsPath: 输出 /opt/kdm/mts/mtscfg.ini,不得少于MAX_IDS_PATH_LEN(256)
返回值说明：是否成功     
====================================================================*/
IDSAPI BOOL32 IdsGetAbsFilePath(const s8* pszFileDir, const s8* pszFileName, s8* pszFileAbsPath, u32 dwPathlen);

IDSAPI void IdsGetCpuInfo(TIdsCpuInfo& tInfo);

IDSAPI void IdsGetMemInfo(TIdsMemInfo& tInfo);

IDSAPI s32 IdsTransUtf8ToAnsi( const s8 *pchBufUtf8, s8 *pchBufAnsi, s32 nBufAnsiLen);
IDSAPI s32 IdsTransAnsiToUtf8( const s8 *pchBufAnsi, s8 *pchBufUtf8, s32 nBufUtf8Len);
IDSAPI s32 IdsTransWCharToAnsi(u16 wWChar, s8 *pchBufAnsi, s32 nBufAnsiLen);
IDSAPI s32 IdsProcessSpecialChars(const s8 * szInPut,s8 * szOutPut);
IDSAPI s32 IdsReplaceStr(const s8* szInBuf, s8* szOutBuf, const s8* szMatchStr, const s8* szReplaceStr);

/*====================================================================
函数名      IdsGetState,   功能：获取状态描述字符
参数说明  ：enIdsState
返回值说明：状态描述字符串           
====================================================================*/
IDSAPI const s8* IdsGetStateStr(u8 byState);

/*====================================================================
函数名      IdsCheckIpConflict,   功能：检测IP冲突
参数说明  ：ip地址
返回值说明：TRUE为不冲突，FALSE冲突      
====================================================================*/
IDSAPI BOOL32 IdsCheckIpConflict(u32 dwAddr);		

#endif




