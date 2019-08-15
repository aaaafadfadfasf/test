#ifndef _KDMSEARCH_H_
#define _KDMSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int m_dwTargetIP;
    unsigned int m_dwNewIP;
    unsigned int m_dwNewMask;
    unsigned int m_dwNewGW;
} TSetNewIpInfo;

// �޸�IP�ص������ṹ��0 -- �ɹ�����0 -- ʧ��
typedef int (*FEvSetDevIp)(void* pUserHandle, TSetNewIpInfo tNewIpInfo);

// �����ص�
typedef void (*FEvReboot)(void* pUserHandle, int nResult);

/************************************************************************/
/* �������ܣ���������ģ��                                               */
/* ����˵����pfunSetIp ���� �޸�IP��ַ�ص�ָ��                          */
/*           pchConfigPath ���� �����ļ�·��                            */
/*                              �磺/usr/config/                        */
/*                              ��c:\d0h\usr\config\                  */
/* ��������ֵ��0 ���� �ɹ�����0 ���� ʧ��                               */
/* ����˵��������ģ������һ�������߳�                                   */
/************************************************************************/
int KdmSearchStartup(void* pUserHandle, FEvSetDevIp pfunSetIp, FEvReboot pfunReboot, char* pchConfigPath);

/************************************************************************/
/* �������ܣ�ֹͣ����ģ��                                               */
/* ����˵������                                                         */
/* ��������ֵ����                                                       */
/* ����˵����                                                           */
/************************************************************************/
void KdmSearchCleanup(void);

#ifdef __cplusplus
}
#endif

#endif // ifndef _KDMSEARCH_H_

/************************************************************************/
/* _280X_ �����ļ���ʽ                                                  */
/* �����ļ�����mdcfg.ini                                                */
/* #��ͷ����Ϊע�ͣ�ע���뵥����һ��                                    */
/************************************************************************/
/*

#����������Ϣ
[NETINFO]
EthNum = 0
#��ʽ
#EthN = ip Netmask Gateway MacAddr
#��û������ʱ������0.0.0.0����


#�豸��Ϣ
[DEVINFO] 
DeviceType = 80
DeviceName = G200
TypeName = G200
SeriesName = G_Serial
MulticastIp = 224.16.32.1
MulitcastPort = 5510

[DEBUGOPTION]
#���Կ��أ�1Ϊ�򿪵��Դ�ӡ��0Ϊ�رա��޸ĺ���Ҫ����ģ��
DebugPrint = 1
#�˿�����ʱ��Ч
DebugLog = 0

*/

/************************************************************************/
/* _280X_ �汾��Ϣ�ļ�                                                  */
/* �ļ�����kdm-search.ver                                               */
/************************************************************************/
/*

Kdm search version: KDM SEARCH 2.20100612, compile time: Jun 27 2010 00:14:34.

*/

/**************************** FILE END *******************************/

