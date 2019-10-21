/*****************************************************************************
   模块名      : Helper
   文件名      : networkhelper.h
   相关文件    : 
   文件实现功能: 定义网络配置操作辅助函数
   作者        : 黄至春
   版权        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人       走读人      修改记录
   2010/01/13  1.0         黄至春                     创建
******************************************************************************/
#ifndef _NETWORKHELPER_H_
#define _NETWORKHELPER_H_

#include "helpercommon.h"
#include "mischelper.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <list>
using std::list;

#ifndef IF_NAMESIZE
#define  IF_NAMESIZE            (u32)16
#endif

#define MAX_IF_NUM 32			/*每个设备最大接口数*/

#define IFCFG(ethname) "/etc/sysconfig/network-scripts/ifcfg-"#ethname

char* InetNtoA(u32 dwAddr);

struct TEthToolOpts
{
	TEthToolOpts()
	{
		memset(this, 0, sizeof(TEthToolOpts));
	}

	s8* ToString(s8* lpszBuf, s32 nLen)
	{
		SNPRINTF(lpszBuf, nLen, "speed %d autoneg %s", m_nSpeed, m_achAutoNeg);
	}

	s32 m_nSpeed;
    s8 m_achDuplex[6]; //full half
	s8 m_achAutoNeg[5];//TAutoNeg emAutoNeg;
};

class CIfcfg
{
public:

	BOOL32 GetEthToolOpts(const s8* lpszDevName, TEthToolOpts& tEthToolOpts);
	BOOL32 SetEthToolOpts(const s8* lpszDevName, TEthToolOpts& tEthToolOpts);
};

struct TEthInfo
{
	TEthInfo()
	{
		memset(this, 0, sizeof(TEthInfo));
	}

	s32 m_nEthIdx;
	s8 m_achName[IF_NAMESIZE];
};

struct TEthInfoList
{
	TEthInfoList()
	{
		memset(this, 0, sizeof(TEthInfo));
	}

	u32 nNum;
	TEthInfo m_atEthInfo[MAX_IF_NUM];
};

//ptEthInfoList为NULL时，返回的是网卡数
s32 GetEthList(TEthInfoList* ptEthInfoList);



/* 单个网卡信息 */
typedef struct {
    u32 nId; 										/*网卡号*/
    u32 nState; 									/*网卡状态 参见 OSP_ETHERNET_ADAPTER_UP 等枚举（事实上当前实现未接网线的网卡也会为down）*/
    u32 nConnectState;                              /*网卡连接状态 0 表示连接，1表示未连接，2表示出错 */
    u8  achName[OSP_ETHERNET_ADAPTER_NAME_LENGTH]; 		    /*网卡名*/
    u8  achDescription[OSP_ETHERNET_ADAPTER_NAME_LENGTH];	/*网卡信息描述*/
    u8  achMacAddr[OSP_ETHERNET_ADAPTER_MAC_LENGTH]; 		/*网卡物理地址*/
    u32 nIpNum; 									        /*地址数量*/
    u8  achFullName[OSP_ETHERNET_ADAPTER_MAX_IP][OSP_ETHERNET_ADAPTER_NAME_LENGTH]; 	/*网卡全名*/
    u32 anIp[OSP_ETHERNET_ADAPTER_MAX_IP]; 				        /*地址*/
	u32 anSubNetMask[OSP_ETHERNET_ADAPTER_MAX_IP]; 				/*子网掩码*/
}TEthernetAdapterInfo;

/* 多个网卡信息 */
typedef struct {
    u32 nNum; 														     /*网卡数量*/
    TEthernetAdapterInfo atEthernetAdapter[OSP_ETHERNET_ADAPTER_MAX_NUM];/*网卡信息*/
}TEthernetAdapterInfoAll;


/*========================================================================
函 数 名：GetEthernetAdapterInfoAll
功    能：获取所有网卡信息（不包括回环网卡）
参    数：
返 回 值：TRUE成功 FALSE失败
--------------------------------------------------------------------------
修改记录：
日      期  版本    修改人  修改内容
2011/10/19  2801    陈晶晶  添加
2013/09/03          huzhiyun 修改接口注释，添加是否连接网络状态和添加TEthernetAdapterInfo::nState说明
========================================================================*/
BOOL32 GetEthernetAdapterInfoAll(TEthernetAdapterInfoAll * ptEthernetAdapterInfoAll);

const s8 *GetcfgEthx_xValue(u32 nIndex1, u32 nIndex2,const s8 *achKey,s8 *pBuff);
const s8 *GetcfgEthValue(u32 nIndex1, const s8 *achKey,s8 *pBuff);
const s8 *GetcfgGateWayValue(s8 *pGateWayBuff);
s32 GetIfcfgFileNum();
s32 IsIpExist(const s8 *achIpString); //读取失败返回-1，有重复ip返回FALSE,没有重复ip返回TRUE
s32 IsIpOnline(const string &strIp);  //在线返回1，不在线返回0，失败返回负数

// 获取最适合的在线IP地址，算法：与strSrcIp匹配最长的本机IP（存储于strDstIp）
bool GetMostAdaptiveIP(const string &strSrcIp, string &strDstIp);
// 从arrIps中选择与strSrcIp最长匹配的IP
bool GetMostAdaptiveIP(const vector<string> &arrIps, const string &strSrcIp, string &strDstIp);
// 获取在线网卡的IP列表(mapNicIp为Nic与Ip的对应关系)，不支持一个NIC上多IP的场景
bool GetOnlineIpList(map<u32, string> &mapNicIp);
// 获取在线网卡的IP列表
bool GetOnlineIpList(list<string> &lstIp);

struct TNicInfo
{
	string strNicName;
	string strNicIp;
	string strNicMask;
};
struct TIpMask
{
	string strIp;
	string strMask;
};
//获取所有在线的网卡信息（网卡名，ip，子网掩码）（备注：每个网卡默认只有第一ip有效）
bool GetOnlineNicList(map<u32, TNicInfo> &mapNic);
//获取所有在线的网卡信息（网卡名，ip，子网掩码）（备注：每个网卡默认只有第一ip有效）
bool GetOnlineNicList(vector<TNicInfo> &vctNic);
//获取所有在线的IP信息（备注：每个网卡默认只有第一ip有效）
bool GetOnlineIpList(vector<string> &vctOnlineIpList);
//获取严格匹配ip，算法是子网掩码完全掩掉的才认为是同一网段
bool GetAdaptiveIP(const vector<TIpMask> &vctAddrList, const string &strTargetIp, string &strAdapIp);
//获取默认网关对应的网卡名
bool GetDefaultRouteIface(string& strIface);

#endif // _NETWORKHELPER_H_
