/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cmcfg.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: commonconfig �ṩ�������õĶ�д�ӿ�
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/14   1.0         liangli        ����
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
    string platformIP;     // Natӳ��ǰ
    int platformPort;
    string platformNatIP;  // Natӳ���
    int platformNatPort;
	string protocol;	   // Э�飬TCP/UDP
};

//���������ýṹ��ȡ���ӿ��У������ϲ�ҵ��һ���Ի�ȡ��������
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

    string m_parentProxyUrl;	// �ϼ�Proxy Url��

    string m_strStunIP;
    unsigned short m_wStunPort;

    bool    m_bEnablePunch;

    bool    m_bIsDefenseCrossStreamEnable;   //��������������

    unsigned short m_wNmsPort;

    string m_strLocalIP;
    vector<CCommonNetNatInfo> m_vecNatInfo;
    vector<CCommonNetNatInfo> m_vecOtherNatInfo;   //Ϊ��ƽ̨���õĶ˿�ӳ��

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
������      : ReadCommonConfig
����        : �ڲ�����������£���Ҫ���¶�ȡ�����������Ҫ���ô˽ӿ�
�㷨ʵ��    :
             1. ReadCommonConfig��bool bIsChkDevMark = true��������Ĭ�ϲ�����                ��ʶ�Ƿ���DevMark�ĺϷ��ԡ�Ŀǰֻ��vtduģ����Ҫ���ò�����Ϊfalse��             2. ֻ�����漸������ģ�鶼�õ����������ȡʱ�����Ƿ��ȡ�ɹ���                1)����UUID  2)��������   3)proxy��ַ   4)proxy�˿�  5)����IP��ַ                �⼸������κ�һ����ȡʧ�ܳ���ͻ��˳����������ȡʧ�ܾ�ʹ��Ĭ��ֵ
����ȫ�ֱ���:
�������˵��:
             bIsChkDevMark:�Ƿ���devMark��Ĭ�ϼ��
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/21/             liangli
====================================================================*/
bool ReadCommonConfig(bool bIsChkDevMark = true);

/*====================================================================
������      : SaveCommonConfig
����        : �޸Ĺ��������������뱣�浽�ļ��У���Ҫ���ô˽ӿ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/21/             liangli
====================================================================*/
bool SaveCommonConfig();

/*====================================================================
������      : GetCommonConfig
����        : ��ȡCommonConfig�����ṹ
�㷨ʵ��    : �ȵ���ReadCommonConfig()����ܵ���GetCommonConfig�����ص��Ƕ��󣬲�������
����ȫ�ֱ���:
�������˵��:
             
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/05/13/             fanxg
====================================================================*/
CCommonConfig GetCommonConfig();

/*====================================================================
������      : GetGlobalDeviceUUID
����        : ���ر������UUID
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
string GetDomainUUID();
bool SetDomainUUID(const string &strUUID);

/*====================================================================
������      : GetDomainName
����        : �����������ַ���
              ���е������ַ���Լ��ΪȫСд, ������Сд��ĸ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
string GetDomainName();
bool SetDomainName(const string &strName);

/*====================================================================
������      : GetParentDomainUUID
����        : ���ظ������UUID
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/01/08/             huzhiyun
====================================================================*/
string GetParentDomainUUID();
bool SetParentDomainUUID(const string &strUUID);

/*====================================================================
������      : GetParentDomainName
����        : �����������ַ���
              ���е������ַ���Լ��ΪȫСд, ������Сд��ĸ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/01/08/             huzhiyun
====================================================================*/
string GetParentDomainName();
bool SetParentDomainName(const string &strName);

/*====================================================================
������      : GetProxyIPAddress
����        : ����Proxy�ĵ�ַ
              һ����ֻ��һ��Proxy��һ��3as��һ��cmu
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
string GetProxyIPAddress();
bool SetProxyIPAddress(const string &strIpAddr);

/*====================================================================
������      : GetProxyPort
����        : ����Proxy�Ķ˿�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
unsigned short GetProxyPort();
bool SetProxyPort(const unsigned short wPort);

/*====================================================================
������      : GetParentPlatformProxyUrl / SetParentPlatformProxyUrl
����        : ��ú��޸��ϼ�ƽ̨��Proxy��ַ��URL��ʽ ip:port )
              һ����ֻ��һ���ϼ���ͬ�ϼ�ͨ��ֻ��ҪProxy��ַ��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/03/06/             lizhixing
====================================================================*/
string GetParentPlatformProxyUrl();
bool SetParentPlatformProxyUrl(const string &proxyUrl );

/*====================================================================
������      : GetStunIPAddress
����        : ��ȡSTUN��������ַ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/21/             liangli
====================================================================*/
string GetStunIPAddress();
bool SetStunIPAddress(const string &strIpAddr);

unsigned short GetStunPort();
bool SetStunPort(const unsigned short wPort);

/*====================================================================
������      : GetEnablePunch
����        : �Ƿ�����Punch
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/04/15            huangzhichun
====================================================================*/
bool GetEnablePunch();
bool SetEnablePunch(const bool &bEnbale);

/*====================================================================
������      : GetNmsPort
����        : ��ȡ���ܷ�������ַ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2013/04/01/             huzhiyun
====================================================================*/
unsigned short GetNmsPort();
bool SetNmsPort(const unsigned short wPort);

/*====================================================================
������      : GetLogDir
����        : Ŀǰ2.0ģ�����־�Ƚϴ�������commonconfig���ṩһ��ȫ������
             ����ģ����ݴ����ý���־д��ͳһָ��Ŀ¼
             ��־·��Ĭ��/var/log/(��֤��/�ַ���β��win��Ĭ��c:\kdm)

             ����ģ���ڳ�ʼ�� OspExtInit() ʱ
             string strLogDir = GetLogDir();
             TOspExtConf tOspExtConf;
             tOspExtConf.strErrLogDir = strLogDir + "cui/errlog/";
             tOspExtConf.strRunLogDir = strLogDir + "cui/runlog/"
             if ( ! OspExtInit(tOspExtConf) )

             �� OspSipInit()Ҳ��ͬ������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/3/21/             liangli
====================================================================*/
string GetLogDir();
bool SetLogDir(const string &strDir);


/*====================================================================
������      : GetLocalIPAddress
����        : ��ȡ��������ҵ��ͨ�ŵĵ�ַ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/10/10/            huangzhichun
====================================================================*/
string GetLocalIPAddress();
bool SetLocalIPAddress(const string &strIpAddr);

/*====================================================================
������      : GetCommonCmsVer
����        : ��ȡƽ̨��Cms�汾��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCmsVer();
bool SetCommonCmsVer(const string &strVer);

/*====================================================================
������      : GetCommonCmsVer
����        : ��ȡƽ̨��Cms��λ����X86Ϊ32λ��X86_64Ϊ64λ������cms_const)
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  :
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCmsArch();
bool SetCommonCmsArch(const string &strVer);

/*====================================================================
������      : GetCommonCuVer
����        : ��ȡƽ̨��cu�汾��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCuVer();
bool SetCommonCuVer(const string &strVer);

/*====================================================================
������      : GetCommonCuUpdateUrl
����        : ��ȡƽ̨��cu url·��
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/30/            huzhiyun
====================================================================*/
string GetCommonCuUpdateUrl();
bool SetCommonCuUpdateUrl(const string &strCuUpdateUrl);

/*====================================================================
������      : GetCommonNetNatInfo
����        : ��ȡ����NATӳ����Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/30/            huzhiyun
====================================================================*/
vector<CCommonNetNatInfo> GetCommonNetNatInfo();
bool SetCommonNetNatInfo(const vector<CCommonNetNatInfo> &vecNat);
bool AddCommonNetNatInfo(const CCommonNetNatInfo &cInfo);
bool DelCommonNetNatInfo(const TNetAddr &tAddr, const string &strProtocol);
bool ClearCommonNetNatInfo(const string &strPlatIp);
bool FindNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);

/*====================================================================
������      : GetCommonOtherNetNatInfo
����        : ��ȡ����NATӳ����Ϣ����ƽ̨�ģ���Ϊ���ƻ�ԭ�й��ܲ��ֶΡ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2016/10/17/           ����
====================================================================*/
vector<CCommonNetNatInfo> GetCommonOtherNetNatInfo();
bool SetCommonOtherNetNatInfo(const vector<CCommonNetNatInfo> &vecNat);
bool AddCommonOtherNetNatInfo(const CCommonNetNatInfo &cInfo);
bool DelCommonOtherNetNatInfo(const TNetAddr &tAddr, const string &strProtocol);
bool ClearOtherCommonNetNatInfo(const string &strPlatIp);
bool FindOtherNetNatAddr(const TNetAddr& tAddr, TNetAddr& tMapedAddr, const string &strProtocol = NAT_PROTOCOL_TCP);


/*====================================================================
������      : GetRedisIP
����        : ��ȡredis��ip,port,password��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2016/03/29/            huzhiyun
====================================================================*/
string GetRedisIP();
bool SetRedisIP(const string &strIpAddr);
unsigned short GetRedisPort();
bool SetRedisPort(unsigned short wPort);
string GetRedisPassword();


/*====================================================================
������      : GetMysql
����        : ��ȡmysql��user,password,port,secure(�Ƿ����)��Ϣ
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2017/08/21/            zhouxuan
====================================================================*/
string GetMysqlUser();
string GetMysqlPassword();
unsigned short GetMysqlPort();
bool GetMysqlSecure();
//////////////////////////////////////////////////////////////////////////
//��ĸ����������ʹ��

/*====================================================================
������      : CreateDeviceMark
����        : �����豸�ļ�����������ĸ�����߹���ʩ��ʱ��С����ʹ��
�㷨ʵ��    : 1�������ļ�
             2�������豸UUID
             3��д���ļ�
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/15/             liangli
====================================================================*/
bool CreateDeviceMark();

//////////////////////////////////////////////////////////////////////////
//һЩ�����ӿ�

bool UnifyDir( string &strNewDir );

/*=============================================================================
�� �� ���� IsDigital
��    �ܣ� check if the string is all digital
�㷨ʵ�֣� 
ȫ�ֱ����� 
��    ���� s8* pchAlias
�� �� ֵ�� inline BOOL32 
=============================================================================*/
bool IsDigital(const string &str);

/*==================================================================== 
    ������  ��CreateDir
    ����    ��create directory
    ����ȫ�ֱ�����
    �������˵����s8 *pszNewDir  -- [in] new directory
    ����ֵ˵����TRUE: create directory success
                FALSE: create directory failed
----------------------------------------------------------------------
    �޸ļ�¼:
    ��  ��      �汾		�޸���		�߶���      �޸�����
====================================================================*/       
bool CreateDir( const string &strNewDir );

/*====================================================================
������      : GetModuleFullPathAndFileName
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : ���ص�ǰ�����ļ��ľ���·�� ��ģ���ļ���
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
bool GetModuleFullPathAndFileName(string &strFullPath, string &strFileName);

/*====================================================================
������      : IsFileExist
����        : 
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: const string strFile   �������ļ������ļ���
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
bool IsFileExist(const string strFile);

/*==================================================================== 
������  ��DeleteNullDir
����    ��delete null directory
����ȫ�ֱ�����
�������˵����LPCSTR pszDir  -- [in] delete null directory
����ֵ˵����TRUE: delete directory success
FALSE: delete directory failed
----------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾		�޸���		�߶���      �޸�����
====================================================================*/   
bool DeleteNullDir(const string &strDir);

/*====================================================================
������      : CMSDeleteFile
����        : ɾ��һ���ļ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/14/             liangli
====================================================================*/
bool CMSDeleteFile(const string &strFile);

/*========================================================================
������      : IsAlreadyExist
����        : �жϽ����Ƿ��Ѿ�����
�㷨ʵ��    : ����/proc�µ��ļ�, ���������ͬ��,��PID����ͬ�Ĵ���
����ȫ�ֱ���:
�������˵��: char* pidName  -- ������
pid_t tCurPid  -- ��ǰ���̺�
�������˵��:  
����ֵ˵��  : TRUE ����, FALSE ������
--------------------------------------------------------------------------
�޸ļ�¼    :
��  ��       �汾        �޸���        �޸�����
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
������      : ReadFileToString
����        : ���ļ��������뵽string������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/15/             liangli
====================================================================*/
bool ReadFileToString(LPCSTR lpFile, string &strFileContent);

/*====================================================================
������      : WriteFileFromString
����        : string����д���ļ�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : �ɹ�:true��ʧ��:false
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2011/11/15/             liangli
====================================================================*/
bool WriteFileFromString(LPCSTR lpFile, const string &strFileContent, bool bAllowEmpty=true);

/*====================================================================
������      : GetLanDesc
����        : ��ȡ��������
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: strDescID-��������ID���μ�cms_landesc.h
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/11/23/             fanxg
====================================================================*/
string GetLanDesc(const string& strDescID);

/*====================================================================
������      : GetHttpPort SetHttpPort
����        : ��ȡ������ҵ��Http�˿�
�㷨ʵ��    :
����ȫ�ֱ���:
�������˵��: 
����ֵ˵��  : 
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2014/11/26/             huzy
====================================================================*/
unsigned short GetHttpPort();
bool SetHttpPort(unsigned short wPort);


unsigned short GetExtendsMode();
bool SetExtensionMode(unsigned int mode);

#endif

