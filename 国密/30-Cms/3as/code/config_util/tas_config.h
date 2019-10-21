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

    /* proxy������� */
    static string GetTasURI();
    static string GetLocalIP4Proxy();
    static u16 GetLocalPort4Proxy();

    /* NMS��������� */
    static string GetNMSIP();
    static u16 GetNMSPort();
    static string GetLocalIP4NMS();

    /* ���ݿ��������� */
    static s32 GetDbType();
    static string GetDbIP();
    static u16 GetDbPort();
    static string GetDbUserName();
    static string GetDbPassword();
    static string GetDbName();
    
    /* License SDK ����������*/
    static void SetLsKeyType(u16 wLsKeyType);
    static u16 GetLsKeyType();
    static void SetLsIp(const string& strLsIp);
    static string GetLsIP();
    static void SetLsPort(u16 wLsPort);
    static u16 GetLsPort();

    /* �߼����� */
    /* ��Ϣ���д�С���� */
    static u32 GetCCtrlAppQueueSize();
    /* ��ȡDB������������ */
    static u32 GetDbTaskItemMaxNum();
    static u32 GetDbTaskThreadNum();
    /* �Ự����������� */
    static u32 GetSessionChangeNtfMaxNum(); // �����Ự�����������֪ͨ��������ֵ
    /* Ĭ�ϵ���־���� */
    static s32 GetDefaultLogLevel();
    /* licenseʹ�ü��ʱ��������λ���� */
    static u32 GetLicenseUsageInterval();
    /* ���������������㼶�Ƿ��Ϊ5�� */
    static bool CanMaxCivilCodeLevUp2Five();

    static u32 GetManagePuMaxNum();
    static void SetManagePuMaxNum(u32 dwManagePuMaxNum);
	static string GetStartTime();
	static void SetStartTime(const string& val);

	/*CU�������*/
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

    static bool s_bCanMaxCivilCodeLevUp2Five;    // ���������������㼶Ĭ��Ϊ4�����Ƿ��Ϊ5��

	static s32 s_nPwdLimitTime;   //�����������������޶�ʱ�䣬��λ���ӣ�Ĭ��1����������3�δ�������5����
	static s32 s_nPwdInputTimes;  //���������������
    static s32 s_nPwdLockedTime;  //���뱻������ʱ��
};

#endif // _TAS_CONFIG_H_
