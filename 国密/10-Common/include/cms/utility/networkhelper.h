/*****************************************************************************
   ģ����      : Helper
   �ļ���      : networkhelper.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: �����������ò�����������
   ����        : ������
   ��Ȩ        : <Copyright(C) 2009 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���       �߶���      �޸ļ�¼
   2010/01/13  1.0         ������                     ����
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

#define MAX_IF_NUM 32			/*ÿ���豸���ӿ���*/

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

//ptEthInfoListΪNULLʱ�����ص���������
s32 GetEthList(TEthInfoList* ptEthInfoList);



/* ����������Ϣ */
typedef struct {
    u32 nId; 										/*������*/
    u32 nState; 									/*����״̬ �μ� OSP_ETHERNET_ADAPTER_UP ��ö�٣���ʵ�ϵ�ǰʵ��δ�����ߵ�����Ҳ��Ϊdown��*/
    u32 nConnectState;                              /*��������״̬ 0 ��ʾ���ӣ�1��ʾδ���ӣ�2��ʾ���� */
    u8  achName[OSP_ETHERNET_ADAPTER_NAME_LENGTH]; 		    /*������*/
    u8  achDescription[OSP_ETHERNET_ADAPTER_NAME_LENGTH];	/*������Ϣ����*/
    u8  achMacAddr[OSP_ETHERNET_ADAPTER_MAC_LENGTH]; 		/*���������ַ*/
    u32 nIpNum; 									        /*��ַ����*/
    u8  achFullName[OSP_ETHERNET_ADAPTER_MAX_IP][OSP_ETHERNET_ADAPTER_NAME_LENGTH]; 	/*����ȫ��*/
    u32 anIp[OSP_ETHERNET_ADAPTER_MAX_IP]; 				        /*��ַ*/
	u32 anSubNetMask[OSP_ETHERNET_ADAPTER_MAX_IP]; 				/*��������*/
}TEthernetAdapterInfo;

/* ���������Ϣ */
typedef struct {
    u32 nNum; 														     /*��������*/
    TEthernetAdapterInfo atEthernetAdapter[OSP_ETHERNET_ADAPTER_MAX_NUM];/*������Ϣ*/
}TEthernetAdapterInfoAll;


/*========================================================================
�� �� ����GetEthernetAdapterInfoAll
��    �ܣ���ȡ����������Ϣ���������ػ�������
��    ����
�� �� ֵ��TRUE�ɹ� FALSEʧ��
--------------------------------------------------------------------------
�޸ļ�¼��
��      ��  �汾    �޸���  �޸�����
2011/10/19  2801    �¾���  ���
2013/09/03          huzhiyun �޸Ľӿ�ע�ͣ�����Ƿ���������״̬�����TEthernetAdapterInfo::nState˵��
========================================================================*/
BOOL32 GetEthernetAdapterInfoAll(TEthernetAdapterInfoAll * ptEthernetAdapterInfoAll);

const s8 *GetcfgEthx_xValue(u32 nIndex1, u32 nIndex2,const s8 *achKey,s8 *pBuff);
const s8 *GetcfgEthValue(u32 nIndex1, const s8 *achKey,s8 *pBuff);
const s8 *GetcfgGateWayValue(s8 *pGateWayBuff);
s32 GetIfcfgFileNum();
s32 IsIpExist(const s8 *achIpString); //��ȡʧ�ܷ���-1�����ظ�ip����FALSE,û���ظ�ip����TRUE
s32 IsIpOnline(const string &strIp);  //���߷���1�������߷���0��ʧ�ܷ��ظ���

// ��ȡ���ʺϵ�����IP��ַ���㷨����strSrcIpƥ����ı���IP���洢��strDstIp��
bool GetMostAdaptiveIP(const string &strSrcIp, string &strDstIp);
// ��arrIps��ѡ����strSrcIp�ƥ���IP
bool GetMostAdaptiveIP(const vector<string> &arrIps, const string &strSrcIp, string &strDstIp);
// ��ȡ����������IP�б�(mapNicIpΪNic��Ip�Ķ�Ӧ��ϵ)����֧��һ��NIC�϶�IP�ĳ���
bool GetOnlineIpList(map<u32, string> &mapNicIp);
// ��ȡ����������IP�б�
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
//��ȡ�������ߵ�������Ϣ����������ip���������룩����ע��ÿ������Ĭ��ֻ�е�һip��Ч��
bool GetOnlineNicList(map<u32, TNicInfo> &mapNic);
//��ȡ�������ߵ�������Ϣ����������ip���������룩����ע��ÿ������Ĭ��ֻ�е�һip��Ч��
bool GetOnlineNicList(vector<TNicInfo> &vctNic);
//��ȡ�������ߵ�IP��Ϣ����ע��ÿ������Ĭ��ֻ�е�һip��Ч��
bool GetOnlineIpList(vector<string> &vctOnlineIpList);
//��ȡ�ϸ�ƥ��ip���㷨������������ȫ�ڵ��Ĳ���Ϊ��ͬһ����
bool GetAdaptiveIP(const vector<TIpMask> &vctAddrList, const string &strTargetIp, string &strAdapIp);
//��ȡĬ�����ض�Ӧ��������
bool GetDefaultRouteIface(string& strIface);

#endif // _NETWORKHELPER_H_
