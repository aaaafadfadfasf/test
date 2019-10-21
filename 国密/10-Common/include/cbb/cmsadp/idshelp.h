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


//��ȡIP��ַ(�ַ���)
IDSAPI BOOL32 IdsGetIPStr(u32 dwIP, s8 achIPStr[], u32 dwLen);

//osp��ӡ
IDSAPI BOOL32 IdsPrint(const s8 *szFormat, ...);

#define MAX_IDS_WCHAR_LEN	(1024)
#define MAX_IDS_VERSTR_LEN	(128)
#define MAX_IDS_PATH_LEN	(256)  //���·������
#define MAX_IDS_IPADDR_LEN  (16)   //ip��ַ���� 123.456.789.012
#define MAX_IDS_BASESTR_LEN	 (32)
#define MAX_IDS_TIMESTR_LEN	 MAX_IDS_BASESTR_LEN
#define MAX_IDS_MACADDR_LEN	 MAX_IDS_BASESTR_LEN
#define MAX_IDS_NETNAME_LEN	(MAX_IDS_BASESTR_LEN*2)
#define MAX_IDS_NETINF_NUM	(4)

#define IDS_FORMAT_VER(mn, ver) "%s Version: %s (Compile Time: %s, %s)\r\n", #mn, #ver, __TIME__, __DATE__

#define IDS_HARDWARE_LEN   (u32)(50) //Ӳ���汾�ַ�������

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
	s8 m_szMacAddr[MAX_IDS_MACADDR_LEN]; //�ַ�����ʽ
	u8 m_achMacBinary[MAX_IDS_MACADDR_LEN]; //��������ʽ
	s8 m_szDevName[MAX_IDS_MACADDR_LEN];
};

struct TIdsCpuInfo
{
public:
	TIdsCpuInfo()
	{
		memset(this, 0, sizeof(TIdsCpuInfo));
	}
	u8 m_byUsePercent;	//ʹ�ðٷֱ� 0-100
};

struct TIdsMemInfo
{
public:
	TIdsMemInfo()
	{
		memset(this, 0, sizeof(TIdsMemInfo));
	}
	u8 m_byUsePercent;	//ʹ�ðٷֱ� 0-100	
	u32 m_dwPhysicsSize; /* �����ڴ��С(KByte) */	
	u32 m_dwAllocSize; /* ��ʹ���ڴ�(KBytes) */	
};

typedef void (*CpuCheckCB)(enCheckStatus enStat, void *pContext);
typedef void (*MemCheckCB)(enCheckStatus enStat, void *pContext);
typedef void (*NetCheckCB)(enCheckStatus enStat, u8 byEthNo, void *pContext);

//��ʼ��
IDSAPI BOOL32 IdsInitHelpLib();

IDSAPI BOOL32 IdsSetCpuCheck(CpuCheckCB pCheckCB, u8 byAlarmPercent, void *pContext);

IDSAPI BOOL32 IdsSetMemCheck(MemCheckCB pCheckCB, u8 byAlarmPercent, void *pContext);

IDSAPI BOOL32 IdsSetNetCheck(NetCheckCB pCheckCB, void *pContext);

IDSAPI s32 IdsExecCmd(s8* szOutBuf, u16 wOutBufLen, const s8* szCmdFormat, ...);


//�ļ��Ƿ����
IDSAPI BOOL32 IdsExistFile(const s8 *pszFileName);

//Ŀ¼�Ƿ����
IDSAPI BOOL32 IdsExistDir(const s8 *pszDirName);

//����Ŀ¼��ǿ�ҽ��鲻ʹ�øú�����ʹ��IdsCreateDirEx����
IDSAPI BOOL32 IdsCreateDir(const s8 *pszDirName, u32 dwDirLen);

//����Ŀ¼
IDSAPI BOOL32 IdsCreateDirEx(const s8 *pszDirName, u32 dwDirLen, u16& wErrorCode);

//��ȡIP��ַ(������)
IDSAPI u32 IdsGetIPAddr(const s8* pszIpAddress);

//��ȡ"127.0.0.1"��IP��ַ
IDSAPI u32 IdsGetLoopBackIP();


//����������
IDSAPI BOOL32 IdsCreateLink(const s8* pszSrcFile, const s8* pszDstFile);

//ɾ���ļ�
IDSAPI BOOL32 IdsRemoveFile(const s8* pszFileName);

//ɾ������Ŀ¼�ļ�
IDSAPI BOOL32 IdsRemoveFiles(const s8* pszDirName);

// ��ȡϵͳIP��ַ
IDSAPI BOOL32 IdsGetNetInfo(TIdsNetInfo* ptNetInfo);
//��������������ʹ��
IDSAPI u16 IdsGetIPInfo(TSysNetInfo atNetINfo[], u16 wMaxNetNum);
IDSAPI u32 IdsGetGateway(void);

// ����ϵͳIP��ַ
IDSAPI BOOL32 IdsSetNetInfo(TIdsNetInfo* ptNetInfo);

//���õ�������IP��ַ���ṩkdmsearch����
IDSAPI BOOL32 IdsSetIndivialNetInfo(u32 dwOriginalIP, TSysNetInfo tNewInfo);

//��ȡ������������
IDSAPI BOOL32 IdsGetParamValue(const s8 *const s, const s8 *const szParam, s8 *szValue);

// �ַ���str2�Ƿ����ַ���str1�У��Ҳ����򷵻ؿ�
// �ҵ��򷵻�str1��str2��ʼ���ַ���
IDSAPI s8* IdsStrInStr(const s8* str1, const s8* str2);

// �ַ�����, ����һ���ַ�c����һ���ַ���strSrc��ĩ�γ��ֵ�λ��
IDSAPI s8* IdsStrrch(s8* strSrc, const char c); // 2014.6.5 modify by xjb for Bug00183701

//��ȡe2prom�汾��Ϣ(�ַ���)
IDSAPI BOOL32 IdsGetHardwareInfo(s8 achHardware[], u32 dwLen);

//��ȡe2prom�汾��Ϣ(�ַ���)
IDSAPI BOOL32 IdsGetProductHardWare(s8 achHardware[], u32 dwLen, s32& nProductType, 
				const s8* pszDefaultProduct = NULL, const s8* pszDefaultPath = NULL);

//�ַ���ת��ΪСд
IDSAPI BOOL32 IdsToLower(const s8 achIn[],s8 achLower[]);

//��ȡMAC��ַ
IDSAPI BOOL32 IdsGetMacAddr(const s8 *achDevName, TIdsMacInfo &tIdsMacInfo);

//��ȡ��ǰʱ��
IDSAPI BOOL32 IdsGetCurTimeStr(s8* pachString, u32 dwStringLen);

//��ȡapache���õ������Ϣ��Ŀǰ��web�����˿�
IDSAPI BOOL32 IdsGetApacheCfgInfo(const s8* pszConfPath, u16 &wPort); // 2014.5.21 add by xjb for Bug00182018 

/*====================================================================
������    ��IdsGetVersionStr  ���ܣ���ȡ��Ʒ�İ汾��Ϣ
����˵��  ��pszModuleName  ģ����(eg: tvm)�����߲�Ʒ��(eg: KDM201-TVM)
			pszModuleVer: �汾��(eg: v1r1fix3)
			pszModuleType: ��Ʒ�ͺţ�����L, W, L64, W64
			pszOutVerStr: �����
			dwOutVerStrlen��������ĳ���
����ֵ˵�����ɹ���ʧ��
----------------------------------------------------------------------
�޸ļ�¼  ��
��  ��      �汾        �޸���            �߶���          �޸ļ�¼
2014/6/3   V1.0        ������             
====================================================================*/
IDSAPI BOOL32 IdsGetModuleVersion(const s8* pszModuleName, const s8* pszModuleVer, const s8 *pszModuleType,
							   s8 *pszOutVerStr, u32 dwOutVerStrlen);

/*====================================================================
������    ��IdsGetExeDir������ ����ȡ����ִ��Ŀ¼ ��: /opt/kdm/mts/mtu/
����˵��  ��pszMainStr: ����ִ��·���ַ���ָ��
			dwOutVerStrlen������ִ���ַ������ȣ���������MAX_IDS_PATH_LEN(256)
����ֵ˵���� �ɹ���ʧ��         
====================================================================*/
IDSAPI BOOL32 IdsGetExeDir(s8 *pszMainStr, u32 dwMainStrlen);

/*====================================================================
������      IdsGetProductDir  �����ܣ���ȡ��Ʒִ��Ŀ¼ /opt/kdm/mts/
����˵��  ��pszMainStr: ����ִ��·���ַ���ָ�� 
			dwOutVerStrlen������ִ���ַ������ȣ���������MAX_IDS_PATH_LEN(256)
����ֵ˵���� �ɹ���ʧ��          
====================================================================*/
IDSAPI BOOL32 IdsGetProductDir(s8 *pszMainStr, u32 dwMainStrlen);

/*====================================================================
������      IdsGetAbsFilePath �����ܣ�ƴ���ļ����·��
����˵��  ��pszFileDir: �ļ�·�� /opt/kdm/mts/
			pszFileName���ļ���  mtscfg.ini
			pszFileAbsPath: ��� /opt/kdm/mts/mtscfg.ini,��������MAX_IDS_PATH_LEN(256)
����ֵ˵�����Ƿ�ɹ�     
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
������      IdsGetState,   ���ܣ���ȡ״̬�����ַ�
����˵��  ��enIdsState
����ֵ˵����״̬�����ַ���           
====================================================================*/
IDSAPI const s8* IdsGetStateStr(u8 byState);

/*====================================================================
������      IdsCheckIpConflict,   ���ܣ����IP��ͻ
����˵��  ��ip��ַ
����ֵ˵����TRUEΪ����ͻ��FALSE��ͻ      
====================================================================*/
IDSAPI BOOL32 IdsCheckIpConflict(u32 dwAddr);		

#endif




